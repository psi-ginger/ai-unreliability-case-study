#include "AuthGameProtocol.h"
#include "AuthUserList.h"
#include "AccountDBProcess.h"
#include "UserManager.h"
#include "../FrontServer/Log.h"

#ifdef USE_HANAUTH
	// - NHN ASM -
	#include "./HanAuth/HanAuthForSvr.h"
	// - NHN ASM -
#endif //USE_HANAUTH

#ifdef _CHINA
	// 중국 웹서비스
	#include <comutil.h>
	#include "../../NLib/md5.h"
	#include "TiredSystem.h"
	AEService::CAEService			    g_UserTiredSystem;
	CLogPrint							g_Log;
#endif

extern CLogPrint			g_CLogPrint;
extern BOOL					g_bRun;

//extern bool GamePotReport(int nCount);
//extern BOOL HTTPHangameRTA(int Count);

#include "./JapanAuth/JapanAuth.h"

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#include "./Porte/PorteAuthForSvr.h"	
	using namespace Porte::PorteAuthSystem;
#endif // USE_PORTEAUTH

	//KHY - 0218 - 중국 NHN 피로도 모듈 적용 - 인증모듈이 적용되어야만 한다.
#ifdef USE_HANANTITOXIC
	#include "./HanAntiToxic/HanAntiToxic.h"
#endif


AuthGameProtocol::AuthGameProtocol()
{
//	m_kUpdateTimer_GamepotJP.Init(60 * 1000);
//	m_kUpdateTimer_HangameJP.Init(10 * 60 * 1000);
}

AuthGameProtocol::~AuthGameProtocol()
{
	
}

void AuthGameProtocol::Init(UI16 usGameServerListenPort)
{
	m_SF.CreateFactory( GetSessionManager(), 30, 5 * 1000 * 1000, 5 * 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), usGameServerListenPort );
	
	SetFPS( 200 );

//	m_kUpdateTimer_GamepotJP.SetActive(true, ::GetTickCount());
//	m_kUpdateTimer_HangameJP.SetActive(true, ::GetTickCount());
}


void AuthGameProtocol::LogOut( ITZSession *pSession )
{
	
}

void AuthGameProtocol::SessionProc( ITZSession *pSession )
{

	int count = pSession->GetRecvBuffer()->GetPacketCount();

	for( int i = 0; i < count; ++i ) {

		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( pPacket == NULL ) return;										// 패킷이 없으면 나간다
		
		// 패킷 커맨드에 따른 처리
		switch( pPacket->usCmd ) {

		case AUTHMSG_REPORT_GAMEHEARTBEAT:
			{
			}
			break;

		case AUTHMSG_REQUEST_GAMEAUTH:
			{
#ifdef _CHINA
				// webservice-
				CoInitialize(NULL);

				printf( "Request user auth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GameAuth	*pRecvMsg = (sRequest_GameAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_GameAuth	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GAMEAUTH;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pSession->m_usIndex;
				sendMsg.usCharID = pRecvMsg->usCharID;

				sendMsg.idnum = pRecvMsg->siAuthKey;

				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				_bstr_t serverID = pRecvMsg->userServerID;
				_bstr_t id = pRecvMsg->userid;
				_bstr_t pw = pRecvMsg->userpw;
				_bstr_t ip = pRecvMsg->userIP;

				_bstr_t resultstr = "Tired Login";

				const size_t newsize = 100;
				char nstring[newsize];
				strncpy(nstring, (char *)resultstr, 100 );
				printf( "%s\n", nstring );

				BSTR  bstr = resultstr.copy();  

		//		g_Log.FilePrint( "TiredSystem.txt", "Tired Login" );

				SI32 result = -1;
				SI32 tryTiredSystem = 0;
				g_UserTiredSystem.HLJZLogIn( serverID, id, ip, &result, &bstr ) ;

				resultstr.Assign( bstr );
				strncpy(nstring, (char *)resultstr, 100 );

				if( strcmp(nstring, "Tired Login") == 0 )
				{
					tryTiredSystem = 1;
					g_UserTiredSystem.HLJZLogIn( serverID, id, ip, &result, &bstr ) ;
				}

				resultstr.Assign( bstr );
				strncpy(nstring, (char *)resultstr, 100 );

				printf( "Tired Login Result = %d %s %d %s \n" , tryTiredSystem, pRecvMsg->userServerID, result, nstring);
		//		g_Log.FilePrint( "TiredSystem.txt", "Tired Login Result Time[Try : %d][ServerID : %s][Result : %d][Message : %s]", tryTiredSystem, pRecvMsg->userServerID, result, nstring );

				SysFreeString ( bstr );

				sendMsg.siPlayingTime = result;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "IDNUM:%d, ID:%s, PW:%s\n", sendMsg.idnum, sendMsg.userid, sendMsg.userpw );

#else
				printf( "Request user auth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GameAuth	*pRecvMsg = (sRequest_GameAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_GameAuth	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GAMEAUTH;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pSession->m_usIndex;
				sendMsg.usCharID = pRecvMsg->usCharID;

				sendMsg.idnum = pRecvMsg->siAuthKey;

				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				sendMsg.siPlayingTime = -1;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "IDNUM:%d, ID:%s, PW:%s\n", sendMsg.idnum, sendMsg.userid, sendMsg.userpw );
#endif
			}
			break;

		case AUTHMSG_REQUEST_HANAUTH_GAMEAUTH:
			{
#ifdef USE_HANAUTH
				// 화면에 로그 출력
				printf( "[HANAUTH] Request user HanAuth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_HanAuth_GameAuth	*pRecvMsg = (sRequest_HanAuth_GameAuth *)pPacket;

				// HanAuth 인증과정을 거친다.
				char szOutBuf[ SIZE_OUTBUFFER ] = {0x00};
				//int ret = HanAuthForSvr( pRecvMsg->userid, pRecvMsg->authstring, szOutBuf, SIZE_OUTBUFFER );
				if ( pRecvMsg->userid == NULL)	break	;
				if ( pRecvMsg->authstring == NULL)	break	;
				int ret = HanAuthForSvr( pRecvMsg->userid, pRecvMsg->authstring, szOutBuf, sizeof( szOutBuf ) );
				// 로그 출력
				g_CLogPrint.WriteFileLog("HanAuth.txt", __FILE__, __LINE__, "[GAMEAUTH] UserID : [%s], OutBuf : [%s]", pRecvMsg->userid, szOutBuf);

				if( ret == 0 )
				{
					// AccountDBManager로 보내기 전에 나이를 먼저 체크한다.
					char szTempOutBuf[ SIZE_OUTBUFFER ] = {0x00};

					// 한게임 데이터 추출
					strcpy(szTempOutBuf, szOutBuf);
					char* pRet = NULL;
					char* pID = NULL;
					char* pAge = NULL;
					char* pIDNo = NULL;

					char szAge[32] = {0, };		// 나이 - for KOREA
					char szIDNo[32] = {0, };	// IDNO - for USA

					SI32 siAge = 0;
					UI32 uiIDNo = 0;

					pRet = strtok(szTempOutBuf, "|");
					if(pRet != NULL)		pID   = strtok(NULL, "|");
					if(pID  != NULL)		pAge  = strtok(NULL, "|");	// 나이 - for KOREA
					if(pAge != NULL)		pIDNo = strtok(NULL, "|");	// IDNO - for USA

					if(pAge != NULL)		strncpy(szAge, pAge, sizeof(szAge));
					if(strlen(szAge))		siAge = atoi(szAge);		// 나이 - for KOREA

					if(pIDNo != NULL)		strncpy(szIDNo, pIDNo, sizeof(szIDNo));
					if(strlen(szIDNo))		uiIDNo = (UI32)atoi(szIDNo);		// IDNO - for USA

					if(!(pRecvMsg->usServerAge <= siAge))
					{
						// 나이가 안되서 서버에 입장 할 수 없다.
						sResponse_GameAuth	sendMsg;

						sendMsg.packetHeader.usSize = sizeof( sendMsg );
						sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_GAMEAUTH;

						sendMsg.usCharID = pRecvMsg->usCharID;

						sendMsg.uiIDNum = pRecvMsg->siAuthKey;

						sendMsg.siSex = 0;

						sendMsg.siGameMasterLevel = 0;
						sendMsg.siAccountStatus = -1000;

						sendMsg.siBlockMode = 0;
						sendMsg.siBlockYear = 0;
						sendMsg.siBlockMonth = 0;
						sendMsg.siBlockDay = 0;
						sendMsg.siBlockHour = 0;
						sendMsg.siBlockMin = 0;

						sendMsg.szBlockReason[ 0 ] = NULL;
						sendMsg.siPlayingTime = -1;

						sendMsg.uiIDNo = 0;		// 한게임용 IDNO - by LEEKH 2007.11.11

						// 화면에 로그 출력
						printf( "[HANAUTH] IDNUM:%d, ServerAge:%d, UserAge:%d, AccountStatus:%d\n", sendMsg.uiIDNum, pRecvMsg->usServerAge, siAge, sendMsg.siAccountStatus);

						// 로그 출력
						g_CLogPrint.WriteFileLog("HanAuth_Error.txt", __FILE__, __LINE__, "[GAMEAUTH] refuse to enter server - UserID : [%s], ServerAge : [%d], UserAge : [%d]", pRecvMsg->userid, pRecvMsg->usServerAge, siAge);

						if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED )					
						{
							pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
						}

						break;
					}

					// AccountDBManager로 인증 요청
					sDBRequest_GameAuth	sendMsg;

					sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GAMEAUTH;
					sendMsg.packetHeader.usSize = sizeof( sendMsg );

					sendMsg.usGameServerID	= pSession->m_usIndex;
					sendMsg.usCharID		= pRecvMsg->usCharID;
					sendMsg.idnum			= pRecvMsg->siAuthKey;

					memcpy( sendMsg.userid, pRecvMsg->userid, 20 );			sendMsg.userid[ 19 ] = NULL;
					sendMsg.userpw[ 0 ]		= NULL;

					sendMsg.uiIDNo = uiIDNo;		// 한게임용 IDNO - by LEEKH 2007.11.11

					g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

					// 화면에 로그 출력
					printf( "[HANAUTH] IDNUM:%d, ID:%s, PW:%s IDNO: %d \n", sendMsg.idnum, sendMsg.userid, sendMsg.userpw, sendMsg.uiIDNo );

					// 로그 출력
					g_CLogPrint.WriteFileLog("HanAuth.txt", __FILE__, __LINE__, "[GAMEAUTH] allow to enter server - UserID : [%s], ServerAge : [%d], UserAge : [%d]", pRecvMsg->userid, pRecvMsg->usServerAge, siAge);
				}
				else
				{
					//------------------------------------------------------------
					// 한게임 인증 함수 실패
					sResponse_GameAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_GAMEAUTH;

					sendMsg.usCharID			= pRecvMsg->usCharID;
					sendMsg.uiIDNum				= pRecvMsg->siAuthKey;
					sendMsg.siSex				= 0;

					sendMsg.siGameMasterLevel	= 0;
					sendMsg.siAccountStatus		= -1100;

					sendMsg.siBlockMode			= 0;
					sendMsg.siBlockYear			= 0;
					sendMsg.siBlockMonth		= 0;
					sendMsg.siBlockDay			= 0;
					sendMsg.siBlockHour			= 0;
					sendMsg.siBlockMin			= 0;

					sendMsg.szBlockReason[ 0 ]	= NULL;
					sendMsg.siPlayingTime		= -1;

					sendMsg.uiIDNo				= 0;		// 한게임용 IDNO - by LEEKH 2007.11.11

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
					}

					// 로그 출력
					g_CLogPrint.WriteFileLog("HanAuth_Error.txt", __FILE__, __LINE__, "[GAMEAUTH] HanAuthForServer() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);

					// 프로그램 강제 종료
					if( ret == HAN_AUTHSVR_DLL_UNEXPECTED)
					{
						g_bRun = FALSE;
						break;
					}
				}
#endif //USE_HANAUTH
			}
			break;

		case AUTHMSG_REQUEST_TAIWAN_GAMEAUTH:
			{
#ifdef _TAIWAN
				printf( "[TAIWAN] Request user auth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_Taiwan_GameAuth	*pRecvMsg = (sRequest_Taiwan_GameAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_GameAuth	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GAMEAUTH;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pSession->m_usIndex;
				sendMsg.usCharID = pRecvMsg->usCharID;

				sendMsg.idnum = pRecvMsg->siAuthKey;

				memcpy( sendMsg.userid, pRecvMsg->gameaccount, sizeof(sendMsg.userid) );
				sendMsg.userid[ sizeof(sendMsg.userid) - 1 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->onetinepassword, sizeof(sendMsg.userpw) );
				sendMsg.userpw[ sizeof(sendMsg.userpw) - 1 ] = NULL;

				sendMsg.siPlayingTime = -1;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "[TAIWAN] IDNUM:%d, ID:%s, PW:%s\n", sendMsg.idnum, sendMsg.userid, sendMsg.userpw );
#endif
			}

		case AUTHMSG_REQUEST_GAMELOGOUT:
			{
#ifdef _CHINA
				// webservice-
				CoInitialize(NULL);

				printf( "Request user logout from GameServer\n" );

				sRequest_GameLogout	*pRecvMsg = (sRequest_GameLogout *)pPacket;

				_bstr_t serverID = pRecvMsg->userServerID;
				_bstr_t id = pRecvMsg->szAccountID;
				_bstr_t playingCharName = pRecvMsg->szPlayingCharNmae;

				_bstr_t resultstr = "Tired LogOut";

				const size_t newsize = 100;
				char nstring[newsize];
				strncpy(nstring, (char *)resultstr, 100 );
				printf( "%s\n", nstring );

				BSTR  bstr = resultstr.copy();  

		//		g_Log.FilePrint( "TiredSystem.txt", "Tired LogOut" );

				SI32 result = -1;
				SI32 tryTiredSystem = 0;
				g_UserTiredSystem.HLJZLogOut( serverID, id, playingCharName, pRecvMsg->siLevel, &result, &bstr ) ;

				resultstr.Assign( bstr );
				strncpy(nstring, (char *)resultstr, 100 );

				if( -1 == result && strcmp( nstring, "Tired LogOut" ) == 0 )
				{
					tryTiredSystem = 1;
					g_UserTiredSystem.HLJZLogOut( serverID, id, playingCharName, pRecvMsg->siLevel, &result, &bstr ) ;

					if( -1 == result && strcmp( nstring, "error" ) == 0 )
					{
						g_UserTiredSystem.HLJZLogOut( serverID, id, playingCharName, pRecvMsg->siLevel, &result, &bstr ) ;
					}

					resultstr.Assign( bstr );
					strncpy(nstring, (char *)resultstr, 100 );
				}

				if( -1 == result && strcmp( nstring, "Tired LogOut" ) == 0 )
				{
					tryTiredSystem = 2;
					g_UserTiredSystem.HLJZLogOut( serverID, id, playingCharName, pRecvMsg->siLevel, &result, &bstr ) ;

					if( -1 == result && strcmp( nstring, "error" ) == 0 )
					{
						g_UserTiredSystem.HLJZLogOut( serverID, id, playingCharName, pRecvMsg->siLevel, &result, &bstr ) ;
					}

					resultstr.Assign( bstr );
					strncpy(nstring, (char *)resultstr, 100 );
				}

		//		g_Log.FilePrint( "TiredSystem.txt", "Tired LogOut Result Time[Try : %d][ServerID : %s][Result : %d][Message : %s]", tryTiredSystem, pRecvMsg->userServerID, result, nstring );

				printf( "Tired LogOut Result = %d %s %d %s\n" , tryTiredSystem, pRecvMsg->userServerID, result, nstring );

				SysFreeString ( bstr ); 
				
				g_usermanager.GameLogOut( pRecvMsg->uiIDNum );
#else
				printf( "Request user logout from GameServer\n" );
				
				sRequest_GameLogout	*pRecvMsg = (sRequest_GameLogout *)pPacket;

//KHY - 0218 - 중국 NHN 피로도 모듈 적용 - 플레이 타임 저장.
#ifdef USE_HANANTITOXIC							
				SI32	siHanAntiToxicSetRet = 0;

				printf( "AUTHMSG_REQUEST_GAMELOGOUT - szAccountID = %s,  siPlayTimeMinute =%d  \n", pRecvMsg->szAccountID, pRecvMsg->siPlayTimeMinute );
							
				if(pRecvMsg->siPlayTimeMinute > 0) // 0보다 작다면, 피로도 적용대상이 아니었다.
				{
					siHanAntiToxicSetRet = HanSetAntiToxic("N",pRecvMsg->szAccountID,pRecvMsg->siPlayTimeMinute);
					if(siHanAntiToxicSetRet < 0)		
					{	
						SI32 HanAntiToxicReTryCount = 0;

						while(siHanAntiToxicSetRet < 0) //꼭 성공 해야 한다.
						{
							HanAntiToxicReTryCount++;

							g_CLogPrint.WriteFileLog("AuthserverProcessLog_HanAntiToxicRet.txt", __FILE__, __LINE__, "HanAntiToxicReTryCount = %d, HanSetAntiToxic Ret=%d, szAccountID = %s,  siPlayTimeMinute =%d", HanAntiToxicReTryCount , siHanAntiToxicSetRet , pRecvMsg->szAccountID , pRecvMsg->siPlayTimeMinute);	

							if(HanAntiToxicReTryCount < 10) // 10번만 돌자.
							{
								SI32	siHanAntiToxicInitRet = 0;

								siHanAntiToxicInitRet = HanAntiToxicInit("C_GZ");
								if(siHanAntiToxicInitRet < 0)		
								{	
									printf( "DBMSG_RESPONSE_GAMEAUTH - siHanAntiToxicInitRet =%d \n", siHanAntiToxicInitRet );
								}
								else
								{
									//재시도.
									siHanAntiToxicSetRet = HanSetAntiToxic("N",pRecvMsg->szAccountID,pRecvMsg->siPlayTimeMinute);
								}
							}
							else
							{
								siHanAntiToxicSetRet = 100; // 빠져나오자.
							}
						}								

					}
				}
#endif //USE_HANANTITOXIC

				SI32 siAccountStatus = 0;

				if( _tcsstr(pRecvMsg->szAccountID, TEXT("@HG")) != NULL )
					siAccountStatus = 4;
				else if( _tcsstr(pRecvMsg->szAccountID, TEXT("@GP")) != NULL )
					siAccountStatus = 8;

				// 동접수 감소
				switch(siAccountStatus)
				{
				case 2:		g_usermanager.m_GamaniaJPCount--;	break;
				case 4:		g_usermanager.m_HanGameJPCount--;	break;
				case 8:		g_usermanager.m_GamepotJPCount--;	break;
				case 10:	g_usermanager.m_NEXONJPCount--;	break;
				case 0:		g_usermanager.m_NDEJPCount--;	break;
				}

				g_usermanager.GameLogOut( pRecvMsg->uiIDNum );
#endif
			}
			break;
			
		case AUTHMSG_REQUEST_HANAUTH_GAMERUNAUTH:
			{
#ifdef USE_HANAUTH
#ifdef _KOREA  // -- 실시간 나이체크는 한국 NHN만 적용한다.
				// 실시간 나이체크 삭제
/*				printf( "Request user HanAuth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_HanAuth_GameRunAuth	*pRecvMsg = (sRequest_HanAuth_GameRunAuth *)pPacket;

				// HanAuth 인증과정을 거친다.

				char szOutBuf[ SIZE_OUTBUFFER ] = {0x00};

				int ret = HanAuthForSvr( pRecvMsg->userid, pRecvMsg->authstring, szOutBuf, SIZE_OUTBUFFER );

				if( ret == 0 )
				{
					// AccountDBManager로 보내기 전에 나이를 먼저 체크한다.
					char szTempOutBuf[ SIZE_OUTBUFFER ] = {0x00};

					// 나이 데이터 추출
					strcpy(szTempOutBuf, szOutBuf);
					char* pToken = NULL;
					char szAge[32];
					pToken = strtok(szTempOutBuf, "|");
					if(pToken != NULL)		pToken = strtok(NULL, "|");
					if(pToken != NULL)		pToken = strtok(NULL, "|");

					strncpy(szAge, pToken, sizeof(szAge));
					SI32 siAge = atoi(szAge);

					// 서버의 나이와 비교
					if(!(pRecvMsg->usServerAge <= siAge))
					{
						// 나이가 안되서 서버에 입장 할 수 없다.
						sResponse_GameRunAuth	sendMsg;

						sendMsg.packetHeader.usSize = sizeof( sendMsg );
						sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH;

						sendMsg.usCharID = pRecvMsg->usCharID;

						sendMsg.siResult = 0;	// 인증에 실패
						printf( "CharID:%s, ServerAge:%d, UserAge:%d\n", sendMsg.usCharID, pRecvMsg->usServerAge, siAge);

						// 로그 출력
						g_CLogPrint.WriteFileLog("HanAuth_Error.txt", __FILE__, __LINE__, "[GAMERUNAUTH] refuse to use server - UserID : [%s], ServerAge : [%d], UserAge : [%d]", pRecvMsg->userid, pRecvMsg->usServerAge, siAge);

						if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED ) 
						{
							pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
						}
						break;
					}

					sResponse_GameRunAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH;

					sendMsg.usCharID = pRecvMsg->usCharID;

					sendMsg.siResult = 1;	// 인증에 성공

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
					}

					// 로그 출력
					//g_CLogPrint.WriteFileLog("HanAuth.txt", __FILE__, __LINE__, "[GAMERUNAUTH] allow to use server - UserID : [%s], ServerAge : [%d], UserAge : [%d]", pRecvMsg->userid, pRecvMsg->usServerAge, siAge);

				}
				else
				{
					sResponse_GameRunAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH;

					sendMsg.usCharID = pRecvMsg->usCharID;

					sendMsg.siResult = 1;	// 인증에 성공

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED )
					{
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	
					}

					// 로그 출력
					//g_CLogPrint.WriteFileLog("HanAuth_Error.txt", __FILE__, __LINE__, "[GAMERUNAUTH] HanAuthForServer() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);
				}*/
#endif
#endif //USE_HANAUTH
			}
			break;

		case AUTHMSG_REQUEST_GMTOOLAUTH:
			{
				printf( "Request GMTOOL auth from GameServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GMToolAuth*	pRecvMsg = (sRequest_GMToolAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_GMTOOLLogin	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_GMTOOLLOGIN;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pSession->m_usIndex;
				sendMsg.usCharID = pRecvMsg->usCharID;

				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "ID:%s, PW:%s, CharID:%d \n", sendMsg.userid, sendMsg.userpw, sendMsg.usCharID );
			}
			break;
		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void AuthGameProtocol::Update()
{
/*	if( m_kUpdateTimer_GamepotJP.IsTimed(::GetTickCount()) == true )
	{
		GamePotReport(g_usermanager.m_GamepotJPCount);
	}

	if( m_kUpdateTimer_HangameJP.IsTimed(::GetTickCount()) == true )
	{
		HTTPHangameRTA(g_usermanager.m_HanGameJPCount);
	}*/
}
