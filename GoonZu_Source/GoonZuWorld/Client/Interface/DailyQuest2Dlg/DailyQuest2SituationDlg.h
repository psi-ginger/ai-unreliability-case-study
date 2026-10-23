#ifndef _DAILYQUESTSITUAIONDLG_H_
#define _DAILYQUESTSITUAIONDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

class CDailyQuestSituationDlg
{
public:
	HWND m_hDlg;

	CDailyQuestSituationDlg();
	~CDailyQuestSituationDlg();

	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticDailyQuestSituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK DailyQuestSituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();
	
	void LoadSituation();

private:
	bool m_bShow;

	bool m_bLoadSituationComplete;
};

#endif