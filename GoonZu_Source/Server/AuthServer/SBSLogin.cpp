#include "AccountDBProcess.h"
#include "AuthFrontProtocol.h"
#include "SBSLogin.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include <stdio.h>
#include <time.h>


SI32 SBSLogin::m_siActiveIP;

unsigned __stdcall sbs_worker( LPVOID param )
{
	SBSLogin	*pClass = (SBSLogin *)param;
	char buf[ 1024 ];

	while( pClass->m_bRun ) {

		Sleep( 10 );

		if( pClass->m_siCount > 0 ) {

			// sbs 서버로 접속 시도
			SI32 ip = pClass->m_siActiveIP;

			if( pClass->Connect() == FALSE ) {

				sprintf( buf, "Connect Failed..(ip:%s)", pClass->m_szIP[ ip ] );
				pClass->WriteLogFile( buf );

				if( pClass->m_siCount > 0 ) {

					InterlockedDecrement( (LPLONG)&pClass->m_siCount );

					SI32 workindex = pClass->m_siStartIndex;

					workindex++;

					if( workindex == pClass->m_siMaxList ) workindex = 0;

					InterlockedExchange( (LPLONG)&pClass->m_siStartIndex, workindex );

				}
				continue;
			} else {
				sprintf( buf, "Connect Success..(ip:%s)", pClass->m_szIP[ ip ] );
				pClass->WriteLogFile( buf );
			}

			if( pClass->RequestAuth() == FALSE ) continue;

			pClass->ResponseAuth();

		}
	}

	return 0;
}

SBSLogin::SBSLogin()
{
	m_socket = INVALID_SOCKET;
	m_szIP[ 0 ][ 0 ] = NULL;
	m_szIP[ 1 ][ 0 ] = NULL;

	m_siActiveIP = 0;

	m_pLoginInfo = NULL;
	m_siMaxList = 0;
	m_siCount = 0;
}

SBSLogin::~SBSLogin()
{
	m_bRun = FALSE;

	Sleep( 1000 );

	if( m_pLoginInfo ) delete [] m_pLoginInfo;
}

void SBSLogin::Init( SI32 maxlist, char *szIP, char *szIP2, UI16 port )
{
	if( m_pLoginInfo ) delete [] m_pLoginInfo;

	m_pLoginInfo = new sbslogininfo[ maxlist ];

	m_siMaxList = maxlist;
	m_siCount = 0;

	memcpy( m_szIP[ 0 ], szIP, 20 );
	m_szIP[ 0 ][ 19 ] = NULL;

	memcpy( m_szIP[ 1 ], szIP2, 20 );
	m_szIP[ 1 ][ 19 ] = NULL;
	
	m_usPort = port;

	m_siStartIndex = 0;
	m_siEndIndex = 0;

	m_siActiveIP = 0;
}



void SBSLogin::CreateThread()
{

	HANDLE hThread;
	UI32 uiThreadID;

	m_bRun = TRUE;

	hThread = (HANDLE) _beginthreadex( NULL, 0, sbs_worker, (LPVOID)this, 0, &uiThreadID );
	CloseHandle( hThread );	
}

BOOL SBSLogin::Connect()
{
	if( m_socket != INVALID_SOCKET ) {
		
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
	}

	memset( &m_sockaddr, 0, sizeof( m_sockaddr ) );

	m_sockaddr.sin_family = AF_INET;

	if(isalpha( m_szIP[ m_siActiveIP ][0] )) {   /* server address is a name */

		struct hostent *hp;

		hp = gethostbyname( m_szIP[ m_siActiveIP ] );

		if (hp == NULL) {
			return FALSE;
		}

		//m_sockaddr.sin_addr.s_addr = hp->h_addr;

		memcpy( &m_sockaddr.sin_addr.s_addr, hp->h_addr, hp->h_length );

	} else { 

		m_sockaddr.sin_addr.s_addr = inet_addr( m_szIP[ m_siActiveIP ] );
	} 

	
	m_sockaddr.sin_port = htons( m_usPort );

	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	if( m_socket == INVALID_SOCKET ) {
		printf( "socket error!\n" );
		return FALSE;
	}

	int err = connect( m_socket, (SOCKADDR *)&m_sockaddr, sizeof( m_sockaddr ) );

	if( err == SOCKET_ERROR ) {

		printf( "connect failed!\n" );

		m_siActiveIP = 1 - m_siActiveIP;

		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	FD_ZERO( &m_fdset );
	FD_SET( m_socket, &m_fdset );

	printf( "connect success!\n" );	

	return TRUE;
}

void SBSLogin::CloseSocket()
{
	if( m_socket == INVALID_SOCKET ) return;
	
	closesocket( m_socket );
	m_socket = INVALID_SOCKET;

	printf( "close socket!\n" );

	if( m_siCount > 0 ) {

		InterlockedDecrement( (LPLONG)&m_siCount );

		SI32 workindex = m_siStartIndex;

		workindex++;

		if( workindex == m_siMaxList ) workindex = 0;

		InterlockedExchange( (LPLONG)&m_siStartIndex, workindex );

	}
}

BOOL SBSLogin::AddAuthUser( UI16 requestcode, UI16 packetUnique, UI16 frontCharID, char *szID, char *szPW, char *szIP, char *szCookie, char *szMacAddress )
{
	if( m_siCount == m_siMaxList ) return FALSE;

	SI32 workindex = m_siEndIndex;

	sbslogininfo *pInfo = &m_pLoginInfo[ workindex ];

	pInfo->m_requestCode = requestcode;

	pInfo->m_usPacketUnique = packetUnique;
	pInfo->m_usFrontCharID = frontCharID;

	memcpy( pInfo->m_szID, szID, 20 );
	pInfo->m_szID[ 19 ] = NULL;

	memcpy( pInfo->m_szPW, szPW, 20 );
	pInfo->m_szPW[ 19 ] = NULL;

	memcpy( pInfo->m_szIP, szIP, 20 );
	pInfo->m_szIP[ 19 ] = NULL;

	memcpy( pInfo->m_szCookie, szCookie, 20 );
	pInfo->m_szCookie[ 19 ] = NULL;

	memcpy( pInfo->m_szMacAddress, szMacAddress, 20 );
	pInfo->m_szMacAddress[ 19 ] = NULL;

	pInfo->m_bValid = TRUE;

	workindex++;

	if( workindex == m_siMaxList ) workindex = 0;

	InterlockedExchange( (LPLONG)&m_siEndIndex, workindex );

	InterlockedIncrement( (LPLONG)&m_siCount );

	return TRUE;
}

BOOL SBSLogin::RequestAuth()
{
	char buf[ 1024 ];

	sbslogininfo *pInfo = &m_pLoginInfo[ m_siStartIndex ];

	// build packet 
	sprintf( buf, "%s<ID#PW>%s", &pInfo->m_szID[ 2 ], pInfo->m_szPW );
	int len = strlen( buf );
	
	// send
	int sendbytes = send( m_socket, buf, len, 0 );

	printf( "send string: %s\n", buf, len );

	if( sendbytes < len ) {
		printf( "send error!\n" );
		CloseSocket();
		return FALSE;
	}

	char buf2[ 1024 ];
	sprintf( buf2, "Request : %s", buf, len );
	WriteLogFile( buf2 );
	
	return TRUE;
}

void SBSLogin::ResponseAuth()
{

	char buf[ 1024 ];

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

		if( FD_ISSET( m_socket, &readfds ) ){

			m_recvbytes = recv( m_socket, m_recvbuf, 1024, 0 );

			if( m_recvbytes <= 0 ) {
				sprintf( buf, "Recv Error!" );
				WriteLogFile( buf );

				CloseSocket();
				return;

			} else {

				m_recvbuf[ m_recvbytes ] = NULL;
				sprintf( buf, "Recv String : %s[ %d ]", m_recvbuf, GetTickCount() - tick );
				printf( "%s\n", buf );
				WriteLogFile( buf );
				
				sbslogininfo *pInfo = &m_pLoginInfo[ m_siStartIndex ];

				if( pInfo->m_requestCode == REQUESTCODE_IDNUMGET ) {

					RequestIDNUMGET();

				} else if( pInfo->m_requestCode == REQUESTCODE_JOINGAME ) {

					RequestJOINGAME();
				}

				CloseSocket();
				return;
			}	
		}
	}
}

void SBSLogin::RequestIDNUMGET()
{
	if( m_recvbuf[ 0 ] == 'Y' ) {
		// 인증 성공
		
		// 주민 번호 만들기 
		char jumin[ 20 ];
		jumin[ 0 ] = NULL;

		if( m_recvbuf[ 2 ] == 'M' ) {
			strcpy( jumin, "0000001000000" );
		} else if( m_recvbuf[ 2 ] == 'F' ) {
			strcpy( jumin, "0000002000000" );
		}

		sbslogininfo *pInfo = &m_pLoginInfo[ m_siStartIndex ];

		// AccountDBManager로 인증 요청
		sDBRequest_IDNumGet	sendMsg;

		sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );

		sendMsg.usPacketUnique = pInfo->m_usPacketUnique;
		sendMsg.usFrontCharID = pInfo->m_usFrontCharID;

		memcpy( sendMsg.userid, pInfo->m_szID, 20 );
		sendMsg.userid[ 19 ] = NULL;

		memcpy( sendMsg.userpw, pInfo->m_szPW, 20 );
		sendMsg.userpw[ 19 ] = NULL;

		memcpy( sendMsg.userIP, pInfo->m_szIP, 20 );
		sendMsg.userIP[ 19 ] = NULL;

		memcpy( sendMsg.jumin, jumin, 20 );
		sendMsg.jumin[ 19 ] = NULL;

		memcpy( sendMsg.macaddress, pInfo->m_szMacAddress, 20 );
		sendMsg.macaddress[ 19 ] = NULL;
		
		g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

	} else {
		// 인증 실패

		// 응답 보내기

		sbslogininfo *pInfo = &m_pLoginInfo[ m_siStartIndex ];

		sResponse_FrontAuth	sendMsg;

		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

		sendMsg.usPacketUnique = pInfo->m_usPacketUnique;
		sendMsg.usFrontCharID = pInfo->m_usFrontCharID;

		sendMsg.usResult = 2;

		sendMsg.siAuthKey = 0;

		sendMsg.siGameMasterLevel = 0;

		ITZSession *pSession = g_afp.GetSessionManager()->GetSession(0);

		if( pSession ) pSession->GetSendBuffer()->Write( (char *)&sendMsg );

	}

}

void SBSLogin::RequestJOINGAME()
{
	if( m_recvbuf[ 0 ] == 'Y' ) {
		// 인증 성공
		
		// 주민 번호 만들기 
		char jumin[ 20 ];
		jumin[ 0 ] = NULL;

		if( m_recvbuf[ 2 ] == 'M' ) {
			strcpy( jumin, "0000001000000" );
		} else if( m_recvbuf[ 2 ] == 'F' ) {
			strcpy( jumin, "0000002000000" );
		}

		sbslogininfo *pInfo = &m_pLoginInfo[ m_siStartIndex ];

		// AccountDBManager로 인증 요청
		sDBRequest_JoinGame sendMsg;

		sendMsg.packetHeader.usCmd = DBMSG_REQUEST_JOINGAME;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );

		sendMsg.usPacketUnique = pInfo->m_usPacketUnique;
		sendMsg.usFrontCharID = pInfo->m_usFrontCharID;

		// id
		memcpy( sendMsg.userid, pInfo->m_szID, 20 );
		sendMsg.userid[ 19 ] = NULL;

		// pw
		memcpy( sendMsg.userpw, pInfo->m_szPW, 20 );
		sendMsg.userpw[ 19 ] = NULL;

		// jumin
		memcpy( sendMsg.jumin, jumin, 20 );
		sendMsg.jumin[ 19 ] = NULL;

		// cookie
		memcpy( sendMsg.cookie, pInfo->m_szCookie, 20 );
		sendMsg.cookie[ 19 ] = NULL;

		// ip
		memcpy( sendMsg.szIP, pInfo->m_szIP, 20 );
		sendMsg.szIP[ 19 ] = NULL;
		
		g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

	} else {
		// 인증 실패

	}

}

void SBSLogin::WriteLogFile( char *szLog ) 
{
	_SYSTEMTIME time;

	GetLocalTime( &time );

	char filename[ 128 ];
	char fileBuf[ 1024 ];

	sprintf( filename, "sbslog\\%02d월%02d일_%04d.txt", time.wMonth, time.wDay, m_usPort );

	FILE *fp = fopen( filename, "a+t" );
	fseek( fp, 0, SEEK_END );

	wsprintf( fileBuf,"[%02d월%02d일%02d시%02d분%02d초] %s\r\n", 
		time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, szLog );
	
	fwrite( fileBuf, sizeof( char ), strlen( fileBuf ), fp );

	fclose(fp);

}