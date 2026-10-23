#include "..\Common\CommonHeader.h"

//---------------------------------------
// Server
//---------------------------------------
#include "..\Server\Server.h"
#include "..\Server\Statistics\Statistics.h"


#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-DailyQuest.h"

#include "..\Common\DailyQuest\DailyQuestMgr.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_DAILYQUESTLIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuestList * pclinfo = (cltGameMsgRequest_DailyQuestList*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;	
	
	cltGameMsgResponse_DailyQuestList clGameMsgResponse_DailyQuestList;
#ifdef _SAFE_MEMORY
	pclDailyQuestMgr->GetDailyQuestList( &clGameMsgResponse_DailyQuestList.m_DailyQuestObjDataList[0] );
#else
	pclDailyQuestMgr->GetDailyQuestList( clGameMsgResponse_DailyQuestList.m_DailyQuestObjDataList );
#endif
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUESTLIST, sizeof( clGameMsgResponse_DailyQuestList ), (BYTE*)&clGameMsgResponse_DailyQuestList );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	//pclDailyQuestMgr->GetDailyQuestObj( pclinfo->siDailyQuestIndex, pclinfo->siDailyQuestCount, FromPersonID, id );
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuestSelect( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuestSelect * pclinfo = (cltGameMsgRequest_DailyQuestSelect*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;
	
	// 기존에 DailyQuest 진행중인지 체크
	if ( pclCM->CR[id]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		return;
	}
	
	SI16 ListIndex = -1;
	// 선택한 퀘스트 퀘스트 리스트에 없음.
	CDailyQuestObjData *pTemp = pclDailyQuestMgr->GetDailyQuestListObj( pclinfo->DailyQuestObjData.m_siIndex, pclinfo->DailyQuestObjData.m_siCount, &ListIndex );
	
	if ( pTemp == NULL )
	{
		// 누가 그 퀘스트에 참여해서 퀘스트가 없어 져서 참여 불가능.
		SendServerResponseMsg(0, SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	// pc방 퀘스트인 경우 사용자가 pc방인지 여부 확인
/*	if ( pTemp->m_bPCRoomQuest )
	{
		if( !pclCM->CR[ id ]->pclCI->clBI.IsPCRoom() ) return;
	}*/

	// 선택한 퀘스트가 현재 퀘스트 내용이랑 같은지 확인
	if ( ! pTemp->IsSameData( &pclinfo->DailyQuestObjData ) )
	{
		// 누가 그 퀘스트에 참여해서 퀘스트가 없어 져서 참여 불가능.
		SendServerResponseMsg(0, SRVAL_DAILYQUEST_FAIL_NOTEXISTLIST,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if ( pclCM->CR[id]->pclCI->clIP.GetMoney() < pclinfo->DailyQuestObjData.m_siFee )
	{		
		return;
	}
	
	SI32 VillageUnique = pclCM->CR[id]->GetCurrentVillageUnique();
	// 필드에서도 모병관 다이얼로그 열려있으면 되게 할려고 안 막음
	//if( VillageUnique < 1 || VillageUnique >= MAX_VILLAGE_NUMBER )	
	//{
	//	return;
	//}

	GMONEY VillagePayFeeMoney = pclinfo->DailyQuestObjData.m_siFee * VILLAGE_PROFIT_RATE_FOR_DAILYQUEST /100;

	// 퀘스트가 선택되었으므로, 리스트에서 삭제한다.
	pclDailyQuestMgr->InitQuestListData( ListIndex, pclinfo->DailyQuestObjData.m_siMonsterKind, 
										 pclinfo->DailyQuestObjData.m_siMonsterNum,
										 pclinfo->DailyQuestObjData.m_siRewardItemUnique, pclinfo->DailyQuestObjData.m_siRewardItemNum, 
										 pclinfo->DailyQuestObjData.m_siRewardMoney, pclinfo->DailyQuestObjData.m_siRewardExp, 
										 pclinfo->DailyQuestObjData.m_siFee, pclinfo->DailyQuestObjData.m_siLeftTime);

	sDBRequest_DailyQuestAdd clMsg( 
		ListIndex,
		FromPersonID, 
		id,
		pclinfo->DailyQuestObjData.m_siMonsterKind,
		pclinfo->DailyQuestObjData.m_siMonsterNum,
		pclinfo->DailyQuestObjData.m_siRewardItemUnique,
		pclinfo->DailyQuestObjData.m_siRewardItemNum,
		pclinfo->DailyQuestObjData.m_siRewardMoney,
		pclinfo->DailyQuestObjData.m_siRewardExp,
		pclinfo->DailyQuestObjData.m_siFee,
		pclinfo->DailyQuestObjData.m_siLeftTime,
		pclinfo->DailyQuestObjData.m_siLeftTime,
		VillageUnique,
		RANKTYPE_CITYHALL,
		VillagePayFeeMoney,
		false,
		pclinfo->bMoneySelected
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, VillagePayFeeMoney);
	
	// 모병관 퀘스트 수락 통계. 
	pclStatisticsManager->clDailyStatistics.siDailyQuestAcceptNum ++;

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuestSituation( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuestSituation * pclinfo = (cltGameMsgRequest_DailyQuestSituation*)pclMsg->cData;

	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuestInfo.siMonsterKind == 0 )
	{
		return;
	}

	cltGameMsgResponse_DailyQuestSituation clGameMsgResponse_DailyQuestSituation( &pclCM->CR[id]->pclCI->clDailyQuestInfo) ;
	cltMsg clMsg( GAMEMSG_RESPONSE_DAILYQUESTSITUATION, sizeof( clGameMsgResponse_DailyQuestSituation ), (BYTE*)&clGameMsgResponse_DailyQuestSituation );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DailyQuestEnd( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DailyQuestEnd * pclinfo = (cltGameMsgRequest_DailyQuestEnd*)pclMsg->cData;
	
	if( pclCM->IsValidID( id ) == false )return;

	SI32 FromPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( FromPersonID < 1) return ;

	// 기존에 DailyQuest 진행중이 아니라면 return
	if ( pclCM->CR[id]->pclCI->clDailyQuestInfo.siMonsterKind == 0 )
	{
		return;
	}

	cltItem clItem;
	clItem.Init();
	sDBRequest_DailyQuestDel clMsg(
		FromPersonID,
		id,
		0,
		0,
		&clItem,
		1,
		0,
		0,
		0
		);

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
	return;
}