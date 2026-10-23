#ifndef _MSGTYPEDAILYQUEST3_H_
#define _MSGTYPEDAILYQUEST3_H_

#include "..\..\Common\CommonHeader.h"
#include "../../Common/DailyQuest3/DailyQuest3Mgr.h"
#include "../../Common/DailyQuest3/DailyQuest3Obj.h"
#include "../../Common/Item/ItemCommon/cltItem.h"

// DailyQuest 리스트 요청
class cltGameMsgRequest_DailyQuest3List
{
public:
	bool bRequest;	
	cltGameMsgRequest_DailyQuest3List()
	{		
	}
};

// DailyQuest 리스트 요청 응답
class cltGameMsgResponse_DailyQuest3List
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CDailyQuest3ObjData, MAX_AVAILABLE_DAILYQUEST3_NUM>		m_DailyQuest3ObjDataList;
#else
	CDailyQuest3ObjData m_DailyQuest3ObjDataList[ MAX_AVAILABLE_DAILYQUEST3_NUM ];
#endif
};

class cltGameMsgRequest_DailyQuest3Select
{
public:
	CDailyQuest3ObjData DailyQuest3ObjData;
	BOOL				bMoneySelected;
	SI16				siIndex;

	cltGameMsgRequest_DailyQuest3Select( CDailyQuest3ObjData *pDailyQuest3ObjData, BOOL moneySelected, SI16 Indexnum )
	{
		memcpy( &DailyQuest3ObjData, pDailyQuest3ObjData, sizeof( CDailyQuest3ObjData ) );
		bMoneySelected = moneySelected;
		siIndex = Indexnum;
	}	
};

class cltGameMsgResponse_DailyQuest3Select
{
public:
	cltPDailyQuest3Info	DailyQuest3Info;
	GMONEY				siLeftMoney;

	cltGameMsgResponse_DailyQuest3Select( cltPDailyQuest3Info *pDailyQuest3Info, GMONEY LeftMoney )
	{
		memcpy( &DailyQuest3Info, pDailyQuest3Info, sizeof( cltPDailyQuest3Info ) );
		siLeftMoney = LeftMoney;
	}
};

class cltGameMsgRequest_DailyQuest3Situation
{
public:
	bool bRequest;
	cltGameMsgRequest_DailyQuest3Situation()
	{
	}
};

class cltGameMsgResponse_DailyQuest3Situation
{
public:
	cltPDailyQuest3Info DailyQuest3Info;

	cltGameMsgResponse_DailyQuest3Situation( cltPDailyQuest3Info *pDailyQuest3Info )
	{
		memcpy( &DailyQuest3Info, pDailyQuest3Info, sizeof( cltPDailyQuest3Info ) );
	}
};

class cltGameMsgResponse_DailyQuest3Change
{
public:
	SI32 siLeftTime;
	SI32 siMakedItem;
	bool bHiddenquest;

	cltGameMsgResponse_DailyQuest3Change( SI32 LeftTime, SI32 MakedItem, bool Hiddenquest )
	{
		siLeftTime = LeftTime;
		siMakedItem = MakedItem;
		bHiddenquest = Hiddenquest;
	}
};

class cltGameMsgRequest_DailyQuest3End
{
public:
	bool bSuccess;

	cltGameMsgRequest_DailyQuest3End()
	{
	}
	
};

class cltGameMsgResponse_DailyQuest3End
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
	bool bHiddenquest;

	
	cltGameMsgResponse_DailyQuest3End( UI08 Success, SI64 CurExp, SI16 InventoryItemIndex, SI16 ChangedItemNum, // SI32->SI64 경험치 오버플로우때문에 수정
		cltItem *pclItem, GMONEY AddMoney, SI32 ChangedExp ,SI32 AddGP,bool HiddenQuest)
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