// GoonzuUserlogDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GoonzuUserlog.h"
#include "GoonzuUserlogDlg.h"
#include ".\goonzuuserlogdlg.h"
#include "ODBC/RichODBC.h"
#include "VillageManager/VillageManager.h"
#include "item/itemcommon/textmgr.h"

#include "GoonzuLogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern cltTextMgr g_pTextMgr;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

	// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGoonzuUserlogDlg 대화 상자



CGoonzuUserlogDlg::CGoonzuUserlogDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGoonzuUserlogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoonzuUserlogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_NameEdit);
	DDX_Control(pDX, IDC_EDIT2, m_PassWord);
	DDX_Control(pDX, IDC_COMBO_SERVERSELECT, m_cbServerSelect);
}

BEGIN_MESSAGE_MAP(CGoonzuUserlogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CGoonzuUserlogDlg 메시지 처리기

BOOL CGoonzuUserlogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pGoonzuLogView= NULL;
	strcpy(UserID,"");
	strcpy(PassWord,"");

	BYTE	szGoonzuPath[MAX_PATH] = "";
	char	szTextMgr[512] = "";
	DWORD	dwSize = sizeof(szGoonzuPath);
	HKEY	hKey;
	LONG	lReturn;
	CString	szError;

	lReturn = RegOpenKey(HKEY_CURRENT_USER, "Software\\Intizen\\Goonzu", &hKey);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("레지스트리를 열 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return TRUE;
	}
	lReturn = RegQueryValueEx(hKey, "PATH", NULL, NULL, szGoonzuPath, &dwSize);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("군주가 설치된 폴더를 찾을 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return TRUE;
	}

	RegCloseKey(hKey);

	sprintf(szTextMgr, "%sTextMgr\\Textmgr.txt", szGoonzuPath);
	g_pTextMgr.Create(szTextMgr);


	m_cbServerSelect.AddString("Ndoors");
	m_cbServerSelect.AddString("NetMarble");
	m_cbServerSelect.SetCurSel(0);

	// ID와 PW가 인자로 오면 그값으로 실행한다.
	if(__argc == 3)
	{
		OnBnClickedOk();
	}

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CGoonzuUserlogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CGoonzuUserlogDlg::OnPaint() 
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
HCURSOR CGoonzuUserlogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGoonzuUserlogDlg::OnBnClickedOk()
{
#ifdef _DEBUG

	//	CVillageManager village= CVillageManager();
	//	village.Set("123");

	//Item.Set("123","456");


#endif


	if(__argc == 1)
	{
		m_NameEdit.GetWindowText(UserID,20);
		m_PassWord.GetWindowText(PassWord,20);
	}
	else if(__argc == 3)
	{
		strncpy(UserID, __argv[1], 20);
		strncpy(PassWord, __argv[2], 20);
	}

	if ( strlen( UserID ) < 1 || strlen( PassWord ) < 1)
	{
		OnCancel();
		return;
	}

	if(strcmp(UserID,"sa")==0)
	{
		OnCancel();
		return;
	}

	// 인스턴스 생성
	RichODBC *pDB = new RichODBC;
	BOOL dbCon = FALSE;


	CString szServerName;
	int	siServerSelect;

	m_cbServerSelect.GetWindowText(szServerName);

//	if(strcmp(szServerName, "Ndoors") == 0)
	if(szServerName == "Ndoors")
	{
		siServerSelect = NDOORS;
		dbCon = pDB->Connect("SQL Server", SERVERIP, "RichLog", UserID, PassWord);
	}
	else
	{
		siServerSelect = NETMARBLE;
		dbCon = pDB->Connect("SQL Server", NETMARBLESERVERIP, "RichLog", UserID, PassWord);
	}

	dbCon = pDB->Connect("SQL Server", SERVERIP, "RichLog", UserID, PassWord);
	pDB->Disconnect();

	delete pDB;




	// DB 접속
	if (dbCon)
	{

		if(m_pGoonzuLogView)
		{
			delete m_pGoonzuLogView;
			m_pGoonzuLogView= NULL;
		}
		m_pGoonzuLogView = new CGoonzuLogView(this);
		m_pGoonzuLogView->SetAccount( UserID, PassWord );
		m_pGoonzuLogView->SetServerSelect(siServerSelect);
		strcpy( UserID, "" );
		strcpy( PassWord, "" );
		m_pGoonzuLogView->Create( IDD_DIALOG1, this);
		ShowWindow( SW_SHOWMINIMIZED | SW_HIDE);
		m_pGoonzuLogView->ShowWindow(SW_SHOWNORMAL );
		m_pGoonzuLogView->ShowUserLog();
	}
	else
	{
		OnCancel();
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CGoonzuUserlogDlg::ExitProcess()
{

	if ( m_pGoonzuLogView )
	{
		delete m_pGoonzuLogView;
		m_pGoonzuLogView = NULL;
	}

	EndDialog( 0 );
	return;
}

//BOOL CGoonzuUserlogDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	AfxMessageBox("왔다");
//	m_pGoonzuLogView->OnCopyData(pWnd, pCopyDataStruct);
//	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
//}

