// LogToolBarDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include ".\logtoolbardlg.h"


// CLogToolBarDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogToolBarDlg, CDialog)
CLogToolBarDlg::CLogToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogToolBarDlg::IDD, pParent)
	, m_strServerName(_T(""))
	, m_strPersonName(_T(""))
	, m_strDate(_T(""))
	, m_strStartTime(_T(""))
	, m_strEndTime(_T(""))
	, m_strSearchWord(_T(""))
	, m_strCommandFilter(_T(""))
	, m_strTargetName(_T(""))
	, m_strCommand(_T(""))
	, m_strItemUnique(_T(""))
{
	m_siCommandMode		= LOGCLIENT_COMMAMDMODE_NONE;
}

CLogToolBarDlg::~CLogToolBarDlg()
{
}

void CLogToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMBO_SERVERNAME, m_strServerName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strPersonName);
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_strDate);
	DDX_DateTimeCtrl(pDX, IDC_STARTTIME, m_strStartTime);
	DDX_DateTimeCtrl(pDX, IDC_ENDTIME, m_strEndTime);
	DDX_Text(pDX, IDC_EDIT_SEARCHWORD, m_strSearchWord);
	DDX_CBString(pDX, IDC_COMBO_FILTER, m_strCommandFilter);
	DDX_Text(pDX, IDC_EDIT_TARGETNAME, m_strTargetName);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_strCommand);
	DDX_Text(pDX, IDC_EDIT_ITEMUNIQUE, m_strItemUnique);
}

BOOL CLogToolBarDlg::PreTranslateMessage(MSG* pMsg)
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


BEGIN_MESSAGE_MAP(CLogToolBarDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_RADIO_COMMAND, OnBnClickedRadioCommand)
	ON_BN_CLICKED(IDC_RADIO_COMMANDFILTER, OnBnClickedRadioCommand)
	ON_WM_KEYUP()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLogToolBarDlg 메시지 처리기입니다.
BOOL CLogToolBarDlg::OnInitDialog()
{
	CDateTimeCtrl* pDateTimeCtrl	= (CDateTimeCtrl*)GetDlgItem(IDC_DATE);
	CDateTimeCtrl* pStartTimeCtrl	= (CDateTimeCtrl*)GetDlgItem(IDC_STARTTIME);
	CDateTimeCtrl* pEndTimeCtrl		= (CDateTimeCtrl*)GetDlgItem(IDC_ENDTIME);

	if(pDateTimeCtrl	!= NULL)
	{
		SYSTEMTIME	systime;
		pDateTimeCtrl->GetTime(&systime);
		systime.wDay--;
		pDateTimeCtrl->SetTime(&systime);
	}
	if(pStartTimeCtrl	!= NULL)	pStartTimeCtrl->SetFormat("HH:mm");
	if(pEndTimeCtrl		!= NULL)	pEndTimeCtrl->SetFormat("HH:mm");

	CButton*	pRadioCommand		= (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CButton*	pRadioCommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);
	CEdit*		pEditCommand		= (CEdit*)GetDlgItem(IDC_EDIT_COMMAND);
	CComboBox*	pComboBoxCommandFilter	= (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);

	if(pRadioCommand)			pRadioCommand->SetCheck(BST_CHECKED);
	if(pEditCommand)			pEditCommand->EnableWindow(TRUE);
	if(pComboBoxCommandFilter)	pComboBoxCommandFilter->EnableWindow(FALSE);

	m_siCommandMode		= LOGCLIENT_COMMAMDMODE_COMMAND;
	
	return TRUE;
}

//=============================================================================
//  @Name   OnBnClickedBtnSearch()
//! @author LeeKH						
//! @date   2007-3-6
//! @param  X   X
//! @return X
//! @brief  서버에 검색 정보를 전송한다.
//=============================================================================
void CLogToolBarDlg::OnBnClickedBtnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGLogClientDlg* pLogClientDlg = (CGLogClientDlg*)AfxGetMainWnd();
	if(pLogClientDlg == NULL)				return;

	NLogClientProcess* pLogClientProcess = pLogClientDlg->GetLogClientProcess();
	if(pLogClientProcess == NULL)			return;

	UpdateData();

	if(m_strServerName.GetLength() <= 0)
	{
		AfxMessageBox("Select Server", MB_OK);
		return;
	}
	
	BOOL		bLoginProcess = FALSE;
	CLoginDlg	LoginDlg;

	if(pLogClientProcess)
	{
		if(	strlen(pLogClientProcess->GetLoginID()) > 0 && 
			strlen(pLogClientProcess->GetLoginPW()) > 0)
		{
			bLoginProcess = TRUE;
		}
		else
		{
			if(LoginDlg.DoModal() == IDOK)
			{
				if(	LoginDlg.m_strLoginID.GetLength() > 0 &&
					LoginDlg.m_strLoginID.GetLength() > 0)
				{
					pLogClientProcess->SetLoginID(LoginDlg.m_strLoginID);
					pLogClientProcess->SetLoginPW(LoginDlg.m_strLoginPW);
					bLoginProcess = TRUE;
				}
			}
		}
	}

	if(bLoginProcess == FALSE)
	{
		return;
	}

	CString		strStartDate, strEndDate;

	strStartDate	= m_strDate;
	strStartDate	+= " ";
	strStartDate	+= m_strStartTime;

	strEndDate		= m_strDate;
	strEndDate		+= " ";
	strEndDate		+= m_strEndTime;

	char		szStartDate[24];
	char		szEndDate[24];

	ZeroMemory(szStartDate, sizeof(szStartDate));
	ZeroMemory(szEndDate,	sizeof(szEndDate));
	MStrCpy(szStartDate,	strStartDate,	sizeof(szStartDate));
	MStrCpy(szEndDate,		strEndDate,		sizeof(szEndDate));

	char		szServerName[MAX_DBLOG_SIZE_SERVERNAME];
	ZeroMemory(szServerName,	sizeof(szServerName));
	MStrCpy(szServerName,	m_strServerName,	sizeof(szServerName));

	char		szPersonName[MAX_PLAYER_NAME];
	ZeroMemory(szPersonName,	sizeof(szPersonName));
	MStrCpy(szPersonName,	m_strPersonName,	sizeof(szPersonName));

	char		szDstPersonName[MAX_PLAYER_NAME];
	ZeroMemory(szDstPersonName,	sizeof(szDstPersonName));
	MStrCpy(szDstPersonName,	m_strTargetName,	sizeof(szDstPersonName));
	
	char		szItemUnique[MAX_DBLOG_SIZE_ITEMUNIQUE];
	ZeroMemory(szItemUnique,	sizeof(szItemUnique));
	MStrCpy(szItemUnique,	m_strItemUnique,	sizeof(szItemUnique));

	char		szLogText[MAX_DBLOG_SIZE_LOGTEXT];
	ZeroMemory(szLogText,	sizeof(szLogText));
	MStrCpy(szLogText,	m_strSearchWord,	sizeof(szLogText));

	char		szCommand[MAX_DBLOG_SIZE_COMMAND];
	ZeroMemory(szCommand,	sizeof(szCommand));
	char		szCategory[MAX_DBLOG_SIZE_CATEGORY];
	ZeroMemory(szCategory,	sizeof(szCategory));

	switch(m_siCommandMode)
	{
	case LOGCLIENT_COMMAMDMODE_COMMAND:
		{
			MStrCpy(szCommand, m_strCommand,		sizeof(szCommand));
		}
		break;
	case LOGCLIENT_COMMAMDMODE_COMMANDFILTER:
		{
			CComboBox* pComboCommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
			if(pComboCommandFilter != NULL && pComboCommandFilter->m_hWnd != NULL)
			{
				SI32 siCurSel = pComboCommandFilter->GetCurSel();
				if(siCurSel >= 0 && siCurSel < m_FilterArray.GetCount())
				{
					MStrCpy(szCategory, m_FilterArray[siCurSel], sizeof(szCategory));
				}		
			}
		}
		break;
	}

	pLogClientDlg->RequestLogData(	szStartDate, szEndDate, 
									szServerName, szCommand, szCategory, 
									szPersonName, szDstPersonName, szItemUnique, 
									szLogText);
}

//=============================================================================
//  @Name   OnBnClickedButtonStop()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolBarDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGLogClientDlg* pLogClientDlg = (CGLogClientDlg*)AfxGetMainWnd();
	if(pLogClientDlg == NULL)				return;

	pLogClientDlg->Stop();
}

//=============================================================================
//  @Name   OnBnClickedRadioCommand()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolBarDlg::OnBnClickedRadioCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CButton*	pRadioCommand		= (CButton*)GetDlgItem(IDC_RADIO_COMMAND);
	CButton*	pRadioCommandFilter = (CButton*)GetDlgItem(IDC_RADIO_COMMANDFILTER);
	CEdit*		pEditCommand		= (CEdit*)GetDlgItem(IDC_EDIT_COMMAND);
	CComboBox*	pComboBoxCommandFilter	= (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);

	SI32	siInputMode = LOGCLIENT_COMMAMDMODE_NONE;

	if(pRadioCommand != NULL)
	{
		if(pRadioCommand->GetCheck() == BST_CHECKED)		{		siInputMode = LOGCLIENT_COMMAMDMODE_COMMAND; 	}
	}

	if(pRadioCommandFilter != NULL)
	{
		if(pRadioCommandFilter->GetCheck() == BST_CHECKED)	{		siInputMode = LOGCLIENT_COMMAMDMODE_COMMANDFILTER; 	}
	}

	switch(siInputMode)
	{
	case LOGCLIENT_COMMAMDMODE_COMMAND:
		{
			if(pEditCommand)			pEditCommand->EnableWindow(TRUE);
			if(pComboBoxCommandFilter)	pComboBoxCommandFilter->EnableWindow(FALSE);
			m_siCommandMode		= LOGCLIENT_COMMAMDMODE_COMMAND;
		}
		break;
	case LOGCLIENT_COMMAMDMODE_COMMANDFILTER:
		{
			if(pEditCommand)			pEditCommand->EnableWindow(FALSE);
			if(pComboBoxCommandFilter)	pComboBoxCommandFilter->EnableWindow(TRUE);
			m_siCommandMode		= LOGCLIENT_COMMAMDMODE_COMMANDFILTER;
		}
		break;
	default:
		{
			if(pEditCommand)			pEditCommand->EnableWindow(FALSE);
			if(pComboBoxCommandFilter)	pComboBoxCommandFilter->EnableWindow(FALSE);
			m_siCommandMode		= LOGCLIENT_COMMAMDMODE_NONE;
		}
		break;
	}

}

//=============================================================================
//  @Name   Init()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL CLogToolBarDlg::Init()
{
	CString	strPath;
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';

	strPath = szExecuteFile;

	CString	ServerNames;
	ServerNames = strPath + "\\Data\\ServerNames.txt";
	if(LoadServerName(ServerNames) == FALSE)
	{
		AfxMessageBox("\\Data\\ServerNames.txt File is Not Found");
	}

	CString	CmdFilters;
	CmdFilters = strPath + "\\Data\\CommandFilters.txt";
	if(LoadCommandFilter(CmdFilters) == FALSE)
	{
		AfxMessageBox("\\Data\\CommandFilters.txt File is Not Found");
	}

	return TRUE;
}

//=============================================================================
//  @Name   LoadServerName()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL CLogToolBarDlg::LoadServerName(LPCTSTR pszFileName)
{
	CComboBox* pComboServerName = (CComboBox*)GetDlgItem(IDC_COMBO_SERVERNAME);
	if(pComboServerName == NULL || pComboServerName->m_hWnd == NULL)		return FALSE;

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( (TCHAR*) pszFileName );

	if (!bLoadFile)
	{
		return FALSE;
	}

	char szServerName[64];

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szServerName,		sizeof(szServerName),		
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			pComboServerName->AddString(szServerName);
		}
	}

	return TRUE;
}

//=============================================================================
//  @Name   LoadCommandFilter()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL CLogToolBarDlg::LoadCommandFilter(LPCSTR pszFileName)
{
	CComboBox* pComboCommandFilter = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
	if(pComboCommandFilter == NULL || pComboCommandFilter->m_hWnd == NULL)		return FALSE;

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	bLoadFile = dataloader.LoadDataFromFile( (TCHAR*) pszFileName );

	if (!bLoadFile)
	{
		return FALSE;
	}

	char szFilterName[64];
	char szCommandFilter[1024];
	CString	strCommandFilter;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szFilterName,		sizeof(szFilterName),		
		NDATA_MBSTRING,	szCommandFilter,	sizeof(szCommandFilter),		
		0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			strCommandFilter = szCommandFilter;
			if(strCommandFilter[0] == '\"')									strCommandFilter.Delete(0, 1);
			if(strCommandFilter[strCommandFilter.GetLength()-1] == '\"')	strCommandFilter.Delete(strCommandFilter.GetLength()-1, 1);
			m_FilterArray.Add(strCommandFilter);
			pComboCommandFilter->AddString(szFilterName);
		}
	}

	return TRUE;
}
