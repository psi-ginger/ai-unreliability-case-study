// SkillLevelUpEffect.h: interface for the CSkillLevelUpEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLLEVELUPEFFECT_H__0568FA0F_3D8F_4E9E_9C47_C02D9B0BA550__INCLUDED_)
#define AFX_SKILLLEVELUPEFFECT_H__0568FA0F_3D8F_4E9E_9C47_C02D9B0BA550__INCLUDED_

#include <Directives.h>
#include <time.h>
#include "TSpr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillLevelUpEffect  
{
public:
	CSkillLevelUpEffect( TSpr *pXSPR,TSpr * pXSPR_Eff );
	virtual ~CSkillLevelUpEffect();

	void	Initialize();
	void	Destroy();

	void	Set();
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:
	TSpr	*m_pSkillLevelUpEffectXSPR;
	TSpr	*m_pSkillLevelUpEffectXSPR_Eff;

	bool	m_bSkillLevelUpStatus;

	SI32	m_si32CurrentDrawFrame;
	SI32	m_si32SprImageNum;

	SI32	m_si32DelayCount;

	clock_t	m_delayClock;


};

#endif // !defined(AFX_SKILLLEVELUPEFFECT_H__0568FA0F_3D8F_4E9E_9C47_C02D9B0BA550__INCLUDED_)
