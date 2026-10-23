#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

#include "ServerProcess.h"
//#include "ServerInfo.h"
#include "VersionInfo.h"
#include "MasterMsg.h"

#include "MasterClientProtocol.h"
#include "../../DBManager/LogDBManager/LogDBMsg.h"
#include "ToolsLog.h"

#include "../../NLib/NMap.h"

#include "../../Server/FrontServer/Log.h"

extern UserCountInfo			g_UserCountInfo;


ServerProcess::ServerProcess()
{
}


ServerProcess::~ServerProcess()
{

}


void ServerProcess::Init()
{

	m_sf.CreateFactory( &m_sm, MAX_SERVER_COUNT, 50000, 50000 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	for( int i = 0; i < MAX_SERVER_COUNT; ++i ) {

		m_serverSession[ i ] = m_sm.GetSession( i );

	}

	m_bStartFrontServer = FALSE;

	m_dwLastNewCharNum = GetTickCount();
}


void ServerProcess::Run()
{
	ITZSession	*pSession;
	cAgent		*pAgent;
	UI16		usWorldID;

	m_sm.Update();

	int i;

	DWORD dwCurrentTick = GetTickCount();

	if( dwCurrentTick - m_dwLastChkTick > 30 * 1000 ) {
		m_dwLastChkTick = dwCurrentTick;

		SYSTEMTIME systime;
		GetLocalTime( &systime );

	}

	if( m_bStartFrontServer == TRUE ) {

		if( GetTickCount() - m_dwStartFrontServerTick >= 10 * 1000 ) {

			m_bStartFrontServer = FALSE;
		}
	}


	// Heartbeat 시간 초과 여부 체크 ------------------------------------------
	g_serverinfo.Wait();

	if( dwCurrentTick - g_serverinfo.m_dwFrontServerTick >= 8000 ) {
		g_serverinfo.m_frontRefresh.usStatus = SERVERSTATUS_NOHEARTBEAT;
	}


	if( dwCurrentTick - g_serverinfo.m_dwAuthServerTick >= 8000 ) {
		g_serverinfo.m_authRefresh.usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	if( dwCurrentTick - g_serverinfo.m_dwAccountDBManagerTick >= 8000 ) {
		g_serverinfo.m_accountDBManagerRefresh.usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	for( i = 0; i < g_serverinfo.GetServerCount(); ++i ) {

		usWorldID = g_serverinfo.m_serverinfo[ i ].usWorldID;

		if( usWorldID == 0 ) continue;

		if( dwCurrentTick - g_serverinfo.m_dwGameServerTick[ usWorldID - 1 ] >= 8000 ) {
			g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usStatus = SERVERSTATUS_NOHEARTBEAT;
			g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usCurrentNPCNum = 0;
			g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usCurrentUserNum = 0;
		}

		if( dwCurrentTick - g_serverinfo.m_dwGameDBManagerTick[ usWorldID - 1 ] >= 8000 ) {
			g_serverinfo.m_gameDBManagerRefresh[ usWorldID - 1 ].usStatus = SERVERSTATUS_NOHEARTBEAT;
		}
	}


	BOOL bSendNewCharNum = FALSE;

	sResponse_NewCharNum sendNewCharNum;
	ZeroMemory( &sendNewCharNum, sizeof( sendNewCharNum ) );

	sendNewCharNum.packetHeader.usCmd = MASTERMSG_RESPONSE_NEWCHARNUM;
	sendNewCharNum.packetHeader.usSize = sizeof( sendNewCharNum );

	// 서버로 주기적으로 보낼 캐릭터 생성 숫자
	if( dwCurrentTick - m_dwLastNewCharNum >= 5 * 60 * 1000 ) {
		m_dwLastNewCharNum = dwCurrentTick;
		bSendNewCharNum = TRUE;

		sendNewCharNum.siCount = g_serverinfo.GetWorldCount();

		SI32 totalnewcharnum = 0;
		for( int i = 0; i < g_serverinfo.GetWorldCount(); ++i ) {
			sendNewCharNum.siServerID[ i ] = g_serverinfo.m_gameWorldInfo[ i ].gameServerInfo.siServerID;
			sendNewCharNum.siNewCharNum[ i ] = g_serverinfo.m_siNewCharNum[ i ];
			sendNewCharNum.siDelCharNum[ i ] = g_serverinfo.m_siDelCharNum[ i ];

			totalnewcharnum += sendNewCharNum.siNewCharNum[ i ];
		}

		if( totalnewcharnum == 0 )bSendNewCharNum = FALSE;
	}

	g_serverinfo.Release();

	//-------------------------------------------------------------------------

	for( i = 0; i < g_serverinfo.GetServerCount(); ++i ) {

		if( i >= MAX_SERVER_COUNT - 1 ) break;

		pSession = m_serverSession[ i ];

		if( pSession == NULL ) continue;

		pAgent = (cAgent *)m_serverSession[ i ]->GetPerson();

		if( pAgent == NULL ) continue;

		g_serverinfo.Wait();
		usWorldID = g_serverinfo.m_serverinfo[ i ].usWorldID;
		g_serverinfo.Release();


		if( pSession->GetState() == SESSION_STATE_NONE ) {

			g_serverinfo.Wait();

			if( _tcslen(g_serverinfo.m_serverinfo[ i ].szPrivateIP) > 0 &&
				m_sm.Connect( i, g_serverinfo.m_serverinfo[ i ].szPrivateIP, AGENT_LISTEN_PORT ) == pSession ) {

			} else {

			}

			g_serverinfo.Release();

			continue;

		} else if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) {

			// 캐릭터 생성 숫자 정보를 보낸다
			if( bSendNewCharNum ) {
				pSession->GetSendBuffer()->Write( (BYTE *)&sendNewCharNum );
			}


			// 일정시간 간격으로 허트비트를 보낸다
			if( GetTickCount() - pAgent->m_dwLastSendTick >= 5000 ) {

				pAgent->m_dwLastSendTick = GetTickCount();

				sReportToAgent_Heartbeat sendMsg;

				sendMsg.packetHeader.usCmd = MASTERMSG_REPORT_MASTER_HEARTBEAT;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			}

			// 일정시간동안 아무런 패킷이 없으면 접속을 끊는다
			if( GetTickCount() - pAgent->m_dwLastRecvTick >= 10000 ) {
				DWORD a = GetTickCount() - pAgent->m_dwLastRecvTick;
				pSession->CloseSocket();
				continue;
			}

			// 서버 기본 정보를 보내지 않았으면 보낸다 
			if( pAgent->m_bSendServerInfo == FALSE ) {

				g_serverinfo.Wait();

				sReportToAgent_ServerInfo sendMsg;

				sendMsg.packetHeader.usCmd = AGENTMSG_REPORT_SERVERINFO;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.siServerID = g_serverinfo.m_serverinfo[ i ].siServerID;

				memcpy( sendMsg.szFilePath, g_serverinfo.m_serverinfo[ i ].szFilePath, 128 );
				sendMsg.szFilePath[ 127 ] = NULL;

				memcpy( sendMsg.szMessage, g_serverinfo.m_serverinfo[ i ].szMessage, 20 );
				sendMsg.szMessage[ 19 ] = NULL;

				printf( "send server info : %s\n", sendMsg.szFilePath );

				pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

				pAgent->m_bSendServerInfo = TRUE;

				g_serverinfo.Release();
			}

			// 패킷 처리

			// 버퍼에서 패킷을 하나 꺼내온다
			sPacketHeader *pPacket = (sPacketHeader *)pSession->GetRecvBuffer()->GetFirstPacket();

			if( pPacket == NULL ) continue;

			pAgent->m_dwLastRecvTick = GetTickCount();

			switch( pPacket->usCmd ) {

			case MASTERMSG_RESPONSE_GMORDER:
				{
					const SI32 ConstTmpBufSize = 64;
					sResponse_GMOrder *pRecvMsg = (sResponse_GMOrder *)pPacket;

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

					// 게임 서버 정보
					sGameWorldInfo* pGameWorldInfo = g_serverinfo.GetGameWorldInfo(pRecvMsg->siWorldId);

					if(pGameWorldInfo != NULL)
					{
						StringCchCopy(szServerName,ConstTmpBufSize,pGameWorldInfo->szWorldName);
					}

					// 요청자
					cClient* pRequester = g_mcp.m_SF.GetPerson(pRecvMsg->uiCharID);
					
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

					TCHAR	szFileName[ConstTmpBufSize];
					StringCchPrintf(szFileName,ConstTmpBufSize,TEXT("GMCommand%d-%d-%d.txt"),
						systime.wYear,systime.wMonth,systime.wDay);

					if(pRecvMsg->isSuccess == TRUE)
					// 시간 , 아이피, 아이디, 서버, 명령어
						g_CLogPrint.FilePrint(szFileName,"[%s] [%s] [%s] [%s]",  szServerName, szID, szIP ,pRecvMsg->order);

					// 세션
					ITZSession* pSession = NULL;

					// 모든 접속자에게 보내기
					for( SI32 i=0; i<ConstMAXMasterClientCount; i++ )
					{
						ITZSession* pSession = g_mcp.m_MasterClientSession[i];
						if( pSession == NULL )										continue;
						if( pSession->GetState() != SESSION_STATE_ESTABLISHED )		continue;
		
						sResponse_GMC_GMOrder clMsg(pRecvMsg->order,
													pRecvMsg->siWorldId,
													szID ,
													szIP,
													szTime,
													pRecvMsg->isSuccess
													);
						pSession->GetSendBuffer()->Write( (BYTE*)&clMsg );
					}
					break;
				}

			case MASTERMSG_REQUEST_FRONTSERVERINITINFO:
				{
					g_serverinfo.Wait();

					printf( "프론트서버로부터 초기화 정보 요청 받음\n" );

					sResponse_FrontServerInitInfo	sendMsg;

					// server id 설정
					sendMsg.siServerID = g_serverinfo.m_frontServerInfo.siServerID;

					// auth server ip / port 설정 
					strcpy( sendMsg.szAuthServerIP, g_serverinfo.m_authServerInfo.szPrivateIP );
					sendMsg.usAuthServerPort = g_serverinfo.m_authServerInfo.usFrontServerListenPort;

					// front listen port 설정 (client, gameserver)
					sendMsg.usClientListenPort		= g_serverinfo.m_frontServerInfo.usClientListenPort;
					sendMsg.usGameServerListenPort	= g_serverinfo.m_frontServerInfo.usGameServerListenPort;

					// world list

					for( int j = 0; j < g_serverinfo.m_usWorldCount; ++j ) {

						sendMsg.worldinfo[ j ].siServerID = g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.siServerID;
						sendMsg.worldinfo[ j ].usWorldID = g_serverinfo.m_gameWorldInfo[ j ].usWorldID;
						strcpy( sendMsg.worldinfo[ j ].szWorldName, g_serverinfo.m_gameWorldInfo[ j ].szWorldName );
						strcpy( sendMsg.worldinfo[ j ].szOpenDate,  g_serverinfo.m_gameWorldInfo[ j ].szOpenDate );
						strcpy( sendMsg.worldinfo[ j ].szShortMessage, g_serverinfo.m_gameWorldInfo[ j ].szShortMessage );

						strcpy( sendMsg.worldinfo[ j ].szGameServerIP,  g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.szPublicIP );
						sendMsg.worldinfo[ j ].usGameServerPort = g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.usClientListenPort;

						strcpy( sendMsg.worldinfo[ j ].szGameDBManagerIP, g_serverinfo.m_gameWorldInfo[ j ].gameDBManagerInfo.szPrivateIP );
						sendMsg.worldinfo[ j ].usGameDBManagerPort = g_serverinfo.m_gameWorldInfo[ j ].gameDBManagerInfo.usGameServerListenPort;
					}

					sendMsg.count = g_serverinfo.m_usWorldCount;

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_FRONTSERVERINITINFO_STRING:
				{
					g_serverinfo.Wait();
					{
						sResponse_FrontServerInitInfo_String	sendMsg;

						NTCHARString<400>		kMapString;
						NMapString<40, 128>		kMapFrontServerInfo;
						kMapFrontServerInfo.CreateMap( 30 );

						printf( "프론트서버로부터 초기화 정보 요청 받음\n" );

						// server id 설정
						sendMsg.siServerID = g_serverinfo.m_frontServerInfo.siServerID;

						// auth server ip / port 설정 
						kMapFrontServerInfo["AuthServerIP"]			= g_serverinfo.m_authServerInfo.szPrivateIP;
						kMapFrontServerInfo["AuthServerPort"]		= UI16ToString(g_serverinfo.m_authServerInfo.usFrontServerListenPort);

						// front listen port 설정 (client, gameserver)
						kMapFrontServerInfo["ClientListenPort"]		= UI16ToString(g_serverinfo.m_frontServerInfo.usClientListenPort);
						kMapFrontServerInfo["GameServerListenPort"] = UI16ToString(g_serverinfo.m_frontServerInfo.usGameServerListenPort);

						kMapFrontServerInfo["ServiceArea"]			= g_serverinfo.m_masterServerInfo.szServiceArea;
						kMapFrontServerInfo["ServiceMode"]			= g_serverinfo.m_masterServerInfo.szServiceMode;

						kMapFrontServerInfo.ToString( kMapString );
						StringCchCopy( sendMsg.szMapString, sizeof(sendMsg.szMapString), kMapString );

						NTCHARString<400>		kMapWorldString;
						NMapString<40, 128>		kMapWorldInfo;
						kMapWorldInfo.CreateMap( 30 );

						// world list
						for( int j = 0; j < g_serverinfo.m_usWorldCount; ++j )
						{
							kMapWorldString = "";
							kMapWorldInfo.ClearAll();

							kMapWorldInfo["ServerID"]			= SI32ToString( g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.siServerID );
							kMapWorldInfo["WorldID"]			= UI16ToString( g_serverinfo.m_gameWorldInfo[ j ].usWorldID );
							kMapWorldInfo["WorldName"]			= g_serverinfo.m_gameWorldInfo[ j ].szWorldName;
							kMapWorldInfo["OpenDate"]			= g_serverinfo.m_gameWorldInfo[ j ].szOpenDate;
							kMapWorldInfo["ShortMessage"]		= g_serverinfo.m_gameWorldInfo[ j ].szShortMessage;

							kMapWorldInfo["GameServerIP"]		= g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.szPublicIP;
							kMapWorldInfo["GameServerPort"]		= SI32ToString(g_serverinfo.m_gameWorldInfo[ j ].gameServerInfo.usClientListenPort);

							kMapWorldInfo["GameDBManagerIP"]	= g_serverinfo.m_gameWorldInfo[ j ].gameDBManagerInfo.szPrivateIP;
							kMapWorldInfo["GameDBManagerPort"]	= SI32ToString(g_serverinfo.m_gameWorldInfo[ j ].gameDBManagerInfo.usGameServerListenPort);

							kMapWorldInfo.ToString( kMapWorldString );
							StringCchCopy( sendMsg.szWorldMapStr[j], sizeof(sendMsg.szWorldMapStr[j]), kMapWorldString );
						}

						sendMsg.count = g_serverinfo.m_usWorldCount;

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}

					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_VERSIONINFO:
				{
					printf( "프론트서버로부터 버전 정보 요청 받음\n" );

					sResponse_VersionInfo	sendMsg;

					sendMsg.count = g_versioninfo.m_uiVersionCount;

					for( UI32 i = 0; i < g_versioninfo.m_uiVersionCount; ++i ) {

						sendMsg.versioninfo[ i ] = g_versioninfo.m_uiVersionList[ i ];
					}

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

				}
				break;

			case MASTERMSG_REQUEST_AUTHSERVERINITINFO:
				{

					g_serverinfo.Wait();

					sResponse_AuthServerInitInfo	sendMsg;

					// server id 설정 
					sendMsg.siServerID = g_serverinfo.m_authServerInfo.siServerID;

					// account db manager ip / port
					strcpy( sendMsg.szAccountDBManagerIP, g_serverinfo.m_accountDBManagerInfo.szPrivateIP );
					sendMsg.usAccountDBManagerPort = g_serverinfo.m_accountDBManagerInfo.usAuthServerListenPort;

					// account db
					memcpy( sendMsg.szAccountDBIP, g_serverinfo.m_accountDBInfo.szPublicIP, 20 );
					sendMsg.szAccountDBIP[ 19 ] = NULL;

					// FrontServer Listen Port
					sendMsg.usFrontServerListenPort	= g_serverinfo.m_authServerInfo.usFrontServerListenPort;

					// GameServer Listen Port
					sendMsg.usGameServerListenPort	= g_serverinfo.m_authServerInfo.usGameServerListenPort;

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_AUTHSERVERINITINFO_STRING:
				{
					g_serverinfo.Wait();
					{
						sResponse_AuthServerInitInfo_String	sendMsg;
						NTCHARString<512>		kMapString;
						NMapString<40, 128>		kMapAuthServerInfo;
						kMapAuthServerInfo.CreateMap( 30 );

						sendMsg.siServerID = g_serverinfo.m_authServerInfo.siServerID;

						kMapAuthServerInfo["AccountDBManagerIP"]		= g_serverinfo.m_accountDBManagerInfo.szPrivateIP;
						kMapAuthServerInfo["AccountDBManagerPort"]		= UI16ToString(g_serverinfo.m_accountDBManagerInfo.usAuthServerListenPort);

						kMapAuthServerInfo["AccountDBIP"]				= g_serverinfo.m_accountDBInfo.szPublicIP;
						kMapAuthServerInfo["FrontServerListenPort"]		= UI16ToString(g_serverinfo.m_authServerInfo.usFrontServerListenPort);
						kMapAuthServerInfo["GameServerListenPort"]		= UI16ToString(g_serverinfo.m_authServerInfo.usGameServerListenPort);

						kMapAuthServerInfo["ServiceArea"]				= g_serverinfo.m_masterServerInfo.szServiceArea;
						kMapAuthServerInfo["ServiceMode"]				= g_serverinfo.m_masterServerInfo.szServiceMode;

						kMapAuthServerInfo.ToString( kMapString );
						StringCchCopy( sendMsg.szMapString, sizeof(sendMsg.szMapString), kMapString );

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO:
				{

					g_serverinfo.Wait();

					sResponse_AccountDBManagerInitInfo	sendMsg;

					// server id 설정 
					sendMsg.siServerID = g_serverinfo.m_accountDBManagerInfo.siServerID;

					// acccount db ip, database, userid, userpw
					strcpy( sendMsg.szAccountDBIP, g_serverinfo.m_accountDBInfo.szPrivateIP );
					strcpy( sendMsg.szDatabaseName, g_serverinfo.m_accountDBInfo.szDatabaseName );
					strcpy( sendMsg.szUserID, g_serverinfo.m_accountDBInfo.szUserID );
					strcpy( sendMsg.szUserPW, g_serverinfo.m_accountDBInfo.szUserPW );

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO_STRING:
				{
					g_serverinfo.Wait();
					{
						sResponse_AccountDBManagerInitInfo_String	sendMsg;

						NTCHARString<512>		kMapString;
						NMapString<40, 128>		KMapAccountDBMgrInfo;
						KMapAccountDBMgrInfo.CreateMap( 30 );

						sendMsg.siServerID = g_serverinfo.m_accountDBManagerInfo.siServerID;

						KMapAccountDBMgrInfo["AccountDBIP"]		= g_serverinfo.m_accountDBInfo.szPrivateIP;
						KMapAccountDBMgrInfo["DatabaseName"]	= g_serverinfo.m_accountDBInfo.szDatabaseName;
						KMapAccountDBMgrInfo["UserID"]			= g_serverinfo.m_accountDBInfo.szUserID;
						KMapAccountDBMgrInfo["UserPW"]			= g_serverinfo.m_accountDBInfo.szUserPW;

						KMapAccountDBMgrInfo["ServiceArea"]		= g_serverinfo.m_masterServerInfo.szServiceArea;
						KMapAccountDBMgrInfo["ServiceMode"]		= g_serverinfo.m_masterServerInfo.szServiceMode;

						KMapAccountDBMgrInfo.ToString( kMapString );
						StringCchCopy( sendMsg.szMapString, sizeof(sendMsg.szMapString), kMapString );

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					}
					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_GAMESERVERINITINFO:
				{
					if( usWorldID == 0 ) break;

					g_serverinfo.Wait();

					printf( "게임서버로부터 초기화 정보 요청 받음\n" );

					printf( "world id = %d\n", usWorldID );
					printf( "gamedbmanagerip = %s", g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.szPrivateIP );
					printf( "gameserverip = %s", g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.szPrivateIP );

					sResponse_GameServerInitInfo	sendMsg;

					// server id 설정
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ i ].siServerID;

					strcpy( sendMsg.szFrontServerIP, g_serverinfo.m_frontServerInfo.szPrivateIP );
					sendMsg.usFrontServerPort = g_serverinfo.m_frontServerInfo.usGameServerListenPort;

					strcpy( sendMsg.szAuthServerIP, g_serverinfo.m_authServerInfo.szPrivateIP );
					sendMsg.usAuthServerPort = g_serverinfo.m_authServerInfo.usGameServerListenPort;

					strcpy( sendMsg.szGameDBManagerIP, 
						g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.szPrivateIP );


					sendMsg.usGameDBManagerPort = g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.usGameServerListenPort;

					sendMsg.usClientListenPort	= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.usClientListenPort;
					sendMsg.usServerUnique		= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.usServerUnique;

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_GAMESERVERINITINFO_STRING:
				{
					if( usWorldID == 0 ) break;

					g_serverinfo.Wait();
					{
						sResponse_GameServerInitInfo_String	sendMsg;

						NTCHARString<1536>		kMapString;
						NMapString<40, 128>		KMapGameServerInfo;
						KMapGameServerInfo.CreateMap( 30 );

						// server id 설정
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ i ].siServerID;

						KMapGameServerInfo["FrontServerIP"]		= g_serverinfo.m_frontServerInfo.szPrivateIP;
						KMapGameServerInfo["FrontServerPort"]	= UI16ToString(g_serverinfo.m_frontServerInfo.usGameServerListenPort);

						KMapGameServerInfo["AuthServerIP"]		= g_serverinfo.m_authServerInfo.szPrivateIP;
						KMapGameServerInfo["AuthServerPort"]	= UI16ToString(g_serverinfo.m_authServerInfo.usGameServerListenPort);

						KMapGameServerInfo["GameDBManagerIP"]	= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.szPrivateIP;
						KMapGameServerInfo["GameDBManagerPort"]	= UI16ToString(g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.usGameServerListenPort);

						KMapGameServerInfo["ClientListenPort"]	= UI16ToString(g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.usClientListenPort);
						KMapGameServerInfo["ServerUnique"]		= UI16ToString(g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.usServerUnique);

						KMapGameServerInfo["ServiceArea"]		= g_serverinfo.m_masterServerInfo.szServiceArea;
						KMapGameServerInfo["ServiceMode"]		= g_serverinfo.m_masterServerInfo.szServiceMode;

						KMapGameServerInfo.ToString( kMapString );
						StringCchCopy( sendMsg.szMapString, sizeof(sendMsg.szMapString), kMapString );

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO:
				{
					if( usWorldID == 0 ) break;

					g_serverinfo.Wait();

					printf( "게임DB Manager로부터 초기화 정보 요청 받음\n" );

					sResponse_GameDBManagerInitInfo	sendMsg;

					// server id 설정
					sendMsg.siServerID = g_serverinfo.m_serverinfo[ i ].siServerID;

					sendMsg.usGameServerListenPort = g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.usGameServerListenPort;

					strcpy( sendMsg.szGameDBIP, g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szPrivateIP );
					strcpy( sendMsg.szDatabaseName, g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szDatabaseName );
					strcpy( sendMsg.szUserID, g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szUserID );
					strcpy( sendMsg.szUserPW, g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szUserPW );

					strcpy( sendMsg.szLogDBIP,			g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szPrivateIP );
					strcpy( sendMsg.szLogDatabaseName,	g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szDatabaseName );
					strcpy( sendMsg.szLogUserID,		g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szUserID );
					strcpy( sendMsg.szLogUserPW,		g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szUserPW );
					strcpy( sendMsg.szLogProcedure,		g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szLogProcedure );

					pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

					g_serverinfo.Release();

				}
				break;

			case MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO_STRING:
				{
					if( usWorldID == 0 )		break;

					g_serverinfo.Wait();
					{
						sResponse_GameDBManagerInitInfo_String	sendMsg;

						NTCHARString<1536>		kMapString;
						NMapString<40, 128>		KGameDBMgrInfo;
						KGameDBMgrInfo.CreateMap( 30 );

						// server id 설정
						sendMsg.siServerID = g_serverinfo.m_serverinfo[ i ].siServerID;

						KGameDBMgrInfo["GameServerListenPort"] = UI16ToString(g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBManagerInfo.usGameServerListenPort);

						KGameDBMgrInfo["GameDBIP"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szPrivateIP;
						KGameDBMgrInfo["DatabaseName"]		= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szDatabaseName;
						KGameDBMgrInfo["UserID"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szUserID;
						KGameDBMgrInfo["UserPW"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameDBInfo.szUserPW;

						KGameDBMgrInfo["LogDBIP"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szPrivateIP;
						KGameDBMgrInfo["LogDatabaseName"]	= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szDatabaseName;
						KGameDBMgrInfo["LogUserID"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szUserID;
						KGameDBMgrInfo["LogUserPW"]			= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szUserPW;
						KGameDBMgrInfo["LogProcedure"]		= g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].logDBInfo.szLogProcedure;

						KGameDBMgrInfo["BillingServerIP"]	= g_serverinfo.m_billingServerInfo.szPublicIP;
						KGameDBMgrInfo["BillingServerPort"]	= UI16ToString(g_serverinfo.m_billingServerInfo.usBillingServerPort);
						KGameDBMgrInfo["BillingTimeOut"]	= UI16ToString(g_serverinfo.m_billingServerInfo.usBillingTimeOut);
						KGameDBMgrInfo["BillingMode"]		= g_serverinfo.m_billingServerInfo.szBillingMode;

						KGameDBMgrInfo["ServiceArea"]		= g_serverinfo.m_masterServerInfo.szServiceArea;
						KGameDBMgrInfo["ServiceMode"]		= g_serverinfo.m_masterServerInfo.szServiceMode;

						KGameDBMgrInfo.ToString( kMapString );
						StringCchCopy( sendMsg.szMapString, sizeof(sendMsg.szMapString), kMapString );

						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
					}
					g_serverinfo.Release();

				}
				break;

			case MASTERMSG_REPORT_FRONTSERVER_HEARTBEAT:
				{
					g_serverinfo.m_dwFrontServerTick = GetTickCount();

					g_serverinfo.Wait();

					sReport_FrontServerHeartbeat *pRecvMsg =
						(sReport_FrontServerHeartbeat *)pPacket;

					g_serverinfo.m_frontRefresh = pRecvMsg->refreshInfo;
					g_serverinfo.m_frontRefresh.dwProcessId = pRecvMsg->dwProcessId;
					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REPORT_AUTHSERVER_HEARTBEAT:
				{
					g_serverinfo.m_dwAuthServerTick = GetTickCount();

					g_serverinfo.Wait();

					sReport_AuthServerHeartbeat *pRecvMsg =
						(sReport_AuthServerHeartbeat *)pPacket;

					g_serverinfo.m_authRefresh = pRecvMsg->refreshInfo;
					g_serverinfo.Release();										
				}
				break;

			case MASTERMSG_REPORT_ACCOUNTDBMANAGER_HEARTBEAT:
				{
					g_serverinfo.m_dwAccountDBManagerTick = GetTickCount();

					g_serverinfo.Wait();

					sReport_AccountDBManagerHeartbeat *pRecvMsg =
						(sReport_AccountDBManagerHeartbeat *)pPacket;

					g_serverinfo.m_accountDBManagerRefresh = pRecvMsg->refreshInfo;
					g_serverinfo.Release();
				}
				break;

			case MASTERMSG_REPORT_GAMESERVER_HEARTBEAT:
				{
					if( usWorldID == 0 ) break;

					g_serverinfo.Wait();

					g_serverinfo.m_dwGameServerTick[ usWorldID - 1 ] = GetTickCount();

					sReport_GameServerHeartbeat *pRecvMsg = (sReport_GameServerHeartbeat *)pPacket;


					g_serverinfo.m_gameRefresh[ usWorldID - 1 ] = pRecvMsg->refreshInfo;

					g_UserCountInfo.m_uiGamepotJP	= 0;
					g_UserCountInfo.m_uiHangemeJP	= 0;
					g_UserCountInfo.m_uiNexonJP	    = 0;

					for( int j = 0; j < g_serverinfo.m_usWorldCount; ++j )
					{						
						g_UserCountInfo.m_uiGamepotJP += g_serverinfo.m_gameRefresh[ j ].usCurrentGamePotJP;
						g_UserCountInfo.m_uiHangemeJP += g_serverinfo.m_gameRefresh[ j ].usCurrentHanGameJP;
						g_UserCountInfo.m_uiNexonJP   += g_serverinfo.m_gameRefresh[ j ].usCurrentNEXONJP;
					}

					g_serverinfo.Release();

				}
				break;

			case MASTERMSG_REPORT_GAMEDBMANAGER_HEARTBEAT:
				{
					if( usWorldID == 0 ) break;

					g_serverinfo.Wait();

					g_serverinfo.m_dwGameDBManagerTick[ usWorldID - 1 ] = GetTickCount();

					sReport_GameDBManagerHeartbeat *pRecvMsg =
						(sReport_GameDBManagerHeartbeat *)pPacket;

					g_serverinfo.m_gameDBManagerRefresh[ usWorldID - 1 ] = pRecvMsg->refreshInfo;
					g_serverinfo.Release();
				}
				break;

			case GOONZUGM_RESPONSE_ACCOUNTCHARINFO:
				{
					sResponse_GMC_AccountCharInfo *pRecvMsg = (sResponse_GMC_AccountCharInfo *)pPacket;

					ITZPerson* pPerson = g_mcp.m_SF.GetPerson(pRecvMsg->m_siCharID);

					g_serverinfo.Wait();
					strncpy(pRecvMsg->m_szServerName, g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].szWorldName, sizeof(pRecvMsg->m_szServerName));
					g_serverinfo.Release();

					if(pPerson != NULL)
					{
						pPerson->GetSendBuffer()->Write((BYTE*)pRecvMsg);
					}
				}
				break;

			case GOONZUGM_RESPONSE_USERCHARINFO:
				{
					sResponse_GMC_UserCharInfo *pRecvMsg = (sResponse_GMC_UserCharInfo *)pPacket;

					ITZPerson* pPerson = g_mcp.m_SF.GetPerson(pRecvMsg->m_siCharID);
					if(pPerson != NULL)
					{
						pPerson->GetSendBuffer()->Write((BYTE*)pRecvMsg);
					}
				}
				break;

			case GAMESERVER_SEND_TOOLSLOG_MASTER:
				{
					sRequest_Send_ToolsLog_MasterServer *pRecvMsg =
						(sRequest_Send_ToolsLog_MasterServer *)pPacket;

#ifdef _CHINA
					sDBRequest_ToolsLog clSendMsg( pRecvMsg->szServerName, pRecvMsg->szCommand, 
						                           pRecvMsg->szAccountID, pRecvMsg->szCharName, pRecvMsg->szLog );
					g_ToolsLog.SendMsg( (sPacketHeader *)&clSendMsg );
#endif
				}
				break;

			case MASTERMSG_REQUEST_GAMEDBMGR_ERRORQUERY_STRING:
				{
					sRequest_GameDBMgr_ErroyQuery_String *pRecvMsg = (sRequest_GameDBMgr_ErroyQuery_String *)pPacket;

					NMapString<32, 128>	kMap;
					kMap.CreateMap( 20 );
					kMap.Parse( pRecvMsg->m_szMapString );

					kMap["WorldID"] = UI16ToString( g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].usWorldID );
					kMap["WorldName"] = g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].szWorldName;

					NTCHARString1024 kMapStr;
					kMap.ToString( kMapStr );

					for( SI32 i=0; i<ConstMAXMasterClientCount; i++ )
					{
						ITZSession* pSession = g_mcp.m_MasterClientSession[i];
						if( pSession == NULL )										continue;
						if( pSession->GetState() != SESSION_STATE_ESTABLISHED )		continue;

						sResponse_GameDBMgr_ErrorQuery_String clMsg( kMapStr );
						pSession->GetSendBuffer()->Write( (BYTE*)&clMsg );
					}
				}
				break;


			} // switch

			pSession->GetRecvBuffer()->RemoveFirstPacket();

		} // if

	} // for

}

void ServerProcess::SendPacketAll( char *pPacket )
{
	ITZSession	*pSession;

	for( int i = 0; i < MAX_SERVER_COUNT; ++i ) {

		if( i >= MAX_SERVER_COUNT - 1 ) break;
		/*
		g_serverinfo.Wait();
		if( g_serverinfo.m_serverinfo[ i ].usServerType != SERVERTYPE_GAMESERVER ) {
		g_serverinfo.Release();
		continue;
		}
		g_serverinfo.Release();
		*/
		pSession = m_serverSession[ i ];

		if( pSession == NULL ) continue;

		if( pSession->GetState() != SESSION_STATE_ESTABLISHED ) continue;

		pSession->GetSendBuffer()->Write( (BYTE *)pPacket );

	}
}

ITZSession*	ServerProcess::GetGameServerSession(UI32 uiWorldId)
{
	const SI32 ConstServerTypeCount = 3;
	const SI32 ConstGameServerFlag = 4;

	// 리턴 할 세션 포인터
	ITZSession* pReturnSession = NULL;

	// 세션 배열의 해당 인덱스 
	UI32 uiIndex = 0;
	// WorldID값은 1부터 시작하기때문에
	uiIndex = (uiWorldId-1) * ConstServerTypeCount + ConstGameServerFlag;
	if(uiIndex > MAX_SERVER_COUNT)
	{
		return NULL;
	}
	
	pReturnSession = m_serverSession[uiIndex];
	
	return pReturnSession;

}
