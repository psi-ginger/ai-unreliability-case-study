#include "../Common/CommonHeader.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/Feast/Feast.h"
//--------------------------------------
// Server
//--------------------------------------
#include "../../Server/Server.h"
#include "../../Server/Minister/MinisterMgr.h"
#include "../../Server/Minister/Minister-Hozo.h"
#include "../../Server/Minister/Minister-Goonzu.h"
#include "../../Server/Statistics/Statistics.h"
#include "../../Server/ServerReward/ServerReward.h"
#include "../../PartyMgr/PartyMgr.h"
#include "../../PartyMgr/PartyObj.h"
#include "../CommonLogic/Msg/MsgType-Party.h"
#include "../CommonLogic/Msg/MsgType-Hunt.h"
#include "../../Server/TraceBoot.h"

#include "Char/CharServer/Char-Server.h"
#include "Map/FieldObject/FieldObject.h"

#include "../Common/Bullet/Bullet.h"
#include "Util/Util.h"
#include "../../Order/Order.h"
#include "../CharManager/CharManager.h"

#include "../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Summon.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-DailyQuest.h"
#include "../../DBManager/GameDBManager_World/DBMsg-DailyQuest2.h"

#include "../../DBManager/GameDBManager_World/DBMsg-DailyQuest3.h"

#include "../../DBManager/GameDBManager_World/DBMsg-ItemMall.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Village.h"
#include "../../DBManager/GameDBManager_World/DBMsg-PostOffice.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Agriculture.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Summon.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-SoulGuard.h"

#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-DailyQuest.h"
#include "../CommonLogic/Msg//MsgType-DailyQuest2.h"
#include "../CommonLogic/Msg//MsgType-DailyQuest3.h"
#include "../common/AutoHunting/AutoHunting.h"
#include "../common/Event/Hunting/Hunting.h"

#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/Guild/GuildWar.h"

#include "../CommonLogic/KoinoboryFamily/KoinoboryFamily.h"

#include "../CommonLogic/CityHall/CityHall.h"
#include "../CommonLogic/BlackArmy/BlackArmy.h"

#include "../Common/NPCManager/NPC.h"

#include "../../CommonLogic/MagicMgr/MagicMgr.h"
#include "../../CommonLogic/MsgRval-Define.h."

#include "../../../NLib/NUtil.h"
// NUtil.cpp�� ����� ����
extern UI64	uiSec; // �ð� ����� ���� ���� ����


#include "ITZCell.h"
#include "ITZEncrypt.h"
#include "../CommonLogic/MsgType-System.h"
#include <MsgType-Person.h>
#include <MsgType-Item.h>
#include <Msg/MsgType-ItemMall.h>
#include "Msg/MsgType-MofuMofuEvent.h"


#include "../Common/Map/MapManager/MapManager.h"

#include "../../Server/Minister/MinisterMgr.h"
#include "../../Server/Minister/Minister-Hozo.h"
#include "../../Server/Minister/Minister-Gongzo.h"
#include "Msg/MsgType-PersonalShop.h"

#include "../../Server/ServerEvent/ServerEvent-Quest.h"
//------------------------------------
// Common
//------------------------------------
#include "../../Skill/Skill.h"
#include "../../Skill/Skill-Manager.h"
#include "../../common/Item/ItemCommon/cltSetItem.h"

#include "../../Apex/ApexGoonZu.h"

#include "MsgType-Person.h"

extern cltCommonLogic* pclClient;
extern HashStringInt*  pclHashTable;

extern SI32 AttackAtbInfo[];

#ifdef _SAFE_MEMORY
NSafeTArray<SI32, MAX_CHAR_NUMBER>	cltCharCommon::CharUniqueInfo;
#else
SI32	cltCharCommon::CharUniqueInfo[MAX_CHAR_NUMBER];	
#endif


#include "../Client/ninterface/nhuntmapinfodlg/HuntMapInfoDlg.h"

#include "../../CommonLogic/HuntQuestItemMgr/HuntQuestItemMgr.h"

//========================================
// cltCharUpdateInfo
//========================================
cltCharUpdateInfo::cltCharUpdateInfo()
{
	Init();
}

void cltCharUpdateInfo::Init()
{
	bMode	= false;
	siPara1	= 0;

}

//========================================
// cltItemReserveUnitInfo
//========================================
cltItemReserveUnitInfo::cltItemReserveUnitInfo()
{
	Init();
}
void cltItemReserveUnitInfo::Init()
{
	siUnique	= 0;
	siItemNum	= 0;
	siItemPos	= -1;
}
void cltItemReserveUnitInfo::Set(cltItemReserveUnitInfo* pclinfo)
{
	siUnique	= pclinfo->siUnique;
	siItemNum	= pclinfo->siItemNum;
	siItemPos   = pclinfo->siItemPos;
}


//========================================
// cltSummonItemReserveUnitInfo
//========================================
cltSummonItemReserveUnitInfo::cltSummonItemReserveUnitInfo()
{
	Init();

}

cltSummonItemReserveUnitInfo::cltSummonItemReserveUnitInfo( SI08 siSummonIndex, SI16 siItemUnique, SI16 siItemNum, SI16 siItemPos )
{
	m_siSummonIndex	= siSummonIndex;
	m_siItemUnique	= siItemUnique;
	m_siItemNum		= siItemNum;
	m_siItemPos		= siItemPos;

}

void cltSummonItemReserveUnitInfo::Init()
{
	m_siSummonIndex	= -1;
	m_siItemUnique	= 0;
	m_siItemNum		= 0;
	m_siItemPos		= -1;

}

void cltSummonItemReserveUnitInfo::Set( cltSummonItemReserveUnitInfo* pclinfo )
{
	m_siSummonIndex	= pclinfo->m_siSummonIndex;
	m_siItemUnique	= pclinfo->m_siItemUnique;
	m_siItemNum		= pclinfo->m_siItemNum;
	m_siItemPos		= pclinfo->m_siItemPos;

}

//========================================
// cltCharSimpleVary
//========================================
cltCharSimpleVary::cltCharSimpleVary()
{
	Init();
}

void cltCharSimpleVary::Init()
{
	ZeroMemory(this, sizeof(cltCharSimpleVary));
}

//========================================
// cltCharUpdateTimer
//========================================
cltCharUpdateTimer::cltCharUpdateTimer()
{
	m_clTimer[CHAR_UPDATETIMER_TIMEMETERRATE_DB].Init(10 * 60 * 1000);		// 10��
	m_clTimer[CHAR_UPDATETIMER_TIMEMETERRATE_CLIENT].Init(20 * 1000);		// 20��
	m_clTimer[CHAR_UPDATETIMER_TRANSFORMINFO_CLIENT].Init(1* 60 * 1000);	// 1 ��
	m_clTimer[CHAR_UPDATETIMER_TRANSFORM_TIME_LIMIT].Init(30 * 1000);		// 30��

	m_clTimer[CHAR_UPDATETIMER_CRYSTALITEM_DB].Init(30* 60 * 1000);			// 30��.
	m_clTimer[CHAR_UPDATETIMER_CRYSTALITEMINFO_CLIENT].Init(5* 60 * 1000);	// 5��.
	m_clTimer[CHAR_UPDATETIMER_TIREDSYSTEM_TIME_LIMIT].Init(20 * 1000);		// 40��
	m_clTimer[CHAR_UPDATETIMER_30SECOND].Init(30 * 1000);					// 30�� Ÿ�̸�
	m_clTimer[CHAR_UPDATETIMER_3SECOND].Init(3 * 1000);						// 3�� Ÿ�̸�

	m_clTimer[CHAR_UPDATETIMER_BUGCHAR_CHECK].Init(10 * 1000);				// 10�� Ÿ�̸�
	
}

void cltCharUpdateTimer::Init()
{
	//========================================================================================
	// ó������ ������ Ÿ�̸Ӱ� �ƴϸ� ���⼭ Active�� ��Ű�� �ʰ� ����Ѵ�.
	//========================================================================================

	m_clTimer[CHAR_UPDATETIMER_TIMEMETERRATE_DB].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_TIMEMETERRATE_CLIENT].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_TRANSFORMINFO_CLIENT].SetActive(true, GetTickCount());

	m_clTimer[CHAR_UPDATETIMER_CRYSTALITEM_DB].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_CRYSTALITEMINFO_CLIENT].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_TIREDSYSTEM_TIME_LIMIT].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_30SECOND].SetActive(true, GetTickCount());
	m_clTimer[CHAR_UPDATETIMER_3SECOND].SetActive(true, GetTickCount());				// ������ 3�� üũ Ÿ�̸�.

	m_clTimer[CHAR_UPDATETIMER_BUGCHAR_CHECK].SetActive(true, GetTickCount());			// ���� ĳ���� üũ�� Ÿ�̸�.
}


//========================================
// cltCharServer
//========================================
cltCharServer::cltCharServer() : cltCharCommon()
{
	LastReceveFromClientClock	=	0;
	dwLastCheckAutoFoodClock	=	0;

	dwLastGameGuardAuthClock	=	0;
	dwTimerClock				=	0;
	NPCGroup					=	0;


	//--------------------------------
	// Daily Quest Action Clock
	//--------------------------------
	dwDailyQuestClock			=	0;
	dwDailyQuest2Clock			=	0;
	dwDailyQuest3Clock			=	0;

	bSendSecretNCHarUniqueSwitch	= false;

	//--------------------------------
	// ���� ���� ����. 
	//--------------------------------
	uiLevelInfoUpdateFrame	= 0;
	clOldLevelInfo.Init();

	clSimpleVary.Init();
	m_clCharUpdateTimer.Init();

	dwLastVisitVillageClock	= 0;

	uiCSAAuth_Count	= 0;
	bWaitGameGuardClientAuth = false;  

	//---------------------------------
	// ���� ���� �ð�. 
	//---------------------------------
	siMagicDelay		= 0;

	//---------------------------------
	// ������ ���õǾ��� ���� ���.
	//---------------------------------
	siOldWeaponMagic	= 0;

	//---------------------------------
	// �����ϴ� ���̶��, ���� ������
	//---------------------------------
	siFishingBaitItemUnique = 0;
	//-------------------------
	// �ڵ� ����
	//-------------------------
	bAutoFishing = false;

	//-------------------------
	// ��þƮ 100% ���� ����ġ (����϶���)
	//-------------------------
	bSuccessEnchant = false;

	//-------------------------
	// ���� �ð� üũ
	//-------------------------
	dwPreCatchFishClock	= 0;
	dwSummonFishClock = 0;

	//-------------------------
	// ��� ���̶��,  ȹ���ҷ��� ��� ������
	//-------------------------d
	siFarmingItemUnique = 0;

	//-------------------------
	// ��� �Ӵ��� ����( �ӽ÷� )
	//-------------------------
	//siFarmingRentVillageUnique = 0;

	//-------------------------
	// �ڵ� ���
	//-------------------------
	bAutoFarming = false;

	//-------------------------
	// ��� ȹ�� ������ ����
	//-------------------------
	siFarmingItemCount = 0;

	//-------------------------
	// ��� �ֱ�
	//-------------------------
	dwPreGetFarmingItemClock	= 0;
	dwFarmingClock = 0;

	//-------------------------
	// ä��
	//-------------------------
	siMiningItemUnique = 0;
	//siMiningRentVillageUnique = 0;
	bAutoMining = false;
	dwPreGetMiningItemClock = 0;
	//dwMiningClock = 0;
	//dwStartAgricultureClock = 0;
	//siLeaseLandVillage = 0;
	//uiLeaseLandExpire = 0;

	//-----------------------------
	// ����Ȱ�� ����
	//-----------------------------
	m_pclAgriculture = NULL;

	//-----------------------------
	// �ڵ� ȸ�� ���� 
	//-----------------------------
	dwAutoRecoverLifeClock	= 0;	
	dwAutoRecoverManaClock	= 0;

	//-----------------------------
	// ������ ���� �ڵ� ȸ�� ����  Cushion
	//-----------------------------	
	dwCusionRecoverLifeClock = 0;		// ���������� �ڵ� ȸ���� �Ǿ��� �ð�. 
	dwCusionRecoverManaClock = 0;		// ���������� �ڵ� ȸ���� �Ǿ��� �ð�. 

	// PCK : ���ڴ� �Ŀ��� ���� ��ȯ�� �ڵ� ȸ�� ���� (09.06.15)
	dwCusionRecoverSummonLifeClock = 0;		// ���������� ��ȯ�� �ڵ� ȸ���� �Ǿ��� �ð�. 
	dwCusionRecoverSummonManaClock = 0;		// ���������� ��ȯ�� �ڵ� ȸ���� �Ǿ��� �ð�.

	//KHY - 1220 - ���Žý���. - �ڵ�ȸ��
	dwTransFormRecoverManaClock = 0;

	// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
	uiBestGuild	= false;					

	clCharWarpInfo.Init();
	siWarpUnique	= 0;

	//----------------------------
	// ��� ���� 
	//---------------------------
	bDeathPenaltySwitch = false;
	bRequestReviveSwitch = false;

	DeathClock			= 0;
	siCharDyingDelay	= 0;

	bReadySwitch		= false;
	bFirstCellOpSwtich	= false;

	pclCharListInfo		= NULL;

	//----------------------------
	// �Ӹ��� 
	//----------------------------
	bHeadChatSwitch		= false;		// ���� �Ӹ����� �����Ǿ� �ִ°�?
	StringCchCopy(szHeadChatText, MAX_HEAD_CHAT_TEXT_SIZE, TEXT(""));

	//----------------------------
	// ���λ��� ������ 
	//----------------------------
	clShopData.Init();

	m_pclShopRepairData = NULL;

	//---------------------------
	// ĳ���� ���  
	//---------------------------
	siCharMode				= 0;
	siCharModePara1			= 0;

	//---------------------------
	// ��� ��� 
	//---------------------------
	siKilledMode			= 0;
	StringCchCopy(szKillMeCharName, MAX_PLAYER_NAME, TEXT(""));

	//-------------------------
	// ���ڸ����� ��� 
	//-------------------------
	bStaySwitch			= false;	// ���ڸ��� ���缭 �ִ��� ����. 

	//-------------------------
	// �÷��� �ð� ���ϱ� 
	//-------------------------
	dwStartClock		= 0;
	dwInstantPlayTime	= 0;

	// ���� �ð� �����ϱ�
	ZeroMemory(&sTimeStartTime , sizeof(_SYSTEMTIME) );

	// �α��� �������� ����
	siLoginedLevel = 0;

	//-------------------------
	// ����ȿ��. 
	//-------------------------
	dwGeneralEftSetClock	= 0;

	//-------------------------
	// ä�� �Է� �ð� 
	//-------------------------
	dwLastChatInputClock	= 0;

	//-------------------------------------
	// ���� ���� �ð�. 
	//-------------------------------------
	dwNoFirstAttackClock	= 0;
	//-------------------------------------
	// ICE ���� �ð�. 
	//-------------------------------------
	dwNoIceAttackClock		= 0;
	//---------------------------------------
	// Ŭ���̾�Ʈ�� �޽��� ���� ������ �ð�. 
	//---------------------------------------
	dwLastSendServerClock	= 0;

	//-------------------------------------
	// ���� ���� 
	//-------------------------------------
	bMakeItemReserveSwitch	= false;
	bMakeItemMinigameSwitch = false;
	bMakeItemNoMinigame = false ;
	bEnchantItemSwitch = false;
	bMakeItemSwitch = false ;
	bFinishEnchantItemSwitch = false;
	dwMakeItemStartClock	= 0;
	dwMakeItemDelayClock	= 0;
	siMonsterCorp			= 0 ;
	dwStartMiniGameTime = 0;
	pclReservedMakeItemData	= new cltGameMsgRequest_MakeItemInv;

	// [����] ���ֵ��̹̼� : �̼� ���� ���� �ʱ�ȭ
	clGoonzuDayMissionInfo.Clear();

	// �ڴ�� ����
	bRouletteStart = false;
	m_siBettingCount = 0;
	m_siResultCount = 0;
	dwRouletteStartClock = 0;
	dwRouletteDelayClock = 0;
	SI32 x = 0, y = 0;
	for( y = 0 ; y< 5; ++y )
	{
		m_siRandomValue[y] = 0;	// ���õ� ������ 0

		for( x = 0; x < 5; ++x )
		{
			m_stRouletteTable[y][x].m_bUserSelect = false;
			m_stRouletteTable[y][x].m_bCpuSelect  = false;
			m_stRouletteTable[y][x].m_bResult     = false;

			m_stRouletteTable[y][x].m_siX = 0;
			m_stRouletteTable[y][x].m_siY = 0;
			m_stRouletteTable[y][x].m_siValue = y*5 + (x + 1);
		}
	}

	//-----------------------------------
	// ���� ��� ���� ����. 
	//-----------------------------------
	bHopeToQuitSwitch		= false;

	//----------------------------------
	// ���� �ð� ����� ���� ���� 
	//----------------------------------
	bCloseSocketSwitch		= false;
	dwCloseSocketTimer		= 0;


	//---------------------------------
	// ȯ�� ����
	//---------------------------------
	bWhisperChatReject = false;
	bPrivateTradeReject = false;
	bPartyInviteReject = false;
	bFriendAddReject = false;

	//---------------------------------
	// ���� ��ġ �浹 üũ ����. 
	//---------------------------------
	dwLastCheckCollClock	= 0;

	//---------------------------------
	// �нż� ���� �ð� 
	//---------------------------------
	dwLastClone = 0;

	//-----------------------------
	// �����ؾ��� �ڸ� ����
	//-----------------------------
	siCaptionKind = CAPTIONKIND_NEWSCHAT;

	//-----------------------------
	// ��ƿ ���� 
	//-----------------------------
	dwStealClock	= 0;
	siStealNum		= 0;			// ��ƿ�� Ƚ��. 

	//----------------------------
	// ���� ���� 
	//----------------------------
	dwPrisonClock	= 0;

	//-----------------------------------
	// ����Ʈ�� �Ϸ����� �ʾ� ������ �Ұ� ���� �ð�
	//--------------------------------------------
	dwLastLevelUpFailForQuestClock	= 0;

	//-----------------------------------
	// ���� ���� 
	//-----------------------------------
	siReservedAttackTargetID		= 0;

	//-----------------------------------
	// ��ȯ�� ó�� 
	//-----------------------------------
	bIsEvolving		= false;
	bIsTradeing		= false;

	//------------------------------------
	// WeaponSkill ó��
	//----------------------------------
	bIsWeaponSkillUp = false ;

	//-----------------------------------
	// GameEvent
	//------------------------------------
	bCheckGameEvent = false ;

	//-------------------------------------
	// ���� �ŷ����̴�.
	//-------------------------------------
	bTradeServerUsing = false ;

	//-------------------------------------
	// ���� �������� �Դ� ���̴�. ���ÿ� �Ծ ������ ������� ������ �ذ��ϱ� ���� ����
	//-------------------------------------
	bIsNowAddBufThroughDB = false;

	//-------------------------------------
	// ���� �������� �̴� ���̴�. �ѹ��� ������ ������ �Ҽ� ���Բ� �ϴ� ����
	//-------------------------------------
	bNowGachaBuy = false;

	//-------------------------------------
	// ��ȯ���� ��ȯ���̹Ƿ� ���� ��ȯ���� ��ȯ��Ű�� ���ϰ� �Ѵ�.
	//-------------------------------------
	bIsNowAddSummonThroughDB = false;

	bIsNowAddSoulGuardThroughDB				= false;
	dwLastSoulGuardActionClock				= 0;
	dwLastSoulGuardParentDeadMessageClock	= 0;

	// �׷� ó��
	siGroupUnique = 0 ;
	siGroupMember = 0 ;
	bGroupBossSwitch = FALSE ;


	//---------------------------------
	// ���� ���� ���� ��Ȳ
	//---------------------------------
	bSwordScroll	= false;
	bSpearScroll	= false;
	bAxeScroll		= false;
	bGunScroll		= false;
	bBowScroll		= false;
	bStaffScroll	= false;
	bCannonScroll	= false;

	//---------------------------------
	// ���� ���� ������ ��Ȳ
	//---------------------------------
	siSwordScrollNum  = 0;
	siSpearScrollNum  = 0;
	siAxeScrollNum    = 0;
	siGunScrollNum    = 0;
	siBowScrollNum    = 0;
	siStaffScrollNum  = 0;

	//--------------------------------------------
	// ������ ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetPremium = false;
	//--------------------------------------------
	// Ȱ���� ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetCombat = false;
	//--------------------------------------------
	// ����� ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetMakeItem = false;

	bIsBulletService = false;
	bIsSummonStaminaService = false;

	//--------------------------------------------
	// ĳ�� ������ �ȱ� ���� - �ߺ� �ȱ� ����
	//--------------------------------------------
	bIsBuyCashItem = false;

	//--------------------------------------------
	// ������ �ݰ� �ִ�.
	//--------------------------------------------
	bIsPickUpItem = false ;

	//-------------------------------------------------
	// ������ ������ ������ �ް� �ִ�.
	//-------------------------------------------------
	bIsItemMallGetItem = false;
	//-------------------------------------------------
	// ������ ������ ������ ��ų� ������ �ִ�.
	//-------------------------------------------------
	bIsItemMallBuyItem = false;
	//--------------------------------------
	// ���ָӴ� ������ �����
	//--------------------------------------
	bUseBagItem = false;

	//--------------------------------------
	// ���� ���� ������ �����
	//--------------------------------------
	bUseChangeHeroItem = false ;

	//-------------------------------------
	// ���� ������ ����.
	//-------------------------------------
	siVillageWarKillScore = 0;

	// APEX-
	bApexLogin = false;
	// -APEX

	// �Ƿι���-
	bTiredSystem = false;
	uiTiredState = TIRED_STATUS_NORMAL;
	dwPlayingTime = 0;
	siTiredNotice = 0;
	lastTiredNoticedovertime = 0;

	//	dwStartMiniGame = 0;

#ifdef USE_GAME_GUARD_SERVER
	m_bCCSAuthInit				= false;
	m_siCCSAuthState			= CCSAUTH_STATE_NONE;
	m_dwCCSAuthLastActionTick	= 0;
	m_siCCSAuthCount			= 0;
#endif

	uiIDNo						= 0;
	bMakeTimeReduce				= false;

	siQuestCount				= 0;
	siServerEvent_IndunScore	= 0;

	clSummonBulletUseReserveInfo.Init();

	// ĳ���� �귿 ��� ���� ����
	siCharRouletteValue			= 0;
}

cltCharServer::~cltCharServer()
{
	if(pclReservedMakeItemData)
	{
		delete pclReservedMakeItemData;
		pclReservedMakeItemData = NULL;
	}

	if(pclCI)
	{
		delete pclCI;
		pclCI = NULL;
	}

	if(pclCharListInfo)
	{
		delete[] pclCharListInfo;
		pclCharListInfo = NULL;
	}

	if( m_pclAgriculture )
	{
		delete m_pclAgriculture;
		m_pclAgriculture = NULL;
	}

	if( m_pclShopRepairData )
	{
		delete m_pclShopRepairData;
		m_pclShopRepairData = NULL;
	}
}

void cltCharServer::InitCharServer()
{
	SI32 i;

#ifdef USE_GAME_GUARD_SERVER

	//clWarpCSA.Init(this, true, true);

#endif

	LastReceveFromClientClock	=	0;
	dwLastCheckAutoFoodClock	=	0;
	dwLastGameGuardAuthClock	=	0;
	dwTimerClock				=	0;
	NPCGroup					=	-1;

	bSendSecretNCHarUniqueSwitch = false;	

	//--------------------------------
	// Daily Quest Action Clock
	//--------------------------------
	dwDailyQuestClock			= 0;
	dwDailyQuest2Clock			= 0;
	dwDailyQuest3Clock			= 0;

	//---------------------------------
	// �����ϴ� ���̶��, ���� ������
	//---------------------------------
	siFishingBaitItemUnique = 0;

	//-------------------------
	// �ڵ� ����
	//-------------------------
	bAutoFishing = false;

	//-------------------------
	// ��þƮ 100% ���� ����ġ (����϶���)
	//-------------------------
	bSuccessEnchant = false;

	//-------------------------
	// ���� �ð� üũ
	//-------------------------
	dwPreCatchFishClock	= 0;
	dwSummonFishClock = 0;

	//-------------------------
	// ��� ���̶��,  ȹ���ҷ��� ��� ������
	//-------------------------
	siFarmingItemUnique = 0;

	//-------------------------
	// ��� �Ӵ��� ����( �ӽ÷� )
	//-------------------------
	//siFarmingRentVillageUnique = 0;

	//-------------------------
	// �ڵ� ���
	//-------------------------
	bAutoFarming = false;

	//-------------------------
	// ��� ȹ�� ������ ����
	//-------------------------
	siFarmingItemCount = 0;

	//-------------------------
	// ��� �ֱ�
	//-------------------------
	dwPreGetFarmingItemClock	= 0;
	siFarmingActionCount = 0;

	//-------------------------
	// ä��
	//-------------------------
	siMiningItemUnique = 0;
	//siMiningRentVillageUnique = 0;
	bAutoMining = false;
	//siMiningItemCount = 0;
	siMiningActionCount = 0;
	dwPreGetMiningItemClock = 0;


	//dwStartAgricultureClock = 0;

	//siLeaseLandVillage = 0;
	//uiLeaseLandExpire = 0;

	if( m_pclAgriculture )		m_pclAgriculture->Init();


	//--------------------------------
	// ���� ���� ����. 
	//--------------------------------
	uiLevelInfoUpdateFrame	= 0;
	clOldLevelInfo.Init();

	clSimpleVary.Init();
	m_clCharUpdateTimer.Init();
	dwLastVisitVillageClock	= pclClient->CurrentClock;

	uiCSAAuth_Count	= 0;
	bWaitGameGuardClientAuth = false;  

	clCharWarpInfo.Init();
	siWarpUnique	= 0;

	// ���� Ŭ���̾�Ʈ�κ��� �Ϸ� ��ȣ�� ���� ���ߴ�. 
	bReadySwitch		= false;
	bFirstCellOpSwtich	= false;

	//-------------------------------------------
	// �޸� ���� - ������ �����Ѵ�.
	//-------------------------------------------
	if(IsPC(CharUnique))
	{
		if(pclCharListInfo == NULL)		pclCharListInfo = new cltCharListInfo[MAX_TOTAL_CHAR_PER_USER];
	}

	// ������Ʈ ����. 
	for(i = 0;i < MAX_UPDATE_SWITCH;i++)
	{
		clUpdateInfo[i].Init();
	}

	//-----------------------------
	// �ڵ� ȸ�� ���� 
	//-----------------------------
	dwAutoRecoverLifeClock	= pclClient->CurrentClock;
	dwAutoRecoverManaClock	= pclClient->CurrentClock;

	//-----------------------------
	// �ڵ� ȸ�� ���� 
	//-----------------------------
	dwCusionRecoverLifeClock = pclClient->CurrentClock;
	dwCusionRecoverManaClock = pclClient->CurrentClock;

	// PCK : ���ڴ� �Ŀ��� ���� ��ȯ�� �ڵ� ȸ�� ���� (09.06.15)
	dwCusionRecoverSummonLifeClock = pclClient->CurrentClock;
	dwCusionRecoverSummonManaClock = pclClient->CurrentClock;

	//KHY - 1220 - ���Žý���.
	dwTransFormRecoverManaClock = pclClient->CurrentClock;

	// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
	uiBestGuild	= false;					
	//----------------------------
	// �Ӹ��� 
	//----------------------------
	bHeadChatSwitch		= false;		// ���� �Ӹ����� �����Ǿ� �ִ°�?
	StringCchCopy(szHeadChatText, MAX_HEAD_CHAT_TEXT_SIZE, TEXT(""));

	//----------------------------
	// ���λ��� ������ 
	//----------------------------
	clShopData.Init();

	if( m_pclShopRepairData )			m_pclShopRepairData->Init();

	//----------------------------
	// ��� ���� 
	//---------------------------
	bDeathPenaltySwitch = false;
	bRequestReviveSwitch = false;
	DeathClock			= 0;
	siCharDyingDelay	= 0;


	WaitCount			= 0;
	Wait				= 0;
	RepeatNumber		= 0;

	//---------------------------
	// ĳ���� ��� 
	//---------------------------
	siCharMode			= 0;	
	siCharModePara1		= 0;


	//---------------------------
	// ��� ��� 
	//---------------------------
	siKilledMode			= 0;
	StringCchCopy(szKillMeCharName, MAX_PLAYER_NAME, TEXT(""));


	//-------------------------
	// ���ڸ����� ��� 
	//-------------------------
	bStaySwitch			= false;	// ���ڸ��� ���缭 �ִ��� ����. 

	//-------------------------
	// �÷��� �ð� ���ϱ� 
	//-------------------------
	dwStartClock		= pclClient->CurrentClock;
	dwInstantPlayTime	= 0;

	sTimeStartTime		= ((cltServer*)pclClient)->sTime;

	//-------------------------
	// �α��� ������ ����
	//-------------------------
	siLoginedLevel = 0;

	//-------------------------
	// ����ȿ��. 
	//-------------------------
	dwGeneralEftSetClock	= 0;

	//-------------------------
	// ä�� �Է� �ð� 
	//-------------------------
	dwLastChatInputClock	= 0;

	//-------------------------------------
	// ���� ���� �ð�. 
	//-------------------------------------
	dwNoFirstAttackClock	= 0;
	//-------------------------------------
	// ICE ���� �ð�. 
	//-------------------------------------
	dwNoIceAttackClock		= 0;

	//---------------------------------------
	// Ŭ���̾�Ʈ�� �޽��� ���� ������ �ð�. 
	//---------------------------------------
	dwLastSendServerClock	= 0;
	//-------------------------------------
	// ���� ���� 
	//-------------------------------------
	bMakeItemReserveSwitch	= false;
	bMakeItemMinigameSwitch = false;
	bMakeItemNoMinigame = false ;
	bEnchantItemSwitch = false;
	bMakeItemSwitch = false ;
	bFinishEnchantItemSwitch = false;
	dwMakeItemStartClock	= 0;
	dwMakeItemDelayClock	= 0;
	siMonsterCorp			= 0;
	dwStartMiniGameTime = 0;
	pclReservedMakeItemData->Init();

	// [����] ���ֵ��̹̼� : �̼� ���� ���� �ʱ�ȭ
	clGoonzuDayMissionInfo.Clear();

	// �ڴ�� ����
	bRouletteStart = false;
	m_siBettingCount = 0;
	m_siResultCount = 0;
	dwRouletteStartClock = 0;
	dwRouletteDelayClock = 0;
	SI32 x = 0, y = 0;
	for( y = 0 ; y< 5; ++y )
	{
		m_siRandomValue[y] = 0;	// ���õ� ������ 0

		for( x = 0; x < 5; ++x )
		{
			m_stRouletteTable[y][x].m_bUserSelect = false;
			m_stRouletteTable[y][x].m_bCpuSelect  = false;
			m_stRouletteTable[y][x].m_bResult     = false;

			m_stRouletteTable[y][x].m_siX = 0;
			m_stRouletteTable[y][x].m_siY = 0;
			m_stRouletteTable[y][x].m_siValue = y*5 + (x + 1);
		}
	}

	//-----------------------------------
	// ���� ��� ���� ����. 
	//-----------------------------------
	bHopeToQuitSwitch		= false;

	//----------------------------------
	// ���� �ð� ����� ���� ���� 
	//----------------------------------
	bCloseSocketSwitch		= false;
	dwCloseSocketTimer		= 0;


	//---------------------------------
	// ������ ��� ����. 
	//---------------------------------
	clItemUseReserveInfo.Init();

	//---------------------------------
	// ȯ�� ����
	//---------------------------------
	bWhisperChatReject = false;
	bPrivateTradeReject = false;
	bPartyInviteReject = false;
	bFriendAddReject = false;

	//---------------------------------
	// ���� ��ġ �浹 üũ ����. 
	//---------------------------------
	dwLastCheckCollClock	= 0;

	//---------------------------------
	// �нż� ���� �ð� : �ణ �ð��� ���ܼ� ���� ���ü� �ֵ��� �Ѵ�.
	//---------------------------------
	dwLastClone = pclClient->CurrentClock-25000;

	//-----------------------------
	// �����ؾ��� �ڸ� ����
	//-----------------------------
	siCaptionKind = CAPTIONKIND_NEWSCHAT;

	//-----------------------------
	// ��ƿ ���� 
	//-----------------------------
	dwStealClock	= 0;	
	siStealNum		= 0;			// ��ƿ�� Ƚ��. 

	//----------------------------
	// ���� ���� 
	//----------------------------
	dwPrisonClock	= 0;

	//-----------------------------------
	// ����Ʈ�� �Ϸ����� �ʾ� ������ �Ұ� ���� �ð�
	//--------------------------------------------
	dwLastLevelUpFailForQuestClock	= 0;

	//-----------------------------------
	// ���� ���� 
	//-----------------------------------
	siReservedAttackTargetID		= 0;

	//-----------------------------------
	// ��ȯ�� ó�� 
	//-----------------------------------
	bIsEvolving		= false;
	bIsTradeing		= false;

	//------------------------------------
	// WeaponSkill ó��
	//----------------------------------
	bIsWeaponSkillUp = false ;

	//----------------------------------
	// GameEvent
	//----------------------------------
	bCheckGameEvent = false ;

	//-------------------------------------
	// ���� �ŷ����̴�.
	//-------------------------------------
	bTradeServerUsing = false ;

	//-------------------------------------
	// ���� �������� �Դ� ���̴�. ���ÿ� �Ծ ������ ������� ������ �ذ��ϱ� ���� ����
	//-------------------------------------
	bIsNowAddBufThroughDB = false;

	//-------------------------------------
	// ���� �������� �̴� ���̴�. �ѹ��� ������ ������ �Ҽ� ���Բ� �ϴ� ����
	//-------------------------------------
	bNowGachaBuy = false;

	//-------------------------------------
	// ��ȯ���� ��ȯ���̹Ƿ� ���� ��ȯ���� ��ȯ��Ű�� ���ϰ� �Ѵ�.
	//-------------------------------------
	bIsNowAddSummonThroughDB = false;

	bIsNowAddSoulGuardThroughDB				= false;
	dwLastSoulGuardActionClock				= 0;
	dwLastSoulGuardParentDeadMessageClock	= 0;

	// �׷� ó��
	siGroupUnique = 0 ;
	siGroupMember = 0 ;
	bGroupBossSwitch = FALSE ;

	//---------------------------------
	// ���� ���� ���� ��Ȳ
	//---------------------------------
	bSwordScroll	= false;
	bSpearScroll	= false;
	bAxeScroll		= false;
	bGunScroll		= false;
	bBowScroll		= false;
	bStaffScroll	= false;
	bCannonScroll	= false;

	//---------------------------------
	// ���� ���� ������ ��Ȳ
	//---------------------------------
	siSwordScrollNum  = 0;
	siSpearScrollNum  = 0;
	siAxeScrollNum    = 0;
	siGunScrollNum    = 0;
	siBowScrollNum    = 0;
	siStaffScrollNum  = 0;

	//--------------------------------------------
	// ������ ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetPremium = false;
	//--------------------------------------------
	// Ȱ���� ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetCombat = false;
	//--------------------------------------------
	// ����� ���� - �ߺ��ؼ� ����� ���´�.
	//--------------------------------------------
	bIsSetMakeItem = false;

	bIsBulletService = false;
	bIsSummonStaminaService = false;

	//--------------------------------------------
	// ĳ�� ������ �ȱ� ���� - �ߺ� �ȱ� ����
	//--------------------------------------------
	bIsBuyCashItem = false;

	//--------------------------------------------
	// ������ �ݰ� �ִ�.
	//--------------------------------------------
	bIsPickUpItem = false ;

	//-------------------------------------------------
	// ������ ������ ������ �ް� �ִ�.
	//-------------------------------------------------
	bIsItemMallGetItem = false;
	//-------------------------------------------------
	// ������ ������ ������ ��ų� ������ �ִ�.
	//-------------------------------------------------
	bIsItemMallBuyItem = false;
	//--------------------------------------
	// ���ָӴ� ������ �����
	//--------------------------------------
	bUseBagItem = false;

	//--------------------------------------
	// ���� ���� ������ �����
	//--------------------------------------
	bUseChangeHeroItem = false ;

	//-------------------------------------
	// ���� ������ ����.
	//-------------------------------------
	siVillageWarKillScore	= 0;

	bMakeTimeReduce			= false;

	//-------------------------------------
	// ����Ʈ�� ī��Ʈ ������
	//-------------------------------------
	siQuestCount			= 0;

	//-------------------------------------
	// ���� �̺�Ʈ �δ� ���� ������
	//-------------------------------------
	siServerEvent_IndunScore	= 0;

	// ĳ���� �귿 ��� ���� ����
	siCharRouletteValue			= 0;

	//[����] : 2010�ų��̺�Ʈ npcī�ε� �迭�� �ʱ�ȭ
	InitArray_RemoveUnLuckQuestNPC();
}

//-------------------------------------------
// Rank ���� 
//-------------------------------------------
BOOL cltCharServer::SetRank(cltSimpleRank* pclrank)
{
	pclCI->clCharRank.Set(pclrank);
	return  TRUE;
}

//----------------------------------
// ������ �۾�.
//----------------------------------
//ĳ���Ͱ� ó�� ����� ������ ���� ��Ϲ� ���� ���� ��û�� ���� �ӹ��� �����Ѵ�. 
void cltCharServer::CellAction()
{
	// ����� ĳ������ ���, 
	// Ŭ���̾�Ʈ�� �غ���� �ʾ����� ó���� �� ����. 
	if(GetSession())
	{
		if(bReadySwitch == false)return ;
	}

	// �� ĳ���� �ϻ��� �ѹ��� �۵��ϵ��� �Ѵ�. 
	if(bFirstCellOpSwtich == true)return;
	bFirstCellOpSwtich	= true;

	// ó�� �ʿ� �����Ҷ� �ֺ��� �ڽ��� ������ �뺸�ϰ� �ֺ��� ������ ���´�
	pclClient->pclMapManager->m_world.GetMap( GetMapIndex() )->MoveChar( GetX(), GetY(), this, false );

	// ����� ������ �ִٸ� ��û�Ѵ�. 
	if(GetSession())
	{
		sDBRequest_PopLetter clMsg(GetID(), pclCI->GetPersonID());
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

		if(pclClient->siServiceArea == ConstServiceArea_Japan)
		{
			sDBRequest_PopAccountLetter clMsg2( GetID() , pclCI->clBI.szAccountID );
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg2);
		}
	}

}

// �� �������� ������ �����Ѵ�. 
void cltCharServer::DeleteInCell()
{
	ITZCell *pCurrentCell = GetCell();
	if( pCurrentCell)
	{

		ITZMap* pCellMap = (ITZMap*)pclClient->pclMapManager->m_world.GetMap(GetMapIndex());

		if(pCellMap)
		{
			// ���� �޽����� ���� ������ ��´� 
			pCellMap->SetCellList( pCurrentCell, NULL );

			// ���� �޽����� ������
			pCellMap->SendPersonLeaveMsgToOthers(this);

			// ������ ĳ���͸� �����Ѵ� 
			GetCell()->DelPerson( this );
		}
	}

}

// ���� ��ġ�� ����Ѵ�. 
void cltCharServer::CalculateRealXY()
{

	SI32 mapcenterx = 0, mapcentery = 0;
	pclMap->GetXYDotFromXYCord(GetX(), GetY(), mapcenterx, mapcentery);

	// ĳ������ �߽��� ���Ѵ�
	siCenterX = mapcenterx;

	siCenterY = mapcentery;
}

void cltCharServer::GetBottomPoint(cltPos* pclpos)
{
	pclpos->siX = siCenterX;
	pclpos->siY = siCenterY;
}

// ��ȯ���� �ڵ� ȸ���� �Ѵ�.
void cltCharServer::SummonAutoHeal()
{
	if ( ( pclClient->GetFrame() / CHAR_ACTION_INTERVAL_FRAME ) % 3 != 0 ) return ; // 10frame�� �ѹ��� �Դ´�.

	SI32 parentid = GetSummonParentID();
	SI32 ItemUnique = ITEMUNIQUE(2675) ; // ĥ����

	if ( pclCM->IsValidID( parentid ) == FALSE ) return ;
	if ( pclCM->IsAlive( parentid ) == FALSE ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[parentid];
	if ( pclchar == NULL ) return ;

	cltSummonInfo* pclSummonInfo = &pclchar->pclCI->clSummonInfo ;
	if ( pclSummonInfo == NULL ) return ;
	if ( pclSummonInfo->bAutoHeal == false ) return ;

	if ( pclCI->clIP.GetLife() <= 0 ) return ;

	if ( pclchar->pclCI->clCharItem.GetItemNum(ItemUnique) <= 0 )
	{
		ItemUnique = ITEMUNIQUE(2680) ; // ��ȭ��
	}

	if ( pclchar->pclCI->clCharItem.GetItemNum(ItemUnique) <= 0 )
	{
		ItemUnique = ITEMUNIQUE(2681) ; // ȯ���� ��ȯ�� ȸ����

		if ( pclchar->pclCI->clCharItem.GetItemNum(ItemUnique) <= 0 )
			return ;
	}

	SI32 pos = pclchar->pclCI->clCharItem.GetItemPos(ItemUnique);
	if (pos < 0) return ;

	if ( pclSummonInfo->siSelectAutoHeal < 0 || pclSummonInfo->siSelectAutoHeal > 2 ) return ;

	SI32 percent = 30 ;
	switch(pclchar->pclCI->clSummonInfo.siSelectAutoHeal)
	{
	case 0: percent = 30 ; break;
	case 1: percent = 50 ; break;
	case 2: percent = 70 ; break;
	default: return ;
	}

	if ( pclCI->clIP.GetLife() < ( clPB.GetMaxLife() * percent ) / 100 )
	{
		cltItem clitem;
		clitem.Set( &pclchar->pclCI->clCharItem.clItem[pos]);
		clitem.SetItemNum(1);
		pclchar->UseItem(clitem);

		while( pclchar->clItemUseReserveInfo.Push( clitem.siUnique, -clitem.siItemNum, pos) == false )
		{
			pclchar->ItemUseReserveAction();
		}

		cltItem *pclFrom = &pclchar->pclCI->clCharItem.clItem[ pos ];

		// Ŭ���̾�Ʈ�� ������Ʈ �� ������ ������. 
		if( pclFrom->siItemNum == 1 )
		{
			pclchar->ItemUseReserveAction();
		}
		// pclchar->ItemUseReserveAction(); �ؿ� �־�� �Ѵ�. 
		// �׷��� ������ �κ����� ��ǰ�� ��ġ�� ã�� �� ����.!
		pclFrom->Decrease(1);

		// Ŭ���̾�Ʈ�� ����� ������ ������. 
		cltGameMsgResponse_PersonItemInfo clinfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// ��ȯ���� ������ ���󰡰� �Ѵ�. 
void cltCharServer::SummonFollowHost()
{
	SI32 CharParentID = GetSummonParentID();

	if(pclCM->IsValidID(CharParentID)==FALSE)
	{
		// ������ �Ҿ���ȵ�. - �����ؾ��Ѵ�.
		return ;
	}

	if(pclCM->IsAlive(CharParentID) == false)return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[CharParentID];

	if ( pclchar->siSelectedCharIndex == GetSummonIndex() ) return ;

	SI32 length = max( TABS(pclchar->GetX() - GetX()) , TABS(pclchar->GetY() - GetY()) );
	SI32 xvary = 1 - (rand() % 3);
	SI32 yvary = 1 - (rand() % 3);
	SI32 x, y;

	bool bfollowswitch = false;

	//----------------------------------------
	// �������� �ӵ��� ���߾� �̵��ϵ��� �Ѵ�.
	//----------------------------------------
	SI32 siNewSummonSpeed = pclCM->CR[ CharParentID ]->clPB.TotalMoveSpeed;	
	if(clPB.TotalMoveSpeed!= siNewSummonSpeed )
	{
		clPB.TotalMoveSpeed = siNewSummonSpeed;
	}

	// ���� �ʿ� ������ ���󰡰� �ٸ� ������ ���� Warp�Ѵ�.
	if(GetMapIndex() == pclchar->GetMapIndex())
	{
		if(length >= 12)			// ū������ �������� warp�Ѵ�.
		{
			if(pclchar->pclMap->FindEmptyAreaForCharLocate(GetKind(), pclchar->GetX()+xvary, pclchar->GetY()+yvary, 20, &x, &y, true) == true)
			{
				cltWarpInfo clinfo(true, pclchar->GetMapIndex(), false, false, x, y, -1, -1);
				clCharWarpInfo.Set(&clinfo);
				Warp();
				bfollowswitch = true;
			}
		}
		else if(length >= 4)	// �����Ÿ��� �������� ���󰣴�.
		{
			// ����, �̵� ���̰ų� �Ÿ��� ����ġ�� Ȯ��Ǿ� �ִٸ�, 
			if(GetCurrentOrder() == ORDER_WAIT || GetCurrentOrder() == ORDER_MOVE || length >= 8)
			{
				bfollowswitch = true;
				OrderMove(siID, pclchar->GetX() + xvary, pclchar->GetY() + yvary, BY_PLAYER);

				cltOrderDataInfo clOrderDataInfo;
				if( pclClient->pclOrderManager->MakeOrder_MOVE(&clOrderDataInfo, pclchar->GetX() + xvary, pclchar->GetY() + yvary, BY_PLAYER) == true)
				{
					cltOrderData clOrderData;
					clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

					cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
					SendNeighbourMsg(&clMsg, true);
				}
			}
		}
	}
	else		// ���� �ʿ� ���� ������ warp�Ѵ�.
	{
		if(pclchar->pclMap->FindEmptyAreaForCharLocate(GetKind(), pclchar->GetX()+xvary, pclchar->GetY()+yvary, 20, &x, &y, true) == true)
		{
			bool bForceSwitch = false;

			// �ٴڿ� �̺�Ʈ Ÿ���� �ִ°����� ĳ���Ͱ� �̵��ߴٸ�, ��ȯ���� �����̵��� ��Ų��.
			cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(pclchar->GetMapIndex());
			if(pclmap != NULL)
			{
				SI32 siColl = pclmap->CalcCollInfo(pclchar->GetX(), pclchar->GetY());
				if(siColl & BLOCK_EVENT4 || siColl & BLOCK_EVENT5 || siColl & BLOCK_EVENT6)
				{
					bForceSwitch = true;
					x = pclchar->GetX();
					y = pclchar->GetY();
				}
			}

			// �̵�
			cltWarpInfo clinfo(true, pclchar->GetMapIndex(), bForceSwitch, false, x, y, -1, -1);
			clCharWarpInfo.Set(&clinfo);
			Warp();
			bfollowswitch = true;
		}
	}

	// ������ ���󰡴� ��尡 �ƴ϶��. 
	if(bfollowswitch == false)
	{
		// ��ȯ���� ���ݼӼ��� �������� �����̾�߸� ������ �����Ѵ�.
		cltSummon	*pclSummon = pclchar->pclCI->clSummonInfo.GetSummon( GetSummonIndex() );
		if(pclSummon && pclSummon->siAttackType == SUMMON_ATTACKTYPE_ACTIVE)
		{
			// ������ �������� ĳ���Ͱ� �ִٸ�, �����Ѵ� 
			SI16 attackid = pclCM->CR[CharParentID]->clAttackTarget.GetAttackID();
			// Ÿ���� ������ �־�� �ϸ�, 
			if(pclCM->IsAlive(attackid))
			{
				// ���� ��ȯ���� �������̸� ���� ������ ���� ������ �ʿ�� ����.(��ȿ����)
				if(GetCurrentOrder() != ORDER_ATTACK)
				{
					// ��ȯ�� �ڽ��� ������ �־�� �ϰ� ���� �������� ���� ���ο� ���� ��ġ���� �ʾƾ� �Ѵ�. 
					if(pclCM->IsAlive(GetID()) == TRUE && clAttackTarget.GetAttackID() != attackid)
					{
						// �������� �����ϵ��� �Ѵ�. 
						OrderAttack(GetID(), attackid, BY_PLAYER);

						cltOrderDataInfo clOrderDataInfo;
						if( pclClient->pclOrderManager->MakeOrder_ATTACK(&clOrderDataInfo, attackid, BY_PLAYER) == true)
						{
							cltOrderData clOrderData;
							clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

							cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
							SendNeighbourMsg(&clMsg, true);
						}
					}
				}

			}
		}
	}

}

void cltCharServer::SoulGuardFollowHost( void )
{
	if ( FALSE == pclCM->IsAlive(GetID()) )
	{
		return;
	}

	SI32 siParentCharID = GetSoulGuardParentCharID();
	if ( false == pclCM->IsValidID(siParentCharID) )
	{
		// ���� ���̵� �̻��ϴ� �̶��� ??
		if ( 0 < GetLife() )
		{
			SetLife( 0 ); // �������� 0���� �����
		}
		
		return;
	}

	// �׾����� ����
	if ( FALSE == pclCM->IsAlive(siParentCharID) )
	{
		// ��ȣ���� ü�� �޽����� ������
		SendSoulGuardParentDeadMessage();

		return;
	}

	cltCharServer* pclParent = pclCM->GetCharServer( siParentCharID );
	if ( NULL == pclParent )
	{
		return;
	}

	SI32 siParentX		= pclParent->GetX();
	SI32 siParentY		= pclParent->GetY();

	SI32 siXVary		= 1 - (rand() % 3);
	SI32 siYVary		= 1 - (rand() % 3);

	SI32 siParentAddX	= siParentX + siXVary;
	SI32 siParentAddY	= siParentY + siYVary;

	SI32 siLength		= max( TABS(siParentX - GetX()) , TABS(siParentY - GetY()) );

	SI32 siX			= 0;
	SI32 siY			= 0;

	//----------------------------------------
	// �������� �ӵ��� ���߾� �̵��ϵ��� �Ѵ�.
	//----------------------------------------
	SI32 siHostSpeed = pclParent->clPB.TotalMoveSpeed;	
	if ( siHostSpeed != clPB.TotalMoveSpeed )
	{
		clPB.TotalMoveSpeed = siHostSpeed;
	}

	// ���� �ʿ� ���� ���
	if ( GetMapIndex() == pclParent->GetMapIndex() )
	{
		// �Ÿ��� �ʹ� �ִٸ� ĳ���Ϳ��� ������Ų��
		if ( 12 <= siLength )
		{
			if ( true == pclParent->pclMap->FindEmptyAreaForCharLocate(GetKind(), siParentAddX, siParentAddY, 20, &siX, &siY, true) == true)
			{
				cltWarpInfo clinfo(true, pclParent->GetMapIndex(), false, false, siX, siY, -1, -1);
				clCharWarpInfo.Set(&clinfo);
				Warp();
			}
		}
		// �����Ÿ� �̻��� �׳� ���󰣴�
		else if ( 2 <= siLength )
		{
			OrderMove( GetID(), siParentAddX, siParentAddY, BY_PLAYER );

			cltOrderDataInfo clOrderDataInfo;
			if( pclClient->pclOrderManager->MakeOrder_MOVE(&clOrderDataInfo, siParentAddX, siParentAddY, BY_PLAYER) == true)
			{
				cltOrderData clOrderData;
				clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

				cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
				SendNeighbourMsg(&clMsg, true);
			}
		}
	}
	// �ٸ� ���� ���
	else
	{
		if ( true == pclParent->pclMap->FindEmptyAreaForCharLocate(GetKind(), siParentAddX, siParentAddY, 20, &siX, &siY, true) )
		{
			bool bForceSwitch = false;

			// �ٴڿ� �̺�Ʈ Ÿ���� �ִ°����� ĳ���Ͱ� �̵��ߴٸ�
			cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer( pclParent->GetMapIndex() );
			if(pclmap != NULL)
			{
				SI32 siColl = pclmap->CalcCollInfo( siParentX, siParentY );
				if(siColl & BLOCK_EVENT4 || siColl & BLOCK_EVENT5 || siColl & BLOCK_EVENT6)
				{
					bForceSwitch = true;
					siX = siParentX;
					siY = siParentY;
				}
			}

			// �̵�
			cltWarpInfo clinfo( true, pclParent->GetMapIndex(), bForceSwitch, false, siX, siY, -1, -1 );
			clCharWarpInfo.Set(&clinfo);
			Warp();
		}

	}

}

void cltCharServer::CharServerAction_NPC()
{
	if(GetMana() < clPB.GetMaxMana() && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)
	{
		pclCI->clIP.IncreaseMana(2);
	}


	//=======================================================
	// LIFE TIMER
	//=======================================================
	// ���� �ð��� ������ �ִ� ĳ������ ���, �ð� ����� ���. 
	if(dwTimerClock)
	{
		SI32 NpcKind = GetKind()	;
		// 30�� �� ������ �ְ�.
		SI32 siMonsterLifeTime = 30000;

		if( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) )
		{
			siMonsterLifeTime = 30000;	
		}
		// [�߰� : ���� ���ϵ� ��� �׵��� �Ѵ�. Ȳ���� 2007. 9. 18]
		else if( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDBOARBOSS")))
		{
			siMonsterLifeTime = 1800000;	
		}
		// [�߰� : �߼� ������ �̺�Ʈ. ĥ������ 3�ð� �Ŀ� �׵��� �Ѵ�. Ȳ���� 2007. 11. 12]
		else if(  NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_TURKEY")))
		{
			siMonsterLifeTime = 10800000;	
		}
		// [���� �̺�Ʈ : 2008.01.10 �迵�� �߰� - ī��Ʈ���� 50�е� �״´�]
		else if( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_KITERACCOON")) )
		{
			SYSTEMTIME sTime;
			GetLocalTime( &sTime );

			if ( pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_RaccoonTime"), &sTime ) == true )
			{
				siMonsterLifeTime = 3000000;
			}
		}
		// �����ũ �̺�Ʈ : ���̳븮�� �йи��� 50�е��� ����ִ´�.
		else if( NpcKind == pclClient->pclKoinoboryFamilyManager->GetKoinoboryRedKind() ||
			NpcKind == pclClient->pclKoinoboryFamilyManager->GetKoinoboryBlueKind() ||
			NpcKind == pclClient->pclKoinoboryFamilyManager->GetKoinoboryYellowKind() )
		{
			siMonsterLifeTime = 50 * 60 * 1000;
		}
		// [����] �߼� �̺�Ʈ. ��, �����䳢 30�е��ȸ� ���. => 2008-8-7
		else if (	 NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_MOON"))	
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_COSMOSRABBIT2")))
		{
			siMonsterLifeTime = 30 * 60 * 1000;
		}
		else if( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENPC")) )// �ҷ��� �̺�Ʈ NPC ���ͼ� 30������ �����
		{
			siMonsterLifeTime = 30 * 60 * 1000;
		}
		else if ( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_QUESTEMPERORMASTER")) ) // ����Ʈ Ȳ���� ��ȥ(60���� �����)
		{
			siMonsterLifeTime = 3600000; //60 * 60 * 1000;
		}
		// ����ã�� �̺�Ʈ �� ���� ���Ǿ� 1�е� �׿� ������.
		else if ( NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGHIRO")) 
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGJEAN")) 
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGMERIEL"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGCLARA"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGJEAN"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGCLARA"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGMERIEL"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))
			|| NpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))		) 
		{
			siMonsterLifeTime = 10000;
		}
		else
		{
			siMonsterLifeTime = 30000;
		}

		if ( clIdentity.siIdentity == IDENTITY_TREASURE_EVENT )	// ���� Ʈ���� �̺�Ʈ �����̰� ���� ���Ͱ� �ƴ� ��쿡�� ��� ���� �ð� 10��
		{
			if ( NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGHIRO")) 
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGJEAN")) 
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGMERIEL"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGCLARA"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGJEAN"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGCLARA"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGMERIEL"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))
				|| NpcKind != pclClient->GetUniqueFromHash(TEXT("KIND_BLESSINGSPARKY"))	)
			{
				siMonsterLifeTime = 10 * 60 * 1000;	// 10�и� ��� �ְ� �Ѵ�.
			}
		}
		else if ( clIdentity.siIdentity == IDENTITY_SOULGUARD_SUMMONPUMKIN )	// �����̻��� ��ȯ�� �� 20�ʸ� ��� �ִ´�
		{
			siMonsterLifeTime = 20 * 1000;	// 10�и� ��� �ְ� �Ѵ�.
		}
		// ���� ���ϱ� �̺�Ʈ �÷����ҋ� �ƴϸ� �ش� ���͵��� �� �׿� ������ ���ؼ� 
		else if ( clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT )	// �����̻��� ��ȯ�� �� 20�ʸ� ��� �ִ´�
		{
			if(pclClient->pclRacoonDodgeEventMgr->GetStep() != EVENTSTEP_PLAY )
			{
				siMonsterLifeTime= 1	;
			}
			else
			{
				siMonsterLifeTime = 1000000 ;	// �ٷ� ���δ�
			}
		}

		
		if(TABS(pclClient->CurrentClock - dwTimerClock) >= siMonsterLifeTime)
		{
			cltGameMsgResponse_IncLife clInfo(GetCharUnique(), 0, 0, 0, 0, INCLIFE_REASON_TIMEOUT);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
			SendNeighbourMsg(&clMsg, true);

			SetLife(0);
		}
	}

	//=======================================================
	// SELF CLONE
	//=======================================================
	// ������ �н��ϴ� ���̶�� �ð��� �����Ǿ� �ִٸ� 
	if(IsAtb(ATB_CLONE) && clInnerStatus.IsStatus(INNERSTATUS_CLONE) == false && clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)
	{
		// 60 �ʰ������� �н��Ѵ�.  (45 ~ 74�� ���̿� )
		if(TABS(pclClient->CurrentClock - dwLastClone) >= (45000+rand()%30))
		{

			SI32 siCloneX, siCloneY;
			cltServer* pclserver = (cltServer*)pclClient;
			if(pclMap->FindEmptyAreaForCharLocate(GetKind(), GetX(), GetY(), 20, &siCloneX, &siCloneY, true) == true)
			{
				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 level = pclCI->clIP.GetLevel();
				//if ( level <= 0 || level > MAX_LEVEL ) level = 1 ;
				if ( level <= 0 ) level = 1 ;

				SI32 siCloneChar = pclserver->SetNPCChar(GetKind(), pclCI->clBI.siHomeVillage, NULL, level, 0, siCloneX, siCloneY, GetMapIndex(), NPCITEMMODE_NONE, &clinnerstatus );
				if(pclCM->IsValidID(siCloneChar))
				{
					// ������ �����ð��� ���� ������ �ð��� �����Ѵ�.
					((cltCharServer*)(pclCM->CR[ siCloneChar ]))->dwTimerClock = pclserver->CurrentClock;
					((cltCharServer*)(pclCM->CR[ siCloneChar ]))->clInnerStatus.SetStatus(INNERSTATUS_CLONE);

					dwLastClone = pclClient->CurrentClock;
				}
			}
		}
	}
}

void cltCharServer::CharServerAction_Summon()
{
	cltCharServer* pclParentChar = pclCM->GetCharServer( GetSummonParentID() );
	if( pclParentChar )
	{
		if (TABS(pclClient->GetFrame() - uiStaminaFrame) > 1200)
		{
			SI32 amt = 0;

			if (pclKI->siSummonRank > 0)
			{
				// PCK : ATB_SUMMONSTAMINASTATIC�� ������ �⺻ ��ȯ�� ���¹̳ʰ��ҷ��� �����Ų��. (08.07.18)
				cltKindInfo* pclKindInfo = NULL;
				if( pclClient->pclKindInfoSet->IsValidKind(GetKind()) )	{	pclKindInfo = pclClient->pclKindInfoSet->pclSummonKI[GetKind()];	}				

				if (pclKindInfo && pclKindInfo->IsAtb(ATB_SUMMONSTAMINASTATIC) == TRUE)
				{
					amt = SUMMON_DECREASESTAMINA_AMT;
				}
				else
				{
					// �Ҹ�Ǵ� ���¹̳��� ���� ���Ѵ�.
					amt =  SUMMON_DECREASESTAMINA_AMT - (pclKI->siSummonRank / 5) ;

					// �Ҹ�Ǵ� ���¹̳��� ���� 0���� �۾� ���� 1�� ������Ų��.
					if(amt <= 0) { amt = 1; }
				}
			}

			if (clPB.pclIP->GetLuk() > amt)
			{
				// cyj ���� �ð���ŭ ���¹̳ʸ� ����ش� 
				// tys ���� �ȿ� �ְų� �溹�� �ȿ� ������ ���¹̳� ���� ����
				// ��ȯ�� ���׹̳� ���� �̿�� �� ��
				if( pclParentChar->GetCurrentVillageUnique() > 0					|| 
					pclParentChar->GetMapIndex() == MAPINDEX_PALACE					||
					pclParentChar->GetMapIndex() == MAPINDEX_TRADINGPORT			||
					pclParentChar->GetMapIndex() == MAPINDEX_MAINFIELD				||
					pclParentChar->GetMapIndex() == MAPINDEX_PRIVATEMARKETAREA		||	// [����] ���λ������� : ��ȯ�� ���׹̳� ���� ����
					pclParentChar->GetMapIndex() == MAPINDEX_PARTYHALL_1			||	// ��ƼȦ ���������� ��ȯ�� ���׹̳� ���� ����
					pclParentChar->GetMapIndex() == MAPINDEX_PARTYHALL_2			||
					pclParentChar->GetMapIndex() == MAPINDEX_PARTYHALL_3			||
					pclParentChar->GetMapIndex() == MAPINDEX_PARTYHALL_4			||
					pclParentChar->GetMapIndex() == MAPINDEX_PARTYHALL_5			||
					pclParentChar->GetMapIndex() == MAPINDEX_FESTIVAL				||
					pclParentChar->pclCI->clBI.bSummonStaminaService				||
					pclParentChar->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER )	)
				{
					//if( false == pclParentChar->pclCI->clBI.bSummonStaminaService )
					{
						// [����] ��ȯ�� ����. 
						if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) ) 
						{
							// ������ �ߴٸ�
							if( pclParentChar->pclCI->clSummonInfo.IsSummonTranform( GetSummonIndex() ) )
							{
								TransformSummonStaminaDown( pclParentChar, amt );
							}
						}
					}
				}
				else
				{
					// [����] ��ȯ�� ����.
					if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) ) 
					{
						// ������ �ߴٸ�
                        if( pclParentChar->pclCI->clSummonInfo.IsSummonTranform( GetSummonIndex() ) )
						{
							TransformSummonStaminaDown( pclParentChar, amt );

							amt = 0;	// �⺻ ���׹̳ʴ� ���� �ʴ´�.
						}
						else
						{
							TransformSummonStaminaUp( pclParentChar, amt );
						}
					}					
					
					// �⺻ ���׹̳�.
					clPB.clTotalBA.SetLuk( clPB.pclIP->DecreaseLuk(amt) );
				}
			}
			else
			{
				clPB.pclIP->SetLuk( 0 );
				clPB.clTotalBA.SetLuk( clPB.pclIP->GetLuk() );

				if( pclParentChar->GetLife() > 0 )
				{
					cltServer *pclserver = (cltServer*)pclClient;

					cltSummon *pclSummon = pclParentChar->pclCI->clSummonInfo.GetSummon(GetSummonIndex());

					pclSummon->siTakeInMinute = pclClient->GetMinuteFromsTime(pclserver->sTime);

					//---------------------------------------------
					// DB�� ��ȯ���� �����Ѵ�.
					//---------------------------------------------
					cltSummon clSummon;
					pclParentChar->SendDBSummon(SETSUMMON_REASON_UPDATE, GetSummonIndex());

					clSummon.Set(pclSummon);
					clSummon.siStatus = SUMMON_STATUS_TAKEIN;

					pclParentChar->SendDBSummon(SETSUMMON_REASON_TAKEIN, GetSummonIndex(), &clSummon);

					((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMON_WASTEALLSTAMINA,  0, 0, GetSummonParentID() );

					cltGameMsgResponse_ServerTime clTime(pclserver->sTime);
					cltMsg clMsg(GAMEMSG_RESPONSE_SERVERTIME, sizeof(clTime), (BYTE*)&clTime);

					pclParentChar->SendNetMsg((sPacketHeader*)&clMsg);

				}
			}

			// ��ȯ����� ������Ʈ �����ش�.
			if( pclParentChar->GetLife() > 0 )
			{
				pclParentChar->SetUpdateSwitch(UPDATE_SUMMON, true, 0);
			}

			uiStaminaFrame = pclClient->GetFrame();

		}

		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_CRYSTALITEM_DB].IsTimed(GetTickCount()) ) // 30��.
		{
			SendDBCrystalDurabilityForSummon();
		}

		//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü ������ client�� ����   - �ϴ� 5��. - �ȿ��� 1% �϶��� ���������
		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_CRYSTALITEMINFO_CLIENT].IsTimed(GetTickCount()) ) // 5��.
		{			
			SendClientCrystalDurabilityForSummon();
		}			


	}

}

void cltCharServer::CharServerAction_SoulGuard()
{
	// 1�� �������� �ȴ�
	if ( TABS(pclClient->CurrentClock-dwLastSoulGuardActionClock) < 60000 )
	{
		return;
	}

	dwLastSoulGuardActionClock = pclClient->CurrentClock;

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	SI32 siParentCharID = GetSoulGuardParentCharID();
	if ( false == pclCM->IsValidID(siParentCharID) )
	{
		return;
	}

	// �׾����� ����
	if ( FALSE == pclCM->IsAlive(siParentCharID) )
	{
		return;
	}

	cltCharServer* pclParent = pclCM->GetCharServer( siParentCharID );
	if ( NULL == pclParent )
	{
		return;
	}

	//---------------------------------------------------------------
	// ���� üũ�� ���� ��ȣ������ ���ش�
	//---------------------------------------------------------------

	SI32 siGetLimitDateVary = pclParent->pclSoulGuard->GetLimitDateVary();
	
	// ���� �����̴� �׳� ����
	if ( siGetLimitDateVary == MAX_SI32 )
	{
		return;
	}

	// ���� ������ �����ִٸ� �׳� ����
	if ( siGetLimitDateVary > pclserver->pclTime->clDate.GetDateVary() )
	{
		return;
	}

	///////////////////////////////////////////////////////////////////
	// ��ȣ������ ����
	///////////////////////////////////////////////////////////////////

	// �̹� ��ȣ���� �״� ó���� �ߴٸ� ����
	if ( SOULGUARD_STATUS_DEAD == pclParent->pclSoulGuard->GetStatus() )
	{
		return;
	}

	// �켱 �׾��ٰ� ������ �ְ�
	pclParent->pclSoulGuard->SetStatus( SOULGUARD_STATUS_DEAD );

	// �������� 0���� �����ؼ� �״� ����� ��Ÿ���� �Ѵ�
	SetLife( 0 );

	// �̿��鿡�� �״´ٴ� ü�� �޽����� ������
	NTCHARString128	strMessage	= GetTxtFromMgr(30248);
	NTCHARString64	strName		= GetTxtFromMgr(30146);

	cltGameMsgResponse_Chat clChat( GetCharUnique(), CHATMODE_NORMAL, strName, strMessage );
	cltMsg clChatMsg(GAMEMSG_RESPONSE_CHAT, clChat.siDataSize, (BYTE*)&clChat);
	SendNeighbourMsg(&clChatMsg, true);

	cltGameMsgResponse_IncLife clLifeInfo(GetCharUnique(), 0, 0, 0, 0, INCLIFE_REASON_TIMEOUT);
	cltMsg clLifeMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clLifeInfo), (BYTE*)&clLifeInfo);
	SendNeighbourMsg(&clLifeMsg, true);

	// DB�� �׾��ٰ� ������
	sDBRequest_SoulGuard_Dead clMsg( pclParent->pclCI->GetPersonID(), pclParent->pclSoulGuard->GetSoulGuardID(), SOULGUARD_DEAD_MODE_DEAD );
	pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

}


void cltCharServer::CharServerAction_PC()
{

	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;


	// 1�ʸ��� ���´�.
	if(TABS(pclClient->CurrentClock - clSimpleVary.dwLastCheckCharServerActionPCClock) > 1000)
	{
		clSimpleVary.dwLastCheckCharServerActionPCClock = pclClient->CurrentClock;
		CalcPlayTime();

#ifdef USE_GAME_GUARD_SERVER

		CCSAuthRun();

#endif

		// 30�ʿ� �ѹ��� ���� ĳ���� üũ�� �Ѵ�
		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_BUGCHAR_CHECK].IsTimed(GetTickCount()) )
		{
			if ( 0 < pclCI->GetPersonID() )	// ĳ���� ���ÿ� ��� ĳ���ʹ� ������ PID�� 0�̴�
			{
				if ( (GetID() != pclCM->GetIDFromName((TCHAR*)GetName()))
					|| (GetID() != pclCM->GetIDFromPersonID(pclCI->GetPersonID()))
				)
				{
					if ( false == bCloseSocketSwitch )
					{
						pclserver->pclLog->FilePrint( TEXT("Config\\BugChar.txt"), TEXT("PersonID:%d"), pclCI->GetPersonID() );

						bCloseSocketSwitch = true;
						dwCloseSocketTimer = 5;
					}
				}
			}	// if ( 0 < pclCI->GetPersonID() ) ����
		}	// if( m_clCharUpdateTimer[CHAR_UPDATETIMER_BUGCHAR_CHECK].IsTimed(GetTickCount()) ) ����

		// ��� ����Ʈ ����
		if (TABS(pclClient->CurrentClock - clSimpleVary.dwGuildPointClock) > 3600000 ) // 60�� ( 3600�� )
		{
			if ( pclCI->clBI.clGuildRank.IsGuildMember() == true && clSimpleVary.dwGuildPointClock != 0 )
			{
				cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
				if ( pGuild )
				{
					sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCI->GetPersonID(),1,TYPE_GUILDPOINT_ADD_PLAYTIME,150);
					pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}

			clSimpleVary.dwGuildPointClock = pclClient->CurrentClock;
		}
		// ������ �ý��� ó�� - by LEEKH 2007.12.27
		cltTimeMeterRateInfo * pclTimeMeterRateInfo = &pclCI->clBI.clTimeMeterRateInfo;

		// �ð� ó��
		for( SI32 i=0; i < MAX_TIMEMETERRATE; i++ )
		{
			cltTimeMeterRate*	pclTimeMeterRate = &pclTimeMeterRateInfo->m_clTimeMeterRate[i];
			if( pclTimeMeterRate->IsUse() )
			{
				//				if( TABS(pclClient->CurrentClock - pclTimeMeterRate->dwStartedTime) >= 1000 )
				{
					//pclTimeMeterRate->dwStartedTime = pclClient->CurrentClock;

					SI32	siUseClock = TABS(pclClient->CurrentClock - pclTimeMeterRate->dwStartedTime);
					SI32	siUseSecond = siUseClock / 1000;
					SI32	siLeftClock = siUseClock % 1000;

					// 1000���� ������ �������� �ʴ� Ŭ������ ���� ��꿡 �����ϱ� ���� ������ ���� �����Ѵ�.
					pclTimeMeterRate->dwStartedTime = pclClient->CurrentClock - siLeftClock;

					SI32	siUseTime = pclTimeMeterRateInfo->m_siUseTimePerSec * siUseSecond;
					if( pclTimeMeterRate->siLeftTimeSec < siUseTime)		siUseTime = pclTimeMeterRate->siLeftTimeSec;
					if(siUseTime > 0)		pclTimeMeterRate->UseTime( siUseTime );

					// �ð��� ��� �Һ�������, DB�� �����Ѵ�. ����� ���� ��Ų��.
					if( pclTimeMeterRate->siLeftTimeSec <= 0)
					{
						sDBRequest_TimeMeterRate_UseTime clMsg(GetCharID(), pclCI->GetPersonID(), pclTimeMeterRate->siTimeMeterRateType, pclTimeMeterRate->siUseTimeSec, true );
						pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						// ���� �ð��� �������� �ش�.
						if( clTransFormInfo.bTransFormMode )
						{
							// DB�� ���� �ð��� �߰����ش�.
							sDBRequest_Transform_AddTime clMsg1(GetCharID(), pclCI->GetPersonID(), clTransFormInfo.uiNowTransFormKind, pclTimeMeterRate->siUseTimeSec );
							pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg1);
						}

						pclTimeMeterRateInfo->UseEnd( pclTimeMeterRate->siTimeMeterRateType );

					}

					// ������ ����ó���� ���� �ΰ����� ������ ����
					switch(pclTimeMeterRate->siTimeMeterRateType)
					{
					case TIMEMETERRATE_TRANSFORM:		// ����
						{
							UI16 tKind = clTransFormInfo.uiNowTransFormKind;
							SI32 siBeforeSync = (SI32)clTransFormInfo.GetSynchroTransForm( tKind );
							// �ش� ������ ���ð��� ���Ѵ�.
							clTransFormInfo.AddTransFormTime(tKind, siUseTime);
							SI32 siCurSync = (SI32)clTransFormInfo.GetSynchroTransForm( tKind );

							if( siCurSync == 100 &&
								siBeforeSync < siCurSync )
							{
								// ��ũ�� 100�� �޼��� ��� ��ǰ �˸�â�� ����.
								cltItem clItem;		clItem.Init();

								SI32 siItemUnique = GetItemUniqueFromCharKind(clTransFormInfo.uiNowTransFormKind);
								if(siItemUnique > 0)
								{
									SI16 brareswitch = 0;
									if( pclClient->pclItemManager->MakeRandItemUnique( siItemUnique, &clItem, 0, 0, &brareswitch,0,0,0,0,0,0) == true )
									{
										clItem.SetItemNum(1);
										cltGameMsgResponse_MaxSyncro_Attainment clMaxSyncro( &clItem, clTransFormInfo.uiNowTransFormKind, pclCI->clBI.GetName() );
										cltMsg clMsg( GAMEMSG_RESPONSE_MAXSYNCRO_ATTAINMENT, sizeof(clMaxSyncro), (BYTE*)&clMaxSyncro );
										pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
									}
								}

							}

							// ��ũ���� ����Ǿ����Ƿ�, Ŭ���̾�Ʈ�� �ѹ� �����Ѵ�.
							if( siCurSync != siBeforeSync )
							{
								// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
								cltGameMsgResponse_SetTransForm	clTrasFormInfo( &clTransFormInfo );
								cltMsg clMsg( GAMEMSG_RESPONSE_TRANSFORM, sizeof(cltTransFormInfo), (BYTE*)&clTrasFormInfo );
								SendNetMsg( (sPacketHeader*)&clMsg );			
							}

							// ���� 100% ����Ʈ�� �׷��ش�.
							if( siCurSync >= 100 )
							{
								cltCharChangeStatusInfo clchangeinfo(CHANGETYPE_TRANSFORM100EFFECT, true);
								cltGameMsgResponse_StatusInfo clinfo(GetID(), &clchangeinfo);
								cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clinfo), (BYTE*)&clinfo);
								SendNeighbourMsg(&clMsg, true);					
							}

							// ������ �ð��� ������, ������ �����Ų��.
							if( pclTimeMeterRate->siLeftTimeSec <= 0)
							{
								cltCharServer* pclchar = (cltCharServer*)pclCM->CR[GetID()];
								cltItem *pclItemList = pclCM->CR[GetID()]->pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
								cltPSkillInfo *pclskillinfo = &pclchar->pclCI->clCharSkill;
								// ���� ����
								CharTransform( 0 ,pclchar->ActiveWeapon , pclItemList ,pclskillinfo);
							}
						}
						break;
					case TIMEMETERRATE_MOTORCYCLE:		// �������.
						{
							// ���Ͱ� ����Ǿ����Ƿ�, Ŭ���̾�Ʈ�� �ѹ� �����Ѵ�.
							// ��ü������ ���� �ð������� ��������� �׷��ʿ䰡 ����.

							// ������ �ð��� ������, ������̿��� ������ �Ѵ�.
							if( pclTimeMeterRate->siLeftTimeSec <= 0)
							{
								cltCharServer* pclchar = (cltCharServer*)pclCM->CR[GetID()];
								SI16 showHorseIndex = pclchar->pclCI->clHorseInfo.GetCurrentShowHorseIndex();

								pclchar->UnrideHorse();

								// DB�� ���� ������ �˸���. 
								sDBRequest_SetRideHorse cldbmsg(GetCharID(), pclCI->GetPersonID(), showHorseIndex , false);
								pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&cldbmsg);

							}
						}
						break;
					}
				}
			}
		}

		// ���� �������� DB�� ������ ������ �����Ѵ�. - by LEEKH 2007.12.27
		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_TIMEMETERRATE_DB].IsTimed(GetTickCount()) )
		{
			for( SI32 i=0; i < MAX_TIMEMETERRATE; i++ )
			{
				cltTimeMeterRate*	pclTimeMeterRate = &pclTimeMeterRateInfo->m_clTimeMeterRate[i];
				if( pclTimeMeterRate->IsUse() )
				{
					bool	bUpdate = false;

					if( pclTimeMeterRate->siLeftTimeSec <= 0)		bUpdate = true;

					if( pclTimeMeterRate->siUseTimeSec > 0)
					{
						// DB �� ��뷮�� �����Ѵ�.
						sDBRequest_TimeMeterRate_UseTime clMsg(GetCharID(), pclCI->GetPersonID(), pclTimeMeterRate->siTimeMeterRateType, pclTimeMeterRate->siUseTimeSec, bUpdate );
						pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}

					// ������ ���񽺿� ���� ������ ó��
					switch(pclTimeMeterRate->siTimeMeterRateType)
					{
					case TIMEMETERRATE_TRANSFORM:		// ����
						{
							if( pclTimeMeterRate->siUseTimeSec > 0)
							{
								if( clTransFormInfo.bTransFormMode )
								{
									// DB�� ���� �ð��� �߰����ش�.
									sDBRequest_Transform_AddTime clMsg1(GetCharID(), pclCI->GetPersonID(), clTransFormInfo.uiNowTransFormKind, pclTimeMeterRate->siUseTimeSec );
									pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg1);
								}
							}
						}
						break;
					}

					// ���� DB����� ���� ��뷮 �ʱ�ȭ
					pclTimeMeterRate->siUseTimeSec = 0;
				}
			}
		}

		// ���� �������� Ŭ���̾�Ʈ�� ������ ������ �����Ѵ�. - by LEEKH 2007.12.27
		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_TIMEMETERRATE_CLIENT].IsTimed(GetTickCount()) )
		{
			for( SI32 i=0; i < MAX_TIMEMETERRATE; i++ )
			{
				cltTimeMeterRate*	pclTimeMeterRate = &pclTimeMeterRateInfo->m_clTimeMeterRate[i];
				if( pclTimeMeterRate->IsUse() )
				{
					// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
					cltGameResponse_TimeMeterRate_Update	clTimeMeterRate( pclTimeMeterRate );
					cltMsg clMsg( GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE, sizeof(clTimeMeterRate), (BYTE*)&clTimeMeterRate );
					SendNetMsg( (sPacketHeader*)&clMsg );
				}
			}
		}

		// ���� �������� Ŭ���̾�Ʈ�� ���� ������ ������.
		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_TRANSFORMINFO_CLIENT].IsTimed(GetTickCount()) )
		{
			if(clTransFormInfo.bTransFormMode == true)
			{
				// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
				cltGameMsgResponse_SetTransForm	clTrasFormInfo( &clTransFormInfo );
				cltMsg clMsg( GAMEMSG_RESPONSE_TRANSFORM, sizeof(cltTransFormInfo), (BYTE*)&clTrasFormInfo );
				SendNetMsg( (sPacketHeader*)&clMsg );			
			}
		}

		// �Ƿε� �ý���  - Ŭ���̾�Ʈ�� ������ �ð��� ����, ������ ������.
		if(true == bTiredSystem )
		{
			if( m_clCharUpdateTimer[CHAR_UPDATETIMER_TIREDSYSTEM_TIME_LIMIT].IsTimed(GetTickCount()) ) // 20��
			{
				SI16 NowTiredNotice = 0;
				SI32 playMinute = dwPlayingTime / 60;

				switch(playMinute)
				{
				case 60: // 1 �ð�.
					NowTiredNotice = TIRED_NOTICE_1_HOUR;
					break;
				case 120: // 2 �ð�.
					NowTiredNotice = TIRED_NOTICE_2_HOUR;
					break;
				case 150: // 2 �ð� 30��
					NowTiredNotice = TIRED_NOTICE_2_5_HOUR;
					break;
				case 180: // 3 �ð�.
					{
						uiTiredState = TIRED_STATUS_LITTLE_TIRED;
						NowTiredNotice = TIRED_NOTICE_3_HOUR;
					}
					break;
				case 210: // 3 �ð� 30��
					NowTiredNotice = TIRED_NOTICE_3_5_HOUR;
					break;
				case 240: // 4 �ð�.
					NowTiredNotice = TIRED_NOTICE_4_HOUR;
					break;
				case 270: // 4 �ð� 30��
					NowTiredNotice = TIRED_NOTICE_4_5_HOUR;
					break;
				case 300: // 5 �ð�.
					{
						uiTiredState = TIRED_STATUS_MUCH_TIRED;
						NowTiredNotice = TIRED_NOTICE_5_HOUR;
					}
					break;
				default :
					{
						// 5�ð� �̻��� ��쿡 ���� ���� ó��.
						if(playMinute > 300)
						{
							SI32 overtime = (playMinute - 300)%15;
							SI32 dovertime = ((playMinute - 300)/15)+1;

							if((overtime == 0) && (lastTiredNoticedovertime  < dovertime)) // 15�п� �ѹ���
							{
								uiTiredState = TIRED_STATUS_MUCH_TIRED;
								NowTiredNotice = TIRED_NOTICE_5_OVER_HOUR;

								lastTiredNoticedovertime = dovertime;
							}
						}						
					}
				}

				// Ŭ���̾�Ʈ�� �˷��� �Ҷ��� �˸���.
				if(((NowTiredNotice > 0) && (NowTiredNotice !=siTiredNotice))||(NowTiredNotice == TIRED_NOTICE_5_OVER_HOUR))
				{
					// ���� ������� ������ ���´�.
					siTiredNotice = NowTiredNotice;

					cltGameMsgResponse_TiredStateNotice clInfo( uiTiredState, siTiredNotice ,GetName());
					cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS, sizeof(clInfo), (BYTE*)&clInfo );
					SendNetMsg( (sPacketHeader*)&clMsg );
				}			
			}			
		}

		if(TABS(pclClient->CurrentClock - clSimpleVary.dwRealKillClock) > 600000) // 10�� ( 600�� )
		{
			clSimpleVary.dwRealKillClock = pclClient->CurrentClock;

			// �߱��� ��ũ�� ���� ���� �������� �ʴ´�.
			if( clSimpleVary.siCheckMonsterNum >= 80 && pclClient->siServiceArea != ConstServiceArea_China )
			{
				if ( pclClient->pclMapManager->IsEncountMap(GetMapIndex()) &&  GetMapIndex() != MAPINDEX_MOFUMOFULAND )
				{
					SI32 x = GetX();
					SI32 y = GetY();

					SI32 kind = pclClient->GetUniqueFromHash(TEXT("KIND_JOKER"));

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);		// ��� PC�� ��ȣ��, ������� ������. 

					if(pclMap->FindEmptyAreaForCharLocate(kind, GetX() + 2, GetY(), 100, &x, &y, true) )
					{
						// �������� 100������ ��ȯ�ȴ�.
						SI32 siSummonID = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 100, 0,  x, y, GetMapIndex(), NPCITEMMODE_NORMAL, &clinnerstatus );

						//-------------------------------------------------------------
						// JOKERMONSTER
						//-------------------------------------------------------------
						// ��Ŀ ������ �����ð��� ���� ������ �ð��� �����Ѵ�.
						if(pclCM->IsValidID(siSummonID) )
						{
							((cltCharServer*)pclCM->CR[ siSummonID])->dwTimerClock = pclserver->CurrentClock;
						}
						//-------------------------------------------------------------
					}
				}
			}

			SetKillMonsterNum(0);
		}


		//// �ҷ��� NPC ����
		//if(TABS(pclClient->CurrentClock - clSimpleVary.dwRealKillClock) > 60) // 10�� ( 600�� )//1�ð�����
		//{
		//	clSimpleVary.dwRealKillClock = pclClient->CurrentClock;
		//	if ( pclClient->pclMapManager->IsEncountMap(GetMapIndex()) &&  GetMapIndex() != MAPINDEX_MOFUMOFULAND )
		//	{
		//		SI32 x = GetX();
		//		SI32 y = GetY();

		//		cltInnerStatusInfo clinnerstatus;
		//		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);		// ��� PC�� ��ȣ��, ������� ������. 
		//		// �������� 100������ ��ȯ�ȴ�.
		//		SI32 siSummonID = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENPC")), VILLAGE_NONE, NULL, 100, 0,  x+2, y, GetMapIndex(), NPCITEMMODE_NORMAL, &clinnerstatus );

		//		//-------------------------------------------------------------
		//		// JOKERMONSTER
		//		//-------------------------------------------------------------
		//		// ��Ŀ ������ �����ð��� ���� ������ �ð��� �����Ѵ�.
		//		if(pclCM->IsValidID(siSummonID) )
		//		{
		//			((cltCharServer*)pclCM->CR[ siSummonID])->dwTimerClock = pclserver->CurrentClock;
		//		}
		//		//-------------------------------------------------------------
		//	}
		//	SetKillMonsterNum(0);
		//}
		//---------------------------------------------
		// �ڵ� ȸ�� ó�� 
		//---------------------------------------------
		cltItem* pclitemlist = pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
		SI32 itemtype = pclserver->pclItemManager->GetItemType(pclitemlist[ActiveWeapon].siUnique) ;
		SI32 skilllevel = 0 ;

		DWORD autorecoverlifeclock = 50000 ;
		DWORD autorecovermanaclock = 50000 ;
		/*
		switch ( itemtype )
		{

		case ITEMTYPE_SPEAR:
			{
				skilllevel = pclCI->clCharSkill.GetTotalSkillLevel(SKILL_SPEAR_INCRECMANA, 0, 0, pclitemlist);
				if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
				{
					// SJY DEBUG
					SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW3,skilllevel);
					autorecovermanaclock = autorecovermanaclock -  rate * 1000 ;
				}
			}
			break;

		case ITEMTYPE_AXE:
			{
				skilllevel = pclCI->clCharSkill.GetTotalSkillLevel(SKILL_AXE_INCRECLIFE, 0, 0, pclitemlist);
				if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
				{
					// SJY DEBUG
					SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW3,skilllevel);
					autorecoverlifeclock = autorecoverlifeclock -  rate * 1000 ;
				}
			}
			break;
		}
		*/

			// ���� ������ ���� ���� ȸ���ӵ� ����
			if ( IsPC(GetCharUnique()) )
			{
				SI32 siSubRecoveryManaClock = m_clMagicBufMgr.GetIncMPRecoveryTime( this );
				if ( siSubRecoveryManaClock )
				{
					DWORD dwCalcRecoveryManaClock = (autorecovermanaclock / siSubRecoveryManaClock) * 100;
					autorecovermanaclock = dwCalcRecoveryManaClock;
					pclClient->pclLog->LogPrint( TEXT("����ȸ���ӵ� : %d\n"), dwCalcRecoveryManaClock );
				}
			}

			if((DWORD)TABS(pclClient->CurrentClock - dwAutoRecoverLifeClock) > autorecoverlifeclock)
			{
				dwAutoRecoverLifeClock = pclClient->CurrentClock;

				AutoRecoverLife();
			}

			//
			DWORD autocusionrecoverclock  = 5000;

			if( (DWORD)TABS( pclClient->CurrentClock - dwCusionRecoverLifeClock ) > autocusionrecoverclock )
			{
				dwCusionRecoverLifeClock = pclClient->CurrentClock;

				//
				AutoCushionRecoverLife();

			}

			if( (DWORD)TABS(pclClient->CurrentClock - dwAutoRecoverManaClock) > autorecovermanaclock)
			{
				dwAutoRecoverManaClock = pclClient->CurrentClock;

				AutoRecoverMana();
			}

			if( (DWORD)TABS(pclClient->CurrentClock - dwCusionRecoverManaClock) > autocusionrecoverclock )
			{
				dwCusionRecoverManaClock = pclClient->CurrentClock;

				//
				AutoCushionRecoverMana();
			}

			DWORD autocusionOFSclock  = 30000;

			// ��� ȸ��
			if( (DWORD)TABS(pclClient->CurrentClock - dwCusionRecoverOragansClock) > autocusionOFSclock )
			{
				dwCusionRecoverOragansClock = pclClient->CurrentClock;

				AutoCushionRecoverOragans();
			}
			// ������ �϶�.
			if( (DWORD)TABS(pclClient->CurrentClock - dwCusionFallSatisfiedClock) > autocusionOFSclock )
			{
				dwCusionFallSatisfiedClock = pclClient->CurrentClock;

				AutoCushionFallSatisfied();
			}

			// PCK : ���ڴ� �Ŀ��� ���� ��ȯ�� �ڵ� ȸ�� ���� (09.06.15)
			if( (DWORD)TABS( pclClient->CurrentClock - dwCusionRecoverSummonLifeClock ) > autocusionrecoverclock )
			{
				dwCusionRecoverSummonLifeClock = pclClient->CurrentClock;

				AutoCushionRecoverSummonLife();
			}

			if( (DWORD)TABS(pclClient->CurrentClock - dwCusionRecoverSummonManaClock) > autocusionrecoverclock )
			{
				dwCusionRecoverSummonManaClock = pclClient->CurrentClock;

				AutoCushionRecoverSummonMana();
			}

			DWORD autoTransformrecoverclock  = 5000;
			//KHY - 1220 - ���Žý���. - ������ ��ũ�� 100% = ����ȸ������.
			if( (DWORD)TABS(pclClient->CurrentClock - dwTransFormRecoverManaClock) > autoTransformrecoverclock )
			{
				dwTransFormRecoverManaClock = pclClient->CurrentClock;

				//
				AutoTransFormRecoverMana();
			}


			// ���ѿ��� �̿�� ���� �۾�
			if ( pclCI->clBI.bUseNoLimitTicketWarp )
			{
				// �̿�Ⱓ�� ������ �����ִ� ��Ŷ�� db�� ������.
				if ( pclCI->clBI.siNoLimitTicketWarpDateVary < pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_NoLimitTicket_Warp clMsg( false ,GetID(), pclCI->GetPersonID(), 0, &clitem, 0, NOLIMIT_TICKET );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}

			// �������� Ƽ�� �����۾�
			if ( pclCI->clBI.bSpaceBoxPremiumService )
			{
				// �̿�Ⱓ�� ������ �����ִ� ��Ŷ�� db�� ������.
				if ( pclCI->clBI.siSpaceBoxPremiumServiceDate< pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_NoLimitTicket_Warp clMsg( false ,GetID(), pclCI->GetPersonID(), 0, &clitem, 0, SPACEBOX_TICKET );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}
			// ��ȯ�� Ȯ�� Ƽ�� ���� �۾�
			if ( pclCI->clBI.bSummonExtraSlot )
			{
				// �̿�Ⱓ�� ������ �����ִ� ��Ŷ�� db�� ������.
				if ( pclCI->clBI.bSummonExtraSlot==true && pclCI->clBI.siSummonExtraSlotDate < pclserver->pclTime->clDate.GetDateVary() )
				{
					// ���Ⱓ�� �ٵǸ� Ƽ���� ����������.
					cltItem clitem;
					clitem.Init();
					sDBRequest_NoLimitTicket_Warp clMsg( false ,GetID(), pclCI->GetPersonID(), 0, &clitem, 0, SUMMON_SPACE_TICKET );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					// �������� ���� �ִ´�.
					for( SI32 i=MAX_SUMMON_OWN; i<MAX_HAVE_SUMMON; i++)
					{
						cltSimpleSummon* pclSimpleSummon = pclCI->clSummonInfo.GetSummonBase(i);
						SI32 siSummonID = pclSimpleSummon->siSummonID;
						SI32 siSummonIndex = pclCI->clSummonInfo.GetSummonIndexFromSummonID(siSummonID);
						if(siSummonIndex >= 0)
						{
							sDBRequest_SummonSlotOut clMsg(GetCharID(), pclCI->GetPersonID(), siSummonID, siSummonIndex);
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}
					}
				}
			}

			// �����̾� ���� ����
			if ( pclCI->clBI.bPremiumService )
			{
				// �̿�Ⱓ�� ������ �����ش�.
				if ( pclCI->clBI.siPremiumServiceDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_SetPremium clMsg(GetID(),pclCI->GetPersonID(),false,0,0,0,0,0,&clitem,0,SETPREMIUM_EXP);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}
			// �����̾� ���� ����
			if ( pclCI->clBI.bCombatService )
			{
				// �̿�Ⱓ�� ������ �����ش�.
				if ( pclCI->clBI.siCombatServiceDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_SetPremium clMsg(GetID(),pclCI->GetPersonID(),false,0,0,0,0,0,&clitem,0,SETPREMIUM_COMBAT);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}
			}
			// ���� �����̾� ���� ����
			if ( pclCI->clBI.bMakeItemService )
			{
				// �̿�Ⱓ�� ������ �����ش�.
				if ( pclCI->clBI.siMakeItemServiceDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_SetPremium clMsg(GetID(),pclCI->GetPersonID(),false,0,0,0,0,0,&clitem,0,SETPREMIUM_MAKEITEM);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}
			}

			// źȯ �����̾� ���� ����
			if ( pclCI->clBI.bBulletService )
			{
				// �̿�Ⱓ�� ������ �����ش�.
				if ( pclCI->clBI.siBulletServiceDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_SetPremium clMsg(GetID(),pclCI->GetPersonID(),false,0,0,0,0,0,&clitem,0,SETPREMIUM_BULLET);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}
			}

			// ��ȯ�� ���¹̳� �����̾� ���� ����
			if ( pclCI->clBI.bSummonStaminaService )
			{
				// �̿�Ⱓ�� ������ �����ش�.
				if ( pclCI->clBI.siSummonStaminaServiceDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_SetPremium clMsg(GetID(),pclCI->GetPersonID(),false,0,0,0,0,0,&clitem,0,SETPREMIUM_SUMMONSTIMINA);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				}
			}

			// ���� �����̾� ����
			if ( pclCI->clBI.bMarKetPremiumService )
			{
				// �̿�Ⱓ�� ������ �����ִ� ��Ŷ�� db�� ������.
				if ( pclCI->clBI.siMarKetPremiumServiceDate < pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_NoLimitTicket_Warp clMsg( false ,GetID(), pclCI->GetPersonID(), 0, &clitem, 0, MARKET_TICKET );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}

			//[����] ���ո��� �����̾� ����.
			if ( pclCI->clBI.bNewMarketPremiumService )
			{
				// �̿�Ⱓ�� ������ �����ִ� ��Ŷ�� db�� ������.
				if ( pclCI->clBI.siNewMarketPremiumServiceDate < pclserver->pclTime->clDate.GetDateVary() )
				{
					cltItem clitem;
					clitem.Init();
					sDBRequest_NoLimitTicket_Warp clMsg( false ,GetID(), pclCI->GetPersonID(), 0, &clitem, 0, NewMarket_TICKET );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}

			// �ð��� ���� ������ ���� �Ѵ�.
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclCI->clBI.clBufInfo );
			for ( SI16 i=0 ; i < MAX_BUF_COUNT ; i++ )
			{
				if ( clBuf.m_stBuf[i].siBufType == 0 )
					continue;

				if ( ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() > clBuf.m_stBuf[i].siOverTime )
				{
					sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), i, &clBuf.m_stBuf[i]);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


					if( clBuf.m_stBuf[i].siBufType  == BUF_NOFIRSTATTACK		|| 
						clBuf.m_stBuf[i].siBufType  == BUF_HINAMOCHI			|| 
						clBuf.m_stBuf[i].siBufType  == BUF_EVENT_EGGMOONCAKE	|| 
						// �߽��� ������ ����
						(clBuf.m_stBuf[i].siBufType  == BUF_TRANSFORM && clBuf.m_stBuf[i].siGetType != BUF_GET_TYPE_NIGHTMARKET)
					)
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}

					if( clBuf.m_stBuf[i].siBufType  == BUF_LOVEPOWER || clBuf.m_stBuf[i].siBufType  == BUF_FRIENDSHIP )
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_HEARTEFFECT, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}
					else if( clBuf.m_stBuf[i].siBufType == BUF_FOURLEAF )
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FOURLEAF, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}

					else if( clBuf.m_stBuf[i].siBufType == BUF_HAPPYBEAN )
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(BUF_HAPPYBEAN, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}
					else if( clBuf.m_stBuf[i].siBufType == BUF_THANKSGIVING_DAY )
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(BUF_THANKSGIVING_DAY, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}
					else if( clBuf.m_stBuf[i].siBufType == BUF_FARMINGNMININGFOREVER )
					{
						// �̿����� �뺸 
						cltCharChangeStatusInfo	StatusInfo(BUF_FARMINGNMININGFOREVER, false);
						cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
						SendNeighbourMsg(&clMsg, true);
					}


				}
			}

			// ����,������ �ӽ� ��� �Ⱓ�� ������
			if ( m_pclAgriculture && m_pclAgriculture->GetCurrentVillageUnique() > 0)
			{
				UI32 uiLeaseExpire = m_pclAgriculture->GetExpireTime( m_pclAgriculture->GetCurrentVillageUnique() );
				if( uiLeaseExpire > 0 && pclClient->GetMinuteFromsTime( ((cltServer*)pclClient)->sTime ) >= uiLeaseExpire )
				{
					// DB�� ���� ��û
					sDBRequest_ExpiredAgricultureLease clMsg( pclCI->GetPersonID(), m_pclAgriculture->GetCurrentVillageUnique(), uiLeaseExpire );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg );

					// ���� ���� ����
					m_pclAgriculture->DeleteAgriculture( m_pclAgriculture->GetCurrentVillageUnique() );
					m_pclAgriculture->EndAgriculture( m_pclAgriculture->GetCurrentVillageUnique() );
				}
			}

			// ���� ���� �۾�. 
			if(clInnerStatus.IsStatus(INNERSTATUS_PRISON))
			{
				// ���� �ð��� ����Ѵ�. 
				if(TABS(pclClient->CurrentClock - dwPrisonClock) > 1000)
				{
					dwPrisonClock = pclClient->CurrentClock;

					if(pclCI->clGuiltyInfo.siLeftPrisonSecond)pclCI->clGuiltyInfo.siLeftPrisonSecond--;

					//������ ���� ������ Ŭ���̾�Ʈ���� �뺸�Ѵ�. 
					SetUpdateSwitch(UPDATE_GUILTY, true, 0);

					// �溹������ ���� ��Ų��. 
					if(pclCI->clGuiltyInfo.siLeftPrisonSecond == 0)
					{
						clInnerStatus.DelStatus(INNERSTATUS_PRISON);
						WarpToSafeZone();

						pclCI->clGuiltyInfo.Init();
						sDBRequest_GuiltyInfoRecord clMsg(GetID(), pclCI->GetPersonID(), &pclCI->clGuiltyInfo);
						pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&clMsg );


					}
				}
			}
			else if(pclCI->clGuiltyInfo.siLeftPrisonSecond)
			{
				SendPrison();
			}

			//-------------------------------------
			// ���� ������ �����Ѵ�. 
			//-------------------------------------
			if(clSimpleVary.bCheckFameLevelSwitch == false && (pclClient->GetFrame() > dwMakeFrame + 200))
			{
				clSimpleVary.bCheckFameLevelSwitch = true;
				IncreaseFameExp(0);
			}

			// ��ƿ ������ �����Ѵ�. 
			if(GetStealNum())
			{
				if(TABS(pclClient->CurrentClock - dwStealClock) > 60000)
				{
					dwStealClock	= pclClient->CurrentClock;
					siStealNum--;
				}
			}
			
			// ���� �ý��� ����
			if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
			{
				// ��ȥ �ߴٸ�
				if ( TRUE == IsMarried() )
				{
					//============================================
					// ��ȥ ����� ������ ���� üũ
					//============================================
					SI32 siNowDateVary = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary();

					if ( (FALSE == pclMarriage->GetRequestItemGet())					// ���� ��û ���� �ʾҴٸ�
						&& (TRUE == pclMarriage->IsWeddingAnniversary(siNowDateVary))	// ��ȥ ����� �̰� 
						&& (FALSE == pclMarriage->IsWeddingItemGet(siNowDateVary))		// ���� ��ȥ ����� �������� �ȹ޾Ұ�
					)
					{
						// �켱 ��û �ߴٰ� �����ϰ�
						pclMarriage->SetRequestItemGet( TRUE );

						// �������� ��ȥ ����� ������ ������� ������
						((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_REQUEST_WEDDING_ITEMGET, 0, 0, GetCharUnique() );
						
					}

					//============================================
					// ������ ���� üũ
					//============================================
					SI32 siNowHourVary	= pclserver->pclTime->clDate.GetHourVary();
					SI16 siNowLoveLevel = pclMarriage->GetLoveLevel();
					if ( (MARRIAGE_LOVE_LEVEL_01 < siNowLoveLevel) && (0 < pclMarriage->GetLimitHourVary()) )
					{
						SI16 siCalcLoveLevel = pclMarriage->CalcNowLoveLevel( siNowHourVary );
						if ( siCalcLoveLevel != siNowLoveLevel )
						{
							// ���ӽð� �Ϸ�(24�ð�,���ǽð� 48��)���� 2�ܰ辿 �������� �ϴϱ� 24�ð��� �����ش�
							pclMarriage->SetLimitHourVary( siNowHourVary + 24 );
							pclMarriage->SetLoveLevel( siCalcLoveLevel );

							sDBRequest_Marry_Love_Set clMsg( pclCI->GetPersonID(), pclMarriage->GetUseHourVary(), pclMarriage->GetLimitHourVary(), pclMarriage->GetLoveLevel() );
							pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );

							/*
							// ����� ĳ����
							SI32			siMateCharID	= pclClient->pclCM->GetIDFromPersonID( pclMarriage->GetMatePersonID() );
							cltCharServer*	pclMateChar		= pclClient->pclCM->GetCharServer( siMateCharID );
							if ( (pclMateChar) && (pclMateChar->IsMarried()) )
							{
								pclMateChar->pclMarriage->SetLimitDateVary( siNowDateVary + 1 );
								pclMarriage->SetLoveLevel( siCalcLoveLevel );

								sDBRequest_Marry_Love_Set clMsg( pclCI->GetPersonID(), pclMateChar->pclMarriage->GetUseDateVary(), pclMarriage->GetLimitDateVary(), pclMateChar->pclMarriage->GetLoveLevel() );
								pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clMsg );
							}
							*/
						}
					}	// if ( (MARRIAGE_LOVE_LEVEL_01 < siNowLoveLevel) && (0 < pclMarriage->GetLimitDateVary()) )
				}
			}


			// �ڵ� ���� ���� �������̶��. 
			if(bCloseSocketSwitch)
			{
				if(dwCloseSocketTimer)
				{
					dwCloseSocketTimer --;

					if(dwCloseSocketTimer == 0)
					{
						// apex-
#ifdef _CHINA
						if( 0 < GetID() )
							NoticeApexProxy_UserLogout( GetID(), (char*)pclCI->clBI.szAccountID );
						bApexLogin = false;
#endif
						// -apex

#ifdef USE_GAME_GUARD_SERVER
						CCSAuthClose();
#endif
						bCloseSocketSwitch = false;
						GetSession()->CloseSocket();

						// -- ������ ĳ���� ����ó�� --

						// ĳ���� ���� ó��
						//Close();
						// ��Ʈ��ũ ���� ����
						//GetSession()->CloseSocket();
					}
				}
			}

			// �ڱ� �ڸ��� �浹 �������� Ȯ���Ѵ�. 
			if(bReadySwitch && TABS(pclClient->CurrentClock - dwLastCheckCollClock) > 2000)
			{
				dwLastCheckCollClock = pclClient->CurrentClock;

				if( IsColl(GetX(), GetY()) == true)	//  �踦 Ÿ�� �ִ� ���¿����� ���� ������ ��Ű�� �ʴ´�. 
				{
					TCHAR* pText = GetTxtFromMgr(2817);
					if (pclClient->GameMode == GAMEMODE_SERVER)
						pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), pText, 
						pclCI->GetPersonID(), GetName(), GetMapIndex(), GetX(), GetY(), siWarpUnique);

					// ��������� �̵�. 
					WarpToSafeZone();
				}
			}

			// �� ���� ��ġ�� ���Ѵ�.
			if(TABS(pclClient->CurrentClock - clSimpleVary.dwLastHorseActionClock) >= 2000)
			{
				clSimpleVary.dwLastHorseActionClock = pclClient->CurrentClock;

				SI32 horseaction = 0;
				SI32 datevary = pclserver->pclTime->clDate.GetDateVary();

				//KHY - 1001 - ��µ��� 5���� ���� ����. - ��� ���� �˻��Ѵ�.  - ��縻�� ������ ���⼭ �˻��Ѵ�.
				for( int i = 0; i < MAX_HORSE_PER_PERSON; i++) 
				{
					cltHorse* pclhorse_die = pclCI->clHorseInfo.GetHorse(i);

					if(pclhorse_die && pclhorse_die->siHorseUnique )
					{
						// ������ �� �Ǿ��ٸ� ���δ�. 
						SI32 life = pclserver->pclHorseManager->GetLife(pclhorse_die, datevary);
						if(life <= 0)
						{
							// ���� Ÿ�� �ִٴ� ������ �ʱ�ȭ ������� �Ѵ�.
							pclCI->clHorseInfo.siCurrentHorseRIDEHORSE =  false;
							pclhorse_die->Init();
							//KHY - 1001 - ��µ��� 5���� ���� ����. - ���° ���� �״��� �˱����ؼ�// 200 + �������� �ε��������� ��ȯ�ȴ�.
							horseaction =  (HORSEACTION_DIE + i);		// ���ó��. 
							break;
						}
					}
				}

				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

				if(horseaction < HORSEACTION_DIE )
				{
					if(pclhorse != NULL)
						horseaction = CharHorseAction(pclhorse , true);
				}

				if(horseaction == HORSEACTION_UPDATE)	// ������Ʈ ó��. 
				{
					if(pclhorse != NULL)
					{
						// DB�� �����Ѵ�. 
						UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse, pclhorse, SETHORSE_REASON_UPDATE);

						SetUpdateSwitch(UPDATE_HORSE, true, 0);
					}
				}
				else if ( horseaction == HORSEATTION_TICKET ) // ���ɷ�����̿�Ƕ�����
				{
					if(pclhorse != NULL)
					{
						// DB�� �����Ѵ�. 
						UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse,
							pclhorse,SETHORSE_REASON_TICKET);

						SetUpdateSwitch(UPDATE_HORSE, true, 0);
					}
				}
				else if ( horseaction == HORSEATTION_PREMIUMPARTS ) // ���ɷ�����̿�Ƕ�����
				{
					if(pclhorse != NULL)
					{
						// DB�� �����Ѵ�. 
						UpdateHorseToDB((cltServer*)pclClient, pclCI->clHorseInfo.siCurrentShowHorse,
							pclhorse,SETHORSE_REASON_PREMIUM_PARTS);

						SetUpdateSwitch(UPDATE_HORSE, true, 0);
					}
				}
				//KHY - 1001 - ��µ��� 5���� ���� ����. - ���° ���� �״��� �˱����ؼ�// 200 + �������� �ε��������� ��ȯ�ȴ�.
				else if(horseaction >= HORSEACTION_DIE)	// ���ó��. 
				{
					SI16 DiehorseIndex = horseaction - 200; //horseaction = �׻� 200 �̻��̴�.
					cltHorse* pclhorse_Die = pclCI->clHorseInfo.GetHorse(DiehorseIndex);

					if(pclhorse_Die)
					{
						// DB�� �����Ѵ�. 
						UpdateHorseToDB((cltServer*)pclClient, DiehorseIndex, pclhorse_Die, SETHORSE_REASON_DIE);

						SetUpdateSwitch(UPDATE_HORSE, true, 0);
					}
				}

				// �ڵ������̰� �����Ǿ��ִٸ�, 
				if(pclCI->clHorseInfo.bAutoFoodSwitch)
				{
					if(TABS(pclClient->CurrentClock - dwLastCheckAutoFoodClock) > 5000)
					{
						dwLastCheckAutoFoodClock = pclClient->CurrentClock;

						SI32 itemunique, itemnum;
						if(pclCI->clHorseInfo.GetNeed(pclhorse, &itemunique, &itemnum))
						{
							AutoHorseFood(itemunique, itemnum);
						}
					}
				}

				if(TABS(pclClient->CurrentClock - clSimpleVary.dwlastCheckHorseSpeedQuestClock) >= 30000)
				{
					clSimpleVary.dwlastCheckHorseSpeedQuestClock = pclClient->CurrentClock;
					DoQuestCheckAndAction(QUEST_CLEAR_COND_HORSESPEED);
				}

			}



			//----------------------------------
			// �ʱ� ��ȣ ��ġ. 
			//----------------------------------
			if(clSimpleVary.bInitProtectSwitch)
			{
				if(TABS(pclClient->CurrentClock - clSimpleVary.dwInitProtectClock) >= 10000)
				{
					clSimpleVary.bInitProtectSwitch = false;
				}
			}

			if(bReadySwitch)
			{
				// �ְ� ���� ����. 
				if(siReservedTopSkillUnique != clInnerStatus.siTopSkillUnique)
				{
					clInnerStatus.siTopSkillUnique = siReservedTopSkillUnique;

					// Ŭ���̾�Ʈ�� ������ ������. 
					cltCharChangeStatusInfo clchangeinfo(CHANGETYPE_TOPSKILLUNIQUE, true, siReservedTopSkillUnique);
					cltGameMsgResponse_StatusInfo clinfo(GetID(), &clchangeinfo);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clinfo), (BYTE*)&clinfo);
					SendNeighbourMsg(&clMsg, true);
				}

				// ���� 100% ����Ʈ ������ ������.
				/*if( clTransFormInfo.bTransFormMode == true )
				{
					SI32 siTransFormSyc = (SI32)clTransFormInfo.GetSynchroTransForm(clTransFormInfo.uiNowTransFormKind);

					if(siTransFormSyc >=100)
					{
						//clTransFormInfo.bTransForm100Effect  = true;

						cltCharChangeStatusInfo clchangeinfo(CHANGETYPE_TRANSFORM100EFFECT, true, siReservedTopSkillUnique);
						cltGameMsgResponse_StatusInfo clinfo(GetID(), &clchangeinfo);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clinfo), (BYTE*)&clinfo);
						SendNeighbourMsg(&clMsg, true);					
					}
				}
				*/
					if(TABS(pclClient->GetFrame() - uiLevelInfoUpdateFrame) >= 12000)	// 5��.
					{
						uiLevelInfoUpdateFrame = pclClient->GetFrame();

						//---------------------------------
						// ��� ����ġ ���� ����. 
						//---------------------------------
						SendDBSkillExp();	

						//--------------------------------------
						// ����Ǿ� �ִ� ������ ����� ó���Ѵ�.
						//--------------------------------------
						ItemUseReserveAction();

						//--------------------------------------
						// ����Ǿ� �ִ� źȯ ������ ����� ó���Ѵ�.
						//--------------------------------------
						BulletUseReserveAction();

						//---------------------------------
						// ������ ���� ����.  - ������ ��ü �������� �����Ѵ�.
						//---------------------------------
						SendDBWasteDurability();
						SendDBSummonWasteDurability();

						//--------------------------------------
						// �ڽ��� ��ȯ�� ������ ���� 
						//--------------------------------------
						SendDBSummon(SETSUMMON_REASON_UPDATE, MAX_SUMMON_OWN);

						cltLevelInfo cllevelinfo(pclCI->clIP.siExp, pclCI->clIP.siLevel, pclCI->clIP.siBonus, pclCI->clIP.siSkillBonus, pclCI->clIP.siFameExp, pclCI->clIP.siFameLevel);
						if(clOldLevelInfo.IsSame(&cllevelinfo) == false)
						{
							//---------------------------------
							// ���� ���� ���� 
							//---------------------------------
							SendDBLevelInfo(0, 0);
						}
					}

					//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü ������ DB����  - �ϴ� 30��.
					if( m_clCharUpdateTimer[CHAR_UPDATETIMER_CRYSTALITEM_DB].IsTimed(GetTickCount()) ) // 30��.
					{
						SendDBCrystalDurability();
					}

					//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü ������ client�� ����   - �ϴ� 5��. - �ȿ��� 1% �϶��� ���������
					if( m_clCharUpdateTimer[CHAR_UPDATETIMER_CRYSTALITEMINFO_CLIENT].IsTimed(GetTickCount()) ) // 5��.
					{			
						SendClientCrystalDurability();
					}			
			}


			//----------------------------------
			// �̼� ����Ʈ �ð� ���� ó��
			//----------------------------------
			if( pclCI->clQuestInfo.clMissionQuestInfo.siUnique > 0 && pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].siCurQuestUnique > 0 )
			{
				pclCI->clQuestInfo.clMissionQuestInfo.TimeReduce( 1 );		// 1�� ����

				if( pclCI->clQuestInfo.clMissionQuestInfo.IsTimeLeft() == false )		// �ð� ����
				{
					// ������ ����Ʈ �ʱ�ȭ
					pclCI->clQuestInfo.clMissionQuestInfo.Init();
					pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1].Init();

					// �̼� ����Ʈ ������ DB�� �����Ѵ�.
					sDBRequest_SetMissionQuestInfo clMissionQuestInfo(pclCI->GetPersonID(), &pclCI->clQuestInfo.clMissionQuestInfo );
					pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMissionQuestInfo);

					// ����Ʈ ���¸� DB�� �����Ѵ�.
					sDBRequest_SetQuestInfo clQuestInfo(pclCI->GetPersonID(), QUEST_TYPE_MISSION1, &pclCI->clQuestInfo.clUnit[QUEST_TYPE_MISSION1]);
					pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clQuestInfo);

					// Ŭ���̾�Ʈ�� ����Ʈ�� ���¸� �뺸�Ѵ�. 
					SetUpdateSwitch(UPDATE_QUEST, true, 0);
				}
			}

			//----------------------------------
			// Daily Quest
			//----------------------------------
			if ( pclCI->clDailyQuestInfo.siMonsterKind > 0 )
			{
				// 30�ʴ� �ѹ��� ó��
				if( TABS( pclClient->CurrentClock - dwDailyQuestClock ) >= 30000 )
				{
					dwDailyQuestClock = pclClient->CurrentClock;
					pclCI->clDailyQuestInfo.siLeftTime -= 30;		// 30�� �ٿ���.

					if ( pclCI->clDailyQuestInfo.siLeftTime <= 0 )
					{
						// �ð��� �Ѿ����Ƿ� ����Ʈ ����
						pclCI->clDailyQuestInfo.siLeftTime = 0;
						cltItem clItem;
						clItem.Init();
						sDBRequest_DailyQuestDel clMsg( pclCI->GetPersonID(), GetID(), 0, 0, &clItem, 1,0, 0, 0 );
						cltServer *pclServer = (cltServer *)pclClient;
						pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
					else
					{
						// �ð��ȿ� ����Ʈ���෮�� �������� ��� -> �Ϸ�ó�� �� ���� ����
						if ( pclCI->clDailyQuestInfo.siKillMonsterNum >= pclCI->clDailyQuestInfo.siMonsterNum )
						{
							SI32 value = 100;

							cltItem clItem;
							clItem.siUnique = pclCI->clDailyQuestInfo.siRewardItemUnique;
							clItem.siItemNum = ( pclCI->clDailyQuestInfo.siRewardItemNum * value ) / 100;

							cltItem clToItem;
							clToItem.Init();

							SI16 InvPos = -1;
							SI32 rtnvalue;

							// �������� �̻��ڵ� ���� - By LEEKH 2008-10-23
							//if( pclCI->clDailyQuestInfo.siQuestTime - pclCI->clDailyQuestInfo.siLeftTime )

							// ������ �ִ°��
							if ( pclCI->clDailyQuestInfo.bMoneySelected )
							{
								sDBRequest_DailyQuestDel clMsg( pclCI->GetPersonID(), GetID(), true, InvPos, &clItem, 1, ( pclCI->clDailyQuestInfo.siRewardMoney * value ) / 100, pclCI->clDailyQuestInfo.bMoneySelected, pclCI->clDailyQuestInfo.siRewardExp );
								cltServer *pclServer = (cltServer *)pclClient;
								pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

								pclServer->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DAILYQUEST, (pclCI->clDailyQuestInfo.siRewardMoney * value) / 100);
							}
							else 
							{
								// ���� ������ ����
								if ( CanAddInv( INVMODE_PERSON, &clItem, &InvPos, &clToItem, &rtnvalue) )
								{
									sDBRequest_DailyQuestDel clMsg( pclCI->GetPersonID(), GetID(), true, InvPos, &clItem, 1, pclCI->clDailyQuestInfo.siRewardMoney, pclCI->clDailyQuestInfo.bMoneySelected, pclCI->clDailyQuestInfo.siRewardExp );
									cltServer *pclServer = (cltServer *)pclClient;
									pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
								}
								else
								{
									// �κ� �� ���� �ȵǹǷ�... - ����Ʈ�� ���� ���¸� �����Ѵ�.
									cltServer *pclServer = (cltServer *)pclClient;
									pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

									cltGameMsgResponse_DailyQuestChange clinfo( pclCI->clDailyQuestInfo.siLeftTime, pclCI->clDailyQuestInfo.siKillMonsterNum );
									cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUESTCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
									pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
								}

							}
						}
						else
						{
							// ����Ʈ ���޷��� �������� ���� ��� ���� ���¸� ����
							cltGameMsgResponse_DailyQuestChange clinfo( pclCI->clDailyQuestInfo.siLeftTime, pclCI->clDailyQuestInfo.siKillMonsterNum );
							cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUESTCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
							pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
			}

			//----------------------------------
			// Daily Quest2
			//----------------------------------
			if ( pclCI->clDailyQuest2Info.siMonsterKind > 0 )
			{
				// 30�ʴ� �ѹ��� ó��
				if( TABS( pclClient->CurrentClock - dwDailyQuest2Clock ) >= 30000 )
				{
					dwDailyQuest2Clock = pclClient->CurrentClock;
					pclCI->clDailyQuest2Info.siLeftTime -= 30;		// 30�� �ٿ���.

					if ( pclCI->clDailyQuest2Info.siLeftTime <= 0 )
					{
						pclCI->clDailyQuest2Info.siLeftTime = 0;
						cltItem clItem;
						clItem.Init();
						sDBRequest_DailyQuest2Del clMsg( pclCI->GetPersonID(), GetID(), 0, 0, &clItem, 1,0, 0,pclCI->clDailyQuest2Info.bHiddenQuest);
						cltServer *pclServer = (cltServer *)pclClient;
						pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
				else
				{
					if ( pclCI->clDailyQuest2Info.siKillMonsterNum >= pclCI->clDailyQuest2Info.siMonsterNum )
					{

						SI32 value = 100;

						cltItem clItem;
						clItem.siUnique = pclCI->clDailyQuest2Info.siRewardItemUnique;
						clItem.siItemNum = ( pclCI->clDailyQuest2Info.siRewardItemNum * value ) / 100;

						cltItem clToItem;
						clToItem.Init();

						SI16 InvPos = -1;
						SI32 rtnvalue;

						// �������� �̻��ڵ� ���� - by LEEKH 2008-10-23
						//if( pclCI->clDailyQuest2Info.siQuestTime - pclCI->clDailyQuest2Info.siLeftTime)
						// ������ �ִ°��
						if ( pclCI->clDailyQuest2Info.bMoneySelected )
						{
							sDBRequest_DailyQuest2Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos, &clItem, 1, ( pclCI->clDailyQuest2Info.siRewardMoney * value ) / 100, pclCI->clDailyQuest2Info.bMoneySelected, pclCI->clDailyQuest2Info.bHiddenQuest);
							cltServer *pclServer = (cltServer *)pclClient;
							pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						}
						else 
						{
							if ( pclCI->clDailyQuest2Info.bHiddenQuest == true ) 
							{
								// �ݺ� ����
								cltItem clItem2;
								clItem2.siUnique	=	ITEMUNIQUE(13321)	;
								clItem2.siItemNum	=	1	;


								if( CheckItemForEmptyAnyItemSlotByHiddenQuesst(INVMODE_PERSON, &clItem, &clItem2)  )
								{
									if ( CanAddInv( INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
									{
										sDBRequest_DailyQuest2Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos, &clItem, 1, pclCI->clDailyQuest2Info.siRewardMoney, pclCI->clDailyQuest2Info.bMoneySelected, pclCI->clDailyQuest2Info.bHiddenQuest);
										cltServer *pclServer = (cltServer *)pclClient;
										pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
									}
								}
								else
								{
									// �κ� �� ���� �ȵǹǷ�...
									cltServer *pclServer = (cltServer *)pclClient;
									pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

									cltGameMsgResponse_DailyQuest2Change clinfo( pclCI->clDailyQuest2Info.siLeftTime, pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.bHiddenQuest );
									cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST2CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
									pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);

								}
							}
							else
							{
								if ( CanAddInv( INVMODE_PERSON, &clItem, &InvPos, &clToItem, &rtnvalue) )
								{
									sDBRequest_DailyQuest2Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos, &clItem, 1, pclCI->clDailyQuest2Info.siRewardMoney, pclCI->clDailyQuest2Info.bMoneySelected, pclCI->clDailyQuest2Info.bHiddenQuest);
									cltServer *pclServer = (cltServer *)pclClient;
									pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
								}
								else
								{
									// �κ� �� ���� �ȵǹǷ�...
									cltServer *pclServer = (cltServer *)pclClient;
									pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

									cltGameMsgResponse_DailyQuest2Change clinfo( pclCI->clDailyQuest2Info.siLeftTime, pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.bHiddenQuest );
									cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST2CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
									pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
								}
							}
						}
					}
					else
					{
						cltGameMsgResponse_DailyQuest2Change clinfo( pclCI->clDailyQuest2Info.siLeftTime, pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.bHiddenQuest );
						cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST2CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
			//----------------------------------
			// Daily Quest3
			//----------------------------------
			if ( pclCI->clDailyQuest3Info.siItemUnique > 0 &&
				pclCI->clDailyQuest3Info.bHiddenQuest == false ) // ���ϸ� ����Ʈ 3�϶� ������ ��쿡�� �ð��� ���پ�鵵�� �մϴ�.
			{
				// 30�ʴ� �ѹ��� �����ص�.
				if( TABS( pclClient->CurrentClock - dwDailyQuest3Clock ) >= 30000 )
				{
					dwDailyQuest3Clock = pclClient->CurrentClock;
					pclCI->clDailyQuest3Info.siLeftTime -= 30;		// 30�� �ٿ���.

					if ( pclCI->clDailyQuest3Info.siLeftTime <= 0 )
					{
						pclCI->clDailyQuest3Info.siLeftTime = 0;
						cltItem clItem;
						clItem.Init();
						sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), 0, 0,
							pclCI->clDailyQuest3Info.siItemUnique , &clItem, 1,0,pclCI->clDailyQuest3Info.bHiddenQuest);
						cltServer *pclServer = (cltServer *)pclClient;
						pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
					else
					{
						if ( pclCI->clDailyQuest3Info.siMadeItemNum >= pclCI->clDailyQuest3Info.siItemNum)
						{
							SI32 value = 100;

							cltItem clItem;
							clItem.siUnique = pclCI->clDailyQuest3Info.siRewardItemUnique;
							clItem.siItemNum = ( pclCI->clDailyQuest3Info.siRewardItemNum * value ) / 100;

							cltItem clToItem;
							clToItem.Init();

							SI16 InvPos = -1;
							SI32 rtnvalue;

							// ���� ���� �̻� �ڵ� ���� - by LEEKH 2008-10-23
							//if( pclCI->clDailyQuest3Info.siQuestTime - pclCI->clDailyQuest3Info.siLeftTime)
							// ������ �ִ°��
							if ( pclCI->clDailyQuest3Info.bMoneySelected )
							{
								sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos,
									pclCI->clDailyQuest3Info.siItemUnique ,&clItem, 1, pclCI->clDailyQuest3Info.bMoneySelected, pclCI->clDailyQuest3Info.bHiddenQuest);
								cltServer *pclServer = (cltServer *)pclClient;
								pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
							}
							else 
							{
								if ( pclCI->clDailyQuest3Info.bHiddenQuest == true ) 
								{
									// �ݺ� ����
									cltItem clItem2;
									clItem2.siUnique	=	ITEMUNIQUE(13321)	;
									clItem2.siItemNum	=	1	;

									if( CheckItemForEmptyAnyItemSlotByHiddenQuesst(INVMODE_ALL, &clItem, &clItem2)  )
									{
										if ( CanAddInv( INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
										{
											sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos,
												pclCI->clDailyQuest3Info.siItemUnique ,&clItem, 1, pclCI->clDailyQuest3Info.bMoneySelected, pclCI->clDailyQuest3Info.bHiddenQuest);
											cltServer *pclServer = (cltServer *)pclClient;
											pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
										}
									}
									else
									{
										// �κ� �� ���� �ȵǹǷ�...
										cltServer *pclServer = (cltServer *)pclClient;
										pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

										cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum , pclCI->clDailyQuest3Info.bHiddenQuest );
										cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
										pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);

									}
								}
								else
								{
									if ( CanAddInv( INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
									{
										sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos,
											pclCI->clDailyQuest3Info.siItemUnique ,&clItem, 1, pclCI->clDailyQuest3Info.bMoneySelected, pclCI->clDailyQuest3Info.bHiddenQuest);
										cltServer *pclServer = (cltServer *)pclClient;
										pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
									}
									else
									{
										// �κ� �� ���� �ȵǹǷ�...
										cltServer *pclServer = (cltServer *)pclClient;
										pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

										cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum , pclCI->clDailyQuest3Info.bHiddenQuest );
										cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
										pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
									}
								}

							}
						}
						else
						{
							cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum, pclCI->clDailyQuest3Info.bHiddenQuest );
							cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
							pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
			}
			else if( pclCI->clDailyQuest3Info.siItemUnique > 0 && pclCI->clDailyQuest3Info.bHiddenQuest == true )
			{
				if ( pclCI->clDailyQuest3Info.siMadeItemNum >= pclCI->clDailyQuest3Info.siItemNum )
				{
					SI32 value = 100;

					cltItem clItem;
					clItem.siUnique = pclCI->clDailyQuest3Info.siRewardItemUnique;
					clItem.siItemNum = ( pclCI->clDailyQuest3Info.siRewardItemNum * value ) / 100;

					cltItem clToItem;
					clToItem.Init();

					SI16 InvPos = -1;
					SI32 rtnvalue;

					if ( pclCI->clDailyQuest3Info.bHiddenQuest == true ) 
					{
						// �ݺ� ����
						cltItem clItem2;
						clItem2.siUnique	=	ITEMUNIQUE(13321)	;
						clItem2.siItemNum	=	1	;

						if( CheckItemForEmptyAnyItemSlotByHiddenQuesst(INVMODE_ALL, &clItem, &clItem2)  )
						{
							if ( CanAddInv( INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
							{
								sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos,
									pclCI->clDailyQuest3Info.siItemUnique ,&clItem, 1, pclCI->clDailyQuest3Info.bMoneySelected, pclCI->clDailyQuest3Info.bHiddenQuest);
								cltServer *pclServer = (cltServer *)pclClient;
								pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
							}
						}
						else
						{
							// �κ� �� ���� �ȵǹǷ�...
							cltServer *pclServer = (cltServer *)pclClient;
							pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

							cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum , pclCI->clDailyQuest3Info.bHiddenQuest );
							cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
							pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);

						}
					}
					else
					{
						if ( CanAddInv( INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
						{
							sDBRequest_DailyQuest3Del clMsg( pclCI->GetPersonID(), GetID(), true, InvPos,
								pclCI->clDailyQuest3Info.siItemUnique ,&clItem, 1, pclCI->clDailyQuest3Info.bMoneySelected, pclCI->clDailyQuest3Info.bHiddenQuest);
							cltServer *pclServer = (cltServer *)pclClient;
							pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						}
						else
						{
							// �κ� �� ���� �ȵǹǷ�...
							cltServer *pclServer = (cltServer *)pclClient;
							pclServer->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  0, 0, GetCharUnique());

							cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum , pclCI->clDailyQuest3Info.bHiddenQuest );
							cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
							pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}


				}
				else
				{
					cltGameMsgResponse_DailyQuest3Change clinfo( pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.siMadeItemNum, pclCI->clDailyQuest3Info.bHiddenQuest );
					cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
			for (SI32 i = 0; i < MAX_SUMMON_OWN; i++)
			{
				cltSummon *pSummon = pclCI->clSummonInfo.GetSummon(i);
				if (pSummon != NULL)
				{
					// PCK - ATB_SUMMONSTAMINASTATIC �Ӽ��� ������ �ڵ�ȸ���� �ǵ��� �߰���. (08.07.18)
					bool	bStaminaRecovery = true;
					cltKindInfo* pclKI = NULL;
					if( pclClient->pclKindInfoSet->IsValidKind(pSummon->siKind) )
					{
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
						{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[pSummon->siKind];	}
						else
						{	pclKI = pclClient->pclKindInfoSet->pclKI[pSummon->siKind];			}

						if( pclKI != NULL && pclKI->IsAtb(ATB_SUMMONSTAMINASTATIC) == TRUE )
						{
							bStaminaRecovery = false;
						}
					}

					if(bStaminaRecovery)
					{
						SI32 serverminutevary = pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime);

						if (pSummon->clIP.GetLuk() <= 0 && 
							TABS(pSummon->siTakeInMinute - serverminutevary) >= SUMMON_TAKEOUT_DELAY)
						{
							pSummon->siTakeInMinute = serverminutevary;

#ifdef _SAFE_MEMORY
							SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclCI->clCharItem.clItem[0], siID );
#else
							SI32 summonskill = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, pclCI->clCharItem.clItem, siID );
#endif
							pSummon->clIP.SetLuk( pclClient->GetMaxStaminaValue(summonskill, pSummon->siKind) );

							SetUpdateSwitch(UPDATE_SUMMON, true, 0);
							SendDBSummon(SETSUMMON_REASON_UPDATE, i);
						}
					}
				}
			}


			// ä�� ���� ó��
			if( m_stMuteTime.wYear > 0 )
			{
				SI64 siTimeDiff = TimeOperation(&m_stMuteTime, &pclserver->sTime);

				// ����ð��� ������ �ð����� �����̸� �����ȴ�.
				if( siTimeDiff <= 0 )
				{
					ZeroMemory(&m_stMuteTime, sizeof(m_stMuteTime));

					// �����Ͱ� �����ϴ� ��쿡�� CharID�� �����Ѵ�.
					sDBRequest_SetMuteTime clMsg(0, pclCI->GetPersonID(), &m_stMuteTime);
					pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
			}
	}

	//---------------------------------
	// �κ��� ��ǰ �߰� ���� ���� ���� 
	//---------------------------------
	if((pclClient->GetFrame() / CHAR_ACTION_INTERVAL_FRAME) % 50 == 0)
	{
		InputItemReserveAction();
	}

	// ��ǰ ����
	if(bMakeItemReserveSwitch)
	{
		if(TABS(pclClient->CurrentClock - dwMakeItemStartClock) > (int)dwMakeItemDelayClock)
		{
			bMakeItemReserveSwitch = false;
			if ( bMakeItemNoMinigame == true )
			{
				bMakeItemNoMinigame = false ;
				MakeItem(pclReservedMakeItemData);
				bMakeItemSwitch = true ;
			}

			if(bEnchantItemSwitch ==true)
			{
				bEnchantItemSwitch = false ;
				EnchantItem(pclReservedMakeItemData);
				bFinishEnchantItemSwitch = true ;
			}

			//-------------------------------------------
			// Ŭ���̾�Ʈ�� ���� �ð��� �뺸�Ѵ�. 
			//-------------------------------------------
			cltGameMsgResponse_MakeItemDelay clinfo(0);
			cltMsg clMsg(GAMEMSG_RESPONSE_MAKEITEMDELAY, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}


	}
	else if ( bMakeItemMinigameSwitch && !bMakeItemNoMinigame && !bMakeItemSwitch )
	{
		bMakeItemMinigameSwitch = false ;
		MakeItem(pclReservedMakeItemData);
		bMakeItemSwitch = true ;
	}

	/*cyj �ڴ�ڰ��� ����
	// �ڴ�� ����
	if( bRouletteStart )
	{
		if(TABS(pclClient->CurrentClock - dwRouletteStartClock) > (int)dwRouletteDelayClock)
		{
			SI32 TicketPos = -1;
			SI32 TicketNum = 0;

			TicketPos = pclCI->clCharItem.GetItemPos(8200);
			TicketNum = pclCI->clCharItem.GetItemNum(8200);

			if(TicketNum >= m_siBettingCount && TicketPos >= 0)
			{
				// ��ι��� �Һ��Ѵ�.
				//////////////////////////////////////////////////////////////////////////
				cltItem clItem;
				clItem.Init();
				clItem.siUnique = 8200;
				clItem.siItemNum = m_siBettingCount;

				SI32 FromPersonID = pclCI->GetPersonID();

				cltServer* pclserver = (cltServer*)pclClient;
				sDBRequest_UseItem clMsg( GetID(), FromPersonID, TicketPos, &clItem ,USEITEM_REASON_ROULLET);
				pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				//////////////////////////////////////////////////////////////////////////

				CheckRoulette();

				bool bLotto = false;
				bLotto = CheckLottoRoulette();

				SI16 siResult = 0;
				SI16 siNumber = 0;

				SI32 ItemUnique = 8200;	// ��ι�

				if( bLotto )
				{
					siNumber = 100;
					GiveItem(ItemUnique, siNumber, INPUTITEMPERSON_OPTION_ROULETTEGAME,0);
					siResult = 2;

					cltCharServer* pclchar = (cltCharServer*)pclCM->CR[GetID()];
					cltServer* pclserver = (cltServer*)pclClient;

					cltItem clItem;
					// ĳ������ �ش�
					SI32 itemunique = pclClient->pclItemManager->GetRandBigWarItem();
					//SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
					if(itemunique > 0)
					{
						//clItem.Set(& pclClient->pclItemManager->pclItemInfo[ref]->clItem);
						bool RareSwitch = false;
						if ( pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clItem, 0, &RareSwitch,
							0,0,0,0,0,14 ) )
						{
							SI32 rtnvalue = 0;
							if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_ROULETTEGAME, &rtnvalue) == false)
							{
								// �κ��� ���ڸ��� ��� �߰��� �� ����. 
								if(rtnvalue == CANADDINV_FAIL_FULL)
								{
									pclserver->SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
								}

								pclClient->pclLog->FilePrint(TEXT("Config\\RouletteGame.Txt"), TEXT("Lotto User : [%s] / BettingCount : [%d]  / UserGet : [%d]"), pclCM->CR[GetID()]->GetName(), m_siBettingCount, siNumber);
							}	
						}
					}
				}
				else 
				{
					if( m_siResultCount > 0 )
					{
						siNumber = m_siResultCount*3;
						GiveItem(ItemUnique, siNumber, INPUTITEMPERSON_OPTION_ROULETTEGAME,0);
						siResult = 1;

						pclClient->pclLog->FilePrint(TEXT("Config\\RouletteGame.Txt"), TEXT("Normal User : [%s] / BettingCount : [%d] / UserGet : [%d]"), pclCM->CR[GetID()]->GetName(), m_siBettingCount, siNumber);
					}
					else
					{
						pclClient->pclLog->FilePrint(TEXT("Config\\RouletteGame.Txt"), TEXT("Normal User : [%s] / BettingCount : [%d] / UserGet : [%d]"), pclCM->CR[GetID()]->GetName(), m_siBettingCount, siNumber);
					}
				}

				cltGameMsgResponse_RouletteGame_Result clinfo(m_siRandomValue, siResult, siNumber);
				cltMsg clMsg2(GAMEMSG_RESPONSE_ROULETTEGAME_RESULT, sizeof(clinfo), (BYTE*)&clinfo);
				SendNetMsg((sPacketHeader*)&clMsg2);
			}

			bRouletteStart = false;

			m_siBettingCount = 0;
			m_siResultCount = 0;
			dwRouletteStartClock = 0;
			dwRouletteDelayClock = 0;
			SI32 x = 0, y = 0;
			for( y = 0 ; y< 5; ++y )
			{
				m_siRandomValue[y] = 0;	// ���õ� ������ 0

				for( x = 0; x < 5; ++x )
				{
					m_stRouletteTable[y][x].m_bUserSelect = false;
					m_stRouletteTable[y][x].m_bCpuSelect  = false;
					m_stRouletteTable[y][x].m_bResult     = false;

					m_stRouletteTable[y][x].m_siX = 0;
					m_stRouletteTable[y][x].m_siY = 0;
					m_stRouletteTable[y][x].m_siValue = y*5 + (x + 1);
				}
			}

		}

	}
	*/
}



// ���� ĳ������ �ൿ. 
void cltCharServer::CharServerAction()
{
	// ��, ���� ���� ���� ĳ���� ó��. 
	if(TABS(pclClient->CurrentClock - clSimpleVary.dwlastCheckCharServerStatusActionClock) > 1000)
	{
		clSimpleVary.dwlastCheckCharServerStatusActionClock = pclClient->CurrentClock;
		CharServerStatusAction();

	}
	// �����̵� 
	if(IsWarpStatus())
	{
		Warp();
	}

	if( pclClient->IsCountrySwitch( Switch_Elixir ) )
	{
		//[����] ������ �Ⱓ�� �Ѿ� ����.
		if( pcltElixir && pcltElixir->IsElixir() )
		{
			if( pcltElixir->GetMinuteVary() < ((cltServer*)pclClient)->pclTime->clDate.GetMinuteVary() )
			{
				if(pclCI->GetPersonID() > 0)
				{
					sDBRequest_Elixir_Set clMsg( GetID(), pclCI->GetPersonID() );
					((cltServer*)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
				}
			}
		}
	}

	
	if( pclClient->IsCountrySwitch( Switch_SkillBook ) )
	{
		//[����] ��ų�� ������ �ð��� ������ �����Ѵ�.
		if( pclCharSkillBookInfo )
		{
			if( pclCharSkillBookInfo->IsTimeOver( this ) )
			{
				ParametaBoxAction();
			}
		}

		if( m_clCharCurseEffect.IsTimeOver( GetID() ) )
		{
			ParametaBoxAction();
		}
	}


	//----------------------------------
	// ���� ������ ������Ų��. 
	//----------------------------------
	siMagicDelay += CHAR_ACTION_INTERVAL_FRAME;

	m_clMagicBufMgr.DecFrame( CHAR_ACTION_INTERVAL_FRAME );

	//----------------------------------
	// NPC�� ��� 
	//----------------------------------
	if(IsNPC(GetCharUnique()) || IsInstanceNPC(GetCharUnique()))
	{
		//===================================
		// SUMMON : ��ȯ���� ������ ���󰣴�.
		//===================================
		if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			if(GetMovePercent() == 0)
			{
				SummonFollowHost();
			}

			SummonAutoHeal();

			CharServerAction_Summon();

		}
		//===================================
		// ��ȣ���� ������ ���󰣴�
		//===================================
		else if ( IDENTITY_SOULGUARD == clIdentity.siIdentity )
		{
			if(GetMovePercent() == 0)
			{
				SoulGuardFollowHost();
			}

			CharServerAction_SoulGuard();
		}

		if(TABS(pclClient->CurrentClock - clSimpleVary.dwLastCheckCharServerNPCActionClock) > 3000)
		{
			clSimpleVary.dwLastCheckCharServerNPCActionClock = pclClient->CurrentClock;

			CharServerAction_NPC();
		}
	}
	// ������̸�, 
	else if( IsPC(GetCharUnique()) )
	{
		CharServerAction_PC();

	}


}

//[�߰� : Ȳ���� 2007. 12. 11 �ų�Ʈ�� ��ġ�� ������ �ֹ� ����ȿ�� �ֱ�. ���� ������]
void cltCharServer::NewYearTreeEft(SI32 _VillageUnique)
{
	SI32			id		= 0;
	SI32			index	= 0;
	SI32			Add	    = 0;
	cltCharServer*	pclchar = NULL;

	while(id = pclCM->GetCharOrder(index))
	{
		index++;
		pclchar = (cltCharServer*)pclCM->CR[id];
		if(pclchar->GetSession())
		{
			// ���� ���� �ֹ��̾�� �Ѵ�. 
			if(pclchar->pclCI->clBI.siHomeVillage == _VillageUnique)
			{
				Add = pclchar->pclCI->clIP.GetLevel() / 10;

				pclchar->clPB.siGeneralAC	  += Add;
				pclchar->clPB.siGeneralDamage += Add;
			}

			// Ŭ���̾�Ʈ���� �˸���. 
			cltGameMsgResponse_GeneralEft clinfo(pclchar->GetCharUnique(), pclchar->clPB.siGeneralAC, pclchar->clPB.siGeneralDamage);
			cltMsg clMsg(GAMEMSG_RESPONSE_GENERALEFT, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNeighbourMsg(&clMsg, true);
		}
	}
}


// ���� ȿ���� �����Ѵ�. 
void cltCharServer::GeneralEft(bool bacswitch, SI32 acuprate, bool bdamageswitch, SI32 damageuprate , bool alluser)
{
	// ���� �ֹ��� �ƴϸ� �۵����� �ʴ´�. 
	if(pclCI->clCharRank.siVillageUnique <= 0 && alluser == false)return ;


	SI32 id;

	SI32 index = 0;
	while(id = pclCM->GetCharOrder(index))
	{
		index++;

		// �ڱ� �ڽ��� �ش���� �ʴ´�. 
		if(id == GetID())continue;


		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if(pclchar->GetSession())
		{

			// ���� ���� �ֹ��̾�� �Ѵ�. 
			if(pclchar->pclCI->clBI.siHomeVillage == pclCI->clCharRank.siVillageUnique || alluser == true )
			{
				// ���� ������ �־�� �Ѵ�. 
				if(pclchar->GetMapIndex() == GetMapIndex() && max(TABS(pclchar->GetX() - GetX()), TABS(pclchar->GetY() - GetY()))  < 20 )
				{

					bool btempacswitch		= false;
					bool btempdamageswitch	= false;

					//-------------------------------------
					// ���� ��� 
					// ������ ����ȿ������ ���ƾ�..
					//-------------------------------------
					if(bacswitch)
					{
						pclchar->clPB.siGeneralAC		= acuprate;
						pclchar->clPB.dwGeneralACClock	= pclClient->CurrentClock;

						// �ڽ��� ������ �������� ���ʽ� ��ġ�� �����Ѵ�.					

						if(pclchar->clPB.pclIP->GetLevel() <20)
							pclchar->clPB.siGeneralAC += 30;
						else if(pclchar->clPB.pclIP->GetLevel() <30)
							pclchar->clPB.siGeneralAC += 20;
						else if(pclchar->clPB.pclIP->GetLevel() <40)
							pclchar->clPB.siGeneralAC += 10;

						btempacswitch = true;
					}

					//-------------------------------------
					// ������ ���. 
					// ������ ����ȿ������ ���ƾ�..
					//-------------------------------------
					if(bdamageswitch)
					{
						pclchar->clPB.siGeneralDamage		= damageuprate;
						pclchar->clPB.dwGeneralDamageClock	= pclClient->CurrentClock;

						// �ڽ��� ������ �������� ���ʽ� ��ġ�� �����Ѵ�.					

						if(pclchar->clPB.pclIP->GetLevel() <20)
							pclchar->clPB.siGeneralDamage += 30;
						else if(pclchar->clPB.pclIP->GetLevel() <30)
							pclchar->clPB.siGeneralDamage += 20;
						else if(pclchar->clPB.pclIP->GetLevel() <40)
							pclchar->clPB.siGeneralDamage += 10;

						btempdamageswitch = true;
					}

					if(btempacswitch || btempdamageswitch)
					{
						// Ŭ���̾�Ʈ���� �˸���. 
						cltGameMsgResponse_GeneralEft clinfo(pclchar->GetCharUnique(), pclchar->clPB.siGeneralAC, pclchar->clPB.siGeneralDamage);
						cltMsg clMsg(GAMEMSG_RESPONSE_GENERALEFT, sizeof(clinfo), (BYTE*)&clinfo);
						pclchar->SendNeighbourMsg(&clMsg, true);
					}


				}
			}
		}
	}

}



//------------------------------------
// �����̵� ���� 
//------------------------------------
void cltCharServer::InitWarpInfo()
{
	clCharWarpInfo.Init();
}

void cltCharServer::SetWarpInfo(const cltWarpInfo* pclCharWarpInfo, SI32 warpunique)
{
	clCharWarpInfo.Set(pclCharWarpInfo);
	siWarpUnique	= warpunique;
}

// Ư�� ������ �����ϰ� �Ѵ�. 
void cltCharServer::SetWarpToVillage(cltMapManager* pclmapmanager, SI16 villageunique, SI32 warpunique)
{
	SI32 id = GetID();
	//����� - 07.07.24 : ĳ���Ͱ� ������ �̵����� �ʴ´�.
	if (pclCM->IsAlive(id) == false) return;

	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;


	SI32 maptypeunique	= pclmapmanager->pclMapArray[villageunique]->siMapUnique;
	if(maptypeunique < 0 || maptypeunique >= MAX_MAPTYPE_NUMBER)		return;

	SI32 x				= pclmapmanager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
	SI32 y				= pclmapmanager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

	OrderStop(GetID(), BY_COMPUTER);

	cltWarpInfo clWarpInfo(1, villageunique, false, false, x, y, -1, -1);
	SetWarpInfo(&clWarpInfo, warpunique);
}


bool cltCharServer::IsWarpStatus()
{

	if(clCharWarpInfo.GetWarpSwitch() == false)return false;

	if(clCharWarpInfo.GetGateX() == -1)
	{
	}
	else
	{

		SI32 length = max( TABS(GetX() - clCharWarpInfo.GetGateX()), TABS(GetY() - clCharWarpInfo.GetGateY()) );
		if(length >= 12)
		{
			return false;
		}

	}

	if (clCharWarpInfo.GetWarpSwitch() == 0 )
		return false;
	else return true ;
}
// ���� �̵���Ų��. 
BOOL cltCharServer::Warp(bool bwritelog /* = false */)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 id = GetID();

	//================================================================
	// ��Ʋ�ξ�
	//================================================================
	CBattleRoyalMgr* pclBattleRoyalMgr = pclserver->m_pclBattleRoyalMgr;
	CBattleRoyal* pclBattleRoyal = NULL;
	SI32 siDstMapIndex = clCharWarpInfo.GetMapIndex();
	if( NULL != pclserver->m_pclBattleRoyalMgr)
	{
		pclBattleRoyal = pclBattleRoyalMgr->GetBattleRoyal(this);
		if( NULL != pclBattleRoyal)
		{
			// ����忡�� ���� �ؼ� ������ �Ұ���
			if( (pclBattleRoyal->GetStadiumMapIndex() == GetMapIndex()))
			{
				if(siKilledMode == KILLEDMODE_BATTLEROYAL)
				{
					clCharWarpInfo.uiMapIndex = MAPINDEX_MAINFIELD;
					clCharWarpInfo.uiX = 499;
					clCharWarpInfo.uiY = 258;

					pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;
					pclBattleRoyal->RestorChar(this);
				}
				else
				{
					if( pclBattleRoyal->IsGamingState())
					{
						return FALSE;
					}
					else
					{
						pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;
						pclBattleRoyal->RestorChar(this);
					}

				}
			}

			// ������ �� �¿��
			if( pclBattleRoyal->GetStadiumMapIndex() == siDstMapIndex )
			{
				pclBattleRoyal->TakeinHorse(this);
				if(FALSE == pclBattleRoyal->TransformChar(this))
				{
					clCharWarpInfo.uiMapIndex = MAPINDEX_MAINFIELD;
					clCharWarpInfo.uiX = 499;
					clCharWarpInfo.uiY = 258;
				}
			}
		}
	}

	// ������ Ȯ���Ѵ�. 
	if(GetMapIndex() == 0)
	{
		pclCI->clBI.clLastMainMapPos.Set(GetX(), GetY());
	}

	//================================================================
	// ��ȯ�� ���� ������ �ʱ�ȭ�Ѵ�.
	//================================================================
	siSelectedCharIndex = -1 ;

	//====================================================================================
	// �����ؼ� �� �������� ���λ����� �ִ� ������ ���� ���Ѵ�.
	//====================================================================================
	if ( bPersonalShopSwitch == true &&  
		pclClient->pclMapManager->pclVillageManager->IsValidVillage(clCharWarpInfo.GetMapIndex()) == true )
	{
		CMinisterHozo *pclminister = (CMinisterHozo *)((cltServer*)pclClient)->pclMinisterMgr->GetMinister(MINISTER_HOZO);
		SI32 siShopNum = pclClient->pclMapManager->pclVillageManager->GetPersonalShopNumber(clCharWarpInfo.GetMapIndex());
		SI08 siShopMode = pclClient->pclMapManager->pclVillageManager->GetPersonalShopNumber(clCharWarpInfo.GetMapIndex());
		if(pclminister  && siShopNum >= pclminister->GetMaxShopNum() )
		{
			bPersonalShopSwitch = false;

			// ���� ������ �����Ѵ�.
			cltGameMsgResponse_StartPersonalShop clinfo(GetCharUnique(), -4,false, PSHOP_MODE_NONE );
			cltMsg clMsg(GAMEMSG_RESPONSE_STARTPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);

		}
	}
	//====================================================================================

	cltMapCommon* pclmap	= pclClient->pclMapManager->GetMapPointer(clCharWarpInfo.GetMapIndex());
	if(pclmap == NULL)return FALSE;

	//====================================================================================
	// ���λ��� ���� �� �����ؼ� �� ������ ���λ��� �Ұ� �����̶�� ���λ����� ����� �����Ѵ�
	//====================================================================================
	if ( (true == bPersonalShopSwitch) || (true == bPremiumPersonalShop) )
	{
		if ( false == pclClient->pclMapManager->IsCanMarketOpenMap(clCharWarpInfo.GetMapIndex()) )
		{
			SetPersonalShopBanner( NULL );
			SetPersonalShopData( false, NULL );

			// ���λ����� ���� �Ǿ��ٰ� �˷���
			pclserver->SendServerResponseMsg( 0, SRVAL_NOTICE_CLOSE_PERSONALSHOP_BY_WARP, 0, 0, GetCharUnique() );
		}
	}

	SI32 mapindex = -1, x, y;
	if(clCharWarpInfo.uiForceSwitch)
	{
		mapindex	= clCharWarpInfo.GetMapIndex() ;
		x			= clCharWarpInfo.GetX();
		y			= clCharWarpInfo.GetY();
	}
	else
	{

		mapindex = clCharWarpInfo.GetMapIndex() ;

		// ���� �ذ��� �� ���� ������ �� ���� ã�´�. 
		if(pclmap->FindEmptyAreaForCharLocate(GetKind(), clCharWarpInfo.GetX(), clCharWarpInfo.GetY(), 30, &x, &y, false) == false)
		{
			TCHAR* pText = GetTxtFromMgr(2818);
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), pText, 
										pclCI->GetPersonID(), GetName(), clCharWarpInfo.GetMapIndex(), clCharWarpInfo.GetX(), clCharWarpInfo.GetY());

			
			// ĳ���͸� �溹������ ���� ������Ų��. 
			mapindex = MAPINDEX_PALACE;
			x	= 27;
			y	= 87;
		}
	}

	if(mapindex < 0)
	{
		return FALSE;
	}

	// ���� ���� ������ �ִ� ���̶��..
	SI32 userLimit = pclClient->pclMapManager->GetUserLimit(mapindex);
	if ( IsPC(GetID()) && userLimit > 0 &&
		pclClient->pclMapManager->IsInstanceMap(mapindex) == false )
	{
		if ( pclClient->pclMapManager->GetUserCount(mapindex) >= userLimit )
		{
			// Ŭ�и��� �ִ��� Ȯ���Ѵ�.
			SI32 cloneMapIndex = pclClient->pclMapManager->GetCloneMap( mapindex );
			if ( cloneMapIndex )
			{
				mapindex = cloneMapIndex;
			}
		}
	}

	if( mapindex == MAPINDEX_PALACE )
	{
		if ( pclClient->siServiceArea == ConstServiceArea_China )
		{
			switch( rand() % 3 )
			{
				case 0:	x = 17; y = 87; break;
				case 1:	x = 37; y = 87; break;
				case 2:	x = 27; y = 96; break;
			}
		}
		else
		{
			x = rand()%7 + 24;
			y = rand()%23 + 68;                
		}
	}

	// ������ �������� ĳ������ ������ �����Ѵ�. 
	BOOL boldsetidswitch = SetIDSwitch;
	DeleteID();

	// ĳ���͸� ������Ų��. 
	pclCI->clBI.SetMapIndex(mapindex);
	pclMap	= pclClient->pclMapManager->GetMapPointer( mapindex );
	SetX(x);
	SetY(y);

	// ĳ������ ���� ������ ������ �ٽ� ǥ���Ѵ�.

	if(boldsetidswitch)
	{
		SetID();
	}

	clMoveInfo.siMovePercent	= 0;
	bBusySwitch					= false;

	//====================================================================================
	// ĳ���͸� ������Ų��. 
	OrderStop(GetID(), BY_PLAYER);

	// �Ӹ��� ä���� ���ش�. 
	SetHeadChat(false, NULL);

	//====================================================================================

	cltOrderDataInfo clOrder;
	clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);

	// �ܺ� ���.
	cltShapeEquipInfo clequip;
	if(IsPC(GetCharUnique()))
	{
		GetShapeEquipInfo(&clequip);
	}


	UI32 uiRequestAddinfo = 0;
	//if(clIdentity.siIdentity) uiRequestAddinfo = 1; 
	//IDENTITY_CASTLE:
	//IDENTITY_VILLAGEGOD: 
	//IDENTITY_VILLAGEGOD_SUMMON:
	//IDENTITY_PERSON_SUMMON:
	//IDENTITY_VILLAGEHUNT:			
	//KHY - 1001 - ��µ��� 5���� ���� ����.
	// �� ĳ���Ϳ��� ������ ����Ǿ��� ������ �ٽ� �α����� �ؾ� ���� �뺸�Ѵ�. 
	SI32 siHorseUnique		= 0;
	SI32 siAddStatusDate	= 0;
	SI32 siTicketNum		= 0;
	// �����̾� ���� ���� ����
	SI32 siPremiumpartsslot1		= 0;
	SI32 sipremiumpartsuniqueslot1	= 0;
	SI32 siPremiumpartsslot2		= 0;
	SI32 sipremiumpartsuniqueslot2	= 0;
	SI32 siPremiumpartsslot3		= 0;
	SI32 sipremiumpartsuniqueslot3	= 0;
	SI32 siPremiumpartsNum			= 0;

	cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclHorse != NULL)
	{
		siHorseUnique				= pclHorse->siHorseUnique;
		siAddStatusDate				= pclHorse->clAddStatusDate.GetDateVary();
		siTicketNum					= pclHorse->siStatusTicketNum;

		siPremiumpartsslot1			= pclHorse->clPremiumPartsDate_Slot1.GetDateVary();
		sipremiumpartsuniqueslot1	= pclHorse->siPremiumUnique_Slot1;

		siPremiumpartsslot2			= pclHorse->clPremiumPartsDate_Slot2.GetDateVary();
		sipremiumpartsuniqueslot2	= pclHorse->siPremiumUnique_Slot2;

		siPremiumpartsslot3			= pclHorse->clPremiumPartsDate_Slot3.GetDateVary();
		sipremiumpartsuniqueslot3	= pclHorse->siPremiumUnique_Slot3;

		siPremiumpartsNum			= pclHorse->siPremiumPartsNum;
	}

	//[����] ��ƼȦ. ��Ƽ�忡�� ���� �������� ����. => 2008. 3. 12
	PartyHallItemRemove( id );

	if( !pclClient->IsCountrySwitch( Switch_Festival_Food ) )
	{
		//[����] �������� ������ ����.
		FestivalFoodItemRemove( id );
	}

	//[����] ����ũ�ι��� ���� ����Ҽ� ���� �Ÿ� ���� => 2008-8-14
	if( pclClient->IsCountrySwitch( Switch_FourLeaf ) )
	{
		NotFourLeafItemRemove( id );
	}

	SI32 BufTransformKind = 0;
	if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) )
	{
		stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf( BUF_TRANSFORM );
		if(pclBuf)		BufTransformKind = pclBuf->siPercent;
	}

	/*if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM_HORSE ) )
	{
		stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf( BUF_TRANSFORM_HORSE );
		if(pclBuf)		BufTransformKind = pclBuf->siPercent;
	}*/

	// pvp �� ���� �ϸ� ���°Ŵ�. - ���� ���� ����	
	if ( siWarpUnique != 10000 	&& pclCM->CR[id]->siEnemUnique > 0 && pclCM->CR[id]->GetPVP_LeagueEnemyPersonID() <= 0 ) 
	{
		SI32 siWarpChar	= id;													// ������ �ɸ��� �� �ɸ���.
		if( pclCM->IsValidID( id ) == false )	return	false;					// ���� ���̵� ���� �˻� ���� 
		
		SI32 siWarpChaeParentID = pclCM->CR[id]->GetSummonParentID();			// ����ź �ɸ����� �θ� �ɸ��͸� ���Ѵ�.
		if ( siWarpChaeParentID == 0 ) siWarpChaeParentID = id;					// ���� ����ź �ɸ��Ͱ� ���ɸ� �׳� ���̵� ����.
		
		cltCharServer* pclWarpChar = pclCM->GetCharServer(siWarpChaeParentID);	
		if(pclWarpChar == NULL)	return false;
		
		SI16 siWinnerCharID = pclWarpChar->siEnemUnique;						// ���� Ÿ�� �����Ƿ� ����ź �ɸ����� ���� ���� ������ �̱� �ɸ����̴�.
		if( pclCM->IsValidID( siWinnerCharID ) == false ) return	false;		// ���� ���̵� ���� �˻� ����
		
		cltCharServer* pclWinnerChar = pclCM->GetCharServer(siWinnerCharID);	
		if(pclWinnerChar == NULL) return false;

		// ���� ſ���ϱ� ������ ���̴�.
		pclWarpChar->siEnemUnique	=	0;		// ������ �ɸ��� ���� �ʱ�ȭ ���ش�
		pclWinnerChar->siEnemUnique	=	0;
		if( pclClient->IsCountrySwitch( Switch_ValenTineWar ) )
		{
			//�߷�Ÿ�ε��� ���� ���� üũ, Ŀ���̳� �ַζ�� ���߰�, �������� ���� �ʾ��� ��쿡�� �ش�.
			SI32 siCouple  = pclWinnerChar->pclValentineWar->GetCouple();	// �̱� ����� Ŀ�� ����
			SI32 siLoser   = pclWarpChar->pclValentineWar->GetCouple();		// �� ����� Ŀ�� ����
			bool bGetPrize = pclWinnerChar->pclValentineWar->GetPrize();	// �̱����� �������� �޾Ҿ�����
			if(  abs(pclWarpChar->pclCI->clIP.GetLevel() - pclWarpChar->pclCI->clIP.GetLevel() ) < 11
				&& siCouple != 0 && siLoser != 0
				&& siCouple != siLoser 
				&& bGetPrize == false )
			{
				SI32 siRewardItemUnique	=	ITEMUNIQUE(2781) ;
				if( siRewardItemUnique ) 
				{
					cltItem clItem;
					SI16 brareswitch = 0;
					if ( pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique,&clItem,0,0,&brareswitch,0,0,0,0,0) == true ) 
					{
						clItem.siItemNum = 1;
						SI32 rtnvalue = 0;
						if(pclWinnerChar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, &rtnvalue))
						{
							// �������� �ְ� ��ٰ� ���� ���ش�.
							sDBRequest_Valantine_Get_Prize clMsg( pclWinnerChar->pclCI->GetPersonID(), true );
							pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
						}
						else
						{
							if(rtnvalue == CANADDINV_FAIL_FULL)
							{
								sDBRequest_PostOfficeMasterSendItem clMsg( INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, 0, (TCHAR*)pclWinnerChar->GetName(), &clItem, &pclserver->pclTime->clDate );
								pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
								// �������� �ְ� ��ٰ� ���� ���ش�.
								sDBRequest_Valantine_Get_Prize clMsg2( pclWinnerChar->pclCI->GetPersonID(), true );
								pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
							}
						}
					}		
				}
			}
		}
		// ���� Ŭ���̾�Ʈ�� �˷��ش�.
		{//�¸���
			// ���ΰ� pvp�� �����鼭 ���� �ߵ����� ����
			pclWinnerChar->DeletePoison();						// �ߵ� ���¸� �����Ѵ�.
			pclWinnerChar->DeletePoisonSetItemEffect();			// �ߵ� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteIce();							// ���� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteCurseAttack();					// ���ݷ��� ���� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteCurseCritical();				// Critical Ȯ���� �������� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteCurseDefense();				// ������ �������� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteCurseMovespeed();				// �̵��ӵ� ���� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteMagicFire();					// �ҿ� Ÿ�� ���¸� �����Ѵ�.
			pclWinnerChar->DeleteFrozen();						// ���� ���� ���¸� �����Ѵ�.
		}
		{	// �й���
			// ���ΰ� pvp�� �����鼭 ���� �ߵ����� ����
			pclWarpChar->DeletePoison();						// �ߵ� ���¸� �����Ѵ�.
			pclWarpChar->DeletePoisonSetItemEffect();			// �ߵ� ���¸� �����Ѵ�.
			pclWarpChar->DeleteIce();							// ���� ���¸� �����Ѵ�.
			pclWarpChar->DeleteCurseAttack();					// ���ݷ��� ���� ���¸� �����Ѵ�.
			pclWarpChar->DeleteCurseCritical();					// Critical Ȯ���� �������� ���¸� �����Ѵ�.
			pclWarpChar->DeleteCurseDefense();					// ������ �������� ���¸� �����Ѵ�.
			pclWarpChar->DeleteCurseMovespeed();				// �̵��ӵ� ���� ���¸� �����Ѵ�.
			pclWarpChar->DeleteMagicFire();						// �ҿ� Ÿ�� ���¸� �����Ѵ�.
			pclWarpChar->DeleteFrozen();						// ���� ���� ���¸� �����Ѵ�.
		}
		// �¸���
		cltGameMsgResponse_PVP_Request clWinner( END_PVP , WIN_PVP, pclWinnerChar->GetCharUnique(), TEXT("") );	
		cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clWinner), (BYTE*)&clWinner );
		pclWinnerChar->SendNeighbourMsg(&clMsg, true);

		pclWinnerChar->IncQuestPara( QUEST_CLEAR_COND_PVPWIN, 0, 1 );

		// �й���
		cltGameMsgResponse_PVP_Request clclLoser( END_PVP , LOSE_PVP, pclWarpChar->GetCharUnique(), TEXT("") );
		cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clclLoser), (BYTE*)&clclLoser );
		pclWarpChar->SendNeighbourMsg(&clMsg2, true);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : �ռ����� 2008. 10. 29] // ������ �й�
			// PCK : ���� - PersonID�� ĳ�����̸��� ���ؼ� ���� ���� �����Ѵ�. (09.07.31)
			// para1	:	�¸��� personID
			// para2	:	�й��� personID

			TCHAR szCharName[100];
			ZeroMemory(szCharName, sizeof(szCharName));
			StringCchPrintf(szCharName, sizeof(szCharName), TEXT("WinnerChar:%s, LoserChar:%s"), pclWinnerChar->GetName(), pclWarpChar->GetName());

			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP, LOGCOMMAND_INDEX_PERSONALPVP_END_WARP, 
											0, this, 0 , 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclWinnerChar->pclCI->GetPersonID(), 
											pclWarpChar->pclCI->GetPersonID(), 0, 0, 0, szCharName, NULL);
		}
		{
			// ������ �����.
			TCHAR fileNameBuffer[256];
			//���Կ� ���� �ð��� �ؽ�Ʈ�� �ϼ��Ѵ�.
			TCHAR		szTime[128];
			TCHAR		szBuffer0[128];
			SYSTEMTIME	stTimeInfo;
			GetLocalTime(&stTimeInfo);
			StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
			StringCchPrintf( szBuffer0, sizeof(szBuffer0), "%s", &szTime ) ;
			StringCchPrintf( fileNameBuffer, sizeof(fileNameBuffer), TEXT("Config\\PVPLog_%04d%02d%02d.log"), 
							 stTimeInfo.wYear, stTimeInfo.wMonth, stTimeInfo.wDay );
			 pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), TEXT(" PVPEND_WARP \tWINOPERSON : \t[%s]\t \t LOSEPERSON2 : \t[%s]\t Time"),
											pclWinnerChar->GetName(), pclWarpChar->GetName(), szBuffer0 );
		}
	}

	// �Ʒ� ǥ�õ� ������ ������ �� ĳ���� ���� ���¿� ���� �ƿ� �Ⱥ��̰� �ɼ��� �ִ�
	SI16 siDisplayLoveLevel	= 0;
	//BOOL bSameMap			= FALSE;

	SI16 siConnectMode		= MARRIAGE_MATE_CONNECT_MODE_NONE;

	//SI16 siSameMap			= 0;
	SI16 siLoveLevel		= 0; 

	// [����] ��ȥ���¶�� ������ �� ĳ���Ͱ� ���� �ʻ� ���� �ϴ��� �˻��Ѵ�
	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		if ( (pclMarriage) && (TRUE == IsMarried()) )
		{
			siLoveLevel	= pclMarriage->GetLoveLevel(); 
			//
			SI32			siMateCharID	= pclClient->pclCM->GetIDFromPersonID( pclMarriage->GetMatePersonID() );
			cltCharServer*	pclMateChar		= pclClient->pclCM->GetCharServer( siMateCharID );

			// ĳ���Ͱ� ������ �ִٸ� ���� �ʿ� �ִ��� �˻�
			if ( (pclMateChar) && (pclMateChar->IsMarried()) && (pclMateChar->pclMarriage->GetMatePersonID() == pclCI->GetPersonID()) )
			{
				siConnectMode = MARRIAGE_MATE_CONNECT_MODE_CONNECT;

				// ���� �ʿ� �ִ�
				if ( GetMapIndex() == pclMateChar->GetMapIndex() )
				{
					siConnectMode = MARRIAGE_MATE_CONNECT_MODE_SAMEMAP;

					// �������Ե� ������ ������ ���� ������ ����
					siDisplayLoveLevel = pclMarriage->GetLoveLevel();
				}

				// �� ĳ������ ������ �Ʒ����� ����������
				// ���� �����ִ� ĳ������ ������ �Ⱥ����ֱ� ������ �� ������ �Բ� ������� �Ѵ�
				if ( (pclMateChar->pclMarriage) && (pclMateChar->pclMarriage->GetConnectMode() != siConnectMode) )
				{
					pclMateChar->pclMarriage->SetConnectMode( siConnectMode );
					
					cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_DISPLAYLOVELEVEL, false, siDisplayLoveLevel );
					cltGameMsgResponse_StatusInfo	clInfo( pclMateChar->GetCharUnique(), &clChangeInfo );

					cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo );
					pclMateChar->SendNeighbourMsg( &clStatusInfoMsg, false );

					// ����ڿ��Դ� ���� ������ �������� ���� ������
					pclserver->SendServerResponseMsg( 0, SRVAL_SET_MARRY_WITHSAMEMAP, siConnectMode, siLoveLevel, pclMateChar->GetCharUnique() );
				}
			}	// if ( pclMateChar )
		}	// if ( (pclMarriage) && (TRUE == IsMarried()) )
	}	// if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )

	cltMakeCharInfo	clMakeCharInfo(GetKind(), 
									CharUnique, 
									pclCI->clIP.GetLevel(), 
									pclCI->clIP.GetLife(), 
									GetX(), 
									GetY(), 
									GetMapIndex(), 
									siChildCharUnique, 
									siSummonIndex, 
									bHeadChatSwitch, // �Ӹ����� �����ϴ°�?
									bPersonalShopSwitch,		
									bNoFirstAttackSwitch | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK) | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI) | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) | pclCI->clBI.clBufInfo.IsHaveBufExecptGetType(BUF_TRANSFORM,BUF_GET_TYPE_NIGHTMARKET) ,
									bNoIceAttackSwitch,
									pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) && pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP),
									pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FOURLEAF) ,
									pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER),
									IsHaveCushionItem() ,
									pclCI->clBI.uiSex, 
									&clequip, 
									pclCI->clHorseInfo.siCurrentShowHorse,
									clPB.GetTotalMoveSpeed(),
									clJim.siJimHostCharUnique,
									GetGameMasterLevel(),
									pclClient->pclMapManager->pclMapArray[GetMapIndex()]->siMapUnique,
									siHorseUnique,
									pclCI->clBI.siHomeVillage,
									pclCI->clBI.siSchoolUnique,
									&pclCI->clCharRank, 
									siAddStatusDate,
									siTicketNum,
									&clOrder,
									&clIdentity, 
									&clInnerStatus,
									uiRequestAddinfo,
									clCharWarpInfo.uiTicketUseSwitch,
									pclCI->clBI.uiGMMode,
									pclCI->clBI.bPCRoom,
									siPersonalMakeItemType,
									&pclCI->clBI.clGuildRank,
									&pclCI->clPersonNaming,
									pclCI->clHorseInfo.siCurrentHorseRIDEHORSE,
									clTransFormInfo.uiNowTransFormKind,
									clTransFormInfo.bTransFormMode,
									uiBestGuild,										// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
									bIsElectionStump,
									clTransFormInfo.bTransForm100Effect,
									BufTransformKind,
									siEnemUnique,
									pclValentineWar,
									bElixirSwitch
									,pclPVP_LeagueInfo
									,siPremiumpartsslot1
									,sipremiumpartsuniqueslot1
									,siPremiumpartsslot2
									,sipremiumpartsuniqueslot2
									,siPremiumpartsslot3
									,sipremiumpartsuniqueslot3
									,siPremiumpartsNum
									,m_uiWeaponSkillMasterEmblem
									,siDisplayLoveLevel
									,siMySoulGuardUnique
									,bRepairShopSwitch
									,&pclCI->clBI.clBufInfo);

	cltMsg clMsg(GAMEMSG_RESPONSE_LOGIN, sizeof(clMakeCharInfo), (BYTE*)&clMakeCharInfo);

	SendNetMsg((sPacketHeader*)&clMsg);

	// SameMap ���°� ����ɶ��� �������� �Ѵ�
	if ( (pclMarriage) && (TRUE == IsMarried()) && (pclMarriage->GetConnectMode() != siConnectMode) )
	{
		pclMarriage->SetConnectMode( siConnectMode );

		pclserver->SendServerResponseMsg( 0, SRVAL_SET_MARRY_WITHSAMEMAP, siConnectMode, siLoveLevel, GetCharUnique() );
	}
	
	// Ŭ���̾�Ʈ�� �غ�ɶ����� ��ٸ��� �Ѵ�. 
	bReadySwitch		= false;
	bFirstCellOpSwtich	= false;

	if(bwritelog)
	{
		// ������ ����ϰ� �Ͼ�Ƿ� �� �ʿ��� ��츸 �α׸� ������ ����.
		SI32 mapunique = pclClient->pclMapManager->GetMapUnique(clCharWarpInfo.GetMapIndex());
		((cltServer*)pclClient)->SendLog_Warp(pclCI->GetPersonID(), mapunique);
	}

	// ���� ������ �ʱ�ȭ�Ѵ�. 
	clCharWarpInfo.SetWarpSwitch(0);

	// �ֺ��� ĳ���͵鿡�� ������ ���� ������ �˸���. 
	pclClient->pclMapManager->m_world.GetMap( GetMapIndex() )->Warp( GetX(), GetY(), this, clCharWarpInfo.uiTicketUseSwitch );


	if( siPartyIndex > 0)
	{
		CPartyObj* pParty =	pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
		if( pParty ) 
			pParty->NotifyMovePosToPartyMember(GetCharUnique(),x,y,true);
	}

	// [����] ĳ���� ���� �̵��� ��ġ�� �α׿� �����.
	SendDB_PosLifeMana();

	if(clCharWarpInfo.GetMapIndex() > 88 && clCharWarpInfo.GetMapIndex() < 300)
	{
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->pclStatisticsManager->clDailyStatistics.siDungeonUserNumber ++;
	}
		
	/*
	else if ( clCharWarpInfo.GetMapIndex() >= MAPINDEX_DAEMADO && clCharWarpInfo.GetMapIndex() < MAPINDEX_DAEMADO + MAX_INSTANCE_MAP_NUMBER )
	{
		cltServer* pclserver = ( cltServer*)pclClient;
		pclserver->pclStatisticsManager->clDailyStatistics.siInstanceDungeonUserNumber ++;
	}*/

		// ������ ������ �� ���̶��, 
		if(pclClient->pclVillageManager->IsValidVillage( GetMapIndex() ))
		{
			// ������ ���� ������ Ȯ���Ѵ�. 
			SI32 x = pclClient->pclVillageManager->pclVillageInfo[ GetMapIndex() ]->PosX;
			SI32 y = pclClient->pclVillageManager->pclVillageInfo[ GetMapIndex() ]->PosY;

			if(TABS(pclClient->CurrentClock - dwLastVisitVillageClock) > 300000)
			{
				dwLastVisitVillageClock = pclClient->CurrentClock;
				pclClient->pclVillageManager->pclVillageInfo[ GetMapIndex() ]->IncVisitNum(1);
			}

			pclCI->clBI.clLastMainMapPos.Set(x, y);

			// �ش縶���� �������ΰ� ?
			if(pclClient->pclCityHallManager->IsGeneralMeeting( GetMapIndex()))
			{
				TCHAR* pText = GetTxtFromMgr(2819);
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, GetCharUnique(), pText);
			}

			if(pclClient->pclCityHallManager->IsPrepareBankruptcy( GetMapIndex()))
			{
				TCHAR* pText = GetTxtFromMgr(10182);
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, GetCharUnique(), pText);
			}

		}

		// �����ϸ� �����¿��� pvp�� ���� pvp-
		clInnerStatus.clEnemyType.siEnemyType = ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR;
		
		if( pclClient->pclMapManager->IsInstanceMap( GetMapIndex() ) )
		{
			SI32 siParentMap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
			cltMapCommon* pclParentMap = pclClient->pclMapManager->GetMapPointer( siParentMap );
			if( pclMap != NULL )
			{
				SI16 siMapType = pclClient->pclMapManager->GetInstanceMapType( siParentMap );

				if( pclParentMap->m_siInstanceMapGateIndex >= 0 )
				{
					cltInstanceMapGate	clGates[MAX_INSTANCEMAPGATE_COUNT];

					SI32 siCount = pclClient->pclMapManager->pclInstanceMapGateManager->GetInstanceMapGates( 
						GetMapIndex(), pclParentMap->m_siInstanceMapGateIndex,
						clGates, MAX_INSTANCEMAPGATE_COUNT );

					cltGameMsgResponse_InstanceMap_GateInfo clinfo(clGates, siCount);
					cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTMAP_GATEINFO, clinfo.GetSize(), (BYTE*)&clinfo);
					SendNetMsg((sPacketHeader*)&clMsg);
				}

				switch ( siMapType ) 
				{
				case INSTANCEMAP_TYPE_GUILD:
					{
						cltGameMsgResponse_cInstantsDungeonRunInfo	clInfo(pclserver->CurrentClock - pclParentMap->m_dwInstanceMapStartTime, pclParentMap->m_siInstanceMapScore, pclClient->pclGuildManager->siTopClearPoint, DUNGEONRUN_START, siMapType);
						cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg( (sPacketHeader*)&clMsg );
					}
					break;

				case INSTANCEMAP_TYPE_PARTY:
					{
						DWORD	dwTime			= pclserver->CurrentClock - pclParentMap->m_dwInstanceMapStartTime;
						SI32	siKillCount		= (pclParentMap->m_siMaxMonsterNum - pclParentMap->m_siKillCount);
						SI32	siBonusCount	= 0;

						cltGameMsgResponse_cInstantsDungeonRunInfo	clInfo(dwTime, siKillCount, siBonusCount, DUNGEONRUN_START, siMapType);
						cltMsg clMsg(GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg( (sPacketHeader*)&clMsg );
					}
					break;
				}
			}
		}

		return TRUE;
}

// ��������� �̵���Ų��. 
void cltCharServer::WarpToSafeZone()
{
	// ��������� ���� ��Ų��. 
	OrderStop(GetID(), BY_COMPUTER);

	//if ( pclClient->siServiceArea == ConstServiceArea_China )
	//{
	//		cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
	//		SetWarpInfo(&clWarpInfo, 56);
	//		// ������ ���� ��Ų��. 
	//		Warp();
	//}
	//else
	//{
	//	if ( pclKI->siDefaultCountry == ConstServiceArea_China )
	//	{
	//		cltWarpInfo clWarpInfo(1, MAPINDEX_CHINABORDER, false, false, 17, 19, -1, -1);	// �溹�� ��������� ����. 
	//		SetWarpInfo(&clWarpInfo, 56);
	//		// ������ ���� ��Ų��. 
	//		Warp();
	//	}
	//	else
	//	{
	cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
	SetWarpInfo(&clWarpInfo, 56);
	// ������ ���� ��Ų��. 
	Warp();
	//}
	//}
}


//----------------------------------
// Skill
//----------------------------------
// ��ų�� ���ʷ� �����Ѵ�. 
BOOL cltCharServer::DevelopSkill(SI32 skillunique)
{
	// ĳ������ ��ų�� �����Ѵ�. 
	return pclCI->clCharSkill.MakeSkill(skillunique); 
}




bool cltCharServer::IsDeathPenaltyCondition()
{
	// �ν��Ͻ� ���������� ���Ƽ�� �ִ�.
	if( siKilledMode == KILLEDMODE_INSTANCEMAP )		return true;
	if(siKilledMode > 0)
		return false;

	return true;
}

// ����� ���� ���Ƽ. 
void cltCharServer::DeathPenalty(SI32 killedmode, TCHAR* szkillername)
{
	if(bDeathPenaltySwitch == true)		return ;

	bDeathPenaltySwitch = true;
	bRequestReviveSwitch = true;

	// ��� �ð��� ����Ѵ�. 
	DeathClock = pclClient->CurrentClock;
	// ���� 20�̸������� ����� ���� ���Ƽ�� ����. 
	if(pclCI->clIP.GetLevel() < FATHER_LEVEL )	return ;	
	GMONEY changedmoney = 0;
	SI32 changedexp = 0;

	if( pclCI->clIP.GetLevel() >= FATHER_LEVEL )
	{
		if( 0 < GetLife() )
			return;

		if( IsDeathPenaltyCondition() == false )
			return ;

		//  ������� �г�Ƽ �ٽ� ����
		if( pclClient->pclMapManager->pclVillageManager->IsValidVillage(GetMapIndex()) == true )			return;

		// [����] ���λ������� : ��� �г�Ƽ�� ����
		if ( GetMapIndex() == MAPINDEX_PRIVATEMARKETAREA )			return;

		if( GetMapIndex() == MAPINDEX_PALACE )						return;

		// ���������� ������ ������ �г�Ƽ�� ����
		for(SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
		{
			cltItem * pclitem = &pclCI->clCharItem.clItem[i];	if( pclitem->siUnique <= 0 ) continue;
			// ���������� �ִ�.
			if(	pclitem->siUnique == ITEMUNIQUE(2731)	&& pclitem->GetItemNum() > 0 )	return	;
		}

		// ������ �ٿ��ش�. (10%)
		changedmoney = pclCI->clIP.GetMoney()/10;

		// ����ġ�� �ٿ��ش�.(level * 10)
		changedexp = pclCI->clIP.GetLevel() * 10;
		pclCI->clIP.DecreaseExp( changedexp );

		// Ŭ���̾�Ʈ�� ������ ������.
		// ����ġ ������� �뺸�Ѵ�. 
		{
			cltGameMsgResponse_GetExp clinfo(GetCharUnique(),  -changedexp,  0, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo), (BYTE*)&clinfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}


		// ���������� ��������� �ι� ó������ �ʰ� �������� ä���(Ŭ���̾�Ʈ�δ� ������ �ʴ´�) -KKM-
		//	pclCI->clIP.SetLife( clPB.GetMaxLife() );
		SendDBLevelInfo(0, 0);

		// ������ ���ϰ��� DB�� �䱸�Ѵ�. 
		{
			sDBRequest_ChangeMoney clMsg(GetID(), pclCI->GetPersonID(), CHANGE_MONEY_REASON_DEATH, -changedmoney, szKillMeCharName );
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


			// ��ȭ�� ���� 
			cltServer* pclserver = (cltServer*)pclClient;
			cltMoney clmoney;
			clmoney.Set(changedmoney);
			pclserver->clCurrencyInfo.clOutputMoney_Dead.Increase(&clmoney);

			pclserver->pclDelMoneyStatistics->Add(DELMONEY_DEATHPANELTY, changedmoney);
		}

		// ��尡 �ִٸ� GP�� ���
		//{
		//	// ��尡 �ִٸ� GP�� ������Ʈ�Ѵ�.
		//	if ( pclCI->clBI.clGuildRank.IsGuildMember() == true )
		//	{
		//		cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
		//		if ( pGuild )
		//		{
		//			sDBRequest_Guild_ChangeGuildPoint clMsg(pclCI->clBI.clGuildRank.GetGuildUnitIndex(),pclCI->GetPersonID(),-3,TYPE_GUILDPOINT_REMOVE_DEAD);
		//			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		//		}
		//	}
		//}

	}

	if(killedmode == -1)
	{
		killedmode = siKilledMode;
	}

	TCHAR* pkillername = szKillMeCharName;
	if(szkillername)
	{
		pkillername = szkillername;
	}

	cltGameMsgResponse_DeathPenalty clInfo(changedmoney, changedexp, killedmode, pkillername);
	cltMsg clMsg(GAMEMSG_RESPONSE_DEALTHPENALTYINFO, sizeof(clInfo), (BYTE*)&clInfo);
	// Ŭ���̾�Ʈ�� �޽����� ������. 
	SendNetMsg((sPacketHeader*)&clMsg);
}

void cltCharServer::RequestRevive(SI32 id, SI16 DieMoode)
{
	if (bRequestReviveSwitch)
	{
		bRequestReviveSwitch = false;
		cltGameMsgResponse_Revive clInfo(id, 1,DieMoode);
		if( siKilledMode == KILLEDMODE_INSTANCEMAP )
		{
			clInfo.siParam = 1000;
		}
		cltMsg clMsg(GAMEMSG_RESPONSE_REVIVE, sizeof(clInfo), (BYTE*)&clInfo);
		SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltCharServer::Revive()
{

	// ���� �ð��� ����� ������ ��ٸ���. 
	//if(TABS(pclClient->CurrentClock - DeathClock ) < 5000)return ;
	DeathClock = pclClient->CurrentClock;

	bool	bWarped = false;

	SI32 villageunique = 0;
	if(siKilledMode == KILLEDMODE_VILLAGEWAR)
	{
		villageunique = pclCI->clBI.siHomeVillage;
	}
	else
	{
		villageunique = GetCurrentVillageUnique();
	}

	if(villageunique <= 0 )
	{
		// ���� ��ġ�� ������ �θ� ������ ã�´�. 
		SI32 parentmap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
		if(parentmap == 0)
			parentmap = GetMapIndex();

		//cyj ������ġ�� �������� ��� parentmap�� villageunique ��
		if(pclClient->pclMapManager->IsHuntMap( GetMapIndex() ) == true)
		{
			villageunique = parentmap;
		}
		// by LEEKH ���� ��ġ�� �δ��� ��� �Ѿ����� ������.
		// �Ϲ������� �� ��ġ�� ������ �ʴ´�.
		else if ( pclClient->pclMapManager->IsInstanceMap( GetMapIndex() ) == true && 
			siKilledMode != KILLEDMODE_INSTANCEMAP )
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
			SetWarpInfo(&clWarpInfo, 56);
			Warp();

			bWarped = true;
		}
		else
		{
			// �θ� ������ �߿����� ��ǥ�� ã�´�. 
			SI32 x, y;
			if( parentmap != 0 && pclClient->pclMapManager->pclGateManager->FindMainFieldPos(parentmap, &x, &y) == true )
			{

			}
			else
			{
				x	= pclCI->clBI.clLastMainMapPos.siX;
				y	= pclCI->clBI.clLastMainMapPos.siY;
			}

			// ����� ������ ã�´�.
			villageunique = pclClient->pclVillageManager->FindNearVillage(x, y);
		}
	}

	// �������� ä���. 
	pclCI->clIP.SetLife( clPB.GetMaxLife() );

	// ���� ���·� �����.
	SetCurrentOrder( ORDER_REVIVE );

	// �δ����� ���� ���
	if( siKilledMode == KILLEDMODE_INSTANCEMAP )
	{
		SI32 siMapIndex		= GetMapIndex();
		SI32 siParentMap	= pclClient->pclMapManager->GetParentMap( siMapIndex );
		SI16 siMapType		= pclClient->pclMapManager->GetInstanceMapType( siParentMap );
		SI32 siSourceMap	= 0;

		if( pclClient->pclMapManager->HaveCloneMap( siParentMap ) == true )
		{
			siSourceMap = siParentMap;
		}
		else
		{
			siSourceMap = pclClient->pclMapManager->GetSourceMap( siParentMap );			
		}

		// [����] ������ : ������ �ȿ��� �׾��� ��� ��ġ�� ������ ������ġ��
		if ( siMapType == INSTANCEMAP_TYPE_PARTY )
		{
			SI32		siWarpX = PIRATESHIP_START_POSITION_X + (rand() % 5);
			SI32		siWarpY = PIRATESHIP_START_POSITION_Y + (rand() % 5);
			cltWarpInfo clWarpPirateShip(1, siParentMap, false, false, siWarpX, siWarpY, -1, -1);

			SetWarpInfo( &clWarpPirateShip, 1024 );
			Warp();

			bWarped = true;
		}
		else if ( siMapType == INSTANCEMAP_TYPE_GUILD )
		{
			for (SI32 i=0; i<MAX_GATE_NUMBER; i++)
			{
				cltGate* pclGate = &pclClient->pclMapManager->pclGateManager->clGate[i];
				if( pclGate != NULL && pclGate->FOUnique > 0 &&	pclGate->DstMapIndex == siSourceMap )
				{
					cltWarpInfo clWarpInfo(1, siParentMap, false, false, pclGate->DstX, pclGate->DstY, -1, -1);
					SetWarpInfo(&clWarpInfo, 56);
					Warp();

					bWarped = true;
				}
			}
		}
		else
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
			SetWarpInfo(&clWarpInfo, 56);
			Warp();

			bWarped = true;
		}

		if( bWarped == false )
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
			SetWarpInfo(&clWarpInfo, 56);
			Warp();

			bWarped = true;
		}
	}

	if( siKilledMode == KILLEDMODE_GOTOSAFEZONE )
	{
		cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
		SetWarpInfo(&clWarpInfo, 56);

		Warp();

		bWarped = true;
	}

	if( bWarped == false && pclCI->clIP.GetLevel() < INIT_LEVEL  )//�� 10������ �溹�ÿ��� ��Ƴ���.
	{
		//if ( pclClient->siServiceArea == ConstServiceArea_China )
		//{
		//	cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �ڱݼ� ��������� ����. 
		//	SetWarpInfo(&clWarpInfo, 56);
		//}
		//else
		//{
		//	if(pclKI->siDefaultCountry == ConstServiceArea_Korea)
		//	{
		//		cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
		//		SetWarpInfo(&clWarpInfo, 56);
		//		// ������ ���� ��Ų��. 
		//	}
		//	else
		//	{
		//cyj cltWarpInfo clWarpInfo(1, MAPINDEX_CHINABORDER, false, false, 17, 19, -1, -1);	// �溹�� ��������� ����. 
		cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
		SetWarpInfo(&clWarpInfo, 56);
		// ������ ���� ��Ų��. 

		//}
		//}

		Warp();

		bWarped = true;
	}
	// pvp ���Ÿ ������ ���Ŀ�
	if(pclClient->IsCountrySwitch(Switch_PKHuntMap_Change_Info)	== true )
	{
		if( pclClient->pclMapManager->IsPkMap( GetMapIndex() )== true )
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);	// �溹�� ��������� ����. 
			SetWarpInfo(&clWarpInfo, 56);

			Warp();

			bWarped = true;
		}
	}
	if( bWarped == false && villageunique > 0 && villageunique < MAX_VILLAGE_NUMBER )
	{

		SI32 mapindex	= pclClient->pclVillageManager->pclVillageInfo[villageunique]->siVillageUnique;

		SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[ mapindex ]->siMapUnique;
		SI32 centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
		SI32 centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

		OrderStop(siID, BY_COMPUTER);

		cltWarpInfo clWarpInfo(1, mapindex, false, false, centerx, centery, -1, -1);
		SetWarpInfo(&clWarpInfo, 200);

		// ������ ���� ��Ų��. 
		Warp();
	}


	SendDB_PosLifeMana();	

	// Ŭ���̾�Ʈ�� ������Ʈ �Ѵ�. 
	SetUpdateSwitch(UPDATE_IP, true, 0);

	// ĳ������ ���λ����� �ʱ�ȭ�Ѵ�.
	SetPersonalShopBanner(NULL);
	SetPersonalShopData(false, NULL);


	// ��� ��� �ʱ�ȭ.�� 
	siKilledMode = 0;
	StringCchCopy(szKillMeCharName, MAX_PLAYER_NAME, TEXT(""));

}

void cltCharServer::Resurrection( bool pvpFlag /* false �� �⺻ */)	// ���ڸ����� ��� ��Ȱ��Ų��.
{
	DeathClock = pclClient->CurrentClock;

	SI32 currentMap = GetMapIndex();
	SI32 currentX   = GetX();
	SI32 currentY	= GetY();

	// �������� ä���. 
	pclCI->clIP.SetLife( clPB.GetMaxLife() );

	// ���� ���·� �����.
	SetCurrentOrder( ORDER_REVIVE );

	OrderStop(siID, BY_COMPUTER);

	cltWarpInfo clWarpInfo(1, currentMap, pvpFlag, false, currentX, currentY, -1, -1);
	SetWarpInfo(&clWarpInfo, 200);

	// ������ ���� ��Ų��. 
	Warp();

	SendDB_PosLifeMana();	

	// Ŭ���̾�Ʈ�� ������Ʈ �Ѵ�. 
	SetUpdateSwitch(UPDATE_IP, true, 0);

	// ĳ������ ���λ����� �ʱ�ȭ�Ѵ�.
	SetPersonalShopBanner(NULL);
	SetPersonalShopData(false, NULL);

	// ��� ��� �ʱ�ȭ.�� 
	siKilledMode = 0;
	StringCchCopy(szKillMeCharName, MAX_PLAYER_NAME, TEXT(""));
}

// �ð� ����� ���� �ڵ� �α׾ƿ�. 
bool cltCharServer::AutoLogOut()
{
	// PC�� ��쿡�� �ڵ� �α׾ƿ��� �۵��Ѵ�. 
	if(IsPC(GetCharUnique()) == false)return false;

	if(LastReceveFromClientClock == 0)return false;

	// ������ �޽����� ������ 3�� �̻��� ����ߴ�. 
	if( TABS(pclClient->CurrentClock - LastReceveFromClientClock ) > 3 * 60 * 1000 )
	{
		if(GetSession())
		{
			// apex-
#ifdef _CHINA
			SI32 id = pclCM->GetIDFromCharUnique( GetCharUnique() );
			if( 0 < id )
			{
				NoticeApexProxy_UserLogout( id, (char*)pclCI->clBI.szAccountID );
				bApexLogin = false;
				pclClient->pclLog->FileConfigPrint ("AutoLogOut", "%s", (char*)GetName() );
			}
#endif

#ifdef USE_GAME_GUARD_SERVER
			CCSAuthClose();
#endif
			// -apex
			GetSession()->CloseSocket();
		}
		return true;
	}

	return false;
}


// ĳ���� ���� �Ŀ� ��ȣȭŰ�� ĳ���� ����ũ ���� Ŭ���̾�Ʈ�� ������. 
// Version the bundled "New Era" client was built with. The client quits as soon
// as the server advertises a higher build in GAMEMSG_RESPONSE_CLIENTCHARUNIQUE.
#define CLIENT_COMPAT_VERSION		1

void cltCharServer::SecretNCharUniqueSendOperation()
{
	TraceBoot("SECRETOPS enter charID=%d charUnique=%d flag=%d session=%p",
		m_uiCharID, GetCharUnique(), (int)bSendSecretNCHarUniqueSwitch, (void*)GetSession());

	if(bSendSecretNCHarUniqueSwitch == false)return ;
	bSendSecretNCHarUniqueSwitch	= false;

	if(GetSession() == NULL)
	{
		TraceBoot("SECRETOPS ABORT session is NULL");
		return;
	}

	// ��ȣȭ�� ���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� �� ���Ѵ�. 
	UI08 key1 = (UI08)rand();
	UI08 key2 = (UI08)rand();

	// ��ȣȭŰ�� �����Ѵ�. 
	GetSession()->GetEncrypt()->SetKey( key1, key2 );

	// ��ȣȭŰ���� CharUnique���� Ŭ���̾�Ʈ���� ������ �ؾ� �Ѵ�. 
	// The bundled "New Era" client is compiled with version 1, and in
	// DoMsg_GAMEMSG_RESPONSE_CLIENTCHARUNIQUE (Client-SystemNPC.cpp) it does:
	//     if( pclInfo->siVersion > siVersion ) { MsgBox(...); dwQuitTimer = 1; }
	// i.e. it quits as soon as the server advertises a HIGHER build. We used to
	// echo pclClient->GetVersion() here, which is the server's own version (573
	// for ConstServiceArea_English), so 573 > 1 tripped the gate and the client
	// closed itself immediately after the secret-key exchange. Advertise the
	// client build version instead; the key exchange itself is unaffected.
	cltGameMsgResponse_ClientCharUnique clInfo(GetCharUnique(), key1, key2, CLIENT_COMPAT_VERSION);
	cltMsg clMsg(GAMEMSG_RESPONSE_CLIENTCHARUNIQUE, sizeof(clInfo), (BYTE*)&clInfo);

	TraceBoot("SECRETOPS SEND charUnique=%d key1=%d key2=%d msgSize=%d",
		GetCharUnique(), (int)key1, (int)key2, (int)sizeof(clInfo));

	// Ŭ���̾�Ʈ�� �޽����� ������. 
	SendNetMsg((sPacketHeader*)&clMsg);

}

//----------------------------------------------------------
// Ŭ���̾�Ʈ���� ���� �Լ�. 
//----------------------------------------------------------
//������ ������. 
void cltCharServer::SendLetterMsg(cltLetterUnit* pclletter)
{
	cltMsg clMsg(GAMEMSG_RESPONSE_LETTER, sizeof(cltLetterUnit), (BYTE*)pclletter);
	SendNetMsg((sPacketHeader*)&clMsg);

}

void cltCharServer::SendAccountLetterMsg(cltAccountLetterUnit* pclletter)
{
	cltMsg clMsg(GAMEMSG_RESPONSE_ACCOUNTLETTER, sizeof(cltAccountLetterUnit), (BYTE*)pclletter);
	SendNetMsg((sPacketHeader*)&clMsg);
}

//-----------------------------------
// �α��� �� ��ȣ 
//-----------------------------------
void cltCharServer::SetInitProtectSwitch()
{
	clSimpleVary.bInitProtectSwitch	= true;
	clSimpleVary.dwInitProtectClock	= pclClient->CurrentClock;
}

bool cltCharServer::GetInitProtectSwitch()
{
	return clSimpleVary.bInitProtectSwitch;
}

// �ڵ� ȸ��/����.  ���. 
void cltCharServer::AutoRecoverLife()
{
	SI32 vary = 0;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if(pclCI->clHealthInfo.uiHealthPoint == 100)return ;

	if(pclCI->clHealthInfo.uiHealthPoint > 100)
	{
		vary = pclCI->clHealthInfo.uiHealthPoint - 100;

		// ü�� ȸ��. 
		SI32 life  = min(vary, max(0, clPB.GetMaxLife() - GetLife()) );
		if(life)
		{
			IncreaseLife(life);

			// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
			if(IsPC(GetCharUnique()))
			{
				cltGameMsgResponse_IncLife clInfo(GetCharUnique(), pclCI->clIP.GetLife(),  life, clPB.siReservedLife, 0, INCLIFE_REASON_HEALTH);
				cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
				SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}

}

void cltCharServer::AutoCushionRecoverLife()
{
	SI32 vary = 0;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13628): // �̺�1
		case  ITEMUNIQUE(13629): // �̺�2 
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary += 40;
			break;
		default:
			vary += 20;
		}
	}

	// ȸ���� ü���� ������ ������.
	if( vary <= 0) return;

	// ü�� ȸ��. 
	SI32 life  = min(vary, max(0, clPB.GetMaxLife() - GetLife()) );
	if(life)
	{
		IncreaseLife(life);

		// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
		if(IsPC(GetCharUnique()))
		{
			cltGameMsgResponse_IncLife clInfo(GetCharUnique(), pclCI->clIP.GetLife(),  life, clPB.siReservedLife, 0, INCLIFE_REASON_HEALTH);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}

// �ڵ� ȸ��/����.  ���. 
void cltCharServer::AutoRecoverMana()
{
	SI32 vary = 0;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if(pclCI->clHealthInfo.uiHealthPoint == 100)return ;

	if(pclCI->clHealthInfo.uiHealthPoint > 100)
	{
		vary = pclCI->clHealthInfo.uiHealthPoint - 100;
		// ���� ȸ���ð� �÷��ִ� ���������� �ɷ�������� ���� ȸ������ ������ 10���� ����
		if ( (10 > vary) && (0 < m_clMagicBufMgr.GetIncMPRecoveryTime((cltCharCommon*)this)) )
		{
			vary = 10;
		}

		// ���� ȸ��. 
		SI32 mana = min(vary, clPB.GetMaxMana() - GetMana());

		if(mana)
		{
			IncreaseMana(mana);

			// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
			if(IsPC(GetCharUnique()))
			{
				cltGameMsgResponse_IncMana clInfo(GetCharUnique(), pclCI->clIP.GetMana(),  mana, INCMANA_REASON_HEALTH);
				cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
				SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}

}


void cltCharServer::AutoCushionRecoverMana()
{
	SI32 vary = 0;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13628): // �̺�1
		case  ITEMUNIQUE(13629): // �̺�2 
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary += 40;
			break;
		default:
			vary += 20;
		}
	}


	// ȸ���� ������ ������ ������.
	if( vary <= 0) return;


	// ���� ȸ��. 
	SI32 mana = min(vary, clPB.GetMaxMana() - GetMana());
	if(mana)
	{
		IncreaseMana(mana);

		// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
		if(IsPC(GetCharUnique()))
		{
			cltGameMsgResponse_IncMana clInfo(GetCharUnique(), pclCI->clIP.GetMana(),  mana, INCMANA_REASON_HEALTH);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}

// PCK : ���ڴ� ������ �漮 - ��ȯ�� ü�� ȸ�� ��� (09.06.15)
void cltCharServer::AutoCushionRecoverSummonLife()
{
	SI32 vary = 0;
	SI32 siIndex = 0;
	SI32 life = 0;

	SI32 siSummonCharID = GetSummonID();

	cltCharServer* pclSummonChar = pclCM->GetCharServer(siSummonCharID);

	if( NULL == pclSummonChar)	return;

	SI32 siSummonIndex = pclCM->CR[siSummonCharID]->GetSummonIndex();
	cltSummon *pclSummon = pclCI->clSummonInfo.GetSummon(siSummonIndex);

	SI32 siSummonKind = 0;
	SI32 siSummonStatus = 0;

	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary += 10;
			break;
		default:
			vary += 0;
		}
	}

	// ȸ���� ü���� ������ ������.
	if( vary <= 0) return;

	// ü�� ȸ��.
	for(siIndex = 0; siIndex < MAX_SUMMON_OWN; ++siIndex)
	{
		siSummonKind	= pclSummonChar->pclCI->clBI.GetKind();
		siSummonStatus	= pclSummon->siStatus;

		SI32	siSummonMaxLife = pclSummonChar->clPB.GetMaxLife();
		SI32	siSummonNowLife = pclSummonChar->pclCI->clIP.GetLife();

		// ��ȯ���� Ȱ��ȭ �Ǿ� ������
		if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_TAKEOUT == siSummonStatus) )
		{
			// ���� ���¶�� ȸ������ �ʴ´�.
			if(siSummonNowLife <= 0)	return ;
			life  = min(vary, max(0, siSummonMaxLife - siSummonNowLife));
		}	
	}

	if(life)
	{
		pclSummonChar->IncreaseLife(life);

		// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
		if(IsPC(GetCharUnique()))
		{
			cltGameMsgResponse_IncLife clInfo(pclSummonChar->GetCharUnique(), pclSummonChar->pclCI->clIP.GetLife(),  life, pclSummonChar->clPB.siReservedLife, 0, INCLIFE_REASON_HEALTH);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

// PCK : ���ڴ� ������ �漮 - ��ȯ�� ���� ȸ�� ��� (09.06.15)
void cltCharServer::AutoCushionRecoverSummonMana()
{
	SI32 vary = 0;
	SI32 siIndex = 0;
	SI32 mana = 0;

	SI32 siSummonCharID = GetSummonID();

	cltCharServer* pclSummonChar = pclCM->GetCharServer(siSummonCharID);

	if( NULL == pclSummonChar)	return;

	SI32 siSummonIndex = pclCM->CR[siSummonCharID]->GetSummonIndex();
	cltSummon *pclSummon = pclCI->clSummonInfo.GetSummon(siSummonIndex);

	SI32 siSummonKind = 0;
	SI32 siSummonStatus = 0;


	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary += 10;
			break;
		default:
			vary += 0;
		}
	}

	// ȸ���� ������ ������ ������.
	if( vary <= 0) return;

	// ���� ȸ��.
	for(siIndex = 0; siIndex < MAX_SUMMON_OWN; ++siIndex)
	{
		siSummonKind	= pclSummonChar->pclCI->clBI.GetKind();
		siSummonStatus	= pclSummon->siStatus;

		SI32	siSummonMaxMana = pclSummonChar->clPB.GetMaxMana();
		SI32	siSummonNowMana = pclSummonChar->pclCI->clIP.GetMana();

		// ��ȯ���� Ȱ��ȭ �Ǿ� ������
		if ( (0 < siSummonKind) && (MAX_KIND_NUMBER > siSummonKind) && (SUMMON_STATUS_TAKEOUT == siSummonStatus) )
		{
			// ���� : MP�� 0�̿��� ��ȯ�� ������ ȸ���Ǿ� �Ѵ�. (09.07.24)
			if(siSummonNowMana < 0)	return ;
			mana  = min(vary, max(0, siSummonMaxMana - siSummonNowMana));
		}	
	}

	if(mana)
	{
		pclSummonChar->IncreaseMana(mana);

		// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
		if(IsPC(GetCharUnique()))
		{
			cltGameMsgResponse_IncMana clInfo(pclSummonChar->GetCharUnique(), pclSummonChar->pclCI->clIP.GetMana(),  mana, INCMANA_REASON_HEALTH);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

// ��� ȸ��.
void cltCharServer::AutoCushionRecoverOragans()
{
	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();
	SI32 vary = 0;
	bool select = false;

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13628): // �̺�1
		case  ITEMUNIQUE(13629): // �̺�2
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary = rand()%5+1; // ȸ����
			break;
		default:
			vary = 0;
		}
	}

	// ���������� ����ļ�ġ�� ���ҽ�ų�� ������ �����Ѵ�.
	if(vary >0)
	{
		SI32 maxOrg = MAXORG(pclCI->clIP.GetLevel());
		SI16 trueOrg[5] = {0,}; // 0��ĭ�� ���� �ʴ´�.
		SI16 temSelectOrg = 0;
		SI16 temprandSelect = 0;
		SI32 randSelect = 0;

		for(SI16 i = 1;i < 5 ;i++)
		{
			SI32 tempOrg = pclCI->clHealthInfo.GetOrg(i);

			if(tempOrg < maxOrg)
			{
				temSelectOrg++;
				trueOrg[temSelectOrg] = i; //1~4��ĭ�� �����Ѵ�.
			}

		}

		// ȸ����ų ��� ����.
		if(temSelectOrg >0)
		{
			temprandSelect = rand()%temSelectOrg+1; // �������߿��� �����ϰ� ������.
			randSelect	= trueOrg[temprandSelect];  // ���� ��ġ�� ����� ���� ã�´�.

			SI32 nowOrgval = pclCI->clHealthInfo.GetOrg(randSelect); // ���� �����ġ�� ���Ѵ�.

			// �����ġ�� �ִ밪�� ���� �� ����.
			if((nowOrgval+vary) > maxOrg)
				vary = maxOrg - nowOrgval;

			switch(randSelect)
			{
			case 1: // ��ȭ���.
				pclCI->clHealthInfo.IncreaseInnerOrg1(vary);
				break;
			case 2: // ��ȯ���.
				pclCI->clHealthInfo.IncreaseInnerOrg2(vary);
				break;
			case 3: // ȣ�����.
				pclCI->clHealthInfo.IncreaseInnerOrg3(vary);
				break;
			case 4: // �Ű���.
				pclCI->clHealthInfo.IncreaseInnerOrg4(vary);
				break;
			}

			select= true;
		}
	}

	if( select == true)
	{	
		//Ŭ��� �˸���.
		SetUpdateSwitch(UPDATE_HEALTH, true, 0);

		// DB�� �����Ѵ�. 
		sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
}

// �������� �϶���Ų��.
void cltCharServer::AutoCushionFallSatisfied()
{
	SI32 vary = 0;
	bool select = false;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if( GetCurrentOrder() != ORDER_SITDOWN ) return;

	SI16 haveCushion =  IsHaveCushionItem();

	if( haveCushion > 0 )
	{
		switch(haveCushion)
		{
		case  ITEMUNIQUE(13628): // �̺�1
		case  ITEMUNIQUE(13629): // �̺�2
		case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
		case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
		case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
			vary = 1; // ���ҷ�
			break;
		default:
			vary = 0;
		}
	}

	// ���������� ����ļ�ġ�� ���ҽ�ų�� ������ �����Ѵ�.
	if(vary >0)
	{
		SI32 level  = pclCI->clIP.GetLevel();	
		SI16 nowHungry = clPB.clHealth.GetHungry();
		SI16 basicHungry = 30;

		if(level > 30 && level <= 100 )
		{
			basicHungry = level;
		}
		// PCK : ĳ���� ������ 100���� �̻��� �Ѿ�� ����� ��ġ�� 100���� �����Ѵ�. (09.07.08)
		else if(level > 100)
		{
			basicHungry = 100;
		}

		if(nowHungry > basicHungry)
			select= true;
	}

	if(select == true)
	{
		// �������� ���ҽ�Ų��.
		clPB.clHealth.DecreaseHungry(vary);

		//Ŭ��� �˸���.
		SetUpdateSwitch(UPDATE_HEALTH, true, 0);

		// DB�� �����Ѵ�. 
		sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
}

//KHY - 1220 - ���Žý���.
void cltCharServer::AutoTransFormRecoverMana()
{
	SI32 vary = 0;

	// ���� ���¿����� �ڵ� ȸ�� ���� �ʴ´�. 
	if(GetLife() <= 0)return ;

	if( IsCanTransFormRecover() == true  )
	{
		vary += 10;
	}

	// ȸ���� ������ ������ ������.
	if( vary <= 0) return;


	// ���� ȸ��. 
	SI32 mana = min(vary, clPB.GetMaxMana() - GetMana());
	if(mana)
	{
		IncreaseMana(mana);

		// ������� ���, Ŭ���̾�Ʈ�� �����ش�. 
		if(IsPC(GetCharUnique()))
		{
			cltGameMsgResponse_IncMana clInfo(GetCharUnique(), pclCI->clIP.GetMana(),  mana, INCMANA_REASON_HEALTH);
			cltMsg clMsg(GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}

// ���� ���� ���� �۾�. 
void cltCharServer::LevelUpAction()
{
	cltServer* pclserver = (cltServer*)pclClient;
	TCHAR Nbuffer[512]=TEXT("");

	SI32 PrevLevel = pclCI->clIP.GetLevel();

	if(IsPC(GetCharUnique()))
	{
		// �޸� ���� �������� ���� ��� ������ ���׷��̵� ==============================================================================
		CDormancySystem *pclDormancySystem = pclserver->pclDormancySystem;
		if( NULL != pclDormancySystem)
		{
			CDormancyEquipItemRewardServer *pclDormancyServer = (CDormancyEquipItemRewardServer *)pclDormancySystem->GetDormancyEquipItemReward();
			if(NULL != pclDormancyServer)
			{
				pclDormancyServer->UpgradeItemInven(this);
			}
		}
		//==============================================================================================================================
	}

	// ������ �÷��ش�. 
	pclCI->clIP.IncreaseLevel(1);

	// ���ʽ��� �����Ѵ�.
	pclCI->clIP.IncreaseBonus(5);

	// ��ų ���ʽ��� �����Ѵ�. 
	pclCI->clIP.IncreaseSkillBonus(1);

	// ü���� ����. 
	SetLife(clPB.GetMaxLife());

	// �������� ����. 
	SetMana(clPB.GetMaxMana());

	// DB�� ������ ������. 
	SendDBLevelInfo(5, 1);	

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// �α� �ۼ� - ������ �α�
		// param1 == ���� ��������, param2 == ���� ����.
		// PCK - ĳ���͸� �α׸� ����ϰ� �Ѵ�.
		if(IsPC(GetCharUnique()))
		{
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARLEVELUP, 
				0, this, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				PrevLevel, pclCI->clIP.GetLevel(), 0, 0, 0, NULL, NULL);
		}
		// �α� �ۼ� - ��ȯ�� ������ �α�
		// param1 == ���� ��������, param2 == ���� ����.
		// param3 == ��ȯ���ε���
		// pszCharParam1 == ��ȯ���̸�
		if(IsNPC(GetCharUnique()))
		{
			// ��ȯ���� �÷��̾� ��ü
			cltCharServer* pclPlayerChar = (cltCharServer*) pclCM->GetCharServer(GetSummonParentID());
			if( NULL !=  pclPlayerChar)
			{
				cltSummon *pclSummon = pclPlayerChar->pclCI->clSummonInfo.GetSummon(GetSummonIndex());
				
				TCHAR szSummonName[32] = {0,};
				if (pclSummon != NULL) 
				{
					StringCchCopy(szSummonName, 32, pclSummon->szName);
				}

				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON, LOGCOMMAND_INDEX_SUMMON_LEVELUP, 
				0, pclPlayerChar, pclSummon, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				PrevLevel, pclCI->clIP.GetLevel(), GetSummonIndex(), 0, 0, szSummonName, NULL);
			}
		}
	}

	// ��尡 �ִٸ� GP�� ������Ʈ�Ѵ�.
	if ( pclCI->clBI.clGuildRank.IsGuildMember() == true )
	{
		cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
		if ( pGuild )
		{
			sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCI->GetPersonID(),10,TYPE_GUILDPOINT_ADD_LEVELUP);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			//KHY - 1205 - ���������� ����
			//if (pclClient->IsWhereServiceArea(ConstSwitch_LevelUpNotice) )
			if (pclClient->IsCountrySwitch(Switch_LevelUpNotice) )
			{
				SI32 siPersonID = pclCI->GetPersonID();
				if(IsPC(GetCharUnique()))
				{
					TCHAR szName[MAX_PLAYER_NAME];
					MStrCpy( szName,GetName(), MAX_PLAYER_NAME );

					cltGameMsgResponse_LevelUpNotice clinfo(szName, pclCI->clIP.GetLevel(), 0 , 0 , 1);
					cltMsg clMsg2(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
					//KHY - 0131 - ��� ��� �޽��� ���� - �̸����� �˻� - ����ũ��ȣ �˻�����!
					pclClient->pclCM->SendAllGuildMemberMsg((sPacketHeader*)&clMsg2,pGuild->siUnique,pGuild->szName);
				}
			}
		}
	}

	// ������������ �������� ���� ��ǰ�ֱ�
	//KHY - 0610 - �������� S �߰�����.
	if ( ConstServiceArea_Japan != pclClient->siServiceArea )
	{
		SendDBUpdateTeacherPoint( pclCI->clIP.GetLevel() );
	}

	if( pclClient->IsCountrySwitch( Switch_Elixir ) )
	{
		// �������� �ϸ� �׿� �ش��ϴ� ������ ����Ʈ�� ���� �����ش�.
		if( pcltElixir && pcltElixir->IsElixir() ) 
		{
			cltElixir_Info  clElixirInfo;
			clElixirInfo.m_siEndMinuteVary	= pcltElixir->GetMinuteVary();
			clElixirInfo.m_uiMultiple 		= pcltElixir->GetMultiple();
			clElixirInfo.m_siPoint	  		= pcltElixir->GetPoint() + pcltElixir->GetMultiple();
			clElixirInfo.m_siStr	  		= pcltElixir->GetStr();
			clElixirInfo.m_siDex	  		= pcltElixir->GetDex();
			clElixirInfo.m_siVit	  		= pcltElixir->GetVit();
			clElixirInfo.m_siMag	  		= pcltElixir->GetMag();
			clElixirInfo.m_siHnd	  		= pcltElixir->GetHnd();
			clElixirInfo.m_siWis	  		= pcltElixir->GetWis();

			if(pclCI->GetPersonID() > 0)
			{
				sDBRequest_Elixir_Set clMsg( GetID(), pclCI->GetPersonID(), &clElixirInfo );
				((cltServer*)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );	
			}
		}
	}
	
	//[����] ����, ����. �������� 20 �̻��̰� ���� ���� �϶� �������� ������ ��������ش�.
	SI32 siFatherPersonID = pclCI->clBI.clFatherInfo.clFather.GetPersonID();
	if( 0 < siFatherPersonID )
	{
		if( 20 <= pclCI->clIP.GetLevel() && 49 > pclCI->clIP.GetLevel())	// ���� ���� Ȯ��.
		{
			// 1���� ������ ������ �ش�.
			//if( 0 == pclCI->clIP.GetLevel() % 2 )
			{
				SI32 siFatherID = pclClient->pclCM->GetIDFromPersonID(siFatherPersonID);			
				cltCharServer* pclFatherChar = pclCM->GetCharServer(siFatherID);		// ������ ���ӿ��� Ȯ��.
				if( pclFatherChar ) 
				{
					// ������ ���� ������ ������ ���ٸ� ������ �ָ���~����
					if(		(pclCI->clBI.siHomeVillage == pclFatherChar->pclCI->clBI.siHomeVillage) 
						&&	(0 < pclCI->clBI.siHomeVillage && 0 < pclFatherChar->pclCI->clBI.siHomeVillage) )
					{
						SI32 usetime = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary() + 24 ;
						SI16 index   = 0;

						// ������ ����.
						if( false == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MENTOR ) )  
						{
							index = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_MENTOR, false );

							stBuf addBuf(BUF_MENTOR, 10, usetime);
							AddBufThroughDB( index, &addBuf , false , 0, NULL );
						}

						// ������ ����.
						if( false == pclFatherChar->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_MENTOR ) )  
						{
							index = pclFatherChar->pclCI->clBI.clBufInfo.FindEmptySlot( BUF_MENTOR, false );

							stBuf addBuf(BUF_MENTOR, 10, usetime);
							pclFatherChar->AddBufThroughDB( index, &addBuf , false , 0, NULL );

						}
					}
				}
			}
		}
	}

	// Ŭ���̾�Ʈ�� ������ �ö����� �뺸�Ѵ�.  (JAYCEE : �̿����Ե� �뺸�� �Ѵ�.)
	cltGameMsgResponse_LevelUp clinfo(GetCharUnique(), pclCI->clIP.GetLevel(), 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUP, sizeof(clinfo), (BYTE*)&clinfo);
	SendNeighbourMsg(&clMsg, true);

	// ������ �ִ°�� ������ ������ DB�� ������ ���� ����Ѵ�
	if(pclCI->clBI.siHomeVillage > 0 && pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT)
	{
		// PCK : ������ ī��Ʈ Ƚ�� �߰�.
		sDBRequest_VillageJoinLevelUp clMsg(pclCI->GetPersonID(), pclCI->clBI.clVillageScoreInfo.siLevelUpCount, pclCI->clBI.siHomeVillage); 
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

		//KHY - 1205 - ���������� ����
		if (pclClient->IsCountrySwitch(Switch_LevelUpNotice) )
		{
			SI32 siPersonID = pclCI->GetPersonID();
			if(IsPC(GetCharUnique()))
			{
				TCHAR szName[MAX_PLAYER_NAME];
				MStrCpy( szName,GetName(), MAX_PLAYER_NAME );

				cltGameMsgResponse_LevelUpNotice clinfo(szName, pclCI->clIP.GetLevel(), 0 , 0 , 2);
				cltMsg clMsg2(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg2,pclCI->clBI.siHomeVillage);
			}
		}		
	}

	// Ŭ���̾�Ʈ�� ������ �뺸���ش�.
	// ����� ĳ�����ΰ�쿡�� �뺸�Ѵ�. ��ȯ���� �뺸�Ѵ� 
	if( IsPC(GetCharUnique()) || clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
		SetUpdateSwitch(UPDATE_IP, true, 0);
	}

	// ��ȯ�� ������ ����Ʈ
	SI32 siParentID = GetSummonParentID();
	if(pclCM->IsAlive(siParentID))
	{
		pclCM->CR[siParentID]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SUMMONLEVELUP);
	}

	if ( IsPC(GetCharUnique()) )
	{
		// �������� ���� ������ �ش�. - ������
		if ( pclClient->pclEventTimeManager->InEventTime(TEXT("LevelUpReward"), &pclserver->sTime) )
		{
			SI32 siItemUnique[MAX_LEVELUP_REWARD_ITEM];
			SI32 siItemNum[MAX_LEVELUP_REWARD_ITEM];
			SI16 siNotice[MAX_LEVELUP_REWARD_ITEM];

			if(pclCI->clIP.GetLevelUpRewardItem(pclCI->clIP.GetLevel(), siItemUnique, siItemNum, siNotice))
			{
				for(int i=0; i<MAX_LEVELUP_REWARD_ITEM; i++)
				{
					if(siItemUnique[i] <= 0)			continue;
					if(siItemNum[i] <= 0)				continue;

					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siItemUnique[i]);
					if(ref > 0)
					{
						UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;				
						if ( GIVEITEM_RESULT_NOINVEN == GiveItem(siItemUnique[i], siItemNum[i], INPUTITEMPERSON_OPTION_LEVELUP_REWARD, usedate) )
						{
							// ������ ��� �������� �������� ����.
							cltItem clInputItem;
							SI16 bRareSwitch = 0;
							pclClient->pclItemManager->MakeRandItemUnique( siItemUnique[i], &clInputItem, 0, 0, &bRareSwitch, 0, 0, 0, 0, 0, usedate );

							clInputItem.siItemNum = siItemNum[i];

							pclserver->SendPostOfficeItemToPerson( pclCI->GetPersonID(), &clInputItem , NULL ,0  );
						}
					}

					pclserver->SendServerResponseMsg( 0, SRVAL_NOTICE_LEVELUP_REWARD, pclCI->clIP.GetLevel(), siItemUnique[i], GetCharUnique() );

					// ��� �˸�â ����
					if ( 0 < siNotice[i] )
					{
						cltItem	clNoticeItem;
						SI16	bRareSwitch = 0;

						pclClient->pclItemManager->MakeRandItemUnique(siItemUnique[i],&clNoticeItem,0,0,&bRareSwitch);

						cltGameMsgResponse_RareItemMade	clinfo((TCHAR*)(GetName()), pclCI->GetPersonID(), 
							&clNoticeItem,
							RARAITEM_NOTICE_SWITCH_LEVELUPREWARD,
							pclCI->clIP.GetLevel() );

						cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMMADE, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}



		// �Ϻ� �̺�Ʈ ����� ���� �ڵ� ���� - by LEEKH 2007.10.04
		/*if( pclClient->IsWhereServiceArea( ConstSwitch_LevelUpAction ) )
		{
			//KHY - 0807 - �̺�Ʈ �߻� �ð� ����.
			setLevUpActionStartTime(2007,8,8,16);
			setLevUpActionEndTime(2007,8,16,11);

			_SYSTEMTIME sysNowtime;
			GetLocalTime( &sysNowtime );

			UI32 uNowtime = pclserver->GetMinuteFromsTime(sysNowtime);
			UI32 uStarttime = pclserver->GetMinuteFromsTime(sTimeLevelUpActionStartTime);
			UI32 uEndtime = pclserver->GetMinuteFromsTime(sTimeLevelUpActionEndTime);


			if((uNowtime >= uStarttime)&&(uNowtime<=uEndtime))
			{
				// �������� ���� ������ �ش�. - ������
				SI32 siItemUnique[MAX_LEVELUP_REWARD_ITEM];
				SI32 siItemNum[MAX_LEVELUP_REWARD_ITEM];

				if(pclCI->clIP.GetLevelUpRewardItem(pclCI->clIP.GetLevel(), siItemUnique, siItemNum))
				{
					for(int i=0; i<MAX_LEVELUP_REWARD_ITEM; i++)
					{
						if(siItemUnique[i] <= 0)			continue;
						if(siItemNum[i] <= 0)				continue;

						SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siItemUnique[i]);
						if(ref > 0)
						{
							UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;				
							if(GiveItem(siItemUnique[i], siItemNum[i], INPUTITEMPERSON_OPTION_LEVELUP_REWARD , usedate) == false)
							{
								// ������ ��� �������� �������� ����.
							}
						}
					}
				}
			}
		}*/

			// ������, ��Ŀ �̺�Ʈ - �Ⱓ����
			if (pclClient->IsCountrySwitch(Switch_GoStopPokerEvent) )
			{		
				_SYSTEMTIME sStartEventDay , sEndEventDay;

				ZeroMemory(&sStartEventDay,sizeof(_SYSTEMTIME));
				ZeroMemory(&sEndEventDay,sizeof(_SYSTEMTIME));

				sStartEventDay.wYear = 2006;
				sStartEventDay.wMonth = 9;
				sStartEventDay.wDay = 20;
				sStartEventDay.wHour = 0;
				sStartEventDay.wMinute = 0;

				sEndEventDay.wYear = 2006;
				sEndEventDay.wMonth = 10;
				sEndEventDay.wDay = 19;
				sEndEventDay.wHour = 0;
				sEndEventDay.wMinute = 0;

				SI32 startEventdatevary = pclClient->GetMinuteFromsTime(sStartEventDay);
				SI32 endEventdatevary = pclClient->GetMinuteFromsTime(sEndEventDay);

				_SYSTEMTIME fTimeDatevary = pclCI->clBI.GetFirstCreatTime(); //���� ���� ���� �ð�.

				SI32 firstCreatTimedatevary = pclClient->GetMinuteFromsTime(fTimeDatevary);

				if((startEventdatevary <= firstCreatTimedatevary)&&(endEventdatevary >= firstCreatTimedatevary) && pclCI->clIP.GetLevel() == 5)
				{
					pclserver->SendLog_SetEventCouponInfo(this, GOSTOP_POKER_EVENT,0);
				}
			}

			//KHY - 1024 - �Ѱ��� VIP �̺�Ʈ- �Ⱓ����
			//by LEEKH 2007.10.30 - �Ⱓ ����
			if (pclClient->IsCountrySwitch(Switch_HanGameVIPEvent) )
			{		
				_SYSTEMTIME sStartEventDay , sEndEventDay;

				ZeroMemory(&sStartEventDay,sizeof(_SYSTEMTIME));
				ZeroMemory(&sEndEventDay,sizeof(_SYSTEMTIME));

				sStartEventDay.wYear = 2006;
				sStartEventDay.wMonth = 11;
				sStartEventDay.wDay = 8;
				sStartEventDay.wHour = 0;
				sStartEventDay.wMinute = 0;

				sEndEventDay.wYear = 2006;
				sEndEventDay.wMonth = 11;
				sEndEventDay.wDay = 30;
				sEndEventDay.wHour = 0;
				sEndEventDay.wMinute = 0;

				SI32 startEventdatevary = pclClient->GetMinuteFromsTime(sStartEventDay);
				SI32 endEventdatevary = pclClient->GetMinuteFromsTime(sEndEventDay);

				_SYSTEMTIME fTimeDatevary = pclCI->clBI.GetFirstCreatTime(); //���� ���� ���� �ð�.

				SI32 firstCreatTimedatevary = pclClient->GetMinuteFromsTime(fTimeDatevary);

				if((startEventdatevary <= firstCreatTimedatevary)&&(endEventdatevary >= firstCreatTimedatevary) && pclCI->clIP.GetLevel() == 5)
				{
					pclserver->SendLog_SetEventCouponInfo(this, HAN_GAME_VIP_EVENT,2); // 2 = 5���� �޼� �̺�Ʈ Ŭ����.
				}
			}


			//KHY - 1025 - ������ �̺�Ʈ - �Ⱓ����. -10���� �޼�. - 11������.
			if (pclClient->IsCountrySwitch(Switch_NowComEvent) )
			{
				_SYSTEMTIME sStartEventDay, sEndEventDay;

				ZeroMemory(&sStartEventDay,	sizeof(_SYSTEMTIME));
				ZeroMemory(&sEndEventDay,	sizeof(_SYSTEMTIME));

				sStartEventDay.wYear   = 2006;
				sStartEventDay.wMonth  =   11;
				sStartEventDay.wDay    =    8;
				sStartEventDay.wHour   =    0;
				sStartEventDay.wMinute =    0;

				sEndEventDay.wYear	 = 2006;
				sEndEventDay.wMonth  =   12;
				sEndEventDay.wDay	 =    5;
				sEndEventDay.wHour	 =    0;
				sEndEventDay.wMinute =    0;

				SI32 startEventdatevary = pclClient->GetMinuteFromsTime(sStartEventDay);
				SI32 endEventdatevary   = pclClient->GetMinuteFromsTime(sEndEventDay);

				_SYSTEMTIME fTimeDatevary   = pclCI->clBI.GetFirstCreatTime(); //���� ���� ���� �ð�.
				SI32 firstCreatTimedatevary = pclClient->GetMinuteFromsTime(fTimeDatevary);

				if((startEventdatevary <= firstCreatTimedatevary)&&(endEventdatevary >= firstCreatTimedatevary) && pclCI->clIP.GetLevel() == 11)
				{
					pclserver->SendLog_SetEventCouponInfo(this, NOW_COM_EVENT, 2); // 2 = 10���� �޼� �̺�Ʈ Ŭ����.
				}
			}

#ifdef USE_PROMOTOR_SERVER
			if( true == bIsPromoter )
			{
				// Ŭ���̾�Ʈ�� ���θ��� ����â�� ����� ���� ��ư�� �����ֶ�� �޽��� ����. (������ ���� �� �ִ� ������ �ִٸ�)
				if( pclCI->clIP.GetLevel() < pclserver->pclRewardMgr->GetRecommendGiveItemMaxLevel() )
					pclserver->SendServerResponseMsg(0, SRVAL_PROMOTER_CHAR_LEVELUP_MINI_BUTTON, 0, 0, GetCharUnique());
			}
			
			// �߱��� �ڵ��� ������ �Ǿ߸� �����۰� ����Ʈ�� �ش�.	
			if( /*true == bCellularPhoneAuth &&*/ true == bIsPromoter )
#endif
			{
				// ��õ���� �ִ��� Ȯ���Ѵ�. 
				SI32 recommenderpersonid = pclCI->clBI.clRecommenderInfo.siPersonID;
				SI32 recommendPoint		 = pclClient->GetRecommendPointPerLevel( pclCI->clIP.GetLevel() ); //������ ���� ��õ����Ʈ�� �ִ��� üũ
				if( recommenderpersonid > 0 ) 
				{
					sDBRequest_ChangeRecommendPoint clMsg(  GetID() , pclCI->GetPersonID(), (TCHAR*)GetName(), 
															pclCI->clIP.GetLevel(), recommenderpersonid,  
															recommendPoint, RPGIVE_REASON_LEVELUP	);

					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}

				// ��õ���� ����� �ξ��ٸ� �ڽſ��� ������ ����
				if ( (0 < recommenderpersonid) && (true == pclClient->pclEventTimeManager->InEventTime(TEXT("RecommendReward"), &pclserver->sTime)) )
				{
					if( pclserver->pclRewardMgr )
					{
						pclserver->pclRewardMgr->GiveRecommendRewardItem( pclCI->GetPersonID(), pclCI->clIP.GetLevel(), true, NULL, bCellularPhoneAuth );
					}
				}
				else
				{
					if( recommenderpersonid > 0 && ( pclCI->clIP.GetLevel() == 40 || pclCI->clIP.GetLevel() == 50  || pclCI->clIP.GetLevel() == 60 || pclCI->clIP.GetLevel() == 70 )) 
					{
						SI32 itemunique = 0; //
						SI32 itemnum = 0;

						switch(pclCI->clIP.GetLevel())
						{
						case 40:	itemunique = ITEMUNIQUE(5055); //ITEMUNIQUE(5320);		
									itemnum = 20;	
									break;
						case 50:	itemunique = ITEMUNIQUE(5045); //ITEMUNIQUE(5316);		
									itemnum = 30;	
									break;
						case 60:	itemunique = ITEMUNIQUE(5017); //ITEMUNIQUE(5306);		
									itemnum = 40;	
									break;
						case 70:	itemunique = ITEMUNIQUE(24000);		
									itemnum = 30; 	
									break;
						}

						SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);

						if(itemunique > 0 && itemnum > 0 && ref > 0)
						{
							UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
							if ( GiveItem(itemunique, itemnum, INPUTITEMPERSON_OPTION_RECOMMEND , usedate) != GIVEITEM_RESULT_SUCCESS )
							{
								// �κ��丮�� ���� �������� �������� ������.

								cltItem clInputItem;
								SI16 bRareSwitch = 0;
								pclClient->pclItemManager->MakeRandItemUnique(itemunique,&clInputItem,0,0,&bRareSwitch);

								clInputItem.siItemNum = itemnum;

								((cltServer*)pclClient)->SendPostOfficeItemToPerson( pclCI->GetPersonID(), &clInputItem , NULL ,0  );
							}
							else
							{
								cltLetterMsg_LevelUpREcommandReward clletter( pclCI->clIP.GetLevel(), itemunique , itemnum );
								pclserver->SendLetterMsg(  pclCI->GetPersonID() , (cltLetterHeader*)&clletter );					
							}

							if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
							{
								// [�α��߰� : Ȳ���� 2007. 10. 19] // ����õ�� ����.
								// param1 == ������ ����ũ.
								// param2 == ������ ����.
								pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_COMMENDER_REWARD, 
																0, this, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
																itemunique, itemnum, 0, 0, 0, NULL, NULL);
							}
						}
					}
				}
			}
#ifdef USE_PROMOTOR_SERVER	
			else if( true == bIsPromoter )
			{
				// �ڵ��� ������ �ȵǼ� �ټ� �ִ� �������� �ִµ� ���ٶ� �޽����� ������.
				if( pclserver->pclRewardMgr->IsGiveRecommendRewardItem( pclCI->GetPersonID(), pclCI->clIP.GetLevel(), true, NULL ) )
				{
					// �߱��� �ڵ��� ������ �ȵǸ� �ش� �޽����� Ŭ�� �ش�.
					//pclserver->SendServerResponseMsg(0, SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH, 0, 0, GetCharUnique());
					pclserver->SendServerResponseCharMsg2( 0, SRVAL_RECOMMEND_GIVE_ITEM_FAIL, NULL, NULL, NULL, GetCharUnique() );		

					SI32 recommenderpersonid = pclCI->clBI.clRecommenderInfo.siPersonID;
					if( recommenderpersonid > 0 ) 
					{
						SI32 siID = pclCM->GetIDFromPersonID(recommenderpersonid);
						cltCharServer* pRecommendeeChar = pclCM->GetCharServer( siID );
						if( pRecommendeeChar )
						{
							// �߱��� �ڵ��� ������ �ȵǸ� �ش� �޽����� Ŭ�� �ش�.
							//SendServerResponseMsg(0, SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH, 1, 0, pRecommendeeChar->GetCharUnique());

							pclserver->SendServerResponseCharMsg2( 0, SRVAL_RECOMMENDEE_GIVE_ITEM_FAIL, (TCHAR*)GetName(), NULL, NULL, pRecommendeeChar->GetCharUnique() );		
						}
					}
				}
			}
#endif
			//KHY - 0610 - �������� S �߰�����.
			if ( ConstServiceArea_Japan != pclClient->siServiceArea )
			{
				//-------------------------------------
				// ������ �ִ��� Ȯ���Ѵ�. 
				//-------------------------------------
				if(pclCI->clBI.clFatherInfo.clFather.siPersonID)
				{
					if(pclCI->clIP.GetLevel() == NEWFATHER_LEVEL)
					{
						sDBRequest_PersonSimpleInfo clMsg(GetID(), pclCI->clBI.clFatherInfo.clFather.siPersonID, PERSONSIMPLEINFO_REASON_FATHERREWARD);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}

					if( false == pclCI->clBI.clFatherInfo.bOverTake )
					{
						if( pclCI->clIP.GetLevel() >= NEWFATHER_LEVEL )
						{
							sDBRequest_OverTakeTeacherLevel clMsg(GetID(), pclCI->clBI.clFatherInfo.clFather.siPersonID, pclCI->clIP.GetLevel(), pclCI->GetPersonID() );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}
					}
				}
			}


			if(pclCI->clIP.GetLevel() == FATHER_LEVEL)
			{
				//-------------------------------------
				// ������ �ִ��� Ȯ���Ѵ�. 
				//-------------------------------------

				if(pclCI->clBI.clFatherInfo.clFather.siPersonID)
				{
					if( ConstServiceArea_Japan == pclClient->siServiceArea )
					{
						sDBRequest_PersonSimpleInfo clMsg(GetID(), pclCI->clBI.clFatherInfo.clFather.siPersonID, PERSONSIMPLEINFO_REASON_FATHERREWARD);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}

					// ��尡 �ִ��� ������ �𸣰����� �ϴ� �ְ� ����.
					sDBRequest_Guild_ChangeGuildPoint clMsg2(0,pclCI->clBI.clFatherInfo.clFather.siPersonID,1,TYPE_GUILDPOINT_ADD_CHILD);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg2);
				}


				//�߱��� �ƴ� ���� ����ȸ�� �������� ���� �Ѵ� 
				if(pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))
				{
				}
				else
				{
					//-----------------------------------------------
					// ȣ���Ǽ��� ���� ����ȸ�� ���� �ڱ��� �����Ѵ�.
					//-----------------------------------------------
					// ȣ���Ǽ��� ���� �ڱ��� ���Ѵ�. 
					CMinisterHozo* pclminister = (CMinisterHozo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_HOZO);
					GMONEY money = pclminister->GetFizationMoney();

					// �Ƿι��� �ý��� ����
					money = CalcTiredPoint( money );
					if(money)
					{
						// �ս��ڻ꿡 ������ �ִ��� Ȯ�� ���Ѵ�. 
						CMinisterGoonzu *pclministergoonzu = (CMinisterGoonzu *)pclserver->pclMinisterMgr->GetMinister(MINISTER_GOONZU);
						INT64 budget = pclministergoonzu->GetTotalFinances();
						//if(budget >= money)
						{
							// �ս��ڽſ��� �ڱ��� �Ҹ��Ų��.
							pclministergoonzu->RemoveFinances(money);

							sDBRequest_ChangeMoney clMsg(GetID(), pclCI->GetPersonID(), CHANGE_MONEY_REASON_FIZATION, money );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

							pclserver->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_LEVEL20, money);
						}

						if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
						{
							// [�α��߰� : Ȳ���� 2007. 9. 3] // ������. 
							//pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_???, LOGCOMMAND_INDEX_?????, 
							//									0, this, NULL, 0, money, 0, NULL, 0, 0, 0, 0, 0, 0, 
							//									0, 0, 0, 0, 0, NULL, NULL);*/
						}

					}
				}
			}

			// �ʺ��� ����� ���� ���� �̻��̸�, 
			if(pclCI->clIP.GetLevel() >= MAX_LEVEL_FOR_BEGINNERMAP)
			{
				// �ʺ��� ����Ϳ� �ִٸ�, 
				if(GetMapIndex() == MAPINDEX_BEGINNER1 )
				{
					WarpToSafeZone();
				}
			}
			// ��Ƽ �÷��� ���̶�� ��Ƽ���鿡�� ������ �˷���
			if( siPartyIndex > 0)
			{
				CPartyObj* pParty =	pclClient->pclPartyMgr->GetPartyObj(siPartyIndex);
				if( pParty ) 
					pParty->NotifyLevelUpToPatryMember( GetCharUnique()  ,pclCI->clIP.GetLevel() );
			}	

			// [����] �ʺ��� ����
			if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
			{
				SI32 siGetPersonID	= pclCI->GetPersonID();
				SI32 siGetCharID	= GetID();
				SI32 siGetLevel		= pclCI->clIP.GetLevel();
				SI32 siGetVillage	= pclCI->clBI.siHomeVillage;


				// ���� 30�� �Ǿ����� ĳ���Ͱ� �ʺ��� ������ ���ԵǾ� �־�� ���ֹ����� �ش�
				if ( (Const_BV_StockDividend_Level == siGetLevel) && (Const_Beginner_Village == siGetVillage) )
				{
					bool bCondition = false;

					// �ʺ��� ������ �ֽ��� 10�� �ִ��� ã�´�
					for ( SI32 siCount=0; siCount<MAX_STOCK_KIND_IN_ACOUNT; siCount++)
					{
						SI32 siFindVillageUnique	= pclCI->clStock.clInfo.clUnit[siCount].siVillageUnique;
						SI32 siFindStockAmount		= pclCI->clStock.clInfo.clUnit[siCount].siAmount;

						if ( Const_Beginner_Village == siFindVillageUnique )
						{
							if  ( 10 <= siFindStockAmount ) 
							{
								bCondition = true;
							}
							break;
						}
					}

					if ( true == bCondition )
					{
						// ����ݰ�(�ս��ڻ�)
						CMinisterGoonzu	*pclMinisterGoonzu	= (CMinisterGoonzu*)pclserver->pclMinisterMgr->GetMinister(MINISTER_GOONZU);
						// ���ۿ��� �ʺ��� ���� ���ֹ��� ��������
						CMinisterHozo	*pclMinisterHozo	= (CMinisterHozo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_HOZO);

						if ( (pclMinisterGoonzu != NULL) && (pclMinisterHozo != NULL) )
						{
							GMONEY gmBVMoney = (SI32)pclMinisterHozo->GetBeginnerVillageMoney();

							// ����ݰ����� �ڱ��� �Ҹ��Ų��
							pclMinisterGoonzu->RemoveFinances( gmBVMoney );

							sDBRequest_ChangeMoney clMsg(siGetCharID, siGetPersonID, CHANGE_MONEY_REASON_BV_STOCKDIVIDEND, gmBVMoney );
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

							pclserver->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_ANKALANSTOCK30, gmBVMoney);
						}
					}	// if ( true == bCondition ) ����
				}	// if ( (Const_BV_StockDividend_Level == siGetLevel) && (Const_Beginner_Village == siGetVillage) ) ����

				// ���� 40�̵Ǹ� �ʺ��� �������� Ż���Ų��
				if ( (Const_BV_Max_Resident_Level == siGetLevel) && (Const_Beginner_Village == siGetVillage) )
				{
					sDBRequest_CancelResident clMsg( siGetCharID, siGetPersonID, Const_Beginner_Village, true );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			} // if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) ) ����

			if ( pclClient->IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();

				if ( pclSE_Quest )
				{
					SI32 siMyLevel	= pclCI->clIP.GetLevel();
					SI32 siAmount	= max( 1, siQuestCount );

					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_LEVELUP, siMyLevel, siAmount );
				}

			}	// ( pclClient->IsCountrySwitch(Switch_Server_Event) ) ����
	}
}

// ����ġ�� �ø��� �ʿ��� ��� ��ġ�� ���Ѵ�. 
bool cltCharServer::IncreaseExp(SI64 exp, SI32 reason)// SI32->SI64 ����ġ �����÷ο춧���� ����	
{
	if(pclCM->IsAlive(GetID()) == FALSE &&
		reason != GETEXP_REASON_VILLAGEWARKILLSCORE)		return false;

	if(pclCI->clIP.IncreaseExp(exp) == TRUE)// SI32->SI64 ����ġ �����÷ο춧���� ����	
	{
		LevelUpAction();
		return true;
	}
	else
	{
		// ����ġ ������� �뺸�Ѵ�. 
		cltGameMsgResponse_GetExp clinfo(GetCharUnique(),  exp, 0, reason);
		cltMsg clMsg(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo), (BYTE*)&clinfo);

		// ��ȯ������ �θ� ������ �θ����� �����Ѵ�.
		SI32 siParentID = GetSummonParentID();
		if(pclCM->IsAlive(siParentID))
		{
			pclCM->CR[siParentID]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}




	return false;

}

// ���� ����ġ�� �ø��� �ʿ��� ��� ��ġ�� ���Ѵ�. 
bool cltCharServer::IncreaseFameExp(SI32 exp, SI32 reason)
{

	cltServer* pclserver = (cltServer*)pclClient;
	TCHAR Nbuffer[512]=TEXT(" ");

	if(pclCI->clIP.IncreaseFameExp(exp) == true)
	{

		if(pclCI->clIP.siFameLevel < 100)
		{
			pclCI->clIP.siFameLevel++;

			// Ŭ���̾�Ʈ ������ ������Ʈ �Ѵ�. 
			SetUpdateSwitch(UPDATE_IP, true, 0);

			// DB�� ������ ������. 
			SendDBLevelInfo(0, 0);	

			// Ŭ���̾�Ʈ�� ������ �ö����� �뺸�Ѵ�. (JAYCEE : �̿����Ե� �뺸�� �Ѵ�.)
			cltGameMsgResponse_LevelUp clinfo(GetCharUnique(), 0, pclCI->clIP.siFameLevel);
			cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUP, sizeof(clinfo), (BYTE*)&clinfo);
			SendNeighbourMsg(&clMsg, true);

			/*
			SI32 itemunique = 0;
			if(pclCI->clIP.siFameLevel == 10)
			{
				itemunique	= 13095;	// �������������. 
				pclserver->pclStatisticsManager->clDailyStatistics.siFameLevel10Num++;
			}
			else if(pclCI->clIP.siFameLevel == 20)
			{
				itemunique	= 18100;	// ��������. 
				pclserver->pclStatisticsManager->clDailyStatistics.siFameLevel20Num++;
			}
			else if(pclCI->clIP.siFameLevel == 30)
			{
				itemunique	= 13098;		// �������������
				pclserver->pclStatisticsManager->clDailyStatistics.siFameLevel30Num++;
			}
			else if(pclCI->clIP.siFameLevel == 40)
			{
				itemunique	= 9400;		// �������庹
				pclserver->pclStatisticsManager->clDailyStatistics.siFameLevel40Num++;
			}

			if(itemunique)
			{
				cltItem clitem;
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
				if(ref >= 0)
				{
					if(pclClient->pclItemManager->pclItemInfo[ref])
					{
						clitem.Set(&pclClient->pclItemManager->pclItemInfo[ref]->clItem);
						clitem.siItemNum = 1;

						//�κ��� �־��. 
						SI32 rtnvalue = 0;
						if(InputItem(pclClient->pclItemManager, &clitem, INPUTITEMPERSON_OPTION_FAMELEVEL, &rtnvalue) == false)
						{
							// �����ϸ� �������� ������. 
							TCHAR buffer[256];
							TCHAR* pText = GetTxtFromMgr(531);
							TCHAR grade[128]= TEXT(" ") ;
							sprintf(grade,TEXT("%d"),pclCI->clIP.siFameLevel);

							Nsprintf(Nbuffer,pText,TEXT("grade"), grade   ,TEXT("item"),  clitem.GetShortName(pclClient->pclItemManager)  ,NULL);
							//sprintf(buffer, pText, pclCI->clIP.siFameLevel, clitem.GetShortName(pclClient->pclItemManager));
							pclserver->SendPostOfficeItemToPerson((TCHAR*)GetName(), &clitem, buffer);
						}

					}
				}

				if(clitem.siUnique)
				{
					cltGameMsgResponse_FameLevelReward clinfo((TCHAR*)GetName(), pclCI->clIP.siFameLevel, &clitem);
					cltMsg clMsg(GAMEMSG_RESPONSE_FAMELEVELREWARD, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
				}
			}*/

				return true;
		}

	}
	else
	{
		// ����ġ ������� �뺸�Ѵ�. 
		cltGameMsgResponse_GetExp clinfo(GetCharUnique(),  0, exp, reason);
		cltMsg clMsg(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo), (BYTE*)&clinfo);
		SendNetMsg((sPacketHeader*)&clMsg);

	}

	return false;

}



// ĳ������ �Ӹ����� �����Ѵ�. 
void cltCharServer::SetHeadChat(bool bheadchatswitch, TCHAR* ptext)
{
	bHeadChatSwitch = bheadchatswitch;
	if(ptext)
	{
		MStrCpy( szHeadChatText, ptext, MAX_HEAD_CHAT_TEXT_SIZE );
	}
}

void cltCharServer::SetPersonalShopBanner(cltShopTradeData* pclshopdata)
{
	if(pclshopdata)
	{
		clShopBanner.SetData(pclshopdata);
	}
}

// ĳ������ PersonalShop������ �����Ѵ�.
void cltCharServer::SetPersonalShopData(bool bpersonalshopswitch, cltShopTradeData* pclshopdata)
{
	/*
	SI32			m_siPersonID;
	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	BOOL			m_ShopMode;			// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	cltItem			m_clItem[ MAX_SHOP_ITEM_NUMBER ];
	SI16			m_siSelectedInventoryPos[ MAX_SHOP_ITEM_NUMBER ];
	SI32			m_siPrice[ MAX_SHOP_ITEM_NUMBER ];*/

		bPersonalShopSwitch = bpersonalshopswitch;

	if( bPersonalShopSwitch == false )
	{
		bPremiumPersonalShop = false;
	}

	if(pclshopdata)
	{
		clShopData.Set(pclshopdata);

		bPremiumPersonalShop = pclshopdata->m_bPremium;
	}
}

void cltCharServer::SetRepairShopData( bool bRepairShopSW, cltShopRepairData* pclRepairData )
{
	bRepairShopSwitch = bRepairShopSW;

	if( bRepairShopSwitch == false )
	{
		if( m_pclShopRepairData )		m_pclShopRepairData->Init();
		return;
	}

	if( m_pclShopRepairData && pclRepairData )
	{
		m_pclShopRepairData->Set( pclRepairData );
	}
}

bool cltCharServer::AfterCharAction()
{

	// ������ ����Ǹ� Ŭ���̾�Ʈ���� �뺸�Ѵ�.
	//if(IsPC(GetCharUnique()) == true || clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
		SendChangedDataToClient();
	}

	// ������ �߿� ���� ������ �� �ְ�
	if(pclClient->bVillageWarSwitch && GetCurrentVillageUnique() > 0 && GetCurrentVillageUnique() < MAX_VILLAGE_NUMBER)
	{
		// �� �ּ����� �����ִ� ������ ���� ��������ϰ�� 
		if(pclClient->pclVillageManager->IsAggressiveVillage(pclCI->clBI.siHomeVillage, GetCurrentVillageUnique()))
		{
			SI32 siCastleUnique = 0;
			// ������ ��� �������� ���� ������ ƨ�ܳ���.
			if(pclClient->pclVillageManager->IsThereCastle(GetCurrentVillageUnique(), &siCastleUnique))
			{
				if(pclCM->IsAlive(siCastleUnique))
				{

					SI32 dx, dy;
					if(pclClient->pclMapManager->pclMapArray[0]->FindEmptyAreaForCharLocate(GetKind(), 
						pclClient->pclVillageManager->pclVillageInfo[ GetCurrentVillageUnique()]->PosX,
						pclClient->pclVillageManager->pclVillageInfo[ GetCurrentVillageUnique()]->PosY,
						20, 
						&dx, &dy, true) == true)
					{
						cltWarpInfo clWarpInfo(true, 0, false, false, dx, dy, -1, -1);
						SetWarpInfo(&clWarpInfo, 300);
					}
				}
			}
		}
	}

	//cyj ���� ���� �ʾȿ� �������
	bool bWarpOut = false;
	if (IsPC(siID))
	{
		if ( GetMapIndex() >= BLACKARMY_MAP_STARTINDEX && GetMapIndex() < BLACKARMY_MAP_STARTINDEX + MAX_BLACKARMY_MAP_CNT)
		{
			// ����ð��� �ƴϸ� ���� ����
			if ( pclClient->pclBlackArmyManager->bWarSwitch == false)
			{
				bWarpOut = true;
			}
			else 
			{
				if (siBlackArmyWarMapIndex >= 0 && siBlackArmyWarMapIndex < MAX_BLACKARMY_MAP_CNT)
				{
					// ����ʿ� �����޾����� �ش� ���� �ƴ� ������ �����ÿ��� ������
					if (GetMapIndex() != pclClient->pclBlackArmyManager->clWarMap[siBlackArmyWarMapIndex].siMapIndex)
					{
						bWarpOut = true;
					}

					// ����ʿ� �����޾����� �ش���� ������ ������� ������
					if (pclClient->pclBlackArmyManager->clWarMap[siBlackArmyWarMapIndex].bWarDoing == false)
					{
						bWarpOut = true;
					}

				}
				// ����ʿ� �������� �ʾ����� ������
				else
				{
					bWarpOut = true;
				}
			}

			if (bWarpOut)
			{

				SI32 warmapindex = GetMapIndex() - BLACKARMY_MAP_STARTINDEX;

				if (warmapindex >= 0)
				{
					cltGate* gate = NULL;
					if( BLACKARMY_GATEUNIQUE_START + warmapindex >= 0 && 
						BLACKARMY_GATEUNIQUE_START + warmapindex < MAX_GATE_NUMBER )
					{
						gate = &pclClient->pclMapManager->pclGateManager->clGate[BLACKARMY_GATEUNIQUE_START + warmapindex];
					}

					if(gate)
					{
						cltWarpInfo clWarp(true, gate->SrcMapIndex, false, false, gate->SrcX, gate->SrcY, -1, -1 );
						SetWarpInfo(&clWarp,56);
					}
				}
			}
		}
	}
	return false;
}

// ���� �÷��� �ð��� ����Ѵ�. 
void cltCharServer::CalcPlayTime()
{
	DWORD playsecond = TABS(pclClient->CurrentClock - dwStartClock) / 1000;
	dwStartClock = pclClient->CurrentClock;
	pclCI->clBI.siPlaySecond += playsecond;
	pclCI->clBI.siPlayEventSecond += playsecond;
	dwPlayingTime += playsecond;

	dwInstantPlayTime += playsecond;

	cltServer* pclserver = (cltServer*)pclClient;

	// �⼮üũ
#ifdef _DEBUG
	DWORD	attendInterval = 30;			// ������ �⼮ üũ �޼��� ������ �ð�
#else
	DWORD	attendInterval = pclserver->siAttendCheckMinute * 60;
#endif 

	if ( dwInstantPlayTime > attendInterval )
	{
		NDate Today;
		Today.SetDate( pclserver->sTime.wYear, pclserver->sTime.wMonth, pclserver->sTime.wDay );

		if ( Today.GetDateVary() >= pclserver->clAttendEventStart.GetDateVary() &&
			Today.GetDateVary() <= pclserver->clAttendEventEnd.GetDateVary() )
		{
			// �⼮ ��ư�� �����µ� ���� �ʿ� ����
			if ( (pclCI->clBI.bAttendToday[ ATTEND_EVENTTYPE_ATTENDANCE ] == false) 
				&& (pclCI->clBI.bIsAttendTime == false)
				)
			{
				cltGameMsgResponse_DemandAttend clInfo( -1,-1, ATTEND_EVENTTYPE_SEASON2, true );
				cltMsg clMsg( GAMEMSG_RESPONSE_DEMANDATTEND, sizeof(clInfo), (BYTE*)&clInfo );
				SendNetMsg( (sPacketHeader*)&clMsg );
			}
			// �⼮�� �ð��� �Ǿ���
			if ( pclCI->clBI.bIsAttendTime == false )
			{
				pclCI->clBI.bIsAttendTime = true;
			}
		}
	}

	// �Ƿι���- ���⼭�� uiTiredState �� ���º����� �ٷ��� �ʰ�,  CharServer_Action �� CHAR_UPDATETIMER_TIREDSYSTEM_TIME_LIMIT �κп��� �ٷ��.
	/*
	if( true == bTiredSystem )
	{
		if( dwPlayingTime > 60 * 60 * 3 && dwPlayingTime <= 60 * 60 * 5)
		{
			// ���� �Ǿ������ Ŭ���̾�Ʈ�� ����
			if ( uiTiredState != TIRED_STATUS_LITTLE_TIRED )
			{
				cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS , TIRED_STATUS_LITTLE_TIRED );
				SendNetMsg((sPacketHeader*)&clMsg);
				// ���� ��ġ�� �ݸ� �޴´�.
				uiTiredState = TIRED_STATUS_LITTLE_TIRED;
			}
		}
		else if( dwPlayingTime > 60 * 60 * 5 )
		{
			if ( uiTiredState != TIRED_STATUS_MUCH_TIRED )
			{
				cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS , TIRED_STATUS_MUCH_TIRED );
				SendNetMsg((sPacketHeader*)&clMsg);
				// �ƹ��͵� ���� �� �Ѵ�.
				uiTiredState = TIRED_STATUS_MUCH_TIRED;
			}
		}
		else
		{
			if ( uiTiredState != TIRED_STATUS_NORMAL )
			{
				cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS , TIRED_STATUS_NORMAL );
				SendNetMsg((sPacketHeader*)&clMsg);
				uiTiredState = TIRED_STATUS_NORMAL;
			}
		}
	}
	*/
	// ���� (���� �ִ� ����� ���ؼ� �÷��� �ð��� ���Ѵ�)
	if( pclCI->clBI.siHomeVillage > 0 && pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT)
		pclCI->clBI.clVillageScoreInfo.siPlayTime += playsecond;

}


// �ΰ����� ġ��. 
bool cltCharServer::HealAI()
{
	SI32 magickind = MAGIC_MON_HEAL;
	if(pclKI->siMagic != magickind)return false;

	if(TABS(pclClient->CurrentClock - dwLastMagicClock) < 5000)return false;

	dwLastMagicClock = pclClient->CurrentClock;

	bool bwarswitch = false;


	SI32 id = pclCM->FindDamagedFriend(GetID(), GetMapIndex(), GetX(), GetY(), bwarswitch);
	if(id == 0)return false;

	SI32 returnval = 0;
	if(CanMagic(id, magickind, &returnval) == true)
	{
		bBusySwitch = false;

		pclClient->pclOrderManager->OrderMagic(GetID(), id, magickind, BY_COMPUTER);

		return true;		
	}

	return false;

}



// Ŭ���̾�Ʈ�� ������ ���� ������ �޽������� �ð� ������ Ȯ���Ѵ�. 
bool cltCharServer::CheckLastMsgClock(DWORD interval)
{
	if(TABS(pclClient->CurrentClock - dwLastSendServerClock) < (int)interval)	return true;

	dwLastSendServerClock = pclClient->CurrentClock;

	return false;
}

// �����۸��� ������ ������ ������ üũ�Ѵ�. 1�ʿ� 5ȸ �̻��� ��û�� ����
bool cltCharServer::CheckItemMallBadRequest()
{
	static SI32 requestCnt = 0;
	static DWORD startedClock = 0;

	if(requestCnt == 0)
	{
		startedClock = pclClient->CurrentClock;
	}	
	else if(requestCnt == 10 )
	{
		if(pclClient->CurrentClock - startedClock  < 1000  )
			return false;
	}

	requestCnt++;
	if(requestCnt > 10)
		requestCnt = 0;

	return true;
}


// ������� �����. 
void cltCharServer::MakeHunger(SI32 addHunger)
{
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))return ;

	SI32 hunger = max(1, pclCI->clIP.GetLevel() / 8);

	// ���ѷ� �̻��� ���Ը� ���� �ٴϸ� ����� ������ 2.5��
	if ( pclClient->siServiceArea != ConstServiceArea_China )
	{
		if(pclCI->clCharItem.GetTotalWeight(pclClient->pclItemManager) >= clPB.GetMaxWeight())
		{
			hunger = hunger * 5 /2;
		}
	}

	hunger = hunger + addHunger;

	clPB.clHealth.DecreaseHungry(hunger);
	SetUpdateSwitch(UPDATE_HEALTH, true, 0);

}

void cltCharServer::SetUpdateSwitch(SI32 updatetype, bool mode, SI32 para1)
{
	if(updatetype < 0 && updatetype >= MAX_UPDATE_SWITCH)		return;

	clUpdateInfo[updatetype].bMode		= mode;
	clUpdateInfo[updatetype].siPara1	= para1;
}

void cltCharServer::SetWasteDurUpdateSwitch(SI32 itemposition, bool mode)
{
	if(itemposition < 0 && itemposition >= MAX_ITEM_PER_PERSON)		return;

	if(mode)
	{
		clSimpleVary.bWasteDurSwitch = true;
	}

	clSimpleVary.WasteDurUpdateSwitch[itemposition] = mode;
}

void cltCharServer::SetSummonWasteDurUpdateSwitch( SI32 SummonIndex, SI32 itemposition, bool mode )
{
	if( SummonIndex < 0 || SummonIndex >= clSimpleVary.bSummonWasteDurSwitch.GetSize() )						return;
	if( itemposition < 0 || itemposition >= clSimpleVary.SummonWasteDurUpdateSwitch[SummonIndex].GetSize() )	return;

	if(mode)
	{
		clSimpleVary.bSummonWasteDurSwitch[SummonIndex] = true;
	}

	clSimpleVary.SummonWasteDurUpdateSwitch[SummonIndex][itemposition] = mode;
}

//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. -
//�̰����� ����ü�� �������� ��´� - ���ݽ�, ����.
void cltCharServer::SetCrystalDurUpdateSwitch(SI32 itemposition, bool mode)
{
	if(itemposition < 0 && itemposition >= MAX_SHAPE_ITEM)		return;

	//if(pclClient->GameMode != GAMEMODE_SERVER)		return;
	//cltCharServer* pclchar = (cltCharServer*)pclCM->CR[GetID()];

	if(mode)
	{
		clSimpleVary.bCrystalDurSwitch = true;
	}

	clSimpleVary.CrystalDurUpdateSwitch[itemposition] = mode;
}

void cltCharServer::SubDecreaseCrystalDur(SI16 ActiveItem,SI16 durvary)
{
	cltItem* pclitem;
	pclitem = (cltItem*)&pclCI->clCharItem.clItem[ActiveItem];

	if(pclitem->siUnique != 0 )
	{
		// �������� �Ҹ��Ų��.
		if(pclitem->DecreaseCrystalDurability(pclClient->pclItemManager, durvary))
		{
			// �������� �Ҹ�Ǿ����� ǥ���صд�.
			SetCrystalDurUpdateSwitch(ActiveItem, true);
		}
	}
}
void cltCharServer::DecreaseCrystalDurAC(SI16 durvary)
{
	// ����� ��Ƽ�� ������ ���ҽ�Ų��.
	SubDecreaseCrystalDur(ActiveWeapon,durvary);
}

//�̰����� ����ü�� �������� ��´� - ����, �� ����.
void cltCharServer::DecreaseCrystalDurDE(SI16 durvary)
{
	// ���� ��� ����Ų��. - ���� ���������� �̰����� ó���Ѵ�.
	SubDecreaseCrystalDur(PERSONITEM_HELMET,durvary);
	SubDecreaseCrystalDur(PERSONITEM_ARMOUR,durvary);
	SubDecreaseCrystalDur(PERSONITEM_BELT,durvary);
	SubDecreaseCrystalDur(PERSONITEM_SHOES,durvary);

	// �ƹ�Ÿ ����ü�� �������� ���� ��Ű�� �ʴ´�.
	//SubDecreaseCrystalDur(PERSONITEM_NECK,durvary);
	//SubDecreaseCrystalDur(PERSONITEM_RING1,durvary);
	//SubDecreaseCrystalDur(PERSONITEM_RING2,durvary);
	//SubDecreaseCrystalDur(PERSONITEM_MANTLE,durvary);
	//SubDecreaseCrystalDur(PERSONITEM_HAT,durvary);
	//SubDecreaseCrystalDur(PERSONITEM_DRESS,durvary);
}

void cltCharServer::SubSendClientCrystalDurability(SI16 ActiveItem)
{
	cltItem* pclitem;
	pclitem = (cltItem*)&pclCI->clCharItem.clItem[ActiveItem];

	if(pclitem->siUnique != 0 )
	{
		// �������� 1%������ ���ߴ��� Ȯ���Ѵ�.
		if(pclitem->GetDecreaseCrystalDurabilityPer(pclClient->pclItemManager))
		{			
			// �������� �Ҹ�Ǿ����� Ŭ���̾�Ʈ�� �˸���.
			cltGameMsgResponse_CrystalItemInfo clinfo(ActiveItem , pclitem);
			cltMsg clMsg(GAMEMSG_RESPONSE_CRYSTALITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}


// ����� ����ü ������ ������ Client�� ������.
void cltCharServer::SendClientCrystalDurability( )
{
	/*for(SI16 pos = 0; pos <PERSONITEM_INV0 ; pos++)
	{
		switch(pos)
		{
		case PERSONITEM_HELMET :
		case PERSONITEM_ARMOUR :
		case PERSONITEM_NECK :
		case PERSONITEM_RING1 :
		case PERSONITEM_RING2 :
		case PERSONITEM_BELT :
		case PERSONITEM_SHOES :
		case PERSONITEM_MANTLE :
		case PERSONITEM_HAT :
		case PERSONITEM_DRESS :
			SubSendClientCrystalDurability(pos);
			break;
		}		
	}
	*/
		SubSendClientCrystalDurability(PERSONITEM_HELMET);
	SubSendClientCrystalDurability(PERSONITEM_ARMOUR);
	SubSendClientCrystalDurability(PERSONITEM_BELT);
	SubSendClientCrystalDurability(PERSONITEM_SHOES);

	SubSendClientCrystalDurability(ActiveWeapon);
}

void cltCharServer::SubDecreaseCrystalDurForSummon( SI16 siActiveItem, SI16 siDurvary )
{
	SI32 siParentID = GetSummonParentID();
	if ( FALSE == pclCM->IsValidID(siParentID) )
	{
		return;
	}

	cltCharServer* pclParentChar = (cltCharServer*)pclCM->CR[siParentID];
	if ( NULL == pclParentChar )
	{
		return;
	}

	SI16 siSummonIndex = GetSummonIndex();
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	cltItem* pclitem = pclParentChar->pclCI->clSummonInfo.GetSummonEquipItem( siSummonIndex, siActiveItem );
	if ( NULL == pclitem )
	{
		return;
	}

	if ( 0 < pclitem->siUnique )
	{
		// �������� �Ҹ��Ų��.
		if ( pclitem->DecreaseCrystalDurability(pclClient->pclItemManager, siDurvary) )
		{
			// �������� �Ҹ�Ǿ����� ǥ���صд�.
			SetCrystalDurUpdateSwitch( siActiveItem, true );
		}
	}

}


void cltCharServer::DecreaseCrystalDurACForSummon( SI16 siDurvary )
{
	// ����� ��Ƽ�� ������ ���ҽ�Ų��.
	SubDecreaseCrystalDurForSummon( ActiveWeapon, siDurvary );

}

void cltCharServer::DecreaseCrystalDurDEForSummon( SI16 siDurvary )
{
	// ���߿� ������������ �߰��ɼ��� �ֱ⶧���� �������� ���θ� üũ�Ѵ�
	SubDecreaseCrystalDurForSummon( PERSONITEM_HELMET,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_ARMOUR,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_NECK,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_RING1,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_RING2,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_BELT,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_SHOES,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_MANTLE,	siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_HAT,		siDurvary );
	SubDecreaseCrystalDurForSummon( PERSONITEM_DRESS,	siDurvary );
}

void cltCharServer::SendClientCrystalDurabilityForSummon()
{
	SI32 siParentID = GetSummonParentID();
	if ( FALSE == pclCM->IsValidID(siParentID) )
	{
		return;
	}

	cltCharServer* pclParentChar = (cltCharServer*)pclCM->CR[siParentID];
	if ( NULL == pclParentChar )
	{
		return;
	}

	SI16 siSummonIndex = GetSummonIndex();
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	for ( SI32 siPos=0; siPos<MAX_SHAPE_ITEM; ++siPos )
	{
		cltItem* pclItem = pclParentChar->pclCI->clSummonInfo.GetSummonEquipItem( siSummonIndex, siPos );
		if ( (NULL == pclItem) || (0 >= pclItem->siUnique) )
		{
			continue;
		}

		if ( pclItem->GetDecreaseCrystalDurabilityPer(pclClient->pclItemManager) )
		{
			cltGameMsgResponse_SummonChangeItem clInfo( siSummonIndex, siPos, pclItem );
			cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONCHANGEITEM, sizeof(clInfo), (BYTE*)&clInfo );
			pclParentChar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
	}

}

void cltCharServer::SetSkillUpdate(SI32 skillunique, SI32 addskillexp)
{
	//=================================================
	// ���� �˻�
	//=================================================
	if(skillunique < 0 || skillunique >= MAX_SKILL_NUMBER) return;

	if( addskillexp < 1 ) return;

	clSimpleVary.SkillUpdateSwitch[skillunique] = true;
	clSimpleVary.bSkillExpUpdateSwitch			= true;

	//=================================================
	// Person �� Skill ���� Update�� 
	//=================================================
	SI32 oldskilllevel	= pclCI->clCharSkill.GetSkillExpLevel(skillunique);

	pclCI->clCharSkill.clSkill[skillunique].siSkillExp	+= addskillexp;	// ��ų ����ġ���� 

	cltGameMsgResponse_OneSkill clInfo(skillunique, &pclCI->clCharSkill.clSkill[skillunique]);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSON_ONESKILL, sizeof(clInfo), (BYTE*)&clInfo);
	SendNetMsg((sPacketHeader*)&clMsg);

	//=================================================
	// Person �� Skill ���� Update �� 
	//=================================================
	SI32 newskilllevel	= pclCI->clCharSkill.GetSkillExpLevel(skillunique);
	if(oldskilllevel < newskilllevel)					// ������ ������ �ö��ٸ�, 
	{
		cltServer* pclserver = (cltServer*)pclClient;

		SI32 totalskilllevel = pclCI->clCharSkill.clSkill[skillunique].uiLevel + newskilllevel;			
		pclserver->SendServerResponseMsg(0, SRVAL_SETSKILL,  skillunique, totalskilllevel, GetCharUnique());
		// ��尡 �ִٸ�
		if ( pclCI->clBI.clGuildRank.IsGuildMember() == true )
		{
			cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
			if ( pGuild )
			{
				sDBRequest_Guild_ChangeGuildPoint clMsg(pGuild->siUnique,pclCI->GetPersonID(),6,TYPE_GUILDPOINT_ADD_SKILLLEVELUP);
				pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}

		//KHY - 1205 - ���������� ����
		if (pclClient->IsCountrySwitch(Switch_LevelUpNotice) )
		{
			if(IsPC(GetCharUnique()))
			{
				TCHAR szName[MAX_PLAYER_NAME];
				MStrCpy( szName, GetName(), MAX_PLAYER_NAME );

				if (pclCI->clBI.clGuildRank.IsGuildMember() == true ) //���.
				{
					cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCI->clBI.clGuildRank.GetGuildUnitIndex());
					if ( pGuild )
					{
						cltGameMsgResponse_LevelUpNotice clinfo(szName, 0, totalskilllevel , skillunique , 1);
						cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
						//KHY - 0131 - ��� ��� �޽��� ���� - �̸����� �˻� - ����ũ��ȣ �˻�����!
						pclClient->pclCM->SendAllGuildMemberMsg((sPacketHeader*)&clMsg,pGuild->siUnique,pGuild->szName);

					}
				}

				if(pclCI->clBI.siHomeVillage > 0 && pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT) //���� 
				{

					//KHY - 1205 - ���������� ����
					cltGameMsgResponse_LevelUpNotice clinfo(szName, 0, totalskilllevel , skillunique , 2);
					cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg,pclCI->clBI.siHomeVillage);

				}
			}
		}
		cltSkillInfo* pclskillinfo = pclserver->pclSkillManager->pclSkillInfo[skillunique];
		if ( ( pclskillinfo->siSkillAtb & SKILLATB_LIST) != 0 )
		{
			if( newskilllevel % 5 == 0 )
			{
				SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

				// ���� ���� ������ ����
				// �������*�������*NPCRATE*5+40000;
				CMinisterGongzo *pclminister = (CMinisterGongzo *)pclserver->pclMinisterMgr->GetMinister(MINISTER_GONGZO);
				GMONEY money = newskilllevel * newskilllevel * npcrate * 5 + 40000;

				// �߰� ���� ������(�����Ǽ��� ����) 
				GMONEY addCraftFund = pclminister->m_stGongzoMember.siCraftFund;
				money = money + ( money * addCraftFund / 100 );

				// �Ƿι��� �ý��� ����
				money = CalcTiredPoint( money );

				if(money)
				{
					// �ս��ڻ꿡 ������ �ִ��� Ȯ�� ���Ѵ�. 
					CMinisterGoonzu *pclministergoonzu = (CMinisterGoonzu *)pclserver->pclMinisterMgr->GetMinister(MINISTER_GOONZU);
					INT64 budget = pclministergoonzu->GetTotalFinances();
					//if(budget >= money)
					{
						// �ս��ڽſ��� �ڱ��� �Ҹ��Ų��. �߱��� ����
						pclministergoonzu->RemoveFinances(money);

						sDBRequest_ChangeMoney clMsg(GetID(), pclCI->GetPersonID(), CHANGE_MONEY_REASON_CRAFTFUND, money );
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						pclserver->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_SKILLMONEY, money);
					}

				}

			}
		}


		// ������� ���� - ������
		if ( (PERSONITEM_WEAPON1 == ActiveWeapon) || (PERSONITEM_WEAPON2 == ActiveWeapon) )
		{
			SI32 siItemType				= pclCI->clCharItem.GetWeaponType( pclClient->pclItemManager, ActiveWeapon );
			SI32 siWeaponSkillUnique	= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
			UI08 uiGetEmblem			= 0;

			// ���� ����ϴ� ������ ��ų�� �˻��Ѵ�
			if ( siWeaponSkillUnique == skillunique )
			{
				uiGetEmblem = pclClient->pclSkillManager->m_clMasterManager.GetEmblemIndex( siItemType, newskilllevel );

				if ( m_uiWeaponSkillMasterEmblem != uiGetEmblem )
				{
					m_uiWeaponSkillMasterEmblem = uiGetEmblem;

					cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_EMBLEM, false, uiGetEmblem );
					cltGameMsgResponse_StatusInfo	clInfo( GetCharUnique(), &clChangeInfo );

					cltMsg clEmblenMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo);
					SendNeighbourMsg( &clEmblenMsg, true );
				}
			}
		}	// ����	if ( (PERSONITEM_WEAPON1 == ActiveWeapon) || (PERSONITEM_WEAPON2 == ActiveWeapon) ) 
	}

}

BOOL cltCharServer::DoAnimationDelay(BOOL atb)
{
	SI32	delay;
	SI32	framenumber;

	bool	IsHero = (pclKI != NULL && GetKindInfo(true)->IsAtb(ATB_HERO));

	// by LEEKH 2007.12.24 - TRANSFORM
	if( clTransFormInfo.bTransFormMode == true )
	{
		// ������ ���... ������ ĳ������ ����� ���ϸ��̼��� ó���Ѵ�.- ������...
		if( Animation == ANITYPE_ATTACK || Animation == ANITYPE_ATTACK1 || Animation == ANITYPE_ATTACK2 )
		{
			IsHero = true;
		}
	}

	if( IsHero )
	{
		framenumber = pclCharDraw->GetFrameNum(HeroAnimation);
		delay		= pclCharDraw->GetDelayTime(HeroAnimation, GetKind() );
	}
	else
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 aniref	= GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(Animation);

		if(aniref == -1)
		{			
			return FALSE;
		}

		// by LEEKH 2007.12.24 - TRANSFORM
		framenumber = GetKindInfo(true)->clCharAniManager.GetFrameNum(aniref);
		delay		= GetKindInfo(true)->clCharAniManager.GetDelay(aniref);
	}

	if(framenumber == 0)
	{
		pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("GetFrameNum() Kind:%d ani:%d"), GetKind(), Animation);
	}


	// �ѹ��� �ִϸ��̼� �ϰ� �Ǿ� �ִ� ���, 
	clAniInfo.dwElapsedClock = min(10000, (pclClient->CurrentClock - clAniInfo.dwAniStartClock));

	SI32 frame = 0;
	if(delay == 0)
	{
		frame = 0;
	}
	else
	{
		DWORD dwdata	= clAniInfo.dwElapsedClock / (delay * 50);
		SI32 sidata		= (SI32)dwdata;
		frame = min(framenumber - 1, sidata);
	}
	// ���� ������ ������ �����̾��ٸ� ù �������� ����. 
	if(clAniInfo.GetAniStep() == (framenumber - 1))
	{
		clAniInfo.SetAniStep(0);
		clAniInfo.dwAniStartClock	= pclClient->CurrentClock;
		clAniInfo.dwElapsedClock	= 0;

		return TRUE;
	}
	else
	{
		SI32 oldframe = clAniInfo.GetAniStep();
		if(oldframe != frame)
		{
			clAniInfo.SetAniStep(frame);

			return TRUE;
		}
	}


	return FALSE;
}


void cltCharServer::CharServerStatusAction()
{

	// ���� �ߵ��Ǿ����� ���� ������ �ȴٸ� 
	if( clInnerStatus.IsStatus(INNERSTATUS_POISON) || clInnerStatus.IsStatus(INNERSTATUS_POISON_SETITEM_EFFECT) )
	{
		// 1�� ���� �������� ���ҽ�Ų��.
		if(TABS(pclClient->CurrentClock - dwPoisonCheckClock) > 2000)
		{
			dwPoisonCheckClock = pclClient->CurrentClock;

			if(GetLife() > 0) 
			{
				cltAttackTarget cltarget(GetID(), &pclCI->clBI.clPos, 10000,0); // ���غ��� - 100%�� 10000 �̴�.
				cltAttackDamageInfo cldamageinfo(ATTACKTYPE_POISONSTATUS, siPoisonPower, 0);
				((cltServer*)pclClient)->HitOperation(siPoisonCharUnique, &cldamageinfo, 1, &cltarget, pclMap);
			}
		}

		if((DWORD)TABS(pclClient->CurrentClock - dwPoisonStartClock) > dwPoisonLastClock)
		{
			SI32 siChangeType = 0;
			// �ߵ� ���� �ص�.
			if( clInnerStatus.IsStatus( INNERSTATUS_POISON ) )
			{
				clInnerStatus.DelStatus(INNERSTATUS_POISON);
				siChangeType = CHANGETYPE_POISON;
			}
			else if( clInnerStatus.IsStatus( INNERSTATUS_POISON_SETITEM_EFFECT ) )
			{
				clInnerStatus.DelStatus(INNERSTATUS_POISON_SETITEM_EFFECT);
				siChangeType = CHANGETYPE_POISON_SETITEM_EFFECT;
			}

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo( siChangeType, false );
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	// ������������ ��������� �������ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwIceStartClock) > dwIceLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_ICE);

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_ICE, false);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}

		// PCK : SOULGUARD_ABILITY_NOICE�� ���� ��� �߰� (09.07.08)
		if(pclClient->IsCountrySwitch(Switch_SoulGuard))
		{
			SI32 siNoIce = SoulGuardAbilityActivity( SOULGUARD_ABILITY_NOICE );
			if( 0 < siNoIce)
			{
				clInnerStatus.DelStatus(INNERSTATUS_ICE);

				// �̿����� �뺸 
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_ICE, false);
				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}	
		}
	}

	// ���� �������ݿ� ���ߴٰ� �����ȴٸ�
	if(clInnerStatus.IsStatus(INNERSTATUS_FROZEN))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwFrozenStartClock) > dwFrozenLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_FROZEN);

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FROZEN, false);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}

		// PCK : SOULGUARD_ABILITY_NOICE�� ���� ��� �߰� (09.07.08)
		if(pclClient->IsCountrySwitch(Switch_SoulGuard))
		{
			SI32 siNoIce = SoulGuardAbilityActivity( SOULGUARD_ABILITY_NOICE );
			if( 0 < siNoIce)
			{
				clInnerStatus.DelStatus(INNERSTATUS_FROZEN);

				// �̿����� �뺸 
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FROZEN, false);
				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}
		}
	}
	// ���Ͽ� �ɷ� ���� �ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_STUN))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwStunStartClock) > dwStunLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_STUN);

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_STUN, false);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}

		// PCK : SOULGUARD_ABILITY_NOICE�� ���� ��� �߰� (09.07.08)
		if(pclClient->IsCountrySwitch(Switch_SoulGuard))
		{
			SI32 siNoIce = SoulGuardAbilityActivity( SOULGUARD_ABILITY_NOICE );
			if( 0 < siNoIce)
			{
				clInnerStatus.DelStatus(INNERSTATUS_STUN);

				// �̿����� �뺸 
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_STUN, false);
				cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				SendNeighbourMsg(&clMsg, true);
			}
		}
	}

	// ���ݷ� ���Ϸ������� �������ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEATTACK))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwCurseAttackStartClock) > dwCurseAttackLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_CURSEATTACK);
			siCurseAttackPower					= 0;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEATTACK, false, GetMana());
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}

	}

	// ũ��Ƽ�� ���Ϸ������� �������ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSECRITICAL))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwCurseCriticalStartClock) > dwCurseCriticalLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_CURSECRITICAL);
			siCurseCriticalPower				= 0;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSECRITICAL, false, GetMana());
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	// ���� ���Ϸ������� �������ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEDEFENSE))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwCurseDefenseStartClock) > dwCurseDefenseLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_CURSEDEFENSE);
			siCurseDefensePower				= 0;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEDEFENSE, false, GetMana());
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	// �̵��ӵ� ���Ϸ������� �������ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_CURSEMOVESPEED))
	{ 
		if((DWORD)TABS(pclClient->CurrentClock - dwCurseMovespeedStartClock) > dwCurseMovespeedLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_CURSEMOVESPEED);
			siCurseMovespeedPower				= 0;
			clPB.GivenMoveSpeed					= 0;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEMOVESPEED, false, GetMana(), 0);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	// �̵��ӵ� ���� ������ ���� �ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_INCREASEMOVESPEED))
	{
		if((DWORD)TABS(pclClient->CurrentClock - dwIncreaseMovespeedStartClock) > dwIncreaseMovespeedLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_INCREASEMOVESPEED);
			siIncreaseMovespeedPower			= 0;
			clPB.GivenMoveSpeed					= 0;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_INCREASEMOVESPEED, false, 0);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);

			// �̵��ӵ� ���� �޼��� �ѷ���
			TCHAR* pTitle = GetTxtFromMgr(5614);
			TCHAR *pText = GetTxtFromMgr(6413);
			cltGameMsgResponse_Notice clNotice(pTitle, pText,true);
			cltMsg clMsg2(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);
			SendNetMsg((sPacketHeader*)&clMsg2);


		}
	}


	// ���� �ߵ��Ǿ����� ���� ������ �ȴٸ� 
	if(clInnerStatus.IsStatus(INNERSTATUS_MAGICFIRE))
	{
		// 1�� ���� �������� ���ҽ�Ų��.
		if(TABS(pclClient->CurrentClock - dwMagicFireCheckClock) > 2000)
		{
			dwMagicFireCheckClock = pclClient->CurrentClock;

			if(GetLife() > 0) 
			{
				cltAttackTarget cltarget(GetID(), &pclCI->clBI.clPos, 100,0);
				cltAttackDamageInfo cldamageinfo(ATTACKTYPE_MAGICFIRESTATUS, siMagicFirePower, 0);
				((cltServer*)pclClient)->HitOperation(siMagicFireCharUnique, &cldamageinfo, 1, &cltarget, pclMap);

			}
		}

		if((DWORD)TABS(pclClient->CurrentClock - dwMagicFireStartClock) > dwMagicFireLastClock)
		{
			clInnerStatus.DelStatus(INNERSTATUS_MAGICFIRE);

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_MAGICFIRE, false);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	// ���������� �����Ǵ� DRAIN�� ������ �ʿ䰡 ����.
	if(bDrainSwitch)
	{
		bDrainSwitch = false;
	}


	//---------------------------------
	// ��ó�� 
	//---------------------------------
	if(clJim.siJimHostCharUnique)
	{
		bool bjimloseswitch = false;

		// ���� ��ü ĳ���Ͱ� ��ȿ�ϸ� ���� Ǯ����. 
		SI32 hostid = pclCM->GetIDFromCharUnique(clJim.siJimHostCharUnique);
		if(pclCM->IsAlive(hostid) == false)
		{
			bjimloseswitch = true;
		}
		// ��� �־ ���� ������ ��ġ���� ������, 
		else if(pclCM->CR[hostid]->clJim.siJimSlaveCharUnique != GetCharUnique())
		{
			bjimloseswitch = true;
		}

		// ���� �ð� ����ϸ� ���� Ǯ����. 
		if(TABS(pclClient->GetFrame() - clJim.dwJimFrame) > 140)
		{
			bjimloseswitch = true;
		}

		if(bjimloseswitch == true)
		{
			// ���� Ǯ����. 
			LoseJim();
		}
	}

	//---------------------------------
	// ����ȿ���� �ο��Ѵ�. 
	//----------------------------------
	// 10�ʿ� �ѹ��� Ȯ���Ѵ�.
	//[����] ���ְ� ����������� ����ȿ�� ����. => 2008-5-8
	if(TRUE == pclCM->IsAlive(GetID()) && TABS(pclClient->CurrentClock - dwGeneralEftSetClock) > 10000)
	{
		//[����] ����ȿ���� �ش� ĳ���� ����������� �����ϰ� ����.
		dwGeneralEftSetClock = pclClient->CurrentClock;

		bool bacswitch		= false;
		SI32 acuprate		= 0;
		bool bdamageswitch	= false;
		SI32 damageuprate	= 0;
		bool balluserswitch = false;

		//[�߰� : Ȳ���� 2007. 11. 27 "gmmode" 4�� �ߵ��ÿ� ��ڰ� �ֺ� ĳ���� ���ֿ� ���� ����ȿ���� ��.]
		if((pclCI->clBI.uiGRank == GRANK_KING || pclCI->clBI.uiGMMode == GMMODE_ATB_BUFF))
		{  
			balluserswitch	= true;
			bacswitch		= true;
			acuprate		= min(50, pclCI->clIP.GetLevel() * 35 / 100);
			bdamageswitch	= true;
			damageuprate	= min(50, pclCI->clIP.GetLevel() * 20 / 100);
		}
		else
		{
			if(pclCI->clCharRank.siType == RANKTYPE_CITYHALL)
			{
				bacswitch = true;
				acuprate  = min(50, pclCI->clIP.GetLevel() * 35 / 100);
			}
			else if(pclCI->clCharRank.siType)
			{
				bdamageswitch = true;
				damageuprate  = min(50, pclCI->clIP.GetLevel() * 20 / 100);
			}
		}

		if(bacswitch || bdamageswitch)
		{
			GeneralEft(bacswitch, acuprate, bdamageswitch, damageuprate , balluserswitch );
		}
	}

	//-------------------------------------
	// ���� ��� ����. 
	//--------------------------------------
	if(bNoFirstAttackSwitch == true)
	{
		if(TABS(pclClient->CurrentClock - dwNoFirstAttackClock) > 60000)
		{
			bNoFirstAttackSwitch = false;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, bNoFirstAttackSwitch);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}
	//-------------------------------------
	// ICE ��� ����. 
	//--------------------------------------
	if(bNoIceAttackSwitch == true)
	{
		if(TABS(pclClient->CurrentClock - dwNoIceAttackClock) > 30000)
		{
			bNoIceAttackSwitch = false;

			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOICEATTACK, bNoIceAttackSwitch);
			cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}
}

// ĳ���͸� �󸰴�. 
BOOL cltCharServer::SetIce(SI32 damage, DWORD _dwAddClock)
{
	DWORD lastclock  = 2000 + min(10000, damage*100) + _dwAddClock;
	DWORD startclock = pclClient->CurrentClock;

	// ICE�� ����ؼ� Ÿ���� ������ �����ִ� �簣���� ���� ������ ���� 
	// ������ �����ð��� ������Ų��.
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE))
	{
		DWORD leftclock = pclClient->CurrentClock - dwIceStartClock;
		if(leftclock>lastclock)
		{
			lastclock  = dwIceLastClock;
			startclock = dwIceStartClock;
		}
	}
	clInnerStatus.SetStatus(INNERSTATUS_ICE);
	dwIceLastClock	= lastclock;
	dwIceStartClock	= startclock;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_ICE, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	// ������ ���� 
	OrderForceStop(GetID(), BY_PLAYER);

	return TRUE;

}

// ���� �������� �󸰴�.
BOOL cltCharServer::SetFrozen(SI32 damage)
{
	DWORD lastclock  = 2000 + min(10000, damage*100);
	DWORD startclock = pclClient->CurrentClock;

	// ICE�� ����ؼ� Ÿ���� ������ �����ִ� �簣���� ���� ������ ���� 
	// ������ �����ð��� ������Ų��.
	if(clInnerStatus.IsStatus(INNERSTATUS_FROZEN))
	{
		DWORD leftclock = pclClient->CurrentClock - dwFrozenStartClock;
		if(leftclock>lastclock)
		{
			lastclock  = dwFrozenLastClock;
			startclock = dwFrozenStartClock;
		}
	}
	clInnerStatus.SetStatus(INNERSTATUS_FROZEN);
	dwFrozenLastClock	= lastclock;
	dwFrozenStartClock	= startclock;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FROZEN, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	// ������ ���� 
	OrderForceStop(GetID(), BY_PLAYER);

	return TRUE;

}
// ���� ȿ���� �ش�
BOOL cltCharServer::SetStun(SI32 value)
{

	DWORD lastclock  =  value*1000;
	DWORD startclock = pclClient->CurrentClock;

	// ICE�� ����ؼ� Ÿ���� ������ �����ִ� �簣���� ���� ������ ���� 
	// ������ �����ð��� ������Ų��.
	if(clInnerStatus.IsStatus(INNERSTATUS_STUN))
	{
		DWORD leftclock = pclClient->CurrentClock - dwStunLastClock;
		if(leftclock>lastclock)
		{
			lastclock  = dwStunLastClock;
			startclock = dwStunStartClock;
		}
	}
	clInnerStatus.SetStatus(INNERSTATUS_STUN);
	dwStunLastClock	= lastclock;
	dwStunStartClock	= startclock;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_STUN, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	// ������ ���� 
	OrderForceStop(GetID(), BY_PLAYER);

	return TRUE;
}
// �ҿﰡ�� - �󸰴�
BOOL cltCharServer::SetSoulGuardIce(SI32 value)
{

	DWORD	lastclock  =  value*1000;
	DWORD	startclock = pclClient->CurrentClock;

	// ICE�� ����ؼ� Ÿ���� ������ �����ִ� �簣���� ���� ������ ���� 
	// ������ �����ð��� ������Ų��.
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE))
	{
		DWORD leftclock = pclClient->CurrentClock - dwStunLastClock;
		if(leftclock>lastclock)
		{
			lastclock  = dwIceLastClock;
			startclock = dwIceStartClock;
		}
	}
	clInnerStatus.SetStatus(INNERSTATUS_ICE);
	dwIceLastClock	= lastclock;
	dwIceStartClock	= startclock;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(INNERSTATUS_ICE, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	// ������ ���� 
	OrderForceStop(GetID(), BY_PLAYER);

	return TRUE;
}

// �ҿ� �¿��.
BOOL cltCharServer::SetMagicFire(SI32 enemy, SI32 damage)
{
	// �ҿ� ���� ������ 3���� 1�� ����.
	damage /= 10;

	clInnerStatus.SetStatus(INNERSTATUS_MAGICFIRE);
	siMagicFireCharUnique	= enemy;			// ���� �¿� ĳ����
	dwMagicFireLastClock	= 20000;			// ���� �����ð�. 
	dwMagicFireStartClock	= pclClient->CurrentClock;		// �� ���۽ð�. 
	siMagicFirePower		= damage;			// ���� ����. 
	dwMagicFireCheckClock  = pclClient->CurrentClock;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_MAGICFIRE, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

// �ߵ��� ��Ų��. 
BOOL cltCharServer::SetPoison(SI32 poisoner, SI32 damage, DWORD _dwClock )
{
	// POISON������ �������� damage�� �ݸ� ����.
	damage /= 2;

	clInnerStatus.SetStatus(INNERSTATUS_POISON);
	siPoisonCharUnique	= poisoner;						// ���� �ߵ���Ų ĳ���� ����ũ. 
	dwPoisonLastClock	= _dwClock;						// �ߵ��� �����ð�. 
	dwPoisonStartClock	= pclClient->CurrentClock;		// �ߵ� ���۽ð�. 
	siPoisonPower		= damage;						// �ߵ��� ����. 
	dwPoisonCheckClock  = pclClient->CurrentClock;		// ���� �ð�

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_POISON, true);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

//��Ʈ �������� ȿ���� �ߵ��� ��Ų��. 
BOOL cltCharServer::SetPoisonSetItemEffect(SI32 poisoner, SI32 damage, DWORD _dwClock )
{
	// POISON������ �������� damage�� �ݸ� ����.
	damage /= 2;

	clInnerStatus.SetStatus( INNERSTATUS_POISON_SETITEM_EFFECT );
	siPoisonCharUnique	= poisoner;						// ���� �ߵ���Ų ĳ���� ����ũ. 
	dwPoisonLastClock	= _dwClock;						// �ߵ��� �����ð�. 
	dwPoisonStartClock	= pclClient->CurrentClock;		// �ߵ� ���۽ð�. 
	siPoisonPower		= damage;						// �ߵ��� ����. 
	dwPoisonCheckClock  = pclClient->CurrentClock;		// ���� �ð�

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_POISON_SETITEM_EFFECT, true );
	cltGameMsgResponse_StatusInfo clChangeInfo( GetCharUnique(), &StatusInfo );
	cltMsg clMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo );
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::SetDrain(SI32 damage, SI32 passivedamage, SI32 passivemana)
{
	//======================================
	// �ִ� 30% �� ���� �ʵ��� �Ѵ�.
	//======================================
	if(passivedamage>30) passivedamage = 30;

	SI32 IncLife = min(damage * passivedamage /100 ,  clPB.GetMaxLife() - GetLife());
	SI32 DecMana = passivemana;
	// mana�� �����̶� ���� �ִٸ� ���ϴ� mana ���� �۴��� �� ��ʴ�� �������� ä���ش�.
	if(passivemana && GetMana() &&  GetMana()< passivemana)
	{
		IncLife  = (IncLife * GetMana())/DecMana;
		DecMana  = GetMana();
	}

	// �������� ä��� �Ǹ� �������� ä��� ������ ��´�.
	if(IncLife>0 && GetMana() >= DecMana)
	{
		bDrainSwitch = true;		// ���ǹ̴� ���� 
		IncreaseLife(IncLife);
		DecreaseMana(DecMana);

		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_DRAIN, true, GetLife(), GetMana(), IncLife);
		cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		SendNeighbourMsg(&clMsg, true);
		return TRUE;
	}

	return FALSE;
}


BOOL cltCharServer::SetCurseAttack(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid)
{
	if( pclCM->IsValidID(enemyid) == FALSE)		return FALSE;
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];

	if(pclenemychar->clInnerStatus.IsStatus(INNERSTATUS_CURSEATTACK) == true) return FALSE;

	SI32 DecMana = passivemana;
	// mana�� �����̶� ���� �ִٸ� ���ϴ� mana ���� �۴��� �� ��ʴ��
	if(passivemana && GetMana() &&  GetMana()< passivemana)
	{
		passivedamage	= (passivedamage * GetMana())/DecMana;
		DecMana			= GetMana();
	}

	// ������ ��´�.
	if(passivedamage> 0 && GetMana() >= DecMana)
	{
		// �ִ� 30% �� ���� �ʵ��� �Ѵ�.
		if(passivedamage>30)	passivedamage = 30;

		pclenemychar->clInnerStatus.SetStatus(INNERSTATUS_CURSEATTACK);
		pclenemychar->dwCurseAttackLastClock	= 5000+min(15000, damage*30);	// ���ݷ� ������ �����ð�. 
		pclenemychar->dwCurseAttackStartClock	= pclClient->CurrentClock;		// ���ݷ� ����   ���۽ð�. 
		pclenemychar->siCurseAttackPower		= passivedamage;				// ���ݷ� ������ ����. 

		DecreaseMana(DecMana);


		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEATTACK, true, GetMana());
		cltGameMsgResponse_StatusInfo clChangeInfo(pclenemychar->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		pclenemychar->SendNeighbourMsg(&clMsg, true);
		return TRUE;
	}
	return FALSE;
}


BOOL cltCharServer::SetCurseCritical(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid)
{
	if( pclCM->IsValidID(enemyid) == FALSE)		return FALSE;
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];

	if(pclenemychar->clInnerStatus.IsStatus(INNERSTATUS_CURSECRITICAL) == true) return FALSE;

	SI32 DecMana = passivemana;
	// mana�� �����̶� ���� �ִٸ� ���ϴ� mana ���� �۴��� �� ��ʴ��
	if(passivemana && GetMana() &&  GetMana()< passivemana)
	{
		passivedamage	= (passivedamage * GetMana())/DecMana;
		DecMana			= GetMana();
	}

	// ������ ��´�.
	if(passivedamage> 0 && GetMana() >= DecMana)
	{
		// �ִ� 80% �� ���� �ʵ��� �Ѵ�.
		if(passivedamage>80)	passivedamage = 80;

		pclenemychar->clInnerStatus.SetStatus(INNERSTATUS_CURSECRITICAL);
		pclenemychar->dwCurseCriticalLastClock	= 5000+min(15000, damage*30);	// ũ��Ƽ�� ������ �����ð�. 
		pclenemychar->dwCurseCriticalStartClock	= pclClient->CurrentClock;		// ũ��Ƽ�� ����   ���۽ð�. 
		pclenemychar->siCurseCriticalPower		= passivedamage;				// ũ��Ƽ�� ������ ����. 

		DecreaseMana(DecMana);

		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSECRITICAL, true, GetMana());
		cltGameMsgResponse_StatusInfo clChangeInfo(pclenemychar->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		pclenemychar->SendNeighbourMsg(&clMsg, true);
		return TRUE;
	}

	return FALSE;

}


BOOL cltCharServer::SetCurseDefense(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid)
{
	if( pclCM->IsValidID(enemyid) == FALSE)		return FALSE;
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];

	if(pclenemychar->clInnerStatus.IsStatus(INNERSTATUS_CURSEDEFENSE) == true) return FALSE;

	SI32 DecMana = passivemana;
	// mana�� �����̶� ���� �ִٸ� ���ϴ� mana ���� �۴��� �� ��ʴ��
	if(passivemana && GetMana() &&  GetMana()< passivemana)
	{
		passivedamage	= (passivedamage * GetMana())/DecMana;
		DecMana			= GetMana();
	}

	// ������ ��´�.
	if(passivedamage> 0 && GetMana() >= DecMana)
	{
		// �ִ� 30% �� ���� �ʵ��� �Ѵ�.
		if(passivedamage>30)	passivedamage = 30;

		pclenemychar->clInnerStatus.SetStatus(INNERSTATUS_CURSEDEFENSE);
		pclenemychar->dwCurseDefenseLastClock	= 5000+min(15000, damage*30);	// ���ݷ� ������ �����ð�. 
		pclenemychar->dwCurseDefenseStartClock	= pclClient->CurrentClock;		// ���ݷ� ����   ���۽ð�. 
		pclenemychar->siCurseDefensePower		= passivedamage;				// ���ݷ� ������ ����. 

		DecreaseMana(DecMana);

		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEDEFENSE, true, GetMana());
		cltGameMsgResponse_StatusInfo clChangeInfo(pclenemychar->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		pclenemychar->SendNeighbourMsg(&clMsg, true);
		return TRUE;
	}
	return FALSE;
}


BOOL cltCharServer::SetCurseMovespeed(SI32 damage, SI32 passivedamage, SI32 passivemana, SI32 enemyid)
{
	if( pclCM->IsValidID(enemyid) == FALSE)		return FALSE;
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];

	if(pclenemychar->clInnerStatus.IsStatus(INNERSTATUS_CURSEMOVESPEED) == true) return FALSE;

	SI32 DecMana = passivemana;
	// mana�� �����̶� ���� �ִٸ� ���ϴ� mana ���� �۴��� �� ��ʴ��
	if(passivemana && GetMana() &&  GetMana()< passivemana)
	{
		passivedamage	= (passivedamage * GetMana())/DecMana;
		DecMana			= GetMana();
	}

	// ������ ��´�.
	if(passivedamage> 0 && GetMana() >= DecMana)
	{
		// �ִ� 50% �� ���� �ʵ��� �Ѵ�.
		if(passivedamage>50)	passivedamage = 50;

		pclenemychar->clInnerStatus.SetStatus(INNERSTATUS_CURSEMOVESPEED);
		pclenemychar->dwCurseMovespeedLastClock	= 5000+min(15000, damage*30);	// �̵��ӵ� ���� �����ð�. 
		pclenemychar->dwCurseMovespeedStartClock= pclClient->CurrentClock;		// �̵��ӵ� ����   ���۽ð�. 
		pclenemychar->siCurseMovespeedPower		= passivedamage;				// �̵��ӵ� ������ ����. 

		DecreaseMana(DecMana);

		SI32 siCurrentSpeed = pclenemychar->clPB.TotalMoveSpeed;
		pclenemychar->clPB.GivenMoveSpeed = max(0, siCurrentSpeed - siCurrentSpeed * passivedamage / 100);
		pclenemychar->clPB.TotalMoveSpeed = pclenemychar->clPB.GivenMoveSpeed;

		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEMOVESPEED, true, GetMana(), pclenemychar->clPB.GivenMoveSpeed);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclenemychar->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		pclenemychar->SendNeighbourMsg(&clMsg, true);
		return TRUE;
	}
	return FALSE;
}

BOOL cltCharServer::DeletePoison()				// �ߵ� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_POISON);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_POISON, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeletePoisonSetItemEffect()				// �ߵ� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus( INNERSTATUS_POISON_SETITEM_EFFECT) ;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_POISON_SETITEM_EFFECT, false );
	cltGameMsgResponse_StatusInfo clChangeInfo( GetCharUnique(), &StatusInfo );
	cltMsg clMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, PACKET(clChangeInfo) );
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteIce()					// ���� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_ICE);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_ICE, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteCurseAttack()			// ���ݷ��� ���� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_CURSEATTACK);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEATTACK, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteCurseCritical()		// Critical Ȯ���� �������� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_CURSECRITICAL);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSECRITICAL, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteCurseDefense()		// ������ �������� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_CURSEDEFENSE);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEDEFENSE, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteCurseMovespeed()		// �̵��ӵ� ���� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_CURSEMOVESPEED);

	clPB.GivenMoveSpeed					= 0;

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_CURSEMOVESPEED, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteMagicFire()			// �ҿ� Ÿ�� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_MAGICFIRE);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_MAGICFIRE, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

BOOL cltCharServer::DeleteFrozen()				// ���� ���� ���¸� �����Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_FROZEN);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FROZEN, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}
BOOL cltCharServer::DeleteStun()				// ���� ���¸� ���� �Ѵ�.
{
	clInnerStatus.DelStatus(INNERSTATUS_STUN);

	// �̿����� �뺸 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_STUN, false);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNeighbourMsg(&clMsg, true);

	return TRUE;
}

//[����] ����� ���� ����.
BOOL cltCharServer::DeleteCurseEffect()				
{
	m_clCharCurseEffect.Clear();
	m_clCharCurseEffect.SendMsgCurseInfo( GetID() );
	
	return TRUE;
}


//ĳ���Ͱ� ���� �޾��� �� ó���ؾ��� �ϵ�. 
void cltCharServer::BeHitOperate(SI32 attackid)
{
	if(pclCM->IsValidID(attackid) == false)			return;

	cltServer* pclserver			= (cltServer*)pclClient;
	cltCharServer* pclattackchar	= (cltCharServer*)pclCM->CR[attackid];

	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. - ����ü Ÿ��.
	// ĳ���Ͱ� ���� �޾�����, ����ü�� �������� ���� �����ش�. 
	if ( IDENTITY_PERSON_SUMMON == clIdentity.siIdentity )
	{
		DecreaseCrystalDurDEForSummon(CRYSTAL_DECREASE_DURABILITY_ACNUM);
	}
	else
	{
		DecreaseCrystalDurDE(CRYSTAL_DECREASE_DURABILITY_ACNUM);
	}

	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		if( pclCM->IsAlive(GetID()) )
		{
			cltCharServer* pclenemychar = pclCM->GetCharServer( attackid );

			bool bDecreaseDur = false;		// �������� ���� ���Ѿ� �ϴ°�?

			// ���� ĳ���Ͱ� ������ ��쿡�� �������� �����ȴ�.
			if( pclenemychar &&
				IsNPC(pclenemychar->GetCharUnique()) &&
				pclenemychar->clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )
			{
				bDecreaseDur = true;
			}

			if( bDecreaseDur )
			{
				DecreaseArmourDur( attackid );
			}
		}
		else
		{
			DecreaseEquipDurFromDead( );
		}
	}

	//-----------------------------------------
	// ��Ȳ���̳� �����̸� ������ ���ݹް� ������ �����Ѵ�. 
	//-----------------------------------------
	switch(clIdentity.siIdentity)			// ��Ȳ��
	{
	case IDENTITY_CASTLE:
	case IDENTITY_VILLAGEGOD:
		{
			SI32 villageunique = pclCI->clBI.siHomeVillage;
			if( villageunique >= 0 && villageunique < MAX_VILLAGE_NUMBER &&
				pclserver->pclVillageManager->pclVillageInfo[villageunique] )
			{
				pclserver->pclVillageManager->pclVillageInfo[villageunique]->bAttackedVillage = true;

				//--------------------------------------------------
				//10 ���߿� 3���� ���͸� ������Ų��.
				//--------------------------------------------------
				if(rand()%10 <3)
				{
					SI32 villagecharunique = 0;
					if(pclserver->pclVillageManager->IsThereVillageGod(villageunique, &villagecharunique) == TRUE) 
					{
						SI32 siMapIndex   = pclCM->CR[attackid]->GetMapIndex();
						SI32 siSummonKind = pclCM->CR[villagecharunique]->GetKind();
						SI32 siSummonX    = pclCM->CR[attackid]->GetX();
						SI32 siSummonY    = pclCM->CR[attackid]->GetY(); 

						if(siSummonKind > 0)
						{
							cltOrderDataInfo Order;
							pclClient->pclOrderManager->MakeOrder_ATTACK(&Order, attackid, BY_PLAYER);

							cltIdentity clIdentity;
							clIdentity.MakeGod(IDENTITY_VILLAGEGOD_SUMMON, 15);

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_VILLAGE, 0);

							SI32 siSummonChar = pclserver->SetNPCChar(siSummonKind, villageunique, &clIdentity, 1, 0, siSummonX, siSummonY, siMapIndex, NPCITEMMODE_NONE, &clinnerstatus, &Order);

							if(pclCM->IsValidID(siSummonChar))
							{
								pclCM->CR[ siSummonChar ]->ParametaBoxAction();

								// MAXLIFE, MANA�� ����Ͽ� ü�°� �������� �������ش�. 
								pclCM->CR[ siSummonChar ]->SetLife(pclCM->CR[ siSummonChar ]->clPB.GetMaxLife());
								pclCM->CR[ siSummonChar ]->SetMana(pclCM->CR[ siSummonChar ]->clPB.GetMaxMana());

								// ��ȯ���� �����ð��� ���� ������ �ð��� �����Ѵ�.
								((cltCharServer*)(pclCM->CR[ siSummonChar ]))->dwTimerClock = pclserver->CurrentClock;
							}
						}
					}
				}
			}
		}
		break;
	case IDENTITY_VILLAGEHUNT: // Ű�޶���
		{
			pclserver->pclEventHunting->AddContributor(attackid);
		}
		break;
	default:	break;
	}


	if(pclCM->IsAlive(GetID()))
	{
		// ���� �������� ������ ���Ͷ��, 
		if(pclKI->clBossItemInfo.siItemIndex)
		{
			// ������ �������� ã�´�. 

			SI32 itemunique, itemnum;
			if(pclKI->clBossItemInfo.GetRandItem(&itemunique, &itemnum) == true)
			{
				if(itemunique)
				{
					pclClient->pclItemManager->AddWarItemNum(itemunique, itemnum);

					// �����ڰ� �����̶�� ������ ���̵� ��� ����Ѵ�. 
					SI32 killerid = attackid;

					if(pclCM->IsValidID(killerid))
					{
						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[killerid];

						//cyj pclchar->GiveItem(itemunique, itemnum, INPUTITEMPERSON_OPTION_WAR,0);
						SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
						if(ref > 0)  //[����]FindItemRefFromUnique ����ó��
						{
							if(pclClient->pclItemManager->pclItemInfo[ref] != NULL)
							{
								UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
								pclchar->GiveItem(itemunique, itemnum, INPUTITEMPERSON_OPTION_WAR, usedate);
							}
						}
					}
				}
			}
		}

		// ����� ���, 
		if(GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_DRAGONKING")) )
		{
			if(rand() % 1 == 0)
			{
				cltOrderDataInfo Order;
				pclClient->pclOrderManager->MakeOrder_ATTACK(&Order, attackid, BY_PLAYER);

				SI32 siMapIndex   = GetMapIndex();
				SI32 siSummonX    = GetX();
				SI32 siSummonY    = GetY(); 

				SI32 quardid = pclserver->MakeGuardChar(pclClient->GetUniqueFromHash(TEXT("KIND_BOMBDRAGON")), pclCI->clBI.siHomeVillage, siMapIndex, siSummonX, siSummonY, &Order, &clInnerStatus.clEnemyType);
				if(pclCM->IsValidID(quardid))
				{
					// ��ȯ���� �����ð��� ���� ������ �ð��� �����Ѵ�.
					((cltCharServer*)(pclCM->CR[ quardid ]))->dwTimerClock = pclClient->CurrentClock;
				}
			}
		}


		SI16 questid = 0;
		if( IsPC(pclattackchar->GetCharUnique()) )
		{
			questid = attackid;
		}

		if( pclCM->IsValidID(questid) )
		{
			SI32 enemykind = GetKind();
			SI32 killednum = 1;

			// ���͸� ������ ����Ʈ�� �������̾��� ?
			((cltCharServer*)pclCM->CR[questid])->IncQuestPara(QUEST_CLEAR_COND_HITMON, enemykind, killednum);
		}
	}
	else
	{
		//-------------------------------------------------------
		// Daily ����Ʈ �������̾��ٸ� ���� ��ġ�� ���Ѵ�. (CLONE ĳ���ʹ� COUNT�� ���� �ʴ´�.)
		//-------------------------------------------------------
		SI16 questid = 0;
		if ( clInnerStatus.IsStatus(INNERSTATUS_CLONE) == false && pclCM->IsAlive( attackid ) == TRUE )
		{
			SI32 enemykind = GetKind();

			if( IsPC(pclattackchar->GetCharUnique()) )
			{
				questid = attackid;
			}

			// ��ȯ�� �̸�, �θ� ����Ʈ ����ڷ� ����. 
			if(IsNPC(pclattackchar->GetCharUnique()) && pclattackchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				questid = pclattackchar->GetSummonParentID();
			}

			if ( pclCM->IsValidID( questid ) )
			{
				cltCharServer* pclquestchar = (cltCharServer*)pclCM->CR[questid];

				if( pclquestchar && pclquestchar->pclCI->clDailyQuestInfo.siMonsterKind == enemykind )
				{
					if ( 0 < pclquestchar->siQuestCount )	// ��� ���ɾ� ī��Ʈ ����
					{
						pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum += pclquestchar->siQuestCount;
					}
					else										// �Ϲ� ��� 
					{
						++pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum;
					}

					if ( pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum > 30000 )
					{
						pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum = 30000;
					}

					if ( pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum >= pclquestchar->pclCI->clDailyQuestInfo.siMonsterNum )
					{
						SI32 value = 100;

						cltItem clItem;
						clItem.siUnique = pclquestchar->pclCI->clDailyQuestInfo.siRewardItemUnique;
						clItem.siItemNum = ( pclquestchar->pclCI->clDailyQuestInfo.siRewardItemNum * value ) / 100;

						cltItem clToItem;
						clToItem.Init();

						SI16 InvPos = -1;
						SI32 rtnvalue;

						GMONEY money = ( pclquestchar->pclCI->clDailyQuestInfo.siRewardMoney * value ) / 100;
						BOOL selectedmoney = pclquestchar->pclCI->clDailyQuestInfo.bMoneySelected;

						if ( pclquestchar->pclCI->clDailyQuestInfo.bMoneySelected )
						{
							sDBRequest_DailyQuestDel clMsg( pclquestchar->pclCI->GetPersonID(), questid, true, InvPos, &clItem, 1, money, selectedmoney, pclquestchar->pclCI->clDailyQuestInfo.siRewardExp );
							pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

							pclserver->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DAILYQUEST, money);
						}
						else
						{
							if ( pclquestchar->CanAddInv(INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
							{
								sDBRequest_DailyQuestDel clMsg( pclquestchar->pclCI->GetPersonID(), questid, true, InvPos, &clItem, 1, money, selectedmoney, pclquestchar->pclCI->clDailyQuestInfo.siRewardExp );
								pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
							}
							else
							{
								// �κ� �� ���� �ȵǹǷ�...
								pclserver->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  1, 0, pclquestchar->GetCharUnique());

								// �������� ���� ���� ����
								cltGameMsgResponse_DailyQuestChange clinfo( pclquestchar->pclCI->clDailyQuestInfo.siLeftTime, pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum );
								cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUESTCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
								pclquestchar->SendNetMsg((sPacketHeader*)&clMsg);					
							}
						}
					}
					else
					{
						// DB �� ������Ʈ ������ �ʰ�, �������� �ٷ� �˷���.
						cltGameMsgResponse_DailyQuestChange clinfo( pclquestchar->pclCI->clDailyQuestInfo.siLeftTime, pclquestchar->pclCI->clDailyQuestInfo.siKillMonsterNum );
						cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUESTCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
						pclquestchar->SendNetMsg((sPacketHeader*)&clMsg);					
					}
				}
			}
		}

		//-------------------------------------------------------
		// Daily2 ����Ʈ �������̾��ٸ� ���� ��ġ�� ���Ѵ�. (CLONE ĳ���ʹ� COUNT�� ���� �ʴ´�.)
		//-------------------------------------------------------
		if ( clInnerStatus.IsStatus(INNERSTATUS_CLONE) == false && pclCM->IsAlive( attackid ) == TRUE )
		{
			SI32 enemykind = GetKind();

			if( IsPC(pclattackchar->GetCharUnique()) )
			{
				questid = attackid;
			}

			// ��ȯ�� �̸�, �θ� ����Ʈ ����ڷ� ����. 
			if(IsNPC(pclattackchar->GetCharUnique()) && pclattackchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				questid = pclattackchar->GetSummonParentID();
			}

			if ( pclCM->IsValidID( questid ) )
			{
				cltCharServer* pclquestchar = (cltCharServer*)pclCM->CR[questid];

				if( pclquestchar && pclquestchar->pclCI->clDailyQuest2Info.siMonsterKind == enemykind )
				{
					if ( 0 < pclquestchar->siQuestCount )	// ��� ���ɾ� ī��Ʈ ����
					{
						pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum += pclquestchar->siQuestCount;
					}
					else										// �Ϲ� ��� 
					{
						++pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum;
					}

					if ( pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum > 30000 )
					{
						pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum = 30000;
					}

					if ( pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum >= pclquestchar->pclCI->clDailyQuest2Info.siMonsterNum )
					{
						SI32 value = 100;

						cltItem clItem;
						clItem.siUnique = pclquestchar->pclCI->clDailyQuest2Info.siRewardItemUnique;
						clItem.siItemNum = ( pclquestchar->pclCI->clDailyQuest2Info.siRewardItemNum * value ) / 100;

						cltItem clToItem;
						clToItem.Init();

						SI16 InvPos = -1;
						SI32 rtnvalue;

						GMONEY money = ( pclquestchar->pclCI->clDailyQuest2Info.siRewardMoney * value ) / 100;
						BOOL selectedmoney = pclquestchar->pclCI->clDailyQuest2Info.bMoneySelected;

						if ( pclquestchar->pclCI->clDailyQuest2Info.bMoneySelected )
						{

							sDBRequest_DailyQuest2Del clMsg( pclquestchar->pclCI->GetPersonID(), questid, true, InvPos, &clItem, 1, money, selectedmoney, pclquestchar->pclCI->clDailyQuest2Info.bHiddenQuest );
							pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
						}
						else
						{
							if ( pclquestchar->CanAddInv(INVMODE_ALL, &clItem, &InvPos, &clToItem, &rtnvalue) )
							{
								sDBRequest_DailyQuest2Del clMsg( pclquestchar->pclCI->GetPersonID(), questid, true, InvPos, &clItem, 1, money, selectedmoney, pclquestchar->pclCI->clDailyQuest2Info.bHiddenQuest );
								pclserver->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
							}
							else
							{
								// �κ� �� ���� �ȵǹǷ�...
								pclserver->SendServerResponseMsg(0, SRVAL_QUEST_FAIL_FULLINV,  1, 0, pclquestchar->GetCharUnique());

								// ���� ������Ʈ
								cltGameMsgResponse_DailyQuest2Change clinfo( pclquestchar->pclCI->clDailyQuest2Info.siLeftTime, pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.bHiddenQuest );
								cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST2CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
								pclquestchar->SendNetMsg((sPacketHeader*)&clMsg);					
							}
						}
					}
					else
					{
						// DB �� ������Ʈ ������ �ʰ�, �������� �ٷ� �˷���.
						cltGameMsgResponse_DailyQuest2Change clinfo( pclquestchar->pclCI->clDailyQuest2Info.siLeftTime, pclquestchar->pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.bHiddenQuest );
						cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST2CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
						pclquestchar->SendNetMsg((sPacketHeader*)&clMsg);					
					}

				}
			}
		}

		//-------------------------------------------------------
		// ���� óġ ����Ʈ �������̾��ٸ� ���� ��ġ�� ���Ѵ�. ( CLONE�� �ش����� �ʴ´�.)
		//-------------------------------------------------------
		if(clInnerStatus.IsStatus(INNERSTATUS_CLONE) == false && pclCM->IsAlive(attackid) == TRUE)
		{
			if( IsPC(pclattackchar->GetCharUnique()) )
			{
				questid = attackid;
			}

			// ��ȯ�� �̸�, �θ� ����Ʈ ����ڷ� ����. 
			if(IsNPC(pclattackchar->GetCharUnique()) && pclattackchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				questid = pclattackchar->GetSummonParentID();

				// ��ȯ���� ���͸� óġ�ϴ� ����Ʈ�� �������̾��� ?
				if(pclCM->IsValidID(questid))
				{
					((cltCharServer*)pclCM->CR[questid])->IncQuestPara(QUEST_CLEAR_COND_SUMMONBATTLE , GetKind(), 1);
				}
			}
		}

		if(questid)
		{
			SI32 enemykind		= GetKind();
			SI32 enemyMapIndex	= GetMapIndex();
			SI32 killednum		= 1;
			SI16 siTemp1		= 0, siTemp2 = 0, siTemp3 = 0, siTemp4 = 0;

			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[questid];
			if ( pclchar == NULL ) return;

			// ��� ���ɾ� ����
			if( 0 < pclchar->siQuestCount )
			{
				killednum = pclchar->siQuestCount;
			}

			//KHY - 1113 - ũ�������� �̺�Ʈ - ũ�������� ����Ʈ�� Ư���ϴ�.
			if( pclserver->pclEventTimeManager->InEventTime("ChristmasEvent", &pclserver->sTime) )
			{
				ChristamsEventQuest(&enemykind);
			}

			// ���͸� óġ�ϴ� ����Ʈ�� �������̾��� ?
			pclchar->IncQuestPara(QUEST_CLEAR_COND_KILLMON, enemykind, killednum);

			// Ư�� �ʿ� �ִ� ���͸� óġ�ض�
			if ( IsNPC(GetCharUnique()) ) 
			{
				pclchar->IncQuestPara(QUEST_CLEAR_COND_MAPKILLMON, enemyMapIndex, killednum);
			}
			
			// ���� ������ �ƹ��� para2��ŭ �׿����ϴ� ����Ʈ���
			if ( pclchar->pclCI->clQuestInfo.IsQuestGoing( QUEST_CLEAR_COND_KILLDARKSOLDIER, &siTemp1, &siTemp2, &siTemp3, &siTemp4 ) == true )
			{
				TCHAR tszBlackArmyName[][64] = {
					// ���Ǳ��� �̺�Ʈ ����
					"KIND_NEWRIFLE", "KIND_NEWCATAR", "KIND_NEWDAEGUM", "KIND_YELLOWSHABEL", "KIND_GREENSHABEL", "KIND_REDSHABEL", "KIND_BLUESHABEL"
						// ���� �ȳ������� �ߵ��ϴ� ���� ���ܿ� ������ ����
						,"KIND_DAEGUM", "KIND_CATAR", "KIND_RIFLE", "KIND_BLACKBOSS"
				};
				SI32 siCount = sizeof(tszBlackArmyName)/sizeof(TCHAR)/64;


				for ( SI32 siIndex=0; siIndex<siCount; siIndex++ )
				{
					if ( enemykind == pclserver->GetUniqueFromHash(tszBlackArmyName[siIndex]) )
					{

						pclchar->IncQuestPara(QUEST_CLEAR_COND_KILLDARKSOLDIER, 1, killednum);
						break;
					}
				}
			}

			// [����] ����� ��Ƽ�� �ξ������� �۵����� �ʵ��� ���´�
			if ( pclchar->siPartyIndex <= 0 )
			{
				pclchar->IncQuestPara(QUEST_CLEAR_COND_TWOWAYKILLMON , enemykind, killednum);
				pclchar->IncQuestPara(QUEST_CLEAR_COND_TWOWAYKILLMONBONUS , enemykind, killednum);
			}

			// [����] ���� �̺�Ʈ ����Ʈ
			if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest = (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
				if ( pclSE_Quest )
				{
					SI32 siEnemyLevel = pclCI->clIP.GetLevel();

					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_HUNTKIND, enemykind, killednum );
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_HUNTLEVEL, siEnemyLevel, killednum );
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_HUNTBOSS, enemykind, killednum );
				}
			}

			// ��Ƽ����Ʈ
			if(pclserver->IsCountrySwitch(Switch_PartyQuest_PartyMatching))
			{
				// ��Ƽ�� ���� �÷��̾ ��Ƽ����Ʈ ������ ���� �մϴ�.
				if(pclchar->siPartyIndex > 0)
				{
					pclserver->m_pPartyQuestMgr->UpdatePartyQuest(pclchar->GetID() ,pclchar->siPartyIndex, PARTYQUEST_TYPE_HUNTING, enemykind, killednum);
				}
			}
		}
	}
}

SI32 cltCharServer::DecreaseLife(SI32 life)
{
	cltServer* pclserver = (cltServer*)pclClient;

	pclCI->clIP.DecreaseLife(max(0, life));

	if(GetLife() < 0 )
	{
		pclCI->clIP.SetLife(0);
	}

	// �����. 
	if(GetLife() <= 0)
	{
		//-------------------------------
		// ���� ĳ������ ������ �����Ѵ�. 
		//-------------------------------
		pclserver->pclKindInfoSet->IncreasePreferenceChar(GetKind(), (pclClient->GetFrame() - dwMakeFrame) * 50 );

		//-----------------------------
		// ����ڰ� ���� ���̶�� DB�� �⺻ ������ �����Ѵ�. 
		//-----------------------------
		if(IsPC(GetCharUnique()))
		{
			// ������ Ȯ���Ѵ�. 
			if(GetMapIndex() == 0)
			{
				pclCI->clBI.clLastMainMapPos.Set(GetX(), GetY());
			}

			SendDB_PosLifeMana();

			//----------------------------------
			// �ʺ��ڿ� ����Ϳ��� ����ߴٸ�, 
			//----------------------------------
			if(GetMapIndex() == MAPINDEX_BEGINNER1 )
			{
				siKilledMode = KILLEDMODE_BEGINNERMAP;
				StringCchCopy(szKillMeCharName, MAX_PLAYER_NAME, TEXT(""));

			}

		}

	}

	return GetLife();

}

// �̿����� �� �ܾ� ������ �뺸�Ѵ�. 
bool cltCharServer::SendNeighbourMyShapeStatus()
{
	SI32 horseunique	= 0;
	cltDate addstatus;
	addstatus.MakeFullDateFromDateVary(0);
	SI32 ticket = 0 ;

	cltDate PremiumpartsDate_Slot1;
	PremiumpartsDate_Slot1.MakeFullDateFromDateVary(0);
	SI32 siPremiumPartsUnique_Slot1 = 0 ;

	cltDate PremiumpartsDate_Slot2;
	PremiumpartsDate_Slot2.MakeFullDateFromDateVary(0);
	SI32 siPremiumPartsUnique_Slot2 = 0 ;

	cltDate PremiumpartsDate_Slot3;
	PremiumpartsDate_Slot3.MakeFullDateFromDateVary(0);
	SI32 siPremiumPartsUnique_Slot3 = 0 ;

	SI32 siPremiumPartsNum = 0 ;
	//SI32 horseindex		= pclCI->clHorseInfo.siCurrentHorseIndex;
	//if(horseindex	 >= 0)
	//KHY - 1001 - ��µ��� 5���� ���� ����.
	if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true)
	{
		cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
		if(pclHorse != NULL)
		{
			horseunique = pclHorse->siHorseUnique;
			addstatus.Set(&pclHorse->clAddStatusDate);
			ticket = pclHorse->siStatusTicketNum ;
		}
	}
	cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
	{
		if(pclHorse != NULL)
		{
			//�����̾� ���� 
			PremiumpartsDate_Slot1.Set(&pclHorse->clPremiumPartsDate_Slot1);
			siPremiumPartsUnique_Slot1 = pclHorse->siPremiumUnique_Slot1 ;
			PremiumpartsDate_Slot2.Set(&pclHorse->clPremiumPartsDate_Slot2);
			siPremiumPartsUnique_Slot2 = pclHorse->siPremiumUnique_Slot2 ;
			PremiumpartsDate_Slot3.Set(&pclHorse->clPremiumPartsDate_Slot3);
			siPremiumPartsUnique_Slot3 = pclHorse->siPremiumUnique_Slot3 ;

			siPremiumPartsNum = pclHorse->siPremiumPartsNum ;
		}
	}
	// �ܺ� ���. 
	cltShapeEquipInfo clequip;
	if(IsPC(GetCharUnique()))
	{
		GetShapeEquipInfo(&clequip);
	}

	cltGameMsgResponse_PersonAllShapeInfo clInfo(GetCharUnique(), &clequip, horseunique,&addstatus,ticket);

	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONALLSHAPEINFO, sizeof(clInfo), (BYTE*)&clInfo);
	SendNeighbourMsg(&clMsg, false);

	// ������� ���� ����������
	if ( (PERSONITEM_WEAPON1 == ActiveWeapon) || (PERSONITEM_WEAPON2 == ActiveWeapon) )
	{
		SI32 siItemType		= pclCI->clCharItem.GetWeaponType( pclClient->pclItemManager, ActiveWeapon );
		SI32 siSkillUnique	= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		UI08 siGetEmblem	= 0;
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel	= pclCI->clCharSkill.GetSkillExpLevel( siSkillUnique );
			siGetEmblem	= pclClient->pclSkillManager->m_clMasterManager.GetEmblemIndex( siItemType, siSkillLevel );
		}

		if ( m_uiWeaponSkillMasterEmblem != siGetEmblem )
		{
			m_uiWeaponSkillMasterEmblem = siGetEmblem;

			cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_EMBLEM, false, siGetEmblem );
			cltGameMsgResponse_StatusInfo	clInfo( GetCharUnique(), &clChangeInfo );

			cltMsg clEmblenMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo);
			SendNeighbourMsg( &clEmblenMsg, true );
		}

	}

	return true;
}


bool cltCharServer::SendNeighbourMyMakeItemStatus()
{

	SI08 sitempMakeItemSwitch = siPersonalMakeItemType;

	cltGameMsgResponse_PersonMakeItemSwitch clInfo(GetCharUnique(), sitempMakeItemSwitch);

	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONMAKEITEMSWITCH, sizeof(clInfo), (BYTE*)&clInfo);
	SendNeighbourMsg(&clMsg, true);

	return true;
}

// �������� ĳ������ ���� �̵� ����.
void cltCharServer::CharServerMoveByGate(SI32 x, SI32 y)
{			
	if(clInnerStatus.clEnemyType.IsType(ENEMYTYPE_VILLAGE) == false)return ;

	// �ʵ忡 FieldObject�� �����ϴ��� Ȯ���Ѵ�.
	SI32 unique = pclMap->GetFieldObject_Unique(x, y);
	if(unique <= 0)return ;

	SI32 ref	= pclMap->pclFieldObjectManager->FindRefFromUniqueMinus(unique);
	if(ref < 0)return ;

	SI32 type	= pclMap->pclFieldObjectManager->pclInfo[ref]->dwType;

	if(type == FOTYPE_VILLAGE)
	{
		// ������ Unique������ ���´�. 
		SI32 villageunique = pclMap->GetFieldObjectPara(x, y);

		if(villageunique > 0 && villageunique < MAX_VILLAGE_NUMBER)
		{
			//  �����ϰ��� �ϴ� ������ ������ �������� �����ΰ� ?
			SI32 castleid = pclClient->pclVillageManager->pclVillageInfo[villageunique]->siCastleCharUnique;
			if(castleid && IsEnemy(castleid) == true)
			{
				// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
				((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_WAR_CANTENTERENEMYCASTLE,  0, 0, GetCharUnique());
			}
			// ������ ���� �ִٸ�, 
			else if(clInnerStatus.IsStatus(INNERSTATUS_PRISON))
			{
			}
			else
			{

				// ���� ������ ������ Ȯ���Ѵ�. 
				//if(pclClient->pclMapManager->GetLevelLimit(villageunique) <= pclCI->clIP.GetLevel())
				if(pclClient->pclMapManager->IsEnterLevel(villageunique, pclCI->clIP.GetLevel()) == true)
				{
					SI32 mapindex	= pclClient->pclVillageManager->pclVillageInfo[villageunique]->siVillageUnique;

					SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[ mapindex ]->siMapUnique;
					SI32 centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
					SI32 centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

					cltWarpInfo clWarpInfo(1, mapindex, false, false, centerx, centery, x, y);
					SetWarpInfo(&clWarpInfo, 30);

					// �����¿��� pvp�� ���� pvp-
					clInnerStatus.clEnemyType.siEnemyType = ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR;
				}
			}

		}
		else
		{
			MsgBox(TEXT("fdsj8jfd"), TEXT("fdsn83f:%d"), villageunique);
		}
	}
	else if(type == FOTYPE_GATE)
	{

		SI32 gateunique = pclMap->GetFieldObjectPara(x, y);
		if(gateunique <= 0)return ;

		cltGateManager* pclgatemanager = pclClient->pclMapManager->pclGateManager;
		cltInstanceMapGateManager* pclinstancemapgatemanager = pclClient->pclMapManager->pclInstanceMapGateManager;
		cltServer* pclserver = (cltServer*)pclClient;

		bool bwarpfailswitch = false;
		SI32 siSourceMapIndex = GetMapIndex();

		// ���� ������ ������ Ȯ���Ѵ�. 
		SI32 mapindex = 0;
		SI32 dx = 0;
		SI32 dy = 0;

		SI32 itemunique = 0;
		SI32 itemnum = 0;


		// �δ��̸�, �δ� ���� ����Ʈ���� ã�´�.
		if( pclClient->pclMapManager->IsInstanceMap( GetMapIndex() ) )
		{
			SI32 siParentMapIndex = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
			cltMapCommon* pclParentMap = pclClient->pclMapManager->GetMapPointer( siParentMapIndex );
			if( pclParentMap == NULL )							return;
			if( pclParentMap->m_siInstanceMapGateIndex < 0)		return;

			cltInstanceMapGate* pclGate = pclinstancemapgatemanager->GetGatebyUnique(gateunique, pclParentMap->m_siInstanceMapGateIndex);
			mapindex	= pclGate->DstMapIndex;
			dx			= pclGate->DstX;
			dy			= pclGate->DstY;

			siSourceMapIndex = pclGate->SrcMapIndex;

			// ������ ���� ����Ʈ�� ó������ �ʴ´�.
			if( pclGate->siUnique <= 0 )						return;
			if( pclGate->FOUnique <= 0 )						return;
		}
		else
		{
			siSourceMapIndex = pclgatemanager->clGate[gateunique].SrcMapIndex;

			mapindex	= pclgatemanager->clGate[gateunique].DstMapIndex;
			dx			= pclgatemanager->clGate[gateunique].DstX;
			dy			= pclgatemanager->clGate[gateunique].DstY;

			itemunique = pclgatemanager->clGate[gateunique].ItemUnique;
			itemnum	= pclgatemanager->clGate[gateunique].ItemNum;
		}

		if(IsPC(GetCharUnique()))
		{
			// ����� �̵��� �� �ִ� ���ΰ� ?
			if (pclClient->pclMapManager->IsLandMap(mapindex) == false)return ;
		}

		cltCharServer* pclchar = NULL;
		pclchar = this;

		// [����] ���� ������ �� ���¿��� ������ �� �� �ִ� �������� ������ �� �� ���� �������� �̵��ϴ� ���̶�� ���´�
		if ( (pclClient->pclMapManager->IsCanMarketOpenMap( siSourceMapIndex ) == true)
			&& (pclClient->pclMapManager->IsCanMarketOpenMap( mapindex ) == false) )
		{
			if ( (true == pclchar->bPersonalShopSwitch) || (true == pclchar->bPremiumPersonalShop) )
			{
				((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_FAIL_WARPTO_NOTMARKETOPEN_MAP,  0, 0, GetCharUnique());
				return;
			}

		}

		// �߱��� �ϴ� ���� �κ��̴� -KKM-
		if( true == pclClient->IsWhereServiceArea( ConstServiceArea_China ) )
		{
			if( true == pclClient->pclMapManager->IsBlockWarp( mapindex ) )
			{
				//	pclserver->SendServerResponseMsg(0, SRVAL_FAIL_FORBID_ENTERMAP, 0, 0, pclchar->GetCharUnique());
				//	return;
			}
		}

		if(pclClient->pclMapManager->IsEnterLevel(mapindex, pclchar->pclCI->clIP.GetLevel()) == false
			&& mapindex != MAPINDEX_BATTLEROYAL_MABRIUM)
		{
			// �ش���� ���� LvX~LvY������ �� �� �ֽ��ϴ�.
			pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_LEVEL_RANGE, 
				pclClient->pclMapManager->GetMinLevel(mapindex), 
				pclClient->pclMapManager->GetMaxLevel(mapindex),
				pclchar->GetCharUnique());
			return;
		}

		if(itemunique > 0 && itemnum > 0)
		{
			if(pclCI->clCharItem.GetItemNum(itemunique) < itemnum)
			{
				// �ش���� XX ������ X���� �־�� �� �� �ֽ��ϴ�.
				pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_NOITEM, 
					itemunique, itemnum,
					pclchar->GetCharUnique());
				return;
			}
		}

		SI32 levellimit = pclClient->pclMapManager->GetLevelLimit(mapindex);
		if(levellimit <= pclchar->pclCI->clIP.GetLevel())
		{
			//[����] ��Ʋ�ξ� �����̸�
			if( mapindex == MAPINDEX_BATTLEROYAL_MABRIUM )
			{
			
				CBattleRoyalMgr* pclBattleRoyalMgr = ((cltServer*)pclClient)->m_pclBattleRoyalMgr;
				if( NULL != pclBattleRoyalMgr)
				{
					CBattleRoyal* pclBattleRoyal = pclBattleRoyalMgr->GetBattleRoyal(this);
					if(NULL != pclBattleRoyal)
					{
						mapindex = pclBattleRoyal->GetStadiumMapIndex();

						switch(pclBattleRoyal->CanEnter(pclchar))
						{
						case BATTLEROYAL_ENTERERROR_ISOPENED:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTOPEND,  0, 0, pclchar->GetCharUnique());
								break;
							}
						case BATTLEROYAL_ENTERERROR_WASJOINED:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTREJOIN,  0, 0, pclchar->GetCharUnique());
								break;
							}
						case BATTLERPYAL_ENTERERROR_ISMAKING:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTREADYFORMAKING,  0, 0, pclchar->GetCharUnique());
								break;
							}
						case BATTLERPYAL_ENTERERROR_TAKEDOUTSUMMON:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTREADYFORSUMMON,  0, 0, pclchar->GetCharUnique());
								break;
							}
						case BATTLERPYAL_ENTERERROR_FULLEDGAMER:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTREADYFORFULLEDGAMER,  0, 0, pclchar->GetCharUnique());
								break;
							}
						case BATTLERPYAL_ENTERERROR_JOINPARTY:
							{
								bwarpfailswitch = true;
								pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_JOINPARTY,  0, 0, pclchar->GetCharUnique());
								break;
							}
						default:
							{
								break;
							}
						}
					}
					else
					{
						bwarpfailswitch = true;
						pclserver->SendServerResponseMsg(0, SRVAL_BATTLEROYAL_NOTPROPERLEVEL,  0, 0, pclchar->GetCharUnique());
					}
				}
				
			}
			// �ʺ��������̸�, 
			SI32 minlevel = MAX_LEVEL_FOR_BEGINNERMAP;
			if((mapindex == MAPINDEX_BEGINNER1 )
				&& pclchar->pclCI->clIP.GetLevel() >= minlevel)
			{
				bwarpfailswitch = true;
				// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
				pclserver->SendServerResponseMsg(0, SRVAL_TOOHIGHLEVEL,  minlevel, 0, pclchar->GetCharUnique());
			}

			//cyj ���� ���� ����Ʈ�϶�
			if ( gateunique >= BLACKARMY_GATEUNIQUE_START && gateunique < BLACKARMY_GATEUNIQUE_START + MAX_BLACKARMY_MAP_CNT)
			{ 
				// ����ð��� �ƴϸ� ���� ����
				if ( pclClient->pclBlackArmyManager->bWarSwitch == false)
				{
					bwarpfailswitch = true;

					pclserver->SendServerResponseMsg(0, SRVAL_BLACKARMY_NOTWAR,	0, 0, pclchar->GetCharUnique());
				}
				else 
				{
					if (siBlackArmyWarMapIndex >= 0 && siBlackArmyWarMapIndex < MAX_BLACKARMY_MAP_CNT)
					{
						// ����ʿ� �����޾����� �ش� ���� �ƴ� ������ �̵��ÿ� ���� ����
						if (gateunique != pclClient->pclBlackArmyManager->clWarMap[siBlackArmyWarMapIndex].siGateUnique)
						{
							bwarpfailswitch = true;

							pclserver->SendServerResponseMsg(0, SRVAL_BLACKARMY_NOTALLOC,	0, 0, pclchar->GetCharUnique());

						}

						// ����ʿ� �����޾����� �ش���� ������ ������� ���� ����
						if (pclClient->pclBlackArmyManager->clWarMap[siBlackArmyWarMapIndex].bWarDoing == false)
						{
							bwarpfailswitch = true;

							pclserver->SendServerResponseMsg(0, SRVAL_BLACKARMY_NOTWAR,	0, 0, pclchar->GetCharUnique());

						}

					}
					// ����ʿ� �������� �ʾ����� ���� ����
					else
					{
						bwarpfailswitch = true;

						pclserver->SendServerResponseMsg(0, SRVAL_BLACKARMY_NOTALLOC,	0, 0, pclchar->GetCharUnique());
					}
				}
			}

			// �δ��� �ƴѰ����� �δ����� �����ϴ� ��쿡��
			// ������ �� �ִ� �δ��� ã�´�.
			if( pclClient->pclMapManager->IsInstanceMap( siSourceMapIndex ) == false &&
				pclClient->pclMapManager->IsInstanceMap(mapindex) )
			{
				// �� �� �ִ� ���� ã�´�.
				SI32 siCanEnterMapIndex = pclClient->pclMapManager->GetEnterableInstanceMapIndex(mapindex, this);
				if(siCanEnterMapIndex > 0)
				{
					if( pclClient->pclMapManager->EnterInstanceMap(siCanEnterMapIndex, this) )
					{
						// ���õ� �� �ε����� ��ü�Ѵ�.
						mapindex = siCanEnterMapIndex;
					}
					else
					{
						bwarpfailswitch = true;

						// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
						cltServer* pclserver = (cltServer*)pclClient;
						pclserver->SendServerResponseMsg(0, SRVAL_FAIL_INSTANTDUNGEON_NO_GUILD,  0, 0, pclchar->GetCharUnique());
						return;
					}
				}
				else
				{
					// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
					cltServer* pclserver = (cltServer*)pclClient;

					// [�߰� : Ȳ���� 2008. 1. 16 => �δ��� �����Ϸ��� ���� ��尡 ������ ���� �޽���]
					if(0 == pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex)
					{
						pclserver->SendServerResponseMsg(0, SRVAL_FAIL_INSTANTDUNGEON_MY_NO_GUILD,  0, 0, pclchar->GetCharUnique());
					}		
					else if( siCanEnterMapIndex == 0 )
					{
						pclserver->SendServerResponseMsg(0, SRVAL_FAIL_INSTANTDUNGEON_NO_GUILD,  0, 0, pclchar->GetCharUnique());
					}
					else
					{
						pclserver->SendServerResponseMsg(0, SRVAL_FAIL_INSTANTDUNGEON_OVER_COUNT_GUILD,  0, 0, pclchar->GetCharUnique());
					}
					// �� �� �ִ� �δ��� �����ϴ�.
					bwarpfailswitch = true;

					return;
				}
			}

			// �����忡�� ������ ���� ���, 
			if (pclClient->pclMapManager->IsHuntMap( siSourceMapIndex ) == true)
			{
				// ������ ���� Ÿ���� ���Ѵ�. 
				SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[mapindex]->siMapUnique;
				if(maptypeunique)
				{
					dx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
					dy = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
				}

			}

			if(bwarpfailswitch == false)
			{
				if(itemunique > 0 && itemnum > 0)
				{
					// �������� ����Ͽ� ����ϴ� ����Ʈ�ΰ��
					if(TakeOutItem(itemunique, itemnum, PERSONITEMINFO_OPTION_NONE) == true)
					{
						cltWarpInfo clWarpInfo(1, mapindex, false, false, dx, dy, x, y);
						SetWarpInfo(&clWarpInfo, 40);

						// �����ۻ���� �Ϻ��ϰ� �ϱ����� �ٷ� ������ ��Ų��.
						// �����ۻ��� �Բ� ���� �α׸� DB�� ���´�.
						Warp(true);
					}
					else
					{
						// �ش���� XX ������ X���� �־�� �� �� �ֽ��ϴ�.
						pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_NOITEM, 
							itemunique, itemnum,
							pclchar->GetCharUnique());
						return;
					}
				}
				else
				{
					// �������� ������� �ʴ� ���� ���� ������� ������ ó���Ѵ�.
					cltWarpInfo clWarpInfo(1, mapindex, false, false, dx, dy, x, y);
					SetWarpInfo(&clWarpInfo, 40);
				}

				// �����¿��� pvp�� ���� pvp-
				clInnerStatus.clEnemyType.siEnemyType = ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR;
			}
		}
		else
		{
			// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendServerResponseMsg(0, SRVAL_NOTENOUGHLEVELTOMAP,  levellimit, 0, pclchar->GetCharUnique());

		}
	}
}



//------------------------------------------
// ĳ���� ������ ���� �⺻ ���� 
//------------------------------------------
bool cltCharServer::GetMakeCharInfo(cltMakeCharInfo* pclinfo, bool borderswitch, bool bticketusewarpswitch)
{
	SI32 x, y;
	SI32 attackid;
	SI32 attackcharuniqueid;
	cltOrderDataInfo		clOrder;

	// ����� ĳ���͸� ������ Ȯ������ �ʴ´�. 
	//cyj ����� ĳ���͵� ���� Ȯ�� �ϵ��� ����
	if(GetLife() <= 0 && IsAtb(ATB_HERO) == FALSE) return false;

	// ���� ���۸� ����. 
	if(borderswitch == false)
	{
		clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);
	}
	else
	{
		// ���� �����ϰ� �ִ� ���۵� �����Ѵ�. 
		switch(GetCurrentOrder())
		{
		case ORDER_MOVE:
			if(siMoveMapIndex == GetMapIndex())
			{
				x = clMoveInfo.siDX;
				y = clMoveInfo.siDY;
				pclClient->pclOrderManager->MakeOrder_MOVE(&clOrder, x, y, BY_PLAYER);
			}
			else
			{
				clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);
			}
			break;
		case ORDER_STRAIGHTMOVE:
			if(siMoveMapIndex == GetMapIndex())
			{
				x = clMoveInfo.siDX;
				y = clMoveInfo.siDY;
				pclClient->pclOrderManager->MakeOrder_STRAIGHTMOVE(&clOrder, x, y, BY_PLAYER);
			}
			else
			{
				clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);
			}

			break;

		case ORDER_ATTACK:
			x			= clAttackTarget.clPos.GetX();
			y			= clAttackTarget.clPos.GetY();
			attackid	= clAttackTarget.GetAttackID();
			if(pclCM->IsAlive(attackid))
			{
				attackcharuniqueid	= pclCM->CR[attackid]->GetCharUnique();
			}
			else
			{
				attackcharuniqueid	= 0;
			}

			clOrder.Make(ORDER_ATTACK, BY_PLAYER, (x|(y<<16)), attackcharuniqueid, 0, 0);
			break;
		case ORDER_FISH:
			pclClient->pclOrderManager->MakeOrder_FISH(&clOrder, siDirection, BY_PLAYER);
			break;

		case ORDER_FARM:
			pclClient->pclOrderManager->MakeOrder_FARM(&clOrder, siFarmX, siFarmY, BY_PLAYER);
			break;
		case ORDER_MINE:
			pclClient->pclOrderManager->MakeOrder_MINE(&clOrder, siMineX, siMineY, BY_PLAYER);
			break;
		case ORDER_SITDOWN:
			pclClient->pclOrderManager->MakeOrder_SITDOWN(&clOrder, siDirection, BY_PLAYER );
			break;


		case ORDER_DEAD:
		case ORDER_DIE:
			clOrder.Make(ORDER_DIE, BY_PLAYER, 0, 0, 0, 0);
			break;

		case ORDER_ATTACK_CLIENT:
			x			= clAttackTarget.clPos.GetX();
			y			= clAttackTarget.clPos.GetY();
			attackid	= clAttackTarget.GetAttackID();
			if(pclCM->IsAlive(attackid))
			{
				attackcharuniqueid	= pclCM->CR[attackid]->GetCharUnique();
			}
			else
			{
				attackcharuniqueid	= 0;
			}

			clOrder.Make(ORDER_ATTACK_CLIENT, BY_PLAYER, (x|(y<<16)), attackcharuniqueid, 0, 0);
			break;

		case ORDER_SPECIAL_ACTION:
			{
				clOrder.Make(ORDER_SPECIAL_ACTION, BY_PLAYER, 0, 0, 0, 0);
			}
			break;

		default:
			clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0 );
			break;
		}
	}

	// �ܺ� ���. 
	cltShapeEquipInfo clequip;
	if(IsPC(GetCharUnique()))
	{
		GetShapeEquipInfo(&clequip);
	}

	// AddInfo�� �䱸���� ����. 
	UI32 uiRequestAddinfo = 0;

	SI32 siHorseUnique		= 0;
	SI32 siAddStatusDate	= 0;
	SI32 siTicketNum		= 0;

	// �����̾� ���� ���� ����
	SI32 siPremiumpartsslot1		= 0;
	SI32 sipremiumpartsuniqueslot1	= 0;
	SI32 siPremiumpartsslot2		= 0;
	SI32 sipremiumpartsuniqueslot2	= 0;
	SI32 siPremiumpartsslot3		= 0;
	SI32 sipremiumpartsuniqueslot3	= 0;
	SI32 siPremiumpartsNum		= 0;

	// LEEKH 2007.10.28 - ��µ��� ó�� ����
	cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclHorse != NULL)
	{
		siHorseUnique	= pclHorse->siHorseUnique;
		siAddStatusDate	= pclHorse->clAddStatusDate.GetDateVary();
		siTicketNum		= pclHorse->siStatusTicketNum;

		siPremiumpartsslot1		= pclHorse->clPremiumPartsDate_Slot1.GetDateVary();
		sipremiumpartsuniqueslot1		= pclHorse->siPremiumUnique_Slot1;

		siPremiumpartsslot2		= pclHorse->clPremiumPartsDate_Slot2.GetDateVary();
		sipremiumpartsuniqueslot2		= pclHorse->siPremiumUnique_Slot2;

		siPremiumpartsslot3		= pclHorse->clPremiumPartsDate_Slot3.GetDateVary();
		sipremiumpartsuniqueslot3		= pclHorse->siPremiumUnique_Slot3;

		siPremiumpartsNum		= pclHorse->siPremiumPartsNum;

	}

	SI32 BufTransformKind = 0;
	if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) )
	{
		stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf( BUF_TRANSFORM );
		if(pclBuf)			BufTransformKind = pclBuf->siPercent;
	}

	SI16 siDisplayLoveLevel = 0;
	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		if ( (pclMarriage) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclMarriage->GetConnectMode()) )
		{
			siDisplayLoveLevel = pclMarriage->GetLoveLevel();
		}
	}

	//KHY - 1001 - ��µ��� 5���� ���� ����.
	cltMakeCharInfo	clMakeCharInfo(GetKind(), GetCharUnique(), pclCI->clIP.GetLevel(), GetLife(), GetX(), GetY(), GetMapIndex(), 
		siChildCharUnique, 
		siSummonIndex, 
		bHeadChatSwitch,		// �Ӹ����� �����ϴ°�?
		bPersonalShopSwitch,
		bNoFirstAttackSwitch | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK) | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI)  | pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) 
		| pclCI->clBI.clBufInfo.IsHaveBufExecptGetType(BUF_TRANSFORM, BUF_GET_TYPE_NIGHTMARKET),	// ���� �� �����Ǿ� �ִ°�. 
		bNoIceAttackSwitch,
		pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) && pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP) ,
		pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FOURLEAF) , pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER),
		IsHaveCushionItem() ,
		pclCI->clBI.uiSex,			// ����. 
		&clequip,
		pclCI->clHorseInfo.siCurrentShowHorse,
		clPB.GetTotalMoveSpeed(),
		clJim.siJimHostCharUnique, 
		GetGameMasterLevel(), 
		0,
		siHorseUnique,
		pclCI->clBI.siHomeVillage,
		pclCI->clBI.siSchoolUnique,
		&pclCI->clCharRank, 
		siAddStatusDate,
		siTicketNum,
		&clOrder,
		&clIdentity, 
		&clInnerStatus,
		uiRequestAddinfo,
		bticketusewarpswitch,
		pclCI->clBI.uiGMMode,
		pclCI->clBI.bPCRoom,
		siPersonalMakeItemType,
		&pclCI->clBI.clGuildRank,
		&pclCI->clPersonNaming,
		pclCI->clHorseInfo.siCurrentHorseRIDEHORSE,
		clTransFormInfo.uiNowTransFormKind,
		clTransFormInfo.bTransFormMode,
		uiBestGuild,										// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
		bIsElectionStump,
		clTransFormInfo.bTransForm100Effect,
		BufTransformKind,
		siEnemUnique,
		pclValentineWar,
		bElixirSwitch
		,pclPVP_LeagueInfo
		,siPremiumpartsslot1
		,sipremiumpartsuniqueslot1
		,siPremiumpartsslot2
		,sipremiumpartsuniqueslot2
		,siPremiumpartsslot3
		,sipremiumpartsuniqueslot3
		,siPremiumpartsNum
		,m_uiWeaponSkillMasterEmblem
		,siDisplayLoveLevel
		,siMySoulGuardUnique
		,bRepairShopSwitch
		,&pclCI->clBI.clBufInfo
		);

	pclinfo->Set(&clMakeCharInfo);

	return true;
}

// ���� ĳ������ �����͸� ���Ѵ�. 
cltCharServer* cltCharServer::GetHostChar()
{
	// ��ȯ���̸�, ������ ���� ���� ���̴�. 
	if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
		if(pclCM->IsValidID(clIdentity.clSummonInfo.siParentCharUnique))
		{
			return (cltCharServer*)(pclCM->CR[clIdentity.clSummonInfo.siParentCharUnique]);
		}
	}

	return this;
}

// ����Ǿ� �ִ� ���� ������ �����Ѵ�. 
bool  cltCharServer::DoReservedAttackAction(cltOrderDataInfo* pclorder)
{
	if(siReservedAttackTargetID == 0)return false;

	pclClient->pclOrderManager->MakeOrder_ATTACK(pclorder, siReservedAttackTargetID, BY_PLAYER);

	SetReservedAttackTargetID(0);

	return true;
}

bool  cltCharServer::DoReservedAttackClientAction(cltOrderDataInfo* pclorder)
{
	if(siReservedAttackTargetID == 0)return false;

	pclClient->pclOrderManager->MakeOrder_ATTACK_CLIENT(pclorder, siReservedAttackTargetID, BY_PLAYER);

	SetReservedAttackTargetID(0);

	return true;
}



// ���� ���� ���� ��� ���¸� üũ�Ѵ�. 
void  cltCharServer::CheckWeaponSkillScroll(cltItemManagerCommon* pclItemManager)
{
	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USESWORD1) == true)
	{
		bSwordScroll = true;
		siSwordScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USESWORD1);
	}
	else
	{
		bSwordScroll = false;
	}

	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USESPEAR1) == true)
	{
		bSpearScroll = true;
		siSpearScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USESPEAR1);
	}
	else
	{
		bSpearScroll = false;
	}

	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USEAXE1) == true)
	{
		bAxeScroll = true;
		siAxeScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USEAXE1);
	}
	else
	{
		bAxeScroll = false;
	}

	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USEGUN1) == true)
	{
		bGunScroll = true;
		siGunScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USEGUN1);
	}
	else
	{
		bGunScroll = false;
	}
	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USEBOW1) == true)
	{
		bBowScroll = true;
		siBowScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USEBOW1);
	}
	else
	{
		bBowScroll = false;
	}
	if(pclCI->clCharItem.UseWeasponSkillScroll(pclItemManager,SKILL_USESTAFF1) == true)
	{
		bStaffScroll = true;
		siStaffScrollNum = pclCI->clCharItem.UseWeasponSkillScrollNum(pclItemManager,SKILL_USESTAFF1);
	}
	else
	{
		bStaffScroll = false;
	}

}

// �� �ܾ� ��� ��Ȳ�� ���´�. 
void cltCharServer::GetShapeEquipInfo(cltShapeEquipInfo* pclequip)
{
#ifdef _DEBUG
	//--------------------------------------------------
	// ������� ���� ��� ���¸� üũ�Ѵ�. 
	//--------------------------------------------------
	CheckWeaponSkillScroll( pclClient->pclItemManager);
#endif


	// ����, �Ǻ��� ����� �����Ѵ�. 
	// ����, ������ ����� �����Ѵ�. 
	if((ActiveClothes<0)||(ActiveHat<0))
	{
		return;
	}

	cltItem* pclarmour	= &pclCI->clCharItem.clItem[ ActiveClothes ];
	cltItem* pclhelmet = &pclCI->clCharItem.clItem[ActiveHat];		

	SI32 AvailableClothes = ActiveClothes;
	SI32 AvailableHelmet = ActiveHat;

	cltServer *pclserver = (cltServer*)pclClient;

	//-------------------------------------------------------------------------
	// �Ⱓ�� üũ�Ͽ�, ��¥ ������߸� �ϴ� �� �Ʒ��� �����.
	// ����, �Ǻ���


	//�ƹ�Ÿ �������� ��� ������ ����ũ�� 0 ���� ������ �ʴ´�.
	if(pclarmour->IsHaveUseDay())
	{
		SI32 useableDay = 0;
		BOOL rVal = pclserver->GetItemUseAbleDay(pclarmour,&useableDay);
		
		if(useableDay < 0 && rVal == TRUE ) // �Ⱓ�� ���� �������̸� �ȱ׷��ش�.
		{
			if ( ITEMTYPE_AVATAR_DRESS == pclarmour->GetAavatarType( pclserver->pclItemManager ) )
			{
			}
			else // �ƹ�Ÿ �������� �ƴҰ�� ����ȴ�.
			{
				pclarmour->siUnique = 0;
			}
		}
	}

	// ����, ������
	if(pclhelmet->IsHaveUseDay())
	{
		SI32 useableDay = 0;
		BOOL rVal = pclserver->GetItemUseAbleDay(pclhelmet,&useableDay);
		
		if(useableDay < 0 && rVal == TRUE ) // �Ⱓ�� ���� �������̸� �ȱ׷��ش�.
		{
			if ( ITEMTYPE_AVATAR_HAT== pclhelmet->GetAavatarType( pclserver->pclItemManager ) )
			{
			}
			else
			{
				pclhelmet->siUnique = 0;
			}
		}
	}


	if( pclarmour->siUnique == 0 )
	{
		if( AvailableClothes == PERSONITEM_DRESS)
			AvailableClothes = PERSONITEM_ARMOUR;
		else
			AvailableClothes = PERSONITEM_DRESS;     
	}

	if(pclhelmet->siUnique==0)
	{
		if(AvailableHelmet == PERSONITEM_HELMET)
			AvailableHelmet = PERSONITEM_HAT;
		else
			AvailableHelmet = PERSONITEM_HELMET;
	}

	pclhelmet 	= &pclCI->clCharItem.clItem[ AvailableHelmet ];
	pclarmour       = &pclCI->clCharItem.clItem[ AvailableClothes ];	
	//-------------------------------------------------------------------------

	SI32 NowActiveClothesUnique = pclCI->clCharItem.clItem[AvailableClothes].siUnique;		// ���� �������� �Ǹ�
	SI32 NowActiveHatUnique = pclCI->clCharItem.clItem[AvailableHelmet].siUnique;			// ���� �������� ����


	//[�߰� : Ȳ���� 2007. 12. 12 �ų� �̺�Ʈ ����Ʈ �׸��� �����۵� ����.]
	UI32	RareInfo = pclarmour->clItemCommon.clCommonInfo.uiRareInfo;

	SYSTEMTIME sTime;
	GetLocalTime( &sTime );

	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		// �����۸� �������ΰ�?
		if(pclClient->pclItemManager->IsItemInfoAtb( pclarmour->siUnique, ITEMINFOATB_ITEMMALL ))
		{
			RareInfo = NEWYEAR_EFFECT;
		}
	}


	if ( (NowActiveHatUnique == ITEMUNIQUE(23117) && NowActiveClothesUnique == ITEMUNIQUE(23696))  // ��ȥ ���� �Ӹ� + ��ȥ ���Ŀ� �νõ�  �̰ų�
		||  (NowActiveHatUnique == ITEMUNIQUE(23118) && NowActiveClothesUnique == ITEMUNIQUE(23698)))     // ��ȥ�� ����� +  ������ �巹�� �϶���.

	{
		// ��ȥ���� ��쿡�� ����Ʈ�� �׷��ش�.
		if(IsMarried())
		{
			RareInfo = MARRIED_FULLDRESS_EFFECT;
		}
	}

	cltShapeEquipInfoUnit clArmour(pclarmour->siUnique, RareInfo, pclarmour->Element.siType, pclarmour->Element.siPower );
	//---------------------------------------------------------------------------------------------------------------
	RareInfo = 0;

	//[�߰� : Ȳ���� 2007. 12. 12 �ų� �̺�Ʈ ����Ʈ �׸��� �����۵� ����.]
	RareInfo = pclhelmet->clItemCommon.clCommonInfo.uiRareInfo;
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		// �����۸� �������ΰ�?
		if(pclClient->pclItemManager->IsItemInfoAtb( pclhelmet->siUnique, ITEMINFOATB_ITEMMALL ))
		{
			RareInfo = NEWYEAR_EFFECT;
		}
	}

	if ( (NowActiveHatUnique == ITEMUNIQUE(23117) && NowActiveClothesUnique == ITEMUNIQUE(23696))  // ��ȥ ���� �Ӹ� + ��ȥ ���Ŀ� �νõ�  �̰ų�
		||  (NowActiveHatUnique == ITEMUNIQUE(23118) && NowActiveClothesUnique == ITEMUNIQUE(23698)))     // ��ȥ�� ����� +  ������ �巹�� �϶���.
	{
		// ��ȥ���� ��쿡�� ����Ʈ�� �׷��ش�.
		if(IsMarried())
		{
			RareInfo = MARRIED_FULLDRESS_EFFECT;
		}
	}

	cltShapeEquipInfoUnit clHelmet(pclhelmet->siUnique, RareInfo, pclhelmet->Element.siType, pclhelmet->Element.siPower );
	//---------------------------------------------------------------------------------------------------------------
	RareInfo = 0;


	cltShapeEquipInfoUnit clMantle(pclCI->clCharItem.clItem[PERSONITEM_MANTLE].siUnique, pclCI->clCharItem.clItem[PERSONITEM_MANTLE].clItemCommon.clCommonInfo.uiRareInfo
		, pclCI->clCharItem.clItem[PERSONITEM_MANTLE].Element.siType , pclCI->clCharItem.clItem[PERSONITEM_MANTLE].Element.siPower );

	//[�߰� : Ȳ���� 2007. 12. 12 �ų� �̺�Ʈ ����Ʈ �׸��� �����۵� ����.]
	RareInfo = pclCI->clCharItem.clItem[ActiveWeapon].clItemCommon.clCommonInfo.uiRareInfo;
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		SI32	Unique[] = {97, 172, 372, 272, 468, 568};
		SI32	Cnt		 = sizeof(Unique)/ sizeof(SI32);
		for(SI32 i = 0; i < Cnt; ++i)
		{
			if(Unique[i] == pclCI->clCharItem.clItem[ActiveWeapon].siUnique)		
			{
				if(RARELEVEL_6 != pclCI->clCharItem.clItem[ ActiveWeapon ].clItemCommon.clCommonInfo.uiRareInfo)
				{
					RareInfo = NEWYEAR_EFFECT;
				}
			}
		}
	}

	cltShapeEquipInfoUnit clWeapon(pclCI->clCharItem.clItem[ActiveWeapon].siUnique, 
		RareInfo, pclCI->clCharItem.clItem[ActiveWeapon].Element.siType , pclCI->clCharItem.clItem[ActiveWeapon].Element.siPower );

	cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;

#ifdef _SAFE_MEMORY
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		m_siSetItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep , USE_SETITEM_1 );
		m_siSetItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep2, USE_SETITEM_2 );
		m_siSetItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep3, USE_SETITEM_3 );
		m_siSetItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep4, USE_SETITEM_4 );
		m_siSetItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep5, USE_SETITEM_5 );
	}
	else
		m_siSetItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( &pclCI->clCharItem.clItem[0] );
#else
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		m_siSetItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep , USE_SETITEM_1 );
		m_siSetItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep2, USE_SETITEM_2 );
		m_siSetItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep3, USE_SETITEM_3 );
		m_siSetItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep4, USE_SETITEM_4 );
		m_siSetItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( this, &m_siSetItemStep5, USE_SETITEM_5 );
	}
	else
		m_siSetItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( pclCI->clCharItem.clItem );
#endif
	// ��Ʈ������ ���� ����
	cltShapeEquipInfo clinfo(	&clArmour, &clHelmet, &clMantle, &clWeapon,
		bSwordScroll,
		bSpearScroll,
		bAxeScroll,
		bGunScroll,
		bBowScroll,
		bStaffScroll,
		bCannonScroll,
		siSwordScrollNum,
		siSpearScrollNum,
		siAxeScrollNum,
		siGunScrollNum,
		siBowScrollNum,
		siStaffScrollNum,
		m_siSetItemType,
		m_siSetItemStep,
		m_siSetItemType2,
		m_siSetItemStep2,
		m_siSetItemType3,
		m_siSetItemStep3,
		m_siSetItemType4,
		m_siSetItemStep4,
		m_siSetItemType5,
		m_siSetItemStep5		);
	pclequip->Set(&clinfo);

}

void cltCharServer::SetLastKillClock(DWORD clock)
{
	clSimpleVary.dwLastKillClock = clock;
}

void cltCharServer::SetReservedAttackTargetID(SI16 id)
{
	siReservedAttackTargetID = id;
}

//--------------------------------
// Group
//--------------------------------
void cltCharServer::SetNPCGroup(SI32 group)
{
	NPCGroup	= group;
}

SI32 cltCharServer::GetNPCGroup() const
{
	return NPCGroup;
}

void cltCharServer::SetLastReceveFromClientClock(DWORD clock)
{
	LastReceveFromClientClock = clock;
}

bool cltCharServer::CheckSubBigItem()
{
	if (pclCI->clIP.GetLevel() <= 0) 
		return false;

	UI32 usertotalstatus = pclCI->clIP.GetLevel() * 6;
	UI32 luckpercent = pclCI->clIP.GetLuk() * 100 / usertotalstatus;
	UI32 hndpercent = pclCI->clIP.GetHnd() * 100 / usertotalstatus;

	UI32 luckpoint = 0;
	UI32 hndpoint = 0;
	UI32 subpoint = 0;

	if (luckpercent >= 0)
	{
		if (luckpercent < 3)	luckpoint = 6;
		else if (luckpercent < 6)	luckpoint = 12;
		else if (luckpercent < 9)	luckpoint = 18;
		else if (luckpercent < 12)	luckpoint = 24;
		else if (luckpercent < 15)	luckpoint = 30;
		else if (luckpercent < 18)	luckpoint = 36;
		else if (luckpercent < 21)	luckpoint = 42;
		else if (luckpercent < 24)	luckpoint = 48;
		else if (luckpercent < 27)	luckpoint = 54;
		else	luckpoint = 60;
	}

	if (hndpercent >= 0)
	{
		if (hndpercent < 3)	hndpoint = 6;
		else if (hndpercent < 6)	hndpoint = 12;
		else if (hndpercent < 9)	hndpoint = 18;
		else if (hndpercent < 12)	hndpoint = 24;
		else if (hndpercent < 15)	hndpoint = 30;
		else if (hndpercent < 18)	hndpoint = 36;
		else if (hndpercent < 21)	hndpoint = 42;
		else if (hndpercent < 24)	hndpoint = 48;
		else if (hndpercent < 27)	hndpoint = 54;
		else	hndpoint = 60;
	}

	subpoint = (luckpoint * 0.3 + hndpoint * 0.2) + 2;

	UI32 randvalue = rand() % 1000000;
	// ��ھ����� ��
	if (randvalue < subpoint)
	{
		return true;
	}

	return false;

}

bool cltCharServer::IsNowAddBufThroughDB()
{
	return bIsNowAddBufThroughDB;		 
}

bool cltCharServer::AddBufThroughDB(SI16 index, stBuf* pbuf, bool useitem, SI16 itempos, cltItem* pclitem, GMONEY money)
{
	if( IsNowAddBufThroughDB() ) return false;

	sDBRequest_AddBuf clAddbuf( pclCI->GetPersonID(), index, pbuf, useitem, itempos, pclitem, money);
	((cltServer*)pclClient)->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clAddbuf);

	bIsNowAddBufThroughDB = true;	
	return true;
}

bool cltCharServer::AddBufThroughDB_AccumulateEnable(stBuf* pBuf, bool useitem, SI16 itempos, cltItem* pclitem, GMONEY money)
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return false;
	if ( NULL == pBuf ) return false;

	// ���� ������ �̹� �ִ� ��Ȳ���� üũ
	bool bIsExistBuf = pclCI->clBI.clBufInfo.IsHaveBuf( pBuf->siBufType );		// ���� ������ �ִ��� Ȯ��
	SI32 siRemainTime = 0;
	SI16 siIndex = -1;

	// ���� ������ ������쿡�� ������ �ִ� ������ ���� �ð��� ���� �߰��� ���� �ð��� ������.
	if ( true == bIsExistBuf )
	{
		SI32 siCurrentGameTime = 0;
		siCurrentGameTime = pServer->pclTime->clDate.GetHourVary();

		SI32 siBufOverTime = 0;
		siBufOverTime = pclCI->clBI.clBufInfo.GetBufOvertime( pBuf->siBufType );

		// ������ ���� �ð��� ���Ѵ�.
		if ( siCurrentGameTime <= siBufOverTime )
		{
			siRemainTime = siBufOverTime - siCurrentGameTime;
		}

		// ������ ��ġ�� �ε����� ���´�.
		SI16 siExistBufIndex = 0;
		siExistBufIndex = pclCI->clBI.clBufInfo.FindBufSlot( pBuf->siBufType );

		stBuf* pExistBuf = pclCI->clBI.clBufInfo.GetBuf( pBuf->siBufType );
		if ( NULL != pExistBuf )
		{
			siIndex = siExistBufIndex;		// �ش� �ڸ��� �ٽ� ���� ������ ���Ŵϱ� siIndex�� ������ ������ ��ġ�� �ִ´�.

			// ��� ���� ������ �޼��� ����
			//stBuf delbuf( pExistBuf->siBufType, 0, 0, 0 );
			//sDBRequest_DeleteBuf clMsg( pclCI->GetPersonID(), siExistBufIndex, &delbuf );
			//pServer->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
		}
	}
	else
	{
		siIndex = pclCI->clBI.clBufInfo.FindEmptySlot( pBuf->siBufType, false );	// ������ �� �ε����� ���´�.
	}

	SI32 siUsetime = pBuf->siOverTime;			// ������ overtime�� �� �Լ� ȣ�� ���� ���Ǿ �Ѿ�;� �Ѵ�. (���� ��ƾ ħ�� ����)
	siUsetime += siRemainTime;

	if ( 0 <= siIndex )
	{
		stBuf addbuf( pBuf->siBufType, pBuf->siPercent, siUsetime );
		AddBufThroughDB( siIndex, &addbuf, useitem, itempos, pclitem, money );
	}

	return true;
}

// Ư�� ����ũ�� ���� �ֹο��� ������ ������ �ش�. (������ �ִ� �������Ը� ��) - Ʈ���Ű� Ư�� ������ ����� ������ �ϰ�쿡�� �� �Լ� ȣ���ϴ°� �ٶ�����.
bool cltCharServer::AddBufToAllTownPerson(SI32 siTownUnique, stBuf* pBuf, SI32 siSkipBuf1, SI32 siSkipBuf2)		// ���� ������ �ϴ� �ΰ��� ����. �� �ʿ��ϸ� ...�������� ��ü
{
	// 1. ���� ���� üũ
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer->pclVillageManager ) return false;
	if ( false == pServer->pclVillageManager->IsValidVillage( siTownUnique ) ) return false;
	if ( NULL == pBuf ) return false;

	// 2. �α����ϰ� �ִ� ��� ���� �ֹο��� ���� ����
	SI32 index = 0;
	SI32 id;
	while ( id = pServer->pclCM->GetCharOrder(index) )		// id�� false ��, 0�� �Ǹ� ����. (GetCharOrder() �Լ��� 0�� �����ϸ� ����)
	{
		index++;

		cltCharServer* pclchar  = (cltCharServer*)pServer->pclCM->CR[id];
		if ( NULL == pclchar ) continue;

		if(pclchar->IsValidConnection() == false) continue;					// ������� �Ŀ��Ը� �����Ѵ�
		if(pclchar->pclCI->clBI.siHomeVillage != siTownUnique) continue;	// �ش� ���� �ֹο��Ը� �ش�.

		// ���� ���޿� ���� ���� ó�� (����ġ �� ��� ����� ���� ȿ���� ������ ����ġ ���� ���ް�..)
		if ( true == pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(siSkipBuf1) ) continue;
		if ( true == pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(siSkipBuf2) ) continue;

		// ���ִ� ���� �ִϱ� �ֹ� �˻��ϴ� ���ְ� ������ �׳� ��ŵ
		if ( RANKTYPE_CITYHALL == pclchar->pclCI->clCharRank.siType ) continue;

		AddTownBufThroughDB( pclchar, pBuf );
	}

	return true;
}

bool cltCharServer::AddTownBufThroughDB(cltCharServer* pCharServer, stBuf* pBuf)
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return false;
	if ( NULL == pCharServer || NULL == pBuf ) return false;

	// ���� ������ �̹� �ִ� ��Ȳ���� üũ
	bool bIsExistBuf = pCharServer->pclCI->clBI.clBufInfo.IsHaveBuf( pBuf->siBufType );		// ���� ������ �ִ��� Ȯ��
	SI32 siRemainTime = 0;
	SI16 siIndex = -1;

	// ���� ������ ������쿡�� ������ �ִ� ������ ���� �ð��� ���� �߰��� ���� �ð��� ������.
	if ( true == bIsExistBuf )
	{
		SI32 siCurrentGameTime = 0;
		siCurrentGameTime = pServer->pclTime->clDate.GetHourVary();

		SI32 siBufOverTime = 0;
		siBufOverTime = pCharServer->pclCI->clBI.clBufInfo.GetBufOvertime( pBuf->siBufType );

		// ������ ���� �ð��� ���Ѵ�.
		if ( siCurrentGameTime <= siBufOverTime )
		{
			siRemainTime = siBufOverTime - siCurrentGameTime;
		}

		// ������ ��ġ�� �ε����� ���´�.
		SI16 siExistBufIndex = 0;
		siExistBufIndex = pCharServer->pclCI->clBI.clBufInfo.FindBufSlot( pBuf->siBufType );

		stBuf* pExistBuf = pCharServer->pclCI->clBI.clBufInfo.GetBuf( pBuf->siBufType );
		if ( NULL != pExistBuf )
		{
			siIndex = siExistBufIndex;

			// ��� ���� ������ �޼��� ����
			//stBuf delbuf( pExistBuf->siBufType, 0, 0, 0 );
			//sDBRequest_DeleteBuf clMsg( pCharServer->pclCI->GetPersonID(), siExistBufIndex, &delbuf );
			//pServer->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
		}
	}
	else
	{
		siIndex = pCharServer->pclCI->clBI.clBufInfo.FindEmptySlot( pBuf->siBufType, false );	// ������ �� �ε����� ���´�.
	}

	SI32 siUsetime = pServer->pclTime->clDate.GetHourVary() + 24;			// ����Ʈ ���� ���ǽð� 48�а� (���ӽð�1��).
	siUsetime += siRemainTime;

	if ( 0 <= siIndex )
	{
		stBuf addbuf( pBuf->siBufType, pBuf->siPercent, siUsetime );
		pCharServer->AddBufThroughDB( siIndex, &addbuf, false, 0, NULL );
	}

	return true;
}

SI64 cltCharServer::CalcTiredPoint( SI64 point )
{
	switch( uiTiredState )
	{
	case TIRED_STATUS_LITTLE_TIRED:
		return point >> 1; // �������� ���ȴ�.
	case TIRED_STATUS_MUCH_TIRED:
		return 0;
	default:
		return point;
	}
}
SI32 cltCharServer::CalcTiredPoint( SI32 point )
{
	switch( uiTiredState )
	{
	case TIRED_STATUS_LITTLE_TIRED:
		return point >> 1;
	case TIRED_STATUS_MUCH_TIRED:
		return 0;
	default:
		return point;
	}
}
SI16 cltCharServer::CalcTiredPoint( SI16 point )
{
	switch( uiTiredState )
	{
	case TIRED_STATUS_LITTLE_TIRED:
		return point >> 1;
	case TIRED_STATUS_MUCH_TIRED:
		return 0;
	default:
		return point;
	}
}

bool cltCharServer::CharTransform( SI16 siCharKind  , SI16 ActiveWeapon , cltItem *pclItemList ,cltPSkillInfo *pclskillinfo)
{
	if(siCharKind > 0)
	{
		// ���� �ϱ�
		// �ɷ�ġ �缳��.
		clTransFormInfo.bTransFormMode = true;	
		clTransFormInfo.uiNowTransFormKind = siCharKind;
	}
	else
	{
		// ���� ����
		clTransFormInfo.bTransFormMode = false;	
	}

	// ������ ĳ���� ���°��� �ٲ��� �Ѵ�.
	SetParametaBoxAction(true);

	// ���� - �ٽñ׷��ֱ�.
	OrderStop(GetCharID(), BY_COMPUTER);

	// �����϶��� �̺�Ʈ Ÿ���� ����� ���� �Ѵ�.
	cltWarpInfo clWarpInfo(1, GetMapIndex(),true, false, GetX(), GetY(), -1, -1);
	SetWarpInfo(&clWarpInfo, 10000);
	Warp();

	if( siCharKind > 0)
	{
		// Ŭ���̾�Ʈ�� �˸��� - ������ �ϴ� ��쿡�� ������ ������Ʈ �Ѵ�.
		cltGameMsgResponse_SetTransForm clinfo1(&clTransFormInfo);
		cltMsg clMsg1(GAMEMSG_RESPONSE_TRANSFORM, sizeof(clinfo1), (BYTE*)&clinfo1);
		SendNetMsg((sPacketHeader*)&clMsg1);
	}

	return true;
}

//KHY - 1113 - ũ�������� �̺�Ʈ - ��ǥ kind������ ��ȯ�Ѵ�.
void cltCharServer::ChristamsEventQuest(SI32 *Enemykind)
{

	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_YETI"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_YETI_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_YETI_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_YETI"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWELF"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWELF_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWELF_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_SNOWELF"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_TONGUEGIRL"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_TONGUEGIRL_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_TONGUEGIRL_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_TONGUEGIRL"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWMAN"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWMAN_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_SNOWMAN_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_SNOWMAN"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_MARIONETTE_SANTA"));

		return;
	}
	//---------------------------------------------------------------------//
	if(*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA_1"))||
		*Enemykind == pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA_2"))) 
	{
		*Enemykind = pclClient->GetUniqueFromHash(TEXT("KIND_WILDCAT_SANTA"));

		return;
	}
	//---------------------------------------------------------------------//
}

//-----------------------------------------------------------------------------------------------------------------------
// Dec		: ���� ���� �ִ� ��ȣ������ Ư�� �ɷ��� �ߵ� ��Ų��
// Param	: siAbility = �ߵ� ��Ű�����ϴ� Ư���ɷ�
//			  siLessThanValue = �� ���� 0�̻��϶� �� ������ Ư���ɷ� �ߵ� ��ġ�� ������ �ߵ����� ����	
//			  siMoreThanValue = �� ���� 0�̻��϶� �� ������ Ư���ɷ� �ߵ� ��ġ�� ũ�� �ߵ����� ����
// Return	: Ư���ɷ� �ߵ� ��ġ
//-----------------------------------------------------------------------------------------------------------------------
SI32 cltCharServer::SoulGuardAbilityActivity( SI32 siAbility, SI32 siLessThanValue/* =0 */, SI32 siMoreThanValue/* =0  */)
{
	if ( !IsPC(GetCharID()) )
	{
		return 0;
	}

	if ( 0 >= siAbility )
	{
		return 0;
	}

	// ��Ÿ�� üũ
	if(pclClient->CurrentClock > 0)	//���� �ð��� ������
	{
		if(clockSoulGuardCoolTime > pclClient->CurrentClock)
			return 0;
	}
	else // ���� �ð��� �������
	{
		if(TABS(clockSoulGuardCoolTime) <= TABS(pclClient->CurrentClock))
			return 0;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return 0;
	}

	if ( NULL == pclserver->m_pclSoulGuardMgr )
	{
		return 0;
	}

	if ( FALSE == IsSoulGuardStatusOut() )
	{
		return 0;
	}

	// ģȭ���� 0�����̸� �ٷ� ����
	if ( 0 >= pclSoulGuard->GetFellowShip() )
	{
		return 0;
	}

	// �ش� �ɷ��� ���� ���� ������ ����
	if ( FALSE == pclSoulGuard->IsHaveAbility(siAbility) )
	{
		return 0;
	}

	// ������ ��ȯ�� ���¸� �˻�� ������ ���� ��ȯ�� ĳ���ͷ� �ٽ��ѹ� �˻��Ѵ�
	cltCharServer* pclMySoulGuard = pclserver->pclCM->GetCharServer( siMySoulGuardUnique );
	if ( NULL == pclMySoulGuard )
	{
		return 0;
	}

	if ( pclCI->GetPersonID() != pclMySoulGuard->clIdentity.clSoulGuard.m_siParentPersonID )
	{
		return 0;
	}

	// ��ȣ������ �����ִ� �ɷ� ������ �����´�
	cltSoulGuardAbilityUnit* pclGetUnit = pclserver->m_pclSoulGuardMgr->GetAbilityUnit( pclSoulGuard->GetKind(), siAbility );
	if ( NULL == pclGetUnit )
	{
		return 0;
	}

	// ���� �ߵ����� �� ������ ���ٸ� ����
	if ( (0 < siLessThanValue) &&  (siLessThanValue > pclGetUnit->GetValue()) )
	{
		return 0;
	}

	// ���� �ߵ����� �� ������ ũ�ٸ� ����
	if ( (0 < siMoreThanValue) && (siMoreThanValue < pclGetUnit->GetValue()) )
	{
		return 0;
	}

	// �Ҹ�Ǵ� ģȭ�º��� �� ���� ģȭ���� ���� �ִٸ� �������� ����
	if ( pclGetUnit->GetDecFellowShip() > pclSoulGuard->GetFellowShip() )
	{
		return 0;
	}

	// ģȭ�� ��� Ȯ��
	SI32 siRandProbably = (rand() % Const_Max_SoulGuard_ActiveProbably) + 1;
	if ( pclGetUnit->GetProbably() < siRandProbably  )
	{
		return 0;
	}

	// �� ������� ������ �ʴ� ���� Ư�� �ɷ��� ��� �����ϴ�
	SI16 siCalcFellowShip = pclSoulGuard->GetFellowShip() - pclGetUnit->GetDecFellowShip();
	if ( 0 > siCalcFellowShip )
	{
		siCalcFellowShip = 0;
	}

	// ģȭ�� ����
	pclSoulGuard->SetFellowShip( siCalcFellowShip );

	// ��Ÿ���� ���� ���ְ�
	clockSoulGuardCoolTime = (pclserver->CurrentClock + (pclGetUnit->GetCoolTimeSec() * 1000));

	// �ɷ¿� ���� ��縦 ���� ĳ���Ϳ��� �ѷ��ش�
	SI32 siTextNum = pclserver->m_pclSoulGuardMgr->GetSoulGuardSpeechTextNum( siAbility );
	if ( 0 < siTextNum )
	{
		NTCHARString128 strText = GetTxtFromMgr( siTextNum );

		strText.Replace( TEXT("#mastername#"),	(TCHAR*)GetName() );
		strText.Replace( TEXT("#value#"),		SI16ToString(pclGetUnit->GetValue()) );

		cltGameMsgResponse_Chat clChat( siMySoulGuardUnique, CHATMODE_NORMAL, pclSoulGuard->GetName(), strText );
		cltMsg clMsg(GAMEMSG_RESPONSE_CHAT, clChat.siDataSize, (BYTE*)&clChat);

		SendNeighbourMsg( &clMsg, true );
	}

	// Ư����� �ߵ���Ų��
	SI32 siSoulGuardCharID = GetSoulGuardCharID();
	if ( pclCM->IsValidID(siSoulGuardCharID) )
	{
		OrderSpecialAction( siSoulGuardCharID, ANITYPE_SOULGUARDSF, BY_PLAYER );
	}
	
	// DB���� �����ش�
	sDBRequest_SoulGuard_FellowShipSet clMsg( pclCI->GetPersonID(), pclSoulGuard->GetSoulGuardID(), pclSoulGuard->GetFellowShip() );
	pclserver->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

	return pclGetUnit->GetValue();

}


void cltCharServer::SendSoulGuardParentDeadMessage( void )
{
	// 30�� �������� �޽����� ������
	if ( TABS(pclClient->CurrentClock-dwLastSoulGuardParentDeadMessageClock) < 30000 )
	{
		return;
	}

	dwLastSoulGuardParentDeadMessageClock = pclClient->CurrentClock;

	SI32 siTextNum		= 0;
	SI32 siRandValue	= rand() % 4;

	switch ( siRandValue )
	{
	case 0:	{	siTextNum = 30249;	}	break;
	case 1:	{	siTextNum = 30250;	}	break;
	case 2:	{	siTextNum = 30251;	}	break;
	case 3:	{	siTextNum = 30252;	}	break;
	}

	if ( 0 >= siTextNum )
	{
		return;
	}

	NTCHARString128 strMessage	= GetTxtFromMgr( siTextNum );
	NTCHARString64	strName;

	// �̸��� �����´�
	SI32			siParentCharID	= GetSoulGuardParentCharID();
	cltCharServer*	pclParentChar	= pclClient->pclCM->GetCharServer( siParentCharID );
	if ( (pclParentChar) && (pclParentChar->pclSoulGuard) )
	{
		strName = pclParentChar->pclSoulGuard->GetName();
	}
	else
	{
		strName = (TCHAR*)GetName();
	}

	cltGameMsgResponse_Chat clChat( GetCharUnique(), CHATMODE_NORMAL, strName, strMessage );
	cltMsg clMsg(GAMEMSG_RESPONSE_CHAT, clChat.siDataSize, (BYTE*)&clChat);
	SendNeighbourMsg(&clMsg, true);
	
}

void cltCharServer::TransformSummon( cltCharManager* _pclCM, cltSummonTransformManager* _pclSummonTransformManager )
{
	//if( false == pclCI->clSummonInfo.IsSummonTranformTickCount() ) return;

	// ���� ��ȯ���� ������ ���.
	cltSummon*	pclCurSummonInfo = pclCI->clSummonInfo.GetSummon( siSummonIndex );
	if( NULL == pclCurSummonInfo ) return;

	// ���� ��ȯ���� ���Ű����� ��ȯ���� �ִ��� ã�´�.
	SI32	siTransformKind		= 0;
	SI32	siTransformAbility  = 0;
	_pclSummonTransformManager->GetTransformSummonInfo( pclCurSummonInfo->siKind, siTransformKind, siTransformAbility );
	if( 0 == siTransformKind ) return;

	// ���� ���� ��ȯ�� ������ ��������.B
	cltSummon clPrevSummonInfo;
	clPrevSummonInfo.Set( pclCurSummonInfo );
	if( false == pclCI->clSummonInfo.IsSummonTranform(siSummonIndex) && 0 >= clPrevSummonInfo.clIP.GetLife() ) return;
	if( false == pclCI->clSummonInfo.IsSummonTranform(siSummonIndex) && SUMMON_POSSIBLE_TRANSFORM_TIME > pclCurSummonInfo->m_siTranformStamina ) return;
	

	// ��ȯ�� ��ȯ���� �־�� �Ѵ�.
	SI32			siTansformSummonIndex	= siSummonIndex;
	cltCharServer*	pclSummon				= pclCM->GetCharServer( GetSummonID() );
	UI32			uiPrevStaminaFrame		= 0;
	if( pclSummon )
	{
		uiPrevStaminaFrame = pclSummon->uiStaminaFrame;

		// ���ӿ��� ��ȯ���� �����. 
		cltCharServer* pclCharSummon = _pclCM->GetCharServer( GetSummonID() );
		if( NULL== pclCharSummon ) return;
		pclCharSummon->DeleteInCell();
		if( FALSE == _pclCM->DeleteChar( GetSummonID() ) )
		{
			_pclCM->DeleteChar( pclCharSummon->GetID() );
		}

		// ������ ���� �ִ� ��ȯ���� ������ �����Ѵ�.
		siChildCharUnique = 0;
		siSummonIndex	  = 0;

		// �������׵� ������ ���Ž�Ų��.
		cltGameMsgResponse_ChangeSummonStatus clInfo( GetID(), siChildCharUnique, siTansformSummonIndex );
		cltMsg clMsg( GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo );

		pclCurSummonInfo = pclCI->clSummonInfo.GetSummon( siTansformSummonIndex );
		if( pclCurSummonInfo )
		{
			cltGameMsgResponse_Summon clInfo( GetCharUnique(), siTansformSummonIndex, pclCurSummonInfo );
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	else
	{
		return;
	}

	//---------------------------------------------
	// ������ ��ȯ���� �����ϰ� ����.
	//---------------------------------------------
	cltSummon	clTranformSummon;
	clTranformSummon.Set( &clPrevSummonInfo );
	clTranformSummon.siKind	= siTransformKind;
	clTranformSummon.clIP.SetStr(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetStr(),  siTransformAbility ) ); 
	clTranformSummon.clIP.SetWis(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetWis(),  siTransformAbility ) ); 
	clTranformSummon.clIP.SetHnd(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetHnd(),  siTransformAbility ) ); 
	clTranformSummon.clIP.SetVit(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetVit(),  siTransformAbility ) ); 
	clTranformSummon.clIP.SetDex(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetDex(),  siTransformAbility ) );  
	clTranformSummon.clIP.SetMag(  CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetMag(),  siTransformAbility ) );  
	clTranformSummon.clIP.SetMana( CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetMana(), siTransformAbility ) ); 
	clTranformSummon.clIP.SetLife( CalcuSummonTransFormAbility( clPrevSummonInfo.clIP.GetLife(), siTransformAbility ) ); 

	// ���� ����.
	if( pclCI->clSummonInfo.IsSummonTranform( siTansformSummonIndex ) )
	{	
		pclCI->clSummonInfo.SetSummonTranformIndex( -1 );	
		pclCI->clSummonInfo.SetSummonTranformTickCount();
	}
	// ���� 
	else
	{ 
		pclCI->clSummonInfo.SetSummonTranformIndex( siTansformSummonIndex );
		pclCI->clSummonInfo.SetSummonTranformTickCount();
	}

	SI32			siTransformSummonID	= CreateSummon( &clTranformSummon );
	cltSummon*		pclTranformSummon	= NULL;
	cltCharServer*	pclTransformSummon  = pclCM->GetCharServer( siTransformSummonID );
	if( pclTransformSummon )
	{
		// ��ȯ�� ���׹̳ʸ� ó���ϴ� ������ ���� �������� ������ �������ش�.
		pclTransformSummon->uiStaminaFrame = uiPrevStaminaFrame;
		//==========================================
		// ������ ���� �ִ� ��ȯ���� ������ �����Ѵ�.
		//==========================================
		pclTranformSummon = pclCI->clSummonInfo.GetSummonFromSummonID( clTranformSummon.siSummonID );
		if( pclTranformSummon )
		{
			pclTranformSummon->Set( &clTranformSummon );
			cltGameMsgResponse_Summon clInfo( GetCharUnique(), siTansformSummonIndex, pclTranformSummon );
			cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo );
			SendNetMsg( (sPacketHeader*)&clMsg );
		}

		siChildCharUnique	= siTransformSummonID; 
		siSummonIndex		= siTansformSummonIndex;

		// ������ ��ȯ�� ĳ���� ������.
		cltCharServer* pclCharTranformSummon = pclCM->GetCharServer( siTransformSummonID );
		if( NULL == pclCharTranformSummon ) return;
		
		pclCharTranformSummon->ParametaBoxAction();

		// �������׵� ������ ���Ž�Ų��.
		cltGameMsgResponse_ChangeSummonStatus clInfo( GetCharUnique(), siChildCharUnique, siSummonIndex);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
		SendNetMsg((sPacketHeader*)&clMsg);
	}	

	// �����̴� ĳ���͸� ����ڷ� �����.
	siSelectedCharIndex = -1 ;
}

void cltCharServer::UpDateSummonTransForm( cltSummonTransformManager* _pclSummonTransformManager, cltSummon* _pSummon, SI32 siTansformSummonIndex, cltSummon** _pTransformSummon  )
{
#if defined(_DBMANAGER)
	return;
#else
	static cltSummon clSrcSummon;

	if( NULL == _pSummon ) return;

	*_pTransformSummon = NULL;

	// ������ ���ִ°�?
	if( pclCI->clSummonInfo.IsSummonTranform( siTansformSummonIndex ) )
	{
		SI32	siTransformKind		= 0;
		SI32	siTransformAbility	= 0;

		// ���� �� ��ȯ���� ���Ž�ų ������ ������ ��´�.
		_pclSummonTransformManager->GetTransformSummonInfo( _pSummon->siKind, siTransformKind, siTransformAbility );

		if( siTransformKind )
		{
			clSrcSummon.Set( _pSummon );
			clSrcSummon.siKind = siTransformKind;
			clSrcSummon.clIP.SetStr(  CalcuSummonTransFormAbility( _pSummon->clIP.GetStr(),  siTransformAbility ) ); 
			clSrcSummon.clIP.SetWis(  CalcuSummonTransFormAbility( _pSummon->clIP.GetWis(),  siTransformAbility ) ); 
			clSrcSummon.clIP.SetHnd(  CalcuSummonTransFormAbility( _pSummon->clIP.GetHnd(),  siTransformAbility ) ); 
			clSrcSummon.clIP.SetVit(  CalcuSummonTransFormAbility( _pSummon->clIP.GetVit(),  siTransformAbility ) ); 
			clSrcSummon.clIP.SetDex(  CalcuSummonTransFormAbility( _pSummon->clIP.GetDex(),  siTransformAbility ) );  
			clSrcSummon.clIP.SetMag(  CalcuSummonTransFormAbility( _pSummon->clIP.GetMag(),  siTransformAbility ) );  
			clSrcSummon.clIP.SetMana( CalcuSummonTransFormAbility( _pSummon->clIP.GetMana(), siTransformAbility ) ); 
			clSrcSummon.clIP.SetLife( CalcuSummonTransFormAbility( _pSummon->clIP.GetLife(), siTransformAbility ) ); 
	
			*_pTransformSummon = &clSrcSummon;
		}
	}
#endif
}

SI32 cltCharServer::CalcuSummonTransFormAbility( IN SI32 _siStatus, IN SI32 _siAbility )
{
	if( 10 < _siAbility	) return _siStatus * (_siAbility / 10);
	else				  return _siStatus / _siAbility;
}

void cltCharServer::TransformSummonStaminaUp( cltCharServer* pclParentChar, SI32 amt )
{
	cltSummon* pclSummon = pclParentChar->pclCI->clSummonInfo.GetSummon( pclParentChar->siSummonIndex );
	if( pclSummon ) 
	{
		cltServer* pclServer = (cltServer*)pclClient;
		if( pclServer->GetSummonTransformManager()->IsTransformSummonInfo( pclSummon->siKind ) )
		{
			pclSummon->m_siTranformStamina += amt / 10;

			// �ִ�ġ�� ���� �ʰ� �Ѵ�.
			if( MAX_SUMMON_TRANSFORM_TIME < pclSummon->m_siTranformStamina )
				pclSummon->m_siTranformStamina = MAX_SUMMON_TRANSFORM_TIME;
		}
	}
}

void cltCharServer::TransformSummonStaminaDown( cltCharServer* pclParentChar, SI32 amt )
{
	// �ش� ��ȯ���� ���� ���׹̳ʸ� �ٿ��ش�.
	cltSummon* pclSummon = pclParentChar->pclCI->clSummonInfo.GetSummon( pclParentChar->siSummonIndex );
	if( pclSummon )
	{
		pclSummon->m_siTranformStamina -= amt;

		if( 0 >= pclSummon->m_siTranformStamina ) 
		{
			cltServer* pclServer = (cltServer*)pclClient;

			pclParentChar->TransformSummon( pclServer->pclCM, pclServer->GetSummonTransformManager() );
			pclSummon->m_siTranformStamina = 0;
		}
	}
}

//[����] - 2010�ų��̺�Ʈ ���� : ���޵� Ȯ���� ���� ������Ű���� ����
void cltCharServer::GiveFortuneCookiePiece( SI16 siRate, SI16 siCount )
{
	cltServer* pclserver = (cltServer*)pclClient;

	// �ų��̺�Ʈ ���ȿ��� ������Ű ���� ����.
	if( true == pclserver->pclEventTimeManager->InEventTime("Fortune_Event", &pclserver->sTime) )
	{
		if( pclClient->IsCountrySwitch(Switch_NewYearEvent_2010) ) 
		{
			// ������ Ȯ���� ���� ��Ű������ �������� ������ ����
			SI16 siChooseValue = 0;
			siChooseValue = rand() % 1000 + 1;

			if ( siChooseValue <= siRate )
			{
				// Ȯ���� ���Եƴٸ� �������� �������� �ش�.
				//GiveItem( 13062, siCount, 0, 0 );

				pclCM->GiveItem( GetID(), 13062, siCount, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_ADDITIONALITEM_GET, GetID() );
			}	
		}
	}
}

// ����� ���� ����Ʈ �������� ���� �մϴ�.
void cltCharServer::HuntQuestitem(SI32 siCharID, SI32 siMonsterKind)
{
	cltServer* pServer = (cltServer*)pclClient;

	SI32 siQuestClearCondition	= QUEST_CLEAR_COND_HUNT_ITEM;
	SI16 siQuestType			= 0;
	SI16 siQuestUnique			= 0;
	SI16 siPara1				= 0;
	SI16 siPara2				= 0;

	// ������� ������ȹ�� ���� ����Ʈ�� �������̶��
	if(pclCI->clQuestInfo.IsQuestGoing(siQuestClearCondition, &siQuestType, &siQuestUnique, &siPara1, &siPara2) == true)
	{
		// ����Ʈ������ ȹ�� ���� ����Ʈ�� ���������� Ȯ���մϴ�.
		HuntQuestItem_Unit* pData = pclClient->m_pHuntQuestItemMgr->FindQuestItem(siQuestUnique, siMonsterKind);

		if(pData == NULL)
			return;

		SI32 siRandValue = rand()%100 + 1;

		if(siRandValue > pData->m_siRandomRate)
			return;

		cltItem clItem;
		ZeroMemory(&clItem, sizeof(clItem));
		clItem.siUnique  =	pData->m_siItemUnique;		
		clItem.siItemNum =	1;

		SI32 rtnvalue	= 0;

		if(InputItem(pclClient->pclItemManager, &clItem,INPUTITEMPERSON_OPTION_WAR, &rtnvalue) == false)
		{
			// �κ��� ���ڸ��� ��� �߰��� �� ����. 
			if(rtnvalue == CANADDINV_FAIL_FULL)
			{
				pServer->SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, GetCharUnique());
			}
		}

		//if(0 < siQuestCount )
		//	siQuestItemNum = siQuestCount;

		//IncQuestPara(QUEST_CLEAR_COND_HUNT_ITEM, pData->m_siItemUnique,	siQuestItemNum );
	}
}

