#ifndef _BLACKWAR_
#define _BLACKWAR_

#include "../../CommonLogic/CommonLogic.h"

#define MAX_BLACKWAR_UNIT			10

const SI32 REWARD_ITEMUNIQUE[] = { 5090, 5100, 5056, 5200, 5057, 5010, 5050, 5075, 5017, 
5065, 5080, 5030, 5070, 5000, 5040, 5060, 5020, 5025 };

const SI32 REWARD_ITEMUNIQUE_PERCENT[] = { 12, 12, 11, 11, 10, 7, 7, 6, 3,
3,   3,  3,  2,  2, 2, 2, 2, 1 };

//***************************************************************************************************
//
// Class Name 		: CBlackWarUnit
//
// Last Modified 	: 2007 / 04 / 11
// Created		 	: 김광명
//
// Function			: 흑의군단 유닛 설정
//
//***************************************************************************************************
class CBlackWarUnit
{
public:

	SI32 m_siKind;
	SI32 m_siNum;			// 마을 주변에 나올것들
	SI32 m_siRate;
	SI32 m_siBasicLevel;
	SI32 m_siRandLevel;
	SI32 m_siCurNum;

public:

	CBlackWarUnit() : m_siKind(0), m_siNum(0), m_siRate(0), m_siBasicLevel(0), m_siRandLevel(0), m_siCurNum(0)
	{
	};

	CBlackWarUnit(SI32 Kind, SI32 Num, SI32 Rate, SI32 CurNum, SI32 Basic, SI32 Rand) :
	m_siKind(Kind), m_siNum(Num), m_siRate(Rate), m_siCurNum(CurNum), m_siBasicLevel(Basic), m_siRandLevel(Rand)
	{
	};

	void Init()
	{
		m_siKind		= 0;
		m_siNum			= 0;
		m_siRate		= 0;
		m_siBasicLevel	= 0;
		m_siRandLevel	= 0;
		m_siCurNum		= 0;
	}

	SI32 GetKind() { return m_siKind;}
	SI32 GetNum() { return m_siNum;}
	SI32 GetRate() { return m_siRate;}
	SI32 GetBasicLevel() { return m_siBasicLevel;}
	SI32 GetRandLevel() { return m_siRandLevel;}
	SI32 GetCurNum() { return m_siCurNum;}
};

//***************************************************************************************************
//
// Class Name 		: CBlackWarManager
//
// Last Modified 	: 2007 / 04 / 11
// Created		 	: 김광명
//
// Function			: 흑의군단 매니저
//
//***************************************************************************************************
class CBlackWarManager
{
private:

#ifdef _SAFE_MEMORY
	NSafeTArray<CBlackWarUnit, MAX_BLACKWAR_UNIT>		m_clBlackWarUnit;
#else
	CBlackWarUnit m_clBlackWarUnit[MAX_BLACKWAR_UNIT];
#endif

	// 지금 전쟁이 진행중인가?
	bool	m_bBlackWarSwitch;	

	// 전체 병사수. 
	SI32	m_siTotalUnitNumber;		

	// 현재 병사수. 
	SI32	m_siCurUnitNumber;		

	// 현재까지 만들어진 수. 
	SI32	m_siMadeNumber;			

	// 유니트는 몇 종이 있는가?
	SI32	m_siAddUnitNumber;		

	// 사상자 숫자. 
	SI32	m_siCasualtyNumber;		

	// 남은 시간. (ms) 
	DWORD	m_dwLeftTimeClock;	

	// 시작한 시간
	DWORD	m_dwBlackWarStartClock;

	// 남은 몬스터 숫자.
	SI32	m_siLeftUnitNumber;		

	bool	m_bDieBossSwitch;
	bool	m_bMakeBossSwitch;
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, 4>							m_bMakeMiddleBossSwitch;
	NSafeTArray<SI32, MAX_WARRANK_PERSON_PER_MAP>	m_siRewardCharID;
#else
	bool	m_bMakeMiddleBossSwitch[4];

	SI32	m_siRewardCharID[MAX_WARRANK_PERSON_PER_MAP];
#endif


	// 전쟁에서 이겼는가?
	bool	m_bWinSwith;

	// 전쟁이 시작한 시간
	SI32	m_siStartHour;

	SI32	m_siKillBlackUnitNumber;

	// 전쟁이 끝난 시간
	DWORD	m_dwBlackWarEndClock;
	// 마지막 전쟁에서 MVP PersonID
	SI32	m_siMVPPersonID;

	//------------
	// 출력 
	//------------
	bool	m_bDrawSwitch;
	SI32	m_siDrawCmdFont;
	DWORD   m_dwDrawStartClock;

public:

	CBlackWarManager();
	~CBlackWarManager();

	void Init();

	// 전쟁을 기획한다. 
	bool MakeWar( DWORD ClocK, SI32 StartHour );
	// 전쟁 유닛를 추가한다. 
	void AddBlackWarUnit( SI32 Kind, SI32 UnitNumber );

	// 전쟁을 종결 짓는다
	void WarEnd();

	// 전쟁 액션 (서버) 
	void Action();

	void SendWarMsg( SI32 Cmd, SI32 Mapindex, SI32 X, SI32 Y, bool MakeBoss );

	void BlackWarReward( bool Flag );

	void SetDraw(SI32 Cmd);
	// 전황을 표시한다. (클라이언트) 
	void Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 Rx, SI32 Ry );

	void SetBlackWar( bool Flag )
	{
		m_bBlackWarSwitch = Flag;
	}
	bool IsBlackWar()
	{
		return m_bBlackWarSwitch;
	}

	void SetDieBoss( bool Flag )
	{
		m_bDieBossSwitch = Flag;
	}

	void SetLeftTime( DWORD LeftClock )
	{
		m_dwLeftTimeClock = LeftClock;
	}

	bool IsWinSwitch()
	{
		return m_bWinSwith;
	}

	void SetStartHour( SI32 StartHour )
	{
		m_siStartHour = StartHour;
	}

	SI32 GetStartHour()
	{
		return m_siStartHour;
	}

	void IncreaseKillBlackUnitNumber()
	{
		m_siKillBlackUnitNumber++;
	}

	bool IsMVPPersonID( SI32 PersonID )
	{
	//	return m_siMVPPersonID == PersonID ? true : false;
		if( m_siMVPPersonID == PersonID )
		{
			return true;
		}
		else
			return false;
	}
	//[진성] 마을 정보에 전쟁 정보 표시하게 패킷을 던질 것. => 2008-4-10
	void VillageRecordList(void);
};

#endif