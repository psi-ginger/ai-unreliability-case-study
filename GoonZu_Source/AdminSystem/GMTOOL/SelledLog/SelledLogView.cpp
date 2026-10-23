// NewLog\NewLogView.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "SelledLogView.h"
#include "SelledLogFrame.h"
#include ".\SelledLogview.h"

// CNewLogView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelledLogView, CDialog)
CSelledLogView::CSelledLogView(CWnd* pParent /*=NULL*/)
: CDialog(CSelledLogView::IDD, pParent)
{
	m_nRowCount			= 0;
	m_nSortColumn		= 0;
}

CSelledLogView::~CSelledLogView()
{
}

void CSelledLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelledLogView, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearchNew)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStopNew)
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListResultNew)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, OnNMClickListResultNew)
	ON_BN_CLICKED(IDC_BUTTON_SEL_DELETE, OnBnClickedButtonSelDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_DESEL_DELETE, OnBnClickedButtonDeselDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DELETE, OnBnClickedButtonAllDeleteNew)
	ON_BN_CLICKED(IDC_BUTTON_SEL_EXPORT, OnBnClickedButtonSelExportNew)
	ON_BN_CLICKED(IDC_BUTTON_ALL_EXPORT, OnBnClickedButtonAllExportNew)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHINRESULT, OnBnClickedButtonSearchinresultNew)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT, OnBnClickedButtonUnSelectAll)
END_MESSAGE_MAP()

// CNewLogView 메시지 처리기입니다.

BOOL CSelledLogView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	NGameDataManager* pkGameDataManager = &theApp.m_kDataManager;
	if(pkGameDataManager == NULL)				return FALSE;

	CDateTimeCtrl* pkDateTime_StartDate	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTTIME);
	CDateTimeCtrl* pkDateTime_EndDate	= (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDTIME);

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

	// 리스트 뷰 설정
	CListCtrl* pkListCtrl_Result = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result != NULL)
	{
		DWORD dwExStyle;
		dwExStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		pkListCtrl_Result->SetExtendedStyle(dwExStyle);

		DWORD dwStyle = pkListCtrl_Result->GetStyle();
		dwStyle = dwStyle | LVS_SHOWSELALWAYS;
		::SetWindowLong(pkListCtrl_Result->m_hWnd, GWL_STYLE, dwStyle);
	}

	if(pkListCtrl_Result != NULL)
	{
		int nCol = 0;
		pkListCtrl_Result->InsertColumn(nCol++, "SendFlag", LVCFMT_LEFT, 60);
		pkListCtrl_Result->InsertColumn(nCol++, "CancelFlag", LVCFMT_LEFT, 70);
		pkListCtrl_Result->InsertColumn(nCol++, "ItemAcceptFlag", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyServerNation", LVCFMT_LEFT, 100);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyServerIndex", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyCharNation", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyUserID", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyCharName", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyPersonID", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "SellServerNation", LVCFMT_LEFT, 100);
		pkListCtrl_Result->InsertColumn(nCol++, "SellServerIndex", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "SellCharNation", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "SellUserID", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "SellCharName", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "SellPersonID", LVCFMT_LEFT, 90);
		pkListCtrl_Result->InsertColumn(nCol++, "ItemUnique", LVCFMT_LEFT, 70);
		pkListCtrl_Result->InsertColumn(nCol++, "ItemNum", LVCFMT_LEFT, 70);
		pkListCtrl_Result->InsertColumn(nCol++, "Price", LVCFMT_LEFT, 60);
		pkListCtrl_Result->InsertColumn(nCol++, "SellDate", LVCFMT_LEFT, 140);
		pkListCtrl_Result->InsertColumn(nCol++, "BuyDate", LVCFMT_LEFT, 140);
	}

	// 서버 코드 설정
	CComboBox* pkCombo_SvrCode = (CComboBox*)GetDlgItem(IDC_COMBO_SERVERCODE);
	if( pkCombo_SvrCode )
	{
		pkCombo_SvrCode->AddString("all");
		pkCombo_SvrCode->AddString("kor");
		pkCombo_SvrCode->AddString("jpn");
		pkCombo_SvrCode->AddString("eng");

		pkCombo_SvrCode->SetCurSel( 0 );
	}

	CComboBox* pkCombo_SvrIndex = (CComboBox*)GetDlgItem(IDC_COMBO_SERVERINDEX);
	if( pkCombo_SvrIndex )
	{
		pkCombo_SvrIndex->AddString("all");
		pkCombo_SvrIndex->AddString("1");
		pkCombo_SvrIndex->AddString("2");
		pkCombo_SvrIndex->AddString("3");
		pkCombo_SvrIndex->AddString("4");

		pkCombo_SvrIndex->SetCurSel( 0 );
	}

	CComboBox* pkCombo_Command = (CComboBox*)GetDlgItem(IDC_COMBO_COMMAND);
	if( pkCombo_Command )
	{
		pkCombo_Command->AddString("selled");
		pkCombo_Command->AddString("canceled");
		pkCombo_Command->AddString("error");

		pkCombo_Command->SetCurSel( 0 );
	}

	CButton*	pkButton_AutoClear = (CButton*)GetDlgItem(IDC_CHECK_AUTOCLEAR);
	if(pkButton_AutoClear)	pkButton_AutoClear->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSelledLogView::OnSize(UINT nType, int cx, int cy)
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

BOOL CSelledLogView::PreTranslateMessage(MSG* pMsg)
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

void CSelledLogView::OnBnClickedButtonSearchNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// START DATETIME, END DATETIME
	CDateTimeCtrl*	pkDateTime_StartDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_STARTTIME);
	CDateTimeCtrl*	pkDateTime_EndDate = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIME_ENDTIME);

	CTime	StartTime, EndTime;
	pkDateTime_StartDate->GetTime(StartTime);
	pkDateTime_EndDate->GetTime(EndTime);

	if(EndTime < StartTime)
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

		CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
		if(pkSelledLogFrame != NULL)
		{
			pkSelledLogFrame->RemoveAllData();
		}
	}

	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame != NULL)		pkSelledLogFrame->m_siSearchCount = 0;

	RequestLog();
}

BOOL CSelledLogView::RequestLog()
{
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return FALSE;

	sDBRequest_SelledItemLog	SelledLog;

	CString		strItem;

	GetDlgItemText(IDC_COMBO_BUYSERVERCODE, strItem);
	if( strItem == "all" )
	{		strcpy(SelledLog.szBuyServerNation, "");		}
	else
	{		strcpy(SelledLog.szBuyServerNation, strItem);	}

	GetDlgItemText(IDC_COMBO_BUYSERVERINDEX, strItem);
	if( strItem == "all" )
	{		SelledLog.siBuyServerIndex = 0;					}
	else
	{		SelledLog.siBuyServerIndex = atoi(strItem);		}

	GetDlgItemText(IDC_COMBO_SELLSERVERCODE, strItem);
	if( strItem == "all" )
	{		strcpy(SelledLog.szSellServerNation, "");		}
	else
	{		strcpy(SelledLog.szSellServerNation, strItem);	}

	GetDlgItemText(IDC_COMBO_SELLSERVERINDEX, strItem);
	if( strItem == "all" )
	{		SelledLog.siSellServerIndex = 0;					}
	else
	{		SelledLog.siSellServerIndex = atoi(strItem);		}

	GetDlgItemText(IDC_EDIT_BUYCHARID, strItem);
	strcpy(SelledLog.szBuyID, strItem);

	GetDlgItemText(IDC_EDIT_BUYCHARNAME, strItem);
	strcpy(SelledLog.szBuyCharName, strItem);

	GetDlgItemText(IDC_EDIT_SELLCHARID, strItem);
	strcpy(SelledLog.szSellID, strItem);

	GetDlgItemText(IDC_EDIT_SELLCHARNAME, strItem);
	strcpy(SelledLog.szSellCharName, strItem);

	GetDlgItemText(IDC_EDIT_ITEMUNIQUE, strItem);
	SelledLog.siItemUnique = atoi(strItem);

	GetDlgItemText(IDC_DATETIME_STARTTIME, strItem);
	strcpy(SelledLog.szStartDate, strItem);

	GetDlgItemText(IDC_DATETIME_ENDTIME, strItem);
	strcpy(SelledLog.szEndDate, strItem);

	GetDlgItemText(IDC_COMBO_COMMAND, strItem);
	if( strItem == "selled" )
	{		SelledLog.siCancelFlag = 0;		SelledLog.siAcceptFlag = 1;		SelledLog.siSendFlag = 1;		}
	else if( strItem == "canceled" )
	{		SelledLog.siCancelFlag = 1;		SelledLog.siAcceptFlag = 0;		SelledLog.siSendFlag = 0;		}
	else if( strItem == "error" )
	{		SelledLog.siCancelFlag = 0;		SelledLog.siAcceptFlag = 0;		SelledLog.siSendFlag = 1;		}

	pkSelledLogFrame->RequestLogData(&SelledLog);

	return TRUE;
}

void CSelledLogView::OnBnClickedButtonStopNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	pkSelledLogFrame->Stop();
}

//=================================================================================
//pkListCtrl_Result->InsertColumn(nCol++, "SendFlag", LVCFMT_LEFT, 80);
//pkListCtrl_Result->InsertColumn(nCol++, "CancelFlag", LVCFMT_LEFT, 80);
//pkListCtrl_Result->InsertColumn(nCol++, "ItemAcceptFlag", LVCFMT_LEFT, 80);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyServerNation", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyServerIndex", LVCFMT_LEFT, 150);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyCharNation", LVCFMT_LEFT, 150);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyUserID", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyCharName", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyPersonID", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "SellServerNation", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "SellServerIndex", LVCFMT_LEFT, 150);
//pkListCtrl_Result->InsertColumn(nCol++, "SellCharNation", LVCFMT_LEFT, 150);
//pkListCtrl_Result->InsertColumn(nCol++, "SellUserID", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "SellCharName", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "SellPersonID", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "ItemUnique", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "ItemNum", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "Price", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "SellDate", LVCFMT_LEFT, 100);
//pkListCtrl_Result->InsertColumn(nCol++, "BuyDate", LVCFMT_LEFT, 100);
//=================================================================================

void CSelledLogView::AddLogData(stLOGData_SelledItem* pkData)
{
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;
	
	CString strItem;
	int nItem = 0;	
	int nSubItem = 0;

	strItem.Format("%d", pkData->siSendFlag);
	nItem = pkListCtrl_Result_New->InsertItem(LVIF_TEXT, m_nRowCount, strItem, 0, 0, 0, 0);

	m_nRowCount++;

	strItem.Format("%d", pkData->siCancelFlag);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siItemAcceptFlag);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szBuyServerNation);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siBuyServerIndex);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siBuyCharNation);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szBuyUserID);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szBuyCharName);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siBuyPersonID);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szSellServerNation);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siSellServerIndex);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siSellCharNation);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szSellUserID);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%s", pkData->szSellCharName);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siSellPersonID);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siItemUnique);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%d", pkData->siItemNum);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%I64d", pkData->siPrice);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%04d-%02d-%02d %02d:%02d:%02d", pkData->stSellDate.wYear, pkData->stSellDate.wMonth, pkData->stSellDate.wDay, pkData->stSellDate.wHour, pkData->stSellDate.wMinute, pkData->stSellDate.wSecond);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);

	strItem.Format("%04d-%02d-%02d %02d:%02d:%02d", pkData->stBuyDate.wYear, pkData->stBuyDate.wMonth, pkData->stBuyDate.wDay, pkData->stBuyDate.wHour, pkData->stBuyDate.wMinute, pkData->stBuyDate.wSecond);
	pkListCtrl_Result_New->SetItem(nItem, ++nSubItem, LVIF_TEXT, strItem, 0, 0, 0, 0);
}

void CSelledLogView::OnHdnItemclickListResultNew(NMHDR *pNMHDR, LRESULT *pResult)
{
	return;

	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CSelledLogFrame* pkLogFrame = (CSelledLogFrame*)GetParent();
	if(pkLogFrame == NULL)					return;
	// 검색중이라면 명령이 실행되지 않는다.
	if(pkLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	m_nSortColumn = phdr->iItem;

	pkListCtrl_Result_New->SortItems(MyCompareProc, (DWORD_PTR)this);
	for(int i=0; i<m_nRowCount; i++)
	{
		pkListCtrl_Result_New->SetItem(i, 0, LVIF_PARAM, NULL, 0, 0, 0, i, 0);
	}

	*pResult = 0;
}

// Sort the item in reverse alphabetical order.
int CSelledLogView::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	CSelledLogView* pkLogView = (CSelledLogView*) lParamSort;

	CListCtrl* pListCtrl = (CListCtrl*)pkLogView->GetDlgItem(IDC_LIST_RESULT);

	CString    strItem1 = pListCtrl->GetItemText((int)lParam1, pkLogView->m_nSortColumn);
	CString    strItem2 = pListCtrl->GetItemText((int)lParam2, pkLogView->m_nSortColumn);

	int nRet = 0;
	nRet = strcmp(strItem2, strItem1);

	return nRet;
}

void CSelledLogView::OnNMClickListResultNew(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO	HitTestInfo;
	HitTestInfo.pt		= pNMListView->ptAction;

	pkListCtrl_Result_New->SubItemHitTest(&HitTestInfo);

	if(HitTestInfo.iItem >= 0 && HitTestInfo.iSubItem >= 0)
	{
		CString strText = pkListCtrl_Result_New->GetItemText(HitTestInfo.iItem, HitTestInfo.iSubItem);
		SetDlgItemText(IDC_EDIT_CLICKEDITEM, strText);

		CString		strDescription;
		strDescription = "";
	}
}

void CSelledLogView::OnBnClickedButtonSelDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkSelledLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	int  nItem = -1;
	nItem = pkListCtrl_Result_New->GetNextItem(nItem, LVNI_SELECTED);

	while(nItem != -1)
	{
		pkListCtrl_Result_New->DeleteItem(nItem);
		m_nRowCount--;

		nItem = pkListCtrl_Result_New->GetNextItem(-1, LVNI_SELECTED);
	}
}

void CSelledLogView::OnBnClickedButtonDeselDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkSelledLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
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

void CSelledLogView::OnBnClickedButtonAllDeleteNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkSelledLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	pkListCtrl_Result_New->DeleteAllItems();
	m_nRowCount = 0;

	pkSelledLogFrame->RemoveAllData();
}

void CSelledLogView::OnBnClickedButtonSelExportNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkSelledLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	if(pkListCtrl_Result_New->GetSelectedCount() <= 0)
	{
		AfxMessageBox("No Selecteion!!");
		return;
	}

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkSelledLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkSelledLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_SELECTED);
	}
}

void CSelledLogView::OnBnClickedButtonAllExportNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSelledLogFrame* pkSelledLogFrame = (CSelledLogFrame*)GetParent();
	if(pkSelledLogFrame == NULL)				return;

	// 검색중이라면 명령이 실행되지 않는다.
	if(pkSelledLogFrame->m_bSearching == TRUE)	return;

	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	if(pkListCtrl_Result_New->GetItemCount() <= 0)
	{
		AfxMessageBox("No Data!!");
		return;
	}

	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", pkSelledLogFrame, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		pkSelledLogFrame->SaveLogFile(fileDlg.GetPathName(), LVNI_ALL);
	}
}

void CSelledLogView::OnBnClickedButtonSearchinresultNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택되있는열이 있으면 선택해제한다.
	CListCtrl* pkListCtrl_Result_New = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	if(pkListCtrl_Result_New == NULL)			return;

	CString	strSearchWord;
	CEdit* pkEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEARCHINRESULT);
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
		//for(int j=1; j<m_ActiveListViewColumnCount; j++)
		//{
		//	strText = pkListCtrl_Result_New->GetItemText(i, j);
		//	if(strText.Find(strSearchWord, 0) >= 0)
		//	{
		//		pkListCtrl_Result_New->SetItemState(i, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
		//		pkListCtrl_Result_New->SetFocus();
		//		nSearched++;
		//	}
		//}
	}

	CString Msg;
	Msg.Format("[ %d ] Items Search", nSearched);
	AfxMessageBox(Msg);
}

void CSelledLogView::OnBnClickedButtonSelectAll()
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

void CSelledLogView::OnBnClickedButtonUnSelectAll()
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
