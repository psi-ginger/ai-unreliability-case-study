#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"
#include "../CommonLogic/Msg/MsgType-MofuMofuEvent.h"
#include "../CommonLogic/msg/MsgType-Village.h"
#include "Char\CharManager\CharManager.h"

#include "..\CommonLogic\Msg\MsgType-Vote.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "..\Interface\SoundOnOffDlg\SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../ninterface/NMofuMofuList/NMofuMofuList.h"
#include "../ninterface/nnotifygetexpanditemdlg/NNotifyGetExpAndItemDlg.h"






void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_Reserve *pclInfo = (cltGameMsgResponse_MofuMofuEvent_Reserve*)pclMsg->cData;

	if(pclInfo->m_siResult == 1)
	{
		TCHAR* pTitle = GetTxtFromMgr(6694);
		TCHAR* pText = GetTxtFromMgr(7074);

		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_ReserveList *pclInfo = (cltGameMsgResponse_MofuMofuEvent_ReserveList*)pclMsg->cData;

	if ( m_pDialog[NMOFUMOFULIST_DLG] == NULL )
		return;

	((CNMofuMofuListDlg*)m_pDialog[NMOFUMOFULIST_DLG])->SetReserveList(pclInfo->m_siCount, pclInfo->m_Reserved);
	((CNMofuMofuListDlg*)m_pDialog[NMOFUMOFULIST_DLG])->RefreshList();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_START(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_Start *pclInfo = (cltGameMsgResponse_MofuMofuEvent_Start*)pclMsg->cData;

	siMofuMofuEvent_VilliageUnique = pclInfo->m_siVillageUnique;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_END(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_End *pclInfo = (cltGameMsgResponse_MofuMofuEvent_End*)pclMsg->cData;
	
	siMofuMofuEvent_VilliageUnique = 0;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_SETINFO(cltMsg* pclMsg, SI32 id)
{
	//cltGameMsgResponse_MofuMofuEvent_SetInfo *pclInfo = (cltGameMsgResponse_MofuMofuEvent_SetInfo*)pclMsg->cData;

	//pclCM->CR[1]->siMofuMofuEventField = pclInfo->m_siFieldIndex;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_INFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_Info *pclInfo = (cltGameMsgResponse_MofuMofuEvent_Info*)pclMsg->cData;

	pclMessage->SetMofuMofuEventMsg(pclInfo->m_siLeftSecond, pclInfo->m_siMaMaMofuMofu, pclInfo->m_siKilledMofuMofu, pclInfo->m_siTotalMofuMofu);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_MAMAMOFUMOFU(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu *pclInfo = (cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu*)pclMsg->cData;

	SI32 siCharID = cltCharCommon::CharUniqueInfo[pclInfo->m_siCharUnique];
	cltCharCommon* pclchar = pclClient->pclCM->CR[siCharID];
	if(pclchar != NULL)
	{
		if(pclClient->pclCM->CR[siCharID]->clIdentity.siIdentity == IDENTITY_MOFUMOFUEVENT)
		{
			if(pclClient->pclCM->CR[siCharID]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_MAMARACCOON")))
			{
				pclClient->pclCM->CR[siCharID]->clIdentity.clMofoMofuEvent.siCanAttack = pclInfo->m_siCanAttack;
			}
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MONSTER_ATTACK_START(cltMsg* pclMsg, SI32 id)
{
	SI32* pclInfo = (SI32*)pclMsg->cData;

	pclClient->pclMonsterAttackEvent->m_bEvent = true;
	pclClient->pclMonsterAttackEvent->SetEventVillageUnique(*pclInfo);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MONSTER_ATTACK_REWARD(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Event_MonsterAttack_Reward* pRcvInfo = (cltGameMsgResponse_Event_MonsterAttack_Reward*)pclMsg->cData;
	if ( NULL == pRcvInfo ) return;

	if ( m_pDialog[NMONSTER_ATTACK_EVENT] != NULL )
	{			
	DestroyInterface( m_pDialog[NMONSTER_ATTACK_EVENT] );
	}

	if( m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ] == NULL )
	CreateInterface( NNOTIFYGETEXPANDITEM_DLG );

	((CNNotifyGetExpAndItemDlg*)m_pDialog[NNOTIFYGETEXPANDITEM_DLG])->MonsterAttackReward( pRcvInfo->m_siRewardItemUnique, pRcvInfo->m_siRewardItemNum, pRcvInfo->m_siFameExp );
}