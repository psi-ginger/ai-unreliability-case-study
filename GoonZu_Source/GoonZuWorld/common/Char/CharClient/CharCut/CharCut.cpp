#include "CharCut.h"
#include "../Lib/Graphic/Graphic_Capsule.h"
#include "../Client/GlobalImgFile/GlobalImgFile.h"
#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

CCharCut::CCharCut()
{	
//	m_pCutXSPR[0] = pclClient->GetGImgPointer(GIMG_CUT);
//	m_pCutXSPR[1] = pclClient->GetGImgPointer(GIMG_NEW_ATTACK_STAFF);
//	m_pCutXSPR[2] = pclClient->GetGImgPointer(GIMG_NEW_ATTACK_SWORD);
//	m_pCutXSPR[3] = pclClient->GetGImgPointer(GIMG_NEW_ATTACK_SPEAR);
//	m_pCutXSPR[4] = pclClient->GetGImgPointer(GIMG_NEW_ATTACK_AXE);
//	m_pCutXSPR[5] = pclClient->GetGImgPointer(GIMG_NEW_ATTACK_BOWGUN);
	
	m_pCutXSPR = NULL;
    m_cCurrentFrame = 0;
	m_cCutStatus = NOT_CUT_STATUS;
	m_siMode = 0;

}

CCharCut::~CCharCut()
{
}


void CCharCut::SetAction(SI32 mode)
{
	if ( m_cCutStatus == NOT_CUT_STATUS )
	{		
		m_siMode = mode ;
		m_cCurrentFrame = 0;
		m_cCutStatus = CUT_STATUS;

		switch(m_siMode)
		{
		case 0:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_CUT);
			break;
		case 1:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_STAFF);
			break;
		case 2:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_SWORD);
			break;
		case 3:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_SPEAR);
			break;
		case 4:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_AXE);
			break;
		case 5:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_BOWGUN);
			break;
		case 6:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_STAFF2);
			break;
		case 7:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_SWORD2);
			break;
		case 8:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_SPEAR2);
			break;
		case 9:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_AXE2);
			break;
		case 10:
			m_pCutXSPR = pclClient->GetGlobalSpr(GIMG_NEW_ATTACK_BOWGUN2);
			break;
		}
	}

	return;
}

void CCharCut::Action()
{
	if ( m_cCutStatus == NOT_CUT_STATUS )
	{
		return;
	}

	m_cCurrentFrame++;

	if ( m_cCurrentFrame == m_pCutXSPR->GetImageNum() )
	{
		m_cCutStatus = NOT_CUT_STATUS;
	}
	return;
}


void CCharCut::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_cCutStatus != CUT_STATUS )
	{
		return;
	}

	// 마지막 프레임에서 페이드 아웃을 줘야 해서리...
	if ( m_cCurrentFrame == m_pCutXSPR->GetImageNum() - 1 )
	{
		
		GP.PutSprLight1(m_pCutXSPR, siCenterXPos - m_pCutXSPR->GetXSize() / 2, 
			siCenterYPos - m_pCutXSPR->GetYSize() / 2,
			m_cCurrentFrame );
	}
	else
	{
		GP.PutSprLight1(m_pCutXSPR,  siCenterXPos - m_pCutXSPR->GetXSize() / 2, 
			siCenterYPos - m_pCutXSPR->GetYSize() / 2,
			m_cCurrentFrame );
	}
	return;
}