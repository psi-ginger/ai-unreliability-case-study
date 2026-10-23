// GoonzuUserlogDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
class CGoonzuLogView;

enum SERVERSELECT
{
	NDOORS,
	NETMARBLE
};

// CGoonzuUserlogDlg 대화 상자
class CGoonzuUserlogDlg : public CDialog
{
// 생성
public:
	CGoonzuUserlogDlg(CWnd* pParent = NULL);	// 표준 생성자
	void ExitProcess();

// 대화 상자 데이터
	enum { IDD = IDD_GOONZUUSERLOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

	


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	char UserID[264];
	char PassWord[64];
	CGoonzuLogView *m_pGoonzuLogView;
	afx_msg void OnBnClickedOk();
	CEdit m_NameEdit;
	CEdit m_PassWord;
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	CComboBox m_cbServerSelect;
};
