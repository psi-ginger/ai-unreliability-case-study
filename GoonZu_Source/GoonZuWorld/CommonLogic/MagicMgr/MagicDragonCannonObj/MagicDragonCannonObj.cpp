#include "MagicDragonCannonObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"

#include "../../../Client/client.h"

#include "../../../Common/Smoke/Smoke.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CMagicDragonCannonObj::CMagicDragonCannonObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicDragonCannonObj::~CMagicDragonCannonObj()
{
	Destroy();
}

void CMagicDragonCannonObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;

//	                            0
//						  1     2     3
//					4     5     6     7     8 
//					      9     10    11
//						        12
#define CANNONPOINTWITH   80
#define CANNONPOINTHEIGHT 50

	m_siCannonPointX[0] = 0;					m_siCannonPointY[0] = -CANNONPOINTHEIGHT*2;	

	m_siCannonPointX[1] =-CANNONPOINTWITH;		m_siCannonPointY[1] = -CANNONPOINTHEIGHT;		
	m_siCannonPointX[2] = 0;					m_siCannonPointY[2] = -CANNONPOINTHEIGHT;
	m_siCannonPointX[3] = CANNONPOINTWITH;		m_siCannonPointY[3] = -CANNONPOINTHEIGHT;

	m_siCannonPointX[4] =-CANNONPOINTWITH*2;	m_siCannonPointY[4] = 0;	
	m_siCannonPointX[5] =-CANNONPOINTWITH;		m_siCannonPointY[5] = 0;
	m_siCannonPointX[6] = 0;					m_siCannonPointY[6] = 0;	
	m_siCannonPointX[7] = CANNONPOINTWITH;		m_siCannonPointY[7] = 0;	
	m_siCannonPointX[8] = CANNONPOINTWITH*2;	m_siCannonPointY[8] = 0;

	m_siCannonPointX[9] =-CANNONPOINTWITH;		m_siCannonPointY[9] = CANNONPOINTHEIGHT;	
	m_siCannonPointX[10]= 0;					m_siCannonPointY[10]= CANNONPOINTHEIGHT;	
	m_siCannonPointX[11]= CANNONPOINTWITH;		m_siCannonPointY[11]= CANNONPOINTHEIGHT;

	m_siCannonPointX[12]= 0;					m_siCannonPointY[12]= CANNONPOINTHEIGHT*2;
	return;
}

void CMagicDragonCannonObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicDragonCannonObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
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

	if ( IsNPC( charunique ) || IsInstanceNPC(charunique) )
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
	
#ifdef _SAFE_MEMORY
	m_siDrawFrame.ZeroMem();
#else
	memset(m_siDrawFrame, 0, sizeof(m_siDrawFrame));
#endif



	return true;
}

// 디레이 주의 할것
bool CMagicDragonCannonObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	


	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		if ( GetCurrentFrame() == (GetMaxFrameNum()-1) && GetCurrentMagicDelayFrame() == 0 )
		{
			SI32 charunique = m_pMagicInfo->sHeader.si32ToID;
			if ( charunique < 1 )	{	return false;	}
			
			SI32 id			= pclClient->pclCM->GetIDFromCharUnique(charunique);
			if ( id < 1 )			{	return false;	}

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

		if ( GetCurrentFrame() == 3 )
		{
			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				if(GetMagicAttributePtr()->si32MagicRadius>1)
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAGIC_METEOR_MULTI_FIRE")), pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
				}
				else
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAGIC_METEOR_FIRE")), pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
				}
			}
		}
	}


	return false;
}

void CMagicDragonCannonObj::Draw( )
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

	if ( GetMagicAttributePtr()->si32MagicRadius == 4 )
	{
		SetMagicSpr( GIMG_SWORDMETEOR );				// Image index
	}
	else
	{
		SetMagicSpr( GIMG_MAGIC_DRAGONCANNON );				// Image index
	}

	if(GetCurrentFrame() == (GetMaxFrameNum()/2) && GetCurrentMagicDelayFrame() == 0)
	{
		if(GetMagicAttributePtr()->si32MagicRadius>5)
		{
			for(SI32 i = 0; i < DRAGONCANNON_BULLET_NUM; ++i)
			{
				m_siDrawFrame[i] = rand()%(DRAGONCANNON_BULLET_NUM);
			}
		}
		else if(GetMagicAttributePtr()->si32MagicRadius == 4)
		{
			for(SI32 i = 0; i < 4; ++i)
			{
				m_siDrawFrame[i] = rand()%(4);
			}
		}
		else if(GetMagicAttributePtr()->si32MagicRadius == 3)
		{
			for(SI32 i = 0; i < 3; ++i)
			{
				m_siDrawFrame[i] = rand()%(3);
			}
		}
	}

	if(GetCurrentFrame() >= (GetMaxFrameNum()/2))
	{
		for(SI32 i = 0; i < DRAGONCANNON_BULLET_NUM; ++i)
		{
			++m_siDrawFrame[i];
		}
	}
	
	SI32 siRealDotX = 0;
	SI32 siRealDotY = 0;
	SI32 siTileX, siTileY;
	
	if(GetMagicAttributePtr()->si32MagicRadius>5)
	{
		for(SI32 i = 0; i < DRAGONCANNON_BULLET_NUM; ++i)
		{
			siRealDotX = m_clDotHitPos.siX + m_siCannonPointX[i];
			siRealDotY = m_clDotHitPos.siY + m_siCannonPointY[i];
			m_pclMap->GetRealTileByRealDot( siRealDotX, siRealDotY, &siTileX, &siTileY);

			DrawDragonCannon(siRealDotX, siRealDotY, m_siDrawFrame[i]);
		}
	}
	else if(GetMagicAttributePtr()->si32MagicRadius == 4)
	{
		SI32 siTri[4] = { 2,5, 9, 11};

		for(SI32 i = 0; i < 4; ++i)
		{
			siRealDotX = m_clDotHitPos.siX + m_siCannonPointX[siTri[i]];
			siRealDotY = m_clDotHitPos.siY + m_siCannonPointY[siTri[i]];
			m_pclMap->GetRealTileByRealDot( siRealDotX, siRealDotY, &siTileX, &siTileY);

			DrawDragonCannon(siRealDotX, siRealDotY, m_siDrawFrame[i]);
		}
	}
	else if ( 3 == GetMagicAttributePtr()->si32MagicRadius )
	{
		SI32 siTri[3] = { 2, 9, 11};
		
		for(SI32 i = 0; i < 3; ++i)
		{
			siRealDotX = m_clDotHitPos.siX + m_siCannonPointX[siTri[i]];
			siRealDotY = m_clDotHitPos.siY + m_siCannonPointY[siTri[i]];
			m_pclMap->GetRealTileByRealDot( siRealDotX, siRealDotY, &siTileX, &siTileY);

			DrawDragonCannon(siRealDotX, siRealDotY, m_siDrawFrame[i]);
		}
	}
	else
	{
		SI32 i = 6; 
		siRealDotX = m_clDotHitPos.siX + m_siCannonPointX[i];
		siRealDotY = m_clDotHitPos.siY + m_siCannonPointY[i];
		m_pclMap->GetRealTileByRealDot( siRealDotX, siRealDotY, &siTileX, &siTileY);

		DrawDragonCannon(siRealDotX, siRealDotY, m_siDrawFrame[i]);

	}

	return;
}


// Push
bool CMagicDragonCannonObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicDragonCannonObj::SetDamage( SI32 si32Damage )
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


void CMagicDragonCannonObj::DrawDragonCannon(SI32 siRealX, SI32 siRealY, SI32 DrawFrame)
{

	cltMapClient* pclmap = (cltMapClient*)m_pclMap;
	SHORT siScreenX = 0, siScreenY= 0;
	pclmap->TransFromRealDotToScreenDot(siRealX, siRealY, siScreenX, siScreenY);

	SI32 siTatalFrame = GetMagicAttributePtr()->si32MagicNextFrameDelay * GetMaxFrameNum() /2;
	if(DrawFrame>siTatalFrame) return;

	//--------------------------------------
	// SHADOW
	//--------------------------------------
	TSpr* pspr	= pclClient->GetGlobalSpr(GIMG_MAGIC_DRAGONSHADOW);

	if(pspr)
	{
		SI32 siDrawX, siDrawY;
		siDrawX = siScreenX - pspr->GetXSize() /2;
		siDrawY = siScreenY - pspr->GetYSize() /2;
		GP.PutSprMultiply(pspr, siDrawX, siDrawY, 0);
	}

	if(DrawFrame>0)
	{
		m_si32DrawXPos = siScreenX - GetMagicSpr()->GetXSize() /2;
		m_si32DrawYPos = siScreenY - GetMagicSpr()->GetYSize() - (700 - DrawFrame *700/siTatalFrame);
		SI32 siMaxFont = GetMagicSpr()->GetImageNum() ;
		
		
		// MAGIC 을 만든다.
		if ( GetMagicAttributePtr()->si32MagicRadius == 4 )
		{
			if(siMaxFont > 0)	GP.PutSpr(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos,	DrawFrame%siMaxFont);
		}
		else
		{
			if(siMaxFont > 0)	GP.PutSprAdd(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos,	DrawFrame%siMaxFont);
		}
		
		// TAIL
		{
			SI32 siDrawX, siDrawY;
			SHORT siTailRealX, siTailRealY;
			siDrawX = siScreenX;
			siDrawY = m_si32DrawYPos + GetMagicSpr()->GetYSize()*2/3;
			pclmap->TransFromScreenToRealXY(siDrawX, siDrawY, siTailRealX, siTailRealY);
			if ( GetMagicAttributePtr()->si32MagicRadius == 4 )
			{
				pclClient->pclSmokeManager->SetSmoke(SMOKE_SWORDMETEOR_TAIL,siTailRealX,siTailRealY,pclmap);
			}
			else
			{
				pclClient->pclSmokeManager->SetSmoke(SMOKE_DRAGONEXPTAIL, siTailRealX, siTailRealY, pclmap);
			}
		}
		
		// EXPLOSION 을 만든다.
		if(DrawFrame == siTatalFrame)
		{
			if ( GetMagicAttributePtr()->si32MagicRadius == 4 )
			{
				pclClient->pclSmokeManager->SetSmoke(SMOKE_SWORDMETEOR_EXPLOSION,siRealX+10,siRealY,pclmap);
				pclClient->pclSmokeManager->SetSmoke(SMOKE_SWORDMETEOR_HIT,siRealX,siRealY,pclmap);
			}
			else
			{
				pclClient->pclSmokeManager->SetSmoke(SMOKE_DRAGONEXPLOSION, siRealX, siRealY, pclmap);
			}
		}
	}

}