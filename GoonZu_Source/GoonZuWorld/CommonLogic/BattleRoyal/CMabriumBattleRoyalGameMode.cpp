#include "./CMabriumBattleRoyalGameMode.h"


CMabriumBattleRoyalGameMode::CMabriumBattleRoyalGameMode()
{
	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar");
}
CMabriumBattleRoyalGameMode::~CMabriumBattleRoyalGameMode()
{

}