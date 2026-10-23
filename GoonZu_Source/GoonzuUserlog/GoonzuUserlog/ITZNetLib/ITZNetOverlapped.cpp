#include "ITZNetOverlapped.h"
#include "ITZSendBuffer.h"
#include "ITZRecvBuffer.h"
#include "ITZSessionManager.h"


void CALLBACK send_completion( DWORD dwError, DWORD dwBytesTransferred, 
							  LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags )
{

	ITZSession	*pSession = (ITZSession *)lpOverlapped->hEvent;

	if( dwError !=0 || dwBytesTransferred == 0 ) {
		
		pSession->SetState( SESSION_STATE_DISCONNECTED );

		return;
	}
	
	pSession->GetSendBuffer()->Completion( dwBytesTransferred );

}

void CALLBACK recv_completion( DWORD dwError, DWORD dwBytesTransferred, 
							  LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags )
{

	WSABUF		wsabuf;

	ITZSession	*pSession = (ITZSession *)lpOverlapped->hEvent;

	if( dwError !=0 || dwBytesTransferred == 0 ) {
		
		pSession->SetState( SESSION_STATE_DISCONNECTED );

		return;
	}


	if( pSession->GetRecvBuffer()->Completion( dwBytesTransferred ) == FALSE ) {

		pSession->SetState( SESSION_STATE_DISCONNECTED );

		return;
	}

	// ´ÙÀ½ Recv
	pSession->GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

	ZeroMemory( &pSession->m_recvIoData, sizeof( PER_IO_DATA ) );

	pSession->m_recvIoData.dwOperationType = RECV_POSTED;

	pSession->m_recvIoData.overlapped.hEvent = pSession;

	int ret = WSARecv( pSession->GetSocket(), &wsabuf, 1, 
		&pSession->m_recvIoData.dwIoSize, 
		&pSession->m_recvIoData.dwFlags,
		&pSession->m_recvIoData.overlapped, recv_completion );

	if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

		pSession->SetState( SESSION_STATE_DISCONNECTED );

		return;
	}			

}

unsigned __stdcall overlapped_worker( LPVOID param )
{

	ITZNetOverlapped	*pClass = (ITZNetOverlapped *)param;
	
	WSABUF	wsabuf;

	ITZObjectPtr *pObjectPtr;
	ITZSession *pSession;

	int ret, err;

	BOOL bClosing = FALSE;

	while( 1 ) {
		
		SleepEx( 10, TRUE );

		if( pClass->m_bRun == FALSE ) bClosing = TRUE;

		LIST_LOOP( pClass->m_pSessionManager->m_pSessionPtrList, pObjectPtr ) 
		{
			pSession = (ITZSession *)( pObjectPtr->GetObject() );

			if( bClosing == TRUE ) {
				pSession->CloseSocket();
				continue;
			}

			if( pSession->GetState() == SESSION_STATE_NONE ) continue;

			if( pSession->GetState() == SESSION_STATE_CONNECTED ) {
			
				pClass->_beginSession( pSession );
				continue;
			
			} else if( pSession->GetState() == SESSION_STATE_DISCONNECTED ) {

				pSession->SetState( SESSION_STATE_NONE );
				
				pClass->m_pSessionManager->DelSession( pSession );
				continue;

			} else if( pSession->GetState() != SESSION_STATE_ESTABLISHED ||
				
				pSession->GetSendBuffer()->IsReadyToSend() == FALSE ) continue;
	
			if( pSession->GetSendBuffer()->GetSendParam( &wsabuf.buf, (SI32 *)&wsabuf.len ) == FALSE ) continue;

			ZeroMemory( &pSession->m_sendIoData, sizeof( PER_IO_DATA ) );

			pSession->m_sendIoData.dwOperationType = SEND_POSTED;

			pSession->m_sendIoData.overlapped.hEvent = pSession;

			ret = WSASend( pSession->GetSocket(), &wsabuf, 1, 
				&pSession->m_sendIoData.dwIoSize, 
				pSession->m_sendIoData.dwFlags,
				&pSession->m_sendIoData.overlapped, send_completion );

			if( ret == SOCKET_ERROR ) {

				err = WSAGetLastError();

				if( err != ERROR_IO_PENDING ) {
				
					pSession->SetState( SESSION_STATE_DISCONNECTED );
				}
			}

		} // list loop

		if( pClass->m_bRun == FALSE && bClosing == TRUE ) break;

	} // while 

	SetEvent( pClass->m_hExitEvent );

	return 0;
}


ITZNetOverlapped::ITZNetOverlapped()
{

	m_bRun = FALSE;

	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );
	
}

ITZNetOverlapped::~ITZNetOverlapped()
{

	if( m_bRun ) {
		ResetEvent( m_hExitEvent );
		InterlockedExchange( (LONG*)&m_bRun, FALSE );
		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	CloseHandle( m_hExitEvent );
}

void ITZNetOverlapped::CreateOverlapped( ITZSessionManager *pssmgr )
{

	int maxsize = pssmgr->m_siMaxSize;

	m_pSessionManager = pssmgr;

	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, overlapped_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	

}

void ITZNetOverlapped::_beginSession( ITZSession *pSession )
{

	WSABUF	wsabuf;
	int		ret;

	pSession->GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

	ZeroMemory( &pSession->m_recvIoData, sizeof( PER_IO_DATA ) );

	pSession->m_recvIoData.dwOperationType = RECV_POSTED;

	pSession->m_recvIoData.overlapped.hEvent = pSession;

	ret = WSARecv( pSession->GetSocket(), &wsabuf, 1, 
		&pSession->m_recvIoData.dwIoSize, 
		&pSession->m_recvIoData.dwFlags, 
		&pSession->m_recvIoData.overlapped, recv_completion ); 

	if( ret == SOCKET_ERROR && ( WSAGetLastError() != ERROR_IO_PENDING ) ) {

		pSession->SetState( SESSION_STATE_DISCONNECTED );
		return;
	}

	pSession->SetState( SESSION_STATE_ESTABLISHED );
}

