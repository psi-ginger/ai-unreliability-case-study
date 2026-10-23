#pragma once

// CNewResultDlg 대화 상자입니다.

class CNewResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewResultDlg)

public:
	CNewResultDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewResultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWRESULT_DIALOG };

private:
	//=============================================================================
	// 시스템 변수
	//=============================================================================
	CDialog*		m_pResultDlg;

public:
	//=============================================================================
	// 인터페이스 함수
	//=============================================================================
	void AddLogData(LPCSTR pszDate, LPCSTR pszServer, LPCSTR pszCommand, LPCSTR pszUser, LPCSTR pszTargetUser, LPCSTR pszItemUnique, LPCSTR pszLogText);

protected:
	//=============================================================================
	// 기능 함수
	//=============================================================================
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//=============================================================================
	// 초기화 함수
	//=============================================================================
	virtual BOOL OnInitDialog();

	//=============================================================================
	// 이벤트 핸들러
	//=============================================================================
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//=============================================================================
	// 메시지 맵 함수를 생성했습니다.
	DECLARE_MESSAGE_MAP()
};
