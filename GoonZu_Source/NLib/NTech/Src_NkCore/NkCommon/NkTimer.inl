//-------------------------------------------------------------------------------------------------
// NkTimer.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

inline void NkTimer::Start()
{
	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	m_llStartTime = qwTime.QuadPart;
	m_llHoldTime = 0;
}

inline void NkTimer::Stop()
{
	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	m_llStartTime = qwTime.QuadPart - m_llStartTime;
}

inline void NkTimer::Hold()
{
	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	m_llHoldTime = qwTime.QuadPart;
}

inline void NkTimer::Unhold()
{
	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	m_llStartTime += qwTime.QuadPart - m_llHoldTime;
}

inline float NkTimer::GetElapsedTime()
{
	return (float)(m_llStartTime / (double)ms_llQPFTicksPerSec);
}