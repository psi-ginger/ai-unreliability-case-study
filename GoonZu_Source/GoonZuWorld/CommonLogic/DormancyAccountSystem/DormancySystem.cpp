#include ".\dormancysystem.h"

CDormancySystem::~CDormancySystem(void)
{
	if( GAMEMODE_SERVER == m_siGameMode)
	{
		if(NULL != m_pclDormancyEquipItemReward)
		{
			delete (m_pclDormancyEquipItemReward);
			m_pclDormancyEquipItemReward = NULL;
		}
		if(NULL != m_pclDormancyPointReward)
		{
			delete (m_pclDormancyPointReward);
			m_pclDormancyPointReward = NULL;
		}
	}
	else
	{
		if(NULL != m_pclDormancyEquipItemReward)
		{
			delete (m_pclDormancyEquipItemReward);
			m_pclDormancyEquipItemReward = NULL;
		}
		if(NULL != m_pclDormancyPointReward)
		{
			delete (m_pclDormancyPointReward);
			m_pclDormancyPointReward = NULL;
		}
	}
}

CDormancySystem::CDormancySystem(SI32 siGameMode)
{
	// 휴면계정보상장비 클래스 
	m_pclDormancyEquipItemReward = NULL;
	// 휴면계정포인트 클래스 
	m_pclDormancyPointReward = NULL;
	// 게임 모드
	m_siGameMode = siGameMode;
}

// 휴면계정보상장비 클래스 참조자 얻기
const CDormancyEquipItemReward *CDormancySystem::GetDormancyEquipItemReward() const
{
	return m_pclDormancyEquipItemReward;
}

// 휴면계정포인트 클래스 참조자 얻기
const CDormancyPointReward *CDormancySystem::GetDormancyPointReward() const
{
	return m_pclDormancyPointReward;
}

//생성
BOOL	CDormancySystem::Create()
{
	if(NULL != m_pclDormancyEquipItemReward)
		return FALSE;
	if(NULL != m_pclDormancyPointReward)
		return FALSE;

	if( GAMEMODE_SERVER == m_siGameMode)
	{
		m_pclDormancyEquipItemReward = (CDormancyEquipItemReward*) new CDormancyEquipItemRewardServer();
		m_pclDormancyPointReward =(CDormancyPointReward*) new CDormancyPointRewardServer();
	}
	else
	{
		m_pclDormancyEquipItemReward = (CDormancyEquipItemReward*) new CDormancyEquipItemRewardClient();
		m_pclDormancyPointReward =(CDormancyPointReward*) new CDormancyPointRewardClient();
	}
	
	if(NULL == m_pclDormancyEquipItemReward)
		return FALSE;
	if(NULL == m_pclDormancyPointReward)
		return FALSE;

	if(!m_pclDormancyEquipItemReward->Create())
	{
		return FALSE;
	}
	if(!m_pclDormancyPointReward->Create())
	{
		return FALSE;
	}

	return TRUE;
}

//게임 모드
SI32	CDormancySystem::GetGameMode()
{
	return m_siGameMode;
}
