#include "ValentineWarEffect.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/Char/CharCommon/Char-Common.h"
#include "../common/Char/CharClient/Char-Client.h"

CValentineWarEffect::CValentineWarEffect( TSpr *pValentineWarEffect )
{
	m_pValentineWarEffectTSPR = pValentineWarEffect;

	m_siValentineWarSprImageNum = pValentineWarEffect->GetImageNum();

	m_siDelayFrame = 3;

	m_siCurrentFrame = 0;

	m_ValentineWarClock  = 0;

	siCouple = 0;

}
CValentineWarEffect::~CValentineWarEffect()
{

}

void CValentineWarEffect::Action( SI32 siCouple )	// 하트이펙트를 그려야 할지 말지를 인자로 받음
{
	if( siCouple <= 0 ) return; // 그려지지 않는 상황이라면 액션 없다.

	if (TABS(pclClient->CurrentClock - m_ValentineWarClock ) > 50)
	{
		m_ValentineWarClock = pclClient->CurrentClock;

		if( m_siCurrentFrame > m_siValentineWarSprImageNum -1 )
		{
			m_siCurrentFrame = 0;
		}
		else
		{
			m_siCurrentFrame++;
		}
	}

}

void CValentineWarEffect::Draw( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	SI32 XPos= 0,YPos=0;
	if( siCouple <= 0 ) return; // 그려지지 않는 상황이라면 엑션 없다.
	
	XPos = si32CenterXPos - m_pValentineWarEffectTSPR->GetXSize() / 2+1;

	if (pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE== true) {//현제 타고 있는 말의 인덱스가 없으면 말안탄거니
		YPos = si32CenterYPos - m_pValentineWarEffectTSPR->GetYSize() / 2-2;
	}
	else{
		YPos = si32CenterYPos - m_pValentineWarEffectTSPR->GetYSize() / 2-16;
	}	
	GP.PutSpr(m_pValentineWarEffectTSPR, XPos, YPos, m_siCurrentFrame );    
}


