#ifndef _MAGICMGR_H_
#define _MAGICMGR_H_

#include "MagicBaseInfo.h"
#include "../../Common/SMLib/LibList/LibListMgr.h"


class	CMagicBaseObj;
class	CMagicMsgObj;

class	CMagicAttribute;

class	cltMapCommon;

class CMagicMgr : public CLibListMgr
{
public:
	CMagicMgr();
	~CMagicMgr();

	void					Initialize();
	void					Destroy();

	void					DeleteMagic();
	void					ListInitialize( CLibList *pExistList, CLibList *pEmptyList );

	//------------------Max개의 메시지를 초기에 만드는 부분---	
	void					CreateMaxMsg();
	//--------------------------------------------------------

	//------------------EmptyList 에 넣는 부분----------------	
	void					CreateMaxEmptyObj();

	//--------------------------------------------------------

	//------------------마법의 기본 속성 데이터 알아낼려고----	
	CMagicAttribute			*GetMagicAttributePtr();
	stMagicAttributeData	*GetMagicAttribute( SI32 si32MagicKind );
	//--------------------------------------------------------	


	//------------------마법 기술에 따른 마법력 소모량을 받아옮
	SI32					GetSpendMana( SI32 si32MagicKind, SI32 siSkillLevel ,SI32 id = 0 );
	//--------------------------------------------------------	


	bool					SetMagic( stMagicHeader *pstMagicHeader, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique );		// 마법의 개수가 초과하면 false 리턴	
	void					Action();			// Action
	void					Draw();			// Draw

	void					Push( BYTE *pData, SI32 si32Size );		// Push
	bool					Pop( BYTE *pData, SI32 *pSize );		// Pop
	
	bool					CalculateDamage(SI32 Min, SI32 Max, SI32 &Damage);
	bool					GetMinMaxDamage(stMagicInfo *pstMagicInfo, SI32 siAddDamage, SI32 si32SkillLevel, SI32 &Min, SI32 &Max, SI32 siOrganUnique);
	bool					GetMinMaxSkillDamage(SI32 si32MagicKind, SI32 si32SkillLevel, SI32 &Min, SI32 &Max);
	SI32 					GetAttributeDamage( stMagicInfo* _pstMagicInfo );
	


	void					IncreaseMagicUse( SI32 kind );
	void					WriteMagicUseData();
private:

	// LIST
	CLibList			*m_pMagicFireBallExistList,				*m_pMagicFireBallEmptyList;
	CLibList			*m_pMagicBigWindExistList,				*m_pMagicBigWindEmptyList;
	CLibList			*m_pMagicThunderExistList,				*m_pMagicThunderEmptyList;
	CLibList			*m_pMagicThunder1ExistList,				*m_pMagicThunder1EmptyList;
	CLibList			*m_pMagicWhirlPoolExistList,			*m_pMagicWhirlPoolEmptyList;
	CLibList			*m_pMagicFlameExistList,				*m_pMagicFlameEmptyList;
	CLibList			*m_pMagicHealExistList,					*m_pMagicHealEmptyList;
	CLibList			*m_pMagicRockSlideExistList,			*m_pMagicRockSlideEmptyList;
	CLibList			*m_pMagicPoisonExistList,				*m_pMagicPoisonEmptyList;
	CLibList			*m_pMagicIceExistList,					*m_pMagicIceEmptyList;
	CLibList			*m_pMagicLightingFistExistList,			*m_pMagicLightingFistEmptyList;
	CLibList			*m_pMagicPoisonFlameExistList,			*m_pMagicPoisonFlameEmptyList;
	CLibList			*m_pMagicMeteorExistList,				*m_pMagicMeteorEmptyList;
	CLibList			*m_pMagicDropMoneyExistList,			*m_pMagicDropMoneyEmptyList;
	CLibList			*m_pMagicBlackInkExistList,				*m_pMagicBlackInkEmptyList;
	CLibList			*m_pMagicDropSnowmanExistList,			*m_pMagicDropSnowmanEmptyList;
	CLibList			*m_pMagicDropForkExistList,				*m_pMagicDropForkEmptyList;
	CLibList			*m_pMagicCurseDefenseExistList,			*m_pMagicCurseDefenseEmptyList;
	CLibList			*m_pMagicCurseMovespeedExistList,		*m_pMagicCurseMovespeedEmptyList;
	CLibList			*m_pMagicThrowKnifeExistList,			*m_pMagicThrowKnifeEmptyList;
	CLibList			*m_pMagicLaserBeamExistList,			*m_pMagicLaserBeamEmptyList;
	CLibList			*m_pMagicTriMeteorExistList,			*m_pMagicTriMeteorEmptyList;
	CLibList			*m_pMagicFireGroundExistList,			*m_pMagicFireGroundEmptyList;
	CLibList			*m_pMagicSwordBombExistList,			*m_pMagicSwordBombEmptyList;

	CLibList			*m_pMagicFreezeExplosionExistList,		*m_pMagicFreezeExplosionEmptyList;

	CLibList			*m_pMagicUpperDragonExistList,			*m_pMagicUpperDragonEmptyList;
	CLibList			*m_pMagicPalmWindExistList,				*m_pMagicPalmWindEmptyList;
	CLibList			*m_pMagicSummonDragonExistList,			*m_pMagicSummonDragonEmptyList;

	CLibList			*m_pMagicFireBurnExistList,				*m_pMagicFireBurnEmptyList;

	CLibList			*m_pMagicSkellGhostExistList,			*m_pMagicSkellGhostEmptyList;

	CLibList			*m_pMagicSwordMeteorExistList,			*m_pMagicSwordMeteorEmptyList;

	CLibList			*m_pMagicFrozenExistList,				*m_pMagicFrozenEmptyList;

	CLibList			*m_pMagicTwinSpearExistList,			*m_pMagicTwinSpearEmptyList;
	CLibList			*m_pMagicFireWhirlWindExistList,		*m_pMagicFireWhirlWindEmptyList;

	CLibList			*m_pMagicNorthWindExistList,			*m_pMagicNorthWindEmptyList;

	CLibList			*m_pMagicFireStormExistList,			*m_pMagicFireStormEmptyList;
	CLibList			*m_pMagicEarthQuakeExistList,			*m_pMagicEarthQuakeEmptyList;
	CLibList			*m_pMagicSpearDragonExistList,			*m_pMagicSpearDragonEmptyList;

	CLibList			*m_pMagicGargoyleExistList,				*m_pMagicGargoyleEmptyList;
	CLibList			*m_pMagicCerberusExistList,				*m_pMagicCerberusEmptyList;

	CLibList			*m_pMagicFrozenSorcererExistList,		*m_pMagicFrozenSorcererEmptyList;
	CLibList			*m_pMagicMarionetteExistList,			*m_pMagicMarionetteEmptyList;
	CLibList			*m_pMagicFrokenExistList,				*m_pMagicFrokenEmptyList;
	CLibList			*m_pMagicIcehoundExistList,				*m_pMagicIcehoundEmptyList;
	CLibList			*m_pMagicFrozenGolemExistList,			*m_pMagicFrozenGolemEmptyList;
	CLibList			*m_pMagicFrozenKnightExistList,			*m_pMagicFrozenKnightEmptyList;
	CLibList			*m_pMagicFrozenQueenExistList,			*m_pMagicFrozenQueenEmptyList;

	CLibList			*m_pMagicGotpaExsitList,				*m_pMagicGotpaEmptyList;
	CLibList			*m_pMagicNinjaPandaExistList,			*m_pMagicNinjaPandaEmptyList;
	CLibList			*m_pMagicRaccoonDogExistList,			*m_pMagicRaccoonDogEmptyList;
	CLibList			*m_pMagicBossMonkeyExistList,			*m_pMagicBossMonkeyEmptyList;
	CLibList			*m_pMagicToadPoisonExistList,			*m_pMagicToadPoisonEmptyList;
	CLibList			*m_pMagicBloodBatExistList,				*m_pMagicBloodBatEmptyList;
	CLibList			*m_pMagicWerewolfExistList,				*m_pMagicWerewolfEmptyList;
	CLibList			*m_pMagicDevilousExistList,				*m_pMagicDevilousEmptyList;
	CLibList			*m_pMagicGangsiExistList,				*m_pMagicGangsiEmptyList;
	CLibList			*m_pMagicDraculaExistList,				*m_pMagicDraculaEmptyList;
	CLibList			*m_pMagicPriestExistList,				*m_pMagicPriestEmptyList;
	
    CLibList			*m_pMagicHarpyExistList,				*m_pMagicHarpyEmptyList;
	CLibList			*m_pMagicMandragoraExistList,			*m_pMagicMandragoraEmptyList;
	CLibList			*m_pMagicGiraffeExistList,				*m_pMagicGiraffeEmptyList;
	CLibList			*m_pMagicPapaGoblinExistList,			*m_pMagicPapaGoblinEmptyList;
	CLibList			*m_pMagicPirateSoldierExistList,		*m_pMagicPirateSoldierEmptyList;
	CLibList			*m_pMagicPirateArcherExistList,			*m_pMagicPirateArcherEmptyList;
	CLibList			*m_pMagicBlackBossExistList,			*m_pMagicBlackBossEmptyList;
	CLibList			*m_pMagicEmperorCavalryExistList,		*m_pMagicEmperorCavalryEmptyList;
	CLibList			*m_pMagicEmperorMasterExistList,		*m_pMagicEmperorMasterEmptyList;
	CLibList			*m_pMagicGreatWallSoldierExistList,		*m_pMagicGreatWallSoldierEmptyList;
	CLibList			*m_pMagicGreatWallMagicianExistList,	*m_pMagicGreatWallMagicianEmptyList;
	CLibList			*m_pMagicEmperorWarriorExistList,		*m_pMagicEmperorWarriorEmptyList;
	CLibList			*m_pMagicGreatWallSpiderExistList,		*m_pMagicGreatWallSpiderEmptyList;
	CLibList			*m_pMagicGreatWallTwinHeadExistList,	*m_pMagicGreatWallTwinHeadEmptyList;
	CLibList			*m_pMagicBearTangExistList,				*m_pMagicBearTangEmptyList;
	CLibList			*m_pMagicSuperBearTangExistList,		*m_pMagicSuperBearTangEmptyList;
	CLibList			*m_pMagicSFrozenGolemExistList,			*m_pMagicSFrozenGolemEmptyList;
	CLibList			*m_pMagicSFrozenQueenExistList,			*m_pMagicSFrozenQueenEmptyList;
	CLibList			*m_pMagicCatPetExistList,				*m_pMagicCatPetEmptyList;
	CLibList			*m_pMagicAuraBladeExistList,			*m_pMagicAuraBladeEmptyList;
	CLibList			*m_pMagicTripleDeathExistList,			*m_pMagicTripleDeathEmptyList;
	CLibList			*m_pMagicCosmosRabbitExistList,			*m_pMagicCosmosRabbitEmptyList;
	CLibList			*m_pMagicRudolphpetExistList,			*m_pMagicRudolphpetEmptyList;
	CLibList			*m_pMagicDropGreatWallAsuraExistList,	*m_pMagicDropGreatWallAsuraEmptyList;
	CLibList			*m_pMagicAmbersaurusExistList,			*m_pMagicAmbersaurusEmptyList;
	CLibList			*m_pMagicRollingstegoExiteList,			*m_pMagicRollingstegoEmptyList;
	CLibList			*m_pMagicPumkinExiteList,				*m_pMagicPumkinEmptyList;					// 펌킨
	CLibList			*m_pMagicDragonFireExistList,			*m_pMagicDragonFireEmptyList;				//[진성] 미니 드래곤.
	CLibList			*m_pMagicDragonStunExistList,			*m_pMagicDragonStunEmptyList;				//[진성] 드래곤.

	CLibList			*m_pMagicSkillBookCrashExistList,		*m_pMagicSkillBookCrashEmptyList;			//[진성] 스킬북
	CLibList			*m_pMagicSkillBookFlameBusterExistList,	*m_pMagicSkillBookFlameBusterEmptyList;		//[진성] 스킬북
	CLibList			*m_pMagicSkillBookFlameWhirlExistList,	*m_pMagicSkillBookFlameWhirlEmptyList;		//[진성] 스킬북.
	CLibList			*m_pMagicSkillBookIceRainExistList,		*m_pMagicSkillBookIceRainEmptyList;			//[진성] 스킬북.
	CLibList			*m_pMagicSkillBookSlingExistList,		*m_pMagicSkillBookSlingEmptyList;			//[진성] 스킬북.
	CLibList			*m_pMagicSkillBookTornadoExistList,		*m_pMagicSkillBookTornadoEmptyList;			//[진성] 스킬북.
	CLibList			*m_pMagicSkillBookWetFogExistList,		*m_pMagicSkillBookWetFogEmptyList;			//[진성] 스킬북.
	CLibList			*m_pMagicSkillBookWindcuterExistList,	*m_pMagicSkillBookWindcuterEmptyList;		//[진성] 스킬북.

	CLibList			*m_pMagicClownBallExistList,			*m_pMagicClownBallEmptyList;
	CLibList			*m_pMagicBouquetExistList,				*m_pMagicBouquetEmptyList;

	CLibList			*m_pMsgExistList;								// Push, Pop 할 시에 사용하는 리스트	
	CLibList			*m_pMsgEmptyList;								// Push, Pop 할 시에 사용하는 리스트

	CMagicAttribute		*m_pMagicAttribute;

	// 서버 DEBUG용 
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_MAGIC_KIND_NUM>		ServerDebugMagicUse;
#else
	SI32				ServerDebugMagicUse[MAX_MAGIC_KIND_NUM];				
#endif
	DWORD				ServerDebugClock;
};

#endif

