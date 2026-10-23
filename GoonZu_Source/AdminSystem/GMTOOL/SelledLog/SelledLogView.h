#pragma once


#include "../TradeLog/LogDBMsg.h"

// CNewLogView 대화 상자입니다.

class CSelledLogView : public CDialog
{
	DECLARE_DYNAMIC(CSelledLogView)

public:
	CSelledLogView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelledLogView();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_SELLEDLOGVIEW };

	void AddLogData(stLOGData_SelledItem* pkData);

	BOOL RequestLog();

protected:
	int		m_nRowCount;

public:
	int					m_nSortColumn;

protected:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSearchNew();
	afx_msg void OnBnClickedButtonStopNew();
	afx_msg void OnHdnItemclickListResultNew(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListResultNew(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSelDeleteNew();
	afx_msg void OnBnClickedButtonDeselDeleteNew();
	afx_msg void OnBnClickedButtonAllDeleteNew();
	afx_msg void OnBnClickedButtonSelExportNew();
	afx_msg void OnBnClickedButtonAllExportNew();
	afx_msg void OnBnClickedButtonSearchinresultNew();
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonUnSelectAll();
};
