#include "ToolsLog.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"


CToolsLogProcess::CToolsLogProcess()
{
	m_pToolsLogSession = NULL;
}

CToolsLogProcess::~CToolsLogProcess()
{

}

void CToolsLogProcess::Init(char *szIP, UI16 usPort )
{ 

	m_sf.CreateFactory( &m_sm, 1, 5000 * 1000, 5000 * 1000 );

	m_pToolsLogSession = m_sm.GetSession( 0 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	strcpy( m_szIP, szIP );
	m_usPort = usPort;
}

void CToolsLogProcess::Run()
{

	if( m_pToolsLogSession == NULL ) return;

	m_sm.Update();

	// 세션이 연결되지 않은 상태이면,
	if( m_pToolsLogSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pToolsLogSession )
		{
			printf( "Connect!!" );
		}
		else
		{
		}
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pToolsLogSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

	}
}

void CToolsLogProcess::SendMsg(sPacketHeader *pPacket )
{
	if( m_pToolsLogSession == NULL ) return;
	if( m_pToolsLogSession->GetState() != SESSION_STATE_ESTABLISHED ) return;

	m_pToolsLogSession->GetSendBuffer()->Write( (BYTE *)pPacket );
}

