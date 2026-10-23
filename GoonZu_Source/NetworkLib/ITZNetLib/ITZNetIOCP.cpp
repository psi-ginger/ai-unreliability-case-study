#include "ITZNetIOCP.h"
#include "ITZSendBuffer.h"
#include "ITZRecvBuffer.h"
#include "ITZSessionManager.h"

unsigned __stdcall iocp_worker( LPVOID param )
{
	ITZNetIOCP	*pClass = (ITZNetIOCP *)param;
	
	WSABUF	wsabuf;

	ITZObjectPtr *pObjectPtr;
	ITZSession *pSession;

	int ret;

	BOOL bClosing = FALSE;

	DWORD tick;

	bool loop = true;
	while( loop ) {
		
		Sleep( 20 );

		if( pClass->m_bRun == FALSE ) bClosing = TRUE;

		tick = GetTickCount();

		InterlockedExchange( (LPLONG)&pClass->m_dwLastTick, tick );

		LIST_LOOP( pClass->m_pSessionManager->m_pSessionPtrList, pObjectPtr ) 
		{
			pSession = (ITZSession *)( pObjectPtr->GetObject() );

			if( bClosing == TRUE ) {
				pSession->CloseSocket();
				continue;
			}

			if( pSession->IsActive() && pSession->GetState() != SESSION_STATE_ESTABLISHED )
			{
				if( tick - pSession->m_dwEstablishedTick >= 10 * 1000 )
				{
					pSession->SetState( SESSION_STATE_NONE );
					pClass->m_pSessionManager->DelSession( pSession );
					continue;
				}
			}

			if( pSession->GetState() == SESSION_STATE_CONNECTED ) {
			
				pClass->_beginSession( pSession );
				continue;
			
			} else if( pSession->GetState() == SESSION_STATE_DISCONNECTED ) {
				
				pSession->SetState( SESSION_STATE_NONE );
				
				pClass->m_pSessionManager->DelSession( pSession );

				continue;

			} else if( pSession->GetState() != SESSION_STATE_ESTABLISHED ||
				
				pSession->GetSendBuffer()->IsReadyToSend() == FALSE ) continue;
	
			if( pSession->GetSendBuffer()->GetSendParam( (BYTE**)&wsabuf.buf, (SI32 *)&wsabuf.len ) == FALSE ) continue;

			ZeroMemory( &pSession->m_sendIoData, sizeof( PER_IO_DATA ) );

			pSession->m_sendIoData.dwOperationType = SEND_POSTED;

			ret = WSASend( pSession->GetSocket(), &wsabuf, 1, 
				&pSession->m_sendIoData.dwIoSize, 
				pSession->m_sendIoData.dwFlags,
				&pSession->m_sendIoData.overlapped, NULL );

			if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {
				
			//tsoi 	printf(  TEXT("error\n") );

				pSession->SetState( SESSION_STATE_DISCONNECTED );
			}

		} // list loop

		if( pClass->m_bRun == FALSE && bClosing == TRUE ) break;

	} // while 

	PostQueuedCompletionStatus( pClass->m_hIOCP, 0, 0, 0 );

	return 100;
}

unsigned __stdcall iocp_worker_completion( LPVOID param )
{

	ITZNetIOCP		*pClass = (ITZNetIOCP *)param;

	BOOL			bSuccess = FALSE;
	DWORD			dwError;
	DWORD			dwIoSize = 0;

	ITZSession		*pSession = NULL;
	PER_IO_DATA		*pPerIoData = NULL;
	
	WSABUF			wsabuf;

	int ret;

	bool loop = true;
	while( loop ) {


		bSuccess = GetQueuedCompletionStatus( pClass->m_hIOCP, &dwIoSize, (LPDWORD)&pSession, (LPOVERLAPPED *)&pPerIoData, INFINITE );
		
		if( !bSuccess ) {
			dwError = GetLastError();

			if( dwError == 64 && pSession != NULL ) {
				// 소켓이 이미 닫힌 경우
				
				pSession->SetState( SESSION_STATE_DISCONNECTED );

			}
			continue;
		}

		// 스레드의 강제 종료 명령이 내려진 경우
		if( pSession == NULL ) break;

		// 클라이언트와의 소켓 연결이 끊어졌으면,
		if( dwIoSize == 0 ) {
			
			pSession->SetState( SESSION_STATE_DISCONNECTED );
			
			continue;
		}

		switch( pPerIoData->dwOperationType ) {
		case RECV_POSTED:
			
			if( pSession->GetRecvBuffer()->Completion( dwIoSize ) == FALSE ) {

				pSession->SetState( SESSION_STATE_DISCONNECTED );

				continue;
			}

			// 다음 Recv

			pSession->GetRecvBuffer()->GetRecvParam( (BYTE**)&wsabuf.buf, (SI32 *)&wsabuf.len );

			ZeroMemory( &pSession->m_recvIoData, sizeof( PER_IO_DATA ) );

			pSession->m_recvIoData.dwOperationType = RECV_POSTED;

			ret = WSARecv( pSession->GetSocket(), &wsabuf, 1, 
				&pSession->m_recvIoData.dwIoSize, 
				&pSession->m_recvIoData.dwFlags,
				&pSession->m_recvIoData.overlapped, NULL );

			if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

				pSession->SetState( SESSION_STATE_DISCONNECTED );
			}			

			break;

		case SEND_POSTED:

			pSession->GetSendBuffer()->Completion( dwIoSize );

			break;

		} //switch


	} //while

	SetEvent( pClass->m_hExitEvent );

	return 101;

}



ITZNetIOCP::ITZNetIOCP()
{
	m_hIOCP = NULL;

	m_bRun = FALSE;

	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );
	
}

ITZNetIOCP::~ITZNetIOCP()
{

	Destroy();

	CloseHandle( m_hExitEvent );

}

void ITZNetIOCP::CreateIOCP( ITZSessionManager *pssmgr, int nThread )
{
	if( m_bRun == TRUE ) return;

	int maxsize = pssmgr->m_siMaxSize;
	int i;

	m_pSessionManager = pssmgr;

	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	HANDLE hThread;
	UI32 uiThreadID;

	for( i = 0; i < nThread; ++i ) {
		hThread = (HANDLE) _beginthreadex( NULL, 0, iocp_worker_completion, (LPVOID)this, 0, &uiThreadID );
		CloseHandle( hThread );	
	}

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, iocp_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	
}

void ITZNetIOCP::Destroy()
{
	if( m_bRun ) {
		ResetEvent( m_hExitEvent );
		m_bRun = FALSE;
		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	if( m_hIOCP ) CloseHandle( m_hIOCP );

	m_hIOCP = NULL;
}

void ITZNetIOCP::_beginSession( ITZSession *pSession )
{

	WSABUF	wsabuf;

	HANDLE hiocp = CreateIoCompletionPort( (HANDLE) pSession->GetSocket(), m_hIOCP, (DWORD)pSession, 0 );

	if( hiocp == NULL ) {

		int err = GetLastError();

		pSession->SetState( SESSION_STATE_DISCONNECTED );
		return;
	}

	pSession->GetRecvBuffer()->GetRecvParam( (BYTE**)&wsabuf.buf, (SI32 *)&wsabuf.len );

	ZeroMemory( &pSession->m_recvIoData, sizeof( PER_IO_DATA ) );

	pSession->m_recvIoData.dwOperationType = RECV_POSTED;

	int ret = WSARecv( pSession->GetSocket(), &wsabuf, 1, 
		&pSession->m_recvIoData.dwIoSize, 
		&pSession->m_recvIoData.dwFlags, 
		&pSession->m_recvIoData.overlapped, NULL ); 

	if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

		pSession->SetState( SESSION_STATE_DISCONNECTED );
		return;
	}

	pSession->SetState( SESSION_STATE_ESTABLISHED );
}

