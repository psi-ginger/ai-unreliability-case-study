#include "../Client.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-ValentineEvent.h"

#include "../CommonLogic/ValentineEvent/ValentineEvent.h"
#include "../common/Char/CharManager/CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_BUF(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ValentineEventBuf* pclInfo = (cltGameMsgResponse_ValentineEventBuf*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CValentineEventClientMgr *pclValentineClientMgr = (CValentineEventClientMgr *) m_pclValentineEventMgr;

	if( NULL == pclValentineClientMgr)
		return;

	pclValentineClientMgr->ReceiveBuf(pclInfo->m_bBufTime, pclInfo->m_siAllBufIndex, pclInfo->m_siVillageBufIndex, pclInfo->m_siUniqueOfBestVillage);

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ValentineEventChangeBuf* pclInfo = (cltGameMsgResponse_ValentineEventChangeBuf*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CValentineEventClientMgr *pclValentineClientMgr = (CValentineEventClientMgr *) m_pclValentineEventMgr;

	if( NULL == pclValentineClientMgr)
		return;

	pclValentineClientMgr->ChangedBufInfo((sValentineBuf*)&(pclInfo->m_sBuf));
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_ISHOLDING(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ValentineEventIsHolding* pclInfo = (cltGameMsgResponse_ValentineEventIsHolding*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	if( NULL == m_pclValentineEventMgr)
		return;

	SI32 siCharID = pclClient->pclCM->GetIDFromCharUnique(pclInfo->m_siCharUnique);
	cltCharClient* pclTargetChar = pclClient->pclCM->GetCharClient(siCharID);
	
	if( pclTargetChar->GetKind() == pclClient->GetUniqueFromHash("KIND_CHOCOLATE"))
	{
		if( pclInfo->m_bHolding)
			((CValentineEventClientMgr*)m_pclValentineEventMgr)->ShowPresentThatStoreSell(pclInfo->m_siStoreNameTextUnique);
		else
			((CValentineEventClientMgr*)m_pclValentineEventMgr)->ShowMsgStoreIsReadyForOpening(pclInfo->m_siStoreNameTextUnique);
	}
	else
	{
		if( pclInfo->m_bHolding)
			((CValentineEventClientMgr*)m_pclValentineEventMgr)->ShowGivePresentMenu(pclTargetChar);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_START(cltMsg* pclMsg, SI32 id)
{
	if( NULL == m_pclValentineEventMgr)
		return;

	((CValentineEventClientMgr*)m_pclValentineEventMgr)->HoldEvent();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_END(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ValentineEventEnd* pclInfo = (cltGameMsgResponse_ValentineEventEnd*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	if( NULL == m_pclValentineEventMgr)
		return;

	((CValentineEventClientMgr*)m_pclValentineEventMgr)->CloseEvent(pclInfo->m_siPopularNPCCharKind, pclInfo->m_siVillageUnique);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_GIVE_PRESENT_TONPC(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ValentineEventGivePresentToNPC* pclInfo = (cltGameMsgResponse_ValentineEventGivePresentToNPC*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	SI32 siNPCCharID = pclCM->GetIDFromCharKind(pclInfo->m_siNPCCharKind);

	cltCharClient* pclNPC = (cltCharClient* )pclCM->GetCharClient(siNPCCharID);

	if( NULL == pclNPC)
		return;
	
	if( NULL == m_pclValentineEventMgr)
		return;

	((CValentineEventClientMgr*)m_pclValentineEventMgr)->ShowEmoticonWhenNpcReceivedItem(pclNPC);
}