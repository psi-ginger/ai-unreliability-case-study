#ifndef _CHARCRITICALEFFECT_H_
#define _CHARCRITICALEFFECT_H_

#include <Directives.h>
#include "TSpr.h"

enum { CRITICALEFFECT_STATUS, NOT_CRITICALEFFECT_STATUS };

class CCharCriticalEffect
{
public:
	CCharCriticalEffect( TSpr * );
	~CCharCriticalEffect();

	void	SetAction();
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:	
	TSpr	*m_pCriticalEffectXSPR;
	char	m_cCurrentFrame;
	
	char	m_cStatus;	
};

#endif