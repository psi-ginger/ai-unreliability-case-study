#ifndef _DBMSGREALESTATEMARKET_H
#define _DBMSGREALESTATEMARKET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\RealEstate\RealEstate.h"

//------------------------------------------
// 여러 개의 거간 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_REALESTATEMARKET_ORDER_NUM	50


class sDBRequest_RealEstateMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBRequest_RealEstateMarketGetManyOrder()
	{
		ZeroMemory( this, sizeof(sDBRequest_RealEstateMarketGetManyOrder) );

		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketGetManyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_GETMANYORDER;
	}

};

class sDBResponse_RealEstateMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.
	
	UI16				usCount;							// 주문 개수
	cltRealEstateOrder	clOrder[ GET_MANY_REALESTATEMARKET_ORDER_NUM ];

	sDBResponse_RealEstateMarketGetManyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketGetManyOrder));
	}
};




//------------------------------------------
// 거간 주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_RealEstateMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 주문을 행한 마을의 위치. 

	cltRealEstateOrder	clOrder;				// DB에 넣어야 할 주문의 내용. 

	GMONEY				siTax;					// 납부해야 할 세금. 

	sDBRequest_RealEstateMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealEstateMarketOrderSell));
	}

	sDBRequest_RealEstateMarketOrderSell(SI32 id, SI32 villageunique, cltRealEstateOrder* pclorder, GMONEY tax)
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketOrderSell );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_ORDERSELL;
		usCharID		= id;

		siVillageUnique	= villageunique;
		clOrder.Set( pclorder );
		siTax			= tax;
	}
};


class sDBResponse_RealEstateMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	SI32				siVillageUnique;			// VillageUnique
	cltRealEstateOrder	clOrder;					// DB에 저장된 주문의 정보. 

	GMONEY				siTax;						// 납부한 세금. 

	cltMoney			clRealEstateMarketMoney;	// 거간 보유자금
	GMONEY				siPersonMoney;				// person 소지금 

	sDBResponse_RealEstateMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketOrderSell));
	}
};

//------------------------------------------
// 사복시 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_RealEstateMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;	// 사복시가 위치한 곳의 VillageUnique
	cltFee				clFee;				// 설정해야 할 사복시의 수수료 정보


	sDBRequest_RealEstateMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealEstateMarketSetFee));
	}

	sDBRequest_RealEstateMarketSetFee( SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee )
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketSetFee );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_SETFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_RealEstateMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;
	SI32				siVillageUnique;	// 사복시가 위치한 마을의 VillageUnique
	cltFee				clFee;				// 사복시의 거래 수수료 정보. 

	sDBResponse_RealEstateMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketSetFee));
	}

};


//------------------------------------------
// 거간에서 부동산을 거래할 것을 요청 
//------------------------------------------

class sDBRequest_RealEstateMarketBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 주문자 person id
	SI32				siVillageUnique;	// 거래를 요청하는 마을의 VillageUnique. 	

	SI32				siOrderIndex;		// 구입할 부동산의 주문 번호 
	SI32				siHouseVillage;		// 구입할 부동산이 위치한 마을 
	SI32				siHouseIndex;		// 구입할 부동산의 시전번호 
	GMONEY				siPrice;			// 거래 단가.

	sDBRequest_RealEstateMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealEstateMarketBuy));
	}

	sDBRequest_RealEstateMarketBuy(SI32 id, SI32 personid, SI32 villageunique, SI32 orderindex, SI32 housevillage, SI32 houseindex, GMONEY price )
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketBuy );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_BUY;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;		
		siOrderIndex	= orderindex;
		siHouseVillage  = housevillage;
		siHouseIndex	= houseindex;
		siPrice			= price;
	}
};


class sDBResponse_RealEstateMarketBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 구입한 Person 
	TCHAR				szPersonName[ MAX_PLAYER_NAME ];
	GMONEY				siMoney;			// 거래 후 소지금 
	SI32				siVillageUnique;	// Request에서 입력된 VillageUnique
	SI32				siOrderIndex;		// 주문 번호

	GMONEY				siTradePrice;		// 실제로 거래된 가격. (거래가 이루어지지 않았다면 0)
 	SI32				siHouseVillage;		// 거래 부동산 위치 마을 
	SI32				siHouseIndex;		// 거래 부동산 시전 번호
	
	SI32				siSellerPersonID;	// 팔자의 PersonID
	cltMoney			clSellerBankMoney;	// 팔자의 은행 잔고

	sDBResponse_RealEstateMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketBuy));
	}
};


//------------------------------------------
// 거간 나의 주문 정보를 요청한다. 
//------------------------------------------
class sDBRequest_RealEstateMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 Person

	sDBRequest_RealEstateMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealEstateMarketMyOrder));
	}

	sDBRequest_RealEstateMarketMyOrder(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketMyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_MYORDER;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_RealEstateMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;											// 결과값.

	UI16				usCount;
	cltRealEstateOrder	clMyOrder[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];		// 나의 주문정보 
	
	sDBResponse_RealEstateMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketMyOrder));
	}
};


//------------------------------------------
// 거간 주문을 취소한다 
//------------------------------------------
class sDBRequest_RealEstateMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상이 되는 Person
	SI32				siVillageUnique;
	SI32				siHouseIndex;

	sDBRequest_RealEstateMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealEstateMarketCancelOrder));
	}

	sDBRequest_RealEstateMarketCancelOrder( SI32 id, SI32 personid, SI32 villageunique, SI32 houseindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketCancelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_CANCELORDER;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique = villageunique;
		siHouseIndex    = houseindex;
	}

};


class sDBResponse_RealEstateMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person
	
	SI32				siOrderIndex;		// 삭제된 주문 번호 
	SI32				siVillageUnique;
	SI32				siHouseIndex;

	sDBResponse_RealEstateMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealEstateMarketCancelOrder));
	}
};


//------------------------------------------
// 주문 가격을 낮춘다. 
//------------------------------------------
class sDBRequest_ReduceRealEstateOrderPrice
{
public:
	sPacketHeader			packetHeader;

	int						arrayindex;
	cltRealEstateOrder		clOrder;

	sDBRequest_ReduceRealEstateOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ReduceRealEstateOrderPrice));
	}

	sDBRequest_ReduceRealEstateOrderPrice( int index, cltRealEstateOrder* pclorder)
	{
		packetHeader.usSize = sizeof( sDBRequest_ReduceRealEstateOrderPrice );
		packetHeader.usCmd = DBMSG_REQUEST_REDUCEREALESTATEORDERPRICE;

		arrayindex = index;
		clOrder.Set(pclorder);
	}
};


class sDBResponse_ReduceRealEstateOrderPrice
{
public:
	sPacketHeader			packetHeader;
	SI32					siResult;			// 결과값.

	int						arrayindex;
	cltRealEstateOrder		clOrder;			// DB에 저장된 주문의 정보. 

	sDBResponse_ReduceRealEstateOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ReduceRealEstateOrderPrice));
	}
};

class sDBRequest_RealEstateMarketDelOrder
{
public:
	sPacketHeader			packetHeader;
	
	SI32					siOrderIndex;
	SI32					siPersonID;

	sDBRequest_RealEstateMarketDelOrder( SI32 orderindex, SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_RealEstateMarketDelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_REALESTATEMARKET_DELORDER;

		siOrderIndex = orderindex;
		siPersonID = personid;
	}

};

class sDBResponse_RealEstateMarketDelOrder
{
public:
	sPacketHeader			packetHeader;
	SI32					siResult;			// 결과값.

	SI32					siOrderIndex;
	SI32					siPersonID;

	sDBResponse_RealEstateMarketDelOrder()
	{
		ZeroMemory( this, sizeof( sDBResponse_RealEstateMarketDelOrder ) );
	}
};


#endif
