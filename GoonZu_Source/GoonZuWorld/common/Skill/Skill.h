//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _SKILL_H
#define _SKILL_H

#include <Directives.h>

#define MAX_SKILL_NUMBER			121		// 스킬의 최대 갯수. (120->121)
#define ALL_MAKESKILL_UNIQUE		((MAX_SKILL_NUMBER) + 1)	// 제조스킬 전부를 가리킴
#define ALL_WEAPONSKILL_UNIQUE		((MAX_SKILL_NUMBER) + 2)	// 무기스킬 전부
#define ALL_PRODUCTSKILL_UNIQUE		((MAX_SKILL_NUMBER) + 3)	// 생산스킬 전부
#define ALL_ALLSKILL_UNIQUE			((MAX_SKILL_NUMBER) + 4)	// 모든 스킬
#define ALL_ALLAVATARSKILL_UNIQUE	((MAX_SKILL_NUMBER) + 5)	// 모든 아바타 스킬

#define MAX_CHILD_SKILL_NUM			4
#define MAX_SKILL_TYPE_NUMBER		15

//KHY - 0318 - 군주중국 스킬 70제한.
#if _NHN_CHINA
	#define MAX_SKILL_LEVEL				70			// 경험에 의한 레벨 최고단계. 
#else
	#define MAX_SKILL_LEVEL				80			// 경험에 의한 레벨 최고단계. 
#endif

#define MAX_WEAPONSKILL_LEVEL		5			// 전투향상술 최고 레벨

#define MAX_SKILL_NAME_LENGTH		64			// 스킬 이름의 최대 길이. 
#define MAX_SKILL_TYPE_NAME_LENGTH	64
#define MAX_SKILL_TYPE_CODE_LENGTH	64

#define MAX_SKILL_TREEDLG_TIMER		1000		// 기술 트리 다이얼로그의 타이머 간격 설정

//--------------------------------
// 스킬의 타입.
//--------------------------------
// 1 ~ MAX_SKILL_TYPE_NUMBER까지 순차적으로 나가야 한다. 

// 아래는 type
//#define SKILLTYPE_MED				1			// 의술
//#define SKILLTYPE_FOOD		    2			// 요리술

//#define SKILLTYPE_WEAR				1			// 장비제작술  
//#define SKILLTYPE_MAKEWEAPON		2			// 무기제작술
//#define SKILLTYPE_ACCESSARY			3			// 장신구제작술
//#define SKILLTYPE_USEWEAPON			4			// 무기 사용술
//#define SKILLTYPE_MAKETOOL			5			// 도구 제작술 
//#define SKILLTYPE_SEA				6			// 해양기술
//#define SKILLTYPE_HARVEST			7			// 기본생산술
//#define SKILLTYPE_ETC				8			// 기타기술
//#define SKILLTYPE_FIGHT				9			// 전투력향상술
//#define SKILLTYPE_MAKEAVATAR		10			// 아바타 제조술


//	[종호_SKILLINTERFACE]
#define SKILLTYPE_R_USEWEAPON			1			// 무기사용술
#define SKILLTYPE_R_MAKEWEAPON			2			// 무기제작술
#define SKILLTYPE_R_ARMOR				3			// 방어구제작술
#define SKILLTYPE_R_MAKEHARVEST			4			// 생산제작술
#define SKILLTYPE_R_HARVEST				5			// 생산기술
#define SKILLTYPE_R_MAKECLOTH			6			// 재단술
#define SKILLTYPE_R_MAKEACCESSARY		7			// 악세사리 제작술		
#define SKILLTYPE_R_ETC					8			// 기타기술
#define SKILLTYPE_FIGHT					9			// 전투력향상술



// 무기 스킬종류
#define WEAPONSKILLTYPE_SWORD		1
#define WEAPONSKILLTYPE_SPEAR		2
#define WEAPONSKILLTYPE_AXE			3
#define WEAPONSKILLTYPE_STAFF		4
#define WEAPONSKILLTYPE_BOW			5
#define WEAPONSKILLTYPE_GUN			6

#define MAX_WEAPONSKILLTYPE_NUM		6				// 무기스킬 최대 갯수

//아래 열몇개는 unique 임
#define SKILL_MAKEMED					1			// 약조제술.
#define SKILL_MAKEFOOD					2			// 요리술.
#define SKILL_MAKEARMOUR1				3			// 갑옷제작술1
#define SKILL_MAKEHELMET1				4			// 투구제작술1
#define SKILL_MAKESWORD1				5			// 검제작술1
#define SKILL_MAKESPEAR1				6			// 창제작술1
#define SKILL_MAKEAXE1					7			// 도끼제작술1
#define SKILL_MAKESTAFF1				8			// 지팡이제작술1
#define SKILL_MAKEBOW1					9			// 활제작술1
#define SKILL_MAKEGUN1					10			// 총제작술1
#define SKILL_MAKESHOES1				11			// 신발제작술1	
#define SKILL_MAKEBELT1					12			// 요대제작술1	
//cyj 반지제작술 삭제 #define SKILL_MAKERING1				13			// 반지제작술1
#define SKILL_MAKEDRESS1				14			// 의복제작술1
#define SKILL_USESWORD1					16			// 검술
#define SKILL_USESPEAR1					17			// 창술
#define SKILL_USEBOW1					18			// 궁술
#define SKILL_USEAXE1					19			// 부술(도끼술)
#define SKILL_USEGUN1					20			// 총술
#define SKILL_USESTAFF1					21			// 지팡이술
#define SKILL_SUMMON					22			// 소환술 
#define SKILL_FISH						23			// 어획술 
//cyj 목걸이제작술 삭제 #define SKILL_MAKENECK1				25			// 목걸이제작술1
#define SKILL_HORSETRAINING				26			// 말조련술
#define SKILL_MAKEIRON1					27			// 철물제작술 

//cyj 도기/문방구/어구 제작술 통합
//#define SKILL_MAKEFISHINGROD		24			// 어구제작술. 
//#define SKILL_MAKEDISH1			28			// 도기제작술 

//cyj 대포제작술 삭제 #define SKILL_MAKECANNON			29			// 대포제조술
//cyj 부적제작술 삭제 #define SKILL_MAKESCROLL			30			// 부적제작술
#define SKILL_CAPTURE					31			// 포획술
#define SKILL_MAKEHAT1					32			// 모자제작술
//cyj 고물처리술 삭제 #define SKILL_ALCHEMY				33			// 고물처리술
#define SKILL_FARM						34			// 농경술
#define SKILL_MINE						35			// 채굴술

#define SKILL_SWORD_INCDR				36			// 검술 : 물리회피율증가
#define SKILL_SWORD_INCVIT				37			// 검술 : 최대생명력증가
#define SKILL_SWORD_INCMAGAT			38			// 검술 : 마법공격력증가 (기존 SKILL_SWORD_INCWEAAT:무기공격력 증가)

#define SKILL_SPEAR_INCAC				39			// 창술 : 물리방어력증가
#define SKILL_SPEAR_DOUBLEAT			40			// 창술 : 일정확률로 공격력을 2배 (기존 SKILL_SPEAR_INCRECMANA:마법회복력증가)
#define SKILL_SPEAR_INCMAGAT			41			// 창술 : 마법공격력증가

#define SKILL_AXE_DECGAP				42			// 부술 : 공격력간격감소 (기존 SKILL_AXE_INCRECLIFE:체력회복력증가)
#define SKILL_AXE_INDURE				43			// 부술 : 피격동작시간감소
#define SKILL_AXE_INCCRI				44			// 부술 : 필살율증가

#define SKILL_BOW_DRAIN					45			// 궁술 : 체력흡수 (기존 SKILL_BOW_INCAR:명중율증가)
#define SKILL_BOW_INCRANGE				46			// 궁술 : 사거리증가
#define SKILL_BOW_INCMAGAT				47			// 궁술 : 마법공격력증가

#define SKILL_GUN_INCPHYAR				48			// 총술 : 물리명중률증가 (기존 SKILL_GUN_INCMOVE:이동속도증가)
#define SKILL_GUN_DECMAGDELAY			49			// 총술 : 마법사용간격감소
#define SKILL_GUN_INCWEAAT				50			// 총술 : 무기공격력증가

#define SKILL_STAFF_INCMAG				51			// 지팡이술 : 최대도력증가
#define SKILL_STAFF_DECUSEMANA			52			// 지팡이술 : 마법필요도력감소
#define SKILL_STAFF_INCMAGAT			53			// 지팡이술 : 마법공격력증가

#define SKILL_MAKETOOL1					54			// 생산도구제작술

#define SKILL_ARTIFACT					55			// [추가 : 황진성 2008. 2. 25 => 아티펙트 술 (결정체 제조술) ]
// 검 무기향상술 히든 1단계
#define SKILL_SWORD_SPEAR_INCAC			56
#define SKILL_SWORD_AXE_DECGAP			57
#define SKILL_SWORD_BOW_DRAIN			58
#define SKILL_SWORD_GUN_INCPHYAR		59
#define SKILL_SWORD_STAFF_INCMAG		60
// 검 무기향상술 히든 2단계
#define SKILL_SWORD_SPEAR_DOUBLEAT		61
#define SKILL_SWORD_AXE_INDURE			62
#define SKILL_SWORD_BOW_INCRANGE		63
#define SKILL_SWORD_GUN_DECMAGDELAY		64
#define SKILL_SWORD_STAFF_DECUSEMANA	65

// 창 무기향상술 히든 1단계
#define	SKILL_SPEAR_SWORD_INCDR			66
#define	SKILL_SPEAR_AXE_DECGAP			67
#define	SKILL_SPEAR_BOW_DRAIN			68
#define	SKILL_SPEAR_GUN_INCPHYAR		69
#define	SKILL_SPEAR_STAFF_INCMAG		70
// 창 무기향상술 히든 2단계
#define	SKILL_SPEAR_SWORD_INCVIT		71
#define	SKILL_SPEAR_AXE_INDURE			72
#define	SKILL_SPEAR_BOW_INCRANGE		73
#define	SKILL_SPEAR_GUN_DECMAGDELAY		74
#define	SKILL_SPEAR_STAFF_DECUSEMANA	75

// 도끼 무기향상술 히든 1단계
#define	SKILL_AXE_SWORD_INCDR			76
#define	SKILL_AXE_SPEAR_INCAC			77
#define	SKILL_AXE_BOW_DRAIN				78
#define	SKILL_AXE_GUN_INCPHYAR			79
#define	SKILL_AXE_STAFF_INCMAG			80
// 도끼 무기향상술 히든 2단계
#define	SKILL_AXE_SWORD_INCVIT			81
#define	SKILL_AXE_SPEAR_DOUBLEAT		82
#define	SKILL_AXE_BOW_INCRANGE			83
#define	SKILL_AXE_GUN_DECMAGDELAY		84
#define	SKILL_AXE_STAFF_DECUSEMANA		85

// 활 무기향상술 히든 1단계
#define	SKILL_BOW_SWORD_INCDR			86
#define	SKILL_BOW_SPEAR_INCAC			87
#define	SKILL_BOW_AXE_DECGAP			88
#define	SKILL_BOW_GUN_INCPHYAR			89
#define	SKILL_BOW_STAFF_INCMAG			90
// 활 무기향상술 히든 2단계
#define	SKILL_BOW_SWORD_INCVIT			91
#define	SKILL_BOW_SPEAR_DOUBLEAT		92
#define	SKILL_BOW_AXE_INDURE			93
#define	SKILL_BOW_GUN_DECMAGDELAY		94
#define	SKILL_BOW_STAFF_DECUSEMANA		95

// 총 무기향상술 히든 1단계
#define	SKILL_GUN_SWORD_INCDR			96
#define	SKILL_GUN_SPEAR_INCAC			97
#define	SKILL_GUN_AXE_DECGAP			98
#define	SKILL_GUN_BOW_DRAIN				99
#define	SKILL_GUN_STAFF_INCMAG			100
// 총 무기향상술 히든 2단계
#define	SKILL_GUN_SWORD_INCVIT			101
#define	SKILL_GUN_SPEAR_DOUBLEAT		102
#define	SKILL_GUN_AXE_INDURE			103
#define	SKILL_GUN_BOW_INCRANGE			104
#define	SKILL_GUN_STAFF_DECUSEMANA		105

// 지팡이 무기향상술 히든 1단계
#define	SKILL_STAFF_SWORD_INCDR			106
#define	SKILL_STAFF_SPEAR_INCAC			107
#define	SKILL_STAFF_AXE_DECGAP			108
#define	SKILL_STAFF_BOW_DRAIN			109
#define	SKILL_STAFF_GUN_INCPHYAR		110
// 지팡이 무기향상술 히든 2단계
#define	SKILL_STAFF_SWORD_INCVIT		111
#define	SKILL_STAFF_SPEAR_DOUBLEAT		112
#define	SKILL_STAFF_AXE_INDURE			113
#define	SKILL_STAFF_BOW_INCRANGE		114
#define	SKILL_STAFF_GUN_DECMAGDELAY		115

//	아바타 제조술
#define SKILL_MAKEAVATAR_DRESS			116			//	[종호]	아바타 의복 제작술
#define SKILL_MAKEAVATAR_HAT			117			//	[종호]	아바타 모자 제작술
#define SKILL_MAKEAVATAR_MANTLE			118			//	[종호]	아바타 망토 제작술
#define SKILL_MAKEAVATAR_RING			119			//	[종호]	아바타 반지 제작술
#define SKILL_MAKEAVATAR_NECK			120			//	[종호]	아바타 목걸이 제작술

#define SKILL_SIKLLBOOK					121			//	[진성]	스킬북 스킬. 이 스킬은 유동적이다. 어떤 스킬인가는 결정되있지 않다.





//[알림: 스킬 개수는 최대 255(현재는 MAX_SKILL_NUMBER=120) 까지만 가능 합니다. 더 늘릴때는 DB작업 필요합니다]
//---------------------------------------------------------------------------------------------------------------------

// 각 기술들의 속성. 
#define SKILLATB_LIST				0x00000001	// 장인등록 기술 목록에 있는 기술. 
#define SKILLATB_WEAPON				0x00000002	// 무기사용술창 목록에 있는 기술
#define SKILLATB_HIDDENGRADE1		0x00000004	// 무기향상술 히든 1단계 기술
#define SKILLATB_HIDDENGRADE2		0x00000008	// 무기향상술 히든 2단계 기술

#define MAX_WEAPONSKILL_POW1		1			// 20% 적용
#define MAX_WEAPONSKILL_POW2		2			// 30% 적용
#define MAX_WEAPONSKILL_POW3		3			// 40% 적용

//-----------------------------------------
// Bit 단위 
//----------------------------------------
#define UPGRADESKILLTYPE_MAKE				1
#define UPGRADESKILLTYPE_USEWEAPON			2
#define UPGRADESKILLTYPE_USE_TABLE_VALUE	4	// 테이블에 있는 값을 사용하려면 이것을 쓰자. 
#define UPGRADESKILLTYPE_STATUS_UP_20		8	// 의복(모자, 옷)에 렌덤으로 스텟 +20 적용됨. 



// 무기술달인
enum SKILLMASTER
{
	SKILLMASTER_NONE				= 0,

	SKILLMASTER_PHYSICALATTACK,		// 물리공격
	SKILLMASTER_PHYSICALDEFENSE,	// 물리방어
	SKILLMASTER_PHYSICALDODGE,		// 물리회피
	SKILLMASTER_PHYSICALHIT,		// 물리명중

	SKILLMASTER_MAGICALATTACK,		// 마법공격
	SKILLMASTER_MAGICALDEFENSE,		// 마법방어
	SKILLMASTER_MAGICALDODGE,		// 마법회피
	SKILLMASTER_MAGICALHIT,			// 마법명중

	SKILLMASTER_STR,				// 근력
	SKILLMASTER_HAND,				// 손재주
	SKILLMASTER_DEX,				// 순발력
	SKILLMASTER_MAG,				// 지력
	SKILLMASTER_WIS,				// 지혜
	SKILLMASTER_VIT,				// 생명력
	SKILLMASTER_LUCK,				// 행운

	SKILLMASTER_CRITICAL,			// 크리티컬
	SKILLMASTER_ATTACKRANGE,		// 공격범위
	SKILLMASTER_ATTACKINTERVAL,		// 공격속도

	//---------------------------------------------
	// 새로운 무기술 적용값은 이 위에 해주세요
	//---------------------------------------------
	SKILLMASTER_MAX
};

enum SKILLMASTER_WEAPONTYPE
{
	SKILLMSTER_WEAPONTYPE_NONE	= 0,

	SKILLMSTER_WEAPONTYPE_SWORD,
	SKILLMSTER_WEAPONTYPE_SPEAR,
	SKILLMSTER_WEAPONTYPE_BOW,
	SKILLMSTER_WEAPONTYPE_AXE,
	SKILLMSTER_WEAPONTYPE_GUN,
	SKILLMSTER_WEAPONTYPE_STAFF,

	SKILLMSTER_WEAPONTYPE_MAX
};

class cltCharManager;
class cltItem;
class cltSkillManager;

// 개개의 스킬별 저장 단위. 
class cltSkillSaveUnit{
public:
	UI08 uiSkillUnique;
	UI08 uiLevel;

	SI32 siSkillExp;		// 기술 경험치.

	cltSkillSaveUnit()
	{
		uiSkillUnique	= 0;
		uiLevel			= 0;
		siSkillExp		= 0;
	}
	void Set(cltSkillSaveUnit* pclinfo)
	{
		uiSkillUnique	= pclinfo->uiSkillUnique;
		uiLevel			= pclinfo->uiLevel;
		siSkillExp		= pclinfo->siSkillExp;
	}
};

#define		THAT_SKILL_EXIST			3100		// 기존에 스킬이 존재한다.
#define		PARENT_NOT_EXIST			3200		// 부모 스킬이 아직 습득하지 못했다.
//---------------------------------------
// 스킬의 저장을 담당할 매니저. 
//---------------------------------------
class cltPSkillInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltSkillSaveUnit, MAX_SKILL_NUMBER>	clSkill;
#else
	cltSkillSaveUnit								clSkill[MAX_SKILL_NUMBER];
#endif

	cltPSkillInfo()
	{
	}

	~cltPSkillInfo(){};


	void Set(cltPSkillInfo* pskill);

	// 기술을 만들 수 있는지 확인한다. 
	bool CanMake(SI32 skillunique, SI32* pskillindex, SI32 *pRetVal);

	// 스킬을 최초로 연마한다.
	BOOL MakeSkill(SI32 skillunique);

	// 스킬을 높여준다.
	BOOL IncreaseSkillLevel(SI32 skillunique, SI32 level);
	//// 경험 레벨을 높여준다.
	//BOOL IncreaseSkillExpLevel(SI32 skillunique, SI32 level);

	// 특정 스킬의 경험치를 가져온다.
#ifdef _SAFE_MEMORY
	SI32 GetSkillExp(SI32 skillunique)	{return clSkill[skillunique].siSkillExp ;}
#else
	SI32 GetSkillExp(SI32 skillunique)const{return clSkill[skillunique].siSkillExp ;}
#endif

	// 특정 스킬의 레벨을 구한다. 
	SI32 GetSkillLevel(SI32 skillunique);
	// 특정 스킬의 경험레벨을 구한다. 
	SI32 GetSkillExpLevel(SI32 skillunique);
	// 특정 스킬의 총레벨을 구한다. 
	SI32 GetTotalSkillLevel(SI32 skillunique, SI32 sikind, SI32 level, cltItem* pclitemlist = NULL, SI32 id = 0);

	// [지연] 특정 스킬의 총 경험치를 구한다.
	SI32 GetTotalSkillExp(SI32 skilluinque);

	// 무기 장착을 위한 스킬이 개발되어 있는지 확인한다. 
	bool ChcekUseWeaponSkill(SI32 type, cltItem* pclitemlist, SI32* preturnval);

	// 2003.10.23
	// 무기 장착을 위한 조건을 가져온다. ( 어떤 기술사용 가능한가와 사용포인트를 알아야 하므로.. )
	bool GetMountSkillKind(SI32 type, cltItem* pclitemlist, SI32 *pMountSkill, SI32 *pSkillPoint );

	// 부모 기술의 레벨을 구한다. 
	UI32 GetParentSkillLevel(SI32 skillunique, cltItem* pclitemlist);

	// 사용자의 기술중 랜덤하게 업그레이드 시킬 기술의 종류와 양을 구한다.
	bool GetRandSkillForUpgrade(cltSkillManager* pclskillmanager, cltItem* pclitemlist, SI32* pskillunique, SI32* pupgradeskilllevel);

	// [지연] 사용자의 생산기술중 하나를 고르고, 해당 스킬의 현재 경험치포인트 + 1 한 만큼을 목표 경험치로 퀘스트 조건 얻는 부분으로 돌려준다.
	bool GetRandSkillEXPupForUpgrade(cltSkillManager* pclskillmanager, cltItem* pclitemlist, SI32* pskillunique, SI32* increaseSkillExp);
};

// 전투향상술 조건 정보
class cltWeaponSkillInfo{
public:
	SI32	siUnique;					// 스킬 유니크

	SI32	siLimitLevel;				// 최소 익히는 레벨
	SI32	siBasicSkillLimitLevel;		// 기본 스킬 제한

	SI32	siLimitSkillUnique;			// 제한 스킬
	SI32	siSkillLimitLevel;			// 최소 제한 스킬 레벨

	SI32	siNeedSkillPoint;			// 스킬 배우는데 필요한 스킬포인트

	GMONEY	siSkillCost;				// 스킬 배우는데 드는 비용

	SI16	siApplyPercent;				// 스킬능력치 적용 퍼센트
	
	// 위 정보들은 기본적으로 해당 무기일때만 유효한 정보이지만 아래 정보들은 다른 무기여도	배울 수 있는 조건이다
	SI16	siHiddenSkillGrade;			// 이 스킬이 몇단계 스킬인가(히든 1이 무기술 4단계, 히든 2가 무기술 5단계)
	SI16	siHiddenLimitLevel;			// 최소 레벨 조건
	SI16	siHiddenLimitSkillLevel;	// 최소 스킬 레벨 조건

	SI16	siItemUnique;				// 히든 스킬 배울때 필요한 아이템
	SI16	siItemNum;					// 히든 스킬 배울때 필요한 아이템 갯수


	cltWeaponSkillInfo()
	{
		ZeroMemory( this, sizeof(cltWeaponSkillInfo) );
	}
};

class cltSkillInfo{
public:
	SI32 siUnique;			// 스킬 개개의 식별자.
	SI32 siParentRef;		// 부모의 레퍼런스. 

	SI32 siSkillType;
	SI32 siWeaponType;

	SI32 siMaxSkillLevel;	// 최대 스킬 레벨. 

	SI32 siSkillAtb;		// 기술의 속성. 

	SI32 siBitMapIndex;		// 비트맵의 인덱스. 

	TCHAR szName[MAX_SKILL_NAME_LENGTH];
	TCHAR szCode[MAX_SKILL_NAME_LENGTH];
	TCHAR szSkillTitle[MAX_SKILL_NAME_LENGTH];

	SI32 siExplainUnique;		// 설명 텍스트 유니크. 

	cltWeaponSkillInfo	** pclWeaponSkillInfo;

	cltSkillInfo(SI32 unique, SI32 skilltype, SI32 skillatb, TCHAR *code, TCHAR* name, SI32 maxskilllevel,
		SI32 parentref, SI32 bitmapindex, SI32 explainunique,SI32 weapontype );
	~cltSkillInfo();

	void MakeSkillTitle();

};

class cltSkillTypeInfo{
public:
	SI32 siSkillType;
	TCHAR szSkillCode[MAX_SKILL_TYPE_CODE_LENGTH];
	TCHAR szName[MAX_SKILL_TYPE_NAME_LENGTH];

	BOOL bShowData;

	cltSkillTypeInfo(SI32 skilltype, TCHAR* pskillcode, TCHAR* name,BOOL bshow );
	~cltSkillTypeInfo();
};

//--------------------------------------------------------------------------------------------------
// 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인
//--------------------------------------------------------------------------------------------------
class cltWeaponSkillMaster
{
private:
	SI16 m_siApplyLevel;	// 적용레벨
	SI16 m_siApplyAbility;	// 적용능력	(SKILLMASTER 값중에 하나임)
	SI32 m_siApplyValue;	// 적용값

public:
	cltWeaponSkillMaster();
	cltWeaponSkillMaster( SI16 siApplyLevel, SI16 siApplyAbility, SI32 siApplyValue );

	~cltWeaponSkillMaster();

	void Init( void );

	void Set( SI16 siApplyLevel, SI16 siApplyAbility, SI32 siApplyValue );
	SI32 GetValue( SI16 siApplyLevel, SI16 siApplyAbility );

	bool IsEmpty( void );

	SI16 GetApplyLevel( void );
	SI16 GetApplyAbility( void );
	SI32 GetApplyValue( void );

};

class cltWeaponSkillMasterManager
{
private:
	NSafeTArray<cltWeaponSkillMaster, SKILLMSTER_WEAPONTYPE_MAX> m_clWeaponSkillMaster;

public:
	cltWeaponSkillMasterManager();
	~cltWeaponSkillMasterManager();

	void Init( void );

	void LoadSkillMasterFile( void );

	SI32 GetApplyValue( SI32 siItemType, SI16 siSkillLevel, SI16 siApplyAbility );
	SI32 GetSkillUseWeaponFromItemType( SI32 siItemType );

	bool GetEmblemExplain( IN SI32 siItemType, IN SI32 siSkillLevel, OUT TCHAR* pszExplain, IN SI32 siTextSize );
	bool IsCanTakeEmblemWithNotice( IN SI32 siSkillUnique, IN SI32 siSkillLevel, OUT TCHAR* pszNotice, IN SI32 siTextSize );
	//bool GetEmblemGimgAndFontIndex( IN SI32 siItemType, IN SI32 siSkillLevel, OUT SI32* psiGimg, OUT SI32* psiFontIndex );
	UI08 GetEmblemIndex( IN SI32 siItemType, IN SI32 siSkillLevel );

private:
	SI32 ConvertTextToWeaponType( TCHAR* pszWeaponTypeText );
	SI32 ConvertTextToAbility( TCHAR* pszAbilityText );

	void Add( SI32 siWeaponType, SI16 siApplyLevel, SI16 siApplyAbility, SI32 siApplyValue );
	SI32 GetWeaponTypeFromItemType( SI32 siItemType );
	SI32 GetWeaponTypeFromSkillUnique( SI32 siSkillUnique );

	SI32 GetEmblemTextNum( SI32 siWeaponType );
	SI32 GetAbilityTextNum( SI32 siAbility );

};

//--------------------------------------------------------------------------------------------------
// 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인 / 무기술의 달인
//--------------------------------------------------------------------------------------------------

// level에 필요한 경험치 값. (1부터 시작한다.)
SI32 GetRequiredSkillExpForLevel(SI32 level);
BOOL LoadSkillExpData();
// 기술 경험치를 근거로 경험레벨을 구한다. 
SI32 GetSkillLevelFromExp(SI32 exp);

// 스킬 레벨에 따라 요구되는 내구도 소모를 구한다. 
SI32 GetWasteDurability(SI32 skilllevel);

SI32 ConvertTextToSkillUnique( TCHAR* pszSkillUnique );
SI32 ConvertTextToSkillTypeUnique(TCHAR* pszSkillUnique);

#endif