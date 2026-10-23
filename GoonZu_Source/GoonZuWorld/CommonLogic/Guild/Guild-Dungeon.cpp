#include "Guild-Dungeon.h"
#include "../../Server/Server.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"

extern cltCommonLogic* pclClient;

cltGuildDungeonMgr::cltGuildDungeonMgr()
{
	m_UpdateDBClock = 0;
	m_SendDBDay = 0;
	SetDungeon();
}

cltGuildDungeonMgr::~cltGuildDungeonMgr()
{

}

void cltGuildDungeonMgr::SetDungeon()
{
	SI32 i = 0;
	for ( SI32 mapindex = 0; mapindex < MAX_MAP_NUMBER ; mapindex++ )
	{
		if ( pclClient->pclMapManager->IsGuildMap(mapindex) )
		{
			m_clUnit[i++].SetMapIndex( mapindex );

			if ( i >= MAX_GUILDDUNGEON ) return;
		}
	}
}

void cltGuildDungeonMgr::SetStatus(SI32 mapindex, SI32 guildunque, SI32 exp, GMONEY money )
{
	for ( SI32 i=0; i< MAX_GUILDDUNGEON; i++ )
	{
		if ( m_clUnit[i].GetMapIndex() == mapindex )
		{
			m_clUnit[i].SetGuildUnique( guildunque );
			m_clUnit[i].SetExp( exp );
			m_clUnit[i].SetMoney( money );

			break;
		}
	}
}

bool cltGuildDungeonMgr::CanApplyGuildDungeon( SI32 mapindex )
{
	cltServer* pclserver= (cltServer*)pclClient;

	SI32 count = 0;
	for( SI32 i=0 ; i <MAX_GUILD_NUM; i++)
	{
		if ( pclClient->pclGuildManager->clUnit[i].clGPoint.siApplyMapIndex == mapindex )
			count++;

		if ( count == MAX_GUILDDUNGEONAPPLY )
			break;
	}

	if ( count <  MAX_GUILDDUNGEONAPPLY )
	{
		return true;
	}

	return false;
}

void cltGuildDungeonMgr::AddExp(SI32 mapindex, SI32 exp )
{
	for ( SI16 i=0; i<MAX_GUILDDUNGEON; i++)
	{
		if ( m_clUnit[i].GetMapIndex() ==  mapindex)
		{
			SI32 GuildUnique = m_clUnit[i].GetGuildUnique() ; // 맵인덱스가 길드 사냥터 이고 , 현제 이 맵에 길드 유니크가 있을경우에만 exp를 더한다
			if ( GuildUnique > 0 ) {
				m_clUnit[i].AddExp( exp );
				break;
			}

		}
	}
}

void cltGuildDungeonMgr::AddMoney(SI32 mapindex, GMONEY money )
{
	for ( SI16 i=0; i<MAX_GUILDDUNGEON; i++)
	{
		if ( m_clUnit[i].GetMapIndex() ==  mapindex)
		{
			SI32 GuildUnique = m_clUnit[i].GetGuildUnique() ; // 맵인덱스가 길드 사냥터 이고 , 현제 이 맵에 길드 유니크가 있을경우에만 exp를 더한다
			if ( GuildUnique > 0 ) {
				m_clUnit[i].AddMoney( money );
				break;
			}

		}
	}
}
SI32 cltGuildDungeonMgr::GetExp(SI32 mapindex )
{
	for ( SI16 i=0; i<MAX_GUILDDUNGEON; i++)
	{
		if ( m_clUnit[i].GetMapIndex() == mapindex )
		{
			return m_clUnit[i].GetExp();
		}
	}

	return 0;
}

GMONEY cltGuildDungeonMgr::GetMoney(SI32 mapindex )
{
	for ( SI16 i=0; i<MAX_GUILDDUNGEON; i++)
	{
		if ( m_clUnit[i].GetMapIndex() == mapindex )
		{
			return m_clUnit[i].GetMoney();
		}
	}
	return 0;
}

void cltGuildDungeonMgr::Action()
{
	// 1분에 한번씩 DB에 저장.

	cltServer* pclserver = (cltServer*)pclClient;

	if ( (TRUE == pclserver->bCompleteDBInitAll) && (TABS(pclClient->CurrentClock - m_UpdateDBClock) > 1000 * 60) )
	{
		// DB로 보낸다.

		m_UpdateDBClock = pclClient->CurrentClock;

		UpdateDB();
	}

	if (pclserver->IsCountrySwitch(Switch_NewGuildHuntMapWar) ) 
	{
		WORD currentDay = 0;
		currentDay = pclserver->sTime.wDay;
		if(1 == pclserver->pclTime->clDate.uiMonth	&&  1 == pclserver->pclTime->clDate.uiDay)
		{
			// 획득한 GP에 따라 소유권을 이전하고 ( DB 에서 )
			// 초기화 시킨다
			if ( m_SendDBDay != currentDay )
			{
				MoveProcess();
				m_SendDBDay = currentDay;
			}
		}
	}
	else
	{
		WORD currentDay = 0;
		currentDay = pclserver->sTime.wDay;
		if ( currentDay == 1 || currentDay == 7 || currentDay == 13 
			|| currentDay == 19 || currentDay == 25 )
		{
			if ( pclserver->sTime.wHour == 0 && pclserver->sTime.wMinute == 0 )
			{
				if ( m_SendDBDay != currentDay )
				{	
					// 획득한 GP에 따라 소유권을 이전하고 ( DB 에서 )
					// 초기화 시킨다

					MoveProcess();

					m_SendDBDay = currentDay;	
				}
			}
		}		
	}
}

void cltGuildDungeonMgr::UpdateDB()
{
	for ( SI16 i = 0; i < MAX_GUILDDUNGEON ; i++ )
	{
		if ( m_clUnit[i].GetMapIndex() == 0 )
			continue;

		//[진성] 길드 유니크가 없으면 db에 기록을 하지 않는다. => 2008-10-10
		/*if ( m_clUnit[i].GetGuildUnique() <= 0 ) 
			continue;*/
		
		if( m_clUnit[i].GetMapIndex() )
		{
			sDBRequest_GuildDungeon_SaveStatus clMsg(	m_clUnit[i].GetMapIndex(), 
														m_clUnit[i].GetExp(), 
														m_clUnit[i].GetMoney() );
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );
		}
	}
}

void cltGuildDungeonMgr::LoadFromDB()
{
	for ( SI16 i = 0; i < MAX_GUILDDUNGEON ; i++ )
	{
		if ( m_clUnit[i].GetMapIndex() == 0 )
			continue;

		sDBRequest_GuildDungeon_LoadStatus clMsg( m_clUnit[i].GetMapIndex() );
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBRequest_GuildDungeon_LoadStatus"));
}

void cltGuildDungeonMgr::MoveProcess()
{
	// 소유권 이전 작업 하기전에 현재까지의 상태를 DB로 보냄
	UpdateDB();

	// 소유권 이전한다.
	// DB에서 하는작업 
	// 1. 각 map별로 가장 많은 GP를 획득한 길드 추출
	// 2. 모든 길드의 신청 mapindex와 battleGP 초기화 시킴
	// 3. map별로 추출한 길드의 소유권을 넘김

	// PCK : 중국, 독일은 자본금, GP 보상 퍼센트가 0.03%가 되도록 한다. (09.04.14)
	if(pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) || pclClient->IsWhereServiceArea(ConstServiceArea_EUROPE))
	{
		sDBRequest_GuildDungeon_TransferOwnership clMsg(GUILDDUNGEON_GIVEGPRATIO_EXCEPTION, GUILDDUNGEON_GIVEMONEYRATIO_EXCEPTION);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );
	}
	else
	{
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
		{
			sDBRequest_GuildDungeon_TransferOwnership clMsg(GUILDDUNGEON_GIVEGPRATIO_EXCEPTION, GUILDDUNGEON_GIVEMONEYRATIO_EXCEPTION);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );
		}
		else
		{
			sDBRequest_GuildDungeon_TransferOwnership clMsg(GUILDDUNGEON_GIVEGPRATIO, GUILDDUNGEON_GIVEMONEYRATIO);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );
		}
	}
}

SI32 cltGuildDungeonMgr::GetMapIndex(SI32 guildunique )
{
	for ( SI16 i = 0; i < MAX_GUILDDUNGEON ; i++ )
	{
		if ( m_clUnit[i].GetMapIndex() == 0 )
			continue;

		if ( m_clUnit[i].GetGuildUnique() == guildunique )
			return m_clUnit[i].GetMapIndex();
	}

	return 0;
}