#include "AuthUserList.h"
#include "AuthFrontProtocol.h"
#include "AccountDBProcess.h"

AuthFrontProtocol::AuthFrontProtocol()
{

}

AuthFrontProtocol::~AuthFrontProtocol()
{

}

void AuthFrontProtocol::Init()
{
	m_SF.CreateFactory( GetSessionManager(), 1, 20 * 1000 * 1000, 20 * 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), 8700 );

	SetFPS( 200 );
}


void AuthFrontProtocol::LogOut( ITZSession *pSession )
{

}

void AuthFrontProtocol::SessionProc( ITZSession *pSession )
{

	int count = pSession->GetRecvBuffer()->GetPacketCount();

	for( int i = 0; i < count; ++i ) {

		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( pPacket == NULL ) return;										// 패킷이 없으면 나간다
		
		// 패킷 커맨드에 따른 처리
		switch( pPacket->usCmd ) {

		case AUTHMSG_REQUEST_FRONTAUTH:
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_FrontAuth	*pRecvMsg = (sRequest_FrontAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_IDNumGet	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				memcpy( sendMsg.userIP, pRecvMsg->userIP, 20 );
				sendMsg.userIP[ 19 ] = NULL;

				memcpy( sendMsg.macaddress, pRecvMsg->macaddress, 20 );
				sendMsg.macaddress[ 19 ] = NULL;

				sendMsg.jumin[ 0 ] = NULL;
				
				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;
			
		case AUTHMSG_REQUEST_FRONTJOINGAME:
			{
				sRequest_FrontJoinGame	*pRecvMsg = (sRequest_FrontJoinGame *)pPacket;
			
				sDBRequest_JoinGame sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_JOINGAME;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				// id
				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				// pw
				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				// cookie
				memcpy( sendMsg.cookie, pRecvMsg->cookie, 20 );
				sendMsg.cookie[ 19 ] = NULL;

				// ip
				memcpy( sendMsg.szIP, pRecvMsg->szIP, 20 );
				sendMsg.szIP[ 19 ] = NULL;

				// jumin
				sendMsg.jumin[0] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

			}
			break;
		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void AuthFrontProtocol::Update()
{

}
