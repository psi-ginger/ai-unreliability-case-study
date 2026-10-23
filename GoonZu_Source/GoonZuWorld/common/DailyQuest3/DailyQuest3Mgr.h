#ifndef _DAILYQUEST3MGR_H_
#define _DAILYQUEST3MGR_H_

#include "..\SMLib\LibList\LibListMgr.h"
#include "NDataTypes.h"
#include <time.h>

#define MAX_AVAILABLE_DAILYQUEST3_NUM	100	///	최대 데일리 퀘스트 3 생성 갯수
#define MAX_DAILYQUEST3_MAKEITEM_NUM	30	//	최대 데일리 퀘스트 3 아이템 생산 갯수

class CDailyQuest3ObjData;
class CDailyQuest3Mgr : public CLibListMgr
{
public:
	CDailyQuest3Mgr();
	~CDailyQuest3Mgr();
	
	void Action( clock_t CurrentClock );
	
	void GetDailyQuest3List( CDailyQuest3ObjData* pDailyQuest3ObjDataList );
	CDailyQuest3ObjData*	GetDailyQuest3ListObj( SI32 Index, SI32 Count, SI32 *pListIndex );
	
	bool SetDailyQuest3List( SI32 ListIndex, SI16 ItemUnique);
	void InitQuest3ListData( SI32 ListIndex, SI16 ItemUnique, SI16 siItemNum, SI16 RewardItemUnique,SI16 RewardItemNum, GMONEY Fee, SI32 LeftTime,SI32 RewardExp, bool bHiddenQuest);


private:
	CDailyQuest3ObjData *m_pDailyQuest3ObjDataList;
	clock_t				m_PreClock[MAX_AVAILABLE_DAILYQUEST3_NUM];
	
};

#endif