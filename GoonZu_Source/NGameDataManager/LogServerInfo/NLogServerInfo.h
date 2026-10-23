//=============================================================================
//
//=============================================================================
#ifndef _NLOGSERVERINFO_H_
#define _NLOGSERVERINFO_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogServerInfo
{
public:
	TCHAR		m_szSeverName[128];
	TCHAR		m_szTableName[128];
	TCHAR		m_szIP[24];
	UI16		m_uiPort;

public:
	NLogServerInfo()
	{
		m_szSeverName[0]	= '\0';
		m_szTableName[0]	= '\0';
		m_szIP[0]			= '\0';
		m_uiPort			= 0;
	}

	~NLogServerInfo()
	{
	}

	void Set()
	{

	}
};

#endif
