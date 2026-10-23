#include "QuestEffect.h"
#include "CommonLogic.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"

#include "..\..\client.h"
extern cltCommonLogic* pclClient;

CQuestEff::CQuestEff( TSpr *pXSPR_QuestReceive , TSpr *pXSPR_QuestClear )
{
	if( pXSPR_QuestReceive == NULL) return;
	if( pXSPR_QuestClear == NULL) return;

	m_pQuestReceiveXSPR = pXSPR_QuestReceive;
	m_pQuestClearXSPR = pXSPR_QuestClear;

	m_si32SprClearImageNum = m_pQuestClearXSPR->GetImageNum();
	m_si32SprReceiveImageNum = m_pQuestReceiveXSPR->GetImageNum();
	
	Initialize();
}

CQuestEff::~CQuestEff()
{

}

void CQuestEff::Initialize()
{
	m_delayClock = pclClient->CurrentClock;
	m_si32CurrentDrawFrame = 0;
	
	m_bReceive = FALSE;
	m_bClear	= FALSE;
	return;
}

void CQuestEff::Destroy()
{
	return;
}

void CQuestEff::SetReceive()
{	
	m_si32CurrentDrawFrame = 30;
	m_bReceive = TRUE;
	m_bClear	= FALSE;

	return;
}

void CQuestEff::SetClear()
{	
	m_si32CurrentDrawFrame = 0;
	m_bReceive = FALSE;
	m_bClear	= TRUE;

	return;
}

void CQuestEff::Action()
{

	return;
}

void CQuestEff::Draw( SI32 siCenterXPos, SI32 siCenterYPos)
{
	if( m_bReceive == FALSE && m_bClear == FALSE) return;

    //static clock_t QuestClock = pclClient->CurrentClock;


	if(TABS(pclClient->CurrentClock - m_delayClock) > 80)
	{
		m_delayClock = pclClient->CurrentClock;

	
		m_si32CurrentDrawFrame++;
	

		if( m_bReceive )
		{
			if( m_si32CurrentDrawFrame >= m_si32SprReceiveImageNum )
			{
				Initialize();
			}
		}
		else if( m_bClear)
		{
			if( m_si32CurrentDrawFrame >= m_si32SprClearImageNum )
			{
				Initialize();
			}
		}

	}

	TSpr * SprSrc = NULL;
	SI16   siAdjustX = 0;
	SI16   siAdjustY = 0;

	if( m_bReceive  )
	{
		SprSrc = m_pQuestReceiveXSPR;
		siAdjustX = -3;
		siAdjustY = 34;

	}
	else if( m_bClear  )
	{
		SprSrc = m_pQuestClearXSPR;
		siAdjustX = -3;
		siAdjustY = 34;
	}	
	else
	{
		SprSrc = NULL;
	}

	if( NULL == SprSrc  )
	{
		return;
	}

	if(m_bReceive)
	{
		GP.PutSpr(SprSrc, siCenterXPos - SprSrc->GetXSize() / 2 + siAdjustX, 
			siCenterYPos - SprSrc->GetYSize() / 2 + siAdjustY,
			m_si32CurrentDrawFrame-30 , false );
	}

	GP.PutSprScreen(SprSrc, siCenterXPos - SprSrc->GetXSize() / 2 + siAdjustX, 
		siCenterYPos - SprSrc->GetYSize() / 2 + siAdjustY,
		m_si32CurrentDrawFrame  , false );

	return;
}
