#pragma once

#include "SelledLogView.h"

#include "NSelledLogClientProcess.h"
#include "../../NGameDataManager/NGameDataManager.h"

#define SELLEDLOGCLIENT_NETWORK_TIMER			120
#define SELLEDLOGCLIENT_NETWORK_TIMER_ELAPSE	20

#define SELLEDLOGCLIENT_SHOW_TIMER				200
#define SELLEDLOGCLIENT_SHOW_TIMER_ELAPSE		300

// CSelledLogFrame 프레임입니다.

class CSelledLogFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSelledLogFrame)
protected:
	CSelledLogFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSelledLogFrame();

	CSelledLogView		m_wndView;

public:
	SI32				m_siSearchCount;
	BOOL				m_bSearching;				// 현재 검색 중인가?

	HCURSOR				m_hArrowCursor;
	HCURSOR				m_hWaitCursor;

public:
	NSelledLogClientProcess*	GetLogClientProcess()		{	return m_pkSelledLogClientProcess;	}

	void	SearchEnable(BOOL nEnable);
	BOOL	AddLogData(stLOGData_SelledItem* pLogData);
	BOOL	RequestLogData(sDBRequest_SelledItemLog* pNewLogMsg);
	BOOL	Stop();
	void	SaveLogFile(CString kFileName, int nMode = LVNI_ALL);
	void	LoadLogFile(CString kFileName);

	BOOL	RequestLog();

	void	RefresgLogData();
	void	RemoveAllData();

protected:

	NSelledLogClientProcess*	m_pkSelledLogClientProcess;		// 서버와 통신하는 네트워크 프로세스

	CPtrArray				m_LogDataList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


