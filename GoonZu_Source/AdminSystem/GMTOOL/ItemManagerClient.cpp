#include ".\itemmanagerclient.h"
#include "../../GoonZuWorld/common/CommonHeader.h"

cltItemManagerClient::cltItemManagerClient(void)
{
	m_pclGameDataManager = NULL;
}

cltItemManagerClient::~cltItemManagerClient(void)
{
}

VOID cltItemManagerClient::InIt(NGameDataManager& gameDataManager)
{
	m_pclGameDataManager = &gameDataManager;
}	

VOID cltItemManagerClient::GetCrystalAbilityTypeName(SI16 siCrystalAbilityType ,SI16 cAbliltyTypeNum,TCHAR* szCrystalReturnString)
{
	TCHAR* pText = NULL;
	TCHAR tembuf[50] = {'\0'};

	switch(siCrystalAbilityType)
	{
	case CRYSTALABILITY_STATE_STR: 
	case CRYSTALABILITY_AVATAR_STATE_STR:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(1785);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_DEX:
	case CRYSTALABILITY_AVATAR_STATE_DEX:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(1786);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_MAG:
	case CRYSTALABILITY_AVATAR_STATE_MAG:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(1787);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_VIT:
	case CRYSTALABILITY_AVATAR_STATE_VIT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(1788);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_LUK:
	case CRYSTALABILITY_AVATAR_STATE_LUK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(10016);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_HND:
	case CRYSTALABILITY_AVATAR_STATE_HND:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(3100);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_SUMMON:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2878);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_FARM:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2883);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MINE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2884);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_FISH:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2881);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEFOOD:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2842);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_CAPTURE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2877);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_HORSETRAINING:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2876);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEIRON1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2871);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKETOOL1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(5774);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;		
	case CRYSTALABILITY_SKILL_MAKEMED:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2851);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKESWORD1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2854);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKESPEAR1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2855);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEAXE1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2856);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKESTAFF1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2857);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEBOW1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2858);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEGUN1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2859);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEARMOUR1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2852);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEHELMET1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2853);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKESHOES1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2861);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_MAKEBELT1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2862);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_ARTIFACT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8225);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USESWORD1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2865);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USESPEAR1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2866);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USEAXE1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2867);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USESTAFF1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2868);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USEBOW1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2869);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_USEGUN1:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(2870);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_STATE_WIS:
	case CRYSTALABILITY_AVATAR_STATE_WIS:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8000);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;

		// %가 붙는다. 
	case CRYSTALABILITY_EFFECT_PHYSICAL_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8001);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8002);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_PHYSICAL_HITRATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8003);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_HITRATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8004);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_PHYSICAL_DODGERATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8005);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_DODGERATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8006);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_PHYSICAL_DEFENSIVERATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8007);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_DEFENSIVERATE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8008);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_HITPOINT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8009);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MANAPOINT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8010);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_WEIGHT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8011);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_FIREATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8012);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_WATERATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8013);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_WINDATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8014);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_EARTHATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8015);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_FIREDEFENCE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8016);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_WATERDEFENCE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8017);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_WINDDEFENCE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8018);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_ATTRIBUTE_EARTHDEFENCE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8019);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;

	case CRYSTALABILITY_SKILL_ALLMAKESKILL:	
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8020);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_ALLAVATARSKILL:	
	case CRYSTALABILITY_AVATAR_SKILL_ALLAVATARSKILL:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40171);  // 모든 아바타 기술  - 
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_AVATAR_ALLMAKESKILL:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(30262);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_ALLWEAPONSKILL:
	case CRYSTALABILITY_AVATAR_ALLWEAPONSKILL:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8021);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_ALLPRODUCTSKILL:
	case CRYSTALABILITY_AVATAR_ALLPRODUCTSKILL:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8022);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;
	case CRYSTALABILITY_SKILL_ALLSKILL:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8023);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;

	case CRYSTALABILITY_EFFECT_PHYSICAL_MIN_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8374);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_PHYSICAL_MAX_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8375);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_MIN_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8376);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_EFFECT_MAGICAL_MAX_ATTACK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8377);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;
	case CRYSTALABILITY_SPECIAL_DEATHBLOW:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8765);
			StringCchPrintf(tembuf,50,TEXT("%s+%d%s"),pText,cAbliltyTypeNum ,TEXT("%"));
		}
		break;

	default:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(5910);
			StringCchPrintf(tembuf,50,TEXT("%s+%d"),pText,cAbliltyTypeNum );
		}
		break;		
	}

	StringCchCopy(szCrystalReturnString, 50, tembuf);

	//return szCrystalReturnString;
}

TCHAR* cltItemManagerClient::GetEnchantTypeName( SI32 siEnchantType)
{
	TCHAR* pText = NULL;

	switch(siEnchantType)
	{
	case ENCHANT_TYPE_FIRE_A:
		pText = m_pclGameDataManager->GetTxtFromMgr(5902);
		break;
	case ENCHANT_TYPE_FIRE_D:
		pText = m_pclGameDataManager->GetTxtFromMgr(5903);
		break;
	case ENCHANT_TYPE_WATER_A:
		pText = m_pclGameDataManager->GetTxtFromMgr(5904);
		break;
	case ENCHANT_TYPE_WATER_D:
		pText = m_pclGameDataManager->GetTxtFromMgr(5905);
		break;
	case ENCHANT_TYPE_WIND_A:
		pText = m_pclGameDataManager->GetTxtFromMgr(5906);
		break;
	case ENCHANT_TYPE_WIND_D:
		pText = m_pclGameDataManager->GetTxtFromMgr(5907);
		break;
	case ENCHANT_TYPE_GROUND_A:
		pText = m_pclGameDataManager->GetTxtFromMgr(5908);
		break;
	case ENCHANT_TYPE_GROUND_D:
		pText = m_pclGameDataManager->GetTxtFromMgr(5909);
		break;
	default:
		pText = m_pclGameDataManager->GetTxtFromMgr(5910);
		break;		
	}

	return pText;
}

TCHAR* cltItemManagerClient::GetSkillNameByUnique( SI32 siSkillUnique)
{
	TCHAR* pText = NULL;

	switch(siSkillUnique)
	{
	case SKILL_MAKEMED:
		pText = m_pclGameDataManager->GetTxtFromMgr(2851);
		break;
	case SKILL_MAKEFOOD:
		pText = m_pclGameDataManager->GetTxtFromMgr(2842);
		break;
	case SKILL_MAKEARMOUR1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2852);
		break;
	case SKILL_MAKEHELMET1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2853);
		break;
	case SKILL_MAKESWORD1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2854);
		break;
	case SKILL_MAKESPEAR1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2855);
		break;
	case SKILL_MAKEAXE1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2856);
		break;
	case SKILL_MAKESTAFF1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2857);
		break;
	case SKILL_MAKEBOW1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2858);
		break;
	case SKILL_MAKEGUN1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2859);
		break;
	case SKILL_MAKESHOES1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2861);
		break;
	case SKILL_MAKEBELT1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2862);
		break;
	case SKILL_MAKEDRESS1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2873);
		break;
	case SKILL_USESWORD1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2865);
		break;
	case SKILL_USESPEAR1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2866);
		break;
	case SKILL_USEBOW1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2869);
		break;
	case SKILL_USEAXE1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2867);
		break;
	case SKILL_USEGUN1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2870);
		break;
	case SKILL_USESTAFF1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2868);
		break;
	case SKILL_SUMMON:
		pText = m_pclGameDataManager->GetTxtFromMgr(2878);
		break;
	case SKILL_FISH:
		pText = m_pclGameDataManager->GetTxtFromMgr(2881);
		break;
	case SKILL_HORSETRAINING:
		pText = m_pclGameDataManager->GetTxtFromMgr(2876);
		break;
	case SKILL_MAKEIRON1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2871);
		break;
	case SKILL_CAPTURE:
		pText = m_pclGameDataManager->GetTxtFromMgr(2877);
		break;
	case SKILL_MAKEHAT1:
		pText = m_pclGameDataManager->GetTxtFromMgr(2874);
		break;
	case SKILL_FARM:
		pText = m_pclGameDataManager->GetTxtFromMgr(2883);
		break;
	case SKILL_MINE:
		pText = m_pclGameDataManager->GetTxtFromMgr(2884);
		break;
	case ALL_MAKESKILL_UNIQUE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8020);
			break;
		}
	case ALL_WEAPONSKILL_UNIQUE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8021);
			break;
		}
	case ALL_PRODUCTSKILL_UNIQUE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8022);
			break;
		}
	case ALL_ALLSKILL_UNIQUE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(8023);
			break;
		}
	case ALL_ALLAVATARSKILL_UNIQUE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40171);
			break;
		}
	case SKILL_MAKEAVATAR_DRESS:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40145);
			break;
		}
	case SKILL_MAKEAVATAR_HAT:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40147);
			break;
		}
	case SKILL_MAKEAVATAR_MANTLE:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40148);
			break;
		}
	case SKILL_MAKEAVATAR_RING:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40149);
			break;
		}
	case SKILL_MAKEAVATAR_NECK:
		{
			pText = m_pclGameDataManager->GetTxtFromMgr(40150);
			break;
		}
	}

	return pText;
}

void cltItemManagerClient::SetItemData(int siServiceArea)
{
	if( siServiceArea == ConstServiceArea_Korea )	// 한국
	{
		// 검류. (1~99)
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordK.txt"));

		// 창류. (100~199)
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearK.txt"));

		// 지팡이류. (200~299)
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffK.txt"));

		// 도끼류. (300~399)
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeK.txt"));

		// 활류 (400~499)
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowK.txt"));

		// 총류 (500~599)
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunK.txt"));

		// 갑옷류. (600~699)
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourK.txt"));

		// 투구류. (700~799)
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetK.txt"));

		// 요대류. (800~999)
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltK.txt"));

		// 목걸이 류. (1000~)
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckK.txt"));

		// 신발류. (1500~)
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesK.txt"));

		// 약류.(2000~)
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealK.txt"));

		// 음식류.(3000~)
		AddFoodItem(TEXT("Item/Data/ItemTypeFoodK.txt"));

		// 책류(4000~)
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookK.txt"));

		// 광석류(5000~)
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralK.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralK.txt"));

		// 동물류 (5500~)
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalK.txt"));

		// 목재류(6000~)
		AddItem(TEXT("Item\\Data\\ItemTypeTreeK.txt"), TEXT("Tree"), ITEMTYPE_TREE);

		// 문구류(6500~)
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperK.txt"));

		// 천류(7000~)
		AddItem(TEXT("Item\\Data\\ItemTypeClothK.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);

		// 반지류(7500~)
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingK.txt") );

		// 기타류(8000~)
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcK.txt"), TEXT("Etc"), ITEMTYPE_ETC);

		// 부적(8500~)
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollK.txt") );

		// 의복류. (9000~9999)
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressK.txt"));

		// 철물류. (10000~10499)
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronK.txt"));

		// 도기류. (10500~10999)
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishK.txt"));

		// 소환인형.(11000 ~ 12999)
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollK.txt"));

		// 모자류. (13000~13999)
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatK.txt"));

		// 표.(14000 ~ 14999)
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketK.txt"));

		// 가방 (15000 ~ 15999)
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagK.txt"));

		// 수산물류.(16000~16999)
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodK.txt"));

		// 어구류. (17000 ~ 17999)
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodK.txt"));

		// 망토 (18000 ~ 18599)
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleK.txt"));


		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{	
		// 아바타 망토 (18600 ~ 18999 ) - 아이템몰에서 판매된 몇 가지 아이템 외에 나머지는 괄호안의 번호 사용. (임의로 설정한 것. 나중에 변경될 수 있음)
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleK.txt"));

		// 아바타 목걸이
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckK.txt"));

		// 아바타 의복
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressK.txt"));

		// 아바타 모자
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatK.txt"));

		// 아바타 반지
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingK.txt"));
		//}

		// 퀘스트 아이템
		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestK.txt"));

		// 이벤트 아이템 (26000~ 27999)	
		//	AddItem(TEXT("Item\\Data\\ItemTypeEvent.txt"), TEXT("Event"), ITEMTYPE_CLOTH);
		// TradeServer
		//AddTradeServerRank("item\\data\\TradeRankK.txt");

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		AddCrystalItem("item\\data\\ItemTypeCrystalK.txt");

		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannon.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedo.txt"));
		*/

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialK.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemK.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxK.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumK.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템
	}
	else if( siServiceArea == ConstServiceArea_Japan )
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordJ.txt"));

		// 창류. (100~199)
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearJ.txt"));

		// 지팡이류. (200~299)
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffJ.txt"));

		// 도끼류. (300~399)
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeJ.txt"));

		// 활류 (400~499)
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowJ.txt"));

		// 총류 (500~599)
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunJ.txt"));

		// 갑옷류. (600~699)
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourJ.txt"));

		// 투구류. (700~799)
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetJ.txt"));

		// 요대류. (800~999)
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltJ.txt"));

		// 목걸이 류. (1000~)
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckJ.txt"));

		// 신발류. (1500~)
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesJ.txt"));

		// 약류.(2000~)
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealJ.txt"));

		// 음식류.(3000~)
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodJ.txt"));

		// 책류(4000~)
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookJ.txt"));

		// 광석류(5000~)
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralJ.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralJ.txt"));

		// 동물류 (5500~)
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalJ.txt"));	

		// 목재류(6000~)
		AddItem(TEXT("Item\\Data\\ItemTypeTreeJ.txt"), TEXT("Tree"), ITEMTYPE_TREE);

		// 문구류(6500~)
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperJ.txt"));

		// 천류(7000~)
		AddItem(TEXT("Item\\Data\\ItemTypeClothJ.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);

		// 반지류(7500~)
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingJ.txt") );

		// 기타류(8000~)
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcJ.txt"), TEXT("Etc"), ITEMTYPE_ETC);

		// 부적(8500~)
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollJ.txt") );

		// 의복류. (9000~9999)
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressJ.txt"));

		// 철물류. (10000~10499)
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronJ.txt"));

		// 도기류. (10500~10999)
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishJ.txt"));

		// 소환인형.(11000 ~ 12999)
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollJ.txt"));

		// 모자류. (13000~13999)
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatJ.txt"));

		// 표.(14000 ~ 14999)
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketJ.txt"));

		// 가방 (15000 ~ 15999)
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagJ.txt"));

		// 수산물류.(16000~16999)
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodJ.txt"));

		// 어구류. (17000 ~ 17999)
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodJ.txt"));

		// 망토 (18000 ~ 18599)
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleJ.txt"));

		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		// 아바타 망토 (18600 ~ 18999)
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleJ.txt"));

		// 아바타 목걸이
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckJ.txt"));

		// 아바타 의복
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressJ.txt"));

		// 아바타 모자
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatJ.txt"));

		// 아바타 반지
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingJ.txt"));
		//}

		// 퀘스트 (24000~
		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestJ.txt"));

		// TradeServer
		//AddTradeServerRank("item\\data\\TradeRank.txt");

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialJ.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemJ.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxJ.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumJ.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		AddCrystalItem("item\\data\\ItemTypeCrystalJ.txt");
		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonJ.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoJ.txt"));
		*/
	}

	else if( siServiceArea == ConstServiceArea_China )
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordC.txt"));

		// 창류. (100~199)
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearC.txt"));

		// 지팡이류. (200~299)
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffC.txt"));

		// 도끼류. (300~399)
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeC.txt"));

		// 활류 (400~499)
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowC.txt"));

		// 총류 (500~599)
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunC.txt"));

		// 갑옷류. (600~699)
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourC.txt"));

		// 투구류. (700~799)
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetC.txt"));

		// 요대류. (800~999)
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltC.txt"));

		// 목걸이 류. (1000~)
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckC.txt"));

		// 신발류. (1500~)
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesC.txt"));

		// 약류.(2000~)
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealC.txt"));

		// 음식류.(3000~)
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodC.txt"));

		// 책류(4000~)
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookC.txt"));

		// 광석류(5000~)
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralC.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralC.txt"));

		// 동물류 (5500~)
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalC.txt"));

		// 목재류(6000~)
		AddItem(TEXT("Item\\Data\\ItemTypeTreeC.txt"), TEXT("Tree"), ITEMTYPE_TREE);

		// 문구류(6500~)
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperC.txt"));

		// 천류(7000~)
		AddItem(TEXT("Item\\Data\\ItemTypeClothC.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);

		// 반지류(7500~)
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingC.txt") );

		// 기타류(8000~)
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcC.txt"), TEXT("Etc"), ITEMTYPE_ETC);

		// 부적(8500~)
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollC.txt") );

		// 의복류. (9000~9999)
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressC.txt"));

		// 철물류. (10000~10499)
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronC.txt"));

		// 도기류. (10500~10999)
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishC.txt"));

		// 소환인형.(11000 ~ 12999)
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollC.txt"));

		// 모자류. (13000~13999)
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatC.txt"));

		// 표.(14000 ~ 14999)
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketC.txt"));

		// 가방 (15000 ~ 15999)
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagC.txt"));

		// 수산물류.(16000~16999)
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodC.txt"));

		// 어구류. (17000 ~ 17999)
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodC.txt"));

		// 망토 (18000 ~ 18599)
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleC.txt"));

		//if( pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		// 아바타 망토 (18600 ~ 18999)
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleC.txt"));

		// 아바타 목걸이
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckC.txt"));

		// 아바타 의복
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressC.txt"));

		// 아바타 모자
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatC.txt"));

		// 아바타 반지
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingC.txt"));
		//}

		// 퀘스트 (24000~
		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestJ.txt"));

		// TradeServer
		//AddTradeServerRank("item\\data\\TradeRank.txt");

		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonC.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoC.txt"));
		*/
	}
	else if( siServiceArea == ConstServiceArea_English )  // 영어
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordE.txt"));
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearE.txt"));
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffE.txt"));
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeE.txt"));
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowE.txt"));
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunE.txt"));
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourE.txt"));
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetE.txt"));
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltE.txt"));
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckE.txt"));
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesE.txt"));
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealE.txt"));
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodE.txt"));
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookE.txt"));
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralE.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralE.txt"));
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalE.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeTreeE.txt"), TEXT("Tree"), ITEMTYPE_TREE);
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperE.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeClothE.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingE.txt") );
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcE.txt"), TEXT("Etc"), ITEMTYPE_ETC);
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollE.txt") );
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressE.txt"));
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronE.txt"));
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishE.txt"));
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollE.txt"));
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatE.txt"));
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketE.txt"));
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagE.txt"));
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodE.txt"));
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodE.txt"));
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleE.txt"));

		//if( pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleE.txt"));
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressE.txt"));
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatE.txt"));
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckE.txt"));
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingE.txt"));
		//}

		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestE.txt"));
		//AddTradeServerRank("item\\data\\TradeRank.txt");


		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonE.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoE.txt"));
		*/

		/*}*/
		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		// 결정체류(1801~1999)
		AddCrystalItem("item\\data\\ItemTypeCrystalE.txt");
		////합성재료류(1701~1800)
		//AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialE.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		////이벤트아이템류(13000~13299)
		//AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemE.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		////박스류(13300~13599)
		//AddBox(TEXT("Item\\Data\\ItemTypeBoxE.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		////프리미엄아이템류(13600~13899)
		//AddPremium(TEXT("Item\\Data\\ItemTypePremiumE.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialE.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemE.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxE.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumE.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템
	}
	else if( siServiceArea == ConstServiceArea_Taiwan )  //대만.
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordT.txt"));
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearT.txt"));
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffT.txt"));
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeT.txt"));
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowT.txt"));
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunT.txt"));
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourT.txt"));
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetT.txt"));
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltT.txt"));
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckT.txt"));
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesT.txt"));
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealT.txt"));
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodT.txt"));
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookT.txt"));
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralT.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralT.txt"));
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalT.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeTreeT.txt"), TEXT("Tree"), ITEMTYPE_TREE);
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperT.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeClothT.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingT.txt") );
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcT.txt"), TEXT("Etc"), ITEMTYPE_ETC);
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollT.txt") );
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressT.txt"));
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronT.txt"));
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishT.txt"));
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollT.txt"));
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatT.txt"));
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketT.txt"));
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagT.txt"));
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodT.txt"));
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodT.txt"));
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleT.txt"));

		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleT.txt"));
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressT.txt"));
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatT.txt"));
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckT.txt"));
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingT.txt"));
		//}

		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestT.txt"));
		//AddTradeServerRank("item\\data\\TradeRank.txt");

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialT.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemT.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxT.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumT.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		AddCrystalItem("item\\data\\ItemTypeCrystalT.txt");

		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonE.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoE.txt"));
		*/

		/*}*/
	}
	else if( siServiceArea == ConstServiceArea_USA )  //USA
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordU.txt"));
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearU.txt"));
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffU.txt"));
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeU.txt"));
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowU.txt"));
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunU.txt"));
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourU.txt"));
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetU.txt"));
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltU.txt"));
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckU.txt"));
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesU.txt"));
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealU.txt"));
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodU.txt"));
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookU.txt"));
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralU.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralU.txt"));
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalU.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeTreeU.txt"), TEXT("Tree"), ITEMTYPE_TREE);
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperU.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeClothU.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingU.txt") );
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcU.txt"), TEXT("Etc"), ITEMTYPE_ETC);
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollU.txt") );
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressU.txt"));
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronU.txt"));
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishU.txt"));
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollU.txt"));
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatU.txt"));
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketU.txt"));
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagU.txt"));
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodU.txt"));
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodU.txt"));
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleU.txt"));

		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleU.txt"));
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressU.txt"));
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatU.txt"));
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckU.txt"));
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingU.txt"));
		//}

		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestU.txt"));
		//AddTradeServerRank("item\\data\\TradeRank.txt");

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialU.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemU.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxU.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumU.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템

		//if( pclClient->IsCountrySwitch( Switch_Artifact ) )
		//{
			//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
			AddCrystalItem("item\\data\\ItemTypeCrystalU.txt");
		//}

		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonE.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoE.txt"));
		*/

		/*}*/
	}
	else if( siServiceArea == ConstServiceArea_NHNChina )  // 영어
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordC.txt"));
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearC.txt"));
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffC.txt"));
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeC.txt"));
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowC.txt"));
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunC.txt"));
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourC.txt"));
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetC.txt"));
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltC.txt"));
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckC.txt"));
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesC.txt"));
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealC.txt"));
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodC.txt"));
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookC.txt"));
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralC.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralC.txt"));
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalC.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeTreeC.txt"), TEXT("Tree"), ITEMTYPE_TREE);
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperC.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeClothC.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingC.txt") );
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcC.txt"), TEXT("Etc"), ITEMTYPE_ETC);
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollC.txt") );
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressC.txt"));
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronC.txt"));
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishC.txt"));
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollC.txt"));
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatC.txt"));
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketC.txt"));
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagC.txt"));
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodC.txt"));
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodC.txt"));
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleC.txt"));

		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleC.txt"));
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressC.txt"));
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatC.txt"));
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckC.txt"));
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingC.txt"));
		//}

		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestC.txt"));
		//AddTradeServerRank("item\\data\\TradeRank.txt");

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		// 결정체류(1801~1999)
		AddCrystalItem("item\\data\\ItemTypeCrystalC.txt");
		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialC.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemC.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxC.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumC.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템
	}
	else if( siServiceArea == ConstServiceArea_EUROPE )
	{
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordG.txt"));
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearG.txt"));
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffG.txt"));
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeG.txt"));
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowG.txt"));
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunG.txt"));
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourG.txt"));
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetG.txt"));
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltG.txt"));
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckG.txt"));
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesG.txt"));
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealG.txt"));
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodG.txt"));
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookG.txt"));
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralG.txt"));
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalG.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeTreeG.txt"), TEXT("Tree"), ITEMTYPE_TREE);
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperG.txt"));
		AddItem(TEXT("Item\\Data\\ItemTypeClothG.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingG.txt") );
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcG.txt"), TEXT("Etc"), ITEMTYPE_ETC);
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollG.txt") );
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressG.txt"));
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronG.txt"));
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishG.txt"));
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollG.txt"));
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatG.txt"));
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketG.txt"));
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagG.txt"));
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodG.txt"));
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodG.txt"));
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleG.txt"));

		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleG.txt"));
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressG.txt"));
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatG.txt"));
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckG.txt"));
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingG.txt"));
		//}

		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestG.txt"));
		//AddTradeServerRank("item\\data\\TradeRank.txt");


		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannonG.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedoG.txt"));
		*/

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		// 결정체류(1801~1999)
		AddCrystalItem("item\\data\\ItemTypeCrystalG.txt");

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialG.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemG.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxG.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumG.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템
	}
	else
	{
		// 검류. (1~99)
		AddSwordItem(TEXT("Item\\Data\\ItemTypeSwordK.txt"));

		// 창류. (100~199)
		AddSpearItem(TEXT("Item\\Data\\ItemTypeSpearK.txt"));

		// 지팡이류. (200~299)
		AddStaffItem(TEXT("Item\\Data\\ItemTypeStaffK.txt"));

		// 도끼류. (300~399)
		AddAxeItem(TEXT("Item\\Data\\ItemTypeAxeK.txt"));

		// 활류 (400~499)
		AddBowItem(TEXT("Item\\Data\\ItemTypeBowK.txt"));

		// 총류 (500~599)
		AddGunItem(TEXT("Item\\Data\\ItemTypeGunK.txt"));

		// 갑옷류. (600~699)
		AddArmourItem(TEXT("Item\\Data\\ItemTypeArmourK.txt"));

		// 투구류. (700~799)
		AddHelmetItem(TEXT("Item\\Data\\ItemTypeHelmetK.txt"));

		// 요대류. (800~999)
		AddBeltItem(TEXT("Item\\Data\\ItemTypeBeltK.txt"));

		// 목걸이 류. (1000~)
		AddNeckItem(TEXT("Item\\Data\\ItemTypeNeckK.txt"));

		// 신발류. (1500~)
		AddShoesItem(TEXT("Item\\Data\\ItemTypeShoesK.txt"));

		// 약류.(2000~)
		AddHealItem(TEXT("Item\\Data\\ItemTypeHealK.txt"));

		// 음식류.(3000~)
		AddFoodItem(TEXT("Item\\Data\\ItemTypeFoodK.txt"));

		// 책류(4000~)
		AddBookItem(TEXT("Item\\Data\\ItemTypeBookK.txt"));

		// 광석류(5000~)
		//AddItem(TEXT("Item\\Data\\ItemTypeMineralK.txt"), TEXT("Mineral"), ITEMTYPE_MINERAL);
		AddMineralItem(TEXT("Item\\Data\\ItemTypeMineralK.txt"));

		// 동물류 (5500~)
		AddAnimalItem(TEXT("Item\\Data\\ItemTypeAnimalK.txt"));

		// 목재류(6000~)
		AddItem(TEXT("Item\\Data\\ItemTypeTreeK.txt"), TEXT("Tree"), ITEMTYPE_TREE);

		// 문구류(6500~)
		AddPaperItem(TEXT("Item\\Data\\ItemTypePaperK.txt"));

		// 천류(7000~)
		AddItem(TEXT("Item\\Data\\ItemTypeClothK.txt"), TEXT("Cloth"), ITEMTYPE_CLOTH);

		// 반지류(7500~)
		AddRingItem(TEXT("Item\\Data\\ItemTypeRingK.txt") );

		// 기타류(8000~)
		AddEtcItem(TEXT("Item\\Data\\ItemTypeEtcK.txt"), TEXT("Etc"), ITEMTYPE_ETC);

		// 부적(8500~)
		AddScrollItem(TEXT("Item\\Data\\ItemTypeScrollK.txt") );

		// 의복류. (9000~9999)
		AddDressItem(TEXT("Item\\Data\\ItemTypeDressK.txt"));

		// 철물류. (10000~10499)
		AddIronItem(TEXT("Item\\Data\\ItemTypeIronK.txt"));

		// 도기류. (10500~10999)
		AddDishItem(TEXT("Item\\Data\\ItemTypeDishK.txt"));

		// 소환인형.(11000 ~ 12999)
		AddDollItem(TEXT("Item\\Data\\ItemTypeDollK.txt"));

		// 모자류. (13000~13999)
		AddHatItem(TEXT("Item\\Data\\ItemTypeHatK.txt"));

		// 표.(14000 ~ 14999)
		AddTicketItem(TEXT("Item\\Data\\ItemTypeTicketK.txt"));

		// 가방 (15000 ~ 15999)
		AddBagItem(TEXT("Item\\Data\\ItemTypeBagK.txt"));

		// 수산물류.(16000~16999)
		AddSeaFoodItem(TEXT("Item\\Data\\ItemTypeSeaFoodK.txt"));

		// 어구류. (17000 ~ 17999)
		AddFishingRodItem(TEXT("Item\\Data\\ItemTypeFishingRodK.txt"));

		// 망토 (18000 ~ 18599)
		AddMantleItem(TEXT("Item\\Data\\ItemTypeMantleK.txt"));


		//if ( TRUE == pclClient->IsCountrySwitch(Switch_Avatar_artisan) )
		//{	
		// 아바타 망토 (18600 ~ 18999 ) - 아이템몰에서 판매된 몇 가지 아이템 외에 나머지는 괄호안의 번호 사용. (임의로 설정한 것. 나중에 변경될 수 있음)
		AddAvatarMantleItem(TEXT("Item\\Data\\ItemTypeAvatarMantleK.txt"));

		// 아바타 목걸이
		AddAvatarNeckItem(TEXT("Item\\Data\\ItemTypeAvatarNeckK.txt"));

		// 아바타 의복
		AddAvatarDressItem(TEXT("Item\\Data\\ItemTypeAvatarDressK.txt"));

		// 아바타 모자
		AddAvatarHatItem(TEXT("Item\\Data\\ItemTypeAvatarHatK.txt"));

		// 아바타 반지
		AddAvatarRingItem(TEXT("Item\\Data\\ItemTypeAvatarRingK.txt"));
		//}

		// 퀘스트 아이템
		AddQuestItem(TEXT("Item\\Data\\ItemTypeQuestK.txt"));

		// 이벤트 아이템 (26000~ 27999)	
		//	AddItem(TEXT("Item\\Data\\ItemTypeEvent.txt"), TEXT("Event"), ITEMTYPE_CLOTH);
		// TradeServer
		//AddTradeServerRank("item\\data\\TradeRankK.txt");

		//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
		AddCrystalItem("item\\data\\ItemTypeCrystalK.txt");

		/*cyj 배 삭제로 인해 대포, 어뢰 제외
		// 대포류 (19000~19999)
		AddCannonItem(TEXT("Item\\Data\\ItemTypeCannon.txt"));

		// 어뢰류 (20000~20999)
		AddTorpedoItem(TEXT("Item\\Data\\ItemTypeTorpedo.txt"));
		*/

		//합성재료류(1701~1800)
		AddCompositionManterial(TEXT("Item\\Data\\ItemTypeCompositionMaterialK.txt"), TEXT("CompositionManterial"), ITEMTYPE_COMPOSITIONMATERIAL);//아티팩트 합성재료
		//이벤트아이템류(13000~13299)
		AddEventitem(TEXT("Item\\Data\\ItemTypeEventitemK.txt"), TEXT("Eventitem"), ITEMTYPE_EVENTITEM);//이벤트 아이템
		//박스류(13300~13599)
		AddBox(TEXT("Item\\Data\\ItemTypeBoxK.txt"), TEXT("Box"), ITEMTYPE_BOX);//박스 아이템
		//프리미엄아이템류(13600~13899)
		AddPremium(TEXT("Item\\Data\\ItemTypePremiumK.txt"), TEXT("Premium"), ITEMTYPE_PREMIUM);//프리미엄아이템
	
	}
}

int cltItemManagerClient::GetType(int siItemUnique)
{
	if( siItemUnique <= 0 || siItemUnique >= MAX_ITEMINFO_NUMBER)
		return 0;

	return m_siItemType[siItemUnique];
}


// 아이템의 성능 설명을 얻어온다. 
BOOL cltItemManagerClient::GetExplain(cltItem* pclitem, TCHAR* pText)
{
	if( NULL == m_pclGameDataManager)
		return FALSE;

	const int MAX_TEXT_SIZE = 1024;

	TCHAR szTemp[MAX_TEXT_SIZE] = TEXT("");
	//TCHAR* pText = m_pclGameDataManager->GetTxtFromMgr(3529);

	if(pText == NULL)return FALSE;

	// 아이템의 타입 획득. 
	SI32 type = GetType(pclitem->siUnique);
	if(type == 0)return FALSE;

	// 내구도. 
	SI32 maxdur	= pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
	SI32 curdur	= pclitem->clItemCommon.clCommonInfo.uiDurability;

	TCHAR szType[MAX_TEXT_SIZE] = TEXT("");
	StringCchPrintf(szType, MAX_TEXT_SIZE, TEXT("%s : "), m_pclGameDataManager->GetTxtFromMgr(1782));
	
	TCHAR szName[MAX_TEXT_SIZE] = TEXT("");
	StringCchPrintf(szName, MAX_TEXT_SIZE, TEXT("%s %s\r\n"), m_pclGameDataManager->GetTxtFromMgr(6073), m_pclGameDataManager->GetItemName(pclitem->siUnique));
	StringCchCat(pText, MAX_TEXT_SIZE, szName);

	TCHAR* pszRare = NULL;
	switch(pclitem->clItemCommon.clCommonInfo.uiRareInfo)
	{
	case 0:
		{
			pszRare = m_pclGameDataManager->GetTxtFromMgr(1214);
			break;
		}
	case 1:
		{
			pszRare = m_pclGameDataManager->GetTxtFromMgr(715);
			break;
		}
	case 2:
		{
			pszRare = m_pclGameDataManager->GetTxtFromMgr(714);
			break;
		}
	case 5:
		{
			pszRare = m_pclGameDataManager->GetTxtFromMgr(716);
			break;
		}
	case 6:
		{
			pszRare = m_pclGameDataManager->GetTxtFromMgr(10200);
			break;
		}
	}

	StringCchPrintf(szTemp, MAX_TEXT_SIZE, TEXT("%s : %s\r\n"), m_pclGameDataManager->GetTxtFromMgr(9585), pszRare);
	StringCchCat(pText, MAX_TEXT_SIZE, szTemp);

	
	switch(type)
	{
	//KHY - 0227 - 결정체 (크리스탈) 제조 추가
	case ITEMTYPE_CRYSTAL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(8062));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			SI16 cAbliltyType = pclitem->clItemCrystal.GetCrystalAblilty();
			SI16 cAbliltyTypeNum = pclitem->clItemCrystal.GetCrystalAbilityNum();

			SI16 MaxDub = (SI16)((pclitem->clItemCrystal.GetMaxDurability()*100) /30000);// 최대 내구도 값 - 30000 고정.
			
			GetCrystalAbilityTypeName(cAbliltyType, cAbliltyTypeNum, szTemp);
			StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			StringCchPrintf(szTemp, MAX_TEXT_SIZE, TEXT("%d"), MaxDub);
			
		}
		break;
	case ITEMTYPE_BOW:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2372));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
				pclitem->clItemWeapon.uiMinDamage, 
				pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);
		}
		break;
	case ITEMTYPE_GUN:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(4114));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
				pclitem->clItemWeapon.uiMinDamage, 
				pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);
		}
		break;
	case ITEMTYPE_AXE:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2370));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
				pclitem->clItemWeapon.uiMinDamage, 
				pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);
		}
		break;
	case ITEMTYPE_STAFF:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(4448));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
				pclitem->clItemWeapon.uiMinDamage, 
				pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);

			if(pclitem->clItemWeapon.uiMag)
			{
				TCHAR szWiz[MAX_TEXT_SIZE] = TEXT("");
				StringCchPrintf(szWiz,MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3530), pclitem->clItemWeapon.uiMag);
				StringCchCat(pText, MAX_TEXT_SIZE, szWiz);
			}
			break;
		}
	case ITEMTYPE_SPEAR:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2368));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
				pclitem->clItemWeapon.uiMinDamage, 
				pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);
		}
		break;
	case ITEMTYPE_SWORD:
		{			
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2366));

			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAttack[MAX_TEXT_SIZE] = TEXT("");
			
			StringCchPrintf(szAttack, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(729), 
					pclitem->clItemWeapon.uiMinDamage, 
					pclitem->clItemWeapon.uiMaxDamage);

			StringCchCat(pText, MAX_TEXT_SIZE, szAttack);
		}
		break;
	case ITEMTYPE_FOOD:
	case ITEMTYPE_SEAFOOD:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2885));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
			break;
		}
	case ITEMTYPE_HEAL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2887));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		
		break;
		}
	case ITEMTYPE_BELT:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1029));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAC[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAC, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 
				pclitem->clItemArmour.uiAC);
			StringCchCat(pText, MAX_TEXT_SIZE, szAC);
			break;
		}
	case ITEMTYPE_HELMET:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1024));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAC[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAC, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 
				pclitem->clItemArmour.uiAC);
			StringCchCat(pText, MAX_TEXT_SIZE, szAC);

			break;
		}
	case ITEMTYPE_ARMOUR:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1026));
			
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			TCHAR szAC[MAX_TEXT_SIZE] = TEXT("");

			StringCchPrintf(szAC, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 
				pclitem->clItemArmour.uiAC);
			StringCchCat(pText, MAX_TEXT_SIZE, szAC);
		}
		break;
	case ITEMTYPE_AVATAR_DRESS:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1027));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_AVATAR_HAT:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1025));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_HAT:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1025));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			if(pclitem->clItemStatusArmour.uiStr)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3565), 	pclitem->clItemStatusArmour.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiDex)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3566), 	pclitem->clItemStatusArmour.uiDex);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiVit)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3567), 	pclitem->clItemStatusArmour.uiVit);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiMag)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6064), 	pclitem->clItemStatusArmour.uiMag);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiLuk)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6065), 	pclitem->clItemStatusArmour.uiLuk);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiHnd)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6108), 	pclitem->clItemStatusArmour.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
			break;
		}
	case ITEMTYPE_DRESS:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1027));

			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			if(pclitem->clItemStatusArmour.uiStr)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3565), 	pclitem->clItemStatusArmour.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiDex)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3566), 	pclitem->clItemStatusArmour.uiDex);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiVit)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3567), 	pclitem->clItemStatusArmour.uiVit);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiMag)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6064), 	pclitem->clItemStatusArmour.uiMag);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiLuk)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6065), 	pclitem->clItemStatusArmour.uiLuk);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemStatusArmour.uiHnd)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6108), 	pclitem->clItemStatusArmour.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
		}
		break;

	case ITEMTYPE_BAG:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1033));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
			if(pclitem->clItemBag.uiAC)
			{
				StringCchPrintf(szTemp,MAX_TEXT_SIZE,m_pclGameDataManager->GetTxtFromMgr(3562), pclitem->clItemBag.uiAC);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
			break;
		}
			// 아바타 망토
	case ITEMTYPE_AVATAR_MANTLE:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1032));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			if(pclitem->clItemAvatarMantle.uiAC)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 	pclitem->clItemAvatarMantle.uiAC);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemAvatarMantle.uiStr)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3565), 	pclitem->clItemAvatarMantle.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemAvatarMantle.uiDex)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3566), 	pclitem->clItemAvatarMantle.uiDex);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemAvatarMantle.uiVit)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3567), 	pclitem->clItemAvatarMantle.uiVit);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
		}
		break;
	case ITEMTYPE_MANTLE:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1032));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			if(pclitem->clItemMantle.uiAC)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 	pclitem->clItemMantle.uiAC);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemMantle.uiStr)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3565), 	pclitem->clItemMantle.uiStr);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemMantle.uiDex)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3566), 	pclitem->clItemMantle.uiDex);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}

			if(pclitem->clItemMantle.uiVit)
			{
				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3567), 	pclitem->clItemMantle.uiVit);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
			break;
		}
	case ITEMTYPE_SHOES:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1030));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

			StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3568), 
				pclitem->clItemShoes.uiAC,
				pclitem->clItemShoes.uiMoveSpeed);

			StringCchCat( pText , MAX_TEXT_SIZE,szTemp );

			break;
		}
	case ITEMTYPE_AVATAR_NECK:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1031));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
	
			TCHAR szSkillUnique[MAX_TEXT_SIZE] = TEXT("");
	
			TCHAR* pszText = GetSkillNameByUnique(pclitem->clItemAvatarNeck.uiUpgradeSkillUnique);
	
			if( NULL == pszText)
			{
				StringCchPrintf(szSkillUnique, MAX_TEXT_SIZE, TEXT("%d"), pclitem->clItemAvatarNeck.uiUpgradeSkillUnique);

				pText = szSkillUnique;
			}

			StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(30042), pszText, pclitem->clItemAvatarNeck.uiSkillValue);
			StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
		}
		break;
	case ITEMTYPE_NECK:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1031));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
			
			break;
		}

	//		// 조건을 확인한다. 
	//		if(bcolorswitch)
	//			if(pclItemInfo[ref]->clCondition.GetCheckedText(szcondition, 1024, NULL, 0 , pclClient->pclCM->CR[1] ) == true)
	//			{
	//				StringCchCopy(buffer, 1024, szcondition);
	//			}
	//			else
	//			{
	//				StringCchCopy(buffer, 1024, TEXT(""));
	//			}
	//		else
	//			if(pclItemInfo[ref]->clCondition.GetText(szcondition, 1024, NULL, 0  ) == true)
	//			{
	//				StringCchCopy(buffer, 1024, szcondition);
	//			}
	//			else
	//			{
	//				StringCchCopy(buffer, 1024, TEXT(""));
	//			}


	//			// 업그레이드 스킬 정보를 확보한다. 
	//			TCHAR SkillName[ 64 ] = TEXT("");

	//			//cyj 목걸이는 밀봉중일때는 능력향상 안보임
	//			if ( pclitem->clItemNeck.uiSkillPointUp && pclitem->IsSealingUp(pclClient->pclItemManager) == false)
	//			{
	//				SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( pclitem->siUnique );
	//				if(ref > 0)
	//				{
	//					SI32 SkillUnique = pclItemInfo[ Ref ]->siUpgradeSkillUnique;		// pclitem->cltItemAvatarNeck.uiSkillType;
	//					if(pclClient->pclSkillManager->GetSkillName( SkillUnique, SkillName ) ==  true)
	//					{
	//						pText = m_pclGameDataManager->GetTxtFromMgr(3564);
	//						StringCchPrintf(temp, 1024, pText, SkillName, pclitem->clItemNeck.uiSkillPointUp);
	//						StringCchCat(buffer, 1024, temp);
	//					}
	//					else if ( SkillUnique == MAX_SKILL_NUMBER )
	//					{
	//					}
	//					else if ( SkillUnique == ALL_MAKESKILL_UNIQUE )
	//					{
	//						pText = m_pclGameDataManager->GetTxtFromMgr(5685);
	//						StringCchPrintf(temp, 1024, pText, pclitem->clItemNeck.uiSkillPointUp);
	//						StringCchCat(buffer, 1024, temp);
	//					}
	//					else if ( SkillUnique == ALL_WEAPONSKILL_UNIQUE )
	//					{
	//						pText = m_pclGameDataManager->GetTxtFromMgr(5679);
	//						StringCchPrintf(temp, 1024, pText, pclitem->clItemNeck.uiSkillPointUp);
	//						StringCchCat(buffer, 1024, temp);
	//					}
	//					else if ( SkillUnique == ALL_PRODUCTSKILL_UNIQUE )
	//					{
	//						pText = m_pclGameDataManager->GetTxtFromMgr(6140);
	//						StringCchPrintf(temp, 1024, pText, pclitem->clItemNeck.uiSkillPointUp);
	//						StringCchCat(buffer, 1024, temp);
	//					}
	//					else if ( SkillUnique == ALL_ALLSKILL_UNIQUE)
	//					{
	//						pText = m_pclGameDataManager->GetTxtFromMgr(6266);
	//						StringCchPrintf(temp, 1024, pText, pclitem->clItemNeck.uiSkillPointUp);
	//						StringCchCat(buffer, 1024, temp);
	//					}


	//				}
	//			}
	//	}
	//	break;

			// 아바타 반지
		case ITEMTYPE_AVATAR_RING:
			{	
				StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1028));
				StringCchCat(pText, MAX_TEXT_SIZE, szType);
				StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

				if(pclitem->clItemAvatarRing.uiAC)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3556), 	pclitem->clItemAvatarRing.uiAC);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiStr)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(799), 	pclitem->clItemAvatarRing.uiStr);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}				
				if(pclitem->clItemAvatarRing.uiDex)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(800), 	pclitem->clItemAvatarRing.uiDex);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiVit)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(801), 	pclitem->clItemAvatarRing.uiVit);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiMag)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3530), 	pclitem->clItemAvatarRing.uiMag);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiLuk)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6453), 	pclitem->clItemAvatarRing.uiLuk);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiHnd)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6454), 	pclitem->clItemAvatarRing.uiHnd);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemAvatarRing.uiWis)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(10346), 	pclitem->clItemAvatarRing.uiWis);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}

			}
			break;
		case ITEMTYPE_RING:
			{			
				StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(1028));
				StringCchCat(pText, MAX_TEXT_SIZE, szType);
				StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

				if(pclitem->clItemRing.uiStr)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(799), 	pclitem->clItemRing.uiStr);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}				
				if(pclitem->clItemRing.uiDex)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(800), 	pclitem->clItemRing.uiDex);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemRing.uiVit)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(801), 	pclitem->clItemRing.uiVit);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemRing.uiMag)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3530), 	pclitem->clItemRing.uiMag);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemRing.uiLuk)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6453), 	pclitem->clItemRing.uiLuk);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}
				if(pclitem->clItemRing.uiHnd)
				{
					StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6454), 	pclitem->clItemRing.uiHnd);
					StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				}

			}
			break;

	case ITEMTYPE_BOOK:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2888));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;

	case ITEMTYPE_MINERAL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2889));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_TREE:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2891));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_SCROLL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2893));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_ANIMAL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2890));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_ETC:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2894));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
		}
		break;
	case ITEMTYPE_DOLL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2895));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_TICKET:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2896));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_IRON:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2399));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_DISH:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2398));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_PAPER:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2470));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_FISHINGROD:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2471));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_QUEST:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(601));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_COMPOSITIONMATERIAL:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(8063));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_EVENTITEM:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(3469));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_BOX:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(5720));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_PREMIUM:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(6615));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
		}
		break;
	case ITEMTYPE_CLOTH:
		{
			StringCchCat(szType, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(2892));
			StringCchCat(pText, MAX_TEXT_SIZE, szType);
			StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));
			break;
		}
	}

	if( pclitem->uiDateUseDay > 0 )
	{
		StringCchPrintf(szTemp, MAX_TEXT_SIZE, TEXT("%s : %d%s\r\n"), m_pclGameDataManager->GetTxtFromMgr(8763), pclitem->uiDateUseDay, m_pclGameDataManager->GetTxtFromMgr(4084));
		StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
	}

	if( pclitem->Element.siType != 0)
	{
		TCHAR* pszElementType = GetEnchantTypeName(pclitem->Element.siType);

		TCHAR szPower[MAX_TEXT_SIZE] = TEXT("");

		StringCchPrintf(szPower, MAX_TEXT_SIZE, TEXT("%d"), pclitem->Element.siPower);

		StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(5901), pszElementType, szPower);
		StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
	}

	if( pclitem->siUseCrystalNum != 0)
	{
		StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(7999), pclitem->siUseCrystalNum);
		StringCchCat(pText, MAX_TEXT_SIZE, szTemp);

		for (int i = 0; i < MAX_CRYSTAL_NUM; i++)
		{
			if( pclitem->cltItemCrytal[i].GetCrystalType() > 0)
			{
				StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

				StringCchPrintf(szTemp, MAX_TEXT_SIZE, TEXT("%s\r\n"), m_pclGameDataManager->GetItemName(pclitem->cltItemCrytal[i].GetCrystalType()));
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);

				GetCrystalAbilityTypeName(pclitem->cltItemCrytal[i].GetCrystalAblilty(), pclitem->cltItemCrytal[i].GetCrystalAbilityNum(), szTemp);
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
				StringCchCat(pText, MAX_TEXT_SIZE, TEXT("\r\n"));

				StringCchPrintf(szTemp, MAX_TEXT_SIZE, m_pclGameDataManager->GetTxtFromMgr(804), (pclitem->cltItemCrytal[i].GetDurability()/300), (pclitem->cltItemCrytal[i].GetMaxDurability()/300));
				StringCchCat(pText, MAX_TEXT_SIZE, szTemp);
			}
		}
	}


	// 옵션 사항이 있다면. 
	/*if(pclitem->clItemCommon.clOptionInfo.GetText(temp, 1024) == true)
	{
		StringCchCat(buffer, 1024, temp);
	}*/

	//if ( true == pclClient->IsCountrySwitch( Switch_NewDurability ) )
	//{
	//	// 내구도 표시.
	//	/*if(maxdur == siForeverDurability && type == ITEMTYPE_BAG)
	//	{
	//	pText = m_pclGameDataManager->GetTxtFromMgr(3609);
	//	StringCchPrintf(temp, 1024,  pText);
	//	StringCchCat(buffer, 1024, temp);
	//	}*/
	//	/*else if(maxdur && type == ITEMTYPE_BAG)
	//	{
	//	pText = m_pclGameDataManager->GetTxtFromMgr(804);
	//	StringCchPrintf(temp, 1024,  pText, curdur, maxdur);
	//	StringCchCat(buffer, 1024, temp);
	//	}*/
	//	if ( maxdur == siForeverDurability )
	//	{
	//		// 무한 내구도이기때문에 표시를 하지 않는다.
	//	}
	//	else if ( maxdur == siForeverDurability2 )
	//	{
	//		// 무한 내구도이기때문에 표시를 하지 않는다.
	//	}
	//	else if( maxdur )
	//	{
	//		//pText = m_pclGameDataManager->GetTxtFromMgr(804);
	//		//StringCchPrintf(temp, 1024,  pText, curdur, maxdur);
	//		//StringCchCat(buffer, 1024, temp);
	//		NTCHARString64 kDurExplain;
	//		GetDurabilityExplain( curdur, maxdur, kDurExplain, bcolorswitch );
	//		StringCchCat(buffer, 1024, kDurExplain);
	//	}
	//}

	//// 타입별 내구도 표시
	//if ( true == pclClient->IsCountrySwitch( Switch_Durability ) )
	//{
	//	if(pclitem != NULL && pclitem->IsSealingUp( pclClient->pclItemManager )	==false )
	//	{
	//		switch ( type )
	//		{
	//		case ITEMTYPE_SWORD:	// 검
	//		case ITEMTYPE_SPEAR:	// 창 
	//		case ITEMTYPE_AXE:		// 도끼
	//		case ITEMTYPE_BOW:		// 활
	//		case ITEMTYPE_GUN:		// 총
	//		case ITEMTYPE_STAFF:	// 지팡이
	//		case ITEMTYPE_ARMOUR:	// 갑옷 
	//		case ITEMTYPE_HELMET:	// 모자 
	//		case ITEMTYPE_BELT:		// 벨트
	//		case ITEMTYPE_SHOES:	// 신발
	//		case ITEMTYPE_MANTLE:	// 망토
	//		case ITEMTYPE_DRESS:	// 의복
	//		case ITEMTYPE_HAT:		// 모자
	//		case ITEMTYPE_RING:		// 반지
	//		case ITEMTYPE_NECK:		// 목걸이
	//		case ITEMTYPE_IRON:		// 철물
	//		case ITEMTYPE_PAPER:	// 문구
	//		case ITEMTYPE_DISH:		// 도기
	//		case ITEMTYPE_FISHINGROD:	// 어구
	//			//-----새롭게 추가된 아바타 물건들
	//		case ITEMTYPE_AVATAR_MANTLE:// 아바타 망토
	//		case ITEMTYPE_AVATAR_DRESS:	// 아바타 의복
	//		case ITEMTYPE_AVATAR_HAT:	// 아바타 모자
	//		case ITEMTYPE_AVATAR_NECK:	// 아바타 목걸이
	//		case ITEMTYPE_AVATAR_RING:	// 아바타 반지
	//			{

	//				static TCHAR tmpbuf[256]  = {'\0'};
	//				static TCHAR tmpbuf2[256] = {'\0'};

	//				pText = m_pclGameDataManager->GetTxtFromMgr(30277);

	//				TCHAR * szDurability = SI32ToString( MAX_DURABILITY_PERCENT_IN_CHINA )	;

	//				if(bcolorswitch)
	//				{
	//					StringCchPrintf(tmpbuf2,sizeof(tmpbuf2),TEXT("%s %s %s"),TEXT("<C05>"),szDurability,TEXT("</C>") );

	//					StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, tmpbuf2);
	//				}
	//				else
	//				{
	//					StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, szDurability);
	//				}

	//				StringCchCat(buffer, sizeof(buffer), tmpbuf);
	//			}
	//			break;
	//		}
	//	}

	//}

	// 무게가 추가된다

	//////////////////////// 속성 표시 ////////////////////////////////

	//// [지연] 아바타 의복, 모자는 속성 정보 표시 안해준다. 그외에 표시 안할것들도 여기에 추가.
	//bool bElementInfoException = false;
	//bElementInfoException = ( ITEMTYPE_AVATAR_DRESS == type || ITEMTYPE_AVATAR_HAT == type );


	//if(pclitem->Element.siType && true != bElementInfoException)
	//{
	//	TCHAR buf[64] = {'\0'};
	//	TCHAR buf2[64] = {'\0'};

	//	SI32 siElementGrade = pclitem->Element.siPower;

	//	if(bcolorswitch)
	//	{
	//		StringCchPrintf(buf,64,TEXT("%s%s%s"),TEXT("<C13>"),GetEnchantTypeName(buf, pclitem->Element.siType), TEXT("</C>") );
	//		StringCchPrintf(buf2,64,TEXT("%s%d%s"),TEXT("<C13>[ "), siElementGrade , TEXT(" ]</C>") ); // 보라색으로 

	//	}
	//	else
	//	{
	//		StringCchCopy(buf,64, GetEnchantTypeName(buf, pclitem->Element.siType));
	//		StringCchPrintf(buf2,64,TEXT("[ %d ]"), siElementGrade );
	//	}

	//	pText = m_pclGameDataManager->GetTxtFromMgr(5901);
	//	StringCchPrintf(temp,1024,pText,buf , buf2);

	//	StringCchCat(buffer, sizeof(buffer) ,temp);


	//	// 무기라면 속성 공격력, 방어구라면 속성 방어력을 표시해준다
	//	switch( type )
	//	{
	//	case ITEMTYPE_SWORD:
	//	case ITEMTYPE_SPEAR:
	//	case ITEMTYPE_STAFF:
	//	case ITEMTYPE_AXE:
	//	case ITEMTYPE_BOW:
	//	case ITEMTYPE_GUN:

	//		{
	//			SI32 EnchantDamage = GetEnchantDamageFromGrade( pclitem->Element.siPower );
	//			pText = m_pclGameDataManager->GetTxtFromMgr(7013);

	//			if(bcolorswitch)
	//			{
	//				StringCchPrintf( buf2 , 64 , TEXT("<C13>%d</C>") , EnchantDamage );// 보라색으로 
	//			}
	//			else
	//			{
	//				StringCchPrintf( buf2 , 64 , TEXT("%d") , EnchantDamage );
	//			}

	//			StringCchPrintf( buf , 64 , pText , buf2 );					
	//			StringCchCat(buffer, sizeof(buffer) ,buf);
	//		}
	//		break;
	//	default:
	//		{
	//			SI32 EnchantDef = pclitem->GetEnchantACFrom( this , true , pclitem->Element.siType );

	//			pText = m_pclGameDataManager->GetTxtFromMgr(7014);

	//			if(bcolorswitch)
	//			{
	//				StringCchPrintf( buf2 , 64 , TEXT("<C13>%d</C>") , EnchantDef ); // 보라색으로 
	//			}
	//			else
	//			{
	//				StringCchPrintf( buf2 , 64 , TEXT("%d") , EnchantDef );
	//			}

	//			StringCchPrintf( buf , 64 , pText , buf2 );

	//			StringCchCat(buffer, sizeof(buffer) ,buf);
	//		}
	//		break;
	//	}

	//}

	/////////////////////////////////////////////////////////////////////


	////cyj 기간 상태
	//SI32 siItemStatus = 0 ;

	//// 기간제
	//if ( pclitem->uiDateYear > 0 && pclitem->uiDateMonth > 0 && pclitem->uiDateDay > 0 && pclitem->uiDateUseDay > 0 )
	//{
	//	TCHAR usedate[128] = TEXT("") ;
	//	pText = m_pclGameDataManager->GetTxtFromMgr(5600) ;
	//	StringCchPrintf(usedate, 128, pText,pclitem->uiDateYear+2000,pclitem->uiDateMonth,pclitem->uiDateDay) ;
	//	StringCchCat(buffer, sizeof(buffer), usedate);

	//	NDate ndate;
	//	ndate.SetDate(pclitem->uiDateYear+2000,pclitem->uiDateMonth,pclitem->uiDateDay);
	//	ndate.AddDays(pclitem->uiDateUseDay);

	//	memset(usedate,0,128);
	//	pText = m_pclGameDataManager->GetTxtFromMgr(5601) ;
	//	StringCchPrintf(usedate, 128, pText,ndate.m_uiYear,ndate.m_uiMonth,ndate.m_uiDay);
	//	StringCchCat(buffer, sizeof(buffer), usedate);
	//}
	//else if ( pclitem->uiDateYear == 0 && pclitem->uiDateMonth == 0 && pclitem->uiDateDay == 0 && pclitem->uiDateUseDay > 0 &&
	//	IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_ITEMMALL) == true )
	//{
	//	TCHAR usedate[128] = TEXT("") ;
	//	pText = m_pclGameDataManager->GetTxtFromMgr(5658) ;
	//	StringCchPrintf(usedate, 128, pText,pclitem->uiDateUseDay) ;
	//	StringCchCat(buffer, sizeof(buffer), usedate);
	//}
	//else if ( pclitem->uiDateYear == 0 && pclitem->uiDateMonth == 0 && pclitem->uiDateDay == 0 && pclitem->uiDateUseDay > 0 &&
	//	IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_ITEMMALL) == false &&//아이템몰 상품이면 자동으로 받은 날짜 기록
	//	( GetItemAtb(pclitem->siUnique) & ITEMATB_OBJECT) 
	//	)
	//{

	//	TCHAR usedate[128] = TEXT("") ;
	//	if ( basicswitch )
	//	{
	//		// 고급품은 품질보증기간을 보여주지 않는다.
	//		if( pclitem->clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
	//		{
	//			pText = m_pclGameDataManager->GetTxtFromMgr(5615) ;
	//			StringCchPrintf(usedate, 128, pText,pclitem->uiDateUseDay);
	//			StringCchCat(buffer, sizeof(buffer), usedate);
	//		}
	//	}
	//	else
	//	{

	//		// cyj 목걸이는 밀봉풀기 전에 품질보증기간 보여주지 않는다.
	//		// LeeKH 고급품은 품질보증기간을 보여주지 않는다.
	//		if (type != ITEMTYPE_NECK &&
	//			pclitem->clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5)
	//		{
	//			pText = m_pclGameDataManager->GetTxtFromMgr(5615); // 품질보증기간 %d 
	//			StringCchPrintf(usedate, 128, pText,pclitem->uiDateUseDay);
	//			StringCchCat(buffer, sizeof(buffer), usedate);
	//		}


	//		pText = m_pclGameDataManager->GetTxtFromMgr(5895) ; // 상태 : 밀봉중

	//		if(bcolorswitch)
	//		{
	//			StringCchCat(buffer, sizeof(buffer), TEXT("<C08>"));
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//			StringCchCat(buffer, sizeof(buffer), TEXT("</C>"));
	//		}
	//		else
	//		{
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//		}
	//	}
	//}
	//else if ( pclitem->IsSealingUp(this) )
	//{
	//	if( !basicswitch)
	//	{
	//		pText = m_pclGameDataManager->GetTxtFromMgr(5895) ; // 상태 : 밀봉중

	//		if(bcolorswitch)
	//		{
	//			StringCchCat(buffer, sizeof(buffer), TEXT("<C08>"));
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//			StringCchCat(buffer, sizeof(buffer), TEXT("</C>"));
	//		}
	//		else
	//		{
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//		}
	//	}			
	//}

	//if ( pclitem->IsBelonging(this) )
	//{	
	//	pText = m_pclGameDataManager->GetTxtFromMgr(5898) ; // 상태 : 귀속
	//	StringCchCat(buffer, sizeof(buffer), pText);

	//}

	//// 기본무기 및 방어구일때
	//if ( type == ITEMTYPE_BOW    || type == ITEMTYPE_GUN    || type == ITEMTYPE_AXE   || 
	//	type == ITEMTYPE_STAFF  || type == ITEMTYPE_SPEAR  || type == ITEMTYPE_SWORD || 
	//	type == ITEMTYPE_ARMOUR || type == ITEMTYPE_HELMET || type == ITEMTYPE_BELT	)
	//{
	//	// 동양, 서양 전용의 장비 표시
	//	if ( pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_EAST) == true)
	//	{
	//		pText = m_pclGameDataManager->GetTxtFromMgr(6029);

	//		if ( bcolorswitch && pclitem->IsCorrectPrivateEquip(this, siKind) == false)
	//		{
	//			StringCchCat(buffer, sizeof(buffer), TEXT("<C08>"));
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//			StringCchCat(buffer, sizeof(buffer), TEXT("</C>"));
	//		}
	//		else
	//		{
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//		}
	//	}
	//	else if ( pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_WEST) == true)
	//	{
	//		pText = m_pclGameDataManager->GetTxtFromMgr(6030);

	//		if ( bcolorswitch && pclitem->IsCorrectPrivateEquip(this, siKind) == false)
	//		{
	//			StringCchCat(buffer, sizeof(buffer), TEXT("<C08>"));
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//			StringCchCat(buffer, sizeof(buffer), TEXT("</C>"));
	//		}
	//		else
	//		{
	//			StringCchCat(buffer, sizeof(buffer), pText);
	//		}

	//	}
	//	else 
	//	{
	//		pText = m_pclGameDataManager->GetTxtFromMgr(6028);
	//		StringCchCat(buffer, sizeof(buffer), pText);
	//	}
	//}


	//// [진성] 통합마켓 추가 후 제조자 이름 얻는 방식 교체. 
	//if (   (pclitem->GetRareType() == RARELEVEL_1)	  // 명품
	//	|| (pclitem->GetRareType() == RARELEVEL_2)	  // 호품
	//	|| (pclitem->GetRareType() == RARELEVEL_5)	) // 신기
	//{
	//	static TCHAR tmpbuf[256]  = {'\0'};
	//	static TCHAR tmpbuf2[256] = {'\0'};

	//	// 제조자 퍼슨 아이디가 아이템에 박혀 있다면!
	//	if( pclitem->siMakerPersonID > 0 )
	//	{
	//		TCHAR name[MAX_PLAYER_NAME];

	//		if (GetMadePersonName(pclitem->siMakerPersonID, name, MAX_PLAYER_NAME))
	//		{
	//			pText = m_pclGameDataManager->GetTxtFromMgr(5894);

	//			if(bcolorswitch)
	//			{
	//				StringCchPrintf(tmpbuf2,sizeof(tmpbuf2),TEXT("%s%s%s"),TEXT("<C05>"),name,TEXT("</C>") );

	//				StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, tmpbuf2);
	//			}
	//			else
	//			{
	//				StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, name);
	//			}

	//			StringCchCat(buffer, sizeof(buffer), tmpbuf);
	//		}
	//	}
	//	else
	//	{
	//		cltPersonNameInfo clPersonInfo;
	//		if( GetMadePersonNameNew( pclitem->siSID, &clPersonInfo, MAX_PLAYER_NAME ) )
	//		{
	//			// [영훈] '[NONAME]' 이라고 찍힌것은 표시되지 않아야 하는 이름임
	//			if ( _tcscmp(clPersonInfo.szPersonName, TEXT("[NONAME]")) != 0 )
	//			{
	//				pText = m_pclGameDataManager->GetTxtFromMgr(5894);

	//				if(bcolorswitch)
	//				{
	//					StringCchPrintf(tmpbuf2, sizeof(tmpbuf2), TEXT("%s%s%s"), TEXT("<C05>"), clPersonInfo.szPersonName, TEXT("</C>") );

	//					StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, tmpbuf2);
	//				}
	//				else
	//				{
	//					StringCchPrintf(tmpbuf, sizeof(tmpbuf), pText, clPersonInfo.szPersonName);
	//				}

	//				StringCchCat(buffer, sizeof(buffer), tmpbuf);
	//			}
	//		}
	//	}
	//}



	////-------------------------------------
	//// 무역 가능 여부
	////--------------------------------------
	//if (pclClient->siServiceArea == ConstServiceArea_English)
	//{	
	//	if ( pclItemInfo[ref]->siTradeServerMinNum > 0 )
	//	{
	//		TCHAR *pText = m_pclGameDataManager->GetTxtFromMgr(6201);
	//		StringCchCat(buffer, sizeof(buffer), pText);
	//	}
	//	else
	//	{
	//		TCHAR *pText = m_pclGameDataManager->GetTxtFromMgr(6202);
	//		StringCchCat(buffer, sizeof(buffer), pText);
	//	}
	//}

	////특산품 표시
	//if (IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_SPECIAL_GLOBAL))
	//{
	//	TCHAR* pText = m_pclGameDataManager->GetTxtFromMgr(6645);
	//	StringCchCat(buffer, sizeof(buffer), pText);
	//}
	//if (IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_SPECIAL_JAPAN))
	//{
	//	TCHAR* pText = m_pclGameDataManager->GetTxtFromMgr(6644);
	//	StringCchCat(buffer, sizeof(buffer), pText);
	//}
	//if (IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_SPECIAL_KOREA))
	//{
	//	TCHAR* pText = m_pclGameDataManager->GetTxtFromMgr(6643);
	//	StringCchCat(buffer, sizeof(buffer), pText);
	//}

	////KHY - 0227 - 결정체 (크리스탈) 제조 추가.
	////박기형 - 2009615 - 아이템에 일반 결정체 조합 와 아바타 결정체 조합을 구분 
	//TCHAR usedCrystalNum[128] = TEXT("") ;
	//if( pclitem->GetUseCrystalNum() > 0) //슬롯이 있다.  = 슬롯을 가질 수 있는 아이템이다.
	//{

	//	switch(type)
	//	{
	//	case ITEMTYPE_AVATAR_MANTLE:
	//	case ITEMTYPE_AVATAR_DRESS:
	//	case ITEMTYPE_AVATAR_HAT:
	//	case ITEMTYPE_AVATAR_NECK:
	//	case ITEMTYPE_AVATAR_RING:
	//		{
	//			// 아바타 결정체 강화 관련 설명 처리
	//			GetExplainEnhanceAvatarItem(buffer,ConstEntireTmpBufSize,type,pclitem);
	//			break;
	//		}
	//	default:
	//		{
	//			TCHAR* pText = m_pclGameDataManager->GetTxtFromMgr(7999);

	//			TCHAR buf1[50] = TEXT("") ;

	//			StringCchPrintf(buf1, 50,pText,pclitem->GetUseCrystalNum());

	//			if(bcolorswitch)
	//				StringCchPrintf(usedCrystalNum, 1024, TEXT("%s%s%s"),TEXT("<C05>"),buf1,TEXT("</C>"));
	//			else
	//				StringCchPrintf(usedCrystalNum, 1024, TEXT("%s"),buf1);					

	//			StringCchCat(buffer, sizeof(buffer), usedCrystalNum);

	//			for(int i= 0; i < MAX_CRYSTAL_NUM;i++)
	//			{
	//				SI16 cAbliltyType = pclitem->cltItemCrytal[i].GetCrystalAblilty();
	//				SI16 cAbliltyTypeNum = pclitem->cltItemCrytal[i].GetCrystalAbilityNum();

	//				if(cAbliltyType>0) // 출력해줘야 하는 능력이 있다.
	//				{			
	//					TCHAR buf[64] = TEXT("") ;
	//					TCHAR buf2[128] = TEXT("") ;
	//					TCHAR CrystalReturnString[50] = TEXT("") ;

	//					SI16 nowDub = (SI16)((pclitem->cltItemCrytal[i].GetDurability()*100) / 30000); // 최대 내구도 값 - 30000 고정.
	//					SI16 MaxDub = (SI16)((pclitem->cltItemCrytal[i].GetMaxDurability()*100) /30000);

	//					GetCrystalAbilityTypeName(buf, cAbliltyType,cAbliltyTypeNum, CrystalReturnString);

	//					if(bcolorswitch)
	//					{
	//						StringCchPrintf(buf,64,TEXT("%s%d%s%d%s"),TEXT("<C05>["), nowDub ,TEXT("/"),MaxDub, TEXT("]") ); // 파란색. 
	//						StringCchPrintf(buf2,128,TEXT("%s%s"),CrystalReturnString, TEXT("\r </C>"));							
	//					}
	//					else
	//					{
	//						StringCchPrintf(buf,64,TEXT("%s%d%s%d%s"),TEXT("["), nowDub ,TEXT("/"),MaxDub, TEXT("]") );  
	//						StringCchPrintf(buf2,128,TEXT("%s%s"),CrystalReturnString,TEXT("\r "));							
	//					}								
	//					StringCchPrintf(temp,1024,TEXT("%s %s"),buf , buf2);	
	//					StringCchCat(buffer, 1024 ,temp);
	//				}
	//			}		

	//			break;
	//		}
	//	}


	//}
	return TRUE;
}

// 음식 아이템을 추가한다. 
void cltItemManagerClient::AddFoodItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;
	
	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		
	
	if (!bLoadFile)
	{
		return;
	}

	SI32 unique			= 0;
	TCHAR szname[60]	= {'\0'};
	TCHAR code[24]		= {'\0'};
	TCHAR iconcode[24]	= {'\0'};
	SI32 showlist		= 0;

	SI32 fieldimg		= 0;
	TCHAR itemmall[128] = {'\0'};
	SI32 weight			= 0;
	SI32 skilllevel		= 0;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt		= 0;		// 기본 생산 확률. (퍼밀)

	SI32 incpbt			= 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd			= 0;
	SI32 hungry			= 0;
	SI32 horse			= 0;
	SI32 org1			= 0;

	SI32 org2			= 0;
	SI32 org3			= 0;
	SI32 org4			= 0;
	SI32 productunit	= 0;
	TCHAR szmaterial[128] =	{'\0'};

	TCHAR sztool[128]	= {'\0'};
	SI32 siExplain		= 0;
	SI32 Grade			= 0;
	SI32 MinNum			= 0;
	SI32 SuperMaket		= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		// datatype,	pVar,	size	 
		NDATA_INT32,	&unique,	  4,		
			NDATA_MBSTRING,	szname,		  128,
			NDATA_MBSTRING,	code,		  128,
			NDATA_MBSTRING,	iconcode,	  128,
			NDATA_INT32,	&showlist,	  4,

			NDATA_INT32,	&fieldimg,	  4,
			NDATA_MBSTRING,	itemmall,	  128,
			NDATA_INT32,	&weight,	  4,
			NDATA_INT32,	&skilllevel,  4,
			NDATA_INT32,	&makepbt,	  4,

			NDATA_INT32,	&incpbt,	  4,
			NDATA_INT32,	&siHnd,		  4,
			NDATA_INT32,	&hungry,	  4,
			NDATA_INT32,	&horse,		  4,
			NDATA_INT32,	&org1,		  4,

			NDATA_INT32,	&org2,		  4,
			NDATA_INT32,	&org3,		  4,
			NDATA_INT32,	&org4,		  4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial,	  128,

			NDATA_MBSTRING,	sztool,		  128,
			NDATA_INT32,	&siExplain,   4,
			NDATA_INT32,	&MinNum,	  4,
			NDATA_INT32,	&SuperMaket,  4,
			0, 0, 0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_FOOD;
			}
		}
	}
}

// 수산물 아이템을 추가한다. 
void cltItemManagerClient::AddSeaFoodItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique = 0;
	TCHAR szname[60]	= {'\0'};
	TCHAR code[24]		= {'\0'};
	TCHAR iconcode[24]	= {'\0'};
	SI32 showlist = 0;
	SI32 fieldimg = 0;
	SI32 weight = 0;
	SI32 skilllevel = 0;	// 사용제한  스킬 레벨 
	SI32 makepbt = 0;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt = 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd = 0;
	SI32 hungry = 0;
	SI32 horse = 0;
	SI32 FishingBait = 0;
	SI32 org1 = 0;
	SI32 org2 = 0;
	SI32 org3 = 0;
	SI32 org4 = 0;
	SI32 productunit = 0;
	TCHAR szmaterial[128]=	{'\0'};
	TCHAR sztool[128]	=	{'\0'};
	SI32 explain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&fieldimg,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&hungry,	4,
			NDATA_INT32,	&horse,		4,
			NDATA_INT32,	&FishingBait,	4,
			NDATA_INT32,	&org1,		4,
			NDATA_INT32,	&org2,		4,
			NDATA_INT32,	&org3,		4,
			NDATA_INT32,	&org4,		4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&explain,	4,
			NDATA_INT32,	&MinNum,  4,
			NDATA_INT32,	&NewMarket,  4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_SEAFOOD;
			}
		}
	}

}


// 소환인형 아이템을 추가한다. 
void cltItemManagerClient::AddDollItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[128];
	SI32 weight;
	SI32 sikind;
	SI32 Grade = 0;
	SI32 productunit = 0;
	TCHAR szmaterial[128]=	{'\0'};
	TCHAR sztool[128]	=	{'\0'};
	SI32 MinNum = 0;
	SI32 explain	 = 0;
	SI32 NewMarket	 = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_MBSTRING,	itemmall,	128,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&sikind,	4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&MinNum,  4,
			NDATA_INT32,	&explain, 4,
			NDATA_INT32,	&NewMarket, 4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_DOLL;
			}
		}
	}
}


// 표아이템을 추가한다. 
void cltItemManagerClient::AddTicketItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;


	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[128];
	SI32 weight;
	SI32 explain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_MBSTRING,	&itemmall,	128,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&explain,	4,
			NDATA_INT32,	&MinNum,  4,
			NDATA_INT32,	&NewMarket,  4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_TICKET;
			}
		}
	}

}


// 약 아이템을 추가한다. 
void cltItemManagerClient::AddHealItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[128];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀)
	SI32 siHnd;
	TCHAR adapt[32];
	SI32 life, mana;
	//TCHAR szdisease[64];
	TCHAR szdiseaseType[64]; // szdisease[64] 를 szdiseaseType[64] 으로 변경.
	SI32 org1;
	SI32 org2;
	SI32 org3;
	SI32 org4;
	TCHAR szeft[64];
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING, itemmall,	128,
			NDATA_INT32, &weight,		4,
			NDATA_INT32, &skilllevel,	4,
			NDATA_INT32, &makepbt,		4,
			NDATA_INT32, &incpbt,		4,
			NDATA_INT32, &siHnd,		4,
			NDATA_MBSTRING,	adapt,		128,
			NDATA_INT32, &life,		4,
			NDATA_INT32, &mana,		4,
			NDATA_MBSTRING,	szdiseaseType,	128,
			NDATA_INT32, &org1,		4,		
			NDATA_INT32,	&org2,		  4,
			NDATA_INT32,	&org3,		  4,
			NDATA_INT32,	&org4,		  4,	
			NDATA_MBSTRING,	szeft,	128,
			NDATA_INT32, &productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,	128,
			NDATA_INT32, &explain, 4,
			NDATA_INT32,	&MinNum,  4,
			NDATA_INT32,	&NewMarket,  4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_HEAL;
			}
		}
	}

}


// 칼 아이템을 추가한다. 
void cltItemManagerClient::AddSwordItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	TCHAR armcode[12];
	SI32 showlist;
	SI32 fieldimg;
	SI32 AttackDelay;		// 공격 간격 시간
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 productunit;
	TCHAR szmaterial[128]	=	TEXT("");
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&fieldimg,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage,	4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&siExplain,	4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true) )
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_SWORD;
			}
		}
	}

}

// 창 아이템을 추가한다. 
void cltItemManagerClient::AddSpearItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[20];
	TCHAR iconcode[20];
	TCHAR armcode[12];
	SI32 showlist;
	SI32 AttackDelay;		// 공격 간격 시간
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨 
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage,	4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32	,	&siExplain,	4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_SPEAR;
			}
		}
	}

}


// 지팡이 아이템을 추가한다. 
void cltItemManagerClient::AddStaffItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	TCHAR armcode[24];
	SI32 showlist;
	SI32 AttackDelay;		// 공격 간격 시간
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨 
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 mag;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage,	4,
			NDATA_INT32,	&mag,		4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&siExplain,	4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_STAFF;
			}
		}
	}

}

// 도끼 아이템을 추가한다. 
void cltItemManagerClient::AddAxeItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[60];
	TCHAR iconcode[20];
	TCHAR armcode[60];
	SI32 showlist;
	SI32 AttackDelay;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨 
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 hitrate;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage,	4,
			NDATA_INT32,	&hitrate,	4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&siExplain,	4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AXE;
			}
		}
	}
}


// 활 아이템을 추가한다. 
void cltItemManagerClient::AddBowItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[20];
	TCHAR armcode[12];
	SI32 ShowList;
	SI32 AttackDelay;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨 
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;	
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 range;
	SI32 hitrate;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&ShowList,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage,	4,
			NDATA_INT32,	&range,		4,
			NDATA_INT32,	&hitrate,	4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,	128,
			NDATA_MBSTRING,	sztool,		128,
			NDATA_INT32,	&siExplain,	4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_BOW;
			}
		}
	}
}

// 총 아이템을 추가한다. 
void cltItemManagerClient::AddGunItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[20];
	TCHAR armcode[12];
	SI32 showlist;
	SI32 AttackDelay;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨 
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level;
	SI32 condition;	
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage, maxdamage;
	SI32 range;
	SI32 hitrate;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{	
		NDATA_INT32,	&unique,	4,		
			NDATA_MBSTRING,	szname,		128,
			NDATA_MBSTRING,	code,		128,
			NDATA_MBSTRING,	iconcode,	128,
			NDATA_MBSTRING,	armcode,	128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&AttackDelay,	4,
			NDATA_INT32,	&weight,	4,
			NDATA_INT32,	&skilllevel,	4,
			NDATA_INT32,	&MountSkillPoint,	4,
			NDATA_INT32,	&makepbt,	4,
			NDATA_INT32,	&incpbt,	4,
			NDATA_INT32,	&level,		4,
			NDATA_INT32,	&condition,	4,
			NDATA_INT32,	&siHnd,		4,
			NDATA_INT32,	&durability,	4,
			NDATA_INT32,	&usedate,	4,
			NDATA_INT32,	&mindamage,	4,
			NDATA_INT32,	&maxdamage, 4,
			NDATA_INT32,	&range, 4,
			NDATA_INT32,	&hitrate, 4,
			NDATA_INT32,	&productunit,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain,4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_GUN;
			}
		}
	}
}

// 갑옷 아이템을 추가한다. 
void cltItemManagerClient::AddArmourItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[12];
	TCHAR iconcode[24];
	TCHAR armcode[12];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siLevel;
	SI32 siStr;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	TCHAR szincskilltype[64];
	SI32 incskillamount;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 DefenE;
	SI32 DefenR;
	SI32 Grade;
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siLevel, 4,
			NDATA_INT32,	&siStr, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac, 4,
			NDATA_MBSTRING,	szincskilltype,128,
			NDATA_INT32,	&incskillamount,4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain,  4,
			NDATA_INT32,	&DefenE,	4,
			NDATA_INT32,	&DefenR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_ARMOUR;
			}
		}
	}
}


// 의복  아이템을 추가한다. 
void cltItemManagerClient::AddDressItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}
	SI32	unique					= 0;
	TCHAR	szname[60]				= TEXT("");
	TCHAR	code[12]				= TEXT("");
	TCHAR	iconcode[24]			= TEXT("");
	TCHAR	armcode[12]				= TEXT("");
	SI32	showlist				= 0;
	TCHAR	itemmall[256]			= TEXT("");
	SI32	weight					= 0;
	SI32	skilllevel				= 0;	// 생산에 필요한 스킬 레벨 
	SI32	makepbt					= 0;		// 기본 생산 확률. (퍼밀) 
	SI32	incpbt					= 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32	condition				= 0;
	SI32	siHnd					= 0;
	SI32	child					= 0;
	SI32	needrank				= 0;
	SI32	durability				= 0;
	//	SI32 ac;
	//SI32 luck;
	TCHAR	szIncSkillUnique[128]	= TEXT("");
	TCHAR	szincskilltype[128]		= TEXT("");
	SI32	incskillamount			= 0;
	//	UI32 sistr;
	//	UI32 sidex;
	//	UI32 sivit;
	//	UI32 simag;
	//	UI32 siluk;
	SI32	decdur					= 0;
	SI32	man						= 0;
	SI32	woman					= 0;
	SI32	productunit				= 0;
	TCHAR	szmaterial[128]			= TEXT("");
	TCHAR	sztool[128]				= TEXT("");
	SI32	siExplain				= 0;
	bool	bMan					= false;
	bool	bWoman					= false;
	SI32	usedate					= 0;
	SI32	DefenE					= 0;
	SI32	DefenR					= 0;
	SI32	Grade					= 0;
	SI32	MinNum					= 0;
	SI32	NewMarket				= 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,			4,		
			NDATA_MBSTRING,	szname,				128,
			NDATA_MBSTRING,	code,				128,
			NDATA_MBSTRING,	iconcode,			128,
			NDATA_MBSTRING,	armcode,			128,
			NDATA_INT32,	&showlist,			4,
			NDATA_MBSTRING,	itemmall,			256,
			NDATA_INT32,	&weight,			4,
			NDATA_INT32,	&skilllevel,		4,
			NDATA_INT32,	&makepbt,			4,
			NDATA_INT32,	&incpbt,			4,
			NDATA_INT32,	&condition,			4,
			NDATA_INT32,	&siHnd,				4,
			NDATA_INT32,	&child,				4,
			NDATA_INT32,	&needrank,			4,
			NDATA_INT32,	&durability,		4,
			NDATA_INT32,	&usedate,			4,
			//NDATA_INT32,	&ac, 4,
			//NDATA_INT32,	&luck, 4,
			NDATA_MBSTRING,	szincskilltype,		128,
			NDATA_MBSTRING,	szIncSkillUnique,	128,
			NDATA_INT32,	&incskillamount,	4,
			//NDATA_INT32,	&sistr,4,
			//NDATA_INT32,	&sidex,4,
			//NDATA_INT32,	&sivit,4,
			//NDATA_INT32,	&simag,4,
			//NDATA_INT32,	&siluk,4,
			NDATA_INT32,	&decdur,			4,
			NDATA_INT32,	&man,				4,
			NDATA_INT32,	&woman,				4,
			NDATA_INT32,	&productunit,		4,
			NDATA_MBSTRING,	szmaterial,			128,
			NDATA_MBSTRING,	sztool,				128,
			NDATA_INT32,	&siExplain,			4,
			//NDATA_INT32,	&DefenE,	4,
			//NDATA_INT32,	&DefenR,	4,
			//NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,			4,
			NDATA_INT32,	&NewMarket,			4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_DRESS;
			}
		}
	}
}

/////////////////
// 아바타 의복 아이템을 추가한다. 
void cltItemManagerClient::AddAvatarDressItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}
	SI32	unique					= 0;
	TCHAR	szname[60]				= TEXT("");
	TCHAR	code[12]				= TEXT("");
	TCHAR	iconcode[24]			= TEXT("");
	TCHAR	armcode[12]				= TEXT("");
	SI32	showlist				= 0;
	TCHAR	itemmall[256]			= TEXT("");
	SI32	weight					= 0;
	SI32	skilllevel				= 0;	// 생산에 필요한 스킬 레벨 
	SI32	makepbt					= 0;		// 기본 생산 확률. (퍼밀) 
	SI32	incpbt					= 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32	condition				= 0;
	SI32	siHnd					= 0;
	SI32	child					= 0;
	SI32	needrank				= 0;
	SI32	durability				= 0;
	//	SI32 ac;
	//SI32 luck;
	TCHAR	szIncSkillUnique[128]	= TEXT("");
	TCHAR	szincskilltype[128]		= TEXT("");
	SI32	incskillamount			= 0;
	//	UI32 sistr;
	//	UI32 sidex;
	//	UI32 sivit;
	//	UI32 simag;
	//	UI32 siluk;
	SI32	decdur					= 0;
	SI32	man						= 0;
	SI32	woman					= 0;
	SI32	productunit				= 0;
	TCHAR	szmaterial[128]			= TEXT("");
	TCHAR	sztool[128]				= TEXT("");
	SI32	siExplain				= 0;
	bool	bMan					= false;
	bool	bWoman					= false;
	SI32	usedate					= 0;
	//SI32	DefenE					= 0;
	//SI32	DefenR					= 0;
	//SI32	Grade					= 0;
	SI32	MinNum					= 0;
	SI32	NewMarket				= 0;
	SI32	IncSkillAmount			= 0; // 스킬 증가치


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,			4,		
			NDATA_MBSTRING,	szname,				128,
			NDATA_MBSTRING,	code,				128,
			NDATA_MBSTRING,	iconcode,			128,
			NDATA_MBSTRING,	armcode,			128,
			NDATA_INT32,	&showlist,			4,
			NDATA_MBSTRING,	itemmall,			256,
			NDATA_INT32,	&weight,			4,
			NDATA_INT32,	&skilllevel,		4,
			NDATA_INT32,	&makepbt,			4,
			NDATA_INT32,	&incpbt,			4,
			NDATA_INT32,	&condition,			4,
			NDATA_INT32,	&siHnd,				4,
			NDATA_INT32,	&child,				4,
			NDATA_INT32,	&needrank,			4,
			NDATA_INT32,	&durability,		4,
			NDATA_INT32,	&usedate,			4,
			//NDATA_INT32,	&ac, 4,
			//NDATA_INT32,	&luck, 4,
			NDATA_MBSTRING,	szincskilltype,		128,
			NDATA_MBSTRING,	szIncSkillUnique,	128,
			NDATA_INT32,	&incskillamount,	4,
			//NDATA_INT32,	&sistr,4,
			//NDATA_INT32,	&sidex,4,
			//NDATA_INT32,	&sivit,4,
			//NDATA_INT32,	&simag,4,
			//NDATA_INT32,	&siluk,4,
			NDATA_INT32,	&decdur,			4,
			NDATA_INT32,	&man,				4,
			NDATA_INT32,	&woman,				4,
			NDATA_INT32,	&productunit,		4,
			NDATA_MBSTRING,	szmaterial,			128,
			NDATA_MBSTRING,	sztool,				128,
			NDATA_INT32,	&siExplain,			4,
			//NDATA_INT32,	&DefenE,	4,
			//NDATA_INT32,	&DefenR,	4,
			//NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,			4,
			NDATA_INT32,	&NewMarket,			4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AVATAR_DRESS;
			}
		}
	}
}
////////////////

// 모자  아이템을 추가한다. 
void cltItemManagerClient::AddHatItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32	unique					= 0;
	TCHAR	szname[60]				= TEXT("");
	TCHAR	code[12]				= TEXT("");
	TCHAR	iconcode[24]			= TEXT("");
	TCHAR	armcode[12]				= TEXT("");
	SI32	showlist				= 0;
	TCHAR	itemmall[256]			= TEXT("");
	SI32	weight					= 0;
	SI32	skilllevel				= 0;		// 생산에 필요한 스킬 레벨 
	SI32	makepbt					= 0;		// 기본 생산 확률. (퍼밀) 
	SI32	incpbt					= 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32	condition				= 0;
	SI32	siHnd					= 0;
	SI32	child					= 0;
	SI32	needrank				= 0;
	SI32	durability				= 0;
	//SI32 ac;
	//SI32 luck;
	TCHAR	szIncSkillUnique[128]	= TEXT("");
	TCHAR	szincskilltype[128]		= TEXT("");
	SI32	incskillamount			= 0;
	//UI32 sistr;
	//UI32 sidex;
	//UI32 sivit;
	//UI32 simag;
	//UI32 siluk;

	SI32	decdur					= 0;
	SI32	man						= 0;
	SI32	woman					= 0;
	SI32	productunit				= 0;
	TCHAR	szmaterial[128]			= TEXT("");
	TCHAR	sztool[128]				= TEXT("");
	SI32	siExplain				= 0;
	bool	bMan					= false;
	bool	bWoman					= false;
	SI32	usedate					= 0;
	SI32	DefenE					= 0;
	SI32	DefenR					= 0;
	SI32	Grade					= 0;
	SI32	MinNum					= 0;
	SI32	NewMarket				= 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,			4,		
			NDATA_MBSTRING,	szname,				128,
			NDATA_MBSTRING,	code,				128,
			NDATA_MBSTRING,	iconcode,			128,
			NDATA_MBSTRING,	armcode,			128,
			NDATA_INT32,	&showlist,			4,
			NDATA_MBSTRING,	itemmall,			256,
			NDATA_INT32,	&weight,			4,
			NDATA_INT32,	&skilllevel,		4,
			NDATA_INT32,	&makepbt,			4,
			NDATA_INT32,	&incpbt,			4,
			NDATA_INT32,	&condition,			4,
			NDATA_INT32,	&siHnd,				4,
			NDATA_INT32,	&child,				4,
			NDATA_INT32,	&needrank,			4,
			NDATA_INT32,	&durability,		4,
			NDATA_INT32,	&usedate,			4,
			//NDATA_INT32,	&ac, 4,
			//NDATA_INT32,	&luck, 4,
			NDATA_MBSTRING,	szincskilltype,		128,
			NDATA_MBSTRING,	szIncSkillUnique,	128,
			NDATA_INT32,	&incskillamount,	4,
			//NDATA_INT32,	&sistr,4,
			//NDATA_INT32,	&sidex,4,
			//NDATA_INT32,	&sivit,4,
			//NDATA_INT32,	&simag,4,
			//NDATA_INT32,	&siluk,4,
			NDATA_INT32,	&decdur,			4,
			NDATA_INT32,	&man,				4,
			NDATA_INT32,	&woman,				4,
			NDATA_INT32,	&productunit,		4,
			NDATA_MBSTRING,	szmaterial,			128,
			NDATA_MBSTRING,	sztool,				128,
			NDATA_INT32,	&siExplain,			4,
			NDATA_INT32,	&DefenE,			4,
			NDATA_INT32,	&DefenR,			4,
			NDATA_INT32,	&Grade,				4,
			NDATA_INT32,	&MinNum,			4,
			NDATA_INT32,	&NewMarket,			4,

			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_HAT;
			}
		}
	}
}

///////////
// 아바타 모자 아이템을 추가한다. 
void cltItemManagerClient::AddAvatarHatItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32	unique					= 0;
	TCHAR	szname[60]				= TEXT("");
	TCHAR	code[12]				= TEXT("");
	TCHAR	iconcode[24]			= TEXT("");
	TCHAR	armcode[12]				= TEXT("");
	SI32	showlist				= 0;
	TCHAR	itemmall[256]			= TEXT("");
	SI32	weight					= 0;
	SI32	skilllevel				= 0;		// 생산에 필요한 스킬 레벨 
	SI32	makepbt					= 0;		// 기본 생산 확률. (퍼밀) 
	SI32	incpbt					= 0;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32	condition				= 0;
	SI32	siHnd					= 0;
	SI32	child					= 0;
	SI32	needrank				= 0;
	SI32	durability				= 0;
	//SI32 ac;
	//SI32 luck;
	TCHAR	szIncSkillUnique[128]	= TEXT("");
	TCHAR	szincskilltype[128]		= TEXT("");
	SI32	incskillamount			= 0;
	//UI32 sistr;
	//UI32 sidex;
	//UI32 sivit;
	//UI32 simag;
	//UI32 siluk;

	SI32	decdur					= 0;
	SI32	man						= 0;
	SI32	woman					= 0;
	SI32	productunit				= 0;
	TCHAR	szmaterial[128]			= TEXT("");
	TCHAR	sztool[128]				= TEXT("");
	SI32	siExplain				= 0;
	bool	bMan					= false;
	bool	bWoman					= false;
	SI32	usedate					= 0;
	//SI32	DefenE					= 0;
	//SI32	DefenR					= 0;
	//SI32	Grade					= 0;
	SI32	MinNum					= 0;
	SI32	NewMarket				= 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,			4,		
			NDATA_MBSTRING,	szname,				128,
			NDATA_MBSTRING,	code,				128,
			NDATA_MBSTRING,	iconcode,			128,
			NDATA_MBSTRING,	armcode,			128,
			NDATA_INT32,	&showlist,			4,
			NDATA_MBSTRING,	itemmall,			256,
			NDATA_INT32,	&weight,			4,
			NDATA_INT32,	&skilllevel,		4,
			NDATA_INT32,	&makepbt,			4,
			NDATA_INT32,	&incpbt,			4,
			NDATA_INT32,	&condition,			4,
			NDATA_INT32,	&siHnd,				4,
			NDATA_INT32,	&child,				4,
			NDATA_INT32,	&needrank,			4,
			NDATA_INT32,	&durability,		4,
			NDATA_INT32,	&usedate,			4,
			//NDATA_INT32,	&ac, 4,
			//NDATA_INT32,	&luck, 4,
			NDATA_MBSTRING,	szincskilltype,		128,
			NDATA_MBSTRING,	szIncSkillUnique,	128,
			NDATA_INT32,	&incskillamount,	4,
			//NDATA_INT32,	&sistr,4,
			//NDATA_INT32,	&sidex,4,
			//NDATA_INT32,	&sivit,4,
			//NDATA_INT32,	&simag,4,
			//NDATA_INT32,	&siluk,4,
			NDATA_INT32,	&decdur,			4,
			NDATA_INT32,	&man,				4,
			NDATA_INT32,	&woman,				4,
			NDATA_INT32,	&productunit,		4,
			NDATA_MBSTRING,	szmaterial,			128,
			NDATA_MBSTRING,	sztool,				128,
			NDATA_INT32,	&siExplain,			4,
			//NDATA_INT32,	&DefenE,			4,
			//NDATA_INT32,	&DefenR,			4,
			//NDATA_INT32,	&Grade,				4,
			NDATA_INT32,	&MinNum,			4,
			NDATA_INT32,	&NewMarket,			4,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AVATAR_HAT;
			}
		}
	}
}

///////////

// 투구 아이템을 추가한다. 
void cltItemManagerClient::AddHelmetItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[12];
	TCHAR iconcode[24];
	TCHAR armcode[12];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siStr;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	TCHAR szincskilltype[64];
	SI32 incskillamount;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 DefenE;
	SI32 DefenR;
	SI32 Grade;
	SI32 NewMarket;

	TCHAR szCanUseType[256];
	SI32 MinNum = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt,4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siStr, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac, 128,
			NDATA_MBSTRING,	szincskilltype, 4,
			NDATA_INT32,	&incskillamount,4,
			NDATA_INT32,	&productunit,  4,
			NDATA_INT32,	&SetItemIndex,128,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain,  4,
			NDATA_INT32,	&DefenE,	4,
			NDATA_INT32,	&DefenR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_HELMET;
			}
		}
	}

}

// 요대 아이템을 추가한다. 
void cltItemManagerClient::AddBeltItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	SI32 showlist;
	TCHAR iconcode[24];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siStr;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 DefenE;
	SI32 DefenR;
	SI32 Grade; 
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,			
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt,4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siStr, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&DefenE,	4,
			NDATA_INT32,	&DefenR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_BELT;
			}
		}
	}
}



// 목걸이 아이템을 추가한다. 
void cltItemManagerClient::AddNeckItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	TCHAR itemmall[256];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siFameLevelCondition;
	SI32 siHnd;
	SI32 durability;		
	SI32 usedate;
	SI32 maxusedate;
	TCHAR szIncSkillUnique[128];
	SI32 siSkillPoint;
	SI32 siMaxSkillPoint;
	SI32 decdur;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 MinNum = 0;
	SI32 Grade = 0;
	SI32 immutableUseDate = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,			
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siFameLevelCondition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate,4,
			NDATA_INT32,	&maxusedate,4,
			NDATA_MBSTRING,	szIncSkillUnique, 128,
			NDATA_INT32,	&siSkillPoint, 4,
			NDATA_INT32,	&siMaxSkillPoint, 4,
			NDATA_INT32,	&decdur, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&immutableUseDate,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_NECK;
			}
		}
	}
}

// 아바타 목걸이 아이템을 추가한다. 
void cltItemManagerClient::AddAvatarNeckItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	TCHAR itemmall[256];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀)
	SI32 condition;		// 필요 레벨
	//SI32 siFameLevelCondition;
	SI32 siHnd;
	SI32 durability;		
	SI32 usedate;
	SI32 maxusedate;
	TCHAR szIncSkillUnique[128];
	SI32 siSkillPoint;
	SI32 siMaxSkillPoint;
	SI32 decdur;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 MinNum = 0;
	SI32 Grade = 0;
	SI32 immutableUseDate = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,			
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,	   // [지연] 착용 필요 레벨
			//NDATA_INT32,	&siFameLevelCondition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate,4,
			NDATA_INT32,	&maxusedate,4,
			NDATA_MBSTRING,	szIncSkillUnique, 128,
			NDATA_INT32,	&siSkillPoint, 4,
			NDATA_INT32,	&siMaxSkillPoint, 4,
			NDATA_INT32,	&decdur, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&immutableUseDate,		4,
			NDATA_INT32,	&NewMarket,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AVATAR_NECK;
			}
		}
	}
}

// 신발아이템을 추가한다. 
void cltItemManagerClient::AddShoesItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}
	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	SI32 movespeed;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 DefenE;
	SI32 DefenR;
	SI32 Grade;	
	TCHAR szCanUseType[256];
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac, 4,
			NDATA_INT32,	&movespeed, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain , 4,
			NDATA_INT32,	&DefenE,	4,
			NDATA_INT32,	&DefenR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_MBSTRING,	szCanUseType,	256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_SHOES;
			}
		}
	}


}

// 책 아이템을 추가한다. 
void cltItemManagerClient::AddBookItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 skill;
	SI32 skilllevellimit;
	SI32 siHnd;

	SI32 ConLevel = 0;			// 레벨	  제약 조건
	SI32 ConStr = 0;			// 근력	  제약 조건
	SI32 ConDex = 0;			// 순발력 제약 조건
	SI32 ConHnd = 0;			// 손재주 제약 조건
	SI32 ConInt = 0;			// 마법력 제약 조건
	SI32 ConVit = 0;			// 체력   제약 조건

	SI32 makepbt = 0;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt = 0;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 level = 0;

	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skill, 4,
			NDATA_INT32,	&skilllevellimit, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&ConStr, 4,
			NDATA_INT32,	&ConDex, 4,
			NDATA_INT32,	&ConHnd, 4,
			NDATA_INT32,	&ConInt, 4,
			NDATA_INT32,	&ConVit, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&level, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	// 제한 레벨 추가 - By LEEKH 2007.07.30
	NDataTypeInfo pDataTypeInfo_ManufactureRevitalize[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skill, 4,
			NDATA_INT32,	&skilllevellimit, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&ConLevel, 4,
			NDATA_INT32,	&ConStr, 4,
			NDATA_INT32,	&ConDex, 4,
			NDATA_INT32,	&ConHnd, 4,
			NDATA_INT32,	&ConInt, 4,
			NDATA_INT32,	&ConVit, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&level, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo_ManufactureRevitalize);

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true) )
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_BOOK;
			}
		}
	}
}

// 동물 아이템을 추가한다. 
void cltItemManagerClient::AddAnimalItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32	showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 grownup = 0;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_ANIMAL;
			}
		}
	}
}


// 문구 아이템을 추가한다. 
void cltItemManagerClient::AddPaperItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain;
	SI32 Grade = 0;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist,	4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&MountSkillPoint, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&productunit,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_PAPER;
			}
		}
	}

}


// 일반 아이템을 추가한다. 
void cltItemManagerClient::AddItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];	
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = itemtype;
			}
		}
	}

}

// 기타 아이템을 추가한다. 
void cltItemManagerClient::AddEtcItem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 grownup = 0;			// 성인 전용 아이템 - 군주 스페셜에서 사용
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 usedate;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;



	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = itemtype;
			}
		}
	}
}


// 반지 아이템을 추가한다. 
void cltItemManagerClient::AddRingItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	TCHAR itemmall[256];
	SI32 showlist;
	SI32 fieldimg;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siLevelCondition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	SI32 str;
	SI32 dex;
	SI32 vit;
	SI32 mag;
	SI32 luk;
	SI32 hnd;
	SI32 decdur;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,			
			NDATA_INT32,	&fieldimg, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siLevelCondition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac,4,
			NDATA_INT32,	&str, 4,
			NDATA_INT32,	&dex, 4,
			NDATA_INT32,	&vit, 4,
			NDATA_INT32,	&mag, 4,
			NDATA_INT32,	&luk, 4,
			NDATA_INT32,	&hnd, 4,
			NDATA_INT32,	&decdur, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain, 4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,


			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_RING;
			}
		}
	}

}

// 아바타 반지 아이템을 추가한다. 
void cltItemManagerClient::AddAvatarRingItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32  unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32  showlist;
	TCHAR itemmall[256];
	SI32 fieldimg;
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀)
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀)
	SI32 siLevelCondition;
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 ac;
	SI32 str;
	SI32 dex;
	SI32 vit;
	SI32 mag;	// 지력 (Int)
	SI32 luk;
	SI32 hnd;
	SI32 wis;	// 지혜
	SI32 decdur;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 explain;
	SI32 AttackE;
	SI32 AttackR;
	SI32 Grade;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&fieldimg, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siLevelCondition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&ac,4,
			NDATA_INT32,	&str, 4,
			NDATA_INT32,	&dex, 4,
			NDATA_INT32,	&vit, 4,
			NDATA_INT32,	&mag, 4,
			NDATA_INT32,	&luk, 4,
			NDATA_INT32,	&hnd, 4,
			NDATA_INT32,	&wis, 4,
			NDATA_INT32,	&decdur, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain, 4,
			NDATA_INT32,	&AttackE,	4,
			NDATA_INT32,	&AttackR,	4,
			NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true) )
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AVATAR_RING;
			}
		}
	}

}
/////////

// 부적 아이템을 추가한다. 
void cltItemManagerClient::AddScrollItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[24];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	TCHAR adapt[128];	// 어느 장비에 적용되는가 ?
	TCHAR function[128];	// 어떤 성능을 나타내는가?
	SI32 ability;		// 얼마만큼의 능력을 발휘하는가?
	SI32 maxability;	// 최대 능력 
	SI32 durdec;		// 내구도는 얼마나 소모하는가 ?
	SI32 siHnd;
	SI32 mag;			// 도력 제한. 
	SI32 productunit;
	TCHAR szmaterial[256];
	TCHAR sztool[128];
	SI32 siexplain;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_MBSTRING,	adapt, 128,
			NDATA_MBSTRING,	function, 128,
			NDATA_INT32,	&ability, 4,
			NDATA_INT32,	&maxability, 4,
			NDATA_INT32,	&durdec, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&mag, 4,
			NDATA_INT32,	&productunit,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siexplain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_SCROLL;
			}
		}
	}
}


// 철물  아이템을 추가한다. 
void cltItemManagerClient::AddIronItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	TCHAR armcode[12]	=	TEXT("");
	SI32 showlist;
	SI32 weight;
	SI32 farmtool;
	SI32 minetool;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage = 0 , maxdamage = 0 ;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&farmtool, 4,
			NDATA_INT32,	&minetool, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&MountSkillPoint, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_IRON;
			}
		}
	}

}


// 도기  아이템을 추가한다. 
void cltItemManagerClient::AddDishItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage = 0 , maxdamage = 0 ;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&MountSkillPoint, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_DISH;
			}
		}
	}

}

// 어구  아이템을 추가한다. 
void cltItemManagerClient::AddFishingRodItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	TCHAR armcode[12]	=	TEXT("");
	SI32 showlist;
	SI32 classification;
	SI32 canequip;
	SI32 weight;
	SI32 skilllevel;		// 생산에 필요한 스킬 레벨
	SI32 MountSkillPoint;	// 장착에 필요한 무기 사용술 스킬 포인트
	SI32 makepbt;			// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;			// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 siHnd;
	SI32 durability;
	SI32 usedate;
	SI32 mindamage = 0 , maxdamage = 0 ;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&classification, 4,
			NDATA_INT32,	&canequip, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&MountSkillPoint, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4 ,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_FISHINGROD;
			}
		}
	}
}


// 가방  아이템을 추가한다. 
void cltItemManagerClient::AddBagItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60]		=	TEXT("");;
	TCHAR code[12]		=	TEXT("");;
	TCHAR iconcode[24]	=	TEXT("");;
	TCHAR armcode[12]	=	TEXT("");;
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siHnd;
	SI32 child;
	SI32 needrank;
	SI32 durability;
	SI32 usedate;
	TCHAR use[64];
	SI32 luck;
	SI32 man;
	SI32 woman;
	SI32 productunit;
	TCHAR szmaterial[128]	=	TEXT("");
	TCHAR sztool[128]		=	TEXT("");
	SI32 explain;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel, 4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&child, 4,
			NDATA_INT32,	&needrank, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_MBSTRING,	use, 128,
			NDATA_INT32,	&luck, 4,
			NDATA_INT32,	&man, 4,
			NDATA_INT32,	&woman, 4,
			NDATA_INT32,	&productunit,4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&explain,4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_BAG;
			}
		}
	}
}


// 망토 아이템을 추가한다. 
void cltItemManagerClient::AddMantleItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[128]		=	TEXT("");;
	TCHAR code[128]		=	TEXT("");;
	TCHAR iconcode[128]	=	TEXT("");;
	TCHAR armcode[128]	=	TEXT("");;
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siHnd;
	SI32 child;
	SI32 needrank;
	SI32 durability;
	SI32 ac;
	SI32 str;
	SI32 dex;
	SI32 vit;
	SI32 man;
	SI32 woman;
	SI32 useDate;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128]	=	TEXT("");
	TCHAR sztool[128]		=	TEXT("");

	SI32 siExplain = 0;

	SI32 AttackE = 0;
	SI32 AttackR = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&child, 4,
			NDATA_INT32,	&needrank, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&ac, 4,
			NDATA_INT32,	&str, 4,
			NDATA_INT32,	&dex, 4,
			NDATA_INT32,	&vit,4,
			NDATA_INT32,	&man, 4,
			NDATA_INT32,	&woman,4,
			NDATA_INT32,	&useDate, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial,128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			//NDATA_INT32,	&AttackE,	4,
			//NDATA_INT32,	&AttackR,	4,
			//NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_MANTLE;
			}
		}
	}
}

// 아바타 망토
void cltItemManagerClient::AddAvatarMantleItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[128]		=	TEXT("");;
	TCHAR code[128]		=	TEXT("");;
	TCHAR iconcode[128]	=	TEXT("");;
	TCHAR armcode[128]	=	TEXT("");;
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 skilllevel;	// 생산에 필요한 스킬 레벨 
	SI32 makepbt;		// 기본 생산 확률. (퍼밀) 
	SI32 incpbt;		// 기술 향상에 따른 생산 확률증대(퍼밀) 
	SI32 condition;
	SI32 siHnd;
	SI32 child;
	SI32 needrank;
	SI32 durability;
	SI32 ac;
	SI32 str;
	SI32 dex;
	SI32 vit;
	SI32 man;
	SI32 woman;
	SI32 useDate;
	SI32 productunit;
	SI32 SetItemIndex;
	TCHAR szmaterial[128]	=	TEXT("");
	TCHAR sztool[128]		=	TEXT("");

	SI32 siExplain = 0;

	SI32 AttackE = 0;
	SI32 AttackR = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;

	//SI32 incskillamount; // 스킬 증가치

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_MBSTRING,	armcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&skilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&condition, 4,
			NDATA_INT32,	&siHnd, 4,
			NDATA_INT32,	&child, 4,
			NDATA_INT32,	&needrank, 4,
			NDATA_INT32,	&durability, 4,
			NDATA_INT32,	&ac, 4,
			NDATA_INT32,	&str, 4,
			NDATA_INT32,	&dex, 4,
			NDATA_INT32,	&vit,4,
			NDATA_INT32,	&man, 4,
			NDATA_INT32,	&woman,4,
			NDATA_INT32,	&useDate, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_INT32,	&SetItemIndex,4,
			NDATA_MBSTRING,	szmaterial,128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			//NDATA_INT32,	&AttackE,	4,
			//NDATA_INT32,	&AttackR,	4,
			//NDATA_INT32,	&Grade,		4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_AVATAR_MANTLE;
			}
		}
	}

}
//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
void cltItemManagerClient::AddCrystalItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 weight;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	SI32 siLevelCondition = 0;
	SI32 siTemp = 0;
	//cltCrystalItemData	clCrystalItemData[MAX_CAN_CRYSTALABILITY_NUM] ;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&siLevelCondition, 4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&siTemp,		4,
			NDATA_INT32,	&NewMarket,		4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData( true ))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_CRYSTAL;
			}
		}
	}
}

void cltItemManagerClient::AddQuestItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR type[24];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade= 0;
	TCHAR itemmall[256] ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	type,	128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain,4,
			NDATA_MBSTRING, itemmall,256,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_QUEST;
			}
		}
	}
}

void cltItemManagerClient::AddCompositionManterial(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 grownup = 0;			// 성인 전용 아이템 - 군주 스페셜에서 사용
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 usedate;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;



	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_COMPOSITIONMATERIAL;
			}
		}
	}
}


void cltItemManagerClient::AddEventitem(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 grownup = 0;			// 성인 전용 아이템 - 군주 스페셜에서 사용
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 usedate;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;



	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_EVENTITEM;
			}
		}
	}
}


void cltItemManagerClient::AddBox(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 grownup = 0;			// 성인 전용 아이템 - 군주 스페셜에서 사용
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 usedate;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;



	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_BOX;
			}
		}
	}
}


void cltItemManagerClient::AddPremium(TCHAR* datfilename, TCHAR* pname, SI32 itemtype)
{

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 makeskill;			// 생산에 필요한 기술.
	SI32 makeskilllevel;	// 생산에 필요한 기술 레벨 
	SI32 makepbt;
	SI32 incpbt;
	SI32 siuse;
	SI32 weight;
	SI32 grownup = 0;			// 성인 전용 아이템 - 군주 스페셜에서 사용
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 usedate;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique, 4,		
			NDATA_MBSTRING,	szname, 128,
			NDATA_MBSTRING,	code, 128,
			NDATA_MBSTRING,	iconcode, 128,
			NDATA_INT32,	&showlist, 4,
			NDATA_MBSTRING,	itemmall, 256,
			NDATA_INT32,	&makeskill, 4,
			NDATA_INT32,	&makeskilllevel,4,
			NDATA_INT32,	&makepbt, 4,
			NDATA_INT32,	&incpbt, 4,
			NDATA_INT32,	&siuse, 4,
			NDATA_INT32,	&weight, 4,
			NDATA_INT32,	&grownup, 4,
			NDATA_INT32,	&productunit, 4,
			NDATA_MBSTRING,	szmaterial, 128,
			NDATA_MBSTRING,	sztool, 128,
			NDATA_INT32,	&siExplain, 4,
			NDATA_INT32,	&usedate, 4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData(true))
		{
			if ( szname[ 0 ] != NULL)
			{
				m_siItemType[unique] = ITEMTYPE_PREMIUM;
			}
		}
	}
}

// 광석 아이템을 추가한다. 
void cltItemManagerClient::AddMineralItem(TCHAR* datfilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		

	if (!bLoadFile)
	{
		return;
	}

	SI32 unique;
	TCHAR szname[60];
	TCHAR code[10];
	TCHAR iconcode[24];
	SI32 showlist;
	TCHAR itemmall[256];
	SI32 weight;
	SI32 grownup = 0;
	SI32 productunit;
	TCHAR szmaterial[128];
	TCHAR sztool[128];	
	SI32 siExplain = 0;
	SI32 Grade = 0 ;
	SI32 MinNum = 0;
	SI32 NewMarket = 0;


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&unique,		4,		
			NDATA_MBSTRING,	szname,			128,
			NDATA_MBSTRING,	code,			128,
			NDATA_MBSTRING,	iconcode,		128,
			NDATA_INT32,	&showlist,		4,
			NDATA_MBSTRING,	itemmall,		256,
			NDATA_INT32,	&weight,		4,
			NDATA_INT32,	&grownup,		4,
			NDATA_INT32,	&productunit,	4,
			NDATA_MBSTRING,	szmaterial,		128,
			NDATA_MBSTRING,	sztool,			128,
			NDATA_INT32,	&siExplain,		4,
			NDATA_INT32,	&MinNum,		4,
			NDATA_INT32,	&NewMarket,		4,

			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() )
	{
		while( !dataloader.IsEndOfData() ) {

			if( dataloader.ReadData(true))
			{
				if ( szname[ 0 ] != NULL)
				{
					m_siItemType[unique] = ITEMTYPE_MINERAL;
				}
			}
		}
	}
}