// JangGilSanDlg.h: interface for the CJangGilSanDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JANGGILSANDLG_H__2819E378_5D1E_4736_BFA9_F8E505529EBF__INCLUDED_)
#define AFX_JANGGILSANDLG_H__2819E378_5D1E_4736_BFA9_F8E505529EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CJangGilSanDlg  
{
public:
	CJangGilSanDlg();
	virtual ~CJangGilSanDlg();

	void Create( HINSTANCE hInst, HWND hParent );

	static BOOL CALLBACK	StatiCJangGilSanSBSProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			JangGilSanSBSProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void ShowSBS(bool bWaitingMan);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	HBITMAP m_hPortraitBitmapSBS;

	HWND	m_hSBS;

	RECT	m_rtSBS;

	BOOL	m_bSBS;

};

#endif // !defined(AFX_JANGGILSANDLG_H__2819E378_5D1E_4736_BFA9_F8E505529EBF__INCLUDED_)
