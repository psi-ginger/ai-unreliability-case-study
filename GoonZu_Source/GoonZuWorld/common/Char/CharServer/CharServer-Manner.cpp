//---------------------------------
// 2004/9/1 김태곤
//---------------------------------
#include "..\..\Server\Server.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"


extern cltCommonLogic* pclClient;

// 감옥에 보낸다. 
bool cltCharServer::SendPrison()
{

	// 감옥에 있어야 할 남은 시간이 있어야 감옥으로 보낼 수 있다. 
	if(pclCI->clGuiltyInfo.siLeftPrisonSecond <= 0)return false;

	clInnerStatus.SetStatus(INNERSTATUS_PRISON);

	// 감옥 위치로 공간 이동 시킨다. 
	cltWarpInfo clWarpInfo(1, PRISON_MAPINDEX, true, false, PRISON_X, PRISON_Y, -1, -1);	// 감옥으로 워프 
	SetWarpInfo(&clWarpInfo, 5032);
	// 실제로 워프 시킨다. 
	Warp();

	//// 감옥 영역 안에 있으면, 
	//if(GetMapIndex() == 0 && GetX() >= 376 && GetX() <= 383 && GetY() >= 500 && GetY() <= 507)
	//{
	//}
	//else
	//{	
	//	// 감옥 위치로 공간 이동 시킨다. 
	//	cltWarpInfo clWarpInfo(1, PRISON_MAPINDEX, true, false, PRISON_X, PRISON_Y, -1, -1);	// 감옥으로 워프 
	//	SetWarpInfo(&clWarpInfo, 5032);
	//	// 실제로 워프 시킨다. 
	//	Warp();

	//}

	return true;
}


// 자동 사냥인지 여부를 확인한다. 
bool cltCharServer::IsAutoHunting()
{
	// 마지막으로 사냥한지 20초이상 경과했다면, 
	if(pclClient->CurrentClock - clSimpleVary.dwLastKillClock > 20000)return false;
	
	// 마을에 있는 것이라면, 
	if ( !pclClient->pclMapManager->IsEncountMap( GetMapIndex() ) ) return false ;

	// 경복궁에 있는 것이라면, 
	if(GetMapIndex() == MAPINDEX_PALACE)return false;
//	if(GetMapIndex() == MAPINDEX_CHINABORDER)return false;

	// 길드전 맵에 있는것이라면
	if( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경전
	{
		if( IsGuildWarMap_Use_New_GuildWar( GetMapIndex() ) )
			return false;
		// 새로운 길드전 중이라면
		if( IsGuildWarMap_NEW( GetMapIndex() ) )
			return false;
	}
	else	// 변경후
	{
		if( IsGuildWarMap_Not_Use_New_GuildWar( GetMapIndex() ) )
			return false;
	}
	// 감옥에 갇혀 있는 것이라면, 
	if(clInnerStatus.IsStatus(INNERSTATUS_PRISON))return  false;

	return true;
}

// 스틸한 횟수를 얻어온다. 
SI16 cltCharServer::GetStealNum()
{
	return siStealNum;
}

