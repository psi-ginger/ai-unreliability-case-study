#ifndef _DBMSGPERSONALSHOP_H
#define _DBMSGPERSONALSHOP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//------------------------------------------
// 개인 상점을 시작하기위해서 Ticket을 사용한다.
//------------------------------------------
class sDBRequest_StartPersonalShop
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI16				siItemPos;
	SI16				siItemUnique;
	SI16				siItemNum;

	cltShopTradeData	clShopData;

	cltItem				clItem;

	sDBRequest_StartPersonalShop()
	{
		ZeroMemory(this, sizeof(sDBRequest_StartPersonalShop));
	}

	sDBRequest_StartPersonalShop(SI32 id,	   SI32 personid,   SI32 villageunique,	
							     SI16 itempos, SI16 itemunique, SI16 itemnum, cltShopTradeData	*pclshopdata, cltItem *pclItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_StartPersonalShop );
		packetHeader.usCmd = DBMSG_REQUEST_STARTPERSONALSHOP;
		
		siCharID	= id;
		siPersonID	= personid;
		
		siVillageUnique = villageunique;
		siItemPos		= itempos;
		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		clShopData.Set(pclshopdata);
		clItem.Set(pclItem);
	}

};

class sDBResponse_StartPersonalShop
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI16				siItemPos;
	cltItem				clItem;
	// 메세지 구분을 위해서 샵 모드를 추가 [성웅]
	SI08		    m_siShopMode;


	// PCK : 로그용 패킷 추가
	cltItem				clItem_Log;

	sDBResponse_StartPersonalShop()
	{
		ZeroMemory( this, sizeof( sDBResponse_StartPersonalShop ) );
	}
};
//------------------------------------------
// 개인상점을 통해 Person간 Item을 돈을 주고 한다.
//------------------------------------------
class sDBRequest_PersonalShopBuyItem
{
public:
	sPacketHeader		packetHeader;
	SI16				siCharID;
	SI32				siBuyerPersonID;	

	SI16				siShopperCharID;
	SI32				siShopperPersonID;	
	
	SI16				siBuyerInvPos;		// Buyer에게 구입한 아이템을 넣어야하는 Inventory 위치 
	SI16				siShopperInvPos;	// Shoper의 해당 Inventory 위치 
	
	SI16				siShowCaseIndex;
	GMONEY				siItemCost;			// Buyer에게서 빼야하는 MONEY

	cltItem				clBuyItem;			// Shopper에게서 빼서 Buyer에게 넣어야하는 아이템 정보 
	bool				bPileSwitch;		// 중첩 허용 여부. 	
	
	sDBRequest_PersonalShopBuyItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonalShopBuyItem));
	}

	sDBRequest_PersonalShopBuyItem(SI32 id,			SI32 buyerpersonid, 
							SI32 shopperid,			SI32 shopperpersonid, 
							SI16 buyerInv,			SI16 shopperInv, 
							SI16 shwcaseindex,	    GMONEY sicost,
							cltItem* pclbuyitem,	bool bpile)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonalShopBuyItem );
		packetHeader.usCmd  = DBMSG_REQUEST_PERSONALSHOPBUYITEM;

		siCharID			= id;
		siBuyerPersonID		= buyerpersonid;

		siShopperCharID		= shopperid;
		siShopperPersonID	= shopperpersonid;	

		siBuyerInvPos		= buyerInv;
		siShopperInvPos		= shopperInv;

		siShowCaseIndex		= shwcaseindex;
		siItemCost			= sicost;

		clBuyItem.Set(pclbuyitem);
		bPileSwitch			= bpile;
	}
};


class sDBResponse_PersonalShopBuyItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값. (성공 1:  실패 : 0)

	SI16				siCharID;
	SI32				siBuyerPersonID;	

	SI16				siShopperCharID;
	SI32				siShopperPersonID;	

	SI16				siBuyerInvPos;		// Buyer에게 구입한 아이템을 넣어야하는 Inventory 위치 
	SI16				siShopperInvPos;	// Shoper의 해당 Inventory 위치 

	cltItem				clBuyerInvItem;		// Buyer의 Inventory에 설정되어야하는 Item 정보 
	cltItem				clShopperInvItem;	// Shopper Inventory에 설정되어야하는 Item 정보 

	GMONEY				siBuyerMoney;		// Buyer의 보유 현금 
	GMONEY				siShopperMoney;		// Buyer의 보유 현금 

	GMONEY				siItemCost;			// 비용 지불 돈 
	SI16				siShowCaseIndex;	// Showcase Index
	SI32				siBuyItemNum;		// clBuyItem.siItemNum 의 정보를 받아 몇개를 샀는지 인지한다.
	
	//[추가 : 황진성 2007. 10. 24 로그에 이용. Buyer가 순수하게 사들이는 아이템]
	cltItem				clBuyItem;			

	sDBResponse_PersonalShopBuyItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonalShopBuyItem));
	}

};


//------------------------------------------
// 개인상점을 통해 Person간 Item을 돈을 주고 한다.
//------------------------------------------
class sDBRequest_PersonalShopSellItem
{
public:
	sPacketHeader		packetHeader;
	SI16				siCharID;
	SI32				siSellerPersonID;	

	SI16				siShopperCharID;
	SI32				siShopperPersonID;	
	
	SI16				siSellerInvPos;		// Seller의 해당 Inventory 위치 
	SI16				siShopperInvPos;	// Seller에게 구입한 아이템을 넣어야하는 Inventory 위치 
	
	SI16				siShowCaseIndex;
	GMONEY				siItemCost;			// Shopper에게서 빼야하는 MONEY

	cltItem				clSellItem;			// Seller에게서 빼서 Shopper에게 넣어야하는 아이템 정보 
	bool				bPileSwitch;		// 중첩 허용 여부. 	
	
	sDBRequest_PersonalShopSellItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonalShopSellItem));
	}

	sDBRequest_PersonalShopSellItem(SI32 id,			SI32 sellerpersonid, 
							SI32 shopperid,			SI32 shopperpersonid, 
							SI16 sellerInv,			SI16 shopperInv, 
							SI16 shwcaseindex,	    GMONEY sicost,
							cltItem* pclsellitem,	bool bpile)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonalShopSellItem );
		packetHeader.usCmd  = DBMSG_REQUEST_PERSONALSHOPSELLITEM;

		siCharID			= id;
		siSellerPersonID	= sellerpersonid;

		siShopperCharID		= shopperid;
		siShopperPersonID	= shopperpersonid;	

		siSellerInvPos		= sellerInv;
		siShopperInvPos		= shopperInv;

		siShowCaseIndex		= shwcaseindex;
		siItemCost			= sicost;

		clSellItem.Set(pclsellitem);
		bPileSwitch			= bpile;
	}
};


class sDBResponse_PersonalShopSellItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값. (성공 1:  실패 : 0)

	SI16				siCharID;
	SI32				siSellerPersonID;	

	SI16				siShopperCharID;
	SI32				siShopperPersonID;	

	SI16				siSellerInvPos;		// Seller의 해당 Inventory 위치 
	SI16				siShopperInvPos;	// Seller에게 구입한 아이템을 넣어야하는 Inventory 위치 

	cltItem				clSellerInvItem;	// Seller의 Inventory에 설정되어야하는 Item 정보 
	cltItem				clShopperInvItem;	// Shopper  Inventory에 설정되어야하는 Item 정보 

	GMONEY				siSellerMoney;		// Seller의 보유 현금 
	GMONEY				siShopperMoney;		// Shopper의 보유 현금 

	GMONEY				siItemCost;			// 비용 지불 돈 
	SI16				siShowCaseIndex;	// Showcase Index
	SI32				siSellItemNum;		// clSellItem.siItemNum 의 정보를 받아 몇개를 샀는지 인지한다.
	
	// PCK - 로그용 패킷
	cltItem				clSellerInvItem_Log;

	sDBResponse_PersonalShopSellItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonalShopSellItem));
	}

};



#endif
