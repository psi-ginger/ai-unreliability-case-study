#include "FrontGameProtocol.h"
//#include "FrontServer.h"
#include "Log.h"


FrontGameProtocol::FrontGameProtocol()
{

}


FrontGameProtocol::~FrontGameProtocol()
{

}


void FrontGameProtocol::Init(UI16 siListenPort)
{

	m_SF.CreateFactory( GetSessionManager(), MAX_GAMESERVER_CONNECTION, 1000 * 1000, 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), siListenPort );

	g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "FrontGameProtocol::Init(%d)", siListenPort );
}



void FrontGameProtocol::LogOut( ITZSession *pSession )
{


}


void FrontGameProtocol::SessionProc( ITZSession *pSession )
{

	cGame *pPerson;

	pPerson = (cGame *)pSession->GetPerson();

	if( pPerson ) {

		if( pPerson->m_bInitServerInfo == FALSE ) {

			if( pPerson->m_bRequestServerInfo == FALSE ) {
				// request!
				pPerson->m_bRequestServerInfo = TRUE;
				pPerson->m_dwRequestTick = GetTickCount();

				RequestGameServerInfo( pSession );
					
			} else {
				
				if( pPerson->m_dwRequestTick + 30 < GetTickCount() ) {
					pPerson->m_bRequestServerInfo = FALSE;
				}
			}
		} 
	}

	GAMESERVER_REFRESHINFO refreshinfo;	
	if( GetTickCount() - pPerson->m_dwLastRecvTick >= 15000 ) {

		EnterCriticalSection( &g_cs );

		if( g_worldlist.GetWorldRefreshInfoByServerID( pPerson->m_siServerID, &refreshinfo ) ) {

			refreshinfo.state = 2;
			refreshinfo.currentConnections = 0;

			g_worldlist.SetWorldRefreshInfo( refreshinfo.worldId, &refreshinfo );

		}

		LeaveCriticalSection( &g_cs );

		printf( "GameServer No Heartbeat!\n" );

		pSession->CloseSocket();
		return;
	}


	// 버퍼에서 패킷을 하나 꺼내온다 
	sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																							
	if( pPacket == NULL ) return;										// 패킷이 없으면 나간다	
	
	pPerson->m_dwLastRecvTick = GetTickCount();

	// 패킷 커맨드에 따른 처리
	switch( pPacket->usCmd ) {

	case FRONTMSG_REPORT_GAMEHEARTBEAT:
		{

			EnterCriticalSection( &g_cs );

			sReportToFront_GameHeartbeat *pRecvMsg = (sReportToFront_GameHeartbeat *)pPacket;

			if( g_worldlist.GetWorldRefreshInfoByServerID( pRecvMsg->serverId, &refreshinfo ) ) {
			
				pPerson->m_siServerID = pRecvMsg->serverId;

				refreshinfo.state = 0;

				g_worldlist.SetWorldRefreshInfo( refreshinfo.worldId, &pRecvMsg->refreshinfo );
			}

			LeaveCriticalSection( &g_cs );

		}
		break;

	case FRONTMSG_RESPONSE_GAMESERVERINFO:
		{
			EnterCriticalSection( &g_cs );

			sResponseToFront_GameServerInfo *pRecvMsg = (sResponseToFront_GameServerInfo *)pPacket;
			
			WORLDINFO worldInfo;
			
			if( g_worldlist.GetWorldInfoByServerID( pRecvMsg->serverId, &worldInfo ) ) {

				worldInfo.gameserverinfo = pRecvMsg->gameserverinfo;
				g_worldlist.SetWorldInfo( worldInfo.refreshinfo.worldId, &worldInfo );

				pPerson->m_bInitServerInfo = TRUE;
			}

			LeaveCriticalSection( &g_cs );
		}
		break;
	}

	pSession->GetRecvBuffer()->RemoveFirstPacket();
}


void FrontGameProtocol::Update()
{
	DWORD tick = GetTickCount();

	InterlockedExchange( (LPLONG)&g_dwGameProtocolTick, tick );
}

void FrontGameProtocol::RequestGameServerInfo( ITZSession *pSession )
{	
	char buffer[ 128 ];
	sPacketHeader *packet = (sPacketHeader *)buffer;

	packet->usCmd = FRONTMSG_REQUEST_GAMESERVERINFO;
	packet->usSize = 10;

	if( pSession ) {
		pSession->GetSendBuffer()->Write( (BYTE*)packet );
	}

}