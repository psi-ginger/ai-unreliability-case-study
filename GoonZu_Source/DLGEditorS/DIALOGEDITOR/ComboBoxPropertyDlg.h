#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CComboBoxPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CComboBoxPropertyDlg)
	enum { IDD = IDD_DIALOG_COMBOBOX_PROPERTIES };
	CEdit m_editCBX_Property[CBX_PROPERTY_COUNT];
	//}}AFX_DATA

	CComboBoxPropertyDlg(CWnd* pParent = NULL);
	virtual ~CComboBoxPropertyDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedButtonCbxSearch();
	afx_msg void OnOK();

public:

	virtual BOOL OnInitDialog();

	virtual void SetValues();
	afx_msg void OnBnClickedButtonCbxPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
