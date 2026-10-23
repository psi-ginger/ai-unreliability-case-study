#pragma once

#include "../resource.h"
#include "GMProcess.h"

// CGMClientView 대화 상자입니다.

class CGMClientView : public CDialog
{
	DECLARE_DYNAMIC(CGMClientView)

public:
	CGMClientView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGMClientView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GMCLIENT };

protected:
	CString	m_strID, m_strPW;
	CString m_strGMCmd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CEdit		m_LoginID;
	CEdit		m_LoginPW;
	CButton		m_LoginButton;
	CEdit		m_GMCommand;
	CButton		m_SearchButton;
	CEdit		m_InfoBox;
	CStatic		m_LoginSection;

	GMProcess*	m_clGMProcess;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo5();

	afx_msg void OnBnClickedLoginbutton();
	afx_msg void OnBnClickedSearchbutton();
	afx_msg void OnTimer(UINT nIDEvent);

public:
	void AddInfoFromServer(const char* pszLine);

	afx_msg void OnBnClickedButton7();
};
