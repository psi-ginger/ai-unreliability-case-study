#include <windows.h>
#include <exdisp.h>		/* Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above */
#include <exdispid.h>		/* Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above */
#include <mshtml.h>		/* Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above */
#include <mshtmhst.h>	// Defines of stuff like IDocHostUIHandler. This is an include file with Visual C 6 and above
#include <crtdbg.h>		/* for _ASSERT() */
#include <tchar.h>
#include "webhtml.h"


#include "../Client/Client.h"
extern cltCommonLogic* pclClient;

/****************************** WindowProc() ***************************
 * Our message handler for our window to host the browser.
 */
//int WindowCount = 0;
static bool bCreated = false;

LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch( uMsg )
	{
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Another window created with an embedded browser object
			//++WindowCount;
			
			bCreated = true;
			
			// Success
		}
		return FALSE;	

	case WM_PARENTNOTIFY:
		{
			if ( LOWORD( wParam ) == WM_DESTROY )				
			{
				SendMessage( hwnd, WM_CLOSE, 0, 0 );
			}
		}
		return FALSE;

	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			//UnEmbedBrowserObject(hwnd);
			SetWindowLong(hwnd, GWL_USERDATA, 0);
			
			// One less window
			//--WindowCount;
			
			// If all the windows are now closed, quit this app
			//if (!WindowCount) PostQuitMessage(0);
			
			bCreated = false;
		}
		return FALSE;
	}

	// NOTE: If you want to resize the area that the browser object occupies when you
	// resize the window, then handle WM_SIZE and use the IWebBrowser2's put_Width()
	// and put_Height() to give it the new dimensions.

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

WNDCLASSEX wc;

HWND CreateBrowserWindow()
{
	if ( bCreated )
	{
		return NULL;
	}
	
	SI32 XSize, YSize;

	cltClient *pclclient = ( cltClient * )pclClient;
	if ( pclclient->szID[ 1 ] == '_' && ( pclclient->szID[ 0 ] == 's' || pclclient->szID[ 0 ] == 'S'  ) )
	{
		// SBS
		XSize = 548;
		YSize = 515;		// 세로 크기 좀더 키움.
	}
	else
	{
		// 군주
		XSize = 481;
		YSize = 305;
	}


	static int Counter = 0;

	HWND _hWnd = NULL;
	HRESULT hr = E_FAIL;
	
	// Ensure that the browser window is registered.
	
	if ( Counter == 0 )
	{
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		
		wc.cbSize		= sizeof(WNDCLASSEX);
		wc.hInstance	= pclClient->GetInstance();
		wc.lpfnWndProc	= WindowProc1;
		wc.style		= CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;//CS_HREDRAW|CS_VREDRAW; 
		wc.lpszClassName= TEXT("Browser_Main");
		wc.hIcon		= LoadIcon( pclClient->GetInstance(), MAKEINTRESOURCE(IDI_ICON_GOONZU));
		wc.hCursor		= LoadCursor( pclClient->GetInstance(), IDC_ARROW );
		wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
		
		
		if(RegisterClassEx(&wc)==NULL)
		{
			goto Exit;
		}
	}

	++Counter;
		
	// Retrieve the system's work area in the RECT structure.
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	// Create the top-level browser window.
	
	
	TCHAR* pText = GetTxtFromMgr(5053);
	_hWnd = CreateWindowEx(0,
		TEXT("Browser_Main"),
		pText,
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		XSize,//rcWorkArea.right - rcWorkArea.left,
		YSize,//rcWorkArea.bottom - rcWorkArea.top,
		HWND_DESKTOP, NULL, pclClient->GetInstance(), 0);
	if (!_hWnd)
		goto Exit;

	hr = S_OK;
Exit:
    return _hWnd;
}