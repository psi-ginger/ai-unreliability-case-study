#include "..\Client\Client.h"

#include "smoke.h"
#include "Char\CharAni\CharAni.h"
#include "Map\Map.h"

cltSmokeManager::cltSmokeManager(SI32 gamemode)
{
	SI32 i;

	InitSmokeKindInfo();

	for(i = 0;i < MAX_SMOKE_NUMBER;i++)
	{
		SmokeInfo[i]	= 0;
	}
	
	if(gamemode == GAMEMODE_SERVER)
	{
		for(i = 0;i < MAX_SMOKE_NUMBER;i++)
		{
			pclSmoke[i]		= new cltSmokeServer;
		}
	}
	else
	{
		for(i = 0;i < MAX_SMOKE_NUMBER;i++)
		{
			pclSmoke[i]		= new cltSmokeClient;
		}

	}

}

cltSmokeManager::~cltSmokeManager()
{
	SI32 i;

	for(i = 0;i < MAX_SMOKE_NUMBER;i++)
	{
		if(pclSmoke[i])
		{
			delete pclSmoke[i];
			pclSmoke[i] = NULL;
		}
	}

}


void cltSmokeManager::InitSmokeKindInfo()
{
	
	SKI[SMOKE_GUN].Init(					TEXT("Smoke\\Smoke_Gun.Spr"),					0,	8,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_SINK].Init(					TEXT("Smoke\\CannonExp.Spr"),					0,	14,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_EXP1].Init(					TEXT("Smoke\\CannonExp.Spr"),					0,	18,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_EXP2].Init(					TEXT("Smoke\\CannonExp.Spr"),					0,	3,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_GROUNDFIRE].Init(				TEXT("Smoke\\CannonExp.Spr"),					0,	36,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	32 );
	SKI[SMOKE_KIMBOMB1].Init(				TEXT("Smoke\\CannonExp.Spr"),					0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_KIMBOMB2].Init(				TEXT("Smoke\\CannonExp.Spr"),					16,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_KIMBOMB3].Init(				TEXT("Smoke\\CannonExp.Spr"),					32,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_LIGHTNING].Init(				TEXT("Smoke\\CannonExp.Spr"),					0,	7,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	320 );
	SKI[SMOKE_GHOST].Init(					TEXT("Smoke\\Ghost.Spr"),						0,	22,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_HORSEDUST].Init(				TEXT("Smoke\\HorseDust.Spr"),					0,	8,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_HALFTRANS,	0,	0 );
	SKI[SMOKE_CANNONEXP].Init(				TEXT("Smoke\\CannonExp.Spr"),					0,	15,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_FIRESMOKE].Init(				TEXT("Smoke\\FireSmoke.Spr"),					0,	6,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_OLDTRANS,	0,	0 );
	SKI[SMOKE_RAREITEMSMOKE1].Init(			TEXT("Smoke\\Rareitemsmoke.Spr"),				0,	14,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_RAREITEMSMOKE2].Init(			TEXT("Smoke\\RereItemSmoke.Spr"),				8,	8,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_RAREITEMSMOKE3].Init(			TEXT("Smoke\\RereItemSmoke.Spr"),				16,	8,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_RAREITEMSMOKE4].Init(			TEXT("Smoke\\RereItemSmoke.Spr"),				24,	8,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_FIREBALLHIT].Init(			TEXT("Smoke\\FireBallHitSmoke.spr"),			0,	18,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_CASTLEBOOM].Init(				TEXT("Smoke\\CastleBoom.spr"),					0,	14,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_SELFDESTUCT].Init(			TEXT("Smoke\\SelfDestruction.spr"),				0,	21,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_ICESMOKE].Init(				TEXT("Smoke\\IceSmoke.spr"),					0,	20,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_ADD,		0,	0 );
	SKI[SMOKE_JARSMOKE].Init(				TEXT("Smoke\\JarSmoke.spr"),					0,	19,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_WINDBLADE].Init(				TEXT("Smoke\\WindBladeSmoke.spr"),				0,	10,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_TREEMON].Init(				TEXT("Smoke\\TreeMon.spr"),						0,	6,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_VINE].Init(					TEXT("Smoke\\Vine.spr"),						0,	11,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );
	SKI[SMOKE_GHOSTFIRE].Init(				TEXT("Smoke\\GhostFire.spr"),					0,	10,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_DRAGONEXPLOSION].Init(		TEXT("Smoke\\Firedragon_explosion.SPR"),		0,	18,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_ADD,		0,	0 );
	SKI[SMOKE_DRAGONEXPTAIL].Init(			TEXT("Smoke\\Firedragon_tail.SPR"),				0,	9,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_MULTIPLY,	0,	0 );
	SKI[SMOKE_SIMTORYONGSTONE].Init(		TEXT("Smoke\\SimToRyong_Stone.SPR"),			0,	11,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	20 );
	SKI[SMOKE_KINGCUTTLEFISH].Init(			TEXT("Smoke\\Smoke_CuttleFish.SPR"),			0,	13,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_ADD,		0,	0 );

	SKI[SMOKE_BEGINWARP].Init(				TEXT("Smoke\\Smoke_BeginWarp.SPR"),				0,	22,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_ADD,		0,	0 );
	SKI[SMOKE_ENDWARP].Init(				TEXT("Smoke\\Smoke_EndWarp.SPR"),				0,	22,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	
	SKI[SMOKE_CRANE_TAIL].Init(				TEXT("Smoke\\Smoke_Crane_Tail.spr"),			0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_CRANE_HIT].Init(				TEXT("Smoke\\Smoke_Crane_Hit.SPR"),				0,	10,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_ADD,		0,	-35 );

	SKI[SMOKE_SELFBOMBWHALE].Init(			TEXT("Smoke\\SelfBombWhale.spr"),				0,	15,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_ADD,		0,	0 );

	SKI[SMOKE_THROWKNIFE_RIGHT].Init(		TEXT("Smoke\\Smoke_ThrowKnifeRight.spr"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_THROWKNIFE_LEFT].Init(		TEXT("Smoke\\Smoke_ThrowKnifeLeft.spr"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );

	SKI[SMOKE_KINGCRAB_TAIL].Init(			TEXT("Smoke\\SMOKE_KINGCRAB_TAIL.spr"),			0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	
	SKI[SMOKE_DEADMESSENGER].Init(			TEXT("Bullet\\Bullet_DeadMessenger.spr"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	
	SKI[SMOKE_FIRGROUNDTAIL].Init(			TEXT("Smoke\\Smoke_FireGroundTail.spr"),		0,	20,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_FIREGROUNDEXPLOSION].Init(	TEXT("Smoke\\Smoke_FireGroundExplosion.spr"),	0,	22,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOK_MASKMONK_STONE].Init(			TEXT("Bullet\\Bullet_DropSmallStone.SPR"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	90 );
	SKI[SMOK_MASKMONK_DROPSTONE].Init(		TEXT("Smoke\\Smoke_DropSmallStone.SPR"),		0,	9,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	-20 );

	SKI[SMOKE_PERSONCANNON_HIT].Init(		TEXT("Smoke\\Smoke_PersonCannon_Hit.spr"),		0,	23,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_MAGICFREEZEEXPLOSION].Init(	TEXT("Smoke\\MagicFreezeExplosion.spr"),		0,	20,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_STAFFSCROLL_HIT].Init(		TEXT("Smoke\\Smoke_StaffScroll_Hit.SPR"),		0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_STAFFSCROLL_TAIL].Init(		TEXT("Bullet\\Bullet_StaffScroll.spr"),			0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );

	SKI[SMOKE_GUNSCROLL_HIT].Init(			TEXT("Smoke\\Smoke_GunScroll_Hit.SPR"),			0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_GUNSCROLL_TAIL].Init(			TEXT("Smoke\\Smoke_GunScroll_Tail.spr"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_BOWSCROLL_HIT].Init(			TEXT("Smoke\\Smoke_BowScroll_Hit.SPR"),			0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_BOWSCROLL_TAIL].Init(			TEXT("Smoke\\Smoke_BowScroll_Tail.spr"),		0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_FOAM_HIT]	.Init(				TEXT("Smoke\\SMOKE_FOAM_HIT.SPR"),				0,	9,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );

	SKI[SMOKE_MORTAL_FIRE].Init(			TEXT("Smoke\\SMOKE_MORTAL_FIRE.SPR"),			0,	9,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	-30 );

	SKI[SMOKE_WATERARROW_TAIL].Init(		TEXT("Smoke\\Smoke_WaterArrow_Tail.SPR"),		0,	4,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_WATERARROW_HIT].Init(			TEXT("Smoke\\Smoke_WaterArrow_Hit.spr"),		0,	17,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	-15 );

	SKI[SMOKE_MAGIC_PALMWIND_HIT].Init(		TEXT("Smoke\\Magic_PalmWind.spr"),				0,	19,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_DOWNING_VINE]	.Init(			TEXT("Smoke\\Smoke_Downing_Vine.spr"),			0,	4,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	0 );

	SKI[SMOKE_PALMWIND_TAIL].Init(			TEXT("Bullet\\Bullet_PalmWind.spr"),			0,	9,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );

	SKI[SMOKE_HORSE_TICKET_EFFECT].Init(	TEXT("Smoke\\Horse_Effect01.Spr"),				0,	24,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0);

	SKI[SMOKE_MAGIC_SKELLGHOST_TAIL].Init(	TEXT("Smoke\\Magic_SkellGhost_Tail.SPR"),		0,	4,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_MAGIC_SKELLGHOST_FIRE].Init(	TEXT("Smoke\\Magic_SkellGhost_Fire.spr"),		0,	10,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	10 );

	SKI[SMOKE_SWORDMETEOR_EXPLOSION].Init(	TEXT("Smoke\\SwordMeteor_Explosion.SPR"),		0,	11,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_SWORDMETEOR_TAIL].Init(		TEXT("Smoke\\SwordMeteor_Tail.spr"),			0,	8,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_SWORDMETEOR_HIT].Init(		TEXT("Smoke\\SwordMeteor_Hit.spr"),				0,	8,	0,	SMOKE_DELAY_10,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	30 );

	SKI[SMOKE_MAIGCFROZEN_TAIL].Init(		TEXT("Smoke\\MagicFrozen_Tail.spr"),			0,	6,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_ICESTONE_HIT].Init(			TEXT("Smoke\\IceStone_Hit.spr"),				0,	8,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_NORMAL,	0,	40 );

	SKI[SMOKE_TWINSPEAR_TAIL].Init(			TEXT("Bullet\\TwinSpear.spr"),					0,	10,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_FIREWHIRLWIND_HIT].Init(		TEXT("Smoke\\FireWhirlWind_Hit.spr"),			0,	7,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_NORTHWIND_HIT].Init(			TEXT("Smoke\\NorthWind_Hit.spr"),				0,	6,	0,	SMOKE_DELAY_2,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	// 지팡이술(4장)
	SKI[SMOKE_NEW_STAFFSCROLL_HIT].Init(	TEXT("Smoke\\Smoke_New_StaffScroll_Hit.SPR"),	0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_NEW_STAFFSCROLL_TAIL].Init(	TEXT("Bullet\\Bullet_New_StaffScroll.spr"),		0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );

	// 총술(4장)
	SKI[SMOKE_NEW_GUNSCROLL_HIT].Init(		TEXT("Smoke\\Smoke_New_GunScroll_Hit.SPR"),		0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_NEW_GUNSCROLL_TAIL].Init(		TEXT("Smoke\\Smoke_New_GunScroll_Tail.spr"),	0,	8,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	// 활(4장)
	SKI[SMOKE_NEW_BOWSCROLL_HIT].Init(		TEXT("Smoke\\Smoke_New_BowScroll_Hit.SPR"),		0,	16,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_NEW_BOWSCROLL_TAIL].Init(		TEXT("Smoke\\Smoke_New_BowScroll_Tail.spr"),	0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );

	SKI[SMOKE_FIRESTORM_TAIL].Init(			TEXT("Bullet\\FireStorm.spr"),					0,	9,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );

	SKI[SMOKE_SPEAR_MANA_TAIL].Init(		TEXT("Bullet\\Spear_Mana.spr"),					0,	7,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_SPEAR_DRAGON_TAIL].Init(		TEXT("Bullet\\Spear_Dragon.spr"),				0,	9,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0,	true );
	SKI[SMOKE_SPEAR_DRAGON_HIT].Init(		TEXT("Smoke\\Smoke_spear_dragon_hit.SPR"),		0,	13,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0);

	SKI[SMOKE_BLACKSPIDER_HIT].Init(		TEXT("Smoke\\Smoke_BlackSpider_Hit.spr"),		0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	-30 );
	SKI[SMOKE_GIRAFFE_HIT].Init(			TEXT("Smoke\\Smoke_Giraffe_Hit.spr"),			0,	13,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_MINEGIRL_HIT].Init(			TEXT("Smoke\\Smoke_MineGirl_Hit.spr"),			0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0);

	SKI[SMOKE_CATRANGE_HIT].Init(			TEXT("Smoke\\Smoke_CatRange_Hit.SPR"),			0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_BEARTANG_HIT].Init(			TEXT("Smoke\\Smoke_BearTang_Magic.SPR"),		0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_SUPERBEARTANG_HIT].Init(		TEXT("Smoke\\Smoke_BearTang_Magic.SPR"),		0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	//SKI[SMOKE_GREATWALLASURA_HIT].Init(		TEXT("Smoke\\Smoke_GreatWallAsura.spr"),		0,	6,	0,	SMOKE_DELAY_1,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
	SKI[SMOKE_MARRIED_FULLDRESS].Init(			TEXT("Smoke\\Fulldress_eft.SPR"),				0,	14,	0,	SMOKE_DELAY_3,	FALSE,	TRANSPARENT_MODE_SCREEN,	0,	0 );
}

SI32 cltSmokeManager::SetSmoke(SI16 siKind, SI16 x, SI16 y, cltMapCommon* pmap,SI32 tempdirection,SI32 groundkind)
{
	SI32 id = FindEmptySmoke();
	if(id <= 0)return 0;
	
	if(id >= MAX_SMOKE_NUMBER)
	{
		MsgBox(TEXT("Smoke"), TEXT("%d %d"), id, siKind);
		return 0;
	}

	pclSmoke[id]->Init(siKind, id, x, y, pmap, &SKI[siKind],tempdirection,groundkind);
	SmokeInfo[id]	=	siKind;

	return id;
}


SI32 cltSmokeManager::FindEmptySmoke()
{
	int i;

	for(i=1;i<MAX_SMOKE_NUMBER;i++)
	{
		if(SmokeInfo[i]==0)return i;
	}
	return 0;

}

void cltSmokeManager:: DeleteSmoke(SI32 id)
{
	SmokeInfo[id] = 0;
}

void cltSmokeManager::DeleteSmoke()
{
	SI32 i;

	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		DeleteSmoke(i);
	}
}

void cltSmokeManager::Draw()
{

	SI32 i;

	// 현재 지도에 부합하는 놈만 그린다. 
	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		if(SmokeInfo[i])
		{
			pclSmoke[i]->Draw();
		}
	}
}

void cltSmokeManager::Action()
{
	SI32 i;

	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		if(SmokeInfo[i])
		{
			if(pclSmoke[i]->Action() == FALSE)
			{
				DeleteSmoke(i);
			}
		}
	}
}