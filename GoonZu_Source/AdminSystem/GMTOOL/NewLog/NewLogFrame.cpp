// NewLog\NewLogFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "NewLogFrame.h"
#include ".\newlogframe.h"


// CNewLogFrame

IMPLEMENT_DYNCREATE(CNewLogFrame, CMDIChildWnd)

CNewLogFrame::CNewLogFrame()
{
	m_siSearchCount			= 0;
	m_bSearching			= FALSE;
	m_pkNewLogClientProcess = NULL;
}

CNewLogFrame::~CNewLogFrame()
{
	RemoveAllData();

	if(m_pkNewLogClientProcess)
	{
		delete m_pkNewLogClientProcess;
	}
}


BEGIN_MESSAGE_MAP(CNewLogFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNewLogFrame 메시지 처리기입니다.

BOOL CNewLogFrame::PreCreateWindow(CREATESTRUCT& cs)
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

int CNewLogFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_NEWLOGVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);

	m_pkNewLogClientProcess = new NNewLogClientProcess(this);
	m_pkNewLogClientProcess->Init();

	CString	strPath;
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';

	strPath = szExecuteFile;

	SetTimer(NEWLOGCLIENT_NETWORK_TIMER,	NEWLOGCLIENT_NETWORK_TIMER_ELAPSE,	NULL);
	SetTimer(NEWLOGCLIENT_SHOW_TIMER,		NEWLOGCLIENT_SHOW_TIMER_ELAPSE,		NULL);

	m_hArrowCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hWaitCursor	= AfxGetApp()->LoadStandardCursor(IDC_WAIT);

	return 0;
}

void CNewLogFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();
}

void CNewLogFrame::OnSize(UINT nType, int cx, int cy)
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

void CNewLogFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case NEWLOGCLIENT_NETWORK_TIMER:
		{
			if( m_pkNewLogClientProcess != NULL )		m_pkNewLogClientProcess->Run();
		}
		break;

	case NEWLOGCLIENT_SHOW_TIMER:
		{
			/*			CStatic* pStatic_Searching = (CStatic*)m_wndView.GetDlgItem(IDC_STATIC_SEARCHING);
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
			}*/
		}
		break;
	}

	CMDIChildWnd::OnTimer(nIDEvent);
}


void CNewLogFrame::SearchEnable(BOOL nEnable)
{
	CButton* pSearchButton = (CButton*)m_wndView.GetDlgItem(IDC_BUTTON_SEARCH_NEW);
	if(pSearchButton != NULL)
	{
		pSearchButton->EnableWindow(nEnable);
	}

	CButton* pStopButton = (CButton*)m_wndView.GetDlgItem(IDC_BUTTON_STOP_NEW);
	if(pStopButton != NULL)
	{
		pStopButton->EnableWindow(!nEnable);
	}

	m_bSearching = !nEnable;
}


BOOL CNewLogFrame::RequestLogData(sDBRequest_NewLog* pNewLogMsg)
{
	if( m_pkNewLogClientProcess == NULL )						return FALSE;
	if( pNewLogMsg == NULL )									return FALSE;

	if( m_pkNewLogClientProcess->GetProcessState() != NEWLOGCLIENTPROCESS_STATE_IDLE)
	{
		AfxMessageBox("In Working. Please Try a few minutes later.", MB_OK);
		return FALSE;
	}

	m_pkNewLogClientProcess->SetLoginID("test");
	m_pkNewLogClientProcess->SetLoginPW("test123");

	sDBRequest_NewLog	NewLogMsg;

	//m_siSearchCount = 0;
	SearchEnable(FALSE);
	m_pkNewLogClientProcess->RequsetLog(pNewLogMsg);

	return TRUE;
}

BOOL CNewLogFrame::RequestLog()
{
	return m_wndView.RequestLog();
}

BOOL CNewLogFrame::Stop()
{
	BOOL bRet;
	if(m_pkNewLogClientProcess != NULL)
	{
		bRet = m_pkNewLogClientProcess->Stop();
	}

	return bRet;
}

BOOL CNewLogFrame::ConvertToLogDataToStringData( stNewLOGData* pLogData, cltNewLogString* pStringData, int nMaxStringData )
{
	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return FALSE;

	if(pLogData == NULL)									return FALSE;

	int nPos = 0;

	pStringData[nPos].Name = "DATE";
	pStringData[nPos].Data.Format("%04d-%02d-%02d %02d:%02d:%02d", pLogData->m_stTime.wYear, pLogData->m_stTime.wMonth, pLogData->m_stTime.wDay,
		pLogData->m_stTime.wHour,	pLogData->m_stTime.wMinute, pLogData->m_stTime.wSecond );
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "IP";
	pStringData[nPos].Data = pLogData->m_szIP;
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CMDTYPE";
	pStringData[nPos].Data.Format("[%2d]", pLogData->m_siCommandType);	

	NLogCommandType* pLogCommandType = pkGameDataManager->GetLogCommandTypeAtIndex(pLogData->m_siCommandType);
	if(pLogCommandType && pLogCommandType->m_siType == pLogData->m_siCommandType)
	{
		pStringData[nPos].Data.Format("[%2d]%s", pLogCommandType->m_siType, pLogCommandType->m_szCommand);	
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CMDINDEX";
	pStringData[nPos].Data.Format("[%2d]", pLogData->m_siCommandIndex);	

	pLogCommandType = pkGameDataManager->GetLogCommandTypeAtIndex(pLogData->m_siCommandType);
	if(pLogCommandType && pLogCommandType->m_siType == pLogData->m_siCommandType)
	{
		for(int i=pLogCommandType->m_siIndexStart; i<pkGameDataManager->GetLogCommandIndexCount(); i++)
		{
			NLogCommandIndex* pCommandIndex = pkGameDataManager->GetLogCommandIndexAtIndex(i);
			if(pCommandIndex && pCommandIndex->m_siIndex == pLogData->m_siCommandIndex)
			{
				pStringData[nPos].Data.Format("[%2d]%s", pCommandIndex->m_siIndex, pCommandIndex->m_szCommand);
				break;
			}
		}
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;
	pStringData[nPos].Name = "REASON";	
	TCHAR* pszReasn = pkGameDataManager->GetReasonDescription(pLogData->m_siCommandType, pLogData->m_siCommandIndex, pLogData->m_uiReason);
	if(pszReasn != NULL && _tcslen(pszReasn) > 0)	{		pStringData[nPos].Data.Format("[%2d]%s", pLogData->m_uiReason, pszReasn);	}
	else											{		pStringData[nPos].Data.Format("[%2d]", pLogData->m_uiReason);				}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "IDNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_siIDNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PERSON(ID)";
	pStringData[nPos].Data.Format("%s(%d)", pLogData->m_szPersonName, pLogData->m_siPersonID);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "LEVEL(EXP)";
	pStringData[nPos].Data.Format("%d(%I64d)", pLogData->m_siLevel, pLogData->m_siExp);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "MONEY";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siMoney);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "MONEY_SUM";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siMoneySum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMUNIQUE";
	TCHAR* pszItemName = NULL;
	if( (pszItemName = pkGameDataManager->GetItemName(pLogData->m_clItem.siUnique)) != NULL )
	{
		pStringData[nPos].Data.Format("[%5d]%s", pLogData->m_clItem.siUnique, pszItemName);
	}
	else
	{
		pStringData[nPos].Data.Format("%d", pLogData->m_clItem.siUnique);
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.siItemNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMSUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_siItemSum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMDATE";
	pStringData[nPos].Data.Format("%04d-%02d-%02d", pLogData->m_clItem.uiDateYear + 2000, pLogData->m_clItem.uiDateMonth, pLogData->m_clItem.uiDateDay);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMSID";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.siSID);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ITEMDATA";
	CString tmp;
	pStringData[nPos].Data = "0x";
	for(SI32 i=0; i<MAX_ITEM_UNION_BYTE; i++)
	{		
		tmp.Format("%02X", pLogData->m_clItem.cData8[i]);
		pStringData[nPos].Data += tmp;
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "RAREGRADE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.clItemCommon.clCommonInfo.uiRareInfo);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "USEDAY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.uiDateUseDay);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;
	
	pStringData[nPos].Name = "CRYSTALNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.siUseCrystalNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL1_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[0].siCrystalType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;
	
	pStringData[nPos].Name = "CRYSTAL1_DURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[0].siDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL1_MAXDURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[0].siMaxDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL1_ABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[0].uiCrystalAbility);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;
	
	pStringData[nPos].Name = "CRYSTAL1_ABILITYNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[0].uiCrystalAbilityNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL2_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[1].siCrystalType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL2_DURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[1].siDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL2_MAXDURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[1].siMaxDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL2_ABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[1].uiCrystalAbility);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL2_ABILITYNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[1].uiCrystalAbilityNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL3_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[2].siCrystalType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL3_DURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[2].siDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL3_MAXDURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[2].siMaxDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL3_ABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[2].uiCrystalAbility);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL3_ABILITYNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[2].uiCrystalAbilityNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL4_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[3].siCrystalType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL4_DURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[3].siDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL4_MAXDURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[3].siMaxDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL4_ABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[3].uiCrystalAbility);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL4_ABILITYNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[3].uiCrystalAbilityNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL5_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[4].siCrystalType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL5_DURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[4].siDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL5_MAXDURABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[4].siMaxDurability);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL5_ABILITY";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[4].uiCrystalAbility);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CRYSTAL5_ABILITYNUM";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.cltItemCrytal[4].uiCrystalAbilityNum);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ENCHANT_TYPE";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.Element.siType);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "ENCHANT_POWER";
	pStringData[nPos].Data.Format("%d", pLogData->m_clItem.Element.siPower);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "SUMMON_KIND";
	pStringData[nPos].Data.Format("%d", pLogData->m_clSummon.siKind);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "SUMMON_NAME";
	pStringData[nPos].Data.Format("%s", pLogData->m_clSummon.szName);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "SUMMON_STATUS";
	switch(pLogData->m_clSummon.siStatus)
	{
	case 1:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("Take out"),pLogData->m_clSummon.siStatus);
			break;
		}
	case 2:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("Take in"),pLogData->m_clSummon.siStatus);
			break;
		}
	case 3:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("Dead"),pLogData->m_clSummon.siStatus);
			break;
		}
	default:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("None"),pLogData->m_clSummon.siStatus);
			break;
		}
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "SUMMON_ATTACKTYPE";
	switch(pLogData->m_clSummon.siAttackType)
	{
	case 1:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("Attacked"),pLogData->m_clSummon.siAttackType);
			break;
		}
	case 2:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("Waited"),pLogData->m_clSummon.siAttackType);
			break;
		}
	default:
		{
			pStringData[nPos].Data.Format("%s(%d)", TEXT("None"),pLogData->m_clSummon.siAttackType);
			break;
		}
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "SUMMON_TAKEINMIN";
	pStringData[nPos].Data.Format("%d", pLogData->m_clSummon.siTakeInMinute);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "T_PERSON(ID)";
	pStringData[nPos].Data.Format("%s(%d)", pLogData->m_szTargetPersonName, pLogData->m_siTargetPersonID);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	TCHAR* pszVillageName = NULL;
	pStringData[nPos].Name = "VILLAGEUNIQUE";
	if( (pszVillageName = pkGameDataManager->GetVillageName(pLogData->m_siVillageUnique)) != NULL )
	{
		pStringData[nPos].Data.Format("[%d]%s", pLogData->m_siVillageUnique, pszVillageName);
	}
	else
	{
		pStringData[nPos].Data.Format("%d", pLogData->m_siVillageUnique);
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "NPCKIND";
	TCHAR* pszCharName = NULL;
	if( (pszCharName = pkGameDataManager->GetCharName(pLogData->m_siNpcKind)) != NULL )
	{
		pStringData[nPos].Data.Format("[%d]%s", pLogData->m_siNpcKind, pszCharName);
	}
	else
	{
		pStringData[nPos].Data.Format("%d", pLogData->m_siNpcKind);
	}
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "HEALTH";
	pStringData[nPos].Data.Format("%d/%d [%d/%d/%d/%d] [%d/%d/%d/%d]", pLogData->m_clHealthInfo.siHungry, pLogData->m_clHealthInfo.uiHealthPoint,
		pLogData->m_clHealthInfo.siInnerOrg1, pLogData->m_clHealthInfo.siInnerOrg2, pLogData->m_clHealthInfo.siInnerOrg3, pLogData->m_clHealthInfo.siInnerOrg4,
		pLogData->m_clHealthInfo.uiOrg1Disease, pLogData->m_clHealthInfo.uiOrg2Disease, pLogData->m_clHealthInfo.uiOrg3Disease, pLogData->m_clHealthInfo.uiOrg4Disease);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "RPPPOINT";
	pStringData[nPos].Data.Format("%d", pLogData->m_siRPPoint);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "FAMEEXP";
	pStringData[nPos].Data.Format("%d", pLogData->m_siFameExp);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PARAM1";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siParam1);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PARAM2";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siParam2);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PARAM3";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siParam3);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PARAM4";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siParam4);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "PARAM5";
	pStringData[nPos].Data.Format("%I64d", pLogData->m_siParam5);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CHARPARAM1";
	pStringData[nPos].Data.Format("%s", pLogData->m_szCharParam1);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	pStringData[nPos].Name = "CHARPARAM2";
	pStringData[nPos].Data.Format("%s", pLogData->m_szCharParam2);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;

	//-----------------------------------------------------------------------------
	// 설명문구는 앞의 데이터가 모두 필요하므로 새로운 데이터는 위에 추가한다.
	//-----------------------------------------------------------------------------
	CString		strDescription;
	CString		strOld;
	pStringData[nPos].Name = "LOGDESC";
	TCHAR* pszDescription = pkGameDataManager->GetDescription(pLogData->m_siCommandType, pLogData->m_siCommandIndex);
	if(pszDescription)		strDescription = pszDescription;

	for(SI32 i=0; i<nPos; i++)
	{
		strOld.Format("#%s#", pStringData[i].Name);			
		strDescription.Replace(strOld, pStringData[i].Data);
	}
	pStringData[nPos].Data.Format("%s", (LPCTSTR)strDescription);
	nPos++;		if(nPos >= nMaxStringData)		return FALSE;
	//-----------------------------------------------------------------------------

	return TRUE;
}

BOOL CNewLogFrame::AddLogData(stNewLOGData* pLogData, SI32 siDataCount)
{
	if(pLogData == NULL)									return FALSE;

	cltNewLogString		LogData[NEWLOGCLIENT_MAX_COLUMN];
	stNewLOGData* pNewData;

	for(SI32 i=0; i<siDataCount; i++)
	{
		if(pLogData[i].m_stTime.wYear > 0)
		{
			pNewData = new stNewLOGData();
			if(pNewData != NULL)
			{
				pNewData->Set(&pLogData[i]);
				m_LogDataList.Add(pNewData);
			}

			ConvertToLogDataToStringData(&pLogData[i], LogData, NEWLOGCLIENT_MAX_COLUMN);

			m_wndView.AddLogData(LogData, NEWLOGCLIENT_MAX_COLUMN);
			m_siSearchCount++;
		}
	}

	return TRUE;
}

stNewLOGData* CNewLogFrame::GetNewLogData(int siIndex)
{
	return (stNewLOGData*)m_LogDataList.GetAt(siIndex);
}

BOOL	CNewLogFrame::DeleteLogData(SI32 siIndex)
{
	if (m_LogDataList.GetCount() <= siIndex || siIndex < 0 )
		return FALSE;

	m_LogDataList.RemoveAt(siIndex);

	return TRUE;
}

void CNewLogFrame::RemoveAllData()
{
	for(int i=0; i<m_LogDataList.GetSize(); i++)
	{
		stNewLOGData* pDel = (stNewLOGData*)m_LogDataList.GetAt(i);
		delete pDel;
	}
	m_LogDataList.RemoveAll();
}

void CNewLogFrame::RefresgLogData()
{
	cltNewLogString		LogData[NEWLOGCLIENT_MAX_COLUMN];

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT_NEW);
	if(pkListCtrl_Result_New == NULL)			return;
	pkListCtrl_Result_New->DeleteAllItems();

	for(int i=0; i<m_LogDataList.GetSize(); i++)
	{
		stNewLOGData* pLogData = (stNewLOGData*)m_LogDataList.GetAt(i);
		if(pLogData)
		{
			ConvertToLogDataToStringData(pLogData, LogData, NEWLOGCLIENT_MAX_COLUMN);
			m_wndView.AddLogData(LogData, NEWLOGCLIENT_MAX_COLUMN);
		}
	}
}

void CNewLogFrame::SaveLogFile(CString kFileName, int nMode /* = LVNI_ALL */)
{
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)m_wndView.GetDlgItem(IDC_LIST_RESULT_NEW);
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

void CNewLogFrame::LoadLogFile(CString kFileName)
{
}

void CNewLogFrame::ActivateFrame(int nCmdShow)
{
	if (nCmdShow == -1)
		nCmdShow = SW_SHOWMAXIMIZED;

	CMDIChildWnd::ActivateFrame(nCmdShow);
}

