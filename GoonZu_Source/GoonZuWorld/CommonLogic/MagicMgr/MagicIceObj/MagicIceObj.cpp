#include "MagicIceObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "../../../Common/SMLib/LibList/LibList.h"
#include "../../../Common/Bullet/Bullet.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
extern cltCommonLogic* pclClient;

CMagicIceObj::CMagicIceObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicIceObj::~CMagicIceObj()
{
	Destroy();
}

void CMagicIceObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage  = 0;


	return;
}

void CMagicIceObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicIceObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{

	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )	return false;
	
	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );
	m_pclMap = pclMap;
	m_si32AddDamage = si32AddDamage;

	stMagicAttributeData *pMagicAttributeData = GetMagicAttributePtr();
	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	if ( IsNPC( charunique ) || IsInstanceNPC(charunique) )
	{
		CMagicBaseObj::SetMagic( &pstMagicInfo->sHeader, MinSkillLevel );
	}
	else
	{
		CMagicBaseObj::SetMagic( &pstMagicInfo->sHeader, si32SkillLevel );
	}


	m_si32TotalDamage = 0;

	SI32 siMinDamage= 0, siMaxDamage = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)==false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);

	return true;
}

// 디레이 주의 할것
bool CMagicIceObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

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
	clPos.Set(&pclClient->pclCM->CR[ Fromid ]->pclCI->clBI.clPos);		// 캐릭터 정보 Fromid 를 쓴이유는 맞는 놈이 때리는 놈의 마법에 영향을 받아야 하는데, 중간에 다른 맵으로 이동하는 경우가 생길 수 있으므로...

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

	if ( GetMagicAttributePtr()->si32MagicRadius == 0 )
	{
		pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MAGICICE, GetKind()
												, Fromid
												, m_si32TotalDamage		// Damage
												, &clTileStartPos		// 출발 Tile Pos
												, &clDotStartPos		// 출발 Dot Pos
												, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
												, &clDotTargetPos		// 맞는 놈의 dot pos
												);
	}
	else if ( GetMagicAttributePtr()->si32MagicRadius == 1 )
	{
		pclClient->pclBulletManager->SetBullet( m_pclMap, BULLETKIND_MAGICFROZEN, GetKind()
												, Fromid
												, m_si32TotalDamage		// Damage
												, &clTileStartPos		// 출발 Tile Pos
												, &clDotStartPos		// 출발 Dot Pos
												, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
												, &clDotTargetPos		// 맞는 놈의 dot pos
												);
	}

	return true;			// 총알 객체에 실어서 날리면 끝나므로..
}

void CMagicIceObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	return;
}


// Push
bool CMagicIceObj::Push( BYTE *pData, SI32 si32Size )
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


