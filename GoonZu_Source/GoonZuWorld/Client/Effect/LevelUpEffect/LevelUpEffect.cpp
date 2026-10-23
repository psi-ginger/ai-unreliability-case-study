#include "LevelUpEffect.h"
#include <Graphic.h>

#include "CommonLogic.h"

CLevelUpEffect::CLevelUpEffect( TSpr *pXSPR ,TSpr * pXSPR_Eff)
{
	if ( pXSPR != NULL && pXSPR != NULL)
	{
		m_pLevelUpEffectXSPR_Eff = pXSPR_Eff;
		m_pLevelUpEffectXSPR = pXSPR;
		m_si32SprImageNum = pXSPR->GetImageNum();
		 
		m_si32DelayCount = 0;
	}

	Initialize();
}

CLevelUpEffect::~CLevelUpEffect()
{
	Destroy();
}

void CLevelUpEffect::Initialize()
{
	m_si32CurrentDrawFrame = 0;
	m_bLevelUpStatus = false;	
	m_si32DelayCount = 0;
	m_levelClock = pclClient->CurrentClock;
}

void CLevelUpEffect::Destroy()
{
}

void CLevelUpEffect::Set()
{
	m_bLevelUpStatus = true;
	m_si32CurrentDrawFrame = 0;
	return;
}

void CLevelUpEffect::Action()
{
	return;
}

void CLevelUpEffect::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
 	if ( m_bLevelUpStatus == false) return;

	if (TABS(pclClient->CurrentClock - m_levelClock ) > 50)
	{
		m_levelClock = pclClient->CurrentClock;

		if( m_si32CurrentDrawFrame >= m_si32SprImageNum -1 )
		{
			m_si32CurrentDrawFrame = 0;
			m_bLevelUpStatus = false;
		}
		else
		{
			m_si32CurrentDrawFrame++;
		}
	}

	GP.PutSpr(	m_pLevelUpEffectXSPR, 
				siCenterXPos - m_pLevelUpEffectXSPR->GetXSize() / 2, 
				siCenterYPos - m_pLevelUpEffectXSPR->GetYSize() / 2 - 45,
				m_si32CurrentDrawFrame  ); 

	GP.PutSprScreen(m_pLevelUpEffectXSPR_Eff, 
					siCenterXPos - m_pLevelUpEffectXSPR_Eff->GetXSize() / 2, 
					siCenterYPos - m_pLevelUpEffectXSPR_Eff->GetYSize() / 2  - 45, 
					m_si32CurrentDrawFrame  );		
	
	return;
}

void CLevelUpEffect::DrawNpc( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_bLevelUpStatus == false) return;

	if (TABS(pclClient->CurrentClock - m_levelClock ) > 50)
	{
		m_levelClock = pclClient->CurrentClock;

		if( m_si32CurrentDrawFrame >= m_si32SprImageNum -1 )
		{
			m_si32CurrentDrawFrame = 0;
			m_bLevelUpStatus = false;
		}
		else
		{
			m_si32CurrentDrawFrame++;
		}
	}

	GP.PutSpr(	m_pLevelUpEffectXSPR, 
		siCenterXPos - m_pLevelUpEffectXSPR->GetXSize() / 2, 
		siCenterYPos - m_pLevelUpEffectXSPR->GetYSize() + 70,   
		m_si32CurrentDrawFrame  ); 

	GP.PutSprScreen(m_pLevelUpEffectXSPR_Eff, 
		siCenterXPos - m_pLevelUpEffectXSPR_Eff->GetXSize() / 2, 
		siCenterYPos - m_pLevelUpEffectXSPR_Eff->GetYSize() + 70,   
		m_si32CurrentDrawFrame  );		

	return;
}
