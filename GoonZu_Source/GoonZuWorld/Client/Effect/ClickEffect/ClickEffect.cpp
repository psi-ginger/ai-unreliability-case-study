#include "ClickEffect.h"
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

CClickEffect::CClickEffect( TSpr *pClickEffectSpr )
{
	m_siXPos = 0;
	m_siYPos = 0;

	m_siCurrentFrame = 0;
	m_siMaxFrame = pClickEffectSpr->GetImageNum() / 2;
	m_delayClock = pclClient->CurrentClock;

}

CClickEffect::~CClickEffect()
{
}

void CClickEffect::Set( SI32 XPos, SI32 YPos )
{
	m_siXPos = XPos;
	m_siYPos = YPos;

	m_siCurrentFrame = 0;
	return;
}

void CClickEffect::Action()
{
	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
        m_delayClock = pclClient->CurrentClock;

		if ( m_siCurrentFrame++ >= m_siMaxFrame )
		{
			m_siCurrentFrame = 0;
		}
	}

	return;
}

void CClickEffect::Draw( SI32 XPos, SI32 YPos )
{
	GP.PutSpr(m_pClickEffectSpr, m_siXPos -  m_pClickEffectSpr->GetXSize() / 2, m_siYPos - m_pClickEffectSpr->GetYSize() / 2, m_siCurrentFrame );
	GP.PutSprScreen(m_pClickEffectSpr, m_siXPos -  m_pClickEffectSpr->GetXSize() / 2, m_siYPos - m_pClickEffectSpr->GetYSize() / 2, m_siCurrentFrame + m_siMaxFrame );

	return;
}