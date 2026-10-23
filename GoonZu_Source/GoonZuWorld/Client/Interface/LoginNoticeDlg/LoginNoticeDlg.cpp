// LoginNoticeDlg.cpp: implementation of the CLoginNoticeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "Client.h"
#include "LoginNoticeDlg.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../../../CommonLogic/CommonLogic.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../common/Char/CharCommon/Char-Common.h"

#include "../../Client/ninterface/NMusicDlg/NMusicDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginNoticeDlg		g_LoginNoticeDlg;

extern cltCommonLogic* pclClient;

CLoginNoticeDlg::CLoginNoticeDlg()
{
	m_siType = OTHER_DLG;
	m_bShow = false;
	m_hLoginNoticeDlg = NULL ;

	ZeroMemory(m_szURL, MAX_BILL_PAGGE_URL_LENGTH);
}

CLoginNoticeDlg::~CLoginNoticeDlg()
{
	if ( m_hLoginNoticeDlg )
	{
		DestroyWindow(m_hLoginNoticeDlg);
	}
}

void CLoginNoticeDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hLoginNoticeDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_LOGINNOTICE ), hParent, StatiCLoginNoticeDlgProc );
	
	Hide();
}

BOOL CALLBACK CLoginNoticeDlg::StatiCLoginNoticeDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_LoginNoticeDlg.LoginNoticeDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CLoginNoticeDlg::LoginNoticeDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================
			EmbedBrowserObject(hDlg);
			//==================================
		}
		break;

	case WM_SIZE:
		{
			//============================
			// HTML Resize 를 한다.
			//============================
			//==================================
			ResizeBrowser(hDlg,  LOWORD(lParam), HIWORD(lParam) );
			//==================================
		}
		break;

	case WM_TIMER:
		{
		}
		break;
	case WM_COMMAND:
		{
			switch( wParam )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{

			Hide();			
		}
		break;

	case WM_DESTROY:
		{
			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			UnEmbedBrowserObject(hDlg);
			//==================================
			
			m_hLoginNoticeDlg = NULL;
		}
		break;
	}

	return FALSE;
}

void CLoginNoticeDlg::Set(TCHAR *url, int width, int height, SI32 Type, TCHAR* Name )
{
	StringCchCopy(m_szURL, MAX_BILL_PAGGE_URL_LENGTH, url);

	m_siType = Type;

	if( Name != NULL )
	{
		SetWindowText( m_hLoginNoticeDlg, Name );

	}
	else
	{
		SetWindowText( m_hLoginNoticeDlg, "" );
	}

	SI32 startx = 0;
	SI32 starty = 0;
	SI32 screenXSize = 0;
	SI32 screenYSize = 0;
	cltClient *pclclient = (cltClient*)pclClient;

	if (g_bFullScreen)
	{
		switch( pclclient->siScreenMode )
		{
		case MODE_GRAPHICS800_600:
			screenXSize = 800;
			screenYSize = 600;
			break;

		case MODE_GRAPHICS1024_768:
			screenXSize = 1024;
			screenYSize = 768;
			break;
		// KHY - 1218 - 1024x600 해상도 추가. imsi
		case MODE_GRAPHICS1024_600:
			screenXSize = 1024;
			screenYSize = 600;
			break;

		case MODE_GRAPHICS1280_1024:
			screenXSize = 1280;
			screenYSize = 1024;
			break;
		}

	}
	else
	{
		RECT rect;
		pclclient->GetClientSize(&rect);
		
		screenXSize = GetSystemMetrics(SM_CXSCREEN);;
		screenYSize = GetSystemMetrics(SM_CYSCREEN);
	}

	startx = (screenXSize - width) / 2;
	starty = (screenYSize - height) / 2;

	// Tys - 타이틀바등의 크기를 고려해서 윈도우 크기 조절
	RECT rtWindow, rtClient;
	GetWindowRect( m_hLoginNoticeDlg, &rtWindow);
	GetClientRect( m_hLoginNoticeDlg, &rtClient);
	int nWidth = ( rtWindow.right - rtWindow.left ) -  rtClient.right + width;
	int nHeight = ( rtWindow.bottom - rtWindow.top ) - rtClient.bottom + height;

	MoveWindow( m_hLoginNoticeDlg, startx, starty, nWidth, nHeight, true);

	Show();

	return;
}

void CLoginNoticeDlg::Show()
{
	//TCHAR url[256] = TEXT("") ;
	//pclClient->GetSubSiteUrl(url,TEXT("center/hq/notice/hq_notice.html"),pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID) ;
	
	if (m_szURL)
	{
		m_bShow = true;
		DisplayHTMLPage(m_hLoginNoticeDlg, m_szURL );
		ShowWindow( m_hLoginNoticeDlg, SW_SHOWNORMAL );

	}

}

void CLoginNoticeDlg::Hide()
{
	m_bShow = false;
	DisplayHTMLPage(m_hLoginNoticeDlg, "about:blank" );
	ShowWindow( m_hLoginNoticeDlg, SW_HIDE );

	cltClient *pclclient = (cltClient*)pclClient;
	if( pclclient->m_pDialog[NMUSIC_DLG] )
	{
		((CNMusicDlg*)pclclient->m_pDialog[NMUSIC_DLG])->SetState( CNMusicDlg::MAKE_MUSICWINDOW );
	}
}

void CLoginNoticeDlg::SetShowWindow( bool Flag )
{
	m_bShow = Flag;
	if( Flag )
	{
		ShowWindow( m_hLoginNoticeDlg, SW_SHOWNORMAL );
	}
	else
	{
		ShowWindow( m_hLoginNoticeDlg, SW_HIDE );
	}
}
