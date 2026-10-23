#pragma once

#include "ItemView.h"


// CItemFrame 프레임입니다.

class CItemFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CItemFrame)
protected:
	CItemFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CItemFrame();

	CItemView m_wndView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


