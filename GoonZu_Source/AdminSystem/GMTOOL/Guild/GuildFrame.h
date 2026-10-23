#pragma once
#include "GuildView.h"


// CGuildFrame 프레임입니다.

class CGuildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGuildFrame)
protected:
	CGuildFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGuildFrame();
	CGuildView m_wndView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


