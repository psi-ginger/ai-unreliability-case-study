//=============================================================================
//
//=============================================================================
#ifndef _NQUESTTYPEGAMEDATAMANAGER_H_
#define _NQUESTTYPEGAMEDATAMANAGER_H_

#include "../../NLib/NArray.h"
#include "NQuestTypeGameData.h"

class NQuestTypeGameDataManager
{
protected:
	NQuestTypeGameData*		m_pkData[MAX_QUEST_TYPE];

public:
	NQuestTypeGameDataManager();
	~NQuestTypeGameDataManager();

	BOOL		Init();
	BOOL		LoadQuestTypeGameData();

	//===============================================================
	// INLINES
	//===============================================================
	inline NQuestTypeGameData*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_QUEST_TYPE)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return MAX_QUEST_TYPE;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_QUEST_TYPE;
	}
};

#endif
