#ifdef __SUN_NNPCTRADE1__


#ifndef _NPCTRADEBASEINFO_H_
#define _NPCTRADEBASEINFO_H_

#include < Directives.h >

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\Common\Item\ItemCommon\NPCItem.h"
#include "..\..\..\CommonLogic\ImageList\ImageList.h"
#include "..\..\..\Common\SystemNPC\SystemNPC.h"
#include <CommonLogic.h>


#define		NPCTRADEINVENTORY_SETTIMER_DEALY_TIME			1000

//------------------------------------------ NPC 거래에서 사용되는 메시지 구조체
struct stNPCTradeData
{
	SI16		si16Msg;
	SI32		si32lParam;
	SI32		siNPCCharUnique;
	cltItem		clItem;
};

//------------------------------------------ NPC 거래에서 사용되는 메시지
#define	NPCTRADE_SELLITEM_TO_NPC		25000		// NPC 에게 아이템을 판매
#define NPCTRADE_BUYITEM_FROM_NPC		25001		// NPC로부터 아이템을 구매

#endif

#endif