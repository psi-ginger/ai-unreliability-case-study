#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Agriculture.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-Farming.h"
#include "Msg\MsgType-Agriculture.h"

#include "../Common/Farming/FarmingMgr.h"
#include "../Common/Order/order.h"
#include "../CommonLogic/Land/Land.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_FARMINGSTART(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )					return;
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )										return;

	cltGameMsgRequest_FarmingStart * pclinfo = (cltGameMsgRequest_FarmingStart*)pclMsg->cData;
	if ( pclinfo->siItemUnique < 1 )						return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	cltAgriculture* pclAgriculture = pclChar->m_pclAgriculture;
	if( pclAgriculture == NULL )								return;

	bool Success = false;

	SI32 NearVillageUnique = 0;
	SI32 returnval = 0;

	SI32 XPos = pclCM->CR[id]->GetX();
	SI32 YPos = pclCM->CR[id]->GetY();
	SI32 TotalSkillLevel = pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[0] );
	bool bCanFarm = true;

	// 스킬제한없는 농경 퀘스트 중이라면 가능.
	SI16 siQuestType, siQuestUnique, siPara1, siPara2;
	SI16 siQuestClearCond	= QUEST_CLEAR_COND_FARMNOSKILL;
	bool bFarmNoSkillGoing	= pclChar->pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2);

	if ( TotalSkillLevel < 1 )
	{
		if ( false == bFarmNoSkillGoing )										{		bCanFarm = false;		}
	}
	// 농사로 획득 가능한 물품인지 확인
	if ( ! pclFarmingMgr->IsAvailableFarmingItem( pclinfo->siItemUnique ) )		{		bCanFarm = false;		}
	
	// 현재 있는 지점에서 농사가 가능한지 체크 
	// 현재 위치한 지점에 밭 타일이 있는지 체크	
	if( pclCM->CR[id]->CanFarm( XPos, YPos, &returnval) == false )
	{
		SendServerResponseMsg( 0, SRVAL_FARM_FAIL, returnval, 0, pclCM->CR[id]->GetCharUnique() );
		bCanFarm = false;
	}
	
	// 주어진 좌표로부터 가까운 마을을 찾는다. 
	NearVillageUnique = pclVillageManager->FindNearVillage( XPos, YPos );
	// 마을 번호가 아님으로..
	if ( NearVillageUnique < 1 || NearVillageUnique > CUR_VILLAGE_LIMIT )		{		bCanFarm = false;		}

	// 농장이 없다면 return
	if ( NearVillageUnique > 0 && NearVillageUnique <= CUR_VILLAGE_LIMIT &&
		 pclVillageManager->pclVillageInfo[ NearVillageUnique ]->clVillageVary.clStructureInfo.IsDuplicate( 130 ) == FALSE )
	{
		SendServerResponseMsg( 0, SRVAL_FARM_LANDSTRUCTURE_NOT_EXIST, 0, 0, pclCM->CR[id]->GetCharUnique() );
		bCanFarm = false;
	}

	//cyj 병이 있으면 농경불가
	if( pclChar->pclCI->clHealthInfo.uiOrg1Disease != 0 || pclChar->pclCI->clHealthInfo.uiOrg2Disease != 0 ||
		pclChar->pclCI->clHealthInfo.uiOrg3Disease != 0 || pclChar->pclCI->clHealthInfo.uiOrg4Disease != 0 )
	{
		SendServerResponseMsg(0, SRVAL_FARM_FAIL, CANFARM_ERORR_HAVEDISEASE ,0 , pclChar->GetCharUnique() );
		bCanFarm = false;
	}

	//cyj 레벨이 안되면 농경 불가
	if (pclChar->pclCI->clIP.GetLevel() < CANFARM_LEVEL)
	{
		SendServerResponseMsg(0, SRVAL_FARM_FAIL, CANFARM_ERORR_CANNOTLEVEL ,0 , pclChar->GetCharUnique() );
		bCanFarm = false;
	}

	if( bCanFarm )
	{
		bool bLease = false;
		BOOL bFarmer = false;

		// 1. 임시 임대를 했는지?
		bLease = pclAgriculture->IsLeaseAgriculture( NearVillageUnique );
		// 2. 건물에서 농장 임대를 했는지?
		bFarmer = pclLandManager->IsFarmer( NearVillageUnique, pclCM->CR[id]->pclCI->GetPersonID() );

		// 둘중에 하나라도 있으면, 농경가능
		if( bLease || bFarmer )
		{
			UI32 uiExpireTime = 0;
			UI32 lefttime = 0;

			pclChar->siFarmingItemUnique = pclinfo->siItemUnique;
			pclChar->dwPreGetFarmingItemClock = 0;
			if( bLease == true && bFarmer == false )		// 임시임대인 경우에만 남은 시간을 계산한다.
			{
				uiExpireTime = pclAgriculture->GetExpireTime( NearVillageUnique );
				lefttime = uiExpireTime - pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime );
			}
			pclAgriculture->StartAgriculture( NearVillageUnique, pclClient->CurrentClock );

			SI32 FarmSkillLevel = pclChar->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclChar->pclCI->clCharItem.clItem[0] );
			if ( (FarmSkillLevel < 10) && (true == bFarmNoSkillGoing) )
			{
				FarmSkillLevel = 10;
			}

			SI32 SuccessPercent = pclCM->CR[id]->GetFarmRate( FarmSkillLevel );

			SI32 DelayTime = 40000;
			DelayTime -= ((pclClient->pclItemManager->clItemStatistic.siItemPriceIndex / 100000 + 1 ) / 2 * 2) * 1000;

			DelayTime = max( 20000, DelayTime );

			cltGameMsgResponse_AgricultureStart clGameMsgResponse_AgricultureStart( pclinfo->siItemUnique, SuccessPercent, RANKTYPE_LAND, DelayTime, lefttime );
			cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTURESTART, sizeof(clGameMsgResponse_AgricultureStart), (BYTE*)&clGameMsgResponse_AgricultureStart );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			Success = true;
		}
		else
		{
			cltStrInfo *pStrInfo = GetStrInfo( NearVillageUnique, RANKTYPE_LAND );
			cltStrInfo *pCityHallInfo = GetStrInfo( NearVillageUnique, RANKTYPE_CITYHALL);

			GMONEY RentFee = 0;
			if ( pStrInfo )		RentFee = pStrInfo->clMineStrInfo.clRentContract.siRentFee;
			if ( RentFee == 0 )	RentFee = 10000;

			SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

			// 대만의 경우는 미오픈된 마을의 경우 농장 임대료를 25만원으로 설정한다. (PCK - 08.01.21)
			if (IsWhereServiceArea(ConstServiceArea_Taiwan))
			{
				if( pCityHallInfo && pCityHallInfo->clCityhallStrInfo.siTotalStockAmount == 0 )
				{
					RentFee = 250000;
				}
				else
				{
					RentFee = RentFee * npcrate / 100;
				}
			}
			else
			{
				RentFee = RentFee * npcrate / 100;
			}

			cltGameMsgResponse_AgricultureForNotRentMan clGameMsgResponse_AgricultureForNotRentMan( NearVillageUnique, RentFee, pclinfo->siItemUnique, RANKTYPE_LAND );
			cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTUREFORNOTRENTMAN, sizeof(clGameMsgResponse_AgricultureForNotRentMan), (BYTE*)&clGameMsgResponse_AgricultureForNotRentMan );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			Success = true;
		}
	}

	if ( ! Success )
	{
		OrderForceStop(id, BY_PLAYER);
	}
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_FARMINGPAYRENTFEE(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )						return;
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )											return;

	cltGameMsgRequest_FarmingPayRentFee * pclinfo = (cltGameMsgRequest_FarmingPayRentFee*)pclMsg->cData;
	if ( pclinfo->siRentFee < 1 )								return;

	bool Success = false;
	GMONEY RentFee = 0;
	SI32 NearVillageUnique = 0;
	SI32 XPos = pclCM->CR[id]->GetX();
	SI32 YPos = pclCM->CR[id]->GetY();

	cltStrInfo *pStrInfo = NULL;
	cltStrInfo *pCityHallInfo = NULL;

	bool bCanFarm = true;

	// 현재 있는 지점에서 농사가 가능한지 체크 
	// 현재 위치한 지점에 밭 타일이 있는지 체크
	SI32 returnval = 0;
	if( pclCM->CR[id]->CanFarm( XPos, YPos, &returnval) == false )
	{
		SendServerResponseMsg( 0, SRVAL_FARM_FAIL, returnval, 0, pclCM->CR[id]->GetCharUnique() );
		bCanFarm = false;
	}

	// 임대 유무를 체크하는 것이지
	// 주어진 좌표로부터 가까운 마을을 찾는다. 
	NearVillageUnique = pclVillageManager->FindNearVillage( XPos, YPos );

	// 마을 번호가 아님으로..
	if ( NearVillageUnique < 1 || NearVillageUnique > CUR_VILLAGE_LIMIT )		bCanFarm = false;

	// 농장이 없다면 return
	if( NearVillageUnique > 0 && NearVillageUnique < CUR_VILLAGE_LIMIT &&
		pclVillageManager->pclVillageInfo[ NearVillageUnique ]->clVillageVary.clStructureInfo.IsDuplicate( 130 ) == FALSE )
	{
		bCanFarm = false;
	}

	pStrInfo = GetStrInfo( NearVillageUnique, RANKTYPE_LAND );
	pCityHallInfo = GetStrInfo( NearVillageUnique, RANKTYPE_CITYHALL);

	if( pStrInfo )	RentFee = pStrInfo->clLandStrInfo.clRentContract.siRentFee;
	if( RentFee <= 0 )		RentFee = 10000;

	SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

	// 대만의 경우는 미오픈된 마을의 경우 농장 임대료를 25만원으로 설정한다. (PCK - 08.01.21)
	if (IsWhereServiceArea(ConstServiceArea_Taiwan))
	{
		if( pCityHallInfo && pCityHallInfo->clCityhallStrInfo.siTotalStockAmount == 0 )
		{
			RentFee = 250000;
		}
		else
		{
			RentFee = RentFee * npcrate / 100;
		}
	}
	else
	{
		RentFee = RentFee * npcrate / 100;
	}
	
	// 그 때 가격과 지금 틀림
	if ( RentFee != pclinfo->siRentFee )						bCanFarm = false;
	// 소지금 부족
	if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < RentFee )		bCanFarm = false;

	if( bCanFarm )
	{
		SI32 expireTime = pclClient->GetMinuteFromsTime( sTime ) + 600;

		sDBRequest_AgriculturePayMoneyForRentLand clMsg( PersonID, id, NearVillageUnique, RANKTYPE_LAND, RentFee, pclinfo->siFarmingItemUnique, expireTime );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, RentFee);

		Success = true;
	}

	if ( ! Success )
	{	
		OrderForceStop(id, BY_PLAYER);
	}

	return;
}