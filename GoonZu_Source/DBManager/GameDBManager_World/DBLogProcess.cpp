#include "DBLogProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"


DBLogProcess::DBLogProcess()
{
	m_pLogSession = NULL;
}

DBLogProcess::~DBLogProcess()
{

}

void DBLogProcess::Init(char *szIP, UI16 usPort )
{ 

	m_sf.CreateFactory( &m_sm, 1, 5000 * 1000, 5000 * 1000 );

	m_pLogSession = m_sm.GetSession( 0 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	strcpy( m_szIP, szIP );
	m_usPort = usPort;
}

void DBLogProcess::Run()
{

	if( m_pLogSession == NULL ) return;

	m_sm.Update();

	// 세션이 연결되지 않은 상태이면,
	if( m_pLogSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pLogSession ) {
		
		} else {

		}
		
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pLogSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

	}
}

void DBLogProcess::SendMsg(sPacketHeader *pPacket )
{

	if( m_pLogSession == NULL ) return;
	if( m_pLogSession->GetState() != SESSION_STATE_ESTABLISHED ) return;

	m_pLogSession->GetSendBuffer()->Write( (BYTE *)pPacket );
}

