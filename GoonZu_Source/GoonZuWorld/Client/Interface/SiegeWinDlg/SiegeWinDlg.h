// SiegeWinDlg.h: interface for the CSiegeWinDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#if !defined(AFX_SIEGEWINDLG_H__BD0FE30A_B3FB_4A19_8B04_BB68BEB373C2__INCLUDED_)
#define AFX_SIEGEWINDLG_H__BD0FE30A_B3FB_4A19_8B04_BB68BEB373C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CSiegeWinDlg  
{
public:
	CSiegeWinDlg();
	virtual ~CSiegeWinDlg();

	static BOOL CALLBACK	StatiCSiegeWinProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			SiegeWinProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set(TCHAR* ptext);

private:
	void Show();
	void Hide();

	HWND m_hSiegeWinDlg;
	HWND m_hSiegeWinStatic;
};

#endif // !defined(AFX_SIEGEWINDLG_H__BD0FE30A_B3FB_4A19_8B04_BB68BEB373C2__INCLUDED_)
*/