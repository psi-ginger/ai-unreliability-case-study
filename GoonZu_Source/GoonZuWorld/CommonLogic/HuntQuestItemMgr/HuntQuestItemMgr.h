#ifndef	__HUNTQUESTITEMMGR_H__
#define __HUNTQUESTITEMMGR_H__

#include <Directives.h>
#include "../../Common/CommonHeader.h"

const SI32 MAX_HUNTQUESTITEM_UNIQUE_NUM = 30;	//	퀘스트 유니크 MAX NUM
const SI32 MAX_HUNTQUESTITEM_UNIT_NUM	= 10;	//	단위데이터의 MAX NUM



struct HuntQuestItem_Unit
{
	SI32	m_siMonsterKind;
	SI32	m_siItemUnique;
	SI32	m_siRandomRate;

	HuntQuestItem_Unit(){ZeroMemory(this, sizeof(HuntQuestItem_Unit));}
};

struct HuntQuestItem_Unit_Info
{
	SI32	m_siQuestUnique;
	
	HuntQuestItem_Unit	m_HuntQuestItem_Unit[MAX_HUNTQUESTITEM_UNIT_NUM];

	HuntQuestItem_Unit_Info(){ZeroMemory(this, sizeof(HuntQuestItem_Unit_Info));}
};

class CHuntQuestItemMgr
{
private:
	SI32						m_siQuestUniqueNum;

	HuntQuestItem_Unit_Info		m_HuntQuestItem_Unit_info[MAX_HUNTQUESTITEM_UNIQUE_NUM];		// 퀘스트 데이터의 단위 정보
	
public:
	bool				Initialize();				//	매니저 초기화
	bool				LoadHuntQuestItemData();	//	퀘스트아이템 Resource Load

	HuntQuestItem_Unit* FindQuestItem(SI32 siQuestUnique, SI32 siMonsterKind);

public:
	CHuntQuestItemMgr();
	~CHuntQuestItemMgr();
};

#endif //__HUNTQUESTITEMMGR_H__