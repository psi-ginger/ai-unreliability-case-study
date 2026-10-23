//---------------------------------
// 2003/11/25 ±èÅÂ°ï
//---------------------------------

#ifndef _DEVNPCINFO_H
#define _DEVNPCINFO_H

#include <directives.h>

#include "DevTool\DevTool.h"

class cltDevNPCInfo : public cltDevTool{
public:

private:
	void ShowInfo(HWND hDlg);

public:
	cltDevNPCInfo(cltCharManager* pclCM);
	~cltDevNPCInfo();

	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif
