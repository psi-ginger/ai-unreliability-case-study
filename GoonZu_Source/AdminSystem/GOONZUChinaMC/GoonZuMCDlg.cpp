// GoonZuMCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "GoonZuMCDlg.h"

#include "MasterProcess.h"
#include ".\goonzumcdlg.h"
#include "process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCDlg dialog


unsigned __stdcall SendMailThread( LPVOID pParam )
{

	

	CGoonZuMCDlg* pClass = (CGoonZuMCDlg*) pParam;

	/*
	CString strAttachFile;		// FILENAME 

	strAttachFile = m_AttachFileName1;
	if( GetFileAttributes(strAttachFile) != INVALID_FILE_ATTRIBUTES )
	{
		g_pkMimeMsg->AttachFile(strAttachFile);
	}
	strAttachFile = m_AttachFileName2;
	if( GetFileAttributes(strAttachFile) != INVALID_FILE_ATTRIBUTES )
	{
		g_pkMimeMsg->AttachFile(strAttachFile);
	}
	*/

	// CONNECT AND SEND
	CSMTPConnection kSMTPConnect;
	BOOL bSuccess = TRUE;

	bSuccess = kSMTPConnect.Connect( pClass->m_smtpserver );
	if(bSuccess == TRUE)  
	{
		bSuccess = kSMTPConnect.SendMessage( *pClass->m_mimeMsg );
		if(bSuccess == FALSE)
		{
			/*
			CString kStr1((LPCTSTR)"HIHI1");
			CString kStr2((LPCTSTR)"TEST");
			MessageBox(NULL, kStr1, kStr2, MB_OK | MB_TOPMOST);
			*/
		}
		kSMTPConnect.Disconnect();
	}
	else
	{
		/*
		CString kStr1((LPCTSTR)"HIHI2");
		CString kStr2((LPCTSTR)"TEST2");
		MessageBox(NULL, kStr1, kStr2, MB_OK | MB_TOPMOST);
		*/
	}

	if(bSuccess == TRUE)
	{
		//CString kStr((LPCTSTR)IDS_STRING_SENT_DATA);
		//pPaperError->m_StatusButton.SetWindowText(kStr);

		Sleep(500);
	}

//	SendMessage(pPaperError->m_hWnd, WM_COMMAND, IDC_OK, NULL);
	return 0;

}


CGoonZuMCDlg::CGoonZuMCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGoonZuMCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGoonZuMCDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for( int i = 0; i < 10; ++i ) {
		m_hWndTabDialog[ i ] = NULL;
	}

	
	m_maxUser = 0;

	m_bFlashMode = FALSE;
	m_bFlashTogle = FALSE;

	m_mimeMsg = NULL;

//	CDialog* temp = (CDialog*)GetDlgItem(CGoonZuMCDlg::IDD);

//	temp->SetWindowText("test");
	

//	SetWindowText("aaA");
}

void CGoonZuMCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGoonZuMCDlg)

	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT1, m_emailedit);
	DDX_Control(pDX, IDC_EDIT2, m_smtpedit);
}

BEGIN_MESSAGE_MAP(CGoonZuMCDlg, CDialog)
	//{{AFX_MSG_MAP(CGoonZuMCDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MAIN, OnSelchangingTabMain)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCDlg message handlers

BOOL CGoonZuMCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetIcon( m_hIcon, FALSE );
	
	// TODO: Add extra initialization here

	m_bError = FALSE;
	m_errorCode = 0;
	m_bSoundOn = FALSE;
	ZeroMemory(m_szErrorServer, 256);

	m_dwLastFrontErrorTick = GetTickCount() - 40000;
	m_dwLastGameErrorTick = GetTickCount() - 40000;
	m_dwLastSendMailTick = GetTickCount() - 40000;


	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	int i;
	
	CRect rect;

	CWnd *pTestWnd = GetDlgItem( IDC_STATIC_RICH );

	pTestWnd->GetWindowRect( &rect );

	rect.top -= 20;
	rect.bottom -= 32;
	rect.left -= 1;
	rect.right -= 6;

	

	m_richMsg.Create( WS_VISIBLE | WS_CHILD|WS_CLIPCHILDREN | WS_VSCROLL | WS_BORDER |
		              ES_MULTILINE|ES_READONLY | ES_AUTOVSCROLL | ES_LEFT | ES_WANTRETURN, 
					  rect, this, NULL );

	CGoonZuMCApp *pApp1 = ((CGoonZuMCApp *)AfxGetApp());
	MasterProcess *pMasterProcess = pApp1->m_pMasterProcess;		

	char tempbuffer[256] = "GoonZuChina - ";
	strcat( tempbuffer, pMasterProcess->m_szIP );
		
	SetWindowText( tempbuffer );

	// 리스트 컨트롤 초기화 
/*
	SendDlgItemMessage( IDC_LIST_LOG, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );



	LVCOLUMN lvcol;

	int width[] = { 140, 600 };
	char *text[] = { "시간", "내용" };

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for( i = 0; i < 2; ++i ) {
		lvcol.cx = width[ i ];
		lvcol.pszText = text[ i ];
		lvcol.iSubItem = i;
		
		SendDlgItemMessage( IDC_LIST_LOG, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}

*/

	m_FrontServerListDlg.Create( IDD_DIALOG_FRONTSERVERLIST, &m_tabMain );
	m_GameServerListDlg.Create( IDD_DIALOG_GAMESERVERLIST, &m_tabMain );
	m_StatisticsDlg.Create( IDD_DIALOG_STATISTICS, &m_tabMain );

	m_hWndTabDialog[ 0 ] = m_FrontServerListDlg.m_hWnd;
	m_hWndTabDialog[ 1 ] = m_GameServerListDlg.m_hWnd;
	m_hWndTabDialog[ 2 ] = m_StatisticsDlg.m_hWnd;

	char *str[] = { "Login Server Group", "Game Server Group", "Graph" };

	TCITEM tcItem;

	int tabnum = 3;

//	if( pApp->m_bMaster ) tabnum = 3; else tabnum = 2;


	for( i = 0; i < tabnum; ++i ) {
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = str[ i ];

		m_tabMain.InsertItem( i, &tcItem );
	}

	m_errorcount = 0;

	TabCtrl_SetCurFocus( m_tabMain.m_hWnd, 1 );

	SetTimer( 1, 500, NULL );
	SetTimer( 2, 1200, NULL );

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGoonZuMCDlg::OnPaint() 
{

	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGoonZuMCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CGoonZuMCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CDialog::OnCreate(lpCreateStruct) == -1 ) return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CGoonZuMCDlg::OnDestroy() 
{
	// TODO: Add your message handler code here
	KillTimer( 1 );
	KillTimer( 2 );

	CDialog::OnDestroy();
}

void CGoonZuMCDlg::OnSelchangingTabMain( NMHDR* pNMHDR, LRESULT* pResult ) 
{

	// TODO: Add your control notification handler code here

	int	num = TabCtrl_GetCurSel( pNMHDR->hwndFrom );
		
	if( m_hWndTabDialog[ num ] ) ::ShowWindow( m_hWndTabDialog[ num ], SW_HIDE );
	
	*pResult = 0;
}

void CGoonZuMCDlg::OnSelchangeTabMain( NMHDR* pNMHDR, LRESULT* pResult ) 
{

	// TODO: Add your control notification handler code here
	
	int num = TabCtrl_GetCurSel( pNMHDR->hwndFrom );

	for ( int i = 0 ; i < 10 ; i ++ )
		if ( m_hWndTabDialog[i] && i != num )
			::ShowWindow( m_hWndTabDialog[ i ], SW_HIDE );

	RECT rect;

	::GetClientRect( m_tabMain.m_hWnd, &rect );

	TabCtrl_AdjustRect( m_tabMain.m_hWnd, FALSE, &rect );

	::MoveWindow( m_hWndTabDialog[ num ], 
		rect.left, rect.top, 
		rect.right - rect.left,
		rect.bottom - rect.top, TRUE );

	::ShowWindow( m_hWndTabDialog[ num ], SW_SHOW );

	*pResult = 0;
}

void CGoonZuMCDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
	
	char buffer[ 1024 ];


	if( GetTickCount() - m_lasterrtick >= 60 * 1000 )
	{
		m_errorcount = 0;
	}


	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	
	if( nIDEvent == 1 )
	{
		if( m_bError )
		{
			SYSTEMTIME systime;
			GetLocalTime( &systime );

			if( m_errorCode & 1 ) 
			{
				sprintf( buffer, "[%02d:%02d:%02d] %s",
					systime.wHour, systime.wMinute, systime.wSecond,
					"Check Front Server!\r\n" );

				if( GetTickCount() - m_dwLastFrontErrorTick > 10000 ) 
				{
					m_richMsg.AddText( buffer, RGB( 240, 20, 20 ) );
					m_bSoundOn = TRUE;
				}

				if (GetTickCount() - m_dwLastSendMailTick > 10000)
				{
					m_errorcount++;

					if( m_errorcount > 3 ) {

						m_emailedit.GetWindowText( m_receiverid );
						m_smtpedit.GetWindowText( m_smtpserver );

						m_sendtext = buffer;

						if (systime.wHour == 6 && systime.wMinute >= 28 && systime.wMinute <= 32)
						{
						}
						else
						{
							SendMail();
						}

						m_errorcount = 0;
					}

					m_dwLastSendMailTick = GetTickCount();

				}

				m_lasterrtick = GetTickCount();
				m_dwLastFrontErrorTick = GetTickCount();
			}

			if( m_errorCode & 2 ) 
			{
				sprintf( buffer, "[%02d:%02d:%02d] Check Game Server %s !\r\n",
					systime.wHour, systime.wMinute, systime.wSecond,
					m_szErrorServer );

				if( GetTickCount() - m_dwLastGameErrorTick > 10000 ) 
				{
					m_richMsg.AddText( buffer, RGB( 20, 20, 240 ) );
					m_bSoundOn = TRUE;					
				}

				if( GetTickCount() - m_dwLastSendMailTick > 10000)
				{
					m_errorcount++;

					if( m_errorcount > 3 ) 
					{
						m_emailedit.GetWindowText( m_receiverid );
						m_smtpedit.GetWindowText( m_smtpserver );

						m_sendtext = buffer;

						if (systime.wHour == 6 && systime.wMinute >= 28 && systime.wMinute <= 32)
						{
						}
						else
						{
							SendMail();
						}

						m_errorcount = 0;
					}

					m_dwLastSendMailTick = GetTickCount();
				}

				m_lasterrtick = GetTickCount();
				m_dwLastGameErrorTick = GetTickCount();
			}

			m_bFlashMode = TRUE;
			

		} 
		else 
		{
			m_bFlashMode = FALSE;
			m_bSoundOn = FALSE;
		}

		if( m_bSoundOn ) 
		{
			pApp->Alarm(0);
		}
	} 
	else if( nIDEvent == 2 ) {

		
		if( pApp->m_pMasterProcess->GetSession()->GetState() != SESSION_STATE_ESTABLISHED ) {

			KillTimer( 2 );

			MessageBox( "Disconnected! Restart this application.", NULL );

			EndDialog( IDCANCEL );
		}

		if( m_bFlashMode == TRUE ) {

			if( m_bFlashTogle ) {
				FlashWindow( FALSE );
				m_bFlashTogle = FALSE;

			} else {
				
				FlashWindow( TRUE );
				m_bFlashTogle = TRUE;
			}
		} else {
			if( m_bFlashTogle ) {
				FlashWindow( FALSE );
				m_bFlashTogle = FALSE;
			}
		}

	}

}

void CGoonZuMCDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CGoonZuMCDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here

	if( nState == WA_CLICKACTIVE ) {
		m_bFlashMode = FALSE;	
	}
	
}
/*
void CGoonZuMCDlg::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CDialog::OnActivateApp(bActive, hTask);
	
	// TODO: Add your message handler code here

}
*/

void CGoonZuMCDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here

}


void CGoonZuMCDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_bFlashMode = FALSE;
	m_bSoundOn = FALSE;
	
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CGoonZuMCDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGoonZuMCDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// check


/*	m_emailedit.GetWindowText( m_receiverid );
	m_smtpedit.GetWindowText( m_smtpserver );

	m_sendtext = "Test";

	SendMail();*/
}

void CGoonZuMCDlg::SendMail()
{
	if( m_smtpserver.IsEmpty() )
	{
		m_smtpserver.SetString( "mail.ndoors.net" );
	}
	if( m_receiverid.IsEmpty() ) return;

	if( m_mimeMsg ) delete [] m_mimeMsg;
	m_mimeMsg = new CMimeMessage();

	CString sender = "jwlee36@ndoors.net";
	CString recipient = "";
	CString subject = "";

	// SET MESSAGE 

	m_mimeMsg->SetSender( sender );
	m_mimeMsg->AddRecipient( m_receiverid );
	//m_mimeMsg->SetSubject( subject );
	m_mimeMsg->AddText( m_sendtext );

	UI32 uiThreadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, SendMailThread, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );

}