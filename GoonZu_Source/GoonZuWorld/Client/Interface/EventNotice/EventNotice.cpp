//// EventNotice.cpp: implementation of the CEventNotice class.
////
////////////////////////////////////////////////////////////////////////
//
//#include "../../Client/client.h"
//
//#include "../../CommonLogic/MsgType.h"
//#include "../../CommonLogic/CommonLogic.h"
//#include "../../CommonLogic/MsgType-System.h"
//
//#include "..\..\FontManager\FontManager.h"
//
//#include "../../../lib/WebHTML/WebHTML.h"
//
//#include "EventNotice.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//CEventNotice::CEventNotice()
//{
//	m_hInst = NULL ;
//	m_hWnd = NULL ;
//
//	m_bRectSwitch = FALSE ;
//
//	m_hDlg = NULL ;
//
//	m_hEditBrush = NULL ;
//}
//
//CEventNotice::~CEventNotice()
//{
//}
//
//void CEventNotice::Create(HINSTANCE hInst, HWND hParent )
//{
//	m_hInst = hInst ;
//	m_hWnd = hParent ;
//}
//
//BOOL CALLBACK CEventNotice::StatiCEventNoticeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pEventNotice->EventNoticeProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CEventNotice::EventNoticeProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	switch( iMsg )
//	{
//	case WM_INITDIALOG:
//		{
//			g_FontManager.SetFont( hDlg, IDC_EDIT_EVENTNOTICE, 25 );
//
//			m_hEditBrush = CreateSolidBrush(COLOR_DIALOG_LIGHTBLUE);
//
//			//---------------------------------
//			// HTML 을 초기화한다.
//			//---------------------------------
//			//==================================
//			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_EVENTNOTICE_HTML);
//			if ( hwnd )
//			{
//				EmbedBrowserObject(hwnd);
//				MoveWindow( hwnd, 0, 0, 350, 315, TRUE );
//			}
//
//			hwnd = GetDlgItem( hDlg, IDC_EDIT_EVENTNOTICE ) ;
//			if ( hwnd )
//			{
//				MoveWindow( hwnd , 0, 317, 350, 110, TRUE );
//			}
//		}
//		break;
//	case WM_SIZE:
//		{
//			/*
//			//============================
//			// HTML Resize 를 한다.
//			//============================			
//			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_EVENTNOTICE_HTML);
//			if( hwnd )
//			{
//				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
//			}
//			*/
//			
//			return FALSE;
//		}
//		break;
//	case WM_CTLCOLORSTATIC:
//		{
//			if ( GetDlgItem( hDlg, IDC_EDIT_EVENTNOTICE ) == (HWND)lParam )
//			{
//				HDC hdc = (HDC)wParam;
//				SetTextColor( hdc, COLOR_DIALOG_BLACK );
//				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
//				return (BOOL)m_hEditBrush;
//			}
//		}
//		break;
//	case WM_CLOSE:
//		{
//			DestroyWindow(m_hDlg);
//		}
//		break;
//	case WM_DESTROY:
//		{
//			m_bRectSwitch = TRUE ;
//
//			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_EVENTNOTICE_HTML );
//			if( hwnd )
//			{
//				DoPageAction(hwnd, WEBPAGE_STOP);
//				UnEmbedBrowserObject(hwnd);
//			}
//
//			if ( m_hEditBrush )
//			{
//				DeleteObject( m_hEditBrush );
//				m_hEditBrush = NULL;
//			}
//
//			m_hDlg = NULL ;
//		}
//		break;
//	}
//
//	return FALSE;
//}
//
//void CEventNotice::Set(TCHAR * szGift,TCHAR * Txt)
//{
//	if ( !IsWindow(m_hDlg) )
//	{
//		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_EVENTNOTICE ), m_hWnd, StatiCEventNoticeProc );
//
//		MoveWindow( m_hDlg, 336, 126, 352, 450, TRUE );
//
//		ShowWindow( m_hDlg, SW_SHOW );
//	}
//
//	TCHAR buf[256] = TEXT("") ;
//	TCHAR path[512] = TEXT("") ;
//	GetCurrentDirectory( 256, path );
//	int BufLen = _tcslen( path );
//	if( path [ BufLen - 1 ] == '\\' )
//	{
//		StringCchPrintf(buf, 256, TEXT("Interface\\EventNotice\\%s.html"), szGift);
//		StringCchCat(path, 512, buf);
//	}
//	else
//	{				
//		StringCchPrintf(buf, 256, TEXT("\\Interface\\EventNotice\\%s.html"), szGift);
//		StringCchCat(path, 512, buf);
//	}
//
//	HWND hStatic = GetDlgItem(m_hDlg,IDC_STATIC_EVENTNOTICE_HTML);	
//	DisplayHTMLPage( hStatic, path );
//
//	hStatic = GetDlgItem(m_hDlg,IDC_EDIT_EVENTNOTICE);
//	SetWindowText( hStatic, Txt );
//}