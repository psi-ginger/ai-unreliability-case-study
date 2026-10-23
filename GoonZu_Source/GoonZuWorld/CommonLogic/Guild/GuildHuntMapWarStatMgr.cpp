#include "GuildHuntMapWarStatMgr.h"
#include "../CommonLogic.h"
#include "../server/server.h"
#include "../Common/Char/CharManager/CharManager.h"
#include "../CommonLogic/MsgType-Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "../Client/MarkManager/MarkManager.h"
#include "../Client/ninterface/NKillSignDlg/NKillSignDlg.h"
#include "../Client/ninterface/NGuildWarResult/NGuildWarResult.h"

extern cltCommonLogic* pclClient;


cltGuildHuntMapWarStatMgr::cltGuildHuntMapWarStatMgr()
{
	Init();
}

cltGuildHuntMapWarStatMgr::~cltGuildHuntMapWarStatMgr()
{
	Init();	
}


void	cltGuildHuntMapWarStatMgr::Init()
{ 
	for(int i = 0; i < GUILDTEAM_END; i++)
	{
		ZeroMemory(&m_szGuildName[i], MAX_GUILD_NAME);	//	길드 이름 초기화

		m_siBGP[i]				=	0;						//	BGP 초기화
		m_siGuildPoint[i]		=	0;						//	길드 포인트 초기화
		m_siGuildMarkIndex[i]	=	0;						//	길드 마크 인덱스
	}

	ZeroMemory(&m_siKillUserName, MAX_PLAYER_NAME);
	ZeroMemory(&m_siDeathUserName, MAX_PLAYER_NAME);
	
	m_bGuildWarSwitch	=	false;

	SetDrawFlag(DRAWFLAG_NONE);
	m_noticeClock		=	0;
	m_siResult			=	0;
}

//	사냥터 획득 길드전 시작
void cltGuildHuntMapWarStatMgr::StartGuildWar( TCHAR* homename, TCHAR* awayname, SI16 homemark, SI16 awaymark )
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	Init();


	m_bGuildWarSwitch = true;

    SetGuildName(GUILDTEAM_HOME, homename);
	SetGuildName(GUILDTEAM_AWAY, awayname);

	SetGuildMarkIndex(GUILDTEAM_HOME, homemark);
	SetGuildMarkIndex(GUILDTEAM_AWAY, awaymark);

	SetDrawFlag(DRAWFLAG_START);
	m_noticeClock = pclClient->CurrentClock;
	m_endClock =  pclClient->CurrentClock + ( GUILDHUNTWAR_TIME * 1000 * 60 );



	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->CreateInterface(NGUILDWARKILLSIGN_DLG);

	pclclient->CreateInterface(NGUILDWARRESULT_DLG);	
	((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG])->Show(false);
	((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG])->m_InterfaceFactory.GetList(NGUILDWARRESULT_LIST)->Clear();	

}

//	사냥터 획득 길드전 종료
void cltGuildHuntMapWarStatMgr::EndGuildWar(SI32 recvGP, SI32 siTeamIndex, TCHAR* szPlayerName, SI32  siKillNum, SI32  siDeathNum)
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	cltClient* pclclient = (cltClient*)pclClient;	
	if(((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG]) == NULL)
		pclclient->CreateInterface(NGUILDWARRESULT_DLG);	

	m_siResult	=	recvGP;

	switch(recvGP)
	{
	case 0:				//	승리
	case 3:
		{
			SetDrawFlag(DRAWFLAG_WIN);
		}
		break;

	case 1:				//	비김
	case 4:
		{
			SetDrawFlag(DRAWFLAG_DRAW);
		}
		break;

	case 2:				//	패배
	case 5:
		{
			SetDrawFlag(DRAWFLAG_LOSE);
		}
		break;

	case 6:				//	잘못된 데이터
		{
			_ASSERT(0);
		}
		break;
	}


	if(((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG]) != NULL)
	{
		((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG])->Show(true);
		((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG])->InputData(siTeamIndex, szPlayerName, siKillNum, siDeathNum);
	}
	
	
	((CNGuildWarResult*)pclclient->m_pDialog[NGUILDWARRESULT_DLG])->setWinLose(recvGP);

	

	m_noticeClock = pclClient->CurrentClock; 

	m_bGuildWarSwitch = false;

	m_siGuildPoint[GUILDTEAM_HOME]	=	0;								
	m_siGuildPoint[GUILDTEAM_AWAY]	=	0;
	m_siGuildMarkIndex[GUILDTEAM_HOME]	=	0;								
	m_siGuildMarkIndex[GUILDTEAM_AWAY]	=	0;								
	SetDrawFlag(DRAWFLAG_NONE);

	//SetGuildName(GUILDTEAM_HOME, TEXT(""));
	//SetGuildName(GUILDTEAM_AWAY, TEXT(""));

	m_endClock = 0;
}

//	길드 이름 설정
void	cltGuildHuntMapWarStatMgr::SetGuildName(SI32 siTeamIndex, TCHAR* szGuildName)
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	StringCchCopy(m_szGuildName[siTeamIndex], MAX_GUILD_NAME, szGuildName);
}


void	cltGuildHuntMapWarStatMgr::SetGuildMarkIndex(SI32 siTeamIndex, SI16 siGuildMarkIndex)			//	길드 마크 인덱스 설정
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	m_siGuildMarkIndex[siTeamIndex]	=	siGuildMarkIndex;
}

//	BGP 설정
void	cltGuildHuntMapWarStatMgr::SetBGP(SI32 siTeamIndex, SI32 siBGP)
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	m_siBGP[siTeamIndex] = siBGP;
}


//	길드전중 Kill Death 현황
void	cltGuildHuntMapWarStatMgr::SetPersonKillInfo(TCHAR* szKillUserName, TCHAR* szDeathUserName, SI32 siTeamIndex)
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	StringCchCopy(m_siKillUserName, MAX_PLAYER_NAME, szKillUserName);
	StringCchCopy(m_siDeathUserName, MAX_PLAYER_NAME, szDeathUserName);

	SetGuildPoint(siTeamIndex);		//	길드 포인트 설정


	cltClient* pclclient = (cltClient*)pclClient;
	
	if(((CNKillSignDlg*)pclclient->m_pDialog[NGUILDWARKILLSIGN_DLG]) == NULL)
	{
		pclclient->CreateInterface(NGUILDWARKILLSIGN_DLG);

	}

	((CNKillSignDlg*)pclclient->m_pDialog[NGUILDWARKILLSIGN_DLG])->SetKillSign(m_siKillUserName, m_siDeathUserName);
}


void	cltGuildHuntMapWarStatMgr::SetGuildPoint(SI32 siTeamIndex)				//	길드 포인트 설정
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	m_siGuildPoint[siTeamIndex]++;
}

void	cltGuildHuntMapWarStatMgr::SetDrawFlag(SI32 siDrawFlag)								// 드로우플래그 설정
{
	/*if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}*/

	m_siDrawFlag	=	siDrawFlag;
}


// 화면에 Display
void	cltGuildHuntMapWarStatMgr::DrawStatus(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)
{
	if (pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) != true)
	{
		_ASSERT(0);
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Wrong Routine : cltGuildHuntMapWarStatMgr::StartGuildWar() Check Switch_NewGuildHuntMapWar"));
	}

	SI32 yindent  = 0;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}

	cltClient* pclclient = (cltClient*)pclClient;


	if(GetDrawFlag() != DRAWFLAG_NONE)
	{
		TSpr* pspr = pclClient->GetGlobalSpr(GIMG_GUILDWAR_NOTICE);
		if(pspr == NULL)return ;

		// 전투 시작, 종료 알림 메세지 출력
		SI32 xPos = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx - 10 ;
		SI32 yPos = ry+yindent + 90;

		if( GP.LockSurface(lpsurface)==TRUE )
		{
			GP.PutSpr(pspr, xPos, yPos, GetDrawFlag() - 1 );//이미지 리소스 분리 작업 [2007.07.18 손성웅]

			GP.UnlockSurface(lpsurface);
		}
	}

	if( TABS(pclClient->CurrentClock - m_noticeClock) > 10000)
		SetDrawFlag(DRAWFLAG_NONE);

	// 시간 출력
	if( m_endClock )
	{
		clock_t overTime = ( m_endClock - ( pclClient->CurrentClock ) ) / 1000;

		overTime = max( 0, overTime );

		TSpr* pspr = pclClient->GetGlobalSpr(GIMG_GUILDWAR_TIME);
		if(pspr == NULL)return ;

		// 전투 시작, 종료 알림 메세지 출력
		SI32 xPos = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx;
		SI32 yPos = ry+yindent + 70;

	

		SI32 siMinuteFirst	=	0;			//	두자릿수 minute
		SI32 siMinuteLast	=	0;			//	한자릿수 minute
		SI32 siSecondFirst	=	0;			//	두자릿수 second
		SI32 siSecondLast	=	0;			//	두자릿수 second

		//	count -> 분 : 초 로 변환
		if( GP.LockSurface(lpsurface)==TRUE )
		{
			SI32	siMinute	=	overTime / 60;
			SI32	siSecond	=	overTime % 60;

			if(siMinute >= 10)
			{
				siMinuteFirst	=	siMinute / 10;
				siMinuteLast	=	siMinute % 10;
			}
			else
			{
				siMinuteFirst	=	0;
				siMinuteLast	=	siMinute;
			}
			
			if(siSecond >= 10)
			{
				siSecondFirst	=	siSecond / 10;
				siSecondLast	=	siSecond % 10;
			}
			else
			{
				siSecondFirst	=	0;
				siSecondLast	=	siSecond;
			}

			GP.PutSpr(pspr, xPos - 34, yPos, siMinuteFirst);
			GP.PutSpr(pspr, xPos - 17, yPos, siMinuteLast);

			GP.PutSpr(pspr, xPos, yPos, 10);

			GP.PutSpr(pspr, xPos + 17, yPos, siSecondFirst);
			GP.PutSpr(pspr, xPos + 34, yPos, siSecondLast);

			GP.UnlockSurface(lpsurface);
		}
	}

	if( m_siDrawFlag || m_bGuildWarSwitch )
	{
		// 카운트 출력
		if ( GP.LockSurface(lpsurface)==TRUE )
		{
			TSpr* pspr =  pclClient->GetGlobalSpr(GIMG_GUILDWAR_COUNT);

			if(pspr == NULL)
			{
				GP.UnlockSurface(lpsurface);
				return ;
			}

			SI32 homeX = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx - 190 ;
			SI32 awayX = (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx + 170 ;

			SI32 homeY = ry+yindent + 100;
			SI32 awayY = homeY;

			SI32 markXpos = 0;

			
			// 홈길드 출력
			if ( m_siGuildPoint[GUILDTEAM_HOME] / 100 > 0 )
			{
				GP.PutSpr(pspr, homeX, homeY+30, m_siGuildPoint[GUILDTEAM_HOME]/100 );
				GP.PutSpr(pspr, homeX + 30, homeY+30, m_siGuildPoint[GUILDTEAM_HOME] % 100 / 10 );
				GP.PutSpr(pspr, homeX + 60, homeY+30, m_siGuildPoint[GUILDTEAM_HOME] % 10 );
				markXpos = homeX + 30;
			}
			else if ( m_siGuildPoint[GUILDTEAM_HOME] / 10 > 0 )
			{
				GP.PutSpr(pspr, homeX, homeY+30, m_siGuildPoint[GUILDTEAM_HOME] / 10 );
				GP.PutSpr(pspr, homeX + 30, homeY+30, m_siGuildPoint[GUILDTEAM_HOME] % 10 );
				markXpos = homeX + 15;
			}
			else
			{
				GP.PutSpr(pspr, homeX, homeY+30, m_siGuildPoint[GUILDTEAM_HOME] % 10 );
				markXpos = homeX;
			}

			if ( m_siGuildMarkIndex[GUILDTEAM_HOME] > 0 )
			{
				SI32 guildicon = -1;
				TSpr* pmarkspr = NULL;		

				pclclient->m_pMarkManager->GetMarkSprAndFontIndex(m_siGuildMarkIndex[GUILDTEAM_HOME] , &pmarkspr , &guildicon );
				if( pmarkspr != NULL && guildicon >= 0 )
				{
					GP.PutSpr(pmarkspr, markXpos, homeY, guildicon );
				}

			}

			// 원정길드 출력
			if ( m_siGuildPoint[GUILDTEAM_AWAY] / 100 > 0 )
			{
				GP.PutSpr(pspr, awayX, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY]/100 );
				GP.PutSpr(pspr, awayX + 30, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY] % 100 / 10 );
				GP.PutSpr(pspr, awayX + 60, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY] % 10 );
				markXpos = awayX + 30;
			}
			else if ( m_siGuildPoint[GUILDTEAM_AWAY] / 10 > 0 )
			{
				GP.PutSpr(pspr, awayX + 30, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY] / 10 );
				GP.PutSpr(pspr, awayX + 60, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY] % 10 );
				markXpos = awayX + 45;
			}
			else
			{
				GP.PutSpr(pspr, awayX + 60, awayY+30, m_siGuildPoint[GUILDTEAM_AWAY] % 10 );
				markXpos = awayX + 60;
			}

			if ( m_siGuildMarkIndex[GUILDTEAM_AWAY] > 0 )
			{
				SI32 guildicon = -1;
				TSpr* pmarkspr = NULL;		

				pclclient->m_pMarkManager->GetMarkSprAndFontIndex(m_siGuildMarkIndex[GUILDTEAM_AWAY] , &pmarkspr , &guildicon );
				if( pmarkspr != NULL && guildicon >= 0 )
				{
					GP.PutSpr(pmarkspr, markXpos, homeY, guildicon );
				}
			}

			GP.UnlockSurface(lpsurface);

		}




		////	실시간 킬뎃 정보 
		//if ( GP.LockSurface(lpsurface)==TRUE )
		//{
		//	TSpr* pspr =  pclClient->GetGlobalSpr(GIMG_GUILDWAR_KILLSIGN);

		//	if(pspr == NULL)
		//	{
		//		GP.UnlockSurface(lpsurface);
		//		return ;
		//	}

		//	SI32 awayX = (pclclient->siClientScreenXsize - pspr->GetXSize()) /2 + rx + 350 ;

		//	SI32 homeY = ry+yindent + 250;
		//	SI32 awayY = homeY;

		//	SI32 markXpos = 0;

		//		GP.PutSpr(pspr, awayX, awayY+60, 0);
		//		markXpos = awayX + 30;

		//		//HDC hdc;
		//		//if(lpsurface->GetDC(&hdc)==DD_OK)
		//		//{
		//		//	if(m_siKillUserUID > 0 && m_siDeathUserUID > 0)
		//		//	{
		//		//		TCHAR szKillUser[256];
		//		//		itoa(m_siKillUserUID, szKillUser, 10);
		//		//		TCHAR szDeathUser[256];
		//		//		itoa(m_siDeathUserUID, szDeathUser, 10);

		//		//		pclClient->pclMessage->DrawMultyColorText(hdc, awayX - 50, awayY+60, szKillUser, COLOR_DIALOG_LIGHTBLUE);
		//		//		pclClient->pclMessage->DrawMultyColorText(hdc, awayX + 50, awayY+60, szDeathUser, COLOR_DIALOG_LIGHTBLUE);
		//		//	}
		//		//}
		//		GP.UnlockSurface(lpsurface);
		//	}
		}
}
























