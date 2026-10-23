//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDINDEX_H_
#define _NLOGCOMMANDINDEX_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogCommandIndex
{
public:
	TCHAR		m_szCommand[50];
	SI32		m_siIndex;
	SI32		m_siType;

public:
	NLogCommandIndex()
	{
		m_szCommand[0]		= '\0';
		m_siIndex			= 0;
		m_siType			= 0;
	}

	~NLogCommandIndex()
	{
	}

	void Set()
	{

	}
};

#endif
