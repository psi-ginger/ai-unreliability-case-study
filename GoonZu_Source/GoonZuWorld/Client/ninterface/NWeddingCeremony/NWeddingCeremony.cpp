
#include "NWeddingCeremony.h"
#include "NWeddingCeremonyListDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../common/Char/CharManager/CharManager.h"
#include "../../common/Family/Marriage.h"
#include "../../CommonLogic/Msg/MsgType-Wedding.h"


extern cltCommonLogic* pclClient;

CNWeddingCeremony::CNWeddingCeremony( void )
{

}

CNWeddingCeremony::~CNWeddingCeremony( void )
{
	SAFE_DELETE(m_pWeddingListDlg);
}

void CNWeddingCeremony::Init( )
{
	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		m_clSimpleList[siIndex].Init();
	}
	
	m_siReserveYear		= 0;
	m_siReserveMonth	= 0;
	m_siReserveDay		= 0;
	m_siReserveHour		= 0;

	m_pWeddingListDlg	= NULL;

}

void CNWeddingCeremony::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NWeddingCeremony/DLG_NWeddingCeremony_Menu.ddf" ) );
	file.CreatePopupDialog( this, NWEDDINGCEREMONY_MENU_DLG, TEXT( "dialog_weddingceremony_menu" ), StaticCallBackDialogNWeddingCeremony );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_MENU_DLG_BUTTON_RESERVE,	this), WEDDINGCEREMONY_MENU_DLG_BUTTON_RESERVE,		TEXT("button_reserve") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_MENU_DLG_BUTTON_LIST,		this), WEDDINGCEREMONY_MENU_DLG_BUTTON_LIST,		TEXT("button_list") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_MENU_DLG_BUTTON_PROPOSE,	this), WEDDINGCEREMONY_MENU_DLG_BUTTON_PROPOSE,		TEXT("button_propose") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_MENU_DLG_BUTTON_APPLY,		this), WEDDINGCEREMONY_MENU_DLG_BUTTON_APPLY,		TEXT("button_apply") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_MENU_DLG_BUTTON_CLOSE,		this), WEDDINGCEREMONY_MENU_DLG_BUTTON_CLOSE,		TEXT("button_close") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	WEDDINGCEREMONY_MENU_DLG_EDITBOX_EXPLAIN,	this), WEDDINGCEREMONY_MENU_DLG_EDITBOX_EXPLAIN,	TEXT("editbox_explain") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1,	this), WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1,	TEXT("listbox_NONAME1") );

	CEdit* pclExplain = m_InterfaceFactory.GetEdit( WEDDINGCEREMONY_MENU_DLG_EDITBOX_EXPLAIN );
	if ( pclExplain )
	{
		pclExplain->SetText( GetTxtFromMgr(9973) );
		pclExplain->Enable( false );
	}

	CList* pclReserve = m_InterfaceFactory.GetList( WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1 );
	if ( pclReserve )
	{
		pclReserve->SetBorder( true );
		pclReserve->SetBKColorUse( true );

		SI16 siScroolSize	= 19;
		SI16 siStateSize	= 110;
		SI16 siDateSize		= pclReserve->GetWidth() - siStateSize - siScroolSize;

		pclReserve->SetColumn( 0, siDateSize,	TEXT("") );
		pclReserve->SetColumn( 1, siStateSize,	TEXT("") );
		pclReserve->SetColumn( 2, siScroolSize,	TEXT("") );

		pclReserve->Refresh();
	}

	// 리스트를 요청한다
	cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET, 0 );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	// 
	m_pWeddingListDlg = new CNWeddingCeremonyListDlg;
	if ( NULL == m_pWeddingListDlg )
	{
		DeleteDialog();
	}

	m_pWeddingListDlg->Create( this );

	// 기본은 예약 메뉴이다
	ChangeTabDialog( WEDDINGCEREMONY_MENU_DLG_BUTTON_RESERVE );
	
}

void CALLBACK CNWeddingCeremony::StaticCallBackDialogNWeddingCeremony( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWeddingCeremony* pThis = (CNWeddingCeremony*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNWeddingCeremony( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNWeddingCeremony::CallBackDialogNWeddingCeremony( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	
	case WEDDINGCEREMONY_MENU_DLG_BUTTON_RESERVE:
		{
			ChangeTabDialog( nControlID );
		}
		break;

	case WEDDINGCEREMONY_MENU_DLG_BUTTON_LIST:
		{
			ChangeTabDialog( nControlID );
		}
		break;

	case WEDDINGCEREMONY_MENU_DLG_BUTTON_PROPOSE:
		{
			OnButtonClickPropose( );
		}
		break;

	case WEDDINGCEREMONY_MENU_DLG_BUTTON_APPLY:
		{
			OnButtonClickReserve();
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
	case WEDDINGCEREMONY_MENU_DLG_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;

	}
}

void CNWeddingCeremony::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch( MsgIndex )
	{
	case 1:	
		{
			if ( true == RetValue )
			{
				cltGameMsgRequest_WeddingReserve clinfo( m_siReserveYear, m_siReserveMonth, m_siReserveDay, m_siReserveHour );
				cltMsg clMsg(GAMEMSG_REQUEST_WEDDING_RESERVE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	case 2:
		{
			if ( m_pWeddingListDlg )
			{
				m_pWeddingListDlg->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
			}
		}
	}
}

void CNWeddingCeremony::OnButtonClickReserve( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	// 결혼하지도 않았는데 예약 하려 할경우
	if ( FALSE == pclChar->IsMarried() )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(422);
		TCHAR* pszText	= GetTxtFromMgr(30065);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	CList* pclReserve = m_InterfaceFactory.GetList( WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1 );
	if ( NULL == pclReserve )
	{
		return;
	}

	// 예약시간(리스트)을 선택하지 않았을 경우
	SI16 siSelectedIndex = pclReserve->GetSelectedIndex();
	if ( (0 > siSelectedIndex) || (WEDDING_RESERVE_LIST_NUM <= siSelectedIndex) )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(1868);
		TCHAR* pszText	= GetTxtFromMgr(30064);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	// 이미 누군가가 예약을 했다면
	if ( WEDDING_RESERVE_STATUS_NONE != m_clSimpleList[siSelectedIndex].m_siReserveStatus )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(422);
		TCHAR* pszText	= GetTxtFromMgr(7072);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	// 선택한 인덱스에서 날짜 정보를 뽑아온다
	TCHAR* pszGetTimeText = pclReserve->GetText( siSelectedIndex, 0 );
	if ( NULL == pszGetTimeText )
	{
		return;
	}

	SI32	siYear		= 0;
	SI32	siMonth		= 0;
	SI32	siDay		= 0;
	SI32	siHour		= 0;

	TCHAR*	pszScanText	= GetTxtFromMgr(3808);

	_stscanf( pszGetTimeText, pszScanText, &siYear, &siMonth, &siDay, &siHour );

	// 우선 유효한 날짜인지 검사
	if ( 1000 > siYear )					return;
	if ( (0 > siMonth)	|| (12 < siMonth) )	return;
	if ( (0 > siDay)	|| (31 < siDay) )	return;
	if ( (0 > siHour)	|| (23 < siHour) )	return;

	// 현재 시간에 예약을 하는것은 불가능하다
	if ( (siYear == pclclient->sTime.wYear) 
		&& (siMonth == pclclient->sTime.wMonth)
		&& (siDay == pclclient->sTime.wDay)
		&& (siHour == pclclient->sTime.wHour)
	)
	{
		TCHAR* pszTitle	= GetTxtFromMgr(422);
		TCHAR* pszText	= GetTxtFromMgr(7228);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	// 기준가격에 더할 수를 뽑아온다
	SI32 siNpcRate = pclclient->pclItemManager->clItemStatistic.siNPCPriceRate;
	siNpcRate = siNpcRate / 500;

	// 가격을 계산하고
	GMONEY siReserveCost = (WEDDING_RESERVE_BASIC_COST) * ( 2.5 * siNpcRate );
	if ( 0 >= siNpcRate )
	{
		siReserveCost = (WEDDING_RESERVE_BASIC_COST);
	}

	// 은행에 돈이 있는지 검사한다
	if ( siReserveCost > pclChar->pclCI->clBank.GetMoney() )
	{
		GMONEY	gmNeedMoney			= TABS( pclChar->pclCI->clBank.GetMoney() - siReserveCost );
		TCHAR	szNeedMoney[128]	= TEXT("");
		TCHAR*	pText				= GetTxtFromMgr(4837);

		g_JYLibFunction.SetNumUnit( gmNeedMoney, szNeedMoney, 128, pText );

		NTCHARString32	strTitle	= GetTxtFromMgr(422);
		NTCHARString256	strText		= TEXT("#message# (#need#)");

		NTCHARString128	strMsg		= GetTxtFromMgr(5950);

		strText.Replace( TEXT("#message#"),	strMsg );
		strText.Replace( TEXT("#need#"),	szNeedMoney );

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
		return;
	}

	// 계산된 가격에 ,를 붙인다
	TCHAR szReserveCost[64] = TEXT("");;
	g_JYLibFunction.SetNumUnit( siReserveCost, szReserveCost, sizeof(siReserveCost) );

	// 메시지 박스를 띄우기전 현재 설정 날짜를 기억한다
	m_siReserveYear		= siYear;
	m_siReserveMonth	= siMonth;
	m_siReserveDay		= siDay;
	m_siReserveHour		= siHour;

	NTCHARString64	strTitle	= GetTxtFromMgr( 6694 );
	
	NTCHARString512	strText1	= GetTxtFromMgr( 30061 );
	NTCHARString256	strText2	= GetTxtFromMgr( 30062 );

	strText1.Replace( TEXT("#cost#"), szReserveCost );
	strText1 += strText2;

	stMsgBox MsgBox;
	MsgBox.Set( this, strTitle, strText1, MSGBOX_TYPE_YESNO, 1 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}

void CNWeddingCeremony::OnButtonClickPropose( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 이미 프로포즈 창이 떠있다면 그냥 종료
	if ( pclclient->m_pDialog[NPROPOSE_QUESTION_DLG] )
	{
		return;
	}

	pclclient->CreateInterface( NPROPOSE_QUESTION_DLG );
}

void CNWeddingCeremony::OnButtonClickApply( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 이미 결혼을 했다면
	if ( TRUE == pclchar->IsMarried() )
	{
		return;
	}

}

void CNWeddingCeremony::SetReserveSimpleList( cltWeddingReserveSimpleUnit* pclSimpleList )
{
	if ( NULL == pclSimpleList )
	{
		return;
	}

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		m_clSimpleList[siIndex].Set( &pclSimpleList[siIndex] );
	}

	RefreshReserveList();

}

void CNWeddingCeremony::RefreshReserveList( void )
{
	CList* pclReserve = m_InterfaceFactory.GetList( WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1 );
	if ( NULL == pclReserve )
	{
		return;
	}

	//
	TCHAR				szBuffer[256]	= TEXT("");
	TCHAR*				pszDateText		= GetTxtFromMgr( 3808 );

	SI16				siYear			= 0;
	SI16				siMonth			= 0;
	SI16				siDay			= 0;
	SI16				siHour			= 0;

	stListBoxItemData	stItemDate;

	// 리스트를 먼저 지우고
	pclReserve->Clear();

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		// 초기화
		stItemDate.Init();
		ZeroMemory( szBuffer, sizeof(szBuffer) );

		// 날짜 텍스트 설정
		if ( pszDateText )
		{
			siYear	= m_clSimpleList[siIndex].m_stReserveTime.wYear;
			siMonth = m_clSimpleList[siIndex].m_stReserveTime.wMonth;
			siDay	= m_clSimpleList[siIndex].m_stReserveTime.wDay;
			siHour	= m_clSimpleList[siIndex].m_stReserveTime.wHour;

			StringCchPrintf( szBuffer, 256, pszDateText, siYear, siMonth, siDay, siHour );
		}

		stItemDate.Set( 0, szBuffer );

		// 예식 상태 텍스트 설정
		switch ( m_clSimpleList[siIndex].m_siReserveStatus )
		{
		case WEDDING_RESERVE_STATUS_NONE:		stItemDate.Set( 1, TEXT("") );				break;
		case WEDDING_RESERVE_STATUS_RESERVE:	stItemDate.Set( 1, GetTxtFromMgr(30082) );	break;
		case WEDDING_RESERVE_STATUS_PROGRESS:	stItemDate.Set( 1, GetTxtFromMgr(30083) );	break;
		}

		pclReserve->AddItem( &stItemDate );
	}

	pclReserve->Refresh();

}

void CNWeddingCeremony::SetReserveList( cltWeddingReserveUnit* pclList )
{
	if ( NULL == pclList )
	{
		return;
	}

	if ( NULL == m_pWeddingListDlg )
	{
		return;
	}

	m_pWeddingListDlg->SetList( pclList );
}

void CNWeddingCeremony::RequestList( void )
{
	if ( m_pWeddingListDlg )
	{
		m_pWeddingListDlg->RequestList();
	}

	cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET, 0 );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
}

void CNWeddingCeremony::ChangeTabDialog( SI32 siControllerID )
{
	if ( WEDDINGCEREMONY_MENU_DLG_BUTTON_RESERVE == siControllerID )
	{
		m_pWeddingListDlg->Hide();

		m_InterfaceFactory.GetList(WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1)->Show( true );
		m_InterfaceFactory.GetList(WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1)->GetScroll()->Show( true );
		m_InterfaceFactory.GetButton( WEDDINGCEREMONY_MENU_DLG_BUTTON_PROPOSE )->Show( true );
		m_InterfaceFactory.GetButton( WEDDINGCEREMONY_MENU_DLG_BUTTON_APPLY )->Show( true );
		m_InterfaceFactory.GetEdit( WEDDINGCEREMONY_MENU_DLG_EDITBOX_EXPLAIN )->Show( true );

	}
	else if ( WEDDINGCEREMONY_MENU_DLG_BUTTON_LIST == siControllerID )
	{
		m_pWeddingListDlg->Show();

		m_InterfaceFactory.GetList(WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1)->Show( false );
		m_InterfaceFactory.GetList(WEDDINGCEREMONY_MENU_DLG_LISTBOX_NONAME1)->GetScroll()->Show( false );
		m_InterfaceFactory.GetButton( WEDDINGCEREMONY_MENU_DLG_BUTTON_PROPOSE )->Show( false );
		m_InterfaceFactory.GetButton( WEDDINGCEREMONY_MENU_DLG_BUTTON_APPLY )->Show( false );
		m_InterfaceFactory.GetEdit( WEDDINGCEREMONY_MENU_DLG_EDITBOX_EXPLAIN )->Show( false );
	}
}

