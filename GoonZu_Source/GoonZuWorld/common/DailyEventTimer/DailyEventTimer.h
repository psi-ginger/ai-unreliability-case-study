#pragma once

#include "../CommonLogic/CommonLogic.h"

#define EVENT_0_OCLOCK				0x00000001
#define EVENT_1_OCLOCK				0x00000002		
#define EVENT_2_OCLOCK				0x00000004		
#define EVENT_3_OCLOCK				0x00000008		
#define EVENT_4_OCLOCK				0x00000010  	
#define EVENT_5_OCLOCK				0x00000020  	
#define EVENT_6_OCLOCK				0x00000040		
#define EVENT_7_OCLOCK				0x00000080		
#define EVENT_8_OCLOCK				0x00000100		
#define EVENT_9_OCLOCK				0x00000200		
#define EVENT_10_OCLOCK				0x00000400		
#define EVENT_11_OCLOCK				0x00000800		
#define EVENT_12_OCLOCK				0x00001000		
#define EVENT_13_OCLOCK				0x00002000		
#define EVENT_14_OCLOCK				0x00004000		
#define EVENT_15_OCLOCK				0x00008000		
#define EVENT_16_OCLOCK				0x00010000		
#define EVENT_17_OCLOCK				0x00020000		
#define EVENT_18_OCLOCK				0x00040000		
#define EVENT_19_OCLOCK				0x00080000		
#define EVENT_20_OCLOCK				0x00100000		
#define EVENT_21_OCLOCK				0x00200000		
#define EVENT_22_OCLOCK				0x00400000		
#define EVENT_23_OCLOCK				0x00800000	

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.09.29
// 클래스명 : CDailyEventTimer
// 설명 : 매일 일어나는 이벤트,
//		  입력된 특정 시간 중 정해진 분단위로 실행 되는 타이머
//================================================================================
class CDailyEventTimer
{	 
	// 맴버 메소드
public:
	CDailyEventTimer();
	~CDailyEventTimer();

	// 이벤트 발생 시간 대 입력
	void	AddTime(UI32 uiHour);
	// 타이머 작동
	BOOL	Execute(SI32	siMinute, BOOL (*pCallBackFunc)());
	// 타이머 멈춤
	void	Stop();

	// 프레임 단위로 실행
	BOOL	IsTimed();

protected:
	// 32비트
	const static SI32 BITSIZE = 32;
	//	처리 함수 포인터
	BOOL (*pFuncProcess)();
	
	//	작동 여부
	BOOL	m_bOperation;
	//	분
	SI32	m_siMinute;

	//  이벤트 발생 시간 저장 데이터
	UI32	m_uiTime;
	
	//  같은 시간 대 중복 실행 방지
	SI32	m_siDuplExecCheck;
};

