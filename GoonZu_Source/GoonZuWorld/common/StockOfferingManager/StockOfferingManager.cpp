#include "StockOfferingManager.h"


cltStockOfferingManager::cltStockOfferingManager()
{
#ifdef _SAFE_MEMORY
	clStockInfo.ZeroMem();
	clStockOrder.ZeroMem();
#else
	ZeroMemory( clStockInfo, sizeof( clStockInfo ) );
	ZeroMemory( clStockOrder, sizeof( clStockOrder) );
#endif

	bLottery = FALSE;
	siCount = 0;
	siDuplicateCount = 0;

	siVillageUnique = 0;

	// 기본적인 겹치기 허용
	siAllowDuplicateCount	= 2;

	// 나라별 설정 - 대만
	if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
	{
		siAllowDuplicateCount	= 1;
	}
	// 나라별 설정 - 일본
	if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
	{
		siAllowDuplicateCount	= 1;
	}
}

cltStockOfferingManager::~cltStockOfferingManager()
{

}

bool cltStockOfferingManager::CheckStockOfferingOrderDuplicate(TCHAR* pszPublicIP, TCHAR* pszPrivateIP, TCHAR* pszMacAddress)
{
	if( pszPublicIP == NULL || pszPrivateIP == NULL || pszMacAddress == NULL )					return false;
	if( _tcslen(pszPublicIP) <= 0 || _tcslen(pszPrivateIP) <= 0 || _tcslen(pszMacAddress) <= 0)	return false;

	for(int i=0; i<siDuplicateCount; i++)
	{
		if( _tcscmp(clStockOrderDuplicate[i].szPublicIP,	pszPublicIP)	== 0 &&
			_tcscmp(clStockOrderDuplicate[i].szPrivateIP,	pszPrivateIP)	== 0 &&
			_tcscmp(clStockOrderDuplicate[i].szMacAddress,	pszMacAddress)	== 0 )
		{
			clStockOrderDuplicate[i].siDuplicateCount++;

			if(clStockOrderDuplicate[i].siDuplicateCount < siAllowDuplicateCount)
				return false;

			return true;
		}
	}

	StringCchCopy(clStockOrderDuplicate[siDuplicateCount].szPublicIP,	sizeof(clStockOrderDuplicate[siDuplicateCount].szPublicIP),		pszPublicIP);
	StringCchCopy(clStockOrderDuplicate[siDuplicateCount].szPrivateIP,	sizeof(clStockOrderDuplicate[siDuplicateCount].szPrivateIP),		pszPrivateIP);
	StringCchCopy(clStockOrderDuplicate[siDuplicateCount].szMacAddress, sizeof(clStockOrderDuplicate[siDuplicateCount].szMacAddress),	pszMacAddress);
	clStockOrderDuplicate[i].siDuplicateCount = 0;

	siDuplicateCount++;

	return false;
}
