#include "FourLeafEffect.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/Char/CharCommon/Char-Common.h"
#include "../common/Char/CharClient/Char-Client.h"

CFourLeafEffect::CFourLeafEffect( TSpr *pFourLeafEffect )
{
	m_pFourLeafEffectTSPR = pFourLeafEffect;

	m_siFourLeafSprImageNum = pFourLeafEffect->GetImageNum();

	m_siDelayFrame = 3;

	m_siCurrentFrame = 0;

	m_FourLeafClock  = 0;

	m_bFourLeafEffect = false;

}
CFourLeafEffect::~CFourLeafEffect()
{

}

void CFourLeafEffect::Action( bool bFourLeafEffect )	// 하트이펙트를 그려야 할지 말지를 인자로 받음
{
	m_bFourLeafEffect = bFourLeafEffect;

	if( m_bFourLeafEffect == false ) return; // 그려지지 않는 상황이라면 엑션 없다.

	if (TABS(pclClient->CurrentClock - m_FourLeafClock ) > 50)
	{
		m_FourLeafClock = pclClient->CurrentClock;

		if( m_siCurrentFrame > m_siFourLeafSprImageNum -1 )
		{
			m_siCurrentFrame = 0;
		}
		else
		{
			m_siCurrentFrame++;
		}
	}

}

void CFourLeafEffect::Draw( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	SI32 XPos= 0,YPos=0;
	if ( m_bFourLeafEffect == false )	return; // 그려지지 않는 상황이라면  그리기 없다.
	
	XPos = si32CenterXPos - m_pFourLeafEffectTSPR->GetXSize() / 2+1;

//KHY - 1001 - 기승동물 5마리 보유 수정.
//	if (pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseIndex!= NULL) {//현제 타고 있는 말의 인덱스가 없으면 말안탄거니
	if (pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE== true) {//현제 타고 있는 말의 인덱스가 없으면 말안탄거니
		YPos = si32CenterYPos - m_pFourLeafEffectTSPR->GetYSize() / 2-2;
	}
	else{
		YPos = si32CenterYPos - m_pFourLeafEffectTSPR->GetYSize() / 2-16;
	}//은아씨 요청에 의해서 말을 탓을 경우 말탄만큼 네잎클로버의 위치를 변화줌.[2007.07.26 손성웅]
	
	GP.PutSpr(m_pFourLeafEffectTSPR, XPos, YPos, m_siCurrentFrame );    
}


