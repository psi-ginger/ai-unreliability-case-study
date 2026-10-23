#ifndef _CHARPARTYEMOTICON_H_
#define _CHARPARTYEMOTICON_H_

#include <Directives.h>
#include "TSpr.h"

class CCharPartyEmoticon
{
public:
	CCharPartyEmoticon();
	~CCharPartyEmoticon();
	
	void	Initialize();
	void	Draw( TSpr *pXSPR, SI32 XPos, SI32 YPos );

private:		
	char	m_cCurrentFrame;
	char	m_cCurrentDelayFrame;
};

#endif