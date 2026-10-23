//#include "NotifyMsgInputDlg.h"
//
//CNotifyMsgInputDlg g_NotifyMsgInputDlg;
//
//CNotifyMsgInputDlg::CNotifyMsgInputDlg()
//{
//	m_hDlg = NULL;
//}
//
//CNotifyMsgInputDlg::~CNotifyMsgInputDlg()
//{
//	if ( m_hDlg )
//	{
//		DestroyWindow( m_hDlg );
//	}
//}
//
//void CNotifyMsgInputDlg::Create( HINSTANCE hInst, HWND hWnd )
//{
//	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_MSG_INPUT ), hWnd, StaticNotifyMsgInputDlgProc );
//
//	Hide();
//
//	return;
//}
//
//void CNotifyMsgInputDlg::Initialize()
//{
//	return;
//}
//
//void CNotifyMsgInputDlg::Destroy()
//{
//	return;
//}
//
//BOOL CALLBACK CNotifyMsgInputDlg::StaticNotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_NotifyMsgInputDlg.NotifyMsgInputDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CNotifyMsgInputDlg::NotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch( iMsg )
//	{
//	case WM_INITDIALOG:
//		{
//			m_hDlg = hDlg;
//		}
//		break;
//
//	case WM_TIMER:
//		{
//		}
//		break;
//
//	case WM_COMMAND:
//		{
//			switch ( LOWORD( wParam ) )
//			{
//			case IDOK:
//				{
//					TCHAR Msg[ 256 ] = TEXT("");
//					GetDlgItemText( hDlg, IDD_DIALOG_MSG_INPUT, Msg, 256);
//				}
//				break;
//			}
//		}
//		break;
//
//	case WM_DESTROY:
//		{
//			m_hDlg = NULL;
//		}
//		break;
//	}
//	return FALSE;
//}
//
//
//void CNotifyMsgInputDlg::Show()
//{
//	ShowWindow( m_hDlg, SW_SHOW );
//	return;
//}
//
//void CNotifyMsgInputDlg::Hide()
//{
//	ShowWindow( m_hDlg, SW_HIDE );
//	return;
//}