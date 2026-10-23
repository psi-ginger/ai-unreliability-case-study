#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CEditBoxPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CEditBoxPropertyDlg)
	enum { IDD = IDD_DIALOG_EDITBOX_PROPERTIES };
	CEdit m_editETB_Property[ETB_PROPERTY_COUNT];
	//}}AFX_DATA

	CEditBoxPropertyDlg(CWnd* pParent = NULL);
	virtual ~CEditBoxPropertyDlg(void);

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
	afx_msg void OnBnClickedButtonEtbPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
