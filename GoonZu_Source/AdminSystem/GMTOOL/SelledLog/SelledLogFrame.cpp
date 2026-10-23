// NewLog\NewLogFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "SelledLogFrame.h"


// CNewLogFrame

IMPLEMENT_DYNCREATE(CSelledLogFrame, CMDIChildWnd)

CSelledLogFrame::CSelledLogFrame()
{
	m_siSearchCount			= 0;
	m_bSearching			= FALSE;
	m_pkSelledLogClientProcess = NULL;
}

CSelledLogFrame::~CSelledLogFrame()
{
	RemoveAllData();

	if(m_pkSelledLogClientProcess)
	{
		delete m_pkSelledLogClientProcess;
	}
}


BEGIN_MESSAGE_MAP(CSelledLogFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNewLogFrame 메시지 처리기입니다.

BOOL CSelledLogFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CSelledLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_SELLEDLOGVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);

	m_pkSelledLogClientProcess = new NSelledLogClientProcess(this);
	m_pkSelledLogClientProcess->Init();

	CString	strPath;
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';

	strPath = szExecuteFile;

	SetTimer(SELLEDLOGCLIENT_NETWORK_TIMER,		SELLEDLOGCLIENT_NETWORK_TIMER_ELAPSE,	NULL);
	SetTimer(SELLEDLOGCLIENT_SHOW_TIMER,		SELLEDLOGCLIENT_SHOW_TIMER_ELAPSE,		NULL);

	m_hArrowCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hWaitCursor	= AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	return 0;
}

void CSelledLogFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();
}

void CSelledLogFrame::OnSize(UINT nType, int cx, int cy)
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

void CSelledLogFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case SELLEDLOGCLIENT_NETWORK_TIMER:
		{
			if( m_pkSelledLogClientProcess != NULL )		m_pkSelledLogClientProcess->Run();
		}
		break;

	case SELLEDLOGCLIENT_SHOW_TIMER:
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


void CSelledLogFrame::SearchEnable(BOOL nEnable)
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


BOOL CSelledLogFrame::RequestLogData(sDBRequest_SelledItemLog* pNewSelledLogMsg)
{
	if( m_pkSelledLogClientProcess == NULL )						return FALSE;
	if( pNewSelledLogMsg == NULL )								return FALSE;

	if( m_pkSelledLogClientProcess->GetProcessState() != SELLEDLOGCLIENTPROCESS_STATE_IDLE)
	{
		AfxMessageBox("In Working. Please Try a few minutes later.", MB_OK);
		return FALSE;
	}

	SearchEnable(FALSE);

	m_pkSelledLogClientProcess->SetIP( "192.168.100.97" );
	m_pkSelledLogClientProcess->SetPort( 8500 );

	m_pkSelledLogClientProcess->RequsetSelledLog(pNewSelledLogMsg);

	return TRUE;
}

BOOL CSelledLogFrame::RequestLog()
{
	return m_wndView.RequestLog();
}

BOOL CSelledLogFrame::Stop()
{
	BOOL bRet;
	if(m_pkSelledLogClientProcess != NULL)
	{
		bRet = m_pkSelledLogClientProcess->Stop();
	}

	return bRet;
}

BOOL CSelledLogFrame::AddLogData(stLOGData_SelledItem* pLogData)
{
	if(pLogData == NULL)									return FALSE;

	stLOGData_SelledItem* pNewData;

	for(SI32 i=0; i<MAX_STLOGDATA_PER_PACKET; i++)
	{
		if(pLogData[i].stSellDate.wYear > 0)
		{
			pNewData = new stLOGData_SelledItem();
			if(pNewData != NULL)
			{
				pNewData->Set(&pLogData[i]);
				m_LogDataList.Add(pNewData);
			}

			m_wndView.AddLogData( &pLogData[i] );
			m_siSearchCount++;
		}
	}

	return TRUE;
}

void CSelledLogFrame::RemoveAllData()
{
	for(int i=0; i<m_LogDataList.GetSize(); i++)
	{
		stLOGData_TradeItem* pDel = (stLOGData_TradeItem*)m_LogDataList.GetAt(i);
		delete pDel;
	}
	m_LogDataList.RemoveAll();
}

void CSelledLogFrame::RefresgLogData()
{
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;
	pkListCtrl_Result_New->DeleteAllItems();

	for(int i=0; i<m_LogDataList.GetSize(); i++)
	{
		stLOGData_SelledItem* pLogData = (stLOGData_SelledItem*)m_LogDataList.GetAt(i);
		if(pLogData)
		{
			m_wndView.AddLogData( pLogData );
		}
	}
}

void CSelledLogFrame::SaveLogFile(CString kFileName, int nMode /* = LVNI_ALL */)
{
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New  == NULL)			return;

	FILE*	fp					= NULL;
	CString ItemStr[7];

	BYTE	UnicodeHeader[3];
	UnicodeHeader[0] = (BYTE)0xff;
	UnicodeHeader[1] = (BYTE)0xfe;

	char	szField[2048] = { '\0', };
	char	szLine[4096];
	WCHAR	wzBufLine[4096];
	char	MakeBuf[4096];

	HDITEM	hditem;
	char	szColumnName[40];

	ZeroMemory(&hditem, sizeof(hditem));
	hditem.mask = HDI_TEXT;
	hditem.cchTextMax = sizeof(szColumnName);
	hditem.pszText = szColumnName;

	for (int i=0; i<pkListCtrl_Result_New->GetHeaderCtrl()->GetItemCount(); i++)
	{
		pkListCtrl_Result_New->GetHeaderCtrl()->GetItem(i, &hditem);
		strcat(szField, hditem.pszText);
		if(i != pkListCtrl_Result_New->GetHeaderCtrl()->GetItemCount()-1)	strcat(szField, "\t");
	}
	strcat(szField, "\r\n");

	CString ItemText;
	fp = fopen(kFileName, "wb");
	if(fp != NULL)
	{
		fwrite(UnicodeHeader, 2, 1, fp);

		ZeroMemory(wzBufLine, sizeof(WCHAR) * 4096);
		int nLen = MultiByteToWideChar(CP_ACP, 0, szField, strlen(szField)+1, wzBufLine, 4096);
		memcpy(MakeBuf, wzBufLine, (wcslen(wzBufLine)) * sizeof(WCHAR));
		fwrite(MakeBuf, (wcslen(wzBufLine)) * sizeof(WCHAR),1, fp);

		int  nItem = -1;
		nItem = pkListCtrl_Result_New ->GetNextItem(nItem, nMode);

		while(nItem != -1)
		{
			ZeroMemory(szLine, sizeof(szLine));
			for (int i=0; i<pkListCtrl_Result_New->GetHeaderCtrl()->GetItemCount(); i++)
			{
				ItemText = pkListCtrl_Result_New->GetItemText(nItem, i);
				strcat(szLine, ItemText);
				if(i != pkListCtrl_Result_New->GetHeaderCtrl()->GetItemCount()-1)	strcat(szLine, "\t");
			}
			strcat(szLine, "\r\n");

			ZeroMemory(wzBufLine, sizeof(WCHAR) * 4096);
			nLen = MultiByteToWideChar(CP_ACP, 0, szLine, strlen(szLine)+1, wzBufLine, 4096);
			memcpy(MakeBuf, wzBufLine, (wcslen(wzBufLine)) * sizeof(WCHAR));
			fwrite(MakeBuf, (wcslen(wzBufLine)) * sizeof(WCHAR),1, fp);

			nItem = pkListCtrl_Result_New ->GetNextItem(nItem, nMode);
		}
		fclose(fp);

		AfxMessageBox( kFileName += " Saved..." );
	}
	else
	{
		AfxMessageBox( "Can't Save !!!" );
	}
}

void CSelledLogFrame::LoadLogFile(CString kFileName)
{
}
