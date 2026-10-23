//---------------------------------
// 2006/2/01 최연준
//---------------------------------

#ifndef _BLACKARMY_H
#define _BLACKARMY_H

#include <Directives.h>
#include <list>
using namespace std;

#include <Stdio.h>
#include "..\StructureClass\StructureClass.h"
#include "..\..\Common\Item\ItemCommon\cltitem.h"

#define MAX_WARRANK_PERSON_PER_MAP		5
#define	MAX_BLACKARMY_MAP_CNT			15
#define MAX_BLACKARMY_MAP_PERSON_CNT	50
#define MAX_BLACKARMY_APPLY_CNT			MAX_BLACKARMY_MAP_CNT * MAX_BLACKARMY_MAP_PERSON_CNT
#define MAX_BLACKARMY_UNITKIND			10
#define MAX_BLACKARMY_MAP_ARMY_CNT		150			// 한 맵에 최대 150마리까지 존재한다.
#define MAX_BLACKARMY_MAP_REGEN_CNT		20			// 한 맵에 최소로 남아있을 몬스터 수

#define BLACKARMY_MAP_STARTINDEX		180
#define BLACKARMY_GATEUNIQUE_START		400

#define BLACKARMY_APPLY_MONEY			5000
#define BLACKARMY_BOSS_BODYGUARD		20
#define BLACKARMY_INC_LEVEL				10

//#define BLACKARMY_BOSS_SHOW				50
//#define BLACKARMY_REJEN_CNT				30
//#define BLACKARMY_FIRST_MON_CNT			200
//#define BLACKARMY_SECOND_MON_CNT		400
//#define BLACKARMY_TOTAL_MON_CNT			600

class cltBlackArmyApplyPerson {
public :
	cltSimplePerson clApplyPerson;		// 캐릭터 정보
	SI16 siWarMapUnique;				// 할당 맵 정보
//	bool bPartyLearder;
//	SI16 siPartyIndex;

	bool bWarMapAlloc;					// 맵에 할당되었는지 정보

	cltBlackArmyApplyPerson();
	cltBlackArmyApplyPerson(cltSimplePerson applyperson);
	~cltBlackArmyApplyPerson();

	void Init();
	void Set(cltBlackArmyApplyPerson *pinfo);

};


class cltBlackArmyApplyList {
public:
	SI32 siPersonIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltBlackArmyApplyPerson, MAX_BLACKARMY_APPLY_CNT>		clApplyPersonList;
#else
	cltBlackArmyApplyPerson clApplyPersonList[MAX_BLACKARMY_APPLY_CNT];
#endif

	cltBlackArmyApplyList();
	~cltBlackArmyApplyList();

	SI32 AddApplyPerson(cltBlackArmyApplyPerson *pPerson);
	void InitList();

	SI32 FindApplyFromPersonid(SI32 personid);
};

class cltBlackArmyUnit{
public:
	SI32 siKind;

	SI32 siNum;				

	SI32 siBasicLevel;
	SI32 siRandLevel;

	SI32 siCurNum;
	SI32 siMadeNum;

	cltBlackArmyUnit();
	cltBlackArmyUnit(SI32 kind, SI32 num, SI32 curnum, SI32 madenum, SI32 basic, SI32 rand);
	void Set(cltBlackArmyUnit *blackarmyunit);
	void Init();

};


class cltBlackArmyWarMap {
public:
	SI32 siPersonIndex;
	SI16 siMapIndex;

	SI32 siGateUnique;

	SI32 siMapUnitNum;			// 한맵의 병사수
	SI32 siMapCurUnitNum;		// 현재 병사수. 
	SI32 siMapMadeNum;			// 현재까지 만들어진 수. 

//	SI32 siWarStep;				// 전쟁진행 단계
	
	SI16 siWarcmd;

	bool bWarDoing;
	bool bBossMaked;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltBlackArmyUnit, MAX_BLACKARMY_UNITKIND>				clBlackArmyUnit;
	NSafeTArray<cltBlackArmyApplyPerson, MAX_BLACKARMY_MAP_PERSON_CNT>	clWarPersonList;
#else
	cltBlackArmyUnit clBlackArmyUnit[MAX_BLACKARMY_UNITKIND];
	cltBlackArmyApplyPerson clWarPersonList[MAX_BLACKARMY_MAP_PERSON_CNT];
#endif

	cltBlackArmyWarMap();
	~cltBlackArmyWarMap();

	SI32 AddWarPerson(cltBlackArmyApplyPerson *pPerson);
	void InitList();
	void Init();

};

class cltMVPEffectList {
public :

	DWORD dwSelectMVPClock;
	SI32 siID;

	cltMVPEffectList ()
	{
		ZeroMemory(this, sizeof( cltMVPEffectList ) );
	}

	cltMVPEffectList (DWORD selectmvpclock, SI32 id)
	{
		dwSelectMVPClock = selectmvpclock;
		siID = id;
	}
};




//-----------------------------------------------
// 흑의군대 이벤트관리자. 
//-----------------------------------------------
class cltBlackArmyManager {
public:
	cltBlackArmyApplyList clApplyList;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltBlackArmyWarMap, MAX_BLACKARMY_MAP_CNT>		clWarMap;			// 맵 데이터 저장공간
	NSafeTArray<cltBlackArmyUnit, MAX_BLACKARMY_UNITKIND>		oneMapUnit;			// 병력정보 저장공간
#else
	cltBlackArmyWarMap	clWarMap[MAX_BLACKARMY_MAP_CNT];
	cltBlackArmyUnit	oneMapUnit[MAX_BLACKARMY_UNITKIND];
#endif


//	SI32		siTotalUnitNum;
//	SI32		siTotalCurUnitNum;		// 현재 병사수. 

	SI32		siUnitKind;				// 유니트는 몇 종이 있는가?
	DWORD		uiLeftTime;				// 남은 시간. (ms) 
	DWORD		dwWarStartClock;
	SI32		siNextJoinMap;			// 유저 분배시 다음에 배분할 맵 번호

	SI32		siSettingMonNum;

	bool bWarSwitch ;	// 지금 전쟁이 진행중인가?
	bool bApplySwitch;
	bool bAutoBlackWarSwitch;

	list<cltMVPEffectList> listMvpEffect;

	//------------
	// 출력 
	//------------
	bool bDrawSwitch;
	SI32 siDrawCmdFont;
	DWORD dwDrawStartClock;

	//SI32 m_siMakeMapNum;				// 유저수로 만들어질 맵의 개수

	cltBlackArmyManager();
	~cltBlackArmyManager();

	void Init();
	void LoadWarInfoFile();

	// 전쟁을 기획한다. 
	bool MakeWar(DWORD clock);

	void Action();

	void JoinWarMap(SI16 ApplyIndex);
	void JoinWarMapWithParty(SI16 ApplyIndex);

	SI32 ApplyPerson(cltBlackArmyApplyPerson *person);

	SI16 FindWarMapIndex(SI32 mapindex);
	SI16 FindWarUnitIndex(SI32 kind);

	void SendWarMsg(SI32 cmd, SI32 warmapindex);
	void WarEnd(SI32 warmapindex);
	void AllWarEnd();

	void SetDraw(SI32 cmd);
	void Draw(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);

};
#endif
