#include "CharJim.h"
#include "Graphic_Capsule.h"


CCharJim::CCharJim( TSpr *p1XSPR )
{	
#ifdef _DEBUG
	if ( p1XSPR == NULL  )
	{
		MessageBox( NULL, TEXT("CCharJim Image == NULL"), TEXT("error"), MB_OK|MB_TOPMOST );
	}
#endif

	m_pJimXSPR = p1XSPR;
	

	m_siDelayFrame = 0;
	m_cCurrentFrame = 0;
	m_cStatus = NOT_JIM_STATUS;

	m_siDelayFrame = 0;
	m_bMyJim = true;

}

CCharJim::~CCharJim()
{
}


void CCharJim::Set( bool bSet, bool bMyJim )
{
	if ( bSet )
	{
		m_cCurrentFrame = 0;
		m_cStatus = JIM_STATUS;
		m_bMyJim = bMyJim;
		m_siDelayFrame = 0;
	}
	else
	{
		m_cCurrentFrame = 0;
		m_cStatus = NOT_JIM_STATUS;
		m_bMyJim = bMyJim;
		m_siDelayFrame = 0;
	}
	
	return;
}

void CCharJim::Action()
{
	if ( m_cStatus == NOT_JIM_STATUS )
	{
		return;
	}

	if ( m_siDelayFrame++ >= DELAYFRAME_NUM )
	{
		
		++m_cCurrentFrame;

		if ( m_cCurrentFrame == 2 )
		{			
			m_cCurrentFrame = 0;
		}

		m_siDelayFrame = 0;
	}	

	return;
}


void CCharJim::Draw( SI32 siCenterXPos, SI32 siCenterYPos )
{
	if ( m_cStatus == NOT_JIM_STATUS )
	{
		return;
	}

	
	if ( m_bMyJim )
	{
		GP.PutSpr(m_pJimXSPR, siCenterXPos - m_pJimXSPR->GetXSize() / 2, 
			siCenterYPos,
			m_cCurrentFrame );
	}
	else
	{
		GP.PutSpr(m_pJimXSPR, siCenterXPos - m_pJimXSPR->GetXSize() / 2, 
			siCenterYPos,
			m_cCurrentFrame + 2 );
	}

	

	return;
}

BOOL CCharJim::IsJimStatus()
{ 
	return (m_cStatus==JIM_STATUS); 
}