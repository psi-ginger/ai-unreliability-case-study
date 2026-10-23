#include "..\Common\CommonHeader.h"
#include "..\CommonLogic\CommonLogic.h"

#include "FrontAuthProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../Server/FrontServer/FrontMsg.h"
#include "../../Server/AuthServer/AuthMsg.h"

#include "../Server.h"

extern cltCommonLogic* pclClient;

FrontAuthProcess::FrontAuthProcess()
{
	m_pFrontSession = NULL;
	m_pAuthSession = NULL;

	m_frontIP[ 0 ] = NULL;
	m_authIP[ 0 ] = NULL;
}

FrontAuthProcess::~FrontAuthProcess()
{
	
}

void FrontAuthProcess::Init( TCHAR *frontIP, UI16 frontPort, TCHAR *authIP, UI16 authPort )
{

	m_sf.CreateFactory( &m_sm, 2, 50000, 50000 );

	m_pFrontSession = m_sm.GetSession( 0 );
	m_pAuthSession = m_sm.GetSession( 1 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	StringCchCopy( m_frontIP, 20, frontIP );
	m_frontPort = frontPort;

	StringCchCopy( m_authIP, 20, authIP );
	m_authPort = authPort;

	m_refreshInfo.state = 0;
}

void FrontAuthProcess::Run()
{

	cltServer *pclServer = (cltServer *)pclClient;

	m_sm.Update();

	DWORD		currentTick;

	// ����Ʈ ������ ������� ���� �����̸�,
	if( m_pFrontSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_frontIP, m_frontPort ) == m_pFrontSession ) {
			m_frontLastTick = GetTickCount() + FRONT_HEARTBEAT_INTERVAL;
		} else {

		}
		
	} 
	// ����Ʈ ������ ���������� ����� �����̸�,
	else if( m_pFrontSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		// heartbeat ������
		
		currentTick = GetTickCount();
		if( currentTick >= m_frontLastTick + FRONT_HEARTBEAT_INTERVAL ) {
			m_frontLastTick = currentTick;

			sReportToFront_GameHeartbeat	sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = FRONTMSG_REPORT_GAMEHEARTBEAT;

			// ���� ���� �� ��� 
			m_refreshInfo.currentConnections = pclServer->pclGameProtocol->GetSessionManager()->GetActiveSessionCount();
			//m_refreshInfo.state = 0;

			sendMsg.serverId = pclServer->siServerID;
			sendMsg.refreshinfo = m_refreshInfo;

			m_pFrontSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}



		// ���ۿ��� ��Ŷ�� �ϳ� �����´�. 
		sPacketHeader* pPacket = (sPacketHeader*)m_pFrontSession->GetRecvBuffer()->GetFirstPacket();

		if(pPacket != NULL)
		{
			switch(pPacket->usCmd)
			{
			case FRONTMSG_REQUEST_GAMESERVERINFO:
				{
					InterlockedExchange( (LPLONG)&((cltServer*)pclClient)->m_bToSendServerInfoToFront, TRUE );
				}
				break;
			default:
				{
					if (pclClient->GameMode != GAMEMODE_SERVER)
						MessageBox(NULL, TEXT("fdsdi9hf"), TEXT("fd9yu2hd"), MB_OK|MB_TOPMOST);
				}
				break;
			}

		m_pFrontSession->GetRecvBuffer()->RemoveFirstPacket();
	}		
		



	}

	// Auth ������ ������� ���� �����̸�,
	if( m_pAuthSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 1, m_authIP, m_authPort ) == m_pAuthSession ) {
			m_authLastTick = GetTickCount() + AUTH_HEARTBEAT_INTERVAL;
		} else {

		}
		
	}
	// Auth ������ ���������� ����� �����̸�, 
	else if( m_pAuthSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		// heartbeat ������
		currentTick = GetTickCount();
		if( currentTick >= m_authLastTick + AUTH_HEARTBEAT_INTERVAL ) {
			m_authLastTick = currentTick;

			sReportToAuth_GameHeartbeat	sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = AUTHMSG_REPORT_GAMEHEARTBEAT;

			sendMsg.serverId = pclServer->siServerID;
			sendMsg.worldId = m_refreshInfo.worldId;

			m_pAuthSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}
	}

	// ���ۿ��� ��Ŷ�� �ϳ� �����´�. 
	sPacketHeader* pPacket = (sPacketHeader*)m_pAuthSession->GetRecvBuffer()->GetFirstPacket();

	if(pPacket != NULL)
	{
		switch(pPacket->usCmd)
		{
		case AUTHMSG_RESPONSE_GAMEAUTH:
			{
				((cltServer*)pclClient)->DoMsg_AUTHMSG_RESPONSE_GAMEAUTH(pPacket);
			}
			break;

		case AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH:
			{
				((cltServer*)pclClient)->DoMsg_AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH(pPacket);
			}
			break;
		case AUTHMSG_RESPONSE_GMTOOLAUTH:
			{
				((cltServer*)pclClient)->DoMsg_AUTHMSG_RESPONSE_GMTOOLAUTH(pPacket);
			}
			break;
		default:
			{
				if (pclClient->GameMode != GAMEMODE_SERVER)
					MessageBox(NULL, TEXT("fdsdi9hf"), TEXT("fd9yu2hd"), MB_OK|MB_TOPMOST);
			}
			break;
		}

		m_pAuthSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

bool FrontAuthProcess::SendMsg(sPacketHeader* pPacket)
{
	m_pAuthSession->GetSendBuffer()->Write( (BYTE *)pPacket);

	return true;
}