//---------------------------------
// 2003/6/9 김태곤
//---------------------------------

#include <CommonLogic.h>
#include "Char\CharClient\Char-Client.h"

#include "..\resource.h"

// 건물
#include "..\CommonLogic\Cityhall\Cityhall.h"
#include "..\CommonLogic\Bank\Bank-Manager.h"
#include "..\CommonLogic\Stock\Stock.h"
#include "..\CommonLogic\Market\Market.h"
#include "..\CommonLogic\House\House.h"
#include "..\CommonLogic\Hunt\Hunt.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"
#include "..\CommonLogic\Port\Port.h"
#include "..\CommonLogic\HorseMarket\HorseMarket.h"
#include "..\CommonLogic\RealEstate\RealEstate.h"
#include "..\CommonLogic\SummonMarket\SummonMarket.h"
#include "..\CommonLogic\ShipMarket\ShipMarket.h"
#include "..\CommonLogic\Land\Land.h"
#include "..\CommonLogic\Feast\Feast.h"
#include "..\CommonLogic\SummonHeroMarket\SummonHeroMarket.h"
#include "..\CommonLogic\Mine\Mine.h"
#include "..\CommonLogic\Guild\Guild.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic*		pclClient;

//-----------------------------------------------------------------
// cltBankManager
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticBankDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// 전장 행수가 아니면 TabCtrl을 하나 줄여서 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_BANK, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclBankManager->siTabCtrlNum	= MAX_BANK_TAB_NUMBER;
	}
	else
	{
		pclClient->pclBankManager->siTabCtrlNum	= MAX_BANK_TAB_NUMBER-1;
	}

	return pclClient->pclBankManager->DlgProc(hDlg, iMsg, wParam, lParam);
}




//-----------------------------------------------------------------
// cltStockManager
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticStockDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	//------------------------------------------------
	// 여각 행수가 아니면 TabCtrl을 하나 줄여서 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_STOCK, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclStockManager->siTabCtrlNum	= MAX_STOCK_TAB_NUMBER;
	}
	else
	{
		pclClient->pclStockManager->siTabCtrlNum	= MAX_STOCK_TAB_NUMBER-1;
	}

	return pclClient->pclStockManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltCityHallManager
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticCityHallDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// 대행수가 아니면 TabCtrl을 하나 줄여서 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_CITYHALL, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclCityHallManager->siTabCtrlNum	= MAX_CITYHALL_TAB_NUMBER;
	}
	else
	{
		pclClient->pclCityHallManager->siTabCtrlNum	= MAX_CITYHALL_TAB_NUMBER - MAX_CITYHALL_CHIEF_TAB_NUMBER;
	}

	return pclClient->pclCityHallManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


BOOL cltCommonLogic::CreateCityHallDlg(SI32 charunique)
{

	if(pclCityHallManager == NULL)return 0;

	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(pclCityHallManager->m_hStrDlg))
	{
		pclCityHallManager->CharUnique	= charunique;

		pclCityHallManager->m_hStrDlg = CreateDialog(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_BANK), GetHwnd(), StaticCityHallDlgProc);

		if(pclCityHallManager->DlgRectInitSwitch == TRUE)
		{
			MoveWindow(pclCityHallManager->m_hStrDlg, 
				pclCityHallManager->DlgRect.left, pclCityHallManager->DlgRect.top, 
				pclCityHallManager->DlgRect.right - pclCityHallManager->DlgRect.left, 
				pclCityHallManager->DlgRect.bottom - pclCityHallManager->DlgRect.top, FALSE);
		}

		ShowWindow(pclCityHallManager->m_hStrDlg, SW_SHOW);

		return TRUE;
	}
	else
	{
		DestroyWindow(pclCityHallManager->m_hStrDlg);
	}


	return 0;
}


//-----------------------------------------------------------------
// cltMarketManager
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// 객주행수가 아니면 TabCtrl을 하나 줄여서 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_MARKET, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclMarketManager->siTabCtrlNum	= MAX_MARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclMarketManager->siTabCtrlNum	= MAX_MARKET_TAB_NUMBER-1;
	}

	return pclClient->pclMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltHouseManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticHouseDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// 시전 행수가 아니면 TabCtrl을 3개만 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_HOUSE, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclHouseManager->siTabCtrlNum	= MAX_HOUSE_TAB_NUMBER;
	}
	else
	{
		pclClient->pclHouseManager->siTabCtrlNum	= MAX_HOUSE_TAB_NUMBER - 2;
	}


	return pclClient->pclHouseManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltHuntManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticHuntDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------
	// 행수가 아니면 TabCtrl을 2개만 사용한다. 
	//------------------------------------------------
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_HUNT, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclHuntManager->siTabCtrlNum	= MAX_HUNT_TAB_NUMBER;
	}
	else
	{
		pclClient->pclHuntManager->siTabCtrlNum	= MAX_HUNT_TAB_NUMBER - 2;
	}


	return pclClient->pclHuntManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltPostOfficeManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticPostOfficeDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_POSTOFFICE, pChar->GetCurrentVillageUnique());
	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclPostOfficeManager->siTabCtrlNum	= MAX_POSTOFFICE_TAB_NUMBER;
	}
	else
	{
		pclClient->pclPostOfficeManager->siTabCtrlNum	= MAX_POSTOFFICE_TAB_NUMBER - 1;
	}


	return pclClient->pclPostOfficeManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltPortManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticPortDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_PORT, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclPortManager->siTabCtrlNum	= MAX_PORT_TAB_NUMBER;
	}
	else
	{
		pclClient->pclPortManager->siTabCtrlNum	= MAX_PORT_TAB_NUMBER - 1;
	}


	return pclClient->pclPortManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltHorseMarketManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticHorseMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_HORSEMARKET, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclHorseMarketManager->siTabCtrlNum	= MAX_HORSEMARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclHorseMarketManager->siTabCtrlNum	= MAX_HORSEMARKET_TAB_NUMBER - 1;
	}


	return pclClient->pclHorseMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltRealEstateMarketManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticRealEstateMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_REALESTATEMARKET, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclRealEstateMarketManager->siTabCtrlNum	= MAX_REALESTATEMARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclRealEstateMarketManager->siTabCtrlNum	= MAX_REALESTATEMARKET_TAB_NUMBER - 1;
	}


	return pclClient->pclRealEstateMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltSummonMarketManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticSummonMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_SUMMONMARKET, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclSummonMarketManager->siTabCtrlNum	= MAX_SUMMONMARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclSummonMarketManager->siTabCtrlNum	= MAX_SUMMONMARKET_TAB_NUMBER - 1;
	}


	return pclClient->pclSummonMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltShipMarketManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticShipMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_SHIPMARKET, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclShipMarketManager->siTabCtrlNum	= MAX_SHIPMARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclShipMarketManager->siTabCtrlNum	= MAX_SHIPMARKET_TAB_NUMBER - 1;
	}


	return pclClient->pclShipMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltLandManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticLandDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_LAND, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclLandManager->siTabCtrlNum	= MAX_LAND_TAB_NUMBER;
	}
	else
	{
		pclClient->pclLandManager->siTabCtrlNum	= MAX_LAND_TAB_NUMBER - 2;
	}


	return pclClient->pclLandManager->DlgProc(hDlg, iMsg, wParam, lParam);
}



//-----------------------------------------------------------------
// cltFeastManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticFeastDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_FEAST, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclFeastManager->siTabCtrlNum	= MAX_FEAST_TAB_NUMBER;
	}
	else
	{
		pclClient->pclFeastManager->siTabCtrlNum	= MAX_FEAST_TAB_NUMBER - 3;
	}


	return pclClient->pclFeastManager->DlgProc(hDlg, iMsg, wParam, lParam);
}





//-----------------------------------------------------------------
// cltSummonHeroMarketManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticSummonHeroMarketDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_SUMMONHEROMARKET, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclSummonHeroMarketManager->siTabCtrlNum	= MAX_SUMMONHEROMARKET_TAB_NUMBER;
	}
	else
	{
		pclClient->pclSummonHeroMarketManager->siTabCtrlNum	= MAX_SUMMONHEROMARKET_TAB_NUMBER - 1;
	}


	return pclClient->pclSummonHeroMarketManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// cltMineManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticMineDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank(RANKTYPE_MINE, pChar->GetCurrentVillageUnique());

	if(pChar->pclCI->clCharRank.IsSame(&clrank) == TRUE)
	{
		pclClient->pclMineManager->siTabCtrlNum	= MAX_MINE_TAB_NUMBER;
	}
	else
	{
		pclClient->pclMineManager->siTabCtrlNum	= MAX_MINE_TAB_NUMBER - 2;
	}


	return pclClient->pclMineManager->DlgProc(hDlg, iMsg, wParam, lParam);
}


//-----------------------------------------------------------------
// cltGuildStrManager	
//-----------------------------------------------------------------
BOOL CALLBACK cltCommonLogic::StaticGuildStrDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	cltCharClient* pChar = (cltCharClient*)pclClient->pclCM->CR[id];

	cltSimpleRank clrank( RANKTYPE_GUILD, pChar->GetCurrentVillageUnique() );

	if( pChar->pclCI->clCharRank.IsSame( &clrank ) == TRUE )
	{
		pclClient->pclGuildStructManager->siTabCtrlNum	= MAX_GUILDSTRUCT_TAB_NUMBER;
	}
	else
	{
		pclClient->pclGuildStructManager->siTabCtrlNum	= MAX_GUILDSTRUCT_TAB_NUMBER - 1;
	}


	return pclClient->pclGuildStructManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

