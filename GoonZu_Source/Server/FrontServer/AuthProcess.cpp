#include "FrontServer.h"
#include "AuthProcess.h"
#include "FrontClientProtocol.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "Log.h"

AuthProcess::AuthProcess()
{
	m_pAuthSession = NULL;
	m_szIP[ 0 ] = NULL;

}

AuthProcess::~AuthProcess()
{

}

void AuthProcess::Init( char *szIP, UI16 usPort )
{
	strcpy( m_szIP, szIP );
	m_usPort = usPort;

	m_sf.CreateFactory( &m_sm, 1, 20 * 1000 * 1000, 20 * 1000 * 1000 );

	ITZSession *pSession = m_sm.GetSession( 0 );

	InterlockedExchange( (LPLONG)&m_pAuthSession, (LONG)pSession );

	m_iocp.CreateIOCP( &m_sm, 1 );

	g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "AuthProcess::Init();IP=%s;Port=%d", szIP, usPort);
}

void AuthProcess::Run()
{
	
	int i;

	m_sm.Update();

	if( m_pAuthSession->GetState() == SESSION_STATE_NONE ) 
	{
		if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pAuthSession ) 
		{
		} 
		else 
		{
		}
	}

	if( m_pAuthSession->GetState() == SESSION_STATE_ESTABLISHED ) 
	{		

		sPacketHeader *pPacket;
		SI32 count = m_pAuthSession->GetRecvBuffer()->GetPacketCount();

		for( i = 0; i < count; ++i ) 
		{
			// 버퍼에서 패킷을 하나 꺼내온다
			pPacket = (sPacketHeader *)m_pAuthSession->GetRecvBuffer()->GetFirstPacket();		
			
			if( pPacket != NULL ) {										
				// 패킷이 있으면,

				switch( pPacket->usCmd ) {

				case AUTHMSG_RESPONSE_FRONTAUTH:
					{
						g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Msg=Recv AUTHMSG_RESPONSE_FRONTAUTH");

//						printf( "인증서버로 부터 인증 패킷 받음!\n" );
						sResponse_FrontAuth *pRecvMsg = (sResponse_FrontAuth *)pPacket;
						if(pRecvMsg->packetHeader.usSize != sizeof(sResponse_FrontAuth))
						{
							g_CLogPrint.WriteFileLog("ErrorLog.txt", __FILE__, __LINE__, "Recv=%d;Want=%d;Msg=sResponse_FrontAuth PacketSize Error",
											pRecvMsg->packetHeader.usSize, sizeof(sResponse_FrontAuth));
							break;
						}
						
						printf( "UserId = %s , %d , %d \n", pRecvMsg->userid,pRecvMsg->usFrontCharID, pRecvMsg->siAuthKey );

						sResponse_LogIn sendMsg;

						sendMsg.packetHeader.usSize = sizeof( sendMsg );
						sendMsg.packetHeader.usCmd	= FRONTMSG_RESPONSE_LOGIN;

						sendMsg.usResult	= pRecvMsg->usResult;
						sendMsg.siAuthKey	= pRecvMsg->siAuthKey;

						// 넥슨 채널링을 위한 ID체크 -넥슨은 DB에서 ID가 변환되어 들어와야 한다.
						memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
						sendMsg.szBlockReason[ 19 ] = NULL;
						
						sendMsg.siGameMasterLevel	= pRecvMsg->siGameMasterLevel;
						sendMsg.siAccountStatus		= pRecvMsg->siAccountStatus;

						sendMsg.siBlockMode		= pRecvMsg->siBlockMode;
						sendMsg.siBlockYear		= pRecvMsg->siBlockYear;
						sendMsg.siBlockMonth	= pRecvMsg->siBlockMonth;
						sendMsg.siBlockDay		= pRecvMsg->siBlockDay;
						sendMsg.siBlockHour		= pRecvMsg->siBlockHour;
						sendMsg.siBlockMinute	= pRecvMsg->siBlockMin;
						
						memcpy( sendMsg.szBlockReason, pRecvMsg->szBlockReason, 30 );
						sendMsg.szBlockReason[ 29 ] = NULL;

						sendMsg.bAlreadyMakeCharSwitch = pRecvMsg->bAlreadyMakeCharSwitch;

						memcpy( sendMsg.szServerName, pRecvMsg->szServerName, 20 );
						sendMsg.szServerName[ 19 ] = NULL;

						sendMsg.m_uiRealAge		= pRecvMsg->m_uiRealAge;

						cClient *pClient = g_fcp.m_SF.GetPerson( pRecvMsg->usFrontCharID );

						if( pClient == NULL )													break;
						if( pClient->GetSession() == NULL )										break;
						if( pClient->GetSession()->GetState() != SESSION_STATE_ESTABLISHED )	break;

						UI16 usUnique = 0;

						EnterCriticalSection( &pClient->m_cs );

						usUnique = pClient->m_siUnique;

						LeaveCriticalSection( &pClient->m_cs );

						if( usUnique != pRecvMsg->usPacketUnique )								break;

						pClient->GetSendBuffer()->Write( (BYTE *)&sendMsg );
//						printf( "클라이언트에 로그인 응답 패킷 보냄\n" );

						g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Msg=Send FRONTMSG_RESPONSE_LOGIN");
					}
					break;

				case AUTHMSG_RESPONSE_FRONTJOINGAME:
					{
						g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Msg=Recv AUTHMSG_RESPONSE_FRONTJOINGAME");

//						printf( "인증서버로 부터 게임가입 패킷 받음\n" );
						sResponse_FrontJoinGame *pRecvMsg = (sResponse_FrontJoinGame *)pPacket;

						if(pRecvMsg->packetHeader.usSize != sizeof(sResponse_FrontJoinGame))
						{
							g_CLogPrint.WriteFileLog("ErrorLog.txt", __FILE__, __LINE__, "Recv=%d;Want=%d;Msg=sResponse_FrontJoinGame PacketSize Error",
								pRecvMsg->packetHeader.usSize, sizeof(sResponse_FrontJoinGame));
							break;
						}

						sResponse_JoinGame sendMsg;

						sendMsg.packetHeader.usCmd = FRONTMSG_RESPONSE_JOINGAME;
						sendMsg.packetHeader.usSize = sizeof( sendMsg );

						sendMsg.siResult = pRecvMsg->siResult;

						cClient *pClient = g_fcp.m_SF.GetPerson( pRecvMsg->usFrontCharID );

						if( pClient == NULL )													break;
						if( pClient->GetSession() == NULL )										break;
						if( pClient->GetSession()->GetState() != SESSION_STATE_ESTABLISHED )	break;

						UI16 usUnique = 0;

						LeaveCriticalSection( &pClient->m_cs );

						usUnique = pClient->m_siUnique;

						LeaveCriticalSection( &pClient->m_cs );

						if( usUnique != pRecvMsg->usPacketUnique ) break;

						pClient->GetSendBuffer()->Write( (BYTE *)&sendMsg );

						g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Msg=Send FRONTMSG_RESPONSE_JOINGAME");

					}
					break;

				}

				// 처리 후 패킷을 버퍼에서 지운다
				m_pAuthSession->GetRecvBuffer()->RemoveFirstPacket();
			}
		}
	}
}
