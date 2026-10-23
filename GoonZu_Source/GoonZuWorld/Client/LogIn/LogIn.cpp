//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "..\Client.h"
#include "..\..\resource.h"
#include "..\Interface\LoginClient\LoginClient.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../NInterface/NLogin/NLogin.h"
#include "../../Apex/gamecode.h"

#include <Wininet.h>

#include "StmOCX.tlh"
#include <comutil.h>
#include <tchar.h>
#include <atlcomcli.h>
#include <stdio.h>

extern cltCommonLogic* pclClient;

// 아이디, 암호 입력 여부 확인. 
bool bIDPassswordInputSwitch	= false;
bool loginswitch				= false;
bool noticeswitch				= false;
bool worldlistswitch			= false;


extern SI16 g_siChannelingCode;
extern char g_nm_id[ 1024 ];
extern char g_nm_pw[ 1024 ];

// WEB LOGIN SERVICE
extern char g_szWeb_id[ 1024 ];
extern char g_szWeb_pw[ 1024 ];
extern bool g_bWebLoginService;

extern bool g_bHanAuth;
extern bool g_bUSA_Auth;
extern bool g_bWebLoginService;
extern DWORD dwQuitTimer;

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
extern bool g_bPorteAuth;
#endif

extern bool g_bIovationAuth;

// PCK : IOVATION 인증 - blackbox
typedef HRESULT (*RegProto)();

// 클라이언트로 하여금 FrontServer에 접속하게 만든다. 
bool cltClient::InitConnectToFrontServer()
{

	//-------------------------------
	// 로그인 대화상자를 준비한다. 
	//-------------------------------
	bLoginRequest = FALSE;

	((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show();

	// 아이디와 암호가 입력 되지 않았다. 
	bIDPassswordInputSwitch = false;

	TCHAR buffer[256] = TEXT("") ;
	TCHAR* pText = GetTxtFromMgr(1036);
	
	StringCchPrintf(buffer, 256, pText);

	((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show( buffer );

	return true;
}

// 클라이언트인 경우 서버와 접속을 시도한다. 
// 1: 게임 시작. 
// -1:종료 
// 0: 대기 

SI32 cltClient::ConnectToFrontServer()
{
	// 클라이언트인 경우에만 처리한다.(맵에디터에서는 사용하지 않는다. ) 
	if(GameMode != GAMEMODE_CLIENT) return 1;
	SI32 returnval = 0;
	
	if( bLoginRequest == FALSE && g_siChannelingCode > 0 ) 
	{
		returnval = LOGIN_OK;
	} 
	else if  (bLoginRequest == FALSE && g_bHanAuth)
	{
		returnval = LOGIN_OK;
	}
#ifdef _USA_NEW_AUTH_
	else if( bLoginRequest == FALSE && g_bUSA_Auth )
	{
		returnval = LOGIN_OK;
	}
#endif
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	else if  (bLoginRequest == FALSE && g_bPorteAuth)
	{
		returnval = LOGIN_OK;
	}
#endif // USE_PORTEAUTH
	else if (bLoginRequest == FALSE && g_bWebLoginService)
	{
		returnval = LOGIN_OK;
	}
	// 자동 로그인 - 기존에 아이디와 패스워드를 입력한 적이 있다면, 자동으로 로그인 된다.
	else if ( bLoginRequest == FALSE && _tcslen(szID) > 0 && _tcslen(szPassword) > 0 )
	{
		returnval = LOGIN_OK;
	}
	else 
	{
		returnval = ((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->GetStatus();
	}

	switch(returnval)
	{
	case LOGIN_READY:
		break;

	case LOGIN_OK:
		{
			if( bLoginRequest == TRUE && GetTickCount() - dwLastLoginTick < 10000 ) {

				TCHAR buf[256];

				TCHAR* pText = GetTxtFromMgr(1037);
				

				StringCchPrintf( buf, 256, pText, 
					( GetTickCount() - dwLastLoginTick ) / 1000 );


				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show( buf );

				
				return 0;
			}

			bLoginRequest = TRUE;
			dwLastLoginTick = GetTickCount();

			if( clUserState.GetState() == USERSTATE_FRONT_CONNECTING ||
				clUserState.GetState() == USERSTATE_FRONT_CONNECTED )  {

				pFrontSession->CloseSocket();

				Sleep( 200 );
			}

			stLoginData sdata;
			
			((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->GetLoginData(&sdata);
			
			// 넷마블 사용자이면,
			if( g_siChannelingCode == CHANNELINGCODE_NETMARBLEJP )
			{	
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "NetmarbleJP" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
			//---------------------------------------------------------------------------
			//KHY - 0617 - 일본 채널링 추가.
			//---------------------------------------------------------------------------
#ifdef _JAPAN			
			// 한게임 사용자이면,
			if( g_siChannelingCode == CHANNELINGCODE_HANGAMEJP )
			{	
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "HangameJP" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
			// 감마니아사용자이면,
			if( g_siChannelingCode == CHANNELINGCODE_GAMANIAJP )
			{	
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "GamaniaJP" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
			// 게임팟 사용자이면,
			if( g_siChannelingCode == CHANNELINGCODE_GAMEPOTJP )
			{	
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "GamePotJP" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
			// 넥슨 사용자이면,
			if( g_siChannelingCode == CHANNELINGCODE_NEXONJP )
			{	
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "NexonJP" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
			// NDE 사용자이면,
			if( g_siChannelingCode == 0 )
			{	
//				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "NdeJP" );
//				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();
//
//				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
//				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
#endif
			//---------------------------------------------------------------------------

			// [진성] USA 새로운 인증.
			if( g_bUSA_Auth )
			{
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "USAHangame" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}

			// NHN - HanAuth 사용
			if (g_bHanAuth)
			{
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "HanGame" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}

			//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
			if (g_bPorteAuth)
			{
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "PorteHanGame" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}
#endif // USE_PORTEAUTH

			// 웹로그인 사용
			if (g_bWebLoginService)
			{
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->SetText( "WebLogin" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[0]->Refresh();

				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->SetText( "password" );
				((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->m_pEdit[1]->Refresh();
			}

			// 아이디를 얻어온다. 
			MStrCpy( szID, sdata.m_strUserID, MAX_PLAYER_NAME);
			
			// 암호를 얻어온다.
			MStrCpy( szPassword, sdata.m_strUserPassword, MAX_PLAYER_NAME );

			// 아이디와 암호가 입력 되었다. 
			bIDPassswordInputSwitch = true;
			loginswitch				= false;
			noticeswitch			= false;
			worldlistswitch			= false;

			// 연결 상태로 변경한다. 
			clUserState.SetState(USERSTATE_FRONT_CONNECTING);
//#ifdef _DEBUG
			pclClient->pclLog->FilePrint("Config\\ErrorLog.log"," clUserState.SetState(USERSTATE_FRONT_CONNECTING) ");
//#endif

		}
		return 0;

	case LOGIN_CANCEL:
		return -1;		// 게임을 종료한다. 
		break;

	case LOGIN_ID_REGIST:
		if ( ! ((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->IsShow() )
		{
			TCHAR* pText = GetTxtFromMgr(1038);

			((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show(pText);
		}

		break;
	}

	// 아이디와 암호가 아직 입력되지 않았다면 , 
	if(bIDPassswordInputSwitch == false)return 0;

	//-----------------------------------------------------
	// FrontServer로 아이디와 암호를 보내서 로그인 시킨다. 
	//-----------------------------------------------------
	if(loginswitch == false && clUserState.GetState() == USERSTATE_FRONT_CONNECTED)
	{
		// 암호화키를 받기 전에는 프론트 서버로 메시지를 보내서는 안된다.!!!!!!!!!
		if(bFrontServerSecretKeySwitch == true)
		{
			loginswitch = true;
			
			// 넷마블 사용자일 경우
			if( g_siChannelingCode == CHANNELINGCODE_NETMARBLEJP ) 
			{

				sRequest_NetmarbleJP_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_NETMARBLEJP_LOGIN;

				MStrCpy( sendmsg.userid, g_nm_id, 100 );
				MStrCpy( sendmsg.userpw, g_nm_pw, 100 );

				GetMacAddress( sendmsg.macaddress , 20);
//#ifdef _DEBUG
				pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "sRequest_NetmarbleJP_LogIn  %s %s %s" ,
						sendmsg.userid , sendmsg.userpw , sendmsg.macaddress );
//#endif

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);
			} 			
			//---------------------------------------------------------
			//KHY - 0617 - 일본 채널링 추가.
			//---------------------------------------------------------
#ifdef _JAPAN
			// 한게임.
			else if( g_siChannelingCode == CHANNELINGCODE_HANGAMEJP ) 
			{
				if(m_siHanAuth_TryCount > 1)
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);	TCHAR* pText  = GetTxtFromMgr(7503);
					MsgBox( pTitle, pText);					dwQuitTimer = 1;
				}
				else
				{
					sRequest_HANGAMEJP_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_HANGAMEJP_LOGIN;

					MStrCpy( sendmsg.userid, g_nm_id, sizeof(sendmsg.userid));
					MStrCpy( sendmsg.userpw, g_nm_pw, sizeof(sendmsg.userpw) );

					MStrCpy( szID, g_nm_id , MAX_PLAYER_NAME );
					MStrCpy( szPassword, g_nm_pw , MAX_PLAYER_NAME );

					GetMacAddress( sendmsg.macaddress , 20);

					pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "Hangame Request Login  %s %s" ,
						sendmsg.userid , sendmsg.macaddress );

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);
				}
			} 
			// 감마니아
			else if( g_siChannelingCode == CHANNELINGCODE_GAMANIAJP ) 
			{
				if(m_siHanAuth_TryCount > 1)
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);	TCHAR* pText  = GetTxtFromMgr(7503);
					MsgBox( pTitle, pText);					dwQuitTimer = 1;
				}
				else
				{
					sRequest_GAMANIAJP_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_GAMANIAJP_LOGIN;

					MStrCpy( sendmsg.userid, g_nm_id, sizeof(sendmsg.userid));
					MStrCpy( sendmsg.userpw, g_nm_pw, sizeof(sendmsg.userpw) );

					MStrCpy( szID, g_nm_id , MAX_PLAYER_NAME );
					MStrCpy( szPassword, g_nm_pw , MAX_PLAYER_NAME );

					GetMacAddress( sendmsg.macaddress , 20);

					pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "Gamania Request Login  %s %s" ,
						sendmsg.userid , sendmsg.macaddress );

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);
				}
			} 
			// 게임팟
			else if( g_siChannelingCode == CHANNELINGCODE_GAMEPOTJP ) 
			{
				if(m_siHanAuth_TryCount > 1)
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);	TCHAR* pText  = GetTxtFromMgr(7503);
					MsgBox( pTitle, pText);					dwQuitTimer = 1;
				}
				else
				{
					sRequest_GAMEPOTJP_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_GAMEPOTJP_LOGIN;

					MStrCpy( sendmsg.userid, g_nm_id,  sizeof(sendmsg.userid) );
					MStrCpy( sendmsg.userpw, g_nm_pw, sizeof(sendmsg.userpw) );

					MStrCpy( szID, g_nm_id , MAX_PLAYER_NAME );
					MStrCpy( szPassword, g_nm_pw , MAX_PLAYER_NAME );

					GetMacAddress( sendmsg.macaddress , 20);

					pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "Gamepot Request Login  %s %s" ,
						sendmsg.userid ,  sendmsg.macaddress );

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);
				}
			} 
			// 넥슨
			else if( g_siChannelingCode == CHANNELINGCODE_NEXONJP ) 
			{
				if(m_siHanAuth_TryCount > 1)
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);	TCHAR* pText  = GetTxtFromMgr(7503);
					MsgBox( pTitle, pText);					dwQuitTimer = 1;
				}
				else
				{
					sRequest_NEXONJP_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_NEXONJP_LOGIN;

					MStrCpy( sendmsg.userid, g_nm_id,  512); // 넥슨은 ID안에 기타정보가 모두 들어온다.
					MStrCpy( sendmsg.userpw, g_nm_pw, sizeof(sendmsg.userpw) );

					// ID는 나중에 AC에서 돌아와야 알 수 있다.
					//MStrCpy( szID, g_nm_id , MAX_PLAYER_NAME );
					//MStrCpy( szPassword, g_nm_pw , MAX_PLAYER_NAME );

					GetMacAddress( sendmsg.macaddress , 20);

					pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "Nexon Request Login  %s %s" ,
						sendmsg.userid ,  sendmsg.macaddress );

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);
				}
			} 
#endif
			//---------------------------------------------------------
			else if ( g_bWebLoginService )
			{
#ifdef _TAIWAN
				sRequest_Taiwan_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_TAIWAN_LOGIN;

				MStrCpy( sendmsg.szGameAccount,		g_szWeb_id, sizeof(sendmsg.szGameAccount) );
				MStrCpy( sendmsg.szOneTimePassword, g_szWeb_pw, sizeof(sendmsg.szOneTimePassword) );

				GetMacAddress( sendmsg.macaddress , 20);

//#ifdef _DEBUG
				pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "sRequest_Taiwan_LogIn  %s %s" ,
					sendmsg.szGameAccount , sendmsg.szOneTimePassword);
//#endif

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);
#endif
			}
#ifdef _USA_NEW_AUTH_
			// [진성]  USA 새로운 인증.
			else if ( g_bUSA_Auth )
			{
				//MessageBox(NULL, "USA 인증하려고 여기에 왔다.", "USA 인증", 0 );
								
				sRequest_USA_LogIn sendMsg;

				sendMsg.packetHeader.usSize	= sizeof(sendMsg);
				sendMsg.packetHeader.usCmd	= FRONTMSG_REQUEST_USA_LOGIN;

				memcpy( sendMsg.authstring, m_szUSAAuth_String, sizeof(sendMsg.authstring) );

				pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "sRequest_USA_LogIn  %s", sendMsg.authstring );
							
				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendMsg);

				//MessageBox(NULL, sendMsg.authstring, "USA 인증", 0 );
			}
#endif

#ifdef USE_HANAUTH
			else if ( g_bHanAuth )
			{
				if(m_siHanAuth_TryCount > 3)
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);
					TCHAR* pText  = GetTxtFromMgr(7503);

					MsgBox( pTitle, pText);
					dwQuitTimer = 1;
				}
				else
				{
					sRequest_HanAuth_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_HANAUTH_LOGIN;

					// 인증스트링 얻기
					SI32 siHanAuthRet = GetAuthString(m_szHanAuth_AuthString, SIZE_AUTHSTRING);
					if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

					if(strlen(m_szHanAuth_GameString) <= 0)
					{
						// 인증 실패 클라이언트 강제 종료
						TCHAR* pTitle = GetTxtFromMgr(3168);
						TCHAR* pText  = GetTxtFromMgr(7503);

						MsgBox( pTitle, pText);
						dwQuitTimer = 1;
					}

					MStrCpy( sendmsg.authstring, m_szHanAuth_AuthString, SIZE_AUTHSTRING );
					MStrCpy( sendmsg.userid,	 m_szHanAuth_UserID,	MAX_PLAYER_NAME );
					sendmsg.userpw[0] = '\0';

					GetMacAddress( sendmsg.macaddress , 20);

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);

	//				WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Send sRequest_HanAuth_LogIn");
				}
			}
#endif // USE_HANAUTH

			//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
#ifndef _SERVER //서버가 정의되어 있지 않다면.
			else if ( g_bPorteAuth )
			{
				if(m_siPorteAuth_TryCount > 3) // 3번 시도.
				{
					// 인증 실패 클라이언트 강제 종료
					TCHAR* pTitle = GetTxtFromMgr(3168);
					TCHAR* pText  = GetTxtFromMgr(7503);

					MsgBox( pTitle, pText);
					dwQuitTimer = 1;
				}
				else
				{
					sRequest_PorteAuth_LogIn sendmsg;

					sendmsg.packetHeader.usSize	= sizeof(sendmsg);
					sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_PORTEAUTH_LOGIN;

					// ID 받아오기.
					SI32 siPorteAuthRet = PubGetUserID(m_szPorteAuth_UserID, sizeof(m_szPorteAuth_UserID));
					if(siPorteAuthRet != 0)		{		m_szPorteAuth_UserID[0] = '\0';		}

					// Auth 티켓 받아 오기.
					siPorteAuthRet = PubGetAuthTicket(m_szPorteAuth_szAuthTICKET, sizeof(m_szPorteAuth_szAuthTICKET));
					if(siPorteAuthRet != 0)		{		m_szPorteAuth_szAuthTICKET[0] = '\0';		}

					if(strlen(m_szPorteAuth_szAuthTICKET) <= 0)
					{
						// 인증 실패 클라이언트 강제 종료
						TCHAR* pTitle = GetTxtFromMgr(3168);
						TCHAR* pText  = GetTxtFromMgr(7503);

						MsgBox( pTitle, pText);
						dwQuitTimer = 1;
					}

					MStrCpy( sendmsg.authstring, m_szPorteAuth_szAuthTICKET, sizeof(m_szPorteAuth_szAuthTICKET) );
					MStrCpy( sendmsg.userid,	 m_szPorteAuth_UserID,	MAX_PLAYER_NAME );
					sendmsg.userpw[0] = '\0';

					MStrCpy( szID, m_szPorteAuth_UserID , MAX_PLAYER_NAME );
					GetMacAddress( sendmsg.macaddress , 20);

					// 프런트 서버로 메시지를 보낸다. 
					SendFrontServerMsg((sPacketHeader*)&sendmsg);

					//				WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Send sRequest_HanAuth_LogIn");
				}
			}
#endif
#endif // USE_PORTEAUTH

#ifdef _IOVATION
			// PCK : IOVATION인증을 위한 새로운 패킷 작성 (2009.02.03)
			else if( g_bIovationAuth )
			{
				sRequest_English_LogIn sendmsg;

				// BlackBox 얻기
				Registeriovation();

				HRESULT hr;
				CComPtr<IStm>	clIStm;

				char	szBlackBox[2048] = { '\0', };

				hr = clIStm.CoCreateInstance( CLSID_Stm );
				if( SUCCEEDED(hr) )
				{
					//BSTR	bstrVersion;
					//bstrVersion = clIStm->ioVersion();

					BSTR	bstrBlackbox;
					bstrBlackbox = clIStm->ioBegin();

					int len = ::WideCharToMultiByte(CP_ACP, 0, bstrBlackbox, wcslen(bstrBlackbox), szBlackBox, 2048, NULL, NULL);
					printf("BlackBox String -----\n%s\n", szBlackBox);

					if(clIStm)		clIStm.Release();
				}

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_ENGLISH_LOGIN;

				MStrCpy(sendmsg.userid, szID,		MAX_PLAYER_NAME );
				MStrCpy(sendmsg.userpw, szPassword, MAX_PLAYER_NAME );
				
				GetMacAddress( sendmsg.macaddress , 20);
				
				MStrCpy(sendmsg.Blackbox, szBlackBox, 2048);

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);
			}
#endif
			else
			{
#ifdef _CHINA // dhm-
				sRequest_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_LOGIN;

				m_pClientDiffHellMan->encrypt_data_with_PrivateKey_Key1( (const char*)szID, Net_Packet_Max_Size );
				memcpy(sendmsg.userid, m_pClientDiffHellMan->m_client_buf,	Net_Packet_Max_Size );
				m_pClientDiffHellMan->encrypt_data_with_PrivateKey_Key1( (const char*)szPassword, Net_Packet_Max_Size );
				memcpy(sendmsg.userpw, m_pClientDiffHellMan->m_client_buf, Net_Packet_Max_Size );

				GetMacAddress( sendmsg.macaddress , 20);

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);

#elif _JAPAN
				//---------------------------------------------------------
				//KHY - 0617 - 일본 채널링 추가.

				// NDE
				sRequest_NDEJP_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_NDEJP_LOGIN;

	#ifndef _DEBUG // 디버그 모드가 아닐때만.
				//  NDE 의 경우, 클라이언트 로그인이 때문에 수동으로 붙여준다. 웹로그인시, 웹에 요청후, 삭제필요. 
				strcat( szID, "@ND"); 
	#endif				
				MStrCpy(sendmsg.userid, szID,		MAX_PLAYER_NAME );
				MStrCpy(sendmsg.userpw, szPassword, MAX_PLAYER_NAME );

				GetMacAddress( sendmsg.macaddress , 20);

				pclClient->pclLog->FilePrint("Config\\ErrorLog.log", "NDE Request Login  %s %s" ,
									sendmsg.userid ,  sendmsg.macaddress );
				
				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);

#elif _EUROPE
				sRequest_EUROPE_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_EUROPE_LOGIN;

				MStrCpy(sendmsg.userid, szID,		MAX_PLAYER_NAME );
				MStrCpy(sendmsg.userpw, szPassword, MAX_PLAYER_NAME );

				GetMacAddress( sendmsg.macaddress , 20);

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);

#elif _KOREA	//////////////////////////////////////////////////
				//	[종호_NHN->NDOORS] 이관 관련 군주S 플로어 추가
				sRequest_LogIn sendmsg;

				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_KOREA_LOGIN;

				MStrCpy(sendmsg.userid, szID,		MAX_PLAYER_NAME );
				MStrCpy(sendmsg.userpw, szPassword, MAX_PLAYER_NAME );

				GetMacAddress( sendmsg.macaddress , 20);

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);
#else	// _KOREA

				//---------------------------------------------------------
				sRequest_LogIn sendmsg;

//#ifdef _ENGLISH
//#ifndef _DEBUG	//	디버그 모드가 아닐때만
//				// 글로벌의 경우 클라이언트 로그인이기 때문에 수동으로 뒤에 "@GB"를 붙여준다. (09.03.18)
//				strcat( szID, "@GB"); 
//#endif
//#endif
				sendmsg.packetHeader.usSize	= sizeof(sendmsg);
				sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_LOGIN;

				MStrCpy(sendmsg.userid, szID,		MAX_PLAYER_NAME );
				MStrCpy(sendmsg.userpw, szPassword, MAX_PLAYER_NAME );

				GetMacAddress( sendmsg.macaddress , 20);

				// 프런트 서버로 메시지를 보낸다. 
				SendFrontServerMsg((sPacketHeader*)&sendmsg);
#endif

			}

			TCHAR* pText = GetTxtFromMgr(1039);

			((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show( pText );

		}

	}

	//--------------------------------------------------
	// FrontServer로 공지사항을 요청한다. 
	//--------------------------------------------------
	if(noticeswitch == false && clUserState.GetState() == USERSTATE_FRONT_AUTHOK)
	{
		noticeswitch = true;

		sRequest_Notice sendmsg;

		sendmsg.packetHeader.usSize	= sizeof(sendmsg);
		sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_NOTICE;

		SendFrontServerMsg((sPacketHeader*)&sendmsg);

		TCHAR* pText = GetTxtFromMgr(1040);

		((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Show( pText );


	}
	//--------------------------------------------------
	// FrontServer로 서버 목록을 요청한다.
	//--------------------------------------------------
	if(worldlistswitch == false && clUserState.GetState() == USERSTATE_FRONT_NOTICEOK)
	{
		worldlistswitch = true;

		sRequest_WorldList sendmsg;

		sendmsg.packetHeader.usSize	= sizeof(sendmsg);
		sendmsg.packetHeader.usCmd	= FRONTMSG_REQUEST_WORLDLIST;
		
		StringCchCopy( sendmsg.szAccountID, sizeof(sendmsg.szAccountID), szID );

		SendFrontServerMsg((sPacketHeader*)&sendmsg);

		// 로그인 대화상자를 숨긴다. 

		((CNLoginDlg*)m_pDialog[ NLOGIN_DLG ])->Hide();

		return 1;
	}

	return 0;
}

// 프론트 서버에 군주 DB사용자를 웹DB로 이전하라고 요청한다. 
void cltClient::RequestJoinGame()
{
	sRequest_JoinGame clMsg;
				
	clMsg.packetHeader.usSize	= sizeof(clMsg);
	clMsg.packetHeader.usCmd	= FRONTMSG_REQUEST_JOINGAME;
	
	DWORD Size = 256;
	TCHAR Buffer[ 256 ];
	
	if ( InternetGetCookie( TEXT("http://kunshu.jp"), TEXT("gzbanner"), Buffer, &Size ) == FALSE )
	{
		StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
	}
	else
	{
		/*
		if ( Size > 8 )
		{	
			if ( _tcsncmp( &Buffer[0], TEXT("gzbanner"), 8 ) == 0 || _tcsncmp( &Buffer[0], TEXT("GZBANNER"), 8 ) == 0 )
			{
				memcpy( clMsg.cookie, &Buffer[9], 20 );
				clMsg.cookie[ 19 ] = NULL;
			}
			else
			{
				_tcscpy( clMsg.cookie, TEXT("GAME") );
			}
		}
		else
		{
			_tcscpy( clMsg.cookie, TEXT("GAME") );
		}
		*/
		if ( Size > 8 && Size < 255 )
		{
			Buffer[ Size ] = NULL;

			TCHAR *pDest = _tcsstr( Buffer ,TEXT("gzbanner") );
			if ( pDest == NULL )
			{
				StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
			}
			else
			{
				SI16 Counter = 0;
				SI16 EqualPos = 0;
				bool bEqual = false;
				bool bFoundSeminicolon = false;
				
				while( pDest[ Counter ] != NULL )
				{
					if ( pDest[ Counter ] == TEXT('=') )
					{
						if ( bEqual == true )
						{
							bEqual = false;
							EqualPos = 0;
							break;
						}

						bEqual = true;
						EqualPos = Counter;
					}
					else if ( pDest[ Counter ] == TEXT(';') )
					{
						bFoundSeminicolon = true;
						break;
					}
					++Counter;
				}

				if ( bEqual == false )
				{
					StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
				}
				else
				{
					if ( bFoundSeminicolon )
					{
						if ( Counter - EqualPos < 2 )
						{
							StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
						}
						else
						{
							if ( Counter - EqualPos - 1 >= 20 )
							{
								StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
							}
							else
							{
								memcpy( clMsg.cookie, &pDest[9], Counter - EqualPos - 1 );
								clMsg.cookie[ Counter - EqualPos - 1 ] = NULL;
							}

							
						}
					}
					else
					{
						if ( Counter - EqualPos < 2 )
						{
							StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
						}
						else
						{
							if ( Counter - EqualPos - 1 >= 20 )
							{
								StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
							}
							else
							{
								memcpy( clMsg.cookie, &pDest[9], Counter - EqualPos - 1 );
								clMsg.cookie[ Counter - EqualPos - 1 ] = NULL;
							}
						}
					}
				}
				
			}
		}
		else
		{
			StringCchCopy( clMsg.cookie, 20, TEXT("GAME") );
		}
		
	}
	
	// SBS 인 경우에는 암호를 전송한다.
	memcpy ( clMsg.userpw, szPassword, 20 );
	clMsg.userpw[ 19 ] = NULL;

	/*
	if ( RegistrationNumber > 0 )
	{
		TCHAR Buffer[256 ] = TEXT("");
		sprintf( Buffer, TEXT("%d"), RegistrationNumber );
		memcpy( clMsg.jumincode, Buffer, 7 );
		clMsg.jumincode[ 7 ] = NULL;
	}
	else
	{
		clMsg.jumincode[ 0 ] = NULL;
	}
	*/
				
	MStrCpy( clMsg.userid, szID,		MAX_PLAYER_NAME );
				
	// 프런트 서버로 메시지를 보낸다. 
	SendFrontServerMsg((sPacketHeader*)&clMsg);
}

// 클라이언트로 하여금 GameServer에 접속하게 만든다. 
bool cltClient::InitConnectToGameServer()
{
	// 클라이언트인 경우에만 처리한다.(맵에디터에서는 사용하지 않는다. ) 
	if(GameMode != GAMEMODE_CLIENT) return true;

	// 게임서버를 접속시도상태로 설정한다. 
	// 이렇게 하면 프론트 서버와의 접속이 끊기게 된다. 
	clUserState.SetState(USERSTATE_GAME_CONNECTING);

	// 초기게임서버 접속시에 미니맵을 보여줄지 안보여줄지를 결정한다 (상민 ,JAYCEE)
	bSmallMapSwitch = g_SoundOnOffDlg.m_bMiniMapShow;

	// 타이머 초기화
	//m_kTimer_WaitGameLogin.SetActive(true, GetTickCount());
	
	return true;
}

SI32 cltClient::ConnectToGameServer()
{
	UI08 uiGamemode = 0;

	// 클라이언트인 경우에만 처리한다.(맵에디터에서는 사용하지 않는다. ) 
	if(GameMode != GAMEMODE_CLIENT) return 1;

	// 사용자의 CharUnique값이 아직 설정되지 않았다면 기다린다. 
	if(MyCharUnique == 0)return 0;
	// 암호화키를 아직 받지 못했다면 기다린다. 
	if(bGameServerSecretKeySwitch	== false)return 0;

	// GameServer의 인증을 통과했다.
	clUserState.SetState(USERSTATE_GAME_AUTHOK);

	if( !g_SoundOnOffDlg.m_bFullScreenMode )
	{
		uiGamemode = 0;	// 윈도우 모드
	} 
	else
	{
		// if( 윈도창 + 새인터페이스 모드 )
		uiGamemode = 1;	// 윈도창 + 새인터페이스 모드
		// else // 전체화면 + 새인터페이스 모드
		// uiGameMode = 2;
	}


	//----------------------------------------
	// 접속이 성공하면 GAMEMSG_REQUEST_LOGIN 
	// 메시지를 보내서 로그인 승인을 요청한다.
	//----------------------------------------

	{
		if(g_bHanAuth)		
		{
#ifdef USE_HANAUTH
			// 인증스트링 얻기
			SI32 siHanAuthRet = GetAuthString(m_szHanAuth_AuthString, SIZE_AUTHSTRING);
			if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

//			WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "Send cltLogIn_HanAuthInfo;siAuthKey = %d", siAuthKey);

			cltLogIn_HanAuthInfo cllogin(siAuthKey, m_szHanAuth_AuthString, m_szHanAuth_UserID, uiGamemode);
			cltMsg clMsg(GAMEMSG_REQUEST_HANAUTH_LOGIN, sizeof(cltLogIn_HanAuthInfo), (BYTE*)&cllogin);

			SendMsgToServer((sPacketHeader*)&clMsg);
#endif // USE_HANAUTH
		}
		else if(g_bWebLoginService)
		{
#ifdef _TAIWAN // -- 대만 로그인 --
			cltLogIn_TaiwanInfo cllogin( siAuthKey, g_szWeb_id, g_szWeb_pw, uiGamemode );
			cltMsg clMsg(GAMEMSG_REQUEST_TAIWAN_LOGIN, sizeof(cltLogIn_TaiwanInfo), (BYTE*)&cllogin);

			SendMsgToServer((sPacketHeader*)&clMsg);
#endif
		}
		else
		{
			cltLogInInfo cllogin( siAuthKey, szID, szPassword, uiGamemode );
			cltMsg clMsg(GAMEMSG_REQUEST_LOGIN, sizeof(cltLogInInfo), (BYTE*)&cllogin);

			SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}

	return 1;
}

bool cltClient::Registeriovation()
{
	bool bRet = false;
	HRESULT hr;

	// register the ActiveX dll
	HMODULE hDLL = LoadLibrary( "StmOCX.dll" );
	if ( hDLL )
	{
		RegProto regFn = (RegProto) GetProcAddress( hDLL, "DllRegisterServer" );
		if ( regFn )
		{
			hr = regFn();
			if( SUCCEEDED(hr) )		bRet = true;
		}

		FreeLibrary( hDLL );
	}

	return bRet;
}