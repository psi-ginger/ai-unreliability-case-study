#pragma once

//#include "../SoulGuardMgr/SoulGuardMgr.h"

// 박스에서 휴면 계정 보상 아이템 꺼내기 요청
class cltGameMsgRequest_DormancyTakeOutEquipItemInBox
{
public:
	// 상자 유니크
	SI32	m_siBoxUnique;
	// 상자 아이템 인벤토리 위치
	SI16	m_siBoxPos;
	// 꺼낼 장비 아이템 유니크
	SI16	m_siEquipItemUnique;
	// 꺼낼 장비 아이템의 박스에서의 인덱스
	SI16	m_siEquipItemIndex;
public:
	cltGameMsgRequest_DormancyTakeOutEquipItemInBox( SI32 siBoxUnique,SI16	siBoxPos, SI16 siEquipItemUnique,SI16 siEquipItemIndex )
	{
		m_siBoxUnique	= siBoxUnique;
		m_siBoxPos	= siBoxPos;
		m_siEquipItemUnique	= siEquipItemUnique;
		m_siEquipItemIndex = siEquipItemIndex;
	}
};	// GAMEMSG_REQUEST_DORMANCY_TAKEOUTEQUIPITEMINBOX

// 박스에서 휴면 계정 보상 아이템 꺼내기 응답
class cltGameMsgResponse_DormancyTakeOutEquipItemInBox
{
public:
	cltItem	m_clBoxItem;	// 사용하고 남은 상자 아이템
	SI16	m_siBoxItemPos;	// 

	cltItem	m_clGetItem;	// 받은 아이템
	SI16	m_siGetItemPos;	//

public:
	cltGameMsgResponse_DormancyTakeOutEquipItemInBox( cltItem* pclBoxItem, SI16 siBoxItemPos, cltItem* pclGetItem, SI16 siGetItemPos )
	{
		if ( pclBoxItem )
		{
			m_clBoxItem.Set( pclBoxItem );
		}
		m_siBoxItemPos = siBoxItemPos;

		if ( pclGetItem )
		{
			m_clGetItem.Set( pclGetItem );
		}
		m_siGetItemPos = siGetItemPos;

	}
};	// GAMEMSG_RESOPONSE_DORMANCY_TAKEOUTEQUIPITEMINBOX

// 서버에서 주기적으로 클라이언트에게 휴면 포인트 갱신 정보 제공
class cltGameMsgResponse_DormancyRefreshPointInfo
{
public:
	SI16		m_siPoint;					// 휴면 포인트
	SI16		m_siRemainedTime;			// 휴면 포인트를 얻기 위해 플레이 시간
	BOOL		m_bDormancyState;			// 휴면 상태
	SI16		m_siStartedYear;			// 시작 년
	SI16		m_siStartedMonth;			// 시작 월
	SI16		m_siStartedDay;			// 시작 일

public:
	cltGameMsgResponse_DormancyRefreshPointInfo( SI16 siPoint, SI16 siRemainedTime, BOOL bDormancyState ,SI16 siStartedYear, SI16 siStartedMonth, SI16 siStartedDay)
	{
		m_siPoint = siPoint;
		m_siRemainedTime = siRemainedTime;
		m_bDormancyState = bDormancyState;

		m_siStartedYear = siStartedYear;
		m_siStartedMonth = siStartedMonth;
		m_siStartedDay = siStartedDay;
	}
};	// GAMEMSG_RESPONSE_DORMANCY_REFRESHPOINTINFO

// 휴면 포인트로 아이템 구입 요청
class cltGameMsgRequest_DormancyBuyPoinItem
{
public:
	// 아이템 유니크
	SI32	m_siItemUnique;
public:
	cltGameMsgRequest_DormancyBuyPoinItem( SI32 siItemUnique)
	{
		m_siItemUnique	= siItemUnique;	
	}
};	// GAMEMSG_REQUEST_DORMANCY_BUYPOINTITEM
