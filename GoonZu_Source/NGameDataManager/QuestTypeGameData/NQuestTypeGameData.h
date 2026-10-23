//=============================================================================
//
//=============================================================================
#ifndef _NQUESTGAMEDATA_H_
#define _NQUESTGAMEDATA_H_

#include "../../NetworkLib/ITZNetLib/itzbase.h"

//---------------------------------
// 퀘스트 타입 
//---------------------------------
#define MAX_QUEST_TYPE				20
#define QUEST_TYPE_SPECIAL			0	// 스페셜 퀘스트. 
#define QUEST_TYPE_BATTLE1			1	// 기초 전투. (내금위종사관)
#define QUEST_TYPE_MAKEHEAL1		2	// 기초 조제술
#define QUEST_TYPE_MAKESWORD1		3	// 기초 검제조술
#define QUEST_TYPE_MAKEBOW1			4	// 기초 활제조술
#define QUEST_TYPE_MAKEHELMET1		5	// 기초 투구제조술
#define QUEST_TYPE_BEGINNER3		6	// 김정호 퀘스트 
#define QUEST_TYPE_MAKESPEAR1		7	// 기초 창제조술
#define QUEST_TYPE_MAKEARMOUR1		8	// 갑옷 제조술
#define QUEST_TYPE_MAKEAX1			9	// 도끼 제조술
#define QUEST_TYPE_MAKEGUN1			10	// 총 제조술
#define QUEST_TYPE_MAKESTAFF1		11	// 지팡이 제조술
#define QUEST_TYPE_MAKEBELT1		12	// 요대 제조술
#define QUEST_TYPE_MAKESHOES1		13	// 신발 제조술
#define QUEST_TYPE_BEGINNER2		14	// 내시 퀘스트 
#define QUEST_TYPE_BATTLE2			15	// 장길산 퀘스트
#define QUEST_TYPE_BASIC1			16	// 기초 상업. (장금이 퀘스트)
#define QUEST_TYPE_BEGINNER4		17	// 나죄수퀘스트
#define QUEST_TYPE_MAKEBEGINNER1	18	//초보자를 위한 제조 퀘스트
#define QUEST_TYPE_BEGINNER5		19	// 역관퀘스트(신이 전용)

#define MAX_QUEST_PER_TYPE_NUMBER	32	// 하나의 퀘스트 타입당 최대 퀘스트 수. 

class NQuestTypeGameData
{
public:
	SI32		m_siQuestType;
	TCHAR		m_szQuestName[128];
	SI32		m_siQuestUnique[MAX_QUEST_PER_TYPE_NUMBER];

public:
	NQuestTypeGameData()
	{
		m_siQuestType		= -1;
		m_szQuestName[0]	= '\0';
		memset(m_siQuestUnique, 0, sizeof(m_siQuestUnique));
	}

	~NQuestTypeGameData()
	{
	}

	void Set()
	{

	}
};

#endif
