//---------------------------------
// 2003/7/17 ±èÅÂ°ï
//---------------------------------

#ifndef _DEVCHARINFO_H
#define _DEVCHARINFO_H

#include <windows.h>
#include <directives.h>

class cltDevCharInfo{
public:
	HWND m_hDlg;	
	HWND TabDlg;

	BOOL DlgRectInitSwitch;
	RECT DlgRect;

	SI32 CharUnique;

	cltCharManager* pclCM;

private:
	void ShowCharInfo(HWND hDlg);

public:
	cltDevCharInfo(cltCharManager* pclCM);
	~cltDevCharInfo();

	BOOL CALLBACK DevCharInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif
