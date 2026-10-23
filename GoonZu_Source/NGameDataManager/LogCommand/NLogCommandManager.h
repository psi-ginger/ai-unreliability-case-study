//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDMANAGER_H_
#define _NLOGCOMMANDMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogCommand.h"

#define MAX_LOGCOMMAND_COUNT		2000

class NLogCommandManager
{
protected:
	NLogCommand*		m_pkData[MAX_LOGCOMMAND_COUNT];
	SI32				m_siCount;

public:
	NLogCommandManager();
	~NLogCommandManager();

	BOOL		Init();
	BOOL		LoadLogCommand();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogCommand*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGCOMMAND_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGCOMMAND_COUNT;
	}
};

#endif
