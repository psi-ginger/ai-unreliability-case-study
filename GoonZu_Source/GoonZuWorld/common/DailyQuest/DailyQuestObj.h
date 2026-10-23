#ifndef _DAILYQUESTOBJ_H_
#define _DAILYQUESTOBJ_H_

#include "..\SMLib\LibList\Obj.h"
#include "NDataTypes.h"

class CDailyQuestObjData
{
public:
	SI16 m_siIndex;
	SI16 m_siCount;

	CDailyQuestObjData()
	{
		memset( this, 0, sizeof( CDailyQuestObjData ) );
	}

	CDailyQuestObjData( SI16 Index )
	{
		memset( this, 0, sizeof( CDailyQuestObjData ) );
		m_siIndex = Index;
	}	

	void Init()
	{
		if ( m_siCount++ > 30000 )
		{
			m_siCount = 0;
		}

		m_siMonsterKind = 0;
		m_siMonsterNum = 0;;
		
		m_siRewardItemUnique = 0;
		m_siRewardItemNum = 0;
		m_siRewardMoney = 0;
		m_siRewardExp = 0;
		
		m_siFee = 0;
		m_siLeftTime = 0;
		
		m_siKillMonsterNum = 0;
//		m_bPCRoomQuest = FALSE;
	}

	void Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, 
		SI32 RewardExp, GMONEY Fee, SI32 LeftTime, SI16 KillMonsterNum)//, BOOL bPCRoomQuest )
	{
		m_siMonsterKind = MonsterKind;
		m_siMonsterNum = MonsterNum;

		m_siRewardItemUnique = RewardItemUnique;
		m_siRewardItemNum = RewardItemNum;
		m_siRewardMoney = RewardMoney;
		m_siRewardExp = RewardExp;

		m_siFee = Fee;
		m_siLeftTime = LeftTime;

		m_siKillMonsterNum = KillMonsterNum;
//		m_bPCRoomQuest = bPCRoomQuest;
	}

	bool IsSame( SI16 Index, SI16 Count )
	{
		if ( m_siIndex == Index && m_siCount == Count )
		{
			return true;
		}

		return false;
	}

	bool IsSameData( CDailyQuestObjData *pDailyQuestObjData )
	{
		if ( pDailyQuestObjData->m_siMonsterKind == m_siMonsterKind &&
			pDailyQuestObjData->m_siMonsterNum == m_siMonsterNum &&
			pDailyQuestObjData->m_siRewardItemUnique == m_siRewardItemUnique &&
			pDailyQuestObjData->m_siRewardItemNum == m_siRewardItemNum &&
			pDailyQuestObjData->m_siRewardMoney == m_siRewardMoney &&
			pDailyQuestObjData->m_siRewardExp == m_siRewardExp &&
			pDailyQuestObjData->m_siFee == m_siFee &&
			pDailyQuestObjData->m_siLeftTime == m_siLeftTime &&
			pDailyQuestObjData->m_siKillMonsterNum == m_siKillMonsterNum
//			pDailyQuestObjData->m_bPCRoomQuest == m_bPCRoomQuest
			)
		{
			return true;
		}

		return false;
	}
	
	SI16 m_siMonsterKind;
	SI16 m_siMonsterNum;

	SI16 m_siRewardItemUnique;
	SI16 m_siRewardItemNum;
	GMONEY m_siRewardMoney;
	SI32 m_siRewardExp;
	
	GMONEY m_siFee;
	SI32 m_siLeftTime;

	SI16 m_siKillMonsterNum;

//	BOOL m_bPCRoomQuest;
};

#endif