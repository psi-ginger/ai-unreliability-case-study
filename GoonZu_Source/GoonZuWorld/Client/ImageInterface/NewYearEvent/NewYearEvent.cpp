#include "NewYearEvent.h"
#include "EtcTool\EtcTool.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

#define NEWYEAR_NOTICE_IMAGE_YPOS			100

CNewYearEventNotice::CNewYearEventNotice()
{
	
	Initialize();
}

CNewYearEventNotice::~CNewYearEventNotice()
{	
	
}

void CNewYearEventNotice::Initialize()
{
	m_bShow = false;
	m_siFrameCount = 0;
	m_siContinueCount = 0;	
	return;
}

void CNewYearEventNotice::Set()
{

	m_siFrameCount = 0;
	m_bShow = true;

}

void CNewYearEventNotice::Action()
{
	if(! m_bShow )	return;

	m_siFrameCount++;

	if( m_siFrameCount > 400 )
	{
		m_siFrameCount = 0;
		m_bShow = false;
	}

	return;
}

void CNewYearEventNotice::Draw( SI32 XMovePos, SI32 YMovePos )
{
	if ( ! m_bShow )
	{
		return;
	}

	TSpr * pEventSpr = NULL;

	pEventSpr = pclClient->GetGlobalSpr(GIMG_NEWYEAR_EVENT_NOTICE);

	if( pEventSpr == NULL)	return; 

	cltClient* pclclient = (cltClient*)pclClient;

	SI32 xPos = ( pclclient->GetScreenXSize() - pEventSpr->GetXSize() ) / 2 + 40;
	SI32 yPos = NEWYEAR_NOTICE_IMAGE_YPOS;

	SI32 imageFont = 0;

	GP.PutSpr( pEventSpr , xPos +XMovePos , yPos + YMovePos , imageFont );

	return;
}