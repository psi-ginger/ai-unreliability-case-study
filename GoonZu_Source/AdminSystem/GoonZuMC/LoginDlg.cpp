// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "LoginDlg.h"

#include "MasterProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_EDIT_PW, m_editPW);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_COMBO_IP, m_comboIP);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btnLogin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

void CLoginDlg::OnButtonLogin() 
{
	// TODO: Add your control notification handler code here

	CGoonZuMCApp *pApp = ((CGoonZuMCApp *)AfxGetApp());
	MasterProcess *pMasterProcess = pApp->m_pMasterProcess;		
	
	char buf[ 128 ];
	int i;

	pMasterProcess->lock();

	m_editID.GetWindowText( buf, 127 );
	memcpy( pMasterProcess->m_szID, buf, 20 );
	pMasterProcess->m_szID[ 19 ] = NULL;

	m_editPW.GetWindowText( buf, 127 );
	memcpy( pMasterProcess->m_szPW, buf, 20 );
	pMasterProcess->m_szPW[ 19 ] = NULL;

	m_comboIP.GetWindowText( buf, 32 );
	memcpy( pMasterProcess->m_szIP, buf, 16 );
	pMasterProcess->m_szIP[ 15 ] = NULL;

	pMasterProcess->m_bLoginFailed = FALSE;

	pMasterProcess->unlock();

	pMasterProcess->SetConnect( TRUE );

	int		c = 0;
	BOOL	bConnected = FALSE;

	DWORD	dwTick = GetTickCount();

	while( GetTickCount() - dwTick < 4000 ) {

		if( pMasterProcess->GetSession()->GetState() == SESSION_STATE_ESTABLISHED ) {

			if( pMasterProcess->m_bLoginOK == TRUE ) {
				bConnected = TRUE;
				break;
			}

			if( pMasterProcess->m_bLoginFailed == TRUE ) {
				break;
			}

		}

		Sleep( 10 );
	}

	if( pMasterProcess->m_bLoginFailed == TRUE ) {
		MessageBox(TEXT("Please check your id and password, or request ServerManager that you can connect to masterserver"));
	}

	pMasterProcess->lock();
	strcpy( pApp->m_szID, pMasterProcess->m_szID );
	
	for( i = 0; i < 10; ++i ) {
		if( strcmp( pApp->m_szIP[ i ], pMasterProcess->m_szIP) == 0 ) {
			pApp->m_ipindex = i;
			break;
		}
	}

	if( i == 10 ) {

		for( i = 8; i >= 0; --i ) {
			memcpy( pApp->m_szIP[ i + 1 ], pApp->m_szIP[ i ], 32 );
		}

		strcpy( pApp->m_szIP[ 0 ], pMasterProcess->m_szIP );
		pApp->m_ipindex = 0;
	}

	pMasterProcess->unlock();


	if( bConnected ) {
		EndDialog( 100 );
	
	} else {
		EndDialog( 99 );
	}

}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	HWND hwnd;

	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {

		if( pMsg->hwnd == ::GetDlgItem( m_hWnd, IDC_EDIT_ID ) ) {
		
			::SetFocus( ::GetDlgItem( m_hWnd, IDC_EDIT_PW ) );

			pMsg->message = 0;

		} else if( pMsg->hwnd == ::GetDlgItem( m_hWnd, IDC_EDIT_PW ) ) {

			pMsg->hwnd = m_hWnd;
			pMsg->message = WM_COMMAND;
			pMsg->wParam = MAKEWPARAM( IDC_BUTTON_LOGIN, BN_CLICKED );
			pMsg->lParam = 0;

		} else if( pMsg->hwnd != ::GetDlgItem( m_hWnd, IDC_BUTTON_LOGIN ) ) {
			pMsg->message = 0;
		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLoginDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog();

	CGoonZuMCApp *pApp = ((CGoonZuMCApp *)AfxGetApp());

	m_editID.SetWindowText( pApp->m_szID );


	for( int i = 0; i < 10; ++i ) {

		if( pApp->m_szIP[ i ][ 0 ] == NULL ) break;
		m_comboIP.InsertString( -1, pApp->m_szIP[ i ] );
	}

	m_comboIP.SetCurSel( pApp->m_ipindex );

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
}
