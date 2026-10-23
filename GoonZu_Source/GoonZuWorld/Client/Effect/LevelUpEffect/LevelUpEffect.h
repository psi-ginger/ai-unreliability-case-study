#ifndef _LEVELUPEFFECT_H_
#define _LEVELUPEFFECT_H_

#include <Directives.h>
#include <time.h>
#include "TSpr.h"
#include "../../../Lib/Graphic/Graphic_Capsule.h"


class CLevelUpEffect
{
public:
	CLevelUpEffect( TSpr *pXSPR , TSpr *pXSPR_Eff );
	~CLevelUpEffect();

	void	Initialize();
	void	Destroy();

	void	Set();
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );
	void	DrawNpc( SI32 siCenterXPos, SI32 siCenterYPos );

private:

	TSpr	*m_pLevelUpEffectXSPR;
	TSpr	*m_pLevelUpEffectXSPR_Eff;

	bool	m_bLevelUpStatus;

	SI32	m_si32CurrentDrawFrame;
	SI32	m_si32SprImageNum;

	SI32	m_si32DelayCount;
	
	clock_t	m_levelClock;

};

#endif
