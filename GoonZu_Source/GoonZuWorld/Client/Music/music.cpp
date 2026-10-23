//---------------------------------
// 2003/7/30 김태곤
//---------------------------------
#include "..\Client.h"

#include "Music.h"
#include "..\..\Common\Message\Message.h"
#include "NDataLoader.h"

extern cltCommonLogic* pclClient;

//------------------------------------------------
// cltEffectFile
//------------------------------------------------
void cltEffectFile::Set(TCHAR* name)
{
	StringCchPrintf(szFileName, MAX_SOUND_EFFECT_FILE_NAME_LENGTH, TEXT("eft\\%s"), name );
}

//------------------------------------------------
// _EffectBuffer
//------------------------------------------------
void _EffectBuffer::Set(SHORT effect, SHORT x, SHORT y)
{
	Effect = effect;
	X = x;
	Y = y;
}



//------------------------------------------------
// _EffectData
//------------------------------------------------

/*cyj
void _EffectData::Set(TCHAR* szname1, TCHAR* szname2, TCHAR* szname3, TCHAR* szname4, TCHAR* szname5)
{
	Init();
	
	if( szname1 )
	{
		clEffectFile[0].Set(szname1);
		EffectNumber++;
	}
	if( szname2 )
	{
		clEffectFile[1].Set(szname2);
		EffectNumber++;
	}
	if( szname3 )
	{
		clEffectFile[2].Set(szname3);
		EffectNumber++;
	}
	if( szname4 )
	{
		clEffectFile[3].Set(szname4);
		EffectNumber++;
	}
	if( szname5 )
	{
		clEffectFile[4].Set(szname4);
		EffectNumber++;
	}


#ifdef _DEBUG
	// 실제로 그 파일이 있는지 확인한다. 
	for(SI32 i = 0; i < EffectNumber ;i++)
	{
		FILE* fp;

		TCHAR buffer[256];
		sprintf(buffer, TEXT("%s.wav"), clEffectFile[i].szFileName);
		fp = _tfopen(buffer, TEXT("rb"));
		if(fp == NULL)
		{
			MsgBox(TEXT("NoEffectFileName"), clEffectFile[i].szFileName);
		}
		else fclose(fp);
	}
#endif
}
*/

void _EffectData::Set(TCHAR* szname)
{
	Init();

	if( szname )
	{
		//cyj SOUNDINFO.TXT에서 값이 NULL이면 셋팅 안함
		if (_tcscmp(szname, TEXT("NULL")) == 0)
			return;

		if (EffectNumber < MAX_SOUND_EFFECT_FILE_NAME_PER_SOUND)
		{
			clEffectFile[EffectNumber].Set(szname);
			EffectNumber++;
		}
		else
		{
			MsgBox(TEXT("EffectNumber over"), TEXT("Error"));
		}

		
	}

#ifdef _DEBUG
	// 실제로 그 파일이 있는지 확인한다. 
	for(SI32 i = 0; i < EffectNumber ;i++)
	{
		FILE* fp;

		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("%s.wav"), clEffectFile[i].szFileName);
		fp = _tfopen(buffer, TEXT("rb"));
		if(fp == NULL)
		{
			MsgBox(TEXT("NoEffectFileName"), clEffectFile[i].szFileName);
		}
		else fclose(fp);
	}
#endif
}

void _EffectData::Free()
{
	SHORT i;

	// 모든 효과음을 다 해제한다. 
	for(i=0;i<EffectNumber;i++)
	{
		clEffectFile[i].Init();
	}

}


// bgamemode :게임안인지 밖인지...
TCHAR* _EffectData::Play(SI32 x, SI32 y, cltMapClient* pclMap, bool bgamemode)	
{
	// by LEEKH 2007.08.17 - bgamemode == true 인 경우에만 pclMap포인터를 체크하는 것으로 변경
	//if( NULL == pclMap )
	//	return NULL;

	SHORT	index;
	SI32	listenerX, listenerY;

	// 준비된 효과음이 있다면, 
	if( EffectNumber == 0 ) return 0;
	index = rand() % EffectNumber;

	if( bgamemode == true)
	{
		// by LEEKH 2007.08.17 - bgamemode == true 인 경우에만 pclMap포인터를 체크하는 것으로 변경
		if( NULL == pclMap )		return NULL;

		listenerX = pclMap->VirtualMapX,	listenerY = pclMap->VirtualMapY;
		// (x, y) 가 (0, 0)일 때는 3D사운드가 아니다.
		if( x < 0 || x >= pclMap->MH.MapXsize)			x = 0;
		if( y < 0 || y >= pclMap->MH.MapYsize)			y = 0;
	}
	else
	{
		listenerX = 0,	listenerY = 0;
	}

	if( x==0 && y==0 )	x = listenerX, y = listenerY;

	return clEffectFile[index].szFileName;
}



//------------------------------------------------------------
// cltMusic
//------------------------------------------------------------
cltMusic::cltMusic(cltClient* pclgame)
{
	/*
	// Sound 해쉬테이블에 등록
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WEAPON_SWORD_FIRE"),SOUND_EFFECT_WEAPON_SWORD_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WEAPON_SWORD_HIT"),SOUND_EFFECT_WEAPON_SWORD_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WEAPON_AXE_FIRE"),SOUND_EFFECT_WEAPON_AXE_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WEAPON_SPEAR_FIRE"),SOUND_EFFECT_WEAPON_SPEAR_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LEVELUP"),SOUND_EFFECT_LEVELUP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RECOVERLIFE"),SOUND_EFFECT_RECOVERLIFE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RECOVERMANA"),SOUND_EFFECT_RECOVERMANA);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FAILURE"),SOUND_EFFECT_FAILURE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SUCCESS"),SOUND_EFFECT_SUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WARBREAK"),SOUND_EFFECT_WARBREAK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WARWIN"),SOUND_EFFECT_WARWIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WARLOSE"),SOUND_EFFECT_WARLOSE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SERVERFAIL"),SOUND_EFFECT_SERVERFAIL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LETTER"),SOUND_EFFECT_LETTER);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKEFAIL"),SOUND_EFFECT_MAKEFAIL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKESUCCESS"),SOUND_EFFECT_MAKESUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NOTICE"),SOUND_EFFECT_NOTICE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WHISPERCHAT"),SOUND_EFFECT_WHISPERCHAT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VILLAGEWARBREAK"),SOUND_EFFECT_VILLAGEWARBREAK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SUMMON_MONSTER"),SOUND_EFFECT_SUMMON_MONSTER);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRITICALHIT"),SOUND_EFFECT_CRITICALHIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_REPAIRWALL"),SOUND_EFFECT_REPAIRWALL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLOCK"),SOUND_EFFECT_CLOCK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EVENTNOTICE"),SOUND_EFFECT_EVENTNOTICE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLOCK_HALF"),SOUND_EFFECT_CLOCK_HALF);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLOCK_OVER"),SOUND_EFFECT_CLOCK_OVER);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NPCREPAIR_SUCCESS"),SOUND_EFFECT_NPCREPAIR_SUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WENNYOLDMAN_SUCCESS"),SOUND_EFFECT_WENNYOLDMAN_SUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MARKETSELLINFO"),SOUND_EFFECT_MARKETSELLINFO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SKILL_LEVELUP"),SOUND_EFFECT_SKILL_LEVELUP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EVENT_BULGASARI"),SOUND_EFFECT_EVENT_BULGASARI);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EVENT_GAURI"),SOUND_EFFECT_EVENT_GAURI);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FISHING_NOW"),SOUND_EFFECT_FISHING_NOW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FISHING_SUCCESS"),SOUND_EFFECT_FISHING_SUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINING_NOW"),SOUND_EFFECT_MINING_NOW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NOTICEADMIN"),SOUND_EFFECT_NOTICEADMIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NO_PRIVATETRADE"),SOUND_EFFECT_NO_PRIVATETRADE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NO_WHISPERCHAT"),SOUND_EFFECT_NO_WHISPERCHAT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_NO_MANA"),SOUND_EFFECT_NO_MANA);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HUNGRY_WARNING"),SOUND_EFFECT_HUNGRY_WARNING);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DURABILITY_WARNING"),SOUND_EFFECT_DURABILITY_WARNING);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STEAL_WARNING"),SOUND_EFFECT_STEAL_WARNING);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WELCOME_TO_GOONZU"),SOUND_EFFECT_WELCOME_TO_GOONZU);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_IZO"),SOUND_EFFECT_IZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HOZO"),SOUND_EFFECT_HOZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HYUNGZO"),SOUND_EFFECT_HYUNGZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GONGZO"),SOUND_EFFECT_GONGZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BYUNGZO"),SOUND_EFFECT_BYUNGZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YEZO"),SOUND_EFFECT_YEZO);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINISTER_GOONZU"),SOUND_EFFECT_MINISTER_GOONZU);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WAR_NEARLYEND"),SOUND_EFFECT_WAR_NEARLYEND);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WEAPONSKILL_UP"),SOUND_EFFECT_WEAPONSKILL_UP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SWALLOW"),SOUND_EFFECT_SWALLOW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_BOOK"),SOUND_EFFECT_SELECT_BOOK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_BOW"),SOUND_EFFECT_SELECT_BOW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_CLOTH"),SOUND_EFFECT_SELECT_CLOTH);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_FOOD"),SOUND_EFFECT_SELECT_FOOD);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_HEAL"),SOUND_EFFECT_SELECT_HEAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_HELMET"),SOUND_EFFECT_SELECT_HELMET);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_SWORD"),SOUND_EFFECT_SELECT_SWORD);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_AXE"),SOUND_EFFECT_SELECT_AXE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_ARMOUR"),SOUND_EFFECT_SELECT_ARMOUR);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_BELT"),SOUND_EFFECT_SELECT_BELT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_ETC"),SOUND_EFFECT_SELECT_ETC);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_GUN"),SOUND_EFFECT_SELECT_GUN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_NECK"),SOUND_EFFECT_SELECT_NECK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_SHOES"),SOUND_EFFECT_SELECT_SHOES);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SELECT_STAFF"),SOUND_EFFECT_SELECT_STAFF);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_BOOK"),SOUND_EFFECT_USE_BOOK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_HEAL"),SOUND_EFFECT_USE_HEAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_MEAT"),SOUND_EFFECT_USE_MEAT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_NOODLE"),SOUND_EFFECT_USE_NOODLE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_SOUP"),SOUND_EFFECT_USE_SOUP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_VEGETABLE"),SOUND_EFFECT_USE_VEGETABLE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_REPAIR_SWORD"),SOUND_EFFECT_REPAIR_SWORD);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_REPAIR_CLOTH"),SOUND_EFFECT_REPAIR_CLOTH);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_SMELLHEAL"),SOUND_EFFECT_USE_SMELLHEAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_USE_UNFREEZEHEAL"),SOUND_EFFECT_USE_UNFREEZEHEAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ITEM_PICKUP"),SOUND_EFFECT_ITEM_PICKUP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_CITYHALL"),SOUND_EFFECT_OPEN_CITYHALL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_MARKET"),SOUND_EFFECT_OPEN_MARKET);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_BANK"),SOUND_EFFECT_OPEN_BANK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_HOUSE"),SOUND_EFFECT_OPEN_HOUSE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_STOCK"),SOUND_EFFECT_OPEN_STOCK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_POSTOFFICE"),SOUND_EFFECT_OPEN_POSTOFFICE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OPEN_HORSE"),SOUND_EFFECT_OPEN_HORSE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACK_MAIN"),SOUND_EFFECT_BACK_MAIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACK_SEAPALACE"),SOUND_EFFECT_BACK_SEAPALACE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACK_GOBLIN"),SOUND_EFFECT_BACK_GOBLIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACK_MIHWABGSA"),SOUND_EFFECT_BACK_MIHWABGSA);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_THUNDER_FIRE"),SOUND_EFFECT_MAGIC_THUNDER_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_DRAIN_FIRE"),SOUND_EFFECT_MAGIC_DRAIN_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_FLAME_FIRE"),SOUND_EFFECT_MAGIC_FLAME_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_BIGWIND_FIRE"),SOUND_EFFECT_MAGIC_BIGWIND_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_WHIRLPOOL_FIRE"),SOUND_EFFECT_MAGIC_WHIRLPOOL_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_STAFF_FIRE"),SOUND_EFFECT_MAGIC_STAFF_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_ICE_FIRE"),SOUND_EFFECT_MAGIC_ICE_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_ICE_HIT"),SOUND_EFFECT_MAGIC_ICE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_POISON_FIRE"),SOUND_EFFECT_MAGIC_POISON_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_POISON_HIT"),SOUND_EFFECT_MAGIC_POISON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_METEOR_FIRE"),SOUND_EFFECT_MAGIC_METEOR_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_METEOR_MULTI_FIRE"),SOUND_EFFECT_MAGIC_METEOR_MULTI_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_LASERBEAM_FIRE"),SOUND_EFFECT_MAGIC_LASERBEAM_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_SWORDBOMB_FIRE"),SOUND_EFFECT_MAGIC_SWORDBOMB_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_NORTHWIND_FIRE"),SOUND_EFFECT_MAGIC_NORTHWIND_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAGIC_FROZEN_FIRE"),SOUND_EFFECT_MAGIC_FROZEN_FIRE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_MASTER"),SOUND_EFFECT_MAKE_MASTER);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_ARMOUR"),SOUND_EFFECT_MAKE_ARMOUR);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_CERAMIC"),SOUND_EFFECT_MAKE_CERAMIC);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_FOOD"),SOUND_EFFECT_MAKE_FOOD);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_MEDICAL"),SOUND_EFFECT_MAKE_MEDICAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_WEAPON"),SOUND_EFFECT_MAKE_WEAPON);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_ORNAMENT"),SOUND_EFFECT_MAKE_ORNAMENT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_ETC"),SOUND_EFFECT_MAKE_ETC);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MAKE_LUCKY"),SOUND_EFFECT_MAKE_LUCKY);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_FIRE_FIREBALL"),SOUND_EFFECT_BULLET_FIRE_FIREBALL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_HIT_FIREBALL"),SOUND_EFFECT_BULLET_HIT_FIREBALL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_FIRE_ARROW"),SOUND_EFFECT_BULLET_FIRE_ARROW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_HIT_ARROW"),SOUND_EFFECT_BULLET_HIT_ARROW);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_FIRE_GUN"),SOUND_EFFECT_BULLET_FIRE_GUN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_HIT_GUN"),SOUND_EFFECT_BULLET_HIT_GUN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_FIRE_CAKE"),SOUND_EFFECT_BULLET_FIRE_CAKE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_HIT_CAKE"),SOUND_EFFECT_BULLET_HIT_CAKE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULLET_HIT_CANNON"),SOUND_EFFECT_BULLET_HIT_CANNON);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HERO_HIT"),SOUND_EFFECT_HERO_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HERO_ATTACK"),SOUND_EFFECT_HERO_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HERO_DIE"),SOUND_EFFECT_HERO_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HERO_MOVING"),SOUND_EFFECT_HERO_MOVING);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEROINE_HIT"),SOUND_EFFECT_HEROINE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEROINE_ATTACK"),SOUND_EFFECT_HEROINE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEROINE_DIE"),SOUND_EFFECT_HEROINE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HERO2_DIE"),SOUND_EFFECT_HERO2_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RABBIT_HIT"),SOUND_EFFECT_RABBIT_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RABBIT_ATTACK"),SOUND_EFFECT_RABBIT_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RABBIT_DIE"),SOUND_EFFECT_RABBIT_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TIGER_HIT"),SOUND_EFFECT_TIGER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TIGER_DIE"),SOUND_EFFECT_TIGER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CANNON_HIT"),SOUND_EFFECT_CANNON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CANNON_ATTACK"),SOUND_EFFECT_CANNON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CANNON_DIE"),SOUND_EFFECT_CANNON_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RAT_HIT"),SOUND_EFFECT_RAT_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RAT_ATTACK"),SOUND_EFFECT_RAT_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RAT_DIE"),SOUND_EFFECT_RAT_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RACCOON_HIT"),SOUND_EFFECT_RACCOON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RACCOON_ATTACK"),SOUND_EFFECT_RACCOON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_RACCOON_DIE"),SOUND_EFFECT_RACCOON_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MUSHROOM_HIT"),SOUND_EFFECT_MUSHROOM_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MUSHROOM_ATTACK"),SOUND_EFFECT_MUSHROOM_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MUSHROOM_DIE"),SOUND_EFFECT_MUSHROOM_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FLOWERPIG_HIT"),SOUND_EFFECT_FLOWERPIG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FLOWERPIG_ATTACK"),SOUND_EFFECT_FLOWERPIG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FLOWERPIG_DIE"),SOUND_EFFECT_FLOWERPIG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COW_HIT"),SOUND_EFFECT_COW_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COW_ATTACK"),SOUND_EFFECT_COW_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COW_DIE"),SOUND_EFFECT_COW_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YAIN_HIT"),SOUND_EFFECT_YAIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YAIN_ATTACK"),SOUND_EFFECT_YAIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YAIN_DIE"),SOUND_EFFECT_YAIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WILDBOAR_HIT"),SOUND_EFFECT_WILDBOAR_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WILDBOAR_ATTACK"),SOUND_EFFECT_WILDBOAR_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WILDBOAR_DIE"),SOUND_EFFECT_WILDBOAR_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BEARLADY_HIT"),SOUND_EFFECT_BEARLADY_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BEARLADY_ATTACK"),SOUND_EFFECT_BEARLADY_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BEARLADY_DIE"),SOUND_EFFECT_BEARLADY_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLIC_HIT"),SOUND_EFFECT_GARLIC_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLIC_ATTACK"),SOUND_EFFECT_GARLIC_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLIC_DIE"),SOUND_EFFECT_GARLIC_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHICKEN_HIT"),SOUND_EFFECT_CHICKEN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHICKEN_ATTACK"),SOUND_EFFECT_CHICKEN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHICKEN_DIE"),SOUND_EFFECT_CHICKEN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COMMANDER_HIT"),SOUND_EFFECT_COMMANDER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COMMANDER_ATTACK"),SOUND_EFFECT_COMMANDER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_COMMANDER_DIE"),SOUND_EFFECT_COMMANDER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ARCHER_HIT"),SOUND_EFFECT_ARCHER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ARCHER_ATTACK"),SOUND_EFFECT_ARCHER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ARCHER_DIE"),SOUND_EFFECT_ARCHER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DRINKER_HIT"),SOUND_EFFECT_DRINKER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DRINKER_ATTACK"),SOUND_EFFECT_DRINKER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DRINKER_DIE"),SOUND_EFFECT_DRINKER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TWISTERM_HIT"),SOUND_EFFECT_TWISTERM_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TWISTERM_ATTACK"),SOUND_EFFECT_TWISTERM_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TWISTERM_DIE"),SOUND_EFFECT_TWISTERM_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CATFISH_HIT"),SOUND_EFFECT_CATFISH_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CATFISH_ATTACK"),SOUND_EFFECT_CATFISH_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CATFISH_DIE"),SOUND_EFFECT_CATFISH_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TURTLE_HIT"),SOUND_EFFECT_TURTLE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TURTLE_ATTACK"),SOUND_EFFECT_TURTLE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TURTLE_DIE"),SOUND_EFFECT_TURTLE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOLDIER_HIT"),SOUND_EFFECT_SOLDIER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOLDIER_ATTACK"),SOUND_EFFECT_SOLDIER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOLDIER_DIE"),SOUND_EFFECT_SOLDIER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOCUST_HIT"),SOUND_EFFECT_LOCUST_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOCUST_ATTACK"),SOUND_EFFECT_LOCUST_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOCUST_DIE"),SOUND_EFFECT_LOCUST_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HORSEMA_HIT"),SOUND_EFFECT_HORSEMA_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HORSEMA_ATTACK"),SOUND_EFFECT_HORSEMA_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HORSEMA_DIE"),SOUND_EFFECT_HORSEMA_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LADYBUG_HIT"),SOUND_EFFECT_LADYBUG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LADYBUG_ATTACK"),SOUND_EFFECT_LADYBUG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LADYBUG_DIE"),SOUND_EFFECT_LADYBUG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BLUEFLY_HIT"),SOUND_EFFECT_BLUEFLY_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BLUEFLY_ATTACK"),SOUND_EFFECT_BLUEFLY_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BLUEFLY_DIE"),SOUND_EFFECT_BLUEFLY_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGOBLIN_HIT"),SOUND_EFFECT_MINIGOBLIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGOBLIN_ATTACK"),SOUND_EFFECT_MINIGOBLIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGOBLIN_DIE"),SOUND_EFFECT_MINIGOBLIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SICKTIGER_HIT"),SOUND_EFFECT_SICKTIGER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SICKTIGER_ATTACK"),SOUND_EFFECT_SICKTIGER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SICKTIGER_DIE"),SOUND_EFFECT_SICKTIGER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOLDBEETLE_HIT"),SOUND_EFFECT_GOLDBEETLE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOLDBEETLE_ATTACK"),SOUND_EFFECT_GOLDBEETLE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOLDBEETLE_DIE"),SOUND_EFFECT_GOLDBEETLE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOLDBEETLE_STONE"),SOUND_EFFECT_GOLDBEETLE_STONE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIGHTSOLDIER_HIT"),SOUND_EFFECT_LIGHTSOLDIER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIGHTSOLDIER_ATTACK"),SOUND_EFFECT_LIGHTSOLDIER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIGHTSOLDIER_DIE"),SOUND_EFFECT_LIGHTSOLDIER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WAITINGMAN_SELECT"),SOUND_EFFECT_WAITINGMAN_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ROYALGUARD_SELECT"),SOUND_EFFECT_ROYALGUARD_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JANGGUMI_SELECT"),SOUND_EFFECT_JANGGUMI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WAITINGWOMAN_SELECT"),SOUND_EFFECT_WAITINGWOMAN_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SPIDERMON_HIT"),SOUND_EFFECT_SPIDERMON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SPIDERMON_ATTACK"),SOUND_EFFECT_SPIDERMON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SPIDERMON_DIE"),SOUND_EFFECT_SPIDERMON_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWMAN_HIT"),SOUND_EFFECT_SNOWMAN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWMAN_ATTACK"),SOUND_EFFECT_SNOWMAN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWMAN_DIE"),SOUND_EFFECT_SNOWMAN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOYO_HIT"),SOUND_EFFECT_TOYO_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOYO_ATTACK"),SOUND_EFFECT_TOYO_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOYO_DIE"),SOUND_EFFECT_TOYO_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWELF_HIT"),SOUND_EFFECT_SNOWELF_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWELF_ATTACK"),SOUND_EFFECT_SNOWELF_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SNOWELF_DIE"),SOUND_EFFECT_SNOWELF_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MADOGHOST_HIT"),SOUND_EFFECT_MADOGHOST_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MADOGHOST_ATTACK"),SOUND_EFFECT_MADOGHOST_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MADOGHOST_DIE"),SOUND_EFFECT_MADOGHOST_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREGOBLIN_HIT"),SOUND_EFFECT_FIREGOBLIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREGOBLIN_ATTACK"),SOUND_EFFECT_FIREGOBLIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREGOBLIN_DIE"),SOUND_EFFECT_FIREGOBLIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INDIACANNON_HIT"),SOUND_EFFECT_INDIACANNON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INDIACANNON_ATTACK"),SOUND_EFFECT_INDIACANNON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INDIACANNON_DIE"),SOUND_EFFECT_INDIACANNON_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANCOMMANDER_HIT"),SOUND_EFFECT_JAPANCOMMANDER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANCOMMANDER_ATTACK"),SOUND_EFFECT_JAPANCOMMANDER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANCOMMANDER_DIE"),SOUND_EFFECT_JAPANCOMMANDER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEYBOXER_HIT"),SOUND_EFFECT_MONKEYBOXER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEYBOXER_ATTACK"),SOUND_EFFECT_MONKEYBOXER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEYBOXER_DIE"),SOUND_EFFECT_MONKEYBOXER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANHEALER_HIT"),SOUND_EFFECT_JAPANHEALER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANHEALER_ATTACK"),SOUND_EFFECT_JAPANHEALER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANHEALER_DIE"),SOUND_EFFECT_JAPANHEALER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOUTHGUARDIAN_HIT"),SOUND_EFFECT_SOUTHGUARDIAN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOUTHGUARDIAN_ATTACK"),SOUND_EFFECT_SOUTHGUARDIAN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SOUTHGUARDIAN_DIE"),SOUND_EFFECT_SOUTHGUARDIAN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EASTGUARDIAN_HIT"),SOUND_EFFECT_EASTGUARDIAN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EASTGUARDIAN_ATTACK"),SOUND_EFFECT_EASTGUARDIAN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_EASTGUARDIAN_DIE"),SOUND_EFFECT_EASTGUARDIAN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANPORTERSOL_HIT"),SOUND_EFFECT_JAPANPORTERSOL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANPORTERSOL_ATTACK"),SOUND_EFFECT_JAPANPORTERSOL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANPORTERSOL_DIE"),SOUND_EFFECT_JAPANPORTERSOL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VIRGINGHOST_HIT"),SOUND_EFFECT_VIRGINGHOST_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VIRGINGHOST_ATTACK"),SOUND_EFFECT_VIRGINGHOST_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VIRGINGHOST_DIE"),SOUND_EFFECT_VIRGINGHOST_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UGLYPILLAR_HIT"),SOUND_EFFECT_UGLYPILLAR_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UGLYPILLAR_ATTACK"),SOUND_EFFECT_UGLYPILLAR_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UGLYPILLAR_DIE"),SOUND_EFFECT_UGLYPILLAR_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOHANG_HIT"),SOUND_EFFECT_TOHANG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOHANG_ATTACK"),SOUND_EFFECT_TOHANG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TOHANG_DIE"),SOUND_EFFECT_TOHANG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SKULLBAT_HIT"),SOUND_EFFECT_SKULLBAT_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SKULLBAT_ATTACK"),SOUND_EFFECT_SKULLBAT_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SKULLBAT_DIE"),SOUND_EFFECT_SKULLBAT_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREMAN_HIT"),SOUND_EFFECT_FIREMAN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREMAN_ATTACK"),SOUND_EFFECT_FIREMAN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREMAN_DIE"),SOUND_EFFECT_FIREMAN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GRPAPAGOBLIN_HIT"),SOUND_EFFECT_GRPAPAGOBLIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GRPAPAGOBLIN_ATTACK"),SOUND_EFFECT_GRPAPAGOBLIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GRPAPAGOBLIN_DIE"),SOUND_EFFECT_GRPAPAGOBLIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BONGDAL_SELECT"),SOUND_EFFECT_BONGDAL_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BONGSAMI_SELECT"),SOUND_EFFECT_BONGSAMI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BONGSUNI_SELECT"),SOUND_EFFECT_BONGSUNI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BONG_SELECT"),SOUND_EFFECT_BONG_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BONGTAGI_SELECT"),SOUND_EFFECT_BONGTAGI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAPANMERCHANT_SELECT"),SOUND_EFFECT_JAPANMERCHANT_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YONGSAMI_SELECT"),SOUND_EFFECT_YONGSAMI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YONGCHUNI_SELECT"),SOUND_EFFECT_YONGCHUNI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YONGTAGI_SELECT"),SOUND_EFFECT_YONGTAGI_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YONGPAL_SELECT"),SOUND_EFFECT_YONGPAL_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINAMERCHANT_SELECT"),SOUND_EFFECT_CHINAMERCHANT_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DOCTOR_SELECT"),SOUND_EFFECT_DOCTOR_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KING_LOGIN"),SOUND_EFFECT_KING_LOGIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HORSE_HUNGRY"),SOUND_EFFECT_HORSE_HUNGRY);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HORSE_MOVE"),SOUND_EFFECT_HORSE_MOVE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FANPRIEST_WARP"),SOUND_EFFECT_FANPRIEST_WARP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_ELEPHANTSANG_ATTACK"),SOUND_EFFECT_ELEPHANTSANG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DRAGONKING_ATTACK"),SOUND_EFFECT_DRAGONKING_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOMG_ATTACK"),SOUND_EFFECT_GOMG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOMG_DIE"),SOUND_EFFECT_GOMG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GOMG_SUMMON"),SOUND_EFFECT_GOMG_SUMMON);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STAGBEETLE_ATTACK"),SOUND_EFFECT_STAGBEETLE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAILYQUEST_OK"),SOUND_EFFECT_DAILYQUEST_OK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS"),SOUND_EFFECT_DAILYQUEST_SUCCESS);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAILYQUEST_FAIL"),SOUND_EFFECT_DAILYQUEST_FAIL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP"),SOUND_EFFECT_DAILYQUEST_GIVEUP);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FRIEND_LOGIN"),SOUND_EFFECT_FRIEND_LOGIN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREDRAGON_ATTACK"),SOUND_EFFECT_FIREDRAGON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLUBCAR_HIT"),SOUND_EFFECT_CLUBCAR_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLUBCAR_ATTACK"),SOUND_EFFECT_CLUBCAR_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CLUBCAR_DIE"),SOUND_EFFECT_CLUBCAR_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAGGERCRIMINAL_HIT"),SOUND_EFFECT_DAGGERCRIMINAL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAGGERCRIMINAL_ATTACK"),SOUND_EFFECT_DAGGERCRIMINAL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DAGGERCRIMINAL_DIE"),SOUND_EFFECT_DAGGERCRIMINAL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLICTANK_HIT"),SOUND_EFFECT_GARLICTANK_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLICTANK_ATTACK"),SOUND_EFFECT_GARLICTANK_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GARLICTANK_DIE"),SOUND_EFFECT_GARLICTANK_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UNIVERSALGENERAL_HIT"),SOUND_EFFECT_UNIVERSALGENERAL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UNIVERSALGENERAL_ATTACK"),SOUND_EFFECT_UNIVERSALGENERAL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_UNIVERSALGENERAL_DIE"),SOUND_EFFECT_UNIVERSALGENERAL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAR_HIT"),SOUND_EFFECT_JAR_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAR_ATTACK"),SOUND_EFFECT_JAR_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JAR_DIE"),SOUND_EFFECT_JAR_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STONECOW_HIT"),SOUND_EFFECT_STONECOW_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STONECOW_ATTACK"),SOUND_EFFECT_STONECOW_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STONECOW_DIE"),SOUND_EFFECT_STONECOW_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHARK_HIT"),SOUND_EFFECT_SHARK_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHARK_ATTACK"),SOUND_EFFECT_SHARK_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHARK_DIE"),SOUND_EFFECT_SHARK_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OCTOPUS_HIT"),SOUND_EFFECT_OCTOPUS_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OCTOPUS_ATTACK"),SOUND_EFFECT_OCTOPUS_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OCTOPUS_DIE"),SOUND_EFFECT_OCTOPUS_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRAB_HIT"),SOUND_EFFECT_CRAB_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRAB_ATTACK"),SOUND_EFFECT_CRAB_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRAB_DIE"),SOUND_EFFECT_CRAB_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TONGUEGIRL_HIT"),SOUND_EFFECT_TONGUEGIRL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TONGUEGIRL_ATTACK"),SOUND_EFFECT_TONGUEGIRL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TONGUEGIRL_DIE"),SOUND_EFFECT_TONGUEGIRL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YETI_HIT"),SOUND_EFFECT_YETI_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YETI_ATTACK"),SOUND_EFFECT_YETI_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YETI_DIE"),SOUND_EFFECT_YETI_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MASKMAN_HIT"),SOUND_EFFECT_MASKMAN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MASKMAN_ATTACK"),SOUND_EFFECT_MASKMAN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MASKMAN_DIE"),SOUND_EFFECT_MASKMAN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CASTLE_ATTACK"),SOUND_EFFECT_CASTLE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TREEMON_HIT"),SOUND_EFFECT_TREEMON_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TREEMON_ATTACK"),SOUND_EFFECT_TREEMON_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TREEMON_DIE"),SOUND_EFFECT_TREEMON_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VINE_HIT"),SOUND_EFFECT_VINE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VINE_ATTACK"),SOUND_EFFECT_VINE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_VINE_DIE"),SOUND_EFFECT_VINE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULGASARI_ATTACK"),SOUND_EFFECT_BULGASARI_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BULGASARI_DIE"),SOUND_EFFECT_BULGASARI_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SASURI_HIT"),SOUND_EFFECT_SASURI_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SASURI_ATTACK"),SOUND_EFFECT_SASURI_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SASURI_DIE"),SOUND_EFFECT_SASURI_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACHELORGHOST_HIT"),SOUND_EFFECT_BACHELORGHOST_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACHELORGHOST_ATTACK"),SOUND_EFFECT_BACHELORGHOST_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BACHELORGHOST_DIE"),SOUND_EFFECT_BACHELORGHOST_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FRUITBROS_HIT"),SOUND_EFFECT_FRUITBROS_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FRUITBROS_ATTACK"),SOUND_EFFECT_FRUITBROS_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FRUITBROS_DIE"),SOUND_EFFECT_FRUITBROS_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHELLFISH_HIT"),SOUND_EFFECT_SHELLFISH_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHELLFISH_ATTACK"),SOUND_EFFECT_SHELLFISH_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SHELLFISH_DIE"),SOUND_EFFECT_SHELLFISH_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SIMTORYONG_HIT"),SOUND_EFFECT_SIMTORYONG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SIMTORYONG_ATTACK"),SOUND_EFFECT_SIMTORYONG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SIMTORYONG_DIE"),SOUND_EFFECT_SIMTORYONG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINASPEARSOL_HIT"),SOUND_EFFECT_CHINASPEARSOL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINASPEARSOL_ATTACK"),SOUND_EFFECT_CHINASPEARSOL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINASPEARSOL_DIE"),SOUND_EFFECT_CHINASPEARSOL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINACANNONSOL_HIT"),SOUND_EFFECT_CHINACANNONSOL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINACANNONSOL_ATTACK"),SOUND_EFFECT_CHINACANNONSOL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINACANNONSOL_DIE"),SOUND_EFFECT_CHINACANNONSOL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINABOWSOL_HIT"),SOUND_EFFECT_CHINABOWSOL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINABOWSOL_ATTACK"),SOUND_EFFECT_CHINABOWSOL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHINABOWSOL_DIE"),SOUND_EFFECT_CHINABOWSOL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BUTCHER_HIT"),SOUND_EFFECT_BUTCHER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BUTCHER_ATTACK"),SOUND_EFFECT_BUTCHER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BUTCHER_DIE"),SOUND_EFFECT_BUTCHER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRANE_HIT"),SOUND_EFFECT_CRANE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRANE_ATTACK"),SOUND_EFFECT_CRANE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CRANE_DIE"),SOUND_EFFECT_CRANE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEY_HIT"),SOUND_EFFECT_MONKEY_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEY_ATTACK"),SOUND_EFFECT_MONKEY_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MONKEY_DIE"),SOUND_EFFECT_MONKEY_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DUNGPIG_HIT"),SOUND_EFFECT_DUNGPIG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DUNGPIG_ATTACK"),SOUND_EFFECT_DUNGPIG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DUNGPIG_DIE"),SOUND_EFFECT_DUNGPIG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WHALE_HIT"),SOUND_EFFECT_WHALE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WHALE_ATTACK"),SOUND_EFFECT_WHALE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WHALE_DIE"),SOUND_EFFECT_WHALE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCUTTLEFISH_HIT"),SOUND_EFFECT_KINGCUTTLEFISH_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCUTTLEFISH_ATTACK"),SOUND_EFFECT_KINGCUTTLEFISH_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCUTTLEFISH_DIE"),SOUND_EFFECT_KINGCUTTLEFISH_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OGONG_HIT"),SOUND_EFFECT_OGONG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OGONG_ATTACK"),SOUND_EFFECT_OGONG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_OGONG_DIE"),SOUND_EFFECT_OGONG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_CLICK"),SOUND_EFFECT_MINIGAME_MONSTERCORP_CLICK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_MOVE"),SOUND_EFFECT_MINIGAME_MONSTERCORP_MOVE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_DOWN"),SOUND_EFFECT_MINIGAME_MONSTERCORP_DOWN);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS1"),SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS1);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS2"),SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS2);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS3"),SOUND_EFFECT_MINIGAME_MONSTERCORP_SUCCESS3);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_FAIL"),SOUND_EFFECT_MINIGAME_MONSTERCORP_FAIL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MINIGAME_MONSTERCORP_SPECIAL"),SOUND_EFFECT_MINIGAME_MONSTERCORP_SPECIAL);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YISOONSIN_HIT"),SOUND_EFFECT_YISOONSIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YISOONSIN_ATTACK"),SOUND_EFFECT_YISOONSIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_YISOONSIN_DIE"),SOUND_EFFECT_YISOONSIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GWONYOOL_HIT"),SOUND_EFFECT_GWONYOOL_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GWONYOOL_ATTACK"),SOUND_EFFECT_GWONYOOL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_GWONYOOL_DIE"),SOUND_EFFECT_GWONYOOL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JANGBOGO_HIT"),SOUND_EFFECT_JANGBOGO_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JANGBOGO_ATTACK"),SOUND_EFFECT_JANGBOGO_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_JANGBOGO_DIE"),SOUND_EFFECT_JANGBOGO_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SAMYOUNGSAINT_HIT"),SOUND_EFFECT_SAMYOUNGSAINT_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SAMYOUNGSAINT_ATTACK"),SOUND_EFFECT_SAMYOUNGSAINT_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_SAMYOUNGSAINT_DIE"),SOUND_EFFECT_SAMYOUNGSAINT_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHOIMOOSUN_HIT"),SOUND_EFFECT_CHOIMOOSUN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHOIMOOSUN_ATTACK"),SOUND_EFFECT_CHOIMOOSUN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CHOIMOOSUN_DIE"),SOUND_EFFECT_CHOIMOOSUN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCRAB_HIT"),SOUND_EFFECT_KINGCRAB_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCRAB_ATTACK"),SOUND_EFFECT_KINGCRAB_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_KINGCRAB_DIE"),SOUND_EFFECT_KINGCRAB_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DOMAFIG_HIT"),SOUND_EFFECT_DOMAFIG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DOMAFIG_ATTACK"),SOUND_EFFECT_DOMAFIG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DOMAFIG_DIE"),SOUND_EFFECT_DOMAFIG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HUNGRYGOBLIN_HIT"),SOUND_EFFECT_HUNGRYGOBLIN_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HUNGRYGOBLIN_ATTACK"),SOUND_EFFECT_HUNGRYGOBLIN_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HUNGRYGOBLIN_DIE"),SOUND_EFFECT_HUNGRYGOBLIN_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOBSTER_HIT"),SOUND_EFFECT_LOBSTER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOBSTER_ATTACK"),SOUND_EFFECT_LOBSTER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LOBSTER_DIE"),SOUND_EFFECT_LOBSTER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREHAETAE_HIT"),SOUND_EFFECT_FIREHAETAE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREHAETAE_ATTACK"),SOUND_EFFECT_FIREHAETAE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FIREHAETAE_DIE"),SOUND_EFFECT_FIREHAETAE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIONMASK_HIT"),SOUND_EFFECT_LIONMASK_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIONMASK_ATTACK"),SOUND_EFFECT_LIONMASK_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LIONMASK_DIE"),SOUND_EFFECT_LIONMASK_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TRIPLETMASK_HIT"),SOUND_EFFECT_TRIPLETMASK_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TRIPLETMASK_ATTACK"),SOUND_EFFECT_TRIPLETMASK_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_TRIPLETMASK_DIE"),SOUND_EFFECT_TRIPLETMASK_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DEADMESSENGER_HIT"),SOUND_EFFECT_DEADMESSENGER_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DEADMESSENGER_ATTACK"),SOUND_EFFECT_DEADMESSENGER_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_DEADMESSENGER_DIE"),SOUND_EFFECT_DEADMESSENGER_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CORNUCOW_HIT"),SOUND_EFFECT_CORNUCOW_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CORNUCOW_ATTACK"),SOUND_EFFECT_CORNUCOW_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_CORNUCOW_DIE"),SOUND_EFFECT_CORNUCOW_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LADYGENERAL_ATTACK"),SOUND_EFFECT_LADYGENERAL_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_LADYGENERAL_DIE"),SOUND_EFFECT_LADYGENERAL_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEDGEHOG_HIT"),SOUND_EFFECT_HEDGEHOG_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEDGEHOG_ATTACK"),SOUND_EFFECT_HEDGEHOG_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_HEDGEHOG_DIE"),SOUND_EFFECT_HEDGEHOG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STRETCHTREE_HIT"),SOUND_EFFECT_STRETCHTREE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STRETCHTREE_ATTACK"),SOUND_EFFECT_STRETCHTREE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_STRETCHTREE_DIE"),SOUND_EFFECT_STRETCHTREE_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MORTAR_HIT"),SOUND_EFFECT_MORTAR_HIT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MORTAR_ATTACK"),SOUND_EFFECT_MORTAR_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_MORTAR_DIE"),SOUND_EFFECT_MORTAR_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INSTANCEBOSS_ATTACK"),SOUND_EFFECT_INSTANCEBOSS_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INSTANCEBOSS_DIE"),SOUND_EFFECT_INSTANCEBOSS_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_BLACKWING_DIE"),SOUND_EFFECT_BLACKWING_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_INHWASANG_DIE"),SOUND_EFFECT_INHWASANG_DIE);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_WAITINGMAN3_SELECT"),SOUND_EFFECT_WAITINGMAN3_SELECT);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_0"),SOUND_EFFECT_FEAST_KIND_0);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_1"),SOUND_EFFECT_FEAST_KIND_1);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_2"),SOUND_EFFECT_FEAST_KIND_2);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_3"),SOUND_EFFECT_FEAST_KIND_3);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_4"),SOUND_EFFECT_FEAST_KIND_4);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_5"),SOUND_EFFECT_FEAST_KIND_5);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_6"),SOUND_EFFECT_FEAST_KIND_6);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_7"),SOUND_EFFECT_FEAST_KIND_7);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_8"),SOUND_EFFECT_FEAST_KIND_8);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_9"),SOUND_EFFECT_FEAST_KIND_9);
	pclClient->pclHashTableString->Insert(TEXT("SOUND_EFFECT_FEAST_KIND_10"),SOUND_EFFECT_FEAST_KIND_10);
	pclClient->pclHashTableString->Insert(TEXT("TOTAL_SOUND_EFFECT_TYPE_NUMBER"),TOTAL_SOUND_EFFECT_TYPE_NUMBER);
	*/

	pclGame = pclgame;

	EffectBufferIndex	= 0;

	pclSoundManager = new CSoundMgr(); 

	pclSoundManager->Create( pclgame->GetHwnd() );

	InitEffectFile();
}

cltMusic::~cltMusic()
{


	FreeEffect();

	if( pclSoundManager )
	{
		delete pclSoundManager;
		pclSoundManager = NULL;
	}

}


void cltMusic::InitEffectFile()
{
	SI32 totaleffect = pclClient->GetUniqueFromHash(TEXT("TOTAL_SOUND_EFFECT_TYPE_NUMBER")) ;
	SI32 maxeffect = MAX_SOUND_EFFECT_TYPE_NUMBER ;

	if ( totaleffect > maxeffect )
	{
		MsgBox(TEXT("EffectTypeBufferOver"), TEXT("TotalEffectTypeBuffer:%d"),pclClient->GetUniqueFromHash(TEXT("TOTAL_SOUND_EFFECT_TYPE_NUMBER")) );
	}

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile(  TEXT("Data/SoundInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(  TEXT("Data/SoundInfo.txt") );
	}
	
	if (!bLoadFile)
	{
		MsgBox(TEXT("Not LoadFromFile SoundInfo"),TEXT("Error"));
	}

	TCHAR type[128] = TEXT("");
	TCHAR path[128] = TEXT("");

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_MBSTRING,	type, 128,		
			NDATA_MBSTRING,	path, 128,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		SI32 index;

		if( dataloader.ReadData() ) {

			if ( type[ 0 ] != NULL)
			{
				if (pclClient->pclHashTableString->Find(type, index))
				{
					ED[index].Set(path);
				}
				else
				{
					TCHAR buffer[ 128 ];
					StringCchPrintf( buffer, 128, TEXT("Not Found SoundInfo in HashTable : %s"), type );
					MsgBox( buffer, TEXT("Error"));
				}
			}
		}
	}
}

void cltMusic::FreeEffect()
{
	SHORT i;
	
	for(i=0;i<MAX_SOUND_EFFECT_TYPE_NUMBER;i++)
	{
		ED[i].Free();
	}
}

BOOL cltMusic::PushEffect(SHORT effect, SHORT x, SHORT y)
{
	if( EffectBufferIndex >= MAX_SOUND_EFFECT_BUFFER_NUMBER )		return FALSE;

	EffectBuffer[EffectBufferIndex++].Set(effect, x, y);
	return TRUE;
}

// 버퍼에 저장된 소리를 하나 꺼내서 연주한다. 
void cltMusic::PopEffect(cltMapClient* pclMap)
{
	SHORT	i;

	if( EffectBufferIndex <= 0 )	
		return;

	// 버퍼에 저장된 것을 꺼낸다. 
	SHORT effect = EffectBuffer[0].Effect;
	SHORT x = EffectBuffer[0].X;
	SHORT y = EffectBuffer[0].Y;

	for( i=1; i<EffectBufferIndex; i++ )
	{
		EffectBuffer[i-1] = EffectBuffer[i];
	}
	EffectBufferIndex--;


	int sx = 0, sy = 0;
	if( pclGame->GetGameStatus() == GS_GAME )
	{
		// 화면 중심과 주어진 좌표와의 상대적 거리를 구한다. 
		if(x == 0 && y == 0)
		{
			sx = 0;
			sy = 0;
		}
		else
		{
			sx=(x-pclMap->VirtualMapX);
			sy=(y-pclMap->VirtualMapY);
		}

		// 위치가 주어진 소리인데 거리가 멀면 소리내지 않는다. 
		if( x && y && (TABS(sx) > 17 || TABS(sy) > 17) )	return;
	}

	bool bgamemode;
	if( pclGame->GetGameStatus() == GS_GAME )
	{
		bgamemode = true;
	}
	else
	{
		bgamemode = false;
	}

/*	TCHAR buffer[256];
	sprintf(buffer, TEXT("%d %d"), sx, sy);
	pclClient->pclMessage->SetMsg(buffer);
*/

	TCHAR* pname = ED[effect].Play(x, y, pclMap, bgamemode);


	if(pname)
	{
		TCHAR  buffer[256];
		bool SoundKind = FALSE;
		float fpos = sx * 1.0f;

		StringCchPrintf(buffer, 256, TEXT("%s.wav"), pname);

		

		// SoundInfo_Unique파일 참조할것. 주인공 타격음일때만 
		if(effect >=1 && effect <= 4)
		{
			SoundKind = TRUE;
		}

		if(SoundKind)
		{
			pclSoundManager->PlayFile(buffer,false,true,fpos,1.0f,true);
		}
		else
		{		
			pclSoundManager->PlayFile(buffer,false,false,fpos);
		}

		//if ( sx < 0 )
		//{
		//	pclSoundManager->PlayFile( 
		//		buffer,
		//		false,/*사운드 루프*/
		//		false,/*기존 사운드 출력중이면 그냥 둠*/
		//		(float)( TSQRT( sx*sx + sy *sy ) * -0.25f )
		//		);
		//}
		//else
		//{
		//	pclSoundManager->PlayFile(
		//		buffer, 
		//		false,/*사운드 루프*/
		//		false,/*기존 사운드 출력중이면 그냥 둠*/
		//		(float)(TSQRT( sx*sx + sy *sy ) * 0.25f )
		//		);
		//}
	}
}

