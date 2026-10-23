//---------------------------------
// 2003/11/26 김태곤
//---------------------------------
#include "../Client/Client.h"

#include "Char/CharCommon/Char-Common.h"
#include "Quest.h"
#include "Char/CharManager/CharManager.h"	


#include "../resource.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Quest.h"
#include "../Client/Music/Music.h"
#include "../Skill/Skill-Manager.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../SystemNPC/SystemNPC.h"

//------------------------------------
// Lib
//------------------------------------

#include "../Lib/WebHTML/WebHTML.h"
#include "HashTable/HashTable_StringInt.h"


extern cltCommonLogic* pclClient;


//--------------------------------------------
// cltQuestManager
//--------------------------------------------
cltQuestManager::cltQuestManager()
{
	SI32 i, j;

	siKind				= 0;
	siStartConversationNum	= 0;
	siStartConversationCounter = 0;
	siEndConversationNum	= 0;
	siEndConversationCounter = 0;
	siViewState			= 1;
	bNewQuseOpened		= FALSE;
	bNewQuestDoing		= true;
	m_hDlg				= NULL;
 
	DlgRectInitSwitch	= FALSE;
	hPortraitBitmap		= NULL;
	hItemImage			= NULL;

	// 단어 조합 정보를 초기화한다. 
	siWordAssemblyNumber	= 0;
	for(i = 0;i < MAX_WORD_ASSEMBLY_CHAR_NUMBER;i++)
	{
		StringCchCopy(szWordAssemblyString[i], 10, TEXT(""));
	}

	TCHAR* pText = GetTxtFromMgr(2998);
	StringCchCopy(szWordAssemblyString[0], 10, pText);
	pText = GetTxtFromMgr(2999);
	StringCchCopy(szWordAssemblyString[1], 10, pText);
	pText = GetTxtFromMgr(3000);
	StringCchCopy(szWordAssemblyString[2], 10, pText);
	pText = GetTxtFromMgr(3001);
	StringCchCopy(szWordAssemblyString[3], 10, pText);
	pText = GetTxtFromMgr(3002);
	StringCchCopy(szWordAssemblyString[4], 10, pText);
	pText = GetTxtFromMgr(3003);
	StringCchCopy(szWordAssemblyString[5], 10, pText);
	pText = GetTxtFromMgr(3004);
	StringCchCopy(szWordAssemblyString[6], 10, pText);
	pText = GetTxtFromMgr(3005);
	StringCchCopy(szWordAssemblyString[7], 10, pText);
	siWordAssemblyNumber	= 8;



	//---------------------------------
	// 사용자의 아이디를 확보한다.
	//---------------------------------
	siID		= 1;

	siQuestType				= QUEST_TYPE_BEGINNER2;
	siCurrentQuestUnique	= 0;
	siCurrentQuestStep		= QUEST_STEP_GREETING;

	for(i = 0;i < MAX_QUEST_TYPE;i++)
	{
		for(j = 0;j < MAX_QUEST_PER_TYPE_NUMBER;j++)
		{
			siQuestBitInfo[i][j] = 0;
			clQuestTxtInfo[i][j].Init();
		}
	}

	for(i = 0;i < MAX_QUEST_TYPE;i++)
	{
		StringCchCopy(szQuestTypeName[i], 128, TEXT(""));
	}

	pclHashTable	= new HashStringInt(MAX_QUEST_CLEAR_COND_NUMBER, MAX_QUEST_CLEAR_COND_NUMBER);

	class cltquestclearcondinfo{
	public:
		SI32 siCleatCond;
		TCHAR szTxt[32];
	};


	cltquestclearcondinfo clquestclearcond[]={
			QUEST_CLEAR_COND_BONUSZERO			,		TEXT("BONUSZERO"),				
			QUEST_CLEAR_COND_SUMMONBONUS		,		TEXT("SUMMONBONUS"),				
			QUEST_CLEAR_COND_ITEM				,		TEXT("ITEM"),					
			QUEST_CLEAR_COND_KILLMON			,		TEXT("KILLMON"),				
			QUEST_CLEAR_COND_MAKEITEM			,		TEXT("MAKEITEM"),				
			QUEST_CLEAR_COND_OPENCHARSTATUS		,		TEXT("OPENCHARSTATUS"),		
			QUEST_CLEAR_COND_LEVEL				,		TEXT("LEVEL"),				
			QUEST_CLEAR_COND_FULLLIFE			,		TEXT("FULLLIFE"),			
			QUEST_CLEAR_COND_OPENSKILL			,		TEXT("OPENSKILL"),			
			QUEST_CLEAR_COND_SKILL				,		TEXT("SKILL"),				
			QUEST_CLEAR_COND_EQUIP				,		TEXT("EQUIP"),				
			QUEST_CLEAR_COND_SELLNPC			,		TEXT("SELLNPC"),			
			QUEST_CLEAR_COND_MONEY				,		TEXT("MONEY"),				
			QUEST_CLEAR_COND_BANK				,		TEXT("BANK"),				
			QUEST_CLEAR_COND_HUNGRY				,		TEXT("HUNGRY"),				
			QUEST_CLEAR_COND_REPAIRWEAPON		,		TEXT("REPAIRWEAPON"),		
			QUEST_CLEAR_COND_ITEMINFO			,		TEXT("ITEMINFO"),			
			QUEST_CLEAR_COND_NEWS				,		TEXT("NEWS"),				
			QUEST_CLEAR_COND_SKILLBONUSZERO		,		TEXT("SKILLBONUSZERO"),		
			QUEST_CLEAR_COND_GOMAP				,		TEXT("GOMAP"),				
			QUEST_CLEAR_COND_LAMPGOMAP			,		TEXT("GOMAP"),		// 요술램프 마을 순회
			QUEST_CLEAR_COND_GETQUEST			,		TEXT("GETQUEST"),			
			QUEST_CLEAR_COND_CLEARQUEST			,		TEXT("CLEARQUEST"),			
			QUEST_CLEAR_COND_SELLMARKET			,		TEXT("SELLMARKET"),			
			QUEST_CLEAR_COND_BASICWEAPONSKILL	,		TEXT("BASICWEAPONSKILL"),	
			QUEST_CLEAR_COND_STRUCTUREOPEN		,		TEXT("STRUCTUREOPEN"),		
			QUEST_CLEAR_COND_ITEMINFODLG		,		TEXT("ITEMINFODLG"),		
			QUEST_CLEAR_COND_SELLSWORDNPC		,		TEXT("SELLSWORDNPC"),		
			QUEST_CLEAR_COND_OTHERMINIHOME		,		TEXT("OTHERMINIHOME"),		
			QUEST_CLEAR_COND_OPENHEADCHAT		,		TEXT("OPENHEADCHAT"),		
			QUEST_CLEAR_COND_BUYFROMPBSTORE		,		TEXT("BUYFROMPBSTORE"),		
			QUEST_CLEAR_COND_OPENPBSTORE		,		TEXT("OPENPBSTORE"),		
			QUEST_CLEAR_COND_OPENSTOCKMARKET	,		TEXT("OPENSTOCKMARKET"),	
			QUEST_CLEAR_COND_STOCK				,		TEXT("STOCK"),				
			QUEST_CLEAR_COND_OPENCITYHALL		,		TEXT("OPENCITYHALL"),		
			QUEST_CLEAR_COND_APPLYVILLAGE		,		TEXT("APPLYVILLAGE"),
			QUEST_CLEAR_COND_USEFAN				,		TEXT("USEFAN"),				
			QUEST_CLEAR_COND_USEWENNYOLDMAN		,		TEXT("USEWENNYOLDMAN"),		
			QUEST_CLEAR_COND_HORSE				,		TEXT("HORSE"),				
			QUEST_CLEAR_COND_OPENHUNT			,		TEXT("OPENHUNT"),			
			QUEST_CLEAR_COND_USEWARPTICKET1		,		TEXT("USEWARPTICKET1"),		
			QUEST_CLEAR_COND_USEWARPTICKET2		,		TEXT("USEWARPTICKET2"),		
			QUEST_CLEAR_COND_SUMMON				,		TEXT("SUMMON"),				
			QUEST_CLEAR_COND_SUMMON2			,		TEXT("SUMMON2"),				
			QUEST_CLEAR_COND_BROADCAST			,		TEXT("BROADCAST"),			
			QUEST_CLEAR_COND_SKILLMASTER		,		TEXT("SKILLMASTER"),		
			QUEST_CLEAR_COND_MAKEFRIEND			,		TEXT("MAKEFRIEND"),			
			QUEST_CLEAR_COND_LOOKPANSEO			,		TEXT("LOOKPANSEO"),			
			QUEST_CLEAR_COND_OPENVILLAGEBOARD	,		TEXT("OPENVILLAGEBOARD"),	
			QUEST_CLEAR_COND_FAMELEVEL			,		TEXT("FAMELEVEL"),			
			QUEST_CLEAR_COND_USEITEM			,		TEXT("USEITEM"),			
			QUEST_CLEAR_COND_SENDITEMBYPOST		,		TEXT("SENDITEMBYPOST"),
			QUEST_CLEAR_COND_VILLAGEDETAILINFO	,		TEXT("VILLAGEDETAILINFO"),	
			QUEST_CLEAR_COND_USEWENNYOLDMAN2	,		TEXT("USEWENNYOLDMAN2"),	
			QUEST_CLEAR_COND_FEEDHORSE1			,		TEXT("FEEDHORSE1"),	
			QUEST_CLEAR_COND_FEEDHORSE2			,		TEXT("FEEDHORSE2"),	
			QUEST_CLEAR_COND_HORSESPEED			,		TEXT("HORSESPEED"),	
			QUEST_CLEAR_COND_ITEMMALL			,		TEXT("ITEMMALL"),			
			QUEST_CLEAR_COND_GGOMGICHANGE		,		TEXT("GGOMGICHANGE"),		
			QUEST_CLEAR_COND_BAG				,		TEXT("BAG"),				
			QUEST_CLEAR_COND_EQUIPPOS			,		TEXT("EQUIPPOS"),			
			QUEST_CLEAR_COND_FISH				,		TEXT("FISH"),				
			QUEST_CLEAR_COND_FARM				,		TEXT("FARM"),
			QUEST_CLEAR_COND_MINE				,		TEXT("MINE"),
			QUEST_CLEAR_COND_BERESIDENT			,		TEXT("BERESIDENT"),			
			QUEST_CLEAR_COND_VILLAGEHUNT		,		TEXT("VILLAGEHUNT"),		
			QUEST_CLEAR_COND_GOGU				,		TEXT("GOGU"),				
			QUEST_CLEAR_COND_REVIVESUMMON		,		TEXT("REVIVESUMMON"),				
			QUEST_CLEAR_COND_SCROLL				,		TEXT("SCROLL"),				
			QUEST_CLEAR_COND_MAXUPSCROLL		,		TEXT("MAXUPSCROLL"),		
			QUEST_CLEAR_COND_SELLTRADER			,		TEXT("SELLTRADER"),			
			QUEST_CLEAR_COND_REVIVESUMMON		,		TEXT("REVIVESUMMON"),
			QUEST_CLEAR_COND_DAILYQUEST			,		TEXT("DAILYQUEST"),
			QUEST_CLEAR_COND_SPECIALQUEST		,		TEXT("SPECIALQUEST"),
			QUEST_CLEAR_COND_SUMMONEVOLUTION	,		TEXT("SUMMONEVOLUTION"),
			QUEST_CLEAR_COND_SUMMONSCROLL		,		TEXT("SUMMONSCROLL"),
			QUEST_CLEAR_COND_ITEMMALL			,		TEXT("ITEMMALL"),
			QUEST_CLEAR_COND_SUMMONBATTLE		,		TEXT("SUMMONBATTLE"),
			QUEST_CLEAR_COND_GONPC				,		TEXT("GONPC"),
			QUEST_CLEAR_COND_SETFATHER			,		TEXT("SETFATHER"),
			QUEST_CLEAR_COND_CLICKITEMINFO		,		TEXT("CLICKITEMINFO"),
			QUEST_CLEAR_COND_OPENMAKEITEM		,		TEXT("OPENMAKEITEM"),
			QUEST_CLEAR_COND_CLICKMAKEBUTTON	,		TEXT("CLICKMAKEBUTTON"),
            QUEST_CLEAR_COND_TUTORIALMAKEITEM	,		TEXT("TUTORIALMAKEITEM"),
			QUEST_CLEAR_COND_ITEMCHECK			,		TEXT("ITEMCHECK"),
			QUEST_CLEAR_COND_UPITEMTOMARKET		,		TEXT("UPITEMTOMARKET"),				
			QUEST_CLEAR_COND_OPENENCHANTITEM	,		TEXT("OPENENCHANTITEM"),
			QUEST_CLEAR_COND_SETENCHANTITEM		,		TEXT("SETENCHANTITEM"),
			QUEST_CLEAR_COND_SETENCHANTELEMENT	,		TEXT("SETENCHANTELEMENT"),
			QUEST_CLEAR_COND_ENCHANTITEM		,		TEXT("ENCHANTITEM"),
			QUEST_CLEAR_COND_WARPTOPALACE		,		TEXT("WARPTOPALACE"),
			QUEST_CLEAR_COND_PRIVATETRADE		,		TEXT("PRIVATETRADE"),
			QUEST_CLEAR_COND_SELLFROMPBSTORE	,		TEXT("SELLFROMPBSTORE"),
			QUEST_CLEAR_COND_MAKEBOOK			,		TEXT("MAKEBOOK"),
			QUEST_CLEAR_COND_HUNTMAPINFODLG		,		TEXT("HUNTMAPINFODLG"),
			QUEST_CLEAR_COND_SEARCHMONSTERINFO	,		TEXT("SEARCHMONSTERINFO"),
			QUEST_CLEAR_COND_PARTYHUNTING		,		TEXT("PARTYHUNTING"),
			QUEST_CLEAR_COND_MAGICINFODLG		,		TEXT("MAGICINFODLG"),
			QUEST_CLEAR_COND_OPENHOUSE			,		TEXT("OPENHOUSE"),
			QUEST_CLEAR_COND_OPENREALESTATEMARKET,		TEXT("OPENREALESTATEMARKET"),
			QUEST_CLEAR_COND_BLACKARMY			,		TEXT("BLACKARMY"),
			QUEST_CLEAR_COND_OPENHORSEMARKET	,		TEXT("OPENHORSEMARKET"),
			QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK,		TEXT("SUMMONEVOLUTIONRANK"),
			QUEST_CLEAR_COND_REVIVESUMMONBYDOLL	,		TEXT("REVIVESUMMONBYDOLL"),
			QUEST_CLEAR_COND_OPENHOZODLG		,		TEXT("OPENHOZODLG"),
			QUEST_CLEAR_COND_OPENBYUNGZODLG		,		TEXT("OPENBYUNGZODLG"),
			QUEST_CLEAR_COND_OPENGONGZODLG		,		TEXT("OPENGONGZODLG"),
			QUEST_CLEAR_COND_OPENIZODLG			,		TEXT("OPENIZODLG"),
			QUEST_CLEAR_COND_OPENGOONZUDLG		,		TEXT("OPENGOONZUDLG"),
			QUEST_CLEAR_COND_VOTEKING			,		TEXT("VOTEKING"),
			QUEST_CLEAR_COND_USEWARPTICKET3		,		TEXT("USEWARPTICKET3"),
			QUEST_CLEAR_COND_USEMAGIC			,		TEXT("USEMAGIC"),
			QUEST_CLEAR_COND_BUYTOPBSTORE		,		TEXT("BUYTOPBSTORE"),
			QUEST_CLEAR_COND_NONE				,		TEXT("NONE"),
			QUEST_CLEAR_COND_ENCHANTANYITEM		,		TEXT("ENCHANTANYITEM"),
			QUEST_CLEAR_COND_OPENFRIEND			,		TEXT("OPENFRIEND"),
			QUEST_CLEAR_COND_OPENMASTER			,		TEXT("OPENMASTER"),
			QUEST_CLEAR_COND_OPENMAKEREQUEST	,		TEXT("OPENMAKEREQUEST"),
			QUEST_CLEAR_COND_MAKEREQUESTSTART	,		TEXT("MAKEREQUESTSTART"),
			QUEST_CLEAR_COND_OPENNAMINGQUEST	,		TEXT("OPENNAMINGQUEST"),
			QUEST_CLEAR_COND_DISASSEMBLEITEM	,		TEXT("DISASSEMBLEITEM"),
			QUEST_CLEAR_COND_SUMMONACTIVE		,		TEXT("SUMMONACTIVE"),
			QUEST_CLEAR_COND_SUMMONDIE			,		TEXT("SUMMONDIE"),
			QUEST_CLEAR_COND_SUMMONCHANGESTATUS	,		TEXT("SUMMONCHANGESTATUS"),
			QUEST_CLEAR_COND_PARTYKILLMON		,		TEXT("PARTYKILLMON"),
			QUEST_CLEAR_COND_SUMMONATTACK		,		TEXT("SUMMONATTACK"),
			QUEST_CLEAR_COND_SUMMONLEVELUP		,		TEXT("SUMMONLEVELUP"),
			QUEST_CLEAR_COND_HUNT_ITEM			,		TEXT("HUNT_ITEM"),
			QUEST_CLEAR_COND_MAKEINTBOOK		,		TEXT("MAKEINTBOOK"),
			QUEST_CLEAR_COND_SETGREETING		,		TEXT("SETGREETING"),
			QUEST_CLEAR_COND_GOHUNTMAP			,		TEXT("GOHUNTMAP"),
			QUEST_CLEAR_COND_MAGICACTIVE		,		TEXT("MAGICACTIVE"),
			QUEST_CLEAR_COND_OPENGUILD			,		TEXT("OPENGUILD"),
			QUEST_CLEAR_COND_APPLYGUILD			,		TEXT("APPLYGUILD"),
			QUEST_CLEAR_COND_LOOKPRISON			,		TEXT("LOOKPRISON"),
			QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE		,	TEXT("OPENGOONZUDLG_NOTICE"),
			QUEST_CLEAR_COND_MAKEPARTY			,		TEXT("MAKEPARTY"),
			QUEST_CLEAR_COND_SORTRESIDENT		,		TEXT("SORTRESIDENT"),
			QUEST_CLEAR_COND_CLICKITEMPROPERTY	,		TEXT("CLICKITEMPROPERTY"),
			QUEST_CLEAR_COND_OPENSKILLETC		,		TEXT("OPENSKILLETC"),
			QUEST_CLEAR_COND_MINENOSKILL		,		TEXT("MINENOSKILL"),
			QUEST_CLEAR_COND_SITDOWN			,		TEXT("SITDOWN"),
			QUEST_CLEAR_COND_NPCINFO			,		TEXT("NPCINFO"),
			QUEST_CLEAR_COND_HITMON				,		TEXT("HITMON"),
			QUEST_CLEAR_COND_OPENGUILDUSER		,		TEXT("OPENGUILDUSER"),
			QUEST_CLEAR_COND_SEARCHSTOCKINFO	,		TEXT("SEARCHSTOCKINFO"),
			QUEST_CLEAR_COND_CLICKSTOCKOFFER	,		TEXT("CLICKSTOCKOFFER"),
			QUEST_CLEAR_COND_CLICKCITYHALLFUND	,		TEXT("CLICKCITYHALLFUND"),
			QUEST_CLEAR_COND_CLICKGENERALMETTING,		TEXT("CLICKGENERALMETTING"),
			QUEST_CLEAR_COND_CLICKPERSONALSHOP	,		TEXT("CLICKPERSONALSHOP"),
			QUEST_CLEAR_COND_OPENITEMMALL		,		TEXT("OPENITEMMALL"),
			QUEST_CLEAR_COND_ANGELLISTCLICK		,		TEXT("ANGELLISTCLICK"),
			QUEST_CLEAR_COND_POLICESETCLICK		,		TEXT("POLICESETCLICK"),
			QUEST_CLEAR_COND_SEARCHFARM			,		TEXT("SEARCHFARM"),
			QUEST_CLEAR_COND_FARMNOSKILL		,		TEXT("FARMNOSKILL"),
			QUEST_CLEAR_COND_LEASECLICK			,		TEXT("LEASECLICK"),
			QUEST_CLEAR_COND_GETMAGICLAMPQUEST	,		TEXT("GETMAGICLAMPQUEST"),
			QUEST_CLEAR_COND_OPENQUESTDIALOG	,		TEXT("OPENQUESTDIALOG"),
			QUEST_CLEAR_COND_FISHNOSKILL		,		TEXT("FISHNOSKILL"),
			QUEST_CLEAR_COND_ISLEARNED_MANUFACTURE,		TEXT("ISLEARNED_MANUFACTURE"),
			QUEST_CLEAR_COND_ISGET_MANUFACTUREQUEST,	TEXT("ISGET_MANUFACTUREQUEST"),
			QUEST_CLEAR_COND_REQUESTMENTORSUCCESS,		TEXT("REQUESTMENTORSUCCESS"),
			QUEST_CLEAR_COND_TAKEPUPILS,				TEXT("TAKEPUPILS"),
			QUEST_CLEAR_COND_CHATTOBEGGINER,			TEXT("CHATTOBEGGINER"),
			QUEST_CLEAR_COND_CHECKSTATUS,				TEXT("CHECKSTATUS"),
			QUEST_CLEAR_COND_ALLWEAPONSKILL,			TEXT("ALLWEAPONSKILL"),
			QUEST_CLEAR_COND_MEMOCLICK,					TEXT("MEMOCLICK"),
			QUEST_CLEAR_COND_MOVETOGROUPCLICK,			TEXT("MOVETOGROUPCLICK"),
			QUEST_CLEAR_COND_EQUIPBAG,					TEXT("EQUIPBAG"),
			QUEST_CLEAR_COND_KILLDARKSOLDIER,			TEXT("KILLDARKSOLDIER"),
			QUEST_CLEAR_COND_MAKE_RAREITEM,				TEXT("MAKE_RAREITEM"),
			QUEST_CLEAR_COND_FULL_ORGAN,				TEXT("FULL_ORGAN"),
			QUEST_CLEAR_COND_TWOWAYKILLMON,				TEXT("TWOWAYKILLMON"),
			QUEST_CLEAR_COND_BUYMARKET,					TEXT("BUYMARKET"),
			QUEST_CLEAR_COND_OPENFATHERDIALOG,			TEXT("OPENFATHERDIALOG"),
			QUEST_CLEAR_COND_ORDERTOMARKET,				TEXT("ORDERTOMARKET"),
			QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG,		TEXT("OPENPOSTOFFICEDIALOG"),
			QUEST_CLEAR_COND_MAKELEVELITEM,				TEXT("MAKELEVELITEM"),
			QUEST_CLEAR_COND_TAKEEQUIPLEVELWEAPON,		TEXT("TAKEEQUIPLEVELWEAPON"),
			QUEST_CLEAR_COND_GETELEMENT_RESTORE,		TEXT("GETELEMENT_RESTORE"),
			QUEST_CLEAR_COND_GETPOWDER_RESTORE,			TEXT("GETPOWDER_RESTORE"),
			QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON,	TEXT("ENCHANTEQUIPLEVELWEAPON"),
			QUEST_CLEAR_COND_TAKEITEM_MATERIAL,			TEXT("TAKEITEM_MATERIAL"),
			QUEST_CLEAR_COND_TAKEWEAPONHAVESLOT,		TEXT("TAKEWEAPONHAVESLOT"),
			QUEST_CLEAR_COND_ARTIFACTMIXWEAPON,			TEXT("ARTIFACTMIXWEAPON"),
			QUEST_CLEAR_COND_MAKECRYSTAL,				TEXT("MAKECRYSTAL"),
			QUEST_CLEAR_COND_TEACHERSET,				TEXT("TEACHERSET"),
			QUEST_CLEAR_COND_SUMMONBONUSZERO,			TEXT("SUMMONBONUSZERO"),
			QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM,		TEXT("TUTORIALMAKELEVELITEM"),
			QUEST_CLEAR_COND_SWORDSKILLCLICK,			TEXT("SWORDSKILLCLICK"),
			QUEST_CLEAR_COND_LEARNWEAPONSKILL,			TEXT("LEARNWEAPONSKILL"),
			QUEST_CLEAR_COND_PVPWIN,					TEXT("PVPWIN"),
			QUEST_CLEAR_COND_PVPLEAGUERESERVE,			TEXT("PVPLEAGUERESERVE"),
			QUEST_CLEAR_COND_PVPLEAGUEWIN,				TEXT("PVPLEAGUEWIN"),
			QUEST_CLEAR_COND_TWOWAYKILLMONBONUS,		TEXT("TWOWAYKILLMONBONUS"),
			QUEST_CLEAR_COND_ARTIFACTMIX,				TEXT("ARTIFACTMIX"),
			QUEST_CLEAR_COND_SETMYKEY,					TEXT("SETMYKEY"),
			QUEST_CLEAR_COND_SUMMONHOUSE_INPUT,			TEXT("SUMMONHOUSE_INPUT"),
			QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT,		TEXT("SUMMONHOUSE_OUTPUT"),
			QUEST_CLEAR_COND_MARKETCONDITION,			TEXT("MARKETCONDITION"),
			QUEST_CLEAR_COND_ENCHANTEQUIPPOS,			TEXT("ENCHANTEQUIPPOS"),
			QUEST_CLEAR_COND_BUFFCLICK,					TEXT("BUFFCLICK"),
			QUEST_CLEAR_COND_GONPC_INFORMATION,			TEXT("GONPC_INFORMATION"),
			QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS,	TEXT("TRANSMONSTERINFO"),
			QUEST_CLEAR_COND_MAPKILLMON,				TEXT("MAPKILLMON"),
			QUEST_CLEAR_COND_MAKESKILLEXPUP,			TEXT("MAKESKILLEXPUP"),
			QUEST_CLEAR_COND_SEARCH_TOTALMAP,			TEXT("SEARCH_TOTALMAP"),
			QUEST_CLEAR_COND_USE_SKILLBOOK,				TEXT("USE_SKILLBOOK"),
	};

	SI32 clearcondnum = sizeof(clquestclearcond) / sizeof(cltquestclearcondinfo);

	for(i = 0;i < clearcondnum;i++)
	{
		pclHashTable->Insert(clquestclearcond[i].szTxt,			clquestclearcond[i].siCleatCond, 0);
	}


	if ( pclClient->GameMode != GAMEMODE_MAPEDIT )
	{
		// Person 퀘스트 정보의 비트와 퀘스트의 Unique를 연결한다. 
		ConnectBitWithUnique();

		// 퀘스트 텍스트를 불러온다. 
		LoadQuestText();

	}

	// 퀘스트 정보를 서버로 부터 받았다.
	bQuestInfoChangedSwitch = true;

	m_pQuestText = NULL;

}

cltQuestManager::~cltQuestManager()
{
	if(pclHashTable)
	{
		delete pclHashTable;
		pclHashTable = NULL;
	}
}


// NPCKind의 퀘스트에 대한 간단한 설명.
bool cltQuestManager::GetQuestExplain(SI32 kind, TCHAR* ptext, SI16 txtSize)
{
	StringCchCopy(ptext, txtSize, TEXT(""));

	TCHAR explain[512] = TEXT("") ;

	cltSystemNPC clinfo;
	if(pclClient->pclSystemNPCManager->FindSystemNPCInfoFromNPCKind(kind, &clinfo) == true)
	{

		if(pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_HIREABLE))
		{
			TCHAR*	pText			= GetTxtFromMgr(3006);
			TCHAR	szBuffer[512]	= { '\0', };
			
			clinfo.GetExplainText( szBuffer, 512 );
			//KHY - 0816 - 마을레벨 삭제 (NPC고용관련)
			StringCchPrintf(explain, 512, pText, szBuffer);
			// pclClient->pclKindInfoSet->GetHireVillageLevel(kind))
		}
		else
		{
			TCHAR*	pText			= GetTxtFromMgr(3007);
			TCHAR	szBuffer[512]	= { '\0', };

			clinfo.GetExplainText( szBuffer, 512 );
			StringCchPrintf(explain, 512, pText, szBuffer);
		}
		StringCchCopy(ptext , txtSize, explain);

		return true;
	}

	return false;
}

// NPCKind가 부여하는 퀘스트의 타입을 얻어온다. 
SI32 cltQuestManager::GetQuestTypeFromNPCKind(SI32 kind)
{
	cltSystemNPC clinfo;
	if ( true == pclClient->pclSystemNPCManager->FindSystemNPCInfoFromNPCKind(kind, &clinfo) )
	{
		return clinfo.m_siQuestType;
	}

	return 0;
}

// 퀘스트 타입을 근거로 이 퀘스트를 부여하는 NPC의  kind를 찾는다. 
SI32 cltQuestManager::GetNPCKindFromQuestType(SI32 questtype)
{
	cltSystemNPC clinfo;
	if ( true == pclClient->pclSystemNPCManager->FindSystemNPCInfoFromQuestType(questtype, &clinfo) )
	{
		return clinfo.m_siKind;
	}

	return 0;
}

// 퀘스트의 유니크를 근거로 해당 비트를 찾는다.
SI32 cltQuestManager::GetBitFromUnique(SI32 questtype, SI32 questunique)
{
	SI32 i;

	for(i = 0; i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		if(siQuestBitInfo[questtype][i] == questunique)
		{
			return i;
		}
	}

	return -1;
}

// 비트를 근거로 유니크를 찾는다. 
SI32 cltQuestManager::GetUniqueFromBit(SI32 questtype, SI32 bit)
{
	return siQuestBitInfo[questtype][bit];
}

// 퀘스트를 추가한다. 
void cltQuestManager::Add(SI32 type, SI32 unique, 
						  cltQuestInitConditionInfo* pclinitcond, 
						  cltQuestClearConditionInfo* pclclearcond, 
						  cltQuestRewardInfo* pclreward, 
						  TCHAR* titletext, 
						  TCHAR* pimagefilename)
{

	SI32 i, ref = -1;
	for(i = 0;i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		if(clQuestInfo[type][i].siUnique == 0)
		{
			ref = i;
			break;
		}
	}


	if(ref < 0)return ;

	clQuestInfo[type][ref].Set(type, unique, pclinitcond, pclclearcond, pclreward, titletext, pimagefilename);
}

// 퀘스트의 비트를 가지고 일치하는 유니크를 찾는다.
SI32 cltQuestManager::GetQuestUniqueFromBit(SI32 questtype, SI32 minbit)
{

	if(questtype < 0 || questtype >= MAX_QUEST_TYPE)return 0;
	if(minbit < 0 || minbit >= MAX_QUEST_PER_TYPE_NUMBER)return 0;

	return siQuestBitInfo[questtype][minbit];
}

// 텍스트를 화면에 보여준다. 
void cltQuestManager::ShowText(HWND hDlg, bool forceswitch)
{
	HWND hwnd;

	static SI32 oldcurrentquestunique	= 0;
	static SI32 oldcurrentqueststep		= 0;
	static SI32 oldpara2				= 0;

	SI32 para2	= pclClient->pclCM->CR[siID]->pclCI->clQuestInfo.clUnit[siQuestType].siQuestPara2;
	if(forceswitch == false)
	{
		// 달라진 내용이 없다면 출력하지 않는다. 
		if(oldcurrentquestunique == siCurrentQuestUnique
			&& oldcurrentqueststep	== 	siCurrentQuestStep
			&& oldpara2				== para2)
		{
			return ;
		}
	}

	// 출력한 정보를 기억해 놓는다. (다시 출력하지 않기 위해. ) 
	oldcurrentquestunique	= siCurrentQuestUnique;
	oldcurrentqueststep		= siCurrentQuestStep;
	oldpara2				= para2;

	// 임무 종료시 효과음 연주. 
	if( siCurrentQuestStep == QUEST_STEP_CLEAR && siCurrentQuestUnique != 0 )
	{
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
	}

	if(siCurrentQuestUnique)
	{
		//--------------------
		// 타이틀을 보여준다. 
		//--------------------
		cltQuestInfo* pclquestinfo = GetQuestInfo(siCurrentQuestUnique);
		if(pclquestinfo)
		{
			const TCHAR* pkindname = pclClient->pclKindInfoSet->GetName(siKind);
			if(pkindname)
			{
				TCHAR buffer[256];
				StringCchPrintf(buffer, 256, TEXT("%s - %s"), pkindname, pclquestinfo->szTitleText);
				SetWindowText(hDlg, buffer);
			}
		}

		//---------------------
		// 본문을 보여준다. 
		//---------------------
		if(pclquestinfo)
		{

			hwnd = GetDlgItem(hDlg, IDC_STATIC_QUEST_EXPLAIN); 

			//==================================
			TCHAR  FullPathName[MAX_PATH];
			if(GetText(siCurrentQuestUnique, siCurrentQuestStep, FullPathName) == true)
			{
				DisplayHTMLPage(hwnd, FullPathName);
			}
			else
			{
				DisplayHTMLStr(hwnd, TEXT("NOTEXT"));		// 임시로 Text를 설정하도록 한다.
			}
			//==================================

			//---------------------------
			// 임무 부여 조건을 보여준다. 
			//---------------------------
			TCHAR buffer[1024] = TEXT("");
			if(pclquestinfo->clInitConditionInfo.siLevel)
			{
				TCHAR temp[256];

				TCHAR* pText = GetTxtFromMgr(3008);
				StringCchPrintf(temp, 256, pText, pclquestinfo->clInitConditionInfo.siLevel);
				StringCchCat(buffer, 1024, temp);
			}
			if(pclquestinfo->clInitConditionInfo.siSkillUnique)
			{
				TCHAR skillname[128];
				if(pclClient->pclSkillManager->GetSkillName(pclquestinfo->clInitConditionInfo.siSkillUnique, skillname) == true)
				{
					TCHAR temptext[128];
					TCHAR* pText = GetTxtFromMgr(3009);
					StringCchPrintf(temptext, 128, pText, skillname, pclquestinfo->clInitConditionInfo.siSkillLevel);

					StringCchCat(buffer, 1024, temptext);
				}
			}
			hwnd = GetDlgItem(hDlg, IDC_EDIT_QUEST_CONDITION); 
			SetWindowText(hwnd, buffer);

		}



		//-----------------------
		// 현재 상태를 보여준다. 
		//----------------------
		TCHAR szcurstatustext[1024] =TEXT("");
		pclClient->pclQuestManager->GetCurStatusText(siID, siQuestType, siCurrentQuestUnique, szcurstatustext, 1024);

		hwnd = GetDlgItem(hDlg, IDC_EDIT_QUEST_STATUS); 
		SetWindowText(hwnd, szcurstatustext);

	}
	else
	{
		hwnd = GetDlgItem(hDlg, IDC_STATIC_QUEST_EXPLAIN); 

		//==================================
		TCHAR  FullPathName[MAX_PATH];
		if(GetText(0, 0, FullPathName) == true)
		{
			DisplayHTMLPage(hwnd, FullPathName);			  // 임시로 Text를 설정하도록 한다.
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(3010);
			DisplayHTMLStr(hwnd, pText);		// 임시로 Text를 설정하도록 한다.
		}
		//==================================
	}

	hwnd = GetDlgItem(hDlg, IDC_EDIT_QUESTREWARD); 

	TCHAR buffer[1024]=TEXT("");
	cltQuestInfo* pclquestinfo = GetQuestInfo(siCurrentQuestUnique);
	if(pclquestinfo)
	{
		pclquestinfo->clQuestRewardInfo.GetExplainText(buffer, 1024);
	}

	SetWindowText(hwnd, buffer);

	// 이미지 출력이었지만 제거했음 
}

// questunique를 가지고 적절한 퀘스트 정보를 찾는다. 
cltQuestInfo* cltQuestManager::GetQuestInfo(SI32 questunique)
{
	SI32 i, j;

	for(i = 0;i < MAX_QUEST_TYPE;i++)
		for(j=0; j< MAX_QUEST_PER_TYPE_NUMBER;j++)
		{
			if(clQuestInfo[i][j].siUnique == questunique)
			{
				return &clQuestInfo[i][j];
			}
		}

		return NULL;
}


// 퀘스트의 유니크와 스텝에 따라 적절한 대사를 얻어온다.
bool cltQuestManager::GetText(SI32 questunique, SI32 queststep, TCHAR *fullpathname)
{

	TCHAR  filename[128];

	//=====================================================
	// CURRENT PATH를 구한다.
	//=====================================================
	fullpathname[0] = NULL;
	_tgetcwd( fullpathname, MAX_PATH );
	int Length = _tcslen(fullpathname);
	if(fullpathname[Length-1] != '\\')	StringCchCat(fullpathname, MAX_PATH, TEXT("\\"));
	StringCchCat(fullpathname, MAX_PATH, TEXT("Quest\\"));
	//=====================================================

	if(questunique == 0 && queststep == 0)
	{
		StringCchCat(fullpathname, MAX_PATH, TEXT("nomore.htm"));
	}
	else
	{
		cltQuestInfo* pclquestinfo = GetQuestInfo(questunique);
		if(pclquestinfo == NULL)	
		{
			memset(fullpathname, 0, MAX_PATH);
			return false;
		}


		switch(queststep)
		{
		case QUEST_STEP_GREETING:
			{
				StringCchPrintf(filename, 128, TEXT("%s\\%03dg.htm"), szQuestTypeName[pclquestinfo->siType], questunique);
			}
			break;
		case QUEST_STEP_MIDDLE:
			{
				StringCchPrintf(filename, 128, TEXT("%s\\%03dm.htm"), szQuestTypeName[pclquestinfo->siType], questunique);
			}
			break;
		case QUEST_STEP_CLEAR:
			{
				StringCchPrintf(filename, 128, TEXT("%s\\%03dc.htm"), szQuestTypeName[pclquestinfo->siType], questunique);
			}
			break;
		}

		StringCchCat(fullpathname, MAX_PATH, filename);
	}

	FILE *fp = _tfopen(fullpathname, TEXT("rt"));
	if(fp == NULL) 
	{
		memset(fullpathname, 0, MAX_PATH);
		//MessageBox(NULL, fullpathname, TEXT("FILE NOT FOUND"), MB_OK|MB_TOPMOST);
		return false;
	}
	if(fp)	   fclose(fp);

	return true;
}

// 진행중인 퀘스트를 확보한다. 
void cltQuestManager::GetCurrentQuestInfo()
{
	SI32 oldquestunique = siCurrentQuestUnique;

	// 진행중인 퀘스트가 있다 
	// 진행중이던 퀘스트를 수행한다. 
	if(pclClient->pclCM->CR[siID]->pclCI->clQuestInfo.clUnit[siQuestType].siCurQuestUnique > 0)
	{
		siCurrentQuestUnique	= pclClient->pclCM->CR[siID]->pclCI->clQuestInfo.clUnit[siQuestType].siCurQuestUnique;
		siCurrentQuestStep		= pclClient->pclCM->CR[siID]->pclCI->clQuestInfo.clUnit[siQuestType].siCurQuestStep;
	}
	// 진행중인 퀘스트가 없다. 
	// 완료하지 않은 가장 낮은 퀘스트를 수행한다. 
	else
	{
		// 0인 가장 낮은 비트. 
		SI32 minbit = pclClient->pclCM->CR[siID]->GetMinBit(siQuestType);
		if(minbit >= 0)
		{
			siCurrentQuestUnique	= GetQuestUniqueFromBit(siQuestType, minbit);
			siCurrentQuestStep		= QUEST_STEP_GREETING;
		}
	}

	// 퀘스트가 바뀌었다면 이미지도 삭제한다. 
	if(oldquestunique != siCurrentQuestUnique)
	{
		// 참고 이미지를 삭제한다. 
		if(hItemImage)
		{
			DeleteObject(hItemImage);
			hItemImage = NULL;
		}

	}

}


// 퀘스트 정보를 보고 텍스트를 구성한다.
bool cltQuestManager::GetCurStatusText(cltCharClient* pclchar, SI16 questtype, cltQuestInfoCommon* pclquestinfo, TCHAR* ptext, SI16 txtSize)
{
	TCHAR buffer[1024];


	if(ptext == NULL)return false;
	StringCchCopy(ptext, txtSize,  TEXT(""));

	cltPersonQuestUnitInfo* pclunit = &pclchar->pclCI->clQuestInfo.clUnit[questtype];

	switch(pclquestinfo->clQuestClearConditionInfo.siFlag)
	{
	case QUEST_CLEAR_COND_ITEM:
	case QUEST_CLEAR_COND_ITEMCHECK:
		{
			// 확보해야할 아이템
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num		= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3011);
			StringCchPrintf(buffer, 1024, pText, pitemname);
			StringCchCat(ptext, txtSize, buffer);

			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclchar->pclCI->clCharItem.GetItemNum(itemunique), num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;
	case QUEST_CLEAR_COND_HUNT_ITEM:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num		= pclquestinfo->clQuestClearConditionInfo.siPara2;

			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(10086);
			StringCchPrintf(buffer, 1024, pText, pitemname);
			StringCchCat(ptext, txtSize, buffer);

			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;
	case QUEST_CLEAR_COND_MAKEITEM:
	case QUEST_CLEAR_COND_TUTORIALMAKEITEM:
		{
			// 확보해야할 아이템
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num		= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3012);
			StringCchPrintf(buffer, 1024, pText, pitemname);
			StringCchCat(ptext, txtSize, buffer);

			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;
	case QUEST_CLEAR_COND_USEITEM:
		{
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num		= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3013);
			StringCchPrintf(buffer, 1024, pText, pitemname);
			StringCchCat(ptext, txtSize, buffer);

			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_BUYFROMPBSTORE:
		{
			TCHAR* pText = GetTxtFromMgr(3014);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_APPLYVILLAGE:
		{
			TCHAR* pText = GetTxtFromMgr(3015);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_USEFAN:
		{
			TCHAR* pText = GetTxtFromMgr(3017);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_SENDITEMBYPOST:
		{
			TCHAR* pText = GetTxtFromMgr(3018);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_REVIVESUMMON:
		{
			TCHAR* pText = GetTxtFromMgr(3019);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_ENCHANTANYITEM:
		{
			TCHAR* pText = GetTxtFromMgr(6963);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_USEWARPTICKET1:
		{
			TCHAR* pText = GetTxtFromMgr(3020);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_USEWARPTICKET2:
		{
			TCHAR* pText = GetTxtFromMgr(3021);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_USEWENNYOLDMAN:
		{
			TCHAR* pText = GetTxtFromMgr(3022);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_OPENPBSTORE:
		{
			TCHAR* pText = GetTxtFromMgr(3023);
			StringCchPrintf(buffer, 1024, pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_KILLMON:
		{
			// 처치해야 할 몬스터 확보.
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pkindname = pclClient->pclKindInfoSet->pclKI[kind]->GetName();
			if(pkindname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3024);//잡아야할 몬스터
			StringCchPrintf(buffer, 1024, pText, pkindname);
			StringCchCat(ptext, txtSize, buffer);

			// 처치해야할 수량과 현재 수량 확보. 
			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_HITMON:
		{
			// 때려야 할 몬스터 확보.
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pkindname = pclClient->pclKindInfoSet->pclKI[kind]->GetName();
			if(pkindname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3024);//때려야할 몬스터
			StringCchPrintf(buffer, 1024, pText, pkindname);
			StringCchCat(ptext, txtSize, buffer);

			// 때려야 할 수량과 현재 수량 확보. 
			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMONBATTLE:
		{
			// 처치해야 할 몬스터 확보.
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pkindname = pclClient->pclKindInfoSet->pclKI[kind]->GetName();
			if(pkindname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3025);
			StringCchPrintf(buffer, 1024, pText, pkindname);
			StringCchCat(ptext, txtSize, buffer);

			// 처치해야할 수량과 현재 수량 확보. 
			StringCchPrintf(buffer, 1024,  TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;


	case QUEST_CLEAR_COND_BONUSZERO:
		{
			TCHAR* pText = GetTxtFromMgr(3026);
			StringCchPrintf(buffer, 1024,  pText, pclchar->pclCI->clIP.GetBonus());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMONBONUS:
		{
			TCHAR* pText = GetTxtFromMgr(3027);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_GETQUEST:
		{
			SI32 npckind =  pclquestinfo->clQuestClearConditionInfo.siPara1 ;

			const TCHAR* pnpcname = pclClient->pclKindInfoSet->GetName(npckind);
			if(pnpcname)
			{
				TCHAR* pText = GetTxtFromMgr(3028);
				StringCchPrintf(buffer, 1024,  pText, pnpcname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;

	case QUEST_CLEAR_COND_CLEARQUEST:
		{
			SI32 questunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			cltQuestInfo* pclquestinfo = GetQuestInfo( questunique );
			if(pclquestinfo)
			{
				SI32 npckind = GetNPCKindFromQuestType(pclquestinfo->siType);

				const TCHAR* pnpcname = pclClient->pclKindInfoSet->GetName(npckind);
				if(pnpcname)
				{
					TCHAR* pText = GetTxtFromMgr(3029);
					StringCchPrintf(buffer, 1024,  pText, pnpcname, pclquestinfo->szTitleText);
					StringCchCopy(ptext, txtSize,  buffer);
				}
			}


		}
		break;


	case QUEST_CLEAR_COND_SELLMARKET:
		{
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num		= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);

			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(3030);

				TCHAR item[128]= TEXT(" ") ;
				TCHAR num1[128]= TEXT(" ") ;

				StringCchPrintf(num1, 128, TEXT("%d"),num);
				Nsprintf(buffer,pText,TEXT("item"),  pitemname ,TEXT("num"), num1   ,NULL);
				//sprintf(buffer, pText, pitemname, num);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;

	case QUEST_CLEAR_COND_SCROLL:
		{
			TCHAR* pText = GetTxtFromMgr(3031);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_DAILYQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(3032);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SPECIALQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(3033);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMONEVOLUTION:
		{
			TCHAR* pText = GetTxtFromMgr(3034);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMONSCROLL:
		{
			TCHAR* pText = GetTxtFromMgr(3035);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_ITEMMALL:
		{
			TCHAR* pText = GetTxtFromMgr(3036);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;



	case QUEST_CLEAR_COND_USEWENNYOLDMAN2:
		{
			TCHAR* pText = GetTxtFromMgr(3037);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_FEEDHORSE1:
		{
			TCHAR* pText = GetTxtFromMgr(3038);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_FEEDHORSE2:
		{
			TCHAR* pText = GetTxtFromMgr(3039);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_HORSESPEED:
		{
			TCHAR* pText = GetTxtFromMgr(3040);
			StringCchPrintf(buffer, 1024,  pText, pclquestinfo->clQuestClearConditionInfo.siPara2);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_FISH:
	case QUEST_CLEAR_COND_FISHNOSKILL:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;

			if( itemunique == 0 )
			{
				// 아이템 유니크가 없는경우 "낚시해보기"
				TCHAR* pText = GetTxtFromMgr( 10057 );
				TCHAR num1[128]= TEXT("") ;
				TCHAR count[128]= TEXT("") ;
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
				StringCchPrintf(num1, 128, TEXT("%d"),num);
				StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
				Nsprintf(buffer,pText,TEXT("num"),num1,TEXT("count"),count,NULL);

				StringCchCopy( ptext, txtSize, pText );
			}
			else
			{
				const TCHAR* pname = pclClient->pclItemManager->GetName(itemunique);
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
				if(pname)
				{
					TCHAR* pText = GetTxtFromMgr(3041);
					TCHAR num1[128]= TEXT(" ") ;
					TCHAR count[128]= TEXT(" ") ;

					StringCchPrintf(num1,128, TEXT("%d"),num);
					StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
					Nsprintf(buffer,pText,TEXT("item"),  pname  ,TEXT("num"),   num1 ,TEXT("count"),count    ,NULL);
					//sprintf(buffer, pText, pname, num, pclunit->siQuestPara2);
					StringCchCopy(ptext, txtSize,  buffer);
				}
			}
		}
		break;

	case QUEST_CLEAR_COND_FARM:
	case QUEST_CLEAR_COND_FARMNOSKILL:
		{
            SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;

			if( itemunique == 0 )
			{
				// 아이템 유니크가 없는경우 "수확해보기"
				TCHAR* pText = GetTxtFromMgr( 10056 );
				TCHAR num1[128]= TEXT("") ;
				TCHAR count[128]= TEXT("") ;
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;

				StringCchPrintf(num1, 128, TEXT("%d"),num);
				StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
				Nsprintf(buffer,pText,TEXT("num"),num1,TEXT("count"),count,NULL);

				StringCchCopy( ptext, txtSize, pText );
			}
			else
			{
				const TCHAR* pname = pclClient->pclItemManager->GetName( itemunique );
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
				if(pname)
				{
					TCHAR* pText = GetTxtFromMgr(3042);

					TCHAR num1[128]= TEXT(" ") ;
					TCHAR count[128]= TEXT(" ") ;

					StringCchPrintf(num1, 128, TEXT("%d"),num);
					StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
					Nsprintf(buffer,pText,TEXT("item"), pname   ,TEXT("num"), num1   ,TEXT("count"),  count  ,NULL);

					//sprintf(buffer, pText, pname, num, pclunit->siQuestPara2);
					StringCchCopy(ptext, txtSize,  buffer);
				}
			}
		}
		break;
	case QUEST_CLEAR_COND_MINE:
	case QUEST_CLEAR_COND_MINENOSKILL:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;

			if( itemunique == 0 )
			{
				// 아이템 유니크가 없는경우 "채광해보기"
				TCHAR* pText = GetTxtFromMgr( 10058 );
				TCHAR num1[128]= TEXT("") ;
				TCHAR count[128]= TEXT("") ;
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;

				StringCchPrintf(num1, 128, TEXT("%d"),num);
				StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
				Nsprintf(buffer,pText,TEXT("num"),num1,TEXT("count"),count,NULL);

				StringCchCopy( ptext, txtSize, pText );
			}
			else
			{
				const TCHAR* pname = pclClient->pclItemManager->GetName(itemunique);
				SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
				if(pname)
				{
					TCHAR* pText = GetTxtFromMgr( 10156 );
					TCHAR num1[128]= TEXT(" ") ;
					TCHAR count[128]= TEXT(" ") ;

					StringCchPrintf(num1, 128, TEXT("%d"),num);
					StringCchPrintf(count, 128, TEXT("%d"),pclunit->siQuestPara2);
					Nsprintf(buffer,pText,TEXT("item"), pname   ,TEXT("num"), num1   ,TEXT("count"),  count  ,NULL);
					StringCchCopy(ptext, txtSize,  buffer);
				}
			}
		}
		break;
	case QUEST_CLEAR_COND_SKILLBONUSZERO:
		{
			TCHAR* pText = GetTxtFromMgr(3043);
			StringCchPrintf(buffer, 1024,  pText, pclchar->pclCI->clIP.GetSkillBonus());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_MONEY:
		{
			TCHAR* pText = GetTxtFromMgr(3044);
			StringCchPrintf(buffer, 1024,  pText, pclquestinfo->clQuestClearConditionInfo.siPara2, pclchar->pclCI->clIP.GetMoney());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_OPENHEADCHAT:
		{
			TCHAR* pText = GetTxtFromMgr(3045);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_BANK:
		{
			TCHAR* pText = GetTxtFromMgr(3046);
			StringCchPrintf(buffer, 1024,  pText, pclquestinfo->clQuestClearConditionInfo.siPara2, pclchar->pclCI->clBank.GetMoney());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_HUNGRY:
		{
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;			
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3047);
			StringCchPrintf(buffer, 1024,  pText,pitemname);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_REPAIRWEAPON:
		if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].siUnique || pclchar->pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].siUnique)
		{
			TCHAR* pText = GetTxtFromMgr(3048);
			StringCchPrintf(buffer, 1024,  pText);
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(3049);
			StringCchPrintf(buffer, 1024,  pText);
		}

		StringCchCopy(ptext, txtSize,  buffer);
		break;




	case QUEST_CLEAR_COND_SELLNPC:
		{
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			TCHAR* pText = GetTxtFromMgr(3050);
			StringCchPrintf(buffer, 1024,  pText, pclClient->pclKindInfoSet->GetName(kind));
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_SELLSWORDNPC:
		{
			// 확보해야할 아이템
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3051);
			StringCchPrintf(buffer, 1024,  pText, pitemname);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_SKILL:
		{
			TCHAR skillname[128];

			if(pclClient->pclSkillManager->GetSkillName(pclquestinfo->clQuestClearConditionInfo.siPara1, skillname) == true)
			{
				TCHAR* pText = GetTxtFromMgr(3052);

				TCHAR level[128]= TEXT(" ") ;
				TCHAR curlevel[128]= TEXT(" ") ;

				StringCchPrintf(level, 128, TEXT("%d"),pclquestinfo->clQuestClearConditionInfo.siPara2);
				StringCchPrintf(curlevel, 128, TEXT("%d"),pclchar->pclCI->clCharSkill.GetTotalSkillLevel(pclquestinfo->clQuestClearConditionInfo.siPara1, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]) );
				Nsprintf(buffer,pText,TEXT("what"), skillname   ,TEXT("level"), level   ,TEXT("curlevel"), curlevel   ,NULL);

				//sprintf(buffer, pText, 
				//	skillname, 
				//	pclquestinfo->clQuestClearConditionInfo.siPara2,
				//	pclchar->pclCI->clCharSkill.GetTotalSkillLevel(pclquestinfo->clQuestClearConditF3089ionInfo.siPara1, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]) );
				StringCchCopy(ptext, txtSize,  buffer);
			}

		}
		break;

	case QUEST_CLEAR_COND_BASICWEAPONSKILL:
		{
			TCHAR* pText = GetTxtFromMgr(3053);
			StringCchPrintf(buffer, 1024,  pText, 
				pclquestinfo->clQuestClearConditionInfo.siPara2,
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESWORD1, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]),
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEBOW1,	0, 0, &pclchar->pclCI->clCharItem.clItem[0]),
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESTAFF1, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]),
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESPEAR1,	0, 0, &pclchar->pclCI->clCharItem.clItem[0]),
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEAXE1,	0, 0, &pclchar->pclCI->clCharItem.clItem[0]),
				pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEGUN1,	0, 0, &pclchar->pclCI->clCharItem.clItem[0]));

			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_EQUIP:
		{
			// 확보해야할 아이템
			SI32 itemunique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(3054);
			StringCchPrintf(buffer, 1024,  pText, pitemname);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;

	case QUEST_CLEAR_COND_EQUIPPOS:
	case QUEST_CLEAR_COND_ENCHANTEQUIPPOS:
		{
			TCHAR szpos[64];

			switch(pclquestinfo->clQuestClearConditionInfo.siPara1)
			{
			case PERSONITEM_HELMET:
				{
					TCHAR* pText = GetTxtFromMgr(1024);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_ARMOUR:
				{
					TCHAR* pText = GetTxtFromMgr(1026);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_WEAPON1:
				{
					TCHAR* pText = GetTxtFromMgr(3055);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_NECK:
				{
					TCHAR* pText = GetTxtFromMgr(1031);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_RING1:
				{
					TCHAR* pText = GetTxtFromMgr(3056);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_RING2:
				{
					TCHAR* pText = GetTxtFromMgr(3057);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_BELT:
				{
					TCHAR* pText = GetTxtFromMgr(1029);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_SHOES:
				{
					TCHAR* pText = GetTxtFromMgr(1030);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_WEAPON2:
				{
					TCHAR* pText = GetTxtFromMgr(3058);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_MANTLE:
				{
					TCHAR* pText = GetTxtFromMgr(1032);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_BAG:
				{
					TCHAR* pText = GetTxtFromMgr(1033);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_BAG_SUB:
				{
					TCHAR* pText = GetTxtFromMgr(1033);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_HAT:
				{
					TCHAR* pText = GetTxtFromMgr(1025);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			case PERSONITEM_DRESS:
				{
					TCHAR* pText = GetTxtFromMgr(1027);
					StringCchCopy(szpos, 64, pText);
				}
				break;

			default:
				StringCchCopy(szpos, 64, TEXT(""));
				break;

			}

			TCHAR* pText = GetTxtFromMgr(3059);
			StringCchPrintf(buffer, 1024,  pText, szpos);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;


	case QUEST_CLEAR_COND_BAG:
		{
			TCHAR* pText = GetTxtFromMgr(3060);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;


	case QUEST_CLEAR_COND_LEVEL:
		{
			TCHAR* pText = GetTxtFromMgr(3061);
			StringCchPrintf(buffer, 1024,  pText, pclquestinfo->clQuestClearConditionInfo.siPara2, pclchar->pclCI->clIP.GetLevel());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_FAMELEVEL:
		{
			TCHAR* pText = GetTxtFromMgr(3062);
			StringCchPrintf(buffer, 1024,  pText, 
				pclquestinfo->clQuestClearConditionInfo.siPara2, pclchar->pclCI->clIP.siFameLevel);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_FULLLIFE:
		{
			TCHAR* pText = GetTxtFromMgr(3063);
			StringCchPrintf(buffer, 1024,  pText, pclchar->pclCI->clIP.GetLife(), pclchar->clPB.GetMaxLife());
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_STOCK:
		{
			TCHAR* pText = GetTxtFromMgr(3064);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_MAKEFRIEND:
		{
			TCHAR* pText = GetTxtFromMgr(3065);
			StringCchPrintf(buffer, 1024,  pText, pclquestinfo->clQuestClearConditionInfo.siPara2);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_HORSE:
		{
			TCHAR* pText = GetTxtFromMgr(3066);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMON:
		{
			TCHAR* pText = GetTxtFromMgr(3067);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SUMMON2:
		{
			TCHAR* pText = GetTxtFromMgr(3068);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_OPENCHARSTATUS:
		{
			TCHAR* pText = GetTxtFromMgr(3069);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_OPENSTOCKMARKET:
		{
			TCHAR* pText = GetTxtFromMgr(3071);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_LOOKPANSEO:
		{
			TCHAR* pText = GetTxtFromMgr(3072);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_OPENVILLAGEBOARD:
		{
			TCHAR* pText = GetTxtFromMgr(3073);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_SETFATHER:
		{
			TCHAR* pText = GetTxtFromMgr(5604);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_OPENCITYHALL:
		{
			TCHAR* pText = GetTxtFromMgr(3074);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_OPENHUNT:
		{
			TCHAR* pText = GetTxtFromMgr(3075);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_SKILLMASTER:
		{
			TCHAR* pText = GetTxtFromMgr(3076);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;


	case QUEST_CLEAR_COND_BROADCAST:
		{
			TCHAR* pText = GetTxtFromMgr(3077);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_NEWS:
		{
			TCHAR* pText = GetTxtFromMgr(3078);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_OTHERMINIHOME:
		{
			TCHAR* pText = GetTxtFromMgr(3079);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;



	case QUEST_CLEAR_COND_ITEMINFO:
		{

			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(3080);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;

	case QUEST_CLEAR_COND_ITEMINFODLG:
		{
			TCHAR* pText = GetTxtFromMgr(3081);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_STRUCTUREOPEN:
		{
			TCHAR* pText = GetTxtFromMgr(3082);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_OPENSKILL:
		{
			TCHAR* pText = GetTxtFromMgr(3083);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;

	case QUEST_CLEAR_COND_GOMAP:			// 요술램프 마을 순회
		{
			SI32 mapindex	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			
			TCHAR* pmapname = pclClient->pclMapManager->GetMapName(mapindex);
			if(pmapname)
			{
				TCHAR* pText = NULL;
				pText = GetTxtFromMgr(3084);
				StringCchPrintf(buffer, 1024,  pText, pmapname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;
	case QUEST_CLEAR_COND_LAMPGOMAP:		// [지연] 요술램프의 마을 순회 퀘스트를 위한 종료 조건 분리.
		{
			SI32 mapindex	= pclquestinfo->clQuestClearConditionInfo.siPara1;

			TCHAR* pmapname = pclClient->pclMapManager->GetMapName(mapindex);

			if(pmapname)	
			{
				TCHAR* pText = NULL;
				pText = GetTxtFromMgr(30214); // 마을 방문:[%d.%s]
				StringCchPrintf(buffer, 1024, pText, mapindex, pmapname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;
	case QUEST_CLEAR_COND_GONPC://와 대화한다.
	case QUEST_CLEAR_COND_GONPC_INFORMATION:
		{
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			TCHAR* pText = GetTxtFromMgr(3227);
			StringCchPrintf(buffer, 1024,  pText, pclClient->pclKindInfoSet->GetName(kind));
			StringCchCopy(ptext, txtSize,  buffer);

		}
		break;

	case QUEST_CLEAR_COND_CLICKITEMINFO:
		{			
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(6257);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);

			}				
		}
		break;
	case QUEST_CLEAR_COND_OPENMAKEITEM:
		{
			TCHAR* pText = GetTxtFromMgr(6267);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_CLICKMAKEBUTTON:
		{
			TCHAR* pText = GetTxtFromMgr(6268);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);
		}
		break;
	case QUEST_CLEAR_COND_UPITEMTOMARKET:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(6269);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;
	case QUEST_CLEAR_COND_SELLTRADER:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(6270);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;
	case QUEST_CLEAR_COND_OPENENCHANTITEM:
		{		
		
			TCHAR* pText = GetTxtFromMgr(6271);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);			
		}
		break;
	case QUEST_CLEAR_COND_SETENCHANTITEM:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(6272);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;
	case QUEST_CLEAR_COND_SETENCHANTELEMENT:
		{
			TCHAR* pText = GetTxtFromMgr(6273);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_ENCHANTITEM:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(6274);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}
			
		}
		break;
	case QUEST_CLEAR_COND_WARPTOPALACE:
		{
			TCHAR* pText = GetTxtFromMgr(6410);
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	
	case QUEST_CLEAR_COND_USEMAGIC:
		{
			TCHAR* pText = GetTxtFromMgr(6934);	// 아무마법이나써볼것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_PRIVATETRADE:
		{
			TCHAR* pText = GetTxtFromMgr(6935);	// 개인거래 해볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_SELLFROMPBSTORE:
		{
			TCHAR* pText = GetTxtFromMgr(6936);	// 개인상점을 이용해 아이템을 팔아 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_USEWARPTICKET3:
		{
			TCHAR* pText = GetTxtFromMgr(6937);	// 워프티켓을 써서 친구에게 날아가 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_BUYTOPBSTORE:
		{
			TCHAR* pText = GetTxtFromMgr(6938);	// 개인상점을 이용해 아이템을 사 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_MAKEBOOK:
		{
			TCHAR* pText = GetTxtFromMgr(6939);	// 책을 만들어 볼 것 
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_HUNTMAPINFODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6940);	// 사냥터 정보창을 열어 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_SEARCHMONSTERINFO:
		{
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;			
			const TCHAR* pkindname = pclClient->pclKindInfoSet->pclKI[kind]->GetName();
			
			if( pkindname != NULL )
			{
				TCHAR* pText = GetTxtFromMgr(6941);	// 몬스터의 정보를 검색해 볼 것%s
				StringCchPrintf(buffer, 1024,  pText , pkindname );
				StringCchCopy(ptext, txtSize,  buffer);		
			}
		}
		break;
	case QUEST_CLEAR_COND_PARTYHUNTING:
		{
			TCHAR* pText = GetTxtFromMgr(6942);	// 파티를 맺고 사냥을 해 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_MAGICINFODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6943);	// 마법 정보창을 열어 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_OPENHOUSE:
		{
			TCHAR* pText = GetTxtFromMgr(6947);	// 임대창고 건물을 열어 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}
		break;
	case QUEST_CLEAR_COND_OPENREALESTATEMARKET:
		{
			TCHAR* pText = GetTxtFromMgr(6948);	// 부동산거래소 건물을 열어 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_BLACKARMY:
		{
			TCHAR* pText = GetTxtFromMgr(6949);	// 흑의 군단 참여 해 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENHORSEMARKET:
		{
			TCHAR* pText = GetTxtFromMgr(6950);	// 사복시(말거래소)를 열어 볼 것
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK:
		{
			TCHAR* pText = GetTxtFromMgr(6951);	// 특정 레벨까지 몬스터를 진화 시켜 볼 것%d
			StringCchPrintf(buffer, 1024,  pText,pclquestinfo->clQuestClearConditionInfo.siPara1);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_REVIVESUMMONBYDOLL:
		{
			TCHAR* pText = GetTxtFromMgr(6952);	// 소환수를 인형을 사용하여 부활시킨다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENHOZODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6953);	// 호조판서(백호관)의 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENBYUNGZODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6954);	// 병조판서(청룡관)의 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENGONGZODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6955);	// 공조판서(현무관)의 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENIZODLG:
		{
			TCHAR* pText = GetTxtFromMgr(6956);	// 이조판서(주작관)의 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_OPENGOONZUDLG:
		{
			TCHAR* pText = GetTxtFromMgr(6957);	// 군주의 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_VOTEKING:
		{
			TCHAR* pText = GetTxtFromMgr(6958);	// 군주선거에 참여해 본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		
		}		
		break;
	case QUEST_CLEAR_COND_VILLAGEDETAILINFO:
		{
			TCHAR* pText = GetTxtFromMgr(6962);	// 마을 정보창을 열어본다
			StringCchPrintf(buffer, 1024,  pText);
			StringCchCopy(ptext, txtSize,  buffer);		

		}
		break;
	case QUEST_CLEAR_COND_OPENFRIEND:
		{
			TCHAR* pText = GetTxtFromMgr(10260);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENMASTER:
		{
			TCHAR* pText = GetTxtFromMgr(10261);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENMAKEREQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(10262);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_MAKEREQUESTSTART:
		{
			TCHAR* pText = GetTxtFromMgr(10263);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENNAMINGQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(10267);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_DISASSEMBLEITEM:
		{
			TCHAR* pText = GetTxtFromMgr(10264);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SUMMONACTIVE:
		{
			TCHAR* pText = GetTxtFromMgr(10265);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SUMMONDIE:
		{
			TCHAR* pText = GetTxtFromMgr(10266);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SUMMONCHANGESTATUS:
		{
			TCHAR* pText = GetTxtFromMgr(10268);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SUMMONATTACK:
		{
			TCHAR* pText = GetTxtFromMgr(10269);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SUMMONLEVELUP:
		{
			TCHAR* pText = GetTxtFromMgr(10270);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_MAKEINTBOOK:
		{
			TCHAR* pText = GetTxtFromMgr(10271);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_PARTYKILLMON:
		{
			// 처치해야 할 몬스터 확보.
			SI32 kind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32 num	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			const TCHAR* pkindname = pclClient->pclKindInfoSet->pclKI[kind]->GetName();
			if(pkindname == NULL)return false;

			TCHAR* pText = GetTxtFromMgr(10272);//잡아야할 몬스터
			StringCchPrintf(buffer, 1024, pText, pkindname);
			StringCchCat(ptext, txtSize, buffer);

			// 처치해야할 수량과 현재 수량 확보. 
			StringCchPrintf(buffer, 1024, TEXT("(%d/%d)"), pclunit->siQuestPara2, num);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;
	case QUEST_CLEAR_COND_SETGREETING:
		{
			TCHAR* pText = GetTxtFromMgr(10291);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_MAGICACTIVE:
		{
			TCHAR* pText = GetTxtFromMgr(10292);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_GOHUNTMAP:
		{
			TCHAR* pText = GetTxtFromMgr(10293);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENGUILD:
		{
			TCHAR* pText = GetTxtFromMgr(10294);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_APPLYGUILD:
		{
			TCHAR* pText = GetTxtFromMgr(10295);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_LOOKPRISON:
		{
			TCHAR* pText = GetTxtFromMgr(10296);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE:
		{
			TCHAR* pText = GetTxtFromMgr(10297);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_MAKEPARTY:
		{
			TCHAR* pText = GetTxtFromMgr(10298);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SORTRESIDENT:
		{
			TCHAR* pText = GetTxtFromMgr(10299);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENSKILLETC:
		{
			TCHAR* pText = GetTxtFromMgr(10301);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SITDOWN:
		{
			TCHAR* pText = GetTxtFromMgr(10302);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_NPCINFO:
		{
			TCHAR* pText = GetTxtFromMgr(7737);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;		
	case QUEST_CLEAR_COND_CLICKITEMPROPERTY:
		{
			SI32 itemunique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			const TCHAR* pitemname = pclClient->pclItemManager->GetName(itemunique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(10300);
				StringCchPrintf(buffer, 1024,  pText, pitemname);
				StringCchCopy(ptext, txtSize,  buffer);
			}				
		}
		break;
	case QUEST_CLEAR_COND_OPENGUILDUSER:
		{
			TCHAR* pText = GetTxtFromMgr(7681);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SEARCHSTOCKINFO:
		{
			TCHAR* pText = GetTxtFromMgr(7754);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CLICKSTOCKOFFER:
		{
			TCHAR* pText = GetTxtFromMgr(7755);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CLICKCITYHALLFUND:
		{
			TCHAR* pText = GetTxtFromMgr(7756);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CLICKGENERALMETTING:
		{
			TCHAR* pText = GetTxtFromMgr(7757);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CLICKPERSONALSHOP:
		{
			TCHAR* pText = GetTxtFromMgr(7758);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENITEMMALL:
		{
			TCHAR* pText = GetTxtFromMgr(7752);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_ANGELLISTCLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7810);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_POLICESETCLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7815);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_SEARCHFARM:
		{
			TCHAR* pText = GetTxtFromMgr(7811);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_LEASECLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7812);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_GETMAGICLAMPQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(7813);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_OPENQUESTDIALOG:
		{
			TCHAR* pText = GetTxtFromMgr(7814);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_ISLEARNED_MANUFACTURE:
		{
			NTCHARString256	kText( GetTxtFromMgr(7846) );
			SI32	siSkillLevel = pclquestinfo->clQuestClearConditionInfo.siPara2;


			kText.Replace( TEXT("#skilllevel#"), SI32ToString( siSkillLevel ) );
			StringCchCopy( ptext, txtSize, kText );
		}
		break;
	case QUEST_CLEAR_COND_ISGET_MANUFACTUREQUEST:
		{
			TCHAR* pText = GetTxtFromMgr(7847);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_REQUESTMENTORSUCCESS:
		{
			TCHAR* pText = GetTxtFromMgr(7848);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_TAKEPUPILS:
		{
			TCHAR* pText = GetTxtFromMgr(7850);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CHATTOBEGGINER:
		{
			TCHAR* pText = GetTxtFromMgr(7849);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_CHECKSTATUS:
		{
			NTCHARString256	kText( GetTxtFromMgr(7855) );
			NTCHARString256 kStatus( TEXT("") );

			SI32	siTxtMgrNum		= pclquestinfo->clQuestClearConditionInfo.siPara1 - 1; //1번부터 시작이기 때문에 인덱스를 참조하기위해 
			SI32	siNeedLevel		= pclquestinfo->clQuestClearConditionInfo.siPara2;

			SI32	siTextNumber[9]	= {4369, 4370, 4371, 4368, 4373, 5773, 4374, 4375, 4376};


			if ( (siTxtMgrNum < 0) || (siTxtMgrNum >= 9) )
			{
				#ifdef _DEBUG
					MsgBox(TEXT("QUEST_CLEAR_COND_CHECKSTATUS 설정오류"), TEXT("para1의 값이 잘못되었습니다"));
				#endif

				return false;
			}

			kStatus = GetTxtFromMgr( siTextNumber[ siTxtMgrNum ] );
			kStatus.Replace( TEXT(":"), TEXT("") );

			kText.Replace( TEXT("#status#"), kStatus );
			kText.Replace( TEXT("#statuslevel#"), SI32ToString( siNeedLevel ) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_ALLWEAPONSKILL:
		{
			TCHAR*	pText			= GetTxtFromMgr(7856);
			TCHAR	tszLevel[20]	= {'\0', };
			

			StringCchPrintf( tszLevel, 20, TEXT("%d"), pclquestinfo->clQuestClearConditionInfo.siPara1 );
			Nsprintf( buffer, pText, TEXT("skilllevel"), tszLevel, NULL );

			StringCchCopy(ptext, txtSize, buffer);
		}
		break;
	/*case QUEST_CLEAR_COND_UPSKILL:
		{
			TCHAR*	pText			= GetTxtFromMgr(7856);
			TCHAR	tszLevel[20]	= {'\0', };


			StringCchPrintf( tszLevel, 20, TEXT("%d"), pclquestinfo->clQuestClearConditionInfo.siPara1 );
			Nsprintf( buffer, pText, TEXT("skilllevel"), tszLevel, NULL );

			StringCchCopy(ptext, txtSize, buffer);
		}
		break;*/
	case QUEST_CLEAR_COND_MEMOCLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7857);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_MOVETOGROUPCLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7858);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;
	case QUEST_CLEAR_COND_EQUIPBAG:
		{
			TCHAR*	pText		= GetTxtFromMgr(1033);
			TCHAR	szPos[64]	= {'\0', };
			
			StringCchCopy(szPos, 64, pText);
			
			pText = GetTxtFromMgr(3059);
			StringCchPrintf(buffer, 1024,  pText, szPos);
			StringCchCat(ptext, txtSize, buffer);
		}
		break;
	case QUEST_CLEAR_COND_KILLDARKSOLDIER:
		{
			NTCHARString256		kText(GetTxtFromMgr(7884));
			kText.Replace(TEXT("#killcount#"), SI32ToString(pclquestinfo->clQuestClearConditionInfo.siPara2));
			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_MAKE_RAREITEM:
		{
			NTCHARString256	kText( GetTxtFromMgr(7934) );
			NTCHARString64	kItemName( TEXT("") );
			SI32			siItemUnique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siMaxItemCount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowItemCount	= pclunit->siQuestPara2;
			

			if ( siItemUnique > 0 )
			{
				kItemName = pclClient->pclItemManager->GetName( siItemUnique );
			}
			
			kText.Replace( TEXT("#itemname#"), kItemName );
			kText.Replace( TEXT("#nowcount#"), SI32ToString(siNowItemCount) );
			kText.Replace( TEXT("#maxcount#"), SI32ToString(siMaxItemCount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_FULL_ORGAN:
		{
			NTCHARString256		kText( GetTxtFromMgr(7935) );
			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_TWOWAYKILLMON:
	case QUEST_CLEAR_COND_TWOWAYKILLMONBONUS:
		{
			NTCHARString256 kText( GetTxtFromMgr(7961) );
			NTCHARString64	kMonsterName	= TEXT("");
			SI32			siMonsterKind	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siMax			= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siCount			= pclunit->siQuestPara2;

			
			if ( siMonsterKind <= 0 )						return false;
			
			kMonsterName = pclClient->pclKindInfoSet->pclKI[siMonsterKind]->GetName();
			if ( kMonsterName.Compare( TEXT("") ) == 0)		return false;

			kText.Replace( TEXT("#name#"), kMonsterName );
			kText.Replace( TEXT("#count#"), SI32ToString( siCount ) );
			kText.Replace( TEXT("#max#"), SI32ToString( siMax ) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_BUYMARKET:
		{
			NTCHARString128	kText( GetTxtFromMgr(8099) );
			NTCHARString64	kItemName		= TEXT("");

			SI32			siItemUnique	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siNowNum		= pclunit->siQuestPara2;
			SI32			siTotalNum		= pclquestinfo->clQuestClearConditionInfo.siPara2;

			// 아무아이템(아이템 Unique가 0) 구입해볼때
			if ( siItemUnique == 0 )
			{
				kText.Replace( TEXT("[#itemname#] "), TEXT("") );
			}
			else
			{
				kItemName = pclClient->pclItemManager->GetName( siItemUnique );
				if ( kItemName.Compare(TEXT("")) == 0 )
				{
					return false;
				}

				kText.Replace( TEXT("#itemname#"), kItemName );
			}

			kText.Replace( TEXT("#nownum#"), SI32ToString( siNowNum ) );
			kText.Replace( TEXT("#totalnum#"), SI32ToString( siTotalNum ) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_OPENFATHERDIALOG:
		{
			NTCHARString128	kText( GetTxtFromMgr(8257) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_TEACHERSET:
		{
			NTCHARString128	kText( GetTxtFromMgr(9567) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_ORDERTOMARKET:
		{
			NTCHARString128	kText( GetTxtFromMgr(8255) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG:
		{
			NTCHARString128	kText( GetTxtFromMgr(8256) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
		
	case QUEST_CLEAR_COND_MAKELEVELITEM:
	case QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM:
		{
			NTCHARString128	kText( GetTxtFromMgr(8266) );
			SI32			siLevel		= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siAmount	= pclquestinfo->clQuestClearConditionInfo.siPara2;


			kText.Replace( TEXT("#level#"), SI32ToString(siLevel) );
			kText.Replace( TEXT("#amount#"), SI32ToString(siAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;
	case QUEST_CLEAR_COND_TAKEEQUIPLEVELWEAPON:
		{
			NTCHARString128 kText( GetTxtFromMgr(8667) );
			SI32			siLevel	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siCount = pclquestinfo->clQuestClearConditionInfo.siPara2;


			kText.Replace( TEXT("#level#"), SI32ToString(siLevel) );
			kText.Replace( TEXT("#count#"), SI32ToString(siCount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_GETELEMENT_RESTORE:
		{
			NTCHARString128 kText( GetTxtFromMgr( 8668 ) );
			SI32			siNeedAmount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowAmount		= pclunit->siQuestPara2;
			

			kText.Replace( TEXT("#needamount#"),	SI32ToString(siNeedAmount) );
			kText.Replace( TEXT("#nowamount#"),		SI32ToString(siNowAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_GETPOWDER_RESTORE:
		{
			NTCHARString128 kText( GetTxtFromMgr( 8669 ) );
			SI32			siNeedAmount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowAmount		= pclunit->siQuestPara2;


			kText.Replace( TEXT("#needamount#"),	SI32ToString(siNeedAmount) );
			kText.Replace( TEXT("#nowamount#"),		SI32ToString(siNowAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON:
		{
			NTCHARString128 kText( GetTxtFromMgr(8671) );
			SI32			siLevel	= pclquestinfo->clQuestClearConditionInfo.siPara1;

			kText.Replace( TEXT("#level#"), SI32ToString(siLevel) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_TAKEITEM_MATERIAL:
		{
			NTCHARString128 kText( GetTxtFromMgr(8672) );
			SI32			siUnique = pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siAmount = pclquestinfo->clQuestClearConditionInfo.siPara2;

			kText.Replace( TEXT("#itemname#"),	pclClient->pclItemManager->GetName(siUnique) );
			kText.Replace( TEXT("#amount#"),	SI32ToString(siAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_TAKEWEAPONHAVESLOT:
		{
			NTCHARString128 kText( GetTxtFromMgr(8673) );
			SI32			siAmount = pclquestinfo->clQuestClearConditionInfo.siPara2;

			kText.Replace( TEXT("#amount#"), SI32ToString(siAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_ARTIFACTMIXWEAPON:
		{
			NTCHARString128 kText( GetTxtFromMgr(8674) );
			SI32			siAmount = pclquestinfo->clQuestClearConditionInfo.siPara2;

			kText.Replace( TEXT("#amount#"), SI32ToString(siAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_ARTIFACTMIX:
		{
			NTCHARString128 kText( GetTxtFromMgr(9922) );
			SI32			siNeed	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNow	= pclunit->siQuestPara2;

			kText.Replace( TEXT("#need#"),	SI32ToString(siNeed) );
			kText.Replace( TEXT("#now#"),	SI32ToString(siNow) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_MAKECRYSTAL:
		{
			NTCHARString128 kText( GetTxtFromMgr(8675) );
			TCHAR			strGrade[][10]	= { TEXT("F"), TEXT("E"), TEXT("D"), TEXT("C"), TEXT("B"), TEXT("A"), TEXT("S1"), TEXT("S2") };
			SI32			siGrade			= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siNeedAmount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowAmount		= pclunit->siQuestPara2;

			if ( (CRYSTAL_GRADE_NONE >= siGrade) || (CRYSTAL_GRADE_MAX <= siGrade) )
			{
				return false;
			}

			kText.Replace( TEXT("#grade#"),			strGrade[siGrade-1] );
			kText.Replace( TEXT("#needamount#"),	SI32ToString(siNeedAmount) );
			kText.Replace( TEXT("#nowamount#"),		SI32ToString(siNowAmount) );

			StringCchCopy(ptext, txtSize, kText);
		}
		break;

	case QUEST_CLEAR_COND_SUMMONBONUSZERO:
		{
			NTCHARString128 kText( GetTxtFromMgr(9726) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_SWORDSKILLCLICK:
		{
			NTCHARString128 kText( GetTxtFromMgr(9867) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_LEARNWEAPONSKILL:
		{
			NTCHARString128 kText( GetTxtFromMgr(9868) );
			SI32			siGrade	= pclquestinfo->clQuestClearConditionInfo.siPara1;
			SI32			siLevel	= pclquestinfo->clQuestClearConditionInfo.siPara2;

			kText.Replace( TEXT("#grade#"), SI32ToString(siGrade) );
			kText.Replace( TEXT("#level#"), SI32ToString(siLevel) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_PVPWIN:
		{
			NTCHARString128 kText( GetTxtFromMgr(9869) );
			SI32			siMaxCount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowCount	= pclunit->siQuestPara2;

			kText.ReplaceAll( TEXT("#max#"), SI32ToString(siMaxCount) );
			kText.Replace( TEXT("#now#"), SI32ToString(siNowCount) );
			
			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_PVPLEAGUERESERVE:
		{
			NTCHARString128 kText( GetTxtFromMgr(9871) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_PVPLEAGUEWIN:
		{
			NTCHARString128 kText( GetTxtFromMgr(9872) );
			SI32			siMaxCount	= pclquestinfo->clQuestClearConditionInfo.siPara2;
			SI32			siNowCount	= pclunit->siQuestPara2;

			kText.ReplaceAll( TEXT("#max#"), SI32ToString(siMaxCount) );
			kText.Replace( TEXT("#now#"), SI32ToString(siNowCount) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_SETMYKEY:
		{
			NTCHARString128 kText( GetTxtFromMgr(9923) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_SUMMONHOUSE_INPUT:
		{
			NTCHARString128 kText( GetTxtFromMgr(9924) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT:
		{
			NTCHARString128 kText( GetTxtFromMgr(9925) );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_MARKETCONDITION:
		{
			NTCHARString128 kText( GetTxtFromMgr(9926) );

			SI32			siItemType = pclquestinfo->clQuestClearConditionInfo.siPara1;
			if ( (0 >= siItemType) || (MAX_ITEMTYPE_NUMBER <= siItemType) )
			{
				break;
			}

			NTCHARString128	kTypeName = pclClient->pclItemManager->pclItemTypeInfo[siItemType]->GetName();
			kText.Replace( TEXT("#typename#"), kTypeName );

			StringCchCopy( ptext, txtSize, kText );
		}
		break;

	case QUEST_CLEAR_COND_BUFFCLICK:
		{
			TCHAR* pText = GetTxtFromMgr(7815);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;

	case QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS:
		{
			TCHAR* pText = GetTxtFromMgr(30263);
			StringCchCopy(ptext, txtSize, pText);
		}
		break;

	case QUEST_CLEAR_COND_MAPKILLMON:
		{
			SI32	siMapIndex	= pclquestinfo->clQuestClearConditionInfo.siPara1;	// 맵인덱스
			SI32	siNowNum	= pclunit->siQuestPara2;							// 현재 잡은 마릿수
			SI32	siMaxNum	= pclquestinfo->clQuestClearConditionInfo.siPara2;	// 여기까지 잡아라

			TCHAR*	pszMapName	= pclClient->pclMapManager->GetMapName( siMapIndex );
			if ( NULL == pszMapName )
			{
				break;
			}

			NTCHARString256 strText = GetTxtFromMgr(30041);
			
			strText.Replace( TEXT("#map#"),		pszMapName );
			strText.Replace( TEXT("#now#"),		SI32ToString(siNowNum) );
			strText.Replace( TEXT("#max#"),		SI32ToString(siMaxNum) );

			StringCchCat(ptext, txtSize, strText);
		}
		break;

	case QUEST_CLEAR_COND_MAKESKILLEXPUP:
		{
			TCHAR skillname[128];

			if(pclClient->pclSkillManager->GetSkillName(pclquestinfo->clQuestClearConditionInfo.siPara1, skillname) == true)
			{
				TCHAR* pText = GetTxtFromMgr(30218); // [#what#] 경험치 포인트 [#exp#]이상 필요 (현재 기술 경험치 포인트 :#curexp#])

				TCHAR exp[128]= TEXT(" ");
				TCHAR curexp[128] = TEXT(" ");

				StringCchPrintf(exp, 128, TEXT("%d"),pclquestinfo->clQuestClearConditionInfo.siPara2);
				StringCchPrintf(curexp, 128, TEXT("%d"),pclchar->pclCI->clCharSkill.GetTotalSkillExp(pclquestinfo->clQuestClearConditionInfo.siPara1));
				Nsprintf(buffer,pText,TEXT("what"), skillname   ,TEXT("exp"), exp   ,TEXT("curexp"), curexp   ,NULL);

				StringCchCopy(ptext, txtSize,  buffer);
			}
		}
		break;

	case QUEST_CLEAR_COND_SEARCH_TOTALMAP:
		{
			NTCHARString256 strText = GetTxtFromMgr(40843);
			StringCchCopy(ptext, txtSize,  strText);
		}
		break;

	default:
		return false;
	}
	
	// [지연] 군주데이미션 : 퀘스트 정보 대화상자에 남은 시간 표시
	if ( NULL != pclquestinfo && MISSIONQUEST_UNIQUE_GOONZUDAY == pclquestinfo->siUnique )
	{
		cltMissionQuestInfo* pMission = (cltMissionQuestInfo*)pclquestinfo;
		SI32 siLeftTime = 0;
		siLeftTime = pMission->GetLeftTime();
		siLeftTime = siLeftTime;	// siLeftTime / 60;

		NTCHARString64 strText( GetTxtFromMgr(40883) );		// "#second# 초"
		strText.Replace( TEXT("#second#"), SI32ToString(siLeftTime) );

		StringCchPrintf( buffer, txtSize, TEXT(" %s \r %s"), ptext, (TCHAR*)strText );
		StringCchCopy( ptext, txtSize, buffer );
	}

	return true;

}

// 현재 상황에 대한 정보 텍스트를 얻어온다.
bool cltQuestManager::GetCurStatusText(SI32 id, SI32 questtype, SI32 questunique, TCHAR* ptext, SI16 txtSize)
{
	if(ptext == NULL)			return false;
	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[id];
	if(pclchar == NULL)			return false;

	StringCchCopy(ptext, txtSize,  TEXT(""));

	cltQuestInfoCommon* pclquestinfo = NULL;

	switch(questtype)
	{
	case QUEST_TYPE_SPECIAL:			pclquestinfo = &pclchar->pclCI->clQuestInfo.clSpecialQuestInfo;			break;
	case QUEST_TYPE_NAMING:				pclquestinfo = &pclchar->pclCI->clQuestInfo.clNamingQuestInfo;			break;
	case QUEST_TYPE_HAPPYBEAN:			pclquestinfo = &pclchar->pclCI->clQuestInfo.clHappyBeanQuestInfo;		break;
	case QUEST_TYPE_MISSION1:			pclquestinfo = &pclchar->pclCI->clQuestInfo.clMissionQuestInfo;			break;
	default:							pclquestinfo = pclClient->pclQuestManager->GetQuestInfo(questunique);	break;
	}

	if(pclquestinfo == NULL)	return false;

	return GetCurStatusText(pclchar, questtype, pclquestinfo, ptext, txtSize);
}

// 특정 타입 퀘스트의 개수를 구한다. 
SI32 cltQuestManager::GetQuestNum(SI32 questtype)
{
	SI32 i;
	SI32 count = 0;

	for(i = 0; i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		if(siQuestBitInfo[questtype][i])
		{
			count++;
		}
	}

	return count;

}
/*
// 특정 타입 퀘스트의 완료된 개수를 구한다. 
SI32 cltQuestManager::GetClearQuestNum(SI32 type)
{
}
*/

// 퀘스트 타입 이름을 구한다. 
TCHAR* cltQuestManager::GetQuestTypeName(SI32 questtype)
{
	if(questtype < 0 || questtype >= MAX_QUEST_TYPE)return NULL;

	return szQuestTypeName[questtype];

}


// 퀘스트의 진행 상황을 텍스트로 얻어온다. 
bool cltQuestManager::GetQuestProgressText(SI16 questtype, cltPQuestInfo* pclpersonquestinfo, TCHAR* ptext, SI16 txtSize)
{
	SI16 curquestunique = pclpersonquestinfo->clUnit[questtype].siCurQuestUnique;

	SI32 i;
	TCHAR temp[256];

	for(i = 0;i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		SI32 questunique = GetUniqueFromBit(questtype, i);
		if(questunique)
		{
			cltQuestInfo* pclquestinfo = GetQuestInfo(questunique);
			if(pclquestinfo)
			{
				StringCchPrintf(temp, 256, TEXT("%d.%s"), questunique, pclquestinfo->szTitleText);
				StringCchCat(ptext, txtSize, temp);

				if(pclpersonquestinfo->clUnit[questtype].GetBit(i))
				{
					StringCchCat(ptext, txtSize, TEXT("(O)"));
				}
				else
				{
					StringCchCat(ptext, txtSize, TEXT("(X)"));
				}

				StringCchCat(ptext, txtSize, TEXT("\r\n"));
			}	
		}
	}
	return true;
}


// 흥부박 퀘스트의 글자 맞추기 단어중에서 랜덤하게 한 글자를 찾는다. 
SI32 cltQuestManager::GetRandWordAssembly()
{
	//	return -1;

	if(siWordAssemblyNumber <= 0)return -1;

	return (rand() % siWordAssemblyNumber);
}

bool cltQuestManager::GetTutorialMakeQuestItem(SI16 QuestUnique, SI32 ItemUnique, OUT cltMaterialInfo * clMaterialItem)
{
	if( QuestUnique == QUESTUNIQUE(25150) && ItemUnique == ITEMUNIQUE(10) ) // 튜토리얼 석검 제조 퀘스트. 퀘스트용 재료를 준다.
	{
		clMaterialItem[0].Set(5030,30);
		clMaterialItem[1].Set(5020,8);
		clMaterialItem[2].Set(7000,2);
		clMaterialItem[3].Set(6110,3);			
		return true;
	}
	
	return false;	
}

// [지연] 시간제한 퀘스트의 시간 갱신 표시 : 남은시간 갱신 함수
void cltQuestManager::UpdateRemainTime( SI32 siTimeReduce )
{
	if ( 0 >= siTimeReduce ) return;

	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pChar = (cltCharClient*)pClient->pclCM->CR[1];
	if( pChar == NULL) return;

	cltMissionQuestInfo* pMission = NULL;
	pMission = (cltMissionQuestInfo*)(&pChar->pclCI->clQuestInfo.clMissionQuestInfo);
	if ( NULL == pMission ) return;

	pMission->TimeReduce( siTimeReduce );
}
