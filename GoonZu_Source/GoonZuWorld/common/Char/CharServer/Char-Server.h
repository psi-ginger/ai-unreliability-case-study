//---------------------------------
// 2003/10/16 김태곤
//---------------------------------
#ifndef _CHARSERVER_H
#define _CHARSERVER_H

#include "Char\CharCommon\Char-Common.h"
#include "../Client/MiniGame/Roulette/RouletteLogic.h"

#include "../../Agriculture/Agriculture.h"

#include "../../NLib/NUpdateTimer.h"

#define CHAT_INPUT_MIN_CLOCK	400				// 채팅 입력의 최소 간격. 

#define CHARMODE_NONE				0
#define CHARMODE_INSTANCEMAP		1			// 인스턴스 던전용 캐릭터
#define CHARMODE_BLACKWAR			2			// 흑의군단 전쟁용 캐릭터. 
#define CHARMODE_SUMMON_EGG2		3			// 수수깨끼소환알2로 소환된 캐릭터
#define CHARMODE_KINGGURI			4			// 대왕구리댁
#define CHARMODE_SANTARACCOON		5			// [추가 : 황진성 2007. 11. 29 산타라쿤 모드 추가.]
#define CHARMODE_TREASUREEVENT		6			// // 보물찾기 이벤트 유령NPC 영웅유령NPC가 죽었을때.
#define CHARMODE_BATTLEROYAL		7			// 배틀로얄의 캐릭터

#define KILLEDMODE_BLACKWAR			1			// 흑의군단 이벤트에서 사망. 
#define KILLEDMODE_VILLAGEWAR		2			// 공성전에서 사망. 
#define KILLEDMODE_BEGINNERMAP		3			// 초보자 사냥터 사망. 
#define KILLEDMODE_VILLAGEHUNT		4			// 불가사리에게 사망. 
#define KILLEDMODE_BLACKARMY		5			// 흑의군대 이벤트에서 사망
#define KILLEDMODE_MOFUMOFUEVENT	6			// 모후모후 이벤트에서 사망
#define KILLEDMODE_GUILDWAR			7			// 길드전에서 사망
#define KILLEDMODE_PVP				8			// PVP에서 사망
#define KILLEDMODE_INSTANCEMAP		9			// 인스턴스맵에서 사망
#define KILLEDMODE_GOTOSAFEZONE		10			// 엔펠리스로 보내야 하는 사망이다.
#define KILLEDMODE_BATTLEROYAL		11			// 배틀로얄에서 사망

#define MINING_DECREASEORG_CNT		2
#define FARMING_DECREASEORG_CNT		4

#define MINING_DECREASEORG_AMT		5
#define FARMING_DECREASEORG_AMT		5
#define ANTIDOTES_DECREASEORG_AMT	3			// 해독제 사용시 신경기관 -3 만큼의 효과를 받는다.

#define REMOVEUNLUCK_QUESTNPC_COUNT	9			// [지연] : 2010신년이벤트 - 세계관 퀘스트 주는 npc의 총 마리수

#ifdef _TAIWAN
	#define	GAMEGUARD_GAMESTART_CERTIFICATION	2*60+30	// 2분 30초
	#define GAMEGUARD_CERTIFICATION				1*60	// 1분
#else
	#define	GAMEGUARD_GAMESTART_CERTIFICATION	50	// 50초
	#define GAMEGUARD_CERTIFICATION				25	// 25초
#endif

class cltGameMsgRequest_MakeItemInv;
class DBToolInfo;
class cltSummon;
class cltAccountLetterUnit;
class cltSummonTransformManager;
class cltCharManager;

//-----------------------------------------
// 캐릭터의 업데이트 정보. 
//-----------------------------------------
class cltCharUpdateInfo{
public:
	bool bMode;
	SI32 siPara1;

	cltCharUpdateInfo();
	void Init();
};

//-----------------------------------------
// 캐릭터의 군주미션 수행 여부에 대한 퀘스트 정보. 
//-----------------------------------------
class cltCharGoonzuDayMissionInfo
{
public:
	UI16 uiYear;			
	UI08 uiMonth;			
	UI08 uiDate;			
	UI16 uiCount;			// 미션 카운트

	cltCharGoonzuDayMissionInfo()
	{
		ZeroMemory( this, sizeof(cltCharGoonzuDayMissionInfo) );
	}

	cltCharGoonzuDayMissionInfo(UI16 year, UI08 month, UI08 date, UI16 count)
	{
		ZeroMemory( this, sizeof(cltCharGoonzuDayMissionInfo) );

		uiYear = year;
		uiMonth = month;
		uiDate = date;
		uiCount = count;
	}

	void Clear()
	{
		ZeroMemory( this, sizeof(cltCharGoonzuDayMissionInfo) );
	}
};

//-----------------------------------------
// 사용되는 아이템 정보 보관. 
//-----------------------------------------
class cltItemReserveUnitInfo{
public:
	SI16 siUnique;
	SI16 siItemNum;
	SI16 siItemPos;
	
	cltItemReserveUnitInfo();
	void Init();
	void Set(cltItemReserveUnitInfo* pclinfo);
};

#define MAX_USEITEM_RESERVE_NUM 128
class cltUseItemReserveInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemReserveUnitInfo, MAX_USEITEM_RESERVE_NUM>	clInfo;
#else
	cltItemReserveUnitInfo clInfo[MAX_USEITEM_RESERVE_NUM];
#endif

	void Init();
	bool Push(SI32 unique, SI32 itemnum, SI32 itempos= -1);
	bool IsReserved();

};

class cltBulletUseReserveInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemReserveUnitInfo, MAX_USEITEM_RESERVE_NUM>	clInfo;
#else
	cltItemReserveUnitInfo clInfo[MAX_USEITEM_RESERVE_NUM];
#endif

	void Init();
	bool Push(SI32 unique, SI32 itemnum, SI32 itempos);
	bool IsReserved();

};

//-----------------------------------------
// 소환수 아이템 예약관련
//-----------------------------------------
class cltSummonItemReserveUnitInfo{
public:
	SI08 m_siSummonIndex;
	SI16 m_siItemUnique;
	SI16 m_siItemNum;
	SI16 m_siItemPos;

	cltSummonItemReserveUnitInfo();
	cltSummonItemReserveUnitInfo( SI08 siSummonIndex, SI16 siItemUnique, SI16 siItemNum, SI16 siItemPos );
	void Init();
	void Set( cltSummonItemReserveUnitInfo* pclinfo );
	void Set( SI08 siSummonIndex, SI16 siItemUnique, SI16 siItemNum, SI16 siItemPos );

};

class cltSummonBulletUseReserveInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltSummonItemReserveUnitInfo, MAX_USEITEM_RESERVE_NUM>		m_clInfo;
#else
	cltSummonItemReserveUnitInfo m_clInfo[MAX_USEITEM_RESERVE_NUM];
#endif

	cltSummonBulletUseReserveInfo();
	void Init();
	bool Push( SI08 siSummonIndex, SI16 siItemUnique, SI16 siItemNum, SI16 siItemPos );
	bool IsReserved();

};


//---------------------------------------------
// 캐릭터용 간단 변수들..
//---------------------------------------------
class cltCharSimpleVary{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<DWORD, MAX_ITEM_PER_PERSON>		dwItemInputReserveClock;
#else
	DWORD	dwItemInputReserveClock[MAX_ITEM_PER_PERSON];	//  아이템 추가를 위한 예약 정보. 
#endif
	bool	bCheckFameLevelSwitch;					// 명성레벨 갱신 여부. 
	
	bool	bSkillExpUpdateSwitch;							// 기술 경험치가 업데으트 된것이 하나라도 있는가. 
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_SKILL_NUMBER>			SkillUpdateSwitch;
#else
	bool	SkillUpdateSwitch[MAX_SKILL_NUMBER];			// Skill Update (스킬 경험치가 바뀌는지 검사)
#endif

	// 내구도 Update정보 
	bool	bWasteDurSwitch;								// 내구도 닳은 물품이 하나라도 있다면. 
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_ITEM_PER_PERSON>		WasteDurUpdateSwitch;
#else
	bool	WasteDurUpdateSwitch[MAX_ITEM_PER_PERSON];
#endif

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	// 내구도 Update정보 
	bool	bCrystalDurSwitch;								// 내구도 닳은 물품이 하나라도 있다면. 
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_SHAPE_ITEM>			CrystalDurUpdateSwitch;
#else
	bool	CrystalDurUpdateSwitch[MAX_SHAPE_ITEM];
#endif

	// LEEKH - SummonItem Durability Add
	NSafeTArray<BOOL, MAX_SUMMON_OWN>										bSummonWasteDurSwitch;
	NSafeTArray< NSafeTArray<bool, MAX_ITEM_PER_PERSON>, MAX_SUMMON_OWN>	SummonWasteDurUpdateSwitch;

	DWORD	dwLastHorseActionClock;							// 말 관련 액션을 취한 마지막 시간. 

	DWORD dwlastCheckCharServerStatusActionClock;
	DWORD dwLastCheckCharServerNPCActionClock;
	DWORD dwlastCheckHorseSpeedQuestClock;
	DWORD dwLastCheckCharServerActionPCClock;
	DWORD dwLastKillClock;		// 다른 캐릭터를 마지막으로 처치한 시간. (자동사냥 방지를 위한 장치) 
	SI32  siCheckMonsterNum;							// 죽인 몬스터 숫자 누적(매크로)
	DWORD dwRealKillClock;							// 매크로 시간 체크 
	DWORD dwGuildPointClock;							// 접속시간 - 길드 GP에 사용
	DWORD dwLastGameGuardCheckClock;				// nProtect 인증 체크 
	
	//bool	bDemandAttend;						// 출석체크 요구 하였는지.

	//-----------------------------------
	// 로그인후 보호 
	//-----------------------------------
	bool	bInitProtectSwitch;
	DWORD	dwInitProtectClock;

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_ITEM_PER_PERSON>		bNoMoveItemSwitch;
#else
	bool	bNoMoveItemSwitch[MAX_ITEM_PER_PERSON];	// 아이템 사용이 예약되었으니까 움직이면 안된다 -  TradeServer 전용!
#endif

	cltCharSimpleVary();
	void Init();
};

enum
{
	CHAR_UPDATETIMER_TIMEMETERRATE_DB,
	CHAR_UPDATETIMER_TIMEMETERRATE_CLIENT,
	CHAR_UPDATETIMER_TRANSFORMINFO_CLIENT,
	CHAR_UPDATETIMER_TRANSFORM_TIME_LIMIT,

	CHAR_UPDATETIMER_CRYSTALITEM_DB,
	CHAR_UPDATETIMER_CRYSTALITEMINFO_CLIENT,

	CHAR_UPDATETIMER_TIREDSYSTEM_TIME_LIMIT,

	CHAR_UPDATETIMER_30SECOND,
	CHAR_UPDATETIMER_3SECOND,

	CHAR_UPDATETIMER_BUGCHAR_CHECK,

	//---------------------------------------------
	CHAR_UPDATETIMER_END
};

// 아이템을 줄때 아이템 밀봉/해제 여부를 설정한다
enum GIVEITEM_SEAL_MODE
{
	GIVEITEM_SEAL_MODE_NONE = 0,	// 아무것도 아님
	GIVEITEM_SEAL_MODE_SEAL,		// 밀봉한다
	GIVEITEM_SEAL_MODE_CANCEL		// 밀봉을 해제한다
};

//---------------------------------------------
// 캐릭터용 UpdateTimer
//---------------------------------------------
class cltCharUpdateTimer
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<NUpdateTimer, CHAR_UPDATETIMER_END>		m_clTimer;
#else
	NUpdateTimer		m_clTimer[CHAR_UPDATETIMER_END];
#endif

public:
	cltCharUpdateTimer();
	~cltCharUpdateTimer()	{}

	void Init();

	NUpdateTimer& operator [] (SI32 siIndex)
	{
		return m_clTimer[siIndex];
	}
};

#ifdef USE_GAME_GUARD_SERVER

	enum CCAAUTH_STATE
	{
		CCSAUTH_STATE_NONE,
		CCSAUTH_STATE_INIT,
		CCSAUTH_STATE_FIRST_AUTH_SENDED,
		CCSAUTH_STATE_FIRST_AUTH_RECEIVED,
		CCSAUTH_STATE_N_AUTH_SEND,
		CCSAUTH_STATE_N_WAIT_NEXTTIME,
	};

#endif

class cltAgriculture
{
private:
	SI32	m_siVillageUnique[MAX_AGRICULTURE_COUNT];
	UI32	m_uiExpireTime[MAX_AGRICULTURE_COUNT];

	DWORD	m_dwStartAgricultureClock;		// 생산 활동 시작시간
	SI32	m_siCurrentVillageUnique;		// 현재 생산활동을 하고 있는 마을 유니크

public:
	cltAgriculture()
	{
		Init();
	}

	~cltAgriculture()
	{
	}

	void Init()		{		ZeroMemory(this, sizeof(cltAgriculture));		}

	void AddAgriculture( SI32 siVillageUnique, UI32 uiExpireTime );
	void DeleteAgriculture( SI32 siVillageUnique );
	UI32 GetExpireTime( SI32 siVillageUnique );
	bool IsLeaseAgriculture( SI32 siVillageUnique );

	bool StartAgriculture( SI32 siVillageUnique, DWORD dwStartClock );
	bool EndAgriculture( SI32 siVillageUnique );

	SI32  GetCurrentVillageUnique()		{		return m_siCurrentVillageUnique;	}
	DWORD GetStartAgricultureClock()	{		return m_dwStartAgricultureClock;	}

protected:
	SI32 FindVillageUnique( SI32 siVillageUnique );
};

//------------------------------------------
// cltCharServer : Server용 
//------------------------------------------
class cltCharServer	: public cltCharCommon
{
private:
	DWORD			LastReceveFromClientClock;	// 클라이언트로부터 메시지를 받은 마지막 시간. 
	DWORD			dwLastCheckAutoFoodClock;	// 마지막으로 자동 먹이기능 체크 시간. 

	SI32			NPCGroup;
	cltWarpInfo		clCharWarpInfo;		// 워프해야 하는가. 
	SI32			siWarpUnique;		// 어떤 종류의 워프를 했는지 식별자. 	

	bool			bSendSecretNCHarUniqueSwitch;	// 캐릭터 생성후에 암호화키와 캐릭터 유니크 값을 클라이언트에 보내는지 여부.
	
	SI32			siMagicDelay;					// 마법 지연 시간. 
	

	cltCharSimpleVary clSimpleVary;

	//----------------------------------------
	// 이전 레벨 정보. 
	//----------------------------------------
	DWORD			uiLevelInfoUpdateFrame;
	cltLevelInfo	clOldLevelInfo;				// 이전 레벨 정보. 
	
	DWORD	dwLastVisitVillageClock;			// 마지막으로 마을을 방문한 시간. 
	
public:

	//----------------------------------------
	// 게임 가드용 멤버들
	//----------------------------------------
#ifdef USE_GAME_GUARD_SERVER
	//cltWrapCCSAuth2	clWarpCSA;

	CCSAuth2		m_clCSA;

	bool			m_bCCSAuthInit;
	SI32			m_siCCSAuthState;

	DWORD			m_dwCCSAuthLastActionTick;

	SI32			m_siCCSAuthCount;

	void CCSAuthInit();
	void CCSAuthClose();

	void CCSCloseClient();

	void CCSReceivedAuth(GG_AUTH_DATA* ggData);

	void CCSAuthRun();

	void CCSAuthWriteProcessLog(TCHAR* pszMsg);
	void CCSAuthWriteErrorLog(TCHAR* pszMsg);

#endif

	UI32			uiCSAAuth_Count;
	DWORD			dwLastGameGuardAuthClock;
	bool			bWaitGameGuardClientAuth;

	DWORD 			dwTimerClock;					// 생존 기간이 정해져 있는 캐릭터임. 
	SI32			siOldWeaponMagic;				// 이전에 선택되었던 마법 기억.

	// 클라이언트가 모든 준비를 마쳤는가 ? 
	// 서버는 바로 이 캐릭터를 만들어서 행동할 수 있지만 
	// 클라이언트는 로드를 포함한 일련의 작업이 
	// 필요한데 그러한 작업이 완료되면 클라이언트가 완료 신호를 서버로 
	// 보내게 되고 서버는 이 스위치를 작동시킨다. 
	// 이 스위치가 작동된 이후에야 서버에서 이 캐릭터에 대한 행동이 이루어지게 된다. 
	bool	bReadySwitch;	

	// 이 캐릭터가 서버에서 생기면 한번 셀에 대한 정보를 업데이트 하는 작업을 수행해야 한다. 
	// 이작업은 이 캐릭터 일생에 한번만 이루어지게 된다. 물론 캐릭터의 이동에 따라 셀정보의 
	// 업데이트는 수시로 이루어지지만 최초 셀정보 관련 작업은 한번만 이루어지는 것이다. 
	// 이 작업이 한번만 이루어짐을 보장하는 변수. 
	bool	bFirstCellOpSwtich;
	
	//-----------------------------------
	// 한 유저당 갖게 되는 3개 캐릭터 정보. 
	//-----------------------------------
	//cltCharListInfo clCharListInfo[MAX_CHAR_PER_USER];
	cltCharListInfo* pclCharListInfo;;

	// 업데이트 정보. 
#ifdef _SAFE_MEMORY
	NSafeTArray<class cltCharUpdateInfo, MAX_UPDATE_SWITCH>		clUpdateInfo;
#else
	class cltCharUpdateInfo	clUpdateInfo[MAX_UPDATE_SWITCH];
#endif

	//----------------------------
	// 사망 관련 
	//---------------------------
	bool bDeathPenaltySwitch;
	bool bRequestReviveSwitch;
	clock_t DeathClock;
	SI32 siCharDyingDelay;

	//-----------------------------
	// 자동 회복 관련 
	//-----------------------------
	DWORD			dwAutoRecoverLifeClock;		// 마지막으로 자동 회복이 되었던 시간. 
	DWORD			dwAutoRecoverManaClock;		// 마지막으로 자동 회복이 되었던 시간. 

	//-----------------------------
	// 고양이 쿠션 자동 회복 관련  Cushion
	//-----------------------------	
	DWORD			dwCusionRecoverLifeClock;		// 마지막으로 자동 회복이 되었던 시간. 
	DWORD			dwCusionRecoverManaClock;		// 마지막으로 자동 회복이 되었던 시간. 

	DWORD			dwCusionRecoverOragansClock;	// 마지막으로 기관 회복이 되었던 시간. 
	DWORD			dwCusionFallSatisfiedClock;		// 마지막으로 포만감 하락이 되었던 시간.

	// PCK : 잠자는 냐옹이 쿠션 소환수 자동 회복 관련 (09.06.15)
	DWORD			dwCusionRecoverSummonLifeClock;		// 마지막으로 소환수 자동 회복이 되었던 시간. 
	DWORD			dwCusionRecoverSummonManaClock;		// 마지막으로 소환수 자동 회복이 되었던 시간. 

	//KHY - 1220 - 변신시스템. - 자동회복
	DWORD			dwTransFormRecoverManaClock;		// 마지막으로 자동 회복이 되었던 시간. 

	// 타이머
	cltCharUpdateTimer m_clCharUpdateTimer;

public:
	SHORT 			WaitCount;
	SHORT 			Wait;
		
	SI32			RepeatNumber;

	//----------------------------
	// 머릿말 
	//----------------------------

	bool bHeadChatSwitch;		// 현재 머릿말이 설정되어 있는가?
	TCHAR szHeadChatText[MAX_HEAD_CHAT_TEXT_SIZE];

	//----------------------------
	// 개인상점 데이터 
	//----------------------------
	cltShopBannerData	clShopBanner;
	cltShopTradeData	clShopData;

	//----------------------------
	// 수리요청 데이터
	//----------------------------
	cltShopRepairData*	m_pclShopRepairData;

	//----------------------------
	// 캐릭터 모드. 
	//----------------------------
	SI16	siCharMode;				// 이 캐릭터가 어떤 모드로 작동하는가 ?(왜구전쟁, .. ) 
	SI16	siCharModePara1;		// 캐릭터 모드용 파라메타. 

	SI16	siKilledMode;							// 사망 모드. 
	TCHAR	szKillMeCharName[MAX_PLAYER_NAME];		// 나를 죽인 캐릭터의 이름. 


	//-------------------------
	// 제자리에서 대기 
	//-------------------------
	bool bStaySwitch;				// 한자리에 멈춰서 있는지 여부. 

	//-------------------------
	// 플레이 시간 구하기 
	//-------------------------
	DWORD dwStartClock;
	DWORD dwInstantPlayTime;		// 서버 접속후 플레잉타임(DB저장안됨)
	
	//-------------------------
	// 게임에 접속한 시각 기록
	//-------------------------
	_SYSTEMTIME sTimeStartTime;
	
	//-------------------------
	// 게임 로그인했을때 자신의레벨 기록
	//-------------------------
	SI32 siLoginedLevel;
	//-------------------------
	// 지휘효과. 
	//-------------------------
	DWORD dwGeneralEftSetClock;
	
	//-------------------------
	// 채팅 입력 시간 
	//-------------------------
	DWORD dwLastChatInputClock;
	

	//-------------------------
	// Daily Quest 에서 주기적으로 저장
	//-------------------------
	DWORD dwDailyQuestClock;
	DWORD dwDailyQuest2Clock;
	DWORD dwDailyQuest3Clock;


	//-------------------------
	// 낚시하는 중이라면, 낚시 아이템
	//-------------------------
	SI16			siFishingBaitItemUnique;
	//-------------------------
	// 자동 낚시
	//-------------------------
	bool			bAutoFishing;
	//-------------------------
	// 낚시 주기
	//-------------------------
	DWORD			dwPreCatchFishClock;
	DWORD		    dwSummonFishClock;     //배에 타고 있을때 소환수도 낚시하기 위한 주기

	//-------------------------
	// 인첸트 100% 성공 스위치 (운영자일때만)
	//-------------------------
	bool			bSuccessEnchant;
	
	//-------------------------
	// 농사 중이라면,  획득할려는 농사 아이템
	//-------------------------
	SI16			siFarmingItemUnique;
	//-------------------------
	// 농사 임대한 마을( 임시로 ) - 농경지가 없어서 빌린 사람들이지..
	//-------------------------
	//SI16			siFarmingRentVillageUnique;
	//-------------------------
	// 자동 농사
	//-------------------------
	bool			bAutoFarming;

	//-------------------------
	// 농사 획득 아이템 개수
	//-------------------------
	SI16			siFarmingItemCount;

	//-------------------------
	// 농사 주기
	//-------------------------
	DWORD			dwPreGetFarmingItemClock;
	SI16			siFarmingActionCount;
	DWORD			dwFarmingClock;

	//-------------------------
	// 채굴
	//-------------------------
	SI16			siMiningItemUnique;
	//SI16			siMiningRentVillageUnique;
	bool			bAutoMining;
	//SI16			siMiningItemCount;
	DWORD			dwPreGetMiningItemClock;
	SI16			siMiningActionCount;
	DWORD			dwMiningClock;
	
	//DWORD			dwStartAgricultureClock;
//	SI32			siLeaseLandVillage;			// 임시로 빌린 광산이나 농장의 마을 유니크
//	UI32			uiLeaseLandExpire;			// 임시로 빌린 광산이나 농장의 계약 종료 시간(분으로 저장)

	//-------------------------------------
	// Agriculture 데이터
	//-------------------------------------
	cltAgriculture*	m_pclAgriculture;
	
	//-------------------------------------
	// 선공 방지 시간. 
	//-------------------------------------
	DWORD dwNoFirstAttackClock;					// 선공 방지가 설정된 시간. 
	//-------------------------------------
	// ICE 방지 시간. 
	//-------------------------------------
	DWORD dwNoIceAttackClock;					// ICE 방지가 설정된 시간 

	//---------------------------------------
	// 클라이언트가 메시지 보낸 마지막 시간. 
	//---------------------------------------
	DWORD dwLastSendServerClock;

	//-------------------------------------
	// 물건 생산 
	//-------------------------------------
	bool bMakeItemReserveSwitch;
	DWORD dwMakeItemStartClock;
	DWORD dwMakeItemDelayClock;
	cltGameMsgRequest_MakeItemInv* pclReservedMakeItemData;

	bool bMakeItemNoMinigame;		// 미니게임없이 만드는 것들이다.
	bool bMakeItemMinigameSwitch;	// 미니게임에 대한 것을 받았다.
	bool bMakeItemSwitch;			// 아이템 다 만들었다.
	
	bool bEnchantItemSwitch;		// 아이템을 인챈트 시킨다.
	bool bFinishEnchantItemSwitch;	// 아이템 인챈트를 마쳤다.

	SI32 siMonsterCorp;				// 몬스터 주식 회사 점수
	DWORD dwStartMiniGameTime;		// 미니 게임을 한 시간

	cltCharGoonzuDayMissionInfo clGoonzuDayMissionInfo;		// [지연] 군주데이미션 : 미션 수행 정보(횟수)

	// 박대박 게임
	bool bRouletteStart;
	SI16  m_siBettingCount;
	SI16  m_siResultCount;
	DWORD dwRouletteStartClock;
	DWORD dwRouletteDelayClock;
#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<RouletteTable, 5>, 5>		m_stRouletteTable;
	NSafeTArray<SI16, 5>								m_siRandomValue;
#else
	RouletteTable	m_stRouletteTable[5][5];
	SI16			m_siRandomValue[5];
#endif
	//-----------------------------------
	// 종료 희망 여부 변수. 
	//-----------------------------------
	bool bHopeToQuitSwitch;
	
	//----------------------------------
	// 일정 시간 경과후 종료 변수 
	//----------------------------------
	bool bCloseSocketSwitch;
	DWORD dwCloseSocketTimer;

	//---------------------------------
	// 아이템 사용 예약. 
	//---------------------------------
	DWORD dwLastCheckUseReserveItemClock;
	cltUseItemReserveInfo			clItemUseReserveInfo;
	cltBulletUseReserveInfo			clBulletUseReserveInfo;
	cltSummonBulletUseReserveInfo	clSummonBulletUseReserveInfo;	// 소환수 전용 발사체 예약
	
	//---------------------------------
	// 환경 설정
	//---------------------------------
	bool bWhisperChatReject;
	bool bPrivateTradeReject;
	bool bPartyInviteReject;
	bool bFriendAddReject;
	//---------------------------------
	// 현재 위치 충돌 체크 간격. 
	//---------------------------------
	DWORD dwLastCheckCollClock;


	//---------------------------------
	// 분신술 간격 시간 
	//---------------------------------
	DWORD dwLastClone;

	//-----------------------------
	// 전송해야할 자막 종류
	//-----------------------------
	SI16			siCaptionKind;
	
	//-----------------------------
	// 스틸 관련 
	//-----------------------------
	DWORD			dwStealClock;
	SI16			siStealNum;			// 스틸한 횟수. 

	//----------------------------
	// 감옥 관련 
	//----------------------------
	DWORD dwPrisonClock;
	
	//-----------------------------------
	// 퀘스트를 완료하지 않아 레벨업 불가 공지 시각
	//--------------------------------------------
	DWORD dwLastLevelUpFailForQuestClock;


	//-----------------------------------
	// 공격 예약 
	//-----------------------------------
	SI16 siReservedAttackTargetID;
	
	//-----------------------------------
	// 소환수 처리 
	//-----------------------------------
	bool bIsEvolving;		// 소환수 진화 처리 상태임을 설정함 
	bool bIsTradeing;		// 소환수 팔기중

	//------------------------------------
	// WeaponSkill 처리
	//----------------------------------
	bool bIsWeaponSkillUp;

	//----------------------------------
	// GameEvent
	//-----------------------------------
	bool bCheckGameEvent;

	//--------------------------------------------
	// 생생탕 관련 - 중복해서 사용을 막는다.
	//--------------------------------------------
	bool bIsSetPremium;
	//--------------------------------------------
	// 활성탕 관련 - 중복해서 사용을 막는다.
	//--------------------------------------------
	bool bIsSetCombat;

	//--------------------------------------------
	// 장생탕 관련 - 중복해서 사용을 막는다.
	//--------------------------------------------
	bool bIsSetMakeItem;

	bool bIsBulletService;

	bool bIsSummonStaminaService;

	//--------------------------------------------
	// 캐쉬 아이템 팔기 관련 - 중복 팔기 방지
	//--------------------------------------------
	bool bIsBuyCashItem;

	//--------------------------------------------
	// 아이템 줍고 있다.
	//--------------------------------------------
	bool bIsPickUpItem;

	//-------------------------------------------------
	// 아이템 몰에서 아이템 받고 있다.
	//-------------------------------------------------
	bool bIsItemMallGetItem;
	//-------------------------------------------------
	// 아이템 몰에서 아이템 사거나 보내고 있다.
	//-------------------------------------------------
	bool bIsItemMallBuyItem;

	//--------------------------------------
	// 복주머니 아이템 사용중
	//--------------------------------------
	bool bUseBagItem;

	//--------------------------------------
	// 영웅 변신 아이템 사용중
	//--------------------------------------
	bool bUseChangeHeroItem;

	//-------------------------------------
	// 무역 거래중이다.
	//-------------------------------------
	bool bTradeServerUsing;

	//-------------------------------------
	// 마을 공성전 전적 점수.
	//-------------------------------------
	SI32 siVillageWarKillScore;

	//-------------------------------------
	// 버프 아이템을 먹는 중이다. 동시에 먹어서 버프가 사라지는 문제를 해결하기 위한 변수
	//-------------------------------------
	bool bIsNowAddBufThroughDB;
	
	//-------------------------------------
	// 가차 아이템을 뽑는 중이다. 한번에 여러번 가차를 할수 없게끔 하는 변수
	//-------------------------------------
	bool bNowGachaBuy;

	//-------------------------------------
	// 소환수를 소환중이므로 다음 소환수는 소환시키지 못하게 한다.
	//-------------------------------------
	bool bIsNowAddSummonThroughDB;

	//-------------------------------------
	// 수호정령 관련
	//-------------------------------------
	bool	bIsNowAddSoulGuardThroughDB;			// 수호정령을 뽑아서 DB와 통신중이다
	DWORD	dwLastSoulGuardActionClock;				// 수호정령 엑션돌때 확인 타임
	DWORD	dwLastSoulGuardParentDeadMessageClock;	// 수호정령 주인이 죽었을때 체팅 메시지를 뿌린 시간

public:
	//--------------------------------------------------------------
	// 몬스터 그룹 매니저 - 현재는 몬스터만 사용
	//---------------------------------------------------------------
	SI32 siGroupUnique; // 자신이 속한 그룹의 유니크
	SI32 siGroupMember; // 자신이 속한 그룹에서 몇번째 대원인가(몇번 버퍼인가?)
	BOOL bGroupBossSwitch; // 자신이 속한 그룹의 보스냐 아니냐

	//---------------------------------
	// 무기 사용술 부적 현황
	//---------------------------------
	bool	bSwordScroll;
	bool	bSpearScroll;
	bool	bAxeScroll;
	bool	bGunScroll;
	bool	bBowScroll;
	bool	bStaffScroll;
	bool	bCannonScroll;
	
	//---------------------------------
	// 무기 사용술 부적수 현황
	//---------------------------------
	SI08	siSwordScrollNum;
	SI08	siSpearScrollNum;
	SI08	siAxeScrollNum;
	SI08	siGunScrollNum;
	SI08	siBowScrollNum;
	SI08	siStaffScrollNum;

	// APEX-
	bool bApexLogin;
	// -APEX

	// 피로방지-
	bool	bTiredSystem;
	UI08	uiTiredState;
	DWORD	dwPlayingTime;	// 유저가 접속해서 게임을 한 시간
	SI16 siTiredNotice;
	SI32   lastTiredNoticedovertime; // 마지막 메시지의 중복을 막기 위해서.

	// 마지막 퀘스트 저장시간
	clock_t	lastQuestSaveDBTime;
	
	// 미니 게임을 시작한 시간
//	DWORD	dwStartMiniGame;

	UI32	uiIDNo;			// 한게임용 IDNO - by LEEKH 2007.11.11

	bool	bMakeTimeReduce;	// 제조 시간 단축 - true : 제조시간 무조건 1초 - by LEEKH 2008.05.09

	SI32	siQuestCount;		// 퀘스트용 카운트 증가 값

	SI64	siServerEvent_IndunScore;	//서버 이벤트 인던 점수용

	SI16	siCharRouletteValue;		// 캐릭터 룰렛 결과 저장 변수

	SI16	siDivinationValue;			// 신년룰렛 결과 저장 변수

	SI16	m_siNewSelectBoxValue;		// 새로운 상자 시스템 임시 저장용(이 값은 단지 클라 조작을 막기위한 변수임)

public:


	cltCharServer();
	virtual ~cltCharServer();

	//----------------------------
	// 서버 캐릭터의 행동. 
	//----------------------------
	void CharServerAction();
	void CharServerAction_NPC();
	void CharServerAction_PC();
	void CharServerAction_Summon();
	void CharServerAction_SoulGuard();

	void SummonFollowHost();		// 소환수가 주인을 따라가게 한다. 
	void SummonAutoHeal();			// 소환수가 자동 회복을 한다.

	//--------------------------------
	// 수호정령
	//--------------------------------
	void SoulGuardFollowHost( void );
	// 수호정령 능력을 발동 시킨다(확률에 의해서 발동 안될수도 있음)
	SI32 SoulGuardAbilityActivity( SI32 siAbility, SI32 siLessThanValue=0, SI32 siMoreThanValue=0 );
	void SendSoulGuardParentDeadMessage( void );



	void InitCharServer();

	BOOL SendNeighbourMsg(cltMsg* pclmsg, bool bselfswitch);		// 이웃에게 메시지를 보낸다. 
	BOOL SendNeighbourExceptMeMsg(cltMsg* pclmsg);

	BOOL SetNeighbour();		// 캐릭터 주변의 이웃 데이터를 정리한다. 

	void CalculateRealXY();		// 찍을 좌표를 계산한다. 

	void GetBottomPoint(cltPos* pclpos);

	//--------------------------------------
	// 클라이언트로 통보 함수 
	//--------------------------------------
	bool SendInnerStatusToNeighbour();	// InnerStatus를 이웃과 자기자신에게 통보한다. 
	BOOL SendChangedDataToClient();
	
	void InitPreGetOrder();				// 명령을 새로 수여하기 전에 해야할 초기화 

	//-----------------------
	//	암호화키와 캐릭터 유니크 정보 통보. 
	//-----------------------
	void SecretNCharUniqueSendOperation();		// 캐릭터 생성 후에 암호화키와 캐릭터 유니크 값을 클라이언트로 보낸다. 

	//-----------------------------------
	// 공간 이동 관련 
	//-----------------------------------
	void InitWarpInfo();
	void SetWarpInfo(const cltWarpInfo* pclCharWarpInfo, SI32 warpunique);
	void SetWarpToVillage(cltMapManager* pclmapmanager, SI16 villageunique, SI32 warpunique);	// 특정 마을로 워프하게 한다. 

	BOOL Warp(bool bwritelog = false);	// 공간 이동시킨다. 
	void WarpToSafeZone();				// 안전지대로 공간이동한다. 
	
	bool IsWarpStatus();

	//--------------------------------
	// CharServer-Magic.cpp
	//--------------------------------
	void SetMagicDelay(SI32 delay);
	SI32 GetMagicDelay();
	
	void CheckCurrentMagic( SI32 _siPrevActiveWeapon );		// 현재 설정된 마법이 적절한지 점검한다. 
	bool MagicInAttackAction();		// 전투중 마법 구현에 대한 행위. 

	SI32 			GetTotalMaxHealth();	// 최종 맥스헬스 값을 구한다. 
	
	bool IsExistItemOnOpeningPersonShop(SI32 siItemPos);		// 개인상점에 자신의 특정 아이템이 등록되었는지..
	
//-----------------------------------------------------------------------------------
//  보물 관련 
//-----------------------------------------------------------------------------------
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y);
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize);

//-----------------------------------------------------------------------------------
// 유닛 인공지능 관련. 
//-----------------------------------------------------------------------------------

	BOOL			CanWork();					// 일을 할 준비가 되어 있는가?
    bool			FindEnemyAction(SI32 id);	// 적에 대한 대처 
	BOOL			Pull(SI32 dir);				// 특정 방향으로 캐릭터를 밀어낸다. 
	
	//--------------------------------------
	// 공격관련 
	//--------------------------------------
	void SetLastKillClock(DWORD clock);							// 마지막으로 사냥한 시간을 체크한다. 
	void SetReservedAttackTargetID(SI16 id);
	void AddKillMonsterNum(SI32 add)
	{
		clSimpleVary.siCheckMonsterNum += add ;
	}
	void SetKillMonsterNum(SI32 num)
	{
		clSimpleVary.siCheckMonsterNum = num ;
	}

	bool  DoReservedAttackAction(cltOrderDataInfo* pclorder);	// 예약되어 있는 공격 명령을 수행한다. 
	bool  DoReservedAttackClientAction(cltOrderDataInfo* pclorder);

	void GetShapeEquipInfo(cltShapeEquipInfo* pclequip);		// 내 외양 장비 상황을 얻어온다. 
	
	void  CheckWeaponSkillScroll(cltItemManagerCommon* pclItemManager);		// 무기 사용술 부적 사용 실태를 체크한다. 

	//-------------------------------------
	// 정지 상태의 캐릭터가 행하는 행위
	//-------------------------------------
	
    BOOL OperateWaitServer();		// 정지시 행동 
	bool SearchEnemy(SI32 range);	// 근처의 적을 찾는다. 
	bool RandomMove();				// 랜덤하게 주변으로 이동한다. 
	
	bool FollowGroupBoss(SI32 bossid);		// 보스가 있으면 보스를 따라가게 한다.

	// mode :	1 -> PC
	//			0 -> NPC
	//			2 -> Instance NPC
	static SI32 FindEmptyCharUnique(SI32 mode)
	{
		SI32 i;


		if(mode == 1)
		{
			for(i =1; i < MAX_PERSON_NUMBER;i++)
			{
				if(CharUniqueInfo[i] == 0)return i;
			}
		}
		else if(mode == 2)
		{
			for(i = (MAX_PERSON_NUMBER ); i < (MAX_PERSON_NUMBER +MAX_INSTANCE_NPC_NUMBER);i++)
			{
				if(CharUniqueInfo[i] == 0)return i;
			}
		}
		else
		{
			for(i = (MAX_PERSON_NUMBER +MAX_INSTANCE_NPC_NUMBER); i < MAX_CHAR_NUMBER;i++)
			{
				if(CharUniqueInfo[i] == 0)return i;
			}
		}

		return 0;
	}
	
	//------------------------------------------------
	// 아이템 관련 
	//-----------------------------------------------

	SI32 CreateSoulGuard( cltSoulGuard* pclCreateSoulGuard );
	void DeleteSoulGuard( void );
	
	SI32 CreateSummon(cltSummon* pclSummon);		// 아이템을 사용한다
	bool UseItem(cltItem clitem,SI32 itempos = 0);	// 아이템을 사용한다. 

	void ItemUseReserveAction();					// 예약되어 있는 사용 아이템 처리를 한다. 
	void BulletUseReserveAction();					// 예약되어 있는 탄환 아이템 처리를 한다. 
	void BulletUseReserveAction_ForPerson();		// 케릭터 탄환 아이템 사용처리 
	void BulletUseReserveAction_ForSummon();		// 케릭터 탄환 아이템 사용처리
	BOOL AddItemToInventory(cltItem* pclitem);		// 아이템을 인벤토리에 넣는다. 
		
	bool InputItem(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32 inputitemreason, SI32* prtnvalue);	//캐릭터에게 아이템을 준다. 
	void InputItemReserveAction();		// InputItem시 예약된 자리를 정리한다. 

	// 트레이드 서버 전용! 트레이드중인 것은 움직이지 못하게 한다!
	void SetNoMoveItemSwitch(SI32 itempos,bool move)
	{
		if ( itempos >= 0 && itempos < MAX_ITEM_PER_PERSON )
			clSimpleVary.bNoMoveItemSwitch[itempos] = move ;
	}

	SI32 GetNoMoveItemSwitch(SI32 itempos)
	{
		if ( itempos >= 0 && itempos < MAX_ITEM_PER_PERSON )
		{
			if ( clSimpleVary.bNoMoveItemSwitch[itempos] )
			{
				// 이미 이동중이다!
				return 1 ;
			}
			else return 0 ; // 이동중이 아니다!
		}
		else return -1 ; // 위치가 잘못 되었다!
	}

	void SetInputItemReserve(SI32 itempos, DWORD data);

	SI32 GiveItem(SI32 itemunique, SI32 itemnum, SI32 giveitemreason,SI32 usedate, SI32 siSealMode=GIVEITEM_SEAL_MODE_NONE, SI32 EnchantType = 0, SI32 EnchantLevel = 0, SI32 SlotNum = 0);	// 캐릭터에게 아이템을 주고 필요한 조치를 취한다. 
	bool TakeOutItem(SI32 itemunique, SI32 itemnum, SI32 takeoutitemreason);		// 캐릭터에게서 아이템을 제거하고 필요한 조치를 취한다. 
	// 주문을 근거로 물건을 인첸 할수 있는지 검사한다.
	bool CanEnchantItem(cltGameMsgRequest_MakeItemInv* pclinfo, SI32 *returnval, SI32 *preturnpara1);
	// 주문을 근거로 물건을 만들수 있는지 검사한다.
	bool CanMakeItem(cltGameMsgRequest_MakeItemInv* pclinfo, SI32 *returnval, SI32* preturnpara1);		
	
	// 인첸트한다
	bool EnchantItem(cltGameMsgRequest_MakeItemInv* pclinfo);
	
	bool MakeItem(cltGameMsgRequest_MakeItemInv* pclinfo);		// 주문을 근거로 물건을 만든다. 
	bool SetReserverMakeItem(DWORD productclock, cltGameMsgRequest_MakeItemInv* pclinfo);		// 생산 예약한다.

	SI32 GetMakeItemNum(SI32 needskillunique, SI32 needskilllevel, SI32 makeitemunique, SI32 makeitemnum);
	SI32 GetMakeCrystalItemNum(SI32 needskillunique, SI32 needskilllevel, SI32 makeitemunique, SI32 makeitemnum);

	void DecreaseEquipDur(SI32 itempos, SI32 durwaste);						// 각 장비의 내구도를 감소시킨다. 
	void DecreaseSummonEquipDur(SI32 siSummonIndex, SI32 itempos, SI32 durwaste);
	void DecreaseArmourDur( SI32 siEnemyID );								// 방구의 내구도를 감소시킨다. 
	void DecreaseWeaponDur( SI32 siEnemyID );								// 무기의  내구도를 감소시킨다. 
	void DecreaseSkillItemEquipDur( SI32 skillunique );		// 특정 기술이나 행운을 가진 물품 내구도를 소모시킨다. 
	void DecreaseEquipDurFromDead( );

	void UseItemReasonDurabilityZero( SI32 siItemPos );	// 내구도가 0이어서 삭제한다

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 내구도 감소 관련.
	void SetCrystalDurUpdateSwitch(SI32 itemposition, bool mode);
	void SubDecreaseCrystalDur(SI16 ActiveItem,SI16 durvary);
	void DecreaseCrystalDurAC(SI16 durvary); // 액티부된 무기의 결정체 내구도를 감소시킨다.
	void DecreaseCrystalDurDE(SI16 durvary); // 모든 방어구의 경정체 내구도를 감소시킨다.

	void SendClientCrystalDurability(); // 변경된 결정체 내구도 정보를 Client로 보낸다.
	void SubSendClientCrystalDurability(SI16 ActiveItem);

	// [영훈] 소환수 장비장착 : 소환수 장비 크리스탈 감소
	void SubDecreaseCrystalDurForSummon( SI16 siActiveItem, SI16 siDurvary );
	void DecreaseCrystalDurACForSummon( SI16 siDurvary );
	void DecreaseCrystalDurDEForSummon( SI16 siDurvary );

	void SendClientCrystalDurabilityForSummon();

	//-------------------------------------------------------------------------------------

	bool CanAddInv(SI32 invmode, cltItem* pclitem, SI16* pitempos, cltItem* pcltoitem, SI32* prtnvalue);
	
	bool IsBlankInv(SI32 invnum,SI32 invmode);

	// 한번에 여러개까지의 공간을 체크해 보기 위해.
	bool CanAddInvManyItems(SI32 invmode, cltItem* pclitem, SI32* prtnvalue);

	void SetRoulette(bool bcheck);	// 시작할껀지 종료할껀지
	void CheckRoulette();			// 먼저 룰렛 체크
	bool CheckLottoRoulette();		// 대박인지 체크

	bool CheckSubBigItem();
	//히든 퀘슨트 일때 2개니까 검사....'';이건 아닌데..
	bool CheckItemForEmptyAnyItemSlotByHiddenQuesst( SI32 invmode, cltItem* pclitem,cltItem* pclitem2);
	
	// 버프를 추가하는 메소드 ( 버프가 DB를 거쳐 완전히 추가되기 전엔 다른 버프를 추가할수 없다 ) [영진]
	bool AddBufThroughDB(SI16 index, stBuf* buf, bool useitem, SI16 itempos, cltItem* pclitem, GMONEY money = 0);
	// [지연] 프리미엄 아이템을 위한 누적 가능한 버프 함수 추가.
	bool AddBufThroughDB_AccumulateEnable(stBuf* pBuf, bool useitem, SI16 itempos, cltItem* pclitem, GMONEY money = 0);
	// [지연] 현재 접속해있는 마을 유저에게 버프를 준다. (아이템 사용한 사람과 같은 마을 사람에게..)
	bool AddBufToAllTownPerson(SI32 siTownUnique, stBuf* pBuf, SI32 siSkipBuf1 = 0, SI32 siSkipBuf2 = 0);
	bool AddTownBufThroughDB(cltCharServer* pCharServer, stBuf* pBuf);
	bool IsNowAddBufThroughDB();

	//-------------------------------------------
	// Rank 관리 
	//-------------------------------------------
	BOOL SetRank(cltSimpleRank* pclrank);

	//----------------------------------
	// Skill
	//----------------------------------
	BOOL DevelopSkill(SI32 skillunique);		// 스킬을 최초로 연마한다. 

	//---------------------------------
	// Update
	//---------------------------------
	void SetUpdateSwitch(SI32 updatetype, bool mode, SI32 para1);
	void SetSkillUpdate(SI32 skillunique, SI32 addskillexp);
	void SetWasteDurUpdateSwitch(SI32 itemposition, bool mode);
	void SetSummonWasteDurUpdateSwitch(SI32 SummonIndex, SI32 itemposition, bool mode);

	//--------------------------------
	// Group
	//--------------------------------
	void SetNPCGroup(SI32 group);
	SI32 GetNPCGroup()const;

	//------------------------------------
	// CharServer-Session.cpp
	//------------------------------------
	///----------------------------------
	// Session 생성시 호출 함수. 
	//-----------------------------------
	void Init();
	void Close();
	void CloseAction(cltServer* pclServer, bool bnormalquitswitch);		// 게임을 종료하기 전에 캐릭터가 수행해야 할 일들. 

	//----------------------------------
	// 사망 관련 
	//----------------------------------
	bool IsDeathPenaltyCondition();
	void DeathPenalty(SI32 killedmode = -1, TCHAR* szkillername= NULL);		// 사망에 따른 페널티. 
	void RequestRevive(SI32 id, SI16 DieMode);
	void Revive();			// 부활하기. 
	void Resurrection( bool pvpFlag );	// 제자리 즉시 부활하기.

	//---------------------------------
	// 클라이언트로부터 메시지를 받은 시간.
	//---------------------------------
	void SetLastReceveFromClientClock(DWORD clock);

	//----------------------------------
	// 로그 아웃 관련 
	//---------------------------------
	bool AutoLogOut();			// 시간 경과에 따른 자동 로그아웃. 

	//----------------------------------
	// 셀관련 작업.
	//----------------------------------
	void CellAction();			//캐릭터가 처음 만들어 졌을때 셀에 등록및 셀의 정보 요청과 같은 임무를 수행한다. 
	void DeleteInCell();		// 셀 내에서의 정보를 삭제한다. 

	//----------------------------------------------------------
	// 클라이언트로의 전송 함수. 
	//----------------------------------------------------------
	void SendLetterMsg(cltLetterUnit* pclletter);		//쪽지를 보낸다. 
	void SendAccountLetterMsg(cltAccountLetterUnit* pclletter); // 

	//--------------------------------------------------------
	// CharServer-SendDB.cpp
	//--------------------------------------------------------
	void SendDB_PosLifeMana();									// 위치, 체력, 마법력 정보를 DB에 저장한다. 
	void SendDBLevelInfo(SI32 incbonus, SI32 incskillbonus);	// level 정보를 DB에 저장한다. 
	void SendDBSkillExp();										// 변경된 SKill정보를 DB에 저장한다.
	void SendDBWasteDurability();								// 변경된 내구도 정보를 DB에 저장한다.
	void SendDBSummonWasteDurability();
	void SendDBSummon(SI32 siReason, UI08 siSummonIndex, cltSummon *pSummon);		// 변경된 소환수 정보를 DB에 저장한다.
	void SendDBSummon(SI32 siReason, SI32 siSummonIndex);
	void UpdateTakeOutSummon();									// 밖으로 나와있는 소환수의 정보를 주인에게 update 시킨다.
	void SendDBUpdateTeacherPoint( SI32 Level );

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	void SendDBCrystalDurability();								// 변경된 결정체 내구도 정보를 DB로 보낸다.
	void SendDBCrystalDurabilityForSummon();					// 소환수용 결정체 내구도 감소
	//-----------------------------------
	// 로그인후 보호 
	//-----------------------------------
	void SetInitProtectSwitch();
	bool GetInitProtectSwitch();
	
	void AutoRecoverLife();		// 자동 회복 기능. 
	void AutoRecoverMana();

	void AutoCushionRecoverLife();	// 고양이 쿠션에 의한 자동 회복기능
	void AutoCushionRecoverMana();

	// PCK : 잠자는 냐옹이 방석에 의한 소환수 자동 회복기능 (09.06.15)
	void AutoCushionRecoverSummonLife();
	void AutoCushionRecoverSummonMana();

	//KHY - 1220 - 변신시스템.
	void AutoTransFormRecoverMana();

	void LevelUpAction();								// 레벨 업에 따른 작업. 
	bool IncreaseExp(SI64 exp, SI32 reason = 0);		// 경험치를 올리고 필요한 모든 조치를 취한다.// SI32->SI64 경험치 오버플로우때문에 수정	 
	bool IncreaseFameExp(SI32 exp, SI32 reason =0);		// 명성 경험치를 올리고 필요한 모든 조치를 취한다. 
	 

	void SetHeadChat(bool bheadchatswitch, TCHAR* ptext);			// 캐릭터의 머릿말을 설정한다. 
	void SetPersonalShopBanner(cltShopTradeData* pclshopdata);		// 캐릭터의 PersonalShop정보를 설정한다.
	void SetPersonalShopData(bool bRepairShopSW, cltShopTradeData* pclshopdata);

	void SetRepairShopData( bool bRepairShopSW, cltShopRepairData* pclRepairData );
	
	bool AfterCharAction();
	void CalcPlayTime();		// 게임 플레이 시간을 계산한다. 

	//---------------------------------------
	// 지휘 효과
	//---------------------------------------
	void GeneralEft(bool bacswitch, SI32 acuprate, bool bdamageswitch, SI32 damageuprate , bool alluser );	// 지휘 효과를 설정한다. 
	
	//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 있다면 버프 적용. 현재 사용안함]
	void NewYearTreeEft(SI32 _VillageUnique);


	bool HealAI();		// 인공지능 치료. 
	void FishAI();		// 인공지능 낚시. 
	void FarmAI();		// 인공지능 농경
	void MineAI();		// 인공지능 채광
	
	bool CheckItemMallBadRequest();				// 아이템몰의 악의적 데이터 갱신을 체크한다. 1초에 5회 이상의 요청을 방지 
	bool CheckLastMsgClock(DWORD interval);		// 클라이언트가 서버로 보낸 마지막 메시지와의 시간 간격을 확인한다. 
	void MakeHunger(SI32 addHunger = 0);							// 배고프게 만든다. 

	BOOL DoAnimationDelay(BOOL atb = 0);
	void CharServerStatusAction();

	//=================================================
	BOOL SetPoison( SI32 poisoner, SI32 damage, DWORD _dwAddClock = 20000 );			// 중독을 시킨다. 
	BOOL SetPoisonSetItemEffect( SI32 poisoner, SI32 damage, DWORD _dwAddClock = 20000 );		//세트 아이템의 효과로 중독을 시킨다. 
	BOOL SetIce(SI32 damage, DWORD _dwAddClock = 0);									// 캐릭터를 얼린다. 
	
	BOOL SetDrain(SI32 damage, SI32 passivedamage, SI32 passivemana);		// Drain으로 부터 에너지를 추가한다.
	BOOL SetCurseAttack(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid);		// 공격력이 저하된다.
	BOOL SetCurseCritical(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid);		// Critical 확률이 낮아진다.
	BOOL SetCurseDefense(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid);		// 방어력이 낮아진다. (피 공격력이 증가)
	BOOL SetCurseMovespeed(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid);	// 이동속도 저하 
	
	BOOL SetMagicFire(SI32 enemy, SI32 damage);		// 불에 태운다.
	BOOL SetFrozen(SI32 damage);					// 보스 얼음으로 얼린다.
	BOOL SetStun(SI32 damage);						// 스턴을 건다
	BOOL SetSoulGuardIce(SI32 damage);				// 소울가드 - 얼린다
	//=================================================

	//=================================================
	BOOL DeletePoison();							// 중독 상태를 삭제한다.
	BOOL DeletePoisonSetItemEffect( void );
	BOOL DeleteIce();								// 얼음 상태를 삭제한다.

	BOOL DeleteCurseAttack();						// 공격력이 저하 상태를 삭제한다.
	BOOL DeleteCurseCritical();						// Critical 확률이 낮아지는 상태를 삭제한다.
	BOOL DeleteCurseDefense();						// 방어력이 낮아지는 상태를 삭제한다.
	BOOL DeleteCurseMovespeed();					// 이동속도 저하 상태를 삭제한다.

	BOOL DeleteMagicFire();							// 불에 타는 상태를 삭제한다.
	BOOL DeleteFrozen();							// 보스 얼음 상태를 삭제한다.
	BOOL DeleteStun();								// 스턴 상태를 해지 한다.
	BOOL DeleteCurseEffect();						// [진성] 디버프 마법 삭제.
	//=================================================

	void BeHitOperate(SI32 attackid);				//캐릭터가 공격 받았을 때 처리해야할 일들. 
	SI32 DecreaseLife(SI32 life);
	
	bool SendNeighbourMyShapeStatus();				// 이웃에게 내 외양 정보를 통보한다. 
	bool SendNeighbourMyMakeItemStatus();

	//------------------------------------------
	// 캐릭터 생성을 위한 기본 정보 
	//------------------------------------------
	bool GetMakeCharInfo(cltMakeCharInfo* pclinfo, bool borderswitch, bool bticketusewarpswitch);
	
	cltCharServer* GetHostChar();		// 주인 캐릭터의 포인터를 구한다. 

	//---------------------------------
	// CharServer-Dying.cpp
	//---------------------------------
	bool	Action_Dying();
	void	Dead();					// 사망 처리. 
	bool	TugDieOperation();		// 줄다리기 경기장에서 죽었을경우 서버에서만 처리하는 함수 
	void	SelfBombAction();			// 자폭처리. 
	
	//----------------------------------------------
	// Char-Horse.cpp
	//----------------------------------------------
	SI32 CharHorseAction(cltHorse* pclhorse, bool bselfswitch);		// 사용자가 보유한 말에 대한 행위를 처리한다. 
	bool SendNeighbourMyHorseStatus(SI32 index);					// 이웃에게 내 말의 상태를 통보한다. 
	bool AutoHorseFood(SI32 itemunique, SI32 itemnum);				// 자동으로 말 먹이를 준다. 

	void UpdateHorseToDB(cltServer* pclserver, SI32 index, cltHorse* pclhorse, SI32 reason);		// 말 정보를 DB에 업데이트 한다. 
	void CharServerMoveByGate(SI32 x, SI32 y);						// 서버에서 캐릭터의 공간 이동 관리.

	//-----------------------------------------------
	// CharServer-Manner.cpp
	//-----------------------------------------------
	SI16 GetStealNum();		// 스틸한 횟수를 얻어온다. 
	bool SendPrison();		// 감옥에 보낸다. 
	bool IsAutoHunting();	// 자동 사냥인지 여부를 확인한다. 

	//----------------------------------------------
	// CharServer-Fish.cpp
	//----------------------------------------------
	bool CatchFishAction( bool IsPerson );				// 물고기를 잡았다.
	SI32 CatchFish_GoldMineral(SI32 FishSkillLevel);
	SI32 CatchFish_SpecialItem();
	SI32 CatchFish_BigItem(SI32 FishSkillLevel);
	bool FishingStart( SI32 FishingBaitItemUnique );	// 낚시 시작
	void FishingStop();									// 낚시 중단 
	bool GetFishingToolInfo( SI16 FishKindListRef, DBToolInfo *pFishingToolList );		// 도구를 사용한다.
	bool IsAvailableFishing( SI32 FishingBaitItemUnique, SI16 FishKindListRef );		// 낚시 가능한지 체크

	//----------------------------------------------
	// CharServer-Farm.cpp
	//----------------------------------------------
	bool FarmingAction( bool IsPerson );
	void FarmingStop();	

	SI32 FarmingBigItem();
	SI32 FarmingBigItem_New(SI32 FarmSkillLevel);

	//----------------------------------------------
	// CharServer-Mine.cpp
	//----------------------------------------------
	bool MiningAction( bool IsPerson );
	void MiningStop();
	bool IsLeaseAgriculture( SI32 villageUnique );

	SI32 MiningBigItem();
	SI32 MiningBigItem_New( SI32 MineSkillLevel );
	
	//-------------------------------------------
	// CharServer-Quest.cpp 
	//-------------------------------------------
	void IncQuestPara(SI32 clearcond, SI32 para1, SI32 incnum);		// 퀘스트의 파라메타를 증가시킨다. 
	void DoQuestCheckAndAction(SI32 questclearcond);				// 캐릭터가 진행하고 있는 퀘스트를 점검하여 종결시킨다. 
	
	bool MakeSpecialQuest();			// 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Battle();		// 전투용 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Farm();		// 농사짓기용 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Fish();		// 낚시하기용 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Mine();		// 채광하기용 특수 퀘스트를 만든다.
	bool MakeSpecialQuest_Skill();		// 기술 올리기  특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_UseItem();	// 물건 만들기 퀘스트를 만든다. 
	bool MakeSpecialQuest_MakeFriend();	// 친구만들기용 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_GetItem();	// 물품 가져오기 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_MakeItem();	// 물품 생산하기 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Equip();		// 장비 장착하기 특수 퀘스트를 만든다. 
	bool MakeSpecialQuest_Scroll();		// 장비에 부적 바르기.
	bool MakeNamingQuest( SI32 index, SI32 level );		// 명칭 퀘스트를 만든다.
	bool MakeSpecialQuest_Gomap();		// [지연/090605] 마을 순회하기 퀘스트를 만든다.
	bool MakeSpecialQuest_MakeSkillExpup();	// [지연/090608] 제조 경험치 +1 올리기 퀘스트를 만든다.

	// [지연- 2009.10.14] 신년이벤트 - 흉 점괘가 나왔을때 수행하는 퀘스트를 만든다.
	bool MakeQuest_RemoveUnLuckStatus();
	bool MakeQuest_RemoveUnLuckStatus_Battle();				// 몬스터 사냥 퀘스트를 만든다.
	bool MakeQuest_RemoveUnLuckStatus_MakeItem();			// 물품 생산하기 퀘스트를 만든다.
	bool MakeQuest_RemoveUnLuckStatus_NPCQuestComplete();	// NPC가 진행하고 있는 퀘스트를 완료하는 퀘스트를 만든다.

	//void EventReward_NewYear2010();		// 2010신년이벤트 - 쿠키 보상

	//KHY - 0822 - 해피빈 퀘스트 추가.
	bool MakeHappyBeanQuest();
	bool MakeHappyBeanQuest_Battle();	// 전투용 특수 퀘스트를 만든다. 	
	bool MakeHappyBeanQuest_Skill();		// 기술 올리기  특수 퀘스트를 만든다. 
	bool MakeHappyBeanQuest_GetItem();	// 물품 가져오기 특수 퀘스트를 만든다. 
	bool MakeHappyBeanQuest_MakeItem();	// 물품 생산하기 특수 퀘스트를 만든다. 
	//	bool MakeHappyBinQuest_Equip();		// 장비 장착하기 특수 퀘스트를 만든다. 

	bool MakeMissionQuest(SI32 siMissionUnique);
	bool MakeMissionQuest_Holloween();

	// [지연 - 2010.01.06] 군주 데이때 수행할 수 있는 미션 추가. (개인대상)
	bool Make_GoonzuDayMission();				// 군주데이미션		
	bool Make_GoonzuDayMission_Battle();		// 몬스터 사냥 퀘스트를 만든다.
	bool Make_GoonzuDayMission_MakeItem();		// 물품 생산하기 퀘스트를 만든다.
	bool Make_GoonzuDayMission_Collect();		// 물품 수집하기 퀘스트를 만든다.
	bool Make_GoonzuDayMission_UseItem();		// 물품 사용하기 퀘스트를 만든다.
	
	//-------------------------------------------
	// CharServer-Jim.cpp
	//-------------------------------------------
	void LoseJim();		// 찜이 풀린다. 

	//-------------------------------------------
	// CharServer-Attack.cpp
	//-------------------------------------------
    void SetBeAttackedSwitch(cltServer* pclserver, SHORT enemyid);	// 적에게 공격받고 있을때 호출하는 함수 
    void BeAttackedAction();										// 적에게 공격받을때의 조치 

	void MakeTorpedo(cltServer* pclserver, SI32 attackid, cltItem* pcltorpedo);		// 어뢰 아이템으로부터 어뢰 캐릭터를 생성한다.
	
	// 피로도 시스템 
    SI64 CalcTiredPoint( SI64 point );
	SI32 CalcTiredPoint( SI32 point );
	SI16 CalcTiredPoint( SI16 point );

	bool CharTransform( SI16 siCharKind , SI16 ActiveWeapon ,cltItem* pclitemlist , cltPSkillInfo *pclskillinfo);

	//KHY - 1014 - 방석 추가. - 이불 특수능력, 기관회복. 포만감 하락.
	void AutoCushionRecoverOragans();
	void AutoCushionFallSatisfied();

	//KHY - 1113 - 크리스마스 이벤트 - 크리스마스 퀘스트는 특별하다.
	void ChristamsEventQuest(SI32 *Enemykind);

	void TransformSummon( IN cltCharManager* _pclCM, IN cltSummonTransformManager* _pclSummonTransformManager );
	void UpDateSummonTransForm( IN cltSummonTransformManager* _pclSummonTransformManager, IN cltSummon* _pSummon, IN SI32 siTansformSummonIndex, OUT cltSummon** _pTranformSummon );
	SI32 CalcuSummonTransFormAbility( IN SI32 siStatus, IN SI32 _siAbility );
	void TransformSummonStaminaUp( cltCharServer* pclParentChar, SI32 amt );
	void TransformSummonStaminaDown( cltCharServer* pclParentChar, SI32 amt );

	void HuntQuestitem(SI32 siCharID, SI32 siMonsterKind);				// 사냥으로 퀘스트아이템 획득

	//[지연] - 2010신년이벤트 관련 ( 1/1000 확률 단위 (1은 백분율로 0.1%, 10은 1%) )
	void GiveFortuneCookiePiece(SI16 siRate, SI16 siCount);

	//[지연] - 2010신년이벤트 관련 - 디버프 퀘스트를 위한 텍스트 npc 카인드를 저장해둘 배열
#ifdef _SAFE_MEMORY
	NSafeTArray<NTCHARString128, REMOVEUNLUCK_QUESTNPC_COUNT>		m_arRemoveUnluckQuest_NPC;
	NSafeTArray<SI32, REMOVEUNLUCK_QUESTNPC_COUNT>					m_arRemoveUnluckQuest_Unique;
#else
	NTCHARString128													m_arRemoveUnluckQuest_NPC[REMOVEUNLUCK_QUESTNPC_COUNT];
	SI32															m_arRemoveUnluckQuest_Unique[REMOVEUNLUCK_QUESTNPC_COUNT];
#endif

	void InitArray_RemoveUnLuckQuestNPC();
};


#endif
