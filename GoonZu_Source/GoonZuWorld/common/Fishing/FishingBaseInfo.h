#ifndef _FISHINGBASEINFO_H_
#define _FISHINGBASEINFO_H_

#define MAX_FISHKIND_NUMBER			100

#include "../CommonHeader.h"
#include "../Agriculture/Agriculture.h"

#define FISHINGROD_ATB_HOOK						0x00000001
#define FISHINGROD_ATB_STAND					0x00000002

#define MAX_BAIT_ITEM_NUMBER					10			// 미끼의 최대 개수

#define MAX_FISHING_TOOL_NUMBER					10			// 낚시에 필요한 도구의 최대 개수.
#define FISHING_TOOL_DURABILITY_DECREASE		5			// 5씩 낚시 도구의 내구도가 깎인다.

#define MAX_CREATE_FISH_PLACE_LIST				30			// 30 군데에서 랜덤하게 생성된다.

enum { FISHING_PLACE_START, FISHING_PLACE_RIVER, FISHING_PLACE_SEASIDE, FISHING_PLACE_NEARSEA, FISHING_PLACE_FARSEA, FISHING_PLACE_END };


enum
{
	FISHING_SKILL_LEVEL_LOW = -5,			// 낚시 레벨이 낮아서 실패
	FISHING_TOOL_NOTEXIST,				// 도구 없음
	FISHING_TOOL_DURABILITY_TOO_LOW,	// 도구 내구도 부족
	FISHING_FAIL,						// 낚다가 실패
	FISHING_GET_SCROLL,					// 최대 내구 부적 낚음.
	FISHING_GET_GOLD,					// 금광석 낚음
	FISHING_SUCCESS						// 성공
};



#endif
