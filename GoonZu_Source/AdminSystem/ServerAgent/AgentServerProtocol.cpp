#include "AgentMasterProtocol.h"
#include "AgentServerProtocol.h"
#include "SessionList.h"

#include "ServerAgent.h"

#include "../MasterServer/MasterMsg.h"

AgentServerProtocol::AgentServerProtocol()
{

}

AgentServerProtocol::~AgentServerProtocol()
{

}

void AgentServerProtocol::Init()
{

	m_SF.CreateFactory( GetSessionManager(), 20, 200000, 200000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), 6400 );

}

void AgentServerProtocol::Update()
{
	
}

void AgentServerProtocol::SessionProc( ITZSession *pSession )
{

	cServer *pServer = (cServer *)(pSession->GetPerson());


	// 버퍼에서 패킷을 하나 꺼내온다 
	sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																							
	if( pPacket == NULL ) return;										// 패킷이 없으면 나간다

	// 마스터서버로 바로 전달 해야할 메시지이면,
	if( pPacket->usCmd >= 10000 ) {

		if(pPacket->usCmd >= 13000)
		{
			int a = 0;
		}
		if(pPacket->usCmd >= 13000)
		{
			int a = 0;
		}
		if( pServer->m_pMaster != NULL ) {

			if( pServer->m_pMaster->GetSession()->GetState() == SESSION_STATE_ESTABLISHED ) {

				pServer->m_pMaster->GetSession()->GetSendBuffer()->Write( (BYTE *)pPacket );
			}
		}

	} else {	

	// 패킷 커맨드에 따른 처리
	switch( pPacket->usCmd ) {

	case AGENTMSG_REPORT_FILEPATH:
		{
			sReportToAgent_FilePath *pRecvMsg = (sReportToAgent_FilePath *)pPacket;

			pServer->SetFilePath( pRecvMsg->szFilePath );

			g_sessionList.Wait();

			// 마스터 리스트에서 폴더와파일명이 같은 것을 찾아서 엮어준다
			for( int i = 0; i < g_amp.m_SF.m_siMaxSize; ++i ) {
				
				cMaster *pMaster = (cMaster *)(g_amp.GetSessionManager()->GetSession( i )->GetPerson() );

				if( pMaster->IsEqualFilePath( pRecvMsg->szFilePath ) == TRUE ) {

					pMaster->SetServer( pServer );

					pServer->SetMaster( pMaster );
					pServer->SetServerID( pMaster->GetServerID() );

					printf( "ok!!! %s\n", pServer->m_szFilePath );

				}
			}

			g_sessionList.AddServerSession( pServer, pRecvMsg->szMessage );

			g_sessionList.Release();

		}
		break;
	} // switch
	} // if

	pSession->GetRecvBuffer()->RemoveFirstPacket();
}

