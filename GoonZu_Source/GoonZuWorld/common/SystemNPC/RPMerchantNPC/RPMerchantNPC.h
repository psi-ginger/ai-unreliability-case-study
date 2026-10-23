#ifndef _RPMERCHANTNPC_H_
#define _RPMERCHANTNPC_H_

#include <Directives.h>
#include "CommonLogic.h"


#define	MAX_RECOMMAND_PRODUCT 16

struct stRecommandProduct
{
	SI32 ItemUnique;
	SI32 ItemNum;
	SI32 ItemUseDate;
	SI32 NeedPoint;

	stRecommandProduct()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(stRecommandProduct));
	}

	void Set( stRecommandProduct * src )
	{
		memcpy(this,src, sizeof(stRecommandProduct));
	}
	bool IsSame( stRecommandProduct * src )
	{
		if( ItemUnique == src->ItemUnique && ItemNum == src->ItemNum 
			&& ItemUseDate == src->ItemUseDate && NeedPoint == src->NeedPoint)
		{
			return true;
		}
		
		return false;
	}

};

#define MAX_RPLISTDATA_PER_PAGE		10


struct stRPListData
{
	TCHAR szCharName[MAX_PLAYER_NAME];
	SI16  siLevel;
	
	stRPListData()
	{
		Init();
	}


	void Init()
	{
		memset(this, 0, sizeof(stRPListData));
	}

};

class CRPMerchantNPCMgr
{
public:
	
	CRPMerchantNPCMgr();
	~CRPMerchantNPCMgr();

	void Init();	
	
	bool LoadRecommandProductInfo();

#ifdef _SAFE_MEMORY
	NSafeTArray<stRecommandProduct, MAX_RECOMMAND_PRODUCT>		m_RecommandProduct;
#else
	stRecommandProduct	m_RecommandProduct[MAX_RECOMMAND_PRODUCT];
#endif

private:	

};

#endif
