//---------------------------------
// 2003/6/20 김태곤
//---------------------------------
#ifndef _TRADEORDER_H
#define _TRADEORDER_H

#include <Directives.h>
#include "../../Lib/EtcTool/EtcTool.h"

#include "../../../NLib/NDataTypes.h"
#include "../../common/Item/ItemCommon/ItemUnit.h"

#define TRADEORDER_DATASIZE 16		// cltItem의 사이즈가 이 데이터 사이즈가 된다. 

//-------------------------------------------
// 주문 타입 
//-------------------------------------------
#define TRADEORDER_TYPE_STOCK		1			// 주식 
#define TRADEORDER_TYPE_OBJECT		3			// 오브젝트 아이템. 
//_LEON_GUILD_STOCK
#define TRADEORDER_TYPE_GUILDSTOCK	5			// 길드주식




#define MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM	8

//------------------------------------------
// 사기, 팔기. 
//------------------------------------------
// 사는 주문인가. 파는 주문인가..
#define ORDERMODE_SELL			1
#define ORDERMODE_BUY			2

class cltNewTradeOrder
{
public:

	cltItem clItemData;
	bool 	bBankError;			// 전장에 돈이 꽉 차서 거래가 안되는 물건이냐? ( 시세 영향 못 끼친다 )
	SI16 	siServerUnique;		// 주문을 낸 유저의 서버 유니크
	SI32 	siOrderIndex;		// 통합 객주에 올려진 Index
	SI32 	siPersonID;			// 주문을 낸 PersonID
	SI32 	siAmount;			// 주문 수량. 
	SI32 	siDate;				// 날짜 정보.
	GMONEY 	siPrice;			// 주문 개당 가격. 

public:
	cltNewTradeOrder()
	{
		Init();
	}

	~cltNewTradeOrder()
	{
	}

	void Init()
	{
		clItemData.Init();
		bBankError		= false;
		siServerUnique	= 0;
		siOrderIndex	= 0;
		siPersonID		= 0;
		siAmount		= 0;
		siDate			= 0;
		siPrice			= 0;
	}

	cltNewTradeOrder( const cltItem* pclitem, bool bankerror, SI16 serverunique, SI32 orderindex, SI32 personid, SI32 amount, SI32 datevary, GMONEY price )
	{
		clItemData.Set(pclitem);
		bBankError		= bankerror;
		siServerUnique	= serverunique;
		siOrderIndex	= orderindex;
		siPersonID		= personid;
		siAmount		= amount;
		siDate			= datevary;
		siPrice			= price;

	}

	bool IsSame(cltNewTradeOrder* porder);

	// 유효한 주문인지 확인한다. 
	bool IsValid()
	{
		if(siAmount == 0)
			return false;

		return true;
	}

	void Set(cltNewTradeOrder* porder)
	{
		memcpy(this, porder, sizeof(cltNewTradeOrder));
	}

	//---------------------------
	// 아이템인 경우, 
	//---------------------------
	SI16 GetItemUnique()const		// 리턴값이 SI16이어야 한다. 
	{
		return clItemData.siUnique;
	}
};

class cltTradeOrder
{
public:
	SI16		siIndex;		// 주문 고유의 식별자(인덱스) (0~약 16000) 
	SI16		siType;			// 어떤종류의 주문인가?	// 주식, 아이템, 혹은 길드주식
	SI32		siPersonID;		// 주문을 낸 PersonID
	GMONEY		siPrice;		// 주문 개당 가격. 
	SI32		siAmount;		// 주문 수량. 
	SI32		siDate;			// 시간 정보.
	bool		bBankError;		// 전장에 돈이 꽉 차서 거래가 안되는 물건이냐? ( 시세 영향 못 끼친다 )

public:
	cltItemData clItemData;
	SI32		siVillageUnique;	// 주문의 대상이 되는 주식의 마을 유니크 
	
public:
	cltTradeOrder()
	{
		Init();
	}

	cltTradeOrder(SI32 personid, GMONEY price, SI32 amount, SI32 datevary, SI32 villageunique)
	{
		siIndex		= 0;
		siType		= TRADEORDER_TYPE_STOCK;
		siPersonID	= personid;
		siPrice		= price;
		siAmount	= amount;
		siDate		= datevary;
		bBankError	= false;

		clItemData.Init();
		siVillageUnique = villageunique;
	}
	cltTradeOrder(SI32 type, SI32 personid, GMONEY price, SI32 amount, SI32 datevary, const cltItem* pclitem)
	{
		siIndex		= 0;
		siType		= type;
		siPersonID	= personid;
		siPrice		= price;
		siAmount	= amount;
		siDate		= datevary;
		bBankError	= false;

		siVillageUnique = 0;		// 유니온이므로 먼저 초기화한다.
		clItemData.Set(pclitem);
	}

	void Init()
	{
		siIndex		= 0;
		siType		= 0;
		siPersonID	= 0;
		siPrice		= 0;
		siAmount	= 0;
		siDate		= 0;
		bBankError	= false;

		clItemData.Init();
		siVillageUnique = 0;
	}

	BOOL IsSame(cltTradeOrder* porder);

	// 유효한 주문인지 확인한다. 
	BOOL IsValid()
	{
		if(siAmount == 0)return FALSE;

		return TRUE;
	}

	void Set(cltTradeOrder* porder)
	{
		memcpy(this, porder, sizeof(cltTradeOrder));
	}

	//----------------------------
	// 주식인 경우, 
	//----------------------------
	SI32 GetVillageUnique()const{return siVillageUnique;}


	//---------------------------
	// 아이템인 경우, 
	//---------------------------
	SI16 GetItemUnique()const		// 리턴값이 SI16이어야 한다. 
	{
		return clItemData.siUnique;
	}
};

class cltTradeOrderManager{
public:
	SI32 siMaxOrderNum;

	cltTradeOrder** pclOrderSell;	// 팔자 주문 
	cltTradeOrder** pclOrderBuy;	// 사자 주문 	

	cltTradeOrderManager(SI32 ordernum);
	~cltTradeOrderManager();

	// 주문을 주문 버퍼의 정해진 자리에 넣는다. 
	bool SetOrder(SI32 ordermode, cltTradeOrder* porder);
	
	// 주어진 주문에 적절한 반대 주문을 찾는다. 
	// 찾으면 그 반대 주문의 포인터를 리턴하고 못 찾으면 NULL을 리턴한다.
	// PersonID : 같은 사람은 자기가 올려 놓은 반대 주문을 찾을 수 없다.
	cltTradeOrder* FindOpositeOrder(SI32 ordermode, cltTradeOrder* porder);

	bool GetMostSellBuyOrder(IN cltTradeOrder* porder, OUT cltTradeOrder* pSellMostOrderList, OUT cltTradeOrder* pBuyMostOrderList, OUT GMONEY * averageSell, OUT  GMONEY * averageBuy) ;

	// PersonID명의의 주문을 모두 취소하게 한다. 
	BOOL CancelOrder(SI32 personid);

	// 같은 종목인지 확인한다. 
	BOOL IsSameKind(const cltTradeOrder* pclsrc, const cltTradeOrder* pcldst);

	// 물품 주문중 가장 저렴한 주문의 정보를 찾는다. 
	bool FindLowPriceOrderInfo(SI32 unique, GMONEY* pprice, SI32* pamount);

	// 주식  주문중 가장 저렴한 주문의 정보를 찾는다. 
	bool FindLowPriceStockOrderInfo(SI32 villageunique, GMONEY* pprice, SI32* pamount);

	// 주식 주문중 가장 비싼 주문의 정보를 찾는다. 
	bool FindHighPriceStockOrderInfo(SI32 villageunique, GMONEY* pprice, SI32* pamount);



	// 주문 리스트에서 인덱스의 주문 정보를 확보한다. 
	cltTradeOrder* GetOrder(SI32 ordermode, SI32 index);

	// 주문의 개수를 구한다. 
	SI32 GetOrderNum(SI32 ordermode);
};

#define MAX_ORDER_PER_PERSON	5	// Person당 가질 수 있는 주식 주문 개수. 
#define MAX_STOCK_TOTURIAL_NUM 20	// 앙카란 주식을 가질수 있는 주문 개수.

// Person당 가질 수 있는 객주최대 주문 수
const SI32 MAX_MARKET_ORDER_PER_PERSON		= 8;
const SI32 MAX_NEWMARKET_ORDER_PER_PERSON	= 12;	//[진성] 통합시장.

//--------------------------------------
// person의 주문 정보. 
//--------------------------------------
class cltCharOrderInfoUnit{
public:
	SI32			siOrderMode;	// ORDERMODE_SELL, ORDERMODE_BUY
	cltTradeOrder	clOrder;

	cltCharOrderInfoUnit()
	{
		siOrderMode = 0;
	}
};

class cltCharOrderInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltCharOrderInfoUnit, MAX_MARKET_ORDER_PER_PERSON>		clOrderUnit;
#else
	cltCharOrderInfoUnit clOrderUnit[MAX_MARKET_ORDER_PER_PERSON];
#endif

	cltCharOrderInfo()
	{
		ZeroMemory(this, sizeof(cltCharOrderInfo));
	}

	void Set(cltCharOrderInfo* porder)
	{
		memcpy(this, porder, sizeof(cltCharOrderInfo));
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltCharOrderInfo));
	}

	// 주문 정보를 설정한다.
	BOOL Set(SI32 ordertype, cltTradeOrder* pclorder);

	// 주문의 개수를 구한다. 
#ifdef _SAFE_MEMORY
	SI32 GetOrderNumber();
#else
	SI32 GetOrderNumber()const;
#endif
	// 주문 번호를 근거로 적절한 주문을 찾는다. 
	cltCharOrderInfoUnit* FindOrder(SI32 orderindex)
	{
		SI32 i;

		for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
		{
			if(clOrderUnit[i].clOrder.siIndex == orderindex)
			{
				return &clOrderUnit[i];
			}
		}
		return NULL;
	}

};

struct SellerInfo
{
	SI32				siSellerPersonID;		// 팔자의 PersonID
	SI16				siSellItemUnique;		// 거래된 아이템 유니크
	GMONEY				siSellPrice;			// 거래된 가격(총가격)
	SI32				siSellItemNum;			// 거래된 아이템의 개수.
	SI32				siVillageUnique;		// 물건을 올린 마을 유니크

	SellerInfo() : siSellerPersonID(0), siSellItemUnique(0), siSellPrice(0), siSellItemNum(0), siVillageUnique(0) 
	{
	}
	~SellerInfo()
	{}
};

#endif
