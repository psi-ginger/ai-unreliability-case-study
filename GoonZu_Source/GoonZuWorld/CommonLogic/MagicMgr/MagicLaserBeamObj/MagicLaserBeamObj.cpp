#include "MagicLaserBeamObj.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"
#include "../../../Common/Smoke/Smoke.h"

extern cltCommonLogic* pclClient;

CMagicLaserBeamObj::CMagicLaserBeamObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicLaserBeamObj::~CMagicLaserBeamObj()
{
	Destroy();
}

void CMagicLaserBeamObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_AddXPos		= 0;
	m_AddYPos		= 0;

	m_si32DrawSXPos	= 0;
	m_si32DrawSYPos	= 0;
	m_si32DrawTXPos	= 0;
	m_si32DrawTYPos	= 0;

	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;

	return;
}

void CMagicLaserBeamObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicLaserBeamObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
{
	
	SI32 charunique = pstMagicInfo->sHeader.si32FromID;
	if ( charunique <1 )		return false;

	// KIND를 설정한다.
	SetKind(pstMagicInfo->sHeader.si32MagicKind);

	//---------------------------------
	// 맞는 사람의 아이디를 구한다. 
	//---------------------------------
	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(pstMagicInfo->sHeader.si32ToID);
	SI32 Fromid	= pclClient->pclCM->GetIDFromCharUnique(pstMagicInfo->sHeader.si32FromID);
	if(pclClient->pclCM->IsValidID(toid) == FALSE || pclClient->pclCM->IsValidID(Fromid) == FALSE)
	{
		return false;
	}

	memcpy( m_pMagicInfo, pstMagicInfo, sizeof( stMagicInfo ) );

	stMagicAttributeData *pMagicAttributeData = GetMagicAttributePtr();
	SI32 MinSkillLevel = pMagicAttributeData->si16MagicSkillLevel;

	if ( IsNPC( charunique )|| IsInstanceNPC(charunique) )
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

	pclClient->pclCM->CR[Fromid]->DecideAttackPoint( 0, m_si32DrawSXPos, m_si32DrawSYPos );
	pclClient->pclCM->CR[toid]->DecideAttackPoint( 0, m_si32DrawTXPos, m_si32DrawTYPos );

	m_AddXPos= (m_si32DrawTXPos - m_si32DrawSXPos) / 10;
	m_AddYPos= (m_si32DrawTYPos - m_si32DrawSYPos) / 10;

	return true;
}

// 디레이 주의 할것
bool CMagicLaserBeamObj::Action()
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

			// 적이 처음과 같은 map에 있고 살아있으면 Damage
			if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				if ( pclClient->pclCM->CR[id]->pclMap == m_pclMap )
				{
					if(m_si32TotalDamage==0) return false; 
					SetDamage( m_si32TotalDamage );
				}
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
				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MAGIC_LASERBEAM_FIRE")), pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
			}
		}		
	}


	return false;
}

void CMagicLaserBeamObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	SetMagicSpr( GIMG_MAGIC_BLACKINK);				// Image index


	//--------------------------------------
	// 화면 출력용 좌표로 전환한다. 
	//--------------------------------------

	SHORT screensx=0, screensy=0;
	SHORT screentx=0, screenty=0;
	cltMapClient* pclmap = (cltMapClient*)m_pclMap;

	SI32 toid	= pclClient->pclCM->GetIDFromCharUnique(m_pMagicInfo->sHeader.si32ToID);
	SI32 Fromid	= pclClient->pclCM->GetIDFromCharUnique(m_pMagicInfo->sHeader.si32FromID);
	if(pclClient->pclCM->IsValidID(toid) == FALSE || pclClient->pclCM->IsValidID(Fromid) == FALSE)
	{
		return;
	}
	
	// 적이 처음과 같은 map에 있고 살아있으면 Damage
	if ( pclClient->pclCM->CR[toid]->pclMap == m_pclMap )
	{
		//pclClient->pclCM->CR[Fromid]->DecideAttackPoint( 0, m_si32DrawSXPos, m_si32DrawSYPos );
		//pclClient->pclCM->CR[toid]->DecideAttackPoint( 0, m_si32DrawTXPos, m_si32DrawTYPos );
		m_si32DrawSXPos+=m_AddXPos;
		m_si32DrawSYPos+=m_AddYPos;

		pclmap->TransFromRealDotToScreenDot(m_si32DrawSXPos, m_si32DrawSYPos, screensx, screensy);
		pclmap->TransFromRealDotToScreenDot(m_si32DrawTXPos, m_si32DrawTYPos, screentx, screenty);
		
		SI32 colorTable[33] = 
		{ 
			118,118,
			140,140,
			46, 46,
			181,181,
			152,152,152,
			18, 18, 18,
			255,255,255,255,255,
			18, 18, 18,
			152,152,152,
			181,181,
			46, 46,
			140,140,
			118,118
		};
		// 1, 2 사분면  뒤집혀 있음 
		if((screenty - screensy) < 0)
		{
			double Length = TSQRT((screenty - screensy)*(screenty - screensy)+(screentx - screensx)*(screentx - screensx));
			double fRad   = 2*3.1415926535897932- acos((screentx - screensx)/Length);
			
			SI32 addX = 0;
			SI32 addY = 0;
			for(SI32 offX = -16, offY = -16, i = 0; offY<=0 && i<=16; offX++, offY++, i++)
			{
				addX = (SI32)((double) offX * cos(fRad)-(double) offY * sin(fRad));
				addY = (SI32)((double) offX * sin(fRad)+(double) offY * cos(fRad));
				SI32 siColor = colorTable[i];
				GP.Line(screensx+addX, screensy+addY , screentx+addX, screenty+addY, siColor ,TRUE);
			}
			for(     offX =  -1, offY =  +1, i = 17; offY<=16 && i<= 32; offX--, offY++, i++)
			{
				addX = (SI32)((double) offX * cos(fRad)-(double) offY * sin(fRad));
				addY = (SI32)((double) offX * sin(fRad)+(double) offY * cos(fRad));
				SI32 siColor = colorTable[i];
				GP.Line(screensx+addX, screensy+addY , screentx+addX, screenty+addY, siColor ,TRUE);
			}
		}
		else	// + 180
		{
			double Length = TSQRT((screenty - screensy)*(screenty - screensy)+(screentx - screensx)*(screentx - screensx));
			double fRad   = acos(  (screentx - screensx)/Length);
			SI32 addX = 0;
			SI32 addY = 0;
			for(SI32 offX = -16, offY = -16, i = 0; offY<=0 && i<=16; offX++, offY++, i++)
			{
				addX = (SI32)((double) offX * cos(fRad)-(double) offY * sin(fRad));
				addY = (SI32)((double) offX * sin(fRad)+(double) offY * cos(fRad));
				SI32 siColor = colorTable[i];
				GP.Line(screensx+addX, screensy+addY , screentx+addX, screenty+addY, siColor,TRUE );
			}
			for(     offX =  -1, offY =  +1, i = 17; offY<=16 && i<= 32; offX--, offY++, i++)
			{
				addX = (SI32)((double) offX * cos(fRad)-(double) offY * sin(fRad));
				addY = (SI32)((double) offX * sin(fRad)+(double) offY * cos(fRad));
				SI32 siColor = colorTable[i];
				GP.Line(screensx+addX, screensy+addY , screentx+addX, screenty+addY, siColor ,TRUE);
			}


		}
	}

	return;
}


// Push
bool CMagicLaserBeamObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicLaserBeamObj::SetDamage( SI32 si32Damage )
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

