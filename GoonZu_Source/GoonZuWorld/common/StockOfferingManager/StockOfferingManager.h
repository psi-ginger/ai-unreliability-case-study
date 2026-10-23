#ifndef _STOCKOFFERINGMANAGER_H
#define _STOCKOFFERINGMANAGER_H

#include "../../CommonLogic/CommonLogic.h"

class cltStockOfferingInfo
{
public:
	SI32				siVillageUnique;
	SI32				siTotalOffering;		// 총 공모 주식 수량
	SI32				siStockAmount;			// 1인당 배당 주식 수
	GMONEY				siPrice;				// 1인당 배당 주식 가격
	SI32				siStatus;

};

class cltStockOfferingOrderInfo
{
public:
	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siStockAmount;
	GMONEY				siMoney;

	TCHAR				szIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];

	void Set(cltStockOfferingOrderInfo* pclInfo)
	{
		memcpy( this, pclInfo, sizeof(*this) );
	}
};

class cltStockOfferingOrderDuplicate
{
public:
	TCHAR				szPublicIP[20];
	TCHAR				szPrivateIP[20];
	TCHAR				szMacAddress[20];
	SI32				siDuplicateCount;

	void Init()
	{
		ZeroMemory(this, sizeof(cltStockOfferingOrderDuplicate));
	}
};

class cltStockOfferingManager
{
public:
	cltStockOfferingManager();
	~cltStockOfferingManager();

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockOfferingInfo, MAX_VILLAGE_NUMBER>		clStockInfo;
#else
	cltStockOfferingInfo		clStockInfo[ MAX_VILLAGE_NUMBER ];
#endif

	BOOL						bLottery;
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockOfferingOrderInfo, 2000>		clStockOrder;
#else
	cltStockOfferingOrderInfo	clStockOrder[ 2000 ];
#endif
	SI32						siCount;

	SI32						siVillageUnique;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltStockOfferingOrderDuplicate, 2000>		clStockOrderDuplicate;
#else
	cltStockOfferingOrderDuplicate	clStockOrderDuplicate[ 2000 ];
#endif
	SI32							siDuplicateCount;
	SI32							siAllowDuplicateCount;

	bool CheckStockOfferingOrderDuplicate(TCHAR* pszPublicIP, TCHAR* pszPrivateIP, TCHAR* pszMacAddress);
};






#endif