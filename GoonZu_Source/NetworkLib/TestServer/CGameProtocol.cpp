#include "CGameProtocol.h"
#include "msgType.h"

CGameProtocol::CGameProtocol()
{

	m_SF.CreateFactory( GetSessionManager(), 1000, 30000, 30000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), 4010 );

}

CGameProtocol::~CGameProtocol()
{

}

void CGameProtocol::LogOut( ITZSession *pSession )
{
	MessageBox( 0, "로그아웃!", "", 0 );
}


void CGameProtocol::SessionProc( ITZSession *pSession )
{

	// 버퍼에서 패킷을 하나 꺼내온다 
	sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );		
																							
	if( pPacket == NULL ) return;										// 패킷이 없으면 나간다

	
	// 패킷 커맨드에 따른 처리
	switch( pPacket->usCmd ) {

	case MSG_REQUEST_TEST1:
		{

			// 커맨드에 따른 메시지 구조체 캐스팅
			sRequest_Test1	*pRecvMsg = (sRequest_Test1 *)pPacket;

			// 처리
			MessageBox( 0, "MSG_REQUEST_TEST1 받음!", "", 0 );

			// 응답 보내기

			sResponse_Test1	sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = MSG_RESPONSE_TEST1;

			pSession->GetSendBuffer()->Write( (char *)&sendMsg );
		}
		break;

	case MSG_REQUEST_TEST2:
		{

			// 커맨드에 따른 메시지 구조체 캐스팅
			sRequest_Test2	*pRecvMsg = (sRequest_Test2 *)pPacket;

			// 처리
			MessageBox( 0, "MSG_REQUEST_TEST2 받음!", "", 0 );

			// 응답 보내기
			sResponse_Test2	sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = MSG_RESPONSE_TEST2;

			pSession->GetSendBuffer()->Write( (char *)&sendMsg );
		}

		break;
	}

	pSession->GetRecvBuffer()->RemoveFirstPacket();						// 처리 후 패킷을 버퍼에서 지운다

}