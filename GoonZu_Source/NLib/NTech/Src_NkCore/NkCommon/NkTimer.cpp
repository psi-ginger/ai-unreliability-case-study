
#include "NkTimer.h"

BOOL NkTimer::ms_bInitialized = 0;
LONGLONG NkTimer::ms_llQPFTicksPerSec = 0;

NkTimer::NkTimer()
{
	if( !ms_bInitialized )
	{
		ms_bInitialized = TRUE;

		LARGE_INTEGER qwTicksPerSec;

		QueryPerformanceFrequency(&qwTicksPerSec);

		ms_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	m_llStartTime = 0;
	m_llHoldTime = 0;
}
