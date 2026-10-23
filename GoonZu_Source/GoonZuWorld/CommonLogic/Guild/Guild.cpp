#include <CommonLogic.h>
#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "Guild.h"
#include "../../Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "../../Client/Music/Music.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Hunt.h"

extern cltCommonLogic* pclClient;

cltGuild::cltGuild(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
}

cltGuild::~cltGuild()
{
}

cltGuildManager::cltGuildManager(cltCharManager* pclcm, SI32 gamemode, SI32 portraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, portraitid, strFileName )
{
	for ( SI32 i = 0 ; i < MAX_GUILD_NUM ; i ++ )
	{
		clUnit[i].Init();
	}
	if(gamemode == GAMEMODE_SERVER)
	{
	}
	else
	{
		clCommonUnit.Init();
	}

	siTopClearPoint				= 0;
	siBeforeYearTopGuildUnique	= 0;
}

cltGuildManager::~cltGuildManager()
{
}

cltGuildInfo *cltGuildManager::GetGuildInfo(SI32 guildunique)
{
	if ( guildunique <= 0 )
		return NULL ;

	for ( SI32 i = 0 ; i < MAX_GUILD_NUM ; i ++ )
	{
		if ( clUnit[i].siUnique != 0 && clUnit[i].siUnique == guildunique )
		{
			return &clUnit[i];
		}
	}

	return NULL;
}
cltGuildInfo* cltGuildManager::GetGuildInfo( TCHAR* guildname )
{
	if ( guildname )
	{
		for( SI32 i = 0; i < MAX_GUILD_NUM; i++ )
		{
			if ( _tcsncmp( clUnit[i].szName, guildname, MAX_GUILD_NAME) == 0 )
			{
				return &clUnit[i];
			}
		}
	}

	return NULL;
}
TCHAR* cltGuildManager::GetGuildName(SI32 guildunique )
{
	if ( guildunique <= 0 )
		return NULL;

	for ( SI32 i = 0 ; i < MAX_GUILD_NUM ; i ++ )
	{
		if ( clUnit[i].siUnique != 0 && clUnit[i].siUnique == guildunique )
		{
			return clUnit[i].GetName();
		}
	}

	return NULL;
}

BOOL cltGuildManager::CreateGuild(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltGuild(siunique);
		return TRUE;
	}

	return FALSE;
}


bool cltGuildManager::AddGuildInfo(cltGuildInfo* pclinfo)
{
	if ( pclinfo->siUnique <= 0 )
		return false;

	for ( SI32 i = 0 ; i < MAX_GUILD_NUM ; i ++ )
	{
		if ( clUnit[i].siUnique == 0 )
		{
			clUnit[i].Set(pclinfo);
			return true ;
		}
	}

	return false ;
}

bool cltGuildManager::DeleteGuildInfo(SI32 guildunique)
{
	if ( guildunique <= 0 )
		return false ;

	for ( SI32 i = 0 ; i < MAX_GUILD_NUM ; i ++ )
	{
		if ( clUnit[i].siUnique != 0 && clUnit[i].siUnique == guildunique )
		{
			clUnit[i].Init();
			return true ;
		}
	}

	return false ;
}
// 최초 환영 인사말표시..
void cltGuildManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(3250);
	SetMainExplain(hDlg, pText);
}

void cltGuildManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}

void cltGuildManager::Action(bool bdaychangedswitch)
{
	cltServer* pclserver = (cltServer* )pclClient;
	if(pclserver == NULL)										return;

	// DB 초기화 완료될때까지 기다림
	if( pclserver->bCompleteDBInitAll == FALSE )				return;

	//---------------------------------------------------
	// 매월 1일에 휴면길드를 정리한다.
	//---------------------------------------------------
	if(bdaychangedswitch)
	{
		if(pclClient->IsCountrySwitch(Switch_Guild_DestroyOverMOnth))
		{
			// 매월 1일에...
			if( pclserver->pclTime->clDate.uiDay == 1 )
			{
				SYSTEMTIME stLastConnect;
				TimeSubDate(&pclserver->sTime, OVER30DAYSNOTINTOGAME, &stLastConnect);

				sDBRequest_Guild_CheckDestroy_Month clMsg( &stLastConnect );// 30일을 보낸다.
				pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);		
			}
		}
	}

	//---------------------------------------------------
	// 매일 수수료를 납부한다.
	//---------------------------------------------------
	if(bdaychangedswitch)
	{
		if(pclserver->pclTime != NULL)
		{
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();

			int count = 0;
			for( int i = 0; i < MAX_GUILD_NUM; ++i )
			{
				if( clUnit[i].siUnique > 0 )
				{
					// [진성] 기간이 음수가 나올수 없는데 나올지도 몰라서 일단 +,-든 30일 이상 차가 나면 들어가게 수정. => 2008-5-14
					if( labs(curdate - clUnit[i].clGPoint.clDate.GetDateVary()) >= 30 )
					{      
						SI32 guildunique = clUnit[i].siUnique ;

						cltStrInfo* pclstr = pclClient->pclGuildManager->GetStrInfo(clUnit[i].siVillageUnique);
						if(pclstr == NULL)		continue;

						GMONEY fee = pclstr->clGuildStrInfo.clFee.CalcFee(MIN_GUILD_CREATE_FEE);

						fee = fee + fee * clUnit[i].clGPoint.siFaultNum ;

						SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
						SI32 membernum = clUnit[i].GetMemberNum();

						npcrate = max( 1, ( npcrate / 100 ) );
						membernum = max( 1, membernum );


						GMONEY upkeep = ( npcrate  * membernum * membernum ) / 2 * 100;

						GMONEY totalfee = fee + upkeep;

						sDBRequest_Guild_Give_Fee clMsg(guildunique,totalfee,&pclserver->pclTime->clDate);
						pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
			}
		}
	}

	//---------------------------------------------------
	// 매년 1월 1일 - 최강길드 결정
	//---------------------------------------------------
	if(bdaychangedswitch)
	{
		if(pclserver->pclTime != NULL)
		{
			// 매년 1월 1일에...
			if( pclserver->pclTime->clDate.uiMonth == 1 &&
				pclserver->pclTime->clDate.uiDay   == 1	)
			{
				siTopClearPoint = 0;
				SI32 Request_Page = 1;

				// 인스턴스 던젼의 최고길드 정보를 요청한다.
				sDBRequest_YearRanking clBeforeYearMsg(0, INSTANCEMAPPOINT_TYPE_GUILD, pclserver->pclTime->clDate.uiYear - 1, true, Request_Page);
				pclserver->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clBeforeYearMsg);

				//[진성] 게임시간으로 1년이 지나 최강길드변경 => 2008-5-29
				pclClient->pclLog->FilePrint( "config\\GuildRank" , "Request_YearRanking = %d-%d-%d", pclserver->pclTime->clDate.uiYear, pclserver->pclTime->clDate.uiMonth, pclserver->pclTime->clDate.uiDay);
			}
		}
	}
}

//------------------------------------------------
// GuildInfo
//------------------------------------------------
SI32 cltGuildInfo::GetLimitMemberNum()
{
	SI32 num = 0 ;

	if ( clGPoint.siLevel <= 0 )
		return 0 ;

	if ( clGPoint.siLevel >= 82 )
		num = MAX_GUILD_MEMBER_NUM ;
	else if ( clGPoint.siLevel < 3 )
		num = MIN_GUILD_MEMBER_NUM ;
	else
	{
		num = ( ( clGPoint.siLevel - 1 ) / 3 ) + MIN_GUILD_MEMBER_NUM ;
	}

	return num ;
}

SI32 cltGuildInfo::GetMemberNum()
{
	SI32 num = 0 ;

	for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	{
		if ( clMember[i].siPersonID > 0 )
		{
			num ++ ;
		}
	}

	return num ;
}

bool cltGuildInfo::IsSecondMaster()
{
	for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	{
		if ( clMember[i].siPersonID > 0 )
		{
			if ( clMember[i].siType == GUILDRANKTYPE_SECONDMASTER )
			{
				return true ;
				break;
			}
		}
	}

	return false ;
}
TCHAR* cltGuildInfo::GetName()
{
	return szName;
}
//길드맵 소유방식변경 길드전 수정 
void cltGuildInfo::setHuntWarEnemy(SI32 enemyGuildunique )
{

	for (SI32 SLot = 0; SLot < MAX_GUUILDENEMYINFO_TODAY; SLot++ )
	{
		if( 0 == clHuntguildEnemy[SLot].GetEnemy() )
		{
			clHuntguildEnemy[SLot].SetEnemy( enemyGuildunique);
			break	;
		}
	}
}
SI32 cltGuildInfo::findEmptySlot()
{
	for (SI32 i = 0 ; i < MAX_GUUILDENEMYINFO_TODAY; i++)
	{
		if ( 0 == clHuntguildEnemy[i].GetEnemy() )
		{
			return i;
		}
	}
	return	0;
}
bool cltGuildInfo::FindBeforeEnemyGuild( SI32 siGuildUnique )
{
	for (SI32 i = 0 ; i < MAX_GUUILDENEMYINFO_TODAY; i++)
	{
		SI32 sibeforeEnemyGuild = clHuntguildEnemy[i].GetEnemy()	;
		if(sibeforeEnemyGuild	==	0)	return	false;
		if ( siGuildUnique ==  sibeforeEnemyGuild )
		{
			return true;
		}
	}
	return false;
}