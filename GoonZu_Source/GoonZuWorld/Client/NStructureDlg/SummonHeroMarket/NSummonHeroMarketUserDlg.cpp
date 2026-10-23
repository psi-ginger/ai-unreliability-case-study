#include "NSummonHeroMarketDlg.h"

#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-SummonHeroMarket.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"


#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;

NSummonHeroMarketUserDlg::NSummonHeroMarketUserDlg()
{
}

NSummonHeroMarketUserDlg::~NSummonHeroMarketUserDlg()
{
}

void NSummonHeroMarketUserDlg::InitDlg()
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	SetControlMap(	BUTTON_SELL	,TEXT("button_sell"));
	SetControlMap(	BUTTON_CANCELORDER	,TEXT("button_cancelorder"));
	//SetControlMap(	BUTTON_SELECTSELL	,"button_selectsell");
	SetControlMap(	EDITBOX_REALFEE	,TEXT("editbox_realfee"));
	SetControlMap(	EDITBOX_SELLPRICE	,TEXT("editbox_sellprice"));
	SetControlMap(	EDITBOX_SELLPRICECHAR	,TEXT("editbox_sellpricechar"));
	SetControlMap(	EDITBOX_PLAYERNAME	,TEXT("editbox_playername"));
	SetControlMap(	LISTBOX_MYSUMMON	,TEXT("listbox_mysummon"));
	SetControlMap(	LISTBOX_MYORDER	,TEXT("listbox_myorder"));
	SetControlMap(	LISTBOX_DETAILINFO	,TEXT("listbox_detailinfo"));
	SetControlMap(  STATICTEXT_FEERATE, TEXT("statictext_feerate" ));
	SetControlMap(  CHECKBOX_SELECTSELL, TEXT("checkbox_selectsell" ));

	//CButton *pButton = (CButton *)m_pControlMap[ BUTTON_SELECTSELL ];
	//pButton->Enable( true );

	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];
	CEdit *pEdit_selectname = (CEdit*)m_pControlMap[ EDITBOX_PLAYERNAME ];
	CCheckBox *pCheck_selectsell = (CCheckBox*)m_pControlMap[ CHECKBOX_SELECTSELL ];

	// default는 일반거래로 시작
	pCheck_selectsell->SetCheck( false );
	pEdit_selectname->Enable( false );

	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	pSummonHeroManager->siMySummonSelectedIndex = -1;
	pSummonHeroManager->siMyOrderSelectedIndex = -1;

#ifdef _SAFE_MEMORY
	pSummonHeroManager->clMyOrder.ZeroMem();
#else
	ZeroMemory( pSummonHeroManager->clMyOrder, sizeof( cltSummonOrder ) );
#endif

	int i;

	int width[]		= { 70, 100 };

	pList_mysummon->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_myorder->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList_mysummon->SetBorder( true );
	pList_mysummon->SetBKColorUse( true );
	pList_myorder->SetBorder( true );
	pList_myorder->SetBKColorUse( true );
	pList_detailinfo->SetBorder( true );
	pList_detailinfo->SetBKColorUse( true );

	TCHAR* pText[3];
	
	pText[0] = GetTxtFromMgr(848);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = NULL;

	for( i = 0; i < 2; ++i ) 
	{ 
        pList_mysummon->SetColumn( i, width[i], pText[i] );
        pList_myorder->SetColumn( i, width[i], pText[i] );
	}

	pList_mysummon->Refresh();
	pList_myorder->Refresh();

	pEdit_sellprice->SetNumberMode( true );

	ShowMySummonList();

	{
		int width[] = { 80, 262 };
		pList_detailinfo->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		TCHAR* pText[3];

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
	cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_MYORDER, 0 );
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void NSummonHeroMarketUserDlg::Action()
{

	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	// 수수료 정보를 표시한다. 
	ShowFee();

	if( pSummonHeroManager->bShowMyOrderSwitch )
	{
		pSummonHeroManager->bShowMyOrderSwitch = FALSE;
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

void NSummonHeroMarketUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];
	CEdit *pEdit_selectname = (CEdit*)m_pControlMap[ EDITBOX_PLAYERNAME ];
	CButton *pButton_sell = (CButton*)m_pControlMap[ BUTTON_SELL ];
	CCheckBox *pCheck_selectsell = (CCheckBox*)m_pControlMap[ CHECKBOX_SELECTSELL ];

	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL
		|| pCheck_selectsell == NULL ) return; 

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_MYSUMMON )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				TCHAR *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_mysummon->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_mysummon->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pSummonHeroManager->siMySummonSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMySummonInfo( );
			}
			break;

		default:
			{
				pSummonHeroManager->siMySummonSelectedIndex = -1;
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
				TCHAR *pchTmp = NULL;
				SI16 siRow = -1, siCol = -1;
				
				pList_myorder->GetSelectedIndex( &siRow, &siCol );
				if( siRow == -1 || siCol == -1 ) return;
				stListBoxItemData *pstListBoxItemData = NULL;
				pstListBoxItemData = pList_myorder->GetItemData( siRow, 0 );
				if( pstListBoxItemData == NULL ) return;
				pSummonHeroManager->siMyOrderSelectedIndex = pstListBoxItemData->siParam[0];	

				ShowMyOrderInfo( );
			}
			break;
		default:
			{
				pSummonHeroManager->siMyOrderSelectedIndex = -1;
			}
			break;
		}
	} 
	
	else CASE_CONTROL( BUTTON_SELL )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				if( pSummonHeroManager->siMySummonSelectedIndex < 0 ) {
					TCHAR* pText = GetTxtFromMgr( 3505 );
					SetMainExplain( pText );
					//SetMainExplain(  TEXT("소환영웅을 선택해 주십시오") );
				}

				TCHAR personname[ MAX_PLAYER_NAME ];

				TCHAR buffer[ 1024 ];
				SI32 price;

				price = _tstoi(GetEditText(EDITBOX_SELLPRICE));

				if( price <= 0 ) {
					TCHAR* pText = GetTxtFromMgr(1758);
					SetMainExplain(  pText );
 					break;
				}

				if( price < 3000 ) {
					TCHAR* pText = GetTxtFromMgr(1759);
					SetMainExplain(  pText );
					break;
				}
				
				if( price > 5000000000 ) {
					TCHAR* pText = GetTxtFromMgr(1794);
					SetMainExplain(  pText );
					break;
				}

				// 소환영웅가 죽었으면 주문할 수 없다
				cltSummon *pclSummon = pclClient->pclCM->CR[ 1 ]->pclCI->clSummonInfo.GetSummon( pSummonHeroManager->siMySummonSelectedIndex );
				if( pclSummon == NULL ) break;

				if(pclSummon->siKind == 0)
				{
					TCHAR* pText = GetTxtFromMgr(1795);
					SetMainExplain(  pText );
					break;
				}
				
				if( pclSummon->siStatus == SUMMON_STATUS_DEAD ||
					pclSummon->clIP.GetLife() <= 0 ) {
						TCHAR* pText = GetTxtFromMgr(1796);
					SetMainExplain(  pText );
					break;	
				}

				// 지정거래시
				if( pCheck_selectsell->IsCheck() == TRUE )
				{
					TCHAR *pchTmp = NULL;
					pchTmp = GetEditText( EDITBOX_PLAYERNAME );
					if(pchTmp == NULL ) return;
					if( _tcslen( pchTmp ) >= 32 ) return;
					_tcscpy( buffer, pchTmp );

					for( int i = 0; i < (int)(_tcslen( buffer )); ++i ) {

						if( buffer[ i ] != ' ' ) break;

					}

					StringCchCopy( personname, MAX_PLAYER_NAME, &buffer[ i ] );

					if( personname[ 0 ] == NULL ) {
						TCHAR* pText = GetTxtFromMgr( 3639 );
						SetMainExplain( pText );
						//SetMainExplain(  "지정거래 할 대상의 캐릭터명을 입력하십시오" );
						break;
					}
				}

				SI32 tax = (SI32)((double)price * 0.01);
				TCHAR* pText = GetTxtFromMgr(3197);
				StringCchPrintf(buffer, 1024, pText, tax);

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
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( id ) + sizeof( price );
					memcpy( TempBuffer, &id, sizeof( id ) );
					memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				else
				{
					pText = GetTxtFromMgr(3196); 

					stMsgBox MsgBox;
					// 현재 dialog가 탭 창 같은 Child Dialog일때
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 1 );
					BYTE TempBuffer[ 128 ];
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
					//cltGameMsgRequest_SummonHeroMarketSell sendMsg( pSummonHeroManager->siMySummonSelectedIndex, price, personname );
					//cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
					//pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
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
				// 소환영웅 회수 
				if( pSummonHeroManager->siMyOrderSelectedIndex < 0 ) break;

				cltGameMsgRequest_SummonHeroMarketCancelOrder sendMsg(
					pSummonHeroManager->clMyOrder[ pSummonHeroManager->siMyOrderSelectedIndex ].siIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER, sizeof( sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

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
				TCHAR* pText = NULL;
				if( pCheck_selectsell->IsCheck() == FALSE )
				{
					pText = GetTxtFromMgr( 4816 );
					pButton_sell->SetText( pText );
					pEdit_selectname->SetText( TEXT("") );	// 지정거래 아닐때 지정거래자명 지움
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

void NSummonHeroMarketUserDlg::ShowMySummonInfo( )
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	if( pSummonHeroManager->siMySummonSelectedIndex < 0 || 
		pSummonHeroManager->siMySummonSelectedIndex >= MAX_SUMMON_OWN ) return;

	TCHAR buffer[256];

	pList_detailinfo->Clear();

	TCHAR* pText[13];

	pText[0] = GetTxtFromMgr(1782);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1506);
	pText[3] = GetTxtFromMgr(1319);
	pText[4] = GetTxtFromMgr(1783);
	pText[5] = GetTxtFromMgr(1784);
	pText[6] = GetTxtFromMgr(1785);
	pText[7] = GetTxtFromMgr(1786);
	pText[8] = GetTxtFromMgr(1787);
	pText[9] = GetTxtFromMgr(1788);
	pText[10] = GetTxtFromMgr(1789);
	pText[11] = GetTxtFromMgr(1790);
	pText[12] = GetTxtFromMgr(1791);

	//for( int i = 0; i < 13; i++ )
	//{
	//	stListBoxItemData stTmpListBoxItemData;
	//	stTmpListBoxItemData.Init();
	//	stTmpListBoxItemData.Set( 0, pText[ i ] );
	//	pList_detailinfo->AddItem( &stTmpListBoxItemData );
	//}	
	//pList_detailinfo->Refresh();

	if( pSummonHeroManager->siMySummonSelectedIndex < 0 ) return;

	cltSummon *pclSummon = pclClient->pclCM->CR[ 1 ]->pclCI->clSummonInfo.GetSummon( pSummonHeroManager->siMySummonSelectedIndex );

	stListBoxItemData stTmpListBoxItemData;
	// 종류
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 0 ] );
	stTmpListBoxItemData.Set( 1, (TCHAR*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName() );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 이름
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 1 ] );
	stTmpListBoxItemData.Set( 1, pclSummon->szName );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 레벨
	_itot( pclSummon->clIP.GetLevel(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 2 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 경험치
	
	_i64tot( pclSummon->clIP.GetCurExp(), buffer, 10 );// SI32->SI64 경험치 오버플로우때문에 수정
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 3 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 체력
	
	_itot( pclSummon->clIP.GetLife(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 4 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 마법력

	_itot( pclSummon->clIP.GetMana(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 5 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 근력
	_itot( pclSummon->clIP.GetStr(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 6 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 순발력

	_itot( pclSummon->clIP.GetDex(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 7 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 도력

	_itot( pclSummon->clIP.GetMag(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 8 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 생명력

	_itot( pclSummon->clIP.GetVit(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 9 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 보너스

	_itot( pclSummon->clIP.GetBonus(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 10 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 부적
	for( SI32 i = 0; i < 2; ++i )
	{
		stTmpListBoxItemData.Init();
		stTmpListBoxItemData.Set( 0, pText[ 11 + i ] );
		if( pclSummon->clScroll.siUnique[i] )
		{
			stTmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] ) );

			StringCchPrintf( buffer, 256, TEXT("%0.1f%%"), (float)(pclSummon->clScroll.siPercent[i] / 10.0));
			stTmpListBoxItemData.Set( 1, buffer );
		}
		else
		{
			stTmpListBoxItemData.Set( 0, TEXT("") );
			stTmpListBoxItemData.Set( 1, TEXT("") );
		}

		pList_detailinfo->AddItem( &stTmpListBoxItemData );
	}

	pList_detailinfo->Refresh();

}

void NSummonHeroMarketUserDlg::ShowMyOrderInfo( )
{

	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	if( pSummonHeroManager->siMyOrderSelectedIndex < 0 ||
		pSummonHeroManager->siMyOrderSelectedIndex >= MAX_SUMMONMARKET_ORDER_PER_PERSON ) return;

	TCHAR buffer[256];

	pList_detailinfo->Clear();

	TCHAR* pText[13];

	pText[0] = GetTxtFromMgr(1782);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1506);
	pText[3] = GetTxtFromMgr(1319);
	pText[4] = GetTxtFromMgr(1783);
	pText[5] = GetTxtFromMgr(1784);
	pText[6] = GetTxtFromMgr(1785);
	pText[7] = GetTxtFromMgr(1786);
	pText[8] = GetTxtFromMgr(1787);
	pText[9] = GetTxtFromMgr(1788);
	pText[10] = GetTxtFromMgr(1789);
	pText[11] = GetTxtFromMgr(1790);
	pText[12] = GetTxtFromMgr(1791);

	//for( int i = 0; i < 13; i++ )
	//{
	//	stListBoxItemData stTmpListBoxItemData;
	//	stTmpListBoxItemData.Init();
	//	stTmpListBoxItemData.Set( 0, pText[ i ] );
	//	pList_detailinfo->AddItem( &stTmpListBoxItemData );
	//}	
	//pList_detailinfo->Refresh();

	if( pSummonHeroManager->siMyOrderSelectedIndex < 0 ) return;

	cltSummon *pclSummon = &(pSummonHeroManager->clMyOrder[ pSummonHeroManager->siMyOrderSelectedIndex ].clSummon);

	stListBoxItemData stTmpListBoxItemData;
	// 종류
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[0 ] );
	stTmpListBoxItemData.Set( 1, (TCHAR*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName() );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 이름
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 1] );
	stTmpListBoxItemData.Set( 1, pclSummon->szName );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 레벨
	_itot( pclSummon->clIP.GetLevel(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[2 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 경험치
	
	_i64tot( pclSummon->clIP.GetCurExp(), buffer, 10 );// SI32->SI64 경험치 오버플로우때문에 수정
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[3 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 체력
	
	_itot( pclSummon->clIP.GetLife(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 4] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 마법력

	_itot( pclSummon->clIP.GetMana(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[5 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 근력
	_itot( pclSummon->clIP.GetStr(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[6 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 순발력

	_itot( pclSummon->clIP.GetDex(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 7] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 도력

	_itot( pclSummon->clIP.GetMag(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[8 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 생명력

	_itot( pclSummon->clIP.GetVit(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 9 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 보너스

	_itot( pclSummon->clIP.GetBonus(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 10 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_detailinfo->AddItem( &stTmpListBoxItemData );

	// 부적
	for( SI32 i = 0; i < 2; ++i )
	{
		stTmpListBoxItemData.Init();
		stTmpListBoxItemData.Set( 0, pText[ 11 + i ] );
		if( pclSummon->clScroll.siUnique[i] )
		{
			stTmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] ) );

			StringCchPrintf( buffer, 256, TEXT("%0.1f%%"), (float)(pclSummon->clScroll.siPercent[i] / 10.0));
			stTmpListBoxItemData.Set( 1, buffer );
		}
		else
		{
			stTmpListBoxItemData.Set( 0, TEXT("") );
			stTmpListBoxItemData.Set( 1, TEXT("") );
		}

		pList_detailinfo->AddItem( &stTmpListBoxItemData );
	}

	pList_detailinfo->Refresh();
}

// 수수료 정보를 보여준다.
BOOL NSummonHeroMarketUserDlg::ShowFee()
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return false;

	CStatic *pStatic_feerate = (CStatic*)m_pControlMap[ STATICTEXT_FEERATE ];
	TCHAR buffer[256];
	float feerate = (float)(pSummonHeroManager->clClientStrInfo.clSummonHeroMarketStrInfo.clFee.GetFeeRate() / 10);

	StringCchPrintf(buffer, 256, TEXT("%3.1f%%"), feerate);
    pStatic_feerate->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL NSummonHeroMarketUserDlg::ShowRealFee(GMONEY price)
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return false;

	TCHAR buffer[256];

	StringCchPrintf(buffer, 256, TEXT("%I64d"), pSummonHeroManager->clClientStrInfo.clSummonHeroMarketStrInfo.clFee.CalcFee( price ));
	SetEditText( EDITBOX_REALFEE, buffer );

	return TRUE;
}

void NSummonHeroMarketUserDlg::ShowMySummonList()
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	pList_mysummon->Clear();

	int index = 0;

	cltSummon *pclSummon = NULL;

	for( int i = 0; i < MAX_SUMMON_OWN; i++ )
	{

		pclSummon = &pclCM->CR[ 1 ]->pclCI->clSummonInfo.clSummon[ i ];

		if( pclSummon->siKind != 0 ) {

			if( pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->bSummonMall == false ) continue;

			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName());
			tmpListBoxItemData.siParam[0] = i;
			tmpListBoxItemData.Set( 1, pclSummon->szName );

			pList_mysummon->AddItem( &tmpListBoxItemData );

			index++;
		}
	}	

	pList_mysummon->Refresh();

	pSummonHeroManager->siMySummonSelectedIndex = -1;
}

void NSummonHeroMarketUserDlg::ShowMyOrderList()
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	// 리스트 멤버
	CList *pList_mysummon = (CList*)m_pControlMap[ LISTBOX_MYSUMMON ];
	CList *pList_myorder = (CList*)m_pControlMap[ LISTBOX_MYORDER ];
	CList *pList_detailinfo = (CList*)m_pControlMap[ LISTBOX_DETAILINFO ];
	if( pList_mysummon == NULL || pList_mysummon == NULL || pList_mysummon == NULL ) return; 

	ShowMySummonList( );

	pList_myorder->Clear();

	int index = 0;

	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_PER_PERSON; i++ )
	{
		if( pSummonHeroManager->clMyOrder[ i ].siPersonID != 0 ) {

			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclKindInfoSet->pclKI[ pSummonHeroManager->clMyOrder[ i ].clSummon.siKind ]->GetName());
			tmpListBoxItemData.siParam[0] = i;
			tmpListBoxItemData.Set( 1, pSummonHeroManager->clMyOrder[ i ].clSummon.szName );

			pList_myorder->AddItem( &tmpListBoxItemData );

			index++;
		}	
	}

	pList_myorder->Refresh();

	pSummonHeroManager->siMyOrderSelectedIndex = -1;
}

void NSummonHeroMarketUserDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	switch( MsgIndex )
	{
	case 0:	// 소환영웅 팔기
		{
			if( pData == NULL || DataLen < 1 )
				return;

			if( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 price = *((SI32*)(pData + 4));

//#if defined(_LEON_ADD_SELECTSELL_CODE)
				cltGameMsgRequest_SummonHeroMarketSell sendMsg( pSummonHeroManager->siMySummonSelectedIndex, price, TEXT(""), cltGameMsgRequest_SummonHeroMarketSell::TRADE_TYPE_NORMAL );
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
//#endif
			}
		}
		break;
	case 1:	// 소환영웅 지정 팔기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 id = *((SI32*)pData);
				SI32 price = *((SI32*)(pData + 4));
				TCHAR personname[128];
				memcpy( personname, ((TCHAR*)(pData + 8)), sizeof( personname ));

//#if defined(_LEON_ADD_SELECTSELL_CODE)
				cltGameMsgRequest_SummonHeroMarketSell sendMsg( pSummonHeroManager->siMySummonSelectedIndex, price, personname, cltGameMsgRequest_SummonHeroMarketSell::TRADE_TYPE_SELECTIVE );
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
//#endif
			}
		}
		break;
	}
}
