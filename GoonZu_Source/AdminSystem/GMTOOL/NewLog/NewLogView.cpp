// NewLog\NewLogView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "NewLogView.h"
#include "NewLogFrame.h"
#include ".\newlogview.h"
#include "../ItemInfoDlg.h"

#define COLUMNLIST_TMP_FILE		"_columnlist.tmp"

cltListViewColumn g_clTotalListViewColumns[] = 
{
	//	{ "NUM",		"NUM",			LVCFMT_LEFT, 40,	1, 0 },
	{ "DATE",		"cdate",		LVCFMT_LEFT, 120,	2, 0 },
	{ "IP",			"IP",			LVCFMT_LEFT, 90,	2, 0 },
	{ "CMDTYPE",	"CommandType",	LVCFMT_LEFT, 150,	2, 0 },
	{ "CMDINDEX",	"CommandIndex",	LVCFMT_LEFT, 150,	2, 0 },
	{ "REASON",		"Reason", 		LVCFMT_LEFT, 200,	2, 0 },
	{ "IDNUM",		"IDNum", 		LVCFMT_LEFT, 60,	1, 0 },
	{ "PERSON(ID)",	"PersonID", 	LVCFMT_LEFT, 120,	2, 0 },
	{ "LEVEL(EXP)",	"Level(Exp)",	LVCFMT_LEFT, 90,	2, 0 },
	{ "MONEY",		"Money", 		LVCFMT_LEFT, 60,	2, 0 },
	{ "MONEY_SUM",	"MoneySum",		LVCFMT_LEFT, 60,	2, 0 },
	{ "ITEMUNIQUE",	"ItemUnique",	LVCFMT_LEFT, 150,	1, 0 },
	{ "ITEMNUM",    "ItemNum",		LVCFMT_LEFT, 40,	1, 0 },
	{ "ITEMSUM",    "ItemSum",		LVCFMT_LEFT, 40,	1, 0 },
	{ "ITEMDATE",   "ItemDate",		LVCFMT_LEFT, 80,	2, 0 },
	{ "ITEMSID",    "SID",			LVCFMT_LEFT, 40,	1, 0 },
	{ "ITEMDATA",   "ItemData",		LVCFMT_LEFT, 40,	2, 0 },
	{ "RAREGRADE",  "RareGrade",	LVCFMT_LEFT, 40,	2, 0 },
	{ "USEDAY",		"DateUseDay",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTALNUM",	"UseCrystalNum",LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL1_TYPE",			"Crystal1_Type",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL1_DURABILITY",	"Crystal1_Dur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL1_MAXDURABILITY",	"Crystal1_MaxDur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL1_ABILITY",		"Crystal1_Ability",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL1_ABILITYNUM",	"Crystal1_AbilityNum",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL2_TYPE",			"Crystal2_Type",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL2_DURABILITY",	"Crystal2_Dur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL2_MAXDURABILITY",	"Crystal2_MaxDur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL2_ABILITY",		"Crystal2_Ability",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL2_ABILITYNUM",	"Crystal2_AbilityNum",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL3_TYPE",			"Crystal3_Type",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL3_DURABILITY",	"Crystal3_Dur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL3_MAXDURABILITY",	"Crystal3_MaxDur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL3_ABILITY",		"Crystal3_Ability",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL3_ABILITYNUM",	"Crystal3_AbilityNum",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL4_TYPE",			"Crystal4_Type",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL4_DURABILITY",	"Crystal4_Dur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL4_MAXDURABILITY",	"Crystal4_MaxDur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL4_ABILITY",		"Crystal4_Ability",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL4_ABILITYNUM",	"Crystal4_AbilityNum",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL5_TYPE",			"Crystal5_Type",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL5_DURABILITY",	"Crystal5_Dur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL5_MAXDURABILITY",	"Crystal5_MaxDur",	LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL5_ABILITY",		"Crystal5_Ability",		LVCFMT_LEFT, 40,	1, 0 },
	{ "CRYSTAL5_ABILITYNUM",	"Crystal5_AbilityNum",	LVCFMT_LEFT, 40,	1, 0 },
	{ "ENCHANT_TYPE",    "EnchantType",		LVCFMT_LEFT, 40,	1, 0 },
	{ "ENCHANT_POWER",   "EnchantPower",		LVCFMT_LEFT, 40,	1, 0 },
	{ "SUMMON_KIND", "SummonKind",	LVCFMT_LEFT, 60,	2, 0 },
	{ "SUMMON_NAME", "SummonName",	LVCFMT_LEFT, 80,	2, 0 },
	{ "SUMMON_STATUS", "SummonStatus",	LVCFMT_LEFT, 100,	2, 0 },
	{ "SUMMON_ATTACKTYPE", "SummonAttackType",	LVCFMT_LEFT, 100,	2, 0 },
	{ "SUMMON_TAKEINMIN", "TakeInMinute",	LVCFMT_LEFT, 100,	2, 0 },
	{ "MONKIND",    "MonKind",		LVCFMT_LEFT, 60,	1, 0 },
	{ "T_PERSON(ID)",	"TargetPersonID", 	LVCFMT_LEFT, 100,	2, 0 },
	{ "VILLAGEUNIQUE",	"VillageUnique", 	LVCFMT_LEFT, 80,	2, 0 },
	{ "NPCKIND",	"NpcKind", 		LVCFMT_LEFT, 40,	1, 0 },
	{ "HEALTH",		"HealthPoint", 		LVCFMT_LEFT, 180,	2, 0 },
	{ "RPPPOINT",	"RPPoint", 	LVCFMT_LEFT, 80,	1, 0 },
	{ "FAMEEXP",	"FameExp", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "PARAM1",		"Param1", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "PARAM2",		"Param2", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "PARAM3",		"Param3", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "PARAM4",		"Param4", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "PARAM5",		"Param5", 		LVCFMT_LEFT, 80,	1, 0 },
	{ "CHARPARAM1",	"CharParam1", 	LVCFMT_LEFT, 200,	2, 0 },
	{ "CHARPARAM2",	"CharParam2", 	LVCFMT_LEFT, 200,	2, 0 },
	{ "LOGDESC",	"LOGDESC", 		LVCFMT_LEFT, 400,	2, 0 },
};

extern CGMTOOLApp theApp;
int g_nListViewColumns = sizeof(g_clTotalListViewColumns) / sizeof(cltListViewColumn);

// CNewLogView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLogView, CDialog)
CNewLogView::CNewLogView(CWnd* pParent /*=NULL*/)
: CDialog(CNewLogView::IDD, pParent)
{
	m_nRowCount			= 0;
	m_nSortColumn		= 0;
	m_siItemDataColumnNum = 0;
	ZeroMemory(m_ActiveListViewColumn, sizeof(m_ActiveListViewColumn));
	m_ActiveListViewColumnCount	= 0;
}

CNewLogView::~CNewLogView()
{
}

void CNewLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewLogView, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_NEW, OnBnClickedButtonSearchNew)
	ON_BN_CLICKED(IDC_BUTTON_STOP_NEW, OnBnClickedButtonStopNew)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListResultNew)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT_NEW, OnNMClickListResultNew)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DELETE_NEW, OnBnClickedButtonSelDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_DESEL_DELETE_NEW, OnBnClickedButtonDeselDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DELETE_NEW, OnBnClickedButtonAllDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_SEL_EXPORT_NEW, OnBnClickedButtonSelExportNew)
	ON_BN_CLICKED(IDC_BUTTON_ALL_EXPORT_NEW, OnBnClickedButtonAllExportNew)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHINRESULT_NEW, OnBnClickedButtonSearchinresultNew)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT, OnBnClickedButtonUnSelectAll)
	ON_CBN_SELCHANGE(IDC_COMBO_COMMANDTYPE, OnCbnSelchangeComboCommandtype)
	ON_BN_CLICKED(IDC_RADIO_CHARACTER, OnBnClickedRadioCharacter)
	ON_BN_CLICKED(IDC_RADIO_ACCOUNTID, OnBnClickedRadioAccountid)
	ON_BN_CLICKED(IDC_GROUP_CLICKEDITEM2, OnBnClickedGroupClickeditem2)
	ON_BN_CLICKED(IDC_BTN_LOADNEWVIEWER, OnBnClickedBtnLoadnewviewer)
END_MESSAGE_MAP()

BOOL CNewLogView::AddColumnFromName(TCHAR* pName)
{
	if(pName == NULL)			return FALSE;

	int	i;
	for(i=0 ;i<m_ActiveListViewColumnCount; i++)
	{
		if(_tcscmp(m_ActiveListViewColumn[i].szName, pName) == 0)
			return FALSE;
	}

	for(i=0 ;i<g_nListViewColumns; i++)
	{
		if(_tcscmp(g_clTotalListViewColumns[i].szName, pName) == 0)
		{
			m_ActiveListViewColumn[m_ActiveListViewColumnCount].Set(&g_clTotalListViewColumns[i]);
			m_ActiveListViewColumnCount++;
			return TRUE;
		}
	}

	return FALSE;	
}

// CNewLogView 메시지 처리기입니다.

BOOL CNewLogView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return FALSE;

	CDateTimeCtrl* pkDateTime_StartDate	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTDATE_NEW);
	CDateTimeCtrl* pkDateTime_EndDate	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDDATE_NEW);

	if(pkDateTime_StartDate	!= NULL)
	{
		pkDateTime_StartDate->SetFormat("yyyy-MM-dd  HH:mm:ss");
		SYSTEMTIME	systime;
		pkDateTime_StartDate->GetTime(&systime);
		systime.wDay--;
		pkDateTime_StartDate->SetTime(&systime);
	}

	if(pkDateTime_EndDate	!= NULL)
	{
		pkDateTime_EndDate->SetFormat("yyyy-MM-dd  HH:mm:ss");

		SYSTEMTIME	systime;
		pkDateTime_EndDate->GetTime(&systime);
		systime.wDay--;
		pkDateTime_EndDate->SetTime(&systime);
	}

	// 컬럼 정보 상태 저장
	FILE*	fp;
	char	szLine[1024];
	fp = fopen(COLUMNLIST_TMP_FILE, "rt");
	if(fp)
	{
		ZeroMemory(szLine, sizeof(szLine));
		while(fgets(szLine, 1024, fp) != NULL)
		{
			if(szLine[strlen(szLine) -1] == '\n')	szLine[strlen(szLine) -1] = '\0';
			AddColumnFromName(szLine);
		}

		fclose(fp);
	}
	else
	{
		// 기본 컬럼 설정
		AddColumnFromName(TEXT("DATE"));
		AddColumnFromName(TEXT("CMDTYPE"));
		AddColumnFromName(TEXT("CMDINDEX"));
		AddColumnFromName(TEXT("REASON"));
		AddColumnFromName(TEXT("IDNUM"));
		AddColumnFromName(TEXT("PERSON(ID)"));
		AddColumnFromName(TEXT("T_PERSON(ID)"));
		AddColumnFromName(TEXT("MONEY"));
		AddColumnFromName(TEXT("MONEY_SUM"));
		AddColumnFromName(TEXT("ITEMUNIQUE"));
		AddColumnFromName(TEXT("ITEMNUM"));
		AddColumnFromName(TEXT("PARAM1"));
		AddColumnFromName(TEXT("PARAM2"));
		AddColumnFromName(TEXT("PARAM3"));
		AddColumnFromName(TEXT("PARAM4"));
		AddColumnFromName(TEXT("PARAM5"));
		AddColumnFromName(TEXT("CHARPARAM1"));
		AddColumnFromName(TEXT("CHARPARAM2"));
	}

	// 리스트 뷰 설정
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result != NULL)
	{
		for(int i=0; i<m_ActiveListViewColumnCount; i++)
		{
			pkListCtrl_Result->InsertColumn(i, m_ActiveListViewColumn[i].szColumnName, m_ActiveListViewColumn[i].nFormat, m_ActiveListViewColumn[i].nWidth );

			if(!_tcscmp(m_ActiveListViewColumn[i].szColumnName, TEXT("ItemUnique")))
			{
				m_siItemDataColumnNum = i;
			}
		}

		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Result->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Result->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Result->m_hWnd, GWL_STYLE, dwStyle);
	}

	CListCtrl* pkListCtrl_Columnlist = (CListCtrl*)GetDlgItem(IDC_LIST_COLUMNLIST);
	if(pkListCtrl_Columnlist != NULL)
	{	
		pkListCtrl_Columnlist->SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
		pkListCtrl_Columnlist->InsertColumn(0, "COLUMN", LVCFMT_LEFT, 130);

		for(int i=0 ;i<g_nListViewColumns; i++)
		{
			pkListCtrl_Columnlist->InsertItem(LVIF_TEXT, i, g_clTotalListViewColumns[i].szName, 0, 0, 0, 0);
			for(int j=0; j<m_ActiveListViewColumnCount; j++)
			{
				if(strcmp(m_ActiveListViewColumn[j].szName, g_clTotalListViewColumns[i].szName) == 0)
				{
					pkListCtrl_Columnlist->SetCheck(i);
				}
			}
		}
	}

	// 서버 이름 설정
	CComboBox* pkCombo_Server = (CComboBox*)GetDlgItem(IDC_COMBO_SERVER_NEW);
	for(int i=0; i<pkGameDataManager->GetServerInfoCount(); i++)
	{
		NLogServerInfo* pkLogServerInfo = pkGameDataManager->GetLogServerInfoAtIndex(i);
		if(pkLogServerInfo == NULL)				continue;

		pkCombo_Server->AddString(pkLogServerInfo->m_szSeverName);
	}

	int nItem = 0;
	CComboBox* pkCombo_CommandType = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDTYPE);
	if(pkCombo_CommandType)
	{
		nItem = pkCombo_CommandType->AddString("--- ALL ---");
		pkCombo_CommandType->SetItemData(nItem, 0);

		for(int i=1; i<pkGameDataManager->GetLogCommandTypeMaxCount(); i++)
		{
			NLogCommandType* pCommandType = pkGameDataManager->GetLogCommandTypeAtIndex(i);
			if(pCommandType)
			{
				CString tmp;
				tmp.Format("[%2d] ", pCommandType->m_siType);
				tmp += pCommandType->m_szCommand;
				nItem = pkCombo_CommandType->AddString(tmp);
				pkCombo_CommandType->SetItemData(nItem, pCommandType->m_siType);
			}
		}
	}

	CComboBox* pkCombo_CommandIndex = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDINDEX);
	if(pkCombo_CommandIndex)
	{
		pkCombo_CommandIndex->AddString("--- ALL ---");
	}

	CButton*	pkButton_AutoClear = (CButton*)GetDlgItem(IDC_CHECK_AUTOCLEAR_NEW);
	if(pkButton_AutoClear)	pkButton_AutoClear->SetCheck(BST_CHECKED);

	//캐릭터 이름 체크
	CheckRadioButton(IDC_RADIO_CHARACTER,IDC_RADIO_ACCOUNTID,IDC_RADIO_CHARACTER);
	CEdit*	pkCharaterName = (CEdit*)GetDlgItem(IDC_EDIT_CHAR_NEW);
	pkCharaterName->EnableWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNewLogView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result == NULL)			return;

	CEdit* pkEdit_LogText = (CEdit*)GetDlgItem(IDC_EDIT_LOGTEXT_NEW);
	if(pkEdit_LogText == NULL)			return;

	CRect	rc;
	GetClientRect(rc);

	CRect	listrc;
	pkListCtrl_Result->GetWindowRect(listrc);
	ScreenToClient(listrc);
	pkListCtrl_Result->MoveWindow(listrc.left, listrc.top, rc.right - 15, rc.bottom - listrc.top - 5 - 150);

	pkEdit_LogText->MoveWindow(listrc.left, rc.bottom - 150, rc.right - 15, 85);

	CEdit* pkEdit_LogText2 = (CEdit*)GetDlgItem(IDC_EDIT_LOGTEXT2_NEW);
	if(pkEdit_LogText2 == NULL)			return;

	pkEdit_LogText2->MoveWindow(listrc.left, rc.bottom - 63, rc.right - 15, 60);
}

BOOL CNewLogView::PreTranslateMessage(MSG* pMsg)
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

void CNewLogView::OnBnClickedButtonSearchNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// START DATETIME, END DATETIME
	CDateTimeCtrl*	pkDateTime_StartDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTDATE_NEW);
	CDateTimeCtrl*	pkDateTime_EndDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDDATE_NEW);

	pkDateTime_StartDate->GetTime(m_StartTime);
	pkDateTime_EndDate->GetTime(m_EndTime);

	if(m_EndTime < m_StartTime)
	{
		AfxMessageBox("Wrong Time !!.");
		return;
	}

	CButton* pkCheck_AutoClear_New = (CButton*)GetDlgItem(IDC_CHECK_AUTOCLEAR_NEW);
	if(pkCheck_AutoClear_New != NULL && pkCheck_AutoClear_New->GetCheck() == BST_CHECKED)
	{
		CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
		if(pkListCtrl_Result_New == NULL)			return;

		pkListCtrl_Result_New->DeleteAllItems();
		m_nRowCount = 0;

		CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
		if(pkNewLogFrame != NULL)
		{
			pkNewLogFrame->RemoveAllData();
		}
	}

	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame != NULL)		pkNewLogFrame->m_siSearchCount = 0;

	RequestLog();
}

BOOL CNewLogView::RequestLog()
{
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return FALSE;

	sDBRequest_NewLog	NewLog;

	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)		return FALSE;

	CComboBox* pkComboBox_ServerName = (CComboBox*)GetDlgItem(IDC_COMBO_SERVER_NEW);
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

	strcpy(NewLog.szServerName, pkLogServerInfo->m_szTableName);

	NNewLogClientProcess* pkNewLogClientProcess = pkNewLogFrame->GetLogClientProcess();

	// IP, PORT
	pkNewLogClientProcess->SetIP(pkLogServerInfo->m_szIP);
	pkNewLogClientProcess->SetPort(pkLogServerInfo->m_uiPort);

	// ID, PW
	strcpy(NewLog.szLoginID, "test");
	strcpy(NewLog.szLoginPW, "test123");

	// START DATETIME, END DATETIME
	if(m_EndTime < m_StartTime)
	{
		return FALSE;
	}

	if( m_StartTime.GetYear() == m_EndTime.GetYear() && 
		m_StartTime.GetMonth() == m_EndTime.GetMonth() &&
		m_StartTime.GetDay() == m_EndTime.GetDay() )
	{
		sprintf(NewLog.szStartDate, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

		sprintf(NewLog.szEndDate, "%04d-%02d-%02d %02d:%02d:%02d", m_EndTime.GetYear(), m_EndTime.GetMonth(), m_EndTime.GetDay(),
			m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond());
	}
	else
	{
		sprintf(NewLog.szStartDate, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());

		sprintf(NewLog.szEndDate, "%04d-%02d-%02d %02d:%02d:%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay(),
			23, 59, 59);

	}

	// 시작시간이 증가한다.
	CTimeSpan span(1, 0, 0, 0);
	CTime	NextTime;
	NextTime = m_StartTime + span;
	m_StartTime = CTime(NextTime.GetYear(), NextTime.GetMonth(), NextTime.GetDay(), 0, 0, 0);

	// IP 주소
	const SI32 IPSIZE = 16;
	DWORD siAddress = 0;
	CIPAddressCtrl*	pkIPAddress = (CIPAddressCtrl*) GetDlgItem(IDC_IPADDRESS);
	pkIPAddress->SendMessage(IPM_GETADDRESS, 0, (LPARAM)&siAddress);
	
	if( siAddress != 0)
	{
		StringCchPrintf(NewLog.szIP, IPSIZE,"%d.%d.%d.%d", FIRST_IPADDRESS(siAddress),SECOND_IPADDRESS(siAddress),
			THIRD_IPADDRESS(siAddress),FOURTH_IPADDRESS(siAddress));
	}

	// 검색 기준 타입 결정
	if(::SendMessage(::GetDlgItem(m_hWnd, IDC_RADIO_CHARACTER),BM_GETCHECK,0,0))
	{
		NewLog.uiFindType = FIND_CHARACTER;
		GetDlgItemText(IDC_EDIT_CHAR_NEW, NewLog.szPersonName, sizeof(NewLog.szPersonName));
	}
	if(::SendMessage(::GetDlgItem(m_hWnd, IDC_RADIO_ACCOUNTID),BM_GETCHECK,0,0))
	{
		NewLog.uiFindType = FIND_ACCOUNTID;
		GetDlgItemText(IDC_EDIT_ACCOUNTID_NEW, NewLog.szAccountID, sizeof(NewLog.szAccountID));
	}
	
	//GetDlgItemText(IDC_EDIT_TCHAR_NEW, NewLog.szPersonName, sizeof(NewLog.szPersonName));

	char szItemUnique[32];
	GetDlgItemText(IDC_EDIT_ITEMUNIQUE_NEW, szItemUnique, sizeof(szItemUnique));
	if(strlen(szItemUnique) > 0)
	{
		NewLog.siItemUnique = atoi(szItemUnique);

		if(NewLog.siItemUnique <= 0)
		{
			NewLog.siItemUnique = pkGameDataManager->GetItemUnique(szItemUnique);
			if(NewLog.siItemUnique <= 0)
			{
				AfxMessageBox("Unknown Item Name. Please Check Item Name.", MB_OK);
				return FALSE;
			}
		}
	}

	char szItemSID[32];
	GetDlgItemText(IDC_EDIT_ITEMSID_NEW, szItemSID, sizeof(szItemSID));
	NewLog.siItemSID = atoi(szItemSID);

	int	nCmdType = 0;
	int nCmdIndex = 0;

	CComboBox* pkComboBox_CmdType = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDTYPE);
	CComboBox* pkComboBox_CmdIndex = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDINDEX);

	int nItem = 0;

	if(pkComboBox_CmdType && pkComboBox_CmdType->GetCurSel() > 0)
	{		
		nItem = pkComboBox_CmdType->GetCurSel();
		if(nItem >= 0)
		{
			nCmdType = (int)pkComboBox_CmdType->GetItemData(nItem);
		}		
	}

	if(pkComboBox_CmdIndex)
	{
		nItem = pkComboBox_CmdIndex->GetCurSel();
		if(nItem >= 0)
		{
			nCmdIndex = (int)pkComboBox_CmdIndex->GetItemData(nItem);
		}
	}

	char szDetailOption[2048];
	GetDlgItemText(IDC_EDIT_DETAILOPTION, szDetailOption, sizeof(szDetailOption));
	if(strlen(szDetailOption) > 0)
	{
		_tcsncpy(NewLog.szDetailOption, szDetailOption, sizeof(NewLog.szDetailOption));
	}

	NewLog.uiCommandType = (UI08)nCmdType;
	NewLog.uiCommandIndex = (UI08)nCmdIndex;

	pkNewLogFrame->RequestLogData(&NewLog);

	return TRUE;
}

void CNewLogView::OnBnClickedButtonStopNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	pkNewLogFrame->Stop();

	m_StartTime = m_EndTime;
}

void CNewLogView::AddLogData(cltNewLogString* LogStringData, int nCount)
{
	if(LogStringData == NULL)					return;
	if(nCount == 0)								return;

	int nItem		= 0;
	int nSubItem	= -1;

	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result == NULL)				return;

	nItem = pkListCtrl_Result->InsertItem(LVIF_TEXT | LVIF_PARAM, m_nRowCount, "", 0, 0, 0, m_nRowCount);

	m_nRowCount++;

	for(int i=0; i<nCount; i++)
	{
		// 컬럼 번호 찾기
		nSubItem = -1;
		for(int j=0; j<m_ActiveListViewColumnCount; j++)
		{
			if( LogStringData[i].Name.Compare(m_ActiveListViewColumn[j].szName) == 0 )
			{
				nSubItem = j;
				break;
			}
		}

		// 데이터 설정
		if(nSubItem >= 0)
		{
			pkListCtrl_Result->SetItemText(nItem, nSubItem, LogStringData[i].Data);
		}
	}
}

void CNewLogView::OnHdnItemclickListResultNew(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNewLogFrame* pkLogFrame = (CNewLogFrame*)GetParent();
	if(pkLogFrame == NULL)					return;
	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	m_nSortColumn = phdr->iItem;
	m_ActiveListViewColumn[phdr->iItem].nSortOrder = !m_ActiveListViewColumn[phdr->iItem].nSortOrder;

	//pkListCtrl_Result_New->SortItems(MyCompareProc, (DWORD_PTR)pkListCtrl_Result_New);
	pkListCtrl_Result_New->SortItems(MyCompareProc, (DWORD_PTR)this);
	for(int i=0; i<m_nRowCount; i++)
	{
		pkListCtrl_Result_New->SetItem(i, 0, LVIF_PARAM, NULL, 0, 0, 0, i, 0);
	}

	*pResult = 0;
}

// Sort the item in reverse alphabetical order.
int CNewLogView::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	//CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CNewLogView* pkLogView = (CNewLogView*) lParamSort;

	CListCtrl* pListCtrl = (CListCtrl*)pkLogView->GetDlgItem(IDC_LIST_RESULT_NEW);

	CString    strItem1 = pListCtrl->GetItemText((int)lParam1, pkLogView->m_nSortColumn);
	CString    strItem2 = pListCtrl->GetItemText((int)lParam2, pkLogView->m_nSortColumn);

	int nRet = 0;
	switch(pkLogView->m_ActiveListViewColumn[pkLogView->m_nSortColumn].nDataType)
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

	if(!pkLogView->m_ActiveListViewColumn[pkLogView->m_nSortColumn].nSortOrder)
	{
		nRet = -nRet;
	}

	return nRet;
}

void CNewLogView::OnNMClickListResultNew(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	const SI32 TEXTSIZE = 2048;
	const SI32 TMPTEXTSIZE = 512;

	TCHAR szText[TEXTSIZE];
	ZeroMemory(szText , sizeof(TCHAR) * TEXTSIZE);

	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	LVHITTESTINFO	HitTestInfo;
	HitTestInfo.pt		= pNMListView->ptAction;

	pkListCtrl_Result_New->SubItemHitTest(&HitTestInfo);

	if(HitTestInfo.iItem >= 0 && HitTestInfo.iSubItem >= 0)
	{
		CString strText = pkListCtrl_Result_New->GetItemText(HitTestInfo.iItem, HitTestInfo.iSubItem);
		
		if( m_siItemDataColumnNum == HitTestInfo.iSubItem)
		{
			CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();

			stNewLOGData* pNewLogData = pkNewLogFrame->GetNewLogData(HitTestInfo.iItem);
			
			TCHAR szItemInfo[1024] = TEXT("");

			if( TRUE == pkNewLogFrame->m_pclItemManagerClient->GetExplain(&pNewLogData->m_clItem,szItemInfo))
			{
				CItemInfoDlg* itemInfoDlg = new CItemInfoDlg;
				itemInfoDlg->Create(IDD_ITEMINFO);
				
				itemInfoDlg->SetInfoText(szItemInfo);

				itemInfoDlg->ShowWindow(SW_SHOW);
			}
		}		
		SetDlgItemText(IDC_EDIT_CLICKEDITEM_NEW, strText);
		

		SI32 siTmpCount = 0;
		CString	strDescription;
		TCHAR szTmpText[TMPTEXTSIZE];
		TCHAR szCrystalText[TEXTSIZE];
		CString	szColumn;
		ZeroMemory(szCrystalText , sizeof(TCHAR) * TEXTSIZE);

		for(int j=0; j<m_ActiveListViewColumnCount; j++)
		{
			ZeroMemory(szTmpText , sizeof(TCHAR) * TMPTEXTSIZE);
			//if( _tcscmp(m_ActiveListViewColumn[j].szName, "LOGDESC") == 0 )
			//{
			strDescription = pkListCtrl_Result_New->GetItemText(HitTestInfo.iItem, j);
			
			szColumn.Insert(0, m_ActiveListViewColumn[j].szName);
			if( szColumn.Find(TEXT("CRYSTAL")))
			{
				if( siTmpCount % 7 == 6)
					StringCchPrintf( szTmpText, TMPTEXTSIZE , "%s : %s\r\n", m_ActiveListViewColumn[j].szName ,strDescription);
				else
					StringCchPrintf( szTmpText, TMPTEXTSIZE , "%s : %s    ", m_ActiveListViewColumn[j].szName ,strDescription);
				StringCchCat(szText, TEXTSIZE, szTmpText);
				++siTmpCount;
			}
			//}
		}
		
		siTmpCount = 0;
		for(int j=0; j<m_ActiveListViewColumnCount; j++)
		{
			ZeroMemory(szTmpText , sizeof(TCHAR) * TMPTEXTSIZE);
			//if( _tcscmp(m_ActiveListViewColumn[j].szName, "LOGDESC") == 0 )
			//{
			strDescription = pkListCtrl_Result_New->GetItemText(HitTestInfo.iItem, j);
			szColumn.Insert(0, m_ActiveListViewColumn[j].szName);
			if( !szColumn.Find(TEXT("CRYSTAL")))
			{
				if( siTmpCount % 7 == 6)
					StringCchPrintf( szTmpText, TMPTEXTSIZE , "%s : %s\r\n", m_ActiveListViewColumn[j].szName ,strDescription);
				else
					StringCchPrintf( szTmpText, TMPTEXTSIZE , "%s : %s    ", m_ActiveListViewColumn[j].szName ,strDescription);
				StringCchCat(szCrystalText, TEXTSIZE, szTmpText);
				++siTmpCount;
			}
			//}
		}
		SetDlgItemText(IDC_EDIT_LOGTEXT2_NEW, szCrystalText);
		SetDlgItemText(IDC_EDIT_LOGTEXT_NEW, szText);
		
	}

	
}

void CNewLogView::OnBnClickedButtonSelDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkNewLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	int  nItem = -1;
	nItem = pkListCtrl_Result_New->GetNextItem(nItem, LVNI_SELECTED);

	while(nItem != -1)
	{
		pkListCtrl_Result_New->DeleteItem(nItem);
		pkNewLogFrame->DeleteLogData(nItem);
		m_nRowCount--;

		nItem = pkListCtrl_Result_New->GetNextItem(-1, LVNI_SELECTED);
	}
}

void CNewLogView::OnBnClickedButtonDeselDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkNewLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	int  nItem = -1;
	int	 nMaxCount = pkListCtrl_Result_New->GetItemCount();

	int	 i;
	for (i=nMaxCount-1 ; i >= 0 ; i--)
	{
		if(pkListCtrl_Result_New->GetItemState(i, LVNI_SELECTED) != LVNI_SELECTED)
		{
			pkListCtrl_Result_New->DeleteItem(i);
			m_nRowCount--;
		}
	}
}

void CNewLogView::OnBnClickedButtonAllDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkNewLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	pkListCtrl_Result_New->DeleteAllItems();
	m_nRowCount = 0;

	pkNewLogFrame->RemoveAllData();
}

void CNewLogView::OnBnClickedButtonSelExportNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkNewLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	if(pkListCtrl_Result_New->GetSelectedCount() <= 0)
	{
		AfxMessageBox("No Selecteion!!");
		return;
	}

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkNewLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkNewLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_SELECTED);
	}
}

void CNewLogView::OnBnClickedButtonAllExportNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
	if(pkNewLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkNewLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	if(pkListCtrl_Result_New->GetItemCount() <= 0)
	{
		AfxMessageBox("No Data!!");
		return;
	}

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkNewLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkNewLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_ALL);
	}
}

void CNewLogView::OnBnClickedButtonSearchinresultNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택되있는열이 있으면 선택해제한다.
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	CString	strSearchWord;
	CEdit* pkEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEARCHINRESULT_NEW);
	if(pkEdit != NULL)
	{
		pkEdit->GetWindowText(strSearchWord);
	}
	if(strSearchWord.GetLength() <= 0)		return;

	int nIndex = 0;
	nIndex = pkListCtrl_Result_New->GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		pkListCtrl_Result_New->SetItemState(nIndex, 0, LVNI_SELECTED | LVNI_FOCUSED );
		nIndex = pkListCtrl_Result_New->GetNextItem(nIndex, LVNI_SELECTED);
	}

	CString	strText;

	int nSearched = 0;

	for (int i=0;i < pkListCtrl_Result_New->GetItemCount();i++)
	{
		for(int j=1; j<m_ActiveListViewColumnCount; j++)
		{
			strText = pkListCtrl_Result_New->GetItemText(i, j);
			if(strText.Find(strSearchWord, 0) >= 0)
			{
				pkListCtrl_Result_New->SetItemState(i, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
				pkListCtrl_Result_New->SetFocus();
				nSearched++;
			}
		}
	}

	CString Msg;
	Msg.Format("[ %d ] Items Search", nSearched);
	AfxMessageBox(Msg);
}

void CNewLogView::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ActiveListViewColumnCount = 0;

	CListCtrl* pkListCtrl_Columnlist = (CListCtrl*)GetDlgItem(IDC_LIST_COLUMNLIST);
	if(pkListCtrl_Columnlist != NULL)

		for(int i=0; i<pkListCtrl_Columnlist->GetItemCount(); i++)
		{
			if(pkListCtrl_Columnlist->GetCheck(i))
			{
				CString tmp = pkListCtrl_Columnlist->GetItemText(i, 0);
				AddColumnFromName((TCHAR*)(LPCTSTR)tmp);
			}
		}

		// 리스트 뷰 설정
		CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
		if(pkListCtrl_Result != NULL)
		{
			int	i;
			int nColCount = pkListCtrl_Result->GetHeaderCtrl()->GetItemCount();
			for(i=0; i<nColCount; i++)
			{
				pkListCtrl_Result->DeleteColumn(0);
			}

			for(i=0; i<m_ActiveListViewColumnCount; i++)
			{
				pkListCtrl_Result->InsertColumn(i, m_ActiveListViewColumn[i].szColumnName, m_ActiveListViewColumn[i].nFormat, m_ActiveListViewColumn[i].nWidth );
				if(!_tcscmp(m_ActiveListViewColumn[i].szColumnName, TEXT("ItemUnique")))
				{
					m_siItemDataColumnNum = i;
				}
			}
		}
		CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();
		if(pkNewLogFrame == NULL)				return;

		// 컬럼 정보 상태 저장
		FILE* fp;
		fp = fopen(COLUMNLIST_TMP_FILE, "wt");
		if(fp)
		{
			for(int i=0; i<m_ActiveListViewColumnCount; i++)
			{
				fprintf(fp, "%s\n",  m_ActiveListViewColumn[i].szName);
			}

			fclose(fp);
		}

		pkNewLogFrame->RefresgLogData();
}

void CNewLogView::OnCbnSelchangeComboCommandtype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return;

	CComboBox* pkCombo_CommandType = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDTYPE);
	CComboBox* pkCombo_CommandIndex = (CComboBox*)GetDlgItem(IDC_COMBO_COMMANDINDEX);
	int nItem = 0;

	if(pkCombo_CommandType && pkCombo_CommandIndex)
	{
		pkCombo_CommandIndex->ResetContent();
		pkCombo_CommandIndex->AddString("--- ALL ---");
		pkCombo_CommandIndex->SetCurSel(0);

		int nSel = pkCombo_CommandType->GetCurSel();

		if(nSel > 0)
		{
			int nType = pkCombo_CommandType->GetItemData(nSel);

			NLogCommandType* pCommandType = pkGameDataManager->GetLogCommandTypeAtIndex(nType);
			if(pCommandType && pCommandType->m_siIndexStart >= 0)
			{
				for(int i=pCommandType->m_siIndexStart; i<pkGameDataManager->GetLogCommandIndexCount(); i++)
				{
					NLogCommandIndex* pCommandIndex = pkGameDataManager->GetLogCommandIndexAtIndex(i);
					if(pCommandIndex)
					{
						if(pCommandIndex->m_siType != nType)				break;

						if(_tcslen(pCommandIndex->m_szCommand) <= 0)		continue;

						CString tmp;
						tmp.Format("[%2d] ", pCommandIndex->m_siIndex);
						tmp += pCommandIndex->m_szCommand;

						nItem = pkCombo_CommandIndex->AddString(tmp);
						pkCombo_CommandIndex->SetItemData(nItem, pCommandIndex->m_siIndex);
					}
				}
			}
		}
	}
}

void CNewLogView::OnBnClickedButtonSelectAll()
{
	CListCtrl* pkListCtrl_Columnlist = (CListCtrl*)GetDlgItem(IDC_LIST_COLUMNLIST);

	if(pkListCtrl_Columnlist != NULL)
	{
		for(int i=0; i<pkListCtrl_Columnlist->GetItemCount(); i++)
		{
			pkListCtrl_Columnlist->SetCheck(i, TRUE);
		}
	}
}

void CNewLogView::OnBnClickedButtonUnSelectAll()
{
	CListCtrl* pkListCtrl_Columnlist = (CListCtrl*)GetDlgItem(IDC_LIST_COLUMNLIST);

	if(pkListCtrl_Columnlist != NULL)
	{
		for(int i=0; i<pkListCtrl_Columnlist->GetItemCount(); i++)
		{
			pkListCtrl_Columnlist->SetCheck(i, FALSE);
		}
	}
}
void CNewLogView::OnBnClickedRadioCharacter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit*	pkCharaterName = (CEdit*)GetDlgItem(IDC_EDIT_CHAR_NEW);
	pkCharaterName->EnableWindow();
	CEdit*	pkAccountID = (CEdit*)GetDlgItem(IDC_EDIT_ACCOUNTID_NEW);
	pkAccountID->EnableWindow(FALSE);
}

void CNewLogView::OnBnClickedRadioAccountid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit*	pkCharaterName = (CEdit*)GetDlgItem(IDC_EDIT_CHAR_NEW);
	pkCharaterName->EnableWindow(FALSE);
	CEdit*	pkAccountID = (CEdit*)GetDlgItem(IDC_EDIT_ACCOUNTID_NEW);
	pkAccountID->EnableWindow();
}

void CNewLogView::OnBnClickedGroupClickeditem2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CNewLogView::OnBnClickedBtnLoadnewviewer()
{
	CNewLogFrame* pclNewLogFrame = theApp.CreatNewLog();

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;

	int  nItem = -1;
	nItem = pkListCtrl_Result_New->GetNextItem(nItem, LVNI_SELECTED);

	//CNewLogView& rNewLogView = pclNewLogFrame->m_wndView;
	CNewLogFrame* pkNewLogFrame = (CNewLogFrame*)GetParent();

	while(nItem != -1)
	{
		stNewLOGData* pNewLogData = pkNewLogFrame->GetNewLogData(nItem);

		pclNewLogFrame->AddLogData(pNewLogData, 1);

		nItem = pkListCtrl_Result_New->GetNextItem(nItem, LVNI_SELECTED);
	}

	/*while(nItem != -1)
	{
		stNewLOGData* pNewLogData = pkNewLogFrame->GetNewLogData(nItem);
	
		cltNewLogString		LogData[NEWLOGCLIENT_MAX_COLUMN];

		pclNewLogFrame->ConvertToLogDataToStringData(pNewLogData, LogData,  NEWLOGCLIENT_MAX_COLUMN);
		
		rNewLogView.AddLogData(LogData,NEWLOGCLIENT_MAX_COLUMN);

		nItem = pkListCtrl_Result_New->GetNextItem(nItem, LVNI_SELECTED);
	}*/
}
