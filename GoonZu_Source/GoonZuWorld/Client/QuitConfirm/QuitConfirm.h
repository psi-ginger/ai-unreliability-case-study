//---------------------------------
// 2003/11/11 김태곤
//---------------------------------
#ifndef _QUITCONFIRM_H
#define _QUITCONFIRM_H

#include <WIndows.h>
#include <Directives.h>

// 종료 확인자. 
class cltQuitMenu{
public:
	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	BOOL DlgRectInitSwitch;
	
	cltQuitMenu();
	~cltQuitMenu();
	
	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK StaticQuitMenuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK QuitMenuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
};

// 종료 확인자. 
class cltQuitConfirm{
public:
	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	BOOL DlgRectInitSwitch;

	cltQuitConfirm();
	~cltQuitConfirm();
	
	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK StaticQuitConfirmDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK QuitConfirmDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
};


#endif
