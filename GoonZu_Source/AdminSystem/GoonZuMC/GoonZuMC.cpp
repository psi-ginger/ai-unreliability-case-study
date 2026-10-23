// GoonZuMC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GoonZuMC.h"
#include "GoonZuMCDlg.h"
#include "mmsystem.h"

#include "LoginDlg.h"

#include "MasterProcess.h"
#include "../MasterServer/ServerInfo.h"

#include "StatisticsData.h"
#include "StatisticsDlg.h"

#include "GMOrderLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void WinSockInit()
{
	WORD	wVersionRequested;
	WSADATA	wsaData;

	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );

	if( err != 0 ) {
		return;
	}

	if( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {

		WSACleanup();
		return;
	}
}



unsigned __stdcall worker( LPVOID param )
{

	CGoonZuMCApp	*pClass = (CGoonZuMCApp *)param;
	
	while( 1 ) {

		pClass = (CGoonZuMCApp	*)AfxGetApp();

		if( pClass == NULL ) break;
		
		if( pClass->m_bRun ) {

			Sleep( 10 );		
			pClass->m_pMasterProcess->Run();

		} else {

			break;
		}
	}

	SetEvent( pClass->m_hExitEvent );

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCApp

BEGIN_MESSAGE_MAP(CGoonZuMCApp, CWinApp)
	//{{AFX_MSG_MAP(CGoonZuMCApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCApp construction

CGoonZuMCApp::CGoonZuMCApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGoonZuMCApp object

CGoonZuMCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCApp initialization

BOOL CGoonZuMCApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	WinSockInit();

	int i;

	m_szID[ 0 ] = NULL;
	
	for( i = 0; i < 10; ++i ) {
		m_szIP[ i ][ 0 ] = NULL;
	}

	FILE *pf;
	
	pf = fopen( "goonzumc.txt", "r" );

	if( pf ) {
	
		fscanf( pf, "%s", m_szID );

		fscanf( pf, "%d", &m_ipindex );

		for( i = 0; i < 10; ++i ) {
			fscanf( pf, "%s", m_szIP[ i ] );
		}

		fclose( pf );

	}

	m_pServerInfo = NULL ;
	m_pServerInfo = new ServerInfo;

	m_pMasterProcess = NULL ;
	m_pMasterProcess = new MasterProcess;

	m_pStatisticsData = NULL;
	m_pStatisticsData = new CStatisticsData;

	m_pGMOrderLogManager = NULL;
	m_pGMOrderLogManager = new CGMOrderLogManager;
	m_pGMOrderLogManager->Create();

	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );

#if defined(_USA)
	m_pMasterProcess->Init( "127.0.0.1", 11200 );
#elif defined(_NHNChina)
       m_pMasterProcess->Init( "127.0.0.1", 11050 );
#else
	m_pMasterProcess->Init( "127.0.0.1", 8200 );
#endif

	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	

	// Login

	m_bMaster = FALSE;

	CLoginDlg	loginDlg;

	while( 1 ) {
		int ret = loginDlg.DoModal();
		
		if( ret == IDOK || ret == IDCANCEL ) break;

		if( ret == 100 ) {

			// save 
			
			pf = fopen( "goonzumc.txt", "w" );

			if( pf ) {
			
				fprintf( pf, "%s\r\n", m_szID );
				fprintf( pf, "%d\r\n", m_ipindex );

				for( i = 0; i < 10; ++i ) {
					if( m_szIP[ i ][ 0 ] == NULL ) break;
					fprintf( pf, "%s\r\n", m_szIP[ i ] );
				}

				fclose( pf );

			}


			CGoonZuMCDlg dlg;
			m_pMainWnd = &dlg;
			int nResponse = dlg.DoModal();
			break;
/*
			if (nResponse == IDOK)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with OK
				break;
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with Cancel
				break;
			}
*/
		}

	}


	// Since the dialog has been closed, return FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}

void CGoonZuMCApp::Alarm( int type )
{
	PlaySound( "ServerFail1.wav", NULL, SND_ASYNC | SND_NOSTOP );	
}

int CGoonZuMCApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if( m_bRun ) {
		ResetEvent( m_hExitEvent );
		InterlockedExchange( (LONG*)&m_bRun, FALSE );
		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	CloseHandle( m_hExitEvent );

	delete m_pMasterProcess ;
	m_pMasterProcess = NULL ;
	
	delete m_pServerInfo ;
	m_pServerInfo = NULL ;
	
	delete m_pGMOrderLogManager;
	m_pGMOrderLogManager = NULL;

	delete m_pStatisticsData;
	m_pStatisticsData = NULL;

	WSACleanup();	


	return CWinApp::ExitInstance();
}
