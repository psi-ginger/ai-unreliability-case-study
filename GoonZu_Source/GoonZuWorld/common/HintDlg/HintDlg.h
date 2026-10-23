/*
#ifndef _HINTDLG_H
#define _HINTDLG_H

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../Resource.h"

#define MAX_HINTCOMBO_TYPE		20
#define MAX_HINTLIST_TYPE		20

class cltHintDlgExplain
{
public:
	cltHintDlgExplain();
	~cltHintDlgExplain();

	void AddComboType(TCHAR *szName,TCHAR *szFile);

	void Create(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK HintDlgExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK HintDlgExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetExplainType(TCHAR* pExplainType);
	void ShowExplainHint(HWND hDlg, SI32 siHintType);

	void Show(TCHAR * szFileName ) ;
	void Set(TCHAR * szFileName ) ;

	void ReadListFile();
	void ShowList(TCHAR * szFileName);

private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	HWND	m_hDlg;	

	RECT	m_DlgRect;

	HBITMAP m_hHitBitmap;

	BOOL	m_DlgRectInitSwitch;

	TCHAR	m_szBmpName[MAX_HINTCOMBO_TYPE][128] ;
	TCHAR	m_szComboType[MAX_HINTCOMBO_TYPE][256] ;

	TCHAR	m_szListType[MAX_HINTLIST_TYPE][128] ;
	TCHAR	m_szSubject[MAX_HINTLIST_TYPE][128] ;

};

#endif

*/