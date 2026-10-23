//_____________________________________________________________________
//
// 황진성 2008-3-26
//
// 마을 이벤트 - 몬스터 습격.
//_____________________________________________________________________
#ifndef _MOSTERATTACK_H_
#define _MOSTERATTACK_H_

#include "../../CommonLogic/CommonLogic.h"



#define EVENT_SPECIALREWARD_COUNT		1			// 아이템 2개 지급 이벤트 횟수 (1번째에만 2개 지급, 누적이 아닌 단지 몇번째인가 그 값)


class cltEventTimeManager;






// 몬스터 정보.
class MosterInfo
{
public:
	enum{ 
			DIE,		// 디졌어.
			LIVE		// 살았어.
		};
public:
	MosterInfo() 
	{  
		Clear();
	}
	void Clear(void)
	{
		ZeroMemory(this, sizeof(MosterInfo));
	}
	bool IsLive(void)
	{
		return m_bMosterLive;
	}
	
	void Live(SI32 _Monsterkind)
	{
		m_siMonsterkind = _Monsterkind; 
		m_bMosterLive   = LIVE;
	}
	void Die(void)
	{
		m_bMosterLive   = DIE;
	}

	void SetMonsterkind(SI32 _siMonsterkind)
	{
		m_siMonsterkind = _siMonsterkind;
	}

	SI32 GetMonsterkind(void)
	{
		return m_siMonsterkind;
	}
	void SetMonsterID(SI32 _siMonsterID)
	{
		m_siMonsterID = _siMonsterID;
	}

	SI32 GetMonsterID(void)
	{
		return m_siMonsterID;
	}

	void SetMapIndex(SI32 _MapIndex)
	{
		m_MapIndex = _MapIndex;
	}

	SI32 GetMapIndex(void)
	{
		return m_MapIndex;
	}
	
private:
	SI32		m_siMonsterkind;	// 몬스터 종류.
	SI32		m_siMonsterID;		// 몬스터의 서버 ID.
	bool		m_bMosterLive;		// 몬스터 살았냐? 죽었냐?
	SI32		m_MapIndex;			// 몬스터 출현 맵 인덱스.
};

// 몬스터 이벤트 클레스 
class CMonsterAttackEvent
{
public:
	enum{ 
			EVENT_END		  = 0,	
			EVENT_START		  = 1,
			MOSTER_NUM		  = 7,		// 출현하는 몬스터 수.
			RUN_TIME		  = 50,		// 진행시간.
//			REWARD_ITEMUNIQUE = 8113,	// 보상 아이템 유니크.
//			REWARD_ITEMNUM	  = 3,		// 보상으로 주는 아이템의 개수.
		};

public:
	CMonsterAttackEvent();
	~CMonsterAttackEvent();

	bool		IsEvent(void);								// 이벤트 중인가.
	void		StartEvent(SI32 _siEventVillageUnique);		// 이벤트 시작.
	void		EndEvent(bool b_GiveReward = false);		// 이벤트 종료.
	void		Action(void);								// 액션.^0^
	void		MonsterDie( SI32 _DieMonsterKind );			// 해당 몬스터가 죽다.
		
	SI16		GetEventVillageUnique(void)						{ return m_siEventVillageUnique;				}
	SYSTEMTIME*	GetStartTime(void)								{ return &m_stStartTime;						}	

public:
	void		Clear(void);
	void		CreateMonster(void);						// 몬스터 생성.
	void		DestroyMonster(void);						// 몬스터 제거.
	void		DrawWantedMonster(void);					// 클라에서 쓰는 수배 몬스터 그리기.
	
	
	void		EventReward(void);								// 보상.
	bool		TimeOver(void);								// 시간이 지났나.
	bool		MonsterAllDie(void);						// 몬스터가 다 죽었는가.
	void		RandMap(void);								// 지도를 렌덤하게 뽑아준다.
	void		SetMonsterKind(void);						// 출현 몬스터 세팅.
	void		SendMsg(SI32 _para1, SI32 _para2 = 0);
	bool		NotifyTime(void);								
	void		RemainTime(void);
		
	void		SetEventVillageUnique(SI16 _EventVillageUnique) { m_siEventVillageUnique = _EventVillageUnique;					}	
	void		SetStartTime(SYSTEMTIME* _stStartTime)			{ memcpy(&m_stStartTime, _stStartTime, sizeof(m_stStartTime));	}
	

public:
	bool				m_bEvent;						// 이벤트 여부.
	SYSTEMTIME			m_stStartTime;					// 시작시간.
	SYSTEMTIME			m_stEndTime;					// 종료시간.
	SI16				m_siEventVillageUnique;			// 이벤트 마을 유니크.

#ifdef _SAFE_MEMORY
	NSafeTArray<MosterInfo, MOSTER_NUM>		m_siMosterInfo;
#else
	MosterInfo			m_siMosterInfo[MOSTER_NUM];		// 몬스터 정보.
#endif

	DWORD				m_dwStartTickCount;
	DWORD				m_dwRemainSecondTime;

	// 이벤트 횟수 카운트와 횟수 세팅 관련 함수들
public:
	//SI16				m_siMonsterAttackEventCount;
	NSafeTArray<SI16, MAX_VILLAGE_NUMBER>		m_arEventCount_EachVillage;
	void				MonsterAttack_EventCount_Increase();
	void				MonsterAttack_EventCount_Reset();
	SI16				MonsterAttack_EventCount_Get();
};

#endif