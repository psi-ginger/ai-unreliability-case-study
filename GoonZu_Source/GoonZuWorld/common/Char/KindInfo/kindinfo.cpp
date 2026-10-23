//---------------------------------
// 2003/5/28 김태곤
//---------------------------------

#include "..\..\..\CommonLogic\CommonLogic.h"

#include "..\..\Client\Music\Music.h"

#include "KindInfo.h"
#include "..\..\..\CommonLogic\AttackType\AttackType.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\..\..\CommonLogic\CityHall\CityHall.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharClient\Char-Client.h"

extern cltCommonLogic* pclClient;

//-------------------------------------------------------
// 보스의 아이템 정보 
//-------------------------------------------------------
cltBossItemInfo::cltBossItemInfo()
{
	Init();
}
void cltBossItemInfo::Init()
{
	siItemIndex = 0;
	for(SI32 i = 0;i < MAX_BOSS_ITEM_NUMBER;i++)
	{
		clUnit[i].Init();
	}
}

void cltBossItemInfo::Set(cltBossItemInfo* pclinfo)
{
	siItemIndex = pclinfo->siItemIndex;
	for(SI32 i = 0;i < MAX_BOSS_ITEM_NUMBER;i++)
	{
		clUnit[i].Set(&pclinfo->clUnit[i]);
	}
}

// 아이템 목록을 추가한다. 
void cltBossItemInfo::Add(cltBossItemInfoUnit* pclinfo)
{
	if(siItemIndex < 0)						return ;
	if(siItemIndex >= MAX_BOSS_ITEM_NUMBER)	return ;

	clUnit[siItemIndex].Set(pclinfo);
	siItemIndex++;

}

void cltBossItemInfo::Add(SI16 itemunique, SI16 rate)
{
	cltBossItemInfoUnit clinfo(itemunique, rate);
	Add(&clinfo);
}

// 임의의 아이템을 찾는다. 
bool cltBossItemInfo::GetRandItem(SI32* pitemunique, SI32* pitemnum)
{
	if(siItemIndex <= 0)					return false;
	if(siItemIndex >= MAX_BOSS_ITEM_NUMBER)	return false;

	SI32 randvalue = rand() % 10000;

	SI32 accvalue = 0;
	for(SI32 i = 0;i < siItemIndex;i++)
	{
		accvalue += clUnit[i].siRate;
		if(randvalue < accvalue)
		{
			*pitemunique	= clUnit[i].siItemUnique;
			*pitemnum		= 1;
			return true;
		}

	}

	return false;
}

//-------------------------------------------------------
// cltKindInfo
//-------------------------------------------------------
cltKindInfo::cltKindInfo(	TCHAR*	szcode, 
							cltKIDrawInfo*				pclkidrawinfo, 
							cltKIEffectInfo*			pcleffectinfo, 
							cltKindHireCondition*		pclhireinfo,
							cltBossItemInfo*			pclBossItemInfo,
						/*	cltAtkTypeTolerance*		pclAtkTypeTolerance,*/
							SI32 movespeed,		
							SI32 size,
							SI32  siblock,
							DWORD atb, 
							DWORD ServiceAreaAtb,
							DWORD defenceatb, 
							SHORT vibrateswitch, 					 
							TCHAR* name,
							SI32 countury)
{
	StringCchCopy(szCode, MAX_KIND_NAME_LENGTH, szcode);

	siPreferenceCharNum		= 0;
	siKindLifeTime			= 0;		// 캐릭터의 평균 생존 시간. 

	pclChatInfo			= NULL;
	pclWarChatInfo		= NULL;

	// 이 캐릭터를 소환할 수 있는 인형 아이템의 유니크. 
	siDollUnique			= 0;	

	memcpy(&clKIDrawInfo, pclkidrawinfo, sizeof(cltKIDrawInfo));					//ok
	clKIEffectInfo.Set(pcleffectinfo);

	clHireCond.Set(pclhireinfo);
	clBossItemInfo.Set(pclBossItemInfo);

//	clAtkTypeTolerance.Set(p	clAtkTypeTolerance);

	siMoveSpeed				= movespeed;
	siSize					= size;
	
	siBlock					= siblock;

	siAtb					= atb;
	siServiceAreaAtb		= ServiceAreaAtb;
	
	DefenceAtb				= defenceatb;
	
	VibrateSwitch			= vibrateswitch;
	

	siAggressive			= 0;
	Exp						= 0;// SI32->SI64 경험치 오버플로우때문에 수정
	siMagic					= 0;
	siSex					= 0;

	siSortLevel				= 0;
	siSortLevel				= 0;

	siCriticalRate			= 0;

	bSummonMall				= FALSE ;

	siEvolveLevel			= 0;
	siSummonPortrait		= 0;
	siSummonJob				= 0;
	siSubSkillLevel			= 0;

	MStrCpy(szName, name, MAX_KIND_NAME_LENGTH);


	// 포획 관련 
	siCaptureLevel			= 0;			// 포획에 필요한 기술 레벨 
	siBasicPbt				= 0;
	siAdvPbt				= 0;			// 포획술 1당 향상되는 포획확률. 

	for ( SI32 i = 0 ; i < MAX_ATTACKTYPE_PER_KIND ; i ++ )
		siAttackList[i] = 0 ;

	siMaxAttackType = 0 ;

	siDefaultCountry = countury ;

	pclCharElemental.Init();

	bSummonTrade			= false;
	siSummonEquip			= 0;
	siSummonStamina			= 0;
}

cltKindInfo::~cltKindInfo()
{
	if(pclWarChatInfo)
	{
		delete pclWarChatInfo;
		pclWarChatInfo = NULL ;
	}
	if(pclChatInfo)
	{
		delete pclChatInfo;
		pclChatInfo = NULL;
	}
}

BOOL cltKindInfo::SetFile(SI32 Ani, BOOL DirSwitch, TCHAR* FileName, SI32 Font, SI32 Frame,SI32 Instant,
						  SI32 Delay, SI32 atb, SI32 EffectFont ,SI32 transmode,SI08 alphavalue,
						  bool attacktype,SI32 ShadowAni,SI32 ShadowFont)
{
	if ( attacktype || Ani == ANITYPE_ATTACK )
	{
		for ( int i = 0 ; i < MAX_ATTACKTYPE_PER_KIND ; i ++ )
		{
			if ( siAttackList[i] == 0 )
			{
				siAttackList[i] = Ani ;
				siMaxAttackType ++ ;
				break ;
			}
		}
	}

	return clCharAniManager.Add(Ani, DirSwitch, FileName, Font, Frame, Instant, Delay, atb,
								EffectFont,transmode,alphavalue,ShadowAni,ShadowFont );
}

BOOL cltKindInfo::SetShadowIndentXY(SI32 anitype , SI32 x , SI32 y)
{
	return clCharAniManager.SetShadowIndentXY(anitype,x,y);
}

void cltKindInfo::IncreasePreferenceCharNum(SI32 lifetime)
{
	lifetime = max(1, min(100, lifetime/1000));

	siKindLifeTime += lifetime;
	++siPreferenceCharNum;

	return;
}

// 포획확률을 구한다. 
SI32 cltKindInfo::GetCaptureRate(SI32 skilllevel)
{
	SI32 rate = 0;
	if(skilllevel < siCaptureLevel)
	{
		rate = 0;
	}
	else
	{
		rate = siBasicPbt;

		SI32 addlevel = skilllevel - siCaptureLevel;

		if(addlevel > 30)
		{
			rate += ( (addlevel - 30) * siAdvPbt / 2 );
			addlevel = 30;
		}
		rate += ( addlevel * siAdvPbt );
	}

	return rate;
}

// 아이템 정보를 분석한다. 
bool cltKindInfo::AnalyzeItemInfo(TCHAR* szitemtext)
{
	TCHAR c;

	// 아이템 
	TCHAR szitemname[128];
	SI32 itemnameindex	= 0;
	bool itemnameswith = false;

	// 아이템 최대수
	TCHAR szitemnum[128];
	SI32 itemnumindex	= 0;
	bool itemnumswitch = true;

	// 아이템 최소수
	TCHAR szitemminnum[128];
	SI32 itemminnumindex	= 0;
	bool itemminnumswitch = true;

	// 아이템 확율
	TCHAR szitempbt[128];
	SI32 itempbtindex	= 0;
	bool itempbtswith = false;

	SI32 textindex	= 0;

	bool unlimitloop = true;

	while( unlimitloop )
	{
		c = szitemtext[textindex];
		if(c == '\0')break;

		textindex++;

		// Space는 무시한다. 
		if( c == ' ')continue;
		if( c == '+')
		{
			itemnameswith	= false;
			itemnumswitch	= true;
			itemminnumswitch = true;

			continue;
		}
		if( c == '(')
		{
			szitemname[itemnameindex] = '\0';	// 이름을 마무리한다. 
			itemnameindex	= 0;

			itemnameswith = true;	// 이름은 얻어왔다. 다음은 아이템확률 순서다. 
			continue;
		}
		if(c == '-')
		{
			itemminnumswitch = false;	// 아이템 최소수를 구하는 모드이다. 
		}
		if(c == ':')
		{
			itemminnumswitch = true ;
			itemnumswitch = false;	// 아이템 최대수를 구하는 모드이다. 
		}
		if( c == ')')
		{

			szitemminnum[itemminnumindex] = '\0';	// 아이템 최소수를 마무리한다. 
			itemminnumindex	= 0;	

			szitemnum[itemnumindex] = '\0';		// 아이템 최대수를 마무리한다. 
			itemnumindex	= 0;	

			szitempbt[itempbtindex] = '\0';		// 아이템 확률을 마무리한다. 
			itempbtindex	= 0;

			//-------------------------------------------
			// 이름과 확률을 모두 얻어왔으므로 분석한다. 
			//-------------------------------------------
			
			// NULL이면 리턴한다. 
			if(_tcscmp(szitemname, TEXT("NULL")) == 0)
			{
				return false;
			}

			//SI32 unique = pclClient->pclItemManager->FindUniqueFromName(szitemname);
			SI32 unique = _tstoi(szitemname);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(unique);
			//if(unique)
			if ( ref > 0 )
			{
				SI32 itempbt	= _tstoi(szitempbt);

				SI32 itemnum;
				SI32 minnum;

				if(itemnumswitch == false )
				{
					itemnum = _tstoi(szitemnum);
					if ( itemnum == 10 )
					{
						SI32 sdgsdg = 0 ;
					}
					if(itemnum <= 0)	itemnum	= 1;
					if(itemnum > MAX_MONSTERDROPITEM_NUM)	itemnum	= MAX_MONSTERDROPITEM_NUM;	

					minnum = _tstoi(szitemminnum);
					if(minnum <= 0)	minnum	= 1;
					if(minnum > MAX_MONSTERDROPITEM_NUM)	minnum	= MAX_MONSTERDROPITEM_NUM;

					if ( minnum > itemnum )
						minnum = 1 ;
				}
				else
				{
					itemnum = 1;
					minnum = 1;
				}

			clItemPbtSet.Add(unique, itempbt, itemnum,minnum);
			}
			else
			{
				MsgBox(TEXT("NotProperItem"), TEXT("text:[%s] name:[%s] itemname:[%s]"), szitemtext, szName, szitemname);
				SI32 a = 0;
			}
			
			continue;
		}

		// 아이템의 이름이 아직 결정되지 않았으면 이름을 입력받는다. 
		if(itemnameswith == false)
		{
			szitemname[itemnameindex] = c;
			itemnameindex++;
			if(itemnameindex > 100)return false;
		}
		else if(itemminnumswitch == false)
		{
			// 아이템 최소수를 구한다. 
			if(c != '-')
			{
				szitemminnum[itemminnumindex] = c;
				itemminnumindex++;
				if(itemminnumindex > 100)return false;
			}
		}
		else if(itemnumswitch  == false)
		{
			// 아이템 최대수를 구한다. 
			if(c != ':')
			{
				szitemnum[itemnumindex] = c;
				itemnumindex++;
				if(itemnumindex > 100)return false;
			}
		}
		else
		{
			szitempbt[itempbtindex] = c;
			itempbtindex++;
		}

	}

	return true;


}


// 이 몬스터가 성황신으로 될수있는지 
bool cltKindInfo::CanGodForVillage(SI32 villageunique)
{
	switch (pclClient->siServiceArea)
	{
	case ConstServiceArea_Japan :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_JAPAN))
				return false;
		}
		break;	
	case ConstServiceArea_English :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_ENGLISH))
				return false;
		}
		break;	
	case ConstServiceArea_China :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_CHINA))
				return false;
		}
	case ConstServiceArea_Korea :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_ENGLISH))
				return false;
		}
		break;	
	case ConstServiceArea_Taiwan :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_TAIWAN))
				return false;
		}
		break;
	case ConstServiceArea_USA  :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_USA))
				return false;
		}
		break;
	case ConstServiceArea_NHNChina :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_NHNCHINA))
				return false;
		}
		break;
	case ConstServiceArea_EUROPE :
		{
			if (!(siServiceAreaAtb & SERVICEAREA_ATB_EUROPE))
				return false;
		}
		break;

	}

	// 무조건 이 캐릭터는 성황신이 될수있다.
	if(siAtb & ATB_DEFAULTGOD)	return true;

	if((siAtb & ATB_GOD )== 0)		return false;

	//cltStrInfo* pclstrinfo = &pclClient->pclCityHallManager->clClientStrInfo;
	cltStrInfo* pclstrinfo = pclClient->pclCityHallManager->GetStrInfo(villageunique);
	if(pclstrinfo == NULL)return false;


	// 주민수 확보. (최대값은 300) 
	SI32 residentnum = pclstrinfo->clCityhallStrInfo.siResidentNum;

	// 성황신으로 되기위한 인구가 현재의 인구보다 크면 false
	if(GetResidentNumForVillageGod() >residentnum) return false;

	return true;
}

// 성황신이 되기위해 필요한 인구수 
SI32 cltKindInfo::GetResidentNumForVillageGod()
{
	// 성황신이 될수 없는 캐릭터는 0
	if(!((siAtb & ATB_DEFAULTGOD) == ATB_DEFAULTGOD) && !((siAtb & ATB_GOD)==ATB_GOD))	return 0;

	return min(150, siSortLevel * 2);
}



// 성황신의 고용하는데 필요한 비용 
GMONEY cltKindInfo::GetMoneyForVillageGod()
{
	// 성황신이 될수 없는 캐릭터는 0
	if(!((siAtb & ATB_DEFAULTGOD) == ATB_DEFAULTGOD) && !((siAtb & ATB_GOD)==ATB_GOD))	return 0;

	// 몬스터의 능력치를 전부더한다.
	GMONEY money = ( clBA.GetStr() + clBA.GetDex() + clBA.GetVit()  + clBA.GetMag()+ clBA.GetWis() ) * 50000;

	return money;
}


// 성황신의 업그레이드 하는데 필요한 비용 
GMONEY cltKindInfo::GetUpMoneyForVillageGod()
{
	// 성황신이 될수 없는 캐릭터는 0
	if(!((siAtb & ATB_DEFAULTGOD) == ATB_DEFAULTGOD) && !((siAtb & ATB_GOD)==ATB_GOD))	return 0;

	return GetMoneyForVillageGod() / 20;
}

// NPC의 혼잣말을 설정한다. 
void cltKindInfo::AddChat(TCHAR* ptext)
{
	if(pclChatInfo == NULL)
	{
		pclChatInfo = new cltKindChatInfo;
		if(pclChatInfo == NULL)
		{
			MsgBox(TEXT("vdfef"), TEXT("vdfddf:%s"), ptext);
			return ;
		}
	}

	pclChatInfo->Add(ptext);

}

// NPC의 특정 맵에서의 혼잣말을 설정한다. 
void cltKindInfo::AddWarChat(TCHAR* ptext,SI32 mapindex)
{
	if(pclWarChatInfo == NULL)
	{
		pclWarChatInfo = new cltWarChatInfo;
		if(pclWarChatInfo == NULL)
		{
			MsgBox(TEXT("vdffs"), TEXT("vdfwwsf:%s"), ptext);
			return ;
		}
	}

	pclWarChatInfo->Add(ptext,mapindex);
}

// NPC의 특정 맵에서의 혼잣말을 설정한다. 
void cltKindInfo::AddWarWinChat(TCHAR* ptext,SI32 mapindex)
{
	if(pclWarChatInfo == NULL)
	{
		pclWarChatInfo = new cltWarChatInfo;
		if(pclWarChatInfo == NULL)
		{
			MsgBox(TEXT("vdwsd"), TEXT("vdsawsx:%s"), ptext);
			return ;
		}
	}

	pclWarChatInfo->AddWin(ptext,mapindex);
}

// 랜덤하게 혼잣말을 구한다. 
TCHAR* cltKindInfo::GetRandChatText()
{
	if(pclChatInfo == NULL)			return NULL;
	if(pclChatInfo->siChatNum <= 0)	return NULL;
	if(pclChatInfo->siChatNum > MAX_NPC_TEXT_NUM)	return NULL;

	return pclChatInfo->pszRandText[rand() % pclChatInfo->siChatNum];
}

// 랜덤하게 혼잣말을 가져온다. 전쟁시 이겼을 때
TCHAR* cltKindInfo::GetWarRandChatWinText(SI32 mapindex)
{
	if(pclWarChatInfo == NULL)			return NULL;
	if(pclWarChatInfo->siChatNum <= 0)	return NULL;
	if(pclWarChatInfo->siChatNum > MAX_NPC_TEXT_NUM)	return NULL;
	if(pclWarChatInfo->siMapindex != mapindex) return NULL;

	return pclWarChatInfo->pszWarTextWin[rand() % pclWarChatInfo->siChatNum];
}

// 랜덤하게 혼잣말을 가져온다. 전쟁중
TCHAR* cltKindInfo::GetWarRandChatText(SI32 mapindex)
{
	if(pclWarChatInfo == NULL)			return NULL;
	if(pclWarChatInfo->siChatNum <= 0)	return NULL;
	if(pclWarChatInfo->siChatNum > MAX_NPC_TEXT_NUM)	return NULL;
	if(pclWarChatInfo->siMapindex != mapindex) return NULL;

	return pclWarChatInfo->pszWarText[rand() % pclWarChatInfo->siChatNum];
}

SI32 cltKindInfo::GetPortraitFont() const	
{	
	return clKIDrawInfo.siPortraitFont;	
}

SI32 cltKindInfo::GetBottomIndent() const
{
	return clKIDrawInfo.siBottomIndent;
}

SI32 cltKindInfo::GetXsizePercent() const
{
	return clKIDrawInfo.siXsizePercent;
}

SI32 cltKindInfo::GetYsizePercent() const
{
	return clKIDrawInfo.siYsizePercent;
}

const TCHAR*	cltKindInfo::GetName() const
{
	return szName;
}

bool cltKindInfo::IsAtb(SI32 atb)
{
	if(this == NULL)return false;
	if(siAtb & atb)return true;
	return false;
}

bool cltKindInfo::IsServiceAreaAtb(SI32 saAtb)
{
	if(this == NULL)return false;
	if(siServiceAreaAtb & saAtb)return true;
	return false;
}

//=================================================
// cltKindSummonJob
//=================================================
cltKindSummonJob::cltKindSummonJob()
{
	memset(szJobCode, 0, sizeof(szJobCode));
	memset(szJobName, 0, sizeof(szJobName));
	siCount = MAX_SUMMON_JOB;
	
	// JOB 추가 
	StringCchCopy(szJobCode[SUMMON_JOB_NULL], 30, 	 TEXT("NULL"));	
	TCHAR* pText = GetTxtFromMgr(1287);
	StringCchCopy(szJobName[SUMMON_JOB_NULL], 30, 	 pText);
	siJobSkill[SUMMON_JOB_NULL] = 0;	 
	
	StringCchCopy(szJobCode[SUMMON_JOB_FISHING], 30, TEXT("FISHING"));
	pText = GetTxtFromMgr(2802);
	StringCchCopy(szJobName[SUMMON_JOB_FISHING], 30,  pText);
	siJobSkill[SUMMON_JOB_FISHING] = SKILL_FISH;

	StringCchCopy(szJobCode[SUMMON_JOB_FARMING], 30, TEXT("FARMING"));
	pText = GetTxtFromMgr(2803);
	StringCchCopy(szJobName[SUMMON_JOB_FARMING], 30, pText);
	siJobSkill[SUMMON_JOB_FARMING] = SKILL_FARM;

	StringCchCopy(szJobCode[SUMMON_JOB_MINING], 30, TEXT("MINING"));
	pText = GetTxtFromMgr(3224);
	StringCchCopy(szJobName[SUMMON_JOB_MINING], 30, pText);
	siJobSkill[SUMMON_JOB_MINING] = SKILL_MINE;
}

SI32 cltKindSummonJob::FindSummonJobFromJobCode(TCHAR *pJobCode)
{
	for(SI32 i = 0; i< siCount; ++i)
	{
		if( siCount >= MAX_SUMMON_JOB )		continue;

		if(_tcsicmp(szJobCode[i], pJobCode) ==0)
		{
			return i;
		}
	}
	return 0;
}

TCHAR *cltKindSummonJob::FindSummonJobNameFromJob(SI32 siJob)
{
	if(siJob < 0 || siJob >= siCount || siJob >= MAX_SUMMON_JOB) return szJobName[0];
	return szJobName[siJob];
}

SI32 cltKindSummonJob::FindSummonJobSkillFromJob(SI32 siJob)
{
	if(siJob < 0 || siJob >= siCount || siJob >= MAX_SUMMON_JOB) return siJobSkill[0];
	return siJobSkill[siJob];
}

//=================================================
// cltDropItem
//=================================================
cltDropItem::cltDropItem()
{
	szName[ 0 ] = NULL;
	szItemNum[ 0 ] = NULL;
	szPercent[ 0 ] = NULL;

}	

//=================================================
// cltBossItemInfoUnit
//=================================================
cltBossItemInfoUnit::cltBossItemInfoUnit()
{
	Init();
}
cltBossItemInfoUnit::cltBossItemInfoUnit(SI16 itemunique, SI16 rate)
{
	siItemUnique	=  itemunique;
	siRate			= rate;
}

void cltBossItemInfoUnit::Init()
{
	siItemUnique	= 0;
	siRate			= 0;
}
void cltBossItemInfoUnit::Set(cltBossItemInfoUnit* pclinfo)
{
	siItemUnique	= pclinfo->siItemUnique;
	siRate			= pclinfo->siRate;
}

//=================================================
// cltKindItemProbability
//=================================================
cltKindItemProbability::cltKindItemProbability()
{
	siItemUnique	= 0;
	siProbability	= 0;
	siItemNum		= 0;
	siItemMinNum	= 0;
}
cltKindItemProbability::~cltKindItemProbability()
{}

void cltKindItemProbability::Set(SI32 unique, SI32 probability, SI32 itemnum,SI32 minnum)
{
	siItemUnique	= unique;
	siProbability	= probability;
	siItemNum		= itemnum;
	siItemMinNum	= minnum;
}

//=================================================
// cltDefaultWeaponInfo
//=================================================
cltDefaultWeaponInfo::cltDefaultWeaponInfo()
{
	siAttackType		= 0;
	siAttackRange		= 0;
	siDefaultMinDamage	= 0;
	siDefaultMaxDamage	= 0;
	siDefaultAC			= 0;

}

void cltDefaultWeaponInfo::Set(SI32 attacktype, SI32 range, SI32 mindamage, SI32 maxdamage, SI32 ac)
{
	siAttackType		= attacktype;
	siAttackRange		= range;
	siDefaultMinDamage	= mindamage;
	siDefaultMaxDamage	= maxdamage;
	siDefaultAC			= ac;
}

void cltDefaultWeaponInfo::Set(cltDefaultWeaponInfo* pclinfo)
{
	siAttackType		= pclinfo->siAttackType;
	siAttackRange		= pclinfo->siAttackRange;
	siDefaultMinDamage	= pclinfo->siDefaultMinDamage;
	siDefaultMaxDamage	= pclinfo->siDefaultMaxDamage;
	siDefaultAC			= pclinfo->siDefaultAC;
}

SI32 cltDefaultWeaponInfo::GetAttackType() const
{
	return siAttackType;
}

SI32 cltDefaultWeaponInfo::GetAttackRange() const
{
	return siAttackRange;
}

SI32 cltDefaultWeaponInfo::GetDefaultMinDamage() const
{
	return siDefaultMinDamage;
}

SI32 cltDefaultWeaponInfo::GetDefaultMaxDamage() const
{
	return siDefaultMaxDamage;
}

SI32 cltDefaultWeaponInfo::GetDefaultAC() const
{
	return siDefaultAC;
}

//=================================================
// cltKIDrawInfo
//=================================================
cltKIDrawInfo::cltKIDrawInfo()
{
	siPortraitFont	= 0;
	siHeadNameFont	= 0;
	siBottomIndent	= 0;        	
	siXsizePercent	= 0;
	siYsizePercent	= 0;
	siTransparentMode = 0;
	siAlphaValue	= 0;
}

void cltKIDrawInfo::Set(SI32 portrait, SI32 headnamefont, SI32 bottomindent,
		 SI32 xpercent, SI32 ypercent, SI32 transparentmode,bool hideshadow,SI08 alphavalue)
{
	siPortraitFont	= portrait;
	siHeadNameFont	= headnamefont;
	siBottomIndent	= bottomindent;        	
	siXsizePercent	= xpercent;
	siYsizePercent	= ypercent;
	siTransparentMode = transparentmode;
	bHideShadow		= hideshadow ;
	siAlphaValue	= alphavalue ;
}

//=================================================
// cltKIEffectInfo
//=================================================
cltKIEffectInfo::cltKIEffectInfo()
{
	siHitEffect		= 0;
	siAttackEffect	= 0;
	siAttackEffect1 = 0;
	siDyingEffect	= 0;
	siSelectEffect1	= 0;
	siSelectEffect2	= 0;
	siSelectEffect3	= 0;
}

void cltKIEffectInfo::Set(SI32 hiteffect, SI32 attackeffect, SI32 attackeffect1, SI32 dyingeffect, SI32 selecteffect1, SI32 selecteffect2, SI32 selecteffect3)
{
	siHitEffect		= hiteffect;
	siAttackEffect	= attackeffect;
	siAttackEffect1 = attackeffect1;
	siDyingEffect	= dyingeffect;
	siSelectEffect1	= selecteffect1;
	siSelectEffect2	= selecteffect2;
	siSelectEffect3	= selecteffect3;
}

void cltKIEffectInfo::Set(cltKIEffectInfo* pclinfo)
{
	Set( pclinfo->siHitEffect, pclinfo->siAttackEffect, pclinfo->siAttackEffect1, pclinfo->siDyingEffect, pclinfo->siSelectEffect1, pclinfo->siSelectEffect2, pclinfo->siSelectEffect3 );
}

SI32 cltKIEffectInfo::GetRandomeSelectEffect()
{
	SI32 siCount = 0;
	if( siSelectEffect1 > 0 )			siCount++;
	if( siSelectEffect2 > 0 )			siCount++;
	if( siSelectEffect3 > 0 )			siCount++;

	if(siCount == 0)		return 0;
	SI32 siRand = rand() % siCount;

	if( siRand == 0 )		return siSelectEffect1;
	if( siRand == 1 )		return siSelectEffect2;
	if( siRand == 2 )		return siSelectEffect3;

	return 0;
}

//=================================================
// cltWarChatInfo
//=================================================
cltWarChatInfo::cltWarChatInfo()
{
	ZeroMemory(this, sizeof(cltWarChatInfo));
}
cltWarChatInfo::~cltWarChatInfo()
{
	SI32 i;

	for(i = 0;i < MAX_NPC_TEXT_NUM;i++)
	{
		if(pszWarTextWin[i])
		{
			delete[] pszWarTextWin[i];
			pszWarTextWin[i] = NULL;
		}

		if(pszWarText[i])
		{
			delete[] pszWarText[i];
			pszWarText[i] = NULL;
		}
	}

	siWinChatNum = 0 ;
	siChatNum = 0 ;
}

void cltWarChatInfo::AddWin(TCHAR* ptext,SI32 mapindex)
{
	if ( siMapindex != 0 && siMapindex != mapindex ) return ;
	if ( siMapindex == 0 ) siMapindex = mapindex ;
	if(ptext == NULL)return;
	if(siWinChatNum < 0)				return ;
	if(siWinChatNum >= MAX_NPC_TEXT_NUM)return ;
	if(pszWarTextWin[siWinChatNum] == NULL)
	{
		pszWarTextWin[siWinChatNum] = new TCHAR [ lstrlen(ptext) + 1];
	}

	if(pszWarTextWin[siWinChatNum])
	{
		StringCchCopy(pszWarTextWin[siWinChatNum], lstrlen(ptext) + 1, ptext);
		siWinChatNum ++;
	}
}

void cltWarChatInfo::Add(TCHAR* ptext,SI32 mapindex)
{
	if ( siMapindex != 0 && siMapindex != mapindex ) return ;
	if ( siMapindex == 0 ) siMapindex = mapindex ;
	if(ptext == NULL)return;
	if(siChatNum < 0)				return ;
	if(siChatNum >= MAX_NPC_TEXT_NUM)return ;
	if(pszWarText[siChatNum] == NULL)
	{
		pszWarText[siChatNum] = new TCHAR [ lstrlen(ptext) + 1];
	}

	if(pszWarText[siChatNum])
	{
		StringCchCopy(pszWarText[siChatNum], lstrlen(ptext) + 1, ptext);
		siChatNum ++;
	}
}

//=================================================
// cltKindChatInfo
//=================================================
cltKindChatInfo::cltKindChatInfo()
{
	ZeroMemory(this, sizeof(cltKindChatInfo));
}

cltKindChatInfo::~cltKindChatInfo()
{
	SI32 i;

	for(i = 0;i < MAX_NPC_TEXT_NUM;i++)
	{
		if(pszRandText[i])
		{
			delete[] pszRandText[i];
			pszRandText[i] = NULL;
		}
	}

	siChatNum = 0 ;
}

void cltKindChatInfo::Add(TCHAR* ptext)
{
	if(ptext == NULL)return;
	if(siChatNum < 0)				return ;
	if(siChatNum >= MAX_NPC_TEXT_NUM)return ;
	if(pszRandText[siChatNum] == NULL)
	{
		pszRandText[siChatNum] = new TCHAR [ lstrlen(ptext) + 1];
	}

	if(pszRandText[siChatNum])
	{
		StringCchCopy(pszRandText[siChatNum], lstrlen(ptext) + 1, ptext);
		siChatNum ++;
	}
}

//=================================================
// cltKindHireCondition
//=================================================
cltKindHireCondition::cltKindHireCondition()
{
	Init();
}


void cltKindHireCondition::Init()
{
	siBasicMoney		= 0;
	siMinVillageLevel	= 0;
}
void cltKindHireCondition::Init(GMONEY money, SI32 villagelevel)
{
	siBasicMoney		= money;
	siMinVillageLevel	= villagelevel;
}


void cltKindHireCondition::Set(cltKindHireCondition* pclinfo)
{
	siBasicMoney		= pclinfo->siBasicMoney;
	siMinVillageLevel	= pclinfo->siMinVillageLevel;
}