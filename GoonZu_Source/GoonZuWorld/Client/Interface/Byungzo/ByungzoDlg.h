// ByungzoDlg.h: interface for the CByungzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYUNGZODLG_H__3ECA0E79_F6F3_4FC2_9DC2_4FB04793DAC1__INCLUDED_)
#define AFX_BYUNGZODLG_H__3ECA0E79_F6F3_4FC2_9DC2_4FB04793DAC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CByungzoDlg  
{
public:
	CByungzoDlg();
	virtual ~CByungzoDlg();

	static BOOL CALLBACK	StatiCByungzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			ByungzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI32 time );

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hGroup;

	SI32 m_siWarTime;

	char m_szByungzoName[MAX_PLAYER_NAME];
	char m_szByungzoProfile[256];
	char m_szByungzoID[MAX_PLAYER_NAME];

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;
};

#endif // !defined(AFX_BYUNGZODLG_H__3ECA0E79_F6F3_4FC2_9DC2_4FB04793DAC1__INCLUDED_)
