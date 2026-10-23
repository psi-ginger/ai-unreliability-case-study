#ifndef _MSGTYPEDAILYQUEST_H_
#define _MSGTYPEDAILYQUEST_H_

#include "..\..\Common\CommonHeader.h"
#include "../../Common/DailyQuest/DailyQuestMgr.h"
#include "../../Common/DailyQuest/DailyQuestObj.h"
#include "../../Common/Item/ItemCommon/cltItem.h"

// DailyQuest 리스트 요청
class cltGameMsgRequest_DailyQuestList
{
public:
	bool bRequest;	
	cltGameMsgRequest_DailyQuestList()
	{		
	}
};

// DailyQuest 리스트 요청 응답
class cltGameMsgResponse_DailyQuestList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CDailyQuestObjData, MAX_AVAILABLE_DAILYQUEST_NUM>		m_DailyQuestObjDataList;
#else
	CDailyQuestObjData m_DailyQuestObjDataList[ MAX_AVAILABLE_DAILYQUEST_NUM ];
#endif
};

class cltGameMsgRequest_DailyQuestSelect
{
public:
	CDailyQuestObjData DailyQuestObjData;
	BOOL	bMoneySelected;

	cltGameMsgRequest_DailyQuestSelect( CDailyQuestObjData *pDailyQuestObjData, BOOL moneySelected )
	{
		memcpy( &DailyQuestObjData, pDailyQuestObjData, sizeof( CDailyQuestObjData ) );
		bMoneySelected = moneySelected;
	}	
};

class cltGameMsgResponse_DailyQuestSelect
{
public:
	cltPDailyQuestInfo	DailyQuestInfo;
	GMONEY				siLeftMoney;

	cltGameMsgResponse_DailyQuestSelect( cltPDailyQuestInfo *pDailyQuestInfo, GMONEY LeftMoney )
	{
		memcpy( &DailyQuestInfo, pDailyQuestInfo, sizeof( cltPDailyQuestInfo ) );
		siLeftMoney = LeftMoney;
	}
};

class cltGameMsgRequest_DailyQuestSituation
{
public:
	bool bRequest;
	cltGameMsgRequest_DailyQuestSituation()
	{
	}
};

class cltGameMsgResponse_DailyQuestSituation
{
public:
	cltPDailyQuestInfo DailyQuestInfo;

	cltGameMsgResponse_DailyQuestSituation( cltPDailyQuestInfo *pDailyQuestInfo )
	{
		memcpy( &DailyQuestInfo, pDailyQuestInfo, sizeof( cltPDailyQuestInfo ) );
	}
};

class cltGameMsgResponse_DailyQuestChange
{
public:
	SI32 siLeftTime;
	SI16 siKillMonsterNum;

	cltGameMsgResponse_DailyQuestChange( SI32 LeftTime, SI16 KillMonsterNum )
	{
		siLeftTime = LeftTime;
		siKillMonsterNum = KillMonsterNum;
	}
};

class cltGameMsgRequest_DailyQuestEnd
{
public:
	bool bSuccess;

	cltGameMsgRequest_DailyQuestEnd()
	{
	}
	
};

class cltGameMsgResponse_DailyQuestEnd
{
public:
	UI08 bSuccess;
	SI64 siCurExp;// SI32->SI64 경험치 오버플로우때문에 수정
	
	SI16 siInventoryItemIndex;
	SI16 siChangedItemNum;
	cltItem clInvItem;
	GMONEY siAddMoney;
	SI32 siChangedExp;

	SI32 siAddGP;
	cltGameMsgResponse_DailyQuestEnd( UI08 Success, SI64 CurExp, SI16 InventoryItemIndex, SI16 ChangedItemNum, cltItem *pclItem, GMONEY AddMoney, SI32 ChangedExp , SI32 AddGP )// SI32->SI64 경험치 오버플로우때문에 수정
	{
		bSuccess = Success;
		siCurExp = CurExp;// SI32->SI64 경험치 오버플로우때문에 수정

		siInventoryItemIndex = InventoryItemIndex;
		siChangedItemNum = ChangedItemNum;
		clInvItem.Set( pclItem );
		siChangedExp = ChangedExp;
		siAddMoney = AddMoney;
		siAddGP = AddGP;
	}	
};

#endif