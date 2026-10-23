#ifndef _DBMSGSUMMONHEROMARKET_H
#define _DBMSGSUMMONHEROMARKET_H

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\SummonHeroMarket\SummonHeroMarket.h"

//------------------------------------------
// 여러 개의 비변사 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_SUMMONHEROMARKET_ORDER_NUM	50


class sDBRequest_SummonHeroMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBRequest_SummonHeroMarketGetManyOrder()
	{
		ZeroMemory( this, sizeof(sDBRequest_SummonHeroMarketGetManyOrder) );

		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketGetManyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_GETMANYORDER;
	}

};

class sDBResponse_SummonHeroMarketGetManyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.
	
	UI16				usCount;							// 주문 개수
	cltSummonOrder		clOrder[ GET_MANY_SUMMONHEROMARKET_ORDER_NUM ];

	sDBResponse_SummonHeroMarketGetManyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketGetManyOrder));

		for( SI32 siCount = 0; siCount < GET_MANY_SUMMONHEROMARKET_ORDER_NUM; ++siCount )
			clOrder[siCount].Init();
	}
};



//------------------------------------------
// 소환영웅 주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_SummonHeroMarketOrderSell
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

	SI32				siVillageUnique;		// 주문을 행한 마을의 위치. 

	cltSummonOrder		clOrder;				// DB에 넣어야 할 주문의 내용. 

	GMONEY				siTax;					// 납부해야 할 세금. 

	TCHAR				szSelectPersonName[ MAX_PLAYER_NAME ];	// 지정거래시 구입할 person name 
																// NULL이면 지정거래아님

	SI16				siPos;
	cltItem				clitem;

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	UI08				uiTradeType;							// 일반 거래 : TRADE_TYPE_NORMAL, 지정 거래 : TRADE_TYPE_SELECTIVE
//#endif

	sDBRequest_SummonHeroMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonHeroMarketOrderSell));
	}

	sDBRequest_SummonHeroMarketOrderSell(SI32 id, SI32 villageunique, cltSummonOrder* pclorder, SI32 tax, TCHAR *personname, SI16 pos, cltItem *pclItem, UI08 tradetype )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketOrderSell );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_ORDERSELL;
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


class sDBResponse_SummonHeroMarketOrderSell
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	SI32				siVillageUnique;			// VillageUnique
	cltSummonOrder		clOrder;					// DB에 저장된 주문의 정보. 

	GMONEY				siTax;						// 납부한 세금. 

	cltMoney			clSummonMarketMoney;		// 소환영웅 거래소 보유자금
	GMONEY				siPersonMoney;				// person 소지금 

	SI16				siPos;						// 인벤토리 위치
	cltItem				clitem;						// 아이템

	sDBResponse_SummonHeroMarketOrderSell()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketOrderSell));
	}
};

//------------------------------------------
// 소환영웅 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_SummonHeroMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;	// 소환영웅 거래소가 위치한 곳의 VillageUnique
	cltFee				clFee;				// 설정해야 할 사복시의 수수료 정보


	sDBRequest_SummonHeroMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonHeroMarketSetFee));
	}

	sDBRequest_SummonHeroMarketSetFee( SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketSetFee );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_SETFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_SummonHeroMarketSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;
	SI32				siVillageUnique;	// 소환영웅거래소가 위치한 마을의 VillageUnique
	cltFee				clFee;				// 소환영웅거래소의 거래 수수료 정보. 

	sDBResponse_SummonHeroMarketSetFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketSetFee));
	}

};


//------------------------------------------
// 소환영웅거래소에서 소환영웅를 거래할 것을 요청 
//------------------------------------------

class sDBRequest_SummonHeroMarketBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 주문자 person id
	SI32				siVillageUnique;	// 거래를 요청하는 마을의 VillageUnique. 	

	SI32				siOrderIndex;		// 구입할 소환영웅의 주문 번호
	GMONEY				siPrice;			// 거래 단가.

	sDBRequest_SummonHeroMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonHeroMarketBuy));
	}

	sDBRequest_SummonHeroMarketBuy(SI32 id, SI32 personid, SI32 villageunique, SI32 orderindex, GMONEY price )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketBuy );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_BUY;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;		
		siOrderIndex	= orderindex;
		siPrice			= price;
	}
};


class sDBResponse_SummonHeroMarketBuy
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
	cltSummon			clSummon;			// 거래된 소환영웅 정보

	SI32				siSellerPersonID;	// 팔자의 PersonID
	cltMoney			clSellerBankMoney;	// 팔자의 은행 잔고

	sDBResponse_SummonHeroMarketBuy()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketBuy));
	}
};



//------------------------------------------
//  나의 주문 정보를 요청한다. 
//------------------------------------------
class sDBRequest_SummonHeroMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 Person

	sDBRequest_SummonHeroMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonHeroMarketMyOrder));
	}

	sDBRequest_SummonHeroMarketMyOrder(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketMyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_MYORDER;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_SummonHeroMarketMyOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;											// 결과값.

	UI16				usCount;
	cltSummonOrder		clMyOrder[ MAX_SUMMONHEROMARKET_ORDER_PER_PERSON ];		// 나의 주문정보 
	
	sDBResponse_SummonHeroMarketMyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketMyOrder));
	}
};


//------------------------------------------
// 소환영웅 주문을 취소한다 
//------------------------------------------
class sDBRequest_SummonHeroMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상이 되는 Person
	SI32				siOrderIndex;	// 주문번호 

	sDBRequest_SummonHeroMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonHeroMarketCancelOrder));
	}

	sDBRequest_SummonHeroMarketCancelOrder( SI32 id, SI32 personid, SI32 orderindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonHeroMarketCancelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER;
		usCharID		= id;

		siPersonID		= personid;
		siOrderIndex	= orderindex;
	}

};


class sDBResponse_SummonHeroMarketCancelOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person
	
	SI32				siOrderIndex;		// 삭제된 주문 번호 
	cltSummon			clSummon;			// 소환영웅 정보 

	sDBResponse_SummonHeroMarketCancelOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonHeroMarketCancelOrder));
	}
};


//------------------------------------------
// 주문 가격을 낮춘다. 
//------------------------------------------
class sDBRequest_ReduceSummonHeroOrderPrice
{
public:
	sPacketHeader			packetHeader;

	int						arrayindex;
	cltSummonOrder			clOrder;

	sDBRequest_ReduceSummonHeroOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ReduceSummonHeroOrderPrice));
	}

	sDBRequest_ReduceSummonHeroOrderPrice( int index, cltSummonOrder* pclorder)
	{
		packetHeader.usSize = sizeof( sDBRequest_ReduceSummonHeroOrderPrice );
		packetHeader.usCmd = DBMSG_REQUEST_REDUCESUMMONHEROORDERPRICE;

		arrayindex = index;
		clOrder.Set(pclorder);
	}
};


class sDBResponse_ReduceSummonHeroOrderPrice
{
public:
	sPacketHeader			packetHeader;
	SI32					siResult;			// 결과값.

	int						arrayindex;
	cltSummonOrder			clOrder;			// DB에 저장된 주문의 정보. 

	sDBResponse_ReduceSummonHeroOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ReduceSummonHeroOrderPrice));
	}
};

#endif
