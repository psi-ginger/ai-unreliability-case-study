#if defined(_NDAILYQUEST3SELECTDLG) && defined(_STARTING_NOT_USING)

#pragma once

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>

class CNDailyQuest3SituationDlg
{
public:
	HWND m_hDlg;

	CNDailyQuest3SituationDlg();
	~CNDailyQuest3SituationDlg();

	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StatiCNDailyQuest3SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK DailyQuest3SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();
	
	void LoadSituation();

private:
	bool m_bShow;

	bool m_bLoadSituationComplete;
};

#endif