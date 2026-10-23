//---------------------------------
// 2004/8/19 김태곤
//---------------------------------
#ifndef _EVENTGOGU_H
#define _EVENTGOGU_H

#include "..\Event.h"

#define MAX_GOGU_SOL_KIND	3

class cltEventGogu : public cltEvent{
public:
	cltEventGogu();
	~cltEventGogu();

	SI32 siTowerID;			// 광개토대왕비 아이디
	SI32 siTotalSolNumber;
	SI32 siMonsterLevel;

	SI32 siMapIndex;

	SI32 siSolKind[MAX_GOGU_SOL_KIND];
	SI32 siSolNum[MAX_GOGU_SOL_KIND];

	void Init();

	// 이벤트 시작. 
	SI32 Start(SI32 villageunique, bool bwarswitch);
	// 이벤트 끝.
	bool End();
	
	bool Action();

	// 명나라 군사들을 만든다.
	void MakeSoldier();
	
	// 광개토대왕비를 만든다. 
	void MakeTower();

	void AddSol(SI32 kind, SI32 num);

	//-------------------------------------------
	// 승리한 마을에 대박아이템을 보상한다. 
	//-------------------------------------------
	void RewardWiner();
	
};

#endif