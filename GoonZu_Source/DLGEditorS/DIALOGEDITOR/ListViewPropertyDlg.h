#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CListViewPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CListViewPropertyDlg)
	enum { IDD = IDD_DIALOG_LISTVIEW_PROPERTIES };
	CEdit m_editLSTV_Property[LSTV_PROPERTY_COUNT];
	//}}AFX_DATA

	CListViewPropertyDlg(CWnd* pParent = NULL);
	virtual ~CListViewPropertyDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedButtonLSTVSearch();
	afx_msg void OnOK();

public:

	virtual BOOL OnInitDialog();

	virtual void SetValues();
	afx_msg void OnBnClickedButtonLstvPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
