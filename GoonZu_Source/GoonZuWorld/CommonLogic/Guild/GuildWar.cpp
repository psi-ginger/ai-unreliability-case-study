#include "guildwar.h"
#include "../CommonLogic.h"
#include "../server/server.h"
#include "../Common/Char/CharManager/CharManager.h"
#include "../CommonLogic/MsgType-Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"

extern cltCommonLogic* pclClient;

//-------------------------------------------------------------------------------
// cltGuildWarReserveUnit
//-------------------------------------------------------------------------------
void cltGuildWarReserveUnit::Set( SI32 roomindex, SI32 homeguildunique, TCHAR* homeguildname, SI32 awayguildunique, TCHAR* awayguildname )
{
	if( homeguildunique <= 0 )				return;
	if( awayguildunique <= 0 )				return;

	if( homeguildname == NULL )				return;
	if( awayguildname == NULL )				return;

	m_siRoomIndex = roomindex;
	m_siHomeGuildUnique = homeguildunique;
	m_siAwayGuildUnique = awayguildunique;
	StringCchCopy( m_szHomeGuildName , MAX_GUILD_NAME, homeguildname );
	StringCchCopy( m_szAwayGuildName , MAX_GUILD_NAME, awayguildname );
}

void cltGuildWarReserveUnit::Set( cltGuildWarReserveUnit* pUnit )
{
	if( pUnit == NULL )						return;

	m_siRoomIndex = pUnit->m_siRoomIndex;
	m_siHomeGuildUnique = pUnit->m_siHomeGuildUnique;
	m_siAwayGuildUnique = pUnit->m_siAwayGuildUnique;
	StringCchCopy( m_szHomeGuildName , MAX_GUILD_NAME, pUnit->m_szHomeGuildName);
	StringCchCopy( m_szAwayGuildName , MAX_GUILD_NAME, pUnit->m_szAwayGuildName);
}

void cltGuildWarReserveUnit::WarStart( SI32 fieldindex )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )											return;
	if ( m_siHomeGuildUnique == 0 || m_siAwayGuildUnique == 0 )			return;

	// 시작되었다고 설정
	m_bWarStart = true;

	// 길드전쟁 매니져 초기화
	pclserver->pclGuildWarManager[fieldindex]->Init();

	// 길드 정보 얻기
	cltGuildInfo* pclHomeGuild = pclClient->pclGuildManager->GetGuildInfo( m_siHomeGuildUnique );
	cltGuildInfo* pclAwayGuild = pclClient->pclGuildManager->GetGuildInfo( m_siAwayGuildUnique );
	if ( !( pclHomeGuild && pclAwayGuild ) )				{			return;				}

	// 해당 필드의 GuildManager에 값을 설정한다.
	pclserver->pclGuildWarManager[fieldindex]->m_siHomeGuildUnique = m_siHomeGuildUnique;
	pclserver->pclGuildWarManager[fieldindex]->m_siAwayGuildUnique = m_siAwayGuildUnique;
	StringCchCopy( pclserver->pclGuildWarManager[fieldindex]->m_szHomeGuildName, MAX_GUILD_NAME, m_szHomeGuildName );
	StringCchCopy( pclserver->pclGuildWarManager[fieldindex]->m_szAwayGuildName, MAX_GUILD_NAME, m_szAwayGuildName );

	//-------------------------------------------------------------
	// 길드전쟁을 시작 할 때,
	// 현재 접속되어있는 길드원들을 길드전쟁맵으로 이동시킨다.
	for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	{
		SI32 personID = pclHomeGuild->clMember[i].siPersonID;
		SI32 charID = pclClient->pclCM->GetIDFromPersonID(personID);
		if ( pclClient->pclCM->IsValidID( charID ) ) // 접속해있다면
		{
			cltWarpInfo clWarpInfo(1, 0, false, true, 0, 0, -1, -1);
			if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) )	// 변경후
			{
				clWarpInfo.uiMapIndex = GUILDWARFIELD_INDEX_OLD + fieldindex;
			}
			else	// 변경전
			{
				clWarpInfo.uiMapIndex = GUILDWARFIELD_INDEX + fieldindex;

			}
			SI32 siRand = rand()%4;
			clWarpInfo.uiX = Const_HomePos[ siRand * 2 ];
			clWarpInfo.uiY = Const_HomePos[ siRand * 2 + 1];

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[charID];

			pclchar->SetWarpInfo(&clWarpInfo, 2200);
			if ( pclchar->Warp() )
			{
				pclserver->pclGuildWarManager[fieldindex]->AddMember( personID, m_siHomeGuildUnique );
			}
		}

		personID = pclAwayGuild->clMember[i].siPersonID;
		charID = pclClient->pclCM->GetIDFromPersonID(personID);
		if ( pclClient->pclCM->IsValidID( charID ) ) // 접속해있다면
		{
			cltWarpInfo clWarpInfo(1, 0, false, true, 0, 0, -1, -1);
			if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) )	// 변경후
			{
				clWarpInfo.uiMapIndex = GUILDWARFIELD_INDEX_OLD + fieldindex;
			}
			else
			{
				clWarpInfo.uiMapIndex = GUILDWARFIELD_INDEX + fieldindex;
			}
			SI32 siRand = rand()%4;
			clWarpInfo.uiX = Const_AwayPos[ siRand * 2 ];
			clWarpInfo.uiY = Const_AwayPos[ siRand * 2 + 1];
	
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[charID];

			pclchar->SetWarpInfo(&clWarpInfo, 2200);
			if ( pclchar->Warp() )
			{
				pclserver->pclGuildWarManager[fieldindex]->AddMember( personID, m_siAwayGuildUnique );
			}
		}
	}
	//-------------------------------------------------------------

	pclserver->pclGuildWarManager[fieldindex]->StartGame();
}

bool cltGuildWarReserveUnit::IsReady()
{
	if ( m_siHomeGuildUnique > 0 && m_siAwayGuildUnique > 0 )
		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// cltGuildWarReserveManager
//-------------------------------------------------------------------------------
cltGuildWarReserveManager::cltGuildWarReserveManager()
{
	Init();
}
cltGuildWarReserveManager::~cltGuildWarReserveManager()
{

}

void cltGuildWarReserveManager::Init()
{
	//if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
	//{
	//	for ( SI32 fieldindex = 0; fieldindex < GUILDWAR_FIELDNUM_OLD; fieldindex++ )
	//	{
	//		if(fieldindex >= GUILDWAR_FIELDNUM_OLD) continue	;
	//		for ( SI32 roomindex = 0; roomindex < GUILDWAR_ROOMNUM; roomindex++)
	//		{
	//			m_clUnit[fieldindex][roomindex].Init();
	//			m_clUnit[fieldindex][roomindex].m_siRoomIndex = roomindex;
	//		}
	//	}

	//}
	//else// 바뀌기전 
	//{
	//	for ( SI32 fieldindex = 0; fieldindex < GUILDWAR_FIELDNUM; fieldindex++ )
	//	{
	//		if(fieldindex >= GUILDWAR_FIELDNUM) continue	;
	//		for ( SI32 roomindex = 0; roomindex < GUILDWAR_ROOMNUM; roomindex++)
	//		{
	//			m_clUnit[fieldindex][roomindex].Init();
	//			m_clUnit[fieldindex][roomindex].m_siRoomIndex = roomindex;
	//		}
	//	}

	//}
	for ( SI32 fieldindex = 0; fieldindex < GUILDWAR_FIELDNUM; fieldindex++ )
	{
		if(fieldindex >= GUILDWAR_FIELDNUM) continue	;
		for ( SI32 roomindex = 0; roomindex < GUILDWAR_ROOMNUM; roomindex++)
		{
			m_clUnit[fieldindex][roomindex].Init();
			m_clUnit[fieldindex][roomindex].m_siRoomIndex = roomindex;
		}
	}
	

	m_siToday = ((cltServer*)pclClient)->sTime.wDay;
}

SI32 cltGuildWarReserveManager::ApplyReserve( SI32 fieldindex, SI32 roomindex, SI32 homeguild, SI32 awayguild)
{
	// 안쓴다..
	return 0;
}

void cltGuildWarReserveManager::ServerAction()
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI16 day = (SI16)pclserver->sTime.wDay;
	SI16 hour = (SI16)pclserver->sTime.wHour;
	SI16 minute = (SI16)pclserver->sTime.wMinute;

	// 0시에 모든 길드 예약 정보를 초기화 한다.
	if( hour == 0 && m_siToday != day )
	{
		Init();
	}

	// 정각과 30분에 예약되어 있는 길드 전쟁을 시작한다.
	if( minute == 0 || minute == 30 )
	{
		// 해당 시간의 방 인덱스를 구한다.
		SI32 roomindex = hour * 2 + minute / 30;
		// 해당 시간의 모든 길드전쟁맵의 길드전쟁을 시작한다.

		// 바뀌고 나서
		if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
		{
			for ( SI32 field=0; field<GUILDWAR_FIELDNUM_OLD; field++ )
			{
				if(field >= GUILDWAR_FIELDNUM_OLD) continue	;
				if ( m_clUnit[field][roomindex].IsStartWar() == false ) 
				{
					if( pclClient->IsCountrySwitch(Switch_EnableGuildWar) )
						m_clUnit[field][roomindex].WarStart( field );
				}
			}

		}
		else	// 바뀌기 전 
		{
			for ( SI32 field=0; field<GUILDWAR_FIELDNUM; field++ )
			{
				if(field >= GUILDWAR_FIELDNUM) continue	;
				if ( m_clUnit[field][roomindex].IsStartWar() == false ) 
				{
					if( pclClient->IsCountrySwitch(Switch_EnableGuildWar) )
						m_clUnit[field][roomindex].WarStart( field );
				}
			}

		}
	}
}

bool cltGuildWarReserveManager::ReserveRoom( SI32 fieldindex, SI32 roomindex, SI32 homeguild, SI32 awayguild )
{
	cltServer* pclserver = (cltServer*)pclClient;

	if(roomindex < 0 || roomindex >= GUILDWAR_ROOMNUM )		return false;
	if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
	{
		if(fieldindex < 0 || fieldindex >= GUILDWAR_FIELDNUM_OLD )	return false;
	}
	else	// 바뀌기전 
	{
		if(fieldindex < 0 || fieldindex >= GUILDWAR_FIELDNUM )	return false;
	}

	cltGuildInfo* pclHomeGuild = pclClient->pclGuildManager->GetGuildInfo( homeguild );
	cltGuildInfo* pclAwayGuild = pclClient->pclGuildManager->GetGuildInfo( awayguild );
	if ( !( pclHomeGuild && pclAwayGuild ) )				return false;

	// 예약
	m_clUnit[fieldindex][roomindex].Set( roomindex, homeguild, pclHomeGuild->szName, awayguild,  pclAwayGuild->szName );

	// 자막광고로 알려줌
	TCHAR* pTitle = GetTxtFromMgr(6757);
	TCHAR* pText = GetTxtFromMgr(8279);
	TCHAR buf[256] = TEXT("");
	TCHAR Hour[4];
	TCHAR Minute[4];
	TCHAR aGuildname[64];// 찍힘당한 길드
	TCHAR hGuildname[64];// 찍는 길드

	SI32 siHour = roomindex / 2; // 시간.
	StringCchPrintf(Hour, 4, TEXT("%02d"), siHour);
	
	SI32 min = (roomindex*30)%60;
	StringCchPrintf(Minute, 4, TEXT("%02d"), min);

	StringCchPrintf(hGuildname, 20, TEXT("%s"), pclHomeGuild->szName );
	StringCchPrintf(aGuildname, 20, TEXT("%s"), pclAwayGuild->szName );

	Nsprintf( buf, pText, TEXT("hour"), Hour, TEXT("minute"), Minute, TEXT("aGuild"), aGuildname,TEXT("hGuild"), hGuildname,  NULL);
	((cltServer*)pclClient)->PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);

	sDBRequest_GuildWar_AddReserve clMsg(fieldindex, roomindex, homeguild, awayguild, pclHomeGuild->siMasterPersonID);
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	// 모든 길드원에게 길드전 쪽지를 보낸다
	cltLetterMsg_GuildWar_SuccessReserve clletter( fieldindex, roomindex, m_clUnit[fieldindex][roomindex].m_szHomeGuildName,  m_clUnit[fieldindex][roomindex].m_szAwayGuildName );
	for ( SI32 i=0; i<MAX_GUILD_MEMBER_NUM ; i++ )
	{
		if( pclHomeGuild->clMember[i].siPersonID > 0 )
            pclserver->SendLetterMsg(  pclHomeGuild->clMember[i].siPersonID , (cltLetterHeader*)&clletter );
		if( pclAwayGuild->clMember[i].siPersonID > 0 )
            pclserver->SendLetterMsg(  pclAwayGuild->clMember[i].siPersonID , (cltLetterHeader*)&clletter );
	}

	return true;
}

bool cltGuildWarReserveManager::IsReservedRoom( SI32 fieldindex, SI32 roomindex )
{
	if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
	{
		if( fieldindex < 0 || fieldindex >= GUILDWAR_FIELDNUM_OLD )	return true;
		if( roomindex < 0 || roomindex >= GUILDWAR_ROOMNUM )	return true;

		if ( m_clUnit[fieldindex][roomindex].m_siHomeGuildUnique > 0 ||
			m_clUnit[fieldindex][roomindex].m_siAwayGuildUnique > 0)
			return true;
		else
			return false;

	}
	else	//바뀌기전 
	{
		if( fieldindex < 0 || fieldindex >= GUILDWAR_FIELDNUM )	return true;
		if( roomindex < 0 || roomindex >= GUILDWAR_ROOMNUM )	return true;

		if ( m_clUnit[fieldindex][roomindex].m_siHomeGuildUnique > 0 ||
			m_clUnit[fieldindex][roomindex].m_siAwayGuildUnique > 0)
			return true;
		else
			return false;

	}
}

// 예약 할수 있는지 체크
SI32 cltGuildWarReserveManager::CanReserveRoom( SI32 siFieldIndex, SI32 siRoomIndex , SI32 siMyGuildUnique , SI32 siEnemyGuildUnique )
{
	SI32 resultVal = GUILDWAR_RESERVE_SUCCESS;
	SI32 siCountGuildWarHome = 0;
	SI32 siCountGuildWarAway = 0;

	if( siRoomIndex < 0 || siRoomIndex >= GUILDWAR_ROOMNUM ) 		return GUILDWAR_RESERVE_FAIL_ALREADYRESERVE;

	// 길드전쟁을 하루에 3번 이상할 수 없다.
	if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
	{
		for( SI16 siFieldnum = 0 ; siFieldnum < GUILDWAR_FIELDNUM_OLD ; siFieldnum++ )
		{
			if(siFieldnum >= GUILDWAR_FIELDNUM_OLD) continue	;
			for( SI16 siRoomnum = 0 ; siRoomnum < GUILDWAR_ROOMNUM ; siRoomnum++ )
			{
				if( m_clUnit[ siFieldnum ][siRoomnum].m_siHomeGuildUnique == siMyGuildUnique ){
					siCountGuildWarHome = siCountGuildWarHome + 1;
				}
				if( m_clUnit[ siFieldnum ][siRoomnum].m_siAwayGuildUnique == siEnemyGuildUnique ){
					siCountGuildWarAway = siCountGuildWarAway + 1;
				}                
			}
		}
	}
	else	//바뀌기전 
	{
		for( SI16 siFieldnum = 0 ; siFieldnum < GUILDWAR_FIELDNUM ; siFieldnum++ )
		{
			if(siFieldnum >= GUILDWAR_FIELDNUM) continue	;
			for( SI16 siRoomnum = 0 ; siRoomnum < GUILDWAR_ROOMNUM ; siRoomnum++ )
			{
				if( m_clUnit[ siFieldnum ][siRoomnum].m_siHomeGuildUnique == siMyGuildUnique ){
					siCountGuildWarHome = siCountGuildWarHome + 1;
				}
				if( m_clUnit[ siFieldnum ][siRoomnum].m_siAwayGuildUnique == siEnemyGuildUnique ){
					siCountGuildWarAway = siCountGuildWarAway + 1;
				}                
			}
		}
	}
	if ( siCountGuildWarAway > GUILDWAR_MAXCOUNT || siCountGuildWarHome > GUILDWAR_MAXCOUNT )
	{
		return GUILDWAR_RESERVE_FAIL_ENEMY_OVERTIME ;
	}
	if( pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar)	 )// 바뀌고 나서
	{
		for( SI16 i = 0 ; i < GUILDWAR_FIELDNUM_OLD ; i++ )
		{
			if(i >= GUILDWAR_FIELDNUM_OLD) continue	;
			if( m_clUnit[ i ][siRoomIndex].m_siHomeGuildUnique == siMyGuildUnique || m_clUnit[ i ][siRoomIndex].m_siAwayGuildUnique == siMyGuildUnique )
			{
				return GUILDWAR_RESERVE_FAIL_ALREADYRESERVE ;
			}
			else if( m_clUnit[ i ][siRoomIndex].m_siHomeGuildUnique == siEnemyGuildUnique || m_clUnit[ i ][siRoomIndex].m_siAwayGuildUnique == siEnemyGuildUnique )
			{
				return GUILDWAR_RESERVE_FAIL_ENEMY_ALREADYRESERVE ;
			}
		}
	}
	else	// 바뀌기 전
	{
		for( SI16 i = 0 ; i < GUILDWAR_FIELDNUM ; i++ )
		{
			if(i >= GUILDWAR_FIELDNUM) continue	;
			if( m_clUnit[ i ][siRoomIndex].m_siHomeGuildUnique == siMyGuildUnique || m_clUnit[ i ][siRoomIndex].m_siAwayGuildUnique == siMyGuildUnique )
			{
				return GUILDWAR_RESERVE_FAIL_ALREADYRESERVE ;
			}
			else if( m_clUnit[ i ][siRoomIndex].m_siHomeGuildUnique == siEnemyGuildUnique || m_clUnit[ i ][siRoomIndex].m_siAwayGuildUnique == siEnemyGuildUnique )
			{
				return GUILDWAR_RESERVE_FAIL_ENEMY_ALREADYRESERVE ;
			}
		}

	}
	return resultVal;
}