#include "AuthServer.h"
#include "AccountDBProcess.h"
#include "AuthFrontProtocol.h"
#include "AuthGameProtocol.h"
#include "AgentProcess.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../AdminSystem/MasterServer/MasterMsg.h"

#include "../../NLib/NMap.h"

extern char				g_szServiceArea[12];
extern char				g_szServiceMode[8];


AgentProcess::AgentProcess()
{
	m_pAgentSession = NULL;
	m_szIP[ 0 ] = NULL;

}

AgentProcess::~AgentProcess()
{

}

void AgentProcess::Init( char *szIP, UI16 usPort )
{

	strcpy( m_szIP, szIP );
	m_usPort = usPort;

	m_sf.CreateFactory( &m_sm, 1, 50 * 1000, 50 * 1000 );

	ITZSession *pSession = m_sm.GetSession( 0 );
	
	InterlockedExchange( (LPLONG)&m_pAgentSession, (LONG)pSession );

	m_iocp.CreateIOCP( &m_sm, 1 );
}

void AgentProcess::Run()
{

	m_sm.Update();

	DWORD		currentTick;

	if( m_pAgentSession == NULL ) return;

	if( g_agent.m_pAgentSession->GetState() == SESSION_STATE_NONE ) {

		if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pAgentSession ) {
			
			m_dwLastTick = GetTickCount() - AGENT_HEARTBEAT_INTERVAL;

			m_bSendFilePath = FALSE;

			printf( "Server Agent Connected!\n" );

		} else {
		}
	}

	// 에이전트세션이 성공적으로 연결된 상태이면,
	else if( m_pAgentSession->GetState() == SESSION_STATE_ESTABLISHED )
	{

		if( m_bSendFilePath == FALSE ) {

			sReportToAgent_FilePath		sendMsg;

			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.packetHeader.usCmd = AGENTMSG_REPORT_FILEPATH;

			strcpy( sendMsg.szMessage, "AuthServer" );

			GetModuleFileName( 0, sendMsg.szFilePath, 127 );

			printf( "%s\n", sendMsg.szFilePath );

			m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			m_bSendFilePath = TRUE;

		}

		// heartbeat 보내기
		
		currentTick = GetTickCount();
		if( currentTick >= m_dwLastTick + AGENT_HEARTBEAT_INTERVAL ) {
			m_dwLastTick = currentTick;


			// 초기화 정보가 없는 상태이면 마스터로 초기화 정보를 요청한다
			if( g_bInitReady == FALSE ) {

				//sRequest_AuthServerInitInfo	sendMsg;
				sRequest_AuthServerInitInfo_String	sendMsg;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			} else { 
				
				sReport_AuthServerHeartbeat		sendMsg;

				sendMsg.packetHeader.usCmd = MASTERMSG_REPORT_AUTHSERVER_HEARTBEAT;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );
				
				sendMsg.dwProcessId = GetCurrentProcessId();
				sendMsg.refreshInfo.usStatus = 0;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );				

			}

		}


		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pAgentSession->GetRecvBuffer()->GetFirstPacket();		
		
		if( pPacket != NULL ) {										
			// 패킷이 있으면,

			switch( pPacket->usCmd ) {

			case MASTERMSG_RESPONSE_AUTHSERVERINITINFO:
				{
					if( g_bInitReady == FALSE ) {

						sResponse_AuthServerInitInfo *pRecvMsg = (sResponse_AuthServerInitInfo *)pPacket;

						// server id 설정
						g_siServerID = pRecvMsg->siServerID;

						g_accountDB.Init( pRecvMsg->szAccountDBManagerIP, pRecvMsg->usAccountDBManagerPort, pRecvMsg->szAccountDBIP );

						// FrontServer, GameServer Listen 설정
						g_agp.Init(pRecvMsg->usGameServerListenPort); 
						g_afp.Init(pRecvMsg->usFrontServerListenPort);

						g_agp.CreateThread();
						g_afp.CreateThread();

						g_bInitReady = TRUE;
					}
				}
				break;

			case MASTERMSG_RESPONSE_AUTHSERVERINITINFO_STRING:
				{
					if( g_bInitReady == FALSE ) {

						sResponse_AuthServerInitInfo_String *pRecvMsg = (sResponse_AuthServerInitInfo_String *)pPacket;

						NMapString<40, 128>		kMapAuthServerInfo;
						kMapAuthServerInfo.CreateMap( 30 );
						kMapAuthServerInfo.Parse( pRecvMsg->szMapString );

						// server id 설정
						g_siServerID = pRecvMsg->siServerID;
                        
						g_accountDB.Init( kMapAuthServerInfo["AccountDBManagerIP"], _ttoi(kMapAuthServerInfo["AccountDBManagerPort"]), kMapAuthServerInfo["AccountDBIP"] );

						// FrontServer, GameServer Listen 설정
						g_agp.Init( _ttoi(kMapAuthServerInfo["GameServerListenPort"]) );
						g_afp.Init( _ttoi(kMapAuthServerInfo["FrontServerListenPort"]) );

						StringCchCopy( g_szServiceArea, sizeof(g_szServiceArea), kMapAuthServerInfo["ServiceArea"] );
						StringCchCopy( g_szServiceMode, sizeof(g_szServiceMode), kMapAuthServerInfo["ServiceMode"] );

						g_agp.CreateThread();
						g_afp.CreateThread();

						g_bInitReady = TRUE;
					}
				}
				break;

			}

			m_pAgentSession->GetRecvBuffer()->RemoveFirstPacket();
		}

	}

}

