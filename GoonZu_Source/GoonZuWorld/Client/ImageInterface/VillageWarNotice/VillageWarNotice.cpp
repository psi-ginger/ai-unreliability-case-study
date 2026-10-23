#include "VillageWarNotice.h"
#include "EtcTool\EtcTool.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

#define VILLAEWAR_NOTICE_IMAGE_YPOS			100

CVillageWarNotice::CVillageWarNotice()
{

	m_pVillageWarSpr = pclClient->GetGlobalSpr(GIMG_VILLAGEWAR_NOTICE);

	Initialize();
}

CVillageWarNotice::~CVillageWarNotice()
{	

}

void CVillageWarNotice::Initialize()
{
	m_bShow = false;
	m_siFrameCount = 0;
	m_siContinueCount = 0;
	m_siVillageWarNoticeMode = 0;
	return;
}

void CVillageWarNotice::Set(SI16 siVillageWarNoticeMode)
{
	m_siVillageWarNoticeMode = siVillageWarNoticeMode;
	
	if( VILLAGEWAR_NOTICE_NONE == m_siVillageWarNoticeMode )
	{
		m_siFrameCount = 0;
		m_bShow	= false;
		
	}
	else
	{
		m_siFrameCount = 0;
		m_bShow = true;

	}

}

void CVillageWarNotice::Action()
{
	if(! m_bShow )	return;

	m_siFrameCount++;
	
	if( m_siFrameCount > 100 )
	{
		m_siFrameCount = 0;
		m_bShow = false;
	}

	return;
}

void CVillageWarNotice::Draw( SI32 XMovePos, SI32 YMovePos )
{
	if ( ! m_bShow )
	{
		return;
	}

	if( VILLAGEWAR_NOTICE_NONE == m_siVillageWarNoticeMode )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;

	SI32 xPos = ( pclclient->GetScreenXSize() - m_pVillageWarSpr->GetXSize() ) / 2 + 40;
	SI32 yPos = VILLAEWAR_NOTICE_IMAGE_YPOS;
	
	SI32 imageFont = 0;

	if( VILLAGEWAR_NOTICE_ATTACK ==  m_siVillageWarNoticeMode )
		imageFont = 0;
	else if( VILLAGEWAR_NOTICE_DEFENCE ==  m_siVillageWarNoticeMode )
		imageFont = 1;
	else if( VILLAGEWAR_NOTICE_WIN ==  m_siVillageWarNoticeMode )
		imageFont = 2;
	else if( VILLAGEWAR_NOTICE_LOSE ==  m_siVillageWarNoticeMode )
		imageFont = 3;
	else
		return;

	GP.PutSpr(m_pVillageWarSpr,xPos +XMovePos ,yPos + YMovePos,imageFont);

	return;
}