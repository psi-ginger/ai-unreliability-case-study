
#include "NWeddingCeremonyListDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../CommonLogic/Msg/MsgType-Wedding.h"
#include "../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNWeddingCeremonyListDlg::CNWeddingCeremonyListDlg( void )
{
	m_siRequestPage = 0;

	m_siCancelYear	= 0;
	m_siCancelMonth	= 0;
	m_siCancelDay	= 0;
	m_siCancelHour	= 0;

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_PAGE_PER_NUM; siIndex++ )
	{
		m_clList[siIndex].Init();
	}
}

CNWeddingCeremonyListDlg::~CNWeddingCeremonyListDlg( void )
{

}

void CNWeddingCeremonyListDlg::Init( )
{
		
}

void CNWeddingCeremonyListDlg::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NWeddingCeremony/DLG_NWeddingCeremony_List.ddf" ) );
	file.CreateChildDialog( this, NWEDDINGCEREMONY_LIST_DLG, TEXT( "dialog_weddingceremony_list" ), StaticCallBackDialogNWeddingCeremonyListDlg, pParent );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_LIST_DLG_BUTTON_CANCEL,		this), WEDDINGCEREMONY_LIST_DLG_BUTTON_CANCEL,		TEXT("button_cancel") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_LIST_DLG_BUTTON_ENTER,		this), WEDDINGCEREMONY_LIST_DLG_BUTTON_ENTER,		TEXT("button_enter") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_LIST_DLG_BUTTON_PREVIOUS,	this), WEDDINGCEREMONY_LIST_DLG_BUTTON_PREVIOUS,	TEXT("button_previous") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	WEDDINGCEREMONY_LIST_DLG_BUTTON_NEXT,		this), WEDDINGCEREMONY_LIST_DLG_BUTTON_NEXT,		TEXT("button_next") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	WEDDINGCEREMONY_LIST_DLG_LISTBOX_LIST,		this), WEDDINGCEREMONY_LIST_DLG_LISTBOX_LIST,		TEXT("listbox_list") );

	CList* pclList = m_InterfaceFactory.GetList( WEDDINGCEREMONY_LIST_DLG_LISTBOX_LIST );
	if ( pclList )
	{
		pclList->SetBorder( true );
		pclList->SetBKColorUse( true );

		pclList->SetTextPrintOption( DT_LEFT );

		pclList->SetColumn( 0, pclList->GetWidth(),	TEXT("") );
		pclList->SetLineHeight( 58 );
	
		pclList->Refresh();
	}

	RequestList();

}

void CALLBACK CNWeddingCeremonyListDlg::StaticCallBackDialogNWeddingCeremonyListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWeddingCeremonyListDlg* pThis = (CNWeddingCeremonyListDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNWeddingCeremonyListDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNWeddingCeremonyListDlg::CallBackDialogNWeddingCeremonyListDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{

	case WEDDINGCEREMONY_LIST_DLG_BUTTON_CANCEL:
		{
			OnButtonClickCancel();
		}
		break;

	case WEDDINGCEREMONY_LIST_DLG_BUTTON_ENTER:
		{
			OnButtonClickEnter();
		}
		break;

	case WEDDINGCEREMONY_LIST_DLG_BUTTON_PREVIOUS:
		{
			OnButtonClickPrevious();	
		}
		break;

	case WEDDINGCEREMONY_LIST_DLG_BUTTON_NEXT:
		{
			OnButtonClickNext();	
		}
		break;
	}
}

void CNWeddingCeremonyListDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 2:	
		{
			if ( true == RetValue )
			{
				cltGameMsgRequest_WeddingReserveCancel clinfo( m_siCancelYear, m_siCancelMonth, m_siCancelDay, m_siCancelHour );
				cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_RESERVE_CANCEL, sizeof(clinfo), (BYTE*)&clinfo );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
	}
}


void CNWeddingCeremonyListDlg::RequestList( void )
{
	cltGameMsgRequest_WeddingListGet clinfo( m_siRequestPage );
	cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_LIST_GET, sizeof(clinfo), (BYTE*)&clinfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

}

void CNWeddingCeremonyListDlg::SetList( cltWeddingReserveUnit* pclList )
{
	if ( NULL == pclList )
	{
		return;
	}

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_PAGE_PER_NUM; siIndex++ )
	{
		m_clList[siIndex].Set( &pclList[siIndex] );
	}

	RefreshList();
}

void CNWeddingCeremonyListDlg::RefreshList( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList( WEDDINGCEREMONY_LIST_DLG_LISTBOX_LIST );
	if ( NULL == pclList )
	{
		return;
	}

	// 
	SI16				siCurrentYear	= pclclient->sTime.wYear;
	SI16				siCurrentMonth	= pclclient->sTime.wMonth;
	SI16				siCurrentDay	= pclclient->sTime.wDay;
	SI16				siCurrentHour	= pclclient->sTime.wHour;

	//
	NTCHARString256		strText;
	stListBoxItemData	stItemDate;

	SI32				siYear			= 0;
	SI32				siMonth			= 0;
	SI32				siDay			= 0;
	SI32				siHour			= 0;

	// 리스트를 먼저 지우고
	pclList->Clear();

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_PAGE_PER_NUM; siIndex++ )
	{
		// 초기화
		strText = GetTxtFromMgr( 30091 );

		stItemDate.Init();

		// 날짜
		siYear	= m_clList[siIndex].m_stReserveTime.wYear;
		siMonth = m_clList[siIndex].m_stReserveTime.wMonth;
		siDay	= m_clList[siIndex].m_stReserveTime.wDay;
		siHour	= m_clList[siIndex].m_stReserveTime.wHour;

		// 날짜가 없을때는 안보여준다
		if ( 1000 > siYear )
		{
			continue;
		}

		strText.Replace( TEXT("#year#"),	SI32ToString(siYear) );
		strText.Replace( TEXT("#month#"),	SI32ToString(siMonth) );
		strText.Replace( TEXT("#day#"),		SI32ToString(siDay) );
		strText.Replace( TEXT("#hour#"),	SI32ToString(siHour) );

		// 예식 상태 텍스트 설정
		if ( TRUE == m_clList[siIndex].IsReserved() )
		{
			if ( TRUE == m_clList[siIndex].IsSameTime(siCurrentYear, siCurrentMonth, siCurrentDay, siCurrentHour) )
			{
				strText.Replace( TEXT("#status#"), GetTxtFromMgr(30083) );
			}
			else
			{
				strText.Replace( TEXT("#status#"), GetTxtFromMgr(30082) );
			}
		}
		else
		{
			strText.Replace( TEXT("#status#"), TEXT("") );
		}

		// 이름 설정
		strText.Replace( TEXT("#name1#"), m_clList[siIndex].m_szMate1_Name );
		strText.Replace( TEXT("#name2#"), m_clList[siIndex].m_szMate2_Name );
		
		stItemDate.Set( 0, strText );

		pclList->AddItem( &stItemDate );
	}

	pclList->Refresh();

}

void CNWeddingCeremonyListDlg::OnButtonClickCancel( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CList* pclList = m_InterfaceFactory.GetList(WEDDINGCEREMONY_LIST_DLG_LISTBOX_LIST);
	if ( NULL == pclList )
	{
		return;
	}

	// 선택된것을 확인
	SI32 siSelectedIndex = pclList->GetSelectedIndex();
	if ( (0 > siSelectedIndex) || (WEDDING_RESERVE_LIST_PAGE_PER_NUM <= siSelectedIndex) )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(1868);
		TCHAR* pszText	= GetTxtFromMgr(30064);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	SI32 siMyPersonID = pclChar->pclCI->GetPersonID();
	if ( 0 >= siMyPersonID )
	{
		return;
	}

	cltWeddingReserveUnit* pclUnit = &m_clList[siSelectedIndex];
	if ( NULL == pclUnit )
	{
		return;
	}

	// 예약된것도 아닌데 지우려 한다면
	if ( FALSE == pclUnit->IsReserved() )
	{
		return;
	}

	SI16 siServerYear	= pclclient->sTime.wYear;
	SI16 siServerMonth	= pclclient->sTime.wMonth;
	SI16 siServerDay	= pclclient->sTime.wDay;
	SI16 siServerHour	= pclclient->sTime.wHour;

	// 현재 시간의 예약은 취소 할수없음
	if ( TRUE == pclUnit->IsSameTime(siServerYear, siServerMonth, siServerDay, siServerHour) )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(1868);
		TCHAR* pszText	= GetTxtFromMgr(30068);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	// 내가 한 예약이 아니라면
	if ( (siMyPersonID != pclUnit->m_siMate1_PersonID) && (siMyPersonID != pclUnit->m_siMate2_PersonID) )
	{
		TCHAR* pszTitle	= GetTxtFromMgr(1868);
		TCHAR* pszText	= GetTxtFromMgr(30069);

		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pszTitle, pszText );
		return;
	}

	m_siCancelYear	= pclUnit->m_stReserveTime.wYear;
	m_siCancelMonth	= pclUnit->m_stReserveTime.wMonth;
	m_siCancelDay	= pclUnit->m_stReserveTime.wDay;
	m_siCancelHour	= pclUnit->m_stReserveTime.wHour;

	NTCHARString128 strText = GetTxtFromMgr(30066);

	stMsgBox MsgBox;
	MsgBox.Set( this, TEXT(""), strText, MSGBOX_TYPE_YESNO, 2 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );
	
}

void CNWeddingCeremonyListDlg::OnButtonClickEnter( void )
{
	cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_HALL_ENTER, 0 );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
}
 
void CNWeddingCeremonyListDlg::OnButtonClickPrevious( void )
{
	if ( 0 > (m_siRequestPage-1) )
	{
		return;
	}

	m_siRequestPage--;

	RequestList();
}

void CNWeddingCeremonyListDlg::OnButtonClickNext( void )
{
	if ( WEDDING_RESERVE_LIST_NUM <= ((m_siRequestPage+1) * WEDDING_RESERVE_LIST_PAGE_PER_NUM) )
	{
		return;
	}

	m_siRequestPage++;

	RequestList();
}
