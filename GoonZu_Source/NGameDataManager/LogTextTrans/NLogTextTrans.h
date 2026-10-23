//=============================================================================
//
//=============================================================================
#ifndef _NLOGTEXTTRANS_H_
#define _NLOGTEXTTRANS_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

class NLogTextTrans
{
public:
	TCHAR		m_szCommand[128];	// LogText의 커맨드종류
	TCHAR		m_szType[128];		// LogText의 타입
	SI32		m_siIndex;			// 텍스트로 변환할 Index
	TCHAR		m_szText[128];		// 변환될 텍스트

public:
	NLogTextTrans()
	{
		m_szCommand[0]	= '\0';
		m_szType[0]	= '\0';
		m_siIndex = 0;
		m_szText[0] = '\0';
	}

	~NLogTextTrans()
	{
	}

	void Set()
	{

	}
};

#endif
