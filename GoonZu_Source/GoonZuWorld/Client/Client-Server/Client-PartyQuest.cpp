#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"

#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../ninterface/npartydlg/NPartyDlg.h"

#include "../../CommonLogic/MsgType-PartyQuest.h"
#include "../PartyQuestMgr/PartyQuestMgr_Client.h"


// [종호] 파티퀘스트 시작시 조건 체크 결과
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyQuest_CheckStart* pclInfo = (cltGameMsgResponse_PartyQuest_CheckStart*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	// 이곳에서 퀘스트 시작시 조건 체크 결과에 따른 처리를 합니다.
	
	// 파티퀘스트 시작 가능/불가능
	SI32 siMessage = 0;
	if(pclInfo->m_bResult == true)
		siMessage = PARTYQUEST_MESSAGE_CAN_START;
	else
		siMessage = PARTYQUEST_MESSAGE_CANNOT_START;


	((CNPartyDlg*)m_pDialog[NPARTY_DLG])->StartPartyQuestMessage(siMessage);
}


// [종호] 파티퀘스트 시작 요청 결과
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_START(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyQuest_Start* pclInfo = (cltGameMsgResponse_PartyQuest_Start*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	// 파티퀘스트를 받았습니다.
	m_pPartyQuestMgr->StartPartyQuestData(&pclInfo->m_PartyQuestData);
}


// 파티퀘스트 정보 갱신
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_UPDATE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyQuest_Update* pclInfo = (cltGameMsgResponse_PartyQuest_Update*)pclMsg->cData;

	if(pclInfo == NULL)
		return;


    m_pPartyQuestMgr->UpdatePartyQuestData(&pclInfo->m_PartyQuestData, pclInfo->m_dwQuestTime, pclInfo->m_bClear);
}


// 파티퀘스트 포기
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_GIVEUP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyQuest_GiveUp* pclInfo = (cltGameMsgResponse_PartyQuest_GiveUp*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	// 파티퀘스트가 종료됐습니다. 플레이어가 가지고있는 파티퀘스트 정보를 초기화 합니다.
	cltClient* pClient = (cltClient*)pclClient;
	pClient->m_pPartyQuestMgr->DoGiveUpPartyQuest();

	// 파티원들에게 알려줍니다.
	((CNPartyDlg*)m_pDialog[NPARTY_DLG])->StartPartyQuestMessage(pclInfo->m_siMessage);
}


