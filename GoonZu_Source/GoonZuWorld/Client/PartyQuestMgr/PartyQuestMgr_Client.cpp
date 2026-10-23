#include "PartyQuestMgr_Client.h"

#include "Char/CharManager/CharManager.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../../Client/Client.h"

#include "../../client/InterfaceMgr/InterfaceFile.h"
#include "../../client/InterfaceMgr/InterfaceMgr.h"


#include "../../client/ninterface/npartydlg/NPartyDlg.h"
#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"

extern cltCommonLogic* pclClient;

CPartyQuestMgr_Client::CPartyQuestMgr_Client()
{
	ZeroMemory(this, sizeof(CPartyQuestMgr_Client));
}

CPartyQuestMgr_Client::~CPartyQuestMgr_Client()
{
	
}


bool CPartyQuestMgr_Client::Initialize()
{
	bool bResult = false;
	
	CPartyQuestMgr_Common::LoadFiles_PartyQuest();

	return bResult;
}


// 파티퀘스트 시작 가능 여부 체크
bool CPartyQuestMgr_Client::DoCheckPartyQuest(SI32 siPartyUserNum, stClientPartyUser ClientPartyUser[])
{
	// 해당 파티원들이 파티퀘스트가 가능한지 체크 합니다.

	bool bResult = false;

	bResult = CheckAlreadyPartyQuest();				// 이미 퀘스트 중인가

	if(bResult == true)
		bResult = CheckPartyLeader();				// 파티장이 파티매칭을 이용해서 파티장이 되었는가?

	if(bResult == true)
		bResult = CheckUserNum(siPartyUserNum);		// 파티원들의 인원은 퀘스트 조건에 부합 하는가?

	if(bResult == true)
		bResult = CheckLevel(ClientPartyUser);		// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?

	return bResult;
}

bool CPartyQuestMgr_Client::CheckAlreadyPartyQuest()
{
	if(m_PartyQuestData.m_bGoingQuest == false)
		return true;
	else
		return true;

}
// 해당 파티의 파티장이 파티매칭 매니저에 등록 되어있는지 체크
bool CPartyQuestMgr_Client::CheckPartyLeader()
{
	cltClient* pClient = (cltClient*)pclClient;
	CNPartyDlg* pDlg = (CNPartyDlg*)pClient->m_pDialog[NPARTY_DLG];

	if ( NULL == pDlg ) return false;
	if ( false == pDlg->m_bIsPartyLeader ) return false;
	if ( false == pDlg->m_bIsPartyQuestParty) return false;
	if ( 0 > pDlg->m_siCategory ) return false;

	return true;
}

bool CPartyQuestMgr_Client::CheckUserNum(SI32 siPartyUserNum)
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( false == CheckPartyLeader() ) return false;

	CNPartyDlg* pDlg = (CNPartyDlg*)pClient->m_pDialog[NPARTY_DLG];
	if ( NULL == pDlg ) return false;

	// 카테고리 인덱스의 초기값이 -1인 이유 : 파티퀘스트 매니저가 리소스를 배열에 읽어들였기 때문에 0도 의미 있는 값이므로 초기값으로 사용하지 못함. 초기값인 -1로 인해 배열 범위 벗어나는 오류 발생하지 않도록 예외처리 꼭 해줘야 함.
	SI32 siCategoryIndex = pDlg->m_siCategory;

	if(siCategoryIndex < 0 || siCategoryIndex >= MAX_PARTYQUEST_CATEGORY_NUM)
		return false;

	PartyQuest_CatecoryData* pCategoryData	=	&m_PartyQuest_CategoryData[siCategoryIndex];

	if(siPartyUserNum < pCategoryData->m_siMinUser || siPartyUserNum > pCategoryData->m_siMaxUser)
		return false;

	return true;
}

bool CPartyQuestMgr_Client::CheckLevel(stClientPartyUser ClientPartyUser[])
{
	cltClient* pClient = (cltClient*)pclClient;

	if ( false == CheckPartyLeader() ) return false;

	CNPartyDlg* pDlg = (CNPartyDlg*)pClient->m_pDialog[NPARTY_DLG];
	if ( NULL == pDlg ) return false;

	SI32 siCategoryIndex = pDlg->m_siCategory;

	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[siCategoryIndex];

	for(SI32 siIndex = 0; siIndex < MAX_PARTY_USER_NUM; siIndex++)
	{
		SI32 siLevel = 0;

		if(ClientPartyUser[siIndex].bPartyStatus == true)
		{
			siLevel = ClientPartyUser[siIndex].siLevel;

			// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?
			if(siLevel < pCategoryData->m_siMinLevel || siLevel > pCategoryData->m_siMaxLevel)
				return false;
		}
	}

	return true;
}

// 파티퀘스트 데이터 설정
void CPartyQuestMgr_Client::StartPartyQuestData(PartyQuest_UnitData* PartyQuestData)
{
	if(PartyQuestData == NULL)
		return;

	// 파티퀘스트 데이터 설정
	CopyMemory(&m_PartyQuestData, PartyQuestData, sizeof(PartyQuest_UnitData));
	m_PartyQuestData.m_bGoingQuest = true;
	
	// 퀘스트 정보를 화면에 출력 합니다.
	UpdatePartyInfoMsg(PartyQuestData, m_PartyQuestData.m_siTimeLimit,false);
}

// 파티퀘스트 데이터 설정
void CPartyQuestMgr_Client::UpdatePartyQuestData(PartyQuest_UnitData* PartyQuestData, DWORD dwQuestTime, bool bClear)
{
	if(PartyQuestData == NULL)
		return;

	// 파티퀘스트 데이터 설정
	if(bClear == false)
	{
		CopyMemory(&m_PartyQuestData, PartyQuestData, sizeof(PartyQuest_UnitData));

		// 퀘스트 정보를 화면에 출력 합니다.
		UpdatePartyInfoMsg(PartyQuestData, dwQuestTime, bClear);
	}
	else
	{
		// 퀘스트 정보를 화면에 출력 합니다.
		UpdatePartyInfoMsg(&m_PartyQuestData, dwQuestTime, true);

		CopyMemory(&m_PartyQuestData, PartyQuestData, sizeof(PartyQuest_UnitData));

		UpdatePartyInfoMsg(PartyQuestData, dwQuestTime, false);
	}
}

void CPartyQuestMgr_Client::UpdatePartyInfoMsg(PartyQuest_UnitData* PartyQuestData, DWORD dwQuestTime, bool bClear)
{
	if(PartyQuestData == NULL)
		return;

	TCHAR szObject[MAX_KIND_NAME_LENGTH] = "";

	switch(PartyQuestData->m_siQuestType)
	{
	case PARTYQUEST_TYPE_HUNTING:
		{
			StringCchCopy(szObject, MAX_KIND_NAME_LENGTH, pclClient->pclKindInfoSet->pclKI[m_PartyQuestData.m_siCondition1]->GetName());

			TCHAR szQuestStep[256] = "";
			itoa(m_PartyQuestData.m_siQuestStep+1, szQuestStep, 10);

			if(bClear == true)
				m_PartyQuestData.m_siAmount = CPartyQuestMgr_Common::GetPartyQuestUnitData(m_PartyQuestData.m_siCategoryIndex, m_PartyQuestData.m_siMissionIndex, m_PartyQuestData.m_siQuestStep)->m_siAmount;

			TCHAR szCurAmount[256] = "";
			itoa(m_PartyQuestData.m_siAmount, szCurAmount, 10);

			TCHAR szMaxAmount[256] = "";
			itoa(CPartyQuestMgr_Common::GetPartyQuestUnitData(m_PartyQuestData.m_siCategoryIndex, m_PartyQuestData.m_siMissionIndex, m_PartyQuestData.m_siQuestStep)->m_siAmount, szMaxAmount, 10);

			TCHAR szTime[256] = "";
			itoa(dwQuestTime ,szTime, 10);

			NTCHARString256	szQuestInfo(GetTxtFromMgr(40790));
			szQuestInfo.Replace("#index#", szQuestStep);
			szQuestInfo.Replace("#monster#", szObject);
			szQuestInfo.Replace("#cur#", szCurAmount);
			szQuestInfo.Replace("#max#", szMaxAmount);
			szQuestInfo.Replace("#time#", szTime);

		
			if(bClear == true)
			{
				szQuestInfo+=GetTxtFromMgr(TEXT(6141)); // [완료]
				ZeroMemory(m_szQuestInfoForDlg, sizeof(m_szQuestInfoForDlg));
			}
			else
			{
				ZeroMemory(m_szQuestInfoForDlg, sizeof(m_szQuestInfoForDlg));
				StringCchCopy(m_szQuestInfoForDlg, sizeof(szQuestInfo), szQuestInfo);
			}

			pclClient->pclMessage->SetPartyQuestGameModeMsg(szQuestInfo, bClear);
		}
		break;

	case PARTYQUEST_TYPE_MAKEITEM:
		{
			StringCchCopy(szObject, MAX_ITEM_NAME_LENGTH, pclClient->pclItemManager->GetName(m_PartyQuestData.m_siCondition1));

			TCHAR szQuestStep[256] = "";
			itoa(m_PartyQuestData.m_siQuestStep+1, szQuestStep, 10);

			if(bClear == true)
				m_PartyQuestData.m_siAmount = CPartyQuestMgr_Common::GetPartyQuestUnitData(m_PartyQuestData.m_siCategoryIndex, m_PartyQuestData.m_siMissionIndex, m_PartyQuestData.m_siQuestStep)->m_siAmount;

			TCHAR szCurAmount[256] = "";
			itoa(m_PartyQuestData.m_siAmount, szCurAmount, 10);

			TCHAR szMaxAmount[256] = "";
			itoa(CPartyQuestMgr_Common::GetPartyQuestUnitData(m_PartyQuestData.m_siCategoryIndex, m_PartyQuestData.m_siMissionIndex, m_PartyQuestData.m_siQuestStep)->m_siAmount, szMaxAmount, 10);

			TCHAR szTime[256] = "";
			itoa(dwQuestTime, szTime, 10);

			NTCHARString256	szQuestInfo(GetTxtFromMgr(40799));
			szQuestInfo.Replace("#index#", szQuestStep);
			szQuestInfo.Replace("#item#", szObject);
			szQuestInfo.Replace("#cur#", szCurAmount);
			szQuestInfo.Replace("#max#", szMaxAmount);
			szQuestInfo.Replace("#time#", szTime);

			if(bClear == true)
			{
				szQuestInfo+=GetTxtFromMgr(TEXT(6141)); // [완료]
				ZeroMemory(m_szQuestInfoForDlg, sizeof(m_szQuestInfoForDlg));
			}
			else
			{
				ZeroMemory(m_szQuestInfoForDlg, sizeof(m_szQuestInfoForDlg));
				StringCchCopy(m_szQuestInfoForDlg, sizeof(szQuestInfo), szQuestInfo);
			}

			pclClient->pclMessage->SetPartyQuestGameModeMsg(szQuestInfo, bClear);
		}
		break;
	
	}
}

void CPartyQuestMgr_Client::DoGiveUpPartyQuest()
{
	ZeroMemory(&m_PartyQuestData, sizeof(PartyQuest_UnitData));
	ZeroMemory(m_szQuestInfoForDlg, sizeof(m_szQuestInfoForDlg));
}	

