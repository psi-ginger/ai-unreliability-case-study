// EventNotice.cpp: implementation of the CEventNotice class.
//
//////////////////////////////////////////////////////////////////////
#ifdef __SUN_EVENTNOTICE__


#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"

#include "..\..\FontManager\FontManager.h"

#include "../../../lib/WebHTML/WebHTML.h"

#include "NEventNotice.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventNotice::CEventNotice()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_bRectSwitch = FALSE ;

	m_hDlg = NULL ;

	m_hEditBrush = NULL ;
}

CEventNotice::~CEventNotice()
{
}

void CEventNotice::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CEventNotice::StatiCEventNoticeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pEventNotice->EventNoticeProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CEventNotice::EventNoticeProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			g_FontManager.SetFont( hDlg, IDC_EDIT_EVENTNOTICE, 35 );

			m_hEditBrush = CreateSolidBrush(COLOR_DIALOG_LIGHTBLUE);

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_EVENTNOTICE_HTML);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 0, 0, 350, 315, TRUE );
			}

			hwnd = GetDlgItem( hDlg, IDC_EDIT_EVENTNOTICE ) ;
			if ( hwnd )
			{
				MoveWindow( hwnd , 0, 317, 350, 110, TRUE );
			}
		}
		break;
	case WM_SIZE:
		{
			/*
			//============================
			// HTML Resize 를 한다.
			//============================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_EVENTNOTICE_HTML);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}
			*/
			
			return FALSE;
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_EVENTNOTICE ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
				return (BOOL)m_hEditBrush;
			}
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		break;
	case WM_DESTROY:
		{
			m_bRectSwitch = TRUE ;

			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_EVENTNOTICE_HTML );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CEventNotice::Set(char * szGift,char * Txt)
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_EVENTNOTICE ), m_hWnd, StatiCEventNoticeProc );

		MoveWindow( m_hDlg, 336, 126, 352, 450, TRUE );

		/*
		RECT rtDlg;
		RECT rtClient;
		GetClientRect(m_hWnd,&rtClient);
		GetClientRect(m_hDlg,&rtDlg);
		
		MoveWindow(m_hDlg, (rtClient.right-rtClient.left)/2-(rtDlg.right - rtDlg.left)/2,
			(rtClient.bottom-rtClient.top)/2-(rtDlg.bottom - rtDlg.top)/2,
			400, 
			530, TRUE);
		*/
		

		/*
		RECT rtDlg;
		RECT rtClient;
		GetClientRect(m_hWnd,&rtClient);
		GetClientRect(m_hDlg,&rtDlg);

		MoveWindow(m_hDlg,(rtClient.right-rtClient.left)/2-(rtDlg.right - rtDlg.left)/2,
				(rtClient.bottom-rtClient.top)/2-(rtDlg.bottom - rtDlg.top)/2,
				rtDlg.right - rtDlg.left, 
				rtDlg.bottom - rtDlg.top, FALSE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}
		*/

		ShowWindow( m_hDlg, SW_SHOW );
	}

	char buf[256] = "" ;
	char path[512] = "" ;
	GetCurrentDirectory( 256, path );
	int BufLen = strlen( path );
	if( path [ BufLen - 1 ] == '\\' )
	{
		wsprintf(buf, "Interface\\EventNotice\\%s.html", szGift);
		strcat(path,buf);
	}
	else
	{				
		wsprintf(buf, "\\Interface\\EventNotice\\%s.html", szGift);
		strcat(path,buf);
	}

	HWND hStatic = GetDlgItem(m_hDlg,IDC_STATIC_EVENTNOTICE_HTML);	
	DisplayHTMLPage( hStatic, path );

	hStatic = GetDlgItem(m_hDlg,IDC_EDIT_EVENTNOTICE);
	SetWindowText( hStatic, Txt );
}

#endif