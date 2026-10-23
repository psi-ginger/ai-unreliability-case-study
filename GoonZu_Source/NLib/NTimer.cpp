//-----------------------------------------------------------------------------
// 2005/09/20 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#include "ntimer.h"

BOOL NTimer::m_bTimerInitialized = FALSE;
LONGLONG NTimer::m_llQPFTicksPerSec = 0;

NTimer::NTimer()
{
    if( m_bTimerInitialized == FALSE )
    {
        m_bTimerInitialized = TRUE;
       
		LARGE_INTEGER qwTicksPerSec;
        
		QueryPerformanceFrequency( &qwTicksPerSec );

        m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

	m_llStartTime = 0;
	m_llStopTime = 0;
	m_llSuspendTime = 0;

	m_state = TIMER_STATE::OFF;
}

NTimer::~NTimer()
{

}

void NTimer::Start()
{

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter( &qwTime );

	m_llStartTime = qwTime.QuadPart;
	m_llStopTime = qwTime.QuadPart;
	m_llSuspendTime = 0;
	
	m_state = TIMER_STATE::ON;
	
}

void NTimer::Stop()
{
	if( m_state == TIMER_STATE::ON ) {
		
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );

		m_llStopTime = qwTime.QuadPart;

		m_state = TIMER_STATE::OFF;
	}
}



void NTimer::Suspend()
{
	if( m_state == TIMER_STATE::ON ) {

		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );

		m_llStopTime = qwTime.QuadPart;

		m_state = TIMER_STATE::HOLD;		

	}
}

void NTimer::Resume()
{
	if( m_state == TIMER_STATE::HOLD ) {

		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );

		m_llSuspendTime += ( qwTime.QuadPart - m_llStopTime );

		m_state = TIMER_STATE::ON;
	}
}

REAL32 NTimer::GetCurrentTime()
{
	REAL64 fTime;

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter( &qwTime );

	fTime = qwTime.QuadPart / (REAL64)m_llQPFTicksPerSec;
	
	return (REAL32)fTime;
}


REAL32 NTimer::GetElapsedTime()
{
	REAL64 fTime;

	if( m_state == TIMER_STATE::ON )
	{
		LARGE_INTEGER qwTime;
		QueryPerformanceCounter( &qwTime );

		fTime = (REAL64)( qwTime.QuadPart - m_llStartTime - m_llSuspendTime ) / (REAL64)m_llQPFTicksPerSec;

		return (REAL32)fTime;
	}
	else
	{
		fTime = (REAL64)( m_llStopTime - m_llStartTime - m_llSuspendTime ) / (REAL64)m_llQPFTicksPerSec; 

		return (REAL32)fTime;
	}
}

REAL32 NTimer::GetSuspendTime()
{
	return (REAL32)( (REAL64)( m_llSuspendTime ) / (REAL64)( m_llQPFTicksPerSec ) );
}
