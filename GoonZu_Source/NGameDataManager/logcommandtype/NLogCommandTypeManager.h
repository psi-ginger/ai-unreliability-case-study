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

public:
	NLogCommandTypeManager();
	~NLogCommandTypeManager();

	BOOL		Init(TCHAR* pszLanaugae);
	BOOL		LoadLogCommandType(TCHAR* pszLanaugae);

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogCommandType*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGCOMMANDTYPE_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline NLogCommandType* GetType(SI32 siType)
	{
		if(siType < 0 || siType >= MAX_LOGCOMMANDTYPE_COUNT)	return NULL;
		return m_pkData[siType];
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGCOMMANDTYPE_COUNT;
	}
};

#endif
