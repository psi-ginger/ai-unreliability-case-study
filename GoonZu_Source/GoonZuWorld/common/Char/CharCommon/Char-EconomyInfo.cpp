#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"

#include "..\Common\Bullet\Bullet.h"
#include "Util\Util.h"

extern cltCommonLogic* pclClient;

//-------------------------------------------
// cltPStockInfo 
//-------------------------------------------
void cltPStockInfo::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	SI32 i = 0;
	pkGMSectionData->BeginData( "StockInfo" );
	{
		pkGMSectionData->BeginData( "StockAccount" );
		{
			for(i=0; i<MAX_STOCK_KIND_IN_ACOUNT; i++)
			{
				pkGMSectionData->AddDataInt("village",	clInfo.clUnit[i].siVillageUnique);
				pkGMSectionData->AddDataInt("amount",	clInfo.clUnit[i].siAmount);
			}
		}
		pkGMSectionData->EndData( "StockAccount" );
		
		pkGMSectionData->BeginData( "StockOrderInfo" );
		{
			for(i=0; i<MAX_MARKET_ORDER_PER_PERSON; i++)
			{
				pkGMSectionData->AddDataInt("mode",		clOrderInfo.clOrderUnit[i].siOrderMode);
				pkGMSectionData->AddDataInt("price",	clOrderInfo.clOrderUnit[i].clOrder.siPrice);
				pkGMSectionData->AddDataInt("amount",	clOrderInfo.clOrderUnit[i].clOrder.siAmount);
			}
		}
		pkGMSectionData->EndData( "StockOrderInfo" );
	}
	pkGMSectionData->EndData( "StockInfo" );
}

//-------------------------------------------
// 전장 관련. 
//-------------------------------------------

// 전장의 잔고를 확인해서 알려준다. 
GMONEY cltCharCommon::GetBankMoney()const
{
	return pclCI->clBank.GetMoney();

}

//------------------------------------------
// 주식 관련. 
//------------------------------------------

// 주식 주문 개수를 확인한다. 
SI32 cltCharCommon::GetStockOrderNumber()const
{
	return pclCI->clStock.clOrderInfo.GetOrderNumber();
}

//------------------------------------------
// 객주 관련. 
//------------------------------------------

// 물품 개수를 확인한다. 
SI32 cltCharCommon::GetMarketOrderNumber()const
{
	SI32 num = 0;
	SI32 i;

	for(i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
	{
		// 유니크가 있따면, 
		if(pclCI->clMarket.clInfo.clItem[i].siUnique)
		{
			num++;
		}
	}

	return num;
}

//[진성] 통합마켓.
SI32 cltCharCommon::GetNewMarketOrderNumber()	const
{
	SI32 num = 0;
	for( SI32 i = 0; i < MAX_NEWMARKET_ITEMLIST; i++ )
	{
		// 유니크가 있따면, 
		/*if( pclCI->clNewMarket.clNewMarketTradeOrder[i].m_clItem.siUnique )
		{
			num++;
		}*/
	}

	return num;
}



//-----------------------------------
// Person의 시전 관련 정보 
//-----------------------------------

bool cltCharRealEstate::Add(SI32 type, SI32 sivillageunique, SI32 index, cltRentContract*  pclcontract)
{
	SI32 i;


	// 중복된 것이 있는지 확인한다. 
	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siType			==  type 
		&& clRealEstate[i].siVillageUnique	==  sivillageunique
		&& clRealEstate[i].siIndex			==	index)
		{
			clRealEstate[i].Set(type, sivillageunique, index, pclcontract);
			return true;
		}

	}

	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siType == 0)
		{
			clRealEstate[i].Set(type, sivillageunique, index, pclcontract);
			return true;
		}
	}

	return false;
}

// 부동산을 제거한다. 
bool cltCharRealEstate::Delete(SI32 type, SI32 sivillageunique, SI32 index)
{
	SI32 i;

	// 중복된 것이 있는지 확인한다. 
	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siType			==  type 
		&& clRealEstate[i].siVillageUnique	==  sivillageunique
		&& clRealEstate[i].siIndex			==	index)
		{
			clRealEstate[i].Init();
			return true;
		}

	}

	return false;
}

// 빈 자리를 찾는다. 
SI32 cltCharRealEstate::FindEmptyIndex()
{
	SI32 i;

	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siType == 0)
		{
			return i;
		}
	}

	return -1;
}

// 소유하고 있는 특정 부동산을 찾아낸다.
cltRealEstateUnit*  cltCharRealEstate::Find(SI32 villageunique, SI32 ranktype, SI32 index)
{
	SI32 i;

	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siVillageUnique	== villageunique
		&& clRealEstate[i].siType			== ranktype
		&& clRealEstate[i].siIndex			== index)
		{
			return &clRealEstate[i];
		}
	}

	return NULL;

}

// 이미 해당 물건이 객주에 올려져 있는가 ?
bool cltPMarketInfo::IsInMarketOrder(SI32 siunique)
{
	if(siunique <= 0)	return false;

	if( pclClient->pclItemManager->GetItemAtb(siunique) & ITEMATB_OBJECT)
	{
		return false;

	}
	else
	{
		for( SI32 i = 0;i < MAX_MARKET_ORDER_PER_PERSON;i++)
		{
			// 같은 유니크가 존재한다면, 
			if(clInfo.clItem[i].siUnique == siunique)return true;
		}
	}
	
	return false;
}

//[진성] 이미 해당 물건이 통합마켓에 올려져 있는가 ?
bool cltNewMarketInfo::IsInNewMarketOrder( SI32 _siUnique )
{
	if( _siUnique <= 0 )	return false;

	if( pclClient->pclItemManager->GetItemAtb(_siUnique) & ITEMATB_OBJECT) 
		return false;
	else
	{
		for( SI32 i = 0; i < MAX_PERSON_NEWMARKET_ORDER_NUM; i++ )
		{
			// 같은 유니크가 존재한다면, 
			//if(clNewMarketTradeOrder[i].m_clItem.siUnique == _siUnique)	return true;
		}
	}

	return false;
}

/*cyj
//======================================
// cltPGuildStockInfo
//======================================
cltPGuildStockInfo::cltPGuildStockInfo()
{
}

void cltPGuildStockInfo::Init()
{
	ZeroMemory(this, sizeof(cltPGuildStockInfo));

	clInfo.Init();
	clOrderInfo.Init();
}

void cltPGuildStockInfo::Set(cltPGuildStockInfo* pinfo)
{
	memcpy(this, pinfo, sizeof(cltPGuildStockInfo));						//not used
}

//======================================
// cltPStockInfo
//======================================
cltPStockInfo::cltPStockInfo()
{
}

void cltPStockInfo::Init()
{
	ZeroMemory(this, sizeof(cltPStockInfo));

	clInfo.Init();
	clOrderInfo.Init();
}

void cltPStockInfo::Set(cltPStockInfo* pinfo)
{
	memcpy(this, pinfo, sizeof(cltPStockInfo));					//not used
}

//======================================
// cltPMarketInfo
//======================================
cltPMarketInfo::cltPMarketInfo()
{
}

void cltPMarketInfo::Init()
{
	ZeroMemory(this, sizeof(cltPMarketInfo));

	clInfo.Init();
}

void cltPMarketInfo::Set(cltPMarketInfo* pinfo)
{
	memcpy(this, pinfo, sizeof(cltPMarketInfo));					//not used
}

//======================================
// cltRealEstateUnit
//======================================
cltRealEstateUnit::cltRealEstateUnit()
{
	siType			= 0;
	siVillageUnique = 0;
	siIndex			= 0;
}

cltRealEstateUnit::cltRealEstateUnit(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo)
{
	siType			= type;
	siVillageUnique = villageunique;
	siIndex			= index;

	clContract.Set(pclinfo);
}

void cltRealEstateUnit::Set(const cltRealEstateUnit* pclinfo)
{
	siType			= pclinfo->siType;
	siVillageUnique = pclinfo->siVillageUnique;
	siIndex			= pclinfo->siIndex;

	clContract.Set(&pclinfo->clContract);
}

void cltRealEstateUnit::Set(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo)
{
	siType			= type;
	siVillageUnique = villageunique;
	siIndex			= index;

	clContract.Set(pclinfo);
}

void cltRealEstateUnit::Init()
{
	siType			= 0;
	siVillageUnique = 0;
	siIndex			= 0;

	clContract.Init();
}

//======================================
// cltCharRealEstate
//======================================
cltCharRealEstate::cltCharRealEstate()
{}

void cltCharRealEstate::Set(cltCharRealEstate* pclinfo)
{
	SI32 i;
	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		clRealEstate[i].Set(&pclinfo->clRealEstate[i]);
	}
}

SI32 cltCharRealEstate::GetRealEstateNum()
{
	SI32 i;
	SI32 count = 0;
	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		if(clRealEstate[i].siType)count++;
	}

	return count;

}
*/
