//---------------------------------
// 2003/6/18 김태곤
//---------------------------------
#ifndef _MARKET_H
#define _MARKET_H

#include <Stdio.h>
#include "..\StructureClass\StructureClass.h"
#include "..\Account\\Account.h"

#include "../Client/Inventory/ItemInventoryBaseDefinition.h"

#include "..\..\Common\Item\ItemCommon\cltitem.h"
#include "..\TradeOrder\TradeOrder.h"

#define MAX_MARKET_TAB_NUMBER		4

// 중국은 객구거래 20000개
#ifdef _CHINA
	#define MAX_MARKET_ORDER_NUMBER		30000
#else
	#define MAX_MARKET_ORDER_NUMBER		15000
#endif

#if defined (_CHINA)
	#define MAX_RESPONSE_ORDER_NUMBER	150				// 서버의 주문 정보중 이 개수 만큼만 받는다. 
#elif defined (_KOREA)
	#define MAX_RESPONSE_ORDER_NUMBER	150
#else
	#define MAX_RESPONSE_ORDER_NUMBER	100	
#endif

#define MAX_PRICE_IN_MARKET			100000000000	// 1000억. 객주에 올려 놓을 수 있는 최대 가격. 
#define MARKET_ORDER_SEND_UNIT		10				// 객주 주문은 10개 단위로 클라이언트로 보내진다. 

#define MAX_NEWMARKET_ITEMLIST				 50
#define	MAX_PERSON_NEWMARKET_ORDER_NUM		 12	// PERSON 당 판매 등록이 가능한 수.

class cltTaxPaymentListDlg;

class cltGameMsgRequest_NewSellItem
{
public:
	cltNewTradeOrder	clTradeOrder;

	SI16				siPos;

	cltGameMsgRequest_NewSellItem(cltNewTradeOrder* pclorder,SI16 pos)
	{
		clTradeOrder.Set(pclorder);

		siPos = pos ;
	}
};

// 객주로부터 아이템을 회수하기 위한 정보. 
class cltGetItemFromMarketInfo
{
public:
	SI32	siPersonID;		// 대상이 되는 Person
	
	SI32	siFromPos;		// 아이템의 계좌내 위치. 
	bool	bPileSwitch;	// 집적되는지 여부. 
	cltItem clFromItem;		// 아이템의 계좌내 위치에 존재하는 아이템.
							// 가져올 개수는 siItemNum에 업데이트 되어 있어야 한다. 

	SI32	siToPos;		// 아이템을 가져다 놓을 인벤내 위치. 
	cltItem clToItem;		// 아이템을 가져다 놓응 인벤내 위치에 존재하는 아이템 정보. 


	cltGetItemFromMarketInfo(SI32 personid, SI32 frompos, bool pileswitch, cltItem* pclfromitem, SI32 topos, cltItem* pcltoitem)
	{
		siPersonID	= personid;
		siFromPos	= frompos;
		bPileSwitch	= pileswitch;
		clFromItem.Set(pclfromitem);

		siToPos		= topos;
		if(pcltoitem)
		{
			clToItem.Set(pcltoitem);
		}
		else
		{
			clToItem.Init();
		}
	}

};

// 대상전에 물건 찾기
class cltGetItemFromNewMarketInfo{
public:

	SI32	siOrderIndex;		// 주문번호
	bool	bPileSwitch;		// 집적되는지 여부. 
	cltItem clFromItem;		// 아이템의 계좌내 위치에 존재하는 아이템.
	//가져올 개수는 siItemNum에 업데이트 되어 있어야 한다. 

	SI32	siToPos;			// 아이템을 가져다 놓을 인벤내 위치. 
	cltItem clToItem;		// 아이템을 가져다 놓응 인벤내 위치에 존재하는 아이템 정보. 


	cltGetItemFromNewMarketInfo(SI32 orderindex, bool pileswitch, cltItem* pclfromitem, SI32 topos, cltItem* pcltoitem)
	{
		siOrderIndex	= orderindex;
		bPileSwitch		= pileswitch;
		clFromItem.Set(pclfromitem);

		siToPos			= topos;
		pcltoitem ? clToItem.Set(pcltoitem) : clToItem.Init();
	}
};
//--------------------------------------------
// 객주 계좌 정보. 
//--------------------------------------------	
class cltMarketAccount 
{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMoney,	MAX_MARKET_ORDER_PER_PERSON>		clMoney;
	NSafeTArray<cltItem,	MAX_MARKET_ORDER_PER_PERSON>		clItem;
#else
	cltMoney	clMoney[MAX_MARKET_ORDER_PER_PERSON];		// 아이템의 가격. 
	cltItem		clItem[MAX_MARKET_ORDER_PER_PERSON];		// 객주에 올려놓은 물건의 정보. 
#endif

	cltMarketAccount() 
	{
		Init();
	};


	~cltMarketAccount(){}
	
	void Init()
	{
		for(SI32 i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
		{
			clMoney[i].Init();
			clItem[i].Init();
		}
	}

	void Set(cltMarketAccount* pclinfo)
	{
		memcpy(this, pclinfo, sizeof(cltMarketAccount));
	}

	BOOL IsSame(cltMarketAccount* pclaccount);

	// 특정 아이템의 수를 파악해서 리턴한다.
	SI32 GetItemNum(cltItem* pclitem);
	
	// 종목을  추가한다. 
	BOOL Add(cltItem* pclitem, SI32 tradeamount);
	// 종목을 뺀다.
	BOOL Sub(cltItem* pclitem, SI32 tradeamount);
	
	// 특정한 아이템을 찾아서 그 포인터를 돌려준다. 
	cltItem* FindItem(const cltItem* pclitem, bool numcomp);
	
	// 객주 주문 정보를 업데이트 한다. 
	bool UpdateMarketOrder(SI32 index, cltTradeOrder* pclorder);
};

class cltNewMarketAccount
{
public:

	SI32		siOrderIndex[MAX_NEWMARKET_ORDER_PER_PERSON];
	cltMoney	clMoney[MAX_NEWMARKET_ORDER_PER_PERSON];		// 아이템의 가격. 
	cltItem		clItem[MAX_NEWMARKET_ORDER_PER_PERSON];			// 객주에 올려놓은 물건의 정보. 

	cltNewMarketAccount() 
	{
		Init();
	};


	~cltNewMarketAccount(){}

	void Init()
	{

		for(SI32 i = 0;i < MAX_NEWMARKET_ORDER_PER_PERSON;i++)
		{
			siOrderIndex[i] = -1;
			clMoney[i].Init();
			clItem[i].Init();
		}
	}

	void Set(cltNewMarketAccount* pclinfo)
	{
		memcpy(this, pclinfo, sizeof(cltNewMarketAccount));
	}

	BOOL IsSame(cltNewMarketAccount* pclaccount);

	// 특정 아이템의 수를 파악해서 리턴한다.
	SI32 GetItemNum(cltItem* pclitem);

	// 특정한 아이템을 찾아서 그 포인터를 돌려준다. 
	cltItem* FindItem(const cltItem* pclitem, bool numcomp);
};

//-----------------------------------------------
// 개별적인 객주 
//-----------------------------------------------
class cltMarket : public cltStructureBasic
{
public:

	cltMarket(SI32 sivillageunique);
	~cltMarket();
};


//----------------------------------------------
// 객주 관리자. 
//----------------------------------------------
class cltMarketManager : public cltStructureMngBasic
{
public:
	cltTradeOrderManager*	pclOrderManager;		// 사자, 팔자 주문 관리자. 

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32*,	MAX_ITEMTYPE_NUMBER>		pclItemTypeSearch;
	NSafeTArray<SI32*,	MAX_ITEMTYPE_NUMBER>		pclItemTypeOrder;
	NSafeTArray<SI32,	MAX_ITEMTYPE_NUMBER>		siItemTypeOrderIndex;
#else
	SI32*											pclItemTypeSearch[MAX_ITEMTYPE_NUMBER];	
	SI32*											pclItemTypeOrder[MAX_ITEMTYPE_NUMBER];	// 각 아이템 타입별로 주문 목록. 
	SI32											siItemTypeOrderIndex[MAX_ITEMTYPE_NUMBER];
#endif

	// 마켓에 등록된 아이템들의 최소가격과 그 아이템의 등록 개수를 저장한다.
#ifdef _SAFE_MEMORY
	NSafeTArray<GMONEY, MAX_ITEMINFO_NUMBER>		m_MinPrice;								// 아이템 당 최저가격
	NSafeTArray<SI32,	MAX_ITEMINFO_NUMBER>		m_MinPriceAmount;						// 최저가격 아이템의 등록 개수
#else
	GMONEY											m_MinPrice[MAX_ITEMINFO_NUMBER];		// 아이템 당 최저가격
	SI32											m_MinPriceAmount[MAX_ITEMINFO_NUMBER];	// 최저가격 아이템의 등록 개수
#endif

	BOOL					ImageListConnectedSwitch;
	cltPItemInfo			clOldPersonItemInfo;

	BOOL					ImageListConnectedToStorageSwitch;	
	cltMarketAccount		clOldAccount;

	BOOL					SellButtonEnableSwitch;
	BOOL					GetFromMarketButtonEnableSwitch;

	cltItem*				pclCurrentInvItem;						// 현재 선택된 인벤토리 아이템.
	cltItem					clCurrentInvItem;

	cltItem*				pclCurrentStgItem;						// 현재 선택된 객주 창고 아이템.
	SI32					CurrentStgItemIndex;					// 현재 선택된 객주 창고 아이템의 창고내 인덱스. 

	HBITMAP					m_hItemImage;
	HBITMAP 				m_hItemMallBitmap;

	cltItem 				clCurrentStgItem;

	BOOL 					bResponseEmptyOrder;
	BOOL 					ShowResponseOrderSiwtch;
	SI32 					siResponseOrderIndex;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltTradeOrder, MAX_RESPONSE_ORDER_NUMBER>	clResponseOrder;
#else
	cltTradeOrder											clResponseOrder[MAX_RESPONSE_ORDER_NUMBER];	// 서버에서 받은 주문정보를 저장할 공간. 
#endif

	SI32 	CurrentSelectedBuyItemUnique;						// 구매를 목적으로 현재 선택된 아이템의 유니크. 

	HWND 	hUserDlgBuyHwnd;									// 구매 대화상자의 핸들. 

	HWND 	m_hUserDlgBuyList;									// 살 물건 리스트의 핸들
	SI32 	m_siSelectedBuyList;								// 살 물건 리스트에서 선택한 물건

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_RESPONSE_ORDER_NUMBER>		m_siSelectedBuyIndex;
#else
	SI32												m_siSelectedBuyIndex[MAX_RESPONSE_ORDER_NUMBER];	// 살 물건 리스트에서 선택한 물건들의 인덱스
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_MARKET_ORDER_NUMBER>			siRefBuffer;
#else
	SI16												siRefBuffer[MAX_MARKET_ORDER_NUMBER];
#endif
	
	SI32	siRefBufferIndex;

	cltMarketManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltMarketManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 객주를 신설한다.
	BOOL CreateMarket(SI32 siunique);

	// 계좌에 넣는다. 
	BOOL Input(SI32 personid, SI32 acountnum, cltItem* pclitem, SI32 tradeamount);
	// 계좌에서 꺼낸다. 
	BOOL Output(SI32 personid, SI32 acountnum, cltItem* pclitem, SI32 tradeamount);

	// 특정아이템의 가장 저렴한 주문을 찾는다. 
	bool FindLowPriceOrderInfo(SI32 unique, GMONEY* pprice, SI32* pamount);

	// 마켓에 등록된 아이템들의 최소가격과 그 아이템의 등록 개수를 저장한다.
	void MakeLowPriceOrderInfo();

	//--------------------------------------
	// Market-Manager.cpp
	//--------------------------------------
	// 임대료를 처리하는등의 액션을 취한다. 
	void Action(bool bdaychangedswitch);

	// 서버 리턴값 처리 함수. 
	void ServerRtnValOperate();

	// 서버에서 받은 주문 정보를 보관한다. 
	// 클라이언트에서 원래 요청했던 정보를 받은 것이다. 
	BOOL PutResponseOrder(cltTradeOrder* pclorder);

	
	// 아이템 타입별 버퍼에 객주 주문 정보를 추가한다. 
	bool SetItemTypeOrder(cltTradeOrder* pclorder, SI32 orderunique);


	// 객주에 올라와 있는 물품 중에서 랜덤하게 고른다. 
	SI16 GetRandomOrder(SI32 itemtype);

	// 객주에 올라와 있는 물품 중에서 시스템이 구입할 물품을 선정한다. 
	cltTradeOrder* GetOrderForSystemBuy(SI32 itemtype);

	// 객주에 올라와 있는 물품 중에서 시스템이 구입할 물품을 선정한다.  - unique로 선택.	
	cltTradeOrder* GetOrderForSystemBuyUnique(SI16 itemUnique , GMONEY gPrice);

	void SystemBuyItemStatistics(SI16 siItemUnique, GMONEY gPrice);
};

//-----------------------------------------------
// 개별적인 대상전 
//-----------------------------------------------
class cltNewMarket : public cltStructureBasic
{
public:

	cltNewMarket(SI32 sivillageunique);
	~cltNewMarket();
};


//----------------------------------------------
// 대상전 관리자. 
//----------------------------------------------
class cltNewMarketManager : public cltStructureMngBasic
{
public:

	bool						m_bSetFee;						// 하루에 한번만 수수료 설정이 가능하게.
	bool						bUpdateItemPrice;
	bool						bUpdateItemReducePrice;

	// 세금 납부 내역 
	DWORD						dwUpdateTime;
	cltTaxPaymentListDlg*		pclTaxPaymentDlg;

	cltNewMarketManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, char *strFileName );
	~cltNewMarketManager();

	// 최초 환영 인사소환수표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 대상전
	BOOL CreateNewMarket(SI32 siunique);

	// 임대료를 처리하는등의 액션을 취한다. 
	void Action(cltTime* pcltime, clock_t CurrentClock);

	// 서버 리턴값 처리 함수. 
	void ServerRtnValOperate();

};

#endif
