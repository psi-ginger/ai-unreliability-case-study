#include "NStockDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"


#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Stock.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"

#include <tchar.h>

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStockUser2Dlg::NStockUser2Dlg()
{
	m_siSelectVillageIndex_ = 0;
}

NStockUser2Dlg::~NStockUser2Dlg()
{
}

void NStockUser2Dlg::InitDlg()
{
	SetControlMap( COMBO_BUYER_TYPE, TEXT("combobox_buyer_type") );
	SetControlMap( COMBO_VILLAGE, TEXT("combobox_village") );

	SetControlMap( EDIT_DATE, TEXT("editbox_date") );
	SetControlMap( EDIT_TOTAL_INCREASESTOCK_AMOUNT, TEXT("editbox_increasestock_amount") );
	SetControlMap( EDIT_LEFTSTOCK_AMOUNT, TEXT("editbox_leftstock_amount") );
	SetControlMap( EDIT_CURRENT_STOCKPRICE, TEXT("editbox_current_stockprice") );
	SetControlMap( EDIT_BUYSTOCK_AMOUNT, TEXT("editbox_buystock_amount") );
	SetControlMap( EDIT_LIMITBUYSTOCK_AMOUNT, TEXT("editbox_limitbuystock_amount") );
	SetControlMap( EDIT_STARTDAY, TEXT("editbox_startday") );

	SetControlMap( BUTTON_BUYSTOCK, TEXT("button_buystock") );

	SetControlMap( LIST_STOCKHOLDER, TEXT("listbox_stockbidder") );

	//[김영훈 추가:2007.11.26 - 주식수 입력란에 숫자만 입력되도록 수정]
	((CEdit*)m_pControlMap[ EDIT_BUYSTOCK_AMOUNT ])->SetNumberMode( true );


	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

	// 증자 마을 리스트를 초기화한다 
	pStockManager->InitStockBidVillage();

	CComboBox	*pCombo;	
	pCombo = (CComboBox*)m_pControlMap[ COMBO_BUYER_TYPE ];

	stComboBoxItemData itemcombo;

	TCHAR *ptext = GetTxtFromMgr(5999);
	itemcombo.Set( ptext );
	pCombo->AddItem( &itemcombo );

	ptext = GetTxtFromMgr(1214);
	itemcombo.Set( ptext );
	pCombo->AddItem( &itemcombo );

	pCombo->Refresh();

	pCombo->SetCursel( 0 );

	// 상위 주주명부
	CList *pList = (CList *)m_pControlMap[ LIST_STOCKHOLDER ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	//	char *text[] = { "아이디", "보유주식" };
	char* pText[2];
	pText[0] = GetTxtFromMgr(1473);
	pText[1] = GetTxtFromMgr(1593);

	pList->SetColumn( 0, 140, pText[ 0 ] );
	pList->SetColumn( 1, 140, pText[ 1 ] );
	pList->Refresh();

	SI32 id = 1;
	if(id)
	{
		// 서버에 증자 마을 리스트 요청	
		cltMsg clMsg( GAMEMSG_REQUEST_STOCKBIDVILLAGELIST, 0 , NULL);

		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// 서버에 증자 마을 리스트 요청	
		cltMsg clMsg2( GAMEMSG_REQUEST_MYSTOCKINFO, 0 , NULL);

		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);
	}
}

void NStockUser2Dlg::Action()
{
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

	if( GetTickCount() - pStockManager->dwLastExplainTick > 6000 )
	{
		char* pText = GetTxtFromMgr(1476);
		SetMainExplain( pText );
	}

	char buffer[256] = {'\0'};

	CComboBox	*pCombo;	
	pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGE ];

	stComboBoxItemData itemcombo;

	// 증자 마을 업데이트 
	if( pStockManager->UpdateStockBidVillageSwitch )
	{
		pStockManager->UpdateStockBidVillageSwitch = FALSE;

		SetEditText( EDIT_DATE, TEXT("") );
		SetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT, TEXT(""));

		// 남은 주식
		SetEditText( EDIT_LEFTSTOCK_AMOUNT, TEXT("") );

		SetEditText( EDIT_CURRENT_STOCKPRICE, TEXT(""));

		SetEditText( EDIT_LIMITBUYSTOCK_AMOUNT, TEXT(""));

		CList *pList = (CList*)m_pControlMap[ LIST_STOCKHOLDER ];
		pList->Clear();
		pList->Refresh();

		pCombo->Clear();

		for( int i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if( pStockManager->clStockBidVillageInfo[ i ].siLeftIncreaseStockAmount_ > 0 )
			{
				itemcombo.Init();
				itemcombo.Set( pclClient->pclVillageManager->pclVillageInfo[i]->szName );

				pCombo->AddItem( &itemcombo );
			}
		}

		NComboBoxSort( pCombo );
	}

	// 증자 중일 경우 남은 시간 업데이트 
	// 선택한 마을의 이름 확보 
	char *szSelText;
	m_siSelectVillageIndex_ = pCombo->GetSelectedIndex();

	SI32 villageunique = 0;
	if( m_siSelectVillageIndex_ >= 0 )
	{
		szSelText = pCombo->GetText( m_siSelectVillageIndex_ );

		// 마을의 유니크를 확보한다. 
		villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
		if( villageunique <= 0 ) return;

		cltDate *pclCurrentDate = &((cltClient *)pclClient)->clClientDate;

		cltDate clEndDate;
		cltDate clLeftDate;

		CComboBox	*pComboType;	
		pComboType = (CComboBox*)m_pControlMap[ COMBO_BUYER_TYPE ];

		// 주주다
		if( 0 == pComboType->GetSelectedIndex() )
		{
			TCHAR *pText = GetTxtFromMgr(1644);
			sprintf( buffer, pText, pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiYear, 
				pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiMonth,
				pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiDay );
			SetEditText( EDIT_STARTDAY, buffer );

			cltDate clCheckDate;
			clCheckDate.Set(&pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_);
			clCheckDate.AddMonth( 3 );

			if( clCheckDate.IsLaterDayThan( pclCurrentDate ) == FALSE ) 
			{
				SetEditText( EDIT_DATE, "" );

				sprintf( buffer, buffer, pText, pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiYear, 
					pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiMonth,
					pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiDay );
				SetEditText( EDIT_STARTDAY, buffer );

				// 총 증자 주식수
				SetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT, TEXT(""));

				// 남은 주식 테스트는 그냥 100
				SetEditText( EDIT_LEFTSTOCK_AMOUNT, TEXT("") );

				SetEditText( EDIT_CURRENT_STOCKPRICE, TEXT(""));

				SetEditText( EDIT_LIMITBUYSTOCK_AMOUNT, TEXT(""));

				CButton	*pCombo = (CButton*)m_pControlMap[ BUTTON_BUYSTOCK ];
				pCombo->Enable( false );
			}
			else
			{
				clEndDate.Set( &pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_ );
				clEndDate.AddMonth( 3 );

				clLeftDate.DiffDate( &clEndDate, pclCurrentDate);

				char* pText = GetTxtFromMgr(6015);
				if( clLeftDate.uiMonth == 1 && clLeftDate.uiDay == 0 && clLeftDate.uiHour == 0 )
				{
					sprintf( buffer, pText, 0, 30, 0 );
				} 
				else
				{
					sprintf( buffer, pText, clLeftDate.uiMonth, clLeftDate.uiDay, clLeftDate.uiHour );
				}

				SetEditText( EDIT_DATE, buffer );

				// 총 증자 주식수
				sprintf( buffer, "%d", pStockManager->clStockBidVillageInfo[ villageunique ].siTottalIncreaseStockAmount_ );
				SetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT, buffer);

				// 남은 주식 테스트는 그냥 100
				sprintf( buffer, "%d", pStockManager->clStockBidVillageInfo[ villageunique ].siLeftIncreaseStockAmount_ );
				SetEditText( EDIT_LEFTSTOCK_AMOUNT, buffer );

				sprintf( buffer, "%I64d", pStockManager->clStockBidVillageInfo[ villageunique ].siCurrentStockPrice_ );
				SetEditText( EDIT_CURRENT_STOCKPRICE, buffer);

				cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

				SI32 tempBuyStockAmount = 0;
				SI32 tempHaveStockAmount = 0;

				for( SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
				{
					if( villageunique == pclchar->pclCI->clStock.clInfo.clUnit[i].siVillageUnique )
					{
						tempHaveStockAmount = pclchar->pclCI->clStock.clInfo.clUnit[i].siAmount;
						tempBuyStockAmount = pclchar->pclCI->clStock.clInfo.clUnit[i].siLimitBuyBidStockAmount;
					}
				}

				sprintf( buffer, "%d", tempBuyStockAmount );
				SetEditText( EDIT_LIMITBUYSTOCK_AMOUNT, buffer);

				CButton	*pCombo = (CButton*)m_pControlMap[ BUTTON_BUYSTOCK ];
				pCombo->Enable( true );
			}
		}
		else if( 1 == pComboType->GetSelectedIndex())
		{
			TCHAR *pText = GetTxtFromMgr(1644);
			sprintf( buffer, pText, pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiYear, 
				pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiMonth,
				pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_.uiDay );
			SetEditText( EDIT_STARTDAY, buffer );

			cltDate clCheckDate;
			clCheckDate.Set(&pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_);
			clCheckDate.AddMonth( 3 );

			if( clCheckDate.IsLaterDayThan( pclCurrentDate ) == TRUE ) 
			{
				SetEditText( EDIT_DATE, "" );

				// 총 증자 주식수
				SetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT, "");

				// 남은 주식 테스트는 그냥 100
				SetEditText( EDIT_LEFTSTOCK_AMOUNT, "" );

				SetEditText( EDIT_CURRENT_STOCKPRICE, "");

				SetEditText( EDIT_LIMITBUYSTOCK_AMOUNT, "");

				CButton	*pCombo = (CButton*)m_pControlMap[ BUTTON_BUYSTOCK ];
				pCombo->Enable( false );
			}
			else
			{
				clEndDate.Set( &pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate_ );
				clEndDate.AddMonth( 6 );
				clLeftDate.DiffDate( &clEndDate, pclCurrentDate);

				char* pText = GetTxtFromMgr(6015);
				if( clLeftDate.uiMonth == 1 && clLeftDate.uiDay == 0 && clLeftDate.uiHour == 0 )
				{
					sprintf( buffer, pText, 0, 30, 0 );
				} 
				else
				{
					sprintf( buffer, pText, clLeftDate.uiMonth, clLeftDate.uiDay, clLeftDate.uiHour );
				}

				SetEditText( EDIT_DATE, buffer );

				// 총 증자 주식수
				sprintf( buffer, "%d", pStockManager->clStockBidVillageInfo[ villageunique ].siTottalIncreaseStockAmount_ );
				SetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT, buffer);

				// 남은 주식 
				sprintf( buffer, "%d", pStockManager->clStockBidVillageInfo[ villageunique ].siLeftIncreaseStockAmount_ );
				SetEditText( EDIT_LEFTSTOCK_AMOUNT, buffer );

				sprintf( buffer, "%I64d", pStockManager->clStockBidVillageInfo[ villageunique ].siCurrentStockPrice_ );
				SetEditText( EDIT_CURRENT_STOCKPRICE, buffer);

				cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

				SI32 tempBuyStockAmount = 0;
				SI32 tempHaveStockAmount = 0;

				for( SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
				{
					if( villageunique == pclchar->pclCI->clStock.clInfo.clUnit[i].siVillageUnique )
						tempHaveStockAmount = pclchar->pclCI->clStock.clInfo.clUnit[i].siAmount;
				}

				sprintf( buffer, "%d", pStockManager->clStockBidVillageInfo[ villageunique ].siLeftIncreaseStockAmount_ );
				SetEditText( EDIT_LIMITBUYSTOCK_AMOUNT, buffer);

				CButton	*pCombo = (CButton*)m_pControlMap[ BUTTON_BUYSTOCK ];
				pCombo->Enable( true );
			}
		}
	}

	if( pStockManager->UpdateInvestSwitch == TRUE )
	{
		pStockManager->UpdateInvestSwitch = FALSE;
		CList *pList = (CList*)m_pControlMap[ LIST_STOCKHOLDER ];
		stListBoxItemData itemdata;

		char buffer[256] = {'\0'};

		pList->Clear();

		if( villageunique < 0 )return;

		if( pclClient->pclVillageManager->pclVillageInfo[ villageunique ] == NULL ) return;

		for( int i = 0;i < MAX_INVEST_NUMBER;i++)
		{
			SI32 personid		= pclClient->pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clInvestInfo.clInvestInfo[i].clPerson.GetPersonID();
			char* pname			= (char *)pclClient->pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clInvestInfo.clInvestInfo[i].clPerson.GetName();
			SI32  stock			= pclClient->pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clInvestInfo.clInvestInfo[i].GetStockNum();

			if( stock == 0 ) continue;

			itemdata.Init();
			itemdata.Set( 0, pname );

			char* pText = GetTxtFromMgr(1594);
			wsprintf(buffer, pText, stock );

			itemdata.Set( 1, buffer );
			pList->AddItem( &itemdata );
		}
		pList->Refresh();
	}
}

void NStockUser2Dlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	//if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	//	char buffer[ 256 ];
	//	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

	// 주주인가? 전체유저인가?
	CASE_CONTROL( COMBO_BUYER_TYPE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				CComboBox	*pCombo = (CComboBox*)m_pControlMap[ COMBO_BUYER_TYPE ];

				SI16 tempSelectIndex = pCombo->GetSelectedIndex();

				if( tempSelectIndex < 0 )
					return;

				SI32 tempSelectVillageUnique = pclClient->pclVillageManager->FindVillageUniqueFromName( pCombo->GetText( tempSelectIndex ) );
				if( tempSelectVillageUnique < 0 )
					return;
				// 서버에 투자자 리스트를 요청한다. 

				cltGameMsgRequest_Investlist pclInfo( tempSelectVillageUnique );
				cltMsg clMsg2(GAMEMSG_REQUEST_INVESTLIST, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg2);
			}
			break;
		}
	}
	// 현재 선택한 마을의 주주명부 보여주기
	else CASE_CONTROL( COMBO_VILLAGE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				CComboBox	*pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGE ];

				m_siSelectVillageIndex_ = pCombo->GetSelectedIndex();

				if( m_siSelectVillageIndex_ < 0 )
					return;

				SI32 tempSelectVillageUnique = pclClient->pclVillageManager->FindVillageUniqueFromName( pCombo->GetText( m_siSelectVillageIndex_ ) );
				if( tempSelectVillageUnique < 0 )
					return;
				// 서버에 투자자 리스트를 요청한다. 

				cltGameMsgRequest_Investlist pclInfo( tempSelectVillageUnique );
				cltMsg clMsg2(GAMEMSG_REQUEST_INVESTLIST, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg2);
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_BUYSTOCK )
	{
		// 사용자 유효성 검사. 
		SI32 id = 1;
		if(id <= 0) return;

		// 선택한 마을의 이름을 확보한다. 
		SI32 amount = 0;
		SI32 limitamount = 0;
		SI32 totalamount = 0;
		SI64 price = 0;

		CComboBox	*pCombo = (CComboBox*)m_pControlMap[ COMBO_VILLAGE ];
		int index = pCombo->GetSelectedIndex();
		if( index < 0 )
			return;

		char *szSelText = pCombo->GetText( index );

		price = _atoi64( GetEditText( EDIT_CURRENT_STOCKPRICE ) );
		amount = atoi( GetEditText( EDIT_BUYSTOCK_AMOUNT ) );
		limitamount = atoi( GetEditText( EDIT_LIMITBUYSTOCK_AMOUNT ) );

		if( price <= 0 )
		{
			char* pText = GetTxtFromMgr(1478);
			SetMainExplain( pText );
			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
			return;
		}

		if( amount <= 0 )
		{
			char* pText = GetTxtFromMgr(1480);
			SetMainExplain( pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}


		
		if( amount > limitamount )
		{
			char* pText = GetTxtFromMgr(6008);
			SetMainExplain( pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}

		totalamount = atoi( GetEditText( EDIT_TOTAL_INCREASESTOCK_AMOUNT ) );

		if( amount > totalamount )
		{
			char* pText = GetTxtFromMgr(1481);
			SetMainExplain( pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}

		SI32 FindIndex = 0;
		// 마을의 유니크를 확보한다. 
		SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
		if( villageunique <= 0 )
		{
			char* pText = GetTxtFromMgr(1482);
			SetMainExplain( pText );
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}

		CComboBox	*pComboBuyer = (CComboBox*)m_pControlMap[ COMBO_BUYER_TYPE ];

		SI16 tempSelectIndex = pComboBuyer->GetSelectedIndex();

		if( tempSelectIndex < 0 || tempSelectIndex > 1)
			return;

		if( 0 ==  tempSelectIndex )
		{
			for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
			{
				// 원하는 종목을 찾으면, 
				if(villageunique == pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique)
				{
					FindIndex = i;
					break;
				}
			}

			if( pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[FindIndex].bBuySwitch == true )
			{
				char* pText = GetTxtFromMgr(6009);
				SetMainExplain( pText );
				return;
			}
		}

		char* pText = GetTxtFromMgr(1484);
		char* pTitle = GetTxtFromMgr(1485);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 10 );
		char TempBuffer[ 128 ] = "";
		SI16 Size = sizeof( id ) + sizeof( price ) + sizeof(amount) + sizeof(villageunique) ;
		memcpy( TempBuffer, &id, sizeof( id ) );
		memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) ], &amount, sizeof( amount ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) + sizeof(amount)], &villageunique, sizeof( villageunique ) );
		pclclient->SetMsgBox( &MsgBox, (BYTE*)TempBuffer, Size );
	}
}

void NStockUser2Dlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 10:	// 증자 주식 사기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI64 price = *((SI64*)(pData + 4));
				int amount = *((int*)(pData + 12 ));
				SI32 villageunique = *((SI32*)(pData + 16 )); 

				cltMoney priceMoney;
				priceMoney.Set( price );

				CComboBox	*pCombo = (CComboBox*)m_pControlMap[ COMBO_BUYER_TYPE ];

				SI16 tempSelectIndex = pCombo->GetSelectedIndex();

				if( tempSelectIndex < 0 || tempSelectIndex > 1)
					return;

				cltGameMsgRequest_BuyBidStock clInfo( tempSelectIndex, villageunique, amount, &priceMoney );

				cltMsg clMsg(GAMEMSG_REQUEST_BUYBIDSTOCK, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);				

				CEdit	*pEdit = (CEdit*)m_pControlMap[ EDIT_BUYSTOCK_AMOUNT ];
				pEdit->SetText("");
			}
		}
		break;
	}
}

// _LEON_MADE_NCOMBOBOX_SORT
void NStockUser2Dlg::NComboBoxSort(CComboBox* in_pCombo )
{
	if( in_pCombo == NULL ) return;

	stComboBoxItemData	*tmpComboBoxItemDatas = new stComboBoxItemData[ MAX_COMBOBOX_ITEM_NUM ];
	stComboBoxItemData	*pTmpComboBoxItemData = NULL;
	SI16 siCount = 0;

	SI32 i;

	for( i= 0; i<MAX_COMBOBOX_ITEM_NUM; i++)
	{
		tmpComboBoxItemDatas[i].Init();
	}

	pTmpComboBoxItemData = in_pCombo->GetItemData( 0 );
	while( pTmpComboBoxItemData != NULL )
	{
		memcpy(tmpComboBoxItemDatas + siCount, pTmpComboBoxItemData, sizeof(stComboBoxItemData));
		siCount++;
		if(siCount >= MAX_COMBOBOX_ITEM_NUM) break;
		pTmpComboBoxItemData = in_pCombo->GetItemData( siCount );
	}

	//	char buffer[256];
	char text[5000] ="";

	SI32 clearnum	= 0;
	SI32 totalnum	= 0;
	//데이터를 갱신하기 전에 깨끗이 지운다.
	in_pCombo->Clear();

	SI32  siSortIndexByName[ MAX_COMBOBOX_ITEM_NUM ];

	memset( siSortIndexByName, -1, sizeof(siSortIndexByName));

	for( i = 0; i < siCount;i++ )
	{
		siSortIndexByName[i] = i;
	}

	//  i-1로 했으므로 0부터..
	for( i = 0; i < siCount; i++ )
	{
		if( siSortIndexByName[ 0 ] < 0  ) continue;
		const char* pBuffer =  tmpComboBoxItemDatas[ siSortIndexByName[ 0 ] ].strText;

		for (SI32 j=1; j<siCount - i; j++)
		{
			if( siSortIndexByName[ j ] < 0 ) continue;
			const char* pname =  tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
			if( _tcscmp( pBuffer, pname) > 0 )
			{
				SI32 siTmp = 0;
				siTmp = siSortIndexByName[ j-1 ];
				siSortIndexByName[ j-1 ] = siSortIndexByName[ j ];
				siSortIndexByName[ j ] = siTmp;
			}

			pBuffer = tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
		}
	}

	for(i = 0; i < siCount; i++)
	{
		if( siSortIndexByName[ i ] < 0 ) continue;

		in_pCombo->AddItem( &(tmpComboBoxItemDatas[ siSortIndexByName[ i ] ]) );
	}

	delete [] tmpComboBoxItemDatas;

	in_pCombo->Refresh();
}
/*
#else
NStockUser2Dlg::NStockUser2Dlg()
{
}

NStockUser2Dlg::~NStockUser2Dlg()
{
}

void NStockUser2Dlg::InitDlg()
{
SetControlMap( COMBO_STOCKBID_ITEM, "combobox_stockbid_item" );

SetControlMap( EDIT_TOTAL_AMOUNT, "editbox_total_amount" );
SetControlMap( EDIT_BEGIN_PRICE, "editbox_begin_price" );

SetControlMap( EDIT_AMOUNT, "editbox_amount" );
SetControlMap( EDIT_PRICE, "editbox_price" );

SetControlMap( EDIT_DATE, "editbox_date" );

SetControlMap( BUTTON_BIDSTOCK, "button_bidstock" );
SetControlMap( BUTTON_REFRESH_LIST, "button_refresh_list" );

SetControlMap( LIST_STOCKBIDDER, "listbox_stockbidder" );


cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

// 증자 마을 리스트를 초기화한다 
pStockManager->InitStockBidVillage();

CList *pList;

// 보유 주식 현황
pList = (CList *)m_pControlMap[ LIST_STOCKBIDDER ];

pList->SetBorder(true);
pList->SetBKColorUse(true);

char* pText = GetTxtFromMgr(3449);
pList->SetColumn( 0, 116, pText );
pText = GetTxtFromMgr(3450);
pList->SetColumn( 1, 116, pText );
pText = GetTxtFromMgr(3451);
pList->SetColumn( 2, 100, pText );
pList->Refresh();

SI32 id = 1;

if( id ) {

cltMsg clMsg( GAMEMSG_REQUEST_STOCKBIDVILLAGELIST, 0 );

pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}


}

void NStockUser2Dlg::Action()
{
char buffer[256];

cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

CComboBox	*pCombo;	
pCombo = (CComboBox*)m_pControlMap[ COMBO_STOCKBID_ITEM ];

stComboBoxItemData itemcombo;

if( GetTickCount() - pStockManager->dwLastExplainTick > 6000 ) {
char* pText = GetTxtFromMgr(1476);
SetMainExplain( pText );
}


// 증자 마을 업데이트 
if( pStockManager->UpdateStockBidVillageSwitch ) {

pStockManager->UpdateStockBidVillageSwitch = FALSE;

pCombo->Clear();

for( int i = 0;i < MAX_VILLAGE_NUMBER;i++)
{
if( pStockManager->clStockBidVillageInfo[ i ].siVillageUnique )
{

itemcombo.Init();
itemcombo.Set( pclClient->pclVillageManager->pclVillageInfo[i]->szName );

pCombo->AddItem( &itemcombo );
}
}

NComboBoxSort( pCombo );

}

CList *pList;

pList = (CList*)m_pControlMap[ LIST_STOCKBIDDER ];
stListBoxItemData itemlist;

// 입찰자 리스트 업데이트
if( pStockManager->UpdateStockBidderSwitch ) {
pStockManager->UpdateStockBidderSwitch = FALSE;

pList->Clear();

for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; i++ )
{

if( pStockManager->clClientStockBidder[ i ].clPerson.GetPersonID() == 0 ) continue;

// 아이디 
itemlist.Init();
itemlist.Set(0, (char*)pStockManager->clClientStockBidder[ i ].clPerson.GetName() );

// 주식 가격 
_i64toa( pStockManager->clClientStockBidder[ i ].siPrice, buffer, 10 );
itemlist.Set( 1, buffer );

// 주식 수
itoa( pStockManager->clClientStockBidder[ i ].siAmount, buffer, 10 );
itemlist.Set( 2, buffer );

pList->AddItem( &itemlist );
}

pList->Refresh();

}

// 증자 중일 경우 남은 시간 업데이트 
{

// 선택한 마을의 이름 확보 
char *szSelText;
int index = pCombo->GetSelectedIndex();

if( index >= 0 ) {

szSelText = pCombo->GetText( index );

// 마을의 유니크를 확보한다. 
SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
if( villageunique <= 0 ) return;

cltDate *pclCurrentDate = &((cltClient *)pclClient)->clClientDate;

cltDate clEndDate;
cltDate clLeftDate;

clEndDate.Set( &pStockManager->clStockBidVillageInfo[ villageunique ].clBiddingDate );

clEndDate.AddMonth( 1 );

clLeftDate.DiffDate( &clEndDate, pclCurrentDate);

char* pText = GetTxtFromMgr(6015);
if( clLeftDate.uiMonth == 1 && clLeftDate.uiDay == 0 && clLeftDate.uiHour == 0 ) {
sprintf( buffer, pText, 30, 0 );
} else {
sprintf( buffer, pText, clLeftDate.uiDay, clLeftDate.uiHour );
}

SetEditText( EDIT_DATE, buffer );
}

}


}

void NStockUser2Dlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
cltClient *pclclient = (cltClient*)pclClient;

//if( m_pBaseDlg->m_dwActionCount % 3 ) return;

char buffer[ 256 ];
cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

CComboBox	*pCombo;	
pCombo = (CComboBox*)m_pControlMap[ COMBO_STOCKBID_ITEM ];

CASE_CONTROL( COMBO_STOCKBID_ITEM )
{
switch( nEvent )
{
case EVENT_COMBOBOX_SELECTION_CHANGED:
{

// 사용자 유효성 검사. 
SI32 id = 1;
if(id <= 0) return;

CList *pList = (CList*)m_pControlMap[ LIST_STOCKBIDDER ];
pList->Clear();
pList->Refresh();


// 선택한 마을의 이름을 확보한다. 
char szBuf[ 256 ] = {'\0'};
char *szSelText = pCombo->GetText( pCombo->GetSelectedIndex() );;

// 마을의 유니크를 확보한다. 
SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
if( villageunique <= 0 )break;						

if( pStockManager->clStockBidVillageInfo[ villageunique ].siVillageUnique == 0 ) break;

// 증자 주식수 
itoa( pStockManager->clStockBidVillageInfo[ villageunique ].siStockAmount, szBuf, 10 );
SetEditText( EDIT_TOTAL_AMOUNT, szBuf );

// 시작가 
//	_i64toa( pStockManager->clStockBidVillageInfo[ villageunique ].siBeginningPrice, szBuf, 10 );
sprintf( szBuf, "%I64d", pStockManager->clStockBidVillageInfo[ villageunique ].siBeginningPrice );
SetEditText( EDIT_BEGIN_PRICE, szBuf );

// 낙찰 희망 주식수 
itoa( pStockManager->clStockBidVillageInfo[ villageunique ].siStockAmount, szBuf, 10 );
SetEditText( EDIT_AMOUNT, szBuf );


// 입찰자 리스트 요청 
cltMsg	clMsg( GAMEMSG_REQUEST_STOCKBIDDERLIST, villageunique );
pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}
break;
}
}

else CASE_CONTROL( BUTTON_BIDSTOCK )
{
// 사용자 유효성 검사. 
SI32 id = 1;
if(id <= 0) return;

// 선택한 마을의 이름을 확보한다. 
int amount = 0;
SI64 price = 0;

int index = pCombo->GetSelectedIndex();
if( index < 0 )
return;

char *szSelText = pCombo->GetText( index );

price = _atoi64( GetEditText( EDIT_PRICE ) );
amount = atoi( GetEditText( EDIT_AMOUNT ) );

if( price <= 0 ) {
char* pText = GetTxtFromMgr(1478);
SetMainExplain( pText );
pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
return;
}

SI64 beginningPrice = _atoi64( GetEditText( EDIT_BEGIN_PRICE ) );
int totalamount = atoi( GetEditText( EDIT_TOTAL_AMOUNT ) );

if( price <  beginningPrice ) {
char* pText = GetTxtFromMgr(1479);
sprintf( buffer, pText, beginningPrice );

SetMainExplain( buffer );
pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
return;

}

if( amount <= 0 ) {
char* pText = GetTxtFromMgr(1480);
SetMainExplain( pText);
pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
return;
}

if( amount > totalamount ) {
char* pText = GetTxtFromMgr(1481);
SetMainExplain( pText);
pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
return;

}

// 마을의 유니크를 확보한다. 
SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
if( villageunique <= 0 ) {
char* pText = GetTxtFromMgr(1482);
SetMainExplain( pText );
pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
return;
}

if( price % 100 ) {
char* pText = GetTxtFromMgr(1483);
SetMainExplain( pText );
pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
return;;
}

char* pText = GetTxtFromMgr(1484);
char* pTitle = GetTxtFromMgr(1485);

stMsgBox MsgBox;
// 현재 dialog가 탭 창 같은 Child Dialog일때
CControllerMgr *pParent = NULL;
pParent = this->GetParentDialog();
if(pParent == NULL ) return;
MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 10 );
char TempBuffer[ 128 ] = "";
SI16 Size = sizeof( id ) + sizeof( price ) + sizeof(amount) + sizeof(villageunique) ;
memcpy( TempBuffer, &id, sizeof( id ) );
memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) ], &amount, sizeof( amount ) );
memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) + sizeof(amount)], &villageunique, sizeof( villageunique ) );
pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
}

else CASE_CONTROL( BUTTON_REFRESH_LIST )
{
// 사용자 유효성 검사. 
SI32 id = 1;
if(id <= 0) return;

// 선택한 마을의 이름을 확보한다. 
char *szSelText;

int index = pCombo->GetSelectedIndex();
if( index < 0 ) return;

szSelText = pCombo->GetText( index );

// 마을의 유니크를 확보한다. 
SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
if( villageunique <= 0 )return;						

cltMsg	clMsg( GAMEMSG_REQUEST_STOCKBIDDERLIST, villageunique );
pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

}

void NStockUser2Dlg::RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen )
{
switch( MsgIndex )
{
case 10:	// 주식 입찰 선정
{
if ( pData == NULL || DataLen < 1 )
{
return;
}

if ( RetValue )
{
SI32 id = *((SI32*)pData);
SI64 price = *((SI64*)(pData + 4));
int amount = *((int*)(pData + 12 ));
SI32 villageunique = *((SI32*)(pData + 16 )); 

cltMoney priceMoney;
priceMoney.Set( price );

cltGameMsgRequest_BidStock clInfo( villageunique, amount, &priceMoney );

cltMsg clMsg(GAMEMSG_REQUEST_BIDSTOCK, sizeof(clInfo), (char*)&clInfo);
pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);				
}
}
break;
}
}

// _LEON_MADE_NCOMBOBOX_SORT
void NStockUser2Dlg::NComboBoxSort(CComboBox* in_pCombo )
{
if( in_pCombo == NULL ) return;

stComboBoxItemData	*tmpComboBoxItemDatas = new stComboBoxItemData[ MAX_COMBOBOX_ITEM_NUM ];
stComboBoxItemData	*pTmpComboBoxItemData = NULL;
SI16 siCount = 0;

SI32 i;

for( i= 0; i<MAX_COMBOBOX_ITEM_NUM; i++)
{
tmpComboBoxItemDatas[i].Init();
}

pTmpComboBoxItemData = in_pCombo->GetItemData( 0 );
while( pTmpComboBoxItemData != NULL )
{
memcpy(tmpComboBoxItemDatas + siCount, pTmpComboBoxItemData, sizeof(stComboBoxItemData));
siCount++;
if(siCount >= MAX_COMBOBOX_ITEM_NUM) break;
pTmpComboBoxItemData = in_pCombo->GetItemData( siCount );
}

//	char buffer[256];
char text[5000] ="";

SI32 clearnum	= 0;
SI32 totalnum	= 0;
//데이터를 갱신하기 전에 깨끗이 지운다.
in_pCombo->Clear();

SI32  siSortIndexByName[ MAX_COMBOBOX_ITEM_NUM ];

memset( siSortIndexByName, -1, sizeof(siSortIndexByName));

for( i = 0; i < siCount;i++ )
{
siSortIndexByName[i] = i;
}

//  i-1로 했으므로 0부터..
for( i = 0; i < siCount; i++ )
{
if( siSortIndexByName[ 0 ] < 0  ) continue;
const char* pBuffer =  tmpComboBoxItemDatas[ siSortIndexByName[ 0 ] ].strText;

for (SI32 j=1; j<siCount - i; j++)
{
if( siSortIndexByName[ j ] < 0 ) continue;
const char* pname =  tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
if( _tcscmp( pBuffer, pname) > 0 )
{
SI32 siTmp = 0;
siTmp = siSortIndexByName[ j-1 ];
siSortIndexByName[ j-1 ] = siSortIndexByName[ j ];
siSortIndexByName[ j ] = siTmp;
}

pBuffer = tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
}
}

for(i = 0; i < siCount; i++)
{
if( siSortIndexByName[ i ] < 0 ) continue;

in_pCombo->AddItem( &(tmpComboBoxItemDatas[ siSortIndexByName[ i ] ]) );
}

delete [] tmpComboBoxItemDatas;

in_pCombo->Refresh();

}
#endif*/
