#include "..\Common\CommonHeader.h"

#include "..\Common\Order\Order.h"
#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"
#include "..\CommonLogic\Msg\MsgType-Fishing.h"
#include "..\CommonLogic\MsgType-Person.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Fishing.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

void cltServer::DoMsg_DBMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY(sPacketHeader* pPacket)
{
	sDBResponse_FishingDecreaseToolDurability *pclMsg = (sDBResponse_FishingDecreaseToolDurability*)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;

	if ( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		// 내구도 소모 시킴.
		for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
		{
			if ( pclMsg->clFishingTool[ i ].siPos > 0 )
			{
				if( pclMsg->clFishingTool[ i ].siPos < 0 || pclMsg->clFishingTool[ i ].siPos >= MAX_ITEM_PER_PERSON )
					pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->clFishingTool[ i ].siPos ].Set( &pclMsg->clFishingTool[ i ].clItem );
			}
		}

		cltGameMsgResponse_FishingDecreaseToolDurability clGameMsgResponse_FishingDecreaseToolDurability( pclMsg->clFishingTool );
		cltMsg clMsg( GAMEMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY, sizeof( clGameMsgResponse_FishingDecreaseToolDurability ), (BYTE*)&clGameMsgResponse_FishingDecreaseToolDurability );
		pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 10. 11] // 낚시대 내구 감소 내역.(수정필요 : 도구관련 배열 10개를 어떻게 하나?)
		//	//  
		//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_FISHING_TOOL_DUR_DECREASE, 
		//									0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		//									0, 0, 0, 0, 0, NULL, NULL);
		//}
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_FISHINGGETMARINEPRODUCTS(sPacketHeader* pPacket)
{
	sDBResponse_FishingGetMarineProducts *pclMsg = (sDBResponse_FishingGetMarineProducts *)pPacket;
	
	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;
	
	// 어획술로 획득한거 통계에 추가
	if ( pclMsg->clLeftItem.siUnique > 0 )
	{
		//cyj 생산품 통계 추가
		pclItemManager->AddProductionItemNum( pclMsg->clLeftItem.siUnique, pclMsg->siAddedItemNum );

		// 새로운 통계 - 생산술에 의한 공급 물품의 통계를 낸다.
		NStatistics_Add("Item", pclMsg->clLeftItem.siUnique, "ProductionNum", pclMsg->siAddedItemNum);
		NStatistics_Set("Item", pclMsg->clLeftItem.siUnique, "Name", pclItemManager->GetName(pclMsg->clLeftItem.siUnique));
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		// 아이템 Update
		if ( pclMsg->siAcquireMarineInvPos > 0 )
		{
			if( pclMsg->siAcquireMarineInvPos >= 0 && pclMsg->siAcquireMarineInvPos < MAX_ITEM_PER_PERSON )
				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siAcquireMarineInvPos ].Set( &pclMsg->clLeftItem );
		}

		cltGameMsgResponse_FishingGetMarineProducts clGameMsgResponse_FishingGetMarineProducts
			(
			FISHING_SUCCESS,
			pclMsg->clLeftItem.siUnique,				
			pclMsg->siAcquireMarineInvPos, 
			&pclMsg->clLeftItem,				
			pclMsg->siKind
			);
		cltMsg clMsg( GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTS, sizeof( clGameMsgResponse_FishingGetMarineProducts ), (BYTE*)&clGameMsgResponse_FishingGetMarineProducts );		
		pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 낚시로 고기 낚은 내용이 기록.
			// Item = 잡은 수산물
			// NPCKind = 잡은 캐릭터의 KIND
			// param1 == 수산물이 들어간 위치,  
			// param2 == 얻은 경험치
			// param3 == 최종경험치
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_FISHING, 
								FISHING_SUCCESS, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clAcquireMarineProductItem, pclMsg->clLeftItem.siItemNum, 0, pclMsg->siKind, 0, 0, 0, 
								pclMsg->siAcquireMarineInvPos, pclMsg->siIncSkillUnique, pclMsg->siSkillTotalExp, 0, 0, 
								NULL, NULL);
		}
		
		cltGameMsgResponse_FishingGetMarineProductsNotify clGameMsgResponse_FishingGetMarineProductsNotify
			(
			FISHING_SUCCESS,
			pclMsg->clLeftItem.siUnique,
			pclMsg->siAddedItemNum,
			pclMsg->siCharUnique,
			pclMsg->siKind
			);
		cltMsg clMsg1( GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTSNOTIFY, sizeof( clGameMsgResponse_FishingGetMarineProductsNotify ), (BYTE*)&clGameMsgResponse_FishingGetMarineProductsNotify );
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg( &clMsg1, true);

		//[지연] - 2010신년이벤트 : 낚시후 3% 확률로 1개의 포춘쿠키 조각 획득
		((cltCharServer*)(pclCM->CR[id]))->GiveFortuneCookiePiece( 30, 1 );
	}

	return;
}