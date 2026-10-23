#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"
#include "..\..\Server\TraceBoot.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
//--------------------------------------
// CommonLogic
//--------------------------------------
#include "MsgType-Person.h"
#include "Feast/Feast.h"
//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"

//-------------------------------------------
//Server
//-------------------------------------------
#include "..\..\Server\Statistics\Statistics.h"

#include "..\..\Client\Music\Music.h"
#include "..\common\NPCManager\NPC.h"

#include "..\..\Bullet\Bullet.h"
#include "Util\Util.h"
#include "..\..\Order\Order.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

#include "Skill\Skill-Manager.h"
#include "Char\CharServer\Char-Server.h"

#include "..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "../../../CommonLogic/Guild/GuildWar.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"
#include "..\..\CommonLogic\MsgRVal-Define.h"
#include "..\..\Common\SystemNPC\SystemNPC.h"

#include "../../Map/MapManager/MapManager.h"
#include "../../../CommonLogic/CityHall/CityHall.h"

#include "..\..\Client\NInterface\NNewInventory\NNewInventory.h"
#include "../../common/VoteMgr/VoteBaseInfo.h"
#include "../../common/Event/event.h"
#include "../../Server/ServerEvent/ServerEvent_Manager.h"

#include "../../CommonLogic/Marriage/MarriageManager.h"
#include "../../CommonLogic/Marriage/WeddingManager.h"

#include "../../CommonLogic/MsgType-Person.h"

#include "ITZCell.h"

extern cltCommonLogic* pclClient;

//-------------------------------------
// cltCharCommon
//-------------------------------------
cltCharCommon::cltCharCommon()
{
	siAccountStatus = 0;
	siIDNUM = 0;

	pclMap		= NULL;
	

	pclCM		= NULL;
	pclKI		= NULL;
	pclGMCommandKindInfo = NULL;
	pclCI		= new cltPersonInfo();

	//_LEON_GUILD
	//------------------------------------
	// ���κ� ������ġ ���� ���� �ӽ� ����
	//-----------------------------------
    siExpForMyGuild = 0;		// ��� Ż��� �ʱ�ȭ �ʿ�
	//------------------------------------
	// ���κ� ������ͱ� ���� ���� �ӽ� ����
	//-----------------------------------
    clProfitForMyGuild.Init();	// ��� Ż��� �ʱ�ȭ �ʿ�

	if(pclCI)
	{
		// �Ķ��Ÿ�� �����Ѵ�.
		clPB.pclCI				= pclCI;
	}
	
	// �Ķ��Ÿ �ڽ��� �ʱ�ȭ�Ѵ�. 
	clPB.Init(pclClient->pclItemManager, &(pclCI->clIP), &pclCI->clHealthInfo, pclKI, pclGMCommandKindInfo);


	siCenterX	= 0;
	siCenterY	= 0;

	//-------------------------------------------
	// ���� ���� �ʱ�ȭ 
	//-------------------------------------------
	clCurOrder.Init();

	//-------------------------------------------
	// ���� ���� �ʱ�ȭ. 
	//-------------------------------------------
	ReserveOrder.Init();

	siSubOrder		= 0;
	bBusySwitch		= false;
	
	
	
	//-----------------------------
	// �ź�
	//-----------------------------
	siID				= 0;
	CharUnique			= 0;
	clIdentity.Init();
	siChildCharUnique	= 0;
	siSummonIndex		= 0;

	//-----------------------------
	// ��ȣ����
	//-----------------------------
	siMySoulGuardUnique		= 0;
	clockSoulGuardCoolTime	= 0;

	bGodMode		= false;
	bOneKill		= false;	// ��ų���
	
	clAddInfo.Init();

	siCurrentAttackType	= 0;

	//-----------------------------
	// �Ӽ��� 
	//----------------------------
	Atb	= 0;                            // ��ü�� �Ӽ� 
	DefenceAtb	= 0;

	bActiveNPC = FALSE;

	//-------------------------------
	// �浹 ���� ����. 
	//-------------------------------
	SelfBlock	= 0;
	Block		= 0;
	SetIDSwitch	= FALSE;

	//----------------------------------
	// ��°� ����� ������ 
	//----------------------------------
	Animation				= 0;
	HeroAnimation			= 0;
	bUpdateAnimationSwitch	= false;
	Frame					= 0;

	MaxWaitType		= 0;
	WaitType		= 0;

	//----------------------------
	// ���� 
	//----------------------------
	siDirection		= 0;
	TempDirection	= 0;
	TargetDirection	= 0;

	//------------------------------
	// ���ݰ��� 
	//------------------------------
	dwAttackGiveUpDelay			= 0;
	dwLastAttackClock			= 0;
	dwLastAttackIntervalClock	= 0;
	dwLastMagicClock			= 0;
	dwLastTorpedoClock			= 0;

	ActiveWeapon				= PERSONITEM_WEAPON1;
	ActiveClothes               = PERSONITEM_DRESS;
	ActiveHat                   = PERSONITEM_HAT;

	//---------------------------
	// �̵�. 
	//---------------------------
	clMoveInfo.Init();
	siMoveMapIndex	= -1;

	//-----------------------------
	// ���� ���� 
	//-----------------------------
	siReservedTopSkillUnique	= 0;		// ���� ����Ǿ� �ִ� ž ��� 


	//---------------------------
	// ��� 
	//---------------------------
	siFarmX	= siFarmY	= 0;

	//---------------------------
	// ä��
	//---------------------------
	siMineX	= siMineY	= 0;

	
	//----------------------------
	// �ִϸ��̼� 
	//---------------------------
	pclCharDraw	= NULL;
	bHitDoneSwitch	= false;

	//------------------------------------
	// ���� ���õ� ���� 
	//------------------------------------
	siCurrentMagic			= 0;
	siCurrentMagicCastType	= 0;

	//------------
	// �ߵ� ���� 
	//------------
	siPoisonCharUnique = 0;			// ���� �ߵ���Ų ĳ���� ����ũ. 
	dwPoisonLastClock = 0;			// �ߵ��� �����ð�. 
	dwPoisonStartClock = 0;			// �ߵ� ���۽ð�. 
	siPoisonPower = 0;				// �ߵ��� ����. 
	dwPoisonCheckClock = 0;

	//-------------
	// ���� ���� 
	//-------------
	dwIceLastClock = 0;
	dwIceStartClock = 0;

	//-------------
	// ���ݷ� ���� ���� ���� 
	//-------------
	dwCurseAttackStartClock = 0;
	dwCurseAttackLastClock = 0;
	siCurseAttackPower = 0;			// ����. 

	//-------------
	// CRITICAL ���� ���� ���� 
	//-------------
	dwCurseCriticalStartClock = 0;
	dwCurseCriticalLastClock  = 0;
	siCurseCriticalPower = 0;		// ����. 

	//------------
	// ���� �ٴ� ������ ���� 
	//------------
	siMagicFireCharUnique = 0;			// ���� ���� ĳ���� ����ũ. 
	dwMagicFireLastClock = 0;			// ���� �����ð�. 
	dwMagicFireStartClock = 0;			// �� ���۽ð�. 
	siMagicFirePower = 0;				// ���� ����. 
	dwMagicFireCheckClock = 0;

	//-------------
	// DRAIN ���� 
	//-------------
	bDrainSwitch = false;
	siDrainFrame = 0;
	
	uiStaminaFrame = 0;

	//--------------
	// �ǰ�
	//--------------
	bHitSwitch	= false;		// �ǰ� ������ ����. 

	//-------------------
	// ���� 
	//-------------------
	siWarEnemyKillNumber	= 0;	// ���￡�� óġ�� ���Ǽ�. 
	siWarKilledNumber		= 0;
	siBlackArmyWarMapIndex	= -1;

	//-------------------
	// ���Ǳ��� ���� 
	//-------------------
	siBlackWarEnemyKillNumber = 0;
	siBlackWarKilledNumber = 0;
	siBlackWarEnemyKillScore = 0;
	siBlackWarEnemyKillVillageScore = 0;

	//------------------------------------
	// ���ĸ��� �̺�Ʈ
	//------------------------------------
//	siMofuMofuEventField	= -1;

	//------------------------------------
	// ��� ���� ���. 
	//------------------------------------
	siGameMasterLevel	= 0;

	//------------------------------------
	// ��
	//------------------------------------
	clJim.Init();

	//------------------------------------
	// ��Ƽ �ε���.
	//------------------------------------
	siPartyIndex	= 0;
	
	//------------------------------------
	// ������� ������
	//------------------------------------
	dwMakeFrame		= 0;

	//-------------------------------------
	// ���� ��� ����. 
	//-------------------------------------
	bNoFirstAttackSwitch	= false;
	//-------------------------------------
	// ICE ��� ����. 
	//-------------------------------------
	bNoIceAttackSwitch		= false;

	//-------------------------------------	
	// [����] ���� �귿 �۵����̸� �ֻ������ ����. => 2008-6-10
	//-------------------------------------
	bBuffRouletteSwitch		= false;

	//-------------------------------------	
	// [����] ������.
	//-------------------------------------
	bElixirSwitch			= false;
	
	//-------------------------------------	
	// [����] ���� PVP ������ �ִ�.
	//-------------------------------------
	siEnemUnique			= 0	;
	//-------------------------------------	
	// [����] ���� PVP ��ٸ��� �ð� 
	//-------------------------------------
	siPVPWaitTime			=	0	;
	//-------------------------------------	
	// [����] �����̺�Ʈ ���� ��ٸ��� �ð�
	//-------------------------------------
	siRacoonDodgeWaitTime	=	0	;
	//-------------------------------------	
	// [����] ���� PVP �ο�� �ð�
	//-------------------------------------
	siPVPTime				=	0	;
	//-------------------------------------	
	// [����] ���� �����ð�
	//-------------------------------------
    siPVPEndTime			=	0	;
	//-------------------------------------	
	// [����] �߷�Ÿ�ε��� �̺�Ʈ�� ���� �ڽ��� �ַ����� Ŀ������ ����� �ð�.
	//-------------------------------------
	dwValentineAnswerTime	=	0	;
	//-------------------------------------	
	// [����] ���� PVP ��ٸ��� �ð����� ���� ID ��� 
	//-------------------------------------
	siPVPWaitRequestId		=	0	;
	siPVPWaitAcceptId		=	0	;
	//-------------------------------------	
	// [����] PVP���� �� ī��Ʈ �ٿ��� �Ѵ�.
	//-------------------------------------
	bPvpCountDown			=	false	;
	// PVP ���
	siPVPWin				=	false	;
	siPVPLose				=	false	;
	siPVPDraw				=	false	;
	bPvpLeague				=	false	;

	//-------------------------------------	
	// PVP ������ ����
	//-------------------------------------	
	pclPVP_LeagueInfo		= NULL;
	pcltElixir				= NULL;
	pclMarriage				= NULL;
	pclFamily				= NULL;

	pclSoulGuard			= NULL;
	pclCharSkillBookInfo	= NULL;

	//-------------------------------------
	// ��Ʈ ����Ʈ ����.  ( Ŭ���̾�Ʈ������ ��ȿ�ϰ� ���δ�. �������� BUF_LOVEPOWER, BUF_FRIENDSHIP �ΰ��� �ߺ��� ����̴� )
	//-------------------------------------
	bHeartEffectSwitch		= false;

	siBufTransformKind		= 0;

	//clFriendInfo.Init();
	pclFriendInfo			= NULL;
	// [����] �����̺�Ʈ ���� ���� �Ӹ����� ī��Ʈ �̹����� �׸���
	bRacoonDodgeEventCountDown		=	false	;

	// [����] ���Һ� �̺�Ʈ : �׷��� ���ҽ� ��� : 5�� ī��Ʈ
	bBBBEvent_5SecCountDownFlag		= false;
	bBBBEvent_StageFailFlag			= false;
	bBBBEvent_StageClearFlag		= false;
	bBBBEvent_TotalStageClearFlag	= false;
	siBBBEvent_TotalStageClearResultImageIndex = 0;

	//--------------------------
	// ��Ʋ�ξ� �Ŀ��� ��� ���� ����
	//--------------------------
	bBattleRoyalPowerupMode = false;

	//-------------------------------------
	// ����� ���Ǵ� ����. 
	//-------------------------------------
	bDyingSwitch	= false;

	//-------------------------------------
	// ���λ��� 
	//-------------------------------------
	bPersonalShopSwitch	= false;		// ���λ����� ���� �ִ°�?
	
	bPremiumPersonalShop = false;

	bRepairShopSwitch	= false;

	//----------------------------
	// ���� �������ΰ� 
	//----------------------------
//	siPersonalMakeItemType = 0;


	bPersonalEnchantItemSwitch  = false;

	// ���� �������� �ؼ� ������ �����ΰ�
	bLevelUpDrawSwitch = false;

	//-------------------------------------------------
	// ��ȯ�� �� ����ڰ� ��Ʈ�� // -1�̸� PC�ڽ�
	//-----------------------------------------------
	siSelectedCharIndex = -1 ;

	//[����] �ڵ��� ���� ����.
	bCellularPhoneAuth  = false;
	bIsPromoter			= false;


	//-------------------------------------
	// ĳ������ �̺�Ʈ ����. 
	//-------------------------------------
	clInnerStatus.Init();

	//----------------------------------
	// �ǰݰ��� 
	//---------------------------------
	clBeHitInfo.Init();

	//----------------------------------
	// ��Ʈ ������ Ÿ�� 0�� ���� ����
	//---------------------------------
	m_siSetItemType = 0;
	m_siSetItemStep = 0;

	m_siSetItemType2 = 0;
	m_siSetItemStep2 = 0;

	m_siSetItemType3 = 0;
	m_siSetItemStep3 = 0;

	m_siSetItemType4 = 0;
	m_siSetItemStep4 = 0;

	m_siSetItemType5 = 0;
	m_siSetItemStep5 = 0;

	//ChangeEquipAbility = 0;

	//----------------------------------
	// ���� �������� ����
	//---------------------------------
	bIsElectionStump = false;
	StringCchCopy( szCandidate,		Const_Max_Candidate,	TEXT("") );
	StringCchCopy( szMaintenance,	Const_Max_Maintenance,	TEXT("") );

	pclSpaceBoxItem		= NULL;
	pclTreasureBoxItem	= NULL;	

	//-----------------------------
	// ������������
	//-----------------------------
	m_clMagicBufMgr.Init();

	m_uiDailyQuestionCount = 0;	// �Ϸ� ���� �ִ� Ƚ��.

	//-------------------------------------	
	// �߷�Ÿ�� ���� ���� ���� 
	//-------------------------------------	
	pclValentineWar = NULL ;

	m_uiWeaponSkillMasterEmblem = 0;


	//  [8/11/2009 ��ȣ_GAWIBAWIBO] ���´��� ����Ʈ
	m_siGawiBawiBoCount = 0;

	ZeroMemory(&m_stMuteTime, sizeof(m_stMuteTime));
}

cltCharCommon::~cltCharCommon()
{
	SAFE_DELETE( pclCI					)
	SAFE_DELETE( pclCharDraw			)
	SAFE_DELETE( pclFriendInfo			)
	SAFE_DELETE( pclSpaceBoxItem		)
	SAFE_DELETE( pclTreasureBoxItem 	)
	SAFE_DELETE( pclPVP_LeagueInfo		)
	SAFE_DELETE( pclValentineWar		)
	SAFE_DELETE( pcltElixir				)
	SAFE_DELETE( pclMarriage			)
	SAFE_DELETE( pclFamily				)
	SAFE_DELETE( pclSoulGuard			)
	NK_SAFE_DELETE( pclCharSkillBookInfo )
}


void cltCharCommon::CharInit(SI32 id, SI32 charunique, SI32 childcharunique, SI32 summonindex, SI32 dir,  cltMapCommon* pclmap, 
							 cltIdentity* pclidentityinfo, cltPersonInfo* pclUserCharInfo, cltInnerStatusInfo* pclInnerStatus, SI16 siSoulGuardUnique )
{
	SI32 i;

	//-------------------------------------------
	// �޸� ���� - ������ �����Ѵ�.
	//-------------------------------------------
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if( IsPC(charunique) )
		{
			if(pclFriendInfo		== NULL)	pclFriendInfo			= new cltFriendInfo;
			if(pclSpaceBoxItem		== NULL)	pclSpaceBoxItem			= new cltSpaceBox;
			if(pclTreasureBoxItem	== NULL)	pclTreasureBoxItem		= new cltTreasureBox;
			if(pclPVP_LeagueInfo	== NULL)	pclPVP_LeagueInfo		= new cltPVP_LeagueInfo;
			if(pclValentineWar		== NULL)	pclValentineWar			= new cltValentineWarInfo;
			if(pcltElixir			== NULL)	pcltElixir				= new cltElixir;
			if(pclMarriage			== NULL)	pclMarriage				= new CMarriage;
			if(pclFamily			== NULL)	pclFamily				= new CFamily;	//��������
			if(pclSoulGuard			== NULL)	pclSoulGuard			= new cltSoulGuard;
			if(pclCharSkillBookInfo	== NULL)	pclCharSkillBookInfo	= NkNew cltCharSkillBookInfo;
			

			// CharServer���� �ִ� ����
            if( ((cltCharServer*)this)->m_pclAgriculture	== NULL)	((cltCharServer*)this)->m_pclAgriculture		= new cltAgriculture;
			if( ((cltCharServer*)this)->m_pclShopRepairData	== NULL)	((cltCharServer*)this)->m_pclShopRepairData		= new cltShopRepairData;

		}
	}
	else if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		if( IsPC(charunique) )
		{
			if(pclFriendInfo		== NULL)	pclFriendInfo			= new cltFriendInfo;
			if(pclSpaceBoxItem		== NULL)	pclSpaceBoxItem			= new cltSpaceBox;
			if(pclTreasureBoxItem	== NULL)	pclTreasureBoxItem		= new cltTreasureBox;
			if(pclPVP_LeagueInfo	== NULL)	pclPVP_LeagueInfo		= new cltPVP_LeagueInfo;
			if(pclValentineWar		== NULL)	pclValentineWar			= new cltValentineWarInfo;
			if(pcltElixir			== NULL)	pcltElixir				= new cltElixir;
			if(pclMarriage			== NULL)	pclMarriage				= new CMarriage;
			if(pclFamily			== NULL)	pclFamily				= new CFamily;	// ��������
			if(pclSoulGuard			== NULL)	pclSoulGuard			= new cltSoulGuard;
			if(pclCharSkillBookInfo	== NULL)	pclCharSkillBookInfo	= NkNew cltCharSkillBookInfo;
		}
	}

	siCenterX		= 0;
	siCenterY		= 0;

	//-------------------------------------------
	// ���� ���� �ʱ�ȭ 
	//-------------------------------------------
	clCurOrder.Init();

	//-------------------------------------------
	// ���� ���� �ʱ�ȭ. 
	//-------------------------------------------
	ReserveOrder.Init();

	siSubOrder	= 0;
	bBusySwitch	= false;

	clMovePointList.Init();

	// �ۿ� ������ �ִ� ���·� �Ǿ� �ִ� ��ȯ���� ��� ������ ���� �ִ´�. 
	for( i = 0 ; i < MAX_SUMMON_OWN; ++i)
	{
		cltSummon *pclSummon = pclUserCharInfo->clSummonInfo.GetSummon(i);
		if(pclSummon && pclSummon->siStatus == SUMMON_STATUS_TAKEOUT)
		{
			pclSummon->siStatus = SUMMON_STATUS_TAKEIN;
		}
	}

	//-----------------------------
	// �ź� - [����] ��ȯ�� ���̺��� �����鼭 ó�� ���ͳ� ��ȯ�� ������ ���� ��ġ ����. 2008-8-29
	//-----------------------------
	siID				= id;
	CharUnique			= charunique;
	if(pclidentityinfo)
	{
		clIdentity.Set(pclidentityinfo);
	}
	else
	{
		clIdentity.Init();
	}
	if ( pclPVP_LeagueInfo ) 
	{
		pclPVP_LeagueInfo->Init()	;
	}
	if ( pclValentineWar ) 
	{
		pclValentineWar->Init()	;
	}

	siChildCharUnique	= childcharunique;
	siSummonIndex		= summonindex;

	siMySoulGuardUnique	= siSoulGuardUnique;

	bGodMode		= false;
	bOneKill		= false;	// ��ų���

	clAddInfo.Init();

	if( (siID > 0 && siID < MAX_CHAR_NUMBER) && (charunique >= 0 && charunique <  MAX_CHAR_NUMBER) )
	{
		CharUniqueInfo[charunique]	=	siID;
	}
	else
	{
		MsgBox(TEXT("fdsf2"), TEXT("Fd2dff:charunique:%d id:%d"), charunique, siID);
	}

	// �߿� �����͸� �����Ѵ�. 
	memcpy(pclCI, pclUserCharInfo, sizeof(cltPersonInfo));						//ok

	// �����ڵ� �߰� - By LEEKH
	if( GetKind() < 0  || GetKind() >= MAX_KINDINFO_NUMBER )			return;

	// �� ĳ������ ������ �ش��ϴ� KindInfo �����͸� ���´�. 
	//[����] ��ȯ�� ���̺� �и��� ĳ���� kind ������ �ʱ�ȭ�� �������� ��ȯ������ �����ϱ�. 2008-8-29
	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		if( clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			pclKI = pclClient->pclKindInfoSet->pclSummonKI[GetKind()];
			pclGMCommandKindInfo = pclClient->pclKindInfoSet->pclGMCommandKindInfo[GetKind()];
		}
		else
		{ 
			pclKI = pclClient->pclKindInfoSet->pclKI[GetKind()];
			pclGMCommandKindInfo = pclClient->pclKindInfoSet->pclGMCommandKindInfo[GetKind()];
		}
	}
	else
	{
		pclKI = pclClient->pclKindInfoSet->pclKI[GetKind()];
	}

	if(pclKI == NULL)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("CharInit() pclKI NULL id:%d charunique:%d kind:%d"), id, charunique, GetKind() );
		return;
	}
	
	// �Ķ��Ÿ �ڽ��� �ʱ�ȭ�Ѵ�. 
	clPB.Init(pclClient->pclItemManager, &(pclCI->clIP), &pclCI->clHealthInfo, pclKI, pclGMCommandKindInfo);


	// ĳ���Ͱ� Ȱ���� ������ �����ش�. 
	pclMap					= pclmap;

	// ������ ĳ���Ϳ� �����ڸ� �����. 
	if(pclCharDraw == NULL)
	{
		pclCharDraw = new CCharacterDrawing(pclClient->GameMode);
	}
	bHitDoneSwitch	= false;

	//------------------------------------
	// ��� ���� ���. 
	//------------------------------------
	siGameMasterLevel	= 0;


	siDirection		=	dir;
	TempDirection	=	dir;
	TargetDirection	=	dir;

	////-----------------------------
	//// �ź�
	////-----------------------------
	//siID				= id;
	//CharUnique			= charunique;
	//if(pclidentityinfo)
	//{
	//	clIdentity.Set(pclidentityinfo);
	//}
	//else
	//{
	//	clIdentity.Init();
	//}

	//siChildCharUnique	= childcharunique;
	//siSummonIndex		= summonindex;


	//bGodMode		= false;
	//bOneKill		= false;	// ��ų���

	//clAddInfo.Init();

	//if( (siID > 0 && siID < MAX_CHAR_NUMBER) && (charunique >= 0 && charunique <  MAX_CHAR_NUMBER) )
	//{
	//	CharUniqueInfo[charunique]	=	siID;
	//}
	//else
	//{
	//	MsgBox(TEXT("fdsf2"), TEXT("Fd2dff:charunique:%d id:%d"), charunique, siID);
	//}


	//----------------------------
	// ���� ��� 
	//----------------------------
	clAttackTarget.Init();
	siCurrentAttackType	= 0;

	//-----------------------------
	// �Ӽ��� 
	//----------------------------
	Atb			= pclKI->siAtb;
	DefenceAtb	= pclKI->DefenceAtb;

	//-------------------------------------------------------------
	// �浹 ���� ����. 
	//------------------------------------------------------------
	Block		=	pclKI->siBlock | BLOCK_SYSTEMNPC;
	SelfBlock	=	BLOCK_CHAR;

	{
		
		cltSystemNPC npcinfo;
		if( pclClient->pclSystemNPCManager->FindSystemNPCInfoFromNPCKind( GetKind(), &npcinfo ) ) {

			SelfBlock |= BLOCK_SYSTEMNPC;
		}

	}


	SetIDSwitch		= false;                            // ������ ������ �����ߴ��� ���θ� ����Ѵ�.

	//----------------------------------
	// ��°� ����� ������ 
	//----------------------------------
	Animation		= 0;					// � ������ �ִϸ��̼��� �����ϴ°�?
	HeroAnimation	= 0;
	bUpdateAnimationSwitch	= true;		// �ִϸ��̼��� �����������θ� �˷��ش�. 
	Frame			= 0;                		// ���� ��� �ܰ踦 �����ϰ� �ִ°�?

	// ���� ���� 
	MaxWaitType		= 1;   // �� ������ ���� �ִϸ��̼����� ���� �� �ִ� �ִ��� ���� 
	WaitType		= ANITYPE_WAIT0;       // ���� �� ������ ���ϰ� �ִ� ���� �ִ� 


	//----------------------------
	// ���� 
	//----------------------------
	siDirection				= SOUTH;                     				// ���� 
	TempDirection			= 0;                				// 16������ �����ϴ°�?
	TargetDirection			= 0;               				// ��ǥ�� ���� 


	//------------------------------
	// ���ݰ��� 
	//------------------------------
	dwAttackGiveUpDelay			= 0;
	dwLastAttackClock			= 0;
	dwLastAttackIntervalClock	= 0;
	dwLastMagicClock			= 0;
	dwLastTorpedoClock			= 0;

	ActiveWeapon				= PERSONITEM_WEAPON1;
	ActiveClothes               = PERSONITEM_DRESS;
	ActiveHat                   = PERSONITEM_HAT;

	//---------------------------
	// �̵�. 
	//---------------------------
	clMoveInfo.Init();
	siMoveMapIndex	= -1;

	//-----------------------------
	// ���� ���� 
	//-----------------------------
	siReservedTopSkillUnique	= 0;		// ���� ����Ǿ� �ִ� ž ��� 

	//---------------------------
	// ��� 
	//---------------------------
	siFarmX	= siFarmY	= 0;


	//---------------------------
	// ä��
	//---------------------------
	siMineX	= siMineY	= 0;


	// ���� ���¸� ���´�. 
	ParametaBoxAction();

	// by LEEKH 2007.12.24 - TRANSFORM
	if( (pclKI != NULL && GetKindInfo()->IsAtb(ATB_HERO)))
	{
		pclCharDraw->LoadItemCodeData(GetKind(), TEXT("KOR01"), TEXT("KOR01"), TEXT("KOR01"), NULL, NULL, 0, NULL );
	}

	// ���� Ŭ���̾�Ʈ ������ ĳ���� ��쿡 �ʱ�ȭ�Ѵ�.
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		((cltCharServer*)this)->InitCharServer();
	}
	else
	{
		((cltCharClient*)this)->InitCharClient();
	}


	// ����Ʈ �ִϸ��̼��� �����Ѵ�. 
	Animation			= ANITYPE_WAIT0;
	HeroAnimation		= 0;

	//  ĳ���� �̸��� �����Ѵ�. 
	if(IsPC(CharUnique) == false)
	{
		if ( GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) )
		{
			//[�߰� : Ȳ���� 2007. 10. 31 ������ �̸��� �ش� ���� ���� �̸����� ����.]
			SI32 siKind[50]	= {0, };
			SI32 siCount	= 0;

			cltNPCManagerClient* pclNPCMgr = (cltNPCManagerClient*)pclClient->pclNPCManager;
			if( pclNPCMgr )
			{
				//pclNPCMgr->GetKindMapInx(pclCM->CR[id]->GetMapIndex(), siKind, siKindLevel, 50);
				//////////////////////////////////////////////////////////////////////////////////////////////
				// �ش� �� �ε����� �ִ� ���� kind������
				SI32 siMapIndex = pclCM->CR[id]->GetMapIndex();
				SI32 siIndex	= 0;
				SI32 siId;

				while ( siId = pclClient->pclCM->GetCharOrder(siIndex) )
				{
					siIndex++;
					
					// NPC�� �ƴϸ� ��Ƽ��
					if ( !IsNPC(pclClient->pclCM->CR[siId]->GetCharUnique()) )
					{
						continue;
					}
					
					// ���� �ڽ��� CharUnique(������)�̸� ��Ƽ��
					if ( pclClient->pclCM->CR[siId]->GetCharUnique() == CharUnique )
					{
						continue;
					}
					
					// ���� �� �ʿ� �ִ� �ɸ��Ͱ� �ƴ϶�� ��Ƽ��
					if ( pclClient->pclCM->CR[siId]->GetMapIndex() != siMapIndex )
					{
						continue;
					}

					siKind[ siCount++ ] = pclClient->pclCM->CR[siId]->GetKind();

					if ( 50 <= siCount )
					{
						break;
					}

				}	// while ( id = pclClient->pclCM->GetCharOrder(siIndex) ) ����

				//////////////////////////////////////////////////////////////////////////////////////////////
				//[�˻� : Ȳ���� 2007. 10. 31 NPC ������ NPC�� �̸� ���.]
				if ( siCount > 0 )
				{
					SI32			siRand		= rand() % siCount;
					NTCHARString32	kNPCName	= pclClient->pclKindInfoSet->GetName(siKind[siRand]);
					
					
					if ( kNPCName.IsEmpty() == false )
					{
						SetName( kNPCName );
					}
					else
					{
						SetName( pclKI->GetName() );
					}
				}
				else
				{
					SetName( pclKI->GetName() );				
				}
			}	// if( pclNPCMgr ) ����
		}	// if ( GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) )
		else
		{
			SetName( pclKI->GetName() );
		}
	}

	//------------------------------------
	// ���� ���õ� ���� 
	//------------------------------------
	siCurrentMagic			= 0;
	siCurrentMagicCastType	= 0;

	// NPC�� �⺻������ �����Ѵ�.(PASSIVE���� ���Ѱ���)
	if(IsNPC(GetCharUnique())||IsInstanceNPC(GetCharUnique()))
	{
		if(pclKI->siMagic)
		{
			siCurrentMagic		 = pclKI->siMagic;
			stMagicAttributeData *pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( siCurrentMagic );
			if ( pMagicAttributeData )
			{
				siCurrentMagicCastType = pMagicAttributeData->si08MagicCastType;
			}
			
		}
	}

	//------------
	// �ߵ� ���� 
	//------------
	siPoisonCharUnique	= 0;			// ���� �ߵ���Ų ĳ���� ����ũ. 
	dwPoisonLastClock	= 0;			// �ߵ��� �����ð�. 
	dwPoisonStartClock	= 0;			// �ߵ� ���۽ð�. 
	siPoisonPower		= 0;			// �ߵ��� ����. 
	dwPoisonCheckClock	= 0;

	//-------------
	// ���� ���� 
	//-------------
	dwIceLastClock	= 0;
	dwIceStartClock = 0;


	//-------------
	// ���ݷ� ���� ���� ���� 
	//-------------
	dwCurseAttackStartClock		= 0;
	dwCurseAttackLastClock		= 0;
	siCurseAttackPower			= 0;	// ����. 

	//-------------
	// CRITICAL ���� ���� ���� 
	//-------------
	dwCurseCriticalStartClock	= 0;
	dwCurseCriticalLastClock	= 0;
	siCurseCriticalPower		= 0;	// ����. 

	//-------------
	// DEFENSE ���� ���� ���� 
	//-------------
	dwCurseDefenseStartClock	= 0;
	dwCurseDefenseLastClock		= 0;
	siCurseDefensePower			= 0;	// ����. 

	//-------------
	// MOVESPEED ���� ���� ���� 
	//-------------
	dwCurseMovespeedStartClock	= 0;
	dwCurseMovespeedLastClock	= 0;
	siCurseMovespeedPower		= 0;	// ����. 

	//-------------
	// MOVESPEED ���� ���� ���� 
	//-------------
	dwIncreaseMovespeedStartClock	= 0;
	dwIncreaseMovespeedLastClock	= 0;
	siIncreaseMovespeedPower		= 0;	// ����. 


	//------------
	// ���� �ٴ� ������ ���� 
	//------------
	siMagicFireCharUnique = 0;			// ���� ���� ĳ���� ����ũ. 
	dwMagicFireLastClock = 0;			// ���� �����ð�. 
	dwMagicFireStartClock = 0;			// �� ���۽ð�. 
	siMagicFirePower = 0;				// ���� ����. 
	dwMagicFireCheckClock = 0;

	//-------------
	// DRAIN ���� 
	//-------------
	bDrainSwitch = false;
	siDrainFrame = 0;

	uiStaminaFrame = 0;

	//--------------
	// �ǰ�
	//--------------
	bHitSwitch	= false;		// �ǰ� ������ ����. 

	//----------------------------------
	// �ǰݰ��� 
	//---------------------------------
	clBeHitInfo.Init();

	//-------------------
	// ���� 
	//-------------------
	siWarEnemyKillNumber	= 0;	// ���￡�� óġ�� ���Ǽ�. 
	siWarKilledNumber		= 0;
	siBlackArmyWarMapIndex	= -1;

	//-------------------
	// ���Ǳ��� ���� 
	//-------------------
	siBlackWarEnemyKillNumber = 0;
	siBlackWarKilledNumber = 0;
	siBlackWarEnemyKillScore = 0;
	siBlackWarEnemyKillVillageScore = 0;

	//------------------------------------
	// ��
	//------------------------------------
	clJim.Init();

	//------------------------------------
	// ��Ƽ �ε���.
	//------------------------------------
	siPartyIndex	= 0;

	//-------------------------------------
	// ���� ��� ����. 
	//-------------------------------------
	bNoFirstAttackSwitch	= false;
	//-------------------------------------
	// ICE ��� ����. 
	//-------------------------------------
	bNoIceAttackSwitch		= false;

	//-------------------------------------	
	// [����] ���� �귿 �۵����̸� �ֻ������ ����. => 2008-6-10
	//-------------------------------------
	bBuffRouletteSwitch		= false;

	//-------------------------------------	
	// [����] ������.
	//-------------------------------------
	bElixirSwitch			= false;

	siBufTransformKind		= 0;

	//-------------------------------------	
	// [����] ���� PVP ������ �ִ�.
	//-------------------------------------
	siEnemUnique				= 0	;
	//-------------------------------------	
	// [����] ���� PVP ��ٸ��� �ð� 
	//-------------------------------------
	siPVPWaitTime			=	0	;
	//-------------------------------------	
	// [����] ���� �����ð�
	//-------------------------------------
	siPVPEndTime		=	0	;
	//-------------------------------------	
	// [����] �����̺�Ʈ ���� ��ٸ��� �ð�
	//-------------------------------------
	siRacoonDodgeWaitTime	=	0	;
	//-------------------------------------	
	// [����] �߷�Ÿ�ε��� �̺�Ʈ�� ���� �ڽ��� �ַ����� Ŀ������ ����� �ð�.
	//-------------------------------------
	dwValentineAnswerTime	=	0	;
	//-------------------------------------	
	// [����] ���� PVP �ο�� �ð�
	//-------------------------------------
	siPVPTime			=	0	;
	// [����] ���� PVP ��ٸ��� �ð����� ���� ID ��� 
	//-------------------------------------
	siPVPWaitRequestId		=	0	;
	siPVPWaitAcceptId		=	0	;
	//-------------------------------------	
	// [����] PVP���� �� ī��Ʈ �ٿ��� �Ѵ�.
	//-------------------------------------
	bPvpCountDown			= false ;
	bPvpLeague				=	false	;
	// PVP ���
	siPVPWin				=	false	;
	siPVPLose				=	false	;
	siPVPDraw				=	false	;
	//-------------------------------------
	// ����� ���Ǵ� ����. 
	//-------------------------------------
	bDyingSwitch			= false;

	//-------------------------------------
	// ���λ��� 
	//-------------------------------------
 	bPersonalShopSwitch	= false;		// ���λ����� ���� �ִ°�?

	bPremiumPersonalShop = false;		// �����̾� ���λ����ΰ�?

	bRepairShopSwitch	= false;

	//-------------------------------------
	// ���� ������
	//-------------------------------------
	siPersonalMakeItemType	= 0;		// ���� �������ΰ�?

	// ���� �������� �ؼ� ������ �����ΰ�
	bLevelUpDrawSwitch = false;

	//-------------------------------------------------
	// ��ȯ�� �� ����ڰ� ��Ʈ�� // -1�̸� PC�ڽ�
	//-----------------------------------------------
	siSelectedCharIndex = -1 ;

	//[����] �ڵ��� ���� ����.
	bCellularPhoneAuth  = false;
	bIsPromoter			= false;

	// [����] �����̺�Ʈ ���� ���� �Ӹ����� ī��Ʈ �̹����� �׸���
	bRacoonDodgeEventCountDown	=	false	;

	// [����] ���Һ� �̺�Ʈ : �׷��� ���ҽ� ���
	//bBBBEvent_5SecCountDownFlag		= false;		// 5��start ī��Ʈ
	//bBBBEvent_StageFailFlag			= false;		// Lose
	//bBBBEvent_StageClearFlag		= false;		// win
	//bBBBEvent_TotalStageClearFlag	= false;		// 10stage win (����)
	//siBBBEvent_TotalStageClearResultImageIndex = 0;

	//-------------------------------------
	// ���� ������. 
	//-------------------------------------
	clInnerStatus.Set(pclInnerStatus);

	// ������ �̺�Ʈ ������ ���� �浹 ������ �缳���Ѵ�. 
	SetBlock();


	// ������� �������� �����Ѵ�. 
	dwMakeFrame = pclClient->GetFrame();

	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		((cltCharClient*)this)->SetFileNFont();
	}

	SetInfo();

	//----------------------------------
	// ��Ʈ ������ Ÿ�� 0�� ���� ����
	//---------------------------------
	m_siSetItemType = 0;
	m_siSetItemStep = 0;

	m_siSetItemType2 = 0;
	m_siSetItemStep2 = 0;

	m_siSetItemType3 = 0;
	m_siSetItemStep3 = 0;

	m_siSetItemType4 = 0;
	m_siSetItemStep4 = 0;

	m_siSetItemType5 = 0;
	m_siSetItemStep5 = 0;

	//ChangeEquipAbility = 0;

	if(IsPC(GetCharUnique()))
	{
		//[����] ��ƼȦ. ��Ƽ�忡�� ���� �������� ����. => 2008. 3. 12
		PartyHallItemRemove(id);

		if( !pclClient->IsCountrySwitch( Switch_Festival_Food ))
		{
			//[����] �������� ������ ����.
			FestivalFoodItemRemove(id);
		}

		//[����] ����ũ�ι��� ���� ����Ҽ� ���� �Ÿ� ���� => 2008-8-14
		if( pclClient->IsCountrySwitch(Switch_FourLeaf))
		{
			NotFourLeafItemRemove(id);
		}
	}

	//----------------------------------
	// ���� �������� ����
	//---------------------------------
	bIsElectionStump = false;
	StringCchCopy( szCandidate,		Const_Max_Candidate,	TEXT("") );
	StringCchCopy( szMaintenance,	Const_Max_Maintenance,	TEXT("") );

	//-----------------------------
	// ������������
	//-----------------------------
	m_clMagicBufMgr.Init();

	m_uiWeaponSkillMasterEmblem = 0;

	ZeroMemory(&m_stMuteTime, sizeof(m_stMuteTime));
}


SI32 cltCharCommon::GetX()const{return pclCI->clBI.GetX();}
SI32 cltCharCommon::GetY()const{return pclCI->clBI.GetY();}

void  cltCharCommon::SetX(SI32 x){pclCI->clBI.SetX(x);} 
void  cltCharCommon::SetY(SI32 y){pclCI->clBI.SetY(y);}

// ������ �����ش�.
SI32 cltCharCommon::GetKind()const{return pclCI->clBI.GetKind();}
void cltCharCommon::SetKind(SI32 kind){pclCI->clBI.SetKind(kind);}

cltKindInfo* cltCharCommon::GetKindInfo(bool bForDraw /* = false */)	// KindInfo ������ �����ش� - ���� �Ŀ��� ���ŵ� ĳ������ ������ ��ȯ�ȴ�. - by LEEKH 2007.12.24
																		// �ܰ����� ������ ���� ���� �߰� - by LEEKH 2008-09-10
{
	cltKindInfo*	retKI = pclKI;

	if(clTransFormInfo.bTransFormMode)
	{
		// PCK : �߽���(����) �߰��� ���� ���� ���� ����, �������Ǹ� ���Ż��¿��� ���Ź����� �ɸ��� �ܰ��� ���Ź����� ���󰣴�.
		if( siBufTransformKind > 0 && bForDraw )
		{
			{
				cltKindInfo*	tempKI = pclClient->pclKindInfoSet->pclKI[siBufTransformKind];
				// ����ĳ���Ϳ� �ش� ���ϸ��̼��� �ִ� ��쿡�� ����ĳ������ �׸� ������ ��ȯ�Ѵ�.
				if( tempKI->clCharAniManager.FindRefFromAnitype(Animation) > 0 )
					retKI = tempKI;
			}
		}
		else
		{
			if(clTransFormInfo.uiNowTransFormKind >= 0 && clTransFormInfo.uiNowTransFormKind < MAX_KINDINFO_NUMBER)
			{
				if(pclClient->pclKindInfoSet->pclKI[clTransFormInfo.uiNowTransFormKind] != NULL)
					retKI = pclClient->pclKindInfoSet->pclKI[clTransFormInfo.uiNowTransFormKind];
			}
		}

	}
	else
	{
		if( siBufTransformKind > 0 && bForDraw )
		{
			if(pclClient->pclKindInfoSet->IsValidKind(siBufTransformKind))
			{
				cltKindInfo*	tempKI = pclClient->pclKindInfoSet->pclKI[siBufTransformKind];
				// ����ĳ���Ϳ� �ش� ���ϸ��̼��� �ִ� ��쿡�� ����ĳ������ �׸� ������ ��ȯ�Ѵ�.
				if( tempKI->clCharAniManager.FindRefFromAnitype(Animation) > 0 )
					retKI = tempKI;
			}
		}
	}

	return retKI;
}


// �� �ε����� ���´�. 
SI32 cltCharCommon::GetMapIndex()const
{
	return pclCI->clBI.GetMapIndex();
}
// �� �ε����� �����Ѵ�.  
void cltCharCommon::SetMapIndex(SI32 index)
{
	pclCI->clBI.SetMapIndex(index);
}

// ĳ������ �̸��� ���´�. 
const TCHAR* cltCharCommon::GetName()const
{	
	return pclCI->clBI.GetName();
}

void cltCharCommon::SetName(const TCHAR* name)
{ 
	pclCI->clBI.SetName(name);
}

SI32 cltCharCommon::GetMana()
{
/*	if(pclCI->clIP.GetMana() > clPB.GetMaxMana())
	{
		pclCI->clIP.SetMana( clPB.GetMaxMana() );

		return clPB.GetMaxMana();
	}
*/
	return pclCI->clIP.GetMana();
}

void cltCharCommon::SetMana(SI32 mana)
{
	pclCI->clIP.SetMana(mana);
}

SI32 cltCharCommon::DecreaseMana(SI32 mana)
{
	pclCI->clIP.DecreaseMana( max(0, mana) );

	if(GetMana() < 0 )
	{
		pclCI->clIP.SetMana(0);
	}

	return GetMana();
}
SI32 cltCharCommon::IncreaseMana(SI32 mana)
{
	pclCI->clIP.IncreaseMana( mana );

	if(GetMana() > clPB.GetMaxMana())
	{
		pclCI->clIP.SetMana( clPB.GetMaxMana() );
	}
	return GetMana();
}

SI32 cltCharCommon::GetLife()
{
/*	if(pclCI->clIP.GetLife() > clPB.GetMaxLife())
	{
		pclCI->clIP.SetLife( clPB.GetMaxLife() );

		return clPB.GetMaxLife();
	}
*/
	return pclCI->clIP.GetLife();
}
void cltCharCommon::SetLife(SI32 life){pclCI->clIP.SetLife(life);}

SI32 cltCharCommon::IncreaseLife(SI32 life)
{
	pclCI->clIP.IncreaseLife( life );

	if(GetLife() > clPB.GetMaxLife())
	{
		pclCI->clIP.SetLife( clPB.GetMaxLife() );
	}

	return  GetLife();
}
SI32 cltCharCommon::GetWis()const{return pclCI->clIP.GetWis();}

SI32 cltCharCommon::GetStr()const{return pclCI->clIP.GetStr();}

SI32 cltCharCommon::GetDex()const{return pclCI->clIP.GetDex();}

SI32 cltCharCommon::GetVit()const{return pclCI->clIP.GetVit();}
void cltCharCommon::SetVit(SI32 vit){pclCI->clIP.SetVit(vit);}

SI32 cltCharCommon::GetMag()const{return pclCI->clIP.GetMag();}
void cltCharCommon::SetMag(SI32 mag){pclCI->clIP.SetMag(mag);}

SI32 cltCharCommon::GetHnd()const{return pclCI->clIP.GetHnd();}
void cltCharCommon::SetHnd(SI32 hnd){pclCI->clIP.SetHnd(hnd);}

// �� �ڽ��� ����ġ�� ���Ѵ�.(��밡 ���� óġ���� �� ���� �� �ִ� ����ġ. ) 
SI64 cltCharCommon::GetMyExp()const{return pclKI->Exp + (pclCI->clIP.GetLevel()-1) ;}	// SI32->SI64 ����ġ �����÷ο춧���� ����


//-------------------------------------
// ���ݰ���. 
//-------------------------------------
void cltCharCommon::GetHitPoint(cltPos* pclpos)
{
	pclpos->siX = siCenterX;
	pclpos->siY = siCenterY;
}

// ������ ���� ������ ��ġ�� �˷��ش�.
void cltCharCommon::DecideAttackPoint(SI32 port, SI32& x, SI32&y)
{
	x	=	siCenterX;
	y	=	siCenterY;
}

// �������� �۾��� ���� �ܰ踦 ���´�. 
SI32 cltCharCommon::GetSubOrder(){return siSubOrder;}


//-----------------------------------------
// ��ȣõ�� ���� 
//-----------------------------------------
void cltCharCommon::SetAngel(bool mode)
{
	pclCI->clBI.uiAngelSwitch = mode;

	// Ŭ���̾�Ʈ�� ������. 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_ANGEL, mode);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNetMsg((sPacketHeader*)&clMsg);

}

void cltCharCommon::SetPolice(bool mode)
{
	pclCI->clBI.uiPoliceSwitch = mode;

	// Ŭ���̾�Ʈ�� ������. 
	cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_POLICE, mode);
	cltGameMsgResponse_StatusInfo clChangeInfo(GetCharUnique(), &StatusInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
	SendNetMsg((sPacketHeader*)&clMsg);
}

//--------------------------------------------------------
// ��ǥ ���� ���� 
//--------------------------------------------------------
// Person�� ���� ��ġ�� ������ Unique�� ���Ѵ�. 
SI32 cltCharCommon::GetCurrentVillageUnique()
{
	// ������ �ε����� ���Ѵ�. 
	SI32 mapindex = GetMapIndex();

	return pclClient->pclMapManager->GetVillageUnique(mapindex);
}


// Person�� ���� ��ġ�� �������� �ִ� ������ Unique�� ���Ѵ�. 
SI32 cltCharCommon::GetHuntVillageUnique()
{
	// ������ �ε����� ���Ѵ�. 
	SI32 mapindex = GetMapIndex();

	if(mapindex < 200)return 0;
	if(mapindex >= 200 + MAX_VILLAGE_NUMBER)return 0;

	return mapindex - 200;
}

bool cltCharCommon::PreCharAction()
{

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		// ĳ���� ���� �Ŀ� ��ȣȭŰ�� ĳ���� ����ũ ���� Ŭ���̾�Ʈ�� ������. 
		((cltCharServer*)this)->SecretNCharUniqueSendOperation();

		// Ŭ���̾�Ʈ�κ��� ���� �ð� �޽����� ���� ���� �ڵ� �α׾ƿ� ��Ų��. 
		if(((cltCharServer*)this)->AutoLogOut() == true)
		{
			return true;
		}
	}

	// �ǰݽ��� ����� �����ش�.
	if(BeHitOperation() == true)
	{
		return true;
	}

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		// ������ ���ݹ������� ��ġ
		((cltCharServer*)this)->BeAttackedAction();
	}

	return false;
}



//---------------------------------------
// Action
//---------------------------------------
BOOL cltCharCommon::Action()
{
	// ���������� ĳ���Ͱ� �� ������ �۵������� �ʴ´�. 
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		DWORD dwdata = (GetID() % CHAR_ACTION_INTERVAL_FRAME);
		if( dwdata != (pclClient->GetFrame() % CHAR_ACTION_INTERVAL_FRAME) )
		{
			return true;
		}

		// NPC�� ��� �ֺ� ���� �÷��̾ �ִ��� �����ϰ�,
		// �÷��̾ �Ѹ��� ���ٸ�, Action�� �۵� ���� �ʴ´�.

		if(( IsNPC(GetCharUnique()) || IsInstanceNPC(GetCharUnique()) ) &&
			clIdentity.siIdentity != IDENTITY_SOULGUARD &&
			clIdentity.siIdentity != IDENTITY_PERSON_SUMMON &&
			((cltCharServer*)this)->siCharMode != CHARMODE_BLACKWAR &&
			((cltCharServer*)this)->pclCI->clIP.GetLife() > 0 )
		{
			ITZCell *pCell = GetCell();

			if( pCell )
			{
				// �ֺ� ���� �÷��̾ �ִ°�?
				if( !pCell->IsTherePlayerNearCells() )
				{
					// bActiveNPC�� TRUE�� �����Ǿ��־��ٸ�, FALSE�� �����ϰ�,
					// ActiveNPCNum�� ���ҽ�Ų��
					if( bActiveNPC )
					{
						bActiveNPC = FALSE;
						pclCM->pclIDManager->SubActiveNPCNumber();
					}
					return true;
				}
				else
				{
					// bActiveNPC�� FALSE�� �����Ǿ��־��ٸ�, TRUE�� �����ϰ�,
					// ActiveNPCNum�� ������Ų��
					if( bActiveNPC == FALSE )
					{
						bActiveNPC = TRUE;
						pclCM->pclIDManager->AddActiveNPCNumber();
					}
				}
			}
		}
		else
		{
			if( bActiveNPC == FALSE )
			{
				bActiveNPC = TRUE;
				pclCM->pclIDManager->AddActiveNPCNumber();
			}
		}
	}

	bool rtnval = PreCharAction();

	if(rtnval)
	{
		return true;
	}

	// ����� ������ �����Ѵ�. 
	if(ReserveOrder.siOrder != ORDER_WAIT)
	{
		GetOrderBuffer();
	}


	// ������ �۾�.
	// ������ �Է¹޴�  GetOrderBuffer()���Ŀ� ó���Ǿ�� �Ѵ�. 
	CellAction();


	//-------------------------------------
	// �������� �������� �״� ������ ó��. 
	//-------------------------------------
	if(GetLife() <= 0)
	{
		if(GetCurrentOrder() != ORDER_DIE && GetCurrentOrder() !=ORDER_DEAD) 
		{
			SetCurrentOrder(ORDER_DIE);
		}
	}
	else
	{

		//-------------------------------------
		// ������ ���� ��ġ. 
		//-------------------------------------
		if(clPB.siReservedLife)
		{
			if(clPB.siReservedLife  >= pclCM->siIncLifeUnit)
			{
				clPB.siReservedLife	-= pclCM->siIncLifeUnit;
				IncreaseLife(pclCM->siIncLifeUnit);

#ifdef _DEBUG
#ifdef _CHINA
				pclClient->pclLog->FileConfigPrint( "ReservedLife", "Update" );
#endif
#endif
			}
			else
			{
				pclCI->clIP.Life	+= clPB.siReservedLife;
				clPB.siReservedLife	= 0;
#ifdef _DEBUG
#ifdef _CHINA
				pclClient->pclLog->FileConfigPrint( "ReservedLife", "Update" );
#endif
#endif
			}

			// ��ȯ���� ���, �θ��� ������ ������Ʈ �Ѵ�. 
			SI32 parentid = GetSummonParentID();
			if(pclCM->IsValidID(parentid) && GetSummonIndex() >= 0 && GetSummonIndex() < MAX_SUMMON_OWN)
			{
				pclCM->CR[parentid]->pclCI->clSummonInfo.clSummon[GetSummonIndex()].clIP.SetLife(pclCI->clIP.Life);
			}
		}
	}


	// ���� ������ �缳���Ѵ�.
	bool bcheckswitch = false;
	if(TABS(pclClient->CurrentClock - clCommonSimpleVary.dwLastCheckParametaBoxActionClock) > (int)pclCM->dwParametaBoxActionIntervalClock)
	{
		clCommonSimpleVary.dwLastCheckParametaBoxActionClock = pclClient->CurrentClock;
		bcheckswitch = true;
	}
	if(clCommonSimpleVary.bForceparametaBoxActionSwitch)
	{
		bcheckswitch = true;
	}

	if(bcheckswitch)
	{
		ParametaBoxAction();
	}

	// �ѹ� ������ ������ �ڿ� �ɼ��� Ǯ���ش�. - by LEEKH
	if(clCommonSimpleVary.bForceparametaBoxActionSwitch)
	{
		clCommonSimpleVary.bForceparametaBoxActionSwitch = false;
	}

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		((cltCharServer*)this)->CharServerAction();
	}
	else
	{
		((cltCharClient*)this)->CharClientAction();
	}
	switch(GetCurrentOrder())
	{
		//------------------------------
		// ��� ���� 
		//------------------------------
	case ORDER_WAIT:

		DeleteID();
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			((cltCharServer*)this)->OperateWaitServer();
		}
		else
		{
			((cltCharClient*)this)->OperateWaitClient();
		}
		SetID();


		break;

		//------------------------------
		// ���� ������ �ߴ�.  
		//------------------------------
	case ORDER_STOP:
	case ORDER_FORCESTOP:
	case ORDER_REVIVE:
		DeleteID();

		SetCurrentOrder(ORDER_WAIT);

		SetID();

		break;


		//------------------------------
		// ���� ����. 
		//------------------------------
	case ORDER_DIE:

		DeleteID();

		if(Action_Dying()	==	true)
		{
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				SetCurrentOrder(ORDER_DEAD);
			}
		}
		else SetID();

		break;

		//------------------------------
		// ���� ����. 
		//------------------------------
	case ORDER_DEAD:

		Dead();

		if( bActiveNPC ) {
			bActiveNPC = FALSE;
			pclCM->pclIDManager->SubActiveNPCNumber();
		}


		return FALSE; 
		break;

		//------------------------------
		// �̵� ����.
		//------------------------------
	case ORDER_MOVE:
		{
			DeleteID();

			// �������� ���� 
			if(Move() == MOVERTNVAL_MOVEDONE)
			{
				OrderStop(siID, BY_COMPUTER);
			}

			if(clMoveInfo.siMovePercent==0)
			{
				bBusySwitch	=	false;
			}
			else 
			{
				bBusySwitch	=	true;
			}

			SetID();

		}

		break;
		
	//------------------------------
	// ���� �̵� ����.
	//------------------------------
	case ORDER_STRAIGHTMOVE:

		DeleteID();

		// �������� ���� 
		if(Move(true) == MOVERTNVAL_MOVEDONE)
		{
			OrderStop(siID, BY_COMPUTER);
		}
		
		if(clMoveInfo.siMovePercent==0)
		{
			bBusySwitch	=	false;
		}
		else 
		{
			bBusySwitch	=	true;
		}

		SetID();


		break;


	//------------------------------
	// ���� ����
	//------------------------------
	case ORDER_ATTACK:

		DeleteID();

		switch(Action_Attack()) 
		{
		case -1:	OrderStop(siID, BY_COMPUTER); 			break;
		case -2:	OrderForceStop(siID, BY_COMPUTER); 		break;
		default:											break;
		}
	
		SetID();

		break;

	//------------------------------
	// ���� ���� ����. 
	//------------------------------
	case ORDER_MAGIC:

		DeleteID();
		Magic();
		SetID();

		break;
		

	case ORDER_FISH:

		DeleteID();
		Fish();
		SetID();

		break;

	case ORDER_FARM:

		DeleteID();
		Farm();
		SetID();

		break;

	case ORDER_MINE:

		DeleteID();
		Mine();
		SetID();

		break;

	case ORDER_SITDOWN:
		{
			SI32 haveCushion = ((cltCharClient*)pclCM->CR[GetID()])->siCushionUnique;
			SI32 SitDownAnytype = ANITYPE_SITDOWN;
			
			if( haveCushion > 0 )
			{
				switch(haveCushion)
				{
					case  ITEMUNIQUE(13628): // �̺�1
					case  ITEMUNIQUE(13629): // �̺�2
						SitDownAnytype = ANITYPE_SLEEP;
						break;
					case  ITEMUNIQUE(13649): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
					case  ITEMUNIQUE(13650): // ���ڴ� �Ŀ��� �漮(�Ϻ���)
					case  ITEMUNIQUE(13647): // ���ڴ� �Ŀ��� �漮(���糲)
					case  ITEMUNIQUE(13648): // ���ڴ� �Ŀ��� �漮(�����)
						SitDownAnytype = ANITYPE_SLEEPCAT;
						break;
					default:
						break;
				}
			}

			DeleteID();		
			SitDown(SitDownAnytype);
			SetID();
		}
		break;

		//------------------------------
		// ���� ����
		//------------------------------
	case ORDER_ATTACK_CLIENT:

		DeleteID();

		switch(Action_Attack()) 
		{
		case -1:	OrderStop(siID, BY_COMPUTER); 			break;
		case -2:	OrderForceStop(siID, BY_COMPUTER); 		break;
		default:											break;
		}

		SetID();

		break;

	case ORDER_SPECIAL_ACTION:
		{
			if( Action_Special() )
			{
				OrderStop(GetID(), BY_COMPUTER);
			}
		}
		break;

	default:
		{
		    MsgBox(TEXT("CharOrder"), TEXT("No Order kind:%d  order:%d"), GetKind(), GetCurrentOrder());
		}
		break;
		
	}

	// ȭ�鿡 ���� ��ǥ�� ���Ѵ�. 
	CalculateRealXY();
	AfterCharAction();
	
	return true;



}


// ĳ������ ������ �����. 
BOOL cltCharCommon::SetID()
{
	if(pclMap == NULL)return FALSE;

	if((pclKI != NULL && pclKI->IsAtb(ATB_HORSE) == true))
	{
		return TRUE;
	}
	
	pclMap->SetCollMap(GetX(), GetY(), SelfBlock);
	pclMap->SetIDMap(siID, GetX(), GetY());
	 
	SetIDSwitch=TRUE;

	return TRUE;
}


// ĳ������ ������ ���ش�. 
BOOL cltCharCommon::DeleteID()
{
	if(SetIDSwitch == FALSE)return FALSE;

	// ���� �ٴڿ��� ������ �ʴ´�.
	if((pclKI != NULL && pclKI->IsAtb(ATB_HORSE) == true))return TRUE;

	pclMap->ClearIDMap(GetX(), GetY());
	pclMap->DeleteCharCollMap(GetX(), GetY());
	
	SetIDSwitch=FALSE;

	return TRUE;
}



// ĳ���Ͱ� �����Ǿ����� �˸���. 
void cltCharCommon::SetInfo()
{
	TraceBoot("SETINFO charID=%d kind=%d charUnique=%d", GetID(), GetKind(), GetCharUnique());
	pclClient->pclCM->SetInfo(this);
}

// id�� range�����ȿ� �ִ°�?
BOOL cltCharCommon::IsInRange(SI32 id, SI32 range)
{

	if(pclCM->IsAlive(id)==FALSE)return FALSE;

	SI32 dx	=	pclCM->CR[id]->GetX();
	SI32 dy	=	pclCM->CR[id]->GetY();

	// ��ǥ������ �Ÿ� 
	SI32 length = max( TABS(dx - GetX()), TABS(dy - GetY())  );

	// ���� �Ÿ� 
	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 reallength = length - ((GetKindInfo()->siSize - 1)/ 2) - ((pclCM->CR[id]->GetKindInfo()->siSize - 1) / 2);

	if(reallength <= range)return TRUE;



	return FALSE;
}


//--------------------------------------------------------------------------------------------
// �ִϸ��̼� ����. 
//--------------------------------------------------------------------------------------------
// Hero Animation �ڵ带 �����Ѵ�. 
SI32 cltCharCommon::CalcHeroAnimation()
{
	TCHAR anicode[256] =  TEXT("");


	switch(Animation)
	{
	case ANITYPE_WAIT0:
		if( GetKind() == KIND_HERO3 ||
			GetKind() == KIND_HEROINE3 ||
			GetKind() == KIND_HERO4 ||
			GetKind() == KIND_HEROINE4 )
		{   

			if( bLevelUpDrawSwitch == true )
			{
				StringCchCopy(anicode, 256, TEXT("SPC")); // ������ ����
			}			
			else if( (siPersonalMakeItemType == MAKE_ANI_ARMOR) || (siPersonalMakeItemType == ENCHANT_ANI_WEPONE)|| (siPersonalMakeItemType == ENCHANT_ANI_ARMOR) )
			{			
				StringCchCopy(anicode, 256, TEXT("MAM")); // ���� ,�� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_HEAL)
			{
				StringCchCopy(anicode, 256, TEXT("MHE")); // ���� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_FOOD) 
			{
				StringCchCopy(anicode, 256, TEXT("MFO")); // ���� ��������
			}
			else if ( (bPersonalShopSwitch == true && bPremiumPersonalShop == true) || ( bIsElectionStump == true ) || bRepairShopSwitch == true ) 
			{
				StringCchCopy(anicode, 256, TEXT("SOP")); // ���λ��� ����
			}
			else
			{
				StringCchCopy(anicode, 256, TEXT("WT1")); // ��⵿��
			}
		}
		else
		{
				StringCchCopy(anicode, 256, TEXT("WT1")); // ��⵿��
		}		
		break;
	case ANITYPE_WAIT1:
		if( GetKind() == KIND_HERO3 ||
			GetKind() == KIND_HEROINE3 ||
			GetKind() == KIND_HERO4 ||
			GetKind() == KIND_HEROINE4 )
		{        
			if( bLevelUpDrawSwitch == true )
			{
				StringCchCopy(anicode, 256, TEXT("SPC")); // ������ ����
			}			
			else if( (siPersonalMakeItemType == MAKE_ANI_ARMOR) || (siPersonalMakeItemType == ENCHANT_ANI_WEPONE)|| (siPersonalMakeItemType == ENCHANT_ANI_ARMOR) )
			{
				StringCchCopy(anicode, 256, TEXT("MAM")); // ���� ,�� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_HEAL)
			{
				StringCchCopy(anicode, 256, TEXT("MHE")); // ���� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_FOOD) 
			{
				StringCchCopy(anicode, 256, TEXT("MFO")); // ���� ��������
			}
			else if ( (bPersonalShopSwitch == true && bPremiumPersonalShop == true) || ( bIsElectionStump == true ) || bRepairShopSwitch == true ) 
			{
				StringCchCopy(anicode, 256, TEXT("SOP")); // ���λ��� ����
			}
			else
			{
				StringCchCopy(anicode, 256, TEXT("WT2")); // ��⵿��
			}
		}
		else
		{
			StringCchCopy(anicode, 256, TEXT("WT2")); // ��⵿��
		}
		break;
	case ANITYPE_ATTACK:
	case ANITYPE_ATTACK1:
	case ANITYPE_ATTACK2:
		// ����ϰ� �ִ� ���⿡ ���� �ٸ� ������ ���Ѵ�. 
		switch(clPB.siArmWeaponUnique)
		{
		case ITEMTYPE_SWORD: 
		case ITEMTYPE_STAFF:
		case ITEMTYPE_AXE:
			StringCchCopy(anicode, 256, TEXT("1HS"));
			break;
		case ITEMTYPE_SPEAR: 
			StringCchCopy(anicode, 256, TEXT("2HT"));
			break;
		case ITEMTYPE_BOW:
			StringCchCopy(anicode, 256, TEXT("2HB"));
			break;
		case ITEMTYPE_GUN:
			StringCchCopy(anicode, 256, TEXT("2HG"));
			break;
		default:
			StringCchCopy(anicode, 256, TEXT("1HS"));
			break;
		}
		break;
	case ANITYPE_DYING:
		StringCchCopy(anicode, 256, TEXT("DTH"));
		break;
	case ANITYPE_MOVE:
		StringCchCopy(anicode, 256, TEXT("MOV"));
		break;
	case ANITYPE_HIT:
		StringCchCopy(anicode, 256, TEXT("HIT"));
		break;
	case ANITYPE_FISH:
		StringCchCopy(anicode, 256, TEXT("FSH"));
		break;
	case ANITYPE_SLEEP:

		if( GetKind() == KIND_HERO3 ||
			GetKind() == KIND_HEROINE3 ||
			GetKind() == KIND_HERO4 ||
			GetKind() == KIND_HEROINE4 )
		{        
			if( bLevelUpDrawSwitch == true )
			{
				StringCchCopy(anicode, 256, TEXT("SPC")); // ������ ����
			}			
			else if( (siPersonalMakeItemType == MAKE_ANI_ARMOR) || (siPersonalMakeItemType == ENCHANT_ANI_WEPONE)|| (siPersonalMakeItemType == ENCHANT_ANI_ARMOR) )
			{			
				StringCchCopy(anicode, 256, TEXT("MAM")); // ���� ,�� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_HEAL)
			{
				StringCchCopy(anicode, 256, TEXT("MHE")); // ���� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_FOOD) 
			{
				StringCchCopy(anicode, 256, TEXT("MFO")); // ���� ��������
			}
			else if ( (bPersonalShopSwitch == true && bPremiumPersonalShop == true) || ( bIsElectionStump == true ) || bRepairShopSwitch == true ) 
			{
				StringCchCopy(anicode, 256, TEXT("SOP")); // ���λ��� ����
			}
			else 
			{
				StringCchCopy(anicode, 256, TEXT("SLEP"));
			}
		}
		else
		{
			StringCchCopy(anicode, 256, TEXT("SLEP"));
		}		
		break;
	case ANITYPE_SLEEPCAT:

		if( GetKind() == KIND_HERO3 ||
			GetKind() == KIND_HEROINE3 ||
			GetKind() == KIND_HERO4 ||
			GetKind() == KIND_HEROINE4 )
		{        
			if( bLevelUpDrawSwitch == true )
			{
				StringCchCopy(anicode, 256, TEXT("SPC")); // ������ ����
			}			
			else if( (siPersonalMakeItemType == MAKE_ANI_ARMOR) || (siPersonalMakeItemType == ENCHANT_ANI_WEPONE)|| (siPersonalMakeItemType == ENCHANT_ANI_ARMOR) )
			{			
				StringCchCopy(anicode, 256, TEXT("MAM")); // ���� ,�� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_HEAL)
			{
				StringCchCopy(anicode, 256, TEXT("MHE")); // ���� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_FOOD) 
			{
				StringCchCopy(anicode, 256, TEXT("MFO")); // ���� ��������
			}
			else if ( (bPersonalShopSwitch == true && bPremiumPersonalShop == true) || ( bIsElectionStump == true ) || bRepairShopSwitch == true ) 
			{
				StringCchCopy(anicode, 256, TEXT("SOP")); // ���λ��� ����
			}
			else 
			{
				StringCchCopy(anicode, 256, TEXT("SLEC"));
			}
		}
		else
		{
			StringCchCopy(anicode, 256, TEXT("SLEC"));
		}		
		break;
	case ANITYPE_FARM:
		StringCchCopy(anicode, 256, TEXT("FAM"));
		break;
	case ANITYPE_MINE:
		StringCchCopy(anicode, 256, TEXT("MIN"));
		break;
	case ANITYPE_SITDOWN:

		if( GetKind() == KIND_HERO3 ||
			GetKind() == KIND_HEROINE3 ||
			GetKind() == KIND_HERO4 ||
			GetKind() == KIND_HEROINE4 )
		{        
			if( bLevelUpDrawSwitch == true )
			{
				StringCchCopy(anicode, 256, TEXT("SPC")); // ������ ����
			}			
			else if( (siPersonalMakeItemType == MAKE_ANI_ARMOR) || (siPersonalMakeItemType == ENCHANT_ANI_WEPONE)|| (siPersonalMakeItemType == ENCHANT_ANI_ARMOR) )
			{			
				StringCchCopy(anicode, 256, TEXT("MAM")); // ���� ,�� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_HEAL)
			{
				StringCchCopy(anicode, 256, TEXT("MHE")); // ���� ��������
			}
			else if( siPersonalMakeItemType == MAKE_ANI_FOOD) 
			{
				StringCchCopy(anicode, 256, TEXT("MFO")); // ���� ��������
			}
			else if ( (bPersonalShopSwitch == true && bPremiumPersonalShop == true) || ( bIsElectionStump == true ) || bRepairShopSwitch == true ) 
			{
				StringCchCopy(anicode, 256, TEXT("SOP")); // ���λ��� ����
			}
			else
			{
				StringCchCopy(anicode, 256, TEXT("SIT"));
			}
		}
		else
		{
			StringCchCopy(anicode, 256, TEXT("SIT"));
		}		
		break;

	default:
		MsgBox(TEXT("mb98usdj"), TEXT("fk8jf:%d"), Animation);
		break;
	}

	//----------------------------------
	// ���� Ÿ�� �ִ� �������� Ȯ���Ѵ�. 
	//----------------------------------
	//KHY - 1001 - ��µ��� 5���� ���� ����.
	if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == false)
	{
		StringCchCat(anicode, 256, TEXT("U"));
	}
	else
	{
		StringCchCat(anicode, 256, TEXT("R"));
	}
	
	if(		_tcscmp(anicode, TEXT("1HSU")) == 0)return HSU1;
	else if(_tcscmp(anicode, TEXT("2HTU")) == 0)return HTU2;
	else if(_tcscmp(anicode, TEXT("2HBU")) == 0)return HBU2;
	else if(_tcscmp(anicode, TEXT("2HGU")) == 0)return HGU2;
	else if(_tcscmp(anicode, TEXT("WT1U")) == 0)return WT1U;
	else if(_tcscmp(anicode, TEXT("WT2U")) == 0)return WT2U;
	else if(_tcscmp(anicode, TEXT("DTHU")) == 0)return DTHU;
	else if(_tcscmp(anicode, TEXT("SPCU")) == 0)return SPCU;
	else if(_tcscmp(anicode, TEXT("MOVU")) == 0)return MOVU;
	else if(_tcscmp(anicode, TEXT("HITU")) == 0)return HITU;
	else if(_tcscmp(anicode, TEXT("1HSR")) == 0)return HSR1;
	else if(_tcscmp(anicode, TEXT("2HTR")) == 0)return HTR2;
	else if(_tcscmp(anicode, TEXT("2HBR")) == 0)return HBR2;
	else if(_tcscmp(anicode, TEXT("2HGR")) == 0)return HGR2;
	else if(_tcscmp(anicode, TEXT("SPCR")) == 0)return SPCR;
	else if(_tcscmp(anicode, TEXT("SPCU")) == 0)return SPCU;
	else if(_tcscmp(anicode, TEXT("WT1R")) == 0)
	{
		return WT1R;
	}
	else if(_tcscmp(anicode, TEXT("WT2R")) == 0)return WT2R;
	else if(_tcscmp(anicode, TEXT("DTHR")) == 0)return DTHR;
	else if(_tcscmp(anicode, TEXT("SPCR")) == 0)return SPCR;
	else if(_tcscmp(anicode, TEXT("MOVR")) == 0)return MOVR;
	else if(_tcscmp(anicode, TEXT("HITR")) == 0)return HITR;
	else if(_tcscmp(anicode, TEXT("FSHU")) == 0 || _tcscmp(anicode, TEXT("FSHR")) == 0 )
	{
		return FSHU;
	}
	else if(_tcscmp(anicode, TEXT("SLEPU")) == 0 || _tcscmp(anicode, TEXT("SLEPR")) == 0 )
	{
		return SLEP;
	}
	else if(_tcscmp(anicode, TEXT("SLECU")) == 0 || _tcscmp(anicode, TEXT("SLECR")) == 0 )
	{
		return SLEC;
	}
	else if(_tcscmp(anicode, TEXT("FAMU")) == 0 || _tcscmp(anicode, TEXT("FAMR")) == 0 )
	{
		return FAMU;
	}
	else if(_tcscmp(anicode, TEXT("MINU")) == 0 || _tcscmp(anicode, TEXT("MINR")) == 0 )
	{
		return MINU;
	}
	else if(_tcscmp(anicode, TEXT("SITU")) == 0 || _tcscmp(anicode, TEXT("SITR")) == 0 )
	{
		return SITU;
	}
	else if(_tcscmp(anicode, TEXT("MAMU")) == 0 ||  _tcscmp(anicode, TEXT("MAMR")) == 0  )
	{
		return MAMU;
	}
	else if(_tcscmp(anicode, TEXT("MHEU")) == 0 ||  _tcscmp(anicode, TEXT("MHER")) == 0  )
	{
		return MHEU;
	}
	else if(_tcscmp(anicode, TEXT("MFOU")) == 0 ||  _tcscmp(anicode, TEXT("MFOR")) == 0  )
	{
		return MFOU ;
	}
	else if(_tcscmp(anicode, TEXT("SOPU")) == 0 ||  _tcscmp(anicode, TEXT("SOPR")) == 0  )
	{
		return SOPU ;
	}			
	else
	{
		MsgBox(TEXT("fdih2d"), TEXT("fd9uj4:[%s]"), anicode);
	}

	return 0;
}

void cltCharCommon::SetAnimation(SI32 ani)
{
	SI32 delay, instant;
	Animation		= ani;
	bool bHero = (pclKI != NULL && GetKindInfo(true)->IsAtb(ATB_HERO));

	// by LEEKH 2007.12.24 - TRANSFORM
	if( clTransFormInfo.bTransFormMode == true )
	{
		// ������ ���... ������ ĳ������ ����� ���ϸ��̼��� ó���Ѵ�.- ������...
		if( pclClient->GameMode == GAMEMODE_SERVER )
		{
			if( Animation == ANITYPE_ATTACK || Animation == ANITYPE_ATTACK1 || Animation == ANITYPE_ATTACK2 )
			{
				bHero = true;
			}
		}
	}

	if( bHero )
	{
		HeroAnimation	= CalcHeroAnimation();
		
		delay			= pclCharDraw->GetDelayTime(HeroAnimation, GetKind());
		if(Animation == ANITYPE_ATTACK || Animation == ANITYPE_ATTACK1 
		|| Animation == ANITYPE_ATTACK1 || Animation == ANITYPE_MAGIC)
		{
			//KHY - 1001 - ��µ��� 5���� ���� ����.
			//if(pclCI->clHorseInfo.siCurrentHorseIndex >=0)
			if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE)				
			{
				//instant = min(pclCharDraw->GetFrameNum(HeroAnimation)-1, 3);
				//cyj �������� ���ݽ� instant �� �ʵ��� ����
				if (GetAttackType() == ATTACKTYPE_STAFF || GetAttackType() == ATTACKTYPE_STAFF1 ||
					GetAttackType() == ATTACKTYPE_STAFF2 || GetAttackType() == ATTACKTYPE_BOW || 
					GetAttackType() == ATTACKTYPE_BOW1 || GetAttackType() == ATTACKTYPE_NEW_BOW1 )
				{
					instant = min(pclCharDraw->GetFrameNum(HeroAnimation)-1, 6);
				}
				else
				{
					instant = min(pclCharDraw->GetFrameNum(HeroAnimation)-1, 3);
				}
			}
			else
			{
				instant = min(pclCharDraw->GetFrameNum(HeroAnimation)-1, 4);
			}
		}
		else
		{
			instant	= pclCharDraw->GetFrameNum(HeroAnimation) - 1 ;
		}
	}
	else
	{
		HeroAnimation	= 0;
		
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 aniref		= 0;
		
		if(selectRealAnyType( ))
		{
			if(Animation == ANITYPE_SLEEP)
			{
				HeroAnimation	= SLEP;
			}
			else if(Animation == ANITYPE_SLEEPCAT)
			{
				HeroAnimation	= SLEC;
			}
			
			aniref			= pclKI->clCharAniManager.FindRefFromAnitype(Animation);
			if(aniref == -1)
			{
				MsgBox(TEXT("SetAnimation(SI32 ani, SI32 *panistep)"), TEXT("kind:%d ani:%d"), GetKind(), Animation);
				return ;
			}

			// by LEEKH 2007.12.24 - TRANSFORM
			delay			= pclKI->clCharAniManager.GetDelay(aniref);
			instant			= pclKI->clCharAniManager.GetInstant(aniref);
		}
		else
		{
			aniref			= GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(Animation);
			if(aniref == -1)
			{
				MsgBox(TEXT("SetAnimation(SI32 ani, SI32 *panistep)"), TEXT("kind:%d ani:%d"), GetKind(), Animation);
				return ;
			}

			// by LEEKH 2007.12.24 - TRANSFORM
			delay			= GetKindInfo(true)->clCharAniManager.GetDelay(aniref);
			instant			= GetKindInfo(true)->clCharAniManager.GetInstant(aniref);
		}
	}

	clAniInfo.SetAniDelay(delay);
	clAniInfo.SetAniInstant( instant );

}


BOOL cltCharCommon::DoAnimationDelay(BOOL atb)
{
	SI32	delay;
	SI32	framenumber;

	// by LEEKH 2007.12.24 - TRANSFORM
	if( (pclKI != NULL && GetKindInfo(true)->IsAtb(ATB_HERO)) )
	{
		framenumber = pclCharDraw->GetFrameNum(HeroAnimation);
	}
	else
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 aniref		= 0;

		if(selectRealAnyType( ))
		{
			aniref	= pclKI->clCharAniManager.FindRefFromAnitype(Animation);
			framenumber = pclKI->clCharAniManager.GetFrameNum(aniref);
		}
		else
		{
			aniref	= GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(Animation);
			framenumber = GetKindInfo(true)->clCharAniManager.GetFrameNum(aniref);
		}
	}
	if(framenumber == 0)
	{
		if (pclClient->GameMode == GAMEMODE_SERVER)
			pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("GetFrameNum() Kind:%d ani:%d"), GetKind(), Animation);
	}


	//===============================================
	// ������
	//===============================================
	if ( ((cltClient*)pclClient)->GetFrameDelay() == 25 )
	{
		delay		= clAniInfo.siAniDelay * 2;
	}
	else
	{
		delay		= clAniInfo.siAniDelay;
	}

	clAniInfo.IncreaseAniDelayor(1);

	if(clAniInfo.GetAniDelayor() >= delay)
	{
		clAniInfo.SetAniDelayor(0);


		// �ѹ��� �ִϸ��̼� �ϰ� �Ǿ� �ִ� ���, 
		// �������� ������ ���ٸ�, 
		// ���̻� �ִϸ��̼��� ������ �ʴ´�. 
		if((atb & ANIATB_ONLYONCE) && clAniInfo.GetAniStep() == (framenumber - 1))
		{
			return TRUE;
		}
		else
		{

			clAniInfo.IncreaseAniStep(1);

			if(framenumber == 0)
			{
				clAniInfo.SetAniStep(0);
			}
			else
			{
				clAniInfo.SetAniStep(clAniInfo.GetAniStep() % framenumber );
			}

			// ������̸� Ÿ�� �ִ� ���
			// ���2, �̵� �ÿ��� ĳ������ ù��°�� ������ �����Ӹ� �׸���.
			if( pclCI->clHorseInfo.GetCurrentHorseRIDEHORSE() && (HeroAnimation == WT2R || HeroAnimation == MOVR) )
			{
				cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
				SI32 siHorseUnique = pclHorse->siHorseUnique;

				if( pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM_HORSE) )
				{
					stBuf* pclBuf = pclCI->clBI.clBufInfo.GetBuf(BUF_TRANSFORM_HORSE);
					if( pclBuf )		siHorseUnique = pclBuf->siPercent;
				}

				if(siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
				{
					if( clAniInfo.GetAniStep() > 0 )
					{
						clAniInfo.SetAniStep(max(framenumber - 1, 0));
					}
				}
			}
		}

		bUpdateAnimationSwitch = true;

		return TRUE;
	}

	//===============================================



	//===============================================
	// ��Ʋ ���
	//===============================================
	//delay		= clAniInfo.siAniDelay;

	//// �ѹ��� �ִϸ��̼� �ϰ� �Ǿ� �ִ� ���, 
	//// �������� ������ ���ٸ�, 
	//// ���̻� �ִϸ��̼��� ������ �ʴ´�. 
	//if((atb & ANIATB_ONLYONCE) )
	//{
	//	if(clAniInfo.GetAniStep() >= (framenumber - 1))
	//	{
	//		return TRUE;
	//	}
	//}


	//DWORD clockdelay	= TABS(pclClient->CurrentClock - dwAniStartClock);
	//DWORD delayperframe = max(50, delay*50);
	//SI32 frame			= clockdelay / delayperframe;
	//if(frame >= framenumber)
	//{
	//	frame = 0;
	//	clAniInfo.siRetryNum++;	// �ݺ�ȸ���� ���Ѵ�. 
	//}

	//SI32 frame2			= min(framenumber-1, frame);

	// �߿������ �ݵ�� ������ �Ѵ�. 
	// ������ anistep�� ���Ѵ�. 
	//SI32 oldanistep = clAniInfo.GetAniStep();
	//if(oldanistep < clAniInfo.siAniInstant && clAniInfo.siAniInstant < frame2)
	//{
	//	frame2 = clAniInfo.siAniInstant;
	//}

	//clAniInfo.SetAniStep( frame2 );
	//bUpdateAnimationSwitch = true;


	//if(frame2 == 0)
	//{
	//	// �ִϸ��̼� ���� �ð��� Ȯ���Ѵ�. 
	//	if(clockdelay > delayperframe)
	//	{
	//		dwAniStartClock = pclClient->CurrentClock;
	//	}
	//}

	//if(framenumber == 0)
	//{
	//	clAniInfo.SetAniStep(0);
	//}
	//
	//// ANIATB_ONLYONCE�Ӽ��϶� true�� �����ϴ� ���� �ִϰ� �Ϸ�Ǿ��ٴ� �ǹ��̴�. 
	//if((atb & ANIATB_ONLYONCE) == 0)
	//{
	//	return TRUE;
	//}

	//===============================================

	return FALSE;
}


// ĳ������ ������ ��ȯ�Ѵ�. 
BOOL cltCharCommon::ChangeDirectionNormal(SHORT destdir)
{
	siDirection=destdir;
	TempDirection=destdir;

	bUpdateAnimationSwitch = true;

	return TRUE;
}

// ĳ������ ������ ��ȯ�Ѵ�. 
BOOL cltCharCommon::ChangeDirection16(SHORT destdir)
{
	SHORT index=0;
	SHORT source_index=0;
	SHORT rotate_dir;
	SHORT dir[]={SOUTH, SSW, SOUTH|WEST, WWS, WEST, WWN, NORTH|WEST, NNW, 
		         NORTH, NNE, NORTH|EAST, EEN, EAST, EES, SOUTH|EAST, SSE };

	if(TempDirection==destdir)
	{
		bBusySwitch  = false;
		return TRUE;
	}


	bBusySwitch = true;

	// ������ �����Ѵ�. 
	index=0;
	bool roop = true;
	while(roop)
	{
		if(dir[index]==TempDirection)
		{
			source_index=index;
			break;
		}
		index++;
	}

	index=0;
	while(roop)
	{
		if(dir[(source_index+index)%16]==destdir)
		{
			break;
		}
		index++;
	}

	// �ݽð� �������� ȸ�� 
	if(index>=8)
	{
        rotate_dir=FALSE;
	}
	else 
	{
		rotate_dir=TRUE;
	}


	if(rotate_dir==TRUE)
	{
	  TempDirection=dir[(source_index+1)%16];
	}
	else 
	{
		if(source_index-1==-1)
	       TempDirection=dir[15];
		else 
			TempDirection=dir[source_index-1];
	}
	
	if(TempDirection<1000)
	{
		siDirection = TempDirection;
	}
	else
	{

	}

	bUpdateAnimationSwitch = true;

	return FALSE;
}

BOOL cltCharCommon::ChangeDirection(SHORT destdir)
{
   return ChangeDirectionNormal(destdir);
}

//----------------------------------
// ���� �Լ�. 
//----------------------------------
void cltCharCommon::SendNetMsg(sPacketHeader* pmsg)
{
	if(IsValidConnection()== false)return ;

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		cltServer* pclserver = (cltServer*)pclClient;
		cltMsg* pclMsg = (cltMsg*)pmsg;
		if(pclMsg->GetTotalSize() > sizeof(cltMsg))
		{
			MsgBox(TEXT("SendNetMsg Overflow"), TEXT("cmd:%d size:%d"), pclMsg->clHead.usCmd, pclMsg->GetTotalSize());
			SI32 a = 0;
		}

		UI32 uiCompSize = 0;
		EnterCriticalSection( &pclserver->m_cs_SendNetMsg );
		{
			if( pclserver->m_uiCompressMinSize > 0 && pmsg->usSize >= pclserver->m_uiCompressMinSize )		// ������ ����Ʈ�̻� �����Ѵ�.
			{
				BYTE* pMsg = (BYTE*)pmsg;
				BYTE* pbufComp = pclserver->m_bufComp;
				memcpy(pbufComp, pmsg, sizeof(sPacketHeader));
				uiCompSize = GetSession()->Compress( &pMsg[sizeof(sPacketHeader)],		pmsg->usSize - sizeof(sPacketHeader),			// SOURCE
													 &pbufComp[sizeof(sPacketHeader)],	sizeof(pclserver->m_bufComp) - sizeof(sPacketHeader) );		// COMPRESS BUFFER
				if( uiCompSize > 0 )	
				{
					sPacketHeader* pCompMsg = (sPacketHeader*)pbufComp;
					pCompMsg->usSize = sizeof(sPacketHeader) + uiCompSize;

					// ��Ŷ ������ �ֻ��� ��Ʈ�� ��Ʈ�ϸ�, ��Ŷ�� ����� ���̴�.
					pCompMsg->usCmd |= 0x8000;

					GetSendBuffer()->Write( pbufComp );		// ����� �����͸� ������.
				}
				else
				{
					GetSendBuffer()->Write( (BYTE*)pmsg );	// // ���࿡ �����ؼ� ������ ������ - �ӵ� ����ȭ
				}
			}
			else
			{
				GetSendBuffer()->Write( (BYTE*)pmsg );		// // ���࿡ �����ؼ� ������ ������ - �ӵ� ����ȭ
			}
		}
		LeaveCriticalSection( &pclserver->m_cs_SendNetMsg );

		// Ŭ���̾�Ʈ�� ������ �޽����� �����Ѵ�. 
		((cltServer*)pclClient)->pclStatisticsManager->clDailyStatistics.siSendToClientMsgNumber++;

		//NStatistics_Add("NetMessage", pmsg->usCmd, "SendCount", 1);
		//NStatistics_Add("NetMessage", pmsg->usCmd, "SendSize", pclMsg->clHead.usSize);

#ifdef WRITE_NETMESSAGE_STATISTICS
		//-------------------------------------------
		// ���� �޽��� ������ ī��Ʈ �Ѵ�.
		//-------------------------------------------
		if( ((cltServer*)pclClient)->pclGameProtocol != NULL &&
			pmsg->usCmd >= 0 && pmsg->usCmd < GAMEMSG_END )
		{
			((cltServer*)pclClient)->pclGameProtocol->m_clNetMessageStatistics[pmsg->usCmd].m_siSend++;
			((cltServer*)pclClient)->pclGameProtocol->m_clNetMessageStatistics[pmsg->usCmd].m_siSize = pclMsg->clHead.usSize;
		}
#endif
	}
	else
	{
		cltClient* pclclient = (cltClient*)pclClient;
		pclclient->SendMsgToServer(pmsg);
	}
}


// ������ �Ǿ� �ִ� Person���� ����. 
bool cltCharCommon::IsValidConnection()
{
	ITZSession*	pSession = GetSession();

	if(pSession == NULL)return false;

	if(pSession->GetState() != SESSION_STATE_ESTABLISHED)return false;

	return true;


}


//---------------------------------
// �ǰ�
//---------------------------------

// �ǰ� ������ ���Ѵ�. 
bool cltCharCommon::BeHitOperation()
{
	if(bHitSwitch == false)return false;

	// by LEEKH 2007.12.24 - TRANSFORM
	SI32 aniref = GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(ANITYPE_HIT);
	if(aniref < 0)return false;


	SetAnimation(ANITYPE_HIT);
	bUpdateAnimationSwitch	=	true;

	if(DoAnimationDelay() == TRUE)
	{
		SetHitSwitch(false);

		clMoveInfo.dwLastMoveClock	= pclClient->CurrentClock;
		clMoveInfo.siSparePercent	= 0;
	}

	if( IsPC( GetID() ) )
	{
		switch( GetCurrentOrder() )
		{
		case ORDER_MOVE:
		case ORDER_ATTACK:
		case ORDER_ATTACK_SUMMON:
		case ORDER_MAGIC:
		case ORDER_MAGIC_SUMMON:
		case ORDER_MOVE_SUMMON:
		case ORDER_ATTACK_CLIENT:
			break;
		default:
			OrderForceStop( GetID(), BY_PLAYER );
			break;
		}
	}

	return true;
}

//---------------------------------
// ���� 
//---------------------------------
SI32 cltCharCommon::GetTotalWeight()
{
	return pclCI->clCharItem.GetTotalWeight(pclClient->pclItemManager);
}


//---------------------------------
// ���� �ؽ�Ʈ 
//---------------------------------

// ĳ������ ������ �ؽ�Ʈ�� �����ؼ� �޴´�.
bool cltCharCommon::GetCharInfoText(TCHAR* buffer)
{
	TCHAR* pmapname = pclClient->pclMapManager->GetMapName(pclMap->siMapIndex);

	if(pmapname == false)return false;

	TCHAR* pText = GetTxtFromMgr(2804);
	StringCchPrintf(buffer, 1024, pText,
		
		GetName(), GetID(), 
		pclCI->GetPersonID(), 
		GetKind(), 
		pmapname,
		GetX(), GetY(), 
		GetCurrentOrder(),
		siSubOrder,
		GetLife(), clPB.GetMaxLife(), GetMana(), clPB.GetMaxMana(), 
		GetAttackType(), GetAttackRange(), clPB.GetTotalMoveSpeed(), 
		clPB.GetTotalMinDamage(), clPB.GetTotalMaxDamage(), clPB.GetTotalAC(), 
		pclCI->clIP.GetLevel(), pclCI->clIP.GetCurExp(), 
		GetStr(), GetDex(), GetMag(), GetVit() ,GetWis());

	return true;

}

//------------------------------------------------
// ��ȯ�������� ��ȯ���� �����Ҽ� �ִ��� �˻��Ѵ�.
//-----------------------------------------------
bool cltCharCommon::CanUseSummonDoll(SI32 itemunique, SI32 *summonkind, SI32 *summonindex, SI32 *preturn)
{
	cltItemManagerCommon* pclItemManager;
	pclItemManager = pclClient->pclItemManager;
	
	// ��ȯ�� �ʱ�ȭ 
	*summonkind		= 0;
	*summonindex	= -1;

	// ��ȯ�� �ʱ�ȭ
	*preturn = CANUSEITEM_ERROR_INVALID;

	// DB�� ��� ���̹Ƿ� ��ȯ���� ������ �� ����.
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if( ((cltCharServer*)this)->bIsNowAddSummonThroughDB )		return false;
	}

	//---------------------------------------------
	// [����] ��Ʋ�ξ�ʿ� �ִٸ� ��� �Ұ�
	//---------------------------------------------
	if( (MAPINDEX_BATTLEROYAL_MABRIUM == GetMapIndex()) 
		|| ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == GetMapIndex()))
	{
		*preturn	= CANUSEITEM_ERROR_BATTLEROYALMAP;
		return false;
	}

	//=========================================================================
	// Level�� �Ǿ�� ����Ҽ� �ִ�.
	//=========================================================================
	if(pclCI->clIP.GetLevel() < LIMIT_LEVEL_HATCH_DOLL )
	{
		*preturn	= CANUSEITEM_ERROR_NEEDLEVEL;
		return false;
	}
	
	//=========================================================================
	// DOLL �������� �ش� Monter Kind�� ��ȯ�������� ���캻��.
	//=========================================================================
	pclItemManager->GetItemDollUnique(itemunique);
	SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);
	if(ref <= 0)
	{
		*preturn	= CANUSEITEM_ERROR_NOUSEABLE;
		return false;
	}
	
	SI32 kind = pclItemManager->pclItemInfo[ref]->siMakeKind;
	if(kind < 0 || kind >= MAX_KINDINFO_NUMBER)
	{
		*preturn	= CANUSEITEM_ERROR_NOUSEABLE;
		return false;
	}
	if(pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_DEFAULTSUMMON) == FALSE)
	{
		*preturn	= CANUSEITEM_ERROR_NOUSEABLE;
		return false;
	}

	// �����ڵ带 ���� ��ȯ�����̶�� �ڽ��� �����ڵ�� �¾ƾ� ��ȯ�� �� �ִ�.
	if ( pclClient->pclKindInfoSet->pclKI[kind]->siDefaultCountry > 0 &&
		 pclKI->siDefaultCountry != pclClient->pclKindInfoSet->pclKI[kind]->siDefaultCountry )
	{
		*preturn	= CANUSEITEM_ERROR_NOCOUNTRY;
		return false;
	}

	//=========================================================================
	// ���� ��ȯ���� �ۿ� ���� �ִ��� ��ȯ���� ���� ������ �ִ��� �˻��Ѵ�.
	//=========================================================================
	BOOL bIsEmpty = FALSE;
	if ( pclClient->IsCountrySwitch(Switch_SummonHouse) )
	{
		if ( FALSE == pclCI->clSummonInfo.GetEmptySummonIndexInHouse(summonindex) )
		{
			*preturn = CANUSEITEM_ERROR_CANTOVERSLOT;
			return false;
		}

		// Ƽ���� ������ ������ �� ����.
		if( *summonindex > 2 )
		{
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				cltServer* pclserver = (cltServer*)pclClient;
				if( pclCI->clBI.bSummonExtraSlot == false || pclCI->clBI.siSummonExtraSlotDate <= pclserver->pclTime->clDate.GetDateVary() )
				{
					*preturn = CANUSEITEM_ERROR_TICKETOVERDATE;
					*summonindex = -1;
					pclserver->SendServerResponseMsg(0, SRVAL_SUMMON_FAIL_LOTSOFDATE,  0, 0, GetCharUnique());// ���Ⱓ�� ���� �Ǿ����ϴ�.
					return false;
				}
			}
			else if(pclClient->GameMode == GAMEMODE_CLIENT)
			{
				cltClient* pclclient = (cltClient*)pclClient;
				if( pclCI->clBI.bSummonExtraSlot == false || pclCI->clBI.siSummonExtraSlotDate <= pclclient->clClientDate.GetDateVary() )
				{
					*preturn = CANUSEITEM_ERROR_TICKETOVERDATE;
					*summonindex = -1;
					return false;
				}
			}
		}
	}
	else
	{
		if ( FALSE == pclCI->clSummonInfo.GetEmptySummonIndex(summonindex) )
		{
			*preturn = CANUSEITEM_ERROR_SUMMON_NOMORE;
			return false;
		}
	}

	// ��ȯ���� Kind�� �����ش�.
	*summonkind = kind;
	return true;
}

bool cltCharCommon::CanUseSoulGuardItem( SI32 siItemUnique, SI16* psiSoulGuardKind, SI16* psiLife, SI16* psiFellowShip, SI32* pReturn )
{
	if ( (NULL == psiSoulGuardKind) || (NULL == psiLife) || (NULL == psiFellowShip) || (NULL == pReturn) )
	{
		return false;
	}

	// ���ϰ� �ʱ�ȭ
	*psiSoulGuardKind	= 0;
	*psiLife			= 0;
	*psiFellowShip		= 0;
	*pReturn			= CANUSEITEM_ERROR_INVALID;

	if ( false == pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		return false;
	}

	// DB�� ��� ���̹Ƿ� ��ȣ������ ��ȯ���� ����
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if( ((cltCharServer*)this)->bIsNowAddSoulGuardThroughDB )
		{
			return false;
		}
	}

	//---------------------------------------------
	// [����] ��Ʋ�ξ�ʿ� �ִٸ� ��� �Ұ�
	//---------------------------------------------
	if( (MAPINDEX_BATTLEROYAL_MABRIUM == GetMapIndex()) 
		|| ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == GetMapIndex()))
	{
		*pReturn	= CANUSEITEM_ERROR_BATTLEROYALMAP;
		return false;
	}


	if ( NULL == pclSoulGuard )
	{
		return false;
	}

	// �̹� ��ȣ������ ���� �ִٸ�
	if ( FALSE == pclSoulGuard->IsEmpty() )
	{
		// ��ȣ���� �����ҿ� ������ �ִ°�?
		SI32 siMaxHaveHouseNum = pclSoulGuard->m_clHouse.GetMaxHaveHouseNum();
		if ( 0 < siMaxHaveHouseNum )
		{
			SI32 siHaveCount	= 0;
			SI32 siRealHaveNum	= (siMaxHaveHouseNum>MAX_SOULGUARD_HOUSE_NUM?MAX_SOULGUARD_HOUSE_NUM:siMaxHaveHouseNum);
			for ( SI32 siIndex=0; siIndex<siRealHaveNum; siIndex++ )
			{
				if ( 0 < pclSoulGuard->m_clHouse.GetSoulGuardIDFromIndex(siIndex) )
				{
					siHaveCount++;
				}
			}

			// ���� �ִ°Ͱ� ������ �ִ� ������ ������ ���̻� ����������
			if ( siRealHaveNum <= siHaveCount )
			{
				*pReturn = CANUSEITEM_ERROR_SOULGUARD_NOMORE;
				return false;
			}

		}
		else 
		{
			*pReturn = CANUSEITEM_ERROR_SOULGUARD_NOMORE;
			return false;
		}
	}

	if ( NULL == pclClient->m_pclSoulGuardMgr )
	{
		return false;
	}

	// ������ ����ũ�� �ش��ϴ� ��ȣ���� ��ȯ ������ �����´�
	cltSoulGuardInfoUnit* pclGetUnit = pclClient->m_pclSoulGuardMgr->GetSoulGuardUnitFromItemUnique( siItemUnique );
	if ( NULL == pclGetUnit )
	{
		return false;
	}

	*psiSoulGuardKind	= pclGetUnit->GetKindUnique();
	*psiLife			= pclGetUnit->GetLife();
	*psiFellowShip		= pclGetUnit->GetFellowShip();

	return true;		
}

//------------------------------------------------
// ������ ���� 
//-----------------------------------------------
// �������� ����Ҽ� �ִ� ������ Ȯ���Ѵ�. 
bool cltCharCommon::CanUseItem(cltItem clitem, SI32* preturn, SI32 siPos)
{

	cltItemManagerCommon* pclItemManager;
	pclItemManager = pclClient->pclItemManager;
	
	//=================================
	// return �ʱ�ȭ
	//=================================
	*preturn = CANUSEITEM_ERROR_INVALID;

	// �������� ��ȿ���� ������ ����� �� ����. 
	if(clitem.siUnique == 0)
	{
		return false;
	}
	
	// �������� ����ϴ� �ڰ� �׾����� ����� �� ����. 
	if(pclCI->clIP.GetLife() <= 0)	// �ູ���� ��Ȱ���� �ƴϰ� �׾����� 
	{
		*preturn	= CANUSEITEM_ERROR_DEADCHAR;
		return false;
	}

	SI32 itemunique = clitem.siUnique;
	SI32 ref		= pclItemManager->FindItemRefFromUnique(itemunique);
	if(ref <= 0)
	{
		*preturn	= CANUSEITEM_ERROR_INVALID;
		return false;
	}

	SI32 itemtype	= pclItemManager->pclItemInfo[ref]->siType;
	SI64 itematb	= pclItemManager->pclItemTypeInfo[itemtype]->siAtb;
	
	// ���Ӽ��� �ƴϸ� ����.
	if(pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CANUSEITEM) == false)	// ��ι�. 
	{
		*preturn	= CANUSEITEM_ERROR_NOUSEABLE;
		return false;
	}
	
	switch(itemtype)
	{
	case ITEMTYPE_DOLL:
		{

			SI32 siKind = 0, siSummonIndex = 0;
			// �ش� ��ȯ�������� ��ȯ���� �����Ҽ� �ִ����� �˻��Ѵ�.
			return CanUseSummonDoll(itemunique, &siKind, &siSummonIndex, preturn);
		
		}
		break;

	case ITEMTYPE_FOOD:
	case ITEMTYPE_SEAFOOD:
		{
			// ���� ������
			if( itemunique == ITEMUNIQUE(3934) || itemunique == ITEMUNIQUE(3931) 
				|| itemunique == ITEMUNIQUE(3930) || itemunique == ITEMUNIQUE(3929)
				|| itemunique == ITEMUNIQUE(3932)|| itemunique == ITEMUNIQUE(3933))
			{
				//---------------------------------------------
				// [����] ��Ʋ�ξ�ʿ� �ִٸ� ��� �Ұ�
				//---------------------------------------------
				if( (MAPINDEX_BATTLEROYAL_MABRIUM == GetMapIndex()) 
					|| ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == GetMapIndex()))
				{
					*preturn	= CANUSEITEM_ERROR_BATTLEROYALMAP;
					return false;
				}
			}

			//cyj �ػ����� ������̸� ���Ұ�
			if( itemunique == ITEMUNIQUE(3860) || itemunique == ITEMUNIQUE(3865) || itemunique == ITEMUNIQUE(3870) )
			{
				if ( pclCI->clBI.bPremiumService )
				{
					*preturn = CANUSEITEM_ERROR_NOMOREUSE;
					return false ;
				}
			}
			//cyj �������� ����, ���� ���� �̸� ���Ұ�
			else if( itemunique == ITEMUNIQUE(3875) || itemunique == ITEMUNIQUE(3880))
			{
				if ( pclCI->clBI.bMakeItemService )
				{
					*preturn = CANUSEITEM_ERROR_NOMOREUSE;
					return false ;
				} // if ( pclCI->clBI.bMakeItemService )

				if ( pclCI->clBI.clBufInfo.CanUseBuf(BUF_MAKEEXPUP) == false )
				{
					*preturn = CANUSEITEM_ERROR_NOMOREUSE;
					return false ;
				}
			}
			// ������ ����
			else if( itemunique == ITEMUNIQUE(3903) )
			{
				if ( pclCI->clBI.bMakeItemService )
				{
					*preturn = CANUSEITEM_ERROR_NOMOREUSE;
					return false ;
				}

				if ( pclCI->clBI.clBufInfo.CanUseBuf(BUF_NOTBONUSTIME_MAKEEXPUP) == false )
				{
					*preturn = CANUSEITEM_ERROR_NOMOREUSE;
					return false ;
				}
			}
			// ��ȣ���� ģȭ�� �����
			else if( ITEMUNIQUE(3928) == itemunique )
			{
				if ( NULL == pclSoulGuard )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}

				if ( FALSE == IsSoulGuardStatusOut() )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}

				if ( Const_Max_SoulGuard_FellowShip <= pclSoulGuard->GetFellowShip() )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}
			}
			// ���������ε� ���� ������ ����. 
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 1)
			{
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

				// ���� ������ ����� �� ����. 
				if(pclhorse == NULL || pclhorse->siHorseUnique <= 0 || pclhorse->siHorseUnique == 9 )
				{
					*preturn	= CANUSEITEM_ERROR_NOHORSE;
					return false;
				}
			}
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 2)
			{
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

				// ���� ������ ����� �� ����. 
				if( pclhorse == NULL || pclhorse->siHorseUnique != 9 )
				{
					*preturn	= CANUSEITEM_ERROR_NOHORSE;
					return false;
				}
			}
			else if(pclItemManager->pclItemInfo[ref]->siHorseFoodSwitch == 3)
			{
				cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

				// ���� ������ ����� �� ����. 
				if(pclhorse == NULL || pclhorse->siHorseUnique <= 0 )
				{
					*preturn	= CANUSEITEM_ERROR_NOHORSE;
					return false;
				}
			}
			else if ( pclItemManager->pclItemInfo[ref]->bFishingBaitSwitch == true )
			{
				// ��ó�� ������ ���� ���ٸ� ����� �� ����.				
				SI32 dir = 0;
				if(IsFishArea(GetX(), GetY(), &dir) == false)
				{
					*preturn	= CANUSEITEM_ERROR_NOPROPERFISHAREA;
					return false;
				}
			}			
			//-----------------------------------
			// ������� Ȯ��. 
			//-----------------------------------
			SI32 returnval = 0;
			if( pclItemManager->CheckCondition(itemunique, this, &returnval) == false )
			{
				*preturn	= CANUSEITEM_ERROR_NOTFITCOND;
				return false;
			}
		}
		break;
	case ITEMTYPE_ANIMAL:
		{
			//---------------------------------------------
			// [����] ��Ʋ�ξ�ʿ� �ִٸ� ��� �Ұ�
			//---------------------------------------------
			if( (MAPINDEX_BATTLEROYAL_MABRIUM == GetMapIndex()) 
				|| ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == GetMapIndex()))
			{
				*preturn	= CANUSEITEM_ERROR_BATTLEROYALMAP;
				return false;
			}

			//KHY - 1001 - ��µ��� 5���� ���� ����.
			//if(!(pclClient->IsWhereServiceArea(ConstSwitch_Can5Horse)))
			if(!(pclClient->IsCountrySwitch(Switch_Can5Horse)))
			{
				if(pclCI->clHorseInfo.HaveHorse()== true) //���� �Ѹ����� ������, ���̻� ����� ����. - �����뱹��������
					return false;
			}
			if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE)				
			{
				*preturn	= CANUSEITEM_ERROR_RIDEHORSEUSEITEM; 	// ���� �ö�ź ���¿��� �������� ����ߴ�.
				return false;
			}
			// �̹� ���� �ִ� ���¿����� ���Ұ�. 
			if(pclCI->clHorseInfo.FindEmptyHorseIndex() < 0)
			{
				//KHY - 1001 - ��µ��� 5���� ���� ����.
				*preturn	= CANUSEITEM_ERROR_OVERMAXHORSE; //�ִ밪 �̻��� ���� ����� ����.
				return false;
			}

			//-----------------------------------
			// ������� Ȯ��. 
			//-----------------------------------
			SI32 retunval = 0;
			if( pclItemManager->CheckCondition(itemunique, this, &retunval) == false )
			{
				*preturn	= CANUSEITEM_ERROR_NOTFITCOND;
				return false;
			}
		}
		break;
	case ITEMTYPE_TICKET:
	case ITEMTYPE_ETC:
		{			
			//	[��ȣ][����] ������̳븮��(8114) �� ��ȯ�˰� ���������� �����ġ ���� 
			// PCK : �������� ��ȯ�� 1,2, ������̳븮��, ����ã���̺�Ʈ ���� ��ȯ�˿� �ִ� NPC������ �Ѿ�� ������� ���ϵ��� ������(09.07.02)
			if ( (ITEMUNIQUE( 7860 ) == clitem.siUnique) || (ITEMUNIQUE( 7865 ) == clitem.siUnique) || (ITEMUNIQUE( 8114 ) == clitem.siUnique) || (ITEMUNIQUE( 13057 ) == clitem.siUnique) )	// ��ȯ��	
			{
				if ( ((GetMapIndex() == MAPINDEX_BEGINNER1) 
					|| (GetMapIndex() == MAPINDEX_PALACE) 
					|| (GetMapIndex() == MAPINDEX_MOFUMOFULAND)
					|| (GetMapIndex() == Const_MapIndex_PollMap)
					|| (GetMapIndex() == MAPINDEX_PRIVATEMARKETAREA)
					|| (GetMapIndex() == MAPINDEX_WEDDINGHALL)
					|| (GetMapIndex() == MAPINDEX_FESTIVAL)		// ���Һ� �̺�Ʈ �ʿ��� ��ȯ�� ������� ���ϵ��� ����.
					|| (GetMapIndex() == MAPINDEX_BATTLEROYAL_MABRIUM )
					|| (GetMapIndex() == MAPINDEX_BATTLEROYAL_GRANDMABRIUM ))
					|| (pclClient->pclCM->pclIDManager->GetNPCCharNumber() > MAX_NPC_NUMBER - 1000)
				)
				{
					*preturn	= CANUSEITEM_ERROR_NOUSE_THISPLACE;

					return false;
				}
			}
			// ������ �ø��� (��ȫ���), (��̲ɴٹ�)
			else if ( (ITEMUNIQUE(8119) == clitem.siUnique) || (ITEMUNIQUE(8120) == clitem.siUnique) )
			{
				if ( NULL == pclMarriage )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}
				
				// ��ȥ ���ߴٸ�
				if ( FALSE == pclMarriage->IsMarried() )
				{
					*preturn = CANUSEITEM_ERROR_NOTMARRIED;
					return false;
				}
				
				/*
				// �̹� �ְ� ���������
				if ( MARRIAGE_LOVE_LEVEL_10 <= pclMarriage->GetLoveLevel() )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}
				*/

				// ������ �����ʿ� ������ ���� �ʴٸ�
				if ( MARRIAGE_MATE_CONNECT_MODE_SAMEMAP != pclMarriage->GetConnectMode() )
				{
					*preturn = CANUSEITEM_ERROR_CANNOT_SAMEMAP;
					return false;
				}

				// �׳� ��� �϶��� �ð� �˻縦 �Ѵ�
				if ( ITEMUNIQUE(8119) == clitem.siUnique )
				{
					// ��� ���� �ð� üũ
					cltDate clCanUseTime;

					if ( GAMEMODE_CLIENT == pclClient->GameMode )		clCanUseTime.Set( &((cltClient*)pclClient)->clClientDate );
					else if ( GAMEMODE_SERVER == pclClient->GameMode )	clCanUseTime.Set( &((cltServer*)pclClient)->pclTime->clDate );

					// +10 �� ���� �ð����� 20��
					if ( (0 < pclMarriage->GetUseHourVary()) && ((pclMarriage->GetUseHourVary() +10) > clCanUseTime.GetHourVary()) )
					{
						*preturn = CANUSEITEM_ERROR_CAN_NOT_USETIME;
						return false;
					}
				}
			}
			else if( itemunique == ITEMUNIQUE(8122) )	//���Ŀ� ��Ű
			{
				if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
				{
					// ��� ���� �ð� üũ
					cltDate clCanUseTime;
					if ( GAMEMODE_CLIENT == pclClient->GameMode )		clCanUseTime.Set( &((cltClient*)pclClient)->clClientDate );
					else if ( GAMEMODE_SERVER == pclClient->GameMode )	clCanUseTime.Set( &((cltServer*)pclClient)->pclTime->clDate );
					// +10 �� ���� �ð����� 20��
					if( pclFamily != NULL )
					{
						SI32 siPersonID = pclCI->GetPersonID();
						if (siPersonID < 0) return false;
						SI32 siParent_PeronID_First = 0	;
						SI32 siParent_PeronID_Second = 0	;
						siParent_PeronID_First = pclFamily->GetParent_1()	;
						siParent_PeronID_Second = pclFamily->GetParent_2()	;
						if ( siParent_PeronID_First <= 0 ||  siParent_PeronID_Second <= 0 )	return	false	;
						SI32 siMyClass = 0	;
						siMyClass = pclFamily->GetMyClass_InFamily();
						if(siMyClass != MYCLASS_IN_FAMILY_CHILDREN)	return false;
                        SI32 siDatevary =	0	;
						siDatevary = pclFamily->GetMyCookie_Limitime( siPersonID )		;
						
						if ( 0 < siDatevary )
						{
							SI32 siHourvary = clCanUseTime.GetHourVary()	;
							if ( siDatevary >  siHourvary )
							{
								*preturn = CANUSEITEM_ERROR_CANNOT_USE_COOKIE_NO_OVER_TIME;
								return false;
							}							
						}
					}
					else
					{
						return false;
					}
				}				
			}
			// PCK : �Ҿ���� ���� ����Ʈ - �Ϻ� �������� ������� ���ϰ� �Ѵ�.
			else if(clitem.siUnique == ITEMUNIQUE(8115) || clitem.siUnique == ITEMUNIQUE(8116))
			{
				*preturn = CANUSEITEM_ERROR_NOUSEABLE;
				return false ;
			}
			// PCK : �������ž��� ����Ʈ ����ÿ��� ����Ҽ��ִ�.
			else if(clitem.siUnique == ITEMUNIQUE(8117) )
			{
				SI16 siMonsterKind = (SI16)pclClient->GetUniqueFromHash( TEXT("KIND_TRICERAMONS") );
				if ( 0 >= siMonsterKind )
				{
					*preturn = CANUSEITEM_ERROR_NOUSEABLE;
					return false ;
				}
				
				if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS, siMonsterKind) )
				{
					*preturn = CANUSEITEM_ERROR_NOUSEABLE;
					return false ;
				}
			}
			// [���] ���ݹ������ ����Ʈ ����ÿ��� ����Ҽ��ִ�
			else if(clitem.siUnique == ITEMUNIQUE(8118) )
			{
				SI16 siMonsterKind = (SI16)pclClient->GetUniqueFromHash( TEXT("KIND_BONETYRANNO") );
				if ( 0 >= siMonsterKind )
				{
					*preturn = CANUSEITEM_ERROR_NOUSEABLE;
					return false ;
				}

				if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMON, siMonsterKind) )
				{
					*preturn = CANUSEITEM_ERROR_NOUSEABLE;
					return false ;
				}
			}
			// PCK : ������ Ÿ�� ETC�� �̵�(09.06.17)
			else if(clitem.siUnique == ITEMUNIQUE(8121))
			{
				SI16 siMonsterKind = (SI16)pclClient->GetUniqueFromHash( TEXT("KIND_QUESTEMPERORMASTER") );
				if ( 0 >= siMonsterKind )
				{
					*preturn = CANUSEITEM_ERROR_INVALID;
					return false;
				}

				if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMON, siMonsterKind) )
				{
					*preturn = CANUSEITEM_ERROR_NOTFITCOND;
					return false;
				}
			}

			else if(clitem.siUnique == ITEMUNIQUE(8150))	// ���� ����� ����
			{
				// ���� ������ Ư�� ��ǥ������ ��� ���� �մϴ�.
				SI32 siPosX = 84;
				SI32 siPosY = 9;
                
				if(GetMapIndex() != MAPINDEX_LABYRINTH_1 || GetX() != siPosX || GetY() != siPosY)
				{
					*preturn = CANUSEITEM_ERROR_NOTFITCOND;
					return false ;
				}
			}
		}
		break;
	case ITEMTYPE_HEAL:
		{
				// ��Ʋ�ξ� - ���� �Ұ� ���
				if( pclClient->GameMode == GAMEMODE_SERVER)
				{
					cltServer* pclServer = (cltServer*)pclClient;
					CBattleRoyalMgr* pclBattleRoyalMgr = pclServer->m_pclBattleRoyalMgr;
					if( NULL != pclBattleRoyalMgr)
					{
						CBattleRoyal* pclBattleRoyal = pclBattleRoyalMgr->GetBattleRoyal((cltCharServer*)this);
						if( NULL != pclBattleRoyal)
						{
							if(!pclBattleRoyal->CanUseHealItem() && pclBattleRoyal->IsExistStadium((cltCharServer*)this) && pclBattleRoyal->IsGamingState())
							{
								*preturn = CANUSEITEM_ERROR_BATTLEROYALMAP;
								return false;

							}
						}
					}
				}

				// �����̾� ȿ���� �ִٸ�
				if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_PREMIUM )
				{
					if ( pclCI->clBI.bPremiumService )
					{
						*preturn = CANUSEITEM_ERROR_NOMOREUSE;
						return false ;
					}
				}
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_COMBAT )
				{
					if ( pclCI->clBI.bCombatService )
					{
						*preturn = CANUSEITEM_ERROR_NOMOREUSE;
						return false ;
					}
				}
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_BULLET )
				{
					if ( pclCI->clBI.bBulletService)
					{
						*preturn = CANUSEITEM_ERROR_NOMOREUSE;
						return false ;
					}
				}
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_SUMMONSTAMINA )
				{
					if ( pclCI->clBI.bSummonStaminaService)
					{
						*preturn = CANUSEITEM_ERROR_NOMOREUSE;
						return false ;
					}
				}
				// �������� �谡 ������ ���� �� �� ��밡���ϴ�.
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_DIGESTIVE )
				{		
	
 					if ( ( clPB.clHealth.GetHungryStatus( pclCI->clIP.GetLevel()) & (HEALTH_STATUS_HUNGRY_1 | HEALTH_STATUS_HUNGRY_2 | HEALTH_STATUS_HUNGRY_3 ) ) == FALSE)
					{
						
						*preturn = CANUSEITEM_ERROR_NOMORE_DIGEST;
						return false ;
					}
				}
				// ���ɾ��� ���°� ������ �� �� ��밡���ϴ�.
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_ALLDESEASE )
				{	
					cltCurDisease clCurdisease;					
					if(clPB.clHealth.GetCurDisease(&clCurdisease) == false)
					{
						*preturn = CANUSEITEM_ERROR_NOMORE_DESEASE;
						return false ;
					}
				}
				// ��Ȱ���� �׾������� ����ϱ⶧���� �Ϲ������� �Ҹ��ų�� ����.
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_RESURRECTION )
				{	
					*preturn = CANUSEITEM_ERROR_NOUSEABLE;
					return false ;
				}
				// [����] ��Ƽ������ ����Ʈ ����ÿ��� ����Ҽ��ִ�
				else if(pclItemManager->pclItemInfo[ref]->siHealEft == HEALEFT_QUEST_ANTIPOTION )
				{
					SI16 siMonsterKind = (SI16)pclClient->GetUniqueFromHash( TEXT("KIND_GREATWALLASURA") );
					if ( 0 >= siMonsterKind )
					{
						*preturn = CANUSEITEM_ERROR_NOUSEABLE;
						return false ;
					}
					
					if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMON, siMonsterKind) )
					{
						*preturn = CANUSEITEM_ERROR_NOUSEABLE;
						return false ;
					}
				}

				cltHorse * pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse()	;//���� Ÿ�� �ִ� ���� ������ �´�
				if ( pclHorse != NULL )
				{
					if(pclHorse->GetPremiumPartsNum() > 0)
					{
						// �����̾� ���� �������� ��� ���̸� �ν����� ��� �Ұ��Ѵ�.
						switch ( clitem.siUnique )
						{
						case ITEMUNIQUE(2740):
						case ITEMUNIQUE(2742):
							{
								*preturn	= CANUSEITEM_ERROR_CANNOTUSEPREMIUMITEM;
								return false	;
							}
							break;
						}

					}
				}
		}
		break;
	case ITEMTYPE_BOOK:
		{
			//[����] ��ų��.
			if( pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SKILLBOOK ) )
			{
				// ���� �������� ����.
				if( pclItemManager->pclSkillBookManager->GetSkillBookParentMagicFromItemUnique( clitem.siUnique ) )
				{
    				SI32 skillunique = clitem.GetSkillUnique( pclItemManager );
					if( pclCharSkillBookInfo->IsFullSubMagic( skillunique ) ) 
					{
						*preturn = CANUSEITEM_ERROR_SKILL_BOOK_SUB_MAGIC_FULL;
						return false;
					}
				
					SI32 skilllevel  = pclCI->clCharSkill.GetTotalSkillLevel( skillunique, 0, 0 );
					SI32 skilllimit  = clitem.GetSkillLevelLimit(pclItemManager);
					if( skilllevel < skilllimit )
					{
						TCHAR szSkillname[128] = "";
						pclClient->pclSkillManager->GetSkillName( skillunique, szSkillname );

						NTCHARString256 szExplain = GetTxtFromMgr(30519);
						szExplain.Replace( "#skillname#", szSkillname );

						cltClient* pclclient = (cltClient*)pclClient;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), szExplain );

						*preturn = CANUSEITEM_ERROR_SKILL_BOOK;
						return false;
					}
				}
				else
				{
					SI32 siSkillUnique = 0;
					switch( GetAttackType() )
					{
						case ATTACKTYPE_SWORD:		siSkillUnique = SKILL_USESWORD1;	break;
						case ATTACKTYPE_SPEAR:		siSkillUnique = SKILL_USESPEAR1;	break;
						case ATTACKTYPE_AXE:		siSkillUnique = SKILL_USEAXE1;		break;
						case ATTACKTYPE_STAFF:		siSkillUnique = SKILL_USESTAFF1;	break;
						case ATTACKTYPE_STAFF1:		siSkillUnique = SKILL_USESTAFF1;	break;
						case ATTACKTYPE_STAFF2:		siSkillUnique = SKILL_USESTAFF1;	break;
						case ATTACKTYPE_NEW_STAFF2:	siSkillUnique = SKILL_USESTAFF1;	break;
						case ATTACKTYPE_BOW:		siSkillUnique = SKILL_USEBOW1;		break;
						case ATTACKTYPE_BOW1:		siSkillUnique = SKILL_USEBOW1;		break;
						case ATTACKTYPE_NEW_BOW1:	siSkillUnique = SKILL_USEBOW1;		break;
						case ATTACKTYPE_GUN:		siSkillUnique = SKILL_USEGUN1;		break;
						case ATTACKTYPE_GUN1:		siSkillUnique = SKILL_USEGUN1;		break;
						case ATTACKTYPE_NEW_GUN1:	siSkillUnique = SKILL_USEGUN1;		break;
					}
					if(0 >= siSkillUnique ) return false;

					SI32 skilllevel  = pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
					SI32 skilllimit  = clitem.GetSkillLevelLimit(pclItemManager);
					if( skilllevel < skilllimit )
					{
						TCHAR szSkillname[128] = "";
						pclClient->pclSkillManager->GetSkillName( siSkillUnique, szSkillname );

						NTCHARString256 szExplain = GetTxtFromMgr(30518);
						szExplain.Replace( "#skillname#", szSkillname );

						cltClient* pclclient = (cltClient*)pclClient;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), szExplain );

						*preturn = CANUSEITEM_ERROR_SKILL_BOOK;
						return false;
					}
				}
			}
			else
			{
				SI32 skillunique = clitem.GetSkillUnique(pclItemManager);
				SI32 skilllevel  = pclCI->clCharSkill.GetSkillLevel(skillunique);

				//-----------------------------------
				// ��� ������ ���� �־�� ��밡��. 
				// ����Ȱ��ȭ �߰� : ������ų�̸� ��ų���ʽ�����Ʈ�� 0�̶� �ø��� �ִ�. - by PCK : 2007.08.09
				//-----------------------------------
				//if( pclClient->IsWhereServiceArea(ConstSwitch_ManufactureRevitalize) )		// ����Ȱ��ȭ
				if( pclClient->IsCountrySwitch(Switch_ManufactureRevitalize) )		// ����Ȱ��ȭ
				{
					if( pclCI->clIP.GetSkillBonus() <= 0 && pclClient->pclSkillManager->IsManufactureSkill(skillunique) == false )
					{
						*preturn	= CANUSEITEM_ERROR_NOSKILLPOINT;
						return false;
					}
				}
				else
				{
					if( pclCI->clIP.GetSkillBonus() <= 0 )
					{
						*preturn	= CANUSEITEM_ERROR_NOSKILLPOINT;
						return false;
					}
				}

				//-----------------------------------
				// ������� Ȯ��. 
				//-----------------------------------
				SI32 retunval = 0;
				if( pclItemManager->CheckCondition(itemunique, this, &retunval) == false )
				{
					*preturn	= CANUSEITEM_ERROR_NOTFITCOND;
					return false;
				}

				//----------------------------------
				// �̹� ���ߵ� ����� �ְ� �����̾�� �ȵȴ�. 
				//----------------------------------
				// ��ų�� �ִ� ��ġ�� ���Ѵ�. 
				SI32 maxskilllevel	= pclClient->pclSkillManager->GetMaxLevel(skillunique);
				if(skilllevel >= maxskilllevel)
				{
					*preturn	= CANUSEITEM_ERROR_ALREADYMAXSKILL;
					return false;
				}

				//-----------------------------------
				// �θ� ����� ���ߵǾ� �־�� �Ѵ�. 
				//-----------------------------------
				if(pclClient->pclSkillManager->GetParentSkillRef(skillunique) )
				{
					if(pclCI->clCharSkill.GetParentSkillLevel(skillunique, NULL) == 0)
					{
						*preturn	= CANUSEITEM_ERROR_NEEDPARENTSKILL;
						return false;
					}
				}

				//-----------------------------------
				// ��� ��¿� ������ �ִ� ���̶��, 
				//-----------------------------------
				SI32 skilllimit = clitem.GetSkillLevelLimit(pclItemManager);
				if(skilllimit)
				{
					// ���� �ش� ����� ������ ���ѿ� �ɸ��� ����. 
					if(skilllevel >= skilllimit)
					{
						*preturn	= CANUSEITEM_ERROR_LIMITSKILLLEVEL;
						return false;
					}
				}
			}
		}
		break;
	case ITEMTYPE_CRYSTAL://��Ƽ��Ʈ ����ü
		{
			return false;
		}break;
	case ITEMTYPE_COMPOSITIONMATERIAL://��Ƽ��Ʈ �ռ����
		{
			return false;
		}break;
	case ITEMTYPE_EVENTITEM://�̺�Ʈ ������
		{
			bool bCanUse = false;
			if(clitem.siUnique == ITEMUNIQUE(13025))	{		bCanUse = true;		}		// �������		
			if(clitem.siUnique == ITEMUNIQUE(13042))	{		bCanUse = true;		}		// ����		
			//KHY - 1113 - ũ�������� �̺�Ʈ -
			if(clitem.siUnique == ITEMUNIQUE(13002))	// ũ�������� �縻	 - �̺�Ʈ�Ⱓ�� ��밡�� ����.
			{			
				/*
				SYSTEMTIME	CurTime;
				if		( pclClient->GameMode == GAMEMODE_SERVER )	{	memcpy(&CurTime, &((cltServer*)pclClient)->sTime, sizeof(CurTime));		}
				else if	( pclClient->GameMode == GAMEMODE_CLIENT )	{	memcpy(&CurTime, &((cltClient*)pclClient)->sTime, sizeof(CurTime));		}
				
				if( pclClient->pclEventTimeManager->InEventTime("ChristmasEvent", &CurTime) )
				{
					bCanUse = true;
				}
				*/
				bCanUse = true;
			}

			if(clitem.siUnique == ITEMUNIQUE(13063))	{		bCanUse = true;		}		// ������Ű
			if(clitem.siUnique == ITEMUNIQUE(13005))	{		bCanUse = true;		}		// Ȳ�ݾ�		
			if(clitem.siUnique == ITEMUNIQUE(13028))	{		bCanUse = true;		}		// Ȳ�ݶ����		
			if(clitem.siUnique == ITEMUNIQUE(13021))	{		bCanUse = true;		}		// ��� ����	
			if(clitem.siUnique == ITEMUNIQUE(13015))	{		bCanUse = true;		}		// ��� ����	
			if(clitem.siUnique == ITEMUNIQUE(13329))	{		bCanUse = true;		}		// �� �ָӴ�	04.21 ����ũ����	
			if(clitem.siUnique == ITEMUNIQUE(13043))	{		bCanUse = true;		}		// [����] ���� �̺�Ʈ. NDE ���� �Ҹ�. => 2008-6-18
			if( false == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_OLYMPICMEDAL )	&&
				clitem.siUnique == ITEMUNIQUE(13044))	{		bCanUse = true;		}		// �ݸ޴�
			if(	false == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_OLYMPICMEDAL )	&&
				clitem.siUnique == ITEMUNIQUE(13045))	{		bCanUse = true;		}		// �ݸ޴�
			if(	false == pclCI->clBI.clBufInfo.IsHaveBuf( BUF_OLYMPICMEDAL )	&&
				clitem.siUnique == ITEMUNIQUE(13046))	{		bCanUse = true;		}		// �ݸ޴�
			if(clitem.siUnique == ITEMUNIQUE(13047))	{		bCanUse = true;		}		//[����] �߼� �̺�Ʈ. �߼����� ���. => 2008-8-8 
			if(clitem.siUnique == ITEMUNIQUE(13048))	{		bCanUse = true;		}		//[����] �߼� �̺�Ʈ. �߼����� ���. => 2008-8-8 
			if(clitem.siUnique == ITEMUNIQUE(13057))	{		bCanUse = true;		}		//[����] ����ã��
			if(clitem.siUnique == ITEMUNIQUE(13059))	{		bCanUse = true;		}		//[���] ũ�������� �̺�Ʈ - ������
			if(clitem.siUnique == ITEMUNIQUE(13061))	{		bCanUse = true;		}		//[���] ũ�������� �̺�Ʈ - ũ�������� ���
			if(clitem.siUnique == ITEMUNIQUE(13064))	{		bCanUse = true;		}		//[���] ũ�������� �̺�Ʈ - ũ�������� ���

			if(clitem.siUnique == ITEMUNIQUE(13058))	
			{

				if( pclCI->clQuestInfo.IsAllClear( pclClient->pclQuestManager, QUEST_TYPE_HUNTINGGROUND03)==true	)
				{
					bCanUse = true;
				}		
				else
				{
					*preturn	= CANUSEITEM_ERROR_NOT_ENOUGH_GET_TREASUR;
					// �ȵ� ������ ����.
				}
			}		//[����] ����ã��			
			if(clitem.siUnique == ITEMUNIQUE(13050))
			{
				if ( pclClient->IsCountrySwitch(Switch_HalloweenEvent) )
				{
					SYSTEMTIME	CurTime;
					if		( pclClient->GameMode == GAMEMODE_SERVER )	{	memcpy(&CurTime, &((cltServer*)pclClient)->sTime, sizeof(CurTime));		}
					else if	( pclClient->GameMode == GAMEMODE_CLIENT )	{	memcpy(&CurTime, &((cltClient*)pclClient)->sTime, sizeof(CurTime));		}
					
					if( pclClient->pclEventTimeManager->InEventTime("HalloweenEvent", &CurTime) )
					{
						bCanUse = true;
					}
				}
			}
			//[����] ���׿��� �ߺ� ��� �Ұ�.
			if(itemunique == ITEMUNIQUE(13048))
			{
				SI16 index	 = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_EVENT_EGGMOONCAKE, false );

				if ( index < 0 )
				{
					bCanUse = false;
				}
			}
			//����ã�� �̺�Ʈ ���Ѹ������� ��� �Ұ�
			if(clitem.siUnique == ITEMUNIQUE(13057))
			{
				if ( (GetMapIndex() == MAPINDEX_BEGINNER1) 
					|| (GetMapIndex() == MAPINDEX_PALACE) 
					|| (GetMapIndex() == MAPINDEX_MOFUMOFULAND)
					|| (GetMapIndex() == Const_MapIndex_PollMap)
					|| (GetMapIndex() == MAPINDEX_PRIVATEMARKETAREA)
					|| (GetMapIndex() == MAPINDEX_WEDDINGHALL)
					)
				{
					*preturn	= CANUSEITEM_ERROR_NOUSE_THISPLACE;

					bCanUse = false;
				}
			}
			
			// [���] ũ�������� �̺�Ʈ - ������ (�ߺ����Ұ�)
			if(clitem.siUnique == ITEMUNIQUE(13059))
			{
				SI16 index	 = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_CHRISTMASEVENT_SNOWBLESS, false );

				if ( index < 0 )
				{
					bCanUse = false;
				}
			}
			
			// [���] ũ�������� �̺�Ʈ - ũ�������� ��� (�ߺ����Ұ�)
			if(clitem.siUnique == ITEMUNIQUE(13061))
			{
				SI16 index	 = pclCI->clBI.clBufInfo.FindEmptySlot( BUF_CHRISTMASEVENT_DECORATION, false );

				if ( index < 0 )
				{
					bCanUse = false;
				}
			}

			return bCanUse;
		}break;
	case ITEMTYPE_BOX://�ڽ� ������
		{
			if ( GAMEMODE_CLIENT == pclClient->GameMode )
			{
				CDormancySystem *pclDormancySystem = pclClient->pclDormancySystem;
				if( NULL != pclDormancySystem)
				{
					CDormancyEquipItemRewardClient *pclDormancyClient = (CDormancyEquipItemRewardClient *)pclDormancySystem->GetDormancyEquipItemReward();
					if( NULL != pclDormancyClient)
					{
						// ������ ����
						pclDormancyClient->UseBox(clitem.siUnique , siPos);
					}
				}
			}

			return true;
		}break;
	case ITEMTYPE_PREMIUM://�����̾�������.
		{
			//���⿡�� �����̾� �������� ��� �Ҽ� �ִ��� üũ �Ѵ�.
			//�������� ����Ҽ� ���°��
			//1. ���� ������ ������̴�.
			// �����̾� ������ �̹� ���� ������ ������̴�.
			// ���� ���ǿ� �ɸ��� ������ ����Ҽ� �ִ�.

			// [����] ��ȣ����_090525 : ��ȣ���� ���������� ��ȯ�Ҽ��ִ��� �˻�
			if ( ITEMUNIQUE(13646) == clitem.siUnique || ITEMUNIQUE(13651) == clitem.siUnique || ITEMUNIQUE(13655) == clitem.siUnique || ITEMUNIQUE(13654) == clitem.siUnique|| ITEMUNIQUE(13659) == clitem.siUnique || ITEMUNIQUE(13658) == clitem.siUnique )
			{
				SI16 siTempKind = 0;
				SI16 siTempLife = 0;
				SI16 siTempShip	= 0;

				return CanUseSoulGuardItem( clitem.siUnique, &siTempKind, &siTempLife, &siTempShip, preturn );
			}

			if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == false)	return false ;
			cltHorse * pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse()	;//���� Ÿ�� �ִ� ���� ������ �´�
			if (pclHorse !=NULL )
			{
				// 
				SI32 siPremiumUnique_SLot1 = pclHorse->GetPremiumUnique_Slot1()	;
				SI32 siPremiumUnique_SLot2 = pclHorse->GetPremiumUnique_Slot2()	;
				SI32 siPremiumUnique_SLot3 = pclHorse->GetPremiumUnique_Slot3()	;
				if( clitem.siUnique == siPremiumUnique_SLot1 
					|| clitem.siUnique == siPremiumUnique_SLot2
					|| clitem.siUnique == siPremiumUnique_SLot3 )
				{
					// �̹� ����� �������Դϴ�.
					*preturn	= CANUSEITEM_ERROR_BEFOREUSE_SAMEITEM;
					return false;
				}
				// �ν����� ��� ���̸� ��� �Ұ�
				if ( pclHorse->siStatusTicketNum > 0 )
				{
					*preturn	= CANUSEITEM_ERROR_CANNOTUSEPREMIUMITEM;
					return false	;
				}
				if( pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE )
				{
					switch ( clitem.siUnique  )
					{
					case ITEMUNIQUE(13638):
					case ITEMUNIQUE(13639):
					case ITEMUNIQUE(13640):
						{
							// ����Ҽ� ���� Ÿ���� �������Դϴ�
							*preturn	= CANUSEITEM_ERROR_CANNOTUSETYPEGORSE_SAMEITEM;
							return false	;
						}
						break;
					}
				}
				else
				{
					switch ( clitem.siUnique  )
					{
					case ITEMUNIQUE(13635 ):
					case ITEMUNIQUE(13636 ):
					case ITEMUNIQUE(13637 ):
						{
							// ����Ҽ� ���� Ÿ���� �������Դϴ�
							*preturn	= CANUSEITEM_ERROR_CANNOTUSETYPEGORSE_SAMEITEM;
							return false	;
						}
					}

				}
			}
			
			return true;
		}break;

	case ITEMTYPE_QUEST:
		{
			//switch ( clitem.siUnique )
			//{
			//case ITEMUNIQUE(8121):
			//	{
			//		SI16 siMonsterKind = (SI16)pclClient->GetUniqueFromHash( TEXT("KIND_QUESTEMPERORMASTER") );
			//		if ( 0 >= siMonsterKind )
			//		{
			//			*preturn = CANUSEITEM_ERROR_INVALID;
			//			return false;
			//		}

			//		if ( false == pclCI->clQuestInfo.IsSameParaQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMON, siMonsterKind) )
			//		{
			//			*preturn = CANUSEITEM_ERROR_NOTFITCOND;
			//			return false;
			//		}

			//		return true;
			//	}
			//	break;
			//}
		}
		break;
	}

	
	return true;
}

bool cltCharCommon::CanRepairItem( cltItem* pclitem )
{
	if( pclitem == NULL	)										return false;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclitem->siUnique );
	if( ref <= 0 )												return false;

	cltItemInfo* pclItemInfo = pclClient->pclItemManager->pclItemInfo[ref];
	SI32 siSkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( pclItemInfo->clSkillInfo.siSkill, 0, 0, &pclCI->clCharItem.clItem[0], siID );

	// ��ų������ �����ؼ� ������ �� ����.
	if( pclItemInfo->clSkillInfo.siSkillLevel > siSkillLevel )
	{
		return false;
	}

	return true;
}

//---------------------------
// ��� 
//---------------------------
bool cltCharCommon::CheckEquipCondition(cltItemManagerCommon* pclItemManager, SI32 pos, cltItem* pclitem, SI32* preturnval)
{
	SI32 siParentID	= GetSummonParentID();
	

	// [����] ��ȯ���϶��� ������ True
	if ( pclCM->IsValidID(siParentID) )
	{
		return true;
	}

	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		
		// [����] ������ üũ
		if ( (PERSONITEM_HELMET <= pos) 
			&& (pos < PERSONITEM_INV0)
			&& (pclitem->IsBulletItem() == false) // �߻�ü �������� �ƴҰ��
			&& (pclitem->GetDurability() <= 0)
		)
		{
			return false;
		}
	}

	// ���� ������ �����Ǿ� �ִ��� Ȯ���Ѵ�.
	if(pos == PERSONITEM_WEAPON1 || pos == PERSONITEM_WEAPON2)
	{	
		SI32 type			= pclitem->GetType(pclItemManager);
		SI32 returnskillval	= 0;


		if(pclCI->clCharSkill.ChcekUseWeaponSkill(type, pclitem, &returnskillval) == false)return false;

		//cyj źȯ�� �������϶� ����â�� �ٸ��������� ������ ���� �Ұ�
		if(pclitem->IsBulletItem() && pclCI->clCharItem.clItem[pos].siUnique != 0 && pclCI->clCharItem.clItem[pos].siUnique != pclitem->siUnique)
			return false;
	}
	
	if ( pos == PERSONITEM_HELMET )
	{
		if ( pclitem->GetType(pclItemManager) == ITEMTYPE_HAT ) return false ;
	}

	if ( pos == PERSONITEM_ARMOUR )
	{
		if ( pclitem->GetType(pclItemManager) == ITEMTYPE_DRESS ) return false ;
	}

	if ( (0 <= pos) && (PERSONITEM_INV0 > pos) ) //( pos < PERSONITEM_INV0 || pos >= MAX_ITEM_PER_PERSON )
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(pclitem->siUnique);
		if(ref <= 0)return false;
		if(pclItemManager->pclItemInfo[ref] == NULL)return false;

		
		// ����� ��. 
		if(pos < PERSONITEM_INV0)
		{
			if(pclItemManager->CheckCondition(pclitem->siUnique, this, preturnval) == false)return false;

			// ���ڿ��� �ϴ� ���, 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 1)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && GetGameMasterLevel() != 1)
				{
					return false;
				}
			}
			// �� 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 2)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && pclCI->clBI.uiGRank != 1)
				{
					return false;
				}
				
			}
			// 6�� �Ǽ� 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 3)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) 
					&& pclCI->clBI.uiGRank != GRANK_MINISTER1
					&& pclCI->clBI.uiGRank != GRANK_MINISTER2
					&& pclCI->clBI.uiGRank != GRANK_MINISTER3
					&& pclCI->clBI.uiGRank != GRANK_MINISTER4
					&& pclCI->clBI.uiGRank != GRANK_MINISTER5
					&& pclCI->clBI.uiGRank != GRANK_MINISTER6)
				{
					return false;
				}
			}
			// ������̾�� �ϴ� ���, 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 4)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && GetGameMasterLevel() != 2)
				{
					return false;
				}
			}
			// ��ȣõ�翩�� �ϴ� ���, 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 5)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && pclCI->clBI.uiAngelSwitch == 0)
				{
					return false;
				}
			}
			// ����ε���̿��� �ϴ� ���
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 6)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && GetGameMasterLevel() != 3)
				{
					return false;
				}
			}
			// ����
			if(pclItemManager->pclItemInfo[ref]->siNeedRank == 7)
			{
				if(GetGameMasterLevel() != (BASIC_GAMEMASTER_LEVEL+1) && pclCI->clBI.uiPoliceSwitch == 0)
				{
					return false;
				}
			}
		}
		/*
		// ��ȯ�϶� ��. 
		else if ( true == bSummon )
		{
			SI32 siSummonIndex = GetSummonIndex();
			// �źл��� ������ �ִ� ���� ������ �� ����. 
			if(pclItemManager->pclItemInfo[ref]->siNeedRank)return false;

			cltSummon* pclsummon = pclCM->CR[siParentID]->pclCI->clSummonInfo.GetSummon( siSummonIndex );
			if ( NULL == pclsummon )
			{
				return false;
			}

			SI16 siAttackWearStoneNum	= pclsummon->siAttackWearStoneNum;
			SI16 siDefenseWearStoneNum	= pclsummon->siDefenseWearStoneNum;

			SI32 siSummonKind = pclsummon->siKind;
			if ( false == pclClient->pclKindInfoSet->IsValidKind( siSummonKind ) )
			{
				return false;
			}

			cltKindInfo* pclki = NULL;
			if ( pclClient->IsCountrySwitch(Switch_Summon_Separate) ) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[siSummonKind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[siSummonKind];
			}

			if ( NULL == pclki )
			{
				return false;
			}
			
			// ��� ���� Ÿ���� �ƴ϶�� 
			if ( SUMMON_EQUIPTYPE_NONE == pclki->siSummonEquip )
			{
				return false;
			}

			
			cltParametaBox clPB;
			cltPIPInfo	clipinfo2;
			clipinfo2.Set(&pclsummon->clIP);
			clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki);

			bool bmosterhero = false;

			cltItem* pclitemlist = pclCI->clSummonInfo.GetSummonEquipItemList( siSummonIndex );
			if ( NULL == pclitemlist )
			{
				return false;
			}

			clPB.Action(this, 0, PERSONITEM_WEAPON1, pclitemlist, NULL, NULL, NULL, NULL, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true, bmosterhero);
			
		}
		*/
	}

	return true;
}

void cltCharCommon::SetHitSwitch(bool mode)
{
	bHitSwitch  = mode;

	if(mode == true)
	{
		// �ǰ� ������ ���� ĳ���;� �Ѵ�. 
		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 aniref			= GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(ANITYPE_HIT);
		if(aniref < 0)return ;

		
		// �ִϸ��̼� ���� �ʱ�ȭ. 
		clAniInfo.Init1();

		clAniInfo.dwAniStartClock	= pclClient->CurrentClock;
		clAniInfo.dwElapsedClock	= 0;
	}

}

//----------------------------------------
// �״� ���� 
//----------------------------------------
bool cltCharCommon::Action_Dying()
{
	//--------------------------------
	// �ٸ� ������ �޾� ������ �ʴ´�. 
	//--------------------------------
	bBusySwitch = true;

	//-----------------------------
	// ����� �ѹ��� �̷������ �� ���� 
	//-----------------------------
	if(bDyingSwitch == false)
	{
		bDyingSwitch = true;

		// by LEEKH 2007.12.24 - TRANSFORM
		if((pclKI != NULL && GetKindInfo()->IsAtb(ATB_SELFBOMB)))
		{
			
			cltPos cltilestartpos(GetX(), GetY());
			cltPos cldotstartpos(siCenterX, siCenterY);
			
			cltAttackTarget cltarget;
			cltarget.Set(&clAttackTarget);
			cltarget.siHitRate	= 100;
			
			cltarget.clPos.SetMapIndex(GetMapIndex());
			cltarget.clPos.SetX(GetX());		// Ÿ���� ��ǥġ�� �ڽ��� ��ġ���� �Ѵ�. 
			cltarget.clPos.SetY(GetY());

			SI32 sibombeffect = BULLETKIND_SELFBOMB ;
			//if ( GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_BOMBWHALE")) )
			//	sibombeffect = BULLETKIND_SELFBOMBWHALE ;

			SI32 bulletid = pclClient->pclBulletManager->SetBullet(pclMap, sibombeffect, 0, 
				siID, GetApower() * 2,
				&cltilestartpos,
				&cldotstartpos, 
				&cltarget,
				&cldotstartpos);
		}
		
	}

    return FALSE;
}

SI32 cltCharCommon::GetSummonID()
{
	// ���� ��ȯ�� ID
	SI32 siSummonID = pclCM->GetIDFromCharUnique(siChildCharUnique);	

	if(pclCM->IsValidID(siSummonID)  == TRUE)
	{
		if(pclCM->CR[siSummonID]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			if( (GetCharUnique()      == pclCM->CR[siSummonID]->clIdentity.clSummonInfo.siParentCharUnique) &&
				(pclCI->GetPersonID() == pclCM->CR[siSummonID]->clIdentity.clSummonInfo.siParentPersonID) )
			{
				return siSummonID;
			}
		}
	}

	return 0;
}


SI32 cltCharCommon::GetSummonParentID()
{

	if(clIdentity.siIdentity ==IDENTITY_PERSON_SUMMON )
	{
		SI32 siParentID = pclCM->GetIDFromCharUnique(clIdentity.clSummonInfo.siParentCharUnique);	
		
		if(pclCM->IsValidID(siParentID) == TRUE && IsPC(siParentID)==TRUE)
		{
			if( GetCharUnique()            == pclCM->CR[siParentID]->siChildCharUnique &&
				clIdentity.clSummonInfo.siParentPersonID == pclCM->CR[siParentID]->pclCI->GetPersonID() ) 
			{
				return siParentID;
			}
		}
	}

	return 0;
}


SI32 cltCharCommon::GetSummonIndex()
{
	if(clIdentity.siIdentity ==IDENTITY_PERSON_SUMMON )
	{
		return clIdentity.clSummonInfo.siMySummonIndex;
	}
	return 0;
	
}

SI32 cltCharCommon::GetSoulGuardCharID()
{
	SI32 siSoulGuardCharID = pclCM->GetIDFromCharUnique( siMySoulGuardUnique );
	if ( false == pclCM->IsValidID(siSoulGuardCharID) )
	{
		return 0;
	}

	cltCharCommon* pclSoulGuardChar = pclCM->CR[siSoulGuardCharID];
	if ( NULL == pclSoulGuardChar )
	{
		return 0;
	}

	if ( IDENTITY_SOULGUARD != pclSoulGuardChar->clIdentity.siIdentity )
	{
		return 0;
	}

	if ( GetCharUnique() != pclSoulGuardChar->clIdentity.clSoulGuard.m_siParentCharUnique )
	{
		return 0;
	}

	if ( pclCI->GetPersonID() == pclSoulGuardChar->clIdentity.clSoulGuard.m_siParentPersonID )
	{
		return siSoulGuardCharID;
	}

	return 0;
}

// [����] ��ȣ����_090525 : ��ȣ������ �θ� CharID�� ���� �´�
SI32 cltCharCommon::GetSoulGuardParentCharID( void )
{
	if ( IDENTITY_SOULGUARD != clIdentity.siIdentity )
	{
		return 0;
	}

	SI32 siParentCharID = pclCM->GetIDFromCharUnique( clIdentity.clSoulGuard.m_siParentCharUnique );
	if ( (false == pclCM->IsValidID(siParentCharID)) || (false == IsPC(siParentCharID)) )
	{
		return 0;
	}

	cltCharCommon* pclParent = pclCM->CR[siParentCharID];
	if ( NULL == pclParent )
	{
		return 0;
	}
	
	// �θ� �����Ǿ��ִ� ĳ���� ����ũ�� ���Ѵ�
	if ( GetCharUnique() != pclParent->siMySoulGuardUnique )
	{
		return 0;
	}

	// �� �θ� ������ ������ �θ� �۽����̵� �´��� ��
	if ( pclParent->pclCI->GetPersonID() == clIdentity.clSoulGuard.m_siParentPersonID )
	{
		return siParentCharID;
	}
	
	return 0;

}

//KHY - 1109 - ��í����- �������漮�߰�
SI32 cltCharCommon::IsHaveCushionItem()
{
	SI16 cushionType[MAX_CUSHION_NUM] = {0,}; // ���漮�� ����. - 0:������ �漮. 1:������ �漮.  2:�̺�. 3:���ڴ� �Ŀ���

	SI32	 returnItemUnique = 0; // ������ ������ ����ũ.
	
	switch(GetKind())
	{
		case KIND_HERO3: // �Ϻ���.
			{
				 cushionType[0] = ITEMUNIQUE(13610);
				 cushionType[1] = ITEMUNIQUE(13617);
				 cushionType[2] = ITEMUNIQUE(13629);
				 cushionType[3] = ITEMUNIQUE(13649);
			}
			break;
		case KIND_HEROINE3: // �Ϻ���.
			{
				 cushionType[0] = ITEMUNIQUE(13611);
				 cushionType[1] = ITEMUNIQUE(13619);
				 cushionType[2] = ITEMUNIQUE(13628);
				 cushionType[3] = ITEMUNIQUE(13650);
			}
			break;
		case KIND_HERO4: // ���糲
			{
				 cushionType[0] = ITEMUNIQUE(13608);
				 cushionType[1] = ITEMUNIQUE(13618);
				 cushionType[2] = ITEMUNIQUE(13629);
				 cushionType[3] = ITEMUNIQUE(13647);
			}
			break;
		case KIND_HEROINE4: // ���翩.
			{
				 cushionType[0] = ITEMUNIQUE(13609);
				 cushionType[1] = ITEMUNIQUE(13620);
				 cushionType[2] = ITEMUNIQUE(13628);
				 cushionType[3] = ITEMUNIQUE(13648);
			}
			break;
	}

	// ��ġ�� ã�� ���� ���� ���� �����Ѵ�.(0�� �ƴ�)
	// ���ڰ� �۾ƾ� �ռ����̴�.
	SI16 temCushionPose1 = 0;
	SI16 temCushionPoseOld = 10000; // �Ұ����� ��ġ.

	for(SI16 x = 0; x < MAX_CUSHION_NUM ;x++)
	{
		if(cushionType[x] > 0) // ��ϵ� �漮 ������.
		{
			temCushionPose1 = pclCI->clCharItem.FindItemInv(cushionType[x]);

			if( temCushionPose1 > 0) // ������ �ִٸ�.
			{
				if(temCushionPose1 < temCushionPoseOld) // ���� ���� ���� ã�´�.
				{
					temCushionPoseOld = temCushionPose1; 
					returnItemUnique =  cushionType[x];
				}
			}
		}		
	}		

	if(returnItemUnique >0)
		return returnItemUnique;
	else
		return 0;
}

//KHY - 1220 - ���Žý���.
bool cltCharCommon::IsCanTransFormRecover()
{		
	if(clTransFormInfo.bTransFormMode == true)
	{
		if(clPB.siTransFormSyc >= 100) // ��ũ�� 100% ���ʽ�.
		{
			switch(clPB.siAttackType)
			{ 
				case ATTACKTYPE_STAFF:
				case ATTACKTYPE_STAFF1:
				case ATTACKTYPE_STAFF2:
				case ATTACKTYPE_NEW_STAFF2:
					return true;
							break;
			}
		}
	}

	return false;
}

bool cltCharCommon::CanTransFormAttackType(SI32 siCharKind)
{
	if( siCharKind < 0 || siCharKind >= MAX_KIND_NUMBER )			return false;

	SI32	siAttackType = pclClient->pclKindInfoSet->pclKI[siCharKind]->clDefaultWeaponInfo.GetAttackType();
	SI32	siCharAttackType = GetAttackType();

	bool	bCanTranform = false;

	//KHY - 0124 - ��í����- ���� ���� ������.
	// �Ƹ��� - ���ο� ���� ĳ���Ͱ� �߰��ɶ�, ���� �Ӽ��� ������ �־�� �Ұ��̴�.

	switch(siAttackType)
	{
	case ATTACKTYPE_SWORD:		// �´� , ������( �� ) , ������ �ٰŸ� 
		{
			switch(siCharAttackType)
			{
			case ATTACKTYPE_SWORD:
			case ATTACKTYPE_SPEAR:
			case ATTACKTYPE_AXE:
				{
					bCanTranform = true;
				}
				break;
			}
		}
		break;
	case ATTACKTYPE_CHRISTMASRANGE:
	case ATTACKTYPE_FIGHTINGMENRANGE:	// �ųʽ��Ǹ�
	case ATTACKTYPE_HALLOWEENGHOST:
	case ATTACKTYPE_MOONBLADE:		// �򰭰���
	case ATTACKTYPE_NINJAMAN:		// ������( �� )
	case ATTACKTYPE_CATRANGE:		// ������( ���Ÿ� )
	case ATTACKTYPE_FAIRYRANGE:		// �������Ǹ�(��)
	case ATTACKTYPE_SNOWQUEEN:		// ��í - 12�� ���Ǹ� ����	���� ���� �罺��ġ
		{
			switch(siCharAttackType)
			{
			case ATTACKTYPE_STAFF:
			case ATTACKTYPE_BOW:
			//case ATTACKTYPE_BOW1:
			//case ATTACKTYPE_NEW_BOW1:
			case ATTACKTYPE_GUN:
			//case ATTACKTYPE_GUN1:
			case ATTACKTYPE_NEW_GUN1:
			case ATTACKTYPE_STAFF1:
			//case ATTACKTYPE_STAFF2:
			//case ATTACKTYPE_NEW_STAFF2:
				{
					bCanTranform = true;
				}
				break;
			}
		}
		break;
	}

	return bCanTranform;
}


SI32 cltCharCommon::GetItemUniqueFromCharKind(SI32 siCharKind)
{
	if( siCharKind == pclClient->GetUniqueFromHash("KIND_ONDAL") )
	{
		return ITEMUNIQUE(13612);
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_PYEONGGANG") )
	{
		return ITEMUNIQUE(13613);
	}
	//KHY - 0124 - ��í����- ���� ���� ������.
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_NINJAMAN") ) // ���� ��  - ��
	{
		return ITEMUNIQUE(13615);
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_NINJAGIRL") ) //  ����  ��  - �ٰŸ�
	{
		return ITEMUNIQUE(13616 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_CATRANGE") ) // ������ ����  - ���Ÿ�.
	{
		return ITEMUNIQUE(13623);
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_CATMELEE") ) // ������ ����  - �ٰŸ�.
	{
		return ITEMUNIQUE(13624 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN") ) // �ҷ��� ����  - �ٰŸ�.
	{
		return ITEMUNIQUE( 13626 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST") )  // �ҷ��� ����  - ���Ÿ�
	{
		return ITEMUNIQUE( 13627 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE") )  // ũ�������� �ٰŸ�����
	{
		return ITEMUNIQUE( 13633 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE") )  // ũ�������� ���Ÿ����� - �����귡���
	{
		return ITEMUNIQUE( 13632 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE") )	// �˻罺�Ǹ�(��) - �ٰŸ�
	{
		return ITEMUNIQUE( 13641 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE") )	// �ųʽ��Ǹ�(��) - ���Ÿ�
	{
		return ITEMUNIQUE( 13642 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_FAIRYMELEE") )	//  �������Ǹ�(��) - �ٰŸ� (�ӽ�:�˻罺�Ǹ�(��) - �ٰŸ�)
	{
		return ITEMUNIQUE( 13652 );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_FAIRYRANGE") )	// �������Ǹ�(��) - �ٰŸ� (�ӽ�:�ųʽ��Ǹ�(��) - ���Ÿ�)
	{
		return ITEMUNIQUE( 13653 );
	}
	// ��í - 12�� ���Ǹ� ����	���� ���� �罺��ġ
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_ICERANGE") )		//  ���� ����	-	( ���Ÿ�)
	{
		return ITEMUNIQUE( 13657  );
	}
	else if( siCharKind == pclClient->GetUniqueFromHash("KIND_ICEMELEE") )	// ������ġ	-	(�ٰŸ�)
	{
		return ITEMUNIQUE( 13656  );
	}
	return 0;
}

SI32 cltCharCommon::GetCharKindFromItemUnique(SI32 siItemUnique)
{
	if( siItemUnique == ITEMUNIQUE(13612) )
	{
		return pclClient->GetUniqueFromHash("KIND_ONDAL");
	}
	else if( siItemUnique == ITEMUNIQUE(13613) )
	{
		return pclClient->GetUniqueFromHash("KIND_PYEONGGANG");
	}
	//KHY - 0124 - ��í����- ���� ���� ������.
	else if( siItemUnique == ITEMUNIQUE(13615) )
	{
		return pclClient->GetUniqueFromHash("KIND_NINJAMAN");
	}
	else if( siItemUnique == ITEMUNIQUE(13616) )
	{
		return pclClient->GetUniqueFromHash("KIND_NINJAGIRL");
	}
	else if( siItemUnique == ITEMUNIQUE(13623) ) // ������ ����  - ���Ÿ�.
	{
		return pclClient->GetUniqueFromHash("KIND_CATRANGE");
	}
	else if( siItemUnique == ITEMUNIQUE(13624) ) // ������ ����  - �ٰŸ�.
	{
		return pclClient->GetUniqueFromHash("KIND_CATMELEE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13626 ) )  // [����] ȣ�ڻ���. �ҷ��� ����  - �ٰŸ�.
	{
		return pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN");
	}
	else if( siItemUnique == ITEMUNIQUE( 13627 ) )  // [����] ȣ�ڻ���. �ҷ��� ����  - ���Ÿ�
	{
		return pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST");
	}
	else if( siItemUnique == ITEMUNIQUE( 13633 ) )  // ũ�������� �ٰŸ�����
	{
		return pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13632 ) )  // ũ�������� ���Ÿ����� - �����귡���
	{
		return pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13641 ) ) // �˻罺�Ǹ�(��) - �ٰŸ�
	{
		return pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13642 ) )  // �ųʽ��Ǹ�(��) - ���Ÿ�
	{
		return pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE");
	}

	// ���� ���Ǹ� �߰� - �׷��� ������ �˻�/�ų� ���Ǹ����� ��ü.
	else if( siItemUnique == ITEMUNIQUE( 13652 ) ) // ���� ���Ǹ�(��) - �ٰŸ� (�ӽ�:�˻罺�Ǹ�(��) - �ٰŸ�)
	{
		return pclClient->GetUniqueFromHash("KIND_FAIRYMELEE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13653 ) )  // ���� ���Ǹ�(��) - ���Ÿ� (�ӽ�:�ųʽ��Ǹ�(��) - ���Ÿ�)
	{
		return pclClient->GetUniqueFromHash("KIND_FAIRYRANGE");
	}
	// ��í - 12�� ���Ǹ� ����	���� ���� �罺��ġ
	else if( siItemUnique == ITEMUNIQUE( 13657  ) )	// ���� ����	-	���Ÿ�
	{
		return pclClient->GetUniqueFromHash("KIND_ICERANGE");
	}
	else if( siItemUnique == ITEMUNIQUE( 13656  ) )  // �罺��ġ	-	�ٰŸ�
	{
		return pclClient->GetUniqueFromHash("KIND_ICEMELEE");
	}
	return 0;
}

// ����� ���ؼ� ���� �� �ִ� �۹��� Ȯ��. 
SI32 cltCharCommon::GetFarmRate(SI32 farmskilllevel )
{
	//cltItemPriceUnit* pclprice = pclClient->pclItemManager->pclItemPrice->GetPrice(pclClient->pclItemManager, itemunique);
	//if(pclprice == NULL)return 0;

	//
	//SI32 delay = min(10000, max(1000, pclprice->siPrice)) * (100 - farmskilllevel) / 100;
	//delay = max(100, delay);		// �ּ� 10�ʴ� ����. 
	//delay *= 60;

	//return delay;
	
	SI32 rate = 10;
	rate += farmskilllevel;

	return rate;

}

// ä���� ���ؼ� ���� �� �ִ� ������ Ȯ��. 
SI32 cltCharCommon::GetMineRate(SI32 mineskilllevel)
{
	SI32 rate = 10;
	rate += mineskilllevel;

	return rate;
}

void cltCharCommon::ParametaBoxAction()
{
	bool		bmonsterhero			= false; // ���Ϳ��� ��ȯ�������� ��ȭ�Ѱ����� ����, 
	cltItem*	pclitemlist				= NULL;
	SI16		siAttackWearStoneNum	= 0;
	SI16		siDefenseWearStonenum	= 0;
	SI32		parentid				= GetSummonParentID();
	
	// ��ȯ�� �϶�
	if ( pclCM->IsValidID(parentid) )
	{
		cltSummon *pclSummon = pclCM->CR[parentid]->pclCI->clSummonInfo.GetSummon( GetSummonIndex() );
		if(pclSummon)
		{
			// ��ȯ���� ��� ����� ���� �´�
			pclitemlist = pclCM->CR[parentid]->pclCI->clSummonInfo.GetSummonEquipItemList( GetSummonIndex() );

			// ��� ���뼮 ������ ���� �´�
			siAttackWearStoneNum	= pclSummon->siAttackWearStoneNum;
			siDefenseWearStonenum	= pclSummon->siDefenseWearStoneNum;
		}

	}
	// ����϶�
	else if( IsPC(GetCharUnique()) )
	{
		pclitemlist = pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
	}

	//------------------------------------
	// ������ ������ �����͸� ���Ѵ�. 
	//------------------------------------
	bool bNpc = false ;
	if ( IsInstanceNPC(GetCharUnique()) || IsNPC(GetCharUnique()) )
		bNpc = true ;

	clPB.Action(this, 
				pclCI->GetPersonID(), 
				ActiveWeapon, 
				pclitemlist, 
				&pclCI->clHorseInfo, 
				&pclCI->clHealthInfo,
				&pclCI->clCharSkill,
				&clIdentity, 
				siAttackWearStoneNum,
				siDefenseWearStonenum,
				pclClient->GameMode, 
				bNpc, 
				bmonsterhero);


	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		//------------------------------------------------
		// Mana�� �ִ밪�� �ʰ��ϴ� ��� �ִ밪���� ������. 
		//-------------------------------------------------
		if(GetMana() > clPB.GetMaxMana())
		{
			//SetMana(clPB.GetMaxMana());
		}

/*		if(GetLife() > clPB.GetMaxLife())
		{
			SetLife(clPB.GetMaxLife());
		}		
*/
	}

}

// �ְ� ���� ��� ���θ� �����Ѵ�. 
void cltCharCommon::ChangeTopSkillUnique(SI32 skillunique)
{
	if(skillunique == clInnerStatus.siTopSkillUnique)return ;

	siReservedTopSkillUnique = skillunique;

}

SI32 cltCharCommon::GetCharUnique() const
{
	return CharUnique;
}

void cltCharCommon::SetCharUnique(SI32 unique)
{
	CharUnique = unique;
}

void cltCharCommon::SetCurrentOrder(SI32 order)
{
	clCurOrder.siOrder = order;
}

SI32 cltCharCommon::GetCurrentOrder()const
{
	return clCurOrder.siOrder;
}

void cltCharCommon::SetID(SI32 id)
{
	siID	= id;
}

SI32 cltCharCommon::GetID() const
{
	return siID;
}

SI32 cltCharCommon::GetCenterX() const
{
	return siCenterX;
}

SI32 cltCharCommon::GetCenterY() const
{
	return siCenterY;
}

SI32 cltCharCommon::GetAttackRange() const
{
	return clPB.GetAttackRange();
}

SI32 cltCharCommon::GetAttackType()
{
	return clPB.GetAttackType();
}

SI32 cltCharCommon::GetMaxWeight()
{
	return clPB.GetMaxWeight();
}

BOOL cltCharCommon::IsSameMap(const cltMapCommon* pclmap)
{
	if(pclMap == pclmap)return TRUE;
	return FALSE;
}

// SBS������ΰ�. 
bool cltCharCommon::IsSBS()
{
	if(pclCI->clBI.szAccountID[0] =='s'
		&& pclCI->clBI.szAccountID[1] =='_')
	{
		return true;
	}

	return false;

}

BOOL cltCharCommon::IsAtb(SI32 atb)
{			
	if( Atb & atb )		return TRUE;
	return FALSE;
}

// ��� ������ �����Ǿ�� 
BOOL cltCharCommon::IsAtbAnd(SI32 atb)
{			
	if((Atb&atb)==atb)return TRUE;
	return FALSE;
}

BOOL cltCharCommon::IsDefenceAtb(SI32 atb)
{	
	if(DefenceAtb==atb)return TRUE;	
	return FALSE; 
};

SI32 cltCharCommon::GetApower()
{
	return clPB.CalcApower();
}

int	cltCharCommon::GetMovePercent()
{
	return clMoveInfo.siMovePercent;
}

void cltCharCommon::MoveInit()
{
	clMovePointList.Init();

	clMoveInfo.siMovePercent	= 0;
}

SI32 cltCharCommon::GetGameMasterLevel()
{
	return siGameMasterLevel;
}

void cltCharCommon::SetGameMasterLevel(SI32 level)
{
	siGameMasterLevel	=	level;
}

SI32 cltCharCommon::GetRareItemRate(SI32 where)
{
	if ( where < 0 || where >= 4 ) return 0 ;
	return siRareItemRate[where];
}

void cltCharCommon::SetRareItemRate(SI32 attack,SI32 mag,SI32 skill,SI32 dur)
{
	siRareItemRate[0] = attack;
	siRareItemRate[1] = mag;
	siRareItemRate[2] = skill;
	siRareItemRate[3] = dur;
}

bool cltCharCommon::GetWennyOldManSuccess()
{
	return bWennyOldMan;
}

SI32 cltCharCommon::GetWennyOldManMode()
{
	return siWennyOldMan;
}

void cltCharCommon::SetWennyOldMan(bool success,SI32 mode)
{
	bWennyOldMan = success ;
	siWennyOldMan = mode ;
}

void cltCharCommon::SetCurrentMagic(SI32 magic, SI08 siCastType)
{
	siCurrentMagic			= magic;
	siCurrentMagicCastType	= siCastType;
}

SI32 cltCharCommon::GetCurrentMagic()
{
	return siCurrentMagic;
}

SI08 cltCharCommon::GetCurrentMagicCastType()
{
	return siCurrentMagicCastType;
}

void cltCharCommon::SetParametaBoxAction(bool mode)
{
	clCommonSimpleVary.bForceparametaBoxActionSwitch = mode;
}

void cltCharCommon::SetpclCM(cltCharManager* pclcm)
{
	pclCM = pclcm;
}

// ĳ���Ͱ� ������ �� �� �ִ°�?
bool cltCharCommon::CanWarpFrom()
{
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		cltServer* pclserver = (cltServer*)pclClient;
		// ���������� ���� �� �� ����.
		if( clInnerStatus.IsStatus(INNERSTATUS_PRISON))
		{
			pclserver->SendServerResponseMsg(0, SRVAL_CAN_NOT_WARP_IN_PRISON, 0, 0, GetCharID());
			return false;
		}

		// �Ҽӵ� ������ ��ȯž�� ������ �ִ����� ���� üũ
		//cltStrInfo* pclinfo = pclClient->pclVillageManager->pclCityHallManager->GetStrInfo(pclCI->clBI.siHomeVillage );
		//if( pclinfo != NULL )
		//{
		//	for (SI32 i = 0; i < MAX_HIRED_EVENTNPC_NUM; i++ )
		//	{
		//		// ��ȯž�� ������ ���� ������
		//		if(pclClient->GetUniqueFromHash(TEXT("KIND_RETURNTOWER")) != pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind)
		//		{
		//			// ������ ���� ���� ���, ������ ��ȯž�� ��ġ �Ǿ�� ���� �����ϴٴ� �޼���.
		//			pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_RETURNTOWER, 0, 0, GetCharID()); 
		//			return false;
		//		}
		//	}		
		//}	

		// 3�� Ÿ�̸� üũ
		if( true == ((cltCharServer*)this)->m_clCharUpdateTimer[CHAR_UPDATETIMER_3SECOND].IsActivated())	// Ÿ�̸Ӱ� ���� ������
		{
			// Ÿ�̸Ӱ� �����ڷ� 3�ʰ� ������ �ʾ�����
			if( false == ((cltCharServer*)this)->m_clCharUpdateTimer[CHAR_UPDATETIMER_3SECOND].IsTimed(GetTickCount()) )
			{
				// �����̵��� 3�ʿ� �ѹ��� �����ϴٴ� �޼���
				pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_LIMITTIME, 0, 0, GetCharID());
				return false;
			}
		}

		// �������� ����� �����忡 ������ ��� �޽����� �ѷ��ش�(�ܼ� ��� ���̴�)
		if ( TRUE == pclserver->IsCountrySwitch(Switch_WeddingSystem) )
		{
			if ( (MAPINDEX_WEDDINGHALL == GetMapIndex()) && (IsMarried()) && (pclserver->m_pclMarriageMgr) )
			{
				cltWeddingReserveUnit* pclUnit = pclserver->m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtrFromNowTime();
				if ( (pclUnit) && (pclUnit->IsReserved()) )
				{
					if ( (pclCI->GetPersonID() == pclUnit->m_siMate1_PersonID ) || ( pclCI->GetPersonID() == pclUnit->m_siMate2_PersonID ) )
					{
						pclserver->SendServerResponseMsg(0, SRVAL_WEDDING_HALL_OUT_NOTICE, 0, 0, GetCharID());
					}
				}
			}
		}

	}

	else
	{
		cltClient* pclclient = (cltClient*)pclClient;

		if( clInnerStatus.IsStatus(INNERSTATUS_PRISON))
		{
			TCHAR* pTitle = GetTxtFromMgr(836);
			TCHAR* pText = GetTxtFromMgr(6034);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

			return false;
		}
	}

	// ��� �ʿ����� ��ڸ��� ���� �� �ִ�.
	if ( GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL && GetMapIndex() == MAPINDEX_MASTERWORLD )
		return false;

	SI32 siCharID = GetCharID();

	// �� ������ ��� ����ϸ� ������ �� �� �ִ�.
	return true;
}

// �ش� ������ �����ؼ� �� �� �ִ°�?
bool cltCharCommon::CanWarpTo( SI32 siToMapIndex )
{
	// ���ڳ����� ���λ������δ� ��ڰ� �ƴϸ� �� �� ����.
	if ( GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL && siToMapIndex == MAPINDEX_MASTERWORLD )		return false;

	// ���� ���� ó��
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		cltServer* pclserver = (cltServer*)pclClient;

		//[����] ���� �̺�Ʈ�� ��Ÿ���� �ʵ��� ��� ������ �̵� �Ұ����ϰ� �Ѵ�.
		if(pclClient->IsCountrySwitch(Switch_Server_Event))
		{
			// ���� �̺�Ʈ ������� ����� ���� ������ �̵��� �Ұ����ϰ� �Ѵ�.
			if(true ==  pclClient->pclServerEventManager->IsEventMap(siToMapIndex))
				return false;
		}

		//���� ���� �޽��� : �� ���� ������ �����Ǿ��ֽ��ϴ�
		if( pclClient->pclMapManager->IsBlockWarp( siToMapIndex ) == true )
		{		
			pclserver->SendServerResponseMsg(0,SRVAL_FAIL_WARP_BLOCKWARP,0,0, GetCharID());
			return false;
		}

		// �ʺ��� ������ �ʺ��ڰ� �ƴϸ� �� �� ����.
		if ( siToMapIndex == MAPINDEX_BEGINNER1 && pclCI->clIP.GetLevel() > 10 )
		{
			pclserver->SendServerResponseMsg(0, SRVAL_TOOHIGHLEVEL,  MAX_LEVEL_FOR_BEGINNERMAP, 0, GetCharUnique());
			return false;
		}
		if ( siToMapIndex == MAPINDEX_FESTIVAL ) 
		{
			pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_RACCOONWORLD, 0, 0, GetCharUnique() );
			return false ;	
		}	// �ش� ������ ������ ������ Ƽ������ ���°� ���� 

		// [��ȣ_NHN->NDOORS] ���ɺ� ����
		// PK���񽺸� ���� ������ PK������ �����ؼ� �� �� ����.

		if(pclClient->siServiceArea == ConstServiceArea_Korea)
		{
			SI32 siCharID = GetCharID();

			if(pclserver->pclCM->IsValidID(siCharID) == false)
				return false;
			
			if(pclClient->pclCM->CR[siCharID]->pclCI->clBI.m_uiRealAge < 18 && pclClient->pclMapManager->IsPkMap( siToMapIndex ))
			{
				pclserver->SendServerResponseMsg(0,SRVAL_FAIL_WARP_BLOCKWARP,0,0, GetCharID());
				return false;
			}
		}
		else
		{
			if( pclClient->siEnablePVP == 0 &&	pclClient->pclMapManager->IsPkMap( siToMapIndex ) )
			{
				pclserver->SendServerResponseMsg(0,SRVAL_FAIL_WARP_BLOCKWARP,0,0, GetCharID());
				return false;
			}
		}

		// �������� ������ �� �� ����.
		if(pclClient->pclMapManager->IsEnterLevel(siToMapIndex, pclCI->clIP.GetLevel()) == false)
		{
			// �ش���� ���� LvX~LvY������ �� �� �ֽ��ϴ�.
			pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_LEVEL_RANGE, 
				pclClient->pclMapManager->GetMinLevel(siToMapIndex), 
				pclClient->pclMapManager->GetMaxLevel(siToMapIndex),
				GetCharUnique());
			return false;
		}

		// [����] ���� ������ �� ���¿��� ������ �� �� �ִ� �������� ������ �� �� ���� �������� �̵��ϴ� ���̶�� ���´�
		if ( (pclClient->pclMapManager->IsCanMarketOpenMap( GetMapIndex() ) == true)
			&& (pclClient->pclMapManager->IsCanMarketOpenMap( siToMapIndex ) == false) )
		{
			SI32 siCharID = GetCharID();

			if ( pclserver->pclCM->IsValidID( siCharID ) == true )
			{
				if ( (pclserver->pclCM->CR[siCharID]->bPersonalShopSwitch == true)
					|| (pclserver->pclCM->CR[siCharID]->bPremiumPersonalShop == true)
					)
				{
					pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_WARPTO_NOTMARKETOPEN_MAP, 0, 0, siCharID );
					return false;
				}
			}
		}

		//================================================================
		// ��Ʋ�ξ�
		//================================================================
		CBattleRoyalMgr* pclBattleRoyalMgr = pclserver->m_pclBattleRoyalMgr;
		CBattleRoyal* pclBattleRoyal = NULL;
		cltCharServer* pclCharServer = (cltCharServer*)this;
		if( NULL != pclserver->m_pclBattleRoyalMgr)
		{
			// �ܺο��� ��������� �����ϱ� �Ұ���
			if( ( MAPINDEX_BATTLEROYAL_MABRIUM == siToMapIndex) || ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == siToMapIndex) )
			{
				return false;
			}

			pclBattleRoyal = pclBattleRoyalMgr->GetBattleRoyal(pclCharServer);
			if(NULL != pclBattleRoyal)
			{
				if( (MAPINDEX_BATTLEROYAL_MABRIUM == GetMapIndex()) || ( MAPINDEX_BATTLEROYAL_GRANDMABRIUM == GetMapIndex()))
				{
					if(pclBattleRoyal->IsGamingState())
						return false;
				}
			}			
		}
		if(siToMapIndex	==	MAPINDEX_FESTIVAL)
		{
			pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_GORACCOONWORLD_WARP, 0, 0,  GetCharID() );
			return false;
		}

		//[����] ��Ƽ�����δ� �̵� �Ұ���.
		/*if( true == pclClient->pclFeastManager->IsPartyHallMap(siToMapIndex) )
		{
			return false;
		}*/

		if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
		{
			if ( pclClient->pclMapManager->IsPkMap(siToMapIndex))
			{
				pclserver->SendServerResponseMsg( 0, SRVAL_FAIL_GORACCOONWORLD_WARP, 0, 0,  GetCharID() );	// �ش��������δ� ���� �Ҽ� �����ϴ�.
				return false	;	// pk ������ ���� �Ұ��� 
			}
		}
	}

	// �δ��϶� ������ �� Ÿ�Կ� ���� �ٸ�
	if( pclClient->pclMapManager->IsInstanceMap(siToMapIndex) )
	{
		if(pclClient->pclMapManager->IsInstanceMap(GetMapIndex()) == false)
		{
			return false;
		}
		
		switch ( pclClient->pclMapManager->GetInstanceMapType(siToMapIndex) ) 
		{
			// ������ �δ� �ȿ����� ������ �����ϴ�.
			case INSTANCEMAP_TYPE_GUILD:
				{
					SI32 siTartgetParentMap		= pclClient->pclMapManager->GetParentMap( siToMapIndex );
					SI32 siSrcParentMap			= pclClient->pclMapManager->GetParentMap( GetMapIndex() );

					// ���� ���� �ƴ϶�� ������ �Ұ��� �ϴ�.
					if( siSrcParentMap != siTartgetParentMap )			return false;
				}
				break;
			
			//  ��Ƽ�ʿ����� �����Ұ���
			case INSTANCEMAP_TYPE_PARTY:
				{
					return false;
				}
				break;
			
			// ���� �̺�Ʈ �㿡���� �����Ұ���
			case INSTANCEMAP_TYPE_SERVER:
				{
					return false;
				}
				break;
		}
	}
	return true;
}

// ��¥ ������ ����� �� �ִ°�?
bool cltCharCommon::CanFreeWarp( SI32 siDestMapIndex )
{	
	//-------------------------------------------------------------------
	// �⺻ üũ
	//-------------------------------------------------------------------

	// �Ⱓ�� ���� Ƽ�� üũ
	if (pclCI->clBI.bUseNoLimitTicketWarp == true)
		return true;

	// �Ⱓ�� ���� Ƽ�� ��ȿ�Ⱓ üũ
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if( pclCI->clBI.siNoLimitTicketWarpDateVary > 0 && 
			pclCI->clBI.siNoLimitTicketWarpDateVary < ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() )
			return false;
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		if( pclCI->clBI.siNoLimitTicketWarpDateVary > 0 && 
			pclCI->clBI.siNoLimitTicketWarpDateVary < ((cltClient*)pclClient)->clClientDate.GetDateVary() )
			return true;
	}

	// �����̸� ������ ��¥
	if ( pclCI->clBI.uiPoliceSwitch == 1 )
	{
		if(pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique == ITEMUNIQUE(23490))
		{
			if( pclClient->GameMode == GAMEMODE_SERVER )
			{
				//((cltCharServer*)this)->DecreaseEquipDur(PERSONITEM_HAT, 5);
			}
			return true;
		}
	}

	// ��ȣõ���̸� ������ ��¥
	if ( pclCI->clBI.uiAngelSwitch == 1 )
	{
		if( pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique == ITEMUNIQUE(23485))
		{
			if( pclClient->GameMode == GAMEMODE_SERVER )
			{
				//((cltCharServer*)this)->DecreaseEquipDur(PERSONITEM_HAT, 5);
			}
			return true;
		}
	}


	// �����̸� ������ ��¥
	if ( pclCI->clBI.uiGRank == GRANK_KING )
	{
		if( pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23500) ||
			pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23505) )
		{
			return true;
		}
	}

	// ����̸� ������ ��¥
	if ( pclCI->clBI.uiGRank == GRANK_MINISTER1 ||
		 pclCI->clBI.uiGRank == GRANK_MINISTER2 ||
		 pclCI->clBI.uiGRank == GRANK_MINISTER3 ||
		 pclCI->clBI.uiGRank == GRANK_MINISTER4 ||
		 pclCI->clBI.uiGRank == GRANK_MINISTER5 ||
		 pclCI->clBI.uiGRank == GRANK_MINISTER6 )
	{				
		if(	pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23510) )		
		{
			return true;
		}
	}

	// �����߿� ��������� ���������� ���ÿ� �ɷ�������, ���� ��¥
	if( pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) && pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP ) )
	{
		return true;
	}

	// PC�� �����̾� ���� - ���� ��¥
	if ( pclCI->clBI.bPCRoom == true )
	{
		return true;
	}

	//[�߰� : Ȳ���� 2008. 1. 15 => �ְ����� ������ ��¥~]
	if(1 == uiBestGuild)
	{
		return true;
	}
	// ������ �ְ�, �ʾ� �ִ� ��� -> �Ѿ縸 ��¥ ���� ����
	if( IsHaveCushionItem() > 0 && GetCurrentOrder() == ORDER_SITDOWN )
	{
		if( siDestMapIndex == MAPINDEX_PALACE )
		{
			// �������� �Ѿ��̸�, ��¥���� ����
			return true;
		}
	}

	// [����] ���縮�� �̵��ϱ� ����Ʈ ����� ��¥ ����
	SI32 siQuestClearCond	= QUEST_CLEAR_COND_WARPTOPALACE;
	SI16 siQuestType		= 0;
	SI16 siQuestUnique		= 0;
	SI16 siPara1			= 0;
	SI16 siPara2			= 0;


	if ( pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2) == true )
	{
		if ( siDestMapIndex == MAPINDEX_PALACE ) 
		{
			// �Ѿ����� ������ ����Ʈ�� �������� ������.
			DoQuestCheckAndAction( siQuestClearCond );
			return true;
		}
		
	}

	// [����] ���ڴ� ���� : ���ڴ��̸� ������ ��¥ (2008. 06 .05)
	if ( CONST_JOURNALIST_HAT_ITEMUNIQUE == pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique )
	{
		return true;
	}

	// �̺�Ʈ ���� ��¥ ��� - �ڽ��� ���� ������ ��¥�� ���� ����
	// Ŭ���̾�Ʈ�� ������ üũ�ϴ� �ڵ尡 Ʋ����.
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		cltStrInfo* pclinfo = pclClient->pclVillageManager->pclCityHallManager->GetStrInfo( pclCI->clBI.siHomeVillage );
		if( pclinfo != NULL )
		{
			for (SI32 i = 0; i <MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if( pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))    == pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind ||
					//pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")) == pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind ||
					pclClient->GetUniqueFromHash(TEXT("KIND_RETURNTOWER")) == pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind )	
				{
					// �ڽ��� ���� ������ ��¥�� ���� ����
					if( pclCI->clBI.siHomeVillage == siDestMapIndex )
					{
						return true;
					}
				}
			}
		}
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		cltCharClient* pclcharclient = (cltCharClient*)this;
		
		for (SI32 i = 0; i <MAX_HIRED_EVENTNPC_NUM; i++)
		{
			if( pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))    == pclcharclient->siEventNPCKind[i] ||
				//pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")) == pclcharclient->siEventNPCKind[i] ||
				pclClient->GetUniqueFromHash(TEXT("KIND_RETURNTOWER")) == pclcharclient->siEventNPCKind[i] )	
			{
				// �ڽ��� ���� ������ ��¥�� ���� ����
				if( pclCI->clBI.siHomeVillage == siDestMapIndex )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool cltCharCommon::CanWarpToChar( SI32 siCharID )
{
	if( pclClient->GameMode != GAMEMODE_SERVER)			return false;
	if( pclClient->pclCM->IsValidID(siCharID) == false)	return false;

	cltServer* pclserver = (cltServer*)pclClient;
	cltCharServer* pclcharTo = (cltCharServer*)pclClient->pclCM->CR[siCharID];

	// ���� ĳ�������״� ���� �� �� ����.
	if ( pclClient->pclCM->IsAlive(siCharID) == false )		return false;

	if ( pclserver->IsCountrySwitch(Switch_NewGuildHuntMapWar)== true )	// ������ 
	{
		if ( IsGuildWarMap_Use_New_GuildWar( pclCM->CR[siCharID]->GetMapIndex() )  )
		{
			BOOL				bInBattle = FALSE;
			cltGuildWarManager*	pclGuildWarManager = NULL;

			if ( pclserver->IsCountrySwitch(Switch_NewGuildHuntMapWar)== true )	// ������ 
			{
				// ������ �������� ������� ������ ã�´�.
				for(SI32 i=0; i<GUILDWAR_FIELDNUM_OLD; i++)
				{
					if(i >= GUILDWAR_FIELDNUM_OLD) continue	;
					if( pclserver->pclGuildWarManager[i]->m_siAwayGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
						pclserver->pclGuildWarManager[i]->m_siHomeGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() )
					{
						pclGuildWarManager = pclserver->pclGuildWarManager[i];
					}
				}
			}
			else	// ������ 
			{
				// ������ �������� ������� ������ ã�´�.
				for(SI32 i=0; i<GUILDWAR_FIELDNUM; i++)
				{
					if(i >= GUILDWAR_FIELDNUM) continue	;
					if( pclserver->pclGuildWarManager[i]->m_siAwayGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
						pclserver->pclGuildWarManager[i]->m_siHomeGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() )
					{
						pclGuildWarManager = pclserver->pclGuildWarManager[i];
					}
				}
			}

			// ������￡ �츮 ��尡 �������ΰ�?
			if ( pclGuildWarManager != NULL &&
				( pclGuildWarManager->m_siAwayGuildUnique == pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
				pclGuildWarManager->m_siHomeGuildUnique == pclCI->clBI.clGuildRank.GetGuildUnitIndex()) )
			{
				bInBattle = TRUE;
			}

			// ���￡ �������� ������ �ƴϹǷ� ���忡 �� �� ����.
			if (bInBattle == FALSE)
			{
				return false;
			}
		}


	}
	else
	{
		if ( IsGuildWarMap_Not_Use_New_GuildWar( pclCM->CR[siCharID]->GetMapIndex() )  )
		{
			BOOL				bInBattle = FALSE;
			cltGuildWarManager*	pclGuildWarManager = NULL;

			if ( pclserver->IsCountrySwitch(Switch_NewGuildHuntMapWar)== true )	// ������ 
			{
				// ������ �������� ������� ������ ã�´�.
				for(SI32 i=0; i<GUILDWAR_FIELDNUM_OLD; i++)
				{
					if(i >= GUILDWAR_FIELDNUM_OLD) continue	;
					if( pclserver->pclGuildWarManager[i]->m_siAwayGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
						pclserver->pclGuildWarManager[i]->m_siHomeGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() )
					{
						pclGuildWarManager = pclserver->pclGuildWarManager[i];
					}
				}
			}
			else	// ������ 
			{
				// ������ �������� ������� ������ ã�´�.
				for(SI32 i=0; i<GUILDWAR_FIELDNUM; i++)
				{
					if(i >= GUILDWAR_FIELDNUM) continue	;
					if( pclserver->pclGuildWarManager[i]->m_siAwayGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
						pclserver->pclGuildWarManager[i]->m_siHomeGuildUnique == pclcharTo->pclCI->clBI.clGuildRank.GetGuildUnitIndex() )
					{
						pclGuildWarManager = pclserver->pclGuildWarManager[i];
					}
				}
			}

			// ������￡ �츮 ��尡 �������ΰ�?
			if ( pclGuildWarManager != NULL &&
				( pclGuildWarManager->m_siAwayGuildUnique == pclCI->clBI.clGuildRank.GetGuildUnitIndex() ||
				pclGuildWarManager->m_siHomeGuildUnique == pclCI->clBI.clGuildRank.GetGuildUnitIndex()) )
			{
				bInBattle = TRUE;
			}

			// ���￡ �������� ������ �ƴϹǷ� ���忡 �� �� ����.
			if (bInBattle == FALSE)
			{
				return false;
			}
		}


	}
	// ���ο� ����� �� ���� ���ΰ�
	if ( IsGuildWarMap_NEW( pclCM->CR[siCharID]->GetMapIndex() )  )	// ������ ������ʿ� �ִ�.
	{
		if ( pclserver->IsCountrySwitch(Switch_NewGuildHuntMapWar) == true )	// �� ������� ���� �̷�ƾ�� ź��.
		{
			BOOL				bInBattle = FALSE;
			cltGuildWarManager*	pclGuildWarManager = NULL;

			SI32 siMyGuildindex = pclCI->clBI.clGuildRank.GetGuildUnitIndex()	;
			if(GUILDWAR_HOMEGUILD == pclserver->pclGuildHuntMapManager->IsHomeGuildorAwayGuild(  siMyGuildindex )  ||
				GUILDWAR_AWAYGUILD == pclserver->pclGuildHuntMapManager->IsHomeGuildorAwayGuild( siMyGuildindex ) )// Ȩ���� �������̴�
			{
				bInBattle = TRUE;
			}
			// ���￡ �������� ������ �ƴϹǷ� ���忡 �� �� ����.
			if (bInBattle == FALSE)
			{
				return false;
			}

		}
	}

	// ������ �߿��� ��븶�� ĳ���Ϳ��� ���� �� �� ����.
	if( pclClient->bVillageWarSwitch == true )
	{
		if(pclClient->pclVillageManager->IsAggressiveVillage(pclCI->clBI.siHomeVillage, pclcharTo->pclCI->clBI.siHomeVillage) == TRUE)
		{
			return false;
		}
	}

	// Ÿ���� �ν��Ͻ� ������ �ִ� ���
	if( pclClient->pclMapManager->IsInstanceMap(pclcharTo->GetMapIndex()) == true )
	{
		// ĳ���Ͱ� �ν��Ͻ� ������ �־�� ������ �����ϴ�.
		if( pclClient->pclMapManager->IsInstanceMap( GetMapIndex() ) == false )		return false;
		
		SI32 siTartgetParentMap = pclClient->pclMapManager->GetParentMap( pclcharTo->GetMapIndex() );
		SI32 siSrcParentMap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );

		// ���� ���� �ƴ϶�� ������ �Ұ��� �ϴ�.
		if( siSrcParentMap != siTartgetParentMap )			return false;
	}

	// �������� ������ �� �� ����.
	if(pclClient->pclMapManager->IsEnterLevel(pclcharTo->GetMapIndex(), pclCI->clIP.GetLevel()) == false)
	{
		// �ش���� ���� LvX~LvY������ �� �� �ֽ��ϴ�.
		pclserver->SendServerResponseMsg(0, SRVAL_FAIL_WARP_LEVEL_RANGE, 
											pclClient->pclMapManager->GetMinLevel(pclcharTo->GetMapIndex()), 
											pclClient->pclMapManager->GetMaxLevel(pclcharTo->GetMapIndex()),
											GetCharUnique());
		return false;
	}

	return true;
}


void cltCharCommon::PartyHallItemRemove( SI32 id )
{
	// �� �ڵ�� ���������� ���� ����
	if ( GAMEMODE_SERVER != pclClient->GameMode )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;

	//[����] ��ƼȦ. ��Ƽ�忡�� ���� ������ ������ ����. => 2008. 3. 12
	UI32 GetItemReason	= 0;
	SI32 InvenCount		= 0;

	// ĳ���� ����â + �κ��丮�� �ִ� ������ ����
	for(InvenCount = 0; InvenCount < MAX_ITEM_PER_PERSON; ++InvenCount)	
	{
		GetItemReason = pclCI->clCharItem.clItem[InvenCount].clItemCommon.clCommonInfo.uiGetItemReason;
		if( cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM == GetItemReason )
		{
			// ������ ����.  
			sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclCI->clCharItem.clItem[InvenCount], USEITEM_REASON_PATYHALL);
			pclserver->pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
		}
	}

    // �������ڿ� ����ִ� �����۵� ����	
	//cltSpaceBox* pclSpaceBoxitem = &pclCI->clSpaceBoxItem;
	cltSpaceBox* pclSpaceBoxitem = pclSpaceBoxItem;
	if(pclSpaceBoxitem)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_SPACE_BOX; ++InvenCount)	
		{
			GetItemReason = pclSpaceBoxitem->clItem[InvenCount].clItemCommon.clCommonInfo.uiGetItemReason;
			if( cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM == GetItemReason )
			{
				// ������ ����.  
				sDBRequest_Use_SpaceBox_Item useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), InvenCount, &pclSpaceBoxitem->clItem[InvenCount], USEITEM_REASON_DATEOVER);
				pclserver->pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
			}
		}
	}
}

//[����] ����ũ�ι��� ���� ����Ҽ� ���� �Ÿ� ���� => 2008-8-14
void cltCharCommon::NotFourLeafItemRemove( SI32 id )
{
	// �� �ڵ�� ���������� ���� ����
	if ( GAMEMODE_SERVER != pclClient->GameMode )
	{
		return;
	}
	cltServer* pclserver = (cltServer*)pclClient;

	UI32 GetItemReason	= 0;
	SI32 InvenCount		= 0;

	// ĳ���� ����â + �κ��丮�� �ִ� ������ ����
	for(InvenCount = 0; InvenCount < MAX_ITEM_PER_PERSON; ++InvenCount)	
	{
		if( ITEMUNIQUE(2755) == pclCI->clCharItem.clItem[InvenCount].siUnique || 
			ITEMUNIQUE(2754) == pclCI->clCharItem.clItem[InvenCount].siUnique	)	
		{	
			if( pclCI->clCharItem.clItem[InvenCount].siUnique != pclserver->siFourLeafUnique )	
			{
				// ������ ����.  
				sDBRequest_UseItem useitem(GetCharID(), pclCI->GetPersonID(), InvenCount, &pclCI->clCharItem.clItem[InvenCount], USEITEM_REASON_PATYHALL);
				pclserver->pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
			}
		}
	}

	// �������ڿ� ����ִ� �����۵� ����	
	//cltSpaceBox* pclSpaceBoxitem = &pclCI->clSpaceBoxItem;
	cltSpaceBox* pclSpaceBoxitem = pclSpaceBoxItem;
	if(pclSpaceBoxitem)
	{
		for(InvenCount = 0; InvenCount < MAX_ITEM_SPACE_BOX; ++InvenCount)	
		{
			if( ITEMUNIQUE(2755) == pclSpaceBoxitem->clItem[InvenCount].siUnique || 
				ITEMUNIQUE(2754) == pclSpaceBoxitem->clItem[InvenCount].siUnique	)	
			{	
				if( pclSpaceBoxitem->clItem[InvenCount].siUnique != pclserver->siFourLeafUnique )	
				{
					// ������ ����.  
					sDBRequest_Use_SpaceBox_Item useitem(GetCharID(), pclCI->GetPersonID(), InvenCount, &pclSpaceBoxitem->clItem[InvenCount], USEITEM_REASON_DATEOVER);
					pclserver->pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
				}
			}
		}
	}
}

//[����] ���� ���� ����
void cltCharCommon::FestivalFoodItemRemove( SI32 id )
{
	cltServer* pclserver = (cltServer*)pclClient;
	// ĳ���� ����â + �κ��丮�� �ִ� ������ ����
	for( SI32 InvenCount = 0; InvenCount < MAX_ITEM_PER_PERSON; ++InvenCount )	
	{
		if( ITEMUNIQUE(3934) == pclCI->clCharItem.clItem[InvenCount].siUnique || 
			ITEMUNIQUE(3931) == pclCI->clCharItem.clItem[InvenCount].siUnique ||
			ITEMUNIQUE(3930) == pclCI->clCharItem.clItem[InvenCount].siUnique ||
			ITEMUNIQUE(3929) == pclCI->clCharItem.clItem[InvenCount].siUnique ||
			ITEMUNIQUE(3932) == pclCI->clCharItem.clItem[InvenCount].siUnique ||
			ITEMUNIQUE(3933) == pclCI->clCharItem.clItem[InvenCount].siUnique 	)	
		{	
			if( pclCI->clCharItem.clItem[InvenCount].siUnique != pclserver->siFourLeafUnique )	
			{
				// ������ ����.  
				sDBRequest_UseItem useitem( GetCharID(), pclCI->GetPersonID(), InvenCount, &pclCI->clCharItem.clItem[InvenCount], USEITEM_REASON_FESTIVAL_FOOD );
				pclserver->pclGameDBProcess->SendMsg( DBSELECT_BASE, (sPacketHeader *)&useitem );
			}
		}
	}
}

//==================================================
// cltShopTradeData
//==================================================
cltShopTradeData::cltShopTradeData()
{
	memset( this, 0, sizeof( cltShopTradeData ) );
}

void cltShopTradeData::Init()
{
	memset( this, 0, sizeof( cltShopTradeData ) );
}

void cltShopTradeData::InitData()
{
#ifdef _SAFE_MEMORY
	m_clItem.ZeroMem();					
	m_siSelectedInventoryPos.ZeroMem();	
	m_siPrice.ZeroMem();					
	m_siCurrentPrice.ZeroMem();			
#else
	memset( m_clItem,					0,	sizeof(m_clItem)				);
	memset( m_siSelectedInventoryPos,	0,	sizeof(m_siSelectedInventoryPos));
	memset( m_siPrice,					0,	sizeof(m_siPrice)				);
	memset( m_siCurrentPrice,			0,	sizeof(m_siCurrentPrice)		);
#endif
	memset( m_szShopTitle,				0,	sizeof(m_szShopTitle)			);
	m_siHandPoint  = 0;
	m_siSkillLevel = 0;
	m_bPremium = false;
}

void cltShopTradeData::Set(cltShopTradeData *pclData)
{
	m_siShopMode		= pclData->m_siShopMode;
	m_siPersonID		= pclData->m_siPersonID;
	MStrCpy(m_szAccountID, pclData->m_szAccountID,MAX_PLAYER_NAME);
	MStrCpy(m_szShopTitle, pclData->m_szShopTitle,MAX_SHOP_TITLE_LENGTH);
	for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
	{
		m_clItem[i].Set(&pclData->m_clItem[i]);
		m_siSelectedInventoryPos[i]= pclData->m_siSelectedInventoryPos[i];
		m_siPrice[i]		= pclData->m_siPrice[i];
		m_siCurrentPrice[i] = pclData->m_siCurrentPrice[i];
	}
	m_siHandPoint  = pclData->m_siHandPoint;
	m_siSkillLevel = pclData->m_siSkillLevel;
	m_bPremium	= pclData->m_bPremium;

	m_clSpacialItem.Set(&pclData->m_clSpacialItem);									// �̰� �̿�� �������� ��ϵȴ�.
	m_siSelectedSpacialInventoryPos = pclData->m_siSelectedSpacialInventoryPos;					// ��ġ�� ��ϵȴ�.
}

void cltShopTradeData::SetCurrentPrice(cltItemManagerCommon *pclitemmanager)
{
#ifdef _SAFE_MEMORY
	m_siCurrentPrice.ZeroMem();
#else
	memset( m_siCurrentPrice,		  0, sizeof(m_siCurrentPrice));
#endif
	//-------------------------------------------
	// ���� �ü��� �����Ѵ�.
	//-------------------------------------------
	for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
	{
		if(m_clItem[i].siUnique)
		{
			cltItemPriceUnit* pclprice = pclitemmanager->GetCurrentPrice(m_clItem[i].siUnique);
			if(pclprice) m_siCurrentPrice[i] = pclprice->siPrice;
		}
	}
}

//==================================================
// cltShopBannerData
//==================================================
cltShopBannerData::cltShopBannerData()
{
	memset( this, 0, sizeof( cltShopBannerData ) );
}

void cltShopBannerData::Init()
{
	memset( this, 0, sizeof( cltShopBannerData ) );
}

void cltShopBannerData::Set(cltShopBannerData *pclBanner)
{
	memcpy(this, pclBanner, sizeof(cltShopBannerData));							//ok
}

void cltShopBannerData::SetData(cltShopTradeData *pclData)
{
	m_siShopMode = pclData->m_siShopMode;
	for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i)
	{
		if(pclData->m_clItem[i].siItemNum > 0)
		{
			m_siUnique[i] = pclData->m_clItem[i].siUnique;
			m_siPrice[i]  = pclData->m_siPrice[i];
		}
		else
		{
			m_siUnique[i] = 0;
			m_siPrice[i]  = 0;
		}
	}
	MStrCpy(m_szShpoTitle,pclData->m_szShopTitle,MAX_SHOP_TITLE_LENGTH);
	m_siHandPoint = pclData->m_siHandPoint; 
	m_siSkillLevel= pclData->m_siSkillLevel;
	m_bPremium = pclData->m_bPremium;
}

void cltShopBannerData::MakeBannerText(cltItemManagerCommon* pclItemManager, TCHAR *ptext,TCHAR *pShopTitle)
{
	TCHAR szExplain[MAX_EXPLAIN_LENTH];

	StringCchCopy(szExplain, MAX_EXPLAIN_LENTH, TEXT(""));

	if(m_bPremium == true)
	{
		if( NULL != pShopTitle )
		{
			StringCchCopy(pShopTitle,MAX_SHOP_TITLE_LENGTH,m_szShpoTitle);
		}
	}

	TCHAR* pText = GetTxtFromMgr(3323);

	if(m_siShopMode == PSHOP_MODE_BUY)		
	{
		pText = GetTxtFromMgr(3323);
		StringCchCat(szExplain, MAX_EXPLAIN_LENTH, pText);				
	}

	else if(m_siShopMode == PSHOP_MODE_SELL)
	{
		pText = GetTxtFromMgr(3324);
		StringCchCat(szExplain, MAX_EXPLAIN_LENTH, pText);		
	}
	else if(m_siShopMode == PSHOP_MODE_MAKEITEM)	
	{
		pText = GetTxtFromMgr(3809);
		StringCchCat(szExplain, MAX_EXPLAIN_LENTH, pText);		
	}
	else if(m_siShopMode == PSHOP_MODE_MAKEITEM_WANT)	//Switch_Change_manufacture_request_interface	[����] ���� �Ƿ� �������̽� ����
	{
		pText = GetTxtFromMgr(40829);
		StringCchCat(szExplain, MAX_EXPLAIN_LENTH, pText);		
	}


	TCHAR szMoney[256];
	TCHAR buffer[256];
	SI32 siCount = 0;
	if( m_siShopMode == PSHOP_MODE_BUY ||
		m_siShopMode == PSHOP_MODE_SELL)
	{
		for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i)
		{
			if(m_siUnique[i])
			{
				TCHAR *pName = (TCHAR *)pclItemManager->GetName(m_siUnique[i]);

				if(pName)
				{
					
						StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("\r\n"));						
				

					TCHAR* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(m_siPrice[i], szMoney, 256, pText);
					g_JYLibFunction.SetNumUnit(m_siPrice[i], buffer, 256, pText);
					size_t buffersize, szMoneysize;
					StringCchLength(buffer, 256, &buffersize);
					StringCchLength(szMoney, 256, &szMoneysize);

					// Japan�� �ƴҰ�� ������ ���� ��¾���
					if(buffersize < szMoneysize || 
						pclClient->siServiceArea != ConstServiceArea_Japan)	
					{
						StringCchCopy(szMoney, 256, buffer);
					}
					
					TCHAR namebuffer[256] = TEXT("");
					StringCchCopy( namebuffer, 256, pName);

					if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_EUROPE) )
					{
						// �ʹ� ��� �̻��ϰ� �����Ƿ�.. 15�� �̻��� �߶��ش�.
						size_t nameSize = 0;
						StringCchLength(namebuffer, 256, &nameSize);
						if (  nameSize > 14 )
						{
							for( SI16 j=13; j<(int)nameSize ; j++ )
							{
								if ( j < 15 ) namebuffer[j]=TEXT('.');
								else
									namebuffer[j] = 0;
							}

						}
					}
					/*
                    if( pclClient->siServiceArea == ConstServiceArea_Japan)
					{
						// �Ϻ��� �ʹ� ��� ©���Ƿ� 5�� �̻���  �߶��ش�.
						size_t siStringLen = 0;
							
						StringCchLength(namebuffer,256, &siStringLen );
						
						if( siStringLen > 5 )
						{
							TCHAR tempbuffer[256] = {'\0',};
							StringCchCopyN( tempbuffer ,256,namebuffer,5);
							StringCchCat(tempbuffer,256,TEXT(".."));  
							StringCchCopy(namebuffer,256,tempbuffer);
						}

					}*/

					StringCchPrintf(buffer, 256, TEXT("%s:%s"), namebuffer, szMoney);
					StringCchCat(szExplain, MAX_EXPLAIN_LENTH, buffer);
					siCount++;
				}
			}
		}
	}
	else if(m_siShopMode == PSHOP_MODE_MAKEITEM)
	{
		for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i)
		{
			if(m_siUnique[i])
			{
				TCHAR *pName = (TCHAR *)pclItemManager->GetName(m_siUnique[i]);
				if(pName)
				{
					if(siCount==0)	StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("\r\n"));
					else
					{
						if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_EUROPE) ) 
						{
							StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("\r\n"));
						}
						else
						{
							StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("/"));
						}
					}
					TCHAR* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(m_siPrice[i], szMoney, 256, pText);
					g_JYLibFunction.SetNumUnit(m_siPrice[i], buffer, 256, pText);
					if(_tcslen(buffer)<_tcslen(szMoney)|| 
						pclClient->siServiceArea != ConstServiceArea_Japan)	
						StringCchCopy(szMoney, 256, buffer);


					{
						TCHAR* pText = NULL;
						if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
						{
							pText = GetTxtFromMgr(9864);
						}
						else
						{
							if ( pclClient->IsCountrySwitch(Switch_Change_manufacture_request_interface) )
							{
								pText = GetTxtFromMgr(9864);
							}
							else
							{
								pText = GetTxtFromMgr(3512);
							}
						}

						StringCchPrintf(buffer, 256, pText, 
							pName, szMoney, m_siSkillLevel ,m_siHandPoint);
						StringCchCat(szExplain, MAX_EXPLAIN_LENTH, buffer);
						siCount++;
					}
				}
			}
		}
	}


	//Switch_Change_manufacture_request_interface	[����] ���� �Ƿ� �������̽� ����
	else if(m_siShopMode == PSHOP_MODE_MAKEITEM_WANT)
	{
		for(SI32 i = 0; i <MAX_SHOP_ITEM_NUMBER; ++i)
		{
			if(m_siUnique[i])
			{
				TCHAR *pName = (TCHAR *)pclItemManager->GetName(m_siUnique[i]);
				if(pName)
				{
					if(siCount==0)	StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("\r\n"));
					else
					{
						if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_EUROPE) ) 
						{
							StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("\r\n"));
						}
						else
						{
							StringCchCat(szExplain, MAX_EXPLAIN_LENTH, TEXT("/"));
						}
					}
					TCHAR* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(m_siPrice[i], szMoney, 256, pText);
					g_JYLibFunction.SetNumUnit(m_siPrice[i], buffer, 256, pText);
					if(_tcslen(buffer)<_tcslen(szMoney)|| 
						pclClient->siServiceArea != ConstServiceArea_Japan)	
						StringCchCopy(szMoney, 256, buffer);


					{
						TCHAR* pText = NULL;
						if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
						{
							pText = GetTxtFromMgr(9864);
						}
						else
						{
							if ( pclClient->IsCountrySwitch(Switch_Change_manufacture_request_interface) )
							{
								pText = GetTxtFromMgr(9864);
							}
							else
							{
								pText = GetTxtFromMgr(3512);
							}
						}

						StringCchPrintf(buffer, 256, pText, 
							pName, szMoney, m_siSkillLevel ,m_siHandPoint);
						StringCchCat(szExplain, MAX_EXPLAIN_LENTH, buffer);
						siCount++;
					}
				}
			}
		}
	}


	
	MStrCpy( ptext, szExplain, MAX_PERSONAL_SHOP_TEXT_SIZE );
}

//==================================================
// cltShopRepairData
//==================================================
cltShopRepairData::cltShopRepairData()
{
	memset( this, 0, sizeof( cltShopRepairData ) );
}

void cltShopRepairData::Init()
{
	memset( this, 0, sizeof( cltShopRepairData ) );
}

void cltShopRepairData::InitData()
{
	m_siPersonID			= 0;
	m_szAccountID[0]		= '\0';

	m_clRepairItem.Init();
	m_siRepairItemPos		= 0;
	m_siPrice				= 0;

	for( SI32 i=0; i<m_kMaterial.GetSize(); i++ )
	{
		m_kMaterial[i].Init();
	}
}

void cltShopRepairData::Set(cltShopRepairData *pclData)
{
	m_siPersonID			= pclData->m_siPersonID;
	StringCchCopy( m_szAccountID, sizeof(m_szAccountID), pclData->m_szAccountID );

	m_clRepairItem.Set( &pclData->m_clRepairItem );
	m_siRepairItemPos		= pclData->m_siRepairItemPos;
	m_siPrice				= pclData->m_siPrice;

	for( SI32 i=0; i<m_kMaterial.GetSize(); i++ )
	{
		m_kMaterial[i].Set( &pclData->m_kMaterial[i] );
	}
}

bool cltShopRepairData::ExistItem( SI16 siPos, cltItem* pclitem )
{
	if( pclitem == NULL )						return false;
	// Rapair Item Check
	if( m_siRepairItemPos == siPos &&
		m_clRepairItem.siUnique == pclitem->siUnique )
	{
		return true;
	}

	for( SI32 i=0; i<m_kMaterial.GetSize(); i++ )
	{
		if( m_kMaterial[i].siStgPos == siPos &&
			m_kMaterial[i].siItemUnique == pclitem->siUnique )
		{
			return true;
		}
	}

	return false;
}

//==================================================
// cltSearchShopData
//==================================================
cltSearchShopData::cltSearchShopData()
{
	memset( this, 0, sizeof( cltSearchShopData ) );
}

void cltSearchShopData::Init()
{
	memset( this, 0, sizeof( cltSearchShopData ) );
}

void cltSearchShopData::CopyGeneralData(cltSearchShopData *pclData)
{
	m_siShopMode  = pclData->m_siShopMode;
	m_siItemUniqueType  = pclData->m_siItemUniqueType;
	m_siItemSearchPrice = pclData->m_siItemSearchPrice;
}

void cltSearchShopData::CopyData(SI32 index1, SI32 index2, cltSearchShopData *pclData)
{
	if(pclData == NULL)		return;
	if(index1 < 0 || index1 >= MAX_SEARCHSHOP_ITEM_NUMBER)		return;
	if(index2 < 0 || index2 >= MAX_SEARCHSHOP_ITEM_NUMBER)		return;

	MStrCpy(m_szName[index1], pclData->m_szName[index2], MAX_PLAYER_NAME);
	m_siLevel[index1]		= pclData->m_siLevel[index2];
	m_siMapIndex[index1]	= pclData->m_siMapIndex[index2];
	m_siMapX[index1]		= pclData->m_siMapX[index2];
	m_siMapY[index1]		= pclData->m_siMapY[index2];

	m_clItem[index1].Set(&pclData->m_clItem[index2]);
	m_siPrice[index1]		= pclData->m_siPrice[index2];

	m_siHandPoint[index1]	= pclData->m_siHandPoint[index2];
	m_siSkillLevel[index1]	= pclData->m_siSkillLevel[index2];
}

void cltSearchShopData::ClearData(SI32 index1)
{
	if(index1 < 0 || index1 >= MAX_SEARCHSHOP_ITEM_NUMBER)		return;

	StringCchCopy(m_szName[index1], MAX_PLAYER_NAME, TEXT(""));
	m_siLevel[index1]		= 0;
	m_siMapIndex[index1]	= 0;
	m_siMapX[index1]		= 0;
	m_siMapY[index1]		= 0;

	m_clItem[index1].Init();
	m_siPrice[index1]		= 0;
	m_siHandPoint[index1]	= 0;
	m_siSkillLevel[index1]	= 0;

}

void cltSearchShopData::MoveData(SI32 index1, SI32 index2, cltSearchShopData *pclData)
{
	CopyData(index1, index2, pclData);
	pclData->ClearData(index2);
}

void cltSearchShopData::Set(cltSearchShopData *pclData)
{
	CopyGeneralData(pclData);

	for(SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i)
	{
		CopyData(i, i, pclData);
	}
}

//==================================================
// cltBeHitInfo
//==================================================
cltBeHitInfo::cltBeHitInfo()
{
	Init();
}

void cltBeHitInfo::Init()
{
	bBeAttackedSwitch	= false;
	siAttackerID			= 0;
}

void cltBeHitInfo::Set(bool bhitswitch, SI16 attackerid)
{
	bBeAttackedSwitch	= bhitswitch;
	siAttackerID		= attackerid;
}

//==================================================
// cltJim
//==================================================
cltJim::cltJim()
{
	Init();
}

void cltJim::Init()
{
	siJimHostCharUnique		= 0;
	siJimSlaveCharUnique	= 0;
	dwJimFrame				= 0;
	dwNewJimFrame			= 0;
}

// ���ϴ�. 
bool cltJim::SetJim(SI32 slavecharunique)
{
	if(siJimSlaveCharUnique)return false;

	siJimSlaveCharUnique	= slavecharunique;

	return true;
}

// �� �Ǵ�. 
bool cltJim::SetBeJimed(SI32 hostcharunique, DWORD frame)
{
	// ���� �� ĳ���Ͱ� �ְ� �װ��� ���� �־��� ĳ���Ϳ� �ٸ��ٸ� 
	if(siJimHostCharUnique && siJimHostCharUnique != hostcharunique)return false;

	// ������ �� ���ΰ� ���� �ٸ� ���̶��.. 
	if(siJimHostCharUnique == 0)
	{
		dwNewJimFrame = frame;
	}

	siJimHostCharUnique		= hostcharunique;
	dwJimFrame				= frame;

	return true;
}

//==================================================
// cltCharListInfo
//==================================================
cltCharListInfo::cltCharListInfo()
{
}

void cltCharListInfo::Init()
{
	ZeroMemory(this, sizeof(cltCharListInfo));

	clBI.Init();
	clIP.Init();
}
void cltCharListInfo::Set(cltCharListInfo* pclinfo)
{
	clBI.Set(&pclinfo->clBI);
	clIP.Set(&pclinfo->clIP);

	/*
	for(SI32 i = 0; i < MAX_SHAPE_ITEM; ++i)
	{
		clCharItem[i].Set(&pclinfo->clCharItem[i]);
	}
	*/
}

//==================================================
// cltMovePointList
//==================================================
cltMovePointList::cltMovePointList()
{
	siPointNum = 0;
}

void cltMovePointList::Init()
{
	siPointNum = 0;
}

bool cltMovePointList::AddPos(SI32 six, SI32 siy)
{
	if(siPointNum < 0 || siPointNum >= MAX_CHAR_MOVE_POINT)return false;

	clPos[siPointNum].Set(six, siy);
	siPointNum++;

	return true;
}

// ù��° ������ �����ߴ°�?
bool cltMovePointList::IsArrive(SI32 six, SI32 siy)
{
	if(siPointNum <= 0)return false;

	if(clPos[siPointNum-1].siX == six && clPos[siPointNum-1].siY == siy)return true;

	return false;
}

// ù��° ������ ����ó��.
void cltMovePointList::Arrive()
{
	if(siPointNum <= 0)return ;

	clPos[siPointNum-1].Init();

	siPointNum--;
}

//==================================================
// cltCharMoveInfo
//==================================================
cltCharMoveInfo::cltCharMoveInfo()
{
	Init();
}

void cltCharMoveInfo::Init()
{
	siDX			= 0;
	siDY			= 0;
	siMovePercent	= 0;
	siSparePercent	= 0;	// ������ %
	siNX			= 0;
	siNY			= 0;
	siStepWidth		= 0;
	dwLastMoveClock	= 0;
}


// �̵� %�� ������Ų��. 
SI32 cltCharMoveInfo::IncMovePercent(SI32 movespeed)
{
	if(siMovePercent < 0)
	{
		siMovePercent += movespeed;
		siMovePercent	= min(100, siMovePercent);
		if(siMovePercent > 0)
		{
			siSparePercent = siMovePercent;
			siMovePercent = 0;
		}
	}
	else
	{
		siMovePercent += movespeed;
		if(siMovePercent > 100)
		{
			siSparePercent = siMovePercent - 100;
			siMovePercent = 100;
		}
	}

	return siMovePercent;
}

//==================================================
// cltCharCommonSimpleVary
//==================================================
cltCharCommonSimpleVary::cltCharCommonSimpleVary()
{
	ZeroMemory(this, sizeof(cltCharCommonSimpleVary));
}

// Ư�� �ִϸ��̼��� ���, ���� ĳ������ pk�� �ʿ��ϱ⵵ �ϴ�. (�ܼ��� �׸��⸸ �ϱ� ���ؼ�)
// ��ȭ�ؾ� �ϴ� ���ϸ��̼� Ÿ�Ե� ���� ���ϸ� �Ѵ�.
bool cltCharCommon::selectRealAnyType( )
{
	bool select  = false;
	
	switch(Animation)
	{
		case ANITYPE_SLEEP:
		case ANITYPE_SLEEPCAT:
			select  = true;
			break;
	}

	return select;
}

SI16 cltCharCommon::GetPVP_LeagueEnemyPersonID( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siEnemyPersonID;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueEnemyPersonID( SI16 siEnemyID )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siEnemyPersonID = siEnemyID;
	}
}



SI16 cltCharCommon::GetPVP_League_Win( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siWin;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_League_Win( SI16 siWin )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siWin = siWin;
	}
}

void cltCharCommon::IncPVP_League_Win( SI16 siWin )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siWin += siWin;
	}
}

SI16 cltCharCommon::GetPVP_League_Draw( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siDraw;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_League_Draw( SI16 siDraw )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siDraw = siDraw;
	}
}

void cltCharCommon::IncPVP_League_Draw( SI16 siDraw )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siDraw += siDraw;
	}
}
SI16 cltCharCommon::GetPVP_League_Lose( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siLose;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_League_Lose( SI16 siLose )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siLose = siLose;
	}
}

void cltCharCommon::IncPVP_League_Lose( SI16 siLose )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siLose += siLose;
	}
}

SI16 cltCharCommon::GetPVP_LeagueStraight_Win( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siStraight_Win;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueStraight_Win( SI16 siWin )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Win = siWin;
	}
}

void cltCharCommon::IncPVP_LeagueStraight_Win( SI16 siWin )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Win += siWin;
	}
}

SI16 cltCharCommon::GetPVP_LeagueStraight_Draw( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siStraight_Draw;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueStraight_Draw( SI16 siDraw )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Draw = siDraw;
	}
}

void cltCharCommon::IncPVP_LeagueStraight_Draw( SI16 siDraw )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Draw += siDraw;
	}
}



SI16 cltCharCommon::GetPVP_LeagueGrade( void )
{
	SI16 siReturnValue = PVP_LEAGUE_GRADE_NONE;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siGrade;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueGrade( SI16 siGrade )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siGrade = siGrade;
	}
}

bool cltCharCommon::IsPVP_LeagueLatestPersonID( SI32 siDestPersonID )
{
	if ( pclPVP_LeagueInfo )
	{
		for ( SI32 siIndex=0; siIndex<PVP_LEAGUE_MAX_LATEST_MATCH_PERSON; ++siIndex )
		{
			if ( pclPVP_LeagueInfo->m_siLatestPersonID[siIndex] == siDestPersonID )
			{
				return true;
			}
		}
	}

	return false;
}

void cltCharCommon::PushPVP_LeagueLatestPersonID( SI32 siDestPersonID )
{
	if ( NULL == pclPVP_LeagueInfo )
	{
		return;
	}

	const SI32 siLastIndex = (PVP_LEAGUE_MAX_LATEST_MATCH_PERSON - 1);

	// �� ���� �ε��������� ���� �� �ִٸ� ������ ��ĭ�� ���� �̵��Ѵ�
	if ( 0 <= pclPVP_LeagueInfo->m_siLatestPersonID[siLastIndex] )
	{
		for ( SI32 siIndex=0; siIndex<siLastIndex; ++siIndex )
		{
			SI32 siNextIndex = siIndex+1;
			if ( PVP_LEAGUE_MAX_LATEST_MATCH_PERSON <= siNextIndex )
			{
				break;
			}

			pclPVP_LeagueInfo->m_siLatestPersonID[siIndex]		= pclPVP_LeagueInfo->m_siLatestPersonID[siNextIndex];
			pclPVP_LeagueInfo->m_siLatestPersonID[siNextIndex]	= 0;
		}
	}

	// ��ĭ�� ���� �־��ָ� �ȴ�
	for ( SI32 siPushIndex=0; siPushIndex<PVP_LEAGUE_MAX_LATEST_MATCH_PERSON; ++siPushIndex )
	{
		if ( 0 >= pclPVP_LeagueInfo->m_siLatestPersonID[siPushIndex] )
		{
			pclPVP_LeagueInfo->m_siLatestPersonID[siPushIndex] = siDestPersonID;
		}
	}

}


SI16 cltCharCommon::GetPVP_LeagueStraight_Lose( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siStraight_Lose;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueStraight_Lose( SI16 siLose )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Lose = siLose;
	}
}

SI16 cltCharCommon::GetPVP_LeagueStraight_Win_Grade( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siStraight_Win_Grade;
	}

	return siReturnValue;
}
void cltCharCommon::SetPVP_LeagueStraight_Win_Grade( SI16 siWin )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Win_Grade = siWin;
	}
}
SI16 cltCharCommon::GetPVP_LeagueStraight_Lose_Grade( void )
{
	SI16 siReturnValue = 0;

	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->m_siStraight_Lose_Grade;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueStraight_Lose_Grade( SI16 siLose )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->m_siStraight_Lose_Grade = siLose;
	}
}
SYSTEMTIME cltCharCommon::GetPVP_LeagueEndBAttleTime( void )
{
	SYSTEMTIME siReturnValue	;
	ZeroMemory( &siReturnValue, sizeof(siReturnValue))	;
	if ( pclPVP_LeagueInfo )
	{
		siReturnValue = pclPVP_LeagueInfo->stLastPVPTime;
	}

	return siReturnValue;
}

void cltCharCommon::SetPVP_LeagueEndBAttleTime( SYSTEMTIME stEndTiem )
{
	if ( pclPVP_LeagueInfo )
	{
		pclPVP_LeagueInfo->stLastPVPTime = stEndTiem;
	}
}

BOOL cltCharCommon::IsMarried( void )
{
	if ( pclMarriage )
	{
		return pclMarriage->IsMarried();
	}

	return FALSE;
}
void cltCharCommon::SetProposePersonID( SI32 siProposePersonID )
{
	if ( pclMarriage )
	{
		pclMarriage->SetMatePersonID( siProposePersonID );
	}
}

SI32 cltCharCommon::GetProposePersonID( void )
{
	if ( pclMarriage )
	{
		return pclMarriage->GetMatePersonID();
	}

	return 0;
}
SI32 cltCharCommon::GetMyFamilyClass( void )	// ��������
{
	if ( pclFamily )
	{
		return pclFamily->GetMyClass_InFamily()	;
	}

	return MYCLASS_IN_FAMILY_NONE;
}

BOOL cltCharCommon::IsSoulGuardStatusOut()
{
	if ( false == IsPC(GetCharUnique()) )
	{
		return FALSE;
	}

	if ( NULL == pclSoulGuard )
	{
		return FALSE;
	}

	if ( TRUE == pclSoulGuard->IsEmpty() )
	{
		return FALSE;
	}

	SI32 siSoulGuardID = pclCM->GetIDFromCharUnique(siMySoulGuardUnique);	
	if ( FALSE == pclCM->IsValidID(siSoulGuardID) )
	{
		return FALSE;
	}

	if ( pclSoulGuard->GetStatus() == SOULGUARD_STATUS_OUT )
	{
		return TRUE;
	}

	return FALSE;
}

bool operator==( const cltSkillBookSubMagic& _LValue, cltSkillBookSubMagic& _RValue )
{
	// �θ� ���� ������ �ִٸ� ���������� ����. �ֳ�? �θ� ������ �ϳ��� ���� ������ ���´�.
	if(	_LValue.GetParentMagicKind() == _RValue.GetParentMagicKind() 
		&& 0						 <  _RValue.GetMagicKind()	) 
	{
		return true;
	}

	return false;
}
void cltCharSkillBookInfo::Clear( void )
{
	for( SI32 siCount = 0; siCount < SKILLBOOK_MAGIC_NUM; ++siCount )
		m_clSkillBookMagic[siCount].Clear();

	m_clSkillBookSubMagicList.RemoveAll();
}

bool cltCharSkillBookInfo::IsSkillBookMagic( void ) 
{
	for( SI32 siCount = 0; siCount < SKILLBOOK_MAGIC_NUM; ++siCount )
	{
		if(		0 < m_clSkillBookMagic[siCount].GetMagicKind() 
			&&	0 < m_clSkillBookMagic[siCount].GetSkillUnique() ) 
			return true;	
	}
		
	

	return false;
}

void cltCharSkillBookInfo::SetSkillBookMagic( IN SI32 _siIndex, cltSkillBookMagic* _pclAddSkillBookMagic )
{
	if( 0 > _siIndex || SKILLBOOK_MAGIC_NUM <= _siIndex )											 return;
	if( 0 >= _pclAddSkillBookMagic->GetMagicKind() || 0 >= _pclAddSkillBookMagic->GetSkillUnique() ) return;

	m_clSkillBookMagic[_siIndex].Set( _pclAddSkillBookMagic );
}

void cltCharSkillBookInfo::AddSkillBookMagic( IN SI32 _siAttackType, cltSkillBookMagic* _pclAddSkillBookMagic )
{
	SI32 siIndex = 	GetIndexFromAttackType( _siAttackType );
	if( 0 > siIndex ) return;
	
	m_clSkillBookMagic[siIndex].Set( _pclAddSkillBookMagic );
}

cltSkillBookMagic* cltCharSkillBookInfo::GetSkillBookMagicIndex( IN SI32 _siIndex )
{
	return &m_clSkillBookMagic[_siIndex];
} 

void cltCharSkillBookInfo::DelSkillBookMagic( IN SI32 _siIndex )
{
	m_clSkillBookMagic[_siIndex].Clear();
} 

void cltCharSkillBookInfo::AddSkillBookSubMagic( cltSkillBookSubMagic _clAddSkillBookSubMagic )
{
	NkListPos ListPos = m_clSkillBookSubMagicList.FindPos( _clAddSkillBookSubMagic );
	m_clSkillBookSubMagicList.Remove( ListPos );
			
	m_clSkillBookSubMagicList.PushBack( _clAddSkillBookSubMagic );
}

cltSkillBookSubMagic* cltCharSkillBookInfo::FindSkillBookSubMagicFromParentMagic( SI32 _siParentMagic )
{
	cltSkillBookSubMagic clAddSkillBookSubMagic( 0, _siParentMagic, 0 );
	NkListPos pListPos = m_clSkillBookSubMagicList.FindPos( clAddSkillBookSubMagic );
	if( NULL == pListPos ) return NULL;
	
	static cltSkillBookSubMagic* pclSkillBookSubMagic;
	
	pclSkillBookSubMagic = m_clSkillBookSubMagicList.GetDataPtr( pListPos );

	return pclSkillBookSubMagic;
}

stMagicAttributeData* cltCharSkillBookInfo::FindMagicAttributeDataFromParentMagic( IN SI32 _siParentMagic, OUT SI32& _siSubMagicKind )
{
	_siSubMagicKind = 0;
	
	cltSkillBookSubMagic* pclSkillBookSubMagic = FindSkillBookSubMagicFromParentMagic( _siParentMagic );
	if( NULL == pclSkillBookSubMagic ) return NULL;

	stMagicAttributeData* psSubMagicData	   = pclClient->pclMagicManager->GetMagicAttribute( pclSkillBookSubMagic->GetMagicKind() ); 
	if( NULL == psSubMagicData ) return NULL;

	_siSubMagicKind = pclSkillBookSubMagic->GetMagicKind();

	return psSubMagicData;
}


void cltCharSkillBookInfo::DelSkillBookSubMagic( const cltSkillBookSubMagic _clAddSkillBookSubMagic )
{
	NkListPos ListPos = m_clSkillBookSubMagicList.FindPos( _clAddSkillBookSubMagic );
	m_clSkillBookSubMagicList.Remove( ListPos );
}

SI32 cltCharSkillBookInfo::GetIndexFromAttackType( IN SI32 _siAttackType )
{
	SI32 siIndex = -1;
	switch( _siAttackType )
	{
	case ATTACKTYPE_SWORD:		siIndex = 0;	break;
	case ATTACKTYPE_SPEAR:		siIndex = 1;	break;
	case ATTACKTYPE_AXE:		siIndex = 2;	break;
	case ATTACKTYPE_STAFF:		siIndex = 3;	break;
	case ATTACKTYPE_STAFF1:		siIndex = 3;	break;
	case ATTACKTYPE_STAFF2:		siIndex = 3;	break;
	case ATTACKTYPE_NEW_STAFF2:	siIndex = 3;	break;
	case ATTACKTYPE_BOW:		siIndex = 4;	break;
	case ATTACKTYPE_BOW1:		siIndex = 4;	break;
	case ATTACKTYPE_NEW_BOW1:	siIndex = 4;	break;
	case ATTACKTYPE_GUN:		siIndex = 5;	break;
	case ATTACKTYPE_GUN1:		siIndex = 5;	break;
	case ATTACKTYPE_NEW_GUN1:	siIndex = 5;	break;
	}

	return siIndex;
}

cltSkillBookMagic*	cltCharSkillBookInfo::GetSkillBookMagic( IN SI32 _siAttackType )			
{	
	SI32 siIndex = 	GetIndexFromAttackType( _siAttackType );
	if( 0 > siIndex ) return NULL;

	return &m_clSkillBookMagic[siIndex];			
}	

NkList<cltSkillBookSubMagic>*	cltCharSkillBookInfo::GetSkillBookSubMagicList( void )	
{	
	return &m_clSkillBookSubMagicList;	
}

bool cltCharSkillBookInfo::IsTimeOver( cltCharServer* _pclChar )
{
	cltServer* pclserver = (cltServer*)pclClient;
	bool bAction = false;
	for( SI32 siCount = 0; siCount < SKILLBOOK_MAGIC_NUM; ++siCount )
	{
		if( m_clSkillBookMagic[siCount].IsTimeOver( &pclserver->sTime ) )
		{
			sDBRequest_SkillBook_Magic_Del	clMsg( _pclChar->GetID(), _pclChar->pclCI->GetPersonID(), siCount, &m_clSkillBookMagic[siCount] );
			pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);	

			bAction = true;
		}
	}
	

	NkListPos				ListPos = m_clSkillBookSubMagicList.GetFrontPos();
	cltSkillBookSubMagic*	pclSkillBookSubMagic;
	while( ListPos )
	{
		pclSkillBookSubMagic = &((NkListNode<cltSkillBookSubMagic>*)ListPos)->GetData();
		if( pclSkillBookSubMagic->IsTimeOver( &pclserver->sTime ) )
		{
			sDBRequest_SkillBook_SubMagic_Del	clMsg( _pclChar->GetID(), _pclChar->pclCI->GetPersonID(), pclSkillBookSubMagic, true );
			pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);	

			bAction = true;
		}

		ListPos = ((NkListNode<cltSkillBookMagic>*)ListPos)->GetNext();
	}

	return bAction;
}

bool cltCharSkillBookInfo::IsFullSubMagic( IN SI32 _siSkillunique )
{
	int						siCount = 0;
	NkListPos				ListPos = m_clSkillBookSubMagicList.GetFrontPos();
	cltSkillBookSubMagic*	pclSkillBookSubMagic;
	while( ListPos )
	{
		pclSkillBookSubMagic = &((NkListNode<cltSkillBookSubMagic>*)ListPos)->GetData();
		if( _siSkillunique == pclSkillBookSubMagic->GetSkillUnique() )
		{
			++siCount;
		}

		ListPos = ((NkListNode<cltSkillBookMagic>*)ListPos)->GetNext();
	}

	return 3 <= siCount ? true : false;
}

void cltCharCurseEffect::SendMsgCurseInfo( SI32 _siTargetID )
{
	cltCharServer* pclTargetChar = pclClient->pclCM->GetCharServer( _siTargetID );
	if( NULL == pclTargetChar ) return;

	cltGameMsgResponse_CurseInfo clCurseInfo( pclTargetChar->GetCharUnique(), &pclTargetChar->m_clCharCurseEffect );
	cltMsg clMsg( GAMEMSG_RESPONSE_CURSEINFO, sizeof(clCurseInfo), (BYTE*)&clCurseInfo );
	pclTargetChar->SendNeighbourMsg(&clMsg, true);
}