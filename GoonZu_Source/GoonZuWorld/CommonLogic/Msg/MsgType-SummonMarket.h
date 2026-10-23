#ifndef _MSGTYPESUMMONMARKET_H
#define _MSGTYPESUMMONMARKET_H

#include "..\SummonMarket\SummonMarket.h"

// 소환수거래소 내 주문 정보 
class cltGameMsgRequest_SummonMarketMyOrder
{
public:
	cltGameMsgRequest_SummonMarketMyOrder()
	{
	}

};

class cltGameMsgResponse_SummonMarketMyOrder
{
public:

	UI16			usCount;
	
	cltSummonOrder	clMyOrder[ MAX_SUMMONMARKET_ORDER_PER_PERSON ];

	cltGameMsgResponse_SummonMarketMyOrder()
	{
		usCount = 0;
	}

};

// 소환수거래소 주문 리스트
class cltGameMsgRequest_SummonMarketOrderList
{
public:
	SI32			siStartPos;
	SI32			siKind;

	SI32			siSelectList;


	cltGameMsgRequest_SummonMarketOrderList( SI32 startpos, SI32 kind, SI32 selectlist )
	{
		siStartPos = startpos;
		siKind = kind;

		siSelectList = selectlist;
	}

};

class cltGameMsgResponse_SummonMarketOrderList
{
public:
	SI32				siCurrentPage;
	SI32				siKind;
	UI16				usTotalPage;

	UI16				usCount;
	cltSummonOrder		clOrder[ MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER ];

	cltGameMsgResponse_SummonMarketOrderList()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_SummonMarketOrderList ) );
	}

	SI32			GetTotalSize()
	{
		return    sizeof(SI32) + sizeof(SI32) + sizeof(SI32)
				+ sizeof(UI16) + sizeof( cltSummonOrder ) * usCount;
	}
};

// 소환수거래소 소환수 팔기 
class cltGameMsgRequest_SummonMarketSell
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
	
	cltGameMsgRequest_SummonMarketSell( SI32 index, GMONEY price, TCHAR *personname, UI08 tradetype )
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

class cltGameMsgResponse_SummonMarketSell
{
public:
	cltSummonOrder	clSummonOrder;

	cltGameMsgResponse_SummonMarketSell()
	{
		clSummonOrder.Init();
	}

	cltGameMsgResponse_SummonMarketSell( cltSummonOrder *pclOrder )
	{
		clSummonOrder.Set( pclOrder );
	}
};



// 소환수거래소 소환수 구입 
class cltGameMsgRequest_SummonMarketBuy
{
public:
	SI32	siOrderIndex;

	cltGameMsgRequest_SummonMarketBuy( SI32 orderindex )
	{
		siOrderIndex = orderindex;
	}

};

class cltGameMsgResponse_SummonMarketBuy
{
public:
	SI32			siOrderIndex;
	cltSummon		clSummon;

	cltGameMsgResponse_SummonMarketBuy( SI32 orderIndex, cltSummon *pclSummon )
	{
		siOrderIndex = orderIndex;
		clSummon.Set( pclSummon );
	}


};

class cltGameMsgRequest_SummonMarketCancelOrder
{
public:
	SI32			siOrderIndex;
	cltGameMsgRequest_SummonMarketCancelOrder( SI32 orderindex )
	{
		siOrderIndex = orderindex;
	}

};

class cltGameMsgResponse_SummonMarketCancelOrder
{
public:
	SI32			siOrderIndex;
	cltSummon		clSummon;

	cltGameMsgResponse_SummonMarketCancelOrder( SI32 orderindex, cltSummon *pclSummon )
	{
		siOrderIndex = orderindex;
		clSummon.Set( pclSummon );
	}
};


// 소환수거래소 수수료 설정
class cltGameMsgRequest_SummonMarketSetFee
{
public:
	cltFee				clFee;						// 거래 수수료 

	cltGameMsgRequest_SummonMarketSetFee()
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_SummonMarketSetFee ) );
	}

	cltGameMsgRequest_SummonMarketSetFee( cltFee* pclFee )
	{
		clFee.Set( pclFee );
	}

};


#endif