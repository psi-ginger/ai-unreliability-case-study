#ifndef _MSGTYPESTOCK_H
#define _MSGTYPESTOCK_H

#include "../common/StockOfferingManager/StockOfferingManager.h"
#include "../CommonLogic/TradeOrder/TradeOrder.h"


//-----------------------------------------
// 여각에 올려 놓은 주식 주문 정보중 가장 적절한 것을 요청하고 돌려준다. 
//------------------------------------------
class cltGameMsgRequest_StockOrderInfo
{
public:
	SI16 siVillageUnique;	// 대상이 되는 마을의 유니크. 
	//SI16 siOrderMode;		// 사자 주문을 찾는가. 팔자 주문을 찾는가 ?

	cltGameMsgRequest_StockOrderInfo(SI16 villageunique)
	{
		siVillageUnique = villageunique;
		//siOrderMode		= ordermode;
	}
};


class cltGameMsgResponse_StockOrderInfo
{
public:
	// SI16 siOrderMode;		// 사자 주문을 찾는가. 팔자 주문을 찾는가 ?

#ifdef _SAFE_MEMORY
	NSafeTArray<cltTradeOrder, MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM>		clSellTradeOrder;
	NSafeTArray<cltTradeOrder, MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM>		clBuyTradeOrder;
#else
	cltTradeOrder		clSellTradeOrder[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];	// 팔자 주문 내용. 
	cltTradeOrder		clBuyTradeOrder[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];	// 팔자 주문 내용. 
#endif
	
	GMONEY				siAverageSell;
	GMONEY				siAverageBuy;

	cltGameMsgResponse_StockOrderInfo(cltTradeOrder* pclSellOrder , cltTradeOrder * pclBuyOrder , GMONEY	AverageSell,GMONEY AverageBuy )
	{
		//siOrderMode	= ordermode;
#ifdef _SAFE_MEMORY
		clSellTradeOrder.ZeroMem();
		clBuyTradeOrder.ZeroMem();

		clSellTradeOrder.MemCpy( pclSellOrder );
		clBuyTradeOrder.MemCpy( pclBuyOrder );
#else
		memcpy( clSellTradeOrder,pclSellOrder,sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
		memcpy( clBuyTradeOrder,pclBuyOrder,sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
#endif
		
		siAverageSell = AverageSell;
		siAverageBuy = AverageBuy;

	}
};


// 현재 증자 중인 마을의 정보를 얻는다 
class cltGameMsgResponse_StockBidVillageList
{
public:

	UI16			usCount;						// 증자 중인 마을 숫자

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockBidVillageInfo, MAX_VILLAGE_NUMBER>		clStockBidVillageInfo;
#else
	cltStockBidVillageInfo	clStockBidVillageInfo[ MAX_VILLAGE_NUMBER ];
#endif

	cltGameMsgResponse_StockBidVillageList()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_StockBidVillageList ) );
	}

	SI32	GetTotalSize() {
		return sizeof( SI32 ) + sizeof( cltStockBidVillageInfo ) * ( usCount );
	}	

};

// 마을 주식 입찰자 정보를 얻어온다
class cltGameMsgResponse_StockBidder
{
public:
	SI32					siVillageUnique;								// 마을 유니크
	UI16					usCount;										// 입찰자 숫자

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockBidder, MAX_STOCKBIDDER_INVILLAGE>			clStockBidder;
#else
	cltStockBidder			clStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];		// 입찰자 배열
#endif

	cltGameMsgResponse_StockBidder()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_StockBidder ) );
	}

	SI32					GetTotalSize() {
		return sizeof( SI32 ) + sizeof( SI32 ) + usCount * sizeof( cltStockBidder );
	}

};


//// 입찰 신청을 한다 
//class cltGameMsgRequest_BidStock
//{
//public:
//	SI32					siVillageUnique;
//	SI32					siAmount;
//	cltMoney				clPrice;
//	
//
//	cltGameMsgRequest_BidStock( SI32 villageUnique, SI32 amount, cltMoney *price )
//	{
//		siVillageUnique = villageUnique;
//
//		siAmount = amount;
//
//		clPrice.Set( price );
//	}
//
//};

// 증자 신청을 한다 
class cltGameMsgRequest_BidStock
{
public:
	SI32	siVillageUnique;
	GMONEY	siStockMoney;	//	1주당 주식가격			

	cltGameMsgRequest_BidStock( SI32 villageUnique, GMONEY StockMoney )
	{
		siVillageUnique = villageUnique;
		siStockMoney = StockMoney;
	}

};

// 입찰 신청을 취소한다 
class cltGameMsgRequest_CancelBidStock
{
public:
	SI32				siVillageUnique;
	SI32				siPersonID;

	cltGameMsgRequest_CancelBidStock( SI32 villageUnique, SI32 personid )
	{
		siVillageUnique = villageUnique;
		siPersonID = personid;
	}
};

class cltGameMsgResponse_StockBidDate
{
public:
	SI32				siVillageUnique;
	cltDate				clBidDate;

	cltGameMsgResponse_StockBidDate( SI32 villageUnique, cltDate *pclDate )
	{
		siVillageUnique = villageUnique;
		clBidDate.Set( pclDate );
	}
};



// 운영자가 주식 공모 시작 했음을 알림
class cltGameMsgResponse_StockDistributionSet
{
public:
	SI32					siVillageUnique;
	SI32					siMaxAmount;
	SI32					siEachPersonAmount;
	GMONEY					siStockPrice;
	bool					bVillageWarStockDistribution;

	cltGameMsgResponse_StockDistributionSet( SI32 VillageUnique, SI32 MaxAmount, SI32 EachPersonAmount, GMONEY StockPrice, bool VillageWarStockDistribution )
	{
		siVillageUnique = VillageUnique;
		siMaxAmount = MaxAmount;
		siEachPersonAmount = EachPersonAmount;
		siStockPrice = StockPrice;

		bVillageWarStockDistribution = VillageWarStockDistribution;
	}
};


// 유저가 주식 공모에 참여
class cltGameMsgRequest_StockDistributionOrder
{
public:
	SI32	siVillageUnique;
	SI32	siAmount;

	bool	bVillageWarStockDistribution;
	
	cltGameMsgRequest_StockDistributionOrder( SI32 VillageUnique, SI32 Amount, bool VillageWarStockDistribution )
	{
		siVillageUnique = VillageUnique;
		siAmount = Amount;
		bVillageWarStockDistribution = VillageWarStockDistribution;
	}
};

class cltGameMsgResponse_StockDistributionOrder
{
public:
	SI32	siVillageUnique;
	SI32	siAmount;				// 실제 구입 수량... 순서에 밀려서 얼마 못 살 경우도 있으므로..
	GMONEY	siPrice;				// 주당 구입 가격

	cltGameMsgResponse_StockDistributionOrder( SI32 VillageUnique, SI32 Amount, GMONEY Price )
	{
		siVillageUnique = VillageUnique;
		siAmount = Amount;
		siPrice = Price;	
	}
};

//-----------------------------------------------
// 여각의 주식 상황 클라이언트로 통보 
//-----------------------------------------------
class cltGameMsgResponse_StockTradeInfo
{
public:
	SI16	siVillageUnique;
	GMONEY	siSellPrice;
	SI32	siSellAmount;
	GMONEY	siBuyPrice;
	SI32	siBuyAmount;

	cltGameMsgResponse_StockTradeInfo(SI16 villageunique, GMONEY sellprice, SI32 sellamount, GMONEY buyprice, SI32 buyamount)
	{
		siVillageUnique	= villageunique;
		siSellPrice		= sellprice;
		siSellAmount	= sellamount;
		siBuyPrice		= buyprice;
		siBuyAmount		= buyamount;

	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_Investlist
//
// Last Modified 	: 2006 / 02 / 07
// Created		 	: 김광명
//
// Function			: 선택한 마을의 주주정보를 요청한다.
//
//***************************************************************************************************
class cltGameMsgRequest_Investlist
{
public:
	SI32 siVillageUnique_;

	cltGameMsgRequest_Investlist() : siVillageUnique_( 0 ) {}

	cltGameMsgRequest_Investlist( SI32 VillageUnique) : siVillageUnique_( VillageUnique ) {}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_BuyBidStock
//
// Last Modified 	: 2006 / 02 / 07
// Created		 	: 김광명
//
// Function			: 증자주식 사는것을 요청한다.
//
//*************************************************************************************************** 
class cltGameMsgRequest_BuyBidStock
{
public:
	SI16					siType_;	// 0은 주주일때 1은 일반인 전체일때
	SI32					siVillageUnique_;
	SI32					siAmount_;
	cltMoney				clPrice_;


	cltGameMsgRequest_BuyBidStock( SI16 siType, SI32 villageUnique, SI32 amount, cltMoney *price )
	{
		siType_ = siType;
		siVillageUnique_ = villageUnique;
		siAmount_ = amount;
		clPrice_.Set( price );
	}
};

// 현재 주식 공모 중인 마을의 정보를 얻는다 
class cltGameMsgResponse_StockOfferingInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockOfferingInfo, MAX_VILLAGE_NUMBER>		clStockOfferingInfo;
#else
	cltStockOfferingInfo	clStockOfferingInfo[ MAX_VILLAGE_NUMBER ];
#endif

	cltGameMsgResponse_StockOfferingInfo()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_StockOfferingInfo ) );
	}
};

// 전장금액부족으로 주식주문 취소 알림
class cltGameMsgResponse_CancelStockOrder
{
public:
	cltGameMsgResponse_CancelStockOrder()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_CancelStockOrder ) );
	}
};

class cltGameMsgRequest_StockOfferingOrder
{
public:
	SI32			siVillageUnique;
	TCHAR			szMacAddress[20];
	TCHAR			szPrivateIP[20];

public:
	cltGameMsgRequest_StockOfferingOrder(SI32 VillageUnique, TCHAR* pszPrivateIP, TCHAR* pszMacAddress)
	{
		szMacAddress[0] = '\0';

		siVillageUnique = VillageUnique;
		if(pszMacAddress)		MStrCpy(szMacAddress, pszMacAddress, sizeof(szMacAddress));
		if(pszPrivateIP)		MStrCpy(szPrivateIP,  pszPrivateIP,  sizeof(szPrivateIP));
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_ExchangeStock
//
// Last Modified 	: 2006 / 02 / 07
// Created		 	: 김광명
//
// Function			: 주식을 현금으로 바꾼다
//
//***************************************************************************************************
class cltGameMsgRequest_ExchangeStock
{
public:
	SI32 siVillageUnique;
	SI32 siSellAmount;
	SI32 siBankType;
	TCHAR szBankAccount[MAX_PLAYER_NAME];
	TCHAR szSecondPassWord[MAX_PLAYER_NAME];

	cltGameMsgRequest_ExchangeStock( SI32 VillageUnique, SI32 SellAmount, SI32 BankType, TCHAR* BankAccount, TCHAR* SecondPassWord )
	{
		siVillageUnique = VillageUnique;
		siSellAmount = SellAmount;
		siBankType = BankType;
		StringCchCopy( szBankAccount, MAX_PLAYER_NAME, BankAccount );
		StringCchCopy( szSecondPassWord, MAX_PLAYER_NAME, SecondPassWord );
	}
};

class cltGameMsgResponse_ExchangeStock
{
public:
	bool bResult;
	SI32 siVillageUnique;
	SI32 siSellAmount;
	SI32 siLeftAmount;

	cltGameMsgResponse_ExchangeStock( bool Result, SI32 VillageUnique, SI32 SellAmount, SI32 LeftAmount) : bResult( Result ), siVillageUnique( VillageUnique ), siSellAmount( SellAmount ), siLeftAmount( LeftAmount ) {}
};
#endif

