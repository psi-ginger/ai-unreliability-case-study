#ifndef _FOREIGNMERCHANTBASEINFO_H_
#define _FOREIGNMERCHANTBASEINFO_H_

#include <Directives.h>
#include <windows.h>

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"
#include "..\..\..\CommonLogic\ImageList\ImageList.h"
#include "Msg\MsgType-NPC.h"
#include "..\..\FontManager\FontManager.h"
#include "..\..\Inventory\ItemInventoryBaseDefinition.h"

#define FOREIGNMERCHANTDLG_SETTIMER_TIME			1000

#define MAX_FOREIGN_MERCHANT_OBJ_NUM				30
#define MAX_FOREIGN_MERCHANT_OBJ_BUFFER_SIZE		128		// 128 byte 면 충분


struct stForeignMerchantHeader
{
	UI16 uiMsg;
	UI16 uiSize;
};

struct stSellToForeignMerchant
{
	stForeignMerchantHeader sHeader;
	SI16					siForeignMerchantKind;		// 상인 구분(일본 상인, 청상인)
	UI16					uiSelectedIndex;			// 아이템 위치 (당연히 10이상의 값이겠지... 장착창에서는 바로 팔수 없으므로... )
	UI16					uiSellItemNum;				// 아이템 판매 개수
	SI16					siSellItemUnique;			// 아이템 유니크 값, <--- 버그의 위험성 때문에... 기간이 끝나는 시점에서 다른 아이템으로 변경되는 동안에 보내진 패킷일 경우에 대한 대비책
	
	SI32					siNPCCharUnique;			// NPC CharUnique

	stSellToForeignMerchant()
	{
		siForeignMerchantKind = 0;
		uiSelectedIndex = 0;
		uiSellItemNum = 0;
		siSellItemUnique = 0;
		siNPCCharUnique = 0;

		sHeader.uiMsg = SELL_TO_FOREIGNMERCHANT;
		sHeader.uiSize = sizeof( stSellToForeignMerchant );
	}
};

#endif