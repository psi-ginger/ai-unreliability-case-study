#include "GameDBProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include "../GameDBManager/DBMsg.h"

GameDBProcess::GameDBProcess()
{
	m_pGameDBSession = NULL;

	m_gameDBManagerIP[ 0 ] = NULL;
}

GameDBProcess::~GameDBProcess()
{

}

void GameDBProcess::Init( char *ip, UI16 usPort )
{
	m_sf.CreateFactory( &m_sm, 1, 1000 * 1000, 1000 * 1000 );

	m_pGameDBSession = m_sm.GetSession( 0 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	strcpy( m_gameDBManagerIP, ip );
	m_gameDBManagerPort = usPort;
}

void GameDBProcess::Run()
{
	m_sm.Update();

	// 세션이 연결되지 않은 상태이면,
	if( m_pGameDBSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_gameDBManagerIP, m_gameDBManagerPort ) == m_pGameDBSession ) {
		} else {
		}
		
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pGameDBSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pGameDBSession->GetRecvBuffer()->GetFirstPacket();
			
		if( pPacket != NULL ) {										
			// 패킷이 있으면,

			switch( pPacket->usCmd ) {

			case DBMSG_RESPONSE_PERSONLISTINFOGET:
				{
					printf( "DBMSG_RESPONSE_PERSONLISTINFOGET\n" );
					sDBResponse_PersonListInfoGet *pRecvMsg =  (sDBResponse_PersonListInfoGet *)pPacket;

					for( int i = 0; i < 3; ++i ) {
						printf( "[%d] kind:%d, name:%s, mapindex:%d, x:%d, y:%d\n",
							i, pRecvMsg->personInfo[ i ].kind,
							pRecvMsg->personInfo[ i ].name,
							pRecvMsg->personInfo[ i ].mapindex,
							pRecvMsg->personInfo[ i ].x,
							pRecvMsg->personInfo[ i ].y );
					}
					
				}
				break;

			}

			// 처리 후 패킷을 버퍼에서 지운다
			m_pGameDBSession->GetRecvBuffer()->RemoveFirstPacket();
		}

	}

}