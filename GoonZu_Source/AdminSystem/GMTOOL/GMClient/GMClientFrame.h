#pragma once
#include "GMClientView.h"


// CGMClientFrame 프레임입니다.

class CGMClientFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGMClientFrame)
protected:
	CGMClientFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGMClientFrame();
	CGMClientView	m_wndView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


