#include ".\CWaitStateForBattleRoyal.h"

CWaitStateForBattleRoyal::CWaitStateForBattleRoyal(void)
{
}

CWaitStateForBattleRoyal::~CWaitStateForBattleRoyal(void)
{
}

BOOL	CWaitStateForBattleRoyal::Handle()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	// 1. 정보 초기화
	m_pclStadium->InitData();

	// 2. 전체 공지 메시지로 5분 후 게임 시작을 알린다. - 경기장
	if(FALSE == m_pclStadium->NoticeWait())
		return FALSE;

	// 3. 게임 모드 설정 - 게임 모드
	if(FALSE == m_pclGameMode->DetermineMode())
		return FALSE;
	return TRUE;
}
