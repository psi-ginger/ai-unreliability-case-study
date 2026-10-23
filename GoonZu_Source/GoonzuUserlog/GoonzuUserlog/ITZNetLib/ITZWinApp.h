#ifndef _ITZWINAPP_H
#define _ITZWINAPP_H

#include "itzbase.h"

class ITZWinApp 
{
public:
	ITZWinApp();
	~ITZWinApp();
 
public:
	virtual LRESULT		WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	
	virtual BOOL		Init( HINSTANCE hInstance, LPCSTR szTitle, LPCSTR szWindowClass, LPCSTR szMenuName, HICON hIcon, HICON hIconSm );
	virtual void		ProcessMessageLoop();

	virtual void		Run() {};

	virtual void		OnCreate() {};
	virtual void		OnDestroy() {};
	virtual	void		OnCommand( int wmId, int wmEvent ) {};
	virtual void		OnPaint( HDC hdc ) {};
	
	HINSTANCE			m_hInst;
	HWND				m_hWnd;

};

#endif