#include ".\CStartStateForBattleRoyal.h"

CStartStateForBattleRoyal::CStartStateForBattleRoyal(void)
{
}

CStartStateForBattleRoyal::~CStartStateForBattleRoyal(void)
{
}

BOOL	CStartStateForBattleRoyal::Handle()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	// 1. 경기장 입구를 연다. - 경기장
	m_pclStadium->OpenGate();
	// 2. 전체 공지 메시지로 결투장 통로가 열림을 알린다. - 경기장
	if(FALSE == m_pclStadium->NoticeOpen())
		return FALSE;
	return TRUE;
}