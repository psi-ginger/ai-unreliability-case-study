#include "FarmingMgr.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

#include "../Char/CharCommon/Char-Common.h"

#include "../Order/order.h"

#include "../../CommonLogic/Msg/MsgType-Farming.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "../../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Fishing.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

CFarmingMgr::CFarmingMgr()
{
	m_siFarmingItemNum = 0;
	m_pItemUniqueList =  NULL;

	pclClient->pclItemManager->GetFarmItemList( &m_siFarmingItemNum, &m_pItemUniqueList );
}

CFarmingMgr::~CFarmingMgr()
{
	if ( m_pItemUniqueList )
	{
		delete [] m_pItemUniqueList;
		m_pItemUniqueList = NULL;
	}

	m_siFarmingItemNum = 0;
}

bool CFarmingMgr::IsAvailableFarmingItem( SI32 ItemUnique )
{
	for( SI16 i = 0; i < m_siFarmingItemNum; ++i )
	{
		if ( m_pItemUniqueList[ i ] == ItemUnique )
		{
			return true;
		}
	}

	return false;
}