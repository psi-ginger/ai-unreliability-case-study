#ifndef _PRIVATETRADEBASEINFO_H_
#define _PRIVATETRADEBASEINFO_H_

#include <Directives.h>

#include "..\..\..\Resource.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "..\..\..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"


#include "../../../Common/Char/CharCommon/Char-EconomyInfo.h"

#include "..\..\FontManager\FontManager.h"
#include "..\..\Inventory\ItemInventoryBaseDefinition.h"

#include "..\..\..\CommonLogic\Msg\MsgType-PrivateTrade.h"

#include "..\..\..\CommonLogic\Stock\Stock.h"




#define PRIVATETRADEDLG_SETTIMER_TIME				300

#define MAX_PRIVATETRADE_OBJ_NUM					30
#define MAX_PRIVATETRADE_OBJ_BUFFER_SIZE			128		// 128 byte 면 충분

#define PRIVATETRADE_CANCEL							5000
#define PRIVATETRADE_ADD_ITEM						5001
#define PRIVATETRADE_DEL_LIST						5002
#define PRIVATETRADE_DEL_ITEM						5003
#define PRIVATETRADE_PRICE							5004
#define PRIVATETRADE_STOCK							5005
#define PRIVATETRADE_HOUSEUNIT						5006
#define PRIVATETRADE_HORSE							5007
#define PRIVATETRADE_SHIP							5008
#define PRIVATETRADE_MYACCEPT						5050
#define PRIVATETRADE_MYCANCEL						5055

struct stPrivateTradeHeader
{
	UI16 uiMsg;
	UI16 uiSize;
};

struct stPrivateTrade_Cancel
{
	stPrivateTradeHeader sHeader;
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	stPrivateTrade_Cancel( SI32 CharUnique, SI16 PrivateTradeOrderMgrIndex, SI16 PrivateTradeOrderCountIndex )
	{
		sHeader.uiMsg = PRIVATETRADE_CANCEL;
		sHeader.uiSize = sizeof( stPrivateTrade_Cancel );

		siOtherCharUnique = CharUnique;
		siPrivateTradeOrderMgrIndex = PrivateTradeOrderMgrIndex;
		siPrivateTradeOrderCountIndex = PrivateTradeOrderCountIndex;
	}
};

struct stPrivateTrade_Add_Item
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16	siItemPos;
	cltItem	clItem;
	SI32	siAddItemNum;

	stPrivateTrade_Add_Item( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 ItemPos, cltItem *pclItem, SI32 AddItemNum )
	{
		sHeader.uiMsg = PRIVATETRADE_ADD_ITEM;
		sHeader.uiSize = sizeof( stPrivateTrade_Add_Item );

		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siAddItemNum = AddItemNum;
	}
};

struct stPrivateTrade_Del_List
{
	stPrivateTradeHeader sHeader;

	SI32	siOtherCharUnique;
	SI16	siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16	siPrivateTradeOrderCountIndex;

	SI16	siDelType;
	SI16	siParam;

	stPrivateTrade_Del_List( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 DelType, SI16 Param )
	{
		sHeader.uiMsg = PRIVATETRADE_DEL_LIST;
		sHeader.uiSize = sizeof( stPrivateTrade_Del_List );

		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siDelType = DelType;
		siParam = Param;		// 아이템을 리스트에서 지우라는 명령이면 아이템의 위치값이 된다.
	}
};

struct stPrivateTrade_Del_Item
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16	siDelIndex;
	cltItem	clItem;			

	stPrivateTrade_Del_Item( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 DelIndex, cltItem *pclItem )
	{
		sHeader.uiMsg = PRIVATETRADE_DEL_ITEM;
		sHeader.uiSize = sizeof( stPrivateTrade_Del_Item );

		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siDelIndex = DelIndex;
		clItem.Set( pclItem );
	}
};

struct stPrivateTrade_Price
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	GMONEY siPrice;

	stPrivateTrade_Price( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, GMONEY Price )
	{
		sHeader.uiMsg = PRIVATETRADE_PRICE;
		sHeader.uiSize = sizeof( stPrivateTrade_Price );

		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siPrice = Price;
	}
};

struct stPrivateTrade_Stock
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI32 siStockVillageUnique;
	SI32 siStockNum;

	stPrivateTrade_Stock( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI32 StockVillageUnique, SI32 StockNum )
	{
		sHeader.uiMsg = PRIVATETRADE_STOCK;
		sHeader.uiSize = sizeof( stPrivateTrade_Stock );

		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siStockVillageUnique = StockVillageUnique;
		siStockNum = StockNum;
	}
};

struct stPrivateTrade_HouseUnit
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI32 siHouseVillageUnique;
	SI32 siHouseUnitSlot;

	stPrivateTrade_HouseUnit( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI32 HouseVillageUnique, SI32 HouseUnitSlot )
	{
		sHeader.uiMsg = PRIVATETRADE_HOUSEUNIT;
		sHeader.uiSize = sizeof( stPrivateTrade_HouseUnit );

		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siHouseVillageUnique = HouseVillageUnique;
		siHouseUnitSlot = HouseUnitSlot;
	}
};

struct stPrivateTrade_Horse
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16 siHorseIndex;

	stPrivateTrade_Horse( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 HorseIndex )
	{
		sHeader.uiMsg = PRIVATETRADE_HORSE;
		sHeader.uiSize = sizeof( stPrivateTrade_Horse );

		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
		
		siHorseIndex = HorseIndex;
	}
};

struct stPrivateTrade_MyAccept
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16 siAcceptCounter;

	stPrivateTrade_MyAccept( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 AcceptCounter )
	{
		sHeader.uiMsg = PRIVATETRADE_MYACCEPT;
		sHeader.uiSize = sizeof( stPrivateTrade_MyAccept );
		
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siAcceptCounter = AcceptCounter;
	}
};

struct stPrivateTrade_MyCancel
{
	stPrivateTradeHeader sHeader;

	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	
	stPrivateTrade_MyCancel( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex )
	{
		sHeader.uiMsg = PRIVATETRADE_MYCANCEL;
		sHeader.uiSize = sizeof( stPrivateTrade_MyCancel );
		
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
	}
};


class CPrivateTradeDlg;
class CNPrivateTradeDlg;
extern CPrivateTradeDlg g_PrivateTradeDlg;

#endif