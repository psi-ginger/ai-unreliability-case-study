#include "ITZPerson.h"
#include "ITZSession.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include "ITZEncrypt.h"

ITZSession::ITZSession()
{
	m_dwCloseTick = 0;

	m_pEncrypt = new ITZEncrypt;

	m_pRecvBuffer = NULL;
	m_pSendBuffer = NULL;

	m_socket = INVALID_SOCKET;

	m_szIP[ 0 ] = NULL;

	m_siState = SESSION_STATE_NONE;

	m_bAttempting = FALSE;

	InitializeCriticalSection( &m_cs );
}

ITZSession::~ITZSession()
{

	CloseSocket();

	delete m_pRecvBuffer;
	m_pRecvBuffer = NULL;

	delete m_pSendBuffer;	
	m_pSendBuffer = NULL;

	delete m_pEncrypt;
	m_pEncrypt = NULL;

	DeleteCriticalSection( &m_cs );
}

void ITZSession::Init()
{
	EnterCriticalSection( &m_cs );

	m_bAttempting = FALSE;

	if( m_pPerson ) m_pPerson->Init();

	LeaveCriticalSection( &m_cs );
}

void ITZSession::Close()
{
	EnterCriticalSection( &m_cs );

	if( m_pPerson ) m_pPerson->Close();

	LeaveCriticalSection( &m_cs );
}

void ITZSession::SetBuffer( SI32 siRecvBufSize, SI32 siSendBufSize, SI32 headerType )
{
	
	delete m_pRecvBuffer;
	delete m_pSendBuffer;

	m_pRecvBuffer = new ITZRecvBuffer;
	m_pSendBuffer = new ITZSendBuffer;

	m_pRecvBuffer->CreateRecvBuffer( siRecvBufSize, headerType );
	m_pSendBuffer->CreateSendBuffer( siSendBufSize, headerType );

}


BOOL ITZSession::InitConnect( char *ip, UI16 usPort )
{

	EnterCriticalSection( &m_cs );

	if( m_bAttempting == TRUE ) {
		
		LeaveCriticalSection( &m_cs );
		return TRUE;
	}

	if( m_siState != SESSION_STATE_NONE ) {

		_closesocket();
		LeaveCriticalSection( &m_cs );

		return FALSE;
	}
	
	strcpy( m_szIP, ip );

	m_usPort = usPort;

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;
	
	if(isalpha( m_szIP[0] )) {   /* server address is a name */

		struct hostent *hp;

		hp = gethostbyname( m_szIP );

		if (hp == NULL) {
			
			LeaveCriticalSection( &m_cs );
			return FALSE;
		}

		//m_sockaddr.sin_addr.s_addr = hp->h_addr;

		memcpy( &m_sockaddr.sin_addr.s_addr, hp->h_addr, hp->h_length );

	} else { 

		m_sockaddr.sin_addr.s_addr = inet_addr( m_szIP );
	} 

	m_sockaddr.sin_port = htons( m_usPort );

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

void ITZSession::SetState( SI32 state )
{
	if( state == SESSION_STATE_DISCONNECTED ) {

		if(	m_siState == SESSION_STATE_NONE ) return;
	}

	InterlockedExchange( (LPLONG)&m_siState, (LONG)state );

	return;
}


BOOL ITZSession::SetSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{
	EnterCriticalSection( &m_cs );

	BOOL ret = FALSE;

	if( m_siState == SESSION_STATE_NONE ) {

		m_socket = socket;
		m_sockaddr = sockaddr;

		memcpy( m_szIP, inet_ntoa( sockaddr.sin_addr ), 20 );
		m_szIP[ 19 ] = NULL;

		m_pRecvBuffer->ClearBuffer();
		m_pSendBuffer->ClearBuffer();

		SetState( SESSION_STATE_CONNECTED );

		ret = TRUE;
	}

	LeaveCriticalSection( &m_cs );

	return ret;
}

BOOL ITZSession::CloseSocket()
{
	EnterCriticalSection( &m_cs );

	BOOL ret = _closesocket();

	LeaveCriticalSection( &m_cs );

	return ret;
}


BOOL ITZSession::_connect()
{	
	EnterCriticalSection( &m_cs );

	int err;
	BOOL ret = FALSE;

	if( abs( (int)(GetTickCount() - m_dwCloseTick) ) < 600 ) {
		goto end;
	}
	
	unsigned long ul;

	if( m_socket == INVALID_SOCKET ) {

		m_socket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	}

	if( m_socket == INVALID_SOCKET ) {
		goto end;
	}

	if( m_bAttempting == FALSE ) {

		ul = 1;
		ioctlsocket( m_socket, FIONBIO, (unsigned long *)&ul );

		m_dwConnectTick = GetTickCount();

		m_bAttempting = TRUE;
	}

	if( GetTickCount() - m_dwConnectTick >= 7000 ) {

		_closesocket();
		goto end;

	}

	err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {
		int lasterr = WSAGetLastError();

		char buf[ 256 ];

		sprintf( buf, "%d\n", lasterr );

		OutputDebugString( buf );
		
		if( lasterr == 10035 || lasterr == 10037 || lasterr == 10022 ) {
			goto end;
		}

		if( lasterr != 10056 ) {

			_closesocket();
			goto end;
		}
	}

	ul = 0;
	ioctlsocket( m_socket, FIONBIO, (unsigned long *)&ul );

	m_bAttempting = FALSE;

	ret = TRUE;

end:
	LeaveCriticalSection( &m_cs );

	return ret;
}

BOOL ITZSession::_closesocket()
{
	if( m_socket != INVALID_SOCKET ) {

		if( m_bAttempting == TRUE ) {
	
			m_bAttempting = FALSE;

			unsigned long ul = 0;
			int ret = ioctlsocket( m_socket, FIONBIO, (unsigned long *)&ul );

			if( ret == SOCKET_ERROR ) {
				MessageBox( NULL, "ERROR", 0, 0 );
			}
		}

		int err = closesocket( m_socket );

		if( err == SOCKET_ERROR ) {
			int lasterr = WSAGetLastError();

			printf( "close socket err(%d)\n", lasterr );

		} else {
			printf( "close socket\n" );
		}

		m_socket = INVALID_SOCKET;

		m_pRecvBuffer->ClearBuffer();
		m_pSendBuffer->ClearBuffer();

		m_dwCloseTick = GetTickCount();

		if( m_siState != SESSION_STATE_NONE )
			SetState( SESSION_STATE_DISCONNECTED );

		return TRUE;
	}

	return FALSE;
}
