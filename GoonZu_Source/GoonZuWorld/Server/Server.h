//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _SERVER_H
#define _SERVER_H

#include "..\Client\Client.h"
#include "Rank\Rank.h"

#include "Statistics/MoneyStatistics/MoneyStatistics.h"

#include "GameProtocol\GameProtocol.h"
#include "FrontAuthProcess\FrontAuthProcess.h"
#include "GameDBProcess\LogDBProcess.h"
#include "GameDBProcess\GameDBProcess.h"
#include "GameDBProcess\GameDBProcess2.h"
#include "AgentProcess\AgentProcess.h"
#include "BillProcess\BillProcess.h"
#include "NTradeServerProcess/NTradeServerProcess.h"
#include "../CommonLogic/Guild/GuildWar.h"
#include "../CommonLogic/Guild/Guild-Dungeon.h"
#include "../common/Item/ItemCommon/Item-BookBox.h"
#include "../common/item/ItemCommon/Item-GodHand.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Web.h"
#include "../common/BokBulBokEventMgr/BokBulBokEvent.h"

#include "../CommonLogic/PartyMatchingMgr/Server_PartyMatchingMgr.h"
#include "../CommonLogic/Msg/MsgType-PartyMatching.h"
#include "PartyQuestMgr/PartyQuestMgr_Server.h"

#include "..\Common\Weather\Weather.h"

#include "NMsgShareBuffer.h"
#include "IPToCountry\IPToCountry.h"

#include "Msg/MsgType-Summon.h"

#include "../Client/CheckProcess/CheckProcess.h"

// -HanAuth_Lib
#ifdef USE_HANREPORT
	#ifdef _SERVER
		#include "../HanAuth/HanReportForSvr.h"

		#ifdef _DEBUG
			#pragma comment(lib, "HanReportForSvrD.lib") 
		#else
			#pragma comment(lib, "HanReportForSvr.lib") 
		#endif // _DEBUG
	#endif // _SERVER
#endif // USE_HANREPORT
// -HanAuth_Lib

#ifdef USE_HANIPCHECK
	#ifdef _NHN_CHINA
		#include "../HanAuth/HANIPCHECK_CHI/HanIPCheckForSvr_CHI.h"
		#ifdef _DEBUG
			#pragma comment(lib, "HanIPCheckForSvrD_CHI.lib") 
		#else
			#pragma comment(lib, "HanIPCheckForSvr_CHI.lib")
		#endif
	#else
	#include "../HanAuth/HanIPCheckForSvr.h"
		#ifdef _DEBUG
			#pragma comment(lib, "HanReportForSvrD.lib") 
		#else
			#pragma comment(lib, "HanReportForSvr.lib")
		#endif
	#endif // _NHN_CHINA
#endif // USE_HANREPORT

#ifdef USE_HANXNOTI
	//#ifdef _SERVER
		#include <XDR/XDR.h>
		
		#include <XnotiAppMessage.h>
		#include <XNotiConnectorAPI.h>

		#include <GoonZuGMT.h>
		#include <SHOP.h>		// 웹 상점

		#ifdef _DEBUG
		#else
			#pragma comment(lib, "XPlatform.lib") 
			#pragma comment(lib, "XStream.lib") 
			#pragma comment(lib, "XSystem.lib") 
			#pragma comment(lib, "Connector.lib") 
			#pragma comment(lib, "Log4XCore.lib") 
			#pragma comment(lib, "Log4XBase.lib") 
			#pragma comment(lib, "XNotiConnector.lib") 
			#pragma comment(lib, "XInNetwork.lib") 
		#endif
	//#endif
#endif // USE_HANXNOTI

#ifdef USE_HAN_BAN_CHECK
	#ifdef _SERVER
		// 한게임 설문 조사 모듈
		#include "../HanBanCheckForSvr/HanBanCheckForSvr.h"

		#ifdef _DEBUG
			#pragma comment(lib, "HanBanCheckForSvrD.lib") 
		#else
			#pragma comment(lib, "HanBanCheckForSvr.lib") 
		#endif
	#endif		
#endif

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#ifdef _SERVER
		#include "../Porte/PorteAuthForSvr.h"	
		using namespace Porte::PorteAuthSystem;
#endif		
#endif // USE_PORTEAUTH

//////////////////////////////////////////////////////////////////////////
// [진성] 중국 프로모터. 
#include "PromoterManager/clPromoterManager.h"
#include "../Promoter/NHNChinaPromoter.h"

#ifdef USE_PROMOTOR_SERVER	
	
	// 제공받은 모듈
	#ifdef _DEBUG	
		#ifndef _DBMANAGER
			#pragma comment(lib, "NHNChinaPromoterDllD.lib") 
		#endif
	#else
		#pragma comment(lib, "NHNChinaPromoterDll.lib") 
	#endif

#endif
//////////////////////////////////////////////////////////////////////////
#include "SummonTransformManager/SummonTransformManager.h"

#include "../CommonLogic/BattleRoyal/CBattleRoyalMgr.h"

#include "../Server/Event/ContributionEvent_Common/ContributionEvent_Common.h"
#include "../Server/Event/GiantRaccoon_Resurrect_Event/GiantRaccoonResurrectEvent.h"

#include "MacroReportedMgr/MacroReportedMgr.h"
#include "GZFtpMgr/GZFtpMgr.h"

struct stHanXNoti_Waiting
{
	SI32			m_siIdx;
	HANDLE			m_hWaitingHandle;
};

// KHY - 0901 -  과금 아이템 자동구매 설정.
#define MAX_SYSTEMBUYTEM_NUM				50

struct SystemBuyItem
{
	SI16			m_siUnique;
	GMONEY			m_gPrice;
};


#define HANXNOTI_MAX_WAITING				16

#define MRS_ERRORCODE_SUCCESS				0
#define MRS_ERRORCODE_FAILED				1

#define MRS_ERRORCODE_INVALID_COMMAND		10
#define MRS_ERRORCODE_INVALID_PARAMETER		11

#define MRS_ERRORCODE_INVALID_CHARNAME		100

#define MRS_ERRORCODE_INVALID_ITEMNAME		200
#define MRS_ERRORCODE_INVALID_ITEMCOUNT		201

#define MRS_ERRORCODE_SYSTEM_TIMEOUT		300
#define MRS_ERRORCODE_SYSTEM_ERROR			301
#define MRS_ERRORCODE_SYSTEM_MSGFULL		302

#define MRS_ERRORCODE_ADDINVENTORYITEMREQ_FAILED	-1
#define MRS_ERRORCODE_DELETEINVENTORYREQ_FAILED		-1

extern NMsgShareBuffer g_msgsharebuffer;

#define WRITE_MEMLOG( a, b, c, d )		g_msgsharebuffer.WriteMsg( a, b, c, d );

#define WRITELOG_DBMSG				100
#define WRITELOG_DBMSG2				101
#define WRITELOG_DBLOG				102
#define WRITELOG_GAMEMSG			110


#define EVENT_NPC_COUNT			5
#define MAX_RANDOM_ROC_COUNT	100	// 랜덤 좌표 생성을 위해 최대한 도는 반복 횟수

// 셀방식 관련
#include "ITZWorld.h"
#include "ServerMap/ServerMap.h"
#include "ItemList\ItemList.h"

class cltLetterHeader;
class cltNoticeManager;
class cltStrInfo;
class HashStringInt;
class HashIntInt;
class CMinisterMgr;
class cltItemMallManager;
class cltMonsterGroupManager;
class CPartyObj;
class cltStockOfferingManager;
class cltIDNumTable;
class GMProtocol;
class CKingGuriEvent;
class CEmpireManager;
class CSantaRaccoon;				//[추가 : 황진성 2007. 11. 29 산타라쿤 클레스.]
class cltRewardMgr;
class cltNewSelectBoxMgr;
class CGawiBawiBoMgr_Server;
class CBattleRoyalMgr;					// 배틀로얄 매니져
class cltContributionEvent_Common;
class cltGiantRaccoonResurrectEvent;	// 자이언트 라쿤 부활


class CGoonzuKingGuriEvent;//[성웅] 라쿤이벤트 군주 이벤트로 변경

struct sGameServerRefreshInfo;

#define REPAIR_ERR_NOTSAMEITEM	1	// 수리 요청한 아이템과 실제 아이템이 다르다. 
#define REPAIR_ERR_SETITEM		2	// 세트 아이템은 사람이 수리할 수 없다.

#define MAX_SPECIAL_QUEST_BONUS_BUFFER_SIZE		1000	// 흥부박 퀘스트 완료자 버퍼 사이즈 
#define MAX_SPECIAL_NEWS_BUFFER_SIZE			1000

//-------------------------------------------
// NPC의 아이템 보유 상황 
//-------------------------------------------
#define NPCITEMMODE_NONE		0	// 아이템 보유 안함. 
#define NPCITEMMODE_NORMAL		1	// 일반적인 아이템 보유. 
#define NPCITEMMODE_HUNT		2	// 수렵장 몬스터로 아이템 보유확률 증대. 
#define NPCITEMMODE_DOCKDO		3	// 독도 몬스터로 아이템 보유확율 감소
#define NPCITEMMODE_GM			4	// GM이 만드는 몬스터로 대박 아이템 없음


//----------------------------------------
// Cash지급 관련 
//----------------------------------------
#define MAX_PUSH_CASH_LIST_NUMBER	(MAX_PERSON_NUMBER)
#define PUSHCASH_REASON_QUEST		1		// 퀘스트 보상으로 지급. 
#define PUSHCASH_REASON_GAMEMASTER	2		// 운영자권한으로 지급. 
#define PUSHCASH_FOREIGNMERCHANTBUY	3		// 상인이 유료 아이템 구입
#define PUSHCASH_REASON_ATTENDANCE	4		// 출석으로 지급. 
#define PUSHCASH_REASON_QUESTEVENT	5		// 퀘스트 이벤트로 지급

#define WARLOSS_STOCK				1000	// 전쟁 패배시 주식 소각 기준 

//----------------------------------------
//cyj 주주총회, 공성전 자막뉴스 간격(분단위)
//----------------------------------------
#define GENERALMEETING_NEWS_INTERVAL	10
#define VILLAGEWAR_NEWS_INTERVAL		5

#define BLACKARMY_NEWS_INTERVAL			1		// 시간단위

enum HUNAMAPWARP_MODE
{
	HUNTMAPWARP_MODE_NONE,
	HUNTMAPWARP_MODE_USEMONEY,
	HUNTMAPWARP_MODE_USEITEM,
};

enum DBINITIALIZE_STEP
{
	DBINITIALIZE_STEP_NONE,
	DBINITIALIZE_STEP_1,
	DBINITIALIZE_STEP_2,
};

#define FINDTREASURE_CUTLEVEL	-21	// 보물찾기 이벤트 몬스터를 잡을때 날수 있는 레벨차이의 최저 래벨차이
#define	MAX_TREASUREEVENT_POSITION_NUM	30		// 보물찾기 이벤트 한맵에 가지고 있는 최대 좌표 갯수
#define	MAX_TREASUREEVENT_USEMAP_NUM	100		// 보물찾기 이벤트에서 사용할 맵 최대 갯수
class cltPushCash{
public:
	UI16 usCharID;
	SI32 siPersonID;
	TCHAR szAccountID[MAX_PLAYER_NAME];
	GMONEY siMoney;
	SI32 siReason;
	SI32 siDay;

	cltPushCash()
	{
		usCharID = 0;
		siPersonID = 0;
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, TEXT(""));
		siMoney = 0;
		siReason = 0;
		siDay = 3;
	}
	void Set(cltPushCash* pclinfo)
	{
		usCharID = pclinfo->usCharID;
		siPersonID	= pclinfo->siPersonID;
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, pclinfo->szAccountID);
		siMoney		= pclinfo->siMoney;
		siReason = pclinfo->siReason;
	}

	void Set( UI16 charid, SI32 personid, TCHAR* szaccountid, GMONEY money, SI32 reason, SI32 day = 3 )
	{
		usCharID = charid;
		siPersonID	= personid;
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, szaccountid);
		siMoney		= money;
		siReason = reason;
		siDay = day;
	}
};


class cltServerStartInfo
{
public:
	SI32	siStockDistributionVillageUnique;		// 주식 공모 중이었다면, 공모 중이었던 마을 Index

	void Set( cltServerStartInfo *pclServerStartInfo )
	{
		memcpy ( this, pclServerStartInfo, sizeof( cltServerStartInfo ) );
	}
};

struct GMServerInfo
{
	BOOL		bAutoStock;
	BOOL		bAutoWar;
};

class cltCurrencyInfo{
public:
	cltMoney clInputMoney_NPC;				// NPC가 공급하는 자금. 
	cltMoney clInputMoney_Quest;
	cltMoney clInputMoney_War;
	cltMoney clInputMoney_Father;			// 제자 양성에 의한 투입 자금. 
	cltMoney clInputMoney_Recommender;		// 추천인에 의한 투입 자금. 


	cltMoney clOutputMoney_NPC;				// NPC가 소모하는 자금. 
	cltMoney clOutputMoney_Dead;			// 사망에 의한 손실 
	cltMoney clOutputMoney_Repair;			// 수리에 의한 소모. 
	cltMoney clOutputMoney_Stock;			// 주식 구입에 의한 손실. 
	cltMoney clOutputMoney_clMarketTax;		// 객주 세금에 의한 회수. 
	cltMoney clOutputMoney_clStockBidTax;	// 증자세에 의한 회수. 
	cltMoney clOutputMoney_clVillageWar;	// 공성전을 위해 투입되는 자금 
	cltMoney clOutputMoney_NPCPayment;		// 마을 주민 고용비 및 급여 
	cltMoney clOutputMoney_QuestFee;		// 퀘스트시 지불 수수료
	cltMoney clOutputMoney_TradeTax;		// 무역 세금에 의한 회수.
	
	cltMoney clTotalBudget;					// 재정 
	cltMoney clReservedBudget;				// 예비재정 		
	cltMoney clOutputMoney_clNewMarketTax;	// 통합시장 세금에 의한 회수. 


	cltCurrencyInfo()
	{
		Init();
	}

	void Init()
	{
		clInputMoney_NPC.Init();
		clInputMoney_Quest.Init();
		clInputMoney_War.Init();
		clInputMoney_Father.Init();
		clInputMoney_Recommender.Init();

		clOutputMoney_NPC.Init();
		clOutputMoney_Dead.Init();
		clOutputMoney_Repair.Init();
		clOutputMoney_Stock.Init();
		clOutputMoney_clMarketTax.Init();
		clOutputMoney_clStockBidTax.Init();
		clOutputMoney_clVillageWar.Init();
		clOutputMoney_NPCPayment.Init();
		clOutputMoney_QuestFee.Init();
		clOutputMoney_TradeTax.Init();

		clTotalBudget.Init();
		clReservedBudget.Init();
		clOutputMoney_clNewMarketTax.Init();
	}

	void GetTotalInput( cltMoney *pclMoney )
	{
		pclMoney->Init();
		pclMoney->itMoney += clInputMoney_NPC.itMoney;
		pclMoney->itMoney += clInputMoney_Quest.itMoney;
		pclMoney->itMoney += clInputMoney_War.itMoney;
		pclMoney->itMoney += clInputMoney_Father.itMoney;
		pclMoney->itMoney += clInputMoney_Recommender.itMoney;

	}

	void GetTotalOutput( cltMoney *pclMoney )
	{
		pclMoney->Init();
		pclMoney->itMoney += clOutputMoney_NPC.itMoney;
		pclMoney->itMoney += clOutputMoney_Dead.itMoney;
		pclMoney->itMoney += clOutputMoney_Repair.itMoney;
		pclMoney->itMoney += clOutputMoney_Stock.itMoney;
		pclMoney->itMoney += clOutputMoney_clMarketTax.itMoney;
		pclMoney->itMoney += clOutputMoney_clStockBidTax.itMoney;
		pclMoney->itMoney += clOutputMoney_clVillageWar.itMoney;
		pclMoney->itMoney += clOutputMoney_NPCPayment.itMoney;
		pclMoney->itMoney += clOutputMoney_QuestFee.itMoney;
		pclMoney->itMoney += clOutputMoney_TradeTax.itMoney;
		pclMoney->itMoney += clOutputMoney_clNewMarketTax.itMoney;
	}

};
// 새롭게 읽을 맵정보 루틴.
class cltTreasure_RocData
{
private:
public:
	SI32 m_siX	;
	SI32 m_siY	;
	cltTreasure_RocData()
	{ ZeroMemory(this, sizeof(cltTreasure_RocData)); };

	~cltTreasure_RocData()
	{ ZeroMemory(this, sizeof(cltTreasure_RocData)); };

	SI32 GetRoc_X(){ return m_siX ;}	;
	SI32 GetRoc_Y(){ return m_siY ;}	;

	void Set_RocX( SI16 siX	){ m_siX = siX ;}	;
	void Set_RocY( SI16 siY	){ m_siX = siY ;}	;
	
	void Set_Roc( SI16 siX	,SI16 siY )
	{ 
		m_siX = siX ;
		m_siY = siY ;
	};
};
class cltTreasure_Roc
{
private:
	cltTreasure_RocData m_RocData[MAX_TREASUREEVENT_POSITION_NUM]	;
public:
	cltTreasure_Roc()
	{ ZeroMemory(this, sizeof(cltTreasure_Roc)); };

	~cltTreasure_Roc()
	{ ZeroMemory(this, sizeof(cltTreasure_Roc)); };

	void SetRoc( SI32 siX, SI32 siY, SI32 siSLot)
	{
		m_RocData[siSLot].Set_Roc(siX,siY)	;
	};
	SI32 GetRocX(SI32 siSLot ){ return m_RocData[siSLot].m_siX	;	}
	SI32 GetRocY(SI32 siSLot ){ return m_RocData[siSLot].m_siY ;	}
};
class cltTreasure_MapInfo
{
private:
	SI32				m_siMapIndex	;	
	SI32				m_RocNum		;	// 좌표를 몇개나 가지고 있나.
	SI32				m_MaxUse_RocNum		;	// 좌표를 몇개나 가지고 있나.
public:
	cltTreasure_Roc*	pclTreasure		;	// X Y 좌표

	cltTreasure_MapInfo()
	{ ZeroMemory(this, sizeof(cltTreasure_MapInfo)); };

	~cltTreasure_MapInfo()
	{ ZeroMemory(this, sizeof(cltTreasure_MapInfo)); };

	void MakeRocSlot()
	{ 
		pclTreasure = (cltTreasure_Roc*)malloc( sizeof( cltTreasure_Roc ) )	;
		ZeroMemory( pclTreasure ,sizeof( cltTreasure_Roc ) )	;
	};
	void SetRoc( SI32 siRocX,  SI32 siRocY, SI32 siSlot )
	{ 
		pclTreasure->SetRoc( siRocX , siRocY,siSlot)	;
	}
	void SetMapIndex( SI32 siMapIndex){	m_siMapIndex = siMapIndex	; }	;
	SI32 GetMapIndex(){ return m_siMapIndex ; }	;

	void Set_MaxUse_RocNum(	SI32 siMaxRocNum){	m_MaxUse_RocNum = siMaxRocNum	; }	;
	SI32 Get_MaxUse_RocNum(){	return m_MaxUse_RocNum ; }	;

	void SetRocNum(	SI32 siRocNum){	m_RocNum = siRocNum	; }	;
	SI32 GetRocNum(){	return m_RocNum ; }	;
	
	SI32 GetRoc_X( SI32 siSLot ){ return pclTreasure->GetRocX( siSLot ) ; }	;
	SI32 GetRoc_Y( SI32 siSLot ){ return pclTreasure->GetRocY( siSLot ) ; }	;
};

class clTreasureEvent_Npc_KillCounnt
{
private:
	SI32 m_siMapIndex ;			// 생성 맵인덱스
	SI32 m_siKillCount ;	// 만들어지는 생성 최대 숫자
public:
	clTreasureEvent_Npc_KillCounnt()
	{ ZeroMemory(this, sizeof(clTreasureEvent_Npc_KillCounnt)); };
	~clTreasureEvent_Npc_KillCounnt(){};
	SI32 GetMapIndex(){		return m_siMapIndex	;}	;
	void SetMapIndex( SI32 siMapindex ){m_siMapIndex = siMapindex ;};

	SI32 Get_KillCount(){	return m_siKillCount	;}	;
	void Set_KillCount( SI32 siKillCount ){ 
		m_siKillCount = siKillCount ; 
	};
	void AddKillCount()	{	m_siKillCount = m_siKillCount +1	;	}
	void DellKillCount( SI32 siDelKill_Point){	m_siKillCount = m_siKillCount - siDelKill_Point	;	}
};
class clTradeItemInfo
{
public:
	_SYSTEMTIME m_sTime;
	SI64		m_siMoney;
	SI32		m_siCnt;

	clTradeItemInfo()
	{
		ZeroMemory(this, sizeof(clTradeItemInfo));
	}
	void Clear(void)
	{
		ZeroMemory(this, sizeof(clTradeItemInfo));
	}
};




#define KIND_TREASURE_DAEGUM1		377		//썩어가는 스워드맨
#define KIND_TREASURE_CATAR1		378		//썩어가는 스피어맨
#define KIND_TREASURE_RIFLE1		379		//썩어가는 거너
#define KIND_TREASURE_DAEGUM2		380		//저주받은 스워드맨
#define KIND_TREASURE_CATAR2		381		//저주받은 스피어맨
#define KIND_TREASURE_RIFLE2		382		//저주받은 거너
#define KIND_TREASURE_REDSHABEL		383		//저주받은 레드샤벨
#define KIND_TREASURE_BLUESHABEL	384		//저주받은 블루샤벨
#define KIND_BLESSINGJEAN			385		//정화된 무사의 영혼
#define KIND_BLESSINGMERIEL			386		//정화된 무녀의 영혼
#define KIND_BLESSINGCLARA			387		//정화된 아이의 영혼
#define KIND_BLESSINGSPARKY			388		//정화된 동물의 영혼
#define KIND_CURSEHIRO				389		//광기의 영웅
#define KIND_BLESSINGHIRO			390		//안식을 찾은 영웅의 영혼

#define  FAMILY_ADD_CHILDREN_EXP	0.003f	// 가족관계를 맺은 사람들중 자식들에게 나눠받을 부모 경험치의 비율
#define  MAX_FAMILY_ADD_EXP			10		// 가족관계를 맺은 사람이 접속했을때 받을수 있는 경험치 증가의 최대


class cltServer : public cltCommonLogic {

// [진성] 변수, 함수 모음
public:
	// [추가 : 황진성 2008. 2. 20 => 소환수 능력치 변경적용하는 부분을 함수로 뺌.]
	void Use_Summon_Bonus(cltGameMsgRequest_SummonUseBonusPoint* pclinfo, SI32 id);
	void New_Use_Summon_Bonus(cltGameMsgRequest_SummonUseBonus* pclinfo, SI32 id);

	void	DayLogFileWrite(void);

	//[추가 : 황진성 2007. 11. 19 명나라 상인 아이템 거래 누적 정보.]
#ifdef _SAFE_MEMORY
	NSafeTArray<clTradeItemInfo, 6>		clItemInfo;
#else
	clTradeItemInfo clItemInfo[6];
#endif

	//[추가 : 황진성 2007. 11. 26 시스템에서 유저가 시장에 올린 아이템 구매관련 누적 정보.]
	clTradeItemInfo clSystemBuyItem;

	void StartPersonalSpecialShop(cltMsg* pclMsg, SI32 id);

	bool	SelectBillURL( TCHAR * AccountID, TCHAR * selectURL );
public:
	

	cltGameProtocol*	pclGameProtocol;
	FrontAuthProcess*	pclFrontAuthProcess;
	AgentProcess*		pclAgentProcess;
	BillProcess*		pclBillProcess;
	GameDBProcess*		pclGameDBProcess;
	GameDBProcess2*		pclGameDBProcess2;
	LogDBProcess*		pclLogDBProcess;
	
	NTradeServerProcess*pclNTradeServerProcess;

	GMProtocol*			pclGMProtocol;

	

	cltIDNumTable*		pclIDNumTable;


	bool				bServerNetoworkStartSwitch;	// 서버의 네트워크가 완전히 준비되었는지 확인한다. 
	bool				bGameTimeRequestSwitch;		// 게임 시간을 얻어 올 것을 요청했음을 확인한다. 
	bool				bGetTimeSwitch;				// DB로부터 게임 시간을 얻어왔음을 확인한다. 
	// 게임 시간을 얻어오기 전까지는 서버의 행동을 하지 않는다. 

	bool				bMapTypeInfoRequestSwitch;	// 맵 타입을 요청했음을 확인한다. 
	bool				bGetMapTypeInfoSwitch;		// 맵 타입정보를 받았음을 확인한다. 

	bool				bForcePersonalShopSwitch;	// 개인성점을 제한한다.
	bool				bForceVillageWarSwitch;		// 공성을 강제로 시작하게 한다.
	bool				bForceTimeSwitch;			// 시간을 빨리돌린다.
	bool				bAutoWarSwitch;				// 자동 전쟁 여부. 

	bool				bAutoStockDistriubtionSwitch;	// 자동 공모 여부

	DWORD				dwVillageWarTimeMin;
	DWORD				dwVillageWarStartHour,	dwVillageWarStartMin;		// 공성시간 설정 
	DWORD				dwVillageWarFinishHour,	dwVillageWarFinishMin;
	DWORD				dwSpecialNews;
	DWORD				dwGeneralMeetingNewsMin;
	DWORD				dwVillageWarNewsMin;
	DWORD				dwBlackArmyNewsHour;


	bool				bProcessDB_WealthRank_NEED;
	bool				bProcessDB_TotalMoney_NEED;
	bool				bProcessDB_TotalChar_NEED;

	bool				bProcessDB_GetMinister_NEED;

	bool				bProcess_InitCalcCharNum;

	bool				bCalcInvestment; // 김선달(투자상담가) 계산하기

	bool				bProcess_InitGameEvent ;

	bool				bProcessDB_TotalVillageScore_NEED;
	// 자동 배당 관련
	bool				bProcessDB_AutoPayDividend_NEED;
	
	// 당일 공성정보가 업데이트 되었는지 스위치
	bool				bProcess_SetNowVillageWarInfo;
	cltDate				clLastSettedNowVillageWarIinfo;

	// 마지막으로 유저들의 공성전 정보가 초기화 된 날짜를 기억. 날짜가 변경되면 유저들의 공성전 전적을 초기화
	SI32				siLastVillageWarKillNumClearDay;

	cltServerStartInfo	clServerStartInfo;

	bool				bHorseCurrentPriceSet;

	UI08				bGMBonusTimeSwitch;		// GM명령으로 보너스 타임 선언했을때의 스위치
	bool				bIsGMBonusTime;			// GM보너스 타임중인가?
	bool				bIsGMBonusTime2;		// GM보너스 타임중인가?(제조)
	
	SI16				siGMBonusTimePercent;	// GM보너스 타임이 설정해주는 보너스 퍼센트
	SI16				siGMBonusTime2Percent;	// GM보너스 타임이 설정해주는 보너스 퍼센트

	bool				bDoNotAutoBonus;		// 자동보너스 시간 금지 스위치

	SI16				siHorseTradeCount;
	GMONEY				siHorseCurrentPrice;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_LOWLEVEL_LIST_NUMBER>		siLowLevelCharUnique;
#else
	SI32				siLowLevelCharUnique[MAX_LOWLEVEL_LIST_NUMBER];
#endif
	SI32				siLowLevelCount;


#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, 10>		siTopLevelVillageUnique;
#else
	SI32				siTopLevelVillageUnique[10] ;
#endif

	//KHY - 1113 - 크리스마스 이벤트 - 맵에 설치될 이벤트용 npc ID를 저장한다.
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, EVENT_NPC_COUNT>		siEventNPCId;
#else
	SI32				siEventNPCId[EVENT_NPC_COUNT];
#endif	

	bool				bAlldayBonus;		//[진성] 자동보너스 시간 하루종일 적용되면 그 하루 중 군주 보너스 시간은 생략하기 위해 추가.

	SI16				siFourLeafUnique;

	// 서버 다운 검증을 위해 코드 위치 변경
	//======================================================
	// 메시지맵
	//======================================================
	// 서버메시지 처리 함수포인터
	class stServerMsgPtr
	{
	public:
		void (cltServer::*ServerMsgFunc)(cltMsg* pclMsg, SI32 id);
	};

	stServerMsgPtr	*m_pServerMsgPtr;
	bool			*m_pbUseMsg;
	void	ServerMsgNullFunc(cltMsg* pclMsg, SI32 id);
	void	RegisterServerMsgMap();

	// DB메시지 처리 함수포인터
	class stDBMsgPtr
	{
	public:
		void (cltServer::*DBMsgFunc)(sPacketHeader* pPacket);
	};

	stDBMsgPtr *m_pDBMsgPtr;
	void	DBMsgNullFunc(sPacketHeader* pPacket);
	void	RegisterDBMsgMap();
	//======================================================

public:
	SI32			siServerID;						// Server ID

	ItemList*		pclItemList;					// 바닥에 떨어진 아이템의 보관 관리자. 

	// 직급 관리자.
	cltRankManager* pclRankManager;

	// 시간 관리자. 
	cltTime*		pclTime;

	// 날씨 관리자. 
	CWeather*		pclWeather;

	cltStockOfferingManager*	pclStockOfferingManager;

	//------------------------------
	// 공지 관리자. 
	//------------------------------
	cltNoticeManager*	pclNoticeManager;

	//------------------------------
	// CHAR NAME으로 검색을 위한 HASH TABLE
	//------------------------------
	HashStringInt*		pclHashTable;

	//------------------------------
	// PERSON ID으로 검색을 위한 HASH TABLE
	//------------------------------
	HashIntInt*			pclPersonHashTable;

	//-----------------------------------
	// 육조판서  
	//-----------------------------------
	CMinisterMgr*		pclMinisterMgr;

	cltIPToCountryMgr*	pclIPToCountryMgr;
	cltAccessDenyMgr*	pclAccessDenyMgr;

	//-------------------------------
	// 채팅 건수
	//-------------------------------
	SI32 siChatCount_Private;
	SI32 siChatCount_General;

	//--------------------------------
	// 길드전 예약 관리자
	//--------------------------------
	cltGuildWarReserveManager*	pclGuildWarReserveManager;
	NSafeTArray<cltGuildWarManager*, GUILDWAR_FIELDNUM>		pclGuildWarManager;
	//--------------------------------
	// 길드소유 던전 관리자
	//--------------------------------
	cltGuildDungeonMgr*			pclGuildDungeonMgr;

	//--------------------------------
	// 책상자 드롭 관련
	//--------------------------------
	cltBookBox*			pclBookBox;

	//--------------------------------
	// 갓핸드 아이템
	//--------------------------------
	CItemGodHandManager*	pclGodHandManager;

	//-------------------------------
	// 동접자 수. 
	//-------------------------------
	SI32 siTodayMaxUserNum;				// 오늘의 최고 동접. 
	SI32 siCurUserNum;					// 현재 동접. 
	SI32 si1To19UserNum;
	SI32 si20To39UserNum;
	SI32 si40To59UserNum;
	SI32 si60To79UserNum;
	SI32 si80To99UserNum;
	SI32 si100To119UserNum;
	SI32 si120To150UserNum;

	SI32 siTotalNewCharNum;				// 전체 서버의 일주일간 캐릭터 생성 숫자 
	SI32 siTotalDelCharNum;				// 전체 서버의 일주일간 캐릭터 삭제 숫자

	SI32 siServerNewCharNum;			// 현재 서버의 일주일간 캐릭터 생성 숫자
	SI32 siServerDelCharNum;			// 현재 서버의 일주일간 캐릭터 삭제 숫자

	SI32 siNewCharNum;					// 새로운 캐릭터 생성 숫자  
	SI32 siDelCharNum;					// 캐릭터 삭제 숫자 
	SI32 siPCRoomCharNum;				// pc방 캐릭터 접속자 숫자

	//SI32 siNetmarbleCharNum;			// Netmarble 캐릭터 접속자 숫자

	SI32 siHangameCharNum;				// 한게임 캐릭터 접속자 숫자.
	SI32 siGamaniaCharNum;				// Gamania 캐릭터 접속자 숫자
	SI32 siGamePotCharNum;				// GamePot 캐릭터 접속자 숫자
	SI32 siNDECharNum;					// NDE캐릭터 접속자 숫자
	SI32 siNEXONCharNum;				// NEXON캐릭터 접속자 숫자

	// [진성] USA 이관.
	SI32 siGlobalNum;					// 글로벌 접속자 수.
	SI32 siNHNNum;						// NHN 접속자 수.

	//-------------------------------
	// 게임 모드 누적수
	//-------------------------------
	SI32 siWndInterfaceNum;				// 현재 서버의 재시작 후 윈도우창 + 윈도우 인터페이스 사용자 누적수
	SI32 siWnd_NewInterfaceNum;			// 현재 서버의 재시작 후 윈도우창 + 새인터페이스 사용자 누적수
	SI32 siFullWnd_NewInterfaceNum;		// 현재 서버의 재시작 후 전체화면 + 새인터페이스 사용자 누적수


	//-------------------------------
	// 프리미엄 시간대 
	//-------------------------------
	_SYSTEMTIME sTime;
	_SYSTEMTIME	sLastNewsTime;			// 마지막으로 뉴스를 얻어온 시간. 	

	//---------------------------------
	// DB 매니저와의 마지막 교신 시각. 
	//---------------------------------
	DWORD dwLastDBClock;

	//--------------------------------
	// 여각 주식 주문 요청. 
	//--------------------------------
	DWORD dwLastStockOrderClock;
	SI32 siLastStockOrderIndex;

	//--------------------------------
	// DB지연시간. 
	//--------------------------------
	DWORD dwDBLatency;

	//--------------------------------
	// 통화량 
	//--------------------------------
	cltCurrencyInfo clCurrencyInfo;

	//--------------------------------
	// 게임 서버 통계. 
	//--------------------------------
	cltStatisticsManager * pclStatisticsManager;

	cltAddMoneyStatistics*		pclAddMoneyStatistics;
	cltDelMoneyStatistics*		pclDelMoneyStatistics;

	//--------------------------------
	// Server Refresh Info
	//--------------------------------
	sGameServerRefreshInfo				*pRefreshInfo;
	CRITICAL_SECTION					cs_refresh;

	//-----------------------------------
	// 파라메타로 사용될 캐릭터 리스트들. 
	//-----------------------------------
	SI32	siParaCharListIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_CHAR_NUMBER>		siParaCharList;
#else
	SI16	siParaCharList[MAX_CHAR_NUMBER];
#endif

	//-----------------------------------
	// 하위 마을 개수
	//-----------------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_VILLAGE_NUMBER>	siSlaveVillageNum;
#else
	SI32	siSlaveVillageNum[ MAX_VILLAGE_NUMBER ];
#endif

	//-----------------------------------
	// 특별한 날 여부. 
	//-----------------------------------
	bool bDoubleBonusDaySwitch;

	bool bSpecailDay1Switch;			// 현충일. 
	bool bSpecailDay2Switch;			// 광복절. 
	bool bSpecailDay3Switch;			// 국군의날 . 
	bool bSpecailDay4Switch;			// 군주오픈기념일
	bool bSpecailDay5Switch;			// 크리스마스  
	bool bSpecailDay6Switch;			// 3.1절
	bool bSpecailDay7switch;			// 충무공 탄신일
	bool bSpecailDay8switch;			// 어린이날
	bool bSpecailDay9switch;			// 바다의 날

	bool bInstanceGate;					// 인스턴스 던전 입구 들어가냐?

	bool bRouletteGame;

	bool bAutoPayDividend;				// 자동 배당 막기(운영자 명령어로)

	bool bTradeServer;
	bool bForceStopTradeProcess;

	bool bForceStopVillageWarProcess;	//

	SI16 siTradeServerStartTime; // 무역 가능 시간 시작 시간
	SI16 siTradeServerEndTime; // 무역 가능 시간 끝나는 시간

	SI16 siLimitMaxUserNumber;

	SI32 siServiceType;

	// pvp-
	SI32					siLoseItem;
	SI32					siRegenerateLifeValue;
	SI32					siBaenNyeonSamRate;
	SI32					siHandDamageRate;
	SI32					siSwordDamageRate;
	SI32					siSpearDamageRate;
	SI32					siAxeDamageRate;
	SI32					siBowDamageRate;
	SI32					siGunDamageRate;
	SI32					siStaffDamageRate;
	SI32					siPKMapExpRate;
	//길드전 가능여부
	bool					bGuildWar;	
	// 파산 진행 여부
	bool					bBankruptcy;
	SI32					siStartBankruptDateVary;

	NDate					clAttendEventStart;		//출석체크 이벤트 시작일
	NDate					clAttendEventEnd;		//출석체크 이벤트 완료일
	SI32					siAttendCheckMinute;	//출석체크 시간..( 접속후 x분)

	SI32					siItemmallRPRate;

	bool					bNoChargeGetItemFromNPC;	// 클라라를 통한 물품 회수 수수료 없음

	CKingGuriEvent*			pclKingGuriEventManager;
	CGoonzuKingGuriEvent*	pclGoonzuKingGuriEventManager;
	CEmpireManager*			pclEmpireManager;

	// 신고된 매크로 유저 관리
	cltMacroReportedMgr*	m_pclMacroReportedMgr;
	cltGZFtpMgr*			m_pclGZFtpMgr;

	// 타이머
	NUpdateTimer			m_clUpdateTimer_Minute;		// 1분 타이머

	CSantaRaccoon*			pclSantaRaccoon;			//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트.]

	//KHY - 0115 발렌타인데이 이벤트.
	NDate					clValentineEventStart;		//발렌타인데이 시작일
	NDate					clValentineEventEnd;			//발렌타인데이 완료일
	SI32						siValentineFarm;				//발렌타인데이  - 농장 에서의 아이템 획득 확률.
	SI32						siValentineMine;				//발렌타인데이  - 광산 에서의 아이템 획득 확률.
	SI32						siValentineFish;				//발렌타인데이  - 낚시 에서의 아이템 획득 확률.
	SI32						siValentineAttack;			//발렌타인데이  - 사냥 에서의 아이템 획득 확률.
	SI32						siValentineItem;				//발렌타인데이  - 제조 에서의 아이템 획득 확률.

	//KHY - 0218 화이트데이 이벤트.
	NDate				clWhiteEventStart;		//화이트데이 시작일
	NDate				clWhiteEventEnd;			//화이트데이 완료일
	SI32						siWhiteFarm;				//화이트데이 - 농장 에서의 아이템 획득 확률.
	SI32						siWhiteMine;				//화이트데이 - 광산 에서의 아이템 획득 확률.
	SI32						siWhiteFish;				//화이트데이  - 낚시 에서의 아이템 획득 확률.
	SI32						siWhiteAttack;			//화이트데이 - 사냥 에서의 아이템 획득 확률.
	SI32						siWhiteItem;				//화이트데이 - 제조 에서의 아이템 획득 확률.

	//KHY - 0530 명나라 상인 구매제한.
	GMONEY					gForeignMerchantBuyLimit;	// 명나라 상인 1일 구매 제한 금액
	GMONEY					gForeignMerchantNowMoney;	// 명나라 상인 1일 구매 금액의 총합.
	SI16					siYesterday;					// 날짜 비교용 변수.

	SI32					siNDEScrollRate;			// 서버 이벤트 NDE 부적 드랍 확률

	float					fForeignMerchantPercentUP	;	// 명나라 상인 구매제한금액 증가율
	float					fForeignMerchantPercentDOWN	;	// 명나라 상인 구매제한금액 감소율

	// KHY - 0901 -  과금 아이템 자동구매 설정.
	SI32					siMarketMoneyItemAutoBuyDelayTime;
	GMONEY					siMarketMoneyItemMaxpriceNow;

	//-------------------------------------
	// 보상아이템 지급관련
	//-------------------------------------
	cltRewardMgr*			pclRewardMgr;

	//-------------------------------------
	// 박스 아이템 관련
	//-------------------------------------
	cltNewSelectBoxMgr*		pclNewSelectBoxMgr;
	//cltElixirManager*		m_pclElixirManager;

	bool				bMakeTreasureEventNPC	;// 보물찾기 이벤트 낡은 쇠뭉치 들을 세웠는지 아닌지 서버에서 딱 한번만 세운다.
	bool				bReadTreasureEvent_Resource	;// 보물찾기 이벤트 관련 리소스를 읽는다.
	// npc가 사라질때마다 기록하는  정보
	clTreasureEvent_Npc_KillCounnt		pclTreasureEvent_Npc_KillCounnt[ MAX_TREASUREEVENT_USEMAP_NUM ];	// 보물찾기 맵별로 NPC가 사라진 횟수
	cltTreasure_MapInfo					pclTreasureMapInfo[MAX_TREASUREEVENT_USEMAP_NUM]	;
	// 사용할 X,Y좌표 모음 맵별로 생성할때마다 지우고 쓸거다. 계속 가지고 있는게 아니고 가지고있는 슬롯도 인스턴스로 사용함
	SI32								pclTreasureMapUseRoc[MAX_TREASUREEVENT_POSITION_NUM]	;			

	bool	bGachaLock	;					// 가차 사용 불가능 , 가능을 셋팅 초기값은 true
	SI32	siGMChangeGachaRareUnique	;	// GM이 설정한 가차유니크 GM이 설정 하면 이걸로만 설정 된다 .시간이 지나도 변경이안됨 . 
	bool	bGachaRareitemStartSet	;		// 가챠 레어 아이템 서버 처음 열때 한번 셋한다 .

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, DBCONNECTION_NUMBER>	siCompleteDBInit;
#else
	SI16 siCompleteDBInit[ DBCONNECTION_NUMBER ];
#endif
	SI16 siCurrentDBInit;

	BOOL bSetVillageLevel;
	BOOL bSetTopLevelVillage;


	// 친구 - 친구 요청한 사람이 게임을 종료한 뒤에 수락버튼이 눌러진 경우에 사용할 계정 ID가 들어갈 버퍼
	TCHAR m_szFriendAccountID[MAX_PLAYER_NAME];

#ifdef _SAFE_MEMORY
	NSafeTArray<SystemBuyItem, MAX_SYSTEMBUYTEM_NUM>	stSystemBuyItem;
#else
	SystemBuyItem		stSystemBuyItem[MAX_SYSTEMBUYTEM_NUM]; // KHY - 0901 -  과금 아이템 자동구매 설정.
#endif

	SI64	siTotalMoney;				// 총통화량 - 하루에 한번씩 갱신된다.

	//-------------------------------------
	// 배틀로얄
	//-------------------------------------
	CBattleRoyalMgr*				m_pclBattleRoyalMgr;
	
	// 앞으로 기부 이벤트 기본정보(기부를당하는 NPC, 기부 아이템, 개수 )는 cltContributionEvent_Common 여기서 관리.
	cltContributionEvent_Common		m_clContributionEvent_Common;
	
	// 자이언트 라쿤 부활 이벤트(기부 이벤트). (cltContributionEvent_Common 상속)
	cltGiantRaccoonResurrectEvent*	m_pclGiantRaccoonResurrectEvent;

	CPartyQuestMgr_Server*			m_pPartyQuestMgr;

protected:
	//----------------------------------
	// 흥부박 퀘스트 완료자 버퍼. 
	//----------------------------------
	SI32 siSpecialQuestBonusBufferIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SPECIAL_QUEST_BONUS_BUFFER_SIZE>	siSpecialQuestBonusBuffer;
#else
	SI32 siSpecialQuestBonusBuffer[MAX_SPECIAL_QUEST_BONUS_BUFFER_SIZE];
#endif

	//----------------------------------
	// 캐시 지급 관련 버퍼.
	//----------------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray<cltPushCash, MAX_PUSH_CASH_LIST_NUMBER>	PushCashList;
#else
	cltPushCash PushCashList[MAX_PUSH_CASH_LIST_NUMBER];
#endif
	SI32 PushCashListIndex;

	//-------------------------------------
	// 뉴스 출력 버퍼
	//-------------------------------------
	SI32 siSpecialNewsBufferIndex;
	SI32 siSpecialNewsSendIndex;
	TCHAR szNewsTitleBuffer[MAX_SPECIAL_NEWS_BUFFER_SIZE][MAX_NEWS_TITLE_LENGTH];
	TCHAR szNewsNameBuffer[MAX_SPECIAL_NEWS_BUFFER_SIZE][MAX_NEWSPAPER_NAME_LENGTH];
	TCHAR szNewsURLBuffer[MAX_SPECIAL_NEWS_BUFFER_SIZE][MAX_PATH];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SPECIAL_NEWS_BUFFER_SIZE>	siSpecialNewsKindBuffer;
	NSafeTArray<SI32, MAX_SPECIAL_NEWS_BUFFER_SIZE>	siSpecialSendKindBuffer;
#else
	SI32 siSpecialNewsKindBuffer[MAX_SPECIAL_NEWS_BUFFER_SIZE];
	SI32 siSpecialSendKindBuffer[MAX_SPECIAL_NEWS_BUFFER_SIZE];
#endif

	//-------------------------------------
	// 프로세스 체크 관련
	//-------------------------------------
	cltCheckProcessManager*		pclCheckProcessManager;

	clPromoterManager*			pclPromoterManager;

	CGawiBawiBoMgr_Server*		m_pGawiBawiBoMgr;

	cltSummonTransformManager*	m_pclSummonTransformManager;		//[진성] 소환수 변신 메니져.		

public:
	cltSummonTransformManager*			GetSummonTransformManager( void ) { return m_pclSummonTransformManager; }

	void								blockRefresh() { EnterCriticalSection( &cs_refresh ); };
	void								unblockRefresh() { LeaveCriticalSection( &cs_refresh ); };

	cltServer(TCHAR* appname,HWND hwnd , HINSTANCE hinst, SI32 gamemode, SI32 servicearea, SI32 runmode);
	~cltServer();

	//------------------------------
	// 출력 
	//------------------------------
	void  DrawServerInfo(HDC hdc);  // 그리기 관련 부분 

	UI32  m_lastFrame;
	UI32  m_currentFrame;

	BOOL	bZeroStatistics;
	BOOL	bListenOK;
	BOOL	bWriteStatistics;

	UI16	m_uiCompressMinSize;
	CRITICAL_SECTION	m_cs_SendNetMsg;
	BYTE				m_bufComp[ sizeof(cltMsg) ];
	BYTE				m_encBuf[ sizeof(cltMsg) ];

	// 건물 정보 갱신 
	void UpdateStrInfoToClient( SI32 id, SI32 strType );

	//----------------------------------------
	// 초기화. 
	//----------------------------------------
	void Init(){};
	void Create();

	BOOL GMOrder( TCHAR *pOrder );
	void RestartTradeServer();

	//----------------------------------------
	// 길드전쟁
	//----------------------------------------
	SI32 GetGuildWarBattleIndex( SI32 siGuildUnitIndex );

	//--------------------------------------------
	// 서버 
	// Server.cpp
	//--------------------------------------------
	// APEX-
	void SendNetMsg( int id, sPacketHeader* pPacket );
	void KillUser( int id );
	// -APEX

	// -HanAuth_Lib
#ifdef USE_HANRTA
//#ifdef _SERVER
	NUpdateTimer					m_kHanRTA_Timer;
#endif // USE_HANRTA

	// 중국RTA - USE_CNRTA
#ifdef USE_CNRTA
	//#ifdef _SERVER
	NUpdateTimer					m_kCNRTA_Timer;
#endif // USE_CNRTA

#ifdef USE_HANXNOTI
	XNoti::Connector::THandle		m_pkHanAuth_XNotiConnector;
	TCHAR							m_szMRSServerID[ 50 ];

	static bool __stdcall HanXNoti_DoMsg_GMTMessageReq_K_GOONZU(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_GMTKickReq_K_GOONZU(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_GMTChatBlockReq_K_GOONZU(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_GMTCmdReq_K_GOONZU(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_GameCharacterListReq(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_AddInventoryItemReq(XStream::Handle stream, void* pAdditionalData, void* pContext);
	static bool __stdcall HanXNoti_DoMsg_DeleteInventoryReq(XStream::Handle stream, void* pAdditionalData, void* pContext);

	CRITICAL_SECTION				m_HanXNoti_CS;
	stHanXNoti_Waiting				m_kHanXNoti_Waitings[HANXNOTI_MAX_WAITING];
	XDR::IMessage*					m_pkHanXNoti_AnsMessages[HANXNOTI_MAX_WAITING];
	SI32 HanXNoti_GetEmptyWaiting(SI32 siIdx);
	void HanXNoti_ClearWaiting(SI32 siArrayIndex);

//#endif
#endif // USE_HANXNOTI

	// -HanAuth_Lib

	// -HanAuth_Lib
	void							HanAuth_Lib_HanReportSendPosLog_Server(TCHAR* pszID, TCHAR* value);
	// -HanAuth_Lib

	NUpdateTimer					m_kUpdateTimer_GameGuard;
	
	// [영훈] Auto Unblock 관련 타이머(서버 기동후 5분이 지나면 작동, 1번만 작동됨) - 2008.02.21
	NUpdateTimer					m_kUpdateTimer_AutoUnblock;

	// [성웅] 1분마다 호출해서 리그전을 발생한다.
	NUpdateTimer					m_kUpdateTimer_PVP_League	;

	// [성웅] 길드 사냥터 길드전을 시작 한다.
	NUpdateTimer					m_kUpdateTimer_GuildHuntMapWar	;

	// [성웅] 10분마다 호출해서 사라진 npc 숫자만큼 생성 한다
	NUpdateTimer					m_kUpdateTimer_TreasureEvent_Respon_Npc	;// 보물찾기 이벤트

	// [ 성웅  ]	1분마다 호출 한다. 날짜를 체크해서 가챠 레어 아이템 유니크를 설정 한다.
	NUpdateTimer					m_kUpdateTimer_GachaRareItmeUnique_Timer;
	
	// [성웅] 라쿤 피하기 이벤트 
	NUpdateTimer					m_kUpdateTimer_RacoonDodgeEvent; // 라쿤 피하기 이벤트 //Switch_10MonthEvent_RacoonDodge

	// 프론트에 전달할 서버 정보 
	GAMESERVER_GAMESERVERINFO	m_gameserverinfo;
	BOOL						m_bGameserverinfo;

	void LoadAutoUnblockFromFile(OUT bool *bAutoUnblockMode, OUT SI32 *siGameVersion);
	void SaveAutoUnblockToFile(IN bool bAutoUnblockMode, IN SI32 siGameVersion );

	// [영훈] 초보자 마을 - 앙카란 : 공작을 마을의 영주로 만들어주는 함수
	void BegnnerVillage_SetVillageChief( SI32 siPersonID );
	
	// [진성] 발렌타인 데이 NPC(카렌디피) 관리 함수.
	void ValentineDayNpc( void );


	inline CGawiBawiBoMgr_Server* GetGawiBawiBoMgr()
	{
		if(m_pGawiBawiBoMgr != NULL)
			return m_pGawiBawiBoMgr;
		else
			return NULL;
	}

	//각각의 메시지 처리 함수들. 

	// APEX-
	void DoMsg_GAMEMSG_REQUEST_APEXDATA(cltMsg* pclMsg, SI32 id);
	// -APEX

	void DoMsg_GAMEMSG_REQUEST_USERORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HIT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETBULLET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_RIGHTMOUSE_USERLIST_NAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_VILLAGESTRUCT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ALLCHARINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_USEBONUS(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_PERSONNAME_FROMPERSONID(cltMsg* pclMsg,SI32 id);

	// Server-Person.cpp
	// 퀘스트 수행에 대한 요청. 
	void DoMsg_GAMEMSG_REQUEST_SETQUESTGOING(cltMsg* pclMsg, SI32 id);
	// 퀘스트 취소에 대한 요청. 
	void DoMsg_GAMEMSG_REQUEST_CANCELQUEST(cltMsg* pclMsg, SI32 id);
	// 퀘스트 상태 통보. 
	void DoMsg_GAMEMSG_REQUEST_QUESTSTATUS(cltMsg* pclMsg, SI32 id);

	// 이벤트 결과를 요청한다.
	void DoMsg_GAMEMSG_REQUEST_PRIZE_GAMEEVENT(cltMsg* pclMsg,SI32 id);
	// 학교 이벤트
	void DoMsg_GAMEMSG_REQUEST_SETSCHOOLEVENT(cltMsg* pclMsg , SI32 id) ;
	// 조선의 반격 이벤트
	//void DoMsg_GAMEMSG_REQUEST_COUNTATTACK_INFO(cltMsg* pclMsg , SI32 id);
	// 현재 천군 이벤트
	void DoMsg_GAMEMSG_REQUEST_EVENTNPC(cltMsg* pclMsg , SI32 id);
	// 캐릭터 인사말
	void DoMsg_GAMEMSG_REQUEST_SETGREETING(cltMsg* pclMsg , SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETGREETING(cltMsg* pclMsg , SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SITDOWN(cltMsg* pclMsg , SI32 id);

	// 홈페이지 셋팅.
	void DoMsg_GAMEMSG_REQUEST_GET_HOMEPAGE_URL(cltMsg* pclMsg , SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SET_HOMEPAGE_URL(cltMsg* pclMsg , SI32 id);


	// 부활할 것인지 문의에 대한 응답
	void DoMsg_GAMEMSG_REQUEST_REVIVE(cltMsg* pclMsg , SI32 id);
	void DoMsg_GAMEMSG_REQUEST_RESURRECTION(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GET_WEAPONSKILLCOST(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SET_WEAPONSKILL(cltMsg* pclMsg,SI32 id);


	void DoMsg_GAMEMSG_REQUEST_MYPUCHIKOMIURL(cltMsg* pclMsg, SI32 id);	// 자신의 푸치코미 열기
	
	void DoMsg_GAMEMSG_REQUEST_USERPUCHIKOMIURL( cltMsg* pclMsg, SI32 id);

	// 보물찾기 이벤트 - 오카라 10개를 쓰고 버프받을래?
	void DoMsg_GAMEMSG_REQUEST_TREASUREEVENT_USEOKARA(cltMsg* pclMsg , SI32 id);

	//-------------------------------------------------
	// 귓속말
	//-------------------------------------------------
	// Server-System.cpp
	// 귓속말 시작.
	void DoMsg_GAMEMSG_REQUEST_WHISPERCHAT_CREATE(cltMsg* pclMsg, SI32 id);
	// 귓속말 초대.
	void DoMsg_GAMEMSG_REQUEST_WHISPERCHAT_INVITE(cltMsg* pclMsg, SI32 id);
	// 현재 귓속말 대상들의 리스트 가져옮
	void DoMsg_GAMEMSG_REQUEST_WHISPERCHAT_USERLIST(cltMsg* pclMsg, SI32 id);
	// 귓속말 나감.
	void DoMsg_GAMEMSG_REQUEST_WHISPERCHAT_OUT(cltMsg* pclMsg, SI32 id);


	// Person의 기술 점수를 복구해준다
	void DoMsg_GAMEMSG_REQUEST_DECREASESKILLBONUS(cltMsg* pclMsg, SI32 id);
	// 말 풀어주기. 
	void DoMsg_GAMEMSG_REQUEST_HORSEFREE(cltMsg* pclMsg, SI32 id);
	//KHY - 1001 - 기승동물 5마리 보유 수정.// 말바꾼다.
	void DoMsg_GAMEMSG_REQUEST_CHANGEHORSE(cltMsg* pclMsg, SI32 id);
	// 유저 아이디 요청( 미니 홈페이지를 띄위기 위해서.)
	void DoMsg_GAMEMSG_REQUEST_USERID(cltMsg* pclMsg, SI32 id);
	// 게임 플레이 시간 요청. 
	void DoMsg_GAMEMSG_REQUEST_PLAYTIME(cltMsg* pclMsg, SI32 id);
	// 캐릭터의 추가정보를 요청 
	void DoMsg_GAMEMSG_REQUEST_ADDINFO(cltMsg* pclMsg, SI32 id);
	// 캐릭터의 현재 마법을 바꾸었다.
	void DoMsg_GAMEMSG_REQUEST_CHANGEMAGIC(cltMsg* pclMsg, SI32 id);


	// NPC 위치 요청. 
	void DoMsg_GAMEMSG_REQUEST_NPCPOS(cltMsg* pclMsg, SI32 id);	
	// 클라이언트의 하트비트 요청. 
	void DoMsg_GAMEMSG_REQUEST_HEARTBEAT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CHAT(cltMsg* pclMsg, SI32 id);
	// 귓말 전송. (CharUnique)
	void DoMsg_GAMEMSG_REQUEST_SECRETCHAT(cltMsg* pclMsg, SI32 id);
	// 귓말 채널 작성 요청(PersonID)
	void DoMsg_GAMEMSG_REQUEST_MAKESECRETCHATCHANNEL(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_ACCEPTSECRETCHATCHANNEL(cltMsg* pclMsg, SI32 id);

	// 머릿말 정보를 요청한다. 
	void DoMsg_GAMEMSG_REQUEST_HEADCHAT(cltMsg* pclMsg, SI32 id);
	// 질문 리스트 요청 
	void DoMsg_GAMEMSG_REQUEST_ASKLIST(cltMsg* pclMsg, SI32 id);
	// 질문 삭제 요청 
	void DoMsg_GAMEMSG_REQUEST_DELASK(cltMsg* pclMsg, SI32 id);
	// 장인 명단 요청. 
	void DoMsg_GAMEMSG_REQUEST_SKILLMASTERLIST(cltMsg* pclMsg, SI32 id);
	// 장인 등록/해지 요청. 
	void DoMsg_GAMEMSG_REQUEST_REGISTMASTER(cltMsg* pclMsg, SI32 id);
	// 게임 종료 희망 통보. 
	void DoMsg_GAMEMSG_REQUEST_HOPETOQUIT(cltMsg* pclMsg, SI32 id);
	// 프론트로 되돌아가기 통보
	void DoMsg_GAMEMSG_REQUEST_HOPETOFRONT(cltMsg* pclMsg,SI32 id);
	// 해당 마을로 Warp를 희망함 
	void DoMsg_GAMEMSG_REQUEST_WARPVILLAGE(cltMsg* pclMsg, SI32 id);
	// 주민이 자기 마을로 Warp를 희망함 
	void DoMsg_GAMEMSG_REQUEST_RESIDENTWARPVILLAGE(cltMsg* pclMsg, SI32 id);

	// 육조 판서 
	void DoMsg_GAMEMSG_REQUEST_GETBYUNGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETGONGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETHYUNGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETHOZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETIZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETYEZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETGOONZUINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SETBYUNGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETBYUNGZOINFO_VILLAGEWARINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETGONGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETHYUNGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETHYUNGZOPRISON(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETHOZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETIZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETYEZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETGOONZUINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GETANGELAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETANGELLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETANGELAPPLYER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETANGEL(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GETCHANGEDNAME(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_AUTOHUNTING(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_AUTOHUNTING_ASK(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SETPOLICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETPOLICELIST(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SETCRIMINAL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETCRIMINALLIST(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GOONZU_SETMINISTER(cltMsg* pclMsg,SI32 id);

	// 군주가 퇴출 마을 설정
	void DoMsg_GAMEMSG_REQUEST_GOONZU_SETLIQUIDATION(cltMsg* pclMsg,SI32 id);

	// 군주가 보너스 타임을 설정
	void DoMsg_GAMEMSG_REQUEST_GOONZU_GETBONUSTIME(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GOONZU_SETBONUSTIME(cltMsg* pclMsg,SI32 id);

	// 환경 설정
	void DoMsg_GAMEMSG_REQUEST_WHISPERCHAT_REJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_REJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PARTYINVITE_REJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_FRIENDADD_REJECT(cltMsg* pclMsg, SI32 id);

	// 케릭터 옵션셋팅
	void DoMsg_GAMEMSG_REQUEST_CHAR_OPTION(cltMsg* pclMsg, SI32 id);
	
	// 스테이터스 초기화 요청 - 상태초기화이용권 사용
	void DoMsg_GAMEMSG_REQUEST_INITSTATUS(cltMsg* pclMsg, SI32 id);

	// 이름의 변경을 요청한다.
	void DoMsg_GAMEMSG_REQUEST_CHANGENAME(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_HUNTMAPWARP(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PERSONALSHOPWARP(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_FRIENDWARP(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PARTYWARP(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_PVPMAPWARP(cltMsg* pclMsg,SI32 id);
	// 30이하 유저들을 짐꾼왜구 마을로 보낸다
	void DoMsg_GAMEMSG_REQUEST_WARJOIN(cltMsg* pclMsg,SI32 id);

	// 운영자가 사용자의 로그를 요청한다.
	void DoMsg_GAMEMSG_REQUEST_PERSONAL_LOG(cltMsg* pclMsg, SI32 id);

	// Server-LogIn.cpp
	void DoMsg_GAMEMSG_REQUEST_MAKENEWCHAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_LOGIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DELUSERCHAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PREDELUSERCHAR(cltMsg* pclMsg, SI32 id);//케릭터삭제유예기간[성웅]
	void DoMsg_GAMEMSG_REQUEST_SELECTCHAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HANAUTH_LOGIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TAIWAN_LOGIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HANAUTH_GAMERUNAUTH(cltMsg* pclMsg, SI32 id);
	
	// 클라이언트가 준비되었다. 
	void DoMsg_GAMEMSG_REQUEST_READY(cltMsg* pclMsg, SI32 id);


	// Server-Item.cpp
	// 아이템을 바닥에 떨어뜨린다. 
	void DropItem(SI32 mapindex, SI32 x, SI32 y, cltItem* pclitem, SI32 droppersonid);

	void DoMsg_GAMEMSG_REQUEST_ITEMMOVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMCHANGE(cltMsg* pclMsg, SI32 id);
	void UseItemAction(SI32 id, cltItemUseInfo* pcliteuseinfo);
	void DoMsg_GAMEMSG_REQUEST_USEITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMSPLIT(cltMsg* pclMsg, SI32 id);

	// 귀속아이템을 파괴할것을 요청
	void DoMsg_GAMEMSG_REQUEST_BELONGITEM_DESTROY(cltMsg* pclMsg, SI32 id);

	// 아이템분해 관련 요청
	void DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_INVREPAIR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_INVDROP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ENCHANT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_INVPICKUP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MAKEITEMSWITCH(cltMsg* pclMsg, SI32 id);
	// 인벤에서 아이템을 만들 것을 요청. 
	void DoMsg_GAMEMSG_REQUEST_MAKEITEMINV(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_REQUEST_ENCHANTITEMINV(cltMsg* pclMsg, SI32 id);

	// 아이템 기간연장 요청.
	void DoMsg_GAMEMSG_REQUEST_ENCHANT_USEDAYPLUS(cltMsg* pclMsg, SI32 id);

	// 아이템 속성 변경 요청
	void DoMsg_GAMEMSG_REQUEST_CHANGEENCHANT(cltMsg* pclMsg, SI32 id);

	// 사람에게서 소환수로 아이템 이동 요청
	void DoMsg_GAMEMSG_REQUEST_SUMMONPUSHITEM(cltMsg* pclMsg, SI32 id);
	// 소환수에서 사람에게로 아이템 이동요청
	void DoMsg_GAMEMSG_REQUEST_SUMMONPOPITEM(cltMsg* pclMsg, SI32 id);

	// NPC수리 요청. 
	void DoMsg_GAMEMSG_REQUEST_NPCREPAIR(cltMsg* pclMsg, SI32 id);
	// NPC수리 가격 요청. 
	void DoMsg_GAMEMSG_REQUEST_NPCREPAIRPRICE(cltMsg* pclMsg, SI32 id);
	// WEAPON 1,2 Activate변경 
	void DoMsg_GAMEMSG_REQUEST_EXCHANGEWEAPON(cltMsg* pclMsg, SI32 id);

	// 연금술사로부터 아이템 변경을 요청
	void DoMsg_GAMEMSG_REQUEST_INV_ChangItemFromNPC(cltMsg* pclMsg,SI32 id);

	// 밀봉해제 요청
	void DoMsg_GAMEMSG_REQUEST_INV_CANCELLATIONSEALINGUP(cltMsg* pclMsg,SI32 id);

	// 아이템 제조창
	void DoMsg_GAMEMSG_REQUEST_MAKEITEMINVINDEX(cltMsg* pclMsg,SI32 id);

	// 인챈트창 
	void DoMsg_GAMEMSG_REQUEST_ENCHANTITEMINVINDEX(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ENCHANTITEMSTARTOREND(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ENCHANTOTHERSHOW(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ENCHANTOTHERPRICE(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ENCHANTITEMSWITCH(cltMsg* pclMsg,SI32 id);

	// clothes 1,2 Activate변경 
	void DoMsg_GAMEMSG_REQUEST_EXCHANGECLOTHES(cltMsg* pclMsg, SI32 id);


	// 아이템 업그레이드( 일반 -> 호품 -> 명품 -> 신기 ) - 혹부리 영감 이용권을 사용해서
	void DoMsg_GAMEMSG_REQUEST_UPGRADEITEM(cltMsg* pclMsg, SI32 id);
	// 아이템 성능 업그레이드( 혹부리 영감 )
	void DoMsg_GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM(cltMsg* pclMsg, SI32 id);

	// 미니 게임 : 몬스터 주식회사를 시작한다.
//	void DoMsg_GAMEMSG_REQUEST_MONSTERCORP_START(cltMsg* pclMsg,SI32 id);

	// 미니 게임 : 몬스터 주식회사 점수
	void DoMsg_GAMEMSG_REQUEST_MONSTERCORP_SCORE(cltMsg* pclMsg,SI32 id);

	// 미니 게임 : 몬스터 주식회사 콤보정보
	void DoMsg_GAMEMSG_REQUEST_MONSTERCORP_COMBOINFO(cltMsg* pclMsg,SI32 id);

	// 미니 게임 : 박대박 베팅 정보
	void DoMsg_GAMEMSG_REQUEST_ROULETTEGAME_INFO(cltMsg* pclMsg,SI32 id);

	// 미니 게임 : 박대박 중지
	void DoMsg_GAMEMSG_REQUEST_ROULETTEGAME_STOP(cltMsg* pclMsg,SI32 id);

	//Server-Structure.cpp
	void DoMsg_GAMEMSG_REQUEST_REPAIRSTRUCTURE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_CHIEFINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STRINFO(cltMsg* pclMsg, SI32 id);
	// 마을 상세 정보중에서 특정 건물의 정보를 요청
	void DoMsg_GAMEMSG_REQUEST_VILLAGEINFOSTRINFO(cltMsg* pclMsg, SI32 id);
	// 마을 마크를 설정한다
	void DoMsg_GAMEMSG_REQUEST_SETVILLAGEMARK(cltMsg* pclMsg, SI32 id);
	// 마을 지도를 설정한다
	void DoMsg_GAMEMSG_REQUEST_SETVILLAGEMAP(cltMsg* pclMsg, SI32 id);
	// 세금 납부 내역 요청 
	void DoMsg_GAMEMSG_REQUEST_GETTAXLIST(cltMsg* pclMsg, SI32 id);
	// 세금 설정
	void DoMsg_GAMEMSG_REQUEST_SETTAX(cltMsg* pclMsg, SI32 id);
	// 세금 납부
	void DoMsg_GAMEMSG_REQUEST_PAYTAX(cltMsg* pclMsg, SI32 id);

	// 배당 내역 리스트 요청 
	void DoMsg_GAMEMSG_REQUEST_GETDIVIDENDLIST( cltMsg *pclMsg, SI32 id );

	// 배당 하기 요청 
	void DoMsg_GAMEMSG_REQUEST_PAYDIVIDEND( cltMsg *pclMsg, SI32 id );

	// 마을 주민 배당
	void DoMsg_GAMEMSG_REQUEST_RESIDENTSHARE( cltMsg *pclMsg, SI32 id );

	// 마을 주민(NPC) 고용 요청
	void DoMsg_GAMEMSG_REQUEST_HIRENPC( cltMsg *pclMsg, SI32 id );

	// 마을 주민(NPC) 급여 정보 요청
	void DoMsg_GAMEMSG_REQUEST_NPCPAYMENT( cltMsg *pclMsg, SI32 id );


	void DoMsg_GAMEMSG_REQUEST_GETNPCLIST( cltMsg *pclMsg, SI32 id );			// 마을 주민(NPC) 리스트 요청 

	void DoMsg_GAMEMSG_REQUEST_APPLYRESIDENT( cltMsg *pclMsg, SI32 id );		// 주민 등록 신청 
	void DoMsg_GAMEMSG_REQUEST_APPLYRESIDENT2( cltMsg *pclMsg, SI32 id );		// [진성] 스승, 제자. 누군가의 초대로 인한 주민등록 신청.
	void DoMsg_GAMEMSG_REQUEST_CANCELRESIDENT( cltMsg *pclMsg, SI32 id );		// 주민 등록 취소 
	void DoMsg_GAMEMSG_REQUEST_BERESIDENT( cltMsg *pclMsg, SI32 id );			// 주민 등록 신청 허가
	void DoMsg_GAMEMSG_REQUEST_RESIDENTAPPLYERLIST( cltMsg *pclMsg, SI32 id );	// 주민 신청자 리스트 요청 
	void DoMsg_GAMEMSG_REQUEST_RESIDENTLIST( cltMsg *pclMsg, SI32 id );			// 주민 리스트 요청 
	void DoMsg_GAMEMSG_REQUEST_RESIDENTINFO( cltMsg *pclMsg, SI32 id );			// 주민 정보 요청 
	void DoMsg_GAMEMSG_REQUEST_SETRESIDENTSCORE( cltMsg *pclMsg, SI32 id );		// 주민 점수 부여 

	void DoMsg_GAMEMSG_REQUEST_SETSTRUCTUREMESSAGE( cltMsg *pclMsg, SI32 id );	// 안내문 설정 

	void DoMsg_GAMEMSG_REQUEST_VILLAGEBOARD(cltMsg *pclMsg, SI32 id );

	// 선전포고 
	void DoMsg_GAMEMSG_REQUEST_DECLAREWAR( cltMsg *pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_CANCELDECLAREWAR( cltMsg *pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_SETSYMBOLSTATUS( cltMsg *pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SETCASTLESTATUS( cltMsg *pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_GETVILLAGEINFO( cltMsg *pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_RESIDENTSLIST( cltMsg *pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_GETSTRNOTICE( cltMsg *pclMsg, SI32 id );


	void DoMsg_GAMEMSG_REQUEST_ALLSTRCHIEFINFO( cltMsg *pclMsg, SI32 id );

	// Server-Bank.cpp
	void DoMsg_GAMEMSG_REQUEST_INPUTTOBANK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_OUTPUTFROMBANK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_OUTPUTFROMBANKALLMONEY(cltMsg* pclMsg, SI32 id);	// 소지금 모두 출금(수수료 제외)
	void DoMsg_GAMEMSG_REQUEST_SETBANKFEE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CREATEBANKACCOUNT(cltMsg* pclMsg, SI32 id);		// 계좌를 개설하라는 요청.
	void DoMsg_GAMEMSG_REQUEST_CLOSEBANKACCOUNT(cltMsg* pclMsg, SI32 id);		// 계좌를 해지하라는 요청.
	void DoMsg_GAMEMSG_REQUEST_INPUTTOBANKALLMONEY(cltMsg* pclMsg, SI32 id);	// 소지금 모두 입금 

	// Server-Stock.cpp
	void DoMsg_GAMEMSG_REQUEST_BUYSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SELLSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CANCELSTOCKORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STOCKORDERINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETSTOCKFEE(cltMsg* pclMsg, SI32 id);


	// 증자 주식 경매 
	void DoMsg_GAMEMSG_REQUEST_STOCKBIDVILLAGELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STOCKBIDDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BIDSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CANCELBIDSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STOCKBIDDATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BUYBIDSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MYSTOCKINFO(cltMsg* pclMsg, SI32 id);

	// 주식 공모	
	void DoMsg_GAMEMSG_REQUEST_STOCKDISTRIBUTIONORDER(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GETBEGINNERVILLAGESTOCK(cltMsg* pclMsg, SI32 id);


	// tys
	void DoMsg_GAMEMSG_REQUEST_STOCKOFFERINGORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STOCKOFFERINGINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_EXCHANGESTOCK(cltMsg* pclMsg, SI32 id);

	// Server-PostOffice.cpp
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMSG(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVMONEYLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_RECVMSGLIST(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETMONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_DELMSG(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SETFEE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_GETRECORD(cltMsg* pclMsg, SI32 id);

	//[진성] 우체국에서 단체 메시지 보내기. => 2008-8-4
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDITEM_GROUP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY_GROUP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_SENDMSG_GROUP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_POSTOFFICE_USER_LIST_GROUP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CHAR_NAME_CHECK(cltMsg* pclMsg, SI32 id);

	//-------------------------------------
	// 시전 연장 하지 못 한 유저의 아이템 찾기
	//-------------------------------------	
	void DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GET_SAVEUSERHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MARKETCONDITIONS(cltMsg* pclMsg, SI32 id);


	// 사복시 
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_MYACCOUNT(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_OUTHORSE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_SELLHORSE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_BUYHORSE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HORSEMARKET_SETFEE(cltMsg* pclMsg, SI32 id );


	// 거간
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_MYORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_SELL(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_BUY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_SETFEE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REALESTATEMARKET_DETAILINFO(cltMsg* pclMsg, SI32 id );

	// 장예원
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_MYORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_SELL(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_BUY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_SETFEE(cltMsg* pclMsg, SI32 id );


	// 농장
	void DoMsg_GAMEMSG_REQUEST_LAND_CHANGECONTRACT(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_UNITLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_UNITDETAIL(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_RENTAPPLY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_CANCELAPPLY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_APPLYERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_DELAPPLYER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_LAND_RENT(cltMsg* pclMsg, SI32 id );

	// 내자시 
	void DoMsg_GAMEMSG_REQUEST_FEAST_RESERVE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_ADDPRESENTLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_DELPRESENTLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_SETCOST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_STGITEMIN( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_STGITEMOUT( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_GIVEITEM( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FEAST_SENDCHAT( cltMsg* pclMsg, SI32 id );

	//#if defined(_LEON_FEAST_EFFECT)
	void DoMsg_GAMEMSG_REQUEST_FEAST_USEEFFECT( cltMsg* pclMsg, SI32 id );
	//#endif
	
	//[진성] 파티홀. 
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_TOTAL_INFO( cltMsg* pclMsg, SI32 id );						// 파티장 전체 정보.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_ADD_PRESENT( cltMsg* pclMsg, SI32 id );					// 예약된 파티장에 선물 추가.
	void DoMsg_GAMEMSG_REQUEST_GAMEMSG_REQUEST_PARTYHALL_RESERVATION( cltMsg* pclMsg, SI32 id );	// 파티장 예약 신청.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION( cltMsg* pclMsg, SI32 id );			// 입장 신청.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_GET_COSTUME( cltMsg* pclMsg, SI32 id );					// 옷 얻기.	=> 2008. 3. 12
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION_LIST( cltMsg* pclMsg, SI32 id );		// 입장 신청자  리스트.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_ADD_DECORATION( cltMsg* pclMsg, SI32 id );					// 장식품 추가.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_GIVEITEM( cltMsg* pclMsg, SI32 id );						// 선물 주기.
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_EXIT( cltMsg* pclMsg, SI32 id );								
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_INVITATION( cltMsg* pclMsg, SI32 id );						// 파티장 초대.		
	void DoMsg_GAMEMSG_REQUEST_PARTYHALL_USEEFFECT( cltMsg* pclMsg, SI32 id );						// 파티장 폭죽.		

	// 비변사
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_MYORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_BUY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_SETFEE(cltMsg* pclMsg, SI32 id );

	// 광산
	void DoMsg_GAMEMSG_REQUEST_MINE_CHANGECONTRACT(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_UNITLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_UNITDETAIL(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_RENTAPPLY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_CANCELAPPLY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_APPLYERLIST(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_DELAPPLYER(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MINE_RENT(cltMsg* pclMsg, SI32 id );


	// Server-CityHall.cpp
	void DoMsg_GAMEMSG_REQUEST_INVESTLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BECHIEF(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CANDIDATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CANCELCANDIDATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CANDIDATELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DELCANDIDATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_RESIGN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETSTRUCTURELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BUILDSTRUCTURE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_DESTROYSTRUCTURE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_VILLAGENOTICE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_PROFITTOCAPITALFUND( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_STARTEVENT( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_VILLAGELEVELDETAILINFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_CHANGESTRBASEMONEY( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_HIREEVENTNPC( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_DONATEVILLAGEMONEY( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_EVENT_INSERTMATERIAL( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_DONATEMEMBERLIST( cltMsg* pclMsg, SI32 id );

	//Server-Market.cpp
	void DoMsg_GAMEMSG_REQUEST_SELLITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETFROMMARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MARKETORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BUYITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETMARKETFEE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PRODUCTPRICE( cltMsg* pclMsg, SI32 id );

	//Server-House.cpp
	void DoMsg_GAMEMSG_REQUEST_CHANGEHOUSECONTRACT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITLIST_DETAIL(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_REQUEST_HOUSEAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSERENTAPPLY(cltMsg* pclMsg, SI32 id);
	//시전사용 해약

	void DoMsg_GAMEMSG_REQUEST_HOUSECANCELLATIONPENALTYMONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSECANCELLATION(cltMsg* pclMsg, SI32 id);
	// 특정 마을에서의 임차 신청을 취소한다. 
	void DoMsg_GAMEMSG_REQUEST_HOUSERENTAPPLYCANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSERENT(cltMsg* pclMsg, SI32 id);
	// 인벤에서 창고로 아이템을 이동. 
	void DoMsg_GAMEMSG_REQUEST_MOVEITEMINVTOSTG(cltMsg* pclMsg, SI32 id);
	// 창고에서 인벤으로 이템을 이동. 
	void DoMsg_GAMEMSG_REQUEST_MOVEITEMSTGTOINV(cltMsg* pclMsg, SI32 id);
	// 창고정보를 요청. 
	void DoMsg_GAMEMSG_REQUEST_STGINFO(cltMsg* pclMsg, SI32 id);
	// 창고에서 아이템을 만들 것을 요청. 
	void DoMsg_GAMEMSG_REQUEST_MAKEITEM(cltMsg* pclMsg, SI32 id);
	// 시전 임차 희망자 명단을 삭제한다. 
	void DoMsg_GAMEMSG_REQUEST_DELHOUSEAPPLYER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITSTABLECREATE(cltMsg* pclMsg, SI32 id);
	// 시전에 말을 넣어 달라고 요청
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITHORSEIN(cltMsg* pclMsg, SI32 id);
	// 시전에서 말을 꺼내 달라고 요청
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITHORSEOUT(cltMsg* pclMsg, SI32 id);	
	// 시전에서 고물상을 생성하라고 요청
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE(cltMsg* pclMsg, SI32 id);
	// 고물에서 아이템을 뽑아낼때 나올 아이템 정보
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO(cltMsg* pclMsg, SI32 id);
	// 고물에서 아이템을 뽑아내라고 요청
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS(cltMsg* pclMsg, SI32 id);
	// 창고 이름 변경
	void DoMsg_GAMEMSG_REQUEST_HOUSEUNITNAMECHANGE(cltMsg* pclMsg, SI32 id);
	// 창고 아이템 정리
	void DoMsg_GAMEMSG_REQUEST_HOUSEITEMARRANGEMENT(cltMsg* pclMsg, SI32 id);

	//-------------------------------------------------------------
	// 농업, 수산업, 축산업 등의 생산 경제 관련

	//-------------------------------------------------------------
	// 공통
	// Server-Agriculture.cpp
	// 자동 생산
	void DoMsg_GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN(cltMsg* pclMsg, SI32 id);


	//-------------------------------------------------------------
	// 농경
	// Server-Farming.cpp
	//-------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_FARMINGSTART(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_FARMINGPAYRENTFEE(cltMsg* pclMsg, SI32 id);

	//-------------------------------------------------------------
	// 광업
	// Server-Mining.cpp
	//-------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_MININGSTART(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MININGPAYRENTFEE(cltMsg* pclMsg, SI32 id);



	// Server-RealEstate.cpp
	// 부동산 계약 연장 요청. 
	void DoMsg_GAMEMSG_REQUEST_EXTENDCONTRACT(cltMsg* pclMsg, SI32 id);


	//Server-NPC.cpp
	void DoMsg_GAMEMSG_REQUEST_SELLNPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BUYNPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NPCSELLITEMINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_INVESTMENTGETINFO(cltMsg* pclMsg, SI32 id);

	// 1:1 거래 요청
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE(cltMsg* pclMsg, SI32 id);
	// 거래 취소.
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_CANCEL(cltMsg* pclMsg, SI32 id);	
	// 거래 목록에 아이템 추가 요청
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDITEM(cltMsg* pclMsg, SI32 id);
	// 거래 목록에서 삭제 요청
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_DELLIST(cltMsg* pclMsg, SI32 id);	
	// 거래 목록에서 아이템 삭제 요청
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_DELITEM(cltMsg* pclMsg, SI32 id);	
	// 가격 거래
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDPRICE( cltMsg* pclMsg, SI32 id );
	// 주식 거래
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDSTOCK( cltMsg* pclMsg, SI32 id );
	// 시전 거래
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDHOUSEUNIT( cltMsg* pclMsg, SI32 id );
	// 말 거래
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_ADDHORSE( cltMsg* pclMsg, SI32 id );
	// 개인 거래에서 내 목록을 수락 
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_MYACCEPT(cltMsg* pclMsg, SI32 id);
	// 개인 거래에서 내 목록을 취소
	void DoMsg_GAMEMSG_REQUEST_PRIVATETRADE_MYCANCEL(cltMsg* pclMsg, SI32 id);




	//------------------------------------------------------------
	// 파티	
	// 파티 생성
	//void DoMsg_GAMEMSG_REQUEST_PARTY_CREATE( cltMsg* pclMsg, SI32 id );
	// 파티 초대( 파티 생성이 안된 상황에서만 가능 )
	void DoMsg_GAMEMSG_REQUEST_PARTY_CREATEINVITE( cltMsg* pclMsg, SI32 id );
	// 파티 초대 생성에 응할때( 파티 생성이 안된 상황에서만 가능 )
	void DoMsg_GAMEMSG_REQUEST_PARTY_CREATEJOIN( cltMsg* pclMsg, SI32 id );
	// 파티 초대
	void DoMsg_GAMEMSG_REQUEST_PARTY_INVITE( cltMsg* pclMsg, SI32 id );
	// 파티 초대 거절
	void DoMsg_GAMEMSG_REQUEST_PARTY_INVITEREJECT( cltMsg* pclMsg, SI32 id );
	// 파티 참여
	void DoMsg_GAMEMSG_REQUEST_PARTY_JOIN( cltMsg* pclMsg, SI32 id );
	// 파티 탈퇴
	void DoMsg_GAMEMSG_REQUEST_PARTY_LEAVE( cltMsg* pclMsg, SI32 id );
	// 파티 강제퇴장
	void DoMsg_GAMEMSG_REQUEST_PARTY_FORCELEAVE( cltMsg* pclMsg, SI32 id );
	// 파티 유저 리스트 요청
	void DoMsg_GAMEMSG_REQUEST_PARTY_ALLUSERLIST( cltMsg* pclMsg, SI32 id );	
	// 파티 아이템 획득 방식 변경
	void DoMsg_GAMEMSG_REQUEST_PARTY_CHANGEITEMGETPATTERN( cltMsg* pclMsg, SI32 id);

	// [지연] 파티매칭으로 추가된 메세지 처리 함수들
	void DoMsg_GAMEMSG_REQUEST_PARTY_WANTJOIN( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_PARTY_WANTJOINRESULT( cltMsg* pclMsg, SI32 id );
	//------------------------------------------------------------


	//------------------------------------------------------------
	// 친구
	void DoMsg_GAMEMSG_REQUEST_FRIENDADD( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_FRIENDDEL( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDADDACCEPT( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_REJECTFRIENDADD( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDMEMO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDMEMOSET( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDCHANGEGROUP( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_FRIENDCHANGEGROUPNAME( cltMsg* pclMsg, SI32 id );
	//------------------------------------------------------------


	//------------------------------------------------------------
	// 투표
	void DoMsg_GAMEMSG_REQUEST_VOTEKINGSELECTION( cltMsg* pclMsg, SI32 id );
	// [영훈] 선거시스템 : 유세활동 시작
	void DoMsg_GAMEMSG_REQUEST_STARTELECTIONSTUMP( cltMsg* pclMsg, SI32 id );
	// [영훈] 선거시스템 : 유세활동 종료
	void DoMsg_GAMEMSG_REQUEST_ENDELECTIONSTUMP( cltMsg* pclMsg, SI32 id );
	// [영훈] 선거시스템 : 케릭터 정보가 초기화 되서 유세 정보를 다시 보내달라고 요청
	void DoMsg_GAMEMSG_REQUEST_ELECTIONSTUMP( cltMsg* pclMsg, SI32 id );
	// [영훈] 선거시스템 : 군주 투표소 지역으로 이동하라
	void DoMsg_GAMEMSG_REQUEST_WARPTOPOLLMAP( cltMsg* pclMsg, SI32 id );
	// [영훈] 선거시스템 : 군주 투표창을 열어라
	void DoMsg_GAMEMSG_REQUEST_VOTEKINGAVAILABLE( cltMsg* pclMsg, SI32 id );
	// [영훈] 선서시스템 : 개표 시작 정보를 요청
	void DoMsg_GAMEMSG_REQUEST_STARTVOTESTATUS( cltMsg* pclMsg, SI32 id );
	//------------------------------------------------------------

	//------------------------------------------------------------
	// Daily 퀘스트
	void DoMsg_GAMEMSG_REQUEST_DAILYQUESTLIST( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuestSelect( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuestSituation( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuestEnd( cltMsg* pclMsg, SI32 id );	
	//------------------------------------------------------------

	//------------------------------------------------------------
	// Dail 퀘스트2
	void DoMsg_GAMEMSG_REQUEST_DAILYQUEST2LIST( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest2Select( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest2Situation( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest2End( cltMsg* pclMsg, SI32 id );	
	//------------------------------------------------------------
	//------------------------------------------------------------
	// Dail 퀘스트3
	void DoMsg_GAMEMSG_REQUEST_DAILYQUEST3LIST( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest3Select( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest3Situation( cltMsg* pclMsg, SI32 id );	
	void DoMsg_GAMEMSG_REQUEST_DailyQuest3End( cltMsg* pclMsg, SI32 id );	
	//------------------------------------------------------------

	//-----------------------------------------------------------------------
	// ItemMall
	void DoMsg_GAMEMSG_REQUEST_ITEMMALLEXISTENCE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMMALLITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMMALLGETITEM(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_CHANGE_CHARKIND(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NOLIMITTICKET_WARP(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_WARP_TARGETWORLD(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GET_SEL_PGLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GET_ITEMMALL_ITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GET_MYCASH_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMMALL_BUYITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ITEMMALL_GIFTITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CHECK_ACCOUNTID(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BILLPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_LOGINPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GACHAPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CHANGE_HERO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PRESENT_ANSWER(cltMsg* pclMsg, SI32 id);

	//---------------------------------------
	// 개인 상점 
	//---------------------------------------
	void DoMsg_GAMEMSG_REQUEST_PERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PERSONALSHOPDATA(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_STARTPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_STOPPERSONALSHOP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_BUYPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SELLPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SEARCHPERSONALSHOPITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MAKEPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MAKEPERSONALSHOP_MANUFACTUER(cltMsg* pclMsg, SI32 id);	// 제조의뢰 인터페이스 개선 
	
	//---------------------------------------

	//---------------------------------------
	// 소환수 
	//---------------------------------------
	void DoMsg_GAMEMSG_REQUEST_SUMMONFREE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONNAMECHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONREVIVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_USESUMMONBONUS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TAKEINOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONATTACKCHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONEVOLVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONSCROLL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMON_STATUSINIT(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMON_SETAUTOHEAL(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SUMMONUSETON(cltMsg* pclMsg, SI32 id);// 소환수에 무기 장착석을 박는다,.
	void DoMsg_GAMEMSG_REQUEST_SUMMONSLOTIN(cltMsg* pclMsg, SI32 id);// Base에 있는 소환수를 Slot으로넣는다.
	void DoMsg_GAMEMSG_REQUEST_SUMMONSLOTOUT(cltMsg* pclMsg, SI32 id);//소환수를 Slot에서 뺴낸다.

	void DoMsg_GAMEMSG_REQUEST_SUMMON_TRANSFORM(cltMsg* pclMsg, SI32 id); //[진성] 소환수 변신.
	//---------------------------------------

	// 추천인 등록
	void DoMsg_GAMEMSG_REQUEST_RECOMMENDPERSON( cltMsg* pclMsg, SI32 id );

	// 자막 종류 변경
	void DoMsg_GAMEMSG_REQUEST_CAPTIONKINDCHANGE( cltMsg* pclMsg, SI32 id );

	// 유저와 외국 상인( 청, 일본 ) NPC 와의 물품 판매 작업
	void DoMsg_GAMEMSG_REQUEST_SELLTOFOREIGNMERCHANT(cltMsg* pclMsg, SI32 id);
	// 유저가 교역 상인 NPC 가 구매중인 아이템이 무엇인지 요청
	void DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTNPCBUYINFO(cltMsg* pclMsg, SI32 id);
	// 유저가 교역 상인 NPC 가 구매 중인 캐쉬 아이템이 무엇인지 요청( 종류별로 )
	void DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEMINFO(cltMsg* pclMsg, SI32 id);
	// 유저가 교역 상인에게 캐쉬 아이템 판매
	void DoMsg_GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM(cltMsg* pclMsg, SI32 id);
	// 유저가 NPC 에게서 자신의 추천포인트를 알아옴
	void DoMsg_GAMEMSG_REQUEST_GETRECOMMANDPOINT(cltMsg* pclMsg, SI32 id);
	// 유저가 NPC에게서 추천포인트로 아이템을 구입함.
	void DoMsg_GAMEMSG_REQUEST_BUYRECOMMANDPRODUCT(cltMsg* pclMsg, SI32 id);
	// 유저를 추천인으로 등록한 캐릭터들의 목록을 얻어옴
	void DoMsg_GAMEMSG_REQUEST_RECOMMANDPOINTLIST(cltMsg* pclMsg, SI32 id);
	// 유저가 NPC를 통해 아이템 제작
	void DoMsg_GAMEMSG_REQUEST_MANUFACTURENPC( cltMsg* pclMsg, SI32 id );
	// 초보자 무기 받기
	void DoMsg_GAMEMSG_REQUEST_RECEIVEBEGINNERITEM( cltMsg* pclMsg, SI32 id );

	// 유저가 외국상인에 물품 판매하기 전에 기간에 따른 정보 요청
	void DoMsg_GAMEMSG_REQUEST_PRESELLTOFOREIGNMERCHANT(cltMsg* pclMsg, SI32 id);

	// 무역 상인이 현재 구입하는 정보를 요청
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYINFO(cltMsg* pclMsg, SI32 id);
	// 무역 상인에게 유저가 품을 판매
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON(cltMsg* pclMsg, SI32 id);
	// 상인이 말을 구입하는 정보 요청
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTHORSEBUYINFO(cltMsg* pclMsg, SI32 id);
	// 상인이 말을 구입
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON(cltMsg* pclMsg, SI32 id);
	// 상인이 물건을 구입하는 정보를 요청
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMINFO(cltMsg* pclMsg, SI32 id);
	// 상인이 물건을 구입
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEM(cltMsg* pclMsg, SI32 id);
	// 상인이 소환수 사는 정보 요청
	void DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYSUMMONINFO(cltMsg* pclMsg,SI32 id);
	// 상인이 소환수를 구입
	void DoMsg_GAMEMSG_REQUEST_SUMMONTRADE(cltMsg* pclMsg,SI32 id);
	// 소환수를 캐릭터로 선택
	void DoMsg_GAMEMSG_REQUEST_SELECT_SUMMONCHAR(cltMsg* pclMsg,SI32 id);

	//매크로
	void DoMsg_GAMEMSG_RESPONSE_RECORD_ACTION( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_RECORD_ACTION_FINISHED( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_GZFTP_PROTOCOL( cltMsg* pclMsg, SI32 id );

	//Server-Auth.cpp
	void DoMsg_AUTHMSG_RESPONSE_GAMEAUTH(sPacketHeader* pPacket);
	void DoMsg_AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH(sPacketHeader* pPacket);
	void DoMsg_AUTHMSG_RESPONSE_GMTOOLAUTH(sPacketHeader* pPacket);
	
	//Server-Bill.cpp
	void DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW(sPacketHeader* pPacket);
	void DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW_WEB(sPacketHeader* pPacket);
	void DoMsg_BILLMSG_RESPONSE_ENCRYPTIDPW_GACHA(sPacketHeader* pPacket);

	//Server-Horse.cpp
	// 말을 타라는 요청에 대한 처리. 
	void DoMsg_GAMEMSG_REQUEST_RIDEHORSE(cltMsg* pclMsg, SI32 id);
	// 말에서 내리라는 요청에 대한 처리
	void DoMsg_GAMEMSG_REQUEST_UNRIDEHORSE(cltMsg* pclMsg, SI32 id);
	// 말 이름 변경
	void DoMsg_GAMEMSG_REQUEST_HORSENAMECHANGE(cltMsg* pclMsg, SI32 id);
	// 말상태초기화이용권으로 초기화 시킨다.
	void DoMsg_GAMEMSG_REQUEST_HORSEINIT(cltMsg* pclMsg, SI32 id);
	// 보너스로 말 능력치를 올린다.
	void DoMsg_GAMEMSG_REQUEST_USEHORSE_BONUS(cltMsg* pclMsg, SI32 id);
	// 말의 품종을 변경한다.
	void DoMsg_GAMEMSG_REQUEST_CHANGE_HORSETYPE(cltMsg* pclMsg,SI32 id);
	// 말능력향상이용권을 이용해서 능력 향상을 시킨다.
	void DoMsg_GAMEMSG_REQUEST_HORSE_ADDSTATUS(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_BLACKARMYAPPLY(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BLACKARMY_RANKINFO(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_BLACKARMY_WARPTOGATE(cltMsg* pclMsg,SI32 id);

	//--------------------------------------
	// 수렵장 
	//--------------------------------------
	// 수렵장에 들어간다. 
	void DoMsg_GAMEMSG_REQUEST_ENTERHUNT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTADD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTDEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HUNTMONSTERIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_HUNTMONSTEROUT(cltMsg* pclMsg, SI32 id);

	//---------------------------------------
	// 마을 상세 정보. 
	//---------------------------------------
	// 마을 상세 정보 요청 . 
	void DoMsg_GAMEMSG_REQUEST_VILLAGEDETAILINFO(cltMsg* pclMsg, SI32 id);



	//---------------------------------------
	// 주주 총회
	//---------------------------------------
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_SUGGEST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_VOTE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_VOTE_REFRESH(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_PARTICIPATE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHATJOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHATOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHAT(cltMsg* pclMsg, SI32 id);

	// 국가 정보 관련
	void DoMsg_GAMEMSG_REQUEST_SETCOUNTRYINFO( cltMsg *pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SETSHOWCOUNTRY( cltMsg *pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_USERLISTBYCOUNTRY( cltMsg *pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ADDCHINESEUSER( cltMsg *pclMsg, SI32 id);

	// 자신의 푸치코미 보기 관련
	void DoMsg_GAMEMSG_REQUEST_SETSHOWPUCHIKOMI( cltMsg *pclMsg, SI32 id );

	// Server-WorldMoney.cp
	void DoMsg_GAMEMSG_REQUEST_DISTRIBUTION_WORLDMONEY(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_WORLDMONEY_TRADELIST(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_WORLDMONEY_SELLORDER(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_WORLDMONEY_BUYORDER(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_WORLDMONEY_CANCELORDER(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_SET_SELLORDER(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_CANCEL_SELLORDER(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_LISTINFO(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_MYINFO(cltMsg*pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_SAVEDACU_LIST(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU(cltMsg*pclMsg,SI32 id);

	// WorldPassport( 무역 여권 )
	void DoMsg_GAMEMSG_REQUEST_WORLDPASSPORT_SET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_WORLDPASSPORT_EXTEND(cltMsg* pclMsg, SI32 id);


	// 20미만 레벨 유저 요청
	void DoMsg_GAMEMSG_REQUEST_LOWLEVELLIST( cltMsg*pclMsg,SI32 id );

	void DoMsg_GAMEMSG_REQUEST_SETEMOTICON( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GAME_GUARD_AUTH( cltMsg*  pclMsg , SI32 id );

	// Server-Guild
	void DoMsg_GAMEMSG_REQUEST_GUILD_CREATEGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_DESTROYGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_JOINGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_CONFIRMJOINGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_LEAVEGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_CHANGEVILLAGE( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_GETGUILDLIST( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_GETGUILDINFO( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_CONFIRMCREATEGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_SETSECONDMASTER( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_GIVEGUILDMONEY( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GUILD_SETSTRFEERATE(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_CHANGESTATUS(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_SETMARK(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_CHANGENAME(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_CHANGEMASTER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDWAR_RESERVELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDWAR_APPLYRESERVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDDUNGEON_APPLYLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDDUNGEON_APPLY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDDUNGEON_STATUS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_MEMBERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_GETVILLAGELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDWAR_APPLYTOENEMY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDWAR_CONFIRMAPPLY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILDWAR_RECOMMANDLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GUILD_NOTCONNECTMASTER_CHANGEMASTER(cltMsg* pclMsg, SI32 id);	// 한달이상 길드장이 접속 안했을때 길드장위임
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_MAPREGISTER(cltMsg* pclMsg, SI32 id);	// 길드 사냥터 도전 맵등록 
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_WARREGISTER(cltMsg* pclMsg, SI32 id);	// 길드 사냥터 길드전 신청
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_REQUEST(cltMsg* pclMsg, SI32 id);	// 길드 사냥터 Request에 대한 대답
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_TOP10(cltMsg* pclMsg, SI32 id);		// 각 맵마다 보여줄 top 10	
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_SERVERTIME(cltMsg* pclMsg, SI32 id);	// 길드 사냥터 도전 정확한 서버시간을 받아 온다.
	void DoMsg_GAMEMSG_REQUEST_GUILDHUNTMAPWAR_HAVEMAP(cltMsg* pclMsg, SI32 id);	// 길드 사냥터를 신청 했는지알아와
		// 가차
	void DoMsg_GAMEMSG_REQUEST_GACHAPRODUCT_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GACHABUY(cltMsg* pclMsg, SI32 id);

	// 가차2
	void DoMsg_GAMEMSG_REQUEST_GACHA2PRODUCT_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GACHA2BUY(cltMsg* pclMsg, SI32 id);

	// 이월 가차
	void DoMsg_GAMEMSG_REQUEST_CARRYOVER_GACHA_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CARRYOVER_SELECTPRODUCT_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CARRYOVER_GACHABUY(cltMsg* pclMsg, SI32 id);

	// 모후모후 이벤트
	void DoMsg_GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MOFUMOFUEVENT_ENTERFIELD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_MOFUMOFUEVENT_WARP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_NAMINGQUEST_START(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETPERSONCURRENTNAMING(cltMsg* pclMsg, SI32 id);
	//KHY - 0822 - 해피빈 퀘스트 추가.
	void DoMsg_GAMEMSG_REQUEST_HAPPYBEANQUEST_START(cltMsg* clMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SYSTEMREWARDLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_RECIEVESYSTEMREWARD(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_ATTEND(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_ATTENDREWARDITEM(cltMsg* pclMsg, SI32 id );

	//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
	void DoMsg_GAMEMSG_REQUEST_CHARSERVERRTN(cltMsg* pclMsg, SI32 id );
	
	// 제국
	void DoMsg_GAMEMSG_REQUEST_CREATEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_DESTROYEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_INVITEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_JOINEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SECEDEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_EMPIREINFO(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_SETEMPIREMONEY(cltMsg* pclMsg, SI32 id );

	//[추가 : 황진성 2007. 12. 3 => 클라이언트에서 서버로 오토프로그램 사용전달.]
	void DoMsg_GAMEMSG_REQUEST_AUTOMACRO_ON(cltMsg* pclMsg, SI32 id );
	
	//[추가 : 황진성 2007. 12. 11 => 신년 이벤튼 버튼를 클라이언트에서 선택 했음. C->S]
	void DoMsg_GAMEMSG_REQUEST_NEWYEAR_CLICK(cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_MISSIONQUEST_START(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MISSIONQUEST_ACCEPT(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_MISSIONQUEST_CANCEL(cltMsg* pclMsg, SI32 id );
	
	// [성웅] 보물찾기 이벤트
	void DoMsg_GAMEMSG_REQUEST_TREASUREEVENT_KILLNPC(cltMsg* pclMsg, SI32 id );
	
	//KHY - 1220 - 변신시스템.
	void DoMsg_GAMEMSG_REQUEST_TRANSFORM(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRANSFORM_END(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRANSFORM_USEITEM(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_OILLING_USEITEM(cltMsg* pclMsg,SI32 id);	
	//[추가 : 황진성 2008. 1. 9 => 인스턴트던젼.]
	void DoMsg_GAMEMSG_REQUEST_INSTANTSDUNGEON_STATUS(cltMsg* pclMsg,SI32 id);

	//명예의 전당 리스트 - 2008.01.08 손성웅
	void DoMsg_GAMEMSG_REQUEST_HOFLIST(cltMsg* pclMsg,SI32 id);

	//랭킹 리스트 - 2008.01.08 손성웅
	void DoMsg_GAMEMSG_REQUEST_RAKINGLIST(cltMsg* pclMsg,SI32 id);

//******************************************************************************************
//[추가 : 황진성 2008. 1. 29 =>  공간 상자 메시지 처리. C->S]
	
	void DoMsg_GAMEMSG_REQUEST_SPACE_BOX_LIST(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_TRESURE_BOX_LIST(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM(cltMsg* pclMsg,SI32 id);		//[추가 : 황진성 2008. 1. 30 => 공간 상자 저장. C->S]
	void DoMsg_GAMEMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM(cltMsg* pclMsg,SI32 id);	//[추가 : 황진성 2008. 1. 30 => 공간 상자 회수. C->S]

//******************************************************************************************
	//--------------------------------------------------------------
	// 추가 : 황진성 2008. 2. 28 => 아티펙트 관련 메시지. C -> S
	//--------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_ARTIFACT_MIXTURE(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ARTIFACT_REMOVE(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE(cltMsg* pclMsg,SI32 id);
	
	void DoMsg_GAMEMSG_REQUEST_OPENBIGITEMBOX(cltMsg* pclMsg,SI32 id);
	
//******************************************************************************************

	// [영훈] CRC 체크 요청 - 2008.02.29
	void DoMsg_GAMEMSG_REQUEST_FILECRCCHECK(cltMsg* pclMsg,SI32 id);
	// [영훈] 해적선 : 인던 입장 가능한지 물어봄
	void DoMsg_GAMEMSG_REQUEST_PIRATESHIP_ENTERFIELD(cltMsg* pclMsg,SI32 id);
	//KHY - 1113 - 크리스마스 이벤트 - 사냥터 분리 입장.
	void DoMsg_GAMEMSG_REQUEST_CHRISTMASEVET_ENTERFIELD(cltMsg* pclMsg,SI32 id);
	// PCK : 08.06.04 : 농경종료시에 종료시간 기록
	void DoMsg_GAMEMSG_REQUEST_FARMINGENDTIME(cltMsg* pclMsg, SI32 id);

	// [진성] 영웅의 동상 이벤트 버프룰렛 돌릴때 사용하는 패킷. => 2008-6-10
	void DoMsg_GAMEMSG_REQUEST_BUFF_ROULETTE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_BALANCECURRENCY_BUFF(cltMsg* pclMsg, SI32 id);
	
	// [진성] 서버 이벤트 랭킹 정보 요청. => 2008-6-20
	void DoMsg_GAMEMSG_REQUEST_SERVER_EVENT_RANK_INFO(cltMsg* pclMsg, SI32 id);

	// [진성] 서버 이벤트 보너스 스테이지 이동 요청. => 2008-6-24
	void DoMsg_GAMEMSG_REQUEST_SERVER_EVENT_BONUS_STAGE(cltMsg* pclMsg, SI32 id);
	// [영훈] 서버 이벤트 퀘스트 시작 정보 요청
	void DoMsg_GAMEMSG_REQUEST_SERVER_EVENT_QUEST_START(cltMsg* pclMsg, SI32 id);
	// [영훈] 서버 이벤트 인던 로비 입장 요청
	void DoMsg_GAMEMSG_REQUEST_SERVER_EVENT_LOBBY_ENTERFIELD(cltMsg* pclMsg, SI32 id);
	// [춘기] 소환수 스태미너 충전
	void DoMsg_GAMEMSG_REQUEST_SUMMON_SPTCHARGE(cltMsg* pclMsg, SI32 id);
	// [영훈] 멀티 클라이언트 갯수 보냄
	void DoMsg_GAMEMSG_REQUEST_CHECKPROCESS(cltMsg* pclMsg,SI32 id);

	//---------------------------------
	//[진성] 호박상자. => 2008-9-16
	//---------------------------------
	void DoMsg_GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET(cltMsg* pclMsg,SI32 id);

	//---------------------------------

	void DoMsg_GAMEMSG_REQUEST_CHAR_ROULETTE(cltMsg* pclMsg,SI32 id);

	void DoMsg_GAMEMSG_REQUEST_VILLAGE_RESIDENT_INVITE(cltMsg* pclMsg,SI32 id);
	
	void DoMsg_GAMEMSG_REQUEST_PVP_FILED(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PVP_LEAGUE_UI_INFOGET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PVP_LEAGUE_APPLY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PVP_LEAGUE_CANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PVP_LEAGUE_AGAIN_RESERVE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_QUESTION_LIST_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_QUESTION_CONTENTS_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CHECK_QUESTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ADD_QUESTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ADD_ANSWER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ADD_ANSWER_COMPLETE_RESULT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DEL_QUESTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_DEL_ANSWER(cltMsg* pclMsg, SI32 id);
	
	

	// PVP_League
	void DoMsg_GAMEMSG_REQUEST_PVP_LEAGUE(cltMsg* pclMsg, SI32 id);

	// 신년운세 룰렛
	void DoMsg_GAMEMSG_REQUEST_DIVINATION_ROULETTE(cltMsg* pclMsg,SI32 id);

	//[진성] 엘릭서.
	void DoMsg_GAMEMSG_REQUEST_ELIXIR_NPC_ITEM_TYPE_LIST_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ELIXIR_NPC_CHANGE_ITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_ELIXIR_SET(cltMsg* pclMsg, SI32 id);

	// [성웅] 발렌타인 데이 솔로냐 커플이냐는 질문에 대한 답변
	void DoMsg_GAMEMSG_REQUEST_VALENTINEWAR_REQUEST(cltMsg* pclMsg, SI32 id);
	// [ 성웅] 발렌타인데이의 정보를 업데이트 한다.
	void DoMsg_GAMEMSG_REQUEST_VALENTINEWAR_INFOGET(cltMsg* pclMsg, SI32 id);

	// [영훈] 새로운 선택상자 상자 아이템
	void DoMsg_GAMEMSG_REQUEST_NEWSELECTBOX_SELECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NEWSELECTBOX_GETITEM(cltMsg* pclMsg, SI32 id);

	//=============================================================================================
	// [진성] 통합마켓.
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_NEWMARKETACCOUNT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NEWMARKETITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NEWSELLITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_GETFROMNEWMARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NEWBUYITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_SETNEWMARKETFEE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_PERSONNAME_SERIALID(cltMsg* pclMsg,SI32 id);
	
	//=============================================================================================
	// [진성] 프로모터.
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_PROMOTER_INFO_TOTAL_GET(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_PROMOTER_INFO_GET(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_PROMOTER_URL_GET(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_PROMOTERNPC_GIVE_ITEM_GET(cltMsg* pclMsg, SI32 id );

	// [영훈] 결혼 시스템
	void DoMsg_GAMEMSG_REQUEST_PROPOSE_QUESTION(cltMsg* pclMsg, SI32 id );				// 프로포즈 질문
	void DoMsg_GAMEMSG_REQUEST_PROPOSE_ANSWER(cltMsg* pclMsg, SI32 id );				// 프로포즈 답변
	void DoMsg_GAMEMSG_REQUEST_WEDDINGANNIVERSARY_ITEMGET(cltMsg* pclMsg, SI32 id );	// 결혼 기념일 아이템 받기 요청

	void DoMsg_GAMEMSG_REQUEST_WEDDING_RESERVE(cltMsg* pclMsg, SI32 id );				// 결혼 예약 하기
	void DoMsg_GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET(cltMsg* pclMsg, SI32 id );		// 결혼 간단 예약 리스트 받기
	void DoMsg_GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER(cltMsg* pclMsg, SI32 id );		// 결혼 관련 질문에 대한 대답
	void DoMsg_GAMEMSG_REQUEST_WEDDING_LIST_GET(cltMsg* pclMsg, SI32 id );				// 결혼 예약 리스트 받기
	void DoMsg_GAMEMSG_REQUEST_WEDDING_HALL_ENTER(cltMsg* pclMsg, SI32 id );			// 결혼식장 들어가기
	void DoMsg_GAMEMSG_REQUEST_WEDDING_RESERVE_CANCEL(cltMsg* pclMsg, SI32 id );		// 결혼 예약 취소하기
	void DoMsg_GAMEMSG_REQUEST_MARRIAGEMATE_WARP(cltMsg* pclMsg, SI32 id );				// 배우자에게 워프하기

	void DoMsg_GAMEMSG_REQUEST_BALANCECANUSECURRENCY(cltMsg* pclMsg, SI32 id );			// [종호] 사용 가능 통화량 요구

	// [성웅]가족 시스템
	void DoMsg_GAMEMSG_REQUEST_FAMILY_REQUEST_ADD_FAMILY(cltMsg* pclMsg, SI32 id );		// [성웅] 가족 시스템 - 자녀 등록 요청
	void DoMsg_GAMEMSG_REQUEST_FAMILY_ADD_FAMILY(cltMsg* pclMsg, SI32 id );				// [성웅] 가족 시스템 - 자녀 등록
	void DoMsg_GAMEMSG_REQUEST_FAMILY_DEL_CHILDREN(cltMsg* pclMsg, SI32 id );			// [성웅] 가족 시스템 - 자녀 삭제 
	void DoMsg_GAMEMSG_REQUEST_FAMILY_WARP(cltMsg* pclMsg, SI32 id );						// [성웅] 가족 시스템 - 타겟 가족으로 워프 
	void DoMsg_GAMEMSG_REQUEST_FAMILY_PLUS_FAMILYEFFECT(cltMsg* pclMsg, SI32 id );			// [성웅] 가족 시스템 - 자녀 삭제 
	void DoMsg_GAMEMSG_REQUEST_FAMILY_CHAT(cltMsg* pclMsg, SI32 id );						// [성웅] 가족 시스템 - 타겟 가족으로 워프 

	// [영훈] 수호정령
	void DoMsg_GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT(cltMsg* pclMsg, SI32 id );						// [영훈] 수호정령 : 소환/해재
	void DoMsg_GAMEMSG_REQUEST_SOULGUARD_CHANGENAME(cltMsg* pclMsg, SI32 id );						// [영훈] 수호정령 : 이름변경
	void DoMsg_GAMEMSG_REQUEST_SOULGUARD_DEAD(cltMsg* pclMsg, SI32 id );							// [영훈] 수호정령 : 죽음/풀어주기

	void DoMsg_GAMEMSG_REQUEST_SOULGUARD_HOUSE_CHANGE(cltMsg* pclMsg, SI32 id );					// [영훈] 캐릭터에 있는 수호정령과 보관소에 있는 수호정령을 맞바꾼다
	void DoMsg_GAMEMSG_REQUEST_SOULGUARD_HOUSE_CREATE(cltMsg* pclMsg, SI32 id );					// [영훈] 수호정령보관소 갯수 늘림

	// [ 성웅 ] 라쿤 피하기이벤트
	//Switch_10MonthEvent_RacoonDodge
	void DoMsg_GAMEMSG_REQUEST_RACOONDODGEEVENTRESERVATION(cltMsg* pclMsg, SI32 id );				// [성웅] 라쿤 피하기 이벤트 예약 신청 
	void DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_REQUEST_PLAYEVENT(cltMsg* pclMsg, SI32 id );		// [성웅] 라쿤 피하기 참여 수락
	void DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_OVER5SECOND_START(cltMsg* pclMsg, SI32 id );		// [성웅] 라쿤 피하기 이벤트 5초 카운트다운 종료 시작
	void DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_GET_RESERVE_LIST(cltMsg* pclMsg, SI32 id );		// [성웅] 라쿤 피하기 이벤트 5초 카운트다운 종료 시작

	void DoMsg_GAMEMSG_REQUEST_PROMOTER_WARP(cltMsg* pclMsg, SI32 id);								// [진성] 프로모터가 새내기 캐릭에게 워프.
	// [ 기형 ] 휴면 계정 시스템
	void DoMsg_GAMEMSG_REQUEST_DORMANCY_TAKEOUTEQUIPITEMINBOX(cltMsg* pclMsg, SI32 id );			// [기형] 휴면 계정 시스템
	void DoMsg_GAMEMSG_REQUEST_DORMANCY_BUYPOINTITEM(cltMsg* pclMsg, SI32 id );						// [기형] 휴면 계정 시스템

	//  [8/12/2009 종호_GAWIBAWIBO]	
	void DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_CHARGED_MONEY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_CALCULATE_RESULT(cltMsg* pclMsg, SI32 id );
	//void DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_COUNT(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_GAWIBAWIBO_REWARD(cltMsg* pclMsg, SI32 id);
	

	// [지연] 복불복
	void DoMsg_GAMEMSG_REQUEST_BBB_CHECK_PlAY_CONDITION(cltMsg* pclMsg, SI32 id);			// [지연] 게임 참여 여부 체크
	void DoMsg_GAMEMSG_REQUEST_BBB_WAITLIST_ANSWER(cltMsg* pclMsg, SI32 id);				// [지연] 복불복 대기자리스트 수락
	void DoMsg_GAMEMSG_REQUEST_BBB_WAITLIST_CANCEL(cltMsg* pclMsg, SI32 id);				// [지연] 대기 리스트 취소 요청 (클라->서버)
	void DoMsg_GAMEMSG_REQUEST_BBB_WARPMAPANSWER(cltMsg* pclMsg, SI32 id);					// [지연] 1순위 대기자의 "워프할래요?"에 대한 응답 (클라->서버)
	void DoMsg_GAMEMSG_REQUEST_BBB_ALREADYRESERVED(cltMsg* pclMsg, SI32 id);				// [지연] 이미 예약중인지 물어

	// [지연] 2010신년이벤트
	void DoMsg_GAMEMSG_REQUEST_2010NEWYEAREVENT_QUESTCANCEL(cltMsg* pclMsg, SI32 id);		// [지연] 퀘 거절 메세지처리

	void DoMsg_GAMEMSG_REQUEST_NEWNPCREPAIR(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_NEWITEMREPAIR(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO(cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_REQUEST_FESTIVAL_EVENT_ENTERFIELD_EVENTAREA(cltMsg* pclMsg, SI32 id );		// [기형] 축제 이벤트 지역으로 이동 요청

	void DoMsg_GAMEMSG_REQUEST_HALLOWEENSPIRITBOX_OK(cltMsg* pclMsg, SI32 id );		// [종호] 할로윈 스피릿 박스

	void DoMsg_GAMEMSG_REQUEST_MONSTERCREATE(cltMsg* pclMsg, SI32 id);				// [춘기] 몬스터 생성요청

	void DoMsg_GAMEMSG_REQUEST_PERSONSLOTNUMCHANGE(cltMsg* pclMsg, SI32 id);		// [영훈] 캐릭터 보관소에서 가져온다
	
	void DoMsg_GAMEMSG_REQUEST_EQUIPMENTCREATE(cltMsg* pclMsg, SI32 id);			// [춘기] 장비 생성요청

	void DoMsg_GAMEMSG_REQUEST_SKILLBOOK_BUYITEM(cltMsg* pclMsg, SI32 id);			// []
	void DoMsg_GAMEMSG_REQUEST_SKILLBOOK_SUBMAGIC_DEL(cltMsg* pclMsg, SI32 id);		//

	//-------------------------------------------------------------------------------------------------------------------
	//[성웅] 라쿤이벤트 군주 이벤트로 변경
	//-------------------------------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_RACCOONEVENT_TOGOONZU(cltMsg* pclMsg, SI32 id);		//[성웅] 라쿤이벤트 군주 이벤트로 변경
	void DoMsg_GAMEMSG_REQUEST_GOONZURACCOONEVENT_SETINFO(cltMsg* pclMsg, SI32 id);		//[성웅] 라쿤이벤트 군주 이벤트로 변경

	//=============================================================================================
	// [기형] 배틀로얄
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_BATTLEROYAL_REMAINEDTIME(cltMsg* pclMsg, SI32 id);	// [기형] 경기 시작 하기 까지 남은 시간 
	void DoMsg_GAMEMSG_REQUEST_BATTLEROYAL_MOVETOSTADIUM(cltMsg* pclMsg, SI32 id);	// [기형] 경기장으로 이동 

	//=============================================================================================
	// [기형] 발렌타인이벤트
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_VALENTINEEVENT_GIVE_PRESENT_TONPC(cltMsg* pclMsg, SI32 id);	// [기형] NPC에게 선물 주기
	void DoMsg_GAMEMSG_REQUEST_VALENTINEEVENT_ISHOLDING(cltMsg* pclMsg, SI32 id);	// [기형] 이벤트 중인지

	//---------------------------------------------------------------------------------------------
	// [ 지연] 초보자 파티 퀘스트 시스템 (파티매칭)
	void DoMsg_GAMEMSG_REQUEST_PARTYMATCHING_LEVEL(cltMsg* pclMsg, SI32 id);		// [지연] 레벨선택창 - 수행 가능 레벨 체크 요청.
	void DoMsg_GAMEMSG_REQUEST_APPYLYTOPARTYJOIN( cltMsg* pclMsg, SI32 id);			// [지연] 파티조회창 - 파티 가입 신청 요청.	
	void DoMsg_GAMEMSG_REQUEST_CREATEMYPARTY( cltMsg* pclMsg, SI32 id);				// [지연] 파티조회창 - 내 파티 만들기 요청.
	void DoMsg_GAMEMSG_REQUEST_CREATECOMPLETEPARTY( cltMsg* pclMsg, SI32 id);		// [지연] 파티만들기창 - 필요조건 입력 후 파티 만들겠다고 요청.
	void DoMsg_GAMEMSG_REQUEST_REMOVEPARTYMATCHING( cltMsg* pclMsg, SI32 id );		// [지연] 파티조회창 - 내가 등록한 파티매칭 삭제
	void DoMsg_GAMEMSG_REQUEST_GETPARTYMEMBERNAMELIST( cltMsg* pclMsg, SI32 id );	// [지연] 파티조회창 - 선택한 파티매칭의 파티원 이름 정보 요청



	void DoMsg_GAMEMSG_REQUEST_PARTY_QUEST_CHECKSTART( cltMsg* pclMsg, SI32 id);	// [종호] 파티퀘스트 시작시 조건 체크	
	void DoMsg_GAMEMSG_REQUEST_PARTY_QUEST_START( cltMsg* pclMsg, SI32 id);			// [종호] 파티퀘스트 시작

	//[진성] 기부 정보 얻기.
	void DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_ITEM_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_ITEM_INPUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_CONTRIBUTION_MAKE_NPC(cltMsg* pclMsg, SI32 id);
	
	void DoMsg_GAMEMSG_REQUEST_GIANTRACCOON_RESURRECT_EVENT_ITEM_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_NPOTRACCOON_ITEM_GET(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_SEARCHMANUFACTURE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_REQUEST_GORACCOONWORLD(cltMsg* pclMsg, SI32 id);		//[성웅] 라쿤월드로 이동 하기 
	// [성웅] 레벨 보상 상자에서 아이템을 준다 
	void DoMsg_GAMEMSG_REQUEST_OPEN_LEVEL_LIMIT_BOX(cltMsg* pclMsg,SI32 id);	//[성웅] 레벨 제한 상자 오픈 

	void DoMsg_GAMEMSG_REQUEST_JustItemUse(cltMsg* pclMsg,SI32 id);	//[성웅] // 죽었던 말던 어떤상황이던 아이템을 줄인다

	//[진성] 발렌타인 데이 과금 아이템.
	void DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_ITEM_PACKING(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_BUY_ITEM(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_QUESTION(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_REQUEST_VALENTINEDAY_CONFESSION_ANSWER(cltMsg* pclMsg,SI32 id);
	
	


//---------------------------------------------------------------------------------------------
//
//									Response Function Area
//
//---------------------------------------------------------------------------------------------
	// ServerDB-Guild
	void DoMsg_DBMSG_RESPONSE_GUILD_CREATEGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_DESTROYGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_JOINGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CONFIRMJOINGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_LEAVEGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CHANGEVILLAGE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CHANGEGUILDPOINT(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_GETGUILDINFO(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_SETSECONDMASTER(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_GIVEGUILDMONEY(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_GIVE_FEE(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_CHANGESTATUS(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_SETSTRFEERATE(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_LEVELUP(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_SETMARK(sPacketHeader* pPacket);
	void DOMSG_DBMSG_RESPONSE_GUILD_CHANGENAME(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CHANGEMASTER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CLOSEGUILD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDWAR_ADDRESERVE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDWAR_RESERVELIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_APPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_SAVESTATUS(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_LOADSTATUS(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_TRANSFEROWNERSHIP(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_CHECKDESTROY_MONTH(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILD_NOTCONNECT_GUILDMASTER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_REGLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_GETTOP10(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_RESULT(sPacketHeader* pPacket);
	
//	void DoMsg_DBMSG_RESPONSE_GUILD_CHANGEVILLAGELIST(sPacketHeader* pPacket);
	

	//Server-DB.cpp

	// 게임 시간을 얻어온다. 
	// 게임 시간을 얻어오기 전까지는 서버는 동작하지 않는다. 
	bool RequestGameTimeFromDB();

	// 각 지도으 타입을 불러온다. 
	bool RequestMapTypeFromDB();


	// 서버의 최초 작동시 DB로부터 값을 얻어온다. 
	bool InitializeDB1();
	bool InitializeDB2();
	void DoDBMsg( sPacketHeader* pPacket );
	void DoDBMsg2( sPacketHeader* pPacket );
	void DoLogMsg( sPacketHeader* pPacket );

	// Person
	// 스승 등록 신청 요청. 
	void DoMsg_GAMEMSG_REQUEST_APPLYFATHER(cltMsg* pclMsg, SI32 id);
	// 스승 등록 신청자 명단 요청. 
	void DoMsg_GAMEMSG_REQUEST_FATHERAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	// 제자 리스트 요청 
	void DoMsg_GAMEMSG_REQUEST_UPDATECHILDLIST(cltMsg* pclMsg, SI32 id);	
	// 스승이 제자를 짜른다
	void DoMsg_GAMEMSG_REQUEST_RENOUNCEPUPIL(cltMsg* pclMsg, SI32 id);	
	// IP를 보내줄 것을 요청 
	void DoMsg_GAMEMSG_REQUEST_PERSON_IP(cltMsg* pclMsg, SI32 id);
	// 캐릭터의 상태 변경 요청을 처리한다. 
	void DoMsg_GAMEMSG_REQUEST_CHANGESTATUSINFO(cltMsg* pclMsg, SI32 id);


	// 스승 설정/해제 
	void DoMsg_DBMSG_RESPONSE_SETFATHER(sPacketHeader* pPacket);
	// Person 간단 정보 요청. 
	void DoMsg_DBMSG_RESPONSE_PERSONSIMPLEINFO(sPacketHeader* pPacket);
	// 제자 양성 성공수 증가  통보. 
	void DoMsg_DBMSG_RESPONSE_INCREASECHILDNUMBER(sPacketHeader* pPacket);
	// 제자 리스트 통보. 
	void DoMsg_DBMSG_RESPONSE_GETCHILDLIST(sPacketHeader* pPacket);	
	// 스테이터스 초기화 
	void DoMsg_DBMSG_RESPONSE_INITSTATUS(sPacketHeader* pPacket);
	// 이름 변경
	void DoMsg_DBMSG_RESPONSE_CHANGENAME(sPacketHeader* pPacket);
	// 종류 변경
	void DoMsg_DBMSG_RESPONSE_CHANGE_CHARKIND(sPacketHeader* pPacket);

	// 현금액이 변경된다. 
	void DoMsg_DBMSG_RESPONSE_CHANGEMONEY(sPacketHeader* pPacket);
	// 케릭터 게임옵션을 셋팅 한다.
	void DoMsg_DBMSG_RESPONSE_CHAR_OPTION(sPacketHeader* pPacket);
	// 케릭터 게임옵션을 가지고온다
	void DoMsg_DBMSG_RESPONSE_CHAR_GET_OPTION(sPacketHeader* pPacket);	
	// 경험치가  변경된다. 
	void DoMsg_DBMSG_RESPONSE_CHANGEEXP(sPacketHeader* pPacket);	
	// 명성 경험치가 변경된다.
	void DoMsg_DBMSG_RESPONSE_CHANGEFAMEEXP(sPacketHeader* pPacket);	
	// 돈을 옮긴다.
	void DoMsg_DBMSG_RESPONSE_MOVEMONEY(sPacketHeader* pPacket);	
	// 퀘스트 저장. 
	void DoMsg_DBMSG_RESPONSE_SETQUESTINFO(sPacketHeader* pPacket);
	// 퀘스트 저장. 
	void DoMsg_DBMSG_RESPONSE_SETSPECIALQUESTINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETPERSONNAMINGINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETPERSONCURRENTNAMING(sPacketHeader* pPacket);

	//KHY - 0822 - 해피빈 퀘스트 추가. 퀘스트 정보 저장 및 시작 정보를 얻어 온다.
	void DoMsg_DBMSG_RESPONSE_GETHAPPYBEANQUESTINFO(sPacketHeader* pPacket);

	//KHY - 0906 - 게임방 이벤트 쿠폰.
	void DoMsg_DBMSG_RESPONSE_GETEVENTCOUPONINFO(sPacketHeader* pPacket);
	////옥션이벤트 [2007.09.10 손성웅]
	//void DoMsg_DBMSG_RESPONSE_GETAUCTIONEVENTCOUPONINFO(sPacketHeader* pPacket);

	// 위치, 생명력, 마법력 정보 저장
	void DoMsg_DBMSG_RESPONSE_SETPOSLIFEMANA(sPacketHeader* pPacket);
	// 건강 정보 저장. 
	void DoMsg_DBMSG_RESPONSE_SETHEALTH(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_PERSONLISTINFOGET(sPacketHeader* pPacket);
	// 새로운 Person이 만들어졌음을 알려주는 메시지 프로시져.
	void DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOCREATE(sPacketHeader* pPacket);
	// 사용자가 Person을 선택했을때 DB로 Person의 전체 정보를 요청하는데 이때 수령한 전체 정보를 처리하는 함수. 
	void DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOGETBYIDNUM(sPacketHeader* pPacket);

	//cyj 사용자의 아이템 정보 받아오는 함수
	void DoMsg_DBMSG_RESPONSE_PERSONITEMINFOGET(sPacketHeader* pPacket);

	//cyj 아이템의 제작자 명을 받아오는 함수
	void DoMsg_DBMSG_RESPONSE_ITEMMAKERNAME(sPacketHeader* pPacket);

	// 영진 - 추천인 포인트가 업데이트 되었음을 알림.
	void DOMSG_DBMSG_RESPONSE_CHANGERECOMMENDPOINT(sPacketHeader* pPacket);
	// 영진 - 추천인 포인트를 받아옴.
	void DoMsg_DBMSG_RESPONSE_GETRECOMMANDPOINT(sPacketHeader* pPacket);

	// 영진 - 기간연장 아이템을 사용했음
	void DoMsg_DBMSG_RESPONSE_ENCHANT_USEDAYPLUS(sPacketHeader* pPacket);

	// [영훈] 아이템 인첸트 속성 변경 - 2008.02.20
	void DoMsg_DBMSG_RESPONSE_CHANGEENCHANT(sPacketHeader* pPacket);

	// Person삭제 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_PERSONTOTALINFODELETE(sPacketHeader* pPacket);
	// Person삭제 요청대기에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOPREDELETE(sPacketHeader* pPacket);
	// Person 정보 저장요청에 대한 응답. (별다른 조치를 취할 것이 없다.)
	void DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOSET(sPacketHeader* pPacket);
	// Person의 주식 계좌 정보 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_PERSONSTOCKACCOUNTGET(sPacketHeader* pPacket);
	// Person의 스킬 레벨을 높인다. 
	void DoMsg_DBMSG_RESPONSE_INCREASESKILL(sPacketHeader* pPacket);
	// Person의 스킬 레벨을 낮춘다.
	void DoMsg_DBMSG_RESPONSE_DECREASESKILL(sPacketHeader* pPacket);
	// Person의 전투향상술 스킬 레벨을 올린다.
	void DoMsg_DBMSG_RESPONSE_SET_WEAPONSKILL(sPacketHeader* pPacket);
	// Person의 스킬경험치를 설정한다.
	void DoMsg_DBMSG_RESPONSE_SETSKILLEXP(sPacketHeader* pPacket);
	// 운영자 명령어로 Person의 스킬을 높인다.
	void DoMsg_DBMSG_RESPONSE_INCREASEMASTERSKILL(sPacketHeader* pPacket);
	// Person의 레벨 정보를 설정한다. 
	void DoMsg_DBMSG_RESPONSE_SETLEVELINFO(sPacketHeader* pPacket);
	// Person의 군주 계정을 받아옮
	void DoMsg_DBMSG_RESPONSE_USERID(sPacketHeader* pPacket);	
	// 학교등록  요청. 
	//	void DoMsg_GAMEMSG_REQUEST_SETSCHOOL(cltMsg* pclMsg, SI32 id);	
	// 스승 등록  요청. 
	void DoMsg_GAMEMSG_REQUEST_SETFATHER(cltMsg* pclMsg, SI32 id);
	// 접속중인 학교 사용자 수요청. 
	void DoMsg_GAMEMSG_REQUEST_SCHOOLUSERNUM(cltMsg* pclMsg, SI32 id);
	// 개인 상세 정보 요청. 
	void DoMsg_GAMEMSG_REQUEST_PERSONPRIVATEINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REQUEST_PERSONPRIVATEINFOBYPERSONID(cltMsg* pclMsg, SI32 id);
	// 추천인 등록
	void DoMsg_DBMSG_RESPONSE_RECOMMENDPERSON(sPacketHeader* pPacket);
	// 말 설정 
	void DoMsg_DBMSG_RESPONSE_SETHORSE(sPacketHeader* pPacket);
	// 말 타고 내리기  설정 
	void DoMsg_DBMSG_RESPONSE_SETRIDEHORSE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETAUTOFOOD(sPacketHeader* pPacket);

	// 말 시세 설정
	void DoMsg_DBMSG_RESPONSE_SETCURRENTHORSEPRICE(sPacketHeader* pPacket);
	// 말 시세 얻어옮
	void DoMsg_DBMSG_RESPONSE_GETCURRENTHORSEPRICE(sPacketHeader* pPacket);
	// 말 이름 변경
	void DoMsg_DBMSG_RESPONSE_HORSENAMECHANGE(sPacketHeader* pPacket);
	// 개인 범죄 기록 저장 
	void DoMsg_DBMSG_RESPONSE_GUILTYINFORECORD(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_PRIZE_GAMEEVENT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_COUNTATTACK_INFO(sPacketHeader* pPacket);
	// 캐릭터 인사말
	void DoMsg_DBMSG_RESPONSE_SETGREETING(sPacketHeader* pPacket);

	// Server&DB-Letter.cpp
	// 쪽지를 저장한다. 
	void DoMsg_DBMSG_RESPONSE_PUSHLETTER(sPacketHeader* pPacket);
	// 쪽지를 꺼낸다.
	void DoMsg_DBMSG_RESPONSE_POPLETTER(sPacketHeader* pPacket);

	// 쪽지를 저장한다. 
	void DoMsg_DBMSG_RESPONSE_PUSHACCOUNTLETTER(sPacketHeader* pPacket);
	// 쪽지를 꺼낸다.
	void DoMsg_DBMSG_RESPONSE_POPACCOUNTLETTER(sPacketHeader* pPacket);

	//Server&DB-Structure.cpp

	void DoMsg_DBMSG_RESPONSE_REPAIRSTRUCTURE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETSTRREPAIRITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETSTRDURA(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_CHANGESTRDURA(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_CHANGESTRMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_CHANGESTRBASEMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETSTRREPAIRAMOUNT(sPacketHeader* pPacket);

	// 건물 정보를 설정하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_SETSTRINFO(sPacketHeader* pPacket);
	// 건물 정보를 얻어오라는  요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETSTRINFO(sPacketHeader* pPacket);
	// 건물 행수정보를 얻어온다.
	void DoMsg_DBMSG_RESPONSE_CHIEFINFO(sPacketHeader* pPacket);

	// 건물 세금 설정 응답
	void DoMsg_DBMSG_RESPONSE_SETTAX( sPacketHeader* pPacket );

	// 건물 세금 납부 하기 응답 
	void DoMsg_DBMSG_RESPONSE_PAYTAX( sPacketHeader* pPacket );

	// 배당 응답 
	void DoMsg_DBMSG_RESPONSE_PAYDIVIDEND( sPacketHeader* pPacket );

	// 자동배당 응답 
	void DoMsg_DBMSG_RESPONSE_AUTOPAYDIVIDEND( sPacketHeader* pPacket );

	// 수수료 납부에 대한 응답
	void DoMsg_DBMSG_RESPONSE_PAYFEE( sPacketHeader* pPacket );

	// 마을 NPC 고용 요청에 대한 응답
	void DoMsg_DBMSG_RESPONSE_HIRENPC( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_BUILDSTRUCTURE( sPacketHeader *pPacket );
	void DoMsg_DBMSG_RESPONSE_DESTROYSTRUCTURE( sPacketHeader *pPacket );


	void DoMsg_DBMSG_RESPONSE_MANYRESIDENTOUT( sPacketHeader *pPacket );

	// Server&DB-Bank.cpp
	// 전장에 돈을 입금하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_INPUTMONEYTOBANK(sPacketHeader* pPacket);
	// 전장에서 돈을 출금하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_OUTPUTMONEYFROMBANK(sPacketHeader* pPacket);
	// 전장 수수료 정보를 설정하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_SETBANKFEE(sPacketHeader* pPacket);
	// 계좌를 개설할 것. 
	void DoMsg_DBMSG_RESPONSE_CREATEBANKACCOUNT(sPacketHeader* pPacket);
	// 계좌를 해지할 것. 
	void DoMsg_DBMSG_RESPONSE_CLOSEBANKACCOUNT(sPacketHeader* pPacket);


	// Server&DB-Stock.cpp
	// 주식 주문을 DB로 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETMANYSTOCKORDER(sPacketHeader* pPacket);
	// 주식 거래의 결과를 얻어온 것을 처리한다. 
	void DoMsg_DBMSG_RESPONSE_TRADESTOCK(sPacketHeader* pPacket);
	// 주문 정보를 DB에 추가하라는 요청에 대한 응답을 처리한다.
	void DoMsg_DBMSG_RESPONSE_ORDERSTOCK(sPacketHeader* pPacket);
	// 주식 주문을 취소하라는 요청에 대한 응답
	void DoMsg_DBMSG_RESPONSE_CANCELSTOCKORDER(sPacketHeader* pPacket);
	// 주식 가격 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYSTOCKPRICE(sPacketHeader* pPacket);
	// 주식 거래 수수료 정보를 설정하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_SETSTOCKFEE(sPacketHeader* pPacket);
	// 증자 주식사기 
	void DoMsg_DBMSG_RESPONSE_BUYBIDSTOCK(sPacketHeader* pPacket);
	// 내 주식 업뎃
	void DoMsg_DBMSG_RESPONSE_MYSTOCKINFO(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_GETSTOCKBIDVILLAGELIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETSTOCKBIDVILLAGE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_BIDSTOCK(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETSTOCKBIDDERLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_COMPLETESTOCKBIDVILLAGE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_STOCKDISTRIBUTIONORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MAXSTOCKDISTRIBUTIONNUM(sPacketHeader* pPacket);
	/*
	void DoMsg_DBMSG_RESPONSE_GETSTOCKOFFERINGINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETSTOCKOFFERING(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ENDSTOCKOFFERING(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_STOCKOFFERINGORDER(sPacketHeader* pPacket);
	*/

	//#if defined(_CITYHALL_BANKRUPTCY)
	void DoMsg_DBMSG_RESPONSE_RETURNSTOCKBIDVILLAGE(sPacketHeader* pPacket);	// 마을 파산시 증자 입찰 보상

	void DoMsg_DBMSG_RESPONSE_NOTIFY_CITYHALL_BANKRUPTCY(sPacketHeader* pPacket);	// 마을 파산시 특정 주주에게 letter로 알림 처리
	//#endif

	// 사복시
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_GETMANYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_ORDERSELL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_MYACCOUNT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_OUTHORSE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_BUYHORSE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HORSEMARKET_SETFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REDUCEHORSEORDERPRICE(sPacketHeader* pPacket);


	// 거간
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_GETMANYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_ORDERSELL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_MYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_CANCELORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_DELORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_BUY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REALESTATEMARKET_SETFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REDUCEREALESTATEORDERPRICE(sPacketHeader* pPacket);



	// 장예원
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_GETMANYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_ORDERSELL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_MYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_CANCELORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_BUY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONMARKET_SETFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REDUCESUMMONORDERPRICE(sPacketHeader* pPacket);


	// 비변사
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_GETMANYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_ORDERSELL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_MYORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_BUY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_SETFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REDUCESUMMONHEROORDERPRICE(sPacketHeader* pPacket);



	// 농장
	void DoMsg_DBMSG_RESPONSE_MANYLANDUNIT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_CHANGECONTRACT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_RENTAPPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_CANCELRENTAPPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_APPLYERLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_DELAPPLYER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_RENT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_PAYRENTFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_LAND_RECALL(sPacketHeader* pPacket);

	// 농장
	void DoMsg_DBMSG_RESPONSE_MANYMINEUNIT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_CHANGECONTRACT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_RENTAPPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_CANCELRENTAPPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_APPLYERLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_DELAPPLYER(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_RENT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_PAYRENTFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MINE_RECALL(sPacketHeader* pPacket);


	// 내자시
	void DoMsg_DBMSG_RESPONSE_FEAST_RESERVE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_OPEN(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_CLOSE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_SETCOST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_ADDPRESENTLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_DELPRESENTLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_STGITEMIN(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_STGITEMOUT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FEAST_GIVEITEM(sPacketHeader* pPacket);

	//#if defined(_LEON_FEAST_EFFECT)
	void DoMsg_DBMSG_RESPONSE_FEAST_USEEFFECT(sPacketHeader* pPacket);
	//#endif
	//[진성] 파티홀.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_CLOSE(sPacketHeader* pPacket);						// 파티장 종료. 
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_ADD_PRESENT(sPacketHeader* pPacket);				// 파티장 선물 추가.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_RESERVATION(sPacketHeader* pPacket);				// 파티장 예약.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_ADD_DECORATION(sPacketHeader* pPacket);				// 장식물 추가.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_RESERVELIST(sPacketHeader* pPacket);				// 파티홀 예약 정보 리스트.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_DECORATION_LIST(sPacketHeader* pPacket);			// 파티홀 장식물 리스트.
	void DoMsg_DBMSG_RESPONSE_PARTYHALL_GIVEITEM(sPacketHeader* pPacket);					// 파티홀 장식물 리스트.
	
	

	// 우체국 
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_MASTERSENDITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_SENDMSG(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVITEMLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_RECVMSGLIST(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_DELMSG(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_SETFEE(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_POSTOFFICE_GETRECORD(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_CHAR_NAME_CHECK(sPacketHeader* pPacket);		//[진성] 유저이름으로 DB에서 존재여부와 personID 얻기 => 2008-8-4
	void DoMsg_DBMSG_RESPONSE_FRIEND_INFO(sPacketHeader* pPacket);			//[진성] 친구 정보 얻기. => 2008-8-6
	void DoMsg_DBMSG_RESPONSE_GUILD_INFO(sPacketHeader* pPacket);			//[진성] 길드 정보 얻기. => 2008-8-6
	void DoMsg_DBMSG_RESPONSE_VILLAGE_RESIDENT_INFO(sPacketHeader* pPacket);//[진성] 주민 정보 얻기. => 2008-8-6

	//-------------------------------------
	// 시전 연장 하지 못 한 유저의 아이템 찾기
	//-------------------------------------	
	void DoMsg_DBMSG_RESPONSE_GET_SAVEUSERITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GET_SAVEUSERITEMLIST(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_GET_SAVEUSERHORSE(sPacketHeader* pPacket);

	// 소환수 
	void DoMsg_DBMSG_RESPONSE_SETSUMMON(sPacketHeader* pPacket);	// 소환수 설정 
	void DoMsg_DBMSG_RESPONSE_SUMMONREVIVE(sPacketHeader* pPacket);	// 소환수 부활 
	void DoMsg_DBMSG_RESPONSE_SUMMONEVOLVE(sPacketHeader* pPacket);	// 소환수 진화 
	void DoMsg_DBMSG_RESPONSE_SUMMONSCROLL(sPacketHeader* pPacket);	// 소환수 강화 
	void DoMsg_DBMSG_RESPONSE_SUMMON_STATUSINIT(sPacketHeader* pPacket); // 소환수 상태 초기화
	void DoMsg_DBMSG_RESPONSE_SUMMON_SETAUTOHEAL(sPacketHeader* pPacket); // 소환수 자동 회복 설정
	void DoMsg_DBMSG_RESPONSE_SUMMONTRADE(sPacketHeader* pPacket); // 소환수 NPC에게 팔기 응답
	void DoMsg_DBMSG_RESPONSE_SUMMON_SLOTIN(sPacketHeader* pPacket); // 소환수를 BAse에서 Slot에 넣는다.
	void DoMsg_DBMSG_RESPONSE_SUMMON_SLOTOUT(sPacketHeader* pPacket); // Slot의 소환수를 빼낸다,.
	
	

	// Server&DB-Item.cpp
	// 인벤에서 아이템을 생산하라는 요청에 응답. 
	void DoMsg_DBMSG_RESPONSE_MAKEITEMINV(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ENCHANTITEMINV(sPacketHeader* pPacket);
	// 아이템을 Person의 ItemPos로 넣으라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_INPUTITEMTOPERSON(sPacketHeader* pPacket);
	// 아이템을 Person의 ItemPos에서 빼라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_OUTPUTITEMFROMPERSON(sPacketHeader* pPacket);
	// 아이템을 Person의 한 위치에서 다른 위치로 이동하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MOVEITEM(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_CHANGEITEM(sPacketHeader* pPacket);
	// 아이템을 Person에서 다른 Person에게 옮기라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MOVEITEMPERSON(sPacketHeader* pPacket);
	// 아이템을 사용하라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_USEITEM(sPacketHeader* pPacket);
	// 아이템 내구도 소모에 대한 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_WASTEDURABILITY(sPacketHeader* pPacket);	
	// 아이템 수리에 대한 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_INVREPAIR(sPacketHeader* pPacket);
	// 아이템을 버린다. 
	void DoMsg_DBMSG_RESPONSE_DROPITEM(sPacketHeader* pPacket);
	// 부적 부착. . 
	void DoMsg_DBMSG_RESPONSE_ENCHANT(sPacketHeader* pPacket);
	// 아이템 설정. (다목적) 
	void DoMsg_DBMSG_RESPONSE_SETITEM(sPacketHeader* pPacket);
	// 아이템 업그레이드 ( 일반 -> 호품 -> 명품 -> 신기 ) - 혹부리 영감 이용권을 사용해서.
	void DoMsg_DBMSG_RESPONSE_UPGRADEITEM(sPacketHeader* pPacket);
	// 아이템 성능 향상 ( 아이템의 성능을 향상 시킨다. 내구는 깎이면서, 제조 부적을 사용하면서.. )
	void DoMsg_DBMSG_RESPONSE_PERFORMANCEUPGRADEITEM(sPacketHeader* pPacket);

	void DOMSG_DBMSG_RESPONSE_STARTPERSONALSHOP(sPacketHeader* pPacket);	// 개인상점을 시작한다.
	void DOMSG_DBMSG_RESPONSE_PERSONALSHOPBUYITEM(sPacketHeader* pPacket);	// 개인상점에서 물품을 구매한다.
	void DOMSG_DBMSG_RESPONSE_PERSONALSHOPSELLITEM(sPacketHeader* pPacket);	// 개인상점에 물품을 판매한다.

	// 기간제 아이템 삭제
	//	void DoMsg_DBMSG_RESPONSE_DELETE_LIMITDATEITEM(sPacketHeader* pPacket);
	//	void DoMsg_DBMSG_RESPONSE_GETLIMITUSEITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETPREMIUM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_EXCHANGEITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_OPENTREASUREBOX(sPacketHeader* pPacket);

	//[진성] 공간상자와 보물상자 기간제 아이템 삭제.
	void DoMsg_DBMSG_RESPONSE_USE_SPACEBOX_ITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_USE_TREASURE_ITEM(sPacketHeader* pPacket);

	// [영훈] 소환수 장비착용 : 사람에서 소환수로 이동한 아이템 결과
	void DoMsg_DBMSG_RESPONSE_SUMMONPUSHITEM(sPacketHeader* pPacket);
	// [영훈] 소환수 장비착용 : 소환수에서 사람에게로 이동한 아이템 결과
	void DoMsg_DBMSG_RESPONSE_SUMMONPOPITEM(sPacketHeader* pPacket);
	// [영훈] 소환수 장비착용 : 소환수 아이템의 상태가 변경되었다
	void DoMsg_DBMSG_RESPONSE_SUMMONITEM_STATUSCHANGE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_REPAIRITEM(sPacketHeader* pPacket);

	// 수렵장 ( Server&DB-Hunt.cpp )
	void DoMsg_DBMSG_RESPONSE_HUNTMONSTERFIELDLISTADD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTMONSTERFIELDLISTDEL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTSTORAGEMONSTERIN(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTSTORAGEMONSTEROUT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTMONEYCHANGE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTBASEINFOAPPLY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTSTORAGEDECREASEDOLLITEMNUM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTGETENTERFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTPAYSUCCESSMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_HUNTLEFTMONSTERSTGIN(sPacketHeader* pPacket);


	// 친구( Server&DB-Friend.cpp )
	void DoMsg_DBMSG_RESPONSE_FRIENDADD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDDEL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDADDLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDDELLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDMEMO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDMEMOSET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDCHANGEGROUP(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDCHANGEGROUPNAME(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FRIENDALLLIST(sPacketHeader* pPacket);

	// 투표( Server&DB-Vote.cpp )	
	void DoMsg_DBMSG_RESPONSE_VOTEINIT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_VOTEKINGSELECTION(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_VOTEKINGAGGREGATE(sPacketHeader* pPacket);

	// Daily Quest( Server&DB-DailyQuest.cpp )	
	void DoMsg_DBMSG_RESPONSE_DAILYQUESTADD( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUESTDEL( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUESTCHANGE( sPacketHeader* pPacket);


	// Server&DB-Market.cpp
	// 객주 주문을 DB로 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETMANYMARKETORDER(sPacketHeader* pPacket);
	// 주문 정보를 DB에 추가하라는 요청에 대한 응답을 처리한다.
	void DoMsg_DBMSG_RESPONSE_ORDERMARKET(sPacketHeader* pPacket);
	// 주문 가격 인하 요청 응답. 
	void DoMsg_DBMSG_RESPONSE_REDUCEORDERPRICE(sPacketHeader* pPacket);
	// 객주 거래 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_TRADEMARKET(sPacketHeader* pPacket);
	// 객주 계좌 정보 요청에  대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MARKETACCOUNT(sPacketHeader* pPacket);
	// 객주 창고에서 아이템을 꺼내오라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETITEMFROMMARKET(sPacketHeader* pPacket);
	// 아이템 가격 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYITEMPRICE(sPacketHeader* pPacket);
	// 아이템 가격 저장. 
	void DoMsg_DBMSG_RESPONSE_SETITEMPRICE(sPacketHeader* pPacket);
	// 거래 수수료 설정 요청에 대한 응답 
	void DoMsg_DBMSG_RESPONSE_SETMARKETFEE(sPacketHeader* pPacket);


	// Server&DB-Cityhall.cpp
	// 마을 투자자리스트를 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_INVESTLIST(sPacketHeader* pPacket);
	// 입후보 할 것을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_CANDIDATE(sPacketHeader* pPacket);
	// 입후보 취소할 것을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_CANCELCANDIDATE(sPacketHeader* pPacket);
	// 입후보자가 특정 신분이 되도록 할 것을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_BERANK(sPacketHeader* pPacket);
	// 특정 Person의 신분 정보를 얻어오라는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_RANKINFO(sPacketHeader* pPacket);
	// 입후보자 명단을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYCANDIDATEINFO(sPacketHeader* pPacket);
	// 후보자 명단을 삭제할 것을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_DELCANDIDATE(sPacketHeader* pPacket);
	// 신분직에서 사임한다는 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_RESIGN(sPacketHeader* pPacket);
	// 신분정보 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYRANK(sPacketHeader* pPacket);
	// 신분정보 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GET_SYSTEMBUYITEMNOWMONEY(sPacketHeader* pPacket);


	void DoMsg_DBMSG_RESPONSE_PROFITTOCAPITALFUND(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_CHANGECITYHALLPROFITMONEY(sPacketHeader* pPacket);

	// 기부하기
	void DoMsg_DBMSG_RESPONSE_DONATEVILLAGEMONEY(sPacketHeader* pPacket);
	// 기부자 목록보기
	void DoMsg_DBMSG_RESPONSE_DONATEMEMBERLIST(sPacketHeader* pPacket);

	// 만들기 이벤트 재료 넣기
	void DoMsg_DBMSG_RESPONSE_EVENT_INSERTMATERIAL(sPacketHeader* pPacket);
	// 만들기 이벤트 재료 현재 정보
	void DoMsg_DBMSG_RESPONSE_EVENT_MATERIALLIST(sPacketHeader* pPacket);
	// 만들기 이벤트 완성 처리
	void DoMsg_DBMSG_RESPONSE_EVENT_MATERIALCOMPLETE(sPacketHeader* pPacket);

	// 주민 등록 신청에 대한 응답 
	void DoMsg_DBMSG_RESPONSE_APPLYRESIDENT(sPacketHeader* pPacket);

	// 주민 등록 신청자 리스트 응답 
	void DoMsg_DBMSG_RESPONSE_GETRESIDENTAPPLYERLIST(sPacketHeader* pPacket);

	// 주민 리스트 응답 
	void DoMsg_DBMSG_RESPONSE_GETRESIDENTLIST(sPacketHeader* pPacket);

	// 주민 등록 허가 응답 
	void DoMsg_DBMSG_RESPONSE_BERESIDENT(sPacketHeader* pPacket);

	// 주민 등록 신청 취소 및 주민 등록 해지 응답 
	void DoMsg_DBMSG_RESPONSE_CANCELRESIDENT(sPacketHeader* pPacket);

	// 주민 상세 정보 응답 
	void DoMsg_DBMSG_RESPONSE_GETRESIDENTINFO(sPacketHeader* pPacket);

	// 주민 점수 부여 응답 
	void DoMsg_DBMSG_RESPONSE_SETRESIDENTSCORE(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_RESIDENTSHARE( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_SETVILLAGEMARK( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETVILLAGEMAP( sPacketHeader* pPacket );

	// 인스턴스 던전 이익금
	void DoMsg_DBMSG_RESPONSE_SETINSTANCEPROFIT_VILLAGE(sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_CHANGE_INSTANCEPROFIT(sPacketHeader* pPacket );

	// Server&DB-House.cpp
	// 시전 계약 조건을 설정한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_CHANGEHOUSECONTRACT(sPacketHeader* pPacket);
	// 단위 시전 정보를 얻어올 것을 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYHOUSEUNIT(sPacketHeader* pPacket);
	// 임차 희망자 명단 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETHOUSEAPPLYERLIST(sPacketHeader* pPacket);
	// 임차 신청을 제출한 것에 대한 응답.
	void DoMsg_DBMSG_RESPONSE_HOUSERENTAPPLY(sPacketHeader* pPacket);
	// 임차 신청 취소 요청에 대한 응답.
	void DoMsg_DBMSG_RESPONSE_HOUSERENTAPPLYCANCEL(sPacketHeader* pPacket);
	// 임대 허가를 제출한 것에 대한 응답.
	void DoMsg_DBMSG_RESPONSE_HOUSERENT(sPacketHeader* pPacket);
	// 아이템을 인벤에서 창고로 옮기는것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MOVEITEMINVTOSTG(sPacketHeader* pPacket);
	// 아이템을 창고에서 인벤으로 옮기는것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MOVEITEMSTGTOINV(sPacketHeader* pPacket);
	// 창고의 특정 위치에 있는 아이템 정보를 요청한 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_GETHOUSESTGPOS(sPacketHeader* pPacket);
	// 임차 희망자를 모두 삭제한다. 
	void DoMsg_DBMSG_RESPONSE_DELHOUSEAPPLYER(sPacketHeader* pPacket);
	// 임대료를 지불한다. 
	void DoMsg_DBMSG_RESPONSE_PAYRENTFEE(sPacketHeader* pPacket);
	// 시전을 회수한다 
	void DoMsg_DBMSG_RESPONSE_RECALLHOUSE(sPacketHeader* pPacket);
	// 시전에 마구간 생성
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITSTABLECREATE(sPacketHeader* pPacket);
	// 시전에 말을 넣는다.
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEIN(sPacketHeader* pPacket);
	// 시전에서 말을 뺀다.
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEOUT(sPacketHeader* pPacket);
	// 시전에 말 정보를 업데이트 한다.
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEINFOSET(sPacketHeader* pPacket);
	// 시전에 있는 말이 먹이를 먹는다.
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITHORSEEATFOOD(sPacketHeader* pPacket);
	// 시전에 고물상 생성
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE(sPacketHeader* pPacket);
	// 고물에서 아이템 뽑아냄
	void DoMsg_DBMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS(sPacketHeader* pPacket);
	// 창고 이름 변경
	void DoMsg_DBMSG_RESPONSE_CHANGEHOUSEUNITNAME(sPacketHeader* pPacket);

	//----------------------------------------------------------------------
	// ItemMall , Server&DB-ItemMall.cpp
	//---------------------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_ITEMMALLEXISTENCE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALLITEMLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALLGETITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWSCHATADDMISSION(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NOLIMITTICKET_WARP(sPacketHeader* pPacket);
	
	// 목표 지역 으로 바로 이동 한다 
	void DoMsg_DBMSG_RESPONSE_WARP_TARGETWORLD(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_GET_SEL_PGLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GET_ITEMMALL_ITEMINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GET_MYCASH_INFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALL_BUYITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALL_GIFTITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_CHECK_ACCOUNTID(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SET_BAGITEM(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SET_CHANGEHERO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALL_BUYITEM_FORXNOTI(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ITEMMALL_GIFTITEM_FORXNOTI(sPacketHeader* pPacket);
	//void DoMsg_DBMSG_RESPONSE_GET_PRESENT_ITEMDATA(sPacketHeader* pPacket);
	//--------------------------------------------------------------------
	// 생산, Server&DB-Agriculture.cpp
	//--------------------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_AGRICULTUREGETPRODUCTS(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_AGRICULTUREPAYMONEYFORRENTLAND(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETAGRICULTURELEASEINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_EXPIREDAGRICULTERLEASE(sPacketHeader* pPacket);

	//--------------------------------------------------------------------
	// 낚시, Server&DB-Fishing.cpp
	//--------------------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_FISHINGGETMARINEPRODUCTS(sPacketHeader* pPacket);




	// Server&DB-NPC.cpp
	// NPC에게 판매할 것을 요청하는 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_SELLNPC(sPacketHeader* pPacket);
	// NPC에게서 구입할 것을 요청하는 것에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_BUYNPC(sPacketHeader* pPacket);

	// Server&DB-Village.cpp
	// 마을 건물 정보 요청. 
	void DoMsg_DBMSG_RESPONSE_GETSTRUCTUREINFO(sPacketHeader* pPacket);


	//-----------------------------------------------------------
	// 마을 정보  ( 선전포고 마을, 
	//-----------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_GETVILLAGEINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DECLAREWAR( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_CANCELDECLAREWAR( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_SETPARENTVILLAGE( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_SETSYMBOLSTATUS( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETCASTLESTATUS( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_SETVILLAGEWARLOSS( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_SETVILLAGELEVEL( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_VILLAGERESIDENTSVISITORSINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ADD_VILLAGE_SCORE(sPacketHeader* pPacket);
	
	
	void DoMsg_DBMSG_RESPONSE_SETVILLAGEWAR_RESULT(sPacketHeader* pPacket);

	//-----------------------------------------------------------
	// 주주총회
	//-----------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_INFO( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_CHIEF( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_VOTE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_CLOSE_CHIEF( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_SETVOTERSTATE( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF( sPacketHeader* pPacket );		//[추가 : 황진성 2008. 2. 21 => 주주총회 발동 설정 마을 관련 처리함수 DB->S ]
	void DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF_LIST( sPacketHeader* pPacket );	//[추가 : 황진성 2008. 4. 11 => 주주총회 발동 설정 리스트 얻기 DB->S ]
	//-----------------------------------------------------------
	// 주식 공모
	//-----------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_GETSTOCKOFFERINGINFO( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETSTOCKOFFERING( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_ENDSTOCKOFFERING( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_STOCKOFFERINGORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GETSTOCKOFFERINGORDERLIST( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_EXECUTESTOCKOFFERINGORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_RESETSTOCKOFFERINGORDERLIST( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_EXCHANGESTOCK( sPacketHeader* pPacket );
	
	void DoMsg_DBMSG_RESPONSE_GIVESTOCK( sPacketHeader* pPacket );

	//#if defined(_CITYHALL_BANKRUPTCY)
	//-----------------------------------------------------------
	// 상단 파산 관련
	//-----------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_CITYHALL_STATE_CHANGED( sPacketHeader* pPacket );

	////마을이 파산하여 초기화 한다.
	//// 파산 후 마을 여러 정보 초기화는 서버 점검시 수작업으로 한다.
	//void DoMsg_DBMSG_RESPONSE_VILLAGE_INITIALIZATION( sPacketHeader* pPacket );
	//#endif

	// Server&DB-RealEstate.cpp
	// 부동산 계약을 연장한다. 
	void DoMsg_DBMSG_RESPONSE_EXTENDCONTRACT(sPacketHeader* pPacket);

	//---------------------------------------------
	// Server&DB-System.cpp
	//---------------------------------------------
	// 게임시작에 대한 정보를 얻어온다.
	void DoMsg_DBMSG_RESPONSE_STARTGAME(sPacketHeader* pPacket);
	// 시간을 얻어오라는 요청에 대한 응답
	void DoMsg_DBMSG_RESPONSE_GETTIME(sPacketHeader* pPacket);
	// 시간을 저장하라는  요청에 대한 응답
	void DoMsg_DBMSG_RESPONSE_SETTIME(sPacketHeader* pPacket);

	// 특정 레벨 범위 사용자에게 역참으로 물품 보내기
	void DoMsg_DBMSG_RESPONSE_MASTERGIVEITEM(sPacketHeader *pPacket);

	void DoMsg_DBMSG_RESPONSE_GIVEITEM(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_DBSYSTEMINFO(sPacketHeader *pPacket);

	void DoMsg_DBMSG_RESPONSE_DBSTARTINFO(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_DBINITCOMPLETE(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SETFOURLEAF(sPacketHeader *pPacket);
	

	void DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_SAVEINVENTORY(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_MOVEITEMSTGTOINV(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_INVENTORYLIST(sPacketHeader *pPacket);

	// 육조 
	void DoMsg_DBMSG_RESPONSE_GETBYUNGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETGONGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETHYUNGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETHOZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETIZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETYEZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETGOONZUINFO(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_SETBYUNGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETBYUNGZOINFO_VILLAGEWARTIME(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETGONGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETHYUNGZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETHOZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETIZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETYEZOINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETGOONZUINFO(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_SETGOONZUFINANCES(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_GETANGELAPPLYERLIST(sPacketHeader* pPacket); // 수호천사 신청자 목록
	void DoMsg_DBMSG_RESPONSE_GETANGELLIST(sPacketHeader* pPacket); // 수호천사 목록
	void DoMsg_DBMSG_RESPONSE_SETANGELAPPLYER(sPacketHeader* pPacket); // 수호천사 신청
	void DoMsg_DBMSG_RESPONSE_SETANGEL(sPacketHeader* pPacket); // 수호천사 임명

	void DoMsg_DBMSG_RESPONSE_GETPOLICELIST(sPacketHeader* pPacket);	// 포교 목록
	void DoMsg_DBMSG_RESPONSE_SETPOLICE(sPacketHeader* pPacket);		// 포교 임명

	void DoMsg_DBMSG_RESPONSE_GETCRIMINALLIST(sPacketHeader* pPacket);	// 현행범 목록
	void DoMsg_DBMSG_RESPONSE_SETCRIMINAL(sPacketHeader* pPacket);		// 현행범 고발

	void DoMsg_DBMSG_RESPONSE_GETCHANGEDNAME(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_GETDAILYNEWCHAR(sPacketHeader* pPacket);


	void DoMsg_DBMSG_RESPONSE_GETWARRANKINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETWARRANKINFO(sPacketHeader* pPacket);

	// 사용자 로그인 허용/제한 응답. 
	void DoMsg_DBMSG_RESPONSE_BLOCK(sPacketHeader* pPacket);
	// 특정 사용자 로그인 허용/제한 응답. 
	void DoMsg_DBMSG_RESPONSE_BLOCKUSER(sPacketHeader* pPacket);
	// 사용자 간단 정보 응답. 
	void DoMsg_DBMSG_RESPONSE_USERCHARINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_USERCHARITEMINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_USERCHARITEMINFO2(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_ACCOUNTINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ACCOUNTINFOFORHANXNOTI(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_CHARINFOFORHANXNOTI(sPacketHeader* pPacket);
	
	// 맵타입 통보. 
	void DoMsg_DBMSG_RESPONSE_MAPTYPEINFO(sPacketHeader* pPacket);
	// 마을 이동 허가 통보 
	void DoMsg_DBMSG_RESPONSE_WARPVILLAGE(sPacketHeader* pPacket);
	// 특정 사용자를 감옥으로 보낸다
	void DoMsg_DBMSG_RESPONSE_PRISONUSER(sPacketHeader* pPacket);

	// 개인 거래 요청에 대한 응답.
	void DoMsg_DBMSG_RESPONSE_PRIVATETRADE(sPacketHeader* pPacket);

	// 운영자가 사용자의 로그를 요청한다.
	void DoMsg_DBLOGMSG_RESPONSE_PERSONAL_LOG(sPacketHeader* pPacket);

	// 운영자가 6조 및 군주를 임명했다.
	void DoMsg_DBMSG_RESPONSE_SETMINISTER(sPacketHeader* pPacket);

	// 군주가 퇴출 마을을 설정했다.
	void DoMsg_DBMSG_RESPONSE_SETLIQUIDATION(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_SETBONUSTIME(sPacketHeader* pPacket);

	// 가차 상품 구매

	void DoMsg_DBMSG_RESPONSE_GACHABUY(sPacketHeader* pPacket);


	// Server&DB-NPC.cpp
	// 교역 상인 NPC 의 정보를 DB에 저장한다.
	void DoMsg_DBMSG_RESPONSE_SETFOREIGNMERCHANTNPCINFO( sPacketHeader* pPacket );
	// 교역 상인 NPC 의 정보를 DB로 부터 받아온다.
	void DoMsg_DBMSG_RESPONSE_GETFOREIGNMERCHANTNPCINFO( sPacketHeader* pPacket );
	// 교역 상인 NPC 가 캐쉬 아이템 구입
	void DoMsg_DBMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM( sPacketHeader* pPacket );
	// 추천 포인트로 아이템 구입
	void DoMsg_DBMSG_RESPONSE_BUYRECOMMANDPRODUCT(sPacketHeader* pPacket );
	// 자신을 추천인으로 등록한 사람들의 목록을 가져옴
	void DoMsg_DBMSG_RESPONSE_RECOMMANDPOINTLIST(sPacketHeader* pPacket );
	// 마을 파산을 시작한다.
	void DoMsg_DBMSG_RESPONSE_VILLAGEBANKRUPTCY(sPacketHeader* pPacket );


	// Server&DB-TradeMerchant.cpp
	// 무역 상인 NCP 의 기본 정보를 DB에 저장한다.
	void DoMsg_DBMSG_RESPONSE_TRADEMERCHANTSET( sPacketHeader* pPacket );
	// 무역 상인에게 아이템을 판매한다. ( 무역 상인 입장에서는 구입 )
	void DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON( sPacketHeader* pPacket );
	// 무역 상인이 말을 구입한다.
	void DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEM( sPacketHeader* pPacket );

	// Server-Web.cpp
	void DoMsg_DBMSG_RESPONSE_NEWS( sPacketHeader* pPacket );

	// ServerDB-WorldMoney
	void DoMsg_DBMSG_RESPONSE_DISTRIBUTION_WORLDMONEY( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_TRADELIST( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_SELLORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_BUYORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_CANCELORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_SETAVERAGEPRICE(sPacketHeader*pPacket);
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_REDUCEPRICE(sPacketHeader*pPacket);
	void DoMsg_DBMSG_RESPONSE_WORLDMONEY_SET_CHAR(sPacketHeader*pPacket);

	void DoMsg_DBMSG_RESPONSE_TRADESERVER_SETORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TRADESERVER_CANCELORDER( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TRADESERVER_BUYITEM( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TRADESERVER_SET_SELLINFO( sPacketHeader* pPacket );

	void DoMsg_DBMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU( sPacketHeader* pPacket );
	
	// WorldPassport
	void DoMsg_DBMSG_RESPONSE_WORLDPASSPORT_SET(sPacketHeader*pPacket);
	void DoMsg_DBMSG_RESPONSE_WORLDPASSPORT_EXTEND(sPacketHeader*pPacket);

	// 국가 정보 입력
	void DoMsg_DBMSG_RESPONSE_SETCOUNTRY( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETSHOWCOUNTRY( sPacketHeader* pPacket );
	
	// 푸치코미 보여주기 안보여주기
	void DoMsg_DBMSG_RESPONSE_SETSHOWPUCHIKOMI( sPacketHeader* pPacket );

	//fp 증가 운영자 명령어
	void DoMsg_DBMSG_RESPONSE_GIVEFP( sPacketHeader* pPacket);

	// 버프 추가/삭제
	void DoMsg_DBMSG_RESPONSE_ADDBUF( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DELETEBUF( sPacketHeader* pPacket);

	// 스승 점수 업데이트
	void DoMsg_DBMSG_RESPONSE_SETTEACHERPOINT( sPacketHeader* pPacket);
	// 스승 점수로 아이템 얻기
	void DoMsg_DBMSG_RESPONSE_GETITEM_FROM_TEACHERPOINT( sPacketHeader* pPacket);
	// 스승이 제자와의 관계를 끊는다
	void DoMsg_DBMSG_RESPONSE_RENOUNCEPUPIL( sPacketHeader* pPacket);
	// 제자가 스승레벨을 추월했는가?
	void DoMsg_DBMSG_RESPONSE_OVERTAKETEACHERLEVEL( sPacketHeader* pPacket);

	// PVP결과
	void DoMsg_DBMSG_RESPONSE_PVPRESULT( sPacketHeader* pPacket);
	
	// 통계 초기화
	void DoMsg_DBMSG_RESPONSE_INITSTATISTICS( sPacketHeader* pPacket );

	// 모후모후 이벤트
	void DoMsg_DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST( sPacketHeader* pPacket );

	// 보상NPC
	void DoMsg_DBMSG_RESPONSE_SYSTEMREWARDLIST( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_RECIEVESYSTEMREWARD( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GIVESYSTEMREWARD( sPacketHeader* pPacket );

	// 출석체크
	void DoMsg_DBMSG_RESPONSE_ATTEND( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETATTEND( sPacketHeader* pPacket );

	//KHY - 1220 - 시간종량제
	//KHY - 1220 - 변신시스템.
	void DoMsg_DBMSG_RESPONSE_TRANSFORM_ADDCHAR( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TIMEMETERRATE_ADDTIME( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_TIMEMETERRATE_USETIME( sPacketHeader* pPacket );	

	void DoMsg_DBMSG_RESPONSE_SET_HOMEPAGE_URL( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_GET_HOMEPAGE_URL( sPacketHeader* pPacket );	

	// 제국
	void DoMsg_DBMSG_RESPONSE_CREATEEMPIRE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_DESTROYEMPIRE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_JOINEEMPIRE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SECEDEEMPIRE( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_ALLEMPIREINFO( sPacketHeader* pPacket );
	void DoMsg_DBMSG_RESPONSE_SETEMPIREMONEY( sPacketHeader* pPacket );

	////GMTOOL[손성웅]
	void DoMsg_DBMSG_RESPONSE_GMTOOL_CHARBASICINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GMTOOL_ANIMALINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GMTOOL_INVENTORYINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GMTOOL_SUMMONINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GMTOOL_SKILLINFO(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GMTOOL_REALESTATEINFO(sPacketHeader* pPacket);

	// [추가 : 황진성 2007. 12. 7 오토프로그램 메시지 처리 DB -> S]
	void DoMsg_DBMSG_RESPONSE_AUTOMACRO(sPacketHeader* pPacket);
	
	//명예의 전당 리스트
	void DoMsg_DBMSG_RESPONSE_HOFLIST(sPacketHeader* pPacket);
	//랭킹 리스트 
	void DoMsg_DBMSG_RESPONSE_RANKINGLIST(sPacketHeader* pPacket);

	//******************************************************************************************
	
	//[추가 : 황진성 2008. 1. 30 => 공간 상자(보물)에 저장. DB->S]
	void DoMsg_DBMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM(sPacketHeader* pPacket);
	
	//[추가 : 황진성 2008. 1. 30 => 공간 상자(보물)에 회수.DB->S]
	void DoMsg_DBMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM(sPacketHeader* pPacket);

	//[추가 : 황진성 2008. 1. 30 => 공간 상자에 아이템 리스트. DB->S]
	void DoMsg_DBMSG_RESPONSE_SPACE_BOX_ITEM_LIST(sPacketHeader* pPacket);
	
	//******************************************************************************************

	//------------------------------------------------------------------------
	//	[추가 : 황진성 2008. 2. 28 => 아티펙트 DB 메시지 처리 함수.]
	//------------------------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_INVENTORYLIST(sPacketHeader* pPacket); 
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_SAVEINVENTORY(sPacketHeader* pPacket); 
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_MOVEITEMSTGTOINV(sPacketHeader* pPacket); 
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_MIXTURE(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_REMOVE(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_ARTIFACT_SLOT_CREATE(sPacketHeader *pPacket);

	//******************************************************************************************

	// Daily Quest2( Server&DB-DailyQuest2.cpp )	
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST2ADD( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST2DEL( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST2CHANGE( sPacketHeader* pPacket);

	// Daily Quest3( Server&DB-DailyQuest3.cpp )	
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST3ADD( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST3DEL( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_DAILYQUEST3CHANGE( sPacketHeader* pPacket);

	//KHY - 0530 명나라 상인 구매제한.
	void DoMsg_DBMSG_RESPONSE_GET_FOREIGNMERCHANTBUYLIMIT( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GET_FOREIGNMERCHANTNOWMONEY( sPacketHeader* pPacket);
	
	//[진성] 서버 이벤트 메시지 처리 (DB->S) => 2008-6-20
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_SERVER_INFO(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_DAILY_SERVER_INFO(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_PERSONAL_INFO(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_SERVER_BUF_KIND(sPacketHeader *pPacket);

	// [영훈] 서버이벤트 관련
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_SET(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_GET(sPacketHeader *pPacket);
	void DoMsg_DBMSG_RESPONSE_SERVER_EVENT_PERSON_SCORE_GET(sPacketHeader *pPacket);

	void DoMsg_DBMSG_RESPONSE_SUMMONUSESTON(sPacketHeader *pPacket);
	
	// PCK : 스폐셜 퀘스트 완료 (로그용)
	void DoMsg_DBMSG_RESPONSE_SETSPECIALQUESTCHANGE(sPacketHeader* pPacket);
	
	// PCK : 네이밍 퀘스트 완료 (로그용)
	void DoMsg_DBMSG_RESPONSE_SETNAMINGQUESTINFO(sPacketHeader* pPacket);

	// PCK : 상단 입출금 내역 (로그용)
	void DoMsg_DBMSG_RESPONSE_CITYHALL_SETINOUTMONEY(sPacketHeader* pPacket);

	// PCK : 마을주민에게 아이템 지급 (로그용)
	void DoMsg_DBMSG_RESPONSE_GIVEVILLAGEITEM(sPacketHeader* pPacket);

	// PCK : 캐릭터 보너스 수치 상태변경 (로그용)
	void DoMsg_DBMSG_RESPONSE_SETBONUSINFO(sPacketHeader* pPacket);

	//[진성] 호박상자 => 2008-9-16
	void DoMsg_DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET(sPacketHeader* pPacket);

	// PCK : 마을 가입후 플레이시간 보기 (로그용)
	void DoMsg_DBMSG_RESPONSE_VILLAGEJOINPLAYTIME(sPacketHeader* pPacket);

	// PCK : 마을 가입후 레벨업 횟수 (로그용)
	void DoMsg_DBMSG_RESPONSE_VILLAGEJOINLEVELUP(sPacketHeader* pPacket);

	// PCK : 마을 가입후 아이템 제조 경험치 (로그용)
	void DoMsg_DBMSG_RESPONSE_VILLAGEJOINMAKEITEM(sPacketHeader* pPacket);

	// [영훈] PVP 무한대전 정보 요청 응답
	void DoMsg_DBMSG_RESPONSE_PVP_LEAGUE_INFOGET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_PVP_LEAGUE_UI_INFOGET(sPacketHeader* pPacket);

	// [성웅] PVP 무한대전 END 기록 응답
	void DoMsg_DBMSG_RESPONSE_PVP_LEAGUE_BATTLE_END( sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_PVP_LEAGUE_BATTLE_CHANGEGRADE( sPacketHeader* pPacket);
	

	//[진성] 도우미
	void DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_DEL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_SET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_GET(sPacketHeader* pPacket);

	//[강형] 채팅블록
	void DoMsg_DBMSG_RESPONSE_SETMUTETIME(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETMUTETIME(sPacketHeader* pPacket);

	// [성웅] 발렌타인 데이
	void DoMsg_DBMSG_RESPONSE_VALENTINEWAR_INFO_GET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_VALENTINEWAR_REQUEST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_VALENTINEWAR_GET_PRIZE(sPacketHeader* pPacket);
	
	//[진성] 엘릭서. 
	void DoMsg_DBMSG_RESPONSE_ELIXIR_SET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_ELIXIR_GET(sPacketHeader* pPacket);

	//[진성] 프로모터.
	void DoMsg_DBMSG_RESPONSE_PROMOTER_INFO_GET(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_SETBALANCECANUSECURRENCY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETBALANCECANUSECURRENCY(sPacketHeader* pPacket);
	

	// [영훈] 새로운 선택상자 아이템 받기
	void DoMsg_DBMSG_RESPONSE_NEWSELECTBOX_ITEM_GET(sPacketHeader* pPacket);

	//=============================================================================================
	// [진성] 통합마켓.
	//---------------------------------------------------------------------------------------------
	void DoMsg_DBMSG_RESPONSE_NEWORDERMARKET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWTRADEMARKET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWMARKETACCOUNT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWMARKETITEMLIST(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_GETFROMNEWMARKET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SETNEWMARKETFEE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWMARKETRESULT(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWMARKET_SETITEMPRICE(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_NEWMARKET_INCREASEBANKMONEY(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_MANYITEMNEWPRICE(sPacketHeader* pPacket);					// 아이템 가격 요청에 대한 응답. 
	void DoMsg_DBMSG_RESPONSE_MANYITEMNEWPRICE_COMPLETED(sPacketHeader* pPacket);		// 아이템 가격 요청에 대한 응답 완료 
	void DoMsg_DBMSG_RESPONSE_SETITEMNEWPRICE(sPacketHeader* pPacket);					// 아이템 가격 저장. 
	void DoMsg_DBMSG_RESPONSE_NEWMARKETREDUCEPRICENOTICE(sPacketHeader* pPacket);		// 아이템 가격 하락정보 요청 
	
	void DoMsg_DBMSG_RESPONSE_ITEMMAKERNAMENEW(sPacketHeader* pPacket);					// 아이템 시리얼 번호로 제조자 얻기. 

	void DoMsg_DBMSG_RESPONSE_CELLULAR_PHONE_AUTH(sPacketHeader* pPacket);				// [진성] 핸드폰 인증 여부.
	
	// [영훈] 결혼 시스템
	void DoMsg_DBMSG_DBMSG_RESPONSE_SET_MARRY(sPacketHeader* pPacket);					//[영훈] 결혼을 했다
	void DoMsg_DBMSG_RESPONSE_GET_MARRY_INFO(sPacketHeader* pPacket);					//[영훈] 결혼정보를 얻는다
	void DoMsg_DBMSG_RESPONSE_MARRY_ITEMGET_SET(sPacketHeader* pPacket);				//[영훈] 결혼 기념일 아이템받기 정보를 셋팅
	void DoMsg_DBMSG_RESPONSE_MARRY_LOVE_SET(sPacketHeader* pPacket);					//[영훈] 결혼 애정도 관련 셋팅
	
	void DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE_LIST_GET(sPacketHeader* pPacket);			//[영훈] 결혼식 목록 받기
	void DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE(sPacketHeader* pPacket);					//[영훈] 결혼식 예약 하기
	void DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE_CANCEL(sPacketHeader* pPacket);			//[영훈] 결혼식 예약 취소

	// [성웅]가족 시스템
	void DoMsg_DBMSG_RESPONSE_GET_FAMILYINFO(sPacketHeader* pPacket);					// [성웅] 가족 시스템 - 가족 정보를 얻는다
	void DoMsg_DBMSG_RESPONSE_FAMILY_ADD_CHILDREN(sPacketHeader* pPacket);				// [성웅] 가족 시스템 - 자식 추가
	void DoMsg_DBMSG_RESPONSE_FAMILY_DEL_CHILDREN(sPacketHeader* pPacket);				// [성웅] 가족 시스템 - 자식 삭제
	void DoMsg_DBMSG_RESPONSE_FAMILY_PLUS_ADD_DATE(sPacketHeader* pPacket);				// [성웅] 가족 시스템 - 가족효과연장
	void DoMsg_DBMSG_RESPONSE_FAMILY__SET_COOKIE_EFFECT(sPacketHeader* pPacket);		// [성웅] 가족 시스템 - 쿠키 사용

	// [영훈] 수호정령
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_CREATE(sPacketHeader* pPacket);					// [영훈] 수호정령 생성
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_GET(sPacketHeader* pPacket);					// [영훈] 캐릭터가 갖고있는 수호정령 가져오기
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_FELLOWSHIP_SET(sPacketHeader* pPacket);			// [영훈] 캐릭터가 갖고있는 수호정령 친화도 설정
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_CHANGE_NAME(sPacketHeader* pPacket);			// [영훈] 캐릭터가 갖고있는 수호정령 이름 변경
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_DEAD(sPacketHeader* pPacket);					// [영훈] 캐릭터가 갖고있는 수호정령 죽음

	void DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_GET(sPacketHeader* pPacket);				// [영훈] 수호정령 보관소 정보 얻어오기
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_CHANGE(sPacketHeader* pPacket);			// [영훈] 보관소에 있는 수호정령과 캐릭터에 있는 수호정령을 맞교환함
	void DoMsg_DBMSG_RESPONSE_SOULGUARD_HOUSE_CREATE(sPacketHeader* pPacket);			// [영훈] 수호정령 보관소에 슬롯 갯수를 늘림
			
	// [기형] 휴면 계정 시스템 
	void DoMsg_DBMSG_RESPONSE_DORMANCY_POINTITEM_BUY(sPacketHeader* pPacket);			// [기형] 포인트로 아이템 구입
	void DoMsg_DBMSG_RESPONSE_DORMANCY_POINTINFO_SET(sPacketHeader* pPacket);			// [기형] 포인트 관련 정보 저장
	void DoMsg_DBMSG_RESPONSE_DORMANCY_STATE_CHANGE(sPacketHeader* pPacket);			// [기형] 휴면 계정 상태 변경

	// [영훈] 캐릭터 보관소
	void DoMsg_DBMSG_RESPONSE_PERSONSLOTNUMCHANGE(sPacketHeader* pPacket);				// [영훈] 캐릭터 보관소에서 캐릭터 가져오기 결과

	// [성웅] 라쿤이벤트 군주 이벤트로 변경
	void DoMsg_DBMSG_RESPONSE_GOONZURACCOONEVENT_SETINFO(sPacketHeader* pPacket);				// [성웅] 라쿤이벤트 저장 
	void DoMsg_DBMSG_RESPONSE_GOONZURACCOONEVENT_GETINFO(sPacketHeader* pPacket);				// [성웅] 라쿤이벤트 내용을 받아 온다 
	
	// [진성] 스킬북.
	void DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGIC_ADD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGIC_DEL(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD(sPacketHeader* pPacket);
	void DoMsg_DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL(sPacketHeader* pPacket);

	// [지연] 복불복
	void DoMsg_DBMSG_RESPONSE_BBB_SET_REWARDMONEY(sPacketHeader* pPacket);				// [지연] 복불복 - 보상 누적 금액 요청에 대한 응답.
	void DoMsg_DBMSG_RESPONSE_BBB_GET_REWARD_MONEY(sPacketHeader* pPacket);				// 

	// [지연] 군주데이미션 : 미션 부여 여부에 대한 디비의 응답
	void DoMsg_DBMSG_RESPONSE_GOONZUDAYMISSIONINFO_GET(sPacketHeader* pPacket);

	void DoMsg_DBMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_INPUT(sPacketHeader* pPacket);		// 
	void DoMsg_DBMSG_RESPONSE_CONTRIBUTION_MAKE_NPC(sPacketHeader* pPacket);		// 
	

	
	

	//=============================================================================================
	//#############################################################################################
	// 디비 패킷 처리함수 여기까지
	//#############################################################################################
	//=============================================================================================

	
	
	// 버퍼에 있는 뉴스 정보를 보낸다.
	bool PopSpecialNews();

	BOOL InBufferMessageOperation(cltMsg* pclMsg, SI32 id);

	SHORT GameAction();
	// 서버의 액션. 
	BOOL ServerAction();

	// 클라이언트로 서버의 응답값을 보낸다. 
	void SendServerResponseMsg(SI16 receiver, UI16 returnval, SI16 para1, SI16 para2, SI16 charunique);
	//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
	void SendServerResponseCharMsg(SI16 receiver, UI16 returnval, TCHAR* para1, TCHAR* para2, SI16 charunique);
	//성웅 클라이언트로 캐릭터형태의 리턴값을 보낸다.2->3개
	void SendServerResponseCharMsg2(SI16 receiver, UI16 returnval, TCHAR* para1, TCHAR* para2, TCHAR* para3, SI16 charunique);

	//-------------------------------------------
	// NPC
	//---------------------------------------------

	// NPC를 만들고 아이템도 준다. 
	SI32 SetNPCChar(SI32 kind, SI16 villageunique, cltIdentity* pclidentity, SI32 level, SI32 life, SI32 x, SI32 y, SI32 mapindex, SI32 itemmode, cltInnerStatusInfo *pInnerStatusInfo, cltOrderDataInfo*pOrder= NULL, cltGMCommandKindInfo* pGMCommandKindinfo = NULL);

	// // 서버인 경우 NPC를 배치한다. 
	BOOL PutNPC();

	// NPCGroup범위 안에 있는가 ?
	bool IsInNPCGroup(SI32 npcgroup, SI32 mapindex, SI32 x, SI32 y);

	// NPCGroup 의 포인터를 얻어온다. 
	cltNPCGroup* GetNPCGroupPointer(SI32 npcgroup);



	//----------------------------------------------
	// MyDPID
	//----------------------------------------------
	BOOL IsUser(DPNID dpid){return FALSE;}

	//----------------------------------------
	// Char Information Update
	//----------------------------------------
	BOOL SendCharUpdate(SI32 charunique, bool burgentmode);


	//-----------------------------------------------
	// 운영자 명령 관련 
	//-----------------------------------------------
	// 운영자 명령을 처리한다. 
	BOOL OperateGameMasterOrder(TCHAR* pstring, SI32 id);


	//-----------------------------------------------
	// 아이템 기간 관련
	//-----------------------------------------------
	// 아이템 기간 관련 처리
	BOOL GetItemUseAbleDay(cltItem* clItem, SI32 * outUseAbleDay); // 사용기간이 지난 아이템이면 참을 반환

	//-----------------------------------------------
	// 아이템 이동 관련
	//-----------------------------------------------
	// 중첩가능한 아이템 이동시 갯수 계산
	BOOL DistributePileMoveItem(SI32 charid, cltItem clFrom, UI16 uiFrom, cltItem clTo, UI16 uiTo);

	//-----------------------------------------------
	// 공격관련 
	//-----------------------------------------------
	// 공격의 여러 속성을 고려하여 적의 체력을 저하시킨다. 
	// 파라메타 설명:
	// myid      : 공격을 시도하는 캐릭터의 아이디 
	// attackatb : 공격속성(main.h에 정의 )
	// range     : 피해 범위, 일반적으로 1( 1이면 -1 ~ +1)
	// power     : 공격력 
	// attackid  : 특별히 맞추고 싶은 캐릭터의 아이디 
	// dx, dy    : 타격을 줄 목적지의 좌표 

	// 리턴값   
	//  0 -> 항상 0을 리턴 
	BOOL HitOperation(SI32 myid, cltAttackDamageInfo* pcldamageinfo, SHORT range, const cltAttackTarget* pcltarget, cltMapCommon* pclMap);

	// 공격속성, 공격력, 공격대상을 고려하여 실제 피해를 계산한다. 
	// PCK : specialattackswitch를  bool에서 type으로 변경한다. (0 : 일반공격, 1 : 치명타, 2 : 추가타(수박버프) 
	bool GetDamage(cltAttackDamageInfo* pcldamageinfo, SI32 myid, SI32 attackid, UI08 specialattackswitch, cltAttackDamageInfo* pclreturndamageinfo);

	//인챈트 by tae-yang
	SI32 GetEnchantDefen(SI32 AttackType, SI32 defenid);

	//===============================================================================
	// 적에게 실제로 데미지를 입힌다.
	// specialattackswitch : 필살기로 작동 여부.
	// PCK : specialattackswitch를  bool에서 type으로 변경한다. (0 : 일반공격, 1 : 치명타, 2 : 추가타(수박버프)
	BOOL Hit(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo, UI08 specialattackswitch);
	//===============================================================================
	// 물리적인 타격시 부가 능력을 적용시킨다.
	BOOL AddScrollMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo);
	// 캐릭터의 STATUS를 변경하는 MAGIC OPERATE 
	BOOL ChangeStatusMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo);
	// PASSIVE MAGIC을 적용시킨다. 
	BOOL PassiveMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo);
	//===============================================================================
	// myid가 enemyid를 공격했을 때의 경험치를 얻어온다. 	
	SI32 GetExp(SI16 myid, SI16 enemyid, SI16 damage);

	//===============================================================================
	// HIT 함수내 스페셜 처리 함수들
	void SpecialDamage( SI32 myid, SI32 enemyid, SI32* psiRealDamage, SI32* psiDisplayDamage );
	void DeadChar_PC( SI32 myid, SI32 enemyid );
	void DeadChar_NPC( SI32 myid, SI32 enemyid );
	bool GiveBigItem( SI32 myid, SI32 enemyid );
	bool GiveSpecialItem( SI32 myid, SI32 enemyid );

	bool GiveBigItem_Event( SI32 myid, SI32 enemyid );

	bool GiveBigItem_New( SI32 myid, SI32 enemyid );
	//===============================================================================

	// 마법에 의한 캐릭터의 조치 계산. 
	void MagicPopAction();

	//---------------------------------------------
	// DB
	// DB와 관련된 함수들 정리. 
	//---------------------------------------------

	//---------------------------------------------------------------------
	// cltRank정보를 DB로 저장한다. 
	//---------------------------------------------------------------------
	SI32 LoadDB_cltRank(SI32 sipersonid, cltSimpleRank* pclrank);
	SI32 DelDB_cltRank(SI32 sipersonid);

	//-----------------------------
	// 네트워크 관련.  
	//----------------------------
	void InitNetwork();
	// Agent에서 받은 정보를 토대로 서버들과의 연결을 설정한다.
	void SetOtherServer(TCHAR* frontip, SI32 frontport, TCHAR* authip, SI32 authport, TCHAR* dbip, SI32 dbport, SI32 clientlistenport);

	// 세션캐릭터를 pclCM과 연결한다. 
	void  SetSessionChar();
	bool StartNetwork();

	void NetworkOperation();
	void GameDBAction();

	bool SendDBMsg(SI32 siselect, sPacketHeader *pPacket);

	// 클라이언트에게 쪽지를 보내준다. 
	SI32 SendLetterMsg(SI32 personid, cltLetterHeader* pclheader);

	// 특정 유저의 계정으로 쪽지를 보내준다.
	SI32 SendAccountLetterMsg(TCHAR* AccountID, cltLetterHeader* pclheader);

	// 전체 유저에게 알림 메시지를 보낸다. 
	bool SendNoticeMsg(SI16 villageunique, SI16 villagerunique, SI16 charunique, TCHAR* msg);

	// 특정 사용자에게 알림 메시지를 보낸다. 
	bool SendNoticePersonMsg(SI16 charunique, TCHAR* msg, TCHAR* header= NULL);

	//
	void SetWarResult( SI32 winVillage, SI32 loseVillage );
	void SetVillageWarLoss( SI32 winVillage, SI32 loseVillage );

	void SetVillageLevel();
	void GetSlaveVillageNum();

	// MD5로 변환할수 있는 값으로 변환한다

	//------------------------------------
	// 시간 
	//------------------------------------
	// DB에 시간을 저장한다. 
	void UpdateDBTime(cltDate* pcldate);

	//----------------------------
	// NPC고용하기. 
	//----------------------------
	bool Hire(SI32 kind, SI32 slot, SI32 villageunique, bool mode);

	void GetTopLevelVillage( SI32 *villageunique, SI32 *villagelevel );


	void SendPostOfficeItemToPerson( TCHAR *szPersonName, cltItem *pclItem, TCHAR *szMsg, GMONEY money = 0 );
	void SendPostOfficeItemToPerson( SI32 personid, cltItem *pclItem, TCHAR *szMsg, GMONEY money = 0 );

	//--------------------------------------------
	// 수리 함수. 
	//--------------------------------------------
	bool Repair(SI32 mastercharunique, SI32 clientcharunique, SI32 clientitempos, cltItem* pclitem, GMONEY repairmoney, SI32* preturnval);

	cltStrInfo* GetStrInfo( SI32 villageUnique, SI32 strType );

	// 호위병을 만든다. 
	SI32 MakeGuardChar(SI32 kind, SI16 homevillage, SI32 mapindex, SI32 x, SI32 y, cltOrderDataInfo* pclorder, cltEnemyTypeInfo* pclenemytype);

	// myid가  enemyid를 공격했을때의 경험치 조치. 
	SI32 ExpOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo);
	
	// ExpOperate함수에서 캐릭터 경험치를 제외하고 스킬경험치 부분만 줄수 있도록 새로 만듦
	SI32 ExpSkillOperate( SI32 siMyID, SI32 siEnemyID, cltAttackDamageInfo* pclDamageInfo );
	// 경험치 조치시 소환수와 내가 나누어 갖도록 한다.
	void IncreaseBothExp(SI32 siID, SI32 siExp);

	//myid가 enemyid를 죽여서 얻게 되는 전리품 처리. 
	void PartyWarItemOperate(SI32 myid, SI32 enemyid,CPartyObj* pPartyObj );
	void WarItemOperate(SI32 myid, SI32 enemyid);

	// DB정보 초기화 완료 여부
	BOOL bCompleteDBInitAll;
	//BOOL bCompleteDBInit[ DBCONNECTION_NUMBER ];

	// 이벤트 설정/해제 
	void SetEvent(SI32 index, bool bmode);

	// pc방 이벤트
	void PCRoomEventAction();

	// 명품이 제작되었음을 전체 사용자에게 알린다. 
	// bselfswitch : 사용자가 직접 제작 혹은 혹부리 영감이 제작. 
	void SendRareItemMade(SI32 id, cltItem* pclitem, SI32 bselfswitch);

	// 서버의 종합 정보를 클라이언트에게 보낸다. 
	void SendServerInfo(SI32 id);

	// 현재 서버의 정보를 획득한다. 
	void GetServerInfo();	

	// KHY - 0901 -  과금 아이템 자동구매 설정.
	void GetServerSystemBuy();	

	// 한게임 서비스용 정보를 획득한다.
	void GetHanGameInfo();	

	// 인벤에서 물품을 만들도록 하는 함수. 
	void MakeItemProcess(SI32 id, cltGameMsgRequest_MakeItemInv* pclinfo);

	SI32 CalcExp_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice );
	SI32 CalcSkillExp_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice );

	SI32 CalcExp_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum );
	SI32 CalcSkillExp_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum );
	
	SI32 CalcExp_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum );
	SI32 CalcSkillExp_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum );

	SI32 CalcTime_MakeItem( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum, SI64 siPrice );
	SI32 CalcTime_MakeItem_New( cltCharServer* pclchar, SI32 skillunique, SI32 siItemUnique, SI32 siNum );
	SI32 CalcTime_MakeItem_FoodHeal( cltCharServer* pclchar, SI32 siSkillUnique, SI32 siItemUnique, SI32 siNum );

	void EnchantItemProcess(SI32 id, cltGameMsgRequest_MakeItemInv* pclinfo);

	BOOL m_bToSendServerInfoToFront;
	void SendServerInfoToFront();

	// 객주에서 물품 구입에 대한 처리를 한다. 
	void MarketBuyAction(SI16 id, SI16 villageunique, cltTradeOrder* pclOrder, SI16 siReason);

	// 특정 캐릭터에게 물품을 주는 처리를 한다. 
	void InputItem(cltCharServer* pclchar, cltItem* pclitem, SI32 inputitemreason);

	// 흥부박퀘스트 보너스 타임 대상자를 구한다. 
	SI32 PopSpecialQuestBonus();
	// 흥부박퀘스트 보너스 타임 대상자를 버퍼에 넣는다. 
	bool PushSpecialQuestBonus(SI32 personid);

	// 뉴스 정보를 버퍼에 넣는다.
	bool PushSpecialNews(TCHAR * szTitle,TCHAR * szName, TCHAR * szURL, SI32 NewsKind = CAPTIONKIND_NEWS , SI32 SendKind = CAPTIONKIND_NEWS );

	// 랜덤하게 유저 1명을 뽑는다.
	SI32 GetRandomID();

	//---------------------------------------------------
	// Server-BonusTime.cpp
	//---------------------------------------------------
	// 전투 보너스 타임 액션.
	void BattleBonusTimeAction();

	// 제조 보너스 타임 
	void MakeBonusTimeAction();

	// 흥부박 이벤트 처리.
	void SpecialQuestAction();

	//KHY - 0115 발렌타인데이 이벤트.
	void ValentineEventGiveItemAction();


	//KHY - 0218 화이트데이 이벤트.
	void WhiteEventGiveItemAction();

	bool PushCash(SI16 charunique, SI32 personid, TCHAR* szaccountid, GMONEY money, SI32 reason, SI32 day = 3 );
	bool PopCash();

	void RefreshHouseUnit();
	void RefreshInvestList();
	void RefreshStock();

	void SendLog_InventoryFull( SI32 personid, SI32 itemunique, SI32 itemnum, SI32 reason );
	void SendLog_GiveQuestItem(SI32 personid,SI32 questtype , SI32 questunique,SI32 itemunique,SI32 itemnum,SI32 ncoin,SI32 usedate,bool success );
	void SendLog_GetQuest(SI32 personid,SI32 questtype , SI32 questunique,bool start);
	void SendLog_Warp(SI32 personid, SI32 mapunique);
	void SendLog_MofuMofuEvent_Start(SI32 villageunique);
	void SendLog_SetHappyBeanClearInfo(cltCharServer* pclchar, SI32 siQuestType, SI32 siQuestState);

	//KHY - 0906 - 게임방 이벤트 쿠폰.
	void SendLog_SetEventCouponInfo(cltCharServer* pclchar, SI32 siEventType, SI32 siPara);
	//LEEKH - 2007.10.29 - 함수 추가
	void SendLog_SetEventCouponInfo(TCHAR* pszAccountID, TCHAR* pszCharName, SI32 siPersonID, SI32 siEventType, SI32 siPara);
	
	////옥션이벤트 [2007.09.10 손성웅]
	//void SendLog_SetAUCTIONEventCouponInfo(cltCharServer* pclchar, SI32 siEventType);


	// by LEEKH - 2007.08.21 - 새로운 로그 시스템
	void SendLog_WriteLogToDB(	SI16 siCommandType, SI16 siCommandIndex, UI08 uiReason, cltCharServer* pclchar, cltSummon* pclSummon, SI32 siTargetPersonID, GMONEY money, GMONEY moneysum, 
								cltItem* pclItem, SI32 siHaveItemCount, SI32 siMonKind, SI16 siNpcKind, SI32 siVillageUnique, SI32 siRPPoint, SI32 siFameExp,
								SI64 siParam1, SI64 siParam2, SI64 siParam3, SI64 siParam4, SI64 siParam5, TCHAR* pszCharParam1, TCHAR* pszCharParam2);

	void SendSet_InstanceProfitVillage(SI32 charid,bool start = false );
	
	void SetTodayVillageWarInfo();

	bool HireEventNPC(SI32 kind, SI32 villageunique, UI16 siSlot, bool mode);

	// 서버 다운 검증을 위해 위치 변경
	////======================================================
	//// 메시지맵
	////======================================================
	//// 서버메시지 처리 함수포인터
	//class stServerMsgPtr
	//{
	//public:
	//	void (cltServer::*ServerMsgFunc)(cltMsg* pclMsg, SI32 id);
	//};

	//stServerMsgPtr *m_pServerMsgPtr;
	//void	ServerMsgNullFunc(cltMsg* pclMsg, SI32 id);
	//void	RegisterServerMsgMap();

	//// DB메시지 처리 함수포인터
	//class stDBMsgPtr
	//{
	//public:
	//	void (cltServer::*DBMsgFunc)(sPacketHeader* pPacket);
	//};

	//stDBMsgPtr *m_pDBMsgPtr;
	//void	DBMsgNullFunc(sPacketHeader* pPacket);
	//void	RegisterDBMsgMap();
	////======================================================

	
	bool	CheckWarp( SI32 ID, SI32 MapIndex, SI32 DestX = -1, SI32 DestY = -1 );

	// pvp-
	SI32	GetPVPWeaponDamage( SI32 RealDamage, SI32 AttackType );

	static bool	NewYearEventDay(void);
	void	NewYearLuckyBox(cltItem& clfromitem);
	void	NewYearButton(void);

	void	NewYearEffect(void);

	//KHY - 0530 명나라 상인 구매제한.
	bool CanChargingBuyLimit( );
	bool CanChargingSystemBuyItemLimit( );
	void SystemMarketSell( void );
	
	// 매일 정각 니 발렌타인데이 솔로냐? 라고 묻는다.
	void	SetValentineDayOnTime(void);
	// 매일 12시 정각에 등급변화를 브로드 케스팅 한다.
	void	ChangePvPWarGradeInfo(void);
	
	bool	IsUseFourLeaf( SI32 siItemUnique, cltCharServer* pclchar );	// 현재 사용가능한 네잎크로바 인가?

	NTCHARString<4> GetTradeServer_ServerNation( SI32 ServiceArea );
	SI16	GetTradeServer_CharNation( cltCharServer* pclchar );

	// 12시가 되면 하루동안의 길드 전싸운 적길드 정보 초기화.
	void InitHuntMapWarEnemy();

	// 보물찾기 이벤트 
	void Read_TreasureEvent_Map_Info()	;		// 맵 정보를 읽어 온다.// 맵인덱스 별로 최대한 만들 npc 숫자.
	void Write_Kill_NPC_Num( SI32 mapindex )	;	// 죽은 npc 기록
	void Respon_TreasureEvent_Npc()	;			// 죽은 npc 만큼 다시 이벤트 npc 생성	
	void Read_TreasureEvent_MapInfo_TheQ()	;	// 맵 좌표 정보를 읽어 온다.
	SI32 MakeMapRocSlot( SI32 siMapindexSlot , SI32 MakeNpcNum )	;					//  랜덤 좌표중 사용할 좌표 만 만들어 낸다 .

	// 가족 
	void AddFamily( SI32 siParent_PersonID, SI32 siChildren_PersonID,SYSTEMTIME stFamilyEffectTime)	;	// 가족 구성원을 추가 한다.
	void DelFamily( SI32 siDelTargetPersonID )	;	// 가족 구성원을 삭제 한다.
	void UpdateFamily( CFamily* pclPamily,SI32 siPersonID )	;//  받은 personID의 케릭터의 가족 정보로 가족들의 가족정보들을 업데이트 해준다

	void SetGachaRareItem_Time()	;	//해당 기간에 따라 가차 최고 상품유니크를 변경 한다. 

	void AddMagicAction( SI32 _siID, SI32 _siTargetID, SI32 _siMagicKind, SI32 siDamage );
	void SubMagicAction( SI32 _siID, SI32 _siTargetID, SI32 _siParentMagicKind, SI32 siDamage );

	// 창고
	BOOL MOVEITEMINVTOSTG(SI32 siCharId, SI32 siVillageUnique, SI32 siHouseIndex, SI32 siItemPos, cltItem* pclInvenItem, bool bNoMoveNewPos = false);

	// [지연] 군주데이미션 : 디비에 저장되어 있는 군주데이 미션 수행내역을 초기화 하도록 하는 함수
	void GoonzuDayMissionInfo_Reset();
};
#endif
