#include "..\Common\CommonHeader.h"

//---------------------------------------
// Server
//---------------------------------------
#include "..\Server\Server.h"
#include "..\Server\Statistics\Statistics.h"


#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest3.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-DailyQuest3.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"


#include "..\Common\DailyQuest3\DailyQuest3Mgr.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgRval-Define.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_DAILYQUEST3LIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest3List * pclinfo = (cltGameMsgRequest_DailyQuest3List*)pclMsg->cData;
	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;	

	cltGameMsgResponse_DailyQuest3List clGameMsgResponse_DailyQuest3List;
#ifdef _SAFE_MEMORY
	pclDailyQuest3Mgr->GetDailyQuest3List( &clGameMsgResponse_DailyQuest3List.m_DailyQuest3ObjDataList[0] );
#else
	pclDailyQuest3Mgr->GetDailyQuest3List( clGameMsgResponse_DailyQuest3List.m_DailyQuest3ObjDataList );
#endif
	SI32 CharLevel  = pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel();
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUEST3LIST, sizeof( clGameMsgResponse_DailyQuest3List ), (BYTE*)&clGameMsgResponse_DailyQuest3List );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//pclDailyQuestMgr->GetDailyQuestObj( pclinfo->siDailyQuestIndex, pclinfo->siDailyQuestCount, FromPersonID, id );
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest3Select( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest3Select * pclinfo = (cltGameMsgRequest_DailyQuest3Select*)pclMsg->cData;

	SI32 charlevel = pclCM->CR[id]->pclCI->clIP.GetLevel();

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;
	
	// 기존에 DailyQuest 진행중인지 체크
	if ( pclCM->CR[id]->pclCI->clDailyQuest3Info.siItemUnique > 0 )
	{
		return;
	}
	
	SI32 ListIndex = -1;
	
	/*if (pclinfo->DailyQuest3ObjData.m_bHiddenQuest != true) {*/
		CDailyQuest3ObjData *pTemp = pclDailyQuest3Mgr->GetDailyQuest3ListObj( pclinfo->DailyQuest3ObjData.m_siIndex, 
			pclinfo->DailyQuest3ObjData.m_siCount,
			&ListIndex );

		if ( pTemp == NULL )
		{
			// 누가 그 퀘스트에 참여해서 퀘스트가 없어 져서 참여 불가능.
			SendServerResponseMsg(0, SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	//}
	// 선택한 퀘스트 퀘스트 리스트에 없음.
	
	// 필요 아이템을 검색해서 지운다.
	int		Inventory;// 인벤토리, 해당 아이템 
	SI32	itemunique = ITEMUNIQUE(24084);

	for( Inventory = PERSONITEM_INV0; Inventory < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; Inventory++ )
	{
		cltItem* pclItem = &pclCM->CR[id]->pclCI->clCharItem.clItem[Inventory];
		cltItem Useitem = NULL;
		Useitem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[Inventory]);
		Useitem.siItemNum = 1;
		if(pclItem == NULL)	return;
		if( pclItem->siUnique == itemunique)
		{
			sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), Inventory, &Useitem, USEITEM_REASON_DAILYQUEST3);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
			break;

		}
		else{
			continue;
		}
	}

	// 선택한 퀘스트가 현재 퀘스트 내용이랑 같은지 확인
	//if ( ! pTemp->IsSameData( &pclinfo->DailyQuest2ObjData ) )
	//{
	//	// 누가 그 퀘스트에 참여해서 퀘스트가 없어 져서 참여 불가능.
	//	SendServerResponseMsg(0, SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST,  0, 0, pclCM->CR[id]->GetCharUnique());
	//	return;
	//}

	//pclinfo->DailyQuest2ObjData.m_siRewardExp 
	//	= (pclinfo->DailyQuest2ObjData.m_siRewardExp * pclinfo->DailyQuest2ObjData.m_siMonsterNum * 130 /100) * (charlevel-75)/10; // 경험치 셋팅
	//pclinfo->DailyQuest2ObjData.m_siRewardExp = (pclinfo->DailyQuest2ObjData.m_siRewardExp /100)*100;//경험치 셋팅.

    //pclinfo->DailyQuest2ObjData.m_siFee = (pclinfo->DailyQuest2ObjData.m_siRewardExp * 6) / 10;// 수수료없음


	/*if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < pclinfo->DailyQuest2ObjData.m_siFee )
	{		
		return;
	}*/
	
	SI32 VillageUnique = pclCM->CR[id]->GetCurrentVillageUnique();

	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager,ITEMUNIQUE(24082));//아이템 시세 받아 오기
	if (pclprice == NULL) return;
	GMONEY VillagePayFeeMoney = pclprice->siPrice*0.1;
	// 퀘스트 시작 아이템 시세의 마켓시세의 10%을 마을 이익금으로 지불
	sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, VillageUnique, VillagePayFeeMoney);
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	if (pclinfo->DailyQuest3ObjData.m_bHiddenQuest == true) { // 이게 히든 퀘스트 이면.
		pclinfo->DailyQuest3ObjData.m_siItemNum = 1;// 신기를 만드는 대신 한개만 만든다.
		pclinfo->DailyQuest3ObjData.m_siRewardItemNum = pclinfo->DailyQuest3ObjData.m_siRewardItemNum*2; // 보상 아이템 숫자 2배
		pclinfo->DailyQuest3ObjData.m_siRewardExp = pclinfo->DailyQuest3ObjData.m_siRewardExp *2; //보상경험치 2배
		// 보상 Gp는 퀘스트가 끝나면서 따로 계산해서 올려준다. 끝날때 따로 올려준다.
		//pclinfo->DailyQuest2ObjData.m_bHiddenQuest = true;
	}

	sDBRequest_DailyQuest3Add clMsg( 
		ListIndex,
		FromPersonID, 
		id,
		pclinfo->DailyQuest3ObjData.m_siItemUnique,
		pclinfo->DailyQuest3ObjData.m_siItemNum,
		pclinfo->DailyQuest3ObjData.m_siRewardItemUnique,
		pclinfo->DailyQuest3ObjData.m_siRewardItemNum,
		pclinfo->DailyQuest3ObjData.m_siRewardExp,
		pclinfo->DailyQuest3ObjData.m_siFee,
		pclinfo->DailyQuest3ObjData.m_siLeftTime,
		pclinfo->DailyQuest3ObjData.m_siLeftTime,
		VillageUnique,
		RANKTYPE_CITYHALL,
		VillagePayFeeMoney,
		false,
		pclinfo->bMoneySelected,
		pclinfo->DailyQuest3ObjData.m_bHiddenQuest
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest3Situation( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest3Situation * pclinfo = (cltGameMsgRequest_DailyQuest3Situation*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuest3Info.siItemUnique == 0 )
	{
		return;
	}

	cltGameMsgResponse_DailyQuest3Situation clGameMsgResponse_DailyQuest3Situation( &pclCM->CR[id]->pclCI->clDailyQuest3Info) ;
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUEST3SITUATION, sizeof( clGameMsgResponse_DailyQuest3Situation ), (BYTE*)&clGameMsgResponse_DailyQuest3Situation );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest3End( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest3End * pclinfo = (cltGameMsgRequest_DailyQuest3End*)pclMsg->cData;
	
	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuest3Info.siItemUnique == 0 )
	{
		return;
	}

	cltItem clItem;
	clItem.Init();
	sDBRequest_DailyQuest3Del clMsg(FromPersonID,id,0,0, pclCM->CR[id]->pclCI->clDailyQuest3Info.siItemUnique,&clItem,1,0,false);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
	return;
}