#ifndef _NTIMER_H
#define _NTIMER_H

#include <windows.h>
#include "ndatatypes.h"

class NTimer
{
public:
	NTimer();
	~NTimer();

	enum TIMER_STATE
	{
		OFF = 0,
		ON,
		HOLD,
	};

	void	Start();
	void	Stop();
	void	Suspend();
	void	Resume();	

	REAL32	GetCurrentTime();					// 현재 시간을 얻는다 
	REAL32	GetElapsedTime();					// Start이후 일시 정지된 시간을 제외한 경과 시간을 얻는다 
	REAL32	GetSuspendTime();					// Start이후 일시 정지된 시간을 얻는다 

private:
	static BOOL		m_bTimerInitialized;
	static LONGLONG	m_llQPFTicksPerSec;

    LONGLONG		m_llStartTime;				// 시작 시간 
	LONGLONG		m_llStopTime;				// 중지/일시정지 시간 
	LONGLONG		m_llSuspendTime;			// 일시정지된 시간 

	TIMER_STATE		m_state;					// 상태 
};

#endif
