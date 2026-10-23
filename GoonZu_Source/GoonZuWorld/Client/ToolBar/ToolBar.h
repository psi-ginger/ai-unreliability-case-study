//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _TOOLBAR_H
#define _TOOLBAR_H

#include <directives.h>

#include <Commctrl.h>	// Common Control 사용법	- Commctrl.h를 include 한다. 
						//							- WM_CREATE에 InitCommonCtrolsEx()를 호출한다 
						//							- ComCtl32.lib를 링크해준다. 				

#define MAX_TOOLBAR_BUTTON_NUMBER 40

class cltToolBarInfo{
public:
	SI32 iBimap;
	SI32 idCommand;
	BYTE fsStyle;
	TCHAR szTip[128];
};

class cltToolBar{
public:
	HWND hToolBar;				// 툴바의 핸들 
	HWND hParent;				// 툴바 부모의 핸들 	
	HINSTANCE ParentInstance;	// 부모의 인스턴스 

	TBBUTTON ToolBtn[MAX_TOOLBAR_BUTTON_NUMBER];

	cltToolBarInfo* pToolBarInfo;
	SI32 ToolButtonNumber;


	HBITMAP hBmpFindShopItem;
#define MAX_INTERNETMENU_ITEM	10
	HBITMAP hBmpInternetHome[MAX_INTERNETMENU_ITEM];


	cltToolBar(HWND hParent, HINSTANCE parentinstance, SI32 mode);
	~cltToolBar();

	SI32 GetToolBarXsize()
	{
		RECT rect;
		GetClientRect(hToolBar, &rect);
		return rect.right;
	}

	SI32 GetToolBarYsize()
	{
		/*
		RECT rect;
		GetClientRect(hToolBar, &rect);
		return rect.bottom;
		*/
		return 26;
	}
	TCHAR* GetToolTip(SI32 idcommand);
	
};


#endif


