#include ".\CGrandMabriumBattleRoyal.h"
#include "./CGrandMabriumBattleRoyalGameMode.h"
#include "./CGrandMabriumBattleRoyalReward.h"
#include "./CGrandMabriumBattleRoyalStadium.h"

CGrandMabriumBattleRoyal::CGrandMabriumBattleRoyal(void)
{
	
}

CGrandMabriumBattleRoyal::~CGrandMabriumBattleRoyal(void)
{

}

//	생성
BOOL	CGrandMabriumBattleRoyal::Create()
{
	if( NULL != m_pclGameMode)
		return FALSE;
	if( NULL != m_pclReward)
		return FALSE;
	if( NULL != m_pclStadium)
		return FALSE;

	m_pclGameMode = new CGrandMabriumBattleRoyalGameMode();
	if( NULL == m_pclGameMode)
	{
		return FALSE;
	}

	m_pclReward = new CGrandMabriumBattleRoyalReward();
	if( NULL == m_pclReward)
	{
		return FALSE;
	}
	m_pclReward->Create();

	m_pclStadium = new CGrandMabriumBattleRoyalStadiumServer();
	if( NULL == m_pclStadium)
	{
		return FALSE;
	}
	m_pclStadium->Init();
	m_pclGameMode->SetGamers(m_pclStadium->GetPersonChangedData(), CBattleRoyalStadiumServer::MAX_GAMER_COUNT, m_pclStadium->GetMobData());
		
	// 상태 객체 초기화
	InitState();

	return TRUE;
}


