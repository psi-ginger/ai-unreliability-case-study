// SkillLevelUpEffect.cpp: implementation of the CSkillLevelUpEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "SkillLevelUpEffect.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"

#include "CommonLogic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillLevelUpEffect::CSkillLevelUpEffect( TSpr *pXSPR,TSpr * pXSPR_Eff )
{
	if ( pXSPR != NULL && pXSPR_Eff != NULL )
	{
		m_pSkillLevelUpEffectXSPR = pXSPR;
		m_pSkillLevelUpEffectXSPR_Eff = pXSPR_Eff;

		m_si32SprImageNum = pXSPR->GetImageNum();
		

		m_si32DelayCount = 0;
	}

	Initialize();
}

CSkillLevelUpEffect::~CSkillLevelUpEffect()
{
}

void CSkillLevelUpEffect::Initialize()
{
	m_si32CurrentDrawFrame = 0;
	m_bSkillLevelUpStatus = false;	
	m_si32DelayCount = 0;
	m_delayClock = pclClient->CurrentClock;

}

void CSkillLevelUpEffect::Destroy()
{
}

void CSkillLevelUpEffect::Set()
{
	m_bSkillLevelUpStatus = true;
	m_si32CurrentDrawFrame = 0;
	return;
}

void CSkillLevelUpEffect::Action()
{
	return;
}

void CSkillLevelUpEffect::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_bSkillLevelUpStatus == false) return;
	

		
	if (TABS(pclClient->CurrentClock - m_delayClock ) > 50)
	{
		m_delayClock = pclClient->CurrentClock;

		if( m_si32CurrentDrawFrame >= m_si32SprImageNum -1 )
		{
			m_si32CurrentDrawFrame = 0;
			m_bSkillLevelUpStatus = false;
		}
		else
		{
			m_si32CurrentDrawFrame++;
		}
	}

	GP.PutSpr(m_pSkillLevelUpEffectXSPR, siCenterXPos - m_pSkillLevelUpEffectXSPR->GetXSize() / 2, 
		siCenterYPos - m_pSkillLevelUpEffectXSPR->GetYSize() / 2 - 45 ,
		m_si32CurrentDrawFrame  );
	
	GP.PutSprScreen(m_pSkillLevelUpEffectXSPR_Eff, siCenterXPos - m_pSkillLevelUpEffectXSPR_Eff->GetXSize() / 2, 
		siCenterYPos - m_pSkillLevelUpEffectXSPR_Eff->GetYSize() / 2 - 45,
		m_si32CurrentDrawFrame  );		
		
	return;
}