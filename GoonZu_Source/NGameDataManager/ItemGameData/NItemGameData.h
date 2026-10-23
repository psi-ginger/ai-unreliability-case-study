//=============================================================================
//
//=============================================================================
#ifndef _NITEMGAMEDATA_H_
#define _NITEMGAMEDATA_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NItemGameData
{
public:
	SI32		m_siUnique;
	TCHAR		m_szItemName[128];

public:
	NItemGameData()
	{
		m_szItemName[0]	= '\0';
	}

	~NItemGameData()
	{
	}

	void Set()
	{

	}
};

#endif
