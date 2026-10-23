//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDFILTERMANAGER_H_
#define _NLOGCOMMANDFILTERMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogCommandFilter.h"

#define MAX_LOGCOMMANDFILTER_COUNT		2000

class NLogCommandFilterManager
{
protected:
	NLogCommandFilter*	m_pkData[MAX_LOGCOMMANDFILTER_COUNT];
	SI32				m_siCount;

public:
	NLogCommandFilterManager();
	~NLogCommandFilterManager();

	BOOL		Init();
	BOOL		LoadLogCommandFilter();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogCommandFilter*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGCOMMANDFILTER_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGCOMMANDFILTER_COUNT;
	}

	NLogCommandFilter* FindLogCommandFilter(TCHAR* pszFilterName);
};

#endif
