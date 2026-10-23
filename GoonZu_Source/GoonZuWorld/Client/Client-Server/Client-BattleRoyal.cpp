#include "../Client.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-BattleRoyal.h"

#include "../CommonLogic/BattleRoyal/CBattleRoyalClient.h"
#include "../common/Char/CharManager/CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_NOTICERESULT(cltMsg* pclMsg, SI32 id)
{
	//	배틀로얄 결과
	cltGameMsgResponse_BattleRoyalNoticeResult* pclInfo = (cltGameMsgResponse_BattleRoyalNoticeResult*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		pclBattleRoyalClient->NoticeResult(pclInfo->m_bResult, pclInfo->m_szWinnerName, pclInfo->m_siMapIndex);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_REMAINEDTIME(cltMsg* pclMsg, SI32 id)
{
	//	배틀로얄 결과
	cltGameMsgResponse_BattleRoyalRemainedTime* pclInfo = (cltGameMsgResponse_BattleRoyalRemainedTime*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		pclBattleRoyalClient->ShowTime(pclInfo->m_siTime);
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_STARTGAME(cltMsg* pclMsg, SI32 id)
{
	//	배틀로얄 결과
	cltGameMsgResponse_BattleRoyalStartGame* pclInfo = (cltGameMsgResponse_BattleRoyalStartGame*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		pclBattleRoyalClient->SetGameMode(pclInfo->m_siMode);
		pclBattleRoyalClient->ShowTimerForGamingState(pclInfo->m_siTime);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BattleRoyalEndGame* pclInfo = (cltGameMsgResponse_BattleRoyalEndGame*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		pclBattleRoyalClient->HideTimerForGamingState();
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BattleRoyalPowerupMode* pclInfo = (cltGameMsgResponse_BattleRoyalPowerupMode*)pclMsg->cData;

	if(pclInfo == NULL)
		return;

	cltCharClient* pclCharSelf = pclClient->pclCM->GetCharClient(1);
	if( NULL ==pclCharSelf)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		if( pclCharSelf->GetMapIndex() == MAPINDEX_BATTLEROYAL_GRANDMABRIUM || pclCharSelf->GetMapIndex() == MAPINDEX_BATTLEROYAL_MABRIUM 
			|| pclInfo->m_bValue == false)
		{
			SI16 siTargetID = pclCM->GetIDFromCharUnique(pclInfo->m_siCharUnique);

			cltCharClient* pclchar = pclCM->GetCharClient(siTargetID);
			if( NULL == pclchar)
				return;
			pclchar->bBattleRoyalPowerupMode = pclInfo->m_bValue;
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_OPENSTADIUM(cltMsg* pclMsg, SI32 id)
{
	cltCharClient* pclCharSelf = pclClient->pclCM->GetCharClient(1);
	if( NULL == pclCharSelf)
		return;

	CBattleRoyalClient * pclBattleRoyalClient = m_pclBattleRoyal ;
	if( NULL != pclBattleRoyalClient)
	{
		pclBattleRoyalClient->ShowWarpButton();
	}

}
