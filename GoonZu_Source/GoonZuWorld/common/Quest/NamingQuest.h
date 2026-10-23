#pragma once 
#include "../../NLib/NDataTypes.h"
#include "Quest.h"
#include "../../NLib/NArray.h"


#define NAMINGQUEST_TYPE_HUNT		1		// 사냥
#define NAMINGQUEST_TYPE_COLLECT	2		// 수집
#define NAMINGQUEST_TYPE_MOVE		3		// 이동
#define NAMINGQUEST_TYPE_GROWTH		4		// 성장




class cltNamingQuestUnit
{
private:
	TCHAR	m_szQuestName[64];
	cltQuestClearConditionInfo m_clClearCond;
	TCHAR	m_szNaming[64];
	SI32	m_siAttack;			// 공격력 보너스 (%)
	SI32	m_siDefense;		// 방어력 보너스 (%)
	SI32	m_siSpeed;			// 이동력 보너스
	SI32	m_siSkill;			// 스킬 경험치 보너스 (%)

public:
	SI32	GetAttack()		{ return m_siAttack; }
	SI32	GetDefense()	{ return m_siDefense; }
	SI32	GetSpeed()		{ return m_siSpeed; }
	SI32	GetSkill()		{ return m_siSkill; }
	TCHAR*	GetQuestName()	{ return m_szQuestName; }
	TCHAR*	GetNaming()		{ return m_szNaming; }


public:
	cltNamingQuestUnit()
	{
		m_clClearCond.Init();
		ZeroMemory( m_szQuestName,  sizeof(m_szQuestName) );
		ZeroMemory( m_szNaming,		sizeof(m_szNaming) );
		m_siAttack	= 0;
		m_siDefense = 0;
		m_siSpeed	= 0;
		m_siSkill	= 0;
	}

	~cltNamingQuestUnit() {	}

	void Set( TCHAR* questName, cltQuestClearConditionInfo* pclCond, TCHAR* naming, SI32 attack, SI32 defense, SI32 speed, SI32 skill );

	cltQuestClearConditionInfo* GetQuestClerCond();
	
};


class cltNamingQuest
{
private:
	SI32	m_siType;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltNamingQuestUnit, MAX_NAMINGQUEST_LEVEL>		m_clUnit;
#else
	cltNamingQuestUnit	m_clUnit[MAX_NAMINGQUEST_LEVEL];
#endif
	SI32	m_siMaxLevel;									// 해당 퀘스트 명칭의 최대 레벨

public:
	cltNamingQuest();
	~cltNamingQuest();

	void				LoadText( TCHAR* filename );
	void				SetType( SI32 type );
	cltNamingQuestUnit* GetQuestInfo( SI32 level );
	SI32				GetMaxLevel() { return m_siMaxLevel; }
	TCHAR*				GetTypeName();
	SI32				GetType();
};

class cltNamingQuestManager
{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltNamingQuest, MAX_NAMINGQUEST_INDEX>		clNamingQuest;
#else
	cltNamingQuest clNamingQuest[MAX_NAMINGQUEST_INDEX];
#endif

public:
	cltNamingQuestManager();
	~cltNamingQuestManager();

	void				LoadText( TCHAR* fileName );
	cltNamingQuestUnit* GetQuestInfo( SI32 index, SI32 level );
	SI32				GetMaxLevel( SI32 index );
	TCHAR*				GetTypeName( SI32 index );
	SI32				GetType( SI32 index );
};
