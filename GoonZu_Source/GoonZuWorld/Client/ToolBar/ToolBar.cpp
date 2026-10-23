//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "ToolBar.h"
#include "..\..\Resource.h"

#include "..\Interface\ToolBarBaseInfoDlg\ToolBarBaseInfoDlg.h"

#include "../Client/Client.h"

extern cltCommonLogic* pclClient;


// 클라이언트용 툴바. 
static cltToolBarInfo clToolBarGameInfo[] ={
	{0, ID_TOOLBAR_SMALLMAP,	TBSTYLE_BUTTON,   TEXT("전체지도\r\nALT+W")}, 
	{0, ID_TOOLBAR_SEPERATOR,	TBSTYLE_SEP,      TEXT("")},
	{2, ID_TOOLBAR_CHARSTATUS,	TBSTYLE_BUTTON,   TEXT("상태정보\r\nALT+A")},
	{3, ID_TOOLBAR_INVENTORY,	TBSTYLE_BUTTON,   TEXT("소지품\r\nALT+I")},
	{4, ID_TOOLBAR_SKILL,		TBSTYLE_BUTTON,   TEXT("기술정보\r\nALT+S")},
	{5, ID_TOOLBAR_PERSONALSHOP,TBSTYLE_DROPDOWN, TEXT("개인상점")},
	{0, ID_TOOLBAR_SEPERATOR,	TBSTYLE_SEP,      TEXT("")},

	{6, ID_TOOLBAR_ITEMINFO,	TBSTYLE_BUTTON, TEXT("물품상세정보")},
	{7, ID_TOOLBAR_MONSTERINFO,	TBSTYLE_BUTTON, TEXT("몬스터상세정보")},
	{8, ID_TOOLBAR_HUNTINGINFO,	TBSTYLE_BUTTON, TEXT("사냥터상세정보")},
	{9, ID_TOOLBAR_NPCINFO,		TBSTYLE_BUTTON, TEXT("주민(NPC)상세정보")},	
	{10, ID_TOOLBAR_VILLAGEINFO,TBSTYLE_BUTTON, TEXT("마을상세정보")},	
	{0, ID_TOOLBAR_SEPERATOR,	TBSTYLE_SEP,    TEXT("")},

	{11, ID_TOOLBAR_VILLAGERESIDENTINFO,TBSTYLE_BUTTON, TEXT("마을주민정보\r\nALT+V")},	
	{12, ID_TOOLBAR_MASTERINFO,	TBSTYLE_BUTTON, TEXT("장인정보\r\nALT+C")},	
	{13, ID_TOOLBAR_MAGICINFO,	TBSTYLE_BUTTON, TEXT("마법정보\r\nALT+M")},	
//	{14, ID_TOOLBAR_SCHOOLINFO,	TBSTYLE_BUTTON, TEXT("학교정보")},	
	{15, ID_TOOLBAR_FRIEND,		TBSTYLE_BUTTON, TEXT("친구등록\r\nALT+F")},
	{16, ID_TOOLBAR_FATHER,		TBSTYLE_BUTTON, TEXT("스승님\r\nALT+T")},
	{0,  ID_TOOLBAR_SEPERATOR,	TBSTYLE_SEP,    TEXT("")},

	
	{17, ID_TOOLBAR_HOMEPAGE,	TBSTYLE_DROPDOWN, TEXT("군주 홈페이지 이동")},
	
//	{20, ID_TOOLBAR_ASK,		TBSTYLE_BUTTON, TEXT("질문목록보기")},  // 지식검색"
	{0,  ID_TOOLBAR_SEPERATOR,	TBSTYLE_SEP,    TEXT("")},
	
	{22,ID_TOOLBAR_CONFIG,		TBSTYLE_BUTTON, TEXT("환경설정\r\nALT+E")},
	{23,ID_TOOLBAR_SCREENSHOT,	TBSTYLE_BUTTON, TEXT("스크린샷\r\nALT+P")},
	
	{24,ID_TOOLBAR_ITEMMALL,	TBSTYLE_BUTTON, TEXT("아이템 몰")},
	{25,ID_TOOLBAR_HELP,		TBSTYLE_BUTTON, TEXT("도움말\r\nALT+H")},

};




// 맵 에디터용 툴바. 
static cltToolBarInfo clToolBarMapEditInfo[] ={
	{0, ID_TOOLBAR_NEWMAP,			TBSTYLE_BUTTON, TEXT("새지도")},
	{1, ID_TOOLBAR_LOAD,			TBSTYLE_BUTTON, TEXT("지도 불러오기")},
	{2, ID_TOOLBAR_SAVE,			TBSTYLE_BUTTON, TEXT("지도 저장")},
	{0, ID_TOOLBAR_SEPERATOR,		TBSTYLE_SEP, TEXT("")},


	{3, ID_TOOLBAR_TERRAINTOOL,		TBSTYLE_BUTTON,	TEXT("타일,오브젝트 툴")},
	{4, ID_TOOLBAR_HEADERMANAGER,	TBSTYLE_BUTTON,	TEXT("헤더편집기")},
	{5, ID_TOOLBAR_GEOINFO,			TBSTYLE_BUTTON, TEXT("지형안내툴")},
	{0, ID_TOOLBAR_SEPERATOR,		TBSTYLE_SEP, TEXT("")},

	{6, ID_TOOLBAR_SMALLMAP,		TBSTYLE_BUTTON,	TEXT("미니맵")}, 
	{7, ID_TOOLBAR_CAPTUREMAP,		TBSTYLE_BUTTON, TEXT("미니맵 저장")},
	{8, ID_TOOLBAR_VILLAGE,			TBSTYLE_CHECK,	TEXT("마을정보")},
	{0, ID_TOOLBAR_SEPERATOR,		TBSTYLE_SEP, TEXT("")},
	
	{9, ID_TOOLBAR_COLLDRAW,		TBSTYLE_BUTTON,	TEXT("충돌지역보여주기")},
	{10,ID_TOOLBAR_NEWCOLL,			TBSTYLE_BUTTON, TEXT("충돌지역재설정")},
	{11,ID_TOOLBAR_CUSTOMCOLL,		TBSTYLE_BUTTON, TEXT("임의충돌설정")},

	{12,ID_TOOLBAR_DELETEUNIQUE,	TBSTYLE_BUTTON, TEXT("유니크 모두 삭제")}
};

cltToolBar::cltToolBar(HWND hparent, HINSTANCE parentinstance, SI32 mode)
{
	SI32 i;
	SI32 BitmapImage = 0;
	SI32 BitmapImageNumber = 0;

	hToolBar = NULL;
	hBmpFindShopItem = NULL;
	memset(hBmpInternetHome, 0, sizeof(hBmpInternetHome));
	
	hParent			=	hparent;
	ParentInstance	=	parentinstance;

	if(mode == 0)
	{
		pToolBarInfo = clToolBarGameInfo;
		ToolButtonNumber	= sizeof(clToolBarGameInfo)/sizeof(cltToolBarInfo);

		BitmapImageNumber	= 26;

		BitmapImage			= IDB_TOOLBAR;
	}
	else
	{
		pToolBarInfo = clToolBarMapEditInfo;
		ToolButtonNumber	= sizeof(clToolBarMapEditInfo)/sizeof(cltToolBarInfo);;
		BitmapImageNumber	= 13;
		BitmapImage			= IDB_TOOLBAR_MAPEDIT;
	}

	if(ToolButtonNumber >= MAX_TOOLBAR_BUTTON_NUMBER)
	{
		MessageBox(NULL, TEXT("fd9ndf"), TEXT("vcvd111"), MB_OK|MB_TOPMOST);
	}
	
	for(i = 0; i < ToolButtonNumber ;i++)
	{
		ToolBtn[i].iBitmap		= pToolBarInfo[i].iBimap;
		ToolBtn[i].idCommand	= pToolBarInfo[i].idCommand;
		ToolBtn[i].fsState		= TBSTATE_ENABLED;
		ToolBtn[i].fsStyle		= pToolBarInfo[i].fsStyle;
		ToolBtn[i].dwData		= 0;
		ToolBtn[i].iString		= 0;
	}

	hToolBar  = CreateToolbarEx(hParent, WS_CHILD|WS_VISIBLE|WS_BORDER|TBSTYLE_TOOLTIPS,
		ID_TOOLBAR, 
		BitmapImageNumber,	// 툴 버튼 비트맵의 갯수. 
		ParentInstance,		// 부모의 인스턴스 
		BitmapImage,		// 비트맵의 핸들 
		ToolBtn,			// 툴버튼에 대한 정보를 가진 TBBUTTON구조체 배열 
		ToolButtonNumber,	// 툴버튼 갯수. 
		16, 16,				// 버튼의 가로 세로 크기. 		
		20/*GetToolBarXsize()*/,	// 비트맵의 크기. (X)
		20/*GetToolBarYsize()*/,	// 비트맵의 크기. (Y)
		sizeof(TBBUTTON));

	if(hToolBar == NULL)
	{
		TCHAR buffer[256];
		TCHAR text[128];
		DWORD errcode = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buffer,  128, NULL);
		StringCchPrintf(text, 128, TEXT("ErrorCode=%d Message=%s"), errcode, buffer);
		MessageBox(NULL, text, TEXT("CreateToolBarEx"), MB_OK|MB_TOPMOST);
	}
	else
	{
		//============================
		// 게임시간과 보유 자금을 표시하는 DialogBox 생성 
		//============================
		g_ToolBarBaseInfoDlg.Create( parentinstance, hToolBar );		
		SendMessage(hToolBar, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);

		//============================
		// TrackPopMenu Icons
		//============================
		hBmpFindShopItem    = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_FINDSHOPITEM));

		hBmpInternetHome[0] = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_MYGOMZI));
		hBmpInternetHome[1] = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_NEWS));
		hBmpInternetHome[2] = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_KNOWLEDGE));
		hBmpInternetHome[3] = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_SARANGBANG));
		hBmpInternetHome[4] = NULL;	// 포도청 아이콘 없음 
		hBmpInternetHome[5] = LoadBitmap(parentinstance, MAKEINTRESOURCE(IDB_BITMAP_VILLAGERESIDENT));
		hBmpInternetHome[6] = NULL; // 객주 아이콘 없음
	}
}


cltToolBar::~cltToolBar()
{
	if(hBmpFindShopItem) DeleteObject(hBmpFindShopItem);

	for(int i = 0; i < MAX_INTERNETMENU_ITEM; ++i)
		if(hBmpInternetHome[i]) DeleteObject(hBmpInternetHome[i]);
}


TCHAR* cltToolBar::GetToolTip(SI32 idcommand)
{
	SI32 i;

	for(i = 0; i< ToolButtonNumber;i++)
	{
		if(pToolBarInfo[i].idCommand == idcommand)
		{
			
			return pToolBarInfo[i].szTip;
		}
	}

	return NULL;
}
