#pragma once


#include "guild.h"
#include "../../common/map/map.h"

#define GUILDWAR_FIELDNUM			5	// 길드전 필드 195~199	->	기존 길드전 필드는 유지시키고 새로운 길드전에서 사용한다
#define GUILDWAR_FIELDNUM_OLD		3	// 길드전 필드 369~371 -> 일반 길드전은 3경기로 축소 되면서 길드전 맵을 옮긴다.
#define	GUILDWAR_ROOMNUM			48				// 30분 간격으로 하루단위로예약
#define	GUILDWAR_MAXCOUNT			3				// 하루에 할수있는 길드전최대 횟수

#define	GUILDWAR_MINLEVEL			4				// 길드전 할수 있는 최소 레벨

#define GUILDWARFIELD_INDEX			195				// 길드전 필드 195~199	->	기존 길드전 필드는 유지시키고 새로운 길드전에서 사용한다
#define GUILDWARFIELD_INDEX_OLD		369				// 길드전 필드 369~371 -> 일반 길드전은 3경기로 축소 되면서 길드전 맵을 옮긴다.

#define GUILDWAR_TIME				10				// 길드워 지속 시간
#define GUILDHUNTWAR_TIME			15				// 사냥터 길드워 지속 시간

#define GUILDWAR_GODTIME			10000			// 사망시 무적시간
#define GUILDWAR_MAX_APPLYCOUNT		3				// 길드워 신청 제한 횟수


#define GUILDWAR_RESERVE_SUCCESS						0x000001	// 같은 시각에 이미 예약을 했다
#define GUILDWAR_RESERVE_FAIL_ALREADYRESERVE			0x000002	// 같은 시각에 이미 예약을 했다
#define GUILDWAR_RESERVE_FAIL_ENEMY_ALREADYRESERVE		0x000004	// 같은 시각에 이미 예약을 했다
#define GUILDWAR_RESERVE_FAIL_ENEMY_OVERTIME			0x000008	// 같은 시각에 이미 예약을 했다

const SI32 Const_HomePos[] = {
	15, 25,
	17, 23,
	21, 35,
	24, 33
};
const SI32 Const_AwayPos[] = {
	23, 17,
	25, 15,
	33, 24,
	35, 21,
};

const inline SI32 GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( SI32 mapindex )	// 변경후
{
	return mapindex - GUILDWARFIELD_INDEX_OLD;	// 새로운 길드전이 들어가면서 기존은 올드가 되서 리턴한다.
}
const inline SI32 GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( SI32 mapindex )	// 변경전
{
	return mapindex - GUILDWARFIELD_INDEX;	// 새로운 길드전이 들어가면서 기존은 올드가 되서 리턴한다.
}
const inline SI32 GetHuntmapGuildWarFieldIndexFromMapIndex( SI32 mapindex )// 길드워 소유방식변경 길드전 맵 확인.
{
	return mapindex - GUILDWARFIELD_INDEX; 
}

inline const bool IsGuildWarMap_Use_New_GuildWar( SI32 mapindex )	/// 변경된 길드전은 이거에서 값을 리턴
{
	if( mapindex >= MAPINDEX_GUILDWARFIELD_OLD && mapindex < MAPINDEX_GUILDWARFIELD_OLD + GUILDWAR_FIELDNUM_OLD )
	{
		return true;
	}
	else
	{
		return false;
	}

}
inline const bool IsGuildWarMap_Not_Use_New_GuildWar( SI32 mapindex )	/// 변경되지 않은 길드전은 이거에서 값을 리턴
{
	if( mapindex >= MAPINDEX_GUILDWARFIELD && mapindex < MAPINDEX_GUILDWARFIELD + GUILDWAR_FIELDNUM )
	{
		return true;
	}
	else
	{
		return false;
	}

}
inline const bool IsGuildWarMap_NEW( SI32 mapindex )	// 새로운 길드전은 이걸로 맵인지 확인.
{
	if( mapindex >= MAPINDEX_GUILDWARFIELD && mapindex < MAPINDEX_GUILDWARFIELD + GUILDWAR_FIELDNUM )
	{
		return true;
	}
	else
	{
		return false;
	}
}


// 길드전 예약 정보
//---------------------------------------------------------------
// cltGuildWarReserveUnit
//---------------------------------------------------------------
class cltGuildWarReserveUnit
{
public:
	SI32		m_siRoomIndex;
	SI32		m_siHomeGuildUnique;
	TCHAR		m_szHomeGuildName[MAX_GUILD_NAME];
	SI32		m_siAwayGuildUnique;
	TCHAR		m_szAwayGuildName[MAX_GUILD_NAME];

	bool		m_bWarStart;

	cltGuildWarReserveUnit(){ Init(); }
	~cltGuildWarReserveUnit(){}

	void Init()
	{
		m_siRoomIndex = 0;
		m_siHomeGuildUnique = 0;
		m_siAwayGuildUnique = 0;
		StringCchCopy( m_szHomeGuildName, MAX_GUILD_NAME, TEXT("") );
		StringCchCopy( m_szAwayGuildName, MAX_GUILD_NAME, TEXT("") );
		m_bWarStart = false;
	}

	void Set( SI32 roomindex, SI32 homeguildunique, TCHAR* homeguildname, SI32 awayguildunique, TCHAR* awayguildname );
	void Set( cltGuildWarReserveUnit* pUnit );

	void WarStart( SI32 fieldindex );

	bool IsStartWar()	{ return m_bWarStart; };		// 시작되어있는가
	bool IsReady();										// 방이 시작될 준비가 되있는가
};


enum{
	GUILDWAR_APPLY_SUCCESS = 1,
	GUILDWAR_APPLY_FAILED_ALREADY_HOME,	// 우리 길드가 오늘 이미 전투했거나 예약되어있다
	GUILDWAR_APPLY_FAILED_ALREADY_AWAY,	// 상대 길드가 오늘 이미 전투했거나 예약되어있다
	GUILDWAR_APPLY_FAILED_FAULTNAME,	// 그런 이름의 길드는 없다.
	GUILDWAR_APPLY_FAILED_MYGUILD,		// 자신의 길드에 신청하려고 했다
	GUILDWAR_APPLY_FAILED_ALREADYAPPLY,	// 이미 예약 신청했음
	GUILDWAR_APPLY_FAILED_LOWLEVEL,		// 레벨이 낮아서 불가
	GUILDWAR_APPLY_FAILED_COUNTOVER,	// 신청횟수(3회) 초과
	GUILDWAR_APPLY_FAILED_UNKNOWN		// 다른 이유에 의해서
};

//---------------------------------------------------------------------
// cltGuildWarReserveManager
//---------------------------------------------------------------------
class cltGuildWarReserveManager
{
public:
	NSafeTArray< NSafeTArray<cltGuildWarReserveUnit, GUILDWAR_ROOMNUM>, GUILDWAR_FIELDNUM>	m_clUnit;	// 바뀌기 전꺼를 위해서 전체 갯수는 줄이지 않는다.

	SI16					m_siToday;			// 현재 날짜를 기억한다.

	cltGuildWarReserveManager();
	~cltGuildWarReserveManager();

	void Init();
	void Set( cltGuildWarReserveManager* pclinfo );

	// 서버
	void ServerAction();
	// 예약 신청한다.
	SI32 ApplyReserve( SI32 fieldindex, SI32 roomindex, SI32 homeguild, SI32 awayguild);

	// 방을 예약 한다.
	bool ReserveRoom( SI32 fieldindex, SI32 roomindex, SI32 homeguild, SI32 awayguild);
	bool IsReservedRoom( SI32 fieldindex, SI32 roomindex );
	SI32 CanReserveRoom( SI32 siFieldIndex, SI32 siRoomIndex , SI32 siMyGuildUnique , SI32 siEnemyGuildUnique );
};


//---------------------------------------------------------------------
// cltGuildWarManager
//---------------------------------------------------------------------
class cltGuildWarManager
{
public:
	// 게임이 시작될때 외부에서 세팅해 주어야 하는 변수들
	SI32		m_siHomeGuildUnique;				// 길드1 유니크
	TCHAR		m_szHomeGuildName[MAX_GUILD_NAME];	// 길드1 이름

	SI32		m_siAwayGuildUnique;				// 길드2 유니크
	TCHAR		m_szAwayGuildName[MAX_GUILD_NAME];	// 길드2 이름

	NSafeTArray<SI32, MAX_GUILD_MEMBER_NUM>		m_siHomePersonID;
	NSafeTArray<SI32, MAX_GUILD_MEMBER_NUM>		m_siAwayPersonID;

	NSafeTArray<SI16, MAX_GUILD_MEMBER_NUM>		m_siHomePersonDrinkPutionCnt;
	NSafeTArray<SI16, MAX_GUILD_MEMBER_NUM>		m_siAwayPersonDrinkPutionCnt;

	NSafeTArray<clock_t, MAX_GUILD_MEMBER_NUM>	m_HomePersonDeadClock;
	NSafeTArray<clock_t, MAX_GUILD_MEMBER_NUM>	m_AwayPersonDeadClock;

	bool		m_bStartedWar;						// 지금 전쟁중인지 여부
	bool		m_bCheckTenSecond;					// 시작하고 십초 가 지났는지 체크 플레그

	clock_t		m_WarStartClock;

	SI32		m_siHomeGuildKill;					// 길드1 죽인숫자
	SI32		m_siAwayGuildKill;

	bool		m_bEndWar;
	clock_t		m_WarEndClock;

	cltGuildWarManager();
	~cltGuildWarManager();

	// 깨끗하게 지워주는 함수. 게임이 시작될때 호출해주고 변수를 세팅해야한다.
	void Init();

	// 변수가 모두 세팅된 상태에서 호출해 주는 함수. 변수값이 정상적으로 세팅안되어있으면 바로 끝낸다.
	bool StartGame();

	// 외부에서 전쟁 멤버를 추가할때 호출해준다.
	bool AddMember(SI32 personID, SI32 guildUnique );

	// 전쟁 멤버가 나갈때 호출해 준다.
	bool RemoveMember(SI32 personID);

	// 상대 유저를 공격할수 있는지 여부
	bool CanAttack(SI32 myGuildUnique, SI32 targetGuildUnique);

	// TODO : char-item 에서는 길드전중 물약을 먹으면 이 함수를 호출해줌
	bool DrinkPotion(SI32 personID);

	// 물약을 먹는데 제한을 둔다.
	bool CanDrinkPotion(SI32 personID);

	// 길드전쟁중 죽었을 경우 따로 처리를 해준다.
	bool GuildWarDie(SI32 personID);
	
	void Action();	        

	// 길드전쟁이 끝날때 각종 보상 등을 처리해주는 함수. 보상이후 값을 초기화해준다.
	void GuildWarEnd();

	// 죽인 숫자를 올려준다.
	void AddKillPoint( SI32 personId );

	// 전쟁 종료후 10초가 지나면 지정된 위치로 보낸다.
	void WarpByEnd();

	// 적대 관계로 설정한다.
	void SetEnemy();

	// 무적인지 
	bool IsGod( SI32 personID );
};

class cltGuildWarClient
{
public:
	bool	m_bGuildWarSwitch;
	TCHAR	m_szHomeGuildName[MAX_GUILD_NAME];
	TCHAR	m_szAwayGuildName[MAX_GUILD_NAME];
	SI32	m_siHomeGuildPoint;
	SI32	m_siAwayGuildPoint;

	SI16	m_siHomeGuildMarkIndex;
	SI16	m_siAwayGuildMarkIndex;

	clock_t	m_noticeClock;
	SI32	m_bDrawFlag;

	clock_t m_endClock;

	cltGuildWarClient();
	void	Init();
	bool	IsStartGuildWar()				{ return m_bGuildWarSwitch; };			// 길드전이 진행중인가
	void	StartGuildWar( TCHAR* homename, TCHAR* awayname, SI16 homemark, SI16 awaymark );
	void	EndGuildWar( SI32 recvGP );
	void	SetHomeKillPoint( SI32 point )	{ m_siHomeGuildPoint = point; };
	void	SetAwayKillPoint( SI32 point )	{ m_siAwayGuildPoint = point; };
	SI32	GetHomeKillPoint()				{ return m_siHomeGuildPoint; };
	SI32	GetAwayKillPoint()				{ return m_siAwayGuildPoint; };
	void	DrawStatus(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);
};


