#ifndef _DBMSGSUMMONMARKET_H
#define _DBMSGSUMMONMARKET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\SummonMarket\SummonMarket.h"

//------------------------------------------
// 여러 개의 소환수거래소 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_SUMMONMARKET_ORDER_NUM	50


class sDBRequest_SummonMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBRequest_SummonMarketGetManyOrder()
	{
		ZeroMemory( this, sizeof(sDBRequest_SummonMarketGetManyOrder) );

		packetHeader.usSize = sizeof( sDBRequest_SummonMarketGetManyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_GETMANYORDER;
	}

};

class sDBResponse_SummonMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.
	
	UI16				usCount;							// 주문 개수
	cltSummonOrder		clOrder[ GET_MANY_SUMMONMARKET_ORDER_NUM ];

	sDBResponse_SummonMarketGetManyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketGetManyOrder));
	}
};



//------------------------------------------
// 소환수 주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_SummonMarketOrderSell
{
public:
//#if defined(_LEON_ADD_SELECTSELL_CODE)
	enum {
        TRADE_TYPE_NORMAL = 1,
		TRADE_TYPE_SELECTIVE = 2,
	};
//#endif

	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 주문을 행한 마을의 위치. 

	cltSummonOrder		clOrder;								// DB에 넣어야 할 주문의 내용. 

	GMONEY				siTax;									// 납부해야 할 세금. 

	TCHAR				szSelectPersonName[ MAX_PLAYER_NAME ];	// 지정거래시 구입할 person name 
																// NULL이면 지정거래아님

	SI16				siPos;
	cltItem				clitem;

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	UI08				uiTradeType;							// 일반 거래 : TRADE_TYPE_NORMAL, 지정 거래 : TRADE_TYPE_SELECTIVE
//#endif

	sDBRequest_SummonMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonMarketOrderSell));
	}

	sDBRequest_SummonMarketOrderSell(SI32 id, SI32 villageunique, cltSummonOrder* pclorder, GMONEY tax, TCHAR *personname, SI16 pos, cltItem *pclItem, UI08 tradetype )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonMarketOrderSell );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_ORDERSELL;
		usCharID		= id;

		siVillageUnique	= villageunique;
		clOrder.Set( pclorder );
		siTax			= tax;

		memcpy( szSelectPersonName, personname, MAX_PLAYER_NAME );
		szSelectPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;

		siPos = pos;
		clitem.Set( pclItem );

//#if defined(_LEON_ADD_SELECTSELL_CODE)
		uiTradeType = tradetype;
//#endif
	}
};


class sDBResponse_SummonMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	SI32				siVillageUnique;			// VillageUnique
	cltSummonOrder		clOrder;					// DB에 저장된 주문의 정보. 

	GMONEY				siTax;						// 납부한 세금. 

	cltMoney			clSummonMarketMoney;		// 소환수 거래소 보유자금
	GMONEY				siPersonMoney;				// person 소지금 

	SI16				siPos;						// 인벤토리 위치
	cltItem				clitem;						// 아이템


	sDBResponse_SummonMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketOrderSell));
	}
};

//------------------------------------------
// 소환수 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_SummonMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;	// 소환수 거래소가 위치한 곳의 VillageUnique
	cltFee				clFee;				// 설정해야 할 사복시의 수수료 정보


	sDBRequest_SummonMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonMarketSetFee));
	}

	sDBRequest_SummonMarketSetFee( SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonMarketSetFee );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_SETFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_SummonMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;
	SI32				siVillageUnique;	// 소환수거래소가 위치한 마을의 VillageUnique
	cltFee				clFee;				// 소환수거래소의 거래 수수료 정보. 

	sDBResponse_SummonMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketSetFee));
	}

};


//------------------------------------------
// 소환수거래소에서 소환수를 거래할 것을 요청 
//------------------------------------------

class sDBRequest_SummonMarketBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 주문자 person id
	SI32				siVillageUnique;	// 거래를 요청하는 마을의 VillageUnique. 	

	SI32				siOrderIndex;		// 구입할 소환수의 주문 번호
	GMONEY				siPrice;			// 거래 단가.

	sDBRequest_SummonMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonMarketBuy));
	}

	sDBRequest_SummonMarketBuy(SI32 id, SI32 personid, SI32 villageunique, SI32 orderindex, GMONEY price )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonMarketBuy );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_BUY;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;		
		siOrderIndex	= orderindex;
		siPrice			= price;
	}
};


class sDBResponse_SummonMarketBuy
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
	cltSummon			clSummon;			// 거래된 소환수 정보

	SI32				siSellerPersonID;	// 팔자의 PersonID
	cltMoney			clSellerBankMoney;	// 팔자의 은행 잔고

	sDBResponse_SummonMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketBuy));
	}
};



//------------------------------------------
//  나의 주문 정보를 요청한다. 
//------------------------------------------
class sDBRequest_SummonMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 Person

	sDBRequest_SummonMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonMarketMyOrder));
	}

	sDBRequest_SummonMarketMyOrder(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonMarketMyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_MYORDER;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_SummonMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;											// 결과값.

	UI16				usCount;
	cltSummonOrder		clMyOrder[ MAX_SUMMONMARKET_ORDER_PER_PERSON ];		// 나의 주문정보 
	
	sDBResponse_SummonMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketMyOrder));
	}
};


//------------------------------------------
// 소환수 주문을 취소한다 
//------------------------------------------
class sDBRequest_SummonMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상이 되는 Person
	SI32				siOrderIndex;	// 주문번호 

	sDBRequest_SummonMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonMarketCancelOrder));
	}

	sDBRequest_SummonMarketCancelOrder( SI32 id, SI32 personid, SI32 orderindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonMarketCancelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONMARKET_CANCELORDER;
		usCharID		= id;

		siPersonID		= personid;
		siOrderIndex	= orderindex;
	}

};


class sDBResponse_SummonMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person
	
	SI32				siOrderIndex;		// 삭제된 주문 번호 
	cltSummon			clSummon;			// 소환수 정보 

	sDBResponse_SummonMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonMarketCancelOrder));
	}
};


//------------------------------------------
// 주문 가격을 낮춘다. 
//------------------------------------------
class sDBRequest_ReduceSummonOrderPrice
{
public:
	sPacketHeader			packetHeader;

	int						arrayindex;
	cltSummonOrder			clOrder;

	sDBRequest_ReduceSummonOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ReduceSummonOrderPrice));
	}

	sDBRequest_ReduceSummonOrderPrice( int index, cltSummonOrder* pclorder)
	{
		packetHeader.usSize = sizeof( sDBRequest_ReduceSummonOrderPrice );
		packetHeader.usCmd = DBMSG_REQUEST_REDUCESUMMONORDERPRICE;

		arrayindex = index;
		clOrder.Set(pclorder);
	}
};


class sDBResponse_ReduceSummonOrderPrice
{
public:
	sPacketHeader			packetHeader;
	SI32					siResult;			// 결과값.

	int						arrayindex;
	cltSummonOrder			clOrder;			// DB에 저장된 주문의 정보. 

	sDBResponse_ReduceSummonOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ReduceSummonOrderPrice));
	}
};

#endif
