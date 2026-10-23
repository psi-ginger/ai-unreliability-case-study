#ifndef _DEVINHEREMAPCHAR_H
#define _DEVINHEREMAPCHAR_H

#include <directives.h>

#include "DevTool\DevTool.h"

class cltDevInHereMapChar : public cltDevTool
{
protected:
	cltCharManager*		m_pclCM;

	HWND				m_hInHereMapListCtrl;


private:
	BOOL OnInitDialog(HWND hDlg);
	BOOL OnButtonRefresh(HWND hDlg);
	void ShowInfo(HWND hDlg);

public:
	cltDevInHereMapChar(cltCharManager* pclCM);
	~cltDevInHereMapChar();

	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

};

#endif
