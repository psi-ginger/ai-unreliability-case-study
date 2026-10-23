//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMANDFILTER_H_
#define _NLOGCOMMANDFILTER_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogCommandFilter
{
public:
	TCHAR		m_szFilterName[64];
	TCHAR		m_szFilter[512];

public:
	NLogCommandFilter()
	{
		m_szFilterName[0]	= '\0';
		m_szFilter[0]		= '\0';
	}

	~NLogCommandFilter()
	{
	}

	void Set()
	{

	}
};

#endif
