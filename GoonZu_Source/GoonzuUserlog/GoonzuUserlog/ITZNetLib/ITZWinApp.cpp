#include "ITZWinApp.h"

ITZWinApp *g_pWinApp = NULL;

LRESULT CALLBACK ITZWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;

	PAINTSTRUCT ps;
	HDC hdc;

	switch( message ) 
	{
		case WM_CREATE:

			g_pWinApp->OnCreate();

			return 0;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			g_pWinApp->OnCommand( wmId, wmEvent );

			return 0;

		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			
			g_pWinApp->OnPaint( hdc );

			EndPaint(hWnd, &ps);

			return 0;

		case WM_DESTROY:

			g_pWinApp->OnDestroy();

			PostQuitMessage(0);
			return 0;
	}

	return g_pWinApp->WndProc( hWnd, message, wParam, lParam );
}


ITZWinApp::ITZWinApp()
{
	g_pWinApp = this;
}

ITZWinApp::~ITZWinApp()
{

}

BOOL ITZWinApp::Init( HINSTANCE hInstance, LPCSTR szTitle, LPCSTR szWindowClass, 
					 LPCSTR szMenuName, HICON hIcon, HICON hIconSm )
{

	m_hInst = hInstance;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)ITZWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= szMenuName;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= hIconSm;

	RegisterClassEx(&wcex);

	m_hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL );

	if(!m_hWnd) return FALSE;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return TRUE;

}

void ITZWinApp::ProcessMessageLoop()
{

	MSG msg;

	// Main message loop
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


LRESULT ITZWinApp::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

