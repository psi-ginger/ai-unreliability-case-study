#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CListBoxPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CListBoxPropertyDlg)
	enum { IDD = IDD_DIALOG_LISTBOX_PROPERTIES };
	CEdit m_editLSTB_Property[LSTB_PROPERTY_COUNT];
	//}}AFX_DATA

	CListBoxPropertyDlg(CWnd* pParent = NULL);
	virtual ~CListBoxPropertyDlg(void);

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
	afx_msg void OnBnClickedButtonLstbPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
