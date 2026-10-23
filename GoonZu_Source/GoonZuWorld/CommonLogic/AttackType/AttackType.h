//---------------------------------
// 2003/6/11 김태곤
//---------------------------------

#ifndef _ATTACKTYPE_H
#define _ATTACKTYPE_H

#include <Directives.h>
//-------------------------------
// 공격 타입
//-------------------------------
enum ATTACKTYPE
{
	ATTACKTYPE_NONE					= 0

	,ATTACKTYPE_HAND				= 1
	,ATTACKTYPE_SWORD						
	,ATTACKTYPE_SPEAR						
	,ATTACKTYPE_AXE							
	,ATTACKTYPE_STAFF						
	,ATTACKTYPE_STAFF1						
	,ATTACKTYPE_BOW							
	,ATTACKTYPE_GUN							
	,ATTACKTYPE_CAKE						
	
	,ATTACKTYPE_EGG							
	,ATTACKTYPE_CANNON						
	,ATTACKTYPE_SHELL						
	,ATTACKTYPE_CATFISH						
	,ATTACKTYPE_LADYBUGFAN					
	,ATTACKTYPE_SNOW						
	,ATTACKTYPE_SELFBOMB				// 자폭 
	,ATTACKTYPE_MADOGHOSTFIRE				
	,ATTACKTYPE_JAPANHEADER					
	,ATTACKTYPE_TOHANG						
	
	,ATTACKTYPE_MAGIC						
	,ATTACKTYPE_POISONSTATUS			// 중독되어 있는 상태에서의 타이머 타격. 
	,ATTACKTYPE_SKULLBATFIRE				
	,ATTACKTYPE_FIREGOBLIN					
	,ATTACKTYPE_CASTLECANNON				
	,ATTACKTYPE_POISON					// 중독을 시킬 수 있는 독. 
	,ATTACKTYPE_ICE							
	,ATTACKTYPE_KNIFE						
	,ATTACKTYPE_GREENFIRE						
	,ATTACKTYPE_BLACKINK					

	,ATTACKTYPE_BOOMERANG				// 부메랑 
	,ATTACKTYPE_WINDBLADE				// 양반탈
	,ATTACKTYPE_SHELLFISHBOMB			// 뿔조가비
	,ATTACKTYPE_TREEMON					// 괴수목 - 가짜총알 사용
	,ATTACKTYPE_VINE					// 외눈넝쿨 - 가짜총알 사용
	,ATTACKTYPE_GHOSTFIRE				// 도령귀신
	,ATTACKTYPE_SIMTORYONG				// 심토룡
	,ATTACKTYPE_KINGCUTTLEFISH			// 대왕오적어
	,ATTACKTYPE_CRANE					// 탄광삽차
	,ATTACKTYPE_MONKEY					// 탄광원숭이

	,ATTACKTYPE_CURSEDEFENSE			// 방어력 저하
	,ATTACKTYPE_CURSEMOVESPEED			// 공격력 저하 
	,ATTACKTYPE_BUTCHER					// 제주소잡이
	,ATTACKTYPE_STONEOLDMAN				// 돌하르방
	,ATTACKTYPE_KINGCRAB				// 대왕소라게
	,ATTACKTYPE_FIREARROW				// 불화살
	,ATTACKTYPE_LOBSTER					// 잠수가재
	,ATTACKTYPE_DEADMESSENGER			// 저승사자
	,ATTACKTYPE_MASKMONK				// 중탈
	,ATTACKTYPE_PERSONCANNON			// 일인포탄

	,ATTACKTYPE_CANNON_MAGIC			// 최무선 마법
	,ATTACKTYPE_HEROSWORD				// 소환영웅용 검
	,ATTACKTYPE_STAFF2						
	,ATTACKTYPE_BOW1						
	,ATTACKTYPE_GUN1						
	,ATTACKTYPE_FOAM					// 먹깨비 거품
	,ATTACKTYPE_FIREBALL				// 일반 공격 화염속구
	,ATTACKTYPE_MORTAL					// 회전절구차 퍼지는 공격
	,ATTACKTYPE_WATERARROW				// 물화살
	,ATTACKTYPE_STAFFWIND				// 사명대사 장풍

	,ATTACKTYPE_FLYVINE					// 날아가는 넝쿨
	,ATTACKTYPE_MAGICFIRESTATUS			// 불에 타면서 받는 공격
	,ATTACKTYPE_MAGICFIREBURN			// 불 태우는 공격
	,ATTACKTYPE_SKELLFISH				// 날아가는 생선뼈
	,ATTACKTYPE_FROZEN					// 보스의 얼림
	,ATTACKTYPE_ICESTONE				// 얼음성 보스
	,ATTACKTYPE_NEW_STAFF2				// 지팡이술(4장이상)
	,ATTACKTYPE_NEW_BOW1				// 활(4장이상)
	,ATTACKTYPE_NEW_GUN1				// 총(4장이상)
	,ATTACKTYPE_ENERGYARROW				// 독안괴

	,ATTACKTYPE_SPEAR_MANA				// 관우
	,ATTACKTYPE_BLACKBOSS				// 검은 전사 두목
	,ATTACKTYPE_OCTOPUS					// 문어
	,ATTACKTYPE_OTOHIME					// 문어
	,ATTACKTYPE_BLACKSPIDER				// 흑거미
	,ATTACKTYPE_EVILEYE					// 이블아이
	,ATTACKTYPE_FROZENSORCERER			// 플
	,ATTACKTYPE_CANNONMOL				// 캐논몰(토행요괴)
	,ATTACKTYPE_PRIEST					// 타락성직자
	,ATTACKTYPE_MINEGIRL				// 탄광소녀

	,ATTACKTYPE_GREENTURTLE				// 초원거북
	,ATTACKTYPE_MAGICALRACCOON			// 메지컬모후
	,ATTACKTYPE_MAMARACCOON				// 마마모후
	,ATTACKTYPE_RACCOONTOWER			// 모후타워
	,ATTACKTYPE_CHICKENBIG				// 왕달구
	,ATTACKTYPE_MOONBLADE				// 평강공주
	,ATTACKTYPE_EMPERORARCHER			// 용갱궁
	,ATTACKTYPE_NINJAMAN				// 닌자남  - 원거리.
	,ATTACKTYPE_PIRATEARCHER			// 해적선해골궁병
	,ATTACKTYPE_EMPERORMASTER			// 용갱장

	,ATTACKTYPE_GREATWALLSOLDIER		// 괴수손
	,ATTACKTYPE_GREATWALLMAGICIAN		// 머리만
	,ATTACKTYPE_CATRANGE				// 고양이 스피릿
	
	,ATTACKTYPE_GREATWALLSPIDER			// 수라
	,ATTACKTYPE_BEARTANG				// 탱이
	,ATTACKTYPE_SUPERBEARTANG			// 슈퍼탱이

	,ATTACKTYPE_HALLOWEENGHOST			// 할로윈
	,ATTACKTYPE_CHRISTMASRANGE			// 크리스마스 원거리.
	,ATTACKTYPE_GREATWALLASURA			// 버려진요새 아수라 - 불꽃 쏘기.
	,ATTACKTYPE_FIGHTINGMENRANGE		// 거너스피릿
	,ATTACKTYPE_FROZENQUEEN				// 프로즌 퀸.
	,ATTACKTYPE_FAIRYRANGE				// 요정스피릿(여)
	,ATTACKTYPE_SNOWQUEEN				// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
	,ATTACKTYPE_CLOWN					// 안개미궁 - 어릿광대

	//----------------------------------------------------------------
	//
	// 이 위로 추가해 주세요
	//
	//----------------------------------------------------------------
	,MAX_ATTACKTYPE_NUMBER
};

// 소환수 무기 타입
#define SUMMON_EQUIPTYPE_NONE					0	// 무기타입이 없음(장비 착용불가능)
#define SUMMON_EQUIPTYPE_FARDISTANCE			1	// 근거리 공격
#define SUMMON_EQUIPTYPE_LONGDISTANCE			2	// 원거리 공격
/*
#define MAX_ATTACKTYPE_NUMBER					100

//-------------------------------
// 공격 타입
//-------------------------------
#define ATTACKTYPE_HAND							1
#define ATTACKTYPE_SWORD						2
#define ATTACKTYPE_SPEAR						3
#define ATTACKTYPE_AXE							4
#define ATTACKTYPE_STAFF						5
#define ATTACKTYPE_STAFF1						6
#define ATTACKTYPE_BOW							7
#define ATTACKTYPE_GUN							8
#define ATTACKTYPE_CAKE							9
#define ATTACKTYPE_EGG							10
#define ATTACKTYPE_CANNON						11
#define ATTACKTYPE_SHELL						12
#define ATTACKTYPE_CATFISH						13
#define ATTACKTYPE_LADYBUGFAN					14
#define ATTACKTYPE_SNOW							15
#define ATTACKTYPE_SELFBOMB						16		// 자폭 
#define ATTACKTYPE_MADOGHOSTFIRE				17
#define ATTACKTYPE_JAPANHEADER					18
#define ATTACKTYPE_TOHANG						19
#define ATTACKTYPE_MAGIC						20
#define ATTACKTYPE_POISONSTATUS					21		// 중독되어 있는 상태에서의 타이머 타격. 
#define ATTACKTYPE_SKULLBATFIRE					22
#define ATTACKTYPE_FIREGOBLIN					23
#define ATTACKTYPE_CASTLECANNON					24
#define ATTACKTYPE_POISON						25		// 중독을 시킬 수 있는 독. 
#define ATTACKTYPE_ICE							26
#define ATTACKTYPE_KNIFE						27
#define ATTACKTYPE_GREENFIRE					28	
#define ATTACKTYPE_BLACKINK						29
#define ATTACKTYPE_BOOMERANG					30		// 부메랑 
#define ATTACKTYPE_WINDBLADE					31		// 양반탈
#define ATTACKTYPE_SHELLFISHBOMB				32		// 뿔조가비
#define ATTACKTYPE_TREEMON						33		// 괴수목 - 가짜총알 사용
#define ATTACKTYPE_VINE							34		// 외눈넝쿨 - 가짜총알 사용
#define ATTACKTYPE_GHOSTFIRE					35		// 도령귀신
#define ATTACKTYPE_SIMTORYONG					36		// 심토룡
#define ATTACKTYPE_KINGCUTTLEFISH				37		// 대왕오적어
#define ATTACKTYPE_CRANE						38		// 탄광삽차
#define ATTACKTYPE_MONKEY						40		// 탄광원숭이
#define ATTACKTYPE_CURSEDEFENSE					41		// 방어력 저하
#define ATTACKTYPE_CURSEMOVESPEED				42		// 공격력 저하 
#define ATTACKTYPE_BUTCHER						43		// 제주소잡이
#define ATTACKTYPE_STONEOLDMAN					44		// 돌하르방
#define ATTACKTYPE_KINGCRAB						45		// 대왕소라게
#define ATTACKTYPE_FIREARROW					46		// 불화살
#define ATTACKTYPE_LOBSTER						47		// 잠수가재
#define ATTACKTYPE_DEADMESSENGER				48		// 저승사자
#define ATTACKTYPE_MASKMONK						49		// 중탈
#define ATTACKTYPE_PERSONCANNON					50		// 일인포탄
#define ATTACKTYPE_CANNON_MAGIC					51		// 최무선 마법
#define ATTACKTYPE_HEROSWORD					52		// 소환영웅용 검

#define ATTACKTYPE_STAFF2						53
#define ATTACKTYPE_BOW1							54
#define ATTACKTYPE_GUN1							55

#define ATTACKTYPE_FOAM							56		// 먹깨비 거품
#define ATTACKTYPE_FIREBALL						57		// 일반 공격 화염속구
#define ATTACKTYPE_MORTAL						58		// 회전절구차 퍼지는 공격
#define ATTACKTYPE_WATERARROW					59		// 물화살

#define ATTACKTYPE_STAFFWIND					60		// 사명대사 장풍

#define ATTACKTYPE_FLYVINE						61		// 날아가는 넝쿨

#define ATTACKTYPE_MAGICFIRESTATUS				62		// 불에 타면서 받는 공격
#define ATTACKTYPE_MAGICFIREBURN				63		// 불 태우는 공격

#define ATTACKTYPE_SKELLFISH					64		// 날아가는 생선뼈

#define ATTACKTYPE_FROZEN						65		// 보스의 얼림
#define ATTACKTYPE_ICESTONE						66		// 얼음성 보스

#define ATTACKTYPE_NEW_STAFF2					67	// 지팡이술(4장이상)
#define ATTACKTYPE_NEW_BOW1						68	// 활(4장이상)
#define ATTACKTYPE_NEW_GUN1						69	// 총(4장이상)

#define ATTACKTYPE_ENERGYARROW					70 // 독안괴
#define ATTACKTYPE_SPEAR_MANA					71 // 관우

#define ATTACKTYPE_BLACKBOSS					72 // 검은 전사 두목
#define ATTACKTYPE_OCTOPUS						73		// 문어
#define ATTACKTYPE_OTOHIME						74		// 문어

#define ATTACKTYPE_BLACKSPIDER					75	// 흑거미
#define ATTACKTYPE_EVILEYE						76	// 이블아이

#define ATTACKTYPE_FROZENSORCERER				77	// 플
#define ATTACKTYPE_CANNONMOL					78	// 캐논몰(토행요괴)

#define	ATTACKTYPE_PRIEST						79	// 타락성직자

#define ATTACKTYPE_MINEGIRL						80	// 탄광소녀

#define ATTACKTYPE_GREENTURTLE					81	// 초원거북
#define	ATTACKTYPE_MAGICALRACCOON				82	// 메지컬모후
#define ATTACKTYPE_MAMARACCOON					83	// 마마모후
#define ATTACKTYPE_RACCOONTOWER					84	// 모후타워
#define	ATTACKTYPE_CHICKENBIG					85	// 왕달구
#define ATTACKTYPE_MOONBLADE					86  // 평강공주
#define ATTACKTYPE_EMPERORARCHER				87	// 용갱궁
#define ATTACKTYPE_NINJAMAN						88	// 닌자남  - 원거리.
#define ATTACKTYPE_PIRATEARCHER					89	// 해적선해골궁병
*/

//#define MAX_ATTACKTYPE_NUMBER					80
//-------------------------------
// 공격타입의 속성
//-------------------------------
// 공격 범위등에 대한 속성. 
#define ATTACKTYPE_ATB_RANGE1						0x1		// 주변까지 검색해서 목표적만 공격한다. 		
#define ATTACKTYPE_ATB_RANGE2						0x2		// 목표적이 무조건 맞는다. 
#define ATTACKTYPE_ATB_RANGE3						0x4		// 일정 범위안의 적을 모두 공격한다. 	

// 공격 효과에 대한 속성. 
#define ATTACKTYPE_ATB_PHYSICS						0x1
#define ATTACKTYPE_ATB_MAGIC						0x2


// 기타 속성. 
#define ATTACKTYPE_ATB_MINRANGE						0x2
#define ATTACKTYPE_ATB_PULL							0x4			// 피격시 뒤로 밀림. 
#define ATTACKTYPE_ATB_PALSY						0x8			// 피격시 마비 효과. 
#define ATTACKTYPE_ATB_CRITICAL						0x10		// 필살기가 작동하는가. 

//---------------------------------
// cltAttackTargetList
//---------------------------------
#define MAX_ATTACK_TARGET_LIST_NUMBER				20

class cltAttackDamageInfo{
public:
	SI32 siAttackType;
	SI32 siDamage;
	SI32 siMagicKind;

	cltAttackDamageInfo()
	{
		Init();
	}
	cltAttackDamageInfo(SI32 attacktype, SI32 damage, SI32 magickind)
	{
		siAttackType	= attacktype;
		siDamage		= damage;
		siMagicKind		= magickind;
	}
	void Init()
	{
		siAttackType	= 0;
		siDamage		= 0;
		siMagicKind		= 0;

	}

	void Set(cltAttackDamageInfo* pcldamageinfo)
	{
		siAttackType	= pcldamageinfo->siAttackType;
		siDamage		= pcldamageinfo->siDamage;
		siMagicKind		= pcldamageinfo->siMagicKind;
	}

};

class cltAttackTargetList{
private:
	SI32 siIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_ATTACK_TARGET_LIST_NUMBER>					siID;
	NSafeTArray<cltAttackDamageInfo, MAX_ATTACK_TARGET_LIST_NUMBER>		clDamageInfo;
#else
	SI32 siID[MAX_ATTACK_TARGET_LIST_NUMBER];
	cltAttackDamageInfo clDamageInfo[MAX_ATTACK_TARGET_LIST_NUMBER];
#endif

public:
	cltAttackTargetList();
	BOOL IsInList(SI32 id);
	void Push(SI32 id, cltAttackDamageInfo* pcldamageinfo);
	BOOL Pop(SI32* pid, cltAttackDamageInfo* pcldamageinfo);

};

//---------------------------------
// 공격 타입 정보 
//---------------------------------
class cltAttackTypeInfo{
public:
	TCHAR	szAttackTypeCode[60];

	SI32 siAttackType;
	SI32 siBulletUnique;		// 불렛
	SI32 siFlag1;				// 공격 범위에 대한 설정 
	SI32 siFlag2;				// 공격의 물리/마법 속성에 대한 설정. 
	SI32 siFlag3;				// 기타 설정. 

	cltAttackTypeInfo(TCHAR* szattacktypecode, SI32 attacktype, SI32 bulletunique, SI32 flag1, SI32 flag2, SI32 flag3);

	~cltAttackTypeInfo();

	SI32 GetFlagRange()const{return siFlag1;}
	SI32 GetFlagPower()const{return siFlag2;}
	SI32 GetFlagEtc()const{return siFlag3;}
};

//----------------------------------
// 공격 타입 정보 관리자. 
//----------------------------------
class cltAttackTypeInfoManager{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltAttackTypeInfo*, MAX_ATTACKTYPE_NUMBER>	pclAttackTypeInfo;
#else
	cltAttackTypeInfo *pclAttackTypeInfo[MAX_ATTACKTYPE_NUMBER];
#endif

	cltAttackTypeInfoManager();
	~cltAttackTypeInfoManager();

	// 공격 타입을 추가한다. 
	BOOL Add(SI32 attacktype, TCHAR* szattacktypecode, SI32 bulletunique, SI32 flag1, SI32 flag2, SI32 flag3);

	// 공격 타입의 총알 종류를 구한다.
	SI32 GetBulletUnique(SI32 attacktype);

	// Code를 근거로 AttackType을 찾는다.
	SI32 FindAttackTypeFromCode(TCHAR* code);
	
	// 공격 타입의 속성을 확인한다. 
	bool IsAtb(SI32 attacktype, SI32 atb);


};

#endif
