#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CChatEditPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CChatEditPropertyDlg)
	enum { IDD = IDD_DIALOG_CHATEDIT_PROPERTIES };
	CEdit m_editCHTETB_Property[CHTETB_PROPERTY_COUNT];
	//}}AFX_DATA

	CChatEditPropertyDlg(CWnd* pParent = NULL);
	virtual ~CChatEditPropertyDlg(void);

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
	afx_msg void OnBnClickedButtonCHTETBPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

