#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "../../CommonLogic/MsgType-PartyQuest.h"

#include "../../common/PartyMgr/PartyMgr.h"
#include "../../common/PartyMgr/PartyObj.h"

// [종호] 파티퀘스트 시작시 조건 체크	
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_QUEST_CHECKSTART(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PartyQuest_CheckStart* pclInfo = (cltGameMsgRequest_PartyQuest_CheckStart*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	// 이곳에서 파티퀘스트 시작 요청을 한 파티가 파티퀘스트 시작 조건을 갖추고 있는지 체크 합니다.
	bool bResult = false;

	cltServer* pClient = (cltServer*)pclClient;

	CPartyObj* pPartyObject = pClient->pclPartyMgr->GetPartyObj(pclInfo->m_siPartyIndex);
	if(pPartyObject == NULL)
		return;

	bResult = m_pPartyQuestMgr->DoCheckPartyQuest(pPartyObject);

	// 체크 후 결과 값을 리턴 합니다.
	cltGameMsgResponse_PartyQuest_CheckStart clInfo(bResult);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART, sizeof(clInfo), (BYTE*)&clInfo);
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg);
}


// [종호] 파티퀘스트 시작
void cltServer::DoMsg_GAMEMSG_REQUEST_PARTY_QUEST_START(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_PartyQuest_Start* pclInfo = (cltGameMsgRequest_PartyQuest_Start*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	// 파티퀘스트 수락하기 이전에 파티원들의 정보가 변경되었을수 있으므로 부여하기 직전 다시 체크 합니다.
	cltServer* pClient = (cltServer*)pclClient;

	CPartyObj* pPartyObject = pClient->pclPartyMgr->GetPartyObj(pclInfo->m_siPartyIndex);
	if(pPartyObject == NULL)
		return;

	bool bResult = m_pPartyQuestMgr->DoCheckPartyQuest(pPartyObject);
	
	if(bResult == false)
	{
		cltGameMsgResponse_PartyQuest_CheckStart clInfo(false);
		cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART, sizeof(clInfo), (BYTE*)&clInfo);
		pclChar->SendNetMsg( (sPacketHeader*)&clMsg);
		
		return;
	}

	// 해당 파티에 퀘스트를 부여 합니다.
	SI32 siCategoryIndex	= pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());
	SI32 siMissionIndex		= pclClient->m_pServer_PartyMatchingMgr->Get_Mission(pPartyObject->GetPartyLeaderUnique());

	if(siCategoryIndex < 0 || siCategoryIndex >= MAX_PARTYQUEST_CATEGORY_NUM)
		return;

	if(siMissionIndex < 0 || siMissionIndex >= MAX_PARTYQUEST_MISSION_NUM)
		return;

	// 파티에게 파티퀘스트 부여
	bResult = pClient->m_pPartyQuestMgr->SetPartyQuestData(&pPartyObject->m_PartyQuestInfo, siCategoryIndex, siMissionIndex);

	pPartyObject->m_dwEndTime = pclClient->CurrentClock + (pPartyObject->m_PartyQuestInfo.m_siTimeLimit*1000);
	pPartyObject->m_dwElapseTime = pclClient->CurrentClock;

	if(bResult == false)
	{
		cltGameMsgResponse_PartyQuest_CheckStart clInfo(false);
		cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART, sizeof(clInfo), (BYTE*)&clInfo);
		pclChar->SendNetMsg( (sPacketHeader*)&clMsg);

		return;
	}

		

	// 퀘스트가 시작 되었습니다. 파티원들에게 알려줍니다.
	cltGameMsgResponse_PartyQuest_Start clInfo(&pPartyObject->m_PartyQuestInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_START, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->SendAllPartyMemberMsg((sPacketHeader*)&clMsg, pclChar->siPartyIndex);

	// 정상적으로 퀘스트 시작이 수행되면 파티매칭 리스트에서 파티매칭 올렸던걸 지워준다.
	if ( NULL != pclClient->m_pServer_PartyMatchingMgr )
	{
		pclClient->m_pServer_PartyMatchingMgr->Remove_PartyMatching( pclChar->GetCharUnique() );
	}
}






