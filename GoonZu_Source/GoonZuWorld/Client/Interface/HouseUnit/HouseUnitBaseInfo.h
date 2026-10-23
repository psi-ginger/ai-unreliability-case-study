#ifndef _HOUSEUNITBASEINFO_H_
#define _HOUSEUNITBASEINFO_H_

#include < Directives.h >

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../../common/CommonHeader.h"

#define		HOUSEUNIT_SETTIMER_DEALY_TIME			1000

#define		TABCTRL_TAB_NUM							4

/*
//------------------------------------------ NPC 거래에서 사용되는 메시지 구조체
struct stNPCTradeData
{
	SI16		si16Msg;
	SI32		si32lParam;
	cltItem		clItem;
};

//------------------------------------------ NPC 거래에서 사용되는 메시지
#define	NPCTRADE_SELLITEM_TO_NPC		25000		// NPC 에게 아이템을 판매
#define NPCTRADE_BUYITEM_FROM_NPC		25001		// NPC로부터 아이템을 구매


*/

//------------------------------------------ 창고 거래에서 사용되는 메시지 구조체
struct stHouseUnitData
{	
	SI16			si16Msg;
	SI32			si32lParam1;
	SI32			si32lParam2;
	SI32			si32VillageUnique;					//창고가 있는 마을의 위치(어디 마을이냐)
	SI32			si32HouseUnitSlot;					//주택의 위치(어느 집이냐)
	cltItem			clItem;
	cltRentContract	clContract;
};

#define	HOUSEUNIT_INPUT_USERINVENTORY_TO_WAREHOUSE		26000
#define	HOUSEUNIT_OUTPUT_USERINVENTORY_FROM_WAREHOUSE	26001

// lParam = 선택한 생성할 아이템의 index, clItem 값 의미 없음.
#define	HOUSEUNIT_WAREHOUSE_MAKEITEM					26100

// 파라미터 없음
#define HOUSEUNIT_RETAL_EXTENSION						26002


#endif