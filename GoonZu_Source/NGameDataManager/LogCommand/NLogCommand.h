//=============================================================================
//
//=============================================================================
#ifndef _NLOGCOMMAND_H_
#define _NLOGCOMMAND_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogCommand
{
public:
	TCHAR		m_szCommand[50];

public:
	NLogCommand()
	{
		m_szCommand[0]	= '\0';
	}

	~NLogCommand()
	{
	}

	void Set()
	{

	}
};

#endif
