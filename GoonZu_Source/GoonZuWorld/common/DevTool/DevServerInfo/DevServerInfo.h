//---------------------------------
// 2003/11/25 ±èÅÂ°ï
//---------------------------------

#ifndef _DEVSERVER_H
#define _DEVSERVER_H

#include <directives.h>

#include "../../NLib/NProfile.h"
#include "DevTool\DevTool.h"

class cltDevServerInfo : public cltDevTool{
private:
	NProfileIterator*	pkProfileIterator; 

	HWND				m_hServerInfoListCtrl;
	SI32				m_siMaxCalls;	
	BOOL				m_bReset;

private:
	BOOL OnInitDialog(HWND hDlg);
	

public:
	cltDevServerInfo();
	~cltDevServerInfo();

	void OnServerInfoDialog(TCHAR* Command);
	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


};

#endif
