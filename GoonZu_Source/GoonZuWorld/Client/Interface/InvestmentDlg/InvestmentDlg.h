// InvestmentDlg.h: interface for the CInvestmentDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVESTMENTDLG_H__17317A62_0FD3_4F5F_80D4_21253D53A122__INCLUDED_)
#define AFX_INVESTMENTDLG_H__17317A62_0FD3_4F5F_80D4_21253D53A122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../../Resource.h"

#define INVESTMENT_GETINFO_MONEY	10000 // Á¶È¸ºñ

class CInvestmentDlg  
{
public:
	CInvestmentDlg();
	virtual ~CInvestmentDlg();

	static BOOL CALLBACK	StatiCInvestmentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			InvestmentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set(SI32 npcunique);
	void Show();

	void ShowListData(InvestmentLines* investmentlines);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;

	HWND m_hInvestmentList;

	HBITMAP m_hPortraitBitmap;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	SI32 m_siNPCCharUnique;

	SI16 m_siItemUnique[MAX_INVESTMENT_ITEM_NUMBER];

};

#endif // !defined(AFX_INVESTMENTDLG_H__17317A62_0FD3_4F5F_80D4_21253D53A122__INCLUDED_)
