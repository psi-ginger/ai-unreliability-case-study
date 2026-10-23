#include "./CGrandMabriumBattleRoyalGameMode.h"


CGrandMabriumBattleRoyalGameMode::CGrandMabriumBattleRoyalGameMode()
{
	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword2");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle2");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar2");
}
CGrandMabriumBattleRoyalGameMode::~CGrandMabriumBattleRoyalGameMode()
{

}