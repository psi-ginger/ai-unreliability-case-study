#ifndef _MSGTYPEDAILYQUEST2_H_
#define _MSGTYPEDAILYQUEST2_H_

#include "..\..\Common\CommonHeader.h"
#include "../../Common/DailyQuest2/DailyQuest2Mgr.h"
#include "../../Common/DailyQuest2/DailyQuest2Obj.h"
#include "../../Common/Item/ItemCommon/cltItem.h"

#define GuildPoint_Rate	130
// DailyQuest 리스트 요청
class cltGameMsgRequest_DailyQuest2List
{
public:
	bool bRequest;	
	cltGameMsgRequest_DailyQuest2List()
	{		
	}
};

// DailyQuest 리스트 요청 응답
class cltGameMsgResponse_DailyQuest2List
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CDailyQuest2ObjData, MAX_AVAILABLE_DAILYQUEST2_NUM>		m_DailyQuest2ObjDataList;
#else
	CDailyQuest2ObjData m_DailyQuest2ObjDataList[ MAX_AVAILABLE_DAILYQUEST2_NUM ];
#endif
};

class cltGameMsgRequest_DailyQuest2Select
{
public:
	CDailyQuest2ObjData DailyQuest2ObjData;
	BOOL				bMoneySelected;
	SI16				siIndex;

	cltGameMsgRequest_DailyQuest2Select( CDailyQuest2ObjData *pDailyQuest2ObjData, BOOL moneySelected, SI16 Indexnum )
	{
		memcpy( &DailyQuest2ObjData, pDailyQuest2ObjData, sizeof( CDailyQuest2ObjData ) );
		bMoneySelected = moneySelected;
		siIndex = Indexnum;
	}	
};

class cltGameMsgResponse_DailyQuest2Select
{
public:
	cltPDailyQuest2Info	DailyQuest2Info;
	GMONEY				siLeftMoney;

	cltGameMsgResponse_DailyQuest2Select( cltPDailyQuest2Info *pDailyQuest2Info, GMONEY LeftMoney )
	{
		memcpy( &DailyQuest2Info, pDailyQuest2Info, sizeof( cltPDailyQuest2Info ) );
		siLeftMoney = LeftMoney;
	}
};

class cltGameMsgRequest_DailyQuest2Situation
{
public:
	bool bRequest;
	cltGameMsgRequest_DailyQuest2Situation()
	{
	}
};

class cltGameMsgResponse_DailyQuest2Situation
{
public:
	cltPDailyQuest2Info DailyQuest2Info;

	cltGameMsgResponse_DailyQuest2Situation( cltPDailyQuest2Info *pDailyQuest2Info )
	{
		memcpy( &DailyQuest2Info, pDailyQuest2Info, sizeof( cltPDailyQuest2Info ) );
	}
};

class cltGameMsgResponse_DailyQuest2Change
{
public:
	SI32 siLeftTime;
	SI16 siKillMonsterNum;
	bool bHiddenquest;

	cltGameMsgResponse_DailyQuest2Change( SI32 LeftTime, SI16 KillMonsterNum, bool Hiddenquest )
	{
		siLeftTime = LeftTime;
		siKillMonsterNum = KillMonsterNum;
		bHiddenquest = Hiddenquest;
	}
};

class cltGameMsgRequest_DailyQuest2End
{
public:
	bool bSuccess;

	cltGameMsgRequest_DailyQuest2End()
	{
	}
	
};

class cltGameMsgResponse_DailyQues2tEnd
{
public:
	UI08 bSuccess;
	SI64 siCurExp;// SI32->SI64 경험치 오버플로우때문에 수정
	
	SI16 siInventoryItemIndex;
	SI16 siChangedItemNum;
	cltItem clInvItem;
	GMONEY siAddMoney;
	SI32 siChangedExp;
	bool bHiddenquest;
	SI32 siAddGP;
	cltGameMsgResponse_DailyQues2tEnd( UI08 Success, SI64 CurExp, SI16 InventoryItemIndex, SI16 ChangedItemNum, // SI32->SI64 경험치 오버플로우때문에 수정
		cltItem *pclItem, GMONEY AddMoney, SI32 ChangedExp , SI32 AddGP, bool HiddenQuest)
	{
		bSuccess = Success;
		siCurExp = CurExp;

		siInventoryItemIndex = InventoryItemIndex;
		siChangedItemNum = ChangedItemNum;
		clInvItem.Set( pclItem );
		siChangedExp = ChangedExp;
		siAddMoney = AddMoney;
		siAddGP = AddGP;
		bHiddenquest = HiddenQuest;
	}	
};

#endif