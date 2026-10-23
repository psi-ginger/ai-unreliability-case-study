//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDINDEXMANAGER_H_
#define _NLOGCOMMANDINDEXMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogCommandIndex.h"

#define MAX_LOGCOMMANDINDEX_COUNT		2000

class NLogCommandIndexManager
{
protected:
	NLogCommandIndex*	m_pkData[MAX_LOGCOMMANDINDEX_COUNT];
	SI32				m_siCount;

public:
	NLogCommandIndexManager();
	~NLogCommandIndexManager();

	BOOL		Init();
	BOOL		LoadLogCommandIndex();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogCommandIndex*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGCOMMANDINDEX_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGCOMMANDINDEX_COUNT;
	}
};

#endif
