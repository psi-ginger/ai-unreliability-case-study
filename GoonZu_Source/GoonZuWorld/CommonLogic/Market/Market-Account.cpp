//---------------------------------
// 2003/6/20 김태곤
//---------------------------------
#include "..\..\CommonLogic\CommonLogic.h"

#include "..\..\Resource.h"

#include "Market.h"

extern cltCommonLogic* pclClient;

//--------------------------------------------------------------------
//	cltmarketAccount
//--------------------------------------------------------------------

// 종목을 추가한다. 
BOOL cltMarketAccount::Add(cltItem* pclitem, SI32 tradeamount)
{
	SI32 i;
	BOOL pileswitch = FALSE;			// 아이템의 중첩 허용 여부. 

	if(pclitem == NULL)return FALSE;

	// 중첩이 안되는 아이템인지 확인한다. 
	SI32 type = pclitem->GetType(pclClient->pclItemManager);
	if(type)
	{
		SI64 atb = pclClient->pclItemManager->pclItemTypeInfo[type]->siAtb;

		// ITEMATB_OBJECT이 아니면 중첩을 허용한다. 
		if((atb & ITEMATB_OBJECT) == 0)
		{
			pileswitch = TRUE;
		}
	}

	// 중첩이 허용되는 아이템이면, 
	if(pileswitch == TRUE)
	{
		// 이미 같은 종목이 있는지 확인한다. 
		for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
		{
			if(clItem[i].siUnique == pclitem->siUnique)
			{
				clItem[i].Increase(tradeamount);
				
				return TRUE;
			}
		}
	}

	// 새로운 종목으로 추가한다. 
	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		if(clItem[i].siUnique == 0)
		{
			clItem[i].Set(pclitem);
			clItem[i].SetItemNum(tradeamount);
			return TRUE;
		}
	}


	return FALSE;
}

// 종목을 뺀다.
BOOL cltMarketAccount::Sub(cltItem* pclitem, SI32 tradeamount)
{
	SI32 i;

	// 이미 같은 종목이 있는지 확인한다. 
	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		cltItem* pclSrcItem = &clItem[i];

		if(pclSrcItem->IsSame(pclitem, false) == TRUE)
		{
			//실제 보유주보다 더 많은 주를 빼면 실패. 
			if(tradeamount > pclSrcItem->GetItemNum())return FALSE;

			pclSrcItem->Decrease(tradeamount);
			if(pclSrcItem->GetItemNum() == 0)
			{
				pclSrcItem->Init();
			}

			return TRUE;
		}
	}

	return FALSE;
}

// 특정한 아이템을 찾아서 그 포인터를 돌려준다. 
cltItem* cltMarketAccount::FindItem(const cltItem* pclitem, bool numcomp)
{
	SI32 i;

	if(pclitem == NULL)return NULL;

	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		if(clItem[i].IsSame( (cltItem*)pclitem, numcomp )== TRUE)
		{
			return &clItem[i];
		}
	}

	return NULL;
}

// 특정 아이템의 수를 파악해서 리턴한다.
SI32 cltMarketAccount::GetItemNum(cltItem* pclitem)
{
	cltItem* pclsrcitem = FindItem(pclitem, false);
	if(pclsrcitem == NULL)return 0;

	return pclsrcitem->GetItemNum();
}

BOOL cltMarketAccount::IsSame(cltMarketAccount* pclaccount)
{
	SI32 i;

	if(pclaccount == NULL)return FALSE;

	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		// 한개의 아이템이라도 서로 틀리다면 틀린 것이다. 
		if(clItem[i].IsSame(&pclaccount->clItem[i], true) == false)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// 객주 주문 정보를 업데이트 한다. 
bool cltMarketAccount::UpdateMarketOrder(SI32 index, cltTradeOrder* pclorder)
{
	if(pclorder == NULL)return false;

	// 가격이 0 이면 주문을 삭제하는 것으로 인식한다. 
	if(pclorder->siPrice == 0)
	{
		clItem[index].Init();
	}
	else
	{
		clItem[index].Set(&pclorder->clItemData);
	}

	
	clMoney[index].Set(pclorder->siPrice);
	
	
	return true;

}


BOOL cltNewMarketAccount::IsSame(cltNewMarketAccount* pclaccount)
{
	SI32 i;

	if(pclaccount == NULL)return FALSE;

	for(i = 0;i < MAX_NEWMARKET_ORDER_PER_PERSON;i++)
	{
		// 한개의 아이템이라도 서로 틀리다면 틀린 것이다. 
		if(clItem[i].IsSame(&pclaccount->clItem[i], true) == false)
		{
			return FALSE;
		}
	}

	return TRUE;
}

SI32 cltNewMarketAccount::GetItemNum(cltItem* pclitem)
{
	cltItem* pclsrcitem = FindItem(pclitem, false);
	if(pclsrcitem == NULL)return 0;

	return pclsrcitem->GetItemNum();
}

cltItem* cltNewMarketAccount::FindItem(const cltItem* pclitem, bool numcomp)
{
	SI32 i;

	if(pclitem == NULL)return NULL;

	for(i = 0;i < MAX_NEWMARKET_ORDER_PER_PERSON;i++)
	{
		if(clItem[i].IsSame((cltItem*)pclitem, numcomp)== TRUE)
		{
			return &clItem[i];
		}
	}

	return NULL;
}

