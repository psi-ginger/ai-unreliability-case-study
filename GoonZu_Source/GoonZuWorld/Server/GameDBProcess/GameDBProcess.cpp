#include "..\Server.h"

#include "GameDBProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"

extern cltCommonLogic* pclClient;

GameDBProcess::GameDBProcess()
{
	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		m_pGameDBSession[ i ] = NULL;
		m_bPing[ i ] = FALSE;
		m_dwPingTick[ i ] = 0;
		m_dwDBLatency[ i ] = 0;
	}

	m_dwCurrentDBLatency = 0;
	
	m_gameDBManagerIP[ 0 ] = NULL;

	m_siWorkSlot = 0;

	m_dwTick = GetTickCount();
}

GameDBProcess::~GameDBProcess()
{

}	

void GameDBProcess::Init( TCHAR *ip, UI16 usPort )
{
	m_sf.CreateFactory( &m_sm, DBCONNECTION_NUMBER, 10000 * 1000, 10000 * 1000 );

	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		m_pGameDBSession[ i ] = m_sm.GetSession( i );
	}

	m_iocp.CreateIOCP( &m_sm, 1 );

	StringCchCopy( m_gameDBManagerIP, 20, ip );
	m_gameDBManagerPort = usPort;
}

void GameDBProcess::Run()
{
	int a;

	for( a = 0; a < DBCONNECTION_NUMBER; ++a ) {
		if( m_pGameDBSession[ a ] == NULL ) {
			m_dwDBLatency[ a ] = 99999;
			m_dwPingTick[ a ] = 0;
			m_bPing[ a ] = FALSE;
		}
	}

	if( a < DBCONNECTION_NUMBER ) return;

	m_sm.Update();	
	
	DWORD dwCurrentTick = GetTickCount();

	DWORD dwMaxDBLatency = 0;

	for( a = 0; a < DBCONNECTION_NUMBER; ++a ) {

		// 세션이 연결되지 않은 상태이면,
		if( m_pGameDBSession[ a ]->GetState() == SESSION_STATE_NONE )
		{
			//printf(  TEXT("DB Manager 접속 시도\n") );
			if( m_sm.Connect( a, m_gameDBManagerIP, m_gameDBManagerPort + a ) == m_pGameDBSession[ a ] ) {
			} else {
			}

			m_dwDBLatency[ a ] = 99999;
			m_dwPingTick[ a ] = 0;
			m_bPing[ a ] = FALSE;
			
		} 
		// 세션이 성공적으로 연결된 상태이면,
		else if( m_pGameDBSession[ a ]->GetState() == SESSION_STATE_ESTABLISHED )
		{
			// DBLatency

			if( m_bPing[ a ] == TRUE ) {
				m_dwDBLatency[ a ] =  dwCurrentTick - m_dwPingTick[ a ];

				if( m_dwDBLatency[ a ] > dwMaxDBLatency )
					dwMaxDBLatency = m_dwDBLatency[ a ];

			} else {
				if( dwCurrentTick - m_dwPingTick[ a ] > 1000 ) {
					
					m_bPing[ a ] = TRUE;
					m_dwPingTick[ a ] = dwCurrentTick;

					sDBRequest_Ping sendMsg( a, dwCurrentTick );
					SendMsg( a, (sPacketHeader *)&sendMsg );
				}
			}

			int count = m_pGameDBSession[ a ]->GetRecvBuffer()->GetPacketCount();

			for( int i = 0; i < count; ++i ) {

				// 버퍼에서 패킷을 하나 꺼내온다
				sPacketHeader *pPacket = (sPacketHeader *)m_pGameDBSession[ a ]->GetRecvBuffer()->GetFirstPacket();
					
				if( pPacket != NULL ) {										
					// 패킷이 있으면,

					// Ping

					if( pPacket->usCmd == DBMSG_RESPONSE_PING ) {
						
						sDBResponse_Ping *pPing = (sDBResponse_Ping *)pPacket;
						m_bPing[ pPing->siSelect ] = FALSE;
						 
					} else {
			
						((cltServer *)pclClient)->DoDBMsg(pPacket);
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pGameDBSession[ a ]->GetRecvBuffer()->RemoveFirstPacket();
					

				}
			
			}


		}
	}

	InterlockedExchange( (LPLONG)&m_dwCurrentDBLatency, dwMaxDBLatency );

}

bool GameDBProcess::SendMsg(sPacketHeader *pPacket)
{

	if( m_pGameDBSession[ DBSELECT_BASE ]->GetSendBuffer() == NULL ) return false;

	if( m_pGameDBSession[ DBSELECT_BASE ]->GetSendBuffer()->Write( (BYTE *)pPacket) == FALSE)
	{
		OutputDebugString("!!!!!! ---- Write Buffer is Full ---- !!!!!!");
	}

	return true;
}

bool GameDBProcess::SendMsg( SI32 siSelect, sPacketHeader *pPacket )
{
	if( siSelect < 0 || siSelect >= DBCONNECTION_NUMBER ) return false;

	if( m_pGameDBSession[ siSelect ]->GetSendBuffer() == NULL ) return false;

	if (m_pGameDBSession[ siSelect ]->GetSendBuffer()->Write( (BYTE *)pPacket) == FALSE )
	{
		OutputDebugString("!!!!!! ---- Write Buffer is Full ---- !!!!!!");
	}

	return true;	
}



BOOL GameDBProcess::IsEstablished()
{
	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		if( m_pGameDBSession[ i ] == NULL ) return FALSE;
		if( m_pGameDBSession[ i ]->GetState() != SESSION_STATE_ESTABLISHED ) return FALSE;
	}

	return TRUE;
}