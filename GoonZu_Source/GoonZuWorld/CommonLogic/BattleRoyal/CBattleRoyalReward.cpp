#include "./CBattleRoyalReward.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../Server/Server.h"
#include "../DBManager/GameDBManager_World/DBMsg-Item.h"

CBattleRoyalReward::CBattleRoyalReward()
{
	m_pclRewardItemToWinner = NULL;
	m_pclRewardToKillingUser = NULL;
}
CBattleRoyalReward::~CBattleRoyalReward()
{
	m_pclRewardItemToWinner = NULL;
	m_pclRewardToKillingUser = NULL;
}
// 생성
BOOL	CBattleRoyalReward::Create()
{
	CRewardItemByProbMgr* pclRewardItemByProbMgr = pclClient->m_pclRewardItemByProbMgr;
	if( NULL == pclRewardItemByProbMgr)
	{
		return FALSE;
	}

	m_pclRewardItemToWinner = pclRewardItemByProbMgr->GetRewardTypeItemByProbMgr(REWARDTYPE::BATTLEROYAL_WINNER);
	m_pclRewardToKillingUser = pclRewardItemByProbMgr->GetRewardTypeItemByProbMgr(REWARDTYPE::BATTLEROYAL_KILLPLAYER);

	if( NULL == m_pclRewardItemToWinner)
		return FALSE;
	if( NULL == m_pclRewardToKillingUser)
		return FALSE;

	return TRUE;
}
// 우승자에게 보상하기
BOOL	CBattleRoyalReward::GiveRewardToWinner(cltCharCommon* pclChar)
{
	if( NULL == m_pclRewardItemToWinner)
		return FALSE;
	if( NULL == pclChar)
		return FALSE;
	
	UI32 uiItemUnique = 0;
	UI32 uiItemNum = 0;

	// 로그 -  계정, 캐릭터 명, 레벨 
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		((cltServer*)pclClient)->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BATTLEROYAL, LOGCOMMAND_INDEX_BATTLEROYAL_WINNER,
			0, (cltCharServer*) pclChar, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, NULL, NULL);
	}

	if(FALSE == m_pclRewardItemToWinner->GetRandItemUnique(&uiItemUnique, &uiItemNum))
		return FALSE;

	if(FALSE == GiveReward(pclChar->GetCharID(), uiItemUnique, uiItemNum,INPUTITEMPERSON_OPTION_BATTLEROYAL_WINNER))
		return FALSE;

	return TRUE;
}
// 참가한 유저를 죽인 유저에게 보상하기 
BOOL	CBattleRoyalReward::GiveRewardToKillingUser(cltCharCommon* pclChar)
{
	if( NULL == m_pclRewardToKillingUser)
		return FALSE;
	if( NULL == pclChar)
		return FALSE;

	UI32 uiItemUnique = 0;
	UI32 uiItemNum = 0;

	if(FALSE == m_pclRewardToKillingUser->GetRandItemUnique(&uiItemUnique, &uiItemNum))
		return FALSE;

	if(FALSE == GiveReward(pclChar->GetCharID(), uiItemUnique, uiItemNum,INPUTITEMPERSON_OPTION_BATTLEROYAL_KILLINGUSER))
		return FALSE;

	return TRUE;
}

BOOL	CBattleRoyalReward::GiveReward( UI32 uiCharID, UI32 uiItemUnique, UI32 uiItemNum, SI32 siInputMode)
{
	cltItemInfo* pclItemInfo = pclClient->pclItemManager->FindItemInfo(uiItemUnique);
	if( NULL ==  pclItemInfo)
		return FALSE;

	if ( uiItemNum == 0)
		uiItemNum = 1;

	UI32 uiUseDate = pclItemInfo->clItem.uiDateUseDay;

	pclClient->pclCM->GiveItem(uiCharID, uiItemUnique, uiItemNum, GIVEITEM_MODE_PERSON, siInputMode, uiUseDate, 0 );

	return TRUE;
}