//---------------------------------
// 2003/6/2 김태곤
//---------------------------------

#ifndef _STOCK_H
#define _STOCK_H

#include <Directives.h>

#include <Stdio.h>
#include "..\StructureClass\StructureClass.h"
#include "..\Account\\Account.h"
#include "..\TradeOrder\TradeOrder.h"
#include "..\Common\StockOfferingManager\StockOfferingManager.h"

#define MAX_STOCK_TAB_NUMBER		4
#define MAX_STOCK_ORDER_NUMBER		5000	// 사자 팔자 주문 개수. 
#define MAX_AMOUNT_PER_DEAL			1000	// 한번에 주문을 낼 수 있는 최대 양. 
#define MAX_STOCK_ACOUNT_NUMBER		100000	// 주식계좌 개수. 
#define MAX_STOCK_KIND_IN_ACOUNT	10		// 한계좌당 가질 수있는 주식 종목 수. 

#define MAX_STOCKBIDDER_INVILLAGE	20		// 한마을당 최대 입찰자수

class cltTime;

class cltTaxPaymentListDlg;


//***************************************************************************************************
//
// Class Name 		: CRequsetIncreaseStockInfo
//
// Last Modified 	: 2006 / 01 / 26
// Created		 	: 김광명
//
// Function			: 마을 증자 주식 관련 정보
//
//***************************************************************************************************
class cltStockBidVillageInfo
{
public:
	SI32	siVillageUnique_;				 // 마을 유니크 
	SI32	siTotalStockAmount_;
	SI32	siTottalIncreaseStockAmount_;	 // 총 증자 주식수 
	SI32	siLeftIncreaseStockAmount_;		 // 남은 주식량
	GMONEY	siCurrentStockPrice_;			 // 1주 현재 주식값 

	cltDate	clBiddingDate_; // 증자 시작일

	cltStockBidVillageInfo()
	{
		siVillageUnique_ = 0;
		siTotalStockAmount_ = 0;
		siTottalIncreaseStockAmount_ = 0;
		siLeftIncreaseStockAmount_ = 0;
		siCurrentStockPrice_ = 0;

		clBiddingDate_.Set( 0, 0, 0, 0, 0 );
	}

	void Set( cltStockBidVillageInfo *pStockBidVillageInfo )
	{
		siVillageUnique_ = pStockBidVillageInfo->siVillageUnique_;
		siTotalStockAmount_ = pStockBidVillageInfo->siTotalStockAmount_;
		siTottalIncreaseStockAmount_ = pStockBidVillageInfo->siTottalIncreaseStockAmount_;
		siLeftIncreaseStockAmount_ = pStockBidVillageInfo->siLeftIncreaseStockAmount_;
		siCurrentStockPrice_ = pStockBidVillageInfo->siCurrentStockPrice_;

		clBiddingDate_.Set( &pStockBidVillageInfo->clBiddingDate_ );
	};
};
/*
class cltStockBidVillageInfo
{
public:
cltStockBidVillageInfo() {};

void Set( cltStockBidVillageInfo *pStockBidVillageInfo ) {

siVillageUnique = pStockBidVillageInfo->siVillageUnique;
clBiddingDate.Set( &pStockBidVillageInfo->clBiddingDate );
siStockAmount = pStockBidVillageInfo->siStockAmount;
siBeginningPrice = pStockBidVillageInfo->siBeginningPrice;
};

SI32					siVillageUnique;				// 마을 유니크 
cltDate					clBiddingDate;					// 최근 주식 경매일
SI32					siStockAmount;					// 증자 주식수 
GMONEY					siBeginningPrice;				// 입찰 시작가 
};
*/

// 증자 주식 입찰자 
class cltStockBidder
{
public:
	cltStockBidder() {};

	cltSimplePerson			clPerson;			// 입찰자 

	SI32					siVillageUnique;	// 입찰 마을 

	SI32					siAmount;			// 주식 수량 
	GMONEY					siPrice;			// 주식 가격 

	cltDate					clDate;				// 입찰일


public:
	void					Set( cltStockBidder *pclStockBidder )
	{
		clPerson.Set( &pclStockBidder->clPerson );

		siVillageUnique = pclStockBidder->siVillageUnique;

		siAmount = pclStockBidder->siAmount;
		siPrice = pclStockBidder->siPrice;

		clDate.Set( &pclStockBidder->clDate );
	}
};


// 주식 입찰자 관리자 
class cltStockBidderManager
{
public:
	cltStockBidderManager() {};

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockBidder, MAX_STOCKBIDDER_INVILLAGE>		clStockBidder;
#else
	cltStockBidder			clStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];			// 입찰자 리스트
#endif

	cltDate					clBiddingDate;										// 최근 마을 주식 경매일 

	BOOL					bBidding;											// 입찰중
};

// 경매 종료 후 입찰자 처리
class cltResultStockBidder
{
public:
	cltResultStockBidder() {
		ZeroMemory( this, sizeof( cltResultStockBidder ) );
	};

	BOOL					bSuccessfulBid;				// 낙찰 여부

	SI32					siPersonID;					// 입찰자 PersonID
	cltMoney				clPersonBankMoney;			// 입찰자 전장 계좌 잔고

	BOOL					bExceedLimit;				// 한도를 초과 했는가?
	cltMoney				clBidMoney;					// 총 입찰 금액 
};



//-------------------------------
// 주식계좌정보.
//-------------------------------
//class cltStockAccountUnit{
//private:
//
//public:
//	SI32 siVillageUnique;
//	SI32 siAmount;
//
//	cltStockAccountUnit()
//	{
//		ZeroMemory(this, sizeof(cltStockAccountUnit));
//	}
//
//	void Init()
//	{
//		ZeroMemory(this, sizeof(cltStockAccountUnit));
//	}
//
//	void Init(SI32 villageunique, SI32 amount)
//	{
//		siVillageUnique		= villageunique;
//		siAmount			= amount;
//	}
//
//	bool IsSame(cltStockAccountUnit* pclunit)
//	{
//		if(siVillageUnique != pclunit->siVillageUnique)return false;
//
//		if(siAmount != pclunit->siAmount)return false;
//
//		return true;
//	}
//	
//};
class cltStockAccountUnit
{
private:

public:
	SI32 siVillageUnique;
	SI32 siAmount;
	SI32 siLimitBuyBidStockAmount;
	bool bBuySwitch; // false일때 증자주식을 살 수 있다

	cltStockAccountUnit()
	{
		ZeroMemory(this, sizeof(cltStockAccountUnit));
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltStockAccountUnit));
	}

	void Init(SI32 villageunique, SI32 amount)
	{
		siVillageUnique		= villageunique;
		siAmount			= amount;
		siLimitBuyBidStockAmount = 0;
		bBuySwitch			= false;
	}

	bool IsSame(cltStockAccountUnit* pclunit)
	{
		if(siVillageUnique != pclunit->siVillageUnique)return false;

		if(siAmount != pclunit->siAmount)return false;

		return true;
	}

};


class cltStockAccount {
public:
	SI32			siPersonID;		// 계좌 주인 PersonID

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockAccountUnit, MAX_STOCK_KIND_IN_ACOUNT>		clUnit;
#else
	cltStockAccountUnit clUnit[MAX_STOCK_KIND_IN_ACOUNT];			// 잔고 . 
#endif
	
	cltStockAccount()
	{
		siPersonID	= 0;
	};

	cltStockAccount(SI32 sipersonid)
	{
		siPersonID	= sipersonid;
	};

	~cltStockAccount(){}
	
	void Init()
	{
		siPersonID	= 0;

		for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			clUnit[i].Init();
		}
	}

	void Init(SI32 sipersonid)
	{
		siPersonID	= sipersonid;

		for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			clUnit[i].Init();
		}
	}
	
	void Set(cltStockAccount* pclinfo)
	{
		memcpy(this, pclinfo, sizeof(cltStockAccount));
	}
	
	// 특정 종목의 주식수를 설정한다. 
	void Set(SI32 villageunique, SI32 amount);
	
	// 특정 종목의 주식 수를 파악해서 리턴한다.
	SI32 GetStockAmount(SI32 villageunique);
	
	// 주식을 추가한다. 
	BOOL Add(SI32 villageunique, SI32 tradeamount);
	// 주식을 뺀다.
	BOOL Sub(SI32 villageunique, SI32 tradeamount);

	// 같은 정보인가?
	bool IsSame(cltStockAccount* pclaccount)
	{
		if(siPersonID != pclaccount->siPersonID)return false;

		for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			if(clUnit[i].IsSame( &pclaccount->clUnit[i] ) == false)return false;
		}

		return true;
	}
	
	
	// 주식을 최소한 1주 이상 가지고 있는가?
	bool HaveStock()
	{
		for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			if(clUnit[i].siVillageUnique)return true;
		}

		return false;
	}
};


//-----------------------------------------------
// 사자 팔자 주문 관리자. 
//-----------------------------------------------
class cltStockOrderManager : public cltTradeOrderManager{
public:

	cltStockOrderManager();
	~cltStockOrderManager();

};


//-------------------------------------------------
// 개별적인 여각. 
//-------------------------------------------------
class cltStockMarket : public cltStructureBasic{
private:

public:
	cltStockMarket(SI32 sivillageunique);
	~cltStockMarket();

};



//--------------------------------------------------
// 증권관리자. 
//--------------------------------------------------
class cltStockManager : public cltStructureMngBasic{
public:

	cltStockOrderManager*	pclOrderManager;		// 주식 사자, 팔자 주문 관리자. 

	// SI32					siMostOrderMode;		// 최적 주문의 모드. (사자, 팔자. ) 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTradeOrder, MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM>		clSellMostOrder;
	NSafeTArray<cltTradeOrder, MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM>		clBuyMostOrder;
#else
	cltTradeOrder			clSellMostOrder[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];			// 최적 주문. 
	cltTradeOrder			clBuyMostOrder[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];			// 최적 주문. 
#endif
	
    GMONEY					siAverageSellMoney;
	GMONEY					siAverageBuyMoney;

	cltStockAccount			clOldStockInfo;


	BOOL					bInitBuyOrder;
	BOOL					bInitSellOrder;

	// 증자 관련 
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockBidVillageInfo, MAX_VILLAGE_NUMBER>		clStockBidVillageInfo;
	NSafeTArray<cltStockBidderManager, MAX_VILLAGE_NUMBER>		clStockBidderManager;

	NSafeTArray<cltStockBidder, MAX_STOCKBIDDER_INVILLAGE>		clClientStockBidder;
#else
	cltStockBidVillageInfo	clStockBidVillageInfo[ MAX_VILLAGE_NUMBER ];		// 증자중인 마을 정보 
	cltStockBidderManager	clStockBidderManager[ MAX_VILLAGE_NUMBER ];			// 입찰자 정보

	cltStockBidder			clClientStockBidder[ MAX_STOCKBIDDER_INVILLAGE ];
#endif
	BOOL bBidding;

	BOOL UpdateStockBidVillageSwitch;											
	BOOL UpdateStockBidderSwitch;
	BOOL UpdateInvestSwitch;			// 상위 주주 목록
	BOOL UpdateStockOfferingInfoSwitch;

	DWORD tickTemp;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockOfferingInfo, MAX_VILLAGE_NUMBER>		clStockOfferingInfo;
#else
	cltStockOfferingInfo	clStockOfferingInfo[ MAX_VILLAGE_NUMBER ];
#endif


	void InitStockBidVillage();
	void InitStockBidder( SI32 villageUnique );
	void InitClientStockBidder();
	
	//void SetStockBidVillage( SI32 villageUnique, cltDate *pclDate, SI32 siStockAmount, SI32 beginningPrice );	// 경매 마을 설정 
	void SetStockBidVillage( SI32 VillageUnique, SI32 TotalStockAmount, SI32 TottalIncreaseStockAmount, SI32 LeftIncreaseStockAmount, GMONEY CurrentStockPrice, cltDate *pclDate );	// 증자마을 설정
	void CompletionStockBidVillage( SI32 villageUnique );									// 경매 완료 

	cltStockManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltStockManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);
	// 여각을 신설한다.
	SI32 CreateStock(SI32 siunique);


	// 보유 주식을 표시한다. 
	void SetHaveStock(HWND hDlg);
	// 주문 내용을 리스트 박스에 추가한다. 
	void SetStockOrderList(HWND hDlg);

	void SetComboBuyKind(HWND hDlg);
	// 매도 종목을 콤보 박스에 추가한다. 
	void SetComboSellKind(HWND hDlg);

	// 선택한 주식 종목에 대한 최적 주문을 설정한다.
	void SetMostOrder(cltTradeOrder* pclSellOrder, cltTradeOrder* pclBuyOrder, GMONEY AverageSell , GMONEY AverageBuy );

	void ShowFeeInfo( HWND hDlg );

	void SetBidItem();

	void Action(cltTime* pcltime);


	// 오래된 주문을 삭제하는등 액션을 취한다. 
	void Action(bool bdaychangedswitch);

	BOOL	ShowFeeRate( HWND hDlg, SI32 rate );
	BOOL	ShowFeeRange( HWND hDlg );
	
};


#endif
