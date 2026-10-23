#if !defined(AFX_SELECTPCXFILEDLG_H__F155A87F_5D8C_4335_BFDA_90003E8B2928__INCLUDED_)
#define AFX_SELECTPCXFILEDLG_H__F155A87F_5D8C_4335_BFDA_90003E8B2928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectPCXFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectPCXFileDlg dialog

class CSelectPCXFileDlg : public CDialog
{
// Construction
public:
	CSelectPCXFileDlg(CWnd* pParent = NULL);   // standard constructor

	CString			m_strSrcPathName;
	CString			m_strSrcFileName;

// Dialog Data
	//{{AFX_DATA(CSelectPCXFileDlg)
	enum { IDD = IDD_DIALOG_PCXFILE };
	CString	m_editComm;
	CString	m_editFileName;
	CString	m_editFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectPCXFileDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectPCXFileDlg)
	afx_msg void OnButtonFind();
	afx_msg void OnButtonOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTPCXFILEDLG_H__F155A87F_5D8C_4335_BFDA_90003E8B2928__INCLUDED_)
