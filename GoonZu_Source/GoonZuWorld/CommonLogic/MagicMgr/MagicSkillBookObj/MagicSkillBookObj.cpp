
#include "MagicSkillBookObj.h"

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
CMagicSkillBookObj::CMagicSkillBookObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();


	Initialize();
}

CMagicSkillBookObj::~CMagicSkillBookObj()
{
	Destroy();
}

void CMagicSkillBookObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;
	return;
}

void CMagicSkillBookObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicSkillBookObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind( pstMagicInfo->sHeader.si32MagicKind );

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


	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 ToID	= pclClient->pclCM->GetIDFromCharUnique( pstMagicInfo->sHeader.si32ToID );
	if ( ToID < 1 ) return false;
	if( pclClient->pclCM->IsValidID( ToID ) == FALSE) return false;

	//--------------------------------
	// 피격 포인트를 구한다. 
	//--------------------------------
	pclClient->pclCM->CR[ ToID ]->GetBottomPoint(&m_clDotHitPos);
	m_pclMap = pclClient->pclCM->CR[ ToID ]->pclMap;



	m_si32TotalDamage = 0;
	SI32 siMinDamage  = 0, siMaxDamage  = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)==false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);

	switch( pstMagicInfo->sHeader.si32MagicKind )
	{
	case MAGIC_SKILL_BOOK_ICERAIN:	SetMagicSpr(GIMG_MAGIC_SKILL_BOOK_ICERAIN);	break;
	case MAGIC_SKILL_BOOK_CRASH:	SetMagicSpr(GIMG_MAGIC_SKILL_BOOK_CRASH);	break;
	case MAGIC_SKILL_BOOK_TORNADO:	SetMagicSpr(GIMG_MAGIC_SKILL_BOOK_TORNADO);	break;
	}	

	return true;
}

// 디레이 주의 할것
bool CMagicSkillBookObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	//--------------------------------------------------------
	//--------------------- 출발하는 놈 꺼
	// FromID 얻기 위해서.
	SI32 charunique = m_pMagicInfo->sHeader.si32FromID;
	if ( charunique < 1 )	return false;

	SI32 Fromid	= pclClient->pclCM->GetIDFromCharUnique(charunique);
	if ( Fromid < 1 )		return false;

	if(pclClient->pclCM->IsValidID(Fromid) == false)	return false;

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
	if ( charunique < 1 )	return false;
	SI32 Toid = pclClient->pclCM->GetIDFromCharUnique(charunique);
	if ( Toid < 1 )			return false;

	if(pclClient->pclCM->IsValidID(Toid) == false)	return false;

	XPos = pclClient->pclCM->CR[Toid]->GetX();		// 캐릭터 X 타일 좌표
	YPos = pclClient->pclCM->CR[Toid]->GetY();		// 캐릭터 Y 타일 좌표

	cltCharPos		clPos;
	clPos.Set(&pclClient->pclCM->CR[ Toid ]->pclCI->clBI.clPos);		// 캐릭터 정보 Fromid 를 쓴이유는 맞는 놈이 때리는 놈의 마법에 영향을 받아야 하는데, 중간에 다른 맵으로 이동하는 경우가 생길 수 있으므로...

	cltAttackTarget clTarget;
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SI32 hitRate	= pclClient->pclCM->CR[Fromid]->clPB.GetHitRate( pclClient->pclCM->CR[Toid], pclClient->pclCM->CR[Fromid], true );
		SI32 dodgeRate	= pclClient->pclCM->CR[Toid]->clPB.GetDodgeRate( pclClient->pclCM->CR[Fromid], pclClient->pclCM->CR[Toid], true );

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
	case MAGIC_GROUP_SKILL_BOOK_ICERAIN:	
		{
			if ( pclClient->GameMode != GAMEMODE_SERVER ) return false;	//여기서 false가 넘어가면 밑에 Draw() 함수가 호출된다
			
			SetDamage( m_si32TotalDamage );
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_CRASH:
		{
			if ( pclClient->GameMode != GAMEMODE_SERVER ) return false;

			SetDamage( m_si32TotalDamage );
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_TORNADO:
		{
			if ( pclClient->GameMode != GAMEMODE_SERVER ) return false;

			SetDamage( m_si32TotalDamage );
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_FLAMEBUSTER:
		{
			pclClient->pclBulletManager->SetBullet(	  m_pclMap, BULLETKIND_FLAMEBUSTER, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_FLAMEWHIRL:
		{
			pclClient->pclBulletManager->SetBullet(	  m_pclMap, BULLETKIND_FLAMEWHIRL, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_WETFOG:
		{
			pclClient->pclBulletManager->SetBullet(	  m_pclMap, BULLETKIND_WETFOG, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_SLING:
		{
			pclClient->pclBulletManager->SetBullet(	  m_pclMap, BULLETKIND_SLING, GetKind()
				, Fromid
				, m_si32TotalDamage		// Damage
				, &clTileStartPos		// 출발 Tile Pos
				, &clDotStartPos		// 출발 Dot Pos
				, &clTarget				// 맞는 놈의 캐릭터와 타일 좌표
				, &clDotTargetPos		// 맞는 놈의 dot pos
				);
		}
		break;
	case MAGIC_GROUP_SKILL_BOOK_WINDCUTER:
		{
			pclClient->pclBulletManager->SetBullet(	  m_pclMap, BULLETKIND_WINDCUTER, GetKind()
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

	return true;			// 총알 객체에 실어서 날리면 끝나므로..  여기서 true가 넘어가면 밑에 Draw() 함수가 호출되지 않는다.
}

void CMagicSkillBookObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	//--------------------------------------
	// 화면 출력용 좌표로 전환한다. 
	//--------------------------------------
	SHORT screenx=0, screeny=0;
	cltMapClient* pclmap = (cltMapClient*)m_pclMap;
	pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);


	m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
	m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()/2 - 60; 


	GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() ); 
	return;
}


// Push
bool CMagicSkillBookObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicSkillBookObj::SetDamage( SI32 si32Damage )
{
	stMagicMsgDamage sMagicMsgDamage;

	sMagicMsgDamage.sMagicMsgHeader.si16MsgSize		= sizeof( stMagicMsgDamage );
	sMagicMsgDamage.sMagicMsgHeader.si32MagicKind	= GetKind();
	sMagicMsgDamage.sMagicMsgHeader.si32MsgKind		= MAGIC_DAMAGE;

	sMagicMsgDamage.si32FromID = m_pMagicInfo->sHeader.si32FromID;
	sMagicMsgDamage.si32ToID   = m_pMagicInfo->sHeader.si32ToID;

	sMagicMsgDamage.si32Damage = si32Damage;

	Push( (BYTE*) &sMagicMsgDamage, sMagicMsgDamage.sMagicMsgHeader.si16MsgSize );	

	return;
}