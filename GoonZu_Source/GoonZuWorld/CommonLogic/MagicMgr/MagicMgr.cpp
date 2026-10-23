#include "MagicMgr.h"

#include "../../Common/SMLib/LibList/LibList.h"
#include "../../Common/Char/CharCommon/Char-Common.h"
#include "../../Common/Char/CharServer/Char-Server.h"
#include "../../Common/Char/CharManager/CharManager.h"
#include "../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../Common/Item/ItemCommon/ItemCommon.h"
#include "../../Common/Skill/Skill-Manager.h"
#include "../../common/PersonInfo.h"

#include "./MagicAttribute/MagicAttribute.h"

#include "./MagicFireBallObj/MagicFireBallObj.h"
#include "./MagicBigWindObj/MagicBigWindObj.h"
#include "./MagicThunderObj/MagicThunderObj.h"
#include "./MagicThunder1Obj/MagicThunder1Obj.h"
#include "./MagicWhirlPoolObj/MagicWhirlPoolObj.h"
#include "./MagicFlameObj/MagicFlameObj.h"
#include "./MagicIceObj/MagicIceObj.h"
#include "./MagicPoisonObj/MagicPoisonObj.h"

#include "./MagicHealObj/MagicHealObj.h"
#include "./MagicRockSlideObj/MagicRockSlideObj.h"

#include "./MagicLightingFistObj/MagicLightingFistObj.h"
#include "./MagicPoisonFlameObj/MagicPoisonFlameObj.h"
#include "./MagicDragonCannonObj/MagicDragonCannonObj.h"
#include "./MagicDropMoneyObj/MagicDropMoneyObj.h"
#include "./MagicBlackInkObj/MagicBlackInkObj.h"
#include "./MagicDropSnowmanObj/MagicDropSnowmanObj.h"
#include "./MagicDropForkObj/MagicDropForkObj.h"
                     
#include "./MagicCurseDefenseObj/MagicCurseDefenseObj.h"
#include "./MagicCurseMovespeedObj/MagicCurseMovespeedObj.h"
#include "./MagicThrowKnifeObj/MagicThrowKnifeObj.h"
#include "./MagicLaserBeamObj/MagicLaserBeamObj.h"
#include "./MagicFireGroundObj/MagicFireGroundObj.h"
#include "./MagicSwordBombObj/MagicSwordBombObj.h"
#include "./MagicFreezeExplosionObj/MagicFreezeExplosionObj.h"
#include "./MagicUpperDragon/MagicUpperDragon.h"
#include "./MagicPalmWind/MagicPalmWind.h"
#include "./MagicFireBurnObj/MagicFireBurnObj.h"
#include "./MagicFireWhirlWindObj/MagicFireWhirlWindObj.h"
#include "./MagicEarthQuakeObj/MagicEarthQuakeObj.h"
#include "./MagicDropGreatWallAsuraObj/MagicDropGreatWallAsuraObj.h"
#include "./MagicDragonSummonPetObj/MagicDragonSummonPetObj.h"
#include "./MagicSkillBookObj/MagicSkillBookObj.h"

#include "MagicMsgObj.h"


#include "../../Client/client.h"

extern cltCommonLogic* pclClient;


CMagicMgr::CMagicMgr() : CLibListMgr()
{
	//////////////////////////////////////////////////////////////////////////
	// NEW MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_NEW_LIST(Exist, Empty)	{ Exist = new CLibList();	Empty = new CLibList();	}
	//////////////////////////////////////////////////////////////////////////

	// 
	MAGIC_NEW_LIST(m_pMagicFireBallExistList,			m_pMagicFireBallEmptyList);
	MAGIC_NEW_LIST(m_pMagicBigWindExistList,			m_pMagicBigWindEmptyList);
	MAGIC_NEW_LIST(m_pMagicThunderExistList,			m_pMagicThunderEmptyList);
	MAGIC_NEW_LIST(m_pMagicThunder1ExistList,			m_pMagicThunder1EmptyList);
	MAGIC_NEW_LIST(m_pMagicWhirlPoolExistList,			m_pMagicWhirlPoolEmptyList);
	MAGIC_NEW_LIST(m_pMagicFlameExistList,				m_pMagicFlameEmptyList);
	MAGIC_NEW_LIST(m_pMagicRockSlideExistList,			m_pMagicRockSlideEmptyList);
	MAGIC_NEW_LIST(m_pMagicHealExistList,				m_pMagicHealEmptyList);
	MAGIC_NEW_LIST(m_pMagicPoisonExistList,				m_pMagicPoisonEmptyList);
	MAGIC_NEW_LIST(m_pMagicIceExistList,				m_pMagicIceEmptyList);
	MAGIC_NEW_LIST(m_pMagicLightingFistExistList,		m_pMagicLightingFistEmptyList);
	MAGIC_NEW_LIST(m_pMagicPoisonFlameExistList,		m_pMagicPoisonFlameEmptyList);
	MAGIC_NEW_LIST(m_pMagicMeteorExistList,				m_pMagicMeteorEmptyList);
	MAGIC_NEW_LIST(m_pMagicDropMoneyExistList,			m_pMagicDropMoneyEmptyList);
	MAGIC_NEW_LIST(m_pMagicBlackInkExistList,			m_pMagicBlackInkEmptyList);
	MAGIC_NEW_LIST(m_pMagicDropSnowmanExistList,		m_pMagicDropSnowmanEmptyList);
	MAGIC_NEW_LIST(m_pMagicDropForkExistList,			m_pMagicDropForkEmptyList);
	MAGIC_NEW_LIST(m_pMagicCurseDefenseExistList,		m_pMagicCurseDefenseEmptyList);
	MAGIC_NEW_LIST(m_pMagicCurseMovespeedExistList,		m_pMagicCurseMovespeedEmptyList);
	MAGIC_NEW_LIST(m_pMagicThrowKnifeExistList,			m_pMagicThrowKnifeEmptyList);
	MAGIC_NEW_LIST(m_pMagicLaserBeamExistList,			m_pMagicLaserBeamEmptyList);
	MAGIC_NEW_LIST(m_pMagicTriMeteorExistList,			m_pMagicTriMeteorEmptyList);
	MAGIC_NEW_LIST(m_pMagicFireGroundExistList,			m_pMagicFireGroundEmptyList);
	MAGIC_NEW_LIST(m_pMagicSwordBombExistList,			m_pMagicSwordBombEmptyList);

	MAGIC_NEW_LIST(m_pMagicFreezeExplosionExistList,	m_pMagicFreezeExplosionEmptyList);

	MAGIC_NEW_LIST(m_pMagicUpperDragonExistList,		m_pMagicUpperDragonEmptyList);
	MAGIC_NEW_LIST(m_pMagicPalmWindExistList,			m_pMagicPalmWindEmptyList);
	MAGIC_NEW_LIST(m_pMagicSummonDragonExistList,		m_pMagicSummonDragonEmptyList);

	MAGIC_NEW_LIST(m_pMagicFireBurnExistList,			m_pMagicFireBurnEmptyList);

	MAGIC_NEW_LIST(m_pMagicSkellGhostExistList,			m_pMagicSkellGhostEmptyList);
	MAGIC_NEW_LIST(m_pMagicSwordMeteorExistList,		m_pMagicSwordMeteorEmptyList);
	MAGIC_NEW_LIST(m_pMagicFrozenExistList,				m_pMagicFrozenEmptyList);

	MAGIC_NEW_LIST(m_pMagicTwinSpearExistList,			m_pMagicTwinSpearEmptyList);
	MAGIC_NEW_LIST(m_pMagicFireWhirlWindExistList,		m_pMagicFireWhirlWindEmptyList);
	MAGIC_NEW_LIST(m_pMagicNorthWindExistList,			m_pMagicNorthWindEmptyList);
	MAGIC_NEW_LIST(m_pMagicFireStormExistList,			m_pMagicFireStormEmptyList);
	MAGIC_NEW_LIST(m_pMagicEarthQuakeExistList,			m_pMagicEarthQuakeEmptyList);
	MAGIC_NEW_LIST(m_pMagicSpearDragonExistList,		m_pMagicSpearDragonEmptyList);

	MAGIC_NEW_LIST(m_pMagicGargoyleExistList,			m_pMagicGargoyleEmptyList);
	MAGIC_NEW_LIST(m_pMagicCerberusExistList,			m_pMagicCerberusEmptyList);

	MAGIC_NEW_LIST(m_pMagicFrozenSorcererExistList,		m_pMagicFrozenSorcererEmptyList);
	MAGIC_NEW_LIST(m_pMagicMarionetteExistList,			m_pMagicMarionetteEmptyList);
	MAGIC_NEW_LIST(m_pMagicFrokenExistList,				m_pMagicFrokenEmptyList);
	MAGIC_NEW_LIST(m_pMagicIcehoundExistList,			m_pMagicIcehoundEmptyList);
	MAGIC_NEW_LIST(m_pMagicFrozenGolemExistList,		m_pMagicFrozenGolemEmptyList);
	MAGIC_NEW_LIST(m_pMagicFrozenKnightExistList,		m_pMagicFrozenKnightEmptyList);
	MAGIC_NEW_LIST(m_pMagicFrozenQueenExistList,		m_pMagicFrozenQueenEmptyList);

	MAGIC_NEW_LIST(m_pMagicGotpaExsitList,				m_pMagicGotpaEmptyList);
	MAGIC_NEW_LIST(m_pMagicNinjaPandaExistList,			m_pMagicNinjaPandaEmptyList);
	MAGIC_NEW_LIST(m_pMagicRaccoonDogExistList,			m_pMagicRaccoonDogEmptyList);
	MAGIC_NEW_LIST(m_pMagicBossMonkeyExistList,			m_pMagicBossMonkeyEmptyList);
	MAGIC_NEW_LIST(m_pMagicToadPoisonExistList,			m_pMagicToadPoisonEmptyList);
	MAGIC_NEW_LIST(m_pMagicBloodBatExistList,			m_pMagicBloodBatEmptyList);
	MAGIC_NEW_LIST(m_pMagicWerewolfExistList,			m_pMagicWerewolfEmptyList);
	MAGIC_NEW_LIST(m_pMagicDevilousExistList,			m_pMagicDevilousEmptyList);
	MAGIC_NEW_LIST(m_pMagicGangsiExistList,				m_pMagicGangsiEmptyList);
	MAGIC_NEW_LIST(m_pMagicDraculaExistList,			m_pMagicDraculaEmptyList);
	MAGIC_NEW_LIST(m_pMagicPriestExistList,				m_pMagicPriestEmptyList);
	MAGIC_NEW_LIST(m_pMagicHarpyExistList,				m_pMagicHarpyEmptyList);
	MAGIC_NEW_LIST(m_pMagicMandragoraExistList,			m_pMagicMandragoraEmptyList);
	MAGIC_NEW_LIST(m_pMagicGiraffeExistList,			m_pMagicGiraffeEmptyList);
	MAGIC_NEW_LIST(m_pMagicPapaGoblinExistList,			m_pMagicPapaGoblinEmptyList);
	MAGIC_NEW_LIST(m_pMagicPirateSoldierExistList,		m_pMagicPirateSoldierEmptyList);
	MAGIC_NEW_LIST(m_pMagicPirateArcherExistList,		m_pMagicPirateArcherEmptyList);
	MAGIC_NEW_LIST(m_pMagicBlackBossExistList,			m_pMagicBlackBossEmptyList);
	MAGIC_NEW_LIST(m_pMagicEmperorCavalryExistList,		m_pMagicEmperorCavalryEmptyList);
	MAGIC_NEW_LIST(m_pMagicEmperorMasterExistList,		m_pMagicEmperorMasterEmptyList);
	MAGIC_NEW_LIST(m_pMagicGreatWallSoldierExistList,	m_pMagicGreatWallSoldierEmptyList);
	MAGIC_NEW_LIST(m_pMagicGreatWallMagicianExistList,	m_pMagicGreatWallMagicianEmptyList);
	MAGIC_NEW_LIST(m_pMagicEmperorWarriorExistList,		m_pMagicEmperorWarriorEmptyList);
	MAGIC_NEW_LIST(m_pMagicGreatWallSpiderExistList,	m_pMagicGreatWallSpiderEmptyList);
	MAGIC_NEW_LIST(m_pMagicGreatWallTwinHeadExistList,	m_pMagicGreatWallTwinHeadEmptyList);
	MAGIC_NEW_LIST(m_pMagicBearTangExistList,			m_pMagicBearTangEmptyList);
	MAGIC_NEW_LIST(m_pMagicSuperBearTangExistList,		m_pMagicSuperBearTangEmptyList);
	MAGIC_NEW_LIST(m_pMagicSFrozenGolemExistList,		m_pMagicSFrozenGolemEmptyList);
	MAGIC_NEW_LIST(m_pMagicSFrozenQueenExistList,		m_pMagicSFrozenQueenEmptyList);
	MAGIC_NEW_LIST(m_pMagicCatPetExistList,				m_pMagicCatPetEmptyList);
	MAGIC_NEW_LIST(m_pMagicAuraBladeExistList,			m_pMagicAuraBladeEmptyList);
	MAGIC_NEW_LIST(m_pMagicTripleDeathExistList,		m_pMagicTripleDeathEmptyList);
	MAGIC_NEW_LIST(m_pMagicCosmosRabbitExistList,		m_pMagicCosmosRabbitEmptyList);
	MAGIC_NEW_LIST(m_pMagicRudolphpetExistList,			m_pMagicRudolphpetEmptyList);
	MAGIC_NEW_LIST(m_pMagicDropGreatWallAsuraExistList,	m_pMagicDropGreatWallAsuraEmptyList);
	MAGIC_NEW_LIST(m_pMagicAmbersaurusExistList,		m_pMagicAmbersaurusEmptyList);
	MAGIC_NEW_LIST(m_pMagicRollingstegoExiteList,		m_pMagicRollingstegoEmptyList);
	MAGIC_NEW_LIST(m_pMagicPumkinExiteList,				m_pMagicPumkinEmptyList);	// 펌킨
	MAGIC_NEW_LIST(m_pMagicDragonFireExistList,			m_pMagicDragonFireEmptyList);	
	MAGIC_NEW_LIST(m_pMagicDragonStunExistList,			m_pMagicDragonStunEmptyList);	
	
	MAGIC_NEW_LIST(m_pMagicSkillBookCrashExistList,			m_pMagicSkillBookCrashEmptyList			);
	MAGIC_NEW_LIST(m_pMagicSkillBookFlameBusterExistList,	m_pMagicSkillBookFlameBusterEmptyList	);
	MAGIC_NEW_LIST(m_pMagicSkillBookFlameWhirlExistList,	m_pMagicSkillBookFlameWhirlEmptyList	);
	MAGIC_NEW_LIST(m_pMagicSkillBookIceRainExistList,		m_pMagicSkillBookIceRainEmptyList		);
	MAGIC_NEW_LIST(m_pMagicSkillBookSlingExistList,			m_pMagicSkillBookSlingEmptyList			);
	MAGIC_NEW_LIST(m_pMagicSkillBookTornadoExistList,		m_pMagicSkillBookTornadoEmptyList		);
	MAGIC_NEW_LIST(m_pMagicSkillBookWetFogExistList,		m_pMagicSkillBookWetFogEmptyList		);	
	MAGIC_NEW_LIST(m_pMagicSkillBookWindcuterExistList,		m_pMagicSkillBookWindcuterEmptyList		);	

	MAGIC_NEW_LIST(m_pMagicClownBallExistList,				m_pMagicClownBallEmptyList);
	MAGIC_NEW_LIST(m_pMagicBouquetExistList,				m_pMagicBouquetEmptyList);

	m_pMagicAttribute = new CMagicAttribute();

	m_pMagicAttribute->AddMagics();		// 모든 마법을 등록한다.
	m_pMagicAttribute->ReadFileData();	// Txt파일로부터 읽어와 설정한다.

	MAGIC_NEW_LIST(m_pMsgExistList, m_pMsgEmptyList )	

	CreateMaxMsg();

	CreateMaxEmptyObj();

	Initialize();
	
	// DEBUG
#ifdef _SAFE_MEMORY
	ServerDebugMagicUse.ZeroMem();
#else
	memset(ServerDebugMagicUse, 0 ,sizeof(ServerDebugMagicUse));
#endif
	ServerDebugClock = pclClient->CurrentClock;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CMagicMgr::~CMagicMgr()
{
	//////////////////////////////////////////////////////////////////////////
	// DELTE MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_SAFE_DELETE(Exist, Empty)	\
	{ if(Exist) { delete Exist; Exist = NULL; } if(Empty) { delete Empty; Empty = NULL; } }
	//////////////////////////////////////////////////////////////////////////


	Destroy();

	// 
	MAGIC_SAFE_DELETE(m_pMagicFireBallExistList,			m_pMagicFireBallEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBigWindExistList,				m_pMagicBigWindEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicThunderExistList,				m_pMagicThunderEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicThunder1ExistList,			m_pMagicThunder1EmptyList);
	MAGIC_SAFE_DELETE(m_pMagicWhirlPoolExistList,			m_pMagicWhirlPoolEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFlameExistList,				m_pMagicFlameEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicHealExistList,				m_pMagicHealEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicRockSlideExistList,			m_pMagicRockSlideEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPoisonExistList,				m_pMagicPoisonEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicIceExistList,					m_pMagicIceEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicLightingFistExistList,		m_pMagicLightingFistEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPoisonFlameExistList,			m_pMagicPoisonFlameEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicMeteorExistList,				m_pMagicMeteorEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDropMoneyExistList,			m_pMagicDropMoneyEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBlackInkExistList,			m_pMagicBlackInkEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDropSnowmanExistList,			m_pMagicDropSnowmanEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDropForkExistList,			m_pMagicDropForkEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicCurseDefenseExistList,		m_pMagicCurseDefenseEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicCurseMovespeedExistList,		m_pMagicCurseMovespeedEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicThrowKnifeExistList,			m_pMagicThrowKnifeEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicLaserBeamExistList,			m_pMagicLaserBeamEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicTriMeteorExistList,			m_pMagicTriMeteorEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFireGroundExistList,			m_pMagicFireGroundEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSwordBombExistList,			m_pMagicSwordBombEmptyList);

	MAGIC_SAFE_DELETE(m_pMagicFreezeExplosionExistList,		m_pMagicFreezeExplosionEmptyList);

	MAGIC_SAFE_DELETE(m_pMagicUpperDragonExistList,			m_pMagicUpperDragonEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPalmWindExistList,			m_pMagicPalmWindEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSummonDragonExistList,		m_pMagicSummonDragonEmptyList);

	MAGIC_SAFE_DELETE(m_pMagicFireBurnExistList,			m_pMagicFireBurnEmptyList);

	MAGIC_SAFE_DELETE(m_pMagicSkellGhostExistList,			m_pMagicSkellGhostEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSwordMeteorExistList,			m_pMagicSwordMeteorEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFrozenExistList,				m_pMagicFrozenEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicTwinSpearExistList,			m_pMagicTwinSpearEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFireWhirlWindExistList,		m_pMagicFireWhirlWindEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicNorthWindExistList,			m_pMagicNorthWindEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFireStormExistList,			m_pMagicFireStormEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicEarthQuakeExistList,			m_pMagicEarthQuakeEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSpearDragonExistList,			m_pMagicSpearDragonEmptyList);

	MAGIC_SAFE_DELETE(m_pMagicGargoyleExistList,			m_pMagicGargoyleEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicCerberusExistList,			m_pMagicCerberusEmptyList);
	
	MAGIC_SAFE_DELETE(m_pMagicFrozenSorcererExistList,		m_pMagicFrozenSorcererEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicMarionetteExistList,			m_pMagicMarionetteEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFrokenExistList,				m_pMagicFrokenEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicIcehoundExistList,			m_pMagicIcehoundEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFrozenGolemExistList,			m_pMagicFrozenGolemEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFrozenKnightExistList,		m_pMagicFrozenKnightEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicFrozenQueenExistList,			m_pMagicFrozenQueenEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGotpaExsitList,				m_pMagicGotpaEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicNinjaPandaExistList,			m_pMagicNinjaPandaEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicRaccoonDogExistList,			m_pMagicRaccoonDogEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBossMonkeyExistList,			m_pMagicBossMonkeyEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicToadPoisonExistList,			m_pMagicToadPoisonEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBloodBatExistList,			m_pMagicBloodBatEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicWerewolfExistList,			m_pMagicWerewolfEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDevilousExistList,			m_pMagicDevilousEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGangsiExistList,				m_pMagicGangsiEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDraculaExistList,				m_pMagicDraculaEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPriestExistList,				m_pMagicPriestEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicHarpyExistList,				m_pMagicHarpyEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicMandragoraExistList,			m_pMagicMandragoraEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGiraffeExistList,				m_pMagicGiraffeEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPapaGoblinExistList,			m_pMagicPapaGoblinEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPirateSoldierExistList,		m_pMagicPirateSoldierEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPirateArcherExistList,		m_pMagicPirateArcherEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBlackBossExistList,		    m_pMagicBlackBossEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicEmperorCavalryExistList,		m_pMagicEmperorCavalryEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicEmperorMasterExistList,		m_pMagicEmperorMasterEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGreatWallSoldierExistList,	m_pMagicGreatWallSoldierEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGreatWallMagicianExistList,	m_pMagicGreatWallMagicianEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicEmperorWarriorExistList,		m_pMagicEmperorWarriorEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGreatWallSpiderExistList,		m_pMagicGreatWallSpiderEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicGreatWallTwinHeadExistList,	m_pMagicGreatWallTwinHeadEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBearTangExistList,			m_pMagicBearTangEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSuperBearTangExistList,		m_pMagicSuperBearTangEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSFrozenGolemExistList,		m_pMagicSFrozenGolemEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicSFrozenQueenExistList,		m_pMagicSFrozenQueenEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicCatPetExistList,				m_pMagicCatPetEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicAuraBladeExistList,			m_pMagicAuraBladeEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicTripleDeathExistList,			m_pMagicTripleDeathEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicCosmosRabbitExistList,		m_pMagicCosmosRabbitEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicRudolphpetExistList,			m_pMagicRudolphpetEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicDropGreatWallAsuraExistList,	m_pMagicDropGreatWallAsuraEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicAmbersaurusExistList,			m_pMagicAmbersaurusEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicRollingstegoExiteList,		m_pMagicRollingstegoEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicPumkinExiteList,				m_pMagicPumkinEmptyList);			// 펌킨
	MAGIC_SAFE_DELETE(m_pMagicDragonStunExistList,			m_pMagicDragonStunEmptyList);	
	MAGIC_SAFE_DELETE(m_pMagicDragonFireExistList,			m_pMagicDragonFireEmptyList);	

	MAGIC_SAFE_DELETE(m_pMagicSkillBookCrashExistList,			m_pMagicSkillBookCrashEmptyList			);
	MAGIC_SAFE_DELETE(m_pMagicSkillBookFlameBusterExistList,	m_pMagicSkillBookFlameBusterEmptyList	);
	MAGIC_SAFE_DELETE(m_pMagicSkillBookFlameWhirlExistList,		m_pMagicSkillBookFlameWhirlEmptyList	);
	MAGIC_SAFE_DELETE(m_pMagicSkillBookIceRainExistList,		m_pMagicSkillBookIceRainEmptyList		);
	MAGIC_SAFE_DELETE(m_pMagicSkillBookSlingExistList,			m_pMagicSkillBookSlingEmptyList			);
	MAGIC_SAFE_DELETE(m_pMagicSkillBookTornadoExistList,		m_pMagicSkillBookTornadoEmptyList		);			// 펌킨
	MAGIC_SAFE_DELETE(m_pMagicSkillBookWetFogExistList,			m_pMagicSkillBookWetFogEmptyList		);	
	MAGIC_SAFE_DELETE(m_pMagicSkillBookWindcuterExistList,		m_pMagicSkillBookWindcuterEmptyList		);	

	MAGIC_SAFE_DELETE(m_pMagicClownBallExistList,				m_pMagicClownBallEmptyList);
	MAGIC_SAFE_DELETE(m_pMagicBouquetExistList,					m_pMagicBouquetEmptyList);
	
	MAGIC_SAFE_DELETE(m_pMsgExistList,						m_pMsgEmptyList);
	if(m_pMagicAttribute) { delete m_pMagicAttribute; m_pMagicAttribute= NULL; } 
}

void CMagicMgr::Initialize()
{
	return;
}

void CMagicMgr::Destroy()
{
	return;
}

void CMagicMgr::DeleteMagic()
{

	//////////////////////////////////////////////////////////////////////////
	// MAGIC INIT MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_INIT_LIST(Exist, Empty)	if(Exist) ListInitialize(Exist, Empty);
	//////////////////////////////////////////////////////////////////////////

	// 
	MAGIC_INIT_LIST(m_pMagicFireBallExistList,			m_pMagicFireBallEmptyList)
	MAGIC_INIT_LIST(m_pMagicBigWindExistList,			m_pMagicBigWindEmptyList)
	MAGIC_INIT_LIST(m_pMagicThunderExistList,			m_pMagicThunderEmptyList)
	MAGIC_INIT_LIST(m_pMagicThunder1ExistList,			m_pMagicThunder1EmptyList)
	MAGIC_INIT_LIST(m_pMagicWhirlPoolExistList,			m_pMagicWhirlPoolEmptyList)
	MAGIC_INIT_LIST(m_pMagicFlameExistList,				m_pMagicFlameEmptyList)
	MAGIC_INIT_LIST(m_pMagicRockSlideExistList,			m_pMagicRockSlideEmptyList)
	MAGIC_INIT_LIST(m_pMagicHealExistList,				m_pMagicHealEmptyList)
	MAGIC_INIT_LIST(m_pMagicPoisonExistList,			m_pMagicPoisonEmptyList)
	MAGIC_INIT_LIST(m_pMagicIceExistList,				m_pMagicIceEmptyList)
	MAGIC_INIT_LIST(m_pMagicLightingFistExistList,		m_pMagicLightingFistEmptyList);
	MAGIC_INIT_LIST(m_pMagicPoisonFlameExistList,		m_pMagicPoisonFlameEmptyList);
	MAGIC_INIT_LIST(m_pMagicMeteorExistList,			m_pMagicMeteorEmptyList);
	MAGIC_INIT_LIST(m_pMagicDropMoneyExistList,			m_pMagicDropMoneyEmptyList);
	MAGIC_INIT_LIST(m_pMagicBlackInkExistList,			m_pMagicBlackInkEmptyList);
	MAGIC_INIT_LIST(m_pMagicDropSnowmanExistList,		m_pMagicDropSnowmanEmptyList);
	MAGIC_INIT_LIST(m_pMagicDropForkExistList,			m_pMagicDropForkEmptyList);
	MAGIC_INIT_LIST(m_pMagicCurseDefenseExistList,		m_pMagicCurseDefenseEmptyList);
	MAGIC_INIT_LIST(m_pMagicCurseMovespeedExistList,	m_pMagicCurseMovespeedEmptyList);
	MAGIC_INIT_LIST(m_pMagicThrowKnifeExistList,		m_pMagicThrowKnifeEmptyList);
	MAGIC_INIT_LIST(m_pMagicLaserBeamExistList,			m_pMagicLaserBeamEmptyList);
	MAGIC_INIT_LIST(m_pMagicTriMeteorExistList,			m_pMagicTriMeteorEmptyList);
	MAGIC_INIT_LIST(m_pMagicFireGroundExistList,		m_pMagicFireGroundEmptyList);
	MAGIC_INIT_LIST(m_pMagicSwordBombExistList,			m_pMagicSwordBombEmptyList);

	MAGIC_INIT_LIST(m_pMagicFreezeExplosionExistList,	m_pMagicFreezeExplosionEmptyList);

	MAGIC_INIT_LIST(m_pMagicUpperDragonExistList,		m_pMagicUpperDragonEmptyList);
	MAGIC_INIT_LIST(m_pMagicPalmWindExistList,			m_pMagicPalmWindEmptyList);
	MAGIC_INIT_LIST(m_pMagicSummonDragonExistList,		m_pMagicSummonDragonEmptyList);

	MAGIC_INIT_LIST(m_pMagicFireBurnExistList,			m_pMagicFireBurnEmptyList);

	MAGIC_INIT_LIST(m_pMagicSkellGhostExistList,		m_pMagicSkellGhostEmptyList);
	MAGIC_INIT_LIST(m_pMagicSwordMeteorExistList,		m_pMagicSwordMeteorEmptyList);
	MAGIC_INIT_LIST(m_pMagicFrozenExistList,			m_pMagicFrozenEmptyList);
	MAGIC_INIT_LIST(m_pMagicTwinSpearExistList,			m_pMagicTwinSpearEmptyList);
	MAGIC_INIT_LIST(m_pMagicFireWhirlWindExistList,		m_pMagicFireWhirlWindEmptyList);
	MAGIC_INIT_LIST(m_pMagicNorthWindExistList,			m_pMagicNorthWindEmptyList);
	MAGIC_INIT_LIST(m_pMagicFireStormExistList,			m_pMagicFireStormEmptyList);
	MAGIC_INIT_LIST(m_pMagicEarthQuakeExistList,		m_pMagicEarthQuakeEmptyList);
	MAGIC_INIT_LIST(m_pMagicSpearDragonExistList,		m_pMagicSpearDragonEmptyList);

	MAGIC_INIT_LIST(m_pMagicGargoyleExistList,			m_pMagicGargoyleEmptyList);
	MAGIC_INIT_LIST(m_pMagicCerberusExistList,			m_pMagicCerberusEmptyList);

	MAGIC_INIT_LIST(m_pMagicFrozenSorcererExistList,	m_pMagicFrozenSorcererEmptyList);
	MAGIC_INIT_LIST(m_pMagicMarionetteExistList,		m_pMagicMarionetteEmptyList);
	MAGIC_INIT_LIST(m_pMagicFrokenExistList,			m_pMagicFrokenEmptyList);
	MAGIC_INIT_LIST(m_pMagicIcehoundExistList,			m_pMagicIcehoundEmptyList);
	MAGIC_INIT_LIST(m_pMagicFrozenGolemExistList,		m_pMagicFrozenGolemEmptyList);
	MAGIC_INIT_LIST(m_pMagicFrozenKnightExistList,		m_pMagicFrozenKnightEmptyList);
	MAGIC_INIT_LIST(m_pMagicFrozenQueenExistList,		m_pMagicFrozenQueenEmptyList);
	MAGIC_INIT_LIST(m_pMagicGotpaExsitList,				m_pMagicGotpaEmptyList);
	MAGIC_INIT_LIST(m_pMagicNinjaPandaExistList,		m_pMagicNinjaPandaEmptyList);
	MAGIC_INIT_LIST(m_pMagicRaccoonDogExistList,		m_pMagicRaccoonDogEmptyList);
	MAGIC_INIT_LIST(m_pMagicBossMonkeyExistList,		m_pMagicBossMonkeyEmptyList);
	MAGIC_INIT_LIST(m_pMagicToadPoisonExistList,		m_pMagicToadPoisonEmptyList);
	MAGIC_INIT_LIST(m_pMagicBloodBatExistList,			m_pMagicBloodBatEmptyList);
	MAGIC_INIT_LIST(m_pMagicWerewolfExistList,			m_pMagicWerewolfEmptyList);
	MAGIC_INIT_LIST(m_pMagicDevilousExistList,			m_pMagicDevilousEmptyList);
	MAGIC_INIT_LIST(m_pMagicGangsiExistList,			m_pMagicGangsiEmptyList);
	MAGIC_INIT_LIST(m_pMagicDraculaExistList,			m_pMagicDraculaEmptyList);
	MAGIC_INIT_LIST(m_pMagicPriestExistList,			m_pMagicPriestEmptyList);
	MAGIC_INIT_LIST(m_pMagicHarpyExistList,				m_pMagicHarpyEmptyList);
	MAGIC_INIT_LIST(m_pMagicMandragoraExistList,		m_pMagicMandragoraEmptyList);
	MAGIC_INIT_LIST(m_pMagicGiraffeExistList,			m_pMagicGiraffeEmptyList);
	MAGIC_INIT_LIST(m_pMagicPapaGoblinExistList,		m_pMagicPapaGoblinEmptyList);
	MAGIC_INIT_LIST(m_pMagicPirateSoldierExistList,		m_pMagicPirateSoldierEmptyList);
	MAGIC_INIT_LIST(m_pMagicPirateArcherExistList,		m_pMagicPirateArcherEmptyList);
	MAGIC_INIT_LIST(m_pMagicBlackBossExistList,			m_pMagicBlackBossEmptyList);
	MAGIC_INIT_LIST(m_pMagicEmperorCavalryExistList,	m_pMagicEmperorCavalryEmptyList);
	MAGIC_INIT_LIST(m_pMagicEmperorMasterExistList,		m_pMagicEmperorMasterEmptyList);
	MAGIC_INIT_LIST(m_pMagicGreatWallSoldierExistList,	m_pMagicGreatWallSoldierEmptyList);
	MAGIC_INIT_LIST(m_pMagicGreatWallMagicianExistList,	m_pMagicGreatWallMagicianEmptyList);
	MAGIC_INIT_LIST(m_pMagicEmperorWarriorExistList,	m_pMagicEmperorWarriorEmptyList);
	MAGIC_INIT_LIST(m_pMagicGreatWallSpiderExistList,	m_pMagicGreatWallSpiderEmptyList);
	MAGIC_INIT_LIST(m_pMagicGreatWallTwinHeadExistList,	m_pMagicGreatWallTwinHeadEmptyList);
	MAGIC_INIT_LIST(m_pMagicBearTangExistList,			m_pMagicBearTangEmptyList);
	MAGIC_INIT_LIST(m_pMagicSuperBearTangExistList,		m_pMagicSuperBearTangEmptyList);
	MAGIC_INIT_LIST(m_pMagicSFrozenGolemExistList,		m_pMagicSFrozenGolemEmptyList);
	MAGIC_INIT_LIST(m_pMagicSFrozenQueenExistList,		m_pMagicSFrozenQueenEmptyList);
	MAGIC_INIT_LIST(m_pMagicCatPetExistList,			m_pMagicCatPetEmptyList);
	MAGIC_INIT_LIST(m_pMagicAuraBladeExistList,			m_pMagicAuraBladeEmptyList);
	MAGIC_INIT_LIST(m_pMagicTripleDeathExistList,		m_pMagicTripleDeathEmptyList);
	MAGIC_INIT_LIST(m_pMagicCosmosRabbitExistList,		m_pMagicCosmosRabbitEmptyList);
	MAGIC_INIT_LIST(m_pMagicRudolphpetExistList,		m_pMagicRudolphpetEmptyList);
	MAGIC_INIT_LIST(m_pMagicDropGreatWallAsuraExistList,m_pMagicDropGreatWallAsuraEmptyList);
	MAGIC_INIT_LIST(m_pMagicAmbersaurusExistList,		m_pMagicAmbersaurusEmptyList);
	MAGIC_INIT_LIST(m_pMagicRollingstegoExiteList,		m_pMagicRollingstegoEmptyList);
	MAGIC_INIT_LIST(m_pMagicPumkinExiteList,			m_pMagicPumkinEmptyList);	// 펌킨
	MAGIC_INIT_LIST(m_pMagicDragonStunExistList,		m_pMagicDragonStunEmptyList);
	MAGIC_INIT_LIST(m_pMagicDragonFireExistList,		m_pMagicDragonFireEmptyList);

	MAGIC_INIT_LIST(m_pMagicSkillBookCrashExistList,			m_pMagicSkillBookCrashEmptyList			);
	MAGIC_INIT_LIST(m_pMagicSkillBookFlameBusterExistList,		m_pMagicSkillBookFlameBusterEmptyList	);
	MAGIC_INIT_LIST(m_pMagicSkillBookFlameWhirlExistList,		m_pMagicSkillBookFlameWhirlEmptyList	);
	MAGIC_INIT_LIST(m_pMagicSkillBookIceRainExistList,			m_pMagicSkillBookIceRainEmptyList		);
	MAGIC_INIT_LIST(m_pMagicSkillBookSlingExistList,			m_pMagicSkillBookSlingEmptyList			);
	MAGIC_INIT_LIST(m_pMagicSkillBookTornadoExistList,			m_pMagicSkillBookTornadoEmptyList		);
	MAGIC_INIT_LIST(m_pMagicSkillBookWetFogExistList,			m_pMagicSkillBookWetFogEmptyList		);
	MAGIC_INIT_LIST(m_pMagicSkillBookWindcuterExistList,		m_pMagicSkillBookWindcuterEmptyList		);
	
	MAGIC_INIT_LIST(m_pMagicClownBallExistList,					m_pMagicClownBallEmptyList);
	MAGIC_INIT_LIST(m_pMagicBouquetExistList,					m_pMagicBouquetEmptyList);

	MAGIC_INIT_LIST(m_pMsgExistList,					m_pMsgEmptyList );
	return;
}

void CMagicMgr::ListInitialize( CLibList *pExistList, CLibList *pEmptyList )
{
	CObj *pObj= pExistList->PopFront();

	while( pObj )
	{
		pEmptyList->PushBack( pObj );
		pObj = pExistList->PopFront();
	}
	
	return;
}

void CMagicMgr::CreateMaxMsg()
{
	CMagicMsgObj *pObj = NULL;
	SI32 siMaxMesage = 0;

	if(pclClient->GameMode == GAMEMODE_SERVER)	siMaxMesage = MAX_MAGIC_MSG_SERVER_NUM;
	else										siMaxMesage = MAX_MAGIC_MSG_NUM;

	for ( SI32 i = 0; i < siMaxMesage; ++i )
	{
		pObj = new CMagicMsgObj();
		m_pMsgEmptyList->PushBack( pObj );
	}

	return;
}

void CMagicMgr::CreateMaxEmptyObj()
{
	//////////////////////////////////////////////////////////////////////////
	// CREATE MAGIC OBEJCT MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_CREATE_OBJECT(ClassType, Max, Empty)	\
	{\
		ClassType *pMagicObj = NULL;\
		for ( SI32 i = 0; i < Max; ++ i )\
		{\
			pMagicObj = new ClassType( this );\
			Empty->PushBack( pMagicObj );\
		}\
	}
	//////////////////////////////////////////////////////////////////////////
	// SWORD
	//////////////////////////////////////////////////////////////////////////
	//
	
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		// [1]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicFireBallEmptyList)
		MAGIC_CREATE_OBJECT(CMagicBigWindObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBigWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThunderObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicThunderEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThunder1Obj,			MAX_MAGIC_SERVER_NUM,	m_pMagicThunder1EmptyList)
		MAGIC_CREATE_OBJECT(CMagicWhirlPoolObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicWhirlPoolEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFlameObj,				MAX_MAGIC_SERVER_NUM,	m_pMagicFlameEmptyList)
		MAGIC_CREATE_OBJECT(CMagicRockSlideObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicRockSlideEmptyList)
		MAGIC_CREATE_OBJECT(CMagicHealObj,				MAX_MAGIC_SERVER_NUM,	m_pMagicHealEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPoisonObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicPoisonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicIceObj,				MAX_MAGIC_SERVER_NUM,	m_pMagicIceEmptyList)
		// [11]
		MAGIC_CREATE_OBJECT(CMagicLightingFistObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicLightingFistEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPoisonFlameObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicPoisonFlameEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropMoneyObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicDropMoneyEmptyList)
		MAGIC_CREATE_OBJECT(CMagicBlackInkObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBlackInkEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropSnowmanObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicDropSnowmanEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropForkObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicDropForkEmptyList)
		MAGIC_CREATE_OBJECT(CMagicCurseDefenseObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicCurseDefenseEmptyList)
		MAGIC_CREATE_OBJECT(CMagicCurseMovespeedObj,	MAX_MAGIC_SERVER_NUM,	m_pMagicCurseMovespeedEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThrowKnifeObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicThrowKnifeEmptyList)
		// [21]
		MAGIC_CREATE_OBJECT(CMagicLaserBeamObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicLaserBeamEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicTriMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireGroundObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicFireGroundEmptyList)
		MAGIC_CREATE_OBJECT(CMagicSwordBombObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSwordBombEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFreezeExplosionObj,	MAX_MAGIC_SERVER_NUM,	m_pMagicFreezeExplosionEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicUpperDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPalmWindObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicPalmWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicSummonDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBurnObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicFireBurnEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkellGhostEmptyList)
		// [31]
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicSwordMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicIceObj,				MAX_MAGIC_SERVER_NUM,	m_pMagicFrozenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicTwinSpearEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireWhirlWindObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicFireWhirlWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThrowKnifeObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicNorthWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicFireStormEmptyList)
		MAGIC_CREATE_OBJECT(CMagicEarthQuakeObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicEarthQuakeEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSpearDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicGargoyleEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicCerberusEmptyList)
		// [41]
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicFrozenSorcererEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicMarionetteEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicFrokenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicIcehoundEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicFrozenGolemEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicFrozenKnightEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicFrozenQueenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicGotpaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicNinjaPandaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicRaccoonDogEmptyList);
		// [51]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBossMonkeyEmptyList);
		MAGIC_CREATE_OBJECT(CMagicPoisonFlameObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicToadPoisonEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicNinjaPandaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBloodBatEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicWerewolfEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicDevilousEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicGangsiEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicDraculaEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicPriestEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicMandragoraEmptyList);
		// [61]
		MAGIC_CREATE_OBJECT(CMagicBigWindObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicHarpyEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicGiraffeEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicPapaGoblinEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicPirateSoldierEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicPirateArcherEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBlackBossEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicEmperorCavalryEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicEmperorMasterEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicGreatWallSoldierEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicGreatWallMagicianEmptyList);
		// [71]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicEmperorWarriorEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicGreatWallSpiderEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicGreatWallTwinHeadEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBearTangEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSuperBearTangEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicSFrozenGolemEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicSFrozenQueenEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicCatPetEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_SERVER_NUM,	m_pMagicAuraBladeEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicTripleDeathEmptyList);
		// [81]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicCosmosRabbitEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicRudolphpetEmptyList);
		MAGIC_CREATE_OBJECT(CMagicDropGreatWallAsuraObj,MAX_MAGIC_SERVER_NUM,	m_pMagicDropGreatWallAsuraEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicAmbersaurusEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicRollingstegoEmptyList);

		MAGIC_CREATE_OBJECT(MagicDragonSummonPetObj,	MAX_MAGIC_SERVER_NUM,	m_pMagicDragonFireEmptyList);
		MAGIC_CREATE_OBJECT(MagicDragonSummonPetObj,	MAX_MAGIC_SERVER_NUM,	m_pMagicDragonStunEmptyList);

		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookCrashEmptyList			);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookFlameBusterEmptyList	);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookFlameWhirlEmptyList	);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookIceRainEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookSlingEmptyList			);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookTornadoEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookWetFogEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicSkillBookWindcuterEmptyList		);

		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicClownBallEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_SERVER_NUM,	m_pMagicBouquetEmptyList		);
	}
	else 
	{
		// [1]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicFireBallEmptyList)
		MAGIC_CREATE_OBJECT(CMagicBigWindObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBigWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThunderObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicThunderEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThunder1Obj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicThunder1EmptyList)
		MAGIC_CREATE_OBJECT(CMagicWhirlPoolObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicWhirlPoolEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFlameObj,				MAX_MAGIC_CLIENT_NUM,	m_pMagicFlameEmptyList)
		MAGIC_CREATE_OBJECT(CMagicRockSlideObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicRockSlideEmptyList)
		MAGIC_CREATE_OBJECT(CMagicHealObj,				MAX_MAGIC_CLIENT_NUM,	m_pMagicHealEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPoisonObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicPoisonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicIceObj,				MAX_MAGIC_CLIENT_NUM,	m_pMagicIceEmptyList)
		// [11]
		MAGIC_CREATE_OBJECT(CMagicLightingFistObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicLightingFistEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPoisonFlameObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicPoisonFlameEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropMoneyObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicDropMoneyEmptyList)
		MAGIC_CREATE_OBJECT(CMagicBlackInkObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBlackInkEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropSnowmanObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicDropSnowmanEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDropForkObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicDropForkEmptyList)
		MAGIC_CREATE_OBJECT(CMagicCurseDefenseObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicCurseDefenseEmptyList)
		MAGIC_CREATE_OBJECT(CMagicCurseMovespeedObj,	MAX_MAGIC_CLIENT_NUM,	m_pMagicCurseMovespeedEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThrowKnifeObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicThrowKnifeEmptyList)
		// [21]
		MAGIC_CREATE_OBJECT(CMagicLaserBeamObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicLaserBeamEmptyList)
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicTriMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireGroundObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicFireGroundEmptyList)
		MAGIC_CREATE_OBJECT(CMagicSwordBombObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSwordBombEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFreezeExplosionObj,	MAX_MAGIC_CLIENT_NUM,	m_pMagicFreezeExplosionEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicUpperDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicPalmWindObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicPalmWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicSummonDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBurnObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicFireBurnEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkellGhostEmptyList)
		// [31]
		MAGIC_CREATE_OBJECT(CMagicDragonCannonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicSwordMeteorEmptyList)
		MAGIC_CREATE_OBJECT(CMagicIceObj,				MAX_MAGIC_CLIENT_NUM,	m_pMagicFrozenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicTwinSpearEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireWhirlWindObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicFireWhirlWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicThrowKnifeObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicNorthWindEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicFireStormEmptyList)
		MAGIC_CREATE_OBJECT(CMagicEarthQuakeObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicEarthQuakeEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSpearDragonEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicGargoyleEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicCerberusEmptyList)
		// [41]
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicFrozenSorcererEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicMarionetteEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicFrokenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicIcehoundEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicFrozenGolemEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicFrozenKnightEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicFrozenQueenEmptyList)
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicGotpaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicNinjaPandaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicRaccoonDogEmptyList);
		// [51]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBossMonkeyEmptyList);
		MAGIC_CREATE_OBJECT(CMagicPoisonFlameObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicToadPoisonEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicNinjaPandaEmptyList)
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBloodBatEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicWerewolfEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicDevilousEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicGangsiEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicDraculaEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicPriestEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicMandragoraEmptyList);
		// [61]
		MAGIC_CREATE_OBJECT(CMagicBigWindObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicHarpyEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicGiraffeEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicPapaGoblinEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicPirateSoldierEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicPirateArcherEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBlackBossEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicEmperorCavalryEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicEmperorMasterEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicGreatWallSoldierEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicGreatWallMagicianEmptyList);
		// [71]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicEmperorWarriorEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicGreatWallSpiderEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicGreatWallTwinHeadEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBearTangEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSuperBearTangEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicSFrozenGolemEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicSFrozenQueenEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicCatPetEmptyList);
		MAGIC_CREATE_OBJECT(CMagicUpperDragonObj,		MAX_MAGIC_CLIENT_NUM,	m_pMagicAuraBladeEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicTripleDeathEmptyList);
		// [81]
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicCosmosRabbitEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicRudolphpetEmptyList);
		MAGIC_CREATE_OBJECT(CMagicDropGreatWallAsuraObj,	MAX_MAGIC_CLIENT_NUM,	m_pMagicDropGreatWallAsuraEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicAmbersaurusEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicRollingstegoEmptyList);

		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicRollingstegoEmptyList);

		MAGIC_CREATE_OBJECT(MagicDragonSummonPetObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicDragonFireEmptyList);
		MAGIC_CREATE_OBJECT(MagicDragonSummonPetObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicDragonStunEmptyList);

		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookCrashEmptyList			);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookFlameBusterEmptyList	);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookFlameWhirlEmptyList	);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookIceRainEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookSlingEmptyList			);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookTornadoEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookWetFogEmptyList		);
		MAGIC_CREATE_OBJECT(CMagicSkillBookObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicSkillBookWindcuterEmptyList		);

		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicClownBallEmptyList);
		MAGIC_CREATE_OBJECT(CMagicFireBallObj,			MAX_MAGIC_CLIENT_NUM,	m_pMagicBouquetEmptyList);
	}

	return;
}



//////////////////////////////////////////////////////////////////////////

CMagicAttribute	*CMagicMgr::GetMagicAttributePtr()
{
	return m_pMagicAttribute;
}

stMagicAttributeData *CMagicMgr::GetMagicAttribute( SI32 si32MagicKind )
{
	return m_pMagicAttribute->GetMagicAttribute( si32MagicKind );
}

SI32 CMagicMgr::GetSpendMana( SI32 si32MagicKind, SI32 siSkillLevel , SI32 id )
{
	stMagicAttributeData *pMagicAttribute = m_pMagicAttribute->GetMagicAttribute( si32MagicKind );
	SI32 siSkillLevelDiff = siSkillLevel - pMagicAttribute->si16MagicSkillLevel;

	if ( siSkillLevelDiff < 0 )	siSkillLevelDiff = 0;

	SI32 siSpendMana = 0;
	if(pMagicAttribute)
	{
		siSpendMana = pMagicAttribute->si32MagicSpendMana 
					+ (pMagicAttribute->si32MagicSpendMana * pMagicAttribute->si16MagicSkillSpendMana * siSkillLevelDiff / 10000);
		
	}

	cltCharCommon* pclchar = NULL;
	if ( (GAMEMODE_SERVER == pclClient->GameMode) && (IsPC(id)) )
	{
		pclchar = pclClient->pclCM->GetChar( id );
	}
	else
	{
		pclchar = pclClient->pclCM->GetChar( 1 );
	}

	if ( (pclchar) && ((PERSONITEM_WEAPON1 == pclchar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pclchar->ActiveWeapon)) )
	{
		SI32 siSkillUnique		= 0;
		SI32 siItemType			= pclchar->pclCI->clCharItem.GetWeaponType( pclClient->pclItemManager, pclchar->ActiveWeapon );
		switch ( siItemType )
		{
			case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_STAFF_DECUSEMANA;	}	break;
			case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_STAFF_DECUSEMANA;	}	break;
			case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_STAFF_DECUSEMANA;		}	break;
			case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_STAFF_DECUSEMANA;		}	break;
			case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_STAFF_DECUSEMANA;		}	break;
			case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_DECUSEMANA;			}	break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclchar->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				siSpendMana -= (siSpendMana * siWeaponSkillRate / 100);
			}
		}
	}

	if( pclClient->IsCountrySwitch(Switch_SkillBook) )
	{
		if( pclchar )
		{
			if( pclchar->pclCharSkillBookInfo )
			{
				SI32 siSubMagickind = 0;
				stMagicAttributeData* psSubMagicData = pclchar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( si32MagicKind, siSubMagickind );
				if( psSubMagicData )
				{
					siSpendMana += (siSpendMana * psSubMagicData->si32MagicSpendMana) / 100;
				}
			}
		}
	}

	return siSpendMana;
}


bool CMagicMgr::SetMagic( stMagicHeader *pstMagicHeader, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	
	int a = MAGIC_BOW_HITPOINT;


	bool success = false;
	//////////////////////////////////////////////////////////////////////////
	// SET MAGIC MACRO
	//////////////////////////////////////////////////////////////////////////
	
	#define MAGIC_SET_MAGIC( ClassType, Exist, Empty )	\
	{	\
	ClassType * pObj = ( ClassType * )Empty->PopFront(); \
	if ( pObj == NULL )	{return false;} pObj->Initialize();	 \
	success = pObj->SetMagic(( stMagicInfo *) pstMagicHeader, si32AddDamage, pclMap, si32SkillLevel, siOrganUnique);\
	if(success == true) { Exist->PushBack( pObj );} \
	else				{ Empty->PushBack( pObj );} \
	}
	//////////////////////////////////////////////////////////////////////////
	
	switch( m_pMagicAttribute->GetMagicGroup(pstMagicHeader->si32MagicKind) )
	{
	case MAGIC_GROUP_FIREBALL:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicFireBallExistList,				m_pMagicFireBallEmptyList);
		break;
	case MAGIC_GROUP_BIGWIND:
		MAGIC_SET_MAGIC(CMagicBigWindObj,		m_pMagicBigWindExistList,				m_pMagicBigWindEmptyList);
		break;
	case MAGIC_GROUP_THUNDER:
		MAGIC_SET_MAGIC(CMagicThunderObj,		m_pMagicThunderExistList,				m_pMagicThunderEmptyList);
		break;
	case MAGIC_GROUP_THUNDER1:
		MAGIC_SET_MAGIC(CMagicThunder1Obj,		m_pMagicThunder1ExistList,				m_pMagicThunder1EmptyList);
		break;
	case MAGIC_GROUP_WHIRLPOOL:
		MAGIC_SET_MAGIC(CMagicWhirlPoolObj,		m_pMagicWhirlPoolExistList,				m_pMagicWhirlPoolEmptyList);
		break;
	case MAGIC_GROUP_FLAME:
		MAGIC_SET_MAGIC(CMagicFlameObj,			m_pMagicFlameExistList,					m_pMagicFlameEmptyList );
		break;
	case MAGIC_GROUP_ROCKSLIDE:
		MAGIC_SET_MAGIC(CMagicRockSlideObj,		m_pMagicRockSlideExistList,				m_pMagicRockSlideEmptyList );
		break;
	case MAGIC_GROUP_HEAL:
		MAGIC_SET_MAGIC(CMagicHealObj,			m_pMagicHealExistList,					m_pMagicHealEmptyList );
		break;
	case MAGIC_GROUP_POISON:
		MAGIC_SET_MAGIC(CMagicPoisonObj,		m_pMagicPoisonExistList,				m_pMagicPoisonEmptyList);
		break;
	case MAGIC_GROUP_ICE:
		MAGIC_SET_MAGIC(CMagicIceObj,			m_pMagicIceExistList,					m_pMagicIceEmptyList);
		break;
	case MAGIC_GROUP_DRAIN:				// PASSIVE
		break;
	case MAGIC_GROUP_LIGHTINGFIST:
		MAGIC_SET_MAGIC(CMagicLightingFistObj,	m_pMagicLightingFistExistList,			m_pMagicLightingFistEmptyList);
		break;
	case MAGIC_GROUP_POISONFLAME:
		MAGIC_SET_MAGIC(CMagicPoisonFlameObj,	m_pMagicPoisonFlameExistList,			m_pMagicPoisonFlameEmptyList);
		break;
	case MAGIC_GROUP_METEOR:
		MAGIC_SET_MAGIC(CMagicDragonCannonObj,	m_pMagicMeteorExistList,				m_pMagicMeteorEmptyList);
		break;
	case MAGIC_GROUP_DROPMONEY:
		MAGIC_SET_MAGIC(CMagicDropMoneyObj,		m_pMagicDropMoneyExistList,				m_pMagicDropMoneyEmptyList);
		break;
	case MAGIC_GROUP_BLACKINK:
		MAGIC_SET_MAGIC(CMagicBlackInkObj,		m_pMagicBlackInkExistList,				m_pMagicBlackInkEmptyList);
		break;
	case MAGIC_GROUP_DROPSNOWMAN:
		MAGIC_SET_MAGIC(CMagicDropSnowmanObj,	m_pMagicDropSnowmanExistList,			m_pMagicDropSnowmanEmptyList);
		break;
	case MAGIC_GROUP_DROPFORK:
		MAGIC_SET_MAGIC(CMagicDropForkObj,		m_pMagicDropForkExistList,				m_pMagicDropForkEmptyList);
		break;
	case MAGIC_GROUP_CURSEATTACK:		// PASSIVE
		break;
	case MAGIC_GROUP_CURSECRITICAL:		// PASSIVE
		break;
	case MAGIC_GROUP_CURSEDEFENSE:
		MAGIC_SET_MAGIC(CMagicCurseDefenseObj,	m_pMagicCurseDefenseExistList,			m_pMagicCurseDefenseEmptyList);
		break;
	case MAGIC_GROUP_CURSEMOVESPEED:
		MAGIC_SET_MAGIC(CMagicCurseMovespeedObj,m_pMagicCurseMovespeedExistList,		m_pMagicCurseMovespeedEmptyList);
		break;
	case MAGIC_GROUP_THROWKNIFE:
		MAGIC_SET_MAGIC(CMagicThrowKnifeObj,	m_pMagicThrowKnifeExistList,			m_pMagicThrowKnifeEmptyList);
		break;
	case MAGIC_GROUP_LASERBEAM:
		MAGIC_SET_MAGIC(CMagicLaserBeamObj,		m_pMagicLaserBeamExistList,				m_pMagicLaserBeamEmptyList);
		break;
	case MAGIC_GROUP_TRIMETEOR:
		MAGIC_SET_MAGIC(CMagicDragonCannonObj,	m_pMagicTriMeteorExistList,				m_pMagicTriMeteorEmptyList);
		break;
	case MAGIC_GROUP_FIREGOUND:
		MAGIC_SET_MAGIC(CMagicFireGroundObj,	m_pMagicFireGroundExistList,			m_pMagicFireGroundEmptyList);
		break;
	case MAGIC_GROUP_SWORDBOMB:
		MAGIC_SET_MAGIC(CMagicSwordBombObj,		m_pMagicSwordBombExistList,				m_pMagicSwordBombEmptyList);
		break;
	case MAGIC_GROUP_FREEZEEXPLOSION:
		MAGIC_SET_MAGIC(CMagicFreezeExplosionObj,m_pMagicFreezeExplosionExistList,		m_pMagicFreezeExplosionEmptyList);
		break;
	case MAGIC_GROUP_UPPERDRGON:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicUpperDragonExistList,			m_pMagicUpperDragonEmptyList);
		break;
	case MAGIC_GROUP_PALMWIND:
		MAGIC_SET_MAGIC(CMagicPalmWindObj,		m_pMagicPalmWindExistList,				m_pMagicPalmWindEmptyList);
		break;
	case MAGIC_GROUP_SUMMONDRAGON:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicSummonDragonExistList,			m_pMagicSummonDragonEmptyList);
		break;
	case MAGIC_GROUP_FIREBURN:
		MAGIC_SET_MAGIC(CMagicFireBurnObj,		m_pMagicFireBurnExistList,				m_pMagicFireBurnEmptyList);
		break;
	case MAGIC_GROUP_SKELLGHOST:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicSkellGhostExistList,			m_pMagicSkellGhostEmptyList);
		break;
	case MAGIC_GROUP_SWORDMETEOR:
		MAGIC_SET_MAGIC(CMagicDragonCannonObj,	m_pMagicSwordMeteorExistList,			m_pMagicSwordMeteorEmptyList);
		break;
	case MAGIC_GROUP_FROZEN:
		MAGIC_SET_MAGIC(CMagicIceObj,			m_pMagicFrozenExistList,				m_pMagicFrozenEmptyList);
		break;
	case MAGIC_GROUP_TWINSPEAR:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicTwinSpearExistList,				m_pMagicTwinSpearEmptyList);
		break;
	case MAGIC_GROUP_FIREWHIRLWIND:
		MAGIC_SET_MAGIC(CMagicFireWhirlWindObj,	m_pMagicFireWhirlWindExistList,			m_pMagicFireWhirlWindEmptyList);
		break;
	case MAGIC_GROUP_NORTHWIND:
		MAGIC_SET_MAGIC(CMagicThrowKnifeObj,	m_pMagicNorthWindExistList,				m_pMagicNorthWindEmptyList);
		break;
	case MAGIC_GROUP_FIRESTORM:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicFireStormExistList,				m_pMagicFireStormEmptyList);
		break;
	case MAGIC_GROUP_EARTHQUAKE:
		MAGIC_SET_MAGIC(CMagicEarthQuakeObj,	m_pMagicEarthQuakeExistList,			m_pMagicEarthQuakeEmptyList);
		break;
	case MAGIC_GROUP_SPEAR_DRAGON:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicSpearDragonExistList,			m_pMagicSpearDragonEmptyList);
		break;
	case MAGIC_GROUP_GARGOYLE:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicGargoyleExistList,				m_pMagicGargoyleEmptyList);
		break;
	case MAGIC_GROUP_CERBERUS:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicCerberusExistList,				m_pMagicCerberusEmptyList);
		break;
	case MAGIC_GROUP_FROZENSORCERER:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicFrozenSorcererExistList,		m_pMagicFrozenSorcererEmptyList);
		break;
	case MAGIC_GROUP_MARIONETTE:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicMarionetteExistList,			m_pMagicMarionetteEmptyList);
		break;
	case MAGIC_GROUP_FROKEN:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicFrokenExistList,				m_pMagicFrokenEmptyList);
		break;
	case MAGIC_GROUP_ICEHOUND:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicIcehoundExistList,				m_pMagicIcehoundEmptyList);
		break;	
	case MAGIC_GROUP_FROZENGOLEM:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicFrozenGolemExistList,			m_pMagicFrozenGolemEmptyList);
		break;
	case MAGIC_GROUP_FROZENKNIGHT:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicFrozenKnightExistList,			m_pMagicFrozenKnightEmptyList);
		break;
	case MAGIC_GROUP_FROZENQUEEN:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicFrozenQueenExistList,			m_pMagicFrozenQueenEmptyList);
		break;
	case MAGIC_GROUP_GOTPA:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicGotpaExsitList,					m_pMagicGotpaEmptyList);
		break;
	case MAGIC_GROUP_NINJAPANDA:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicNinjaPandaExistList,			m_pMagicNinjaPandaEmptyList);
		break;
	case MAGIC_GROUP_RACCOONDOG:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicRaccoonDogExistList,			m_pMagicRaccoonDogEmptyList);
		break;
	case MAGIC_GROUP_BOSSMONKEY:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicBossMonkeyExistList,			m_pMagicBossMonkeyEmptyList);
		break;
	case MAGIC_GROUP_TOADPOISON:
		MAGIC_SET_MAGIC(CMagicPoisonFlameObj,	m_pMagicToadPoisonExistList,			m_pMagicToadPoisonEmptyList);
		break;
	case MAGIC_GROUP_BLOODBAT:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicBloodBatExistList,				m_pMagicBloodBatEmptyList);
		break;
	case MAGIC_GROUP_WEREWOLF:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicWerewolfExistList,				m_pMagicWerewolfEmptyList);
		break;
	case MAGIC_GROUP_DEVILOUS:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicDevilousExistList,				m_pMagicDevilousEmptyList);
		break;
	case MAGIC_GROUP_GANGSI:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicGangsiExistList,				m_pMagicGangsiEmptyList);
		break;
	case MAGIC_GROUP_DRACULA:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicDraculaExistList,				m_pMagicDraculaEmptyList);
		break;
	case MAGIC_GROUP_PRIEST:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicPriestExistList,				m_pMagicPriestEmptyList);
		break;
	case MAGIC_GROUP_HARPY:
		MAGIC_SET_MAGIC(CMagicBigWindObj,		m_pMagicHarpyExistList,					m_pMagicHarpyEmptyList);
		break;
	case MAGIC_GROUP_MANDRAGORA:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicMandragoraExistList,			m_pMagicMandragoraEmptyList);
		break;
	case MAGIC_GROUP_GIRAFFE:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicGiraffeExistList,				m_pMagicGiraffeEmptyList);
		break;
	case MAGIC_GROUP_PAPAGOBLIN:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicPapaGoblinExistList,			m_pMagicPapaGoblinEmptyList);
		break;
	case MAGIC_GROUP_PIRATESOLDIER:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicPirateSoldierExistList,			m_pMagicPirateSoldierEmptyList);
		break;
	case MAGIC_GROUP_PIRATEARCHER:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicPirateArcherExistList,			m_pMagicPirateArcherEmptyList);
		break;
	case MAGIC_GROUP_BLACKBOSS:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicBlackBossExistList,				m_pMagicBlackBossEmptyList);
		break;
	case MAGIC_GROUP_EMPERORCAVALRY:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicEmperorCavalryExistList,		m_pMagicEmperorCavalryEmptyList);
		break;
	case MAGIC_GROUP_EMPERORMASTER:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicEmperorMasterExistList,			m_pMagicEmperorMasterEmptyList);
		break;
	case MAGIC_GROUP_GREATWALLSOLDIER:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicGreatWallSoldierExistList,		m_pMagicGreatWallSoldierEmptyList);
		break;
	case MAGIC_GROUP_GREATWALLMAGICIAN:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicGreatWallMagicianExistList,		m_pMagicGreatWallMagicianEmptyList);
		break;
	case MAGIC_GROUP_EMPERORWARRIOR:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicEmperorWarriorExistList,		m_pMagicEmperorWarriorEmptyList);
		break;
	case MAGIC_GROUP_GREATWALLSPIDER:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicGreatWallSpiderExistList,		m_pMagicGreatWallSpiderEmptyList);
		break;
	case MAGIC_GROUP_GREATWALLTWINHEAD:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicGreatWallTwinHeadExistList,		m_pMagicGreatWallTwinHeadEmptyList);
		break;
	case MAGIC_GROUP_BEARTANG:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicBearTangExistList,				m_pMagicBearTangEmptyList);
		break;
	case MAGIC_GROUP_SUPERBEARTANG:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicSuperBearTangExistList,			m_pMagicSuperBearTangEmptyList);
		break;
	case MAGIC_GROUP_SFROZENGOLEM:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicSFrozenGolemExistList,			m_pMagicSFrozenGolemEmptyList);
		break;
	case MAGIC_GROUP_SFROZENQUEEN:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicSFrozenQueenExistList,			m_pMagicSFrozenQueenEmptyList);
		break;
	case MAGIC_GROUP_CATPET:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicCatPetExistList,				m_pMagicCatPetEmptyList);
		break;
	case MAGIC_GROUP_AURABLADE:
		MAGIC_SET_MAGIC(CMagicUpperDragonObj,	m_pMagicAuraBladeExistList,				m_pMagicAuraBladeEmptyList);
		break;
	case MAGIC_GROUP_TRIPLEDEATH:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicTripleDeathExistList,			m_pMagicTripleDeathEmptyList);
		break;
	case MAGIC_GROUP_BLESSDAMAGE:			// BUF
		break;
	case MAGIC_GROUP_BLESSCIRICAL:			// BUF
		break;
	case MAGIC_GROUP_BLESSHITRATE:			// BUF
		break;
	case MAGIC_GROUP_BLESSRANGE:			// BUF
		break;
	case MAGIC_GROUP_BLESSATTACKINTERVAL:	// BUF
		break;
	case MAGIC_GROUP_BLESSMPRECOVERYTIME:	// BUF
		break;
	case MAGIC_GROUP_BLESSHP:				// BUF
		break;
	case MAGIC_GROUP_COSMOSRABBIT:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicCosmosRabbitExistList,			m_pMagicCosmosRabbitEmptyList);
		break;
	case MAGIC_GROUP_RUDOLPHPET:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicRudolphpetExistList,			m_pMagicRudolphpetEmptyList);
		break;	
	case MAGIC_GROUP_DROPGREATWALLASURA:
		MAGIC_SET_MAGIC(CMagicDropGreatWallAsuraObj,		m_pMagicDropGreatWallAsuraExistList,				m_pMagicDropGreatWallAsuraEmptyList);
		break;
	case MAGIC_GROUP_AMBERSAURUS:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicAmbersaurusExistList,		m_pMagicAmbersaurusEmptyList);
		break;
	case MAGIC_GROUP_ROLLINGSTEGO:
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicRollingstegoExiteList,		m_pMagicRollingstegoEmptyList);

	case MAGIC_GROUP_PUMPKIN:	// 펌킨
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicPumkinExiteList,		m_pMagicPumkinEmptyList);
		break;

	case MAGIC_GROUP_SUMMONDRAGON_STUN:
		MAGIC_SET_MAGIC(MagicDragonSummonPetObj,		m_pMagicDragonStunExistList,	m_pMagicDragonStunEmptyList );
		break;
	
	case MAGIC_GROUP_DRAGONFIRE:	
		MAGIC_SET_MAGIC(MagicDragonSummonPetObj,		m_pMagicDragonFireExistList,	m_pMagicDragonFireEmptyList);
		break;

	//[진성] 스킬북.
	case MAGIC_GROUP_SKILL_BOOK_FLAMEBUSTER:	
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookCrashExistList,		m_pMagicSkillBookCrashEmptyList );
		break;	
	case MAGIC_GROUP_SKILL_BOOK_FLAMEWHIRL:	
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookFlameBusterExistList,	m_pMagicSkillBookFlameBusterEmptyList);
		break;	
	case MAGIC_GROUP_SKILL_BOOK_ICERAIN:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookFlameWhirlExistList,	m_pMagicSkillBookFlameWhirlEmptyList);
		break;
	case MAGIC_GROUP_SKILL_BOOK_WETFOG:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookIceRainExistList,		m_pMagicSkillBookIceRainEmptyList);
		break;
	case MAGIC_GROUP_SKILL_BOOK_CRASH:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookSlingExistList,		m_pMagicSkillBookSlingEmptyList);
		break;
	case MAGIC_GROUP_SKILL_BOOK_SLING:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookTornadoExistList,		m_pMagicSkillBookTornadoEmptyList);
		break;
	case MAGIC_GROUP_SKILL_BOOK_TORNADO:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookWetFogExistList,		m_pMagicSkillBookWetFogEmptyList);
		break;
	case MAGIC_GROUP_SKILL_BOOK_WINDCUTER:		
		MAGIC_SET_MAGIC(CMagicSkillBookObj,		m_pMagicSkillBookWindcuterExistList,	m_pMagicSkillBookWindcuterEmptyList);
		break;

	case MAGIC_GROUP_CLOWNBALL:	
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicClownBallExistList,				m_pMagicClownBallEmptyList);
		break;
	case MAGIC_GROUP_BOUQUET:	
		MAGIC_SET_MAGIC(CMagicFireBallObj,		m_pMagicBouquetExistList,				m_pMagicBouquetEmptyList);
		break;
	}

	// 어떤 마법에 몇개나 사용하는지 검사한다.
	if(IsPC(pstMagicHeader->si32FromID))
	{
		if(success)	IncreaseMagicUse(pstMagicHeader->si32MagicKind);
	}
	
	
	return success;
}

void CMagicMgr::Action()
{
	CMagicBaseObj	*pObj, *pTemp = NULL;
	//////////////////////////////////////////////////////////////////////////
	// ACTIO MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_LIST_ACTION(Exist, Empty)	\
	pObj = ( CMagicBaseObj * )Exist->GetFirst();\
	while ( pObj )\
	{\
		if ( pObj->Action() )\
		{\
			pTemp = pObj;\
			pObj = ( CMagicBaseObj * )pObj->GetNext();\
			Exist->Pop( pTemp );\
			Empty->PushBack( pTemp );\
		}	\
		else\
		{\
			pObj = ( CMagicBaseObj * )pObj->GetNext();\
		}\
	};
	//////////////////////////////////////////////////////////////////////////

	// STAFF
	MAGIC_LIST_ACTION(m_pMagicFireBallExistList,			m_pMagicFireBallEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBigWindExistList,				m_pMagicBigWindEmptyList);
	MAGIC_LIST_ACTION(m_pMagicThunderExistList,				m_pMagicThunderEmptyList);
	MAGIC_LIST_ACTION(m_pMagicThunder1ExistList,			m_pMagicThunder1EmptyList);
	MAGIC_LIST_ACTION(m_pMagicWhirlPoolExistList,			m_pMagicWhirlPoolEmptyList)
	MAGIC_LIST_ACTION(m_pMagicFlameExistList,				m_pMagicFlameEmptyList);
	MAGIC_LIST_ACTION(m_pMagicRockSlideExistList,			m_pMagicRockSlideEmptyList)
	MAGIC_LIST_ACTION(m_pMagicHealExistList,				m_pMagicHealEmptyList)
	MAGIC_LIST_ACTION(m_pMagicPoisonExistList,				m_pMagicPoisonEmptyList)	
	MAGIC_LIST_ACTION(m_pMagicIceExistList,					m_pMagicIceEmptyList)
	MAGIC_LIST_ACTION(m_pMagicLightingFistExistList,		m_pMagicLightingFistEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPoisonFlameExistList,			m_pMagicPoisonFlameEmptyList);
	MAGIC_LIST_ACTION(m_pMagicMeteorExistList,				m_pMagicMeteorEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDropMoneyExistList,			m_pMagicDropMoneyEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBlackInkExistList,			m_pMagicBlackInkEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDropSnowmanExistList,			m_pMagicDropSnowmanEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDropForkExistList,			m_pMagicDropForkEmptyList);
	MAGIC_LIST_ACTION(m_pMagicCurseDefenseExistList,		m_pMagicCurseDefenseEmptyList);
	MAGIC_LIST_ACTION(m_pMagicCurseMovespeedExistList,		m_pMagicCurseMovespeedEmptyList);
	MAGIC_LIST_ACTION(m_pMagicThrowKnifeExistList,			m_pMagicThrowKnifeEmptyList);
	MAGIC_LIST_ACTION(m_pMagicLaserBeamExistList,			m_pMagicLaserBeamEmptyList);
	MAGIC_LIST_ACTION(m_pMagicTriMeteorExistList,			m_pMagicTriMeteorEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFireGroundExistList,			m_pMagicFireGroundEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSwordBombExistList,			m_pMagicSwordBombEmptyList);

	MAGIC_LIST_ACTION(m_pMagicFreezeExplosionExistList,		m_pMagicFreezeExplosionEmptyList);

	MAGIC_LIST_ACTION(m_pMagicUpperDragonExistList,			m_pMagicUpperDragonEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPalmWindExistList,			m_pMagicPalmWindEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSummonDragonExistList,		m_pMagicSummonDragonEmptyList);

	MAGIC_LIST_ACTION(m_pMagicFireBurnExistList,			m_pMagicFireBurnEmptyList);

	MAGIC_LIST_ACTION(m_pMagicSkellGhostExistList,			m_pMagicSkellGhostEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSwordMeteorExistList,			m_pMagicSwordMeteorEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFrozenExistList,				m_pMagicFrozenEmptyList);
	MAGIC_LIST_ACTION(m_pMagicTwinSpearExistList,			m_pMagicTwinSpearEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFireWhirlWindExistList,		m_pMagicFireWhirlWindEmptyList);
	MAGIC_LIST_ACTION(m_pMagicNorthWindExistList,			m_pMagicNorthWindEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFireStormExistList,			m_pMagicFireStormEmptyList);
	MAGIC_LIST_ACTION(m_pMagicEarthQuakeExistList,			m_pMagicEarthQuakeEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSpearDragonExistList,			m_pMagicSpearDragonEmptyList);

	MAGIC_LIST_ACTION(m_pMagicGargoyleExistList,			m_pMagicGargoyleEmptyList);
	MAGIC_LIST_ACTION(m_pMagicCerberusExistList,			m_pMagicCerberusEmptyList);

	MAGIC_LIST_ACTION(m_pMagicFrozenSorcererExistList,		m_pMagicFrozenSorcererEmptyList);
	MAGIC_LIST_ACTION(m_pMagicMarionetteExistList,			m_pMagicMarionetteEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFrokenExistList,				m_pMagicFrokenEmptyList);
	MAGIC_LIST_ACTION(m_pMagicIcehoundExistList,			m_pMagicIcehoundEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFrozenGolemExistList,			m_pMagicFrozenGolemEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFrozenKnightExistList,		m_pMagicFrozenKnightEmptyList);
	MAGIC_LIST_ACTION(m_pMagicFrozenQueenExistList,			m_pMagicFrozenQueenEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGotpaExsitList,				m_pMagicGotpaEmptyList);
	MAGIC_LIST_ACTION(m_pMagicNinjaPandaExistList,			m_pMagicNinjaPandaEmptyList);
	MAGIC_LIST_ACTION(m_pMagicRaccoonDogExistList,			m_pMagicRaccoonDogEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBossMonkeyExistList,			m_pMagicBossMonkeyEmptyList);
	MAGIC_LIST_ACTION(m_pMagicToadPoisonExistList,			m_pMagicToadPoisonEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBloodBatExistList,			m_pMagicBloodBatEmptyList);
	MAGIC_LIST_ACTION(m_pMagicWerewolfExistList,			m_pMagicWerewolfEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDevilousExistList,			m_pMagicDevilousEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGangsiExistList,				m_pMagicGangsiEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDraculaExistList,				m_pMagicDraculaEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPriestExistList,				m_pMagicPriestEmptyList);
	MAGIC_LIST_ACTION(m_pMagicHarpyExistList,				m_pMagicHarpyEmptyList);
	MAGIC_LIST_ACTION(m_pMagicMandragoraExistList,			m_pMagicMandragoraEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGiraffeExistList,				m_pMagicGiraffeEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPapaGoblinExistList,			m_pMagicPapaGoblinEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPirateSoldierExistList,		m_pMagicPirateSoldierEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPirateArcherExistList,		m_pMagicPirateArcherEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBlackBossExistList,			m_pMagicBlackBossEmptyList);
	MAGIC_LIST_ACTION(m_pMagicEmperorCavalryExistList,		m_pMagicEmperorCavalryEmptyList);
	MAGIC_LIST_ACTION(m_pMagicEmperorMasterExistList,		m_pMagicEmperorMasterEmptyList);
    MAGIC_LIST_ACTION(m_pMagicGreatWallSoldierExistList,	m_pMagicGreatWallSoldierEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGreatWallMagicianExistList,	m_pMagicGreatWallMagicianEmptyList);
	MAGIC_LIST_ACTION(m_pMagicEmperorWarriorExistList,		m_pMagicEmperorWarriorEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGreatWallSpiderExistList,		m_pMagicGreatWallSpiderEmptyList);
	MAGIC_LIST_ACTION(m_pMagicGreatWallTwinHeadExistList,	m_pMagicGreatWallTwinHeadEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBearTangExistList,			m_pMagicBearTangEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSuperBearTangExistList,		m_pMagicSuperBearTangEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSFrozenGolemExistList,		m_pMagicSFrozenGolemEmptyList);
	MAGIC_LIST_ACTION(m_pMagicSFrozenQueenExistList,		m_pMagicSFrozenQueenEmptyList);
	MAGIC_LIST_ACTION(m_pMagicCatPetExistList,				m_pMagicCatPetEmptyList);
	MAGIC_LIST_ACTION(m_pMagicAuraBladeExistList,			m_pMagicAuraBladeEmptyList);
	MAGIC_LIST_ACTION(m_pMagicTripleDeathExistList,			m_pMagicTripleDeathEmptyList);
	MAGIC_LIST_ACTION(m_pMagicCosmosRabbitExistList,		m_pMagicCosmosRabbitEmptyList);
	MAGIC_LIST_ACTION(m_pMagicRudolphpetExistList,			m_pMagicRudolphpetEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDropGreatWallAsuraExistList,	m_pMagicDropGreatWallAsuraEmptyList);
	MAGIC_LIST_ACTION(m_pMagicAmbersaurusExistList,			m_pMagicAmbersaurusEmptyList);
	MAGIC_LIST_ACTION(m_pMagicRollingstegoExiteList,		m_pMagicRollingstegoEmptyList);
	MAGIC_LIST_ACTION(m_pMagicPumkinExiteList,				m_pMagicPumkinEmptyList);			// 펌킨
	MAGIC_LIST_ACTION(m_pMagicDragonStunExistList,			m_pMagicDragonStunEmptyList);
	MAGIC_LIST_ACTION(m_pMagicDragonFireExistList,			m_pMagicDragonFireEmptyList);

	MAGIC_LIST_ACTION(m_pMagicSkillBookCrashExistList,			m_pMagicSkillBookCrashEmptyList			);
	MAGIC_LIST_ACTION(m_pMagicSkillBookFlameBusterExistList,	m_pMagicSkillBookFlameBusterEmptyList	);
	MAGIC_LIST_ACTION(m_pMagicSkillBookFlameWhirlExistList,		m_pMagicSkillBookFlameWhirlEmptyList	);
	MAGIC_LIST_ACTION(m_pMagicSkillBookIceRainExistList,		m_pMagicSkillBookIceRainEmptyList		);
	MAGIC_LIST_ACTION(m_pMagicSkillBookSlingExistList,			m_pMagicSkillBookSlingEmptyList			);
	MAGIC_LIST_ACTION(m_pMagicSkillBookTornadoExistList,		m_pMagicSkillBookTornadoEmptyList		);		
	MAGIC_LIST_ACTION(m_pMagicSkillBookWetFogExistList,			m_pMagicSkillBookWetFogEmptyList		);
	MAGIC_LIST_ACTION(m_pMagicSkillBookWindcuterExistList,		m_pMagicSkillBookWindcuterEmptyList		);
	
	MAGIC_LIST_ACTION(m_pMagicClownBallExistList,				m_pMagicClownBallEmptyList);
	MAGIC_LIST_ACTION(m_pMagicBouquetExistList,					m_pMagicBouquetEmptyList);
	// 10 분간격으로 저장한다.
	if (pclClient->GameMode == GAMEMODE_SERVER)
	{
		if((pclClient->CurrentClock - ServerDebugClock) > 1000*60*10)
		{
			WriteMagicUseData();
			ServerDebugClock = pclClient->CurrentClock;
		}
	}
		
	return;
}

void CMagicMgr::Draw( )
{
	CMagicBaseObj	*pObj;
	//////////////////////////////////////////////////////////////////////////
	// DRAW MACRO
	//////////////////////////////////////////////////////////////////////////
	#define MAGIC_DRAW_ACTION(List)	\
	pObj = ( CMagicBaseObj * )List->GetFirst();\
	while ( pObj )\
	{\
		pObj->Draw();\
		pObj = ( CMagicBaseObj * )pObj->GetNext();\
	} 

	//////////////////////////////////////////////////////////////////////////

	// 
	MAGIC_DRAW_ACTION(m_pMagicFireBallExistList);
	MAGIC_DRAW_ACTION(m_pMagicBigWindExistList);
	MAGIC_DRAW_ACTION(m_pMagicThunderExistList);
	MAGIC_DRAW_ACTION(m_pMagicThunder1ExistList);
	MAGIC_DRAW_ACTION(m_pMagicWhirlPoolExistList);
	MAGIC_DRAW_ACTION(m_pMagicFlameExistList);
	MAGIC_DRAW_ACTION(m_pMagicRockSlideExistList);
	MAGIC_DRAW_ACTION(m_pMagicHealExistList);
	MAGIC_DRAW_ACTION(m_pMagicPoisonExistList);
	MAGIC_DRAW_ACTION(m_pMagicIceExistList);
	MAGIC_DRAW_ACTION(m_pMagicLightingFistExistList);
	MAGIC_DRAW_ACTION(m_pMagicPoisonFlameExistList);
	MAGIC_DRAW_ACTION(m_pMagicMeteorExistList);
	MAGIC_DRAW_ACTION(m_pMagicDropMoneyExistList);
	MAGIC_DRAW_ACTION(m_pMagicBlackInkExistList);
	MAGIC_DRAW_ACTION(m_pMagicDropSnowmanExistList);
	MAGIC_DRAW_ACTION(m_pMagicDropForkExistList);
	MAGIC_DRAW_ACTION(m_pMagicCurseDefenseExistList);
	MAGIC_DRAW_ACTION(m_pMagicCurseMovespeedExistList);
	MAGIC_DRAW_ACTION(m_pMagicThrowKnifeExistList);
	MAGIC_DRAW_ACTION(m_pMagicLaserBeamExistList);
	MAGIC_DRAW_ACTION(m_pMagicTriMeteorExistList);
	MAGIC_DRAW_ACTION(m_pMagicFireGroundExistList);
	MAGIC_DRAW_ACTION(m_pMagicSwordBombExistList);

	MAGIC_DRAW_ACTION(m_pMagicFreezeExplosionExistList);

	MAGIC_DRAW_ACTION(m_pMagicUpperDragonExistList);
	MAGIC_DRAW_ACTION(m_pMagicPalmWindExistList);
	MAGIC_DRAW_ACTION(m_pMagicSummonDragonExistList);

	MAGIC_DRAW_ACTION(m_pMagicFireBurnExistList);

	MAGIC_DRAW_ACTION(m_pMagicSkellGhostExistList);
	MAGIC_DRAW_ACTION(m_pMagicSwordMeteorExistList);
	MAGIC_DRAW_ACTION(m_pMagicFrozenExistList);

	MAGIC_DRAW_ACTION(m_pMagicTwinSpearExistList);
	MAGIC_DRAW_ACTION(m_pMagicFireWhirlWindExistList);
	MAGIC_DRAW_ACTION(m_pMagicNorthWindExistList);
	MAGIC_DRAW_ACTION(m_pMagicFireStormExistList);
	MAGIC_DRAW_ACTION(m_pMagicEarthQuakeExistList);
	MAGIC_DRAW_ACTION(m_pMagicSpearDragonExistList);

	MAGIC_DRAW_ACTION(m_pMagicGargoyleExistList);
	MAGIC_DRAW_ACTION(m_pMagicCerberusExistList);

	MAGIC_DRAW_ACTION(m_pMagicFrozenSorcererExistList);
	MAGIC_DRAW_ACTION(m_pMagicMarionetteExistList);
	MAGIC_DRAW_ACTION(m_pMagicFrokenExistList);
	MAGIC_DRAW_ACTION(m_pMagicIcehoundExistList);	
	MAGIC_DRAW_ACTION(m_pMagicFrozenGolemExistList);
	MAGIC_DRAW_ACTION(m_pMagicFrozenKnightExistList);
	MAGIC_DRAW_ACTION(m_pMagicFrozenQueenExistList);

	MAGIC_DRAW_ACTION(m_pMagicGotpaExsitList);
	MAGIC_DRAW_ACTION(m_pMagicNinjaPandaExistList);
	MAGIC_DRAW_ACTION(m_pMagicRaccoonDogExistList);
	MAGIC_DRAW_ACTION(m_pMagicBossMonkeyExistList);
	MAGIC_DRAW_ACTION(m_pMagicToadPoisonExistList);
	MAGIC_DRAW_ACTION(m_pMagicBloodBatExistList);
	MAGIC_DRAW_ACTION(m_pMagicWerewolfExistList);
	MAGIC_DRAW_ACTION(m_pMagicDevilousExistList);
	MAGIC_DRAW_ACTION(m_pMagicGangsiExistList);
    MAGIC_DRAW_ACTION(m_pMagicDraculaExistList);
	MAGIC_DRAW_ACTION(m_pMagicPriestExistList);
	MAGIC_DRAW_ACTION(m_pMagicHarpyExistList);
	MAGIC_DRAW_ACTION(m_pMagicMandragoraExistList);
	MAGIC_DRAW_ACTION(m_pMagicGiraffeExistList);
	MAGIC_DRAW_ACTION(m_pMagicPapaGoblinExistList);
	MAGIC_DRAW_ACTION(m_pMagicPirateSoldierExistList);
	MAGIC_DRAW_ACTION(m_pMagicPirateArcherExistList);
	MAGIC_DRAW_ACTION(m_pMagicBlackBossExistList);
	MAGIC_DRAW_ACTION(m_pMagicEmperorCavalryExistList);
	MAGIC_DRAW_ACTION(m_pMagicEmperorMasterExistList);
	MAGIC_DRAW_ACTION(m_pMagicGreatWallSoldierExistList);
	MAGIC_DRAW_ACTION(m_pMagicGreatWallMagicianExistList);
	MAGIC_DRAW_ACTION(m_pMagicEmperorWarriorExistList);
	MAGIC_DRAW_ACTION(m_pMagicGreatWallSpiderExistList);
	MAGIC_DRAW_ACTION(m_pMagicGreatWallTwinHeadExistList);
	MAGIC_DRAW_ACTION(m_pMagicBearTangExistList);
	MAGIC_DRAW_ACTION(m_pMagicSuperBearTangExistList);
	MAGIC_DRAW_ACTION(m_pMagicSFrozenGolemExistList);
	MAGIC_DRAW_ACTION(m_pMagicSFrozenQueenExistList);
	MAGIC_DRAW_ACTION(m_pMagicCatPetExistList);
	MAGIC_DRAW_ACTION(m_pMagicAuraBladeExistList);
	MAGIC_DRAW_ACTION(m_pMagicCosmosRabbitExistList);
	MAGIC_DRAW_ACTION(m_pMagicRudolphpetExistList);
	MAGIC_DRAW_ACTION(m_pMagicDropGreatWallAsuraExistList);
	MAGIC_DRAW_ACTION(m_pMagicAmbersaurusExistList);
	MAGIC_DRAW_ACTION(m_pMagicRollingstegoExiteList);
	MAGIC_DRAW_ACTION(m_pMagicPumkinExiteList);	// 펌킨
	MAGIC_DRAW_ACTION(m_pMagicDragonStunExistList);	
	MAGIC_DRAW_ACTION(m_pMagicDragonFireExistList);	

	MAGIC_DRAW_ACTION(m_pMagicSkillBookCrashExistList		);
	MAGIC_DRAW_ACTION(m_pMagicSkillBookFlameBusterExistList );
	MAGIC_DRAW_ACTION(m_pMagicSkillBookFlameWhirlExistList	);
	MAGIC_DRAW_ACTION(m_pMagicSkillBookIceRainExistList		);
	MAGIC_DRAW_ACTION(m_pMagicSkillBookSlingExistList		);
	MAGIC_DRAW_ACTION(m_pMagicSkillBookTornadoExistList		);	// 펌킨
	MAGIC_DRAW_ACTION(m_pMagicSkillBookWetFogExistList		);	
	MAGIC_DRAW_ACTION(m_pMagicSkillBookWindcuterExistList	);	

	MAGIC_DRAW_ACTION(m_pMagicClownBallExistList);	
	MAGIC_DRAW_ACTION(m_pMagicBouquetExistList);	

	return;
}

// Push
void CMagicMgr::Push( BYTE *pData, SI32 si32Size )
{
	CMagicMsgObj *pMagicMsgObj = ( CMagicMsgObj * ) m_pMsgEmptyList->PopFront();

	if ( pMagicMsgObj == NULL )
	{
#ifdef _DEBUG
		pclClient->pclLog->FilePrint( TEXT("SMLee.txt"), TEXT("void CMagicMgr::Push( BYTE *pData, SI32 si32Size ), 노드 부족") );
#endif
		return;
	}

	pMagicMsgObj->SetMagicMsg( pData, si32Size );
	m_pMsgExistList->PushBack( ( CObj * ) pMagicMsgObj );
	return;
}

// Pop
//bool CMagicMgr::Pop( stMagicMsgHeader *pMagicMsgHeader )
bool CMagicMgr::Pop( BYTE *pData, SI32 *pSize )
{
	CMagicMsgObj *pMagicMsgObj = ( CMagicMsgObj * )m_pMsgExistList->PopFront();

	if ( pMagicMsgObj )
	{
		pMagicMsgObj->GetMagicMsg( pData, pSize );

		m_pMsgEmptyList->PushBack( pMagicMsgObj );
		return true;
	}
	else
	{
		return false;
	}
}

SI32 CMagicMgr::GetAttributeDamage( stMagicInfo* _pstMagicInfo )
{
	if( NULL == _pstMagicInfo ) return 0;

	if( 0 >= _pstMagicInfo->sHeader.si32MagicKind ) return 0;

	cltCharServer* pclTargetChar = pclClient->pclCM->GetCharServer( _pstMagicInfo->sHeader.si32ToID );
	if( NULL == pclTargetChar ) return 0;
	
	SI32 siTargetKind = pclTargetChar->GetKind();
	if( 0 >= siTargetKind ) return 0;

	stMagicAttributeData* pstMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( _pstMagicInfo->sHeader.si32MagicKind );
	if( NULL == pstMagicAttributeData ) return 0;
	if( NULL == pclClient->pclKindInfoSet->pclKI[siTargetKind] ) return 0;
		
	switch( pstMagicAttributeData->siAttribute )
	{
		case MAGIC_ATTRIBUTE_FIRE:
			{
				if(pclTargetChar->clIdentity.siIdentity == IDENTITY_GMCREATE_MONSTER)
				{
					return pclClient->pclKindInfoSet->pclGMCommandKindInfo[siTargetKind]->clCharElemental.GetFire();
				}
				else
				{
					return pclClient->pclKindInfoSet->pclKI[siTargetKind]->pclCharElemental.GetFire();
				}				
			}
			break;
		case MAGIC_ATTRIBUTE_WATER:	
			{
				if(pclTargetChar->clIdentity.siIdentity == IDENTITY_GMCREATE_MONSTER)
				{
					return pclClient->pclKindInfoSet->pclGMCommandKindInfo[siTargetKind]->clCharElemental.GetWater();
				}
				else
				{
					return pclClient->pclKindInfoSet->pclKI[siTargetKind]->pclCharElemental.GetWater();
				}
			}
			break;
		case MAGIC_ATTRIBUTE_EARTH:	
			{
				if(pclTargetChar->clIdentity.siIdentity == IDENTITY_GMCREATE_MONSTER)
				{
					return pclClient->pclKindInfoSet->pclGMCommandKindInfo[siTargetKind]->clCharElemental.GetLand();
				}
				else
				{
					return pclClient->pclKindInfoSet->pclKI[siTargetKind]->pclCharElemental.GetLand();
				}	
			}
			break;
		case MAGIC_ATTRIBUTE_WIND:	
			{
				if(pclTargetChar->clIdentity.siIdentity == IDENTITY_GMCREATE_MONSTER)
				{
					return pclClient->pclKindInfoSet->pclGMCommandKindInfo[siTargetKind]->clCharElemental.GetWind();
				}
				else
				{
					return pclClient->pclKindInfoSet->pclKI[siTargetKind]->pclCharElemental.GetWind();
				}
			}
			break;
	}
	
	return 0;
}

bool CMagicMgr::GetMinMaxDamage(stMagicInfo *pstMagicInfo, SI32 siAddDamage, SI32 si32SkillLevel, SI32 &Min, SI32 &Max, SI32 siOrganUnique)
{
	Min = 0;	Max = 0;
	stMagicAttributeData* pMagicAttributeData = GetMagicAttribute( pstMagicInfo->sHeader.si32MagicKind );

	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;
	SI32 id = pstMagicInfo->sHeader.si32FromID ;

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind		= 0;
	SI32 	siSubMinDamage		= 0;
	SI32 	siSubMaxDamage		= 0;
	SI32	siSubInc			= 0;
	SI32	siAttributeDamage	= 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) )
	{
		siAttributeDamage = GetAttributeDamage( pstMagicInfo );	
		siAttributeDamage *= -1;

		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			if ( si32SkillLevel < MinSkillLevel )
			{
				Min = 0;	Max = 0;	return false;
			}

			// [진성] 보조 마법에 관한 계산.
			cltCharServer* pclChar = pclClient->pclCM->GetCharServer(id);
			if( pclChar )
			{
				if( pclChar->pclCharSkillBookInfo )
				{
					stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( pstMagicInfo->sHeader.si32MagicKind,
																																	siSubMagickind );
					if( psSubMagicData )
					{
						siSubMinDamage  = psSubMagicData->si32MagicMinDamage;
						siSubMaxDamage  = psSubMagicData->si32MagicMaxDamage;
						siSubInc		= psSubMagicData->si16MagicSkillIncreaseDamage;
					}
				}
			}
		}
		else // Client는 알필요 없다.
		{
			// 클라이언트는 1로 박아 넣는다
			id = 1;

			if ( si32SkillLevel < MinSkillLevel )
			{
				Min = 0;	Max = 0;	return true;
			}

			cltCharClient* pclChar = pclClient->pclCM->GetCharClient(id);
			if( pclChar )
			{
				if( pclChar->pclCharSkillBookInfo )
				{
					stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( pstMagicInfo->sHeader.si32MagicKind,
						siSubMagickind );
					if( psSubMagicData )
					{
						siSubMinDamage  = psSubMagicData->si32MagicMinDamage;
						siSubMaxDamage  = psSubMagicData->si32MagicMaxDamage;
						siSubInc		= psSubMagicData->si16MagicSkillIncreaseDamage;
					}
				}
			}
		}
	}
	

	
	// 영향을 -50 ~ 50 으로 한번 검사한다.
	SI32 siOrganValue = min(50, max(-50, siOrganUnique/2));

	SI32 siTempDamage;
	SI32 siMinDamage = pMagicAttributeData->si32MagicMinDamage;
	siTempDamage  = 0;
	siTempDamage += (siMinDamage * siAttributeDamage ) / 100;
	siTempDamage += (siMinDamage *  siSubMinDamage )   / 100;
	siMinDamage  += siTempDamage;
	
	SI32 siMaxDamage = pMagicAttributeData->si32MagicMaxDamage;
	siTempDamage  = 0;
	siTempDamage += (siMaxDamage * siAttributeDamage) / 100;
	siTempDamage += (siMaxDamage * siSubMaxDamage)    / 100;
	siMaxDamage  += siTempDamage;

	SI32 inc =  pMagicAttributeData->si16MagicSkillIncreaseDamage;
	inc += (inc * siSubInc) / 100;

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		//KHY - 0331 - 마법뎀쥐 적용.
		//SI32 id = pstMagicHeader->si32FromID;
		SI16 ToTalMindMgr = 0 ;
		SI16 ToTalMaxdMgr = 0 ;

		if (IsPC(id) ||IsNPC(id))
		{
			ToTalMindMgr = pclClient->pclCM->CR[id]->clPB.GetTotalMagicalMinAttackDmg();
			ToTalMaxdMgr = pclClient->pclCM->CR[id]->clPB.GetTotalMagicalMaxAttackDmg();
		}

		Min = siMinDamage + siMinDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000 + ToTalMindMgr;
		Max	= siMaxDamage + siMaxDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000 + ToTalMaxdMgr;

		Min+= Min * siOrganValue / 100;
		Max+= Max * siOrganValue / 100;
	}
	else
	{
		Min = siMinDamage + siMinDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000 + siAddDamage;
		Max	= siMaxDamage + siMaxDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000 + siAddDamage;

		Min+= Min * siOrganValue / 100;
		Max+= Max * siOrganValue / 100;
	}
	
	return true;
}


bool CMagicMgr::GetMinMaxSkillDamage(SI32 si32MagicKind, SI32 si32SkillLevel, SI32 &Min, SI32 &Max)
{

	Min = 0;	Max = 0;
	stMagicAttributeData *pMagicAttributeData = GetMagicAttribute( si32MagicKind );

	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind		= 0;
	SI32 	siSubMinDamage		= 0;
	SI32 	siSubMaxDamage		= 0;
	SI32	siSubInc			= 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) ) 
	{
		if(pclClient->GameMode == GAMEMODE_SERVER)
		{
			if ( si32SkillLevel < MinSkillLevel )
			{
				Min = 0;	Max = 0;	return false;
			}

			// 서버에서 사용되는게 없긴한데.. 혹시 모르니까.. 서버에서 사용되면 id를 인자로 받아야 함.
			/*cltCharServer* pclChar = pclClient->pclCM->GetCharServer(1);
			if( pclChar )
			{
				stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( si32MagicKind,	siSubMagickind );
				if( psSubMagicData )
				{
					siSubMinDamage  = psSubMagicData->si32MagicMinDamage;
					siSubMaxDamage  = psSubMagicData->si32MagicMaxDamage;
					siSubInc		= psSubMagicData->si16MagicSkillIncreaseDamage;
				}
			}*/
		}
		else // Client는 알필요 없다.
		{
			if ( si32SkillLevel < MinSkillLevel )
			{
				Min = 0;	Max = 0;	return true;
			}

			cltCharClient* pclChar = pclClient->pclCM->GetCharClient(1);
			if( pclChar )
			{
				stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( si32MagicKind,	siSubMagickind );
				if( psSubMagicData )
				{
					siSubMinDamage  = psSubMagicData->si32MagicMinDamage;
					siSubMaxDamage  = psSubMagicData->si32MagicMaxDamage;
					siSubInc		= psSubMagicData->si16MagicSkillIncreaseDamage;
				}
			}
		}
	}
	
	SI32 siMinDamage = pMagicAttributeData->si32MagicMinDamage;
	SI32 siMaxDamage = pMagicAttributeData->si32MagicMaxDamage;

	SI32 inc =  pMagicAttributeData->si16MagicSkillIncreaseDamage; 
	inc += (inc * siSubInc) / 100;

	Min = siMinDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000;
	Min += (Min * siSubMinDamage) / 100;
	Max	= siMaxDamage * inc * ( si32SkillLevel - MinSkillLevel ) / 10000;
	Max += (Max * siSubMaxDamage) / 100;

	return true;
}

bool CMagicMgr::CalculateDamage(SI32 Min, SI32 Max, SI32 &Damage)
{
	Damage = Min + ( rand() % ( Max - Min + 1) )  ;
	return true;
}




void CMagicMgr::IncreaseMagicUse( SI32 kind )
{
	if(kind <0 || kind >= MAX_MAGIC_KIND_NUM)	return;
	
	ServerDebugMagicUse[kind]++;

}


void CMagicMgr::WriteMagicUseData()
{
	FILE *fp = _tfopen( TEXT("Config/MagicUse.log"), TEXT("wt") );

	if ( fp == NULL )	return;

	stMagicAttributeData *pAttribute = NULL;
	for ( SI32 i = 0; i < MAX_MAGIC_KIND_NUM; ++i )
	{
		pAttribute = GetMagicAttribute(i);
		if(pAttribute)
		{
			_ftprintf( fp, TEXT("%d\t%s\t%s\t%d\n"), i, pAttribute->strMagicKindName, pAttribute->strMagicName, ServerDebugMagicUse[i]);
		}
	}
	
	fclose( fp );
}

//-------------------------------------------------------------------------------------------------------------------------------
// 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선 경계선
//-------------------------------------------------------------------------------------------------------------------------------
void CNMagicBufMgr::Set( CNMagicBufMgr* pclMagicBufMgr )
{
	if ( pclMagicBufMgr )
	{
		memcpy( this, pclMagicBufMgr, sizeof(CNMagicBufMgr) );
	}
}
//----------------------------------------------------------------------------------------
// 이 함수 사용전에 반드시 CanMagicBuf() 함수를 사용해서 사용가능한지를 판단해야 한다
// 이 함수는 단순히 지금 사용하려는 마법과 같은지만을 검사해서 넣기 때문에 쿨타임같은것은
// 검사하지 않는다. 
// 
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::PushMagicBuf( SI32 siMagicKind, SI32 siDecMagicDelayRate, SI32 siID )
{
	stMagicAttributeData* pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( siMagicKind );
	if ( NULL == pMagicAttributeData )
	{
		return;
	}

	// 버프형 타입이 아니면 쓸 수 없다
	if ( MAGIC_CASTTYPE_BUF != pMagicAttributeData->si08MagicCastType )
	{
		return;
	}

	SI32 siEmptySlotNum = -1;

	// 현재 마법이나 빈슬롯을 찾는다(뒤에서 부터 찾는다)
	for ( SI32 siIndex=(MAX_MAGICBUF_NUM-1); siIndex>=0; --siIndex )
	{
		if ( m_clMagicBuf[siIndex].GetMagicKind() == siMagicKind )
		{
			siEmptySlotNum = siIndex;
			break;
		}
		else if ( m_clMagicBuf[siIndex].IsEmpty() )
		{
			siEmptySlotNum = siIndex;
		}
	}

	// [진성] 보조 마법에 관한 계산.
	SI32	siSubMagickind		= 0;
	SI32 	siSubDelayFrame		= 0;
	SI32	siSubActionFrame	= 0;
	if( pclClient->IsCountrySwitch(Switch_SkillBook) )
	{
		if( GAMEMODE_SERVER == pclClient->GameMode )
		{
			cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siID);
			if( pclChar )
			{
				if( pclChar->pclCharSkillBookInfo )
				{
					stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( siMagicKind, siSubMagickind );
					if( psSubMagicData )
					{
						siSubDelayFrame	 = psSubMagicData->si32MagicDelay;
						siSubActionFrame = psSubMagicData->si32MagicMaxActionFrame;
					}
				}
			}	
		}
		else
		{
			cltCharClient* pclChar = pclClient->pclCM->GetCharClient(siID);
			if( pclChar )
			{
				if( pclChar->pclCharSkillBookInfo )
				{
					stMagicAttributeData* psSubMagicData = pclChar->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( siMagicKind, siSubMagickind );
					if( psSubMagicData )
					{
						siSubDelayFrame	 = psSubMagicData->si32MagicDelay;
						siSubActionFrame = psSubMagicData->si32MagicMaxActionFrame;
					}
				}
			}	
		}
	}
		
	// 사용할수 있는 슬롯을 찾았다면
	if ( (-1 < siEmptySlotNum) && (MAX_MAGICBUF_NUM > siEmptySlotNum) )
	{
		SI32 siDelayFrame	= pMagicAttributeData->si32MagicDelay;
		// 마법 지연시간 감소
		if ( 0 < siDecMagicDelayRate )
		{
			siDelayFrame -= (pMagicAttributeData->si32MagicDelay * siDecMagicDelayRate / 100);
		}
		siDelayFrame += (siDelayFrame * siSubDelayFrame) / 100;

		SI32 siActionFrame	= pMagicAttributeData->si32MagicMaxActionFrame;
		siActionFrame +=  (siActionFrame * siSubActionFrame) / 100;
		
		SI32 siStartTime	= pclClient->CurrentClock;
		SI32 siActionTime	= (siActionFrame / ConstFrameRate_Low) * 1000;

		m_clMagicBuf[siEmptySlotNum].Set( siMagicKind, siActionFrame, siDelayFrame, siStartTime, siActionTime );
		
		// 사용량 통계를 위한 추가
		pclClient->pclMagicManager->IncreaseMagicUse( siMagicKind );
	}

}

//----------------------------------------------------------------------------------------
// 해당 마법을 재사용 할 수 있는지 쿨타임을 검사한다
//----------------------------------------------------------------------------------------
bool CNMagicBufMgr::CheckDelay( SI32 siMagicKind )
{
	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		if ( true == m_clMagicBuf[siIndex].IsEmpty() )
		{
			continue;
		}

		// 같은 마법이고 딜레이 타임이 아직도 존재한다면 false
		if ( m_clMagicBuf[siIndex].GetMagicKind() == siMagicKind )
		{
			if ( 0 < m_clMagicBuf[siIndex].GetDelayFrame() )
			{
				return false;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------
// 해당 마법을 사용할 수 있는 슬롯이 있는지 검사한다
//----------------------------------------------------------------------------------------
bool CNMagicBufMgr::CheckMagicBufSlot( SI32 siMagicKind )
{
	// 해당 마법이 아닌 다른 마법으로 슬롯이 다 차있을 경우도 있으므로 빈 슬롯도 검사한다
	SI32 siEmptySlot = 0;

	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		if ( m_clMagicBuf[siIndex].GetMagicKind() == siMagicKind )
		{
			return true;
		}
		else if ( true == m_clMagicBuf[siIndex].IsEmpty() )
		{
			siEmptySlot++;		
		}
	}

	// 빈슬롯이 있어도 성공
	if ( 1 <= siEmptySlot )
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------
// 실질적인 Action()함수라고 생각하면 되겠다 하지만 기존 프레임 사용방식에서 클라이언트
// 에서 프레임 스킵으로 인한 시간이 추가되었다.
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::DecFrame( SI32 siDecFrame )
{
	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		CNMagicBuf* pclMagicBuf = &m_clMagicBuf[siIndex];
		if ( NULL == pclMagicBuf )
		{
			continue;
		}

		if ( true == pclMagicBuf->IsEmpty() )
		{
			continue;
		}
		
		SI32 siDelayFrame = pclMagicBuf->GetDelayFrame();
		if ( 0 < siDelayFrame )
		{
			siDelayFrame = pclMagicBuf->SubDelayFrame( siDecFrame );
			if ( 0 > siDelayFrame )
			{
				pclMagicBuf->SetDelayFrame( 0 );
			}
		}

		SI32 siActionFrame = pclMagicBuf->GetActionFrame();
		if ( 0 < siActionFrame )
		{
			siActionFrame = pclMagicBuf->SubActionFrame( siDecFrame );
			if ( 0 > siActionFrame )
			{
				pclMagicBuf->SetActionFrame( 0 );
			}
		}

		// 두 맴버 변수 모두 0이하 일때 버프를 지운다
		if ( (0 >= siDelayFrame) && (0 >= siActionFrame) )
		{
			m_clMagicBuf[siIndex].Init();
		}
		// 클라이언트일때는 프레임 보다는 시간으로 계산해서 초기화 한다
		else if ( GAMEMODE_CLIENT == pclClient->GameMode )
		{
			SI32 siStartTime	= m_clMagicBuf[siIndex].GetStartTime();
			SI32 siActionTime	= m_clMagicBuf[siIndex].GetActionTime();

			if ( (DWORD)(siStartTime+siActionTime) <= pclClient->CurrentClock )
			{
				m_clMagicBuf[siIndex].Init();
			}
		}
	}

	MoveToFront();
}

//----------------------------------------------------------------------------------------
// 마법 이펙트를 그리는 함수 클라리언트에서만 작동한다
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::Draw( SI32 si32CenterXPos, SI32	si32CenterYPos )
{
	if ( GAMEMODE_CLIENT != pclClient->GameMode )
	{
		return;
	}

	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		CNMagicBuf* pclMagicBuf = &m_clMagicBuf[siIndex];
		if ( NULL == pclMagicBuf )
		{
			continue;
		}

		if ( true == pclMagicBuf->IsEmpty() )
		{
			continue;
		}

		SI16 siGetMaxFrame	= pclMagicBuf->GetMaxFrame();
		SI16 siGetDrawFrame	= pclMagicBuf->GetDrawFrame();

		if ( /**/((0 < siGetMaxFrame) && (siGetDrawFrame < siGetMaxFrame))/**/ || /**/(0 >= siGetMaxFrame)/**/ )
		{
			MagicBufDraw( siIndex, si32CenterXPos, si32CenterYPos );
		}
	}
}

//----------------------------------------------------------------------------------------
// 실질적으로 이펙트를 그리는 함수 위의 함수는 이 함수를 효율적으로 호출하기 위한 인터페이스함수
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::MagicBufDraw( SI32 siMagicBufSlotNum, SI32 si32CenterXPos, SI32 si32CenterYPos )
{
	if ( (0 > siMagicBufSlotNum) || (MAX_MAGICBUF_NUM <= siMagicBufSlotNum) )
	{
		return;
	}

	CNMagicBuf* pclMagicBuf = &m_clMagicBuf[siMagicBufSlotNum];
	if ( NULL == pclMagicBuf )
	{
		return;
	}

	if ( true == pclMagicBuf->IsEmpty() )
	{
		return;
	}

	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pMagicAttributePtr )
	{
		return;
	}

	SI32 siMagicKind	= pclMagicBuf->GetMagicKind();
	SI32 siMagicGroup	= pMagicAttributePtr->GetMagicGroup(siMagicKind);

	SI32 siGIMGIndex	= GetGIMGIndex( siMagicGroup );
	if ( -1 >= siGIMGIndex )
	{
		return;
	}

	TSpr* pMagicSpr = pclClient->GetGlobalSpr( siGIMGIndex );
	if ( NULL == pMagicSpr )
	{
		return;
	}

	SI32 siMaxFrame = pMagicSpr->GetImageNum();
	if ( 0 >= pclMagicBuf->GetMaxFrame() )
	{
		pclMagicBuf->SetMaxFrame( siMaxFrame );
	}

	SI16 siDrawFrame	= pclMagicBuf->GetDrawFrame();
	SI32 siDrawX		= si32CenterXPos - pMagicSpr->GetXSize() / 2;
	SI32 siDrawY		= si32CenterYPos - pMagicSpr->GetYSize() / 2;

	GP.PutSprScreen( pMagicSpr, siDrawX, siDrawY, siDrawFrame );

	// 크리티컬 증가 이미지는 2개가 중복해서 그려진다
	if ( MAGIC_GROUP_BLESSCIRICAL == siMagicGroup )
	{
		TSpr* pMagicSpr2 = pclClient->GetGlobalSpr( GIMG_MAGIC_BLESSCIRICAL2 );
		if ( pMagicSpr2 )
		{
			GP.PutSprScreen( pMagicSpr2, siDrawX, siDrawY, siDrawFrame );	
		}
	}

	// 프레임이 0일때 효과음을 연주한다
	if ( 0 == siDrawFrame )
	{
		PlayTheEffect( siMagicGroup );
	}

	// 그리기 관련 프레임을 1 증가 시킨다
	pclMagicBuf->AddDrawFrame( 1 );

}

//----------------------------------------------------------------------------------------
// 마법그룹에 해당하는 글로벌 이미지 인덱스를 가져오는 함수
//----------------------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetGIMGIndex( SI32 siMagicGroup )
{
	SI32 siGIMGIndex = -1;

	switch ( siMagicGroup )
	{
		case MAGIC_GROUP_BLESSDAMAGE: 			{	siGIMGIndex = GIMG_MAGIC_BLESSDAMAGE;			}	break;
		case MAGIC_GROUP_BLESSCIRICAL: 			{	siGIMGIndex = GIMG_MAGIC_BLESSCIRICAL1;			}	break;
		case MAGIC_GROUP_BLESSHITRATE: 			{	siGIMGIndex = GIMG_MAGIC_BLESSHITRATE;			}	break;
		case MAGIC_GROUP_BLESSRANGE: 			{	siGIMGIndex = GIMG_MAGIC_BLESSRANGE;			}	break;
		case MAGIC_GROUP_BLESSATTACKINTERVAL:	{	siGIMGIndex = GIMG_MAGIC_BLESSATTACKINTERVAL;	}	break;
		case MAGIC_GROUP_BLESSMPRECOVERYTIME:	{	siGIMGIndex = GIMG_MAGIC_BLESSMPRECOVERYTIME;	}	break;
		case MAGIC_GROUP_BLESSHP:				{	siGIMGIndex = GIMG_MAGIC_BLESSHP;				}	break;
	}

	return siGIMGIndex;
}

//---------------------------------------------------------------------------------------
// 공격력 증가( 실제 데미지값으로 내보낸다 )
//---------------------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncDamage( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSDAMAGE );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );


	if ( (20 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 20;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 40;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 50;
	}

	return siReturnValue;
}

//----------------------------------------------------------------------------
// 크리티컬률 증가( %로 값을 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncCriticalRate( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSCIRICAL );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (10 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 10;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 14;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 20;
	}


	return siReturnValue;
}

//----------------------------------------------------------------------------
// 명중률 증가 ( %로 값을 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncHitRate( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSHITRATE );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (10 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 10;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 14;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 20;
	}


	return siReturnValue;
}

//----------------------------------------------------------------------------
// 사정거리 증가( 실제 거리값으로 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncRange( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSRANGE );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (20 <= siSkillLevel) && (60 >= siSkillLevel) )
	{
		siReturnValue = 1;
	}
	else if ( 61 <= siSkillLevel )
	{
		siReturnValue = 2;
	}


	return siReturnValue;
}

//----------------------------------------------------------------------------
// 공격속도 증가( %로 값을 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncAttackInterval( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSATTACKINTERVAL );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (10 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 10;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 14;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 20;
	}


	return siReturnValue;
}

//----------------------------------------------------------------------------
// 마법 회복속도 증가( %로 값을 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncMPRecoveryTime( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSMPRECOVERYTIME );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (40 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 200;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 300;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 400;
	}


	return siReturnValue;
}

//----------------------------------------------------------------------------
// 체력증가 ( 절대값 수치로 내보낸다 )
//----------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetIncHP( cltCharCommon* pclCharCommon )
{
	SI32 siReturnValue = 0;

	SI32 siMagicBufIndex = IsHaveMagicGroup( MAGIC_GROUP_BLESSHP );
	if ( MBM_VARIABLE_RET_DONOTHAVE == siMagicBufIndex )
	{
		return 0;
	}

	SI32 siMagicKind	= m_clMagicBuf[siMagicBufIndex].GetMagicKind();
	SI32 siSkillLevel	= GetTotalSkillLevel( pclCharCommon, siMagicKind );

	if ( (40 <= siSkillLevel) && (50 >= siSkillLevel) )
	{
		siReturnValue = 50;
	}
	else if ( (51 <= siSkillLevel) && (70 >= siSkillLevel) )
	{
		siReturnValue = 100;
	}
	else if ( 71 <= siSkillLevel )
	{
		siReturnValue = 150;
	}


	return siReturnValue;
}
//----------------------------------------------------------------------------------------
// 맴버 변수인 버프마법중 siIndex를 포인터로 가져온다
//----------------------------------------------------------------------------------------
CNMagicBuf* CNMagicBufMgr::GetMagicBuf( SI32 siIndex )
{
	if ( (0 > siIndex) || (MAX_MAGICBUF_NUM <= siIndex) )
	{
		return NULL;
	}

	return &m_clMagicBuf[siIndex];
}

//----------------------------------------------------------------------------------------
// 배열 인덱스 0번을 향해 이동시키는 함수
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::MoveToFront( void )
{
	for ( SI32 siAllRoundCount=0; siAllRoundCount<MAX_MAGICBUF_NUM; ++siAllRoundCount )
	{
		if ( 0 >= siAllRoundCount )
		{
			continue;
		}

		for ( SI32 siReverseCount=siAllRoundCount; siReverseCount>0; --siReverseCount  )
		{
			if ( true == m_clMagicBuf[siReverseCount].IsEmpty() )
			{
				break;
			}

			SI32 siIndex = siReverseCount-1;
			if ( true == m_clMagicBuf[siIndex].IsEmpty() )
			{
				m_clMagicBuf[siIndex].Set( &m_clMagicBuf[siReverseCount] );
				m_clMagicBuf[siReverseCount].Init();
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// 해당 마법 그룹이 사용중인지 검사하는 함수
//----------------------------------------------------------------------------------------
SI32 CNMagicBufMgr::IsHaveMagicGroup( SI32 siMagicGroup )
{
	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pMagicAttributePtr )
	{
		return false;
	}

	for ( SI32 siIndex=0; siIndex<MAX_MAGICBUF_NUM; ++siIndex )
	{
		if ( m_clMagicBuf[siIndex].IsEmpty() )
		{
			continue;
		}

		SI32 siMagicKind = m_clMagicBuf[siIndex].GetMagicKind();
		if ( pMagicAttributePtr->GetMagicGroup(siMagicKind) == siMagicGroup )
		{
			return siIndex;
		}
	}

	return MBM_VARIABLE_RET_DONOTHAVE;

}

//----------------------------------------------------------------------------------------
// 각각의 마법 그룹에서 스킬레벨을 필요로 하기때문에 쉽게 빼올수 있도록 함수화함
//----------------------------------------------------------------------------------------
SI32 CNMagicBufMgr::GetTotalSkillLevel( cltCharCommon* pclCharCommon, SI32 siMagicKind )
{
	if ( (NULL == pclCharCommon) || (0 > siMagicKind) )
	{
		return 0;
	}

	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	if ( NULL == pMagicAttributePtr )
	{
		return 0;
	}

	SI32 siLevel		= pclCharCommon->pclCI->clIP.GetLevel();
	SI32 siSkillType	= pMagicAttributePtr->GetMagicSkillType( siMagicKind );
	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	if( SKILL_SIKLLBOOK == siSkillType && IsPC(pclCharCommon->GetID())  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCharCommon->pclCharSkillBookInfo->GetSkillBookMagic( pclCharCommon->GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return 0;

		siSkillType = pclSkillBookMagic->GetSkillUnique();
	}

#ifdef _SAFE_MEMORY	
	SI32 siSkillLevel	= pclCharCommon->pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, 0, siLevel, &pclCharCommon->pclCI->clCharItem.clItem[0] );
#else	
	SI32 siSkillLevel	= pclCharCommon->pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, 0, siLevel, pclCharCommon->pclCI->clCharItem.clItem );
#endif

	return siSkillLevel;
}

//----------------------------------------------------------------------------------------
// 마법의 효과음을 연주한다
//----------------------------------------------------------------------------------------
void CNMagicBufMgr::PlayTheEffect( SI32 siMagicGroup )
{
	SI32 siEffect = 0;

	switch ( siMagicGroup )
	{
		case MAGIC_GROUP_BLESSDAMAGE:
			{	
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSDAMAGE"));
			}	
			break;

		case MAGIC_GROUP_BLESSCIRICAL:
			{	
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSCIRICAL"));
			}
			break;

		case MAGIC_GROUP_BLESSHITRATE:
			{
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSHITRATE"));
			}
			break;

		case MAGIC_GROUP_BLESSRANGE:
			{	
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSRANGE"));
			}
			break;

		case MAGIC_GROUP_BLESSATTACKINTERVAL:
			{
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSATTACKINTERVAL"));
			}
			break;

		case MAGIC_GROUP_BLESSMPRECOVERYTIME:
			{
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSMPRECOVERYTIME"));
			}
			break;

		case MAGIC_GROUP_BLESSHP:
			{
				siEffect = pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BLESSHP"));
			}
			break;

		default:
			{
				return;
			}
	}

	if ( siEffect )
	{
		pclClient->PushEffect( siEffect, 0, 0 );
	}
}

