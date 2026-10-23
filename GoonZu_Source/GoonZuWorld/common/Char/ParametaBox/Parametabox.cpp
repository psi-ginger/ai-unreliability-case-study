//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "../Client/Client.h"
#include "../Server/Server.h"
#include <math.h>

#include <stdio.h>
#include "ParametaBox.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "Char/CharManager/CharManager.h"

#include "..\..\..\GoonZuWorld\CommonLogic\Cityhall\Cityhall.h"

#include "../../Skill/Skill.h"
#include "../../Skill/Skill-Manager.h"
#include "../../../common/Item/ItemCommon/cltSetItem.h"
#include "../../../CommonLogic/Marriage/MarriageManager.h"

#include <MsgType-Person.h>
#include <MsgType-Item.h >

#include "NFile.h"
#include "NUtil.h"

#include "Guild/Guild.h"

#include "../CommonLogic/StructureClass/StructureClass.h"
#include "../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

//SI64 ExpForLevelUp[MAX_PERSON_LEVEL][3];// SI32->SI64 경험치 오버플로우때문에 수정
//SI32 FameExpForLevelUp[MAX_FAME_LEVEL][3];
ExpTable			ExpForLevelUp;							// 경험치 테이블
FameExpTable		FameExpForLevelUp;						// 명성 경험치 테이블
cltLevelUpReward	LevelUpReward[MAX_CHARACTER_LEVEL+1];	// 캐릭터 레벨업 보상 테이블

//--------------------------------------------
// cltPIPInfoCommon
//-------------------------------------------
BOOL cltPIPInfoCommon::IncreaseExp(SI64 exp)// SI32->SI64 경험치 오버플로우때문에 수정
{
	if(exp <= 0)return FALSE;

	SetCurExp(GetCurExp() + exp);

	if(GetCurExp() >= GetRequiredExpForLevel(GetLevel() + 1) && GetLevel() < MAX_CHARACTER_LEVEL)// SI32->SI64 경험치 오버플로우때문에 수정
		return TRUE;

	return FALSE;
}

// 레벨에 의한 데미지 상승 효과를 구한다. 
SI32 cltPIPInfoCommon::CalcLevelDamage()
{
	SI32 sidamage = 0;
	SI32 level = siLevel;

	if(level > 90)
	{
		sidamage += ((level - 90) / 1);
		level = 90;
	}
	if(level > 70)
	{
		sidamage += ((level - 70) / 2);
		level = 70;
	}
	if(level > 60)
	{
		sidamage += ((level - 60) / 3);
		level = 60;
	}
	if(level > 50)
	{
		sidamage += ((level - 50) / 4);
		level = 50;
	}
	if(level > 40)
	{
		sidamage += ((level - 40) / 5);
		level = 40;
	}
	sidamage	+= (level / 10);

	return sidamage;
}

// 레벨에 따른 보상 아이템 정보를 얻는다.
BOOL cltPIPInfoCommon::GetLevelUpRewardItem(SI32 level, SI32* pItemUnique, SI32* pItemCount, SI16* psiNotice)
{
	if(level <= 0)					return FALSE;
	if(level > MAX_CHARACTER_LEVEL)	return FALSE;
	if(pItemUnique == NULL)			return FALSE;
	if(pItemCount == NULL)			return FALSE;
	if(psiNotice == NULL)			return FALSE;

	SI32	i;

	for(i=0; i<MAX_LEVELUP_REWARD_ITEM; i++)
	{
		pItemUnique[i] = 0;
		pItemCount[i] = 0;
		psiNotice[i] = 0;
	}

	for(i=0; i<MAX_LEVELUP_REWARD_ITEM; i++)
	{
		pItemUnique[i] = LevelUpReward[level].ItemUnique[i];
		pItemCount[i] = LevelUpReward[level].ItemCount[i];
		psiNotice[i] = LevelUpReward[level].siNotice[i];
	}

	return TRUE;
}

void cltPIPInfoCommon::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	pkGMSectionData->BeginData( "IPInfoCommon" );
	{
		pkGMSectionData->AddDataInt( "life",	Life);
		pkGMSectionData->AddDataInt( "mana",	Mana);
		pkGMSectionData->AddDataInt( "level",	siLevel);
		pkGMSectionData->AddDataInt( "bonus",	siBonus);
		pkGMSectionData->AddDataInt( "exp",		siExp);
	}
	pkGMSectionData->EndData( "IPInfoCommon" );
}


//--------------------------------------------
// cltPIPInfo
//--------------------------------------------
void  cltPIPInfo::Set(cltPIPInfo ip)
{
	ZeroMemory(this, sizeof(cltPIPInfo));
	memcpy(this, &ip, sizeof(cltPIPInfo));					//ok
}

void  cltPIPInfo::Set(cltPIPInfo* pip)
{
	if(pip == NULL)return ;

	ZeroMemory(this, sizeof(cltPIPInfo));

	memcpy(this, pip, sizeof(cltPIPInfo));					//ok
}


void  cltPIPInfo::Set(cltPIPInfoCommon* pip)
{
	if(pip == NULL)return ;

	Init();

	cltPIPInfoCommon::Set(pip);
}


bool cltPIPInfo::IncreaseFameExp(SI32 exp)
{
	if(exp < 0)return FALSE;

	siFameExp += exp;

	SI32 famelevel = GetFameLevelFromExp( siFameExp );
	if(famelevel > siFameLevel)return true;

	return false;
}

void cltPIPInfo::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	((cltBasicAbility*)this)->GetGMSectionData(pkGMSectionData);
	((cltPIPInfoCommon*)this)->GetGMSectionData(pkGMSectionData);

	pkGMSectionData->BeginData( "IPInfo" );
	{
		pkGMSectionData->AddDataInt( "skillbonus",	siSkillBonus);
		pkGMSectionData->AddDataInt( "fameexp",		siFameExp);	
		pkGMSectionData->AddDataInt( "famelevel",	siFameLevel);
		pkGMSectionData->AddDataInt( "money",		siMoney);	
	}
	pkGMSectionData->EndData( "IPInfo" );
}


//----------------------------------------------
// cltParametaBox
//----------------------------------------------
cltParametaBox::cltParametaBox()
{
	pclCI			= NULL;

	Init(NULL, NULL, NULL, NULL, NULL);
}

void cltParametaBox::Init(cltItemManagerCommon* pclitemmanager, cltPIPInfo* pclip, cltHealthInfo* pclhealthinfo, cltKindInfo* pclki, cltGMCommandKindInfo* pclGMki)
{
	pclItemManager		= pclitemmanager;
	pclIP				= pclip;
	clHealth.pclInfo	= pclhealthinfo;
	pclKI				= pclki;
	pclGMCommandKindInfo = pclGMki;

	if(pclki)
	{
		pclBPB				= &pclKI->clBPB;
	}
	else
	{
		pclBPB = NULL;
	}

	//------------------------------------------------------------------------
	siTotalPHysicalMinAttackDmg= 0;
	siTotalPHysicalMaxAttackDmg= 0;
	siEquipPHysicalMinAttackDmg= 0;
	siEquipPHysicalMaxAttackDmg= 0;
	siStrPHysicalAttackDmg= 0;

	siTotalMagicalMinAttackDmg= 0;
	siTotalMagicalMaxAttackDmg= 0;
	siEquipMagicalMinAttackDmg= 0;
	siEquipMagicalMaxAttackDmg= 0;

	siTotalPHysicalHitRate= 0;
	siPHysicalHitRateRank= 0;
	siPHysicalHitRateLevel= 0;
	siEquipPHysicalHitRate= 0;
	siAddPHysicalHitRate= 0;

	siTotalMagicalHitRate= 0;
	siMagicalHitRateRank= 0;
	siMagicalHitRateLevel= 0;
	siEquipMagicalHitRate= 0;
	siAddMagicalHitRate= 0;

	siTotalPHysicalDefensiveRate= 0;
	siPHysicalDefensive= 0;
	siEquipPHysicalDefensive= 0;
	siHndPHysicalDefensive= 0;

	siTotalMagicalDefensiveRate= 0;
	siMagicalDefensive= 0;
	siEquipMagicalDefensive= 0;
	siWisMagicalDefensive= 0;

	siTotalPHysicalDodgeRate= 0;
	siPHysicalDodgeRateRank= 0;
	siPHysicalDodgeRateLevel= 0;
	siEquipPHysicalDodgeRate= 0;

	siTotalMagicalDodgeRate= 0;
	siMagicalDodgeRateRank= 0;
	siMagicalDodgeRateLevel= 0;
	siEquipMagicalDodgeRate= 0;

	siTotalHitpoint= 0;
	siEquipHitpoint= 0;
	siVitHitpoint= 0;

	siTotalManapoint= 0;
	siEquipManapoint= 0;
	siMagManapoint= 0;

//---------------------------------------------------------------------

	uiParaActionFrame			= 0;		// 마지막으로 Action을 행한 프레임. 
	uiLastGetCriticalRateFrame	= 0;		// 마지막으로 크리티컬 공격 확률을 구한 프레임. 
	siOldLevel					= 0;

	siReservedLife		= 0;

	//----------------------
	// 크리티컬 공격 확률 
	//----------------------
	siCriticalRate		= 0;

	//---------------------
	// MagicDelay 줄이기
	//---------------------
	siMagicDecDelay		= 0;

	//----------------------
	// 방어력. 
	//----------------------
	siLevelAC			= 0;		// 레벨에 의한 방어력 
	siEquipAC			= 0;
	siDefaultAC			= 0;
	siGeneralAC			= 0;
	dwGeneralACClock	= 0;
	siEvolveAC			= 0;
	siBufAC				= 0;
	siNamingAC			= 0;
	siTransFormAC		= 0;
	TotalAC				= 0;

	//----------------------
	// 데미지 
	//----------------------
	siStrDamage			= 0;		// 근력에 의한 데미지. 
	siStaffSkillDamage	= 0;		// 지팡이술에 의한 데미지. 
	siLevelDamage		= 0;		// 레벨에 의한 데미지. 
	siTorpedoDamage		= 0;
	siNamingDamage		= 0;

	TotalEquipMinDamage	= 0;	// 무기에 의한 데미지. 
	TotalEquipMaxDamage	= 0;

	siGeneralDamage		= 0;
	dwGeneralDamageClock= 0;

	siBufDamage			= 0;

	siEvolveDamage		= 0;

	TotalMinDamage		= 0;
	TotalMaxDamage		= 0;

	siAttackType		= 0;								// 공격 방식. 	
	dwAttackInterval	= 0;								// 공격 주기. 

	//---------------------
	// 운
	//---------------------
	siTotalLuck			= 0;
	siEquipLuck			= 0;

	//--------------------------
	// 장비 관련 
	//--------------------------
	siArmWeaponUnique	= 0;
	clShapeEquip.Init();

	//--------------------------
	// 명중률 
	//--------------------------
	AR				= 0;
	DR				= 0;
	HitRate			= 0;
	siEquipAssistHitRate	= 0;
	siWeaponDefaultHitRate	= 0;

	//--------------------------
	// 장비에 의한 데미지 감소. 
	//--------------------------
	siEquipAssistDamageReduceRate	= 0;

	//-------------------------
	// 이동속도 
	//-------------------------
	BasicMoveSpeed	= 0;
	ShoesMoveSpeed	= 0;
	BufMoveSpeed	= 0;
	TotalMoveSpeed	= 0;
	GivenMoveSpeed	= 0;


	siHealthStrRate	= 0;
	siHealthDexRate	= 0;
	siHealthMagRate = 0;
/*
	siGuildStr=0;	
	siGuildDex=0;	
	siGuildMag=0;	
	siGuildVit=0;	
	siGuildHnd=0;	
	siGuildLuck=0;
*/
	siSkillVitRate	= 0;
	siSkillMagRate	= 0;

	siHorseStr		= 0;
	siHorseDex		= 0;
	siHorseMag		= 0;	

	clEquipBA.Init();
	clTotalBA.Init();

	bSetTotalBA		= false;

	//KHY - 1220 - 변신시스템.
	clTransFormBA.Init();
	siTransFormSyc	= 0;
}

// 다음 레벨로 올라가는데 필요한 경험치. 
SI64 cltParametaBox::GetNextExp()// SI32->SI64 경험치 오버플로우때문에 수정
{
	return GetRequiredExpForLevel(pclIP->GetLevel() + 1);
}

// 이전 레벨에서 지금 레벨로 올라오는데 필요했던 경험치. 
SI64 cltParametaBox::GetPrevExp()// SI32->SI64 경험치 오버플로우때문에 수정
{
	return GetRequiredExpForLevel(pclIP->GetLevel());
}


// 실제로 데미지를 줄 공격력을 구한다. 
SI32 cltParametaBox::CalcApower()
{

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SI32 diff = siTotalPHysicalMaxAttackDmg - siTotalPHysicalMinAttackDmg;
		if(diff < 0)diff = 0;

		return siTotalPHysicalMinAttackDmg + pclClient->GetFrame() % (diff + 1);
	}
	else
	{
		SI32 diff = TotalMaxDamage - TotalMinDamage;
		if(diff < 0)diff = 0;

		return TotalMinDamage + pclClient->GetFrame() % (diff + 1);
	}

	//return 0;

}


// 인챈트 관련 by tae-yang
SI32 cltParametaBox::GetEnchantDefDamage(SI32 EnchantType, cltItem *pclItemList)
{
	SI32 EnchantDamage = 0 ;

	EnchantDamage = pclItemManager->GetEnchantACFrom(pclItemList, EnchantType);

	return EnchantDamage;
}

// 인챈트 관련 by tae-yang
SI32 cltParametaBox::GetEnchantAttDamage(SI32 EnchantType, cltItem *pclItemList)
{
	SI32 EnchantDamage = 0 ;

	EnchantDamage = pclItemManager->GetEnchantAttFrom(pclItemList, EnchantType);

	return EnchantDamage;
}

////  101. 물리 공격력 
void cltParametaBox::SetPhysicalAttackDemage(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo ,bool *bSwitch, SI16 siAttackWearStoneNum)
{
	SI32 totalStr = clTotalBA.GetStr();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 101.1 설정되어있는 기본 데미지  - 캐릭터에 따라 리소스에 설정.
	//--------------------------------------
	SI32 defaultweaponmindamage = 0;
	SI32 defaultweaponmaxdamage = 0;
	
	// 소환수일때 장비가 있다면
	if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (pclitemlist) && (pclitemlist[PERSONITEM_WEAPON1].siUnique) )
	{	
		// 장비석의 갯수와 현재 레벨이 같아야 기본 데미지 적용
		if ( pclchar->pclCI->clIP.GetLevel() == siAttackWearStoneNum )
		{
			defaultweaponmindamage = pclKI->clDefaultWeaponInfo.GetDefaultMinDamage();
			defaultweaponmaxdamage = pclKI->clDefaultWeaponInfo.GetDefaultMaxDamage();
		}
	}
	else if((pclidentityinfo) && pclidentityinfo->siIdentity == IDENTITY_GMCREATE_MONSTER)
	{
		defaultweaponmindamage = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultMinDamage();
		defaultweaponmaxdamage = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultMaxDamage();
	}
	// 소환수가 아니거나 장비가 없다면
	else
	{
		defaultweaponmindamage = pclKI->clDefaultWeaponInfo.GetDefaultMinDamage();
		defaultweaponmaxdamage = pclKI->clDefaultWeaponInfo.GetDefaultMaxDamage();
	}


	//--------------------------------------
	// 101.2  근력에 따른 물리 데미지 설정.
	//--------------------------------------

	switch(siAttackType)
	{
		case ATTACKTYPE_SWORD:
		case ATTACKTYPE_SPEAR:
		case ATTACKTYPE_AXE:
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/20000);
			break;
		case ATTACKTYPE_BOW:
		case ATTACKTYPE_BOW1:
		case ATTACKTYPE_NEW_BOW1:
		case ATTACKTYPE_GUN:
		case ATTACKTYPE_GUN1:
		case ATTACKTYPE_NEW_GUN1:
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/150000 *6);
			break;
		case ATTACKTYPE_STAFF:
		case ATTACKTYPE_STAFF1:
		case ATTACKTYPE_STAFF2:
		case ATTACKTYPE_NEW_STAFF2:
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/150000 *3);
			break;
		//KHY - 0410 - 소환수등  - 원거리 적용.
		case	ATTACKTYPE_HALLOWEENGHOST	:
		case	ATTACKTYPE_CAKE:
		case	ATTACKTYPE_EGG			:
		case	ATTACKTYPE_CATFISH			:
		case	ATTACKTYPE_LADYBUGFAN			:
		case	ATTACKTYPE_TOHANG			:
		case	ATTACKTYPE_OCTOPUS			:
		case	ATTACKTYPE_BOOMERANG			:
		case	ATTACKTYPE_SHELLFISHBOMB			:
		case	ATTACKTYPE_VINE			:
		case	ATTACKTYPE_MONKEY			:
		case	ATTACKTYPE_FIREBALL			:
		case	ATTACKTYPE_FLYVINE			:
		case	ATTACKTYPE_BLACKSPIDER			:
		case	ATTACKTYPE_FROZENSORCERER			:
		case	ATTACKTYPE_CANNONMOL			:
		case	ATTACKTYPE_PRIEST			:
		case	ATTACKTYPE_EMPERORARCHER			:
		case	ATTACKTYPE_PIRATEARCHER			:
		case	ATTACKTYPE_MINEGIRL			:
		case	ATTACKTYPE_GREENTURTLE			:
		case	ATTACKTYPE_CASTLECANNON			:
		case	ATTACKTYPE_CHICKENBIG			:
		case	ATTACKTYPE_EVILEYE			:
		case	ATTACKTYPE_OTOHIME			:
		case	ATTACKTYPE_MAGICALRACCOON			:
		case	ATTACKTYPE_GREATWALLMAGICIAN			:
		case	ATTACKTYPE_GREATWALLASURA			:
		case	ATTACKTYPE_BEARTANG			:
		case	ATTACKTYPE_SUPERBEARTANG			:
		case	ATTACKTYPE_CHRISTMASRANGE			:
		case	ATTACKTYPE_EMPERORMASTER			:
		case	ATTACKTYPE_FROZENQUEEN			:
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/150000 *6);
			break;
		case	ATTACKTYPE_SIMTORYONG			:
		case	ATTACKTYPE_MAMARACCOON			:
		case	ATTACKTYPE_RACCOONTOWER			:
		case	ATTACKTYPE_GREATWALLSPIDER			:
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/20000);
			break;

		default : // 기타 모든 타입 - 맨손포함.
			siStrPHysicalAttackDmg = (totalStr* ((dwAttackInterval*10)+(level*50))/20000);
			break;
	}

	//--------------------------------------
	// 101.3  장비 따른 물리 데미지 설정. 
	//--------------------------------------
	SI16 minDmg = 0;
	SI16 maxDmg = 0;

	SI32 CrysTalMinPer = 0;
	SI32 CrysTalMaxPer = 0;

	if(pclitemlist)
	{
		if (pclClient->IsCountrySwitch(Switch_StatusBug)) 
		{
			minDmg = (SI16)pclItemManager->GetWeaponMinDamage(pclitemlist, activeweaponpos); 
			maxDmg = (SI16)pclItemManager->GetWeaponMaxDamage(pclitemlist, activeweaponpos);
		}
		else
		{
			minDmg = (SI16)pclItemManager->GetWeaponMinDamage(pclitemlist, activeweaponpos,pclchar , &CrysTalMinPer); 
			maxDmg = (SI16)pclItemManager->GetWeaponMaxDamage(pclitemlist, activeweaponpos,pclchar ,&CrysTalMaxPer);
		}

		if(minDmg >0)
			siEquipPHysicalMinAttackDmg = minDmg;
		else
			siEquipPHysicalMinAttackDmg = 0;
		
		if(maxDmg >0)
			siEquipPHysicalMaxAttackDmg = maxDmg;
		else
			siEquipPHysicalMaxAttackDmg = 0;
	}

	//--------------------------------------
	// 101.4  변신에 의한 물리 데미지 설정.
	//--------------------------------------
	//KHY - 1220 - 변신시스템.
	SI16 TminDmg = 0;
	SI16 TmaxDmg = 0;
	
	if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
	{
		switch(siAttackType)
		{ 
			case ATTACKTYPE_SWORD:
			case ATTACKTYPE_SPEAR:
				{
					if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
					{
						TminDmg = 20;
						TmaxDmg = 20;
					}
				}
				break;
			case ATTACKTYPE_AXE:
				{
					TminDmg = (SI16)(siTransFormSyc/2);
					TmaxDmg = (SI16)(siTransFormSyc/2);
				}
				break;					
			case ATTACKTYPE_GUN:
				{
					TminDmg = (SI16)((REAL32)siTransFormSyc*(REAL32)0.4);
					TmaxDmg = (SI16)((REAL32)siTransFormSyc*(REAL32)0.4);
				}
				break;					
		}
	}		

	//--------------------------------------
	// 101.5  소환수 장비석에 따른 데미지 상승
	//--------------------------------------
	SI16 siAttackWearStoneMinDmg = 0;
	SI16 siAttackWearStoneMaxDmg = 0;

	if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (SUMMON_EQUIP_LIMITMINLEVEL < siAttackWearStoneNum) )
	{
		REAL32 siAttackWearStoneIncreasePercent = (siAttackWearStoneNum - SUMMON_EQUIP_LIMITMINLEVEL) * 1.0;

		siAttackWearStoneMinDmg = (SI16)((REAL32)(siEquipPHysicalMinAttackDmg/100) * siAttackWearStoneIncreasePercent);
		siAttackWearStoneMaxDmg = (SI16)((REAL32)(siEquipPHysicalMaxAttackDmg/100) * siAttackWearStoneIncreasePercent);
	}

	//--------------------------------------
	// 101.6 PCK : 불해치를 타고 있을 경우 물리공격력 + 100을 해준다.
	// [춘기] 백호를 타고 있을 경우 물리 공격력 + 100을 해준다. (09.05.15)
	// [춘기] 바포메트를 타고 있을 경우 물리 공격력 +100을 해준다. (09.07.30)
	// [기형] 사수리를 타고 있을 경우 물리 공격력 +100을 해준다. (09.12.31)
	//--------------------------------------
	SI16	siFireHatchMinDmg = 0;
	SI16	siFireHatchMaxDmg = 0;

	if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
	{
		if(pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_FIREHATCH ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_BAPHOMET ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_SASURI)
		{
			siFireHatchMinDmg = 100;
			siFireHatchMaxDmg = 100;
		}
	}

	//--------------------------------------
	// 101.8.6 결혼 관련 데미지 상승
	//--------------------------------------
	SI16 siMarriageMinDmg = 0;
	SI16 siMarriageMaxDmg = 0;

	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) && (pclchar->IsMarried()) )	bMarriageCondition = true;
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) && (pclchar->IsMarried()) )		bMarriageCondition = true;
		}

		if ( (pclClient->m_pclMarriageMgr) && (true == bMarriageCondition) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode()) )
		{
			SI16 siMarriageValue = (SI16)pclClient->m_pclMarriageMgr->GetTotalAttackAbilityValue( pclchar->pclMarriage->GetLoveLevel() );
			if ( 0 < siMarriageValue )
			{
				siMarriageMinDmg = siMarriageValue;
				siMarriageMaxDmg = siMarriageValue;
			}
		}
	}

	//--------------------------------------
	// 101.7  각종 효과에 의한 물리 데미지 상승.
	//--------------------------------------
	minDmg = defaultweaponmindamage + siEquipPHysicalMinAttackDmg + (SI16)siStrPHysicalAttackDmg + TminDmg + siAttackWearStoneMinDmg + siFireHatchMinDmg + siMarriageMinDmg;
	maxDmg = defaultweaponmaxdamage + siEquipPHysicalMaxAttackDmg + (SI16)siStrPHysicalAttackDmg + TmaxDmg + siAttackWearStoneMaxDmg + siFireHatchMaxDmg + siMarriageMaxDmg;

	//--------------------------------------
	// 101.8.1   Identity 가 존재하는 특수 캐릭터인 경우 , 공격력 
	//--------------------------------------
	if(pclidentityinfo)
	{

		switch(pclidentityinfo->siIdentity)
		{
		case IDENTITY_CASTLE:		
			break;
		case IDENTITY_VILLAGEGOD:	
		case IDENTITY_VILLAGEGOD_SUMMON:
			break;
		case IDENTITY_PERSON_SUMMON:
			{
				minDmg = minDmg + (SI32)(minDmg * PERSON_SUMMON_ATTACKBONUS_PERCENT * 0.01f) ;
				maxDmg = maxDmg + (SI32)(maxDmg * PERSON_SUMMON_ATTACKBONUS_PERCENT * 0.01f) ;

				if (pclchar)
				{
					// 내가 소환수라면
					SI32 parentid = pclchar->GetSummonParentID();
					if(pclClient->pclCM->IsValidID(parentid))
					{
						SI32 summonskill = 0;

						if(pclskillinfo)
						{
							summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON,
								0,
								0,
								&pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
						}

						// 소환수 주인의 소환술 스킬 레벨에 따라 소환수의 공격력을 증가시킨다.
						SI32 attackBonusPercent = 0;
						attackBonusPercent = pclClient->GetSummonAttackBonus( summonskill );

						minDmg += minDmg * attackBonusPercent / 100;
						maxDmg += maxDmg * attackBonusPercent / 100;

						// 소환수 주인이 소환수 공격력, 방어력 증가 버프를 가지고 있다면 

						SI32 bufBonusPercent = 0;

						if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
						{
							bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_SUMMONATACUP ) );
						}   
						if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
						{
							bufBonusPercent = max( bufBonusPercent , 20 );
						}   

						if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_EVENT_EGGMOONCAKE ) == true )
						{
							SI32 percent = 0;
							SI32 siBufIndex = pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
							if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
								percent = max( percent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.m_stBuf[siBufIndex].siPercent );
						
							bufBonusPercent = max( bufBonusPercent , percent );
						}  

						minDmg += minDmg * bufBonusPercent / 100;
						maxDmg += maxDmg * bufBonusPercent / 100;
					}

				}

			}
			break;
		case IDENTITY_VILLAGEHUNT:								break;
		default:												break;
		}
	}

	//--------------------------------------
	// 101.8.2  지휘효과에 의한 상승.
	//--------------------------------------
	*bSwitch = false;	
	if(siGeneralDamage)
	{		
		//-------------------------------
		// 게임서버인 경우에는 지휘효과의 시간을 확인한다. 
		//-------------------------------
		if(gamemode == GAMEMODE_SERVER)
		{
			if(TABS(pclClient->CurrentClock - dwGeneralDamageClock) > 10000)
			{
				siGeneralDamage			= 0;				
				dwGeneralDamageClock	= 0;

				*bSwitch = true;
			}
		}

		minDmg = minDmg * (100 + siGeneralDamage ) / 100;
		maxDmg = maxDmg * (100 + siGeneralDamage ) / 100;
	}

	// 활성제에 의한 Attack상승
	if(gamemode == GAMEMODE_SERVER)
	{
		if ( pclchar && IsPC( pclchar->GetID() ) && pclchar->pclCI->clBI.bCombatService )
		{
			CrysTalMinPer += 30 ;
			CrysTalMaxPer += 30 ;
		}
	}
	else if(gamemode == GAMEMODE_CLIENT)
	{
		if ( pclchar && pclchar->GetID() == 1 && pclchar->pclCI->clBI.bCombatService )
		{
			CrysTalMinPer += 30 ;
			CrysTalMaxPer += 30 ;
		}		
	}
	// 말 프리미엄 파츠 풀파츠시 특수능력 삭제.
	//// 프리미엄 파츠 풀파츠 일때 공격력 5% 증가 
	//if(gamemode == GAMEMODE_SERVER)
	//{
	//	if ( pclchar && IsPC( pclchar->GetID() ))
	//	{
	//		cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();
	//		if ( pclHorse != NULL )
	//		{
	//			SI32 siEquipPremiumPartsNum = pclHorse->GetPremiumPartsNum()	;
	//			if (siEquipPremiumPartsNum == 3 )
	//			{
	//				SI32 siHorseunique = pclHorse->siHorseUnique	;
	//				switch (siHorseunique )
	//				{
	//				case HORSEUNIQUE_BROWN:	// 갈색마
	//				case HORSEUNIQUE_BLACK:// 흑마
	//				case HORSEUNIQUE_WHITE:// 백마
	//				case HORSEUNIQUE_RED://적토마
	//				case HORSEUNIQUE_RUDOLF ://루돌프
	//				case HORSEUNIQUE_STEEL ://철기마
	//				case HORSEUNIQUE_BEAST ://구미호
	//				case HORSEUNIQUE_THOUSAND ://천리마
	//				case HORSEUNIQUE_ZEBRA ://얼룩마
	//					{
	//						CrysTalMinPer += 5 ;
	//						CrysTalMaxPer += 5 ;
	//					}
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if(gamemode == GAMEMODE_CLIENT)
	//{
	//	if ( pclchar && pclchar->GetID() == 1 )
	//	{
	//		cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();
	//		if ( pclHorse != NULL )
	//		{
	//			SI32 siEquipPremiumPartsNum = pclHorse->GetPremiumPartsNum()	;
	//			if (siEquipPremiumPartsNum == 3 )
	//			{
	//				SI32 siHorseunique = pclHorse->siHorseUnique	;
	//				switch (siHorseunique )
	//				{
	//				case HORSEUNIQUE_BROWN:	// 갈색마
	//				case HORSEUNIQUE_BLACK:// 흑마
	//				case HORSEUNIQUE_WHITE:// 백마
	//				case HORSEUNIQUE_RED://적토마
	//				case HORSEUNIQUE_RUDOLF ://루돌프
	//				case HORSEUNIQUE_STEEL ://철기마
	//				case HORSEUNIQUE_BEAST ://구미호
	//				case HORSEUNIQUE_THOUSAND ://천리마
	//				case HORSEUNIQUE_ZEBRA ://얼룩마
	//					{
	//						CrysTalMinPer += 5 ;
	//						CrysTalMaxPer += 5 ;
	//					}
	//					break;
	//				}
	//			}
	//		}
	//		
	//	}		
	//}
	//--------------------------------------
	// 101.8.3  버프에 의한 공격력 상승
	//--------------------------------------
	if( gamemode == GAMEMODE_SERVER )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;

			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
				percent += 20;
			
			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
				percent += 20;

			if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}
			
			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}
					
			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_MENTOR) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}

			if( clBuf.IsHaveBuf(BUF_VALENTINEDAY) )
			{
				percent += 10;
			}



			// 크리스마스트리에 의한 공격력 증가
			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. S]
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				cltServer* pclserver = (cltServer*)pclClient;
				cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );
				if (
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))	   == TRUE ||		//[진성] 영웅의 동상 완성시 공격력 증가. => 2008-6-12
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))	   == TRUE   //KHY - 1113 - 크리스마스 이벤트 -크리스마스 버프추가.
					)
					
					percent += 10;
			}

			siBufDamage = percent;
			CrysTalMinPer += percent;
			CrysTalMaxPer += percent;
		}
	}
	else if( gamemode == GAMEMODE_CLIENT )
	{
		if ( (pclchar) && ( 1 == pclchar->GetID()) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;

			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
			{
				percent += 20;
			}

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
			{
				percent += 20;
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}
			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}			
			if( clBuf.IsHaveBuf(BUF_MENTOR) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_VALENTINEDAY) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}
			
			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. C]
			cltCharClient* pclcharclient = (cltCharClient*)pclchar;
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
				if( pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))		 ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))	 ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))		 ||		
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))	||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))	
					)				
				{
					percent += 10;
				}
			}

			siBufDamage = percent;
			CrysTalMinPer += percent;
			CrysTalMaxPer += percent;
		}
	}

	// 셋트 아이템에 의한 물리 공격력 상승
	if( gamemode == GAMEMODE_SERVER )
	{
		// 세트 효과에 따른 물리 공격력 상승.
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if( pclchar->m_siSetItemType )
				{
					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 );
				}
			}
		}
	}
	//클라
	else if( gamemode == GAMEMODE_CLIENT )
	{
		// 세트 효과에 따른 물리 공격력 상승.
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if( pclchar->m_siSetItemType )
				{
					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 );
					
					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 );

					CrysTalMinPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 );
					CrysTalMaxPer += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 );
				}
			}
		}
	}


	// 쿠키에 대한 공격력 상승 

	if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
	{
		if(gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if ( pclchar->pclFamily != NULL )
				{
					if ( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )
					{
						if(pclchar->pclFamily->GetOnChildrenNum() > 0 && pclchar->pclFamily->GetOnParentNum() > 0 )
						{
							cltServer* pclserver = (cltServer*)pclClient;
							SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
							if ( siPersonID > 0) 
							{
								SI32 siEndDateVary =	0	;
								SI32 siServerDateVary =	0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
								siServerDateVary = pclserver->pclTime->clDate.GetHourVary();
								if ( siEndDateVary>0 &&siServerDateVary>0  ) 
								{
									if(  siEndDateVary  >  siServerDateVary )
									{
										SI32 percent	= 0;
										SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel(siPersonID )	;// 레벨에 따라서 증가
										percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

										CrysTalMinPer += percent	;
										CrysTalMaxPer += percent	;
									}
								}
							}
						}
					}
				}
			}

		}
		else if(gamemode == GAMEMODE_CLIENT)
		{
			if ( pclchar && pclchar->GetID() == 1)
			{
				if(pclchar->pclFamily != NULL )
				{
					if ( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )
					{
						if(pclchar->pclFamily->GetOnChildrenNum() > 0 && pclchar->pclFamily->GetOnParentNum() > 0)
						{
							SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
							if ( siPersonID > 0 ) 
							{
								SI32 siEndDateVary = 0	;
								SI32 siClientDateVary = 0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
								siClientDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;

								if(   siEndDateVary >0 && siClientDateVary > 0 )
								{
									if ( siEndDateVary > siClientDateVary ) 
									{
										SI32 percent	= 0;
										SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( siPersonID )	;// 레벨에 따라서 증가
										percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

										CrysTalMinPer += percent	;
										CrysTalMaxPer += percent	;

									}
								}
							}
						}
					}
				}
			}
		}
	}


	//--------------------------------------
	// 101.8.4   명칭에 의한 공격력 상승
	//--------------------------------------
	if ( gamemode == GAMEMODE_SERVER)
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
			{
				cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
				if ( pclunit )
				{
					CrysTalMinPer +=  pclunit->GetAttack();
					CrysTalMaxPer +=  pclunit->GetAttack();

					siNamingDamage = pclunit->GetAttack();
				}

			}
			else
			{
				siNamingDamage = 0;
			}
		}
	}
	else if ( gamemode == GAMEMODE_CLIENT )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) == 1 )
		{
			if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
			{
				cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
				if ( pclunit )
				{
					CrysTalMinPer +=  pclunit->GetAttack();
					CrysTalMaxPer +=  pclunit->GetAttack();
					
					siNamingDamage = pclunit->GetAttack();
				}

			}
			else
			{
				siNamingDamage = 0;
			}
		}
	}

	//--------------------------------------
	// 101.8.5   스킬관련 무기데미지 향상
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	SI32 siDecMinMaxGapRate = 0;

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//---------------------------------------
		// 전투향상술에 의한 데미지 향상
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_INCWEAAT;	}	break;

		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				CrysTalMinPer +=  siWeaponSkillRate;
				CrysTalMaxPer +=  siWeaponSkillRate;
			}
		}
		
		//---------------------------------------
		// 전투 향상술에 의한 Min/Max차이 감소
		siSkillUnique = 0;
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_AXE_DECGAP;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_AXE_DECGAP;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_DECGAP;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_AXE_DECGAP;	}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_AXE_DECGAP;	}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_AXE_DECGAP;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siDecMinMaxGapRate = siWeaponSkillRate;
			}
		}

		//---------------------------------------
		// 무기술의 달인
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_PHYSICALATTACK );
			if ( 0 < siSkillMasterRate )
			{
				CrysTalMinPer +=  siSkillMasterRate;
				CrysTalMaxPer +=  siSkillMasterRate;
			}
		}

	}
	//--------------------------------------
	// 101.5  최종 물리 데미지 설정.  - PvP에 대한 예외처리가 있다.  -CrysTalMinPer,CrysTalMaxPer에 확률 값을 모아서 한번에 계산한다.
	//--------------------------------------
	if(CrysTalMaxPer>0)				
		siTotalPHysicalMaxAttackDmg = maxDmg + (maxDmg *CrysTalMaxPer /100) ;
	else
		siTotalPHysicalMaxAttackDmg = maxDmg ;

	if(CrysTalMinPer>0)
		minDmg = minDmg + (minDmg *CrysTalMinPer /100) ;
	
	//----------------------------------------
	// 전투 향상술에 의한 Min/Max차이 감소
	SI32 siDamageGap = siTotalPHysicalMaxAttackDmg - minDmg;
	if ( (0 < siDamageGap) && (0 < siDecMinMaxGapRate) )
	{
		minDmg += (siDamageGap * siDecMinMaxGapRate / 100);
	}

	// 최소 데미지는, 최대 데미지를 넘을 수 없다.
	SI16 siCurPHysicalMaxAttackDmg = 0;
	if(minDmg >siTotalPHysicalMaxAttackDmg)
	{
		siCurPHysicalMaxAttackDmg = siTotalPHysicalMaxAttackDmg;
	}
	else
		siCurPHysicalMaxAttackDmg = minDmg;

    siTotalPHysicalMinAttackDmg = siCurPHysicalMaxAttackDmg;

	SI32 siCurseAttack = 0;
	if( pclchar )
	{
		if( pclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_ATTACK, pclchar->GetID() ) )
		{
			siTotalPHysicalMinAttackDmg += (siTotalPHysicalMinAttackDmg * pclchar->m_clCharCurseEffect.GetPara()) / 100;
			siTotalPHysicalMaxAttackDmg += (siTotalPHysicalMaxAttackDmg * pclchar->m_clCharCurseEffect.GetPara()) / 100;
		}

		if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			siTotalPHysicalMinAttackDmg += 1 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siTotalPHysicalMinAttackDmg * 5) / 100 : 0;
			siTotalPHysicalMaxAttackDmg += 1 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siTotalPHysicalMaxAttackDmg * 5) / 100 : 0;
		}
	}

	//// [기형] 배틀 로얄의 파워업 상태라면
	//if( (pclchar) && ( true == pclchar->bBattleRoyalPowerupMode))
	//{
	//	siTotalPHysicalMinAttackDmg = siTotalPHysicalMinAttackDmg * 2;
	//	siTotalPHysicalMaxAttackDmg = siTotalPHysicalMaxAttackDmg * 2;
	//}
}

////  102. 마법 공격력 
void cltParametaBox::SetMagicalAttackDemage(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo , bool *bSwitch)
{
	SI32 totalMag = clTotalBA.GetMag();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 102.1 설정되어있는 기본 데미지  - 캐릭터에 따라 리소스에 설정. - 마법은 없다.
	//--------------------------------------

	//--------------------------------------
	// 102.2  지력과 레벨에 따른 마법 데미지 설정.
	//--------------------------------------
	SI16 minDmg = 0;
	SI16 maxDmg = 0;

	minDmg = (totalMag *(4000 + (level*50))/20000);
	maxDmg = (totalMag *(6000 + (level*50))/20000);

	//--------------------------------------
	// 102.3  장비 따른 마법 데미지 설정.  - 리턴 값이 % 다.
	//--------------------------------------
	SI16 EminPer = 0;
	SI16 EmaxPer = 0;

	if(pclitemlist)
	{
		if (pclClient->IsCountrySwitch(Switch_StatusBug)) 
		{
			//minDmg = pclItemManager->GetAddMagicalDamage(pclitemlist, activeweaponpos); 
			//maxDmg = pclItemManager->GetAddMagicalDamage(pclitemlist, activeweaponpos);
		}
		else
		{
			EminPer = (SI16)pclItemManager->GetAddMagicalMinDamage(pclitemlist, activeweaponpos,pclchar);  
			EmaxPer = (SI16)pclItemManager->GetAddMagicalMaxDamage(pclitemlist, activeweaponpos,pclchar);
		}

		if(EminPer >0)
			siEquipMagicalMinAttackDmg = EminPer;
		else
			siEquipMagicalMinAttackDmg = 0;

		if(EmaxPer >0)
			siEquipMagicalMaxAttackDmg = EmaxPer;
		else
			siEquipMagicalMaxAttackDmg = 0;
	}

	// PCK : 불해치를 타고 있을 경우 마법공격력 + 100을 해준다.
	// [춘기] 백호를 타고 있을 경우 마법 공격력 + 100을 해준다. (09.05.15)
	// [춘기] 바포메트를 타고 있을 경우 마법 공격력 + 100을 해준다. (09.07.30)
	// [기형] 사수리를 타고 있을 경우 마법 공격력 + 100을 해준다. (09.12.31)
	if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
	{
		if(pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_FIREHATCH ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_BAPHOMET ||
			pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_SASURI)
		{
			minDmg += 100;
			maxDmg += 100;
		}
	}	

	//--------------------------------------
	// 101.5.1  지휘효과에 의한 상승.
	//--------------------------------------
	*bSwitch = false;	
	if(siGeneralDamage)
	{		
		//-------------------------------
		// 게임서버인 경우에는 지휘효과의 시간을 확인한다. 
		//-------------------------------
		if(gamemode == GAMEMODE_SERVER)
		{
			if(TABS(pclClient->CurrentClock - dwGeneralDamageClock) > 10000)
			{
				siGeneralDamage			= 0;				
				dwGeneralDamageClock	= 0;

				*bSwitch = true;
			}
		}

		minDmg = minDmg * (100 + siGeneralDamage ) / 100;
		maxDmg = maxDmg * (100 + siGeneralDamage ) / 100;
	}

	//--------------------------------------
	// 101.5.2 결혼 관련 데미지 상승
	//--------------------------------------
	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) && (pclchar->IsMarried()) )	bMarriageCondition = true;
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) && (pclchar->IsMarried()) )		bMarriageCondition = true;
		}

		if ( (pclClient->m_pclMarriageMgr) && (true == bMarriageCondition) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode()) )
		{
			SI16 siMarriageValue = (SI16)pclClient->m_pclMarriageMgr->GetTotalAttackAbilityValue( pclchar->pclMarriage->GetLoveLevel() );
			if ( 0 < siMarriageValue )
			{
				minDmg +=  siMarriageValue;
				maxDmg +=  siMarriageValue;
			}
		}
	}

	// 활성제에 의한 Attack상승
	if(gamemode == GAMEMODE_SERVER)
	{
		if ( pclchar && IsPC( pclchar->GetID() ) && pclchar->pclCI->clBI.bCombatService )
		{
			EminPer += 30 ;
			EmaxPer += 30 ;
		}
	}
	else if(gamemode == GAMEMODE_CLIENT)
	{
		if ( pclchar && pclchar->GetID() == 1 && pclchar->pclCI->clBI.bCombatService )
		{
			EminPer += 30 ;
			EmaxPer += 30 ;
		}		
	}
	//클라
	//--------------------------------------
	// 101.5.1  버프에 의한 공격력 상승
	//--------------------------------------
	if( gamemode == GAMEMODE_SERVER )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;


			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
				percent += 20;
			
			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
				percent += 20;


			if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}
		
			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}


			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
			{
				percent += 10;
			}
			
			if( clBuf.IsHaveBuf(BUF_MENTOR) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}
			

			// 크리스마스트리에 의한 공격력 증가
			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. S]
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				cltServer* pclserver = (cltServer*)pclClient;
				cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );
				if (pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) == TRUE ||
					pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))	   == TRUE ||
					pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))   == TRUE ||
					pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))	   == TRUE ||
					pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))	   == TRUE	||
					pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))	   == TRUE
					)
					
					percent += 10;
			}

			siBufDamage = percent;
			EminPer += percent;
			EmaxPer += percent;
		}

	}
	else if( gamemode == GAMEMODE_CLIENT )
	{
		if ( (pclchar) && ( 1 == pclchar->GetID()) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;

			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
			{
				percent += 20;
			}

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
			{
				percent += 20;
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

            if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}


			if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
			{
				percent += 10;
			}

			if( clBuf.IsHaveBuf(BUF_MENTOR) )
			{
				percent += 10;
			}
			
			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}

			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. C]
			cltCharClient* pclcharclient = (cltCharClient*)pclchar;
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
				if( pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))		 ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))	 ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))		 ||
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")) ||		
					pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))		 
					)				
				{
					percent += 10;
				}
			}
			
			siBufDamage = percent;
			EminPer += percent;
			EmaxPer += percent;
		}
	}

	//--------------------------------------
	// 101.5.3  셋트 아이템에 의한 공격력 상승
	//--------------------------------------
	/*
	if( gamemode == GAMEMODE_SERVER )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			// 세트 효과에 따른 마법 공격력 상승.
			if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				if( pclchar->m_siSetItemType )
				{
					SI16 siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_1 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_2 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_3 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_4 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_5 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;
				}
			}

			// 할로윈 셋트를 입으면 마법데미지 10% 증가
			else if( pclchar->m_siSetItemType == HALLOWEEN1_SETITEM 
				|| pclchar->m_siSetItemType == HALLOWEEN2_SETITEM
				|| pclchar->m_siSetItemType == HALLOWEEN3_SETITEM
				|| pclchar->m_siSetItemType == HALLOWEEN4_SETITEM)
			{
				EminPer += 10 ;
				EmaxPer += 10 ;
			}
		}
	}
	//클라
	else if( gamemode == GAMEMODE_CLIENT )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) == 1 )
		{
			// 세트 효과에 따른 마법 공격력 상승.
			if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				if( pclchar->m_siSetItemType )
				{
					SI16 siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_1 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_2 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_3 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_4 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;

					siAddValue = pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_5 );
					EminPer += siAddValue;
					EmaxPer += siAddValue;
				}
			}

			// 할로윈 셋트를 입으면 마법데미지 10% 증가
			else if(   pclchar->m_siSetItemType == HALLOWEEN1_SETITEM 
				|| pclchar->m_siSetItemType == HALLOWEEN2_SETITEM
				|| pclchar->m_siSetItemType == HALLOWEEN3_SETITEM
				|| pclchar->m_siSetItemType == HALLOWEEN4_SETITEM)
			{
				EminPer += 10 ;
				EmaxPer += 10 ;
			}
		}
	}
	*/


	// 쿠키에 대한 공격력 상승 

	if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
	{
		if(gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if ( pclchar->pclFamily != NULL )
				{
					if ( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )
					{
						if(pclchar->pclFamily->GetOnChildrenNum() > 0 && pclchar->pclFamily->GetOnParentNum() > 0 )
						{
							cltServer* pclserver = (cltServer*)pclClient;
							SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
							if ( siPersonID > 0) 
							{
								SI32 siEndDateVary =	0	;
								SI32 siServerDateVary =	0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
								siServerDateVary = pclserver->pclTime->clDate.GetHourVary();
								if ( siEndDateVary>0 &&siServerDateVary>0  ) 
								{
									if(  siEndDateVary  >  siServerDateVary )
									{
										SI32 percent	= 0;
										SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel(siPersonID )	;// 레벨에 따라서 증가
										percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

										EminPer += percent	;
										EmaxPer += percent	;
									}
								}
							}
						}
					}
				}
			}

		}
		else if(gamemode == GAMEMODE_CLIENT)
		{
			if ( pclchar && pclchar->GetID() == 1)
			{
				if(pclchar->pclFamily != NULL )
				{
					if ( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )
					{
						if(pclchar->pclFamily->GetOnChildrenNum() > 0 && pclchar->pclFamily->GetOnParentNum() > 0)
						{
							SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
							if ( siPersonID > 0 ) 
							{
								SI32 siEndDateVary = 0	;
								SI32 siClientDateVary = 0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
								siClientDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;

								if(   siEndDateVary >0 && siClientDateVary > 0 )
								{
									if ( siEndDateVary > siClientDateVary ) 
									{
										SI32 percent	= 0;
										SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( siPersonID )	;// 레벨에 따라서 증가
										percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

										EminPer += percent	;
										EmaxPer += percent	;

									}
								}
							}
						}
					}
				}
			}
		}
	}


	//--------------------------------------
	// 101.5.3  스킬에 의한 마법 공격력 상승
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		// 전투향상술에 의한 마법 공격력 상승
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCMAGAT;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_INCMAGAT;	}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_INCMAGAT;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_INCMAGAT;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				EminPer += siWeaponSkillRate;
				EmaxPer += siWeaponSkillRate;
			}
		}
		
		// 무기술의 달인에 의한 마법 공격력 상승
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_MAGICALATTACK );
			if ( 0 < siSkillMasterRate )
			{
				EminPer +=  siSkillMasterRate;
				EmaxPer +=  siSkillMasterRate;
			}
		}

	}

	// 수호정령에 의한 마법공격력 상승
	/*
	if(pclClient->IsCountrySwitch(Switch_SoulGuard))
	{
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			if((pclchar != NULL))
			{
				if(pclchar->IsSoulGuardStatusOut() == TRUE && pclchar->pclSoulGuard->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SOULGUARD_PUMPKIN")))
				{		
					EminPer += 10	;
					EmaxPer += 10	;
				}
			}
		}
		else if(pclClient->GameMode == GAMEMODE_CLIENT)
		{
			if((pclchar) && (1 == pclchar->GetID()))
			{
				if(pclchar->IsSoulGuardStatusOut() == TRUE && pclchar->pclSoulGuard->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SOULGUARD_PUMPKIN")))
				{		
					EminPer += 10	;
					EmaxPer += 10	;
				}
			}
		}	
	}
	*/
	
	//--------------------------------------
	// 102.4  최종 마법 데미지 설정.  - 이후 마법 발동시 플러스 되는 값이다. - PvP시 적용되는 값은 발동시 적용(?)
	//--------------------------------------
	if(EmaxPer>0)	
		siTotalMagicalMaxAttackDmg = (SI16)(maxDmg + (maxDmg *EmaxPer / 100));
	else
		siTotalMagicalMaxAttackDmg = (SI16)maxDmg ;

	if(EminPer >0)
		minDmg = (SI16)(minDmg + (minDmg *EminPer / 100));

	// 최소 데미지는, 최대 데미지를 넘을 수 없다.
	SI16 siCurMagicalMinAttackDmg = 0;
	if(minDmg >siTotalMagicalMaxAttackDmg)
		siCurMagicalMinAttackDmg = siTotalMagicalMaxAttackDmg;
	else
		siCurMagicalMinAttackDmg = minDmg;	

	SI32 siCurseAttack = 0;
	
	siTotalMagicalMinAttackDmg = siCurMagicalMinAttackDmg;

	if( pclchar )
	{
		if( pclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_ATTACK, pclchar->GetID() ) )
		{
			siTotalMagicalMinAttackDmg += (siTotalMagicalMinAttackDmg * pclchar->m_clCharCurseEffect.GetPara()) / 100;
			siTotalMagicalMaxAttackDmg += (siTotalMagicalMaxAttackDmg * pclchar->m_clCharCurseEffect.GetPara()) / 100;
		}

		if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			siTotalMagicalMinAttackDmg += 4 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siTotalMagicalMinAttackDmg * 5) / 100 : 0;
			siTotalMagicalMaxAttackDmg += 4 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siTotalMagicalMaxAttackDmg * 5) / 100 : 0;
		}
	}

	

	// [기형] 배틀 로얄의 파워업 상태라면
	/*if( (pclchar) && (true == pclchar->bBattleRoyalPowerupMode) )
	{
		siTotalMagicalMinAttackDmg = siTotalMagicalMinAttackDmg * 2;
		siTotalMagicalMaxAttackDmg = siTotalMagicalMaxAttackDmg * 2;
	}*/
}

////  103. 물리 명중률 
void cltParametaBox::SetPhysicalHitRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltIdentity* pclidentityinfo, cltPSkillInfo* pclskillinfo)
{
	if( NULL == pclchar ) return; 

	SI32 totalHnd = clTotalBA.GetHnd();
	SI32 level = pclIP->GetLevel();
	
	//SI32 WeaponDefaultHitRate	= pclitemlist[activeweaponpos].GetDefaultHitRate(pclItemManager); // 안쓰기로 했음.

	//--------------------------------------
	// 103.1 NPC와 비NPC를 구분한다.
	//--------------------------------------
	if(bnpc == false)
	{
		//--------------------------------------
		// 103.2 장비에의한 물리 명중률 향상을 구한다. - NPC는 장비가 없음으로 제외.
		//--------------------------------------
		SI16 EHitRate = 0;

		if(pclitemlist)
		{
			EHitRate = pclItemManager->GetEquipCrystalPhysicalHitRate(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 명중률 향상  - % 
		}

		if(EHitRate>0)
			siEquipPHysicalHitRate = EHitRate;
		else
			siEquipPHysicalHitRate = 0;

		//KHY - 1220 - 변신시스템.
		if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
		{
			switch(siAttackType)
			{				
				case ATTACKTYPE_SPEAR :
				case ATTACKTYPE_BOW :
					siAddPHysicalHitRate = (SI32)(siTransFormSyc/20);
					break;
			}
		}
	}
	else if(pclidentityinfo && pclidentityinfo->siIdentity)	
	{
		//--------------------------------------
		// 103.3 Identity 가 존재하는 특수 캐릭터인 경우 , 명중률 보정값
		//--------------------------------------
		switch(pclidentityinfo->siIdentity)
		{
		case IDENTITY_CASTLE:				siAddPHysicalHitRate = 1000; break; 
		case IDENTITY_VILLAGEGOD:			siAddPHysicalHitRate = 500;	 break;
		case IDENTITY_VILLAGEGOD_SUMMON:	siAddPHysicalHitRate = 200;	 break;
		case IDENTITY_VILLAGEHUNT:			siAddPHysicalHitRate = 200;	 break;
		default:					break;
		}
	}

	//--------------------------------------
	// 103.4 세트아이템에 의한 물리 명중률 향상
	//--------------------------------------
	SI16 siAddPHysicalHitRateSetItem = 0;				// 세트 아이템에 의한 물리 명중률. 
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		//[진성] 세트 아이템 으로 인한 명중률 변경.
		if( pclchar && pclchar->m_siSetItemType )
		{	
			siAddPHysicalHitRateSetItem += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_1 );
			siAddPHysicalHitRateSetItem += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_2 );
			siAddPHysicalHitRateSetItem += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_3 );
			siAddPHysicalHitRateSetItem += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_4 );
			siAddPHysicalHitRateSetItem += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_5 );
		}
	}

	//--------------------------------------
	// 103.5 스킬에 의한 물리 명중률 향상
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	SI16 siAddWeaponSkillHitRate = 0;
	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 103.6 전투향상술
		//--------------------------------------
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_GUN_INCPHYAR;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_GUN_INCPHYAR;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_GUN_INCPHYAR;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_GUN_INCPHYAR;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_INCPHYAR;			}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_GUN_INCPHYAR;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siAddWeaponSkillHitRate	+= siWeaponSkillRate;
			}
		}

		//--------------------------------------
		// 103.7 무기술의 달인
		//--------------------------------------
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI16 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_PHYSICALHIT );
			if ( 0 < siSkillMasterRate )
			{
				siAddWeaponSkillHitRate += siSkillMasterRate;
			}
		}

	}

	//--------------------------------------
	// 103.8 물리 명중 등급 설정.  - 기본값
	//--------------------------------------
	SI16 tempHitRateRank = 0;
	
	if((totalHnd >0) && (level > 0))
		tempHitRateRank = ((totalHnd *3) +level) + siAddPHysicalHitRate;
	else
		tempHitRateRank = 0;


	//--------------------------------------
	// 103.9  물리 명중 등급 설정. - 장비 포함 최종값.
	//--------------------------------------
	if ( (siEquipPHysicalHitRate > 0) || (siAddPHysicalHitRateSetItem > 0) || (siAddWeaponSkillHitRate > 0) ) 
		siPHysicalHitRateRank = tempHitRateRank + (tempHitRateRank * (siEquipPHysicalHitRate + siAddPHysicalHitRateSetItem + siAddWeaponSkillHitRate) / 100); //크리스탈에 의한 회피율 향상 수치를 더한다.
	else
		siPHysicalHitRateRank = tempHitRateRank ; //크리스탈에 의한 회피율 향상 수치를 더한

	//--------------------------------------
	// 기타: 명중률의 최종값은 GetHitRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------

}

////  104. 마법 명중률 
void cltParametaBox::SetMagicalHitRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltIdentity* pclidentityinfo, cltPSkillInfo* pclskillinfo)
{
	if( NULL == pclchar ) return; 

	SI32 totalWis = clTotalBA.GetWis();
	SI32 level = pclIP->GetLevel();

	//SI32 WeaponDefaultHitRate	= pclitemlist[activeweaponpos].GetDefaultHitRate(pclItemManager); // 안쓰기로 했음.

	//--------------------------------------
	// 104.1 NPC와 비NPC를 구분한다.
	//--------------------------------------
	if(bnpc == false)
	{
		//--------------------------------------
		// 104.2 장비에의한 마법 명중률 향상을 구한다. - NPC는 장비가 없음으로 제외.
		//--------------------------------------
		SI16 EHitRate = 0;

		if(pclitemlist)
		{
			EHitRate = pclItemManager->GetEquipCrystalMagicalHitRate(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 마법 명중률 향상  - % 
		}

		if(EHitRate>0)
			siEquipMagicalHitRate = EHitRate;
		else
			siEquipMagicalHitRate = 0;
	}
	else if(pclidentityinfo && pclidentityinfo->siIdentity)	
	{
		//--------------------------------------
		// 104.3 Identity 가 존재하는 특수 캐릭터인 경우 , 명중률 보정값
		//--------------------------------------
		switch(pclidentityinfo->siIdentity)
		{
		case IDENTITY_CASTLE:				siAddMagicalHitRate = 1000; break; 
		case IDENTITY_VILLAGEGOD:			siAddMagicalHitRate = 500;	 break;
		case IDENTITY_VILLAGEGOD_SUMMON:	siAddMagicalHitRate = 200;	 break;
		case IDENTITY_VILLAGEHUNT:			siAddMagicalHitRate = 200;	 break;
		default:					break;
		}
	}


	//--------------------------------------
	// 104.4 [진성] 세트 아이템 으로 인한 명중률 변경.
	//--------------------------------------
	SI32 AddSetItemHitRate = 0;
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		//[진성] 세트 아이템 으로 인한 명중률 변경.
		if( pclchar && pclchar->m_siSetItemType )
		{	
			AddSetItemHitRate += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_1 );
			AddSetItemHitRate += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_2 );
			AddSetItemHitRate += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_3 );
			AddSetItemHitRate += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_4 );
			AddSetItemHitRate += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HIT_RATE, USE_SETITEM_5 );
		}
	}

	//--------------------------------------
	// 104.5 스킬에 의한 마법 명중률 향상
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	SI16 siAddWeaponSkillHitRate = 0;
	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 104.6 무기술의 달인
		//--------------------------------------
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		SI32 siSkillUnique		= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI16 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_MAGICALHIT );
			if ( 0 < siSkillMasterRate )
			{
				siAddWeaponSkillHitRate += siSkillMasterRate;
			}
		}
	}

	//--------------------------------------
	// 104.3 마법 명중 등급 설정.  - 기본값
	//--------------------------------------
	SI16 tempHitRateRank = 0;

	if((totalWis >0) && (level > 0))
		tempHitRateRank = ((totalWis* 2)+level);
	else
		tempHitRateRank = 0;

	// %
	SI32 silastHiRate = siEquipMagicalHitRate + AddSetItemHitRate + siAddWeaponSkillHitRate;

	//--------------------------------------
	// 103.6  마법 명중 등급 설정. . - 장비 포함 최종값.
	//--------------------------------------
	if (silastHiRate >0)
		siMagicalHitRateRank = tempHitRateRank + (tempHitRateRank * silastHiRate /100); //크리스탈에 의한 회피율 향상 수치를 더한다.
	else
		siMagicalHitRateRank = tempHitRateRank ; //크리스탈에 의한 회피율 향상 수치를 더한


	//--------------------------------------
	// 기타: 명중률의 최종값은 GetHitRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------

}

////  105. 물리 방어율
void cltParametaBox::SetPhysicalDefensiveRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo, bool *bSwitch, SI16 siDefenseWearStoneNum)
{
	SI32 totalHnd = clTotalBA.GetHnd();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 105.1 설정되어있는 기본 방어력  - 캐릭터에 따라 리소스에 설정.
	//--------------------------------------
	SI32 DefaultAC = 0;

	if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (pclitemlist) && 
		( pclitemlist[PERSONITEM_HELMET].siUnique || pclitemlist[PERSONITEM_ARMOUR].siUnique || pclitemlist[PERSONITEM_BELT].siUnique || 
		pclitemlist[PERSONITEM_SHOES].siUnique || pclitemlist[PERSONITEM_MANTLE].siUnique ) 

	)
	{	
		// 장비석의 갯수와 현재 레벨이 같아야 기본 데미지 적용
		if ( pclchar->pclCI->clIP.GetLevel() == siDefenseWearStoneNum )
		{
			DefaultAC = pclKI->clDefaultWeaponInfo.GetDefaultAC(); 
		}
	}
	else if((pclidentityinfo) && pclidentityinfo->siIdentity == IDENTITY_GMCREATE_MONSTER)
	{
		DefaultAC = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultAC();
	}
	// 소환수가 아니거나 장비가 없다면
	else
	{
		DefaultAC = pclKI->clDefaultWeaponInfo.GetDefaultAC(); 
	}


	//--------------------------------------
	// 105.2  손재주에 따른 물리 방어력 설정.
	//--------------------------------------

	siHndPHysicalDefensive = (totalHnd+level)*60/100 ;

	//--------------------------------------
	// 105.3  장비 따른 물리 방어력 설정. 
	//--------------------------------------
	SI16 Edefansive = 0;
	SI32 CrysTalPer = 0;

	// Edefansive = 순수 장비 방어력.
	// CrysTalPer = 결정체에의하여 올라가야 하는 퍼센트.
	
	if(pclitemlist)
	{
		Edefansive = (SI16)pclItemManager->GetEquipCrystalPhysicalDefensiveRate(pclitemlist, activeweaponpos,pclchar , &CrysTalPer); 

		if(Edefansive>0)
			siEquipPHysicalDefensive = Edefansive;
		else
			siEquipPHysicalDefensive = 0;
	}

	//--------------------------------------
	// 105.4.1 변신 물리 방어력 .  
	//--------------------------------------	
	SI16 sTransFormAC = 0; // 변신에 변신에 의해 올라가는 방어력.
	
	if ( pclchar ) 
	{
		//KHY - 0124 - 가챠레어- 닌자 변신 아이템.
		if(pclchar->clTransFormInfo.bTransFormMode ==true)
		{
			if( (pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAMAN"))			||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAGIRL"))			||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATMELEE"))			||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATRANGE"))			||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE"))	||	// 눈사람
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE"))	||	// 검사스피릿(여)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE"))	||	// 거너스피릿(남)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYMELEE"))		||	// 요정스피릿(여)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYRANGE"))		||	// 요정스피릿(남)
				// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICERANGE"))			||	// 눈의 여왕
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICEMELEE"))				// 사스콰치
			  )		
			{
				// 방어력 증가.
				sTransFormAC = 80; // 기본 방어력.
				SI32 temState = 0;
				temState = sTransFormAC * (siTransFormSyc/2) / 100;

				sTransFormAC = sTransFormAC + temState; // 싱크로율 증가분.
			}
		}
	}

	//--------------------------------------
	// 105.5  소환수 장비석에 의한 방어력 상승값
	//--------------------------------------
	SI16 siDefenseWearStoneAC = 0;

	if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (SUMMON_EQUIP_LIMITMINLEVEL < siDefenseWearStoneNum) )
	{
		REAL32 siDefenseWearStoneIncreasePercent = (siDefenseWearStoneNum - SUMMON_EQUIP_LIMITMINLEVEL) * 1.0;

		siDefenseWearStoneAC = (SI16)((REAL32)(siEquipPHysicalDefensive/100) * siDefenseWearStoneIncreasePercent);
	}

	//--------------------------------------
	// 105.6  결혼 의한 방어력 상승값
	//--------------------------------------
	SI16 siDefenseMarriageAC = 0;

	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) && (pclchar->IsMarried()) )	bMarriageCondition = true;
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) && (pclchar->IsMarried()) )		bMarriageCondition = true;
		}

		if ( (pclClient->m_pclMarriageMgr) && (true == bMarriageCondition) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode()) )
		{
			SI16 siMarriageValue = (SI16)pclClient->m_pclMarriageMgr->GetTotalDefenseAbilityValue( pclchar->pclMarriage->GetLoveLevel() );
			if ( 0 < siMarriageValue )
			{
				siDefenseMarriageAC = siMarriageValue;
			}
		}
	}
	SI32 siCookieEffect = 0 ;
	// 쿠키 효과
	if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) )
			{
				if( pclchar->pclFamily != NULL )
				{
					if( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)
					{
						if(pclchar->pclFamily->GetOnParentNum() > 0 )
						{
							cltServer* pclserver = (cltServer*)pclClient;
							SI32 siEndDateVary = 0	;
							SI32 siNowDateVary = 0	;
							SI32 siCookieLevel =	0	;
							SI32 siEffect =	0	;
							siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
							siNowDateVary = pclserver->pclTime->clDate.GetHourVary()	;
							if(  siEndDateVary > 0 && siNowDateVary > 0 && siEndDateVary > siNowDateVary )
							{
								siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;
								siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;			

								siCookieEffect	 = siEffect	; // 증가시킬 %
							}

						}
					}
				}	
			}
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) )
			{
				if( pclchar->pclFamily != NULL )
				{
					if( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)
					{
						if(pclchar->pclFamily->GetOnParentNum() > 0  )
						{
							SI32 siEndDateVary =	0	;
							SI32 siNowDateVary =	0	;
							SI32 siCookieLevel =	0	;
							SI32 siEffect =	0	;

							siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
							siNowDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;
							if(  siEndDateVary > 0 && siNowDateVary > 0 &&siEndDateVary > siNowDateVary )
							{
								
								siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;
								siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;
								siCookieEffect	 = siEffect	; // 증가시킬 %
							}

						}
					}
				}	
			}
		}

	}	
	//----------------------------------------------------------------
	// 105.7 탈것(백호)을 타게 되면 물리방어력이 100 상승한다.(09.05.13)
	//----------------------------------------------------------------
	SI16	siPhysicalDefenseHorseAC = 0;

	if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
	{
		if(pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER)
		{
			siPhysicalDefenseHorseAC = 100;
		}
	}

	//--------------------------------------
	// 105.8  기타 물리 방어력 설정.  
	//--------------------------------------
	SI32 temPhysicalDFensive = 0;
	
	temPhysicalDFensive = siHndPHysicalDefensive + siEquipPHysicalDefensive + DefaultAC + sTransFormAC 
		+ siDefenseWearStoneAC + siDefenseMarriageAC + siPhysicalDefenseHorseAC  ; // 손재주 + 장비  + 기본 + 변신 + 소환수장비석 + 결혼 + 탈것(백호)

	SI32 etcDefansiveFer = 0; // 기타 설정에 의한 방어력 절대값 합산.		
	
	etcDefansiveFer = etcPhysicalDefensiveRate( pclitemlist, activeweaponpos, pclchar , &CrysTalPer ,gamemode,pclskillinfo ,bSwitch);

	//--------------------------------------
	// 105.9  최종 물리 방어력 설정.   - CrysTalPer = % 에 의한 상승값 합산.
	//--------------------------------------
	if( etcDefansiveFer >0 && siCookieEffect <=0 )
	{
		siPHysicalDefensive = temPhysicalDFensive + (temPhysicalDFensive * etcDefansiveFer / 100);
	}
	else if ( etcDefansiveFer <= 0 && siCookieEffect >0 )	// 쿠키 이팩트 
	{
		siPHysicalDefensive = temPhysicalDFensive + (temPhysicalDFensive * siCookieEffect / 100) ;
	}
	else if ( etcDefansiveFer > 0 && siCookieEffect > 0 )	// 쿠키 이팩트 
	{
		siPHysicalDefensive = temPhysicalDFensive + (temPhysicalDFensive * (etcDefansiveFer + siCookieEffect) / 100) ;
	}
	else
		siPHysicalDefensive = temPhysicalDFensive;

	if( pclchar )
	{
		if( pclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_DEFENSE, pclchar->GetID() ) )
		{
			siPHysicalDefensive += (siPHysicalDefensive * pclchar->m_clCharCurseEffect.GetPara()) / 100; 
		}

		if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			siPHysicalDefensive += 2 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siPHysicalDefensive * 5) / 100 : 0;
		}
	}

	

	//--------------------------------------
	// 기타: 방어율의 최종값은 GetDefensiveRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------	
}

// 방어율 기타 예외 처리.
SI16 cltParametaBox::etcPhysicalDefensiveRate(cltItem* pclitemlist, SI16 activeweaponpos,cltCharCommon* pclchar,  SI32 * DefensivePer , SI32 gamemode ,cltPSkillInfo*	pclskillinfo, bool *bSwitch)
{
	SI16 temPhysicalDRate = 0; // 퍼센트 값이다.


	//--------------------------------------
	// 105.4.1  물리 방어력- 결정체에 의한 방어력.
	//--------------------------------------
	temPhysicalDRate += *DefensivePer;

	//[진성] 세트 효과에 따른 방어력 상승.
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			if( pclchar->m_siSetItemType ) 
			{
				temPhysicalDRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_DEFENSIVE, USE_SETITEM_1);
				temPhysicalDRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_DEFENSIVE, USE_SETITEM_2);
				temPhysicalDRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_DEFENSIVE, USE_SETITEM_3);
				temPhysicalDRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_DEFENSIVE, USE_SETITEM_4);
				temPhysicalDRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_DEFENSIVE, USE_SETITEM_5);
			}
		}
	}
	else if( pclchar && ( pclchar->m_siSetItemType == BOY_SULBIM_SETITEM  || pclchar->m_siSetItemType == GIRL_SULBIM_SETITEM ) )
	{
		temPhysicalDRate += 20;
	}

	//--------------------------------------
	// 105.4.3 지휘효과에 의한 AC상승. 
	//--------------------------------------	
	*bSwitch = false;
	if(siGeneralAC)
	{
		//-------------------------------
		// 게임서버인 경우에는 지휘효과의 시간을 확인한다. 
		//-------------------------------
		if(gamemode == GAMEMODE_SERVER)
		{
			if(TABS(pclClient->CurrentClock - dwGeneralACClock) > 10000)
			{
				siGeneralAC			= 0;
				dwGeneralACClock	= 0;

				*bSwitch = true;
			}
		}
		temPhysicalDRate +=  siGeneralAC;
	}

	//--------------------------------------
	// 105.4.4 활성제에 의한 AC상승
	//--------------------------------------	
	if(gamemode == GAMEMODE_SERVER)
	{
		if ( pclchar && IsPC( pclchar->GetID() ) && pclchar->pclCI->clBI.bCombatService )
		{
			temPhysicalDRate += 30;
		}
	}
	else if(gamemode == GAMEMODE_CLIENT)
	{
		if ( pclchar && pclchar->GetID() == 1 && pclchar->pclCI->clBI.bCombatService )
		{
			temPhysicalDRate += 30;
		}		
	}

	//--------------------------------------
	// 105.4.5  버프에 의한 AC 상승
	//--------------------------------------
	if( gamemode == GAMEMODE_SERVER )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent =0;
			SI16 add = 0;

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
				percent += 20;

			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
				percent += 20;

			if( clBuf.IsHaveBuf(BUF_MENTOR) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_VALENTINEDAY) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_DEFENSEUP) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_DEFENSEUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_DEFENSEUP30) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_DEFENSEUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}
			
			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}
			
			// 크리스마스트리에 의한 방어력 증가
			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. S]
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				cltServer* pclserver = (cltServer*)pclClient;
				cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );

				if (	
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))	   == TRUE || 	
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))	   == TRUE || 	
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))	   == TRUE		//[진성] 영웅의 동상 완성시 방어력 증가. => 2008-6-12
					)
					percent += 10;
			}

			siBufAC = percent;
			temPhysicalDRate += percent;
		}
	}
	else if( gamemode == GAMEMODE_CLIENT )
	{
		if ( (pclchar) && ( 1 == pclchar->GetID()) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			SI16 percent = 0 ;
			SI16 add = 0;

			if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
				percent += 20;

			if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
				percent += 20;
			
			if( clBuf.IsHaveBuf(BUF_MENTOR) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_VALENTINEDAY) )
				percent += 10;

			if( clBuf.IsHaveBuf(BUF_DEFENSEUP) )
			{
				SI32 siBufIndex = pclchar->pclCI->clBI.clBufInfo.FindBufSlot(BUF_DEFENSEUP);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_DEFENSEUP30) )
			{
				SI32 siBufIndex = pclchar->pclCI->clBI.clBufInfo.FindBufSlot(BUF_DEFENSEUP30);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
					percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
			}

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					percent += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}

			// 크리스마스트리에 의한 방어력 증가
			//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. C]
			cltCharClient* pclcharclient = (cltCharClient*)pclchar;
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
					if( 
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))   ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))	 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))				//[진성] 영웅의 동상 완성시 방어력 증가. => 2008-6-12
					)
					
				{
					percent += 10;
				}
			}	
			siBufAC = percent;
			temPhysicalDRate += percent;
		}
	}

	//--------------------------------------
	// 105.4.6 명칭에 의한 방어력 상승
	//--------------------------------------
	if ( gamemode == GAMEMODE_SERVER)
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
			{
				cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
				if ( pclunit )
				{
					temPhysicalDRate +=  pclunit->GetDefense();
					siNamingAC = pclunit->GetDefense();
				}
			}
			else
			{
				siNamingAC = 0;
			}
		}
	}
	else if ( gamemode == GAMEMODE_CLIENT )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) == 1 )
		{
			if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
			{
				cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
				if ( pclunit )
				{
					temPhysicalDRate += pclunit->GetDefense();
					siNamingAC = pclunit->GetDefense();
				}
			}
			else
			{
				siNamingAC = 0;
			}
		}
	}

	//--------------------------------------
	// 105.4.7 스킬에 의한 물리방어력 상승
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 105.4.8 전투향상술
		//--------------------------------------
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_INCAC;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				temPhysicalDRate += siWeaponSkillRate;
			}
		}

		//--------------------------------------
		// 105.4.9 무기술의 달인
		//--------------------------------------
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_PHYSICALDEFENSE );
			if ( 0 < siSkillMasterRate )
			{
				temPhysicalDRate += siSkillMasterRate;
			}
		}

	}
	
	//--------------------------------------
	// 105.4.7 소환수 공격력 증가 버프에 의한  방어력 상승 - 내가 소환수라면.
	//--------------------------------------
	if (pclchar)
	{
		// 내가 소환수라면
		SI32 parentid = pclchar->GetSummonParentID();
		if(pclClient->pclCM->IsValidID(parentid))
		{
			// 소환수 주인이 소환수 공격력, 방어력 증가 버프를 가지고 있다면 
			SI32 summonskill = 0;

			if(pclskillinfo)
			{
				summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
			}
			// 소환수 주인의 소환술 스킬 레벨에 따라 소환수의 방어력을 증가시킨다.

			SI32 defBonusPercent = 0;
			defBonusPercent = pclClient->GetSummonDefBonus( summonskill );

			//TotalAC += TotalAC * defBonusPercent / 100;		
			temPhysicalDRate += defBonusPercent;

			SI32 bufBonusPercent = 0;

			if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
			{
				// 기존 보너스 보다 적게 들어가진 않는다
				bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SUMMONATACUP) );
			}    

			if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
			{
				// 기존 보너스 보다 적게 들어가진 않는다
				bufBonusPercent = max( bufBonusPercent , 50 );
			}   

			if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_EVENT_EGGMOONCAKE ) == true )
			{
				bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_EVENT_EGGMOONCAKE) );
			}

			//TotalAC += TotalAC * bufBonusPercent / 100;
			temPhysicalDRate += bufBonusPercent;
		}
	}
		
	return temPhysicalDRate;
}


////  106. 마법 방어율
void cltParametaBox::SetMagicalDefensiveRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo)
{
	SI32 totalWis = clTotalBA.GetWis();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 106.1 설정되어있는 기본 방어력  - 캐릭터에 따라 리소스에 설정.
	//--------------------------------------
	//SI32 DefaultAC = pclKI->clDefaultWeaponInfo.GetDefaultAC(); // - 일단 안쓴다.

	//--------------------------------------
	// 106.2  지혜에 따른 마법 방어력 설정.
	//--------------------------------------

	siWisMagicalDefensive = (totalWis+level)*90/100 ;

	//--------------------------------------
	// 106.3  장비 따른 마법 방어력 설정. 
	//--------------------------------------
	SI16 Edefansive = 0;
	SI32 CrysTalPer = 0;

	if(pclitemlist)
	{
		Edefansive = (SI16)pclItemManager->GetEquipCrystalMagicalDefensiveRate(pclitemlist, activeweaponpos,pclchar , &CrysTalPer); 

		if(Edefansive>0)
			siEquipMagicalDefensive = Edefansive;
		else
			siEquipMagicalDefensive = 0;
	}

	//--------------------------------------
	// 106.4  스킬에 따른 마법 방어력
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	SI32 siWeaponSkillRate = 0;
	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 106.5 무기술의 달인
		//--------------------------------------
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		SI32 siSkillUnique		= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_MAGICALDEFENSE );
			if ( 0 < siSkillMasterRate )
			{
				siWeaponSkillRate += siSkillMasterRate;
			}
		}
	}

	//--------------------------------------
	// 106.5 결혼
	//--------------------------------------
	SI16 siMarriageMagicalDefensive = 0;

	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) && (pclchar->IsMarried()) )	bMarriageCondition = true;
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) && (pclchar->IsMarried()) )		bMarriageCondition = true;
		}

		if ( (pclClient->m_pclMarriageMgr) && (true == bMarriageCondition) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode()) )
		{
			SI16 siMarriageValue = (SI16)pclClient->m_pclMarriageMgr->GetTotalDefenseAbilityValue( pclchar->pclMarriage->GetLoveLevel() );
			if ( 0 < siMarriageValue )
			{
				siMarriageMagicalDefensive = siMarriageValue;
			}
		}
	}

	//--------------------------------------
	// 마을 강화제로 인한 마법방어력 상승
	//--------------------------------------
	SI32 siMagicDefRate_ByBuf = 0;
	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );

			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					siMagicDefRate_ByBuf += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}
		}
	}

	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		if ( (pclchar) && ( 1 == pclchar->GetID()) )
		{
			cltBufInfo clBuf;
			clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
			
			if( clBuf.IsHaveBuf(BUF_RESIDENTS_STRONG) )
			{
				SI32 siBufIndex = clBuf.FindBufSlot(BUF_RESIDENTS_STRONG);
				if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
				{
					siMagicDefRate_ByBuf += clBuf.m_stBuf[siBufIndex].siPercent;
				}
			}
		}
	}

	// 쿠키 효과
	SI32 siCookieEffect = 0 ;
	if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
	{
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() ))  )
			{
				if(pclchar->pclFamily != NULL )
				{
					if( pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )	// 쿠키효과는 자식일때만 된다
					{
						if(pclchar->pclFamily->GetOnParentNum() > 0 )
						{
							cltServer* pclserver = (cltServer*)pclClient;
							SI32 siEndDateVary = 0;
							SI32 siNowDateVary = 0;
							SI32 siCookieLevel	=	0	;
							SI32 siEffect	=	0;
							siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
							siNowDateVary = pclserver->pclTime->clDate.GetHourVary()	;
							if(  siEndDateVary > 0 && siEndDateVary > siNowDateVary )
							{
								siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;
								siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;

								siCookieEffect	=	siEffect;
							}
						}
					}	
				}
			}
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) )
			{
				if( pclchar->pclFamily != NULL )
				{
					if( pclchar->pclFamily->GetMyClass_InFamily() ==  MYCLASS_IN_FAMILY_CHILDREN)
					{
						if(pclchar->pclFamily->GetOnParentNum() > 0)
						{
							SI32 siEndDateVary = 0;
							SI32 siNowDateVary = 0;
							SI32 siCookieLevel	=	0	;
							SI32 siEffect	=	0;

							siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
							siNowDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;
							if(  siEndDateVary > 0 && siEndDateVary > siNowDateVary )
							{
								siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;
								siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;

								siCookieEffect	=	siEffect;
							}

						}
					}
				}
			}
		}
	}	
	//----------------------------------------------------------------
	// 106.6 탈것(백호)을 타게 되면 마법 방어력이 100 상승한다.(09.05.13)
	//----------------------------------------------------------------
	SI16	siMagicalDefenseHorseAC = 0;

	if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
	{
		if(pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER)
		{
			siMagicalDefenseHorseAC = 100;
		}
	}

	//----------------------------------------------------------------
	// [지연] 세트 효과에 따른 마법 방어력 상승 ( %)
	//----------------------------------------------------------------
	SI32 setMagicDefenceRate = 0;
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		if ( pclchar && IsPC( pclchar->GetID() ) )
		{
			if( pclchar->m_siSetItemType ) 
			{
				setMagicDefenceRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_DEFENCE, USE_SETITEM_1);
				setMagicDefenceRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_DEFENCE, USE_SETITEM_2);
				setMagicDefenceRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_DEFENCE, USE_SETITEM_3);
				setMagicDefenceRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_DEFENCE, USE_SETITEM_4);
				setMagicDefenceRate  +=  pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MAGIC_DEFENCE, USE_SETITEM_5);
			}
		}
	}

	//--------------------------------------
	// 106.7  % 마법 방어력 합
	//--------------------------------------
	SI32 siMagicalDefensiveRate = CrysTalPer + siWeaponSkillRate + setMagicDefenceRate + siMagicDefRate_ByBuf;		// [지연] 버프에 의한 마법방어력 증가 효과 추가.

	//--------------------------------------
	// 106.8  최종 마법 방어력 설정.  
	//--------------------------------------
	Edefansive =siWisMagicalDefensive +  siEquipMagicalDefensive + siMarriageMagicalDefensive + siMagicalDefenseHorseAC  ;

	if( siMagicalDefensiveRate>0 && siCookieEffect<= 0)
	{
		siMagicalDefensive = Edefansive + (Edefansive *siMagicalDefensiveRate /100) ;
	}
	else if( siCookieEffect> 0 && siMagicalDefensiveRate <= 0 )
	{
		siMagicalDefensive = Edefansive + (Edefansive *siCookieEffect /100) ;
	}
	else if (siCookieEffect > 0 && siMagicalDefensiveRate > 0 )
	{
		siMagicalDefensive = Edefansive + (Edefansive *(siCookieEffect + siMagicalDefensiveRate ) /100) ;
	}
	else
		siMagicalDefensive = Edefansive ;

	if( pclchar )
	{
		if( pclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_DEFENSE, pclchar->GetID() ) )
		{
			siMagicalDefensive += (siMagicalDefensive * pclchar->m_clCharCurseEffect.GetPara()) / 100; 
		}

		if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			siMagicalDefensive += 5 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siMagicalDefensive * 5) / 100 : 0;
		}
	}
		
	
	//--------------------------------------
	// 기타: 방어율의 최종값은 GetDefensiveRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------	

}

////  107. 물리 회피율
void cltParametaBox::SetPhysicalDodgeRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo)
{
	SI32 totalDex = clTotalBA.GetDex();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 107.1 물리 회피 등급 설정. -기본값.
	//--------------------------------------
	SI16 temDodgerateRank = 0;

	if ( (totalDex >0) && (level > 0) )
	{
		temDodgerateRank = ((totalDex * 3)+level);
	}
	else
	{
		temDodgerateRank = 0;
	}
	
	//--------------------------------------
	// 107.2 장비에의한 물리 회피율 향상을 구한다. 
	//--------------------------------------
	SI16 siCrystalDodgeRank = 0;
	if(pclitemlist)
	{
		SI16 EDodgeRate = pclItemManager->GetEquipCrystalPhysicalDodgeRate(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 회피률 향상  - % 
		if ( 0 < EDodgeRate )
		{
			siCrystalDodgeRank = temDodgerateRank * EDodgeRate / 100;
		}
	}

	//--------------------------------------
	// 107.3 스킬에 의한 물리 회피율 향상을 구한다. 
	//--------------------------------------
	SI16	siWeaponDodgeRank	= 0;
	bool	bCalcWeaponSkill	= false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 107.3.1 전투향상술
		//--------------------------------------
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCDR;			}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_SWORD_INCDR;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_SWORD_INCDR;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_SWORD_INCDR;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_SWORD_INCDR;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_SWORD_INCDR;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siWeaponDodgeRank = temDodgerateRank * siWeaponSkillRate / 100;
			}
		}

		//--------------------------------------
		// 107.3.2 무기술의 달인
		//--------------------------------------
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_PHYSICALDODGE );
			if ( 0 < siSkillMasterRate )
			{
				siWeaponDodgeRank +=  siSkillMasterRate;
			}
		}
	}

	//--------------------------------------
	// 107.4 장비에의한 물리 회피등급 설정 - 장비/전투향상술 포함 최종값.
	//--------------------------------------
	siPHysicalDodgeRateRank = temDodgerateRank + siCrystalDodgeRank + siWeaponDodgeRank;

	//--------------------------------------
	// 기타: 회피률의 최종값은 GetDodgeRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------
}

////  108. 마법 회피율
void cltParametaBox::SetMagicalDodgeRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo)
{
	SI32 totalDex = clTotalBA.GetDex();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 108.1 장비에의한 마법 회피율 향상을 구한다. 
	//--------------------------------------
	SI16 EDodgeRate = 0;
	if(pclitemlist)
	{
		EDodgeRate = pclItemManager->GetEquipCrystalMagicalDodgeRate(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 마법 회피율 향상  - % 

		if(EDodgeRate >0)
			siEquipMagicalDodgeRate = EDodgeRate;
		else
			siEquipMagicalDodgeRate = 0;
	}

	//--------------------------------------
	// 108.2 스킬에 의한 마법 회피율 향상을 구한다. 
	//--------------------------------------
	SI16	siWeaponDodgeRate	= 0;
	bool	bCalcWeaponSkill	= false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 108.2.1 무기술의 달인
		//--------------------------------------
		SI32 siItemType		= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		SI32 siSkillUnique	= pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_MAGICALDODGE );
			if ( 0 < siSkillMasterRate )
			{
				siWeaponDodgeRate += siSkillMasterRate;
			}
		}
	}

	//--------------------------------------
	// 108.2 마법 회피 등급 설정.   -기본값.
	//--------------------------------------
	SI16 temDodgerateRank = 0;
	
	if((totalDex >0) && (level > 0))
		temDodgerateRank = ((totalDex * 2)+level);
	else
		temDodgerateRank = 0;

	//--------------------------------------
	// 108.3 마법 회피 % 증가값 설정
	//--------------------------------------
	SI32 siMagicalDodgeRate = siEquipMagicalDodgeRate + siWeaponDodgeRate;

	//--------------------------------------
	// 108.4 장비에의한 마법 회피등급 설정 - 장비 포함 최종값.
	//--------------------------------------
	if(siMagicalDodgeRate >0)
		siMagicalDodgeRateRank = temDodgerateRank + (temDodgerateRank * siMagicalDodgeRate /100); //크리스탈에 의한 회피율 향상 수치를 더한다.
	else
		siMagicalDodgeRateRank = temDodgerateRank ; //크리스탈에 의한 회피율 향상 수치를 더한다.

	//--------------------------------------
	// 기타: 회피률의 최종값은 GetDodgeRate() 에서 , 상대방의 레벨에 따라 결정된다. 
	//--------------------------------------
}

////  109. 체력 
void cltParametaBox::SetHitPoint(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos, bool bnpc, cltPSkillInfo* pclskillinfo)
{
	SI32 totalVit = clTotalBA.GetVit();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 109.1  생명력에 따른 체력 설정.
	//--------------------------------------
	siVitHitpoint = (totalVit * (8000 + (level *5)))/1000 ;	// 여기 식을 바꾸게 되면, 반드시 DoMsg_GAMEMSG_REQUEST_MAKENEWCHAR가서 초기값 계산식두 바꿔줘야 한다.
	
	//--------------------------------------
	// 109.2 장비에의한 체력 향상을 구한다. 
	//--------------------------------------
	SI16 EHitpointPer = 0;
	
	if(pclitemlist)
	{
		EHitpointPer = pclItemManager->GetEquipCrystalHitpoint(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 회피률 향상  - % 

		if(EHitpointPer>0)
			siEquipHitpoint = EHitpointPer;
		else
			siEquipHitpoint = 0;
	}

	//--------------------------------------
	// 109.3  최종 체력 설정.  
	//--------------------------------------
	if(siEquipHitpoint >0)
		siTotalHitpoint = siVitHitpoint + (siVitHitpoint *siEquipHitpoint /100) ;
	else
		siTotalHitpoint = siVitHitpoint ;

	//--------------------------------------
	// 109.4  전투향상술에 의한 생명력 증가
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCVIT;			}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_SWORD_INCVIT;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_SWORD_INCVIT;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_SWORD_INCVIT;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_SWORD_INCVIT;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_SWORD_INCVIT;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siTotalHitpoint = siTotalHitpoint + (siTotalHitpoint * siWeaponSkillRate / 100);
			}
		}
	}

	//--------------------------------------
	// 109.5  버프 마법에 의한 생명력 증가
	//--------------------------------------
	if ( (pclchar) && (IsPC(pclchar->GetID())) )
	{
		siTotalHitpoint += pclchar->m_clMagicBufMgr.GetIncHP( pclchar );
	}

	//--------------------------------------
	// 109.5 결혼 관련 생명력 증가
	//--------------------------------------
	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		bool bMarriageCondition = false;
		if ( GAMEMODE_SERVER == pclClient->GameMode )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) && (pclchar->IsMarried()) )	bMarriageCondition = true;
		}
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) && (pclchar->IsMarried()) )		bMarriageCondition = true;
		}

		if ( (pclClient->m_pclMarriageMgr) && (true == bMarriageCondition) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclchar->pclMarriage->GetConnectMode()) )
		{
			SI32 siMarriageValue = pclClient->m_pclMarriageMgr->GetTotalLifeAbilityValue( pclchar->pclMarriage->GetLoveLevel() );
			if ( 0 < siMarriageValue )
			{
				siTotalHitpoint += siMarriageValue;
			}
		}
	}
	//--------------------------------------
	// 109.5  PVP/PVP_League/SetItem에 의한 생명력 증가
	//--------------------------------------
	if (pclchar != NULL) 
	{
		// pvp나 pvp_league 시 5배 증가
		if ( pclchar->siEnemUnique > 0 ) 
		{
				siTotalHitpoint = siTotalHitpoint * 5;
		}
		else if ( pclchar->GetPVP_LeagueEnemyPersonID() > 0 ) 
		{
			siTotalHitpoint = siTotalHitpoint * 5;
		}
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
		{
			if( IsGuildWarMap_NEW( pclchar->GetMapIndex() ) )
			{
				if( 0 < pclchar->pclCI->clBI.clGuildRank.GetEnemyGuild() )// 적길드 유니크가 있으면 
				{
					siTotalHitpoint = siTotalHitpoint * 5;
				}
			}
		}
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if( pclchar->m_siSetItemType )
			{
				siTotalHitpoint +=	 siTotalHitpoint *  ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_1 ) 
														+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_2 )
														+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_3 )
														+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_4 )
														+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_5 )) * 0.01;
			}
		}

		if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT))
		{
			siTotalHitpoint += 3 <= pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_GIANTRACCOON_RESURRECT_EVENT)->siPercent ? (siTotalHitpoint * 10) / 100 : 0;
		}
	}

	//--------------------------------------
	// 109.5 기타 예외 설정.  
	//--------------------------------------
	if((pclchar!= NULL) && (pclitemlist != NULL)&&(pclchar->GetKind() != KIND_SYSTEM))
	{
		if(pclchar->pclCI->clIP.GetLife() > siTotalHitpoint)
		{
			pclchar->pclCI->clIP.SetLife(siTotalHitpoint);
		}
	}	
	
	
	/*	if(pclchar!= NULL)
	{
		if(pclchar->GetChangeEquipAbility() >= 0) // 장착 혹은 탈착이 헌었다. - 다시 설정한다.
		{	
			if(pclchar->GetLife() >siTotalHitpoint)
				pclchar->SetLife(siTotalHitpoint);

			pclchar->SetChangeEquipAbility(-1); //다시 초기화.
		}
	}
*/
}

////  110. 마법력
void cltParametaBox::SetManaPoint(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo)
{
	SI32 totalMag = clTotalBA.GetMag();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 110.1 지력에 따른 마나 설정.
	//--------------------------------------
	siMagManapoint = (totalMag * (4000 + (level *5)))/1000 ;	// 여기 식을 바꾸게 되면, 반드시 DoMsg_GAMEMSG_REQUEST_MAKENEWCHAR가서 초기값 계산식두 바꿔줘야 한다.
	
	//--------------------------------------
	// 110.2 장비에의한 마나 향상을 구한다. 
	//--------------------------------------
	SI16 EManapointPer = 0;
	
	if(pclitemlist)
	{
		EManapointPer = pclItemManager->GetEquipCrystalManapoint(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 회피률 향상  - % 

		if(EManapointPer>0)
			siEquipManapoint = EManapointPer;
		else
			siEquipManapoint = 0;
	}

	//--------------------------------------
	// 110.3  최종 마나 설정.  
	//--------------------------------------
	if(siEquipManapoint >0)
		siTotalManapoint = siMagManapoint + (siMagManapoint *siEquipManapoint /100) ;
	else
		siTotalManapoint = siMagManapoint ;

	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		if( pclchar && pclchar->m_siSetItemType )
		{
			siTotalManapoint +=  siTotalManapoint * ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_1 )
													+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_2 )
													+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_3 )
													+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_4 )
													+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_5 )) * 0.01;
		}
	}

	if ( (pclchar) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_STAFF_INCMAG;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_STAFF_INCMAG;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_STAFF_INCMAG;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_STAFF_INCMAG;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_STAFF_INCMAG;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_INCMAG;			}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siTotalManapoint += (siTotalManapoint * siWeaponSkillRate / 100);
			}
		}
	}


	
	//--------------------------------------
	// 110.4 기타 예외 설정.  
	//--------------------------------------
	if((pclchar!= NULL) && (pclitemlist != NULL)&&(pclchar->GetKind() != KIND_SYSTEM))
	{
		if(pclchar->pclCI->clIP.GetMana() > siTotalManapoint)
			pclchar->pclCI->clIP.SetMana(siTotalManapoint);
	}	
	/*	if(pclchar!= NULL)
	{
		if(pclchar->GetChangeEquipAbility() >= 0) // 장착 혹은 탈착이 헌었다. - 다시 설정한다.
		{	
			if(pclchar->GetMana() >siTotalManapoint)
				pclchar->SetMana(siTotalManapoint);
			else if(pclchar->GetMana() <0)
				pclchar->SetMana(1);

			pclchar->SetChangeEquipAbility(-1); //다시 초기화.
		}
	}
*/
}

////  111. 무게량
void cltParametaBox::SetWeight(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc)
{
	SI32 totalStr = clTotalBA.GetStr();
	SI32 level = pclIP->GetLevel();

	//--------------------------------------
	// 109.1  힘에 따른 무게량 설정.
	//--------------------------------------
	siStrWeigt = (totalStr * 200) + (level *800);	
	
	//--------------------------------------
	// 109.2 장비에의한 무게량 향상을 구한다. 
	//--------------------------------------
	SI16 EWeightPer = 0;
	
	if(pclitemlist)
	{
		EWeightPer = pclItemManager->GetEquipCrystalWeigt(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 회피률 향상  - % 

		if(EWeightPer>0)
			siEquipWeigt = EWeightPer;
		else
			siEquipWeigt = 0;
	}

	//--------------------------------------
	// 109.3  최종 무게량 설정.  
	//--------------------------------------
	if(siEquipWeigt > 0)
		siTotalWeigt = siStrWeigt + (siStrWeigt *siEquipWeigt /100) ;
	else
		siTotalWeigt = siStrWeigt ;

}

////  112. 크리티컬 설정.
void cltParametaBox::SetCritcalRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltPSkillInfo*	pclskillinfo,cltSummonScroll*	pclsummonscroll)
{
	if(bnpc == false)
	{
		if(TABS(pclClient->GetFrame() - uiLastGetCriticalRateFrame )  > 100)
		{
			uiLastGetCriticalRateFrame = pclClient->GetFrame();

			SI32 skilllevel = 0;
			switch(siAttackType)
			{
			case ATTACKTYPE_SWORD:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESWORD1, 0, 0, pclitemlist);
				}
				break;
			case ATTACKTYPE_SPEAR:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESPEAR1, 0, 0, pclitemlist);
				}
				break;
			case ATTACKTYPE_AXE:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEAXE1, 0, 0, pclitemlist);
				}
				break;
			case ATTACKTYPE_STAFF:
			case ATTACKTYPE_STAFF1:
			case ATTACKTYPE_STAFF2:
			case ATTACKTYPE_NEW_STAFF2:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESTAFF1, 0, 0, pclitemlist);
				}
				break;
			case ATTACKTYPE_BOW:
			case ATTACKTYPE_BOW1:
			case ATTACKTYPE_NEW_BOW1:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEBOW1, 0, 0, pclitemlist);
				}
				break;
			case ATTACKTYPE_GUN:
			case ATTACKTYPE_GUN1:
			case ATTACKTYPE_NEW_GUN1:
				if(pclskillinfo)
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEGUN1, 0, 0, pclitemlist);
				}
				break;
			}

			siCriticalRate = 0;
			if(skilllevel > 30)
			{
				siCriticalRate += ((skilllevel - 30) * 2);
				skilllevel = 30;
			}
			if(skilllevel > 20)
			{
				siCriticalRate += ((skilllevel - 20) * 5);
				skilllevel = 20;
			}

      
			siCriticalRate += (skilllevel * 8);

			// 무기에 추가로 부과된 Critical Rate를 부과한다.
			if(pclitemlist)
			{
				//siCriticalRate += pclItemManager->GetEquipCriticalHitRate(pclitemlist, activeweaponpos);				
				siCriticalRate += pclItemManager->GetEquipCrystalCriticalHitRate(pclitemlist, activeweaponpos,pclchar);				
			}

			// 스킬에 부여된 크리티컬율증가
			SI32 siWeaponSkillCriticalRate = 0;

			if ( (pclchar) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
			{
				// 전투향상술
				SI32 siSkillUnique		= 0;
				SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
				switch ( siItemType )
				{
					case ITEMTYPE_AXE:	{	siSkillUnique = SKILL_AXE_INCCRI;	}	break;
				}

				if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
				{
					SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
					SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
					if ( 0 < siWeaponSkillRate )
					{
						siWeaponSkillCriticalRate += siWeaponSkillRate;
					}
				}

				// 무기술의 달인
				siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
				if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
				{
					SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
					SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_CRITICAL );
					if ( 0 < siSkillMasterRate )
					{
						siWeaponSkillCriticalRate += siSkillMasterRate;
					}
				}

				if ( 0 < siWeaponSkillCriticalRate )
				{
					siCriticalRate = siCriticalRate + (siCriticalRate * siWeaponSkillCriticalRate / 100);
				}
			}

			siCriticalRate = min(MAX_SCROLL_EFFECT, siCriticalRate);
		}

		//KHY - 1220 - 변신시스템.
		if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
		{
			if(siAttackType == ATTACKTYPE_SWORD)
			{								
				siCriticalRate += (SI32)(siTransFormSyc/2);
			}
			//도끼 변신 100% 효과 변경
			else if(siAttackType == ATTACKTYPE_AXE)
			{
				//AXE 100% - 무기 딜레이.
				if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
				{
					DWORD dTransFormAXEInterval = 1000;

					dwAttackInterval = dTransFormAXEInterval;
					//pclItemManager->pclItemInfo[ref]->SetAttackInterval(dTransFormAXEInterval);
				}
			}
		}
		
		SI32 siWeaponSkillAttackIntervalRate = 0;

		siMagicDecDelay = 0;

		if ( (pclchar) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
		{
			// 전투향상술에 의한 공격주기 감소
			SI32 siSkillUnique		= 0;
			SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
			/*
			switch ( siItemType )
			{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_AXE_DECWEAINT;	}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_AXE_DECWEAINT;	}	break;
				case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_DECWEAINT;		}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_AXE_DECWEAINT;	}	break;
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_AXE_DECWEAINT;	}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_AXE_DECWEAINT;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					siWeaponSkillAttackIntervalRate += siWeaponSkillRate;
				}
			}
			*/

			// 무기술의 달인에 의한 공격주기 감소
			siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
				SI32 siSkillMasterRate	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_ATTACKINTERVAL );
				if ( 0 < siSkillMasterRate )
				{
					siWeaponSkillAttackIntervalRate += siSkillMasterRate;
				}
			}

			if ( 0 < siWeaponSkillAttackIntervalRate )
			{
				dwAttackInterval = dwAttackInterval - (dwAttackInterval * siWeaponSkillAttackIntervalRate / 100);
			}

			// 전투향상술에 의한 마법사용주기 감소
			siSkillUnique = 0;
			switch ( siItemType )
			{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_GUN_DECMAGDELAY;	}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_GUN_DECMAGDELAY;	}	break;
				case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_GUN_DECMAGDELAY;		}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_GUN_DECMAGDELAY;		}	break;
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_DECMAGDELAY;			}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_GUN_DECMAGDELAY;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					siMagicDecDelay = siWeaponSkillRate;
				}
			}
		}
		
	}
	else	// NPC
	{
		siCriticalRate = pclKI->siCriticalRate;			//Kind기본의 필살기 확률이 적용된다.

		//----------------------------------------------------
		// 소환수 부적에 의한 Critical Hit 향상
		//----------------------------------------------------
		if(pclsummonscroll)
		{
			siCriticalRate	+= pclsummonscroll->GetScrollCriticalHitRate(pclItemManager);
		}

		siCriticalRate = min(MAX_SCROLL_EFFECT, siCriticalRate);

	}

}

void cltParametaBox::SetEtcSetting(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltPSkillInfo*	pclskillinfo,cltSummonScroll*	pclsummonscroll, cltPersonHorseInfo* pclhorseinfo)
{
	//--------------------------------------------------
	// 이동 속도를 구한다. 
	//--------------------------------------------------
	BasicMoveSpeed		= pclKI->siMoveSpeed;
	
	if(pclitemlist)
	{
		if (pclClient->IsCountrySwitch(Switch_StatusBug)) {
			ShoesMoveSpeed		= pclItemManager->GetMoveSpeed(pclitemlist);
		}
		else{
			ShoesMoveSpeed		= pclItemManager->GetMoveSpeed(pclitemlist,pclchar);
		}
		
	}
	else
	{
		ShoesMoveSpeed = 0;
	}

	if(GivenMoveSpeed)
	{
		TotalMoveSpeed		= GivenMoveSpeed;
	}
	else
	{
		// 말을 타고 있다면, 
		if(pclhorseinfo && pclhorseinfo->siCurrentHorseRIDEHORSE == true)
		{
			cltHorse* pclhorse = pclhorseinfo->GetCurrentShowHorse();
			if (pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN) ||			//해피빈 이벤트 해피타임을 먹으면 이속최대 [2007.08.27 손성웅]
						    pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY)) )	//[추가 : 황진성 2007. 11. 12 추수 감사절 이벤트. 이동속도 최대]
			{ 
				TotalMoveSpeed = 20;
			}
			else if(pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MENTOR)))
			{
				if( 15 > BasicMoveSpeed + ShoesMoveSpeed && 15 > pclhorse->GetRealMoveSpeed())
				{
					TotalMoveSpeed = 15;
				}
				else if(pclhorse)
					TotalMoveSpeed	= pclhorse->GetRealMoveSpeed();		// 말의 이동속도로 속도를 대체한다.
			}
			else
			{
				if(pclhorse)
					TotalMoveSpeed	= pclhorse->GetRealMoveSpeed();		// 말의 이동속도로 속도를 대체한다. 

			}
		}
			
		// 말을 타지 않은 상태라면, 
		else 
		{
			if (pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN) ||			//해피빈 이벤트 해피타임을 먹으면 이속최대 [2007.08.27 손성웅]
							pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY)) )	//[추가 : 황진성 2007. 11. 12 추수 감사절 이벤트. 이동속도 최대] 
			{ 
				TotalMoveSpeed = 20;
			}
			else if(pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MENTOR)))
			{
				if( 15 > BasicMoveSpeed + ShoesMoveSpeed )
				{
					TotalMoveSpeed = 15;
				}
			}
			else
			{// 버프도 없고 말도 타지 않은 상태라면
				TotalMoveSpeed		= BasicMoveSpeed + ShoesMoveSpeed;
			}			
		}
	}

	//--------------------------------------
	// 109.3  소환수 스테미나 설정.  
	//--------------------------------------
	if (pclchar)
	{
		// 내가 소환수라면
		SI32 parentid = pclchar->GetSummonParentID();
		if(pclClient->pclCM->IsValidID(parentid))
		{
			SI32 summonskill = 0;

			if(pclskillinfo)
			{
				summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
			}
			
			MaxStamina		= pclClient->GetMaxStaminaValue(summonskill, pclchar->GetKind());

		}
	}
	else
		MaxStamina		= 10;

	//--------------------------------------
	// 109.4  스킬에 의한 사거리증가
	//--------------------------------------
	bool bCalcWeaponSkill = false;
	if ( GAMEMODE_SERVER == pclClient->GameMode )
	{
		if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
	}

	if ( (bCalcWeaponSkill) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
	{
		//--------------------------------------
		// 109.4.1  전투향상술 %로 증가
		//--------------------------------------
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_BOW_INCRANGE;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_BOW_INCRANGE;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_BOW_INCRANGE;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_INCRANGE;			}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_BOW_INCRANGE;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_BOW_INCRANGE;	}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siAttackRange += (siAttackRange * siWeaponSkillRate / 100);
			}
		}

		//--------------------------------------
		// 109.4.2  무기술의 달인 절대값으로 증가
		//--------------------------------------
		siSkillUnique = pclClient->pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclskillinfo->GetSkillExpLevel( siSkillUnique );
			SI32 siSkillMasterValue	= pclClient->pclSkillManager->m_clMasterManager.GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_ATTACKRANGE );
			if ( 0 < siSkillMasterValue )
			{
				siAttackRange += siSkillMasterValue;
			}
		}
	}
	
}

// 현재의 정보를 근거로 공격 속성을 파악한다. 
void cltParametaBox::CalculateAttackType(cltCharCommon* pclchar, SI16 activeweaponpos, cltItem* pclitemlist)
{

	SI32 attacktype = 0;
	SI32 attackskill = 0 ;

	if(pclitemlist)
	{
		attacktype = pclItemManager->GetWeaponAttackType(pclitemlist, activeweaponpos);
		SI32 ref = pclItemManager->FindItemRefFromUnique(pclitemlist[activeweaponpos].siUnique);
		if ( ref )
		{
			attackskill = pclItemManager->pclItemInfo[ref]->clCondition.siSkillKind ;
		}
	}

	// 무장을 하고 있는 상태면 무기의 공격 타입을 사용한다. 
	if(pclitemlist && attacktype)
	{
		siAttackType		= attacktype;
		siAttackRange		= pclItemManager->GetWeaponAttackRange(pclitemlist, activeweaponpos);

		//--------------------------------------
		// 109.3 변신 관련. 
		//--------------------------------------
		//KHY - 1220 - 변신시스템.
		if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
		{
			if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
			{
				switch(siAttackType)
				{ 
					case ATTACKTYPE_BOW:
					case ATTACKTYPE_GUN:
						siAttackRange += 1;
						break;
				}
			}
		}

		if(siAttackType == ATTACKTYPE_STAFF1 || siAttackType == ATTACKTYPE_STAFF2 || siAttackType == ATTACKTYPE_NEW_STAFF2)
		{
			if(pclIP->GetMana() < 2)
			{
				siAttackType	= ATTACKTYPE_STAFF;
				siAttackRange	= 1;
			}
		}

		dwAttackInterval	= pclItemManager->GetWeaponAttackInterval(pclitemlist, activeweaponpos);
		siArmWeaponUnique	= pclitemlist[activeweaponpos].GetType(pclItemManager);
	}
	else if(clShapeEquip.clWeapon.uiUnique)
	{
		SI32 ref = pclItemManager->FindItemRefFromUnique(clShapeEquip.clWeapon.uiUnique);
		if(ref > 0)
		{
			siAttackType		= pclItemManager->pclItemInfo[ref]->GetAttackType();
			siAttackRange		= pclItemManager->pclItemInfo[ref]->GetAttackRange();

			dwAttackInterval	= pclItemManager->pclItemInfo[ref]->GetAttackInterval();
			siArmWeaponUnique	= pclItemManager->pclItemInfo[ref]->siType;

			attackskill = pclItemManager->pclItemInfo[ref]->clCondition.siSkillKind ;
		}
	}
	else
	{
		// by LEEKH 2007.12.28 TRANSFORM
		if( pclchar != NULL && pclchar->clTransFormInfo.bTransFormMode )
		{
			// 비무장이면 캐릭터 고유의 공격 타입을 사용한다.
			siAttackType		= pclchar->GetKindInfo()->clDefaultWeaponInfo.GetAttackType();		
			// 비무장이면 캐릭터 고유의 사거리를 얻어온다. 
			//siAttackRange		= pclchar->GetKindInfo()->clDefaultWeaponInfo.GetAttackRange();
			//dwAttackInterval	= pclchar->GetKindInfo()->dwAttackInterval;
			siArmWeaponUnique	= 0;
		}
		else
		{
			// 비무장이면 캐릭터 고유의 공격 타입을 사용한다.
			siAttackType		= pclKI->clDefaultWeaponInfo.GetAttackType();		
			// 비무장이면 캐릭터 고유의 사거리를 얻어온다. 
			siAttackRange		= pclKI->clDefaultWeaponInfo.GetAttackRange();
			dwAttackInterval	= pclKI->dwAttackInterval;
			siArmWeaponUnique	= 0;
		}

		// by LEEKH 2007.12.28 TRANSFORM
		/*		if( pclchar != NULL && pclchar->clTransFormInfo.bTransFormMode )
		{
		// 비무장이면 캐릭터 고유의 공격 타입을 사용한다.
		siAttackType		= pclchar->GetKindInfo()->clDefaultWeaponInfo.GetAttackType();		
		// 비무장이면 캐릭터 고유의 사거리를 얻어온다. 
		//siAttackRange		= pclchar->GetKindInfo()->clDefaultWeaponInfo.GetAttackRange();
		//dwAttackInterval	= pclchar->GetKindInfo()->dwAttackInterval;
		siArmWeaponUnique	= 0;
		}*/
	}

}

// 전투 능력을 수치화해서 얻어온다. 
SI32 cltParametaBox::GetBattlePower()
{
	SI32 power = pclIP->siStr + pclIP->siDex + pclIP->siMag + pclIP->siVit;

	return power / 2;
}
SI32 cltParametaBox::GetBattleLevel()
{
	SI32 battleLevel =( 
		sqrt( (double)((pclIP->siDex-7)*48 ) ) + 
		sqrt( (double)((pclIP->siVit-6)*14 ) ) 
		)/2;

	battleLevel = max( 1, battleLevel );

	return battleLevel;
}
SI32 cltParametaBox::GetBossBattleLevel()
{
	SI32 battleLevel =( 
		sqrt( (double)(pclIP->siDex*35 ) ) + 
		sqrt( (double)((pclIP->siVit+1200)*5 ) ) 
		)/2 + (pclIP->siMag/25);

	battleLevel = max( 1, battleLevel );

	return battleLevel;
}


// bnpc인지 여부. 
// bmonsterhero : 몬스터에서 진화한 소환영웅인가. 
BOOL cltParametaBox::Action(cltCharCommon* pclchar, SI32 personid, SI32 activeweaponpos, 
							cltItem*			pclitemlist,	// 장비품 아이템 리스트 . 				
							//cltPItemInfo* pclcharitem, 
							cltPersonHorseInfo* pclhorseinfo, 
							cltHealthInfo*		pclhealthinfo, 
							cltPSkillInfo*		pclskillinfo, 
							cltIdentity*		pclidentityinfo, 
							SI16				siAttackWearStoneNum,
							SI16				siDefenseWearStoneNum,
							SI32 gamemode, bool bnpc,
							bool bmonsterhero
							)
{

	// 클라이언트에서...
	if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		cltClient* pclclient = (cltClient*)pclClient;
		// 주인공의 정보를 받지 못한 상태라면, 능력치 계산을 하지 않는다.
		if( pclchar != NULL && pclchar->GetCharUnique() == pclclient->MyCharUnique )
		{
			if( pclclient->bReceiveMyCharAllInfoSwitch == false )
				return FALSE;
		}
	}
	if(pclIP == NULL)return FALSE;

	//--------------------------------------------
	// 건강 관련 행위 
	//--------------------------------------------
	if(gamemode == GAMEMODE_SERVER)
	{
		// personid가 유효함을 반드시 확인해야 한다. 
		if(bnpc == false && personid)
		{
			if(pclhealthinfo && clHealth.Action(pclIP->GetLevel(), pclClient->CurrentClock) == true)
			{
				// 클라이언트에 통보한다. 
				if(pclchar)
				{
					pclchar->SetUpdateSwitch(UPDATE_HEALTH, true, 0);
				}

			}
		}
	}

	// 1. 능력치 계산


	// 1.0 - 기본 능력
	clBasicBA.SetStr(pclIP->GetStr());
	clBasicBA.SetDex(pclIP->GetDex());
	clBasicBA.SetVit(pclIP->GetVit());
	clBasicBA.SetMag(pclIP->GetMag());
	clBasicBA.SetHnd(pclIP->GetHnd());
	clBasicBA.SetLuk(pclIP->GetLuk());
	clBasicBA.SetWis(pclIP->GetWis());

	// 1.1 - 장비
	SetEquipBasicAbility(pclchar, pclitemlist, activeweaponpos);

	// 1.2 - 변신
	SetTransformBasicAbility(pclchar, pclitemlist, activeweaponpos);

	// 1.3 - 말
	SetHorseBasicAbility(pclhorseinfo, pclchar);

	// 1.4 - 건강에 의한 추가 능력치 -10% ~ +10%
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SetHealthAddAbility(pclhealthinfo,
							cltBasicAbility(
								pclIP->GetStr() + clEquipBA.GetStr() + clHorseBA.GetStr() + clTransFormBA.GetStr(),
								pclIP->GetDex() + clEquipBA.GetDex() + clHorseBA.GetDex() + clTransFormBA.GetDex(),
								0, // 생명력 추가 없음
								pclIP->GetMag() + clEquipBA.GetMag() + clHorseBA.GetMag() + clTransFormBA.GetMag(),
								pclIP->GetHnd() + clEquipBA.GetHnd() + clHorseBA.GetHnd() + clTransFormBA.GetHnd(),
								pclIP->GetLuk() + clEquipBA.GetLuk() + clHorseBA.GetLuk() + clTransFormBA.GetLuk(),
								pclIP->GetWis() + clEquipBA.GetWis() + clHorseBA.GetWis() + clTransFormBA.GetWis()
							) );
	}
	else
	{
		SetHealthAddAbility(pclhealthinfo,	cltBasicAbility(	pclIP->GetStr() + clEquipBA.GetStr() + clHorseBA.GetStr() + clTransFormBA.GetStr(),
																pclIP->GetDex() + clEquipBA.GetDex() + clHorseBA.GetDex() + clTransFormBA.GetDex(),
																0, // 생명력 추가 없음
																pclIP->GetMag() + clEquipBA.GetMag() + clHorseBA.GetMag() + clTransFormBA.GetMag(),
																0,	// 손재주 추가 없음
																0,	// 행운 추가 없음
																0	// 지혜 
															) );
	}
	// 1.5 - 길드에 의한 추가 능력치
	SetGuildBasicAbility(pclchar);

	// 1.6 - 세트 아이템에 의한 추가 능력치
	SetSetItemAddAbility(pclchar, pclitemlist);

	// 1.7 - 스킬에 의한 추가 능력치
	SetSkillAddAbility( pclitemlist, activeweaponpos,
						pclskillinfo,
						cltBasicAbility(
							pclIP->GetStr() + clEquipBA.GetStr() + clHorseBA.GetStr() + clTransFormBA.GetStr() + clGuildBA.GetStr(), 
							pclIP->GetDex() + clEquipBA.GetDex() + clHorseBA.GetDex() + clTransFormBA.GetDex() + clGuildBA.GetDex(), 
							pclIP->GetVit() + clEquipBA.GetVit() + clHorseBA.GetVit() + clTransFormBA.GetVit() + clGuildBA.GetVit(), 
							pclIP->GetMag() + clEquipBA.GetMag() + clHorseBA.GetMag() + clTransFormBA.GetMag() + clGuildBA.GetMag(), 
							pclIP->GetHnd() + clEquipBA.GetHnd() + clHorseBA.GetHnd() + clTransFormBA.GetHnd() + clGuildBA.GetHnd(),
							pclIP->GetLuk() + clEquipBA.GetLuk() + clHorseBA.GetLuk() + clTransFormBA.GetLuk() + clGuildBA.GetLuk(),
							pclIP->GetWis() + clEquipBA.GetWis() + clHorseBA.GetWis() + clTransFormBA.GetWis() + clGuildBA.GetWis()
						) );
	
	//[진성] 버프로 인해 스탯에 적용되는 능력치. => 2008-6-18
	SetBufStatusUp(pclchar);

	//[진성] 엘릭서. 
	SetElixirAddAbility( pclchar );

	// PCK : 히나마쯔리 이벤트 기간동안 스텟 상승 (08.12.17)
	SetHinamatsuriAbility( pclchar );

	SetValentineEventAbility( pclchar);

	// 말파츠에 의한 추가 능력치
	//SetPremiumPartsstatusUp( pclchar );

	// [영훈] 수호정령에 의한 능력치 향상
	SetSoulGuardAddAbility( pclchar );

	// [춘기] 크리스마스 이벤트에 의한 능력치 향상
	SetWorldChristmasTreeAbility( pclchar );

	// 1.8 능력치 합산
	clTotalBA.SetStr( clBasicBA.GetStr() + 
					  clEquipBA.GetStr() +
					  clHorseBA.GetStr() +
					  clGuildBA.GetStr() +
					  clHealthAdd.GetStr() + 
					  clSetItemAdd.GetStr() +
					  clSkillAdd.GetStr() +
					  clTransFormBA.GetStr() + 
					  clbufAdd.GetStr()	+
					  clElixirAdd.GetStr() + 		
					  clHinamatsuriAdd.GetStr() +
					  clSoulGuardAdd.GetStr() +
					  clWorldChristmasTreeAdd.GetStr() +
					  clValentineAdd.GetStr()
					  /* +
					  clHorsePartsAdd.GetStr()*/);

	clTotalBA.SetDex( clBasicBA.GetDex() + 
					  clEquipBA.GetDex() +
					  clHorseBA.GetDex() +
					  clGuildBA.GetDex() +
					  clHealthAdd.GetDex() + 
					  clSetItemAdd.GetDex() +
					  clSkillAdd.GetDex() +
					  clTransFormBA.GetDex() + 
					  clbufAdd.GetDex() +
					  clElixirAdd.GetDex() +
					  clHinamatsuriAdd.GetDex() +
					  clSoulGuardAdd.GetDex() + 
					  clWorldChristmasTreeAdd.GetDex() +
					  clValentineAdd.GetDex()
					  /*clHorsePartsAdd.GetDex()*/);

	clTotalBA.SetMag( clBasicBA.GetMag() + 
					  clEquipBA.GetMag() +
					  clHorseBA.GetMag() +
					  clGuildBA.GetMag() +
					  clHealthAdd.GetMag() + 
					  clSetItemAdd.GetMag() +
					  clSkillAdd.GetMag() +
					  clTransFormBA.GetMag() + 
					  clbufAdd.GetMag() + 
					  clElixirAdd.GetMag() + 
					  clHinamatsuriAdd.GetMag() +
					  clSoulGuardAdd.GetMag() +
					  clWorldChristmasTreeAdd.GetMag() +
					  clValentineAdd.GetMag()
					  /*clHorsePartsAdd.GetMag()*/);

	clTotalBA.SetVit( clBasicBA.GetVit() + 
					  clEquipBA.GetVit() +
					  clHorseBA.GetVit() +
					  clGuildBA.GetVit() +
					  clHealthAdd.GetVit() + 
					  clSetItemAdd.GetVit() +
					  clSkillAdd.GetVit() +
					  clTransFormBA.GetVit() + 
					  clbufAdd.GetVit() + 
					  clElixirAdd.GetVit() +
					  clHinamatsuriAdd.GetVit() +
					  clSoulGuardAdd.GetVit() +
					  clWorldChristmasTreeAdd.GetVit() +
					  clValentineAdd.GetVit()
					  /* +
					  clHorsePartsAdd.GetVit()*/);

	clTotalBA.SetHnd( clBasicBA.GetHnd() + 
					  clEquipBA.GetHnd() +
					  clHorseBA.GetHnd() +
					  clGuildBA.GetHnd() +
					  clHealthAdd.GetHnd() + 
					  clSetItemAdd.GetHnd() +
					  clSkillAdd.GetHnd() +
					  clTransFormBA.GetHnd() + 
					  clbufAdd.GetHnd() +
					  clElixirAdd.GetHnd() + 
					  clHinamatsuriAdd.GetHnd() +
					  clSoulGuardAdd.GetHnd() + 
					  clWorldChristmasTreeAdd.GetHnd()+
					  clValentineAdd.GetHnd()
					  /* +
					  clHorsePartsAdd.GetHnd()*/ );

	clTotalBA.SetWis( clBasicBA.GetWis() + 
					  clEquipBA.GetWis() +
					  clHorseBA.GetWis() +
					  clGuildBA.GetWis() +
					  clHealthAdd.GetWis() + 
					  clSetItemAdd.GetWis() +
					  clSkillAdd.GetWis() +
					  clTransFormBA.GetWis() + 
					  clbufAdd.GetWis() +
					  clElixirAdd.GetWis() + 
					  clHinamatsuriAdd.GetWis() +
					  clSoulGuardAdd.GetWis() + 
					  clWorldChristmasTreeAdd.GetWis() +
					  clValentineAdd.GetWis()
					  /*	+
					   clHorsePartsAdd.GetWis()*/
					   );

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		clTotalBA.SetLuk( clBasicBA.GetLuk() +
						clEquipBA.GetLuk() +
						  clHorseBA.GetLuk() +
						  clGuildBA.GetLuk() +
						  clHealthAdd.GetLuk() + 
						  clSetItemAdd.GetLuk() +
						  clSkillAdd.GetLuk()+
						  clTransFormBA.GetLuk() + 
						  clbufAdd.GetLuk());
	}
	else
	{
		if (bnpc == false)
		{
			clTotalBA.SetLuk( clBasicBA.GetLuk() +
							clEquipBA.GetLuk() +
							  clHorseBA.GetLuk() +
							  clGuildBA.GetLuk() +
							  clHealthAdd.GetLuk() + 
							  clSetItemAdd.GetLuk() +
							  clSkillAdd.GetLuk() +
							  clTransFormBA.GetLuk() + 
							  clbufAdd.GetLuk());
		}
	}

	// 능력치 예외상황
	if(pclidentityinfo)
	{
		switch(pclidentityinfo->siIdentity)
		{
		case IDENTITY_CASTLE:				clTotalBA.SetVit( clTotalBA.GetVit() * pclidentityinfo->clGodInfo.siCapa* 2);	break;
		case IDENTITY_VILLAGEGOD:
			{
				if ( pclKI != NULL && pclKI->IsAtb(ATB_GOD) )
				{
					clTotalBA.SetVit( clTotalBA.GetVit() * pclidentityinfo->clGodInfo.siCapa * 2);
				}
				else
				{
					clTotalBA.SetVit( clTotalBA.GetVit() * pclidentityinfo->clGodInfo.siCapa * 1.6 );
				}

				// [영훈] 구스텟공식일때만 아래식 적용
				if( false == pclClient->IsCountrySwitch(Switch_HealthAddChange) )
				{
					if ( pclIP->Life > clTotalBA.GetVit() * 4 )
					{
						pclIP->SetLife(clTotalBA.GetVit() * 4);
					}
				}
				
			}
			break;
		case IDENTITY_VILLAGEGOD_SUMMON:	clTotalBA.SetVit( clTotalBA.GetVit() * pclidentityinfo->clGodInfo.siCapa* 2);	break;
		case IDENTITY_PERSON_SUMMON:		break;
		case IDENTITY_VILLAGEHUNT:			break;
		default:							break;
		}
	}

	bSetTotalBA = true;

	//--------------------------------------
	// 장비에 의한 운을 구한다. 
	//--------------------------------------
	if(bnpc == false)
	{
		if(pclitemlist)
			siEquipLuck	= pclItemManager->GetLuck(pclitemlist);
		else
			siEquipLuck = 0;
	}

	siTotalLuck	+= siEquipLuck;

	////////////////////////////////////// /////////////////////////////////////////////////////////////////////////
	////  100. 기타 기본 설정
	////  101. 물리 공격력 
	////  102. 마법 공격력 
	////  103. 물리 명중률 
	////  104. 마법 명중률 
	////  105. 물리 방어력 
	////  106. 마법 방어력 
	////  107. 물리 회피율 
	////  108. 마법 회피율 
	////  109. 체력
	////  110. 마법력 
	////  111. 무게량 (?)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//--------------------------------------
	////  100. 기타 기본 설정
	//--------------------------------------

	//--------------------------------------
	// 공격 타입을 정한다. 
	//--------------------------------------
	CalculateAttackType(pclchar, activeweaponpos, pclitemlist);


	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		bool bacswitch = false;
		bool bdamageswitch = false;	
		
		//--------------------------------------
		////  101. 물리 공격력 
		//--------------------------------------
		SetPhysicalAttackDemage(pclchar,pclitemlist, activeweaponpos, bnpc ,gamemode , pclskillinfo  ,pclidentityinfo , &bdamageswitch, siAttackWearStoneNum);

		//--------------------------------------
		////  102. 마법 공격력 
		//--------------------------------------
		SetMagicalAttackDemage(pclchar,pclitemlist, activeweaponpos, bnpc ,gamemode , pclskillinfo ,pclidentityinfo , &bdamageswitch);

		//--------------------------------------
		////  103. 물리 명중률 
		//--------------------------------------
		SetPhysicalHitRate(pclchar,pclitemlist, activeweaponpos, bnpc ,pclidentityinfo, pclskillinfo);

		//--------------------------------------
		////  104. 마법 명중률 
		//--------------------------------------
		SetMagicalHitRate(pclchar,pclitemlist, activeweaponpos,bnpc ,pclidentityinfo, pclskillinfo);

		//--------------------------------------
		////  105. 물리 방어력 
		//--------------------------------------
		SetPhysicalDefensiveRate(pclchar,pclitemlist, activeweaponpos,bnpc ,gamemode , pclskillinfo, pclidentityinfo, &bacswitch, siDefenseWearStoneNum);

		//--------------------------------------
		////  106. 마법 방어력 
		//--------------------------------------
		SetMagicalDefensiveRate(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo);

		//--------------------------------------
		////  107. 물리 회피율 
		//--------------------------------------
		SetPhysicalDodgeRate(pclchar,pclitemlist, activeweaponpos, bnpc, pclskillinfo);

		//--------------------------------------
		////  108. 마법 회피율 
		//--------------------------------------
		SetMagicalDodgeRate(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo);

		//--------------------------------------
		////  109. 체력 
		//--------------------------------------
		SetHitPoint(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo);

		//--------------------------------------
		////  110. 마법력 
		//--------------------------------------
		SetManaPoint(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo);

		//--------------------------------------
		////  111. 무게량 
		//--------------------------------------
		SetWeight(pclchar,pclitemlist, activeweaponpos,bnpc);


		//--------------------------------------
		////  112. 크리티컬 확률 및 공격 딜레이(시전속도) 조정.
		//--------------------------------------
		SetCritcalRate(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo , NULL);

		//--------------------------------------
		////  113. 기타등등 조건 설정. - 이동속도, 소환수 스테미나등.
		//--------------------------------------
		SetEtcSetting(pclchar,pclitemlist, activeweaponpos,bnpc, pclskillinfo , NULL , pclhorseinfo);

		if(bacswitch || bdamageswitch)
		{
			// 클라이언트에게 알린다. 
			if(pclchar)
			{

				cltGameMsgResponse_GeneralEft clinfo(pclchar->GetCharUnique(), siGeneralAC   , siGeneralDamage );
				cltMsg clMsg(GAMEMSG_RESPONSE_GENERALEFT, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclchar)->SendNeighbourMsg(&clMsg, true);
			}

		}

	}
	else  /////////////---------------- 이곳부터 이전 파일이다.----------------------------------------------------------////////////////////////
	{
		// 전투향상술을 계산해야하느지를 결정하는 조건
		bool bCalcWeaponSkill = false;
		if ( (GAMEMODE_SERVER == pclClient->GameMode) && (pclitemlist) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
		{
			if ( (pclchar) && (IsPC( pclchar->GetID() )) )	bCalcWeaponSkill = true;
		}
		else if ( (GAMEMODE_CLIENT == pclClient->GameMode)  && (pclitemlist) && (pclitemlist) && (pclskillinfo) && ((PERSONITEM_WEAPON1 == activeweaponpos) || (PERSONITEM_WEAPON2 == activeweaponpos)) )
		{
			if ( (pclchar) && (1 == pclchar->GetID()) )		bCalcWeaponSkill = true;
		}

		//-------------------------------------------
		// 저항력을 구한다. -안쓴다.
		//-------------------------------------------
		if(pclBPB)
		{
			TotalDamageResist	= pclBPB->GetDamageResist(); // 물리 저항력.
			TotalMagicResist	= pclBPB->GetMagicResist();  // 마법 저항력.
		}
		else
		{
			TotalDamageResist	= 0;
			TotalMagicResist	= 0;
		}

		//--------------------------------------
		// 공격 타입을 정한다. 
		//--------------------------------------
		CalculateAttackType(pclchar, activeweaponpos, pclitemlist);

		//--------------------------------------
		// 장비에 의한 운을 구한다. 
		//--------------------------------------
		if(bnpc == false)
		{
			if(pclitemlist)
				siEquipLuck	= pclItemManager->GetLuck(pclitemlist);
			else
				siEquipLuck = 0;
		}

		siTotalLuck	+= siEquipLuck;


		//-----------------------------------
		// 레벨의 의한 데미지, 방어력 구한다. 
		//-----------------------------------
		SI32 level = pclIP->GetLevel();
		if(level != siOldLevel)
		{
			siOldLevel		= level;
			siLevelDamage	= pclIP->CalcLevelDamage();
			siLevelAC		= CalcLevelAC(level);
		}

		//---------------------------------------------------------
		// 인챈트에 의한 데미지, 방어력 구한다.      by tae-yang
		//---------------------------------------------------------
		if(pclitemlist)
		{
			pclBPB->siDefenR		= pclItemManager->GetEnchantAC(pclitemlist,pclchar);
		}

		//--------------------------------------
		// 무장에 의한 AC
		//-------------------------------------
		//-태양
		if(pclitemlist)
		{
			if (pclClient->IsCountrySwitch(Switch_StatusBug))
			{siEquipAC		= pclItemManager->GetAC(pclitemlist,pclchar);}
			else
			{siEquipAC		= pclItemManager->GetAC(pclitemlist,pclchar);}
		}
		else
		{
			siEquipAC = 0;
		}

		//cyj 방어구중에 장착제한에 걸리는것이 있으면 방어력 20% 감소
		if(pclitemlist)
		{
			SI32 returnval = 0;
			if ((pclitemlist[PERSONITEM_HELMET].siUnique > 0 && (NULL != pclchar) && pclchar->CheckEquipCondition(pclClient->pclItemManager, PERSONITEM_HELMET, &pclitemlist[PERSONITEM_HELMET], &returnval) == false) ||
				(pclitemlist[PERSONITEM_ARMOUR].siUnique > 0 && (NULL != pclchar) && pclchar->CheckEquipCondition(pclClient->pclItemManager, PERSONITEM_ARMOUR, &pclitemlist[PERSONITEM_ARMOUR], &returnval) == false) ||
				(pclitemlist[PERSONITEM_BELT].siUnique   > 0 && (NULL != pclchar) && pclchar->CheckEquipCondition(pclClient->pclItemManager, PERSONITEM_BELT,   &pclitemlist[PERSONITEM_BELT],   &returnval) == false) )
			{
				siEquipAC = siEquipAC * 80 / 100;
			}
		}

		// 기본 방어력
		// 소환수이고 장비가 있을 때
		if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (pclitemlist) && 
			( pclitemlist[PERSONITEM_HELMET].siUnique || pclitemlist[PERSONITEM_ARMOUR].siUnique || pclitemlist[PERSONITEM_BELT].siUnique || 
			pclitemlist[PERSONITEM_SHOES].siUnique || pclitemlist[PERSONITEM_MANTLE].siUnique ) 
		)
		{
			// 장비석의 갯수와 현재 레벨이 같아야 기본 데미지 적용
			if ( pclchar->pclCI->clIP.GetLevel() == siDefenseWearStoneNum )
			{
				siDefaultAC = pclKI->clDefaultWeaponInfo.GetDefaultAC(); 
			}
		}
		else if((pclidentityinfo) && pclidentityinfo->siIdentity == IDENTITY_GMCREATE_MONSTER)
		{
			siDefaultAC = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultAC();
		}
		else
		{
			siDefaultAC = pclKI->clDefaultWeaponInfo.GetDefaultAC(); 
		}



		if(bmonsterhero)
		{
			siEvolveAC	= siDefaultAC;
		}
		else if ( pclKI->bSummonMall == TRUE )
		{
			/*
			siEvolveAC	= 0;

			if ( siEquipAC > 0 )
			{
				siDefaultAC = 0 ;
			}
			*/
		}
		else
		{
			siEvolveAC	= 0;
		}


		if ( pclchar ) 
		{

			//KHY - 0124 - 가챠레어- 닌자 변신 아이템.
			if(pclchar->clTransFormInfo.bTransFormMode == true)
			{
				if( (pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAMAN"))			||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAGIRL"))			||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATMELEE"))			||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATRANGE"))			||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN"))  ||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST"))	||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE"))	||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE"))	||
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE"))	||	// 검사스피릿(여)
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE"))	||	// 거너스피릿(남)
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYMELEE"))		||	// 요정스피릿(남)
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYRANGE"))		||	// 거너스피릿(여)
					// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICERANGE"))			||	// 눈의 여왕
					(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICEMELEE"))				// 사스콰치
				  )				
					{
						// 방어력 증가.
						siTransFormAC = 80; // 기본 방어력.
						SI32 temState = 0;
						temState = siTransFormAC * (siTransFormSyc/2) / 100;

						siTransFormAC = siTransFormAC + temState; // 싱크로율 증가분.				
					}
			}
		}

		//-----------------------------------------------
		// 소환수 장비석에 따른 방어력 상승
		//-----------------------------------------------
		SI16 siDefenseWearStoneAC = 0;

		if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (SUMMON_EQUIP_LIMITMINLEVEL < siDefenseWearStoneNum) )
		{
			REAL32 siDefenseWearStoneIncreasePercent = (siDefenseWearStoneNum - SUMMON_EQUIP_LIMITMINLEVEL) * 1.0;

			siDefenseWearStoneAC = (SI16)((REAL32)(siEquipPHysicalDefensive/100) * siDefenseWearStoneIncreasePercent);
		}

		//KHY - 0124 - 가챠레어- 닌자 변신 아이템.
		TotalAC		= siLevelAC + siDefaultAC + siEquipAC + siEvolveAC + siTransFormAC + siDefenseWearStoneAC;

		// Identity 가 존재하는 특수 캐릭터인 경우 , 방어력 
		if(pclidentityinfo)
		{
			switch(pclidentityinfo->siIdentity)
			{
			case IDENTITY_CASTLE:		TotalAC = TotalAC * pclidentityinfo->clGodInfo.siCapa*80/100;	break;
			case IDENTITY_VILLAGEGOD:
			case IDENTITY_VILLAGEGOD_SUMMON:
				{
					SI32 totalac = TotalAC;
					SI32 value = 0;

					if(totalac > 70)	
					{
						value += ((totalac - 70) * pclidentityinfo->clGodInfo.siCapa * 1 / 100);	
						totalac = 70;
					}
					if(totalac > 60)	
					{
						value += ((totalac - 60) * pclidentityinfo->clGodInfo.siCapa * 2 / 100);	
						totalac = 60;
					}
					if(totalac > 50)	
					{
						value += ((totalac - 50) * pclidentityinfo->clGodInfo.siCapa * 3 / 100);	
						totalac = 50;
					}
					if(totalac > 40)	
					{
						value += ((totalac - 40) * pclidentityinfo->clGodInfo.siCapa * 5 / 100);	
						totalac = 40;
					}
					if(totalac > 20)	
					{
						value += ((totalac - 20) * pclidentityinfo->clGodInfo.siCapa * 10 / 100);	
						totalac = 20;
					}
					if(totalac > 10)	
					{
						value += ((totalac - 10) * pclidentityinfo->clGodInfo.siCapa * 20 / 100);	
						totalac = 10;
					}
					if(totalac > 5)
					{
						value += ((totalac - 5) * pclidentityinfo->clGodInfo.siCapa * 30 / 100);		
						totalac = 5; 
					}

					value += (totalac * pclidentityinfo->clGodInfo.siCapa * 40 / 100);					


					TotalAC = value;	
					if(pclidentityinfo->siIdentity == IDENTITY_VILLAGEGOD_SUMMON)	{	TotalAC /= 3;			}
				}
				break;
			case IDENTITY_PERSON_SUMMON:break;
			case IDENTITY_VILLAGEHUNT:	break;
			default:					break;
			}
		}



		// 세트 효과에 의한 AC상승
		if( pclchar &&  
			( pclchar->m_siSetItemType == BOY_SULBIM_SETITEM  || pclchar->m_siSetItemType == GIRL_SULBIM_SETITEM ) )
		{
			TotalAC = TotalAC + 20;
		}

		// 지휘효과에 의한 AC상승. 
		bool bacswitch = false;


		if(siGeneralAC)
		{
			//-------------------------------
			// 게임서버인 경우에는 지휘효과의 시간을 확인한다. 
			//-------------------------------
			if(gamemode == GAMEMODE_SERVER)
			{
				if(TABS(pclClient->CurrentClock - dwGeneralACClock) > 10000)
				{
					siGeneralAC			= 0;
					dwGeneralACClock	= 0;

					bacswitch = true;

				}
			}


			TotalAC = TotalAC * (100 + siGeneralAC ) / 100;
		}

		// 활성제에 의한 AC상승
		if(gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) && pclchar->pclCI->clBI.bCombatService )
			{
				TotalAC = TotalAC * 130 / 100;
			}
		}
		else if(gamemode == GAMEMODE_CLIENT)
		{
			if ( pclchar && pclchar->GetID() == 1 && pclchar->pclCI->clBI.bCombatService )
			{
				TotalAC = TotalAC * 130 / 100;
			}		
		}

		// 버프에 의한 AC 상승
		if( gamemode == GAMEMODE_SERVER )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
				SI16 percent = 0 ;
				SI16 add = 0;

				if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
					percent += 20;

				if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
					percent += 20;

				if( clBuf.IsHaveBuf(BUF_DEFENSEUP) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_DEFENSEUP);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_DEFENSEUP30) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_DEFENSEUP30);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_MENTOR) )
					percent += 10;

				// [진성] 마을에 건축물에 있다면 추가되는 방어력.	
				if( pclchar->pclCI->clBI.siHomeVillage > 0 )
				{
					cltServer* pclserver = (cltServer*)pclClient;
					cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );

					if (
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) 	== TRUE ||
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))	   	== TRUE ||
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))   	== TRUE ||
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))	   	== TRUE || 	
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))	== TRUE || 
							pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))	== TRUE  		//[진성] 영웅의 동상 완성시 방어력 증가. => 2008-6-12
						)				
						percent += 10;
				}

				siBufAC = percent;
				TotalAC += TotalAC * percent / 100;
				TotalAC += add;
			}
		}
		else if( gamemode == GAMEMODE_CLIENT )
		{
			if ( (pclchar) && ( 1 == pclchar->GetID()) )
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
				SI16 percent = 0 ;
				SI16 add = 0;

				if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
					percent += 20;

				if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
					percent += 20;

				if( clBuf.IsHaveBuf(BUF_DEFENSEUP) )
				{
					SI32 siBufIndex = pclchar->pclCI->clBI.clBufInfo.FindBufSlot(BUF_DEFENSEUP);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_DEFENSEUP30) )
				{
					SI32 siBufIndex = pclchar->pclCI->clBI.clBufInfo.FindBufSlot(BUF_DEFENSEUP30);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_MENTOR) )
					percent += 10;

				// 크리스마스트리에 의한 방어력 증가
				//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. C]
				cltCharClient* pclcharclient = (cltCharClient*)pclchar;
				for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
					if( pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))   ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))		 ||		
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))	)			
					{
						percent += 10;
					}
				}	
				siBufAC = percent;
				TotalAC += TotalAC * percent / 100;
				TotalAC += add;
			}
		}

		// 명칭에 의한 방어력 상승
		if ( gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
				{
					cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
					if ( pclunit )
					{
						TotalAC += TotalAC * pclunit->GetDefense() / 100;
						siNamingAC = pclunit->GetDefense();
					}
				}
				else
				{
					siNamingAC = 0;
				}
			}
		}
		else if ( gamemode == GAMEMODE_CLIENT )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) == 1 )
			{
				if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
				{
					cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
					if ( pclunit )
					{
						TotalAC += TotalAC * pclunit->GetDefense() / 100;
						siNamingAC = pclunit->GetDefense();
					}
				}
				else
				{
					siNamingAC = 0;
				}
			}
		}

		// 소환수 공격력 증가 버프에 의한  방어력 상승
		if (pclchar)
		{
			// 내가 소환수라면
			SI32 parentid = pclchar->GetSummonParentID();
			if(pclClient->pclCM->IsValidID(parentid))
			{
				// 소환수 주인이 소환수 공격력, 방어력 증가 버프를 가지고 있다면 
				SI32 summonskill = 0;

				if(pclskillinfo)
				{
					summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
				}
				// 소환수 주인의 소환술 스킬 레벨에 따라 소환수의 방어력을 증가시킨다.

				SI32 defBonusPercent = 0;
				defBonusPercent = pclClient->GetSummonDefBonus( summonskill );

				TotalAC += TotalAC * defBonusPercent / 100;		

				SI32 bufBonusPercent = 0;

				if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
				{
					// 기존 보너스 보다 적게 들어가진 않는다
					bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_SUMMONATACUP) );
				}    

				if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
				{
					// 기존 보너스 보다 적게 들어가진 않는다
					bufBonusPercent = max( bufBonusPercent , 50 );
				}   

				if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_EVENT_EGGMOONCAKE ) == true )
				{
					bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_EVENT_EGGMOONCAKE) );
				}

				TotalAC += TotalAC * bufBonusPercent / 100;

			}

		}
		if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
		{
			// 쿠키효과로 인해ㅔ 방어력이 상승한다.
			if ( gamemode == GAMEMODE_SERVER)
			{
				if ( pclchar && IsPC( pclchar->GetID() )  )
				{
					if(pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)
						{
							if(pclchar->pclFamily->GetOnParentNum() > 0 )
							{
								SI32 siEndDateVary = 0 ;
								SI32 siNowDateVary = 0	;
								SI32 siCookieLevel =	0	;
								SI32 siEffect =		0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
								siNowDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;
								if(  siEndDateVary > 0 && siEndDateVary > siNowDateVary )
								{									
									siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;
									siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;
									TotalAC = TotalAC + (TotalAC * siEffect / 100)	;
								}
							}	
						}						
					}
				}
			}
			else if (gamemode == GAMEMODE_CLIENT)
			{
				if ( pclchar && pclchar->GetID() == 1)
				{
					if(pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)
						{
							if(pclchar->pclFamily->GetOnParentNum() > 0 )
							{
								SI32 siEndDateVary = 0;
								SI32 siNowDateVary = 0;
								SI32 siCookieLevel =	0	;
								SI32 siEffect =		0	;
								siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( pclchar->pclCI->GetPersonID() ) ;
								siNowDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;
								if(  siEndDateVary > 0 &&  siEndDateVary > siNowDateVary )
								{	
									siCookieLevel = pclchar->pclFamily->GetMyCookieLevel(pclchar->pclCI->GetPersonID())	;
									siEffect = pclchar->pclFamily->GetCookieEffect( siCookieLevel)	;

									TotalAC = TotalAC + (TotalAC * siEffect / 100)	;

								}
							}
						}
					}
				}
			}


		}
		// PCK : 탈것(백호)를 타게 되면 방어력이 100 상승한다. (09.05.15)
		if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
		{
			if( pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER )
			{
				TotalAC += 100;
			}
		}

		//--------------------------------------
		// 무장에 의한 최소 데미지 
		//--------------------------------------
		bool bAttackWearStoneCondition	= false;
		SI32 defaultweaponmindamage		= 0;
		SI32 defaultweaponmaxdamage		= 0;

		// 소환수일때 장비가 있다면
		if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (pclitemlist) && (pclitemlist[PERSONITEM_WEAPON1].siUnique) )
		{	
			// 장비석의 갯수와 현재 레벨이 같아야 기본 데미지 적용
			if ( pclchar->pclCI->clIP.GetLevel() == siAttackWearStoneNum )
			{
				bAttackWearStoneCondition	= true;
				defaultweaponmindamage		= pclKI->clDefaultWeaponInfo.GetDefaultMinDamage();
				defaultweaponmaxdamage		= pclKI->clDefaultWeaponInfo.GetDefaultMaxDamage();
			}
		}
		else if((pclidentityinfo) && pclidentityinfo->siIdentity == IDENTITY_GMCREATE_MONSTER)
		{
			defaultweaponmindamage = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultMinDamage();
			defaultweaponmaxdamage = pclGMCommandKindInfo->clDefaultWeaponInfo.GetDefaultMaxDamage();
		}
		// 소환수가 아니거나 장비가 없다면
		else
		{
			defaultweaponmindamage = pclKI->clDefaultWeaponInfo.GetDefaultMinDamage();
			defaultweaponmaxdamage = pclKI->clDefaultWeaponInfo.GetDefaultMaxDamage();
		}

		//----------------------------------------------------
		// 소환수에서 진화한 소환영웅인 경우, 어드벤티지 인정 
		//----------------------------------------------------
		if(bmonsterhero)
		{
			siEvolveDamage = defaultweaponmindamage * 3 / 2;
		}

		// 무장이 있으면 그 무장의 데미지를 구한다. 
		SI32 damage = 0;
		if(pclitemlist)
		{//니다.
			if (pclClient->IsCountrySwitch(Switch_StatusBug)) {
				damage = pclItemManager->GetWeaponMinDamage(pclitemlist, activeweaponpos); 

				//cyj 사용무기가 장착제한에 걸리면 데미지 20% 감소
				SI32 returnval = 0;
				if ((pclitemlist[activeweaponpos].siUnique > 0) && (pclchar != NULL) && (pclchar->CheckEquipCondition(pclClient->pclItemManager, activeweaponpos, &pclitemlist[activeweaponpos], &returnval) == false) )
				{
					damage = damage * 80 / 100;
				}

				// 장비 착용석 조건에 맞느 소환수라면 기본 데미지를 더해준다
				if ( bAttackWearStoneCondition == true )
				{
					damage += defaultweaponmindamage ;
				}
			}
			else{
				damage = pclItemManager->GetWeaponMinDamage(pclitemlist, activeweaponpos, pclchar); 

				//cyj 사용무기가 장착제한에 걸리면 데미지 20% 감소
				SI32 returnval = 0;

				if ((pclitemlist[activeweaponpos].siUnique > 0)  && (pclchar != NULL) &&  (pclchar->CheckEquipCondition(pclClient->pclItemManager, activeweaponpos, &pclitemlist[activeweaponpos], &returnval) == false) )
				{
					damage = damage * 80 / 100;
				}

				// 장비 착용석 조건에 맞느 소환수라면 기본 데미지를 더해준다
				if ( bAttackWearStoneCondition == true )
				{
					damage += defaultweaponmindamage ;
				}
			}
			
		}
		else
		{
			damage = 0;
		}

		if(damage)
		{
			TotalEquipMinDamage = damage;
		}
		else
		{
			// 무장이 없으면 디폴트 데미지를 구한다. 
			TotalEquipMinDamage	= defaultweaponmindamage;
		}
		//-------------------------------------
		//인챈트에 따른 무장데미지  by tae-yang - 안쓴다.
		//-------------------------------------


		SI32 Enchantdamage = 0;
		SI32 EnchantRingdamage = 0;	
		SI32 EnchantType = 0;
		if(pclitemlist)
		{		

			for(SI32 i=0; i < 4; i++)
			{			
				EnchantRingdamage = EnchantRingdamage +  pclItemManager->GetEnchantAttFrom(pclitemlist, (i * 2 ) + 1); 			
			}		

			Enchantdamage =Enchantdamage + EnchantRingdamage;	
			EnchantType = pclItemManager->GetEnchantWeaponType(pclitemlist, activeweaponpos); 
		}
		else
		{
			Enchantdamage = 0;
			EnchantType = 0;
		}

		if(damage)
		{
			pclBPB->siAttackR = Enchantdamage;
			pclBPB->siAttackE = EnchantType;
		}
		else
		{
			// 무장이 없으면 디폴트 인챈트 데미지를 구한다. 
			//pclBPB->siAttackR 	= 0;
			//pclBPB->siAttackE = 0;
		}
		//--------------------------------------
		// 무장에 의한 최대 데미지 
		//--------------------------------------
		if(pclitemlist)
		{

			if (pclClient->IsCountrySwitch(Switch_StatusBug)) {
				damage = pclItemManager->GetWeaponMaxDamage(pclitemlist, activeweaponpos); 
				/*		// 소환영웅인 경우에는 디폴트 데미지를 더해준다.
				if ( pclKI->bSummonMall == TRUE )
				{
				damage += defaultweaponmaxdamage ;
				}*/
				//cyj 사용무기가 장착제한에 걸리면 데미지 20% 감소
				SI32 returnval = 0;
				if ( (pclitemlist[activeweaponpos].siUnique > 0) && (pclchar != NULL) && (pclchar->CheckEquipCondition(pclClient->pclItemManager, activeweaponpos, &pclitemlist[activeweaponpos], &returnval) == false) )
				{
					damage = damage * 80 / 100;
				}

				// 장비 착용석 조건에 맞느 소환수라면 기본 데미지를 더해준다
				if ( bAttackWearStoneCondition == true )
				{
					damage += defaultweaponmaxdamage ;
				}

			}
			else
			{
				damage = pclItemManager->GetWeaponMaxDamage(pclitemlist, activeweaponpos, pclchar); 
				/*		// 소환영웅인 경우에는 디폴트 데미지를 더해준다.
				if ( pclKI->bSummonMall == TRUE )
				{
				damage += defaultweaponmaxdamage ;
				}*/
				//cyj 사용무기가 장착제한에 걸리면 데미지 20% 감소
				SI32 returnval = 0;
				if ( (pclitemlist[activeweaponpos].siUnique > 0)  && (pclchar != NULL) && (pclchar->CheckEquipCondition(pclClient->pclItemManager, activeweaponpos, &pclitemlist[activeweaponpos], &returnval) == false) )
				{
					damage = damage * 80 / 100;
				}

				// 장비 착용석 조건에 맞느 소환수라면 기본 데미지를 더해준다
				if ( bAttackWearStoneCondition == true )
				{
					damage += defaultweaponmaxdamage ;
				}
			}

		}
		else
		{
			damage = 0;
		}

		if(damage)
		{
			TotalEquipMaxDamage = damage;
		}
		else
		{
			// 무장이 없으면 디폴트 데미지를 구한다. 
			TotalEquipMaxDamage	= defaultweaponmaxdamage;
		}

		//----------------------------------------------------
		// 소환수 장비석에 따른 데미지 상승
		//----------------------------------------------------
		SI16 siAttackWearStoneMinDmg = 0;
		SI16 siAttackWearStoneMaxDmg = 0;

		if ( (pclidentityinfo) && (IDENTITY_PERSON_SUMMON == pclidentityinfo->siIdentity) && (SUMMON_EQUIP_LIMITMINLEVEL < siAttackWearStoneNum) )
		{
			REAL32 siAttackWearStoneIncreasePercent = (siAttackWearStoneNum - SUMMON_EQUIP_LIMITMINLEVEL) * 1.0;

			siAttackWearStoneMinDmg = (SI16)((REAL32)(TotalEquipMinDamage/100) * siAttackWearStoneIncreasePercent);
			siAttackWearStoneMaxDmg = (SI16)((REAL32)(TotalEquipMaxDamage/100) * siAttackWearStoneIncreasePercent);
		}

		//-----------------------------------
		// 근력에 의한 데미지. 
		//-----------------------------------
		// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
		//if( pclClient->IsWhereServiceArea( ConstSwitch_PVP ) )
		if(pclClient->siEnablePVP == 1)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				siStrDamage		= clTotalBA.GetStrDamage(true);
			}
			else
			{
				siStrDamage		= clTotalBA.GetStrDamage(false);
			}
		}
		else
			siStrDamage		= clTotalBA.GetStrDamage(false);

		//----------------------------------
		// 무기사용술 개발에 의한 데미지. 
		//----------------------------------
		if(bnpc == false)
		{
			if(TABS(pclClient->GetFrame() - uiLastGetCriticalRateFrame )  > 100)
			{
				uiLastGetCriticalRateFrame = pclClient->GetFrame();

				SI32 skilllevel = 0;
				switch(siAttackType)
				{
				case ATTACKTYPE_SWORD:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESWORD1, 0, 0, pclitemlist);
					}
					break;
				case ATTACKTYPE_SPEAR:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESPEAR1, 0, 0, pclitemlist);
					}
					break;
				case ATTACKTYPE_AXE:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEAXE1, 0, 0, pclitemlist);
					}
					break;
				case ATTACKTYPE_STAFF:
				case ATTACKTYPE_STAFF1:
				case ATTACKTYPE_STAFF2:
				case ATTACKTYPE_NEW_STAFF2:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESTAFF1, 0, 0, pclitemlist);
					}
					break;
				case ATTACKTYPE_BOW:
				case ATTACKTYPE_BOW1:
				case ATTACKTYPE_NEW_BOW1:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEBOW1, 0, 0, pclitemlist);
					}
					break;
				case ATTACKTYPE_GUN:
				case ATTACKTYPE_GUN1:
				case ATTACKTYPE_NEW_GUN1:
					if(pclskillinfo)
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USEGUN1, 0, 0, pclitemlist);
					}
					break;
				}

				siCriticalRate = 0;
				if(skilllevel > 30)
				{
					siCriticalRate += ((skilllevel - 30) * 2);
					skilllevel = 30;
				}
				if(skilllevel > 20)
				{
					siCriticalRate += ((skilllevel - 20) * 5);
					skilllevel = 20;
				}

	      
				siCriticalRate += (skilllevel * 8);

				// 무기에 추가로 부과된 Critical Rate를 부과한다.
				if(pclitemlist)
				{
					siCriticalRate += pclItemManager->GetEquipCriticalHitRate(pclitemlist, activeweaponpos);				
				}
				
				siMagicDecDelay = 0;
				if ( true == bCalcWeaponSkill )
				{
					// 전투향상술에 의한 필살률 증가
					SI32 siSkillUnique		= 0;
					SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
					switch ( siItemType )
					{
						case ITEMTYPE_AXE:	{	siSkillUnique = SKILL_AXE_INCCRI;	}	break;
					}

					if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
					{
						SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
						SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
						if ( 0 < siWeaponSkillRate )
						{
							siCriticalRate = siCriticalRate + (siCriticalRate * siWeaponSkillRate / 100);
						}
					}

					// 전투향상술에 의한 공격주기 감소
					/*
					siSkillUnique = 0;
					switch ( siItemType )
					{
						case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_AXE_DECWEAINT;	}	break;
						case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_AXE_DECWEAINT;	}	break;
						case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_DECWEAINT;		}	break;
						case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_AXE_DECWEAINT;	}	break;
						case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_AXE_DECWEAINT;	}	break;
						case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_AXE_DECWEAINT;	}	break;
					}

					if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
					{
						SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
						SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
						if ( 0 < siWeaponSkillRate )
						{
							dwAttackInterval = dwAttackInterval - (dwAttackInterval * siWeaponSkillRate / 100);
						}
					}
					*/

					// 전투향상술에 의한 마법사용주기 감소
					siSkillUnique = 0;
					switch ( siItemType )
					{
						case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_GUN_DECMAGDELAY;	}	break;
						case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_GUN_DECMAGDELAY;	}	break;
						case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_GUN_DECMAGDELAY;		}	break;
						case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_GUN_DECMAGDELAY;		}	break;
						case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_DECMAGDELAY;			}	break;
						case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_GUN_DECMAGDELAY;	}	break;
					}

					if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
					{
						SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0, pclitemlist );
						SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
						if ( 0 < siWeaponSkillRate )
						{
							siMagicDecDelay = siWeaponSkillRate;
						}
					}
				}
				/*
				// 전투향상술에 의한 추가필살율 및 마법 시전 속도 줄이기를 설정한다.
				SI32 itemtype = 0 ;
				if ( pclitemlist )
					itemtype = pclItemManager->GetItemType(pclitemlist[activeweaponpos].siUnique) ;
				skilllevel = 0 ;
				siMagicDecDelay = 0 ;
				switch ( itemtype )
				{
				case ITEMTYPE_AXE:
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_AXE_INCCRI, 0, 0, pclitemlist);
						if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
						{
							SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW2,skilllevel) ;

							// SJY DEBUG
							siCriticalRate = siCriticalRate + siCriticalRate * rate / 100 ;
						}
					}
					break;
				case ITEMTYPE_GUN:
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_GUN_DECMAGDELAY, 0, 0, pclitemlist);
						if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
						{
							// SJY DEBUG
							siMagicDecDelay = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW2,skilllevel) ;
						}
					}
					break;
				}
				*/

				siCriticalRate = min(MAX_SCROLL_EFFECT, siCriticalRate);
			}

			//KHY - 1220 - 변신시스템.
			if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
			{
				if(siAttackType == ATTACKTYPE_SWORD)
				{								
					siCriticalRate += (SI32)(siTransFormSyc/2);
				}
				//도끼 변신 100% 효과 변경
				else if(siAttackType == ATTACKTYPE_AXE)
				{
					//AXE 100% - 무기 딜레이.
					if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
					{
						DWORD dTransFormAXEInterval = 1000;

						dwAttackInterval = dTransFormAXEInterval;
						//pclItemManager->pclItemInfo[ref]->SetAttackInterval(dTransFormAXEInterval);
					}
				}
			}
		}
		else	// NPC
		{

			siCriticalRate = pclKI->siCriticalRate;			//Kind기본의 필살기 확률이 적용된다. 
			//----------------------------------------------------
			// 소환수 부적에 의한 Critical Hit 향상
			//----------------------------------------------------
			/*
			if(pclsummonscroll)
			{
				siCriticalRate	+= pclsummonscroll->GetScrollCriticalHitRate(pclItemManager);
			}
			*/
			siCriticalRate = min(MAX_SCROLL_EFFECT, siCriticalRate);
			

		}

		if(bnpc == false)
		{
			//----------------------------------------------------
			// 무기의 기본 명중률
			//----------------------------------------------------
			if(pclitemlist)
			{
				siWeaponDefaultHitRate	= pclitemlist[activeweaponpos].GetDefaultHitRate(pclItemManager);
			}
			else
			{
				siWeaponDefaultHitRate = 0;
			}

			//KHY - 1220 - 변신시스템.
			if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
			{
				switch(siAttackType)
				{				
					case ATTACKTYPE_SPEAR :
					case ATTACKTYPE_BOW :
						siWeaponDefaultHitRate += (SI32)(siTransFormSyc/20);
						break;
				}
			}

			//----------------------------------------------------
			// 장비에 의한 명중률 향상 
			//----------------------------------------------------
			if(pclitemlist)
			{
				siEquipAssistHitRate	= min(150, pclItemManager->GetEquipHitRate(pclitemlist, activeweaponpos)); 
			}
			else
			{
				siEquipAssistHitRate = 0;
			}

			siEquipAssistHitRate = min(MAX_SCROLL_EFFECT, siEquipAssistHitRate);

			if(pclitemlist)
			{
				//KHY - 0227 - 결정체 (크리스탈) 제조 추가.
				//siAddCrystalPhysicalHitRate = pclItemManager->GetEquipCrystalPhysicalHitRate(pclitemlist, activeweaponpos,pclchar);	// 결정체에의한 물리 명중률 향상  - % 
				//siAddCrystalMagicalHitRate = pclItemManager->GetEquipCrystalMagicalHitRate(pclitemlist, activeweaponpos,pclchar);  // 결정체에의한 마법 명중률 향상  - % 
			}

			//----------------------------------------------------
			// 장비에 의한 데미지 감소. 
			//----------------------------------------------------
			if(pclitemlist)
			{

				siEquipAssistDamageReduceRate	= min(150, pclItemManager->GetDamageReduceRate(pclitemlist)); 
			}
			else
			{
				siEquipAssistDamageReduceRate = 0;
			}

			siEquipAssistDamageReduceRate = min(MAX_SCROLL_EFFECT, siEquipAssistDamageReduceRate);


		}
		else if(pclidentityinfo && pclidentityinfo->siIdentity)	
		{
			// Identity 가 존재하는 특수 캐릭터인 경우 , 명중률 
			switch(pclidentityinfo->siIdentity)
			{
			case IDENTITY_CASTLE:				siEquipAssistHitRate = 1000; break; 
			case IDENTITY_VILLAGEGOD:			siEquipAssistHitRate = 500;	 break;
			case IDENTITY_VILLAGEGOD_SUMMON:	siEquipAssistHitRate = 200;	 break;
			case IDENTITY_PERSON_SUMMON:
				{
					siEquipAssistHitRate = 0;
					//----------------------------------------------------
					// 무기의 기본 명중률
					//----------------------------------------------------
					if(pclitemlist) siWeaponDefaultHitRate	= pclitemlist[activeweaponpos].GetDefaultHitRate(pclItemManager);
					else siWeaponDefaultHitRate = 0;
					//----------------------------------------------------
					// 장비에 의한 명중률 향상 
					//----------------------------------------------------
					if(pclitemlist) siEquipAssistHitRate	= min(150, pclItemManager->GetEquipHitRate(pclitemlist, activeweaponpos)); 
					else siEquipAssistHitRate = 0;

					siEquipAssistHitRate = min(MAX_SCROLL_EFFECT, siEquipAssistHitRate);

					//----------------------------------------------------
					// 장비에 의한 데미지 감소. 
					//----------------------------------------------------
					if(pclitemlist) siEquipAssistDamageReduceRate	= min(150, pclItemManager->GetDamageReduceRate(pclitemlist)); 
					else siEquipAssistDamageReduceRate = 0;

					siEquipAssistDamageReduceRate = min(MAX_SCROLL_EFFECT, siEquipAssistDamageReduceRate);
				}
				break;
			case IDENTITY_VILLAGEHUNT:			siEquipAssistHitRate = 200;	 break;
			default:					break;
			}

			//----------------------------------------------------
			// 소환수 부적에 의한 명중률 향상
			//----------------------------------------------------
			/*
			if(pclsummonscroll)
			{
				siEquipAssistHitRate			= siEquipAssistHitRate + pclsummonscroll->GetScrollHitRate(pclItemManager);
			}
			*/

			siEquipAssistHitRate = min(MAX_SCROLL_EFFECT, siEquipAssistHitRate);

			//----------------------------------------------------
			// 소환수 부적에 의한 Reduce Damage 향상
			//----------------------------------------------------
			/*
			if(pclsummonscroll)
			{
				siEquipAssistDamageReduceRate	= siEquipAssistDamageReduceRate + pclsummonscroll->GetScrollDamageReduceRate(pclItemManager);
			}
			*/

			siEquipAssistDamageReduceRate = min(MAX_SCROLL_EFFECT, siEquipAssistDamageReduceRate);
		}


		//----------------------------------------------------
		// 변수들의 최대치를 구한다.
		//----------------------------------------------------
		//MaxLife				= clTotalBA.GetVit() * VIT_LIFE_RATE;
		//MaxMana				= clTotalBA.GetMag() * INT_MANA_RATE;

		// 전투향상술에의한 마나력증가
		MaxMana = clTotalBA.GetMag() * INT_MANA_RATE;

		if ( true == bCalcWeaponSkill )
		{
			SI32 siSkillUnique		= 0;
			SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
			switch ( siItemType )
			{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_STAFF_INCMAG;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_STAFF_INCMAG;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_STAFF_INCMAG;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_STAFF_INCMAG;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_STAFF_INCMAG;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_INCMAG;			}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					MaxMana = MaxMana + (MaxMana * siWeaponSkillRate / 100);
				}
			}
		}


		MaxLife = clTotalBA.GetVit() * VIT_LIFE_RATE;
		
		// 전투향상술에의한 체력증가
		if ( true == bCalcWeaponSkill )
		{
			SI32 siSkillUnique		= 0;
			SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
			switch ( siItemType )
			{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCVIT;			}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_SWORD_INCVIT;	}	break;
				case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_SWORD_INCVIT;		}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_SWORD_INCVIT;		}	break;
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_SWORD_INCVIT;		}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_SWORD_INCVIT;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					MaxLife = MaxLife + (MaxLife * siWeaponSkillRate / 100);
				}
			}
		}

		if(pclchar!= NULL)
		{
			//SI32 siParentID = pclchar->GetSummonParentID();
			// PVP나 PVP_League시 5배 증가.
			if(pclchar->siEnemUnique > 0)
			{
				MaxLife = MaxLife * 5	;
			}
			else if( pclchar->GetPVP_LeagueEnemyPersonID() > 0 )
			{
				MaxLife = MaxLife * 5	;
			}

			if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 적길드 유니크가 있으면 hp5배 뻥튀기.
			{
				if( 0 < pclchar->pclCI->clBI.clGuildRank.GetEnemyGuild() )
				{
					MaxLife = MaxLife * 5	;
				}
			}

			if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				if( pclchar->m_siSetItemType )
				{
					MaxLife += MaxLife *    ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_1 )
										    + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_2 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_3 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_4 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_HP, USE_SETITEM_5 )) * 0.01;
					
					MaxMana += MaxLife *    ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_1 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_2 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_3 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_4 )
											+ pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_MP, USE_SETITEM_5 )) * 0.01;	
				}
			}
		}

		// 버프에 의한 생명력 추가
		if ( (pclchar) && (IsPC(pclchar->GetID())) )
		{
			MaxLife += pclchar->m_clMagicBufMgr.GetIncHP( pclchar );
		}

		if((pclchar!= NULL) && (pclitemlist != NULL)&&(pclchar->GetKind() != KIND_SYSTEM))
		{
			if(pclchar->pclCI->clIP.GetLife() > MaxLife)
				pclchar->pclCI->clIP.SetLife(MaxLife);
		}

		if((pclchar!= NULL) && (pclitemlist != NULL)&&(pclchar->GetKind() != KIND_SYSTEM))
		{
			//if(pclchar->pclCI->clIP.GetMana() > MaxMana)
				//pclchar->pclCI->clIP.SetMana(MaxMana);
		}

		if (pclchar)
		{
			// 내가 소환수라면
			SI32 parentid = pclchar->GetSummonParentID();
			if(pclClient->pclCM->IsValidID(parentid))
			{
				SI32 summonskill = 0;

				if(pclskillinfo)
				{
					summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
				}
				MaxStamina		= pclClient->GetMaxStaminaValue(summonskill, pclchar->GetKind());

			}
		}
		else
			MaxStamina		= 10;

		//---------------------------------------------------
		// 실제 게임에 사용될 공격력을 구한다.
		//---------------------------------------------------
		if(siAttackType == ATTACKTYPE_STAFF1 || siAttackType == ATTACKTYPE_STAFF2 || siAttackType == ATTACKTYPE_NEW_STAFF2)
		{
			// 지팡이술에 의한 데미지를 구한다. 
			siStaffSkillDamage = 0;
			SI32 staffskilllevel = 0;

			if(pclskillinfo)
			{
				staffskilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_USESTAFF1, 0, 0, pclitemlist);
			}

			if(staffskilllevel > 40)
			{
				siStaffSkillDamage += (staffskilllevel - 40) / 2;
				staffskilllevel	= 40;
			}
			siStaffSkillDamage += (staffskilllevel * 2 / 4);

			if ( pclKI->bSummonMall == TRUE )
			{
				TotalMinDamage		= TotalEquipMinDamage * 2 + GetAddMagicDamage() + siStaffSkillDamage + siLevelDamage + siEvolveDamage;
				TotalMaxDamage		= TotalEquipMaxDamage * 2 + GetAddMagicDamage() + siStaffSkillDamage + siLevelDamage + siEvolveDamage;
			}
			else
			{
				TotalMinDamage		= TotalEquipMinDamage + GetAddMagicDamage() + siStaffSkillDamage + siLevelDamage + siEvolveDamage;
				TotalMaxDamage		= TotalEquipMaxDamage + GetAddMagicDamage() + siStaffSkillDamage + siLevelDamage + siEvolveDamage;
			}
		}
		else if ( siAttackType == ATTACKTYPE_STAFFWIND && pclKI->bSummonMall == TRUE ) // 사명대사일 경우
		{
			if ( pclitemlist )
			{
				TotalMinDamage		= TotalEquipMinDamage * 2 + GetAddMagicDamage() + siLevelDamage + siEvolveDamage;
				TotalMaxDamage		= TotalEquipMaxDamage * 2 + GetAddMagicDamage() + siLevelDamage + siEvolveDamage;
			}
			else
			{
				TotalMinDamage		= TotalEquipMinDamage + GetAddMagicDamage() + siLevelDamage + siEvolveDamage;
				TotalMaxDamage		= TotalEquipMaxDamage + GetAddMagicDamage() + siLevelDamage + siEvolveDamage;
			}
		}
		else
		{
			TotalMinDamage		= TotalEquipMinDamage + siStrDamage + siLevelDamage + siTorpedoDamage	+ siEvolveDamage;
			TotalMaxDamage		= TotalEquipMaxDamage + siStrDamage + siLevelDamage + siTorpedoDamage	+ siEvolveDamage;

			//KHY - 1220 - 변신시스템.
			if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
			{
				switch(siAttackType)
				{ 
					case ATTACKTYPE_SWORD:
					case ATTACKTYPE_SPEAR:
						{
							if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
							{
								TotalMinDamage += 20;
								TotalMaxDamage += 20;
							}
						}
						break;
					case ATTACKTYPE_AXE:
						{
							TotalMinDamage += (SI32)(siTransFormSyc/2);
							TotalMaxDamage += (SI32)(siTransFormSyc/2);
						}
						break;					
					case ATTACKTYPE_GUN:
						{
							TotalMinDamage += (SI32)((REAL32)siTransFormSyc*(REAL32)0.4);
							TotalMaxDamage += (SI32)((REAL32)siTransFormSyc*(REAL32)0.4);
						}
						break;					
				}
			}		
		}

		// Identity 가 존재하는 특수 캐릭터인 경우 , 공격력 
		if(pclidentityinfo)
		{

			switch(pclidentityinfo->siIdentity)
			{
			case IDENTITY_CASTLE:		
				{
					TotalMinDamage *= pclidentityinfo->clGodInfo.siCapa * 10 /100;
					TotalMaxDamage *= pclidentityinfo->clGodInfo.siCapa * 10 /100;		
				}
				break;
			case IDENTITY_VILLAGEGOD:	
			case IDENTITY_VILLAGEGOD_SUMMON:
				{
					SI32 totalmindamage = TotalMinDamage;
					SI32 totalmaxdamage = TotalMaxDamage;
					SI32 value = 0;
					SI32 capa	= pclidentityinfo->clGodInfo.siCapa;

					if(totalmindamage > 50)
					{
						value += ((totalmindamage - 50) * capa * 1 / 100);	
						totalmindamage = 50;
					}
					if(totalmindamage > 40)
					{
						value += ((totalmindamage - 40) * capa * 2 / 100);	
						totalmindamage = 40;
					}
					if(totalmindamage > 30)
					{
						value += ((totalmindamage - 30) * capa * 3 / 100);	
						totalmindamage = 30;
					}
					if(totalmindamage > 20)
					{
						value += ((totalmindamage - 20) * capa * 5 / 100);	
						totalmindamage = 20;
					}
					if(totalmindamage > 10)
					{
						value += ((totalmindamage - 10) * capa * 10 / 100);	
						totalmindamage = 10;
					}
					if(totalmindamage > 5)
					{
						value += ((totalmindamage - 5) * capa * 20 / 100);	
						totalmindamage = 5;
					}
					value += (totalmindamage * capa * 30 / 100);	

					TotalMinDamage = value;	

					value = 0;

					if(totalmaxdamage > 50)
					{
						value += ((totalmaxdamage - 50) * capa * 1 / 100);	
						totalmaxdamage = 50;
					}
					if(totalmaxdamage > 40)
					{
						value += ((totalmaxdamage - 40) * capa * 2 / 100);	
						totalmaxdamage = 40;
					}
					if(totalmaxdamage > 30)
					{
						value += ((totalmaxdamage - 30) * capa * 3 / 100);	
						totalmaxdamage = 30;
					}
					if(totalmaxdamage > 20)
					{
						value += ((totalmaxdamage - 20) * capa * 5 / 100);	
						totalmaxdamage = 20;
					}
					if(totalmaxdamage > 10)
					{
						value += ((totalmaxdamage - 10) * capa * 10 / 100);	
						totalmaxdamage = 10;
					}
					if(totalmaxdamage > 5)
					{
						value += ((totalmaxdamage - 5) * capa * 20 / 100);	
						totalmaxdamage = 5;
					}
					value += (totalmaxdamage * capa * 30 / 100);	

					TotalMaxDamage = value;	
				}
				break;
			case IDENTITY_PERSON_SUMMON:
				{
					TotalMinDamage = TotalMinDamage + (SI32)(TotalMinDamage * PERSON_SUMMON_ATTACKBONUS_PERCENT * 0.01f) + siAttackWearStoneMinDmg;
					TotalMaxDamage = TotalMaxDamage + (SI32)(TotalMaxDamage * PERSON_SUMMON_ATTACKBONUS_PERCENT * 0.01f) + siAttackWearStoneMaxDmg;

					if (pclchar)
					{
						// 내가 소환수라면
						SI32 parentid = pclchar->GetSummonParentID();
						if(pclClient->pclCM->IsValidID(parentid))
						{
							SI32 summonskill = 0;

							if(pclskillinfo)
							{
								summonskill = pclClient->pclCM->CR[parentid]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON,
									0,
									0,
									&pclClient->pclCM->CR[parentid]->pclCI->clCharItem.clItem[0], parentid );
							}

							// 소환수 주인의 소환술 스킬 레벨에 따라 소환수의 공격력을 증가시킨다.
							SI32 attackBonusPercent = 0;
							attackBonusPercent = pclClient->GetSummonAttackBonus( summonskill );

							TotalMinDamage += TotalMinDamage * attackBonusPercent / 100;
							TotalMaxDamage += TotalMaxDamage * attackBonusPercent / 100;

							// 소환수 주인이 소환수 공격력, 방어력 증가 버프를 가지고 있다면 

							SI32 bufBonusPercent = 0;

							if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_SUMMONATACUP ) == true )
							{
								bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_SUMMONATACUP ) );
							}   
							if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) == true )
							{
								bufBonusPercent = max( bufBonusPercent , 20 );
							}   

							if( pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_EVENT_EGGMOONCAKE ) == true )
							{
								bufBonusPercent = max( bufBonusPercent , pclClient->pclCM->CR[parentid]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_EVENT_EGGMOONCAKE ) );
							}

							TotalMinDamage += TotalMinDamage * bufBonusPercent / 100;
							TotalMaxDamage += TotalMaxDamage * bufBonusPercent / 100;
						}

					}

				}
				break;
			case IDENTITY_VILLAGEHUNT:								break;
			default:												break;
			}
		}
		
		// PCK : 불해치를 타고 있을 경우 공격력 + 100을 해준다.
		// [춘기] 백호를 타고 있을 경우 공격력 + 100을 해준다. (09.05.15)
		// [춘기] 바포메트를 타고 있을 경우 공격력 + 100을 해준다. (09.07.30)
		// [기형] 사수리를 타고 있을 경우 공격력 + 100을 해준다. (09.12.31)
		if((pclchar) && (IsPC(pclchar->GetID())) && (pclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
		{
			if( pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_FIREHATCH || 
				pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_WHITETIGER ||
				pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_BAPHOMET ||
				pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()->siHorseUnique == HORSEUNIQUE_SASURI)
			{
				TotalMinDamage += 100;
				TotalMaxDamage += 100;
			}
		}

		// 지휘효과에 의한 데미지상승. 
		bool bdamageswitch = false;	
		if(siGeneralDamage)
		{		
			//-------------------------------
			// 게임서버인 경우에는 지휘효과의 시간을 확인한다. 
			//-------------------------------
			if(gamemode == GAMEMODE_SERVER)
			{
				if(TABS(pclClient->CurrentClock - dwGeneralDamageClock) > 10000)
				{
					siGeneralDamage			= 0;				
					dwGeneralDamageClock	= 0;

					bdamageswitch = true;
				}
			}

			TotalMinDamage = TotalMinDamage * (100 + siGeneralDamage ) / 100;
			TotalMaxDamage = TotalMaxDamage * (100 + siGeneralDamage ) / 100;
		}

		// 전투향상술에 의한 공격력을 결정한다.
		if ( true == bCalcWeaponSkill )
		{
			// 예전 스테이터스의 경우 물리/마법 공격력이 나누어져 있지 않기 때문에 두값모두 더해준다
			SI32 siSkillUnique		= 0;
			SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );

			// 물리공격력향상
			switch ( siItemType )
			{
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_INCWEAAT;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					TotalMinDamage = TotalMinDamage + (TotalMinDamage * siWeaponSkillRate / 100);
					TotalMaxDamage = TotalMaxDamage + (TotalMinDamage * siWeaponSkillRate / 100);
				}
			}

			// 마법공격력향상
			siSkillUnique = 0;
			switch ( siItemType )
			{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCMAGAT;	}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_INCMAGAT;	}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_INCMAGAT;		}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_INCMAGAT;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					TotalMinDamage = TotalMinDamage + (TotalMinDamage * siWeaponSkillRate / 100);
					TotalMaxDamage = TotalMaxDamage + (TotalMinDamage * siWeaponSkillRate / 100);
				}
			}

			
		}
		/*
		//	if ( gamemode == GAMEMODE_SERVER )
		{
			if(bnpc == false && personid)
			{
				SI32 itemtype = 0 ;
				if ( pclitemlist )
					itemtype = pclItemManager->GetItemType(pclitemlist[activeweaponpos].siUnique) ;
				SI32 skilllevel = 0 ;

				switch ( itemtype )
				{
				case ITEMTYPE_SWORD:
					{
						//skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_SWORD_INCWEAAT, 0, 0, pclitemlist);//
					}
					break;
				case ITEMTYPE_GUN:
					{
						skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_GUN_INCWEAAT, 0, 0, pclitemlist);
					}
					break;
				}

				if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
				{
					// SJY DEBUG
					SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW2,skilllevel) ;

					TotalMinDamage = TotalMinDamage + TotalMinDamage * rate / 100 ;
					TotalMaxDamage = TotalMaxDamage + TotalMaxDamage * rate / 100 ;
				}
			}
		}
		*/

		// 활성제에 의한 Attack상승
		if(gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) && pclchar->pclCI->clBI.bCombatService )
			{
				TotalMinDamage = TotalMinDamage + TotalMinDamage * 30 / 100 ;
				TotalMaxDamage = TotalMaxDamage + TotalMaxDamage * 30 / 100 ;
			}
		}
		else if(gamemode == GAMEMODE_CLIENT)
		{
			if ( pclchar && pclchar->GetID() == 1 && pclchar->pclCI->clBI.bCombatService )
			{
				TotalMinDamage = TotalMinDamage + TotalMinDamage * 30 / 100 ;
				TotalMaxDamage = TotalMaxDamage + TotalMaxDamage * 30 / 100 ;
			}		
		}
		if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
		{
			// 쿠키에 의한 공격력 상승 
			if(gamemode == GAMEMODE_SERVER)
			{
				if ( pclchar && IsPC( pclchar->GetID() ) )
				{
					if ( pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)	// 쿠키 효과는 자식일때만
						{
							if(pclchar->pclFamily->GetOnParentNum() > 0 && pclchar->pclFamily->GetOnChildrenNum() > 0 )
							{
								cltServer* pclserver = (cltServer*)pclClient;
								SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
								if ( siPersonID > 0) 
								{
									
									SI32 siEndDateVary =	0	;
									SI32 siServerDateVary =	0	;
									siEndDateVary =pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
									siServerDateVary = pclserver->pclTime->clDate.GetHourVary();
									if ( siEndDateVary>0 && siServerDateVary>0  ) 
									{
										if(  siEndDateVary  >  siServerDateVary )
										{
											SI32 percent	= 0;
											SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel(siPersonID )	;// 레벨에 따라서 증가
											percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

											TotalMinDamage += TotalMinDamage * percent / 100;
											TotalMaxDamage += TotalMaxDamage * percent / 100;
										}
									}
								}
							}
						}
					}
				}
			}
			else if(gamemode == GAMEMODE_CLIENT)
			{
				if ( pclchar && pclchar->GetID() == 1)
				{
					if( pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN)	// 쿠키 효과는 자식일때만
						{
							if(pclchar->pclFamily->GetOnParentNum() > 0 && pclchar->pclFamily->GetOnChildrenNum() > 0)
							{
								SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
								if ( siPersonID > 0 ) 
								{
									
									SI32 siEndDateVary = 0	;
									SI32 siClientDateVary = 0	;
									siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
									siClientDateVary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;

									if(   siEndDateVary >0 && siClientDateVary > 0 )
									{
										if ( siEndDateVary > siClientDateVary ) 
										{
											SI32 percent	= 0;
											SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( siPersonID )	;// 레벨에 따라서 증가
											percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;
											TotalMinDamage += TotalMinDamage * percent / 100;
											TotalMaxDamage += TotalMaxDamage * percent / 100;
										}
									}

								}
							}

						}
					}				
				}		
			}
		}
		
		// 말 프리미엄 파츠 풀파츠시 특수능력 삭제.
		//// 프리미엄 파츠가 풀파츠 일때 공격력 5% 증가
		//if(gamemode == GAMEMODE_SERVER)
		//{
		//	if ( pclchar && IsPC( pclchar->GetID() ) )
		//	{
		//		cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()	;
		//		if ( pclHorse == NULL)
		//		{
		//		}
		//		else
		//		{
		//			SI32 siEquipPremiumPartsNum = pclHorse->GetPremiumPartsNum()	;
		//			if ( siEquipPremiumPartsNum == 3 )
		//			{
		//				SI32 siHorseUnique = pclHorse->siHorseUnique ;
		//				switch (siHorseUnique )
		//				{
		//				case HORSEUNIQUE_BROWN:	// 갈색마
		//				case HORSEUNIQUE_BLACK:// 흑마
		//				case HORSEUNIQUE_WHITE:// 백마
		//				case HORSEUNIQUE_RED://적토마
		//				case HORSEUNIQUE_RUDOLF ://루돌프
		//				case HORSEUNIQUE_STEEL ://철기마
		//				case HORSEUNIQUE_BEAST ://구미호
		//				case HORSEUNIQUE_THOUSAND ://천리마
		//				case HORSEUNIQUE_ZEBRA ://얼룩마
		//					{
		//						TotalMinDamage = TotalMinDamage + TotalMinDamage * 5 / 100 ;
		//						TotalMaxDamage = TotalMaxDamage + TotalMaxDamage * 5 / 100 ;
		//					}
		//				}
		//			}
		//			
		//		}
		//	}
		//}
		//else if(gamemode == GAMEMODE_CLIENT)
		//{
		//	if ( pclchar && pclchar->GetID() == 1 )
		//	{
		//		cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()	;
		//		if ( pclHorse == NULL)
		//		{
		//		}
		//		else
		//		{
		//			SI32 siEquipPremiumPartsNum = pclHorse->GetPremiumPartsNum()	;
		//			if ( siEquipPremiumPartsNum == 3 )
		//			{
		//				SI32 siHorseUnique = pclHorse->siHorseUnique ;
		//				switch (siHorseUnique )
		//				{
		//				case HORSEUNIQUE_BROWN:	// 갈색마
		//				case HORSEUNIQUE_BLACK:// 흑마
		//				case HORSEUNIQUE_WHITE:// 백마
		//				case HORSEUNIQUE_RED://적토마
		//				case HORSEUNIQUE_RUDOLF ://루돌프
		//				case HORSEUNIQUE_STEEL ://철기마
		//				case HORSEUNIQUE_BEAST ://구미호
		//				case HORSEUNIQUE_THOUSAND ://천리마
		//				case HORSEUNIQUE_ZEBRA ://얼룩마
		//					{
		//						TotalMinDamage = TotalMinDamage + TotalMinDamage * 5 / 100 ;
		//						TotalMaxDamage = TotalMaxDamage + TotalMaxDamage * 5 / 100 ;
		//					}
		//				}

		//			}
		//		}
		//	}		
		//}		
		// 버프에 의한 공격력 상승
		if( gamemode == GAMEMODE_SERVER )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
				SI16 percent = 0 ;
				SI16 add = 0;


				if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
					percent += 20;
				
				if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
					percent += 10;

				if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
					percent += 10;

				if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
					percent += 20;


				if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}
			
				if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
				{
					percent += 10;
				}

				if( clBuf.IsHaveBuf(BUF_THANKSGIVING_DAY) )
				{
					percent += 10;
				}

				if( clBuf.IsHaveBuf(BUF_MENTOR) )
					percent += 10;

				// 크리스마스트리에 의한 공격력 증가
				//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. S]
				if( pclchar->pclCI->clBI.siHomeVillage > 0 )
				{
					cltServer* pclserver = (cltServer*)pclClient;
					cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );
					if (pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))	   == TRUE ||
						pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))	   == TRUE		)
						
						percent += 10;
				}
				siBufDamage = percent;
				TotalMinDamage += TotalMinDamage * percent / 100;
				TotalMinDamage += add;
				TotalMaxDamage += TotalMaxDamage * percent / 100;
				TotalMaxDamage += add;
			}
		}
		else if( gamemode == GAMEMODE_CLIENT )
		{
			if ( (pclchar) && ( 1 == pclchar->GetID()) )
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );
				SI16 percent = 0 ;
				SI16 add = 0;

				if( clBuf.IsHaveBuf(BUF_FRIENDSHIP) )
				{
					percent += 20;
				}

				if( clBuf.IsHaveBuf(BUF_HINAMOCHI) )
				{
					percent += 20;
				}

				if( clBuf.IsHaveBuf(BUF_ATTACKUP) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_ATTACKUP30) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_ATTACKUP30);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) )
				{
					SI32 siBufIndex = clBuf.FindBufSlot(BUF_EVENT_EGGMOONCAKE);
					if( siBufIndex >= 0 && siBufIndex < MAX_BUF_COUNT )
						percent = max( percent , clBuf.m_stBuf[siBufIndex].siPercent );
				}

				if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
				{
					percent += 10;
				}


				if( clBuf.IsHaveBuf(BUF_HAPPYBEAN) )
				{
					percent += 10;
				}

				if( clBuf.IsHaveBuf(BUF_MENTOR) )
					percent += 10;

				//[추가 : 황진성 2007. 12. 11 마을에 신년트리가 만들어지만 마을 주민에게 버프 적용. C]
				cltCharClient* pclcharclient = (cltCharClient*)pclchar;
				for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
					if( pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))	 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))		 ||		
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))		 ||
						pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))	)				
					{
						percent += 10;
					}
				}

				siBufDamage = percent;
				TotalMinDamage += TotalMinDamage * percent / 100;
				TotalMinDamage += add;
				TotalMaxDamage += TotalMaxDamage * percent / 100;
				TotalMaxDamage += add;
			}
		}

		// 명칭에 의한 공격력 상승
		if ( gamemode == GAMEMODE_SERVER)
		{
			if ( pclchar && IsPC( pclchar->GetID() ) )
			{
				if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
				{
					cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
					if ( pclunit )
					{
						TotalMinDamage += TotalMinDamage * pclunit->GetAttack() / 100;
						TotalMaxDamage += TotalMaxDamage * pclunit->GetAttack() / 100;

						siNamingDamage = pclunit->GetAttack();
					}

				}
				else
				{
					siNamingDamage = 0;
				}
			}
		}
		else if ( gamemode == GAMEMODE_CLIENT )
		{
			if ( pclchar && IsPC( pclchar->GetID() ) == 1 )
			{
				if ( pclchar->pclCI->clPersonNaming.m_siSelectedLevel > 0 )
				{
					cltNamingQuestUnit* pclunit = pclClient->pclNamingQuestManager->GetQuestInfo( pclchar->pclCI->clPersonNaming.m_siSelectedIndex, pclchar->pclCI->clPersonNaming.m_siSelectedLevel );
					if ( pclunit )
					{
						TotalMinDamage += TotalMinDamage * pclunit->GetAttack() / 100;
						TotalMaxDamage += TotalMaxDamage * pclunit->GetAttack() / 100;
						
						siNamingDamage = pclunit->GetAttack();
					}

				}
				else
				{
					siNamingDamage = 0;
				}
			}
		}

		// 쿠키에 의한 공격력 상승 
		if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
		{
			if(gamemode == GAMEMODE_SERVER)
			{
				if ( pclchar && IsPC( pclchar->GetID() ) )
				{
					if(pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN  )
						{
							if( pclchar->pclFamily->GetOnParentNum() > 0 && pclchar->pclFamily->GetOnChildrenNum() > 0 )
							{
								cltServer* pclserver = (cltServer*)pclClient;
								SI32 siPersonID = pclchar->pclCI->GetPersonID()	;
								if ( siPersonID > 0 ) 
								{
									SI32 siEndDateVary =	0	;
									SI32 siServerHourVary =	0	;
									siEndDateVary = pclchar->pclFamily->GetMyCookie_EndTime(siPersonID )	;
									siServerHourVary = pclserver->pclTime->clDate.GetHourVary()	;
									if(  siEndDateVary > 0 &&  siServerHourVary > 0 )
									{
										if ( siEndDateVary >siServerHourVary) 
										{
											SI32 percent	= 0;
											SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( siPersonID )	;// 레벨에 따라서 증가
											percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;

											TotalMinDamage += TotalMinDamage * percent / 100;
											TotalMaxDamage += TotalMaxDamage * percent / 100;
										}
									}
								}
							}				
						}						
					}				
				}
			}
			else if(gamemode == GAMEMODE_CLIENT)
			{
				if ( pclchar && pclchar->GetID() == 1)
				{
					if(pclchar->pclFamily != NULL )
					{
						if(pclchar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )
						{
							if(pclchar->pclFamily->GetOnParentNum() > 0 && pclchar->pclFamily->GetOnChildrenNum() > 0)
							{
								SI32  siPersonID = pclchar->pclCI->GetPersonID();
								if ( siPersonID > 0 ) 
								{
									SI32 siEndHourvary = pclchar->pclFamily->GetMyCookie_EndTime( siPersonID )	;
									SI32 siClientHourvary = ((cltClient*)pclClient)->clClientDate.GetHourVary()	;
									if(    siEndHourvary > 0 &&    siClientHourvary > 0 )
									{
										if( siEndHourvary > siClientHourvary  )
										{
											SI32 percent	= 0;
											SI32 siCookieLevel = pclchar->pclFamily->GetMyCookieLevel( pclchar->pclCI->GetPersonID() )	;// 레벨에 따라서 증가
											percent = pclchar->pclFamily->GetCookieEffect( siCookieLevel )	;
											TotalMinDamage += TotalMinDamage * percent / 100;
											TotalMaxDamage += TotalMaxDamage * percent / 100;
										}
									}
								}							
							}
						}
					}				
				}		
			}


		}
		
		//--------------------------------------------------
		// 이동 속도를 구한다. 
		//--------------------------------------------------
		BasicMoveSpeed		= pclKI->siMoveSpeed;
		
		if(pclitemlist)
		{
			if (pclClient->IsCountrySwitch(Switch_StatusBug)) {
				ShoesMoveSpeed		= pclItemManager->GetMoveSpeed(pclitemlist);
			}
			else{
				ShoesMoveSpeed		= pclItemManager->GetMoveSpeed(pclitemlist,pclchar);
			}
			
		}
		else
		{
			ShoesMoveSpeed = 0;
		}

		if(GivenMoveSpeed)
		{
			TotalMoveSpeed		= GivenMoveSpeed;
		}
		else
		{
			// 말을 타고 있다면, 
			if(pclhorseinfo && pclhorseinfo->siCurrentHorseRIDEHORSE == true)
			{
				cltHorse* pclhorse = pclhorseinfo->GetCurrentShowHorse();
				if (pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN) ||			//해피빈 이벤트 해피타임을 먹으면 이속최대 [2007.08.27 손성웅]
							pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY)) )	//[추가 : 황진성 2007. 11. 12 추수 감사절 이벤트. 이동속도 최대]
				{ 
					TotalMoveSpeed = 20;
				}
				else if(pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MENTOR)))
				{
					if( 15 > BasicMoveSpeed + ShoesMoveSpeed && 15 > pclhorse->GetRealMoveSpeed())
					{
						TotalMoveSpeed = 15;
					}
					else if(pclhorse)
						TotalMoveSpeed	= pclhorse->GetRealMoveSpeed();		// 말의 이동속도로 속도를 대체한다.
				}
				else
				{
					if(pclhorse)
						TotalMoveSpeed	= pclhorse->GetRealMoveSpeed();		// 말의 이동속도로 속도를 대체한다. 

				}
			}
				
			// 말을 타지 않은 상태라면, 
			else 
			{
				if (pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN) ||			//해피빈 이벤트 해피타임을 먹으면 이속최대 [2007.08.27 손성웅]
								pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY)) )	//[추가 : 황진성 2007. 11. 12 추수 감사절 이벤트. 이동속도 최대] 
				{ 
					TotalMoveSpeed = 20;
				}
				else if(pclchar && (pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MENTOR)))
				{
					if( 15 > BasicMoveSpeed + ShoesMoveSpeed )
					{
						TotalMoveSpeed = 15;
					}
				}
				else
				{// 버프도 없고 말도 타지 않은 상태라면
					TotalMoveSpeed		= BasicMoveSpeed + ShoesMoveSpeed;
				}			
			}
		}


		//--------------------------------------------------
		// 공격 가능성과 방어 가능성을 구한다. 
		//--------------------------------------------------
		AR					= ( clTotalBA.GetDex() * 10 ) / 10;		// 민첩성의 100% 적용. 
		DR					= (AR / 3) + (TotalAC / 2);

        
		// 전투향상술에 의한 명중율, 회피율, 방어력을 결정한다.
		if ( true == bCalcWeaponSkill )
		{
			// 회피율
			SI32 siSkillUnique		= 0;
			SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
			switch ( siItemType )
			{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_INCDR;			}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_SWORD_INCDR;	}	break;
				case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_SWORD_INCDR;		}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_SWORD_INCDR;		}	break;
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_SWORD_INCDR;		}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_SWORD_INCDR;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					DR = DR + (DR * siWeaponSkillRate / 100);
				}
			}

			// 방어율
			siSkillUnique = 0;
			switch ( siItemType )
			{
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_INCAC;	}	break;
			}

			if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
			{
				SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
				SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
				if ( 0 < siWeaponSkillRate )
				{
					TotalAC = TotalAC + (TotalAC * siWeaponSkillRate / 100);
				}
			}

		}


		//	if ( gamemode == GAMEMODE_SERVER )
		//	{
		/*
		if(bnpc == false && personid)
		{
			SI32 itemtype = 0 ;
			if ( pclitemlist )
				itemtype = pclItemManager->GetItemType(pclitemlist[activeweaponpos].siUnique) ;
			SI32 skilllevel = 0 ;

			switch ( itemtype )
			{
			case ITEMTYPE_SWORD:
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_SWORD_INCDR, 0, 0, pclitemlist);
					if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
					{
						// SJY DEBUG
						SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW1,skilllevel) ;

						DR = DR + DR * rate / 100 ;
					}
				}
				break;
			case ITEMTYPE_SPEAR:
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_SPEAR_INCAC, 0, 0, pclitemlist);
					if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
					{
						// SJY DEBUG
						SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW1,skilllevel) ;
						TotalAC = TotalAC + TotalAC * rate / 100 ;
					}
				}
				break;
		
			case ITEMTYPE_BOW:
				{
					skilllevel = pclskillinfo->GetTotalSkillLevel(SKILL_BOW_INCAR, 0, 0, pclitemlist);
					if ( skilllevel > 0 && skilllevel <= MAX_WEAPONSKILL_LEVEL )
					{
						// SJY DEBUG
						SI32 rate = pclClient->pclSkillManager->GetWeaponSkillRate(MAX_WEAPONSKILL_POW1,skilllevel) ;
						AR = AR + AR * rate / 100 ;
					}
				}
				break;
		
			}
		}
		*/
		//	}

		// 이동속도, 사정거리를 계산한다.
		{
			if(bnpc == false && personid)
			{
				// 전투향상술에 의한 공격범위 증가
				if ( true == bCalcWeaponSkill )
				{
					SI32 siSkillUnique		= 0;
					SI32 siItemType			= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
					switch ( siItemType )
					{
						case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_BOW_INCRANGE;	}	break;
						case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_BOW_INCRANGE;	}	break;
						case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_BOW_INCRANGE;		}	break;
						case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_INCRANGE;			}	break;
						case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_BOW_INCRANGE;		}	break;
						case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_BOW_INCRANGE;	}	break;
					}

					if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
					{
						SI32 siSkillLevel		= pclskillinfo->GetTotalSkillLevel( siSkillUnique, 0, 0 );
						SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
						if ( 0 < siWeaponSkillRate )
						{
							siAttackRange += (siAttackRange * siWeaponSkillRate / 100);
						}
					}
				}

				//KHY - 1220 - 변신시스템.
				if((pclchar) && (pclchar->clTransFormInfo.bTransFormMode == true))
				{
					if(siTransFormSyc >= 100) // 싱크로 100% 보너스.
					{
						switch(siAttackType)
						{ 
							case ATTACKTYPE_BOW:
							case ATTACKTYPE_GUN:
								siAttackRange += 1;
								break;
						}
					}
				}
			}
		}
	
		//-------------------------------------------------
		// 지휘효과를 클라이언트에게 통보한다. 
		//-------------------------------------------------
		if(bacswitch || bdamageswitch)
		{
			// 클라이언트에게 알린다. 
			if(pclchar)
			{

				cltGameMsgResponse_GeneralEft clinfo(pclchar->GetCharUnique(), siGeneralAC   , siGeneralDamage );
				cltMsg clMsg(GAMEMSG_RESPONSE_GENERALEFT, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclchar)->SendNeighbourMsg(&clMsg, true);
			}

		}
	} //일단 다 룟았다.
	return TRUE;
}

// 방어률을 리턴한다.
SI32 cltParametaBox::GetDefensiveRate(cltCharCommon* Enemypclchar,bool MagicFlag)	
{
	SI32 Enemylevel = Enemypclchar->clPB.pclIP->GetLevel();
	SI32 Mylevel = pclIP->GetLevel();

	if( true == MagicFlag )
	{
		//--------------------------------------
		// 106.5  최종 마법 방어율 설정.  -%  
		//--------------------------------------
		//siTotalMagicalDefensiveRate = (siMagicalDefensive*10000)/(siMagicalDefensive+200+(Enemylevel*10)) + (Mylevel*10);
		siTotalMagicalDefensiveRate = (((siMagicalDefensive*10000)/(siMagicalDefensive+700))*(10000+((Mylevel-Enemylevel)*200)))/10000;

		siTotalMagicalDefensiveRate = max(1000,siTotalMagicalDefensiveRate);
		siTotalMagicalDefensiveRate = min(9000,siTotalMagicalDefensiveRate);

		return siTotalMagicalDefensiveRate;
	}
	else
	{
		//--------------------------------------
		// 105.5  최종 물리 방어율 설정.  -%  
		//--------------------------------------
		//siTotalPHysicalDefensiveRate = (siPHysicalDefensive*10000)/(siPHysicalDefensive+200+(Enemylevel*10)) + (Mylevel*10);
		siTotalPHysicalDefensiveRate = (((siPHysicalDefensive*10000)/(siPHysicalDefensive+700))*(10000+((Mylevel-Enemylevel)*200)))/10000;

		siTotalPHysicalDefensiveRate = max(1000,siTotalPHysicalDefensiveRate);
		siTotalPHysicalDefensiveRate = min(9000,siTotalPHysicalDefensiveRate);

		return siTotalPHysicalDefensiveRate;
	}
	//return 0;
}

// 방어률을 리턴한다.
SI32 cltParametaBox::GetClientDefensiveRate(bool MagicFlag)	
{
	SI32 Mylevel = pclIP->GetLevel();
	SI32 Enemylevel = Mylevel;
	SI32 pDefensive = 0;

	if( true == MagicFlag )
	{
		//--------------------------------------
		// 106.5  최종 마법 방어율 설정.  -%  
		//--------------------------------------
		//pDefensive = (siMagicalDefensive*10000)/(siMagicalDefensive+200+(Enemylevel*10)) + (Mylevel*10);
		pDefensive = (((siMagicalDefensive*10000)/(siMagicalDefensive+700))*(10000+((Mylevel-Enemylevel)*200)))/10000;

		pDefensive = max(1000,pDefensive);
		pDefensive = min(9000,pDefensive);

		return pDefensive;
	}
	else
	{
		//--------------------------------------
		// 105.5  최종 마법 방어율 설정.  -%  
		//--------------------------------------
		//pDefensive = (siPHysicalDefensive*10000)/(siPHysicalDefensive+200+(Enemylevel*10)) + (Mylevel*10);
		pDefensive = (((siPHysicalDefensive*10000)/(siPHysicalDefensive+700))*(10000+((Mylevel-Enemylevel)*200)))/10000;

		pDefensive = max(1000,pDefensive);
		pDefensive = min(9000,pDefensive);

		return pDefensive;
	}
	//return 0;
}

// 상대방의 DR과 나의 AR을 근거로 명중률을 구한다.  - 최종 히트 며부 확률을 구하는 곳이다.
SI32 cltParametaBox::GetHitRate(cltCharCommon* Enemypclchar, cltCharCommon* pclAttackChar, bool MagicFlag )			
{
	SI16 enemydr = Enemypclchar->clPB.GetDR();

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SI32 Enemylevel = Enemypclchar->clPB.pclIP->GetLevel();
		SI32 Mylevel = pclIP->GetLevel();
		SI32 toTalHitRate = 0;

		if( true == MagicFlag )
		{
			//--------------------------------------
			// 104.4 마법 명중 레벨보정 설정. 
			//--------------------------------------
			siMagicalHitRateLevel = (Mylevel - Enemylevel) * 50;

			//--------------------------------------
			// 104.5 마법 명중률 결정. 
			//--------------------------------------
			toTalHitRate = min(10000, ((7000+(siMagicalHitRateRank*6)) * (10000 +siMagicalHitRateLevel))/10000);

			if(toTalHitRate >0)
				siTotalMagicalHitRate = toTalHitRate; //크리스탈에 의한 명중률 향상 수치를 더한다.

			if( Enemypclchar )
			{
				if( Enemypclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_DODGE, Enemypclchar->GetID() ) )
				{
					siTotalMagicalHitRate -= (siTotalMagicalHitRate * Enemypclchar->m_clCharCurseEffect.GetPara()) / 100; 
				}
			}

			return siTotalMagicalHitRate;
		}
		else
		{
			//--------------------------------------
			// 103.4 물리 명중 레벨보정 설정. 
			//--------------------------------------
			siPHysicalHitRateLevel = (Mylevel - Enemylevel) * 50;


			//--------------------------------------
			// 103.5 물리 명중률 결정. 
			//--------------------------------------
			toTalHitRate = min(10000, ((7000+(siPHysicalHitRateRank*6)) * (10000 +siPHysicalHitRateLevel))/10000);

			if(toTalHitRate>0)
				siTotalPHysicalHitRate = toTalHitRate; //크리스탈에 의한 명중률 향상 수치를 더한다.

			if( Enemypclchar )
			{
				if( Enemypclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_DODGE, Enemypclchar->GetID() ) )
				{
					siTotalPHysicalHitRate -= (siTotalPHysicalHitRate * Enemypclchar->m_clCharCurseEffect.GetPara()) / 100; 
				}
			}

			return siTotalPHysicalHitRate;

			//siTotalPHysicalHitRate = toTalHitRate + (toTalHitRate * siEquipPHysicalHitRate /100); //크리스탈에 의한 명중률 향상 수치를 더한다.
		}
	}
	else
	{
		HitRate = 0;
		if( true == MagicFlag )
		{
			HitRate	= (AR * 120) / (AR + enemydr) + (siEquipAssistHitRate/10) + 30;	// 장비에 의한 명중률 향상 수치를 더한다. 마법은 좀 더 더해준다.
		}
		else
		{
			HitRate	= (AR * 120) / (AR + enemydr) + (siEquipAssistHitRate/10);	// 장비에 의한 명중률 향상 수치를 더한다. 

			// 명중률이 60% 미만이면, 디폴트 명중률을 더해준다. 
			if(HitRate < 60)
			{
				HitRate = min(60, HitRate + siWeaponDefaultHitRate);
			}
		}

		HitRate = min(100, HitRate);

		return HitRate;
	}
	//return 0;
}

// 상대방의 DR과 나의 AR을 근거로 명중률을 구한다.  - 최종 히트 며부 확률을 구하는 곳이다.
SI32 cltParametaBox::GetClientHitRate(bool MagicFlag)			
{
	SI32 Mylevel = pclIP->GetLevel();

	//적에 대한 데이터는 '나'와 같다고 상정한다.
	SI32 Enemylevel = Mylevel;   

	SI32 toTalHitRate = 0;

	if( true == MagicFlag )
	{
		//--------------------------------------
		// 104.4 마법 명중 레벨보정 설정. 
		//--------------------------------------
		siMagicalHitRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 104.5 마법 명중률 결정. 
		//--------------------------------------
		toTalHitRate = min(10000, ((7500+(siMagicalHitRateRank*6)) * (10000 +siMagicalHitRateLevel))/10000);

		if(toTalHitRate >0)
			siTotalMagicalHitRate = toTalHitRate; //크리스탈에 의한 명중률 향상 수치를 더한다.

		return siTotalMagicalHitRate;
	}
	else
	{
		//--------------------------------------
		// 103.4 물리 명중 레벨보정 설정. 
		//--------------------------------------
		siPHysicalHitRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 103.5 물리 명중률 결정. 
		//--------------------------------------
		toTalHitRate = min(10000, ((7000+(siPHysicalHitRateRank*6)) * (10000 +siPHysicalHitRateLevel))/10000);

		if(toTalHitRate>0)
			siTotalPHysicalHitRate = toTalHitRate; //크리스탈에 의한 명중률 향상 수치를 더한다.

		return siTotalPHysicalHitRate;
	}

	//return 0;
}

// 회피율을 구한다.
SI32 cltParametaBox::GetDodgeRate(cltCharCommon* Enemypclchar, cltCharCommon* pclAttackChar, bool MagicFlag)			
{
	SI32 Enemylevel = Enemypclchar->clPB.pclIP->GetLevel();
	SI32 Mylevel = pclIP->GetLevel();
	SI32 toTalDodgeRate = 0;

	if( true == MagicFlag )
	{
		//--------------------------------------
		// 108.4 마법 회피 레벨보정 설정. 
		//--------------------------------------
		siMagicalDodgeRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 108.5 마법 회피률 결정. 
		//--------------------------------------
		toTalDodgeRate = min(5000, (siMagicalDodgeRateRank*7*(10000+siMagicalDodgeRateLevel))/10000);

		if(toTalDodgeRate >0)
			siTotalMagicalDodgeRate = toTalDodgeRate; 

		if( Enemypclchar )
		{
			if( Enemypclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_HIT, Enemypclchar->GetID() ) )
			{
				siTotalMagicalDodgeRate += (siTotalMagicalDodgeRate * Enemypclchar->m_clCharCurseEffect.GetPara()) / 100; 
			}
		}

		return siTotalMagicalDodgeRate;
	}
	else
	{
		//--------------------------------------
		// 107.4 물리 회피 레벨보정 설정. 
		//--------------------------------------	
		siPHysicalDodgeRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 107.5 물리 회피률 결정. 
		//--------------------------------------
		toTalDodgeRate = min(5000, (siPHysicalDodgeRateRank*6*(10000+siPHysicalDodgeRateLevel))/10000);

		if(toTalDodgeRate >0)
			siTotalPHysicalDodgeRate = toTalDodgeRate; 

		if( Enemypclchar )
		{
			if( Enemypclchar->m_clCharCurseEffect.IsCurse( cltCharCurseEffect::CURSE_HIT, Enemypclchar->GetID() ) )
			{
				siTotalPHysicalDodgeRate += (siTotalPHysicalDodgeRate * Enemypclchar->m_clCharCurseEffect.GetPara()) / 100; 
			}
		}

		return siTotalPHysicalDodgeRate;
	}

//	return 0 ;
}

// 회피율을 구한다.
SI32 cltParametaBox::GetClientDodgeRate(bool MagicFlag)			
{
	SI32 Mylevel = pclIP->GetLevel();
	SI32 Enemylevel = Mylevel;

	SI32 toTalDodgeRate = 0;

	if( true == MagicFlag )
	{
		//--------------------------------------
		// 108.4 마법 회피 레벨보정 설정. 
		//--------------------------------------
		siMagicalDodgeRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 108.5 마법 회피률 결정. 
		//--------------------------------------
		toTalDodgeRate = min(5000, (siMagicalDodgeRateRank*7*(10000+siMagicalDodgeRateLevel))/10000);

		if(siEquipMagicalDodgeRate >0)
			toTalDodgeRate = toTalDodgeRate ; 

		return toTalDodgeRate;
	}
	else
	{
		//--------------------------------------
		// 107.4 물리 회피 레벨보정 설정. 
		//--------------------------------------	
		siPHysicalDodgeRateLevel = (Mylevel - Enemylevel) * 50;

		//--------------------------------------
		// 107.5 물리 회피률 결정. 
		//--------------------------------------
		toTalDodgeRate = min(5000, (siPHysicalDodgeRateRank*6*(10000+siPHysicalDodgeRateLevel))/10000);

		if(siEquipPHysicalDodgeRate >0)
			toTalDodgeRate = toTalDodgeRate; 

		return toTalDodgeRate;
	}

//	return 0 ;
}


//------------------------
// 체력
//------------------------
SI32 cltParametaBox::GetMaxLife()
{
	SI32 life = 0;
	
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		life = GetTotalHitpoint();
	}
	else
	{
		life = MaxLife;
	}

	return life;
}

//------------------------
// 마나
//------------------------
SI32 cltParametaBox::GetMaxMana()
{
	SI32 mana = 0;
	
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		mana = GetTotalManapoint();
	}
	else
	{
		mana = MaxMana;
	}

	return mana;
}

//------------------------
// 무게 
//------------------------
SI32 cltParametaBox::GetMaxWeight()
{
	SI32 weight = 0;
	
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		weight = GetTotalWeight();
	}
	else
	{
		weight = pclIP->GetLevel() * 1000;
	}

	return weight;
}



// 레벨에 의한 방어력 상승 효과를 구한다. 
SI32 cltParametaBox::CalcLevelAC(SI32 level)
{
	SI32 ac = 0;

	if(level > 90)	{	ac += ((level - 90) / 1);		level = 90;		}
	if(level > 70)	{	ac += ((level - 70) / 2);		level = 70;		}
	if(level > 60)	{	ac += ((level - 60) / 3);		level = 60;		}
	if(level > 50)	{	ac += ((level - 50) / 4);		level = 50;		}
	if(level > 40)	{	ac += ((level - 40) / 5);		level = 40;		}
	ac	+= (level / 10);
	ac /= 2;

	return ac;
}


//----------------------------------------------------------
// 능력치 계산 함수
//----------------------------------------------------------
void cltParametaBox::SetEquipBasicAbility(cltCharCommon* pclchar, cltItem* pclitemlist, SI32 activeweaponpos)
{
	clEquipBA.Init();

	if(pclitemlist == NULL)			return;

	if(pclClient->IsCountrySwitch(Switch_StatusBug))
	{
		SI32 siStr = pclItemManager->GetEquipStr(pclitemlist, activeweaponpos);
		SI32 siDex = pclItemManager->GetEquipDex(pclitemlist, activeweaponpos);
		SI32 siVit = pclItemManager->GetEquipVit(pclitemlist, activeweaponpos);
		SI32 siMag = pclItemManager->GetEquipMag(pclitemlist, activeweaponpos);
		SI32 siLuk = pclItemManager->GetEquipLuk(pclitemlist, activeweaponpos);
		SI32 siHnd = pclItemManager->GetEquipHnd(pclitemlist, activeweaponpos);	
		//SI32 siWis = pclItemManager->GetEquipWis(pclitemlist, activeweaponpos);

		clEquipBA.SetStr(siStr);
		clEquipBA.SetDex(siDex);
		clEquipBA.SetVit(siVit);
		clEquipBA.SetMag(siMag);
		clEquipBA.SetLuk(siLuk);
		clEquipBA.SetHnd(siHnd);
	}
	else
	{
		SI32 siStr = pclItemManager->GetEquipStr(pclitemlist, activeweaponpos, pclchar);
		SI32 siDex = pclItemManager->GetEquipDex(pclitemlist, activeweaponpos, pclchar);
		SI32 siVit = pclItemManager->GetEquipVit(pclitemlist, activeweaponpos, pclchar);
		SI32 siMag = pclItemManager->GetEquipMag(pclitemlist, activeweaponpos, pclchar);
		SI32 siLuk = pclItemManager->GetEquipLuk(pclitemlist, activeweaponpos, pclchar);
		SI32 siHnd = pclItemManager->GetEquipHnd(pclitemlist, activeweaponpos, pclchar);	
		SI32 siWis = pclItemManager->GetEquipWis(pclitemlist, activeweaponpos, pclchar);

		clEquipBA.SetStr(siStr);
		clEquipBA.SetDex(siDex);
		clEquipBA.SetVit(siVit);
		clEquipBA.SetMag(siMag);
		clEquipBA.SetLuk(siLuk);
		clEquipBA.SetHnd(siHnd);
		clEquipBA.SetWis(siWis);
	}
}

void cltParametaBox::SetTransformBasicAbility(cltCharCommon* pclchar, cltItem* pclitemlist, SI32 activeweaponpos)
{
	if(pclchar == NULL)				return;

	cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[pclchar->clTransFormInfo.uiNowTransFormKind];
	if(pclki == NULL)			return;

	SI32 itemtype = 0 ;
	if ( pclitemlist )	itemtype = pclItemManager->GetItemType(pclitemlist[activeweaponpos].siUnique) ;

	clTransFormBA.Init();
	if(pclchar->clTransFormInfo.bTransFormMode == true)
	{
		// 해당 변신 캐릭터의 기본 능력 설정.
		clTransFormBA.Set(&pclki->clBA);

		//싱크로율에 따라 추가 효과를 더한다.
		SI32 temState = 0;
		siTransFormSyc = (SI32)pclchar->clTransFormInfo.GetSynchroTransForm(pclchar->clTransFormInfo.uiNowTransFormKind);

		// 민첩 증가.
		temState = clTransFormBA.GetDex() * (siTransFormSyc/2) / 100;
		clTransFormBA.IncreaseDex(temState);

		// 힘 증가.
		temState = clTransFormBA.GetStr() * (siTransFormSyc/2) / 100;
		clTransFormBA.IncreaseStr(temState);

		// 체력 증가.
		temState = clTransFormBA.GetVit() * (siTransFormSyc/2) / 100;
		clTransFormBA.IncreaseVit(temState);

		// 지혜 증가.
		temState = clTransFormBA.GetWis() * (siTransFormSyc/2) / 100;
		clTransFormBA.IncreaseWis(temState);

		// 지력 증가.
		temState = clTransFormBA.GetMag() * (siTransFormSyc/2) / 100;

		// 지팡이  = 지력 싱크로 %가 더 해진다. 
		if(itemtype == ITEMTYPE_STAFF)
			temState += (SI32)(siTransFormSyc/5);

		clTransFormBA.IncreaseMag(temState);

		//KHY - 0124 - 가챠레어- 닌자 변신 아이템.
		if((pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAMAN"))			||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAGIRL"))			||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATMELEE"))			||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATRANGE"))			||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN"))	||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST"))	||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE"))	||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE"))	||
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE"))	||	// 검사스피릿(여)
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE"))  ||	// 거너스피릿(남)
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYMELEE"))		||	// 요정스피릿남)
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYRANGE"))		||	// 요정스피릿(여)
			// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICERANGE"))			||	// 눈의 여왕
			(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICEMELEE"))				// 사스콰치
		   )		
		{
			// 방어력 증가.
			siTransFormAC = 80; // 기본 방어력.
			temState = siTransFormAC * (siTransFormSyc/2) / 100;

			siTransFormAC = siTransFormAC + temState; // 싱크로율 증가분.				
		}

		if(siTransFormSyc >= 100) // 싱크로율 100% 보너스. - 10 고정.
		{
			if( (pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAMAN"))  		||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_NINJAGIRL")) 		||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATMELEE"))  		||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CATRANGE"))			||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENPUMPKIN"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_HALLOWEENGHOST"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASMELEE"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_CHRISTMASRANGE"))	||
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENMELEE"))	||	// 검사스피릿(여)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FIGHTINGMENRANGE"))	||  // 거너스피릿(남)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYMELEE"))		||	// 요정스피릿(남)
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_FAIRYRANGE"))		||	// 요정스피릿(여)
				// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICERANGE"))			||	// 눈의 여왕
				(pclchar->clTransFormInfo.uiNowTransFormKind == pclClient->GetUniqueFromHash("KIND_ICEMELEE"))				// 사스콰치
			  )		
			{
				clTransFormBA.IncreaseHnd(15);
				clTransFormBA.IncreaseLuk(15);
			}
			else
			{
				clTransFormBA.IncreaseHnd(10);
				clTransFormBA.IncreaseLuk(10);
			}
		}
	}
}

void cltParametaBox::SetHorseBasicAbility(cltPersonHorseInfo* pclhorseinfo, cltCharCommon* pclchar)
{
	clHorseBA.Init();

	if(pclhorseinfo == NULL)							return;
	if(pclhorseinfo->siCurrentHorseRIDEHORSE == false)	return;

	cltHorse* pclhorse = pclhorseinfo->GetCurrentShowHorse();

	if(pclhorse)
	{
		clHorseBA.siStr	= pclhorse->siStr / 100;
		clHorseBA.siDex	= pclhorse->siDex / 100;
		clHorseBA.siMag	= pclhorse->siMag / 100;

		SI32 horsedate = pclClient->pclHorseManager->GetAddStatusLimitDateVary(pclhorse) ;
		SI32 Premiumhorsedate_Slot1 = pclClient->pclHorseManager->GetPremiumPArtsDateVary_Slot1(pclhorse) ;
		SI32 Premiumhorsedate_Slot2 = pclClient->pclHorseManager->GetPremiumPArtsDateVary_Slot2(pclhorse) ;
		SI32 Premiumhorsedate_Slot3 = pclClient->pclHorseManager->GetPremiumPArtsDateVary_Slot3(pclhorse) ;
		SI32 siEquipPremiumPartsNum	=	 pclhorse->GetPremiumPartsNum();
		if ( pclClient->GameMode == GAMEMODE_SERVER )
		{
			if ( pclhorse->siStatusTicketNum > 0 && horsedate > 0 && 
				((cltServer*)pclClient)->pclTime->clDate.GetDateVary() < horsedate )
			{
				clHorseBA.siStr	+= MAX_HORSE_ADDSTATUS ;
				clHorseBA.siDex	+= MAX_HORSE_ADDSTATUS ;
				clHorseBA.siMag	+= MAX_HORSE_ADDSTATUS ;				
			}
			// 프리미엄 말파츠로 인한 추가 능력치			
			// 슬롯 1 
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot1 > 0 &&
				((cltServer*)pclClient)->pclTime->clDate.GetDateVary() < Premiumhorsedate_Slot1)
			{
				SI32 siPremiumUnique	= pclhorse->GetPremiumUnique_Slot1();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}				
			}
			// 슬롯 2
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot2 > 0 &&
				((cltServer*)pclClient)->pclTime->clDate.GetDateVary() < Premiumhorsedate_Slot2)
			{
				SI32 siPremiumUnique = pclhorse->GetPremiumUnique_Slot2();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}
			}
			// 슬롯 3 
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot3 > 0 &&
				((cltServer*)pclClient)->pclTime->clDate.GetDateVary() < Premiumhorsedate_Slot3)
			{
				SI32 siPremiumUnique	= pclhorse->GetPremiumUnique_Slot3();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}
			}

			// [춘기] 크리스마스 이벤트 - 크리스마스 장식 버프가 있으면 탈것의 능력치 + 5를 해준다.
			if(pclchar && IsPC( pclchar->GetCharUnique() ))
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );

				if(clBuf.IsHaveBuf(BUF_CHRISTMASEVENT_DECORATION))
				{
					clHorseBA.siStr += 5;
					clHorseBA.siDex += 5;
					clHorseBA.siVit += 5;
					clHorseBA.siWis += 5;
					clHorseBA.siMag += 5;
					clHorseBA.siLuk += 5;
					clHorseBA.siHnd += 5;
				}
			}
		}
		else if (pclClient->GameMode == GAMEMODE_CLIENT )
		{
			if ( pclhorse->siStatusTicketNum > 0 && horsedate > 0 && 
				((cltClient*)pclClient)->clClientDate.GetDateVary() < horsedate )
			{
				clHorseBA.siStr	+= MAX_HORSE_ADDSTATUS ;
				clHorseBA.siDex	+= MAX_HORSE_ADDSTATUS ;
				clHorseBA.siMag	+= MAX_HORSE_ADDSTATUS ;
			}
			// 프리미엄 말파츠로 인한 추가 능력치
			// 슬롯 1 
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot1 > 0 )
			{
				SI32 siPremiumUnique = pclhorse->GetPremiumUnique_Slot1();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}				
			}
			// 슬롯 2
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot2 > 0 )
			{
				SI32 siPremiumUnique = pclhorse->GetPremiumUnique_Slot2();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}				
			}
			// 슬롯 3 
			if ( siEquipPremiumPartsNum > 0 &&  Premiumhorsedate_Slot3 > 0 )
			{
				SI32 siPremiumUnique = pclhorse->GetPremiumUnique_Slot3();
				if ( siPremiumUnique > 0)
				{
					// 달린만큼 포문 돌고 능력치를 셋팅 한다.
					SetPremiumPartsstatusUp( siPremiumUnique );
				}				
			}

			// [춘기] 크리스마스 이벤트 - 크리스마스 장식 버프가 있으면 탈것의 능력치 + 5를 해준다.
			if(pclchar && IsPC( pclchar->GetCharUnique() ))
			{
				cltBufInfo clBuf;
				clBuf.SetBuf( &pclchar->pclCI->clBI.clBufInfo );

				if(clBuf.IsHaveBuf(BUF_CHRISTMASEVENT_DECORATION))
				{
					clHorseBA.siStr += 5;
					clHorseBA.siDex += 5;
					clHorseBA.siVit += 5;
					clHorseBA.siWis += 5;
					clHorseBA.siMag += 5;
					clHorseBA.siLuk += 5;
					clHorseBA.siHnd += 5;
				}
			}
		}
	}
}
void cltParametaBox::SetPremiumPartsstatusUp( SI32 PremiumItemUnique )
{	// 달린만큼 포문 돌고 능력치를 셋팅 한다.
	switch ( PremiumItemUnique )
	{
	case ITEMUNIQUE(13635):	// 헤드라이트
		{
			clHorseBA.siDex	+= 30 ;
		}
		break;
	case ITEMUNIQUE(13636):	// 불꽃휠
		{
			clHorseBA.siStr	+= 30 ;	// 근력 10
		}
		break;
	case ITEMUNIQUE(13637):	// LED안테나
		{
			clHorseBA.siMag	+= 30 ;;	// 지력 25
		}
		break;
	case ITEMUNIQUE(13638):	// 강화 등자
		{
			clHorseBA.siDex	+= 30 ;	// 순발력 10
		}
		break;
	case ITEMUNIQUE(13639):	// 강화 고삐
		{
			clHorseBA.siStr	+= 30;	// 근력 5
		}
		break;
	case ITEMUNIQUE(13640):	// 강화 고삐
		{
			clHorseBA.siMag	+= 30;	// 지력 15
		}
		break;
	}
}
void cltParametaBox::SetGuildBasicAbility(cltCharCommon* pclchar)
{
	if(pclchar == NULL)			return;

	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		clGuildBA.Init();

		if ( pclchar->pclCI->clBI.clGuildRank.IsGuildMember() )
		{
			if ( pclchar->pclCI->clBI.clGuildRank.IsGuildMember() == false ) return; // 길드 맴버가 아니면 스테이터스 상승을 시키지 않는다.
			if ( pclchar->pclCI->clBI.clGuildRank.GetRankType() == GUILDRANKTYPE_STANDING ) return; // 길드 맴버가 아니면 스테이터스 상승을 시키지 않는다.
			
			cltGuildInfo* pclGuild = pclClient->pclGuildManager->GetGuildInfo(pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex);
			if ( pclGuild != NULL )
			{
				clGuildBA.siStr = pclGuild->clStatus.siStr ;
				clGuildBA.siDex = pclGuild->clStatus.siDex ;
				clGuildBA.siMag = pclGuild->clStatus.siMag ;
				clGuildBA.siVit = pclGuild->clStatus.siVit ;
				clGuildBA.siHnd = pclGuild->clStatus.siHnd ;
				clGuildBA.siLuk = pclGuild->clStatus.siLuk ;
				clGuildBA.siWis= pclGuild->clStatus.siWis;
			}
		}
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		/*	
		if( pclchar->pclCI->clBI.clGuildRank.IsGuildMember() )
		{

			clGuildBA.siStr = siGuildStr;
			clGuildBA.siDex = siGuildDex;
			clGuildBA.siMag = siGuildMag;
			clGuildBA.siVit = siGuildVit;
			clGuildBA.siHnd = siGuildHnd;
			clGuildBA.siLuk = siGuildLuck;
			clGuildBA.siWis= siGuildWis;
		}
		*/

	}
}

void cltParametaBox::SetHealthAddAbility(cltHealthInfo*	pclhealthinfo, cltBasicAbility BasicBA)
{
	siHealthStrRate = 0;
	siHealthDexRate = 0;
	siHealthMagRate = 0;

	siHealthHndRate = 0;
	siHealthLukRate = 0;
	siHealthWisRate = 0;


	clHealthAdd.Init();

	if(pclhealthinfo == NULL)			return;
	
	siHealthStrRate	= pclhealthinfo->uiHealthPoint - 100;
	siHealthDexRate	= pclhealthinfo->uiHealthPoint - 100;
	siHealthMagRate	= pclhealthinfo->uiHealthPoint - 100;

	siHealthHndRate	= pclhealthinfo->uiHealthPoint - 100;
	siHealthLukRate	= pclhealthinfo->uiHealthPoint - 100;
	siHealthWisRate	= pclhealthinfo->uiHealthPoint - 100;


	clHealthAdd.SetStr( (BasicBA.GetStr() * siHealthStrRate) / 100 );
	clHealthAdd.SetDex( (BasicBA.GetDex() * siHealthDexRate) / 100 );
	clHealthAdd.SetMag( (BasicBA.GetMag() * siHealthMagRate) / 100 );

	clHealthAdd.SetHnd( (BasicBA.GetHnd() * siHealthHndRate) / 100 );
	clHealthAdd.SetLuk( (BasicBA.GetLuk() * siHealthLukRate) / 100 );
	clHealthAdd.SetWis( (BasicBA.GetWis() * siHealthWisRate) / 100 );

}

void cltParametaBox::SetElixirAddAbility( cltCharCommon* pclchar )
{
	if( false == pclClient->IsCountrySwitch( Switch_Elixir) ) return;
	
	if( pclchar && IsPC( pclchar->GetCharUnique() ) )
	{
		// [진성] 엘릭서 
		clElixirAdd.Init();

		if( pclchar->pcltElixir )
		{
			clElixirAdd.SetStr( pclchar->pcltElixir->GetStr() );
			clElixirAdd.SetDex( pclchar->pcltElixir->GetDex() );
			clElixirAdd.SetMag( pclchar->pcltElixir->GetMag() );
			clElixirAdd.SetVit( pclchar->pcltElixir->GetVit() );
			clElixirAdd.SetHnd( pclchar->pcltElixir->GetHnd() );
			clElixirAdd.SetWis( pclchar->pcltElixir->GetWis() );
		}
	}
}

void cltParametaBox::SetSetItemAddAbility(cltCharCommon* pclchar, cltItem* pclitemlist)
{
	clSetItemAdd.Init();

	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if( pclchar && IsPC( pclchar->GetCharUnique() ) )
		{
			if( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{			
				//-----------------------------------------------------------------------------
				SI16	 oldSetItemType =  pclchar->m_siSetItemType; // 바뀌었는지 비교하기 위해.
				SI16	 oldSetItemType2 =  pclchar->m_siSetItemType2; // 바뀌었는지 비교하기 위해.
				SI16	 oldSetItemType3 =  pclchar->m_siSetItemType3; // 바뀌었는지 비교하기 위해.
				SI16	 oldSetItemType4 =  pclchar->m_siSetItemType4; // 바뀌었는지 비교하기 위해.
				SI16	 oldSetItemType5 =  pclchar->m_siSetItemType5; // 바뀌었는지 비교하기 위해.

				cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;
				
				SI16 setItemType  = 0;
				SI16 setItemStep  = 0;
				SI16 setItemType2 = 0;
				SI16 setItemStep2 = 0;
				SI16 setItemType3 = 0;
				SI16 setItemStep3 = 0;
				SI16 setItemType4 = 0;
				SI16 setItemStep4 = 0;
				SI16 setItemType5 = 0;
				SI16 setItemStep5 = 0;

				setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep, USE_SETITEM_1 );
				setItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep2, USE_SETITEM_2 );
				setItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep3, USE_SETITEM_3 );
				setItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep4, USE_SETITEM_4 );
				setItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep5, USE_SETITEM_5 );

				pclchar->m_siSetItemType = setItemType;
				pclchar->m_siSetItemStep = setItemStep;

				pclchar->m_siSetItemType2 = setItemType2;
				pclchar->m_siSetItemStep2 = setItemStep2;

				pclchar->m_siSetItemType3 = setItemType3;
				pclchar->m_siSetItemStep3 = setItemStep3;

				pclchar->m_siSetItemType4 = setItemType4;
				pclchar->m_siSetItemStep4 = setItemStep4;

				pclchar->m_siSetItemType5 = setItemType5;
				pclchar->m_siSetItemStep5 = setItemStep5;

				if(	(oldSetItemType  != pclchar->m_siSetItemType)	|| 
					(oldSetItemType2 != pclchar->m_siSetItemType2)	|| 
					(oldSetItemType3 != pclchar->m_siSetItemType3)	|| 
					(oldSetItemType4 != pclchar->m_siSetItemType4)	|| 
					(oldSetItemType5 != pclchar->m_siSetItemType5))
				{
					cltGameMsgResponse_SetItemTypeInfo clinfo3( setItemType, setItemStep, setItemType2, setItemStep2, setItemType3, setItemStep3, setItemType4, setItemStep4, setItemType5, setItemStep5);
					cltMsg clMsg3(GAMEMSG_RESPONSE_SETITEMTYPEINFO, sizeof(clinfo3), (BYTE*)&clinfo3);
					//pclchar->SendNetMsg((sPacketHeader*)&clMsg3);
					((cltCharServer*)pclchar)->SendNeighbourMsg(&clMsg3, true);
				}
				//-----------------------------------------------------------------------------

				if( pclchar->m_siSetItemType )
				{
					cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
					if( pcltSetItemAbilityManager )
					{
						clSetItemAdd.SetStr( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_5)  );
						clSetItemAdd.SetDex( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_5)  );
						clSetItemAdd.SetMag( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_5)  );
						clSetItemAdd.SetVit( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_5)  );
						clSetItemAdd.SetHnd( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_1) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_5) );
						clSetItemAdd.SetLuk( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_5)  );
						clSetItemAdd.SetWis( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_2)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_3)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_4)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_5)  );
					}
				}
			}
			else
			{
				if( pclchar && pclchar->m_siSetItemType == VAMPIRE_SETITEM )
				{
					clSetItemAdd.SetStr( 10 );
					clSetItemAdd.SetDex( 10 );
					clSetItemAdd.SetMag( 10 );
					clSetItemAdd.SetVit( 10 );
				}
			}
		}
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		if( pclchar && (IsPC( pclchar->GetID() ) == 1))
		{
			if( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				//-----------------------------------------------------------------------------
				cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

				SI16 setItemType  = 0;
				SI16 setItemStep  = 0;
				SI16 setItemType2 = 0;
				SI16 setItemStep2 = 0;
				SI16 setItemType3 = 0;
				SI16 setItemStep3 = 0;
				SI16 setItemType4 = 0;
				SI16 setItemStep4 = 0;
				SI16 setItemType5 = 0;
				SI16 setItemStep5 = 0;

				setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep, USE_SETITEM_1 );
				setItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep2, USE_SETITEM_2 );
				setItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep3, USE_SETITEM_3 );
				setItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep4, USE_SETITEM_4 );
				setItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep5, USE_SETITEM_5 );

				pclchar->m_siSetItemType = setItemType;
				pclchar->m_siSetItemStep = setItemStep;

				pclchar->m_siSetItemType2 = setItemType2;
				pclchar->m_siSetItemStep2 = setItemStep2;

				pclchar->m_siSetItemType3 = setItemType3;
				pclchar->m_siSetItemStep3 = setItemStep3;

				pclchar->m_siSetItemType4 = setItemType4;
				pclchar->m_siSetItemStep4 = setItemStep4;

				pclchar->m_siSetItemType5 = setItemType5;
				pclchar->m_siSetItemStep5 = setItemStep5;
				//-----------------------------------------------------------------------------

				if( pclchar->m_siSetItemType )
				{
					cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
					if( pcltSetItemAbilityManager )
					{
						clSetItemAdd.SetStr( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_STR, USE_SETITEM_5) );
						clSetItemAdd.SetDex( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_DEX, USE_SETITEM_5) );
						clSetItemAdd.SetMag( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_MAG, USE_SETITEM_5) );
						clSetItemAdd.SetVit( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_VIT, USE_SETITEM_5) );
						clSetItemAdd.SetHnd( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_1)  + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_2)+ pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_3)+ pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_4)+ pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_HAND, USE_SETITEM_5));
						clSetItemAdd.SetLuk( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_LUK, USE_SETITEM_5) );
						clSetItemAdd.SetWis( pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_1)   + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_2) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_3) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_4) + pcltSetItemAbilityManager->GetSetItemAbilityValue(pclchar, SETITEM_ABILITY_TYPE_WIS, USE_SETITEM_5) );
					}
				}
			}
			else
			{
				if( pclchar && pclchar->m_siSetItemType == VAMPIRE_SETITEM )
				{
					clSetItemAdd.SetStr( 10 );
					clSetItemAdd.SetDex( 10 );
					clSetItemAdd.SetMag( 10 );
					clSetItemAdd.SetVit( 10 );
				}
			}
		}
	}
}

void cltParametaBox::SetSkillAddAbility(cltItem* pclitemlist, SI32 activeweaponpos, cltPSkillInfo* pclskillinfo, cltBasicAbility BasicBA)
{
	// 무기술의 달인에 의한 스텟향상
	clSkillAdd.Init();

	if ( NULL == pclitemlist )																	return;
	if ( !(PERSONITEM_WEAPON1 == activeweaponpos) && !(PERSONITEM_WEAPON2 == activeweaponpos) )	return;
	if ( NULL == pclskillinfo )																	return;

	cltWeaponSkillMasterManager* pclGetMasterMgr = &pclClient->pclSkillManager->m_clMasterManager;
	if ( NULL == pclGetMasterMgr )																return;

	SI32 siStrRate		= 0;
	SI32 siHndRate		= 0;
	SI32 siDexRate		= 0;
	SI32 siMagRate		= 0;
	SI32 siWisRate		= 0;
	SI32 siVitRate		= 0;
	SI32 siLukRate		= 0;

	SI32 siItemType		= pclItemManager->GetItemType( pclitemlist[activeweaponpos].siUnique );
	SI32 siSkillUnique	= pclGetMasterMgr->GetSkillUseWeaponFromItemType( siItemType ) ;
	SI32 siSkillLevel	= 0;

	if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
	{
		siSkillLevel	= pclskillinfo->GetSkillExpLevel( siSkillUnique );

		siStrRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_STR );
		siHndRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_HAND );
		siDexRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_DEX );
		siMagRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_MAG );
		siWisRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_WIS );
		siVitRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_VIT );
		siLukRate		= pclGetMasterMgr->GetApplyValue( siItemType, siSkillLevel, SKILLMASTER_LUCK );

		if ( siStrRate )	clSkillAdd.SetStr( (BasicBA.GetStr()*siStrRate/100) );
		if ( siHndRate )	clSkillAdd.SetHnd( (BasicBA.GetHnd()*siHndRate/100) );
		if ( siDexRate )	clSkillAdd.SetDex( (BasicBA.GetDex()*siDexRate/100) );
		if ( siMagRate )	clSkillAdd.SetMag( (BasicBA.GetMag()*siMagRate/100) );
		if ( siWisRate )	clSkillAdd.SetWis( (BasicBA.GetWis()*siWisRate/100) );
		if ( siVitRate )	clSkillAdd.SetVit( (BasicBA.GetVit()*siVitRate/100) );
		if ( siLukRate )	clSkillAdd.SetLuk( (BasicBA.GetLuk()*siLukRate/100) );
	}

}

void cltParametaBox::SetSoulGuardAddAbility(cltCharCommon* pclchar )
{
	clSoulGuardAdd.Init();

	if ( false == pclClient->IsCountrySwitch(Switch_SoulGuard) )
	{
		return;
	}

	if (NULL == pclchar)
	{
		return;
	}

	if (GAMEMODE_SERVER == pclClient->GameMode)  
	{
		if ( false == IsPC(pclchar->GetCharUnique()) )	return;
	}
	else if ( GAMEMODE_CLIENT == pclClient->GameMode )
	{
		if ( pclchar->GetID() != 1 )					return;
	}

	if ( FALSE == pclchar->IsSoulGuardStatusOut() )
	{
		return;
	}

	SI16 siGetFellowShip = pclchar->pclSoulGuard->GetFellowShip();
	if ( 0 >= siGetFellowShip )
	{
		return;
	}

	SI16 siSoulGuardKind = pclchar->pclSoulGuard->GetKind();
	if ( (0 >= siSoulGuardKind) || (MAX_KIND_NUMBER <= siSoulGuardKind) )	
	{
		return;
	}

	cltKindInfo* pclGetKI = pclClient->pclKindInfoSet->pclKI[siSoulGuardKind];
	if ( NULL == pclGetKI )
	{
		return;
	}
		
	SI32 siStrAdd = (pclGetKI->clBA.GetStr() * siGetFellowShip) / 100;
	SI32 siHndAdd = (pclGetKI->clBA.GetHnd() * siGetFellowShip) / 100;
	SI32 siDexAdd = (pclGetKI->clBA.GetDex() * siGetFellowShip) / 100;
	SI32 siMagAdd = (pclGetKI->clBA.GetMag() * siGetFellowShip) / 100;
	SI32 siWisAdd = (pclGetKI->clBA.GetWis() * siGetFellowShip) / 100;
	SI32 siVitAdd = (pclGetKI->clBA.GetVit() * siGetFellowShip) / 100;
	SI32 siLukAdd = (pclGetKI->clBA.GetLuk() * siGetFellowShip) / 100;

	clSoulGuardAdd.SetStr( siStrAdd );
	clSoulGuardAdd.SetHnd( siHndAdd );
	clSoulGuardAdd.SetDex( siDexAdd );
	clSoulGuardAdd.SetMag( siMagAdd );
	clSoulGuardAdd.SetWis( siWisAdd );
	clSoulGuardAdd.SetVit( siVitAdd );
	clSoulGuardAdd.SetLuk( siLukAdd );
}

void cltParametaBox::SetBufStatusUp(cltCharCommon* pclchar)
{
	if ( pclchar && IsPC( pclchar->GetCharUnique() ) )
	{
		ZeroMemory(&clbufAdd, sizeof(clbufAdd));

		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_STR))
		{
			clbufAdd.SetStr(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_STR)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_DEX))
		{
			clbufAdd.SetDex(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_DEX)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_MAG))
		{
			clbufAdd.SetMag(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_MAG)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_WIS))
		{
			clbufAdd.SetWis(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_WIS)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_VIT))
		{
			clbufAdd.SetVit(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_VIT)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_HAND))
		{
			clbufAdd.SetHnd(pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_HAND)->siPercent);
		}
		if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL))// 금메달
		{
			SI32 siStr = pclchar->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_STR) ;
			SI32 siDex = pclchar->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_DEX) ;
			SI32 siMag = pclchar->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_MAG) ;
			SI32 siVit = pclchar->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_VIT) ;
			SI32 siHnd = pclchar->pclCI->clBI.clBufInfo.GetBufPercent(BUF_STATUE_UP_HAND) ;

			clbufAdd.SetStr(siStr + pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_OLYMPICMEDAL)->siPercent);
			clbufAdd.SetDex(siDex + pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_OLYMPICMEDAL)->siPercent);
			clbufAdd.SetMag(siMag + pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_OLYMPICMEDAL)->siPercent);
			clbufAdd.SetVit(siVit + pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_OLYMPICMEDAL)->siPercent);
			clbufAdd.SetHnd(siHnd + pclchar->pclCI->clBI.clBufInfo.GetBuf(BUF_OLYMPICMEDAL)->siPercent);
		}
	}
}

//[기형] 발렌타인 이벤트 - 버프 보상
void cltParametaBox::SetValentineEventAbility(cltCharCommon* pclchar)
{
	if( NULL == pclchar)
		return;

	clValentineAdd.Init();

	if( NULL == pclClient->m_pclValentineEventMgr)
		return;

	if( !IsPC(pclchar->GetID()))
		return;

	pclClient->m_pclValentineEventMgr->GetValentineBuf(pclchar,clValentineAdd);
}

void cltParametaBox::SetHinamatsuriAbility(cltCharCommon* pclchar)
{
	clHinamatsuriAdd.Init();

	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if( pclchar && IsPC( pclchar->GetCharUnique() ) )
		{
			if( pclchar->pclCI->clBI.siHomeVillage > 0 )
			{
				cltServer* pclserver = (cltServer*)pclClient;
				cltStrInfo* pclinfo = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );

				if (pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))) == TRUE )
				{
					clHinamatsuriAdd.SetStr( 5 );
					clHinamatsuriAdd.SetDex( 5 );
					clHinamatsuriAdd.SetMag( 5 );
					clHinamatsuriAdd.SetVit( 5 );
					clHinamatsuriAdd.SetHnd( 5 );
					clHinamatsuriAdd.SetWis( 5 );
				}
			}
		}
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		if( pclchar && IsPC( pclchar->GetCharUnique() ) )
		{
			cltCharClient* pclcharclient = (cltCharClient*)pclchar;

			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) 
			{
				if( pclcharclient->siEventNPCKind[i] == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))				
				{
					clHinamatsuriAdd.SetStr( 5 );
					clHinamatsuriAdd.SetDex( 5 );
					clHinamatsuriAdd.SetMag( 5 );
					clHinamatsuriAdd.SetVit( 5 );
					clHinamatsuriAdd.SetHnd( 5 );
					clHinamatsuriAdd.SetWis( 5 );
				}
			}
		}
	}
}

void cltParametaBox::SetWorldChristmasTreeAbility(cltCharCommon* pclchar)
{
	clWorldChristmasTreeAdd.Init();
	SI32	siWorldChristmasTree01Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"));
	SI32	siWorldChristmasTree02Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02"));

	if( pclClient->GameMode == GAMEMODE_SERVER )
	{
		if( pclchar && IsPC( pclchar->GetCharUnique() ) )
		{
			cltServer* pclserver = (cltServer*)pclClient;

			SI32 siWorldChristmasTree01ID = pclserver->siWorldTreeID[0];
			SI32 siWorldChristmasTree02ID = pclserver->siWorldTreeID[1];

			cltCharServer*	pclWorldChristmasTreeChar01 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree01ID];
			cltCharServer*	pclWorldChristmasTreeChar02 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree02ID];

			if(pclWorldChristmasTreeChar01)
			{
				if((pclWorldChristmasTreeChar01->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar01->GetKind() == siWorldChristmasTree01Kind))
				{
					clWorldChristmasTreeAdd.SetStr( 5 );
					clWorldChristmasTreeAdd.SetDex( 5 );
					clWorldChristmasTreeAdd.SetMag( 5 );
					clWorldChristmasTreeAdd.SetVit( 5 );
					clWorldChristmasTreeAdd.SetHnd( 5 );
					clWorldChristmasTreeAdd.SetWis( 5 );
				}
			}

			if(pclWorldChristmasTreeChar02)
			{
				if((pclWorldChristmasTreeChar02->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar02->GetKind() == siWorldChristmasTree02Kind))
				{
					clWorldChristmasTreeAdd.SetStr( 10 );
					clWorldChristmasTreeAdd.SetDex( 10 );
					clWorldChristmasTreeAdd.SetMag( 10 );
					clWorldChristmasTreeAdd.SetVit( 10 );
					clWorldChristmasTreeAdd.SetHnd( 10 );
					clWorldChristmasTreeAdd.SetWis( 10 );
				}
			}
		}
	}
	else if( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		if( pclchar && IsPC( pclchar->GetCharUnique() ) )
		{
			SI32 siWorldChristmasTree01ID = pclClient->siWorldTreeID[0];
			SI32 siWorldChristmasTree02ID = pclClient->siWorldTreeID[1];

			if(siWorldChristmasTree01ID > 0)
			{
				clWorldChristmasTreeAdd.SetStr( 5 );
				clWorldChristmasTreeAdd.SetDex( 5 );
				clWorldChristmasTreeAdd.SetMag( 5 );
				clWorldChristmasTreeAdd.SetVit( 5 );
				clWorldChristmasTreeAdd.SetHnd( 5 );
				clWorldChristmasTreeAdd.SetWis( 5 );
			}

			if(siWorldChristmasTree02ID > 0)
			{
				clWorldChristmasTreeAdd.SetStr( 10 );
				clWorldChristmasTreeAdd.SetDex( 10 );
				clWorldChristmasTreeAdd.SetMag( 10 );
				clWorldChristmasTreeAdd.SetVit( 10 );
				clWorldChristmasTreeAdd.SetHnd( 10 );
				clWorldChristmasTreeAdd.SetWis( 10 );
			}
		}
	}
}

// level에 필요한 경험치 값. (1부터 시작한다.)
SI64 GetRequiredExpForLevel(SI32 level, SI32 siType)// SI32->SI64 경험치 오버플로우때문에 수정
{
	if(level < 1)								return 0;
	if(level > ExpForLevelUp.GetSize() )		return 0;

	switch(siType)
	{
	case EXPFORLEVEL_TYPE_CHARACTOR:
		{
			if( level > MAX_CHARACTER_LEVEL )	return 0;
		}
		break;
	case EXPFORLEVEL_TYPE_GUILD:
		{
			if( level > MAX_GUILD_LEVEL )	return 0;
		}
		break;
	}

	return ExpForLevelUp[level - 1][1];
}

// level에 필요한 경험치 값. (1부터 시작한다.)
SI32 GetRequiredFameExpForLevel(SI32 level)
{
	if(level < 1)								return 0;

	return FameExpForLevelUp[level - 1][1];

}

// 명성 경험치를 근거로 명성등급을 구한다. 
SI32 GetFameLevelFromExp(SI32 exp)
{
	for(SI32 i = 0; i < FameExpForLevelUp.GetSize(); i++)
	{
		if(FameExpForLevelUp[i][1] > exp)return i;
	}

	return 0;
}

// 경험치를 근거로 이 경험치의 레벨을 구한다.
SI32 GetLevelFromExp(SI32 exp, SI32 siType)
{
	if ( exp <= 0 ) return 1 ;

	for ( SI32 i = 0 ; i < ExpForLevelUp.GetSize() ; i ++ )
	{
		if ( exp < ExpForLevelUp[i][1] )
		{
			return i ;
		}
	}

	SI32 siReturnLevel = ExpForLevelUp.GetSize();

	switch(siType)
	{
	case EXPFORLEVEL_TYPE_CHARACTOR:
		{
			siReturnLevel = MAX_CHARACTER_LEVEL;
		}
		break;
	case EXPFORLEVEL_TYPE_GUILD:
		{
			siReturnLevel = MAX_GUILD_LEVEL;
		}
		break;
	}

	return siReturnLevel;
}

BOOL LoadExpData()
{
	FILE	*fp = NULL;

	SI64	data;
	TCHAR	buffer[1024 * 100];
	TCHAR	delimiter[] = TEXT("`\t\n");
	TCHAR	*pToken = NULL;
	INT		index;

	//----------------------------------
	// 경험치를 구한다. 
	//----------------------------------
	NFile file; 

	file.LoadFile( TEXT("LevelUpExp.dat") );

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{

		NLoadCryptFile( TEXT("LevelUpExp.dat"),  (BYTE*)buffer );
		buffer[ pHeader->dwDataSize ] = NULL;

		TCHAR *pStart = &buffer[ 0 ];
		int pos = 0;
		int lc = 0;

		while( pos < (int)pHeader->dwDataSize && lc < 4 ) 
		{

			if( buffer[ pos ] == 13 ) { ++lc; pos+=2; }
			else ++pos;
		}

		lc = 0;
		pStart = &buffer[ pos ];
		while( pos < (int)pHeader->dwDataSize && lc < ExpForLevelUp.GetSize() ) 
		{
			if( buffer[ pos ] == 13 ) 
			{ 
				pos+=2; pStart = &buffer[ pos ]; 
				_stscanf(pStart, TEXT("%I64d %I64d %I64d %I64d"), &ExpForLevelUp[lc][0], &ExpForLevelUp[lc][1], &ExpForLevelUp[lc][2], &data);
				++lc; 
			}
			else ++pos;
		}


	} else {

		if ( (fp = _tfopen(TEXT("LevelUpExp.dat"), TEXT("rt"))) == NULL )
		{
			MsgBox(TEXT("Error"), TEXT("File Open Error"));
			return FALSE;
		}

		// 첫번째 5줄은 무시한다. 
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);

		index = 0;

		while( _fgetts(buffer, 1024, fp) != NULL)
		{ 
			if( index >= ExpForLevelUp.GetSize())	break;

			_stscanf(buffer, TEXT("%I64d %I64d %I64d %I64d"), &ExpForLevelUp[index][0], &ExpForLevelUp[index][1], &ExpForLevelUp[index][2], &data);
			index++;
		}

	}

	if( fp ) fclose(fp);

	//----------------------------------
	// 명성경험치를 구한다. 
	//----------------------------------
	file.LoadFile( TEXT("LevelUpFame.dat") );

	pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{

		NLoadCryptFile( TEXT("LevelUpFame.dat"),  (BYTE*)buffer );
		buffer[ pHeader->dwDataSize ] = NULL;

		TCHAR *pStart = &buffer[ 0 ];
		int pos = 0;
		int lc = 0;

		while( pos < (int)pHeader->dwDataSize && lc < 4 ) 
		{

			if( buffer[ pos ] == 13 ) { ++lc; pos+=2; }
			else ++pos;
		}

		lc = 0;
		pStart = &buffer[ pos ];
		while( pos < (int)pHeader->dwDataSize && lc < FameExpForLevelUp.GetSize() ) 
		{
			if( buffer[ pos ] == 13 ) 
			{ 
				pos+=2; pStart = &buffer[ pos ]; 
				_stscanf(pStart, TEXT("%I64d %I64d %I64d"), &FameExpForLevelUp[lc][0], &FameExpForLevelUp[lc][1], &FameExpForLevelUp[lc][2]);
				++lc; 
			}
			else ++pos;
		}
	}
	else
	{
		if ( (fp = _tfopen(TEXT("LevelUpFame.dat"), TEXT("rt"))) == NULL )
		{
			MsgBox(TEXT("Error"), TEXT("File Open Error fd3"));
			return FALSE;
		}

		// 첫번째 5줄은 무시한다. 
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);
		_fgetts(buffer, 1024, fp);

		index = 0;

		while( _fgetts(buffer, 1024, fp) != NULL)
		{ 
			if( index >= FameExpForLevelUp.GetSize())	break;

			_stscanf(buffer, TEXT("%I64d %I64d %I64d"), &FameExpForLevelUp[index][0], &FameExpForLevelUp[index][1], &FameExpForLevelUp[index][2]);
			index++;
		}

		fclose(fp);
	}


	return TRUE;

}

BOOL LoadLevelUpReward()
{
	TCHAR	datfilename[MAX_PATH];
	if( pclClient->siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardK.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardJ.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )	
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )  // 영어
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardE.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardT.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )  
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardU.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )  //NHN중국
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardC.txt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		StringCchCopy(datfilename, MAX_PATH, TEXT("ServerData\\LevelUpRewardG.txt"));
	}

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datfilename);							
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadLevelupReward()"), TEXT("LoadError:[%s]"), datfilename);
		return FALSE;
	}

	SI32 level;
	SI32 itemunique;
	SI32 itemcount;
	SI32 notice;
	TCHAR explain[256] = {0,};

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&level,			4,		
			NDATA_INT32,	&itemunique,	4,		
			NDATA_INT32,	&itemcount,		4,		
			NDATA_INT32,	&notice,		4,		
			NDATA_MBSTRING,	explain,	256,
			0,0,0
	};
	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData())
		{
			if(level <= 0 || level > MAX_CHARACTER_LEVEL)
			{
				MsgBox(TEXT("Error"), TEXT("Invalid Level"));
			}

			bool bSet = false;
			for(int i=0; i<MAX_LEVELUP_REWARD_ITEM; i++)
			{
				if(!LevelUpReward[level].IsSetItem(i))
				{
					if(LevelUpReward[level].SetItem(i, itemunique, itemcount, notice))
					{
						bSet = true;
						break;
					}
				}
			}
			if(bSet == false)
			{
				MsgBox(TEXT("Error"), TEXT("NoMore Item Reward"));
			}
		}
	}

	return TRUE;
}

