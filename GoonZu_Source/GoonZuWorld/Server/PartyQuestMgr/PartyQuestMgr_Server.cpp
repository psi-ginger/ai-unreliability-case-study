#include "PartyQuestMgr_Server.h"

#include "Char/CharManager/CharManager.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../../Client/Client.h"

#include "../../client/InterfaceMgr/InterfaceFile.h"
#include "../../client/InterfaceMgr/InterfaceMgr.h"
#include "../../common/CommonHeader.h"
#include "../../common/PartyMgr/PartyMgr.h"
#include "../../common/PartyMgr/PartyObj.h"
#include "../../Server/server.h"
#include "../../CommonLogic/MsgType-PartyQuest.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"


extern cltCommonLogic* pclClient;

CPartyQuestMgr_Server::CPartyQuestMgr_Server()
{
	ZeroMemory(this, sizeof(CPartyQuestMgr_Server));
}

CPartyQuestMgr_Server::~CPartyQuestMgr_Server()
{
	
}


bool CPartyQuestMgr_Server::Initialize()			//	매니저 초기화
{
	bool bResult = false;
	
	CPartyQuestMgr_Common::LoadFiles_PartyQuest();

	return bResult;
}

bool CPartyQuestMgr_Server::CheckSameArea(CPartyObj* pPartyObj)
{	
	cltServer* pServer = (cltServer*)pclClient;

	SI32 siLeaderID = pPartyObj->GetPartyLeaderUnique();
	
	if(pServer->pclCM->IsValidID(siLeaderID) == false)
		return false;

	for(SI32 i = 0; i < MAX_PARTY_USER_NUM; i++)
	{
		stPartyUser* pstPartyMember = pPartyObj->GetPartyUser(i);

		if((pstPartyMember == NULL) || (pstPartyMember->bPartyStatus == false))	
			continue;

		if(pclClient->pclCM->IsValidID(pstPartyMember->siCharUnique) == false)	
			continue;

		if(pclClient->pclCM->CR[pstPartyMember->siCharUnique]->pclCI->GetPersonID() != pstPartyMember->siPersonID)
			continue;

		if(pServer->pclCM->CR[siLeaderID]->GetMapIndex() != pServer->pclCM->CR[pstPartyMember->siCharUnique]->GetMapIndex())
			return false;
	}

	return true;
}

// 파티퀘스트 정보 갱신
void CPartyQuestMgr_Server::UpdatePartyQuest(SI32 siCharID, SI32 siPartyIndex, SI32 siQuestType, SI32 siCondition, SI32 siAmount)
{
	cltServer* pServer = (cltServer*)pclClient;

	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return;


    bool bResult = CheckSameArea(pPartyObject); // 같은 지역에 없다면 퀘스트 진행이 안됩니다.

	if(bResult == false)	return;
		

	cltCharServer* pclChar = pServer->pclCM->GetCharServer(siCharID);
	if( NULL == pclChar)
		return;

	SI32 siState = PARTYQUEST_UPDATE_STATE_NONE;
	
	bool bIncreaseCondition = false;
	bool bCurQuestClear		= false;

	bIncreaseCondition	= IncreasePartyQuestCondition(siPartyIndex, siQuestType, siCondition, siAmount);	// 퀘스트 Amount 증가

	if(bIncreaseCondition == true)
		bCurQuestClear	= ProcessCurrentQuestClear(siPartyIndex, siCharID);									// 현재 진행한 퀘스트가 클리어 되었는지 체크 합니다.
	
	if(bCurQuestClear == true)
		siState = PARTYQUEST_UPDATE_STATE_CLEAR;
	else if(bIncreaseCondition == true)
		siState = PARTYQUEST_UPDATE_STATE_INCREASE;


	// 갱신된 퀘스트 내용이 있다면 파티원들에게 알립니다.
	if(bIncreaseCondition == true)
	{
		DWORD dwQuestTime = (TABS(pPartyObject->m_dwEndTime) - TABS(pclClient->CurrentClock)) / 1000; // per second
		cltGameMsgResponse_PartyQuest_Update clInfo(bCurQuestClear ,&pPartyObject->m_PartyQuestInfo, dwQuestTime);
		cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_UPDATE, sizeof(clInfo), (BYTE*)&clInfo);
		pServer->pclCM->SendAllPartyMemberMsg((sPacketHeader*)&clMsg, pclChar->siPartyIndex);
	}

	return;
}

bool CPartyQuestMgr_Server::ProcessAllQuestClear(SI32 siPartyIndex)
{
	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return false;

	SI32 siCurStep = pPartyObject->m_PartyQuestInfo.m_siQuestStep;

	SI32 siCategoryIndex	= pPartyObject->m_PartyQuestInfo.m_siCategoryIndex;
	SI32 siMissionIndex		= pPartyObject->m_PartyQuestInfo.m_siMissionIndex;

	// 
	if(CPartyQuestMgr_Common::GetPartyQuestUnitData(siCategoryIndex, siMissionIndex, siCurStep+1)->m_siQuestType == PARTYQUEST_TYPE_CLEAR)
	{
		// 해당 파티가 소유하고 있는 파티퀘스트 정보를 초기화 합니다.
		ZeroMemory(&pPartyObject->m_PartyQuestInfo, sizeof(PartyQuest_UnitData));
		return true;
	}
	
	return false;
}

bool CPartyQuestMgr_Server::ProcessCurrentQuestClear(SI32 siPartyIndex, SI32 siCharID)
{
	// 현재 진행중인 퀘스트가 클리어 되었는지 확인합니다.

	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return false;


	SI32 siCategoryIndex	= pPartyObject->m_PartyQuestInfo.m_siCategoryIndex;
	SI32 siMissionIndex		= pPartyObject->m_PartyQuestInfo.m_siMissionIndex;
	SI32 siStep				= pPartyObject->m_PartyQuestInfo.m_siQuestStep;
	
	switch(pPartyObject->m_PartyQuestInfo.m_siQuestType)
	{
	case PARTYQUEST_TYPE_NONE:
		break;

	case PARTYQUEST_TYPE_HUNTING:
		{
			SI32 siCurAmount = pPartyObject->m_PartyQuestInfo.m_siAmount;										// 현재 사냥한 몬스터 수
			SI32 siMaxAmount = GetPartyQuestUnitData(siCategoryIndex, siMissionIndex, siStep)->m_siAmount;		// 목표치 몬스터 수

			// 현재 퀘스트를 클리어 했다면 
			if(siCurAmount >= siMaxAmount)
			{
				// 퀘스트 보상
				ProcessReward(siPartyIndex, siCharID);

				// 퀘스트를 다음 스텝의 퀘스트로 갱신 합니다.					
				if(ProcessAllQuestClear(siPartyIndex) == false) 
				{
					SI32 siCurStep = pPartyObject->m_PartyQuestInfo.m_siQuestStep;
					CPartyQuestMgr_Common::SetPartyQuestData(&pPartyObject->m_PartyQuestInfo, siCategoryIndex, siMissionIndex, siCurStep+1);

					pPartyObject->m_dwEndTime = pclClient->CurrentClock + (pPartyObject->m_PartyQuestInfo.m_siTimeLimit*1000);
					pPartyObject->m_dwElapseTime = pclClient->CurrentClock;
				}

				return true;
			}
		}
		break;

	case PARTYQUEST_TYPE_MAKEITEM:
		{
			SI32 siCurAmount = pPartyObject->m_PartyQuestInfo.m_siAmount;										// 현재 제조한 아이템 개수
			SI32 siMaxAmount = GetPartyQuestUnitData(siCategoryIndex, siMissionIndex, siStep)->m_siAmount;		// 총 제조해야할 아이템 개수

			// 현재 퀘스트를 클리어 했다면 
			if(siCurAmount >= siMaxAmount)
			{
				// 퀘스트 보상
				ProcessReward(siPartyIndex, siCharID);

				// 퀘스트를 다음 스텝의 퀘스트로 갱신 합니다.					
				if(ProcessAllQuestClear(siPartyIndex) == false) 
				{
					SI32 siCurStep = pPartyObject->m_PartyQuestInfo.m_siQuestStep;
					CPartyQuestMgr_Common::SetPartyQuestData(&pPartyObject->m_PartyQuestInfo, siCategoryIndex, siMissionIndex, siCurStep+1);

					pPartyObject->m_dwEndTime = pclClient->CurrentClock + (pPartyObject->m_PartyQuestInfo.m_siTimeLimit*1000);
					pPartyObject->m_dwElapseTime = pclClient->CurrentClock;
				}

				return true;
			}
		}
		break;

	default:
		break;
	}

	return false;
}

void CPartyQuestMgr_Server::ProcessReward(SI32 siPartyIndex, SI32 siCharID)
{
	cltServer* pServer = (cltServer*)pclClient;

	cltCharServer* pclChar = pServer->pclCM->GetCharServer(siCharID);
	if ( NULL == pclChar )
		return;
	
	// 현재 클리어한 퀘스트에 대한 보상을 해줍니다.
	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return;

	 
	// 파티원들중 랜덤으로 한명에게 보상아이템을 지급합니다.

	SI32 siItemUnique = 0;
	SI32 siItemAmount = 0;
	SI32 siItemUseDay = 0;

	SI32 RewardUserArray[MAX_PARTY_USER_NUM];
	ZeroMemory(RewardUserArray, sizeof(RewardUserArray));
	SI32 siCount = 0;

	// 파티내 인덱스중 유효한 파티원만 추려낸다.
	for(SI32 siPlayerIndex = 0; siPlayerIndex < MAX_PARTY_USER_NUM; siPlayerIndex++)
	{
		stPartyUser* pUser = pServer->pclPartyMgr->GetPartyObj(pclChar->siPartyIndex)->GetPartyUser(siPlayerIndex);
		
		if(pUser == NULL || pServer->pclCM->IsValidID(pUser->siCharUnique) == false)
			continue;

		RewardUserArray[siCount] = pUser->siCharUnique;
		siCount++;
	}

	if(siCount <= 0)
		return;

	// // 유효한 파티원들에게 보상경험치를 적용시킵니다.
	for(SI32 siRewardEXPIndex = 0; siRewardEXPIndex < siCount; siRewardEXPIndex++)
	{
		cltCharServer* pclRewardEXPChar = pServer->pclCM->GetCharServer(RewardUserArray[siRewardEXPIndex]);
		if ( NULL == pclRewardEXPChar )
			continue;

		pclRewardEXPChar->IncreaseExp((SI64)pPartyObject->m_PartyQuestInfo.m_siRewardEXP);
	}

	SI32 siRewardCharUnique = RewardUserArray[rand()%siCount];
	
	cltCharServer* pclRewardChar = pServer->pclCM->GetCharServer(siRewardCharUnique);
	if ( NULL == pclRewardChar )
		return;

	// 보상 아이템 지급
	for(SI32 i = 0; i < MAX_PARTYQUEST_REWARD_NUM; i++)
	{
		siItemUnique = pPartyObject->m_PartyQuestInfo.m_siRewardItemUnique[i];
		siItemAmount = pPartyObject->m_PartyQuestInfo.m_siRewardItemAmount[i];
		siItemUseDay = pPartyObject->m_PartyQuestInfo.m_siRewardItemUseDay[i];

		SI32 ref = pServer->pclItemManager->FindItemRefFromUnique(siItemUnique);
		
		if (ref <= 0)	
			continue;

		if(siItemAmount <= 0)
			continue;

		// 보상아이템을 받을 파티원을 랜덤으로 추출.

		cltItem			clitem;
		ZeroMemory(&clitem, sizeof(clitem));
		clitem.siUnique  = siItemUnique;		
		clitem.siItemNum =	  siItemAmount;

		if(siItemUseDay <= 0)
			clitem.uiDateUseDay = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
		else
			clitem.uiDateUseDay = siItemUseDay;


		pServer->InputItem(pclRewardChar, &clitem, INPUTITEMPERSON_OPTION_PARTYQUEST);
	}
}

bool CPartyQuestMgr_Server::IncreasePartyQuestCondition(SI32 siPartyIndex, SI32 siQuestType, SI32 siCondition, SI32 siAmount)
{
	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return false;

	// 현재 진행중인 퀘스트 타입인지 확인 합니다.
	if(pPartyObject->m_PartyQuestInfo.m_siQuestType != siQuestType)
		return false;

	switch(siQuestType)
	{
	case PARTYQUEST_TYPE_NONE:
		break;

	case PARTYQUEST_TYPE_HUNTING:
		{
			if(pPartyObject->m_PartyQuestInfo.m_siCondition1 == siCondition)	// 수행하고 있는 사냥퀘스트의 MonsterKind와 사냥한 MonsterKind가 같은지 체크 합니다.
			{
				pPartyObject->m_PartyQuestInfo.m_siAmount += siAmount;			// 해당 퀘스트의 몬스터를 사냥했다면 Amount를 증가 시켜줍니다.
				return true;
			}
		}
		break;

	case PARTYQUEST_TYPE_MAKEITEM:
		{
			if(pPartyObject->m_PartyQuestInfo.m_siCondition1 == siCondition)	// 퀘스트중인 제조아이템이랑 현재 제조한 아이템이 같은아이템인지 체크 합니다.
			{
				pPartyObject->m_PartyQuestInfo.m_siAmount += siAmount;			// 진행중인 퀘스트아이템을 제조했다면 카운트를 증가 시킵니다.
				return true;
			}
		}
		break;

	default:
		break;
	}	

	// 업데이트 된 퀘스트가 없다면 return false
	return false;
}


// 파티퀘스트 시작 가능 여부 체크
bool CPartyQuestMgr_Server::DoCheckPartyQuest(CPartyObj* pPartyObject)
{
	// 해당 파티원들이 파티퀘스트가 가능한지 체크 합니다.

	bool bResult = false;

	bResult = CheckAlreadyPartyQuest(pPartyObject);	// 이미 파티 퀘스트중인가

	if(bResult == true)
		bResult = CheckPartyLeader(pPartyObject);		// 파티장이 파티매칭을 이용해서 파티장이 되었는가?

	if(bResult == true)
		bResult = CheckUserNum(pPartyObject);	// 파티원들의 인원은 퀘스트 조건에 부합 하는가?

	if(bResult == true)
		bResult = CheckLevel(pPartyObject);		// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?

	if(bResult == true)
		bResult = CheckPCRoom(pPartyObject);	// 피시방 퀘스트 전용 체크

	return bResult;
}

bool CPartyQuestMgr_Server::CheckAlreadyPartyQuest(CPartyObj* pPartyObject)
{
	if(pPartyObject == NULL)
		return false;

	return !(pPartyObject->m_PartyQuestInfo.m_bGoingQuest);
}

// 해당 파티의 파티장이 파티매칭 매니저에 등록 되어있는지 체크
bool CPartyQuestMgr_Server::CheckPartyLeader(CPartyObj* pPartyObject)
{
	SI32 siResult = -1;

	siResult = pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());

	if(siResult < 0)
		return false;

	return true;
}

bool CPartyQuestMgr_Server::CheckUserNum(CPartyObj* pPartyObject)
{
	SI32 siCategoryIndex = pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());

	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[siCategoryIndex];

	if(pPartyObject->GetPartyUserNum() < pCategoryData->m_siMinUser || pPartyObject->GetPartyUserNum() > pCategoryData->m_siMaxUser)
		return false;

	return true;
}

bool CPartyQuestMgr_Server::CheckLevel(CPartyObj* pPartyObject)
{
	SI32 siCategoryIndex = pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());
	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[siCategoryIndex];

	for(SI32 siIndex = 0; siIndex < MAX_PARTY_USER_NUM; siIndex++)
	{
		SI32 siCharUnique = pPartyObject->GetPartyUser(siIndex)->siCharUnique;

		if ( pclClient->pclCM->IsValidID(siCharUnique) == false )
			continue;

		cltCharCommon* pCharacter = pclClient->pclCM->GetChar(siCharUnique);
		SI32 siLevel = pCharacter->pclCI->clIP.GetLevel();

		// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?
		if(siLevel < pCategoryData->m_siMinLevel || siLevel > pCategoryData->m_siMaxLevel)
			return false;
	}

	return true;
}

bool CPartyQuestMgr_Server::CheckPCRoom(CPartyObj* pPartyObject)
{
	SI32 siCategoryIndex = pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());

	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[siCategoryIndex];

	for(SI32 siIndex = 0; siIndex < MAX_PARTY_USER_NUM; siIndex++)
	{
		SI32 siCharUnique = pPartyObject->GetPartyUser(siIndex)->siCharUnique;

		if ( pclClient->pclCM->IsValidID(siCharUnique) == false )
			continue;

		cltCharCommon* pCharacter = pclClient->pclCM->GetChar(siCharUnique);

		// 하려는 퀘스트가 PC방 전용 퀘스트 인가?
		if(pCategoryData->m_siKind == PARTYQUEST_KIND_PCROOM )
		{
			if(pCharacter->pclCI->clBI.bPCRoom == false)
				return false;
		}
	}

	return true;
}


// 파티퀘스트 포기시 
bool CPartyQuestMgr_Server::DoGiveUpPartyQuest(SI32 siPartyIndex, SI32 siLeaveCharID, SI32 siMessage)
{
	cltServer* pServer = (cltServer*)pclClient;

	CPartyObj* pPartyObject = pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
	if(pPartyObject == NULL)
		return false;	

	PartyQuest_UnitData* pQuestData = &pPartyObject->m_PartyQuestInfo;

	if(pQuestData == NULL || pQuestData->m_bGoingQuest == false)
		return false;

	ZeroMemory(pQuestData, sizeof(PartyQuest_UnitData));

	pPartyObject->m_dwStartTime	 = 0;
	pPartyObject->m_dwElapseTime = 0;
	pPartyObject->m_dwEndTime	 = 0;

	cltGameMsgResponse_PartyQuest_GiveUp clInfo(siMessage);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_GIVEUP, sizeof(clInfo), (BYTE*)&clInfo);
	pServer->pclCM->SendAllPartyMemberMsg((sPacketHeader*)&clMsg, siPartyIndex);		



	cltCharServer* pclChar = pServer->pclCM->GetCharServer( siLeaveCharID );
	if ( NULL == pclChar )
		return false;

	cltGameMsgResponse_PartyQuest_GiveUp clInfo2(siMessage);
	cltMsg clMsg2(GAMEMSG_RESPONSE_PARTY_QUEST_GIVEUP, sizeof(clInfo2), (BYTE*)&clInfo2);
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg2);

	return true;
}

void CPartyQuestMgr_Server::UpdatePartyQuestTime(SI32 siPartyIndex,  PartyQuest_UnitData* pPartyQuestData)
{
	cltServer* pServer = (cltServer*)pclClient;

	if(pPartyQuestData == NULL)
		return;

	if(siPartyIndex <= 0)
		return;


	CPartyObj* pPartyObject = pServer->pclPartyMgr->GetPartyObj(siPartyIndex);
	
	if(pPartyObject == NULL)
		return;
	

	DWORD dwQuestTime = (TABS(pPartyObject->m_dwEndTime) - TABS(pclClient->CurrentClock)) / 1000; // per second
	cltGameMsgResponse_PartyQuest_Update clInfo(false, pPartyQuestData, dwQuestTime);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_QUEST_UPDATE, sizeof(clInfo), (BYTE*)&clInfo);
	pServer->pclCM->SendAllPartyMemberMsg((sPacketHeader*)&clMsg, siPartyIndex);	
}




