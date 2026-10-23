// VillageWarInfoDlg.h: interface for the CVillageWarInfoDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#if !defined(AFX_VILLAGEWARINFODLG_H__037A6596_8567_4FB7_BF53_ECB9D418D87E__INCLUDED_)
#define AFX_VILLAGEWARINFODLG_H__037A6596_8567_4FB7_BF53_ECB9D418D87E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CVillageWarInfoDlg  
{
public:
	CVillageWarInfoDlg();
	virtual ~CVillageWarInfoDlg();

	static BOOL CALLBACK	StatiCVillageWarInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			VillageWarInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set(SI32 winvillage,SI32 losevillage);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;

	HBRUSH m_hEditBrush;

};

#endif // !defined(AFX_VILLAGEWARINFODLG_H__037A6596_8567_4FB7_BF53_ECB9D418D87E__INCLUDED_)
*/