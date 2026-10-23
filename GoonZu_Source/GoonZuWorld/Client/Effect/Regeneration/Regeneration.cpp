#include "Regeneration.h"
#include "CommonLogic.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"

#define	MAX_REGENERATION_DELAY_FRAME		1

CRegeneration::CRegeneration(  TSpr *pRegenerationTSPR  )
{
	if ( pRegenerationTSPR == NULL )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("이미지 없음"), TEXT("pRegenerationTSPR"), MB_OK |MB_TOPMOST);
#endif
	}
	else
	{
		m_pRegenerationTSPR = pRegenerationTSPR;
		m_siSprImageNum = m_pRegenerationTSPR->GetImageNum();
		m_siCurrentFrame = 0;
		m_siMaxFrameNum = m_pRegenerationTSPR->GetImageNum() / 2;

		m_bRegenerationStatus = false;
	}

	m_delayClock = pclClient->CurrentClock;
}

CRegeneration::~CRegeneration()
{
}

void CRegeneration::Set()
{
	m_siCurrentFrame = 0;
	m_siDelayFrame = 0;
	m_bRegenerationStatus = true;
	return;
}


bool CRegeneration::Action()
{
	if ( ! m_bRegenerationStatus )
	{
		return true;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;


		if( ++m_siDelayFrame == MAX_REGENERATION_DELAY_FRAME )
		{
			m_siDelayFrame  = 0;

			if ( ++m_siCurrentFrame == m_siMaxFrameNum )
			{
				m_siCurrentFrame = 0;
				m_bRegenerationStatus = false;

				return true;
			}		
		}
	}


	
	return false;
}


void CRegeneration::DrawBottom( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	if ( ! m_bRegenerationStatus )
	{
		return;
	}

	SI32 XPos = si32CenterXPos - m_pRegenerationTSPR->GetXSize() / 2;
	SI32 YPos = si32CenterYPos - m_pRegenerationTSPR->GetYSize() / 2;

	SI32 width = m_pRegenerationTSPR->GetHRes();
	SI32 height = m_pRegenerationTSPR->GetVRes();

	GP.PutSprLight1(m_pRegenerationTSPR, XPos, YPos, m_siCurrentFrame + m_siMaxFrameNum );
	
}

void CRegeneration::DrawCenter( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	if ( ! m_bRegenerationStatus )
	{
		return;
	}

	SI32 XPos = si32CenterXPos - m_pRegenerationTSPR->GetXSize() / 2;
	SI32 YPos = si32CenterYPos - m_pRegenerationTSPR->GetYSize() / 2;

	SI32 width = m_pRegenerationTSPR->GetHRes();
	SI32 height = m_pRegenerationTSPR->GetVRes();

	GP.PutSprLight1(m_pRegenerationTSPR, XPos, YPos, m_siCurrentFrame );

	
}

bool CRegeneration::GetRegenerationStatus()
{
	return m_bRegenerationStatus;
}
