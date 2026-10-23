#ifndef _GUILDHUNTMAPWARMANAGER_H
#define _GUILDHUNTMAPWARMANAGER_H

#include "../../Common/CommonHeader.h"
#include "../Guild//Guild.h"

#define MAX_GUILDHUNTWAR_MAPNUM		5	// 길드전맵 최대 갯수	-한번에 열리는 최대 길드전횟수
#define GUILDHUNTWAR_GUILDLEV			4	// 길드전 최소 길드 레벨
#define MAx_WARP_TIMETABLE_MINUTE		3
#define MAx_ROOF_TIMETABLE_SECOND		2
#define MAX_INMAPPERSONNUM				32
#define GUILDWAR_WINPOINT				50
#define GUILDWAR_LOSEPOINT				10
#define GUILDWAR_DRAWPOINT				20
#define GUILDHUNTMAPGODTIME				10000
enum
{
	GUILDWAR_HOME = 0,
	GUILDWAR_AWAY,
};
// 길드전 신청 했을떄 메세지.
enum
{
	SUCCEED_RWQUEST_LIST	=	0,			// 신청할 빈자리가 없다.
	SAMEGUILD_REQUEST_LIST	,			// 이미 신청 했다.
	WRONGGUILDUNIQUE_REQUEST_LIST	,	// 잘못된 길드 유니크다.
	FULL_REQUEST_LIST,				// 정상적으로 길드전 리스트에 등록되었다,
};
// 홈길드 인지 아닌지
enum
{
	NOT_FIND_LIST	=	0,	// 리스트에 없음
	GUILDWAR_HOMEGUILD	,		// 홈길드
	GUILDWAR_AWAYGUILD	,		// 어웨이 길드
};
enum
{
	EMPTY_MATCHING_LIST	=	0,	// 리스트에 없음
	MAKE_MATCHING_LIST	,		// 매칭리스트 완성
	GUILDWAR_READY	,			// 길드워를 준비한다.
	GUILDWAR_START	,			// 길드워를 시작 한다.
	GUILDWAR_END	,			// 길드전을 종료한다
	GUILDWAR_END_WAIT	,			// 길드전을 종료한다
	GUILDWAR_CLEANUP,			// 길드전 종료후 할일들을 한다.
};
enum
{
	GUILDWAR_END_WIN	=	0,	// 승 무 패 
	GUILDWAR_END_DRAW,
	GUILDWAR_END_LOSE,
	GUILDWAR_END_WIN_NOWARP,	// 승 무 패 
	GUILDWAR_END_DRAW_NOWARP,
	GUILDWAR_END_LOSE_NOWARP,
	GUILDWAR_END_NORESULT,

};
class cltWarSrarus
{
public:
	cltWarSrarus()
	{ 
		Init();
	}
	~cltWarSrarus()
	{
	}
	void Init()
	{
		siGuildUnique	=	0	;	// 소속길드 유니크
		siPersonID		=	0	;	// 케릭터 personID
		siKillNum		=	0	;	// 상대방을 죽인 숫자
		siDieNum		=	0	;	// 죽은 숫자.
	}
	void SetDeata( SI32 GuildUnique,SI32 PersonID, SI32 KillNum, SI32 DieNum)
	{
		siGuildUnique	=	GuildUnique	;	// 소속길드 유니크
		siPersonID		=	PersonID	;	// 케릭터 personID
		siKillNum		=	KillNum	;	// 상대방을 죽인 숫자
		siDieNum		=	DieNum	;	// 죽은 숫자.
	}
	void SetGuildUnique( SI32 GuildUnique )
	{
		siGuildUnique	=	GuildUnique	;	// 소속길드 유니크
	}
	void SetGuildPersonID( SI32 PersonID )
	{
		siPersonID		=	PersonID	;	// 케릭터 personID
	}
	SI32 GetGuildUnique()
	{
		return siGuildUnique	;
	}
	SI32 GetPerosonID()
	{
		return siPersonID	;
	}
	SI32 GetKillPoint()
	{
		return siKillNum;
	}
	SI32 GetiDieNum()
	{
		return siDieNum	;
	}
	void AddKillPoint()
	{
		siKillNum = siKillNum	+	1	;
	}
	void AddDiePoint()
	{
		siDieNum = siDieNum		+	1	;
	}
private:
	SI32 siGuildUnique	;	// 소속길드 유니크
	SI32 siPersonID		;	// 케릭터 personID
	SI32 siKillNum		;	// 상대방을 죽인 숫자
	SI32 siDieNum		;	// 죽은 숫자.

};
class cltEndWarSrarus
{
private:
	TCHAR szGuildName[MAX_GUILD_NAME]	;	// 소속길드 유니크
	TCHAR szPlayerName[MAX_PLAYER_NAME]		;	// 케릭터 personID
	SI32 siKillNum		;	// 상대방을 죽인 숫자
	SI32 siDieNum		;	// 죽은 숫자.

public:
	cltEndWarSrarus()
	{ 
		Init();
	}
	~cltEndWarSrarus()
	{
	}
	void Init()
	{
		siKillNum		=	0	;	// 상대방을 죽인 숫자
		siDieNum		=	0	;	// 죽은 숫자.
		MStrCpy(szGuildName,		TEXT(""),		MAX_GUILD_NAME);
		MStrCpy(szPlayerName,		TEXT(""),		MAX_PLAYER_NAME );
	}
	void SetDeata( SI32 GuildUnique,SI32 PersonID, SI32 KillNum, SI32 DieNum)
	{
		siKillNum		=	KillNum	;	// 상대방을 죽인 숫자
		siDieNum		=	DieNum	;	// 죽은 숫자.
	}
	void SetGuildName( TCHAR* Guildname)
	{
		MStrCpy(szGuildName,Guildname,sizeof(szGuildName));
	}
	void SetPersonName( TCHAR* PlayerName)
	{
		MStrCpy(szPlayerName,PlayerName,sizeof(szPlayerName));
	}
	TCHAR* GetGuildName()
	{
		return szGuildName	;
	}
	TCHAR* GetPlayer()
	{
		return szPlayerName	;
	}
	SI32 GetKillPoint()
	{
		return siKillNum;
	}
	SI32 GetiDieNum()
	{
		return siDieNum	;
	}
	void SetKillPoint( SI32 KillPoint )
	{
		siKillNum = KillPoint	;
	}
	void SetDiePoint( SI32 DiePoint )
	{
		siDieNum = DiePoint	;
	}
};
class cltPoint	// 길드전 에서 얻는 포인트
{
public:
	SI32 siKillPoint;
	cltPoint()
	{
		init();
	}
	void init()
	{		
		siKillPoint = 0	;
	}
	void SetKillPoint(SI32 sipoint)
	{
		siKillPoint = siKillPoint + sipoint;
	}
};
// 랭킹 데이터
class cltGuildHuntMapWarListRankData
{
private:	
public:
	SI32 siGuildUnique	;
	TCHAR szGuildName[MAX_GUILD_NAME];
	SI32 siWin	;
	SI32 siDraw	;
	SI32 siLose	;
	SI32 siBGP	;
	cltGuildHuntMapWarListRankData()
	{
		Init();
	}
	~cltGuildHuntMapWarListRankData()
	{

	}
	void Init()
	{
		siGuildUnique	=	0;
		MStrCpy(szGuildName,		TEXT(""),		MAX_GUILD_NAME);
		siWin			=	0;
		siDraw			=	0;
		siLose			=	0;
		siBGP			=	0;
	}
	void SetGuildUnique(SI32 siUnique)	{ siGuildUnique = siUnique; }
	SI32 GetGuildUnique(){return siGuildUnique; }

	void SetWin(SI32 Win)	{ siWin = Win; }
	SI32 GetWin(){return siWin; }
	void SetDraw(SI32 Draw)	{ siDraw = Draw; }
	SI32 GetDraw(){return siDraw; }
	void SetLose(SI32 Lose)	{ siLose = Lose; }
	SI32 GetLose(){return siLose; }
	void SetBGP(SI32 BGP)	{ siBGP = BGP; }
	SI32 GetBGP(){return siBGP; }
	TCHAR* GetGuildName()
	{
		return szGuildName	;
	}
	void SetGuildName( TCHAR* Guildname)
	{
		MStrCpy(szGuildName,Guildname,sizeof(szGuildName));
	}

};
class cltGuildHuntMapWarListData
{
public:
	SI32 siGuildUnique;

	cltGuildHuntMapWarListData()
	{
		Init();
	}
	~cltGuildHuntMapWarListData()
	{

	}
	void Init()
	{	
		siGuildUnique =	0;	
	}
	void SetGuildUnique(SI32 siUnique)	{ siGuildUnique = siUnique; }
	SI32 GetGuildUnique(){return siGuildUnique; }
	bool IsSameGuild( SI32 siUnique )
	{ 
		if ( siUnique != 0 && siUnique == siGuildUnique )
		{
			return true	;
		}

		return false	;
	}
	bool IsEmpty()
	{
		if ( siGuildUnique == 0 )	return	true	;
		return false;
	}
};
class cltGuildHuntMapWarMatchingData
{
private:
	SI32 siGuidlUnique	;
public:
	cltGuildHuntMapWarMatchingData()
	{
		siGuidlUnique	=	0;

	}
	~cltGuildHuntMapWarMatchingData()
	{

	}
	void Init()
	{
		siGuidlUnique	=	0	;
	}
	void SetGuild(SI32 siGuildUnique )
	{
		siGuidlUnique	=	siGuildUnique	;
	}
	SI32 GetGuildUniqeue()
	{
		return siGuidlUnique	;
	}
	bool IsSameGuild(SI32 GuildUnique )
	{
		bool bresult = false ;
		if(GuildUnique == GetGuildUniqeue() )
		{
			bresult = true	;
		}
		return bresult	;
	}

};
class cltGuildHuntMapWarManager
{
private:
#if _SAFE_MEMORY
	//--------------------------------------------------------------------------------------------------------------------------------------
	NSafeTArray<cltGuildHuntMapWarListData, MAX_GUILD_NUM>		pclGuildList				;	//	길드전 신청	리스트
	//--------------------------------------------------------------------------------------------------------------------------------------
	NSafeTArray<cltGuildHuntMapWarMatchingData, MAX_GUILDHUNTWAR_MAPNUM>		pclGuildMatchingListHome	;	//	길드전 매칭	리스트
	NSafeTArray<cltGuildHuntMapWarMatchingData, MAX_GUILDHUNTWAR_MAPNUM>		pclGuildMatchingListAway	;	//	길드전 매칭	리스트

	NSafeTArray<cltGuildHuntMapWarMatchingData, MAX_GUILDHUNTWAR_MAPNUM>		pclGuildMatchingListHomeTemp	;	//	길드전 매칭	리스트-사전에 만들어 둔다
	NSafeTArray<cltGuildHuntMapWarMatchingData, MAX_GUILDHUNTWAR_MAPNUM>		pclGuildMatchingListAwayTemp	;	//	길드전 매칭	리스트-사전에 만들어 둔다
	//--------------------------------------------------------------------------------------------------------------------------------------
	NSafeTArray<cltPoint, MAX_GUILDHUNTWAR_MAPNUM>								pclKillPointHome			;	//	길드전 킬 포인트
	NSafeTArray<cltPoint, MAX_GUILDHUNTWAR_MAPNUM>								pclKillPointAway			;	//	길드전 킬 포인트
	//--------------------------------------------------------------------------------------------------------------------------------------
#else
	//--------------------------------------------------------------------------------------------------------------------------------------
	cltGuildHuntMapWarListData				pclGuildList[MAX_GUILD_NUM]			;		//	길드전 신청	리스트
	//--------------------------------------------------------------------------------------------------------------------------------------
	cltGuildHuntMapWarMatchingData			pclGuildMatchingListHome[MAX_GUILDHUNTWAR_MAPNUM]	;		//	길드전 매칭	리스트 홈
	cltGuildHuntMapWarMatchingData			pclGuildMatchingListAway[MAX_GUILDHUNTWAR_MAPNUM]	;		//	길드전 매칭	리스트 어웨이

	cltGuildHuntMapWarMatchingData			pclGuildMatchingListHomeTemp[MAX_GUILDHUNTWAR_MAPNUM]	;		//	길드전 매칭	리스트 홈-사전에 만들어 둔다.
	cltGuildHuntMapWarMatchingData			pclGuildMatchingListAwayTemp[MAX_GUILDHUNTWAR_MAPNUM]	;		//	길드전 매칭	리스트 어웨이-사전에 만들어 둔다
	//--------------------------------------------------------------------------------------------------------------------------------------
	cltPoint								pclKillPointHome[MAX_GUILDHUNTWAR_MAPNUM]				;		//	킬 포인트
	cltPoint								pclKillPointAway[MAX_GUILDHUNTWAR_MAPNUM]				;
	//--------------------------------------------------------------------------------------------------------------------------------------
#endif
	cltWarSrarus							pclWarStatus_Home[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]	;
	cltWarSrarus							pclWarStatus_Away[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]	;
	cltEndWarSrarus							pclEndWarStatus_Home[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]	;
	cltEndWarSrarus							pclEndWarStatus_Away[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]	;
	clock_t									m_HomePersonDeadClock[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]				;		//	킬 포인트
	clock_t									m_AwayPersonDeadClock[MAX_GUILDHUNTWAR_MAPNUM][MAX_GUILD_MEMBER_NUM]				;

	SI32	siListCount	;					// 리스트에 있는 길드의 수
	SI32	siStatus	;					// 현제 상태 변수
public:
	cltGuildHuntMapWarManager();
	~cltGuildHuntMapWarManager();
	void init()	;								// 길드전 리스트를 초기화 한다.

	void SetStatus(SI32 siStatus);				// 상태변수 변경
	SI32 GetStatus()	;						// 현제 상태확인

	// 길드전 신청 리스트
	SI32 add( SI32 siGuildQunique )	;			// 길드전신청 리스트에 길드를 추가 한다.
	bool Delete( SI32 siGuildQunique )	;		// 길드전신청 리스트에 있는 길드를 제거 한다.
	bool IsRequest( SI32 siGuildQunique )	;	// 해당 길드 유니크의 길드가 신청 되었는지 확인 한다.
	SI32 GetRequestCount()	;					// 현제 까지 신청된 길드의 갯수를 가지고 온다
	bool IsSameGuild(SI32 siUnique)	;			// 길드전신청 리스트에 같은 길드가 있는지 검사.
	void MixList()	;							// 길드전 신청 리스트를 섞는다.
	//void ArrangeTempList();						// temp리스트를 정리한다.
	//void ArrangeGuildList();					// 길드전 신청 리스트를 정리 한다.
	void MakeMatchingList();					// 매칭 리스트를 만든다.

	//길드전 매칭 리스트
	bool CanMakeMathingList();					// 길드전 매칭리스트를 만들수 있는지 확인 한디.
	void ListClear();							// 리스트를 모두 지운다.
	SI32 FindSameGuildUnique(SI32 siGuildUnique );					// 기존의 매칭 리스트에서 같은 길드가 있는지 확인 한다.
	bool CopyTempMatchinfList();				// temp리스트에 있는걸 본으로 옮긴다.
	void MatchingListClear();					// 매칭리스트를 모두 지운다.
	void TempMatchingListClear();				// 템프매칭리스트를 모두 지운다.

	// 길드전 진행
	void Action()	;									// 시간에 따라 길드전을 진행 한다.
	void RequestWarpGuildWarMap()	;					// 길드전 맵으로의 이동을 질문한다. ok 한다면 이동 시킨다	
	void Start();										// 길드전을 시작 시킨다.
	void EndGuildWar() ;								// 길드전을 종료
	void EndGuildWarWarp();								// 길드전 종료후 워프
	void EndGuildWar_New() ;							// 길드전을 종료 하고 나머지 사람들을 워프 시킨다.
	void SetEnemy_New();								// 길드전할 각각의 적을 셋팅 한다.
	void WarpGuildWar_New(SI32 personID );				// 길드전에 참가하는 길드의 길드원들을 워프 시킨다.
	SI32 IsHomeGuildorAwayGuild( SI32 siGuildunique) ;	// 해당 길드가 홈길드인지 어웨이 길드인지 확인 한다.
	SI32 FindWarpMapIndex(SI32 siMyGuildunique)	;		// 워프해갈 길드의 유니크를 찾는다.
	void CanStartGuildWar()	;							// 각각 맵에 길드전을 시작할수 있는지 확인
	void addKillPoint(SI32 siKillerPersonId,SI32 DeathPersonID);	// 상대방을 죽였을때.
	void MapUserList();									// 각 맵별 유저 리스트를 만든다.
	void SendKill( SI32 siSlot,SI32 KillerID,SI32 DeathID,SI32 HomeAway ); // 죽은 사람 알림 마지막 인자는 죽은 놈이 홈인지 어웨이인지
	SI32 GetInHuntMapWarPersonNumHome(SI32 slot);					// 매칭된 해당 길드의 사람들이 얼마나 맵에 있는지 확인 한다
	SI32 GetInHuntMapWarPersonNumAway(SI32 slot);
	void SendResultWarHome(SI32 slot,SI32 siWarResult);				// 하나의 길드에  종료 메세지를 날린다.
	void SendResultWarAway(SI32 slot,SI32 siWarResult);
	bool GuildWarRevive( SI32 siPersonID )	;
	void ArrangeList();


	//전쟁 기록을위한 함수들
	void SaveKillinfo( SI32 siMapSlot,SI32 Hone_Away,SI32 siKillerPersonID,SI32 siDiePersonID)	;
	void MakeEndData();													// 마지막에 클라이언트로 보내줄 데이터를 만든다
	bool FindMeInWarStatus(SI32 siMapSlot,SI32 Hone_Away,SI32 siPersonID, SI32* siFindSlot )	; // 내가 지금 워 스테이터스에 있는지 없는 지 확인 한다.
	bool FindMatchingList( SI32 GuildUnique );		// 매칭리스트에 길드 유니크가 있는지 확인.

	void DelEnemy(SI32 siHomeGuildUnique , SI32 siAwayGuildUnique )	;// 적대 길드를 해지 한다.
	void GuildWarptoSafeZone(	SI32 siGuildUnique )	;				// 해당 길드를 엔펠리스로 이동 시킨다.
	void SendResultToDB(SI32 siGuildUnique, SI32 siResultFlag );		// 승패를 디비로 날린다.
	void ClearStatus()	;												// 전투 전적을 초기화 한다.
	void ClearWarStatus();
	void ClearKillPoint();
	void ClearTargetStatus(SI32 siGuildUnique)	;					// 해당 길드의 전적을 초기화 한다.
	bool IsGod( SI32 personID )	;									// 무적시간 체크
};
#endif