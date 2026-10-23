//=============================================================================
//
//=============================================================================
#ifndef _NLOGSERVERINFOMANAGER_H_
#define _NLOGSERVERINFOMANAGER_H_

#include "../../NLib/NArray.h"
#include "NLogServerInfo.h"

#define MAX_SERVER_COUNT		48

class NLogServerInfoManager
{
protected:
	NLogServerInfo*		m_pkData[MAX_SERVER_COUNT];
	SI32				m_siCount;

public:
	NLogServerInfoManager();
	~NLogServerInfoManager();

	BOOL		Init();
	BOOL		LoadServerInfo();

	//===============================================================
	// INLINES
	//===============================================================
	inline NLogServerInfo*	GetAtIndex(SI32 siIndex)
	{
		if(siIndex < 0 || siIndex >= MAX_SERVER_COUNT)	return NULL;
		return m_pkData[siIndex];
	}

	inline SI32				GetCount()
	{
		return m_siCount;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_SERVER_COUNT;
	}

	NLogServerInfo* FindLogServerInfo(TCHAR* pszServerName);
};

#endif
