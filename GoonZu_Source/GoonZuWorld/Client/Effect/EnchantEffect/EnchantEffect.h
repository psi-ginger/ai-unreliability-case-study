#pragma once

#include <Directives.h>
#include <time.h>

#include "TSpr.h"

#define ENCHANT_EFFECT_NONE			0
#define ENCHANT_EFFECT_HERO_WEPONE	1
#define ENCHANT_EFFECT_HERO_AMOR	2
#define ENCHANT_EFFECT_HEROINE_WEPONE	3
#define ENCHANT_EFFECT_HEROINE_AMOR		4


class CEnchantEff
{
public:
	CEnchantEff( TSpr *pXSPR_EnchantHeroWe , TSpr *pXSPR_EnchantHeroAm ,TSpr *pXSPR_EnchantHeroineWe ,TSpr *pXSPR_EnchantHeroineAm);
	~CEnchantEff();

	void	Initialize();
	void	Destroy();


	void	Set(SI32 EffectType);
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos , SI32 siAnimation );

private:
	TSpr	*m_pEnchantHeroWeXSPR;
	TSpr	*m_pEnchantHeroAmXSPR;
	
	TSpr	*m_pEnchantHeroineWeXSPR;
	TSpr	*m_pEnchantHeroineAmXSPR;

	SI32	m_si32CurrentDrawFrame;

	SI32	m_si32SprImageNum;

	SI32	m_si32DelayCount;

	bool	m_bActive;

	SI32	m_siEffectType;

	clock_t	m_delayClock;
	
};
