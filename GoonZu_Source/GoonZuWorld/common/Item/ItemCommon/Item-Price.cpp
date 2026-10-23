//---------------------------------
// 2003/6/24 김태곤
//---------------------------------

#include <stdio.h>
#include "cltItem.h"
#include "NPCItem.h"

#include "../../../CommonLogic/CommonLogic.h"
#include "../../../Server/Server.h"

extern cltCommonLogic* pclClient;

cltItemPrice::cltItemPrice(cltItemManagerCommon* pclitemmanager)
{
	pclItemManager = pclitemmanager;

	SI32 i;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemManager->pclItemInfo[i])
		{
			clUnit[i].siUnique	= pclItemManager->pclItemInfo[i]->clItem.siUnique;
		}
	}

}

cltItemPrice::~cltItemPrice()
{
}

// 아이템의 가격을 설정한다.
void cltItemPrice::SetPrice(cltItemManagerCommon* pclitemmanager, SI32 unique, GMONEY price, SI32 itemnum, bool bforceswitch)
{
	SI32 index = pclitemmanager->FindItemRefFromUnique(unique);
	if(index <= 0)
	{
		return ;
	}

	if(bforceswitch == true)
	{
		clUnit[index].siItemNum = itemnum;
		clUnit[index].siPrice	= price;
		clUnit[index].fPrice	= (float)price;
		return ;
	}

	// 오브젝트성 물품은 계산 안해준다.
	if(pclitemmanager->GetItemAtb(unique) & ITEMATB_OBJECT)
	{
		return ;
	}

	// MIN_ITEM_PRICE냥 이하로는 떨어지지 않는다. 
	price = max(MIN_ITEM_PRICE, price);


	// 개수가 너무 많은 것도 제한한다. 
	if(itemnum > 500)itemnum = 500;


	if(clUnit[index].siPrice)
	{
		//---------------------------------------------------
		// 가격이 시세보다 5%보다 비싸다면 5%까지만 반영한다. 
		//---------------------------------------------------
		GMONEY maxprice = max(1, (clUnit[index].siPrice * 105 / 100));
		if(maxprice == clUnit[index].siPrice)
		{
			maxprice++;
		}

		//---------------------------------------------------
		// 가격이 시세보다 70%이하라면 70%까지만 반영한다.
		//---------------------------------------------------
		GMONEY minprice = max(1, (clUnit[index].siPrice * 70 / 100));
		if(minprice == clUnit[index].siPrice )
		{
			minprice = max(1, minprice-1);
		}
	
		// 비싸게 설정되는 것은 보수적으로 제한한다.
		if(price > maxprice)
		{
			price = maxprice;

			// 개수가 너무 많은 것도 제한한다. 
			if(itemnum > 10)itemnum = 10;
		}
		// 싸게 설정되는 것은 적극적으로 반영한다. 
		else if(price < minprice)
		{
			price = minprice;
			
			// 개수가 너무 많은 것도 제한한다. 
			if(itemnum > 3000)itemnum = 3000;
		}
	}

	clUnit[index].siUnique	= unique;

	if(clUnit[index].siItemNum == 0)
	{
		// 최초 가격은 100냥에서 시작한다. 
		if(price > 100)price = 100;

		SI64 oldPrice = clUnit[index].siPrice;

		clUnit[index].siItemNum = min(10, itemnum);
		clUnit[index].siPrice	= price;
		clUnit[index].fPrice	= (float)price;
	}
	else
	{
		SI32 orgitemnum = min(10000, clUnit[index].siItemNum);

		SI64 oldPrice = (SI64)clUnit[index].fPrice;

		clUnit[index].fPrice	= (float)( (orgitemnum * clUnit[index].fPrice) + (price * itemnum) ) / (orgitemnum + itemnum);
		clUnit[index].siPrice	= (SI64)clUnit[index].fPrice;
		clUnit[index].siItemNum = min(10000, clUnit[index].siItemNum + itemnum);
	}
}

// 아이템의 가격을 구한다. 
cltItemPriceUnit* cltItemPrice::GetPrice(cltItemManagerCommon* pclitemmanager,SI32 unique)
{
	SI32 index = pclitemmanager->FindItemRefFromUnique(unique);
	if(index <= 0)return NULL;

	return &clUnit[index];
}

// 객주 주문가격을 설정한다. 
void cltItemPrice::SetMarketOrderPrice(cltItemManagerCommon* pclitemmanager,SI32 unique, GMONEY price)
{
	SI32 index = pclitemmanager->FindItemRefFromUnique(unique);
	if(index <= 0)return ;

	clUnit[index].siUnique				= unique;
	clUnit[index].siMarketOrderPrice	= price;
}



// 객주 주문가격을 얻어온다. 
GMONEY cltItemPrice::GetMarketOrderPrice(SI32 unique)
{
	SI32 i;

	for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
	{
		if(clUnit[i].siUnique == unique)
		{
			return clUnit[i].siMarketOrderPrice;
		}
	}

	return 0;
}

// 특정 물품의 시세를 알려준다.(NPC용)
GMONEY cltItemPrice::GetPriceSellToNPC(cltItemManagerCommon* pclitemmanager, SI32 unique)
{
	//-------------------------
	// 아이템의 타입을 구한다. 
	//-------------------------
	cltItemInfo* pcliteminfo = pclitemmanager->FindItemInfo(unique);
	if(pcliteminfo == NULL)return 0;
	
	GMONEY price = 0;

	//---------------------------------------------------------------------------
	// 1. 아이템의 타입에 따라 가격을 정한다.
	//---------------------------------------------------------------------------
	switch(pcliteminfo->siType)
	{

	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:		
	case ITEMTYPE_BAG:
	case ITEMTYPE_MANTLE:
		price	= 200;	break;

	case ITEMTYPE_NECK:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
		price	= 100;	break;

	case ITEMTYPE_BOOK:		price	= 200;				break;
		
	case ITEMTYPE_HEAL:
		// 원료이면, (생산에 필요한 물품이 없는 것이라면) 
		if(pcliteminfo->clProductionInfo.clMaterial[0].siUnique == 0)
		{
			price	= 40;
		}
		else
		{
			price	= MIN_ITEM_PRICE;
		}
		break;

	default:				price	= MIN_ITEM_PRICE;	break;		
	}
	//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------
	// 2. 아이템의 유니크에 따라서 가격을 정한다.
	//---------------------------------------------------------------------------
	if ( ITEMUNIQUE(24105) == unique )
	{	
		// 대만은 금액이 다름
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan) )
		{
			price = 88888;
		}
		else
		{
			price = 1000000;
		}
	}
	//---------------------------------------------------------------------------

	return price;
}



