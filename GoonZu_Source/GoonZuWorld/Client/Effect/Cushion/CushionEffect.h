#ifndef _CUSHIONEFFECT_H_
#define _CUSHIONEFFECT_H_

#include <Directives.h>
#include <time.h>

#include "..\..\..\Common\SMLib\LibList\Obj.h"
#include "TSpr.h"

class	CCushionEffect
{
public:
	CCushionEffect( TSpr *pEffect1 , TSpr *pEffect2 , TSpr * pEffect3 , TSpr * pEffect4,TSpr * pEffect5,TSpr * pEffect6,TSpr * pEffect7,TSpr * pEffect8 );
	~CCushionEffect();

	void		Action( SI32 bEffect, SI32 charkind , SI32 direction  );	// 이펙트를 그려야 할지 말지를 인자로 받음
	void		Draw( SI32 si32CenterXPos, SI32	si32CenterYPos );	

private:
	TSpr		*m_pEffectTSPR[8];


	SI32		m_siDelayFrame;

	SI32		m_siCurrentFrame;

	clock_t		m_Clock;

	SI32		m_bEffect;

	/////

	SI32		m_siSelectedSpr;
	SI32		m_siSelectedFont;
	bool		m_bReverse;

	SI32	    m_siAdjustX;
	SI32	    m_siAdjustY;
};

#endif