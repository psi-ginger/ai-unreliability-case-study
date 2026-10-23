#ifndef _DBMSGSOULGUARD_H
#define _DBMSGSOULGUARD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//------------------------------------------
// 휴면 포인트로 아이템을 구입할 것을 요청. 
//------------------------------------------
class sDBRequest_BuyDormancyPointItem
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siPersonID;		// 대상  Person
	cltItem				m_clItem;			// 대상 아이템 정보. 
	bool				m_bPileSwitch;		// 집적 가능 여부. 
	SI32				m_siPoint;			// 설정될 휴면 포인트(아이템을 사고 남은 포인트)
	SI32				m_siItemPoint;		// 아이템 포인트 
	SI16				m_siItemPos;		// 아이템을 얻어올 인벤내 위치.

	sDBRequest_BuyDormancyPointItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_BuyDormancyPointItem));
	}

	sDBRequest_BuyDormancyPointItem(UI16 usCharID, SI32 siPersonID, cltItem* pclItem, bool bPilewitch, SI32 siPoint, SI32 siItemPoint, SI32 siItempos)
	{
		m_sPacketHeader.usSize = sizeof( sDBRequest_BuyDormancyPointItem);
		m_sPacketHeader.usCmd = DBMSG_REQUEST_DORMANCY_POINTITEM_BUY;
		m_usCharID		= usCharID;

		m_siPersonID	= siPersonID;
		m_clItem.Set(pclItem);
		m_bPileSwitch	= bPilewitch;
		m_siPoint		= siPoint;
		m_siItemPoint	= siItemPoint;
		m_siItemPos		= siItempos;
	}
};


class sDBResonse_BuyDormancyPointItem
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siResult;			// 결과값.
	SI32				m_siPersonID;		
	SI16				m_siItemPos;
	cltItem				m_clItem;			// 남은 아이템.
	SI32				m_siItemPoint;		// 아이템 포인트.
	SI32				m_siPoint;			// 설정될 휴면 포인트(아이템을 사고 남은 포인트)

	sDBResonse_BuyDormancyPointItem()
	{
		ZeroMemory(this, sizeof(sDBResonse_BuyDormancyPointItem));
	}
};

//------------------------------------------
// 휴면 포인트 관련 정보 저장 요청 
//------------------------------------------
class sDBRequest_SetDormancyPointInfo
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siPersonID;				// 대상  Person
	SI32				m_siPoint;					// 계산될 휴면 포인트
	SI32				m_siRemainedPlayTime;		// 플레이 시간
	
	sDBRequest_SetDormancyPointInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetDormancyPointInfo));
	}

	sDBRequest_SetDormancyPointInfo(UI16 usCharID, SI32 siPersonID, SI32 siPoint, SI32 siRemainedPlayTime)
	{
		m_sPacketHeader.usSize = sizeof( sDBRequest_SetDormancyPointInfo);
		m_sPacketHeader.usCmd = DBMSG_REQUEST_DORMANCY_POINTINFO_SET;
		m_usCharID			= usCharID;

		m_siPersonID		= siPersonID;
		m_siPoint			= siPoint;
		m_siRemainedPlayTime= siRemainedPlayTime;
	}
};

class sDBResonse_SetDormancyPointInfo
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siResult;				// 결과값.
	SI32				m_siPersonID;		
	SI32				m_siPoint;				// 계산된 휴면 포인트
	SI32				m_siRemainedPlayTime;	// 플레이 시간

	sDBResonse_SetDormancyPointInfo()
	{
		ZeroMemory(this, sizeof(sDBResonse_SetDormancyPointInfo));
	}
};

//------------------------------------------
// 휴면 계정 상태 변경
//------------------------------------------
class sDBRequest_ChangeDormancyState
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siPersonID;				// 대상  Person
	bool				m_bDormancyState;			// 휴면 상태 인지 아닌지

	sDBRequest_ChangeDormancyState()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeDormancyState));
	}

	sDBRequest_ChangeDormancyState(UI16 usCharID, SI32 siPersonID, bool bDormancyState)
	{
		m_sPacketHeader.usSize = sizeof( sDBRequest_ChangeDormancyState);
		m_sPacketHeader.usCmd = DBMSG_REQUEST_DORMANCY_STATE_CHANGE;
		m_usCharID			= usCharID;

		m_siPersonID		= siPersonID;
		m_bDormancyState	= bDormancyState;
	}
};

class sDBResponse_ChangeDormancyState
{
public:
	sPacketHeader		m_sPacketHeader;
	UI16				m_usCharID;

	SI32				m_siResult;				// 결과값.
	SI32				m_siPersonID;		
	bool				m_bDormancyState;		// 휴면 상태 인지 아닌지

	sDBResponse_ChangeDormancyState()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeDormancyState));
	}
};


#endif