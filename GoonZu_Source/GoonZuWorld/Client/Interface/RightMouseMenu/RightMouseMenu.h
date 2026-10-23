#ifndef _RIGHTMOUSEMENU_H_
#define _RIGHTMOUSEMENU_H_

#include <windows.h>
#include <Directives.h>
#include "..\..\..\Resource.h"
#include "../../Common/CommonHeader.h"

#define MENU_BUTTON_WIDTH	44
#define MENU_BUTTON_HEIGHT	9

#define MENU_BUTTON_NUM		6

class CRightMouseMenu
{
public:
	CRightMouseMenu();
	~CRightMouseMenu();

	static	BOOL CALLBACK StaticRightMouseMenuDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK RightMouseMenuDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void Create( HINSTANCE hInst, HWND hWnd );

	bool IsShow();
	void Show( SI32 siUnderMouseID, SI32 siCharUnique, SI32 siMouseXPos, SI32 siMouseYPos ,bool bPersonalShop = false);
	void Hide();

private:
	HWND m_hDlg;
	bool m_bShow;

	SI32	m_siUnderMouseID;
	SI32	m_siCharUnique;

	bool m_bPersonalShopSwitch ;

	HBITMAP m_hRightMouseMenu0;
	HBITMAP m_hRightMouseMenu1;

	HBITMAP m_hRightMouseMenu;
};

extern CRightMouseMenu g_RightMouseMenu;

#endif