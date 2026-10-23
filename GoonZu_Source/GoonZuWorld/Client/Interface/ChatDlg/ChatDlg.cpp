//#include "ChatDlg.h"
//#include "ChatDataMgr.h"
//#include "..\..\FontManager\FontManager.h"
//
//
//CChatDlg	g_ChatDlg;
//
//CChatDlg::CChatDlg()
//{	
//	m_hChatViewBrush = NULL;
//	m_hChatInputBrush = NULL;
//	m_hDlg = NULL;
//	m_bShowFlag = false;
//
//	m_hChatDataViewEdit = NULL;
//	m_hChatDataInputEdit = NULL;
//
//	m_pChatDataViewMgr = new CChatDataMgr( true );
//	m_pChatDataViewMgr->CreateMaxEmptyObj( MAX_CHAT_DATA_RECORD_NUM );
//
//	m_pChatDataSendMgr = new CChatDataMgr( false );
//	m_pChatDataSendMgr->CreateMaxEmptyObj( MAX_CHAT_DATA_BUFFER_NUM );
//
//	m_strChatViewData = new TCHAR[ ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM ];
//	memset( m_strChatViewData, 0, ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM );
//
//	Initialize();
//}
//
//CChatDlg::~CChatDlg()
//{
//	Destroy();
//
//	if ( m_hDlg )
//	{
//		DestroyWindow( m_hDlg );
//	}
//}
//
//void CChatDlg::Create( HINSTANCE hInst, HWND hWnd )
//{
//	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_CHAT ), hWnd, StaticChatDlgProc );
//	Hide();
//	return;
//}
//
//void CChatDlg::Initialize()
//{
//	return;
//}
//
//void CChatDlg::Destroy()
//{
//	if ( m_pChatDataViewMgr )
//	{
//		delete m_pChatDataViewMgr;
//		m_pChatDataViewMgr = NULL;
//	}
//
//	if ( m_pChatDataSendMgr )
//	{
//		delete m_pChatDataSendMgr;
//		m_pChatDataSendMgr = NULL;
//	}
//
//	if ( m_strChatViewData )
//	{
//		delete m_strChatViewData;
//		m_strChatViewData = NULL;
//	}	
//
//	return;
//}
//
//LRESULT CALLBACK CChatDlg::StaticEditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    return g_ChatDlg.EditSubProc( hWnd, msg, wParam, lParam );
//}
//
//
//LRESULT CALLBACK CChatDlg::EditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch(msg) 
//	{
//	case WM_CHAR :
//		if( wParam == VK_RETURN )
//		{
//			if ( m_hDlg )
//			{
//				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
//				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH );
//
//				// 스트링의 길이가 0보다 큰 경우만 보냄. 
//				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
//				if ( _tcslen( m_strChatData ) > 0 )
//				{
//					Push();
//					SetWindowText( m_hChatDataInputEdit, TEXT("") );
//				}
//			}
//		}
//		else if (wParam == VK_ESCAPE )
//		{
//			SetWindowText( m_hChatDataInputEdit, TEXT("") );
//		}
//
//		break;
//	default :
//		break;
//    }
//
//    return CallWindowProc( m_OldEditProc, hWnd, msg, wParam, lParam );
//}
//
//BOOL CALLBACK CChatDlg::StaticChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_ChatDlg.ChatDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CChatDlg::ChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch( iMsg )
//	{
//
//	case WM_INITDIALOG:
//		{
//			m_hDlg = hDlg;
//			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
//			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );
//
//			m_hChatDataViewEdit = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
//			m_hChatDataInputEdit = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );
//
//			m_hChatViewBrush = CreateSolidBrush( COLOR_DIALOG_WHITE );
//			m_hChatInputBrush = CreateSolidBrush( COLOR_DIALOG_WHITE );
//
//			SendMessage( m_hChatDataInputEdit, EM_LIMITTEXT, MAX_CHAT_DATA_LENGTH - 2, 0 );
//
//			m_OldEditProc = (WNDPROC)SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG)StaticEditSubProc);
//
//			return TRUE;
//		}
//		break;
//	
//	case WM_COMMAND:
//		{
//			switch ( LOWORD( wParam ) )
//			{
//			case IDCANCEL:
//				{
//					Hide();
//				}
//				break;
//			}
//		}
//		break;
//	
//	case WM_CTLCOLORSTATIC:				// 에디트 박스가 disable 이거나 read only 일 경우 이리로 옮
//		{
//			if ( m_hChatDataViewEdit == ( HWND ) lParam )
//			{
//				
//				HDC hdc = ( HDC ) wParam;
//				
//				//SetBkMode( hdc, TRANSPARENT );			//<---- 이거 들어가면 스크롤 시에 글자가 겹치는 이상한 문제 발생. 그래서 주석처리 했음.
//				SetTextColor( hdc, COLOR_DIALOG_BLACK );
//				SetBkColor( hdc, COLOR_DIALOG_WHITE );
//		
//
//				
//				return (BOOL)m_hChatViewBrush;
//			}
//		}
//		break;
//
//  
//	case WM_CTLCOLOREDIT:
//		{
//			if ( m_hChatDataInputEdit == ( HWND ) lParam )
//			{
//				HDC hdc = ( HDC ) wParam;
//
//				SetBkMode( hdc, TRANSPARENT );
//				SetTextColor( hdc, COLOR_DIALOG_BLACK );
//				SetBkColor( hdc, COLOR_DIALOG_WHITE );
//	
//
//				return (BOOL)m_hChatInputBrush;
//			}
//		}
//		break;
//	
//
//	case WM_CLOSE:
//		{
//			Hide();
//		}
//		break;
//
//	case WM_DESTROY:
//		{
//			if ( m_hChatViewBrush )
//			{
//				DeleteObject( m_hChatViewBrush );
//				m_hChatViewBrush = NULL;
//			}
//
//			if ( m_hChatInputBrush )
//			{
//				DeleteObject( m_hChatInputBrush );
//				m_hChatInputBrush = NULL;
//			}
//
//			SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG) m_OldEditProc );
//			Hide();
//			
//			m_hDlg = NULL;
//		}
//		break;
//	}
//
//	return FALSE;
//}
//
//void CChatDlg::LoadData()
//{
//	m_pChatDataViewMgr->LoadData( m_strChatViewData , ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM);
//
//	//SetWindowText( m_hChatDataViewEdit, m_strChatViewData );
//
//	SetWindowText( m_hChatDataViewEdit, TEXT("") );
//	SendMessage( m_hChatDataViewEdit, EM_SETSEL, (WPARAM)-1, (LPARAM)-1 );
//	SendMessage( m_hChatDataViewEdit, EM_REPLACESEL, 0, (LPARAM)m_strChatViewData );
//
//	return;
//}
//
//// 다이얼로그 보여 주기	
//void CChatDlg::Show()
//{
//	if ( m_bShowFlag )
//	{
//		return;
//	}
//
//	m_bShowFlag = true;	
//	ShowWindow ( m_hDlg, SW_SHOWNORMAL );
//
//	return;
//}
//
//// 다이얼로그 숨기기
//void CChatDlg::Hide()
//{
//	if ( ! m_bShowFlag )
//	{
//		return;
//	}
//
//	m_bShowFlag = false;	
//	ShowWindow( m_hDlg, SW_HIDE );
//
//	return;
//}
//
//bool CChatDlg::IsShow()
//{
//	return m_bShowFlag;
//}
//
//void CChatDlg::Push()
//{
//	m_pChatDataSendMgr->Push( CHAT_DATA_SEND, m_strChatData );
//	
//	return;
//}
//
//void CChatDlg::Push( TCHAR *strUserID, TCHAR *strChatRecvData )
//{
//	m_pChatDataViewMgr->Push( strUserID, strChatRecvData );
//
//	LoadData();
//	return;
//}
//
//bool CChatDlg::Pop( stChatStatusBarData *pChatStatusBarData )
//{
//	return m_pChatDataSendMgr->Pop( pChatStatusBarData );
//}
