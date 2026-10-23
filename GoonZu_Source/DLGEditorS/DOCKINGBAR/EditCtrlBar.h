// EditCtrlBar.h

#if !defined(AFX_EDITCTRLBAR_H__F7296641_28EE_11D4_B73E_004F4C02CAF7__INCLUDED_)
#define AFX_EDITCTRLBAR_H__F7296641_28EE_11D4_B73E_004F4C02CAF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEditCtrlBar window

#include "sizecbar.h"

//#include "AnimationPage.h"

class CEditCtrlBar : public CSizingControlBar
{
// Construction
public:
	CEditCtrlBar();

// Attributes
public:
	//CBezierPage m_pageBezier;
//	CAnimationPage m_pageAnimation;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCtrlBar)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd *pParent);
	void ResetPage();
	void UpdatePage();
	virtual ~CEditCtrlBar();

	// Generated message map functions
protected:
	void ShowAnimationPage();
	void ShowActivePage();
	//{{AFX_MSG(CEditCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCTRLBAR_H__F7296641_28EE_11D4_B73E_004F4C02CAF7__INCLUDED_)
