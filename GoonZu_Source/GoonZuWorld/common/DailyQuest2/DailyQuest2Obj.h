#ifndef _DAILYQUEST2OBJ_H_
#define _DAILYQUEST2OBJ_H_

#include "..\SMLib\LibList\Obj.h"
#include "NDataTypes.h"

class CDailyQuest2ObjData
{
public:
	SI16 m_siIndex;
	SI16 m_siCount;

	CDailyQuest2ObjData()
	{
		memset( this, 0, sizeof( CDailyQuest2ObjData ) );
	}

	CDailyQuest2ObjData( SI16 Index )
	{
		memset( this, 0, sizeof( CDailyQuest2ObjData ) );
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
		m_siRewardGuildPoint = 0;
		m_bHiddenQuest			= false;
//		m_bPCRoomQuest = FALSE;
	}

	void Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique,
		SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 LeftTime, SI16 KillMonsterNum, 
		SI32 RewardGuildPoint, bool HiddenQuest)//, BOOL bPCRoomQuest )
	{
		m_siMonsterKind = MonsterKind;
		m_siMonsterNum = MonsterNum;

		m_siRewardItemUnique = RewardItemUnique;
		m_siRewardItemNum = RewardItemNum;
		m_siRewardMoney = RewardMoney;
		m_siRewardExp = RewardExp;

		m_siFee = 0;//Fee;
		m_siLeftTime = LeftTime;

		m_siKillMonsterNum = KillMonsterNum;
		m_siRewardGuildPoint = 0;//RewardGuildPoint;
		m_bHiddenQuest		 = HiddenQuest;
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

	/*bool IsSameData( CDailyQuest2ObjData *pDailyQuest2ObjData )
	{
		if ( pDailyQuest2ObjData->m_siMonsterKind == m_siMonsterKind &&
			pDailyQuest2ObjData->m_siMonsterNum == m_siMonsterNum &&
			pDailyQuest2ObjData->m_siRewardItemUnique == m_siRewardItemUnique &&
			pDailyQuest2ObjData->m_siRewardItemNum == m_siRewardItemNum &&
			pDailyQuest2ObjData->m_siRewardMoney == m_siRewardMoney &&
			pDailyQuest2ObjData->m_siRewardExp == m_siRewardExp &&
			pDailyQuest2ObjData->m_siFee == m_siFee &&
			pDailyQuest2ObjData->m_siLeftTime == m_siLeftTime &&
			pDailyQuest2ObjData->m_siKillMonsterNum == m_siKillMonsterNum&&
			pDailyQuest2ObjData->m_siRewardGuildPoint == m_siRewardGuildPoint &&
			pDailyQuest2ObjData->m_bHiddenQuest	== m_bHiddenQuest
			)
		{
			return true;
		}

		return false;
	}*/
	
	SI16 m_siMonsterKind;
	SI16 m_siMonsterNum;

	SI16 m_siRewardItemUnique;
	SI16 m_siRewardItemNum;
	GMONEY m_siRewardMoney;
	SI32 m_siRewardExp;
	
	GMONEY m_siFee;
	SI32 m_siLeftTime;

	SI16 m_siKillMonsterNum;
	SI32 m_siRewardGuildPoint;
	bool m_bHiddenQuest;

//	BOOL m_bPCRoomQuest;
};

#endif