#include "./CGrandMabriumBattleRoyalStadium.h"

CGrandMabriumBattleRoyalStadiumServer::CGrandMabriumBattleRoyalStadiumServer()
{
	m_siMapIndex = MAPINDEX_BATTLEROYAL_GRANDMABRIUM;
	
	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword2");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle2");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar2");
}
CGrandMabriumBattleRoyalStadiumServer::~CGrandMabriumBattleRoyalStadiumServer()
{

}

// 배틀로얄 대기를 알린다.
BOOL	CGrandMabriumBattleRoyalStadiumServer::NoticeWait()
{
	return TRUE; 
}

// 경기장이 열림을 알린다.
BOOL	CGrandMabriumBattleRoyalStadiumServer::NoticeOpen()
{
	return TRUE; 
}

void	CGrandMabriumBattleRoyalStadiumServer::OpenGate()
{
	CBattleRoyalStadium::OpenGate();
}

//======================================================================================================================

CGrandMabriumBattleRoyalStadiumClient::CGrandMabriumBattleRoyalStadiumClient()
{
	m_siMapIndex = MAPINDEX_BATTLEROYAL_GRANDMABRIUM;

	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword2");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle2");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar2");
}
CGrandMabriumBattleRoyalStadiumClient::~CGrandMabriumBattleRoyalStadiumClient()
{

}