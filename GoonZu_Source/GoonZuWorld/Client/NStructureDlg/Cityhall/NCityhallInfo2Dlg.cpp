#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Map\FieldObject\FieldObject.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../client/client.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallInfo2Dlg::NCityhallInfo2Dlg()
{
}

NCityhallInfo2Dlg::~NCityhallInfo2Dlg()
{
}

void NCityhallInfo2Dlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( EDIT_STRMONEY, TEXT("editbox_strmoney") );
	SetControlMap( EDIT_PROFIT, TEXT("editbox_profit") );
	SetControlMap( EDIT_CURRENT_PRICE, TEXT("editbox_current_price") );
	SetControlMap( EDIT_TOTAL_STOCK, TEXT("editbox_total_stock") );

	SetControlMap( LIST_DIVIDEND, TEXT("listbox_inoutmoney") );

	SetControlMap( BUTTON_ACCOUNTBOOK, TEXT("button_accountbook") );
	// 리스트
	CList *pList = (CList *)m_pControlMap[ LIST_DIVIDEND ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR *pText[ 4 ];
	pText[ 0 ] = GetTxtFromMgr( 1636 );
	pText[ 1 ] = GetTxtFromMgr( 1637 );
	pText[ 2 ] = GetTxtFromMgr( 1638);
	pText[ 3 ] = GetTxtFromMgr( 1639 );

	pList->SetColumn( 0, 84, pText[ 0 ] );
	pList->SetColumn( 1, 74, pText[ 1 ] );
	pList->SetColumn( 2, 86, pText[ 2 ] );
	pList->SetColumn( 3, 92, pText[ 3 ] );
	pList->Refresh();

	m_bDispDividend = FALSE;

	m_dividendInfo.SetCount( 0 );

	// 배당 내역 요청
	SI32 CharUnique = pCityhallManager->CharUnique;
	SI32 siVillageUnique = pCityhallManager->siVillageUnique;

	SI32 id = 1;
	if(id)
	{
		cltGameMsgRequest_GetDividendList	clDividendList( siVillageUnique );

		cltMsg clMsg( GAMEMSG_REQUEST_GETDIVIDENDLIST, sizeof(cltGameMsgRequest_GetDividendList), (BYTE*)&clDividendList );

		pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
	}

	SetTimerOnOff( true );
	SetActionTimer( 500 );
}

void NCityhallInfo2Dlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	//	char buffer[ 256 ];
	TCHAR buf[ 256 ] = {'\0'};

	TCHAR* pText = GetTxtFromMgr(611);
	// 자본금을 보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCommonInfo.clMoney.itMoney, buf, 256, pText );
	SetEditText( EDIT_STRMONEY, buf );

	// 수익금을 보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.clProfitMoney.itMoney, buf, 256, pText );
	SetEditText( EDIT_PROFIT, buf );


	// 현재주가를  보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siStockPrice, buf, 256, pText );
	SetEditText( EDIT_CURRENT_PRICE, buf );

	// 총발행주식수를 보여준다 
	pText = GetTxtFromMgr(1663);
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siTotalStockAmount, buf, 256, pText );
	SetEditText( EDIT_TOTAL_STOCK, buf );


	// 수익 배당 내역 보여 주기
	CList *pList = (CList *)m_pControlMap[ LIST_DIVIDEND ];

	if(pList == NULL ) return;

	int count = m_dividendInfo.GetCount();

	if( count > 0 && m_bDispDividend == FALSE ) {
		m_bDispDividend = TRUE;

		int		totstock;
		SI64	price;
		cltDate	*pclDate;

		SI32 i;
		TCHAR buffer[256];

		pList->Clear();

		TCHAR* pText = NULL;
		for( i = 0; i < count; i++ )
		{
			stListBoxItemData tmpListItem;
			tmpListItem.Init();

			// 배당일
			pclDate = &m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].clDate;

			pText = GetTxtFromMgr(1508);
			sprintf( buffer, pText, pclDate->uiYear, pclDate->uiMonth, pclDate->uiDay );
			tmpListItem.Set( 0, (TCHAR *)buffer);


			// 총 주식수
			totstock = m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].siTotalStock;
			pText = GetTxtFromMgr(1594);
			sprintf( buffer, pText, totstock );
			tmpListItem.Set( 1, (TCHAR *)buffer);

			// 1주당 배당금액
			price = m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].clDividendPrice.itMoney;
			//pText = GetTxtFromMgr(1640);
			//sprintf( buffer, pText, price );
			//tmpListItem.Set( 2, (TCHAR *)buffer);

			TCHAR		szPrice[64] = { '\0', };
			GMONEY		siPrice = price ;
			g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
			NTCHARString512	kText(GetTxtFromMgr(1640));
			kText.Replace("#money#",szPrice);
			tmpListItem.Set( 2, kText);


			// 총 배당금액

			sprintf( buffer, pText, totstock * price );
			//_i64toa( totstock * price, buffer, 10 );
			tmpListItem.Set( 3, (TCHAR *)buffer);

			pList->AddItem( &tmpListItem );
		}

		pList->Refresh();
	}
}

void NCityhallInfo2Dlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	cltClient *pclclient = (cltClient*)pclClient;

	CASE_CONTROL( BUTTON_ACCOUNTBOOK )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				// kkm
				pclclient->CreateInterface( NACCOUNTBOOKBASE_DLG );
			}
			break;
		}
	}
}

void NCityhallInfo2Dlg::Show()
{
	NStrTabDlg::Show();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKCITYHALLFUND );
}


/*cyj
NCityhallInfo2Dlg::NCityhallInfo2Dlg()
{
}

NCityhallInfo2Dlg::~NCityhallInfo2Dlg()
{
}

void NCityhallInfo2Dlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( EDIT_STRMONEY, TEXT("editbox_strmoney") );
	SetControlMap( EDIT_PROFIT, TEXT("editbox_profit") );
	SetControlMap( EDIT_CURRENT_PRICE, TEXT("editbox_current_price") );
	SetControlMap( EDIT_TOTAL_STOCK, TEXT("editbox_total_stock") );

	SetControlMap( LIST_INOUTMONEY, TEXT("listbox_inoutmoney") );


	// 리스트
	CList *pList = (CList*)m_pControlMap[ LIST_INOUTMONEY ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	// TCHAR *text[] = { TEXT("시간"), TEXT("내용"), TEXT("입출금액"), TEXT("상단잔고") };
	TCHAR* pText[4];
	
	pText[0] = GetTxtFromMgr(1564);
	pText[1] = GetTxtFromMgr(1565);
	pText[2] = GetTxtFromMgr(1566);
	pText[3] = GetTxtFromMgr(1567);

	pList->SetColumn( 0, 104, pText[ 0 ] );
	pList->SetColumn( 1, 116, pText[ 1 ] );
	pList->SetColumn( 2, 116, pText[ 2 ] );	

	pList->Refresh();

	pCityhallManager->UpdateInOutMoneySwitch = FALSE;

}

void NCityhallInfo2Dlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_INOUTMONEY ];
	stListBoxItemData itemdata;

	int i;
	TCHAR buffer[ 256 ];
	TCHAR buf[ 256 ];
	TCHAR buf2[ 128 ];
	

	TCHAR* pText = GetTxtFromMgr(611);
	// 자본금을 보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCommonInfo.clMoney.itMoney, buf, 256, pText );
	SetEditText( EDIT_STRMONEY, buf );

	// 수익금을 보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.clProfitMoney.itMoney, buf, 256, pText );
	SetEditText( EDIT_PROFIT, buf );

	
	// 현재주가를  보여준다 
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siStockPrice, buf, 256, pText );
	SetEditText( EDIT_CURRENT_PRICE, buf );

	// 총발행주식수를 보여준다 
	pText = GetTxtFromMgr(1663);
	g_JYLibFunction.SetNumUnit( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siTotalStockAmount, buf, 256, pText );
	SetEditText( EDIT_TOTAL_STOCK, buf );


	//
	cltDate *pclDate=NULL;
	cltCityHallInOutMoneyUnitInfo *pclUnitInfo;

	int count = pCityhallManager->clClientStrInfo.clCityhallStrInfo.clInOutMoneyInfo.GetCount();

	if( count > 0 && pCityhallManager->UpdateInOutMoneySwitch == FALSE ) {
		pCityhallManager->UpdateInOutMoneySwitch = TRUE;

		pList->Clear();

		for( i = 0; i < count; i++ )
		{
			pclUnitInfo = &pCityhallManager->clClientStrInfo.clCityhallStrInfo.clInOutMoneyInfo.clInOutMoneyUnitInfo[ i ];

			itemdata.Init();
			// 시간 
			pText = GetTxtFromMgr(1508);
			StringCchPrintf( buffer, 256, pText, 
				pclUnitInfo->clDate.uiYear, pclUnitInfo->clDate.uiMonth, pclUnitInfo->clDate.uiDay );
			itemdata.Set( 0, buffer );
			

			// 내용
			buffer[ 0 ] = NULL;
			switch( pclUnitInfo->siInOutCode )
			{
			case INOUTCODE_DIVIDEND:
				pText = GetTxtFromMgr(1568);
				StringCchCopy( buffer, 256, pText );
				break;

			case INOUTCODE_TAX:
				buf2[ 0 ] = NULL;
				if(pclClient->pclVillageStructureSet->GetStructureNameFromRankType(pclUnitInfo->siStrType, buf2, 128 ) == true)
				{
					pText = GetTxtFromMgr(1569);
					StringCchPrintf( buffer, 256, pText, buf2 );	
				}
				break;

			case INOUTCODE_NPCPAY:
				StringCchCopy( buf2, 128, pclClient->pclKindInfoSet->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1570);
				StringCchPrintf( buffer, 256, pText, buf2 );
				break;

			case INOUTCODE_STOCK:
				pText = GetTxtFromMgr(1571);
				StringCchPrintf( buffer, 256, pText );
				break;

			case INOUTCODE_GODSTAT:
				pText = GetTxtFromMgr(1572);
				StringCchPrintf( buffer, 256, pText );
				break;

			case INOUTCODE_CASTLESTAT:
				pText = GetTxtFromMgr(1573);
				StringCchPrintf( buffer, 256, pText );
				break;
				
			case INOUTCODE_TOPARENT:
				StringCchCopy( buf2, 128, pclClient->pclVillageManager->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1574);
				StringCchPrintf( buffer, 256, pText, buf2 );
				break;

			case INOUTCODE_FROMCHILD:
				StringCchCopy( buf2, 128, pclClient->pclVillageManager->GetName( pclUnitInfo->siStrType ) );
				pText = GetTxtFromMgr(1575);
				StringCchPrintf( buffer, 256, pText, buf2 );
				break;

			case INOUTCODE_EVENTPAY:
				pText = GetTxtFromMgr(1576);
				StringCchCopy( buffer, 256, pText);
				break;

			case INOUTCODE_VILLAGEMARK:
				pText = GetTxtFromMgr(1577);
				StringCchCopy( buffer, 256, pText);
				break;

			case INOUTCODE_RESIDENTSHARE:
				pText = GetTxtFromMgr(1578);
				StringCchCopy( buffer, 256, pText);
				break;

			case INOUTCODE_VILLAGEMAP:
				pText = GetTxtFromMgr(1579);
				StringCchCopy( buffer, 256, pText );
				break;
			case INOUTCODE_INSTANCEPROFIT:
				pText = GetTxtFromMgr(5321);
				StringCchCopy( buffer, 256, pText );
				break;
			case INOUTCODE_DESTROYSTR:
				pText = GetTxtFromMgr(5617);
				StringCchCopy( buffer, 256, pText );
				break;
			case INOUTCODE_NATIONTAX:
				pText = GetTxtFromMgr(6016);
				strcpy( buffer, pText );
				break;
			case INOUTCODE_SETBASEMONEY:
				pText = GetTxtFromMgr(6017);
				strcpy( buffer, pText );
				break;

			}

			itemdata.Set( 1, buffer );

			// 입출금액
			pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit( pclUnitInfo->clInOutMoney.itMoney, buffer, 256, pText );
			
			itemdata.Set( 2, buffer );

			pList->AddItem( &itemdata );
		}				

		pList->Refresh();
	}
}

void NCityhallInfo2Dlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
}
*/