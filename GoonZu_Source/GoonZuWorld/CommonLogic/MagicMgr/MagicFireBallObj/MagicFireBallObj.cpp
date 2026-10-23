#include "MagicFireBallObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"
#include "../MagicAttribute/MagicAttribute.h"
 
  
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "../../../Common/SMLib/LibList/LibList.h"
#include "../../../Common/Bullet/Bullet.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
extern cltCommonLogic* pclClient;

CMagicFireBallObj::CMagicFireBallObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();
	

	Initialize();
}

CMagicFireBallObj::~CMagicFireBallObj()
{
	Destroy();
}

void CMagicFireBallObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;
	return;
}

void CMagicFireBallObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicFireBallObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{

	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);
	
	// MAGIC INFO를 설정한다.
	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );

	m_pclMap = pclMap;
	m_si32AddDamage = si32AddDamage;

	// MagicAttribute를 얻어와 Magic을 설정한다.
	stMagicAttributeData *pMagicAttributeData = GetMagicAttributePtr();
	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	if ( IsNPC( charunique ) || IsInstanceNPC(charunique))
	{
		CMagicBaseObj::SetMagic( &pstMagicInfo->sHeader, MinSkillLevel );
	}
	else
	{
		CMagicBaseObj::SetMagic( &pstMagicInfo->sHeader, si32SkillLevel );
	}


	m_si32TotalDamage = 0;
	SI32 siMinDamage  = 0, siMaxDamage  = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)==false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);

	return true;
}

// 디레이 주의 할것
bool CMagicFireBallObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	/*
	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		if ( m_si32CurrentFrame == MAX_MAGIC_THUNDER_FRAME_NUM / 2 )
		{
			SI32 charunique = m_pMagicThunderInfo->sHeader.si32ToID;
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);

			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				SetDamage( 10 );
			}
		}
	}
	else												// 클라이언트
	{
	}
	*/

	//--------------------------------------------------------
	//--------------------- 출발하는 놈 꺼
	// FromID 얻기 위해서.
	SI32 charunique = m_pMagicInfo->sHeader.si32FromID;
	if ( charunique < 1 )
		return false;
	SI32 Fromid			= pclClient->pclCM->GetIDFromCharUnique(charunique);
	if ( Fromid < 1 )
		return false;

	if(pclClient->pclCM->IsValidID(Fromid) == false)
		return false;


	// FromID 타일 좌표 구하기
	SI32 XPos = pclClient->pclCM->CR[Fromid]->GetX();		// 캐릭터 X 타일 좌표
	SI32 YPos = pclClient->pclCM->CR[Fromid]->GetY();		// 캐릭터 Y 타일 좌표

	cltPos clTileStartPos( XPos, YPos );

	// Dot 단위의 출발 좌표
	SI32 StartDotXPos, StartDotYPos;
	pclClient->pclCM->CR[ Fromid ]->DecideAttackPoint( 0, StartDotXPos, StartDotYPos );
	cltPos	clDotStartPos( StartDotXPos, StartDotYPos );
	//---------------------------------------------------------


	//---------------------------------------------------------
	//------------------------ 타겟이 되는 놈꺼
	//
	charunique = m_pMagicInfo->sHeader.si32ToID;
	if ( charunique < 1 )
		return false;
	SI32 Toid  = pclClient->pclCM->GetIDFromCharUnique(charunique);
	if ( Toid < 1 )
		return false;
	
	if(pclClient->pclCM->IsValidID(Toid) == false)
		return false;

	XPos = pclClient->pclCM->CR[Toid]->GetX();		// 캐릭터 X 타일 좌표
	YPos = pclClient->pclCM->CR[Toid]->GetY();		// 캐릭터 Y 타일 좌표

	cltCharPos		clPos;
	clPos.Set(&pclClient->pclCM->CR[ Toid ]->pclCI->clBI.clPos);		// 캐릭터 정보 Fromid 를 쓴이유는 맞는 놈이 때리는 놈의 마법에 영향을 받아야 하는데, 중간에 다른 맵으로 이동하는 경우가 생길 수 있으므로...

	cltAttackTarget clTarget;
	// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
	//if( pclClient->IsWhereServiceArea(ConstSwitch_PVP) )
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SI32 hitRate = pclClient->pclCM->CR[Fromid]->clPB.GetHitRate( pclClient->pclCM->CR[Toid], pclClient->pclCM->CR[Fromid], true );
		SI32 dodgeRate = pclClient->pclCM->CR[Toid]->clPB.GetDodgeRate( pclClient->pclCM->CR[Fromid], pclClient->pclCM->CR[Toid], true );

		clTarget.Set( Toid, &clPos, hitRate ,dodgeRate );		// 목표에 대한 정보	
	}
	else
	{
		if(pclClient->siEnablePVP == 1)
		{
			if( IsPC(Fromid) )
			{
				// 사람이면 민첩에 영향받게 한다.
				SI32 hitRate = pclClient->pclCM->CR[Fromid]->clPB.GetHitRate( pclClient->pclCM->CR[Toid], pclClient->pclCM->CR[Fromid], true );
				clTarget.Set( Toid, &clPos, hitRate,0 );		// 목표에 대한 정보	
			}
			else
				clTarget.Set( Toid, &clPos, 100,0 );		// 목표에 대한 정보

		}
		else
			clTarget.Set( Toid, &clPos, 100,0 );		// 목표에 대한 정보
	}


	// Dot 단위의 출발 좌표
	SI32 TargetDotXPos, TargetDotYPos;
	pclClient->pclCM->CR[ Toid ]->DecideAttackPoint( 0, TargetDotXPos, TargetDotYPos );
	cltPos	clDotTargetPos( TargetDotXPos, TargetDotYPos );
	//---------------------------------------------------------	

	//---------------------------------------------------------	
	// 총알 객체처럼 날리기
	//if(m_si32TotalDamage==0) return false; 

	CMagicAttribute* pMagicAtt =  pclClient->pclMagicManager->GetMagicAttributePtr() ;
	if ( pMagicAtt == NULL ) return false ;

	SI32 group = pMagicAtt->GetMagicGroup(m_pMagicInfo->sHeader.si32MagicKind) ;

	switch(group)
	{
	case MAGIC_GROUP_FIREBALL:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MAGICFIREBALL, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SKELLGHOST:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MAGICSKELLGHOST, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break ;
	case MAGIC_GROUP_TWINSPEAR:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_TWINSPEAR, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_FIRESTORM:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_FIRESTORM, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SPEAR_DRAGON:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_SPEAR_DRAGON, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_GARGOYLE:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_GARGOYLE, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_CERBERUS:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_CERBERUS, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_MARIONETTE:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MARIONETTE, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;

	//case MAGIC_GROUP_FROZENSORCERER:
	//	{
	//		pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_FROZENSORCERER_MAGIC, GetKind()
	//			, Fromid
	//			, m_si32TotalDamage		// Damage
	//			, &clTileStartPos		// 출발 Tile Pos
	//			, &clDotStartPos		// 출발 Dot Pos
	//			, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
	//			, &clDotTargetPos		// 맞는 놈의 dot pos
	//			);
	//	}
	case MAGIC_GROUP_FROKEN:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_FROKEN, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_ICEHOUND:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_ICEHOUND, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_FROZENKNIGHT:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_FROZENKNIGHT, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_NINJAPANDA:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_NINJAPANDA, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_RACCOONDOG:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_RACCOONDOG, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_BOSSMONKEY:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_BOSSMONKEY, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_BLOODBAT:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_BLOODBAT, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_WEREWOLF:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_WEREWOLF, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_DEVILOUS:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_DEVILOUS, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_GANGSI:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_GANGSI, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_GIRAFFE:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_GIRAFFE, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_PAPAGOBLIN:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_PAPAGOBLIN, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_PIRATESOLDIER:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_PIRATESOLDIER, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_PIRATEARCHER:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_PIRATEARCHER_MAGIC, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_BLACKBOSS:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_BLACKBOSS, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_EMPERORCAVALRY:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_EMPERORCAVALRY, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_EMPERORWARRIOR:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_EMPERORWARRIOR, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_GREATWALLTWINHEAD:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_GREATWALLTWINHEAD, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;

	case MAGIC_GROUP_BEARTANG:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_BEARTANG_MAGIC, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SUPERBEARTANG:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_SUPERBEARTANG_MAGIC, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_TRIPLEDEATH:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_TRIPLEDEATH_MAGIC, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_COSMOSRABBIT:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_COSMOSRABBIT, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_RUDOLPHPET:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_RUDOLPHPET, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;

		// 엠버사우루스
	case MAGIC_GROUP_AMBERSAURUS:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_AMBERSAURUS, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;

		// 롤링스테고
	case MAGIC_GROUP_ROLLINGSTEGO:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_ROLLINGSTEGO, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
		// 소울 가드 펌킨이 사용 하는 마법
	case MAGIC_GROUP_PUMPKIN:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MAGICSKELLGHOST, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	//[진성] 드래곤
	case MAGIC_GROUP_DRAGONFIRE:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_FIREBALL, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	// [춘기] 안개미궁 - 어릿광대
	case MAGIC_GROUP_CLOWNBALL:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_CLOWN_MAGIC, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
		// [춘기] 안개미궁 - 안개신부
	case MAGIC_GROUP_BOUQUET:
		{
			pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_BOUQUET, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;

	default: return false ;
	}

	return true;			// 총알 객체에 실어서 날리면 끝나므로..
}

void CMagicFireBallObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}
	return;
}


// Push
bool CMagicFireBallObj::Push( BYTE *pData, SI32 si32Size )
{
	if ( CMagicBaseObj::Push( pData, si32Size ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}