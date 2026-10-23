#pragma once

#include "LogView.h"
#include "./NLogClientProcess.h"
#include "../../NGameDataManager/NGameDataManager.h"

#define LOGCLIENT_NETWORK_TIMER			100
#define LOGCLIENT_NETWORK_TIMER_ELAPSE	10

#define LOGCLIENT_SHOW_TIMER			200
#define LOGCLIENT_SHOW_TIMER_ELAPSE		300

// CLogFrame 프레임입니다.

class CLogFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogFrame)
protected:
	CLogFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CLogFrame();

	CLogView	m_wndView;

public:
	SI32				m_siSearchCount;
	BOOL				m_bSearching;				// 현재 검색 중인가?

	HCURSOR				m_hArrowCursor;
	HCURSOR				m_hWaitCursor;

public:
	NLogClientProcess*	GetLogClientProcess()		{	return m_pkLogClientProcess;	}

	void	SearchEnable(BOOL nEnable);
	BOOL	AddLogData(stLOGData* pLogData);
	BOOL	RequestLogData(char* pszStartTime, char* pszEndTime, char* pszServerName, char* pszCommand, char* pszCategory, char* pszPersonName, char* pszDstPersonName, char* pszItemUnique, char* pszUserSearch);
	BOOL	Stop();
	void	SaveLogFile(CString kFileName, int nMode = LVNI_ALL, BOOL bToTab = FALSE );
	void	LoadLogFile(CString kFileName);
	BOOL	RequestLog();

protected:

	NLogClientProcess*	m_pkLogClientProcess;		// 서버와 통신하는 네트워크 프로세스

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
protected:
	//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

