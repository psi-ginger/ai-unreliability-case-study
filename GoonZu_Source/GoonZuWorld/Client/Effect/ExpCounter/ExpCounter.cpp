#include "ExpCounter.h"

#include "..\..\client.h"
extern cltCommonLogic* pclClient;

CExpCounter::CExpCounter()
{
	m_bExpDrawStatus = false;
	m_siExpDrawMaxFrame = 0;
	m_siExpCurrentFrame = 0;
	m_siChangedExp = 0;
	m_siExpWidth = 0;
	m_siExpHeight = 0;
	m_siExpCharNum = 0;
	m_pExpXSPR = pclClient->GetGlobalSpr( GIMG_EXPCOUNTER );


	for ( SI16 i = 0; i < MAX_SKILLEXP_COUNTER_NUM; ++i )
	{	
		m_siSkillUnique[ i ] = 0;
		m_bSkillExpDrawStatus[ i ] = false;
		m_siSkillExpDrawMaxFrame[ i ] = 0;
		m_siSkillExpCurrentFrame[ i ] = 0;	
		m_siChangedSkillExp[ i ] = 0;	
		m_siSkillExpCharNum[ i ] = 0;
	}

	m_siSkillExpWidth = 0;
	m_siSkillExpHeight = 0;
	
	m_pSkillNameXSPR = pclClient->GetGlobalSpr( GIMG_SKILLNAME );
	m_pSkillExpXSPR = pclClient->GetGlobalSpr( GIMG_SKILLEXPCOUNTER );

	Initialize();
}

CExpCounter::~CExpCounter()
{
	Destroy();
}

void CExpCounter::Initialize()
{
	m_expClock = pclClient->CurrentClock;
}

void CExpCounter::Destroy()
{
	return;
}

void CExpCounter::Set( SI32 si32ChangedExp, SI32 DrawMaxFrame )
{
	m_siChangedExp = si32ChangedExp;
	m_bExpDrawStatus = true;
	m_siExpDrawMaxFrame = DrawMaxFrame;
	m_siExpCurrentFrame = 0;
	
	if ( si32ChangedExp > 0 )
	{
		if ( m_siChangedExp > 0 && m_siChangedExp < 10 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 2;
			m_siExpCharNum = 2;
		}
		else if ( m_siChangedExp >= 10 && m_siChangedExp < 100 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 3;
			m_siExpCharNum = 3;
		}
		else if ( m_siChangedExp >= 100 && m_siChangedExp < 1000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 4;
			m_siExpCharNum = 4;
		}
		else if ( m_siChangedExp >= 1000 && m_siChangedExp < 10000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 5;
			m_siExpCharNum = 5;
		}
		else if ( m_siChangedExp >= 10000 && m_siChangedExp < 100000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 6;
			m_siExpCharNum = 6;
		}
		else
		{
			m_bExpDrawStatus = false;
			return;
		}
	}
	else 
	{
		if ( m_siChangedExp < 0 && m_siChangedExp > -10 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 2;
			m_siExpCharNum = 2;
		}
		else if ( m_siChangedExp <= -10 && m_siChangedExp > -100 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 3;
			m_siExpCharNum = 3;
		}
		else if ( m_siChangedExp <= -100 && m_siChangedExp > -1000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 4;
			m_siExpCharNum = 4;
		}
		else if ( m_siChangedExp <= -1000 && m_siChangedExp > -10000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 5;
			m_siExpCharNum = 5;
		}
		else if ( m_siChangedExp <= -10000 && m_siChangedExp > -100000 )
		{
			m_siExpWidth = m_pExpXSPR->GetXSize() * 6;
			m_siExpCharNum = 6;
		}
		else
		{
			m_bExpDrawStatus = false;
			return;
		}
	}

	return;
}

void CExpCounter::SkillSet( SI16 SkillUnique, SI32 si32ChangedSkillExp, SI32 DrawMaxFrame )
{
	SI16 AvailableIndex = GetAvailableSkillExpIndex();

	m_siSkillUnique[ AvailableIndex ] = SkillUnique;
	m_siChangedSkillExp[ AvailableIndex ] = si32ChangedSkillExp;
	m_bSkillExpDrawStatus[ AvailableIndex ] = true;
	m_siSkillExpDrawMaxFrame[ AvailableIndex ] = DrawMaxFrame;
	m_siSkillExpCurrentFrame[ AvailableIndex ] = 0;

	
	if ( si32ChangedSkillExp > 0 )
	{
		if ( si32ChangedSkillExp > 0 && si32ChangedSkillExp < 10 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 2;
			m_siSkillExpCharNum[ AvailableIndex ] = 2;
		}
		else if ( si32ChangedSkillExp >= 10 && si32ChangedSkillExp < 100 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 3;
			m_siSkillExpCharNum[ AvailableIndex ] = 3;
		}
		else if ( si32ChangedSkillExp >= 100 && si32ChangedSkillExp < 1000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 4;
			m_siSkillExpCharNum[ AvailableIndex ] = 4;
		}
		else if ( si32ChangedSkillExp >= 1000 && si32ChangedSkillExp < 10000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 5;
			m_siSkillExpCharNum[ AvailableIndex ] = 5;
		}
		else if ( si32ChangedSkillExp >= 10000 && si32ChangedSkillExp < 100000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 6;
			m_siSkillExpCharNum[ AvailableIndex ] = 6;
		}
		else
		{
			m_bSkillExpDrawStatus[ AvailableIndex ] = false;
			return;
		}
	}
	else 
	{
		if ( si32ChangedSkillExp < 0 && si32ChangedSkillExp > -10 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 2;
			m_siSkillExpCharNum[ AvailableIndex ] = 2;
		}
		else if ( si32ChangedSkillExp <= -10 && si32ChangedSkillExp > -100 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 3;
			m_siSkillExpCharNum[ AvailableIndex ] = 3;
		}
		else if ( si32ChangedSkillExp <= -100 && si32ChangedSkillExp > -1000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 4;
			m_siSkillExpCharNum[ AvailableIndex ] = 4;
		}
		else if ( si32ChangedSkillExp <= -1000 && si32ChangedSkillExp > -10000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 5;
			m_siSkillExpCharNum[ AvailableIndex ] = 5;
		}
		else if ( si32ChangedSkillExp <= -10000 && si32ChangedSkillExp > -100000 )
		{
			m_siSkillExpWidth = m_pSkillExpXSPR->GetXSize() * 6;
			m_siSkillExpCharNum[ AvailableIndex ] = 6;
		}
		else
		{
			m_bSkillExpDrawStatus[ AvailableIndex ] = false;
			return;
		}
	}

	return;
}

void CExpCounter::Action()
{
	
	if( m_bExpDrawStatus )
	{
		if (TABS(pclClient->CurrentClock - m_expClock) > 50)
		{
			m_expClock = pclClient->CurrentClock;

			++m_siExpCurrentFrame;
		}

		if ( m_siExpCurrentFrame > m_siExpDrawMaxFrame )
		{
			m_bExpDrawStatus = false;
			m_siExpCurrentFrame = 0;
			m_siExpDrawMaxFrame = 0;
		}
	}

	for( SI16 i = 0; i < MAX_SKILLEXP_COUNTER_NUM; ++i )
	{	
		if( m_bSkillExpDrawStatus[ i ] )
		{
			++m_siSkillExpCurrentFrame[ i ];

			if ( m_siSkillExpCurrentFrame[ i ] > m_siSkillExpDrawMaxFrame[ i ] )
			{
				m_bSkillExpDrawStatus[ i ] = false;
				m_siSkillExpCurrentFrame[ i ] = 0;
				m_siSkillExpDrawMaxFrame[ i ] = 0;
			}
		}
	}


	return;
}

void CExpCounter::Draw( SI32 si32CenterXPos, SI32 si32CenterYPos )
{
	if ( m_bExpDrawStatus )
	{	
		// m_pExpXSPR->GetXSize() 
		if ( m_siChangedExp > 0 )
		{
			switch( m_siExpCharNum )
			{
			case 2:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp ); 
				break;
			case 3:
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp / 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp % 10 ); 
				break;
			case 4:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp / 100 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, (m_siChangedExp / 10)%10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp % 10 ); 
				break;
			case 5:
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp / 1000 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( m_siChangedExp / 100 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( m_siChangedExp / 10 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp % 10 ); 
				break;
			case 6:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 3, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp / 10000 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( m_siChangedExp / 1000 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( m_siChangedExp / 100 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( m_siChangedExp / 10 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, m_siChangedExp % 10 ); 
				break;
			}
		}
		else
		{
			SI32 ChanagedExp = m_siChangedExp * (-1);
			
			switch( m_siExpCharNum )
			{
			case 2:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 11 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp ); 
				break;
			case 3:
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 11 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp / 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp % 10 ); 
				break;
			case 4:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 11 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp / 100 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, (ChanagedExp / 10)%10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp % 10 ); 
				break;
			case 5:
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 11 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp / 1000 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos - m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( ChanagedExp / 100 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( ChanagedExp / 10 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, ( int ) ( si32CenterXPos + m_pExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp % 10 ); 
				break;
			case 6:
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 3, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, 11 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp / 10000 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos - m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( ChanagedExp / 1000 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( ChanagedExp / 100 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize(), si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ( ChanagedExp / 10 ) % 10 ); 
				GP.PutSpr(m_pExpXSPR, si32CenterXPos + m_pExpXSPR->GetXSize() * 2, si32CenterYPos - m_pExpXSPR->GetYSize() / 2, ChanagedExp % 10 ); 
				break;
			}
		}
	}



	for ( SI16 i = 0; i < MAX_SKILLEXP_COUNTER_NUM; ++i )
	{
		if ( m_bSkillExpDrawStatus[ i ] )
		{	
			if ( m_siChangedSkillExp[ i ] > 0 )
			{
				switch( m_siSkillExpCharNum[ i ] )
				{
				case 2:
					GP.PutSpr(m_pSkillNameXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize() - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] ); 
					break;
				case 3:
					GP.PutSpr(m_pSkillNameXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 + ( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + ( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] / 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + ( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] % 10 ); 
					break;
				case 4:
					GP.PutSpr(m_pSkillNameXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2 - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] / 100 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, (m_siChangedSkillExp[ i ] / 10)%10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 + si32CenterXPos + m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] % 10 ); 
					break;
				case 5:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2.5f - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] / 1000 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( m_siChangedSkillExp[ i ] / 100 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( m_siChangedSkillExp[ i ] / 10 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] % 10 ); 
					break;
				case 6:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 3 - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 3, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] / 10000 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( m_siChangedSkillExp[ i ] / 1000 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( m_siChangedSkillExp[ i ] / 100 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos + m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( m_siChangedSkillExp[ i ] / 10 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siChangedSkillExp[ i ] % 10 ); 
					break;
				}
			}
			else
			{
				SI32 ChanagedSkillExp = m_siChangedSkillExp[ i ] * (-1);
				
				switch( m_siSkillExpCharNum[ i ] )
				{
				case 2:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 11 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp ); 
					break;
				case 3:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 11 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp / 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp % 10 ); 
					break;
				case 4:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2 - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 11 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp / 100 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, (ChanagedSkillExp / 10)%10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos + m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp % 10 ); 
					break;
				case 5:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2.5f - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 11 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp / 1000 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( ChanagedSkillExp / 100 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 0.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( ChanagedSkillExp / 10 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +( int ) ( si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 1.5f ), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp % 10 ); 
					break;
				case 6:
					GP.PutSpr(m_pSkillNameXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 3 - m_pSkillNameXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, m_siSkillUnique[ i ] );
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 3, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, 11 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp / 10000 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos - m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( ChanagedSkillExp / 1000 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( ChanagedSkillExp / 100 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos + m_pSkillExpXSPR->GetXSize(), si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ( ChanagedSkillExp / 10 ) % 10 ); 
					GP.PutSpr(m_pSkillExpXSPR, 15 +si32CenterXPos + m_pSkillExpXSPR->GetXSize() * 2, si32CenterYPos - m_pSkillExpXSPR->GetYSize() / 2 - m_pSkillNameXSPR->GetYSize() - 13 * i, ChanagedSkillExp % 10 ); 
					break;
				}
			}
		}
	}

	return;
}

SI16 CExpCounter::GetAvailableSkillExpIndex()
{
	SI16 BestOldFrameIndex = 0;
	SI16 BestOldFrame = 0;

	for ( SI16 i = 0; i < MAX_SKILLEXP_COUNTER_NUM; ++i )
	{
		if ( ! m_bSkillExpDrawStatus[ i ] )
		{
			return i;
		}
		else
		{
			if ( BestOldFrame < 1 )
			{
				BestOldFrameIndex = i;
				BestOldFrame = m_siSkillExpCurrentFrame[ i ];
			}
			else if ( m_siSkillExpCurrentFrame[ i ] > 0 )			// 한번 이라도 그려진 적이 있어야지... 덮어 쓰지...
			{
				if ( BestOldFrame > m_siSkillExpCurrentFrame[ i ] )
				{
					BestOldFrameIndex = i;
					BestOldFrame = m_siSkillExpCurrentFrame[ i ];
				}
			}
		}
	}

	return BestOldFrameIndex;
}
