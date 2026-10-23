#ifndef _STOCKDISTRIBUTIONOBJ_H_
#define _STOCKDISTRIBUTIONOBJ_H_

#include "..\SMLib\LibList\Obj.h"
#include "NDataTypes.h"

class CStockDistributionObj : public CObj
{
public:
	CStockDistributionObj( SI32 Index );
	~CStockDistributionObj();

	void Initialize();
	void Destroy();

	SI16 GetCurrentStatus();
	SI32 GetVillageUnique();
	SI32 Set( SI32 VillageUnique, SI32 MaxDistributionStockNum, SI32 DistributionStockUnit, GMONEY DistributionStockPrice, bool VillageWarStockDistribution, SI32 VillageWarWinVillageUnique );
	bool Action();
	SI32 IncreaseDistributionStockNum( SI32 IncreaseDistributionStockNum );
	
	SI32 GetDistributionStockUnit();			// 한명에게 줄수 있는 최대 주식수.
	GMONEY GetDistributionStockPrice();			// 각 주식당 가격

	SI32 GetLeftDistributionStockNum();			// 남은 주식수
	SI32 GetMaxDistributionStockNum();			// 배당 최대 주식수

	bool IsVillageWarStockDistribution();		// 공성에 의한 주식 공모이냐??
private:

	SI32 m_siStockDistributionObjIndex;
	SI16 m_siCurrentStatus;
	
	SI32 m_siVillageUnique;						// 마을

	SI32 m_siMaxDistributionStockNum;			// 최대 유저들에게 주는 주식수
	SI32 m_siDistributionStockUnit;				// 각 유저들에게 줄수 있는 최대 주식수
	GMONEY m_siDistributionStockPrice;			// 각 주식당 가격

	SI32 m_siCurrentDistributionStockNum;		// 현재 분배된 주식수

	SI32 m_siCurrentFrame;

	bool m_bVillageWarStockDistribution;		// 공성에 의한 주식 공모인지 구분 짓기 위한 것.
};

#endif