//=============================================================================
//
//=============================================================================
#ifndef _NLOGTEXTTRANSMANAGER_H_
#define _NLOGTEXTTRANSMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogTextTrans.h"

#define MAX_LOGTEXT_COUNT	500
class NLogTextTransManager
{
protected:
	NLogTextTrans*		m_pkData[MAX_LOGTEXT_COUNT];
	SI32				m_siCount;

public:
	NLogTextTransManager();
	~NLogTextTransManager();

	BOOL		Init();
	BOOL		LoadLogTextTrans();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogTextTrans*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_LOGTEXT_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_LOGTEXT_COUNT;
	}

	TCHAR* FindLogTextTransCommand(const TCHAR* pszCommand, const TCHAR* pszType, SI32 siIndex);
};

#endif
