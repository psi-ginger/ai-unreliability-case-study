//---------------------------------
// 2003/5/17 ���°�
//---------------------------------

#include "../Common/CommonHeader.h"
#include "../Server/Server.h"
#include "../NLib/NUtil.h"
#include "../common/Util/Util.h"
#include "../Server/TraceBoot.h"
// HanAuth_Lib
#ifdef USE_HANRTA
#ifdef _SERVER
#include "../HanAuth/HanRTA.h"
#endif
#endif // USE_HANRTA
// HanAuth_Lib

//USE_CNRTA
#ifdef USE_CNRTA
#ifdef _SERVER
#include "../HanAuth/HanRTA_c.h"
#endif
#endif // USE_HANRTA
//USE_CNRTA

// APEX-
#include "../Apex/ApexGoonZu.h"
// -APEX

#include "../GMSystem/GMProtocol.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "MsgRVal-Define.h"

// �ǹ�
#include "../CommonLogic/Cityhall/Cityhall.h"
#include "../CommonLogic/Bank/Bank-Manager.h"
#include "../CommonLogic/Stock/Stock.h"
#include "../CommonLogic/Market/Market.h"
#include "../CommonLogic/House/House.h"
#include "../CommonLogic/Hunt/Hunt.h"
#include "../CommonLogic/PostOffice/PostOffice.h"
#include "../CommonLogic/HorseMarket/HorseMarket.h"
#include "../CommonLogic/RealEstate/RealEstate.h"
#include "../CommonLogic/SummonMarket/SummonMarket.h"
#include "../CommonLogic/Land/Land.h"
#include "../CommonLogic/Feast/Feast.h"
#include "../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"
#include "../CommonLogic/Mine/Mine.h"
#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/BlackArmy/BlackArmy.h"
#include "../CommonLogic/Koinobory/Koinobory.h"
#include "../CommonLogic/SantaRaccoon/SantaRaccoon.h"
#include "../CommonLogic/KoinoboryFamily/KoinoboryFamily.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../CommonLogic/MagicMgr/MagicBaseInfo.h"
#include "../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"
#include "../CommonLogic/Marriage/MarriageManager.h"
#include "../CommonLogic/Marriage/WeddingManager.h"
#include "../CommonLogic/BoxItemMgr/BoxItemMgr.h"

//----------------------------
// Common
//-----------------------------
#include "Main.h"
#include "../Common/Map/Map.h"
#include "../Common/Skill/Skill-Manager.h"
#include "../Common/Order/Order.h"
#include "../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPC.h"
#include "../Common/NPCManager/NPC.h"
#include "../Common/StockDistributionMgr/StockDistributionMgr.h"
#include "../common/SystemNPC/TradeMerchantNPC/TradeMerchantNPC.h"
#include "../common/Event/Hunting/Hunting.h"
#include "../common/Event/MofuMofu/MofuMofu.h"
#include "../common/SystemNPC/TradeMerchantNPC/TradeMerchantNPC.h"
#include "Father\Father.h"
#include "../Common/Map/MapManager/MapManager.h"
#include "../common/Char/MonsterGroupManager/MonsterGroupManager.h"

#include "../Common/GameEvent/GameEvent.h"
#include "../Common/Bullet/Bullet.h"
#include "../Common/Smoke/smoke.h"

#include "IDNumTable.h"

// ��Ƽ �ý���
#include "../Common/PartyMgr/PartyMgr.h"


#include "Char\CharManager\CharManager.h"

// DailyQuest
#include "../Common/DailyQuest/DailyQuestMgr.h"
// DailyQuest2
#include "../Common/DailyQuest2/DailyQuest2Mgr.h"
// DailyQuest2
#include "../Common/DailyQuest3/DailyQuest3Mgr.h"

// [����] ���� ��ǥ �޴���
#include "../common/VoteMgr/VoteStatusMgr.h"

#include "../common/AutoHunting/AutoHunting.h"

// ���?Ȱ��
#include "../Common/Fishing/FishingMgr.h"

#include "../Common/StockOfferingManager/StockOfferingManager.h"

#include "../Common/GameMaster/GameMaster.h"
//--------------------------------------
// Server
//--------------------------------------
#include "Statistics/Statistics.h"


#include "../../Server/AuthServer/AuthMsg.h"



#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Web.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Stock.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "../../DBManager/GameDBManager_World/DBMsg-IPGClient.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Web.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Village.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Market.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Summon.h"

#include "MsgType-System.h"
#include "MsgType-Stock.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Village.h"
#include "MsgType-System.h"
#include "Msg/MsgType-NPC.h"

// ����
#include "Notice/Notice.h"

#include "../../../AdminSystem/MasterServer/MasterMsg.h"
// HashTable
#include "HashTable/HashTable_StringInt.h"
#include "HashTable/HashTable_IntInt.h"
#include "Minister/MinisterMgr.h"
#include "Minister/Minister-Goonzu.h"
#include "Minister/Minister-Yezo.h"
#include "Minister/Minister-Byungzo.h"

#include "Event/KingGuri/KingGuri.h"
#include "Event/GoonzuKingGuri/GoonzuKingGuri.h"// [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
#include "Empire/EmpireManager.h"

#include "ServerReward/ServerReward.h"
#include "ServerBoxItem/ServerBoxItem.h"
#include "GachaManager/GachaManager.h"
#include "GawiBawiBoMgr/GawiBawiBoMgr_Server.h"
#include "PartyQuestMgr/PartyQuestMgr_Server.h"

extern cltMapCommon* OrgAddress[];

NMsgShareBuffer g_msgsharebuffer;

cltServer::cltServer(TCHAR* appname, HWND hwnd, HINSTANCE hinst, SI32 gamemode, SI32 servicearea, SI32 runmode) : cltCommonLogic(appname, hwnd, hinst, gamemode, servicearea, runmode)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// �Ѱ��� ���񽺿� ������ ȹ���Ѵ�.
	GetHanGameInfo();

	// -HanAuth_Lib
#ifdef USE_HANRTA

	if(servicearea == ConstServiceArea_USA)
	{
		m_kHanRTA_Timer.Init(1 * 60 * 1000);		// 1��
	}
	else
	{
		m_kHanRTA_Timer.Init(5 * 60 * 1000);		// 5��
	}


//#ifdef _KOREA
//	if(siServiceType == SERVICE_ALPHA)
//		HanRTAInit("K_GOONZU", SERVICE_KOR | SERVICE_ALPHA, 2);
//	else
//		HanRTAInit("K_GOONZU", SERVICE_KOR | SERVICE_REAL, 2);
//#endif

#ifdef _USA
	if(siServiceType == SERVICE_ALPHA)
		HanRTAInit("U_GOONZU", SERVICE_USA | SERVICE_ALPHA, 2);
	else
		HanRTAInit("U_GOONZU", SERVICE_USA | SERVICE_REAL, 2);
#endif
	m_kHanRTA_Timer.SetActive(true, ::GetTickCount());
#endif // USE_HANRTA

	// USE_CN_RTA
#ifdef USE_CNRTA
	// 5�п� �ѹ� ���� �ʱ�ȭ
	if ( servicearea == ConstServiceArea_NHNChina )
	{
		m_kCNRTA_Timer.Init(5 * 60 * 1000);		// 5��
	}
	HanRTAInit("C_GZ");
	m_kCNRTA_Timer.SetActive(true, ::GetTickCount());
#endif
	// USE_CN_RTA


#ifdef USE_HANXNOTI

	InitializeCriticalSection(&m_HanXNoti_CS);

	for(SI32 i=0; i<HANXNOTI_MAX_WAITING; i++)
	{
		m_kHanXNoti_Waitings[i].m_hWaitingHandle = CreateEvent(NULL, FALSE, FALSE, "HANXNOTI_EVENT");
		m_kHanXNoti_Waitings[i].m_siIdx = -1;

		m_pkHanXNoti_AnsMessages[i] = NULL;
	}

#endif // USE_HANXNOTI

#ifdef USE_HANREPORT

#ifdef _KOREA
	if(siServiceType == SERVICE_ALPHA)
		;//HanReportInit("K_GOONZU", SERVICE_KOR | SERVICE_REAL, 10);		// ���Ŀ��� ���� ���� ����.
	else
		;//HanReportInit("K_GOONZU", SERVICE_KOR | SERVICE_REAL, 10);		// ���󿡼��� ���� ����
#endif

#ifdef _USA
	if(siServiceType == SERVICE_ALPHA)
		HanReportInit("U_GOONZU", SERVICE_USA | SERVICE_ALPHA, 10);
	else
		HanReportInit("U_GOONZU", SERVICE_USA | SERVICE_REAL, 10);
#endif

#endif // USE_HANREPORT

#ifdef USE_HANIPCHECK

	#ifdef _KOREA
		HanIPCheckInit( "K_GOONZU" );//, SERVICE_KOR | SERVICE_REAL , true );
	#endif

	#ifdef _USA
		if(siServiceType == SERVICE_ALPHA)
			HanIPCheckInit( "U_GOONZU", SERVICE_USA | SERVICE_ALPHA, true );
		else
			HanIPCheckInit( "U_GOONZU", SERVICE_USA | SERVICE_REAL, true );
	#endif

	#ifdef _NHN_CHINA
		HanIPCheckInit( "C_GZ" );
	#endif

#endif // USE_HANIPCHECK
	// -HanAuth_Lib

#if defined (USE_HAN_BAN_CHECK)
	// �Ѱ��� ĳ���� �������� üũ => 2008-8-25
#ifdef _DEBUG
	int ret = HanBanCheckInit("U_GOONZU", SERVICE_ALPHA | SERVICE_USA);
#else
	int ret = 0;
	if(siServiceType == SERVICE_ALPHA)
		ret = HanBanCheckInit("U_GOONZU", SERVICE_ALPHA  | SERVICE_USA);
	else
		ret = HanBanCheckInit("U_GOONZU", SERVICE_REAL  | SERVICE_USA);
#endif

	if (HAN_BANCHECKSVR_OK != ret)
	{
		MessageBox( NULL, "HanBanCheckInit() Fail", "Fail", NULL );
	}
#endif

	bZeroStatistics		= FALSE;
	bWriteStatistics	= FALSE;
	bListenOK			= FALSE;

	g_msgsharebuffer.CreateMsgShareBuffer( 2048, TEXT("GOONZU") );

	// �޽������� ����Ѵ�?
	RegisterServerMsgMap();
	RegisterDBMsgMap();

	SI32 i;

	pclFrontAuthProcess	= NULL;
	pclGameDBProcess	= NULL;
	pclGameDBProcess2	= NULL;
	pclLogDBProcess		= NULL;
	pclAgentProcess		= NULL;
	pclBillProcess		= NULL; 
	pclNTradeServerProcess = NULL;
	pclGMProtocol = NULL;

	pclIDNumTable		= NULL;

	pclWeather			= NULL;

	pclStockOfferingManager = NULL;



	bServerNetoworkStartSwitch	= false;
	bGameTimeRequestSwitch		= false;
	bGetTimeSwitch				= false;

	bMapTypeInfoRequestSwitch	= false;	// ��Ÿ ���� ��û������ Ȯ���Ѵ�. 
	bGetMapTypeInfoSwitch		= false;		// �� Ÿ�������� �޾����� Ȯ���Ѵ�. 


	bForcePersonalShopSwitch	= true;		// �������ڸ��� ���λ��� ���� 
	bForceVillageWarSwitch		= false;
	bForceTimeSwitch			= false;
	bAutoWarSwitch				= true;

	dwVillageWarTimeMin			= 40;

	// pvp-
	siLoseItem         = 0;
	siRegenerateLifeValue = 0;
	siBaenNyeonSamRate = 200;
	siHandDamageRate = 0;
	siSwordDamageRate = 0;
	siSpearDamageRate = 0;
	siAxeDamageRate = 0;
	siBowDamageRate = 0;
	siGunDamageRate = 0;
	siStaffDamageRate = 0;
	siPKMapExpRate	= 0;
	bGuildWar	= false;

	bBankruptcy = false;
	siStartBankruptDateVary = 0;
	siItemmallRPRate = 0;
	siAttendCheckMinute = 60; // [����] �⼮üũ : �⼮üũ �ð�

	bNoChargeGetItemFromNPC = false;

	pclKingGuriEventManager			= NULL;
	pclGoonzuKingGuriEventManager	= NULL; // [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
	pclSantaRaccoon					= NULL;	// [�߰� : Ȳ���� 2007. 11. 29 ��Ÿ���� �̺�Ʈ ������ NULL��]

	//pclKingGuriEventManager = new CKingGuriEvent;
	pclEmpireManager = new CEmpireManager;

	dwVillageWarStartHour			= 0;
	dwVillageWarFinishHour			= 0;
	dwVillageWarStartMin			= 0;
	dwVillageWarFinishMin			= 0;

	if( siServiceArea == ConstServiceArea_English) 
	{
		dwVillageWarStartHour		= 18;		// �۷ι��̸� ������	���� 1��
		dwVillageWarFinishHour		= 19;		// �۷ι��̸� ������	���� 1��
	}
	//[����] USA ���� �ð� ����. => 2008-7-3 => �ڵ� ��ġ ���� �ռ���
	else if ( siServiceArea == ConstServiceArea_USA )
	{
		dwVillageWarStartHour		= 18;
		dwVillageWarFinishHour		= 18;
	}
	else if( siServiceArea == ConstServiceArea_EUROPE) 
	{
		dwVillageWarStartHour		= 1;		// �����̸� ������	���� 1��
		dwVillageWarFinishHour		= 1;		// �����̸� ������	���� 1��
	}
	else 
	{
		dwVillageWarStartHour		= 21;		// �۷ι��� �ƴϸ� ������	���� 9��.
		dwVillageWarFinishHour		= 21;
	}

	dwVillageWarStartMin		= 0;
	dwVillageWarFinishMin		= dwVillageWarStartMin + dwVillageWarTimeMin;


	//------------------------------
	// Ÿ�̸�
	//------------------------------
	m_clUpdateTimer_Minute.Init(60 * 1000);			// 1�� Ÿ�̸�

	//------------------------------
	//cyj ���� ���� ���� ����
	//------------------------------
	dwSpecialNews				= 0;
	dwGeneralMeetingNewsMin		= 0;
	dwVillageWarNewsMin			= 0;
	dwBlackArmyNewsHour			= 0;

	bProcessDB_WealthRank_NEED	= true;
	bProcessDB_TotalMoney_NEED	= true;
	bProcessDB_TotalChar_NEED	= true;

	bProcessDB_GetMinister_NEED = true;

	bProcess_InitCalcCharNum = true ;

	bProcess_InitGameEvent = true ;

	bProcessDB_TotalVillageScore_NEED = true;

	bProcessDB_AutoPayDividend_NEED = true;

	bProcess_SetNowVillageWarInfo = true;

	bCalcInvestment = false ;

	memset( &clServerStartInfo, 0, sizeof( cltServerStartInfo ) );

	pclCheckProcessManager			= NULL;
	pclRewardMgr					= NULL;
	pclNewSelectBoxMgr				= NULL;

	pclPromoterManager				= NULL;
	m_pclSummonTransformManager		= NULL;		

	m_pclBattleRoyalMgr				= NULL;
	m_pclGiantRaccoonResurrectEvent = NULL;

	//m_pclElixirManager			= NULL;

	// KHY - 0901 -  ���� ������ �ڵ����� ����. - serverinfo�� �б����� �ʱ�ȭ �����ش�.
	siMarketMoneyItemAutoBuyDelayTime = 	0; // 1���� �⺻����.
	siMarketMoneyItemMaxpriceNow		=	0;


	// ���� ������ ������ ȹ���Ѵ�. 
	GetServerInfo();

#ifdef USE_HANXNOTI
	m_pkHanAuth_XNotiConnector = XNoti::Connector::CreateHandle();
	if(m_pkHanAuth_XNotiConnector != NULL)
	{
#ifdef _KOREA
		// mrs 2.0
		/*if( XNoti::Connector::Initialize(m_pkHanAuth_XNotiConnector, "XNOTI-PGMT5", "K_GOONZU", m_szMRSServerID, 
		XNoti::Connector::EnumNetType_Multicast, 0, 10) )*/

			// mrs 3.0
			if( XNoti::Connector::Initialize(m_pkHanAuth_XNotiConnector, "X:0:8000:0:0:0", "K_GOONZU", m_szMRSServerID, 0, 10) )
#else

		if( XNoti::Connector::Initialize(m_pkHanAuth_XNotiConnector, "XNOTI-PGMT5", "U_GOONZU", m_szMRSServerID, 
			XNoti::Connector::EnumNetType_Multicast, 0, 10) )
#endif
		{
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(GMTMessageReq_K_GOONZU),	HanXNoti_DoMsg_GMTMessageReq_K_GOONZU, this);
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(GMTKickReq_K_GOONZU),		HanXNoti_DoMsg_GMTKickReq_K_GOONZU, this);
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(GMTChatBlockReq_K_GOONZU),	HanXNoti_DoMsg_GMTChatBlockReq_K_GOONZU, this);
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(GMTCmdReq_K_GOONZU),		HanXNoti_DoMsg_GMTCmdReq_K_GOONZU, this);

			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(GameCharacterListReq),		HanXNoti_DoMsg_GameCharacterListReq, this);
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(AddInventoryItemReq),		HanXNoti_DoMsg_AddInventoryItemReq, this);
			XNoti::Connector::RegistrationMassageHandler(m_pkHanAuth_XNotiConnector, __messageid(DeleteInventoryReq),		HanXNoti_DoMsg_DeleteInventoryReq, this);

		}
		else
		{
			MsgBox("HanAuth", "Fail to init XNotiConnector");
		}
	}
	else
	{
		MsgBox("HanAuth", "Fail to create XNotiConnector");
	}
#endif // USE_HANXNOTI
	// -HanAuth_Lib

	bAutoStockDistriubtionSwitch= false;

	bHorseCurrentPriceSet = false;

	bGMBonusTimeSwitch = 0;

	siHorseTradeCount = 0;
	siHorseCurrentPrice = 0;

	for ( i = 0 ; i < 10 ; i ++ )
		siTopLevelVillageUnique[i] = 0 ;

	pclItemList			= NULL;

	m_lastFrame			= 0;

	siServerID			= 0;

	//------------------------------
	// ���� ������. 
	//------------------------------
	pclNoticeManager	= NULL;

	//------------------------------
	// CHAR NAME���� �˻��� ���� HASH TABLE
	//------------------------------
	pclHashTable		= NULL;

	//------------------------------
	// PERSON ID���� �˻��� ���� HASH TABLE
	//------------------------------
	pclPersonHashTable	= NULL;

	//------------------------------------
	// PersonName ������. 
	//------------------------------------
	pclPersonNameManager = NULL;
	pclNewPersonNameManager = NULL;

	//------------------------------------
	// IPToCountry ������.
	//------------------------------------
	pclIPToCountryMgr = NULL;

	//------------------------------------
	// AccessDeny ������.
	//------------------------------------
	pclAccessDenyMgr = NULL;

	//--------------------------------
	// �����Ǽ� 
	//-----------------------------------
	pclMinisterMgr		= NULL;

	siChatCount_Private	= 0;
	siChatCount_General = 0;

	//--------------------------------
	// �����?���� ������
	//--------------------------------
	pclGuildWarReserveManager = NULL;

	for ( i = 0; i < GUILDWAR_FIELDNUM ; i++ )
		pclGuildWarManager[i] = NULL;

	pclGuildDungeonMgr = NULL;

	pclBookBox = NULL;
	pclGodHandManager = NULL;


	//-------------------------------
	// ������ ��. 
	//-------------------------------
	siTodayMaxUserNum	= 0;		// ������ �ְ� ����. 
	siCurUserNum		= 0;			// ���� ����. 
	si1To19UserNum		= 0;
	si20To39UserNum		= 0;
	si40To59UserNum		= 0;
	si60To79UserNum		= 0;
	si80To99UserNum		= 0;
	si100To119UserNum	= 0;
	si120To150UserNum	= 0;

	siTotalNewCharNum	= 0;
	siTotalDelCharNum	= 0;

	siServerNewCharNum	= 0;
	siServerDelCharNum	= 0;

	siNewCharNum		= 0;		// ���ο� ĳ���� ���� ����
	siDelCharNum		= 0;		// ĳ���� ���� ����
	siPCRoomCharNum		= 0;

	//KHY - 0708 - �Ϻ� ����͸�?Ŭ���̾�Ʈ ����.
	siHangameCharNum	= 0;
	siGamaniaCharNum	= 0;
	siGamePotCharNum	= 0;	
	siNDECharNum		= 0;	
	siNEXONCharNum		= 0;	

	siGlobalNum			= 0;	// �۷ι� ������ ��.
	siNHNNum			= 0;	// NHN ������ ��.

	//	siNetmarbleCharNum  = 0;

	//-------------------------------
	// ���� ���?������
	//-------------------------------
	siWndInterfaceNum= 0;				// ���� ������ �����?�� ������â + ������ �������̽� �����?������
	siWnd_NewInterfaceNum= 0;			// ���� ������ �����?�� ������â + ���������̽� �����?������
	siFullWnd_NewInterfaceNum= 0;		// ���� ������ �����?�� ��üȭ�� + ���������̽� �����?������

	//-------------------------------
	// �����̾� �ð��� 
	//-------------------------------
	ZeroMemory(&sTime, sizeof(_SYSTEMTIME));
	ZeroMemory(&sLastNewsTime, sizeof(_SYSTEMTIME));


	//---------------------------------
	// DB �Ŵ������� ������ ���� �ð�. 
	//---------------------------------
	dwLastDBClock		= 0;

	//--------------------------------
	// ���� �ֽ� �ֹ� ��û. 
	//--------------------------------
	dwLastStockOrderClock	= 0;
	siLastStockOrderIndex	= 0;

	//--------------------------------
	// DB�����ð�. 
	//--------------------------------
	dwDBLatency				= 0;

	pclTime = NULL;



	//--------------------------------
	// ���� ���� ���? 
	//--------------------------------
	pclStatisticsManager	= NULL;


	//--------------------------------
	// server refresh info
	//--------------------------------
	pRefreshInfo = new sGameServerRefreshInfo;

	//--------------------------------
	// �Ķ��Ÿ��?���� ĳ���� ����Ʈ��. 
	//-----------------------------------
	siParaCharListIndex			= 0;
	for(i = 0;i < MAX_CHAR_NUMBER;i++)
	{
		siParaCharList[i] = 0;
	}


	for( i = 0; i < MAX_VILLAGE_NUMBER; ++i ) {
		siSlaveVillageNum[ i ] = -1;
	}


	for( i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		//bCompleteDBInit[ i ] = FALSE;
		siCompleteDBInit[ i ] = DBINITIALIZE_STEP_NONE;
	}
	siCurrentDBInit	= DBINITIALIZE_STEP_NONE;
	bCompleteDBInitAll = FALSE;
	bSetVillageLevel = FALSE;
	bSetTopLevelVillage = FALSE;

	//-----------------------------------
	// Ư���� �� ����. 
	//-----------------------------------
	bDoubleBonusDaySwitch	= false;		// ����, ���� ����ġ ���ʽ� Ÿ���� �ι谡 �Ǵ� ��. 

	bSpecailDay1Switch	= false;			// ������. 
	bSpecailDay2Switch	= false;			// ������. 
	bSpecailDay3Switch	= false;			// �����ǳ� . 
	bSpecailDay4Switch	= false;			// ���ֿ��±����?
	bSpecailDay5Switch	= false;			// ũ��������.
	bSpecailDay6Switch	= false;			// 3.1��
	bSpecailDay7switch	= false;			// �湫�� ź����
	bSpecailDay8switch	= false;			// ��̳�
	bSpecailDay9switch  = false;			// �ٴ��� ��

	bInstanceGate		= true;

	bRouletteGame		= false;

	//----------------------------------
	// ��ι�?����Ʈ �Ϸ��� ����. 
	//----------------------------------
	siSpecialQuestBonusBufferIndex	= 0;

	//----------------------------------
	// ĳ�� ���� ���� ����.
	//----------------------------------
	PushCashListIndex				= 0;


	//----------------------------------
	// �ڵ����?����ġ
	//----------------------------------
	bAutoPayDividend = true;

	// ���� �����Ѱ�?
	bTradeServer = true ;
	bForceStopTradeProcess = false;

	// ������ �����Ѱ�?
	bForceStopVillageWarProcess = false;

	// ���� ���� �ð� �����ϱ� - �⺻ �ð��� ���� 10�ú��� ���� 12�ñ����̴�
	// �۷ι� ������ GMT������!!..
	if ( siServiceArea == ConstServiceArea_English )
	{
		siTradeServerStartTime = 1 ;
		siTradeServerEndTime = 15 ;
	}
	else
	{
		siTradeServerStartTime = 10 ;
		siTradeServerEndTime = 24 ;
	}

	siLimitMaxUserNumber = MAX_PERSON_NUMBER;

	//-------------------------------------
	// ���� ���?����
	//-------------------------------------
	siSpecialNewsBufferIndex			= 0 ;
	siSpecialNewsSendIndex				= 0 ;
	for ( i = 0 ; i < MAX_SPECIAL_NEWS_BUFFER_SIZE ; i ++)
	{
		ZeroMemory(szNewsTitleBuffer[i],MAX_NEWS_TITLE_LENGTH);
		ZeroMemory(szNewsNameBuffer[i],MAX_NEWSPAPER_NAME_LENGTH);
		ZeroMemory(szNewsURLBuffer[i],MAX_PATH);
	}

	//-------------------------------------
	// GM ���ʽ� Ÿ��
	//-------------------------------------
	bIsGMBonusTime				= false;
	bIsGMBonusTime2				= false;

	siGMBonusTimePercent		= 0;
	siGMBonusTime2Percent		= 0;

	bBonusTimeGoonzu			= false;

	bDoNotAutoBonus				= false;

	ZeroMemory( &m_gameserverinfo, sizeof(GAMESERVER_GAMESERVERINFO) );
	m_bGameserverinfo = FALSE;

	m_bToSendServerInfoToFront = FALSE;

	InitializeCriticalSection( &cs_refresh );

	siLastVillageWarKillNumClearDay = -1;

	//[�߰� : Ȳ���� 2007. 11. 19 �����?���� ������ �ŷ� ���� ����.]
#ifdef _SAFE_MEMORY
	clItemInfo.ZeroMem();
#else
	ZeroMemory(clItemInfo, sizeof(clItemInfo));
#endif

	//[�߰� : Ȳ���� 2007. 11. 26 �ý��ۿ��� ������ ���忡 �ø� ������ ���Ű��� ���� ����.]
	ZeroMemory(&clSystemBuyItem, sizeof(clSystemBuyItem));

	// [����] Auto Unblock Ÿ�̸� ���� - 2008.02.21
	m_kUpdateTimer_AutoUnblock.Init( 5 * 60 * 1000 ); // ���� �⵿ �� 5���� ������ ����(�ѹ���)

	bAlldayBonus = false;

	gForeignMerchantBuyLimit = 0;	// �����?���� 1�� ���� ���� �ݾ�
	gForeignMerchantNowMoney = 0;	// �����?���� 1�� ���� �ݾ��� ����.
	siYesterday = 0;

	siNDEScrollRate = 1;

	siFourLeafUnique		= 0;

	fForeignMerchantPercentUP	=	0	;	// �����?���� �������ѱݾ� ������ 
	fForeignMerchantPercentDOWN	=	0	;	// �����?���� �������ѱݾ� ������ 

	// KHY - 0901 -  ���� ������ �ڵ����� ����.
	for(SI32 i=0; i<MAX_SYSTEMBUYTEM_NUM; i++)
	{
		stSystemBuyItem[i].m_siUnique = 0;
		stSystemBuyItem[i].m_gPrice= 0;
	}


	// KHY - 0901 -  ���� ������ �ڵ����� ����.
	GetServerSystemBuy();	

	// 200ms���� ȣ���?���� - by LEEKH
	m_kUpdateTimer_PVP_League.Init( 1000 )	; //[����] ���� ������10�ʸ��� ȣ���Ѵ�.. 1������ �ϴϱ� 10�� ������ ���°� �ȵǳ�''; 

	m_kUpdateTimer_TreasureEvent_Respon_Npc.Init( 60000 * 10)	;// ����ã�� �̺�Ʈ

	// [����] ���?�����?�������?���� �Ѵ�.
	m_kUpdateTimer_GuildHuntMapWar.Init(1000)	;
	
	// [ ���� ]	1�и��� ȣ�� �Ѵ�. ��¥�� üũ�ؼ� ��í ���� ������ ����ũ�� ���� �Ѵ�.
	m_kUpdateTimer_GachaRareItmeUnique_Timer.Init( 1 * 60 * 1000)	; // 1��

	m_kUpdateTimer_RacoonDodgeEvent.Init(100)	; // ���� ���ϱ� �̺�Ʈ �ð� �ʱ�ȭ -> Ư���� 0.5�ʸ��� �ѹ��� ���� Ȯ�� �ؾ� �Ѵ�. //Switch_10MonthEvent_RacoonDodge

	siTotalMoney			=	0;

	m_uiCompressMinSize = 0;

	bMakeTreasureEventNPC	=	false	;// ����ã�� �̺�Ʈ ���� �蹶ġ ���� �������� �ƴ��� �������� �� �ѹ��� �����?
	bReadTreasureEvent_Resource	=	false	;

	bGachaLock	=	false	;
	siGMChangeGachaRareUnique	=	0;
	bGachaRareitemStartSet	=	false	;
 
	InitializeCriticalSection( &m_cs_SendNetMsg );

	m_pGawiBawiBoMgr	= NULL;
	m_pPartyQuestMgr	= NULL;
	
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::Create()
{
	TraceBoot("cltServer::Create ENTER");
	//----------------------------------------------------------------
	// ������������������ ������
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------
	TraceBoot("cltServer::Create _SERVER branch entered");

	pclGMProtocol = new GMProtocol;

	pclGMProtocol->Init( 11400 );
	pclGMProtocol->CreateThread();
	pclGMProtocol->GetListen()->Start();


	SI32 i ;

	SetGameStatus(GS_GAME);
	TraceBoot("cltServer::Create after SetGameStatus(GS_GAME) gs=%d", GetGameStatus());

	cltCommonLogic::Create();

	// [FIX-race-crash] Allocate the game clock BEFORE any asynchronous DB reply can dispatch.
	// ServerAction fires DBREQUEST_GETTIME as soon as the DB link establishes, and its reply
	// handler (DoMsg_DBMSG_RESPONSE_GETTIME) writes pclTime->clDate. If that reply lands while
	// Create is still inside the map-type wait loop below, pclTime is still NULL (it used to be
	// allocated later in this function) -> access violation at ServerDB-System.cpp:1057.
	// cltTime() is dependency-free (time.cpp:238).
	if(pclTime == NULL)
	{
		pclTime = new cltTime();
		TraceBoot("cltServer::Create pclTime allocated early (before DB reply dispatch)");
	}

	// [FIX-boot-deadlock] The server's map list (clUnitMapInfo[]) and its pclCurrentMap are BOTH
	// DB-derived, and the ONLY DB request that fills them (RequestMapTypeFromDB) lives behind
	// ServerAction, which itself is gated on pclCurrentMap!=NULL (CommonLogic-Process.cpp:27).
	// That self-deadlock is now broken by issuing the DB map-list request HERE at boot and
	// waiting for its async reply BEFORE the first MAINLOOP tick, so GameProcess's gate opens.
	if(pclMapManager && pclMapManager->pclCurrentMap == NULL)
	{
		// Kick the (async, DB-side) map-list request: fills clUnitMapInfo[] then sets
		// bGetMapTypeInfoSwitch=true when the DB reply lands.
		RequestMapTypeFromDB();
		TraceBoot("cltServer::Create requested server map-type from DB -> waiting for reply");

		// Poll for the async DB reply (clUnitMapInfo[] filled / bGetMapTypeInfoSwitch==true),
		// bounded so a dead/empty DB cannot hang boot forever.
		SI32 nWait = 0;
		while(bGetMapTypeInfoSwitch == false && nWait < 1500)
		{
			Sleep(10);
			nWait++;
			if((nWait % 200) == 0)
				TraceBoot("cltServer::Create waiting map-type reply nWait=%d bGetMapTypeInfoSwitch=%d", nWait, bGetMapTypeInfoSwitch);
		}

		// Reply arrived -> build all server maps from the now-populated list and select map 0,
		// so pclCurrentMap is non-NULL before GameProcess gates on it.
		if((bGetMapTypeInfoSwitch && pclMapManager->LoadAllMap(this)) || pclMapManager->LoadMap(this, 14, 0, 0, 0, 0, TEXT("MainMap"), FALSE, FALSE, TRUE))
		{
			pclMapManager->SetCurrentMapIndex(0);
			TraceBoot("cltServer::Create PRELOADED server map pclCurrentMap=%p index=%d (waited %d x10ms)", pclMapManager->pclCurrentMap, pclMapManager->CurrentMapIndex, nWait);
		}
		else
		{
			TraceBoot("cltServer::Create map preload FAILED bGetMapTypeInfoSwitch=%d (waited %d x10ms) - boot will stay gated", bGetMapTypeInfoSwitch, nWait);
		}
	}
	else
	{
		TraceBoot("cltServer::Create skip map preload (pclMapManager=%p pclCurrentMap=%p)", pclMapManager, pclMapManager?pclMapManager->pclCurrentMap:NULL);
	}

	// ������ ������.
	pclRankManager = new cltRankManager();

	// ��ũ��
	m_pclMacroReportedMgr	=	new cltMacroReportedMgr;
	m_pclMacroReportedMgr->Initialize();

	m_pclGZFtpMgr			=	new cltGZFtpMgr;
	m_pclGZFtpMgr->Initialize();

	// ���� ������. 
	pclWeather = new CWeather();

	// ������ ������ ������. 
	pclItemList	= new ItemList();
	if(pclItemList)
	{
		pclItemList->CreateItemList(20000);
	}

	//------------------------------
	// ���� ������. 
	//------------------------------
	pclNoticeManager = new cltNoticeManager;

	//------------------------------
	// CHAR NAME ���� �˻��� ���� HASH TABLE
	//------------------------------
	pclHashTable	= new HashStringInt(1000, MAX_PERSON_NUMBER);

	//------------------------------
	// PERSON ID ���� �˻��� ���� HASH TABLE
	//------------------------------
	pclPersonHashTable = new HashIntInt(1000, MAX_PERSON_NUMBER);


	//------------------------------
	// IDNum ���� �˻��� ���� HASH TABLE
	//------------------------------
	pclIDNumTable = new cltIDNumTable;

	//------------------------------------
	// PersonName ������. 
	//------------------------------------
	pclPersonNameManager	= new cltPersonNameManager(2000, 10000);
	pclNewPersonNameManager = new cltNewPersonNameManager(2000, 10000);

	//------------------------------------
	// IPToCountry ������.
	//------------------------------------
	pclIPToCountryMgr = new cltIPToCountryMgr();

	//------------------------------------
	// AccessDeny ������.
	//------------------------------------
	pclAccessDenyMgr = new cltAccessDenyMgr();

	pclStockOfferingManager = new cltStockOfferingManager;

	// ������ ���� ��������ġ �߰�
	if(IsCountrySwitch(Switch_EnableVillageWar))
		bForceStopVillageWarProcess = false;
	else
		bForceStopVillageWarProcess = true;

	//--------------------------------
	// �����Ǽ� 
	//-----------------------------------
	pclMinisterMgr = new CMinisterMgr();

	//--------------------------------
	// �����?���� ������
	//--------------------------------
	pclGuildWarReserveManager = new cltGuildWarReserveManager();

	//---------------------------
	// �����?
	//--------------------------
	for ( i=0; i<GUILDWAR_FIELDNUM; i++ )
	{
		pclGuildWarManager[i] = new cltGuildWarManager();
	}

	pclGuildDungeonMgr = new cltGuildDungeonMgr;

	//-----------------------------
	// å����
	//-----------------------------
	pclBookBox = new cltBookBox;
	pclBookBox->SetFromFile();

	//-----------------------------
	// ���ڵ�
	//-----------------------------
	//	pclGodHandManager = new CItemGodHandManager;

	
	if(IsCountrySwitch(Switch_RaccoonEvent_Change_Goonzu))// [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
	{
		pclGoonzuKingGuriEventManager = new CGoonzuKingGuriEvent;

		if (pclGoonzuKingGuriEventManager)	// ���� ó�� ���鼭 ���� �ϰ� �����͸� �ε� �Ѵ� .
		{
			pclGoonzuKingGuriEventManager->LoadData()	;
		}
		else
		{
			MessageBox( NULL, TEXT("pclGoonzuKingGuriEventManager = new CGoonzuKingGuriEvent;"), "Fail", 0 );
		}
	}
	else
	{
		if(pclClient->IsCountrySwitch(Switch_KingGuriEvent))
		{
			pclKingGuriEventManager = new CKingGuriEvent;
		}
	}
	//if(pclClient->IsCountrySwitch(Switch_SantaRaccoonEvent))
	{
		pclSantaRaccoon	= new CSantaRaccoon;			
	}
	//---------------------------------
	// DB �Ŵ������� ������ ���� �ð�. 
	//---------------------------------
	dwLastDBClock		= 0;

	//--------------------------------
	// ���� �ֽ� �ֹ� ��û. 
	//--------------------------------
	dwLastStockOrderClock	= 0;
	siLastStockOrderIndex	= 0;

	//--------------------------------
	// DB�����ð�. 
	//--------------------------------
	dwDBLatency				= 0;

	//--------------------------------
	// �Ķ��Ÿ��?���� ĳ���� ����Ʈ��. 
	//-----------------------------------
	siParaCharListIndex			= 0;
	for(i = 0;i < MAX_CHAR_NUMBER;i++)
	{
		siParaCharList[i] = 0;
	}
	
	//--------------------------------
	// ��Ʋ�ξ� 
	//-----------------------------------
	if ( pclClient->IsCountrySwitch(Switch_BattleRoyal) == true )
	{
		m_pclBattleRoyalMgr	= new CBattleRoyalMgr();
		if( NULL != m_pclBattleRoyalMgr )
		{
			m_pclBattleRoyalMgr->Create();
		}
	}

	//--------------------------------
	// �߷�Ÿ�� ���� �̺�Ʈ
	//-----------------------------------
	if ( pclClient->IsCountrySwitch(Switch_ValentineDayEvent) == true )
	{
		m_pclValentineEventMgr	= new CValentineEventServerMgr();
		if( NULL != m_pclValentineEventMgr )
		{
			m_pclValentineEventMgr->Create();
		}
	}

	// ��ο�?���õ� �̺�Ʈ�� ���?���� ������ �ʱ�ȭ�� �Ϸ��?�Ŀ� ���� �� �ʱ�ȭ.
	// ���?���õ� �̺�Ʈ�� ���⼭ �ʱ�ȭ ����.
	if( m_clContributionEvent_Common.FileLoad() )
	{
		//-----------------------------------
		// ���̾�Ʈ ���� ��Ȱ ����. 
		//-----------------------------------
		m_pclGiantRaccoonResurrectEvent = new cltGiantRaccoonResurrectEvent;
		if( NULL == m_pclGiantRaccoonResurrectEvent )
		{
			MessageBox(NULL, "Error", "new cltGiantRaccoonResurrectEvent Fail", 0);
		}
	}
	
		
	//--------------------------------
	// ���� ���� ���? 
	//--------------------------------
	pclStatisticsManager	= new cltStatisticsManager;
	pclStatisticsManager->Init();


	pclAddMoneyStatistics		= new cltAddMoneyStatistics;
	pclDelMoneyStatistics		= new cltDelMoneyStatistics;

	//--------------------------------
	// Ÿ�̸�
	//--------------------------------
	m_clUpdateTimer_Minute.SetActive(true, ::GetTickCount());
	// [����] Auto Unblock 
	m_kUpdateTimer_AutoUnblock.SetActive( true, ::GetTickCount() );

	pclCheckProcessManager		= new cltCheckProcessManager;
	if ( pclCheckProcessManager )
	{
		pclCheckProcessManager->LoadMacroList();
	}

	pclRewardMgr				= new cltRewardMgr;
	if ( pclRewardMgr )
	{
		pclRewardMgr->LoadRewardList();
	}

	pclNewSelectBoxMgr			= new cltNewSelectBoxMgr;
	if ( pclNewSelectBoxMgr )
	{
		pclNewSelectBoxMgr->LoadNewSelectBoxItemList();
	}

	//  [8/11/2009 ��ȣ_GAWIBAWIBO]
	if(pclClient->IsCountrySwitch(Switch_GawiBawiBo) == true)
		m_pGawiBawiBoMgr = new CGawiBawiBoMgr_Server;

	if(m_pGawiBawiBoMgr != NULL)
		m_pGawiBawiBoMgr->Initialize();

#ifdef USE_PROMOTOR_SERVER	
	pclPromoterManager = new clPromoterManager;
	if ( NULL == pclPromoterManager )
	{
		MessageBox( NULL, TEXT("pclPromoterManager = new clPromoterManager"), "Fail", 0 );
	}
#endif

	if ( IsCountrySwitch(Switch_WeddingSystem) )
	{
		if ( m_pclMarriageMgr )
		{
			m_pclMarriageMgr->Init();
		}
	}

	if ( IsCountrySwitch(Switch_SummonTramsform) )
	{
		m_pclSummonTransformManager = new cltSummonTransformManager;
		if ( m_pclSummonTransformManager )
		{
			m_pclSummonTransformManager->Init();
		}
		else
		{
			MessageBox( NULL, TEXT("m_pclSummonTransformManager = new cltSummonTransformManager"), TEXT("Fail"), 0 );
		}
	}

	if(IsCountrySwitch(Switch_PartyQuest_PartyMatching))
	{
		m_pPartyQuestMgr	=	new CPartyQuestMgr_Server;
		m_pPartyQuestMgr->Initialize();
	}


	// �ڽ� ������ ������ ���� �������� ���������� �о������?�Ѵ�
	if ( m_pclBoxItemMgr )
	{
		m_pclBoxItemMgr->LoadAllBoxItemSet();
	}

	// APEX-
#ifdef _CHINA
	StartApexProxy();
#endif
	// -APEX

	// ��Ʋ�ʵ� ���Ǳ��� ������ ���� �б�
	pclBlackArmyManager->Init();

	m_kUpdateTimer_PVP_League.SetActive(true, ::GetTickCount() );	//[����] �������� 1�и��� üũ �ؼ� ȣ���Ѵ�.
	m_kUpdateTimer_GuildHuntMapWar.SetActive( true, ::GetTickCount() ); //// [����] ���?�����?�������?���� �Ѵ�.
	m_kUpdateTimer_TreasureEvent_Respon_Npc.SetActive(true,::GetTickCount())	;// ����ã�� �̺�Ʈ
	m_kUpdateTimer_GachaRareItmeUnique_Timer.SetActive(true,::GetTickCount())	;// ��í ���� ������ ���� 
	m_kUpdateTimer_RacoonDodgeEvent.SetActive(true,::GetTickCount())	;// ���� ���ϱ� �̺�Ʈ  //Switch_10MonthEvent_RacoonDodge

	// ũ�������� Ʈ�� �̺�Ʈ ���� ���� �б�
	// 1. 2�ܰ� Ʈ�� ����ð�?
	DWORD	dwWorldCreateClock = 0;
	SI32 siWorldTreeMode = 0;

	FILE	*fpWorldTreeClock	= _tfopen( TEXT("Config\\WorldTreeClock.dat"), TEXT("rt") );
	if( fpWorldTreeClock != NULL)
	{
		_ftscanf( fpWorldTreeClock, TEXT("%d"), &dwWorldCreateClock );

		dwWorldTreeCreateClock = dwWorldCreateClock;

		fclose( fpWorldTreeClock );
	}
	
	// 2. Ʈ�� ���?(!�ܰ��?2�ܰ��?
	FILE	*fpWorldTreeMode		= _tfopen( TEXT("Config/WorldTree.dat"), TEXT("rt") );
	if( fpWorldTreeMode != NULL)
	{
		_ftscanf( fpWorldTreeMode, TEXT("%d"), &siWorldTreeMode );

		if(siWorldTreeMode > 0)
		{
			bWorldTreeConstruct = true;
		}
		else
		{
			bWorldTreeConstruct = false;
		}

		fclose( fpWorldTreeMode );
	}

	// �������?����
	CreateDirectory("Statistics", NULL);

	// ���?���?�ʱ�ȭ
	NStatistics_Init( &m_clLua );

	// ���ó�¥ �������?�б�
	SYSTEMTIME stTime;		GetLocalTime(&stTime);
	TCHAR	szFilename[MAX_PATH];

	StringCchPrintf( szFilename, sizeof(szFilename), "./Statistics/Item_%04d%02d%02d.log", stTime.wYear, stTime.wMonth, stTime.wDay );
	FILE* fp = fopen( szFilename, "r" );
	if( fp != NULL )	{			fclose( fp );			NStatistics_Read( szFilename );		}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}


cltServer::~cltServer()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

#ifdef USE_HANXNOTI
	if(m_pkHanAuth_XNotiConnector)
	{
		XNoti::Connector::DestroyHandle(m_pkHanAuth_XNotiConnector);
		m_pkHanAuth_XNotiConnector = NULL;
	}

	for(SI32 i=0; i<HANXNOTI_MAX_WAITING; i++)
	{
		CloseHandle(m_kHanXNoti_Waitings[i].m_hWaitingHandle);
		if(m_pkHanXNoti_AnsMessages[i])		delete m_pkHanXNoti_AnsMessages[i];
	}

	DeleteCriticalSection(&m_HanXNoti_CS);

#endif // USE_HANXNOTI

	// APEX-
#ifdef _CHINA
	StopApexProxy();
#endif
	// -APEX

	NDelete( pclIDNumTable );
	NDelete( pclStatisticsManager );
	NDelete( pclAddMoneyStatistics );
	NDelete( pclDelMoneyStatistics );
	NDelete( pclGodHandManager );
	NDelete( pclBookBox );
	NDelete( pclGuildDungeonMgr );

	for ( SI32 i=0; i<GUILDWAR_FIELDNUM; i++ )
	{
		NDelete( pclGuildWarManager[i] );
	} 

	NDelete( pclEmpireManager );
	NDelete( pclKingGuriEventManager );
	NDelete( pclGoonzuKingGuriEventManager );	// [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
	NDelete( pclGuildWarReserveManager );
	NDelete( pclMinisterMgr );
	NDelete( pclHashTable );
	NDelete( pclPersonHashTable );
	NDelete( pclPersonNameManager );
	NDelete( pclNewPersonNameManager );
	NDelete( pclIPToCountryMgr );
	NDelete( pclAccessDenyMgr );
	NDelete( pclNoticeManager );
	NDelete( pclItemList );
	NDelete( pclWeather );
	NDelete( pclTime );
	NDelete( pclRankManager );
	NDelete( pclBillProcess );
	NDelete( pclFrontAuthProcess );
	NDelete( pclGameProtocol );
	NDelete( pclAgentProcess );
	NDelete( pclGameDBProcess );
	NDelete( pclGameDBProcess2 );
	NDelete( pclLogDBProcess );
	NDelete( pclNTradeServerProcess );
	NDelete( pRefreshInfo );
	NDelete( pclStockOfferingManager );
	NDelete( pclGMProtocol );
	NDelete( pclSantaRaccoon );					//[�߰� : Ȳ���� 2007. 11. 29 ��Ÿ���� ������ ����.]
	NDelete( pclCheckProcessManager );
	NDelete( pclRewardMgr );
	NDelete( pclNewSelectBoxMgr );
	//NDelete( m_pclElixirManager );
	NDelete( pclPromoterManager );
	NDelete(m_pGawiBawiBoMgr);
	NDelete(m_pclSummonTransformManager);
	NDelete(m_pclBattleRoyalMgr);
	NDelete(m_pclGiantRaccoonResurrectEvent);
	NDelete(m_pPartyQuestMgr);
	
	//cyj m_pServerMsgPtr, m_pDBMsgPtr �޸� ���� �߰�
	NDelete_Array(m_pDBMsgPtr);
	NDelete_Array(m_pServerMsgPtr);
	NDelete_Array(m_pbUseMsg);

	DeleteCriticalSection( &cs_refresh );

	// ���� �������� �ѹ� �����Ѵ�.
	/*	TCHAR	szFilename[MAX_PATH];
	StringCchPrintf(szFilename, sizeof(szFilename), "./Statistics/Statistics_%04d%02d%02d.log", sTime.wYear, sTime.wMonth, sTime.wDay );
	NStatistics_Write( szFilename, &sTime );*/

	NStatistics_AppendTable_Hour( "Item", &sTime );


	Sleep( 2000 );

	// ���?���?Shutdown
	NStatistics_Shutdown();

	DeleteCriticalSection( &m_cs_SendNetMsg );
	
	// ��ũ��
	if( m_pclMacroReportedMgr )
	{
		delete m_pclMacroReportedMgr;
		m_pclMacroReportedMgr	=	NULL;
	}

	if( m_pclGZFtpMgr )
	{
		delete m_pclGZFtpMgr;
		m_pclGZFtpMgr			=	NULL;
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

// APEX-

void cltServer::KillUser( int id )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	NoticeApexProxy_UserLogout( id, (char*)pclchar->pclCI->clBI.szAccountID );

	pclchar->bApexLogin = false;

#ifdef USE_GAME_GUARD_SERVER
	pclchar->CCSAuthClose();
#endif

	pclchar->GetSession()->CloseSocket();

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendNetMsg( int id, sPacketHeader* pPacket )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if( pclCM->IsValidID(id) == false )
		return;

	pclCM->CR[ id ]->SendNetMsg( pPacket );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::DoMsg_GAMEMSG_REQUEST_APEXDATA(cltMsg* pclMsg, SI32 id)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if( pclCM->IsValidID(id) == false )
		return;

	sPacketHeader *pPacket = (sPacketHeader *)pclMsg;
	char *pPacketChar = (char*)pPacket;

	OutputDebugString( "Receive Apex Data\r\n" );

	NoticeApexProxy_UserData( id, pPacketChar + sizeof( sPacketHeader ), pPacket->usSize - sizeof( sPacketHeader ) );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}
// -APEX




//------------------------------------------
// NPC ���� 
//-----------------------------------------
// NPC�� �����?�����۵� �ش�. 
SI32 cltServer::SetNPCChar(SI32 kind, SI16 villageunique, cltIdentity* pclidentity, SI32 level, SI32 life, SI32 x, SI32 y, SI32 mapindex, SI32 itemmode, cltInnerStatusInfo *pInnerStatusInfo, cltOrderDataInfo*pOrder, cltGMCommandKindInfo* pGMCommandKindinfo )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	cltOrderDataInfo clOrder;

	if(pOrder == NULL)
	{
		clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);
	}
	else
	{
		clOrder.Set(pOrder);
	}

	cltSimpleRank clrank;
	clrank.Init();

	cltIdentity				clIdentity;
	if(pclidentity)
	{
		clIdentity.Set(pclidentity);
	}

	cltInnerStatusInfo		clInnerStatus;

	if ( pInnerStatusInfo == NULL )
	{
		clInnerStatus.Init();
	}
	else
	{
		clInnerStatus.Set( pInnerStatusInfo );
	}

	// Client���� �� NPC�� ������ �˷��ش�.
	SI32 charunique;
	SI32 mode = 0;

	// CMT�� ������Ͷ�� 
	if(pGMCommandKindinfo)
	{
		clIdentity.MakeGMCreateMonster(IDENTITY_GMCREATE_MONSTER);
	}

	if( pclidentity != NULL &&
		pclidentity->siIdentity == IDENTITY_INSTANCEMAP )
	{
		mode = 2;
	}
	/*if ( mapindex >= MAPINDEX_DAEMADO && (pclidentity == NULL ||
	(pclidentity && pclidentity->siIdentity != IDENTITY_PERSON_SUMMON) ) && pclKindInfoSet->pclKI[kind]->IsAtb(ATB_MONSTER) )
	{
		mode	= 2;
	}
	else
	{
		mode	= 0;
	}
	*/

	charunique = cltCharServer::FindEmptyCharUnique(mode);

	SI32 childcharunique = 0;
	SI32 summonindex	= 0;
	
	cltBufInfo clBufInfo;
	clBufInfo.Init();
	cltMakeCharInfo clMakeCharInfo(kind, charunique, level, life, x, y, mapindex, 
		childcharunique, summonindex, 0, 0, 0, 0, 0, 
		0, 0, 0, 1, NULL, 0, 0, 0, 0, 0, 0, 
		villageunique, 0, &clrank,NULL,0, &clOrder,&clIdentity, 
		&clInnerStatus, 0, 0, 0, false,0 ,NULL, NULL, false, 
		0, false, false, false, false, 0, 0, NULL, false,NULL
		,0,0,0,0,0,0,0,0,0,0,false, &clBufInfo
		);

	SI32 id = pclCM->pclIDManager->GetID(mode);
	id = SetChar(id, &clMakeCharInfo, pGMCommandKindinfo);

	cltKindInfo*	pclki;
	pclki = pclCM->CR[id]->pclKI;

	if(id)
	{
		//-------------------------------
		// �� ĳ���Ϳ��� �������� �ش�. 
		//-------------------------------
		// KindInfo�� ������ ���� �������� �����? 

		SI32 addrate = 0;
		if(itemmode == NPCITEMMODE_HUNT)
		{
			addrate = pclHuntManager->siHuntAddRate;
		}
		else if ( itemmode == NPCITEMMODE_DOCKDO )
		{
			addrate	= -20 ;
		}
		else
		{
			addrate = 0 ;
		}

		bool bitemreadyswitch = false;
		cltItem clItem;

		if(itemmode != NPCITEMMODE_NONE)
		{
			/*if ( siEventItemDropRate && rand() % siEventItemDropRate == 0 )
			{
				SI32 itemunique = 8150 ; // �ݰ��� ���?
				SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);
				if(ref > 0)
				{
					clItem.Set( &pclItemManager->pclItemInfo[ref]->clItem);
					bitemreadyswitch = true;
				}
			}*/
				// ���?�������� �ش�. -�۷ι� ������;
				if( itemmode != NPCITEMMODE_GM && !bitemreadyswitch && rand() % 10000 == 0
					&& IsWhereServiceArea( ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE )
					)
				{
					SI32 itemunique = pclItemManager->GetRandBigWarItem();
					SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);
					if(itemunique > 0)
					{
						clItem.Set( &pclItemManager->pclItemInfo[ref]->clItem);
						SI16 brareswitch = 0;
						if(pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch) == true)
						{
							bitemreadyswitch = true;
						}
					}
				}
				else
				{
					if(pclItemManager->MakeRandItem(level,  addrate, &pclKindInfoSet->pclKI[kind]->clItemPbtSet, &clItem) == true)
					{
						bitemreadyswitch = true;
					}
				}
		}

		// �������� �غ�Ǹ�?���Ϳ��� �����Ѵ�. 
		if(bitemreadyswitch)
		{
			// �������?�������� ĳ���Ϳ��� �ش�.
			pclCM->CR[id]->AddItemToInventory(&clItem);
		}


		return id;

	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
	return 0;
}


// NPCGroup���� �ȿ� �ִ°� ?
bool cltServer::IsInNPCGroup(SI32 npcgroup, SI32 mapindex, SI32 x, SI32 y)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(pclNPCManager == NULL)return FALSE;

	return pclNPCManager->IsInNPCGroup(npcgroup, mapindex, x, y);

#else
	//----------------------------------------------------------------
	return false;
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}


//--------------------------------------------------------------
// Neighbour
//---------------------------------------------------------------


// ������ ĳ������ ������ ������ ������Ʈ ��Ų��.
BOOL cltServer::SendCharUpdate(SI32 charunique, bool burgentmode)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 id = cltCharServer::CharUniqueInfo[charunique];

	if(id == 0)return FALSE;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];


	//-----------------------------------
	// ������Ʈ �ؾ� �� ������ �����Ѵ�.
	//-----------------------------------
	SI32 life = pclchar->GetLife();

	SI32 reservedlife	= pclchar->clPB.siReservedLife;
	SI32 mana			= pclchar->GetMana();
	SI32 x				= pclchar->GetX();	
	SI32 y				= pclchar->GetY();
	SI32 movspeed		= pclchar->clPB.GetTotalMoveSpeed();
	UI08 gmmode		= pclchar->pclCI->clBI.uiGMMode;
	bool pcroom		= pclchar->pclCI->clBI.bPCRoom;
	SI08 makeanitype	= pclchar->siPersonalMakeItemType;

	cltBasicCharUpdateInfo update(charunique, life, reservedlife, mana, x, y, movspeed, burgentmode, gmmode , pcroom, makeanitype );

	//------------------------------------------------------
	// ������Ʈ �ϴ� ĳ������ �̿�鿡��?������ ���?������.
	//------------------------------------------------------
	cltMsg clMsg(GAMEMSG_RESPONSE_CHARUPDATE, sizeof(cltBasicCharUpdateInfo), (BYTE*)&update);
	pclchar->SendNeighbourMsg(&clMsg, true);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return TRUE;
}

//[�߰�: Ȳ���� 2007. 11. 26 �Ϸ翡 �ѹ� ���� �α�.]
void  cltServer::DayLogFileWrite(void)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------
	if(pclTime == NULL)			return;

	//[����] ���ַ����� 1���̸� ������ ���ų��� ���?=> 2008-8-22
	bool bDayChangedSwitch = pclTime->IsDayChanged();

	// �Ѵ޿� �ѹ� 1�Ͽ�...
	if( bDayChangedSwitch && 1 == pclTime->clDate.uiDay )
	{
		for(SI32 i = 0; i < 6; ++i)
		{
			if(0 == clItemInfo[i].m_siCnt)
				continue;

			// config ������ ���� ����.
			pclLog->FilePrint(TEXT("Config\\SystemBuyItem.log"), TEXT("%d/%d/%d, COUNT: %d, TOTALPRIZE: %I64d, RARELEVEL: %d"),	
				clItemInfo[i].m_sTime.wYear,
				clItemInfo[i].m_sTime.wMonth,
				clItemInfo[i].m_sTime.wDay,
				clItemInfo[i].m_siCnt,
				clItemInfo[i].m_siMoney, 
				i+1);

			clItemInfo[i].Clear();
		}
		// ��¥
		pclLog->FilePrint(TEXT("Config\\SystemBuyItem.log"), TEXT("\r GoonzuDay : %d/%d/%d"),
			pclTime->clDate.uiYear,
			pclTime->clDate.uiMonth,
			pclTime->clDate.uiDay		);

	}
	
	//[�߰� : Ȳ���� 2007. 11. 19 �ϴ��� ���� 0�Ϸ�  �����ϸ� ó�� ������ �ʱ�ȭ�� �ȴ�.]
	static SI32 Day = 0;

	//[�߰� : Ȳ���� 2007. 11. 19  �Ϸ翡 �ѹ� �ٽ� �ʱ�ȭ.(�Ϸ簣���� ���� ����ϱ�����?)
	if(Day != sTime.wDay)	
	{		
		if(0 != Day)
		{
			// �ý����� ������ ���忡 �ø� �������� ������ ����.
			pclLog->FilePrint(TEXT("Config\\MarketSystemBuyItem.log"), TEXT("%d/%d/%d, COUNT: %d, TOTALPRIZE: %I64d"),	
				clSystemBuyItem.m_sTime.wYear,
				clSystemBuyItem.m_sTime.wMonth,
				clSystemBuyItem.m_sTime.wDay,
				clSystemBuyItem.m_siCnt,
				clSystemBuyItem.m_siMoney );

			clSystemBuyItem.Clear();
		}

		Day = sTime.wDay;
	}

	//------------------------------
	// �Ű��?��ũ�� �����?���� �ʱ�ȭ- ��¥�� �ٲ��?�ʱ�ȭ
	m_pclMacroReportedMgr->OnDayChanged();
	//------------------------------


	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

//--------------------------------
// ������ �׼�. 
//--------------------------------
SHORT cltServer::GameAction()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ������ �ؾ� �� �κ��� ���Ѵ�. 
	if(ServerAction() == FALSE)return 0;

	IncreaseFrame();

	// ĳ������ �ൿ 
	pclCM->Action();



	pclBulletManager->Action();						// �Ѿ˵��� �ൿ 

	pclSmokeManager->Action();						// �������?�ൿ 

	pclMagicManager->Action();						// ��������  �ൿ 



	MagicPopAction();

	// ���� �ൿ 
	if(pclVillageManager)
	{
		pclVillageManager->Action();
	}

	//[�߰�: Ȳ���� 2007. 11. 26 �Ϸ翡 �ѹ� ���� �α�.]
	DayLogFileWrite();

	//[�߰� : ���� 2009. 10. 30 2010�ų��̺�Ʈ : �̺�Ʈ �Ⱓ ���� �� �� �������� ���� �����ֱ�]
	//if(pclClient->pclEventTimeManager->InEventTime("Fortune_Event", &sTime) )
	//{
	//	NewYearEffect();
	//}

	//[�߰� : Ȳ���� 2007. 12. 11 �ų� �̺�Ʈ ��ư �����?�޽��� �������?������ ���� �����ֱ�]
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		NewYearButton();
		NewYearEffect();
	}
	//[����] 24�ÿ� ������ ���?����鿡��?�ַδ�? ����� ����!
	if(pclClient->pclEventTimeManager->InEventTime("ValentineEvent", &sTime) )
	{
		if( pclClient->IsCountrySwitch( Switch_ValenTineWar ) )
		{
			SetValentineDayOnTime();
		}		
	}
	if( pclClient->IsCountrySwitch( Switch_PVP_League ) )	// pvp���� ��������?���?�ȱ׷��� �Ƚ��?
	{
		if(sTime.wHour == 11 && sTime.wMinute == 59 )
		{
			ChangePvPWarGradeInfo()	;
		}

	}
	if (  pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) )// ���ο� ���?�����?ȹ���� ������ ����?�ʱ�ȭ �Ѵ�.
	{
		if(pclClient->pclEventTimeManager->InEventTime("GuildHuntMapInit", &sTime) )
		{
			if(sTime.wHour == 0 && sTime.wMinute == 0 && sTime.wSecond	==	0)
			{
				InitHuntMapWarEnemy();
			}
		}
	}
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return 0;
}
void cltServer::SetValentineDayOnTime(void)//24�ÿ� ������ ���?����鿡��?�ַδ�? ����� ����!
{
	static bool OneDay = true;
	static SI32 Minute = 0;			// �޽����� ������ �д���.

	if(pclClient->pclEventTimeManager->InEventTime("ValentineEvent", &sTime) )
	{
		if(sTime.wHour == 0 && sTime.wMinute == 0 && sTime.wSecond == 0 )
		{
			// ��ü ����
			SI32 index = 0;
			SI32 id	   = 0;
			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;
				// �������?�Ŀ��Ը� �����Ѵ�. 
				if(true == pclClient->pclCM->CR[id]->IsValidConnection())
				{
					pclClient->pclCM->CR[id]->pclValentineWar->Init()	;// �ʱ�ȭ �ϰ� ���?
					cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINE_RUSOLO_RUCOUPLE_ONTIME, 0 );
					SendNetMsg(id, (sPacketHeader*)&clMsg );
				}
			}
		}	
	}
}
void cltServer::ChangePvPWarGradeInfo(void) // ���� �� 12�ð� �Ǹ� �����?üũ �ؼ� ��ε�?�ɽ��� �Ѵ�.
{
	sDBRequest_DBSystemInfo clMsg( 0, 0, DBSYSTEMINFO_CHANGEGRADE,sTime);
	pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
}
void cltServer::InitHuntMapWarEnemy()	// �Ϸ絿�� �ο� ���� �ʱ�ȭ
{
	for (SI32 siGuildSlot = 0; siGuildSlot < MAX_GUILD_NUM; siGuildSlot++)
	{
		for ( SI32 siGuildEnemy = 0 ; siGuildEnemy < MAX_GUUILDENEMYINFO_TODAY; siGuildEnemy++)
		{
			pclGuildManager->clUnit[siGuildSlot].clHuntguildEnemy[siGuildEnemy].init()	;
		}
	}
}
//[�߰� : Ȳ���� 2007. 12. 11 �ų� �̺�Ʈ ��ư �����?�޽��� ������.]
void cltServer::NewYearButton(void)
{
	static bool OneDay = true;
	static SI32 Minute = 9;			// �޽����� ������ �д���.

	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		if(20 == sTime.wHour && Minute == sTime.wMinute && true == OneDay)
		{
			OneDay = false;

			// ��ü ����
			SI32 index = 0;
			SI32 id	   = 0;
			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;
				// �������?�Ŀ��Ը� �����Ѵ�. 
				if(true == pclClient->pclCM->CR[id]->IsValidConnection())
				{
					cltMsg clMsg( GAMEMSG_RESPONSE_NEWYEAR_BTN, 0 );
					SendNetMsg(id, (sPacketHeader*)&clMsg );
				}
			}
		}
		else if(20 != sTime.wHour && Minute != sTime.wMinute && false == OneDay)
		{
			OneDay = true;
		}
	}
}

void cltServer::NewYearEffect(void)
{
	static bool OneDay = true;

	//SYSTEMTIME	StartTime = { 2008, 12, 0, 31, 12, 0, 0, 0 };
	//SYSTEMTIME	EndTime	  = { 2009, 02, 0, 01, 23, 0, 0, 0 };

	if ( true == pclClient->IsCountrySwitch(Switch_NewYearEvent_2010) )
	{
		if ( true == pclClient->pclEventTimeManager->InEventTime("Fortune_Event", &sTime) ) //2010�ų��̺�Ʈ
		{
			// ������ ������ �����ش�.
			if( 0 == sTime.wMinute && true == OneDay )
			{
				OneDay = false;

				// ��ü ����
				SI32			index	= 0;
				SI32			id		= 0;
				cltItem			clitem;
				cltCharServer*	pclchar = NULL;

				while(id = pclClient->pclCM->GetCharOrder(index))
				{
					index++;

					// �������?�Ŀ��Ը� �����Ѵ�. 
					if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
					{
						pclchar = (cltCharServer*)pclCM->CR[id];

						ZeroMemory(&clitem, sizeof(clitem));
						clitem.siUnique  = 13042;		
						clitem.siItemNum =	  1;

						InputItem(pclchar, &clitem, INPUTITEMPERSON_OPTION_EVENT);
					}
				}
			}
			else if(0 != sTime.wMinute && false == OneDay)
			{
				OneDay = true;
			}
		}
	}

	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{	
		// ������ ������ �����ش�.
		if(0 == sTime.wMinute && true == OneDay)
		{
			OneDay = false;

			// ��ü ����
			SI32			index	= 0;
			SI32			id		= 0;
			cltItem			clitem;
			cltCharServer*	pclchar = NULL;

			//// [���? �������� �������� ������ �����Ѵ�.
			//pclCM->GiveItem(0, ITEMUNIQUE(13042), 1, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_EVENT, 0);

			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;

				// �������?�Ŀ��Ը� �����Ѵ�. 
				if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
				{
					pclchar = (cltCharServer*)pclCM->CR[id];

					ZeroMemory(&clitem, sizeof(clitem));
					clitem.siUnique  = 13042;
					clitem.siItemNum =	  1;

					InputItem(pclchar, &clitem, INPUTITEMPERSON_OPTION_EVENT);
				}
			}
		}
		else if(0 != sTime.wMinute && false == OneDay)
		{
			OneDay = true;
		}
	}
}

BOOL cltServer::ServerAction()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	static clock_t DelayCheckStartClock = 0;
	static SI32 siGameDelay		= GetFrameDelay();
	static int nTraceSA = 0;
	if ( nTraceSA < 60 )
	{
		nTraceSA++;
		TraceBoot("ServerAction enter bComplete=%d bListenOK=%d step=%d init0..4=%d,%d,%d,%d,%d",
			bCompleteDBInitAll, bListenOK, siCurrentDBInit,
			siCompleteDBInit[0], siCompleteDBInit[1], siCompleteDBInit[2], siCompleteDBInit[3], siCompleteDBInit[4]);
	}

	if( bCompleteDBInitAll == FALSE )
	{
		for( int i = 0; i < DBCONNECTION_NUMBER; ++i )
		{
			if( siCompleteDBInit[ i ] != siCurrentDBInit ) break;
		}

		if( i == DBCONNECTION_NUMBER )
		{
			switch( siCurrentDBInit )
			{
			case DBINITIALIZE_STEP_1:		{		TraceBoot("ServerAction gate: STEP1 done -> InitializeDB2"); InitializeDB2();}					break;
			case DBINITIALIZE_STEP_2:		{		TraceBoot("ServerAction gate: STEP2 done -> all complete"); bCompleteDBInitAll = TRUE;
				// fresh-server fix: seed NPC shop lists immediately (market price index needs player trades which do not exist yet)
				if ( pclItemManager->clItemStatistic.siNPCPriceRate <= 0 ) { pclItemManager->clItemStatistic.siNPCPriceRate = 100; }
			pclNPCItemInfo->Create( pclItemManager->clItemStatistic.siNPCPriceRate ); }		break;
			}
		}
	}

	if( bCompleteDBInitAll == TRUE &&
		bListenOK == FALSE )
	{
		TraceBoot("ServerAction calling pclGameProtocol->Start()");
		bListenOK = TRUE;
		pclGameProtocol->Start();
		TraceBoot("ServerAction pclGameProtocol->Start() returned");
	}

	WRITE_MEMLOG( 120, 1, 0, 0 );

	//-----------------------------
	// �ð� ������ üũ�Ѵ�.
	//-----------------------------
	_SYSTEMTIME	sLocalTime;
	GetLocalTime(&sLocalTime);
	bool bChangeHour = false;
	bool bChangeMinute = false;
	bool bChangeSecond = false;
	bool bChangeDay = false;

	if( sTime.wHour		!= sLocalTime.wHour )		bChangeHour = true;
	if( sTime.wSecond	!= sLocalTime.wSecond )		bChangeSecond = true;
	if( sTime.wMinute	!= sLocalTime.wMinute )		bChangeMinute = true;
	if( sTime.wDay		!= sLocalTime.wDay )		bChangeDay = true;
	//-----------------------------

	//-----------------------------
	//  ���� �ð��� �˾Ƴ���. 
	//-----------------------------
	memcpy(&sTime, &sLocalTime, sizeof(sTime));
	//GetLocalTime(&sTime);

	// ���� �ð��� ���´�. 
	// ���� �ð��� ������ �������� ������ �������� �ʴ´�. 
	if(RequestGameTimeFromDB() == false)
	{
		TraceBoot("ServerAction RequestGameTimeFromDB -> false (waiting time)");
		return false;
	}
	TraceBoot("ServerAction RequestGameTimeFromDB -> true");

	// ������ Ÿ���� �ҷ��´�.
	if(RequestMapTypeFromDB() == false)
	{
		TraceBoot("ServerAction RequestMapTypeFromDB -> false (waiting maptype)");
		return false;
	}
	TraceBoot("ServerAction RequestMapTypeFromDB -> true");

	// ���?�غ� �Ϸ�Ǹ�?
	// Ŭ���̾�Ʈ�� ������ ����ϱ�?�����Ѵ�. 
	// �غ� �� �Ǿ� ������ ������ �������?�ʴ´�.
	if(StartNetwork() == false)
	{
		TraceBoot("ServerAction StartNetwork -> false");
		return false;
	}
	TraceBoot("ServerAction StartNetwork -> true");

	WRITE_MEMLOG( 120, 2, 0, 0 );

	//----------------------------
	// �ð�ó��. 
	//----------------------------
	bool bDayChangedSwitch  =  false;
	if(pclTime)
	{
		pclTime->Action(bForceTimeSwitch);
		bDayChangedSwitch = pclTime->IsDayChanged();
	}


	//----------------------------
	// ���?������ ����
	//----------------------------
	/*	if( bChangeSecond )
	{
	TCHAR	szFilename[MAX_PATH];
	StringCchPrintf(szFilename, sizeof(szFilename), "./Statistics/Statistics_%04d%02d%02d.log", sTime.wYear, sTime.wMonth, sTime.wDay );
	NStatistics_Write( szFilename, &sTime );
	}*/

	if( bChangeHour )
	{
		SYSTEMTIME SaveTime;
		TimeSubHour(&sTime, 1, &SaveTime);
		NStatistics_AppendTable_Hour( "Item", &SaveTime );
	}

	//----------------------------
	// ���?������ ó�� - ��¥�� �Ѿ ���?
	//----------------------------
	/*	if( bChangeDay )
	{
	// ���� ��¥ 23:59:00.000 ~ 23:59.99.999 ������ �����͸� ó���ϱ� ���� �ѹ��� �����Ѵ�.
	TCHAR	szFilename[MAX_PATH];
	SYSTEMTIME stSaveTime;
	TimeSubDate(&sTime, 1, &stSaveTime);
	StringCchPrintf(szFilename, sizeof(szFilename), "./Statistics/Statistics_%04d%02d%02d.log", stSaveTime.wYear, stSaveTime.wMonth, stSaveTime.wDay );
	NStatistics_Write( szFilename, &stSaveTime );
	}
	*/
	//------------------------------------
	// ���� �ʱ⿡ �ʿ��� �۾� 
	//------------------------------------
	if( bSetTopLevelVillage == FALSE &&		// �ְ� ���� ������ �������� �ʾҰ�,
		bCompleteDBInitAll == TRUE &&		// DB �ʱ�ȭ�� �Ϸ�Ǿ���?
		bSetVillageLevel == TRUE )			// ���� �������� �����Ǿ��ٸ�, 
	{
		//----------------------------
		// AutoUnblock ���� �б�
		//----------------------------
		bool bGetAUMode		= false;
		SI32 siGetVersion	= 0;

		LoadAutoUnblockFromFile( &bGetAUMode, &siGetVersion );

		if ( GetGameVersionInFile() != siGetVersion )
		{
			bGetAUMode = false;
		}

		bAutoUnblock = bGetAUMode;


		bSetTopLevelVillage = TRUE;

		// �ְ� ���� ������ ������ ���Ѵ�. 
		SI32 villageunique, villagelevel	= 100;
		GetTopLevelVillage(&villageunique, &villagelevel);
		siBestVillageLevel	= villagelevel;

		SendServerInfo(0);

		SendSet_InstanceProfitVillage(0,true);

#ifdef _DEBUG
		//pclNPCManager->MakeNPCInstnce() ;
#endif
	}

	WRITE_MEMLOG( 120, 3, 0, 0 );

#ifndef	_JAPAN_USER_TEST
	// -HanAuth_Lib
	#ifdef USE_HANRTA
		//------------------------------------
		// HanAuth_Lib : �������� - 5�и��� ȣ��
		//------------------------------------
		if(m_kHanRTA_Timer.IsTimed(GetTickCount(), true))
		{
			TCHAR buffer[256];
			SI32 siCurUserNum = pclGameProtocol->GetSessionManager()->GetActiveSessionCount();
	#if defined (_KOREA)
			StringCchPrintf( buffer, 256, TEXT("gameid=K_GOONZU&servername=%d&conncount=%d"), siServerID, siCurUserNum );
	#elif defined (_USA)
			StringCchPrintf( buffer, 256, TEXT("gameid=U_GOONZU&servername=%d&conncount=%d"), siServerID, siCurUserNum );
	#endif
			HanRTA( buffer );
			//SYSTEMTIME CT;
			//GetLocalTime(&CT);
		}
	#endif // USE_HANRTA
		// -HanAuth_Lib
	#ifdef USE_CNRTA
		//------------------------------------
		// USE_CNRTA : �������� - 5�и��� ȣ��
		//------------------------------------
		if(m_kCNRTA_Timer.IsTimed(GetTickCount(), true))
		{
			TCHAR buffer[256];
			SI32 IdcID	=	siServerID/1000	;
			SI32 Serverid =	siServerID%1000	;
			SI32 siCurUserNum = pclGameProtocol->GetSessionManager()->GetActiveSessionCount();
			StringCchPrintf( buffer, 256, TEXT("gameid=C_GZ&idcid=%0.2d&serverid=%0.3d&membercnt=%d"),IdcID, Serverid, siCurUserNum );		
			HanRTA( buffer );
		}
	#endif
#endif

#ifdef USE_GAME_GUARD_SERVER
	if(m_kUpdateTimer_GameGuard.IsTimed(GetTickCount(), true))
	{
		GGAuthUpdateTimer();
	}
#endif // USE_GAME_GUARD_SERVER

	//WriteFileLog("HanRTA.txt", __FILE__, __LINE__, TEXT("gameid=K_GOONZU&servername=%d&conncount=%d"), siServerID, siCurUserNum);

	// [����] Auto Unblock
	if ( m_kUpdateTimer_AutoUnblock.IsTimed( GetTickCount(), false ) == true )
	{
		bool	bGetAutoUnblockMode = false;
		SI32	siGetGameVersion	= 0;
		SI32	siNowGameVersion	= GetGameVersionInFile();


		LoadAutoUnblockFromFile( &bGetAutoUnblockMode, &siGetGameVersion );

		if ( (bGetAutoUnblockMode == false)				// AutoUnblock ���?����������
			|| (siGetGameVersion != siNowGameVersion)	// ���� ���ӹ����� �̸� ����Ǿ�?�ִ� ���ӹ����� �ٸ��� AutoUnblock Off
			|| (siGetGameVersion <= 0)					// �����?���ӹ����� 0�����̸� AutoUnblock Off
			)
		{
			SaveAutoUnblockToFile( false, siNowGameVersion );
			bAutoUnblock = false;
		}
		// AutoUnblock�� On�̰�, ���� ���ӹ����� �����?���ӹ����� ���ٸ�
		else if ( (bGetAutoUnblockMode == true) && (siGetGameVersion == siNowGameVersion) )
		{
			SaveAutoUnblockToFile( true, siNowGameVersion );
			bAutoUnblock = true;

			pclFrontAuthProcess->m_refreshInfo.state = 0;
		}

		SendServerInfo(0);
	}
	//------------------------------------
	// ���� ����
	//------------------------------------
	if ( GetFrame() > 1000 )
	{	
		// 1 �д� �ѹ��� ȣ��
		if ( GetFrame() % 1200 == 0 )
		{
			if( pclTradeMerchantNPCMgr )
			{
				pclTradeMerchantNPCMgr->Action( sTime.wDay );
			}
		}
	}

	WRITE_MEMLOG( 120, 4, 0, 0 );

	if ( pclTime != NULL && pclTime->clDate.uiYear > 2 && GetFrame() == 1000 )
	{
		if ( ! bHorseCurrentPriceSet )						// �� �ü��� �����Ǿ� ���� �ʴٸ�, �� �ü��� ���;���...
		{
			// ��(ǰ��)�� �����Ѵٸ� �� ���� ���ؼ� �ü��� ��û��.
			if ( pclHorseManager->pclHorseTypeInfo[ 1 ] != NULL )
			{
				sDBRequest_GetCurrentHorsePrice clGetCurrentHorsePrice( 1 );
				SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clGetCurrentHorsePrice );
			}
		}
	}

	WRITE_MEMLOG( 120, 5, 0, 0 );

	//------------------------------------
	// ���� ����
	//------------------------------------
	if ( pclTime != NULL && pclTime->clDate.uiYear > 4 && GetFrame() > 1500 )		// ���� �ð�����, 5�� �̻��� �Ǵ� ���?
		{
			if ( ! bHorseCurrentPriceSet )		// �� �ü��� �������� ���� ����.
			{
			if ( pclHorseManager->pclHorseTypeInfo[ 1 ] != NULL )
			{	
				// 200 ����
				sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, 2000000 );
				SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
				}
		}
		// 3�д�  �ѹ��� ȣ��.
		else if ( GetFrame() % 3600 == 0 )
		{			

			// �Ϸ翡 �ѹ��� �ü� �������?( 06 �ÿ� )
			// 5�� �̻��̰� 14�������ΰ��?
			if ( pclTime->clDate.uiYear < 15 && sTime.wHour == 6 && sTime.wMinute >=0 && sTime.wMinute < 4 ) 
				{
				if ( siHorseTradeCount >= 15 )		// 15�� �̻� 
					{
					if ( pclItemManager->clItemStatistic.siNPCPriceRate * 10000 < siHorseCurrentPrice * 70 / 100 )
					{
						sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, pclItemManager->clItemStatistic.siNPCPriceRate * 10000 );
						SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
					}
					else
					{	
						sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, siHorseCurrentPrice * 70 / 100 );
						SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
					}
					}
				else if ( siHorseTradeCount >= 8 )
					{
						if ( pclItemManager->clItemStatistic.siNPCPriceRate * 10000 < siHorseCurrentPrice * 80 / 100 )
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, pclItemManager->clItemStatistic.siNPCPriceRate * 10000 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
						else
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, siHorseCurrentPrice * 80 / 100 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
					}
					else if ( siHorseTradeCount < 4 )
					{
						if ( pclItemManager->clItemStatistic.siNPCPriceRate * 10000 < siHorseCurrentPrice * 105 / 100 )
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, pclItemManager->clItemStatistic.siNPCPriceRate * 10000 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
						else
						{	
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, siHorseCurrentPrice * 105 / 100 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
					}
				}
				// 15�� �̻��� ���?
				else if ( pclTime->clDate.uiYear > 14 && sTime.wHour == 6 && sTime.wMinute >=0 && sTime.wMinute < 4 )
				{
					if ( siHorseTradeCount >= 25 )			// 25 �� �̻�
					{
						if ( pclItemManager->clItemStatistic.siNPCPriceRate * 10000 < siHorseCurrentPrice * 95 / 100 )
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, pclItemManager->clItemStatistic.siNPCPriceRate * 10000 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
						else
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, siHorseCurrentPrice * 95 / 100 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
					}
					else if ( siHorseTradeCount <= 15 )		// 15 �� ����
					{
						if ( pclItemManager->clItemStatistic.siNPCPriceRate * 10000 < siHorseCurrentPrice * 110 / 100 )
						{
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, pclItemManager->clItemStatistic.siNPCPriceRate * 10000 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
						else
						{	
							sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, siHorseCurrentPrice * 110 / 100 );
							SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
						}
					}
				}
			}
		}

		WRITE_MEMLOG( 120, 5, 0, 0 );

		if( sTime.wMinute % 5 == 0 &&				// 5�а������� 5�ʶ� �����?�����Ѵ�.
			bChangeSecond && sTime.wSecond == 5 )
		{
			if(pclAccessDenyMgr)
			{
				pclAccessDenyMgr->Action();
			}
		}

		// �������� 
		//if( siServiceArea == ConstServiceArea_English )
		//{
		static clock_t ctTradeList = CurrentClock;
		if ( pclNTradeServerProcess && pclNTradeServerProcess->GetTradeServerServersSession() &&
			pclNTradeServerProcess->GetTradeServerServersSession()->GetState() == SESSION_STATE_ESTABLISHED )
		{
			// 1�п� �ѹ��� ����Ʈ ��û�Ѵ�.
			if(TABS(CurrentClock - ctTradeList) > 1000*60)
			{
				if ( szServerName[0] != NULL )
				{
					SI16 serverindex = siServerUnique ;

#ifdef _DEBUG
					NTradeServerRequest_Get_ListInfo clMsg("tst",serverindex);
					pclNTradeServerProcess->SendMsg((sPacketHeader *)&clMsg);
					ctTradeList = CurrentClock ;
#else
					if (pclClient->siServiceArea == ConstServiceArea_Japan)
					{
						NTradeServerRequest_Get_ListInfo clMsg("jpn",serverindex);
						pclNTradeServerProcess->SendMsg((sPacketHeader *)&clMsg);
						ctTradeList = CurrentClock ;
					}
					else if (pclClient->siServiceArea == ConstServiceArea_English)
					{
						NTradeServerRequest_Get_ListInfo clMsg("eng",serverindex);
						pclNTradeServerProcess->SendMsg((sPacketHeader *)&clMsg);
						ctTradeList = CurrentClock ;
					}
#endif
				}
			}
		}
		//}

		//----------------------------
		// ��Ƽ �ൿ 
		//----------------------------
		if ( pclPartyMgr )
		{
			pclPartyMgr->Action();
		}

		WRITE_MEMLOG( 120, 6, 0, 0 );

		//----------------------------
		// ���?�ൿ 
		//----------------------------
		if(pclCityHallManager)
		{
			pclCityHallManager->CommonAction( pclTime );
			pclCityHallManager->Action( pclTime );
		}


		WRITE_MEMLOG( 120, 7, 0, 0 );

		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclBankManager)
		{
			pclBankManager->CommonAction( pclTime );
		}

		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclStockManager)
		{
			pclStockManager->CommonAction( pclTime );
			pclStockManager->Action( pclTime );
		}

		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclMarketManager)
		{
			pclMarketManager->CommonAction( pclTime );
			pclMarketManager->Action(bDayChangedSwitch);
		}

		if( pclClient->IsCountrySwitch( Switch_NewMarket ) )
		{
			//[����] ���ո���.
			if( pclNewMarketManager )
			{
				pclNewMarketManager->Action( pclTime, CurrentClock );
			}
		}

		WRITE_MEMLOG( 120, 8, 0, 0 );

		//----------------------------
		// ���� �ൿ 
		//----------------------------	
		if(pclHouseManager)
		{
			pclHouseManager->CommonAction( pclTime );
			pclHouseManager->Action( pclTime, CurrentClock );
		}


		//----------------------------
		// ������ �ൿ 
		//----------------------------
		if( pclHuntManager )
		{
			pclHuntManager->CommonAction( pclTime );
			if( bChangeSecond )		pclHuntManager->Action();		// �ʴ��ѹ��� ó���Ѵ�.
		}

		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclPostOfficeManager)
		{
			pclPostOfficeManager->CommonAction( pclTime );
		}



		//----------------------------
		// �纹�� �ൿ 
		//----------------------------
		if(pclHorseMarketManager)
		{
			pclHorseMarketManager->CommonAction( pclTime );
			pclHorseMarketManager->Action(bDayChangedSwitch);		
		}


		//----------------------------
		// �Ű� �ൿ 
		//----------------------------
		if(pclRealEstateMarketManager)
		{
			pclRealEstateMarketManager->CommonAction( pclTime );
			pclRealEstateMarketManager->Action(bDayChangedSwitch);		
		}

		//----------------------------
		// �忹�� �ൿ 
		//----------------------------
		if(pclSummonMarketManager)
		{
			pclSummonMarketManager->CommonAction( pclTime );
			pclSummonMarketManager->Action(bDayChangedSwitch);		
		}

		//----------------------------
		// �񺯻� �ൿ 
		//----------------------------
		if(pclSummonHeroMarketManager)
		{
			pclSummonHeroMarketManager->CommonAction( pclTime );
			pclSummonHeroMarketManager->Action(bDayChangedSwitch);		
		}



		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclLandManager)
		{
			pclLandManager->CommonAction( pclTime );
			pclLandManager->Action( pclTime, CurrentClock );
		}

		//----------------------------
		// ���� �ൿ 
		//----------------------------
		if(pclMineManager)
		{
			pclMineManager->CommonAction( pclTime );
			pclMineManager->Action( pclTime, CurrentClock );
		}



		//----------------------------
		// ���ڽ� �ൿ 
		//----------------------------
		if(pclFeastManager)
		{
			pclFeastManager->CommonAction( pclTime );
			pclFeastManager->Action();		
		}

		//-------------------------------
		// ���?�ൿ
		//----------------------------------

		if (pclGuildManager)
		{
			pclGuildManager->CommonAction(pclTime);
			pclGuildManager->Action(bDayChangedSwitch);
		}

		if (pclGuildWarReserveManager)
		{
			pclGuildWarReserveManager->ServerAction();
		}

		for ( SI32 i=0; i<GUILDWAR_FIELDNUM; i++ )
		{
			if (pclGuildWarManager[i])
			{
				pclGuildWarManager[i]->Action();
			}

		}

		if (pclGuildDungeonMgr)
		{
			pclGuildDungeonMgr->Action();
		}

		WRITE_MEMLOG( 120, 9, 0, 0 );

		//----------------------------
		// ���� ���� NPC �ൿ
		//----------------------------
		if(pclForeignMerchantNPCMgr)
		{
			pclForeignMerchantNPCMgr->Action( pclTime );
		}

		//------------------------------
		// ��ť �ŷ��� �ൿ
		//------------------------------
		if(pclDistributionWorldMoney)
		{
			pclDistributionWorldMoney->Action(bDayChangedSwitch);		
		}

		//KHY -0702- �������� ���?�����鿡��, ������ �ð��� �������� �����Ѵ�. GM��ɾ�?�ð� üũ.
		if(pclGMManager)
		{
			pclGMManager->Action( );		
		}	

		WRITE_MEMLOG( 120, 10, 0, 0 );

		//-------------------------------
		// �� �Ŵ��� �ൿ - �ַ��δ� ó�� - by LEEKH 2008.01.07
		//----------------------------------
		if(pclMapManager)
		{
			pclMapManager->Action();
		}

		//----------------------------
		// ���� ó��. 
		//----------------------------
		if(pclWeather)
		{
			pclWeather->Action(CurrentClock);

			if(GetFrame() % 200 == 100)
			{	
				stWeatherData weatherdata;
				if(pclWeather->GetWeatherData(&weatherdata) == true)
				{
					// ���� ������ Ŭ���̾�Ʈ���� �����Ѵ�. 
					cltMsg clMsg(GAMEMSG_RESPONSE_WEATHER, sizeof(weatherdata), (BYTE*)&weatherdata);

					for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
					{
						if( pclCM->IsValidID(id) )
						{
							pclCM->GetCharServer(id)->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
			}
		}

		//cyj �����ð��� 1�и��� Ŭ���̾�Ʈ���� ����
		if (GetFrame() % 1200 == 0)
		{
			// ���� �ð��� Ŭ���̾�Ʈ���� �����Ѵ�. 
			cltGameMsgResponse_ServerTime clTime(sTime);
			cltMsg clMsg(GAMEMSG_RESPONSE_SERVERTIME, sizeof(clTime), (BYTE*)&clTime);

			for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
			{
				if( pclCM->IsValidID(id) )
				{
					if( pclCM->GetCharServer(id)->IsValidConnection() )
						pclCM->GetCharServer(id)->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}

		// [����] GM������ 10�ʸ��� �ѹ��� �ð��� ������
		if (GetFrame() % 200 == 0)
		{
			// ���� �ð��� Ŭ���̾�Ʈ���� �����Ѵ�. 
			cltGameMsgResponse_ServerTime clTime(sTime);
			cltMsg clMsg(GAMEMSG_RESPONSE_SERVERTIME, sizeof(clTime), (BYTE*)&clTime);

			for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
			{
				if( pclCM->IsValidID(id) )
				{
					if( pclCM->GetCharServer(id)->IsValidConnection() &&
						pclCM->GetCharServer(id)->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
					{
						pclCM->GetCharServer(id)->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}

		WRITE_MEMLOG( 120, 11, 0, 0 );

		if(GetFrame() % 6000 == 0)
		{	
			//--------------------------------------
			// ĳ���� ���� ���?
			//--------------------------------------
			pclCM->GetStatistic(&pclStatisticsManager->clDailyStatistics);
		}

		WRITE_MEMLOG( 120, 12, 0, 0 );

		//--------------------------------------
		// ���� ���� ���� 
		//--------------------------------------
		if( bCompleteDBInitAll == TRUE && 
			bSetVillageLevel == FALSE )
		{
			bSetVillageLevel = TRUE;

			SetVillageLevel();
		}

		if( GetFrame() % ( 1200 * 5 ) == 0 ) {
			SetVillageLevel();
		}


		WRITE_MEMLOG( 120, 13, 0, 0 );

		//--------------------------------------
		// �����?���ڸ� Ŭ���̾�Ʈ/ DB���� ������. 
		//--------------------------------------
		if(GetFrame() % 300 == 0)
		{

			SI32 usernum = pclGameProtocol->GetSessionManager()->GetActiveSessionCount();

			//--------------------------------------
			// DB�� �����Ѵ�. 
			//--------------------------------------
			if(pclStatisticsManager)
			{
				// NEXON  
				sDBRequest_SetUserNum clUserNum( siCurUserNum,0 , siHangameCharNum , siGamaniaCharNum, siGamePotCharNum, siNDECharNum , siNEXONCharNum );
				//sDBRequest_SetUserNum clUserNum( siCurUserNum,0 , siHangameCharNum , siGamaniaCharNum, siGamePotCharNum, siNDECharNum );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clUserNum);
			}

			//--------------------------------------
			// Ŭ���̾�Ʈ���� ������. 
			//--------------------------------------

			// 30�� �̻� DB�� ���� �ƹ� �޽����� ���� ���ߴٸ� �̻��� �ִ�. 
			bool bdbfailswitch = false;
			if(dwLastDBClock && TABS(CurrentClock - dwLastDBClock) > 30000)
			{
				bdbfailswitch	= true;
			}

			cltGameMsgResponse_UserNum clInfo(siCurUserNum, siTodayMaxUserNum, bdbfailswitch);
			cltMsg clMsg(GAMEMSG_RESPONSE_USERNUM, sizeof(clInfo), (BYTE*)&clInfo);

			// ��� ĳ���Ϳ��� �����Ѵ�.
			for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
			{
				if( pclCM->IsValidID(id) )
				{
					if( pclCM->GetCharServer(id)->IsValidConnection() &&
						pclCM->GetCharServer(id)->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
					{
						pclCM->GetCharServer(id)->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}

		WRITE_MEMLOG( 120, 14, 0, 0 );

		//-------------------------------
		// ������ ���?���� �������� NPC�� ����ؾ�?�Ѵ�. 
		//-------------------------------
		if(pclNPCManager)
		{
			pclNPCManager->SetChar();
		}

		//-------------------------------
		// ���?�̺�Ʈ�� �����ϴ� �޴����� ���� ��Ų��
		//-------------------------------
		if ( pclEventTimeManager )
		{
			pclEventTimeManager->Action();
		}

		//-------------------------------
		// [����] ���� ��ǥ��Ȳ �޴���
		//-------------------------------
		if ( pclVoteStatusMgr )
		{
			pclVoteStatusMgr->Action();
		}

		WRITE_MEMLOG( 120, 15, 0, 0 );


		if( pclClient->siServiceArea == ConstServiceArea_Japan )
		{

			if( sTime.wYear == 2008 && sTime.wMonth == 7 && sTime.wDay == 21 )
			{
				bSpecailDay9switch = true;

				if( GetFrame() % (5000) == 0 )
				{
					SendNoticeMsg( -1, 0, 0, GetTxtFromMgr(7502) );
				}


			}
			else
			{
				bSpecailDay9switch = false;
			}			
		}

		//------------------------------
		// �ڵ� ����
		//------------------------------
		if ( bAutoStockDistriubtionSwitch )
		{
			if ( GetFrame() > 1000 )
			{
				pclStockDistributionMgr->SendStockDistributionNoticeMsg( GetFrame(), sTime );
			}
		}
		WRITE_MEMLOG( 120, 16, 0, 0 );

		//////////////////////////////////////////////////////////////////////////
		// [�߰� : ���� ���ϵ� ����. Ȳ���� 2007. 9. 18]
		// �ѱ� �߼� �̺�Ʈ.
		if(pclClient->IsCountrySwitch(Switch_BossWildBoar))
		{
			static bool WildBoarEvent = true;					

			// �ش� �ð��� �̺�Ʈ �ѹ�.
			if(true == WildBoarEvent)
			{
				// 15�ÿ� 22��.
				if((15 == sTime.wHour || 22 == sTime.wHour))	
				{
					// 22�� ���� 26�ϱ���.
					if(sTime.wDay >= 13 && sTime.wDay <= 15)
					{
						// 9��.
						if(9 == sTime.wMonth)
						{
							WildBoarEvent = false;

							cltServer *pclserver = (cltServer*)pclClient;

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);		// ���?PC�� ��ȣ��, �������?������. 

							SI32 x, y;
							SI32 siSummonID;
							for(int i = 1; i <= 42; ++i)
							{
								x = pclClient->pclVillageManager->pclVillageInfo[i]->PosX + (5 - rand() % 10);
								y = pclClient->pclVillageManager->pclVillageInfo[i]->PosY + (5 - rand() % 10);

								siSummonID = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_WILDBOARBOSS")), 0, NULL, 50, 0, 
									x, y, 0, NPCITEMMODE_NORMAL, &clinnerstatus);

								// ���� ���ϵ� ���� ������ ����ð���?���� ������ �ð��� �����Ѵ�.
								if(pclCM->IsValidID(siSummonID))
								{
									((cltCharServer*)pclCM->CR[siSummonID])->dwTimerClock = pclserver->CurrentClock;
								}
							}
						}
					}
				}
			}
			// �̺�Ʈ ���� �� �̺�Ʈ �ð��� �ƴ� ���?���� �̺�Ʈ ���� �غ�.
			else 
			{
				if((15 != sTime.wHour && 22 != sTime.wHour))	
				{
					WildBoarEvent = true;	
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// [�߰� : Ȳ���� 2007. 11. 12.  �߼� ������ �̺�Ʈ.  ĥ����]
		if(pclClient->IsCountrySwitch(Switch_Thanksgiving_day))
		{
			static bool Thanksgiving = true;					

			// �ش� �ð��� �̺�Ʈ �ѹ�.
			if(true == Thanksgiving)

			{
				// ���� ���ÿ���.
				bool MonsterSet = false;

				switch(pclClient->siServiceArea)
				{
				case ConstServiceArea_English:
					{			
						// 2��, 8��, 14��, 20��
						if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
						{
							// 20�� ���� 30�ϱ���. �Ǵ� 1�� ���� 4�ϱ���
							if( sTime.wDay >= 20 && sTime.wDay <= 30 )
							{
								// 11��, 12��
								if( 11 == sTime.wMonth )
								{
									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
							if( sTime.wDay >= 1 && sTime.wDay <= 4 )
							{
								if( 12 == sTime.wMonth )
								{
									// �������� 20�ô� �������� �� �̺�Ʈ�� ������ ������ ����.
									if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
										break;

									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
						}
					}
					break;

				case ConstServiceArea_USA:
					{	
						// 2��, 8��, 14��, 20��
						if( 2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour )
						{
							// 21�� ���� 30�ϱ���. �Ǵ� 1�� ���� 5�ϱ���
							if( sTime.wDay >= 21 && sTime.wDay <= 30 )
							{
								// 11��, 12��
								if( 11 == sTime.wMonth )
								{
									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}

							if(	sTime.wDay >= 1 && sTime.wDay <= 5 )
							{
								if(	12 == sTime.wMonth )
								{
									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
						}
					}
					break;
				case ConstServiceArea_NHNChina:
					{			
						// 2��, 8��, 14��, 20��
						if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
						{
							// 20�� ���� 30�ϱ���. �Ǵ� 1�� ���� 4�ϱ���
							if( sTime.wDay >= 20 && sTime.wDay <= 30 )
							{
								// 11��, 12��
								if( 11 == sTime.wMonth )
								{
									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
							if( sTime.wDay >= 1 && sTime.wDay <= 4 )
							{
								if( 12 == sTime.wMonth )
								{
									// �������� 20�ô� �������� �� �̺�Ʈ�� ������ ������ ����.
									if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
										break;

									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
						}
					}
					break;
				case ConstServiceArea_EUROPE:
					{			
						// 2��, 8��, 14��, 20��
						if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
						{
							// 20�� ���� 30�ϱ���. �Ǵ� 1�� ���� 4�ϱ���
							if( sTime.wDay >= 20 && sTime.wDay <= 30 )
							{
								// 11��, 12��
								if( 11 == sTime.wMonth )
								{
									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
							if( sTime.wDay >= 1 && sTime.wDay <= 4 )
							{
								if( 12 == sTime.wMonth )
								{
									// �������� 20�ô� �������� �� �̺�Ʈ�� ������ ������ ����.
									if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
										break;

									Thanksgiving = false;
									MonsterSet	 = true;
								}
							}
						}
					}
					break;

				}

				// true�̸� ���� �ֺ��� ���� ��ġ(ĥ����).
				if(true == MonsterSet)
				{
					cltServer *pclserver = (cltServer*)pclClient;

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);		// ���?PC�� ��ȣ��, �������?������. 

					SI32 x, y;
					SI32 siSummonID;
					for(int i = 1; i <= 42; ++i)
					{
						x = pclClient->pclVillageManager->pclVillageInfo[i]->PosX + (5 - rand() % 10);
						y = pclClient->pclVillageManager->pclVillageInfo[i]->PosY + (5 - rand() % 10);

						siSummonID = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_TURKEY")), 0, NULL, 50, 0, 
							x, y, 0, NPCITEMMODE_NORMAL, &clinnerstatus);

						// ĥ���� ������ ����ð���?���� ������ �ð��� �����Ѵ�.
						if(pclCM->IsValidID(siSummonID))
						{
							((cltCharServer*)pclCM->CR[siSummonID])->dwTimerClock = pclserver->CurrentClock;
						}
					}
				}
			}
			// �̺�Ʈ ���� �� �̺�Ʈ �ð��� �ƴ� ���?���� �̺�Ʈ ���� �غ�.
			else 
			{
				// 2��, 8��, 14��, 20��
				if((2 != sTime.wHour && 8 != sTime.wHour && 14 != sTime.wHour && 20 != sTime.wHour))
				{
					Thanksgiving = true;	
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////


		//------------------------------
		// �������?��ι�? �̺�Ʈ 
		//------------------------------
		//KHY - 0816 - NHN �������?�ڵ��ߵ� ����.
		//if ( IsWhereServiceArea( ConstServiceArea_Korea ) == false ) //080105 ���м� ���� 
		SpecialQuestAction();

		//KHY - 0115 �߷�Ÿ�ε��� �̺�Ʈ.
		ValentineEventGiveItemAction();

		//KHY - 0218 ȭ��Ʈ���� �̺�Ʈ.
		WhiteEventGiveItemAction();

		//------------------------------
		// �ҷ��� �̺�Ʈ - �������?���� - by LEEKH 2007.10.24
		// 1�� �ֱ��?üũ
		//------------------------------
		if( m_clUpdateTimer_Minute.IsTimed(::GetTickCount()) )
		{
			if(IsWhereServiceArea( ConstServiceArea_Taiwan | ConstServiceArea_Japan | ConstServiceArea_USA ))
			{
				SYSTEMTIME	EventStart;
				SYSTEMTIME	EventEnd;

				SYSTEMTIME	CurrentTime;

				ZeroMemory(&EventStart, sizeof(EventStart));
				ZeroMemory(&EventEnd, sizeof(EventEnd));

				SI32		siItemNum = 0;

				if(IsWhereServiceArea(ConstServiceArea_Taiwan))
				{
					EventStart.wYear = 2007;	EventStart.wMonth = 10;		EventStart.wDay = 30;
					EventEnd.wYear = 2007;		EventEnd.wMonth = 11;		EventEnd.wDay = 13;		EventEnd.wHour = 11;
					siItemNum = 1;
				}
				else if(IsWhereServiceArea(ConstServiceArea_Japan))
				{
					EventStart.wYear = 2007;	EventStart.wMonth = 10;		EventStart.wDay = 31;
					EventEnd.wYear = 2007;		EventEnd.wMonth = 11;		EventEnd.wDay = 7;		EventEnd.wHour = 11;
					siItemNum = 3;
				}
				else if(IsWhereServiceArea(ConstServiceArea_USA))
				{
					EventStart.wYear = 2007;	EventStart.wMonth = 10;		EventStart.wDay = 24;
					EventEnd.wYear = 2007;		EventEnd.wMonth = 11;		EventEnd.wDay = 8;
					siItemNum = 2;
				}

				// �̺�Ʈ �Ⱓ üũ
				GetLocalTime(&CurrentTime);
				if(TimeInBounds(&CurrentTime, &EventStart, &EventEnd))
				{
					// ������ ������ ���� - 1�� ���� ������Ʈ �ϱ⶧���� 0�п� �����ϸ� �ȴ�.
					if(CurrentTime.wMinute == 0)
					{
						SI16 itemunique = ITEMUNIQUE(13025);

						//���� : �Ϻ��� 30�������� �����ϰ� �ϰ�, �븸�� 1�������� �����ϰ� �� (PCK : 07.10.29)
						if(pclClient->IsWhereServiceArea(ConstServiceArea_Japan))
						{
							pclCM->GiveItem(0, itemunique, siItemNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER, 0, 0, 30);
						}
						else if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
						{
							pclCM->GiveItem(0, itemunique, siItemNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER, 0, 0, 1);
						}
						else if(pclClient->IsWhereServiceArea(ConstServiceArea_USA))
						{
							if(CurrentTime.wHour == 12 || CurrentTime.wHour == 15 || CurrentTime.wHour == 18)
							{
								pclCM->GiveItem(0, itemunique, siItemNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER, 0, 0, 10);
							}
						}
						else if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
						{
							if(CurrentTime.wHour == 19)// ���м��� 19�ð� �Ǹ� �������?���� 30������ ���� [07.01.07 �ռ���]
							{
								pclCM->GiveItem(0, itemunique, siItemNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER, 0, 0,30);
							}
						}
					}
				}
			}
		}

		//------------------------------
		// ���� ����
		//------------------------------
		// by LEEKH 2008-04-18 - ���� ���� ����
		//if ( pclTime && pclTime->clDate.uiYear > 1 ) // ���� �ð����� �ּ� 2�����?
		{
			if(siServiceArea == ConstServiceArea_China) // �߱� ����
			{
				bool bwarswitch = false;
				// ���Ǳ��� ������ ���� ���� 8�ÿ� �Ѵ�.
				if( ( sTime.wDayOfWeek == 5 || sTime.wDayOfWeek ==6 ) && 
					( sTime.wHour == 20 && sTime.wMinute < 5 ) )
				{
					bwarswitch = true;
				}

				if(bwarswitch)
				{
					if ( bAutoWarSwitch == true && pclBlackWarManager->IsBlackWar()== false )
					{
						pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour ) ;
					}
				}
			}
			else if(siServiceArea == ConstServiceArea_Korea) // �ѱ� ����
			{
				bool bwarswitch = false;

				if(pclClient->IsCountrySwitch(Switch_BlackWarReform))
				{
					//[���� : Ȳ���� 2008. 5. 19 => ���� ���� �����ϸ鼭 �ѱ� �ߵ��ð� ����. ���� 20���̸鼭 5�� �̳���.]
					if( sTime.wHour == 20 && sTime.wMinute < 5)
					{
						bwarswitch = true;
					}
				}
				else
				{
					// ���Ǳ��� ������ ���� 17��, 21��
					if(  sTime.wHour == 17 && sTime.wMinute < 5 || sTime.wHour == 21 && sTime.wMinute < 5 )
					{
						bwarswitch = true;	
					}
				}

				if(bwarswitch)
				{
					if ( bAutoWarSwitch == true && pclBlackWarManager->IsBlackWar()== false )
					{
						pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour ) ;
					}
				}
			}
			else if(siServiceArea == ConstServiceArea_USA) // �̱� ����
			{
				bool bwarswitch = false;
				// ���Ǳ��� ������ ���� ��, ��, �Ͽ��� 12��, 18��
				if( ((sTime.wDayOfWeek == 6 ) || (sTime.wDayOfWeek == 0 ) || (sTime.wDayOfWeek == 3 ))	&&
					( sTime.wHour == 12 && sTime.wMinute < 5 || sTime.wHour == 18 && sTime.wMinute < 5 ) )
				{
					bwarswitch = true;
				}

				if(bwarswitch)
				{
					if ( bAutoWarSwitch == true && pclBlackWarManager->IsBlackWar()== false )
					{
						pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour ) ;
					}
				}
			}
			else if(siServiceArea == ConstServiceArea_Taiwan) // �븸 ����
			{
				bool bwarswitch = false;

				bool bTime = false;

				if( sTime.wDayOfWeek == 6 && sTime.wHour == 20 && sTime.wMinute >= 30 && sTime.wMinute < 35 )
					bTime = true;

				if( sTime.wDayOfWeek == 0 && sTime.wHour == 20 && sTime.wMinute >= 30 && sTime.wMinute < 35 )
					bTime = true;

				// ���Ǳ��� ������ ���� �����? �Ͽ��� ���� 8�� 30��
				if( bTime )
				{
					bwarswitch = true;
				}

				if(bwarswitch)
				{
					if ( bAutoWarSwitch == true && pclBlackWarManager->IsBlackWar()== false )
					{
						pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour ) ;
					}
				}
			}
			else if ( ConstServiceArea_Japan == siServiceArea ) // �Ϻ�
			{
				if ( IsCountrySwitch(Switch_Server_Event) && pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week"), &sTime) )
				{
					bool bwarswitch = false;


					if( sTime.wHour == 20 && sTime.wMinute >= 0 && sTime.wMinute < 5 )
					{
						bwarswitch = true;
					}


					if(bwarswitch)
					{
						if ( bAutoWarSwitch == true && pclBlackWarManager->IsBlackWar()== false )
						{
							pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour ) ;
						}
					}

				}	// if ( IsCountrySwitch(Switch_Server_Event) ) ����
			}	// else if ( ConstServiceArea_Japan == siServiceArea )	����
		}


		WRITE_MEMLOG( 120, 17, 0, 0 );

		//-------------------------------
		// ���� ���ʽ� Ÿ��. 
		//-------------------------------
		BattleBonusTimeAction();

		//------------------------------
		// ���� ���ʽ� Ÿ�� 
		//------------------------------
		MakeBonusTimeAction();

		WRITE_MEMLOG( 120, 18, 0, 0 );

		//-------------------------------
		//cyj ������ȸ ���� �����ڸ����� �˸�
		//-------------------------------
		TCHAR *pText, *pTitle;
		TCHAR buf[256];
		if (TABS(sTime.wMinute - dwGeneralMeetingNewsMin) >= GENERALMEETING_NEWS_INTERVAL) 
		{
			dwGeneralMeetingNewsMin = sTime.wMinute;

			for (int unique = 0;  unique< MAX_VILLAGE_NUMBER; unique++)
			{
				cltCityHall* pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ unique ];
				if (pclCityHall != NULL && pclCityHall->clGeneralMeetingInfo.siMatterType > 0)
				{
					TCHAR *VillageName = pclVillageManager->GetName( unique );
					pText = GetTxtFromMgr(5742);
					StringCchPrintf(buf, 256, pText, VillageName);
					pTitle = GetTxtFromMgr(5482);

					PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);
				}
			}
		}

		//-------------------------------
		//cyj ���Ǳ��� ���� �����ڸ����� �˸�
		//-------------------------------
		if (TABS(sTime.wHour - dwBlackArmyNewsHour ) >= BLACKARMY_NEWS_INTERVAL) 
		{
			dwBlackArmyNewsHour = sTime.wHour;

			cltServer* pclserver = (cltServer*)pclClient;
			CMinisterByungzo* pclminister = (CMinisterByungzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_BYUNGZO);
			if (pclminister)
			{
				UI32 nowtime = pclClient->GetMinuteFromsTime( sTime );
				UI32 warstarttime = (UI32)pclminister->m_siWarStartTime;

				if ( warstarttime > 0 && warstarttime - nowtime > 0 && 
					warstarttime - nowtime <= 3 * 24 * 60 )
				{
					pTitle = GetTxtFromMgr(5482);
					pText = GetTxtFromMgr(6375);

					_SYSTEMTIME starttime = pclClient->GetTimeFromsMinute( warstarttime );

					StringCchPrintf(buf, 256, pText, starttime.wYear, starttime.wMonth, starttime.wDay, starttime.wHour + 1);
					PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);

					pText = GetTxtFromMgr(6376);
					StringCchPrintf(buf, 256, pText, starttime.wYear, starttime.wMonth, starttime.wDay, starttime.wHour, 
						starttime.wHour + 1);

					PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);

				}

			}

		}


		//-------------------------------
		// ������ Ÿ�� 
		//-------------------------------

		// ������ ���� ���� �ƴ϶��?

		if(bForceStopVillageWarProcess == false)
		{
			bool	bInvalidTime = false;		// ���� �ð� üũ
			if( dwVillageWarStartHour  <= 0 && dwVillageWarStartMin  <= 0 &&
				dwVillageWarFinishHour <= 0 && dwVillageWarFinishMin <= 0 )
			{
				bInvalidTime = true;
			}

			if( bInvalidTime == false &&
				(sTime.wHour >= dwVillageWarStartHour  && sTime.wMinute >= dwVillageWarStartMin) && 
				(sTime.wHour <= dwVillageWarFinishHour && sTime.wMinute <  dwVillageWarFinishMin))
			{
				bForceVillageWarSwitch = true;
			}	
			else  //if(	(sTime.wHour == dwVillageWarFinishHour && sTime.wMinute ==  dwVillageWarFinishMin)  )				
			{
				bForceVillageWarSwitch = false;
			}
		}


		WRITE_MEMLOG( 120, 19, 0, 0 );

		if(bForceVillageWarSwitch == true && bForceStopVillageWarProcess == false && pclClient->siServiceArea != ConstServiceArea_China )
		{
			if(bVillageWarSwitch == false )
			{				
				bVillageWarSwitch = true;

				// ��ü �������� �˸� �޽����� ������. 
				pText = GetTxtFromMgr(5737);
				SendNoticeMsg(-1, 0, 0, pText);


				dwVillageWarNewsMin = sTime.wMinute + VILLAGEWAR_NEWS_INTERVAL;
				//cyj ������ ���� �ڸ� �ٷ� �������� ����
				dwSpecialNews = CurrentClock + 60001; 

				// ���� ������ ���� ���� ������ ������Ʈ �Ǿ����� �ʴٸ� ������Ʈ.

				cltDate clToday;
				clToday.uiYear = sTime.wYear - 2000;
				clToday.uiMonth = sTime.wMonth;
				clToday.uiDay = sTime.wDay;

				if(clToday.GetDateVary() !=	clLastSettedNowVillageWarIinfo.GetDateVary())
				{
					SetTodayVillageWarInfo();
				}


				cltGameMsgResponse_WarVillage clWarVillage(bVillageWarSwitch);
				cltMsg clMsg(GAMEMSG_RESPONSE_WARVILLAGE, 	sizeof(clWarVillage), (BYTE*)&clWarVillage);
				pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

			}

			if (TABS(sTime.wMinute - dwVillageWarNewsMin) >= VILLAGEWAR_NEWS_INTERVAL)
			{
				dwVillageWarNewsMin = sTime.wMinute;

				pText = GetTxtFromMgr(5741);
				pTitle = GetTxtFromMgr(5482);

				PushSpecialNews( pText, pTitle , NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);
			}
		}
		else
		{
			if(bVillageWarSwitch == true)
			{
				bVillageWarSwitch = false;

				// ���?������ ��Ȳ���� ���¸� �����Ѵ�.
				pclVillageManager->SetAllVillageGodtoDB();

				// ��ü �������� �˸� �޽����� ������. 
				pText = GetTxtFromMgr(5738);
				SendNoticeMsg(-1, 0, 0, pText);

				cltGameMsgResponse_WarVillage clWarVillage(bVillageWarSwitch);
				cltMsg clMsg(GAMEMSG_RESPONSE_WARVILLAGE, 	sizeof(clWarVillage), (BYTE*)&clWarVillage);
				pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

				//������ �� �������� �������� ���?�������� ���������� ����.

				NDate clCurDate;
				clCurDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);

				for( SI32 i = 0; i < MAX_VILLAGE_NUMBER; i++)
				{
					if(pclVillageManager->siNowWarVillage[i] > 0)
					{
						sDBRequest_CancelDeclareWar clMsg( 0, i, pclVillageManager->siNowWarVillage[i], &clCurDate );
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
					}
				}

			}
			bVillageWarSwitch = false;
		}

		if ( IsCountrySwitch(Switch_RaccoonEvent_Change_Goonzu) )// [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
		{
			if( pclGoonzuKingGuriEventManager )
			{
				pclGoonzuKingGuriEventManager->Action();
			}
		}
		else
		{
			if( pclKingGuriEventManager )
			{
				pclKingGuriEventManager->Action();
			}
		}

		//KHY - 1113 - ũ�������� �̺�Ʈ - ��ũ�� ��Ÿ ����.
		if( pclEventTimeManager->InEventTime("ChristmasEvent", &sTime) )
		{
			if( pclSantaRaccoon )
			{
				pclSantaRaccoon->Action();	
			}
		}

		// [����] ��ȥ �ý���
		if ( IsCountrySwitch(Switch_WeddingSystem) )
		{
			if ( m_pclMarriageMgr )
			{
				m_pclMarriageMgr->ActionForWedding();
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �ڵ� ���?�ǽ�
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if( bAutoPayDividend == true )
		{
			if( pclTime != NULL && pclTime->clDate.uiMonth == 12 && pclTime->clDate.uiDay == 30 )
			{
				if ( bProcessDB_AutoPayDividend_NEED == true )
				{
					SI32 i;
					for(i = 1;i < MAX_VILLAGE_NUMBER;i++)
					{
						// [����] �ʺ��� ���� : �ʺ��� ������ �ڵ����� ���ִ��� ���� �ʴ´�
						if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
						{
							if ( Const_Beginner_Village == i )
							{
								continue;
							}
						}

						if(pclVillageManager->pclVillageInfo[i])
						{
							cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( i );
							if(pStrInfo == NULL)
								continue;

							// ���� �� �ݾ�- ���� ���ͱ� ���� ��ƾ 1 
							SI64 tempDividendMoney = pStrInfo->clCityhallStrInfo.clProfitMoney.itMoney * 9/ 10;

							if( tempDividendMoney < MIN_AUTODIVIEND_MONEY )
								continue;

							// 1. ���ְ� ���� �з�
							if( pStrInfo->clCityhallStrInfo.siTotalStockAmount <= 0 )
								continue;
							cltMoney tempStockDividendMoney;
							tempStockDividendMoney.Set( tempDividendMoney * 55 / 100 );

							if( tempStockDividendMoney.itMoney / pStrInfo->clCityhallStrInfo.siTotalStockAmount <=0 )
								continue;

							// 2. �ֹ��� ���� �з�
							if( pStrInfo->clCityhallStrInfo.siResidentNum <= 0 )
								continue;

							cltMoney tempResidentDividendMoney;
							tempResidentDividendMoney.Set( tempDividendMoney * 3 / 10 );

							if( tempResidentDividendMoney.itMoney / pStrInfo->clCityhallStrInfo.siResidentNum <=0 )
								continue;

							// 3. �ս� ���� ����
							cltMoney tempTaxDividendMoney;
							tempTaxDividendMoney.Set( tempDividendMoney * 9 / 100 );

							if( tempTaxDividendMoney.itMoney <= 0 )
								continue;

							// 4. �����?����
							SI64 tempChiefDividendMoney = tempDividendMoney * 5 / 100;
							// 5. ���� ����
							SI64 tempKingDividendMoney = tempDividendMoney * 1 / 100;

							SI64 tempAllMoney = tempStockDividendMoney.itMoney + 
								tempResidentDividendMoney.itMoney + 
								tempTaxDividendMoney.itMoney +
								tempChiefDividendMoney + 
								tempKingDividendMoney;
							if(tempAllMoney > tempDividendMoney || tempAllMoney < 0 )	continue	;
							sDBRequest_AutoPayDividend clMsg(1, i, &tempStockDividendMoney, &tempResidentDividendMoney, &tempTaxDividendMoney,
								tempChiefDividendMoney, tempKingDividendMoney, 1, pclTime->clDate.uiYear, &pclTime->clDate );

							SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

							pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempResidentDividendMoney.itMoney);
							pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempChiefDividendMoney);
							pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempKingDividendMoney);
						}
					}	

					bProcessDB_AutoPayDividend_NEED = false ;
				}
			}
			else
			{
				bProcessDB_AutoPayDividend_NEED = true;
			}
		}

		// ��¥�� ����Ǹ�?���?������ ���� ������ ������ �ʱ�ȭ �Ѵ�.
		if(sTime.wDay != siLastVillageWarKillNumClearDay )
		{
			SI32 CharID = 0;
			SI32 index = 0;
			while( CharID = pclCM->GetCharOrder( index ))
			{
				index++;

				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[CharID];
				if(IsPC(CharID))
				{
					pclchar->siVillageWarKillScore = 0;
				}
			}

			siLastVillageWarKillNumClearDay = sTime.wDay;
		}


		if(sTime.wHour == 6 && sTime.wMinute == 30 )
		{
			if(bProcessDB_WealthRank_NEED == true)
			{
				sDBRequest_DBSystemInfo clMsg( 0, 0, DBSYSTEMINFO_CALCULATE_WEALTHRANK,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
			}

			bProcessDB_WealthRank_NEED = false;
		}

		// DB���� �ڻ������?����ϵ���?�Ѵ�. ���� 6�� 30��
		if(sTime.wHour == 6 && sTime.wMinute == 30 )
		{
			if(bProcessDB_WealthRank_NEED == true)
			{
				sDBRequest_DBSystemInfo clMsg( 0, 0, DBSYSTEMINFO_CALCULATE_WEALTHRANK,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
			}

			bProcessDB_WealthRank_NEED = false;
		}
		else
		{
			bProcessDB_WealthRank_NEED = true;
		}
		// DB���� ����ȭ���� ����ϵ���?�Ѵ�. ���� 7�� 15��
		SI32 siHour		= 7;
		SI32 siMinute	= 15;

		// �̱��� 0��
		if( siServiceArea == ConstServiceArea_USA )
		{
			siHour		= 0;
			siMinute	= 0;
		}
		if( sTime.wHour == siHour && sTime.wMinute == siMinute )
		{
			if ( bProcessDB_TotalMoney_NEED == true )
			{
				sDBRequest_DBSystemInfo clMsg( 0, 0, DBSYSTEMINFO_CALCULATE_TOTALMONEY,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		

				// ���� ��ȭ���� ��û�Ѵ�.
				sDBRequest_DBSystemInfo clMsgReq( 0, 0, DBSYSTEMINFO_GET_TOTALMONEY,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsgReq);		

				sDBRequest_DBSystemInfo clMsgReq2( 0, 0, DBSYSTEMINFO_GET_TOTALMONEY_YESTERDAY,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsgReq2);		
			}

			bProcessDB_TotalMoney_NEED = false ;
		}
		else
		{
			bProcessDB_TotalMoney_NEED = true ;
		}
		// DB���� �ѹ鼺��(��ĳ���ͼ�)�� ����ϵ���?�Ѵ�. ���� 7�� 20��
		if ( sTime.wHour == 7 && sTime.wMinute == 20 )
		{
			if ( bProcessDB_TotalChar_NEED == true )
			{
				sDBRequest_DBSystemInfo clMsg( 0, 0, DBSYSTEMINFO_CALCULATE_TOTALCHARNUM,sTime);
				pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
			}

			bProcessDB_TotalChar_NEED = false ;
		}
		else
		{
			bProcessDB_TotalChar_NEED = true ;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ���� ���� ������ �������� ������ ���?Ŭ���̾�Ʈ�鿡�� �˷� ������Ʈ ��Ų��.
		// ���� �����ϴ� Ŭ���̾�Ʈ�� ������Ʈ �� ������ �޴´�.
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// ������ ����


		if ( sTime.wHour == 6 && sTime.wMinute == 30 )
		{
			if ( bProcess_SetNowVillageWarInfo == true )
			{
				SetTodayVillageWarInfo();
			}

			bProcess_SetNowVillageWarInfo = false ;
		}
		else
		{
			bProcess_SetNowVillageWarInfo = true ;
		}

		////////////////////////////////////////////////////////////////////////////////////
		// ���ֿ��� �������� ����Ŭ�ι��� �����Ѵ�
		// ���� ����ϰ�?�ִ� ����Ŭ�ι��� �����Ѵ�. - by LEEKH 2008-08-18
		////////////////////////////////////////////////////////////////////////////////////
		static SI32 lastGoonzuItemGiveDay = -1;
		if( sTime.wDay != lastGoonzuItemGiveDay && sTime.wHour == 0 && sTime.wMinute == 0 )
		{	
			if( pclMinisterMgr && pclMinisterMgr->m_MinisterBase[MINISTER_GOONZU]->m_szPersonName[0] != 0 )
			{
				cltItem clInputItem;
				SI16	fourleafItemUnique = ITEMUNIQUE(siFourLeafUnique);
				SI16	itemnum	=	100;
				SI16 bRareSwitch = 0;
				pclClient->pclItemManager->MakeRandItemUnique( fourleafItemUnique ,&clInputItem,0,0,&bRareSwitch);
				clInputItem.siItemNum = itemnum;

				SendPostOfficeItemToPerson(pclMinisterMgr->m_MinisterBase[MINISTER_GOONZU]->m_szPersonName,&clInputItem,NULL,0);
			}			

			lastGoonzuItemGiveDay = sTime.wDay;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// ���� ������ ����Ѵ�?
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if(sTime.wHour == 7 && sTime.wMinute == 30 )
		{
			if( bProcessDB_TotalVillageScore_NEED == true )
			{
				SI32 userindex = 0;
				SI32 userid;

				while(userid = pclCM->GetCharOrder(userindex))
				{
					userindex++;

					// ���ʹ� ����!!
					if ( IsPC(userid) == false )
						continue;

					if(pclCM->CR[userid]->GetSession() == NULL)
						continue;

					cltCharServer* pclchar = (cltCharServer*)pclCM->CR[userid];

					//-------------------------------------
					// ���� �ð��� ���Ѵ�. 
					//-------------------------------------
					pclchar->CalcPlayTime();

					//-------------------------------------
					// ���Ե� ��q���� �ִٸ� �ð��� DB�� ������
					//-------------------------------------
					if(pclchar->pclCI->clBI.siHomeVillage > 0 && pclchar->pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT)
					{
						SI32 personid = pclchar->pclCI->GetPersonID();

						sDBRequest_VillageJoinPlayTime clMsg(personid, pclchar->pclCI->clBI.clVillageScoreInfo.siPlayTime, pclchar->pclCI->clBI.siHomeVillage);
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					}
				}

				SI32 siVillageUnique[MAX_VILLAGE_NUMBER];
				SI32 siVillageResidentsNumber[MAX_VILLAGE_NUMBER];
				SI32 siVillageVisitorsNumber[MAX_VILLAGE_NUMBER];

				SI32 i;
				for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
				{
					siVillageUnique[i] = 0;
					siVillageResidentsNumber[i] = 0;
					siVillageVisitorsNumber[i] = 0;
				}

				for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
				{
					if(pclVillageManager->pclVillageInfo[i] == NULL)
						continue;

					siVillageUnique[i] = pclVillageManager->pclVillageInfo[i]->siVillageUnique;

					cltStrInfo* pclstrinfo = pclCityHallManager->GetStrInfo(i);
					if(pclstrinfo)
					{
						siVillageResidentsNumber[i] = pclstrinfo->clCityhallStrInfo.siResidentNum;
					}
					siVillageVisitorsNumber[i] = pclVillageManager->pclVillageInfo[i]->GetVisitNum();
				}

				sDBRequest_VillageResidentsVisitorsInfo	clMsg;
				SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

				bProcessDB_TotalVillageScore_NEED = false;
			}
		}
		else
		{
			bProcessDB_TotalVillageScore_NEED = true;
		}

		// DB���� �Ǽ����� ������ ������ ������Ʈ�����ش�. ���� 7�� 35��
		if ( sTime.wHour == 7 && sTime.wMinute == 35 )
		{
			if ( bProcessDB_GetMinister_NEED == true && pclMinisterMgr )
				pclMinisterMgr->InitializeDB();

			bProcessDB_GetMinister_NEED = false ;
		}
		else
		{
			bProcessDB_GetMinister_NEED = true ;
		}
		// ������ ĳ���� ���� �� ���� ���� �ʱ�ȭ���ش�.
		if ( sTime.wHour == 0 && sTime.wMinute == 0 )
		{
			if ( bProcess_InitCalcCharNum == true )
			{
				siNewCharNum = 0 ;
				siDelCharNum = 0 ;
			}

			bProcess_InitCalcCharNum = false ;
		}
		else
		{
			bProcess_InitCalcCharNum = true ;
		}

		// ������ �̺�Ʈ ������ �ʱ�ȭ���ش�.
		if ( sTime.wHour == 0 && sTime.wMinute == 1 )
		{
			if ( bProcess_InitGameEvent == true )
			{
				pclGameEventManager->Init();
			}

			bProcess_InitGameEvent = false ;
		}
		else
		{
			bProcess_InitGameEvent = true ;
		}

		WRITE_MEMLOG( 120, 20, 0, 0 );

		//----------------------------
		// ���� 
		//----------------------------
		if(pclNoticeManager)
		{
			pclNoticeManager->Action();
		}

		if(pclBlackWarManager)
		{
			pclBlackWarManager->Action();
		}

		if(pclBlackArmyManager)
		{
			pclBlackArmyManager->Action();
		}

		//	[��ȣ] 20090527 ��ȭ�� ���� �Ŵ��� Action
		if(IsCountrySwitch(Switch_BalanceCurrency) == true)
		{
			if(m_pclBalanceCurrencyMgr != NULL)
				m_pclBalanceCurrencyMgr->Action();
		}

		//-----------------------------------
		// [����] ���Һ� ���� �޴��� Action
		//-----------------------------------
		if(IsCountrySwitch(Switch_BokBulBok)  == true )
		{
			if(m_pServer_BBBMgr != NULL )
				m_pServer_BBBMgr->Action(); //CServer_BBBMgr
		}

		// [����] ��Ƽ��Ī �Ŵ��� (����)
		if ( true == IsCountrySwitch(Switch_PartyQuest_PartyMatching) )
		{
			if ( NULL != m_pServer_PartyMatchingMgr )
			{
				m_pServer_PartyMatchingMgr->Action();
			}
		}

		if(GetFrame() % 3000 == 0)
		{
			if(pclKoinoboryManager)	
			{
				pclKoinoboryManager->Action();
			}

			if(pclSantaRaccoonManager)
			{	
				pclSantaRaccoonManager->Action();
			}

			if(pclKoinoboryFamilyManager )
			{
				pclKoinoboryFamilyManager->Action();
			}

		}

		WRITE_MEMLOG( 120, 20, 0, 1 );

		static WORD lastUpdateHour = sTime.wHour;

		if( sTime.wHour != lastUpdateHour )
		{
			cltItem clItem;
			clItem.Init();

			bool rVal = pclGMManager->GetSupplyItem( &clItem );

			if( rVal == true && clItem.siUnique > 0 ) 
			{
				SI32 itemUnique = clItem.siUnique;
				SI32 itemNum = clItem.siItemNum;
				UI16 itemUsedate = clItem.uiDateUseDay;

				TCHAR * msg = pclGMManager->GetSupplyMsg();
				TCHAR * pText = GetTxtFromMgr(5614);
				SI32 id = 0;
				SI32 index = 0;

				while(id = pclCM->GetCharOrder(index))
				{
					index++;

					if(pclCM->IsAlive(id) && IsPC(pclCM->CR[id]->GetCharUnique()) )
					{	
						cltCharServer * pclChar = (cltCharServer *)pclCM->CR[id];

						pclChar->GiveItem( itemUnique ,  itemNum , INPUTITEMPERSON_OPTION_EVENT , itemUsedate );						

						if( msg != NULL && msg[0] != 0 )
						{
							cltGameMsgResponse_Notice clNotice(pText, msg,true);
							cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);
							pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
						}

					}
				}

			}

			lastUpdateHour = sTime.wHour;
		}

		WRITE_MEMLOG( 120, 20, 0, 2 );
		WRITE_MEMLOG( 120, 21, 0, 0 );

		//--------------------------
		// �ֽ� 
		//--------------------------
		pclStockDistributionMgr->Action();

		//--------------------------
		// ���� ��ȣ�� ����. 
		//--------------------------
		if(GetFrame() % 5000  == 0)
		{
			pclKindInfoSet->WritePreferenceCharData(sTime);
		}

		WRITE_MEMLOG( 120, 22, 0, 0 );

		//------------------------------
		// ���ο� �Ź� ���?��û�Ѵ�. 
		//------------------------------
		if(GetFrame() % 2000 == 0)
		{
#ifdef _DEBUG
			// ���ʷ� �ð��� ������ ���̶��? 
			if(sLastNewsTime.wYear == 0)
			{
				sLastNewsTime = sTime;
			}

			sDBRequest_News clMsg(&sLastNewsTime);
			pclGameDBProcess2->SendMsg((sPacketHeader *)&clMsg);

			sLastNewsTime	= sTime;
#endif
			if( siServiceArea == ConstServiceArea_Japan )
			{
				// ���ʷ� �ð��� ������ ���̶��? 
				if(sLastNewsTime.wYear == 0)
				{
					sLastNewsTime = sTime;
				}

				sDBRequest_News clMsg(&sLastNewsTime);
				pclGameDBProcess2->SendMsg((sPacketHeader *)&clMsg);

				sLastNewsTime	= sTime;
			}
		}

		//-------------------------------------------
		// ������ �ֽ� ��Ȳ�� Ŭ���̾�Ʈ�� �뺸�Ѵ�.
		//-------------------------------------------
		static SI32 stockvillageunique = 1;
		if(GetFrame() % 100 == 0)
		{
			GMONEY sellprice = 0, buyprice = 0;
			SI32 sellamount = 0, buyamount = 0;

			cltStrInfo *pclStrInfo = pclCityHallManager->GetStrInfo( stockvillageunique);
			if(pclStrInfo && pclStrInfo->clCityhallStrInfo.siTotalStockAmount > 0)
			{

				// ���� �ֹ��߿��� ���� ���� ������ �ֹ��� ã�´�. 
				if(pclStockManager->pclOrderManager->FindLowPriceStockOrderInfo(stockvillageunique, &sellprice, &sellamount) == true)
				{

				}
				// ���� �ֹ��߿��� ���� ���� ������ �ֹ��� ã�´�. 
				if(pclStockManager->pclOrderManager->FindHighPriceStockOrderInfo(stockvillageunique, &buyprice, &buyamount) == true)
				{
				}

				if(sellprice || buyprice)
				{
					cltGameMsgResponse_StockTradeInfo clinfo(stockvillageunique, sellprice, sellamount, buyprice, buyamount);
					cltMsg clMsg(GAMEMSG_RESPONSE_STOCKTRADEINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->SendNewsMsg((sPacketHeader*)&clMsg, CAPTIONKIND_STOCK);
				}

			}

			stockvillageunique++;
			stockvillageunique %= MAX_VILLAGE_NUMBER;


		}

		WRITE_MEMLOG( 120, 23, 0, 0 );

		//-----------------------------
		// ���� �ֹ��� ��û�Ѵ�. 
		//-----------------------------
		if(TABS(CurrentClock - dwLastStockOrderClock) > 60000)
		{
			dwLastStockOrderClock = CurrentClock;

			sDBRequest_GetManyStockOrder clMsgSell(0, ORDERMODE_SELL, siLastStockOrderIndex);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgSell);

			sDBRequest_GetManyStockOrder clMsgBuy( 0, ORDERMODE_BUY, siLastStockOrderIndex);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgBuy);

			siLastStockOrderIndex += GET_MANY_STOCK_ORDER_NUM;
			siLastStockOrderIndex %= MAX_STOCK_ORDER_NUMBER;
		}

		//-------------------------------------
		// �� ������ ���� �����?���� �����Ѵ�. 
		//-------------------------------------
		static DWORD dwLastVillageUserNumClock = 0;
		if(TABS(CurrentClock - dwLastVillageUserNumClock) > 10000)
		{
			dwLastVillageUserNumClock = CurrentClock;

			pclVillageManager->GetCurrentVillageUserNum();

			//-------------------------------------
			// ������ �����?���� �����Ѵ�. 
			//-------------------------------------
			si1To19UserNum		= 0;
			si20To39UserNum		= 0;
			si40To59UserNum		= 0;
			si60To79UserNum		= 0;
			si80To99UserNum		= 0;
			si100To119UserNum	= 0;
			si120To150UserNum	= 0;

			SI32 id;

			SI32 index = 0;
			while(id = pclCM->GetCharOrder(index))
			{
				index++;

				if(IsPC(pclCM->CR[id]->GetCharUnique()) == false)continue;

				if(pclCM->CR[id]->pclCI->clIP.GetLevel()		< 20)si1To19UserNum++;
				else if(pclCM->CR[id]->pclCI->clIP.GetLevel()	< 40)si20To39UserNum++;
				else if(pclCM->CR[id]->pclCI->clIP.GetLevel()	< 60)si40To59UserNum++;
				else if(pclCM->CR[id]->pclCI->clIP.GetLevel()	< 80)si60To79UserNum++;
				else if(pclCM->CR[id]->pclCI->clIP.GetLevel()	< 100)si80To99UserNum++;
				else if(pclCM->CR[id]->pclCI->clIP.GetLevel()	< 120)si100To119UserNum++;
				else si120To150UserNum++;

			}


		}

		WRITE_MEMLOG( 120, 24, 0, 0 );

		//-----------------------------------
		// �����۸��� �ּ� �ü��� ����Ѵ�? : �ź� 20�ʿ� ���?
		//-----------------------------------
		if( bChangeSecond && sTime.wSecond == 20 )
		{
			pclMarketManager->MakeLowPriceOrderInfo();
		}

		//-----------------------------------
		// ��ǰ �ü��� �����Ѵ�. : �ź� 25�ʿ� ���?
		//-----------------------------------
		//static dwLastCalcPriceClock = 0;
		//if(TABS(CurrentClock - dwLastCalcPriceClock) > 5000)
		if( bChangeSecond && sTime.wSecond == 25 )
		{
			//dwLastCalcPriceClock = CurrentClock;
			pclItemManager->CalcItemPrice(sTime);
		}

		// 10�п� �ѹ��� ����ü���?���?�����Ϳ� �����Ѵ�.
		if( bChangeMinute && sTime.wMinute % 10 )
		{
			for( SI32 i = 1; i < MAX_ITEMINFO_NUMBER; i++ )
			{
				if( pclItemManager->pclItemInfo[i] )
				{
					SI32 itemunique = pclItemManager->pclItemInfo[i]->clItem.siUnique;
					cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, itemunique );
					if( pclprice->siPrice > 0 )
					{
						NStatistics_Set( "Item", itemunique, "Price", pclprice->siPrice );
						NStatistics_Set( "Item", itemunique, "Name", pclItemManager->GetName(itemunique) );
					}
				}
			}
		}

		//-----------------------------------
		// DailyQuest - ����Ʈ�� ���� ����Ʈ�� �־��ְ�, �����?�ð� ������ ���� �ʴ� ����Ʈ�� ���� ��Ų�Ŀ� �� ����Ʈ ����.
		//-----------------------------------
		static DWORD dwDailyQuestListClock = 0;
		if(TABS( CurrentClock - dwDailyQuestListClock ) > 1000 )
		{
			dwDailyQuestListClock  = CurrentClock;
			pclDailyQuestMgr->Action( CurrentClock );
		}
		//-----------------------------------
		// DailyQuest2 - ����Ʈ�� ���� ����Ʈ�� �־��ְ�, �����?�ð� ������ ���� �ʴ� ����Ʈ�� ���� ��Ų�Ŀ� �� ����Ʈ ����.
		//-----------------------------------
		static DWORD dwDailyQuest2ListClock = 0;
		if(TABS( CurrentClock - dwDailyQuest2ListClock ) > 1000 )//���ϸ� ����Ʈ 2
		{
			dwDailyQuest2ListClock  = CurrentClock;
			pclDailyQuest2Mgr->Action( CurrentClock );
		}
		//-----------------------------------
		// DailyQuest3 - ����Ʈ�� ���� ����Ʈ�� �־��ְ�, �����?�ð� ������ ���� �ʴ� ����Ʈ�� ���� ��Ų�Ŀ� �� ����Ʈ ����.
		//-----------------------------------
		static DWORD dwDailyQuest3ListClock = 0;
		if(TABS( CurrentClock - dwDailyQuest3ListClock ) > 1000 )//���ϸ� ����Ʈ 3
		{
			dwDailyQuest3ListClock  = CurrentClock;
			pclDailyQuest3Mgr->Action( CurrentClock );
		}

		WRITE_MEMLOG( 120, 25, 0, 0 );

		//------------------------------------
		// �Ұ��縮 ���?�̺�Ʈ 
		//------------------------------------
		pclEventHunting->Action();

		//------------------------------------
		// ���ĸ��� ���� �̺�Ʈ 
		//------------------------------------
		pclMofuMofuEvent->Action();

		//------------------------------------
		//[����] ���� �̺�Ʈ - ���� ����. 2008-3-26
		//------------------------------------
		pclMonsterAttackEvent->Action();

		if(IsCountrySwitch(Switch_Helper))
		{
			if( 10 == sTime.wSecond ||
				40 == sTime.wSecond		)
			{
				pclHelperManager->Action();
			}
		}

		if(IsCountrySwitch(Switch_SystemMarketSell))
		{
			SystemMarketSell();		
		}

		if(IsCountrySwitch(Switch_Server_Event))
		{
			static bool bServerEventEndMsg = true;

			//���� �̺�Ʈ ���� �Ⱓ�̸�...
			if( pclEventTimeManager->InEventTime("GoonzuDay_Week", &sTime) )
			{
				pclServerEventManager->ActionGoonzuDay();
			}
			else if( pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week_Reward"), &sTime) )
			{
				pclServerEventManager->ActionGoonzuDayReward();

				if( bServerEventEndMsg )
				{
					bServerEventEndMsg = false;
					pclServerEventManager->SetServerEventState( SERVER_EVENT_STATE_WAIT );
					pclServerEventManager->DeleteGate();
				}

				static DWORD dwTickCount = GetTickCount();
				if( 10000 < GetTickCount() - dwTickCount )
				{
					dwTickCount = GetTickCount();

					cltServerResponseValue clBufreturn(0, SRVAL_SERVER_EVENT_END, 1, 0);
					cltMsg clBuMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clBufreturn);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clBuMsg);
				}
			}
			else
			{
				// ���� ������ �ö��� �� �̺�Ʈ ���� ���� �̺�Ʈ�� �ƴϸ� �ʱ�ȭ�� ��û����.
				static bool bInitServerEventMsg = true;

				if( false == bServerEventEndMsg || true == bInitServerEventMsg)
				{
					bServerEventEndMsg  = true;
					bInitServerEventMsg = false;

					pclServerEventManager->ServerEventStateReady();

					sDBRequest_Server_Event_Clear clMsg(pclClient->siServerUnique );
					((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

					// ĳ������ �δ����� ������ ������ �����Ѵ�.
					SI32			siID	= 0;
					SI32			siIndex	= 0;
					cltCharServer*	pclchar	= NULL;
					while( siID = pclClient->pclCM->GetCharOrder(siIndex) )
					{
						siIndex++;
						if ( false == IsPC( siID ) ) continue;

						pclchar = pclCM->GetCharServer(siID);
						if ( NULL == pclchar )	continue;

						pclchar->siServerEvent_IndunScore = 0;
					}

					for( SI32 siMapIndex = 0; siMapIndex < MAX_MAP_NUMBER; siMapIndex++ )
					{
						if( false == pclMapManager->IsInstanceMap(siMapIndex) )		continue;

						SI16 siMapType = pclMapManager->GetInstanceMapType( siMapIndex );
						if( INSTANCEMAP_TYPE_SERVER != siMapType ) continue;

						SI32 siParentMap = pclMapManager->GetParentMap( siMapIndex );
						if( siParentMap <= 0 )	continue;

						cltMapCommon* pclMap = pclMapManager->GetMapPointer( siParentMap );
						if ( NULL == pclMap )	continue;
						if( pclMap == NULL )	continue;

						pclMap->m_siInstanceMapScore = 0;
					}

					cltServerResponseValue clBufreturn(0, SRVAL_SERVER_EVENT_END, 0, 0);
					cltMsg clBuMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clBufreturn);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clBuMsg);
				}
			}
		}

		//KHY - 1113 - ũ�������� �̺�Ʈ - �̺�Ʈ ������ npc�����?�κ�.
		// [���? ���縮�� �տ� ����Ʈ�� ����°�?�߰� (09.10.22)
		if( pclEventTimeManager->InEventTime("ChristmasEvent", &sTime) )
		{
			SI32	siChiefCount = 0;
			SI32	siChristmasTreeCount = 0;
			SI32	siEventNPCKind[MAX_HIRED_EVENTNPC_NUM];

			for(SI16 i = 0; i < EVENT_NPC_COUNT ; i++)
			{
				if (siEventNPCId[i] > 0 && pclCM->IsValidID(siEventNPCId[i]))
				{
					SI32 index = 0;
					SI32 id	   = 0;
					while(id = pclClient->pclCM->GetCharOrder(index))
					{
						index++;
						// �������?�Ŀ��Ը� �����Ѵ�. 
						if(true == pclClient->pclCM->CR[id]->IsValidConnection())
						{
							// �̹� �������?�ִ�.
							if(siEventNPCId[3] > 0 )
							{
								siWorldTreeID[0] = siEventNPCId[3];
								SendServerResponseMsg(0, SRVAL_RETURN_WORLDCHRISTMASTREE3_SUCCESS, siWorldTreeID[0], 0,pclCM->CR[id]->GetCharUnique());
							}

							if(siEventNPCId[4] > 0)
							{	
								if(TABS(CurrentClock - dwWorldTreeCreateClock) <= 7200000)
								{
									siWorldTreeID[1] = siEventNPCId[4];
									SendServerResponseMsg(0, SRVAL_RETURN_WORLDCHRISTMASTREE2_SUCCESS, siWorldTreeID[1], 0,pclCM->CR[id]->GetCharUnique());
								}
								else
								{
									siWorldTreeID[1] = 0;
									bWorldTreeConstruct = false;
								}
							}
						}
					}
				}
				else
				{
					//������ �Ѵ�.
					SI32 ChristmastreeKind	= pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"));//ũ�������� Ʈ��(������) ī�ε� ã��.
					SI32 RudolphKind	= pclClient->GetUniqueFromHash(TEXT("KIND_RUDOLPH")); // �絹�� ī�ε�
					SI32 siWorldChristmasTreeKind01 = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01")); //����Ʈ�� ī�ε�1
					SI32 siWorldChristmasTreeKind02 = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02")); //����Ʈ�� ī�ε�2

					SI32 NPCcharKind = 0;
					SI32 x = 0, y = 0, tempx = 0, tempy = 0;
					SI32 mapIndex = 0;

					switch(i)
					{
					case 0: 
						{
							NPCcharKind = ChristmastreeKind; // ũ�������� Ʈ��

							tempx = 19;
							tempy = 98;
							mapIndex = 158;// ���縮�� ��
						}
						break;
					case 1: 
						{
							NPCcharKind = ChristmastreeKind; // ũ�������� Ʈ��

							tempx = 26;
							tempy = 42;
							mapIndex = 179;// ������ ��ó.
						}
						break;
					case 2: // ���縮�� �α�
						{
							NPCcharKind = RudolphKind; // �絹�� ����Ʈ�� NPC

							tempx = 296;
							tempy = 328;
							mapIndex = 0;// ���縮�� ��ó- �߿�
						}
						break;
					case 3: // ���縮�� �α�
						{
							for( SI32 j = 0; j < MAX_VILLAGE_NUMBER; j++)
							{
								// ��ī�� ������ ���� �˻����� �����Ѵ�.
								if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
								{
									if(pclClient->pclVillageManager->pclVillageInfo[j])
									{
										if( pclClient->pclVillageManager->pclVillageInfo[ j ]->siVillageUnique == Const_Beginner_Village ) 
										{
											continue;
										}
									}

								}

								SI32 siChiefID = 0;
								cltStrInfo* pclStrInfo = pclCityHallManager->GetStrInfo( j );
								cltRank* pclRank = pclRankManager->GetRank(RANKTYPE_CITYHALL, j);
								
								if(pclRank != NULL)
								{
									siChiefID = pclRank->clPerson.GetPersonID();																
								}
	
								// 1. ������ ���ְ� �ִ��� �˻��Ѵ�.
								if(siChiefID > 0)
								{
									siChiefCount++;
								}

								// 2. �� ������ 2�ܰ� ũ�������� Ʈ���� �ִ��� �˻��Ѵ�.
								if(pclStrInfo)
								{
									for(SI32 k = 0; k<MAX_HIRED_EVENTNPC_NUM; k++)
									{
										siEventNPCKind[k] = pclStrInfo->clCityhallStrInfo.clHiredEventNPC[k].siNPCKind;

										if(siEventNPCKind[k] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))
										{
											siChristmasTreeCount++;
										}
									}
								}
							}

							// 3. ���縮�� �տ� ���ο� ũ�������� Ʈ���� �Ǽ��Ѵ�.
							if( siChiefCount > 0 && siChristmasTreeCount > 0 /*&& (siChiefCount == siChristmasTreeCount)*/
								 && bWorldTreeConstruct == false)
							{
								if (siEventNPCId[4] > 0 && pclCM->IsValidID(siEventNPCId[4]))
								{
									pclCM->CR[siEventNPCId[4]]->DeleteInCell();
									pclCM->DeleteChar(siEventNPCId[4]);

									siEventNPCId[4] = 0;
								}

								NPCcharKind = siWorldChristmasTreeKind01;
								siWorldTreeID[1] = 0;
								siEventNPCId[4] = 0;

								// ���� Ʈ�� ���¸� ������ �����Ѵ�.
								TCHAR szFileName[256] = "";
								StringCchPrintf(szFileName, sizeof(szFileName), TEXT("Config\\WorldTree.dat"));

								FILE *fp = fopen(szFileName, "w+");
								if( fp != NULL)
								{
									fprintf(fp, "%d\n", bWorldTreeConstruct);
									fclose( fp );
								}

								tempx = 290;
								tempy = 372;
								mapIndex = MAPINDEX_MAINFIELD;	
							}
						}
						break;
					case 4 : // ���縮�� �α�
						{
							if(siChiefCount > 0 && siChristmasTreeCount > 0 /*&& (siChiefCount == siChristmasTreeCount)*/
								&& bWorldTreeConstruct == true)
							{
								// �ִٸ� ������ �Ѵ�.
								if (siEventNPCId[3] > 0 && pclCM->IsValidID(siEventNPCId[3]))
								{
									pclCM->CR[siEventNPCId[3]]->DeleteInCell();
									pclCM->DeleteChar(siEventNPCId[3]);

									siEventNPCId[3] = 0;
								}

								NPCcharKind = siWorldChristmasTreeKind02;
								siWorldTreeID[0] = 0;
								siEventNPCId[3] = 0;

								tempx = 290;
								tempy = 372;
								mapIndex = MAPINDEX_MAINFIELD;

								// Ʈ���� ������ �ð��� ����Ѵ�?
								dwWorldTreeCreateClock = CurrentClock;

								TCHAR szFileName[256] = "";
								StringCchPrintf(szFileName, sizeof(szFileName), TEXT("Config\\WorldTreeClock.dat"));

								FILE *fp = fopen(szFileName, "w+");
								if( fp != NULL)
								{
									fprintf(fp, "%d\n", dwWorldTreeCreateClock);
									fclose( fp );
								}

								// ���� Ʈ�� ���¸� ������ �����Ѵ�.
								TCHAR szFileName1[256] = "";
								StringCchPrintf(szFileName1, sizeof(szFileName), TEXT("Config\\WorldTree.dat"));

								FILE *fp1 = fopen(szFileName1, "w+");
								if( fp1 != NULL)
								{
									fprintf(fp1, "%d\n", bWorldTreeConstruct);
									fclose( fp1 );
								}
							}
						}
						break;

					}								

					if(pclMapManager->pclMapArray[mapIndex]->FindEmptyAreaForCharLocate(NPCcharKind, tempx, tempy, 20, &x, &y, true) == false)
					{
						continue;
					}

					siEventNPCId[i] = SetNPCChar(NPCcharKind, VILLAGE_NONE, NULL, 1, 0, x, y, mapIndex, NPCITEMMODE_NONE,NULL );

					// ����Ʈ���� �������� ��ü������ �ڸ����� �˸���.
					SI32 index = 0;
					SI32 id	   = 0;
					while(id = pclClient->pclCM->GetCharOrder(index))
					{
						index++;
						// �������?�Ŀ��Ը� �����Ѵ�. 
						if(true == pclClient->pclCM->CR[id]->IsValidConnection())
						{
							if(siEventNPCId[3] > 0)
							{
								siWorldTreeID[0] = siEventNPCId[3];
								SendServerResponseMsg(0, SRVAL_RETURN_WORLDCHRISTMASTREE_SUCCESS, siWorldTreeID[0], 0,pclCM->CR[id]->GetCharUnique());
							}

							if(siEventNPCId[4] > 0)
							{
								siWorldTreeID[1] = siEventNPCId[4];
								SendServerResponseMsg(0, SRVAL_RETURN_WORLDCHRISTMASTREE2_SUCCESS, siWorldTreeID[1], 0, pclCM->CR[id]->GetCharUnique());
							}
						}
					}

					if ( siEventNPCId[3] > 0 )
					{
						NTCHARString128 strTitle = GetTxtFromMgr(10311);
						NTCHARString128	strText = GetTxtFromMgr(20093);

						PushSpecialNews(strTitle, strText, NULL, CAPTIONKIND_NEWS, CAPTIONKIND_NEWS);
					}
				}
			}
		}
		else
		{
			for(SI16 i = 0; i < EVENT_NPC_COUNT ; i++)
			{
				// �ִٸ� ������ �Ѵ�.
				if (siEventNPCId[i] > 0 && pclCM->IsValidID(siEventNPCId[i]))
				{
					pclCM->CR[siEventNPCId[i]]->DeleteInCell();
					pclCM->DeleteChar(siEventNPCId[i]);

					if(siEventNPCId[3] > 0 || siEventNPCId[4] > 0)
					{
						SI32 index = 0;
						SI32 id	   = 0;
						while(id = pclClient->pclCM->GetCharOrder(index))
						{
							index++;
							// �������?�Ŀ��Ը� �����Ѵ�. 
							if(true == pclClient->pclCM->CR[id]->IsValidConnection())
							{
								SendServerResponseMsg(0, SRVAL_RETURN_WORLDCHRISTMASTREE_DEL, 0, 0, pclCM->CR[id]->GetCharUnique());
							}
						}
					}

					siEventNPCId[i] = 0;					
				}
			}	
		}


		/*
		if(IsCountrySwitch(Switch_Helper))
		{
		pclHelperManager->Action();
		}
		*/

		//------------------------------------
		// ���?����. 
		//------------------------------------
		static DWORD dwLastItemStatisticClock = 0;
		if(GetFrame() > 100 && TABS(CurrentClock - dwLastItemStatisticClock) > 1000000)
		{
			dwLastItemStatisticClock = CurrentClock;
			pclItemManager->WriteStatisticAction(sTime);

			if(pclStatisticsManager)
			{
				pclStatisticsManager->WriteAction(sTime, pclQuestManager);
			}

		}

		if(pclAddMoneyStatistics)	pclAddMoneyStatistics->Action();
		if(pclDelMoneyStatistics)	pclDelMoneyStatistics->Action();

		WRITE_MEMLOG( 120, 26, 0, 0 );

		// ���� �輱��(���ڻ��? ����ϱ�?
		// �����ϸ� �ѹ� ������ش�?
		static bool bstart = true ;
		if ( bstart && GetFrame() > 200 )
		{
			pclItemManager->CalcInvestment(bstart);
			bstart = false ;
		}
		if ( sTime.wMinute == 0 )
		{
			if ( bCalcInvestment == true )
				pclItemManager->CalcInvestment(bCalcInvestment);

			bCalcInvestment = false ;
		}
		else
		{
			bCalcInvestment = true ;
		}


		// ���� 
		if((GetFrame() % 1200) == 0)
		{
			pclFatherManager->Action(pclCM);
		}

		WRITE_MEMLOG( 120, 27, 0, 0 );

		static DWORD dwAutoHunting = 0;
		if(TABS( CurrentClock - dwAutoHunting ) > 1000 )
		{
			if ( pclAutoHuntingManager )
			{
				dwAutoHunting  = CurrentClock;
				pclAutoHuntingManager->Action();
			}
		}

		if ( pclMinisterMgr )
		{
			CMinisterGoonzu *pclministerGoonzu = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
			if ( pclministerGoonzu )
			{
				pclministerGoonzu->DoMsgDBMsgRequest_SetGoonzuFinances(CurrentClock) ;
				pclministerGoonzu->ActionGameServerInfo(CurrentClock) ;
			}
		}

		WRITE_MEMLOG( 120, 28, 0, 0 );

		static DWORD dwAngelCounting = 0;
		if(TABS( CurrentClock - dwAngelCounting ) > 300000 )
		{
			CMinisterHozo *pclministerHozo = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
			if ( pclministerHozo && pclStatisticsManager )
			{
				dwAngelCounting  = CurrentClock;
				pclStatisticsManager->clDailyStatistics.siAngelNumber = pclministerHozo->GetAngelNumber() ;
			}
		}

		static DWORD dwMonsterGroup = 0;
		if(TABS( CurrentClock - dwMonsterGroup ) > 10000 ) // 10�ʸ��� �ѹ���
		{
			if ( bCompleteDBInitAll )
			{
				if ( pclMonsterGroupManager )
				{
					pclMonsterGroupManager->Action();
					dwMonsterGroup  = CurrentClock;
				}
			}
		}

		if( m_bToSendServerInfoToFront ) {
			m_bToSendServerInfoToFront = FALSE;
			SendServerInfoToFront();
		}

		WRITE_MEMLOG( 120, 29, 0, 0 );

		// �ӽ÷� ����Ѵ�? 
		PopCash();

		// ������ ������.
		//static DWORD dwSpecialNews = 0;	���� ����
		if(TABS( CurrentClock - dwSpecialNews ) > 60 * 1000 ) // 1�и��� �ѹ���
		{
			PopSpecialNews();
			dwSpecialNews = CurrentClock;
		}

		// ���� ���� ����
		if( GetFrame() % (20 * 60 * 5) == 0 && bCompleteDBInitAll ) {
			RefreshHouseUnit();
		}

		if( sTime.wHour == 0 && sTime.wMinute == 0 && bZeroStatistics == FALSE )
		{
			ZeroMemory( &clCurrencyInfo, sizeof( clCurrencyInfo ) );
			bZeroStatistics = TRUE;
		}

		if( sTime.wMinute % 30 == 0 && bWriteStatistics == FALSE )
		{
			sDBRequest_WriteStatistics sendmsg(
				&clCurrencyInfo,
				pclGameProtocol->GetSessionManager()->GetActiveSessionCount(),
				pclCM->pclIDManager->GetNPCCharNumber(),
				pclItemManager->siItemNum,
				pclMarketManager->pclOrderManager->GetOrderNum(ORDERMODE_SELL)
				);

			sendmsg.year = sTime.wYear;
			sendmsg.month = sTime.wMonth;
			sendmsg.day = sTime.wDay;
			sendmsg.hour = sTime.wHour;
			sendmsg.minute = sTime.wMinute;

			pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&sendmsg );

			bWriteStatistics = TRUE;


		} else if( sTime.wMinute == 5 || sTime.wMinute == 35 )
		{
			bWriteStatistics = FALSE;
			bZeroStatistics = FALSE;
		}

		WRITE_MEMLOG( 120, 30, 0, 0 );

		// ���� �ڵ� �Ļ� �������̶��?
		if( bCompleteDBInitAll )
		{
			if( bBankruptcy )
			{
				// ���� �ڵ� �Ļ� �ð��� ������ ���¶��?
				if( siStartBankruptDateVary > 1 )
				{
					cltDate tempDate;
					tempDate.MakeFullDateFromDateVary( siStartBankruptDateVary );
					tempDate.AddMonth(3);

					SI32 currentDateVary = pclTime->clDate.GetDateVary();
					if( currentDateVary > tempDate.GetDateVary() )
					{
						SI32 findVillageUnique[5] = { 0, 0, 0, 0, 0};

						pclVillageManager->FindLowTotalVillageScore( findVillageUnique );

						CMinisterHozo *pclministerHozo = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
						if ( pclministerHozo != NULL ) 
						{
							SI32 i = 0;
							for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
							{
								cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[i];
								if( pclCityHall == NULL ) 
									continue;

								pclCityHall->bPrepareBankruptcy = FALSE;

							}

							for( i = 0; i < pclministerHozo->m_siBankruptcyVillageNumber; ++i )
							{
								if( 0 == findVillageUnique[i] )
									continue;

								sDBRequest_CityHall_State_Changed clMsg( findVillageUnique[i], CITYHALL_BEING_NORMAL, CITYHALL_BEING_BANKRUPT );
								pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
							}


							sDBRequest_VillageBankruptcy clMsg2( 0, 0, sDBRequest_VillageBankruptcy::BANKRUPTY );
							pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg2);

							siStartBankruptDateVary = 0;
						}
					}
				}
			}
		}

		WRITE_MEMLOG( 120, 31, 0, 0 );

		//KHY - 0530 �����?���� ��������. - 1�п� �ѹ� �˻�. - ���űݾ��� ����ϰ�? �ٽ� ���� �����ش�.
		//if ( (pclTime->IsDayChanged()) || (GetFrame() % 1200 == 0) )
		{
			if(CanChargingBuyLimit() == true)
			{
				// ���űݾ� �α� ����. - [���ַ�][���űݾ�][�������ѱݾ�]
				pclLog->FilePrint(TEXT("Config\\DailyNPCBuyItemSum.log"), TEXT("[Daily:%d/%d/%d][TotalMoney:%I64d][Limit:%I64d] ")
					,pclTime->clDate.uiYear ,pclTime->clDate.uiMonth ,pclTime->clDate.uiDay
					,gForeignMerchantNowMoney
					,gForeignMerchantBuyLimit);

				// �������� ���� ���� ���� �ݾ��� ���� or ���� ��Ų��.
				if (  gForeignMerchantBuyLimit <= gForeignMerchantNowMoney ) 
				{
					if (fForeignMerchantPercentUP > 0 ) 
					{
						float fTemp = ((float)gForeignMerchantBuyLimit * fForeignMerchantPercentUP)/100	;
						gForeignMerchantBuyLimit = gForeignMerchantBuyLimit + (GMONEY)fTemp ;
					}
				}
				else{
					if (fForeignMerchantPercentDOWN > 0 ) 
					{
						float fTemp = ((float)gForeignMerchantBuyLimit* fForeignMerchantPercentDOWN)/100	;
						gForeignMerchantBuyLimit =gForeignMerchantBuyLimit- (GMONEY)fTemp ;
					}
				}

				//���űݾ����� ����.
				gForeignMerchantNowMoney = 0;

				// �ʱ�ȭ�� ���� ���űݾ��� DB��~
				sDBRequest_SetForeignMerchantNowMoney clMsgForeignMerchantNowMoney(gForeignMerchantNowMoney);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgForeignMerchantNowMoney);

				// �������� ��ȭ�� ��ȭ�� �����?���ΰ��� ���� ���� ���?�����Ѵ�
				sDBRequest_SetForeignMerchantBuyLimit clMsgForeignMerchantBuyLimit( gForeignMerchantBuyLimit, fForeignMerchantPercentUP,fForeignMerchantPercentDOWN );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgForeignMerchantBuyLimit);

			}
			if(CanChargingSystemBuyItemLimit() == true)
			{
				// KHY - 0901 -  ���� ������ �ڵ����� ����.
				// ���űݾ� �α� ����. - [���ַ�][���űݾ�][�������ѱݾ�]
				// ���� ������ �ڵ����� �ݾ��� �Ѿ��� ��ä ����ȭ���� 0.1%�� ���� ���Ѵ�.
				SI64 siLimitMoney = (SI64)((REAL32)siTotalMoney * 0.001f);
				pclLog->FilePrint(TEXT("Config\\DailySystemBuyItemSum.log"), TEXT("[Daily:%d/%d/%d][TotalMoney:%I64d][Limit:%I64d] ")
					,pclTime->clDate.uiYear ,pclTime->clDate.uiMonth ,pclTime->clDate.uiDay
					,siMarketMoneyItemMaxpriceNow
					,siLimitMoney);

				siMarketMoneyItemMaxpriceNow = 0;

				sDBRequest_SetSystemBuyItemNowMoney clMsg(siMarketMoneyItemMaxpriceNow);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}				
		}

		if ( pclCheckProcessManager )
		{
			pclCheckProcessManager->Action();
		}

		// USE_PROMOTOR_SERVER
		if ( pclPromoterManager )
		{
			pclPromoterManager->Action();
		}

		if (m_kUpdateTimer_PVP_League.IsTimed(GetTickCount(), true))
		{
			if ( pclPVP_LeagueMgr )
			{
				pclPVP_LeagueMgr->Action()	;// �ϴ��� �׼� ȣ�� 
			}
		}	
		if ( m_kUpdateTimer_GuildHuntMapWar.IsTimed(GetTickCount(), true))
		{
			if ( pclGuildHuntMapManager )
			{
				pclGuildHuntMapManager->Action()	;
			}
		}
		// �������ϱ� �̺�Ʈ �׼� ȣ�� //Switch_10MonthEvent_RacoonDodge
		if(pclClient->IsCountrySwitch(Switch_10MonthEvent_RacoonDodge))
		{
			if(pclClient->pclEventTimeManager->InEventTime("RaccoonDodge", &sTime) )
			{
				if ( m_kUpdateTimer_RacoonDodgeEvent.IsTimed(GetTickCount(), true))
				{
					if ( pclRacoonDodgeEventMgr )
					{
						pclRacoonDodgeEventMgr->Action()	;
					}
				}
			}
		}
		//--------------------------------------------------------------------------------------
		//		����ã�� �̺�Ʈ
		//		���� ó�� ���� ������ �ѹ��� �����Ѵ�
		//--------------------------------------------------------------------------------------
		if( pclClient->IsCountrySwitch(Switch_TreasureEvent) )
		{
			if(pclClient->pclEventTimeManager->InEventTime("TreasureEvent", &sTime) )
			{
				if ( bReadTreasureEvent_Resource == false)
				{
					// �����͸� �о� �´�.
					Read_TreasureEvent_MapInfo_TheQ();
					bReadTreasureEvent_Resource	= true	;
				}

				if ( bMakeTreasureEventNPC == false)
				{
					SI32 siMonsterKind	=	pclClient->GetUniqueFromHash(TEXT("KIND_ANTIQUITY"))	;// ��ȯ�� �༮.
					for ( SI32 Slot_Mapindex = 0 ; Slot_Mapindex < MAX_TREASUREEVENT_USEMAP_NUM; Slot_Mapindex++)
					{
						if( Slot_Mapindex < 0  || Slot_Mapindex > MAX_TREASUREEVENT_USEMAP_NUM )	break;

						SI32 siMapIndex = pclTreasureMapInfo[Slot_Mapindex].GetMapIndex()	; //���� ���ε����� �̾� �´�.
						if( siMapIndex <= 0)			continue	;

						SI32 siMax_MakeNpcNum	=	pclTreasureMapInfo[Slot_Mapindex].Get_MaxUse_RocNum(); // �ش�ʿ�?�����?�ִ� �ִ��� npc ����
						if( siMax_MakeNpcNum <= 0)		continue	;

						SI32 siHave_Roc_This_Map	=	pclTreasureMapInfo[Slot_Mapindex ].GetRocNum( )	;	//�ش� ���� ���� ��ǥ�� ��
						if( siHave_Roc_This_Map <= 0 )	continue	;	// ��ǥ�� ������ ����
						if( siHave_Roc_This_Map < siMax_MakeNpcNum )		break	;	// ���� ��ǥ�� ���� ��ǥ���� ũ�� ����

						SI32 siUse_MakeNpcSlotNumber = MakeMapRocSlot( Slot_Mapindex , siMax_MakeNpcNum )	;
						if (siUse_MakeNpcSlotNumber <= 0 )	continue	;
						if ( siUse_MakeNpcSlotNumber >= MAX_TREASUREEVENT_POSITION_NUM )
						{
							siUse_MakeNpcSlotNumber = MAX_TREASUREEVENT_POSITION_NUM	;
						}

						for ( SI32 Slot_roc = 0; Slot_roc < siUse_MakeNpcSlotNumber ; Slot_roc++)
						{
							if( Slot_roc < 0  || Slot_roc > MAX_TREASUREEVENT_POSITION_NUM )	break;
							if( Slot_Mapindex < 0  || Slot_Mapindex > MAX_TREASUREEVENT_USEMAP_NUM )	break;

							if(pclTreasureMapUseRoc[Slot_roc]	== NULL)	break	;
							SI32 siRocX = pclTreasureMapInfo[Slot_Mapindex].GetRoc_X( pclTreasureMapUseRoc[ Slot_roc] )	;
							SI32 siRocY = pclTreasureMapInfo[Slot_Mapindex].GetRoc_Y( pclTreasureMapUseRoc[ Slot_roc ] )	;
							cltMapCommon* pclmap	= pclMapManager->GetMapPointer( siMapIndex );
							// �ش� ��ǥ���� ����ġ�� ��´�?

							SI32 siX, siY	;	//��������ڸ�?

							pclmap->FindEmptyAreaForCharLocate( siMonsterKind , siRocX, siRocY, 10, &siX, &siY, false )	;

							SI32 id = SetNPCChar( siMonsterKind, VILLAGE_NONE, NULL, 1, 1000, siX, siY, siMapIndex, NPCITEMMODE_NORMAL, NULL );
						}
					}			
					bMakeTreasureEventNPC	=	true	;
				}
			}

		}
		if( pclClient->IsCountrySwitch(Switch_TreasureEvent) )
		{
			if(pclClient->pclEventTimeManager->InEventTime("TreasureEvent", &sTime) )
			{
				if(m_kUpdateTimer_TreasureEvent_Respon_Npc.IsTimed(GetTickCount(),true))	// ����ã�� �̺�Ʈ
				{
					// ������ 
					Respon_TreasureEvent_Npc()	;
				}

			}
		}
		if( pclClient->IsCountrySwitch(Switch_GACHA_RAREITEM_CHANGE) )
		{
			if(bGachaRareitemStartSet == false)
			{
				SetGachaRareItem_Time()	; // �Ⱓ�� ���� ��í �ְ� ��ǰ�� ���� �Ѵ� .
				bGachaRareitemStartSet	=	true	;
			}
			if(m_kUpdateTimer_GachaRareItmeUnique_Timer.IsTimed(GetTickCount(),true))// ��í ���� ������ ���� 
			{
				SetGachaRareItem_Time()	; // �Ⱓ�� ���� ��í �ְ� ��ǰ�� ���� �Ѵ� .
			}

		}

		//--------------------------------------------------------------------------------------
		//	�޸� ���� �ý��� - ���� �ð� ���� ��Ģ������ ����
		//--------------------------------------------------------------------------------------
		// �޸� ���� �ý����� �������� �����Ѵ�.
		if( NULL != pclDormancySystem)
		{
			CDormancyPointRewardServer *pclDormancyPointReward = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
			cltCharServer *pclCharServer = NULL;
			if( NULL != pclDormancyPointReward)
			{
				// ���� �ð��� ���ڷ� �ְ� ó�� ���� �����Ѵ�.
				if(pclDormancyPointReward->DeterminProcessing(CurrentClock))
				{
					_SYSTEMTIME st;
					GetLocalTime(&st);

					for (SI32 id = 0; id < MAX_PERSON_NUMBER; id++)
					{
						if( pclCM->IsValidID(id) )
						{
							pclCharServer = pclCM->GetCharServer(id);
							if( NULL == pclCharServer)
								continue;
							// ���ӵ� ĳ���͸� ����Ѵ�?
							if( pclCharServer->IsValidConnection())
							{
								pclDormancyPointReward->CalcPoint(pclCharServer);
								if( TRUE == pclDormancyPointReward->IsExpiration(pclCharServer, st))
									pclDormancyPointReward->SendDBChangeDormancyState(pclCharServer, FALSE);
							}
						}
					}
				}
			}
		}
		//--------------------------------------------------------------------------------------
		//	��Ʋ�ξ�
		//--------------------------------------------------------------------------------------
		if( NULL != m_pclBattleRoyalMgr)
		{
			m_pclBattleRoyalMgr->Action();
		}
		//--------------------------------------------------------------------------------------
		//	�߷�Ÿ�� ���� �̺�Ʈ
		//--------------------------------------------------------------------------------------
		if( NULL != m_pclValentineEventMgr)
		{
			((CValentineEventServerMgr*)m_pclValentineEventMgr)->Action();
		}
		
		if( m_pclGiantRaccoonResurrectEvent )
		{
			m_pclGiantRaccoonResurrectEvent->Action();
		}


		// [����] �߷�Ÿ�� ���� NPC ���� ������ ����.
		ValentineDayNpc();


		// ��ũ��
		m_pclGZFtpMgr->Poll();


		//----------------------------------------------------------------
		// ������ ������ ��
		//----------------------------------------------------------------
#endif //_SERVER	`
		//----------------------------------------------------------------

		return TRUE;
}



// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
void cltServer::SendServerResponseMsg(SI16 receiver, UI16 returnval, SI16 para1, SI16 para2, SI16 charunique)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ����ڰ�?�ƴϸ� �޽����� ���� �� ����. 
	if(IsPC(charunique) == false)return ;

	cltServerResponseValue clreturn(receiver, returnval, para1, para2);

	// Ŭ���̾�Ʈ�� ������ ������. 
	cltMsg clMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);

	SI32 id = cltCharServer::CharUniqueInfo[charunique];
	if(id)
	{
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

//KHY - 0910 - Ŭ���̾�Ʈ�� ĳ���������� ���ϰ��� ������.
void cltServer::SendServerResponseCharMsg(SI16 receiver, UI16 returnval, TCHAR *para1, TCHAR *para2, SI16 charunique)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ����ڰ�?�ƴϸ� �޽����� ���� �� ����. 
	if(IsPC(charunique) == false)return ;

	cltServerResponseCharValue clreturn(receiver, returnval, para1, para2);

	// Ŭ���̾�Ʈ�� ������ ������. 
	cltMsg clMsg(GAMEMSG_RESPONSE_CHARSERVERRTN, sizeof(cltServerResponseCharValue), (BYTE*)&clreturn);

	SI32 id = cltCharServer::CharUniqueInfo[charunique];
	if(id)
	{
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

//���� Ŭ���̾�Ʈ�� ĳ���������� ���ϰ��� ������.2->3��
void cltServer::SendServerResponseCharMsg2(SI16 receiver, UI16 returnval, TCHAR *para1, TCHAR *para2, TCHAR *para3 , SI16 charunique)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ����ڰ�?�ƴϸ� �޽����� ���� �� ����. 
	if(IsPC(charunique) == false)return ;

	cltServerResponseCharValue2 clreturn(receiver, returnval, para1, para2, para3);

	// Ŭ���̾�Ʈ�� ������ ������. 
	cltMsg clMsg(GAMEMSG_RESPONSE_CHARSERVERRTN, sizeof(cltServerResponseCharValue2), (BYTE*)&clreturn);

	SI32 id = cltCharServer::CharUniqueInfo[charunique];
	if(id)
	{
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void  cltServer::DrawServerInfo(HDC hdc)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	sGameServerRefreshInfo refreshinfo;

	SI32 yindent	= 20;

	TCHAR buffer[256];
	TCHAR temp[256];
	TCHAR small1[125];
	TCHAR small2[125];

	m_currentFrame = GetFrame();

	SI32 y = 25;

	TCHAR sztime[128];


	// DBLatency
	if(pclGameDBProcess)
	{

		for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
			StringCchCopy( buffer, 256, TEXT("������������������������") );
			TextOut( hdc, 600, 25 + yindent * i, buffer, lstrlen(buffer));
			StringCchPrintf( buffer, 256, TEXT("DBLatency[%2d]:%d(ms)"), i, pclGameDBProcess->m_dwDBLatency[ i ] );
			TextOut( hdc, 600, 25 + yindent * i, buffer, lstrlen(buffer));
		}
	}



	// ���� 

	StringCchPrintf( buffer, 256, TEXT("Area:%d ServerUnique:%d Version: %d"), siServiceArea, siServerUnique, siVersion);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ���� �⺻ ���� 
	StringCchPrintf(buffer, 256, TEXT("Mall:%d MarketAutoBuyDelay:%d"), bItemMallUseSwitch, siMarketAutoBuyDelayFrame);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ���� �����ð�. 
	DWORD second = TABS(CurrentClock - StartClock) / 1000;
	SI32  day = second / (60*60*24);
	second -= (day * 60*60*24);

	SI32 hour = second  / (60*60);
	second -= (hour*60*60);

	SI32 minute = second / 60;
	second -= (minute*60);

	pclTime->clDate.GetDateText(sztime);
	TCHAR* pText = GetTxtFromMgr(3863);
	StringCchPrintf( buffer, 256, pText, sztime, day, hour, minute, second);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// FPS	
	refreshinfo.uiFrame = m_currentFrame;

	// Frame, FPS ��  DB���� �ð�
	refreshinfo.usFPS = m_currentFrame - m_lastFrame;

	StringCchPrintf( small1, 125, TEXT("Frame : %d(FPS : %4d) "), m_currentFrame, m_currentFrame - m_lastFrame );

	// DB���� �ð�. 
	if( pclGameDBProcess ) {
		refreshinfo.usDBLatency = (UI16)( pclGameDBProcess->m_dwCurrentDBLatency / 1000 );
	} else {
		refreshinfo.usDBLatency = 0;
	}

	pText = GetTxtFromMgr(3864);
	StringCchPrintf( small2, 125, pText, dwDBLatency / 1000 );

	StringCchPrintf( buffer, 256, TEXT("%s %s"), small1, small2);

	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	//sprintf( buffer, TEXT("Game Mode : Window %d  Mixing %d  FullScreen %d "), 
	//	siWndInterfaceNum, siWnd_NewInterfaceNum, siFullWnd_NewInterfaceNum );

	// 2005.5.12 ���� ���?2�� �̹Ƿ� ȥ�� ���?FullScreen ���� ǥ����
	StringCchPrintf( buffer, 256, TEXT("Game Mode : Window %d  FullScreen %d "), 
		siWndInterfaceNum, siWnd_NewInterfaceNum );

	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	//// DB���� �ð�. 
	//if( pclGameDBProcess ) {
	//	refreshinfo.usDBLatency = (UI16)( pclGameDBProcess->m_dwCurrentDBLatency / 1000 );
	//} else {
	//	refreshinfo.usDBLatency = 0;
	//}
	//pText = GetTxtFromMgr(3864);
	//sprintf( buffer,pText, dwDBLatency / 1000 );
	//TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	//y += yindent;

	// ���� ��. 
	siCurUserNum = pclGameProtocol->GetSessionManager()->GetActiveSessionCount();
	if(siTodayMaxUserNum < siCurUserNum)
	{
		siTodayMaxUserNum = siCurUserNum;
	}

	refreshinfo.usCurrentUserNum = siCurUserNum;
	//KHY - 0708 - �Ϻ� ����͸�?Ŭ���̾�Ʈ ����.
	//refreshinfo.usCurrentNetMarbleJP = siNetmarbleCharNum;
	refreshinfo.usCurrentHanGameJP	= siHangameCharNum;
	refreshinfo.usCurrentGamaniaJP = siGamaniaCharNum;
	refreshinfo.usCurrentGamePotJP = siGamePotCharNum;
	refreshinfo.usCurrentNDEJP = siNDECharNum;
	refreshinfo.usCurrentNEXONJP = siNEXONCharNum;

	//[����] USA �̰�
	refreshinfo.usCurrentGLOBAL		= siGlobalNum;
	refreshinfo.usCurrentNHN		= siNHNNum; 


	refreshinfo.usMaxUserNum = siTodayMaxUserNum;
	refreshinfo.uiNewCharNum = siNewCharNum;
	refreshinfo.uiDelCharNum = siDelCharNum;
	//refreshinfo.uiPCRoomCharNum = siPCRoomCharNum;
	if ( pclNTradeServerProcess )
	{
		ITZSession* pclTradeSession = pclNTradeServerProcess->GetTradeServerServersSession() ;
		if ( pclTradeSession )
		{
			refreshinfo.uiTradeServerSession = pclTradeSession->GetState() ;
		}
		else
		{
			refreshinfo.uiTradeServerSession = 0 ;
		}
	}
	else
	{
		refreshinfo.uiTradeServerSession = 0 ;
	}

	if ( IsWhereServiceArea(ConstServiceArea_Japan) )
	{
		StringCchPrintf( buffer, 256, TEXT("User/MaxUser:%04d[%04d]/%04d  Hangame:%04d Gamania:%04d	GamePot:%04d NDE:%04d NEXON:%04d Create:%05d Del:%05d PCRoom: %5d"), 
			siCurUserNum, siTodayMaxUserNum, MAX_PERSON_NUMBER, siHangameCharNum ,siGamaniaCharNum,  siGamePotCharNum, siNDECharNum,siNEXONCharNum, siNewCharNum, siDelCharNum, siPCRoomCharNum);
	}
	else
	{
		StringCchPrintf( buffer, 256, TEXT("User/MaxUser:%04d[%04d]/%04d  Create:%05d  Del:%05d  PCRoom:%05d"), 
			siCurUserNum, siTodayMaxUserNum, MAX_PERSON_NUMBER, siNewCharNum, siDelCharNum, siPCRoomCharNum);
	}
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	StringCchPrintf( buffer, 256, TEXT("1~19(%d)20~39(%d)40~59(%d)60~79(%d)80~99(%d)100~119(%d)120~(%d)"), 
		si1To19UserNum,
		si20To39UserNum,
		si40To59UserNum,
		si60To79UserNum,
		si80To99UserNum,
		si100To119UserNum,
		si120To150UserNum);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// NPC��. 
	refreshinfo.usCurrentNPCNum = pclCM->pclIDManager->GetNPCCharNumber();

	StringCchPrintf( buffer, 256, TEXT("NPC/MaxNPC: %4d/%4d ActiveNPC:%4d Summon:%4d InstanceNPC:%4d/%4d"),
		pclCM->pclIDManager->GetNPCCharNumber(), MAX_NPC_NUMBER, 
		pclCM->pclIDManager->GetActiveNPCNumber(),
		pclStatisticsManager->clDailyStatistics.siSummonNumber ,
		pclCM->pclIDManager->GetInstanceNPCNumber(),MAX_INSTANCE_NPC_NUMBER);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// �������� 
	pText = GetTxtFromMgr(3865);
	StringCchPrintf( buffer, 256, pText, 
		pclItemManager->clItemStatistic.siItemPriceIndex,
		pclItemManager->clItemStatistic.siNPCPriceRate);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ��ȭ�� 
	TCHAR szMoney[ 5 ][ 32 ];

	// NPC, Quest, War
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clInputMoney_NPC.itMoney, szMoney[ 0 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clInputMoney_Quest.itMoney, szMoney[ 1 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clInputMoney_War.itMoney, szMoney[ 2 ], 32, TEXT(""));

	StringCchPrintf(buffer, 256, TEXT("Input( NPC:%s  QuestReward:%s  War:%s )"), 
		szMoney[ 0 ], szMoney[ 1 ], szMoney[ 2 ] );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// Father, ��õ��
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clInputMoney_Father.itMoney, szMoney[ 0 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clInputMoney_Recommender.itMoney, szMoney[ 1 ], 32, TEXT("") );

	pText = GetTxtFromMgr(3866);
	StringCchPrintf(buffer, 256, pText, 
		szMoney[ 0 ], szMoney[ 1 ] );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// NPC, Dead, Repair
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_NPC.itMoney, szMoney[ 0 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_Dead.itMoney, szMoney[ 1 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_Repair.itMoney, szMoney[ 2 ], 32, TEXT("") );


	StringCchPrintf(buffer, 256, TEXT("Output( NPC:%s  Dead:%s  Repair:%s )"),
		szMoney[ 0 ], szMoney[ 1 ], szMoney[ 2 ] );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// �ֽ� ����, ���ּ�, ���ڼ�, ������

	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_Stock.itMoney, szMoney[ 0 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_clMarketTax.itMoney, szMoney[ 1 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_clStockBidTax.itMoney, szMoney[ 2 ], 32, TEXT("") );	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_TradeTax.itMoney, szMoney[ 3 ], 32, TEXT("") );


	pText = GetTxtFromMgr(3862);
	StringCchPrintf(buffer, 256, pText, 
		szMoney[ 0 ], szMoney[ 1 ], szMoney[ 2 ], szMoney[ 3 ] );

	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// ���� �ֹ�, ������, Quest Fee

	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_NPCPayment.itMoney, szMoney[ 0 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_clVillageWar.itMoney, szMoney[ 1 ], 32, TEXT("") );
	g_JYLibFunction.SetNumUnit( clCurrencyInfo.clOutputMoney_QuestFee.itMoney, szMoney[ 2 ], 32, TEXT("") );

	pText = GetTxtFromMgr(3861);
	StringCchPrintf(buffer, 256, pText, 
		szMoney[ 0 ], szMoney[ 1 ], szMoney[ 2 ], TEXT("") );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// Total In/Out Money

	cltMoney clTotalInput, clTotalOutput;
	clCurrencyInfo.GetTotalInput( &clTotalInput );
	clCurrencyInfo.GetTotalOutput( &clTotalOutput );

	g_JYLibFunction.SetNumUnit( clTotalInput.itMoney, szMoney[ 0 ], 32, TEXT("") );	
	g_JYLibFunction.SetNumUnit( clTotalOutput.itMoney, szMoney[ 1 ], 32, TEXT("") );	

	StringCchPrintf(buffer, 256, TEXT("Total Input:%s    Total Output:%s"), 
		szMoney[ 0 ], szMoney[ 1 ] );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ItemNum
	StringCchPrintf( buffer, 256, TEXT("ItemNum/Max: %4d/%4d"),pclItemManager->siItemNum, MAX_ITEMINFO_NUMBER);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ���� �ֹ� ����. 
	pText = GetTxtFromMgr(3858);
	StringCchPrintf(buffer, 256, pText, pclMarketManager->pclOrderManager->GetOrderNum(ORDERMODE_SELL), MAX_MARKET_ORDER_NUMBER);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// �����ֹ���
	pText = GetTxtFromMgr(3859);
	StringCchPrintf(buffer, 256, pText, 
		pclStockManager->pclOrderManager->GetOrderNum(ORDERMODE_SELL), 
		pclStockManager->pclOrderManager->GetOrderNum(ORDERMODE_BUY), 
		MAX_STOCK_ORDER_NUMBER);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ��Ƽ ����. 
	pText = GetTxtFromMgr(3860);
	StringCchPrintf(buffer,  256, pText, 
		pclPartyMgr->GetPartyNum(), MAX_PARTY_OBJ_NUM, pclPartyMgr->GetPartyTotalUserNum(), 
		pclStatisticsManager->clDailyStatistics.siHuntUserNumber, pclHuntManager->siHuntAddRate, pclStatisticsManager->clDailyStatistics.siDungeonUserNumber,
		pclStatisticsManager->clDailyStatistics.siInstanceDungeonUserNumber);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ������
	CTradeMerchantNPC *TradeMerchantNPC[3];
	TradeMerchantNPC[0] = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr(pclClient->GetUniqueFromHash( TEXT("KIND_CHINATRATER")));
	TradeMerchantNPC[1] = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr(pclClient->GetUniqueFromHash( TEXT("KIND_RUSSIATRATER")));
	TradeMerchantNPC[2] = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr(pclClient->GetUniqueFromHash( TEXT("KIND_JAPANTRATER")));	

	if ( TradeMerchantNPC[0]->m_siBuyItemUnique < 1 ||
		TradeMerchantNPC[1]->m_siBuyItemUnique < 1 ||
		TradeMerchantNPC[2]->m_siBuyItemUnique < 1
		)
	{
		TCHAR* pText = GetTxtFromMgr(3856);
		StringCchPrintf(buffer, 256, pText );
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(3857);
		StringCchPrintf(buffer, 256, pText, 
			pclItemManager->GetName( TradeMerchantNPC[0]->m_siBuyItemUnique ),
			(MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY-TradeMerchantNPC[0]->m_siBuyTotalMoney)/TradeMerchantNPC[0]->m_siBuyItemPrice,
			MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/TradeMerchantNPC[0]->m_siBuyItemPrice,
			pclItemManager->GetName( TradeMerchantNPC[1]->m_siBuyItemUnique ),
			(MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY-TradeMerchantNPC[1]->m_siBuyTotalMoney)/TradeMerchantNPC[1]->m_siBuyItemPrice,
			MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/TradeMerchantNPC[1]->m_siBuyItemPrice,
			pclItemManager->GetName( TradeMerchantNPC[2]->m_siBuyItemUnique ),
			(MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY-TradeMerchantNPC[2]->m_siBuyTotalMoney)/TradeMerchantNPC[2]->m_siBuyItemPrice,
			MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/TradeMerchantNPC[2]->m_siBuyItemPrice
			);
	}


	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;



	// ����. 
	StringCchPrintf(buffer, 256, TEXT("Weapon: Hnd:%d Swd:%d Spr:%d Stf:%d Axe:%d Bow:%d Gun:%d           "), 
		pclCM->siUserWeaponStatistic[0],
		pclCM->siUserWeaponStatistic[1],
		pclCM->siUserWeaponStatistic[2],
		pclCM->siUserWeaponStatistic[3],
		pclCM->siUserWeaponStatistic[4],
		pclCM->siUserWeaponStatistic[5],
		pclCM->siUserWeaponStatistic[6]);

	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	// ���� ������/ ������ ������ / ����.
	float quitrate = 0.0; 
	SI32 quitsum = pclStatisticsManager->clDailyStatistics.siNormalQuitUserNumber + pclStatisticsManager->clDailyStatistics.siAbnormalQuitUserNumber;
	if(quitsum)
	{
		quitrate = (float)pclStatisticsManager->clDailyStatistics.siAbnormalQuitUserNumber * 100.0f / (float)quitsum;
	}

	pText = GetTxtFromMgr(3855);
	StringCchPrintf(buffer, 256, pText,
		pclStatisticsManager->clDailyStatistics.siNormalQuitUserNumber, 
		pclStatisticsManager->clDailyStatistics.siAbnormalQuitUserNumber, quitrate, '%');

	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// FrontServer

	refreshinfo.usFrontServerConnection = 0;

	if(pclFrontAuthProcess && pclFrontAuthProcess->GetFrontSession())
	{
		switch(pclFrontAuthProcess->GetFrontSession()->GetState())
		{
		case SESSION_STATE_NONE:			StringCchCopy(temp, 256, TEXT("SESSION_STATE_NONE"));				break;
		case SESSION_STATE_ESTABLISHED:		
			{
				refreshinfo.usFrontServerConnection = 1;
				StringCchCopy(temp, 256, TEXT("SESSION_STATE_ESTABLISHED"));								
			}
			break;
		default:							StringCchCopy(temp, 256, TEXT("NOT DEFINED"));					break;

		}
	}
	else
	{
		StringCchCopy(temp, 256, TEXT("Not Allocated"));
	}
	StringCchPrintf( buffer, 256, TEXT("FrontServer: [%s]                "), temp);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;


	// Auth Server
	refreshinfo.usAuthServerConnection = 0;
	if(pclFrontAuthProcess && pclFrontAuthProcess->GetAuthSession())
	{
		switch(pclFrontAuthProcess->GetAuthSession()->GetState())
		{
		case SESSION_STATE_NONE:		StringCchCopy(temp, 256, TEXT("SESSION_STATE_NONE"));				break;
		case SESSION_STATE_ESTABLISHED:	
			{
				refreshinfo.usAuthServerConnection = 1;
				StringCchCopy(temp, 256, TEXT("SESSION_STATE_ESTABLISHED"));		
			}
			break;
		default:						StringCchCopy(temp, 256, TEXT("NOT DEFINED"));					break;
		}
	}
	else
	{
		StringCchCopy(temp, 256, TEXT("Not Allocated"));
	}
	StringCchPrintf( buffer, 256, TEXT("AuthServer: [%s]                "), temp);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	//DBManager
	refreshinfo.usDBManagerConnection = 0;
	if(pclGameDBProcess && pclGameDBProcess->IsEstablished() )
	{
		refreshinfo.usDBManagerConnection = 1;
		StringCchCopy(temp, 256, TEXT("SESSION_STATE_ESTABLISHED"));
	}
	else
	{
		StringCchCopy(temp, 256, TEXT("Not Allocated"));
	}
	StringCchPrintf( buffer, 256, TEXT("DBManager: [%s]                "), temp);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );
	y += yindent;

	//NTradeServer
	if(pclNTradeServerProcess && pclNTradeServerProcess->GetTradeServerServersSession() ) 
	{
		switch(pclNTradeServerProcess->GetTradeServerServersSession()->GetState() )
		{
		case SESSION_STATE_NONE:			StringCchCopy(temp, 256, TEXT("SESSION_STATE_NONE"));				break;
		case SESSION_STATE_ESTABLISHED:		
			{
				//				refreshinfo.usNSBSCashServerConnection = 1;
				StringCchCopy(temp, 256, TEXT("SESSION_STATE_ESTABLISHED"));								
			}
			break;
		default:							StringCchCopy(temp, 256, TEXT("NOT DEFINED"));					break;
		}
	}
	else
	{
		StringCchCopy(temp, 256, TEXT("Not Allocated"));
	}

	SI32 tradeServerReason = -1;

	if( pclNTradeServerProcess )
	{
		tradeServerReason = pclNTradeServerProcess->GetReason();
	}

	StringCchPrintf( buffer, 256, TEXT("TradeServer:[%s] Use:[%d] Reason:[%d]"), temp,bTradeServer, tradeServerReason );
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );

	y += yindent;


	// DB�ʱ�ȭ �Ϸ� ����

	if( bCompleteDBInitAll ) {
		TCHAR* pText = GetTxtFromMgr(3852);
		StringCchCopy(temp, 256, pText );
	} else {
		TCHAR* pText = GetTxtFromMgr(3853);
		StringCchCopy(temp, 256, pText );
	}

	pText = GetTxtFromMgr(3854);
	StringCchPrintf( buffer, 256, pText, temp);
	TextOut(hdc, 20, y, buffer, lstrlen(buffer) );

	m_lastFrame = m_currentFrame;

	refreshinfo.usStatus = SERVERSTATUS_OK;

	blockRefresh();
	*pRefreshInfo = refreshinfo;
	unblockRefresh();

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

// ����ĳ���͸� pclCM�� �����Ѵ�. 
void  cltServer::SetSessionChar()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 i;

	for(i = 1; i < MAX_PERSON_NUMBER;i++)
	{
		pclCM->CR[i] = (cltCharCommon*)pclGameProtocol->m_SF.GetPerson(i);
		if(pclCM->CR[i] == NULL)MsgBox(TEXT("fdsf98h2f"), TEXT("fds9:%d"), i);

		pclCM->CR[i]->SetpclCM(pclCM);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

// DB�� �ð��� �����Ѵ�. 
void cltServer::UpdateDBTime(cltDate* pcldate)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	sDBRequest_SetTime clMsg(CurrentClock, pcldate->uiYear, pcldate->uiMonth, pcldate->uiDay, pcldate->uiHour, pcldate->uiMinute);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}



//----------------------------
// NPC����ϱ�? 
//----------------------------
bool cltServer::Hire(SI32 kind, SI32 slot, SI32 villageunique, bool mode)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------
	// ������ ����
	if( kind < 0 || kind > MAX_KIND_NUMBER )		return false;
	if( slot < 0 || slot >= MAX_HIRED_NPC_NUM )		return false;
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )	return false;

	SI32 pos[8];

	//if(villageunique <= 0)return false;

	SI32 maptypeunique = pclMapManager->pclMapArray[villageunique]->siMapUnique;

	for(SI32 i = 0;i < 4;i++)
	{
		pos[i * 2]		= pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->StrLocation[6].x + (i / 2) * 4;
		pos[i * 2 + 1]	= pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->StrLocation[6].y + (i % 2) * 4;
	}



	if(mode == true)
	{
		if(slot >= 4)return false;

		//cyj ���NPC ��ġ ��ǥ ����
		//SI32 id = SetNPCChar(kind, villageunique, NULL, 1, 0, pos[slot*2], pos[slot*2 + 1], villageunique,/* NULL,*/ NPCITEMMODE_NONE, NULL );
		SI32 id = SetNPCChar(kind, villageunique, NULL, 1, 0, pos[slot*2] + 1 , pos[slot*2 + 1] + 1, villageunique,/* NULL,*/ NPCITEMMODE_NONE, NULL );
		if(id)
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			pclchar->bStaySwitch = true; // ���ڸ��� ���缭 �־��?�Ѵ�. 
		}

	}
	else
	{
		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( villageunique);
		if(pclinfo == NULL)return false;

		SI32 kind = pclinfo->clCityhallStrInfo.clHiredNPC[slot].siNPCKind;
		// �� ������ NPC�� kind�� �����Ѵ�. 
		if(kind)
		{
			SI32 index = 0;
			SI32 id;
			while(id = pclCM->GetCharOrder(index))
			{
				index++;

				// �������?�Ŀ��Ը� �����Ѵ�. 
				if(pclCM->CR[id]->GetKind() == kind)
				{
					if(pclCM->CR[id]->GetCurrentVillageUnique() == villageunique)
					{
						// �� �������� ������ �����Ѵ�. 
						pclCM->CR[id]->DeleteInCell();
						if(pclCM->DeleteChar(id) == TRUE)
						{
							index--;
						}
					}
				}
			}
		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return true;
}

void cltServer::SetWarResult( SI32 winVillage, SI32 loseVillage )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	//���� [������] �θ�-�ڽ� ���� �ý��� ����

	/*	pclVillageManager->SetWarResult( winVillage, loseVillage );

	cltStrInfo *pWinStrInfo, *pLoseStrInfo;

	pWinStrInfo = GetStrInfo( winVillage, RANKTYPE_CITYHALL );
	if(pWinStrInfo == NULL)return ;
	pLoseStrInfo = GetStrInfo( loseVillage, RANKTYPE_CITYHALL );
	if(pLoseStrInfo == NULL)return ;

	if( pWinStrInfo && pLoseStrInfo ) {

		pWinStrInfo->clCityhallStrInfo.siParentVillage = 
			pclVillageManager->pclVillageInfo[ winVillage ]->clVillageVary.siParentVillage;

		pWinStrInfo->clCityhallStrInfo.siMasterVillage = 
			pclVillageManager->pclVillageInfo[ winVillage ]->clVillageVary.siMasterVillage;

		pLoseStrInfo->clCityhallStrInfo.siParentVillage = 
			pclVillageManager->pclVillageInfo[ loseVillage ]->clVillageVary.siParentVillage;

		pLoseStrInfo->clCityhallStrInfo.siMasterVillage = 
			pclVillageManager->pclVillageInfo[ loseVillage ]->clVillageVary.siMasterVillage;

	}


	// DB���� 
	{

		sDBRequest_SetParentVillage clMsg( loseVillage, winVillage, 
			pclVillageManager->pclVillageInfo[ loseVillage ]->clVillageVary.siMasterVillage );

		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
	}

	{
		sDBRequest_SetParentVillage clMsg( winVillage, 
			pclVillageManager->pclVillageInfo[ winVillage ]->clVillageVary.siParentVillage,
			pclVillageManager->pclVillageInfo[ winVillage ]->clVillageVary.siMasterVillage );

		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
	}
	*/

		// ���� ������ ������ ���� ����
	{

		SI32 index	= 0;
		SI32 id		= 0;
		SI32 warExpPerScore = 0;
		FILE *fp  = NULL;

		TCHAR szFileName[256] = {'\0',};

		StringCchPrintf(szFileName,256,TEXT("Config\\VillageWar%04d%02d%02d.log"), sTime.wYear,sTime.wMonth, sTime.wDay );

		fp = fopen (szFileName ,"a+");

		if(fp != NULL)
		{
			fprintf(fp,"VillageUnique \t Name \t Score \t Exp \n");
		}


		while(id = pclCM->GetCharOrder(index))
		{
			index++;

			if(pclCM->IsValidID(id) && IsPC(pclCM->CR[id]->GetCharUnique()) )// �׾ ������ ���������?�ְ� ����
			{	
				cltCharServer * pclChar = (cltCharServer *)pclCM->CR[id];


				if( pclChar->pclCI->clBI.siHomeVillage == winVillage)
				{
					warExpPerScore = pclChar->siVillageWarKillScore * 500;
				}
				else if( pclChar->pclCI->clBI.siHomeVillage == loseVillage )
				{
					warExpPerScore = pclChar->siVillageWarKillScore * 300;
				}
				else
				{
					continue;
				}

				// �ּ� 1000, �ִ� �ڽ��� ���� * 1000 �� ����ġ�� ��´�?
				warExpPerScore =max( 1000 , min( warExpPerScore, pclChar->pclCI->clIP.GetLevel() * 1000 ) );

				pclChar->IncreaseExp(warExpPerScore, GETEXP_REASON_VILLAGEWARKILLSCORE);

				if(fp != NULL)
				{
					fprintf(fp,"%d\t%s\t%d\t%d\n",
						pclChar->pclCI->clBI.siHomeVillage, pclChar->GetName(), pclChar->siVillageWarKillScore , warExpPerScore);
				}
				warExpPerScore = 0;
			}
		}

		if(fp != NULL)
		{
			fclose(fp);
		}


	}


	NDate clCurDate;
	clCurDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);

	// �������� ���� ���� ( �й踶�� -> �ٸ����� )
	{
		SI32 targetvill = pclVillageManager->siDeclareWarVillage[ loseVillage ];

		if( targetvill != 0 ) {
			sDBRequest_CancelDeclareWar clMsg( 0, loseVillage, targetvill, &clCurDate  );
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
		}
	} 

	// �������� ���� ���� ( �ٸ������� -> �й踶�� )
	{
		for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

			if( pclVillageManager->siDeclareWarVillage[ i ] == loseVillage ) {

				sDBRequest_CancelDeclareWar clMsg( 0, i, loseVillage, &clCurDate );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	
			}
		}

	}

	// ���?Ŭ���̾�Ʈ���� �뺸 
	{
		cltGameMsgResponse_VillageWarResult sendMsg;
		sendMsg.siLoseVillage = loseVillage;
		sendMsg.siWinVillage = winVillage;

		cltMsg clMsg( GAMEMSG_RESPONSE_VILLAGEWARRESULT, sizeof( cltGameMsgResponse_VillageWarResult ), (BYTE*)&sendMsg );
		pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
	}

	SI32 WinnerRank =  pclVillageManager->GetVillageRank(winVillage);
	SI32 LoserRank  =  pclVillageManager->GetVillageRank(loseVillage);

	if( WinnerRank == 0 || LoserRank == 0 ) return;

	SI32 RankGap = WinnerRank - LoserRank;

	if( TABS(RankGap) >= MAX_VILLAGE_NUMBER ) return;

	SI32	WinnerGetVillageWarScore = 0;  //�¸������� ���?�Ǵ� ����
	GMONEY	WinnerGetVillageMoney	 = 0;  //�¸������� ���?�Ǵ� ��

	if( RankGap == 0 )
	{
		WinnerGetVillageWarScore = 1000;
		WinnerGetVillageMoney    = 10000000;		
	}
	else if( RankGap > 0)
	{
		SI32   BonusScore = 0;	
		GMONEY BonusMoney = 0;

		for( SI32 i = 0; i < RankGap; i++)
		{
			BonusScore  += (10    + i*20   );
			BonusMoney  += (10000 + i*20000);
		}

		WinnerGetVillageWarScore =     1000 + BonusScore;
		WinnerGetVillageMoney    = 10000000 + BonusMoney;
	}
	else if( RankGap < 0)
	{	

		WinnerGetVillageWarScore =     1000 + (RankGap * 20   );
		WinnerGetVillageMoney    = 10000000 + (RankGap * 60000);
	}

	/*
	// �¸� ���� ó��

	//��û �ں��� ����
	{
		sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEWAR, 0, winVillage, RANKTYPE_CITYHALL, WinnerGetVillageWarScore );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	}

	// �й� ���� ó��
	//��û �ں��� ����
	{
		sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEWAR, 0, loseVillage, RANKTYPE_CITYHALL, -WinnerGetVillageWarScore );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
	}*/


		//������ ��. ���?�޽��� DB��. ����Ʈ ����, �ں��� ���� �� �̷����?

		NDate WarEndDate;
	WarEndDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);

	sDBRequest_SetVillageWarResult sendMsg( winVillage, 
		loseVillage, 
		WinnerGetVillageWarScore, 
		WinnerGetVillageMoney, 
		&WarEndDate);

	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);	

	// Ŭ���̾�Ʈ��, ���ڰ� ��Ե�?������, �ں����� ����  
	/*    
	TCHAR szDest[256] = {'\0',};
	TCHAR * pText = GetTxtFromMgr(6479);
	StringCchPrintf(szDest,256,pText, pclVillageManager->GetName(winVillage),
		pclVillageManager->GetName(loseVillage) ,WinnerGetVillageWarScore, WinnerGetVillageMoney  );

	SendNoticeMsg( winVillage, 0 , 0 , szDest);
	SendNoticeMsg( loseVillage, 0 , 0 , szDest);
	*/
	{
		cltGameMsgResponse_VillageWarResultToVillageMember pclinfo( winVillage,
			loseVillage,
			WinnerGetVillageWarScore,
			WinnerGetVillageMoney);
		cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEWARRESULT_FOR_VILLAGEMEMBER,sizeof(pclinfo),(BYTE*)&pclinfo );        	
		pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg, winVillage);
		pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg, loseVillage);

	}

	// �й� ������ �г�Ƽ ����

	// ��Ȳ�� ���� -5
	// ������ ���� -5	
	pclVillageManager->AdjustCapaOfGodInVillage(loseVillage,-5);	
	//pclVillageManager->AdjustCapaOfCastleInVillage(loseVillage,-5);


	// ������ ���?�ǹ��� �������� 2000 ����. �ּ� 100�� ������
	pclVillageManager->AdjustAllStrDurInVillage(loseVillage,-2000,100);
	// �����?���� ����Ʈ ������ ���� �Ѵ�.
	//������ �� �������� �������� ���?�������� ���������� ����.
	sDBRequest_CancelDeclareWar clMsg( 0, winVillage, loseVillage, &WarEndDate );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);	

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

void cltServer::SetVillageWarLoss( SI32 winVillage, SI32 loseVillage )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	//����[������]
	//�й��?�ֽ� �Ұ� ����

	//	sDBRequest_SetVillageWarLoss sendMsg( loseVillage, winVillage, WARLOSS_STOCK, 10 );

	//	SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SetVillageLevel()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	GetSlaveVillageNum();

	cltStrInfo *pStrInfo;
	cltCityHall *pCityHall;

	SI32 villagelevel;

	SI32 villageupgradeunique[10] = {0,};
	SI32 villageupgradelevel[10] = {0,};

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		if( pclVillageManager->pclVillageInfo[ i ] == NULL ) continue;

		if( siSlaveVillageNum[ i ] == -1 ) continue;

		pStrInfo = GetStrInfo( i, RANKTYPE_CITYHALL );
		if( pStrInfo == NULL ) continue;


		// �������� �������� ���� + �ֹμ�����  + �湮�ڼ� ����
		pCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ i ];

		pCityHall->clLevelInfo.siSlaveVillage = siSlaveVillageNum[ i ];
		pCityHall->clLevelInfo.siResident = ( pStrInfo->clCityhallStrInfo.siResidentNum / 100 );

		SI32 templevel;
		if( pclVillageManager->GetVisitLevel( i, &templevel ) )
			pCityHall->clLevelInfo.siVisitorNum = templevel;

		villagelevel = pCityHall->clLevelInfo.GetVillageLevel();

		for ( SI32 j = 0 ; j < 10 ; j ++ )
		{
			if ( villageupgradelevel[j] < villagelevel )
			{
				for ( SI32 k = 9  ; k > j ; k -- )
				{
					villageupgradeunique[k] = villageupgradeunique[k-1] ;
					villageupgradelevel[k] = villageupgradelevel[k-1] ;
				}

				villageupgradeunique[j] = i ;
				villageupgradelevel[j] = villagelevel ;
				break ;
			}
		}

		if( villagelevel == pStrInfo->clCityhallStrInfo.siVillageLevel ) continue;

		sDBRequest_SetVillageLevel sendMsg( i, villagelevel,
			pCityHall->clLevelInfo.siSlaveVillage,
			pCityHall->clLevelInfo.siResident,
			pCityHall->clLevelInfo.siVisitorNum
			);
		SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );
	}

#ifdef _SAFE_MEMORY
	for(SI32 i=0; i<10; i++)		siTopLevelVillageUnique[0] = villageupgradeunique[0];
#else
	memcpy(siTopLevelVillageUnique,villageupgradeunique,sizeof(SI32) * 10 ) ;
#endif

	cltGameMsgResponse_SetVillageUpgrade sendMsg( &siTopLevelVillageUnique[0] );
	cltMsg clMsg( GAMEMSG_RESPONSE_SETVILLAGEUPGRADE, sizeof( sendMsg ), (BYTE*)&sendMsg );
	pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

void cltServer::GetSlaveVillageNum()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	int i=0, j=0;
	SI32 parent;

	for( i = 0; i < MAX_VILLAGE_NUMBER; ++i ) siSlaveVillageNum[ i ] = 0;

	for( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		if( pclVillageManager->pclVillageInfo[ i ] == NULL ) continue;

		parent = pclVillageManager->pclVillageInfo[ i ]->clVillageVary.siParentVillage;

		int c = 0;

		while( parent ) {

			siSlaveVillageNum[ parent ]++;

			parent = pclVillageManager->pclVillageInfo[ parent ]->clVillageVary.siParentVillage;

			if( ++c > 100 ) 
				if (pclClient->GameMode != GAMEMODE_SERVER)
					MessageBox( NULL, TEXT("errloop"), 0, 0 );
		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}



void cltServer::UpdateStrInfoToClient( SI32 id, SI32 strType )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// Person�� ��ġ�� ������ Unique�� ���Ѵ�. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// ������ villageunique �ٰŷ� �ǹ��� ������ ���Ѵ�. 
	cltStrInfo* pclinfo = GetStrInfo( villageunique, strType );
	if(pclinfo == NULL)return ;

	if( strType == RANKTYPE_CITYHALL ) {
		pclinfo->clCityhallStrInfo.siSymbolMaxLife = 
			pclVillageManager->GetMaxLifeOfGodInVillage( villageunique );

		pclinfo->clCityhallStrInfo.siCastleMaxLife =
			pclVillageManager->GetMaxLifeOfCastleInVillage( villageunique );
	}

	if(pclinfo)
	{
		cltMsg clNormalMsg(GAMEMSG_RESPONSE_STRINFO, 	pclinfo->GetTotalSize(), 	(BYTE*)pclinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

cltStrInfo* cltServer::GetStrInfo( SI32 villageUnique, SI32 strType )
{
	cltStrInfo* pclinfo = NULL;

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	switch( strType )
	{
	case RANKTYPE_BANK:
		pclinfo = pclBankManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_STOCK:
		pclinfo = pclStockManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_MARKET:
		pclinfo = pclMarketManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_HUNT:
		pclinfo = pclHuntManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_HOUSE:
		pclinfo = pclHouseManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_CITYHALL:
		pclinfo = pclCityHallManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_POSTOFFICE:
		pclinfo = pclPostOfficeManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_HORSEMARKET:
		pclinfo = pclHorseMarketManager->GetStrInfo(villageUnique);
		break;

	case RANKTYPE_REALESTATEMARKET:
		pclinfo = pclRealEstateMarketManager->GetStrInfo(villageUnique);
		break;

	case RANKTYPE_SUMMONMARKET:
		pclinfo = pclSummonMarketManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_LAND:
		pclinfo = pclLandManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_FEAST:
		pclinfo = pclFeastManager->GetStrInfo(villageUnique);
		break;

	case RANKTYPE_SUMMONHEROMARKET:
		pclinfo = pclSummonHeroMarketManager->GetStrInfo(villageUnique);
		break;

	case RANKTYPE_MINE:
		pclinfo = pclMineManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_GUILD:
		pclinfo = pclGuildManager->GetStrInfo(villageUnique);
		break;
	case RANKTYPE_NEWMARKET:
		pclinfo = pclNewMarketManager->GetStrInfo(villageUnique);
		break;

	default:
		//		MsgBox(TEXT("fdf83jfd"), TEXT("vill:%d strtype:%d"), villageUnique, strType);
		pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("GetStrInfo(%d,%d)"), villageUnique, strType );
		break;
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return pclinfo;
}

// �̺�Ʈ ����/���� 
void cltServer::SetEvent(SI32 index, bool bmode)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// �̹� ���� ���� �����Ǿ� �ִٸ� �ߺ��������� �ʴ´�. 
	if(bEventSwitch[index] == bmode)return ;

	bEventSwitch[index] = bmode;

	// ���?ĳ������ �����?�缳���Ѵ�. 
	pclCM->InitBlock();

	// �����?�̺�Ʈ ������ ���?����ڿ���?������. 
	cltGameMsgResponse_ServerEventInfo clinfo( &bEventSwitch[0] );
	cltMsg clMsg(GAMEMSG_RESPONSE_SERVEREVENTINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}


// ��ǰ�� ���۵Ǿ����� ��ü ����ڿ���?�˸���. 
// rareSwitch : � ��θ�?���� ���� ������ . �Ʒ��� ���� �����������?
// const SI32 RAREITEM_NOTICE_SWITCH_MADE = 1;
//const SI32 RAREITEM_NOTICE_SWITCH_WENNYOLDMAN = 2;
//const SI32 RAREITEM_NOTICE_SWITCH_GACHA = 3;

void cltServer::SendRareItemMade(SI32 id, cltItem* pclitem, SI32 rareSwitch)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(id <= 0)return ;
	if(id >= MAX_CHAR_NUMBER)return ;
	if(pclCM->CR[id] == NULL)return ;
	if(pclitem == NULL)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 fameexp = 0;


	if(rareSwitch == RAREITEM_NOTICE_SWITCH_MADE)
	{
		fameexp = 100;
	}
	else
	{
		fameexp = 0;
	}


	cltGameMsgResponse_RareItemMade	clinfo((TCHAR*)(pclchar->GetName()), pclchar->pclCI->GetPersonID(), 
		pclitem,
		rareSwitch,
		fameexp);	

	//KHY - 0725 - GM������ �ִ� ĳ���ʹ� ��ǰ������ ���ͼ��� �ȵȴ�.
	// �������?��� ���� �����?���Ѵ�. 
	SI32 gamemasterlevel = pclchar->GetGameMasterLevel();

	if(gamemasterlevel < BASIC_GAMEMASTER_LEVEL)
	{
		cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMMADE, sizeof(clinfo), (BYTE*)&clinfo);

		// �ѱ��� 18�� �̻� ��íâ ����ش�?
		if(IsWhereServiceArea(ConstServiceArea_Korea) == true)
			pclCM->SendAllAdultUserMsg((sPacketHeader*)&clMsg);
		else
			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
	}
	// ���?����ġ�� �ο��Ѵ�. 
	if( fameexp )
	{
		pclchar->IncreaseFameExp(fameexp); 
	}


	// ���Ե� ������ �ִٸ� DB�� ������ ������
	if( rareSwitch == RAREITEM_NOTICE_SWITCH_MADE && pclchar->pclCI->clBI.siHomeVillage > 0)
	{
		sDBRequest_VillageJoinMakeRareItem clMsg2(pclchar->pclCI->GetPersonID());
		SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);

		// LEEKH MODIFY 2007.10.25
		/*if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 9] // ��ǰ ����.
			// param1 == ��ǰ�� ȹ���� ���? param2 == ���?����ġ������.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_MAKE_RARE_ITEM, 
				0, pclchar, NULL, 0, 0, 0, pclitem, 0, 0, 0, 0, 0, 0, 
				rareSwitch, fameexp, 0, 0, 0, NULL, NULL);
		}*/
	}


	/*
	// �����?�����? 
	TCHAR itemname[128]=TEXT("");
	if(pclitem->GetFullName(pclItemManager, itemname, 128) == true)
	{
		pclLog->FilePrint(TEXT("config\\RareItemRecord.log"), TEXT("Person:[%s]	item:[%s]	stiwch[%d]"), pclchar->GetName(), itemname, rareSwitch);
	}
	*/

		//----------------------------------------------------------------
		// ������ ������ ��
		//----------------------------------------------------------------
#endif //_SERVER
		//----------------------------------------------------------------
}

// ������ ���� ������ Ŭ���̾�Ʈ���� ������. 
void cltServer::SendServerInfo(SI32 id)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	cltGameMsgResponse_ServerInfo clServerInfo(siServiceArea, siServerUnique, bItemMallUseSwitch, 
		bVillageWarSwitch,	bBonusTimeSwitch,  bBonus2TimeSwitch, bSpecialQuestBonusTimeSwitch, 
		bBroadcastSwitch,	siBestVillageLevel, 
		pclItemManager->clItemStatistic.siNPCPriceRate, pclItemManager->clItemStatistic.siItemPriceIndex, &bEventSwitch[0], siMinimumTeacherLevel,
		siServiceAge, siEnablePVP, bAutoUnblock, bVoteCampaign, &bEventToggleSwitch[0] );
	cltMsg clMsg(GAMEMSG_RESPONSE_SERVERINFO, 	sizeof(clServerInfo), (BYTE*)&clServerInfo);

	if(id)
	{
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else
	{
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}


// ���� ������ ������ ȹ���Ѵ�. 
void cltServer::GetServerInfo()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 i;
	//	SI32 sidata;
	//	SI32 sirate;
	FILE* fp;
	TCHAR buffer[1024];
	TCHAR temp[256];
	TCHAR temp2[256];
	TCHAR* pfilename = TEXT("ServerInfo\\ServerInfo.txt");

	fp = _tfopen(pfilename, TEXT("rt"));

	if(fp == NULL)
		return ;

	// ù��° 4���� �����Ѵ�. 
	for(i = 0;i < 4;i++)
		_fgetts(buffer, 1024, fp);

	//================================================================================
	// ServerInfo.txt �д� ���?���� - by LEEKH
	//================================================================================
	TCHAR text[128];

	while( _fgetts(buffer, 1024, fp) != NULL )
	{
		text[0] = 0;
		temp[0] = 0;
		temp2[0] = 0;

		// ServerInfo.txt uses "Key = Value" (all 65 data lines). The old format
		// "%s %s %s" captured the '=' sign as the value, so every setting below
		// silently parsed from "=" and the whole file was ignored.
		_stscanf(buffer, TEXT("%s = %s %s"), text, temp, temp2);

		if		(_tcscmp(text, "ServiceArea") == 0)					{	siServiceArea = GetServiceAreaFromCode(temp);						TraceBoot("ServerInfo: ServiceArea='%s' -> siServiceArea=%d (English=%d)", temp, siServiceArea, ConstServiceArea_English);	}
		// MasterServer�� ���� �̵� - by LEEKH 2007.10.10
		//else if	(_tcscmp(text, "ServerUnique") == 0)			{	siServerUnique = atoi(temp);										}
		else if	(_tcscmp(text, "ServerID") == 0)					{	MStrCpy(szServerID, temp, sizeof(szServerID));						}	
		else if	(_tcscmp(text, "ItemMall") == 0)					{	bItemMallUseSwitch	= DATA_TO_BOOL(atoi(temp));						}
		else if	(_tcscmp(text, "MarketAutoBuyDelay(Frame)") == 0)	{	siMarketAutoBuyDelayFrame	= min(10000, max(1000, atoi(temp)));	}
		else if	(_tcscmp(text, "EventRate") == 0)					{	siEventItemDropRate	= min(1000000, max(1000, atoi(temp)));			}
		else if	(_tcscmp(text, "MinimumTeacherLevel") == 0)			{	siMinimumTeacherLevel	= max(20, min(50, atoi(temp)));				}
		// pvp-
		else if	(_tcscmp(text, "LoseItem"			) == 0)		{	siLoseItem	= atoi(temp);						}
		else if	(_tcscmp(text, "RegenerateLifeValue") == 0)		{	siRegenerateLifeValue	= atoi(temp);			}
		else if	(_tcscmp(text, "BaekNyeonSamRate"	) == 0)		{	siBaenNyeonSamRate	= atoi(temp);				}
		else if	(_tcscmp(text, "HandDamageRate"		) == 0)		{	siHandDamageRate	= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "SwordDamageRate"	) == 0)		{	siSwordDamageRate	= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "SpearDamageRate"	) == 0)		{	siSpearDamageRate	= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "AxeDamageRate"		) == 0)		{	siAxeDamageRate		= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "BowDamageRate"		) == 0)		{	siBowDamageRate		= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "GunDamageRate"		) == 0)		{	siGunDamageRate		= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "StaffDamageRate"	) == 0)		{	siStaffDamageRate	= min( 200, atoi(temp) );	}
		else if	(_tcscmp(text, "PKMapExpRate"		) == 0)		{	siPKMapExpRate	= max( 0, atoi(temp) );			}
		else if (_tcscmp(text, "GuildWar"			) == 0)		{	bGuildWar = atoi(temp)?true:false;				}
		else if (_tcscmp(text, "Bankruptcy"			) == 0)		{	bBankruptcy = DATA_TO_BOOL(atoi(temp));			}
		else if (_tcscmp(text, "ItemmallRPRate"		) == 0)		{	siItemmallRPRate = atoi(temp);					}
		else if (_tcscmp(text, "AttendCheckTime"	) == 0)		{	siAttendCheckMinute = max(1, atoi(temp));		}
		else if (_tcscmp(text, "AttendEventDay"		) == 0)
		{
			SI32 startYear, startMonth, startDay;
			SI32 endYear, endMonth, endDay;

			startYear = startMonth = startDay = endYear = endMonth = endDay = 0;

			_stscanf(temp, TEXT("%d-%d-%d"), &startYear, &startMonth, &startDay );

			clAttendEventStart.m_uiYear = (UI16)startYear;
			clAttendEventStart.m_uiMonth = (UI08)startMonth;
			clAttendEventStart.m_uiDay = (UI08)startDay;

			_stscanf(temp2, TEXT("%d-%d-%d"), &endYear, &endMonth, &endDay );

			clAttendEventEnd.m_uiYear = (UI16)endYear;
			clAttendEventEnd.m_uiMonth = (UI08)endMonth;
			clAttendEventEnd.m_uiDay = (UI08)endDay;
		}
		else if(_tcscmp(text, "EnablePVP") == 0 )		{		siEnablePVP = atoi(temp);							}

		//KHY - 0115 �߷�Ÿ�ε��� �̺�Ʈ.
		else if (_tcscmp(text, "ValentineEventDay") == 0 )
		{
			SI32 startYear, startMonth, startDay;
			SI32 endYear, endMonth, endDay;

			startYear = startMonth = startDay = endYear = endMonth = endDay = 0;

			_stscanf(temp, TEXT("%d-%d-%d"), &startYear, &startMonth, &startDay );

			clValentineEventStart.m_uiYear = (UI16)startYear;
			clValentineEventStart.m_uiMonth = (UI08)startMonth;
			clValentineEventStart.m_uiDay = (UI08)startDay;

			_stscanf(temp2, TEXT("%d-%d-%d"), &endYear, &endMonth, &endDay );

			clValentineEventEnd.m_uiYear = (UI16)endYear;
			clValentineEventEnd.m_uiMonth = (UI08)endMonth;
			clValentineEventEnd.m_uiDay = (UI08)endDay;
		}
		else if (_tcscmp(text, "ValentineEventProbability") == 0 )  // ���� �����?���� Ȯ��
		{ 
			_stscanf(temp, TEXT("%d-%d-%d-%d-%d"), &siValentineFarm, &siValentineMine, &siValentineFish,&siValentineAttack,&siValentineItem ); 
		}
		//KHY - 0218 ȭ��Ʈ���� �̺�Ʈ.
		else if (_tcscmp(text, "WhiteEventDay") == 0 )
		{
			SI32 startYear, startMonth, startDay;
			SI32 endYear, endMonth, endDay;

			startYear = startMonth = startDay = endYear = endMonth = endDay = 0;

			_stscanf(temp, TEXT("%d-%d-%d"), &startYear, &startMonth, &startDay );

			clWhiteEventStart.m_uiYear = (UI16)startYear;
			clWhiteEventStart.m_uiMonth = (UI08)startMonth;
			clWhiteEventStart.m_uiDay = (UI08)startDay;

			_stscanf(temp2, TEXT("%d-%d-%d"), &endYear, &endMonth, &endDay );

			clWhiteEventEnd.m_uiYear = (UI16)endYear;
			clWhiteEventEnd.m_uiMonth = (UI08)endMonth;
			clWhiteEventEnd.m_uiDay = (UI08)endDay;
		}
		else if (_tcscmp(text, "WhiteEventProbability") == 0 )  // ���� �����?���� Ȯ��
		{ 
			_stscanf(temp, TEXT("%d-%d-%d-%d-%d"), &siWhiteFarm, &siWhiteMine, &siWhiteFish,&siWhiteAttack,&siWhiteItem ); 
		}
		else if(_tcscmp(text, "NoChargeGetItemFromNPC") == 0 )	{	bNoChargeGetItemFromNPC = DATA_TO_BOOL(atoi(temp));		}
		else if(_tcscmp(text, "NDEScrollRate") == 0 )			{	siNDEScrollRate = atoi(temp);							}				
		//KHY - 0530 �����?���� ��������.
		//else if(_tcscmp(text, "BuyLimitMoney") == 0 )	{	gForeignMerchantBuyLimit = atoi(temp);		}
		// KHY - 0901 -  ���� ������ �ڵ����� ����.
		else if(_tcscmp(text, "MoneyItemAutoBuyDelayTime(seceound)") == 0 )			
		{	
			SI32 tempTime = atoi(temp);

			if(tempTime > 0)
			{
				tempTime = tempTime * 20; // ���������� ��ȯ.	

				siMarketMoneyItemAutoBuyDelayTime = min(72000, max(1200, tempTime)); // �ּ� 1��~1�ð�	
			}
			else if(tempTime ==0)
				siMarketMoneyItemAutoBuyDelayTime = 0;
		}
		// ��Ŷ ���� �ּ�ũ�� ����
		else if(_tcscmp(text, "PacketCompMinSize") == 0 )			{	m_uiCompressMinSize = atoi(temp);						}				
	}


	GetServerNameFromServerUnique(siServerUnique, szServerName );

	if (fp)		fclose(fp);
	//================================================================================

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

// KHY - 0901 -  ���� ������ �ڵ����� ����.
void cltServer::GetServerSystemBuy()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 i;
	FILE* fp;
	TCHAR buffer[1024];
	TCHAR* pfilename = TEXT("ServerInfo\\SystemBuy.txt");

	fp = _tfopen(pfilename, TEXT("rt"));

	if(fp == NULL)		return;

	// ù��° 4���� �����Ѵ�. 
	for(i = 0;i < 4;i++)
		_fgetts(buffer, 1024, fp);

	TCHAR tempUnique[128];

	SI16 unique = 0;
	for(SI32 i = 0 ; i <MAX_SYSTEMBUYTEM_NUM ; i++)
	{
		if( _fgetts(buffer, 1024, fp) == NULL ) break;

		_stscanf(buffer, TEXT("%s"), tempUnique);
		unique = atoi(tempUnique);

		stSystemBuyItem[i].m_siUnique = unique;
	}

	if (fp)		fclose(fp);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::GetHanGameInfo()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	FILE* fp;
	TCHAR buffer[1024];
	TCHAR* pfilename = TEXT("ServerInfo\\HanGameInfo.txt");
	fp = _tfopen(pfilename, TEXT("rt"));
	if(fp == NULL)							return;

	TCHAR key[256], value[256];

	while(_fgetts(buffer, 1024, fp) != NULL)
	{
		_stscanf(buffer, TEXT("%s %s"), key, value);

		if(strcmp(key, "ServiceType") == 0)
		{
			siServiceType = 0;
#ifdef USE_HANRTA
			siServiceType = SERVICE_REAL;		// �ʱⰪ REAL
			if(strcmp(value, "ALPHA") == 0)			siServiceType = SERVICE_ALPHA;
			else if(strcmp(value, "REAL") == 0)		siServiceType = SERVICE_REAL;
#endif
		}
		else if(strcmp(key, "ServiceAge") == 0)
		{
			siServiceAge = atoi(value);
		}
	}

	fclose(fp);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::GetTopLevelVillage( SI32 *villageunique, SI32 *villagelevel )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 topvillage;
	SI32 toplevel;

	topvillage = 0;
	toplevel = -1;

	cltStrInfo *pStrInfo;

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		pStrInfo = GetStrInfo( i, RANKTYPE_CITYHALL );
		if( pStrInfo == NULL ) continue;

		if( pStrInfo->clCityhallStrInfo.siVillageLevel > toplevel ) {
			toplevel = pStrInfo->clCityhallStrInfo.siVillageLevel;
			topvillage = i;
		}
	}

	*villageunique = topvillage;
	*villagelevel = toplevel;

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return;
}

void cltServer::SendServerInfoToFront()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if( m_bGameserverinfo == FALSE ) return;

	if( pclFrontAuthProcess ) {

		if( pclFrontAuthProcess->GetFrontSession() ) {

			sResponseToFront_GameServerInfo sendMsg;

			sendMsg.packetHeader.usCmd = FRONTMSG_RESPONSE_GAMESERVERINFO;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.serverId = siServerID;
			sendMsg.gameserverinfo = m_gameserverinfo;

			ITZSendBuffer *pSendBuffer = pclFrontAuthProcess->GetFrontSession()->GetSendBuffer();

			if( pSendBuffer ) {
				pSendBuffer->Write( (BYTE*)&sendMsg );
			}

		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}


// Ư�� ĳ���Ϳ��� ��ǰ�� �ִ� ó���� �Ѵ�. 
void cltServer::InputItem(cltCharServer* pclchar, cltItem* pclitem, SI32 inputitemreason)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32 rtnvalue	= 0;

	if(pclchar->InputItem(pclItemManager, pclitem, inputitemreason, &rtnvalue) == false)
	{
		// �κ��� ���ڸ��� ��� �߰��� �� ����. 
		if(rtnvalue == CANADDINV_FAIL_FULL)
		{
			SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  pclitem->siUnique, 0, pclchar->GetCharUnique());
		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

SI32 cltServer::GetRandomID()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	SI32* plist		= NULL;
	SI32 listindex	= 0;
	SI32 id			= 0;
	SI32 index		= 0;

	plist = new SI32 [ MAX_PERSON_NUMBER ];

	while(id = pclCM->GetCharOrder(index))
	{
		index++;

		if( IsPC(id) == false )
			continue ;

		if ( ((cltCharServer*)pclCM->CR[id])->IsAutoHunting() == false )
			continue ;

		plist[listindex] = id ;
		listindex ++ ;
	}

	SI32 targetid = 0;
	if(listindex)
	{
		targetid = plist[ rand() % listindex];
	}

	delete[] plist;

	return targetid;
	//----------------------------------------------------------------
#else
	return 0;
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}


// ��ι������?���ʽ� Ÿ�� ����ڸ�?���Ѵ�. 
SI32 cltServer::PopSpecialQuestBonus()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(siSpecialQuestBonusBufferIndex <= 0)return 0;

	SI32 i;
	SI32* plist		= NULL;
	SI32 listindex	= 0;
	SI32 id			= 0;

	plist = new SI32 [siSpecialQuestBonusBufferIndex ];

	for(i = 0;i < siSpecialQuestBonusBufferIndex;i++)
	{
		id = pclCM->GetIDFromPersonID( siSpecialQuestBonusBuffer[ i] );
		if(pclCM->IsValidID(id))
		{
			plist [listindex] =  id;
			listindex++;
		}
	}

	SI32 targetid = 0;
	if(listindex)
	{
		targetid = plist[ rand() % listindex];
	}

	delete[] plist;

	pclLog->FilePrint(TEXT("Config\\SpecialQuest.log"), TEXT("Pop Index:%d"), siSpecialQuestBonusBufferIndex);
	return targetid;

	//----------------------------------------------------------------
#else
	return 0;
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

// ��ι������?���ʽ� Ÿ�� ����ڸ�?���ۿ� �ִ´�. 
bool cltServer::PushSpecialQuestBonus(SI32 personid)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(siSpecialQuestBonusBufferIndex >= MAX_SPECIAL_QUEST_BONUS_BUFFER_SIZE)return false;

	siSpecialQuestBonusBuffer[ siSpecialQuestBonusBufferIndex ] = personid;
	siSpecialQuestBonusBufferIndex++;


	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

	return true;
}

bool cltServer::PushCash(SI16 charunique, SI32 personid, TCHAR* szaccountid, GMONEY money, SI32 reason, SI32 day )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if( siServiceArea != ConstServiceArea_Korea ) return false;
	/*
	if ( personid < 1 )
	{
		return false;
	}

	if(PushCashListIndex >= MAX_PUSH_CASH_LIST_NUMBER)return false;

	PushCashList[PushCashListIndex].Set( charunique, personid, szaccountid, money, reason, day );
	PushCashListIndex++;

	TCHAR buffer[256]=TEXT("");
	sprintf(buffer, TEXT("AccountID:%s Money:%d"), szaccountid, money);

	bool bcheck = false;
	if(reason == PUSHCASH_REASON_QUEST)
	{
		strcat(buffer, TEXT("(Quest)"));
		bcheck = true;
	}
	else if(reason == PUSHCASH_REASON_GAMEMASTER)
	{
		strcat(buffer, TEXT("(GM)"));
		bcheck = true;
	}
	else if(reason == PUSHCASH_FOREIGNMERCHANTBUY)
	{
		strcat(buffer, TEXT("(ReSell)"));
		bcheck = true;
	}
	/*	else if(reason == PUSHCASH_REASON_ATTENDANCE)
	{
		strcat(buffer, TEXT("(Attendance)"));
		bcheck = true ;
	}*/
		/*
		if(bcheck)
		{
			pclLog->FilePrint(TEXT("Config\\Cash.log"), buffer);		
		}


		if(charunique)
		{
			SendServerResponseMsg(0, SRVAL_GIVECASH,  money, reason, charunique);
		}

		*/

			//----------------------------------------------------------------
			// ������ ������ ��
			//----------------------------------------------------------------
#endif //_SERVER
			//----------------------------------------------------------------

			return false;
}

bool cltServer::PopCash()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(PushCashListIndex <= 0)return false;
	//if(sTime.wMinute <= 1)return false;
	/*
	SI32 i;

	for(i = 0;i < PushCashListIndex;i++)
	{
		/*	
		sDBRequest_GiveCash clMsg( 
			PushCashList[i].usCharID,
			PushCashList[i].siPersonID, 
			PushCashList[i].szAccountID, 
			PushCashList[i].siMoney,
			PushCashList[i].siReason );

		pclGameDBProcess->SendMsg( DBSELECT_CASH, (sPacketHeader *)&clMsg);


		if( PushCashList[ i ].szAccountID[ 1 ] == '_' &&
			PushCashList[ i ].szAccountID[ 0 ] == 's' ) {


				NCashRequest_GiveEventCash sendMsg(
					PushCashList[i].usCharID,
					PushCashList[i].siPersonID, 
					&PushCashList[i].szAccountID[ 2 ], 
					PushCashList[i].siMoney,
					PushCashList[i].siReason,
					PushCashList[i].siDay,
					2 );

				pclNCashProcess->SendMsg( 2, (sPacketHeader *)&sendMsg );

			} else {

				NCashRequest_GiveEventCash sendMsg(
					PushCashList[i].usCharID,
					PushCashList[i].siPersonID, 
					PushCashList[i].szAccountID, 
					PushCashList[i].siMoney,
					PushCashList[i].siReason,
					PushCashList[i].siDay,
					1 );

				pclNCashProcess->SendMsg( 1, (sPacketHeader *)&sendMsg );
			}

	}
	*/
		PushCashListIndex = 0;


	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif _SERVER
	//----------------------------------------------------------------

	return true;

}

void cltServer::RefreshInvestList()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		if(pclVillageManager->pclVillageInfo[i])
		{
			// ���?������ ������ ������ ��û�Ѵ�. 
			sDBRequest_InvestList clMsgInvest(i);
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsgInvest);
		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------

}

void cltServer::RefreshStock()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// �ֽ��� �ֹ� ������ ��û�Ѵ�. 
	for( int i = 0;i < MAX_STOCK_ORDER_NUMBER;i+= GET_MANY_STOCK_ORDER_NUM)
	{
		sDBRequest_GetManyStockOrder clMsgSell(0, ORDERMODE_SELL, i);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgSell);

		sDBRequest_GetManyStockOrder clMsgBuy( 0, ORDERMODE_BUY, i);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgBuy);
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendLog_InventoryFull( SI32 personid, SI32 itemunique, SI32 itemnum, SI32 reason )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR log[ 512 ];

	StringCchPrintf( log, 512, TEXT("ItemNum = %d Reason = %d"), itemnum, reason );

	sDBRequest_WriteLog		sendMsg( personid, 0, itemunique, TEXT("Inventory Full"), log );
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendLog_GiveQuestItem(SI32 personid,SI32 questtype , SI32 questunique,SI32 itemunique,SI32 itemnum,SI32 ncoin,SI32 usedate,bool success )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR log[ 512 ];

	if ( success )
		StringCchPrintf( log, 512, TEXT("QuestType = %d QuestUnique = %d ItemNum = %d nCoin = %d ItemUseDate = %d Success = true"),questtype,questunique, itemnum, ncoin );
	else
		StringCchPrintf( log, 512, TEXT("QuestType = %d QuestUnique = %d ItemNum = %d nCoin = %d ItemUseDate = %d Success = false"),questtype,questunique, itemnum, ncoin );

	sDBRequest_WriteLog	sendMsg( personid, 0, itemunique, TEXT("Give QuestItem"), log );
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendLog_GetQuest(SI32 personid,SI32 questtype , SI32 questunique,bool start)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR log[ 512 ];

	if ( start )
		StringCchPrintf( log, 512, TEXT("QuestType = %d QuestUnique = %d Start"), questtype, questunique );
	else
		StringCchPrintf( log, 512, TEXT("QuestType = %d QuestUnique = %d End"), questtype, questunique );

	sDBRequest_WriteLog	sendMsg( personid, 0, 0, TEXT("Go QuestInfo"), log );
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

// ������ �����Ͼ�� ����̹Ƿ�?�� �ʿ��� �κи� ����ϵ���?����.
void cltServer::SendLog_Warp(SI32 personid, SI32 mapunique)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR log[ 512 ];

	StringCchPrintf( log, 512, TEXT("MapUnique = %d"), mapunique );

	sDBRequest_WriteLog	sendMsg( personid, 0, 0, TEXT("Warp To"), log );
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}


void cltServer::SendLog_MofuMofuEvent_Start(SI32 villageunique)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR log[ 512 ];

	StringCchPrintf( log, 512, TEXT("VillageUnique = %d"), villageunique );

	sDBRequest_WriteLog	sendMsg( 0, 0, 0, TEXT("MofuMofuEvent Start"), log );
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendLog_SetHappyBeanClearInfo(cltCharServer* pclchar, SI32 siQuestType, SI32 siQuestState)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(pclchar == NULL)				return;

	TCHAR	szServerName[50];

	StringCchPrintf( szServerName, 50, TEXT("%d"), siServerID );

	sDBRequest_Log_SetHappyBeanClearInfo	sendMsg(szServerName, pclchar->pclCI->clBI.szAccountID, (TCHAR*)pclchar->GetName(), pclchar->pclCI->GetPersonID(), siQuestType, siQuestState);
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

//LEEKH - 2007.10.29 - �Լ� �߰�
void cltServer::SendLog_SetEventCouponInfo(TCHAR* pszAccountID, TCHAR* pszCharName, SI32 siPersonID, SI32 siEventType, SI32 siPara)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(pszAccountID == NULL)		return;
	if(pszCharName == NULL)			return;
	if(siPersonID == 0)				return;

	TCHAR	szServerName[50];

	StringCchPrintf( szServerName, 50, TEXT("%d"), siServerID );

	sDBRequest_Log_SetEventCouponInfo	sendMsg(szServerName, pszAccountID, 
		pszCharName, siPersonID, siEventType,siPara);
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

//KHY - 0906 - ���ӹ� �̺�Ʈ ����.
void cltServer::SendLog_SetEventCouponInfo(cltCharServer* pclchar, SI32 siEventType , SI32 siPara)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(pclchar == NULL)				return;

	TCHAR	szServerName[50];

	StringCchPrintf( szServerName, 50, TEXT("%d"), siServerID );

	sDBRequest_Log_SetEventCouponInfo	sendMsg(szServerName, pclchar->pclCI->clBI.szAccountID, 
		(TCHAR*)pclchar->GetName(), pclchar->pclCI->GetPersonID(), siEventType,siPara);
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

////�����̺�Ʈ [2007.09.10 �ռ���]
//void cltServer::SendLog_SetAUCTIONEventCouponInfo(cltCharServer* pclchar, SI32 siEventType)
//{
//	//----------------------------------------------------------------
//	// ������ ������ ��
//	//----------------------------------------------------------------
//#ifdef _SERVER
//	//----------------------------------------------------------------
//
//	if(pclchar == NULL)				return;
//
//	TCHAR	szServerName[50];
//
//	StringCchPrintf( szServerName, 50, TEXT("%d"), siServerID );
//
//	sDBRequest_Log_SetEventCouponInfo	sendMsg(szServerName, pclchar->pclCI->clBI.szAccountID, 
//		(TCHAR*)pclchar->GetName(), pclchar->pclCI->GetPersonID(), siEventType);
//	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&sendMsg );
//
//	//----------------------------------------------------------------
//	// ������ ������ ��
//	//----------------------------------------------------------------
//#endif //_SERVER
//	//----------------------------------------------------------------
//}


// by LEEKH - 2007.08.21 - ���ο� �α� �ý���
void cltServer::SendLog_WriteLogToDB(SI16 siCommandType, SI16 siCommandIndex, UI08 uiReason, cltCharServer* pclchar, cltSummon* pclSummon, SI32 siTargetPersonID, GMONEY money, GMONEY moneysum, 
									 cltItem* pclItem, SI32 siHaveItemCount, SI32 siMonKind, SI16 siNpcKind, SI32 siVillageUnique, SI32 siRPPoint, SI32 siFameExp,
									 SI64 siParam1, SI64 siParam2, SI64 siParam3, SI64 siParam4, SI64 siParam5, TCHAR* pszCharParam1, TCHAR* pszCharParam2)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ���ο� �α׸� �������?�ʴ� ������ �ƹ��ϵ� ���� �ʴ´�.
	if(!pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		return;
	}

	sDBRequest_WriteLogToDB	clMsg;

	//StringCchPrintf(clMsg.m_szDate, sizeof(clMsg.m_szDate), TEXT("%04d-%02d-%02d %02d:%02d:%02d"), sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond);
	::GetLocalTime(&clMsg.m_stTime);

	// �����?����ID�� �̿��Ͽ� LOG ���̺� ������ ���� ���� �̸� ����
	// ����ID�� 100���� ������ ���� ��ȣ�� ��������.
	// MasterServer�� ServerInfo.txt�� �׷��� ������ �Ǿ��ִ�.
	switch(siServiceArea)
	{
	case ConstServiceArea_Korea:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "GoonzuSpecial", siServerID / 100 );
		}
		break;
	case ConstServiceArea_Japan:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "Japan", siServerID / 100 );
		}
		break;
	case ConstServiceArea_English:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "Global", siServerID / 100 );
		}
		break;
	case ConstServiceArea_Taiwan:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "Taiwan", siServerID / 100 );
		}
		break;
	case ConstServiceArea_China:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "China", siServerID / 100 );
		}
		break;
	case ConstServiceArea_USA:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "USA", siServerID / 100 );
		}
		break;
	case ConstServiceArea_NHNChina:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "NHNChi", siServerID / 100 );
		}
		break;
	case ConstServiceArea_EUROPE:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "EUROPE", siServerID / 100 );
		}
		break;
	default:
		{
			StringCchPrintf(clMsg.m_szServerName, sizeof(clMsg.m_szServerName), TEXT("%s%02d"), "Goonzu", siServerID / 100 );
		}
		break;
	}

	clMsg.m_siCommandType	= siCommandType;
	clMsg.m_siCommandIndex	= siCommandIndex;	
	clMsg.m_uiReason		= uiReason;

	if(pclchar)
	{
		if(pclchar->pclCI != NULL)
		{
			StringCchCopy(clMsg.m_szPersonName, sizeof(clMsg.m_szPersonName), pclchar->GetName());
			clMsg.m_szPersonName[sizeof(clMsg.m_szPersonName)-1] = '\0';
			clMsg.m_siPersonID		= pclchar->pclCI->GetPersonID();
			clMsg.m_siIDNum			= pclchar->pclCI->clBI.siIDNum;
			clMsg.m_siLevel			= pclchar->pclCI->clIP.GetLevel();
			clMsg.m_siExp			= pclchar->pclCI->clIP.GetCurExp();// SI32->SI64 ����ġ �����÷ο춧���� ����

			clMsg.m_siUseLog += NEWLOG_USE_HEALTHINFO; 
			clMsg.m_clHealthInfo.Set(&pclchar->pclCI->clHealthInfo);
		}

		if(pclchar->GetSession() != NULL && pclchar->GetSession()->GetIP() != NULL)
		{
			StringCchCopy(clMsg.m_szIP, sizeof(clMsg.m_szIP), pclchar->GetSession()->GetIP());
			clMsg.m_szIP[sizeof(clMsg.m_szIP) - 1] = '\0';
		}

	}
	clMsg.m_siTargetPersonID	= siTargetPersonID;

	clMsg.m_siMoney				= money;
	clMsg.m_siMoneySum			= moneysum;

	if(pclItem)
	{
		clMsg.m_siUseLog += NEWLOG_USE_ITEM;
		clMsg.m_clItem.Set(pclItem);
	}
	clMsg.m_siItemSum			= siHaveItemCount;

	if(pclSummon)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SUMMON;
		clMsg.m_clSummon.Set(pclSummon);
	}

	clMsg.m_siMonKind			= siMonKind;
	clMsg.m_siVillageUnique		= siVillageUnique;
	clMsg.m_siNpcKind			= siNpcKind;

	clMsg.m_siRPPoint			= siRPPoint;
	clMsg.m_siFameExp			= siFameExp;

	if(siParam1 != 0)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SIPARAM1;
		clMsg.m_siParam1			= siParam1;
	}

	if(siParam2 != 0)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SIPARAM2;
		clMsg.m_siParam2			= siParam2;
	}

	if(siParam3 != 0)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SIPARAM3;
		clMsg.m_siParam3			= siParam3;
	}

	if(siParam4 != 0)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SIPARAM4;
		clMsg.m_siParam4			= siParam4;
	}

	if(siParam5 != 0)
	{
		clMsg.m_siUseLog += NEWLOG_USE_SIPARAM5;
		clMsg.m_siParam5			= siParam5;
	}

	if(pszCharParam1)
	{
		clMsg.m_siUseLog += NEWLOG_USE_CHARPARAM1;
		StringCchCopy(clMsg.m_szCharParam1, sizeof(clMsg.m_szCharParam1), pszCharParam1);
		clMsg.m_szCharParam1[sizeof(clMsg.m_szCharParam1)-1] = '\0';
	}

	if(pszCharParam2)
	{
		clMsg.m_siUseLog += NEWLOG_USE_CHARPARAM2;
		StringCchCopy(clMsg.m_szCharParam2, sizeof(clMsg.m_szCharParam2), pszCharParam2);
		clMsg.m_szCharParam2[sizeof(clMsg.m_szCharParam2)-1] = '\0';
	}

	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg );

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

BOOL cltServer::GMOrder( TCHAR *pOrder )
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	TCHAR order[256];
	TCHAR szOrder[ 256 ];
	TCHAR cmd[ 256 ];

	const SI32 ConstPersonNameSize = 128;

	memcpy( szOrder, pOrder, 256 );
	szOrder[ 255 ] = NULL;

	// ��� �����?���?��Ͽ�?�����? 
	pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("Monitering Client: [%s]"), &szOrder[1] );

	_stscanf( szOrder, TEXT("%s"), cmd );

	if( _tcscmp( cmd, TEXT("/nt") ) == 0 ) {

		TCHAR szMsgtext[1024];
		StringCchCopy(szMsgtext, 1024, &szOrder[ 4 ] );
		TCHAR* pszText = GetTxtFromMgr(5482); 

		// ��ڰ� �ϴ°��̶��?ä��â���� ��Ÿ�����Ѵ�.
		//KHY - 0129 - �ϴ� ä��â�� ���� ��¿��θ�?�����Ѵ�.
		cltGameMsgResponse_Notice clNotice(pszText, szMsgtext,true);
		cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg); 
	}

	else if( _tcscmp( cmd, TEXT("/nc") ) == 0 ) {

		//TCHAR szMsg[1024]={0,};
		//TCHAR szMsgtext[1024]={0,};
		//TCHAR szInterval[1024]={0,} ;

		//StringCchCopy(szMsg, 1024, &szOrder[ 4 ] );

		//_stscanf(szMsg, TEXT("%s"),szInterval);

		//SI32 size = _tcslen(szInterval);
		//if(size < 1024-1)
		//{
		//	StringCchCopy(szMsgtext, 1024, &szMsg[size] );
		//}

		////cyj nc��ɿ���?������ �ֵ��� ����
		//DWORD dwinterval = _tstoi(szInterval) * 1200;

		//if((dwinterval == 0) || (!_tcscmp(szMsgtext,TEXT(""))))
		//{
		//	return FALSE;
		//}

		//pclNoticeManager->Add(dwinterval, szMsgtext);
		SI32 siInterval	=	0;
		SI32 siCount =	0	;

		TCHAR Text[2048] ;
		ZeroMemory(Text, sizeof(Text));	

		_stscanf(&szOrder[1], TEXT("%s %d %d %s "), order, &siInterval, &siCount,Text);

		//cyj nc��ɿ���?������ �ֵ��� ����
		DWORD dwinterval = siInterval * 1200;

		if( (dwinterval <= 0) || (!_tcscmp(Text,TEXT(""))) || (siCount <= 0) )
		{
			return FALSE;
		}

		pclNoticeManager->Add(dwinterval, Text, siCount);
	}

	else if( _tcscmp( cmd, TEXT("/ncdel") ) == 0 ) {
		pclNoticeManager->DelAllNotice();
	}

	else if( _tcscmp( cmd, TEXT("/block") ) == 0 ) {
		pclFrontAuthProcess->m_refreshInfo.state = 2;
	}

	else if( _tcscmp( cmd, TEXT("/out") ) == 0 ) {
		pclCM->CloseAllSocket();
	}

	else if( _tcscmp( cmd, TEXT("/unblock") ) == 0 ) {
		pclFrontAuthProcess->m_refreshInfo.state = 0;
	}


	/*else if( _tcscmp( cmd, TEXT("/giftnt") ) == 0 ) {

	TCHAR RecvGiftName[ 256 ] = TEXT("");
	TCHAR MasterNoticeMsg[ 256 ] =TEXT("");

	_stscanf( szOrder, TEXT("%s %s"), cmd, RecvGiftName );

	SI32 size = _tcslen( RecvGiftName );

	if( size == 0 || size > 32 ) return FALSE;

	StringCchCopy( MasterNoticeMsg, 256, &szOrder[ 9 + size ] );

	RecvGiftName[ 255 ] = NULL;
	MasterNoticeMsg[ 255 ] = NULL;

	if( _tcslen( RecvGiftName ) < 1 )
	{
	}
	else
	{
		cltGameMsgResponse_GiftNT clinfo( RecvGiftName, MasterNoticeMsg );
		cltMsg clMsg(GAMEMSG_RESPONSE_GIFTNT, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
	}
}*/

/*else if( _tcscmp( cmd, TEXT("/ntlogin") ) == 0 ) {
pclNoticeManager->OnOffLogInNotice();
}*/

	else if( _tcscmp( cmd, TEXT("/ntadmin") ) == 0 ) {

		TCHAR buffer[ 256 ];
		MStrCpy( buffer, &szOrder[ 9 ], 200 );

		cltGameMsgResponse_NoticeAdmin clNoticeAdmin(buffer);
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTICEADMIN, sizeof(clNoticeAdmin), (BYTE*)&clNoticeAdmin);

		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
	}
	//// ���Ǳ��� �̺�Ʈ �߻� (�߱���)
	//else if(_tcscmp( cmd, TEXT("/blackwar")) == 0)
	//{
	//	pclBlackWarManager->MakeWar( CurrentClock, sTime.wHour );
	//}
	//// ���Ǳ��� �̺�Ʈ �߻� (����)
	//else if(_tcscmp( cmd, TEXT("/blackarmy")) == 0)
	//{
	//	pclBlackArmyManager->MakeWar(CurrentClock);
	//}
	//// ���Ǳ��� �̺�Ʈ ���� (�߱��� - ����) 
	//else if(_tcscmp( cmd, TEXT("/blackwarend")) == 0)
	//{		
	//	pclBlackWarManager->SetLeftTime( 5000 );
	//}
	//// ���Ǳ��� �̺�Ʈ ���� (���� - Ư����)
	//else if(_tcscmp( cmd, TEXT("/blackarmyend")) == 0)
	//{		
	//	pclBlackArmyManager->AllWarEnd();
	//}
	//// ���� ��û ���� ����ġ
	//else if(_tcscmp( cmd, TEXT("/warapply")) == 0)
	//{
	//	SI32 ApplySwitch = 0;
	//	_stscanf( szOrder, TEXT("%s %d"), cmd, &ApplySwitch);

	//	if (ApplySwitch == 1)
	//	{
	//		pclBlackArmyManager->bApplySwitch = true;
	//	}
	//	else
	//	{
	//		ApplySwitch = 0;
	//		pclBlackArmyManager->bApplySwitch = false;
	//	}

	//	//SendServerResponseMsg(0, SRVAL_BLACKARMY_WARAPPLY,	ApplySwitch, 0, pclchar->GetCharUnique());
	//}
	//cyj ���ʽ� Ÿ�� ���� �޴� �߰�
	else if(_tcscmp(cmd, TEXT("/sbonus")) == 0)
	{
		if (bBonusTimeSwitch == FALSE)
		{
			bBonusTimeSwitch = TRUE;
			bGMBonusTimeSwitch = 1;
			bBonusTimeGoonzu = true;
		}
	}
	//cyj ���ʽ� Ÿ�� ���� �޴� �߰�
	else if(_tcscmp(cmd, TEXT("/ebonus")) == 0)
	{
		if (bBonusTimeSwitch == TRUE)
		{
			bBonusTimeSwitch = FALSE;
			bGMBonusTimeSwitch = 1;
			bBonusTimeGoonzu = false;
		}
	}
	//cyj �������ʽ� Ÿ�� ���� �޴� �߰�
	else if(_tcscmp(cmd, TEXT("/smakebonus")) == 0)
	{
		if (bBonus2TimeSwitch == FALSE)
		{
			bBonus2TimeSwitch = TRUE;
			bGMBonusTimeSwitch = 2;
		}
	}
	//cyj �������ʽ� Ÿ�� ���� �޴� �߰�
	else if(_tcscmp(cmd, TEXT("/emakebonus")) == 0)
	{
		if (bBonus2TimeSwitch == TRUE)
		{
			bBonus2TimeSwitch = FALSE;
			bGMBonusTimeSwitch = 2;
		}
	}
	else if(_tcscmp(cmd,TEXT("/skoinobory")) == 0)
	{
		pclKoinoboryFamilyManager->StartEvent();
	}
	else if(_tcscmp(cmd,TEXT("/ekoinobory")) == 0)
	{
		pclKoinoboryFamilyManager->EndEvent();
	}
	else if(_tcscmp(cmd,TEXT("/userkick")) == 0)
	{
		TCHAR szPersonName[ConstPersonNameSize] = {0,};
		_stscanf(&szOrder[1], TEXT("%s %s"), order, szPersonName);
		//bool bconnectswitch = false;

		SI32 id = pclCM->GetIDFromName(szPersonName);			
		if ( pclCM->IsValidID( id ) )
		{
			//bconnectswitch = true;
			cltCharServer* pKickedChar= (cltCharServer*)(pclCM->CR[id]);
			pKickedChar->bCloseSocketSwitch = true;
			pKickedChar->dwCloseSocketTimer = 5;
		}
		else
		{
			return FALSE;
		}
	}
	else if(_tcscmp(cmd,TEXT("/userblock")) == 0)
	{
		TCHAR szPersonName[ConstPersonNameSize] = {0,};

		SI32 siDay = 0;
		_stscanf(&szOrder[1], TEXT("%s %s %d"), order, szPersonName, &siDay);

		SI32 id = pclCM->GetIDFromName(szPersonName);			
		if ( pclCM->IsValidID( id ) )
		{
			//bconnectswitch = true;
			cltCharServer* pKickedChar= (cltCharServer*)(pclCM->CR[id]);
			pKickedChar->bCloseSocketSwitch = true;
			pKickedChar->dwCloseSocketTimer = 5;
		}
		else
		{
			return FALSE;
		}

		if(siDay < 0 || siDay > 10)
			return FALSE;

		ConvertUnderLineToSpace( szPersonName, ConstPersonNameSize );

		TCHAR szTemp[ MAX_BLOCKREASON_SIZE ];
		StringCchPrintf(szTemp, MAX_BLOCKREASON_SIZE, TEXT("%s %s %d"), order, szPersonName, siDay);
		SI32 siStartpos = lstrlen( szTemp );
		TCHAR* pszStartpos = &szOrder[1] + siStartpos;

		TCHAR szText[MAX_BLOCKREASON_SIZE];
		MStrCpy( szText, pszStartpos, MAX_BLOCKREASON_SIZE );
		szText[ MAX_BLOCKREASON_SIZE - 1 ] = '\0';

		SI32 siBlockmode = 1;
		if(siDay == 0)
			siBlockmode = 2;	

		sDBRequest_BlockUser clMsg(0, siBlockmode, szPersonName, siDay, szText);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else if(_tcscmp(cmd,TEXT("/userunblock")) == 0)
	{
		TCHAR szPersonName[ConstPersonNameSize] = {0,};
		_stscanf(&szOrder[1], TEXT("%s %s"), order, szPersonName);

		TCHAR* pText = GetTxtFromMgr(5409);
		sDBRequest_BlockUser clMsg(0, 0, szPersonName, 0, pText);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	}
	else
	{
		return FALSE;
	}
	/*else if( _tcscmp( cmd , TEXT("/ssantamon")) == 0 )
	{
		pclSantaRaccoonManager->StartEvent();
	}
	else if( _tcscmp( cmd , TEXT("/esantamon")) == 0 )
	{
		pclSantaRaccoonManager->EndEvent();
	}*/

		//----------------------------------------------------------------
		// ������ ������ ��
		//----------------------------------------------------------------
#endif //_SERVER
		//----------------------------------------------------------------
		return TRUE;
}

void cltServer::RestartTradeServer()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	pclNTradeServerProcess->GetTradeServerServersSession()->CloseSocket();
	delete pclNTradeServerProcess;
	pclNTradeServerProcess = NULL;

	pclNTradeServerProcess = new NTradeServerProcess();
	pclNTradeServerProcess->Init();

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SendSet_InstanceProfitVillage(SI32 charid,bool start)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if ( start == false )
	{
		SI32 homevillage = 0 ;
		if ( charid != -1)
		{
			if ( pclCM->IsValidID(charid) == false ) return ;

			homevillage = pclCM->CR[charid]->pclCI->clBI.siHomeVillage ;
			if ( pclVillageManager->IsValidVillage(homevillage) == false )
			{
				homevillage = 0 ;
			}

			SI32 index = 0;
			SI32 id;
			while(id = pclCM->GetCharOrder(index))
			{
				index++;

				if( IsPC(id) == false ) continue;

				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
				if ( pclchar == NULL ) continue ;
				if ( pclchar->IsValidConnection() == false ) continue ;

				SendServerResponseMsg(0,SRVAL_INSTANCEPROFIT_VILLAGE,homevillage,0,pclchar->GetCharUnique());
			}
		}

		sDBRequest_SetInstanceProfit_Village sendMsg(homevillage,false);
		pclGameDBProcess->SendMsg( DBSELECT_BASE, (sPacketHeader *)&sendMsg );
	}
	else
	{
		sDBRequest_SetInstanceProfit_Village sendMsg(0,true);
		pclGameDBProcess->SendMsg( DBSELECT_BASE, (sPacketHeader *)&sendMsg );
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

void cltServer::SetTodayVillageWarInfo()
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	NDate StandardDate;	// ���� �ð�. ���� ������ ���� �ð����� ����
	StandardDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);

	for(SI32 i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		if( pclVillageManager->siDeclareWarVillage[i] > 0  ) {


			// ���� ���ῡ ���۵Ǵ� �������̶��?siNowWarVillage ������ �߰�
			NDate * DeclareWarDate = 	&pclVillageManager->clDeclareWarDate[i];

			// ������ ���� ��¥�� ���� 8�ø� �������� 3�� ���̾��ٸ� ���������� ����.
			if(DeclareWarDate->GetDateVary() + 2 == StandardDate.GetDateVary() )			
			{
				pclVillageManager->siNowWarVillage[i] = pclVillageManager->siDeclareWarVillage[i];
			}
		}
	}

	// ������ ���?Ŭ���̾�Ʈ����(���?Ŭ���̾�Ʈ�� ���������� �̸� ������ �ִ�) ���ŵ� ���� ���� ������ ������Ʈ

	cltGameMsgResponse_GetNowVillageWarInfo clinfo;
#ifdef _SAFE_MEMORY
	memcpy(clinfo.siNowWarVillage, &pclVillageManager->siNowWarVillage[0], sizeof(clinfo.siNowWarVillage) );
#else
	memcpy(clinfo.siNowWarVillage, pclVillageManager->siNowWarVillage, sizeof(clinfo.siNowWarVillage) );
#endif
	cltMsg clMsg(GAMEMSG_RESPONSE_GETNOWVILLAGEINFO,sizeof(clinfo), (BYTE*)&clinfo);

	pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg );

	clLastSettedNowVillageWarIinfo.uiYear = sTime.wYear - 2000;
	clLastSettedNowVillageWarIinfo.uiMonth = sTime.wMonth;
	clLastSettedNowVillageWarIinfo.uiDay = sTime.wDay;

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}


//----------------------------
//  �̺�Ʈ ������ �Ǽ��ϱ�
//----------------------------
bool cltServer::HireEventNPC(SI32 kind, SI32 villageunique, UI16 siSlot, bool mode)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	// ������ ����
	if( kind   < 0			|| kind			 >	MAX_KIND_NUMBER )			return false;
	if( siSlot < 0			|| siSlot		 >=	MAX_HIRED_EVENTNPC_NUM )	return false;
	if( villageunique <= 0	|| villageunique >= MAX_VILLAGE_NUMBER )		return false;

	if( mode == true )
	{
		SI32 siNpcID;
		// ���?	
		if (kind == pclClient->GetUniqueFromHash( TEXT("KIND_RETURNTOWER")))
		{ 
			//��ȯž ��ġ
			POINT posWarpTower = { 36, 30 };
			siNpcID = SetNPCChar(kind, villageunique, NULL, 1, 0, posWarpTower.x, posWarpTower.y , villageunique, NPCITEMMODE_NONE, NULL );
		}
		else
		{
			POINT pos = { 23, 30 };
			siNpcID = SetNPCChar(kind, villageunique, NULL, 1, 0, pos.x, pos.y , villageunique, NPCITEMMODE_NONE, NULL );
		}
		
		cltCharServer* pclNpcChar = pclCM->GetCharServer(siNpcID);
		if( NULL == pclNpcChar ) return false;
			
		pclNpcChar->bStaySwitch = true; // ���ڸ��� ���缭 �־��?�Ѵ�. 
	}
	else
	{
		// Ʈ�� ö��
		cltStrInfo* pclStrInfo = pclCityHallManager->GetStrInfo( villageunique );
		if(pclStrInfo == NULL)	return false;

		SI32 siEventNpckind = pclStrInfo->clCityhallStrInfo.clHiredEventNPC[ siSlot ].siNPCKind;
		// �� ������ NPC�� siEventNpckind�� �����Ѵ�. 
		if( siEventNpckind )
		{
			SI32 index = 0;
			SI32 id;
			while( id = pclCM->GetCharOrder(index) )
			{
				index++;

				// �������?�Ŀ��Ը� �����Ѵ�. 
				if(pclCM->CR[id]->GetKind() == siEventNpckind)
				{
					if(pclCM->CR[id]->GetCurrentVillageUnique() == villageunique)
					{
						// �� �������� ������ �����Ѵ�. 
						pclCM->CR[id]->DeleteInCell();
						if( pclCM->DeleteChar(id) == TRUE )
						{
							index--;
						}
					}
				}
			}
		}
	}

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
	return true;
}

void cltServer::HanAuth_Lib_HanReportSendPosLog_Server(TCHAR* pszID, TCHAR* pszValue)
{
	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#ifdef _SERVER
	//----------------------------------------------------------------

	if(pszID == NULL || strlen(pszID) <= 0)			return;
	if(pszValue == NULL || strlen(pszValue) <= 0)	return;

	TCHAR	szReportString[1024];
	StringCchPrintf(szReportString, sizeof(szReportString), "id=%s&value=%s", pszID, pszValue);

#ifdef USE_HANREPORT
#if defined(_KOREA)		// �ѱ� ����
	if(siServiceType == SERVICE_ALPHA)
		;//HanReportSendPosLog(szReportString);		// ���Ŀ��� ���� ���� ����. 
	else
		;//HanReportSendPosLog(szReportString);		// ���󿡼��� ���� ���� 
#elif defined(_USA)		// �̱� ����
	HanReportSendPosLog(szReportString);
#endif
#endif // USE_HANREPORT

	// TEST LOG
	//WriteFileLog("HanReport.txt", __FILE__, __LINE__, "id=%s&value=%s", pszID, pszValue);

	//----------------------------------------------------------------
	// ������ ������ ��
	//----------------------------------------------------------------
#endif //_SERVER
	//----------------------------------------------------------------
}

#ifdef USE_GAME_GUARD_SERVER

GGAUTHS_API void NpLog(int mode , char* msg)
{
	if( msg == NULL || msg[0] == 0) return;

	TCHAR	szFileName[MAX_PATH] = { '\0', };
	TCHAR	szType[MAX_PATH] = { '\0', };

	if( mode & NPLOG_DEBUG ||
		mode & NPLOG_ERROR )
	{
		// DEBUG LOG
		if( mode & NPLOG_DEBUG )		StringCchPrintf(szType, sizeof(szType), TEXT("%s"), "Debug");
		// ERROR LOG
		if( mode & NPLOG_ERROR )		StringCchPrintf(szType, sizeof(szType), TEXT("%s"), "Error");

		SYSTEMTIME CT;
		GetLocalTime(&CT);

		TCHAR szFileName[256] = "";
		StringCchPrintf(szFileName, sizeof(szFileName), TEXT("GameGuardLog\\GameGuard%s_%02d%02d%02d.txt"), szType, CT.wYear, CT.wMonth, CT.wDay);

		FILE * fp = NULL;
		fp = fopen(szFileName, "a+");

		if( fp )
		{
			fprintf(fp , "[%02d:%02d:%02d]\t%s\n" ,CT.wHour , CT.wMinute , CT.wSecond ,msg);
			fclose(fp);
		}		
	}
}

GGAUTHS_API void GGAuthUpdateCallback(PGG_UPREPORT repot)
{
	TCHAR szReportMsg[1024] = "";
	StringCchPrintf( szReportMsg, sizeof(szReportMsg), TEXT("GGAuth version update [%s] : [%ld] -> [%ld] \n") ,
		repot->nType == 1 ? TEXT("GameGuard Ver") : TEXT("Protocol Num"), repot->dwBefore, repot->dwNext );

	// ���Ӱ��� ������Ʈ �α׸� �����?

	SYSTEMTIME CT;
	GetLocalTime(&CT);

	TCHAR szFileName[MAX_PATH] = "";
	StringCchPrintf(szFileName, sizeof(szFileName), TEXT("GameGuardLog\\GameGuardUpdate_%02d%02d%02d.txt"), CT.wYear, CT.wMonth, CT.wDay);

	FILE * fp = NULL;
	fp = fopen(szFileName, "a+");

	if( fp )
	{
		fprintf(fp , szReportMsg);
		fclose(fp);
	}
}

#endif	// USE_GAME_GUARD_SERVER

#ifdef USE_HANXNOTI

SI32 cltServer::HanXNoti_GetEmptyWaiting(SI32 siIdx)
{
	SI32	siRet = -1;

	EnterCriticalSection(&m_HanXNoti_CS);
	{
		for(SI32 i=0; i<HANXNOTI_MAX_WAITING; i++)
		{
			if(m_kHanXNoti_Waitings[i].m_siIdx < 0)
			{
				::ResetEvent(m_kHanXNoti_Waitings[i].m_hWaitingHandle);
				m_kHanXNoti_Waitings[i].m_siIdx = siIdx;
				siRet = i;

				break;
			}
		}
	}
	LeaveCriticalSection(&m_HanXNoti_CS);

	return siRet;
}

void cltServer::HanXNoti_ClearWaiting(SI32 siArrayIndex)
{
	if(siArrayIndex < 0 && siArrayIndex >= HANXNOTI_MAX_WAITING)		return;

	EnterCriticalSection(&m_HanXNoti_CS);
	{
		m_kHanXNoti_Waitings[siArrayIndex].m_siIdx = -1;
		if(m_pkHanXNoti_AnsMessages[siArrayIndex] != NULL)
		{
			delete m_pkHanXNoti_AnsMessages[siArrayIndex];
			m_pkHanXNoti_AnsMessages[siArrayIndex] = NULL;
		}
	}
	LeaveCriticalSection(&m_HanXNoti_CS);
}

#endif // USE_HANXNOTI


SI32 cltServer::GetGuildWarBattleIndex( SI32 siGuildUnitIndex )
{
	if ( IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// ������
	{
		for(SI32 i=0; i<GUILDWAR_FIELDNUM_OLD; i++)
		{
			if(i >= GUILDWAR_FIELDNUM_OLD) continue	;
			if( pclGuildWarManager[i]->m_siAwayGuildUnique == siGuildUnitIndex ||
				pclGuildWarManager[i]->m_siHomeGuildUnique == siGuildUnitIndex )
			{
				return i;
			}
		}

	}
	else	// ������
	{
		for(SI32 i=0; i<GUILDWAR_FIELDNUM; i++)
		{
			if(i >= GUILDWAR_FIELDNUM) continue	;
			if( pclGuildWarManager[i]->m_siAwayGuildUnique == siGuildUnitIndex ||
				pclGuildWarManager[i]->m_siHomeGuildUnique == siGuildUnitIndex )
			{
				return i;
			}
		}
	}
	return -1;
}

// [����] Auto Unblock : ���Ϸκ��� AutoUnblock ���?�о����?- 2008.02.21
void cltServer::LoadAutoUnblockFromFile(OUT bool *bAutoUnblockMode, OUT SI32 *siGameVersion)
{
	FILE	*fp			= _tfopen( TEXT("Config/AutoUnblock.dat"), TEXT("rt") );
	SI32 siGetMode		= 0;
	SI32 siGetVersion	= 0;

	*bAutoUnblockMode	= false;
	*siGameVersion		= 0;

	if ( fp != NULL )
	{
		_ftscanf( fp, TEXT("%d %d"), &siGetMode, &siGetVersion );

		if (siGetMode > 0)
		{
			*bAutoUnblockMode = true;
		}
		else
		{
			*bAutoUnblockMode = false;
		}

		*siGameVersion = siGetVersion;

		fclose( fp );
	}
}

// [����] Auto Unblock : ���Ͽ� AutoUnblock ���?���� - 2008.02.21
void cltServer::SaveAutoUnblockToFile(IN bool bAutoUnblockMode, IN SI32 siGameVersion )
{
	FILE	*fp				= _tfopen( TEXT("Config/AutoUnblock.dat"), TEXT("wt") );	
	SI32	siSetMode		= (bAutoUnblockMode?1:0);


	if ( fp != NULL)
	{
		_ftprintf(fp, TEXT("%d %d"), siSetMode, siGameVersion);

		fclose(fp);
	}
}

// [����] �ʺ��� ���� - ��ī�� : ������ ������ ���ַ� ������ִ�?�Լ�
void cltServer::BegnnerVillage_SetVillageChief( SI32 siPersonID )
{
	if ( siPersonID <= 0 )
	{
		return;
	}

	cltRank* pclRank = pclRankManager->GetRankInfo( siPersonID );
	if ( pclRank != NULL )
	{
		if ( pclRank->GetRankType() == RANKTYPE_CITYHALL )
		{
			return;
		}
	}

	cltSimpleRank clSimpleRank( RANKTYPE_CITYHALL, Const_Beginner_Village );

	sDBRequest_Candidate clMsg( 0, siPersonID, &clSimpleRank );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	/*
	//-- �̹� ������ ������ ���?�ʺ��� ������ ���ַ� �Ӹ����� �ʴ´�
	cltRank* pclRank = pclRankManager->GetRankInfo( siPersonID );
	if ( pclRank != NULL )
	{
		if ( pclRank->GetRankType() == RANKTYPE_CITYHALL )
		{
			return;
		}
	}

	//-------------------------------------------------------------------------------------------
	//	�ʺ��� ������ ���� ���ְ� �������?�ذ� ��Ų��
	//-------------------------------------------------------------------------------------------

	cltRank* pclOldRank = pclRankManager->GetRank( RANKTYPE_CITYHALL, Const_Beginner_Village );

	if ( pclOldRank != NULL )
	{
		cltRank clOldRank;
		SI32	siOldPersonID	= 0;
		SI32	siOldCharID		= 0;

		clOldRank.Set( pclOldRank );

		siOldPersonID	= clOldRank.clPerson.GetPersonID();
		siOldCharID		= pclCM->GetIDFromPersonID( siOldPersonID );

		cltLetterMsg_Fire clFire( &clOldRank.clRank, &pclTime->clDate );

		SendLetterMsg( siOldPersonID, (cltLetterHeader*)&clFire );

		if ( pclCM->IsValidID(siOldCharID) )
		{
			pclCM->CR[siOldCharID]->pclCI->clCharRank.Init();
			pclCM->CR[siOldCharID]->SetUpdateSwitch( UPDATE_RANK, true, 0 );
		}
	}


	//-------------------------------------------------------------------------------------------
	//	������ ���ַ� �Ӹ��Ѵ�
	//-------------------------------------------------------------------------------------------

	//-- ���� ����
	cltStrInfo	*pclStrInfo = GetStrInfo( Const_Beginner_Village, RANKTYPE_CITYHALL );
	pclStrInfo->clCityhallStrInfo.clChiefDate.Set( &pclTime->clDate );

	//-- �ź�����
	cltSimpleRank	clChiefRank;
	clChiefRank.Set( RANKTYPE_CITYHALL, Const_Beginner_Village );

	cltSimplePerson	clChiefPerson;
	clChiefPerson.Set( siPersonID, pclCM->CR[siCharID]->GetName() );

	//-- ���ο� �ź����� ������Ʈ
	pclRankManager->SetRank( &clChiefRank, &clChiefPerson );

	//-- �Ӹ��?���ۿ��� ������ ������
	cltLetterMsg_BeRank clBeRank( &clChiefRank, &pclTime->clDate );

	SI32 siLetterID = SendLetterMsg( siPersonID, (cltLetterHeader*)&clBeRank );
	if ( pclCM->IsValidID( siLetterID ) )
	{
		pclCM->CR[siLetterID]->pclCI->clCharRank.Set( &clChiefRank );
		pclCM->CR[siLetterID]->SetUpdateSwitch( UPDATE_RANK, true, 0 );
	}
	*/
}

void cltServer::ValentineDayNpc( void )
{
	static DWORD dwTickCount = GetTickCount();
	if( GetTickCount() - dwTickCount < 60000 )	return;

	dwTickCount = GetTickCount();

	static SI32 siValentineDayNpcID = 0;
	
	if( pclEventTimeManager->InEventTime( "DayTradeItem", &sTime ) )
	{
		if( 0 >= siValentineDayNpcID )
		{
			siValentineDayNpcID = SetNPCChar(	GetUniqueFromHash("KIND_GIFTRACCOON"), 
												VILLAGE_NONE, 
												NULL, 
												1, 
												0, 
												280, 
												337, 
												0, 
												NPCITEMMODE_NONE, 
												NULL );
		}
	}
	else
	{
		// ������ NPC ����
		if( 0 < siValentineDayNpcID )
		{
			cltCharServer* pclChar = pclCM->GetCharServer( siValentineDayNpcID );
			if( pclChar )
			{
				pclChar->DeleteInCell();
				pclCM->DeleteChar( pclChar->GetID() );

				siValentineDayNpcID = 0;
			}
		}
	}
}

//KHY - 0530 �����?���� ��������. - ���� ��¥�� ��ϵ�?��¥�� ���Ͽ� ���� ���?����. - �ѹ��� �����Ѵ�.  
bool cltServer::CanChargingBuyLimit( )
{
	if(pclTime == NULL)			return false;

	// �Ѵ��߿� �ְ� ���� ��¥�� �̰��� ����ϸ�?�ȴ�. 
	SI32 SelectDay[] = {1};  
	SI32 numday = sizeof(SelectDay) / sizeof(SI32);

	bool bdaychangedswitch = pclTime->IsDayChanged();
	SI16 today = pclTime->clDate.GetDay();

	// �Ϸ翡 �ѹ��� -���ӽð�.
	if( bdaychangedswitch )
	{
		if(gForeignMerchantBuyLimit > 0)
		{
			for(SI32 i = 0; i < numday ; i++)
			{
				if(today == SelectDay[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

// KHY - 0901 -  ���� ������ �ڵ����� ����.
bool cltServer::CanChargingSystemBuyItemLimit( )
{
	if(pclTime == NULL)			return false;

	// �Ѵ��߿� �ְ� ���� ��¥�� �̰��� ����ϸ�?�ȴ�. 
	SI32 SelectDay[] = {1};  
	SI32 numday = sizeof(SelectDay) / sizeof(SI32);

	bool bdaychangedswitch = pclTime->IsDayChanged();
	SI16 today = pclTime->clDate.GetDay();

	// �Ϸ翡 �ѹ��� -���ӽð�.
	if( bdaychangedswitch )
	{
		if(siMarketMoneyItemMaxpriceNow > 0)
		{
			for(SI32 i = 0; i < numday ; i++)
			{
				if(today == SelectDay[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

void cltServer::SystemMarketSell( void )
{
	if( pclTime == NULL	)					return;

	if( false == pclTime->IsDayChanged() )	return;
	if( 1 != pclTime->clDate.uiDay     )	return;	// ���ַ� 1�� ���� üũ.

	cltTradeOrder*		pclOrder	 = NULL;
	cltItemPriceUnit*	pclprice	 = NULL;
	SI32				siItemUnique = 0;
	SI32				siRef		 = 0;
	SI32				siEquipLevel = 0;
	SI32				sOrderCount  = 0;
	bool				bAddItem     = false;
	SI32				siItemCount[MAX_ITEMINFO_NUMBER];	ZeroMemory( siItemCount, sizeof(siItemCount) );

	for( SI32 Count = 0; Count < MAX_MARKET_ORDER_NUMBER; ++Count )
	{
		pclOrder = pclMarketManager->pclOrderManager->GetOrder( ORDERMODE_SELL, Count );
		if( NULL == pclOrder )	continue;

		siItemUnique = pclOrder->clItemData.siUnique;
		if( 0 >= siItemUnique )	continue;	

		siRef = pclItemManager->FindItemRefFromUnique( siItemUnique );
		if( 0 >= siRef )		continue;

		siEquipLevel = pclItemManager->pclItemInfo[siRef]->clCondition.siLevel;
		if( 1 < siEquipLevel && 26 > siEquipLevel ) 
		{
			// ���?���������� Ȯ��.
			bAddItem = false;
			switch(pclItemManager->GetItemType( siItemUnique ))
			{
			case ITEMTYPE_SWORD:
			case ITEMTYPE_SPEAR:
			case ITEMTYPE_STAFF:
			case ITEMTYPE_AXE:
			case ITEMTYPE_BOW:
			case ITEMTYPE_GUN:
			case ITEMTYPE_CANNON:	
				{
					bAddItem = true;
				}
			}
		}
		else
		{
			continue;
		}
		if( false == bAddItem ) continue;

		// ������ �ø� ��ǰ�̸�.
		if( 0 >= pclOrder->siPersonID )
		{
			// ������ �Ĵ� ������ �ü��� �� �� �ü����� ������ �����Ͽ� ����.
			pclprice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, siItemUnique);
			if( NULL == pclprice ) continue;

			// ������ �ü����� �۾����� �ý����� �����Ͽ� �����Ѵ�.
			if( pclOrder->siPrice < pclprice->siPrice )
			{
				sDBRequest_TradeMarket clMsg( 0, 0, false, pclOrder, pclOrder->siIndex, 0, 0, MARKETTRADE_REASON_NORMAL );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
			else
			{
				// ������ �ȵȰ� ������ �߰�.
				++siItemCount[siRef];	// ���� �ֹ��� �� 2 ~ 25 Lv �� ���?������ ���� Ȯ��.
			}
		}
		// ������ �ø� ��ǰ�̸�.
		else
		{
			// ������ �߰�.
			++siItemCount[siRef];	// ���� �ֹ��� �� 2 ~ 25 Lv �� ���?������ ���� Ȯ��.
		}
	}

	SI32			siOrderItemRandNum = 0;
	cltTradeOrder	clOrder; 
	for( SI32 Count = 0; Count < MAX_ITEMINFO_NUMBER; ++Count )
	{
		if( NULL == pclItemManager->pclItemInfo[Count] ) continue;

		if( 0 >= siItemCount[Count] )
		{
			siEquipLevel = pclItemManager->pclItemInfo[Count]->clCondition.siLevel;
			if( 1 >= siEquipLevel || 25 < siEquipLevel ) continue;

			siItemUnique = pclItemManager->pclItemInfo[Count]->clItem.siUnique;
			bAddItem     = false;
			switch(pclItemManager->GetItemType( siItemUnique ))
			{
			case ITEMTYPE_SWORD:
			case ITEMTYPE_SPEAR:
			case ITEMTYPE_STAFF:
			case ITEMTYPE_AXE:
			case ITEMTYPE_BOW:
			case ITEMTYPE_GUN:
			case ITEMTYPE_CANNON:	
				{
					bAddItem = true;	
				}
			}
			if( false == bAddItem ) continue;

			cltItem		clGetItem;
			SI16		brareswitch = 0;
			SI32		usedate;

			pclprice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, siItemUnique );
			if( NULL == pclprice )	continue;

			// �ý����� ���Ͽ� ���?��ų �������� ���� �����ϰ�.. 3~8��.
			usedate			   = pclItemManager->pclItemInfo[Count]->clItem.uiDateUseDay;
			siOrderItemRandNum = 3 + ( rand() % 6 );
			for(SI32 Count = 0; Count < siOrderItemRandNum; ++Count)
			{
				clGetItem.Init(); 
				if( false == pclItemManager->MakeRandItemUnique( siItemUnique, &clGetItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) )
					continue;

				clOrder.Init();
				clOrder.siAmount = 1;
				clOrder.siType   = TRADEORDER_TYPE_OBJECT;
				clOrder.clItemData.Set( &clGetItem );	

				// ������ ���⼭ ��������.
				clOrder.siPrice	= pclprice->siPrice + ( pclprice->siPrice * ( 0.1 * ( 1 + rand()%5 ) ) );

				sDBRequest_OrderMarket	clMsg( 0, ORDERMODE_SELL, 0, &clOrder, 0, 0, 0 );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
	}
}

// ���� ��밡����?����ũ�ι� �ΰ�?
bool cltServer::IsUseFourLeaf( SI32 siItemUnique, cltCharServer* pclchar )	
{
	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		//[����] ����ũ�ι��� ���� ����Ҽ�?���� �Ÿ� ������ ���Ѵ�. => 2008-8-14
		if( ITEMUNIQUE(2755) == siItemUnique || ITEMUNIQUE(2754) == siItemUnique )	
		{	
			if( siItemUnique != siFourLeafUnique )	
			{
				SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, pclchar->GetID() );
				return false;	
			}
		}
	}

	return true;
}

NTCHARString<4> cltServer::GetTradeServer_ServerNation( SI32 ServiceArea )
{
	NTCHARString<4>	kRet;

	switch( ServiceArea )
	{
	case ConstServiceArea_Japan:		{		kRet = "jpn";		}		break;
	case ConstServiceArea_English:		{		kRet = "eng";		}		break;
		//case ConstServiceArea_China:		{		kRet = "chi";		}		break;	// ȯ��ð�?���� �߱�
		//case ConstServiceArea_NHNChina:		{		kRet = "nch";		}		break;	// HANGAME ���� �߱�
		//case ConstServiceArea_Taiwan:		{		kRet = "tai";		}		break;
		//case ConstServiceArea_Korea:		{		kRet = "nkr";		}		break;	// HANGAME ���� Special
		//case ConstServiceArea_USA:			{		kRet = "usa";		}		break;	// HANGAME ���� Special
	}

#ifdef _DEBUG
	kRet = "tgl";
#endif

	return kRet;
}

SI16 cltServer::GetTradeServer_CharNation( cltCharServer* pclchar )
{
	SI16 siRet = -1;
	if( pclchar == NULL )			return siRet;

	switch( siServiceArea )
	{
		//case ConstServiceArea_Korea:	{		siRet = pclCountryManager->GetUniqueFromCountry( "Korea" );		}		break;
	case ConstServiceArea_Japan:	{		siRet = pclCountryManager->GetUniqueFromCountry( "Japan" );		}		break;
		//case ConstServiceArea_Taiwan:	{		siRet = pclCountryManager->GetUniqueFromCountry( "Taiwan" );	}		break;
		//case ConstServiceArea_China:
		//case ConstServiceArea_NHNChina:	{		siRet = pclCountryManager->GetUniqueFromCountry( "China" );		}		break;
	case ConstServiceArea_English:
		{
			siRet = pclchar->pclCI->clBI.siCountryID;
			if( siRet == -1)	siRet = 2;		// 2 ~ 9 ������ �� �����̴�.
		}
		break;
	}

#ifdef _DEBUG
	siRet = pclCountryManager->GetUniqueFromCountry( "United States" );
#endif

	return siRet;
}
// ����ã�� �̺�Ʈ
void cltServer::Write_Kill_NPC_Num( SI32 mapindex )		// ������ ���� �����?npc ī��Ʈ �Ѵ� 
{
	for ( SI32 slot = 0 ; slot<MAX_TREASUREEVENT_USEMAP_NUM; slot++)
	{
		if( mapindex == pclTreasureEvent_Npc_KillCounnt[slot].GetMapIndex()	)
		{
			pclTreasureEvent_Npc_KillCounnt[slot].AddKillCount()	;

		}
	}


}
void cltServer::Respon_TreasureEvent_Npc(  )
{
	// ���� npc ��ŭ �ٽ� ������ �Ѵ�.
	SI32 siMonsterKind	=	pclClient->GetUniqueFromHash(TEXT("KIND_ANTIQUITY"))	;// ��ȯ�� �༮

	for ( SI32 Slot_Mapindex = 0 ; Slot_Mapindex < MAX_TREASUREEVENT_USEMAP_NUM; Slot_Mapindex++)
	{
		if( Slot_Mapindex < 0  || Slot_Mapindex > MAX_TREASUREEVENT_USEMAP_NUM )	break;

		SI32 siMapIndex = pclTreasureMapInfo[Slot_Mapindex].GetMapIndex( )	; //���� ���ε����� �̾� �´�.
		if( siMapIndex <= 0)			continue	;

		SI32 siMax_MakeNpcNum	=	pclTreasureMapInfo[Slot_Mapindex].Get_MaxUse_RocNum(); // �ش�ʿ�?�����?�ִ� �ִ��� npc ����
		if( siMax_MakeNpcNum <= 0)		continue	;

		SI32 siHave_Roc_This_Map	=	pclTreasureMapInfo[Slot_Mapindex ].GetRocNum( )	;	//�ش� ���� ���� ��ǥ�� ��
		if( siHave_Roc_This_Map <= 0 )	continue	;	// ��ǥ�� ������ ����

		SI32 siResponwnNum = pclTreasureEvent_Npc_KillCounnt[Slot_Mapindex].Get_KillCount()	;	// ���� �������� npc ����
		if( siResponwnNum <= 0 )	continue	;

		SI32 siUse_MakeNpcSlotNumber = MakeMapRocSlot( Slot_Mapindex , siResponwnNum )	;

		if (siUse_MakeNpcSlotNumber <= 0 )	continue	;

		if ( siUse_MakeNpcSlotNumber >= MAX_TREASUREEVENT_POSITION_NUM )
		{
			siUse_MakeNpcSlotNumber = MAX_TREASUREEVENT_POSITION_NUM	;
		}

		for ( SI32 Slot_roc = 0; Slot_roc <= siUse_MakeNpcSlotNumber ; Slot_roc++)
		{
			if( Slot_roc < 0  || Slot_roc > MAX_TREASUREEVENT_POSITION_NUM )	break;
			if( Slot_Mapindex < 0  || Slot_Mapindex > MAX_TREASUREEVENT_USEMAP_NUM )	break;

			if(pclTreasureMapUseRoc[Slot_roc]	== NULL)	break	;

			SI32 siRocX = pclTreasureMapInfo[Slot_Mapindex].GetRoc_X( pclTreasureMapUseRoc[ Slot_roc] )	;
			SI32 siRocY = pclTreasureMapInfo[Slot_Mapindex].GetRoc_Y( pclTreasureMapUseRoc[ Slot_roc ] )	;
			cltMapCommon* pclmap	= pclMapManager->GetMapPointer( siMapIndex );
			// �ش� ��ǥ���� ����ġ�� ��´�?

			SI32 siX, siY	;	//��������ڸ�?

			pclmap->FindEmptyAreaForCharLocate( siMonsterKind , siRocX, siRocY, 10, &siX, &siY, false )	;

			SI32 id = SetNPCChar( siMonsterKind, VILLAGE_NONE, NULL, 1, 1000, siX, siY, siMapIndex, NPCITEMMODE_NORMAL, NULL );
		}

		pclTreasureEvent_Npc_KillCounnt[Slot_Mapindex].Set_KillCount(0);	// �� �־����� ���� ���� �ʱ�ȭ
	}			
}
void cltServer::Read_TreasureEvent_MapInfo_TheQ()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/TreasureEvent_RocInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/TreasureEvent_RocInfo.txt") );
	}
	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile test.txt"),TEXT("Error"));
		return;
	}
	SI32 siMapIndex	=	0	, siMapRoc_X	=	0,	siMapRoc_Y	=	0	,pMax_Create_Num = 0 ;
	TCHAR mapname[256] = TEXT("");

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siMapIndex,	4,
			NDATA_INT32,	&siMapRoc_X,	4,
			NDATA_INT32,	&siMapRoc_Y,	4,
			NDATA_INT32,	&pMax_Create_Num,	4,
			NDATA_MBSTRING,	mapname,	256,
			0,0,0
	};	
	dataloader.RegDataType( pDataTypeInfo );

	SI16 index		=	0	;
	SI16 RocCount	=	0	;
	SI16 siNumindex		=	0	;
	SI32 siSlot = 0	;
	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			SI32  Mapindex = pclTreasureMapInfo[index].GetMapIndex()	;

			if ( Mapindex == 0 && Mapindex != siMapIndex )	// �ƹ��͵� ������ �� ó��
			{
				if ( index > MAX_TREASUREEVENT_USEMAP_NUM || index < 0)	return	;
				if ( siMapIndex < 0)	return	;
				if( RocCount >= MAX_TREASUREEVENT_POSITION_NUM)	continue	;
				pclTreasureMapInfo[index].SetMapIndex(siMapIndex)	;
				pclTreasureEvent_Npc_KillCounnt[index].SetMapIndex(siMapIndex)	;
				pclTreasureMapInfo[index].Set_MaxUse_RocNum(pMax_Create_Num)	;
				pclTreasureMapInfo[index].MakeRocSlot( )	;
				pclTreasureMapInfo[index].SetRoc( siMapRoc_X,  siMapRoc_Y , siSlot )	;
				pclTreasureMapInfo[index].SetRocNum( RocCount++)	;
			}
			else if( Mapindex > 0 && Mapindex == siMapIndex )
			{
				if ( index > MAX_TREASUREEVENT_USEMAP_NUM || index < 0)	return	;
				if( RocCount >= MAX_TREASUREEVENT_POSITION_NUM)	continue	;
				siSlot++; // ���� ���Կ� ��ǥ�� �ִ´� 
				pclTreasureMapInfo[index].SetRoc( siMapRoc_X,  siMapRoc_Y , siSlot )	;
				pclTreasureMapInfo[index].SetRocNum( RocCount++)	;


			}
			else	// ó���� �ƴϰ� ���� �ε����� �ƴϸ� ���� �ε�����.
			{
				if ( index > MAX_TREASUREEVENT_USEMAP_NUM || index < 0)	return	;
				if ( siMapIndex < 0)	return	;
				// ���ݱ����� ī��Ʈ �� ��ǥ�� ���?�Ѵ�.

				//Roc���?������ �ʱ�ȭ 
				RocCount = 0	;
				siSlot	=	0	;
				index++;
				// ���?�Ѵ�.
				pclTreasureMapInfo[index].SetMapIndex(siMapIndex)	;
				pclTreasureEvent_Npc_KillCounnt[index].SetMapIndex(siMapIndex)	;
				pclTreasureMapInfo[index].Set_MaxUse_RocNum(pMax_Create_Num)	;
				pclTreasureMapInfo[index].MakeRocSlot()	;// �޸𸮸� ��������
				pclTreasureMapInfo[index].SetRoc( siMapRoc_X,  siMapRoc_Y , siSlot )	;
				pclTreasureMapInfo[index].SetRocNum( RocCount++)	;
				// �ٽ� ���?����.
			}

		}
	}

}
SI32 cltServer::MakeMapRocSlot( SI32 siMapindexSlot , SI32 MakeNpcNum )	// �������� ���ε��� ���԰� npc�� ������ �޾� �´�.//���ϰ��� ���� ����
{
	SI32 siSlot			=	0	;
	SI32 siUse_RocSlot	=	0	;
	SI32 siIndex		=	0	;	 // ���� �ε��� 
	SI32 MakeNpcSlotNum	=	0	;	// ������ �����?����.

	// �����?�ʱ�ȭ 
	ZeroMemory( pclTreasureMapUseRoc , sizeof( pclTreasureMapUseRoc) )	;									//0���� �� ä�� �ִ´�.
	if( siMapindexSlot < 0 || siMapindexSlot > MAX_TREASUREEVENT_USEMAP_NUM )
	{
		return	MakeNpcSlotNum	;
	}

	SI32 siHave_Roc_This_Map	=	pclTreasureMapInfo[siMapindexSlot ].GetRocNum( )	;					//�ش� ���� ���� ��ǥ�� ��
	if( siHave_Roc_This_Map <= 0 ) 
	{
		return	MakeNpcSlotNum	;
	}
	SI32 siMaxSlotNum = pclTreasureMapInfo[siMapindexSlot ].GetRocNum()	;
	if( siMaxSlotNum <= 0 || siMaxSlotNum > MAX_TREASUREEVENT_POSITION_NUM ) 
	{
		return	MakeNpcSlotNum	;
	}

	// ��ǥ�� ������ ����
	// while ���� ��¥�� Ƚ�� ������ �аŴ� for������ ���� 
	for( SI32 siRndMakeNum = 0;	siRndMakeNum < MAX_RANDOM_ROC_COUNT;	siRndMakeNum++ )
	{
		SI32 rnd_Slot	=	rand()%siHave_Roc_This_Map	;	// ���� �ϰ� ���� ������ ����
		if ( siMaxSlotNum < rnd_Slot )	continue	;
		bool bIsSame	=	false ;
		SI32 CheckCount	=	0	;

		for (SI32 i = 0; i < MAX_TREASUREEVENT_POSITION_NUM; i++  ) 
		{
			// �迭�� ������ �ֳ� Ȯ�� 
			if ( pclTreasureMapUseRoc[ i ] ==  rnd_Slot) 
			{
				bIsSame	=	true	;	
			}
		}
		if ( bIsSame == false )		// ������ �������?�������?����
		{
			if ( siIndex < MAX_TREASUREEVENT_POSITION_NUM )
				if( pclTreasureMapInfo[siMapindexSlot ].GetRoc_Y(rnd_Slot) <= 0 ) continue	;
			pclTreasureMapUseRoc[ siIndex ]	=	rnd_Slot	;
			siIndex++;
			{
				if( pclTreasureMapInfo[siMapindexSlot ].GetRoc_X(rnd_Slot) <= 0 ) continue	;
				MakeNpcSlotNum++;
			}
		}
		if (MakeNpcSlotNum == MakeNpcNum )	break ;// ������ �� ������ŭ ���������?���°� ����
	}
	return MakeNpcSlotNum	;
}
// ����
//���� �������� �߰� �Ѵ�
void cltServer::AddFamily( SI32 siParent_PersonID, SI32 siChildren_PersonID, SYSTEMTIME stFamilyEffectTime )
{
	if(IsCountrySwitch( (Switch_We_Are_Family)  )	== false	)	return	;
	// PersonID�� �������� ���ؼ� ���?parent�� �������� id�� �̴���.
	SI32 siParent_ID = pclCM->GetIDFromPersonID( siParent_PersonID )	;
	cltCharServer* pBaseParentchar = pclCM->GetCharServer( siParent_ID)	;	// ���������� ������ �� �θ� �ɸ���

	SI32 siChildren_ID = pclCM->GetIDFromPersonID( siChildren_PersonID )	;	if ( pclCM->IsValidID(siChildren_ID) == false)	return	;
	cltCharServer* pChildrenChar = pclCM->GetCharServer( siChildren_ID)		;	if( pChildrenChar == NULL )	return;

	//  �����?����
	SI32 siMatePersonID = pBaseParentchar->pclMarriage->GetMatePersonID() ;		if ( siMatePersonID <= 0 )	return	;
	SI32 siMate_ID = pclCM->GetIDFromPersonID( siMatePersonID )	;				if ( pclCM->IsValidID( siMate_ID) == false )	return	;
	cltCharServer* pMateChar = pclCM->GetCharServer( siMate_ID)	;				if(pMateChar == NULL )	return	;

	TCHAR* ParentName_1	=	(TCHAR*)pBaseParentchar->GetName()	;
	TCHAR* ParentName_2	=	(TCHAR*)pMateChar->GetName()	;
	TCHAR* AddName		=	(TCHAR*)pChildrenChar->GetName()	;

	cltLetterMsg_ADDFamilyLetter_ToFamily clletter1( ParentName_1,ParentName_2,AddName);
	SendLetterMsg(siParent_PersonID, (cltLetterHeader*)&clletter1);
	SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_FAMILY, 0, 0, pBaseParentchar->GetCharUnique() );	// �������� ����ߴ�?

	cltLetterMsg_ADDFamilyLetter_ToFamily clletter2( ParentName_1,ParentName_2,AddName );
	SendLetterMsg(siMatePersonID, (cltLetterHeader*)&clletter2);
	SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_FAMILY, 0, 0, pMateChar->GetCharUnique() );			// �������� ����ߴ�?

	cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me clletter3( ParentName_1,ParentName_2,AddName);
	SendLetterMsg(siChildren_PersonID, (cltLetterHeader*)&clletter3);
	SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_FAMILY, 0, 0, pChildrenChar->GetCharUnique());		// �������� ����ߴ�?

	for (SI32 childrenSlot = 0 ; childrenSlot< MAX_CHILDREN_NUM; childrenSlot++ )
	{
		SI32 siChildrenPersonID_inFamily = pBaseParentchar->pclFamily->GetChildPersonID( childrenSlot );
		if(siChildrenPersonID_inFamily <= 0 )	continue	;

		SI32 childrenID = pclCM->GetIDFromPersonID(siChildrenPersonID_inFamily )	;
		cltCharServer* pOtherChildrenChar = pclCM->GetCharServer( childrenID )	;
		if ( pOtherChildrenChar == NULL ) // �������ڿ��Դ� �׳� ������ �����ش�
		{
			cltLetterMsg_ADDFamilyLetter_ToFamily clletter( ParentName_1,ParentName_2,AddName);
			SendLetterMsg(siChildrenPersonID_inFamily, (cltLetterHeader*)&clletter);
		}
		else	// ������
		{
			cltLetterMsg_ADDFamilyLetter_ToFamily clletter( ParentName_1,ParentName_2,AddName);
			SendLetterMsg(siChildrenPersonID_inFamily, (cltLetterHeader*)&clletter);
			SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_FAMILY, 0, 0, pOtherChildrenChar->GetCharUnique() );			// �������� ����ߴ�?
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------
}
//���� �������� ���� �Ѵ�
void cltServer::DelFamily( SI32 siDelTargetPersonID )
{
	if(IsCountrySwitch( (Switch_We_Are_Family)  )	== false	)	return	;
	// ������ �ɸ����� ���� ������ ��´�?
	SI32 siParent_ID = pclCM->GetIDFromPersonID( siDelTargetPersonID )	;
	cltCharServer* pDelTargetchar = pclCM->GetCharServer( siParent_ID)	; // ������ �ɸ��� 

	CFamily FamilyData ;
	FamilyData.Set( pDelTargetchar->pclFamily)	; // �ٸ� �����鿡�� ���� ���ֱ� ���� �� ���� ������ �޴´�.
	FamilyData.DelFamilyMember(siDelTargetPersonID)	;// �ش� ��ǥ�� �ο��� �����?
	pDelTargetchar->pclFamily->Init()	; // �ش� �ɸ����� ���������� ���� �Ѵ�.

	//���������?Ŭ���̾�Ʈ�� �ʱ�ȭ�� �Ѵ�.
	cltGameResponse_UpdateFamily clinfo( pDelTargetchar->pclFamily,pDelTargetchar->pclMarriage );
	cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
	pDelTargetchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );

	for ( SI32 siSlot = 0 ; siSlot<MAX_PARENT_NUM; siSlot++) 
	{
		// �θ��ɸ��Ϳ� ������ �ִ� ������������ ���� �Ѵ�.
		SI32 siParent_PersonID = FamilyData.GetParent_PersonId( siSlot )	;
		SI32 siParentID = pclCM->GetIDFromPersonID( siParent_PersonID)	;		if(pclCM->IsValidID( siParentID) == false )	continue	;

		cltCharServer* pclParentchar = pclCM->GetCharServer( siParentID )	;	if(pclParentchar==NULL)	continue	;
		pclParentchar->pclFamily->DelFamilyMember( siDelTargetPersonID )	;
		pclParentchar->pclFamily->LoseOnParentNum();

		//���������?Ŭ���̾�Ʈ�� �ʱ�ȭ�� �Ѵ�.
		cltGameResponse_UpdateFamily clinfo( pclParentchar->pclFamily ,pclParentchar->pclMarriage);
		cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
		pclParentchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );

	}
	for (SI32 childrenSlot = 0 ; childrenSlot< MAX_CHILDREN_NUM; childrenSlot++ )
	{
		// �ڽ��ɸ��� ���� ���� �������� �ش� �ɸ��͸� ���� �Ѵ�.
		SI32 sichildren_PersonID = FamilyData.GetChildPersonID( siSlot )	;
		SI32 sichildrenID = pclCM->GetIDFromPersonID( sichildren_PersonID)	;		if(pclCM->IsValidID( sichildrenID) == false )	continue	;

		cltCharServer* pclchildrenchar = pclCM->GetCharServer( sichildrenID )	;	if(pclchildrenchar==NULL)	continue	;
		pclchildrenchar->pclFamily->DelFamilyMember( siDelTargetPersonID )	;
		pclchildrenchar->pclFamily->LoseOnChildrenNum();

		//���������?Ŭ���̾�Ʈ�� �ʱ�ȭ�� �Ѵ�.
		cltGameResponse_UpdateFamily clinfo( pclchildrenchar->pclFamily,pclchildrenchar->pclMarriage );
		cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
		pclchildrenchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );
	}
}
// �������� ������ ������Ʈ �Ѵ�.
void cltServer:: UpdateFamily( CFamily* pclPamily,SI32 siPersonID  )
{
	// ���⼭ personID�� �޼����� ���� ����̴�? �̰� ������ ������ �������� �����ֱ� ���ؼ� ����.
	if(IsCountrySwitch( (Switch_We_Are_Family)  )	== false	)	return	;
	if ( NULL == pclPamily )	return;

	// ��񿡼�?���� ������ �̿��ؼ� ���� ���� ������ ������Ʈ �Ѵ�.
	CFamily FamilyData ;
	FamilyData.Init()	;
	FamilyData.Set( pclPamily );

	// ���� ���� ������ ���� ������ �߰� �Ѵ�.
	if( FamilyData.GetParent_1() > 0 || FamilyData.GetParent_2() > 0)
	{
		for ( SI32 siSlot = 0 ; siSlot < MAX_PARENT_NUM ; siSlot++ ) 
		{
			SI32 siFamilyPersonID = FamilyData.GetParent_PersonId( siSlot )	;
			SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
			if ( pclCM->IsValidID( siFamilyId ) == true )
			{
				// ���̵� ��ȿ�ϴٸ� ���� ���ִ� ���̴�.
				FamilyData.ConnectIn( siFamilyPersonID , MYCLASS_IN_FAMILY_PARANTS )	;	// ������������ ���?�Ѵ�.
			}
		}
		for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ ) 
		{
			SI32 siFamilyPersonID = FamilyData.GetChildPersonID( siSlot)	;
			SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
			if ( pclCM->IsValidID( siFamilyId ) == true  )
			{
				// ���̵� ��ȿ�ϴٸ� ���� ���ִ� ���̴�.
				FamilyData.ConnectIn( siFamilyPersonID , MYCLASS_IN_FAMILY_CHILDREN )	;	// ������������ ���?�Ѵ�.
			}
		}
		// ������ ���� ������ ���?���� ���ִ� ����鿡��?�����ش�.

		for ( SI32 siSlot = 0 ; siSlot < MAX_PARENT_NUM ; siSlot++ ) 
		{
			SI32 siFamilyPersonID = FamilyData.GetParent_PersonId( siSlot )	;
			SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
			if ( pclCM->IsValidID( siFamilyId ) == false )	continue ;	//���̵��� ��ȿ�� �˻�.
			cltCharServer* pclChar = pclCM->GetCharServer( siFamilyId )	;
			FamilyData.SetMyClass_InFamily( MYCLASS_IN_FAMILY_PARANTS )	;
			pclChar->pclFamily->Set(&FamilyData)	;
			// Ŭ���̾�Ʈ�� �ش� �ɸ����� ���� ������Ʈ
			cltGameResponse_UpdateFamily clInfo( &FamilyData ,pclChar->pclMarriage);
			cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clInfo), (BYTE*)&clInfo );
			pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
		}
		for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ ) 
		{
			SI32 siFamilyPersonID = FamilyData.GetChildPersonID( siSlot)	;
			SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
			if ( pclCM->IsValidID( siFamilyId ) == false )	continue ;	//���̵��� ��ȿ�� �˻�.
			cltCharServer* pclChar = pclCM->GetCharServer( siFamilyId )	;
			FamilyData.SetMyClass_InFamily( MYCLASS_IN_FAMILY_CHILDREN )	;
			pclChar->pclFamily->Set(&FamilyData)	;

			// Ŭ���̾�Ʈ�� �ش� �ɸ����� ���� ������Ʈ
			cltGameResponse_UpdateFamily clInfo( &FamilyData ,pclChar->pclMarriage);
			cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clInfo), (BYTE*)&clInfo );
			pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
		}
	}	
	else	// ������ ���� ����̴�?
	{
		// del �϶��� personID�� ��񿡼�?�ȹ޾� �´�. �׸��� �������� �ٷ� �ʱ�ȭ ���ֱ� ������ �̰����� ���� �ʴ´�.
		if ( siPersonID <= 0 )	return	;
		SI32 siNoFamilyPersonId = pclCM->GetIDFromPersonID( siPersonID )	;
		if ( pclCM->IsValidID( siNoFamilyPersonId ) == false )	return ;	//���̵��� ��ȿ�� �˻�.
		cltCharServer* pclChar = pclCM->GetCharServer( siNoFamilyPersonId )	;
		if ( pclChar != NULL )
		{
			pclChar->pclFamily->Set(&FamilyData)	;

			// Ŭ���̾�Ʈ�� �ش� �ɸ����� ���� ������Ʈ
			cltGameResponse_UpdateFamily clInfo( &FamilyData ,pclChar->pclMarriage);
			cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clInfo), (BYTE*)&clInfo );
			pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
		}		


	}

}
void cltServer::SetGachaRareItem_Time()
{
	// �ϴ� ���� ó���� ���� �⺻ ��ƾ .
	SI32 siChane_ItemUnique	=	0	;
	// �ش� Ÿ�ӻ��̸� �ش� ���������� ���� �������� ���� �Ѵ�. 

	if(pclClient->pclGachaManager == NULL )										return	;
	if(pclClient->pclGachaManager->m_GacharareTimePeriod[0].ItemUnique <= 0 )	return	;
	if(pclClient->pclGachaManager->m_GachaProduct[0][0].ItemUnique <= 0 )		return	;

	if(siGMChangeGachaRareUnique> 0 )	// GM �� ���� �ߵ�. ������ �ش� ����ũ�� ���δ� .
	{
		if( siGMChangeGachaRareUnique != 0 && siGMChangeGachaRareUnique > 0 )
		{
			// �Ϲ� ��í ���� ������ ����ũ�� ���� �Ѵ�. 
			// �� ���� �ְ� ��ǰ�� ���� .[Ű][������]
			pclClient->pclGachaManager->m_GachaProduct[0][0].ItemUnique = siGMChangeGachaRareUnique	;	// ���?
			pclClient->pclGachaManager->m_GachaProduct[1][0].ItemUnique = siGMChangeGachaRareUnique	;	// �ǹ�
			pclClient->pclGachaManager->m_GachaProduct[2][0].ItemUnique = siGMChangeGachaRareUnique	;	// �����?
		}

	}
	else
	{
		for( SI32 siSlot = 0 ; siSlot < MAX_CHANGERARE_PRODUCT; siSlot++)
		{
			if(	siSlot >= MAX_CHANGERARE_PRODUCT  )	break	;
			SYSTEMTIME CurrentTime	;
			SYSTEMTIME stStartTime = pclClient->pclGachaManager->m_GacharareTimePeriod[siSlot].stStartTime	;	// �ش� ���������� ������ ���� �ð�
			SYSTEMTIME stEndTime = pclClient->pclGachaManager->m_GacharareTimePeriod[siSlot].stEndTime	;	// �ش� ���������� ������ ���� �ð�

			GetLocalTime(&CurrentTime);
			bool bInTime = TimeInBounds( &CurrentTime,&stStartTime,&stEndTime )	;
			if( bInTime== true )
			{
				siChane_ItemUnique = pclClient->pclGachaManager->m_GacharareTimePeriod[siSlot].ItemUnique	;
				if( siChane_ItemUnique != 0 && siChane_ItemUnique > 0 )
				{
					// �Ϲ� ��í ���� ������ ����ũ�� ���� �Ѵ�. 
					// �� ���� �ְ� ��ǰ�� ���� .[Ű][������]
					pclClient->pclGachaManager->m_GachaProduct[0][0].ItemUnique = siChane_ItemUnique	;	// ���?
					pclClient->pclGachaManager->m_GachaProduct[1][0].ItemUnique = siChane_ItemUnique	;	// �ǹ�
					pclClient->pclGachaManager->m_GachaProduct[2][0].ItemUnique = siChane_ItemUnique	;	// �����?
				}	break	;			
			}
		}
	}
}

void cltServer::AddMagicAction( SI32 _siID, SI32 _siTargetID, SI32 _siMagicKind, SI32 siDamage )
{
	// ��ų�� ����
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer( _siID );
	if( NULL == pclChar ) return;

	if( false == pclChar->pclCharSkillBookInfo->IsSkillBookMagic() ) return;

	cltCharServer* pclTargetChar = pclClient->pclCM->GetCharServer( _siTargetID );
	if( NULL == pclTargetChar ) return;

	cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
	if( NULL == pclSkillBookMagic ) return;

	if( pclSkillBookMagic->GetMagicKind() == _siMagicKind ) 
	{
		stMagicAttributeData* pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttributePtr()->GetMagicAttribute( _siMagicKind );
		if( NULL == pMagicAttributeData ) return;

		switch( pMagicAttributeData->siAddMagic )
		{
		case ADDMAGIC_CURSE_POISON:	
			{
				pclTargetChar->SetPoison( _siID, siDamage, pMagicAttributeData->dwAddMagicDuration );
			}
			break;
		case ADDMAGIC_CURSE_ATTACK:	
			{
				pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_ATTACK, 
					pMagicAttributeData->dwAddMagicDuration,
					pMagicAttributeData->siAddMagicValue,
					pclTargetChar->GetID() );
			}
			break;
		case ADDMAGIC_CURSE_DEFENSE:	
			{
				pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_DEFENSE, 
					pMagicAttributeData->dwAddMagicDuration,
					pMagicAttributeData->siAddMagicValue,
					pclTargetChar->GetID() 	);
			}
			break;
		case ADDMAGIC_CURSE_DODGE:	
			{
				pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_DODGE, 
					pMagicAttributeData->dwAddMagicDuration,
					pMagicAttributeData->siAddMagicValue,
					pclTargetChar->GetID()  );
			}
			break;
		case ADDMAGIC_CURSE_HIT:	
			{
				pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_HIT, 
					pMagicAttributeData->dwAddMagicDuration,
					pMagicAttributeData->siAddMagicValue,
					pclTargetChar->GetID()  );
			}
			break;
		case ADDMAGIC_CURSE_ICE:	
			{
				pclTargetChar->SetIce( siDamage, pMagicAttributeData->dwAddMagicDuration );
			}
			break;
			
		}

		pclTargetChar->ParametaBoxAction();
	}
}

void cltServer::SubMagicAction( SI32 _siID, SI32 _siTargetID, SI32 _siParentMagicKind, SI32 siDamage )
{
	// ��ų�� ����
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer( _siID );
	if( NULL == pclChar ) return;

	cltCharServer* pclTargetChar = pclClient->pclCM->GetCharServer( _siTargetID );
	if( NULL == pclTargetChar ) return;

	cltSkillBookSubMagic* pclSkillBookSubMagic = pclChar->pclCharSkillBookInfo->FindSkillBookSubMagicFromParentMagic( _siParentMagicKind );
	if( NULL == pclSkillBookSubMagic ) return;

	stMagicAttributeData* pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttributePtr()->GetMagicAttribute( pclSkillBookSubMagic->GetMagicKind() );
	if( NULL == pMagicAttributeData ) return;

	switch( pMagicAttributeData->siAddMagic )
	{
	case ADDMAGIC_CURSE_POISON:	
		{
			pclTargetChar->SetPoison( _siID, siDamage, pMagicAttributeData->dwAddMagicDuration );
		}
		break;
	case ADDMAGIC_CURSE_ATTACK:	
		{
			pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_ATTACK, 
				pMagicAttributeData->dwAddMagicDuration,
				pMagicAttributeData->siAddMagicValue,
				pclTargetChar->GetID() );
		}
		break;
	case ADDMAGIC_CURSE_DEFENSE:	
		{
			pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_DEFENSE, 
				pMagicAttributeData->dwAddMagicDuration,
				pMagicAttributeData->siAddMagicValue,
				pclTargetChar->GetID() 	);
		}
		break;
	case ADDMAGIC_CURSE_DODGE:	
		{
			pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_DODGE, 
				pMagicAttributeData->dwAddMagicDuration,
				pMagicAttributeData->siAddMagicValue,
				pclTargetChar->GetID()  );
		}
		break;
	case ADDMAGIC_CURSE_HIT:	
		{
			pclTargetChar->m_clCharCurseEffect.SetCurse(	cltCharCurseEffect::CURSE_HIT, 
				pMagicAttributeData->dwAddMagicDuration,
				pMagicAttributeData->siAddMagicValue,
				pclTargetChar->GetID()  );
		}
		break;
	case ADDMAGIC_CURSE_ICE:	
		{
			pclTargetChar->SetIce( siDamage, pMagicAttributeData->dwAddMagicDuration );
		}
		break;
	}

	pclTargetChar->ParametaBoxAction();

}