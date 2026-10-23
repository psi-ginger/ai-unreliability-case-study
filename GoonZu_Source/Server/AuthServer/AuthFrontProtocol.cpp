#include <windows.h>
/*#include "Wininet.h"

#include <comutil.h>
#include <tchar.h>
#include <atlcomcli.h>

#include "C:\Program Files\MSSOAP\Inc\mssoap30.h"
#pragma comment(lib, "C:\\Program Files\\MSSOAP\\Lib\\mssoap30.lib") 
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "comsupp.lib")
*/

#include "AuthUserList.h"
#include "AuthFrontProtocol.h"
#include "AccountDBProcess.h"

#include "../FrontServer/Log.h"

#include "NUtil.h"

#ifdef USE_HANAUTH
	// - NHN ASM -
	#include "./HanAuth/HanAuthForSvr.h"
	// - NHN ASM -
#endif //USE_HANAUTH

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#include "./Porte/PorteAuthForSvr.h"	
	using namespace Porte::PorteAuthSystem;
#endif // USE_PORTEAUTH

//KHY - 0218 - 중국 NHN 피로도 모듈 적용 - 인증모듈이 적용되어야만 한다.
#ifdef USE_HANANTITOXIC
	#include "./HanAntiToxic/HanAntiToxic.h"

	extern TCHAR g_HanSID[MAX_SID_BUF];
	extern BOOL g_Tired;
#endif


extern CLogPrint			g_CLogPrint;
extern BOOL					g_bRun;

#include "./JapanAuth/JapanAuth.h"

int CALLBACK JapanAuthCallBackFunc(AuthInfo info, int nRet);

AuthFrontProtocol::AuthFrontProtocol()
{

}

AuthFrontProtocol::~AuthFrontProtocol()
{
	JapanAuthShutdown();
}

void AuthFrontProtocol::Init(UI16 usFrontServerListenPort)
{
	m_SF.CreateFactory( GetSessionManager(), 1, 20 * 1000 * 1000, 20 * 1000 * 1000 );

	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), usFrontServerListenPort );

	SetFPS( 200 );

	JapanAuthInit(20, JapanAuthCallBackFunc);
}


void AuthFrontProtocol::LogOut( ITZSession *pSession )
{

}

void AuthFrontProtocol::SessionProc( ITZSession *pSession )
{

	int count = pSession->GetRecvBuffer()->GetPacketCount();

	for( int i = 0; i < count; ++i ) {

		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( pPacket == NULL ) return;										// 패킷이 없으면 나간다
		
		// 패킷 커맨드에 따른 처리
		switch( pPacket->usCmd ) {

		case AUTHMSG_REQUEST_FRONTAUTH:
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_FrontAuth	*pRecvMsg = (sRequest_FrontAuth *)pPacket;

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

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;

		case AUTHMSG_REQUEST_KOREA_FRONTAUTH:		// [종호_NHN->NDOORS] Korea 인증
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_FrontAuth	*pRecvMsg = (sRequest_FrontAuth *)pPacket;

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

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;
//---------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가.
		case AUTHMSG_REQUEST_HANGAMEJP_FRONTAUTH:
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_HANGAMEJP_FrontAuth	*pRecvMsg = (sRequest_HANGAMEJP_FrontAuth *)pPacket;

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

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;
		case AUTHMSG_REQUEST_GAMANIAJP_FRONTAUTH:
			{
				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GAMANIAJP_FrontAuth	*pRecvMsg = (sRequest_GAMANIAJP_FrontAuth *)pPacket;

				// -  감마니아는 인증을 보낼때, @GA를 빼고 보낸다. 
				TCHAR	GA_userid[ 20 ] = {'0',};

				// @퍼블리셔 를 빼는 함수.
				DeltPublisherID(pRecvMsg->userid ,GA_userid);
				
				//int ret  = HttpsPostAuth(GA_userid , pRecvMsg->userpw); 
				AuthInfo info;
				info.Set(GA_userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->macaddress);
				info.m_usPacketUnique	= pRecvMsg->usPacketUnique;
				info.m_usFrontCharID	= pRecvMsg->usFrontCharID;

				int ret = JapanAuth_GamaniaAuth(info);	// 감마니아는 자체 인증이 필요하다.

				if( ret == 0)	// 인증 모듈에서 실패!
				{
					// FrontServer로 인증 실패 전송
					sResponse_FrontAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

					// GAMANIAJP 인증실패
					sendMsg.usResult = 14;

					sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

					sendMsg.siAuthKey = 0;

					sendMsg.siGameMasterLevel = 0;

					sendMsg.siBlockMode = 0;
					sendMsg.siBlockYear = 0;
					sendMsg.siBlockMonth = 0;
					sendMsg.siBlockDay = 0;
					sendMsg.siBlockHour = 0;
					sendMsg.siBlockMin = 0;

					sendMsg.szBlockReason[ 0 ] = NULL;
					sendMsg.bAlreadyMakeCharSwitch = 0;
					sendMsg.szServerName[ 0 ] = NULL;

					sendMsg.siAccountStatus = 0;

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED) 
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	

					// 로그 출력
					g_CLogPrint.WriteFileLog("GAMANIAJP_Error.txt", __FILE__, __LINE__, "[FRONTAUTH] GAMANIAJPAuthForServer() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);

				}

			}
			break;

		case AUTHMSG_REQUEST_GAMEPOTJP_FRONTAUTH:
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_GAMEPOTJP_FrontAuth	*pRecvMsg = (sRequest_GAMEPOTJP_FrontAuth *)pPacket;

				//int ret = SoapAuth(pRecvMsg->userid , pRecvMsg->userpw ,pRecvMsg->userIP ); 
				AuthInfo info;
				info.Set(pRecvMsg->userid, pRecvMsg->userpw, pRecvMsg->userIP, pRecvMsg->macaddress);
				info.m_usPacketUnique	= pRecvMsg->usPacketUnique;
				info.m_usFrontCharID	= pRecvMsg->usFrontCharID;

				int ret = JapanAuth_GamepotAuth(info);

				if(ret == 0) // 인증모듈에서 실패 했음!!// 게임팟은 자체 인증이 필요하다.
				{
					// FrontServer로 인증 실패 전송
					sResponse_FrontAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

					// GamePot 인증실패
					sendMsg.usResult = 15;

					sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

					sendMsg.siAuthKey = 0;

					sendMsg.siGameMasterLevel = 0;

					sendMsg.siBlockMode = 0;
					sendMsg.siBlockYear = 0;
					sendMsg.siBlockMonth = 0;
					sendMsg.siBlockDay = 0;
					sendMsg.siBlockHour = 0;
					sendMsg.siBlockMin = 0;

					sendMsg.szBlockReason[ 0 ] = NULL;
					sendMsg.bAlreadyMakeCharSwitch = 0;
					sendMsg.szServerName[ 0 ] = NULL;

					sendMsg.siAccountStatus = 0;

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED) 
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	

					// 로그 출력
					g_CLogPrint.WriteFileLog("GAMANIAJP_Error.txt", __FILE__, __LINE__, "[FRONTAUTH] GAMEPOTJPAuthForServer() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);

				}
			}
			break;

		case AUTHMSG_REQUEST_NDEJP_FRONTAUTH:
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_NDEJP_FrontAuth	*pRecvMsg = (sRequest_NDEJP_FrontAuth *)pPacket;

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

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;
		case AUTHMSG_REQUEST_NEXONJP_FRONTAUTH:  // 넥슨 은 아직 알 수 없다.
			{

				printf( "Request user auth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_NEXONJP_FrontAuth	*pRecvMsg = (sRequest_NEXONJP_FrontAuth *)pPacket;


				// AccountDBManager로 인증 요청 -
				sDBRequest_IDNumGet	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;


				// 넥슨은  ID에 들어있는 정보에 대한 파싱이 필요하다.
				NexonjpIDDataDivision(pRecvMsg, &sendMsg);

				//memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				//sendMsg.userid[ 19 ] = NULL;
				//memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				//sendMsg.userpw[ 19 ] = NULL;

				memcpy( sendMsg.userIP, pRecvMsg->userIP, 20 );
				sendMsg.userIP[ 19 ] = NULL;

				memcpy( sendMsg.macaddress, pRecvMsg->macaddress, 20 );
				sendMsg.macaddress[ 19 ] = NULL;

				sendMsg.jumin[ 0 ] = NULL;
				
				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "[%s] [%s] [%s] [%s] [%s] \n",sendMsg.userid,sendMsg.requestTime,sendMsg.mkey1,sendMsg.mkey2,sendMsg.key);


				//printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );

			}
			break;

//---------------------------------------------------------
		case AUTHMSG_REQUEST_HANAUTH_FRONTAUTH:
			{
#ifdef USE_HANAUTH
				// 화면에 로그 출력
				printf( "[HANAUTH] Request user HanAuth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_HanAuth_FrontAuth	*pRecvMsg = (sRequest_HanAuth_FrontAuth *)pPacket;
				// HanAuth 인증과정을 거친다.
				char szOutBuf[ SIZE_OUTBUFFER ] = {0x00};
				int ret = HanAuthForSvr( pRecvMsg->userid, pRecvMsg->authstring, szOutBuf, SIZE_OUTBUFFER, pRecvMsg->userIP );

				// 로그 출력
				g_CLogPrint.WriteFileLog("HanAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], OutBuf : [%s]", pRecvMsg->userid, szOutBuf);

				if( ret == 0 )
				{
					TCHAR szSSN[24] = { '\0', };

					SI32 siAge = HanAuthGetUserAge(szOutBuf);
					SI32 siSex = HanAuthGetUserSex(szOutBuf);

					switch(siSex)
					{
					case HAN_AUTHSVR_USER_IS_MALE:		siSex = 0;		break;
					case HAN_AUTHSVR_USER_IS_FEMALE:	siSex = 1;		break;
					default:							siSex = -1;		break;
					}
					if(siSex >= 0 && siAge > 0 && siAge < 200)
                        SSNCreate(siAge, siSex, szSSN, sizeof(szSSN));

					// AccountDBManager로 인증 요청
					sDBRequest_IDNumGet	sendMsg;

					sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
					sendMsg.packetHeader.usSize = sizeof( sendMsg );

					sendMsg.usPacketUnique	= pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID	= pRecvMsg->usFrontCharID;

					memcpy( sendMsg.userid, 	pRecvMsg->userid, 		20 );	sendMsg.userid[ 19 ] = NULL;
					memcpy( sendMsg.userpw, 	pRecvMsg->userpw, 		20 );	sendMsg.userpw[ 19 ] = NULL;
					memcpy( sendMsg.userIP, 	pRecvMsg->userIP, 		20 );	sendMsg.userIP[ 19 ] = NULL;
					memcpy( sendMsg.macaddress, pRecvMsg->macaddress,	20 );	sendMsg.macaddress[ 19 ] = NULL;
					memcpy( sendMsg.jumin,		szSSN,					20 );	sendMsg.jumin[ 19 ] = NULL;

					g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

					// 화면에 로그 출력
					printf( "[HANAUTH] ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
				}
				// HanAuth 인증 함수 실패
				else
				{
					// FrontServer로 인증 실패 전송
					sResponse_FrontAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

					// 한게임 인증실패
					sendMsg.usResult = 12;

					sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

					sendMsg.siAuthKey = 0;

					sendMsg.siGameMasterLevel = 0;

					sendMsg.siBlockMode = 0;
					sendMsg.siBlockYear = 0;
					sendMsg.siBlockMonth = 0;
					sendMsg.siBlockDay = 0;
					sendMsg.siBlockHour = 0;
					sendMsg.siBlockMin = 0;

					sendMsg.szBlockReason[ 0 ] = NULL;
					sendMsg.bAlreadyMakeCharSwitch = 0;
					sendMsg.szServerName[ 0 ] = NULL;

					sendMsg.siAccountStatus = 0;

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED) 
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	

					// 로그 출력
					g_CLogPrint.WriteFileLog("HanAuth_Error.txt", __FILE__, __LINE__, "[FRONTAUTH] HanAuthForServer() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);

					// 프로그램 강제 종료
					if( ret == HAN_AUTHSVR_DLL_UNEXPECTED)
					{
						g_bRun = FALSE;
						break;
					}
				}
#endif // USE_HANAUTH
			}
			break;

//---------------------------------------------------------
		case AUTHMSG_REQUEST_PORTEAUTH_FRONTAUTH:
			{
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH

				// 화면에 로그 출력
				printf( "[PORTEAUTH] Request user PorteAuth from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_PorteAuth_FrontAuth	*pRecvMsg = (sRequest_PorteAuth_FrontAuth *)pPacket;
				// HanAuth 인증과정을 거친다.
				char szOutBuf[ SIZE_USERINFORMATION ] = {0x00};
				int ret = PubCheckAuth( pRecvMsg->userid, pRecvMsg->authstring, pRecvMsg->userIP, szOutBuf, sizeof(szOutBuf) );

				// 로그 출력
				g_CLogPrint.WriteFileLog("PorteAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], OutBuf : [%s]", pRecvMsg->userid, szOutBuf);

				if( ret == 0 )
				{
					TCHAR szSex[8] = { '\0', };
					TCHAR szAge[8] = { '\0', };
					TCHAR szSIDval[MAX_SID_BUF] = { '\0', };
					TCHAR szTIREDval[8] = { '\0', };

					SI32 siSex = 0; 
					SI32 siAge = 0; 

					int GetValueret  = PubGetValue(szOutBuf,"SEX",szSex, sizeof(szSex));
					if(GetValueret ==0)
					{
						g_CLogPrint.WriteFileLog("PorteAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], Sex : [%s]", pRecvMsg->userid, szSex);
					}

					GetValueret  = PubGetValue(szOutBuf,"AGE",szAge, sizeof(szAge));
					if(GetValueret ==0)
					{
						g_CLogPrint.WriteFileLog("PorteAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], AGE : [%s]", pRecvMsg->userid, szAge);
					}

//KHY - 0218 - 중국 NHN 피로도 모듈 적용
#ifdef USE_HANANTITOXIC
					// 중독방지 모듈용, SID ( 신분증 번호)를 받는다. 
					GetValueret  = PubGetValue(szOutBuf,"SID",szSIDval, sizeof(szSIDval));
					if(GetValueret ==0)
					{						
						memcpy( g_HanSID, 	szSIDval, 	MAX_SID_BUF );
						g_CLogPrint.WriteFileLog("PorteAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], SID : [%s]", pRecvMsg->userid, szSIDval);
					}

					// 중독방지 모듈용, TIRED 적용 여부를 판단한다.
					g_Tired = FALSE;	

					GetValueret  = PubGetValue(szOutBuf,"TIRED",szTIREDval, sizeof(szTIREDval));
					if(GetValueret ==0)
					{	
						if(strcmp("Y", &szTIREDval[0]) ==0)
							 g_Tired = TRUE;							
						else
							 g_Tired = FALSE;						
						
						g_CLogPrint.WriteFileLog("PorteAuth.txt", __FILE__, __LINE__, "[FRONTAUTH] UserID : [%s], szTIREDval : [%c]", pRecvMsg->userid, szTIREDval[0]);
					}
#endif
					// AccountDBManager로 인증 요청
					sDBRequest_IDNumGet	sendMsg;

					sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
					sendMsg.packetHeader.usSize = sizeof( sendMsg );

					sendMsg.usPacketUnique	= pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID	= pRecvMsg->usFrontCharID;

					memcpy( sendMsg.userid, 	pRecvMsg->userid, 		20 );	sendMsg.userid[ 19 ] = NULL;
					memcpy( sendMsg.userpw, 	pRecvMsg->userpw, 		20 );	sendMsg.userpw[ 19 ] = NULL;
					memcpy( sendMsg.userIP, 	pRecvMsg->userIP, 		20 );	sendMsg.userIP[ 19 ] = NULL;
					memcpy( sendMsg.macaddress, pRecvMsg->macaddress,	20 );	sendMsg.macaddress[ 19 ] = NULL;

					g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

					// 화면에 로그 출력
					printf( "[PORTEAUTH] ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
				}
				// HanAuth 인증 함수 실패
				else
				{
					// FrontServer로 인증 실패 전송
					sResponse_FrontAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd = AUTHMSG_RESPONSE_FRONTAUTH;

					// 한게임 인증실패
					sendMsg.usResult = 20;

					sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

					sendMsg.siAuthKey = 0;

					sendMsg.siGameMasterLevel = 0;

					sendMsg.siBlockMode = 0;
					sendMsg.siBlockYear = 0;
					sendMsg.siBlockMonth = 0;
					sendMsg.siBlockDay = 0;
					sendMsg.siBlockHour = 0;
					sendMsg.siBlockMin = 0;

					sendMsg.szBlockReason[ 0 ] = NULL;
					sendMsg.bAlreadyMakeCharSwitch = 0;
					sendMsg.szServerName[ 0 ] = NULL;

					sendMsg.siAccountStatus = 0;

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED) 
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	

					// 로그 출력
					g_CLogPrint.WriteFileLog("PorteAuth_Error.txt", __FILE__, __LINE__, "[FRONTAUTH] AUTHMSG_REQUEST_PORTEAUTH_FRONTAUTH() Error - UserID : [%s], ErrorCode : [%d]", pRecvMsg->userid, ret);

					// 프로그램 강제 종료
					/*if( ret == HAN_AUTHSVR_DLL_UNEXPECTED)
					{
						g_bRun = FALSE;
						break;
					}*/
				}
#endif // USE_PORTEAUTH
			}
			break;
			
		case AUTHMSG_REQUEST_TAIWAN_FRONTAUTH:
			{
#ifdef _TAIWAN
				printf( "Request user auth (TAIWAN) from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_Taiwan_FrontAuth	*pRecvMsg = (sRequest_Taiwan_FrontAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_IDNumGet	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				memcpy( sendMsg.userid, pRecvMsg->gameaccount, sizeof(sendMsg.userid) );
				sendMsg.userid[ sizeof(sendMsg.userid) - 1 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->onetimepassword, sizeof(sendMsg.userpw) );
				sendMsg.userpw[ sizeof(sendMsg.userpw) - 1 ] = NULL;

				memcpy( sendMsg.userIP, pRecvMsg->userIP, sizeof(sendMsg.userIP) );
				sendMsg.userIP[ sizeof(sendMsg.userIP) - 1 ] = NULL;

				memcpy( sendMsg.macaddress, pRecvMsg->macaddress, sizeof(sendMsg.macaddress) );
				sendMsg.macaddress[ sizeof(sendMsg.macaddress) - 1 ] = NULL;

				sendMsg.jumin[ 0 ] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
#endif
			}
			break;
			
		// PCK : IOVATION 인증 요청 (2009.02.03)
		case AUTHMSG_REQUEST_ENGLISH_FRONTAUTH:
			{
				printf( "Request user auth (IOVATION) from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_English_FrontAuth	*pRecvMsg = (sRequest_English_FrontAuth *)pPacket;

				// AccountDBManager로 인증 요청
				sDBRequest_English_IDNumGet	sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_ENGLISH_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;
				sendMsg.accountstatus = 200;	// 200 => 글로벌에서 보냄을 AccountDBManager에 알림.
				
				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				memcpy( sendMsg.userIP, pRecvMsg->userIP, 20 );
				sendMsg.userIP[ 19 ] = NULL;

				memcpy( sendMsg.macaddress, pRecvMsg->macaddress, 20 );
				sendMsg.macaddress[ 19 ] = NULL;

				sendMsg.jumin[ 0 ] = NULL;

				memcpy( sendMsg.Blackbox, pRecvMsg->Blackbox, 2048 );
				sendMsg.macaddress[ 2047 ] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

				printf( "ID:%s\n, PW:%s\n, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
			}
			break;
		case AUTHMSG_REQUEST_USA_FRONTAUTH:
			{
				printf( "Request user auth (USA) from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_USA_FrontAuth	*pRecvMsg = (sRequest_USA_FrontAuth*)pPacket;

				AuthInfo info;
				info.Init();
				
				info.SetUSAAuthString( pRecvMsg->authstring );
				info.m_usPacketUnique	= pRecvMsg->usPacketUnique;
				info.m_usFrontCharID	= pRecvMsg->usFrontCharID;

				int ret = USAAuth_NHNAuth( info );	

				// 인증 모듈에서 실패!
				if( ret == 0 )	
				{
					// FrontServer로 인증 실패 전송
					sResponse_FrontAuth	sendMsg;

					sendMsg.packetHeader.usSize = sizeof( sendMsg );
					sendMsg.packetHeader.usCmd  = AUTHMSG_RESPONSE_FRONTAUTH;

					// USA 인증실패
					sendMsg.usResult		  		= 30;
					sendMsg.usPacketUnique    		= pRecvMsg->usPacketUnique;
					sendMsg.usFrontCharID     		= pRecvMsg->usFrontCharID;
					sendMsg.siAuthKey		  		= 0;
					sendMsg.siGameMasterLevel 		= 0;
					sendMsg.siBlockMode  			= 0;
					sendMsg.siBlockYear  			= 0;
					sendMsg.siBlockMonth 			= 0;
					sendMsg.siBlockDay   			= 0;
					sendMsg.siBlockHour  			= 0;
					sendMsg.siBlockMin   			= 0;
					sendMsg.szBlockReason[ 0 ]		= NULL;
					sendMsg.bAlreadyMakeCharSwitch	= 0;
					sendMsg.szServerName[ 0 ]		= NULL;
					sendMsg.siAccountStatus			= 0;

					if( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED) 
						pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );	

					// 로그 출력
					g_CLogPrint.WriteFileLog("USA_Error.txt", __FILE__, __LINE__, "[FRONTAUTH] USAAuthForServer() Error - Auth String : [%s], ErrorCode : [%d]", pRecvMsg->authstring, ret);
				}
			}
			break;

		case AUTHMSG_REQUEST_EUROPE_FRONTAUTH:
			{
				printf( "Request user auth(Europe) from FrontServer\n" );

				// 커맨드에 따른 메시지 구조체 캐스팅
				sRequest_EUROPE_FrontAuth *pRecvMsg = (sRequest_EUROPE_FrontAuth *)pPacket;

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

				printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );


			}
			break;

		case AUTHMSG_REQUEST_FRONTJOINGAME:
			{
				sRequest_FrontJoinGame	*pRecvMsg = (sRequest_FrontJoinGame *)pPacket;
			
				sDBRequest_JoinGame sendMsg;

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_JOINGAME;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				// id
				memcpy( sendMsg.userid, pRecvMsg->userid, 20 );
				sendMsg.userid[ 19 ] = NULL;

				// pw
				memcpy( sendMsg.userpw, pRecvMsg->userpw, 20 );
				sendMsg.userpw[ 19 ] = NULL;

				// cookie
				memcpy( sendMsg.cookie, pRecvMsg->cookie, 20 );
				sendMsg.cookie[ 19 ] = NULL;

				// ip
				memcpy( sendMsg.szIP, pRecvMsg->szIP, 20 );
				sendMsg.szIP[ 19 ] = NULL;

				// jumin
				sendMsg.jumin[0] = NULL;

				g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

			}
			break;
		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void AuthFrontProtocol::Update()
{

}

// 순수 아이디만을 파싱하는 함수.
void DeltPublisherID(TCHAR * Input_userid ,TCHAR * Output_userid)
{
	SI16 idSize = strlen(Input_userid);

	for(SI16 i =0;i < idSize; i++)
	{
		// "@" 문자 이전의 문자만을 리턴한다.
		if(Input_userid[i] == '@')
		{
			strncpy(Output_userid,Input_userid, i );
			break;
		}
	}	
}

int CALLBACK JapanAuthCallBackFunc(AuthInfo info, int nRet)
{
	switch(info.m_nAuthType)
	{
	case AUTH_TYPE_GAMEPOT:
		{
			if( 0 != nRet ) return 0;

			// AccountDBManager로 인증 요청
			sDBRequest_IDNumGet	sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.usPacketUnique	= info.m_usPacketUnique;
			sendMsg.usFrontCharID	= info.m_usFrontCharID;

			memcpy( sendMsg.userid, info.m_szID, 20 );
			sendMsg.userid[ 19 ] = NULL;

			memcpy( sendMsg.userpw, info.m_szPW, 20 );
			sendMsg.userpw[ 19 ] = NULL;

			memcpy( sendMsg.userIP, info.m_szIP, 20 );
			sendMsg.userIP[ 19 ] = NULL;

			memcpy( sendMsg.macaddress, info.m_szMAC, 20 );
			sendMsg.macaddress[ 19 ] = NULL;

			sendMsg.jumin[ 0 ] = NULL;

			g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

			printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
		}
		break;
	case AUTH_TYPE_GAMANIA:
		{
			if( 0 != nRet ) return 0;

			// AccountDBManager로 인증 요청
			sDBRequest_IDNumGet	sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.usPacketUnique	= info.m_usPacketUnique;
			sendMsg.usFrontCharID	= info.m_usFrontCharID;

			memcpy( sendMsg.userid, info.m_szID, 20 );
			if( strlen(sendMsg.userid) <= 16 )	{		strcat(sendMsg.userid, "@GA");		}
			sendMsg.userid[ 19 ] = NULL;

			memcpy( sendMsg.userpw, info.m_szPW, 20 );
			sendMsg.userpw[ 19 ] = NULL;

			memcpy( sendMsg.userIP, info.m_szIP, 20 );
			sendMsg.userIP[ 19 ] = NULL;

			memcpy( sendMsg.macaddress, info.m_szMAC, 20 );
			sendMsg.macaddress[ 19 ] = NULL;

			sendMsg.jumin[ 0 ] = NULL;

			g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

			printf( "ID:%s, PW:%s, IP:%s\n", sendMsg.userid, sendMsg.userpw, sendMsg.userIP );
		}
		break;
	case AUTH_TYPE_USA:
		{
			// AccountDBManager로 인증 요청
			sDBRequest_IDNumGet	sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_REQUEST_IDNUMGET;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.usPacketUnique	= info.m_usPacketUnique;
			sendMsg.usFrontCharID	= info.m_usFrontCharID;
			sendMsg.accountstatus   = 100;							// 100 => USA 에서 보냄을 AccountDBManager에 알림.
		
			printf( "USA AUTH STRING:%s\n", info.m_Authstring );	

			memcpy( sendMsg.userid, info.m_szID, 20 );
			sendMsg.userid[ 19 ] = NULL;

			memcpy( sendMsg.userIP, info.m_szIP, 20 );
			sendMsg.userIP[ 19 ] = NULL;

			sendMsg.jumin[ 0 ]	= NULL;

			g_accountDB.SendMsg( (sPacketHeader *)&sendMsg );

			printf( "ID:%s, IP:%s\n", sendMsg.userid, sendMsg.userIP );
		}
		break;
	}

	return 0;
}


//---------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가. - 감마니아용 인증 함수.
/*int HttpsPostAuth(TCHAR *pszID, TCHAR *pszPW)	// For 감마니아
{
	if(pszID == NULL || pszPW == NULL)			return -1;

	int bResult = -1;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("AuthTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[1024] = { '\0', };
	BOOL bRet = FALSE;

	wsprintf(szPostData, "id=%s&pw=%s", pszID, pszPW);

	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(hSession,
												"gash.gamania.co.jp",
												INTERNET_DEFAULT_HTTPS_PORT,
												"",
												"",
												INTERNET_SERVICE_HTTP,//urlcomponent.nScheme,
												0,
												0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest( hConnect,
													"POST",
													"/acxauth/b2b.auth/authkunshu.ashx",
													HTTP_VERSION,
													"",
													NULL,
													INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
													0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024] = { '\0', };

				wsprintf(szLen, "%d", strlen(szPostData));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet = ::HttpAddRequestHeaders(hObject,(LPCTSTR)aHeader,-1L,HTTP_ADDREQ_FLAG_ADD);
				bRet = ::HttpSendRequest(hObject, NULL, 0, (LPVOID)szPostData, (DWORD)strlen(szPostData));

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				char szResult[2048] = { '\0', };

				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					strcat(szResult, szBuffer);
					Sleep(1);
				}

				if(strcmp(szBuffer, "00") == 0)		bResult = 0;
				else if(strcmp(szBuffer, "01") == 0)		bResult = 1;
				else if(strcmp(szBuffer, "10") == 0)		bResult = 10;
				else if(strcmp(szBuffer, "11") == 0)		bResult = 11;
				else if(strcmp(szBuffer, "99") == 0)		bResult = 99;				

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}

BOOL HTTPHangameRTA(int Count)	// For HangameJP
{
	BOOL bResult = FALSE;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("HTTPHangameRTA", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[128] = { '\0', };
	char szObject[1024] = { '\0', };
	BOOL bRet = FALSE;

	sprintf(szObject, "publish/common/rta.asp?gameid=pubkunshu&cnt=%d", Count);

	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(hSession,
			"ssl.hangame.co.jp",
			INTERNET_DEFAULT_HTTP_PORT,
			"",
			"",
			INTERNET_SERVICE_HTTP,
			0,
			0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest( hConnect,
				"GET",
				szObject,
				HTTP_VERSION,
				"",
				NULL,
				NULL,
				0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024] = { '\0', };

				sprintf(szLen, "%d", strlen(szPostData));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet = ::HttpAddRequestHeaders(hObject,(LPCTSTR)aHeader,-1L,HTTP_ADDREQ_FLAG_ADD);
				bRet = ::HttpSendRequest(hObject, NULL, 0, (LPVOID)NULL, (DWORD)0);

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				DWORD dwTotalRead = 0;
				char szResult[2048] = { '\0', };

				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					dwTotalRead += dwRead;
					if( strlen(szResult) + dwRead >= sizeof(szResult) )
						break;
					strcat(szResult, szBuffer);
					Sleep(1);
				}

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}

//KHY - 0617 - 일본 채널링 추가. - 게임팟용 인증 함수.
int SoapAuth(TCHAR *pszID, TCHAR *pszPW ,  TCHAR *puserIP)	// For GamePot
{
	HRESULT	hr;

	hr = CoInitialize(NULL);

	//bool ret = GamePotAuth("channeling001@GP", "1CE2BD159ED941D1", "127.0.0.1");
	SI16 ret = GamePotAuth(pszID, pszPW, puserIP);

	CoUninitialize();

	return ret;
}


int GamePotAuth(char* pszID, char* pszPW, char* pszIP)
{
	if(pszID == NULL || pszPW == NULL || pszIP == NULL)		return -1;

	int				bRet = -1; // 실패 
	HRESULT			hr;
	TCHAR			szMethodName[] = "GameLogin";
	LPOLESTR		fszMemberFunc = new OLECHAR[strlen(szMethodName) + 1];

	_bstr_t			bstrWSDLFile = (_T("https://kunshu.postar.jp/api/channeling.asmx?wsdl"));

	DISPPARAMS		dispparams;
	EXCEPINFO		excepinfo;
	VARIANT			returnval;
	unsigned int		uArgErr = 0;

	CComPtr<ISoapClient>	pSoapClient;

	if(fszMemberFunc)		ZeroMemory(fszMemberFunc, sizeof(OLECHAR) * (strlen(szMethodName) + 1));
	::MultiByteToWideChar(CP_ACP, 0, szMethodName, (int)strlen(szMethodName), fszMemberFunc, (int)strlen(szMethodName));

	dispparams.rgvarg = NULL;

	hr = pSoapClient.CoCreateInstance(__uuidof(SoapClient30));
	if( hr == S_OK )
	{
		_bstr_t			empty(_T(""));
		DISPID			dispid;

		hr = pSoapClient->MSSoapInit(bstrWSDLFile, empty, empty, empty);		
		hr = pSoapClient->GetIDsOfNames(IID_NULL, &fszMemberFunc, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

		BSTR		bstrParam_ID;
		BSTR		bstrParam_PW;
		BSTR		bstrParam_IP;
		BSTR		bstrParam_ERRNO;
		BSTR		bstrParam_ERRMSG;

		OLECHAR		oleBuf[1024];

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszID, (int)strlen(pszID), oleBuf, (int)strlen(pszID));
		bstrParam_ID = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszPW, (int)strlen(pszPW), oleBuf, (int)strlen(pszPW));
		bstrParam_PW = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszIP, (int)strlen(pszIP), oleBuf, (int)strlen(pszIP));
		bstrParam_IP = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		bstrParam_ERRNO = SysAllocStringLen(oleBuf, 1024);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		bstrParam_ERRMSG = SysAllocStringLen(oleBuf, 1024);

		dispparams.rgvarg				= new VARIANTARG[5];
		dispparams.cArgs				= 5;
		dispparams.cNamedArgs			= 0;
		dispparams.rgdispidNamedArgs	= NULL;

		dispparams.rgvarg[0].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[0].pbstrVal	= &bstrParam_ERRMSG;

		dispparams.rgvarg[1].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[1].pbstrVal	= &bstrParam_ERRNO;

		dispparams.rgvarg[2].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[2].pbstrVal	= &bstrParam_IP;

		dispparams.rgvarg[3].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[3].pbstrVal	= &bstrParam_PW;

		dispparams.rgvarg[4].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[4].pbstrVal	= &bstrParam_ID;

		VariantInit(&returnval);

		excepinfo.wCode					= 0;
		excepinfo.bstrSource			= NULL;
		excepinfo.bstrDescription		= NULL;
		excepinfo.bstrHelpFile			= NULL;

		hr = pSoapClient->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &returnval, &excepinfo, &uArgErr);

		if( hr == S_OK )
		{
			if( returnval.vt == VT_BOOL )
			{
				if( returnval.boolVal == VARIANT_TRUE )
				{
					// 인증 성공
					if( wcscmp(bstrParam_ERRNO, L"0") == 0 )
					{
						bRet = 0; //성공 
					}
				}
				else
				{
					// 인증 실패
					if( wcscmp(bstrParam_ERRNO, L"1") == 0 )
					{
						bRet = 1;  					
					}
					else if( wcscmp(bstrParam_ERRNO, L"100") == 0 )
					{
						bRet = 100;  
					}
					else if( wcscmp(bstrParam_ERRNO, L"254") == 0 )
					{
						bRet = 254;  					
					}
					else if( wcscmp(bstrParam_ERRNO, L"255") == 0 )
					{
						bRet = 255;  					
					}
					else
					{
						bRet = -1;
					}
					
				}
			}
		}

		SysFreeString(bstrParam_ID);
		SysFreeString(bstrParam_PW);
		SysFreeString(bstrParam_IP);
		SysFreeString(bstrParam_ERRNO);
		SysFreeString(bstrParam_ERRMSG);
	}

	if(pSoapClient)				pSoapClient.Release();
	if(dispparams.rgvarg)		delete[] dispparams.rgvarg;

	return bRet;
}

bool GamePotReport(int nCount)
{
	bool			bRet = false;
	HRESULT			hr;
	TCHAR			szMethodName[] = "GameReport";
	LPOLESTR		fszMemberFunc = new OLECHAR[strlen(szMethodName) + 1];

	_bstr_t			bstrWSDLFile = (_T("https://kunshu.postar.jp/api/channeling.asmx?wsdl"));

	DISPPARAMS		dispparams;
	EXCEPINFO		excepinfo;
	VARIANT			returnval;
	unsigned int	uArgErr = 0;

	CComPtr<ISoapClient>	pSoapClient;

	if(fszMemberFunc)		ZeroMemory(fszMemberFunc, sizeof(OLECHAR) * (strlen(szMethodName) + 1));
	::MultiByteToWideChar(CP_ACP, 0, szMethodName, (int)strlen(szMethodName), fszMemberFunc, (int)strlen(szMethodName));

	dispparams.rgvarg = NULL;

	hr = pSoapClient.CoCreateInstance(__uuidof(SoapClient30));
	if( hr == S_OK )
	{
		_bstr_t			empty(_T(""));
		DISPID			dispid;

		hr = pSoapClient->MSSoapInit(bstrWSDLFile, empty, empty, empty);		
		hr = pSoapClient->GetIDsOfNames(IID_NULL, &fszMemberFunc, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

		dispparams.rgvarg				= new VARIANTARG[2];
		dispparams.cArgs				= 2;
		dispparams.cNamedArgs			= 0;
		dispparams.rgdispidNamedArgs	= NULL;

		dispparams.rgvarg[0].vt			= VT_DATE;
		SYSTEMTIME	curTime;
		GetLocalTime(&curTime);
		SystemTimeToVariantTime(&curTime, &dispparams.rgvarg[0].date);

		dispparams.rgvarg[1].vt			= VT_INT;
		dispparams.rgvarg[1].intVal		= nCount;

		VariantInit(&returnval);

		excepinfo.wCode					= 0;
		excepinfo.bstrSource			= NULL;
		excepinfo.bstrDescription		= NULL;
		excepinfo.bstrHelpFile			= NULL;

		hr = pSoapClient->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &returnval, &excepinfo, &uArgErr);

		if( hr == S_OK )
		{
			if( returnval.vt == VT_BOOL )
			{
				if( returnval.boolVal == VARIANT_TRUE )
				{
					bRet = true;
				}
				else
				{
				}
			}
		}
	}

	if(pSoapClient)				pSoapClient.Release();
	if(dispparams.rgvarg)		delete[] dispparams.rgvarg;

	return bRet;
}
*/

// 주민등록번호 (SSN)을 유저가 입력한 나이와 성별을 이용해서 생성한다.
// siAge     [input]  : 나이
// siSex     [input]  : 0(남자), 1(여자)
// pszRet    [output] : 주민번호
// uiRetSize [input]  : pszRet의 길이
bool SSNCreate(SI08 siAge, SI08 siSex, TCHAR* pszRet, UI32 uiRetSize)
{
	SI16 siBirthYear;
	TCHAR szSSN[16] = {'\0',};
	TCHAR szBirthYear[8] = { '\0',};

	if(pszRet == NULL)			return false;

	// 현재의 년도를 구한다. (일단 테스트를 위해서 2008을 임의로 집어넣는다.)
	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	SI16 siCurrentYear = stTime.wYear;

	// 출생년도를 구하기 위해서 현재 년도에서 자기나이를 뺀후에 1을 더한다.
	// 한국의 경우 태어나자마자 1살로 취급하기 때문에 마지막에 1을 더해야 한다.
	siBirthYear = siCurrentYear - siAge + 1;

	// 출생년도를 문자열로 변환한다.
	StringCchPrintf(szBirthYear, sizeof(szBirthYear), "%d", siBirthYear);

	// 출생년도의 뒤의 2자리를 주민등록번호에 집어넣는다.
	_tcsncpy(szSSN, &szBirthYear[2], 2);

	// 나머지 월, 일은 '0101'로 채워넣는다.
	_tcscat(szSSN, TEXT("0101"));

	// 뒤의 첫번째 자리는 성별구분으로 채워넣는다.
	if(siBirthYear >= 1900 && siBirthYear <= 1999)	// 1900년대생 이면
	{
		if	   (siSex == 0)		   {	_tcscat(szSSN, TEXT("1"));	}		// 남자면 1
		else if(siSex == 1)		   {	_tcscat(szSSN, TEXT("2"));	}		// 여자면 2
	}
	else if(siBirthYear >= 2000)	// 2000년대생 이면
	{
		if	   (siSex == 0)		   {	_tcscat(szSSN, TEXT("3"));	}		// 남자면 3
		else if(siSex == 1)		   {	_tcscat(szSSN, TEXT("4"));	}		// 여자면 4
	}
	else if(siBirthYear <= 1899)	// 1800년대 이전생 이면
	{
		if	   (siSex == 0)		   {	_tcscat(szSSN, TEXT("9"));	}		// 남자면 9
		else if(siSex == 1)		   {	_tcscat(szSSN, TEXT("0"));	}		// 여자면 0
	}

	// 나머지 6자리는 '000000'로 채워넣는다.
	_tcscat(szSSN, TEXT("000000"));

	_tcsncpy(pszRet, szSSN, uiRetSize);

	return true;
}

// 일본 넥슨 용 파싱 함수.
void NexonjpIDDataDivision(sRequest_NEXONJP_FrontAuth *FfrontAuth , sDBRequest_IDNumGet *FsendMsg)
{
	char temTotalChar[512] = { '\0', };

	char	szDivider1[] = {'&'};
	char	szDivider2[] = {'='};

	char temuserid[50] = { '\0', };
	char temrequestTime[60] = { '\0', };
	char temmkey1[50] = { '\0', };
	char temmkey2[50] = { '\0', };
	char temmkey[262] = { '\0', };

	char tem2userid[40] = { '\0', };
	char tem2requestTime[24] = { '\0', };
	char tem2mkey1[37] = { '\0', };
	char tem2mkey2[37] = { '\0', };
	char tem2mkey[261] = { '\0', };

	char imsiChar[300] = { '\0', };
	
	memcpy(temTotalChar, FfrontAuth->userid , 512);

	//printf( " 0 -FfrontAuth->userid: %s\n", FfrontAuth->userid);
	//printf( " 0 -1 temTotalChar: %s\n", temTotalChar);


	// 1차 "&"단위로 끊는다. 
	HQDivideBy2(szDivider1,temTotalChar,"%s %s %s %s %s",temuserid,temrequestTime,temmkey1,temmkey2,temmkey);

	//printf( " 1- [%s] [%s] [%s] [%s] [%s] \n",temuserid,temrequestTime,temmkey1,temmkey2,temmkey);

	// 2차 각각 "=" 단위로 끊어서 저장.
	HQDivideBy2(szDivider2,temuserid,"%s %s",imsiChar,tem2userid);
	HQDivideBy2(szDivider2,temrequestTime,"%s %s",imsiChar,tem2requestTime);	
	HQDivideBy2(szDivider2,temmkey1,"%s %s",imsiChar,tem2mkey1);
	HQDivideBy2(szDivider2,temmkey2,"%s %s",imsiChar,tem2mkey2);
	HQDivideBy2(szDivider2,temmkey,"%s %s",imsiChar,tem2mkey);

	//printf( " 2- [%s] [%s] [%s] [%s] [%s] \n",tem2userid,tem2requestTime,tem2mkey1,tem2mkey2,tem2mkey);

	// 파싱된 값을 전달한다.
	memcpy(FsendMsg->userid,tem2userid,sizeof(tem2userid));
	memcpy(FsendMsg->requestTime,tem2requestTime,sizeof(tem2requestTime));
	memcpy(FsendMsg->mkey1,tem2mkey1,sizeof(tem2mkey1));
	memcpy(FsendMsg->mkey2,tem2mkey2,sizeof(tem2mkey2));
	memcpy(FsendMsg->key,tem2mkey,sizeof(tem2mkey));

	//printf( "3- [%s] [%s] [%s] [%s] [%s] \n",FsendMsg->userid,FsendMsg->requestTime,FsendMsg->mkey1,FsendMsg->mkey2,FsendMsg->key);
}
