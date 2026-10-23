//-------------------------------
// SJY 2005/12/06
//-------------------------------
#include "WorldMoneyMgr.h"

#include "CommonLogic.h"

#include "../../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"

extern cltCommonLogic* pclClient;

#define MAX_VALUE_UP			105
#define MAX_VALUE_DOWN			95
#define MAX_VALUE_PRICE			500

cltDistributionWorldMoney::cltDistributionWorldMoney()
{
	m_bActive = FALSE ; // 현재 진행중인가

	m_dwStartClock = 0 ; // 시작 시간
	m_dwEndClock = 0 ; // 끝나야할 시간

	m_siWorldMoney = 0 ; // 현재 공모중인 아큐의 양
	m_siWorldMoneyPrice = 0 ; // 현재 공모중인 아큐의 가격
	m_siTotalWorldMoney = 0 ; // 현재 공모중인 아큐의 총양

	m_wOldHour = (WORD)-1;
	m_siLimitLowPrice = 0;
	m_siLimitHighPrice = 0;


	for ( SI32 i = 0 ; i < MAX_WORLDMONEY_TRADELIST_NUM ; i ++ )
	{
		clTradeList[i].Set(0,0,0,0);
	}

	siAverageWorldMoneyPrice = 0 ;

	return ;
}

cltDistributionWorldMoney::~cltDistributionWorldMoney()
{
	m_bActive = FALSE ; // 현재 진행중인가

	m_dwStartClock = 0 ; // 시작 시간
	m_dwEndClock = 0 ; // 끝나야할 시간

	m_siWorldMoney = 0 ; // 현재 공모중인 아큐의 양
	m_siWorldMoneyPrice = 0 ; // 현재 공모중인 아큐의 가격
	m_siTotalWorldMoney = 0 ; // 현재 공모중인 아큐의 총양

	m_wOldHour = (WORD)-1;
	m_siLimitLowPrice = 0;
	m_siLimitHighPrice = 0;

	for ( SI32 i = 0 ; i < MAX_WORLDMONEY_TRADELIST_NUM ; i ++ )
	{
		clTradeList[i].Set(0,0,0,0);
	}

	siAverageWorldMoneyPrice = 0 ;
}

BOOL cltDistributionWorldMoney::IsActive()
{
	return m_bActive ;
}

DWORD cltDistributionWorldMoney::GetStartClock()
{
	return m_dwStartClock ;
}

DWORD cltDistributionWorldMoney::GetEndClockTime()
{
	return m_dwEndClock ;
}

BOOL cltDistributionWorldMoney::IsEndDistribution()
{
	if ( !m_bActive )
		return FALSE ;
	else
	{
		if ( m_dwStartClock == 0 || m_dwEndClock == 0 )
		{
			m_bActive = FALSE ;
			return FALSE ;
		}

		if ((DWORD)(TABS(pclClient->CurrentClock - m_dwStartClock)) >=  m_dwEndClock)
		{
			m_bActive = FALSE ;
			m_dwStartClock = 0 ;
			m_dwEndClock = 0 ;
			return FALSE ;
		}
		else
		{
			return TRUE ;
		}
	}

//	return TRUE ;
}

void cltDistributionWorldMoney::Set(BOOL acive,SI64 worldmoney,SI64 price,SI64 total,DWORD start,DWORD end)
{
	m_bActive = acive ;
	m_dwStartClock = start ;
	m_dwEndClock = end ;

	m_siWorldMoney = worldmoney ;
	m_siWorldMoneyPrice = price ;
	m_siTotalWorldMoney = total ;
}

BOOL cltDistributionWorldMoney::IsCorrectDistribution(SI64 worldmoney,SI64 price)
{
	if ( IsEndDistribution() == FALSE )
		return FALSE ;

	if ( m_siWorldMoney != worldmoney )
		return FALSE ;
	if ( m_siWorldMoneyPrice != price )
		return FALSE ;
	if ( m_siTotalWorldMoney < worldmoney )
		return FALSE ;

	m_siTotalWorldMoney -= worldmoney ;

	return TRUE ;
}

// worldmoney가 일종의 itemnum이고 price는 price다
SI64 cltDistributionWorldMoney::SetAverageWorldMoney(SI64 worldmoney,SI64 price)
{
	SI64 averageprice = siAverageWorldMoneyPrice ;

	cltServer *pclserver = (cltServer*)pclClient;

	if (m_wOldHour != pclserver->sTime.wHour)
	{
		m_siLimitLowPrice = averageprice * 80 / 100;
		m_siLimitHighPrice = averageprice * 120 / 100;
		m_wOldHour = pclserver->sTime.wHour;
	}

	if ( worldmoney && price )
	{
		if ( averageprice )
		{
			// 거래량이 10아큐 이하 이면 시세 변동에 반영하지 않음
			if (worldmoney <= 10)
				return averageprice;

			// 거래량이 300아큐 초과이면 시세 변동에 반영하지 않음
			if (worldmoney > 300)
				return averageprice;

			//---------------------------------------------------
			// 가격이 시세의 MAX_VALUE_UP%이상이면 MAX_VALUE_UP%까지만 반영한다. 
			//---------------------------------------------------
			SI64 maxprice = max(1, (averageprice * MAX_VALUE_UP / 100));
			if(maxprice == averageprice)
			{
				maxprice++;
			}

			//---------------------------------------------------
			// 가격이 시세보다 MAX_VALUE_DOWN%이하라면 MAX_VALUE_DOWN%까지만 반영한다.
			//---------------------------------------------------
			SI64 minprice = max(1, (averageprice * MAX_VALUE_DOWN / 100));
			if(minprice == averageprice )
			{
				minprice = max(1, minprice-1);
			}

			// 비싸게 설정되는 것은 보수적으로 제한한다.
			if(price > maxprice)
			{
				price = maxprice;

				// 개수가 너무 많은 것도 제한한다. 
				if(worldmoney > 10)worldmoney = 10;
			}
			// 싸게 설정되는 것은 적극적으로 반영한다. 
			else if(price < minprice)
			{
				price = minprice;

				// 개수가 너무 많은 것도 제한한다. 
				if(worldmoney > 1000)worldmoney = 1000;
			}
		}

		if(averageprice == 0)
		{
			// 최초 가격은 100냥에서 시작한다. 
			if ( worldmoney > 5000 ) worldmoney = 5000 ;

			averageprice = worldmoney ;
		}
		else
		{
			GMONEY orgworldmoney = min(10000, worldmoney);

			// 1시간동안 시세변화는 20%를 넘지 않도록
			GMONEY tmpaverageprice = (( averageprice * orgworldmoney ) + ( price * worldmoney ))/(orgworldmoney+worldmoney);
			//averageprice = (( averageprice * orgworldmoney ) + ( price * worldmoney ))/(orgworldmoney+worldmoney);

			if (tmpaverageprice > m_siLimitHighPrice)
			{
				averageprice = m_siLimitHighPrice;
			}
			else if (tmpaverageprice < m_siLimitLowPrice)
			{
				averageprice = m_siLimitLowPrice;
			}
			else
			{
				averageprice = tmpaverageprice;
			}
		}

		// ACU시세 최저가는 100
		averageprice = max(averageprice, 100);
	}

	return averageprice ;
}

void cltDistributionWorldMoney::Action(bool bdaychangedswitch)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	// DB 초기화 완료될때까지 기다림
	if( pclserver->bCompleteDBInitAll == FALSE ) return ;

	if ( bdaychangedswitch )
	{
		SI32 curdate = pclserver->pclTime->clDate.GetDateVary() ;

		for ( SI32 i = 0 ; i < MAX_WORLDMONEY_TRADELIST_NUM ; i ++ )
		{
			// 100냥 이하로는 안 떨군다.
			if ( clTradeList[i].siPrice <= 100 )
				continue;
			if ( clTradeList[i].siPersonID <= 0 )
				continue;

			// 30일에 한번 깎는다.
			if ( clTradeList[i].siDate + 30 <= curdate )
			{
				SI64 price = ( clTradeList[i].siPrice * 90 ) / 100 ; // 90%로 만든다.
				if ( price <= 100 )
					price = 100 ;

				sDBRequest_WorldMoney_ReducePrice clMsg(i,price,curdate,clTradeList[i].siPersonID,clTradeList[i].siWorldMoney);
				pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}
}