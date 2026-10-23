#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxPropertyDlg dialog

class CCheckBoxPropertyDlg : public CDiagramPropertyDlg
{
// Construction
public:
	CCheckBoxPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckBoxPropertyDlg)
	enum { IDD = IDD_DIALOG_CHECKBOX_PROPERTIES };
	CEdit	m_editCHKB_Property[CHKB_PROPERTY_COUNT];
//}}AFX_DATA

	virtual void SetValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnBnClickedCHKBHide();
	afx_msg void OnBnClickedButtonCHKBSearch();
	afx_msg void OnBnClickedButtonCHKBPathEmpty();
	afx_msg void OnBnClickedButtonCHKBTextSearch();
	afx_msg void OnOK();

public:	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

