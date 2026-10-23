// GlobalTradeServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GlobalTradeServer.h"
#include "GlobalTradeServerDlg.h"

#include "DBProcess.h"
#include "DBProtocol.h"
#include "GameProtocol.h"
#include "DBLogProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern DBProcess		g_DBProcess;
extern DBProtocol		g_DBProtocol;
extern GameProtocol 	g_GameProtocol;
extern DBLogProtocol	g_dap;


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


// CGlobalTradeServerDlg 대화 상자



CGlobalTradeServerDlg::CGlobalTradeServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlobalTradeServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlobalTradeServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CGlobalTradeServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGlobalTradeServerDlg 메시지 처리기

BOOL CGlobalTradeServerDlg::OnInitDialog()
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
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);

	m_ListCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	LV_COLUMN lvColum;
	lvColum.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColum.fmt = LVCFMT_CENTER;

	for( int  i = 0 ; i < 2; ++i )
	{
		lvColum.iSubItem = i;
		switch(i)
		{
		case 0:
			{
				lvColum.cx = (rect.Width() - 30) / 2;
				lvColum.pszText = "IP";
			}
			break;
		case 1:
			{
				lvColum.cx = (rect.Width() - 30) / 2;
				lvColum.pszText = "STATE";
			}
			break;
		}

		m_ListCtrl.InsertColumn( i, &lvColum );
	}

	LV_ITEM lvItem;
	for ( SI32 i = 0 ; i < g_GameProtocol.GetSessionManager()->m_siMaxSize ; i ++ )
	{
		ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession(i);

		lvItem.iItem = i;
		lvItem.mask = LVIF_TEXT;
		lvItem.iSubItem = 0;

		if ( pSession == NULL )
		{
			lvItem.pszText = "";
			m_ListCtrl.InsertItem( &lvItem );
			m_ListCtrl.SetItemText(lvItem.iItem, 1, "SESSION_STATE_NONE" );

			continue;
		}

		lvItem.pszText = pSession->m_szIP;
		m_ListCtrl.InsertItem( &lvItem );

		if( SESSION_STATE_NONE == pSession->GetState() )
		{
			m_ListCtrl.SetItemText(lvItem.iItem, 1, "SESSION_STATE_NONE" );
		}
		else if( SESSION_STATE_CONNECTED == pSession->GetState() )
		{
			m_ListCtrl.SetItemText(lvItem.iItem, 1, "SESSION_STATE_CONNECTED" );
		}
		else if( SESSION_STATE_ESTABLISHED == pSession->GetState() )
		{
			m_ListCtrl.SetItemText(lvItem.iItem, 1, "SESSION_STATE_ESTABLISHED" );
		}
		else if( SESSION_STATE_DISCONNECTED == pSession->GetState() )
		{
			m_ListCtrl.SetItemText(lvItem.iItem, 1, "SESSION_STATE_DISCONNECTED" );
		}
	}

	m_bWorkRun = false;
	SetTimer( 1, 200, NULL );
	SetTimer( 2, 10000, NULL );

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CGlobalTradeServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGlobalTradeServerDlg::OnPaint() 
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
HCURSOR CGlobalTradeServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGlobalTradeServerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnTimer(nIDEvent);

	if( m_bWorkRun == false )
	{
		m_bWorkRun = true;

		g_DBProcess.Run();

		m_bWorkRun = false;
	}

	if( 2 == nIDEvent )
	{
		for ( SI32 i = 0 ; i < g_GameProtocol.GetSessionManager()->m_siMaxSize ; i ++ )
		{
			ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession(i);
			if ( pSession == NULL )				continue;

			m_ListCtrl.SetItemText( i, 0, pSession->m_szIP );

			if( SESSION_STATE_NONE == pSession->GetState() )
			{
				m_ListCtrl.SetItemText(i, 1, "SESSION_STATE_NONE" );
			}
			else if( SESSION_STATE_CONNECTED == pSession->GetState() )
			{
				m_ListCtrl.SetItemText(i, 1, "SESSION_STATE_CONNECTED" );
			}
			else if( SESSION_STATE_ESTABLISHED == pSession->GetState() )
			{
				m_ListCtrl.SetItemText(i, 1, "SESSION_STATE_ESTABLISHED" );
			}
			else if( SESSION_STATE_DISCONNECTED == pSession->GetState() )
			{
				m_ListCtrl.SetItemText(i, 1, "SESSION_STATE_DISCONNECTED" );
			}
		}
	}
}
