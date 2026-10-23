//=============================================================================
//
//=============================================================================
#ifndef _NVILLAGEGAMEDATAMANAGER_H_
#define _NVILLAGEGAMEDATAMANAGER_H_

#include "../../NLib/NArray.h"
#include "NVillageGameData.h"

#define MAX_VILLAGE_COUNT		100

class NDataTypeInfo;

class NVillageGameDataManager
{
protected:
	NVillageGameData*		m_pkData[MAX_VILLAGE_COUNT];

public:
	NVillageGameDataManager();
	~NVillageGameDataManager();

	BOOL		Init(SI32 siServiceArea);
	BOOL		LoadVillageGameData(SI32 siServiceArea);

	//===============================================================
	// INLINES
	//===============================================================
	inline NVillageGameData*	GetData(SI32 siVillageUnique)
	{
		if(siVillageUnique < 0 || siVillageUnique >= MAX_VILLAGE_COUNT)	return NULL;
		return m_pkData[siVillageUnique];
	}

	inline TCHAR* GetVillageName(SI32 siVillageUnique)
	{
		if(siVillageUnique < 0 || siVillageUnique >= MAX_VILLAGE_COUNT)	return NULL;
		if(m_pkData[siVillageUnique] != NULL)		return m_pkData[siVillageUnique]->m_szName;

		return NULL;
	}

	inline SI32				GetCount()
	{
		return MAX_VILLAGE_COUNT;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_VILLAGE_COUNT;
	}
};

#endif
