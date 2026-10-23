#include <CommonLogic.h>

#include "WorldTraderManager.h"

#include "../../Resource.h"

#include "Char/CharCommon/Char-Common.h"

#include "../Server/Server.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// cltWorldTraderManager
//----------------------------------------------
cltWorldTraderManager::cltWorldTraderManager()
{
	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
	{
		clOrder[i].Init();
	}
}

cltWorldTraderManager::~cltWorldTraderManager()
{
	return ;
}

cltTradeServerOrder* cltWorldTraderManager::GetTradeServerOrder( SI32 siOrderIndex )
{
	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
	{
		if( clOrder[i].siIndex == siOrderIndex )
			return &clOrder[i];
	}

	return NULL;
}