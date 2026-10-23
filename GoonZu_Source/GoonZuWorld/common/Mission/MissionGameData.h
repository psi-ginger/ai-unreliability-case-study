#ifndef _MISSIONGAMEDATA_H_
#define _MISSIONGAMEDATA_H_

#include "Mission.h"

typedef NSafeTArray<cltMissionReward, MAX_MISSON_REWARD_COUNT>	ARRAY_MISSIONREWARDS;

class cltMissionGameData
{
public:
	SI16							m_siMissionUnique;
	NTCHARString<MAX_MISSON_UNIQUESTRING_LENGTH>	m_kMissionUniqueString;

	cltMissionClearCondition		m_clClearCondition;
	ARRAY_MISSIONREWARDS			m_clRewards;

	SI32							m_siTime;
	SI32							m_siFee;

public:
	cltMissionGameData()
	{
		ZeroMemory(this, sizeof(*this));
	}
	~cltMissionGameData()
	{
	}

	inline NTCHARString<MAX_MISSON_UNIQUESTRING_LENGTH>& GetUniqueString()
	{
		return m_kMissionUniqueString;
	}

	bool AddReward( UI16 uiRewardType, UI16 uiKindUnique, SI32 siAmount )
	{
		bool bRet = false;
		for( SI32 i=0; i<m_clRewards.GetSize(); i++ )
		{
			if( m_clRewards[i].IsEmpty() == false )		continue;

			m_clRewards[i].m_siRewardType	= uiRewardType;
			m_clRewards[i].m_siKindUnique	= uiKindUnique;
			m_clRewards[i].m_siAmount		= siAmount;

			bRet = true;
		}

		return bRet;
	}
};

typedef NSafeTArray<cltMissionGameData*, MAX_MISSON_DATA_COUNT>	ARRAY_MISSIONGAMEDATAS;

class cltMissionGameDataManager
{
protected:
	ARRAY_MISSIONGAMEDATAS			m_clMissionGameData;

public:
	cltMissionGameDataManager();
	~cltMissionGameDataManager();

	bool Init();
	bool LoadDataFromFile();

	cltMissionGameData* GetMissionDataFromUniqueString( const TCHAR* pszUniqueString );
	cltMissionGameData* GetMissionDataFromUnique( SI16 siUnique );
};

#endif