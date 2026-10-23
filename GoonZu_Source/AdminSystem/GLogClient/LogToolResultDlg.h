#pragma once
#include "afxwin.h"
#include "afxcmn.h"

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

// CLogToolResultDlg 대화 상자입니다.

class CLogToolResultDlg : public CDialog
{
// 대화 상자 데이터입니다.
	enum { IDD = IDD_RESULT_DIALOG };

	DECLARE_DYNAMIC(CLogToolResultDlg)

public:
	CLogToolResultDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogToolResultDlg();

private:
	//=============================================================================
	// 시스템 변수
	//=============================================================================
	int				m_nRowCount;
	CNewResultDlg*	m_pChildResultDlg;
	
protected:
	//=============================================================================
	// 컨트롤과 연계되는 변수
	//=============================================================================
	CString		m_strDetailSearchWord;
	CStatic		m_groupboxDetail;
	CEdit		m_editLogText;
	CListCtrl	m_listviewResult;

public:
	//=============================================================================
	// 인터페이스 함수
	//=============================================================================
	void 			AddLogData(LPCSTR pszDate, LPCSTR pszServer, LPCSTR pszCommand, LPCSTR pszUser, LPCSTR pszTargetUser, LPCSTR pszItemUnique, LPCSTR pszLogText);
	void 			ClearLog();
	BOOL 			Init();

protected:
	//=============================================================================
	// 기능 함수
	//=============================================================================
	int	 			AddListCtrlItem(CListCtrl* pListCtrl, int nItem, int nSubItem, char* pszItemText);

	void 			SaveLogFile(CString kFileName, bool bInfo = true);
	void			SaveSelected_LogFile(CString kFileName, bool bInfo = true);
	bool 			LoadLogFile(CString kFileName);

	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	//=============================================================================
	// 초기화 함수
	//=============================================================================
	virtual BOOL	OnInitDialog();

	//=============================================================================
	// 정렬함수
	//=============================================================================
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//=============================================================================
	// 이벤트 핸들러
	//=============================================================================
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonDelSelect();
	afx_msg void OnBnClickedButtonDelDeselect();
	afx_msg void OnBnClickedButtonDetailsearch();
	afx_msg void OnBnClickedButtonSendnewwindow();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSelexport();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonImport();

	//=============================================================================
	// 메시지 맵 함수를 생성했습니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHdnItemclickListResult(NMHDR *pNMHDR, LRESULT *pResult);
};
