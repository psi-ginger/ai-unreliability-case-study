#include "HealCounter.h"

#include "..\..\client.h"
extern cltCommonLogic* pclClient;

CHealCounter::CHealCounter()
{
	m_bDrawStatus = false;
	m_si32DrawMaxFrame = 0;
	m_si32CurrentFrame = 0;
	m_si32ChangedExp = 0;
	m_si32Width = 0;
	m_si32Height = 0;
	m_si32ExpCharNum = 0;
	m_pXSPR = pclClient->GetGlobalSpr( GIMG_HEALCOUNTER );

	Initialize();
}

CHealCounter::~CHealCounter()
{
	Destroy();
}

void CHealCounter::Initialize()
{
	m_delayClock = pclClient->CurrentClock;

}

void CHealCounter::Destroy()
{
	return;
}

void CHealCounter::Set( SI32 si32ChangedExp, SI32 DrawMaxFrame )
{
	m_si32ChangedExp = si32ChangedExp;
	m_bDrawStatus = true;
	m_si32DrawMaxFrame = DrawMaxFrame;
	m_si32CurrentFrame = 0;

	
	if ( si32ChangedExp > 0 )
	{
		if ( m_si32ChangedExp > 0 && m_si32ChangedExp < 10 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 2;
			m_si32ExpCharNum = 2;
		}
		else if ( m_si32ChangedExp >= 10 && m_si32ChangedExp < 100 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 3;
			m_si32ExpCharNum = 3;
		}
		else if ( m_si32ChangedExp >= 100 && m_si32ChangedExp < 1000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 4;
			m_si32ExpCharNum = 4;
		}
		else if ( m_si32ChangedExp >= 1000 && m_si32ChangedExp < 10000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 5;
			m_si32ExpCharNum = 5;
		}
		else if ( m_si32ChangedExp >= 10000 && m_si32ChangedExp < 100000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 6;
			m_si32ExpCharNum = 6;
		}
		else
		{
			return;
		}
	}
	else 
	{
		if ( m_si32ChangedExp < 0 && m_si32ChangedExp > -10 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 2;
			m_si32ExpCharNum = 2;
		}
		else if ( m_si32ChangedExp <= -10 && m_si32ChangedExp > -100 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 3;
			m_si32ExpCharNum = 3;
		}
		else if ( m_si32ChangedExp <= -100 && m_si32ChangedExp > -1000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 4;
			m_si32ExpCharNum = 4;
		}
		else if ( m_si32ChangedExp <= -1000 && m_si32ChangedExp > -10000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 5;
			m_si32ExpCharNum = 5;
		}
		else if ( m_si32ChangedExp <= -10000 && m_si32ChangedExp > -100000 )
		{
			m_si32Width = m_pXSPR->GetXSize() * 6;
			m_si32ExpCharNum = 6;
		}
		else
		{
			return;
		}
	}

	return;
}

void CHealCounter::Action()
{
	if ( ! m_bDrawStatus )
	{
		return;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;


		++m_si32CurrentFrame;

		if ( m_si32CurrentFrame > m_si32DrawMaxFrame )
		{
			m_bDrawStatus = false;
			m_si32CurrentFrame = 0;
			m_si32DrawMaxFrame = 0;
		}
	}

	return;
}

void CHealCounter::Draw( SI32 si32CenterXPos, SI32 si32CenterYPos )
{
	if ( ! m_bDrawStatus )
	{
		return;
	}

	// m_pXSPR->GetXSize() 
	if ( m_si32ChangedExp > 0 )
	{
		switch( m_si32ExpCharNum )
		{
		case 2:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp ); 
			break;
		case 3:
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, 10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / 10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % 10 ); 
			break;
		case 4:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / 100 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, (m_si32ChangedExp / 10)%10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % 10 ); 
			break;
		case 5:
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, 10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / 1000 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, ( m_si32ChangedExp / 100 ) % 10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, ( m_si32ChangedExp / 10 ) % 10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % 10 ); 
			break;
		case 6:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 3, si32CenterYPos - m_pXSPR->GetYSize() / 2, 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / 10000 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, ( m_si32ChangedExp / 1000 ) % 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, ( m_si32ChangedExp / 100 ) % 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, ( m_si32ChangedExp / 10 ) % 10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % 10 ); 
			break;
		}
	}
	else
	{
		switch( m_si32ExpCharNum )
		{
		case 2:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, 11 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp * -1);
			break;
		case 3:
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, 11 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			break;
		case 4:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, 11 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -100 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			break;
		case 5:
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, 11 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -1000 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos - m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -100 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			GP.PutSpr(m_pXSPR, ( int ) ( si32CenterXPos + m_pXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			break;
		case 6:
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 3, si32CenterYPos - m_pXSPR->GetYSize() / 2, 11 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -10000 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos - m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -1000 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -100 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize(), si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp / -10 ); 
			GP.PutSpr(m_pXSPR, si32CenterXPos + m_pXSPR->GetXSize() * 2, si32CenterYPos - m_pXSPR->GetYSize() / 2, m_si32ChangedExp % -10 ); 
			break;
		}
	}

	return;
}