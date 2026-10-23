//---------------------------------
// 2004/10/20 김태곤
//---------------------------------

//------------------------------
// CommonLogic
//------------------------------
#include "CommonLogic.h"
#include "Village.h"
#include "Cityhall/Cityhall.h"
#include "../CommonLogic/TextMgr/TextMgr.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char/CharServer/Char-Server.h"
#include "Char/CharManager/CharManager.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-structure.h"
#include "NDataLoader.h"

// 건물
#include "../CommonLogic/Cityhall/Cityhall.h"
#include "../CommonLogic/Bank/Bank-Manager.h"
#include "../CommonLogic/Stock/Stock.h"
#include "../CommonLogic/Market/Market.h"
#include "../CommonLogic/House/House.h"
#include "../CommonLogic/Hunt/Hunt.h"
#include "../CommonLogic/PostOffice/PostOffice.h"
#include "../CommonLogic/HorseMarket/HorseMarket.h"
#include "../CommonLogic/RealEstate/RealEstate.h"
#include "../CommonLogic/SummonMarket/SummonMarket.h"
#include "../CommonLogic/Land/Land.h"
#include "../CommonLogic/Feast/Feast.h"
#include "../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"
#include "../CommonLogic/Mine/Mine.h"
#include "../CommonLogic/Guild/Guild.h"

//---------------------------------
// Server
//---------------------------------
#include "Server.h"

extern cltCommonLogic* pclClient;

//------------------------------------------------------------------------------
// cltVillageManager
//------------------------------------------------------------------------------
cltVillageManager::cltVillageManager(SI32 villagemode )
{
	SI32 i;
 
	pclBankManager				= pclClient->pclBankManager;
	pclStockManager				= pclClient->pclStockManager;
	pclCityHallManager			= pclClient->pclCityHallManager;
	pclMarketManager			= pclClient->pclMarketManager;
	pclHouseManager				= pclClient->pclHouseManager;
	pclHuntManager				= pclClient->pclHuntManager;
	pclPostOfficeManager		= pclClient->pclPostOfficeManager;
	pclHorseMarketManager		= pclClient->pclHorseMarketManager;
	pclRealEstateMarketManager	= pclClient->pclRealEstateMarketManager;
	pclSummonMarketManager		= pclClient->pclSummonMarketManager;
	pclLandManager				= pclClient->pclLandManager;
	pclFeastManager				= pclClient->pclFeastManager;
	pclSummonHeroMarketManager	= pclClient->pclSummonHeroMarketManager;
	pclMineManager				= pclClient->pclMineManager;
	pclGuildManager				= pclClient->pclGuildManager;
	pclNewMarketManager			= pclClient->pclNewMarketManager;	//[진성] 통합마켓.

	siVillageMode = villagemode;

	bCheckVisitLevelSwitch	= false;

	siInstanceProfitVillageunique = 0 ;
	siTempInstanceProfit = 0 ;
	dwLastInstanceProfit = 0 ;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		pclVillageInfo[i] = NULL;
	}

	// 각 마을의 적군 수를 파악한 마지막 시간. 
	dwLastEnemyNumCountClock	= 0;
	
	
	// 선전포고 마을 변수 초기화
	ResetDeclareWar();

	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 초기화.]
	BlackWarRecordInit();
}

cltVillageManager::~cltVillageManager()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			delete pclVillageInfo[i];
			pclVillageInfo[i] = NULL;
		}
	}

}

SI32 cltVillageManager::FindVillageUniqueFromName(TCHAR* pname)
{
	SI32 i;

	if(pname == NULL)return -1;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			if(_tcscmp(pname, pclVillageInfo[i]->szName) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}

BOOL cltVillageManager::IsAggressiveVillage( SI32 village1, SI32 village2 )
{
	if( village1 <= 0 || village1 >= MAX_VILLAGE_NUMBER ||
		village2 <= 0 || village2 >= MAX_VILLAGE_NUMBER ) return FALSE;

	if( siNowWarVillage[ village1 ] == village2 ||
		siNowWarVillage[ village2 ] == village1 ) return TRUE;

	return FALSE;
}


// 마을 정보를 삭제한다.
BOOL cltVillageManager::DeleteVillage()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			delete pclVillageInfo[i];
			pclVillageInfo[i] = NULL;
		}
	}
	return TRUE;
}


// unique값의 마을을 추가한다. 
BOOL cltVillageManager::AddVillage(SI32 siunique, TCHAR * pname, SI32 villageposx, SI32 villageposy, SI32 mapindex, SI32 season, bool bfarm, bool biron )
{

	if(pname == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Not Proper Village Name.(NULL)"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}
	
	if(pclVillageInfo[siunique] == NULL)
	{
		pclVillageInfo[siunique] = new cltVillageInfo(this, siVillageMode, siunique, 0,  pname, villageposx, villageposy, mapindex, season, bfarm, biron);
	}
	else
	{
		MsgBox(TEXT("AddVillage()"), TEXT("fd972jf:%d"), siunique);
	}

	return TRUE;
}

// 모든 마을을 삭제한다. 
void cltVillageManager::DelAllVillage()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i] == NULL)continue;

		delete pclVillageInfo[i];
		pclVillageInfo[i] = NULL;
	}

}


BOOL cltVillageManager::Action()
{
	SI32 i;
	
	//-------------------------------------------------
	// 점령 여부를 파악한다. 
	//-------------------------------------------------
	// 게임서버만 행한다. 
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		// 던전 이익금을 쌓는다. 10분간격
		if ( TABS(pclClient->CurrentClock - dwLastInstanceProfit ) > 600000 )
		{
			dwLastInstanceProfit = pclClient->CurrentClock ;

			// 던전 이익금을 db에 저장한다.
			if(siTempInstanceProfit)
			{

				sDBRequest_Change_InstanceProfit sendMsg( siTempInstanceProfit,false);
				((cltServer *)pclClient)->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );

				// 저장할 돈을 초기화 한다. 
				siTempInstanceProfit = 0;
			}
		}

		if(TABS(pclClient->CurrentClock - dwLastEnemyNumCountClock) > 3000)
		{
			dwLastEnemyNumCountClock = pclClient->CurrentClock; 
			
			for(i = 1;i < MAX_VILLAGE_NUMBER;i++)
			{
				//---------------------------------------
				// 모든 마을의 적군 수를 초기화한다. 
				//---------------------------------------
				if(pclVillageInfo[i])
				{
					pclVillageInfo[i]->siEnemyNum			= 0;
					pclVillageInfo[i]->siPersonalShopNum	= 0;
				}
			}
			
			
			SI32 id;
			
			SI32 index			   = 0;
			cltCharServer* pclchar = NULL;
			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;
				
				pclchar = (cltCharServer* )pclClient->pclCM->CR[id];
			
				SI32 mapindex = pclchar->GetMapIndex();

				if(mapindex <= 0)continue;
				if(mapindex >= MAX_VILLAGE_NUMBER)continue;

				// 마을안의 개인상점 개수 파악. 
				if(pclchar->bPersonalShopSwitch)
				{
					if(pclVillageInfo[mapindex])
					{
						pclVillageInfo[mapindex]->siPersonalShopNum++;
					}
				}
			}

		}

		if(pclClient->GetFrame() % 1000 == 0)
		{
			//-----------------------------------------------
			// 마을의 방문자 수를  근거로 마을 레벨을 구한다.
			//-----------------------------------------------
			CalcVisitLevel();
		}

	}
	
	for(i = 1;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			pclVillageInfo[i]->Action();
		}
	}

	return TRUE;
}


// 마을 주가를 설정한다. 
void cltVillageManager::SetCurrentStockPrice(SI32 villageunique, GMONEY price)
{
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)	return;

	if(pclVillageInfo[villageunique])
	{
		pclVillageInfo[villageunique]->SetCurrentStockPrice(price);
	}
}


//Person이 villageunique의 최대 주주인지 확인한다. 
BOOL cltVillageManager::IsMostBeholder(SI32 villageunique, SI32 personid)
{
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)	return FALSE;

	return pclVillageInfo[villageunique]->IsMostBeholder(personid);
}


// 마을의 이름을 얻어온다. 
TCHAR* cltVillageManager::GetName(SI32 villageunique)
{
	//if(villageunique <= 0)return NULL;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return NULL;
	if(pclVillageInfo[villageunique] == NULL)return NULL;

	return pclVillageInfo[villageunique]->szName;
}

// 마을의 이름을 근거로 맵인덱스를 찾는다. 
SI32 cltVillageManager::GetMapIndexFromName(TCHAR* name)
{
	SI32 villageunique = FindVillageUniqueFromName(name);
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return -1;

	return villageunique;
	//return pclVillageInfo[villageunique]->MapIndex;
}

// 임의의 마을을 찾는다. 
SI32 cltVillageManager::FindRandVillage()
{
	SI32 i;
	SI32 villagelist[MAX_VILLAGE_NUMBER];
	SI32 villagelistindex = 0;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i] == NULL)continue;

		villagelist[villagelistindex] = pclVillageInfo[i]->siVillageUnique;
		villagelistindex++;
	}

	if(villagelistindex)
	{
		return villagelist[rand() % villagelistindex];
	}

	return 0;
}

//====================================
// 성황신 
//====================================
// 각 마을의 성황신의 최대 Life를 얻어온다.
SI32 cltVillageManager::GetMaxLifeOfGodInVillage(SI32 villageunique)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 villagegodcharunique;

	if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
	{
		return ((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clPB.GetMaxLife();
	}
	
	return 0;
}
// 각 마을의 성황신의 Life를 설정한다.
BOOL cltVillageManager::SetLifeOfGodInVillage(SI32 villageunique, SI32 life)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 villagegodcharunique;

	if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->SetLife(life);
		return TRUE;
	}

	return FALSE;
}

// 각 마을의 성황신의 Mana를 설정한다.
BOOL cltVillageManager::SetManaOfGodInVillage(SI32 villageunique, SI32 mana)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 villagegodcharunique;

	if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->SetMana(mana);
		return TRUE;

	}

	return FALSE;
}

// 각 마을의 성황신의 Capa를 설정한다.
BOOL cltVillageManager::SetCapaOfGodInVillage(SI32 villageunique, SI32 capa)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 villagegodcharunique;

	if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa = capa;
		return TRUE;
	}

	return FALSE;
}

BOOL cltVillageManager::AdjustCapaOfGodInVillage(SI32 villageunique, SI32 adjustcapa)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 villagegodcharunique;

	// 성황신이 살아있을경우.
	if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa += adjustcapa;
		if( ((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa < 100 )
			((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa = 100;
		else if(((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa > MAX_CHARACTER_LEVEL * 2 + 100)
			((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.clGodInfo.siCapa = MAX_CHARACTER_LEVEL * 2 + 100;

		cltStrInfo *pStrInfo = ((cltServer*)pclClient)->GetStrInfo( villageunique,RANKTYPE_CITYHALL );

		//성황신이 내려가려나.일단 삭제
		/*pStrInfo->clCityhallStrInfo.siSymbolCapa = pStrInfo->clCityhallStrInfo.siSymbolCapa-2;*/

	}

	return FALSE;
}


// 마을의 성황신을 바꾼다. (villageunique 에서 villagegodkind 가 가능함을 이미 검사했어야 한다.)
BOOL cltVillageManager::ChangeVillageGod(SI32 villageunique, SI32 villagegodkind)
{
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)	return FALSE;

	// 해당 마을이 존재하고
	if(pclVillageInfo[villageunique])
	{
		// 해당 마을의 성황신이 존재할경우 
		SI32 villagegodcharunique;
		
		if(IsThereVillageGod(villageunique, &villagegodcharunique) == TRUE)
		{
			// 셀 내에서의 정보를 삭제한다. 
			pclClient->pclCM->CR[villagegodcharunique]->DeleteInCell();
			pclClient->pclCM->DeleteChar( villagegodcharunique );
			
		}

		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
		if(pclinfo == NULL)return FALSE;

		//=========================
		//재설정되어야하는 변수들
		//=========================
		// 지금 새로운 성황신을 설정한다.
		pclinfo->clCityhallStrInfo.siSymbolKind  = villagegodkind;
		pclinfo->clCityhallStrInfo.siSymbolCapa = 100;

		pclinfo->clCityhallStrInfo.siSymbolLife = 0;
		pclinfo->clCityhallStrInfo.siSymbolMana = 0;
		
		// 지금의 CharUnique 정보를 지운다.
		pclVillageInfo[villageunique]->siVillageGodCharUnique	= 0;

		// 새로운 성황신을 생성시킨다.
		pclVillageInfo[villageunique]->CreateVillageGod();
		
	}

	return FALSE;
}

// 모든 마을의 성황신의 상태를 저장한다.
void cltVillageManager::SetAllVillageGodtoDB()
{
	// 해당 마을이 존재하면 각 마을의 성황신 상태를 저장한다.
	for(int i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			pclVillageInfo[i]->SetVillageGodtoDB();
		}
	}

}



// 해당 마을에 VillageGod이 존재하는지 
BOOL cltVillageManager::IsThereVillageGod(SI32 villageunique, SI32 *siGodCharUnique)
{
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)	return FALSE;

	// 해당 마을이 존재하고
	if(pclVillageInfo[villageunique])
	{
		// 해당 마을의 God Unique ID를 얻어온다.
		SI32 villagegodcharunique = pclVillageInfo[villageunique]->siVillageGodCharUnique;
		// 해당 마을의 성황신이 지금 유효하다면 삭제한다.
		if(((cltServer*)pclClient)->pclCM->IsValidID(villagegodcharunique) == TRUE )
		{

			// 해당 마을의 정보를 얻어와서 성황신이 설정되어 있으면 
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
			if(pclinfo && pclinfo->clCityhallStrInfo.siSymbolKind >0)
			{
				// 해당 몬스터터가 해당 마을의 VillageGod 인지 검사한다.
				if( ((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->clIdentity.siIdentity  == IDENTITY_VILLAGEGOD &&
				    ((cltServer*)pclClient)->pclCM->CR[ villagegodcharunique ]->pclCI->clBI.siHomeVillage	  == villageunique	)
				{
					*siGodCharUnique = villagegodcharunique;
					return TRUE;
				}
			}
		}
	}

	*siGodCharUnique = 0;
	return FALSE;

}
//====================================
// 성문 
//====================================
// 각 마을의 성문의 최대 Life를 얻어온다.
SI32 cltVillageManager::GetMaxLifeOfCastleInVillage(SI32 villageunique)
{

	// 해당 마을의 성황신이 존재할경우 
	SI32 castlecharunique;

	if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
	{
		return ((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clPB.GetMaxLife();
	}

	return 0;
}
// 각 마을의 성문의 Life를 설정한다.
BOOL cltVillageManager::SetLifeOfCastleInVillage(SI32 villageunique, SI32 life)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 castlecharunique;

	if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->SetLife(life);
		return TRUE;
	}

	return FALSE;
}


// 각 마을의 성문의 Mana를 설정한다.
BOOL cltVillageManager::SetManaOfCastleInVillage(SI32 villageunique, SI32 mana)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 castlecharunique;

	if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->SetMana(mana);
		return TRUE;
	}

	return FALSE;
}
// 각 마을의 성문의 Capa를 설정한다.
BOOL cltVillageManager::SetCapaOfCastleInVillage(SI32 villageunique, SI32 capa)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 castlecharunique;

	if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa = capa;
		return TRUE;
	}
	
	return FALSE;
}

// 각 마을의 성문의 Capa를 변경한다.
BOOL cltVillageManager::AdjustCapaOfCastleInVillage(SI32 villageunique, SI32 adjustcapa)
{
	// 해당 마을의 성황신이 존재할경우 
	SI32 castlecharunique;

	if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
	{
		((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa += adjustcapa;
		if( ((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa < 100 )
			((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa = 100;
		else if(((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa > MAX_CHARACTER_LEVEL * 2 + 100)
			((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.clGodInfo.siCapa = MAX_CHARACTER_LEVEL * 2 + 100;

		return TRUE;
	}

	return FALSE;
}

// 마을의 성문  바꾼다. (villageunique 에서  castlekind 가 가능함을 이미 검사했어야 한다.)
BOOL cltVillageManager::ChangeCastle(SI32 villageunique, SI32 castlekind)
{
	// 해당 마을이 존재하고
	if(pclVillageInfo[villageunique])
	{
		// 해당 마을의 성황신이 존재할경우 
		SI32 castlecharunique;
		
		if(IsThereCastle(villageunique, &castlecharunique) == TRUE)
		{
			// 셀 내에서의 정보를 삭제한다. 
			pclClient->pclCM->CR[castlecharunique]->DeleteInCell();
			pclClient->pclCM->DeleteChar( castlecharunique );
		}

		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
		if(pclinfo == NULL)return FALSE;

		//=========================
		//재설정되어야하는 변수들
		//=========================
		// 지금 새로운 성문을 설정한다.
		pclinfo->clCityhallStrInfo.siCastleKind = castlekind;
		pclinfo->clCityhallStrInfo.siCastleLife = 0;
		pclinfo->clCityhallStrInfo.siCastleMana = 0;
		pclinfo->clCityhallStrInfo.siCastleCapa = 100;
		// 지금의 CharUnique 정보를 지운다.
		pclVillageInfo[villageunique]->siCastleCharUnique	= 0;

		// 새로운 성문을 생성시킨다.
		pclVillageInfo[villageunique]->CreateCastle();
		
	}

	return FALSE;
}


// 모든 마을의 성문 상태를 저장한다.
void cltVillageManager::SetAllCastletoDB()
{
	// 해당 마을이 존재하면 각 마을의 성황신 상태를 저장한다.
	for(int i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			pclVillageInfo[i]->SetCastletoDB();
		}
	}

}



// 해당 마을에 성벽이 존재하는지 
BOOL cltVillageManager::IsThereCastle(SI32 villageunique, SI32 *siCastleUnique)
{
	// 해당 마을이 존재하고
	if(pclVillageInfo[villageunique])
	{
		// 해당 마을의 God Unique ID를 얻어온다.
		SI32 castlecharunique = pclVillageInfo[villageunique]->siCastleCharUnique;
		// 해당 마을의 성황신이 지금 유효하다면 삭제한다.
		if(((cltServer*)pclClient)->pclCM->IsValidID(castlecharunique) == TRUE )
		{
			// 해당 마을의 정보를 얻어와서 성황신이 설정되어 있으면 
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
			if(pclinfo && pclinfo->clCityhallStrInfo.siCastleKind >0)
			{
				// 해당 몬스터터가 해당 마을의 VillageGod 인지 검사한다.
				if( ((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->clIdentity.siIdentity == IDENTITY_CASTLE &&
					((cltServer*)pclClient)->pclCM->CR[ castlecharunique ]->pclCI->clBI.siHomeVillage	 == villageunique)
				{
					*siCastleUnique = castlecharunique;
					return TRUE;
				}
			}
		}
	}

	*siCastleUnique = 0;
	return FALSE;

}
//====================================

// 주어진 좌표로부터 가까운 마을을 찾는다. 
SI32 cltVillageManager::FindNearVillage(SI32 x, SI32 y)
{
	SI32 villageunique = 0;
	SI32 length;
	SI32 minlength = 10000;
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i] == NULL)continue;

		//if(pclVillageInfo[i]->bEnemyCaptureSwitch)continue;

		length = max( TABS(x - pclVillageInfo[i]->PosX), TABS(y - pclVillageInfo[i]->PosY));
		if(length < minlength)
		{
			minlength = length;
			
			villageunique	= i;
		}
	}

	return villageunique;
}

// 마을 전공을 초기화한다. 
void cltVillageManager::InitWarKillNumber()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			pclVillageInfo[i]->siWarKillNumber = 0;
			pclVillageInfo[i]->bOsakaWarSwitch = false;
		}
	}

}

// 마을 전공을 소트한다. 
void cltVillageManager::WarKillNumberSort(SI16* psiVillageUnique,  SI16* psiVillageWarKillNumber)
{
	SI32 i;
	SI32 index = 0;
	bool bVillageCheckSwitch[MAX_VILLAGE_NUMBER];

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		bVillageCheckSwitch[i] = false;
	}


	while(index < WAR_REWARD_VILLAGE_NUMBER)		// 20위까지 산정. 
	{
		SI16 maxnum		= 0;
		SI16 maxunique	= 0;
		
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(pclVillageInfo[i] == NULL)continue;
			
			if(bVillageCheckSwitch[i] == false && pclVillageInfo[i]->siWarKillNumber > maxnum)
			{
				maxnum		= pclVillageInfo[i]->siWarKillNumber;
				maxunique	= i;
			}
		}
		
		if(maxunique)
		{
			psiVillageUnique[index]			= maxunique;
			psiVillageWarKillNumber[index]	= pclVillageInfo[maxunique]->siWarKillNumber;
			
			bVillageCheckSwitch[maxunique] = true;
		}
		index++;
	}
}

void cltVillageManager::WarOsakaWarpSwitch()
{
	SI32 i;
	
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i] == NULL)continue;
	
		// 마을 전공이 1000명보다 크면 오사카 갈 수 있다
		if( pclVillageInfo[i]->siWarKillNumber >= 1000)
			pclVillageInfo[i]->bOsakaWarSwitch = true;
	}
}

// 현재 마을에 있는 사용자를 구한다.
void cltVillageManager::GetCurrentVillageUserNum()
{

	SI32 i;

	//-----------------------------------
	// 현재의 마을 사용자수를 초기화한다. 
	//-----------------------------------
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i])
		{
			pclVillageInfo[i]->siUserNum = 0;
		}
	}
	
	//-----------------------------------
	// 마을 사용자수를 집계한다. 
	//-----------------------------------
	SI32 id;
	
	SI32 index = 0;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;
		
		if(IsPC(pclClient->pclCM->CR[id]->GetCharUnique()) == false)continue;

		SI32 villageunique = pclClient->pclCM->CR[id]->GetCurrentVillageUnique();
		if(villageunique <= 0)continue;
		if(villageunique >= MAX_VILLAGE_NUMBER)continue;
		if(pclVillageInfo[villageunique] == NULL)continue;

		pclVillageInfo[villageunique]->siUserNum++;
	}
}

void cltVillageManager::SetWarResult( SI32 siWinVillage, SI32 siLoseVillage )
{
/*
	int i=0;

	cltVillageVary *pclWinVillVary, *pclLoseVillVary;

	pclWinVillVary = &pclVillageInfo[ siWinVillage ]->clVillageVary;
	pclLoseVillVary = &pclVillageInfo[ siLoseVillage ]->clVillageVary;

	int parent = pclWinVillVary->siParentVillage;

	while( parent ) {

		if( parent == siLoseVillage ) {

			pclWinVillVary->siParentVillage = pclLoseVillVary->siParentVillage;
			pclWinVillVary->siMasterVillage = pclLoseVillVary->siMasterVillage;
		}

		parent = pclVillageInfo[ parent ]->clVillageVary.siParentVillage;

	}

	// 승리한 마을을 패배한 마을의 부모 마을로 설정한다
	pclLoseVillVary->siParentVillage = siWinVillage;

	// 패배한 마을의 최고부모마을을 설정한다
	if( pclWinVillVary->siMasterVillage == 0 ) {
		pclLoseVillVary->siMasterVillage = siWinVillage;
	} else {
		pclLoseVillVary->siMasterVillage = pclWinVillVary->siMasterVillage;
	}
*/
}

void cltVillageManager::ResetDeclareWar()
{
#ifdef _SAFE_MEMORY
	siDeclareWarVillage.ZeroMem();
	siNowWarVillage.ZeroMem();
	clDeclareWarDate.ZeroMem();
#else
	ZeroMemory( siDeclareWarVillage, sizeof( siDeclareWarVillage ) );
	ZeroMemory( siNowWarVillage, sizeof( siDeclareWarVillage ) );
	ZeroMemory( clDeclareWarDate, sizeof( clDeclareWarDate ) ); 
#endif
}

BOOL cltVillageManager::GetVillageRelation( SI32 village, SI32 slaveVillage )
{

	SI32 parent = pclVillageInfo[ slaveVillage ]->clVillageVary.siParentVillage;

	while( parent ) {
	
		if( village == parent ) return TRUE;

		parent = pclVillageInfo[ parent ]->clVillageVary.siParentVillage;
	}

	return FALSE;
}

// 유효한 마을 가운데 주식이 전혀 없는 마을을 찾아서 리턴한다. 
SI32 cltVillageManager::FindNoStockVillage()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageInfo[i] == NULL)continue;

		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( i);
		if(pclinfo == NULL)continue;
		if(pclinfo->clCityhallStrInfo.usCurStatus != CITYHALL_BEING_NORMAL ) continue ;

		// 주식 수가 전혀 없는 마을을 구한다. 
		if(pclinfo->clCityhallStrInfo.siTotalStockAmount == 0)return i;
	}

	return 0;

}

// 마을에 건물이 있는지 여부를 반환한다
BOOL cltVillageManager::IsThereStructure( SI32 villageunique, SI32 ranktype )
{
	if( pclVillageInfo[ villageunique ] == NULL ) return FALSE;

	return pclVillageInfo[ villageunique ]->clVillageVary.clStructureInfo.IsThereStructure( ranktype );
}

// 마을에 있는 건물의 위치를 반환한다.
bool cltVillageManager::GetPosFromStrUnique(SI32 villageUnique, SI32 strunique, OUT SI32* xpos, OUT SI32* ypos )
{
	if ( pclVillageInfo[ villageUnique ] == NULL ) return false;

	return pclVillageInfo[ villageUnique ]->clVillageVary.clStructureInfo.GetPosFromStrUnique( strunique, xpos, ypos );
}

// 마을 방문자수에 의한 레벨을 구한다. 
void cltVillageManager::CalcVisitLevel()
{

	if(pclClient->GetFrame() < 80000)return ;			// 서버 오픈 초기에는 마을 레벨을 계산하지 않는다. 
	bCheckVisitLevelSwitch	= true;						// 마을 레벨수가 유효하다. 

	SI32 i;
	SI32 mindata	= 0;
	SI32 village[MAX_VILLAGE_NUMBER];
	SI32 data[MAX_VILLAGE_NUMBER];
	
	ZeroMemory(village, sizeof(SI32) * MAX_VILLAGE_NUMBER);
	ZeroMemory(data,	sizeof(SI32) * MAX_VILLAGE_NUMBER);

	SI32 id			= 0;
	SI32 index		= 0;
	for(i = 0; i < MAX_VILLAGE_NUMBER; i++)
	{
		if(pclVillageInfo[i] == NULL)continue;

		SortAdd(village, data, &mindata, i, pclVillageInfo[i]->GetVisitNum(), MAX_VILLAGE_NUMBER);
	}

	SI16 villageunique	= 0;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		SI16 villageunique = village[i];

		if(pclVillageInfo[villageunique] == NULL)continue;

		pclVillageInfo[ villageunique ]->SetVisitLevel( (MAX_VILLAGE_NUMBER - i) / 10);
	}

}

// 마을 방문자수에 의한 레벨을 구한다. 
bool cltVillageManager::GetVisitLevel(SI32 villageunique, SI32* plevel)
{
	if(bCheckVisitLevelSwitch == false)return false;

	if(pclVillageInfo[villageunique] == NULL)return false;

	*plevel	= pclVillageInfo[villageunique]->GetVisitLevel();
	
	return true;

}

// 마을 안의 개인 상점 개수를 파악한다. 
SI32 cltVillageManager::GetPersonalShopNumber(SI32 villageunique)
{

	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return 0;

	if(pclVillageInfo[villageunique] == NULL)return 0;

	return pclVillageInfo[villageunique]->siPersonalShopNum;
}


// 파일에서 읽어서 지도에 마을을 심는다. 
BOOL cltVillageManager::LoadVillage(SI32 gamemode, SI32 servicearea)
{
/*
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoK.txt") );
		break;
	case ConstServiceArea_China :
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoC.txt") );
		break;
	case ConstServiceArea_Japan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoJ.txt") );
		break;
	case ConstServiceArea_English : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoE.txt") );
		break;
	case ConstServiceArea_Taiwan : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoT.txt") );
		break;
	case ConstServiceArea_USA  : 
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/VillageInfoU.txt") );
		break;

	}
*/
	TCHAR	datfilename[25];
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  // 중국어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("Data/VillageInfoG.txt"));
	}

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);							
	}
	
	if (!bLoadFile)
	{
		MsgBox(TEXT("Not LoadFromFile VillageInfo"),TEXT("Error"));
		return FALSE;
	}

	TCHAR province[64] = TEXT(""), townname[64] = TEXT("");
	SI32 vunique = 0, posx = 0, posy = 0, mapinx = 0, season = 0, farm = 0, iron = 0;
	bool bFarm, bIron;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&vunique,4,		
			NDATA_MBSTRING,	province,64,
			NDATA_MBSTRING,	townname,64,
			NDATA_INT32,	&posx, 4,
			NDATA_INT32,	&posy, 4,
			NDATA_INT32,	&mapinx, 4,
			NDATA_INT32,	&season, 4,
			NDATA_INT32,	&farm, 4,
			NDATA_INT32,	&iron,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() ) 
		{

			if ( province[ 0 ] != NULL)
			{
				bFarm	= DATA_TO_BOOL(farm) ;
				bIron	= DATA_TO_BOOL(iron) ;

				SI32 villimit;
				if(gamemode == GAMEMODE_MAPEDIT)
				{
					villimit = MAX_VILLAGE_NUMBER;
				}
				else
				{
					villimit = CUR_VILLAGE_LIMIT;
				}

				if(vunique <= villimit)
				{
					AddVillage(vunique, townname, posx,  posy, mapinx, season, bFarm, bIron );
				}

				
			}
		}
	}

	return TRUE;
}

BOOL cltVillageManager::IsVillageAtWar(SI32 villageunique)
{
	if( pclClient->bVillageWarSwitch == false )
		return FALSE;

	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)
		return FALSE;
    
	if( siNowWarVillage[ villageunique ]) 
		return TRUE;

	for( SI32 i = 0; i < MAX_VILLAGE_NUMBER; i++ )
	{
		if( siNowWarVillage[i] == villageunique )
			return TRUE;
	}


	return FALSE;	
}

SI32 cltVillageManager::GetVillageRank( SI32 villageunique )
{
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)
		return 0;
	
	SI32 siTotalVillageScore		= pclVillageInfo[villageunique]->clVillageVary.siTotalVillageScore;
	SI32 siCompareTotalVillageScore = 0;
	SI32 VillaeRank = 1;

	for( SI32 i = 0; i < MAX_VILLAGE_NUMBER; i++)
	{
		if(  i == villageunique ) continue;
		if( pclVillageInfo[i] == NULL) continue;

		siCompareTotalVillageScore = pclVillageInfo[i]->clVillageVary.siTotalVillageScore;

		if( siTotalVillageScore < siCompareTotalVillageScore )
		{
			VillaeRank++;
		}

	}


	return VillaeRank;

}

BOOL cltVillageManager::AdjustAllStrDurInVillage( SI32 villageunique, SI32 adjustDur,SI32 lowest = 0)
{
	 
	cltStrInfo* pclStrinfo = NULL;

	// 은행이 있다면
					
	pclStrinfo = 	pclBankManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}
		
		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_BANK, TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}


	// 여각이 있다면
	pclStrinfo = 	pclStockManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}

		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_STOCK, TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}


	// 마켓이 있다면
	pclStrinfo = 	pclMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_MARKET, TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//수렵장이  있다면
	pclStrinfo = 	pclHuntManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_HUNT,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//시전 있다면
	pclStrinfo = 	pclHouseManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_HOUSE,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//관청 있다면
	pclStrinfo = 	pclCityHallManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_CITYHALL,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//우체국 있다면
	pclStrinfo = 	pclPostOfficeManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_POSTOFFICE,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//사복시 있다면
	pclStrinfo = 	pclHorseMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_HORSEMARKET,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//여각있다면
	pclStrinfo = 	pclRealEstateMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_REALESTATEMARKET,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	//소환수거래소 있다면
	pclStrinfo = 	pclSummonMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}

		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_SUMMONMARKET,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = 	pclLandManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_LAND,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = 	pclFeastManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_FEAST,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = 	pclSummonHeroMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_SUMMONHEROMARKET,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = 	pclMineManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{		
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_MINE,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = 	pclGuildManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{		
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_GUILD,TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

	// 있다면
	pclStrinfo = pclNewMarketManager->GetStrInfo(villageunique);

	if( pclStrinfo != NULL )
	{		
		SI32 TempDur = adjustDur;

		if( pclStrinfo->clCommonInfo.siStrDura + TempDur < lowest )
		{
			TempDur = lowest - pclStrinfo->clCommonInfo.siStrDura;
		}


		sDBRequest_ChangeStrDura sendMsg( villageunique, RANKTYPE_NEWMARKET, TempDur );
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}


	return TRUE;
}

void cltVillageManager::FindLowTotalVillageScore( SI32* VillageUnique )
{
	SI32 totalVillageScore[MAX_VILLAGE_NUMBER] = { 0, };
	SI32 villageUnique[MAX_VILLAGE_NUMBER] = { 0, };

	SI32 i;
	SI32 index = 0;

	SI32 tempbuf[ MAX_VILLAGE_NUMBER ];

	for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		tempbuf[ i ] = i;

		if( pclVillageInfo[i] == NULL )
			continue;
		
		totalVillageScore[i] = pclVillageInfo[i]->clVillageVary.siTotalVillageScore;
	}

	for( SI32 vill = 0; vill < MAX_VILLAGE_NUMBER - 1; vill++ )
	{
		if( pclVillageInfo[vill] == NULL )
			continue;

		for(i = vill + 1; i < MAX_VILLAGE_NUMBER; i++)
		{
			if( pclVillageInfo[i] == NULL )
				continue;

			int cmpindex1 = tempbuf[ vill ];
			int cmpindex2 = tempbuf[ i ];

			if(totalVillageScore[cmpindex1] > totalVillageScore[cmpindex2])
			{
				int temp = tempbuf[ vill ];
				tempbuf[ vill ] = tempbuf[ i ];
				tempbuf[ i ] = temp;
			}
		}
	}

	for(i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		if( pclVillageInfo[ tempbuf[i] ]== NULL)
			continue;
		villageUnique[ index ] = tempbuf[ i ];
		index++;
	}

	for( i = 0; i < 5; ++i )
	{
		VillageUnique[i] = villageUnique[i];
	}
}
