#if !defined(AFX_FRONTSERVERLISTDLG_H__7DA5FAA3_306C_4892_95CB_1D204EB7C92C__INCLUDED_)
#define AFX_FRONTSERVERLISTDLG_H__7DA5FAA3_306C_4892_95CB_1D204EB7C92C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrontServerListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrontServerListDlg dialog

#include "ListView.h"

class CFrontServerListDlg : public CDialog
{
// Construction
public:
	CFrontServerListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFrontServerListDlg)
	enum { IDD = IDD_DIALOG_FRONTSERVERLIST };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrontServerListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//Control 
	ListView	m_FrontServerList;
	ListView	m_AuthServerList;

	// Generated message map functions
	//{{AFX_MSG(CFrontServerListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonFronton();
	afx_msg void OnButtonFrontoff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRONTSERVERLISTDLG_H__7DA5FAA3_306C_4892_95CB_1D204EB7C92C__INCLUDED_)
