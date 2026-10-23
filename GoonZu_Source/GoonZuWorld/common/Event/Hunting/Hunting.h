//---------------------------------
// 2004/8/13 김태곤
//---------------------------------
#ifndef _EVENTHUNTING_H
#define _EVENTHUNTING_H

#include "..\Event.h"

#include <set>

#define EVENT_GOONZUDAY_PLAY_COUNT		1		// 군주 데이시 수행한 이벤트 횟수.


class cltEventTimeManager;


class cltEventHunting : public cltEvent{
public:

	SI16	siMonsterID;			// 몬스터의 아이디. 
	SI16	siMonsterLevel;			// 몬스터의 설정 레벨 

	cltEventHunting();
	~cltEventHunting();

	void Init();

	// 불가사리 사냥 이벤트를 시작한다. 
	bool Start(SI32 sivillageunique, SI32 cost = 0);
	// 불가사리 사냥 이벤트를 시작한다. 
	void End();

	// 불가사리 사냥 이벤트 진행. 
	bool Action();

	// 사냥감을 만든다. 
	void MakeMonster();

	// 몹 한대라도 친사람.
	void AddContributor(SI32 AttackerID);

private:
	std::set<SI32> _Contributors;
	SI32	siCost;

	// 이벤트 횟수 카운트와 횟 수 셋팅 관련 함수들
public:
	//SI16	siHuntingEventcount;
	NSafeTArray<SI16, MAX_VILLAGE_NUMBER>		m_arEventCount_EachVillage;
	void	HuntingEventCount_Increase();
	void	HuntingEventCount_Reset();
	SI16	HuntingEvnetCount_Get();

	bool	IsMosterHit(SI32 siCharID, SI64 siExp, SI32 siRewardItemUnique = 0, SI32 siRewardItemNum = 0);

};


#endif