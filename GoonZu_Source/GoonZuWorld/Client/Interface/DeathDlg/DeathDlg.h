/*
#ifndef _DEATHDLG_H_
#define _DEATHDLG_H_

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CDeathDlg
{
public:
	CDeathDlg();
	~CDeathDlg();

	void					Create( HINSTANCE hInst, HWND hParent );
	void					set(SI32 money , SI32 exp, SI16 killmode, char* pname );

	static BOOL CALLBACK	StatiCDeathDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			DeathDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show();
	void					Hide();

	inline	void			SetSex(SI16 sex){m_siSex = sex;};

private:
	HWND			m_hDeathDlg;
	HWND			m_hDeathEdit;
	HBITMAP			m_hHeroBitmap[4];
	HBITMAP			m_hHeroineBitmap[4];
	SI16			m_siBitmapAnimationIndex;
	SI16			m_siSex;
};

extern	CDeathDlg		g_DeathDlg;

#endif
*/