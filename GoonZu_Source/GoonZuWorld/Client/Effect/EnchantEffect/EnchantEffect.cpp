#include "EnchantEffect.h"
#include "CommonLogic.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"

#include "..\..\client.h"
extern cltCommonLogic* pclClient;

CEnchantEff::CEnchantEff( TSpr *pXSPR_EnchantHeroWe , TSpr *pXSPR_EnchantHeroAm ,TSpr *pXSPR_EnchantHeroineWe ,TSpr *pXSPR_EnchantHeroineAm )
{
	m_si32CurrentDrawFrame = 0;
	m_si32DelayCount = 0;

	m_si32SprImageNum = 0;

	m_pEnchantHeroWeXSPR = pXSPR_EnchantHeroWe;
	m_pEnchantHeroAmXSPR = pXSPR_EnchantHeroAm;
	m_pEnchantHeroineWeXSPR = pXSPR_EnchantHeroineWe;
	m_pEnchantHeroineAmXSPR = pXSPR_EnchantHeroineAm;

	m_bActive = false;

	Initialize();
}

CEnchantEff::~CEnchantEff()
{

}

void CEnchantEff::Initialize()
{
	m_si32CurrentDrawFrame = 0;
	m_si32DelayCount = 0;
	m_bActive = false;
	m_delayClock = pclClient->CurrentClock;

	return;
}

void CEnchantEff::Destroy()
{
	return;
}

void CEnchantEff::Set(SI32 EffectType)
{
	m_bActive = true;
	m_si32CurrentDrawFrame = 0;
	m_siEffectType = EffectType;

	if( m_siEffectType == ENCHANT_EFFECT_HERO_WEPONE )
	{
		m_si32SprImageNum =  m_pEnchantHeroWeXSPR->GetImageNum();
	}
	else if( m_siEffectType == ENCHANT_EFFECT_HERO_AMOR )
	{
		m_si32SprImageNum =  m_pEnchantHeroAmXSPR->GetImageNum();
	}
	else if( m_siEffectType == ENCHANT_EFFECT_HEROINE_WEPONE )
	{
		m_si32SprImageNum =  m_pEnchantHeroineWeXSPR->GetImageNum();
	}
	else if( m_siEffectType == ENCHANT_EFFECT_HEROINE_AMOR )
	{
		m_si32SprImageNum =  m_pEnchantHeroAmXSPR->GetImageNum();
	}
	else
	{
		m_bActive = false;
	}

	return;
}

void CEnchantEff::Action()
{
	
	return;
}

void CEnchantEff::Draw( SI32 siCenterXPos, SI32 siCenterYPos , SI32 siAnimation )
{
	if( m_bActive == false) return;
	
	// 제조 동작은 대기, 앉아있는 상태에서만 가능하다.
	if( ( siAnimation == ANITYPE_WAIT0 ||
		 siAnimation == ANITYPE_WAIT1 ||
		 siAnimation == ANITYPE_SITDOWN )	== false )    
		return;

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;

		if ( m_si32DelayCount++ == 2 )
		{
			m_si32DelayCount = 0;
			m_si32CurrentDrawFrame++;
		}

		if( m_si32CurrentDrawFrame >= m_si32SprImageNum )
		{
			m_si32CurrentDrawFrame = 0;
		}

	}

	TSpr * SprSrc = NULL;
	SI16   siAdjustX = 0;
	SI16   siAdjustY = 0;

	if( ENCHANT_EFFECT_HERO_WEPONE == m_siEffectType  )
	{
		SprSrc = m_pEnchantHeroWeXSPR;
		siAdjustX = -3;
		siAdjustY = 34;
	}
	else if( ENCHANT_EFFECT_HERO_AMOR == m_siEffectType  )
	{
		SprSrc = m_pEnchantHeroAmXSPR;
		siAdjustX = -3;
		siAdjustY = 34;
	}
	else if( ENCHANT_EFFECT_HEROINE_WEPONE == m_siEffectType  )
	{
		SprSrc = m_pEnchantHeroineWeXSPR;
		siAdjustX = 5;
		siAdjustY = 32;
	}
	else if( ENCHANT_EFFECT_HEROINE_AMOR == m_siEffectType  )
	{
		SprSrc = m_pEnchantHeroineAmXSPR;
		siAdjustX = 5;
		siAdjustY = 32;
	}
	else
	{
		SprSrc = NULL;
	}

	if( NULL == SprSrc  )
	{
		return;
	}


	GP.PutSprScreen(SprSrc, siCenterXPos - SprSrc->GetXSize() / 2 + siAdjustX, 
		siCenterYPos - SprSrc->GetYSize() / 2 + siAdjustY,
		m_si32CurrentDrawFrame  , false );

	return;
}
