#pragma once

#include "./CAbstractState.h"
#include "./CBattleRoyalGameMode.h"
#include "./CBattleRoyalReward.h"
#include "./CBattleRoyalStadium.h"

class CAbstractState;
class CBattleRoyalGameMode;
class CBattleRoyalReward;
class CBattleRoyalStadium;

class CBattleRoyalState : public CAbstractState
{
public:
	CBattleRoyalState(void);
	virtual ~CBattleRoyalState(void);

	BOOL	Init(CBattleRoyalGameMode* pclGameMode, CBattleRoyalReward* pclReward, CBattleRoyalStadiumServer* pclStadium);
protected:
	CBattleRoyalGameMode* m_pclGameMode;
	CBattleRoyalReward* m_pclReward;
	CBattleRoyalStadiumServer* m_pclStadium;
};
