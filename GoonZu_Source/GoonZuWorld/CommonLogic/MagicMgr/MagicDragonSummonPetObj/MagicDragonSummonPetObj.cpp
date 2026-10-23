#include "MagicDragonSummonPetObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"


#include "../../../Client/client.h"
#include "../../../Client/Music/Music.h"
extern cltCommonLogic* pclClient;

MagicDragonSummonPetObj::MagicDragonSummonPetObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

MagicDragonSummonPetObj::~MagicDragonSummonPetObj()
{
	Destroy();
}

void MagicDragonSummonPetObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;

	return;
}

void MagicDragonSummonPetObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool MagicDragonSummonPetObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap,SI32 si32SkillLevel, SI32 siOrganUnique )
{
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	
	SI32 FromID	= pclClient->pclCM->GetIDFromCharUnique( pstMagicInfo->sHeader.si32FromID );
	if ( FromID < 1 ) return false;

	if( pclClient->pclCM->IsValidID( FromID ) == FALSE) return false;

	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );


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

	//--------------------------------
	// 피격 포인트를 구한다. 
	//--------------------------------
	pclClient->pclCM->CR[ FromID ]->GetBottomPoint(&m_clDotHitPos);
	m_pclMap = pclClient->pclCM->CR[ FromID ]->pclMap;

	m_si32AddDamage = si32AddDamage;
	m_si32TotalDamage = 0;

	SI32 siMinDamage = 0, siMaxDamage = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)== false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);


	return true;
}

// 디레이 주의 할것
bool MagicDragonSummonPetObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		if( MAGIC_MON_SUMMONDRAGON_STUN == GetKind() )
		{
			if( GetCurrentMagicDelayFrame() == 0  && (GetCurrentFrame()%GetMagicAttributePtr()->si32MagicRadius) == 0 )
			{

				if(m_si32TotalDamage==0) return false; 
				SetDamage( m_si32TotalDamage, GetCurrentFrame() / GetMagicAttributePtr()->si32MagicRadius );
			}
		}
		else
		{
			if( GetCurrentMagicDelayFrame() == 0 )
			{
				SetDamage( m_si32TotalDamage, 0 );
				return true; 
			} 
		}
	} 
	else
	{
		if( MAGIC_MON_SUMMONDRAGON_STUN == GetKind() )
		{
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DRAGONYONGGARY_MAGIC")), 0, 0 );
		}
		else
		{
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DRAGONBABY_MAGIC")), 0, 0 );
		}
	}

	return false;
}

void MagicDragonSummonPetObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	return;
}


// Push
bool MagicDragonSummonPetObj::Push( BYTE *pData, SI32 si32Size )
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

void MagicDragonSummonPetObj::SetDamage( SI32 si32Damage, SI32 si32CurrentRadius )
{
	if( MAGIC_MON_SUMMONDRAGON_STUN == GetKind() )
	{
		stMagicMsgCircleDamage	sMagicMsgCircleDamage;

		sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize	= sizeof( stMagicMsgCircleDamage );
		sMagicMsgCircleDamage.sMagicMsgHeader.si32MagicKind = GetKind();
		sMagicMsgCircleDamage.sMagicMsgHeader.si32MsgKind	= MAGIC_CIRCLE_DAMAGE;

		sMagicMsgCircleDamage.si32FromID		= m_pMagicInfo->sHeader.si32FromID;
		sMagicMsgCircleDamage.si32ToID			= m_pMagicInfo->sHeader.si32ToID;
		sMagicMsgCircleDamage.si32Damage		= si32Damage;
		sMagicMsgCircleDamage.si32CurrentRadius = si32CurrentRadius;

		sMagicMsgCircleDamage.si32Radius = GetMagicAttributePtr()->si32MagicRadius;

		Push( (BYTE*) &sMagicMsgCircleDamage, sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize );
	}
	else
	{
		stMagicMsgDamage sMagicMsgDamage;
		sMagicMsgDamage.sMagicMsgHeader.si16MsgSize = sizeof( stMagicMsgDamage );
		sMagicMsgDamage.sMagicMsgHeader.si32MagicKind = GetKind();
		sMagicMsgDamage.sMagicMsgHeader.si32MsgKind = MAGIC_DAMAGE;

		sMagicMsgDamage.si32FromID = m_pMagicInfo->sHeader.si32FromID;
		sMagicMsgDamage.si32ToID = m_pMagicInfo->sHeader.si32ToID;

		sMagicMsgDamage.si32Damage = si32Damage;

		Push( (BYTE*) &sMagicMsgDamage, sMagicMsgDamage.sMagicMsgHeader.si16MsgSize );	
	}

	

	return;
}

