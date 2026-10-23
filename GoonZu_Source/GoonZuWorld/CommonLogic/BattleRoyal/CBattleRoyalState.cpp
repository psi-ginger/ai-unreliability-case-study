#include ".\cbattleroyalstate.h"

CBattleRoyalState::CBattleRoyalState(void)
{
	m_pclGameMode = NULL;
	m_pclReward = NULL;
	m_pclStadium = NULL;
}

CBattleRoyalState::~CBattleRoyalState(void)
{
}

BOOL	CBattleRoyalState::Init(CBattleRoyalGameMode* pclGameMode, CBattleRoyalReward* pclReward, CBattleRoyalStadiumServer* pclStadium)
{
	if( NULL == pclGameMode)
		return FALSE;
	if( NULL == pclReward)
		return FALSE;
	if( NULL == pclStadium)
		return FALSE;

	m_pclGameMode = pclGameMode;
	m_pclReward = pclReward;
	m_pclStadium = pclStadium;

	return TRUE;
}