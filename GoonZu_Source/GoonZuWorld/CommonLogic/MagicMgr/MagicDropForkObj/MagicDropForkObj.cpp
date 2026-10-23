#include "MagicDropForkObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
#include "../../../Common/Smoke/Smoke.h"

extern cltCommonLogic* pclClient;

CMagicDropForkObj::CMagicDropForkObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicDropForkObj::~CMagicDropForkObj()
{
	Destroy();
}

void CMagicDropForkObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;

	return;
}

void CMagicDropForkObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicDropForkObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(pstMagicInfo->sHeader.si32ToID);
	if(pclClient->pclCM->IsValidID(toid) == FALSE)
	{
		return false;
	}

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


	m_si32AddDamage = si32AddDamage;
	m_si32TotalDamage = 0;

	SI32 siMinDamage= 0, siMaxDamage = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)==false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);

	//--------------------------------
	// 피격 포인트를 구한다. 
	//--------------------------------
	pclClient->pclCM->CR[toid]->GetBottomPoint(&m_clDotHitPos);
	m_pclMap	= pclClient->pclCM->CR[toid]->pclMap;
	


	return true;
}

// 디레이 주의 할것
bool CMagicDropForkObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	


	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		if ( GetCurrentFrame() == (GetMaxFrameNum() - 2) && GetCurrentMagicDelayFrame() == 0 )		
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
				SetDamage( m_si32TotalDamage );
			}
		}
	}
	else												// 클라이언트
	{
		SI32 charunique = m_pMagicInfo->sHeader.si32ToID;
		if ( charunique <1 )
		{
			return false;
		}
		SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);
		if ( id < 1 )
		{
			return false;
		}
		
		if ( GetCurrentFrame() == 1 )
		{
			if(pclClient->pclCM->IsAlive(id) == TRUE)			
			{
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GOLDBEETLE_STONE")), pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
			}
		}		
	}


	return false;
}

void CMagicDropForkObj::Draw( )
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

	//--------------------------------------
	// FRONT
	//--------------------------------------
	SetMagicSpr( GIMG_MAGIC_DROPFORK_FRONT);				// Image index
	m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
	m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()*2/3;

	GP.PutSpr(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() );


	//--------------------------------------
	// BACK
	//--------------------------------------
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_DROPFORK_BACK);
	if(pspr)
	{
		m_si32DrawXPos = screenx - pspr->GetXSize()/2;
		m_si32DrawYPos = screeny - pspr->GetYSize()*2/3;
		GP.PutSprScreen(pspr, m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() );
	}

	return;
}


// Push
bool CMagicDropForkObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicDropForkObj::SetDamage( SI32 si32Damage )
{
	stMagicMsgWhirlPoolDamage	sMagicMsgWhirlPoolDamage;
	
	sMagicMsgWhirlPoolDamage.sMagicMsgHeader.si16MsgSize	= sizeof( stMagicMsgWhirlPoolDamage );
	sMagicMsgWhirlPoolDamage.sMagicMsgHeader.si32MagicKind	= GetKind();
	sMagicMsgWhirlPoolDamage.sMagicMsgHeader.si32MsgKind	= MAGIC_WHIRLPOOL_DAMAGE;

	m_pclMap->GetRealTileByRealDot( m_clDotHitPos.siX, m_clDotHitPos.siY, &sMagicMsgWhirlPoolDamage.si32XCenterPos, &sMagicMsgWhirlPoolDamage.si32YCenterPos);

	sMagicMsgWhirlPoolDamage.si32FromID = m_pMagicInfo->sHeader.si32FromID;
	sMagicMsgWhirlPoolDamage.si32ToID   = m_pMagicInfo->sHeader.si32ToID;
	sMagicMsgWhirlPoolDamage.si32Damage = si32Damage;

	sMagicMsgWhirlPoolDamage.si32Radius = GetMagicAttributePtr()->si32MagicRadius;

	
	Push( (BYTE*) &sMagicMsgWhirlPoolDamage, sMagicMsgWhirlPoolDamage.sMagicMsgHeader.si16MsgSize );

	return;
}

