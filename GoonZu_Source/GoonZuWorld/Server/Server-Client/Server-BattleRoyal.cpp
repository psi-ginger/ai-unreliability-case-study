#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-BattleRoyal.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_BATTLEROYAL_REMAINEDTIME(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_BattleRoyalRemainedTime* pclInfo = (cltGameMsgRequest_BattleRoyalRemainedTime*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	if( NULL != m_pclBattleRoyalMgr)
	{
		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclChar);
		if( NULL != pclBattleRoyal )
		{
			SI32 siTime = pclBattleRoyal->GetNextStartRemainedTime();
			
			cltGameMsgResponse_BattleRoyalRemainedTime clInfo(siTime);
			cltMsg clMsg( GAMEMSG_RESPONSE_BATTLEROYAL_REMAINEDTIME, sizeof(clInfo), (BYTE*)&clInfo );
			pclChar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BATTLEROYAL_MOVETOSTADIUM(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if( NULL == pclChar)
		return;

	if( NULL != m_pclBattleRoyalMgr)
	{
		CBattleRoyal::WarpFrontOfStadium(pclChar);
	}
}
