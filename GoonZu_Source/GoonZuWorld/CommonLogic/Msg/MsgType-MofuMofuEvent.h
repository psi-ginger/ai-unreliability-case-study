#ifndef _MSGTYPEMOFUMOFUEVENT_H_
#define _MSGTYPEMOFUMOFUEVENT_H_

#include "../common/Event/MofuMofu/MofuMofu.h"

// 가변패킷을 위한 선언 - 1바이트 패킹
#pragma pack(1)

class cltGameMsgRequest_MofuMofuEvent_Reserve
{
public:
	SI16	m_siYear;
	SI16	m_siMonth;
	SI16	m_siDay;
	SI16	m_siHour;

public:
	cltGameMsgRequest_MofuMofuEvent_Reserve(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour)
		:	m_siYear(siYear), m_siMonth(siMonth), m_siDay(siDay), m_siHour(siHour)
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_Reserve
{
public:
	SI32	m_siResult;

	SI16	m_siYear;
	SI16	m_siMonth;
	SI16	m_siDay;
	SI16	m_siHour;

public:
	cltGameMsgResponse_MofuMofuEvent_Reserve(SI32 siResult, SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour)
		:	m_siResult(siResult),
			m_siYear(siYear), m_siMonth(siMonth), m_siDay(siDay), m_siHour(siHour)
	{
	}
};

class cltGameMsgRequest_MofuMofuEvent_ReserveList
{
public:

public:
	cltGameMsgRequest_MofuMofuEvent_ReserveList()
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_ReserveList
{
public:
	SI16							m_siCount;
	cltMofuMofuEventReserveUnit		m_Reserved[MOFUMOFUEVENT_ROOMNUM];

public:
	cltGameMsgResponse_MofuMofuEvent_ReserveList()
	{
		m_siCount = 0;
	}

	// 가변 패킷을 위한 함수 - by LeeKH 2007.04.19
	SI16 GetSize()
	{
		if(m_siCount > MOFUMOFUEVENT_ROOMNUM)		m_siCount = MOFUMOFUEVENT_ROOMNUM;
		SI16 siSize =	sizeof(SI16) + 
						(sizeof(cltMofuMofuEventReserveUnit) * m_siCount);

		if(siSize > sizeof(*this))					siSize = sizeof(*this);

		return siSize;
	}
};

class cltGameMsgResponse_MofuMofuEvent_Start
{
public:
	SI32	m_siVillageUnique;

public:
	cltGameMsgResponse_MofuMofuEvent_Start(SI32 siVillageUnique)
		:	m_siVillageUnique(siVillageUnique)
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_End
{
public:
	SI32	m_siVillageUnique;

public:
	cltGameMsgResponse_MofuMofuEvent_End(SI32 siVillageUnique)
		:	m_siVillageUnique(siVillageUnique)
	{
	}
};

class cltGameMsgResquest_MofuMofuEvent_EnterField
{
public:
	SI32	m_siFieldIndex;
	SI32	m_siInOut;			// 1: IN, 0: OUT

public:
	cltGameMsgResquest_MofuMofuEvent_EnterField(SI32 siFieldIndex, SI32 siInOut)
		:	m_siFieldIndex(siFieldIndex),	m_siInOut(siInOut)
	{
	}
};

class cltGameMsgResquest_MofuMofuEvent_Warp
{
public:
	cltGameMsgResquest_MofuMofuEvent_Warp()
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_SetInfo
{
public:
	SI32	m_siFieldIndex;

public:
	cltGameMsgResponse_MofuMofuEvent_SetInfo(SI32 siFieldIndex)
		:	m_siFieldIndex(siFieldIndex)
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_Info
{
public:
	SI32	m_siLeftSecond;
	SI16	m_siMaMaMofuMofu;
	SI16	m_siTotalMaMaMofuMofu;
	SI16	m_siKilledMofuMofu;
	SI16	m_siTotalMofuMofu;

public:
	cltGameMsgResponse_MofuMofuEvent_Info(SI32 siLeftSecond, SI16 siMaMaMofuMofu, SI16 siTotalMaMaMofuMofu, SI16 siKilledMofuMofu, SI16 siTotalMofuMofu)
		:	m_siLeftSecond(siLeftSecond),
			m_siMaMaMofuMofu(siMaMaMofuMofu),			m_siTotalMaMaMofuMofu(siTotalMaMaMofuMofu),
			m_siKilledMofuMofu(siKilledMofuMofu),		m_siTotalMofuMofu(siTotalMofuMofu)
	{
	}
};

class cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu
{
public:
	SI32	m_siCharUnique;
	SI32	m_siCanAttack;

public:
	cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu(SI32 siCharUnique, SI32 siCanAttack)
		:	m_siCharUnique(siCharUnique),	m_siCanAttack(siCanAttack)
	{
	}
};

#pragma pack()

#endif
