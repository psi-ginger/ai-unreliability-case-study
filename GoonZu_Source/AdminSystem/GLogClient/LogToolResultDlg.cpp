// LogToolResultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include ".\logtoolresultdlg.h"

CResultSort	g_SortInfo[] = 
{
	RESULT_COLUMN_NUM,			1, 1,
	RESULT_COLUMN_DATE,			2, 1,
	RESULT_COLUMN_SERVER,		2, 1,
	RESULT_COLUMN_COMMAND,		2, 1,
	RESULT_COLUMN_USER,			2, 1,
	RESULT_COLUMN_TARGETUSER,	2, 1,
	RESULT_COLUMN_ITEMUNIQUE,	1, 1,
	RESULT_COLUMN_LOGTEXT,		2, 1,
};

CResultSort g_CurrentSort;

// CLogToolResultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogToolResultDlg, CDialog)
CLogToolResultDlg::CLogToolResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogToolResultDlg::IDD, pParent)
	, m_strDetailSearchWord(_T(""))
{
	m_nRowCount			= 0;

	m_pChildResultDlg	= NULL;
}

CLogToolResultDlg::~CLogToolResultDlg()
{
}

void CLogToolResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUPBOX, m_groupboxDetail);
	DDX_Control(pDX, IDC_EDIT_LOGTEXT, m_editLogText);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listviewResult);
	DDX_Text(pDX, IDC_EDIT_DETAILSEARCHWORD, m_strDetailSearchWord);
}

BOOL CLogToolResultDlg::PreTranslateMessage(MSG* pMsg)
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

BEGIN_MESSAGE_MAP(CLogToolResultDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_DEL_SELECT, OnBnClickedButtonDelSelect)
	ON_BN_CLICKED(IDC_BUTTON_DEL_DESELECT, OnBnClickedButtonDelDeselect)
	ON_BN_CLICKED(IDC_BUTTON_DETAILSEARCH, OnBnClickedButtonDetailsearch)
	ON_BN_CLICKED(IDC_BUTTON_SENDNEWWINDOW, OnBnClickedButtonSendnewwindow)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, OnNMClickListResult)
	ON_BN_CLICKED(IDC_BUTTON_SELEXPORT, OnBnClickedButtonSelexport)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnBnClickedButtonImport)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListResult)
END_MESSAGE_MAP()


// CLogToolResultDlg 메시지 처리기입니다.
BOOL CLogToolResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listviewResult.InsertColumn(0, "NUM",			LVCFMT_LEFT,  60);
	m_listviewResult.InsertColumn(1, "DATE",		LVCFMT_LEFT, 140);
	m_listviewResult.InsertColumn(2, "SERVER",		LVCFMT_LEFT, 100);
	m_listviewResult.InsertColumn(3, "COMMAND",		LVCFMT_LEFT, 140);
	m_listviewResult.InsertColumn(4, "USER",		LVCFMT_LEFT, 120);
	m_listviewResult.InsertColumn(5, "TARGET USER", LVCFMT_LEFT, 120);
	m_listviewResult.InsertColumn(6, "ITEM UNIQUE",	LVCFMT_LEFT, 100);
	m_listviewResult.InsertColumn(7, "LOGTEXT",		LVCFMT_LEFT, 200);

	DWORD dwExStyle;
	dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

	m_listviewResult.SetExtendedStyle(dwExStyle);

	DWORD dwStyle = m_listviewResult.GetStyle();
	dwStyle = dwStyle | LVS_SHOWSELALWAYS;
	::SetWindowLong(m_listviewResult.m_hWnd, GWL_STYLE, dwStyle);



	return TRUE;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int		nTop = 0;

	if(	m_groupboxDetail.m_hWnd != NULL &&
		m_editLogText.m_hWnd	!= NULL &&
		m_listviewResult.m_hWnd	!= NULL)
	{
		CRect	rcGroupBox, rcEdit, rcWin;

		GetClientRect(rcWin);

		m_groupboxDetail.GetWindowRect(rcGroupBox);
		m_editLogText.GetWindowRect(rcEdit);

		m_groupboxDetail.MoveWindow(5, 5, rcWin.Width() - 10, rcGroupBox.Height());
		nTop += 5 + rcGroupBox.Height() + 5;

		m_listviewResult.MoveWindow(5, nTop, rcWin.Width() - 10, rcWin.Height() - rcGroupBox.Height() - rcEdit.Height() - 20);

		m_editLogText.MoveWindow(5, rcWin.bottom - rcEdit.Height() - 5, rcWin.Width() - 10, rcEdit.Height());
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearLog();
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonDelSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  nItem = -1;
	nItem = m_listviewResult.GetNextItem(nItem, LVNI_SELECTED);

	while(nItem != -1)
	{
		m_listviewResult.DeleteItem(nItem);
		m_nRowCount--;

		nItem = m_listviewResult.GetNextItem(-1, LVNI_SELECTED);
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonDelDeselect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  nItem = -1;
	int	 nMaxCount = m_listviewResult.GetItemCount();

	int	 i;
	for (i=nMaxCount-1 ; i >= 0 ; i--)
	{
		if(m_listviewResult.GetItemState(i, LVNI_SELECTED) != LVNI_SELECTED)
		{
			m_listviewResult.DeleteItem(i);
			m_nRowCount--;
		}
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonDetailsearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	// 선택되있는열이 있으면 선택해제한다.
	int nIndex = 0;
	nIndex = m_listviewResult.GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		m_listviewResult.SetItemState(nIndex, 0, LVNI_SELECTED | LVNI_FOCUSED );
		nIndex = m_listviewResult.GetNextItem(nIndex, LVNI_SELECTED);
	}

	int  nItem = -1;
	CString	strText;

	int nSearched = 0;

	for (int i=0;i < m_listviewResult.GetItemCount();i++)
	{
		nItem = m_listviewResult.GetNextItem(nItem, LVNI_ALL);
		for(int j=0; j<8; j++)
		{
			strText = m_listviewResult.GetItemText(nItem, j);
			if(strText.Find(m_strDetailSearchWord, 0) >= 0)
			{
				m_listviewResult.SetItemState(nItem, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
				m_listviewResult.SetFocus();
				nSearched++;
			}
		}
	}

	CString Msg;
	Msg.Format("[ %d ] Items Search", nSearched);
	AfxMessageBox(Msg);
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nRowCount <= 0)			return;

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", this, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		SaveLogFile(fileDlg.GetPathName());
	}
}

//=============================================================================
//  @Name   OnBnClickedButtonSelexport()
//! @author LeeKH						
//! @date   2007-4-26
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonSelexport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_listviewResult.GetItemCount() <= 0)			return;

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", this, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		SaveSelected_LogFile(fileDlg.GetPathName());
	}
}

//=============================================================================
//  @Name   OnBnClickedButtonImport()
//! @author LeeKH						
//! @date   2007-4-26
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CFileDialog fileDlg(TRUE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", this, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		
	}
}

//=============================================================================
//  @Name   OnNMClickListResult()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CString	strLogText;
	if(m_listviewResult.GetSelectedCount() == 1)
	{
		int nItem = m_listviewResult.GetNextItem(-1, LVNI_SELECTED);
		if(nItem >= 0)
		{
			strLogText = m_listviewResult.GetItemText( nItem, 7 );
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LOGTEXT);
			if(pEdit)		pEdit->SetWindowText(strLogText);
		}
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::AddLogData(LPCSTR pszDate, LPCSTR pszServer, LPCSTR pszCommand, LPCSTR pszUser, LPCSTR pszTargetUser, LPCSTR pszItemUnique, LPCSTR pszLogText)
{
	int nItem = 0;

	char	szTemp[32];
	sprintf(szTemp, "%d", m_nRowCount + 1);
	nItem = m_listviewResult.InsertItem(LVIF_TEXT | LVIF_PARAM, m_nRowCount, szTemp, 0, 0, 0, m_nRowCount);
	m_listviewResult.SetItemText(nItem, 1, pszDate);
	m_listviewResult.SetItemText(nItem, 2, pszServer);
	m_listviewResult.SetItemText(nItem, 3, pszCommand);
	m_listviewResult.SetItemText(nItem, 4, pszUser);
	m_listviewResult.SetItemText(nItem, 5, pszTargetUser);
	m_listviewResult.SetItemText(nItem, 6, pszItemUnique);
	m_listviewResult.SetItemText(nItem, 7, pszLogText);

	m_nRowCount++;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL CLogToolResultDlg::Init()
{
	return TRUE;
}


//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::ClearLog()
{
	m_listviewResult.DeleteAllItems();
	m_nRowCount = 0;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pChildResultDlg == NULL)
	{
		m_pChildResultDlg = new CNewResultDlg();
	}
	if(m_pChildResultDlg != NULL && m_pChildResultDlg->m_hWnd == NULL)
	{
		if(m_pChildResultDlg->Create(IDD_NEWRESULT_DIALOG) == FALSE)	return;
	}
	if(m_pChildResultDlg == NULL || m_pChildResultDlg->m_hWnd == NULL)	return;

	m_pChildResultDlg->ShowWindow(SW_SHOW);
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::OnBnClickedButtonSendnewwindow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_listviewResult.GetSelectedCount() <= 0)			return;

	if(m_pChildResultDlg == NULL)
	{
		m_pChildResultDlg = new CNewResultDlg();
	}
	if(m_pChildResultDlg != NULL && m_pChildResultDlg->m_hWnd == NULL)
	{
		if(m_pChildResultDlg->Create(IDD_NEWRESULT_DIALOG) == FALSE)	return;
	}

	if(m_pChildResultDlg == NULL || m_pChildResultDlg->m_hWnd == NULL)	return;

	int  nItem = -1;
	nItem = m_listviewResult.GetNextItem(nItem, LVNI_SELECTED);

	while(nItem != -1)
	{
		CString ItemStr[7];

		ItemStr[0] = m_listviewResult.GetItemText( nItem, 1 );
		ItemStr[1] = m_listviewResult.GetItemText( nItem, 2 );
		ItemStr[2] = m_listviewResult.GetItemText( nItem, 3 );
		ItemStr[3] = m_listviewResult.GetItemText( nItem, 4 );
		ItemStr[4] = m_listviewResult.GetItemText( nItem, 5 );
		ItemStr[5] = m_listviewResult.GetItemText( nItem, 6 );
		ItemStr[6] = m_listviewResult.GetItemText( nItem, 7 );

		m_pChildResultDlg->AddLogData(ItemStr[0], ItemStr[1], ItemStr[2], ItemStr[3], ItemStr[4], ItemStr[5], ItemStr[6]);

		nItem = m_listviewResult.GetNextItem(nItem, LVNI_SELECTED);
	}
	
	m_pChildResultDlg->ShowWindow(SW_SHOW);
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::SaveLogFile(CString kFileName, bool bInfo)
{
	FILE*	fp					= NULL;
	CString	strSaveFileName;
	CString ItemStr[7];
	
	// 저장할 내용이 없으면 리턴한다.
	if(m_nRowCount == 0)	return;

	strSaveFileName = kFileName;

	fp=fopen(strSaveFileName,"wb");
	if(fp != NULL)
	{
		int ItemCount = m_listviewResult.GetItemCount();

		fprintf(fp,"DATE\tSERVER\tCOMMAND\tUSER\tTARGET_USER\tITEM_UNIQUE\tLOGTEXT\n");
		for ( int i = 0; i < ItemCount; ++i )
		{
			ItemStr[0] = m_listviewResult.GetItemText( i, 1 );
			ItemStr[1] = m_listviewResult.GetItemText( i, 2 );
			ItemStr[2] = m_listviewResult.GetItemText( i, 3 );
			ItemStr[3] = m_listviewResult.GetItemText( i, 4 );
			ItemStr[4] = m_listviewResult.GetItemText( i, 5 );
			ItemStr[5] = m_listviewResult.GetItemText( i, 6 );
			ItemStr[6] = m_listviewResult.GetItemText( i, 7 );

			fprintf(fp, "\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\r\n",ItemStr[0], ItemStr[1], ItemStr[2],
									ItemStr[3], ItemStr[4], ItemStr[5], ItemStr[6]);
		}
		fclose(fp);

		if(bInfo)	AfxMessageBox( kFileName += " Saved..." );
	}
	else
	{
		AfxMessageBox( "Can't Save !!!" );
	}
}

//=============================================================================
//  @Name   SaveSelected_LogFile()
//! @author LeeKH						
//! @date   2007-4-26
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CLogToolResultDlg::SaveSelected_LogFile(CString kFileName, bool bInfo)
{
	FILE*	fp					= NULL;
	CString	strSaveFileName;
	CString ItemStr[7];

	// 저장할 내용이 없으면 리턴한다.
	if(m_nRowCount <= 0)	return;

	// 선택된 내용이 없으면 리턴한다.
	if(m_listviewResult.GetItemCount() <= 0)	return;

	strSaveFileName = kFileName;

	fp=fopen(strSaveFileName,"w");
	if(fp != NULL)
	{
		int ItemCount = m_listviewResult.GetItemCount();

		fprintf(fp,"DATE\tSERVER\tCOMMAND\tUSER\tTARGET_USER\tITEM_UNIQUE\tLOGTEXT\n");

		int  nItem = -1;
		nItem = m_listviewResult.GetNextItem(nItem, LVNI_SELECTED);

		while(nItem != -1)
		{
			CString ItemStr[7];

			ItemStr[0] = m_listviewResult.GetItemText( nItem, 1 );
			ItemStr[1] = m_listviewResult.GetItemText( nItem, 2 );
			ItemStr[2] = m_listviewResult.GetItemText( nItem, 3 );
			ItemStr[3] = m_listviewResult.GetItemText( nItem, 4 );
			ItemStr[4] = m_listviewResult.GetItemText( nItem, 5 );
			ItemStr[5] = m_listviewResult.GetItemText( nItem, 6 );
			ItemStr[6] = m_listviewResult.GetItemText( nItem, 7 );

			fprintf(fp, "\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\t"
						"\"%s\"\n",ItemStr[0], ItemStr[1], ItemStr[2],
						ItemStr[3], ItemStr[4], ItemStr[5], ItemStr[6]);

			nItem = m_listviewResult.GetNextItem(nItem, LVNI_SELECTED);
		}
		fclose(fp);

		if(bInfo)	AfxMessageBox( kFileName += " Saved..." );
	}
	else
	{
		AfxMessageBox( "Can't Save !!!" );
	}
}

// Sort the item in reverse alphabetical order.
int CLogToolResultDlg::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

void CLogToolResultDlg::OnHdnItemclickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    
	g_CurrentSort = g_SortInfo[phdr->iItem];
	g_SortInfo[phdr->iItem].nSort = !g_SortInfo[phdr->iItem].nSort;

	m_listviewResult.SortItems(MyCompareProc, (DWORD_PTR)&m_listviewResult);
	for(int i=0; i<m_nRowCount; i++)
	{
		m_listviewResult.SetItem(i, 0, LVIF_PARAM, NULL, 0, 0, 0, i, 0);
	}
		
	*pResult = 0;
}
