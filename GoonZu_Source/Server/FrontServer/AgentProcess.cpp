#include "FrontServer.h"

#include "FrontGameProtocol.h"
#include "FrontClientProtocol.h"

#include "AgentProcess.h"
#include "AuthProcess.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../../AdminSystem/MasterServer/MasterMsg.h"
#include "../../NLib/NMap.h"

#include "Log.h"

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

	m_sf.CreateFactory( &m_sm, 1, 50000, 50000 );

	ITZSession *pSession = m_sm.GetSession( 0 );
	
	InterlockedExchange( (LPLONG)&m_pAgentSession, (LONG)pSession );

	m_iocp.CreateIOCP( &m_sm, 1 );

	g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "AgentProcess::Init(%s, %d)", szIP, usPort );
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

			strcpy( sendMsg.szMessage, "FrontServer" );

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

				//sRequest_FrontServerInitInfo	sendMsg;
				sRequest_FrontServerInitInfo_String		sendMsg;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

				sRequest_VersionInfo	sendMsg2;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg2 );

			} else { 

				
				sReport_FrontServerHeartbeat		sendMsg;

				sendMsg.packetHeader.usCmd = MASTERMSG_REPORT_FRONTSERVER_HEARTBEAT;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.dwProcessId = GetCurrentProcessId();
				sendMsg.refreshInfo.siAuthConnect = 
					(SI32)( g_auth.m_pAuthSession->GetState() == SESSION_STATE_ESTABLISHED );

				if(g_fgp.GetSessionManager() != NULL)
				{
					sendMsg.refreshInfo.siGameServerConnect = g_fgp.GetSessionManager()->GetActiveSessionCount();
				}
				else
				{
					sendMsg.refreshInfo.siGameServerConnect = 0;
				}

				sendMsg.refreshInfo.siGameVersion = g_versionList[ g_versionCount - 1 ];
				if(g_fcp.GetSessionManager() != NULL)
				{
					sendMsg.refreshInfo.siUserNum = g_fcp.GetSessionManager()->GetActiveSessionCount();
				}
				else
				{
					sendMsg.refreshInfo.siUserNum = 0;
				}
				sendMsg.refreshInfo.usStatus = 0;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );				

			}

		}


		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pAgentSession->GetRecvBuffer()->GetFirstPacket();		
		
		if( pPacket != NULL ) {										
			// 패킷이 있으면,

			switch( pPacket->usCmd ) {

			case MASTERMSG_RESPONSE_FRONTSERVERINITINFO:
				{
					g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "Recv MASTERMSG_RESPONSE_FRONTSERVERINITINFO" );

					EnterCriticalSection( &g_cs );

					if( g_bInitReady == FALSE ) {

						sResponse_FrontServerInitInfo *pRecvMsg = (sResponse_FrontServerInitInfo *)pPacket;

						if(pRecvMsg->packetHeader.usSize != sizeof(sResponse_FrontServerInitInfo))
						{
							g_CLogPrint.WriteFileLog("ErrorLog.txt", __FILE__, __LINE__, "Recv=%d;Want=%d;Msg=sResponse_FrontServerInitInfo PacketSize Error",
								pRecvMsg->packetHeader.usSize, sizeof(sResponse_FrontServerInitInfo));
							break;
						}

						// server id 설정
						g_siServerID = pRecvMsg->siServerID;

						// authserver ip / port 설정 
						g_auth.Init( pRecvMsg->szAuthServerIP, pRecvMsg->usAuthServerPort );

						// 연결 기다림 초기화
						g_fcp.Init(pRecvMsg->usClientListenPort);
						g_fgp.Init(pRecvMsg->usGameServerListenPort);

						g_fcp.CreateThread();
						g_fgp.CreateThread();

						// world list 정보 설정
						WORLDINFO worldInfo;

						for( int i = 0; i < pRecvMsg->count; ++i ) {

							ZeroMemory( &worldInfo, sizeof(worldInfo) );
						
							worldInfo.baseinfo.worldId = pRecvMsg->worldinfo[ i ].usWorldID;
							strcpy( worldInfo.baseinfo.worldname, pRecvMsg->worldinfo[ i ].szWorldName );
							worldInfo.baseinfo.siServerID = pRecvMsg->worldinfo[ i ].siServerID;
							strcpy( worldInfo.baseinfo.gameserverip, pRecvMsg->worldinfo[ i ].szGameServerIP );
							worldInfo.baseinfo.gameserverport = pRecvMsg->worldinfo[ i ].usGameServerPort;
							strcpy( worldInfo.baseinfo.opendate, pRecvMsg->worldinfo[ i ].szOpenDate );
							strcpy( worldInfo.baseinfo.shortmessage, pRecvMsg->worldinfo[ i ].szShortMessage );

							worldInfo.refreshinfo.worldId = pRecvMsg->worldinfo[ i ].usWorldID;
							worldInfo.refreshinfo.state = 0;
							worldInfo.refreshinfo.currentConnections = 0;

							g_worldlist.SetWorldInfo( pRecvMsg->worldinfo[ i ].usWorldID, &worldInfo );

						}
					}

					LeaveCriticalSection( &g_cs );

					g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "End MASTERMSG_RESPONSE_FRONTSERVERINITINFO" );
				}
				break;

			case MASTERMSG_RESPONSE_FRONTSERVERINITINFO_STRING:
				{
					EnterCriticalSection( &g_cs );

					if( g_bInitReady == FALSE ) {

						sResponse_FrontServerInitInfo_String *pRecvMsg = (sResponse_FrontServerInitInfo_String *)pPacket;

						NMapString<40, 128>		kMapFrontServerInfo;
						kMapFrontServerInfo.CreateMap( 30 );
						kMapFrontServerInfo.Parse( pRecvMsg->szMapString );

						// server id 설정
						g_siServerID = pRecvMsg->siServerID;

						StringCchCopy( g_szServiceArea, sizeof(g_szServiceArea), kMapFrontServerInfo["ServiceArea"] );
						StringCchCopy( g_szServiceMode, sizeof(g_szServiceMode), kMapFrontServerInfo["ServiceMode"] );

						// authserver ip / port 설정 
						g_auth.Init( kMapFrontServerInfo["AuthServerIP"], _ttoi(kMapFrontServerInfo["AuthServerPort"]) );

						// 연결 기다림 초기화
						g_fcp.Init( _ttoi(kMapFrontServerInfo["ClientListenPort"]) );
						g_fgp.Init( _ttoi(kMapFrontServerInfo["GameServerListenPort"]) );

						g_fcp.CreateThread();
						g_fgp.CreateThread();

						// world list 정보 설정
						WORLDINFO worldInfo;

						NTCHARString<256>		kMapWorldString;
						NMapString<40, 128>		kMapWorldInfo;
						kMapWorldInfo.CreateMap( 30 );

						for( int i = 0; i < pRecvMsg->count; ++i ) {

							kMapWorldInfo.ClearAll();
							ZeroMemory( &worldInfo, sizeof(worldInfo) );
							kMapWorldInfo.Parse( pRecvMsg->szWorldMapStr[i]	);

							worldInfo.baseinfo.worldId			= _ttoi( kMapWorldInfo["WorldID"] );
							StringCchCopy(worldInfo.baseinfo.worldname, sizeof(worldInfo.baseinfo.worldname), kMapWorldInfo["WorldName"]);
							worldInfo.baseinfo.siServerID		= _ttoi( kMapWorldInfo["ServerID"] );
							StringCchCopy(worldInfo.baseinfo.gameserverip, sizeof(worldInfo.baseinfo.gameserverip), kMapWorldInfo["GameServerIP"]);
							worldInfo.baseinfo.gameserverport	= _ttoi(kMapWorldInfo["GameServerPort"]);
							StringCchCopy(worldInfo.baseinfo.opendate, sizeof(worldInfo.baseinfo.opendate), kMapWorldInfo["OpenDate"]);
							StringCchCopy(worldInfo.baseinfo.shortmessage, sizeof(worldInfo.baseinfo.shortmessage), kMapWorldInfo["ShortMessage"]);

							worldInfo.refreshinfo.worldId		= _ttoi( kMapWorldInfo["WorldID"] );
							worldInfo.refreshinfo.state = 0;
							worldInfo.refreshinfo.currentConnections = 0;

							g_worldlist.SetWorldInfo( worldInfo.baseinfo.worldId, &worldInfo );
						}
					}

					LeaveCriticalSection( &g_cs );

					g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "End MASTERMSG_RESPONSE_FRONTSERVERINITINFO" );
				}
				break;

			case MASTERMSG_RESPONSE_VERSIONINFO:
				{
					g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "Recv MASTERMSG_RESPONSE_VERSIONINFO" );

					EnterCriticalSection( &g_cs );

					if( g_bInitReady == FALSE ) {

						sResponse_VersionInfo	*pRecvMsg = (sResponse_VersionInfo *)pPacket;

						g_versionCount = pRecvMsg->count;

						for( UI32 i = 0; i < pRecvMsg->count; ++i ) {

							g_versionList[ i ] = pRecvMsg->versioninfo[ i ];
						}

					}

					g_bInitReady = TRUE;

					LeaveCriticalSection( &g_cs );

					g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "End MASTERMSG_RESPONSE_VERSIONINFO" );
				}
				break;

			}

			m_pAgentSession->GetRecvBuffer()->RemoveFirstPacket();
		}

	}

}

