#include "stdafx.h"
#include "client.h"


/*
BOOL InBufferMessageOperation(cltMsg* pclMsg, SI32 id)
{
	// 메시지를 수령한 시간을 확인한다.
	//dwLastReceiveFromServerClock = CurrentClock;
	char Nbuffer[512]=" ";

	switch(pclMsg->clHead.usCmd)
	{
	case RESPONSE_LOGIN:
		{

		}
		break;

	default:
		break;
	}

	return true;
}
*/

#include "./ITZNetLib/ITZNetworkThread.h"
#include "./ITZNetLib/ITZSessionFactory.h"
#include "./ITZNetLib/ITZSessionManager.h"
#include "./ITZNetLib/ITZSession.h"
#include "./ITZNetLib/ITZNetIOCP.h"
#include "./ITZNetLib/ITZPerson.h"
#include "./ITZNetLib/ITZEncrypt.h"

#include "./ITZNetLib/ITZRecvBuffer.h"
#include "./ITZNetLib/ITZSendBuffer.h"

#include "../TestServer/msgType.h"
#include "./ITZNetLib/ITZNetOverlapped.h"

#include <conio.h>
#include <stdio.h>

class cChar : public ITZPerson
{
public:
	cChar() {};
	~cChar() {};
};


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

struct cTestPH
{
	UI16	size;
	UI08	chk;
	UI16	cmd;
};

struct cPacket1
{
	cTestPH		ph;
	char 		data[ 1024 ];
};


int				count = 10000;

cPacket1		packet;

ITZRecvBuffer	recvBuf;



unsigned __stdcall thread( LPVOID param )
{

	char *pBuf;
	char *pTempPtr;
	SI32 siSize;
	SI32 temp;


	for( int i = 0; i < count; ++i ) {


		//packet.ph.size = 501;
		packet.ph.size = 10 + ( rand() % 512 );
		packet.ph.cmd = i;

		temp = packet.ph.size;
		pTempPtr = (char *)&packet;

		while( temp > 0 ) {

			recvBuf.GetRecvParam( &pBuf, &siSize );

			if( siSize == 0 ) {
				Sleep( 10 );
				continue;
			}

			if( siSize < temp ) {

				memcpy( pBuf, pTempPtr, siSize );

				recvBuf.Completion( siSize );

				pTempPtr += siSize;

				temp -= siSize;

			} else {

				memcpy( pBuf, pTempPtr, temp );

				recvBuf.Completion( temp );

				pTempPtr += temp;

				temp = 0;
			}
		}

		//		Sleep( 20 );
		//		if( i == 0 ) Sleep( rand() % 20 );

	}

	return 0;
}


void MessageMain()
{
	memset( &packet, 1, sizeof( packet ) );

	recvBuf.CreateRecvBuffer( 2000 );
	recvBuf.ClearBuffer();

	UI32 uiThreadID;

	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, thread, NULL, 0, &uiThreadID );
	CloseHandle( hThread );	

	/*
	for( int i = 0; i < count; ++i ) {

	//printf( "%d...", recvBuf.GetPacketCount() );

	int count = recvBuf.GetPacketCount();
	cPacket1 *pPacket = (cPacket1 *)recvBuf.GetFirstPacket();

	if( pPacket == NULL ) {
	//printf( "pPacket == NULL" );
	--i;
	continue;
	}

	printf( "pc:%d  ", count );

	if( pPacket->ph.size > 600 || pPacket->ph.cmd != i ) {
	printf( "%d, %d, %d ERROR!!\n", pPacket->ph.size, pPacket->ph.cmd, (int)pPacket - (int)recvBuf.m_pBasePtr );
	exit(0);
	}

	printf( "size: %d, cmd: %d, addr:%d, endPtr:%d\n", 
	pPacket->ph.size, 
	pPacket->ph.cmd, 
	(int)pPacket - (int)recvBuf.m_pBasePtr,
	(int)recvBuf.m_pEndPtr - (int)recvBuf.m_pBasePtr );

	recvBuf.RemoveFirstPacket();

	//		Sleep(20);
	}

	*/

	WinSockInit();


	printf( "test client... (1:패킷보내기, ESC:종료)\n" );

	ITZSessionFactory< cChar > sf;

	ITZSessionManager			sm;

	ITZNetIOCP					iocp;
	ITZNetOverlapped			overlapped;

	ITZSession*					pSession;


	sf.CreateFactory( &sm, 1, 30000, 30000 );			// Session객체들을 생성하고, SessionManager를 구성해 준다 

	pSession = sm.GetSession( 0 );						// 세션 메니저로 부터 하나의 클라이언트용 세션을 얻는다

	//	iocp.CreateIOCP( &sm, 1 );							// iocp를 생성한다

	overlapped.CreateOverlapped( &sm );					// overlapped를 생성한다


	// loop
	BOOL bRun = TRUE;
	while( bRun ) {

		Sleep( 10 );

		// 세션이 연결되지 않은 상태이면, 연결 시도 한다
		if( pSession->GetState() == SESSION_STATE_NONE ) {
			printf( "연결시도..." );
			if( sm.Connect( 0, "192.168.1.95", 4010 ) ) {
				printf( "성공\n" ); 
			} else {
				printf( "실패\n" );
				Sleep( 500 );
			}
		}


		// 세션이 성공적으로 연결된 상태이면, 세션의 패킷 처리를 한다
		if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) {		

			char *pPacket = pSession->GetRecvBuffer()->GetFirstPacket();		// 버퍼에서 패킷을 하나 꺼내온다

			if( pPacket != NULL ) {										
				// 패킷이 있으면,

				printf( "패킷을 하나 받음!\n" );

				sResponse_Test1* pResponse = (sResponse_Test1*)pPacket;

				printf( "받은 패킷 : %d\n", pResponse->a);

				pSession->GetRecvBuffer()->RemoveFirstPacket();						// 처리 후 패킷을 버퍼에서 지운다
			}
		}



	

		// console

		
		if( kbhit() ) {
			switch( getch() )
			{
			case 27:
				bRun = FALSE;
				break;

			case '1':
				{
					printf( "패킷 보내기 MSG_REQUEST_TEST1\n" );

					sRequest_Test1 sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = MSG_REQUEST_TEST1;
					sendMsg.a = rand() % 256;

					pSession->GetSendBuffer()->Write( (char *)&sendMsg ); 

					printf("보낸 패킷 : %d\n", sendMsg.a );
				}
				break;

			case '2':
				{
					printf( "패킷 보내기 MSG_REQUEST_TEST2\n" );

					sRequest_Test2 sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = MSG_REQUEST_TEST2;

					pSession->GetSendBuffer()->Write( (char *)&sendMsg ); 
				}
				break;
			}					
		}		
	}

				
	WSACleanup();
	return;
}


BOOL InBufferMessageOperation(cltMsg* pclMsg, SI32 id)
{
	// 메시지를 수령한 시간을 확인한다.
	//dwLastReceiveFromServerClock = CurrentClock;
	char Nbuffer[512]=" ";

	switch(pclMsg->clHead.usCmd)
	{
	case RESPONSE_LOGIN:
		{
            
		}
		break;


	default:
		break;
	}

	return true;
}
