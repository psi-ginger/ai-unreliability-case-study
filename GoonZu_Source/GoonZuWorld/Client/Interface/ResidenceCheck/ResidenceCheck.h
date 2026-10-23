// ResidenceCheck.h: interface for the CResidenceCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIDENCECHECK_H__52B43CC1_624F_48FC_8D00_0C74127BFCC6__INCLUDED_)
#define AFX_RESIDENCECHECK_H__52B43CC1_624F_48FC_8D00_0C74127BFCC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CResidenceCheck  
{
public:
	CResidenceCheck();
	virtual ~CResidenceCheck();

	void Create( HINSTANCE hInst, HWND hParent );
	void set(HWND hDlg,TCHAR * szName,TCHAR * szLevel,SI32 currentchar);
	void Show();
	void Hide();
	BOOL GetCheck();
	inline void SetCheck(){m_bCheck = FALSE;};
	inline TCHAR* GetResidenceNum(){return m_szResidenceNum;};

	static BOOL CALLBACK	StatiCResidenceCheckProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			ResidenceCheckProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK EditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
private:
	void Init();

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	HWND	m_hResidenceCheck;
	HWND	m_hResidenceEdit;

	WNDPROC	m_OldEditProc;

	BOOL	m_bCheck;

	TCHAR	m_szResidenceNum[12];

	SI32	m_siCurrentCharInfoList;

};

#endif // !defined(AFX_RESIDENCECHECK_H__52B43CC1_624F_48FC_8D00_0C74127BFCC6__INCLUDED_)