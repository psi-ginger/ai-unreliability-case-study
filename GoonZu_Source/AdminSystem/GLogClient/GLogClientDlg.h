// GLogClientDlg.h : 헤더 파일
//

#pragma once

//=============================================================================
//	CLASS   X
//! @brief  로그 툴 메인 다이얼로그
//=============================================================================
class CGLogClientDlg : public CDialog
{
// 대화 상자 데이터
enum { IDD = IDD_GLOGCLIENT_DIALOG };

// 생성
public:
	CGLogClientDlg(CWnd* pParent = NULL);	// 표준 생성자

public:
	SI32				m_siSearchCount;

protected:
	//=============================================================================
	// 시스템 변수
	//=============================================================================
	HICON m_hIcon;

	NLogClientProcess*	m_pclLogClientProcess;		// 서버와 통신하는 네트워크 프로세스

	CLogToolBarDlg		m_LogToolBarDlg;			// 로그 검색 툴바
	CLogToolResultDlg	m_LogToolResultDlg;			// 로그 검색 결과 다이얼로그

	BOOL				m_bSearching;				// 현재 검색 중인가?

public:
	//=============================================================================
	// 인터페이스 함수
	//=============================================================================
	BOOL RequestLogData(char* pszStartTime, char* pszEndTime, char* pszServerName, char* pszCommand, char* pszCategory, char* pszPersonName, char* pszDstPersonName, char* pszItemUnique, char* pszUserSearch);
	BOOL Stop();

	NLogClientProcess* GetLogClientProcess()		{		return m_pclLogClientProcess;			}

	BOOL AddLogData(stLOGData* pLogData);
	void ClearLog();
	void SearchEnable(BOOL nEnable);

protected:
	//=============================================================================
	// 기능 함수
	//=============================================================================
	void			NetworkProcess();
	virtual void	DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	//=============================================================================
	// 초기화 함수
	//=============================================================================
	virtual BOOL	OnInitDialog();

	//=============================================================================
	// 이벤트 핸들러
	//=============================================================================
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	afx_msg void		OnTimer(UINT nIDEvent);
	afx_msg void		OnDestroy();
	afx_msg void		OnSize(UINT nType, int cx, int cy);

	//=============================================================================
	// 메시지 맵 함수를 생성했습니다.
	DECLARE_MESSAGE_MAP()
};
