#include ".\CGameStateForBattleRoyal.h"

CGameStateForBattleRoyal::CGameStateForBattleRoyal(void)
{
}

CGameStateForBattleRoyal::~CGameStateForBattleRoyal(void)
{
}

BOOL	CGameStateForBattleRoyal::Handle()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	// 1. 유저 입장 불가능 처리 - 경기장
	m_pclStadium->CloseGate();
	// 2. 참가 유저 리스트 저장 - 경기장
	if(FALSE == m_pclStadium->SaveGamerList())
		return FALSE;
	// 3. 몬스터를 생성한다. - 경기장
	if(FALSE == m_pclStadium->CreateMob())
		return FALSE;
	// 4. 게임 모드 설정 적용 - 게임 모드
	if(FALSE == m_pclGameMode->Apply())
		return FALSE;
	// 5. 하이드 모드 상태의 유저를 보여준다. - 게임 모드
	if(FALSE == m_pclGameMode->ChangeVisibleMode())
		return FALSE;
	
	return TRUE;
}