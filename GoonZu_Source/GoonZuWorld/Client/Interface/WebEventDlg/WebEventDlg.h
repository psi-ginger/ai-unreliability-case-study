//// WebEventDlg.h: interface for the CWebEventDlg class.
////
////////////////////////////////////////////////////////////////////////
//
//#if !defined(AFX_WEBEVENTDLG_H__9D0EFAAD_7ABD_409B_8214_342383896F97__INCLUDED_)
//#define AFX_WEBEVENTDLG_H__9D0EFAAD_7ABD_409B_8214_342383896F97__INCLUDED_
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
//class CWebEventButtonDlg
//{
//public:
//	CWebEventButtonDlg(SI32 x , SI32 y);
//	~CWebEventButtonDlg();
//
//	void ShowWebEventButton(HWND hParent, HINSTANCE hInst,SI32 wx,SI32 wy);
//
//	static BOOL CALLBACK StaticWebEventButtonMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL CALLBACK WebEventButtonButtonMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//private:
//	HWND		m_hWndButton;
//	HINSTANCE	m_hInstButton;
//
//	HWND		m_hButtonDlg; // 다이얼로그 핸들
//	HWND		m_hBitmapButton; // 버튼 핸들
//
//	HBITMAP		m_hButtonBitmap[2]; // 비트맵 이미지 핸들
//
//	SI32	m_siDrawX;
//	SI32	m_siDrawY;
//
//};
//
//class CWebEventDlg  
//{
//public:
//	CWebEventDlg();
//	virtual ~CWebEventDlg();
//
//	static BOOL CALLBACK	StatiCWebEventDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//	BOOL CALLBACK			WebEventDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//
//	void Create( HINSTANCE hInst, HWND hParent );
//	BOOL Set(TCHAR * url,SI32 xsize,SI32 ysize);
//
//	void GetSBSSchoolEventUrl(TCHAR *url,TCHAR * accountID,const TCHAR * name);
//
//	CWebEventButtonDlg * m_pWebEventButtonDlg;
//
//private:
//	HINSTANCE m_hInst;
//	HWND m_hWnd;
//
//	HWND m_hDlg;
//
//};
//
//#endif // !defined(AFX_WEBEVENTDLG_H__9D0EFAAD_7ABD_409B_8214_342383896F97__INCLUDED_)
