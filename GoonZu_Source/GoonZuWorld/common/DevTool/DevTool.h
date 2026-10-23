//---------------------------------
// 2003/111/25 김태곤
//---------------------------------

#ifndef _DEVTOOL_H
#define _DEVTOOL_H

#include <directives.h>

class cltDevTool{
public:
	HWND m_hDlg;	

	bool DlgRectInitSwitch;
	RECT DlgRect;


private:

public:
	cltDevTool();
	virtual ~cltDevTool();

	virtual SI32 GetCharUnique(){return 0;}

	virtual BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;

	// 대화상자가 없으면 만들고 있으면 없앤다. 
	bool Create(HINSTANCE hinst, HWND hwnd, SI32 dlgitem, DLGPROC dlgproc, bool bShow = true);

	// WM_DESTROY에서의 조치.
	void ACTION_WM_DESTROY();


};

#endif
