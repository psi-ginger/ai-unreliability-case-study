#include "MasterClientProtocol.h"
#include "ServerInfo.h"
#include "AccessInfo.h"
#include "VersionInfo.h"
#include "MasterMsg.h"
#include "AccountDBProcess.h"
#include "ServerProcess.h"
#include "../../DBManager/AccountDBManager/AccountDBMsg.h"
#include "LogFile.h"

extern ServerInfo				g_serverinfo;
extern CAccessInfo				g_AccessInfo;

MasterClientProtocol::MasterClientProtocol()
{
	ZeroMemory(m_MasterClientSession, sizeof(m_MasterClientSession));
}


MasterClientProtocol::~MasterClientProtocol()
{

}


void MasterClientProtocol::Init()
{

	m_SF.CreateFactory( GetSessionManager(), 50, 200000, 200000 );

	for( int i = 0; i < 50; ++i ) {

		m_MasterClientSession[ i ] = GetSessionManager()->GetSession( i );

	}

	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	if( g_serverinfo.m_masterServerInfo.usClientListenPort <= 0 )
	{
		MessageBox( NULL, "ClientListPort is not Valid", "ERROR", MB_OK );
		return;
	}
	GetListen()->CreateListen( GetSessionManager(), g_serverinfo.m_masterServerInfo.usClientListenPort );
}

void MasterClientProtocol::Update()
{
	m_bRefreshInfoSendState = FALSE;

	// 4초 주기로 서버 갱신 정보 전송
	if( GetTickCount() - m_siRefreshInfoLastSendTick >= 4000 ) {
		m_siRefreshInfoLastSendTick = GetTickCount();
		m_bRefreshInfoSendState = TRUE;
		//g_serverinfo.printErrorServer();
	}
}

void MasterClientProtocol::SessionProc( ITZSession *pSession )
{
	cClient *pClient = (cClient *)pSession->GetPerson();

	if( NULL == pClient)
		return;

	// 주기적으로 서버 갱신 정보 보내기
	if( m_bRefreshInfoSendState == TRUE ) {

		pClient->m_dwLastSendTick = GetTickCount();

		sReport_RefreshServerInfo sendMsg;

		g_serverinfo.Wait();
		sendMsg.siMaxUserNum = g_maxUser;
		sendMsg.frontRefresh = g_serverinfo.m_frontRefresh;
		sendMsg.authRefresh = g_serverinfo.m_authRefresh;
		sendMsg.accountDBManagerRefresh = g_serverinfo.m_accountDBManagerRefresh;

		sendMsg.usCount = g_serverinfo.m_usWorldCount;
		if( sendMsg.usCount > MAX_WORLDINFO_NUMBER )		sendMsg.usCount = MAX_WORLDINFO_NUMBER;

		for( int i = 0; i < sendMsg.usCount; ++i ) {
			sendMsg.gameRefresh[ i ] = g_serverinfo.m_gameRefresh[ i ];
			sendMsg.gameDBManagerRefresh[ i ] = g_serverinfo.m_gameDBManagerRefresh[ i ];

			if( sendMsg.gameDBManagerRefresh[ i ].usStatus == 0 ) {
				int a = 0;
			}
		}

		g_serverinfo.Release();

		//	sendMsg.SetSize();

		pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

	}

	// 버퍼에서 패킷을 하나 꺼내온다 
	sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );

	if( pPacket == NULL ) return;										// 패킷이 없으면 나간다

	FILE*	fp = NULL;

	// 패킷 커맨드에 따른 처리
	switch( pPacket->usCmd )
	{

		// 로그인 
	case MASTERMSG_REQUEST_GMCLOGIN:
		{
			sRequest_GMCLogin *pRecvMsg = (sRequest_GMCLogin *)pPacket;

			fp = fopen("adminlogin.log", "a");
			if(fp)		{	fprintf(fp, "recv sRequest_GMCLogin : pRecvMsg->szID:%s, pRecvMsg->szPW:%s\n", pRecvMsg->szID, pRecvMsg->szPW);	fclose(fp);		}

			sDBRequest_AdminLogin	sendMsg1;

			sendMsg1.packetHeader.usCmd = DBMSG_REQUEST_ADMINLOGIN;
			sendMsg1.packetHeader.usSize = sizeof( sendMsg1 );

			memcpy( sendMsg1.userid, pRecvMsg->szID, ConstIDSize );
			memcpy( sendMsg1.userpw, pRecvMsg->szPW, 20 );
			//memcpy( sendMsg1.userip, pSession->GetIP(), 20 );
			//sendMsg1.userip[ 19 ] = NULL;

			// 임시로 로그인 정보 넣기 
			memcpy(pClient->m_szID, pRecvMsg->szID, ConstIDSize);

			sendMsg1.usMasterCharID = pSession->GetPerson()->GetCharID();

			g_ap.GetSession()->GetSendBuffer()->Write( (BYTE *)&sendMsg1 );

			fp = fopen("adminlogin.log", "a");
			if(fp)		{	fprintf(fp, "send sDBRequest_AdminLogin : sendMsg1.usMasterCharID:%d, sendMsg1.userid:%s, sendMsg1.userpw:%s\n", sendMsg1.usMasterCharID, sendMsg1.userid, sendMsg1.userpw);	fclose(fp);		}

		}
		break;

	case MASTERMSG_REQUEST_SERVERINFO1:
		{
			g_serverinfo.Wait();

			sResponse_ServerInfo1	sendMsg2;

			sendMsg2.frontServerInfo = g_serverinfo.m_frontServerInfo;
			sendMsg2.authServerInfo = g_serverinfo.m_authServerInfo;
			sendMsg2.accountDBManagerInfo = g_serverinfo.m_accountDBManagerInfo;
			sendMsg2.accountDBInfo = g_serverinfo.m_accountDBInfo;

			pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg2 );

			g_serverinfo.Release();

		}
		break;

	case MASTERMSG_REQUEST_SERVERINFO2:
		{
			g_serverinfo.Wait();

			sResponse_ServerInfo2	sendMsg3;

			sendMsg3.usCount = g_serverinfo.m_usWorldCount;
			if( sendMsg3.usCount > MAX_WORLDINFO_NUMBER )		sendMsg3.usCount = MAX_WORLDINFO_NUMBER;

			//memcpy( sendMsg3.gameWorldInfo, g_serverinfo.m_gameWorldInfo,
			//	sizeof( sGameWorldInfo ) * sendMsg3.usCount );
			for( SI32 i=0; i<sendMsg3.usCount; i++ )
			{
				memcpy( &sendMsg3.gameWorldInfo[i], &g_serverinfo.m_gameWorldInfo[i], sizeof(sGameWorldInfo) );
			}

			sendMsg3.SetSize();

			pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg3 );

			pClient->m_dwLastSendTick -= 5000;

			g_serverinfo.Release();

		}
		break;

	case MASTERMSG_REQUEST_GMCDAYUSERNUM:
		{
			sRequest_GMCDAYUSERNUM *pMsg = (sRequest_GMCDAYUSERNUM *)pPacket;

			sResponse_GMCDAYUSERNUM sendDayUserNum;

			logFile->CriTicalWait();
			sendDayUserNum.siResult = logFile->ReadLogFileEX( pMsg->siYear, pMsg->siMonth, pMsg->siDay );

			sendDayUserNum.siYear = pMsg->siYear ;
			sendDayUserNum.siMonth = pMsg->siMonth ;
			sendDayUserNum.siDay = pMsg->siDay ;
			sendDayUserNum.siLayer = pMsg->siLayer;

			for ( int i = 0 ; i < 288 ; i ++ )
				sendDayUserNum.siUserNum[i] = logFile->m_iServerTotalUseNum[i];

			logFile->CriTicalRelease();

			pSession->GetSendBuffer()->Write( (BYTE *)&sendDayUserNum );

		}
		break;

	case GOONZUMC_REQUEST_SERVER_ON:
		{
			sRequest_GMCServerOn *pMsg = (sRequest_GMCServerOn*)pPacket;

			ITZSession *pSession = g_sp.m_serverSession[ 4 + ( pMsg->siServerID * 3 ) ];

			if( pSession ) 
			{
				if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
				{
					sCommand_ServerOn sendMsg;
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ 4 + ( pMsg->siServerID * 3 ) ].siServerID;
					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_SERVER_OFF:
		{
			sRequest_GMCServerOff *pMsg = (sRequest_GMCServerOff*)pPacket;

			ITZSession *pSession = g_sp.m_serverSession[ 4 + ( pMsg->siServerID * 3 ) ];

			if( pSession ) 
			{
				if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) 
				{
					sCommand_ServerOff sendMsg;
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ 4 + ( pMsg->siServerID * 3 ) ].siServerID;
					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_ALL_SERVER_ON:
		{
			for( int i = 0; i < g_serverinfo.GetWorldCount(); ++i )
			{
				ITZSession *pSession = g_sp.m_serverSession[ 4 + ( i * 3 ) ];

				if( pSession ) 
				{
					if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						sCommand_ServerOn sendMsg;
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ 4 + ( i * 3 ) ].siServerID;
						//	sendMsg.dwProcessId = g_serverinfo.m_frontRefresh.dwProcessId;

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_ALL_SERVER_OFF:
		{
			for( int i = 0; i < g_serverinfo.GetWorldCount(); ++i )
			{
				ITZSession *pSession = g_sp.m_serverSession[ 4 + ( i * 3 ) ];

				if( pSession ) 
				{
					if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						sCommand_ServerOff sendMsg;
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ 4 + ( i * 3 ) ].siServerID;
						//	sendMsg.dwProcessId = g_serverinfo.m_frontRefresh.dwProcessId;

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_GAMEDBMANAGER_ON:
		{
			sRequest_GMCGameDBManagerOn *pMsg = (sRequest_GMCGameDBManagerOn*)pPacket;

			ITZSession *pSession = g_sp.m_serverSession[ 5 + ( pMsg->siServerID * 3 ) ];

			if( pSession ) 
			{
				if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
				{
					sCommand_GameDBManagerOn sendMsg;
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ 5 + ( pMsg->siServerID * 3 ) ].siServerID;
					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_GAMEDBMANAGER_OFF:
		{
			sRequest_GMCServerOff *pMsg = (sRequest_GMCServerOff*)pPacket;

			ITZSession *pSession = g_sp.m_serverSession[ 5 + ( pMsg->siServerID * 3 ) ];

			if( pSession ) 
			{
				if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) 
				{
					sCommand_GameDBManagerOff sendMsg;
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ 5 + ( pMsg->siServerID * 3 ) ].siServerID;
					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_ON:
		{
			for( int i = 0; i < g_serverinfo.GetWorldCount(); ++i )
			{
				ITZSession *pSession = g_sp.m_serverSession[ 5 + ( i * 3 ) ];

				if( pSession ) 
				{
					if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						sCommand_GameDBManagerOn sendMsg;
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ 5 + ( i * 3 ) ].siServerID;
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
				}
			}
		}
		break;

	case GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_OFF:
		{
			for( int i = 0; i < g_serverinfo.GetWorldCount(); ++i )
			{
				ITZSession *pSession = g_sp.m_serverSession[ 5 + ( i * 3 ) ];

				if( pSession ) 
				{
					if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						sCommand_GameDBManagerOff sendMsg;
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ 5 + ( i * 3 ) ].siServerID;
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
				}
			}
		}
		break;

		/*	case GOONZUMC_REQUEST_FRONTSERVERON:
		{

		ITZSession *pSession = g_sp.m_serverSession[ SERVERINFO_FRONTSERVER_INDEX ];

		if( pSession ) {

		if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) {

		sCommand_ServerStart startMsg;
		startMsg.siServerID = g_serverinfo.m_serverinfo[ SERVERINFO_FRONTSERVER_INDEX ].siServerID;
		startMsg.dwProcessId = g_serverinfo.m_frontRefresh.dwProcessId;

		pSession->GetSendBuffer()->Write( (BYTE *)&startMsg );
		}
		}
		}
		break;

		case GOONZUMC_REQUEST_FRONTSERVEROFF:
		{

		ITZSession *pSession = g_sp.m_serverSession[ SERVERINFO_FRONTSERVER_INDEX ];

		if( pSession ) {

		if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) {

		sCommand_ServerStop stopMsg;
		stopMsg.siServerID = g_serverinfo.m_serverinfo[ SERVERINFO_FRONTSERVER_INDEX ].siServerID;
		stopMsg.dwProcessId = g_serverinfo.m_frontRefresh.dwProcessId;

		pSession->GetSendBuffer()->Write( (BYTE *)&stopMsg );
		}
		}
		}
		break;
		*/
	case GOONZUMC_REQUEST_SERVERCOMMAND:
		{

		}
		break;

	case GOONZUMC_REQUEST_GMORDER:
		{
			sRequest_GMC_GMOrder *pRecvMsg = (sRequest_GMC_GMOrder *)pPacket;
			
			// 서버 식별 ID
			UI32 uiCharID = pSession->GetPerson()->GetCharID();

			// 접근 가능 아이피인지 검사한다.
			if(g_AccessInfo.ConfirmAccessMC(pSession->m_szIP) == FALSE)
			{
				const SI32 ConstTmpBufSize = 64;

				//시간 구하기
				SYSTEMTIME systime;
				GetLocalTime(&systime);

				// 요청자 아이디
				TCHAR szID[ConstTmpBufSize]={0,};
				// 요청자 아이피
				TCHAR szIP[ConstTmpBufSize]={0,};
				// 요청 완료 시간
				TCHAR szTime[ConstTmpBufSize]={0,};
				// 서버 이름
				TCHAR szServerName[ConstTmpBufSize]={0,};

				// 요청자
				cClient* pRequester = g_mcp.m_SF.GetPerson(uiCharID);

				ITZSession* pRequestSession = NULL;
				if(pRequester != NULL)
				{
					StringCchCopy(szID,ConstTmpBufSize,pRequester->m_szID);

					// 요청자 세션
					pRequestSession = pRequester->GetSession();
				}

				if(pRequestSession != NULL)
				{
					StringCchCopy(szIP,ConstTmpBufSize,pRequestSession->m_szIP);
				}

				StringCchPrintf(szTime,ConstTmpBufSize,TEXT("%d.%d.%d.%d:%d:%d"),
					systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);


				sResponse_GMC_GMOrder clMsg(pRecvMsg->order,
					pRecvMsg->siWorldId,
					szID ,
					szIP,
					szTime,
					GMORDER_RESULT_DENY
					);
				pSession->GetSendBuffer()->Write( (BYTE*)&clMsg );
				break;
			}


			cClient *pClient = g_mcp.m_SF.GetPerson(uiCharID);
			if( pClient == NULL || pClient->m_nMasterLevel < 102)		break;

			// 전송할 게임 서버
			ITZSession* pGameServer = g_sp.GetGameServerSession(pRecvMsg->siWorldId);

			// 전송
			sRequest_GMOrder sendMsg( pRecvMsg->order ,uiCharID, pRecvMsg->siWorldId);
			pGameServer->GetSendBuffer()->Write((BYTE *)&sendMsg);
		}
		break;

	case GOONZUMC_REQUEST_MAKEFILE:
		{
			sRequest_GMC_MakeFile *pRecvMsg = (sRequest_GMC_MakeFile *)pPacket;

			sResponse_GMC_MakeFile sendMsg( pRecvMsg->GetFileName() );

			g_sp.SendPacketAll( (char *)&sendMsg );
		}
		break;

	case GOONZUMC_REQUEST_MAKEDIRECTORY:
		{
			sRequest_GMC_MakeDirectory *pRecvMsg = (sRequest_GMC_MakeDirectory *)pPacket;

			sResponse_GMC_MakeDirectory sendMsg( pRecvMsg->GetDirectoryName() );

			g_sp.SendPacketAll( (char *)&sendMsg );
		}
		break;

	case GOONZUMC_REQUEST_SEND_FILEDATA:
		{
			sRequest_GMC_SendFileData *pRecvMsg = (sRequest_GMC_SendFileData *)pPacket;

			sResponse_GMC_SendFileData sendMsg( pRecvMsg->m_siSendCount,
				pRecvMsg->m_siReadData,
				pRecvMsg->m_szFileName,
				pRecvMsg->m_cData );

			g_sp.SendPacketAll( (char *)&sendMsg );
		}
		break;

	case GOONZUGM_REQUEST_ACCOUNTCHARINFO:
		{
			sRequest_GMC_AccountCharInfo *pRecvMsg = (sRequest_GMC_AccountCharInfo *)pPacket;

			pRecvMsg->m_siCharID = pSession->GetPerson()->GetCharID();

			g_sp.SendPacketAll( (char *)pRecvMsg );
		}
		break;

	case GOONZUGM_REQUEST_USERCHARINFO:
		{
			sRequest_GMC_UserCharInfo *pRecvMsg = (sRequest_GMC_UserCharInfo *)pPacket;

			pRecvMsg->m_siCharID = pSession->GetPerson()->GetCharID();

			g_sp.SendPacketAll( (char *)pRecvMsg );
		}
		break;

	default:
		break;
	}

	pSession->GetRecvBuffer()->RemoveFirstPacket();
}
