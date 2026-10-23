//=============================================================================
//
//=============================================================================
#ifndef _NLOGRESAONDESCRIPTIONMANAGER_H_
#define _NLOGRESAONDESCRIPTIONMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogReasonDescription.h"

#define MAX_LOGRESAONDESCRIPTION_COUNT		3000

class NLogReasonDescriptionManager
{
protected:
	NLogReasonDescription*	m_pkData[MAX_LOGRESAONDESCRIPTION_COUNT];
	SI32				m_siCount;

public:
	NLogReasonDescriptionManager();
	~NLogReasonDescriptionManager();

	BOOL		Init(TCHAR* pszLanaugae);
	BOOL		LoadLogReasonDescription(TCHAR* pszLanaugae);

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogReasonDescription*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGRESAONDESCRIPTION_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGRESAONDESCRIPTION_COUNT;
	}

	TCHAR* GetReasonDescription(SI32 siType, SI32 siIndex, SI32 siReason)
	{
		for(SI32 i=0; i<MAX_LOGRESAONDESCRIPTION_COUNT; i++)
		{
			if( m_pkData[i] == NULL )		continue;

			if( m_pkData[i]->m_siType == siType && m_pkData[i]->m_siIndex == siIndex && m_pkData[i]->m_siReason == siReason )
			{
				return m_pkData[i]->m_szDescription;
			}
		}
		return NULL;
	}
};

#endif
