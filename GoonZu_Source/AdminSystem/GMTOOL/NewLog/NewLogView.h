#pragma once

enum // 행동자 타입
{
	ACTERTYPE_CHAR = 1, // 캐릭터 명
	ACTERTYPE_ID,	    // 계정 명
	ACTERTYPE_IP        // 아이피 주소
};

struct cltListViewColumn
{
	TCHAR		szName[32];
	TCHAR		szColumnName[32];	// 보이는 이름.
	int			nFormat;
	int			nWidth;
	int			nDataType;		// 1: Number, 2: String
	int			nSortOrder;		// 1: 오름차순, 0: 내림차순

	void Set(cltListViewColumn* pListViewColumn)
	{
		if(pListViewColumn == NULL)		return;

		memcpy(this, pListViewColumn, sizeof(*this));
	}
};

struct cltNewLogString
{
	CString		Name;
	CString		Data;
};

// CNewLogView 대화 상자입니다.

class CNewLogView : public CDialog
{
	DECLARE_DYNAMIC(CNewLogView)

public:
	CNewLogView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLogView();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOGVIEW };

	void AddLogData(cltNewLogString* LogStringData, int nCount);

	BOOL RequestLog();

protected:
	int		m_nRowCount;

	CTime	m_StartTime, m_EndTime;
	CTime	m_CurrentTime;

	BOOL AddColumnFromName(TCHAR* pName);


public:
	int					m_siItemDataColumnNum;
	int					m_nSortColumn;
	cltListViewColumn	m_ActiveListViewColumn[80];
	int					m_ActiveListViewColumnCount;

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
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonUnSelectAll();
	afx_msg void OnCbnSelchangeComboCommandtype();
	afx_msg void OnBnClickedRadioCharacter();
	afx_msg void OnBnClickedRadioAccountid();
	afx_msg void OnBnClickedGroupClickeditem2();
	afx_msg void OnBnClickedBtnLoadnewviewer();
};
