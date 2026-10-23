//=============================================================================
//
//=============================================================================
#ifndef _NVILLAGEGAMEDATA_H_
#define _NVILLAGEGAMEDATA_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NVillageGameData
{
public:
	SI32		m_siVillageUnique;
	TCHAR		m_szName[128];

public:
	NVillageGameData()
	{
		m_siVillageUnique	= 0;
		m_szName[0]			= '\0';
	}

	~NVillageGameData()
	{
	}

	void Set()
	{

	}
};

#endif
