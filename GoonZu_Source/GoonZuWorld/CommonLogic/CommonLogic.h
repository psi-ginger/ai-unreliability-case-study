//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _COMMONLOGIC_H
#define _COMMONLOGIC_H

#define CHANNELINGCODE_NETMARBLEJP 1 //현재는 사용 안함.

//----------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가.
//----------------------------------------------------------
#define CHANNELINGCODE_NDEJP			-1  // NDE  - 지금은 사용하지 않는다. - 현재(0618) NDE 는 0 으로 들어온다.
#define CHANNELINGCODE_HANGAMEJP		2  // 한게임 
#define CHANNELINGCODE_GAMANIAJP		3  // 감마니아
#define CHANNELINGCODE_GAMEPOTJP		4  // 게임팟
#define CHANNELINGCODE_NEXONJP			5  // 게임팟

//----------------------------------------------------------
#define	MAX_MONSTERDROPITEM_NUM		20	// 몬스터 드랍하는 최대 아이템 갯수
#define MAX_WORLDTREE_COUNT			2	// 크리스마스이벤트 - 월드트리 갯수

// 서버 클라이언트 공통용. 
#include <directives.h>
#include "../common/CommonHeader.h"
#include "../Server/Time/MyTime.h"
#include "../Common/Map/TileSet/TileSet.h"
#include "Village/Village.h"
#include "NHashTableString.h"

#include "../Lib/CRC32/CRC32.h"

#ifdef USE_GAME_GUARD_SERVER

#include "../GameGuard/ggsrv25.h"
#pragma comment(lib, "ggsrvlib25.lib")

#endif

#ifdef USE_GAME_GUARD_CLIENT
#ifdef _KOREA
#include "../GameGuard/NPGameLib.h"
#pragma comment(lib, "NPGameLib.lib") 
#endif

//#define NO_GAMEGUARD
#ifdef _TAIWAN
#include "../GameGuard/NPGameLib_MD_DE.h"
#pragma comment(lib, "NPGameLib_MD_DE.lib")
#endif

#endif

// 욕설 필터링.
#ifdef _IAF_EXPORT
	#include "../IAF/IAF_Export.h"		

	#ifdef _DEBUG
		#pragma comment(lib, "IAFD.lib") 
	#else
		#pragma comment(lib, "IAF.lib") 
	#endif
#endif



// 공격 타입 
#include "AttackType/AttackType.h"

#include "../Common/Message/Message.h"
#include "../Client/ToolBar/ToolBar.h"
#include "../Common/Map/MapManager/MapManager.h"
//#include "../Common/CharStatus/CharStatus.h"
#include "../Common/Item/ItemServer/Item-ManagerServer.h"
#include "../Common/Item/ItemClient/Item-ManagerClient.h"
#include "../Common/Config/Config.h"
#include "../Common/Item/ItemCommon/NPCItem.h"
#include "../Common/Item/ItemCommon/ItemCommon.h"
#include "../Common/Char/PersonNameManager/PersonNameManager.h"
#include "../CommonLogic/Guild/GuildWar.h"
#include "LogPrint/LogPrint.h"

#include "CountryMgr/CountryManager.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../common/Map/FieldObject/FieldObject.h"
// 욕설 필터
#include "../Client/abusefilter/AbuseFilter.h"

#include "../common/BlackWar/BlackWar.h"
#include "../common/Event/MonsterAttackEvent/MonsterAttack.h"
#include "../Server/HelperManager/HelperManager.h"

#include "../common/PVP_Leage/PVP_LeageMgr.h"
#include "../CommonLogic/Guild//GuildHuntMapWarManager.h"
//#include "../CommonLogic/Guild/GuildHuntMapWarStatMgr.h"
#include "../CommonLogic/BalanceCurrency/BalanceCurrencyMgr.h"

#include "../CommonLogic/SoulGuardMgr/SoulGuardMgr.h"

#include "../common/RacoonDodgeEvent/RacoonDodgeEvent.h"

#include "../common/BokBulBokEventMgr/BokBulBokEvent.h"
#include "../CommonLogic/PartyMatchingMgr/Server_PartyMatchingMgr.h" // 파티매칭

#include "../CommonLogic/DormancyAccountSystem/DormancySystem.h"
#include "../CommonLogic/ValentineEvent/ValentineEvent.h"
#include "../common/Reward/RewardItemByProbMgr.h"

#include "..\Common\Char\KindInfo\KindInfo.h"

#include "BoxItemMgr/BoxItemMgr.h"

class cltWarRankManager;
class TSpr;
class cltCursor;
class AttackTarget;
class cltBulletManager;
class cltSmokeManager;
class cltDevTool;
class cltDevServerInfo;
class cltSmallMap;
class cltNPCGroup;
class cltSystemNPCManager;
class CForeignMerchantNPCMgr;
class CTradeMerchantNPCMgr;
class CPrivateTradeOrderMgr;
class CMagicMgr;
class cltSkillManager;
class cltOrderManager;
class cltKindInfoSet;
class cltPersonInfo;
class cltAttackTarget;
class cltKey;
class cltDiseaseManager;
class cltQuestManager;
class cltNPCManagerCommon;
class cltBlackArmyManager;
class cltKoinoboryManager;
class cltSantaRaccoonManager;
class cltKoinoboryFamilyManager;
class CGachaManager;
class CGacha2Manager;
class CArtifactRestoreManager;		//[추가 : 황진성 2008. 3. 3 => 아티펙트 환원 아이템 정보.]
class CFourLeafManager;

class CPartyMgr;
class cltAskManager;
class cltMasterManager;
class CNMasterInfo;

//class cltSchoolManager;
class cltFatherManager;
class cltHiredNPCPayManager;
class cltIdentity;

class CHuntMapInfo;

class cltItemMallManager;
class cltItemMallButtonManager;

class cltMonsterGroupManager;

// 각시설물들 
class cltCityHallManager;
class cltBankManager;
class cltMarketManager;
class cltHouseManager;
class cltStockManager;
class cltHuntManager;
class cltPostOfficeManager;
class cltHorseMarketManager;
class cltRealEstateMarketManager;
class cltSummonMarketManager;
class cltLandManager;
class cltFeastManager;
class cltSummonHeroMarketManager;
class cltMineManager;
class cltGuildManager;
class cltNewMarket_Manager;

class CStockDistributionMgr;
class CVoteMgr;
class CDailyQuestMgr;
class CDailyQuest2Mgr;
class CDailyQuest3Mgr;
class cltHorseManager;
class cltGameMasterManager;

class CFishingMgr;
class CFarmingMgr;


class cltEventHunting;
class cltMofuMofuEvent;
class CMonsterAttackEvent;

class cltAutoHuntingManager;
class cltVillageStructureSet;

class CForeignMerchantNPCBuyCashItem;

class CNTeacherDlg;
class CNFatherDlg;

class cltGameEvent;

class cltWorldTraderManager; // 아큐 매니저
class cltDistributionWorldMoney; // 아큐 공모 매니저

class CMiningMgr;

class CRPMerchantNPCMgr;
class cltGuildDungeonItem;
class cltEventStructure;
class cltEventTimeManager;

class CBlackWarManager;
class cltNamingQuestManager;

class CVoteStatusMgr;			// [영훈] 선거 개표

//====================================
// 서버 이벤트용 클레스
class CNServerRankInfo;				
class CNPersonalRankInfo;
class CNServerEvent_Rank_Record;
class CNServerEvent_Manager;
//====================================

class cltHelperManager;

class CPVP_LeagueMgr;
class cltGuildHuntMapWarManager;// [성웅] 길드 사냥터 소유 길드전 매니저.
class cltGuildHuntMapWarStatMgr;// [종호] 길드 사냥터 소유 길드전 상태 매니저.

class cltMarriageManager;
class CBalanceCurrencyMgr;		//	[종호]  20090526 통화량 조절 매니저

class CRacoonDodgeEventMgr;

class CServer_BBBMgr;		// [지연] 복불복 게임 메니져
class CDormancySystem;			//	[기형] 휴면 계정 시스템
class CRewardItemByProbMgr;		//  [기형] 확률 보상 아이템 메니저
class CPartyQuestMgr_Common;	//	[종호] 파티퀘스트 매니저;
class CValentineEventMgr;

class CHuntQuestItemMgr;

class CBoxItemMgr;

// 게임 상태 변수 		
enum eGameStatus{
	GS_INITCONNECTFRONTSERVER		= 1	,
	GS_CONNECTFRONTSERVER				, 

	GS_SHOWGOVERNMENT,	

	GS_INITSELECTWORLDLIST,
	GS_SELECTWORLDLIST, 

	GS_INITCONNECTGAMESERVER,
	GS_CONNECTGAMESERVER	,

	GS_INITCHARLIST			, 
	GS_CHARLIST				,

	GS_GAME,
	GS_GAMEDESTROY,
};

// 한게임 용 게임 상태 변수들

#define HANREPORT_CLIENT_GAMESTART			"300"

#ifdef _KOREA
	#define HANREPORT_CLIENT_GAMEEND		"301"
#else
	#ifdef _NHN_CHINA
		#define HANREPORT_CLIENT_GAMEEND		"331"
	#else 
		#define HANREPORT_CLIENT_GAMEEND		"900"
	#endif
#endif

#define HANREPORT_CLIENT_INITCOMPLETE		"302"

#define HANREPORT_CLIENT_GAMESTRING_FAILED	"305"

#define HANREPORT_CLIENT_LOGIN_REQUEST		"309"
#define HANREPORT_CLIENT_LOGINOK			"310"
#define HANREPORT_CLIENT_LOGIN_FAILED		"311"
#define HANREPORT_CLIENT_SELECTSERVER		"315"
#define HANREPORT_CLIENT_SELECTCHAR			"320"
#define HANREPORT_CLIENT_MAKECHAR			"325"

#define HANREPORT_CLIENT_LOGINTOGAMESERVER		"330"
#define HANREPORT_CLIENT_LOGOUTFROMGAMESERVER	"331"


#define HANREPORT_SERVER_STARTED			"400"
#define HANREPORT_SERVER_ENDED				"499"

#define _CCSAUTH_STATE_INIT					"410"	// 생성및 초기화.
#define _CCSAUTH_STATE_FIRST_AUTH_SENDED	"411"	// 첫번째 인증.
#define _CCSAUTH_STATE_FIRST_AUTH_CHECK		"412"	// 첫번째 인증 검사.
#define _CCSAUTH_STATE_N_AUTH_SEND			"413"	// N 번째 인증 전송.
#define _CCSAUTH_STATE_N_AUTH_CHECK			"414"	// N 번째 인증 검사.
#define _CCSAUTH_STATE_CLOSE				"415"	// 종료.


// 각종 네트워크 메시지 정의 . 
#include "MsgType.h"

/*
typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER    NameBuff [30];

}ASTAT, * PASTAT;*/

class cltCommonLogic;
class cltClient;
class cltServer;

cltCommonLogic*	GetCommonLogic();
cltClient* GetClientClass();
cltServer* GetServerClass();

NHashTableString< int >* GetGlobalHashTableString();


extern cltCommonLogic *pclClient;

#define MAX_SETCHAR_RESERVE_NUMBER		300

//---------------------------
// 개발툴 정보. 
//---------------------------
#define MAX_DEV_TOOL_NUM				10			// 개발툴 대화상자의 개수. 
#define DEV_TOOL_CHARINFO				0
#define DEV_TOOL_NPCINFO				1	
#define DEV_TOOL_PROFILEINFO			2
#define DEV_TOOL_SERVERINFO				3
#define DEV_TOOL_INHEREMAPCHAR			4			// 현재 내가 있는 멥의 케릭터/NPC에 관련된 정보를본다

#ifndef _CHINA
	const SI32 Recommendpoint[120] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										10, 4, 5, 6, 7, 8, 9, 10, 11, 20,
										12, 13, 14, 15, 16, 17, 18, 19, 20, 40,
										21, 22, 23, 24, 25, 26, 27, 28, 29, 60,
										35, 40, 45, 50, 55, 60, 65, 70, 75, 100,
										80, 90, 100, 110, 120, 130, 140, 150, 160, 200,
										120, 130, 140, 150, 160, 170, 180, 190, 200, 300,
										210, 225, 240, 255, 270, 285, 300, 315, 330, 400,
										360, 380, 400, 420, 440, 460, 480, 500, 520, 600,
										540, 560, 580, 600, 620, 640, 660, 680, 700, 800,
										720, 740, 760, 780, 800, 820, 840, 860, 880, 1000,
								   	
};
#else
	const SI32 Recommendpoint[120] = {	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
										1, 2, 3, 4, 5, 6, 7, 8, 9, 20,
										11, 12, 13, 14, 15, 16, 17, 18, 19, 30,
										21, 22, 23, 24, 25, 26, 27, 28, 29, 40,
										31, 32, 33, 34, 35, 36, 37, 38, 39, 60,
										41, 42, 43, 44, 45, 46, 47, 48, 49, 80,
										51, 52, 53, 54, 55, 56, 57, 58, 59, 120,
										61, 62, 63, 64, 65, 66, 67, 68, 69, 160,
										71, 72, 73, 74, 75, 76, 77, 78, 79, 200,
										81, 82, 83, 84, 85, 86, 87, 88, 89, 240,
										91, 92, 93, 94, 95, 96, 97, 98, 99, 280,
};    
#endif


// 캐릭터 정보를 서버에서 클라이언트로 보낼 때 
// 사용할 데이터 정보 
// 이 클래스에 값을 입력한 후에 클라이언트로 보낸다. 
class cltBasicCharUpdateInfo{
public:
	SI16 siCharUnique;
	SI08 siMoveSpeed;
	bool bUrgentMode;
	SI32 siLife;
	SI32 siReservedLife;
	SI32 siMana;
	SI16 siX, siY;
	UI08 uiGMMode;
	bool bPCRoom;
	SI08 siMakeAniType;

	cltBasicCharUpdateInfo(SI16 charunique, SI32 silife, SI32 reservedlife, SI32 mana, SI16 x, SI16 y, SI08 movespeed, bool burgent, UI08 gmmode, bool pcroom, SI08 makeanitype  )
	{
		siCharUnique	= charunique;
		siLife			= silife;
		siReservedLife	= reservedlife;
		siMana			= mana;
		siX				= x;
		siY				= y;
		siMoveSpeed		= movespeed;
		bUrgentMode		= burgent;
		uiGMMode		= gmmode;
		bPCRoom			= pcroom;
		siMakeAniType	= makeanitype;
	};
};

#define MAX_CRCFILECHECK_COUNT	1000

class NCRCFileCheck
{
public:
	NTCHARString128		m_FileName;
	UI32				m_siCRCCode;
	UI32				m_siFileSize;

public:
	NCRCFileCheck()
	{
		m_siCRCCode		= 0;
		m_siFileSize	= 0;
	}

	void Set(NCRCFileCheck* pFileCheck)
	{
		if(pFileCheck == NULL)			return;
        
		m_FileName		= pFileCheck->m_FileName;
		m_siCRCCode		= pFileCheck->m_siCRCCode;
		m_siFileSize	= pFileCheck->m_siFileSize;
	}
};

class NCRCFileCheckManager
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<NCRCFileCheck*, MAX_CRCFILECHECK_COUNT>	m_Files;
#else
	NCRCFileCheck*		m_Files[MAX_CRCFILECHECK_COUNT];
#endif
	SI32				m_siCount;
	CCRC32				m_CRC;

	NCRCFileCheckManager()
	{
#ifdef _SAFE_MEMORY
		m_Files.ZeroMem();
#else
		ZeroMemory(m_Files, sizeof(m_Files));
#endif
		m_siCount = 0;
	}

	~NCRCFileCheckManager()
	{
		for(SI32 i=0; i<MAX_CRCFILECHECK_COUNT; i++)
		{
			if( m_Files[i] != NULL )
				delete m_Files[i];
		}
	}

	void Init();
	void AddFile(const TCHAR* pszFileName);
	bool CheckFile(NCRCFileCheck* pCRCFileCheck);

	void RequestCheckFile();
};

template< class DataType >
class NHashTableString;

// 함수등록 define
#define LUA_RegisterCFunction(LUA, FUNC)		LUA.RegistCFunction( FUNC, #FUNC );

//---------------------------------------------
// cltCommonLogic
//---------------------------------------------
class cltCommonLogic : public NkMemory
{
protected:
	// 서버나 클라이언트나 공통으로 유지하는 변수. 
	HWND				    Hwnd;
	HINSTANCE				hInst;
	TCHAR					AppName[MAX_PATH];

	UI32					uiFrame;				// 게임이 진행된 정도. 

	SI32					siGameStatus;			// 게임의 현재 상태를 알리는 변수. 

	SI32					siFrameDelay;			// 한프레임당 소요 시간. 

	stWeatherData			CurrentWeather;			// 현재의 날씨. 

	//-----------------------------------
	// 게임의 버전. 
	//-----------------------------------
	SI32	siVersion;


public:
	//----------------------------------
	// 서비스 지역 
	//----------------------------------
	SI32					siServiceArea;

	//----------------------------------
	// 서비스 모드
	//----------------------------------
	SI32					siRunMode;

	//-------------------------------------------
	// 사용 언어 ( 게임 내 문자 출력 관련 ) 
	//-------------------------------------------
	SI32 siLanguage; 

	//-------------------------------------------
	// 운영 체제 버젼 
	//-------------------------------------------
	//osvi.dwPlatformId , osvi.dwMajorVersion , osvi.dwMinorVersion ,  osvi.wProductType
	DWORD dwPlatformId;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	BYTE  wProductType;

	//----------------------------------
	// 테스트용
	//----------------------------------
	SI32					siMaster;

	// 각 서버의 기본 정보. 
	SI16					siServerUnique;
	TCHAR					szServerID[20];	// CNC, CTC구분, 지역구분, 서버구분
	TCHAR					szServerName[20];
	bool					bItemMallUseSwitch;
	SI32					siMarketAutoBuyDelayFrame;
	SI32					siHorseMarketAutoBuyDelayFrame;

	// SJY
	SI32					siEventItemDropRate;

	SI32					siMinimumTeacherLevel;
	
	cltMapManager*			pclMapManager;

	cltConfig*				pclConfig;

	//	cltTextMgr*				pclTextMgr;
	//	cltTextMgr*				pclStructureMgr;

	cltDiseaseManager*		pclDiseaseManager;
	cltCharManager*			pclCM;
	cltKindInfoSet*			pclKindInfoSet;

	cltPersonNameManager*		pclPersonNameManager;
	cltNewPersonNameManager*	pclNewPersonNameManager;

	cltSkillManager*		pclSkillManager;
	cltItemManagerCommon*	pclItemManager;

	cltBulletManager*		pclBulletManager;
	cltSmokeManager*		pclSmokeManager;

	cltTileManager*			pclTileManager;				// 지도가 사용할 타일 세트에 대한 정보를 저장한다. 
	cltFieldObjectManager*	pclFieldObjectManager;		// 지도가 사용할 필드 오브젝트 세트에 대한 정보를 저장한다. 

	// 개발에 필요한 각종 툴. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltDevTool*, MAX_DEV_TOOL_NUM>		pclDevTool;
#else
	cltDevTool*			pclDevTool[MAX_DEV_TOOL_NUM];
#endif


	// 각 시설물들 관리자. 
	cltBankManager*				pclBankManager;
	cltStockManager*			pclStockManager;
	cltCityHallManager*			pclCityHallManager;
	cltMarketManager*			pclMarketManager;
	cltHouseManager*			pclHouseManager;
	cltHuntManager*				pclHuntManager;
	cltPostOfficeManager*		pclPostOfficeManager;
	cltHorseMarketManager*		pclHorseMarketManager;
	cltRealEstateMarketManager*	pclRealEstateMarketManager;
	cltSummonMarketManager*		pclSummonMarketManager;
	cltLandManager*				pclLandManager;
	cltFeastManager*			pclFeastManager;
	cltSummonHeroMarketManager* pclSummonHeroMarketManager;
	cltMineManager*				pclMineManager;
	cltGuildManager*			pclGuildManager;
	//cltNewMarketManager*		pclNewMarketManager;
	cltNewMarketManager*		pclNewMarketManager;


	cltVillageManager*			pclVillageManager;			// 마을 정보를 저장하는 공간이다. 
	cltVillageStructureSet*		pclVillageStructureSet;
	cltAttackTypeInfoManager*	pclAttackTypeInfoManager;
	cltSystemNPCManager*		pclSystemNPCManager;
	CForeignMerchantNPCMgr*		pclForeignMerchantNPCMgr;		// 교역 상인(청, 일본) NPC
	CTradeMerchantNPCMgr*		pclTradeMerchantNPCMgr;			// 무역 상인 NPC 
	CRPMerchantNPCMgr*			pclRPMerchantNPCMgr;			// 추천 포인트 상품 관리자
	CPrivateTradeOrderMgr*		pclPrivateTradeOrderMgr;		// 개인간 거래	
	CMagicMgr*					pclMagicManager;
	cltOrderManager*			pclOrderManager;
	cltQuestManager*			pclQuestManager;

	cltNamingQuestManager*		pclNamingQuestManager;			// 네이밍 퀘스트

	CBlackWarManager*			pclBlackWarManager;				// 흑의군단 전쟁이벤트( 광명 )
	cltBlackArmyManager*		pclBlackArmyManager;			// 흑의군대 이벤트 매니저
	cltKoinoboryManager*		pclKoinoboryManager;			// 코이노보리 이벤트 매니저.
	cltSantaRaccoonManager*		pclSantaRaccoonManager;			// 산타구리댁 이벤트 매니저
	cltKoinoboryFamilyManager*  pclKoinoboryFamilyManager;		// 코이노보리 가족 이벤트 매니저.

	CGachaManager*				pclGachaManager;				// 가챠 레어 아이템 목록 메니져
	CGacha2Manager*				pclGacha2Manager;			//KHY - 1104 -  갓차2 -라쿤 보물 상자. 

	CArtifactRestoreManager*	pclArtifactRestoreManager;	// [추가 : 황진성 2008. 3. 4 => 아티펙트 환원에 사용되는 아이템 정보들.]

    CFourLeafManager*			pclFourLeafManager;

	CPartyMgr*					pclPartyMgr;		// 파티 관련
	CStockDistributionMgr*		pclStockDistributionMgr;
	CVoteMgr*					pclVoteMgr;			// 투표
	CDailyQuestMgr*				pclDailyQuestMgr;	// Daily 퀘스트
	CDailyQuest2Mgr*			pclDailyQuest2Mgr;	// Daily 퀘스트2
	CDailyQuest3Mgr*			pclDailyQuest3Mgr;	// Daily 퀘스트3
	CFishingMgr*				pclFishingMgr;		// 낚시
	CFarmingMgr*				pclFarmingMgr;		// 농경
	CForeignMerchantNPCBuyCashItem*	pclForeignMerchantNPCBuyCashItem;	// 캐쉬 아이템 구입

	CMiningMgr*					pclMiningMgr;

	cltMonsterGroupManager*		pclMonsterGroupManager;

	cltWarRankManager*			pclWarRankManager;
	cltMasterManager*			pclMasterManager;

	//	cltItemMallManager*			pclItemMallManager; // 아이템 몰 매니저
	//	cltItemMallButtonManager*	pclItemMallButtonManager; // 아이테 몰 버튼 매니저

	cltAutoHuntingManager*		pclAutoHuntingManager; // 자동 사냥 체크 매니저

	// NPC관리자.
	cltNPCManagerCommon* pclNPCManager;
	cltHiredNPCPayManager *pclHiredNPCPayManager;

	//	cltSchoolManager*		pclSchoolManager;
	cltFatherManager*		pclFatherManager;

	cltHorseManager*		pclHorseManager;

	cltGameMasterManager*   pclGMManager;

	cltEventHunting*		pclEventHunting;

	cltMofuMofuEvent*		pclMofuMofuEvent;

	cltGameEvent*			pclGameEventManager;

	CMonsterAttackEvent*	pclMonsterAttackEvent;

	cltWorldTraderManager*	pclWorldTraderManager;
	cltDistributionWorldMoney* pclDistributionWorldMoney;

	cltCountryManager*		pclCountryManager;		
	// 일본판에서 무역에도 쓰이기 때문에 하나 더 생성
	cltCountryManager*		pclCountryManagerJapan;	

	cltGuildDungeonItem*	pclGuildDungeonItemMgr;
	CAbuseFilter			*m_pCAbuseFilter;

	cltEventStructure*		pclEventStructure;

	cltEventTimeManager*	pclEventTimeManager;

	NCRCFileCheckManager*	pclCRCFileCheckManager;		// CRC코드를 이용하여 파일을 체크하는 메니져

	cltDevServerInfo*		pclserverinfoDlg;

	CVoteStatusMgr*			pclVoteStatusMgr;			// [영훈] 선거 개표관련 메니져

	CNServerEvent_Manager*  pclServerEventManager;		// [진성] 서버 이벤트 메니져. => 2008-6-20

	cltHelperManager*		pclHelperManager;			// [진성] 도우미 => 2008-10-14

	CPVP_LeagueMgr*			pclPVP_LeagueMgr;			// [성웅] pvp_League

	cltGuildHuntMapWarManager*	pclGuildHuntMapManager ;	// [성웅] 길드 사냥터 소유 길드전 매니저. 2009-03-27

	cltGuildHuntMapWarStatMgr*	m_pclGuildHuntMapWarStatMgr;	//[종호]사냥터소유방식(Random) 길드전

	CRacoonDodgeEventMgr*	pclRacoonDodgeEventMgr;				//[성웅] 라쿤 피하기 이벤트 //Switch_10MonthEvent_RacoonDodge

	CDormancySystem*		pclDormancySystem;			// [기형] 휴면 계정 시스템

	CValentineEventMgr*		m_pclValentineEventMgr; // [기형] 발렌타인 이벤트

	CServer_BBBMgr*			m_pServer_BBBMgr;		// [지연] 복불복 이벤트 메니져

	CServer_PartyMatchingMgr*	m_pServer_PartyMatchingMgr;		// [지연] 파티매칭 매니저

	TCHAR	SavePath[256];								// 현재 게임이 설치된 경로 

	CRewardItemByProbMgr*	m_pclRewardItemByProbMgr;	// 확률 보상 아이템 매니저

	CHuntQuestItemMgr*		m_pHuntQuestItemMgr;

	// 메시지 관련
	cltMessage* pclMessage;

	SI32 GameMode;							// 이 게임이 어떤 게임인가 ?(서버, 클라이언트, 맵에디터등.. ) 
	// GAMEMODE_SERVER
	// GAMEMODE_CLIENT
	// GAM DE_MAPEDIT


	//--------------------------------------------
	// 커서관리
	//--------------------------------------------
	cltCursor*		pclCursor;


	//------------------------------------
	// 게임에 사용되는 시계변수 
	//------------------------------------
	DWORD StartClock, CurrentClock;

	//----------------------------------
	//	NPC용 아이템 정보. 
	//----------------------------------
	cltNPCItemInfo* pclNPCItemInfo;

	//---------------------------------
	// 오류 기록. 
	//---------------------------------
	CLogPrint* pclLog;

	NHashTableString< int >*	pclHashTableString;
	NHashTableString< SI64 >*	pclHashTableGlobalValue;

	//
	cltMarriageManager*			m_pclMarriageMgr;

	CBalanceCurrencyMgr*		m_pclBalanceCurrencyMgr;	//	통화량 조절 매니저

	cltSoulGuardMgr*			m_pclSoulGuardMgr;

	//---------------------------------
	// 상자류 아이템 관련 메니져
	//---------------------------------
	CBoxItemMgr*				m_pclBoxItemMgr;

	//---------------------------------
	// 서버와 클라이언트가 공유해야할 정보. 
	//---------------------------------
	bool	bVillageWarSwitch;		// 공성전 가능한 시간인지 여부 
	bool	bBonusTimeSwitch;		// 경험치를 2배로 주는 프리미엄 타임인가가여부. 
	bool	bBonus2TimeSwitch;		// 제조 경험치를 2배로 주는 프리미엄 타임인가가여부. 
	bool	bSpecialQuestBonusTimeSwitch;
	bool	bBroadcastSwitch;		// 방송중인지 여부. 
	SI08	siBestVillageLevel;		// 최고 마을 레벨 
	bool	bBonusTimeGoonzu;		// 군주 보너스 타임 여부


	//---------------------------------------------------------------------------------
	bool	bTimeCount	;		// TimeSet으로 셋팅된 시간의 타임 카운트를 셋팅 한다 .
	float	siTimeSet	;
	SI32	siRoundCount	;	// 보여줄 라운드 
	DWORD	dwWriteSetTime	;	// 타임 셋 한 시간 기억 1초씩 빼기 위해서 
	

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_EVENT_NUMBER>		bEventSwitch;
#else
	bool	bEventSwitch[MAX_EVENT_NUMBER];		
#endif
	bool	bEnchantSwitch;

	SI16	siServiceAge;		// 서버의 접속 가능 연령
	SI08	siEnablePVP;		// PVP가능 여부

	SI16 sUpViewEffect; //KHY - 0723 - 기간아이템 파괴 방지. 기간 종료 표시. 0= 없음. 1 = 기간지난 아이템.

	// [영훈] Auto Unblock - 2008.02.21
	bool	bAutoUnblock;

	// [영훈] 선거 유세기간임을 알림
	bool	bVoteCampaign;

	// [영훈] 이벤트를 On/Off하는 스위치(EVENT_TOGGLE_SWITCH에 새로운 나열자를 추가하면 다른 것도 가능)
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, EVENT_TOGGLE_SWITCH_MAX>	bEventToggleSwitch;
#else
	bool				bEventToggleSwitch[EVENT_TOGGLE_SWITCH_MAX];
#endif

	NBitArray<Switch_Last>		m_bitArray_SwitchContry;

	bool LoadSwichCountry();
	SI32 ConvertSwitchCountryToIndex(TCHAR* pszSwitch);
	bool IsCountrySwitch( SI32 siSwitchCountry );

#ifdef _SAFE_MEMORY
	NSafeTArray<cltAttendanceReward*, MAX_ATTENDANCE_COUNT>		m_pclAttendanceReward;
#else
	cltAttendanceReward*	m_pclAttendanceReward[MAX_ATTENDANCE_COUNT];
#endif

	NLua					m_clLua;
	
	// [춘기] 크리스마스 이벤트 - 맵에 설치되는 월드트리 ID
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_WORLDTREE_COUNT>		siWorldTreeID;
#else
	SI32	siWorldTreeID[MAX_WORLDTREE_COUNT];
#endif

	// [춘기] 크리스마스 이벤트 - 트리가 건설되었는지 여부
	bool	bWorldTreeConstruct;
	DWORD	dwWorldTreeCreateClock;

public:
	//--------------------------
	// Virtual Methods
	//--------------------------
	virtual void InitNetwork(){};
	virtual TSpr* GetGlobalSpr(SI32 index){return NULL;}
	virtual bool SendDBMsg(SI32 siselect, sPacketHeader *pPacket){return false;}
	virtual	TCHAR* GetToolTip(SI32 idcommand){return NULL;}
	virtual void SetStatusBarText(TCHAR* text, SI32 index){};
	virtual SI32 GetScreenRX()const {return 0;}
	virtual SI32 GetScreenRY()const {return 0;}
	virtual bool DoMsg_WM_COMMAND(WPARAM wParam){return false;}
	virtual void ClientWinMain(){}
	virtual void PushKey(cltKey* pclkey){}
	virtual BOOL SendMsgToServer(sPacketHeader* pmsg){return FALSE;}

	cltCommonLogic( TCHAR* appname, HWND hwnd , HINSTANCE hinst, SI32 gamemode, SI32 servicearea, SI32 runmode );
	virtual ~cltCommonLogic();

	void MainInit();

	HWND GetHwnd()const{return Hwnd;}
	void SetHwnd(HWND hwnd){Hwnd = hwnd;}

	HINSTANCE GetInstance()const{return hInst;}
	void SetInstance(HINSTANCE hinst){hInst = hinst;}

	void SetFrame(UI32 frame){uiFrame =  frame;}
	UI32 GetFrame()const{return uiFrame;}
	void IncreaseFrame(){uiFrame ++;}

	//----------------------------------------
	// 초기화. 
	//----------------------------------------
	virtual void Init(){};
	virtual void Create();

	//--------------------------------------------
	// 모달리스 대화상자 처리 관련 
	//--------------------------------------------
	BOOL ModalessDlgProc(MSG* pmsg);

	//-------------------------------------------
	// 게임 지연 
	//-------------------------------------------
	SI32 GetFrameDelay()const{ return siFrameDelay;}
	void SetFrameDelay(SI32 delay){siFrameDelay = delay;}

	//--------------------------------------------
	// 마우스 조작 관련 
	//--------------------------------------------
	virtual BOOL GameOverHead(){return FALSE;};

	//---------------------------------------------
	// 캐릭터 선택 관련. 
	//---------------------------------------------
	virtual BOOL IsUser(SI32 charunique){return FALSE;}			


	//---------------------------------------------
	// 
	//---------------------------------------------
	SI32 GetGameStatus()const{return siGameStatus;}
	void SetGameStatus(SI32 sistatus){ InterlockedExchange( (LPLONG)&siGameStatus, sistatus ); };

	//---------------------------------------------
	// SavePath 및 Reg 처리 
	//---------------------------------------------
	const TCHAR* GetSavePath()const{return SavePath;}

	//---------------------------------------------
	// 네트워크 관련 
	// CommonLogic-Network.cpp
	//---------------------------------------------
	virtual void NetworkOperation(){}
	virtual void  SetSessionChar() = 0;

	virtual BOOL InBufferMessageOperation(cltMsg* pclMsg, SI32 id){return FALSE;}

	//-----------------------------------------------
	// 캐릭터 처리 
	// SetChar.cpp
	//-----------------------------------------------
	// 가장 기본이 되는 캐릭터를 생성한다. 
	bool MakeSimpleChar(SI32 id, SI32 charunique);

	// 일반 캐릭터를 설정한다. 
	SI32 SetChar(SI32 id, cltMakeCharInfo* pclmakecharinfo, cltGMCommandKindInfo* pclGMCommandKindInfo = NULL );

	// 일반 캐릭터를 설정한다. 
	SI32 SetChar1(SI32 id, SI32 charunique, SI32 childcharunique, SI32 summonindex, cltIdentity* pclidentityinfo, cltPersonInfo* pclUserCharInfo, cltInnerStatusInfo* pclInnerStatus, SI16 mySoulGuardUnique);

	//----------------------------------------------
	// 명령 처리 
	// CommonLogic-order.cpp
	//----------------------------------------------
	// 플레이어의 명령을 전달한다. 
	BOOL LetOrder(cltOrderData *porder,SI32 id = 0);

	//---------------------------------------------
	// DevCharInfo
	//---------------------------------------------
	virtual BOOL CreateDevCharInfoDlg();
	static BOOL CALLBACK StaticDevCharInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------
	// DevNPCInfo
	//---------------------------------------------
	virtual BOOL CreateDevNPCInfoDlg();
	static BOOL CALLBACK StaticDevNPCInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------
	// DevProfileInfo
	//---------------------------------------------
	virtual BOOL CreateDevProfileInfoDlg();
	static BOOL CALLBACK StaticDevProfileDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------
	// DevServerInfo
	//---------------------------------------------
	virtual BOOL CreateDevServerInfoDlg();
	static BOOL CALLBACK StaticDevServerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//---------------------------------------------
	// DevInHereMapChar
	//---------------------------------------------
	virtual BOOL CreateDevInHereMapCharDlg();
	static BOOL CALLBACK StaticDevInHereMapCharDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	//-------------------------------------------
	// About 관련 
	//-------------------------------------------
	BOOL About();
	static BOOL CALLBACK StaticAboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	//----------------------------------------
	// Char Information Update
	//----------------------------------------
	virtual BOOL SendCharUpdate(SI32 charunique, bool bmode){return FALSE;};

	//------------------------------------------------------
	// Village 관련 
	//------------------------------------------------------
	void Village();


	// 로그인 경로에 따른( goonzu, sbs ) url 얻어오기
	void GetGoonZuUrl( TCHAR *url, TCHAR *szID );						// 군주 홈페이지 
	void GetMyCZUrl( TCHAR *url, TCHAR *szID );						// 나의 꼼지 
	void GetCharCZUrl( TCHAR *url, TCHAR *szMyID, TCHAR *szID );		// 상대방 꼼지 

	void GetCZProfileUrl( TCHAR *url, TCHAR *szID );
	void GetCZDarakUrl( TCHAR *url, TCHAR *szID );
	void GetCZIntroUrl( TCHAR *url, TCHAR *szID );

	void GetSubSiteUrl(TCHAR * url,TCHAR *szSubPath,TCHAR *szID );		// 기타 서브 사이트 연결 - 아이템 몰 등
	void GetHelpSiteUrl(TCHAR * url,TCHAR *szID);

	void GetCPasswdUrl(TCHAR *url,TCHAR *szID);						// 비밀번호 변경 페이지

	// 텍스트 매니저에서 텍스트를 얻어온다.
	//	TCHAR * GetTxtFromMgr(SI32 index , bool bStructure = false );


	//-----------------------------------------------
	// 사운드 관련 
	//-----------------------------------------------
	virtual BOOL PushEffect(SI32 effect, SI32 x, SI32 y){return FALSE;}


	//-----------------------------------------------
	// 초기화 
	//-----------------------------------------------
	// 실제 게임의 루틴이 들어있는 함수 
	// 리턴값: 0 -> normal
	SHORT GameProcess();
	virtual SHORT GameAction() { return 0; }

	bool IsChangeFileExist();
	void ChangeFile();

	//-----------------------------------
	// 게임의 버전. 
	//-----------------------------------
	SI32 GetVersion()
	{
		/*
		WIN32_FIND_DATA	wfd;

		HANDLE hSrch = FindFirstFile( TEXT("GoonZuRun1.exe"), &wfd );

		if ( hSrch != INVALID_HANDLE_VALUE )
		{
		if ( DeleteFile(  TEXT("Patch.exe") ) == TRUE )
		{
		rename( TEXT("GoonZuRun1.exe"), TEXT("Patch.exe") );
		CopyFile( TEXT("Patch.exe"), TEXT("GoonZuRun.exe"), FALSE );
		}
		else
		{
		DeleteFile(  TEXT("GoonZuRun.exe") );
		rename( TEXT("GoonZuRun1.exe"), TEXT("GoonZuRun.exe") );
		}

		FindClose( hSrch );
		}
		*/

		return siVersion;
	}
	void SetMasterArea();			// master.dat 파일 읽기

	cltNPCGroup* GetNPCGroupPointer(SI32 npcgroup);

	SI32 GetUniqueFromHash( TCHAR* pText );
	UI32 GetMinuteFromsTime( _SYSTEMTIME time );
	_SYSTEMTIME GetTimeFromsMinute( UI32 datevary );
	cltDate SysteTimeToCltTime( _SYSTEMTIME time );

	SI32 GetMaxStaminaValue(SI32 summonskill, SI32 summonkind);
	SI32 GetSummonAttackBonus(SI32 summonskill);
	SI32 GetSummonDefBonus(SI32 summonskill);

	//----------------------------------------------
	//TradeServer
	//----------------------------------------------
	void GetServerNameFromNation(char*code,SI16 index,char*name);
	bool GetNationCode( OUT char* pszNationCode, IN SI32 siSize );	// 지금 현재 국가 코드(영문3글자)를 가져온다

	//---------------------------------------------
	//Recommand Point
	//---------------------------------------------
	SI32 GetRecommendPointPerLevel(SI32 level);

	//---------------------------------------------
	bool IsWhereServiceArea( SI32 ServiceArea );

	bool GetItemByAttendanceDay( SI32 AttendenceDay, cltItem* pclItem );
	bool SetItemByAttendanceDay( SI16 siAttendenceDay, SI16 siItemUnique, SI16 siItemNum, UI16 uiDateUseDay );

	bool LoadGlobalValue();
	SI64 GetGlobalValue(TCHAR* pszGlobalValue);

	//----------------------------------------------
	// Lua 관련
	//----------------------------------------------
	bool InitLua();

	SI32 GetTextColumn();
};


#endif
