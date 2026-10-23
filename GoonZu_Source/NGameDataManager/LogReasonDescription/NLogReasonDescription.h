//=============================================================================
//
//=============================================================================
#ifndef _NLOGREASONDESCRIPTION_H_
#define _NLOGREASONDESCRIPTION_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogReasonDescription
{
public:
	TCHAR		m_szCommand[50];
	SI32		m_siIndex;
	SI32		m_siType;
	SI32		m_siReason;
	TCHAR		m_szDescription[2048];

public:
	NLogReasonDescription()
	{
		m_szCommand[0]			= '\0';
		m_siIndex				= 0;
		m_siType				= 0;
		m_szDescription[0]		= '\0';
	}

	~NLogReasonDescription()
	{
	}

	void Set()
	{

	}
};

#endif
