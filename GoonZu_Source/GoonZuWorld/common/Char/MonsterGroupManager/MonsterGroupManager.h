//---------------------------------
// 2004/12/23 성준엽
//---------------------------------

#ifndef _MONSTERGROUPMANAGER_H
#define _MONSTERGROUPMANAGER_H

class cltIDManager;
class cltCharCommon;
class cltItem;

#include "CommonHeader.h"

#define MAX_MONSTER_GROUP_NUM			100		// 서버 내 최대 그룹 수

#define MAX_MONSTER_NUM_PER_GROUP		5		// 그룹 내 최대 몬스터 수

#define MAX_MONSTER_KIND_PER_GROUP		5		// 그룹 내 최대 몬스터 종류 수

class cltKindInfoPerGroup{
public:
	SI32		siKind;			// 종류
	SI32		siValue;		// 비율
	SI32		siLevel;		// 레벨

	void Init();
};

// 그룹 생성 정보
class cltInitInfoPerGroup{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltKindInfoPerGroup, MAX_MONSTER_KIND_PER_GROUP>		pclKindInfoPerGroup;
#else
	cltKindInfoPerGroup			pclKindInfoPerGroup[MAX_MONSTER_KIND_PER_GROUP];
#endif

	SI32						siMapIndex;		// 출현할 맵 인덱스
	SI32						siCenterX;		// 출현할 맵의 좌표 x
	SI32						siCenterY;		// 출현할 맵의 좌표 y
	SI32						siRadius;		// 출현할 위치 범위
	bool						bMapBoss;		// 맵의 보스 그룹이냐 아니냐

	void Init();
	bool BeingKindInfo(SI32 index);
};

// 실제 그룹 정보
class cltMonsterGroupInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_MONSTER_NUM_PER_GROUP>		siCharUnique;
#else
	SI32						siCharUnique[MAX_MONSTER_NUM_PER_GROUP];
#endif
	SI32						siBossUnique;
	bool						bMapBoss;				// 맵의 보스 그룹이냐 아니냐
	bool						bSetMapBossGroup;		// 맵의 보스 그룹이면 졸개들을 설정했냐 안 했냐

	void Init();
};

class cltMonsterGroupManager{
public:
	cltMonsterGroupManager();
	~cltMonsterGroupManager();

#ifdef _SAFE_MEMORY
	NSafeTArray<cltInitInfoPerGroup, MAX_MONSTER_GROUP_NUM>		pclInitInfo;
	NSafeTArray<cltMonsterGroupInfo, MAX_MONSTER_GROUP_NUM>		pclMonsterGroup;
#else
	cltInitInfoPerGroup		pclInitInfo[MAX_MONSTER_GROUP_NUM]; // 1~MAX_MONSTER_GROUP_NUM - 1
	cltMonsterGroupInfo		pclMonsterGroup[MAX_MONSTER_GROUP_NUM]; // 1~MAX_MONSTER_GROUP_NUM - 1
#endif

public:
	void Action();

	bool BeingGroup(SI32 index);
	bool BeingInitGroup(SI32 index);
	bool IsMapBossGroup(SI32 index);

	void MakeGroup(SI32 index);
	void SetBoss(SI32 index);

	void MakeMapBossGroup(SI32 index);

	void DeleteMember(SI32 index,SI32 unique,BOOL bBoss );

	SI32 GetBossID(SI32 index);

	bool FindBattleMember(SI32 * targetID,SI32 index,BOOL bBoss);
	void MemberAttack(SI32 targetID,SI32 index);

	bool SetInitFromFile(SI32 index , SI32 mapindex,SI32 centerx,SI32 centery ,SI32 radius,
						 SI32* kind,SI32* level,SI32* value );

};

#endif
