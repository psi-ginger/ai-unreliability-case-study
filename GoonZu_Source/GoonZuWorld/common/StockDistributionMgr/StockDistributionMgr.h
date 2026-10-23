#ifndef _STOCKDISTRIBUTIONMGR_H_
#define _STOCKDISTRIBUTIONMGR_H_

#include "..\SMLib\LibList\LibListMgr.h"
#include "NDataTypes.h"
class CStockDistributionObj;

class CStockDistributionMgr : public CLibListMgr
{
public:
	CStockDistributionMgr();
	~CStockDistributionMgr();

	void Initialize();
	void Destroy();

	void CreateMaxEmptyObj();

	SI32 Set( SI32 VillageUnique, SI32 MaxDistributionStockNum, SI32 DistributionStockUnit, GMONEY DistributionStockPrice, bool VillageWarStockDistribution, SI32 VillageWarWinVillageUnique );
	void Action();
	

	CStockDistributionObj *GetStockDistributionObj( SI32 VillageUnique, bool VillageWarStockDistribution );

	void IncreaseDistributionStockNum( SI32 VillageUnique, SI32 IncreaseDistributionStockNum );
	
	bool IsAvailable( SI32 VillageUnique );
	
	bool NotVillageWarStockDistributionExist();			// 마을 주식 공모 중인지.. ( 단, 공성에 의한 공모는 빠짐 )

	void SendStockDistributionNoticeMsg( SI32 CurrentFrame, _SYSTEMTIME sTime );

	void SendStockDistributionExplainMsg( SI32 CurrentFrame, _SYSTEMTIME sTime );

private:

};


#endif