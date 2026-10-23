// LogFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "LogFrame.h"

// CLogFrame

IMPLEMENT_DYNCREATE(CLogFrame, CMDIChildWnd)

CLogFrame::CLogFrame()
{
	m_bSearching			= FALSE;
	m_hArrowCursor			= NULL;
	m_hWaitCursor			= NULL;
}

CLogFrame::~CLogFrame()
{
	if(m_pkLogClientProcess)
	{
		delete m_pkLogClientProcess;
	}

	//if(m_hArrowCursor)	CloseHandle(m_hArrowCursor);
	//if(m_hWaitCursor)	CloseHandle(m_hWaitCursor);
}


BEGIN_MESSAGE_MAP(CLogFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//윈도우 모양을 만드는

BOOL CLogFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style =	WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}


// CLogFrame 메시지 처리기입니다.

int CLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_LOGVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);

	m_pkLogClientProcess = new NLogClientProcess(this);
	m_pkLogClientProcess->Init();

	CString	strPath;
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';

	strPath = szExecuteFile;

	SetTimer(LOGCLIENT_NETWORK_TIMER,	LOGCLIENT_NETWORK_TIMER_ELAPSE, NULL);
	SetTimer(LOGCLIENT_SHOW_TIMER,		LOGCLIENT_SHOW_TIMER_ELAPSE,	NULL);

	m_hArrowCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hWaitCursor	= AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	return 0;
}

void CLogFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();
}

void CLogFrame::SearchEnable(BOOL nEnable)
{
	CButton* pSearchButton = (CButton*)m_wndView.GetDlgItem(IDC_BUTTON_SEARCH);
	if(pSearchButton != NULL)
	{
		pSearchButton->EnableWindow(nEnable);
	}

	CButton* pStopButton = (CButton*)m_wndView.GetDlgItem(IDC_BUTTON_STOP);
	if(pStopButton != NULL)
	{
		pStopButton->EnableWindow(!nEnable);
	}

	m_bSearching = !nEnable;
}

BOOL CLogFrame::RequestLogData(char* pszStartTime, char* pszEndTime, char* pszServerName, char* pszCommand, char* pszCategory, char* pszPersonName, char* pszDstPersonName, char* pszItemUnique, char* pszUserSearch)
{
	if( m_pkLogClientProcess == NULL )						return FALSE;

	if( m_pkLogClientProcess->GetProcessState() != LOGCLIENTPROCESS_STATE_IDLE)
	{
		AfxMessageBox("In Working. Please Try a few minutes later.", MB_OK);
		return FALSE;
	}

	m_pkLogClientProcess->SetLoginID("test");
	m_pkLogClientProcess->SetLoginPW("test123");

	sDBRequest_Log	LogMsg(	0,
		m_pkLogClientProcess->GetLoginID(),
		m_pkLogClientProcess->GetLoginPW(),
		pszCommand, pszCategory,
		pszServerName, pszPersonName, pszDstPersonName,
		pszItemUnique,
		pszUserSearch,
		pszStartTime, pszEndTime);

	//	m_siSearchCount = 0;
	SearchEnable(FALSE);
	m_pkLogClientProcess->RequsetLog(&LogMsg);

	return TRUE;
}

BOOL CLogFrame::AddLogData(stLOGData* pLogData)
{
	if(pLogData == NULL)									return FALSE;

	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)		return FALSE;

	for(SI32 i=0; i<MAX_STLOGDATA_PER_PACKET; i++)
	{
		if(strlen(pLogData[i].szDate) > 0)
		{
			TCHAR ItemName[256];
			TCHAR* pszItemName = pkGameDataManager->GetItemName(atoi(pLogData[i].szItemUnique));
			if(pszItemName != NULL)
			{
				StringCchPrintf(ItemName, sizeof(ItemName), "%s(%s)", pszItemName, pLogData[i].szItemUnique);
			}
			else
			{
				StringCchPrintf(ItemName, sizeof(ItemName), "%s", pLogData[i].szItemUnique);
			}

			m_wndView.AddLogData(	pLogData[i].szDate, pLogData[i].szServerName, pLogData[i].szCommand, 
				pLogData[i].szPersonName, pLogData[i].szDstPersonName,
				ItemName, 
				pLogData[i].szLogTxt);
			m_siSearchCount++;
		}
	}

	return TRUE;
}

BOOL CLogFrame::Stop()
{
	BOOL bRet;
	if(m_pkLogClientProcess != NULL)
	{
		bRet = m_pkLogClientProcess->Stop();
	}

	return bRet;
}

void CLogFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_wndView.m_hWnd != NULL)
	{
		CRect	rc;
		GetClientRect(rc);

		m_wndView.MoveWindow(rc);
	}

}

void CLogFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case LOGCLIENT_NETWORK_TIMER:
		{
			if( m_pkLogClientProcess != NULL )		m_pkLogClientProcess->Run();
		}
		break;

	case LOGCLIENT_SHOW_TIMER:
		{
			CStatic* pStatic_Searching = (CStatic*)m_wndView.GetDlgItem(IDC_STATIC_SEARCHING);
			if(pStatic_Searching)
			{
				if(m_bSearching)
				{
					static BOOL bShow = FALSE;
					bShow = !bShow;
					pStatic_Searching->ShowWindow(bShow);
				}
				else
				{
					pStatic_Searching->ShowWindow(FALSE);
				}
			}
		}
		break;
	}

	CMDIChildWnd::OnTimer(nIDEvent);
}

void CLogFrame::OnDestroy()
{
	CMDIChildWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(LOGCLIENT_NETWORK_TIMER);
	KillTimer(LOGCLIENT_SHOW_TIMER);
}

void CLogFrame::SaveLogFile(CString kFileName, int nMode /* = LVNI_ALL */, BOOL bToTab /* = FALSE */ )
{
	CListCtrl* pkListCtrl_Result = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	FILE*	fp					= NULL;
	CString ItemStr[7];

	BYTE	UnicodeHeader[3];
	UnicodeHeader[0] = (BYTE)0xff;
	UnicodeHeader[1] = (BYTE)0xfe;

	char	szField[] = "DATE\tSERVER\tCOMMAND\tCHAR\tTARGET_CHAR\tITEM_UNIQUE\tLOGTEXT\r\n";
	char	szLine[4096];
	WCHAR	wzBufLine[4096];
	char	MakeBuf[4096];

	fp = fopen(kFileName, "wb");
	if(fp != NULL)
	{
		fwrite(UnicodeHeader, 2, 1, fp);

		ZeroMemory(wzBufLine, sizeof(WCHAR) * 4096);
		int nLen = MultiByteToWideChar(CP_ACP, 0, szField, strlen(szField), wzBufLine, 4096);
		memcpy(MakeBuf, wzBufLine, wcslen(wzBufLine) * sizeof(WCHAR));
		fwrite(MakeBuf, wcslen(wzBufLine) * sizeof(WCHAR),1, fp);

		int  nItem = -1;
		nItem = pkListCtrl_Result->GetNextItem(nItem, nMode);
		while(nItem != -1)
		{
			ItemStr[0] = pkListCtrl_Result->GetItemText( nItem, 1 );
			ItemStr[1] = pkListCtrl_Result->GetItemText( nItem, 2 );
			ItemStr[2] = pkListCtrl_Result->GetItemText( nItem, 3 );
			ItemStr[3] = pkListCtrl_Result->GetItemText( nItem, 4 );
			ItemStr[4] = pkListCtrl_Result->GetItemText( nItem, 5 );
			ItemStr[5] = pkListCtrl_Result->GetItemText( nItem, 6 );
			ItemStr[6] = pkListCtrl_Result->GetItemText( nItem, 7 );

			if(bToTab)		{		ItemStr[6].Replace(",", "\t");			}

			wsprintf(szLine,	"%s\t"
				"%s\t"
				"%s\t"
				"%s\t"
				"%s\t"
				"%s\t"
				"%s\r\n",ItemStr[0], ItemStr[1], ItemStr[2],
				ItemStr[3], ItemStr[4], ItemStr[5], ItemStr[6]);

			ZeroMemory(wzBufLine, sizeof(WCHAR) * 4096);
			nLen = MultiByteToWideChar(CP_ACP, 0, szLine, strlen(szLine), wzBufLine, 4096);
			memcpy(MakeBuf, wzBufLine, wcslen(wzBufLine) * sizeof(WCHAR));
			fwrite(MakeBuf, wcslen(wzBufLine) * sizeof(WCHAR),1, fp);

			nItem = pkListCtrl_Result->GetNextItem(nItem, nMode);
		}
		fclose(fp);

		AfxMessageBox( kFileName += " Saved..." );
	}
	else
	{
		AfxMessageBox( "Can't Save !!!" );
	}
}

BOOL CLogFrame::RequestLog()
{
	return m_wndView.RequestLog();
}

void CLogFrame::LoadLogFile(CString kFileName)
{
}

BOOL CLogFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_bSearching)
	{
		POINT	pt;
		GetCursorPos(&pt);

		CRect	rc;
		CListCtrl* pkListCtrl_Result = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT);
		pkListCtrl_Result->GetWindowRect(rc);

		if(rc.PtInRect(pt))
		{
			SetCursor(m_hWaitCursor);
			return TRUE;
		}
	}
	else
	{
		SetCursor(m_hArrowCursor);
		return TRUE;
	}

	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
}

