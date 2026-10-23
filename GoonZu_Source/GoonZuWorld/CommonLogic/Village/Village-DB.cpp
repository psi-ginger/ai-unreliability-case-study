//---------------------------------
// 2003/5/27 김태곤
//---------------------------------

#include "..\CommonLogic\CommonLogic.h"
#include "Village.h"

// 건물
#include "..\CommonLogic\Cityhall\Cityhall.h"
#include "..\CommonLogic\Bank\Bank-Manager.h"
#include "..\CommonLogic\Stock\Stock.h"
#include "..\CommonLogic\Market\Market.h"
#include "..\CommonLogic\House\House.h"
#include "..\CommonLogic\Hunt\Hunt.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"
#include "..\CommonLogic\HorseMarket\HorseMarket.h"
#include "..\CommonLogic\RealEstate\RealEstate.h"
#include "..\CommonLogic\SummonMarket\SummonMarket.h"
#include "..\CommonLogic\Land\Land.h"
#include "..\CommonLogic\Feast\Feast.h"
#include "..\CommonLogic\SummonHeroMarket\SummonHeroMarket.h"
#include "..\CommonLogic\Mine\Mine.h"
#include "..\CommonLogic\Guild\Guild.h"

extern cltCommonLogic* pclClient;

// DB에서 전장 정보를 읽어와서 업데이트 한다. 
// 이 함수는 클라이언트, 서버 공히 최초 마을 초기화시 한번만 수행하게 된다. 
BOOL cltVillageInfo::LoadBankFromDB()
{
	// 전장을 만든다. 
	pclVillageManager->pclBankManager->CreateBank(siVillageUnique);

	return TRUE;
}


// DB에서 여각 정보를 읽어와서 업데이트 한다. 
BOOL cltVillageInfo::LoadStockFromDB()
{
	// 여각을 만든다
	pclVillageManager->pclStockManager->CreateStock(siVillageUnique);
	return TRUE;
}

// DB에서 여각 정보를 읽어와서 업데이트 한다. 
BOOL cltVillageInfo::LoadMarketFromDB()
{
	// 객주를 만든다. 
	pclVillageManager->pclMarketManager->CreateMarket(siVillageUnique);
	
	return TRUE;
}

BOOL cltVillageInfo::LoadNewMarketFromDB()
{
	//[진성] 통합마켓을 만든다. 
	//pclVillageManager->pclNewMarketManager->CreateNewMarket(siVillageUnique);
	
	pclVillageManager->pclNewMarketManager->CreateNewMarket(siVillageUnique);

	return TRUE;
}

// DB에서 시전 정보를 읽어와서 업데이트 한다. 
BOOL cltVillageInfo::LoadHouseFromDB()
{
	// 시전을  만든다. 
	pclVillageManager->pclHouseManager->CreateHouse(siVillageUnique);
	
	return TRUE;
}



// DB에서 상단 정보를 읽어와서 업데이트 한다. 
BOOL cltVillageInfo::LoadCityHallFromDB()
{
	// 상단을 만든다. 
	pclVillageManager->pclCityHallManager->CreateCityHall(siVillageUnique);

	return TRUE;
}

// DB에서 수렵장 정보를 읽어와서 업데이트 한다. 
BOOL cltVillageInfo::LoadHuntFromDB()
{
	// 수렵장을 만든다. 
	pclVillageManager->pclHuntManager->CreateHunt(siVillageUnique);
	
	return TRUE;
}

// DB에서 역참 정보를 읽어와서 업데이트 한다.
BOOL cltVillageInfo::LoadPostOfficeFromDB()
{
	// 역참을 만든다. 
	pclVillageManager->pclPostOfficeManager->CreatePostOffice(siVillageUnique);
	
	return TRUE;
}

// DB에서 사복시 정보를 읽어와서 업데이트 한다.
BOOL cltVillageInfo::LoadHorseMarketFromDB()
{
	// 사복시를 만든다. 
	pclVillageManager->pclHorseMarketManager->CreateHorseMarket( siVillageUnique );

	return TRUE;
}

BOOL cltVillageInfo::LoadRealEstateMarketFromDB()
{
	pclVillageManager->pclRealEstateMarketManager->CreateRealEstateMarket( siVillageUnique );

	return TRUE;
}

BOOL cltVillageInfo::LoadSummonMarketFromDB()
{
	pclVillageManager->pclSummonMarketManager->CreateSummonMarket( siVillageUnique );

	return TRUE;
}


BOOL cltVillageInfo::LoadLandFromDB()
{
	pclVillageManager->pclLandManager->CreateLand( siVillageUnique );

	return TRUE;
}

BOOL cltVillageInfo::LoadFeastFromDB()
{
	pclVillageManager->pclFeastManager->CreateFeast( siVillageUnique );

	return TRUE;
}


BOOL cltVillageInfo::LoadSummonHeroMarketFromDB()
{
	pclVillageManager->pclSummonHeroMarketManager->CreateSummonHeroMarket( siVillageUnique );

	return TRUE;
}

BOOL cltVillageInfo::LoadMineFromDB()
{
	pclVillageManager->pclMineManager->CreateMine( siVillageUnique );

	return TRUE;
}

BOOL cltVillageInfo::LoadGuildFromDB()
{
	pclVillageManager->pclGuildManager->CreateGuild( siVillageUnique );

	return TRUE;
}
