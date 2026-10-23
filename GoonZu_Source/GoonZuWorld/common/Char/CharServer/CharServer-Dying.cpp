//---------------------------------
// 2004/10/30 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharServer\Char-Server.h"
#include "..\CharManager\CharManager.h"
#include "Order/order.h"

//--------------------------------------
// CommonLogic
//--------------------------------------
#include "CityHall\CityHall.h"
#include "Msg\MsgType-Summon.h"
#include "MsgType-Person.h"


//--------------------------------------
// Server
//--------------------------------------
#include "..\..\Server\Server.h"


//--------------------------------------
// DBManager
//--------------------------------------
#include "..\..\DBManager\GameDBManager_World\DBMsg-Summon.h"

extern cltCommonLogic* pclClient;

// 일정시간 죽도록 한다. 
bool cltCharServer::Action_Dying()
{
	cltCharCommon::Action_Dying();

	// 대만에서는 캐릭터 부활대기시간을 약5초로 한다. (PCK - 2007.09.11) - 정확하게 5초로는 안됨.
	// 50(상수) * (6(Frame) * 50(ms)) = 15000ms (15초)
	// 앞의 상수값을 변화시켜서 캐릭터 부활대기시간을 조절할수 있다.
	SI32	siDyingDelay = 50;
	if (pclClient->IsCountrySwitch(Switch_DyingDelay))
		siDyingDelay = 17;
	else
		siDyingDelay = 50;

	if(siCharDyingDelay >= siDyingDelay)
	{
		siCharDyingDelay = 0;

		return TRUE;
	}
	else
	{
		// 한번만 실행되게 한다. 
		if(siCharDyingDelay == 0)
		{
			// 몬스터가 해당 캐릭터의 소환수라면 
			if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				
				// 소환수인 경우 경험치 페널티를 준다. 
				// 전쟁중 사망한 경우에는 페널티가 없다. 
				if(IsDeathPenaltyCondition())
				{
					SI32 summonlevel = pclCI->clIP.GetLevel();
					SI32 decexp = summonlevel * 10;
					
					pclCI->clIP.DecreaseExp(decexp);
					SetUpdateSwitch(UPDATE_IP, true, 0);
				}
			}
		}
		
		//if(GetKind () == pclClient->GetUniqueFromHash(TEXT("KIND_TOYO1")))
		//{
		//	return TRUE;
		//}

		siCharDyingDelay++;
		
	}
	


	return FALSE;

}

// 사망 처리. 
void cltCharServer::Dead()
{
	//if(GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_TOYO1")))
	//{
	//	cltServer* pclserver = (cltServer*)pclClient;

	//	cltInnerStatusInfo clinnerstatus;
	//	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

	//	SI32 id = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_TOYO2")), pclCI->clBI.siHomeVillage, NULL, 1, 0, GetX(), GetY(), 0, NPCITEMMODE_NORMAL, &clinnerstatus );
	//}

	// 해당 몬스터가 성문이라면 
	if(clIdentity.siIdentity == IDENTITY_CASTLE)
	{
		if( pclCI->clBI.siHomeVillage >= 0 && pclCI->clBI.siHomeVillage < MAX_VILLAGE_NUMBER )
		{
			pclClient->pclVillageManager->pclVillageInfo[pclCI->clBI.siHomeVillage]->siCastleCharUnique = 0;
			pclClient->pclVillageManager->pclVillageInfo[pclCI->clBI.siHomeVillage]->dwLastCastleClock = pclClient->CurrentClock; 
			//SI32 siKind = pclClient->pclKindInfoSet->GetDefaultCastle();
			SI32 siKind = pclClient->pclKindInfoSet->GetDefaultCastle();

			SI32 siCapa = ((cltCityHall *)(pclClient->pclCityHallManager->pclStruct[pclCI->clBI.siHomeVillage]))->clStrInfo.clCityhallStrInfo.siCastleCapa - 5 ;
			if( siCapa  < 100) siCapa  = 100;

			((cltCityHall *)(pclClient->pclCityHallManager->pclStruct[pclCI->clBI.siHomeVillage]))->SetCastle(siKind, 0, 0, siCapa);
		}
	}
	
	// 몬스터가 해당 마을의 성황신이라면 1분후에 다시 살리기위해서 초기화를 한다.
	else if(clIdentity.siIdentity == IDENTITY_VILLAGEGOD)
	{
		if( pclCI->clBI.siHomeVillage >= 0 && pclCI->clBI.siHomeVillage < MAX_VILLAGE_NUMBER )
		{
			pclClient->pclVillageManager->pclVillageInfo[pclCI->clBI.siHomeVillage]->siVillageGodCharUnique = 0;
			pclClient->pclVillageManager->pclVillageInfo[pclCI->clBI.siHomeVillage]->dwLastVillageGodClock = pclClient->CurrentClock; 
			
			// 죽으면 Default 성황신으로 초기화시킨다.
			//SI32 siKind = pclClient->pclKindInfoSet->GetDefaultVillageGod();
			SI32 siKind = ((cltCityHall *)(pclClient->pclCityHallManager->pclStruct[pclCI->clBI.siHomeVillage]))->clStrInfo.clCityhallStrInfo.siSymbolKind;
			SI32 siCapa = ((cltCityHall *)(pclClient->pclCityHallManager->pclStruct[pclCI->clBI.siHomeVillage]))->clStrInfo.clCityhallStrInfo.siSymbolCapa - 5 ;

			if( siCapa  < 100) siCapa  = 100;

			((cltCityHall *)(pclClient->pclCityHallManager->pclStruct[pclCI->clBI.siHomeVillage]))->SetSymbol(siKind, 0, 0, siCapa );
		}
	}
	
	// 몬스터가 해당 캐릭터의 소환수라면 
	if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
		SI32 siParentID = GetSummonParentID();
		if(pclCM->IsValidID(siParentID)==TRUE)
		{
			cltCharServer* pclchar = pclCM->GetCharServer( siParentID );
			if( NULL == pclchar ) return;

			cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummon( GetSummonIndex() );
			//==========================================
			// 주인이 갖고 있는 소환수의 정보를 설정한다.
			//==========================================
			if(pclSummon)
			{
				if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
				{
					// DB로 업데이트 시켜준다. 이때 변신이 해지 된다.
					pclchar->SendDBSummon( SETSUMMON_REASON_UPDATE, GetSummonIndex() );
				}
				
				pclSummon->siStatus = SUMMON_STATUS_DEAD;
				//pclSummon->siShortenLife += 1;		// 수명을 1일씩 줄인다.
				pclSummon->clIP.Set(&pclCI->clIP);
				pclSummon->clIP.SetLife(0);			// 혹시 몰라서 
				pclSummon->clIP.SetMana(0);
				cltGameMsgResponse_Summon clInfo(pclchar->GetCharUnique(), GetSummonIndex(), pclSummon);
				cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			//==========================================
			// 소환수 정보를 DB에 저장한다.
			//==========================================
			{
				pclchar->SendDBSummon(SETSUMMON_REASON_UPDATE, GetSummonIndex());
			}
			//==========================================
			// 주인한테도 주인의 정보를 갱신시킨다.
			//==========================================
			{
				pclchar->siChildCharUnique = 0;
				
				cltGameMsgResponse_ChangeSummonStatus clInfo(pclchar->GetCharUnique(), pclchar->siChildCharUnique, pclchar->siSummonIndex);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}
// 자폭처리. 
void cltCharServer::SelfBombAction()
{
	cltGameMsgResponse_IncLife clInfo(GetCharUnique(), 0, 0, 0, 0, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
	SendNeighbourMsg(&clMsg, true);
	
	SetLife(0);
}