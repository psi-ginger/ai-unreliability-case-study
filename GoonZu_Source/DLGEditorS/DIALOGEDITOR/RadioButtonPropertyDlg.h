#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CRadioButtonPropertyDlg dialog

class CRadioButtonPropertyDlg : public CDiagramPropertyDlg
{
// Construction
public:
	CRadioButtonPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRadioButtonPropertyDlg)
	enum { IDD = IDD_DIALOG_RADIOBUTTON_PROPERTIES };
	CEdit	m_editRADB_Property[RADB_PROPERTY_COUNT];
//}}AFX_DATA

	virtual void SetValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadioButtonPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRadioButtonPropertyDlg)
	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedButtonRADBSearch();
	afx_msg void OnBnClickedButtonRADBPathEmpty();
	afx_msg void OnBnClickedButtonRADBTextSearch();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
