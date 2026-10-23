#ifndef _DBMSGSOULGUARD_H
#define _DBMSGSOULGUARD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

// 
class sDBRequest_SoulGuardCreate
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID;

	TCHAR				m_szName[MAX_SOULGUARD_NAME];	// 수호정령 이름

	SI16				m_siStatus;						// 수호정령 상태
	SI16				m_siKind;						// 수호정령 KIND

	SI32				m_siLimitDateVary;				// 이때까지 살아있음

	SI16				m_siFellowShip;					// 친화도

public:
	sDBRequest_SoulGuardCreate( )
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuardCreate) );
	}

	sDBRequest_SoulGuardCreate( SI32 siPersonID, TCHAR* pszName, SI16 siStatus, SI16 siKind, SI32 siLimitDateVary, SI16 siFellowShip )
	{
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuardCreate);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_CREATE;

		m_siPersonID			= siPersonID;

		if ( pszName )
		{
			StringCchCopy( m_szName, sizeof(m_szName), pszName );
		}

		m_siStatus				= siStatus;
		m_siKind				= siKind;

		m_siLimitDateVary		= siLimitDateVary;

		m_siFellowShip			= siFellowShip;
		
	}
};

class sDBResponse_SoulGuardCreate
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;

	SI32				m_siPersonID;

	SI32				m_siSoulGuardID;				// 수호정령 고유 ID(PersonID같이 유니크한 것임)
	SI08				m_siPosType;					// 어디에 생성되어있는지 알려줌 (1 캐릭터, 2 보관소)

	TCHAR				m_szName[MAX_SOULGUARD_NAME];	// 수호정령 이름

	SI16				m_siStatus;						// 수호정령 상태
	SI16				m_siKind;						// 수호정령 KIND

	SI32				m_siLimitDateVary;				// 이때까지 살아있음

	SI16				m_siFellowShip;					// 친화도

public:
	sDBResponse_SoulGuardCreate()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuardCreate) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_CREATE

// 캐릭터에게 있는 수호정령 정보를 갖고 온다
class sDBRequest_SoulGuardGet
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

public:
	sDBRequest_SoulGuardGet()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuardGet) );
	}

	sDBRequest_SoulGuardGet( SI32 siPersonID )
	{
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuardGet);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_GET;

		m_siPersonID			= siPersonID;
	}
};

class sDBResponse_SoulGuardGet
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;						// 결과값

	SI32				m_siPersonID;

	SI32				m_siSoulGuardID;				// 수호정령 고유 ID(PersonID같이 유니크한 것임)

	TCHAR				m_szName[MAX_SOULGUARD_NAME];	// 수호정령 이름

	SI16				m_siStatus;						// 수호정령 상태
	SI16				m_siKind;						// 수호정령 KIND

	SI32				m_siLimitDateVary;				// 이때까지 살아있음

	SI16				m_siFellowShip;					// 친화도

public:
	sDBResponse_SoulGuardGet()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuardGet) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_GET


// 캐릭터에게 있는 수호정령 친화력를 설정한다
class sDBRequest_SoulGuard_FellowShipSet
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	SI32			m_siSoulGuardID;
	SI16			m_siFellowShip;

public:
	sDBRequest_SoulGuard_FellowShipSet()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuard_FellowShipSet) );
	}

	sDBRequest_SoulGuard_FellowShipSet( SI32 siPersonID, SI32 siSoulGuardID, SI16 siFellowShip )
	{
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuard_FellowShipSet);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_FELLOWSHIP_SET;

		m_siPersonID			= siPersonID;
		m_siSoulGuardID			= siSoulGuardID;

		m_siFellowShip			= siFellowShip;
	}
};

class sDBResponse_SoulGuard_FellowShipSet
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;

	SI32			m_siPersonID;

	SI32			m_siSoulGuardID;
	SI16			m_siFellowShip;

public:
	sDBResponse_SoulGuard_FellowShipSet()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuard_FellowShipSet) );
	}

};	// DBMSG_RESPONSE_SOULGUARD_FELLOWSHIP_SET

// 캐릭터에 있는 수호정령의 이름 변경
class sDBRequest_SoulGuard_Change_Name
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	SI32			m_siSoulGuardID;
	TCHAR			m_szChangeName[MAX_SOULGUARD_NAME];

public:
	sDBRequest_SoulGuard_Change_Name()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuard_Change_Name) );
	}

	sDBRequest_SoulGuard_Change_Name( SI32 siPersonID, SI32 siSoulGuardID, TCHAR* pszChangeName )
	{
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuard_Change_Name);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_CHANGE_NAME;

		m_siPersonID			= siPersonID;
		m_siSoulGuardID			= siSoulGuardID;

		if ( pszChangeName )
		{
			StringCchCopy( m_szChangeName, sizeof(m_szChangeName), pszChangeName );
		}
	}
};

class sDBResponse_SoulGuard_Change_Name
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;

	SI32			m_siPersonID;

	SI32			m_siSoulGuardID;
	TCHAR			m_szChangeName[MAX_SOULGUARD_NAME];

public:
	sDBResponse_SoulGuard_Change_Name()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuard_Change_Name) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_CHANGE_NAME

enum SOULGUARD_DEAD_MODE
{
	SOULGUARD_DEAD_MODE_FREE = 1,	// 주인이 풀어준다
	SOULGUARD_DEAD_MODE_DEAD		// 목숨이 다해서 죽었다(이것은 클라이언트로 부터 올수는 없다 오로지 받을때만 쓰임)
};

// 캐릭터에 있는 수호정령이 수명이 다해서 죽는다
class sDBRequest_SoulGuard_Dead
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	SI32			m_siSoulGuardID;

	SI32			m_siDeadMode;

public:
	sDBRequest_SoulGuard_Dead()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuard_Dead) );
	}

	sDBRequest_SoulGuard_Dead( SI32 siPersonID, SI32 siSoulGuardID, SI32 siDeadMode )
	{
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuard_Dead);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_DEAD;

		m_siPersonID			= siPersonID;
		m_siSoulGuardID			= siSoulGuardID;
		m_siDeadMode			= siDeadMode;
	}
};

class sDBResponse_SoulGuard_Dead
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;

	SI32			m_siPersonID;

	SI32			m_siDeadMode;

public:
	sDBResponse_SoulGuard_Dead()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuard_Dead) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_DEAD

//--------------------------------------------------------------------------- 
//                          수호정령 보관소 관련
//--------------------------------------------------------------------------- 

// 수호정령 보관소 정보를 얻어온다
class sDBRequest_SoulGuardHouse_Get
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

public:
	sDBRequest_SoulGuardHouse_Get()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuardHouse_Get) );
	}

	sDBRequest_SoulGuardHouse_Get( SI32 siPersonID )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_HOUSE_GET;
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuardHouse_Get);

		m_siPersonID			= siPersonID;
	}

};

class sDBResponse_SoulGuardHouse_Get
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;				// 결과값

	SI32			m_siPersonID;			// 요청 ID

	SI32			m_siHouseNum;			// 현재 사용가능한 보관소 갯수

	SI32			m_siSoulGuardID[MAX_SOULGUARD_HOUSE_NUM];		// 수호정령 ID
	SI16			m_sisoulGuardKind[MAX_SOULGUARD_HOUSE_NUM];	// 수호정령 KIND

public:
	sDBResponse_SoulGuardHouse_Get()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuardHouse_Get) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_HOUSE_GET

// 캐릭터의 수호정령과 보관소의 수호정령을 맞교환 한다
class sDBRequest_SoulGuardHouse_Change
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	SI32			m_siCharSoulGuardID;	// 캐릭터가 갖고 있는 수호정령 ID
	SI32			m_siHouseSoulGuardID;	// 보관소에 있는 수호정령 ID

public:
	sDBRequest_SoulGuardHouse_Change()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuardHouse_Change) );
	}

	sDBRequest_SoulGuardHouse_Change( SI32 siPersonID, SI32 siCharSoulGuardID, SI32 siHouseSoulGuardID )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_HOUSE_CHANGE;
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuardHouse_Change);

		m_siPersonID			= siPersonID;

		m_siCharSoulGuardID		= siCharSoulGuardID;
		m_siHouseSoulGuardID	= siHouseSoulGuardID;
	}
};

class sDBResponse_SoulGuardHouse_Change
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;						// 결과값

	SI32				m_siPersonID;
	// 보관소에 있었던 수호정령 정보
	SI32				m_siSoulGuardID;				// 수호정령 고유 ID(PersonID같이 유니크한 것임)

	TCHAR				m_szName[MAX_SOULGUARD_NAME];	// 수호정령 이름

	SI16				m_siStatus;						// 수호정령 상태
	SI16				m_siKind;						// 수호정령 KIND

	SI32				m_siLimitDateVary;				// 이때까지 살아있음

	SI16				m_siFellowShip;					// 친화도

public:
	sDBResponse_SoulGuardHouse_Change()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuardHouse_Change) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_HOUSE_CHANGE

// 캐릭터의 수호정령과 보관소의 수호정령을 맞교환 한다
class sDBRequest_SoulGuardHouse_Create
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	SI32			m_siHouseNum;	// 이값은 추가되는 갯수가 아니라 최대갯수를 보내줌

	SI16			m_siItemPos;	// 사용되는 아이템 위치
	cltItem			m_clItem;		// 사용되는 아이템

public:
	sDBRequest_SoulGuardHouse_Create()
	{
		ZeroMemory( this, sizeof(sDBRequest_SoulGuardHouse_Create) );
	}

	sDBRequest_SoulGuardHouse_Create( SI32 siPersonID, SI32 siHouseNum, SI16 siItemPos, cltItem* pclItem )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_SOULGUARD_HOUSE_CREATE;
		m_packetHeader.usSize	= sizeof(sDBRequest_SoulGuardHouse_Create);

		m_siPersonID			= siPersonID;

		m_siHouseNum			= siHouseNum;

		m_siItemPos				= siItemPos;
		if ( pclItem )
		{
			m_clItem.Set( pclItem );
		}
	}
};

class sDBResponse_SoulGuardHouse_Create
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;			// 결과값

	SI32			m_siPersonID;

	SI32			m_siHouseNum;		// 최대 사용 갯수

	SI16			m_siLeftItemPos;	// 사용한 아이템 위치
	cltItem			m_clLeftItem;		// 사용한 아이템

public:
	sDBResponse_SoulGuardHouse_Create()
	{
		ZeroMemory( this, sizeof(sDBResponse_SoulGuardHouse_Create) );
	}
};	// DBMSG_RESPONSE_SOULGUARD_HOUSE_CREATE

#endif