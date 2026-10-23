#include "PartyQuestMgr_Common.h"

#include "Char/CharManager/CharManager.h"
#include "../CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../../Client/Client.h"

#include "../../client/InterfaceMgr/InterfaceFile.h"
#include "../../client/InterfaceMgr/InterfaceMgr.h"
#include "../../common/CommonHeader.h"
#include "../../common/PartyMgr/PartyObj.h"
#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"
#include "../../Client/ninterface/npartydlg/NPartyDlg.h"


extern cltCommonLogic* pclClient;

CPartyQuestMgr_Common::CPartyQuestMgr_Common()
{
	ZeroMemory(this, sizeof(CPartyQuestMgr_Common));
}

CPartyQuestMgr_Common::~CPartyQuestMgr_Common()
{
	
}


bool CPartyQuestMgr_Common::Initialize()			//	매니저 초기화
{
	//	매니저 초기화 작업.. 파일로드 등등
	bool bResult = false;
	
	bResult = LoadFiles_PartyQuest();	// 파티퀘스트 카테고리 파일 로드

	return bResult;
}

bool CPartyQuestMgr_Common::LoadFiles_PartyQuest()
{
	NDataLoader	dataloader;

	if(false == dataloader.LoadDataFromFile(TEXT("quest/PartyQuest/PartyQuest_Category.txt")))
		return false;
	
	SI32	siIndex			= 0;
	TCHAR	szDestTitle[64]	= TEXT("");	
	TCHAR	szKind[64]		= TEXT("");
	SI32	siMinLevel		= 0;
	SI32	siMaxLevel		= 0;
	SI32	siMinUser		= 0;
	SI32	siMaxUser		= 0;
	SI32	siMissionCount	= 0;

	TCHAR	szSourceTitle[MAX_PARTYQUEST_CATEGORY_NUM][64];
	ZeroMemory(szSourceTitle, MAX_PARTYQUEST_CATEGORY_NUM * 64);

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,			4,
		NDATA_MBSTRING,	szDestTitle,		64,
		NDATA_MBSTRING,	szKind,				64,
		NDATA_INT32,	&siMinLevel,		4,
		NDATA_INT32,	&siMaxLevel,		4,
		NDATA_INT32,	&siMinUser,			4,
		NDATA_INT32,	&siMaxUser,			4,
		NDATA_INT32,	&siMissionCount,	4,
		0,				0,					0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 siCategoryNum = 0;	// 카테고리 퀘스트파일 내의 총 퀘스트 수
	

	// 카테고리 파일 로드
	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if(siCategoryNum >= MAX_PARTYQUEST_CATEGORY_NUM)
				continue;

			m_PartyQuest_CategoryData[siCategoryNum].m_siIndex			=	siIndex;
			StringCchCopy(szSourceTitle[siCategoryNum],	64, szDestTitle);
			m_PartyQuest_CategoryData[siCategoryNum].m_siKind			= QuestKindToInteger(szKind);
			m_PartyQuest_CategoryData[siCategoryNum].m_siMinLevel		= siMinLevel;
			m_PartyQuest_CategoryData[siCategoryNum].m_siMaxLevel		= siMaxLevel;
			m_PartyQuest_CategoryData[siCategoryNum].m_siMinUser			= siMinUser;
			m_PartyQuest_CategoryData[siCategoryNum].m_siMaxUser			= siMaxUser;
			m_PartyQuest_CategoryData[siCategoryNum].m_siMissionCount	= siMissionCount;

			siCategoryNum++;
		}
	}


	TCHAR	szQuestFileName[1024] = TEXT("");

	SI32	siQuestStep			= 0;
	TCHAR	szQuestType[64]		= TEXT("");
	TCHAR	szCondition1[64]	= TEXT("");
	TCHAR	szCondition2[64]	= TEXT("");
	SI32	siAmount			= 0;
	SI32	siRewardEXP			= 0;
	SI32	siRewardItemUnique[MAX_PARTYQUEST_REWARD_NUM];
	SI32	siReawrdItemAmount[MAX_PARTYQUEST_REWARD_NUM];
	SI32	siRewardItemUseDay[MAX_PARTYQUEST_REWARD_NUM];

	ZeroMemory(siRewardItemUnique, sizeof(siRewardItemUnique));
	ZeroMemory(siReawrdItemAmount, sizeof(siReawrdItemAmount));
	ZeroMemory(siRewardItemUseDay, sizeof(siRewardItemUseDay));

	SI32	siTimeLimit			= 0;

	NDataTypeInfo pDataTypeInfo2[] =
	{
		NDATA_INT32,	&siQuestStep,			4,
		NDATA_MBSTRING,	szQuestType,			64,
		NDATA_MBSTRING,	szCondition1,			64,
		NDATA_MBSTRING,	szCondition2,			64,
		NDATA_INT32,	&siAmount,				4,
		NDATA_INT32,	&siRewardEXP,			4,
		NDATA_INT32,	&siRewardItemUnique[0],	4,
		NDATA_INT32,	&siReawrdItemAmount[0],	4,
		NDATA_INT32,	&siRewardItemUseDay[0],	4,
		NDATA_INT32,	&siRewardItemUnique[1],	4,
		NDATA_INT32,	&siReawrdItemAmount[1],	4,
		NDATA_INT32,	&siRewardItemUseDay[1],	4,
		NDATA_INT32,	&siRewardItemUnique[2],	4,
		NDATA_INT32,	&siReawrdItemAmount[2],	4,
		NDATA_INT32,	&siRewardItemUseDay[2],	4,
		NDATA_INT32,	&siRewardItemUnique[3],	4,
		NDATA_INT32,	&siReawrdItemAmount[3],	4,
		NDATA_INT32,	&siRewardItemUseDay[3],	4,
		NDATA_INT32,	&siRewardItemUnique[4],	4,
		NDATA_INT32,	&siReawrdItemAmount[4],	4,
		NDATA_INT32,	&siRewardItemUseDay[4],	4,
		NDATA_INT32,	&siTimeLimit,			4,
		0,				0,						0
	};

	dataloader.RegDataType( pDataTypeInfo2 );

	//  카테고리 수에 따라 하위 폴더내의 파일 로드
	for(SI32 siIndex = 0; siIndex < siCategoryNum; siIndex++)
	{
		for(SI32 siMissionCount = 0; siMissionCount < m_PartyQuest_CategoryData[siIndex].m_siMissionCount; siMissionCount++)
		{
			if(siMissionCount >= MAX_PARTYQUEST_MISSION_NUM)
				continue;

			sprintf(szQuestFileName, TEXT("quest/PartyQuest/%s/%s_%01d.txt"), szSourceTitle[siIndex], szSourceTitle[siIndex], siMissionCount+1);
			
			if(false == dataloader.LoadDataFromFile(szQuestFileName))
				return false;


			SI32 siStepNum = 0;

			while( !dataloader.IsEndOfData() ) 
			{
				if (dataloader.ReadData())	
				{
					// 이곳에서 단위 퀘스트 파일을 로드 합니다.

					if(siStepNum >= MAX_PARTYQUEST_STEP_NUM)
						continue;

					PartyQuest_UnitData* pData = NULL;
					pData = &(m_PartyQuest_CategoryData[siIndex].m_PartyQuest_UnitData[siMissionCount][siStepNum]);

					if(pData == NULL)
						continue;

					pData->m_siCategoryIndex	= siIndex;
					pData->m_siMissionIndex		= siMissionCount;
					pData->m_siQuestStep = siQuestStep;
					pData->m_siQuestType = QuestTypeToInteger(szQuestType);

					if(pData->m_siQuestType == PARTYQUEST_TYPE_HUNTING)
						pData->m_siCondition1 = QuestCondition1ToInteger(szCondition1);
					else if(pData->m_siQuestType == PARTYQUEST_TYPE_MAKEITEM)
						pData->m_siCondition1 = atoi(szCondition1);

					pData->m_siCondition2 = QuestCondition2ToInteger(szCondition2);
					pData->m_siAmount = siAmount;

					pData->m_siRewardEXP = siRewardEXP;

					for(SI32 siReWardNum = 0; siReWardNum < MAX_PARTYQUEST_REWARD_NUM; siReWardNum++)
					{
						pData->m_siRewardItemUnique[siReWardNum] = siRewardItemUnique[siReWardNum];
						pData->m_siRewardItemAmount[siReWardNum] = siReawrdItemAmount[siReWardNum];
						pData->m_siRewardItemUseDay[siReWardNum] = siRewardItemUseDay[siReWardNum];
					}

					pData->m_siTimeLimit= siTimeLimit*60; // Per Second

					siStepNum++;
				}
			}
		}
	}

	return true;	
}


SI32 CPartyQuestMgr_Common::QuestKindToInteger(TCHAR* szQuestKindToInteger)
{
	if ( NULL == szQuestKindToInteger )
		return PARTYQUEST_KIND_NONE;

	COMPARE_TEXT_WITH_VALUE( szQuestKindToInteger, PARTYQUEST_KIND_NORMAL  );
	COMPARE_TEXT_WITH_VALUE( szQuestKindToInteger, PARTYQUEST_KIND_PCROOM  );
	
#ifdef _DEBUG	 
		MessageBox(NULL, "Cannot Fonud Type", "Error", MB_ICONWARNING | MB_OK);		
#endif

	return PARTYQUEST_KIND_NONE;
}

SI32 CPartyQuestMgr_Common::QuestTypeToInteger(TCHAR* szQuestType)
{
	if (NULL == szQuestType)
		return PARTYQUEST_TYPE_NONE;

	COMPARE_TEXT_WITH_VALUE(szQuestType, PARTYQUEST_TYPE_HUNTING);
	COMPARE_TEXT_WITH_VALUE(szQuestType, PARTYQUEST_TYPE_MAKEITEM);
	COMPARE_TEXT_WITH_VALUE(szQuestType, PARTYQUEST_TYPE_CLEAR);
	
#ifdef _DEBUG	 
		MessageBox(NULL, "Cannot Fonud Type", "Error", MB_ICONWARNING | MB_OK);		
#endif

	return PARTYQUEST_TYPE_NONE;
}


SI32 CPartyQuestMgr_Common::QuestCondition1ToInteger(TCHAR* szQuestCondition1)
{
	if (NULL == szQuestCondition1)
		return 0;

	return pclClient->GetUniqueFromHash(TEXT(szQuestCondition1));
}

SI32 CPartyQuestMgr_Common::QuestCondition2ToInteger(TCHAR* szQuestCondition2)
{
	return 0;
}


PartyQuest_CatecoryData* CPartyQuestMgr_Common::GetPartyQuestCategoryData(SI32 siIndex)
{
	if(siIndex < 0 || siIndex >= MAX_PARTYQUEST_CATEGORY_NUM)
		return NULL;

	return &m_PartyQuest_CategoryData[siIndex];
}

PartyQuest_UnitData* CPartyQuestMgr_Common::GetPartyQuestUnitData(SI32 siCategoryIndex, SI32 siMissionIndex, SI32 siStepIndex)
{
	if(	siCategoryIndex < 0 || 
		siMissionIndex < 0 || 
		siCategoryIndex >=MAX_PARTYQUEST_CATEGORY_NUM || 
		siMissionIndex >= MAX_PARTYQUEST_MISSION_NUM)
		return NULL;

	return &(m_PartyQuest_CategoryData[siCategoryIndex].m_PartyQuest_UnitData[siMissionIndex][siStepIndex]);
}

PartyQuest_UnitData* CPartyQuestMgr_Common::GetPartyQuestUnitData(SI32 siCategoryIndex, SI32 siMissionIndex, SI32 siStepIndex, TCHAR* szTitle)
{
	if(	siCategoryIndex < 0 || 
		siMissionIndex < 0 || 
		siCategoryIndex >=MAX_PARTYQUEST_CATEGORY_NUM || 
		siMissionIndex >= MAX_PARTYQUEST_MISSION_NUM)
		return NULL;


	PartyQuest_UnitData* pQuestData = &(m_PartyQuest_CategoryData[siCategoryIndex].m_PartyQuest_UnitData[siMissionIndex][siStepIndex]);
	SI32 siQuestType = pQuestData->m_siQuestType;
		
	if(siQuestType == PARTYQUEST_TYPE_HUNTING)
	{
		// 사냥인경우 condition1의 값은 monster kind 이다.
		StringCchCopy(szTitle, MAX_KIND_NAME_LENGTH, pclClient->pclKindInfoSet->pclKI[pQuestData->m_siCondition1]->GetName());
		
	}
	else if(siQuestType == PARTYQUEST_TYPE_MAKEITEM)
	{
		//	제조인경우 condition1의 값은 item unique 이다.
		StringCchCopy(szTitle, MAX_ITEM_NAME_LENGTH, pclClient->pclItemManager->GetName(pQuestData->m_siCondition1));
	}

	return pQuestData;
}


// 파티퀘스트 시작 가능 여부 체크
bool CPartyQuestMgr_Common::DoCheckPartyQuest(CPartyObj* pPartyObject)
{
	// 해당 파티원들이 파티퀘스트가 가능한지 체크 합니다.

	bool bResult = false;
	
	bResult = CheckAlreadyPartyQuest(pPartyObject);	// 이미 파티 퀘스트중인가

	if(bResult == false)
		CheckPartyLeader(pPartyObject);				// 파티매칭을 통해 파티장이 되었는가.
	
	if(bResult == true)
		bResult = CheckUserNum(pPartyObject);	// 파티원들의 인원은 퀘스트 조건에 부합 하는가?

	if(bResult == true)
		bResult = CheckLevel(pPartyObject);		// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?

	if(bResult == true)
		bResult = CheckPCRoom(pPartyObject);	// 피시방 퀘스트 전용 체크

	return bResult;
}

bool CPartyQuestMgr_Common::CheckAlreadyPartyQuest(CPartyObj* pPartyObject)
{
	if(pPartyObject == NULL)
		return true;
	
	return pPartyObject->m_PartyQuestInfo.m_bGoingQuest;
}

// 해당 파티의 파티장이 파티매칭 매니저에 등록 되어있는지 체크
bool CPartyQuestMgr_Common::CheckPartyLeader(CPartyObj* pPartyObject)
{
	PartyQuest_UnitData* pPartyQuestData = &(pPartyObject->m_PartyQuestInfo);

	SI32 siResult = -1;

	if (pclClient->GameMode == GAMEMODE_SERVER)
	{
		siResult = pclClient->m_pServer_PartyMatchingMgr->Get_Category(pPartyObject->GetPartyLeaderUnique());

		if(siResult == -1)
			return false;
	}
	else if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		cltClient* pClient = (cltClient*)pclClient;
		CNPartyDlg* pDlg = (CNPartyDlg*)pClient->m_pDialog[NPARTY_DLG];

		// 파티 리더이고, 파티퀘스트 파티고, 카테고리 값이 유효하면 true 리턴 셋중 하나라도 아니면 false 리턴
		return (pDlg->m_bIsPartyLeader && pDlg->m_bIsPartyQuestParty && 0 <= pDlg->m_siCategory);
	}

	return true;
}

bool CPartyQuestMgr_Common::CheckUserNum(CPartyObj* pPartyObject)
{
	PartyQuest_UnitData*		pPartyQuestData =	&(pPartyObject->m_PartyQuestInfo);
	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[pPartyQuestData->m_siCategoryIndex];

	if(pPartyObject->GetPartyUserNum() < pCategoryData->m_siMinUser || pPartyObject->GetPartyUserNum() > pCategoryData->m_siMaxUser)
		return false;

	return true;
}

bool CPartyQuestMgr_Common::CheckLevel(CPartyObj* pPartyObject)
{
	PartyQuest_UnitData*		pPartyQuestData =	&(pPartyObject->m_PartyQuestInfo);
	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[pPartyQuestData->m_siCategoryIndex];

	for(SI32 siIndex = 0; siIndex < MAX_PARTY_USER_NUM; siIndex++)
	{
		SI32 siCharUnique = pPartyObject->GetPartyUser(siIndex)->siCharUnique;

		if ( pclClient->pclCM->IsValidID(siCharUnique) == false )
			continue;

		if (pclClient->GameMode == GAMEMODE_SERVER)
		{
			cltCharCommon* pCharacter = pclClient->pclCM->GetChar(siCharUnique);
			SI32 siLevel = pCharacter->pclCI->clIP.GetLevel();

			// 파티원들의 레벨은 퀘스트 조건에 부합 하는가?
			if(siLevel < pCategoryData->m_siMinLevel || siLevel > pCategoryData->m_siMaxLevel)
				return false;
		}
	}

	return true;
}

bool CPartyQuestMgr_Common::CheckPCRoom(CPartyObj* pPartyObject)
{
	PartyQuest_UnitData*		pPartyQuestData =	&(pPartyObject->m_PartyQuestInfo);
	PartyQuest_CatecoryData*	pCategoryData	=	&m_PartyQuest_CategoryData[pPartyQuestData->m_siCategoryIndex];

	for(SI32 siIndex = 0; siIndex < MAX_PARTY_USER_NUM; siIndex++)
	{
		SI32 siCharUnique = pPartyObject->GetPartyUser(siIndex)->siCharUnique;

		if ( pclClient->pclCM->IsValidID(siCharUnique) == false )
			continue;

		if (pclClient->GameMode == GAMEMODE_SERVER)
		{
			cltCharCommon* pCharacter = pclClient->pclCM->GetChar(siCharUnique);

			// 하려는 퀘스트가 PC방 전용 퀘스트 인가?
			if(pCategoryData->m_siKind == PARTYQUEST_KIND_PCROOM )
			{
				if(pCharacter->pclCI->clBI.bPCRoom == false)
					return false;
			}
		}
	}

	return true;
}

// 파티퀘스트 시작 요청시 파티퀘스트 정보를 설정해줍니다.
bool CPartyQuestMgr_Common::SetPartyQuestData(PartyQuest_UnitData* pQuestData, SI32 siCategoryIndex, SI32 siMissionindex, SI32 siStep)
{
	if(siCategoryIndex < 0 || siCategoryIndex >= MAX_PARTYQUEST_CATEGORY_NUM)
		return false;

	if(siMissionindex < 0 || siMissionindex >= MAX_PARTYQUEST_MISSION_NUM)
		return false;

	CopyMemory(pQuestData, &(m_PartyQuest_CategoryData[siCategoryIndex].m_PartyQuest_UnitData[siMissionindex][siStep]), sizeof(PartyQuest_UnitData));

	// 파티퀘스트 데이터 초기값으로 설정
	pQuestData->m_bGoingQuest	= true;
	pQuestData->m_siAmount		= 0;

	return true;
}

