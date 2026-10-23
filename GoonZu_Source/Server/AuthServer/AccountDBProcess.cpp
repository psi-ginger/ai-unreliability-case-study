#include "AccountDBProcess.h"
#include "AuthFrontProtocol.h"
#include "AuthGameProtocol.h"
#include "AuthUserList.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"
#include "GASH.h"
#include "UserManager.h"

#include "cltIOVATION.h"

#ifdef _CHINA
	#include "GleeService.h"
	#include <comutil.h>
	#include "../../NLib/md5.h"
	GameActivService::CGameActivService	g_GameActivServiceT;
#endif

#ifdef _IOVATION
	extern cltIOVATION		g_clioVation;
#endif

//KHY - 0218 - 중국 NHN 피로도 모듈 적용 - 인증모듈이 적용되어야만 한다.
#ifdef USE_HANANTITOXIC
#include "./HanAntiToxic/HanAntiToxic.h"

	extern TCHAR g_HanSID[MAX_SID_BUF];
	extern BOOL g_Tired;
#endif

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
#ifdef _CHINA
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
								// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28
								//if( pRecvMsg->siAccountStatus == 2 )
								//{
								//	g_gash.AddRequestInfo( REQUESTCODE_AUTH, 
								//		pRecvMsg->usPacketUnique, 
								//		pRecvMsg->usFrontCharID,
								//		pRecvMsg->userid,
								//		pRecvMsg->userpw,
								//		pRecvMsg->userIP,
								//		(BYTE*)&sendMsg,
								//		sizeof( sendMsg ) 
								//		);

								//} else {

									g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

									pSession = g_afp.GetSessionManager()->GetSession(0);

									if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
								//}
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
								// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28
								//if( pRecvMsg->siAccountStatus == 2 )
								//{
								//	g_gash.AddRequestInfo( REQUESTCODE_AUTH, 
								//		pRecvMsg->usPacketUnique, 
								//		pRecvMsg->usFrontCharID,
								//		pRecvMsg->userid,
								//		pRecvMsg->userpw,
								//		pRecvMsg->userIP,
								//		(BYTE*)&sendMsg,
								//		sizeof( sendMsg ) 
								//		);

								//} else {

									g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

									pSession = g_afp.GetSessionManager()->GetSession(0);

									if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
								//}
							}
#else
							printf( "DBMSG_RESPONSE_IDNUMGET\n" );

							sDBResponse_IDNumGet *pRecvMsg = (sDBResponse_IDNumGet *)pPacket;

							// 응답 보내기

							sResponse_FrontAuth	sendMsg;

							sendMsg.packetHeader.usSize = sizeof( sendMsg );
							sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

							sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
							sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

							// 넥슨 채널링을 위한 ID체크 -넥슨은 DB에서 ID가 변환되어 들어와야 한다.
							memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
							sendMsg.szBlockReason[ 19 ] = NULL;

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

							sendMsg.m_uiRealAge = pRecvMsg->m_uiRealAge;


							if( pRecvMsg->idnum > 0 ) {
								sendMsg.usResult = 0;
							} else {
								sendMsg.usResult = -pRecvMsg->idnum;
							}							

							// 감마니아 계정이라면,
							// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28
							//if( pRecvMsg->siAccountStatus == 2 )
							//{
							//	g_gash.AddRequestInfo( REQUESTCODE_AUTH, 
							//						   pRecvMsg->usPacketUnique, 
							//						   pRecvMsg->usFrontCharID,
							//						   pRecvMsg->userid,
							//						   pRecvMsg->userpw,
							//						   pRecvMsg->userIP,
							//						   (BYTE*)&sendMsg,
							//						   sizeof( sendMsg ) 
							//						   );

							//} else {

								g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

								pSession = g_afp.GetSessionManager()->GetSession(0);

								if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
							//}
#endif
						}
						break;
					// PCK : IOVATION 인증 (2009.02.03)
					case DBMSG_RESPONSE_ENGLISH_IDNUMGET:
						{
							printf( "DBMSG_RESPONSE_ENGLISH_IDNUMGET\n" );

							sDBResponse_English_IDNumGet *pRecvMsg = (sDBResponse_English_IDNumGet *)pPacket;

							// 응답 보내기

							sResponse_FrontAuth	sendMsg;

							sendMsg.packetHeader.usSize = sizeof( sendMsg );
							sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

							sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
							sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

							memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
							sendMsg.szBlockReason[ 19 ] = NULL;

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

							if( pRecvMsg->idnum > 0 ) 
							{
								sendMsg.usResult = 0;
							} 
							else 
							{
								sendMsg.usResult = -pRecvMsg->idnum;
							}

#ifdef _IOVATION
							// DB인증이 성공한 경우에만 IOVATION인증을 처리한다.
							if( sendMsg.usResult == 0 )
							{
								if( g_clioVation.CheckTransantion( pRecvMsg->userid, pRecvMsg->Blackbox, pRecvMsg->userIP ) == false)
								{
									// 인증 실패 메시지를 보낸다.
									sResponse_FrontAuth	sendFailMsg;			ZeroMemory(&sendFailMsg, sizeof(sendFailMsg));

									sendFailMsg.packetHeader.usSize = sizeof( sendFailMsg );
									sendFailMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

									sendFailMsg.usPacketUnique = pRecvMsg->usPacketUnique;
									sendFailMsg.usFrontCharID = pRecvMsg->usFrontCharID;

									memcpy( sendFailMsg.userid, pRecvMsg->userid, 20 );
									sendFailMsg.szBlockReason[ 19 ] = NULL;

									sendFailMsg.siAuthKey = 0;

									memcpy( sendFailMsg.szServerName, pRecvMsg->szServerName, 20 );
									sendFailMsg.szServerName[ 19 ] = NULL;

									sendFailMsg.usResult = 18;		// 실패 - IOVATION 인증 실패

									pSession = g_afp.GetSessionManager()->GetSession(0);
									if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendFailMsg );	

									break;
								}
							}
#endif

							g_usermanager.FrontAuth( pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->idnum, pRecvMsg->siAccountStatus );

							pSession = g_afp.GetSessionManager()->GetSession(0);

							if( pSession ) pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
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

							sendMsg.siBlockMode		= pRecvMsg->siBlockMode;
							sendMsg.siBlockYear		= pRecvMsg->siBlockYear;
							sendMsg.siBlockMonth	= pRecvMsg->siBlockMonth;
							sendMsg.siBlockDay		= pRecvMsg->siBlockDay;
							sendMsg.siBlockHour		= pRecvMsg->siBlockHour;
							sendMsg.siBlockMin		= pRecvMsg->siBlockMin;
							sendMsg.siAccountStatus = pRecvMsg->siAccountStatus;
							sendMsg.siDormancyState = pRecvMsg->siDormancyState;
							
							memcpy( sendMsg.szBlockReason, pRecvMsg->szBlockReason, 30 );
							sendMsg.szBlockReason[ 29 ] = NULL;

							if( pRecvMsg->idnum > 0 ) {
								sendMsg.uiIDNum = pRecvMsg->idnum;								

								// 동접수 증가
								switch(pRecvMsg->siAccountStatus)
								{
								case 4:		g_usermanager.m_HanGameJPCount++;	break;
								case 8:		g_usermanager.m_GamepotJPCount++;	break;
								case 2:		g_usermanager.m_GamaniaJPCount++;	break;
								case 10:	g_usermanager.m_NEXONJPCount++;	break;
								case 0:		g_usermanager.m_NDEJPCount++;	break;
								}
								//sendMsg.siAccountStatus = g_usermanager.GameAuth( pRecvMsg->idnum );

							} else {
								sendMsg.uiIDNum = 0;
							}

//KHY - 0218 - 중국 NHN 피로도 모듈 적용
#ifdef USE_HANANTITOXIC
							SI32 HanAntiToxicresult = -1;
							SI32 HanAntiToxicPlayTime = -1;

							printf( "DBMSG_RESPONSE_GAMEAUTH - g_Tired =%d  \n", g_Tired );
							
							if(g_Tired == TRUE)
							{
								HanAntiToxicresult = HanGetAntiToxic("N", pRecvMsg->userid, g_HanSID, &HanAntiToxicPlayTime);

								if(HanAntiToxicresult < 0)
								{
									SI32 HanAntiToxicReTryCount = 0;

									printf( "DBMSG_RESPONSE_GAMEAUTH - HanAntiToxicresult =%d, HanAntiToxicPlayTime = %d \n", HanAntiToxicresult ,HanAntiToxicPlayTime );
									
									while(HanAntiToxicresult < 0) //꼭 성공 해야 한다.
									{
										HanAntiToxicReTryCount++;

										if(HanAntiToxicReTryCount < 10) // 10번만 돌자.
										{
											SI32	siHanAntiToxicInitRet = 0;

											siHanAntiToxicInitRet = HanAntiToxicInit("C_GZ");
											if(siHanAntiToxicInitRet != 0)		
											{	
												printf( "DBMSG_RESPONSE_GAMEAUTH - siHanAntiToxicInitRet =%d \n", siHanAntiToxicInitRet );
		
											}
											else
											{
												//재시도.
												Sleep(1);
												HanAntiToxicresult = HanGetAntiToxic("N", pRecvMsg->userid, g_HanSID, &HanAntiToxicPlayTime);
											}
										}
										else
										{
											HanAntiToxicresult = 100; //빠져나오자.
										}
									}								
								}
							}
							
							sendMsg.siPlayingTime = HanAntiToxicPlayTime;
#else
							sendMsg.siPlayingTime = pRecvMsg->siPlayingTime;
#endif
							sendMsg.uiIDNo        = pRecvMsg->uiIDNo;		// 한게임용 IDNO - by LEEKH 2007.11.11

							printf( "CharID:%d, idnum:%d, GameMasterLevel:%d, Sex:%d, IDNo:%d PlayTime: %d, DormancyState: %d\n", 
								sendMsg.usCharID, 
								sendMsg.uiIDNum,
								sendMsg.siGameMasterLevel, 
								sendMsg.siSex, 
								sendMsg.uiIDNo,
								sendMsg.siPlayingTime, 
								sendMsg.siDormancyState );

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
	
					case DBMSG_RESPONSE_GMTOOLLOGIN:
						{
							printf( "DBMSG_RESPONSE_GMTOOLLOGIN\n" );

							sDBResponse_GMTOOLLogin *pRecvMsg = (sDBResponse_GMTOOLLogin *)pPacket;

							sResponse_GMToolAuth sendMsg;

							sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_GMTOOLAUTH;
							sendMsg.packetHeader.usSize = sizeof( sendMsg );

							memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
							sendMsg.userid[ 19 ] = NULL;

							sendMsg.siMasterLevel = pRecvMsg->siGameMasterLevel;
							sendMsg.usCharID = pRecvMsg->usCharID;

							pSession = g_agp.GetSessionManager()->GetSession( pRecvMsg->usGameServerID );
							if( pSession != NULL && pSession->GetState() == SESSION_STATE_ESTABLISHED )
							{
								pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
							}
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
