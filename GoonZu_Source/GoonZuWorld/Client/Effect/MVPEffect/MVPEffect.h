#ifndef _MVPEFFECT_H_
#define _MVPEFFECT_H_

#include <Directives.h>
#include <time.h>
#include "TSpr.h"
#include "../../../Lib/Graphic/Graphic_Capsule.h"

class CMVPEffect
{
public:
	CMVPEffect( TSpr *pXSPR, TSpr *pAroundXSPR );
	~CMVPEffect();

	void	Initialize();
	void	Destroy();

	void	Set();
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:

	TSpr	*m_pMVPEffectXSPR;
	TSpr	*m_PMVPAroundEffectXSPR;

	bool	m_bMVPStatus;

	SI32	m_si32CurrentDrawFrame;

	SI32	m_si32SprImageNum_MVP;
	SI32	m_si32SprImageNum_Around;

	SI32	m_si32DelayCount;

	clock_t	m_delayClock;

};

#endif
