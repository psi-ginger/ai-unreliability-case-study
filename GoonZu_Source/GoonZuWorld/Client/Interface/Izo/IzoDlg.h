// IzoDlg.h: interface for the CIzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IZODLG_H__86DA7CBA_455B_4E22_BF28_59B77B53FCE1__INCLUDED_)
#define AFX_IZODLG_H__86DA7CBA_455B_4E22_BF28_59B77B53FCE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

#define MAX_IZO_TAB		2

class CIzoDlg  
{
public:
	CIzoDlg();
	virtual ~CIzoDlg();

	static BOOL CALLBACK	StatiCIzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			IzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCIzoPriceValueProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			IzoPriceValueProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCIzoShipValueProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			IzoShipValueProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI32 pricetime,SI32 priceinx,SI32 pricevalue,
			  SI64 money,SI32 calctime,SI32 shipvalue,SI32 tradetax,SI32 tradelimitmoney);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hGroup;
	HWND m_hTabDlg[MAX_IZO_TAB];

	char m_szIzoName[MAX_PLAYER_NAME];
	char m_szIzoProfile[256];
	char m_szIzoID[MAX_PLAYER_NAME];

	SI32 m_siPriceValue;
	SI32 m_siPriceTime;
	SI32 m_siTinTin;

	SI64 m_siTotalMoney;
	SI32 m_siCalcTime;

	SI32 m_siShipValue;
	SI32 m_siTradeTax;
	SI32 m_siTradeLimitMoney;

	SI32 m_siViewDlg;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;

};

#endif // !defined(AFX_IZODLG_H__86DA7CBA_455B_4E22_BF28_59B77B53FCE1__INCLUDED_)
