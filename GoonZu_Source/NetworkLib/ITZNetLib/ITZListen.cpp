
#include "ITZListen.h"
#include "ITZSessionManager.h"

unsigned __stdcall listen_worker( LPVOID param )
{

	ITZListen		*pClass = (ITZListen *)param;
	DWORD			dwRet;

	SI32			err;

	SOCKET			listenSocket = 0;
	SOCKET			newSocket = 0;

	SOCKADDR_IN		sockaddr;
	SI32			siAddrlen;

	siAddrlen = sizeof( SOCKADDR );

	WSANETWORKEVENTS NetworkEvents;	

	HANDLE hEvents[ 2 ];

	hEvents[ 0 ] = pClass->m_hActionEvent;
	hEvents[ 1 ] = WSACreateEvent();
	

	bool loop = true;
	while( loop ) {

		dwRet = WSAWaitForMultipleEvents( 2, hEvents, FALSE, WSA_INFINITE, FALSE );

		dwRet = dwRet - WSA_WAIT_EVENT_0;

		if( dwRet == 0 ) {
			ResetEvent( hEvents[ 0 ] );

			// Action Event 처리
			
			if( pClass->m_uiActionCode == ITZLISTEN_ACT_START ) {
				
				if( pClass->m_uiState == ITZLISTEN_STATE_RUN ) {
					continue;
				}

				if( pClass->m_uiState == ITZLISTEN_STATE_CLOSE ) {
					// 소켓이 닫힌 상태라면,

					listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

					if( listenSocket == INVALID_SOCKET ) {
						continue;
					}

					linger lg;

					lg.l_onoff = 1;
					lg.l_linger = 0;

					setsockopt( listenSocket, SOL_SOCKET, SO_LINGER, (char*)&lg, sizeof(lg) );


					err = bind( listenSocket, (SOCKADDR *)&pClass->m_sockaddr, sizeof( pClass->m_sockaddr ) );

					if( err == SOCKET_ERROR ) {

						closesocket( listenSocket );
						continue;
					}

					WSAEventSelect( listenSocket, hEvents[ 1 ], FD_ACCEPT );

					err = listen( listenSocket, 200 );

					if( err == SOCKET_ERROR ) {

						closesocket( listenSocket );

						continue;
					}

					pClass->m_uiState = ITZLISTEN_STATE_RUN;	

				} else if( pClass->m_uiState == ITZLISTEN_STATE_STOP ) {

					pClass->m_uiState = ITZLISTEN_STATE_RUN;	
				}


			} else if( pClass->m_uiActionCode == ITZLISTEN_ACT_STOP ) {

					pClass->m_uiState = ITZLISTEN_STATE_STOP;

			} else if( pClass->m_uiActionCode == ITZLISTEN_ACT_CLOSE ) {

					closesocket( listenSocket );

					listenSocket = INVALID_SOCKET;

					pClass->m_uiState = ITZLISTEN_STATE_CLOSE;
			
			} else if( pClass->m_uiActionCode == ITZLISTEN_ACT_EXIT ) {

				break;
			}

		} else if( dwRet == 1 ) {
			// 소켓 이벤트 처리
			WSAResetEvent( hEvents[ 1 ] );

			WSAEnumNetworkEvents( listenSocket, hEvents[ 1 ], &NetworkEvents );

			if( NetworkEvents.lNetworkEvents & FD_ACCEPT ) {

				if( NetworkEvents.iErrorCode[ FD_ACCEPT_BIT ] != 0 ) {

					continue;
				}

				newSocket = accept( listenSocket, (SOCKADDR *)&sockaddr, &siAddrlen );

				if( newSocket == INVALID_SOCKET ) {

					/*
					TCHAR buf[ 256 ];
					int err = GetLastError();
					sprintf( buf, TEXT("accept error: %d\n"), err );

					OutputDebugString( buf );
					*/
					continue;
				}

				if( pClass->m_uiState != ITZLISTEN_STATE_RUN ) {

					closesocket( newSocket );

					continue;
				}

				// 연결을 받아들인다				
				if( pClass->m_pSessionManager->NewSocket( newSocket, sockaddr ) == NULL ) {

					closesocket( newSocket );

					continue;

				}

			}

		}

	} // while( 1 )

	if( listenSocket != INVALID_SOCKET ) closesocket( listenSocket );
	WSACloseEvent( hEvents[ 1 ] );

	SetEvent( pClass->m_hExitEvent );

	return 0;
}



ITZListen::ITZListen()
{
	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );

	m_hActionEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_bInitOk = FALSE;

	m_uiState = 0;
	m_uiActionCode = 0;

}

ITZListen::~ITZListen()
{
	
	Destroy();

	CloseHandle( m_hActionEvent );

	CloseHandle( m_hExitEvent );
}


void ITZListen::CreateListen( ITZSessionManager *pssmgr, UI16 usPort ) 
{

	if( m_bInitOk ) return;

	m_pSessionManager = pssmgr;

	UI32 uiThreadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, listen_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );

	SetListenPort( usPort );

	m_uiState = ITZLISTEN_STATE_CLOSE;

	m_bInitOk = TRUE;
}

void ITZListen::Destroy()
{
	if( m_bInitOk ) {
		ResetEvent( m_hExitEvent );

		m_uiActionCode = ITZLISTEN_ACT_EXIT;
		SetEvent( m_hActionEvent );	

		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	m_bInitOk = FALSE;
}

void ITZListen::SetListenPort( UI16 usPort )
{

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	m_sockaddr.sin_port = htons( usPort );

	m_usPort = usPort;

}

void ITZListen::Start()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = ITZLISTEN_ACT_START;

	SetEvent( m_hActionEvent );

}

void ITZListen::Stop()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = ITZLISTEN_ACT_STOP;

	SetEvent( m_hActionEvent );
}

void ITZListen::Close()
{
	if( m_bInitOk == FALSE ) return;

	m_uiActionCode = ITZLISTEN_ACT_CLOSE;

	SetEvent( m_hActionEvent );

}

