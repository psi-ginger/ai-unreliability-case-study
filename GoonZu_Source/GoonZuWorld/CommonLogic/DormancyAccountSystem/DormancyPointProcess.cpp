#include ".\dormancypointProcess.h"
#include "DormancyInfo.h"
#include "../common/char/CharCommon/Char-Common.h"

CDormancyPointProcess::CDormancyPointProcess(void)
{
	// 마지막으로 처리 한 시간
	m_siLastProcessingTime = 0;

	// 처리 여부
	m_bProcessing = FALSE;
}

CDormancyPointProcess::~CDormancyPointProcess(void)
{
}

// 포인트 계산
SI32	CDormancyPointProcess::CalcPoint(cltCharCommon *pclChar)
{
	if( NULL == pclChar)
		return CALCPOINT_RESULT_CALCERROR;
	if( NULL == pclChar->pclCI)
		return CALCPOINT_RESULT_CALCERROR;
	if( FALSE == m_bProcessing)
		return CALCPOINT_RESULT_CALCERROR;

	// 휴면 정보 
	CDormancyInfo &rclDormancyInfo = pclChar->pclCI->clDormancyInfo;

	// 플레이 시간 증가 (1분 증가)
	rclDormancyInfo.IncreasePlayTime();

	// 포인트 지급 하는 시간을 플레이 하였다면
	if( POINT_TIME <= rclDormancyInfo.GetRemainedDormancyPlayTime() )
	{
		// 플레이 시간 초기화
		rclDormancyInfo.SetRemainedDormancyPlayTime( INIT_TIME );
		return CALCPOINT_RESULT_INCREASEPOINT;
	}
	
	return CALCPOINT_RESULT_INCREASEPLAYTIME;
}

// 처리 할지 여부 결정
BOOL	CDormancyPointProcess::DeterminProcessing(DWORD	siCurrentime)
{
	if( m_siLastProcessingTime > siCurrentime)
	{
		m_siLastProcessingTime = 0;
	}

	if( (siCurrentime - m_siLastProcessingTime) >= UNIT_TIME)
	{
		m_bProcessing = TRUE;
		m_siLastProcessingTime = siCurrentime;
	}
	else
	{
		m_bProcessing = FALSE;
		return FALSE;
	}
	return TRUE;
}

// 포인트 변경
BOOL	CDormancyPointProcess::ChangePoint(cltCharCommon *pclChar, SI32 siPoint)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == pclChar->pclCI)
		return FALSE;

	// 휴면 정보 
	CDormancyInfo &pclDormancyInfo = pclChar->pclCI->clDormancyInfo;

	pclDormancyInfo.CalcPoint(siPoint);

	return TRUE;
}