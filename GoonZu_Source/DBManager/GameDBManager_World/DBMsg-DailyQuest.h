#ifndef _DBMSG_DAILYQUEST_H_
#define _DBMSG_DAILYQUEST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

class sDBRequest_DailyQuestAdd
{
public:
	sPacketHeader packetHeader;

	SI16 siQuestListIndex;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siMonsterKind;
	SI16 siMonsterNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	GMONEY	siRewardMoney;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;
	SI32 siLeftTime;
	
	SI32 siVillageUnique;
	SI32 siStrType;				// 세금을 넣어야 하는 건물
	GMONEY siVillagePayFeeMoney;

	BOOL bPCRoomQuest;
	BOOL bMoneySelected;

	sDBRequest_DailyQuestAdd( SI16 QuestListIndex, SI32 PersonID, SI32 CharID, SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 QuestTime, SI32 LeftTime, SI32 VillageUnique, SI32 StrType, GMONEY VillagePayFeeMoney, BOOL bpcroomquest, BOOL bmoneyselected )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuestAdd );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUESTADD;

		siQuestListIndex = QuestListIndex;

		siPersonID = PersonID;
		siCharID = CharID;

		siMonsterKind = MonsterKind;
		siMonsterNum = MonsterNum;

		siRewardItemUnique = RewardItemUnique;
		siRewardItemNum = RewardItemNum;
		siRewardMoney = RewardMoney;
		siRewardExp = RewardExp;

		siFee = Fee;
		siQuestTime = QuestTime;
		siLeftTime = LeftTime;

		siVillageUnique = VillageUnique;
		siStrType = StrType;
		siVillagePayFeeMoney = VillagePayFeeMoney;

		bPCRoomQuest = bpcroomquest;
		bMoneySelected = bmoneyselected;
	}
};

class sDBResponse_DailyQuestAdd
{
public:
	sPacketHeader		packetHeader;

	SI32 siResult;
	
	SI16 siQuestListIndex;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siMonsterKind;
	SI16 siMonsterNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	GMONEY siRewardMoney;
	SI16 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;
	SI32 siLeftTime;
	GMONEY siLeftMoney;

	SI32 siVillageUnique;
	SI32 siStrType;
	cltMoney clStrMoney;

	BOOL bPCRoomQuest;
	BOOL bMoneySelected;


	sDBResponse_DailyQuestAdd()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuestAdd ) );
	}
};

class sDBRequest_DailyQuestDel
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	UI08 uiSuccess;

	SI16 siInventoryIndex;
	cltItem clRewardItem;
	UI08 uiPileSwitch;

	GMONEY	siRewardMoney;
	BOOL	bMoneySelected;

	SI32 siRewardExp;
		
	sDBRequest_DailyQuestDel( SI32 PersonID, SI32 CharID, UI08 Success, SI16 InventoryIndex, cltItem *pRewardItem, UI08 PileSwitch, GMONEY RewardMoney, BOOL MoneySelected, SI32 RewardExp )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuestDel );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUESTDEL;

		siPersonID = PersonID;
		siCharID = CharID;

		uiSuccess = Success;
		
		siInventoryIndex = InventoryIndex;
		clRewardItem.Set( pRewardItem );
		uiPileSwitch = PileSwitch;

		siRewardMoney = RewardMoney;
		bMoneySelected = MoneySelected;

		siRewardExp = RewardExp;
	}
};

class sDBResponse_DailyQuestDel
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	UI08 uiSuccess;

	SI16 siInventoryItemIndex;	
	cltItem clInventoryItem;

	SI16 siChangedItemNum;

	GMONEY	siAddMoney;

	SI64 siExp;		// SI32->SI64 경험치 오버플로우때문에 수정	

	cltItem clRewardItem;		// For LOG

	sDBResponse_DailyQuestDel()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuestDel ) );
	}
};

class sDBRequest_DailyQuestChange
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siKillMonsterNum;
	SI32 siLeftTime;

	// PCK - 몬스터 카인드 추가(로그용)
	SI16 siMonsterKind;

	sDBRequest_DailyQuestChange( SI32 PersonID, SI32 CharID, SI16 KillMonsterNum, SI32 LeftTime, SI16 MonsterKind )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuestChange );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUESTCHANGE;

		siPersonID = PersonID;
		siCharID = CharID;

		siKillMonsterNum = KillMonsterNum;
		siLeftTime = LeftTime;
		siMonsterKind = MonsterKind;
	}
};

class sDBResponse_DailyQuestChange
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siKillMonsterNum;
	SI32 siLeftTime;

	// PCK - 몬스터 카인드 추가(로그용)
	SI16 siMonsterKind;
	
	sDBResponse_DailyQuestChange()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuestChange ) );
	}
};


#endif
