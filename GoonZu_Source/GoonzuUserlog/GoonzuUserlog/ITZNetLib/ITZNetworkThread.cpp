#include "ITZNetworkThread.h"

BOOL	g_bInitWinSock = FALSE;

unsigned __stdcall nt_worker( LPVOID param )
{
	
	ITZNetworkThread	*pClass = (ITZNetworkThread *)param;

	DWORD				dwCurrentTick;
	DWORD				dwGTick;
	
	LONG				lATick;

	DWORD 				dwTickPF;

	dwGTick = GetTickCount();

	lATick = 0;

	dwTickPF = (DWORD)( 1000 / pClass->m_dwFPS );

	dwCurrentTick = GetTickCount();
	dwGTick = GetTickCount();

	while( pClass->m_bRun ) {


		dwCurrentTick = GetTickCount();

		if( dwCurrentTick < dwGTick + dwTickPF ) {
			Sleep( 10 );
			continue;
		}

		dwGTick += dwTickPF;


/*
		dwCurrentTick = GetTickCount();
 
		lATick += (LONG)( dwTickPF - ( dwCurrentTick - dwGTick ) );

		if( lATick > (LONG)(dwTickPF * 3) ) {
			dwGTick = dwCurrentTick + dwTickPF;
			lATick = 0;
		}

		while( lATick >= 10 ) {
			Sleep( 10 );
			lATick -= 10;
		}

		dwGTick += dwTickPF;
*/


		pClass->Update();

		pClass->m_pSessionManager->Update();

		ITZSession *pSession;

		for( pSession = pClass->m_pSessionManager->GetFirstSession(); pSession != NULL; pSession = pClass->m_pSessionManager->NextSession( pSession ) ) { 

			if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) { 

				pClass->SessionProc( pSession ); 
			} 

		}

	}

	SetEvent( pClass->m_hExitEvent );

	return 0;
}


ITZNetworkThread::ITZNetworkThread()
{

	InitWinSock();

	m_pSessionManager = new ITZSessionManager;
	m_pListen = new ITZListen;
	m_pNetIOCP = new ITZNetIOCP;

	m_bRun = FALSE;

	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );

	m_dwFPS = 20;
}

ITZNetworkThread::~ITZNetworkThread()
{
	Destroy();

	CloseHandle( m_hExitEvent );

	delete m_pListen;
	delete m_pNetIOCP;
	delete m_pSessionManager;

	m_pListen = NULL;
	m_pNetIOCP = NULL;
	m_pSessionManager = NULL;
}


void ITZNetworkThread::CreateThread()
{

	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, nt_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	

}

void ITZNetworkThread::Destroy()
{
	if( m_bRun ) {
		ResetEvent( m_hExitEvent );
		m_bRun = FALSE;
		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	m_pListen->Destroy();
	m_pNetIOCP->Destroy();
}

void ITZNetworkThread::InitWinSock()
{

	if( g_bInitWinSock == FALSE ) {

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

		g_bInitWinSock = TRUE;
	}

}


