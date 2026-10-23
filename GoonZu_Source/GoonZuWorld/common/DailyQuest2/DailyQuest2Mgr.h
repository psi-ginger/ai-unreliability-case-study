#ifndef _DAILYQUEST2MGR_H_
#define _DAILYQUEST2MGR_H_

#include "..\SMLib\LibList\LibListMgr.h"
#include "NDataTypes.h"
#include <time.h>
#include "../../../NLib/NArray.h"

#define MAX_AVAILABLE_DAILYQUEST2_NUM	30	///최대 데일리 퀘스트 생성 갯수

class CDailyQuest2ObjData;
class CDailyQuest2Mgr : public CLibListMgr
{
public:
	CDailyQuest2Mgr();
	~CDailyQuest2Mgr();
	
	//void CreateMaxEmptyObj();

	/*
	bool Set( SI32 PersonID, SI32 CharID, SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, SI32 RewardExp, GMONEY Fee, SI32 LeftTime, SI16 KillMonsterNum );
	void End( SI16 Index, SI16 Count, SI32 PersonID, SI32 CharID, bool bSuccess );
	

	bool IsExist( SI32 PersonID );

	
	*/

	void Action( clock_t CurrentClock );
	
	void GetDailyQuest2List( CDailyQuest2ObjData* pDailyQuest2ObjDataList );
	CDailyQuest2ObjData*	GetDailyQuest2ListObj( SI16 Index, SI16 Count, SI16 *pListIndex );
	
	bool SetDailyQuest2List( SI16 ListIndex, SI16 MonsterKind);//, BOOL bPCRoomQuest );
	void InitQuest2ListData( SI16 ListIndex, SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 LeftTime,/* SI32 siRewardGuildPoint, */bool bHiddenQuest);//, BOOL bPCRoomQuest );


private:
	CDailyQuest2ObjData *m_pDailyQuest2ObjDataList;
#ifdef _SAFE_MEMORY
	NSafeTArray<clock_t, MAX_AVAILABLE_DAILYQUEST2_NUM>		m_PreClock;
#else
	clock_t				m_PreClock[MAX_AVAILABLE_DAILYQUEST2_NUM];
#endif
	
};

#endif