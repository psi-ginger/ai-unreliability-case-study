#ifndef _WORLDMONEYMGR_H
#define _WORLDMONEYMGR_H

#include <Directives.h>

#include "../../../NLib/NArray.h"
#include <Stdio.h>

#define MAX_WORLDMONEY_TRADELIST_NUM		500
#define MAX_WORLDMONEY_TRADELIST_PER_SEND	50

class cltWorldOrder
{
public:
	SI64 siPrice; // 주문의 가격
	SI32 siIndex; // 주문의 고유값

	SI16 siUnique;
	SI16 siItemNum;

	SI16 siType; // 속성 종류
	SI16 siPower; // 속성석 힘

	SI16 siRank; // 주문품 등급

	SI08 siRareType; // 주문품의 레어도(명품,호품 등)

	char szData[24] ;

public:
	cltWorldOrder()
	{
		siPrice = 0 ; // 주문의 가격
		siIndex = 0 ; // 주문의 고유값

		siUnique = 0 ;
		siItemNum = 0 ;

		siType = 0 ; // 속성 종류
		siPower = 0 ; // 속성석 힘

		siRank = 0 ; // 주문품 등급

		siRareType = 0 ; // 주문품의 레어도(명품,호품 등)

		ZeroMemory(szData,sizeof(char) * 24);
	}

	void Set(cltWorldOrder* pclinfo)
	{
		siPrice = pclinfo->siPrice ;
		siIndex = pclinfo->siIndex ;

		siUnique = pclinfo->siUnique ;
		siItemNum = pclinfo->siItemNum ;

		siType = pclinfo->siType ;
		siPower = pclinfo->siPower ;

		siRank = pclinfo->siRank ;

		siRareType = pclinfo->siRareType ;

		if ( pclinfo->szData[0] != NULL )
			memcpy(szData,pclinfo->szData,24);
		else ZeroMemory(szData,sizeof(char) * 24);

	}

	//void Set(SI64 price , SI32 index , SI16 unique , SI16 num , SI16 type , SI16 power , SI08 rank , SI08 rare )
	void Set(SI64 price , SI32 index , SI16 unique , SI16 num , SI16 type , SI16 power , SI16 rank , SI08 rare ,char*data)
	{
		siPrice = price ;
		siIndex = index ;

		siUnique = unique ;
		siItemNum = num ;

		siType = type ;
		siPower = power ;

		siRank = rank ;

		siRareType = rare ;

		if ( data )
			memcpy(szData,data,24);
		else ZeroMemory(szData,sizeof(char) * 24);

	}
};

class cltWorldMoneyTradeList
{
public:
	SI64		siWorldMoney;
	SI64		siPrice;
	SI32		siPersonID;

	SI32		siDate; // 올린 시간

public:
	void Set(cltWorldMoneyTradeList* pclinfo)
	{
		siWorldMoney = pclinfo->siWorldMoney ;
		siPrice = pclinfo->siPrice ;
		siPersonID = pclinfo->siPersonID ;
		siDate = pclinfo->siDate ;
	}

	//void Set(SI32 worldmoney,SI32 price,SI32 personid)
	void Set(SI64 worldmoney,SI64 price,SI32 personid,SI32 date)
	{
		siWorldMoney = worldmoney ;
		siPrice = price ;
		siPersonID = personid ;
		siDate = date ;
	}
};

class cltWorldMoneyAccount{
public:
	SI64			siWorldMoney;			// 소유중인 아큐

	SI64			siTradeWorldMoney;		// 거래소에 올린 아큐
	SI64			siTradeWorldMoneyPrice;	// 거래소에 올린 아큐의 가격

public:
	void Set(cltWorldMoneyAccount * pclinfo)
	{
		siWorldMoney			= pclinfo->GetWorldMoney() ;

		siTradeWorldMoney		= pclinfo->siTradeWorldMoney ;
		siTradeWorldMoneyPrice	= pclinfo->siTradeWorldMoneyPrice ;
	}

	SI64 GetWorldMoney()
	{
		return siWorldMoney ;
	}
	void SetWorldMoney(SI64 money)
	{
		siWorldMoney = money ;
	}
	void IncreaseWorldMoney(SI64 money)
	{
		siWorldMoney += money ;
	}
	void DecreaseWorldMoney(SI64 money)
	{
		siWorldMoney -= money ;
	}

};

// 아큐 공모 클래스
class cltDistributionWorldMoney{
private:
	BOOL			m_bActive; // 현재 진행중인가

	DWORD			m_dwStartClock; // 시작 시간
	DWORD			m_dwEndClock; // 끝나야할 시간

	SI64			m_siWorldMoney; // 현재 공모중인 아큐의 양
	SI64			m_siWorldMoneyPrice; // 현재 공모중인 아큐의 가격
	SI64			m_siTotalWorldMoney; // 현재 공모중인 아큐의 총양

	WORD			m_wOldHour;
	SI64			m_siLimitLowPrice;
	SI64			m_siLimitHighPrice;

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltWorldMoneyTradeList, MAX_WORLDMONEY_TRADELIST_NUM>	clTradeList;
#else
	cltWorldMoneyTradeList	clTradeList[MAX_WORLDMONEY_TRADELIST_NUM];
#endif
	SI64					siAverageWorldMoneyPrice; // 아큐 시세

	cltDistributionWorldMoney();
	~cltDistributionWorldMoney();

	//void			Set(cltWorldMoneyTradeList*pclinfo)
	void Set(cltWorldMoneyTradeList*pclinfo)
	{
		for ( SI32 i = 0 ; i < MAX_WORLDMONEY_TRADELIST_NUM ; i ++ )
		{
			clTradeList[i].Set(&pclinfo[i]);
		}
	}

	void Set(SI32 index , SI32 worldmoney,SI32 price,SI32 personid,SI32 date)
	{
		if ( index < 0 || index >= MAX_WORLDMONEY_TRADELIST_NUM )
			return ;

		clTradeList[index].Set(worldmoney,price,personid,date);
	}

	BOOL			IsActive();
	DWORD			GetStartClock();
	DWORD			GetEndClockTime();

	BOOL			IsEndDistribution(); // 공모가 끝났는가?
	void			Set(BOOL acive,SI64 worldmoney,SI64 price,SI64 total,DWORD start,DWORD end); // 설정한다
	BOOL			IsCorrectDistribution(SI64 worldmoney,SI64 price);

	SI64			SetAverageWorldMoney(SI64 worldmoney,SI64 price) ; // 거래된 아큐와 가격으로 아큐 시세를 구한다.

	void			Action(bool bdaychangedswitch);

};


#endif
