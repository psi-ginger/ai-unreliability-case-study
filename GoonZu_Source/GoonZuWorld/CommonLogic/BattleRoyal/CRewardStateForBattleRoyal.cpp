#include ".\CRewardStateForBattleRoyal.h"
#include "../common/Char/CharServer/Char-Server.h"

CRewardStateForBattleRoyal::CRewardStateForBattleRoyal(void)
{
}

CRewardStateForBattleRoyal::~CRewardStateForBattleRoyal(void)
{
}

BOOL	CRewardStateForBattleRoyal::Handle()
{
	if( NULL == m_pclStadium)
		return FALSE;
	if( NULL == m_pclReward)
		return FALSE;
	if( NULL == m_pclGameMode)
		return FALSE;

	// 1. 경기 결과 결정. - 경기장 . 보상
	if(FALSE == m_pclStadium->ProcessResult())
		return FALSE;
	cltCharCommon* pclChar = m_pclStadium->GetWinner();
	if( NULL !=  pclChar)
	{
		if(FALSE == m_pclReward->GiveRewardToWinner(pclChar))
			return FALSE;
	}
	
	// 2. 남아 있는 유저를 내보낸다.. - 경기장
	if(FALSE == m_pclStadium->BanGamer())
		return FALSE;
	
	// 3. 결과 공지를 전체 팝업으로 알려준다.. - 경기장
	if(FALSE == m_pclStadium->NoticeResult())
		return FALSE;
	return TRUE;
}