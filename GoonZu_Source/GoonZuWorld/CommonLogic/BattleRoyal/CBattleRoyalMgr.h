//
//
//  @ File Name : CBattleRoyalMgr.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 배틀 로얄 매니져 클래스
//

#if !defined(_CBATTLEROYALMGR_H)
#define _CBATTLEROYALMGR_H

#include "../CommonLogic.h"
#include "./CBattleRoyal.h"

class CBattleRoyal;
class CDailyEventTimer;
class NUpdateTimer;

class CBattleRoyalMgr {
public:
	CBattleRoyalMgr(SI32 siGameMode = GAMEMODE_CLIENT);
	~CBattleRoyalMgr();

	BOOL	Create();  // 배틀 로얄 생성
	void	Destroy(); // 배틀 로얄 제거
	BOOL	Action();  // 프레임 마다 수행

	// 캐릭터에 맞는 배틀로얄 포인터 얻기
	CBattleRoyal*	GetBattleRoyal(cltCharServer* pclChar);
	// GM 명령어로 강제로 배틀로얄 실행 시키기,
	BOOL			OperatorGMBattleRoyal();
	// GM 명령어로 배틀로얄 게임 모드 제어 하기
	BOOL			SetGMGameMode(SI32 siMode);
	// 다음 단계로 넘기기
	void			SetNextStep();
private:
	static const SI32 WAITTIME = 1000 * 60 * 4;
	static const SI32 STARTTIME = 1000 * 60 * 1;
	/*static const SI32 WAITTIME = 1000 ;
	static const SI32 STARTTIME = 1000 * 60 * 5 - 1000;*/
	static const SI32 GAMETIME = 1000 * 60 * 10;
	static const SI32 REWARDTIME = 1000 * 60 * 1;
	static const SI32 DAYTOHOURS = 24;
	static const SI32 EVENTREFRESHTIME = 3;

	// 모드 플레그
	SI32	m_siGameMode;

	CBattleRoyal*		m_pclMabriumBattleRoyal;		// 마브리움 배틀 로얄
	CBattleRoyal*		m_pclGrandMabriumBattleRoyal;	// 그랑마브리움 배틀 로얄

	CDailyEventTimer	m_clEventWait;			// 게임 대기 상태 타이머
	NUpdateTimer		m_clMarbriumChangeStateTimer;		// 마브리움 상태 변환 타이머
	NUpdateTimer		m_clGrandMabriumChangeStateTimer;	// 그랑마브리움 상태 변환 타이머

	bool				m_bGMOperation;
	bool				m_bNextStep;
};

#endif  //_CBATTLEROYALMGR_H
