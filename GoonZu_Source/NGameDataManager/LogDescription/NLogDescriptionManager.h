//=============================================================================
//
//=============================================================================
#ifndef _NLOGDESCRIPTIONMANAGER_H_
#define _NLOGDESCRIPTIONMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogDescription.h"

#define MAX_LOGDESCRIPTION_COUNT		2000

class NLogDescriptionManager
{
protected:
	NLogDescription*	m_pkData[MAX_LOGDESCRIPTION_COUNT];
	SI32				m_siCount;

public:
	NLogDescriptionManager();
	~NLogDescriptionManager();

	BOOL		Init();
	BOOL		LoadLogDescription();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogDescription*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGDESCRIPTION_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGDESCRIPTION_COUNT;
	}

	TCHAR* GetDescription(SI32 siType, SI32 siIndex)
	{
		for(SI32 i=0; i<MAX_LOGDESCRIPTION_COUNT; i++)
		{
			if( m_pkData[i] == NULL )		continue;

			if( m_pkData[i]->m_siType == siType && m_pkData[i]->m_siIndex == siIndex )
			{
				return m_pkData[i]->m_szDescription;
			}
		}
		return NULL;
	}
};

#endif
