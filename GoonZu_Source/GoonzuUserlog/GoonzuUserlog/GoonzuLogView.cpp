// GoonzuLogView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GoonzuUserlog.h"
#include "GoonzuLogView.h"
#include ".\goonzulogview.h"
#include "ItemExplain.h"
#include "ODBC/RichODBC.h" 
#include "../../GoonZuWorld/common/Item/ItemCommon/cltItem.h"
#include "GoonzuUserlogDlg.h"
#include "VillageManager/VillageManager.h"
#include "LogUnit.h"

// CGoonzuLogView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGoonzuLogView, CDialog)
CGoonzuLogView::CGoonzuLogView(CWnd* pParent /*=NULL*/)
	: CDialog(CGoonzuLogView::IDD, pParent)
	, m_subitem(0)
	, m_ChildProcess(NULL)
	, m_siMaxRowcount(0)
	, m_siCurRow(0)
	, m_iSortColumn(-1)
	, m_bSortAscending(TRUE)
	, m_bExit(FALSE)
{
	m_pItemManager = new cltItemManagerCommon();
	m_pItemManager->Create();
	m_pItemExplain = new CItemExplain();
	m_pItemExplain->Create( IDD_DIALOG2, this);
	m_pItemExplain->ShowWindow(SW_HIDE);

	m_ChildHwnd = NULL;
	m_hLoadThread = NULL;
	m_hQueryThread = NULL;

	// 실행경로를 얻는다.
	GetModuleFileName(NULL, m_strAppFullPath, sizeof(m_strAppFullPath));

	// Accelerator를 로드한다.
	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_USERLOG_ACCEL));
}

CGoonzuLogView::~CGoonzuLogView()
{
	if ( m_pItemManager )
	{
		delete m_pItemManager;
		m_pItemManager = NULL;
	}

	if(m_pItemExplain)
	{
		delete m_pItemExplain;
		m_pItemExplain= NULL;
	}

	if(m_ChildProcess != NULL)
		CloseHandle(m_ChildProcess);

	if(m_hQueryThread) CloseHandle(m_hQueryThread);
	if(m_hLoadThread) CloseHandle(m_hLoadThread);
}

void CGoonzuLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO8, m_SearchServer);
	DDX_Control(pDX, IDC_EDIT2, m_UserEdit);
	DDX_Control(pDX, IDC_EDIT3, m_ItemEdit);
	DDX_Control(pDX, IDC_BUTTON3, m_SearchButton3);
	DDX_Control(pDX, IDC_LIST2, m_ListView);
	DDX_Control(pDX, IDC_COMBO9, m_SearchItem);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_EDIT6, m_ViewEdit);
	DDX_Control(pDX, IDC_EDIT1, m_SearchItemEdit);
	DDX_Control(pDX, IDC_EDIT4, m_BinaryEdit);
	DDX_Control(pDX, IDC_EDIT5, m_UniqueEdit);
	DDX_Control(pDX, IDC_BUTTON_SAVEFILE, m_ButtonSaveFile);
	DDX_Control(pDX, IDC_EDIT_ROWCOUNT, m_RowcountEdit);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_SearchEdit);
	DDX_Control(pDX, IDC_EDIT_USER_SEARCH, m_UserSearchEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_DTDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STARTTIME, m_DTStartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ENDTIME, m_DTEndTime);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonGetUnique);
}


void CGoonzuLogView::SetCombo()
{
	COleDateTime time = COleDateTime::GetCurrentTime();
	
	switch (m_siServerSelect)
	{
	case NDOORS:
		m_SearchServer.AddString("태조");
		m_SearchServer.AddString("정종");
		m_SearchServer.AddString("태종");
		m_SearchServer.AddString("세종");
		m_SearchServer.AddString("문종");
		m_SearchServer.AddString("단종");
		m_SearchServer.AddString("세조");
		m_SearchServer.AddString("예종");
		m_SearchServer.AddString("성종");
		m_SearchServer.AddString("연산군");
		break;
	case NETMARBLE:
		m_SearchServer.AddString("백두");
		m_SearchServer.AddString("한라");
		m_SearchServer.AddString("금강");
		break;
	}
	m_SearchServer.SetCurSel(0);

	m_SearchItem.AddString("마을");
	m_SearchItem.AddString("아이템");
	m_SearchItem.SetCurSel(0);

	m_DTDate.SetTime(time);
	m_DTStartTime.SetTime(time);
	m_DTEndTime.SetTime(time);
}

void CGoonzuLogView::SetListView()
{
	
	char* cListHeader[]={"번호","날짜","명령","행동자","피동자","아이템유니크","세부정보"};
	int nHeaderWidth[]={56,138,155,135,135,178,0};
	
	LV_COLUMN lvColomn;
	lvColomn.mask =LVCF_FMT |  LVCF_TEXT | LVCF_WIDTH;
	
	
	for(int i=0; i<7;i++)
	{
		m_ListView.InsertColumn(i, cListHeader[i]);
		m_ListView.SetColumnWidth(i, nHeaderWidth[i]);
		m_subitem++;
	}
}

void CGoonzuLogView::SetFilterBox()
{
	InsertFilter(0, "전체");
	InsertFilter(1, "아이템몰거래");
	InsertFilter(2, "역참");
	InsertFilter(3, "로긴/로그아웃");
	InsertFilter(4, "개인거래");
	InsertFilter(5, "소환수거래");

	m_ListView.SetExtendedStyle
		(m_ListView.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	m_ListBox.SetCurSel(0);
}

void CGoonzuLogView::ShowUserLog()
{
	SetFilterBox();
	SetCombo();
	SetListView();
}

BEGIN_MESSAGE_MAP(CGoonzuLogView, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnLvnItemchangedList2)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_EN_CHANGE(IDC_EDIT6, OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECTED_DELETE, OnBnClickedButtonUnselectedDelete)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SENDCHILDWINDOW, OnBnClickedButtonSendchildwindow)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnNMDblclkList2)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_SELECTED_DELETE, OnBnClickedButtonSelectedDelete)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST2, OnLvnKeydownList2)
	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT4, OnEnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, OnEnSetfocusEdit5)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH, OnEnSetfocusEditSearch)
	ON_EN_SETFOCUS(IDC_EDIT2, OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, OnEnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT_USER_SEARCH, OnEnSetfocusEditUserSearch)
	ON_LBN_SETFOCUS(IDC_LIST1, OnLbnSetfocusList1)
	ON_COMMAND(ID_EXIT_APP, OnExitApp)
	ON_COMMAND(ID_OPEN_FILE, OnOpenFile)
	ON_COMMAND(ID_SAVE_FILE, OnSaveFile)
	ON_COMMAND(ID_SEND_DATA, OnSendData)
	ON_COMMAND(ID_DELETE_SELECTED, OnDeleteSelected)
	ON_COMMAND(ID_DELETE_UNSELECTED, OnDeleteUnselected)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DATEVARY, OnBnClickedButtonDatevary)
	ON_EN_SETFOCUS(IDC_EDIT_DATEVARY, OnEnSetfocusEditDatevary)
END_MESSAGE_MAP()


// CGoonzuLogView 메시지 처리기입니다.

void CGoonzuLogView::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	unsigned uThreadId;

	m_hQueryThread = (HANDLE)_beginthreadex(NULL, 0, QueryLogData, this, 0, &uThreadId);
	if(m_hQueryThread == NULL)
	{
		AfxMessageBox("CreateThread실패");
	}
}

unsigned __stdcall CGoonzuLogView::QueryLogData(LPVOID lpParam)
{
	CGoonzuLogView *pLogView = (CGoonzuLogView*)lpParam;

	CLogUnit LogUnit;
	BOOL bExecute = FALSE;
	CString szAutoSave;

	pLogView->m_ListView.DeleteAllItems();
	pLogView->m_ViewEdit.Clear();

	//--------------------------------------
	// 입력용 변수 설정..
	//--------------------------------------
	int siLogType = 0;
	char szServerName[20] = "";
	char szPersonName[20] = "";
	char szUserSearch[MAX_SEARCH_LEN] = "";
	char text_itemunique[20]="";
	int siItemUnique = 0;
	char szStartDate[24] = "";
	char szEndDate[24] = "";
	CString szRowcount;

	COleDateTime SearchDate, StartTime, EndTime, CurrTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan TimeSpan;

	// 입력된 시간을 가져온다.
	pLogView->m_DTDate.GetTime(SearchDate);
	pLogView->m_DTStartTime.GetTime(StartTime);
	pLogView->m_DTEndTime.GetTime(EndTime);

	// 시작시간과 종료시간에 검색일을 설정한다.
	StartTime.SetDateTime(SearchDate.GetYear(), SearchDate.GetMonth(), SearchDate.GetDay(), StartTime.GetHour(), StartTime.GetMinute(), 0);
	EndTime.SetDateTime(SearchDate.GetYear(), SearchDate.GetMonth(), SearchDate.GetDay(), EndTime.GetHour(), EndTime.GetMinute(), 0);

	strcpy(szStartDate, StartTime.Format("%Y-%m-%d %H:%M:00.000"));
	strcpy(szEndDate, EndTime.Format("%Y-%m-%d %H:%M:59.999"));

	// 세부 검색옵션을 가져온다.
	siLogType=pLogView->m_ListBox.GetCaretIndex();
	pLogView->m_UserEdit.GetWindowText(szPersonName,20);
	pLogView->m_UserSearchEdit.GetWindowText( szUserSearch, MAX_SEARCH_LEN);
	pLogView->m_SearchServer.GetWindowText(szServerName,20);
	pLogView->m_ItemEdit.GetWindowText(text_itemunique,20);

	if(pLogView->IsNumber(text_itemunique) == true)
		siItemUnique = atoi(text_itemunique);
	else
		siItemUnique = pLogView->m_pItemManager->FindUniqueFromName(text_itemunique);

	// 서버설정이 잘못되었다면 리턴.
	if(pLogView->m_SearchServer.GetWindowTextLength() == 0)
		return 0;

	pLogView->DisableButton();

	// AutoSave 파일명을 만든다.
	szAutoSave.Format("%s_%s_%d_%d_%s_%s_%s.txt"
		, szServerName, szPersonName, siItemUnique, siLogType, StartTime.Format("%Y%m%d%H%M"), EndTime.Format("%Y%m%d%H%M")
		, szUserSearch);

	// 파일명으로 쓸 수 없는 문자를 변경한다.
	szAutoSave.Replace("\\", "-"); szAutoSave.Replace("/", "-"); szAutoSave.Replace(":", "-"); szAutoSave.Replace("*", "-"); szAutoSave.Replace("<", "-"); szAutoSave.Replace(">", "-"); szAutoSave.Replace("|", "-");

	if(pLogView->LoadLogFile(szAutoSave))
		bExecute = FALSE;
	else
		bExecute = TRUE;

	if(bExecute)
	{
		// 인스턴스 생성
		RichODBC *pDB = new RichODBC;
		// DB 접속
		
		switch (pLogView->m_siServerSelect)
		{
		case NDOORS:
			pDB->Connect("SQL Server", SERVERIP, "RichLog", pLogView->m_username, pLogView->m_password);
			break;
		case NETMARBLE:
			pDB->Connect("SQL Server", NETMARBLESERVERIP, "RichLog", pLogView->m_username, pLogView->m_password);
			break;
		}

		// 쿼리 준비..
		pDB->SetQuery("{call rsp_PersonalLog3 (?, ?, ?, ?, ?, ?, ?)}");

		// 입력 파라메타값 설정.

		pDB->SetParam( 1, SQL_INTEGER, &siLogType, sizeof(siLogType) );
		pDB->SetParam( 2, SQL_VARCHAR, &szServerName, sizeof(szServerName) );
		pDB->SetParam( 3, SQL_VARCHAR, &szPersonName, sizeof(szPersonName) );
		pDB->SetParam( 4, SQL_INTEGER, &siItemUnique, sizeof(siItemUnique) );
		pDB->SetParam( 5, SQL_VARCHAR, &szStartDate, sizeof(szStartDate) );
		pDB->SetParam( 6, SQL_VARCHAR, &szEndDate, sizeof(szEndDate) );
		pDB->SetParam( 7, SQL_VARCHAR, &szUserSearch, sizeof(szUserSearch) );

		// 쿼리 실행
		pDB->ExecSQL();

		// 결과 구하기.
		pLogView->m_siMaxRowcount=0;

		while(pDB->FetchData()>0)
		{
			// 로딩중 프로그램을 종료시키려하면 루프를 빠져나간다.
			if(pLogView->m_bExit == TRUE)
				break;

			char szItemUnique[10] = "";

			pDB->GetData( 1, LogUnit.szDate, sizeof(LogUnit.szDate) );
			pDB->GetData( 2, LogUnit.szCommand, sizeof(LogUnit.szCommand) );
			pDB->GetData( 3, LogUnit.szRetPersonName, sizeof(LogUnit.szRetPersonName) );
			pDB->GetData( 4, LogUnit.szDstPersonName, sizeof(LogUnit.szDstPersonName) );
			pDB->GetData( 5, szItemUnique, sizeof(szItemUnique) );
			pDB->GetData( 6, LogUnit.szLogTxt, sizeof(LogUnit.szLogTxt) );

			sprintf(LogUnit.szItemUnique,"%s(%s)", szItemUnique, pLogView->m_pItemManager->GetName(atoi(szItemUnique)));

			pLogView->InsertLogUnit(&LogUnit);

			if(pLogView->m_siMaxRowcount % 100 == 0)
			{
				szRowcount.Format("검색중 : %d", pLogView->m_siMaxRowcount);
				pLogView->m_SearchButton3.SetWindowText(szRowcount);
			}
			//printf("%s, %s, %s, %s, %d, %s\n", szDate, szCommand, szRetPersonName, szDstPersonName, siRetItemUnique, szLogTxt);
		}

		// DB 접속종료..
		pDB->Disconnect();

		// 오토세이브. EndTime이 현재시간과 20분 이상 차이나야 저장한다(클라이언트시간과 로그서버시간이 다를 수 있으므로 최근시간일경우 잘못저장 될 수 있음).
		if(pLogView->m_bExit == FALSE)
		{
			TimeSpan = CurrTime - EndTime;
			if(TimeSpan.GetTotalMinutes() >= 20 && pLogView->m_siMaxRowcount > 0)
			{

				pLogView->SaveLogFile(szAutoSave, false);
			}
		}
	}

	pLogView->EnableButton();
	pLogView->ShowRowcount();
	return 0;
}

void CGoonzuLogView::ShowRowcount()
{
	CString szCount;
	szCount.Format("%d/%d", m_siCurRow, m_siMaxRowcount);
	m_RowcountEdit.SetWindowText(szCount);
}

void CGoonzuLogView::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( pNMLV->iItem >= 0 && ( (pNMLV->uNewState == 3 && pNMLV->uOldState == 0) || (pNMLV->uNewState == 1 && pNMLV->uOldState == 0) ) )
	{
		int nIndex = pNMLV->iItem;
		CString szLog = m_ListView.GetItemText(nIndex, 6);
		CString szItemUnique = m_ListView.GetItemText(nIndex, 5);

		// 세부항목내용을 에디트뷰에 따로 표시한다.
		m_ViewEdit.SetWindowText(szLog);

		// 아이템유니크값을 자동으로 넣어준다.
		m_UniqueEdit.SetWindowText(szItemUnique.Left(szItemUnique.Find("(")));

		// 아이템정보값을 자동으로 넣어준다.
		int nFirst = szLog.Find("0x");
		if( nFirst >= 0 )
			m_BinaryEdit.SetWindowText(szLog.Mid(nFirst, 26));
		else
			m_BinaryEdit.SetWindowText("");

        m_siCurRow = nIndex+1;
		ShowRowcount();

		// 아이템정보창이 떠있으면 업데이트를 한다.
		if(m_pItemExplain->IsWindowVisible())
		{
            ShowItemInfo();
		}
	}

	*pResult = 0;
}



void CGoonzuLogView::SetAccount(char* UserID,char * PassWord )
{
	strcpy(m_username,UserID);
	strcpy(m_password,PassWord);

}
void CGoonzuLogView::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGoonzuLogView::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGoonzuLogView::OnBnClickedButton4()
{
	CFileDialog fileDlg(FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", NULL, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		SaveLogFile(fileDlg.GetPathName());
	}
}

void CGoonzuLogView::SaveLogFile(CString szFileName, bool bInfo)
{
	FILE *fp;
	char filename[MAX_PATH]="";
	char data1[128]="";
	char data2[128]="";
	char data3[128]="";
	char data4[128]="";
	char data5[128]="";
	char data6[128]="";

	// 저장할 내용이 없으면 리턴한다.
	if(m_siMaxRowcount == 0)
		return;

	strncpy(filename, szFileName, szFileName.GetLength());

	fp=fopen(filename,"w+t");
	if(fp != NULL)
	{
		int ItemCount = m_ListView.GetItemCount();

		CString ItemStr[6];

		fprintf(fp,"날자\t명령\t행동자\t피동자\t아이템유니크\t세부정보\n");
		for ( int i = 0; i < ItemCount; ++i )
		{
			ItemStr[0] = m_ListView.GetItemText( i, 1 );
			ItemStr[1] = m_ListView.GetItemText( i, 2 );
			ItemStr[2] = m_ListView.GetItemText( i, 3 );
			ItemStr[3] = m_ListView.GetItemText( i, 4 );
			ItemStr[4] = m_ListView.GetItemText( i, 5 );
			ItemStr[5] = m_ListView.GetItemText( i, 6 );
			fprintf(fp,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\n",ItemStr[0],ItemStr[1],ItemStr[2],ItemStr[3],ItemStr[4],ItemStr[5]);
		}
		fclose(fp);

		if(bInfo)
			AfxMessageBox( szFileName += "에 저장되었습니다" );
	}
	else
	{
		AfxMessageBox( "저장 할 수 없습니다." );
	}
}


void CGoonzuLogView::OnBnClickedButton1()
{
	char itemname[64]="";
	char itemedit[64]="";
	
	m_SearchItem.GetWindowText(itemname,40);
	m_SearchItemEdit.GetWindowText(itemedit,40);
	if(strcmp(itemname,"마을")==0)
	{
        CVillageManager *village= new CVillageManager();

		m_SearchItemEdit.GetWindowText(itemedit,40);		
		GetDlgItem(IDC_EDIT_VIEWNAMECHANGE)->SetWindowText(village->Set(itemedit));
		if(village)
		{
			delete village;
			village=NULL;
		}
	}
	else
	{
		CString szItemUnique;
		m_SearchItemEdit.GetWindowText(itemedit,40);
		if(IsNumber(itemedit) == true)
		{
			int unique = atoi(itemedit);
			szItemUnique.Format("%s", m_pItemManager->GetName(unique));
		}
		else
		{
			szItemUnique.Format("%d", m_pItemManager->FindUniqueFromName(itemedit));
		}
		GetDlgItem(IDC_EDIT_VIEWNAMECHANGE)->SetWindowText(szItemUnique);
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGoonzuLogView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowItemInfo();
	m_pItemExplain->ShowWindow(SW_SHOWNA);
}

void CGoonzuLogView::ShowItemInfo()
{
	char Binaryedit[32]="";
	char Uniqueedit[32]="";
	SI32 itemunique = 0 ;

	m_BinaryEdit.GetWindowText(Binaryedit,32);
	m_UniqueEdit.GetWindowText(Uniqueedit,32);

	itemunique = atoi(Uniqueedit);

	if ( Binaryedit[0] == '0' && Binaryedit[1] == 'x' )
	{
		for ( int i = 0 ; i < 12 ; i ++ )
		{
			char buf[2] = "" ;
			memcpy(buf,&Binaryedit[i*2+2],sizeof(char)*2);

			int n = 0;
			for(int j = 0; j < 2; j++)
			{
				char c = toupper(buf[j]);
				if(c < '0' || c > 'F' || (c > '9' && c < 'A'))
					break;
				int d = c - '0';
				if(d > 9) d -= 7;
				n = n * 16 + d;
			}

			if ( i < 4 )
				Uniqueedit[(i%4)+4] = (char)n ;
			else if ( i < 8 )
				Uniqueedit[(i%4)+8] = (char)n ;
			else if ( i < 12 )
				Uniqueedit[(i%4)+12] = (char)n ;
		}
	}
	else
	{
		for ( int i = 0 ; i < 12 ; i ++ )
		{
			char buf[2] = "" ;
			memcpy(buf,&Binaryedit[i*2],sizeof(char)*2);

			int n = 0;
			for(int j = 0; j < 2; j++)
			{
				char c = toupper(buf[j]);
				if(c < '0' || c > 'F' || (c > '9' && c < 'A'))
					break;
				int d = c - '0';
				if(d > 9) d -= 7;
				n = n * 16 + d;
			}

			if ( i < 4 )
				Uniqueedit[7-(i%4)] = (char)n ;
			else if ( i < 8 )
				Uniqueedit[11-(i%4)] = (char)n ;
			else if ( i < 12 )
				Uniqueedit[15-(i%4)] = (char)n ;
		}
	}

	cltItem pclItem ;
	pclItem.Set(Uniqueedit);
	pclItem.siUnique = itemunique;
	pclItem.siItemNum = 1 ;

	char buffer[1024] = "" ;


	//m_pItemManager->GetExplain(pclItem.siUnique,buffer);
	m_pItemManager->GetExplain(&pclItem,buffer, true);

	m_pItemExplain->m_ExplainEdit.SetWindowText(buffer);
}

void CGoonzuLogView::OnCancel()
{
	// 각 스레드 종료
	m_bExit = TRUE;

	if ( m_pItemExplain )
	{
		delete m_pItemExplain;
		m_pItemExplain = NULL;
	}

	CDialog::OnCancel();
	((CGoonzuUserlogDlg *)AfxGetApp()->GetMainWnd())->ExitProcess();

	return;
}

void CGoonzuLogView::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGoonzuLogView::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int siTotItems = m_ListView.GetItemCount();
	m_bSortAscending = (pNMLV->iSubItem == m_iSortColumn ? !m_bSortAscending : TRUE);
	m_iSortColumn = pNMLV->iSubItem;

	// Callback Item Setting
	for(int i=0; i<siTotItems; i++) 
	{
		m_ListView.SetItemData( i,  i);
	}

	m_ListView.SortItems( CompareFunction, (LPARAM)(CGoonzuLogView*)this );

	*pResult = 0;
}

bool CGoonzuLogView::IsNumber(LPCTSTR pszText)
{
	ASSERT_VALID_STRING( pszText );

	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;

	return true;
}


int CGoonzuLogView::NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	ASSERT_VALID_STRING( pszNumber1 );
	ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = atoi( pszNumber1 );
	const int iNumber2 = atoi( pszNumber2 );

	if( iNumber1 < iNumber2 )
		return -1;

	if( iNumber1 > iNumber2 )
		return 1;

	return 0;
}


bool CGoonzuLogView::IsDate( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	// format should be 99/99/9999.

	if( lstrlen( pszText ) != 10 )
		return false;

	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& pszText[ 2 ] == _T('/')
		&& _istdigit( pszText[ 3 ] )
		&& _istdigit( pszText[ 4 ] )
		&& pszText[ 5 ] == _T('/')
		&& _istdigit( pszText[ 6 ] )
		&& _istdigit( pszText[ 7 ] )
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}

int CGoonzuLogView::DateCompare( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = atoi( strDate1.Mid( 6, 4 ) );
	const int iYear2 = atoi( strDate2.Mid( 6, 4 ) );

	if( iYear1 < iYear2 )
		return -1;

	if( iYear1 > iYear2 )
		return 1;

	const int iMonth1 = atoi( strDate1.Mid( 3, 2 ) );
	const int iMonth2 = atoi( strDate2.Mid( 3, 2 ) );

	if( iMonth1 < iMonth2 )
		return -1;

	if( iMonth1 > iMonth2 )
		return 1;

	const int iDay1 = atoi( strDate1.Mid( 0, 2 ) );
	const int iDay2 = atoi( strDate2.Mid( 0, 2 ) );

	if( iDay1 < iDay2 )
		return -1;

	if( iDay1 > iDay2 )
		return 1;

	return 0;
}

int CALLBACK CGoonzuLogView::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CGoonzuLogView* pLogView = (CGoonzuLogView*)lParamData;

	CString pszText1 = pLogView->m_ListView.GetItemText((int)lParam1, pLogView->m_iSortColumn);
	CString pszText2 = pLogView->m_ListView.GetItemText((int)lParam2, pLogView->m_iSortColumn);

	if(pszText1.GetLength() == 0)
		pszText1 = " ";
	if(pszText2.GetLength() == 0)
		pszText2 = " ";

	if(pLogView->m_iSortColumn == 0)
		return pLogView->m_bSortAscending ? pLogView->NumberCompare( pszText1, pszText2 ) : pLogView->NumberCompare( pszText2, pszText1 );
	else
        return pLogView->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
/*
	if( IsNumber( pszText1 ) )
		return m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
	else if( IsDate( pszText1 ) )
		return m_bSortAscending ? DateCompare( pszText1, pszText2 ) : DateCompare( pszText2, pszText1 );
	else
		// text.
		return m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
*/
}

void CGoonzuLogView::OnBnClickedButtonSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szSearch;
	int siStart = 0, siFind = 0;

	DisableButton();

	// 선택되있는열이 있으면 선택해제한다.
	int nIndex = 0;
	nIndex = m_ListView.GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		m_ListView.SetItemState(nIndex, 0, LVNI_SELECTED | LVNI_FOCUSED );
		nIndex = m_ListView.GetNextItem(nIndex, LVNI_SELECTED);
	}

	// 검색시작
	m_SearchEdit.GetWindowText(szSearch);

	if(szSearch.GetLength() == 0)
		return;

	siFind = ListFindItem(szSearch, siStart);

	EnableButton();

	CString szMsg;
	if(siFind > 0)
		szMsg.Format("%d열 찾음", siFind);
	else
		szMsg.Format("찾을 수 없음");

	AfxMessageBox(szMsg);
}

int CGoonzuLogView::ListFindItem(CString szSearch, int siStart)
{
	int iCount;
	int	siFind = 0;
	iCount = m_ListView.GetItemCount();    // 리스트의 전체 갯수

	for(int i=siStart; i<iCount; i++)
	{
		for(int j=2; j<m_subitem; j++)
		{
			CString szItemText = m_ListView.GetItemText(i, j);
			if(szItemText.MakeLower().Find(szSearch.MakeLower()) >= 0)
			{
				m_ListView.SetItemState(i, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED );
				m_ListView.SetFocus();
				siFind++;
				break;
			}
		}
	}
	return siFind;
}

void CGoonzuLogView::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CGoonzuLogView::OnBnClickedButtonUnselectedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iMaxCount = m_ListView.GetItemCount();
	for(int nIndex = iMaxCount-1; nIndex >= 0; nIndex--)
	{
		if(m_ListView.GetItemState( nIndex, LVNI_SELECTED) != LVNI_SELECTED)
		{
			m_ListView.DeleteItem(nIndex);
			m_siMaxRowcount--;
		}
	}
}

void CGoonzuLogView::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", NULL, 0);
	if(fileDlg.DoModal() == IDOK)
	{
		// 기존 출력내용을 지운다.
		m_ListView.DeleteAllItems();
		m_ViewEdit.SetWindowText("");

		m_szFileName = fileDlg.GetPathName();

		unsigned uThreadId;

		m_hLoadThread = (HANDLE)_beginthreadex(NULL, 0, LoadLogFile, this, 0, &uThreadId);
		if(m_hLoadThread == NULL)
		{
			AfxMessageBox("CreateThread실패");
		}

		ShowRowcount();
	}
}

unsigned __stdcall CGoonzuLogView::LoadLogFile(LPVOID lpParam)
{
	CGoonzuLogView *pLogView = (CGoonzuLogView*)lpParam;

	pLogView->LoadLogFile(pLogView->m_szFileName);
	return 0;
}

bool CGoonzuLogView::LoadLogFile(CString szFileName)
{
	char strbuffer[1024] = "";
	CString szRowcount;
	CLogUnit LogUnit;

	m_siMaxRowcount = 0;
	DisableButton();

	FILE *fp = fopen(szFileName, "r");
	if(fp == NULL)
		return false;

	while(fgets(strbuffer, 1024, fp) != NULL)
	{
		// 로딩중 프로그램을 종료시키려하면 루프를 빠져나간다.
		if(m_bExit == TRUE)
			break;

		LogUnit.Init();
		
		int point = 0;
		int startpoint = 0;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szDate, &point, sizeof(LogUnit.szDate)) == false) continue;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szCommand, &point, sizeof(LogUnit.szCommand)) == false) continue;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szRetPersonName, &point, sizeof(LogUnit.szRetPersonName)) == false) continue;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szDstPersonName, &point, sizeof(LogUnit.szDstPersonName)) == false) continue;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szItemUnique, &point, sizeof(LogUnit.szItemUnique)) == false) continue;
		if(GetStringFromLogfile(&strbuffer[point], LogUnit.szLogTxt, &point, sizeof(LogUnit.szLogTxt)) == false) continue;
		
		InsertLogUnit(&LogUnit);

		if(m_siMaxRowcount % 100 == 0)
		{
			szRowcount.Format("검색중 : %d", m_siMaxRowcount);
			m_SearchButton3.SetWindowText(szRowcount);
		}

	}

	if(fp != NULL)
		fclose(fp);

	EnableButton();

	return true;
}

void CGoonzuLogView::OnBnClickedButtonSendchildwindow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DisableButton();
	OpenChildProcess();
	SendSelectedLogUnit();
	EnableButton();
}

void CGoonzuLogView::OpenChildProcess()
{
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;
	char strCmdLine[512] = "";
	DWORD ExitCode;

	GetExitCodeProcess(m_ChildProcess, &ExitCode);
	if(ExitCode != STILL_ACTIVE)
	{
		if(m_ChildProcess != NULL)
		{
			m_ChildHwnd = NULL;
			CloseHandle(m_ChildProcess);
		}

		strcat(strCmdLine, " ");
		strcat(strCmdLine, m_username);
		strcat(strCmdLine, " ");
		strcat(strCmdLine, m_password);

		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = 0;

		if(CreateProcess( m_strAppFullPath, strCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == FALSE)
		{
			AfxMessageBox("자식프로세스를 열 수 없습니다.");
			return;
		}

		m_dwProcessId = pi.dwProcessId;
		m_ChildProcess = pi.hProcess;
		
		// 프로세스가 완전히 오픈될 때까지 기다린다.
		WaitForInputIdle(pi.hProcess, INFINITE);
		Sleep(1000 );

		// HWND를 얻는다.
		EnumWindows(FindChildWindow, (LPARAM)(CGoonzuLogView*)this);
	}
}

void CGoonzuLogView::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedButton2();
	*pResult = 0;
}

void CGoonzuLogView::InsertLogUnit(CLogUnit *pLogUnit)
{
	char buffer[64]="";

	LV_ITEM LI;
	LI.mask=LVIF_TEXT;
	LI.state=0;
	LI.stateMask=0;

	sprintf(buffer,"%d",m_siMaxRowcount+1);
	LI.iItem=m_siMaxRowcount;
	LI.iSubItem=0;
	LI.pszText=buffer;
	LI.lParam = m_siMaxRowcount;
	m_ListView.InsertItem(&LI);

	LI.iSubItem=1;
	LI.pszText=pLogUnit->szDate;
	m_ListView.SetItem(&LI);

	LI.iSubItem=2;
	LI.pszText=pLogUnit->szCommand;
	m_ListView.SetItem(&LI);

	LI.iSubItem=3;
	LI.pszText=pLogUnit->szRetPersonName;
	m_ListView.SetItem(&LI);

	LI.iSubItem=4;
	LI.pszText=pLogUnit->szDstPersonName;
	m_ListView.SetItem(&LI);

	LI.iSubItem=5;
	LI.pszText=pLogUnit->szItemUnique;
	m_ListView.SetItem(&LI);

	LI.iSubItem=6;
	LI.pszText=pLogUnit->szLogTxt;
	m_ListView.SetItem(&LI);

	m_siMaxRowcount++;
}


void CGoonzuLogView::DisableButton()
{
	GetDlgItem(IDC_LIST2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SENDCHILDWINDOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SELECTED_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNSELECTED_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVEFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VIEWNAMECHANGE)->EnableWindow(FALSE);
	m_SearchButton3.SetWindowText("검색중");
}

void CGoonzuLogView::EnableButton()
{
	GetDlgItem(IDC_LIST2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SENDCHILDWINDOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SELECTED_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNSELECTED_DELETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SAVEFILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_VIEWNAMECHANGE)->EnableWindow(TRUE);
	m_SearchButton3.SetWindowText("검색하기");
	m_siCurRow = 0;
	ShowRowcount();
}

void CGoonzuLogView::SendSelectedLogUnit()
{
	COPYDATASTRUCT cds;
	CLogUnit LogUnit;
	CString szTotalSend;
	int nIndex = 0, siSendCount = 0;

	// 마우스로 선택된 항목을 보낸다.
	nIndex = m_ListView.GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		m_ListView.GetItemText(nIndex, 1, LogUnit.szDate, sizeof(LogUnit.szDate));
		m_ListView.GetItemText(nIndex, 2, LogUnit.szCommand, sizeof(LogUnit.szCommand));
		m_ListView.GetItemText(nIndex, 3, LogUnit.szRetPersonName, sizeof(LogUnit.szRetPersonName));
		m_ListView.GetItemText(nIndex, 4, LogUnit.szDstPersonName, sizeof(LogUnit.szDstPersonName));
		m_ListView.GetItemText(nIndex, 5, LogUnit.szItemUnique, sizeof(LogUnit.szItemUnique));
		m_ListView.GetItemText(nIndex, 6, LogUnit.szLogTxt, sizeof(LogUnit.szLogTxt));

		cds.dwData = 0;
		cds.cbData = sizeof(CLogUnit);
		cds.lpData = &LogUnit;
		::SendMessage(m_ChildHwnd, WM_COPYDATA, (WPARAM)AfxGetInstanceHandle(), (LPARAM)&cds);

		siSendCount++;
		// 테스트코드.. 자기자신에게 보낸다.
		//::SendMessage(GetSafeHwnd(), WM_COPYDATA, (WPARAM)AfxGetInstanceHandle(), (LPARAM)&cds);

		nIndex = m_ListView.GetNextItem(nIndex, LVNI_SELECTED);
	}
}

int CALLBACK CGoonzuLogView::FindChildWindow(HWND hwnd, LPARAM lParam)
{
	DWORD dwID;
	char szText[100];
	CGoonzuLogView *pLogView = (CGoonzuLogView*)lParam;

	GetWindowThreadProcessId(hwnd, &dwID);

	if(dwID == pLogView->m_dwProcessId)
	{
		// 같은 ProcessID를 가진 핸들만 10개가 나왔다. 그중에서 실제 사용해야할 핸들은?
		// 캡션을 조사해서 GoonzuUserLog로 되어있는넘이 실제 핸들
		::GetWindowTextA(hwnd, szText, 100);
		if(strcmp("GoonzuUserLog", szText) == 0)
		{
			pLogView->m_ChildHwnd = hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CGoonzuLogView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLogUnit LogUnit;
	memcpy(&LogUnit, pCopyDataStruct->lpData, sizeof(CLogUnit));

	InsertLogUnit(&LogUnit);

	ShowRowcount();	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CGoonzuLogView::OnBnClickedButtonSelectedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = 0;
	nIndex = m_ListView.GetNextItem(-1, LVNI_SELECTED);
	while(nIndex != -1)
	{
		m_ListView.DeleteItem(nIndex);
		m_siMaxRowcount--;
		nIndex = m_ListView.GetNextItem(nIndex-1, LVNI_SELECTED);
	}
}

BOOL CGoonzuLogView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CString szTimeformatStr = _T(" HH시 mm분");
	m_DTStartTime.SetFormat(szTimeformatStr);
	m_DTEndTime.SetFormat(szTimeformatStr);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGoonzuLogView::InsertFilter(int nIndex, char *strFilter)
{
//	m_ListBox.AddString((LPCTSTR)strFilter);
	m_ListBox.InsertString(nIndex, strFilter);
}

bool CGoonzuLogView::GetStringFromLogfile(char *source, char *out, int *index, int size)
{
	char *pdst = NULL;
	char ch = '"';
	int nResult = -1, nStart = -1, nEnd = -1;

	pdst = strchr( source, ch );
	nStart = (int)(pdst - source + 1);
	if(pdst == NULL)
		return false;

	pdst = strchr( &source[nStart], ch );
	nEnd = (int)(pdst - &source[nStart] + 1);
	if(pdst == NULL)
		return false;

	memcpy(out, &source[nStart], (nEnd-1) > size ? size : (nEnd-1));
	*index += (nEnd + nStart);
	return true;
}

void CGoonzuLogView::OnLvnKeydownList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pLVKeyDow->wVKey == VK_DELETE)
	{
		OnBnClickedButtonSelectedDelete();
	}
	*pResult = 0;
}

void CGoonzuLogView::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// OnOk를 주석처리하여 Default 버튼이 없는상태에서 엔터를 눌렀을때 창이 닫히는 경우를 막는다.
	//CDialog::OnOK();
}

void CGoonzuLogView::OnEnSetfocusEdit1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON1);
}

void CGoonzuLogView::OnEnSetfocusEdit4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON2);
}

void CGoonzuLogView::OnEnSetfocusEdit5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON2);
}

void CGoonzuLogView::OnEnSetfocusEditSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON_SEARCH);
}

void CGoonzuLogView::OnEnSetfocusEdit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON3);
}

void CGoonzuLogView::OnEnSetfocusEdit3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON3);
}

void CGoonzuLogView::OnEnSetfocusEditUserSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON3);
}

void CGoonzuLogView::OnLbnSetfocusList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON3);
}

void CGoonzuLogView::OnExitApp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 스레드 종료 확인
	OnCancel();
}

void CGoonzuLogView::OnOpenFile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButtonLoad();
}

void CGoonzuLogView::OnSaveFile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButton4();
}

void CGoonzuLogView::OnSendData()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButtonSendchildwindow();
}

void CGoonzuLogView::OnDeleteSelected()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButtonSelectedDelete();
}

void CGoonzuLogView::OnDeleteUnselected()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButtonUnselectedDelete();
}

BOOL CGoonzuLogView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// ESC로 종료되는걸 막기
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	// 메뉴 단축키 처리
	if(m_hAccel) {
		if( ::TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) ) {
			return (TRUE);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGoonzuLogView::OnBnClickedButtonDatevary()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szDateVary;
	CString szGameTime;
	SI32	siDateVary = 0;
	UI32	uiYear = 0, uiMonth = 0, uiDay = 0;
	GetDlgItem(IDC_EDIT_DATEVARY)->GetWindowText(szDateVary);

	siDateVary = atoi(szDateVary);

	uiYear  = siDateVary / (12* 30);
	siDateVary %= (12* 30);

	uiMonth = siDateVary / 30 + 1;
	siDateVary %= 30;

	uiDay		= siDateVary + 1;

	szGameTime.Format("%d년 %d월 %d일", uiYear, uiMonth, uiDay);
	GetDlgItem(IDC_EDIT_GAMETIME)->SetWindowText(szGameTime);
}

void CGoonzuLogView::OnEnSetfocusEditDatevary()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SendMessage (DM_SETDEFID, IDC_BUTTON_DATEVARY);
}
