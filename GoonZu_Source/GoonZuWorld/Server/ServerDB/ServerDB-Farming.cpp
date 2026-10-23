#include "..\Common\CommonHeader.h"
#include "..\Common\Order\Order.h"
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\CommonLogic\Msg\MsgType-Farming.h"
#include "..\CommonLogic\MsgType-Person.h"

#include "..\..\..\DBManager\GameDBManager\DBMsg-Farming.h"
#include "..\..\..\DBManager\GameDBManager\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager\DBMsg-Item.h"

#include "../CommonLogic/Land/Land.h"

void cltServer::DoMsg_DBMSG_RESPONSE_FARMINGPAYMONEYFORRENTLAND( sPacketHeader* pPacket )
{
	sDBResponse_FarmingPayMoneyForRentLand *pclMsg = (sDBResponse_FarmingPayMoneyForRentLand*)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		if ( pclMsg->siResult == -1 )
		{
			// 소지금 부족
		}
		else
		{
			pclLog->FilePrint( "Farm.txt", "DoMsg_DBMSG_RESPONSE_FARMINGPAYMONEYFORRENTLAND pclMsg->siResult = %d", pclMsg->siResult );
		}

		return;
	}

	// 농장이 없다면 return
	if ( pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ]->clVillageVary.clStructureInfo.IsDuplicate( 130 ) == FALSE )
	{
		return;
	}

	// 농장의 보유 자금을 업데이트 한다. 
	cltStrInfo* pclinfo = pclLandManager->GetStrInfo( pclMsg->siVillageUnique );
	if ( pclinfo )
	{
		pclinfo->clCommonInfo.clMoney.itMoney += pclMsg->siPayMoney;
	}
	
	//cltMoney clmoney;
	//clmoney.Set( pclMsg->)
	//pclLandManager->SetMoney( pclMsg->siVillageUnique, &clmoney );
	
	
	
	SI32 id = pclMsg->siCharID;

	if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
		if ( pclChar )
		{
			pclChar->siFarmingRentVillageUnique = pclMsg->siVillageUnique;
			pclChar->siFarmingItemUnique = pclMsg->siFarmingItemUnique;
		}

		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );

		cltGameMsgResponse_FarmingPayRentFee clinfo( pclMsg->siPayMoney, pclMsg->siLeftMoney, pclMsg->siVillageUnique );
		cltMsg clMsg( GAMEMSG_RESPONSE_FARMINGPAYRENTFEE,  sizeof( clinfo ), (char*)&clinfo );
		pclCM->CR[id]->SendNetMsg((char*)&clMsg);
		
		SI32 FarmSkillLevel = pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[0] );

		cltGameMsgResponse_FarmingStart clGameMsgResponse_FarmingStart( pclMsg->siFarmingItemUnique, pclCM->CR[id]->GetFarmRate( pclChar->siFarmingItemUnique, FarmSkillLevel ) );
		cltMsg clMsg1( GAMEMSG_RESPONSE_FARMINGSTART, sizeof(clGameMsgResponse_FarmingStart), (char*)&clGameMsgResponse_FarmingStart );
		pclCM->CR[id]->SendNetMsg((char*)&clMsg1);
	}

	return;
}