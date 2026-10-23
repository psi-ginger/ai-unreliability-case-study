// GlobalTradeServerDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


// CGlobalTradeServerDlg 대화 상자
class CGlobalTradeServerDlg : public CDialog
{
// 생성
public:
	CGlobalTradeServerDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_GLOBALTRADESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

	BOOL	m_bWorkRun;


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
	afx_msg void OnTimer(UINT nIDEvent);
	CListCtrl m_ListCtrl;
};
