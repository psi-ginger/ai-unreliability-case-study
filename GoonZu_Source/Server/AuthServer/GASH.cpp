#include "AccountDBProcess.h"
#include "AuthFrontProtocol.h"
#include "GASH.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include "AuthMsg.h"
#include <stdio.h>
#include <time.h>
#include "UserManager.h"

unsigned __stdcall gash_worker( LPVOID param )
{
	CGash	*pClass = (CGash *)param;
	char buf[ 1024 ];

	while( pClass->m_bRun ) {

		Sleep( 10 );

		// 슬립 뒤에 한번 더 검사
		if(pClass->m_bRun == FALSE)		break;

		EnterCriticalSection( &pClass->m_cs );

		SI32 msgcount = pClass->m_siCount;

		LeaveCriticalSection( &pClass->m_cs );

		if( msgcount > 0 ) {
			
			UI16 port = 0;
			
			gashmsginfo *pInfo = &pClass->m_pGashMsgInfo[ pClass->m_siStartIndex ];

			switch( pInfo->m_requestCode )
			{
			case REQUESTCODE_AUTH:
				port = 5411;
				break;

			case REQUESTCODE_LOGIN:
				port = 5412;
				break;

			case REQUESTCODE_LOGOUT:
				port = 5413;
				break;
			}

			// gash 서버로 접속 시도
			if( pClass->Connect( port ) == FALSE ) {

				sprintf( buf, "Connect Failed..(IP:%s,PORT:%d)", pClass->m_szIP, port );
				pClass->WriteLogFile( buf );

				EnterCriticalSection( &pClass->m_cs );

				if( pClass->m_siCount > 0 ) {

					pClass->m_siCount--;

					SI32 workindex = pClass->m_siStartIndex;

					workindex++;

					if( workindex == pClass->m_siMaxList ) workindex = 0;

					pClass->m_siStartIndex = workindex;					


				}
				LeaveCriticalSection( &pClass->m_cs );

				continue;
			} else {
				
				sprintf( buf, "Connect Success..(IP:%s,PORT:%d)", pClass->m_szIP, port );
				pClass->WriteLogFile( buf );
			}

			gashmsginfo *pMsgInfo = &pClass->m_pGashMsgInfo[ pClass->m_siStartIndex ];

			if( pClass->RequestMsg( pMsgInfo ) == FALSE ) continue;

			pClass->ResponseMsg( pMsgInfo );
		}
	}

	return 0;
}

CGash::CGash()
{
	m_socket = INVALID_SOCKET;
	m_szIP[ 0 ] = NULL;

	m_pGashMsgInfo = NULL;
	m_siMaxList = 0;
	m_siCount = 0;

	m_hThread = INVALID_HANDLE_VALUE;

	InitializeCriticalSection( &m_cs );
}

CGash::~CGash()
{
	m_bRun = FALSE;

	// 잠시 기다려준다.
	//Sleep( 1000 );
	DWORD dwWaitObject = WaitForSingleObject(m_hThread, 2 * 1000);		// 최대 2초 동안 기다린다.

	if( dwWaitObject == WAIT_TIMEOUT)
        TerminateThread( m_hThread , 1000);

	CloseHandle( m_hThread );	
	DeleteCriticalSection( &m_cs );
	if( m_pGashMsgInfo ) delete [] m_pGashMsgInfo;
}

void CGash::Init( SI32 maxlist, char *szIP )
{
	if( m_pGashMsgInfo ) delete [] m_pGashMsgInfo;

	m_pGashMsgInfo = new gashmsginfo[ maxlist ];

	m_siMaxList = maxlist;
	m_siCount = 0;

	memcpy( m_szIP, szIP, 64 );
	m_szIP[ 63 ] = NULL;

	//m_usPort = port;

	m_siStartIndex = 0;
	m_siEndIndex = 0;
}



void CGash::CreateThread()
{
//	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	m_hThread = (HANDLE) _beginthreadex( NULL, 0, gash_worker, (LPVOID)this, 0, &uiThreadID );
//	CloseHandle( hThread );	
}

BOOL CGash::Connect( UI16 port )
{
	if( m_socket != INVALID_SOCKET ) {

		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
	}

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;

	if( isalpha( m_szIP[0] ) ) {   /* server address is a name */

		struct hostent *hp;

		hp = gethostbyname( m_szIP );

		if (hp == NULL) {
			return FALSE;
		}

		//m_sockaddr.sin_addr.s_addr = hp->h_addr;

		memcpy( &m_sockaddr.sin_addr.s_addr, hp->h_addr, hp->h_length );

	} else { 

		m_sockaddr.sin_addr.s_addr = inet_addr( m_szIP );
	} 


	m_sockaddr.sin_port = htons( port );

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET ) {
		printf( "socket error!\n" );
		return FALSE;
	}

	int err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {

		printf( "connect failed!\n" );

		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	FD_ZERO( &m_fdset );
	FD_SET( m_socket, &m_fdset );

	printf( "connect success!\n" );	

	return TRUE;
}

void CGash::CloseSocket()
{
	if( m_socket == INVALID_SOCKET ) return;

	closesocket( m_socket );
	m_socket = INVALID_SOCKET;

	printf( "close socket!\n" );

	EnterCriticalSection( &m_cs );

	if( m_siCount > 0 ) {

		m_siCount--;

		SI32 workindex = m_siStartIndex;

		workindex++;

		if( workindex == m_siMaxList ) workindex = 0;

		m_siStartIndex = workindex;
	}

	LeaveCriticalSection( &m_cs );
}

BOOL CGash::AddRequestInfo( SI16 requestcode, UI16 packetUnique, UI16 charID, char *szID, char *szPW, char *szIP, BYTE *pdata, int datasize )
{
	EnterCriticalSection( &m_cs );

	if( m_siCount == m_siMaxList ) {
		LeaveCriticalSection( &m_cs );
		return FALSE;
	}


	SI32 workindex = m_siEndIndex;

	gashmsginfo *pInfo = &m_pGashMsgInfo[ workindex ];

	pInfo->m_requestCode = requestcode;

	pInfo->m_usPacketUnique = packetUnique;
	pInfo->m_usCharID = charID;

	memcpy( pInfo->m_szID, szID, 20 );
	pInfo->m_szID[ 19 ] = NULL;

	memcpy( pInfo->m_szPW, szPW, 20 );
	pInfo->m_szPW[ 19 ] = NULL;

	memcpy( pInfo->m_szIP, szIP, 20 );
	pInfo->m_szIP[ 19 ] = NULL;

	if( pdata && datasize ) {
		memcpy( pInfo->cData, pdata, datasize );
	}

	pInfo->m_bValid = TRUE;

	workindex++;

	if( workindex == m_siMaxList ) workindex = 0;

	m_siEndIndex = workindex;

	m_siCount++;

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

BOOL CGash::RequestMsg( gashmsginfo *pInfo )
{
	
	char buf[ 4096 ];
	char buf2[ 4096 ];

	buf[ 0 ] = NULL;

	switch( pInfo->m_requestCode )
	{
	case REQUESTCODE_AUTH:
		{
			sprintf( buf, "%s'%s'%s'%s'%s'%s'\r\n", 
				"610074", "J6", 
				pInfo->m_szID, 
				pInfo->m_szPW,
				"1",
				pInfo->m_szIP
				);
		}
		break;

	case REQUESTCODE_LOGIN:
		{
			sprintf( buf, "%s'%s'%s'%s'%s'%s'%s'%s'\r\n", 
				"610074", "J6", 
				pInfo->m_szID, 
				pInfo->m_szPW,
				"1",
				"I",
				"0",
				pInfo->m_szIP
				);
		}
		break;

	case REQUESTCODE_LOGOUT:
		{
			sprintf( buf, "%s'%s'%s'%s'%s'%s'%s'\r\n", 
				"610074", "J6", 
				pInfo->m_szID, 
				"R",
				"",
				"",
				pInfo->m_szIP
				);
		}
		break;
	}


	// send
	int len = strlen( buf );
	int sendbytes;

	if( len > 0 ) {
		sendbytes = send( m_socket, buf, len, 0 );
	}

	sprintf( buf2, "Send String: %s", buf );
	int l = strlen( buf2 );
	buf2[ l - 2 ] = NULL;
	printf( "%s\n", buf2 );
	WriteLogFile( buf2 );


	if( sendbytes < len ) {
		sprintf( buf2, "Send Error!\n" );
		printf( "%s\n", buf2 );
		WriteLogFile( buf2 );
		CloseSocket();
		return FALSE;
	}

	return TRUE;
}

void CGash::ResponseMsg( gashmsginfo *pInfo )
{

	char buf[ 4096 ];

	fd_set	readfds;
	timeval	timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	DWORD tick = GetTickCount();

	// 응답 대기
	while( 1 ) {

		Sleep( 10 );

		readfds = m_fdset;

		select( 0, &readfds, 0, 0, &timeout );

		if( FD_ISSET( m_socket, &readfds ) ) {

			m_recvbytes = recv( m_socket, buf, 4096, 0 );

			memcpy( m_recvbuf, buf, m_recvbytes );

			if( m_recvbytes <= 0 ) {
				sprintf( buf, "Recv Error!" );
				printf( "%s\n", buf );
				WriteLogFile( buf );
				CloseSocket();
				return;

			} 

			m_recvbuf[ m_recvbytes ] = NULL;

			if( m_recvbytes > 2 )
			{
				m_recvbuf[ m_recvbytes - 2 ] = NULL;
			}
			
			sprintf( buf, "Recv String: %s[ %d ]", m_recvbuf, GetTickCount() - tick );
			printf( "%s\n", buf );
			WriteLogFile( buf );


			switch( pInfo->m_requestCode )
			{
			case REQUESTCODE_AUTH:
				{

					if( m_recvbuf[ 0 ] == '1' ) {
						pInfo->response_frontauth.usResult = 0;

						g_usermanager.FrontAuth( 
							pInfo->m_szID,
							pInfo->m_szPW,
							pInfo->m_szIP,
							pInfo->response_frontauth.siAuthKey,
							2 );

					} else {
						pInfo->response_frontauth.usResult = 11;
					}

					ITZSession *pSession = g_afp.GetSessionManager()->GetSession(0);

					if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&pInfo->response_frontauth );	
				}
				break;

			case REQUESTCODE_LOGIN:
				{

				}
				break;

			case REQUESTCODE_LOGOUT:
				{

				}
				break;
			}

			CloseSocket();
			return;
				
		}
	}

}

void CGash::WriteLogFile( char *szLog ) 
{
	
	_SYSTEMTIME time;

	GetLocalTime( &time );

	char filename[ 128 ];
	char fileBuf[ 1024 ];

	sprintf( filename, "GASH_%02d-%02d.txt", time.wMonth, time.wDay );

	FILE *fp = fopen( filename, "a+t" );
	fseek( fp, 0, SEEK_END );

	wsprintf( fileBuf,"[%02d-%02d-%02d:%02d:%02d.%03d] %s\r\n", 
		time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLog );

	fwrite( fileBuf, sizeof( char ), strlen( fileBuf ), fp );

	fclose(fp);

}