#include "..\Common\CommonHeader.h"

//---------------------------------------
// Server
//---------------------------------------
#include "..\Server\Server.h"
#include "..\Server\Statistics\Statistics.h"


#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest2.h"


#include "MsgRval-Define.h"
#include "Msg\MsgType-DailyQuest2.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"


#include "..\Common\DailyQuest2\DailyQuest2Mgr.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgRval-Define.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_DAILYQUEST2LIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest2List * pclinfo = (cltGameMsgRequest_DailyQuest2List*)pclMsg->cData;
	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;	

	cltGameMsgResponse_DailyQuest2List clGameMsgResponse_DailyQuest2List;
#ifdef _SAFE_MEMORY
	pclDailyQuest2Mgr->GetDailyQuest2List( &clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[0] );
#else
	pclDailyQuest2Mgr->GetDailyQuest2List( clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList );
#endif
	SI32 CharLevel  = pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel();
	

	// 보상 새로 계산.
	for(SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; i ++){
		if (clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemUnique == ITEMUNIQUE(5380) ||
			clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemUnique == ITEMUNIQUE(7175) ||
			clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemUnique == ITEMUNIQUE(6140) ) {
				cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager,
					clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemUnique);//아이템 시세 받아 오기
				if(clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siMonsterKind == NULL) continue;
				if (pclprice == NULL) continue;		
				SI32 addReWardExp = (clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardExp * clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siMonsterNum * 130/100 * ((float)(CharLevel-75)/10))+100;
				SI32 addItemnum = rand()%8+3;
				SI32 addguildpoint = addReWardExp/100;
				clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardExp = addReWardExp;
				clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemNum = addItemnum;
				clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardGuildPoint = addguildpoint;
		}
		else{
			cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager,
				clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemUnique);//아이템 시세 받아 오기
			if(clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siMonsterKind == NULL) continue;
			if (pclprice == NULL) continue;		
			SI32 addReWardExp = (clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardExp * clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siMonsterNum * 130/100 * ((float)(CharLevel-75)/10));
			// 100을 추가로 준다 
			addReWardExp = addReWardExp	+ 100 ;
			// 100 단위 절삭 
			addReWardExp = (addReWardExp	/100 )	* 100	;

			SI32 addItemnum = (SI32)(min(60, max(1,(addReWardExp*4)/(pclprice->siPrice*10))));
			SI32 addguildpoint = addReWardExp/100;
			clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardExp = addReWardExp;
			clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardItemNum = addItemnum;
			clGameMsgResponse_DailyQuest2List.m_DailyQuest2ObjDataList[i].m_siRewardGuildPoint = addguildpoint;

		}
	}
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUEST2LIST, sizeof( clGameMsgResponse_DailyQuest2List ), (BYTE*)&clGameMsgResponse_DailyQuest2List );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//pclDailyQuestMgr->GetDailyQuestObj( pclinfo->siDailyQuestIndex, pclinfo->siDailyQuestCount, FromPersonID, id );
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest2Select( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest2Select * pclinfo = (cltGameMsgRequest_DailyQuest2Select*)pclMsg->cData;

	SI32 charlevel = pclCM->CR[id]->pclCI->clIP.GetLevel();

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;
	
	// 기존에 DailyQuest 진행중인지 체크
	if ( pclCM->CR[id]->pclCI->clDailyQuest2Info.siMonsterKind > 0 ){		return;	}
	
	SI16 ListIndex = -1;
	// 선택한 퀘스트 퀘스트 리스트에 없음.
	CDailyQuest2ObjData *pTemp = pclDailyQuest2Mgr->GetDailyQuest2ListObj( pclinfo->DailyQuest2ObjData.m_siIndex, pclinfo->DailyQuest2ObjData.m_siCount, &ListIndex );
	
	if ( pTemp == NULL )
	{
		// 누가 그 퀘스트에 참여해서 퀘스트가 없어 져서 참여 불가능.
		SendServerResponseMsg(0, SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}
	// 필요 아이템을 검색해서 지운다.
	int		Inventory;// 인벤토리, 해당 아이템 
	SI32	itemunique = ITEMUNIQUE(24082);

	for( Inventory = PERSONITEM_INV0; Inventory < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; Inventory++ )
	{
		cltItem* pclItem = &pclCM->CR[id]->pclCI->clCharItem.clItem[Inventory];
		cltItem Useitem = NULL;
		Useitem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[Inventory]);
		Useitem.siItemNum = 1;
		if(pclItem == NULL)	return;
		if( pclItem->siUnique == itemunique)
		{
			sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), Inventory, &Useitem, USEITEM_REASON_DAILYQUEST2);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
			break;

		}
		else{
			continue;
		}
	}	
	SI32 VillageUnique = pclCM->CR[id]->GetCurrentVillageUnique();

	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager,ITEMUNIQUE(24082));//아이템 시세 받아 오기
	if (pclprice == NULL) return;

	GMONEY VillagePayFeeMoney = pclprice->siPrice*0.1;// 퀘스트 시작 아이템 시세의 마켓시세의 10%
	// 퀘스트 시작 아이템 시세의 마켓시세의 10%을 마을 이익금으로 지불
	sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, VillageUnique, VillagePayFeeMoney);
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	//레벨이 120 이상이면 경험치 공식 수정
	SI32 siRewardExp	=	pclinfo->DailyQuest2ObjData.m_siRewardExp	;
	SI32 siMonsterKind	=	pclinfo->DailyQuest2ObjData.m_siMonsterKind	;
	SI32 siMonsterNum	=	pclinfo->DailyQuest2ObjData.m_siMonsterNum	;
	SI32 siRewardItemUnique	=	pclinfo->DailyQuest2ObjData.m_siRewardItemUnique	;
	SI32 siRewardItemNum	=	pclinfo->DailyQuest2ObjData.m_siRewardItemNum	;
	SI32 siRewardMoney		=	pclinfo->DailyQuest2ObjData.m_siRewardMoney	;

	SI32 siCharLevel = pclCM->CR[id]->pclCI->clIP.GetLevel() ;	
	if (siCharLevel >= 120 ) 
	{
		if ( siMonsterKind > 0 ) 
		{
			cltKindInfo* pclki = pclKindInfoSet->pclKI[siMonsterKind] ;	
			if(pclki != NULL )
			{
				SI32 MonsterExp = pclki->Exp	;
				siRewardExp	=	((MonsterExp *  siMonsterNum * 580) / 100 ) + 100 ;
				//100단위 절삭
				siRewardExp = (siRewardExp / 100) * 100;
			}
		}
	}
	// 그이하면 그대로 적용 
    if (pclinfo->DailyQuest2ObjData.m_bHiddenQuest == true) 
	{	
		// 이게 히든 퀘스트 이면.
		siMonsterNum = siMonsterNum * 2;//몬스터 숫자 배
		siRewardItemNum = siRewardItemNum * 2; // 보상 아이템 숫자 2배
		siRewardExp =  siRewardExp * 2; //보상경험치 2배
	}
	sDBRequest_DailyQuest2Add clMsg( 
		ListIndex,
		FromPersonID, 
		id,
		siMonsterKind,
		siMonsterNum,
		siRewardItemUnique,
		siRewardItemNum,
		siRewardMoney,
		siRewardExp,
		pclinfo->DailyQuest2ObjData.m_siFee,
		pclinfo->DailyQuest2ObjData.m_siLeftTime,
		pclinfo->DailyQuest2ObjData.m_siLeftTime,
		VillageUnique,
		RANKTYPE_CITYHALL,
		VillagePayFeeMoney,
		false,
		pclinfo->bMoneySelected,
		pclinfo->DailyQuest2ObjData.m_bHiddenQuest
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest2Situation( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest2Situation * pclinfo = (cltGameMsgRequest_DailyQuest2Situation*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuest2Info.siMonsterKind == 0 )
	{
		return;
	}

	cltGameMsgResponse_DailyQuest2Situation clGameMsgResponse_DailyQuest2Situation( &pclCM->CR[id]->pclCI->clDailyQuest2Info) ;
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUEST2SITUATION, sizeof( clGameMsgResponse_DailyQuest2Situation ), (BYTE*)&clGameMsgResponse_DailyQuest2Situation );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuest2End( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuest2End * pclinfo = (cltGameMsgRequest_DailyQuest2End*)pclMsg->cData;
	
	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuest2Info.siMonsterKind == 0 )
	{
		return;
	}

	cltItem clItem;
	clItem.Init();
	sDBRequest_DailyQuest2Del clMsg(
		FromPersonID,
		id,
		0,
		0,
		&clItem,
		1,
		0,
		0,
		//0,
		//0,
		false
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
	return;
}