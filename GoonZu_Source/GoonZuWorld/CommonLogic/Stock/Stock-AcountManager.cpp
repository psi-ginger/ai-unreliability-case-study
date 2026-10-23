//---------------------------------
// 2003/6/3 김태곤
//---------------------------------
#include "Stock.h"
#include "..\..\CommonLogic\CommonLogic.h"

#include "..\..\Resource.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------------
// cltStockAccount: 주식 계좌 정보. 
//----------------------------------------------------

// 특정 종목의 주식수를 설정한다. 
void cltStockAccount::Set(SI32 villageunique, SI32 amount)
{
	SI32 i;

	if(villageunique <= 0 && amount <= 0)return;

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		// 원하는 종목을 찾으면, 
		if(villageunique == clUnit[i].siVillageUnique)
		{
			clUnit[i].siAmount	= amount;
			if(amount == 0)
			{
				clUnit[i].siVillageUnique = 0;
			}
			return ;
		}
	}

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		// 빈 칸을 찾으면, 
		if(clUnit[i].siVillageUnique == 0)
		{
			clUnit[i].Init(villageunique, amount);
			return ;
		}
	}
}

// 특정 종목의 주식 수를 파악해서 리턴한다.
SI32 cltStockAccount::GetStockAmount(SI32 villageunique)
{
	SI32 i;

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		// 원하는 종목을 찾으면, 
		if(villageunique == clUnit[i].siVillageUnique)
		{
			return clUnit[i].siAmount;
		}
	}

	return 0;

}

// 주식을 추가한다. 
BOOL cltStockAccount::Add(SI32 villageunique, SI32 tradeamount)
{
	SI32 i;

	// 이미 같은 종목이 있는지 확인한다. 
	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		if(clUnit[i].siVillageUnique == villageunique)
		{
			clUnit[i].siAmount += tradeamount;
			return TRUE;
		}
	}

	// 새로운 종목으로 추가한다. 
	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		if(clUnit[i].siVillageUnique == 0)
		{
			clUnit[i].Init(villageunique, tradeamount);
			return TRUE;
		}
	}

	return FALSE;
}

// 주식을 뺀다.
BOOL cltStockAccount::Sub(SI32 villageunique, SI32 tradeamount)
{
	SI32 i;

	// 이미 같은 종목이 있는지 확인한다. 
	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		if(clUnit[i].siVillageUnique == villageunique)
		{
			//실제 보유주보다 더 많은 주를 빼면 실패. 
			if(tradeamount > clUnit[i].siAmount )return FALSE;

			clUnit[i].siAmount -= tradeamount;
			return TRUE;
		}
	}

	return FALSE;
}

