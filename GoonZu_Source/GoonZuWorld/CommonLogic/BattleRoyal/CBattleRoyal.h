//
//
//  @ File Name : CBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 배틀로얄 처리 클래스
//


#if !defined(_CBATTLEROYAL_H)
#define _CBATTLEROYAL_H

#include "../CommonLogic.h"
#include "./CWaitStateForBattleRoyal.h"
#include "./CGameStateForBattleRoyal.h"
#include "./CEndStateForBattleRoyal.h"
#include "./CRewardStateForBattleRoyal.h"
#include "./CStartStateForBattleRoyal.h"

#include "./CAbstractState.h"

#include "./CBattleRoyalGameMode.h"
#include "./CBattleRoyalReward.h"
#include "./CBattleRoyalStadium.h"

#include "../common/DailyEventTimer/DailyEventTimer.h"

class CWaitStateForBattleRoyal;
class CGameStateForBattleRoyal;
class CEndStateForBattleRoyal;
class CRewardStateForBattleRoyal;
class CStartStateForBattleRoyal;

class CAbstractState;

class CBattleRoyalGameMode;
class CBattleRoyalReward;
class CBattleRoyalStadium;

class CDailyEventTimer;

enum
{
	BATTLEROYAL_ENTERERROR_ISOPENED = 2,
	BATTLEROYAL_ENTERERROR_WASJOINED,
	BATTLERPYAL_ENTERERROR_ISMAKING,
	BATTLERPYAL_ENTERERROR_TAKEDOUTSUMMON,
	BATTLERPYAL_ENTERERROR_FULLEDGAMER,
	BATTLERPYAL_ENTERERROR_JOINPARTY
};

class CBattleRoyal {

public:
	CBattleRoyal();
	virtual ~CBattleRoyal();

	virtual	BOOL	Create() = 0;	// 생성

	// 상태 관련 ===================================================
	BOOL	Waiting();	// 대기 상태로
	BOOL	Gaming();	// 게임 상태로
	BOOL	Ending();	// 종료 상태로
	BOOL	Rewarding();// 보상 상태로
	BOOL	Starting(); // 시작 상태로
	BOOL	Action();	// 행동

	BOOL	IsWaiteState();	// 대기상태인지
	BOOL	IsGamingState();// 게임상태인지
	BOOL	IsEndState();	// 종료상태인지
	BOOL	IsRewardState();// 보상상태인지
	BOOL	IsStartState(); // 시작상태인지
	// =============================================================

	BOOL	CanEnter(cltCharServer* pclChar);	// 입장 가능한지  
	BOOL	GetCanEnterLevel(UI32* uiMinLevel, UI32* uiMaxLevel);	// 입장 가능 레벨
	BOOL	GiveItemToKiller(cltCharServer* pclChar); // 배틀로얄 중에 유저를 죽인 사람에게 아이템을 준다.(100% 확률 아님)
	SI32	GetStadiumMapIndex();				// 맵 인덱스
	BOOL	CheckOutGamer();					// 경기장을 나갔는지 지속적으로 체크
	BOOL	IsEarlyEnd();
	BOOL	DieMob();
	void	SetNextStartTime(SI32 siNextStartTime);
	SI32	GetNextStartRemainedTime();
	BOOL	CanUseHealItem();	// 힐링아이템을 사용 할 수 있는 지
	BOOL	SetGameTime(SI32 siTime);	// 게임 플레이 시간 설정
	BOOL	IsExistStadium(cltCharServer* pclChar);
	BOOL	ActionGameMode();
	BOOL	SetGMGameMode(SI32 siMode);
	BOOL	TakeinHorse(cltCharServer* pclChar);	// 경기장에 들어갈 때 말에서 내려 있다면 강제로 말 넣기
	BOOL	RestorChar(cltCharServer* pclChar);		// 경기장에 들어와서 변한 유저 정보를 원상태로 돌린다.
	BOOL	TransformChar(cltCharServer* pclChar);		// 유저를 몹으로 변신 시킨다.
	static	void WarpFrontOfStadium(cltCharServer* pclChar);	// 경기장 앞으로 이동
protected:

	void	InitState();	// 상태 객체 초기화

	CWaitStateForBattleRoyal	m_clWaitState;	// 대기 상태 객체
	CGameStateForBattleRoyal	m_clGameState;	// 게임 상태 객체
	CEndStateForBattleRoyal		m_clEndState;	// 종료 상태 객체
	CRewardStateForBattleRoyal	m_clRewardState;// 보상 상태 객체
	CStartStateForBattleRoyal	m_clStartState; // 시작 상태 객체

	CAbstractState*				m_pclState;		// 현재 상태

	CBattleRoyalGameMode*		m_pclGameMode;	// 게임 모드
	CBattleRoyalReward*			m_pclReward;	// 보상
	CBattleRoyalStadiumServer*	m_pclStadium;	// 경기장

	SI32						m_siNextStartTime;	// 다음 경기 시작 시간
};

#endif  //_CBATTLEROYAL_H