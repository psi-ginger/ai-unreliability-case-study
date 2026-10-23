#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CStaticTextPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CStaticTextPropertyDlg)
	enum { IDD = IDD_DIALOG_STATIC_TEXT_PROPERTIES };
	CEdit m_editSTATT_Property[STATT_PROPERTY_COUNT];
	//}}AFX_DATA

	CStaticTextPropertyDlg(CWnd* pParent = NULL);
	virtual ~CStaticTextPropertyDlg(void);

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
	afx_msg void OnBnClickedButtonStattPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
