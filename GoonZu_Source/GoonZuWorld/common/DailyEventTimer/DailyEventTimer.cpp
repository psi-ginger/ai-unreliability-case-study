#include ".\DailyEventTimer.h"

CDailyEventTimer::CDailyEventTimer()
{
	m_bOperation = FALSE;
	m_siMinute = 0;
	pFuncProcess = NULL;
	m_siDuplExecCheck = 0;
	m_uiTime = 0;
}

CDailyEventTimer::~CDailyEventTimer()
{
	
}

// 이벤트 발생 시간 대 입력
void	CDailyEventTimer::AddTime(UI32 uiHour)
{
	m_uiTime = m_uiTime | uiHour;
}
// 타이머 작동
BOOL	CDailyEventTimer::Execute(SI32 siMinute, BOOL (*pCallBackFunc)())
{
	m_siMinute = siMinute;
	pFuncProcess = pCallBackFunc;

	m_bOperation = TRUE;

	return TRUE;
}
// 타이머 멈춤
void	CDailyEventTimer::Stop()
{
	m_bOperation = FALSE;
}

// 프레임 단위로 실행
BOOL	CDailyEventTimer::IsTimed()
{
	if( FALSE == m_bOperation)
		return FALSE;

	_SYSTEMTIME st;
	GetLocalTime(&st);

	if( m_siDuplExecCheck == (st.wHour + st.wMinute))
	{
		return FALSE;
	}

	m_siDuplExecCheck = st.wHour + st.wMinute;

	if( st.wMinute == m_siMinute)
	{
		SI32 siTemp = m_uiTime << (BITSIZE - ( st.wHour + 1 ));

		if( siTemp & 0x80000000)
		{
			if( NULL != pFuncProcess )
			{
				pFuncProcess();
			}
			return TRUE;
		}
	}
	return FALSE;
}