//// WebEventDlg.cpp: implementation of the CWebEventDlg class.
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
//#include "WebEventDlg.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//CWebEventDlg::CWebEventDlg()
//{
//	m_hInst = NULL ;
//	m_hWnd = NULL ;
//
//	m_hDlg = NULL ;
//	
//	m_pWebEventButtonDlg = NULL ;
//}
//
//CWebEventDlg::~CWebEventDlg()
//{
//	m_hInst = NULL ;
//	m_hWnd = NULL ;
//
//	if ( m_pWebEventButtonDlg )
//	{
//		delete m_pWebEventButtonDlg ;
//		m_pWebEventButtonDlg = NULL ;
//	}
//}
//
//void CWebEventDlg::Create(HINSTANCE hInst, HWND hParent )
//{
//	m_hInst = hInst ;
//	m_hWnd = hParent ;
//
//	SI32 siCY = GetSystemMetrics(SM_CYCAPTION);
//	m_pWebEventButtonDlg = new CWebEventButtonDlg(203,siCY+30);
//}
//
//BOOL CALLBACK CWebEventDlg::StatiCWebEventDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pWebEventDlg->WebEventDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CWebEventDlg::WebEventDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
//			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_WEBEVENT_HTML);
//			if ( hwnd )
//			{
//				EmbedBrowserObject(hwnd);
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
//	case WM_CLOSE:
//		{
//			DestroyWindow(m_hDlg);
//		}
//		break;
//	case WM_DESTROY:
//		{
//			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_WEBEVENT_HTML );
//			if( hwnd )
//			{
//				UnEmbedBrowserObject(hwnd);
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
//BOOL CWebEventDlg::Set(TCHAR * url,SI32 xsize,SI32 ysize)
//{
//	if ( !IsWindow(m_hDlg) )
//	{
//		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_WEBEVENT ), m_hWnd, StatiCWebEventDlgProc );
//
//		RECT rt;
//		GetClientRect(m_hDlg,&rt);
//		MoveWindow(m_hDlg,rt.left,rt.top,xsize+8,ysize+8,TRUE);
//
//		HWND hwnd = GetDlgItem(m_hDlg, IDC_STATIC_WEBEVENT_HTML);
//		MoveWindow(hwnd,0,0,xsize,ysize,TRUE);
//		DisplayHTMLPage(hwnd,url);
//
//		ShowWindow( m_hDlg, SW_SHOW );
//		return TRUE ;
//	}
//
//	return FALSE ;
//}
//
//void CWebEventDlg::GetSBSSchoolEventUrl(TCHAR *url,TCHAR * accountID,const TCHAR * name)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	if ( name == NULL || !_tcscmp(name, TEXT("NOPERSONNAME")) ) return ;
//	if ( accountID == NULL || !_tcscmp(accountID, TEXT("")) ) return ;
//
//	TCHAR buffer[256] = TEXT("") ;
//	TCHAR server[20] = TEXT("") ;
//
//	StringCchCopy(buffer, 256, pclclient->clSelectedWorldInfo.baseinfo.worldname );
//	if ( !_tcscmp(buffer, TEXT("")) ) return ;
//
//	int i = 0 ;
//	bool loop = true;
//	while(loop)
//	{
//		if ( buffer[i] == '(' || i == 10 )
//			break ;
//		i ++ ;
//	}
//	memcpy(server,buffer,i);
//	server[i] = NULL ;
//#ifdef _DEBUG
//	StringCchCopy(server, 20, TEXT("성종"));
//#endif
//
//	//StringCchPrintf(buffer, 256, TEXT("center/hq/school/event_main.asp?user_server=%s&user_character=%s"),
//	//				server,name);
//
//	//pclClient->GetSubSiteUrl(url,buffer,accountID );
//}