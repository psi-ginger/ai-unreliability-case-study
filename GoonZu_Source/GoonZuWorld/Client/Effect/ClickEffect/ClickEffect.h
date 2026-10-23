#ifndef _CLICKEFFECT_H_
#define _CLICKEFFECT_H_

#include <Directives.h>
#include <Graphic.h>
#include <time.h>
#include "TSpr.h"

class CClickEffect
{
public:
	CClickEffect( TSpr *pClickEffectSpr );
	~CClickEffect();

	void Set( SI32 XPos, SI32 YPos );
	void Action();
	void Draw( SI32 XPos, SI32 YPos );

private:
	TSpr *m_pClickEffectSpr;
	SI32	m_siCurrentFrame;
	SI32	m_siMaxFrame;
	SI32	m_siXPos;
	SI32	m_siYPos;


	clock_t	m_delayClock;
};

#endif