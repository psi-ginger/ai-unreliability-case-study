// HozoDlg.h: interface for the CHozoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOZODLG_H__7104AF91_885F_482B_954A_6ACE7F323339__INCLUDED_)
#define AFX_HOZODLG_H__7104AF91_885F_482B_954A_6ACE7F323339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

#define MAX_HOZO_TAB		5

class CHozoDlg  
{
public:
	CHozoDlg();
	virtual ~CHozoDlg();

	static BOOL CALLBACK	StatiCHozoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHozoResidentProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoResidentProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHozoPrizeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoPrizeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHozoFixationProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoFixationProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHozoChangedNameListProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoChangedNameListProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHozoNewCharRateProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HozoNewCharRateProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI32 residentnum,SI32 maxshop,SI32 totalcharnum,SI32 calctime,
			  SI32 prizemoney,SI32 prizesettime,SI32 fixationmoney,SI32 newcharrate);

	void ShowChangedNameList(SI32 page , ChangedNameChar changednamechar []);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hTabDlg[MAX_HOZO_TAB];
	HWND m_hGroup;

	char m_szHozoName[MAX_PLAYER_NAME];
	char m_szHozoProfile[256];
	char m_szHozoID[MAX_PLAYER_NAME];

	SI32 m_siPrizeMoney;
	SI32 m_siPrizeSetTime;
	SI32 m_siFixationMoney;

	SI32 m_siViewDlg ;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;

	HBITMAP m_hHitButton;

	HWND m_hChangedNameList;
	SI32 m_siListPage;

};

#endif // !defined(AFX_HOZODLG_H__7104AF91_885F_482B_954A_6ACE7F323339__INCLUDED_)
