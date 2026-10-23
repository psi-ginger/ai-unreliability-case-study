#pragma once
#include "afxdtctl.h"
#include "afxwin.h"

enum
{
	LOGCLIENT_COMMAMDMODE_NONE			= 0,
	LOGCLIENT_COMMAMDMODE_COMMAND		= 1,
	LOGCLIENT_COMMAMDMODE_COMMANDFILTER	= 2,
};

// CLogToolBarDlg 대화 상자입니다.

class CLogToolBarDlg : public CDialog
{
// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOOL_DIALOG };

	DECLARE_DYNAMIC(CLogToolBarDlg)

public:
	CLogToolBarDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogToolBarDlg();

protected:
	//=============================================================================
	// 시스템 변수
	//=============================================================================
	CStringArray	m_FilterArray;
	SI32			m_siCommandMode;

	//=============================================================================
	// 컨트롤과 연계되는 변수
	//=============================================================================
	CString 		m_strServerName;
	CString 		m_strPersonName;
	CString 		m_strDate;
	CString 		m_strStartTime;
	CString 		m_strEndTime;
	CString 		m_strSearchWord;
	CString 		m_strCommandFilter;
	CString			m_strTargetName;
	CString			m_strCommand;
	CString			m_strItemUnique;

public:
	//=============================================================================
	// 인터페이스 함수
	//=============================================================================
	BOOL			Init();

protected:
	//=============================================================================
	// 기능 함수
	//=============================================================================
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	BOOL			LoadServerName(LPCTSTR pszFileName);
	BOOL			LoadCommandFilter(LPCTSTR pszFileName);

	//=============================================================================
	// 이벤트 핸들러
	//=============================================================================
	afx_msg void	OnBnClickedBtnSearch();
	afx_msg void	OnBnClickedButtonStop();
	afx_msg void	OnBnClickedRadioCommand();

	//=============================================================================
	// 초기화 함수
	//=============================================================================
	virtual BOOL	OnInitDialog();

	//=============================================================================
	// 메시지 맵 함수를 생성했습니다.
	DECLARE_MESSAGE_MAP()
};
