#pragma once

#include "../CommonLogic/CommonLogic.h"

#include "DormancyEquipItemReward.h"
#include "DormancyPointReward.h"
#include "DormancyInfo.h"

class	CDormancyEquipItemReward;
class	CDormancyPointReward;
class	CDormancyInfo;

//================================================================================
// 작성자 : 박기형
// 작성일 : 2009.07.30
// 클래스명 : CDormancySystem
// 설명 : 휴면 계정 시스템 관련 모듈 클래스
//================================================================================
class CDormancySystem
{
// 맴버 변수
public:
	// 휴면계정보상장비 클래스 
	CDormancyEquipItemReward* m_pclDormancyEquipItemReward;
	// 휴면계정포인트 클래스 
	CDormancyPointReward* m_pclDormancyPointReward;
	// 모드 플레그
	SI32	m_siGameMode;


// 멤버 메소드
public:
	// 소멸자
	virtual ~CDormancySystem();

	// 생성자
	CDormancySystem(SI32 siGameMode = GAMEMODE_CLIENT);
	
	// 휴면계정보상장비 클래스 참조자 얻기
	const CDormancyEquipItemReward *GetDormancyEquipItemReward() const;
	// 휴면계정포인트 클래스 참조자 얻기
	const CDormancyPointReward *GetDormancyPointReward() const;

	//생성
	BOOL	Create();

	//게임 모드
	SI32	GetGameMode();
};