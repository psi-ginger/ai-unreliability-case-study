//---------------------------------
// 2003/7/17 ±èÅÂ°ï
//---------------------------------

#ifndef _DEVCHARINFO_H
#define _DEVCHARINFO_H

#include <directives.h>

#include "DevTool\DevTool.h"

class cltDevCharInfo : public cltDevTool{
public:

	SI32 CharUnique;

	cltCharManager* pclCM;

private:
	void ShowCharInfo(HWND hDlg);

public:
	cltDevCharInfo(cltCharManager* pclCM);
	~cltDevCharInfo();

	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetCharUnique(){return CharUnique;}

};

#endif
