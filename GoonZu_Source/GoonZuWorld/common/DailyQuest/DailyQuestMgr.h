#ifndef _DAILYQUESTMGR_H_
#define _DAILYQUESTMGR_H_

#include "..\SMLib\LibList\LibListMgr.h"
#include "NDataTypes.h"
#include <time.h>
#include "../../../NLib/NArray.h"

#define MAX_AVAILABLE_DAILYQUEST_NUM	50

class CDailyQuestObjData;
class CDailyQuestMgr : public CLibListMgr
{
public:
	CDailyQuestMgr();
	~CDailyQuestMgr();
	
	//void CreateMaxEmptyObj();

	/*
	bool Set( SI32 PersonID, SI32 CharID, SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, SI32 RewardExp, GMONEY Fee, SI32 LeftTime, SI16 KillMonsterNum );
	void End( SI16 Index, SI16 Count, SI32 PersonID, SI32 CharID, bool bSuccess );
	

	bool IsExist( SI32 PersonID );

	
	*/

	void Action( clock_t CurrentClock );
	
	void GetDailyQuestList( CDailyQuestObjData* pDailyQuestObjDataList );
	CDailyQuestObjData*	GetDailyQuestListObj( SI16 Index, SI16 Count, SI16 *pListIndex );
	
	bool SetDailyQuestList( SI16 ListIndex, SI16 MonsterKind);//, BOOL bPCRoomQuest );
	void InitQuestListData( SI16 ListIndex, SI16 MonsterKind, SI16 MonsterNum, 
		SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, 
		SI32 RewardExp, GMONEY Fee, SI32 LeftTime);//, BOOL bPCRoomQuest );


private:
	CDailyQuestObjData *m_pDailyQuestObjDataList;
#ifdef _SAFE_MEMORY
	NSafeTArray<clock_t, MAX_AVAILABLE_DAILYQUEST_NUM>		m_PreClock;
#else
	clock_t				m_PreClock[MAX_AVAILABLE_DAILYQUEST_NUM];
#endif
	
};

#endif