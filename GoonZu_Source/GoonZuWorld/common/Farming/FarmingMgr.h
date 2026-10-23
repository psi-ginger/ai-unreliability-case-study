#ifndef _FARMINGMGR_H_
#define _FARMINGMGR_H_

#include "FarmingBaseInfo.h"

class CFarmingMgr
{
public:
	CFarmingMgr();
	~CFarmingMgr();

	bool	IsAvailableFarmingItem( SI32 ItemUnique );

private:
	SI32	m_siFarmingItemNum;
	SI32	*m_pItemUniqueList;
};

#endif