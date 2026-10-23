#pragma once

enum ENUM_RESULT_COLUMN
{
	RESULT_COLUMN_NUM,
	RESULT_COLUMN_DATE,
	RESULT_COLUMN_SERVER,
	RESULT_COLUMN_COMMAND,
	RESULT_COLUMN_USER,
	RESULT_COLUMN_TARGETUSER,
	RESULT_COLUMN_ITEMUNIQUE,
	RESULT_COLUMN_LOGTEXT
};

struct CResultSort
{
	ENUM_RESULT_COLUMN	Column;
	int					nType;		// 1: Number, 2: String
	int					nSort;		// 1: 오름차순, 0: 내림차순
};

// CLogView 대화 상자입니다.

class CLogView : public CDialog
{
	DECLARE_DYNAMIC(CLogView)

public:
	CLogView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogView();

	CToolTipCtrl	m_kToolTip_Result;

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGVIEW };

public:
	void	AddLogData(LPCSTR pszDate, LPCSTR pszServer, LPCSTR pszCommand, LPCSTR pszUser, LPCSTR pszTargetUser, LPCSTR pszItemUnique, LPCSTR pszLogText);

	// 문자열 분리함수
	bool	Split(CString Src, CString Deli, CStringArray &OutString);

	// 문자열 변환 함수
	CString	ChangeString(CString szCommand, CString InputText);

	BOOL	RequestLog();

protected:
	int		m_nRowCount;
	CTime	m_StartTime, m_EndTime;
	CTime	m_CurrentTime;

protected:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedRadioCommand();
	afx_msg void OnBnClickedRadioCommandfilter();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonAllDelete();
	afx_msg void OnBnClickedButtonAllExport();
	afx_msg void OnBnClickedButtonSelExport();
	afx_msg void OnBnClickedButtonSelDelete();
	afx_msg void OnBnClickedButtonDeselDelete();
	//	afx_msg void OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearchinresult();
	afx_msg void OnHdnItemclickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSearchinresult();
	afx_msg void OnEnChangeEditChar();
};
