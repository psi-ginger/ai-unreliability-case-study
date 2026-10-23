#include "GlobalImgFile.h"
#include "EtcTool\EtcTool.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient ;

cltGlobalImgFile::cltGlobalImgFile()
{
	SI32 i;

	for(i = 0;i < MAX_GLOBAL_IMG_FILE_NUMBER;i++)
	{
		pTSpr[i]		= NULL;
		pszFileName[i]	= NULL;
	}
	
	Add(GIMG_CHARSHADOW,	TEXT("GImg\\CShadow.spr"));
	Add(GIMG_POISON,		TEXT("GImg\\Poison.spr"));
	Add(GIMG_SETITEM_POISON,TEXT("GImg\\setitem_curse.SPR"));
	Add(GIMG_CUT,			TEXT("GImg\\Cut.spr"));
	Add(GIMG_EXPCOUNTER,	TEXT("GImg\\ExpCounter.Spr"));
	Add(GIMG_WATERDROP,		TEXT("GImg\\WaterDrop.Spr"));
	Add(GIMG_FIREFLY,		TEXT("GImg\\FireFly.Spr"));	
	Add(GIMG_HELPPANNEL,	TEXT("GImg\\HelpPannel.spr"));
	Add(GIMG_DMGCOUNTER,	TEXT("GImg\\DmgCounter.spr"));
	Add(GIMG_FIELDITEM,		TEXT("GImg\\FieldItem.spr"));
	Add(GIMG_LEVELUP,		TEXT("GImg\\LevelUp.Spr"));
	Add(GIMG_REGENERATION,	TEXT("GImg\\Regeneration.Spr"));
	Add(GIMG_ICEFLASH,		TEXT("GImg\\IceFlash.Spr"));

	Add(GIMG_BIGBOSS,			TEXT("GImg\\BigBoss.Spr"));
	Add(GIMG_SMALLBOSS,			TEXT("GImg\\SmallBoss.Spr"));
	Add(GIMG_CRITICALEFFECT,	TEXT("GImg\\CriticalEffect.Spr"));
	Add(GIMG_CLICKPOINT,		TEXT("GImg\\ClickPoint.Spr"));
	Add(GIMG_MONSTERINFOBACK,	TEXT("GImg\\MonsterInfoBack.Spr"));
	Add(GIMG_WARKILL,			TEXT("GImg\\WarKill.Spr"));
	Add(GIMG_WARKILLNUM,		TEXT("GImg\\WarKillNum.Spr"));
	Add(GIMG_JIM,				TEXT("GImg\\Jim.Spr"));
	Add(GIMG_SKILLLEVELUP,		TEXT("GImg\\SkillLevelUp.Spr"));
	Add(GIMG_SERVERNAME,		TEXT("GImg\\servername.spr"));
	Add(GIMG_SKILLNAME,			TEXT("GImg\\SkillName.Spr"));
	Add(GIMG_SKILLEXPCOUNTER,	TEXT("GImg\\SkillExpCounter.Spr"));

	// 캐릭터 특수 이펙트 관련 - 부적 붙이고 공격
/*	Add(GIMG_1HAND_SWING_EFFECT,		"GImg\\Special_Sword.Spr");
	Add(GIMG_2HAND_BRANDISH_EFFECT,		"GImg\\Special_Spear_Brandish.Spr");
	Add(GIMG_2HAND_JAB_EFFECT,			"GImg\\Special_Spear_Jab.Spr");

	Add(GIMG_HORSE1HAND_SWING_EFFECT,	TEXT("GImg\\Special_Sword_Horse.Spr"));
	Add(GIMG_HORSE2HAND_BRANDISH_EFFECT, TEXT("GImg\\Special_Spear_Brandish_Horse.Spr"));
	Add(GIMG_HORSE2HAND_JAB_EFFECT,		"GImg\\Special_Spear_Jab_Horse.Spr");

	Add(GIMG_1HANDDMG_SWING_EFFECT,		"GImg\\Special_SwordDmg.Spr");
	Add(GIMG_2HANDDMG_BRANDISH_EFFECT,	TEXT("GImg\\Special_SpearDmg_Brandish.Spr"));
	Add(GIMG_2HANDDMG_JAB_EFFECT,		"GImg\\Special_SpearDmg_Jab.Spr");*/

	Add(GIMG_SPECIAL_SWORD,					TEXT("GImg\\Special_Sword.Spr"));
	Add(GIMG_SPECIAL_SPEAR_BRANDISH,		TEXT("GImg\\Special_Spear_Brandish.Spr"));
	Add(GIMG_SPECIAL_SPEAR_JAB,				TEXT("GImg\\Special_Spear_Jab.Spr"));
	Add(GIMG_SPECIAL_AXE,					TEXT("GImg\\Special_Axe.Spr"));

	Add(GIMG_SPECIAL_SWORD_HORSE,			TEXT("GImg\\Special_Sword_Horse.Spr"));
	Add(GIMG_SPECIAL_SPEAR_BRANDISH_HORSE,	TEXT("GImg\\Special_Spear_Brandish_Horse.Spr"));
	Add(GIMG_SPECIAL_SPEAR_JAB_HORSE,		TEXT("GImg\\Special_Spear_Jab_Horse.Spr"));
	Add(GIMG_SPECIAL_AXE_HORSE,				TEXT("GImg\\Special_Axe_Horse.Spr"));

	Add(GIMG_SPECIAL_SWORDDMG,				TEXT("GImg\\Special_SwordDmg.Spr"));
	Add(GIMG_SPECIAL_SPEARDMG_BRANDISH,		TEXT("GImg\\Special_SpearDmg_Brandish.Spr"));
	Add(GIMG_SPECIAL_SPEARDMG_JAB,			TEXT("GImg\\Special_SpearDmg_Jab.Spr"));
	Add(GIMG_SPECIAL_AXEDMG,				TEXT("GImg\\Special_AxeDmg.Spr"));

	// 학교 관련
	Add(GIMG_SCHOOL1,			TEXT("GImg\\School1.Spr"));
	Add(GIMG_SCHOOL2,			TEXT("GImg\\School2.Spr"));
	Add(GIMG_SCHOOL3,			TEXT("Gimg\\School3.Spr"));

	Add(GIMG_MAGIC_THUNDER,		TEXT("GImg\\MagicThunder.Spr"));
	Add(GIMG_MAGIC_ROCKSLIDE,	TEXT("GImg\\MagicRockSlide.Spr"));
	
	Add(GIMG_MAGIC_FLAME,		TEXT("GImg\\MagicFlame.Spr"));
	Add(GIMG_MAGIC_WHIRLPOOL,	TEXT("GImg\\MagicWhirlPool.Spr"));
	Add(GIMG_MAGIC_WAR,			TEXT("GImg\\War.Spr"));
	
	Add(GIMG_MAGIC_PROMPTBAR,	TEXT("GImg\\PromptBar.Mpr"));

	Add(GIMG_MAGIC_DEFUP,		TEXT("GImg\\defUp.Spr"));

	Add(GIMG_VILLAGENAMEBACK,	TEXT("GImg\\VillageNameBack.Spr"));
	
	Add(GIMG_FIRE1,				TEXT("GImg\\Fire1.Spr"));
	Add(GIMG_FIRE2,				TEXT("GImg\\Fire2.Spr"));
	Add(GIMG_FIRE3,				TEXT("GImg\\Fire3.Spr"));
	Add(GIMG_FIRE4,				TEXT("GImg\\Fire4.Spr"));
	
	Add(GIMG_NOFIRSTATTACK,		TEXT("GImg\\NoFirstAttack.Spr"));
	
	Add(GIMG_RANKMARK,			TEXT("GImg\\RankMark.Spr"));
	
	Add(GIMG_HEAL,				TEXT("GImg\\Heal.Spr"));

	Add(GIMG_MAKEITEMDRAWPART1,	TEXT("GImg\\MakeItemDrawPart1.Spr"));
	Add(GIMG_MAKEITEMDRAWPART2,	TEXT("GImg\\MakeItemDrawPart2.Spr"));
	Add(GIMG_MAKEITEMDRAWPART3,	TEXT("GImg\\MakeItemDrawPart3.Spr"));
	
	Add(GIMG_PARTYEMOTICON,		TEXT("GImg\\PartyEmoticon.SPR"));

	Add(GIMG_KINGEFFECT,		TEXT("GImg\\KingEffects.spr"));
	Add(GIMG_GODEFFECT,			TEXT("GImg\\GodEffects.SPR"));
	
	Add(GIMG_FRIENDLOGIN,		TEXT("GImg\\FriendLogin.SPR"));

	Add(GIMG_ICEFROZEN,			TEXT("GImg\\IceFrozen.spr"));
	Add(GIMG_MAGIC_DRAIN,		TEXT("GImg\\MagicDrain.spr"));
	Add(GIMG_HEALCOUNTER,		TEXT("GImg\\HealCounter.spr"));
	
	Add(GIMG_VILLAGENOTICEBACKGROUND, TEXT("GImg\\VillageNoticeBackGround.spr"));
		
	Add(GIMG_MAGIC_POISONFLAME,	TEXT("GImg\\MagicPoisonFlame.Spr"));
	Add(GIMG_SYIDEOCK,			TEXT("GImg\\Syideock.Spr"));
	
	Add(GIMG_NOICEATTACK,		TEXT("GImg\\NoIceAttack.Spr"));
	Add(GIMG_SUMMONMARK,		TEXT("GImg\\SummonMark.SPR"));

	Add(GIMG_VILLAGEMARK01,		TEXT("GImg\\VillageMark01.spr"));
	Add(GIMG_MAGIC_DRAGONCANNON,TEXT("GImg\\firedragon_cannon.SPR"));
	Add(GIMG_MAGIC_DRAGONSHADOW,TEXT("GImg\\Firedragon_shadow.SPR"));
	
	Add(GIMG_GWANGGAETO_EFFECT,	TEXT("Gimg\\GwangGaeToEffect.spr"));

	Add(GIMG_MAGIC_DROPMONEY,	TEXT("Gimg\\MagicDropMoney.spr"));
	Add(GIMG_MAGIC_BLACKINK,	TEXT("Gimg\\MagicBlackInk.spr"));
	Add(GIMG_MAGIC_DROPSNOWMAN,	TEXT("Gimg\\MagicDropSnowman.spr"));
	
	Add(GIMG_MAGIC_DROPFORK_FRONT,	TEXT("Gimg\\MagicDropFork_Front.SPR"));
	Add(GIMG_MAGIC_DROPFORK_BACK,	TEXT("Gimg\\MagicDropFork_Back.SPR"));
	Add(GIMG_RESIDENCELOGIN,		TEXT("Gimg\\ResidenceLogin.SPR"));
	
	Add(GIMG_MAGIC_CURSEATTACK,		TEXT("Gimg\\MagicCurseAttack.SPR"));
	Add(GIMG_MAGIC_CURSECRITICAL,	TEXT("Gimg\\MagicCurseCritical.SPR"));

	Add(GIMG_MAGIC_CURSEDEFENSE,	TEXT("Gimg\\MagicCurseDefense.SPR"));
	Add(GIMG_MAGIC_CURSEMOVESPEED,	TEXT("Gimg\\MagicCurseMovespeed.SPR"));

	Add(GIMG_MAGIC_SKILL_BOOK_ICERAIN,	TEXT("Gimg\\MAGIC_ICERAIN.SPR"));
	Add(GIMG_MAGIC_SKILL_BOOK_CRASH,	TEXT("Gimg\\MAGIC_STONECRASH.SPR"));
	Add(GIMG_MAGIC_SKILL_BOOK_TORNADO,	TEXT("Gimg\\MAGIC_TORNADO.SPR"));
	
	
	Add(GIMG_ITEMIMG01,				TEXT("Gimg\\ItemImg01.SPR"));
	Add(GIMG_ITEMIMG02,				TEXT("Gimg\\ItemImg02.SPR"));
	Add(GIMG_ITEMIMG03,				TEXT("Gimg\\ItemImg03.SPR"));
	Add(GIMG_ITEMIMG04,				TEXT("Gimg\\ItemImg04.SPR"));
	Add(GIMG_ITEMIMG05,				TEXT("Gimg\\ItemImg05.SPR"));
	Add(GIMG_ITEMIMG06,				TEXT("Gimg\\ItemImg06.SPR"));
	Add(GIMG_ITEMIMG07,				TEXT("Gimg\\ItemImg07.SPR"));
	Add(GIMG_ITEMIMG08,				TEXT("Gimg\\ItemImg08.SPR"));
	Add(GIMG_ITEMIMG09,				TEXT("Gimg\\ItemImg09.SPR"));
	Add(GIMG_ITEMIMG10,				TEXT("Gimg\\ItemImg10.SPR"));
	Add(GIMG_ITEMIMG11,				TEXT("Gimg\\ItemImg11.SPR"));
	Add(GIMG_ITEMIMG12,				TEXT("Gimg\\ItemImg12.SPR"));
	Add(GIMG_ITEMIMG13,				TEXT("Gimg\\ItemImg13.SPR"));
	Add(GIMG_ITEMIMG14,				TEXT("Gimg\\ItemImg14.SPR"));
	Add(GIMG_ITEMIMG15,				TEXT("Gimg\\ItemImg15.SPR"));
	Add(GIMG_ITEMIMG16,				TEXT("Gimg\\ItemImg16.SPR"));
	Add(GIMG_ITEMIMG17,				TEXT("Gimg\\ItemImg17.SPR"));
	Add(GIMG_ITEMIMG18,				TEXT("Gimg\\ItemImg18.SPR"));
	Add(GIMG_ITEMIMG19,				TEXT("Gimg\\ItemImg19.SPR"));
	Add(GIMG_ITEMIMG20,				TEXT("Gimg\\ItemImg20.SPR"));

	Add(GMIG_MAGIC_SWORDBOMB1,		TEXT("Gimg\\MagicSwordBomb1.SPR"));
	Add(GMIG_MAGIC_SWORDBOMB2,		TEXT("Gimg\\MagicSwordBomb2.SPR"));

	Add(GIMG_NOTICEEVENT_STAMP1,	TEXT("Gimg\\STAMPEVENT1.SPR"));
	Add(GIMG_NOTICEEVENT_STAMP2,	TEXT("Gimg\\STAMPEVENT2.SPR"));
	Add(GIMG_NOTICEEVENT_STAMP3,	TEXT("Gimg\\STAMPEVENT3.SPR"));
	
	Add(GIMG_HUMAN_EFFECT,			TEXT("Gimg\\HumanEffect.SPR"));
	Add(GIMG_HUMAN_EFFECT2,			TEXT("Gimg\\HumanEffect2.SPR"));
	Add(GIMG_HUMAN_EFFECT3,			TEXT("Gimg\\HumanEffect3.SPR"));
	Add(GIMG_HUMAN_EFFECT4,			TEXT("Gimg\\HumanEffect4.SPR"));
	Add(GIMG_HUMAN_EFFECT5,			TEXT("Gimg\\HumanEffect5.SPR"));
	Add(GIMG_HUMAN_EFFECT6,			TEXT("Gimg\\HumanEffect6.SPR"));
	Add(GIMG_HUMAN_EFFECT7,			TEXT("Gimg\\HumanEffect7.SPR"));
		
	Add(GIMG_MAKEMASTER,			TEXT("Gimg\\Master.SPR" ));

	Add(GIMG_INTERFACE_CLOSEBUTTON,	TEXT("Gimg\\Interface\\Common\\CloseButton.SPR" ));
		
	Add(GIMG_MAGIC_UPPERDRAGON1, 	TEXT("Gimg\\MagicUpperDragon1.spr"));
	Add(GIMG_MAGIC_UPPERDRAGON2, 	TEXT("Gimg\\MagicUpperDragon2.spr"));
	Add(GIMG_MAGIC_UPPERDRAGON3, 	TEXT("Gimg\\MagicUpperDragon3.spr"));

	Add(GIMG_FOG, TEXT("Gimg\\Fog.spr"));

	Add(GIMG_PCROOM_MARK,			TEXT("GImg\\PCROOM_MARK.spr" ));
	Add(GIMG_PCROOM_STATUS,			TEXT("GImg\\PCROOM_STATUS.spr" ));

	Add(GIMG_MAGICFIRE,				TEXT("GImg\\MagicFire.spr" ));
	Add(GIMG_MAGICFIRE_BURN,		TEXT("GImg\\MagicFire_Burn.spr" ));
	Add(GIMG_MAGICFIRE_BURNRING,	TEXT("GImg\\MagicFire_BurnRing.spr" ));

	Add(GIMG_VILLAGEUNITNPC,		TEXT("GImg\\VillageUnitNpc.spr" ));

	Add(GIMG_JINSOO,				TEXT("GImg\\JinSoo.Spr"));
	Add(GIMG_SYIDEOCKCRY,			TEXT("Gimg\\SyiDeockCry.spr"));

	Add(GIMG_SWORDMETEOR,			TEXT("Gimg\\SwordMeteor.spr"));
	Add(GIMG_MAGICFROZEN,			TEXT("Gimg\\MagicFrozen.spr"));

	Add(GIMG_MAGIC_FIREWHIRLWIND,	TEXT("Gimg\\MagicFireWhirlWind2.spr"));
	Add(GIMG_NEW_ATTACK_STAFF, 		TEXT("Gimg\\NewHitTypeStaff.spr"));
	Add(GIMG_NEW_ATTACK_SWORD, 		TEXT("Gimg\\NewHitTypeSword.spr"));
	Add(GIMG_NEW_ATTACK_SPEAR, 		TEXT("Gimg\\NewHitTypeSpear.spr"));
	Add(GIMG_NEW_ATTACK_AXE,		TEXT("Gimg\\NewHitTypeAxe.spr"));
	Add(GIMG_NEW_ATTACK_BOWGUN,		TEXT("Gimg\\NewHitTypeBowGun.spr"));

	Add(GIMG_NEW_ATTACK_STAFF2, 	TEXT("Gimg\\NewHitTypeStaff2.spr"));
	Add(GIMG_NEW_ATTACK_SWORD2, 	TEXT("Gimg\\NewHitTypeSword2.spr"));
	Add(GIMG_NEW_ATTACK_SPEAR2, 	TEXT("Gimg\\NewHitTypeSpear2.spr"));
	Add(GIMG_NEW_ATTACK_AXE2,		TEXT("Gimg\\NewHitTypeAxe2.spr"));
	Add(GIMG_NEW_ATTACK_BOWGUN2,	TEXT("Gimg\\NewHitTypeBowGun2.spr"));

	Add(GIMG_MAGIC_SUMMONDRAGON,	TEXT("Gimg\\Magic_SummonDragon.spr"));

	if ( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		Add(GIMG_NAESI,				TEXT("GImg\\Naesi.SPR"));
		//Add(GIMG_GAMESTATUS,		TEXT("GImg\\GameStatus.SPR"));
	}

	//else if( pclClient->siServiceArea == ConstServiceArea_China )
	//{
	//	Add(GIMG_NAESI,				TEXT("GImg\\Naesi.SPR"));
	//	Add(GIMG_GAMESTATUS,		TEXT("GImg\\GameStatus.SPR"));
	//}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		Add(GIMG_NAESI,			TEXT("GImg\\Naesi.SPR"));
		//Add(GIMG_GAMESTATUS,	TEXT("GImg\\GameStatus.SPR"));
	}
	else
	{
		Add(GIMG_NAESI,			TEXT("GImg\\NaesiE.SPR"));
		//Add(GIMG_GAMESTATUS,	TEXT("GImg\\GameStatusE.SPR"));
	}

	Add(GIMG_MAGIC_EARTHQUAKE,	TEXT("Gimg\\Magic_EarthQuake.spr"));

//	Add(GIMG_ARROW,		 "Gimg\\Arrow.spr");
//	Add(GIMG_ARROWEFFECT,TEXT("Gimg\\ArrowEffect.spr"));
	Add(GIMG_QUESTNPCEFFECT,	TEXT("Gimg\\QUESTNPCEFFECT.spr"));
	Add(GIMG_CLICKMOUSE,		TEXT("Gimg\\ClickArrow.spr"));

	Add(GIMG_PREMIUM_STATUS,	TEXT("GImg\\Premium_Status.spr") );

	Add(GIMG_MAGIC_THUNDER1,	TEXT("GImg\\MagicThunder1.Spr"));

	Add(GIMG_RANKING,			TEXT("GImg\\Ranking.Spr"));   
	Add(GIMG_MVPMARK1,			TEXT("GImg\\MVPMark01.Spr"));

	Add(GIMG_GMMARK,			TEXT("GImg\\GMMARK.Spr"));
	Add(GIMG_MVP_EFFECT,		TEXT("Gimg\\MVPEffect.SPR"));
	Add(GIMG_MVP_AROUND_EFFECT, TEXT("Gimg\\MVPAroundEffect.SPR"));

	Add(GIMG_SEEDDROP1,			TEXT("GImg\\SeedDrop1.Spr"));
	Add(GIMG_SEEDDROP2,			TEXT("GImg\\SeedDrop2.Spr"));
	Add(GIMG_SEEDDROP3,			TEXT("GImg\\SeedDrop3.Spr"));
	Add(GIMG_SEEDDROP4,			TEXT("GImg\\SeedDrop4.Spr"));

	////////////////////////////////////////////////////////////////////
/*
	if (pclClient->siServiceArea == ConstServiceArea_Korea) {
		Add(GIMG_ITEMSEALED,	TEXT("GImg\\ITEM_SEALED_K.SPR"));
	}	
	else if (pclClient->siServiceArea == ConstServiceArea_China || pclClient->siServiceArea == ConstServiceArea_Japan) {
		Add(GIMG_ITEMSEALED, TEXT("GImg\\ITEM_SEALED_C.SPR"));
	}
	else if (pclClient->siServiceArea == ConstServiceArea_Taiwan) {
		Add(GIMG_ITEMSEALED,	TEXT("GImg\\ITEM_SEALED_E.SPR"));
	}	
	else{
		Add(GIMG_ITEMSEALED,	TEXT("GImg\\ITEM_SEALED_E.SPR"));
	}
	////////////////////////////////////////////////////////////////////////////
*/
	Add(GIMG_ITEMSEALED,				TEXT("GImg\\ITEM_SEALED.SPR"));//이미지 리소스 분리 작업 [2007.07.18 성웅]

	Add(GIMG_MAGIC_FROZENGOLEM,			TEXT("GImg\\MagicFrozenGolem.SPR"));
	Add(GIMG_MAGIC_FROZENQUEEN,			TEXT("GImg\\MagicFrozenQueen.SPR"));
	Add(GIMG_MAGIC_FROZENSORCERER,		TEXT("GImg\\MagicFrozenSorcerer.SPR"));
	Add(GIMG_MAGIC_GOTPA,				TEXT("GImg\\MagicGotpa.SPR"));
	Add(GIMG_ENCHANT_EFFECT_HERO_WE,	TEXT("GImg\\ENCHANT_EFFECT_HERO_WE.SPR"));
	Add(GIMG_ENCHANT_EFFECT_HERO_AM,	TEXT("GImg\\ENCHANT_EFFECT_HERO_AM.SPR"));
	Add(GIMG_ENCHANT_EFFECT_HEROINE_WE,	TEXT("GImg\\ENCHANT_EFFECT_HEROINE_WE.SPR"));
	Add(GIMG_ENCHANT_EFFECT_HEROINE_AM,	TEXT("GImg\\ENCHANT_EFFECT_HEROINE_AM.SPR"));

	Add(GIMG_QUEST_EFFECT_RECEIVE,		TEXT("GImg\\QUEST_EFFECT_RECEIVE.SPR"));
	Add(GIMG_QUEST_EFFECT_CLEAR,		TEXT("GImg\\QUEST_EFFECT_CLEAR.SPR"));

	Add(GIMG_MAGIC_DRACULA,				TEXT("GImg\\MagicDracula.SPR"));
	Add(GIMG_MAGIC_DRACULAFX,			TEXT("GImg\\MagicDraculaFX.SPR"));

	Add(GIMG_MAGIC_PRIEST,				TEXT("Smoke\\Firedragon_explosion.SPR"));

	Add(GIMG_BUFIMAGE,					TEXT("GIMG\\BufImage.SPR"));
	Add(GIMG_BUFPERCENT,				TEXT("GIMG\\BufPercent.SPR"));
	Add(GIMG_BEGINWARP,					TEXT("Smoke\\Smoke_BeginWarp.SPR"));
	Add(GIMG_VILLAGEWAR_NOTICE,			TEXT("GIMG\\VILLAGEWAR_NOTICE.SPR"));

	Add(GIMG_MAGIC_MANDRAGORA,			TEXT("GIMG\\MandragoraBullet_FX.SPR"));	
	Add(GIMG_SKILLLEVELUP_EFF,			TEXT("GIMG\\SKILLLEVELUP_EFF.SPR"));
	Add(GIMG_LEVELUP_EFF,				TEXT("GIMG\\LEVELUP_EFF.SPR"));
	Add(GIMG_CHRISTMAS_BUF,				TEXT("GIMG\\CHRISTMASBUF.SPR"));

	Add(GIMG_GUILDWAR_NOTICE,			TEXT("GIMG\\GuildWarNotice.spr"));

	Add(GIMG_GUILDWAR_COUNT,			TEXT("GIMG\\GuildWarCount.spr"));

	Add(GIMG_NEWYEAR_EVENT_NOTICE,		TEXT("GIMG\\NewYearEvent.SPR"));
	Add(GIMG_SUMMON_EVENT_BUF,			TEXT("GIMG\\SummonEventBuf.SPR"));
	Add(GIMG_EMOTICON,					TEXT("GIMG\\Emoticon.SPR"));
	Add(GIMG_VALENTINEEMOTICON,			TEXT("GIMG\\Valentine_emoticon.SPR"));
	Add(GIMG_MAGIC_PAPAGOBIN,			TEXT("char\\PaPaGoblin\\PaPaGoblinMagic.spr"));
	Add(GIMG_HEART_EFFECT,				TEXT("GIMG\\HeartEffect.SPR"));
	Add(GIMG_MISSHIT,					TEXT("GImg\\MissHit.spr"));
	Add(GIMG_EMOTICON_SMALL,			TEXT("GIMG\\SmallEmo.spr"));
	Add(GIMG_HORSEWING1 , 				TEXT("CCD\\HorseWingsData\\HorseWings1HSR.Spr"));
	Add(GIMG_HORSEWING2 , 				TEXT("CCD\\HorseWingsData\\HorseWings2HTR.Spr"));
	Add(GIMG_HORSEWING3 , 				TEXT("CCD\\HorseWingsData\\HorseWings2HBR.Spr"));
	Add(GIMG_HORSEWING4 , 				TEXT("CCD\\HorseWingsData\\HorseWings2HGR.Spr"));
	Add(GIMG_HORSEWING5 , 				TEXT("CCD\\HorseWingsData\\HorseWingsHITR.Spr"));
	Add(GIMG_HORSEWING6 , 				TEXT("CCD\\HorseWingsData\\HorseWingsMOVR.Spr"));
	Add(GIMG_HORSEWING7 , 				TEXT("CCD\\HorseWingsData\\HorseWingsWT1R.Spr"));
	Add(GIMG_HORSEWING8 , 				TEXT("CCD\\HorseWingsData\\HorseWingsWT2R.Spr"));

	Add(GIMG_QUESTARROW ,				TEXT("GImg\\QuestArrow.spr"));

	Add(GIMG_FOURLEAF ,					TEXT("GImg\\FOURLEAF.spr"));
	Add(GIMG_GOVERNMENT_RANKMARK,		TEXT("GImg\\GovernMentRankMark.SPR"));

	Add(GIMG_CUSHION_1 , 				TEXT("GImg\\CUSHION_1.SPR"));
	Add(GIMG_CUSHION_2 , 				TEXT("GImg\\CUSHION_2.SPR"));
	Add(GIMG_CUSHION_3 , 				TEXT("GImg\\CUSHION_3.SPR"));
	Add(GIMG_CUSHION_4 , 				TEXT("GImg\\CUSHION_4.SPR"));
	Add(GIMG_CUSHION_CAT_1 , 			TEXT("GImg\\CUSHION_CAT_1.SPR"));
	Add(GIMG_CUSHION_CAT_2 , 			TEXT("GImg\\CUSHION_CAT_2.SPR"));
	Add(GIMG_CUSHION_CAT_3 , 			TEXT("GImg\\CUSHION_CAT_3.SPR"));
	Add(GIMG_CUSHION_CAT_4 , 			TEXT("GImg\\CUSHION_CAT_4.SPR"));

	Add(GIMG_SPECIAL_PCROOM_MARK_1, 	TEXT("GImg\\Special_Pcroom_Mark_1.SPR"));
	Add(GIMG_SPECIAL_PCROOM_MARK_2, 	TEXT("GImg\\Special_Pcroom_Mark_2.SPR"));
	Add(GIMG_SPECIAL_PCROOM_MARK_3, 	TEXT("GImg\\Special_Pcroom_Mark_3.SPR"));
	Add(GIMG_SPECIAL_PCROOM_MARK_4, 	TEXT("GImg\\Special_Pcroom_Mark_4.SPR"));

	Add(GIMG_SPECIAL_PCROOM_LARGE_MARK,		TEXT("GImg\\Special_Pcroom_Large_Mark.SPR"));
	Add(GIMG_SPECIAL_NO_PCROOM_LARGE_MARK,	TEXT("GImg\\Special_No_Pcroom_Large_Mark.SPR"));

	if ( pclClient->IsCountrySwitch( Switch_NewHitEffect ) )
	{
		Add(GIMG_DAMAGE_PC_0, 			TEXT("GImg\\DamagePC_0.SPR"));
		Add(GIMG_DAMAGE_PC_1, 			TEXT("GImg\\DamagePC_1.SPR"));
		Add(GIMG_DAMAGE_PC_2, 			TEXT("GImg\\DamagePC_2.SPR"));
		Add(GIMG_DAMAGE_PC_3, 			TEXT("GImg\\DamagePC_3.SPR"));
		Add(GIMG_DAMAGE_PC_4, 			TEXT("GImg\\DamagePC_4.SPR"));
		Add(GIMG_DAMAGE_PC_5, 			TEXT("GImg\\DamagePC_5.SPR"));
		Add(GIMG_DAMAGE_PC_6, 			TEXT("GImg\\DamagePC_6.SPR"));
		Add(GIMG_DAMAGE_PC_7, 			TEXT("GImg\\DamagePC_7.SPR"));
		Add(GIMG_DAMAGE_PC_8, 			TEXT("GImg\\DamagePC_8.SPR"));
		Add(GIMG_DAMAGE_PC_9, 			TEXT("GImg\\DamagePC_9.SPR"));
		Add(GIMG_DAMAGE_NPC_0,	 		TEXT("GImg\\DamageNPC_0.SPR"));
		Add(GIMG_DAMAGE_NPC_1,	 		TEXT("GImg\\DamageNPC_1.SPR"));
		Add(GIMG_DAMAGE_NPC_2,	 		TEXT("GImg\\DamageNPC_2.SPR"));
		Add(GIMG_DAMAGE_NPC_3,	 		TEXT("GImg\\DamageNPC_3.SPR"));
		Add(GIMG_DAMAGE_NPC_4,	 		TEXT("GImg\\DamageNPC_4.SPR"));
		Add(GIMG_DAMAGE_NPC_5,	 		TEXT("GImg\\DamageNPC_5.SPR"));
		Add(GIMG_DAMAGE_NPC_6,	 		TEXT("GImg\\DamageNPC_6.SPR"));
		Add(GIMG_DAMAGE_NPC_7,	 		TEXT("GImg\\DamageNPC_7.SPR"));
		Add(GIMG_DAMAGE_NPC_8,	 		TEXT("GImg\\DamageNPC_8.SPR"));
		Add(GIMG_DAMAGE_NPC_9,	 		TEXT("GImg\\DamageNPC_9.SPR"));
		Add(GIMG_DAMAGE_PC_MISS, 		TEXT("GImg\\DamagePC_Miss.SPR"));
		Add(GIMG_DAMAGE_NPC_MISS,		TEXT("GImg\\DamageNPC_Miss.SPR"));
		Add(GIMG_DAMAGE_CRITICAL,		TEXT("GImg\\Damage_Critical.SPR"));
		Add(GIMG_DAMAGE_PC_DODGE, 		TEXT("GImg\\DamagePC_Dodge.SPR"));
		Add(GIMG_DAMAGE_NPC_DODGE, 		TEXT("GImg\\DamageNPC_Dodge.SPR"));
	}

	Add(GIMG_ITEMEXPIRED,					TEXT("GImg\\ItemExpired.SPR"));
	Add(GIMG_ITEMCANNOTEQIP,				TEXT("GImg\\ItemCanNotEquip.SPR"));

	//KHY - 1109 - 가챠레어- 강아지방석추가
	Add(GIMG_CUSHIOND_1 ,					TEXT("GImg\\CUSHIOND_1.SPR"));
	Add(GIMG_CUSHIOND_2 ,					TEXT("GImg\\CUSHIOND_2.SPR"));
	Add(GIMG_CUSHIOND_3 ,					TEXT("GImg\\CUSHIOND_3.SPR"));
	Add(GIMG_CUSHIOND_4 ,					TEXT("GImg\\CUSHIOND_4.SPR"));
	Add(GIMG_CUSHION_DOG_1 ,				TEXT("GImg\\CUSHION_DOG_1.SPR"));
	Add(GIMG_CUSHION_DOG_2 ,				TEXT("GImg\\CUSHION_DOG_2.SPR"));
	Add(GIMG_CUSHION_DOG_3 ,				TEXT("GImg\\CUSHION_DOG_3.SPR"));
	Add(GIMG_CUSHION_DOG_4 ,				TEXT("GImg\\CUSHION_DOG_4.SPR"));

	//[추가 : 황진성 2008. 1. 21 => 리스트 박스에 사용할 길드 마크 2, 3번 이미지 파일 추가.]
	Add(GIMG_VILLAGEMARK02,					TEXT("GImg\\VILLAGEMARK02.SPR"));
	Add(GIMG_VILLAGEMARK03,					TEXT("GImg\\VILLAGEMARK03.SPR"));

	//KHY - 0313 - 변신 100%효과 발생.
	Add(GIMG_TRANSFORM_SYNCHRO100,			TEXT("GImg\\Synchro100eft.SPR"));

	//PCK : 용갱장 마법추가 (08.03.23)
	Add(GIMG_MAGIC_EMPERORMASTER,			TEXT("GImg\\MagicEmperorMaster.SPR"));

	// [진성] 아티펙트 관련 이미지 추가.
	Add(GIMG_ARTIFACT_SLOT_STATUS,			TEXT("GImg/Artifact_Slot_Status.SPR"));
	Add(GIMG_ARTIFACT_GAUGE_BACK,			TEXT("GImg/artifact_gauge_back.SPR"));
	Add(GIMG_ARTIFACT_GAUGE,				TEXT("GImg/artifact_gauge.SPR"));
	Add(GIMG_ARTIFACT_SLOT_EFFECT,			TEXT("GImg/artifact_slot_effect.SPR"));
	Add(GIMG_ARTIFACT_SLOT_REMOVE,			TEXT("GImg/Artifact_Slot_Remove.SPR"));

	// PCK : 괴수손 마법추가 (08.04.11)
	Add(GIMG_MAGIC_GREATWALLSOLDIER,		TEXT("GImg\\MagicGreatWallSoldier.SPR"));	
	Add(GIMG_MAGIC_GREATWALLSOLDIERFX,		TEXT("GImg\\MagicGreatWallSoldierFX.SPR"));

	// PCK : 머리만 마법 추가 (08.04.11)
	Add(GIMG_MAGIC_GREATWALLMAGICIAN,		TEXT("GImg\\MagicGreatWallMagician.SPR"));

	// [진성] 화살표 들어왔다 나가는 이미지. => 2008-4-18
	Add(GIMG_ARROW_IN_OUT,					TEXT("GImg\\arrow_in_out.SPR"));

	// PCK : 수라 마법 추가 (08.05.28)
	Add(GIMG_MAGIC_GREATWALLSPIDER,			TEXT("GImg\\MagicGreatWallSpider.SPR"));

	// [진성] 주사위 돌려라~ ㅋㅋ. => 2008-7-9
	Add(GIMG_MAGIC_DICE,					TEXT("GImg\\RouletteDice.SPR"));
	// [성웅] PVP전 카운트 다운~
	Add(GIMG_PVP_COUNTDOWN,					TEXT("GImg\\PVP_count.SPR "));
	// [성웅] PVP전 카운트 다운~
	Add(GIMG_PVP_WLD,						TEXT("GImg\\PVP_WLD.SPR "));

	// [지연] 복불복 게임 - WIN 이미지
	Add(GIMG_BBB_WL,						TEXT("GImg\\bbbwl.SPR"));

	// [영훈] NPC초상 작은 이미지
	Add(GIMG_PORTRAIT_SMALL01,				TEXT("Interface\\Portrait\\PortraitSmallIcon01.SPR"));
	Add(GIMG_PORTRAIT_SMALL02,				TEXT("Interface\\Portrait\\PortraitSmallIcon02.SPR"));
	
	Add(GIMG_GACHASUMMON_EFFECT,			TEXT("GImg\\GachaSummonEffect.SPR"));	

	//KHY - 0826 - 시세확인 아이콘 추가.
	Add(GIMG_MARKETCONDITIONS_ICON,			TEXT("GImg/MarketCondition.SPR"));

	// PCK : 소환수용 프로즌골렘, 프로즌퀸 추가
	Add(GIMG_MAGIC_SFROZENGOLEM,			TEXT("GImg\\MagicFrozenGolem.SPR"));
	Add(GIMG_MAGIC_SFROZENQUEEN,			TEXT("GImg\\MagicFrozenQueen.SPR"));

	//[진성] 아이템 이미지 큰거. => 2008-9-23
	Add(GIMG_ITEMIMAGEBIG01,				TEXT("GImg\\ITEMIMGBIG01.SPR"));
	
	Add(GIMG_SETITEMEFFECT0,				TEXT("Gimg\\SetItemEffect.spr"));
	Add(GIMG_SETITEMEFFECT2,				TEXT("Gimg\\SetItemEffect2.spr"));
	Add(GIMG_SETITEMEFFECT3,				TEXT("Gimg\\SetItemEffect3.spr"));
	Add(GIMG_SETITEMEFFECT4,				TEXT("Gimg\\SetItemEffect4.spr"));
	Add(GIMG_SETITEMEFFECT5,				TEXT("Gimg\\SetItemEffect5.spr"));
	Add(GIMG_SETITEMEFFECT6,				TEXT("Gimg\\SetItemEffect6.spr"));
	Add(GIMG_SETITEMEFFECT7,				TEXT("Gimg\\setitemeffect7.spr"));	//주변에 도는거
	Add(GIMG_SETITEME0,						TEXT("Gimg\\setiteme.spr"));		//바닥
	Add(GIMG_SETITEME2,						TEXT("Gimg\\setiteme2.spr"));		//바닥
	Add(GIMG_SETITEME3,						TEXT("Gimg\\setiteme3.spr"));		//바닥
	Add(GIMG_SETITEME4,						TEXT("Gimg\\setiteme4.spr"));		//바닥
	Add(GIMG_SETITEME5,						TEXT("Gimg\\setiteme5.spr"));		//바닥
	Add(GIMG_SETITEME6,						TEXT("Gimg\\setiteme6.spr"));		//바닥
	Add(GIMG_SETITEME7,						TEXT("Gimg\\setiteme7.spr"));		//바닥
		

	Add(GIMG_MAGIC_BLESSDAMAGE,				TEXT("Gimg\\MAGIC_BLESSDAMAGE.spr"));
	Add(GIMG_MAGIC_BLESSCIRICAL1,			TEXT("Gimg\\MAGIC_BLESSCIRICAL1.spr"));
	Add(GIMG_MAGIC_BLESSCIRICAL2,			TEXT("Gimg\\MAGIC_BLESSCIRICAL2.spr"));
	Add(GIMG_MAGIC_BLESSHITRATE,			TEXT("Gimg\\MAGIC_BLESSHITRATE.spr"));
	Add(GIMG_MAGIC_BLESSRANGE,				TEXT("Gimg\\MAGIC_BLESSRANGE.spr"));
	Add(GIMG_MAGIC_BLESSATTACKINTERVAL,		TEXT("Gimg\\MAGIC_BLESSATTACKINTERVAL.spr"));
	Add(GIMG_MAGIC_BLESSMPRECOVERYTIME,		TEXT("Gimg\\MAGIC_BLESSMPRECOVERYTIME.spr"));
	Add(GIMG_MAGIC_BLESSHP,					TEXT("Gimg\\MAGIC_BLESSHP.spr"));
	Add(GIMG_MAGIC_AURABLADE,				TEXT("Gimg\\magic_aurablade.spr"));
	Add(GIMG_PVP_LEAGUE,					TEXT("Gimg\\fieldpk_mark.spr"));
	Add(GIMG_VALENTINEWAR,					TEXT("GImg\\valentinewar.spr"));			// 발렌타인 데이 전쟁 
	Add(GIMG_ELIXIR,						TEXT("GImg\\Elixir_efx.spr"));				// [진성] 엘릭서 이펙트.

	// 무기술의 달인 - 엠블렘
	Add(GIMG_EMBLEM,						TEXT("GImg\\emblem.SPR"));
	Add(GIMG_MAGIC_GREATWALLASURA,			TEXT("GImg\\Magic_GreatWallAsura.spr"));	// 버려진요새 아수라 - 하늘에서 떨어지는 석상.
	
	Add(GIMG_GUILDWAR_KILLSIGN,				TEXT("GIMG\\killsign.spr"));				//	[종호] - 길드전 킬뎃 표시 칼 이미지
	Add(GIMG_GUILDWAR_TIME,					TEXT("GIMG\\GuildWarTime.spr"));			//	[종호] - 길드전 시간 표시;

	Add(GIMG_WEDDING_EFT_S,					TEXT("GImg\\Wedding_eftS.SPR"));			// [영훈] 웨딩 이펙트
	Add(GIMG_WEDDING_EFT_M,					TEXT("GImg\\Wedding_eftM.SPR"));
	Add(GIMG_WEDDING_EFT_L,					TEXT("GImg\\Wedding_eftL.SPR"));

	Add(GIMG_HEAL_Y,						TEXT("GImg\\heal_Y.SPR"));					// [영훈] 노란색 힐 이미지

	Add(GIMG_PCROOM_CHAR_HEAD,				TEXT("GImg\\PCRoom_Char_Head.SPR"));		

	Add(GIMG_HEAL_R,						TEXT("GImg\\heal_R.SPR"));					// [춘기 붉은색 힐 이미지

	// 가위바위보 이미지
	Add(GIMG_GAWIBAWIBO,					TEXT("GImg\\GawiBawiBo.SPR"));				// [종호] 가위바위보 이미지


	Add(GIMG_ROUNDCOUNT,					TEXT("GImg\\Round.SPR"));					// [성웅] 라운드 카운트 이미지
	Add(GIMG_STUN,							TEXT("GImg\\Stun.Spr"));					// [성웅] 스턴 이미지
	Add(GIMG_DRAGON_STUN,					TEXT("GImg\\Magic_Dragon_Stun.SPR"));		// [진성] 드래곤 스턴.

	Add(GIMG_HEAL_P,						TEXT("GImg\\heal_P.SPR"));					// [춘기] 보라색 힐 이미지

	Add(GIMG_CURSE_ATTACK,					TEXT("GImg\\CURSE_ATTACK.SPR"));			// [진성] 저주
	Add(GIMG_CURSE_DEFENSE,					TEXT("GImg\\CURSE_DEFENSE.SPR"));			// [진성] 
	Add(GIMG_CURSE_DODGE,					TEXT("GImg\\CURSE_DODGE.SPR"));				// [진성] 
	Add(GIMG_CURSE_HIT,						TEXT("GImg\\CURSE_HIT.SPR"));				// [진성] 

	Add(GIMG_HEAL_B,						TEXT("GImg\\heal_B.SPR"));					// [성웅] 아이스 정령 이미지 
	
	Add(GIMG_VALENTINEDAY_HEART,			TEXT("GImg\\ValentineDayHeart.SPR"));		// [진성] 발렌타인 데이 하트.
}


cltGlobalImgFile::~cltGlobalImgFile()
{
	SI32 i;

	for(i = 0;i < MAX_GLOBAL_IMG_FILE_NUMBER;i++)
	{
		if( pTSpr[i] )
		{

			if(pTSpr[i]->pImage)
			{
				GP.FreeSpr( *(pTSpr[i]) );
			}
			delete pTSpr[i];
			pTSpr[i] = NULL;
		}


		if(pszFileName[i])
		{
			delete[] pszFileName[i];
			pszFileName[i] = NULL;
		}
	}

}


void cltGlobalImgFile::Add(SI32 index, TCHAR* pfilename)
{
	if(pszFileName[index])
	{
		MsgBox(TEXT("fdsf38jf"), TEXT("Fd3f:%d %s"), index, pfilename);
		return ;
	}

	SI32 size = _tcslen(pfilename) + 1;
	if(size >= 256)
	{
		MsgBox(TEXT("vcv3fd"), TEXT("v3fsd:%d"), size);
		return ;
	}

	pszFileName[index] = new TCHAR[ size ];

	StringCchCopy(pszFileName[index], size, pfilename);

}

TSpr* cltGlobalImgFile::GetGlobalSpr( SI32 index )
{
	if(index < 0)
		return NULL;

	if(pTSpr[index] == NULL)
	{
		if(pszFileName[index])
		{
			pTSpr[index] = new TSpr;
			if(pTSpr[index])
			{
				GP.LoadSprFromFile(pszFileName[index],	pTSpr[index]);
			}
		}
	}

	return pTSpr[index];
}

SI16 cltGlobalImgFile::GetBufImageFontIndex(SI16 siBufType)
{
	SI16	siRetFontIndex = 0;
	if ( (siBufType < 0) || (siBufType >= BUF_END) ) return siRetFontIndex;

	switch(siBufType)
	{
	case BUF_ATTACKUP:				siRetFontIndex = 0;		break;
	case BUF_DEFENSEUP:				siRetFontIndex = 1;		break;
	case BUF_SPEEDUP:				siRetFontIndex = 2;		break;
	case BUF_MAKEEXPUP:				siRetFontIndex = 3;		break;
	case BUF_NOFIRSTATTACK:			siRetFontIndex = 4;		break;
	case BUF_SUMMONATACUP:			siRetFontIndex = 5;		break;
	case BUF_LOVEPOWER:				siRetFontIndex = 6;		break;
	case BUF_FRIENDSHIP:			siRetFontIndex = 7;		break;
	case BUF_HINAMOCHI:				siRetFontIndex = 8;		break;
	case BUF_SUCCESS_MANUFACTURE:	siRetFontIndex = 9;		break;
	case BUF_MOFUMOFU_PERFUME:		siRetFontIndex = 10;	break;
	case BUF_FOURLEAF:				siRetFontIndex = 11;	break;
	case BUF_PREMIUMEXPUP:			siRetFontIndex = 12;	break;
	case BUF_HAPPYBEAN:				siRetFontIndex = 14;	break;
	case BUF_ATTACKUP30:			siRetFontIndex = 15;	break;
	case BUF_DEFENSEUP30:			siRetFontIndex = 16;	break;
	case BUF_SUPEREXPUP:			siRetFontIndex = 17;	break;
	case BUF_NOTBONUSTIME_MAKEEXPUP:siRetFontIndex = 3;		break;
	case BUF_THANKSGIVING_DAY:		siRetFontIndex = 18;	break;
	case BUF_FARMINGNMININGFOREVER:	siRetFontIndex = 22;	break;
	case BUF_STATUE_UP_STR:			siRetFontIndex = 23;	break;
	case BUF_STATUE_UP_DEX:			siRetFontIndex = 24;	break;
	case BUF_STATUE_UP_MAG:			siRetFontIndex = 25;	break;
	case BUF_STATUE_UP_VIT:			siRetFontIndex = 26;	break;
	case BUF_STATUE_UP_HAND:		siRetFontIndex = 27;	break;
	case BUF_EXP_UP:				siRetFontIndex = 28;	break;
	case BUF_REWARDBONUS:			siRetFontIndex = 29;	break;
	case BUF_OLYMPICMEDAL:			siRetFontIndex = 23;	break;
	case BUF_EVENT_EGGMOONCAKE:		siRetFontIndex = 30;	break;
	case BUF_MENTOR:				siRetFontIndex = 33;	break;
	case BUF_PREMIUMMAKEEXPUP:		siRetFontIndex = 32;	break;
	case BUF_SPECIALAMPUL_EXPUP300:	siRetFontIndex = 32;    break;
	case BUF_WATERMELON:			siRetFontIndex = 37;	break;
	case BUF_STATUE_UP_WIS:			siRetFontIndex = 41;	break;
	case BUF_EVENT_FORTUNE_BAD:		siRetFontIndex = 43;	break;
	case BUF_CHRISTMASEVENT_SNOWBLESS:	siRetFontIndex = 44;	break;
	case BUF_RESIDENTS_STRONG:		siRetFontIndex = 47;	break; 
	case BUF_RESIDENTS_EXPUP_10:	siRetFontIndex = 48;	break;
	case BUF_VALENTINEDAY:			siRetFontIndex = 49;	break;		
	}

	return siRetFontIndex;
}
