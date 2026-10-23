#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

class CImageStaticPropertyDlg : public CDiagramPropertyDlg
{
public:
	//{{AFX_DATA(CImageStaticPropertyDlg)
	enum { IDD = IDD_DIALOG_IMAGE_STATIC_PROPERTIES };
	CEdit m_editIMGSTT_Property[IMGSTT_PROPERTY_COUNT];
	//}}AFX_DATA

	CImageStaticPropertyDlg(CWnd* pParent = NULL);
	virtual ~CImageStaticPropertyDlg(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP();

	afx_msg void OnPaint();
	afx_msg void OnButtonApply();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedButtonIMGSTTSearch();
	afx_msg void OnOK();

public:

	virtual BOOL OnInitDialog();

	virtual void SetValues();
	afx_msg void OnBnClickedButtonImgsttPathEmpty();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
