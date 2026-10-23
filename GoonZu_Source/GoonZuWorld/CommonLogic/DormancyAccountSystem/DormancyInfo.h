#pragma once

#include "../CommonLogic/CommonLogic.h"

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.08.12
// 클래스명 : CDormancyInfo
// 설명 : 휴면 계정 시스템 관련 캐릭터가 가지고 있어야 할 정보
//================================================================================
class CDormancyInfo
{
	// 맴버 변수
public:

private:
	_SYSTEMTIME  m_StartedDate;				// 휴면 계정이 된 날짜
	SI32	m_siDormancyPoint;				// 휴면 포인트
	SI32	m_siRemainedDormancyPlayTime;	// 휴면 포인트를 받기 위해 필요한 휴면 플레이 시간
	BOOL	m_bDormancyState;				// 휴면 계정 상태 인지
	BOOL	m_bGiveDormancyEquipItem;		// 휴면 보급 상자 아이템을 보급 받았는지?

	// 맴버 메소드
public:
	// 생성자
	CDormancyInfo();
	// 소멸자
	virtual ~CDormancyInfo(void);

	VOID	Set( CDormancyInfo	*pclDormancyInfo );

	VOID	IncreasePoint();
	VOID	CalcPoint(SI32 siPoint);
	VOID	IncreasePlayTime();

	VOID	SetDormancyPoint(SI32 siDormancyPoint);
	VOID	SetRemainedDormancyPlayTime(SI32 siRemainedDormancyPlayTime);
	VOID	SetDormancyState(BOOL bDormancyState);
	VOID	SetGiveDormancyEquipItem(BOOL bGiveDormancyEquipItem);

	SI32	GetDormancyPoint();
	SI32	GetRemainedDormancyPlayTime();
	BOOL	GetDormancyState();
	BOOL	GetGiveDormancyEquipItem();

	SI32*	GetDormancyPointPointer();
	SI32*	GetRemainedDormancyPlayTimePointer();
	BOOL*	GetDormancyStatePointer();
	BOOL*	GetGiveDormancyEquipItemPointer();
	_SYSTEMTIME* GetStartedDate();
	
};