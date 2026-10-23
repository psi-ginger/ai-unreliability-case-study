//---------------------------------
// 2004/8/13 김태곤
//---------------------------------
#ifndef _EVENT_H
#define _EVENT_H

#include <Directives.h>

#include "../../NLib/NArray.h"
#include "../../NLib/NUpdateTimer.h"
#include "CommonHeader.h"


template< class DataType >
class NHashTableString;

class cltCharServer;




class cltEvent{
public:
	SI16	siVillageUnique;
	DWORD	dwLeftClock;			// 이벤트 잔여시간. 
	DWORD	dwStartClock;			// 이벤트 시작 시간. 
	DWORD	dwLastInfoClock;		// 마지막으로 이벤트 정보를 통보한 시간. 
	SI16	siHuntmapIndex;			// 이벤트 npc가 나타날 맵의 인덱스 
	
	bool bEventSwitch;			// 이벤트가 진행중인지 여부. 

	cltEvent();
	~cltEvent();

	void Init();
};

#define MAX_EVENTTIME_COUNT		1000
#define MAX_HUNTMAP_INDEX		50

enum EVENTTYPE
{
	EVENTTYPE_NONE,
	EVENTTYPE_PERIOD,
	EVENTTYPE_SHOT,	
};

enum
{
	EXEC_NONE,

	EXEC_PERIOD_COLLECTIONITEM,
	EXEC_PERIOD_HUNTITEM,
	EXEC_PERIOD_MAKEEXPBONUS,
	EXEC_PERIOD_HUNTEXPBONUS,

	EXEC_SHOT_GIVEITEM,
	EXEC_SHOT_MASTERNOTICE,
	EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE,	//[진성] 몬스터 출현 이벤트 - 마을 안 출현.			=> 2008-8-7 
	EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE,	//[진성] 몬스터 출현 이벤트 - 마을 밖 출현.			=> 2008-8-7
	EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE,//[진성] 몬스터 출현 이벤트 - 마을에 무작위 출현.	=> 2008-8-7

	EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP,//[성웅] 몬스터 출현 이벤트 - 마을에 무작위 출현.	=> 2008-9-17
	EXEC_SHOT_GUILDWAR,						//[성웅] 길드 사냥터 소유 길드전 매니저. - 길드전 20분마다 시작	=>	2009-03-27
	EXEC_SHOT_GIVEMISSION,					//[지연] 군주데이 미션을 부여한다.
};


class cltEventTime
{
public:
	TCHAR			m_szEventName[48];
	SI08			m_siEventType;

	TCHAR			m_szParentEventName[48];

	SYSTEMTIME		m_StartTime;
	SYSTEMTIME		m_EndTime;

	bool			m_bEventShoted;
	bool			m_bEventActivated;

	NUpdateTimer	m_kUpdateTimer_Event;

	SI16			m_siEventExecuteType;

	SI16			m_siBonusAmount;
	SI16			m_siItemUnique;
	SI16			m_siItemNum;

	SI16			m_siCharKind;				//[진성] 생성되는 캐릭터 종류.			=> 2008-8-7
	SI16			m_siCharNum;				//[진성] 생성되는 캐릭터 수.			=> 2008-8-7
	SI16			m_siCharLevel;				//[진성] 생성되는 캐릭터 레벨.			=> 2008-8-7
	SI16			m_siCharLife;				//[진성] 생성되는 캐릭터 생명력.		=> 2008-8-7	
	SI16			m_siVillage;				//[진성] 생성되는 캐릭터 마을.			=> 2008-8-7
	SI16			m_siPosX;					//[진성] 생성되는 캐릭터 X좌표.			=> 2008-8-7
	SI16			m_siPosY;					//[진성] 생성되는 캐릭터 Y좌표.			=> 2008-8-7
	// 0으로 주면 랜덤으로 돌리고 해당 맵인덱스를주면 그 맵에 등장시키기 위해 추가
	SI16			m_siHuntmapIndex;			//[성웅] 생성되는 헌트맵인덱스	=> 2008-9-17 

	SI16			m_siTextMgr;

public:
	cltEventTime()
	{
		ZeroMemory(this, sizeof(cltEventTime));
	}

	~cltEventTime()
	{
	}

	bool SetActivated(bool bActivated, bool bSendAllChar = false);
};

class cltEventTimeManager
{
protected:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltEventTime*, MAX_EVENTTIME_COUNT>		m_pEventTimes;
#else
	cltEventTime*				m_pEventTimes[MAX_EVENTTIME_COUNT];
#endif
	SI32						m_siCount;

	NUpdateTimer				m_kUpdateTimer_Action;

	NHashTableString< int >*	pclHashTableString;	

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_NPC_NUMBER>		siMonsterID;
#else
	SI16						siMonsterID[MAX_NPC_NUMBER];		// [진성] 출현 몬스터의 아이디. 2008-8-27
#endif

	// 사냥터정보를 얻어 오기 위해서 헌트맵인포파일에서 맵인덱스를 가지고와서 넣어둘곳
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_HUNTMAP_INDEX>		m_HuntmapIndexByResource;
#else
	SI16			m_HuntmapIndexByResource[ MAX_HUNTMAP_INDEX ];	//[성웅] HuntMapInfo.txt에서 읽어온  헌트맵인덱스	=> 2008-9-17 
#endif
	SI16						eventmapindex	;					// 이벤트 맵을 읽어 왔을때 맵인덱스 

public:
	enum KITERACCOON_EVENT
	{
		KITERACCOON_EVENT_NONE = 0 
		,KITERACCOON_EVENT_START	// 카이트라쿤 이벤트 시작
		,KITERACCOON_EVENT_KILL		// 카이트라쿤을 누군가가 죽임
		,KITERACCOON_EVENT_END		// 카이트라쿤 이벤트 종료
	};

	cltEventTimeManager();
	~cltEventTimeManager();

	void Init();

	SI32 ConvertEventExecuteTypeStringToEnum(TCHAR* pszEventExecuteType);

	void LoadEventTimePeriod();
	void LoadEventTimeWeek();
	void LoadEventTimeShot();
	void LoadHuntmapInfo();

	bool AddEventByTime( TCHAR* pszEventName, SI16 siSYear, SI16 siSMonth, SI16 siSDay, SI16 siSHour, SI16 siSMinute,
		SI16 siEYear, SI16 siEMonth, SI16 siEDay, SI16 siEHour, SI16 siEMinute, bool Activated);

	bool AddEventByWeek( TCHAR* pszEventName, SI16 siSWeek, SI16 siSHour, SI16 siSMinute, SI16 siEWeek, SI16 siEHour, SI16 siEMinute, bool Activated );

	bool AddShotEvent( TCHAR* pszEventName, SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI16 siMinute, bool Activated , TCHAR* pszParentName = NULL);

	bool InEventTime( TCHAR* pszEventName, SYSTEMTIME* pCurTime );
	SI08 GetEventType( TCHAR* pszEventName );

	void Action();

	bool EventAction( cltEventTime* pclEventTime );

	void SendEventMsg( SI16 siType, SI32 siCharID=0, SI32 siPara1=0, SI32 siPara2=0 );

	bool EventPeriod_CollectionItem( cltEventTime* pclEventTime );
	bool EventPeriod_HuntItem( cltEventTime* pclEventTime );
	bool EventPeriod_MakeExpBonus( cltEventTime* pclEventTime );
	bool EventPeriod_HuntExpBonus( cltEventTime* pclEventTime );

	bool EventAction_GiveItem( cltEventTime* pclEventTime );
	bool EventAction_MasterNotice( cltEventTime* pclEventTime );
	bool EventAction_MonsterAppear( SI16 siType, cltEventTime* pclEventTime );			// [진성] 추석 이벤트 몬스터 출현.			 => 2008-8-7
	bool EventAction_MonsterAppearNotice( SI16 siType, cltEventTime* pclEventTime );	// [진성] 추석 이벤트 몬스터 출현 위치 공지. => 2008-8-27
	void EventAction_GoonzuDay_GiveMission( cltEventTime* pEventTime );					// [지연] 군주데이미션부여

	cltEventTime* GetEvent( TCHAR* pszEventName );
	void SetEventMapIndex( SI16 sieventmapindex );
	SI16 GetEventMapIndex();
	bool IsEventStartTiem( TCHAR* pszEventName, SYSTEMTIME* stTime);
	bool IsEventEndTiem( TCHAR* pszEventName, SYSTEMTIME* stTime );
	bool IsEventStartTiemOverMinute( TCHAR* pszEventName,SYSTEMTIME* pCurTime , SI32 siOvertime ) ;// overtime 만큼 지났는지 확인(분)

	void GiveMission(cltCharServer* pChar);
};


#endif