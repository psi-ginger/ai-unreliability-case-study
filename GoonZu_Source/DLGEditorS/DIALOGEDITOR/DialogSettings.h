#if !defined(AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_)
#define AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSettings.h : header file
//
#include "../resource.h"
#include "DLGDefine.h"
#include "../MyDefine/MyDefine.h"
#include "../SprFile/SprFile.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog

class CDialogSettings : public CDialog//public CDiagramButton//public CDiagramPropertyDlg
{
// Construction
public:
	CDialogSettings(CWnd* pParent = NULL);   // standard constructor
	~CDialogSettings();   // standard destructor

public:

// Dialog Data
	//{{AFX_DATA(CDialogSettings)
	enum { IDD = IDD_DIALOG_SETTINGS };
	//int		m_height;
	//int		m_width;
	int		m_gridHeight;
	int		m_gridWidth;
	int		m_marginLeft;
	int		m_marginTop;
	int		m_marginBottom;
	int		m_marginRight;

	// leon add
	CString	m_defaultImagePath;	// default 경로, 파일에 경로명 저장시 이 부분을 루트로
							// 상대 경로만 저장됨
	CString m_defaultTxtPath;	// default TextMgr path "C:\공유방\betherich\TextMgr\"
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSettings)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_)
