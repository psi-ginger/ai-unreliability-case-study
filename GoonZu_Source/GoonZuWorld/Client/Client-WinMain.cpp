//---------------------------------
// 2003/10/2 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "Client.h"

#include "..\Common\Map\Map.h"
#include "..\resource.h"
#include "..\Server\Candidate\Candidate.h"
#include "Music\Music.h"
#include "../Client/DrawLogo/DrawLogo.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Client/Interface/LoginClient/LoginClient.h"
#include "../Client/StatusBar/ChatStatusBar/ChatStatusBar.h"
#include "../Client/NInterface/NLogin/NLogin.h"
#include "../Client/MiniHomeUpdates/PopupCheck.h"

#include "../common/Char/CharManager/CharManager.h"

extern bool bDrawSkipSwitch;
extern bool g_bHanAuth;
extern bool g_bUSA_Auth;
extern DWORD dwQuitTimer;

#ifdef USE_GAME_GUARD_CLIENT
	extern CNPGameLib * pNpgl;
#endif

// WInMain안에서의 클라이언트 기능. 
void cltClient::ClientWinMain()
{
	
	// 윈도우가 활성화 되어 있을때에만 효과음을 낸다. 
	//if(bActive == TRUE)
	{
		// 효과음을 연주한다. 
		PopEffect();

	}
	
	switch(GetGameStatus())
	{
		//---------------------------
		// Front Server와의 접속. 
		//---------------------------
	case GS_INITCONNECTFRONTSERVER:	GsInitConnectFrontServer();		break;
		
	case GS_CONNECTFRONTSERVER:		GsConnectFrontServer();			break;

		//---------------------------
		// 연령관련 화면 표시
		//---------------------------
	case GS_SHOWGOVERNMENT:			GsShowGovernment();				break;
		
		//--------------------------
		// World List 선택 
		//--------------------------
	case GS_INITSELECTWORLDLIST:	GsInitSelectWorldList();		break;
		
	case GS_SELECTWORLDLIST:		GsSelectWorldList();			break;
		
		//---------------------------
		// Game Server 와의 접속. 
		//---------------------------
	case GS_INITCONNECTGAMESERVER:	GsInitConnectGameServer();		break;
		
	case GS_CONNECTGAMESERVER:		GsConnectGameServer();			break;
		
		//--------------------------
		// Person List 선택.
		//--------------------------
	case GS_INITCHARLIST:			GsInitCharList();				break;
		
	case GS_CHARLIST:				GsCharList();					break;
		
		//--------------------------
		// 게임 
		//-------------------------
	case GS_GAME:					GsGame();						break;

	case GS_GAMEDESTROY:			DestroyWindow( GetHwnd() );		return;
	}

//	// USA 웹페이지 호출 모듈
//	if( IsCountrySwitch(Switch_PopupCheck) )
//	{
//		// 초기 한번 호출
//#ifdef USE_HANAUTH
//
//		if( m_kUpdateTimer_PopupCheck.IsActivated() == false &&
//			GetGameStatus() >= GS_INITSELECTWORLDLIST )
//		{
//			// 타이머 켜기
//			m_kUpdateTimer_PopupCheck.SetActive(true, ::GetTickCount());
//
//			TCHAR FullURL[100] = {'\0', };
//			if(m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
//			{
//				StringCchPrintf(FullURL, sizeof(FullURL),
//					"http://alpha-services.ijji.com/service/api/popupCheck?id=%s&gameid=U_GOONZU&type=0", m_szHanAuth_UserID);
//			}
//			else
//			{
//				StringCchPrintf(FullURL, sizeof(FullURL),
//					"http://services.ijji.com/service/api/popupCheck?id=%s&gameid=U_GOONZU&type=0", m_szHanAuth_UserID);
//			}
//
//			m_pPopupCheck->Request_PopupCheck(FullURL);
//		}
//#else
//		if( m_kUpdateTimer_PopupCheck.IsActivated() == false &&
//			_tcslen(pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID) > 0 )
//		{
//			// 타이머 켜기
//			m_kUpdateTimer_PopupCheck.SetActive(true, ::GetTickCount());
//
//			TCHAR FullURL[100] = {'\0', };
//			StringCchPrintf(FullURL, sizeof(FullURL),
//				"http://services.ijji.com/service/api/popupCheck?id=%s&gameid=U_GOONZU&type=0", pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID);
//
//			// 알파테스트.
//			StringCchPrintf( FullURL, sizeof(FullURL),
//				"http://alpha-services.ijji.com/service/api/popupCheck?id=searer1&gameid=U_GOONZU&type=0");
//
//			m_pPopupCheck->Request_PopupCheck(FullURL);
//		}
//
//#endif // USE_HANRTA
//	}

	// 등급표시 그리기
	if (IsWhereServiceArea(ConstServiceArea_Korea) && GetGameStatus() == GS_GAME && m_kTimer_ShowRank_Interval.IsTimed(GetTickCount()) == true)
	{
		DrawGovernMentRankMark(1);
	}

#ifdef USE_GAME_GUARD_CLIENT

	if(m_kTimer_Check_GameGuard_Interval.IsTimed(GetTickCount()) == true)
	{
		if( pNpgl->Check() != NPGAMEMON_SUCCESS ) dwQuitTimer = 1;
	}

#endif

#ifdef USE_HANAUTH

	// 일정시간 간격으로 서버를 사용할 수 있는 나이인지 체크한다.
/*	if( GetGameStatus() >= GS_GAME &&
		kHanAuth_UpdateTimer_GameRunAuth.IsTimed(::GetTickCount(), true))
	{
		// 한국 NHN에서만 게임중에 나이를 체크한다.
		if(IsWhereServiceArea(ConstServiceArea_Korea))
		{
			// 인증스트링 얻기
			SI32 siHanAuthRet = GetAuthString(m_szHanAuth_AuthString, SIZE_AUTHSTRING);
			if(siHanAuthRet != 0)		{		m_szHanAuth_GameString[0] = '\0';		}

			cltHanAuth_GameRunAuth clRunAuth;
			MStrCpy(clRunAuth.authstring,	m_szHanAuth_AuthString,	2048 );
			MStrCpy(clRunAuth.userid,		m_szHanAuth_UserID,		MAX_PLAYER_NAME );

			cltMsg clMsg(GAMEMSG_REQUEST_HANAUTH_GAMERUNAUTH, sizeof(cltHanAuth_GameRunAuth), (BYTE*)&clRunAuth);

			SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
*/
#endif

	if ( GetGameStatus() >= GS_INITCONNECTFRONTSERVER && GetGameStatus() < GS_GAME )
	{
		if ( lpBackScreen && GP.LockSurface( lpBackScreen ) == TRUE )
		{
			GP.UnlockSurface( lpBackScreen );
		}
		else
		{
			return;
		}

		GP.ClearSurface( lpBackScreen );
		
/*
// 동영상 출력
		if( m_DrawLogo ) //잠시 막자
			m_DrawLogo->PlayMovie(TEXT("Movie/Logo.mpg"));
// 보여주고 없애자
		if ( m_DrawLogo )
		{
			delete m_DrawLogo;
			m_DrawLogo = NULL;
		}
*/
		if ( m_pLoadingImg )
		{
			m_pLoadingImg->Draw( lpBackScreen, GetScreenRX(), GetScreenRY() );
		}

		GameDrawUpdateScreen();
	}

}

void cltClient::GsGame()
{

#ifndef JEWAN_FRAME_SKIP

	if(bDrawSkipSwitch == false)
	{
		GameDraw();	
	}

#endif

	GameProcess();
}


void cltClient::GsInitConnectFrontServer()
{
	//DrawGovernMentRankMark(0);

	// 프론트 서버에 접속하도록 만든다. 
	if(InitConnectToFrontServer() == true)
	{
		SetGameStatus(GS_CONNECTFRONTSERVER);
	}
	else
	{
		SetGameStatus( GS_GAMEDESTROY );
		//DestroyWindow(GetHwnd());
	}
}

void cltClient::GsConnectFrontServer()
{
	// 클라이언트 인 경우 서버와 접속을 시도한다. 
	SI32 returnval = ConnectToFrontServer();

	if(returnval == 1)
	{
		SetGameStatus(GS_SHOWGOVERNMENT);

		if ( IsWhereServiceArea(ConstServiceArea_Korea) )
		{
			m_kTimer_WaitGameLogin.SetActive( true, GetTickCount() );
		}
		else
		{
			m_kTimer_WaitGameLogin.SetActive( true, 1 );
		}
	}
	else if(returnval == -1)
	{
		SetGameStatus( GS_GAMEDESTROY );
		//DestroyWindow(GetHwnd());
	}
}

void cltClient::GsInitConnectGameServer()
{

	// 게임  서버에 접속하도록 만든다. 
	if(InitConnectToGameServer() == true)
	{
		SetGameStatus(GS_CONNECTGAMESERVER);
	}
	else
	{
	}


}

void cltClient::GsConnectGameServer()
{
	// 클라이언트 인 경우 서버와 접속을 시도한다. 
	SI32 returnval = ConnectToGameServer();

	if(returnval == 1)
	{
		SetGameStatus(GS_INITCHARLIST);

	}
	else if(returnval == -1)
	{
		DestroyWindow(GetHwnd());

		// 한게임 범용보고
		//HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_CLIENT_SELECTSERVER);
	}
}


void cltClient::GsInitCharList()
{
	// 캐릭터 리스트가 준비되어 있다면, 
	if(IsReadyCharList() == TRUE)
	{
		SetGameStatus(GS_CHARLIST);
		clSelectedWorldInfo.baseinfo.worldId;
		
		// 한게임 범용보고
		if(siServiceArea == ConstServiceArea_NHNChina)
		{
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(clSelectedWorldInfo.baseinfo.siServerID ,m_szPorteAuth_UserID, HANREPORT_CLIENT_SELECTCHAR);
		}
		else
		{
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_CLIENT_SELECTCHAR);
		}
	}
	else
	{
	}
}


void cltClient::GsCharList()
{
	if(SelectChar() == FALSE)
	{	
		SetGameStatus(GS_GAME);

		// 한게임 범용보고
		if(siServiceArea == ConstServiceArea_NHNChina)
		{
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(clSelectedWorldInfo.baseinfo.siServerID ,m_szPorteAuth_UserID, HANREPORT_CLIENT_LOGINTOGAMESERVER);
		}
		else
		{
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_CLIENT_LOGINTOGAMESERVER);
		}


		//cyj Toolbar, Statusbar 게임 들어간후에 생성
		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{
			/*if ( m_pDialog[ NCHAT_DLG ] )
			{
				if ( ! m_pDialog[ NCHAT_DLG ]->IsShow() )
				{
					m_pDialog[ NCHAT_DLG ]->Show( true );
				}
			}*/
			if ( m_pDialog[ STATUSBAR_DLG ] )
			{
				if ( ! m_pDialog[ STATUSBAR_DLG ]->IsShow() )
				{
					m_pDialog[ STATUSBAR_DLG ]->Show( true );
				}
			}
			if ( m_pDialog[ NCHARMENUBAR_DLG ] )
			{
				if ( ! m_pDialog[ NCHARMENUBAR_DLG ]->IsShow() )
				{
					m_pDialog[ NCHARMENUBAR_DLG]->Show( true );
				}
			}				
			if ( m_pDialog[ NSYSMENUBAR_DLG ] )
			{
				if ( ! m_pDialog[ NSYSMENUBAR_DLG ]->IsShow() )
				{
					m_pDialog[ NSYSMENUBAR_DLG ]->Show( true );
				}
			}
			if ( m_pDialog[ NCHATBAR_DLG ] )
			{
				if ( ! m_pDialog[ NCHATBAR_DLG ]->IsShow() )
				{
					m_pDialog[ NCHATBAR_DLG ]->Show( true );
				}
			}

			if ( m_pDialog[ NMINIMAP_DLG ] )
			{
				if( ! m_pDialog[ NMINIMAP_DLG ]->IsShow() )
				{
					m_pDialog[ NMINIMAP_DLG ]->Show( true );
				}
			}

			if ( m_pDialog[ NPREMIUMSTATUS_DLG ] )
			{
				if( ! m_pDialog[ NPREMIUMSTATUS_DLG ]->IsShow() )
				{
					m_pDialog[ NPREMIUMSTATUS_DLG ]->Show( true );
				}
			}

			if ( m_pDialog[ NBUFFSTATUS_DLG ] )//툴팁추가.[2007.07.12 성웅]
			{
				if( ! m_pDialog[ NBUFFSTATUS_DLG ]->IsShow() )
				{
					m_pDialog[ NBUFFSTATUS_DLG ]->Show( true );
				}
			}
			
			if ( m_pDialog[ NDORMANCYPOINT_DLG ] )//휴면 포인트 다이얼로그 [2009.09.15 기형]
			{
				if( ! m_pDialog[ NDORMANCYPOINT_DLG ]->IsShow() )
				{
					m_pDialog[ NDORMANCYPOINT_DLG ]->Show( true );
				}
			}
		}

	}
}

void cltClient::GsQuit()
{
	DestroyWindow(GetHwnd());
}


void cltClient::GsInitSelectWorldList()
{

	if(CreateSelectWorldListDlg(true) == true)
	{
		// 한게임 범용보고
		if(siServiceArea == ConstServiceArea_NHNChina)
		{
			HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(clSelectedWorldInfo.baseinfo.siServerID ,m_szPorteAuth_UserID, HANREPORT_CLIENT_SELECTSERVER);
		}
		else
		{
			HanAuth_Lib_HanReportSendPosLog_Client(m_szHanAuth_UserID, HANREPORT_CLIENT_SELECTSERVER);
		}


		SetGameStatus(GS_SELECTWORLDLIST);
	}
}

void cltClient::GsSelectWorldList()
{

	if(SelectWorldList() == true)
	{
		SetGameStatus(GS_INITCONNECTGAMESERVER);
	}
}

void cltClient::GsShowGovernment()
{
	if ( m_kTimer_WaitGameLogin.IsTimed(GetTickCount()) )
	{
		SetGameStatus( GS_INITSELECTWORLDLIST );
	}
}
