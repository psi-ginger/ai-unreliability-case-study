//---------------------------------
// 2003/6/3 김태곤
//---------------------------------
#include "Stock.h"
#include "..\..\CommonLogic\CommonLogic.h"

#include "..\..\Resource.h"

extern cltCommonLogic* pclClient;


//-----------------------------------------------
// 사자 팔자 주문 관리자. 
//-----------------------------------------------
cltStockOrderManager::cltStockOrderManager() : cltTradeOrderManager(MAX_STOCK_ORDER_NUMBER)
{

}

cltStockOrderManager::~cltStockOrderManager()
{

}





