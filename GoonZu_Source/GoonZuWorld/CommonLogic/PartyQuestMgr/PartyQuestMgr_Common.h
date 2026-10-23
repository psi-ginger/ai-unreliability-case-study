#pragma once

#include <Directives.h>
#include "../../Common/CommonHeader.h"

class CPartyObj;

//////////////////////////////////////////////////////////////////////////
//	파티 퀘스트 매니저
//	클라이언트와 서버에서 공용으로 쓰임.
//////////////////////////////////////////////////////////////////////////

const SI32 MAX_PARTYQUEST_CATEGORY_NUM	= 10;	//	파티퀘스트 카테고리 파일내의 총 갯수
const SI32 MAX_PARTYQUEST_MISSION_NUM	= 10;	//	파티퀘스트 미션의 총 갯수
const SI32 MAX_PARTYQUEST_STEP_NUM		= 5;	//	파티퀘스트 STEP의 총 갯수
const SI32 MAX_PARTYQUEST_REWARD_NUM	= 5;	//	파티퀘스트 보상아이템의 총 갯수

//////////////////////////////////////////////////////////////////////////
//	Data Files
//////////////////////////////////////////////////////////////////////////

// 파티퀘스트의 단위 데이터
struct PartyQuest_UnitData
{
	// 파티퀘스트 데이터 관련 멤버 변수들	
	bool	m_bGoingQuest;
	SI32	m_siCategoryIndex;
	SI32	m_siMissionIndex;
	SI32	m_siQuestStep;
	SI32	m_siQuestType;
	SI32	m_siCondition1;
	SI32	m_siCondition2;
	SI32	m_siAmount;
	SI32	m_siRewardEXP;
	SI32	m_siRewardItemUnique[MAX_PARTYQUEST_REWARD_NUM];
	SI32	m_siRewardItemAmount[MAX_PARTYQUEST_REWARD_NUM];
	SI32	m_siRewardItemUseDay[MAX_PARTYQUEST_REWARD_NUM];
	SI32	m_siTimeLimit;

	PartyQuest_UnitData()
	{
		ZeroMemory(this, sizeof(PartyQuest_UnitData));
	}
};

// 파티퀘스트의 카테고리 데이터
struct PartyQuest_CatecoryData
{
	// 카테고리 관련 멤버 변수들
	SI32	m_siIndex;
	SI32	m_siKind;
	SI32	m_siMinLevel;
	SI32	m_siMaxLevel;
	SI32	m_siMinUser;
	SI32	m_siMaxUser;
	SI32	m_siMissionCount;

	PartyQuest_UnitData m_PartyQuest_UnitData[MAX_PARTYQUEST_MISSION_NUM][MAX_PARTYQUEST_STEP_NUM];

	PartyQuest_CatecoryData(){ZeroMemory(this, sizeof(PartyQuest_CatecoryData));}
};


//////////////////////////////////////////////////////////////////////////
//	파티퀘스트 매니저
//////////////////////////////////////////////////////////////////////////

enum PARTYQUEST_CATEGORY_KIND
{
	PARTYQUEST_KIND_NONE,
	PARTYQUEST_KIND_NORMAL,
	PARTYQUEST_KIND_PCROOM,
	PARTYQUEST_KIND_END,
};

enum PARTYQUEST_TYPE
{
	PARTYQUEST_TYPE_NONE,
	PARTYQUEST_TYPE_HUNTING,
	PARTYQUEST_TYPE_MAKEITEM,
	PARTYQUEST_TYPE_CLEAR,
	PARTYQUEST_TYPE_END,
};


enum PARTYQUEST_UPDATE_STATE
{
	PARTYQUEST_UPDATE_STATE_NONE,
	PARTYQUEST_UPDATE_STATE_INCREASE,		// 해당 퀘스트 갱신
	PARTYQUEST_UPDATE_STATE_CLEAR,			// 퀘스트 클리어
	PARTYQUEST_UPDATE_STATE_END,
};


class CPartyQuestMgr_Common
{
public:
	PartyQuest_CatecoryData m_PartyQuest_CategoryData[MAX_PARTYQUEST_CATEGORY_NUM];

public:
	bool	Initialize();	//	매니저 초기화

	// About Load Resource 
	bool	LoadFiles_PartyQuest();
	SI32	QuestKindToInteger(TCHAR* szQuestKindToInteger);
	SI32	QuestTypeToInteger(TCHAR* szQuestType);
	SI32	QuestCondition1ToInteger(TCHAR* szQuestCondition1);
	SI32	QuestCondition2ToInteger(TCHAR* szQuestCondition2);


	
	PartyQuest_CatecoryData*GetPartyQuestCategoryData(SI32 siIndex = 0);
	PartyQuest_UnitData*	GetPartyQuestUnitData(SI32 siCategoryIndex, SI32 siMissionIndex, SI32 siStepIndex);							// 퀘스트 단위 데이터 가져오기
	PartyQuest_UnitData*	GetPartyQuestUnitData(SI32 siCategoryIndex, SI32 siMissionIndex, SI32 siStepIndex, TCHAR* szTitle);			// 퀘스트 단위 데이터 타이틀과 함꼐 가져오기

	bool	DoCheckPartyQuest(CPartyObj* pPartyObject);	// 파티퀘스트 시작 가능 여부 체크
	bool	CheckAlreadyPartyQuest(CPartyObj* pPartyObject);		// 이미 퀘스트중인지
	bool	CheckPartyLeader(CPartyObj* pPartyObject);	// 파티퀘스트 파티장 체크
	bool	CheckUserNum(CPartyObj* pPartyObject);		// 파티 인원 체크
	bool	CheckLevel(CPartyObj* pPartyObject);		// 레벨 체크
	bool	CheckPCRoom(CPartyObj* pPartyObject);		// 피시방 전용 퀘스트 체크

	bool	SetPartyQuestData(PartyQuest_UnitData* pQuestData, SI32 siCategoryIndex, SI32 siMissionindex, SI32 siStep = 0);	// 파티구성시 class CPartyObj 에 있는 파티퀘스트 데이터를 Setting 합니다.
	

public:
	CPartyQuestMgr_Common();
	~CPartyQuestMgr_Common();
};



