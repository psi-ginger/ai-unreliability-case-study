
#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
//
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-RealEstateMarket.h"

#include "NRealEstateMarketDlg.h"
#include "../../CommonLogic/House/House.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/BUTTON.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../../CommonLogic/MsgType-Structure.h"

#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

NRealEstateMarketBuyDlg::NRealEstateMarketBuyDlg()
{
}

NRealEstateMarketBuyDlg::~NRealEstateMarketBuyDlg()
{
}

void NRealEstateMarketBuyDlg::InitDlg()
{
	SetControlMap(	BUTTON_BUY,TEXT("button_buy"));
	SetControlMap(	BUTTON_REFRESH,TEXT("button_refresh"));

	SetControlMap(	LISTBOX_HOUSE,TEXT("listbox_house"));
	SetControlMap(	EDITBOX_EXPLAIN	,TEXT("editbox_explain"));

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	pRealEstateManager->siSelectedIndex = -1;
	pRealEstateManager->bShowResponseOrderSwitch = FALSE;

	pRealEstateManager->bRequestDetailInfo = FALSE;
	pRealEstateManager->bUpdateDetailInfo = FALSE;

	int i;	
	
	// 리스트 멤버
	CList *pList_house = (CList*)m_pControlMap[ LISTBOX_HOUSE ];
	if( pList_house == NULL ) return; 

	{
		int width[]		= { 88, 56, 100 };

		TCHAR* pText[3];

		pText[0] = GetTxtFromMgr(1770);
		pText[1] = GetTxtFromMgr(1771);
		pText[2] = GetTxtFromMgr(1678);

		for( i = 0; i < 3; ++i ) 
		{ 
			pList_house->SetColumn( i, width[i], pText[i] );
		}

        pList_house->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		pList_house->SetBorder( true );
		pList_house->SetBKColorUse( true );
		pList_house->Refresh();
	}

	cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_ORDERLIST, 0 );
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
}

void NRealEstateMarketBuyDlg::Action()
{
	SI32 id = 1;
	
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_house = (CList*)m_pControlMap[ LISTBOX_HOUSE ];


	if(pRealEstateManager->bUpdateDetailInfo == TRUE ) {
		pRealEstateManager->bUpdateDetailInfo = FALSE;

		ShowResponseExplain();
	}


	if(pRealEstateManager->bRequestDetailInfo == TRUE ) {
		pRealEstateManager->bRequestDetailInfo = FALSE;

		if( pRealEstateManager->siSelectedIndex >= 0 ) {

			cltGameMsgRequest_RealEstateMarketDetailInfo sendMsg( 
				pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siVillageUnique,
				pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siHouseIndex );

			cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_DETAILINFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
		}
	}

	if(pRealEstateManager->bShowResponseOrderSwitch == TRUE )
	{
		pRealEstateManager->bShowResponseOrderSwitch = FALSE;

		ShowResponseOrder();
	}

	if(pRealEstateManager->bSelectedSwitch == TRUE)
	{
		ShowResponseExplain();

		pRealEstateManager->bSelectedSwitch = FALSE;

		pList_house->Clear();
		pList_house->Refresh();

		cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_ORDERLIST, 0 );
		pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
	}	
}

void NRealEstateMarketBuyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_house = (CList*)m_pControlMap[ LISTBOX_HOUSE ];
	SI32 id = 1;

	CASE_CONTROL( LISTBOX_HOUSE )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				TCHAR *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_house->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_house->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pRealEstateManager->siSelectedIndex = pstListBoxItemData->siParam[0];	
				
				pRealEstateManager->bRequestDetailInfo = TRUE;
			}
			break;
			default :
			{
				//pRealEstateManager->siSelectedIndex = -1;
				//SetEditText( EDITBOX_EXPLAIN, TEXT("") ); 
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_REFRESH )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pRealEstateManager->bSelectedSwitch = true;
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_BUY )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				TCHAR buf[ 256 ];

				if( pclClient->pclCM->CR[ id ]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE ) {
					TCHAR* pText = GetTxtFromMgr(1753);
					StringCchPrintf( buf, 256, pText, LEVELLIMIT_USE_HOUSE );
					SetMainExplain( buf );
					break;
				}

				if( pRealEstateManager->siSelectedIndex < 0 )break;

				if( pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siPersonID == 0 )break;

					cltGameMsgRequest_RealEstateMarketBuy sendMsg( 
					pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siIndex,
					pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siVillageUnique,
					pRealEstateManager->clResponseOrder[ pRealEstateManager->siSelectedIndex ].siHouseIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_REALESTATEMARKET_BUY, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			break;
		}
	}
}

void NRealEstateMarketBuyDlg::ShowResponseOrder( )
{

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	// 리스트 멤버
	CList *pList_house = (CList*)m_pControlMap[ LISTBOX_HOUSE ];

	int i;
	TCHAR buffer[256 ];
	TCHAR pricestring[64];

	// 기존의 리스트를 삭제한다. 
	pList_house->Clear();
	pList_house->Refresh();


	bool bcheck[MAX_REALESTATEMARKET_RESPONSE_ORDER_NUMBER];
	for( i = 0; i < MAX_REALESTATEMARKET_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}

	SI32 addnum = 0 ;
	while(addnum < pRealEstateManager->siTotalResponseOrderNumber)
	{
		GMONEY minprice	= 0x7fffffffffffffffL;//SI64의 MAX로 설정 
		SI32 minindex	= -1;	

		for( i = 0; i < pRealEstateManager->siTotalResponseOrderNumber; i++ )
		{
			if( bcheck[i] == false && pRealEstateManager->clResponseOrder[i].siPrice <= minprice)
			{
				minprice	= pRealEstateManager->clResponseOrder[i].siPrice;
				minindex	= i;
			}
		}
		
		if(minindex >= 0)
		{
			bcheck[minindex] = true;

			// 마을
			stListBoxItemData stTmpListBoxItemData;
			stTmpListBoxItemData.Init();
			stTmpListBoxItemData.siParam[0] = minindex;
			stTmpListBoxItemData.Set( 0, (TCHAR*)pclClient->pclVillageManager->GetName( pRealEstateManager->clResponseOrder[minindex].siVillageUnique ) );
			
			// 시전주소
			_itot( pRealEstateManager->clResponseOrder[minindex].siHouseIndex + 1, buffer, 10 );
			stTmpListBoxItemData.Set( 1, buffer );

			// 가격 
			TCHAR* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(pRealEstateManager->clResponseOrder[minindex].siPrice, pricestring, 64, pText);
			stTmpListBoxItemData.Set( 2, pricestring );
			pList_house->AddItem( &stTmpListBoxItemData );

			addnum++;
		}
	}
	pList_house->Refresh();
	pRealEstateManager->siSelectedIndex = -1;
}

void NRealEstateMarketBuyDlg::ShowResponseExplain()
{

	// 거간 매니져
	cltRealEstateMarketManager *pRealEstateManager = ((cltRealEstateMarketManager *)m_pStrManager);
	if( pRealEstateManager == NULL ) return;

	//KHY - 0816 - 부동산 새로고침 버튼 버그수정.
	if(pRealEstateManager->bRequestDetailInfo ==FALSE) return;

	TCHAR *pVillageName;
	TCHAR buffer[ 1024 ];

	if( pclClient->pclVillageManager->IsValidVillage( pRealEstateManager->clDetailInfo.siVillageUnique ) == FALSE ) return;

	pVillageName = pclClient->pclVillageManager->GetName( pRealEstateManager->clDetailInfo.siVillageUnique );

	if( pVillageName == NULL ) return;

	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr(1772);
	pText[1] = GetTxtFromMgr(1773);
	pText[2] = GetTxtFromMgr(1561);

	StringCchPrintf( buffer, 1024, 
		pText[0],
		pVillageName, 
		pRealEstateManager->clDetailInfo.siHouseIndex + 1, 
		pRealEstateManager->clDetailInfo.siCurrentRentFee,
		pRealEstateManager->clDetailInfo.siRentFee,
		pRealEstateManager->clDetailInfo.clContractDate.uiYear + 
		pRealEstateManager->clDetailInfo.siRentPeriod,
		pRealEstateManager->clDetailInfo.clContractDate.uiMonth,
		pRealEstateManager->clDetailInfo.clContractDate.uiDay,
		pRealEstateManager->clDetailInfo.siItemNum,
		pRealEstateManager->clDetailInfo.siStableNum,
		pRealEstateManager->clDetailInfo.siHorseNum
		);
	

	SetEditText( EDITBOX_EXPLAIN, buffer );
}
