#include "AuthGameProtocol.h"
#include "AuthUserList.h"
#include "AccountDBProcess.h"
#include "UserManager.h"




AuthGameProtocol::AuthGameProtocol()
{
	
}

AuthGameProtocol::~AuthGameProtocol()
{
	
}

void AuthGameProtocol::Init()
{
	m_SF.CreateFactory( GetSessionManager(), 30, 5 * 1000 * 1000, 5 * 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), 5600 );
	
	SetFPS( 200 );
}


void AuthGameProtocol::LogOut( ITZSession *pSession )
{
	
}

void AuthGameProtocol::SessionProc( ITZSession *pSession )
{

	int count = pSession->GetRecvBuffer()->GetPacketCount();

	for( int i = 0; i < count; ++i ) {

		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( pPacket == NULL ) return;										// 패킷이 없으면 나간다
		
		// 패킷 커맨드에 따른 처리
		switch( pPacket->usCmd ) {

		case AUTHMSG_REPORT_GAMEHEARTBEAT:
			{
			}
			break;

		case AUTHMSG_REQUEST_GAMEAUTH:
			{

				printf( "Request user auth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GameAuth	*pRecvMsg = (sRequest_GameAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_GameAuth	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GAMEAUTH;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pSession->m_usIndex;
				sendMsg.usCharID = pRecvMsg->usCharID;

				sendMsg.idnum = pRecvMsg->siAuthKey;

				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "IDNUM:%d, ID:%s, PW:%s\n", sendMsg.idnum, sendMsg.userid, sendMsg.userpw );

			}
			break;


		case AUTHMSG_REQUEST_GAMELOGOUT:
			{
				printf( "Request user logout from GameServer\n" );
				
				sRequest_GameLogout	*pRecvMsg = (sRequest_GameLogout *)pPacket;

				g_usermanager.GameLogOut( pRecvMsg->uiIDNum );


			}
			break;

		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void AuthGameProtocol::Update()
{

}