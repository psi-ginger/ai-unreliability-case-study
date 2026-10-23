//-------------------------------------------------------------------------------------------------
// NkTimer.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

class NkTimer : public NkMemory
{
public:
	NkTimer();

	inline void Start();
	inline void Stop();

	inline void Hold();
	inline void Unhold();
	
	inline float GetElapsedTime();

private:
	static BOOL ms_bInitialized;
	static LONGLONG ms_llQPFTicksPerSec;

	LONGLONG m_llStartTime;
	LONGLONG m_llHoldTime;
};

#include "NkTimer.inl"