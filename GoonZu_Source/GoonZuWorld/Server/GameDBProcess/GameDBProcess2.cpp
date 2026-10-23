#include "..\Server.h"

#include "GameDBProcess2.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"

extern cltCommonLogic* pclClient;

GameDBProcess2::GameDBProcess2()
{
	m_pGameDBSession = NULL;

	m_gameDBManagerIP[ 0 ] = NULL;
}

GameDBProcess2::~GameDBProcess2()
{

}

void GameDBProcess2::Init( TCHAR *ip, UI16 usPort )
{
	m_sf.CreateFactory( &m_sm, 1, 5000 * 1000, 5000 * 1000 );

	m_pGameDBSession = m_sm.GetSession( 0 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	StringCchCopy( m_gameDBManagerIP, 20, ip );
	m_gameDBManagerPort = usPort;
}

void GameDBProcess2::Run()
{
	if(m_pGameDBSession == NULL)return ;

	m_sm.Update();	

	// 세션이 연결되지 않은 상태이면,
	if( m_pGameDBSession->GetState() == SESSION_STATE_NONE )
	{
		//printf(  TEXT("DB Manager 접속 시도\n") );
		if( m_sm.Connect( 0, m_gameDBManagerIP, m_gameDBManagerPort ) == m_pGameDBSession ) {
		} else {
		}
		
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pGameDBSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		int count = m_pGameDBSession->GetRecvBuffer()->GetPacketCount();

		for( int i = 0; i < count; ++i ) {

			// 버퍼에서 패킷을 하나 꺼내온다
			sPacketHeader *pPacket = (sPacketHeader *)m_pGameDBSession->GetRecvBuffer()->GetFirstPacket();
				
			if( pPacket != NULL ) {										
				// 패킷이 있으면,

				((cltServer *)pclClient)->DoDBMsg2(pPacket);

				// 처리 후 패킷을 버퍼에서 지운다
				m_pGameDBSession->GetRecvBuffer()->RemoveFirstPacket();
				

			}
		
		}


	}

}

bool GameDBProcess2::SendMsg(sPacketHeader *pPacket)
{
	if(m_pGameDBSession == NULL) return false;
	if(m_pGameDBSession->GetSendBuffer() == NULL) return false;

	if(m_pGameDBSession->GetState() != SESSION_STATE_ESTABLISHED) return false;

	m_pGameDBSession->GetSendBuffer()->Write( (BYTE *)pPacket);
	return true;
}



