// NConvertDlg.h : header file
//

#if !defined(AFX_NCONVERTDLG_H__16CF4236_6BCC_4B15_9345_EB3FC5EE0B21__INCLUDED_)
#define AFX_NCONVERTDLG_H__16CF4236_6BCC_4B15_9345_EB3FC5EE0B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNConvertDlg dialog

class CNConvertDlg : public CDialog
{
// Construction
public:
	CNConvertDlg(CWnd* pParent = NULL);	// standard constructor

	CString			m_strPathName;
	CString			m_strFileName;

	unsigned short 	m_pal565[ 256 ];
	unsigned short	m_pal555[ 256 ];

	BOOL			PcxTrans();
	void			pcxrecord( HANDLE hFile, int count, unsigned char color );

	BOOL			GetPCXPal16( char *filename );

	BOOL			ConvMpr();
	BOOL			ConvSpr();
// Dialog Data
	//{{AFX_DATA(CNConvertDlg)
	enum { IDD = IDD_NCONVERT_DIALOG };
	CString	m_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNConvertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPcxtrans();
	afx_msg void OnButtonConvspr();
	afx_msg void OnButtonConvmpr();
	afx_msg void OnButtonMakealphapcx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCONVERTDLG_H__16CF4236_6BCC_4B15_9345_EB3FC5EE0B21__INCLUDED_)
