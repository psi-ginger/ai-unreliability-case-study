#pragma once
#include "IdView.h"


// CIdFrame 프레임입니다.

class CIdFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CIdFrame)
protected:
	CIdFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CIdFrame();
	CIdView	m_wndView;

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


