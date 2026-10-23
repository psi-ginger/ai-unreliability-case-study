//---------------------------------
// 2003/6/24 김태곤
//---------------------------------
#ifndef _ITEMCOMMON_H
#define _ITEMCOMMON_H

#include "..\..\..\Lib\EtcTool\EtcTool.h"

#define MAX_ITEMINFO_NUMBER		4000
#define MIN_DUR_FOR_REPAIR		60		// 수리를 하기 위한 최소 내구도. 이 이하에서는 수리 불가.

#define LOW_DURABILITY_NOTIFY	30		// 내구가 부족함을 알리는 기준 ( 내구 30 부터 )

#define RARELEVEL_1				2		// 호품 등급. 	
#define RARELEVEL_2				1		// 명품 등급. 
#define RARELEVEL_3				3		// 신기 등급. 
#define RARELEVEL_4				4		// 명품 등급. 
#define RARELEVEL_5				5		// 고급품 등급. 
#define RARELEVEL_6				6		// 고물품 등급. 

#define NEWYEAR_EFFECT			7		// [추가 : 황진성 2007. 12. 12 신년 이벤트 기간동안 과금 아이템 빤짝!]

#define MARRIED_FULLDRESS_EFFECT	8	// 추가 : 결혼예복 이펙트

#define MAX_ATTENDANCE_COUNT	100		// 100일까지 정보를 저장할 수 있다.

// [영훈] 기자단 모자 (2008.06.05)
const SI32 CONST_JOURNALIST_HAT_ITEMUNIQUE = /*ITEMUNIQUE*/(23470);

enum ITEMREPAIR_MODE
{
	ITEMREPAIR_MODE_NONE = 0,

	ITEMREPAIR_MODE_SELF,				// 직접수리
	ITEMREPAIR_MODE_REQUEST,			// 수리 의뢰
	ITEMREPAIR_MODE_REQUEST_PRECESS,	// 수리 의뢰품 수리
	ITEMREPAIR_MODE_CANCEL,				// 수리 의뢰중 취소
	ITEMREPAIR_MODE_GROWUP_MAX,			// 최대 내구도 증가
	ITEMREPAIR_MODE_NPC,				// NPC에 의한 수리

	ITEMREPAIR_MODE_MAX
	
};

// 내구도 수리 요청 이용권
const SI32 Const_RepairRequestTicket_ItemUnique = /*ITEMUNIQUE*/(24124);


class cltAttendanceReward
{
public:
	SI16	m_siItemUnique;
	SI16	m_siItemNum;
	SI16	m_uiDateUseDay;

public:
	cltAttendanceReward()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

#endif