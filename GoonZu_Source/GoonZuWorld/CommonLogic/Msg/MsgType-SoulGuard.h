#ifndef _MSGTYPESOULGUARD_H
#define _MSGTYPESOULGUARD_H

#include "../SoulGuardMgr/SoulGuardMgr.h"

// 수호정령 소환/회수
class cltGameMsgRequest_SoulGuardStatusInOut
{
public:
	SI32	m_siSoulGuardID;
	SI16	m_siStatus;

public:
	cltGameMsgRequest_SoulGuardStatusInOut( SI32 siSoulGuardID, SI16 siStatus )
	{
		m_siSoulGuardID	= siSoulGuardID;
		m_siStatus		= siStatus;
	}
};	// GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT

class cltGameMsgResponse_SoulGuardStatusInOut
{
public:
	SI32	m_siSoulGuardID;
	SI16	m_siStatus;

	SI32	m_siSoulGuardCharUnique;

public:
	cltGameMsgResponse_SoulGuardStatusInOut( SI32 siSoulGuardID, SI16 siStatus, SI32	siSoulGuardCharUnique )
	{
		m_siSoulGuardID			= siSoulGuardID;
		m_siStatus				= siStatus;

		m_siSoulGuardCharUnique	= siSoulGuardCharUnique;
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT

// 수호정령 친화력 변경(받는 패킷만 있음)
class cltGameMsgResponse_SoulGuardFellowShipSet
{
public:
	SI32	m_siSoulGuardID;
	SI16	m_siFellowShip;

public:
	cltGameMsgResponse_SoulGuardFellowShipSet( SI32 siSoulGuardID, SI16 siFellowShip )
	{
		m_siSoulGuardID	= siSoulGuardID;
		m_siFellowShip	= siFellowShip;
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_FELLOWSHIPSET

// 수호정령 이름 변경
class cltGameMsgRequest_SoulGuardChangeName
{
public:
	SI32	m_siSoulGuardID;
	TCHAR	m_szChangeName[MAX_SOULGUARD_NAME];

public:
	cltGameMsgRequest_SoulGuardChangeName( SI32 siSoulGuardID, TCHAR* pszChangeName )
	{
		m_siSoulGuardID = siSoulGuardID;

		if ( pszChangeName )
		{
			StringCchCopy( m_szChangeName, sizeof(m_szChangeName), pszChangeName );
		}	
	}
};	// GAMEMSG_REQUEST_SOULGUARD_CHANGENAME

class cltGameMsgResponse_SoulGuardChangeName
{
public:
	SI32	m_siSoulGuardID;
	TCHAR	m_szChangeName[MAX_SOULGUARD_NAME];

public:
	cltGameMsgResponse_SoulGuardChangeName( SI32 siSoulGuardID, TCHAR* pszChangeName )
	{
		m_siSoulGuardID = siSoulGuardID;

		if ( pszChangeName )
		{
			StringCchCopy( m_szChangeName, sizeof(m_szChangeName), pszChangeName );
		}	
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_CHANGENAME

// 수호정령 죽음/풀어주기

// 요청 패킷은 없음
// GAMEMSG_REQUEST_SOULGUARD_DEAD

class cltGameMsgResponse_SoulGuardDead
{
public:
	SI16	m_siDeadMode;

public:
	cltGameMsgResponse_SoulGuardDead( SI16 siDeadMode )
	{
		m_siDeadMode = siDeadMode;
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_DEAD


// 수호정령 가져오기
class cltGameMsgResponse_SoulGuardGet
{
public:
	SI32				m_siSoulGuardID;				// 수호정령 고유 ID(PersonID같이 유니크한 것임)

	TCHAR				m_szName[MAX_SOULGUARD_NAME];	// 수호정령 이름

	SI16				m_siStatus;						// 수호정령 상태
	SI16				m_siKind;						// 수호정령 KIND

	SI32				m_siLimitDateVary;				// 이때까지 살아있음

	SI16				m_siFellowShip;					// 친화도

public:
	cltGameMsgResponse_SoulGuardGet( SI32 siSoulGuardID, TCHAR* pszName, SI16 siStatus, SI16 siKind, SI32 siLimitDateVary, SI16 siFellowShip )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_SoulGuardGet) );

		m_siSoulGuardID		= siSoulGuardID;

		if ( pszName )
		{
			StringCchCopy( m_szName, sizeof(m_szName), pszName );
		}

		m_siStatus			= siStatus;
		m_siKind			= siKind;

		m_siLimitDateVary	= siLimitDateVary;

		m_siFellowShip		= siFellowShip;
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_GET

// 수호정령 보관소 정보 받아오기

// 요청 패킷은 없음(서버에서만 요청하게 됨)

class cltGameMsgResponse_SoulGuardHouse_Get
{
public:
	SI16	m_siMaxHaveHouseNum;

	SI32	m_siSoulGuardID[MAX_SOULGUARD_HOUSE_NUM];
	SI16	m_siSoulGuardKind[MAX_SOULGUARD_HOUSE_NUM];

public:
	cltGameMsgResponse_SoulGuardHouse_Get( SI16 siMaxHaveHouseNum, SI32* psiSoulGuardID, SI16* psiSoulGuardKind )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_SoulGuardHouse_Get) );

		m_siMaxHaveHouseNum = siMaxHaveHouseNum;

		if ( psiSoulGuardID )
		{
			memcpy( m_siSoulGuardID, psiSoulGuardID, sizeof(m_siSoulGuardID) );
		}

		if ( psiSoulGuardKind )
		{
			memcpy( m_siSoulGuardKind, psiSoulGuardKind, sizeof(m_siSoulGuardKind) );
		}
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_HOUSE_GET

// 캐릭터에 있는 수호정령과 보관소에 있는 수호정령을 맞교환한다
class cltGameMsgRequest_SoulGuardHouse_Change
{
public:
	SI32	m_siSelectIndex;	// 선택한 수호정령 보관소 번호

public:
	cltGameMsgRequest_SoulGuardHouse_Change( SI32 siSelectIndex )
	{
		m_siSelectIndex = siSelectIndex;
	}
};	// GAMEMSG_REQUEST_SOULGUARD_HOUSE_CHANGE

// 수호정령 보관소 갯수 늘리기

// 요청 패킷 자체는 없음
//GAMEMSG_REQUEST_SOULGUARD_HOUSE_CREATE

class cltGameMsgResponse_SoulGuardHouse_Create
{
public:
	SI16	m_siMaxHaveHouseNum;

	SI16	m_siLeftItemPos;
	cltItem	m_clLeftItem;

public:
	cltGameMsgResponse_SoulGuardHouse_Create( SI16 siMaxHaveHouseNum, SI16 siLeftItemPos, cltItem* pclLeftItem )
	{
		m_siMaxHaveHouseNum	= siMaxHaveHouseNum;

		m_siLeftItemPos		= siLeftItemPos;
		if ( pclLeftItem )
		{
			m_clLeftItem.Set( pclLeftItem );
		}
	}
};	// GAMEMSG_RESPONSE_SOULGUARD_HOUSE_CREATE


#endif