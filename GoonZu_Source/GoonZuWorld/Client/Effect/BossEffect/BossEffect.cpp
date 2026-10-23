#include "BossEffect.h"
#include <Graphic.h>
#include "..\..\..\Server\Rank\Rank.h"
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

#define	MAX_BOSSEFFECT_DELAY_FRAME		3

CBossEffect::CBossEffect( TSpr *pBigBossEffectTSPR, TSpr *pSmallBossEffectTSPR )
{
	if ( pBigBossEffectTSPR == NULL || pSmallBossEffectTSPR == NULL )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("이미지 없음"), TEXT("Boss Image"), MB_OK |MB_TOPMOST);
#endif
	}
	else
	{
		m_pBigBossEffectTSPR = pBigBossEffectTSPR;
		m_pSmallBossEffectTSPR = pSmallBossEffectTSPR;

		m_siBigBossSprImageNum = m_pBigBossEffectTSPR->GetImageNum() ;
		m_siSmallBossSprImageNum = m_pSmallBossEffectTSPR->GetImageNum() ;
		m_siCurrentFrame = 0;
		m_siBigBossMaxFrameNum = m_pBigBossEffectTSPR->GetImageNum() ;
		m_siSmallBossMaxFrameNum = m_pSmallBossEffectTSPR->GetImageNum() ;

		m_bBossEffectStatus = false;
		m_siBossType = 0;
		m_delayClock = pclClient->CurrentClock;
	}
}

CBossEffect::~CBossEffect()
{
}

void CBossEffect::Set( SI32 BossType )
{
	m_siCurrentFrame = 0;
	m_siDelayFrame = 0;
	m_bBossEffectStatus = true;
	m_siBossType = BossType;
	m_siDelayFrame = 0;

	return;
}


bool CBossEffect::Action()
{
	if ( ! m_bBossEffectStatus || m_siBossType == 0 )
	{
		return true;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;


		if( ++m_siDelayFrame == MAX_BOSSEFFECT_DELAY_FRAME )
		{
			m_siDelayFrame  = 0;

			if ( m_siBossType == RANKTYPE_CITYHALL )
			{
				if ( ++m_siCurrentFrame == m_siBigBossMaxFrameNum )
				{
					m_siCurrentFrame = 0;
					//m_bBossEffectStatus = false;

					return true;
				}
			}
			else
			{
				if ( ++m_siCurrentFrame == m_siSmallBossMaxFrameNum )
				{
					m_siCurrentFrame = 0;
					//m_bBossEffectStatus = false;

					return true;
				}
			}
		}
	}

	
	return false;
}


void CBossEffect::Draw( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	if ( ! m_bBossEffectStatus || m_siBossType == 0 )
	{
		return;
	}

	if ( m_siBossType == RANKTYPE_CITYHALL )
	{
		SI32 XPos = si32CenterXPos - m_pBigBossEffectTSPR->GetXSize() / 2;
		SI32 YPos = si32CenterYPos - m_pBigBossEffectTSPR->GetYSize() / 2;

		SI32 width = m_pBigBossEffectTSPR->GetHRes();
		SI32 height = m_pBigBossEffectTSPR->GetVRes();

		GP.PutSprLight1(m_pBigBossEffectTSPR, XPos, YPos, m_siCurrentFrame );
	}
	else
	{
		SI32 XPos = si32CenterXPos - m_pSmallBossEffectTSPR->GetXSize() / 2;
		SI32 YPos = si32CenterYPos - m_pSmallBossEffectTSPR->GetYSize() / 2;

		SI32 width = m_pSmallBossEffectTSPR->GetHRes();
		SI32 height = m_pSmallBossEffectTSPR->GetVRes();

		GP.PutSprLight1(m_pSmallBossEffectTSPR, XPos, YPos, m_siCurrentFrame );
	}
	
}




bool CBossEffect::GetBossEffectStatus()
{
	return m_bBossEffectStatus;
}
