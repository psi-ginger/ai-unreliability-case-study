#ifndef _MSGTYPEHORSEMARKET_H
#define _MSGTYPEHORSEMARKET_H

#include "..\HorseMarket\HorseMarket.h"

// 사복시 내 주문 정보 
class cltGameMsgRequest_HorseMarketMyAccount
{
public:
	cltGameMsgRequest_HorseMarketMyAccount()
	{
	}

};

class cltGameMsgResponse_HorseMarketMyAccount
{
public:
	cltHorseOrder	clHorseOrder;

	cltGameMsgResponse_HorseMarketMyAccount( cltHorseOrder *pclHorseOrder )
	{
		clHorseOrder.Set( pclHorseOrder );
	}

};

// 사복시 주문 리스트
class cltGameMsgRequest_HorseMarketOrderList
{
public:
	SI32	siHorseUnique;	
	SI32	siPriceType;

	cltGameMsgRequest_HorseMarketOrderList( SI32 horseUnique, SI32 pricetype )
	{
		siHorseUnique = horseUnique;
		siPriceType = pricetype;
	}

};

class cltGameMsgResponse_HorseMarketOrderList
{
public:
	UI16			usCount;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHorseOrder, MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER>		clHorseOrder;
#else
	cltHorseOrder	clHorseOrder[ MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER ];
#endif

	cltGameMsgResponse_HorseMarketOrderList()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_HorseMarketOrderList ) );
	}

	SI32			GetTotalSize() {
		return sizeof( cltHorseOrder ) * usCount + 8;
	}

};

// 사복시 말 팔기 
class cltGameMsgRequest_HorseMarketSellHorse
{
public:
	SI16			siHorseIndex;				// 말 슬롯 
	GMONEY			siPrice;					// 판매 가격 
	

	cltGameMsgRequest_HorseMarketSellHorse( SI16 horseindex, GMONEY price )
	{
		siHorseIndex = horseindex;
		siPrice = price;
	}
};

class cltGameMsgResponse_HorseMarketSellHorse
{
public:
	SI16		siHorseIndex;
	cltHorseOrder	clHorseOrder;

	cltGameMsgResponse_HorseMarketSellHorse()
	{
		clHorseOrder.Init();
	}

	cltGameMsgResponse_HorseMarketSellHorse( SI16 horseindex, cltHorseOrder *pclHorseOrder )
	{
		siHorseIndex = horseindex;
		clHorseOrder.Set( pclHorseOrder );
	}
};



// 사복시 말 구입 
class cltGameMsgRequest_HorseMarketBuyHorse
{
public:
	SI32	siOrderIndex;
	SI32	siSellerPersonID;

	cltGameMsgRequest_HorseMarketBuyHorse( SI32 orderindex, SI32 sellerpersonid )
	{
		siOrderIndex = orderindex;
		siSellerPersonID = sellerpersonid;
	}

};

class cltGameMsgResponse_HorseMarketBuyHorse
{
public:
	SI16			siHorseIndex;
	cltHorse		clHorse;
	SI32			siOrderIndex;

	cltGameMsgResponse_HorseMarketBuyHorse( SI16 horseindex, cltHorse *pclHorse, SI32 orderIndex )
	{
		siHorseIndex = horseindex;
		if(pclHorse)	clHorse.Set( pclHorse );
		siOrderIndex = orderIndex;
	}


};

class cltGameMsgRequest_HorseMarketOutHorse
{
public:
	cltGameMsgRequest_HorseMarketOutHorse()
	{
	}

};

class cltGameMsgResponse_HorseMarketOutHorse
{
public:
	SI16		siHorseIndex;
	cltHorse		clHorse;

	cltGameMsgResponse_HorseMarketOutHorse( SI16 horseindex, cltHorse *pclHorse )
	{
		siHorseIndex = horseindex;
		if(pclHorse)	clHorse.Set( pclHorse );
	}
};


// 사복시 수수료 설정
class cltGameMsgRequest_HorseMarketSetFee
{
public:
	cltFee				clFee;						// 거래 수수료 

	cltGameMsgRequest_HorseMarketSetFee()
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_HorseMarketSetFee ) );
	}

	cltGameMsgRequest_HorseMarketSetFee( cltFee* pclFee )
	{
		clFee.Set( pclFee );
	}

};


#endif