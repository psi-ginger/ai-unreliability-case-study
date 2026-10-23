#include "MagicHealObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
#include "../../../Client/Music/Music.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

CMagicHealObj::CMagicHealObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicHealObj::~CMagicHealObj()
{
	Destroy();
}

void CMagicHealObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	
	return;
}

void CMagicHealObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicHealObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )	return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	/*
	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(pstHealInfo->sHeader.si32ToID);
	if( pclClient->pclCM->IsValidID(toid) == FALSE)return ;
	*/

	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 FromID	= pclClient->pclCM->GetIDFromCharUnique( pstMagicInfo->sHeader.si32FromID );
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
	//pclClient->pclCM->CR[toid]->GetBottomPoint(&m_clDotHitPos);
	//pclMap	= pclClient->pclCM->CR[toid]->pclMap;
	pclMap  = NULL; // 받은것을 사용안하나 보다.
	pclClient->pclCM->CR[ FromID ]->GetBottomPoint(&m_clDotHitPos);
	m_pclMap= pclClient->pclCM->CR[ FromID ]->pclMap;
	
	m_si32AddDamage = pstMagicInfo->sHeader.si32Mana;

	m_si32TotalDamage = m_si32AddDamage;
	return true;
}

// 디레이 주의 할것
bool CMagicHealObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		
		if ( ( GetCurrentFrame() == GetMaxFrameNum()/3 && GetCurrentMagicDelayFrame() == 0 )  )
		{
			SI32 charunique = m_pMagicInfo->sHeader.si32ToID;
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);

			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				if(m_si32TotalDamage==0) return false; 

				SI32 needrecover = pclClient->pclCM->CR[id]->clPB.GetMaxLife() - pclClient->pclCM->CR[id]->pclCI->clIP.GetLife();
				SI32 recover = min(m_si32TotalDamage * 4, needrecover);

				// 
				SetDamage( recover );
			}
		}
	}
	else												// 클라이언트
	{
		if ( GetCurrentFrame() == 1 )
		{
			SI32 charunique = m_pMagicInfo->sHeader.si32FromID;
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);

			cltClient *pclclient = (cltClient*)pclClient;

			//pclclient->pclMusic->PushEffect( EFFECT_MAGIC_FLAME_FIRE, pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
		}		
	}


	return false;
}

void CMagicHealObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}


	return;
}


// Push
bool CMagicHealObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicHealObj::SetDamage( SI32 si32Damage)
{
	stMagicMsgDamage sMagicMsgDamage;
	
	sMagicMsgDamage.sMagicMsgHeader.si16MsgSize = sizeof( stMagicMsgDamage );
	sMagicMsgDamage.sMagicMsgHeader.si32MagicKind = GetKind();
	sMagicMsgDamage.sMagicMsgHeader.si32MsgKind = MAGIC_INCREASE_LIFE;

	sMagicMsgDamage.si32FromID = m_pMagicInfo->sHeader.si32FromID;
	sMagicMsgDamage.si32ToID   = m_pMagicInfo->sHeader.si32ToID;
	sMagicMsgDamage.si32Damage = si32Damage;
	
	Push( (BYTE*) &sMagicMsgDamage, sMagicMsgDamage.sMagicMsgHeader.si16MsgSize );	


	return;
}
