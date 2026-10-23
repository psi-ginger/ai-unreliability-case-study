#include "NShipMarketDlg.h"

#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-ShipMarket.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"


#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../Common/Ship/Ship.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;

NShipMarketUserDlg::NShipMarketUserDlg()
{
}

NShipMarketUserDlg::~NShipMarketUserDlg()
{
}

void NShipMarketUserDlg::InitDlg()
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	SetControlMap(	BUTTON_SELL	,"button_sell");
	SetControlMap(	BUTTON_CANCELORDER	,"button_cancelorder");
	//SetControlMap(	BUTTON_SELECTSELL	,"button_selectsell");
	SetControlMap(	EDITBOX_REALFEE	,"editbox_realfee");
	SetControlMap(	EDITBOX_SELLPRICE	,"editbox_sellprice");
	SetControlMap(	EDITBOX_SELLPRICECHAR	,"editbox_sellpricechar");
	SetControlMap(	EDITBOX_PLAYERNAME	,"editbox_playername");
	SetControlMap(	LISTBOX_MYSHIP	,"listbox_myship");
	SetControlMap(	LISTBOX_MYORDER	,"listbox_myorder");
	SetControlMap(	LISTBOX_DETAILINFO	,"listbox_detailinfo");
	SetControlMap(  STATICTEXT_FEERATE, "statictext_feerate" );
	SetControlMap(  CHECKBOX_SELECTSELL, "checkbox_selectsell" );


	//CButton *pButton = (CButton *)m_pControlMap[ BUTTON_SELECTSELL ];
	//pButton->Enable( false );

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];
	CEdit *pEdit_selectname = (CEdit*)m_pControlMap[ EDITBOX_PLAYERNAME ];

	CCheckBox *pCheck_selectsell = (CCheckBox*)m_pControlMap[ CHECKBOX_SELECTSELL ];

	// default는 일반거래로 시작
	pCheck_selectsell->SetCheck( false );
	pEdit_selectname->Enable( false );

	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	pShipManager->siMyShipSelectedIndex = -1;
	pShipManager->siMyOrderSelectedIndex = -1;

	ZeroMemory( pShipManager->clMyOrder, sizeof( cltShipOrder ) );

	int i;

	int width[]		= { 70, 100 };

	pList_myship->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_myorder->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_myship->SetBorder( true );
	pList_myship->SetBKColorUse( true );
	pList_myorder->SetBorder( true );
	pList_myorder->SetBKColorUse( true );
	pList_detailinfo->SetBorder( true );
	pList_detailinfo->SetBKColorUse( true );

	char* pText[3];
	
	pText[0] = GetTxtFromMgr(1813);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = NULL;

	for( i = 0; i < 2; ++i ) 
	{ 
        pList_myship->SetColumn( i, width[i], pText[i] );
        pList_myorder->SetColumn( i, width[i], pText[i] );
	}

	pList_myship->Refresh();
	pList_myorder->Refresh();

	pEdit_sellprice->SetNumberMode( true );

	ShowMyShipList();
	ShowMyOrderInfo();

	{
		int width[] = { 80, 262 };
		pList_detailinfo->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		char* pText[3];

		pText[0] = GetTxtFromMgr(1780);
		pText[1] = GetTxtFromMgr(1565);
		pText[2] = NULL;

		for( i = 0; i < 2; ++i ) 
		{ 
			pList_detailinfo->SetColumn( i, width[i], pText[i] );
		}
	}		

	pList_detailinfo->Refresh();

	SI32 id = 1;

	// 내 주문 정보 요청 
	cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_MYORDER, 0 );
	pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
}

void NShipMarketUserDlg::Action()
{

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	// 수수료 정보를 표시한다. 
	ShowFee();

	if( pShipManager->bShowMyOrderSwitch )
	{
		pShipManager->bShowMyOrderSwitch = FALSE;
		ShowMyOrderList( );
	}

	char buffer[256];
	char *pchTmp = NULL;
	pchTmp = GetEditText( EDITBOX_SELLPRICE );
	if( pchTmp == NULL ) return;
	if( _tcslen(pchTmp) >= 50 ) return;
	_tcscpy( buffer, pchTmp );
	SI64 price = _atoi64(buffer);

	char* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumTxt(price, buffer, pText);
	SetEditText( EDITBOX_SELLPRICECHAR, buffer );

	ShowRealFee( price );


}

void NShipMarketUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];
	CEdit *pEdit_selectname = (CEdit*)m_pControlMap[ EDITBOX_PLAYERNAME ];
	CButton *pButton_sell = (CButton*)m_pControlMap[ BUTTON_SELL ];
	CCheckBox *pCheck_selectsell = (CCheckBox*)m_pControlMap[ CHECKBOX_SELECTSELL ];

	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL 
		|| pCheck_selectsell == NULL ) return; 

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_MYSHIP )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				char *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_myship->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_myship->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pShipManager->siMyShipSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMyShipInfo( );
			}
			break;

		default:
			{
				pShipManager->siMyShipSelectedIndex = -1;
			}
			break;
		}
	}
	else CASE_CONTROL( LISTBOX_MYORDER )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				char *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_myorder->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_myorder->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pShipManager->siMyOrderSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMyOrderInfo( );
			}
			break;
		default:
			{
				pShipManager->siMyOrderSelectedIndex = -1;
			}
			break;
		}
	} 
	//else CASE_CONTROL( BUTTON_SELL )
	//{
	//	switch( nEvent )
	//	{
	//	case EVENT_BUTTON_CLICKED:
	//		{
	//			if( pShipManager->siMyShipSelectedIndex < 0 ) {
	//				char* pText = GetTxtFromMgr(3492);
	//				SetMainExplain(pText);
	//			}

	//			char buffer[ 1024 ];
	//			SI32 price;

	//			price = atoi( GetEditText( EDITBOX_SELLPRICE ) );

	//			if( price <= 0 ) {
	//				char* pText = GetTxtFromMgr(1758);
	//				SetMainExplain( pText );
 //					break;
	//			}

	//			if( price < 3000 ) {
	//				char* pText = GetTxtFromMgr(1759);
	//				SetMainExplain( pText );
	//				break;
	//			}
	//			
	//			if( price > 1000000000 ) {
	//				char* pText = GetTxtFromMgr(1814);
	//				SetMainExplain( pText );
	//				break;
	//			}

	//			cltShip *pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pShipManager->siMyShipSelectedIndex ];
	//			if( pclShip == NULL ) break;

	//			if(pclShip->siShipID < 1)
	//			{
	//				char* pText = GetTxtFromMgr(1815);
	//				SetMainExplain( pText );
	//				break;
	//			}

	//			if( pclShip->siShipPos < ShipPos_Port || 
	//				pclShip->siShipPos > CUR_VILLAGE_LIMIT ) {
	//				char* pText = GetTxtFromMgr(1816);
	//				SetMainExplain( pText );
	//				break;
	//			}

	//			SI32 tax = (SI32)((double)price * 0.01);
	//			char* pText = GetTxtFromMgr(1817);
	//			sprintf(buffer, pText, tax);

	//			pText = GetTxtFromMgr(1818);

	//			stMsgBox MsgBox;
	//			// 현재 dialog가 탭 창 같은 Child Dialog일때
	//			CControllerMgr *pParent = NULL;
	//			pParent = this->GetParentDialog();
	//			if(pParent == NULL ) return;
	//			MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
	//			char TempBuffer[ 128 ] = "";
	//			SI16 Size = sizeof( id ) + sizeof( price );
	//			memcpy( TempBuffer, &id, sizeof( id ) );
	//			memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
	//			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	//			
	//			//if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
	//			//{
	//			//	break;
	//			//}

	//			//
	//			//cltGameMsgRequest_ShipMarketSell sendMsg( pclShip->siShipID, price );
	//			//cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_SELL, sizeof( sendMsg ), (char *)&sendMsg );
	//			//pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
	//		}
	//		break;
	//	}
	//}
	else CASE_CONTROL( BUTTON_SELL )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				if( pShipManager->siMyShipSelectedIndex < 0 ) {

					char* pText = GetTxtFromMgr(5054);
					SetMainExplain( pText);
				}

				char personname[ MAX_PLAYER_NAME ];

				char buffer[ 256 ];
				SI32 price;

				price = atoi(GetEditText( EDITBOX_SELLPRICE ));

				if( price <= 0 ) {
					char* pText = GetTxtFromMgr(1758);
					SetMainExplain( pText );
 					break;
				}

				if( price < 3000 ) {
					char* pText = GetTxtFromMgr(1759);
					SetMainExplain( pText );
					break;
				}
				
				if( price > 1000000000 ) {
					char* pText = GetTxtFromMgr(1814);
					SetMainExplain( pText );
					break;
				}

				
				cltShip *pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pShipManager->siMyShipSelectedIndex ];

				if(pclShip->siShipID < 1)
				{
					char* pText = GetTxtFromMgr(1815);
					SetMainExplain( pText );
					break;
				}
				
				if( pclShip->siShipPos < ShipPos_Port || pclShip->siShipPos > CUR_VILLAGE_LIMIT ) {
					char* pText = GetTxtFromMgr(1816);
					SetMainExplain( pText );
					break;
				}

				// 지정거래시
				if( pCheck_selectsell->IsCheck() == TRUE )
				{
					char *pchTmp = NULL;
					pchTmp = GetEditText( EDITBOX_PLAYERNAME );
					if(pchTmp == NULL ) return;
					if( _tcslen( pchTmp ) >= 32 ) return;
					_tcscpy( buffer, pchTmp );

					for( int i = 0; i < strlen( buffer ); ++i ) {

						if( buffer[ i ] != ' ' ) break;

					}

					strcpy( personname, &buffer[ i ] );

					if( personname[ 0 ] == NULL ) {
						char* pText = GetTxtFromMgr(3639);
						SetMainExplain( pText );
						break;
					}
				}

				SI32 tax = (SI32)((double)price * 0.01);
				char* pText = GetTxtFromMgr(1817);
				sprintf(buffer, pText, tax);

				// 일반거래시
				if( pCheck_selectsell->IsCheck() == FALSE )
				{
					pText = GetTxtFromMgr(1798); 

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					char TempBuffer[ 128 ] = "";
					SI16 Size = sizeof( id ) + sizeof( price );
					memcpy( TempBuffer, &id, sizeof( id ) );
					memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				else
				{
					pText = GetTxtFromMgr(1818); 

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 1 );
					char TempBuffer[ 128 ] = "";
					SI16 Size = sizeof( id ) + sizeof( price ) + sizeof( personname );
					memcpy( TempBuffer, &id, sizeof( id ) );
					memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
					memcpy( &TempBuffer[ sizeof(id) + sizeof( price ) ], personname, sizeof( personname ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					//if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
					//{
					//	break;
					//}
					//
					//cltGameMsgRequest_ShipMarketSell sendMsg( pShipManager->siMyShipSelectedIndex, price, personname );
					//cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_SELL, sizeof( sendMsg ), (char *)&sendMsg );
					//pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
				}

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
				// 선박 회수 
				if( pShipManager->siMyOrderSelectedIndex < 0 ) break;

				cltGameMsgRequest_ShipMarketCancelOrder sendMsg(
					pShipManager->clMyOrder[ pShipManager->siMyOrderSelectedIndex ].siIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_CANCELORDER, sizeof( sendMsg), (char *)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
			}
			break;
		}
	}
	else CASE_CONTROL( CHECKBOX_SELECTSELL )
	{
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			{
				char* pText = NULL;
				if( pCheck_selectsell->IsCheck() == FALSE )
				{
					pText = GetTxtFromMgr( 4816 );
					pButton_sell->SetText( pText );
					pEdit_selectname->SetText( "" );	// 지정거래 아닐때 지정거래자명 지움
					pEdit_selectname->Enable( false );
				}
				else
				{
					pText = GetTxtFromMgr( 4818 );
					pButton_sell->SetText( pText );
					pEdit_selectname->Enable( true );
				}
			}

		}
	}
}

void NShipMarketUserDlg::ShowMyShipInfo( )
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	if( pShipManager->siMyShipSelectedIndex < 0 || 
		pShipManager->siMyShipSelectedIndex >= MAX_SHIP_PER_PERSON ) return;

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

	if( pShipManager->siMyShipSelectedIndex < 0 ) return;

	cltShip *pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pShipManager->siMyShipSelectedIndex ];

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

void NShipMarketUserDlg::ShowMyOrderInfo( )
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	if( pShipManager->siMyOrderSelectedIndex < 0 ||
		pShipManager->siMyOrderSelectedIndex >= MAX_SHIPMARKET_ORDER_PER_PERSON ) return;

	cltShip *pclShip = NULL;
	SI32 shipid = pShipManager->clMyOrder[ pShipManager->siMyOrderSelectedIndex ].siShipID;

	for( int i = 0; i < MAX_SHIP_PER_PERSON; ++i ) {
		
		pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ i ];
		
		if( shipid == pclShip->siShipID ) break;
	}
	if( i == MAX_SHIP_PER_PERSON ) return;

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

	if( pShipManager->siMyOrderSelectedIndex < 0 ) return;

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

// 수수료 정보를 보여준다.
BOOL NShipMarketUserDlg::ShowFee()
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return false;

	CStatic *pStatic_feerate = (CStatic*)m_pControlMap[ STATICTEXT_FEERATE ];
	char buffer[256];
	float feerate = (float)(pShipManager->clClientStrInfo.clShipMarketStrInfo.clFee.GetFeeRate() / 10);

	sprintf(buffer, "%3.1f%%", feerate);
    pStatic_feerate->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL NShipMarketUserDlg::ShowRealFee(SI32 price)
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return false;

	char buffer[256];

	sprintf(buffer, "%d", pShipManager->clClientStrInfo.clShipMarketStrInfo.clFee.CalcFee( price ));
	SetEditText( EDITBOX_REALFEE, buffer );

	return TRUE;
}

void NShipMarketUserDlg::ShowMyShipList()
{

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	char buffer[ 256 ];

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	pList_myship->Clear();

	int index = 0;

	cltShip *pclShip = NULL;

	for( int i = 0; i < MAX_SHIP_PER_PERSON; i++ )
	{
		pclShip = &pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ i ];

		if( pclShip->siKind != 0 && pclShip->siShipID != 0) {

			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.siParam[0] = i;

			tmpListBoxItemData.Set( 0, (char *)pclClient->pclShipManager->GetShipKindName( pclShip->siKind ));

			if( pclShip->GetName() ) {
				strcpy( buffer, pclShip->GetName() );
			} else {
				char* pText = GetTxtFromMgr(1561);
				strcpy( buffer, pText );
			}
			
			tmpListBoxItemData.Set( 1, buffer );
			pList_myship->AddItem( &tmpListBoxItemData );
			
			index++;
		}
	}	

	pList_myship->Refresh();

	pShipManager->siMyShipSelectedIndex = -1;

}

void NShipMarketUserDlg::ShowMyOrderList()
{

	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	// 리스트 멤버
	CList *pList_myship = (CList*)m_pControlMap[ LISTBOX_MYSHIP ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_myship == NULL || pList_myship == NULL || pList_myship == NULL ) return; 

	ShowMyShipList( );

	pList_myorder->Clear();

	int index = 0;

	char buffer[ 256 ];
	cltShip *pclShip = NULL;

	for( int i = 0; i < MAX_SHIPMARKET_ORDER_PER_PERSON; i++ )
	{

		if( pShipManager->clMyOrder[ i ].siPersonID != 0 ) {

			for( int j = 0; j < MAX_SHIP_PER_PERSON; j++ ) {

				pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ j ];

				if( pShipManager->clMyOrder[ i ].siShipID == pclShip->siShipID ) {

					stListBoxItemData tmpListBoxItemData;
					tmpListBoxItemData.Init();
					tmpListBoxItemData.siParam[0] = i;
					
					tmpListBoxItemData.Set( 0, (char *)pclClient->pclShipManager->GetShipKindName( pclShip->siKind ));

					if( pclShip->GetName() ) {
						strcpy( buffer, pclShip->GetName() );
					} else {
						char* pText = GetTxtFromMgr(1561);
						strcpy( buffer, pText );
					}

					tmpListBoxItemData.Set( 1, buffer );
					pList_myorder->AddItem( &tmpListBoxItemData );
					
					index++;

					break;
				}
			}
		}	

	}

	pList_myorder->Refresh();

	pShipManager->siMyOrderSelectedIndex = -1;
}

void NShipMarketUserDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen )
{
	// 전함사 매니져
	cltShipMarketManager *pShipManager = ((cltShipMarketManager *)m_pStrManager);
	if( pShipManager == NULL ) return;

	switch( MsgIndex )
	{
	case 0:	// 선박 판매
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				cltShip *pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pShipManager->siMyShipSelectedIndex ];

				SI32 id = *((SI32*)pData);
				SI32 price = *((SI32*)(pData + 4));

//#if defined(_LEON_ADD_SELECTSELL_CODE)
				cltGameMsgRequest_ShipMarketSell sendMsg( pclShip->siShipID, price, "", cltGameMsgRequest_ShipMarketSell::TRADE_TYPE_NORMAL );
				cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_SELL, sizeof( sendMsg ), (char *)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
//#endif
			}
		}
		break;
	case 1:	// 선박 지정 판매
		{
			if( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 price = *((SI32*)(pData + 4));
				char personname[128];
				memcpy( personname, ((char*)(pData + 8)), sizeof( personname ));

				cltShip *pclShip = &pclClient->pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pShipManager->siMyShipSelectedIndex ];

//#if defined(_LEON_ADD_SELECTSELL_CODE)
				cltGameMsgRequest_ShipMarketSell sendMsg( pclShip->siShipID, price, personname, cltGameMsgRequest_ShipMarketSell::TRADE_TYPE_SELECTIVE );
				cltMsg clMsg( GAMEMSG_REQUEST_SHIPMARKET_SELL, sizeof( sendMsg ), (char *)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (char*)&clMsg );
//#endif
			}
		}
		break;
	}
}