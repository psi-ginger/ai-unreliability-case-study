//#include <CommonLogic.h>
#include "..\..\Server\Server.h"
//
//#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
//#include "..\..\Common\Util\Control\Control.h"
//
//#include "..\..\Resource.h"
//
//#include "ShipMarket.h"
#include "..\..\DBManager\GameDBManager\DBMsg-Market.h"
//
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-ShipMarket.h"
//
//#include "../CityHall/TaxpaymentListDlg.h"
#include "..\..\Common\Ship\Ship.h"
#include "NShipMarketDlg.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/BUTTON.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

//#include "../../../CommonLogic/MsgType-Person.h"
#include "../../../CommonLogic/MsgType-Structure.h"

#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

NShipMarketBuyDlg::NShipMarketBuyDlg()
{
}

NShipMarketBuyDlg::~NShipMarketBuyDlg()
{
}

void NShipMarketBuyDlg::InitDlg()
{
	SetControlMap(	BUTTON_BUY	,"button_buy");
	SetControlMap(	BUTTON_REFRESH	,"button_refresh");
	SetControlMap(	BUTTON_SELECTORDER	,"button_selectorder");
	SetControlMap(	LISTBOX_SHIP	,"listbox_ship");
	SetControlMap(	LISTBOX_EXPLAIN	,"listbox_explain");

	//CButton *pButton = (CButton *)m_pControlMap[ BUTTON_SELECTORDER ];
	//pButton->Enable( false );

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

//	pShipManager->m_siStartPos = -1;
	pShipManager->siSelectedIndex = -1;
	
	pShipManager->bShowResponseOrderSwitch = FALSE;

	int i;

	// 리스트 멤버
	CList *pList_ship = (CList*)m_pControlMap[ LISTBOX_SHIP ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];

	if( pList_ship == NULL || pList_explain == NULL ) return; 
	{
		int width[]		= { 120, 102, 18 };

		char* pText[3];

		pText[0] = GetTxtFromMgr(1802);
		pText[1] = GetTxtFromMgr(1678);
		pText[2] = NULL;

		for( i = 0; i < 3; ++i ) 
		{ 
			pList_ship->SetColumn( i, width[i], pText[i] );
		}

        pList_ship->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		pList_ship->SetBorder( true );
		pList_ship->SetBKColorUse( true );
		pList_ship->Refresh();
	}

	{
		int width[] = { 102, 120, 18 };

		char* pText[3];

		pText[0] = GetTxtFromMgr(1780);
		pText[1] = GetTxtFromMgr(1565);
		pText[2] = NULL;

		for( i = 0; i < 3; ++i ) 
		{ 
			pList_explain->SetColumn( i, width[i], pText[i] );
		}

		pList_explain->SetBorder( true );
		pList_explain->SetBKColorUse( true );
		pList_explain->Refresh();

		SI32 id = 1;
		cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_ORDERLIST, 0 );
		pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
	}		
}

void NShipMarketBuyDlg::Action()
{
	SI32 id = 1;
	
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_ship = (CList*)m_pControlMap[ LISTBOX_SHIP ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	if( pList_ship == NULL || pList_explain == NULL ) return; 

	if(pShipManager->bShowResponseOrderSwitch == TRUE )
	{
		pShipManager->bShowResponseOrderSwitch = FALSE;

		ShowResponseOrder();
	}

	if(pShipManager->bSelectedSwitch == TRUE)
	{
		pShipManager->bSelectedSwitch = FALSE;

		pList_ship->Clear();
		pList_ship->Refresh();
		
		cltGameMsgRequest_ShipMarketOrderList sendMsg( pShipManager->m_siSelectList );

		cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_ORDERLIST, sizeof( sendMsg ), (char*)&sendMsg );
		pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );

		pShipManager->m_siSelectList = 0;
	}	
}

void NShipMarketBuyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_ship = (CList*)m_pControlMap[ LISTBOX_SHIP ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	
	if( pList_ship == NULL || pList_explain == NULL ) return; 

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_SHIP )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
		{
			char *pchTmp = NULL;
			SI16 siRow = -1, siCol = -1;
			
			pList_ship->GetSelectedIndex( &siRow, &siCol );
			if( siRow == -1 || siCol == -1 ) return;
			stListBoxItemData *pstListBoxItemData = NULL;
			pstListBoxItemData = pList_ship->GetItemData( siRow, 0 );
			if( pstListBoxItemData == NULL ) return;
			pShipManager->siSelectedIndex = pstListBoxItemData->siParam[0];	

			if( pShipManager->siSelectedIndex >= 0 )
				ShowResponseExplain( );
		}
		break;
		default :
			{
				pShipManager->siSelectedIndex = -1;
				pList_explain->Clear();
				pList_explain->Refresh();
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_SELECTORDER )
	{
		
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pShipManager->m_siSelectList = 1;
				pShipManager->bSelectedSwitch = true;
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
				pShipManager->m_siSelectList = 0;
				pShipManager->bSelectedSwitch = true;
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
				char buf[ 256 ];

				if( pclClient->pclCM->CR[ id ]->pclCI->clIP.GetLevel() < MIN_USING_SHIP_LEVEL ) {
					char* pText = GetTxtFromMgr(1753);
					sprintf( buf, pText, MIN_USING_SHIP_LEVEL );
					SetMainExplain( buf );
					break;
				}

				if( pShipManager->siSelectedIndex < 0 ) break;

				if( pShipManager->clResponseOrder[ pShipManager->siSelectedIndex ].siPersonID == 0 ) break;

				cltGameMsgRequest_ShipMarketBuy sendMsg( pShipManager->clResponseOrder[ pShipManager->siSelectedIndex ].siIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_BUY, sizeof( sendMsg ), (char *)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );

			}
			break;
		}
	}
}

void NShipMarketBuyDlg::ShowResponseOrder( )
{

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;
	// 리스트 멤버
	CList *pList_ship = (CList*)m_pControlMap[ LISTBOX_SHIP ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];

	if( pList_ship == NULL || pList_explain == NULL ) return; 

	int i;
	char buffer[256 ];
	char pricestring[64];

	// 기존의 리스트를 삭제한다. 
	pList_ship->Clear();
	pList_ship->Refresh();


	bool bcheck[MAX_SHIPMARKET_RESPONSE_ORDER_NUMBER];
	for( i = 0; i < MAX_SHIPMARKET_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}

	SI32 addnum = 0 ;
	while(addnum < pShipManager->siTotalResponseOrderNumber)
	{
		SI32 minprice	= 2100000000;
		SI32 minindex	= -1;	

		for( i = 0; i < pShipManager->siTotalResponseOrderNumber; i++ )
		{
			if( bcheck[i] == false && pShipManager->clResponseOrder[i].siPrice < minprice)
			{
				minprice	= pShipManager->clResponseOrder[i].siPrice;
				minindex	= i;
			}
		}
		
		if(minindex >= 0)
		{
			bcheck[minindex] = true;

			int len = strlen( pShipManager->clResponseOrder[minindex].clShip.szName );
			
			if( len > 0 ) {
				sprintf( buffer, "[%s]%s", 
					(char*)pclClient->pclShipManager->GetShipKindName( pShipManager->clResponseOrder[minindex].clShip.siKind ),
					pShipManager->clResponseOrder[minindex].clShip.GetName() );
			} else {
				char* pText = GetTxtFromMgr(1781);
				sprintf( buffer, pText,
					(char*)pclClient->pclShipManager->GetShipKindName( pShipManager->clResponseOrder[minindex].clShip.siKind ) );
			}


			// 선박
			stListBoxItemData stTmpListBoxItemData;
			stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 0, buffer );
			stTmpListBoxItemData.siParam[0] = minindex;
			// 가격 
			char* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(pShipManager->clResponseOrder[minindex].siPrice, pricestring, pText);

			//stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 1, pricestring );
			pList_ship->AddItem( &stTmpListBoxItemData );

			addnum++;
		}
	}
	pList_ship->Refresh();
	pShipManager->siSelectedIndex = -1;

}

void NShipMarketBuyDlg::ShowResponseExplain()
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_ship = (CList*)m_pControlMap[ LISTBOX_SHIP ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];

	if( pShipManager->siSelectedIndex < 0 ) return;

	char buffer[256];

	pList_detailinfo->Clear();

	char* pText[8];

	pText[0] = GetTxtFromMgr(1782);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1803);
	pText[3] = GetTxtFromMgr(1804);
	pText[4] = GetTxtFromMgr(1805);
	pText[5] = GetTxtFromMgr(1806);
	pText[6] = GetTxtFromMgr(1807);
	pText[7] = GetTxtFromMgr(1808);


	cltShip *pclShip = &pShipManager->clResponseOrder[ pShipManager->siSelectedIndex ].clShip;
	// 선박 종류
	char *ShipKindName = pclClient->pclShipManager->GetShipKindName( pclShip->siKind );
	if ( ShipKindName == NULL ) return;
		
	SI32 Ref = pclClient->pclShipManager->GetRefFromShipKind( pclShip->siKind );
	if ( Ref < 0 ) return;


	cltShipInfo *pclBaseShipInfo = pclClient->pclShipManager->pclShipBaseInfoList[ Ref ];

	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ pclBaseShipInfo->m_clShipBaseInfo.siKind ];
	if ( pclKindInfo == NULL ) return;

	char szKind[ 64 ] = "";
	if( pclBaseShipInfo->m_bDeepSeaShip )
	{
		char* pText = GetTxtFromMgr(1809);
		sprintf( szKind, pText, ShipKindName );
	}
	else
	{
		char* pText = GetTxtFromMgr(1810);
		sprintf( szKind, pText, ShipKindName );
	}	

	char szShipName[ 32 ];

	if( pclShip->szName[ 0 ] == NULL ) {

		char* pText = GetTxtFromMgr(1561);
		strcpy( szShipName, pText );
	} else {
		strcpy( szShipName, pclShip->szName );
	}

	stListBoxItemData stTmpListBoxItemData;
	// 종류
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 0 ] );
	stTmpListBoxItemData.Set( 1, szKind );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 이름
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 1 ] );
	stTmpListBoxItemData.Set( 1, szShipName );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 무게
	itoa( pclBaseShipInfo->m_siTotalWeight, buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 2 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 방어력
	itoa( pclKindInfo->clDefaultWeaponInfo.GetDefaultAC(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 3 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 내구도
	sprintf( buffer, "%d/%d", 
		pclShip->siDurability, ( pclKindInfo->clBA.GetVit() - pclShip->siDecreaseDurability ) * 4 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 4 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 최대승객수
	SI16 ShipManagerRef = pclClient->pclShipManager->GetRefFromShipKind( pclShip->siKind );	
	if ( ShipManagerRef >= 0 )
	{
		itoa( pclClient->pclShipManager->pclShipBaseInfoList[ ShipManagerRef ]->m_siMaxPassengerNumber, buffer, 10 );

		stTmpListBoxItemData.Init();
		stTmpListBoxItemData.Set( 0, pText[ 5 ] );
		stTmpListBoxItemData.Set( 1, buffer );
		pList_detailinfo->AddItem( &stTmpListBoxItemData );
	}

	// 포좌수
	itoa( pclBaseShipInfo->m_siCannonNum, buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 6 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 총좌수
	itoa( pclBaseShipInfo->m_siGunNum, buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 7 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );
	pList_detailinfo->Refresh();
}
