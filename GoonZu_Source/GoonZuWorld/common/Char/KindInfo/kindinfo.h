//---------------------------------
// 2003/5/28 김태곤
//---------------------------------
#ifndef _CLTKINDINFO_H
#define _CLTKINDINFO_H  

#include "../../Char/CharAni/CharAni.h"
#include "../ParametaBox/ParametaBox.h"
#include "../../Item/ItemCommon/cltItem.h"

//-----------------
// Definition
//-----------------
#define MAX_KIND_NAME_LENGTH					50
#define MAX_KINDINFO_NUMBER						2000			// 엔피씨 오버로 맥스 수치 변경. 400->420->500
#define MAX_ITEM_PROBABILITY_PER_KIND			5			// 한종류의 Kind가 보유할 수 있는 초기 아이템의 수.
#define MAX_SUMMON_CHATCOMMANDNUMBER			100			// 채팅커맨드의 최대 갯수
//---------------------------------------------------------------------
// 캐릭터의 종류
//---------------------------------------------------------------------
#define KIND_HERO1						1		// 견이
#define KIND_HEROINE1					2		// 연이
#define KIND_HERO2						3		// 신이
#define KIND_HEROINE2					4		// 신이짝
#define KIND_HERO3						5		// 일본남
#define KIND_HEROINE3					6		// 일본여
#define KIND_HERO4						7		// 서양남
#define KIND_HEROINE4					8		// 서양녀

#define KIND_SYSTEM						9		// 시스템대기 상태의 캐릭터. (접속중인 캐릭터이다.) 

#define KIND_JOKER						253		// 오토잡는 몬스터

//#define MAX_KINDINFO_NUMBER			240		
//#define MAX_KIND_NUMBER				240

//--------------------------------
//각 파라메타들의 정의 
//--------------------------------

#define DPOWER(x)			(x)		// 방어력 정의 
#define MOVESPEED(x)	    (x)		// 이동속도 정의 
#define SIZE(x)				(x)		// 사이즈 정의 
#define ATTACKINTERVAL(x)	(x)		// 공격 주기 
#define PERCENT(x)			(x)		// 퍼센트 정의 

// 공격 내성 정의
#define SWORD_DP(x)			(x)
#define SPEAR_DP(x)			(x)
#define AXE_DP(x)			(x)
#define BOW_DP(x)			(x)
#define GUN_DP(x)			(x)
#define STAFF_DP(x)			(x)
#define CANNON_DP(x)		(x)

class cltQuestRewardInfo;


//---------------------------------------------
// DROP ITEM 정보를 얻어오기위한 구조체 //JAYCEE
//---------------------------------------------
class cltDropItem{
public:
	TCHAR szName[MAX_ITEM_NAME_LENGTH]; // MAX_ITEM_NAME_LENGTH
	TCHAR szItemNum[20];
	TCHAR szPercent[15];

	cltDropItem();
	
};

//--------------------------------------------
// 보스 캐릭터 피격시 주는 물품 정보 
//--------------------------------------------
class cltBossItemInfoUnit{
public:
	SI16 siItemUnique;
	SI16 siRate;		
	
	cltBossItemInfoUnit();
	cltBossItemInfoUnit(SI16 itemunique, SI16 rate);

	void Init();
	void Set(cltBossItemInfoUnit* pclinfo);
	
};
#define MAX_BOSS_ITEM_NUMBER	30
class cltBossItemInfo{
public:

	SI32	siItemIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltBossItemInfoUnit, MAX_BOSS_ITEM_NUMBER>	clUnit;
#else
	cltBossItemInfoUnit clUnit[MAX_BOSS_ITEM_NUMBER];
#endif

	cltBossItemInfo();
	void Init();
	void Set(cltBossItemInfo* pclinfo);
	void Add(cltBossItemInfoUnit* pclinfo);
	void Add(SI16 itemunique, SI16 rate);

	// 임의의 아이템을 찾는다. 
	bool GetRandItem(SI32* pitemunique, SI32* pitemnum);


};

//---------------------------------------------
// 캐릭터가 갖게 되는 아이템의 종류와 그 비율. 
//---------------------------------------------
class cltKindItemProbability{
public:
	SI32	siItemUnique;
	SI32	siProbability;						//  1/10000
	SI32	siItemNum;							// 나오는 아이템의 개수. 
	SI32	siItemMinNum;

	cltKindItemProbability();
	~cltKindItemProbability();

	void Set(SI32 unique, SI32 probability, SI32 itemnum,SI32 minnum);
};

class cltKindItemProbabilitySet{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltKindItemProbability, MAX_ITEM_PROBABILITY_PER_KIND>		clKindItemProbability;
#else
	cltKindItemProbability clKindItemProbability[MAX_ITEM_PROBABILITY_PER_KIND];
#endif

	cltKindItemProbabilitySet(){};
	~cltKindItemProbabilitySet(){}

	void Init();

	// 아이템을 추가한다. 
	//prbability : 1/10000 확률. 
	BOOL Add(SI32 siitemunique, SI32 prbability, SI32 itemnum,SI32 minnum);

	// 랜덤하게 하나의 아이템의 유니크 값을 리턴한다. 아무것도 리턴하지 않을 수도 있다.
	bool GetRandDropItem(SI32 level, SI32 AddRate, SI32* punique, SI32* pitemnum);

	// 아이템의 유니크를 근거로 이 캐릭터가 이 아템을 주는 것과 관련된 정보를 찾는다. 
	cltKindItemProbability* FindItem(SI32 itemunique);

};

//---------------------------------------
// 디폴트 무장 속성 
//---------------------------------------
class cltDefaultWeaponInfo{
private:
	SI32 siAttackType;			// 공격 타입 (ATTACKTYPE_SWORD..) 
	SI32 siAttackRange;			// 사정거리 범위 

	SI32 siDefaultMinDamage;
	SI32 siDefaultMaxDamage;

	SI32 siDefaultAC;

public:
	cltDefaultWeaponInfo();

	void Set(SI32 attacktype, SI32 range, SI32 mindamage, SI32 maxdamage, SI32 ac);
	void Set(cltDefaultWeaponInfo* pclinfo);

	SI32 GetAttackType() const;
	SI32 GetAttackRange() const;

	SI32 GetDefaultMinDamage() const;
	SI32 GetDefaultMaxDamage() const;

	SI32 GetDefaultAC() const;
};

//-------------------------------------
// KindInfo의 출력관련 정보 
//-------------------------------------
class cltKIDrawInfo{
public:
	SI32 siPortraitFont;
	SI32 siHeadNameFont;				// 머리 위에 뜨는 이름. (NPC)	
	SI32 siBottomIndent;        	
	SI32 siXsizePercent,  siYsizePercent;
	SI16 siTransparentMode;
	bool bHideShadow;
	SI08 siAlphaValue;

	cltKIDrawInfo();

	void Set(SI32 portrait, SI32 headnamefont, SI32 bottomindent,
			 SI32 xpercent, SI32 ypercent, SI32 transparentmode,bool hideshadow = false ,SI08 alphavalue = 0 );
};

//--------------------------------------------
// 효과음 정보. 
//--------------------------------------------
class cltKIEffectInfo{
public:
	SI32	siHitEffect;			// 피격시 이펙트. (퍽..) 
	SI32	siAttackEffect;			// 공격시 이펙트. (에잇... )
	SI32	siAttackEffect1;		// 공격시 이펙트. (에이이이~ㅅ)
	SI32	siDyingEffect;			// 사망시 이펙트. (아악..) 
	SI32	siSelectEffect1;			// 선택시 이펙트. 
	SI32	siSelectEffect2;			// 선택시 이펙트. 
	SI32	siSelectEffect3;			// 선택시 이펙트. 

	cltKIEffectInfo();

	void Set(SI32 hiteffect, SI32 attackeffect, SI32 attackeffect1, SI32 dyingeffect, SI32 selecteffect1, SI32 selecteffect2, SI32 selecteffect3);
	void Set(cltKIEffectInfo* pclinfo);
    
	SI32 GetRandomeSelectEffect();
};

#define MAX_NPC_TEXT_NUM	5
//--------------------------------------
// 전쟁시 NPC 대사
//--------------------------------------
class cltWarChatInfo{
public:
	SI32 siWinChatNum;
	SI32 siChatNum;

#ifdef _SAFE_MEMORY
	NSafeTArray<TCHAR*, MAX_NPC_TEXT_NUM>	pszWarTextWin;
	NSafeTArray<TCHAR*, MAX_NPC_TEXT_NUM>	pszWarText;
#else
	TCHAR* pszWarTextWin[MAX_NPC_TEXT_NUM];
	TCHAR* pszWarText[MAX_NPC_TEXT_NUM];
#endif

	SI32 siMapindex;

	cltWarChatInfo();
	~cltWarChatInfo();

	void AddWin(TCHAR* ptext,SI32 mapindex);
	void Add(TCHAR* ptext,SI32 mapindex);
};

//--------------------------------------
// NPC 대사 
//--------------------------------------

class cltKindChatInfo{
public:
	SI32 siChatNum;
#ifdef _SAFE_MEMORY
	NSafeTArray<TCHAR*, MAX_NPC_TEXT_NUM>	pszRandText;
#else
	TCHAR* pszRandText[MAX_NPC_TEXT_NUM];
#endif

	cltKindChatInfo();
	~cltKindChatInfo();

	void Add(TCHAR* ptext);
};


//---------------------------------------
//  Kind 고용 조건. 
//---------------------------------------
class cltKindHireCondition{
public:
	GMONEY	siBasicMoney;			// 고용 최소 비용. 
	SI32	siMinVillageLevel;		// 고용 최소 마을 레벨 

	cltKindHireCondition();

	void Init();
	void Init(GMONEY money, SI32 villagelevel);

	void Set(cltKindHireCondition* pclinfo);
};

//-----------------------------------------------
// 각 몬스터의 속성
//----------------------------------------------
class cltCharElemental
{
private:
	SI32 siFire; // 불
	SI32 siWater; // 물
	SI32 siLand; // 땅
	SI32 siWind; // 바람
	SI32 siNature; // 물리
	SI32 siFireA;
	SI32 siWaterA;
	SI32 siLandA;
	SI32 siWindA;

public:



	void Init()
	{
		siFire = 0 ;
		siWater = 0 ;
		siLand = 0 ;
		siWind = 0 ;
		siNature = 0 ;
		siFireA = 0;
		siWaterA = 0;
		siLandA = 0;
		siWindA = 0 ;
	}

	SI32 GetFire(){return siFire;}
	SI32 GetWater(){return siWater;}
	SI32 GetLand(){return siLand;}
	SI32 GetWind(){return siWind;}
	SI32 GetNature(){return siNature;}
	SI32 GetFireA(){return siFireA;}
	SI32 GetWaterA(){return siWaterA;}
	SI32 GetLandA(){return siLandA;}
	SI32 GetWindA(){return siWindA;}



	void Set(SI32 fire,SI32 water,SI32 land,SI32 wind,SI32 nature, SI32 fireA,SI32 waterA,SI32 landA ,SI32 windA)
	{
		siFire = fire;
		siWater = water;
		siLand = land ;
		siWind = wind ;
		siNature = nature ;
		siFireA = fireA;
		siWaterA = waterA;
		siLandA = landA ;
		siWindA = windA ;

	}
	void Set(cltCharElemental*pclinfo)
	{
		siFire = pclinfo->GetFire();
		siWater = pclinfo->GetWater();
		siLand = pclinfo->GetLand();
		siWind = pclinfo->GetWind();
		siNature = pclinfo->GetNature();
		siFireA = pclinfo->GetFireA();
		siWaterA = pclinfo->GetWaterA();
		siLandA = pclinfo->GetLandA();
		siWindA = pclinfo->GetWindA();
	}
};

//---------------------------------------
// cltKindInfo
//---------------------------------------
class cltKindInfo 
{
private:
	TCHAR 					szName[MAX_KIND_NAME_LENGTH];
	
public:
	SI32					siDollUnique;				// 이 캐릭터를 소환할 수 있는 인형 아이템의 유니크. 
	TCHAR					szCode[MAX_KIND_NAME_LENGTH];

	SI32					siPreferenceCharNum;
	SI32					siKindLifeTime;

	SI32					siDefaultCountry; // 속하는 국가 코드 . 소환영웅 및 PC만 사용한다.

	cltBasicAbility				clBA;				// 각 캐릭터가 가져야 할 기본 능력치.(향후 변경 되는 변수들)  
	cltBasicParametaBox			clBPB;				// 각 캐릭터가 가져야 할 기본 능력치.(한번 정해지면 변경되지 않는 변수들)  
	cltKindItemProbabilitySet	clItemPbtSet;		// 아이템을 가질 확률 
	cltDefaultWeaponInfo		clDefaultWeaponInfo;// 디폴트 무장 정보. 
	cltKIDrawInfo				clKIDrawInfo;		//출력관련 정보. 
	cltKIEffectInfo				clKIEffectInfo;		// 사운드 이펙트 정보. 	
	cltKindHireCondition		clHireCond;			// 마을에서의 고용 조건. 
	cltBossItemInfo				clBossItemInfo;		// 보스 캐릭터의 아이템 정보. 
	//cltAtkTypeTolerance			clAtkTypeTolerance; // 무기에 대한 내성

	SI32 					siMoveSpeed;
	SI32					siSize;					// 캐릭터의 사이즈. 
	
	SI32					siBlock;
						
	DWORD 					dwAttackInterval;
	SI32					siCriticalRate;		// 필살기 확률 
	SI32 					siAtb;
	SI32					siServiceAreaAtb;	// 사용되는 서비스 버전 속성

	DWORD 					DefenceAtb;
	SHORT 					VibrateSwitch;

	SHORT 					Exp;				
	SI32					siAggressive;		// 호전성. 
	SI32					siMagic;			// 사용하는 마법. 
	SI32					siSkillLevel;		// 스킬레벨 
	SI32					siSex;				// 성별. (1: 남자. 기타:여자.)

	SI32					siSortLevel;		// 등급 레벨
	SI32					siSummonRank;		// 소환수 등급
	SI32					siMagicLamp;		// 요술램프(전투퀘스트 용 몬스터레벨을 담고 있는 변수)

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SCROLL_SUMMON_PER_KIND>	siSummonScroll;
#else
	SI32					siSummonScroll[MAX_SCROLL_SUMMON_PER_KIND]; // 소환수에 바를 수있는 부적 코드
#endif
	
	SI32					siEvolveLevel;		// 진화 레벨
	SI16					siSummonPortrait;	// 소환수 인터페이스 초상화 
	SI16					siSummonJob;		// 소환수의 특수 기술 (낚시,농경술..)
	SI32					siSubSkillLevel;	// 소환수의 특수 기술 스킬 레벨
	
	BOOL					bSummonMall;		// 아이템몰에서 파는 소환수
	BOOL					bSummonTrade;		// 소환수 거래.<SetSummonEtcInfo()서만 > 
	SI32					siSummonStamina;	// 소환수의 스태미너 (PCK - 08.07.18)
	SI16					siSummonEquip;		// 소환수 장비 착용( 0=장비착용할수없음, 1=근거리, 2=원거리 )

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_ATTACKTYPE_PER_KIND>	siAttackList;
#else
	SI32					siAttackList[MAX_ATTACKTYPE_PER_KIND] ;	// 공격 애니동작 타입
#endif
	SI32					siMaxAttackType;

	cltCharAnimationManager clCharAniManager;

	cltKindChatInfo*		pclChatInfo;			// 채팅 정보. 

	cltWarChatInfo*			pclWarChatInfo;			// 전쟁시 채팅 정보
	
	cltCharElemental		pclCharElemental;		// 캐릭터 속성 관련
	// 포획 관련 
	SI32		siCaptureLevel;				// 포획에 필요한 기술 레벨 
	SI32		siBasicPbt;
	SI32		siAdvPbt;					// 포획술 1당 향상되는 포획확률. 

	cltKindInfo(TCHAR* szcode, 
				cltKIDrawInfo*				pclKIDrawInfo,
				cltKIEffectInfo*			pcleffectinfo, 
				cltKindHireCondition*		pclhireinfo,
				cltBossItemInfo*			pclBossItemInfo,
			/*	cltAtkTypeTolerance*		pclAtkTypeTolerance,*/
				SI32 movespeed, 
				SI32 sisize,
				SI32 siblock,
				DWORD atb, 
				DWORD ServiceAreaAtb,
				DWORD defenceatb,
				SHORT vibrateswitch, 
				TCHAR* name,
				SI32 countury = 0 );

	~cltKindInfo();


	void IncreasePreferenceCharNum(SI32 lifetime);	

	BOOL SetFile(SI32 Ani, BOOL DirSwitch, TCHAR* FileName,SI32 Font,SI32 Frame,SI32 Instant,
				 SI32 Delay, SI32 atb, SI32 EffectFont ,SI32 transmode = 0 ,SI08 alphavalue = 0 ,
				 bool attacktype = false, SI32 ShadowAni = 0 , SI32 ShadowFont = 0 );
	BOOL SetShadowIndentXY(SI32 anitype , SI32 x , SI32 y);

	SI32 GetAttackType();

	DWORD Add();

	SI32 GetPortraitFont() const;
	SI32 GetBottomIndent() const;
	SI32 GetXsizePercent() const;
	SI32 GetYsizePercent() const;

	const TCHAR*	GetName() const;

	// 아이템 정보를 분석한다. 
	bool AnalyzeItemInfo(TCHAR* szitemtext);

	bool IsAtb(SI32 atb);
	bool IsServiceAreaAtb(SI32 saAtb);
	SI32 GetCaptureRate(SI32 skilllevel);				// 포획확률을 구한다. 
	
	// 성황신을 위하 계산 
	bool CanDefulatVillageGod();						// 이 몬스터가 Defaut 인지를 검사한다.
	bool CanGodForVillage(SI32 villageunique);			// 이 몬스터가 성황신으로 될수있는지 
	SI32 GetResidentNumForVillageGod();					// 성황신이 되기위해 필요한 인구수 
	GMONEY GetMoneyForVillageGod();						// 성황신의 고용하는데 필요한 비용 
	GMONEY GetUpMoneyForVillageGod();						// 성황신의 업그레이드 하는데 필요한 비용 

	// NPC의 혼잣말을 설정한다. 
	void AddChat(TCHAR* ptext);
	void AddWarWinChat(TCHAR* ptext, SI32 mapindex);
	void AddWarChat(TCHAR* ptext, SI32 mapindex);

	// 랜덤하게 혼잣말을 구한다. 
	TCHAR* cltKindInfo::GetRandChatText();
	TCHAR* cltKindInfo::GetWarRandChatWinText(SI32 mapindex);
	TCHAR* cltKindInfo::GetWarRandChatText(SI32 mapindex);
};


#define SUMMON_JOB_NULL		0
#define SUMMON_JOB_FISHING	1
#define SUMMON_JOB_FARMING	2
#define SUMMON_JOB_MINING	3
#define MAX_SUMMON_JOB		4


class cltKindSummonJob
{
public:
	TCHAR szJobCode[MAX_SUMMON_JOB][30];
	TCHAR szJobName[MAX_SUMMON_JOB][30];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SUMMON_JOB>	siJobSkill;
#else
	SI32 siJobSkill[MAX_SUMMON_JOB];
#endif
	SI32 siCount;

	cltKindSummonJob();
	SI32 FindSummonJobFromJobCode(TCHAR *pJobCode);
	TCHAR *FindSummonJobNameFromJob(SI32 siJob);
	SI32 FindSummonJobSkillFromJob(SI32 siJob);
};

// PCK - 소환수 채팅 커맨드 추가 (08.07.22)
class cltSummonChatCommand
{
public:
	SI32	siChatTextLine;
	SI16	siAniType;
	SI32	siLevel;	// 채팅 커맨드를 사용가능한 소환수의 Level,
	SI16	siCharKind;	// 채팅 커맨드를 사용하는 소환수 Kind
	
	cltSummonChatCommand()
	{
		siChatTextLine	= 0;
		siAniType		= 0;
		siLevel			= 0;
		siCharKind		= 0;
	}
};

// PCK : /CMT명령어로 만들어야할 몬스터 정보 추가
class cltGMCommandKindInfo
{
public:
	cltBasicAbility				clBA;				// 각 캐릭터가 가져야 할 기본 능력치.(향후 변경 되는 변수들)  
	cltDefaultWeaponInfo		clDefaultWeaponInfo;// 디폴트 무장 정보. 

	DWORD 					dwAttackInterval;
	SI32					siCriticalRate;		// 필살기 확률 
	SI32					siAggressive;		// 호전성. 

	cltCharElemental		clCharElemental;	// 캐릭터 속성 관련
};

//------------------------------------------
// cltKindInfoSet
//------------------------------------------
class cltKindInfoSet
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltKindInfo*, MAX_KINDINFO_NUMBER>	pclKI;

	NSafeTArray<cltKindInfo*, MAX_KINDINFO_NUMBER>	pclSummonKI;	//[진성]소환수 정보 분리. => 2008-6-18

	NSafeTArray<cltSummonChatCommand*, MAX_SUMMON_CHATCOMMANDNUMBER>	pclSummonChatCommand;	// [춘기] 소환수 채팅 커맨드 추가 (08.07.22)

	NSafeTArray<cltGMCommandKindInfo*, MAX_KINDINFO_NUMBER>	pclGMCommandKindInfo;		// [춘기] /CMT명령어로 만드는 몬스터 정보
#else
	cltKindInfo* pclKI[MAX_KINDINFO_NUMBER];

	cltKindInfo* pclSummonKI[MAX_KINDINFO_NUMBER];					//[진성]소환수 정보 분리. => 2008-6-18

	cltSummonChatCommand*	pclSummonChatCommand[MAX_SUMMON_CHATCOMMANDNUMBER];	// [춘기] 소환수 채팅 커맨드 추가 (08.07.22)

	cltGMCommandKindInfo*	pclGMCommandKindInfo[MAX_KINDINFO_NUMBER];		// [춘기] /CMT명령어로 만드는 몬스터 정보
#endif

	cltKindSummonJob	clSummonJob;		// 소환수의 부가직업 
	cltKindInfoSet();
	~cltKindInfoSet();

	//------------------------------------
	// 게임에 사용되는 Block변수와 기타변수
	//------------------------------------
	SHORT BasicBlock		;
	SHORT ShallowWaterBlock	;
	SHORT DeepWaterBlock	;

	bool IsValidKind(SI32 kind);
	
	void AddCharKindInfoSetFromFile();

	SI32 FindKindFromName(TCHAR* npcname);							// NPC 이름을 근거로 Kind를 구한다. 
	SI32 FindKindFromCode(TCHAR* npccode);							// NPC Code를  근거로 Kind를 구한다. 
	void SetCharAttackInfo(TCHAR* datafilename);						// 캐릭터의 공격 속성을 설정한다.
	void SetCharEtcInfo(TCHAR * datafilename);						// 캐릭터의 기타 속성을 설정한다. - sortlevel , etc...
	void GetCharAttackInfo( SI32 siKind, TCHAR *pBuffer, SI16 txtSize );			// 캐릭터의 종류에 따른 공격 속성을 text로 얻어온다.	
	void SetCharItemInfo(TCHAR* datafilename);						// 캐릭터의 아이템을 설정한다. 

	// PCK - 소환수 채팅 커맨드 추가 (08.07.22)
	void SetSummonChatCommand();

	//[진성] 소환수 테이블 분리. => 2008-6-
	void SetSummonEtcInfo(TCHAR* datafilename);
	void SetSummonAttackInfo(TCHAR* datafilename);
	//void GetCharItemInfo( SI32 siKind, TCHAR *pBuffer );			// 캐릭터의 종류에 따른 아이템을 text로 얻어온다.

	SI32 GetSortLevel(SI32 kind);									// 몬스터 등급 레벨
	SI32 GetSubSkillLevel(SI32 kind,SI32 skill);					// 소환수의 서브 스킬

	bool GetItemMonsterExplain(SI32 itemunique, TCHAR* explaintext, SI16 txtSize);	// 특정 아이템을 주는 몬스터의 정보를 얻어온다. 
	bool GetItemMonsterExplain(SI32 itemunique, cltDropItem *dropitem, SI32 *siCount);

	bool GetItemFromKindExplain(SI32 kind, TCHAR* explaintext, SI16 txtSize );							// 특정 캐릭터가 주는 아이템의 정보를 얻어온다.
	bool GetItemFromKindExplain(SI32 kind, cltDropItem *dropitem, SI32 *siCount);		// 특정 캐릭터가 주는 아이템의 정보를 얻어온다.
	
	const TCHAR* GetName(SI32 kind);				// kind를 근거로 이름을 얻어온다. 
	void IncreasePreferenceChar( SI32 kind, SI32 lifetime );
	void ReadPreferenceCharData(TCHAR * filename);
	void WritePreferenceCharData(_SYSTEMTIME sTime);
	void SetDollInfo();							// 캐릭터의 소환 아이템을 설정한다. 
	SI32 GetDefaultVillageGod();				// 기본 성황신을 얻어온다.
	SI32 GetDefaultCastle();					// 기본 성문을  얻어온다.
	bool GetDailyQuestKindInfo(SI32* pkind, SI32*pbattlepower, SI32* pexp);			// 매일 퀘스트 대상 몬스터와 전투력을 랜덤하게 구한다. 
	bool GetDailyQuest2KindInfo(SI32* pkind, SI32*pbattlepower, SI32* pexp);		// 매일 퀘스트2 대상 몬스터와 전투력을 랜덤하게 구한다.
	
	GMONEY GetHireBasicMoney(SI32 kind);			// 고용 기본 비용 획득. 
	SI32 GetHireVillageLevel(SI32 kind);			// 고용 전제 마을 레벨
	bool FindGoodMonsterForSpecialQuest(SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo);	// 특수 임무의 사냥용으로 적합한 몬스터를 구한다. 
	//KHY - 0822 - 해피빈 퀘스트 추가.
	bool FindGoodMonsterForHappyBeanQuest(SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo);	// 특수 임무의 사냥용으로 적합한 몬스터를 구한다. 

	// 2010신년이벤트.
	bool FindGoodMonsterForRemoveUnLuckStatusQuest( SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo );	// 사냥 퀘스트용으로 적합한 몬스터를 구한다.
	bool Find_MakeLevel_ItemNum(SI32 level, SI32 makelevel);	// 캐릭터 레벨과 제조레벨에 맞는 무기, 장비 및 제조 수량을 구한다.

	void DeleteIfNotUse(DWORD curclock);			// 사용되지 않는 파일은 제거한다.
	bool IsSummonFromRank(SI32 rank);				// 주어진 랭크에 해당하는 소환수가 존재하냐? 다만 소환영웅일 경우엔 생각하지 않는다

	bool GetNPCImageFileAndFont(SI32 siKind, TCHAR* pszFileName, SI32 siLen, SI32* siFont);
	bool GetNPCImageFileAndFontsmall(SI32 siKind, TCHAR* pszFileName, SI32 siLen, SI32* siFont);

	SI16 GetAnitypeFromText(TCHAR* pszText, SI16 siKind, SI16 siLevel);
	SI32 GetTextMgrCanSummonChatCommand(SI16 siKind, SI16 siLevel);

	bool GetNPCGlobalImageIndexAndFont( IN SI32 siKind, OUT SI32* psiGimgIndex, OUT SI32* psiFont );

	// 군주데이미션
	bool FindGoodMonsterForGoonzuDay( SI32 level, SI32* pkind, SI32* pmonnum ); // 사냥 퀘스트용으로 적합한 몬스터를 구한다.
};

#endif