#ifndef _MAGICMGRDLG_H_
#define _MAGICMGRDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#define MAGICMGRDLG_SETTIMER_TIME		1000		// 1초 당 한번씩 WM_TIMER 호출

#include "..\..\..\CommonLogic\MagicMgr\MagicBaseInfo.h"

class CMagicMgrDlg
{
public:
	CMagicMgrDlg();
	~CMagicMgrDlg();

	void Create( HINSTANCE hInst, HWND hWnd , SI32 x , SI32 y );

	static	BOOL CALLBACK StaticMagicMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK MagicMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	static	BOOL CALLBACK StaticMagicTabDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK MagicTabDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void ShowTabDialog(int index);
	BOOL SetNewMagic(int index);
	bool IsShow();
	void Show();
	void Hide();
	void DrawMagicIcon(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y );

	bool IsChanged();

	void Clear();
	void AddMagicList( char *strMagicObj , SI32 index);

	void SetExplain();

	SI16 GetMagicKind();
	SI16 GetMagicKind( SI32 key);

private:
	HWND m_hDlg;
	bool m_bShow;
	HWND m_hTabCtrl;
	HWND m_hButton;

	HWND m_hMagicList;
	HWND m_hMagicExplainEdit;
	HWND m_hMagicExplainList;

	SI16 m_siOriMagicSkillLevel;		// 마법 레벨이 변경 될때마다 리스트 박스 업데이트 해줌


	HWND m_hMagicTabDlg;
	SI16 m_CurrentTabIndex;				// 현재선택되어져 있는 TabControl의 Index

	DWORD m_dwLastChangedMagic; 
	// 그림
	HBITMAP m_hBitMap ;
	HBRUSH  m_hEditBrush;

	char m_szBitmapStr[32];
	char m_szOldBitmapStr[32];

	void SetMagicBitmap(HWND hDlg);

	// 마법 아이콘
	cltTSpr m_MagicIcon;

	SI32 m_siDrawX;
	SI32 m_siDrawY;
};

extern CMagicMgrDlg g_MagicMgrDlg;



#endif