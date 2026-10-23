#ifndef _DBMSGMARKET_H
#define _DBMSGMARKET_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//------------------------------------------
// 여러개의 객주 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_MARKET_ORDER_NUM	50
class sDBRequest_GetManyMarketOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siStartIndex;	//  얻어와야 할 정보의 시작 인덱스 

	sDBRequest_GetManyMarketOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetManyMarketOrder));
	}

	sDBRequest_GetManyMarketOrder(SI32 id, SI32 ordermode, SI32 startindex)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetManyMarketOrder );
		packetHeader.usCmd = DBMSG_REQUEST_GETMANYMARKETORDER;
		usCharID		= id;

		siOrderMode		= ordermode;
		siStartIndex	= startindex;
	}
};

class sDBResponse_GetManyMarketOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siStartIndex;		// 얻어온 주문의 시작 인덱스.
	cltTradeOrder		clTradeOrder[GET_MANY_MARKET_ORDER_NUM];

	sDBResponse_GetManyMarketOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetManyMarketOrder));
	}
};




//------------------------------------------
// 주문 가격을 낮춘다. 
//------------------------------------------
class sDBRequest_ReduceOrderPrice
{
public:
	sPacketHeader		packetHeader;

	cltTradeOrder		clOrder;

	// PCK : 깎이기 전의 가격
	GMONEY	siPrice;

	sDBRequest_ReduceOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ReduceOrderPrice));
	}

	sDBRequest_ReduceOrderPrice(cltTradeOrder* pclorder, GMONEY Price)
	{
		packetHeader.usSize = sizeof( sDBRequest_ReduceOrderPrice );
		packetHeader.usCmd = DBMSG_REQUEST_REDUCEORDERPRICE;

		clOrder.Set(pclorder);
		siPrice = Price;
	}
};


class sDBResponse_ReduceOrderPrice
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI16				siMarketIndex;		// 객주내의 내 물품중에서의 인덱스. 
	cltTradeOrder		clOrder;			// DB에 저장된 주문의 정보. 

	// PCK : 로그용 패킷
	GMONEY	siPrice;						// 깎이기 전의 물픔가격.
	sDBResponse_ReduceOrderPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ReduceOrderPrice));
	}
};


//------------------------------------------
// 객주주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_OrderMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siVillageUnique;	// 주문을 행한 마을의 위치. 
	cltTradeOrder		clOrder;		// DB에 넣어야 할 주문의 내용. 

	SI16				siItemPos;		// Person에서 아이템이 위치하고 있는 곳. 
	bool				bPileSwitch;	// 옮길 아이템이 중첩되는지 여부. 

	GMONEY				siTax;			// 납부해야 할 세금. 

	sDBRequest_OrderMarket()
	{
		ZeroMemory(this, sizeof(sDBRequest_OrderMarket));
	}

	sDBRequest_OrderMarket(SI32 id, SI32 ordermode, SI32 villageunique, cltTradeOrder* pclorder, SI32 itempos, bool bpileswitch, GMONEY tax)
	{
		packetHeader.usSize = sizeof( sDBRequest_OrderMarket );
		packetHeader.usCmd = DBMSG_REQUEST_ORDERMARKET;
		usCharID		= id;

		siOrderMode		= ordermode;
		siVillageUnique	= villageunique;
		clOrder.Set(pclorder);

		siItemPos		= itempos;
		bPileSwitch		= bpileswitch;
		siTax			= tax;
	}
};


class sDBResponse_OrderMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siOrderMode;		// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	cltTradeOrder		clOrder;			// DB에 저장된 주문의 정보. 

	SI16				siItemFromPos;		// Person에서 아이템이 위치하던 곳. 
	cltItem				clFromItem;			// Person내 아이템의 최신 정보. 

	GMONEY				siTax;				// 납부해야 할 세금. 

	//[추가 : 황진성 2007. 10. 25 로그에서 이용, 파는 아이템 정보]
	cltItem				clOrderItem;

	sDBResponse_OrderMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_OrderMarket));
	}
};

//------------------------------------------
// 한개 아이템 가격을 DB에 저장한다. 
//------------------------------------------
class sDBRequest_SetItemPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siItemUnique;
	GMONEY				siPrice;
	REAL64				fPrice;
	SI32				siItemNum;
	TCHAR				szItemName[64];

	sDBRequest_SetItemPrice(SI32 itemunique, GMONEY price, REAL64 fprice, SI32 itemnum, const TCHAR* pszItemName)
	{
		ZeroMemory(this, sizeof(sDBRequest_SetItemPrice));
		packetHeader.usSize = sizeof( sDBRequest_SetItemPrice);
		packetHeader.usCmd = DBMSG_REQUEST_SETITEMPRICE;

		siItemUnique	= itemunique;
		siPrice			= price;
		fPrice			= fprice;
		siItemNum		= itemnum;

		if(pszItemName != NULL)		strncpy(szItemName, pszItemName, sizeof(szItemName));
	}
};

class sDBResponse_SetItemPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	sDBResponse_SetItemPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetItemPrice));
	}
};

//------------------------------------------
//KHY - 0530 명나라 상인 구매제한. - 1일 명나라 상인의 구매 액수를 저장한다. 
//------------------------------------------

// 현재까지 구매된 금액의 총합 - 기준 날짜( )까지의 총합만을 저장한다. 
class sDBRequest_SetForeignMerchantNowMoney
{
public:
	sPacketHeader		packetHeader;

	//SI16					siForeignMerchantKind;
	GMONEY				gPrice;

	sDBRequest_SetForeignMerchantNowMoney(GMONEY price)
	{
		ZeroMemory(this, sizeof(sDBRequest_SetForeignMerchantNowMoney));

		packetHeader.usSize = sizeof( sDBRequest_SetForeignMerchantNowMoney);
		packetHeader.usCmd = DBMSG_REQUEST_SET_FOREIGNMERCHANTNOWMONEY;
		
		//siForeignMerchantKind = ForeignMerchantKind;
		gPrice			= price;
	}
};

class sDBResponse_SetForeignMerchantNowMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	sDBResponse_SetForeignMerchantNowMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetForeignMerchantNowMoney));
	}
};

// 현재까지 구매된 금액의 총합 - 기준 날짜( )까지의 총합을 가져온다.
class sDBRequest_GetForeignMerchantNowMoney
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_GetForeignMerchantNowMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetForeignMerchantNowMoney));

		packetHeader.usSize = sizeof( sDBRequest_GetForeignMerchantNowMoney);
		packetHeader.usCmd = DBMSG_REQUEST_GET_FOREIGNMERCHANTNOWMONEY;
	}
};

class sDBResponse_GetForeignMerchantNowMoney
{
public:
	sPacketHeader		packetHeader;

	SI32					siResult;			// 결과값. (1: 성공 0:실패) 
	GMONEY				gPrice;

	sDBResponse_GetForeignMerchantNowMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetForeignMerchantNowMoney));
	}
};


// 구매할 수 있는 금액 저장. - 서버가 리부팅 되거나, GM 명령어 ( SetForeignMerchantBuyLimit ) 에 의해서 설정된다.
class sDBRequest_SetForeignMerchantBuyLimit
{
public:
	sPacketHeader		packetHeader	;

	GMONEY				gPrice			;
	float				fUpPercent		;
	float				fDownPercent	;

	sDBRequest_SetForeignMerchantBuyLimit( GMONEY price, float upPercent, float downPercent )
	{
		ZeroMemory(this, sizeof(sDBRequest_SetForeignMerchantBuyLimit));

		packetHeader.usSize = sizeof( sDBRequest_SetForeignMerchantBuyLimit);
		packetHeader.usCmd = DBMSG_REQUEST_SET_FOREIGNMERCHANTBUYLIMIT;

		gPrice			=	price;
		fUpPercent		=	upPercent;
		fDownPercent	=	downPercent;
	}
};

class sDBResponse_SetForeignMerchantBuyLimit
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	sDBResponse_SetForeignMerchantBuyLimit()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetForeignMerchantBuyLimit));
	}
};
// 명나라상인 구매제한 금액을 가지고 온다.
class sDBRequest_GetForeignMerchantBuyLimit
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_GetForeignMerchantBuyLimit()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetForeignMerchantBuyLimit));

		packetHeader.usSize = sizeof( sDBRequest_GetForeignMerchantBuyLimit);
		packetHeader.usCmd = DBMSG_REQUEST_GET_FOREIGNMERCHANTBUYLIMIT;
	}
};

class sDBResponse_GetForeignMerchantBuyLimit
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	GMONEY				gPrice;
	float				fUpPercent;
	float				fDownPercent;

	sDBResponse_GetForeignMerchantBuyLimit()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetForeignMerchantBuyLimit));
	}
};

// KHY - 0901 -  과금 아이템 자동구매 설정.
// 현재까지 구매된 금액의 총합 - 기준 날짜( )까지의 총합만을 저장한다. 
class sDBRequest_SetSystemBuyItemNowMoney
{
public:
	sPacketHeader		packetHeader;

	GMONEY				gPrice;

	sDBRequest_SetSystemBuyItemNowMoney(GMONEY price)
	{
		ZeroMemory(this, sizeof(sDBRequest_SetSystemBuyItemNowMoney));

		packetHeader.usSize = sizeof( sDBRequest_SetSystemBuyItemNowMoney);
		packetHeader.usCmd = DBMSG_REQUEST_SET_SYSTEMBUYITEMNOWMONEY;
		
		gPrice			= price;
	}
};

class sDBResponse_SetSystemBuyItemNowMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	GMONEY				gPrice;

	sDBResponse_SetSystemBuyItemNowMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetSystemBuyItemNowMoney));
	}
};

class sDBRequest_GetSystemBuyItemNowMoney
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_GetSystemBuyItemNowMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetSystemBuyItemNowMoney));

		packetHeader.usSize = sizeof( sDBRequest_GetSystemBuyItemNowMoney);
		packetHeader.usCmd = DBMSG_REQUEST_GET_SYSTEMBUYITEMNOWMONEY;
	}
};

class sDBResponse_GetSystemBuyItemNowMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	GMONEY				gPrice;

	sDBResponse_GetSystemBuyItemNowMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetSystemBuyItemNowMoney));
	}
};
//------------------------------------------
// 여러개의 아이템 가격을 요청
//------------------------------------------
class sDBRequest_ManyItemPrice
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_ManyItemPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyItemPrice));
		packetHeader.usSize = sizeof( sDBRequest_ManyItemPrice);
		packetHeader.usCmd = DBMSG_REQUEST_MANYITEMPRICE;
	}
};

#define MANYITEM_PRICE_NUMBER	50
class sDBResponse_ManyItemPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공, 0: 실패, -1: 마지막) 
	SI32				siItemUnique[MANYITEM_PRICE_NUMBER];			// 아이템의 유니크. 
	GMONEY				siCurrentItemPrice[MANYITEM_PRICE_NUMBER];	// 현재의 가격 (시가) 
	REAL64				fPrice[MANYITEM_PRICE_NUMBER];
	SI32				siItemNum[MANYITEM_PRICE_NUMBER];			// 집계된 개수. 

	sDBResponse_ManyItemPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyItemPrice));
	}
};

//------------------------------------------
// 객주 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_SetMarketFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Who
	SI32				siPersonID;

	// Where
	SI32				siVillageUnique;	// 객주가 위치한 곳의 VillageUnique

	// What
	cltFee				clFee;				// 설정해야 할 객주의 수수료 정보


	sDBRequest_SetMarketFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetMarketFee));
	}

	sDBRequest_SetMarketFee(SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetMarketFee );
		packetHeader.usCmd = DBMSG_REQUEST_SETMARKETFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_SetMarketFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// result 
	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;

	// where
	SI32				siVillageUnique;	// 객주가 위치한 마을의 VillageUnique

	// what
	cltFee				clFee;				// 객주의 거래 수수료 정보. 

	sDBResponse_SetMarketFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetMarketFee));
	}

};


//------------------------------------------
// 객주에서 물품을 거래할 것을 요청 
//------------------------------------------

#define MARKETTRADE_REASON_NORMAL		1
#define MARKETTRADE_REASON_EQUIPITEM	2
#define MARKETTRADE_REASON_CASHITEM		3

class sDBRequest_TradeMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;// 거래를 요청하는 마을의 VillageUnique. 	
	bool				bPileSwitch;	// 집적되는지 여부. 
	cltTradeOrder		clOrgOrder;		// 클라이언트로부터 온 주문. 
	SI32				siDstOrderIndex;// 반대 주문의 인덱스 (주문번호)

	GMONEY				siPrice;		// 거래 단가.
	SI16				siItemPos;		// 아이템을 위치할 공간. 

	SI16				siReason;

	sDBRequest_TradeMarket()
	{
		ZeroMemory(this, sizeof(sDBRequest_TradeMarket));
	}

	sDBRequest_TradeMarket(SI32 id, SI32 villageunique, bool pileswitch, cltTradeOrder* pclorgorder, SI32 dstorderindex, GMONEY price, SI16 itempos, SI16 reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeMarket );
		packetHeader.usCmd = DBMSG_REQUEST_TRADEMARKET;
		usCharID		= id;

		siVillageUnique	= villageunique;
		bPileSwitch		= pileswitch;
		clOrgOrder.Set(pclorgorder);
		siDstOrderIndex	= dstorderindex;
		siPrice			= price;
		siItemPos		= itempos;

		siReason		= reason;
	}
};


class sDBResponse_TradeMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 거래가 이루어진 Person
	GMONEY				siMoney;			// 현금 상황. 
	SI32				siVillageUnique;	// Request에서 입력된 VillageUnique
	cltTradeOrder		clDstOrder;			// 팔자 주문의 수정된 내용. 

	GMONEY				siTradePrice;		// 실제로 거래된 가격. (거래가 이루어지지 않았다면 0)
	SI16				siItemPos;			// 거래 결과 추가된 아이템의 위치
	cltItem				clItem;				// 거래 결과 추가된 아이템의 정보. 
	
	SI32				siSellerPersonID;	// 팔자의 PersonID
	GMONEY				siSellerBankMoney;	// 팔자의 은행 잔고. 
	SI32				siTradeItemNum;		// 거래딘 아이템의 개수. 

	//[추가 : 황진성 2007. 10. 25 순수하게 거래한 아이템 정보]
	cltItem				clTradeItem;		 
	SI16				siReason;

	sDBResponse_TradeMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_TradeMarket));
	}
};


//------------------------------------------
// 객주계좌 정보를 요청한다. 
//------------------------------------------
class sDBRequest_MarketAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상이 되는 Person

	sDBRequest_MarketAccount()
	{
		ZeroMemory(this, sizeof(sDBRequest_MarketAccount));
	}

	sDBRequest_MarketAccount(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_MarketAccount );
		packetHeader.usCmd = DBMSG_REQUEST_MARKETACCOUNT;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_MarketAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person

	cltMarketAccount	clMarketAccount;	// 객주 계좌 정보. 
	
	sDBResponse_MarketAccount()
	{
		ZeroMemory(this, sizeof(sDBResponse_MarketAccount));
	}
};


//------------------------------------------
// 객주 창고로부터 아이템을 꺼내올 것을 요청한다. 
//------------------------------------------
class sDBRequest_GetItemFromMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;


	SI32 siPersonID;		// 대상이 되는 Person
	
	SI16 siFromPos;			// 아이템의 계좌내 위치. 
	bool bPileSwitch;		// 집적되는지 여부. 
	cltItem clFromItem;		// 아이템의 계좌내 위치에 존재하는 아이템.
							//가져올 개수는 siItemNum에 업데이트 되어 있어야 한다. 

	SI16 siToPos;			// 아이템을 가져다 놓을 인벤내 위치. 


	sDBRequest_GetItemFromMarket()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetItemFromMarket));
	}

	sDBRequest_GetItemFromMarket(SI32 id, SI32 personid, SI32 frompos, bool pileswitch, cltItem* pclfromitem, SI32 topos)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetItemFromMarket );
		packetHeader.usCmd = DBMSG_REQUEST_GETITEMFROMMARKET;
		usCharID		= id;

		siPersonID		= personid;
		siFromPos		= frompos;
		bPileSwitch		= pileswitch;
		clFromItem.Set(pclfromitem);

		siToPos			= topos;
	}

	sDBRequest_GetItemFromMarket(SI32 id, cltGetItemFromMarketInfo* pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetItemFromMarket );
		packetHeader.usCmd = DBMSG_REQUEST_GETITEMFROMMARKET;
		usCharID		= id;

		siPersonID		= pclinfo->siPersonID;
		siFromPos		= pclinfo->siFromPos;
		bPileSwitch		= pclinfo->bPileSwitch;
		clFromItem.Set(&pclinfo->clFromItem);

		siToPos			= pclinfo->siToPos;
	}
};


class sDBResponse_GetItemFromMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		// 대상이 되는 Person
	
	SI16				siFromPos;			// 아이템의 계좌내 위치. 
	cltItem				clFromItem;		// 아이템의 계좌내 위치에 존재하는 아이템.
										

	SI16				siToPos;		// 아이템을 가져다 놓을 인벤내 위치. 
	cltItem				clToItem;		// 아이템을 가져다 놓을 인벤내 위치에 존재하는 아이템 정보. 
	
	cltTradeOrder		clOrder;		// 아이템 회수의 결과 변경된 주문 내용. 

	//[추가 : 황진성 2007. 10. 25 로그에 이용. 회수 아이템 정보]
	cltItem				clGetItem;		

	sDBResponse_GetItemFromMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetItemFromMarket));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_GetItemFromNewMarket
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에서 물품 찾기
//
//***************************************************************************************************
class sDBRequest_GetItemFromNewMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32 siServerUnique;	// 찾을려고 하는 사람이 속한 서버 유니크
	SI32 siPersonID;		// 대상이 되는 Person
	SI32 siOrderIndex;		// 찾을려고 하는 아이템의 주문번호

	bool bPileSwitch;		// 집적되는지 여부. 
	cltItem clFromItem;		// 아이템의 계좌내 위치에 존재하는 아이템.
	//가져올 개수는 siItemNum에 업데이트 되어 있어야 한다. 

	SI16 siToPos;			// 아이템을 가져다 놓을 인벤내 위치. 

	sDBRequest_GetItemFromNewMarket(SI32 id, SI32 serverunique, SI32 personid, SI32 orderindex, bool pileswitch, cltItem* pclfromitem, SI32 topos)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetItemFromNewMarket );
		packetHeader.usCmd = DBMSG_REQUEST_GETITEMFROMNEWARKET;
		usCharID		= id;

		siServerUnique = serverunique;
		siPersonID		= personid;
		siOrderIndex = orderindex;
		bPileSwitch		= pileswitch;
		clFromItem.Set(pclfromitem);

		siToPos			= topos;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_GetItemFromNewMarket
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [응답]통합 객주에서 물품 찾기
//
//***************************************************************************************************
class sDBResponse_GetItemFromNewMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;		// 결과값.
	SI32				siPersonID;		// 대상이 되는 Person

	SI16				siToPos;		// 아이템을 가져다 놓을 인벤내 위치. 
	cltItem				clToItem;		// 아이템을 가져다 놓을 인벤내 위치에 존재하는 아이템 정보. 

	//[추가 : 황진성 2007. 10. 25 로그에 이용. 회수 아이템 정보]
	cltItem				clGetItem;		

	sDBResponse_GetItemFromNewMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetItemFromNewMarket));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewMarketItemList
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에 올려진 아이템 리스트 요청
//
//***************************************************************************************************
class sDBRequest_NewMarketItemList
{
public:

	enum
	{
		TYPE_NORMAL_LIST = 0,
		TYPE_CHEAP_LIST
	};

	sPacketHeader		packetHeader;
	UI16				usCharID;

	UI08				uiType;
	SI32				siItemUnique;	// 리스트를 요청할 아이템의 유니크 
	SI16				siRequestPage;	// 요청한 페이지 정보

	sDBRequest_NewMarketItemList( SI32 id, UI08 type, SI32 itemunique, SI32 requestpage )
	{
		packetHeader.usSize = sizeof( sDBRequest_NewMarketItemList );
		packetHeader.usCmd = DBMSG_REQUEST_NEWMARKETITEMLIST;
		usCharID		= id;

		uiType			= type;
		siItemUnique	= itemunique;
		siRequestPage   = requestpage;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewMarketItemList
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [응답]통합 객주에 올려진 아이템 리스트 요청
//
//***************************************************************************************************
class sDBResponse_NewMarketItemList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	UI08				uiType;
	SI32				siResult;		// 결과값.
	SI32				siItemUnique;	// 요청한 아이템 유니크
	SI16				siMaxPage;		// 리스트의 총 페이지 수
	SI64				siCurrentPrice; // 현재 시세
	float				fCurrentPrice;  
	cltNewTradeOrder	clNewTradeOrder[MAX_NEWMARKET_ITEMLIST];

	sDBResponse_NewMarketItemList()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewMarketItemList));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewOrderMarket
//
// Last Modified 	: 2008/10/20
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에 아이템을 올린다.
//
//***************************************************************************************************
class sDBRequest_NewOrderMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;	// 주문을 행한 마을의 위치. 
	cltNewTradeOrder	clOrder;			// DB에 넣어야 할 주문의 내용. 

	SI16				siItemPos;			// Person에서 아이템이 위치하고 있는 곳. 
	bool				bPileSwitch;		// 옮길 아이템이 중첩되는지 여부. 

	SI64				siTax;				// 납부해야 할 세금. 

	UI08				uiOrderMode;		// 주문모드(0:사용자, 1:시스템)

	sDBRequest_NewOrderMarket(SI32 id, SI32 villageunique, cltNewTradeOrder* pclorder, SI32 itempos, bool bpileswitch, SI64 tax, UI08 uiordermode )
	{
		packetHeader.usSize = sizeof( sDBRequest_NewOrderMarket );
		packetHeader.usCmd = DBMSG_REQUEST_NEWORDERMARKET;
		usCharID		= id;

		siVillageUnique	= villageunique;
		clOrder.Set(pclorder);

		siItemPos		= itempos;
		bPileSwitch		= bpileswitch;
		siTax			= tax;

		uiOrderMode		=	uiordermode;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_NewOrderMarket
//
// Last Modified 	: 2008/10/20
// Created		 	: 김광명
//
// Function			: [응답]통합 객주에 아이템을 올린다.
//
//***************************************************************************************************
class sDBResponse_NewOrderMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	cltNewTradeOrder	clOrder;			// DB에 저장된 주문의 정보. 

	SI16				siItemFromPos;		// Person에서 아이템이 위치하던 곳. 
	cltItem				clFromItem;			// Person내 아이템의 최신 정보. 

	SI64				siTax;				// 납부해야 할 세금. 
	SI32				siVillageUnique;	// 주문을 행한 마을의 위치. 

	sDBResponse_NewOrderMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewOrderMarket));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewTradeMarket
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [요청]통합 객주로 구매요청을 한다.
//***************************************************************************************************
class sDBRequest_NewTradeMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siServerUnique;
	SI32				siVillageUnique;// 거래를 요청하는 마을의 VillageUnique.
	SI32				siBuyerPersonID;
	cltNewTradeOrder	clOrgOrder;		// 클라이언트로부터 온 주문. 

	SI64				siPrice;		// 거래 단가.
	SI16				siItemPos;		// 아이템을 위치할 공간. 

	bool				bPileSwitch;

	bool				m_bCreateSID;		// SID 생성 여부

	SYSTEMTIME			stRequestTime;		// 거래 요청한 시각 (현실시간)

	sDBRequest_NewTradeMarket()
	{
		ZeroMemory(this, sizeof(sDBRequest_NewTradeMarket));
	}

	sDBRequest_NewTradeMarket(SI32 id, SI32 serverunique, SI32 villageunique, SI32 buyerpersonid, cltNewTradeOrder* pclorgorder, SI64 price, SI16 itempos, bool PileSwitch, bool bCreateSID, SYSTEMTIME*	pRequestTime )
	{
		packetHeader.usSize = sizeof( sDBRequest_NewTradeMarket );
		packetHeader.usCmd = DBMSG_REQUEST_NEWTRADEMARKET;
		usCharID		= id;

		siServerUnique = serverunique;
		siVillageUnique	= villageunique;
		siBuyerPersonID = buyerpersonid;
		clOrgOrder.Set(pclorgorder);
		siPrice			= price;
		siItemPos		= itempos;
		bPileSwitch		= PileSwitch;
		m_bCreateSID	= bCreateSID;

		memcpy( &stRequestTime, pRequestTime, sizeof(stRequestTime) );
	}
};


//***************************************************************************************************
//
// Class Name 		: sDBResponse_NewTradeMarket
//
// Last Modified 	: 2008/10/23
// Created		 	: 김광명
//
// Function			: [응답]통합 객주로 구매요청을 한다.
//***************************************************************************************************
class sDBResponse_NewTradeMarket
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;				// 결과값.
	SI32				siBuyerPersonID;		// 거래가 이루어진 Person
	SI64				siMoney;				// 현금 상황. 
	SI32				siVillageUnique;		// Request에서 입력된 VillageUnique

	SI64				siTradePrice;			// 실제로 거래된 가격. (거래가 이루어지지 않았다면 0)
	SI16				siItemPos;				// 거래 결과 추가된 아이템의 위치
	cltItem				clItem;					// 거래 결과 추가된 아이템의 정보. 

	SI32				siSellerServerUnique;	// 팔자의 서버유니크
	SI32				siSellerPersonID;		// 팔자의 PersonID
	SI64				siSellerBankMoney;		// 팔자의 은행 잔고. 
	SI32				siTradeItemNum;			// 거래된 아이템의 개수. 

	float				fPrice;					// 아이템의 현시세
	bool				m_bSystem;

	//[추가 : 황진성 2007. 10. 25 순수하게 거래한 아이템 정보]
	cltItem				clTradeItem;		 

	sDBResponse_NewTradeMarket()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewTradeMarket));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewMarketAccount
//
// Last Modified 	: 2008/10/20
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에 올린 물품 정보를 요청한다.
//
//***************************************************************************************************
class sDBRequest_NewMarketAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI16				siServerUnique;
	SI32				siPersonID;		// 대상이 되는 Person

	sDBRequest_NewMarketAccount()
	{
		ZeroMemory(this, sizeof(sDBRequest_NewMarketAccount));
	}

	sDBRequest_NewMarketAccount(SI32 id, SI16 serverunique, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_NewMarketAccount );
		packetHeader.usCmd = DBMSG_REQUEST_NEWMARKETACCOUNT;
		usCharID		= id;

		siServerUnique = serverunique;
		siPersonID		= personid;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_NewMarketAccount
//
// Last Modified 	: 2008/10/20
// Created		 	: 김광명
//
// Function			: [응답]통합 객주에 올린 물품 정보를 요청한다.
//
//***************************************************************************************************
class sDBResponse_NewMarketAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 Person
	SI16				siServerUnique;		// 대상이 되는 Server

	cltNewMarketAccount	clMarketAccount;	// 객주 계좌 정보. 

	sDBResponse_NewMarketAccount()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewMarketAccount));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewMarketResult
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에서 팔린 정보를 각 서버로 업데이트 한다.
//
//***************************************************************************************************
class sDBRequest_NewMarketResult
{
public:
	sPacketHeader		packetHeader;

	SI16				siServerUnique;

	sDBRequest_NewMarketResult()
	{
		ZeroMemory(this, sizeof(sDBRequest_NewMarketResult));
	}

	sDBRequest_NewMarketResult(SI16 serverunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_NewMarketResult );
		packetHeader.usCmd = DBMSG_REQUEST_NEWMARKETRESULT;

		siServerUnique = serverunique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_NewMarketResult
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]통합 객주에서 팔린 정보를 각 서버로 업데이트 한다.
//
//***************************************************************************************************
const SI16 MAX_NEWMARKET_RESULTLIST = 100;
class sDBResponse_NewMarketResult
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI16				siServerUnique;		// 대상이 되는 Server

	SellerInfo			stSellerInfo[MAX_NEWMARKET_RESULTLIST];

	sDBResponse_NewMarketResult()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewMarketResult));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_SetNewMarketFee
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]대상전 수수료 설정
//
//***************************************************************************************************
class sDBRequest_SetNewMarketFee
{
public:
	sPacketHeader		packetHeader;
	SI32				siCharID;

	// Who
	SI32				siPersonID;

	// Where
	SI32				siVillageUnique;	// 객주가 위치한 곳의 VillageUnique

	// What
	cltFee				clFee;				// 설정해야 할 객주의 수수료 정보

	sDBRequest_SetNewMarketFee(SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetNewMarketFee );
		packetHeader.usCmd = DBMSG_REQUEST_SETNEWMARKETFEE;
		siCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_SetNewMarketFee
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]대상전 수수료 설정
//
//***************************************************************************************************
class sDBResponse_SetNewMarketFee
{
public:
	sPacketHeader		packetHeader;
	SI32				siCharID;

	// result 
	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;

	// where
	SI32				siVillageUnique;	// 객주가 위치한 마을의 VillageUnique

	// what
	cltFee				clFee;				// 객주의 거래 수수료 정보. 

	sDBResponse_SetNewMarketFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetNewMarketFee));
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NewMarket_IncreaseBankMoney
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]대상전에서 판매된 금액을 업데이트 시킨다.
//
//***************************************************************************************************
class sDBRequest_NewMarket_IncreaseBankMoney
{
public:
	sPacketHeader		packetHeader;
	SI32				siCharID;

	// Who
	SI32				siPersonID;
	SI64				siIncreaseMoney;

	sDBRequest_NewMarket_IncreaseBankMoney(SI32 id, SI32 personid, SI64 IncreaseMoney)
	{
		packetHeader.usSize = sizeof( sDBRequest_NewMarket_IncreaseBankMoney );
		packetHeader.usCmd = DBMSG_REQUEST_NEWMARKET_INCREASEBANKMONEY;
		siCharID		= id;

		siPersonID		= personid;
		siIncreaseMoney = IncreaseMoney;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_NewMarket_IncreaseBankMoney
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [응답]대상전에서 판매된 금액을 업데이트 시킨다.
//
//***************************************************************************************************
class sDBResponse_NewMarket_IncreaseBankMoney
{
public:
	sPacketHeader		packetHeader;
	SI32				siCharID;

	// result 
	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;

	//cltBankAccount		clBankAccount;			// 입금 된 금액. 	
	cltMoney			clTotalMoney;		// 입금 된 총금액. 
	// to bank info
	SI32				siVillageUnique;		
	cltMoney			clCurrentLeftMoney;		// 보유자금 
	cltMoney			clTotalInputMoney;		// 총수입액


	sDBResponse_NewMarket_IncreaseBankMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewMarket_IncreaseBankMoney));
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ManyItemNewPrice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]아이템 시세를 받아온다
//
//***************************************************************************************************
class sDBRequest_ManyItemNewPrice
{
public:

	enum
	{
		REQUEST_ALLITEM = 0,
		REQUEST_CHANGE_PRICE_ITEM
	};

	sPacketHeader		packetHeader;

	UI08 uiType;

	SYSTEMTIME			stRequestTime;	// 가격하락 정보를 요청한 시각 (현실시간)

	sDBRequest_ManyItemNewPrice( UI08 Type, SYSTEMTIME*	pRequestTime )
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyItemNewPrice));
		packetHeader.usSize = sizeof( sDBRequest_ManyItemNewPrice);
		packetHeader.usCmd = DBMSG_REQUEST_MANYITEMNEWPRICE;

		uiType = Type;

		memcpy( &stRequestTime, pRequestTime, sizeof(stRequestTime) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ManyItemNewPrice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [응답]아이템 시세를 받아온다
//
//***************************************************************************************************
class sDBResponse_ManyItemNewPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	SI16				siItemUnique[MANYITEM_PRICE_NUMBER];			// 아이템의 유니크. 
	SI64				siCurrentItemPrice[MANYITEM_PRICE_NUMBER];	// 현재의 가격 (시가) 
	float				fPrice[MANYITEM_PRICE_NUMBER];
	SI32				siItemNum[MANYITEM_PRICE_NUMBER];			// 집계된 개수. 

	sDBResponse_ManyItemNewPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyItemNewPrice));
	}
};

class sDBResponse_ManyItemNewPrice_Completed
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	sDBResponse_ManyItemNewPrice_Completed()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyItemNewPrice_Completed));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ManyItemNewPrice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청]아이템 시세를 설정한다
//
//***************************************************************************************************
class sDBRequest_SetItemNewPrice
{
public:
	sPacketHeader		packetHeader;

	SI16				siItemUnique;
	SI64				siPrice;
	float				fPrice;
	SI32				siItemNum;

	sDBRequest_SetItemNewPrice(SI16 itemunique, SI64 price, float fprice, SI32 itemnum)
	{
		ZeroMemory(this, sizeof(sDBRequest_SetItemNewPrice));
		packetHeader.usSize = sizeof( sDBRequest_SetItemNewPrice);
		packetHeader.usCmd = DBMSG_REQUEST_SETITEMNEWPRICE;

		siItemUnique	= itemunique;
		siPrice			= price;
		fPrice			= fprice;
		siItemNum		= itemnum;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ManyItemNewPrice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [응답]아이템 시세를 설정한다.
//
//***************************************************************************************************
class sDBResponse_SetItemNewPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	sDBResponse_SetItemNewPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetItemNewPrice));
	}
};

//***************************************************************************************************
//
// Class Name 		: NewMarketReducePriceNotice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [요청] 서버별로 대상전에서 가격이 하락한 정보를 요청한다.
//
//***************************************************************************************************
class sDBRequest_NewMarketReducePriceNotice
{
public:
	sPacketHeader		packetHeader;

	SI16				siServerUnique;

	SYSTEMTIME			stRequestTime;	// 가격하락 정보를 요청한 시각 (현실시간)

	sDBRequest_NewMarketReducePriceNotice( SI16 ServerUnique, SYSTEMTIME* pRequestTime )
	{
		packetHeader.usSize = sizeof( sDBRequest_NewMarketReducePriceNotice);
		packetHeader.usCmd = DBMSG_REQUEST_NEWMARKETREDUCEPRICENOTICE;

		siServerUnique = ServerUnique;
		memcpy( &stRequestTime, pRequestTime, sizeof(stRequestTime) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ManyItemNewPrice
//
// Last Modified 	: 2008/11/05
// Created		 	: 김광명
//
// Function			: [응답]아이템 시세를 설정한다.
//
//***************************************************************************************************
SI16 const MAX_REDUCEPRICE_USERLIST = 1000;
class sDBResponse_NewMarketReducePriceNotice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 

	struct	
	{		
		SI32			siPersonID;
		SI32			siItemUnique;
		SI32			siItemQty;
		cltMoney		clPrice;
	}stData[ MAX_REDUCEPRICE_USERLIST ];

	//SI32				siPersonID[MAX_REDUCEPRICE_USERLIST];

	sDBResponse_NewMarketReducePriceNotice()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewMarketReducePriceNotice));
	}
};

#endif
