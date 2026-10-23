#ifndef _MSGTYPEHUNT_H
#define _MSGTYPEHUNT_H

#include "../../DBManager/GameDBManager_World/DBMsg-Hunt.h"

#include "../../common/Map/GateManager/InstanceMapGateManager.h"

//-----------------------------------------
// 수렵장에 입장할 것을 요청. 
//------------------------------------------
class cltGameMsgRequest_EnterHunt
{
public:
	SI16 siVillageUnique;	// 대상이 되는 마을의 유니크. 

	cltGameMsgRequest_EnterHunt(SI16 villageunique)
	{
		siVillageUnique = villageunique;
	}
};

class cltGameMsgRequest_HuntMonsterFieldListAdd
{
public:
	SI32 siVillageUnique;
	SI32 siMonsterKind;
	SI16 siMonsterLevel;
	SI16 siMonsterNum;

	cltGameMsgRequest_HuntMonsterFieldListAdd( 
		SI32 VillageUnique, SI32 MonsterKind, SI16 MonsterLevel, SI16 MonsterNum )
	{
		siVillageUnique = VillageUnique;
		siMonsterKind = MonsterKind;
		siMonsterLevel = MonsterLevel;
		siMonsterNum = MonsterNum;
	}	
};

class cltGameMsgResponse_HuntMonsterFieldListAdd
{
public:
	SI32 siVillageUnique;
	SI16 siAddListIndex;
	SI32 siMonsterKind;
	SI16 siMonsterNum;
	SI16 siMonsterLevel;

	cltGameMsgResponse_HuntMonsterFieldListAdd
		(
		SI32 VillageUnique,
		SI16 AddListIndex,
		SI32 MonsterKind,
		SI16 MonsterNum,
		SI16 MonsterLevel
		)
	{
		siVillageUnique = VillageUnique; 
		siAddListIndex = AddListIndex;
		siMonsterKind = MonsterKind;
		siMonsterNum = MonsterNum;
		siMonsterLevel = MonsterLevel;
	}
};

class cltGameMsgRequest_HuntMonsterFieldListDel
{
public:
	SI32 siVillageUnique;
	SI16 siDelListIndex;

	cltGameMsgRequest_HuntMonsterFieldListDel( 
		SI32 VillageUnique, SI16 DelListIndex )
	{
		siVillageUnique = VillageUnique;
		siDelListIndex = DelListIndex;
	}
};

class cltGameMsgResponse_HuntMonsterFieldListDel
{
public:
	SI32 siVillageUnique;
	SI16 siDelListIndex;

	cltGameMsgResponse_HuntMonsterFieldListDel( 
		SI32 VillageUnique, SI16 DelListIndex )
	{
		siVillageUnique = VillageUnique;
		siDelListIndex = DelListIndex;
	}
};


//-----------------------------------------
// 수렵장 창고에 몬스터 넣기
//------------------------------------------
class cltGameMsgRequest_HuntMonsterIn
{
public:
	SI32	siVillageUnique;
	SI32	siItemUnique;
	SI16	siItemNum;
	SI16	siFromPersonInventoryItemIndex;
	
	cltGameMsgRequest_HuntMonsterIn
		(
		SI32 VillageUnique,
		SI16 FromPersonInventoryItemIndex,
		SI32 ItemUnique,
		SI16 ItemNum
		)
	{
		siVillageUnique = VillageUnique;
		siFromPersonInventoryItemIndex = FromPersonInventoryItemIndex;
		siItemUnique = ItemUnique;
		siItemNum = ItemNum;
	}
};

class cltGameMsgResponse_HuntMonsterIn
{
public:
	SI32	siVillageUnique;
	cltItem	clItem;
	SI16	siUserInventoryIndex;

	SI16	siHuntStorageItemIndex;
	SI32	siHuntStorageItemUnique;
	SI16	siHuntStorageItemNum;

	cltGameMsgResponse_HuntMonsterIn(
		SI32 VillageUnique,
		cltItem *pclItem,
		SI16 UserInventoryIndex,
		SI16 HuntStorageItemIndex,
		SI32 HuntStorageItemUnique,
		SI16 HuntStorageItemNum
		)
	{
		siVillageUnique = VillageUnique;
		clItem.Set( pclItem );
		siUserInventoryIndex = UserInventoryIndex;
		siHuntStorageItemIndex = HuntStorageItemIndex;
		siHuntStorageItemUnique = HuntStorageItemUnique;
		siHuntStorageItemNum = HuntStorageItemNum;
	}
};

//-----------------------------------------
// 수렵장 창고에 몬스터 빼기
//------------------------------------------
class cltGameMsgRequest_HuntMonsterOut
{
public:
	SI32	siVillageUnique;
	SI32	siItemUnique;
	SI16	siItemNum;
	SI16	siFromHuntStorageItemIndex;
	
	cltGameMsgRequest_HuntMonsterOut
		(
		SI32 VillageUnique,
		SI16 FromHuntStorageItemIndex,
		SI32 ItemUnique,
		SI16 ItemNum
		)
	{
		siVillageUnique = VillageUnique;
		siFromHuntStorageItemIndex = FromHuntStorageItemIndex;
		siItemUnique = ItemUnique;
		siItemNum = ItemNum;
	}
};

class cltGameMsgResponse_HuntMonsterOut
{
public:
	SI32	siVillageUnique;
	cltItem	clItem;
	SI16	siUserInventoryIndex;

	SI16	siHuntStorageItemIndex;
	SI32	siHuntStorageItemUnique;
	SI16	siHuntStorageItemNum;

	cltGameMsgResponse_HuntMonsterOut(
		SI32 VillageUnique,
		cltItem *pclItem,
		SI16 UserInventoryIndex,
		SI16 HuntStorageItemIndex,
		SI32 HuntStorageItemUnique,
		SI16 HuntStorageItemNum
		)
	{
		siVillageUnique = VillageUnique;
		clItem.Set( pclItem );
		siUserInventoryIndex = UserInventoryIndex;
		siHuntStorageItemIndex = HuntStorageItemIndex;
		siHuntStorageItemUnique = HuntStorageItemUnique;
		siHuntStorageItemNum = HuntStorageItemNum;
	}
};

#define MAX_PAGE_COUNT 10
//-----------------------------------------
// 인스턴스 던전의 명예의 전당 페이지 변환.
//------------------------------------------
class cltGameMsgRequest_HOFList
{
public:
	SI32	siRequestPage;

	cltGameMsgRequest_HOFList( SI32 RequestPage)
	{
		siRequestPage = RequestPage;
	}
};

class cltGameMsgResponse_HOFList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<HOFList, MAX_HOF_PAGE>		stHOFList;
#else
	HOFList		stHOFList[MAX_HOF_PAGE];
#endif
	SI16		siCurPage;

	SI16		siMaxPage;

	cltGameMsgResponse_HOFList( HOFList* pHOFList, SI16 CurPage, SI16 MaxPage )
	{
		for( SI32 i = 0; i < MAX_HOF_PAGE; ++i )
		{
			stHOFList[i].uiYear				= pHOFList[i].uiYear;			
			stHOFList[i].siGuildMarkindex	= pHOFList[i].siGuildMarkindex;
			MStrCpy( stHOFList[i].szHOFGuildName, pHOFList[i].szHOFGuildName, sizeof(stHOFList[i].szHOFGuildName) );
			stHOFList[i].siClearPoint		= pHOFList[i].siClearPoint;
			stHOFList[i].ClearTime			= pHOFList[i].ClearTime;
			stHOFList[i].ClearDateTime		= pHOFList[i].ClearDateTime;
		}
		siCurPage = CurPage;
		siMaxPage = MaxPage;
	}
};
//-----------------------------------------
// 인스턴스 던전의 랭킹.
//------------------------------------------
class cltGameMsgRequest_RankingList
{
public:
	UI08				uiYear;			// 해당 년도 (게임시간)
	SI16				siPageNum;		// 요청 페이지

	cltGameMsgRequest_RankingList( UI08 Year, SI16 page )
	{
		uiYear = Year;
		siPageNum = page;
	}
};

class cltGameMsgResponse_RankingListt
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<HOFList, MAX_HOF_PAGE>		stHOFList;
#else
	HOFList		stHOFList[MAX_HOF_PAGE];
#endif

	SI16		siPageNum;
	SI16		siMaxPageNum;

	cltGameMsgResponse_RankingListt( HOFList* pHOFList, SI16 pagenum, SI16 maxpagenum)
	{
		for( SI32 i = 0; i < MAX_HOF_PAGE; ++i )
		{
			stHOFList[i].siGuildMarkindex = pHOFList[i].siGuildMarkindex;
			MStrCpy( stHOFList[i].szHOFGuildName, pHOFList[i].szHOFGuildName, sizeof(stHOFList[i].szHOFGuildName) );
			stHOFList[i].siClearPoint= pHOFList[i].siClearPoint;
			stHOFList[i].ClearTime = pHOFList[i].ClearTime;
			stHOFList[i].ClearDateTime = pHOFList[i].ClearDateTime;
		}

		siPageNum		= pagenum;
		siMaxPageNum	= maxpagenum;
	}
};

#define MAX_INSTANCEDUNGEON_STATUS	6

class cltGameMsgRequest_InstantsDungeonStatus
{
public:

	cltGameMsgRequest_InstantsDungeonStatus()
	{

	}
};

// [추가 : 황진성 2008. 1. 10 => 인던 현재 상태정보를 보내줄 패킷]
class cInstantsDungeonStatusBaseInfo
{
public:
	TCHAR	szGuildName[20];
	SI32	siGuildMarkIndex;
	DWORD	siRemainTime;		// ms
	SI32	siScore;

	cInstantsDungeonStatusBaseInfo()
	{
		ZeroMemory(this, sizeof(cInstantsDungeonStatusBaseInfo));
	}
	cInstantsDungeonStatusBaseInfo(TCHAR* _pGuildName, SI32 _GuildMarkIndex, DWORD _RemainTime, SI32 _Score)
	{
		ZeroMemory(this, sizeof(cInstantsDungeonStatusBaseInfo));

		memcpy(szGuildName, _pGuildName, sizeof(szGuildName));	
		siGuildMarkIndex = _GuildMarkIndex;
		siRemainTime	 = _RemainTime;		
		siScore			 = _Score;
	}
};

class cltGameMsgResponse_InstantsDungeonStatus
{
public:
	cInstantsDungeonStatusBaseInfo	clGuildInfo[MAX_INSTANCEDUNGEON_STATUS];

	cltGameMsgResponse_InstantsDungeonStatus()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_InstantsDungeonStatus));
	}
	cltGameMsgResponse_InstantsDungeonStatus(cInstantsDungeonStatusBaseInfo* _GuildInfo)
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_InstantsDungeonStatus));
		memcpy(clGuildInfo, _GuildInfo, sizeof(clGuildInfo));
	}
};

enum
{
	DUNGEONRUN_START = 1,
	DUNGEONRUN_UPDATE,
	DUNGEONRUN_END,
};

// [추가 : 황진성 2008. 1. 10 => 인던 진행정보를 보내줄 패킷]
class cltGameMsgResponse_cInstantsDungeonRunInfo
{
public:
	DWORD	dwTime;
	SI32	siScore;
	SI32	siBestScore;
	
	SI08	siMode;

	SI16	siMapType;

	cltGameMsgResponse_cInstantsDungeonRunInfo(DWORD _Time, SI32 _Score, SI32 _BestScore, SI08 mode, SI16 _siMapType)
	{
		siMode		= 0;
		dwTime      = _Time;
		siScore		= _Score;
		siBestScore = _BestScore;
		siMode		= mode;

		siMapType	= _siMapType;
	}
};

#define MAX_INSTANCEMAPGATE_COUNT	80

class cltGameMsgResponse_InstanceMap_GateInfo
{
public:
	SI16				siCount;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltInstanceMapGate, MAX_INSTANCEMAPGATE_COUNT>		clGates;
#else
	cltInstanceMapGate	clGates[MAX_INSTANCEMAPGATE_COUNT];
#endif

	cltGameMsgResponse_InstanceMap_GateInfo( cltInstanceMapGate* pclGates, SI32 count)
	{
		siCount = count;
		if( siCount > MAX_INSTANCEMAPGATE_COUNT )	siCount = MAX_INSTANCEMAPGATE_COUNT;

		for( SI32 i=0; i<siCount; i++ )
		{
			clGates[i].Set(&pclGates[i]);
		}
	}

	// 가변 패킷
	SI32 GetSize()
	{
		return sizeof(SI16) + (sizeof(cltInstanceMapGate) * siCount);
	}
};

// [추가 : 황진성 2008. 1. 11 => 인던 보상정보 보내줄 패킷]
class cltGameMsgResponse_InstantDungeonReward
{
public:
	TCHAR	szGuildName[20];
	SI32	siGuildMarkIndex;
	TCHAR	szGuildMasteName[MAX_PLAYER_NAME];

	cltGameMsgResponse_InstantDungeonReward()
	{

	}
	cltGameMsgResponse_InstantDungeonReward(TCHAR* _pGuildName, SI32 _GuildMarkIndex, const TCHAR* _GuildMasterName)
	{
		ZeroMemory(szGuildName, sizeof(szGuildName));
		if(_pGuildName)					memcpy(szGuildName, _pGuildName, sizeof(szGuildName));	

		siGuildMarkIndex = _GuildMarkIndex;

		ZeroMemory(szGuildMasteName, sizeof(szGuildMasteName));
		if(_GuildMasterName != NULL)	memcpy(szGuildMasteName, _GuildMasterName, sizeof(szGuildMasteName));
	}
};

// [추가 : 황진성 2008. 1. 11 => 인던 보상정보 보내줄 패킷]
class cltGameMsgResponse_InstantDungeon_Top_Change
{
public:
	TCHAR	szGuildName[20];
	SI32	siGuildMarkIndex;
	SI32	siScore;
	TCHAR	szGuildMasteName[MAX_PLAYER_NAME];

	cltGameMsgResponse_InstantDungeon_Top_Change()
	{

	}
	cltGameMsgResponse_InstantDungeon_Top_Change(TCHAR* _pGuildName, SI32 _GuildMarkIndex, SI32 _siScore, TCHAR* _GuildMasterName)
	{
		siGuildMarkIndex = 0;
		siScore			 = 0;
		ZeroMemory(szGuildName, sizeof(szGuildName));
		ZeroMemory(szGuildMasteName, sizeof(szGuildMasteName));

		if(_pGuildName)			memcpy(szGuildName, _pGuildName, sizeof(szGuildName));	
		siGuildMarkIndex = _GuildMarkIndex;
		siScore			 = _siScore;
		if(_GuildMasterName)	memcpy(szGuildMasteName, _GuildMasterName, sizeof(szGuildMasteName));	
	}
};

// [영훈] 해적선 : 파티인던멥 클리어해서 어떠한 아이템을 받았다는 메시지
class cltGameMsgResponse_InstantDungeon_Party_Clear
{
public:
	TCHAR	tszUserName[MAX_PLAYER_NAME];
	SI32	siGetItemUnique;


	cltGameMsgResponse_InstantDungeon_Party_Clear( TCHAR* ptszUserName, SI32 siItemUnique )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_InstantDungeon_Party_Clear) );

		StringCchCopy( tszUserName, MAX_PLAYER_NAME, ptszUserName );

		siGetItemUnique = siItemUnique;
	}
};

// [영훈] 서버이벤트 인던 : 현재 상황 보내기
class cltGameMsgResponse_Server_Event_InstanceDungeon_RunInfo
{
public:
	SI16	m_siServerUnique;	// 현재 서버 유니크

	SI32	m_siClearStep;		// 현재 클리어 단계

	DWORD	m_dwRemainTime;		// 현재 남은 시간
	SI32	m_siRemainMonster;	// 현재 남은 몬스터

	SI64	m_siScore;			// 현재 내 서버 점수
	SI32	m_siDungeonScore;	// 현재 인던 스코어

	cltGameMsgResponse_Server_Event_InstanceDungeon_RunInfo( SI16 siServerUnique, SI32 siClearStep, DWORD dwRemainTime, SI32 siRemainMonster, SI64 siScore, SI32 siDungeonScore )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Server_Event_InstanceDungeon_RunInfo) );

		m_siServerUnique	= siServerUnique;

		m_siClearStep		= siClearStep;

		m_dwRemainTime		= dwRemainTime;
		m_siRemainMonster	= siRemainMonster;

		m_siScore			= siScore;
		m_siDungeonScore	= siDungeonScore;
	}
};

#endif