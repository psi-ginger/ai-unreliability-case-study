//=============================================================================
//
//=============================================================================
#ifndef _NCHARGAMEDATAMANAGER_H_
#define _NCHARGAMEDATAMANAGER_H_

#include "../../NLib/NArray.h"
#include "NCharGameData.h"
#include "../../NLib/NTextManager.h"

#define MAX_CHAR_COUNT		1000

class NDataTypeInfo;

class NCharGameDataManager
{
protected:
	NCharGameData*		m_pkData[MAX_CHAR_COUNT];

public:
	NCharGameDataManager();
	~NCharGameDataManager();

	BOOL		Init(SI32 siServiceArea, NTextManager* pTextManager);
	BOOL		LoadCharGameData(SI32 siServiceArea , NTextManager* pTextManager);

	//===============================================================
	// INLINES
	//===============================================================
	inline NCharGameData*	GetData(SI32 siUnique)
	{
		if(siUnique < 0 || siUnique >= MAX_CHAR_COUNT)	return NULL;
		return m_pkData[siUnique];
	}

	inline TCHAR* GetCharName(SI32 siUnique)
	{
		if(siUnique < 0 || siUnique >= MAX_CHAR_COUNT)	return NULL;
		if(m_pkData[siUnique] != NULL)		return m_pkData[siUnique]->m_szCharName;

		return NULL;
	}

	inline SI32				GetCount()
	{
		return MAX_CHAR_COUNT;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_CHAR_COUNT;
	}
};

#endif
