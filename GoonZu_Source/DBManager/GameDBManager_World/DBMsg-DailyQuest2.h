#ifndef _DBMSG_DAILYQUEST2_H_
#define _DBMSG_DAILYQUEST2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

class sDBRequest_DailyQuest2Add
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

	//SI32 siGuildPoint;
	bool bHiddenQuest;

	sDBRequest_DailyQuest2Add( SI16 QuestListIndex, SI32 PersonID, SI32 CharID, SI16 MonsterKind, SI16 MonsterNum, 
		SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee,
		SI32 QuestTime, SI32 LeftTime, SI32 VillageUnique, SI32 StrType, GMONEY VillagePayFeeMoney, 
		BOOL bpcroomquest, BOOL bmoneyselected /*,SI32 GuildPoint*/, bool HiddenQuest )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest2Add );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST2ADD;

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

		//siGuildPoint = GuildPoint;
		bHiddenQuest = HiddenQuest;

		bPCRoomQuest = bpcroomquest;
		bMoneySelected = bmoneyselected;
	}
};

class sDBResponse_DailyQuest2Add
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


	sDBResponse_DailyQuest2Add()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest2Add ) );
	}
};

class sDBRequest_DailyQuest2Del
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

	//SI32 siGuildPoint; 길드 포인트는 따로 증가 시킨다.
	bool bHiddenQuest;

	//SI32 siRewardExp;-> 비교할수가 없으므로 뺀다.
		
	sDBRequest_DailyQuest2Del( SI32 PersonID, SI32 CharID, UI08 Success, SI16 InventoryIndex, cltItem *pRewardItem, UI08 PileSwitch, GMONEY RewardMoney, BOOL MoneySelected/* SI32 RewardExp, SI32 GuildPoint*/, bool HiddenQuest )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest2Del );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST2DEL;

		siPersonID = PersonID;
		siCharID = CharID;

		uiSuccess = Success;
		
		siInventoryIndex = InventoryIndex;
		clRewardItem.Set( pRewardItem );
		uiPileSwitch = PileSwitch;

		siRewardMoney = RewardMoney;
		bMoneySelected = MoneySelected;

		//siRewardExp = RewardExp;
		//siGuildPoint = GuildPoint; 길드 포인트는 따로 패킷을 보낸다.
		bHiddenQuest = HiddenQuest;


	}
};

class sDBResponse_DailyQuest2Del
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

	SI64 siExp;// SI32->SI64 경험치 오버플로우때문에 수정	

	//SI32 siGuildPoint; 길드 포인트는 따로 증가 시킨다.
	bool bHiddenQuest;

	cltItem clRewardItem;		// For LOG

	sDBResponse_DailyQuest2Del()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest2Del ) );
	}
};

class sDBRequest_DailyQuest2Change
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siKillMonsterNum;
	SI32 siLeftTime;
	bool bHiddenQuest;

	// PCK ; 몬스터 카인드 추가 (로그용)
	SI16 siMonsterKind;

	sDBRequest_DailyQuest2Change( SI32 PersonID, SI32 CharID, SI16 KillMonsterNum, SI32 LeftTime, bool Hiddenquest, SI16 MonsterKind )
	{
		packetHeader.usSize = sizeof( sDBRequest_DailyQuest2Change );
		packetHeader.usCmd	= DBMSG_REQUEST_DAILYQUEST2CHANGE;

		siPersonID = PersonID;
		siCharID = CharID;

		siKillMonsterNum = KillMonsterNum;
		siLeftTime = LeftTime;
		bHiddenQuest = Hiddenquest;
		siMonsterKind = MonsterKind;
	}
};

class sDBResponse_DailyQuest2Change
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siKillMonsterNum;
	SI32 siLeftTime;
	bool bHiddenQuest;
	
	// PCK : 몬스터 카인드 추가 (로그용)
	SI16 siMonsterKind;

	sDBResponse_DailyQuest2Change()
	{
		memset( this, 0, sizeof( sDBResponse_DailyQuest2Change ) );
	}
};


#endif
