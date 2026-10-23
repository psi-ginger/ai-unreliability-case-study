//---------------------------------
// 2007/4/16 이강형
//---------------------------------
#ifndef _MOFUMOFU_H
#define _MOFUMOFU_H

#include "..\Event.h"
#include "../NLib/NUpdateTimer.h"

#define MOFUMOFUEVENT_MAPINDEX	0

#define MOFUMOFUEVENT_SENDMSG_MIN			5				// 5분 전부터 메시지를 전송한다.
#define MOFUMOFUEVENT_ENABLE_ENTER_TIME_MS	5 * 60 * 1000	// 5분 뒤부터 입장 가능
#define MOFUMOFUEVENT_MAKE_ENEMY_TIME_MS	4 * 60 * 1000	// 4분 뒤에 몬스터 소환
#define MOFUMOFUEVENT_DEFAULT_TIME_MS		50 * 60 * 1000	// 50분 동안 이벤트를 진행한다.

#define MOFUMOFUEVENT_MAXKINGENEMY_COUNT	1				// 전체 잡아야 할 킹(보스) 몬스터 수
#ifdef _DEBUG
	#define MOFUMOFUEVENT_MAXENEMY_COUNT	90
#else
	#define MOFUMOFUEVENT_MAXENEMY_COUNT	1800			// 전체 잡아야 할 몬스터 수
#endif


#define MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT		50			// 한 지역에 최대로 소환되는 모후모후 수
#define MOFUMOFUEVENT_FIELD_MAMAMOFUMOFU_INDEX	50 + 10		// 한 지역의 마마모후 인덱스
#define MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT		5			// 한 지역에 소환되는 지역 수
#define MOFUMOFUEVENT_FIELD_SPWAN_ENEMY_COUNT	10			// 한 종류의 몬스터가 소환되는 수
#define MOFUMOFUEVENT_FIELD_MIN_ENEMY_COUNT		4			// 다음 몬스터가 소환될 조건

#define MOFUMOFUEVENT_MAXFIELD_COUNT		3				// 모후모후이벤트 지역의 전투필드 수

#define	MOFUMOFUEVENT_ROOMNUM				24				// 1시간 간격으로 예약

#define KIND_MAMARACCOON					170				// 마마구리댁
#define KIND_MAGICALRACCOON					171				// 메지컬구리댁
#define KIND_RACCOONLORD					172				// 구리댁로드
#define KIND_RACCOONTOWER					173				// 구리댁타워
#define KIND_ZORRORACCOON					174				// 조로구리댁

//---------------------------------
// 모후모후던젼 이벤트 예약
//---------------------------------
class cltMofuMofuEventReserveUnit
{
public:
	SYSTEMTIME	m_ReserveHour;
	SI32		m_siVillageUnique;

public:
	cltMofuMofuEventReserveUnit()
	{
		Init();
	}
	~cltMofuMofuEventReserveUnit()
	{
	}

	void Init()
	{
		ZeroMemory(&m_ReserveHour, sizeof(m_ReserveHour));
		m_siVillageUnique	= 0;
	}

	void Set(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siVillageUnique)
	{
		m_ReserveHour.wYear		= siYear;
		m_ReserveHour.wMonth	= siMonth;
		m_ReserveHour.wDay		= siDay;
		m_ReserveHour.wHour		= siHour;

		m_siVillageUnique	= siVillageUnique;
	}

	void Set(cltMofuMofuEventReserveUnit* pUnit)
	{
		if(pUnit == NULL)				return;

		memcpy(&m_ReserveHour, &(pUnit->m_ReserveHour), sizeof(m_ReserveHour));
		m_siVillageUnique	= pUnit->m_siVillageUnique;
	}
};

//---------------------------------
// 모후모후던젼 이벤트 클래스
//---------------------------------
class cltMofuMofuField
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT>		m_siEnemyID;
#else
	SI32	m_siEnemyID[MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT];
#endif
	SI32	m_siCount;
	SI32	m_siCreateCount;
	SI32	m_siKilledCount;
	SI32	m_siMamaMofuMofuID;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT>		m_siSpwanX;
	NSafeTArray<SI16, MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT>		m_siSpwanY;
#else
	SI16	m_siSpwanX[MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT];
	SI16	m_siSpwanY[MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT];
#endif

	SI16	m_siFieldIndex;

	SI16	m_siInPosX;
	SI16	m_siInPosY;

	SI16	m_siOutPosX;
	SI16	m_siOutPosY;
public:
	cltMofuMofuField()
	{
		Init();
	}

	~cltMofuMofuField()
	{
	}

	void Init()
	{
#ifdef _SAFE_MEMORY
		m_siSpwanX.ZeroMem();
		m_siSpwanY.ZeroMem();
#else
		memset(m_siSpwanX, 0, sizeof(m_siSpwanX));
		memset(m_siSpwanY, 0, sizeof(m_siSpwanY));
#endif

		m_siInPosX	= 0;
		m_siInPosY	= 0;

		m_siOutPosX	= 0;
		m_siOutPosY	= 0;

		ReInit();
	}

	void ReInit()
	{
#ifdef _SAFE_MEMORY
		m_siEnemyID.ZeroMem();
#else
		memset(m_siEnemyID, 0, sizeof(m_siEnemyID));
#endif
		m_siCount			= 0;
		m_siMamaMofuMofuID	= 0;

		m_siCreateCount		= 0;
		m_siKilledCount		= 0;
	}

	SI32 CreateEnemy();					// 40 마리 생성
	SI32 CreateMamaMofuMofu();			// 마마모후모후 생성
	BOOL KilledEnemy(SI32 siIndex, SI32 siEnemyID);

	BOOL CanAttackMaMaMofuMofu();
};

//---------------------------------
// 모후모후던젼 이벤트 클래스
//---------------------------------
class cltMofuMofuEvent : public cltEvent
{
protected:
	SI32				m_siKingMofuMofuID;			// 대장 마마모후모후

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMofuMofuEventReserveUnit, MOFUMOFUEVENT_ROOMNUM>		m_clUnit;
	NSafeTArray<cltMofuMofuField, MOFUMOFUEVENT_MAXFIELD_COUNT>			m_clField;
#else
	cltMofuMofuEventReserveUnit		m_clUnit[MOFUMOFUEVENT_ROOMNUM];
	cltMofuMofuField				m_clField[MOFUMOFUEVENT_MAXFIELD_COUNT];
#endif

	NUpdateTimer		m_UpdateTimer;
	NUpdateTimer		m_EnableEnterTimer;
	NUpdateTimer		m_CreateEnemyTimer;
	NUpdateTimer		m_EventTimer;

	BOOL				m_bEnterField;
	BOOL				m_bCreateFirstEnemy;
	SI32				m_siLastSendMsgMinute;

	SI32				m_siKingMofuMofuKilledCount;
	SI32				m_siMofuMofuKilledCount;

	SI16				m_siHour;			// 마지막 시간을 기억한다.


public:
	cltMofuMofuEvent();
	~cltMofuMofuEvent();

	// 모후모후 이벤트 초기화 작업
	BOOL Init();

	// 모후모후 이벤트 재 초기화 작업
	BOOL ReInit();

	// 모후모후 이벤트를 시작한다. 
	BOOL Start(SI16 siRoomIndex, SI32 VillageUnique);

	// 모후모후 이벤트를 종료. - 대장 마마모후모후를 잡았을 경우 or 시간이 다 되었을 경우
	void End();

	// 모후모후 이벤트 진행. 
	BOOL Action();

	// 예약 가능?
	BOOL CanReserveRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siVillageUnique);
	// 예약
	BOOL ReserveRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siVillageUnique);
	// 설정
	BOOL SetRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siVillageUnique);
	// 방 얻기
	cltMofuMofuEventReserveUnit *GetRoom(SI32 siRoomIndex);
	// 방번호 얻기
	SI32 GetRoomIndex(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour);

	// 현재 이벤트를 진행하고 있는 마을
	SI32 GetCurrentEventVillage();

	// 필드에 입장이 가능한가?
	BOOL CanEnterField()				{		return m_bEnterField;		}

	// 현재 서버시간을 이용하여 RoomIndex를 가져온다.
	SI32 GetRoomIndexFromServerTime();

	BOOL KilledEnemy(SI16 siFieldIndex, SI16 siIndex, SI32 siEnemeyID);

	cltMofuMofuField* GetField(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex > MOFUMOFUEVENT_MAXFIELD_COUNT)		return NULL;
		return &m_clField[siIndex];
	}

	static	SI16 m_siMaxEnemyCount;

};

#endif