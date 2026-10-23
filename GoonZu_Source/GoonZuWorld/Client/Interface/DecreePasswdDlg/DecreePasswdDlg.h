// DecreePasswdDlg.h: interface for the CDecreePasswdDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECREEPASSWDDLG_H__617A89F4_C9B7_44FA_A62C_73C35235F87A__INCLUDED_)
#define AFX_DECREEPASSWDDLG_H__617A89F4_C9B7_44FA_A62C_73C35235F87A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../../Resource.h"

class CDecreePasswdDlg  
{
public:
	CDecreePasswdDlg();
	virtual ~CDecreePasswdDlg();

	static BOOL CALLBACK	StatiCDecreePasswdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			DecreePasswdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent,SI32 xsize,SI32 ysize );
	BOOL Set(char * accountID , char * passwd);

	bool CorrectPasswdRule(char * accountID , char * passwd);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	HWND m_hDlg;

	char	m_szAccountID[64];

	SI32	m_siXsize;
	SI32	m_siYsize;

};

#endif // !defined(AFX_DECREEPASSWDDLG_H__617A89F4_C9B7_44FA_A62C_73C35235F87A__INCLUDED_)
