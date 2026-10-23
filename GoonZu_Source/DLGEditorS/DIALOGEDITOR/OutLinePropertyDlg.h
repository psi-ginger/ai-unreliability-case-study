#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class COutLinePropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(COutLinePropertyDlg)
	enum { IDD = IDD_DIALOG_OUTLINE_PROPERTIES };
	CEdit m_editOUTLNE_Property[OUTLNE_PROPERTY_COUNT];
	//}}AFX_DATA

	COutLinePropertyDlg(CWnd* pParent = NULL);
	virtual ~COutLinePropertyDlg(void);

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
	afx_msg void OnBnClickedButtonOUTLNEPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
