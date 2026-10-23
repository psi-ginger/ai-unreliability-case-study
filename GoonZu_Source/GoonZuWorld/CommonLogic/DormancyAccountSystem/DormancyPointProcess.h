#pragma once

#include "../../GoonZuWorld/Common/CommonHeader.h"

class cltCharCommon;

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.2
// 클래스명 : CDormancyPointProcess
// 설명 : 휴면 계정 시스템의 포인트 처리 클래스
//================================================================================

class CDormancyPointProcess
{
//맴버 변수
public:
	// 포인트 계산 결과 
	const static SI32 CALCPOINT_RESULT_INCREASEPOINT = 1;
	const static SI32 CALCPOINT_RESULT_INCREASEPLAYTIME = 2;
	const static SI32 CALCPOINT_RESULT_CALCERROR = 3;

private:
	// 포인트 지급을 위해 계산되는 초기 시간
	const static SI32 INIT_TIME = 0; //0분 (단위 : 분)

	// 포인트를 지급 하는 시간
	const static SI32 POINT_TIME = 60; //1시간 (단위 : 분)

	// 처리 단위 시간
	const static SI32 UNIT_TIME = 60000; //1분 (단위 : 밀리세컨드)

	// 마지막으로 처리 한 시간
	DWORD	m_siLastProcessingTime;

	// 처리 여부
	BOOL	m_bProcessing;

//맴버 메소드
public:
	CDormancyPointProcess(void);
	virtual ~CDormancyPointProcess(void);

	// 포인트 변경
	BOOL	ChangePoint(cltCharCommon *pclChar, SI32 siPoint);

	// 포인트 계산
	BOOL	CalcPoint(cltCharCommon *pclChar);

	// 처리 할지 여부 결정
	BOOL	DeterminProcessing(DWORD	siCurrentime);
};
