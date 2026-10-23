#include "GomZiDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../Music/Music.h"
#include "../SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


#include "../../../lib/WebHTML/WebHTML.h"

#include <wininet.h>

extern cltCommonLogic* pclClient;


CGomZiDlg::CGomZiDlg()
{
	m_hDlg = NULL;
	m_hDlgChild = NULL;
	m_bShow = false;

}

CGomZiDlg::~CGomZiDlg()
{	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CGomZiDlg::Init()
{
	return;
}

void CGomZiDlg::Create( HINSTANCE hInst, HWND hParent )
{
	/*
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_GOMZI ), hParent, StaticGomZiDlgProc );
	
	Hide();
	*/
	WNDCLASSEX		wc;
	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.hInstance	= hInst;
	wc.lpfnWndProc	= StaticGomZiDlgProc;
	wc.style		= CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;//CS_HREDRAW|CS_VREDRAW; 
	wc.lpszClassName="GomZiFrame";
	wc.hIcon		= LoadIcon( hInst, MAKEINTRESOURCE(IDI_ICON_GOONZU));
	wc.hCursor		= LoadCursor( hInst, IDC_ARROW );
	wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	
	if(RegisterClassEx(&wc)==NULL)
	{
		MessageBox(NULL, "ERROR GomZiFrame SEARCH", "GomZiFrame", MB_OK|MB_TOPMOST);
	}

	wc.lpfnWndProc	= StaticBrowserWindowProc;
	//wc.style		= CS_HREDRAW|CS_VREDRAW;
	wc.lpszClassName = "GomZi";

	if(RegisterClassEx(&wc)==NULL)
	{
		MessageBox(NULL, "ERROR GomZi CHILD", "GomZi", MB_OK|MB_TOPMOST);
	}

	char* pText = GetTxtFromMgr(2190);

	// 아래에 깔리는 다이얼로그 프레임
	if ( ( m_hDlg = CreateWindowEx(0, "GomZiFrame", pText, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,//WS_CHILD|WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, 736, 636, HWND_DESKTOP/*hParent*/, NULL, hInst, 0) ) != NULL ) // HWND_DESKTOP
			//CW_USEDEFAULT, CW_USEDEFAULT, 760, 666, HWND_DESKTOP/*hParent*/, NULL, hInst, 0) ) != NULL ) // HWND_DESKTOP
	{
		// 프레임 위에 있는 html 페이지
		// Create a child window to host the browser object. 
		// NOTE: We embed the browser object duing our WM_CREATE handling for this child window.
		//if ( ( m_hDlgChild = CreateWindowEx( 0, "GomZi", NULL, WS_CHILD|WS_VISIBLE, 0, 0, 734, 620, m_hDlg, NULL, hInst, 0 ) ) ) 
		if ( ( m_hDlgChild = CreateWindowEx( 0, "GomZi", NULL, WS_CHILD|WS_VISIBLE, 0, 0, 764, 650, m_hDlg, NULL, hInst, 0 ) ) ) 
		{
			// Show the Main window.
			//UpdateWindow(m_hDlg);
		}
		else
		{
			DestroyWindow(m_hDlg);
		}
	}
		

	return;
}

LRESULT CALLBACK CGomZiDlg::StaticBrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pGomZiDlg->BrowserWindowProc( hDlg, iMsg, wParam, lParam );
}


LRESULT CALLBACK CGomZiDlg::BrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch (iMsg)
	{
	case WM_CREATE:
		{
			EmbedBrowserObject( hDlg );
		}
		break;
	/*
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hDlg, LOWORD(lParam), HIWORD(lParam));
		}
		break;
	*/
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hDlg);
			m_hDlgChild = NULL;
		}
		break;
	}

	return (DefWindowProc(hDlg, iMsg, wParam, lParam));
}


LRESULT CALLBACK CGomZiDlg::StaticGomZiDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pGomZiDlg->GomZiDlgProc( hDlg, iMsg, wParam, lParam );
	//return FALSE;
}

LRESULT CALLBACK CGomZiDlg::GomZiDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg )
	{	
	/*
	case WM_SIZE:
		{
			if(m_hDlgChild)
				MoveWindow(m_hDlgChild, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		}
		break;
	*/
		
	// WM_CLOSE 에서 return TRUE; 이것을 하면 창을 닫더라도 WM_DESTROY 호출되지 않아서
	// 꼼지 창을 계속 생성하지 않아도 된다.
	case WM_CLOSE:
		{
			Hide();
		}
		return TRUE;
		
		
	case WM_DESTROY:
		{
			/*
			if ( m_hDlgChild )
			{
				DestroyWindow( m_hDlgChild );
			}
			*/
			m_hDlg = NULL;
		}
		break;
	}

	return(DefWindowProc(hDlg, iMsg, wParam, lParam));
}

bool CGomZiDlg::IsShow()
{
	return m_bShow;
}

void CGomZiDlg::Show()
{	
	if ( m_hDlg == NULL )
	{
		return;
	}

	if ( g_bFullScreen )
	{
		ShowWindow( pclClient->GetHwnd(), SW_MINIMIZE );
	}

	// 화면 앞으로 보여줌.
	SetForegroundWindow( m_hDlg );

	if ( m_bShow )
	{
		return;
	}

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CGomZiDlg::Hide()
{
	if ( m_bShow )
	{
		m_bShow = false;
		ShowWindow( m_hDlg, SW_HIDE );
	}

	return;
}

void CGomZiDlg::Set( char *URL )
{
	if ( m_hDlgChild )
	{
		DisplayHTMLPage( m_hDlgChild, URL);
	}

	Show();	
	
	return;
}