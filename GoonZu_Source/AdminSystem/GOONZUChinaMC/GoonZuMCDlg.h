// GoonZuMCDlg.h : header file
//

#if !defined(AFX_GOONZUMCDLG_H__69573146_06A0_452A_A0E2_81B3B3D802CA__INCLUDED_)
#define AFX_GOONZUMCDLG_H__69573146_06A0_452A_A0E2_81B3B3D802CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCDlg dialog

#include "FrontServerListDlg.h"
#include "GameServerListDlg.h"
#include "StatisticsDlg.h"
#include "MyRichEdit.h"
#include "afxwin.h"



class CGoonZuMCDlg : public CDialog
{
// Construction
public:
	CGoonZuMCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data

	HWND							m_hWndTabDialog[ 10 ];

	CFrontServerListDlg				m_FrontServerListDlg;
	CGameServerListDlg				m_GameServerListDlg;
	CStatisticsDlg					m_StatisticsDlg;

	int								m_maxUser;

	BOOL							m_bDispDisconnect;

	BOOL							m_bSoundOn;
	BOOL							m_bFlashMode;
	BOOL							m_bFlashTogle;

	MyRichEdit						m_richMsg;

	BOOL							m_bError;
	int								m_errorCode;
	char							m_szErrorServer[256];

	DWORD							m_dwLastFrontErrorTick;
	DWORD							m_dwLastGameErrorTick;
	DWORD							m_dwLastSendMailTick;


	CMimeMessage					*m_mimeMsg;

	CString							m_smtpserver;
	CString							m_receiverid;
	CString							m_sendtext;

	DWORD							m_lasterrtick;
	int								m_errorcount;

	void							SendMail();
 
	//{{AFX_DATA(CGoonZuMCDlg)
	enum { IDD = IDD_GOONZUMC_DIALOG };
	CTabCtrl	m_tabMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoonZuMCDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGoonZuMCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangingTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_emailedit;
	CEdit m_smtpedit;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOONZUMCDLG_H__69573146_06A0_452A_A0E2_81B3B3D802CA__INCLUDED_)
