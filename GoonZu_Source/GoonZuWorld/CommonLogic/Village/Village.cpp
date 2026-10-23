//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "..\Client\Client.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg.h"

//------------------------------
// CommonLogic
//------------------------------
#include "Village.h"
#include "MsgType-System.h"
#include "Cityhall\Cityhall.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharServer\Char-Server.h"
#include "Char\CharManager\CharManager.h"

//---------------------------------
// Server
//---------------------------------
#include "..\Server\Server.h"
#include "..\..\Server\Rank\Rank.h"
#include "../../Server/AuthServer/AuthMsg.h"

extern cltCommonLogic* pclClient;

LPDIRECTDRAWSURFACE7		g_lpVillageTitleSurface = NULL;

//--------------------------------------------------
// cltVillageInfo
//--------------------------------------------------
cltVillageInfo::cltVillageInfo(cltVillageManager* pclvillageset, SI32 villagemode, SI32 siunique, DWORD atb, TCHAR* name, SI32 villageposx, SI32 villageposy, SI32 mapindex, SI32 season, bool bfarm, bool biron )
{
	if(pclvillageset == NULL)
	{
		MsgBox(TEXT("cltVillageInfo()"), TEXT("fd0982"));
	}
	else
	{
		pclVillageManager	= pclvillageset;
	}

	siVillageUnique	= siunique;	
	dwAtb		= atb;
	_tcsncpy(szName, name, 30);

	pTitleBuffer	= NULL;
	pTitleEncBuffer = NULL;

	bEnemyCaptureSwitch	= false;
	siEnemyNum			= 0;
	siPersonalShopNum	= 0;

	PosX			= villageposx;
	PosY			= villageposy;

	siWarKillNumber	= 0;
	siUserNum		= 0;

	dwLastVillageGodClock	= 0;
	dwLastCastleClock		= 0;
	dwLastAttackedVillageClock = 0;

	siVillageGodCharUnique	= 0;
	siCastleCharUnique		= 0;
	siKingMonumentUnique	= 0;
	bAttackedVillage		= FALSE;
	bOsakaWarSwitch			= false;
	
	siVisitorNumber			= 0;		// 방문객 수 초기화. 
	siVisitLevel			= 0;
	
	
	bFarmSwitch				= bfarm;
	bIronSwitch				= biron;
	bUpgradeSwitch			= false ;
	
	if(season == 0)		siSeason	= SEASON_SPRING;
	else if(season == 1)siSeason	= SEASON_AUTUMN;
	else if(season == 2)siSeason	= SEASON_WINTER;

	switch(season)
	{
	case SEASON_SPRING:
		siVillageType = VILLAGETYPE_SPRING_NORMAL;
		break;
	case SEASON_AUTUMN:
		siVillageType = VILLAGETYPE_AUTUMN_NORMAL;
		break;
	case SEASON_WINTER:
		siVillageType = VILLAGETYPE_WINTER_NORMAL;
		break;
	}
	
	memset(RankAccountID, 0, sizeof(RankAccountID));

	if(villagemode == GAMEMODE_SERVER)
	{
		LoadCityHallFromDB();
		
		LoadBankFromDB();
		
		LoadStockFromDB();
		
		LoadMarketFromDB();
		
		LoadHouseFromDB();
		
		// DB에서 수렵장 정보를 읽어와서 업데이트 한다. 
		LoadHuntFromDB();
		
		// DB에서 역참 정보를 읽어와서 업데이트 한다.
		LoadPostOfficeFromDB();

		LoadHorseMarketFromDB();

		LoadRealEstateMarketFromDB();

		LoadSummonMarketFromDB();

		LoadLandFromDB();

		LoadFeastFromDB();

		LoadSummonHeroMarketFromDB();

		LoadMineFromDB();

		LoadGuildFromDB();

		LoadNewMarketFromDB();
	}
}

cltVillageInfo::~cltVillageInfo()
{
	if( g_lpVillageTitleSurface ) {
		g_lpVillageTitleSurface->Release();
		g_lpVillageTitleSurface = NULL;
	}

	if(pTitleBuffer)
	{
		delete[] pTitleBuffer;
		pTitleBuffer = NULL;
	}

	if(pTitleEncBuffer)
	{
		delete[] pTitleEncBuffer;
		pTitleEncBuffer = NULL;
	}
}


BOOL  cltVillageInfo::LoadTitle()
{

	if( g_lpVillageTitleSurface == NULL ) {



		if( g_graphics.CreateSurface16( &g_lpVillageTitleSurface, 300, 64 ) == FALSE ) {
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox( NULL, TEXT("CreateOffScreen Error, VillageTitleSurface"), TEXT("fdsf"), MB_OK |MB_TOPMOST);
		}


	}


	if(pTitleBuffer)return FALSE;

	cltClient* pclclient = (cltClient*)pclClient;

	if(pclclient->lpBackScreen)
	{
		SIZE size={0, 0};
		HDC hdc;
		
		//  마을 이름을 서피스에 쓴다. 
		if(pclclient->lpBackScreen->GetDC(&hdc)== DD_OK)
		{
//			TextOut(hdc, 0, 0, szName, lstrlen(szName));
			
			// 마을 이름 이미지의 사이즈를 얻어온다. 
			GetTextExtentPoint32(hdc, szName, _tcslen(szName), &size);
			
			pclclient->lpBackScreen->ReleaseDC(hdc);
		}
		
		if(size.cx && size.cy )
		{
			// 마을 이미지를 버퍼에 넣는다. 
			TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_VILLAGENAMEBACK);
			
			if(pspr && pspr->GetXSize())
			{

				SI32 xnum = size.cx / pspr->GetXSize();
				TitleImageXsize	= xnum * pspr->GetXSize();
				
				if(size.cx % pspr->GetXSize())
				{
					TitleImageXsize += pspr->GetXSize();
					xnum ++;
				}
				TitleImageXsize += (pspr->GetXSize() * 2) ;

				TitleImageYsize	= pspr->GetYSize();


				//pTitleBuffer = new UI08[ TitleImageXsize * TitleImageYsize * 4 ];
				pTitleBuffer = new UI08[ sizeof(RGB16) * 300 * 64 ];

				g_graphics.ClearSurface( g_lpVillageTitleSurface, RGB16_TRANS );
				
				if( GP.LockSurface( g_lpVillageTitleSurface ) ) {


					GP.PutSpr(pspr,0, 0, 0);
						
					for(SI32 i = 0;i < xnum;i++)
					{
						GP.PutSpr(pspr,pspr->GetXSize() + i * pspr->GetXSize(), 0, 1);
					}

					GP.PutSpr(pspr,pspr->GetXSize() + xnum * pspr->GetXSize(), 0, 2);

					GP.UnlockSurface( g_lpVillageTitleSurface );
				}
					
				// 마을 이름을 출력한다. 
				if( g_lpVillageTitleSurface->GetDC(&hdc)== DD_OK)
				{
					TextOut(
						hdc, 
						pspr->GetXSize(), 
						(TitleImageYsize - size.cy) / 2, 
						szName, lstrlen(szName) );

					g_lpVillageTitleSurface->ReleaseDC( hdc );
				}

			
				if(  GP.LockSurface( g_lpVillageTitleSurface ) )
				{
					if(pTitleBuffer)
					{
						int size;
						g_graphics.EncodeXpr( (RGB16*)pTitleBuffer, 
											  300,
											  64,
											  RGB16_TRANS,
											  &size );
					}


				GP.UnlockSurface( g_lpVillageTitleSurface );
				}

			}

/*			
			// 마을 이미지를 버퍼에 얻어온다.
			UI08* ptemp;
			ptemp = new UI08[size.cx * size.cy];
			
			if(GP.LockSurface(pclclient->lpBackScreen))
			{
				GP.GetImage(0, 0, size.cx, size.cy, ptemp);
				
				GP.UnlockSurface(pclclient->lpBackScreen);
			}
			
			// 마을 이미지를 버퍼에 넣는다. 
			cltTSpr* pspr	= pclClient->GetGImgPointer(GIMG_VILLAGENAMEBACK);
			if(pspr)
			{
				SI32 xnum = size.cx / pspr->GetXSize();
				TitleImageXsize	= xnum * pspr->GetXSize();
				
				if(size.cx % pspr->GetXSize())
				{
					TitleImageXsize += pspr->GetXSize();
					xnum ++;
				}
				TitleImageXsize += (pspr->GetXSize() * 2) ;

				TitleImageYsize	= pspr->GetYSize();

#ifdef GRA_MODE_16

#else

				pTitleBuffer = new UI08[TitleImageXsize * TitleImageYsize];



				if(GP.LockSurface(pTitleBuffer, TitleImageXsize, TitleImageYsize) )
				{
					// 투명색으로 채운다. 
					GP.FillBox(0, 0, TitleImageXsize-1, TitleImageYsize-1, COLOR_TRANS,FALSE);

					GP.PutSpr(pspr,0, 0, 0);
					
					for(SI32 i = 0;i < xnum;i++)
					{
						GP.PutSpr(pspr,pspr->GetXSize() + i * pspr->GetXSize(), 0, 1);
					}

					GP.PutSpr(pspr,pspr->GetXSize() + xnum * pspr->GetXSize(), 0, 2);


					// 마을 이름을 출력한다. 
					//GP.PutSpr8T(pspr->GetXSize(), (TitleImageYsize - size.cy) / 2, size.cx, size.cy, ptemp);
					GP.PutSpr(pspr->GetXSize(), (TitleImageYsize - size.cy) / 2, size.cx, size.cy,
							  ptemp,PUT_SPR_SPR,SPR_MODE_T);
				}
			}
			
			delete[] ptemp;
*/
		}

		
	}
	
	return TRUE;
}

// 특정 시설이 마을에 있는지 여부를 확인한다. 
BOOL cltVillageInfo::HasStructure(SI32 structureunique)
{
	return clVillageVary.clStructureInfo.IsDuplicate(structureunique);
}

BOOL cltVillageInfo::Action()
{
	SI32 diff = 1;

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{

		//--------------------------------------
		// 점령 여부 파악 
		//--------------------------------------
		if(bEnemyCaptureSwitch == false && siEnemyNum >= 10)
		{
			bEnemyCaptureSwitch = true;

			//-----------------------------------
			// 클라이언트에 통보. 
			//-----------------------------------
			cltGameMsgResponse_EnemyCapture clinfo(siVillageUnique, bEnemyCaptureSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_ENEMYCAPTURE, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}

		if(bEnemyCaptureSwitch == true && siEnemyNum == 0)
		{
			bEnemyCaptureSwitch = false;

			//-----------------------------------
			// 클라이언트에 통보. 
			//-----------------------------------
			cltGameMsgResponse_EnemyCapture clinfo(siVillageUnique, bEnemyCaptureSwitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_ENEMYCAPTURE, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

		}
		

	//-------------------------------------------------------
	// 투자자 정보를 DB에 요청한다. 
	//-------------------------------------------------------
		SI32 diff = 100;
		if(pclClient->GetFrame() % diff == 0)
		{
			if( (pclClient->GetFrame() / diff) % MAX_VILLAGE_NUMBER  == (DWORD)siVillageUnique)
			{
				sDBRequest_InvestList clMsgInvest(siVillageUnique);
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsgInvest);
			}
		}


		



	if( TABS(pclClient->CurrentClock - dwLastVillageGodClock) > 60000)
		{
			dwLastVillageGodClock = pclClient->CurrentClock;

			//-------------------------------------------------------
			// 수호상 1분 마다 검사 
			//-------------------------------------------------------

			CreateVillageKingMonument();

			//-------------------------------------------------------
			// 성황신 1분 마다 검사 
			//-------------------------------------------------------
			// 성황신이 존재하지 않을경우 새로 생성한다.
			CreateVillageGod();

			// 성황신이 현재 존재할경우  알정주기마다 저장 
			SetVillageGodtoDB();

			//-------------------------------------------------------
			// 성벽 1분 마다 검사 
			//-------------------------------------------------------
			// 성벽이 존재하지 않을경우 새로 생성한다.
			CreateCastle();

			// 성벽이 현재 존재할경우  알정주기마다 저장 
			SetCastletoDB();
		}


	//-------------------------------------------------------
	// 성벽이나 성황신이 공격을 당하는지 1분 마다 검사 
	//-------------------------------------------------------
		if( TABS(pclClient->CurrentClock - dwLastAttackedVillageClock) > 60000)
		{
			dwLastAttackedVillageClock = pclClient->CurrentClock;
			if(bAttackedVillage == TRUE)
			{
				// 전체 유저에게 알림 메시지를 보낸다. 
				TCHAR buffer[256];
				TCHAR* pText = GetTxtFromMgr(1436);
				StringCchPrintf(buffer, 256, pText, szName );
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, buffer);
				bAttackedVillage = FALSE;
			}
		}

	}
	else
	{
		if(pTitleBuffer == NULL)
		{
			LoadTitle();
		}
	}
	
	return TRUE;
}

BOOL cltVillageInfo::CreateVillageKingMonument()
{
	//마을이 전쟁중일때 수호상이 생성되어 있지 않는다면 생성한다.

	if( pclClient->bVillageWarSwitch == true &&  pclVillageManager->IsVillageAtWar(siVillageUnique) == TRUE
		&& siKingMonumentUnique <= 0 )
	{


		cltIdentity clidentity;
		clidentity.MakeKingMonument(IDENTITY_VILLAGE_KINGMONUMENT, siVillageUnique);

		cltServer* pclserver = (cltServer*)pclClient;

		SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[siVillageUnique]->siMapUnique;

		//cyj 성황신 마을중앙에 생성되도록 수정
		SI32 centerx = 32;
		SI32 centery = 50;
		
		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_VILLAGE, 0);

		siKingMonumentUnique = pclserver->SetNPCChar(
			pclClient->GetUniqueFromHash(TEXT("KIND_KINGMONUMENT")) , siVillageUnique, &clidentity, 1, 0, 
			centerx, centery, siVillageUnique, NPCITEMMODE_NONE, &clinnerstatus );
		
		return TRUE;
	}

	//전쟁중이 아닐때 수호상이 존재한다면 삭제한다.
	if( pclClient->bVillageWarSwitch == false || pclVillageManager->IsVillageAtWar(siVillageUnique) == false)
	{
		if(siKingMonumentUnique > 0 )
		{
			if(pclClient->pclCM->IsValidID(siKingMonumentUnique))
			{
				pclClient->pclCM->CR[siKingMonumentUnique]->DeleteInCell();
				pclClient->pclCM->DeleteChar(siKingMonumentUnique);
			}

			siKingMonumentUnique = 0;
		}
	}

	return FALSE;
	
}

// 현재 성황신의 상태를 DB에 저장하도록 한다.
BOOL cltVillageInfo::SetVillageGodtoDB()
{
	cltStrInfo* pclinfo = pclVillageManager->pclCityHallManager->GetStrInfo(siVillageUnique);
	if(pclinfo == NULL)return FALSE;

	// 성황신이 현재 존재할경우  알정주기마다 저장 
	if(pclinfo->clCityhallStrInfo.siSymbolKind > 0  && siVillageGodCharUnique >0)
	{
		cltServer* pclserver = (cltServer*)pclClient;
		// 적절한 성황신인지 검사하고 
		if(pclserver->pclCM->IsValidID(siVillageGodCharUnique) && pclserver->pclCM->CR[ siVillageGodCharUnique ]->pclCI->clBI.siHomeVillage == siVillageUnique)
		{
			SI32 kind = pclserver->pclCM->CR[ siVillageGodCharUnique ]->GetKind();
			SI32 life = pclserver->pclCM->CR[ siVillageGodCharUnique ]->GetLife();
			SI32 mana = pclserver->pclCM->CR[ siVillageGodCharUnique ]->GetMana();
			SI32 capa = pclserver->pclCM->CR[ siVillageGodCharUnique ]->clIdentity.clGodInfo.siCapa;
	
			((cltCityHall *)(pclVillageManager->pclCityHallManager->pclStruct[siVillageUnique]))->SetSymbol(kind, life, mana, capa);
			return TRUE;
		}
	}

	return FALSE;
}


// 현재 성황신의 상태를 DB에 저장하도록 한다.
BOOL cltVillageInfo::SetCastletoDB()
{
	cltStrInfo* pclinfo = pclVillageManager->pclCityHallManager->GetStrInfo(siVillageUnique);
	if(pclinfo == NULL)return FALSE;

	// 성황신이 현재 존재할경우  알정주기마다 저장 
	if(pclinfo->clCityhallStrInfo.siCastleKind > 0  && siCastleCharUnique >0)
	{
		cltServer* pclserver = (cltServer*)pclClient;
		// 적절한 성황신인지 검사하고 
		if(pclserver->pclCM->IsValidID(siCastleCharUnique) && pclserver->pclCM->CR[ siCastleCharUnique ]->pclCI->clBI.siHomeVillage == siVillageUnique)
		{
			SI32 kind = pclserver->pclCM->CR[ siCastleCharUnique ]->GetKind();
			SI32 life = pclserver->pclCM->CR[ siCastleCharUnique ]->GetLife();
			SI32 mana = pclserver->pclCM->CR[ siCastleCharUnique ]->GetMana();
			SI32 capa = pclserver->pclCM->CR[ siCastleCharUnique ]->clIdentity.clGodInfo.siCapa;
			
			((cltCityHall *)(pclVillageManager->pclCityHallManager->pclStruct[siVillageUnique]))->SetCastle(kind, life, mana, capa);
			return TRUE;
		}
	}

	return FALSE;
}

// 성황신을 새로 성성한다.
BOOL cltVillageInfo::CreateVillageGod()
{
	cltStrInfo* pclinfo = pclVillageManager->pclCityHallManager->GetStrInfo(siVillageUnique);
	if(pclinfo==NULL)return FALSE;
	
	// DB에는 존재하지만 현재 생성되어 있지 않다면 새로 만든다.
	if(pclinfo->clCityhallStrInfo.siSymbolKind > 0  && siVillageGodCharUnique <=0)
	{
		SI16 siGodKind = pclinfo->clCityhallStrInfo.siSymbolKind;
		SI32 siGodLife = pclinfo->clCityhallStrInfo.siSymbolLife;
		SI32 siGodMana = pclinfo->clCityhallStrInfo.siSymbolMana;
		SI16 siGodCapa = pclinfo->clCityhallStrInfo.siSymbolCapa;

		//if(siGodKind<=0)	siGodKind = pclClient->pclKindInfoSet->GetDefaultVillageGod();
		
		// 성황신이 죽어있는경우 전쟁중이라면 새로만들지 않는다.
		if(siGodLife<= 0 &&  pclClient->bVillageWarSwitch == true)	
		{
			return FALSE;
		}
		

		cltIdentity clidentity;
		clidentity.MakeGod(IDENTITY_VILLAGEGOD, siGodCapa);

		cltServer* pclserver = (cltServer*)pclClient;

		SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[siVillageUnique]->siMapUnique;

		//cyj 성황신 마을중앙에 생성되도록 수정
		SI32 centerx = 30;
		SI32 centery = 30;
		//SI32 centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
		//SI32 centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_VILLAGE, 0);
		
		siVillageGodCharUnique = pclserver->SetNPCChar(
			siGodKind, siVillageUnique, &clidentity, 1, 0, 
			centerx, centery, siVillageUnique, NPCITEMMODE_NONE, &clinnerstatus );
		
		
		if(siVillageGodCharUnique)
		{
		
			// 죽은후에 부활할경우 체력을 회복시켜준다.
			if(siGodLife<= 0)	
			{
				pclserver->pclCM->CR[ siVillageGodCharUnique ]->ParametaBoxAction();
				
				siGodLife = pclserver->pclCM->CR[ siVillageGodCharUnique ]->clPB.GetMaxLife();
				siGodMana = pclserver->pclCM->CR[ siVillageGodCharUnique ]->clPB.GetMaxMana();
				
				siGodCapa -= 4;

				if( siGodCapa < 100 )
					siGodCapa = 100;

				((cltCityHall *)(pclVillageManager->pclCityHallManager->pclStruct[siVillageUnique]))->SetSymbol(siGodKind, siGodLife, siGodMana, siGodCapa);
			}

			
			// 체력과 마법력을 설정해준다. 
			pclserver->pclCM->CR[ siVillageGodCharUnique ]->SetLife(siGodLife);
			pclserver->pclCM->CR[ siVillageGodCharUnique ]->SetMana(siGodMana);
			pclserver->pclCM->CR[ siVillageGodCharUnique ]->clIdentity.clGodInfo.siCapa = siGodCapa;
		}
		return TRUE;
	}
	return FALSE;

}


// 성벽을 새로 성성한다.
BOOL cltVillageInfo::CreateCastle()
{
	cltStrInfo* pclinfo = pclVillageManager->pclCityHallManager->GetStrInfo(siVillageUnique);
	if(pclinfo == NULL)return FALSE;
	
	// DB에는 존재하지만 현재 생성되어 있지 않다면 새로 만든다.
	if(pclinfo->clCityhallStrInfo.siSymbolKind > 0  && siCastleCharUnique <=0)
	{
		SI16 siCastleKind = pclinfo->clCityhallStrInfo.siCastleKind;
		SI32 siCastleLife = pclinfo->clCityhallStrInfo.siCastleLife;
		SI32 siCastleMana = pclinfo->clCityhallStrInfo.siCastleMana;		
		SI16 siCastleCapa = pclinfo->clCityhallStrInfo.siCastleCapa;		

		//if(siCastleKind<=0)	siCastleKind = pclClient->pclKindInfoSet->GetDefaultCastle();

		// 성벽이 죽어있는경우 전쟁중이라면 새로만들지 않는다.
		if(siCastleLife<= 0 &&  pclClient->bVillageWarSwitch == true)	
		{
			return FALSE;
		}
		
	
		// 야외지역 마을입구에서 만든다.
		cltIdentity clidentity;
		clidentity.MakeGod(IDENTITY_CASTLE, siCastleCapa);

		cltServer* pclserver = (cltServer*)pclClient;

		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_VILLAGE, 0);

		//cyj 성벽 위치 조정
		//siCastleCharUnique = pclserver->SetNPCChar(siCastleKind, siVillageUnique, &clidentity, 1, 0, PosX-1, PosY+2, 0, NPCITEMMODE_NONE, &clinnerstatus );
		siCastleCharUnique = pclserver->SetNPCChar(siCastleKind, siVillageUnique, &clidentity, 1, 0, PosX-1, PosY+3, 0, NPCITEMMODE_NONE, &clinnerstatus );
		
		
		if(siCastleCharUnique)
		{
			// 죽은후에 부활할경우 체력을 회복시켜준다.
			if(siCastleLife<= 0)	
			{
				pclserver->pclCM->CR[ siCastleCharUnique ]->ParametaBoxAction();

				siCastleLife = pclserver->pclCM->CR[ siCastleCharUnique ]->clPB.GetMaxLife()/10;
				siCastleMana = pclserver->pclCM->CR[ siCastleCharUnique ]->clPB.GetMaxMana()/10;
				
				siCastleCapa = ((cltCityHall *)(pclVillageManager->pclCityHallManager->pclStruct[siVillageUnique]))->clStrInfo.clCityhallStrInfo.siCastleCapa;

				((cltCityHall *)(pclVillageManager->pclCityHallManager->pclStruct[siVillageUnique]))->SetCastle(siCastleKind, siCastleLife, siCastleMana, siCastleCapa);

			}
			
			// 체력과 마법력을 설정해준다. 
			pclserver->pclCM->CR[ siCastleCharUnique ]->SetLife(siCastleLife);
			pclserver->pclCM->CR[ siCastleCharUnique ]->SetMana(siCastleMana);
			pclserver->pclCM->CR[ siCastleCharUnique ]->clIdentity.clGodInfo.siCapa		= siCastleCapa;
		}
		return TRUE;
	}
	return FALSE;

}

// PersonID가 최대 주주인지 확인한다. 
BOOL cltVillageInfo::IsMostBeholder(SI32 personid)
{
	// 최대 주주와 같은 PersonID를 사용하는지 확인한다. 
	if(clVillageVary.clInvestInfo.clInvestInfo[0].clPerson.GetPersonID() == personid)
	{
		// 최대주주는 같은데 보유주식 수량이 다르면 정보를 업데이트 해야 한다. 

		return TRUE;
	}

	return FALSE;
}

void  cltVillageInfo::Draw(SI32 dotx, SI32 doty)
{
	if( pTitleBuffer == NULL ) return;

	GP.PutXpr16( dotx - TitleImageXsize / 2, 
				 doty - 20, 
		         300, 
				 64,
				 (RGB16*)pTitleBuffer, 
				 RGB16_TRANS );

}

