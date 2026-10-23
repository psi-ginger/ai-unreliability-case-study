#ifndef _DBMSG_DAILYQUEST3_H_
#define _DBMSG_DAILYQUEST3_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

class sDBRequest_DailyQuest3Add
{
public:
	sPacketHeader packetHeader;

	SI16 siQuestListIndex;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siItemUnique;
	SI16 siItemNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;
	SI32 siLeftTime;
	
	SI32 siVillageUnique;
	SI32 siStrType;				// 세금을 넣어야 하는 건물
	GMONEY siVillagePayFeeMoney;

	BOOL bPCRoomQuest;
	BOOL bMoneySelected;	

	//SI32 siGuildPoint;
	bool bHiddenQuest;

	sDBRequest_DailyQuest3Add( SI32 QuestListIndex, SI32 PersonID, SI32 CharID, SI16 ItemUnique, SI16 ItemNum, 
		SI16 RewardItemUnique, SI16 RewardItemNum, SI32 RewardExp, GMONEY Fee,
		SI32 QuestTime, SI32 LeftTime, SI32 VillageUnique, SI32 StrType, GMONEY VillagePayFeeMoney, 
		BOOL bpcroomquest, BOOL bmoneyselected , bool HiddenQuest )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest3Add );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST3ADD;

		siQuestListIndex = QuestListIndex;

		siPersonID = PersonID;
		siCharID = CharID;

		siItemUnique = ItemUnique;
		siItemNum = ItemNum;

		siRewardItemUnique = RewardItemUnique;
		siRewardItemNum = RewardItemNum;
		siRewardExp = RewardExp;

		siFee = Fee;
		siQuestTime = QuestTime;
		siLeftTime = LeftTime;

		siVillageUnique = VillageUnique;
		siStrType = StrType;
		siVillagePayFeeMoney = VillagePayFeeMoney;

		bHiddenQuest = HiddenQuest;

		bPCRoomQuest = bpcroomquest;
		bMoneySelected = bmoneyselected;
	}
};

class sDBResponse_DailyQuest3Add
{
public:
	sPacketHeader		packetHeader;

	SI32 siResult;
	
	SI16 siQuestListIndex;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siItemUnique;
	SI16 siItemNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;
	SI32 siLeftTime;
	GMONEY siLeftMoney;

	SI32 siVillageUnique;
	SI32 siStrType;
	cltMoney clStrMoney;

	BOOL bPCRoomQuest;
	bool bMoneySelected;

	//SI32 siGuildPoint;

	bool bHiddenQuest;


	sDBResponse_DailyQuest3Add()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest3Add ) );
	}
};

class sDBRequest_DailyQuest3Del
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	UI08 uiSuccess;

	SI16 siItemUnique;	// 해당 아이템의 기술 경험치를 주어야 하기 때문에 만든 아이템 유니크를 저장 한다.
	SI16 siInventoryIndex;
	cltItem clRewardItem;
	UI08 uiPileSwitch;

	BOOL	bMoneySelected;

	bool bHiddenQuest;

		
	sDBRequest_DailyQuest3Del( SI32 PersonID, SI32 CharID, UI08 Success, SI16 InventoryIndex, SI16 ItemUnique, cltItem *pRewardItem, UI08 PileSwitch, BOOL MoneySelected, bool HiddenQuest )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest3Del );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST3DEL;

		siPersonID = PersonID;
		siCharID = CharID;

		uiSuccess = Success;

        siItemUnique = ItemUnique;
		siInventoryIndex = InventoryIndex;
		clRewardItem.Set( pRewardItem );
		uiPileSwitch = PileSwitch;

		bMoneySelected = MoneySelected;
		bHiddenQuest = HiddenQuest;


	}
};

class sDBResponse_DailyQuest3Del
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	UI08 uiSuccess;

	SI16 siItemUnique;	// 해당 아이템의 기술 경험치를 주어야 하기 때문에 만든 아이템 유니크를 저장 한다.
	SI16 siInventoryItemIndex;	
	cltItem clInventoryItem;

	SI16 siChangedItemNum;

	GMONEY	siAddMoney;

	SI64 siExp;// SI32->SI64 경험치 오버플로우때문에 수정	

	//SI32 siGuildPoint; 길드 포인트는 따로 증가 시킨다.
	bool bHiddenQuest;

	cltItem clRewardItem;		// For LOG

	sDBResponse_DailyQuest3Del()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest3Del ) );
	}
};

class sDBRequest_DailyQuest3Change
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siMakedItem;
	SI32 siLeftTime;
	bool bHiddenQuest;
	
	sDBRequest_DailyQuest3Change( SI32 PersonID, SI32 CharID, SI16 MakedItem, SI32 LeftTime, bool HiddenQuest )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest3Change );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST3CHANGE;

		siPersonID = PersonID;
		siCharID = CharID;

		siMakedItem = MakedItem;
		siLeftTime = LeftTime;
		bHiddenQuest = HiddenQuest;

	}
};

class sDBResponse_DailyQuest3Change
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siMakedItem;
	SI32 siLeftTime;
	bool bHiddenQuest;

	sDBResponse_DailyQuest3Change()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest3Change ) );
	}
};


#endif
