#ifndef _MSGTYPESUMMONHEROMARKET_H
#define _MSGTYPESUMMONHEROMARKET_H

#include "..\SummonHeroMarket\SummonHeroMarket.h"

// 소환수거래소 내 주문 정보 
class cltGameMsgRequest_SummonHeroMarketMyOrder
{
public:
	cltGameMsgRequest_SummonHeroMarketMyOrder()
	{
	}

};

class cltGameMsgResponse_SummonHeroMarketMyOrder
{
public:

	UI16			usCount;
	
	cltSummonOrder	clMyOrder[ MAX_SUMMONHEROMARKET_ORDER_PER_PERSON ];

	cltGameMsgResponse_SummonHeroMarketMyOrder()
	{
		usCount = 0;
	}

};

// 소환수거래소 주문 리스트
class cltGameMsgRequest_SummonHeroMarketOrderList
{
public:
	SI32			siStartPos;
	SI32			siKind;

	SI32			siSelectList;

	cltGameMsgRequest_SummonHeroMarketOrderList( SI32 startpos, SI32 kind, SI32 selectlist )
	{
		siStartPos = startpos;
		siKind = kind;

		siSelectList = selectlist;

	}
};

class cltGameMsgResponse_SummonHeroMarketOrderList
{
public:
	UI16				usCount;
	cltSummonOrder		clOrder[ MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER ];
	SI32				siNextStartPos;
	SI32				skKind;

	cltGameMsgResponse_SummonHeroMarketOrderList()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_SummonHeroMarketOrderList ) );
	}

	SI32			GetTotalSize() {
		return sizeof( cltSummonOrder ) * usCount + 8;
	}

};

// 소환수거래소 소환수 팔기 
class cltGameMsgRequest_SummonHeroMarketSell
{
public:
//#if defined(_LEON_ADD_SELECTSELL_CODE)
	enum {
        TRADE_TYPE_NORMAL = 1,
		TRADE_TYPE_SELECTIVE = 2,
	};
//#endif

	SI32			siSummonIndex;
	GMONEY			siPrice;

	TCHAR			szSelectPersonName[ MAX_PLAYER_NAME ];

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	UI08			uiTradeType;		// 일반 거래 : TRADE_TYPE_NORMAL, 지정 거래 : TRADE_TYPE_SELECTIVE
//#endif

	cltGameMsgRequest_SummonHeroMarketSell( SI32 index, GMONEY price, TCHAR *personname, UI08 tradetype )
	{
		siSummonIndex = index;
		siPrice = price;

		memcpy( szSelectPersonName, personname, MAX_PLAYER_NAME );
		szSelectPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;

//#if defined(_LEON_ADD_SELECTSELL_CODE)
		uiTradeType = tradetype;
//#endif

	}

};

class cltGameMsgResponse_SummonHeroMarketSell
{
public:
	cltSummonOrder	clSummonOrder;

	cltGameMsgResponse_SummonHeroMarketSell()
	{
		clSummonOrder.Init();
	}

	cltGameMsgResponse_SummonHeroMarketSell( cltSummonOrder *pclOrder )
	{
		clSummonOrder.Set( pclOrder );
	}
};



// 소환수거래소 소환수 구입 
class cltGameMsgRequest_SummonHeroMarketBuy
{
public:
	SI32	siOrderIndex;

	cltGameMsgRequest_SummonHeroMarketBuy( SI32 orderindex )
	{
		siOrderIndex = orderindex;
	}

};

class cltGameMsgResponse_SummonHeroMarketBuy
{
public:
	SI32			siOrderIndex;
	cltSummon		clSummon;

	cltGameMsgResponse_SummonHeroMarketBuy( SI32 orderIndex, cltSummon *pclSummon )
	{
		siOrderIndex = orderIndex;
		clSummon.Set( pclSummon );
	}


};

class cltGameMsgRequest_SummonHeroMarketCancelOrder
{
public:
	SI32			siOrderIndex;
	cltGameMsgRequest_SummonHeroMarketCancelOrder( SI32 orderindex )
	{
		siOrderIndex = orderindex;
	}

};

class cltGameMsgResponse_SummonHeroMarketCancelOrder
{
public:
	SI32			siOrderIndex;
	cltSummon		clSummon;

	cltGameMsgResponse_SummonHeroMarketCancelOrder( SI32 orderindex, cltSummon *pclSummon )
	{
		siOrderIndex = orderindex;
		clSummon.Set( pclSummon );
	}
};


// 소환수거래소 수수료 설정
class cltGameMsgRequest_SummonHeroMarketSetFee
{
public:
	cltFee				clFee;						// 거래 수수료 

	cltGameMsgRequest_SummonHeroMarketSetFee()
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_SummonHeroMarketSetFee ) );
	}

	cltGameMsgRequest_SummonHeroMarketSetFee( cltFee* pclFee )
	{
		clFee.Set( pclFee );
	}

};


#endif