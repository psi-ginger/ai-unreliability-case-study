#ifndef _MSGTYPEFISHING_H_
#define _MSGTYPEFISHING_H_

#include "..\..\Common\CommonHeader.h"
#include "../../Common/Fishing/FishingMgr.h"
#include "../../Common/Fishing/FishKind.h"
#include "../../Common/Item/ItemCommon/cltItem.h"

#include "../../../DBManager/GameDBManager_World/dbMsg.h"

class cltGameMsgResponse_FishingStart
{
public:
	SI32 siFishingBaitItemUnique;
#ifdef _SAFE_MEMORY
	NSafeTArray<DBToolInfo, MAX_FISHING_TOOL_NUMBER>		clFishingToolList;
#else
	DBToolInfo clFishingToolList[ MAX_FISHING_TOOL_NUMBER ];
#endif

	cltGameMsgResponse_FishingStart( SI32 FishingBaitItemUnique, DBToolInfo *pclFishingToolList )
	{
		siFishingBaitItemUnique = FishingBaitItemUnique;
	
		for( SI32 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
		{
			if ( pclFishingToolList[i].clItem.siUnique > 0 )
			{
				clFishingToolList[i].Set(&pclFishingToolList[i]);
			}
		}
	}	
};

// 도구 소모
class cltGameMsgResponse_FishingDecreaseToolDurability
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<DBToolInfo, MAX_FISHING_TOOL_NUMBER>		clFishingToolInfo;
#else
	DBToolInfo clFishingToolInfo[ MAX_FISHING_TOOL_NUMBER ];
#endif

	cltGameMsgResponse_FishingDecreaseToolDurability( DBToolInfo *pclFishingToolInfo )
	{
#ifdef _SAFE_MEMORY
		clFishingToolInfo.MemCpy(pclFishingToolInfo);
#else
		memcpy( clFishingToolInfo, pclFishingToolInfo, sizeof ( DBToolInfo ) * MAX_FISHING_TOOL_NUMBER );
#endif
	}
};

// 물고기 낚거나 실패, 물고기 낚았을 시에는 스킬 경험치 올려줌.
class cltGameMsgResponse_FishingGetMarineProducts
{
public:
	SI16 siReason;

	SI16 siFishItemUnique;

	SI16 siInvPos;
	cltItem clItem;
	
	SI32 siKind;					// 누가 낚았는지에 대한 정보를 가지고 있음.

	cltGameMsgResponse_FishingGetMarineProducts( 
		SI16 Reason, 
		SI16 FishItemUnique,
		SI16 InvPos, cltItem *pclItem, 
		SI32 Kind )
	{
		siReason = Reason;

		siFishItemUnique = FishItemUnique;

		siInvPos = InvPos;
		clItem.Set( pclItem );

		siKind = Kind;
	}
};

class cltGameMsgResponse_FishingGetMarineProductsNotify
{
public:
	SI16 siReason;

	SI16 siFishItemUnique;
	SI16 siFishNum;

	SI32 siCharUnique;
	SI32 siKind;

	cltGameMsgResponse_FishingGetMarineProductsNotify( SI16 Reason, SI16 FishItemUniue, SI16 FishNum, SI32 CharUnique, SI32 Kind )
	{
		siReason = Reason;

		siFishItemUnique = FishItemUniue;
		siFishNum = FishNum;

		siCharUnique = CharUnique;
		siKind = Kind;
	}
};



#endif