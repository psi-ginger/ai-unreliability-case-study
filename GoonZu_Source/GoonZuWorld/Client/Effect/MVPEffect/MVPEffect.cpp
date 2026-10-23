#include "MVPEffect.h"
#include "CommonLogic.h"
#include <Graphic.h>

CMVPEffect::CMVPEffect( TSpr *pXSPR, TSpr *pAroundXSPR )
{
	if ( pXSPR != NULL )
	{
		m_pMVPEffectXSPR = pXSPR;
		m_PMVPAroundEffectXSPR = pAroundXSPR; 
		m_si32SprImageNum_MVP = pXSPR->GetImageNum();
		m_si32SprImageNum_Around = pAroundXSPR->GetImageNum();

		m_si32DelayCount = 0;
	}

	Initialize();
}

CMVPEffect::~CMVPEffect()
{
	Destroy();
}

void CMVPEffect::Initialize()
{
	m_si32CurrentDrawFrame = 0;
	m_bMVPStatus = false;	
	m_si32DelayCount = 0;
	m_delayClock = pclClient->CurrentClock;
}

void CMVPEffect::Destroy()
{
}

void CMVPEffect::Set()
{
	m_bMVPStatus = true;
	m_si32CurrentDrawFrame = 0;
	return;
}

void CMVPEffect::Action()
{
	return;
}

void CMVPEffect::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_bMVPStatus )
	{

		if ( m_si32SprImageNum_MVP - 2 < m_si32CurrentDrawFrame && m_si32SprImageNum_Around - 2 < m_si32CurrentDrawFrame)
		{
			if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
			{
				m_delayClock = pclClient->CurrentClock;

				m_si32CurrentDrawFrame++;

				if ( m_si32CurrentDrawFrame > 60 )
				{
					m_si32CurrentDrawFrame = 0;
					m_bMVPStatus = false;
				}

			}

		}
		else
		{
			if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
			{
				m_delayClock = pclClient->CurrentClock;

				if ( m_si32DelayCount++ == 1 )
				{
					m_si32DelayCount = 0;
					++m_si32CurrentDrawFrame;
				}
			}

			if ( m_si32SprImageNum_MVP - 2 >= m_si32CurrentDrawFrame)
			{

				GP.PutSprScreen(m_pMVPEffectXSPR, siCenterXPos - m_pMVPEffectXSPR->GetXSize() / 2, 
					siCenterYPos - m_pMVPEffectXSPR->GetYSize() / 2,
					m_si32CurrentDrawFrame  , false );

			}

			if ( m_si32SprImageNum_Around - 1 >= m_si32CurrentDrawFrame )
			{

				GP.PutSprScreen(m_PMVPAroundEffectXSPR, siCenterXPos - m_PMVPAroundEffectXSPR->GetXSize() / 2, 
					siCenterYPos - m_PMVPAroundEffectXSPR->GetYSize() / 2,
					m_si32CurrentDrawFrame , false );

			}

		}

	}

	return;
}