//---------------------------------
// 2003/5/17 ���°�
//---------------------------------

// ������ Ŭ���̾�Ʈ ��� �������� ����ϴ� Define

#ifndef _COMMONHEADER_H
#define _COMMONHEADER_H

#include <windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <io.h>
#include <wingdi.h>
#include <math.h>
#include <commctrl.h>
#include <crtdbg.h>


#include "../lib/Directives.h"


#include "../lib/commonutil.h"
#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../../NLib/NDataTypes.h"
#include "../../NLib/NTCHARString.h"
#include "../../NLib/NArray.h"

#include "../../NLib/NProfile.h"
#include "../../NLib/NBitArray.h"

#include "../GMSystem/GMSectionData.h"

#include "../../NLib/NLua.h"

#include "../../NStatistics/NStatistics.h"

#include "../../NLib/NTech/sdk/nkcore/Include/NkCore.h"
#include "../../NLib/NTech/sdk/nkcore/Include/NkVector.h"
#include "../../NLib/NTech/sdk/nkcore/Include/NkRbTreeMap.h"


#if defined(GOONZU)
	#define NKCOMMON_DEBUG_LIB		"../NLib/NTech/sdk/nkcore/lib/debug/NkCommon.lib"
	#define NKCOMMON_RELEASE_LIB	"../NLib/NTech/sdk/nkcore/lib/release/NkCommon.lib"
#else
	#define NKCOMMON_DEBUG_LIB		"../../NLib/NTech/sdk/nkcore/lib/debug/NkCommon.lib"
	#define NKCOMMON_RELEASE_LIB	"../../NLib/NTech/sdk/nkcore/lib/release/NkCommon.lib"
#endif


#ifdef _DEBUG
	#pragma comment(lib, NKCOMMON_DEBUG_LIB)
#else
	#pragma comment(lib, NKCOMMON_RELEASE_LIB)
#endif

class cGMClient;

//KHY - 0318 - NHNChina�� �ϴ� 100���� �����̴�.
#if _NHN_CHINA 
	#define MAX_EXPTABLE_LEVEL					100			// ����ġ ���̺��� �ְ� �ܰ�
	#define MAX_CHARACTER_LEVEL					100			// ĳ���� ������ �ܰ�. 
#else
	#define MAX_EXPTABLE_LEVEL					120			// ����ġ ���̺��� �ְ� �ܰ�
	#define MAX_CHARACTER_LEVEL					120			// ĳ���� ������ �ܰ�. 
#endif

#define MAX_GUILD_LEVEL						100			// ��� ������ �ܰ�. 
#define MAX_FAME_LEVEL						100			// ��������� �ܰ�.

#define MAX_MACROACTION_SAMENAME_COUNT		1000		// ��ũ�� �׼��� ������ �ִ� 1000�� ���� ���� �ɸ� �������� ������ ���� �Ҽ� �ִ� .

typedef NSafeTArray< NSafeTArray<SI64, 3>, MAX_EXPTABLE_LEVEL >		ExpTable;
typedef NSafeTArray< NSafeTArray<SI64, 3>, MAX_FAME_LEVEL >			FameExpTable;

#define MAXORG(level)	(25 + (level) / 2)			// ������ ���� ����� �ִ� ��� ����. 

//#define NAME  "����"
//#define TITLE "����"

#define ITEMUNIQUE(a)			(a)
#define FOUNIQUE(a)				(a)
#define QUESTUNIQUE(a)			(a)

//-------------------------------------------------------------
// ���� ���� ( 16������ ó���ؼ� ������ �����ϰ� �Ѵ� )
//-------------------------------------------------------------
const SI32 ConstServiceArea_Invalid		= 0x80000000;

const SI32 ConstServiceArea_None		= 0x00000000;
const SI32 ConstServiceArea_Korea		= 0x00000001;
const SI32 ConstServiceArea_China		= 0x00000002;
const SI32 ConstServiceArea_Japan		= 0x00000004;	
const SI32 ConstServiceArea_English		= 0x00000008;
const SI32 ConstServiceArea_Taiwan		= 0x00000010;
const SI32 ConstServiceArea_USA			= 0x00000020;
const SI32 ConstServiceArea_NHNChina	= 0x00000040;
const SI32 ConstServiceArea_EUROPE		= 0x00000080;
const SI32 ConstServiceArea_DEV			= 0x00000100;

const SI32 ConstServiceArea_All			= 0x7FFFFFFF;

//-------------------------------------------------------------
// ���� ����ġ - Started by LEEKH
//-------------------------------------------------------------
// Server data files are shipped as PLAINTEXT, so the English service area must NOT
// take the LoadDataFromCryptFile() path. Leaving English in this mask makes every
// NDataLoader silently parse 0 rows (empty pclKI[], missing hash-table uniques)
// because plaintext bytes are fed through the crypt reader.
const SI32 ConstSwitch_TextEncryption	= ConstServiceArea_Japan | ConstServiceArea_Taiwan | ConstServiceArea_China | ConstServiceArea_Korea | ConstServiceArea_USA | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE;	//KHY - 0809 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
const SI32 ConstSwitch_NewLog			= ConstServiceArea_Taiwan | ConstServiceArea_China | ConstServiceArea_Korea | ConstServiceArea_English | ConstServiceArea_USA | ConstServiceArea_Japan | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE;	// ConstServiceArea_All

enum SWITCH_COUNTRY
{
	Switch_First						,
	//==================================================

	Switch_AddCapitalRemove				,
	Switch_ChatBalloonLine				,
	Switch_EncryptTxtData				,
	Switch_SummonEnolveVillageMoney		,
	Switch_EnchantNoSealing				,
	Switch_DateOverItemNoDestroy		,
	Switch_ManufactureRevitalize		,
	Switch_MakePerfectItem				,
	Switch_DoNotFailedMakeItem			,
	Switch_NotScailed					,
	Switch_ShowBuffToolTip				,
	Switch_WENNYDISASSEMBLELEVEL		,
	Switch_WENNYBOXCHECKLEVEL			,
	Switch_Event_Hunt					,
	Switch_Event_MofuMofu				,
	Switch_ToolTipAutoLine				,
	Switch_DisplayLevelInPannel			,
	Switch_LevelUpAction				,
	Switch_NamingQuest					,
	Switch_RecommendPointPerLevel		,
	Switch_MagicNeedDex					,
	Switch_BaekNyeonSam					,
	Switch_FreshManHelpDialog			,
	Switch_DalilyQuestGrayList			,
	Switch_GachaAvatarCorrect			,
	Switch_Help							,
	Switch_EnableGuildWar				,
	Switch_EnableVillageWar				,
	Switch_StatusBug					,
	Switch_GuildstatusBugFix			,
	Switch_reply						,
	Switch_HappyBeanQuest				,
	Switch_Sound						,
	Switch_UseWebPageItemMall			,
	Switch_ArchangelArchdevil			,
	Switch_AuctionEvent					,
	Switch_GameRoomCouponEvent			,
	Switch_ServerListArray				,
	Switch_WENNYBOXDisable				,
	Switch_WENNYDISASSEMBLEAvata		,
	Switch_HorseAbility					,
	Switch_DyingDelay					,
	Switch_DeleteLuckButton				,
	Switch_DeleteHandButton				,
	Switch_GoStopPokerEvent				,
	Switch_AutoHuntingReport			,
	Switch_MaterialBox					,
	Switch_TeacherLevelReward			,
	Switch_KingGuriEvent				,
	Switch_Health						,
	Switch_FieldObjectPreLoad			,
	Switch_ItemmallNotDisassemble		,
	Switch_GodHandItem					,
	Switch_NewHitEffect					,
	Switch_Fairy						,
	Switch_LevelupUpText				,
	Switch_BossWildBoar					,
	Switch_DonateDlg					,
	Switch_Button6HozoDlg				,
	Switch_20levelparty					,
	Switch_Can5Horse					,
	Switch_DropBookBox					,
	Switch_WennyOldMan_CityhallProfitMoney	,
	Switch_HanGameVIPEvent				,
	Switch_NowComEvent					,
	Switch_RepairMoney					,
	Switch_Thanksgiving_day				,
	Switch_LotteryStockRandom			,
	Switch_PreDeleteChar				,
	Switch_ChristmasEvent				,
	Switch_NoCheckMapIndex_Quest		,
	Switch_WarpInHuntMap				,
	Switch_SaveHorseToHouse				,
	Switch_SantaRaccoonEvent 			,
	Switch_SellToForeignMerchant		,
	Switch_LevelUpNotice				,
	Switch_NewYearEvent					,
	Switch_BasicStructureSet			,
	Switch_VillageWarpTower				,
	Switch_RaccoonGoldeggChange			,
	Switch_Transform					,
	Switch_BlackWarReform				,
	Switch_ChangeFullScreen				,
	Switch_UseDefaultIME				,
	Switch_InstanceDungeonTicket		,
	Switch_NONATIALIASED_QUALITY		,
	Switch_PartyMassageimprovement		,
	Switch_SpaceBox						,
	Switch_HINADOLL						,
	Switch_Puchikomi					,
	Switch_Summon_Ability_Reform		,
	Switch_GeneralMeetingSuggest		,
	Switch_Artifact						,
	Switch_FixedPositionFeastEffect		,
	Switch_ChatEditFilter				,
	Switch_AutoMakeItem					,
	Switch_ClickAndDrop					,
	Switch_TutorialVillage				,
	Switch_PartyHall					,
	Switch_PirateShip					,		// [����] ������ : ��Ƽ�δ� ����ġ
	Switch_PCRoom						,		// [����] PC�� ���� - �׽�Ʈ ��
	Switch_UseDayPlusRevitalize			,		// [����] �Ⱓ������ �� ���� - ��ǰ�� ȣǰ�� �Ⱓ������ ����
	Switch_CharStatusReform				,		// [����] ĳ���� ����â ����. 2008-3-24	Z
	Switch_SpecialPersonShop			,		// [����] �к� �����̾� ����. 2008-3-25	
	Switch_HealthAddChange				,		// KHY - 0325 - �ｺ ���� ����ġ.
	Switch_Event_Monster_Attack			,		// [����] ���� �̺�Ʈ - ���� ����. 2008-3-26	
	Switch_BigItemModify				,		// [����] ��� �ý��� ����
	Switch_MakeItemEXP					,		// [����] ���� ����ġ ����
	Switch_MarketBuyNew					,		// [����] ������ ����â ���� ����.
	Switch_GongZoArmorPercent			,		// [����] ���� - �� ���� �ۼ�Ʈ �߰�
	Switch_Izo_TinTinPercent			,		// [����] ���� - ������ ��� ������
	Switch_SummonReform					,		// [����] ĳ���� ����â ����. 2008-3-24
	Switch_FishFarmMineChange			,		// [���] ä��,���,���� ����. 2008.04.15
	Switch_FindInfo						,		// [����] ����â �˻� ��� 
	Switch_BlackArmyTicket				,		// [����] ���Ǳ��� ���ɼ� �����ڵ� �߰�
	Switch_NoPayBack					,		// [����] û��öȸ���� �߰�.
	Switch_PopupCheck					,		// [����] �˾�â ����.
	Switch_Event_Hunt_Reform			,		// [����] �����̺�Ʈ Ű�޶� ���� ���� ����.
	Switch_Notice_Separate				,		// [����] ���� �и�. 2008-6-2
	Switch_Char_Option					,		// [����] �ɸ��Ϳɼ���������.( 2008-06-03 ä��â �����и� �߰� )
	Switch_Hero_Bronze_Statue			,		// [����] ������ ���� ���� �̺�Ʈ. => 2008-6-9
	Switch_Summon_Separate				,		// [����] ��ȯ�� ���̺� �и�. 2008-6-10
	Switch_Show_MapName					,		// [����] �� �̵��� �� �̸� �����ֱ� (2008.06.19)
	Switch_Server_Event					,		// [����] ���� �̺�Ʈ 2008-6-20
	Switch_Artifact_Reform				,		// [����] ��Ƽ��Ʈ ����. => 2008-7-9
	Switch_DestroyDropItem				,		// [����] ������ Drop�� ������ �ı�
	Switch_ChangeHouseList				,		// [���] â�� �������̽� ���� (08.07.10)
	Switch_Summon_Market				,		// [����] ��ȯ�� �ŷ���. => 2008-7-15
	Switch_MarketConditions 			,		// [��] �ü�Ȯ��  2008-07-18
	Switch_Summon_EquipSton				,		// [����] ��ȯ�� ���� ������
	Switch_Summon_StaminaCharge			,		// [���] ��ȯ�� �������ͽ� �߰�
	Switch_Guild_DestroyOverMOnth		,		// [����] �Ѵ��̻� ��Ȱ�� ��� ���� 
	Switch_Postoffice_Group_Message		,		// [����] ��ü�� ��ü �޽��� ������. => 2008-8-4
	Switch_Guild_GuildMaster_Change		,		// [����] ��� ������ ���� �ý���
	Switch_QuestInfoDlg_Renewal			,		// [����] ����Ʈ �˸�â ������
	Switch_ShowQuestNotice				,		// [����] ����Ʈ ����â
	Switch_Homepage_link				,		// [��] Ȩ������ ��ũ �ý���.
	Switch_FourLeaf						,		// [����] ���� �ӱ⺰ ����ũ�ι� ����. => 2008-8-14 
	Switch_SummonHouse					,		// [����] ��ȯ�� ���� 
	Switch_Gacha_Reform					,		// [����] ���� ����. 
	Switch_Mykey						,		// [����] ������ �޴�.	=> 2008-9-3
	Switch_HalloweenEvent				,		// [����] �ҷ��� �̺�Ʈ
	Switch_MudangPresent				,		// [��] �������� ����.
	Switch_Trade_Renewal				,		// [����] ���� ����
	Switch_GlobalItemMall_Limit			,		// [����] �۷ι� ������ �� ��������
	Switch_EditWordBreak				,		// [����] ������ ���̾�α� ��Ʈ�� �ڵ� WORDBREAK����
	Switch_GameTip						,		// [����] ���� ��.
	Switch_Helper						,		// [����] �����.
	Switch_Teacher_Reform				,		// [����] ����,���� ����.
	Switch_SystemMarketSell				,		// [����] �ý��ۿ��� ���忡 ��� �Ǹ�.
	Switch_IKillU						,		// [����] ���ΰ�PVP�ý���
	Switch_PVP_League					,		// [����] PVP ����
	Switch_Set_Item_Add					,		// [����] ��Ʈ ������ �߰�.
	Switch_Elixir						,		// [����] ������.
	Switch_ValenTineWar					,		// [����] �߷�Ÿ�ε��� ���� 
	Switch_NewMarket					,		// [����] ���ո���.
	Switch_ManufactureInfluenceLuck		,		// [����] �����ÿ� ������ ��ſ� ������� ����ȴ�
	Switch_MakeFoodHealEXP				,		// [����] ����/�� ��������ġ ����(����ġ�� �����־ ����/���� �ƴϸ� ���� ����ġ ��������)
	Switch_WeaponSkill					,		// [����] ��������
	Switch_NewGuildHuntMapWar			,		// [����] ���ο� ��� ����� ��û 2009-03-24
	Switch_PeriodItem					,		// [��ȣ] ItemMall���� ���� �ѷ��ִ��� => (BUFF & Service ����) �Ⱓ ������ ������ ����  
	Switch_CarryOverGacha				,		// [����] �̿� ��í(Switch_Gacha_Reform ����ġ ���� �켱 ������ ó����)
	Switch_Avatar_artisan				,		// [��ȣ] �ƹ�Ÿ ���� �߰�
	Switch_TreasureEvent				,		// [����] ����ã�� �̺�Ʈ
	Switch_WeddingSystem				,		// [����] ��ȥ �ý���
	Switch_ItemExplain2					,		// [��ȣ] ������ ����2
	Switch_BalanceCurrency				,		// [��ȣ] ��ȭ�� ����
	Switch_SoulGuard					,		// [����] ��ȣ����
	Switch_We_Are_Family				,		// [����] ��ȥ �ý���
	Switch_GACHA_RAREITEM_CHANGE		,		// [����] ���� ��������� ���� 
	Switch_Durability			 		,		// [����] ������ ǥ�� - �߱��� ��� �ϱ� ���ؼ� 
	Switch_10MonthEvent_RacoonDodge		,		// [����] ���� ���ϱ� �̺�Ʈ [ 10�� �̺�Ʈ ]
	Switch_NewDurability			 	,		// [����] �� ������ �ý��� - ���� �ý��� �ڵ带 Ȱ���Ͽ� ������.
	Switch_BokBulBok					,		// [����] ���Һ� ����. (2009.08.03)
	Switch_GawiBawiBo					,		// [��ȣ] ����-����������
	Switch_SummonTramsform				,		// [����] ��ȯ�� ����.
	Switch_Dormancy						,		// [����] �޸� ���� �ý���.
	Switch_Festival_Food				,		// [����] ���� ����.
	Switch_SkillBook					,		// [����] ��ų��.
	Switch_CharHouse					,		// [����] ĳ���� ������
	Switch_RaccoonEvent_Change_Goonzu	,		// [����] �����̺�Ʈ ���� �̺�Ʈ�� ���� 2009-10
	Switch_BattleRoyal					,		// [����] ��Ʋ�ξ�
	Switch_NewYearEvent_2010			,		// [����] 2010�ų��̺�Ʈ.
	Switch_PartyQuest_PartyMatching		,		// [����] �ʺ��� ��Ƽ ����Ʈ �ý���(��Ƽ��Ī).
	Switch_ValentineDayEvent			,		// [����] �߷�Ÿ�ε����̺�Ʈ
	Switch_Change_manufacture_request_interface,// [����] ���� �Ƿ� �������̽� ����
	Switch_PKHuntMap_Change_Info,				// [����] pk ����� ��� ���� 
	Switch_Go_RaccoonWorld,						// [����] ������� �̵� ���� 
	Switch_LevelLimitBox,						// [����] *�߱���* �������� ���� ���� 
	//==================================================
	Switch_Last,
	Switch_Count = Switch_Last - 1		,
};

//-------------------------------------------------------------
// �����
//-------------------------------------------------------------
const SI16 ConstLanguage_English	=	1;
const SI16 ConstLanguage_China		=	2;
const SI16 ConstLanguage_Japan		=	3;
const SI16 ConstLanguage_Korea		=	4;
const SI16 ConstLanguage_USA		=	5;

//-------------------------------------------------------------
// ������ ���
//-------------------------------------------------------------
const SI32 ConstMaster_Korea		=	1;
const SI32 ConstMaster_Japan		=	2;
const SI32 ConstMaster_Global		=	3;
const SI32 ConstMaster_Taiwan		=	4;
const SI32 ConstMaster_USA			=	5;

//-------------------------------------------------------------
// FPS
//-------------------------------------------------------------
const SI16 ConstFrameRate_Low		=	20;
const SI16 ConstFrameRate_High		=	40;


#define MAX_SERVER_NUMBER						4	// �ϳ��� ��ī��Ʈ DB�� ���� ���� �ǰ� �ִ� ������ ��. 
													// �������� ���� �ٸ� siServerUnique �� ������ �־�� �Ѵ�. ex) 1,2,3,4 ..
													// �����۸��� ���� �������� �����Ҷ� �� ������ �����Ѵ�.

#define NOOBJECT								0
#define OBJECT1									1


#define NEW_SELECTED_DELAY						6
#define MOUSE_OVER_DELAY						1

#define MAX_WAIT_STEP_NUMBER					16	// ĳ���Ͱ� ���� �� �ִ� �ִ� ���� ��� �������� �� 
#define MAX_WAIT_TYPE							2   // ���� ���ϴ� �ִϸ��̼��� �ִ� �� .

#define MAX_MOVE_STEP_NUMBER					10	// ĳ���Ͱ� ���� �� �ִ� �ִ� �̵� ��� �������� �� 
#define MAX_DYING_STEP_NUMBER					16	// ĳ���Ͱ� ���� �� �ִ� �ִ� ��� ��� �������� �� 
#define  MAX_ATTACK_STEP_NUMBER					16	// ĳ���Ͱ� ���� �� �ִ� �ִ� ���� ��� �������� �� 

#define GAMECOLOR_RED						69
#define GAMECOLOR_YELLOW					70
#define GAMECOLOR_GREEN						68

//============================================================
// DIALOG BOX COLOR ���� 
//============================================================
#define COLOR_DIALOG_YELLOW					RGB(248, 255, 199)
#define COLOR_DIALOG_GREEN					RGB(136, 251, 148)
#define COLOR_DIALOG_RED					RGB(226, 114, 150)
#define COLOR_DIALOG_LIGHTRED				RGB(255, 167, 179)
#define COLOR_DIALOG_BLUE					RGB(15,  167, 240)
#define COLOR_DIALOG_BLACK					RGB( 0, 0, 0 )
#define COLOR_DIALOG_WHITE					RGB(255, 255, 255)
#define COLOR_DIALOG_LIGHTBLUE				RGB(143, 202, 233)
#define COLOR_DIALOG_WHITEBLUE				RGB(238, 238, 255)
#define COLOR_DIALOG_GRAY					RGB(224,213,213)
#define COLOR_DIALOG_DLG_BLUE				RGB(205, 205, 205)
#define COLOR_DIALOG_STATIC_BLUE			RGB(226, 222, 207)
#define COLOR_DIALOG_READONLYTEXT			RGB(170, 110, 95)
//============================================================

#define DOUBLE_CLICK_DELAY						300

// ���� ���� 						
#define SOUTH              						1
#define SW                 						2
#define WEST               						4
#define NW										8
#define NORTH               					16
#define NE                  					32
#define EAST                					64
#define SE                  					128 
						
#define SSW										1000						
#define WWS 									1001						
#define WWN 									1002						
#define NNW 									1003						
#define NNE 									1004						
#define EEN 									1005						
#define EES 									1006
#define SSE 									1007


//----------------------------------------------------
//    ĳ����  �Ӽ� 
//----------------------------------------------------
#define ATB_MOVE							0x00000001		// �����̴� ĳ���� �Ӽ� 
#define ATB_HERO							0x00000002		// ������ĳ���� �Ӽ�.  
#define ATB_ATTACK							0x00000004		// ���� �Ӽ�
#define ATB_SUMMONSTAMINASTATIC				0x00000008		// ��ȯ���� ���¹̳� �Ӽ�.
#define ATB_WEAKENEMYNOATTACK				0x00000010  	// ���� ���� �������� �ʴ� �Ӽ�. 
#define ATB_FISH							0x00000020  	// ���� ����. 
#define ATB_NOJJIM							0x00000040		// ����� �ʴ� ĳ����. 
//#define ATB_MINRANGE						0x00000080		// ������ �Ұ�- �����ϱ� ������ ���ϸ�����Ʈ2�� ���ؼ� ����
#define ATB_DAILYQUEST2						0x00000080		// ������ �Ұ�
#define ATB_HORSE							0x00000100		// ����. 
#define ATB_SYSTEMNPC						0x00000200		// SystemNPC
#define ATB_SELFBOMB						0x00000400		// �������
#define ATB_WAITSELFBOMB					0x00000800		// ������ �������
#define ATB_NONPCLIST						0x00001000		// NPC����Ʈ�� ���Ե��� �ʴ´�. 
#define ATB_COWORK							0x00002000		// �ֺ� ������ ���������Ѵ�. 
#define ATB_MONSTER							0x00004000		// ����
#define ATB_GOD								0x00008000		// ��Ȳ���� �ɼ��ִ� ĳ���� 
#define ATB_DEFAULTGOD						0x00010000		// ��Ȳ���� ���� �⺻�� �Ǵ� ĳ���� 
#define ATB_DEFAULTCASTLE					0x00020000		// ������ ���� �⺻�� �Ǵ� ĳ���� 
#define ATB_DAILYQUEST						0x00040000		// DailyQuest�� ������ ĳ����
#define ATB_WARMONSTER						0x00080000		// �����߿��� ������ ���� - ���� ǥ��â�� ǥ�� �Ұ�
#define ATB_EVENTXBOX						0x00100000		// EVENT �Ӽ��� ���� �ִ�.
#define ATB_CLONE							0x00200000		// �нż��� ������ �Ѵ�.
#define ATB_DEFAULTSUMMON					0x00400000		// ��ȯ���� �ɼ� �ִ��� �����Ѵ�..
#define ATB_HIREABLE						0x00800000		// �������� ���밡���ϴ�. 
#define ATB_BOSS							0x01000000		// ������ �����̴�.
#define ATB_FARM							0x02000000		// ������Ⱑ �����ϴ�. 
#define ATB_DAMAGELIMIT						0x04000000		// �ǰݽ� �������� �Ѱ谡 �ִ�. 
#define ATB_MINE							0x08000000		// ä���� �����ϴ�.
#define ATB_NOHEALTHBAR						0x10000000		// ü�¹ٸ� �� �����ش�.
#define ATB_NOICE							0x20000000		// ��� Ȯ���� 10���� 1�� �پ���.
#define ATB_TRANSFORM						0x40000000		// [���� : Ȳ���� 2007. 12. 28 => ���� ATB]
#define ATB_EVENT							0x80000000		// �̺�Ʈ�� ����

#define SERVICEAREA_ATB_KOREA				0x00000001		// �ѱ������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_CHINA				0x00000002		// �߱������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_JAPAN				0x00000004		// �Ϻ������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_ENGLISH				0x00000008		// ��������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_TAIWAN				0x00000010		// Ÿ�̿Ϲ����� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_USA					0x00000020		// USA�������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_NHNCHINA			0x00000040		// NHN_China�������� ���Ǵ� ĳ��������
#define SERVICEAREA_ATB_EUROPE				0x00000080		// ������������ ���Ǵ� ĳ��������


// UI08 �̹Ƿ� 0x00000008������ ��� ����
#define GMMODE_ATB_NONE						0		// �Ϲݸ��
#define GMMODE_ATB_GMSHOW					1		// ������� ��Ÿ���� ���( [GM] ǥ�� )
#define GMMODE_ATB_HIDE						2		// ���̵���
#define GMMODE_ATB_WARMVP					3		// ���� MVP [�Ϲ������� ����]
#define GMMODE_ATB_BUFF						5		// [�߰� : Ȳ���� 2007. 11. 27 ��� ����̰� �ֺ� ĳ������ ���� ����]

//------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------
#define TRANSPARENT_MODE_NORMAL					1
#define TRANSPARENT_MODE_OLDTRANS				2
#define TRANSPARENT_MODE_HALFTRANS				3
#define TRANSPARENT_MODE_SCREEN					4
#define TRANSPARENT_MODE_MULTIPLY				5
#define TRANSPARENT_MODE_ADD					6
#define	TRANSPARENT_MODE_ALPHA					7

//------------------------------------------------------------------------------------------------
// �ź� �Ӽ� - MAX_KINDINFO_NUMBER ũ�Ⱑ 420->500 �ۿ� �ȵǱ� ������ ���� ī�ε常(�Ӽ���������) �ٸ���� ���ҽ��� ���Ӱ� �߰����� �ʰ� �ش� ī�ε忡 IDENTITY ���� ���� �����ؼ� ����ϸ� ��.
//------------------------------------------------------------------------------------------------
#define IDENTITY_CASTLE							1			// �ź��� ���� ����
#define IDENTITY_VILLAGEGOD						2			// �ź��� ��Ȳ�� ����
#define IDENTITY_VILLAGEGOD_SUMMON				3			// �ź��� ������ ��Ȳ���� ��ȯ�� ��ȯ�� ����
#define IDENTITY_PERSON_SUMMON					4			// �ź��� ������ ��ȯ�� ��ȯ�� ���� 
#define IDENTITY_VILLAGEHUNT					5			// �ź��� ������� ��ɿ� ���� 
#define IDENTITY_BLACKARMY						6			// ���Ǳ��� ����
#define IDENTITY_VILLAGE_KINGMONUMENT			7			// ���� ��ȣ��
#define IDENTITY_BLACKWAR						8			// ���Ǳ��� ����
#define IDENTITY_MOFUMOFUEVENT					9			// ���ĸ����̺�Ʈ ����
#define IDENTITY_INSTANCEMAP					10			// �ν��Ͻ��� ����
#define IDENTITY_MONSTER_ATTACK_EVENT			11			// [����] ���� �̺�Ʈ - ���� ���� => 2008-3-26
#define IDENTITY_QUEST							12			// ����Ʈ�� ����
#define IDENTITY_SOULBOUND						13			// �ͼӸ��� (��ȯ�� ����� �����Ҽ��ִ�)
#define IDENTITY_SOULGUARD						14			// [����] ��ȣ����_090525 : 
#define IDENTITY_TREASURE_EVENT					15			// [����] Ʈ���� �̺�Ʈ�� ������ ���� 2009.07.01
#define IDENTITY_RACOONDODGE_EVENT				16			// [����] 6���̺�Ʈ �������ϱ� �̺����� ������ ���� 2009.08.11
#define IDENTITY_BOKBULBOK_EVENTMONSTER			17			// [����] ���Һ��̺�Ʈ : �Ϲ� ���� 2009.08.23
#define IDENTITY_BOKBULBOK_KEYMONSTER			18			// [����] ���Һ��̺�Ʈ : Ű ���� 2009.08.23
#define IDENTITY_BOKBULBOK_ONEKILLMONSTER		19			// [����] ���Һ��̺�Ʈ : ��ų ���� 2009.08.23
#define IDENTITY_SOULGUARD_SUMMONPUMKIN			20			// [����] �ҿﰡ�� ��Ų�� ��ȯ�� ���� 2009-8-28
#define IDENTITY_BOKBULBOK_ITEMMONSTER			21			// [����] ���Һ��̺�Ʈ : ���� ����
#define IDENTITY_BATTLEROYAL_MONSTER			22			// [����] ��Ʋ�ξ� ����
#define IDENTITY_GMCREATE_MONSTER				23			// [���] /CMT ���ɾ�� ������ ����
#define IDENTITY_GMCREATE_MONSTER				23			// [���] /CMT ���ɾ�� ������ ����
#define IDENTITY_GIANTRACCOONRESURRECTEVENT		24			// [����] ���̾�Ʈ ���� ��Ȱ ���� �̺�Ʈ. 

//------------------------------------------------------------------------------------------------
// ��� �Ӽ�		
//------------------------------------------------------------------------------------------------
#define DEFENCEATB_HUMAN    					1			// �Ϲ� ���� 
#define DEFENCEATB_SWORD    					2			// �Ϲ� ���� 
#define DEFENCEATB_ARMOUR   					3			// ���� 
#define DEFENCEATB_HORSE    					4			// �⺴ 
#define DEFENCEATB_STONE    					6			// �ǹ� 



//----------------------------------------
// ĳ���� ���� 
//----------------------------------------
#define MAX_ORDER_NUMBER				20

#define ORDER_WAIT    					1
#define ORDER_STOP    					2 
#define ORDER_FORCESTOP    				3					// Ŭ���̾�Ʈ�� ���޵Ǿ������� ���� STOP	 
#define ORDER_MOVE    					4					// �ܼ��̵� 
#define ORDER_ATTACK  					5					// �ܼ� ���� 
#define ORDER_ATTACK_SUMMON				6					// ��ȯ���� ����.  
#define ORDER_DIE     					7
#define ORDER_DEAD    					8
#define ORDER_MAGIC						9
#define ORDER_REVIVE					10
#define ORDER_STRAIGHTMOVE				11					// �����̵�. 
#define ORDER_FISH						12					// ���� �ϱ�. 
#define ORDER_FARM						13					// �������. 
#define ORDER_MAGIC_SUMMON				14					// ��ȯ���� ���� ����
#define ORDER_MINE						15					// ä��
#define ORDER_MOVE_SUMMON				16					// ��ȯ���� �̵�
#define ORDER_SITDOWN					17					// �ɱ�
#define ORDER_ATTACK_CLIENT				18					// �ܼ�����( ������û�� Ŭ��->������ ������ )
#define ORDER_SPECIAL_ACTION			19					// ���� ��ȯ�� - Ư�� ����


#define MAX_HORSE_NAME			12	// �� �̸��� ����. 
#define MAX_SUMMON_NAME			12	// ��ȯ�� �̸��� ����. 
#define MAX_SOULGUARD_NAME		12	// ��ȣ���� �̸��� ����

#define MAX_SOULGUARD_HOUSE_NUM	10	// ��ȣ���� ������ �ִ� ����

#define MAX_RENT_FEE			1000000
#define MAX_RENT_PERIOD			9

// ���� �̸� ���� 
#define FILE_NAME_SIZE							128

#define NORMAL_TEXT_COLOR	RGB(250, 250, 250)
#define BLANK_TEXT_COLOR	RGB(10, 10, 10)


#define MAX_ORDER_QUE							50

#define MAX_CHAR_NUMBER							20000			// ���ӿ� ������ �� �ִ� �ִ��� ĳ���� ���� 

#ifdef _CHINA
	#define MAX_PERSON_NUMBER						3000
	#define MAX_NPC_NUMBER							11000	// �ǹ��� ĳ���� ������.
	#define MAX_INSTANCE_NPC_NUMBER					5500		// �ν��Ͻ� ���� NPC
#else
	#define MAX_PERSON_NUMBER						3000
	#define MAX_NPC_NUMBER							11000	// �ǹ��� ĳ���� ������.
	#define MAX_INSTANCE_NPC_NUMBER					5500		// �ν��Ͻ� ���� NPC
#endif
	
#define MAX_KIND_NUMBER								2000		// �� ĳ������ ���� ���� 420->500 ���� ���� ���鼭 ���� ������ 420���� ����
//#define MAX_KIND_PER_PLAYER						10		// ���ΰ� ĳ������ ���� ����(���� +1��! 10���� NPC)
#define MAX_SORT_LEVEL								2000

#define MAX_BULLET_NUMBER						1004
#define MAX_SMOKE_NUMBER						1500

#define MAX_PLAYER_NAME							20	// ����ڰ� ���� �� �ִ� �ִ� �̸� ũ��.
#define MAX_SERVER_NAME							20	// ������ ���� �� �ִ� �ִ� �̸� ũ��.


#define MAX_CHAR_PER_USER						3	// �� ������ ���� �� �ִ� ĳ������ ��. 
#define MAX_CHAR_HOUSE_PER_USER					12	// �� ������ ���� �� �ִ� ĳ���ͺ������� ����
#define MAX_TOTAL_CHAR_PER_USER					((MAX_CHAR_PER_USER)+(MAX_CHAR_HOUSE_PER_USER))

#define MAX_VILLAGE_NUMBER						100	// ���ӿ� ������ ��� ���°� ?
#define VILLAGE_NONE							0	// 0 ������( ���� ������ �ǹ� ) 	

#define MAX_RESIDENTS_PER_VILLAGE				300 // �� ������ �ִ� �ֹμ�

#define MAX_INVEST_NUMBER						100	// ����ؾ� �ϴ� ������ ������ ����Ʈ 10 	

#define	MAX_CONSTRUCTION_NUM					10	// �� ������ ���� �� �ִ� �ǹ��� �ִ��. 

#define MAX_HEAD_CHAT_TEXT_SIZE					128

#define MAX_CHILDINFO_NUMBER					10	// ���� ������ �ѹ��� ���� �� �ִ� �ִ� ��. 

#define WAR_REWARD_VILLAGE_NUMBER				20	// �ֱ� ����� 20�� �������� �����ؼ� �����Ѵ�. 

#define	MAX_CHAT_DATA_LENGTH					100	// �ѱ� 50 ��

#define MAX_PERSONAL_SHOP_TEXT_SIZE				256	// �ѱ� 30�� ���λ��� banner 

#define MAX_BLOCKREASON_SIZE					200 // ����� ������ �� ������ �ؽ�Ʈ ������. 
#define MAX_BLOCKLOG_NUM						10	// ���� �α׸� �����ö� �ִ� ������ �� �ִ� ����

#define MAX_SCROLL_SUMMON_PER_KIND				2			// �������� ��ȯ���� �ٸ� ���ִ� �ִ� ������

#define MAX_HORSE_LIFE							3000	// ������ ����
#define MAX_SPECIALHORSE_LIFE					5000	// Ư���� ����
#define MAX_UNLIMITEDHORSE_LIFE					30001	// ���� ����
//KHY - 0622 - ����S �ʺ��ڿ�  ������ �߰�. - 0820 - ����.
#define MAX_FOAL_LIFE							1500	//3000	// ������ ����
//PCK - �̺�Ʈ �� ȣ���� ����
#define MAX_TIGER2_LIFE							300
#define MAX_GRAYWOLF_LIFE						3000	// ȸ������ ����

#define MAX_USERLIST_NUM						50		// �ִ� ������ ���� ����Ʈ�� ����[������]

#define MAX_FRIEND_GROUPNAME					20

#define MAX_CLIENT_COMMAND_SIZE					128

#define BASIC_ITEMPRICE_DEFAULT					20
//PCK - ��ȣ ����
#define MAX_WHITETIGER_LIFE						1000
#define MAX_WHITEHORSE_TEST_LIFE				300		// ü��� �鸶 : �ռ���-2009.03.17
//-----------------------------------
// ������ġ ���� 
//-----------------------------------
#define PRISON_MAPINDEX	0
#define PRISON_X		283
#define PRISON_Y		672


// PC�ΰ� ?
#define IsPC(x)			(x > 0 && x < MAX_PERSON_NUMBER)
// InstanceNPC�ΰ�?
#define IsInstanceNPC(x)	(x >= (MAX_PERSON_NUMBER ) && x < (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER))
// NPC�ΰ� ?
#define IsNPC(x)		(x >= (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER) && x < MAX_CHAR_NUMBER )

#define NETWORK_SERVER		1
#define NETWORK_CLIENT		2

enum eGameMode{
	GAMEMODE_SERVER	= 1	,
	GAMEMODE_CLIENT		,
	GAMEMODE_MAPEDIT
};

enum eRunMode {
	// �� ������ ����Ǹ� �ȵ˴ϴ�
	RUNMODE_DEBUG = 1,
	RUNMODE_TEST,
	RUNMODE_BETA,
	RUNMODE_REAL,		// �ٲ��� �ʰ� �ڿ� �߰��Ѵ�.

	RUNMODE_INSIDE		// ���� ���� �׽�Ʈ��
};

#define BY_PLAYER     0		
#define BY_COMPUTER   1

#define MIN_ITEM_PRICE	20			// ������ ������ �ּҰ�(�� ���Ϸδ� �������� �ʴ´�.) 

#define MAX_MATERIAL_NUMBER	10								// ������ ��� ���� �ִ밪
#define MAX_MATERIAL_NUMBER_FOR_DB	MAX_MATERIAL_NUMBER + 2	// ������ ��� ���� �ִ밪 Database �� - ����������� ���� �ϳ� �߰�
															//KHY - 1211 - ���ݻ�ǰ - ���� ���� - ���� ���� ���� �߰�.

#define FATHER_LEVEL	20
#define NEWFATHER_LEVEL 50

#define INIT_LEVEL		10			// �ʱ�ȭ �Ǵ� ���� 

#define INIT_LEVEL_SECOND		30			// �ι�° �ʱ�ȭ �Ǵ� ���� 


#define PERSON_SUMMON_ATTACKBONUS_PERCENT 10

//--------------------------
// ��ü ��� 
//--------------------------
#define ORG_1			1			// ��ȭ�� 
#define ORG_2			2			// ��ȯ�� 
#define ORG_3			3			// ȣ���� 
#define ORG_4			4			// �Ű�� 


#define MAX_LEVEL_FOR_BEGINNERMAP	10		// �ʺ��� ����� ������ ���� �ִ� ���� 

//----------------------------------------
// ��Ÿ�۾��� ���� ���� ����
//----------------------------------------

#define MAX_LEVEL_FOR_SANTAFACTORY_102	30		
#define MAX_LEVEL_FOR_SANTAFACTORY_123	60
#define MAX_LEVEL_FOR_SANTAFACTORY_124	100

//----------------------------------------
// ������ Ÿ�� 
//----------------------------------------
#define MAX_RANKTYPE_NUMBER	20

#define RANKTYPE_CITYHALL				1		// �����
#define RANKTYPE_BANK					2		// ������� 
#define RANKTYPE_STOCK					3		// ������� 
#define RANKTYPE_MARKET					4		// �������
#define RANKTYPE_HOUSE					5		// �������
#define RANKTYPE_HUNT					6		// ��������� 
#define RANKTYPE_POSTOFFICE				7		// ��ü�����
#define RANKTYPE_PORT					8		// �������
#define RANKTYPE_HORSEMARKET			9		// �纹�����
#define RANKTYPE_REALESTATEMARKET		10		// �Ű����
#define RANKTYPE_SUMMONMARKET			11		// �忹�����
#define RANKTYPE_SHIPMARKET				12		// ���Ի���� 
#define RANKTYPE_LAND					13		// �������  
#define RANKTYPE_FEAST					14		// ���ڽ����
#define RANKTYPE_SUMMONHEROMARKET		15		// �񺯻����
#define RANKTYPE_MINE					16		// �������
#define RANKTYPE_GUILD					17		// ������ 		
//#define RANKTYPE_NEWMARKET			18		// [����] ���ո��� 2009-1-5 
#define RANKTYPE_NEWMARKET				18		// [����] ���ո��� 2009-1-5 

#define CITYHALL_UNIQUE				1		// ��û
#define BANK_UNIQUE					10		// ���� 
#define STOCK_UNIQUE				20		// ���� 
#define MARKET_UNIQUE				40		// ����
#define HOUSE_UNIQUE				50		// ����(â��)
#define HUNT_UNIQUE					60		// ������ 
#define POSTOFFICE_UNIQUE			70		// ��ü��
//#define PORT_UNIQUE					80		// ����
#define HORSEMARKET_UNIQUE			90		// �纹��
#define REALESTATEMARKET_UNIQUE		100		// �Ű�
#define SUMMONMARKET_UNIQUE			110		// �忹��
//#define SHIPMARKET_UNIQUE			12		// ���Ի� 
#define LAND_UNIQUE					130		// ����  
#define FEAST_UNIQUE				140		// ���ڽ�
#define SUMMONHEROMARKET_UNIQUE		150		// �񺯻�
#define MINE_UNIQUE					160		// ����
#define GUILD_UNIQUE				170		// ��� 		
#define NEWMARKET_UNIQUE			180		// [����] ���ո��� 2009-1-5 




// ������ �ִ� ����. 
#define MAX_MAP_NUMBER					400

#define MAX_CLONEMAP_TYPE				10		// // �����(����) �� �ִ� ����
#define CLONEMAP_NUM					10		// �� Ÿ�Դ� ������� �ִ� ����

#define MAX_DUR_FOR_TUNE	200		// ������ �ʿ��� �ִ� ������(�� ��ġ �����̾�� �Ѵ�.) 

//------------------------------------
// ���� ���� ���� ���� 
//------------------------------------
#define VILLAGE_PROFIT_RATE_FOR_MERCHANT	5	// ������ �ŷ� ����� �� %�� ���� �������� �����ϴ°�. 
#define VILLAGE_PROFIT_RATE_FOR_DAILYQUEST	5	// �𺴰� ����Ʈ  ����� �� %�� ���� �������� �����ϴ°�. 
#define VILLAGE_PROFIT_RATE_FOR_NPCREPAIR	5	// �念�� ���� ����� �� %�� ���� �������� �����ϴ°�. 
#define VILLAGE_PROFIT_FOR_WENNYOLDMAN		5000
#define VILLAGE_PROFIT_FOR_FANPRIEST		2000
#define VILLAGE_PROFIT_FOR_PERSONALSHOP     5000
#define VILLAGE_PROFIT_FOR_INVESTMENT		5000 // ���ڻ�㰡(���� �輱��)
#define VILLAGE_PROFIT_RATE_FOR_DAILYQUEST2	5	// �𺴰� ����Ʈ  ����� �� %�� ���� �������� �����ϴ°�. 

#define MAX_EVENT_NUMBER					8		// �̺�Ʈ�� ����. 
#define MIN_LEVEL_FOR_HORSE					15		// ���� �����ϱ� ���� �ּ� ���� 

//--------------------------------
// ������ ���� 
//--------------------------------
#define SEASON_SPRING		0
#define SEASON_AUTUMN		1
#define SEASON_WINTER		2


//------------------------------
// ������ Ÿ�� 
//------------------------------
#define VILLAGETYPE_SPRING_NORMAL		1
#define VILLAGETYPE_AUTUMN_NORMAL		2
#define VILLAGETYPE_WINTER_NORMAL		3

//------------------------------
// ���� �̺�Ʈ �ǹ� 
//------------------------------
#define EVENT_STRUCTURE_TREE				   1
#define EVENT_STRUCTURE_CHRISTMASTREEBASE	  10
#define EVENT_STRUCTURE_NEWYEARTREEBASE		  11
#define EVENT_STRUCTURE_HERO_BRONZE			   2		//[����] ������ ����. => 2008-6-9
#define EVENT_STRUCTURE_HALLOWEEN			   3		//[����] �ҷ��� �̺�Ʈ ������� �⺻ ȣ��
#define EVENT_STRUCTURE_CHRISTMASTREE		1001
#define EVENT_STRUCTURE__PCAKE				3001
#define EVENT_STRUCTURE__PCHAMPAGNE			3002
#define EVENT_STRUCTURE__PBARBECUE			3003
#define EVENT_STRUCTURE__PGARLAND			3004
#define EVENT_STRUCTURE__PICESTATUE			3005
#define EVENT_PVP_LEAGUE					3006		//[����] PVP_League ������ ȹ��
#define EVENT_STRUCTURE_WORLDCHRISTMASTREE	4001		// [���] ���縮�� �տ� �������� ũ�������� Ʈ��

//------------------------------
// ������ Ÿ�� 
//------------------------------
#define HUNTTYPE_ALL		1

// ��Ī
#define MAX_NAMINGQUEST_INDEX	15
#define MAX_NAMINGQUEST_LEVEL	15

//------------------------------
// �̺�Ʈ Ÿ�� -  //KHY - 0906 - ���ӹ� �̺�Ʈ ����.
//------------------------------
#define GAMEROOM_COUPON_EVENT		1
#define AUCTION_COUPON_EVENT		2
#define GOSTOP_POKER_EVENT			4
#define HAN_GAME_VIP_EVENT			5
#define NOW_COM_EVENT				6
#define SUPER_ROOKIE_LEVELUP_EVENT	7


//------------------------------
// [����] �⼮ ���� �̺�Ʈ
//------------------------------
enum ATTEND_EVENTTYPE 
{
	ATTEND_EVENTTYPE_NONE			= 0 // ��������
	,ATTEND_EVENTTYPE_ATTENDANCE	= 1	// �⼮üũ �̺�Ʈ
	,ATTEND_EVENTTYPE_PCROOM			// PC�� ����Ŭ�ι� ����
	,ATTEND_EVENTTYPE_SEASON2			// ����2 ���±�� �̺�Ʈ

	//---------------------------------
	// ���ο� Ÿ���� �� ���� ���ּ���
	//---------------------------------
	,ATTEND_EVENTTYPE_MAX				// �̺�Ʈ Ÿ���� ��
};

///////////////////////////////// ���ڰ��� ���ڿ��� ����ؼ� �ش� ���ڿ��� ������ ���ڰ��� ��ȯ�Ѵ�////////////////////////////////////
#define COMPARE_TEXT_WITH_VALUE( text, value ) \
{	if ( 0 == _tcscmp(text, #value) )	return value;	}
///////////////////////////////// ���ڰ��� ���ڿ��� ����ؼ� �ش� ���ڿ��� ������ ���ڰ��� ��ȯ�Ѵ�////////////////////////////////////

// [����] ���� ����
const SI32 Const_Max_Candidate		= 21;		// ���� �ĺ� ������ �̸�
const SI32 Const_Max_Maintenance	= 256;		// ���� ����

// [����] �ʺ��� ���� - ��ī��
const SI32 Const_Beginner_Village		= 1;	// �ʺ��� ���� ����ũ
const SI32 Const_BV_Max_Resident_Level	= 40;	// �ʺ��� ������ ������ �� �ִ� �ִ� LV
const SI32 Const_BV_StockDividend_Level	= 30;	// �ʺ��� ������ ������ ���ֹ���� �޴� ����
const SI32 Const_BV_Max_Resident_Num	= 1500;	// �ʺ��� ������ ������ �� �ִ� �ִ� �ֹ� ��

const SI32 Const_Attendance_Modulation_Day = 28;	// [����] �⼮�ϼ��� 28�� �ֱ�� 0���� �ʱ�ȭ �ȴ�

const SI32 Const_Passport_Issue_Cost_Acu	= 5;	// [����] �������� ���� �߱޺��(ACU)
const SI32 Const_Passport_Extend_Cost_Acu	= 3;	// [����] �������� ���� ������(ACU)

//----------------------------------------
// [����] �̺�Ʈ On/Off��������
//----------------------------------------
enum EVENT_TOGGLE_SWITCH
{
	EVENT_TOGGLE_SWITCH_FISHINGEVENT = 0,	// ���� �̺�Ʈ
	EVENT_TOGGLE_SWITCH_SYSTEMBUY,			// �ý��� ����


	//------------------------------------
	// ���ο� ���� �� ���� �����ּ���
	//------------------------------------
	EVENT_TOGGLE_SWITCH_MAX
};

//----------------------------------------
// [����] PVP_LEAGUE ��ް���(Ȥ�� �߰��� �þ���� ����ؼ� �� 5�� �����Ǵ� ���� ���´�)
//----------------------------------------
enum PVP_LEAGUE_GRADE
{
	PVP_LEAGUE_GRADE_NONE	= 0,	// ����� ����(PVP �������� ���� �ʾ���)

	PVP_LEAGUE_GRADE_9		= 5,
	PVP_LEAGUE_GRADE_8		= 10,
	PVP_LEAGUE_GRADE_7		= 15,
	PVP_LEAGUE_GRADE_6		= 20,
	PVP_LEAGUE_GRADE_5		= 25,
	PVP_LEAGUE_GRADE_4		= 30,
	PVP_LEAGUE_GRADE_3		= 35,
	PVP_LEAGUE_GRADE_2		= 40,
	PVP_LEAGUE_GRADE_1		= 45,

	PVP_LEAGUE_GRADE_MAX
};

const SI32 Const_PVP_League_Default_Grade = PVP_LEAGUE_GRADE_6;

//------------------------------
// ���� �Ӽ� Ÿ��  by tea-yang
//------------------------------
// Ȧ���� ���ݼӼ�, ¦���� ���Ӽ���
// [����] define���� enum���� ����
enum ENCHANT_TYPE
{
	 ENCHANT_TYPE_DEFAULT		= 0	// 00 �ƹ��Ӽ��� �� �پ�����
	,ENCHANT_TYPE_FIRE_A			// 01 �� ���ݼӼ�
	,ENCHANT_TYPE_FIRE_D			// 02 �� ���Ӽ�
	,ENCHANT_TYPE_WATER_A			// 03 �� ���ݼӼ�
	,ENCHANT_TYPE_WATER_D			// 04 �� ���Ӽ�
	,ENCHANT_TYPE_WIND_A			// 05 �ٶ� ���ݼӼ�
	,ENCHANT_TYPE_WIND_D			// 06 �ٶ� ���Ӽ�
	,ENCHANT_TYPE_GROUND_A			// 07 �� ���ݼӼ�
	,ENCHANT_TYPE_GROUND_D			// 08 �� ���Ӽ�
	,ENCHANT_TYPE_FIRE_WATER_A		// 09 �� & �� ���ݼӼ�
	,ENCHANT_TYPE_FIRE_WATER_D		// 10 �� & �� ���Ӽ�
	,ENCHANT_TYPE_FIRE_WIND_A		// 11 �� & �ٶ� ���ݼӼ�
	,ENCHANT_TYPE_FIRE_WIND_D		// 12 �� & �ٶ� ���Ӽ�
	,ENCHANT_TYPE_FIRE_GROUND_A		// 13 �� & �� ���ݼӼ�
	,ENCHANT_TYPE_FIRE_GROUND_D		// 14 �� & �� ���Ӽ�
	,ENCHANT_TYPE_WATER_WIND_A		// 15 �� & �ٶ� ���ݼӼ�
	,ENCHANT_TYPE_WATER_WIND_D		// 16 �� & �ٶ� ���Ӽ�
	,ENCHANT_TYPE_WATER_GROUND_A	// 17 �� & �� ���ݼӼ�
	,ENCHANT_TYPE_WATER_GROUND_D	// 18 �� & �� ���Ӽ�
	,ENCHANT_TYPE_WIND_GROUND_A		// 19 �ٶ� & �� ���ݼӼ�
	,ENCHANT_TYPE_WIND_GROUND_D		// 20 �ٶ� & �� ���Ӽ�
};


//-----------------------------------------------------------------------------
// Data x�� ���� ����ִ��� ������ �˻� ������ true(TRUE) , ������ false(FALSE)
//-----------------------------------------------------------------------------
#define DATA_TO_BOOL(x)				(x!=0)?(x>0):false?true:false
#define DATA_TO_TYPE_BOOL(x)		(x!=0)?(x>0):FALSE?TRUE:FALSE

// TradeServer - SJY
#define MAX_TRADESERVER_ORDER		1 // �ִ� �ֹ���
#define MAX_LOWLEVEL_LIST_NUMBER	21 // 20�̸� ���� ����

//-----------------------------------------------------------------------------
// ������ ����� ������

enum
{
	GONGZO_SKILL_START = 1 ,
	//=====================================================
	
	GONGZO_SKILL_NONE  = 0,
	GONGZO_SKILL_SWORD = GONGZO_SKILL_START ,
	GONGZO_SKILL_SPEAR ,
	GONGZO_SKILL_STAFF ,
	GONGZO_SKILL_AXE ,
	GONGZO_SKILL_BOW ,
	GONGZO_SKILL_GUN ,

	GONGZO_SKILL_HELMET,
	GONGZO_SKILL_SHOES,
	GONGZO_SKILL_BELT,
	GONGZO_SKILL_RESERVED1,
	GONGZO_SKILL_ARMOR,
	GONGZO_SKILL_RESERVED2,
	GONGZO_SKILL_RESERVED3,

	//=====================================================
	GONGZO_SKILL_LAST = GONGZO_SKILL_RESERVED3,
	GONGZO_SKILL_TOTAL,
};

struct GongzoMember
{
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, GONGZO_SKILL_TOTAL>		siPriceValue;
	NSafeTArray<SI16, GONGZO_SKILL_TOTAL>		siSkillValue;
#else
	SI16	siPriceValue[GONGZO_SKILL_TOTAL];
	SI16	siSkillValue[GONGZO_SKILL_TOTAL];
#endif

	GMONEY	siCraftFund;
};

#define MAX_INVESTMENT_ITEM_NUMBER	50// ���ڻ�㰡�� ������ ���� ���� �� ����

struct InvestmentLines
{
	SI32	siInvestmentItemUnique; // �������� ����ũ
	GMONEY	siInvestmentItemPrice; // �������� ����

	GMONEY	siMarginalProfit; // ����

#ifdef _SAFE_MEMORY
	NSafeTArray<GMONEY, MAX_MATERIAL_NUMBER>	siMaterialItemPrice;
#else
	GMONEY	siMaterialItemPrice[MAX_MATERIAL_NUMBER];
#endif
};

//-------------------------------------------
// �� 
//-------------------------------------------
#define MAX_MONEY	2100000000	// ���� �ִ� ����. 
class cltMoney{
public:
	GMONEY itMoney;

	void Init();

	void Set(cltMoney* pclmoney);
	void Set(SI64 money);
	
	cltMoney Minus(cltMoney* pclmoney);
	cltMoney Plus(cltMoney* pclmoney);
	void Increase(cltMoney* pclmoney);
	void Decrease(cltMoney* pclmoney);

	SI32 GetPercent(cltMoney* pclmoney); // itMoney���� pclmoney�� �����ϴ� ����. 
	void Clear();
	bool Empty();
	
	bool Large(cltMoney* pclmoney);
	bool Small(cltMoney* pclmoney);
};


//-------------------------------------------
// ��ġ ���� 
//-------------------------------------------
class cltPos{
public:
	SI32 siX;
	SI32 siY;

	
	cltPos();
	cltPos(SI32 x, SI32 y);
	void Init();
	void Set(const cltPos* pclpos);
	void Set(SI32 six, SI32 siy);
};

// ���� ����. 
class cltWarpInfo{
public:
	UI32 uiWarpSwitch:		1;
	UI32 uiMapIndex:		9;
	UI32 uiForceSwitch:		1;		// ���ֺ��ڸ� �˻� ���� �ٷ� ���ڸ��� ���� 
	UI32 uiTicketUseSwitch:	1;		// ���� �������� ����� ����. (Ư�� ȿ���� ������� �Ѵ�) 
	UI32 uiX:				10;
	UI32 uiY:				10; 

	SI16	siGateX, siGateY;	// ����Ʈ ��ġ. (����Ʈ�� ���� �������� -1) 

public:

	
	cltWarpInfo();
	cltWarpInfo(UI32 warpswitch, UI32 mapindex, UI32 forceswitch, UI32 ticketuse, UI32 x, UI32 y, SI16 gatex, SI16 gatey);

	void Set(const cltWarpInfo* pclinfo);
	void Init();

	UI32 GetWarpSwitch() const;
	void SetWarpSwitch(UI32 mode);

	UI32 GetMapIndex() const;
	UI32 GetX() const;
	UI32 GetY() const;

	SI16 GetGateX() const;
	SI16 GetGateY() const;
};


//---------------------------------
// ������ Person���� 
//---------------------------------
class cltSimplePerson{
public:
	SI32	siPersonID;
	TCHAR	szName[MAX_PLAYER_NAME];

	cltSimplePerson();
	cltSimplePerson(SI32 personid, const TCHAR* pname);
	
	void Set(const cltSimplePerson* pinfo);
	void Set(SI32 personid, const TCHAR* pname);
	
	void Init();
	
	SI32 GetPersonID() const;
	void SetPersonID(SI32 id);

	const TCHAR* GetName()const;
	void SetName(const TCHAR* pname);
	
};


//-------------------------------
// ĳ������ ��ġ ���� 
//-------------------------------
class cltCharPos{
public:
	SI16 siMapIndex;	// ĳ������ ��ġ.(���� �ε���)  
	SI16 siX;			// ĳ������ ��ġ (���� ��ǥ ) 
	SI16 siY;			// ĳ������ ��ġ (���� ��ǥ ) 
	SI16 siReserved;

public:
	
	cltCharPos();
	cltCharPos(SI32 mapindex, SI32 x, SI32 y);
	
	void Set(const cltCharPos* pclpos);
	void Init();
	
	void SetMapIndex(SI32 index);
	SI32 GetMapIndex() const;

	void SetX(SI32 x);
	SI32 GetX() const;

	void SetY(SI32 y);
	SI32 GetY() const;
	
};


//----------------------------------
// ���� ������ ������ �ź� ���� 
//----------------------------------
class cltSimpleRank{
public:
	SI08 siType;			// ������ Ÿ��. 
	SI08 siVillageUnique;	// ������ ���õ� ������ ��� �ش� ����. 

public:

	
	cltSimpleRank();
	cltSimpleRank(SI32 ranktype, SI32 villageunique);
	cltSimpleRank(cltSimpleRank* pclrank);
	
	void Init();
	
	void Set(SI08 ranktype, SI08 villageunique);
	void Set(const cltSimpleRank* pinfo);
	
	void SetRankType(SI08 type);
	SI32 GetRankType() const;

	void SetVillageUnique(SI08 villageunique);
	SI32 GetVillageUnique() const;

	BOOL IsSame(cltSimpleRank* pclrank) const;
	
};

#define GUILDRANKTYPE_NONE			0		// ���� �ƴ� - �ʱⰪ
#define GUILDRANKTYPE_MASTER		1		// �����
#define GUILDRANKTYPE_MEMBER		2		// ����
#define GUILDRANKTYPE_STANDING		3		// ��� ���� �����
#define GUILDRANKTYPE_SECONDMASTER	4		// �α����
#define GUILD_MAXNAMENUM				13		// �ִ����̸�����
#define MAX_GUILDRANK				10		// ����ͱ���� ���

//----------------------------------
// ���� ������ ������ ��� �ź� ���� 
//----------------------------------
class cltSimpleGuildRank{
public:
	SI08 siType;			// ������ Ÿ��. 
	SI32 siVillageUnique;	// ��� �Ҽ� ����
	SI32 siGuildUnitIndex;	// ��� �ε��� 
	TCHAR szGuildName[GUILD_MAXNAMENUM];	// ��� �̸�
	UI08 uiConfirm;			// ��� ���� ����
	SI32 siMarkIndex;		// ��� ��ũ
	SI32 siEnemyGuildUnique; // ����� ����ũ
	SI32 siEmpireUnique;	// ��尡 �Ҽӵ� ��������ũ

public:
	cltSimpleGuildRank();
	cltSimpleGuildRank( SI08 ranktype, SI32 villageunique, SI32 guildunitindex,TCHAR*name ,UI08 confirm,SI32 mark, SI32 enemyguildunique = 0, SI32 EmpireUnique = 0);
	cltSimpleGuildRank( cltSimpleGuildRank* pclrank );
	
	void Init();
	void Set( SI08 ranktype, SI32 villageunique, SI32 guildunitindex ,TCHAR*name,UI08 confirm,SI32 mark, SI32 enemyguildunique = 0, SI32 EmpireUnique = 0);
	void Set(const cltSimpleGuildRank* pclrank);
	
	void SetRankType(SI08 type);
	SI32 GetRankType() const;

	void SetVillageUnique(SI32 villageunique);
	SI32 GetVillageUnique() const;

	void SetGuildUnitIndex(SI32 guildunitindex);
	SI32 GetGuildUnitIndex() const;

	void SetEnemyGuild( SI32 enemyguildunique );
	SI32 GetEnemyGuild() const;

	void SetEmpireUnique( SI32 EmpireUnique );
	SI32 GetEmpireUnique() const;

	BOOL IsSame(cltSimpleGuildRank* pclrank) const;

	bool IsGuildMember();

};

// ��Ī ����.
class cltPersonNaming
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_NAMINGQUEST_INDEX>	m_siLevel;			// �ε����� �ش��ϴ� ���̹��� ����.
#else
	SI32 m_siLevel[MAX_NAMINGQUEST_INDEX];			// �ε����� �ش��ϴ� ���̹��� ����.
#endif
	SI32 m_siSelectedIndex;							// ���� ���õ� �ε���
	SI32 m_siSelectedLevel;

public:
	cltPersonNaming();
	~cltPersonNaming();

	void Init();

	void Set( cltPersonNaming* pclCharNamingInfo );
};


//KHY - 1220 - ���Žý���.
#define MAX_TRANSFORM_CHAR 16 // �������Ǹ� �ƽ���ġ 14->16���� �ø�.

class cltTransForm
{
public:
	UI16 	uiCanTransFormKind;		// ���� ������ ĳ������ ����.
	SI32	siUseTotalSecond;		// ���� ĳ���� ���� ��� �ð�.

	cltTransForm()
	{
		ZeroMemory(this, sizeof(cltTransForm));
	}

	void Set(cltTransForm* pclTransForm)
	{
		if(pclTransForm == NULL)		return;

		uiCanTransFormKind	= pclTransForm->uiCanTransFormKind;
		siUseTotalSecond	= pclTransForm->siUseTotalSecond;
	}	

};

class cltTransFormInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTransForm, MAX_TRANSFORM_CHAR>	clTransForm;
#else
	cltTransForm clTransForm[MAX_TRANSFORM_CHAR]; // ���� ĳ���ͺ� �⺻ ����.
#endif

	UI16 	uiNowTransFormKind;	// ������(Ȥ�� ����)�������� ĳ������ ����.
	bool 	bTransFormMode;		// ���� ���� ����. - True = ����.

	bool 	bTransForm100Effect;	// ���� 100% ����Ʈ�� �׶���.

public:
	cltTransFormInfo();
	void Init();
	void Set(cltTransFormInfo* pclTransFormInfo)
	{
		if(pclTransFormInfo == NULL)		return;

		for(SI16 i=0; i<MAX_TRANSFORM_CHAR; i++)
		{
			clTransForm[i].Set(&pclTransFormInfo->clTransForm[i]);
		}

		uiNowTransFormKind = pclTransFormInfo->uiNowTransFormKind;
		bTransFormMode = pclTransFormInfo->bTransFormMode;
		bTransForm100Effect = pclTransFormInfo->bTransForm100Effect;
	}

	bool SetTransFormChar(UI16 uTransFormChar);				// ���� ĳ���͸� �����Ѵ�.
	bool SearchTransFormChar(UI16 uTransFormChar);				// ���� ĳ���͸� �����Ѵ�.	

	SI32 AddTransFormTime(UI16 uiTransformChar, SI32 sAddSecond);		// ���� ĳ������ �ð��� �� �Ѵ�. - return = ���� ���� ���ɽð�.

	bool CanTransForm(UI16 uTransFormChar);					// ���� ���ɿ��θ� Ȯ���Ѵ�.
	REAL32 GetSynchroTransForm(UI16 uTransFormChar);			// �ش� Ÿ���� ĳ���Ϳ��� ��ũ������ ����Ѵ�. - return = ��ũ������ %�� ��ȯ�Ѵ�.
};

//----------------------------------
// ���¹�ȣ Ŭ���� 
//---------------------------------
class cltAccountNum{
public:
	SI32 siVillageUnique;	// ������ �ִ� ������ ����ũ. 
	SI32 siAccountNum;		// ���¹�ȣ. 

	cltAccountNum();
	cltAccountNum(SI32 villageunique, SI32 accountnum);
	
	void Init();
	void Set(const cltAccountNum* pclaccountnum);

	// �� ���� ���°� ��ȿ�Ѱ�?
	BOOL IsValid();
	bool IsSame(cltAccountNum* pclnum);

	void SetVillageUnique(SI32 villageunique);
	SI32 GetVillageUnique() const;

	void SetAccountNum(SI32 num);
	SI32 GetAccountNum() const;
};

//-------------------------------------
// �ź��� ���� ����� ����. 
// (�����, �������, ������� ���...
//-------------------------------------
class cltRankPerson{
public:
	SI32 siVillageUnique;
	cltSimplePerson	clPerson;

	cltRankPerson();
	cltRankPerson(SI32 sivillageunique, const cltSimplePerson* pinfo);

	void Set(const cltRankPerson* pclinfo);
	void Set(SI32 sivillageunique, const cltSimplePerson* pinfo);
	
	void Init(SI32 villageunique);
	
	SI32 GetPersonID() const;
	SI32 GetVillageUnique() const;
	
};


//------------------------------------------
// ������ Ŭ���̾�Ʈ�� ������ ���ϰ� Ŭ���� 
//------------------------------------------
class cltServerResponseValue{
public:
	SI16 siReceiver;
	SI16 siReturnValue;
	SI16 siPara1;
	SI16 siPara2;
	
	cltServerResponseValue();
	cltServerResponseValue(SI16 receiver, SI16 value, SI16 sipara1, SI16 sipara2);	
};

//KHY - 0910 - Ŭ���̾�Ʈ�� ĳ���������� ���ϰ��� ������.
//------------------------------------------
// ������ Ŭ���̾�Ʈ�� ������ ���ϰ� Ŭ���� 
//------------------------------------------
class cltServerResponseCharValue{
public:
	SI16 siReceiver;
	SI16 siReturnValue;
	TCHAR siPara1[100];
	TCHAR siPara2[100];

	
	cltServerResponseCharValue();
	cltServerResponseCharValue(SI16 receiver, SI16 value, TCHAR* sipara1, TCHAR* sipara2);
	
};
//KHY - 0910 - Ŭ���̾�Ʈ�� ĳ���������� ���ϰ��� ������.
//------------------------------------------
// ������ Ŭ���̾�Ʈ�� ������ ���ϰ� Ŭ���� 
//------------------------------------------
class cltServerResponseCharValue2{
public:
	SI16 siReceiver;
	SI16 siReturnValue;
	TCHAR siPara1[100];
	TCHAR siPara2[100];
	TCHAR siPara3[100];


	cltServerResponseCharValue2();
	cltServerResponseCharValue2(SI16 receiver, SI16 value, TCHAR* sipara1, TCHAR* sipara2, TCHAR* sipara3 );

};
//------------------------------------------
// ���� ��� �ڷ��� ��� ������ ����� ���� ����ü�� ���� [����]
//------------------------------------------
#define SERVER_BACKUPFILE_HEADER_VERSION 100

struct stServerBackUpFileHeader
{
	TCHAR szDesc[32];
	SI32  siVersion;	
};


#define MIN_FEE	100
#define MAX_FEE	100000
#define MAX_FEE_RATE	100		// 10%

#define MIN_FEE_RATE_NEWMARKET	 5		//[����] ���ո����� ���� �������   5%
#define MAX_FEE_RATE_NEWMARKET	15		//[����] ���ո����� �ִ� �������  15%

//-----------------------------------------
// ������ ���� 
//-----------------------------------------
class cltFee{
private:
	SI32	siFeeRate;		// Ÿ�� ����� ������ (�۹�) 
	GMONEY	siMinFee;		// ���� ������. 
	GMONEY	siMaxFee;		// �ִ� ������. 	
public:

	void Set(cltFee* pclfee,SI32 value = 1000);
	
	// �ŷ� �ݾ��� �ٰŷ� �����Ḧ ����Ѵ�. 
	GMONEY CalcFee(GMONEY money,SI32 value = 1000); // value�� 100�̸� 100���� ��� , 1000�̸� õ���� ���
	BOOL SetFeeRate(SI32 rate,SI32 value = 1000 );  // value�� 100�̸� 100���� ��� , 1000�̸� õ���� ���
	BOOL SetFeeRateNew( SI32 rate, SI32 value = 1000 );		// 5 ~ 10%
	SI32 GetFeeRate() const;
	BOOL SetMinFee(GMONEY fee);
	BOOL SetMinFeeNew(GMONEY fee);
	GMONEY GetMinFee() const;

	BOOL SetMaxFee(GMONEY fee);
	BOOL SetMaxFeeNew(GMONEY fee);
	GMONEY GetMaxFee() const;

	// ���� �����ᰡ �������� Ȯ���Ѵ�. 
	BOOL IsValid(SI32 value); // value�� 100�̸� 100���� ��� , 1000�̸� õ���� ���
	
	// ���� �����ᰡ �������� Ȯ���Ѵ�. 
	BOOL IsValid();

	BOOL IsValidNew();
};

//-------------------------------------
// �α��ο�û ���� 
//-------------------------------------
class cltLogInInfo{
public:
	SI32	siAuthKey;
	TCHAR	szID[MAX_PLAYER_NAME];			// ���̵��� ������. 
	TCHAR	szPassword[MAX_PLAYER_NAME];	// ��ȣ�� ������. 
	UI08	uiGameMode;						// ���� Ŭ���̾�Ʈ ���� ��� : 0 => ������ â + ������ �������̽����, 
											// 1 => ������ + ���������̽� ���, 2 => ��üȭ�� + ���������̽� ���
	
	cltLogInInfo(SI32	authkey, TCHAR* id, TCHAR* password, UI08 gamemode);
	
};


//-------------------------------------
// NHN HANAUTH �α��ο�û ���� 
//-------------------------------------
class cltLogIn_HanAuthInfo{
public:
	SI32	siAuthKey;
	TCHAR	szAuthString[2048];				// NHN HANAUTH AuthString
	TCHAR	szID[MAX_PLAYER_NAME];			// ���̵��� ������. 
	UI08	uiGameMode;						// ���� Ŭ���̾�Ʈ ���� ��� : 0 => ������ â + ������ �������̽����, 
	// 1 => ������ + ���������̽� ���, 2 => ��üȭ�� + ���������̽� ���

	cltLogIn_HanAuthInfo(SI32 authkey, TCHAR* authstring, TCHAR* id, UI08 gamemode);
};

//-----------------------------------------------------------------------------------------------
// ���� ��û(���ӽ��൵�߿� ��������üũ) : NHN HANAUTH (PCK : 2007.09.19)
//-----------------------------------------------------------------------------------------------
struct cltHanAuth_GameRunAuth
{
	TCHAR				authstring[ 2048 ];
	TCHAR				userid[ 20 ];
};

//-------------------------------------
// TAIWAN �α��ο�û ���� 
//-------------------------------------
class cltLogIn_TaiwanInfo{
public:
	SI32	siAuthKey;
	TCHAR	szGameAccount[30];				// Game Account
	TCHAR	szOneTimePassword[40];			// One Time Password
	UI08	uiGameMode;						// ���� Ŭ���̾�Ʈ ���� ��� : 0 => ������ â + ������ �������̽����, 
	// 1 => ������ + ���������̽� ���, 2 => ��üȭ�� + ���������̽� ���

	cltLogIn_TaiwanInfo(SI32 authkey, TCHAR* gameaccount, TCHAR* onetimepassword, UI08 gamemode);
};

//-------------------------------
// ĳ���Ϳ��� �־����� ���� 
//-------------------------------
class cltOrderDataInfo{
public:
	SI08 siOrder;             // �����ؾ� �� ���� �ڵ� 
	SI08 siHostMode;          // ������ ���� ��ü (�÷��̾� �Ǵ� ��ǻ�� )

	SI32 siPara1;				// ���ɿ� ���� ���� �Ķ��Ÿ 
	SI16 siPara2;				// ���ɿ� ���� ���� �Ķ��Ÿ 
	SI16 siPara3;				// ���ɿ� ���� ���� �Ķ��Ÿ 
	SI16 siPara4;				// ���ɿ� ���� ���� �Ķ��Ÿ (PVP������ ����)

	DWORD	dwPushedTime;		// ������ �ð�

	cltOrderDataInfo();
	cltOrderDataInfo(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3, SI16 para4);
	cltOrderDataInfo(cltOrderDataInfo* pclinfo);
	
	void Init();
	void Make(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3, SI16 para4);
	void Set(cltOrderDataInfo* pclorder);

};

// ������ ��ü 
// ��ǻ�Ͱ� ������ �����ϴ� ������ ������ 
class cltOrderData {
public:
    cltOrderDataInfo ODI;

	SI32	CharUnique;					 // ������ �����ؾ� �� ĳ������ CharUnique
	
	cltOrderData();

	void Make(SI32 order, SI32 charunique, SI32 para1, SI16 para2, SI16 para3, SI16 para4, SI08 hostmode);
	void Make(SI32 charunique, cltOrderDataInfo* pclinfo);
	
};

class cltShapeEquipInfoUnit{
public:
	UI32	uiUnique:	16;  // 32767 ������.
	UI32	uiRare:		4; // 15 ������.
	UI32	uiEnchantPower:	4;	// 15 �������� ���� ����
	UI32	uiEnchantType: 4;	// 15������ Ÿ���� ���� ����
	UI32	uiReserved:	4;
	
	cltShapeEquipInfoUnit();
	cltShapeEquipInfoUnit(UI32 unique, UI32 rare , UI32 enchanttype , UI32 enchantlevel );
	
	void Init();
	void Set(cltShapeEquipInfoUnit* pclinfo);
	
};
// �ܺ� ���ǰ ����.
class cltShapeEquipInfo{
public:
	cltShapeEquipInfoUnit clArmour;
	cltShapeEquipInfoUnit clHelmet;
	cltShapeEquipInfoUnit clMantle;
	cltShapeEquipInfoUnit clWeapon;

	UI32	uiSwordScroll:	1;
	UI32	uiSpearScroll:	1;
	UI32	uiAxeScroll:	1;
	UI32	uiGunScroll:	1;
	UI32	uiBowScroll:	1;
	UI32	uiStaffScroll:	1;
	UI32	uiCannonScroll:	1;

	SI08	siSwordScrollNum;
	SI08	siSpearScrollNum;
	SI08	siAxeScrollNum;
	SI08	siGunScrollNum;
	SI08	siBowScrollNum;
	SI08	siStaffScrollNum;

	SI16	siCompletedSetItemIndex;
	SI16	siCompletedSetItemIndexStep;

	SI16	siCompletedSetItemIndex2;
	SI16	siCompletedSetItemIndexStep2;

	SI16	siCompletedSetItemIndex3;
	SI16	siCompletedSetItemIndexStep3;

	SI16	siCompletedSetItemIndex4;
	SI16	siCompletedSetItemIndexStep4;

	SI16	siCompletedSetItemIndex5;
	SI16	siCompletedSetItemIndexStep5;
	
	cltShapeEquipInfo();
	cltShapeEquipInfo(cltShapeEquipInfoUnit* pclArmour, cltShapeEquipInfoUnit* pclHelmet, cltShapeEquipInfoUnit* pclMantle, cltShapeEquipInfoUnit* pclWeapon,
						bool bsword, bool bspear, bool baxe, bool bgun, bool bbow, bool bstaff, bool bcannon,
						SI08 SwordScrollNum, SI08 SpearScrollNum, SI08 AxeScrollNum,
						SI08 GunScrollNum, SI08 BowScrollNum, SI08 StaffScrollNum,
						SI16 CompletedSetItemIndex,
						SI16 siCompletedSetItemIndexStep,
						SI16 CompletedSetItemIndex2,
						SI16 siCompletedSetItemIndexStep2,
						SI16 CompletedSetItemIndex3,
						SI16 siCompletedSetItemIndexStep3,
						SI16 CompletedSetItemIndex4,
						SI16 siCompletedSetItemIndexStep4,
						SI16 CompletedSetItemIndex5,
						SI16 siCompletedSetItemIndexStep5);
	void Init();
	void Set(cltShapeEquipInfo* pclinfo);
	
};

//
#define CHANGETYPE_POISON					1
#define CHANGETYPE_ICE						2
#define CHANGETYPE_DRAIN					3
#define CHANGETYPE_HEAL						4
#define CHANGETYPE_NOFIRSTATTACK			5	// ���� ���� ȿ��. 
#define CHANGETYPE_NOICEATTACK				6	// ���� ���� ȿ��. 
#define CHANGETYPE_CURSEATTACK				7	// 
#define CHANGETYPE_CURSECRITICAL			8
#define CHANGETYPE_CURSEDEFENSE				9	// 
#define CHANGETYPE_CURSEMOVESPEED			10
#define CHANGETYPE_ANGEL					11	// ��ȣõ�� ���� ����. 
#define CHANGETYPE_AUTOFOOD					12	// �ڵ� ������ ���� ���� ����. 
#define CHANGETYPE_TOPSKILLUNIQUE			13	// �ְ������� ��� 
#define CHANGETYPE_MAGICFIRE				14	// ����ġ�� ���� ������ ��
#define CHANGETYPE_FROZEN					15	// ������ ������ �� ����
#define CHANGETYPE_POLICE					16	// ���� ���� ����
#define CHANGETYPE_INCREASEMOVESPEED		17	// �̵��ӵ����� ����
#define CHANGETYPE_HEARTEFFECT				18	// ��Ʈ ����Ʈ ����
#define CHANGETYPE_FOURLEAF					19	// ���� Ŭ�ι� ����Ʈ
#define CHANGETYPE_TRANSFORM100EFFECT		20	// ���� 100% ����Ʈ
#define CHANGETYPE_TRANSFORMBUF				21	// ���� 100% ����Ʈ
#define CHANGETYPE_EMBLEM					22	// ������� ���� - ����������
#define CHANGETYPE_DISPLAYLOVELEVEL			23	// �������� ������ ������ ����Ǿ���
#define CHANGETYPE_TRANSFORM_HORSE			24	// ���� ���� - Ż���� ����
#define CHANGETYPE_STUN						25	// ���� 
#define CHANGETYPE_SUMMONNOFIRSTATTACK		26	// ��ȯ�� ���� ���� ȿ��.
#define CHANGETYPE_NOFIRSTATTACKPREMIUM		27	// ���� ���� ȿ�� �����̾�
#define CHANGETYPE_POISON_SETITEM_EFFECT	28	// ��Ʈ ���������� ���� ��ȿ��.

//----------------------------------------
// ĳ���͵��� ���� �������� ���� ����
//-----------------------------------------
class cltCharChangeStatusInfo{
public:
	SI32	siChangeType;
	bool	bChangeSwitch;
	SI32	siChageAmount1;
	SI32	siChageAmount2;
	SI32	siChageAmount3;

	cltCharChangeStatusInfo ();
	cltCharChangeStatusInfo (SI32 changetype, bool bswitch, SI32 amount1 = 0, SI32 amount2 =0, SI32 amount3 = 0 );
	
	void Set(cltCharChangeStatusInfo *pclInfo);
	
};

//----------------------------------------
// ĳ���͵��� �ΰ��������� ���� ����
//-----------------------------------------
class cltCharAddStatusInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, 2>	siReserved;
#else
	SI16			siReserved[2];
#endif

/*	SI08			siIdentity;
	SI16			siHome;							// ��Ȳ���� ��� �ڽ��� �����ϴ� ������ Unique ��
	SI16			siCapa;							// ��Ȳ���� �������� 

	SI32			siChildCharUnique;				// ��ȯ���� ���� �ִٸ� ��ȯ���� Unique
	SI32			siParentCharUnique;				// �ڽ��� ��ȯ���̶�� �θ��� Unique
	SI32			siParentPersonID;				// �ڽ��� ��ȯ���̶�� �θ��� PersonID : Ȯ���ϰ� ���ϱ� ���ؼ���
	SI16			siSummonIndex;					// �ڽ��� ��ȯ����� ��ȯ�� Index	
*/

	cltCharAddStatusInfo ();

//	cltCharAddStatusInfo (SI08 identity, SI16 home, SI16 capa, SI32 childunique, SI32 parentunique, SI32 parentperson, SI16 summonindex);

	void Init();
	void Set(cltCharAddStatusInfo *pclInfo);
	
};

//------------------------------------
// Event ĳ���� ���� ����. 
//------------------------------------
class cltEventCharInfo{
public:
	UI32 bEvent1Switch:		1;
	UI32 bEvent2Switch:		1;
	UI32 bEvent3Switch:		1;
	UI32 bEvent4Switch:		1;
	UI32 bEvent5Switch:		1;
	UI32 bEvent6Switch:		1;
	UI32 bEvent7Switch:		1;

	cltEventCharInfo();
	
	void Init();
	void Set(cltEventCharInfo* pclinfo);
	
};

//----------------------------------------
// ĳ������ ���� Status ���� 
//-----------------------------------------
#define ENEMYTYPE_ALLPC							0x0001	// ��� ����ڿ� ������. 
#define ENEMYTYPE_ALLNPC						0x0002	// ��� NPC�� ������ 
#define ENEMYTYPE_VILLAGE						0x0004	// �������� ������ ������. 
#define ENEMYTYPE_ONEVILLAGE					0x0008	// Ư�� �� �������� ������ (para1�� Ư�� ������ ����ũ ����) 
#define ENEMYTYPE_GUILDWAR						0x0010	// ����� - para1�� �ο�� ����� ����ũ
#define ENEMYTYPE_PVP							0x0020	// PVP ĳ����

#define PVPTYPE_ALL	  0	// ��� PVP���� ����
#define PVPTYPE_GREEN 1 // �������� PVP���� ����

class cltEnemyTypeInfo{
public:
	SI16	siEnemyType;
	SI16	siPara1;

	cltEnemyTypeInfo();
	void Init();
	void Set(cltEnemyTypeInfo* pclinfo);
	void Set(SI16 type, SI16 para1);
	
	bool IsType(SI32 type);
	
};

#define INNERSTATUS_POISON						1
#define INNERSTATUS_ICE							2
#define INNERSTATUS_CURSEATTACK					4
#define INNERSTATUS_CURSECRITICAL				8
#define INNERSTATUS_CLONE						16
#define INNERSTATUS_PRISON						32
#define INNERSTATUS_CURSEDEFENSE				64
#define INNERSTATUS_CURSEMOVESPEED				128
#define INNERSTATUS_MAGICFIRE					256		// ����ġ ������ ���ؼ� �� ����
#define INNERSTATUS_FROZEN						512		// ������ ���� ������ �¾Ƽ� ������
#define INNERSTATUS_INCREASEMOVESPEED			1024	// �̵��ӵ� ���� ����
#define INNERSTATUS_STUN						2048	// ����
#define INNERSTATUS_POISON_SETITEM_EFFECT		4096	// ��Ʈ ���������� ����� ��ȿ��.

class cltInnerStatusInfo{	
public:
	SI32	siStatus;
	cltEnemyTypeInfo		clEnemyType;		// ���� ���� ����. 
	cltEventCharInfo		clEventInfo;		// ĳ������ �̺�Ʈ ���� ����. 
	
	
	SI16	siTopSkillUnique;					// �ְ� ������ ����� ����ũ 

	cltInnerStatusInfo();

	cltInnerStatusInfo(SI32 status,	cltEnemyTypeInfo* pclenemytype, 
		cltEventCharInfo* pclevent,
		SI16 topskillunique);
	
	void Set(cltInnerStatusInfo* pclinfo);
	void Init();
	
	bool IsStatus(SI32 status);
	void SetStatus(SI32 status);
	void DelStatus(SI32 status);
	

};

//--------------------------------
// ĳ���� ���̵�Ƽ ���� 
//--------------------------------
class cltIdentityCommonInfo{
public:
	SI16	siPara1;
	SI16	siPara2;
	SI32	siPara3;
};

class cltIdentitySummonInfo{
public:
	SI16	siMySummonIndex;
	SI16	siParentCharUnique;
	SI32	siParentPersonID;
};

class cltIdentityVillageGodInfo{
public:
	SI16	siCapa;
};

class cltIdentityVillageHuntInfo{
public:
	SI16	siHome;
	SI16	siPara2;
};

class cltIdentityBlackArmyInfo
{
public:
	SI16	siWarMapIndex;
	SI16	siMapIndex;
};

class cltIdentityVillageKingMonument
{
public:
	SI32	siVillageUnique;	
};

class cltIdentityMofuMofuEvent
{
public:
	SI16	siField;
	SI16	siIndex;
	SI32	siCanAttack;
};

class cltIdentityInstanceMap
{
public:
	SI16	siPara1;
	SI16	siPara2;
	SI32	siScore;
};

class cltIdentityQuestInfo
{
public:
	SI32	m_siEnemyPersonID;
	SI32	m_siEnemyCharID;	// Ŭ���̾�Ʈ��
};

class cltIdentitySoulBound
{
public:
	SI32	m_siPersonID;		// �ͼ��� ID
};

class cltIdentitySoulGuard
{
public:
	SI16	m_siParentCharUnique;
	SI32	m_siParentPersonID;
};

class cltIdentity{
public:
	SI08	siIdentity;

	union
	{
		cltIdentityCommonInfo			clCommonInfo;
		cltIdentitySummonInfo			clSummonInfo;
		cltIdentityVillageGodInfo		clGodInfo;
		cltIdentityVillageHuntInfo		clHuntInfo;
		cltIdentityBlackArmyInfo		clBlackArmyInfo;
		cltIdentityVillageKingMonument	clKingMonument;
		cltIdentityMofuMofuEvent		clMofoMofuEvent;
		cltIdentityInstanceMap			clInstanceMap;
		cltIdentityQuestInfo			clQuestInfo;
		cltIdentitySoulBound			clSoulBound;
		cltIdentitySoulGuard			clSoulGuard;
	};

	cltIdentity();
	cltIdentity(SI08 identity, SI16 para1, SI16 para2, SI32 para3);
	void Init();
	void Set(cltIdentity* pclinfo);
	
	void MakeSummon(SI32 iddentity, SI32 index, SI32 parentcharid, SI32 parentpersonid);	// ��ȯ���� �����Ѵ�. 
	void MakeGod(SI32 iddentity, SI32 capa);												// ����, ��Ȳ��, ��ȯ�ż�ȯ���� �����Ѵ�.
	void MakeHunt(SI32 iddentity, SI32 villageunique);										// �Ұ��縮�� �����Ѵ�.
	void MakeBlackWar(SI32 iddentity);
	void MakeBlackArmy(SI32 iddentity, SI16 WarMapIndex, SI16 MapIndex);
	void MakeKingMonument(SI32	iddentity, SI32 villageunique);								// ���� ��ȣ���� �����Ѵ�.
	void MakeMofuMofuEvent(SI32 iddentity, SI16 siField, SI16 siIndex, SI32 siCanAttack);
	void MakeInstanceMap(SI32 iddentity, SI32 siScore);
	void MakeMonsterAttackEvent( SI32 iddentity );											//[����] ���� �̺�Ʈ - ���� ���� => 2008-3-26
	void MakeQuest( SI16 identity, SI32 siEnemyPersonID, SI32 siEnemyCharID );
	void MakeSoulBound( SI16 identity, SI32 siPersonID );									// �ͼ� �Ӽ� : �ҷ��� PersonID�� �����Ҽ�����
	void MakeSoulGuard( SI08 identity, SI16 siParentCharUnique, SI32 siParentPersonID );	// [����] ��ȣ����_090525 : 
	void MakeTreasureEvent( SI32 iddentity );												// [����] Ʈ���� �̺�Ʈ 
	void MakeRaccoonDodgeEvent( SI32 iddentity );											// [����] ���� ���ϱ� �̺�Ʈ
	void MakeBBBEventMonster( SI32 identity );												// [����] ���Һ� �̺�Ʈ�� ��
	void MakeSoulGuard_Summon( SI32 identity );												// [����] 
	void MakeBattleRoyalMob( SI32 siIdentity);												// [����] ��Ʋ�ξ� �� 
	void MakeGMCreateMonster( SI32 siIdentity);												// [���] CMT���ɾ�� ������ ����
	void MakeGiantRaccoonResurrectEvent( void );											// [����] ���̾�Ʈ ���� ��Ȱ ���� �̺�Ʈ ����. 

};

#define MAKECHARTYPE_NPC		0
#define MAKECHARTYPE_PC			1

#define PVP_LEAGUE_MAX_LATEST_MATCH_PERSON 2

class cltPVP_LeagueInfo
{
public:
	SI16	m_siEnemyPersonID	;	// PVP�������� ��
	SI16	m_siWin	;	// ���� ���
	SI16	m_siDraw	;	// ���� ���º� ���
	SI16	m_siLose	;	// ���� �й� ���
	SI16	m_siStraight_Win	;	// ���� ���
	SI16	m_siStraight_Draw	;	// ���� ���º� ���
	SI16	m_siStraight_Lose	;	// ���� �й� ���
	SI16	m_siGrade	;			// ���
	SI16	m_siStraight_Win_Grade	;	// ��޻���� ���� ���� ��� 
	SI16	m_siStraight_Lose_Grade	;	// ����϶��� ���� ���� ��� 

	SI16	m_siLatestPersonID[PVP_LEAGUE_MAX_LATEST_MATCH_PERSON];	// �ֱ� ���� �ο� ���

	SYSTEMTIME	stLastPVPTime	;

public:
	cltPVP_LeagueInfo()		{	Init();											}
	~cltPVP_LeagueInfo()	{}

	void Init( void )		{	ZeroMemory( this, sizeof(cltPVP_LeagueInfo) );	}
	void Set( cltPVP_LeagueInfo* pclLeagueInfo )
	{
		if ( pclLeagueInfo )
		{
			memcpy( this, pclLeagueInfo, sizeof(cltPVP_LeagueInfo) );
		}
	}
};

enum ValentineWarStatus
{
	VALENTINEWAR_NONE	= 0,
	VALENTINEWAR_COUPLE,
	VALENTINEWAR_SOLO
};
class cltValentineWarInfo
{
public:
	SI16	m_siCouple	;		// Ŀ������ �ƴ��� ���� ���� ���ߴ���
	bool	m_bPrize		;		// ��ǰ�� �޾Ҵ��� ���� �ʾҴ���.
	SYSTEMTIME	m_stAnswerTime	;	// ������ �亱�� �ð�

public:
	cltValentineWarInfo()		{	Init();											}
	~cltValentineWarInfo()	{}

	void Init( void )		{	ZeroMemory( this, sizeof(cltValentineWarInfo) );	}
	void Set( cltValentineWarInfo* pclValentineWarInfo )
	{
		if ( pclValentineWarInfo )
		{
			memcpy( this, pclValentineWarInfo, sizeof(cltValentineWarInfo) );
		}
	}
	SI16 GetCouple(){ return m_siCouple ;}
	void SetCouple( SI16 siCouple){ m_siCouple = siCouple;}

	bool GetPrize(){ return m_bPrize ;}
	void SetPrize( bool siPrize){ m_bPrize = siPrize;}

	SYSTEMTIME GetAnswerTime(){ return m_stAnswerTime ;}
	void SetAnswerTime( SYSTEMTIME stAnswerTime){ m_stAnswerTime = stAnswerTime;}
};

//----------------------------------------
// ĳ���͸� ���鶧 ����ϴ� ����. 
//-----------------------------------------
class cltMakeCharInfoCommon{
public:
	UI32	uiType:					1;		// NPC���ΰ� ����� ���ΰ�. 
	UI32    uiMakeCharKind:			16;		// ����� ������ ĳ���� ����. 
	UI32    uiMakeCharUnique:		16;		// �� �޽����� ����� ��������� �� ĳ������ CharUnique
	UI32	uiLevel:				8;		// (0~255)
	UI32	uiLife:					20;		// ���� �� ĳ������ ü��. 
	UI32	uiX:					10;
	UI32	uiY:					10;
	UI32	uiMapIndex:				10;		// ���� �ε��� (0~2^10 = 1024)
	UI32	uiTotalMoveSpeed:		6;	
	SI08	siHomeVillageUnique;		// �ּ��� ������ ����ũ. 
	SI16	siJimCharUnique;			// �� ĳ���͸� ���� ĳ������ ����ũ. 
	cltOrderDataInfo	clOrder;		// ĳ���Ͱ� ������� �� ���ؾ� �� �ӹ�. 		
	cltIdentity			clIdentity;		// ���̵�Ƽ ����. 
	cltInnerStatusInfo	clInnerStatus;	// ĳ������ �߰����� (��, ����)
	cltMakeCharInfoCommon();
	
	cltMakeCharInfoCommon(UI32 makekind, UI32 makecharunique, SI32 home, UI32 level, 
		UI32 life, UI32 x, UI32 y, UI32 mapindex, UI32 speed, SI16 jimcharunique, 
		cltOrderDataInfo* pclorder,
		cltIdentity* pclidentity, 
		cltInnerStatusInfo* pclinnerstatus);
	
	void Init();
	void Set(cltMakeCharInfoCommon* pclinfo);
	

};

class cltMakeCharInfoUser{
public:

};

//[����] ���� �ߵ� ���. => 2008-6-11
enum
{
	BUF_GET_TYPE_NONE				// ���...	
	,BUF_GET_TYPE_BUFROULETTE		// ���� �귿�� �̿���.
	,BUF_GET_TYPE_NEWYEAREVENT		// �ų��̺�Ʈ �귿�� �̿���.
	,BUF_GET_TYPE_RUMBLECHERRY		// ����ü�� �������� �̿���.
	,BUF_GET_TYPE_FORTUNECOOKIE		// ������Ű �������� �̿���.
	,BUF_GET_TYPE_NIGHTMARKET		// �߽��� ���� ����
};

// ���� ����  - ���� ���� �ٲ�� �ȵ�. DB�� ����Ǵ� ��
// �ΰ��� �̻��� ȿ��, Ư�� ȿ���ϰ�쿣 ���ο� ���� Ÿ�� �����
// �ٸ� ������ �ߺ��Ǿ� �������� �� �ٸ� ȿ���� ��Ÿ���� ��� ���� ���ο� ����Ÿ�� �ʿ�
enum
{
	BUF_ATTACKUP = 1,
	BUF_DEFENSEUP,
	BUF_SPEEDUP,
	BUF_MAKEEXPUP,									// �������ʽ� Ÿ�Ӱ� ��ø�ȴ�.
	BUF_NOFIRSTATTACK,
	BUF_SUMMONATACUP,
	BUF_LOVEPOWER,
	BUF_FRIENDSHIP,
	BUF_HINAMOCHI,
	BUF_SUCCESS_MANUFACTURE,
	BUF_MOFUMOFU_PERFUME,
	BUF_FOURLEAF,
	BUF_PREMIUMEXPUP,
	BUF_HAPPYBEAN,
	BUF_ATTACKUP30,									//������ ������ 30�ϰ�[2007.08.30 �ռ���]
	BUF_DEFENSEUP30,								//���� ������ 30�ϰ�[2007.08.30 �ռ���]
	BUF_SUPEREXPUP,
	BUF_NOTBONUSTIME_MAKEEXPUP,						// �������ʽ� Ÿ�ӿ� ��ø���� �ʴ´�.
	BUF_THANKSGIVING_DAY,							
	BUF_FARMINGNMININGFOREVER,						//  ä���� ���� ������ 

	BUF_STATUE_UP_STR,								//[����] ��		+ a	=> 2008-6-18
	BUF_STATUE_UP_DEX,								//[����] ���߷�	+ a	=> 2008-6-18
	BUF_STATUE_UP_MAG,								//[����] ����	+ a	=> 2008-6-18			
	BUF_STATUE_UP_VIT,								//[����] ������	+ a	=> 2008-6-18
	BUF_STATUE_UP_HAND,								//[����] ������ + a	=> 2008-6-18
	BUF_EXP_UP,										//[����] ����ġ + a	=> 2008-6-18	

	BUF_REWARDBONUS,								//[����] �̰� ���� ������
	BUF_OLYMPICMEDAL,
	BUF_EVENT_EGGMOONCAKE,							//[����] ���׿���.	=> 2008-8-14
	BUF_TRANSFORM,									//[����] ���Ź���

	BUF_MENTOR,										//[����] ����, ���� ����.	2008-10-8						
	BUF_PREMIUMMAKEEXPUP,							//[���] ���������̾� ����  2008-10-15
	BUF_SPECIALAMPUL_EXPUP300,						//[����] ��������ġ 300% �߰� ����. 2009-02-26 (����ȿ���)
	BUF_QUEST_ANTIPOTION,							//[����] �Ƽ����� ������ 30%����

	BUF_EVENT_FINDTREASURE,							//[����] ����ã�� �̺�Ʈ
	BUF_QUEST_ATTACK,								//[���] ��Ƽ��� ���ݽ� 30%���ݷ� ����
	BUF_WATERMELON,									//[���] ���ڹ��� - ������ ���� 5%�� Ȯ���� ��󿡰� �� �������� �ѹ� �� �ְԵ�.(�߰�Ÿ ����)
	BUF_TRANSFORM_HORSE,							//[���] Ż�ͺ��Ź��� (08.09.11)

	BUF_STATUE_UP_WIS,								//[����] ����	+ a	=> 2009-10-22 //2010
	BUF_EVENT_FORTUNE_BAD,							//[����] ������Ű - ���� ǥ���ϱ� ���� ����(���� ȿ���� ����)

	BUF_CHRISTMASEVENT_SNOWBLESS,					//[���] ���� �ູ ����
	BUF_CHRISTMASEVENT_DECORATION,					//[���] ũ�������� ��� ����
	
	BUF_GIANTRACCOON_RESURRECT_EVENT,				//[����] ���̾�Ʈ ���� �̺�Ʈ ����.
	BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE,	//[����] ���̾�Ʈ ���� �̺�Ʈ �ǹ� ���� �ϼ� ����.

	BUF_RESIDENTS_EXPUP_10,				// [����] ���� EXP ĸ�� - ���� �Ҽӵ� ������ �ֹε鿡�� ����ġ 10%���� ȿ��.
	BUF_RESIDENTS_STRONG,				// [����] ���� ��ȭ�� - ���� �Ҽӵ� ������ �ֹε鿡�� ���� ����/���� ���ݷ�, ���� �������� + 10% ȿ��. 

	BUF_VALENTINEDAY,								// �߷�Ÿ�� ����
	
	BUF_END
};

#define MAX_BUF	BUF_END - 1

class stBuf
{
public:
	SI16	siBufType;			//������ ȿ��
	SI16	siPercent;			//���� ȿ�� ( �ۼ�Ʈ�� �����Ҷ� )
	SI16	siGetType;			//[����] ���� Ÿ������ ���� �̸� ����. => 2008-6-11
	SI32	siOverTime;			//���� ���� �ð� ( ���ӽð��� ������ ȯ�� )
	//	bool	bOverlap;			//��ø�� ��������.					- ���� by LEEKH 2008.03.08


	stBuf()
	{
		siBufType		= 0;
		siPercent		= 0;
		siOverTime		= 0;
		siGetType		= 0;
	}

	stBuf(SI16 BufType, SI16 Percent, SI32 OverTime, SI16 _siGetType = 0)
	{
		siBufType	= BufType;
		siPercent	= Percent;
		siOverTime	= OverTime;
		siGetType	= _siGetType;
	}

	void Set(stBuf* pBuf)
	{
		if( pBuf == NULL )		return;

		siBufType	= pBuf->siBufType;
		siPercent	= pBuf->siPercent;
		siGetType	= pBuf->siGetType;
		siOverTime	= pBuf->siOverTime;
	}
};


#define MAX_BUF_COUNT		12

class cltBufInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<stBuf, MAX_BUF_COUNT>		m_stBuf;
#else
	stBuf	m_stBuf[MAX_BUF_COUNT];
#endif

	// ������ ��뿩�θ� ǥ���ϴ� ��Ʈ�迭
	NBitArray<BUF_END>				m_bitBufUse;

	cltBufInfo();

	// �������� ������ �����̾�����
	bool m_bNoFirstAttackPreminum;

	void Init();
	void SetBuf( cltBufInfo* pclinfo );
	SI16 FindEmptySlot( SI16 type, bool overlap = false);
	void AddBuf( SI16 index, stBuf buf );
	void DeleteBuf( SI16 index );
	bool CanUseBuf( SI16 type );
	SI32 CalcTotalBufValue( SI16 type, SI32 currentValue );
	bool IsHaveBuf(SI16 type);
	// ���� Ÿ�԰� ��Ÿ���� ������
	bool IsHaveBufWithGetType( SI16 siFindBufType, SI16 siFindGetType );
	// ���� Ÿ�������� ��Ÿ���� �������� �ȵ�
	bool IsHaveBufExecptGetType( SI16 siFindBufType, SI16 siFindGetType );
	SI32 GetBufPercent(SI16 type);
	SI32 GetBufOvertime(SI16 type);	
	stBuf* GetBuf(SI16 type);
	// [����] ���� siType�� ���� Ÿ���� �ִ� ����(�迭��ȣ)�� ã�´� - 2008.01.23
	SI16 FindBufSlot( SI16 siType );

	// [����] ������ Ư���ϰ� ��Ե� Ÿ���� ���� ã��. 
	SI16 FindBufGetTypeSlot( SI16 GetType );

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};



class cltPVP_LeagueInfo;
class cltValentineWarInfo;

class cltMakeCharInfo : public cltMakeCharInfoCommon{
public:
	//--------------------------------
	// ����� ���� ���� 
	//--------------------------------
	UI32	uiChildCharUnique:		16;			// ������ ��ȯ���� ����ũ. 
	UI32	uiSummonIndex:			3;			// ���� ��ȯ���� �ε���. 
	UI32	uiHeadChatSwitch:		1;			// �Ӹ����� �ִ°� ?
	UI32	uiPersonalShopSwitch:	1;			// ���λ����� �ִ°�?	
	UI32	uiNoFirstAttackSwitch:	1;			// ���� �� �����Ǿ� �ִ°�?
	UI32	uiNoIceAttackSwitch:	1;			// ICE �� �����Ǿ� �ִ°�?
	UI32	uiHeartEffectSwitch:	1;			// ��Ʈ ����Ʈ�� ���� �Ǿ� �ִ°�? ( ���,���� ������ ��ġ�� ����� ���� )
	UI32	uiFourLeafSwitch:		1;			// ���� Ŭ�ι� ����Ʈ�� ���� �Ǿ� �ִ°�?
	UI32	uiCusionSwitch:			16;			// ����(�漮)�� �ٴڿ� ����ִ°�.
	UI32	uiSex:					1;			// ����. 


	UI32	uiHorseUnique1:			8;		// ���� ����.
	UI32	uiSchoolUnique:			10;		// �б��� ����ũ. 
	UI32	uiRequestAddinfo:		1;		// �ź��� ���� �߰����� ��û ����
	UI32	uiTicketWarpSwitch:		1;		// �����̿���� ����� �����̵��ΰ�. 
	UI32	uiBestGuild:			1;		// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
	UI32	uiElectionStump:		1;		// [����] ���� ���� ���ΰ�
	UI32	uiElixir:				1;		// [����] ������ ������ΰ�.
	UI32	uiRepairShopSwitch:		1;		// [����] �����Ƿ� ���ΰ�.


	SI08	siCurrentShowHorse; //( 0~4 ) �� 5������ �ε���.
	SI08	siGameMasterLevel;			// ��� ���� �ܰ�. 
	SI16	siMapUnique;				// ������ũ. 

	cltShapeEquipInfo clShapeEquip;
	
	cltSimpleRank		clRank;

	SI32				siAddStatusDate;
	SI32				siStatusTicketNum;

	UI08				uiGMMode;		// ��� ��� ����
	bool				bPCRoom;		// pc�� �����ΰ�
	SI08				siMakeAniType;	// ���� ���ϸ��̼� ��� ����
	cltSimpleGuildRank	clGuildRank;	// ���
	cltPersonNaming		clPersonNaming;
	bool 				siCurrentHorseRIDEHORSE;

	UI16				uiNowTransFormKind;
	bool				bTransFormMode;
	bool				bTransForm100Effect;
	SI16				siBufTransformKind;
	SI16				siBufTransformHorseKind;
	SI16				siEnemyUnique;		// ���� PVP �� ����ũ
	cltValentineWarInfo	clValentineWarInfo	;
	cltPVP_LeagueInfo	clPVP_LeagueInfo;
	// �����̾� ���� ����
	SI32				siPremiumPartsDate_Slot1;
	SI32				siPremiumPartsUnique_Slot1;
	SI32				siPremiumPartsDate_Slot2;
	SI32				siPremiumPartsUnique_Slot2;
	SI32				siPremiumPartsDate_Slot3;
	SI32				siPremiumPartsUnique_Slot3;
	SI32				siPremiumPartsNum;

	UI08				uiWeaponSkillMasterEmblem;	// ������� ���� - ������

	SI16				siDisplayLoveLevel;			// ���鿡�� �������Ե� ������

	SI16				siSoulGuardUnique;			// �� ��ȣ���� ����ũ

	cltBufInfo			m_clBufInfo;				// [����] ���������� ��´�.

	//KHY - 1001 - ��µ��� 5���� ���� ����.
	//SI08 horseindex,
	cltMakeCharInfo();

	cltMakeCharInfo(UI32 					uikind, 
					UI32 					makecharunique, 
					UI32 					level, 
					UI32 					life, 
					UI32 					x, 
					UI32 					y, 
					UI32 					mapindex, 
					UI32 					childcharunique, 
					UI32 					summonindex, 
					UI32 					bheadchatswitch, 
					UI32 					bpersonalshopswitch, 
					UI32 					bnofirstattackswitch,
					UI32 					bnoiceattackswitch,
					UI32 					bhearteffectswitch,
					UI32 					bfourleafswitch,
					UI32 					bfarmminingswitch,//��� ���� ����
					UI32 					bcushionswitch,
					UI32 					bsex,
					cltShapeEquipInfo*		pclequipinfo, 
					SI08 					CurrentShowHorse, 
					UI32 					totalmovespeed, 
					SI16 					jimcharunique, 
					SI08 					gamemasterlevel, 
					SI16 					mapunique, 
					UI32 					horseunique1, 
					UI32 					homevillageunique, 
					UI32 					schoolunique,
					cltSimpleRank*			pclrank,
					SI32 					addstatusdate,
					SI32 					ticketnum, 
					cltOrderDataInfo*		pclorder,
					cltIdentity*			pclidentity,
					cltInnerStatusInfo* 	pclinnerstatus, 
					UI32 					requestaddinfo,
					UI32 					ticketwarp,
					UI08 					gmmode ,
					bool 					pcroom,
					SI08 					makeanitype,
					cltSimpleGuildRank*		pclguildrank,
					cltPersonNaming*		pclpersonnaming,
					bool 					CurrentHorseRIDEHORSE,
					UI16 					NowTransFormKind,
					bool 					TransFormMode,
					UI32 					uiBestGuild,					// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
					bool 					bIsElectionStump,
					bool 					TransForm100Effect,
					SI16 					siBufTransform,
					SI16 					EnemyUnique,					// ���� PVP - ������ũ
					cltValentineWarInfo*	pclValentineWarInfo,
					bool					bElixir,
					cltPVP_LeagueInfo*		pclPVP_Leagurinfo,
					SI32 					addPremiumPartsdate_Slot1,
					SI32 					addPremiumPartsUnique_Slot1,
					SI32 					addPremiumPartsdate_Slot2,
					SI32 					addPremiumPartsUnique_Slot2,
					SI32 					addPremiumPartsdate_Slot3,
					SI32 					addPremiumPartsUnique_Slot3,
					SI32 					PremiumPartsNum,
					UI08 					weaponSkillMasterEmblem,
					SI16 					displayLoveLevel,
					SI16 					soulGuardUnique,
					bool 					bRepairShopSW,
					cltBufInfo*				_pclBufInfo
		);					
	
	void Init();
	void Set(cltMakeCharInfoCommon* pclinfo);
	void Set(cltMakeCharInfo* pclinfo);
};

const SI16 MAX_PUPILNUMBER = 10;

class cltPFatherInfo{
public:
	cltSimplePerson	clFather;			// ���� ����. 
	TCHAR			szAccountID[MAX_PLAYER_NAME];
	SI08			siMyLevelForFather;	// ������ ������ ����� �� ���� 
	SI08			siFatherChangeNumber;	// ������ ������ Ƚ��. 
	SI32		    siTeacherPoint; // ���� ����Ʈ
	bool		    bOverTake;		// û���� ������ �޾Ҵ°�?
    
	cltPFatherInfo();
	void Init();
	void Set(cltPFatherInfo* pclinfo);

	void SetTeacherPoint( SI32 TeacherPoint )
	{
		siTeacherPoint = TeacherPoint;
	}
	SI32 GetTeacherPoint()
	{
		return siTeacherPoint;
	}

	void SetOverTake( bool flag )
	{
		bOverTake = flag;
	}
	bool IsOverTake()
	{
		return bOverTake;
	}
};

class cltVillageScoreInfo
{
public:
	// ���� ���� �� �����Ѵ�!!
	SI32		siPlayTime;				// ���� ���� �� �÷��� �ð�
	SI08		siLevelUpCount;			// ���� ���� �� ���� Ƚ��
	SI32		siWarKillNum;			// ���� ���� �� ������
	SI16		siMakeRareItemCount;	// ���� ���� �� ��ǰ ���� Ƚ��
	SI16		siMonthlyScore;			// ���� ����
	SI32		siTotalScore;			// ��Ż ����

	cltVillageScoreInfo()
	{
		Init();
	}

	void Init()
	{
		siPlayTime = 0;				// ���� ���� �� �÷��� �ð�
		siLevelUpCount= 0;			// ���� ���� �� ���� Ƚ��
		siWarKillNum= 0;		// ���� ���� �� ������
		siMakeRareItemCount= 0;	// ���� ���� �� ��ǰ ���� Ƚ��
		siMonthlyScore= 0;	// ���� ����
		siTotalScore= 0;	// ��Ż ����
	}

	void MonthlyInit()
	{
		siPlayTime = 0;
		siLevelUpCount = 0;
		siWarKillNum = 0;
		siMakeRareItemCount = 0;
		siMonthlyScore = 0;
	}

	void Set(cltVillageScoreInfo* pclinfo)
	{
		siPlayTime = pclinfo->siPlayTime;
		siLevelUpCount = pclinfo->siLevelUpCount;
		siWarKillNum = pclinfo->siWarKillNum;
		siMakeRareItemCount = pclinfo->siMakeRareItemCount;
		siMonthlyScore = pclinfo->siMonthlyScore;
	}

};

class CEmpireInfo
{
public:

	SI08	m_siEmpireRank;		// �������� ���� ��ũ
	SI32	m_siEmpireUnique;   // ���� ���� ����ũ
	
public:

	enum
	{
		/// �ҼӾ���
		EMPIRE_NONE = 0,
		/// ������ ���� �Ϲ���
		EMPIRE_PERSON,
		/// ������ ����
		EMPIRE_EMPEROR,
		/// ������ �Ҹ���( ������ ���� ���� ����� )
		EMPIRE_LITTLE_EMPEROR
	};

	CEmpireInfo() : m_siEmpireRank(0), m_siEmpireUnique(0) {};
	~CEmpireInfo() {}

	void SetEmpireInfo( SI08 RankType, SI32 EmpireUnique )
	{
		m_siEmpireRank = RankType;
		m_siEmpireUnique = EmpireUnique;
	}
	void SetEmpireInfo( CEmpireInfo*  EmpireInfo )
	{
		if( EmpireInfo != NULL )
		{
			m_siEmpireRank = EmpireInfo->GetEmpireRank();
			m_siEmpireUnique = EmpireInfo->GetEmpireUnique();
		}
		else
		{
			m_siEmpireRank = 0;
			m_siEmpireUnique = 0;
		}
	}

	SI08 GetEmpireRank()
	{
		return m_siEmpireRank;
	}

	SI32 GetEmpireUnique()
	{
		return m_siEmpireUnique;
	}
};

//KHY - 1220 - ���Žý���.
//KHY - 1111 - ������� �߰�.
//�ð� ������  ����  - ���� ���� �ٲ�� �ȵ�. DB�� ����Ǵ� �� - index ����, type���� �̴�.
enum
{
	TIMEMETERRATE_TRANSFORM = 1,
	TIMEMETERRATE_MOTORCYCLE,		
	TIMEMETERRATE_END
};

// �� ������ ����
#define TIMEMETERRATE_TRANSFORM_CLIENTTIME	10  //10�ʿ� �ѹ�.
#define TIMEMETERRATE_TRANSFORM_DBTIME	60

#define MAX_TIMEMETERRATE	TIMEMETERRATE_END 

/*
struct stTimeMeterRate
{
	SI16	siTimeMeterRateType;			// �ð� ������ ����.
	SI32	siLeftTime;						// ���� �ð� - ��밡����
	SI64	siUsedTime;						// �̹� ����� �ð�.
	DWORD   dwGoClientTime;					// Ŭ���̾�Ʈ�� ������ �ð������� ���� �ð�.	
	DWORD   dwGoDBTime;						// DB�� ������ �ð������� ���� �ð�.	
};
*/

class cltTimeMeterRate
{
public:
	SI16	siTimeMeterRateType;			// �ð� ������ ����.
	SI32	siLeftTimeSec;					// ���� �ð� - ��밡���� (��)

	SI32	siUseTimeSec;					// ���ð� - �������� ��� (��)
	DWORD	dwStartedTime;					// ������ �ð�

public:
	cltTimeMeterRate()
	{
		Init();
	}
	~cltTimeMeterRate()
	{
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltTimeMeterRate));
	}

	void Set(cltTimeMeterRate *pTimeMeterRate)
	{
		if(pTimeMeterRate == NULL)					return;
		
		siLeftTimeSec		= pTimeMeterRate->siLeftTimeSec;
		siUseTimeSec		= pTimeMeterRate->siUseTimeSec;
		dwStartedTime		= pTimeMeterRate->dwStartedTime;
	}

	SI32 AddLeftTime(SI32 siAddSec)		{	siLeftTimeSec += siAddSec;								return siLeftTimeSec;		}
	SI64 UseTime(SI32 siUseSec)
	{	
		siLeftTimeSec -= siUseSec;	siUseTimeSec += siUseSec;	
		if(siLeftTimeSec <= 0 )		siLeftTimeSec = 0;	
		return siUseTimeSec;
	}

	bool IsUse()
	{
		if( dwStartedTime > 0 )		return true;
		if( siUseTimeSec > 0 )		return true;

		return false;
	}
};

class cltTimeMeterRateInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTimeMeterRate, MAX_TIMEMETERRATE>	m_clTimeMeterRate;
#else
	cltTimeMeterRate	m_clTimeMeterRate[MAX_TIMEMETERRATE];
#endif

	SI32				m_siUseTimePerSec;

	cltTimeMeterRateInfo();

	void Init();

	void SetTimeMeterRate( cltTimeMeterRateInfo* pclinfo );

	void AddLeftTime( SI16 siTimeMeterType, SI32 siAddTimeSec );
	void UseTime( SI16 siTimeMeterType, SI32 siUseTimeSec );
	SI32 GetLeftTime( SI16 siTimeMeterType );

	bool UseStart( SI16 siTimeMeterType );
	bool UseEnd( SI16 siTimeMeterType );

	//type �� index�� ����.
	cltTimeMeterRate* GetTimeMeterRate( SI16 siTimeMeterRateType );
};

//------------------------------------
// �ź�. 
//------------------------------------
#define GRANK_KING			1		// ����(��) 
#define GRANK_MINISTER1		2		// ����(�����Ǽ�) 
#define GRANK_MINISTER2		3		// ����(ȣ��) 
#define GRANK_MINISTER3		4		// ?(����)
#define GRANK_MINISTER4		5		// ����(����) 
#define GRANK_MINISTER5		6		// ����(����) 
#define GRANK_MINISTER6		7		// ����(����) 

#define GRANK_MC_WOMAN		100		// ���� MC
#define GRANK_MC_MAN		101		// ���� MC

//-----------------------------------
// ���� �⺻��  �Ǵ� ĳ���� ����. 
//-----------------------------------
class cltPBasicInfo{
public:
	SI32		siIDNum;						// �� Person�� �Ҽӵ� IDNum
	TCHAR		szAccountID[MAX_PLAYER_NAME];	// �� Person�� �Ҽӵ� ������ ���̵�. 	
	UI08		uiIndex;		// ĳ���� ������ �� ��° ���Կ� ��ġ�ϴ°� ?
	UI08		uiSex;			// ����, (���� 1, ���� 0)
	UI16 		uiKind;			// ĳ������ ����. 
	SI08		siHomeVillage;	// �ּ���. 
	UI08		uiGRank;		// �ź�. 
	UI08		uiAngelSwitch;	// ��ȣõ������ ����. 
	UI08		uiPoliceSwitch;
	cltCharPos	clPos;	// ĳ������ ��ġ 
	SI32		siPlaySecond;	// �÷����� �� ���� �ð�(��) 
	SI32		siPlayEventSecond;	// �̺�Ʈ ������ �÷����� �� ���� �ð�(��) - tys
	cltPos		clLastMainMapPos;	// �߿������� ���� ��ġ. 
	SI16		siSchoolUnique;	// ��� �б��� ����ũ. 
	
	// ��� ���� ����. 
	SI32		siWealthRank;		// ������. 
	cltMoney	clTotalWealthMoney;	// �� ����. 
	SI32		siWealthCalcDay;	//	������ ����� ��. 

	cltSimplePerson	clPerson;	// �ڽ��� ����. 

	// ���� ���� ����.
	cltPFatherInfo clFatherInfo;

	// ��õ�� ���� ����
	cltSimplePerson clRecommenderInfo;	// ��õ�� ����
	
	SI16	siVoteKingSelectionIndex;	// ��ǥ�� �ĺ��� ��ȣ	
	
	bool	bWennyOldManUpgrade;			// ȣǰ �� ���� �ִ���?

	bool	bNewAccount; // Ư���� ���Ŀ� ������ �����ΰ�?
	bool	bGameEvent; // ���� ����ǰ� �ִ� �̺�Ʈ�� �����ߴ°�?
	bool	bWinPrizeEvent; // ���� ����ǰ� �ִ� �̺�Ʈ�� ��÷�Ǿ��°�?

	bool	bUseNoLimitTicketWarp ; // ���ѿ��� �ϴ°�?
	SI32	siNoLimitTicketWarpDateVary ; // ���ѿ��� �Ⱓ

	UI08	uiGMMode;	// ��� ��� ���� (0 : ��ָ��, 1 : GM���, 2: ���̵� ���)  [3���϶��� ����MVP ���]
	bool	bPCRoom;
	
	TCHAR	sPCroomGrade;	// PC���� G , P , S
	
	TCHAR	szCRMcode[16];

	bool	bQuestEvolve; // ����Ʈ ��ȭ�� ������ �ִ���?

	TCHAR	szGreeting[128]; // �λ縻

	SI32	siCountryID;
	bool	bShowCountry;
	bool	bShowPuchikomi;

	cltSimpleGuildRank	clGuildRank;	// ������ ��尡 �ִ� ���� ��ȣ
										// ������ ����� �ε���
										// ������ ����� �ź� ����
	bool	bPremiumService;		// �����̾� ���� ���ΰ�?
	SI32	siPremiumServiceDate;
	bool	bCombatService;		// ���� �����̾� ���� ���ΰ�?
	SI32	siCombatServiceDate;
	bool    bMakeItemService;		
	SI32	siMakeItemServiceDate; //���� �����̾� ���� ���ΰ�?
	bool	bBulletService;			// źȯ �����̾� �������ΰ�.
	SI32	siBulletServiceDate;
	bool	bSummonStaminaService;			// ��ȯ�� ���¹̳� �����̾� ���� ���ΰ�?
	SI32	siSummonStaminaServiceDate;

	cltVillageScoreInfo clVillageScoreInfo;	// �ֹ�Ȱ�� ����
	cltBufInfo clBufInfo;

	//KHY - 1220 - �ð�������
	cltTimeMeterRateInfo clTimeMeterRateInfo;

	bool	bReceiveBeginnerWeapon;

	// PVP�����ؼ� �߰� -pvp �̱�Ƚ��, �й��� Ƚ��, ���� �̱� �ְ��������� �̸�
	SI32	siPVPWinNumber;
	SI32	siPVPLoseNumber;
	TCHAR	szPVPWinHighLevelUserName[MAX_PLAYER_NAME];

	bool	bMarKetPremiumService;
	SI32	siMarKetPremiumServiceDate;

	bool	bNewMarketPremiumService;					// [����] ���ո��� �����̾�.
	SI32	siNewMarketPremiumServiceDate;

	//// ���� ����
	bool	bSpaceBoxPremiumService;
	SI32	siSpaceBoxPremiumServiceDate;
	// PCK : ��Ī ���� ���� ����
	cltPersonNaming clPersonNamaing;

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, ATTEND_EVENTTYPE_MAX>			bAttendToday;
#else
	bool	bAttendToday[ATTEND_EVENTTYPE_MAX];			// ���� �̹� �⼮üũ �Ͽ��°�
#endif
	bool	bIsAttendTime;								// �⼮ ������ ���� �� �ִ� �ð��� �Ǿ��°�?
	bool	bIsCanAttend;								// �⼮�� �� �� �ִ°�?
	bool	bIsCanReward;								// ������ ���� �� �ִ°�?
	SI32	siAttendanceDays;							// ���� �⼮�ϼ�. 
	CEmpireInfo clEmpireInfo;		// ���� ��������
	//KHY - 0906 - ���� ���� ���� �ð�.
	_SYSTEMTIME  siFirstCreatTimeDateVary; // ���� ���� ���� �ð�.(DateVary)

	bool	bSummonExtraSlot		;				// ��ȯ�� Ȯ�� Ƽ�� ����Ͽ����� ����
	SI32	siSummonExtraSlotDate	;				// ��ȯ�� Ȯ�� Ƽ�� ��볯��

	UI08	m_uiRealAge;	// �ֹε�� ���� ���� ���̰� ����(���� �ѱ��� ����)
public:

	cltPBasicInfo();
	~cltPBasicInfo();
	
	cltPBasicInfo(TCHAR* paccountid, SI32 idnum, UI08 index, UI08 sex, 
		cltSimplePerson* pclperson, UI08 kind, SI08 homevillage, 
		cltCharPos* pclpos, SI32 playsecond, SI32 playeventsecond, 
		SI16 schoolunique, cltPos* pclmainmappos, cltPFatherInfo* pclfather, 
		cltSimplePerson* pclRecommenderInfo, SI16 VoteKingSelectionIndex, 
		SI32 wealthrank, cltMoney totalmoney, SI32 wealthday, 
		bool wennyoldmanupgrade,bool newaccount, bool gameevent , 
		bool winprizeevent,bool nolimitticketwarp , SI32 warpticketdatevary,
        UI08 gmmode , bool pcroom, TCHAR pcroomgrade , TCHAR * crmcode,
		bool bquestevolve, TCHAR* greeting,	cltSimpleGuildRank* pclguildrank ,
		bool premium,SI32 premiumdate,bool combat,SI32 combatdate,
		bool MakeItem, SI32 MakeItemdate, bool BulletService, 
		SI32 BulletServiceDate,	bool SummonStaminaService, 
		SI32 SummonStaminaServiceDate, cltVillageScoreInfo* pclVillageScoreInfo,
		cltBufInfo* pclBufInfo, bool ReceiveBeginnerWeapon, SI32 WinNumber,
		SI32 LoseNumber, TCHAR * HighLevelUserName, bool MarKetPremiumService, 
		SI32 MarKetPremiumServiceDate, bool* bAttendToday, CEmpireInfo* EmpireInfo, _SYSTEMTIME firstcreatTime,
		bool SpaceBoxPremiumService,SI32 SpaceBoxPremiumServiceDate,bool SummonExtraSlot,SI32	siSummonExtraSlotDate,
		bool NewMarketPremiumService, SI32 NewMarketPremiumServiceDate,
		UI08 uiRealAge

		);
		 
	void Set(cltPBasicInfo* pclinfo);

	void Init();
	
	void SetIndex(SI32 index);
	SI32 GetIndex() const;

	void SetKind(SI32 kind);
	SI32 GetKind() const;

	const cltCharPos* GetPos() const;

	void SetMapIndex(SI32 index);
	SI32 GetMapIndex() const;

	void SetX(SI32 x);
	SI32 GetX() const;

	void SetY(SI32 y);
	SI32 GetY() const;

	void SetName(const TCHAR* pname);
	const TCHAR* GetName() const;

	bool IsQuestEvolve();
	void SetQuestEvolve(bool questevolve);

	//KHY - 0906 - ���� ���� ���� �ð�.
	_SYSTEMTIME GetFirstCreatTime(){return siFirstCreatTimeDateVary; }

	void GetGMSectionData(GMSectionData* pkGMSectionData);
	void SendInfofromServer(cGMClient* pGMClient);
	
};

//-------------------------------
// �⺻�� �Ǵ� �ɷ�ġ. 
//-------------------------------
class cltBasicAbility{
public:
	// �⺻ �ɷ�ġ.(����ڰ� ���ʽ��� ������ �ø��� �ɷ�ġ.) 
	SI32	siStr;				// �ٷ�	
	SI32	siDex;				// ���߷�
	SI32	siVit;				// ������ 
	SI32	siMag;				// ���� 
	SI32	siHnd;				// ������. 
	SI32	siLuk;				// ��� (2005-12-21 �߰� by cyj)
	SI32	siWis;				// KHY - 0325 - ���� �߰�.
	

	cltBasicAbility()
	{
		Init();
	}
	cltBasicAbility(SI32 str, SI32 dex, SI32 vit, SI32 mag, SI32 hnd, SI32 luk  ,SI32 Wis)
		:	siStr(str),		siDex(dex),		siVit(vit),		siMag(mag),		siHnd(hnd),		siLuk(luk),	siWis(Wis)
	{
	}

	~cltBasicAbility()
	{
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltBasicAbility));
	}
	void Set(SI32 str, SI32 dex, SI32 vit, SI32 mag, SI32 hnd, SI32 luk,SI32 Wis);
	void Set(cltBasicAbility* pclinfo);

	SI32 GetWis() const;
	void SetWis(SI32 dat);
	SI32 IncreaseWis(SI32 amount);
	SI32 DecreaseWis(SI32 amount);
	
	SI32 GetStr() const;
	void SetStr(SI32 dat);
	SI32 IncreaseStr(SI32 amount);
	SI32 DecreaseStr(SI32 amount);
	SI32 GetStrDamage( bool Person );

	SI32 GetDex() const;
	void SetDex(SI32 dat);
	SI32 IncreaseDex(SI32 amount);
	SI32 DecreaseDex(SI32 amount);

	SI32 GetVit() const;
	void SetVit(SI32 dat);
	SI32 IncreaseVit(SI32 amount);
	SI32 DecreaseVit(SI32 amount);
	
	SI32 GetMag() const;
	void SetMag(SI32 dat);
	SI32 IncreaseMag(SI32 amount);
	SI32 DecreaseMag(SI32 amount);

	SI32 GetHnd() const;
	void SetHnd(SI32 dat);
	SI32 IncreaseHnd(SI32 amount);
	SI32 DecreaseHnd(SI32 amount);

	SI32 GetLuk() const;
	void SetLuk(SI32 dat);
	SI32 IncreaseLuk(SI32 amount);
	SI32 DecreaseLuk(SI32 amount);

	SI32 GetBattleLevel()
	{
		SI32 battleLevel =(SI32)(( 
			sqrt( (double)((siDex-7)*48 ) ) + 
			sqrt( (double)((siVit-6)*14 ) ) 
			)/2);

		battleLevel = max( 1, battleLevel );

		return battleLevel;
	}

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};

//-----------------------------------------
// ���� ������ ����
//-----------------------------------------
class cltInvestInfo{
public:
	cltSimplePerson	clPerson;
	SI32			siStockNum;		// ���� �ֽļ�. 

public:
	
	cltInvestInfo();
	
	void Set(cltInvestInfo* pinfo);
	void Init();
	
	SI32 GetStockNum() const;
	void SetStockNum(SI32 num);

};

class cltVillageInvestInfo{
public:
	SI32 siVillageUnique;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltInvestInfo, MAX_INVEST_NUMBER>	clInvestInfo;
#else
	cltInvestInfo clInvestInfo[MAX_INVEST_NUMBER];
#endif

	cltVillageInvestInfo();

	void Set(cltVillageInvestInfo* pclMayorInfo);
	
};

//--------------------------------------
// �ǰ� ����
//--------------------------------------
#define HEALTH_ORGAN_TYPE_ORG1			1		// ��ȭ���
#define HEALTH_ORGAN_TYPE_ORG2			2		// ��ȯ���
#define HEALTH_ORGAN_TYPE_ORG3			3		// ȣ�����
#define HEALTH_ORGAN_TYPE_ORG4			4		// �Ű���

//--------------------------------
// �ǰ� Ŭ����. (�ٽ�) 
//--------------------------------
class cltHealthInfo{
public:
	//------------------------
	// ���� ���� 
	//------------------------
	SI16 siHungry		;			// �����.		(0~ 256)	�⺻�� 128	

	//-------------------------
	// ��� ���� 
	//-------------------------
	SI08 siInnerOrg1	;		// ��ȭ�� (-100 ~ 100)
	SI08 siInnerOrg2	;		// ��ȯ�� 
	SI08 siInnerOrg3	;		// ȣ���� 
	SI08 siInnerOrg4	;		// �Ű�� 

	//------------------------
	// ���� ���� 
	//------------------------
	UI08 uiOrg1Disease	;		// ��ȭ�� ����.			
	UI08 uiOrg2Disease	;		// ��ȯ�� ����. 
	UI08 uiOrg3Disease	;		// ȣ���� ����. 
	UI08 uiOrg4Disease	;		// �Ű�� ����. 

	//-----------------------
	// ���� �ǰ� ����.
	//-----------------------
	UI08 uiHealthPoint;		// 0 ~ 100	:100�̸� �ɷ�ġ�� ��� ������ �� �ֵ�. 

	cltHealthInfo();

	void Init();
	void Set(cltHealthInfo* pclinfo);
	void IncreaseInnerOrg1(SI32 amount);
	void IncreaseInnerOrg2(SI32 amount);
	void IncreaseInnerOrg3(SI32 amount);
	void IncreaseInnerOrg4(SI32 amount);
	bool DecreaseOrg(SI32 type, SI32 amount, SI32 level, bool forceDec = false);	// ���� ����� �ջ��Ų��. 

	SI32 GetOrg(SI32 type);		// ���� ����� ���¸� ���´�. 

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};

//-------------------------
// �ð������� ���� Ŭ���� 
//-------------------------
class cltDate{
public:
	UI32 uiYear:	8;	// 0 ~255
	UI32 uiMonth:	4;	// 0 ~15
	UI32 uiDay:		6;	// 0 ~63
	UI32 uiHour:	5;	// 0 ~31
	UI32 uiMinute:	6;	// 0 ~63
	UI32 uiReserved:3;	

public:

	/*
	void Set(SI32 year, SI32 month, SI32 day, SI32 hour, SI32 minute);
	void Set(const cltDate* pdate);
	
	BOOL Increase
	Time(bool* pbdaychangedswitch, bool bforcetimeswitch);
	BOOL GetDateText(TCHAR* buffer);		// �ð��� ���ڿ��� �����. 
	void AddMonth(SI32 month);			// ���� ���Ѵ�. 
	SI32 GetDateVary();					// ��¥ ������ ���Ѵ�. (SI32 ���� �ϳ��� ��¥�� ��� ���ױ׸���) 
	SI32 GetHourVary();					// �ð� ������ ���Ѵ�. 
	void MakeFullDateFromDateVary(SI32 datevary);		// DateVary�� ������ ���� ��¥ ������ �����Ѵ�. 
	
	BOOL IsLaterDayThan( cltDate* pclDate );
	BOOL IsLaterMonthThan( cltDate* pclDate );

	void DiffDate( cltDate *pCurrentDate, cltDate *pOldDate );
	SI32 GetDay();
	*/

	void Set(SI32 year, SI32 month, SI32 day, SI32 hour, SI32 minute)
	{
		uiYear		= year;
		uiMonth		= month;
		uiDay		= day;
		uiHour		= hour;
		uiMinute	= minute;
		uiReserved	= 0;
	}

	void Set(const cltDate* pdate)
	{
		memcpy(this, pdate, sizeof(cltDate));						//ok
	}

	SI32 GetDateVary()
	{
		SI32 vary ;
		vary = (uiDay-1) + (uiMonth-1)*30 + (uiYear*12)*30;

		return vary;
	}


	// �ð� ������ ���Ѵ�. 
	SI32 GetHourVary()
	{
		return  GetDateVary() * 24 + uiHour;
	}

	SI32 GetMinuteVary()
	{
		return GetHourVary() * 60 + uiMinute;
	}

	// DateVary�� ������ ���� ��¥ ������ �����Ѵ�. 
	void MakeFullDateFromDateVary(SI32 datevary)
	{
		uiYear  = datevary / (12* 30);
		datevary %= (12* 30);

		uiMonth = datevary / 30 + 1;
		datevary %= 30;

		uiDay		= datevary + 1;
		uiHour		= 0;
		uiMinute	= 0;
	}
	
	// HourVary�� ������ ���� ��¥ ������ �����Ѵ�. KHY - 0606 
	void MakeFullHourFromHourVary(SI32 hourvary)
	{
		uiYear    = hourvary / (12 * 30 * 24);
		hourvary %= (12 * 30 * 24);

		uiMonth   = (hourvary / (30 * 24)) + 1;
		hourvary %= (30 * 24);

		uiDay	  = hourvary / 24 + 1;
		uiHour	  = hourvary - ((hourvary / 24) * 24);
		uiMinute  = 0;
	}

	// MinuteVary�� ������ ���� ��¥ ������ �����Ѵ�. by LEEKH 20008-11-11
	void MakeFullHourFromMinuteVary(SI32 minutevary)
	{
		uiYear    = minutevary / (12 * 30 * 24 * 60);
		minutevary %= (12 * 30 * 24 * 60);

		uiMonth   = (minutevary / (30 * 24 * 60)) + 1;
		minutevary %= (30 * 24 * 60);

		uiDay	  = minutevary / (24 * 60) + 1;
		minutevary %= (24 * 60);

		uiHour	  = minutevary / 60;
		minutevary %= 60;

		uiMinute  = minutevary - ((minutevary / 60) * 60);
	}

	SI32 GetDay()
	{
		return min(30, max(1, uiDay));
	}

	BOOL IsLaterDayThan( cltDate* pclDate )
	{

		UI32 a, b;
		a = uiYear * 10000 + uiMonth * 100 + uiDay;
		b = pclDate->uiYear * 10000 + pclDate->uiMonth * 100 + pclDate->uiDay;

		return ( a > b ); 
	}

	BOOL IsLaterMonthThan( cltDate* pclDate )
	{

		UI32 a, b;
		a = uiYear * 100 + uiMonth;
		b = pclDate->uiYear * 100 + pclDate->uiMonth;

		return ( a > b );
	}


	// ���� ���Ѵ�. 
	void AddMonth(SI32 addmonth)
	{
		SI32 month = uiMonth;
		month += addmonth;

		SI32 addyear = ( month - 1 ) / 12;
		if(addyear)
		{
			uiYear += addyear;
			month  = (( month - 1 ) % 12) + 1;
		}

		uiMonth = (UI32)month;
	}

	BOOL IncreaseTime(bool* pbdaychangedswitch, bool bforcetimeswitch)
	{
		// ��¥�� ������� �ʾ����� �����Ѵ�. (����Ʈ. ) 
		*pbdaychangedswitch	= false;

		BOOL updateswitch = FALSE;

		uiMinute ++;

		if(bforcetimeswitch)
		{
			if(uiMinute >= 1)
			{
				uiMinute = 0;
				uiHour++;

				// ���� �ð� ���������� TRUE ����.
				if(uiHour % 2 ==0)
				{
					updateswitch = TRUE;
				}

				if(uiHour >= 1)
				{
					uiHour=0;
					uiDay++;

					// ��¥�� ����Ǿ����� �����Ѵ�. 
					*pbdaychangedswitch	= true;

					// ��¥�� ����Ǹ� �ݵ�� DB�� �����Ѵ�. 
					updateswitch = TRUE;

					if(uiDay > 30)
					{
						uiDay = 1;
						uiMonth++;

						if(uiMonth > 12)
						{
							uiMonth = 1;
							uiYear++;
						}
					}
				}
			}
		}
		else
		{

			if(uiMinute >= 60)
			{
				uiMinute = 0;
				uiHour++;

				// ���� �ð� ���������� TRUE ����.
				if(uiHour % 2 ==0)
				{
					updateswitch = TRUE;
				}

				if(uiHour >= 24)
				{
					uiHour=0;
					uiDay++;

					// ��¥�� ����Ǿ����� �����Ѵ�. 
					*pbdaychangedswitch	= true;

					// ��¥�� ����Ǹ� �ݵ�� DB�� �����Ѵ�. 
					updateswitch = TRUE;

					if(uiDay > 30)
					{
						uiDay = 1;
						uiMonth++;

						if(uiMonth > 12)
						{
							uiMonth = 1;
							uiYear++;
						}
					}
				}
			}
		}

		return updateswitch;
	}

	// �ð��� ���ڿ��� �����.  // %d�� %d�� %d�� %d�� 
	BOOL GetDateText(TCHAR* buffer)
	{
		if(buffer == NULL)return FALSE;

		TCHAR* pText = GetTxtFromMgr(3808);
		StringCchPrintf(buffer, 128, pText, uiYear, uiMonth, uiDay, uiHour);

		return TRUE;
	}

	// �ð��� ���ڿ��� �����.  // %d�� %d�� %d��
	BOOL GetRealDateText(TCHAR* buffer)
	{
		if(buffer == NULL)return FALSE;

		TCHAR* pText = GetTxtFromMgr(1644);
		StringCchPrintf(buffer, 128, pText, uiYear, uiMonth, uiDay);

		return TRUE;
	}


	void DiffDate( cltDate *pCurrentDate, cltDate *pOldDate )
	{
		int temp;

		ZeroMemory( this, sizeof( cltDate ) );

		// minute
		temp = pCurrentDate->uiMinute - pOldDate->uiMinute;
		if( temp < 0 ) { uiMinute = 60 + temp; temp = -1; }
		else { uiMinute = temp; temp = 0; }

		// Hour
		temp += ( pCurrentDate->uiHour - pOldDate->uiHour );
		if( temp < 0 ) { uiHour = 24 + temp; temp = -1; }
		else { uiHour = temp; temp = 0; }

		// Day
		temp += ( pCurrentDate->uiDay - pOldDate->uiDay );
		if( temp < 0 ) { uiDay = 30 + temp; temp = -1; }
		else { uiDay = temp; temp = 0; }

		// Month
		temp += ( pCurrentDate->uiMonth - pOldDate->uiMonth );
		if( temp < 0 ) { uiMonth = 12 + temp; temp = -1; }
		else { uiMonth = temp; temp = 0; }

		// Year
		temp += ( pCurrentDate->uiYear - pOldDate->uiYear );
		uiYear = temp;
	}

};

//----------------------------------------
// �Ӵ��� ���� 
//----------------------------------------
class cltRentContract{
public:
	GMONEY siRentFee;		// �� �Ӵ��. 
	SI32 siRentPeriod;	// �Ӵ�Ⱓ. 
	cltDate	clDate;		// ����Ͻ�. 

	void Set(GMONEY fee, SI32 period);
	void Set(GMONEY Fee, SI32 Period, cltDate *pclDate );
	void Set(const cltRentContract* pclcontract);
	
	BOOL IsValid();
	BOOL IsSame(const cltRentContract* pclcontract);
	
	bool IsEnd(cltDate* pcldate);			// ����� ����Ǿ��°�?
	void Init();
	
	void GetRentInfoText( TCHAR *RentInfoText, SI16 txtSize );

};


//-----------------------------------------------
// ���� ���� 
//-----------------------------------------------
#define HORSE_NEED_FEED			1		// ���̸� �� ���� �䱸. 

#define HORSE_REWARDTYPE_SPEED	1
#define HORSE_REWARDTYPE_STR	2
#define HORSE_REWARDTYPE_DEX	3
#define HORSE_REWARDTYPE_MAG	4

class cltHorse{
public:

	SI16 siHorseUnique;
	TCHAR	szName[MAX_HORSE_NAME];		// �̸� 
	cltDate clBirthDate;				// ���� 

	SI08	siSex;						// ���� 
	SI08	siPregnentPeriod;			// �ӽűⰣ
	SI16	siLife;						// ���� 

	SI16	siStr;
	SI16	siDex;
	SI16	siMag;
	SI16	siMoveSpeed;

	SI32	siStatusBonus;				// �� ���ʽ�

	cltDate clAddStatusDate;			// ���ɷ�����̿�� �ٸ� ����
	SI32	siStatusTicketNum;			// ���ɷ�����̿�� ���� �߶���

	//------------------------------
	// ���������� ������Ʈ �� �ð�. 
	//------------------------------
	SI32	siLastUpdateSecond;
	
	//---------------------------
	// ���� �䱸 ���� ���� 
	//---------------------------
	SI32	siLastNeedTime;				// ���������� �䱸�ߴ� �ð�. 
	SI32	siDueTime;					// �䱸���� ������ �Ѱ� �ð�. ( �� �ð����� �䱸������ ������Ű�� ���ϸ� ����) 

	SI16	siNeed;						// ���� �䱸����. 
	SI16	siPara1;		
	SI16	siPara2;
	SI16	siSupply;					// ���� ��Ȳ. 
	SI16	siRate;						// ���� �䱸�� ������ Ȯ��. 

	//--------------------------
	// ���� ���� 
	//--------------------------
	SI16	siRewardType;
	SI16	siRewardAmount;

	//--------------------------
	// ��Ʃ�װ��� ���� 
	//--------------------------
	cltDate clPremiumPartsDate_Slot1	;
	SI32	siPremiumUnique_Slot1		;
	cltDate clPremiumPartsDate_Slot2	;
	SI32	siPremiumUnique_Slot2		;
	cltDate clPremiumPartsDate_Slot3	;
	SI32	siPremiumUnique_Slot3		;
	SI32	siPremiumPartsNum			;
public:

	cltHorse();
	cltHorse(SI16 unique, TCHAR* pname, cltDate* pcldate, SI08 sex, SI16 life, SI08 siPregnentPeriod,
			 SI16 str, SI16 dex, SI16 mag, SI16 movepseed, SI32 curhourvary,SI32 bonus
			 ,cltDate* pclstatusdate , SI32 ticketnum
			 ,cltDate* pclPremiumPartsdate_Slot1,SI32 PremiumUnique_Slot1
			 ,cltDate* pclPremiumPartsdate_Slot2,SI32 PremiumUnique_Slot2
			 ,cltDate* pclPremiumPartsdate_Slot3,SI32 PremiumUnique_Slot3
			 ,SI32 PremiumPartsNum);
	
	void Init();
	void Set(cltHorse* pclhorse);	
	
	SI16 GetRealMoveSpeed();			// ���ӿ� ����� ���� �̵� �ӵ��� ���Ѵ�.
	SI32 GetAge(SI32 curday);			// ���̸� ���Ѵ�. 
	bool IsSame(cltHorse* pclhorse);	// ���� ������ ���θ� �Ǵ��Ѵ�.
	
	// �����̾� ���� ������ ������ �´� 
	// ��������ũ
	SI32 GetPremiumUnique_Slot1(void);
	SI32 GetPremiumUnique_Slot2(void);
	SI32 GetPremiumUnique_Slot3(void);
	// ������ ��¥
	cltDate GetPremiumPartsDate_Slot1(void);
	cltDate GetPremiumPartsDate_Slot2(void);
	cltDate GetPremiumPartsDate_Slot3(void);
	// �̹� ���� ������ �����ߴ���
	bool HaveSamePremiumParts(SI32 siItemUnique);
	// �󽽷��� ã�´�.
	SI32 FindEmptyPremiumPartsSLot(void);
	// �����̾� ������ ����
	void SetPremiumParts(cltDate* pclDate,SI32 siUnique );

	SI32 GetPremiumPartsNum(void)	;
	void AddPremiumPartsNum( SI32 siNum)	;
	SI32 GetPremiumPartsNum_Slot1(void)		;
	SI32 GetPremiumPartsNum_Slot2(void)		;
	SI32 GetPremiumPartsNum_Slot3(void)		;

	void SetPremiumPartsUnique_Slot1(SI32 siUnique)		;
	void SetPremiumPartsUnique_Slot2(SI32 siUnique)		;
	void SetPremiumPartsUnique_Slot3(SI32 siUnique)		;
};


//-------------------------------------------------
// cltAttackTarget
//-------------------------------------------------
class cltAttackTarget{
public:
	SI32 			siAttackID;                  // ������ ��� ���̵� 
	cltCharPos		clPos;
	
	SI32			siHitRate;						// ���� ��� ���� ���߷�. 
	SI32			siDodgeRate;					// ���� ��� ���� ȸ�Ƿ�. 

	bool			bPVPHit;					// PVP�� ��븦 �����°�?

	cltAttackTarget();
	cltAttackTarget(SI32 attackid, const cltCharPos* pclpos, SI32 hitrate, SI32	DodgeRate,bool PVPHit = false);
	
	void Set(const cltAttackTarget* pcltarget);
	void Set(SI32 attackid, const cltCharPos* pclpos, SI32 hitrate, SI32 DodgeRat, bool PVPHit = false);
	void Init();
	
	void SetAttackID(SI32 id);
	SI32 GetAttackID() const;
	
	void SetDodgeRate(SI32 siDodgeRate);
	SI32 GetDodgeRate(){return siDodgeRate;} 

};

//---------------------------------
// ���� ���. 
//---------------------------------
#define MAX_PRISON_REASON_SIZE	50
#define MAX_PRISON_SECOND		30
#define MAX_PRISON_LIST_NUM		50 // �����Ǽ� â���� ������ ����Ʈ �� ����

struct HyungzoMember
{
	SI32	siDate;										// ����������
	SI32	siPrisonSecond;								// �����ð�
	TCHAR	szCharName[MAX_PLAYER_NAME];				// ������
	TCHAR	szPrisonReason[MAX_PRISON_REASON_SIZE];		// ��������
};

class cltGuiltyInfo{
public:
	SI32	siTotalPrisonSecond;			// �� �����ð�. 
	SI32	siLeftPrisonSecond;			// ���� �����ð�. 
	TCHAR	szPrisonReason[MAX_PRISON_REASON_SIZE];
	SI32	siTotalPrisonNum;

	cltGuiltyInfo();
	
	void Init();
	void Set(cltGuiltyInfo* pclinfo);
	void SetTotalPrisonSecond(SI32 second);
	void SetLeftPrisonSecond(SI32 second);
	void SetPrisonReason(TCHAR * reason);
	void SetPrisonNumber(SI32 num);
	
};

//----------------------------------------------
// ������ ���� ���� 
//----------------------------------------------
class cltHuntStg{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	void Set(cltHuntStg* pclstg);
	
};

class cltHuntLineUp{
public:
	SI16 siLineUpKind;		// ������ ��ġ�� ���� ����
	SI16 siLineUpKindNum;	// ������ ��ġ�� ���� ����
	SI16 siLineUpKindLevel; // ������ ��ġ�� ���� ����

	void Set(cltHuntLineUp* pclinfo);

};

#define MAX_FEAST_PRESENT_NUMBER							10
#define MAX_FEAST_STG_NUMBER								10

// ���ڽ� â�� ����
class cltFeastStg 
{
public:
	SI16 siItemUnique;
	SI16 siItemNum;
	
	void Set( cltFeastStg* pclstg );

};


//-----------------------------------------
// ���� ��� ��û�� ����. 
//-----------------------------------------
#define MAX_FATHER_APPLYER_LIST_NUMBER	50

class cltFatherApplyerInfo{
public:
	SI16 siCharUnique;
	SI32 siPersonID;
	SI16 siLevel;
	SI16 siFameLevel;
	bool bAngelSwitch;					// ��ȣõ������ ����. 
	TCHAR szAccountID[MAX_PLAYER_NAME];
	//NTCHARString<MAX_PLAYER_NAME>	szAccountID;
	TCHAR szName[MAX_PLAYER_NAME];
	//NTCHARString<MAX_PLAYER_NAME>	szName;

	cltFatherApplyerInfo();
	cltFatherApplyerInfo(SI16 charunique, SI32 personid, TCHAR* pname, TCHAR* paccountid, SI16 level, SI16 famelevel, bool bangel);

	void Init();
	void Set(cltFatherApplyerInfo* pclinfo);

};

//-------------------------------------
// ���� 1���� ����. 
//-------------------------------------
class cltChildInfo{
public:
	cltSimplePerson clPerson;
	SI16			siLevel;
	TCHAR			szAccountID[MAX_PLAYER_NAME];

	
	cltChildInfo();
	cltChildInfo(cltSimplePerson* pclperson, SI16 level,TCHAR * accountID);
	void Init();
	void Set(cltChildInfo* pclchild);
	
};

//------------------------------
// Personģ�� ����. 
//------------------------------
#define FRIEND_GROUP_UNKNOWN	0
#define FRIEND_GROUP_1			1
#define FRIEND_GROUP_2			2
#define FRIEND_GROUP_3			3
#define FRIEND_GROUP_4			4

#define MAX_FRIEND_GROUP					5		// 0���� �׷���� �׷�
#define	MAX_FRIEND_NUMBER					100

#define MAX_FRIEND_NOTESIZE					201		// 50�� ���� �޸� ����
class cltPFriendInfo{
public:
	TCHAR szGroupName[MAX_FRIEND_GROUP][MAX_FRIEND_GROUPNAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI08, MAX_FRIEND_NUMBER>				siGroupID;
	NSafeTArray<cltSimplePerson, MAX_FRIEND_NUMBER>		clFriend;
#else
	SI08 siGroupID[ MAX_FRIEND_NUMBER ];
	cltSimplePerson clFriend[MAX_FRIEND_NUMBER];
#endif
	TCHAR szAccountID[MAX_FRIEND_NUMBER][MAX_PLAYER_NAME];

	void Set(cltPFriendInfo* pclinfo);
};

class cltFriendInfo : public cltPFriendInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_FRIEND_NUMBER>		siCharID;
	NSafeTArray<bool, MAX_FRIEND_NUMBER>		bConnectStatus;
#else
	SI32 siCharID[ MAX_FRIEND_NUMBER ];
	bool bConnectStatus[ MAX_FRIEND_NUMBER ];
#endif
//	TCHAR szAccountID[ MAX_FRIEND_NUMBER ][MAX_PLAYER_NAME];

	void Init();
	void Init( SI16 Index );
	
	void Set( SI16 Index, SI08 GroupID, SI32 PersonID, SI32 CharID, TCHAR *CharName, bool ConnectStatus ,TCHAR *AccountID);
	void Set( SI16 Index, SI08 GroupID, SI32 CharID, bool ConnectStatus ,TCHAR * AccountID);
	void Set( cltPFriendInfo *pclInfo );
	void Set( cltFriendInfo *pclInfo );
	
	SI16 GetFriendNum();

};
// �ɸ��� �ɼ� ����
// [ �߰� ] PVP < 2008-10-9 >
//====================================
// cltPDailyQuestInfo
//====================================
class cltPCharOptionInfo
{
	public:
		bool bChatSetNormal		;
		bool bChatSetVillage	;
		bool bChatSetGuild		;
		bool bChatSetParty		;
		bool bChatSetSystem		;
		bool bPvpMode			;	//PVP�� ���� �� ���ΰ�.

	cltPCharOptionInfo();
	void Init()	;
	void Set( bool ChatSetNormal,bool ChatSetVillage,bool ChatSetGuild, bool ChatSetParty,bool ChatSetSystem, bool pvpmode )	;
	void Set( cltPCharOptionInfo *pclInfo )	;
	void SetChatSetNormal( bool ChatSetNormal )	;
	void SetChatSetVillage( bool ChatSetVillage )	;
	void SetChatSetGuild( bool ChatSetGuild )	;
	void SetChatSetParty( bool ChatSetParty )	;
	void SetChatSetSystem( bool ChatSetSystem )	;
	void SetPVPMode( bool pvpmode )	;
	bool GetPVPMode()	;
	
};

class cltPDailyQuestInfo
{
public:
	SI16 siMonsterKind;
	SI16 siMonsterNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	GMONEY siRewardMoney;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;	// �� �ð�
	SI32 siLeftTime;    // ���� �ð�

	SI16 siKillMonsterNum;

	BOOL bMoneySelected;

	cltPDailyQuestInfo();

	void Init();
	void Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 QuestTiem, SI32 LeftTime, SI16 KillMonsterNum, BOOL MoneySelected );
	void Set( cltPDailyQuestInfo *pclInfo );
	BOOL IsSame(cltPDailyQuestInfo *pclInfo );
};
//���ϸ� ����Ʈ 2 
class cltPDailyQuest2Info
{
public:
	SI16 siMonsterKind;
	SI16 siMonsterNum;

	SI16 siRewardItemUnique;
	SI16 siRewardItemNum;
	GMONEY siRewardMoney;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;	// �� �ð�
	SI32 siLeftTime;    // ���� �ð�

	SI16 siKillMonsterNum;

	BOOL bMoneySelected;

	bool bHiddenQuest;

	cltPDailyQuest2Info();

	void Init();
	void Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 QuestTime, SI32 LeftTime, SI16 KillMonsterNum, BOOL MoneySelected, bool HiddenQuest );
	void Set( cltPDailyQuest2Info *pclInfo );
	BOOL IsSame(cltPDailyQuest2Info *pclInfo );
};
//���ϸ� ����Ʈ 3 
class cltPDailyQuest3Info
{
public:
	SI32 siItemUnique;
	SI32 siItemNum;

	SI32 siRewardItemUnique;
	SI32 siRewardItemNum;
	SI32 siRewardExp;

	GMONEY siFee;
	SI32 siQuestTime;	// �� �ð�
	SI32 siLeftTime;    // ���� �ð�

	SI16 siMadeItemNum;

	BOOL bMoneySelected;

	bool bHiddenQuest;

	cltPDailyQuest3Info();

	void Init();
	void Set( SI32 ItemQnique, SI32 ItemNum, SI32 RewardItemUnique, SI32 RewardItemNum,SI32 RewardExp, GMONEY Fee, SI32 QuestTiem, SI32 LeftTime, SI16 MakedItemNum, BOOL MoneySelected, bool HiddenQuest);
	void Set( cltPDailyQuest3Info *pclInfo );
	BOOL IsSame(cltPDailyQuest3Info *pclInfo );
};

//--------------------------------------
// ���
//--------------------------------------
class cltMaterialInfo {
public:
	SI32	siUnique;		// ����� ����ũ. 
	SI32	siNeedNum;		// ����� �ʿ� ����. 

	cltMaterialInfo();
	cltMaterialInfo(SI32 unique, SI32 itemnum);

	void Set(cltMaterialInfo* pclmaterial);
	void Set(SI32 unique, SI32 itemnum);

};

//--------------------------------------
// ����
//--------------------------------------
class cltToolInfo {
public:
	SI32	siUnique;		// ������ ����ũ. 

	cltToolInfo();
	cltToolInfo(SI32 unique);
	void Set(cltToolInfo* pcltool);
	void Set(SI32 unique);
	
};

inline void NTraceF(TCHAR* pszFormat, ...)
{
	TCHAR	szBuf[2048];
	va_list	ar;

	va_start(ar, pszFormat);

	wvsprintf(szBuf, pszFormat, ar);
	OutputDebugString(szBuf);

	va_end(ar);
}

//--------------------------------------
// PVP ���Ѵ��� ����
//--------------------------------------
const SI32 Const_PVP_League_Latest_Max	= 5;	// �ֽ� ��� ��� �ִ� ����
const SI32 Const_PVP_League_Top_Max		= 10;	// ���� ���� ��� �ִ� ����

enum PVPLEAGUE_RESULT
{
	PVPLEAGUE_RESULT_NONE	= 0,
	PVPLEAGUE_RESULT_WIN,
	PVPLEAGUE_RESULT_LOSE,
	PVPLEAGUE_RESULT_DRAW,
	PVPLEAGUE_RESULT_LOGOUT,
};

class cltPVP_League_OtherUserInfo
{
public:
	SI16	siLevel;
	SI08	siGrade;
	SI08	siResult; // PVPLEAGUE_RESULT

	SI32	siWin;
	SI32	siLose;
	SI32	siDraw;

	TCHAR	szName[MAX_PLAYER_NAME];
	
	cltPVP_League_OtherUserInfo()
	{
		ZeroMemory( this, sizeof(cltPVP_League_OtherUserInfo) );
	}

	~cltPVP_League_OtherUserInfo()
	{
	}

	void Set( cltPVP_League_OtherUserInfo* pclParam )
	{
		if ( pclParam )
		{
			memcpy( this, pclParam, sizeof(cltPVP_League_OtherUserInfo) );
		}
	}
};

//[����] ������.
class cltElixir
{
public:
	enum { 
			LIMIT_MULTIPLE =   3,		// ������ ��� ����.
			WATER_MILTIPLE =  10,		// ������ ���� ���� ������ ����Ǵ� ���.
		};


public:
	cltElixir() { ZeroMemory( this, sizeof(*this) );	}
	~cltElixir(){										}

public:
	SI16 		GetPoint( void )						{ return m_siPoint;						}
	void 		SetPoint( SI16 _siPoint)				{ m_siPoint = _siPoint; 				}

	SI16 		GetStr( void )							{ return m_siStr;						}
	void 		SetStr( SI16 _siStr)					{ m_siStr = _siStr; 					}

	SI16 		GetDex( void )							{ return m_siDex;						}
	void 		SetDex( SI16 _siDex)					{ m_siDex = _siDex; 					}

	SI16 		GetVit( void )							{ return m_siVit;						}
	void 		SetVit( SI16 _siVit)					{ m_siVit = _siVit; 					}

	SI16 		GetMag( void )							{ return m_siMag;						}
	void 		SetMag( SI16 _siMag)					{ m_siMag = _siMag; 					}

	SI16 		GetHnd( void )							{ return m_siHnd;						}
	void 		SetHnd( SI16 _siHnd)					{ m_siHnd = _siHnd; 					}

	SI16 		GetWis( void )							{ return m_siWis;						}
	void 		SetWis( SI16 _siWis)					{ m_siWis = _siWis; 					}

	cltDate*	GetEndDate( void )						{ return &m_clEndDate;					}
	SI32		GetMinuteVary( void )					{ return m_clEndDate.GetMinuteVary();	}
	void		SetEndMinuteVary( SI32 _siEndDateVary )	{ m_clEndDate.MakeFullHourFromMinuteVary( _siEndDateVary );		}

	UI08		GetMultiple( void )						{ return m_siMultiple;					}
	void		SetMultiple( UI08 _siMultiple)			{ m_siMultiple = _siMultiple;			}	

	SI16		GetTotalPoint( void )					{ return m_siPoint + m_siStr + m_siDex + m_siVit + m_siMag + m_siHnd + m_siWis; }

public:
	enum{ TYPE_STR, TYPE_DEX, TYPE_MAG, TYPE_VIT, TYPE_WIS, TYPE_HND, TYPE_POINT };

	bool IsElixir( void )
	{
		if( 0 < GetMultiple() && LIMIT_MULTIPLE >= GetMultiple() )	return true;

		return false;
	}
	
	SI16* GetTypePointer( SI32 _Type )
	{
		switch( _Type )
		{
			case TYPE_STR:		return &m_siStr; 
			case TYPE_DEX:		return &m_siDex; 
			case TYPE_VIT:		return &m_siVit; 
			case TYPE_MAG:		return &m_siMag; 
			case TYPE_HND:		return &m_siHnd; 
			case TYPE_WIS:		return &m_siWis; 
			
			default: return NULL;
		}
	}
	bool Calculation( SI32 _Type, SI32 _Value, cltElixir* _pclPrevElixir = NULL )
	{
		SI16* pType = GetTypePointer( _Type );
		if( NULL == pType )	return false;

		if( _pclPrevElixir ) 
		{
			SI16* pPrevType = _pclPrevElixir->GetTypePointer( _Type );

			if( 0 < _Value )	_Value = min( m_siPoint, _Value );
			else				_Value = max( *pPrevType - *pType, _Value );

			if( *pPrevType				   > *pType	   + _Value ) return false;
			if( _pclPrevElixir->GetPoint() < m_siPoint - _Value ) return false;
		}

		if( 0 > *pType	  + _Value ) return false;
		if( 0 > m_siPoint - _Value ) return false;

		*pType		+= _Value;
		m_siPoint	-= _Value;

		return true;
	}

private:
	cltDate	m_clEndDate;			// ������ ����� ������ �ð�.
	UI08	m_siMultiple;			// ������ ��� ���. 

	SI16	m_siPoint;				// ����Ʈ.
	SI16	m_siStr;				// �ٷ�	
	SI16	m_siDex;				// ���߷�
	SI16	m_siVit;				// ������ 
	SI16	m_siMag;				// ���� 
	SI16	m_siHnd;				// ������. 
	SI16	m_siWis;				// ���� �߰�.
};
// ������ �Ѵ� ��� �ϴ� �����Ͷ� ���⿡ �ø��� ����
#define MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT	3	// �� �����̺�Ʈ���� �����Ҽ� �ִ� �ִ����� �ʵ��޺��ڽ� �ε��� ����
#define MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT	3	// �Ϸ簡���� ���̾�Ʈ ������ Ƚ�� 3ȸ 
#define MAX_MONSTER_NUM_GOONZURACCOONEVENT	3		// �ִ� ���� ���� 3����
#define MAX_HOUR_GOONZURACCOONEVENT	23				// �Ϸ�� 24�ð� ������ 0�ú��� ~23�ñ��� �ð��� ǥ�� �Ѵ� ǥ��ð��� 23�ñ����⶧���� 24�ô� 00 �� 
class RocData
{
public:
	/*void SetRoc(TCHAR* pMapname, SI32 siX, SI32 siY)
	{ 
		SetMapName(pMapname)	;
		SetX(siX);
		SetY(siY);
	}*/
	void SetRoc(SI32 siMapIndex, SI32 siX, SI32 siY)
	{ 
		SetMapindex(siMapIndex)	;	// ����Ǯ�� ���� ���ε����� ����. �׷��� ������ �ش� ���̸��� �����ֱ����ؼ� ���ε����� �ؽ�Ʈ�Ŵ��� ��ȣ�� �ִ´�.
		SetX(siX);
		SetY(siY);
	}
	/*void SetMapName(TCHAR* szMapName)
	{ 
		MStrCpy(m_szMapName, szMapName, sizeof(m_szMapName));
	}*/
	void SetMapindex(SI32 siMapindex){ m_siMapIndex = siMapindex; }
	void SetX(SI32 siX){ m_siX = siX;}
	void SetY(SI32 siY){ m_siY = siY;}

	//TCHAR* GetMapName(){return m_szMapName; }
	SI32 GetMapIndex(){return m_siMapIndex	;}
	SI32 GetX(){ return m_siX; }
	SI32 GetY(){ return m_siY; }
	//TCHAR m_szMapName[50];
	SI32 m_siMapIndex	;
	SI32 m_siX	;
	SI32 m_siY	;
};
class cGoonzuRaccoonEventData
{
public:
	void init()
	{ 
		m_siTime = 0 ;
		m_siMakeMonsterNum = 0 ;
		m_RocData[0].SetRoc(0,0,0)	;
		m_RocData[1].SetRoc(0,0,0)	;
		m_RocData[2].SetRoc(0,0,0)	;
	};
	void SetTime( SI32 siTime){ m_siTime = siTime ;}
	void SetMakeMonsterNum(SI32 siNum){ m_siMakeMonsterNum = siNum ;}
	/*void SetMapName( SI32 siArrayIndex, TCHAR* szMapname)
	{
		m_RocData[siArrayIndex].SetMapName( szMapname );
	}*/
	void SetMapIndex( SI32 siArrayIndex, SI32 siMapIndex)
	{
		m_RocData[siArrayIndex].SetMapindex( siMapIndex );
	}
	void SetMapRocX( SI32 siArrayIndex, SI32 siX)
	{
		m_RocData[siArrayIndex].SetX(siX);
	}
	void SetMapRocY( SI32 siArrayIndex, SI32 siY)
	{
		m_RocData[siArrayIndex].SetY( siY );
	}
	
	SI32 GetTime(){ return m_siTime ; }
	SI32 GetMonsterNum(){ return m_siMakeMonsterNum ; }
	/*TCHAR* GetRocName( SI32 ArrayIndex)
	{
		if ( ArrayIndex >= MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT ) return	0	;
		return m_RocData[ArrayIndex].GetMapName()	;
	}*/
	SI32 GetRocIndex( SI32 ArrayIndex)
	{
		if ( ArrayIndex >= MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT ) return	0	;
		return m_RocData[ArrayIndex].GetMapIndex()	;
	}
	SI32 GetRocX( SI32 ArrayIndex)
	{
		if ( ArrayIndex >= MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT ) return	0	;
		return m_RocData[ArrayIndex].GetX();
	}
	SI32 GetRocY( SI32 ArrayIndex)
	{
		if ( ArrayIndex >= MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT ) return	0	;
		return m_RocData[ArrayIndex].GetY();
	}
	SI32 m_siTime	;			// ���� �ð�
	SI32 m_siMakeMonsterNum	;	// �ش� �ð��� ���� ��ձ����� ����
	RocData m_RocData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT]	;	// ���� ����ǥ ������ 
};


// ������ ���� �αװ��� ����
class cltUserBlockLog
{
public:
	UI08	m_uiWarnMode;							// ������
	UI08	m_uiBlockMode;							// �������
	TCHAR	m_szBlockReason[MAX_BLOCKREASON_SIZE];	// ��������
	TCHAR	m_szStartDate[24];						// ������
	TCHAR	m_szEndDate[24];						// ������

public:
	cltUserBlockLog( void )		{	ZeroMemory(this, sizeof(*this));	}
	~cltUserBlockLog( void )	{}
	
	void Init( void )			{	ZeroMemory(this, sizeof(*this));	}

};

#endif
