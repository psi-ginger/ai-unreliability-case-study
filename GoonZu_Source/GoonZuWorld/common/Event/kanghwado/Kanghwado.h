//---------------------------------
// 2005/06/15 김광명
//---------------------------------
#ifndef _EVENTKANGHWADO_H
#define _EVENTKANGHWADO_H

#include "..\Event.h"

#define MAX_KANGHWADO_SOL_KIND	3
#define MAX_KANGHWADO_SOLSHIP_KIND	1

#define MAX_WESTMOTHERSHIP	3

#define MAX_CANNON_NUM 10

class cltEventKanghwado : public cltEvent{
public:
	cltEventKanghwado();
	~cltEventKanghwado();

	// 강화도 보호비 아이디(성문)
	SI32 siTowerID;		
	// 서양 모선 아이디
	SI32 siMotherShipID[MAX_WESTMOTHERSHIP];
	// 서양 수송선 아이디
	SI32 siTroopShipID[MAX_WESTMOTHERSHIP];
	// 강화도에 나올 병사수
	SI32 siTotalSolNumber;
	// 강화도에 나올 정찰병사수
	SI32 siTotalPatrolSolNumber;
	// 강화도에 나올 배수
	SI32 siTotalShipNumber;
	SI32 siTotalMotherShipNumber;
	// 강화도에 병사들 레벨
	SI32 siMonsterLevel;
	// 강화도 맵의 인덱스
	SI32 siMapIndex;
	// 최대로 나올 병사수
	SI32 siMaxSolNumber;
	// 오후2시 전쟁인가? 마을 이벤트인가?
	bool bWarStart;

	SI32 siSolKind[MAX_KANGHWADO_SOL_KIND];
	SI32 siSolNum[MAX_KANGHWADO_SOL_KIND];

	SI32 siSolShipKind[MAX_KANGHWADO_SOLSHIP_KIND];
	SI32 siSolShipNum[MAX_KANGHWADO_SOLSHIP_KIND];

	// 수송선이 만들어졌나 체크
	bool bMakeTroopShip[MAX_WESTMOTHERSHIP];	
	
	// 모선이 나올좌표
	POINT MotherShipPoint[MAX_WESTMOTHERSHIP];

	// 수송선이 도착해야할 좌표
	POINT TroopShipArrivalPoint[MAX_WESTMOTHERSHIP];

	// 강화대포 아이디
	SI32 KanghwaCannon[MAX_CANNON_NUM];
	
	void Init();

	// 이벤트 시작. 
	SI32 Start(SI32 villageunique, bool warstart = false);
	// 이벤트 끝.
	bool End();

	bool Action();

	// 강화도 군사들을 만든다.
	void MakeSoldier();

	void MakePatrolSoldier();

	// 강화도 보호비를 만든다. 
	void MakeTower();

	// 강화도 대포 만들기
	void MakeCannon();

	// 서양 모선 만들기
	void MakeMotherShip();

	// 서양 공격배 만들기
	void MakeBattleBoat();

	void AddSol(SI32 kind, SI32 num);
	void AddShip(SI32 kind, SI32 num);

	void DelSolAll();
	void DelShipAll();

	//-------------------------------------------
	// 승리한 마을에 대박아이템을 보상한다. 
	//-------------------------------------------
	void RewardWiner();

};

#endif