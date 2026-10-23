#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-DormancyAccount.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_DORMANCY_TAKEOUTEQUIPITEMINBOX(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DormancyTakeOutEquipItemInBox* pclInfo = (cltGameMsgRequest_DormancyTakeOutEquipItemInBox*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	CDormancyEquipItemRewardServer *pclDormancyServer = (CDormancyEquipItemRewardServer *)pclDormancySystem->GetDormancyEquipItemReward();
	if(NULL == pclDormancyServer)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	pclDormancyServer->GiveItemToPlayer(pclInfo->m_siBoxUnique,pclInfo->m_siEquipItemUnique, pclInfo->m_siBoxPos, pclChar, pclInfo->m_siEquipItemIndex);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DORMANCY_BUYPOINTITEM(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DormancyBuyPoinItem* pclInfo = (cltGameMsgRequest_DormancyBuyPoinItem*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	CDormancyPointRewardServer *pclDormancyServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
	if(NULL == pclDormancyServer)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	pclDormancyServer->BuyPointItemRequest(id, pclChar, pclInfo->m_siItemUnique);
}
