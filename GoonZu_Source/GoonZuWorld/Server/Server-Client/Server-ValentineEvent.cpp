#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-ValentineEvent.h"
#include "../CommonLogic/ValentineEvent/ValentineEvent.h"
#include "../CommonLogic/ValentineEvent/ValentineStore.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEEVENT_GIVE_PRESENT_TONPC(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ValentineEventGivePresentToNPC* pclInfo = (cltGameMsgRequest_ValentineEventGivePresentToNPC*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	if( NULL == m_pclValentineEventMgr )
		return;

	if( !m_pclValentineEventMgr->IsHolding())
		return;

	((CValentineEventServerMgr*)m_pclValentineEventMgr)->GivePresentItem(pclChar, pclInfo->m_siNPCCharKind, pclInfo->m_clPresentItem ,pclInfo->m_siInvenIndexOfPresentItem);


}
void cltServer::DoMsg_GAMEMSG_REQUEST_VALENTINEEVENT_ISHOLDING(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ValentineEventIsHolding* pclInfo = (cltGameMsgRequest_ValentineEventIsHolding*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	if( NULL == m_pclValentineEventMgr )
		return;

	CValentineStore* pclStore = ((CValentineEventServerMgr*)m_pclValentineEventMgr)->FindStoreByCharUnique(pclInfo->m_siCharUnique);
	
	SI32 siStoreNameTextUnique = 0;
	if( NULL == pclStore)
	{
		SI32 siNPCCharID = pclCM->GetIDFromCharUnique(pclInfo->m_siCharUnique);
		cltCharServer* pclNPC = pclCM->GetCharServer(siNPCCharID);
		if(NULL == pclNPC)
		{
			return;
		}
		if( false == m_pclValentineEventMgr->IsValentineNPC(pclNPC->GetKind()))
			return;
	}
	else
	{
		siStoreNameTextUnique = pclStore->GetNameTextUnique();
	}

	cltGameMsgResponse_ValentineEventIsHolding clSendMsg(pclInfo->m_siCharUnique, m_pclValentineEventMgr->IsHolding(),siStoreNameTextUnique);
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINEEVENT_ISHOLDING, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}