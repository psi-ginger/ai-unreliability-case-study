//---------------------------------
// 2003/7/4 김태곤
//---------------------------------

#ifndef _PERSONINFO_H
#define _PERSONINFO_H

//-------------------------------
// Common
//-------------------------------
#include "Quest\Quest.h"
#include "Quest/NamingQuest.h"
#include "Char\ParametaBox\ParametaBox.h"
#include "Char\CharCommon\Char-Economyinfo.h"
#include "Skill\Skill.h"
#include "Horse\Horse.h"

#include "Char\CharCommon\Char-Common.h"

//-------------------------------
// CommonLogic
//-------------------------------
#include "../CommonLogic/Bank/Bank.h"
#include "../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"
#include "../CommonLogic/WorldPassport/WorldPassport.h"

#include "../CommonLogic/MagicMgr/MagicBaseInfo.h"
#include "../CommonLogic/DormancyAccountSystem/DormancySystem.h"
#include "../CommonLogic/DormancyAccountSystem/DormancyInfo.h"

#define MAX_SUMMON_OWN		3							// 최대로 꺼낼수 있는 소환수 숫자.
#define MAX_HAVE_SUMMON		8							// 최대로 가질수 있는 소환수 숫자.
#define MAX_ELEMENTAL_OWN	3							// 최대로 가질수 있는 정령수 숫자

enum CHANNELING_USER
{
	CHANNELING_USER_NONE,
	CHANNELING_USER_JAPAN_NDE,
	CHANNELING_USER_JAPAN_HANGAME,
	CHANNELING_USER_JAPAN_GAMEPOT,
	CHANNELING_USER_JAPAN_GAMANIA,
	CHANNELING_USER_JAPAN_NEXON,
	CHANNELING_USER_ENGLISH_NDI,
	CHANNELING_USER_ENGLISH_USA,
};

class cltSummonScroll
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_SCROLL_SUMMON_PER_KIND>	siUnique;
	NSafeTArray<SI16, MAX_SCROLL_SUMMON_PER_KIND>	siPercent;
#else
	SI16 siUnique[MAX_SCROLL_SUMMON_PER_KIND];
	SI16 siPercent[MAX_SCROLL_SUMMON_PER_KIND];
#endif
	
	cltSummonScroll();
	cltSummonScroll(SI16 unique0, SI16 percent0, SI16 unique1, SI16 percent1);

	void Init();
	void Set(cltSummonScroll* pclSummonScroll);
	
	SI16 GetPercent(SI16 scroll);
	BOOL FindUnique(SI16 scroll);
	
	BOOL SetPercent(SI16 scroll, SI16 percent);
	BOOL AddPercent(SI16 scroll, SI16 percent);
	
	SI32 GetScrollRate(cltItemManagerCommon* pclItemManager, SI32 siScrollFunction);
	
	// GetEquipHitRate
	inline SI32 GetScrollHitRate(cltItemManagerCommon* pclItemManager)	
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_UPHITRATE);
	}
	// GetEquipCriticalHitRate
	inline SI32 GetScrollCriticalHitRate(cltItemManagerCommon* pclItemManager)	
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_UPCRITICAL);
	}
	// GetDamageReduceRate
	inline SI32 GetScrollDamageReduceRate(cltItemManagerCommon* pclItemManager)	
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_REDUCEDAMAGE);
	}
	// GetEquipPoisonRate
	inline SI32 GetScrollPoisonRate(cltItemManagerCommon* pclItemManager)	
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_POISON);
	}
	// GetEquipIceRate
	inline SI32 GetScrollIceRate(cltItemManagerCommon* pclItemManager)	
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_ICE);
	}
	// GetEquipDrainRate
	inline SI32 GetScrollDrainRate(cltItemManagerCommon* pclItemManager)
	{
		return GetScrollRate(pclItemManager, SCROLL_UNIQUE_FUNCTION_DRAIN);
	}
	
};

// [영훈] 소환수 한마리당 장비 아이템 정보
class cltSummonItemInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_SHAPE_ITEM >	clEquipItem;	// 소환수 장비 아이템
#else
	cltItem		clEquipItem[MAX_SHAPE_ITEM];				// 소환수 장비 아이템
#endif

	cltSummonItemInfo()		{ Init(); };
	~cltSummonItemInfo()	{};

	void Set(cltSummonItemInfo* pclSummonItemInfo)
	{
		memcpy ( this, pclSummonItemInfo, sizeof(cltSummonItemInfo) );
	};

	void Init()
	{
		ZeroMemory( this, sizeof(cltSummonItemInfo) );
	};


};

class cltSummon;

class cltSimpleSummon{
public:
	SI16				siKind;						// 소환수의 종류 
	SI16				siLevel;					// 소환수의 레벨
	SI32				siSummonID;					// 소환수ID
	TCHAR				szName[MAX_SUMMON_NAME];	// 이름
public:
	cltSimpleSummon();
	cltSimpleSummon(
		SI16 kind,
		SI16 Level,
		SI32 SummonID,				
		TCHAR* pname
	);

	void Init();
	void Set(cltSimpleSummon* pclSimpleSummon);
	void Set(cltSummon* pclSummon);
};

class cltSummon{
public:
	//SI08				siUnique;
	SI08				siStatus;
	SI08				siAttackType;
	SI08				siIdentity;
	SI16				siKind;						// 소환수의 종류 
	SI16				siAttackWearStoneNum;		// 무기 장비 착용석 갯수
	SI16				siDefenseWearStoneNum;		// 방어구 장비 착용석 갯수
	SI32				siTakeInMinute;
	SI32				siSummonID;					// 소환수ID
	TCHAR				szName[MAX_SUMMON_NAME];	// 이름 
	SI16				m_siTranformStamina;		// [진성] 소환수의 변신 스태미너.

	cltPIPInfoCommon	clIP;
	cltSummonScroll		clScroll;
	
public:

	cltSummon();
	cltSummon(
		SI32				SummonID,
		SI16				kind,			
		TCHAR*				pname, 
		SI08				status,		
		SI08				attacktype,	
		SI08				identity, 
		SI32				takeinminute,
		SI16				attackWearStoneNum,
		SI16				defenseWearStoneNum,
		cltPIPInfoCommon*	pclip,  
		cltSummonScroll*	pscroll,
		SI16				_siTranformStamina	);

	void Init();
	void Set( cltSummon* pclSummon );
};

//---------------------------------------------
// 소환수의 정보를 담을 클래스 
//---------------------------------------------
class cltSummonInfo
{
public:

	//SI16		siCurrentSummon;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltSummon,			MAX_SUMMON_OWN>		clSummon;
	NSafeTArray<cltSummonItemInfo,	MAX_SUMMON_OWN>		clItemInfo;					// 소환수 장비 아이템
	NSafeTArray<cltSimpleSummon,	MAX_HAVE_SUMMON>	clSummonBase;
#else
	cltSummon			clSummon[MAX_SUMMON_OWN];
	cltSummonItemInfo	clItemInfo[MAX_SUMMON_OWN];									// 소환수 장비 아이템
	cltSimpleSummon		clSummonBase[MAX_HAVE_SUMMON];
#endif

	SI16			siSelectAutoHeal;
	bool			bAutoHeal;

	bool			m_bIsSummonTranform;												// [진성] 소환수 변신 유무.
	SI16			m_siSummonTranformIndex;											// [진성] 변신 소환수 인덱스.
	DWORD			m_dwSummonTranformTickCount;										// [진성] 변신 후 재변신이나 회수시 제한시간.
	
public:

	cltSummonInfo();
	~cltSummonInfo();
	void Init();
	void Set(cltSummonInfo* pclSummoninfo);
	
	BOOL				CanHaveSummon();
	BOOL				GetEmptySummonIndex(SI32 *index);
	BOOL				GetEmptySummonIndexInHouse(SI32 *index); // 소환수 보관소에서 없는 Index를 찾는다
	BOOL				HaveSummon();
	void				SetSummon(SI16 index, cltSummon *pclSummon);
	cltSummon*			GetSummon(SI16 index);
	cltSimpleSummon*	GetSummonBase( SI16 index );
	cltSummon*			GetSummonFromSummonID(SI32 siSummonID);
	cltSimpleSummon*	GetSimpleSummonFromSummonID(SI32 siSummonID);
	void				SummonInit(SI16 index);
	SI08				GetSummonIndexFromSummonID(SI32 siSummonID);
	SI08				GetSummonBaseIndexFromSummonID(SI32 siSummonID);

	bool				IsSummonTranform( SI32 siTranformIndex );
	void				SetIsSummonTranform( bool _bIsSummonTranform );
	
	SI16				GetSummonTranformIndex(void);
	void				SetSummonTranformIndex(SI16 _siSummonTranformIndex);
	
	bool				IsSummonTranformTickCount( void );
	void				SetSummonTranformTickCount( void );
	void				EndSummonTranformTickCount( void );



	//-------------------------------------------
	// 소환수 아이템 관련
	//-------------------------------------------
	
	// 해당 소환수가 장비 아이템을 하나라도 같고 있는지 검사
	BOOL		IsHaveEquipItem( SI16 siSummonIndex );
	// 해당 소환수의 아이템 위치에 아이템을 Set한다
	void		SetSummonEquipItem( SI16 siSummonIndex, SI16 siItemPos, cltItem* pclItem );
	// 해당 소환수의 아이템 위치의 아이템을 Get한다
	cltItem*	GetSummonEquipItem( SI16 siSummonIndex, SI16 siItemPos );
	// 해당 소환수의 아이템배열 전부를 Get한다.
	cltItem*	GetSummonEquipItemList( SI16 siSummonIndex );

	// 해당 아이템 유니크의 장비가 있는지 검사
	BOOL		IsHaveEquipItemFromItemUnique( SI16 siSummonIndex, SI32 siItemUnique );
	// 해당 아이템 유니크가 몇개나 있는지
	SI32		GetHaveItemNumFromItemUnique( SI16 siSummonIndex, SI32 siItemUnique );
	// 해당 아이템이 어디에 있는지
	SI32		GetItemPosInEquip( SI16 siSummonIndex, SI32 siItemUnique );
	// 장비의 중독률 향상을 구한다(근거리 무기만)
	SI32		GetSummonEquipPoisonRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager, SI16 siActiveweapon);
	// 장비의 동빙한설확률 향상을 구한다(원거리 무기만)
	SI32		GetSummonEquipIceRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager, SI16 siActiveweapon);
	// 장비의 흡혈확률 향상을 구한다(신발만)
	SI32		GetSummonEquipDrainRate( SI16 siSummonIndex, cltItemManagerCommon* pclItemManager,SI16 siActive);
};

//***************************************************************************************************
//
// Class Name 		: cltPersonInfoOne
//
// Last Modified 	: 2007 / 03 / 08
// Created		 	: 김광명
//
// Function			: cltPersonInfo 내용을 분할1
//
//***************************************************************************************************
class cltPersonInfoOne
{
public:

	cltPBasicInfo			clBI;				// 캐릭터의 가장 기본이 되는 정보. 
	cltPIPInfo				clIP;				// 핵심적인 정보. 	
	cltPSkillInfo			clCharSkill;		// 스킬과 관련된 데이터 정보. 
//	cltPItemInfo			clCharItem;			// 아이템 보유 등과 관련된 정보. 
	cltSimpleRank			clCharRank;			// 신분관련 정보. 
	cltPItemMallItemInfo	clItemMall;			// 아이템 몰 아이템 정보.

	//-------------------------------
	// 경제 관련 정보들 
	//-------------------------------
	cltBankAccount			clBank;
	cltPStockInfo			clStock;
	cltPMarketInfo			clMarket;
	cltCharRealEstate		clRealEstate;
	cltWorldMoneyAccount	clWorldMoney;

	//------------------------------
	// 건강 정보. 
	//------------------------------
	cltHealthInfo		clHealthInfo;

	//----------------------------
	// 말 관련 정보. 
	//----------------------------
	cltPersonHorseInfo clHorseInfo;

	//----------------------------
	//퀘스트 관련 정보. 
	//----------------------------
	cltPQuestInfo clQuestInfo;

	//---------------------------
	// 전과 관련 정보
	//---------------------------
	cltGuiltyInfo	clGuiltyInfo;
	
	//--------------------------
	// 게임설정 정보. 
	//--------------------------
	cltPCharOptionInfo	clCharOption;

	//--------------------------
	// DailyQuest 정보. 
	//--------------------------
	cltPDailyQuestInfo clDailyQuestInfo;

	//--------------------------
	// DailyQuest2 정보. 
	//--------------------------
	cltPDailyQuest2Info clDailyQuest2Info;
	//--------------------------
	// DailyQuest3 정보. 
	//--------------------------
	cltPDailyQuest3Info clDailyQuest3Info;

	
	//--------------------------
	// 무역 서버 정보
	//--------------------------
	cltWorldOrder clWorldOrder;

	UI16 uiGachaCnt;

	//--------------------------
	// 캐릭터 명칭
	//--------------------------
	cltPersonNaming	clPersonNaming;

	//--------------------------
	// 무역관련 여권
	//--------------------------
	cltWorldPassport clWorldPassport;

	cltPersonInfoOne()
	{
		uiGachaCnt = 0;
	}

	~cltPersonInfoOne()
	{
	}

	void Set(cltPersonInfoOne* pclinfo)
	{
		clBI.Set(&pclinfo->clBI);
		clIP.Set(&pclinfo->clIP);
		clCharSkill.Set(&pclinfo->clCharSkill);
		clCharRank.Set(&pclinfo->clCharRank);
		clBank.Set(&pclinfo->clBank);
		clStock.Set(&pclinfo->clStock);

		clMarket.Set(&pclinfo->clMarket);
		clRealEstate.Set(&pclinfo->clRealEstate);
		clWorldMoney.Set(&pclinfo->clWorldMoney);

		clHealthInfo.Set(&pclinfo->clHealthInfo);
		clHorseInfo.Set(&pclinfo->clHorseInfo);
		clQuestInfo.Set(&pclinfo->clQuestInfo);
		clGuiltyInfo.Set(&pclinfo->clGuiltyInfo);
		clCharOption.Set(&pclinfo->clCharOption);
		clDailyQuestInfo.Set(&pclinfo->clDailyQuestInfo);
		clDailyQuest2Info.Set(&pclinfo->clDailyQuest2Info);
		clDailyQuest3Info.Set(&pclinfo->clDailyQuest3Info);


		//clSummonInfo.Set(&pclinfo->clSummonInfo);
		clWorldOrder.Set(&pclinfo->clWorldOrder);
		uiGachaCnt = pclinfo->uiGachaCnt;
		clPersonNaming.Set(&pclinfo->clPersonNaming );

		clWorldPassport.Set( &pclinfo->clWorldPassport );
	}
};

//***************************************************************************************************
//
// Class Name 		: cltPersonInfoTwo
//
// Last Modified 	: 2007 / 03 / 08
// Created		 	: 김광명
//
// Function			: cltPersonInfo 내용을 분할2 새로운 정보 추가는 여기에 합시다.
//					  cltPersonInfo.SetTwo()-> 뒤에 넣어주어야 합니다. 
//
//***************************************************************************************************
class cltPersonInfoTwo
{
public:
	//------------------------------
	// 아이템 정보. 
	//------------------------------
	cltPItemInfo			clCharItem;			// 아이템 보유 등과 관련된 정보. 
	//--------------------------
	// 소환수 정보. 
	//--------------------------
	cltSummonInfo		clSummonInfo;

	cltPersonInfoTwo()
	{
	}

	~cltPersonInfoTwo()
	{
	}

	void Set(cltPersonInfoTwo* pclinfo)
	{
		clCharItem.Set(&pclinfo->clCharItem);
		clSummonInfo.Set(&pclinfo->clSummonInfo);
	}
};

//-----------------------------------------------
// 사용자 캐릭터의 정보를 모두 담을 클래스 
// 이 클래스가 DB에 저장되는 캐릭터 정보가 된다. 
//------------------------------------------------
class cltCharServer;
class cltPersonInfo
{
public:

	cltPBasicInfo			clBI;				// 캐릭터의 가장 기본이 되는 정보. 
	cltPIPInfo				clIP;				// 핵심적인 정보. 	
	cltPSkillInfo			clCharSkill;		// 스킬과 관련된 데이터 정보. 
	cltPItemInfo			clCharItem;			// 아이템 보유 등과 관련된 정보. 

	//[추가 : 황진성 2008. 1. 29 => 공간 상자와 보물상자의 아이템 정보.]
//	cltSpaceBox				clSpaceBoxItem;		// 아이템 보유 등과 관련된 정보.
//	cltTreasureBox			clTreasureBoxItem;	// 아이템 보유 등과 관련된 정보.

	cltSimpleRank			clCharRank;			// 신분관련 정보. 
	cltPItemMallItemInfo	clItemMall;			// 아이템 몰 아이템 정보.

	//-------------------------------
	// 경제 관련 정보들 
	//-------------------------------
	cltBankAccount			clBank;
	cltPStockInfo			clStock;
	cltPMarketInfo			clMarket;
	cltCharRealEstate		clRealEstate;
	cltWorldMoneyAccount	clWorldMoney;

	//------------------------------
	// 건강 정보. 
	//------------------------------
	cltHealthInfo		clHealthInfo;

	//----------------------------
	// 말 관련 정보. 
	//----------------------------
	cltPersonHorseInfo clHorseInfo;

	//----------------------------
	//퀘스트 관련 정보. 
	//----------------------------
	cltPQuestInfo clQuestInfo;

	//---------------------------
	// 전과 관련 정보
	//---------------------------
	cltGuiltyInfo	clGuiltyInfo;

	//--------------------------
	// 게임설정 정보. 
	//--------------------------
	cltPCharOptionInfo	clCharOption;
	
	//--------------------------
	// DailyQuest 정보. 
	//--------------------------
	cltPDailyQuestInfo clDailyQuestInfo;

	//--------------------------
	// DailyQuest2 정보. 
	//--------------------------
	cltPDailyQuest2Info clDailyQuest2Info;

	//--------------------------
	// DailyQuest3 정보. 
	//--------------------------
	cltPDailyQuest3Info clDailyQuest3Info;

	//--------------------------
	// 소환수 정보. 
	//--------------------------
	cltSummonInfo		clSummonInfo;
	
	//--------------------------
	// 무역 서버 정보
	//--------------------------
	cltWorldOrder clWorldOrder;
	
	UI16 uiGachaCnt;

	//--------------------------
	// 캐릭터 명칭
	//--------------------------
	cltPersonNaming	clPersonNaming;

	//--------------------------
	// 무역관련 여권
	//--------------------------
	cltWorldPassport	clWorldPassport;

	//--------------------------
	// 휴면 계정 시스템 관련
	//--------------------------
	CDormancyInfo	clDormancyInfo;


	cltPersonInfo()
	{
		uiGachaCnt = 0;
	}

	~cltPersonInfo()
	{
	}

	void Set(cltPersonInfo* pclinfo)
	{
		clBI.Set(&pclinfo->clBI);
		clIP.Set(&pclinfo->clIP);
		clCharSkill.Set(&pclinfo->clCharSkill);
		clCharItem.Set(&pclinfo->clCharItem);
		clCharRank.Set(&pclinfo->clCharRank);
		clBank.Set(&pclinfo->clBank);
		clStock.Set(&pclinfo->clStock);
		clMarket.Set(&pclinfo->clMarket);
		clRealEstate.Set(&pclinfo->clRealEstate);
		clWorldMoney.Set(&pclinfo->clWorldMoney);

		clHealthInfo.Set(&pclinfo->clHealthInfo);
		clHorseInfo.Set(&pclinfo->clHorseInfo);
		clQuestInfo.Set(&pclinfo->clQuestInfo);
		clGuiltyInfo.Set(&pclinfo->clGuiltyInfo);
		clCharOption.Set(&pclinfo->clCharOption);
		clDailyQuestInfo.Set(&pclinfo->clDailyQuestInfo);
		clDailyQuest2Info.Set(&pclinfo->clDailyQuest2Info);
		clDailyQuest3Info.Set(&pclinfo->clDailyQuest3Info);
		clSummonInfo.Set(&pclinfo->clSummonInfo);
		clWorldOrder.Set(&pclinfo->clWorldOrder);
		uiGachaCnt = pclinfo->uiGachaCnt;
		clPersonNaming.Set(&pclinfo->clPersonNaming);

		clWorldPassport.Set( &pclinfo->clWorldPassport );
		clDormancyInfo.Set( &pclinfo->clDormancyInfo);
	}

	void SetPersonID(SI32 id)
	{
		clBI.clPerson.siPersonID = id;
	}

	SI32 GetPersonID() const
	{
		return clBI.clPerson.siPersonID;
	}

	// cltPersonInfoOne의 정보를 셋팅한다
	void SetOne(cltPersonInfoOne* pclinfo)
	{
		clBI.Set(&pclinfo->clBI);
		clIP.Set(&pclinfo->clIP);
		clCharSkill.Set(&pclinfo->clCharSkill);
		clCharRank.Set(&pclinfo->clCharRank);
		clBank.Set(&pclinfo->clBank);
		clStock.Set(&pclinfo->clStock);
		clMarket.Set(&pclinfo->clMarket);
		clRealEstate.Set(&pclinfo->clRealEstate);
		clWorldMoney.Set(&pclinfo->clWorldMoney);

		clHealthInfo.Set(&pclinfo->clHealthInfo);
		clHorseInfo.Set(&pclinfo->clHorseInfo);
		clQuestInfo.Set(&pclinfo->clQuestInfo);
		clGuiltyInfo.Set(&pclinfo->clGuiltyInfo);
		clCharOption.Set(&pclinfo->clCharOption);
		clDailyQuestInfo.Set(&pclinfo->clDailyQuestInfo);
		clDailyQuest2Info.Set(&pclinfo->clDailyQuest2Info);
		clDailyQuest3Info.Set(&pclinfo->clDailyQuest3Info);
		clWorldOrder.Set(&pclinfo->clWorldOrder);
		uiGachaCnt = pclinfo->uiGachaCnt;
		clPersonNaming.Set(&pclinfo->clPersonNaming);

		clWorldPassport.Set( &pclinfo->clWorldPassport );
	}

	// cltPersonInfoTwo의 정보를 셋팅한다
	void SetTwo(cltPersonInfoTwo* pclinfo)
	{
		clCharItem.Set(&pclinfo->clCharItem);
		clSummonInfo.Set(&pclinfo->clSummonInfo);
	}
};



//-------------------------------------------
// 주인공 캐릭터 정보 백업용 데이터. 
//-------------------------------------------
class cltHeroDataBackup{
public:
	cltPersonInfo			clMyCharPersonInfo;
	SI32					siWarEnemyKillNumber;// 전쟁에서 처치한 적의 수도 백업. 
	SI32					siWarKilledNumber;		//전쟁에서 죽은 수의 백업. 
	SI32					siBlackArmyWarMapIndex;
	SI32					siBlackWarEnemyKillNumber;	// 흑의군단 전쟁에서 처치한 적의 수도 백업. 
	SI32					siBlackWarKilledNumber;		// 흑의군단 전쟁에서 죽은 수의 백업. 

	//[추가 : 황진성 2008. 1. 28 => 흑의 군단 개편 정보 백업]
	SI32					siBlackWarEnemyKillScore;			// [황진성 2008. 1. 28 => 전쟁에서 처치한 적의 점수.]
	SI32					siBlackWarEnemyKillVillageScore;	// [황진성 2008. 1. 28 => 전쟁에서 처치한 적에 해당하는 내 마을 점수.]

	bool					bStartMakeItemSwitch;
	DWORD					dwStartMakeItemClock;
	DWORD					dwDelayMakeItemClock;
	
	SI32                    siOldActiveClothes;
	SI32                    siOldActiveHat;
	SI32					siOldActiveWeapon;

	SI32					siOldCurrnetMagic;
	SI08					siOldCurrnetMagicCastType;

	SI32					siOldBasicMoveSpeed;
	SI32					siOldShoesMoveSpeed;
	SI32					siOldTotalMoveSpeed;
	SI32					siOldGivenMoveSpeed;	// 서버로부터 주어진 이동 속도. 

/*
	SI32					siGuildOldStr;			// 길드 힘
	SI32					siGuildOldDex;			// 길드 순발
	SI32					siGuildOldMag;			// 길드 도력
	SI32					siGuildOldVit;			// 길드 체력
	SI32					siGuildOldHnd;			// 길드 손재주
	SI32					siGuildOldLuck;			// 길드 운
*/
	cltBasicAbility			clGuildOldBA;

	
	cltInnerStatusInfo		clOldInnerStatus;
	cltCharAddStatusInfo	clOldAddInfo;
	cltPItemMallItemInfo	clOldItemMall;
//	cltPLimitDateItemInfo	clOldLimitDateItemInfo;

	cltSpaceBox				clSpaceBoxItem;		// 아이템 보유 등과 관련된 정보.
	cltTreasureBox			clTreasureBoxItem;	// 아이템 보유 등과 관련된 정보.

	CNMagicBufMgr			clOldMagicBufMgr;	// 버프 마법 정보

	SYSTEMTIME				stMuteTime;

	// 발렌타인데이 전쟁 정보
	cltValentineWarInfo		clValentinewarinfo;

	cltPVP_LeagueInfo		clPVP_LeagueInfo;
	
	cltHeroDataBackup();
	~cltHeroDataBackup();
	
};

SI32 GetChannelingUser(TCHAR* pszID);

#endif


