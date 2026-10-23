#pragma once
#include "BlockView.h"


// CBlockFrame 프레임입니다.

class CBlockFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBlockFrame)
protected:
	CBlockFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBlockFrame();
	CBlockView m_wndView;


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


