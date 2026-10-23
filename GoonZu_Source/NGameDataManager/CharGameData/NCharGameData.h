//=============================================================================
//
//=============================================================================
#ifndef _NCHARGAMEDATA_H_
#define _NCHARGAMEDATA_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NCharKindGameData
{
public:
	SI32		m_siUnique;
	TCHAR		m_szKind[128];

public:
	NCharKindGameData()
	{
		m_siUnique		= 0;
		m_szKind[0]		= '\0';
	}

	~NCharKindGameData()
	{
	}

	void Set()
	{

	}
};

class NCharGameData
{
public:
	SI32		m_siUnique;
	TCHAR		m_szKind[128];
	TCHAR		m_szCharName[128];

public:
	NCharGameData()
	{
		m_siUnique		= 0;
		m_szKind[0]		= '\0';
		m_szCharName[0]	= '\0';
	}

	~NCharGameData()
	{
	}

	void Set()
	{

	}
};

#endif
