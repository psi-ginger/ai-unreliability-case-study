//---------------------------------
// 2003/11/25 ±èÅÂ°ï
//---------------------------------

#ifndef _DEVPROFILE_H
#define _DEVPROFILE_H

#include <directives.h>

#include "../../NLib/NProfile.h"
#include "DevTool\DevTool.h"

class cltDevProfileInfo : public cltDevTool{
private:
	NProfileIterator*	pkProfileIterator; 

	HWND				m_hProfileListCtrl;
	SI32				m_siMaxCalls;	
	BOOL				m_bReset;

private:
	BOOL OnInitDialog(HWND hDlg);
	BOOL OnTimerDialog(HWND hDlg);

public:
	cltDevProfileInfo();
	~cltDevProfileInfo();

	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


};

#endif
