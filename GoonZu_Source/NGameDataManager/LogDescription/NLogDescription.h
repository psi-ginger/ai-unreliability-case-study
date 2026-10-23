//=============================================================================
//
//=============================================================================
#ifndef _NLOGDESCRIPTION_H_
#define _NLOGDESCRIPTION_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogDescription
{
public:
	TCHAR		m_szCommand[50];
	SI32		m_siIndex;
	SI32		m_siType;
	TCHAR		m_szDescription[2048];

public:
	NLogDescription()
	{
		m_szCommand[0]		= '\0';
		m_siIndex			= 0;
		m_siType			= 0;
		m_szDescription[0]	= '\0';
	}

	~NLogDescription()
	{
	}

	void Set()
	{

	}
};

#endif
