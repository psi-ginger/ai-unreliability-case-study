#pragma once

#include "DLGDefine.h"
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"
#include "DLGDefine.h"

// CDLGPropertyDlg 대화 상자입니다.
class CDiagramEditor;

class CDLGPropertyDlg : public CDiagramPropertyDlg//public CDialog
{
	DECLARE_DYNAMIC(CDLGPropertyDlg)

public:
	CDLGPropertyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDLGPropertyDlg();
	virtual void SetValues();
	
	virtual void	ShowProperties( CWnd* parent, BOOL show = TRUE );
	void SetDLGEditor(CDiagramEditor* pDLGEditor);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGPROPERTYDLG };
	//{{AFX_DATA(CDialogSettings)
	// 자식창 파일 명이 한 문자열로 관리되므로..
	CEdit m_editDLG_Property[DLG_PROPERTY_COUNT];
	//}}AFX_DATA

public:
	CWnd* m_pParent;
	CDiagramEditor* m_pDLGEditor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgApply();
	afx_msg void OnBnClickedButtonDlgClose();
	afx_msg void OnBnClickedButtonDlgSearch();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonDlgPathEmpty2();
	afx_msg void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
