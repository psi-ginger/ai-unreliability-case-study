#ifndef _MSGTYPE_TRADEMERCHANT_H_
#define _MSGTYPE_TRADEMERCHANT_H_

#include "../../Common/SystemNPC/TradeMerchantNPC/TradeMerchantNPC.h"

// NPC 가 구입할려는 아이템의 정보를 요청
class cltGameMsgRequest_TradeMerchantBuyInfo
{
public:
	SI32 siNPCCharUnique;
	SI32 siNPCKind;

	cltGameMsgRequest_TradeMerchantBuyInfo( SI32 NPCCharUnique, SI32 NPCKind )
	{
		siNPCCharUnique = NPCCharUnique;
		siNPCKind = NPCKind;
	}
};

class cltGameMsgResponse_TradeMerchantBuyInfo
{
public:
	CTradeMerchantNPC cTradeMerchantNPC;

	cltGameMsgResponse_TradeMerchantBuyInfo( CTradeMerchantNPC *pTradeMerchatNPC )
	{
		cTradeMerchantNPC.Set( pTradeMerchatNPC );
	}
};

class cltGameMsgRequest_TradeMerchantBuyItemFromPerson
{
public:	
	SI32 siNPCKind;
	SI16 siItemPos;
	cltItem clItem;

	cltGameMsgRequest_TradeMerchantBuyItemFromPerson( SI32 NPCKind, SI16 ItemPos, cltItem *pclItem )
	{
		siNPCKind = NPCKind;
		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class cltGameMsgResponse_TradeMerchantBuyItemFromPerson
{
public:	
	SI16 siItemPos;
	cltItem clItem;

	GMONEY siIncraseMoney;		// 받은 금액
	GMONEY siLeftMoney;			// 남은 총액

	cltGameMsgResponse_TradeMerchantBuyItemFromPerson( SI16 ItemPos, cltItem *pclItem, 
		GMONEY IncreaseMoney, GMONEY LeftMoney )
	{
		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siIncraseMoney = IncreaseMoney;
		siLeftMoney = LeftMoney;
	}
};



class cltGameMsgRequest_TradeMerchantHorseBuyInfo
{
public:
	SI32 siMerchantKind;
		
	cltGameMsgRequest_TradeMerchantHorseBuyInfo( SI32 MerchantKind )
	{
		siMerchantKind = MerchantKind;		
	}
};

class cltGameMsgResponse_TradeMerchantHorseBuyInfo
{
public:
	SI16 siHorseUnique;
	GMONEY siHorsePrice;

	cltGameMsgResponse_TradeMerchantHorseBuyInfo( SI16 HorseUnique, GMONEY HorsePrice )
	{
		siHorseUnique = HorseUnique;
		siHorsePrice = HorsePrice;
	}
};

class cltGameMsgRequest_TradeMerchantBuyHorseFromPerson
{
public:
	SI32 siMerchantKind;
		
	cltGameMsgRequest_TradeMerchantBuyHorseFromPerson( SI32 MerchantKind )
	{
		siMerchantKind = MerchantKind;
	}
};

class cltGameMsgResponse_TradeMerchantBuyHorseFromPerson
{
public:
	GMONEY siHorsePrice;
	GMONEY siLeftPersonMoney;

	cltGameMsgResponse_TradeMerchantBuyHorseFromPerson(	GMONEY HorsePrice, GMONEY LeftPersonMoney )
	{		
		siHorsePrice = HorsePrice;
		siLeftPersonMoney = LeftPersonMoney;
	}
};

class cltGameMsgRequest_TradeMerchantBuyItemInfo
{
public:
	SI32 siMerchantKind;
	SI16 siItemUnique;
	SI16 siItemPos;
	
	cltGameMsgRequest_TradeMerchantBuyItemInfo( SI32 MerchantKind,
		SI16 ItemUnique, SI16 ItemPos )
	{
		siMerchantKind = MerchantKind;

		siItemUnique = ItemUnique;
		siItemPos = ItemPos;
	}
};

class cltGameMsgResponse_TradeMerchantBuyItemInfo
{
public:
	SI32 siMerchantKind;
	
	SI16 siItemUnique;
	
	GMONEY siBuyPrice;
	
	cltGameMsgResponse_TradeMerchantBuyItemInfo( SI32 MerchantKind,
		SI16 ItemUnique, GMONEY BuyPrice )
	{
		siMerchantKind = MerchantKind;

		siItemUnique = ItemUnique;

		siBuyPrice = BuyPrice;
	}
};

class cltGameMsgRequest_TradeMerchantBuyItem
{
public:
	SI32 siMerchantKind;
	SI16 siItemUnique;
	SI16 siItemPos;
	
	cltGameMsgRequest_TradeMerchantBuyItem( SI32 MerchantKind,
		SI16 ItemUnique, SI16 ItemPos )
	{
		siMerchantKind = MerchantKind;

		siItemUnique = ItemUnique;
		siItemPos = ItemPos;
	}
};

class cltGameMsgResponse_TradeMerchantBuyItem
{
public:
	SI32 siMerchantKind;
	
	SI16 siItemPos;
	cltItem clItem;

	SI16 siItemUnique;
	
	GMONEY siIncreaseMoney;
	GMONEY siLeftPersonMoney;
	
	cltGameMsgResponse_TradeMerchantBuyItem( SI32 MerchantKind,
		SI16 ItemPos, cltItem *pclItem,
		SI16 ItemUnique,
		GMONEY IncreaseMoney, GMONEY LeftPersonMoney )
	{
		siMerchantKind = MerchantKind;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siItemUnique = ItemUnique;

		siIncreaseMoney = IncreaseMoney;
		siLeftPersonMoney = LeftPersonMoney;
	}
};

#endif
