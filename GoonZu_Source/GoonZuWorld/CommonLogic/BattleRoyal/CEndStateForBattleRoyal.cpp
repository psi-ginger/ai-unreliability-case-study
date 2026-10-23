#include ".\CEndStateForBattleRoyal.h"

CEndStateForBattleRoyal::CEndStateForBattleRoyal(void)
{
}

CEndStateForBattleRoyal::~CEndStateForBattleRoyal(void)
{
}

BOOL	CEndStateForBattleRoyal::Handle()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	// 1. Έχ Α¦°Ε
	if(FALSE == m_pclStadium->RemoveMob())
		return FALSE;

	return TRUE;
}