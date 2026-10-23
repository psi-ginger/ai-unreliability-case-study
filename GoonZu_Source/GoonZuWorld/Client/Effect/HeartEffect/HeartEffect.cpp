#include "HeartEffect.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

CHeartEffect::CHeartEffect( TSpr *pHeartEffect )
{
	m_pHeartEffectTSPR = pHeartEffect;

	m_siHeartSprImageNum = pHeartEffect->GetImageNum();

	m_siDelayFrame = 3;

	m_bHeartEffect = false;

	m_siCurrentFrame = 0;

	m_HeartClock  = 0;

}
CHeartEffect::~CHeartEffect()
{

}

void CHeartEffect::Action( bool bHeartEffect )	// 하트이펙트를 그려야 할지 말지를 인자로 받음
{
	m_bHeartEffect = bHeartEffect;
	
	if( m_bHeartEffect == false ) return; // 그려지지 않는 상황이라면 엑션 없다.
		
	if (TABS(pclClient->CurrentClock - m_HeartClock ) > 50)
	{
		m_HeartClock = pclClient->CurrentClock;

		if( m_siCurrentFrame > m_siHeartSprImageNum -1 )
		{
			m_siCurrentFrame = 0;
		}
		else
		{
			m_siCurrentFrame++;
		}
	}

}

void CHeartEffect::Draw( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	if ( m_bHeartEffect == false )	return; // 그려지지 않는 상황이라면  그리기 없다.

 	SI32 XPos = si32CenterXPos - m_pHeartEffectTSPR->GetXSize() / 2;
	SI32 YPos = si32CenterYPos - m_pHeartEffectTSPR->GetYSize() / 2 + 5;

	GP.PutSprScreen(m_pHeartEffectTSPR, XPos, YPos, m_siCurrentFrame );    
}


