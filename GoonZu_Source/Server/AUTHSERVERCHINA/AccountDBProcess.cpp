#include "GleeService.h"
#include "AccountDBProcess.h"
#include "AuthFrontProtocol.h"
#include "AuthGameProtocol.h"
#include "AuthUserList.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include "GASH.h"
#include "UserManager.h"
#include <comutil.h>
#include "md5.h"

AccountDBProcess::AccountDBProcess()
{
	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		m_pAccountDBSession[ i ] = NULL;
	}
	

	m_accountDBManagerIP[ 0 ] = NULL;

	m_siWorkSlot = 0;

	InitializeCriticalSection( &m_cs );
}

AccountDBProcess::~AccountDBProcess()
{
	DeleteCriticalSection( &m_cs );
}

void AccountDBProcess::Init( char *szAccountDBManagerIP, UI16 usPort, char *szAccountDBIP )
{
	m_sf.CreateFactory( &m_sm, DBCONNECTION_NUMBER, 2 * 1000 * 1000, 2 * 1000 * 1000 );

	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		
		m_pAccountDBSession[ i ] = m_sm.GetSession( i );
	}

	m_iocp.CreateIOCP( &m_sm, 1 );

	strcpy( m_accountDBManagerIP, szAccountDBManagerIP );
	m_accountDBManagerPort = usPort;

	memcpy( m_accountDBIP, szAccountDBIP, 20 );
	m_accountDBIP[ 19 ] = NULL;
}

void AccountDBProcess::Run()
{
	int i, j, count;

	sPacketHeader *pPacket;
	ITZSession *pSession;

	m_sm.Update();

	for( i = 0; i < DBCONNECTION_NUMBER; ++i ) {

		// 세션이 연결되지 않은 상태이면,
		if( m_pAccountDBSession[ i ]->GetState() == SESSION_STATE_NONE )
		{

			if( m_sm.Connect( i, m_accountDBManagerIP, m_accountDBManagerPort + i ) == m_pAccountDBSession[ i ] ) {
				printf( "Connect to AccontDBManager [SUCCESS]\n" );
			} else {
				printf( "Trying to connect to AccontDBManager..\n" );
			}
			
		} 
		// 세션이 성공적으로 연결된 상태이면,
		else if( m_pAccountDBSession[ i ]->GetState() == SESSION_STATE_ESTABLISHED )
		{

			count = m_pAccountDBSession[ i ]->GetRecvBuffer()->GetPacketCount();

			for( j = 0; j < count; ++j ) {


				// 버퍼에서 패킷을 하나 꺼내온다
				pPacket = (sPacketHeader *)m_pAccountDBSession[ i ]->GetRecvBuffer()->GetFirstPacket();
					
				if( pPacket != NULL ) {										
					// 패킷이 있으면,

					switch( pPacket->usCmd ) {
					case DBMSG_RESPONSE_IDNUMGET:
						{
							sDBResponse_IDNumGet *pRecvMsg = (sDBResponse_IDNumGet *)pPacket;

							printf( "DBMSG_RESPONSE_IDNUMGET\n" );

							// 아이디가 없으면 자동 등록한다
							if( -1 == pRecvMsg->idnum )
							{
								char md5buf[ 256 ];
								char temp[ 256 ];

								sprintf( temp, "%sGleeDEV", pRecvMsg->userpw );

								md5_string( (unsigned char*)temp, md5buf, strlen( temp ) );

								_bstr_t dbip = g_accountDB.m_accountDBIP;
								_bstr_t id = pRecvMsg->userid;
								_bstr_t pw = md5buf;

								_bstr_t resultstr = "Start";

								const size_t newsize = 100;
								char nstring[newsize];
								strncpy(nstring, (char *)resultstr, 100 );
								printf( "%s\n", nstring );

								BSTR  bstr = resultstr.copy();  

								SI32 result = -1;
								g_GameActivServiceT.GleeActivHLJZGame( dbip, 0, id, pw, &result, &bstr ) ;

								resultstr.Assign( bstr );
								strncpy(nstring, (char *)resultstr, 100 );
								printf( "result = %d %s\n" , result, nstring);

								SysFreeString ( bstr ); 

								switch( result )
								{
								case 0:
									{
										// AccountDBManager로 인증 요청
										sDBRequest_IDNumGet	sendMsg;

										sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
										sendMsg.packetHeader.usSize = sizeof( sendMsg );

										sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
										sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

										memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
										sendMsg.userid[ 19 ] = NULL;

										memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
										sendMsg.userpw[ 19 ] = NULL;

										memcpy( sendMsg.userIP, pRecvMsg->userIP, 20 );
										sendMsg.userIP[ 19 ] = NULL;

										memcpy( sendMsg.macaddress, pRecvMsg->macaddress, 20 );
										sendMsg.macaddress[ 19 ] = NULL;

										sendMsg.jumin[ 0 ] = NULL;

										g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

										printf( "ReLogin Try\n" );
									}
									break;
								default:
									break;
								}
								

								// 응답 보내기
								sResponse_FrontAuth	sendMsg;

								sendMsg.packetHeader.usSize = sizeof( sendMsg );
								sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

								sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
								sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

								sendMsg.siAuthKey = pRecvMsg->idnum;

								sendMsg.siGameMasterLevel = pRecvMsg->siGameMasterLevel;
								sendMsg.siAccountStatus = pRecvMsg->siAccountStatus;

								sendMsg.siBlockMode = pRecvMsg->siBlockMode;
								sendMsg.siBlockYear = pRecvMsg->siBlockYear;
								sendMsg.siBlockMonth = pRecvMsg->siBlockMonth;
								sendMsg.siBlockDay = pRecvMsg->siBlockDay;
								sendMsg.siBlockHour = pRecvMsg->siBlockHour;
								sendMsg.siBlockMin = pRecvMsg->siBlockMin;

								memcpy( sendMsg.szBlockReason, pRecvMsg->szBlockReason, 30 );
								sendMsg.szBlockReason[ 29 ] = NULL;

								sendMsg.bAlreadyMakeCharSwitch = pRecvMsg->bAlreadyMakeCharSwitch;

								memcpy( sendMsg.szServerName, pRecvMsg->szServerName, 20 );
								sendMsg.szServerName[ 19 ] = NULL;



								if( pRecvMsg->idnum > 0 ) {
									sendMsg.usResult = 0;
								} else {
									sendMsg.usResult = -pRecvMsg->idnum;
								}							

								// 감마니아 계정이라면,
								if( pRecvMsg->siAccountStatus == 2 )
								{
									g_gash.AddRequestInfo( REQUESTCODE_AUTH, 
										pRecvMsg->usPacketUnique, 
										pRecvMsg->usFrontCharID,
										pRecvMsg->userid,
										pRecvMsg->userpw,
										pRecvMsg->userIP,
										(BYTE*)&sendMsg,
										sizeof( sendMsg ) 
										);

								} else {

									g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

									pSession = g_afp.GetSessionManager()->GetSession(0);

									if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
								}
							}
							else
							{
								// 응답 보내기
								sResponse_FrontAuth	sendMsg;

								sendMsg.packetHeader.usSize = sizeof( sendMsg );
								sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

								sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
								sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

								sendMsg.siAuthKey = pRecvMsg->idnum;

								sendMsg.siGameMasterLevel = pRecvMsg->siGameMasterLevel;
								sendMsg.siAccountStatus = pRecvMsg->siAccountStatus;

								sendMsg.siBlockMode = pRecvMsg->siBlockMode;
								sendMsg.siBlockYear = pRecvMsg->siBlockYear;
								sendMsg.siBlockMonth = pRecvMsg->siBlockMonth;
								sendMsg.siBlockDay = pRecvMsg->siBlockDay;
								sendMsg.siBlockHour = pRecvMsg->siBlockHour;
								sendMsg.siBlockMin = pRecvMsg->siBlockMin;

								memcpy( sendMsg.szBlockReason, pRecvMsg->szBlockReason, 30 );
								sendMsg.szBlockReason[ 29 ] = NULL;

								sendMsg.bAlreadyMakeCharSwitch = pRecvMsg->bAlreadyMakeCharSwitch;

								memcpy( sendMsg.szServerName, pRecvMsg->szServerName, 20 );
								sendMsg.szServerName[ 19 ] = NULL;



								if( pRecvMsg->idnum > 0 ) {
									sendMsg.usResult = 0;
								} else {
									sendMsg.usResult = -pRecvMsg->idnum;
								}							

								// 감마니아 계정이라면,
								if( pRecvMsg->siAccountStatus == 2 )
								{
									g_gash.AddRequestInfo( REQUESTCODE_AUTH, 
										pRecvMsg->usPacketUnique, 
										pRecvMsg->usFrontCharID,
										pRecvMsg->userid,
										pRecvMsg->userpw,
										pRecvMsg->userIP,
										(BYTE*)&sendMsg,
										sizeof( sendMsg ) 
										);

								} else {

									g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

									pSession = g_afp.GetSessionManager()->GetSession(0);

									if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
								}
							}
						}
						break;

					case DBMSG_RESPONSE_GAMEAUTH:
						{
							printf( "DBMSG_RESPONSE_GAMEAUTH\n" );

							sDBResponse_GameAuth *pRecvMsg = (sDBResponse_GameAuth *)pPacket;

							// 응답 보내기

							sResponse_GameAuth	sendMsg;

							ZeroMemory( &sendMsg, sizeof( sResponse_GameAuth ) );

							sendMsg.packetHeader.usSize = sizeof( sendMsg );
							sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_GAMEAUTH;

							sendMsg.usCharID = pRecvMsg->usCharID;
							
							sendMsg.siGameMasterLevel = pRecvMsg->siGameMasterLevel;

							sendMsg.siBlockMode = pRecvMsg->siBlockMode;
							sendMsg.siBlockYear = pRecvMsg->siBlockYear;
							sendMsg.siBlockMonth = pRecvMsg->siBlockMonth;
							sendMsg.siBlockDay = pRecvMsg->siBlockDay;
							sendMsg.siBlockHour = pRecvMsg->siBlockHour;
							sendMsg.siBlockMin = pRecvMsg->siBlockMin;
							sendMsg.siAccountStatus = pRecvMsg->siAccountStatus;
							
							memcpy( sendMsg.szBlockReason, pRecvMsg->szBlockReason, 30 );
							sendMsg.szBlockReason[ 29 ] = NULL;

							if( pRecvMsg->idnum > 0 ) {
								sendMsg.uiIDNum = pRecvMsg->idnum;								

								//sendMsg.siAccountStatus = g_usermanager.GameAuth( pRecvMsg->idnum );

							} else {
								sendMsg.uiIDNum = 0;
							}

							printf( "CharID:%d, idnum:%d, GameMasterLevel:%d, Sex:%d\n", sendMsg.usCharID, sendMsg.uiIDNum, sendMsg.siGameMasterLevel, sendMsg.siSex );

							pSession = g_agp.GetSessionManager()->GetSession( pRecvMsg->usGameServerID );

							if( pSession->GetState() == SESSION_STATE_ESTABLISHED ) {

								if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
							}
/*
							pSession = g_afp.GetSessionManager()->GetSession(0);

							if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
*/
							
						}
						break;
						
					case DBMSG_RESPONSE_JOINGAME:
						{
							printf( "DBMSG_RESPONSE_JOINGAME\n" );

							sDBResponse_JoinGame *pRecvMsg = (sDBResponse_JoinGame *)pPacket;

							sResponse_FrontJoinGame sendMsg;

							sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTJOINGAME;
							sendMsg.packetHeader.usSize = sizeof( sendMsg );

							sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
							sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

							sendMsg.siResult = pRecvMsg->siResult;

							pSession = g_afp.GetSessionManager()->GetSession(0);

							if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
						}
						break;
	
					}

					// 처리 후 패킷을 버퍼에서 지운다
					m_pAccountDBSession[ i ]->GetRecvBuffer()->RemoveFirstPacket();

				} // if  packet

			} // for j
		
		} // if established

	} // for i

}

void AccountDBProcess::SendMsg( sPacketHeader *pPacket )
{
	EnterCriticalSection( &m_cs );

	m_siWorkSlot++;

	if( m_siWorkSlot == DBCONNECTION_NUMBER ) m_siWorkSlot = 0;

	while( m_pAccountDBSession[ m_siWorkSlot ]->GetState() != SESSION_STATE_ESTABLISHED ) {
		m_siWorkSlot++;
		if( m_siWorkSlot == DBCONNECTION_NUMBER ) m_siWorkSlot = 0;
	}

	if( m_pAccountDBSession[ m_siWorkSlot ]->GetSendBuffer() == NULL ) return;

	m_pAccountDBSession[ m_siWorkSlot ]->GetSendBuffer()->Write( (BYTE *)pPacket);	
	
	LeaveCriticalSection( &m_cs );
}