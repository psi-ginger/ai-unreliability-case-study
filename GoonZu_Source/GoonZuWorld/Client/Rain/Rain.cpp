#include "Rain.h"

CRain::CRain( TSpr *pXSPR, SI32 siWidth, SI32 siHeight )
{
	if ( pXSPR != NULL )
	{
		m_siWidth = siWidth;
		m_siHeight = siHeight;

		m_pRainSPR = pXSPR;

		Initialize();
	}	
}

CRain::~CRain()
{
	Destroy();
}

void CRain::Initialize()
{
	m_bRainStatus = NOT_RAIN_STATUS;

	m_dwStartTime = 0;
	m_dwDelayTime = 0;

	return;
}

void CRain::Destroy()
{
	m_bRainStatus = NOT_RAIN_STATUS;

	m_dwStartTime = 0;
	m_dwDelayTime = 0;

	return;
}

void CRain::SetAction( DWORD dwDelayTime )
{
	m_dwDelayTime = dwDelayTime;
	return;
}

void CRain::Action( DWORD dwCurrentTime )
{
	if ( m_bRainStatus == NOT_RAIN_STATUS )
	{
		m_dwStartTime = dwCurrentTime;
	}

	if ( dwCurrentTime >= m_dwStartTime + m_dwDelayTime )
	{
		m_bRainStatus = NOT_RAIN_STATUS;
	}

	return;
}

void CRain::Draw()
{
	if ( m_bRainStatus == NOT_RAIN_STATUS )
	{
		return;
	}
	
	//-------------------------------------------------------------------------
	//-------------- 이미지 출력부
	

	//-------------------------------------------------------------------------




	return;
}