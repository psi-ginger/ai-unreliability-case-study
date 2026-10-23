//---------------------------------
// 2003/6/2 김태곤
//---------------------------------

#ifndef _ECONOMYINFO_H
#define _ECONOMYINFO_H

#include "..\CommonLogic\Stock\Stock.h"
#include "..\CommonLogic\Market\Market.h"
//#include "..\CommonLogic\House\House.h"



#define REALESTATE_TYPE_HOUSE				1
#define REALESTATE_TYPE_LAND				2
#define REALESTATE_TYPE_MINE				3

#define MAX_REAL_ESTATE_NUMBER_PER_PERSON	10

//---------------------------------------------
// Person의 경제 관련 정보중 증권관련 정보.
//---------------------------------------------
/*
//_LEON_GUILD_STOCK
class cltPGuildStockInfo{
public:
	cltGuildStockAccount	clInfo;
	//cltCharOrderInfo		clOrderInfo;
	cltGuildCharOrderInfo	clOrderInfo;

	/*cyj
	cltPGuildStockInfo();

	void Init();
	void Set(cltPGuildStockInfo* pinfo);
	*/
/*
	cltPGuildStockInfo()
	{
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltPGuildStockInfo));

		clInfo.Init();
		clOrderInfo.Init();
	}

	void Set(cltPGuildStockInfo* pinfo)
	{
		memcpy(this, pinfo, sizeof(cltPGuildStockInfo));					//ok
	}


};*/


class cltPStockInfo{
public:
	cltStockAccount		clInfo;
	cltCharOrderInfo	clOrderInfo;

	/*cyj
	cltPStockInfo();

	void Init();
	void Set(cltPStockInfo* pinfo);
	*/

	cltPStockInfo()
	{
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltPStockInfo));

		clInfo.Init();
		clOrderInfo.Init();
	}

	void Set(cltPStockInfo* pinfo)
	{
		memcpy(this, pinfo, sizeof(cltPStockInfo));							//ok
	}

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};

//-------------------------------------
// Person의 객주 관련 정보. 
//-------------------------------------
class cltPMarketInfo{
public:
	cltMarketAccount	clInfo;

	/*cyj
	cltPMarketInfo();

	void Init();
	void Set(cltPMarketInfo* pinfo);
	
	
	*/

	cltPMarketInfo()
	{
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltPMarketInfo));

		clInfo.Init();
	}

	void Set(cltPMarketInfo* pinfo)
	{
		memcpy(this, pinfo, sizeof(cltPMarketInfo));								//ok
	}

	bool IsInMarketOrder(SI32 siunique);		// 이미 해당 물건이 객주에 올려져 있는가 ?

};

//[진성] 통합마켓에 내가 올린 아이템.
class cltNewMarketInfo
{
public:
	cltNewMarketAccount		clInfo;

	cltNewMarketInfo()
	{
	}

	void Init()
	{
		ZeroMemory( this, sizeof(*this) );
	}

	void Set(cltNewMarketInfo* pinfo)
	{
		memcpy( this, pinfo, sizeof(*this) );								//ok
	}

	bool IsInNewMarketOrder( SI32 _siUnique );		// 이미 해당 물건이 객주에 올려져 있는가 ?
};


//-----------------------------------
// Person의 시전 관련 정보 
//-----------------------------------
class cltRealEstateUnit{
public:
	SI32 siType;
	SI32 siVillageUnique;
	SI32 siIndex;

	// 해당 부동산의 계약 조건. 
	cltRentContract clContract;
	
	/*cyj
	cltRealEstateUnit();
	cltRealEstateUnit(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo);

	void Set(const cltRealEstateUnit* pclinfo);
	void Set(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo);
	void Init();
	*/

	cltRealEstateUnit()
	{
		siType			= 0;
		siVillageUnique = 0;
		siIndex			= 0;
	}

	cltRealEstateUnit(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo)
	{
		siType			= type;
		siVillageUnique = villageunique;
		siIndex			= index;

		clContract.Set(pclinfo);
	}

	void Set(const cltRealEstateUnit* pclinfo)
	{
		siType			= pclinfo->siType;
		siVillageUnique = pclinfo->siVillageUnique;
		siIndex			= pclinfo->siIndex;

		clContract.Set(&pclinfo->clContract);
	}

	void Set(SI32 type, SI32 villageunique, SI32 index, cltRentContract* pclinfo)
	{
		siType			= type;
		siVillageUnique = villageunique;
		siIndex			= index;

		clContract.Set(pclinfo);
	}

	void Init()
	{
		siType			= 0;
		siVillageUnique = 0;
		siIndex			= 0;

		clContract.Init();
	}
};

class cltCharRealEstate{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltRealEstateUnit, MAX_REAL_ESTATE_NUMBER_PER_PERSON>	clRealEstate;
#else
	cltRealEstateUnit clRealEstate[MAX_REAL_ESTATE_NUMBER_PER_PERSON];
#endif

	/*cyj
	cltCharRealEstate();

	void Set(cltCharRealEstate* pclinfo);
	SI32 GetRealEstateNum();		// 소유하고 있는 부동산의 개수를 리턴한다. 
	*/

	cltCharRealEstate()
	{}

	void Set(cltCharRealEstate* pclinfo)
	{
		SI32 i;
		for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
		{
			clRealEstate[i].Set(&pclinfo->clRealEstate[i]);
		}
	}

	SI32 GetRealEstateNum()
	{
		SI32 i;
		SI32 count = 0;
		for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
		{
			if(clRealEstate[i].siType)count++;
		}

		return count;

	}

	bool Add(SI32 type, SI32 sivillageunique, SI32 index, cltRentContract*  pclcontract );	// 부동산을 추가한다. 
	bool Delete(SI32 type, SI32 sivillageunique, SI32 index);	// 부동산을 제거한다. 

	SI32 FindEmptyIndex();			// 빈 자리를 찾는다. 
	cltRealEstateUnit*  Find(SI32 villageunique, SI32 ranktype, SI32 index);	// 소유하고 있는 특정 부동산을 찾아낸다.

};



#endif