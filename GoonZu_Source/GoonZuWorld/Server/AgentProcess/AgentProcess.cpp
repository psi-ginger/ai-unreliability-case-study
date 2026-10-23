#include "..\Server.h"
#include "AgentProcess.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "../Common/Char/CharManager/CharManager.h"

#include "../../../AdminSystem/MasterServer/MasterMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"

extern cltCommonLogic* pclClient;


AgentProcess::AgentProcess()
{
	m_pAgentSession = NULL;
	m_szIP[ 0 ] = NULL;

	m_bInitReady = FALSE;
}

AgentProcess::~AgentProcess()
{

}

void AgentProcess::Init( TCHAR *szIP, UI16 usPort )
{

	StringCchCopy( m_szIP, 16, szIP );
	m_usPort = usPort;

	m_sf.CreateFactory( &m_sm, 1, 50000, 50000 );

	ITZSession *pSession = m_sm.GetSession( 0 );

	InterlockedExchange( (LPLONG)&m_pAgentSession, (LONG)pSession );

	m_iocp.CreateIOCP( &m_sm, 1 );


}

void AgentProcess::Run()
{

	cltServer *pclServer = (cltServer *)pclClient;

	m_sm.Update();

	DWORD		currentTick;

	if( m_pAgentSession == NULL ) return;

	// 에이전트세션이 연결되지 않은 상태이면,
	if( m_pAgentSession->GetState() == SESSION_STATE_NONE )
	{
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

			StringCchCopy( sendMsg.szMessage, 20, TEXT("GameServer") );

			GetModuleFileName( 0, sendMsg.szFilePath, 127 );

			//printf( TEXT("%s\n"), sendMsg.szFilePath );

			m_pAgentSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			m_bSendFilePath = TRUE;
		}

		// heartbeat 보내기
		currentTick = GetTickCount();
		if( currentTick >= m_dwLastTick + AGENT_HEARTBEAT_INTERVAL ) {
			m_dwLastTick = currentTick;

			// 초기화 정보가 없는 상태이면 마스터로 초기화 정보를 요청한다
			if( m_bInitReady == FALSE ) {
				
				sRequest_GameServerInitInfo		sendMsg;

				m_pAgentSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
				
				// 초기화 정보 요청

			//tsoi 	printf(  TEXT("초기화 요청\n" ));

			} else {

				sReport_GameServerHeartbeat		sendMsg;

				sendMsg.packetHeader.usCmd = MASTERMSG_REPORT_GAMESERVER_HEARTBEAT;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.dwProcessId = GetCurrentProcessId();
  
				pclServer->blockRefresh();
				sendMsg.refreshInfo = *(pclServer->pRefreshInfo);

				pclServer->unblockRefresh();

				m_pAgentSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
				
			}

		}


		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)m_pAgentSession->GetRecvBuffer()->GetFirstPacket();		
		
		if( pPacket != NULL ) {
			// 패킷이 있으면,

			switch( pPacket->usCmd ) {

			case MASTERMSG_RESPONSE_GAMESERVERINITINFO:
				{

					if( m_bInitReady == FALSE ) {

						sResponse_GameServerInitInfo *pRecvMsg = (sResponse_GameServerInitInfo *)pPacket;

						pclServer->siServerID = pRecvMsg->siServerID;

						// 수신한 서버 정보를 토대로 다른 서버들과의 연결을 시도한다. 
						((cltServer*)pclClient)->SetOtherServer(
							pRecvMsg->szFrontServerIP,  pRecvMsg->usFrontServerPort, // Front 
							pRecvMsg->szAuthServerIP,		pRecvMsg->usAuthServerPort,		//Auth
							pRecvMsg->szGameDBManagerIP,	pRecvMsg->usGameDBManagerPort,	// DBManager
							pRecvMsg->usClientListenPort);									// Client Listen Port

						pclClient->siServerUnique = pRecvMsg->usServerUnique;
						if(pRecvMsg->usServerUnique == 0)
						{
							MessageBox(NULL, "Error [ServerUnique = 0] - Check ServerInfo.txt In MasterServer", "sResponse_GameServerInitInfo", MB_OK);
						}
					}

					m_bInitReady = TRUE;
				}
				break;

			case MASTERMSG_RESPONSE_NEWCHARNUM:
				{
					sResponse_NewCharNum *pRecvMsg = (sResponse_NewCharNum *)pPacket;

					SI32 totalnewcharnum = 0;
					SI32 totaldelcharnum = 0;

					for( int i = 0; i < pRecvMsg->siCount; ++i ) {
						if( pclServer->siServerID == pRecvMsg->siServerID[ i ] ) {
							pclServer->siServerNewCharNum = pRecvMsg->siNewCharNum[ i ];
							pclServer->siServerDelCharNum = pRecvMsg->siDelCharNum[ i ];
						}

						totalnewcharnum += pRecvMsg->siNewCharNum[ i ];
						totaldelcharnum += pRecvMsg->siDelCharNum[ i ];

					}

					pclServer->siTotalNewCharNum = totalnewcharnum;
					pclServer->siTotalDelCharNum = totaldelcharnum;

				}
				break;

			case MASTERMSG_REQUEST_GMORDER:
				{
					sRequest_GMOrder *pRecvMsg = (sRequest_GMOrder *)pPacket;

					BOOL result = pclServer->GMOrder( pRecvMsg->order );

					// 결과 전송
					sResponse_GMOrder clMsg( pRecvMsg->order, pRecvMsg->uiCharID, result, pRecvMsg->siWorldId);
					m_pAgentSession->GetSendBuffer()->Write((BYTE*)&clMsg);
				}
				break;

			case MASTERMSG_COMMAND_SERVER_OFF:
				{
					PostMessage(pclClient->GetHwnd(), WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
				}
				break;

			//==========================================================================
			// GMTOOL에서 온 신호
			case GOONZUGM_REQUEST_ACCOUNTCHARINFO:
				{
					sRequest_GMC_AccountCharInfo *pRecvMsg = (sRequest_GMC_AccountCharInfo *)pPacket;
					
					sDBRequest_AccountInfo clMsg( pRecvMsg->m_siCharID, pRecvMsg->m_szAccountID, 1);
					pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
				break;

			case GOONZUGM_REQUEST_USERCHARINFO:
				{
					sRequest_GMC_UserCharInfo *pRecvMsg = (sRequest_GMC_UserCharInfo *)pPacket;

					bool bconnectswitch = false;
					if(pclServer->pclCM->GetPersonIDFromName(pRecvMsg->m_szCharName))	{	bconnectswitch = true;	}
					
					sDBRequest_UserCharInfo clMsg( pRecvMsg->m_siCharID, pRecvMsg->m_szCharName, bconnectswitch, 0, 1);
					pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
				break;
			//==========================================================================
			}

			m_pAgentSession->GetRecvBuffer()->RemoveFirstPacket();

		}

	}

}

