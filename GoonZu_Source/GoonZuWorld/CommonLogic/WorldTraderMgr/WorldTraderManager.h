#ifndef _WORLDTRADERMANAGER_H
#define _WORLDTRADERMANAGER_H

#include <Directives.h>

#include "../../Server/NTradeServerProcess/TradeServerMsg.h"

class cltWorldTraderManager
{
public:
	cltWorldTraderManager();
	~cltWorldTraderManager();

	NSafeTArray<cltTradeServerOrder, MAX_TRADESERVER_ORDER_LIST>	clOrder;

	cltTradeServerOrder* GetTradeServerOrder( SI32 siOrderIndex );
};

#endif