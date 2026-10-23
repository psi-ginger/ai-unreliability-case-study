#ifndef _DBMSG_TRADEMERCHANT_H_
#define  _DBMSG_TRADEMERCHANT_H_

#include "DBMsg.h"
// 무역상인

// 어떤 무역상인이 구입할려는 것을 설정( DB 에 없으면 Insert, 있으면 Update )
class sDBRequest_TradeMerchantSet
{
public:
	sPacketHeader		packetHeader;
	
	SI32 siTradeMerchantKind;	// 무역 상인 NPC 종류
	
	GMONEY siBuyTotalMoney;		// 총 구입 가격
	
	SI16 siBuyItemUnique;		// 구입하는 아이템
	GMONEY siBuyItemPrice;		// 구입하는 아이템 가격

	sDBRequest_TradeMerchantSet( SI32 TradeMerchantKind, GMONEY BuyTotalMoney,
		SI16 BuyItemUnique, GMONEY BuyItemPrice )
	{
		packetHeader.usCmd = DBMSG_REQUEST_TRADEMERCHANTSET;
		packetHeader.usSize = sizeof( sDBRequest_TradeMerchantSet );

		siTradeMerchantKind = TradeMerchantKind;
		siBuyTotalMoney = BuyTotalMoney;

		siBuyItemUnique = BuyItemUnique;
		siBuyItemPrice = BuyItemPrice;
	}
};

class sDBResponse_TradeMerchantSet
{
public:
	sPacketHeader		packetHeader;

	SI32 siResult;				// 1 성공
	
	SI32 siTradeMerchantKind;	// 무역 상인 NPC 종류
	
	GMONEY siBuyTotalMoney;		// 총 구입 가격
	
	SI16 siBuyItemUnique;		// 구입하는 아이템
	GMONEY siBuyItemPrice;		// 구입하는 아이템 가격
	
	sDBResponse_TradeMerchantSet()
	{
		memset( this, 0, sizeof( sDBResponse_TradeMerchantSet ) );
	}
};

// 유저가 무역 상인에게 물건을 판매한다.
class sDBRequest_TradeMerchantBuyItemFromPerson
{
public:
	sPacketHeader		packetHeader;

	SI32 siCharID;
	SI32 siPersonID;
	
	SI32 siTradeMerchantKind;		// 무역 상인 NPC 종류

	SI16 siItemPos;					// 아이템 위치
	cltItem clItem;					// 아이템 
	
	GMONEY siIncreaseMoney;			// 아이템 가격 - siBuyTotalMoney 에서 이 액수 만큼 빼 줘야 함.
	GMONEY siTradeTax;				// 왕실자금으로 들어가는 세금 - siBuyTotalMoney에서 빼 줘야 함.
		
	sDBRequest_TradeMerchantBuyItemFromPerson( SI32 CharID, SI32 PersonID , SI32 TradeMerchantKind, SI16 ItemPos, cltItem *pclItem, GMONEY IncreaseMoney,GMONEY TradeTax )
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeMerchantBuyItemFromPerson );
		packetHeader.usCmd	= DBMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON;

		siCharID = CharID;
		siPersonID = PersonID;

		siTradeMerchantKind = TradeMerchantKind;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siIncreaseMoney = IncreaseMoney;
		siTradeTax		= TradeTax ;
	}
};


class sDBResponse_TradeMerchantBuyItemFromPerson
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;					// 1 성공, -2 : NPC 구입할수 있는 돈 부족 으로 실패
	
	SI32 siCharID;
	SI32 siPersonID;

	SI32 siTradeMerchantKind;		// 무역 상인 NPC 종류

	SI16 siItemPos;					// 아이템 위치
	cltItem clItem;					// 남은 아이템 정보

	GMONEY siIncreateMoney;			// 추가된 돈 ( 보낸거 그대로 넣어주면 됨 ) - 위에 siBuyTotalMoney 을 깎아내려야 함.
	GMONEY siTradeTax;				// 왕실자금으로 들어가는 세금
	GMONEY siLeftMoney;				// 추가하고 남은 돈

	GMONEY siLeftBuyTotalMoney;		// NPC 가 앞으로 구입할수 있는 남은 구입 가격

	sDBResponse_TradeMerchantBuyItemFromPerson()
	{
		memset( this, 0, sizeof( sDBResponse_TradeMerchantBuyItemFromPerson ) );
	}
};


// 상인이 말을 구입
class sDBRequest_TradeMerchantBuyHorseFromPerson
{
public:
	sPacketHeader		packetHeader;

	SI32 siCharID;
	SI32 siPersonID;
	
	SI32 siTradeMerchantKind;		// 상인 NPC 종류 ( DB 에서는 받은 값을 그냥 돌려 주면 됨 )

	SI16 siPersonHorseIndex;		// 소유한 말의 index

	GMONEY siIncreaseMoney;			// 말 가격
		
	sDBRequest_TradeMerchantBuyHorseFromPerson( SI32 CharID, SI32 PersonID,
		SI32 TradeMerchantKind, SI16 PersonHorseIndex,
		GMONEY IncreaseMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeMerchantBuyHorseFromPerson );
		packetHeader.usCmd	= DBMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON;

		siCharID = CharID;
		siPersonID = PersonID;

		siTradeMerchantKind = TradeMerchantKind;

		siPersonHorseIndex = PersonHorseIndex;

		siIncreaseMoney = IncreaseMoney;
	}
};


class sDBResponse_TradeMerchantBuyHorseFromPerson
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;					// 1 성공, -2 : NPC 구입할수 있는 돈 부족 으로 실패
	
	SI32 siCharID;
	SI32 siPersonID;

	SI32 siTradeMerchantKind;		// 상인 NPC 종류

	SI16 siPersonHorseIndex;		// 팔린 말의 index

	GMONEY siIncreaseMoney;			// 추가된 돈 ( 보낸거 그대로 넣어주면 됨 )
	GMONEY siLeftMoney;				// 추가하고 남은 돈	

	sDBResponse_TradeMerchantBuyHorseFromPerson()
	{
		memset( this, 0, sizeof( sDBResponse_TradeMerchantBuyHorseFromPerson ) );
	}
};


// 상인이 아이템을 구입
class sDBRequest_TradeMerchantBuyItem
{
public:
	sPacketHeader		packetHeader;

	SI32 siCharID;
	SI32 siPersonID;
	
	SI32 siTradeMerchantKind;		// NPC 종류 ( DB 에서는 받은 값을 그냥 돌려 주면 됨 )

	SI16 siItemPos;
	cltItem clItem;					// 상인이 구입하는 아이템의 정보

	GMONEY siIncreaseMoney;			// 말 가격
		
	sDBRequest_TradeMerchantBuyItem( SI32 CharID, SI32 PersonID,
		SI32 TradeMerchantKind,
		SI16 ItemPos, cltItem *pclItem,
		GMONEY IncreaseMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeMerchantBuyItem );
		packetHeader.usCmd	= DBMSG_REQUEST_TRADEMERCHANTBUYITEM;

		siCharID = CharID;
		siPersonID = PersonID;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siTradeMerchantKind = TradeMerchantKind;

		siIncreaseMoney = IncreaseMoney;
	}
};


class sDBResponse_TradeMerchantBuyItem
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;					// 1 성공, -2 : NPC 구입할수 있는 돈 부족 으로 실패
	
	SI32 siCharID;
	SI32 siPersonID;

	SI32 siTradeMerchantKind;		// 상인 NPC 종류
	
	SI16 siItemPos;					// 아이템 위치
	SI16 siBuyItemUnique;			// 상인이 구입한 아이템의 Item Unique
	SI16 siBuyItemNum;				// 상인이 구입한 아이템의 개수
	cltItem clLeftItem;				// 상인이 구입하고 남은 유저의 아이템 정보

	GMONEY siIncreaseMoney;			// 추가된 돈 ( 보낸거 그대로 넣어주면 됨 )
	GMONEY siLeftMoney;				// 추가하고 남은 돈	

	sDBResponse_TradeMerchantBuyItem()
	{
		memset( this, 0, sizeof( sDBResponse_TradeMerchantBuyItem ) );
	}
};

#endif