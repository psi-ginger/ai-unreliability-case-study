#include "MagicMoveTimerBombObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"

#include "../../../Client/client.h"
#include "../../../Client/Music/Music.h"
extern cltCommonLogic* pclClient;

CMagicMoveTimerBombObj::CMagicMoveTimerBombObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicMoveTimerBombObj::~CMagicMoveTimerBombObj()
{
	Destroy();
}

void CMagicMoveTimerBombObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage  = 0;

	return;
}

void CMagicMoveTimerBombObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicMoveTimerBombObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	/*
	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(pstMoveTimerBombInfo->sHeader.si32ToID);
	if( pclClient->pclCM->IsValidID(toid) == FALSE)return ;
	*/

	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 FromID	= pclClient->pclCM->GetIDFromCharUnique( pstMagicInfo->sHeader.si32FromID );
	if( pclClient->pclCM->IsValidID( FromID ) == FALSE)
	{
		return false;
	}
	
	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );
	
	stMagicAttributeData *pMagicAttributeData = GetMagicAttributePtr();
	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	if ( IsNPC( charunique ) )
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
	pclClient->pclCM->CR[ FromID ]->GetBottomPoint(&m_clDotHitPos);
	m_pclMap = pclClient->pclCM->CR[ FromID ]->pclMap;
	
	m_si32AddDamage = si32AddDamage;


	m_si32TotalDamage = 0;

	SI32 siMinDamage= 0, siMaxDamage = 0;
	if(pclClient->pclMagicManager->GetMinMaxDamage(pstMagicInfo->sHeader.si32MagicKind, m_si32AddDamage, si32SkillLevel, siMinDamage, siMaxDamage, siOrganUnique)==false)
	{
		return false;
	}
	pclClient->pclMagicManager->CalculateDamage(siMinDamage, siMaxDamage, m_si32TotalDamage);

	return true;
}

// 디레이 주의 할것
bool CMagicMoveTimerBombObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		/*
		
		if ( GetCurrentFrame() == GetMaxFrameNum() || GetCurrentFrame()%GetMagicAttributePtr()->si32MagicRadius == 0 )
		{
			SI32 charunique = m_pMagicMoveTimerBombInfo->sHeader.si32ToID;
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);

			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				if(m_si32TotalDamage==0) return false; 
				SetDamage( m_si32TotalDamage, GetCurrentFrame() / GetMagicAttributePtr()->si32MagicRadius );
			}
		}
		*/
	}
	else												// 클라이언트
	{
		/*
		if ( GetCurrentFrame() == 1 )
		{
			SI32 charunique = m_pMagicMoveTimerBombInfo->sHeader.si32FromID;
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);

			cltClient *pclclient = (cltClient*)pclClient;

			//pclclient->pclMusic->PushEffect( EFFECT_MAGIC_MoveTimerBomb_FIRE, pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
		}
		*/
	}


	return false;
}

void CMagicMoveTimerBombObj::Draw( )
{	
	/*
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	SetMagicSpr( GIMG_MAGIC_MOVETIMERBOMB );				// Image index

	//--------------------------------------
	// 화면 출력용 좌표로 전환한다. 
	//--------------------------------------
	SHORT screenx=0, screeny=0;
	cltMapClient* pclmap = (cltMapClient*)pclMap;
	pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);


	m_si32DrawXPos = screenx - GetMagicSpr()->clHeader.siXsize/2;
	m_si32DrawYPos = screeny - GetMagicSpr()->clHeader.siYsize/2;


	//clGrp.FillBox(screenx-5, screeny-5, screenx+5, screeny+5, COLOR_WHITE);

	GetMagicSpr()->PutSprLight1T( m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() );
	//GetMagicSpr()->PutSprT( m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() + GetMaxFrameNum() );
	//clGrp.PutImage( m_si32DrawXPos, m_si32DrawYPos, GetMagicSpr()->clHeader.siXsize, GetMagicSpr()->clHeader.siYsize, GetMagicSpr()->Image );
	*/

	return;
}


// Push
bool CMagicMoveTimerBombObj::Push( char *pData, SI32 si32Size )
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

void CMagicMoveTimerBombObj::SetDamage( SI32 si32Damage, SI32 si32CurrentRadius )
{
	/*
	stMagicMsgCircleDamage	sMagicMsgCircleDamage;
	
	sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize = sizeof( stMagicMsgCircleDamage );
	sMagicMsgCircleDamage.sMagicMsgHeader.si32MagicKind = GetKind();
	sMagicMsgCircleDamage.sMagicMsgHeader.si32MsgKind = MAGIC_CIRCLE_DAMAGE;
	
	sMagicMsgCircleDamage.si32FromID = m_pMagicMoveTimerBombInfo->sHeader.si32FromID;
	sMagicMsgCircleDamage.si32Damage = si32Damage;
	sMagicMsgCircleDamage.si32CurrentRadius = si32CurrentRadius;

	sMagicMsgCircleDamage.si32Radius = GetMagicAttributePtr()->si32MagicRadius;
	
	Push( (char*) &sMagicMsgCircleDamage, sMagicMsgCircleDamage.sMagicMsgHeader.si16MsgSize );
	*/

	return;
}

