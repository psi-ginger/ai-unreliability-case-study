//// HtmlDlg.h: interface for the CHtmlDlg class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_HTMLDLG_H__B3A4DDA2_F574_41D6_8967_B2D2771EB657__INCLUDED_)
//#define AFX_HTMLDLG_H__B3A4DDA2_F574_41D6_8967_B2D2771EB657__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include <Directives.h>
//#include <windows.h>
//#include <stdio.h>
//
//#include "../../../Resource.h"
//
//#define MAX_HTMLDLG_NUM		10
//
//class CHtmlDlg  
//{
//public:
//	CHtmlDlg();
//	virtual ~CHtmlDlg();
//
//	static BOOL CALLBACK	StatiCHtmlDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL CALLBACK			HtmlDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//	void Create( HINSTANCE hInst, HWND hParent,SI32 index );
//	void Set(TCHAR * Url,TCHAR * Txt,bool bWeb,SI32 x,SI32 y,SI32 xsize,SI32 ysize);
//
//	void Show();
//	void Hide();
//
//	BOOL GetDlgShow(){return m_bShow;}
//
//private:
//	HWND m_hParent;
//	HINSTANCE m_hInst;
//
//	HWND m_hDlg;
//
//	BOOL m_bShow;
//
//};
//
//class cltHtmlDlgManager
//{
//public:
//	cltHtmlDlgManager();
//	~cltHtmlDlgManager();
//
//	void Create(HINSTANCE hInst, HWND hParent);
//
//	CHtmlDlg	*m_pCHtmlDlg[MAX_HTMLDLG_NUM];
//
//	void Set(TCHAR * Url,TCHAR * Txt,bool bWeb,SI32 x,SI32 y,SI32 xsize,SI32 ysize);
//
//};
//
//#endif // !defined(AFX_HTMLDLG_H__B3A4DDA2_F574_41D6_8967_B2D2771EB657__INCLUDED_)
