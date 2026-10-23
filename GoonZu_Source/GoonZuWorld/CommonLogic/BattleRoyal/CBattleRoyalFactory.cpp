#include "./CBattleRoyalFactory.h"
#include "./CMabriumBattleRoyal.h"
#include "./CGrandMabriumBattleRoyal.h"

// 배틀로얄 생성
CBattleRoyal*	CBattleRoyalFactory::CreateBattleRoyal(SI32 siKind)
{
	switch(siKind)
	{
	case MABRIUM:
		{
			return new CMabriumBattleRoyal();
		}
	case GRANDMABRIUM:
		{
			return new CGrandMabriumBattleRoyal();
		}
	}
	return NULL; 
}

// 배틀로얄 제거
BOOL	CBattleRoyalFactory::DestroyBattleRoyal(CBattleRoyal* pclBattleRoyal)
{
	if( NULL == pclBattleRoyal)
		return FALSE;

	delete pclBattleRoyal;
	return TRUE;
}