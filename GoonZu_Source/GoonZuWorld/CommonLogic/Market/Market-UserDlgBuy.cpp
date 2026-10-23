//---------------------------------
// 2003/6/24 김태곤
//---------------------------------
#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\Resource.h"

#include "Market.h"
#include <JYLibFunction.h>

static bool bSelectedSwitch = false;

extern cltCommonLogic* pclClient;


// 서버에서 받은 주문 정보를 보관한다. 
// 클라이언트에서 원래 요청했던 정보를 받은 것이다. 
BOOL cltMarketManager::PutResponseOrder(cltTradeOrder* pclorder)
{

	if(siResponseOrderIndex >= MAX_RESPONSE_ORDER_NUMBER)return FALSE;

	clResponseOrder[siResponseOrderIndex].Set(pclorder);

	siResponseOrderIndex++;

	// 화면의 정보를 갱신하도록 한다. 
	ShowResponseOrderSiwtch = TRUE;
	return TRUE;



}
