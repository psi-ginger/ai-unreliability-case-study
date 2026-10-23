// GoonzuDlg.h: interface for the CGoonzuDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GOONZUDLG_H__5D2AF71F_2A75_4283_8851_BA719F818E70__INCLUDED_)
#define AFX_GOONZUDLG_H__5D2AF71F_2A75_4283_8851_BA719F818E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CGoonzuDlg  
{
public:
	CGoonzuDlg();
	virtual ~CGoonzuDlg();

	static BOOL CALLBACK	StatiCGoonzuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			GoonzuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,INT64 finances);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hGroup;

	char m_szGoonzuName[MAX_PLAYER_NAME];
	char m_szGoonzuProfile[256];
	char m_szGoonzuID[MAX_PLAYER_NAME];

	SI32 m_siResidentNum;
	SI32 m_siSelectMinister;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;

	INT64	m_iTotalFinances;

};

#endif // !defined(AFX_GOONZUDLG_H__5D2AF71F_2A75_4283_8851_BA719F818E70__INCLUDED_)
