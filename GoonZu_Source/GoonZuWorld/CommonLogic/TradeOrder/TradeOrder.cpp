//---------------------------------
// 2003/6/20 김태곤
//---------------------------------

#include "TradeOrder.h"
#include "..\..\CommonLogic\CommonLogic.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------------------
//	cltTradeOrder
//----------------------------------------------------------
BOOL cltTradeOrder::IsSame(cltTradeOrder* porder)
{
	if(siPersonID	!= porder->siPersonID)			return FALSE;
	if(siPrice		!= porder->siPrice)				return FALSE;
	if(siAmount		!= porder->siAmount)			return FALSE;
	
	switch(siType)
	{
	case TRADEORDER_TYPE_STOCK:
		if(siVillageUnique != porder->GetVillageUnique())return FALSE;
		break;
	case TRADEORDER_TYPE_OBJECT:
		{
			cltItem clsrcitem;
			clsrcitem.Set(&clItemData);
			
			cltItem cldstitem;
			cldstitem.Set(&porder->clItemData);
			
			return clsrcitem.IsSame(&cldstitem, true);
		}
		break;
	default:
		MsgBox(TEXT("fd93"), TEXT("fd983:%d"),siType);
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------
// cltTradeOrderManager
//----------------------------------------------------------
cltTradeOrderManager::cltTradeOrderManager(SI32 maxordernum)
{
	SI32 i;

	siMaxOrderNum	= maxordernum;

	pclOrderSell	= new cltTradeOrder*[maxordernum];
	pclOrderBuy		= new cltTradeOrder*[maxordernum];


	for(i = 0;i < siMaxOrderNum;i++)
	{
		pclOrderSell[i] = new cltTradeOrder;	// 팔자 주문 
		pclOrderBuy[i]	= new cltTradeOrder;	// 사자 주문 	
	}


}

cltTradeOrderManager::~cltTradeOrderManager()
{
	SI32 i;

	for(i = 0;i < siMaxOrderNum;i++)
	{
		if(pclOrderSell[i])
		{
			delete pclOrderSell[i];
			pclOrderSell[i] = NULL;
		}

		if(pclOrderBuy[i])
		{
			delete pclOrderBuy[i];
			pclOrderBuy[i] = NULL;
		}
	}

	if(pclOrderSell)
	{
		delete[] pclOrderSell;
		pclOrderSell = NULL;
	}
	if(pclOrderBuy)
	{
		delete[] pclOrderBuy;
		pclOrderBuy = NULL;
	}

}

// 주문을 주문 버퍼의 정해진 자리에 넣는다. 
bool cltTradeOrderManager::SetOrder(SI32 ordermode, cltTradeOrder* porder)
{
	if(porder == NULL)return false;

	cltTradeOrder** ppDstStockOrder = NULL;			// 주문. 

	if(ordermode == ORDERMODE_SELL)
	{
		ppDstStockOrder	= pclOrderSell;
	}
	else
	{
		ppDstStockOrder	= pclOrderBuy;
	}
	
	if(porder->siType == 1)
	{
		if(porder->siAmount != 0 && porder->siVillageUnique == 0)
		{
			TCHAR* pTitle = GetTxtFromMgr(422);
			TCHAR* pText = GetTxtFromMgr(1681);
			MsgBox(pTitle, pText);
			return false;
		}
	}


	// 정해진 위치에 주문의 내용을 넣는다.
	if(porder->siIndex < 0 || porder->siIndex >= siMaxOrderNum)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("SetOrder():0 ordermode:%d index:%d type:%d villageunique:%d price:%d"), ordermode, porder->siIndex, porder->siType, porder->siVillageUnique, porder->siPrice);
		return false;
	}
	if(ppDstStockOrder[porder->siIndex] == NULL)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("SetOrder():1 ordermode:%d index:%d type:%d villageunique:%d price:%d"), ordermode, porder->siIndex, porder->siType, porder->siVillageUnique, porder->siPrice);
		return false;
	}

	// 주문 가격이 0 이면 삭제로 작용. 
	if(porder->siPrice == 0)
	{
		ppDstStockOrder[porder->siIndex]->Init();
	}
	else
	{
		ppDstStockOrder[porder->siIndex]->Set(porder);
	}
	
	return true;
}

// 주어진 주문에 적절한 반대 주문을 찾는다. 
// 찾으면 그 반대 주문의 포인터를 리턴하고 못 찾으면 NULL을 리턴한다. 
cltTradeOrder* cltTradeOrderManager::FindOpositeOrder(SI32 ordermode, cltTradeOrder* porder)
{

	SI32 i;
	SI32 bestindex = -1;
	GMONEY bestprice;
	
	if(ordermode == ORDERMODE_SELL)
	{
		bestprice			= porder->siPrice;

		// 조건을 충족시킬 수 있는 상대 주문이 있는지 확인한다. 
		for(i = 0;i < siMaxOrderNum;i++)
		{
			// 같은 종목의 주식을 찾는다. 
			if(IsSameKind(pclOrderBuy[i], porder) == TRUE)
			{
				// 가격이 유효한 것을 찾는다.
				if(pclOrderBuy[i]->siPrice >=  porder->siPrice)
				{
					if(pclOrderBuy[i]->siPrice >= bestprice)
					{
						// 동일인의 주문은 취급하지 않는다. 
						if(pclOrderBuy[i]->siPersonID != porder->siPersonID)
						{
							bestindex	= i;
							bestprice	= pclOrderBuy[i]->siPrice;
						}
					}
				}
			}
		}

		if(bestindex >= 0)
		{
			return pclOrderBuy[bestindex];
		}
	}
	else
	{
		bestprice			= porder->siPrice;

		// 조건을 충족시킬 수 있는 상대 주문이 있는지 확인한다. 
		for(i = 0;i < siMaxOrderNum;i++)
		{
			// 같은 종목의 주식을 찾는다. 
			if(IsSameKind(pclOrderSell[i], porder) == TRUE)
			{
				// 가격이 유효한 것을 찾는다.
				if(pclOrderSell[i]->siPrice <=  porder->siPrice)
				{
					if(pclOrderSell[i]->siPrice <= bestprice)
					{
						// 동일인의 주문은 취급하지 않는다. 
						if(pclOrderSell[i]->siPersonID != porder->siPersonID)
						{
							bestindex	= i;
							bestprice	= pclOrderSell[i]->siPrice;
						}
					}
				}
			}
		}

		if(bestindex >= 0)
		{
			return pclOrderSell[bestindex];
		}
	}
	
	return NULL;
}


bool cltTradeOrderManager::GetMostSellBuyOrder(IN cltTradeOrder* porder, OUT cltTradeOrder* pSellMostOrderList, OUT cltTradeOrder* pBuyMostOrderList, OUT GMONEY * averageSell, OUT  GMONEY * averageBuy)
{
	// 모든 주문을 검색하여 가장 싼 주문 10개와 가장 비싼 주문 10개를 찾아낸다.

	GMONEY worstPrice = 0;
	GMONEY sumSell = 0;
	GMONEY sumBuy = 0;
	SI32 cntSell = 0;
	SI32 cntBuy = 0;

	SI32 i = 0 , j = 0, k = 0;
	
	// 가장 싼 주문을 10개 찾는다.

	for(i = 0;i < siMaxOrderNum;i++)
	{
		// 같은 종목의 주식을 찾는다. 
		if(IsSameKind(pclOrderSell[i], porder) == TRUE )
		{
			sumSell += pclOrderSell[i]->siPrice;
			cntSell ++;

			// 일단 빈 목록은 무조건 채워넣고 본다.
			if( pSellMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1].IsValid() == false )
			{
				for( j = 0; j < MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM; j ++ )
				{
					if( pSellMostOrderList[ j ].IsValid() == false )
					{
						pSellMostOrderList[ j ].Set( pclOrderSell[i] );
						
						break;
					}
					else if( pSellMostOrderList[ j ].siPrice > pclOrderSell[i]->siPrice )
					{
						// 목록을 하나씩 밀고 그자리에 값을 넣는다.
						for( k = MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1 ; k > j ; k-- )
						{							
							pSellMostOrderList[ k ].Set( &pSellMostOrderList[ k - 1 ] );
						}
						
                        pSellMostOrderList[ j ].Set( pclOrderSell[ i ] );
						break;	
					}					
				}				
			}
			// 마지막 가장 비싼 목록과 비교하여 더 싼 주식이 있다면 목록 안으로 집어 넣는다.
			else if( pSellMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1].siPrice > pclOrderSell[i]->siPrice )
			{
				for( j = 0; j < MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM; j ++ )
				{
					if( pSellMostOrderList[ j ].siPrice > pclOrderSell[i]->siPrice )
					{
						// 목록을 하나씩 밀고 그자리에 값을 넣는다.
						for( k = MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1 ; k > j ; k-- )
						{							
							pSellMostOrderList[ k ].Set( &pSellMostOrderList[ k - 1 ] );
						}

						pSellMostOrderList[ j ].Set( pclOrderSell[ i ] );
						break;
					}
				}
			}
		}


		// 같은 종목의 주식을 찾는다. 
		if(IsSameKind(pclOrderBuy[i], porder) == TRUE )
		{
			sumBuy += pclOrderBuy[i]->siPrice;
			cntBuy ++;

			// 일단 빈 목록은 무조건 채워넣고 본다.
			if( pBuyMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1].IsValid() == false )
			{
				for( j = 0; j < MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM; j ++ )
				{
					if( pBuyMostOrderList[ j ].IsValid() == false )
					{
						pBuyMostOrderList[ j ].Set( pclOrderBuy[i] );

						break;
					}
					else if( pBuyMostOrderList[ j ].siPrice < pclOrderBuy[i]->siPrice )
					{
						// 목록을 하나씩 밀고 그자리에 값을 넣는다.
						for( k = MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1 ; k > j ; k-- )
						{							
							pBuyMostOrderList[ k ].Set( &pBuyMostOrderList[ k - 1 ] );
						}

						pBuyMostOrderList[ j ].Set( pclOrderBuy[ i ] );
						break;

					}					
				}				
			}
			// 마지막 가장 비싼 목록과 비교하여 더  비싼 주식 주문이 있다면 목록 안으로 집어 넣는다.
			else if( pBuyMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1].siPrice < pclOrderBuy[i]->siPrice )
			{
				for( j = 0; j < MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM; j ++ )
				{
					if( pBuyMostOrderList[ j ].siPrice < pclOrderBuy[i]->siPrice )
					{
						// 목록을 하나씩 밀고 그자리에 값을 넣는다.
						for( k = MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM - 1 ; k > j ; k-- )
						{							
							pBuyMostOrderList[ k ].Set( &pBuyMostOrderList[ k - 1 ] );
						}

						pBuyMostOrderList[ j ].Set( pclOrderBuy[ i ] );
						break;
					}
				}
			}
		}
	}

	if( cntSell > 0 )
	{
		*averageSell = sumSell / cntSell;		
	}
	else
	{
		averageSell = 0;
	}
	
	if( cntBuy > 0 )
	{
		*averageBuy = sumBuy / cntBuy;		
	}
	else
	{
		averageBuy = 0;
	}	

	
	return true;
}

// PersonID명의의 주문을 모두 취소하게 한다. 
BOOL cltTradeOrderManager::CancelOrder(SI32 personid)
{
	SI32 i;

	for(i = 0;i < siMaxOrderNum;i++)
	{
		if(pclOrderSell[i]->siPersonID == personid)
		{
			pclOrderSell[i]->Init();
		}

		if(pclOrderBuy[i]->siPersonID == personid)
		{
			pclOrderBuy[i]->Init();
		}

	}

	return TRUE;
}

// 주문 리스트에서 인덱스의 주문 정보를 확보한다. 
cltTradeOrder* cltTradeOrderManager::GetOrder(SI32 ordermode, SI32 index)
{
	if(index < 0 || index >= siMaxOrderNum)return NULL;
	
	if(ordermode == ORDERMODE_SELL)
	{
		return pclOrderSell[index];
	}
	else
	{
		return pclOrderBuy[index];
	}
}


// 같은 종목인지 확인한다. 
BOOL cltTradeOrderManager::IsSameKind(const cltTradeOrder* pclsrc, const cltTradeOrder* pcldst)
{

	if(pclsrc->siType != pcldst->siType)return FALSE;

	
	switch(pclsrc->siType )
	{
		// 주식인 경우 
	case TRADEORDER_TYPE_STOCK:
		if(pclsrc->GetVillageUnique() == pcldst->GetVillageUnique())return TRUE;
		break;
	case TRADEORDER_TYPE_OBJECT:
		{
			cltItem clItem1, clItem2;
			clItem1.Set(&pclsrc->clItemData);
			clItem2.Set(&pcldst->clItemData);
			return clItem1.IsSame(&clItem2, true);
		}
		break;

	default:
		MsgBox(TEXT("cltTradeOrderManager::IsSameKind()"), TEXT("fd98yj:%d"), pclsrc->siType);
	}
	
	return FALSE;
}

// 물품 주문중 가장 저렴한 주문의 정보를 찾는다. 
bool cltTradeOrderManager::FindLowPriceOrderInfo(SI32 unique, GMONEY* pprice, SI32* pamount)
{
	SI32	i;
	GMONEY minprice	= 100000000;
	SI32 minamount	= -1;

	cltItem  clItem;
	for(i = 0;i < siMaxOrderNum;i++)
	{
		clItem.Set(&pclOrderSell[i]->clItemData);
		
		if(clItem.siUnique == unique)
		{
			if(pclOrderSell[i]->siPrice < minprice && pclOrderSell[i]->bBankError == false )
			{
				minprice	= pclOrderSell[i]->siPrice;
				minamount	= pclOrderSell[i]->siAmount;
			}
		}
	}

	if(minamount > 0)
	{
		*pprice		=	minprice;
		*pamount	=	minamount;
		return true;
	}
	return false;
}


// 주식  주문중 가장 저렴한 주문의 정보를 찾는다. 
bool cltTradeOrderManager::FindLowPriceStockOrderInfo(SI32 villageunique, GMONEY* pprice, SI32* pamount)
{
	SI32	i;
	GMONEY minprice	= 100000000;
	SI32 minamount	= -1;

	if(villageunique <= 0)return false;

	for(i = 0;i < siMaxOrderNum;i++)
	{
		if(villageunique == pclOrderSell[i]->siVillageUnique)
		{
			if(pclOrderSell[i]->siPrice < minprice)
			{
				minprice	= pclOrderSell[i]->siPrice;
				minamount	= pclOrderSell[i]->siAmount;
			}
		}
	}

	if(minamount > 0)
	{
		*pprice		=	minprice;
		*pamount	=	minamount;
		return true;
	}
	return false;
}

// 주식 주문중 가장 비싼 주문의 정보를 찾는다. 
bool cltTradeOrderManager::FindHighPriceStockOrderInfo(SI32 villageunique, GMONEY* pprice, SI32* pamount)
{
	SI32	i;
	GMONEY maxprice	= 0;
	SI32 minamount	= -1;

	if(villageunique <= 0)return false;
	
	for(i = 0;i < siMaxOrderNum;i++)
	{
		if(villageunique == pclOrderBuy[i]->siVillageUnique)
		{
			if(pclOrderBuy[i]->siPrice > maxprice)
			{
				maxprice	= pclOrderBuy[i]->siPrice;
				minamount	= pclOrderBuy[i]->siAmount;
			}
		}
	}

	if(minamount > 0)
	{
		*pprice		=	maxprice;
		*pamount	=	minamount;
		return true;
	}
	return false;
}


// 주문의 개수를 구한다. 
SI32 cltTradeOrderManager::GetOrderNum(SI32 ordermode)
{
	SI32 i;
	SI32 num = 0;
	cltTradeOrder** ppOrder;

	if(ordermode == ORDERMODE_SELL)
	{
		ppOrder = pclOrderSell;
	}
	else
	{
		ppOrder = pclOrderBuy;
	}

	for(i = 0;i < siMaxOrderNum;i++)
	{
		if(ppOrder[i]->siAmount)
		{
			num ++;
		}
	}

	return num;
}

//----------------------------------------------
// cltCharOrderInfo
//----------------------------------------------
#ifdef _SAFE_MEMORY
	SI32 cltCharOrderInfo::GetOrderNumber()
#else
	SI32 cltCharOrderInfo::GetOrderNumber()const
#endif
{
	SI32 i;
	SI32 num = 0;

	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		// 개수가 있는 것만 유효하다. 
		if(clOrderUnit[i].clOrder.siAmount)
		{
			num++;
		}
	}

	return num;
}

// 주문 정보를 설정한다.
BOOL cltCharOrderInfo::Set(SI32 ordermode, cltTradeOrder* pclorder)
{
	SI32 i;
	
	//-----------------------------------------------
	// 이미 같은 인덱스의 주문이 있는지 확인한다. 
	//-----------------------------------------------
	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		if(clOrderUnit[i].siOrderMode)
		{
			// 주문의 종류도 같고 주문번호도 같다면 실제로는 같은 주문이다. 
			// 이런 경우에는 새로 주문을 추가하지 않고 기존 주문을 수정한다. 
			if(clOrderUnit[i].siOrderMode			== ordermode
				&& clOrderUnit[i].clOrder.siIndex	== pclorder->siIndex)
			{
				// 무효한 주문이 오면 초기화한다. 
				if(pclorder->siAmount)
				{
					clOrderUnit[i].clOrder.Set(pclorder);
				}
				else
				{
					clOrderUnit[i].siOrderMode	= 0;
					clOrderUnit[i].clOrder.Init();
				}
				
				return TRUE;
			}
		}
	}
	
	//------------------------------------------------
	// 기존의 주문이 없다면, 
	//------------------------------------------------
	for(i = 0; i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		if(clOrderUnit[i].siOrderMode == 0)
		{
			// 무효한 주문이 오면 초기화한다. 
			if(pclorder->siAmount)
			{
				clOrderUnit[i].siOrderMode	= ordermode;
				clOrderUnit[i].clOrder.Set(pclorder);
			}
			else
			{
				clOrderUnit[i].siOrderMode	= 0;
				clOrderUnit[i].clOrder.Init();
			}
			return TRUE;
		}
	}
	return FALSE;
}
