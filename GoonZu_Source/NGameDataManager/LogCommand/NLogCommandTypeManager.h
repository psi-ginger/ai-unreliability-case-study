//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDTYPEMANAGER_H_
#define _NLOGCOMMANDTYPEMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogCommandType.h"

#define MAX_LOGCOMMANDTYPE_COUNT		200

class NLogCommandTypeManager
{
protected:
	NLogCommandType*	m_pkData[MAX_LOGCOMMANDTYPE_COUNT];
	SI32				m_siCount;

public:
	NLogCommandTypeManager();
	~NLogCommandTypeManager();

	BOOL		Init();
	BOOL		LoadLogCommandType();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogCommandType*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGCOMMANDTYPE_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGCOMMANDTYPE_COUNT;
	}
};

#endif
