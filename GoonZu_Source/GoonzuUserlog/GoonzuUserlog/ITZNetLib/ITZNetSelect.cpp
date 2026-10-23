#include "ITZNetSelect.h"
#include "ITZSendBuffer.h"
#include "ITZRecvBuffer.h"
#include "ITZSessionManager.h"
#include <stdlib.h>


struct WSABUFCE
{
	char			*buf;
	unsigned long	len;
};


unsigned __stdcall select_worker( LPVOID param )
{

	fd_set	readfds, writefds, execeptfds;

	ITZNetSelect	*pClass = (ITZNetSelect *)param;
	
	WSABUFCE	wsabuf;
	
	ITZObjectPtr *pObjectPtr;
	ITZSession *pSession;
	
	int ret, err;
	
	BOOL bClosing = FALSE;
	
	while( 1 ) {
		
		Sleep( 20 );
	
		if( pClass->m_bRun == FALSE ) bClosing = TRUE;

		timeval	timeout;

		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		readfds = pClass->m_fdset;
		writefds = pClass->m_fdset;
		execeptfds = pClass->m_fdset;

		select( 0, &readfds, &writefds, &execeptfds, &timeout );
	
		LIST_LOOP( pClass->m_pSessionManager->m_pSessionPtrList, pObjectPtr ) 
		{
			pSession = (ITZSession *)( pObjectPtr->GetObject() );
			
			if( bClosing == TRUE ) {
				pSession->CloseSocket();
				continue;
			}

			if( pSession->GetState() == SESSION_STATE_CONNECTED ) {
			
				pClass->_beginSession( pSession );
				continue;
			
			} else if( pSession->GetState() == SESSION_STATE_DISCONNECTED ) {
				
				pClass->m_pSessionManager->DelSession( pSession );
				continue;
	
			} else if( pSession->GetState() != SESSION_STATE_ESTABLISHED ) continue;


			if( FD_ISSET( pSession->GetSocket(), &readfds ) ){

				pSession->GetRecvBuffer()->GetRecvParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

				int recvbytes = recv( pSession->GetSocket(), wsabuf.buf, wsabuf.len, 0 );

				pSession->GetRecvBuffer()->Completion( recvbytes );
			}

			if( FD_ISSET( pSession->GetSocket(), &writefds ) ){

				pSession->GetSendBuffer()->GetSendParam( &wsabuf.buf, (SI32 *)&wsabuf.len );

				int sendbytes = send( pSession->GetSocket(), wsabuf.buf, wsabuf.len, 0 );

				pSession->GetSendBuffer()->Completion( sendbytes );
			}


		} // list loop

		if( pClass->m_bRun == FALSE && bClosing == TRUE ) break;

	} // while 

	SetEvent( pClass->m_hExitEvent );

	return 0;

}

ITZNetSelect::ITZNetSelect()
{

	m_bRun = FALSE;

	m_hExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );
	
}

ITZNetSelect::~ITZNetSelect()
{

	if( m_bRun ) {
		ResetEvent( m_hExitEvent );
		m_bRun = FALSE;
		WaitForSingleObject( m_hExitEvent, INFINITE );	
	}

	CloseHandle( m_hExitEvent );
}

void ITZNetSelect::CreateSelect( ITZSessionManager *pssmgr )
{

	int maxsize = pssmgr->m_siMaxSize;

	m_pSessionManager = pssmgr;

	FD_ZERO( &m_fdset );

	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, select_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	

}

void ITZNetSelect::_beginSession( ITZSession *pSession )
{

	FD_SET( pSession->GetSocket(), &m_fdset );

	pSession->SetState( SESSION_STATE_ESTABLISHED );
}

