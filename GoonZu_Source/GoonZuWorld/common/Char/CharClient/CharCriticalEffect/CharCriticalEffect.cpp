#include "CharCriticalEffect.h"
#include "Graphic_Capsule.h"


CCharCriticalEffect::CCharCriticalEffect( TSpr *p1XSPR )
{	
#ifdef _DEBUG
	if ( p1XSPR == NULL  )
	{
		MessageBox( NULL, TEXT("CCharCriticalEffect Image == NULL"), TEXT("error"), MB_OK|MB_TOPMOST );
	}
#endif

	m_pCriticalEffectXSPR = p1XSPR;
	

	m_cCurrentFrame = 0;
	m_cStatus = NOT_CRITICALEFFECT_STATUS;

}

CCharCriticalEffect::~CCharCriticalEffect()
{
}


void CCharCriticalEffect::SetAction()
{
	if ( m_cStatus == NOT_CRITICALEFFECT_STATUS )
	{		
		m_cCurrentFrame = 0;
		m_cStatus = CRITICALEFFECT_STATUS;
	}
	
	return;
}

void CCharCriticalEffect::Action()
{
	if ( m_cStatus == NOT_CRITICALEFFECT_STATUS )
	{
		return;
	}

	m_cCurrentFrame++;

	if ( m_cCurrentFrame == m_pCriticalEffectXSPR->GetImageNum() )
	{
		m_cStatus = NOT_CRITICALEFFECT_STATUS;
		m_cCurrentFrame = 0;
	}
	return;
}


void CCharCriticalEffect::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_cStatus != CRITICALEFFECT_STATUS )
	{
		return;
	}

	GP.PutSprScreen(m_pCriticalEffectXSPR, siCenterXPos - m_pCriticalEffectXSPR->GetXSize() / 2, 
			siCenterYPos - m_pCriticalEffectXSPR->GetYSize() / 2, m_cCurrentFrame );


	/*
	// 마지막 프레임에서 페이드 아웃을 줘야 해서리...
	if ( m_cCurrentFrame == m_pCriticalEffectXSPR->clHeader.siFontNum - 1 )
	{
		m_pCriticalEffectXSPR->PutSprLight1T( siCenterXPos - m_pCriticalEffectXSPR->clHeader.siXsize / 2, 
			siCenterYPos - m_pCriticalEffectXSPR->clHeader.siYsize / 2,
			m_cCurrentFrame );
	}
	else
	{
		m_pCriticalEffectXSPR->PutSprLight1T( siCenterXPos - m_pCriticalEffectXSPR->clHeader.siXsize / 2, 
			siCenterYPos - m_pCriticalEffectXSPR->clHeader.siYsize / 2,
			m_cCurrentFrame );
	}
	m_pCriticalEffectXSPR->PutSprT( siCenterXPos - m_pCriticalEffectXSPR->clHeader.siXsize / 2, 
			siCenterYPos - m_pCriticalEffectXSPR->clHeader.siYsize / 2,
			m_cCurrentFrame );
	*/

	
	//m_pCriticalEffectXSPR->PutSprLight1T( siCenterXPos - m_pCriticalEffectXSPR->clHeader.siXsize / 2, 
	//		siCenterYPos - m_pCriticalEffectXSPR->clHeader.siYsize / 2, m_cCurrentFrame );




	return;
}