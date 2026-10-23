#include "NRealEstateMarketDlg.h"

#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-RealEstateMarket.h"

#include "../../CommonLogic/House/House.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"

#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;

NRealEstateMarketUserDlg::NRealEstateMarketUserDlg()
{

}


NRealEstateMarketUserDlg::~NRealEstateMarketUserDlg()
{

}


void NRealEstateMarketUserDlg::InitDlg()
{
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	SetControlMap(	BUTTON_SELL	,TEXT("button_sell"));
	SetControlMap(	BUTTON_CANCELORDER	,TEXT("button_cancelorder"));

	SetControlMap(	EDITBOX_REALFEE	,TEXT("editbox_realfee"));
	SetControlMap(	EDITBOX_SELLPRICE	,TEXT("editbox_sellprice"));
	SetControlMap(	EDITBOX_SELLPRICECHAR	,TEXT("editbox_sellpricechar"));
	SetControlMap(	EDITBOX_EXPLAIN	,TEXT("editbox_explain"));

	SetControlMap(	LISTBOX_MYHOUSE	,TEXT("listbox_myhouse"));
	SetControlMap(	LISTBOX_MYORDER	,TEXT("listbox_myorder"));

	SetControlMap(  STATICTEXT_FEERATE, TEXT("statictext_feerate" ));

	// 리스트 멤버
	CList *pList_myhouse = (CList*)m_pControlMap[ LISTBOX_MYHOUSE ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];

	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];

	pRealEstateManager->siMyRealEstateSelectedIndex = -1;
	pRealEstateManager->siMyOrderSelectedIndex = -1;
	pRealEstateManager->bShowMyOrderSwitch = FALSE;

#ifdef _SAFE_MEMORY
	pRealEstateManager->clMyOrder.ZeroMem();
#else
	ZeroMemory( pRealEstateManager->clMyOrder, sizeof( cltRealEstateOrder ) );
#endif

	int i;

	int width[]		= { 96, 72 };

	pList_myhouse->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_myorder->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_myhouse->SetBorder( true );
	pList_myhouse->SetBKColorUse( true );
	pList_myorder->SetBorder( true );
	pList_myorder->SetBKColorUse( true );

	TCHAR* pText[3];
	
	pText[0] = GetTxtFromMgr(1770);
	pText[1] = GetTxtFromMgr(1771);
	pText[2] = NULL;

	for( i = 0; i < 2; ++i ) 
	{ 
        pList_myhouse->SetColumn( i, width[i], pText[i] );
        pList_myorder->SetColumn( i, width[i], pText[i] );
	}

	pList_myhouse->Refresh();
	pList_myorder->Refresh();

	pEdit_sellprice->SetNumberMode( true );

	pRealEstateManager->SetMyRealEstateList();
	ShowMyRealEstateList();
	ShowMyOrderInfo();

	SI32 id = 1;

	// 내 주문 정보 요청
	cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_MYORDER, 0 );
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NRealEstateMarketUserDlg::Action()
{

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myhouse = (CList*)m_pControlMap[ LISTBOX_MYHOUSE ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];

	// 수수료 정보를 표시한다. 
	ShowFee();

	if( pRealEstateManager->bShowMyOrderSwitch )
	{
		pRealEstateManager->bShowMyOrderSwitch = FALSE;
		ShowMyOrderList( );
	}

	TCHAR buffer[256];
	TCHAR *pchTmp = NULL;
	pchTmp = GetEditText( EDITBOX_SELLPRICE );
	if( pchTmp == NULL ) return;
	if( _tcslen(pchTmp) >= 50 ) return;
	_tcscpy( buffer, pchTmp );
	SI64 price = _tstoi64(buffer);

	TCHAR* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(price, buffer, 256, pText);
	SetEditText( EDITBOX_SELLPRICECHAR, buffer );

	ShowRealFee( price );


}

void NRealEstateMarketUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myhouse = (CList*)m_pControlMap[ LISTBOX_MYHOUSE ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];

	if( pList_myhouse == NULL || pList_myhouse == NULL || pList_myhouse == NULL ) return; 

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_MYHOUSE )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				TCHAR *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_myhouse->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_myhouse->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pRealEstateManager->siMyRealEstateSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMyRealEstateInfo( );
			}
			break;

			/*
		default:
			{
				pRealEstateManager->siMyRealEstateSelectedIndex = -1;
			}
			break;

			*/
		}
	}
	else CASE_CONTROL( LISTBOX_MYORDER )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				TCHAR *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_myorder->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_myorder->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pRealEstateManager->siMyOrderSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMyOrderInfo( );
			}
			break;
			/*
		default:
			{
				pRealEstateManager->siMyOrderSelectedIndex = -1;
			}
			break;*/
		}
	} 
	else CASE_CONTROL( BUTTON_SELL )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				
				if( pRealEstateManager->siMyRealEstateSelectedIndex < 0 ) {
					TCHAR* pText = GetTxtFromMgr(3484);
					SetMainExplain(pText);
				}

				SI64 price;

				price = _tstoi64( GetEditText( EDITBOX_SELLPRICE ) );

				if( pRealEstateManager->siMyRealEstateSelectedIndex < 0 ) {
					break;
				}


				if( price <= 0 ) {
					TCHAR* pText = GetTxtFromMgr(1758);
					SetMainExplain( pText );
 					break;
				}

				// 이미 주문이 있는지 여부를 체크한다
				for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
				{
					if( pRealEstateManager->clMyOrder[ i ].siPersonID != 0 ) {
						if( pRealEstateManager->clMyOrder[ i ].siVillageUnique ==
							pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siVillageUnique
							&&
							pRealEstateManager->clMyOrder[ i ].siHouseIndex ==
							pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siIndex
							) {
								TCHAR* pText = GetTxtFromMgr(1776);
							SetMainExplain( pText );
							break;
						}
					}
				}
				if( i < MAX_REAL_ESTATE_NUMBER_PER_PERSON ) break;

				// [수정 : 황진성 2007.8.31]
				// 부동산_부동산을 팔때 최소 가격이 3000골드에서 5000 골드로 수정.
				if( price < 5000 ) 
				{
					TCHAR* pText = GetTxtFromMgr(1759);
					SetMainExplain( pText);
					break;
				}
				
				if( price > pclClient->GetGlobalValue("GV_EstateMarketMoneyLimit") ) 
				{
					// 화폐단위 콤마 표시
					TCHAR	szPrice[64]	= { '\0', };
					GMONEY	gmMoney		= pclClient->GetGlobalValue("GV_EstateMarketMoneyLimit");
					g_JYLibFunction.SetNumUnit(gmMoney, szPrice, sizeof(szPrice));

					// 실제 화폐 입력
					NTCHARString128	strText( GetTxtFromMgr(1760) );
					strText.Replace( TEXT("#money#"), szPrice );

					SetMainExplain( strText );
					break;
				}

				// 화폐단위 콤마 표시
				TCHAR	szTax[64]	= { '\0', };
				GMONEY	gmTax		= (GMONEY)((double)price * 0.01);
				g_JYLibFunction.SetNumUnit(gmTax, szTax, sizeof(szTax));

				// 실제 화폐 입력
				NTCHARString256	strText( GetTxtFromMgr(1777) );
				strText.Replace( TEXT("#money#"), szTax );
				
				TCHAR* pText = GetTxtFromMgr(1778);

				stMsgBox MsgBox;
				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( price );
				memcpy( TempBuffer, &price, sizeof( price ) );
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pText, strText, MSGBOX_TYPE_YESNO, 0 );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

				//if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
				//{
				//	break;
				//}
				//
				//cltGameMsgRequest_RealEstateMarketSell	sendMsg( 
				//	pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siVillageUnique, 
				//	pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siIndex, 
				//	price );

				//cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
				//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );

			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_CANCELORDER )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				// 주문 취소
				if( pRealEstateManager->siMyOrderSelectedIndex < 0 ) break;

				cltGameMsgRequest_RealEstateMarketCancelOrder sendMsg(
					pRealEstateManager->clMyOrder[ pRealEstateManager->siMyOrderSelectedIndex ].siVillageUnique,
					pRealEstateManager->clMyOrder[ pRealEstateManager->siMyOrderSelectedIndex ].siHouseIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_CANCELORDER, sizeof( sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );


			}
			break;
		}
	}

	else CASE_CONTROL( EDITBOX_SELLPRICE )
	{
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			{
				SI64 siPrice = _tstoi64( GetEditText( EDITBOX_SELLPRICE ) );
				
				// 제한 금액 이상 입력시 제한금액으로 설정해줌
				if( siPrice > pclClient->GetGlobalValue("GV_EstateMarketMoneyLimit") ) 
				{
					TCHAR szMaxValue[128] = { '\0', };
					StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I64d"), pclClient->GetGlobalValue("GV_EstateMarketMoneyLimit") );
					
					SetEditText( EDITBOX_SELLPRICE, szMaxValue );
				}
			}
			break;
		}

	}

}

void NRealEstateMarketUserDlg::ShowMyRealEstateInfo( )
{


}

void NRealEstateMarketUserDlg::ShowMyOrderInfo( )
{

}

// 수수료 정보를 보여준다.
BOOL NRealEstateMarketUserDlg::ShowFee()
{
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return false;

	CStatic *pStatic_feerate = (CStatic*)m_pControlMap[ STATICTEXT_FEERATE ];
	TCHAR buffer[256];
	float feerate = (float)(pRealEstateManager->clClientStrInfo.clRealEstateMarketStrInfo.clFee.GetFeeRate() / 10);

	StringCchPrintf(buffer, 256, TEXT("%3.1f%%"), feerate);
    pStatic_feerate->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL NRealEstateMarketUserDlg::ShowRealFee(GMONEY price)
{
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return false;

	// 화폐단위 콤마 표시
	TCHAR	szPrice[64]	= { '\0', };
	GMONEY	gmPrice		= pRealEstateManager->clClientStrInfo.clRealEstateMarketStrInfo.clFee.CalcFee( price );
	g_JYLibFunction.SetNumUnit(gmPrice, szPrice, sizeof(szPrice));

	SetEditText( EDITBOX_REALFEE, szPrice );

	return TRUE;
}

void NRealEstateMarketUserDlg::ShowMyRealEstateList()
{
	TCHAR buffer[ 256 ];

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myhouse = (CList*)m_pControlMap[ LISTBOX_MYHOUSE ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];

	pList_myhouse->Clear();

	int index = 0;

	for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
	{

		if( pRealEstateManager->clMyRealEstate[ i ].siType == REALESTATE_TYPE_HOUSE ) {


			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.siParam[0] = i;
			tmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclVillageManager->GetName( pRealEstateManager->clMyRealEstate[ i ].siVillageUnique ));

			_itot( pRealEstateManager->clMyRealEstate[ i ].siIndex + 1, buffer, 10 );
			tmpListBoxItemData.Set( 1, buffer );

			pList_myhouse->AddItem( &tmpListBoxItemData );

			index++;
		}
	}	

	pList_myhouse->Refresh();

	pRealEstateManager->siMyRealEstateSelectedIndex = -1;
}

void NRealEstateMarketUserDlg::ShowMyOrderList()
{
	TCHAR buffer[ 1024 ];

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myhouse = (CList*)m_pControlMap[ LISTBOX_MYHOUSE ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];

	ShowMyRealEstateList( );

	pList_myorder->Clear();

	int index = 0;

	for( int i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; i++ )
	{
		if( pRealEstateManager->clMyOrder[ i ].siPersonID != 0 ) {

			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.siParam[0] = i;
			tmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclVillageManager->GetName( pRealEstateManager->clMyOrder[ i ].siVillageUnique ));
			_itot( pRealEstateManager->clMyOrder[ i ].siHouseIndex + 1, buffer, 10 );
			tmpListBoxItemData.Set( 1, buffer );

			pList_myorder->AddItem( &tmpListBoxItemData );

			index++;
		}	
	}

	pList_myorder->Refresh();

	pRealEstateManager->siMyOrderSelectedIndex = -1;
}

void NRealEstateMarketUserDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	switch( MsgIndex )
	{
	case 0:	// 부동산 팔기
		{
			if( pData == NULL || DataLen < 1 )
				return;

			if( RetValue )
			{
				SI64 price = *((SI64*)pData);

				cltGameMsgRequest_RealEstateMarketSell	sendMsg( 
					pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siVillageUnique, 
					pRealEstateManager->clMyRealEstate[ pRealEstateManager->siMyRealEstateSelectedIndex ].siIndex, 
					price );

				cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
