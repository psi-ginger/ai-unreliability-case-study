#include "MagicPoisonFlameObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"
#include "../MagicAttribute/MagicAttribute.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
#include "../../../Client/Music/Music.h"
extern cltCommonLogic* pclClient;

CMagicPoisonFlameObj::CMagicPoisonFlameObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicPoisonFlameObj::~CMagicPoisonFlameObj()
{
	Destroy();
}

void CMagicPoisonFlameObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;
	m_si32DrawYIndent = 0;

	return;
}

void CMagicPoisonFlameObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicPoisonFlameObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap,SI32 si32SkillLevel, SI32 siOrganUnique )
{
	/*
	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(pstFlameInfo->sHeader.si32ToID);
	if( pclClient->pclCM->IsValidID(toid) == FALSE)return ;
	*/


	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 FromID	= pclClient->pclCM->GetIDFromCharUnique( pstMagicInfo->sHeader.si32FromID );
	if ( FromID < 1 ) return false;

	if( pclClient->pclCM->IsValidID( FromID ) == FALSE) return false;
	
	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );
	
	stMagicAttributeData *pMagicAttributeData = GetMagicAttributePtr();
	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	if ( IsNPC( charunique )  || IsInstanceNPC(charunique) )
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
	//pclClient->pclCM->CR[toid]->GetBottomPoint(&clDotHitPos);
	//pclMap	= pclClient->pclCM->CR[toid]->pclMap;
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

	
	// CLIENT  용 
	if ( pclClient->GameMode == GAMEMODE_CLIENT)		// CLIENT
	{
		m_si32DrawYIndent = ((cltCharClient *)(pclClient->pclCM->CR[FromID]))->ImageYsize /5;
	}
	return true;
}

// 디레이 주의 할것
bool CMagicPoisonFlameObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		
		if ( ( GetCurrentFrame() == GetMaxFrameNum() && GetCurrentMagicDelayFrame() == 0 ) || GetCurrentFrame()%GetMagicAttributePtr()->si32MagicRadius == 0 )
		{
			SI32 charunique = m_pMagicInfo->sHeader.si32ToID;
			if ( charunique < 1 )
			{
				return false;
			}
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);
			if ( id < 1 )
			{
				return false;
			}

			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{

				if(m_si32TotalDamage==0) return false; 
				SetDamage( m_si32TotalDamage, GetCurrentFrame() / GetMagicAttributePtr()->si32MagicRadius );
			}
		}
	}
	else												// 클라이언트
	{
		if ( GetCurrentFrame() == 1 )
		{
			SI32 charunique = m_pMagicInfo->sHeader.si32FromID;
			if ( charunique < 1 )
			{
				return false;
			}
			SI32 id	= pclClient->pclCM->GetIDFromCharUnique(charunique);
			if( id < 1 )
			{
				return false;
			}
			
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAGIC_FLAME_FIRE")), pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
		}		
	}


	return false;
}

void CMagicPoisonFlameObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}
	CMagicAttribute* pMagicAtt =  pclClient->pclMagicManager->GetMagicAttributePtr() ;
	if ( pMagicAtt == NULL ) return ;
	SI32 group = pMagicAtt->GetMagicGroup(m_pMagicInfo->sHeader.si32MagicKind) ;

	switch ( group )
	{
	case MAGIC_GROUP_TOADPOISON:
		// 피격 이펙트 없음
		break;
	default:
		SetMagicSpr( GIMG_MAGIC_POISONFLAME );				// Image index

		//--------------------------------------
		// 화면 출력용 좌표로 전환한다. 
		//--------------------------------------
		SHORT screenx=0, screeny=0;
		cltMapClient* pclmap = (cltMapClient*)m_pclMap;
		pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

		m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
		m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()/2 - m_si32DrawYIndent;


		GP.PutSprLight1(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() );
		break;

	}
	return;
}


// Push
bool CMagicPoisonFlameObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicPoisonFlameObj::SetDamage( SI32 si32Damage, SI32 si32CurrentRadius )
{
	stMagicMsgCircleDamage	sMagicMsgCircleDamage;
	
	sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize = sizeof( stMagicMsgCircleDamage );
	sMagicMsgCircleDamage.sMagicMsgHeader.si32MagicKind = GetKind();
	sMagicMsgCircleDamage.sMagicMsgHeader.si32MsgKind = MAGIC_CIRCLE_DAMAGE;
	
	sMagicMsgCircleDamage.si32FromID	= m_pMagicInfo->sHeader.si32FromID;
	sMagicMsgCircleDamage.si32ToID		= m_pMagicInfo->sHeader.si32ToID;
	sMagicMsgCircleDamage.si32Damage	= si32Damage;
	sMagicMsgCircleDamage.si32CurrentRadius = si32CurrentRadius;

	sMagicMsgCircleDamage.si32Radius = GetMagicAttributePtr()->si32MagicRadius;
	
	Push( (BYTE*) &sMagicMsgCircleDamage, sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize );

	return;
}

