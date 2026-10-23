#ifndef _DBMSGSTOCK_H
#define _DBMSGSTOCK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Stock\Stock.h"

#define MIN_AUTODIVIEND_MONEY	1000000

//-----------------------------------------------------------------
// Person의 주식 계좌를 요청한다.
//-----------------------------------------------------------------
class sDBRequest_PersonStockAccountGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 PersonID

	sDBRequest_PersonStockAccountGet(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonStockAccountGet);
		packetHeader.usCmd = DBMSG_REQUEST_PERSONSTOCKACCOUNTGET;

		siPersonID		= personid;
	}
};


class sDBResponse_PersonStockAccountGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				Result;
	SI32				siPersonID;			// 대상이 되는 PersonID
	cltStockAccount		clStockAccount;		// 주식 계좌 정보. 

	sDBResponse_PersonStockAccountGet()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonStockAccountGet));
	}
};


//------------------------------------------
// 주식 주문을 알려줄 것을 요청 
//------------------------------------------
#define GET_MANY_STOCK_ORDER_NUM	100
class sDBRequest_GetManyStockOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siStartIndex;	//  얻어와야 할 정보의 시작 인덱스 

	sDBRequest_GetManyStockOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetManyStockOrder));
	}

	sDBRequest_GetManyStockOrder(SI32 id, SI32 ordermode, SI32 startindex)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetManyStockOrder );
		packetHeader.usCmd = DBMSG_REQUEST_GETMANYSTOCKORDER;
		usCharID		= id;

		siOrderMode		= ordermode;
		siStartIndex	= startindex;
	}
};


class sDBResponse_GetManyStockOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siStartIndex;		// 얻어온 주문의 시작 인덱스.
	cltTradeOrder		clTradeOrder[GET_MANY_STOCK_ORDER_NUM];

	sDBResponse_GetManyStockOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetManyStockOrder));
	}
};

//------------------------------------------
// 주식을 거래할 것을 요청 
//------------------------------------------
class sDBRequest_TradeStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI32				siVillageUnique;// 거래를 요청하는 마을의 VillageUnique. 	
	cltTradeOrder		clOrgOrder;		// 클라이언트로부터 온 주문. 
	SI32				siDstOrderIndex;// 반대 주문의 인덱스 (주문번호)

	GMONEY				siPrice;		// 거래 단가.

	sDBRequest_TradeStock()
	{
		ZeroMemory(this, sizeof(sDBRequest_TradeStock));
	}

	sDBRequest_TradeStock(SI32 id, SI32 ordermode, SI32 villageunique, cltTradeOrder* pclorgorder, SI32 dstorderindex, GMONEY price)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeStock );
		packetHeader.usCmd = DBMSG_REQUEST_TRADESTOCK;
		usCharID		= id;

		siOrderMode		= ordermode;
		siVillageUnique	= villageunique;
		clOrgOrder.Set(pclorgorder);
		siDstOrderIndex	= dstorderindex;
		siPrice			= price;
	}
};


class sDBResponse_TradeStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI08				siOrderMode;		// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	SI08				siVillageUnique;	// Request에서 입력된 VillageUnique
	GMONEY				siTradePrice;		// 실제로 거래된 가격. (거래가 이루어지지 않았다면 0)

	// Org 주문자. 
	SI16				siTradeAmount;		// 거래된 주식수. 
	SI32				siLeftAmount;		// 남은 주식수. 
	GMONEY				siBankMoney;		// 전장 잔고. (거래후. ) 

	// Dst 주문자. 
	SI32				siDstLeftAmount;	// 남은 주식수. 
	GMONEY				siDstBankMoney;		// 전장 잔고. (거래후. ) 

	cltTradeOrder		clOrgOrder;			// 수정된 클라이언트의 주문. 
	cltTradeOrder		clDstOrder;			// 반대 주문의 수정된 내용. 


	sDBResponse_TradeStock()
	{
		ZeroMemory(this, sizeof(sDBResponse_TradeStock));
	}
};


//------------------------------------------
// 주식주문을 DB에 넣을 것을 요청 
//------------------------------------------
class sDBRequest_OrderStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siOrderMode;	// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	cltTradeOrder		clOrder;		// DB에 넣어야 할 주문의 내용. 

	sDBRequest_OrderStock()
	{
		ZeroMemory(this, sizeof(sDBRequest_OrderStock));
	}

	sDBRequest_OrderStock(SI32 id, SI32 ordermode, cltTradeOrder* pclorder)
	{
		packetHeader.usSize = sizeof( sDBRequest_OrderStock );
		packetHeader.usCmd = DBMSG_REQUEST_ORDERSTOCK;
		usCharID		= id;

		siOrderMode		= ordermode;
		clOrder.Set(pclorder);
	}
};


class sDBResponse_OrderStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siOrderMode;		// 사자 주문인가. 팔자 주문인가..(ORDERMODE_SELL, ORDERMODE_BUY) 
	cltTradeOrder		clOrder;			// DB에 저장된 주문의 정보. 

	sDBResponse_OrderStock()
	{
		ZeroMemory(this, sizeof(sDBResponse_OrderStock));
	}
};



//------------------------------------------
// 주식주문을 취소할 것을 요청 
//------------------------------------------
class sDBRequest_CancelStockOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;


	SI32				siPersonID;		// 주문을 취소할 PersonID

	sDBRequest_CancelStockOrder()
	{
		ZeroMemory(this, sizeof(sDBRequest_CancelStockOrder));
	}

	sDBRequest_CancelStockOrder(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_CancelStockOrder );
		packetHeader.usCmd = DBMSG_REQUEST_CANCELSTOCKORDER;
		usCharID		= id;

		siPersonID		= personid;
	}
};


class sDBResponse_CancelStockOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	SI32				siPersonID;			// 주문 취소를 요청했던 PersonID

	sDBResponse_CancelStockOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_CancelStockOrder));
	}
};

//------------------------------------------
// 여러개의 주식 가격을 요청
//------------------------------------------
class sDBRequest_ManyStockPrice
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_ManyStockPrice()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyStockPrice));
		packetHeader.usSize = sizeof( sDBRequest_ManyStockPrice );
		packetHeader.usCmd = DBMSG_REQUEST_MANYSTOCKPRICE;
	}
};


class sDBResponse_ManyStockPrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값. (1: 성공 0:실패) 
	GMONEY				siCurrentStockPrice[MAX_VILLAGE_NUMBER];	// 현재의 주가. (시가) 
	GMONEY				siAverageStockPrice[MAX_VILLAGE_NUMBER];	// 평균 주가

	sDBResponse_ManyStockPrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyStockPrice));
	}
};



//------------------------------------------
// 여각 거래 수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_SetStockFee
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


	sDBRequest_SetStockFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetStockFee));
	}

	sDBRequest_SetStockFee(SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetStockFee );
		packetHeader.usCmd = DBMSG_REQUEST_SETSTOCKFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clFee.Set(pclfee);
	}
};


class sDBResponse_SetStockFee
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

	sDBResponse_SetStockFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetStockFee));
	}

};



//---------------------------------------
// 현재 증자 진행 중인 마을 정보를 얻어온다
//---------------------------------------
class sDBResponse_GetStockBidVillageList
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	UI16					usCount;											// 증자중인 마을 개수  
	cltStockBidVillageInfo	clStockBidVillageInfo[ MAX_VILLAGE_NUMBER ];		// 증자중인 마을 정보

	sDBResponse_GetStockBidVillageList()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetStockBidVillageList ) );
	}

};

//---------------------------------------
// 입찰 중인 마을의 입찰자 리스트를 요청한다 
//---------------------------------------
class sDBRequest_GetStockBidderList
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siVillageUnique;

	sDBRequest_GetStockBidderList( SI32 villageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetStockBidderList );
		packetHeader.usCmd = DBMSG_REQUEST_GETSTOCKBIDDERLIST;

		siVillageUnique = villageUnique;
	}
};


//---------------------------------------
// 입찰 중인 마을의 입찰자 리스트를 반환한다
//---------------------------------------
class sDBResponse_GetStockBidderList
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siVillageUnique;

	UI16					usCount;
	cltStockBidder			clStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];

	sDBResponse_GetStockBidderList()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetStockBidderList ) );
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_BidStock
//
// Last Modified 	: 2006 / 01 / 27
// Created		 	: 김광명
//
// Function			: [요청] 대행수가 증자 신청한다.
//
//***************************************************************************************************
class sDBRequest_BidStock
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siPersonID;			
	SI32					siVillageUnique;		// 주식 종목(마을)
	GMONEY					siStockMoney;			// 증자 주식의 가격 1주

	cltDate					clDate;					// 입찰일	

	sDBRequest_BidStock( UI16 id, SI32 personid, SI32 villageUnique, SI64 StockMoney, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_BidStock );
		packetHeader.usCmd = DBMSG_REQUEST_BIDSTOCK;

		usCharID = id;

		siPersonID = personid;
		siVillageUnique = villageUnique;
		siStockMoney = StockMoney;

		clDate.Set( pclDate );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_BidStock
//
// Last Modified 	: 2006 / 01 / 27
// Created		 	: 김광명
//
// Function			: [응답] 대행수가 증자 신청한다.
//
//***************************************************************************************************
class sDBResponse_BidStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	cltStockBidVillageInfo	clStockBidVillageInfo;

	// PCK : 로그용 패킷 추가
	SI32				siVillageUnique;

	sDBResponse_BidStock()
	{
		ZeroMemory( this, sizeof( sDBResponse_BidStock ) );
	}
};
/*
//---------------------------------------
// 증자 주식에 입찰 신청한다
//---------------------------------------
class sDBRequest_BidStock
{
public:
sPacketHeader			packetHeader;
UI16					usCharID;

SI32					siPersonID;			

SI32					siTradeVillageUnique;	// 거래 마을 
SI64					siFee;					// 수수료 

SI32					siVillageUnique;		// 주식 종목(마을)
SI32					siAmount;				// 주식 수량 
cltMoney				clPrice;				// 주식 가격 

cltDate					clDate;					// 입찰일	

sDBRequest_BidStock( UI16 id, SI32 personid, SI32 tradeVillage, SI64 fee, SI32 villageUnique, SI32 amount, cltMoney *pclMoney, cltDate *pclDate )
{
packetHeader.usSize = sizeof( sDBRequest_BidStock );
packetHeader.usCmd = DBMSG_REQUEST_BIDSTOCK;

usCharID = id;

siPersonID = personid;

siTradeVillageUnique = tradeVillage;
siFee = fee;

siVillageUnique = villageUnique;
siAmount = amount;
clPrice.Set( pclMoney );
clDate.Set( pclDate );
}
};


//---------------------------------------
// 증자 주식에 입찰 신청에 대한 응답 
//---------------------------------------
class sDBResponse_BidStock
{
public:
sPacketHeader		packetHeader;
UI16				usCharID;

SI32				siResult;

SI32				siTradeVillageUnique;
cltMoney			clTradeStockStrMoney;

SI32				siVillageUnique;
cltStockBidder		clStockBidder;

cltMoney			clPersonBankMoney;

cltDate				clDate;						// 입찰일 

sDBResponse_BidStock()
{
ZeroMemory( this, sizeof( sDBResponse_BidStock ) );
}

};
*/

//---------------------------------------
// 증자 주식에 입찰 신청 취소 요청
//---------------------------------------
class sDBRequest_CancelBidStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;

	sDBRequest_CancelBidStock( SI32 villageUnique, SI32 personid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_CANCELBIDSTOCK;
		packetHeader.usSize = sizeof( sDBRequest_CancelBidStock );

		siVillageUnique = villageUnique;
		siPersonID = personid;
	}
};



//---------------------------------------
// 증자 주식에 입찰 신청 취소에 대한 응답 
//---------------------------------------
class sDBResponse_CancelBidStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	sDBResponse_CancelBidStock()
	{
		ZeroMemory( this, sizeof( sDBResponse_CancelBidStock ) );
	}

};



//---------------------------------------
// 증자 중인 마을을 설정한다
//---------------------------------------
class sDBRequest_SetStockBidVillage
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;								// 마을
	SI32				siStockAmount;									// 증자 주식수
	cltDate				clDate;											// 증자일
	GMONEY				siBeginningPrice;								// 시작가 

	sDBRequest_SetStockBidVillage( SI32 villageUnique, SI32 stockAmount, cltDate *pclDate, GMONEY beginningPrice )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetStockBidVillage );
		packetHeader.usCmd = DBMSG_REQUEST_SETSTOCKBIDVILLAGE;

		siVillageUnique = villageUnique;
		siStockAmount = stockAmount;
		clDate.Set( pclDate );
		siBeginningPrice = beginningPrice;
	}
};


//---------------------------------------
// 증자 중인 마을을 설정한다
//---------------------------------------
class sDBResponse_SetStockBidVillage
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;								// 마을
	SI32				siStockAmount;									// 증자 주식수
	cltDate				clDate;											// 증자일
	GMONEY				siBeginningPrice;								// 시작가

	sDBResponse_SetStockBidVillage()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetStockBidVillage ) );
	}

};

//---------------------------------------
// 경매를 종료한다 
//---------------------------------------
class sDBRequest_CompleteStockBidVillage
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// 마을

	sDBRequest_CompleteStockBidVillage( SI32 villageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_CompleteStockBidVillage );
		packetHeader.usCmd = DBMSG_REQUEST_COMPLETESTOCKBIDVILLAGE;

		siVillageUnique = villageUnique;
	}

};

//---------------------------------------
// 경매 종료 결과 
//---------------------------------------
class sDBResponse_CompleteStockBidVillage
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siResult;											// 경매 성공 여부(1:성공 0:실패)

	SI32					siVillageUnique;
	cltMoney				clTotalMoney;										// 증자 금액
	cltMoney				clCityhallMoney;									// 상단 자금 
	cltMoney				clTaxMoney;											// 증자로 나간 세금액

	cltResultStockBidder	clResultStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];

	sDBResponse_CompleteStockBidVillage()
	{
		ZeroMemory( this, sizeof( sDBResponse_CompleteStockBidVillage ) );
	}

};


//--------------------------------------
// 증자시 마을 주식의 입찰 시작가 설정 요청 
//--------------------------------------
class sDBRequest_SetBeginningPrice
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siVillageUnique;
	GMONEY					siBeginningPrice;

	sDBRequest_SetBeginningPrice( SI32 villageUnique, GMONEY beginningPrice )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetBeginningPrice );
		packetHeader.usCmd = DBMSG_REQUEST_SETBEGINNINGPRICE;

		siVillageUnique = villageUnique;
		siBeginningPrice = beginningPrice;
	}


};


//--------------------------------------
// 주식 공모에서 유저가 주식 구입 요청
//--------------------------------------
class sDBRequest_StockDistributionOrder
{
public:
	sPacketHeader			packetHeader;

	SI32					siVillageUnique;

	SI32					siPersonID;
	SI32					siCharID;

	SI32					siStockDistributionNum;
	GMONEY					siStockPrice;

	bool					bVillageWarStockDistribution;

	sDBRequest_StockDistributionOrder( SI32 VillageUnique, SI32 PersonID, SI32 CharID, SI32 StockDistributionNum, GMONEY StockPrice, bool VillageWarStockDistribution )
	{
		packetHeader.usSize = sizeof( sDBRequest_StockDistributionOrder );
		packetHeader.usCmd = DBMSG_REQUEST_STOCKDISTRIBUTIONORDER;

		siVillageUnique = VillageUnique;
		siPersonID = PersonID;
		siCharID = CharID;

		siStockDistributionNum = StockDistributionNum;
		siStockPrice = StockPrice;

		bVillageWarStockDistribution = VillageWarStockDistribution;
	}
};

class sDBResponse_StockDistributionOrder
{
public:
	sPacketHeader			packetHeader;

	SI32					siResult;

	SI32					siVillageUnique;

	SI32					siPersonID;
	SI32					siCharID;

	SI32					siStockDistributionNum;
	GMONEY					siStockPrice;

	cltMoney				clPersonMoney;

	bool					bVillageWarStockDistribution;

	sDBResponse_StockDistributionOrder()
	{
		ZeroMemory( this, sizeof( sDBResponse_StockDistributionOrder ) );
	}
};


class sDBRequest_MaxStockDistributionNum
{
public:
	sPacketHeader			packetHeader;

	SI32					siVillageUnique;

	SI32					siMaxStockDistributionNum;

	SI32					siStockDistributionNum;
	GMONEY					siStockPrice;

	bool					bVillageWarStockDistribution;

	UI08					uiRememberVillageUnique;

	SI32					siVillageWarWinVillageUnique;

	sDBRequest_MaxStockDistributionNum( SI32 VillageUnique, SI32 MaxStockDistributionNum, SI32 StockDistributionNum, GMONEY StockPrice, bool VillageWarStockDistribution, UI08 RememberVillageUnique, SI32 VillageWarWinVillageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_MaxStockDistributionNum );
		packetHeader.usCmd = DBMSG_REQUEST_MAXSTOCKDISTRIBUTIONNUM;

		siVillageUnique = VillageUnique;

		siMaxStockDistributionNum = MaxStockDistributionNum;

		siStockDistributionNum = StockDistributionNum;

		siStockPrice = StockPrice;

		bVillageWarStockDistribution = VillageWarStockDistribution;

		uiRememberVillageUnique = RememberVillageUnique;

		siVillageWarWinVillageUnique = VillageWarWinVillageUnique;
	}
};

class sDBResponse_MaxStockDistributionNum
{
public:
	sPacketHeader			packetHeader;

	SI32					siResult;

	SI32					siVillageUnique;

	SI32					siMaxStockDistributionNum;

	SI32					siStockDistributionNum;
	GMONEY					siStockPrice;

	bool					bVillageWarStockDistribution;

	UI08					uiRememberVillageUnique;

	SI32					siVillageWarWinVillageUnique;

	sDBResponse_MaxStockDistributionNum()
	{
		ZeroMemory( this, sizeof( sDBResponse_MaxStockDistributionNum ) );
	}
};

//#if defined(_CITYHALL_BANKRUPTCY)
//---------------------------------------
// [요청] 마을이 파산하여 주식 경매 입찰을 모두 돌려준다.
//---------------------------------------
class sDBRequest_ReturnStockBidVillage
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// 마을

	sDBRequest_ReturnStockBidVillage( SI32 villageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_ReturnStockBidVillage );
		packetHeader.usCmd = DBMSG_REQUEST_RETURNSTOCKBIDVILLAGE;

		siVillageUnique = villageUnique;
	}

};

//---------------------------------------
// [응답] 마을이 파산하여 주식 경매 입찰을 모두 돌려준다.
//---------------------------------------
class sDBResponse_ReturnStockBidVillage
{
public:
	sPacketHeader			packetHeader;
	UI16					usCharID;

	SI32					siResult;											// 경매 입찰 보상 성공 여부(1:성공 0:실패)

	SI32					siVillageUnique;

	cltResultStockBidder	clResultStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];

	sDBResponse_ReturnStockBidVillage()
	{
		ZeroMemory( this, sizeof( sDBResponse_ReturnStockBidVillage ) );
	}

};
//#endif

//***************************************************************************************************
//
// Class Name 		: sDBRequest_BuyBidStock
//
// Last Modified 	: 2006 / 02 / 07
// Created		 	: 김광명
//
// Function			: [요청] 증자주식 사기
//
//***************************************************************************************************
class sDBRequest_BuyBidStock
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		 // 살려고 요청한 사람
	SI08				siBuyMode;		 // 0 : 주주 1 : 일반
	SI32				siVillageUnique; // 증자 주식을 살려고 하는 마을 유니크 	
	SI32				siAmount;		 // 주문 수량 (주문번호)
	GMONEY				siPrice;		 // 1주당 금액
	GMONEY				siTotalPrice;	 // 총 거래 금액
	bool				bBuySwitch;		 // 주주가 샀을때는 true 중복 사기 방지
	GMONEY				siFee;			 // 증자주식의 수수료

	sDBRequest_BuyBidStock()
	{
		ZeroMemory(this, sizeof(sDBRequest_BuyBidStock));
	}

	sDBRequest_BuyBidStock(SI32 ID, SI32 PersonID, SI08 BuyMode, SI32 VillageUnique, SI32 Amount, GMONEY Price, GMONEY TotalPrice, bool BuySwitch, GMONEY Fee )
	{
		packetHeader.usSize = sizeof( sDBRequest_BuyBidStock );
		packetHeader.usCmd = DBMSG_REQUEST_BUYBIDSTOCK;

		usCharID		= ID;

		siPersonID		= PersonID;
		siBuyMode		= BuyMode;
		siVillageUnique	= VillageUnique;
		siAmount		= Amount;
		siPrice		    = Price;
		siTotalPrice	= TotalPrice;
		bBuySwitch	    = BuySwitch;
		siFee			= Fee;
	}
};

class sDBResponse_BuyBidStock
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;

	SI08				siBuyMode;		 // 0 : 주주 1 : 일반
	SI32				siPersonID;		 // 살려고 요청한 사람
	SI32				siResult;
	SI32				siAmount;		 // 주문 수량 (주문번호)
	GMONEY				siTotalPrice;	 // 총 거래 금액
	GMONEY				siFee;
	GMONEY				siBuyStockLeftMoney; // 주식을 사고 난 후에 내 소지금

	cltStockBidVillageInfo clStockBidVillageInfo;

	cltMoney				clCityhallMoney;									// 상단 자금 

	sDBResponse_BuyBidStock()
	{
		ZeroMemory( this, sizeof( sDBResponse_BuyBidStock ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_BuyBidStock
//
// Last Modified 	: 2006 / 02 / 07
// Created		 	: 김광명
//
// Function			: [요청] 증자주식 사기
//
//***************************************************************************************************
class sDBRequest_MyStockInfo
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;		 // 살려고 요청한 사람


	sDBRequest_MyStockInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_MyStockInfo));
	}

	sDBRequest_MyStockInfo(UI16 ID, SI32 PersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_MyStockInfo );
		packetHeader.usCmd = DBMSG_REQUEST_MYSTOCKINFO;

		usCharID		= ID;
		siPersonID		= PersonID;
	}
};

class sDBResponse_MyStockInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	UI16				usCharID;

	SI32				siPersonID;		 // 살려고 요청한 사람

	cltStockAccount		clInfo;			// 업데이트 될 주식 정보

	sDBResponse_MyStockInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_MyStockInfo ) );
	}
};



//-------------------------------------------------------------------------------------------------------
// 주식 공모
//-------------------------------------------------------------------------------------------------------

// 주식 공모 마을 정보를 획득한다
class sDBRequest_GetStockOfferingInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;

	sDBRequest_GetStockOfferingInfo( SI32 villageunique )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetStockOfferingInfo );
		packetHeader.usCmd = DBMSG_REQUEST_GETSTOCKOFFERINGINFO;

		siVillageUnique		= villageunique;
	}
};


class sDBResponse_GetStockOfferingInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	SI32				siStatus;				// 상태 ( 0: 공모 아님, 1: 공모 청약 중,  2: 공모 청약 마감)
	SI32				siTotalOffering;		// 총 공모 주식 수량
	SI32				siStockAmount;			// 1인당 배당 주식 수
	GMONEY				siPrice;				// 1인당 배당 주식 가격


	sDBResponse_GetStockOfferingInfo()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetStockOfferingInfo) );
	}

};


// 주식 공모 마을 설정 (공모 청약 시작)
class sDBRequest_SetStockOffering
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	SI32				siTotalOffering;		// 총 공모 주식 수량
	SI32				siStockAmount;			// 1인당 배당 주식 수
	GMONEY				siPrice;				// 1인당 배당 주식 가격

	sDBRequest_SetStockOffering( SI32 villageunique, SI32 totaloffering, SI32 stockamount, GMONEY price  )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetStockOffering );
		packetHeader.usCmd = DBMSG_REQUEST_SETSTOCKOFFERING;

		siVillageUnique	= villageunique;
		siTotalOffering = totaloffering;
		siStockAmount = stockamount;
		siPrice = price;
	}

};

class sDBResponse_SetStockOffering
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	SI32				siTotalOffering;		// 총 공모 주식 수량
	SI32				siStockAmount;			// 1인당 배당 주식 수
	GMONEY				siPrice;				// 1인당 배당 주식 가격

	sDBResponse_SetStockOffering()
	{
		ZeroMemory( this, sizeof(sDBResponse_SetStockOffering) );
	}

};


// 주식 공모 청약 종료
class sDBRequest_EndStockOffering
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;

	sDBRequest_EndStockOffering( SI32 villageunique )
	{
		packetHeader.usSize = sizeof( sDBRequest_EndStockOffering );
		packetHeader.usCmd = DBMSG_REQUEST_ENDSTOCKOFFERING;

		siVillageUnique		= villageunique;
	}

};

class sDBResponse_EndStockOffering
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;

	sDBResponse_EndStockOffering()
	{
		ZeroMemory( this, sizeof(sDBResponse_EndStockOffering) );
	}

};

// 주식 공모 청약 주문
class sDBRequest_StockOfferingOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	TCHAR				szIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];

	sDBRequest_StockOfferingOrder( UI16 charid, SI32 villageunique, SI32 personid, SI32 stockamount, GMONEY money, TCHAR* pszIP, TCHAR* pszPrivateIP, TCHAR* pszMacAddress )
	{
		packetHeader.usSize = sizeof( sDBRequest_StockOfferingOrder );
		packetHeader.usCmd = DBMSG_REQUEST_STOCKOFFERINGORDER;

		usCharID			= charid;
		siVillageUnique		= villageunique;
		siPersonID			= personid;
		siStockAmount		= stockamount;
		siMoney				= money;

		szIP[0]				= '\0';
		szPrivateIP[0]		= '\0';
		szMacAddress[0]		= '\0';

		if(pszIP)			MStrCpy(szIP, pszIP, sizeof(szIP));
		if(pszPrivateIP)	MStrCpy(szPrivateIP, pszPrivateIP, sizeof(szPrivateIP));
		if(pszMacAddress)	MStrCpy(szMacAddress, pszMacAddress, sizeof(szMacAddress));
	}
};

class sDBResponse_StockOfferingOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	GMONEY				siPersonMoney;

	sDBResponse_StockOfferingOrder()
	{
		ZeroMemory( this, sizeof(sDBResponse_StockOfferingOrder) );
	}
};


// 주식 청약자 리스트
class sDBRequest_GetStockOfferingOrderList
{
public:
	sPacketHeader		packetHeader;
	SI32				siVillageUnique;

	sDBRequest_GetStockOfferingOrderList( SI32 villageunique )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetStockOfferingOrderList );
		packetHeader.usCmd = DBMSG_REQUEST_GETSTOCKOFFERINGORDERLIST;

		siVillageUnique = villageunique;
	}


};

class sDBResponse_GetStockOfferingOrderList
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	TCHAR				szIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];

	sDBResponse_GetStockOfferingOrderList()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetStockOfferingOrderList) );
	}

};


// 주식 청약 주문 실행
class sDBRequest_ExecuteStockOfferingOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siType;				// 1: 주식 배분 당첨,  2: 주식 배분 당첨 실패

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	TCHAR				szIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];

	sDBRequest_ExecuteStockOfferingOrder(UI16 id, SI32 type, SI32 villageunique, SI32 personid, SI32 stockamount, GMONEY money, TCHAR* pszIP, TCHAR* pszPrivateIP, TCHAR* pszMacAddress )
	{
		packetHeader.usSize = sizeof( sDBRequest_ExecuteStockOfferingOrder );
		packetHeader.usCmd = DBMSG_REQUEST_EXECUTESTOCKOFFERINGORDER;

		usCharID = id;
		
		siType			= type;
		siVillageUnique	= villageunique;
		siPersonID		= personid;
		siStockAmount	= stockamount;
		siMoney			= money;

		szIP[0]				= '\0';
		szPrivateIP[0]		= '\0';
		szMacAddress[0]		= '\0';

		if(pszIP)			MStrCpy(szIP, pszIP, sizeof(szIP));
		if(pszPrivateIP)	MStrCpy(szPrivateIP, pszPrivateIP, sizeof(szPrivateIP));
		if(pszMacAddress)	MStrCpy(szMacAddress, pszMacAddress, sizeof(szMacAddress));
	}

};

class sDBResponse_ExecuteStockOfferingOrder
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siType;				// 1: 주식 배분 당첨,  2: 주식 배분 당첨 실패

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	TCHAR				szIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];

	sDBResponse_ExecuteStockOfferingOrder()
	{
		ZeroMemory( this, sizeof(sDBResponse_ExecuteStockOfferingOrder) );
	}

};


class sDBRequest_ResetStockOfferingOrderList
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;

	sDBRequest_ResetStockOfferingOrderList( SI32 villageunique )
	{
		packetHeader.usSize = sizeof( sDBRequest_ResetStockOfferingOrderList );
		packetHeader.usCmd = DBMSG_REQUEST_RESETSTOCKOFFERINGORDERLIST;

		siVillageUnique = villageunique;


	}
};

class sDBResponse_ResetStockOfferingOrderList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siVillageUnique;

	sDBResponse_ResetStockOfferingOrderList()
	{
		ZeroMemory( this, sizeof( sDBResponse_ResetStockOfferingOrderList ) );
	}
};


//***************************************************************************************************
//
// Class Name 		: sDBRequest_ExchangeFromStock
//
// Last Modified 	: 2007 / 11 / 01 
// Created		 	: 김광명
//
// Function			: [요청] 주식 1주당 현실 돈 1위안으로 바꿔준다.
//
//*************************************************************************************************** 
class sDBRequest_ExchangeFromStock
{
public:
	sPacketHeader	packetHeader;

	/// 주식을 돈으로 바꾸려고 하는 사람
	SI32			siCharID;
	/// 주식을 돈으로 바꾸려고 하는 사람 PersonID
	SI32			siPersonID;
	/// 주식을 바꾸려고 하는 사람 2차비밀번호
	TCHAR			szSecondPassWord[MAX_PLAYER_NAME];
	/// 국가 주식 유니크
	SI32			siStockUnique;
	/// 팔려고 하는 수량
	SI32			siSellAmount;
	/// 돈을 받을 은행
	SI32			siBankType;
	/// 돈을 받을 계좌
	TCHAR			szBankAccount[MAX_PLAYER_NAME];

	sDBRequest_ExchangeFromStock( SI32 CharID, SI32 PersonID, TCHAR* SecondPassWord,
								  SI32 StockUnique, SI32 SellAmount, SI32 BankType, TCHAR* BankAccount )
	{
		packetHeader.usSize = sizeof( sDBRequest_ExchangeFromStock );
		packetHeader.usCmd = DBMSG_REQUEST_EXCHANGEFROMSTOCK;

		siCharID = CharID;
		siPersonID = PersonID;
		StringCchCopy( szSecondPassWord, MAX_PLAYER_NAME, SecondPassWord );
		siStockUnique = StockUnique;
		siSellAmount = SellAmount;
		siBankType = BankType;
		StringCchCopy( szBankAccount, MAX_PLAYER_NAME, BankAccount );

	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ExchangeFromStock
//
// Last Modified 	: 2007 / 11 / 01 
// Created		 	: 김광명
//
// Function			: [요청] 주식 1주당 현실 돈 1위안으로 바꿔준다.
//
//*************************************************************************************************** 
class sDBResponse_ExchangeFromStock
{
public:
	sPacketHeader		packetHeader;

	/// 결과값
	SI32				siResult;
	/// 요청한 사람
	SI32				siCharID;
	/// 판매한 주식 유니크
	SI32				siStockUnique;
	/// 판매한 수량
	SI32				siSellAmount;
    /// 판매하고 남은 수량(유저)
	SI32				siLeftStockAmount;
	/// 판매하고 남은 국가의 전체 주식수
	SI32				siTotalStockAmount;

	sDBResponse_ExchangeFromStock()
	{
		ZeroMemory( this, sizeof( sDBResponse_ExchangeFromStock ) );
	}
};

// [영훈] 초보자 마을 : 주식을 주라고 요청한다
class sDBRequest_GiveStock
{
public:
	sPacketHeader	packetHeader;

	SI32			siCharID;
	SI32			siPersonID;

	SI32			siVillageUnique;	// 어느 마을의 주식인가
	SI32			siStockAmount;		// 얼마를 줘야 하는가

	sDBRequest_GiveStock( SI32 para_siCharID, SI32 para_siPersonID, SI32 para_siVillageUnique, SI32 para_siStockAmount )
	{
		packetHeader.usSize	= sizeof( sDBRequest_GiveStock );
		packetHeader.usCmd	= DBMSG_REQUEST_GIVESTOCK;

		siCharID			= para_siCharID;
		siPersonID			= para_siPersonID;

		siVillageUnique		= para_siVillageUnique;	
		siStockAmount		= para_siStockAmount;
	}
};

// [영훈] 초보자 마을 : 주식을 주라고 요청한 결과
class sDBResponse_GiveStock
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;			// 결과값

	SI32			siCharID;
	SI32			siPersonID;

	SI32			siVillageUnique;	// 어느마을 주식인가
	SI32			siStockAmount;		// 얼마를 줬는가

	
	sDBResponse_GiveStock()
	{
		ZeroMemory( this, sizeof( sDBResponse_GiveStock ) );
	}

};

#endif