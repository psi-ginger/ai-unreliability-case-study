// EventNotice.h: interface for the CEventNotice class.
//
//////////////////////////////////////////////////////////////////////
#ifdef __SUN_EVENTNOTICE__

//#if !defined(AFX_EVENTNOTICE_H__340D5976_D21A_4A96_9240_785873127B79__INCLUDED_)
//#define AFX_EVENTNOTICE_H__340D5976_D21A_4A96_9240_785873127B79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../../Resource.h"

class CEventNotice  
{
public:
	CEventNotice();
	virtual ~CEventNotice();

	static BOOL CALLBACK	StatiCEventNoticeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			EventNoticeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set(char * szGift,char * Txt);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;

	HBRUSH m_hEditBrush;
};

#endif // !defined(AFX_EVENTNOTICE_H__340D5976_D21A_4A96_9240_785873127B79__INCLUDED_)
