// LogView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "LogFrame.h"
#include "Logview.h"

CResultSort	g_SortInfo[] = 
{
	RESULT_COLUMN_NUM,			1, 0,
		RESULT_COLUMN_DATE,			2, 0,
		RESULT_COLUMN_SERVER,		2, 0,
		RESULT_COLUMN_COMMAND,		2, 0,
		RESULT_COLUMN_USER,			2, 0,
		RESULT_COLUMN_TARGETUSER,	2, 0,
		RESULT_COLUMN_ITEMUNIQUE,	1, 0,
		RESULT_COLUMN_LOGTEXT,		2, 0,
};

CResultSort g_CurrentSort;

// CLogView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogView, CDialog)
CLogView::CLogView(CWnd* pParent /*=NULL*/)
: CDialog(CLogView::IDD, pParent)
{
	m_nRowCount		= 0;
}

CLogView::~CLogView()
{
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogView, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_RADIO_COMMAND, OnBnClickedRadioCommand)
	ON_BN_CLICKED(IDC_RADIO_COMMANDFILTER, OnBnClickedRadioCommandfilter)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DELETE, OnBnClickedButtonAllDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALL_EXPORT, OnBnClickedButtonAllExport)
	ON_BN_CLICKED(IDC_BUTTON_SEL_EXPORT, OnBnClickedButtonSelExport)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DELETE, OnBnClickedButtonSelDelete)
	ON_BN_CLICKED(IDC_BUTTON_DESEL_DELETE, OnBnClickedButtonDeselDelete)
	//	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, OnNMClickListResult)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHINRESULT, OnBnClickedButtonSearchinresult)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListResult)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, OnNMClickListResult)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RESULT, OnLvnItemchangedListResult)
	ON_EN_CHANGE(IDC_EDIT_SEARCHINRESULT, OnEnChangeEditSearchinresult)
	ON_EN_CHANGE(IDC_EDIT_CHAR, OnEnChangeEditChar)
END_MESSAGE_MAP()


// CLogView 메시지 처리기입니다.

BOOL CLogView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return FALSE;

	CDateTimeCtrl* pkDateTime_Date		= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_DATE);
	CDateTimeCtrl* pkDateTime_StartTime	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTTIME);
	CDateTimeCtrl* pkDateTime_EndTime	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDTIME);

	if(pkDateTime_StartTime != NULL)
	{
		pkDateTime_StartTime->SetFormat("yyyy-MM-dd  HH:mm:ss");
	}

	if(pkDateTime_EndTime != NULL)
	{
		pkDateTime_EndTime->SetFormat("yyyy-MM-dd  HH:mm:ss");
	}

	if(pkDateTime_Date	!= NULL)
	{
		SYSTEMTIME	systime;
		pkDateTime_Date->GetTime(&systime);
		systime.wDay--;
		pkDateTime_Date->SetTime(&systime);
	}

	//칼럼 넣기.
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result != NULL)
	{
		pkListCtrl_Result->InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
		pkListCtrl_Result->InsertColumn(1, "DATE",			LVCFMT_LEFT, 140);
		pkListCtrl_Result->InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
		pkListCtrl_Result->InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
		pkListCtrl_Result->InsertColumn(4, "USER",			LVCFMT_LEFT, 120);
		pkListCtrl_Result->InsertColumn(5, "TARGET USER",	LVCFMT_LEFT, 120);
		pkListCtrl_Result->InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
		pkListCtrl_Result->InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 220);

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Result->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Result->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Result->m_hWnd, GWL_STYLE, dwStyle);
	}

	m_kToolTip_Result.Create(pkListCtrl_Result);
	m_kToolTip_Result.Activate(TRUE);
	m_kToolTip_Result.SetWindowText("TEST");
	pkListCtrl_Result->SetToolTips(&m_kToolTip_Result);

	CComboBox* pkCombo_Server = (CComboBox*)GetDlgItem(IDC_COMBO_SERVER);
	for(int i=0; i<pkGameDataManager->GetServerInfoCount(); i++)
	{
		NLogServerInfo* pkLogServerInfo = pkGameDataManager->GetLogServerInfoAtIndex(i);
		if(pkLogServerInfo == NULL)				continue;

		pkCombo_Server->AddString(pkLogServerInfo->m_szSeverName);
	}

	CComboBox* pkCombo_Command = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	for(int i=0; i<pkGameDataManager->GetLogCommandCount(); i++)
	{
		NLogCommand* pkLogCommand = pkGameDataManager->GetLogCommandAtIndex(i);
		if(pkLogCommand == NULL)				continue;

		pkCombo_Command->AddString(pkLogCommand->m_szCommand);
	}

	CComboBox* pkCombo_CommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDFILTER);
	for(int i=0; i<pkGameDataManager->GetLogCommandFilterCount(); i++)
	{
		NLogCommandFilter* pkLogCommandFilter = pkGameDataManager->GetLogCommandFilterAtIndex(i);
		if(pkLogCommandFilter == NULL)				continue;

		pkCombo_CommandFilter->AddString(pkLogCommandFilter->m_szFilterName);
	}

	CButton*	pkButton_Command = (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CButton*	pkButton_CommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);

	pkButton_Command->SetCheck(BST_CHECKED);
	pkButton_CommandFilter->SetCheck(BST_UNCHECKED);

	pkCombo_Command->EnableWindow(TRUE);
	pkCombo_CommandFilter->EnableWindow(FALSE);

	CButton*	pkButton_AutoClear = (CButton*)GetDlgItem(IDC_CHECK_AUTOCLEAR);
	pkButton_AutoClear->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLogView::AddLogData(LPCSTR pszDate, LPCSTR pszServer, LPCSTR pszCommand, LPCSTR pszUser, LPCSTR pszTargetUser, LPCSTR pszItemUnique, LPCSTR pszLogText)
{
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	int nItem = 0;

	char	szTemp[32];
	sprintf(szTemp, "%d", m_nRowCount + 1);
	nItem = pkListCtrl_Result->InsertItem(LVIF_TEXT | LVIF_PARAM, m_nRowCount, szTemp, 0, 0, 0, m_nRowCount);
	pkListCtrl_Result->SetItemText(nItem, 1, pszDate);
	pkListCtrl_Result->SetItemText(nItem, 2, pszServer);
	pkListCtrl_Result->SetItemText(nItem, 3, pszCommand);
	pkListCtrl_Result->SetItemText(nItem, 4, pszUser);
	pkListCtrl_Result->SetItemText(nItem, 5, pszTargetUser);
	pkListCtrl_Result->SetItemText(nItem, 6, pszItemUnique);
	pkListCtrl_Result->SetItemText(nItem, 7, pszLogText);

	m_nRowCount++;
}

void CLogView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	CEdit* pkEdit_LogText = (CEdit*)GetDlgItem(IDC_EDIT_LOGTEXT);
	if(pkEdit_LogText == NULL)			return;

	CRect	rc;
	GetClientRect(rc);

	CRect	listrc;
	pkListCtrl_Result->GetWindowRect(listrc);
	ScreenToClient(listrc);
	pkListCtrl_Result->MoveWindow(listrc.left, listrc.top, rc.right - 15, rc.bottom - listrc.top - 5 - 45);

	pkEdit_LogText->MoveWindow(listrc.left, rc.bottom - 45, rc.right - 15, 40);
}

BOOL CLogView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	return CDialog::PreTranslateMessage(pMsg);
}

void CLogView::OnBnClickedButtonSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDateTimeCtrl*	pkDateTime_StartDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTTIME);
	CDateTimeCtrl*	pkDateTime_EndDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDTIME);

	pkDateTime_StartDate->GetTime(m_StartTime);
	pkDateTime_EndDate->GetTime(m_EndTime);

	if(m_EndTime < m_StartTime)
	{
		AfxMessageBox("Wrong Time !!.");
		return;
	}

	CButton* pkCheck_AutoClear = (CButton*)GetDlgItem(IDC_CHECK_AUTOCLEAR);
	if(pkCheck_AutoClear != NULL && pkCheck_AutoClear->GetCheck() == BST_CHECKED)
	{
		CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
		if(pkListCtrl_Result == NULL)			return;

		pkListCtrl_Result->DeleteAllItems();
		m_nRowCount = 0;
	}

	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame != NULL)		pkLogFrame->m_siSearchCount = 0;

	RequestLog();
}

BOOL CLogView::RequestLog()
{
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return FALSE;

	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)		return FALSE;

	CComboBox* pkComboBox_ServerName = (CComboBox*)GetDlgItem(IDC_COMBO_SERVER);
	if(pkComboBox_ServerName->GetCurSel() < 0)
	{
		AfxMessageBox("Please Select Server!!");
		return FALSE;
	}

	// 서버이름
	TCHAR	szServerName[128];
	pkComboBox_ServerName->GetLBText(pkComboBox_ServerName->GetCurSel(), szServerName);

	NLogServerInfo* pkLogServerInfo = pkGameDataManager->FindLogServerInfo(szServerName);
	if(pkLogServerInfo == NULL)			return FALSE;

	// 명령어
	TCHAR szCommand[MAX_DBLOG_SIZE_COMMAND];
	memset(szCommand, 0, sizeof(szCommand));
	CButton*	pkButton_Command = (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CComboBox* pkCombo_Command = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	if( pkButton_Command->GetCheck() == BST_CHECKED ) //&& pkCombo_Command->GetCurSel() >= 0)
	{
		//pkCombo_Command->GetLBText(pkCombo_Command->GetCurSel(), szCommand);
		GetDlgItemText(IDC_COMBO_COMMAND, szCommand, sizeof(szCommand));
	}

	// 명령어 필터
	TCHAR szFilterName[64];
	memset(szFilterName, 0, sizeof(szFilterName));
	CButton*	pkButton_CommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);
	CComboBox* pkCombo_CommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDFILTER);
	if(pkButton_CommandFilter->GetCheck() == BST_CHECKED && pkCombo_CommandFilter->GetCurSel() >= 0)
	{
		pkCombo_CommandFilter->GetLBText(pkCombo_CommandFilter->GetCurSel(), szFilterName);
	}

	TCHAR* pszFilter = NULL;
	NLogCommandFilter* pkLogCommandFilter = pkGameDataManager->FindCommandFilterInfo(szFilterName);
	if(pkLogCommandFilter != NULL)
	{
		pszFilter = pkLogCommandFilter->m_szFilter;
	}

	// 행동자
	TCHAR szChar[MAX_PLAYER_NAME];
	CEdit* pkEdit_Char = (CEdit*)GetDlgItem(IDC_EDIT_CHAR);
	pkEdit_Char->GetWindowText(szChar, sizeof(szChar));

	// 피 행동자
	TCHAR szTargetChar[MAX_PLAYER_NAME];
	CEdit* pkEdit_TChar = (CEdit*)GetDlgItem(IDC_EDIT_TCHAR);
	pkEdit_TChar->GetWindowText(szTargetChar, sizeof(szTargetChar));

	// 아이템 유니크
	TCHAR szItemUnique[MAX_DBLOG_SIZE_ITEMUNIQUE];
	CEdit* pkEdit_ItemUnique = (CEdit*)GetDlgItem(IDC_EDIT_ITEMUNIQUE);
	pkEdit_ItemUnique->GetWindowText(szItemUnique, sizeof(szItemUnique));

	// LogText
	TCHAR szLogText[MAX_DBLOG_SIZE_LOGTEXT];
	CEdit* pkEdit_LogText = (CEdit*)GetDlgItem(IDC_EDIT_DETAIL);
	pkEdit_LogText->GetWindowText(szLogText, sizeof(szLogText));

	NLogClientProcess* pkLogClientProcess = pkLogFrame->GetLogClientProcess();
	if(pkLogClientProcess == NULL)					return FALSE;

	pkLogClientProcess->SetIP(pkLogServerInfo->m_szIP);
	pkLogClientProcess->SetPort(pkLogServerInfo->m_uiPort);

	TCHAR	szStartTime[50];
	TCHAR	szEndTime[50];

	// START DATETIME, END DATETIME
	if(m_EndTime < m_StartTime)
	{
		return FALSE;
	}

	if( m_StartTime.GetYear() == m_EndTime.GetYear() && 
		m_StartTime.GetMonth() == m_EndTime.GetMonth() &&
		m_StartTime.GetDay() == m_EndTime.GetDay() )
	{
		sprintf(szStartTime, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

		sprintf(szEndTime, "%04d-%02d-%02d %02d:%02d:%02d", m_EndTime.GetYear(), m_EndTime.GetMonth(), m_EndTime.GetDay(),
			m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond());
	}
	else
	{
		sprintf(szStartTime, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

		sprintf(szEndTime, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			23, 59, 59);

	}

	// 시작시간이 증가한다.
	CTimeSpan span(1, 0, 0, 0);
	CTime	NextTime;
	NextTime = m_StartTime + span;
	m_StartTime = CTime(NextTime.GetYear(), NextTime.GetMonth(), NextTime.GetDay(), 0, 0, 0);

	pkLogFrame->RequestLogData(	szStartTime, szEndTime, 
		pkLogServerInfo->m_szTableName, szCommand, pszFilter, 
		szChar, szTargetChar, 
		szItemUnique, szLogText);

	return TRUE;
}

void CLogView::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	pkLogFrame->Stop();

	m_StartTime = m_EndTime;
}

void CLogView::OnBnClickedRadioCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton*	pkButton_Command = (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CButton*	pkButton_CommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);
	CComboBox* pkCombo_Command = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	CComboBox* pkCombo_CommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDFILTER);

	pkCombo_Command->EnableWindow(pkButton_Command->GetCheck() == BST_CHECKED);
	pkCombo_CommandFilter->EnableWindow(pkButton_CommandFilter->GetCheck() == BST_CHECKED);
}

void CLogView::OnBnClickedRadioCommandfilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton*	pkButton_Command = (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CButton*	pkButton_CommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);
	CComboBox* pkCombo_Command = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	CComboBox* pkCombo_CommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDFILTER);

	pkCombo_Command->EnableWindow(pkButton_Command->GetCheck() == BST_CHECKED);
	pkCombo_CommandFilter->EnableWindow(pkButton_CommandFilter->GetCheck() == BST_CHECKED);
}

void CLogView::OnBnClickedButtonAllDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	pkListCtrl_Result->DeleteAllItems();
	m_nRowCount = 0;
}

void CLogView::OnBnClickedButtonSelDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	int  nItem = -1;
	nItem = pkListCtrl_Result->GetNextItem(nItem, LVNI_SELECTED);

	while(nItem != -1)
	{
		pkListCtrl_Result->DeleteItem(nItem);
		m_nRowCount--;

		nItem = pkListCtrl_Result->GetNextItem(-1, LVNI_SELECTED);
	}
}

void CLogView::OnBnClickedButtonDeselDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	int  nItem = -1;
	int	 nMaxCount = pkListCtrl_Result->GetItemCount();

	int	 i;
	for (i=nMaxCount-1 ; i >= 0 ; i--)
	{
		if(pkListCtrl_Result->GetItemState(i, LVNI_SELECTED) != LVNI_SELECTED)
		{
			pkListCtrl_Result->DeleteItem(i);
			m_nRowCount--;
		}
	}
}

void CLogView::OnBnClickedButtonAllExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	CButton*	pkButton_ToTab = (CButton*)GetDlgItem(IDC_CHECK_TOTAB);
	if(pkButton_ToTab == NULL)			return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_ALL, pkButton_ToTab->GetCheck() == BST_CHECKED);
	}
}

void CLogView::OnBnClickedButtonSelExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)				return;

	CButton*	pkButton_ToTab = (CButton*)GetDlgItem(IDC_CHECK_TOTAB);
	if(pkButton_ToTab == NULL)			return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_SELECTED, pkButton_ToTab->GetCheck() == BST_CHECKED);
	}
}

void CLogView::OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO	HitTestInfo;
	HitTestInfo.pt		= pNMListView->ptAction;

	pkListCtrl_Result->SubItemHitTest(&HitTestInfo);

	if(HitTestInfo.iItem >= 0 && HitTestInfo.iSubItem >= 0)
	{
		CString strText = pkListCtrl_Result->GetItemText(HitTestInfo.iItem, HitTestInfo.iSubItem);

		// LogText를 클릭할 경우에는 변환해서 보여준다.
		if (HitTestInfo.iSubItem == 7)
		{
			CString strCommand = pkListCtrl_Result->GetItemText(HitTestInfo.iItem, 3);
			CString OutPutText = ChangeString(strCommand, strText);
			SetDlgItemText(IDC_EDIT_CLICKEDITEM, OutPutText);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_CLICKEDITEM, strText);
		}

		strText = pkListCtrl_Result->GetItemText(HitTestInfo.iItem, 7);

		// ChangeString함수의 인자값을 정의
		CString strCommand = pkListCtrl_Result->GetItemText(HitTestInfo.iItem, 3);
		CString OutPutText = ChangeString(strCommand, strText);
		SetDlgItemText(IDC_EDIT_LOGTEXT, OutPutText);
	}
}

void CLogView::OnBnClickedButtonSearchinresult()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택되있는열이 있으면 선택해제한다.
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	CString	strSearchWord;
	CEdit* pkEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEARCHINRESULT);
	if(pkEdit != NULL)
	{
		pkEdit->GetWindowText(strSearchWord);
	}
	if(strSearchWord.GetLength() <= 0)		return;

	int nIndex = 0;
	nIndex = pkListCtrl_Result->GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		pkListCtrl_Result->SetItemState(nIndex, 0, LVNI_SELECTED | LVNI_FOCUSED );
		nIndex = pkListCtrl_Result->GetNextItem(nIndex, LVNI_SELECTED);
	}

	CString	strText;

	int nSearched = 0;

	for (int i=0;i < pkListCtrl_Result->GetItemCount();i++)
	{
		for(int j=1; j<8; j++)
		{
			strText = pkListCtrl_Result->GetItemText(i, j);
			if(strText.Find(strSearchWord, 0) >= 0)
			{
				pkListCtrl_Result->SetItemState(i, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
				pkListCtrl_Result->SetFocus();
				nSearched++;
			}
		}
	}

	CString Msg;
	Msg.Format("[ %d ] Items Search", nSearched);
	AfxMessageBox(Msg);
}


// Sort the item in reverse alphabetical order.
int CLogView::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;

	CString    strItem1 = pListCtrl->GetItemText(lParam1, g_CurrentSort.Column);
	CString    strItem2 = pListCtrl->GetItemText(lParam2, g_CurrentSort.Column);

	int nRet = 0;
	switch(g_CurrentSort.nType)
	{
	case 1:	// number
		{
			int	nItem1, nItem2;
			nItem1 = atoi(strItem1);
			nItem2 = atoi(strItem2);

			if(nItem1 == nItem2)
				nRet = 0;
			else
				nRet = nItem1 > nItem2 ? 1 : -1;
		}
		break;
	case 2:	// string
		{
			nRet = strcmp(strItem2, strItem1);
		}
		break;
	}

	if(!g_CurrentSort.nSort)
	{
		nRet = -nRet;
	}

	return nRet;
}

void CLogView::OnHdnItemclickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CLogFrame* pkLogFrame = (CLogFrame*)GetParent();
	if(pkLogFrame == NULL)					return;
	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result == NULL)			return;

	g_CurrentSort = g_SortInfo[phdr->iItem];
	g_SortInfo[phdr->iItem].nSort = !g_SortInfo[phdr->iItem].nSort;

	pkListCtrl_Result->SortItems(MyCompareProc, (DWORD_PTR)pkListCtrl_Result);
	for(int i=0; i<m_nRowCount; i++)
	{
		pkListCtrl_Result->SetItem(i, 0, LVIF_PARAM, NULL, 0, 0, 0, i, 0);
	}

	*pResult = 0;
}

void CLogView::OnLvnItemchangedListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CLogView::OnEnChangeEditSearchinresult()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLogView::OnEnChangeEditChar()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//=============================================================================
//  @Name   Split()
//! @author PCK						
//! @date   2008-1-29
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
bool CLogView::Split(CString Src, CString Deli, CStringArray &OutString)
{
	int nStart = 0; 
	int nFind = 0;
	CString temp = Src;

	if(temp[temp.GetLength()-1]!= Deli)
	{
		temp = temp+Deli;
	}

	nFind = temp.Find(Deli, nStart);

	while (nFind >0)
	{
		CString Token;

		Token = temp.Mid(nStart, nFind-nStart);
		OutString.Add(Token);
		nStart = nFind +1;
		nFind = temp.Find(Deli, nStart);
	}

	return TRUE;
}

CString CLogView::ChangeString(CString szCommand, CString InputText)
{
	//*******변수정의*******//
	// ","로 분리된 문자열을 저장할 문자열배열
	CStringArray OutText;
	OutText.RemoveAll();

	// "="으로 분리된 문자열을 저장할 문자열배열
	CStringArray PText;

	// 변환된 최종 LogText를 저장할 문자열
	CString UnionText = "";

	// "a = b" 형식으로 저장될 문자열
	CString UText = "";	
	// ******변수정의끝******//

	// ","로 문자열을 분리한다.
	Split(InputText, ",", OutText);

	//위에 분리된 문자열배열의 배열 갯수를 구한다.
	SI32 nCount = OutText.GetCount();

	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return FALSE;

	// 형변환 (CStrng -> TCHAR*)
	TCHAR* pszCommand;
	pszCommand = szCommand.GetBuffer(szCommand.GetLength());

	// 배열갯수만큼 변환을 한다.
	for(SI32 i=0;i<nCount;i++)
	{
		// 초기화
		PText.RemoveAll();

		// "="로 다시 분리한다.
		Split(OutText[i], "=", PText);

		// 배열 갯수 구하기.
		SI32 nCnt = PText.GetCount();

		// 배열의 갯수가 2개로 되어 있을때만 변환한다.
		// 간혹 "="가 빠져서 2개가 아닌 경우가 있으므로, 그럴때는 변환하지 않고 그냥 그대로 출력한다.
		if(nCnt == 2)
		{
			// 형변환 (Char -> int)
			SI32 siIndex = 0;
			siIndex = atoi(PText[1]);

			TCHAR*	pszResult;

			CString	tolower_cmd;
			CString	tolower_text;
			tolower_cmd = szCommand;			tolower_cmd.MakeLower();	tolower_cmd.Trim();
			tolower_text = PText[0];			tolower_text.MakeLower();	tolower_text.Trim();

			// 컬럼을 비교해서 3가지가 일치하게 되면 변환한다.
			pszResult = pkGameDataManager->FindLogTextTransCommand(tolower_cmd, tolower_text, siIndex);

			// 변환된 문자열을 저장한다.
			// pszResult == NULL이면 그냥 PText[1]을 저장한다.
			if (pszResult == NULL)
			{
				UText = PText[0] + "=" + PText[1];
			}
			else
			{
				UText = PText[0] + "=" + pszResult;
			}

			// 변환된 문자열을 다시 저장한다.
			OutText[i] = UText;
		}

		// 마지막의 ","는 제외한다.
		if (i == nCount-1)
		{
			// 최종변환된 LogText를 저장한다.
			UnionText = UnionText + OutText[i];
		}
		else
		{
			// 최종변환된 LogText를 저장한다.
			UnionText = UnionText + OutText[i]+ ",";
		}
	}
	return UnionText;
}