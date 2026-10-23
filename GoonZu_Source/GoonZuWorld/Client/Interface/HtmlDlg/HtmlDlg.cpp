//// HtmlDlg.cpp: implementation of the CHtmlDlg class.
////
////////////////////////////////////////////////////////////////////////
//
//#include "../../Client/client.h"
//
//#include "../../CommonLogic/MsgType.h"
//#include "../../CommonLogic/CommonLogic.h"
//#include "../../CommonLogic/MsgType-System.h"
//
//#include "../../../lib/WebHTML/WebHTML.h"
//
//#include "HtmlDlg.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//SI32 m_siIndex;
//SI32 m_siXsize;
//SI32 m_siYsize;
//
//CHtmlDlg::CHtmlDlg()
//{
//	m_hParent = NULL ;
//	m_hInst = NULL ;
//
//	m_hDlg = NULL ;
//	m_bShow = FALSE ;
//
//	m_siIndex = 0 ;
//
//	m_siXsize = 0 ;
//	m_siYsize = 0 ;
//}
//
//CHtmlDlg::~CHtmlDlg()
//{
//}
//
//void CHtmlDlg::Create(HINSTANCE hInst, HWND hParent ,SI32 index)
//{
//	m_siIndex = index ;
//
//	m_hParent = hParent ;
//	m_hInst = hInst ;
//}
//
//BOOL CALLBACK CHtmlDlg::StatiCHtmlDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pHtmlDlgManager->m_pCHtmlDlg[m_siIndex]->HtmlDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CHtmlDlg::HtmlDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	switch( iMsg )
//	{
//	case WM_INITDIALOG:
//		{
//			//---------------------------------
//			// HTML 을 초기화한다.
//			//---------------------------------
//			//==================================
//			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_HTMLNOTICE_HTML);
//			if ( hwnd )
//			{
//				MoveWindow( hwnd, 0, 0, m_siXsize, m_siYsize, TRUE );
//				EmbedBrowserObject(hwnd);
//			}
//		}
//		break;
//	case WM_CLOSE:
//		{
//			DestroyWindow(hDlg);
//		}
//		break;
//	case WM_DESTROY:
//		{
//			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_HTMLNOTICE_HTML );
//			if( hwnd )
//			{
//				UnEmbedBrowserObject(hwnd);
//			}
//
//			m_bShow = FALSE ;
//			m_hDlg = NULL ;
//		}
//		break;
//	}
//
//	return FALSE;
//}
//
//void CHtmlDlg::Set(TCHAR * Url,TCHAR * Txt,bool bWeb,SI32 x,SI32 y,SI32 xsize,SI32 ysize)
//{
//	m_siXsize = xsize ;
//	m_siYsize = ysize ;
//
//	m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_HTMLNOTICE ), m_hParent, StatiCHtmlDlgProc );
//
//	MoveWindow( m_hDlg, x, y, xsize, ysize, TRUE );
//
//	TCHAR path[512] = TEXT("") ;
//
//	if ( !bWeb )
//	{
//		TCHAR buf[256] = TEXT("") ;
//		GetCurrentDirectory( 256, path );
//		int BufLen = _tcslen( path );
//		if( path [ BufLen - 1 ] == '\\' )
//		{
//			StringCchPrintf(buf, 256, TEXT("%s"), Url);
//			StringCchCat(path, 512, buf);
//		}
//		else
//		{
//			StringCchPrintf(buf, 256, TEXT("\\%s"), Url);
//			StringCchCat(path, 512, buf);
//		}
//	}
//	else
//	{
//		StringCchCopy(path, 512, Url);
//	}
//
//	SetWindowText(m_hDlg,Txt);
//	DisplayHTMLPage( GetDlgItem(m_hDlg,IDC_STATIC_HTMLNOTICE_HTML), path );
//
//	Show();
//}
//
//void CHtmlDlg::Show()
//{
//	m_bShow = TRUE ;
//	ShowWindow(m_hDlg,SW_SHOW);
//}
