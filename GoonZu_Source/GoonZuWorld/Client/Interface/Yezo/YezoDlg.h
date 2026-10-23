// YezoDlg.h: interface for the CYezoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YEZODLG_H__AFA50BF3_6DDB_4091_BEC5_0A8C1DED4B32__INCLUDED_)
#define AFX_YEZODLG_H__AFA50BF3_6DDB_4091_BEC5_0A8C1DED4B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

#define MAX_YEZO_TAB		3

class CYezoDlg  
{
public:
	CYezoDlg();
	virtual ~CYezoDlg();

	static BOOL CALLBACK	StatiCYezoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			YezoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCYezoJapanwarProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			YezoJapanwarProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCYezoAngelProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			YezoAngelProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCYezoAngelApplyerProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			YezoAngelApplyerProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCYezoAngelListProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			YezoAngelistProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool SetListDlg(SI32 index);
	void ShowListDlg(YezoAngelList yezoangellist []);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI32 sword,SI32 gun,SI32 command,SI32 porter,SI32 villageunique);

private:
	void SetVillageCombo();

	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hTabDlg[MAX_YEZO_TAB];
	HWND m_hGroup;

	char m_szYezoName[MAX_PLAYER_NAME];
	char m_szYezoProfile[256];
	char m_szYezoID[MAX_PLAYER_NAME];

	SI32 m_siSword;
	SI32 m_siGun;
	SI32 m_siCommand;
	SI32 m_siPorter;

	SI32 m_siViewDlg ;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;

	HBITMAP m_hHitButton;

	YezoAngelList	m_stYezoAngelList[MAX_YEZO_ANGELLIST];

	HWND m_hListDlg;
	HWND m_hAngelList;

	SI32 m_siSelectListInx;
	SI32 m_siListIndex[MAX_YEZO_ANGELLIST];

	SI32 m_siSelectList;

	BOOL m_bRectListSwitch;
	RECT m_rtListDlg;

};

#endif // !defined(AFX_YEZODLG_H__AFA50BF3_6DDB_4091_BEC5_0A8C1DED4B32__INCLUDED_)
