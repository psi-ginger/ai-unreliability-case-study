#ifndef _DBMSGHORSEMARKET_H
#define _DBMSGHORSEMARKET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\HorseMarket\HorseMarket.h"

//------------------------------------------
// 여러 개의 사복시 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_HORSEMARKET_ORDER_NUM	50


class sDBRequest_HorseMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBRequest_HorseMarketGetManyOrder()
	{
		ZeroMemory( this, sizeof(sDBRequest_HorseMarketGetManyOrder) );

		packetHeader.usSize = sizeof( sDBRequest_HorseMarketGetManyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_GETMANYORDER;
	}

};

class sDBResponse_HorseMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.
	
	UI16				usCount;							// 주문 개수
	cltHorseOrder		clHorseOrder[ GET_MANY_HORSEMARKET_ORDER_NUM ];

	sDBResponse_HorseMarketGetManyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketGetManyOrder));
	}
};




//------------------------------------------
// 사복시 주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_HorseMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 주문을 행한 마을의 위치. 

	SI16				siHorseIndex;			// 말 슬롯
	cltHorseOrder		clHorseOrder;			// DB에 넣어야 할 주문의 내용. 

	GMONEY				siTax;					// 납부해야 할 세금. 

	sDBRequest_HorseMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBRequest_HorseMarketOrderSell));
	}

	sDBRequest_HorseMarketOrderSell(SI32 id, SI32 villageunique, SI16 horseindex, cltHorseOrder* pclorder, GMONEY tax )
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseMarketOrderSell );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_ORDERSELL;
		usCharID		= id;

		siVillageUnique	= villageunique;
		siHorseIndex = horseindex;
		clHorseOrder.Set( pclorder );
		siTax			= tax;
	}
};


class sDBResponse_HorseMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// VillageUnique
	SI16				siHorseIndex;
	cltHorseOrder		clHorseOrder;		// DB에 저장된 주문의 정보. 

	GMONEY				siTax;				// 납부한 세금. 

	cltMoney			clHorseMarketMoney;	// 사복시 보유자금
	GMONEY				siPersonMoney;		// person 소지금 

	sDBResponse_HorseMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketOrderSell));
	}
};

//------------------------------------------
// 사복시 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_HorseMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;	// 사복시가 위치한 곳의 VillageUnique
	cltFee				clFee;				// 설정해야 할 사복시의 수수료 정보


	sDBRequest_HorseMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_HorseMarketSetFee));
	}

	sDBRequest_HorseMarketSetFee( SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee )
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseMarketSetFee );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_SETFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_HorseMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;
	SI32				siVillageUnique;	// 사복시가 위치한 마을의 VillageUnique
	cltFee				clFee;				// 사복시의 거래 수수료 정보. 

	sDBResponse_HorseMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketSetFee));
	}

};


//------------------------------------------
// 사복시에서 말을 거래할 것을 요청 
//------------------------------------------

class sDBRequest_HorseMarketBuyHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 주문자 person id
	SI16				siHorseIndex;		// 말 슬롯
	SI32				siVillageUnique;	// 거래를 요청하는 마을의 VillageUnique. 	
	
	SI32				siOrderIndex;		// 구입할 말의 주문 번호 

	GMONEY				siPrice;			// 거래 단가.

	sDBRequest_HorseMarketBuyHorse()
	{
		ZeroMemory(this, sizeof(sDBRequest_HorseMarketBuyHorse));
	}

	sDBRequest_HorseMarketBuyHorse(SI32 id, SI32 personid, SI16 horseindex, SI32 villageunique, SI32 orderindex, GMONEY price )
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseMarketBuyHorse );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_BUYHORSE;
		usCharID		= id;

		siPersonID		= personid;
		siHorseIndex	= horseindex;
		siVillageUnique	= villageunique;		
		siOrderIndex	= orderindex;
		siPrice			= price;
	}
};


class sDBResponse_HorseMarketBuyHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 구입한 Person 
	GMONEY				siMoney;			// 거래 후 소지금 
	SI32				siVillageUnique;	// Request에서 입력된 VillageUnique
	SI32				siOrderIndex;		// 주문 번호

	GMONEY				siTradePrice;		// 실제로 거래된 가격. (거래가 이루어지지 않았다면 0)
 	cltHorse			clHorse;			// 거래 말 정보 
	SI16				siHorseIndex;		// 말 슬롯 
	
	SI32				siSellerPersonID;	// 팔자의 PersonID
	cltMoney			clSellerBankMoney;	// 팔자의 은행 잔고

	sDBResponse_HorseMarketBuyHorse()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketBuyHorse));
	}
};


//------------------------------------------
// 사복시 나의 주문 정보를 요청한다. 
//------------------------------------------
class sDBRequest_HorseMarketMyAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 Person

	sDBRequest_HorseMarketMyAccount()
	{
		ZeroMemory(this, sizeof(sDBRequest_HorseMarketMyAccount));
	}

	sDBRequest_HorseMarketMyAccount(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseMarketMyAccount );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_MYACCOUNT;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_HorseMarketMyAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltHorseOrder		clHorseOrder;		// 말 주문 정보 
	
	sDBResponse_HorseMarketMyAccount()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketMyAccount));
	}
};


//------------------------------------------
// 사복시 창고로부터 말을 꺼내올 것을 요청한다. 
//------------------------------------------
class sDBRequest_HorseMarketOutHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상이 되는 Person

	SI16				siHorseIndex;	// 말 슬롯

	sDBRequest_HorseMarketOutHorse()
	{
		ZeroMemory(this, sizeof(sDBRequest_HorseMarketOutHorse));
	}

	sDBRequest_HorseMarketOutHorse(SI32 id, SI32 personid, SI16 horseindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseMarketOutHorse );
		packetHeader.usCmd = DBMSG_REQUEST_HORSEMARKET_OUTHORSE;
		usCharID		= id;

		siPersonID		= personid;

		siHorseIndex	= horseindex;
	}

};


class sDBResponse_HorseMarketOutHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person
	
	SI32				siOrderIndex;		// 삭제된 주문 번호 
	cltHorse			clHorse;			// 말 정보 
	SI16				siHorseIndex;		// 말 슬롯 

	sDBResponse_HorseMarketOutHorse()
	{
		ZeroMemory(this, sizeof(sDBResponse_HorseMarketOutHorse));
	}
};


//------------------------------------------
// 주문 가격을 낮춘다. 
//------------------------------------------
class sDBRequest_ReduceHorseOrderPrice
{
public:
	sPacketHeader		packetHeader;

	int					arrayindex;
	cltHorseOrder		clOrder;

	sDBRequest_ReduceHorseOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ReduceHorseOrderPrice));
	}

	sDBRequest_ReduceHorseOrderPrice( int index, cltHorseOrder* pclorder)
	{
		packetHeader.usSize = sizeof( sDBRequest_ReduceHorseOrderPrice );
		packetHeader.usCmd = DBMSG_REQUEST_REDUCEHORSEORDERPRICE;

		arrayindex = index;
		clOrder.Set(pclorder);
	}
};


class sDBResponse_ReduceHorseOrderPrice
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	int					arrayindex;
	cltHorseOrder		clOrder;			// DB에 저장된 주문의 정보. 

	sDBResponse_ReduceHorseOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ReduceHorseOrderPrice));
	}
};

#endif
