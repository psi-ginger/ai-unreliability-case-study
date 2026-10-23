// FanPriestDlg.h: interface for the CFanPriestDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FANPRIESTDLG_H__4940513D_D44D_4CD1_819C_DE615AA26E3F__INCLUDED_)
#define AFX_FANPRIESTDLG_H__4940513D_D44D_4CD1_819C_DE615AA26E3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CFanPriestDlg  
{
public:
	CFanPriestDlg();
	virtual ~CFanPriestDlg();

	static BOOL CALLBACK	StatiCFanPriestDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			FanPriestDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();

private:
	void ShowVillageList();

	HINSTANCE m_hInst;
	HWND m_hWnd;

	HWND m_hDlg;
	HWND m_hEdit;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	SI32 m_iSelectVillageInx;
	SI32 m_iOldVillageInx;

	HBITMAP m_hBitmap;
};

#endif // !defined(AFX_FANPRIESTDLG_H__4940513D_D44D_4CD1_819C_DE615AA26E3F__INCLUDED_)
