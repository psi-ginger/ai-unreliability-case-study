//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDTYPE_H_
#define _NLOGCOMMANDTYPE_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogCommandType
{
public:
	TCHAR		m_szCommand[50];
	SI32		m_siType;
	SI32		m_siIndexStart;

public:
	NLogCommandType()
	{
		m_szCommand[0]	= '\0';
		m_siType		= 0;
		m_siIndexStart	= -1;
	}

	~NLogCommandType()
	{
	}

	void Set(TCHAR* pszCommand, SI32 siType)
	{
        if(pszCommand)	_tcsncpy(m_szCommand, pszCommand, sizeof(m_szCommand));
		m_siType		= siType;
	}
};

#endif
