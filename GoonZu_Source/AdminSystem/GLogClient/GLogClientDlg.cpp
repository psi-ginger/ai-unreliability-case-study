// GLogClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include ".\glogclientdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLogClientDlg 대화 상자



CGLogClientDlg::CGLogClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGLogClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pclLogClientProcess	= NULL;
	m_bSearching			= FALSE;
}

void CGLogClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CGLogClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			{
				return TRUE;
			}
			break;
		}
	}

	return CDialog::PreTranslateMessage( pMsg );
}


BEGIN_MESSAGE_MAP(CGLogClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGLogClientDlg 메시지 처리기

BOOL CGLogClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pclLogClientProcess = new NLogClientProcess();
    m_pclLogClientProcess->Init();

	// 소켓 작업을 위한 타이머
	SetTimer(LOGCLIENT_NETWORK_TIMER, LOGCLIENT_NETWORK_TIMER_ELAPSE, NULL);

	// "Searching..." 글자 깜빡임 타이머
	SetTimer(LOGCLIENT_SHOW_TIMER, LOGCLIENT_SHOW_TIMER_ELAPSE, NULL);

	// 차일드 창 생성
	m_LogToolBarDlg.Create(IDD_TOOL_DIALOG, this);
	m_LogToolResultDlg.Create(IDD_RESULT_DIALOG, this);

	CRect winrc;
	GetClientRect(winrc);

	CRect toolrc;
	m_LogToolBarDlg.GetWindowRect(toolrc);

	m_LogToolBarDlg.MoveWindow(0, 0, winrc.Width(), toolrc.Height());
	m_LogToolResultDlg.MoveWindow(0, toolrc.Height(), winrc.Width(), winrc.Height() - toolrc.Height());

	m_LogToolBarDlg.UpdateWindow();
	m_LogToolBarDlg.ShowWindow(SW_SHOW);

	m_LogToolResultDlg.UpdateWindow();
	m_LogToolResultDlg.ShowWindow(SW_SHOW);

	m_LogToolBarDlg.Init();
	m_LogToolResultDlg.Init();

	SearchEnable(TRUE);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CGLogClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CGLogClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//=============================================================================
//  @Name   OnTimer()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CGLogClientDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case LOGCLIENT_NETWORK_TIMER:
		{
			NetworkProcess();
		}
		break;

	case LOGCLIENT_SHOW_TIMER:
		{
			CStatic* pStatic = (CStatic*)m_LogToolBarDlg.GetDlgItem(IDC_STATIC_SEARCHING);
			if(pStatic)
			{
				if(m_bSearching)
				{
					static BOOL bShow = FALSE;
					bShow = !bShow;
					pStatic->ShowWindow(bShow);
				}
				else
				{
					pStatic->ShowWindow(FALSE);
				}
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

//=============================================================================
//  @Name   OnDestroy()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CGLogClientDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// 네트워크 처리를 위한 타이머 삭제
	KillTimer(LOGCLIENT_NETWORK_TIMER);

	if(m_pclLogClientProcess)
	{
		delete m_pclLogClientProcess;
		m_pclLogClientProcess = NULL;
	}
}

//=============================================================================
//  @Name   OnSize()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CGLogClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int		nTop = 0;

	if(m_LogToolBarDlg.m_hWnd != NULL)
	{
		CRect rc;
		m_LogToolBarDlg.GetWindowRect(rc);
		m_LogToolBarDlg.MoveWindow(0, 0, cx, rc.Height());

		nTop += rc.Height();
	}

	if(m_LogToolResultDlg.m_hWnd != NULL)
	{
		CRect rc;
		CRect winrc;
		m_LogToolResultDlg.GetWindowRect(rc);
		GetClientRect(winrc);
		m_LogToolResultDlg.MoveWindow(0, nTop, cx, winrc.Height() - nTop);
	}

}

//===========================================================================
// 기능 함수
//===========================================================================
void CGLogClientDlg::NetworkProcess()
{
	if( m_pclLogClientProcess != NULL )
	{
		m_pclLogClientProcess->Run();
	}
}

//===========================================================================
// 인터페이스
//===========================================================================
BOOL CGLogClientDlg::RequestLogData(char* pszStartTime, char* pszEndTime, char* pszServerName, char* pszCommand, char* pszCategory, char* pszPersonName, char* pszDstPersonName, char* pszItemUnique, char* pszUserSearch)
{
	if( m_pclLogClientProcess == NULL )						return FALSE;

	if( m_pclLogClientProcess->GetProcessState() != LOGCLIENTPROCESS_STATE_IDLE)
	{
		AfxMessageBox("In Working. Please Try a few minutes later.", MB_OK);
		return FALSE;
	}

	sDBRequest_Log	LogMsg(	0,
							m_pclLogClientProcess->GetLoginID(),
							m_pclLogClientProcess->GetLoginPW(),
							pszCommand, pszCategory,
							pszServerName, pszPersonName, pszDstPersonName,
							pszItemUnique,
							pszUserSearch,
							pszStartTime, pszEndTime);

	m_siSearchCount = 0;
	SearchEnable(FALSE);
	m_pclLogClientProcess->RequsetLog(&LogMsg);

	return TRUE;
}

//=============================================================================
//  @Name   Stop()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  로그 데이터 받는것을 중지한다.
//=============================================================================
BOOL CGLogClientDlg::Stop()
{
	BOOL bRet;
	if(m_pclLogClientProcess != NULL)
	{
		bRet = m_pclLogClientProcess->Stop();
	}

	return bRet;
}

//=============================================================================
//  @Name   AddLogData()
//! @author LeeKH						
//! @date   2007-3-6
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL CGLogClientDlg::AddLogData(stLOGData* pLogData)
{
	if(pLogData == NULL)									return FALSE;

	for(SI32 i=0; i<MAX_STLOGDATA_PER_PACKET; i++)
	{
		if(strlen(pLogData[i].szDate) > 0)
		{
			m_LogToolResultDlg.AddLogData(	pLogData[i].szDate, pLogData[i].szServerName, pLogData[i].szCommand, 
											pLogData[i].szPersonName, pLogData[i].szDstPersonName,
											pLogData[i].szItemUnique, 
											pLogData[i].szLogTxt);
			m_siSearchCount++;
		}
	}

	return TRUE;
}

//=============================================================================
//  @Name   ClearLog()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CGLogClientDlg::ClearLog()
{
	m_LogToolResultDlg.ClearLog();
}

//=============================================================================
//  @Name   SearchEnable()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CGLogClientDlg::SearchEnable(BOOL nEnable)
{
	CButton* pSearchButton = (CButton*)m_LogToolBarDlg.GetDlgItem(IDC_BTN_SEARCH);
	if(pSearchButton != NULL)
	{
		pSearchButton->EnableWindow(nEnable);
	}

	CButton* pStopButton = (CButton*)m_LogToolBarDlg.GetDlgItem(IDC_BUTTON_STOP);
	if(pStopButton != NULL)
	{
		pStopButton->EnableWindow(!nEnable);
	}

	m_bSearching = !nEnable;
}