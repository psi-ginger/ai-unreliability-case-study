#include "MagicUpperDragon.h"

#include "../MagicMsgObj.h"
#include "../MagicMgr.h"
#include "../MagicAttribute/MagicAttribute.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/client.h"

extern cltCommonLogic* pclClient;

CMagicUpperDragonObj::CMagicUpperDragonObj( CMagicMgr *pMagicMgr ) : CMagicBaseObj( pMagicMgr )
{
	m_pMagicInfo = new stMagicInfo();

	Initialize();
}

CMagicUpperDragonObj::~CMagicUpperDragonObj()
{
	Destroy();
}

void CMagicUpperDragonObj::Initialize()
{
	memset( m_pMagicInfo, 0, sizeof( stMagicInfo ) );
	m_si32AddDamage = 0;
	m_si32TotalDamage = 0;

	return;
}

void CMagicUpperDragonObj::Destroy()
{
	if ( m_pMagicInfo )
	{
		delete m_pMagicInfo;
		m_pMagicInfo = NULL;
	}

	return;
}

bool CMagicUpperDragonObj::SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique )
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
bool CMagicUpperDragonObj::Action()
{	
	if ( CMagicBaseObj::Action() )
	{
		return true;
	}	

	if ( pclClient->GameMode == GAMEMODE_SERVER )		// 서버
	{
		if ( GetCurrentFrame() == GetMaxFrameNum() / 3 && GetCurrentMagicDelayFrame() == 0 )
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
			//if(pclClient->pclCM->IsAlive(id) == TRUE)
			{
				//pclClient->PushEffect( EFFECT_MAGIC_SWORDBOMB_FIRE, pclClient->pclCM->CR[id]->GetX(), pclClient->pclCM->CR[id]->GetY() );
			}
		}
	}


	return false;
}

void CMagicUpperDragonObj::Draw( )
{	
	if ( GetStatus() != MAGIC_PLAY_STATUS )
	{
		return;
	}

	CMagicAttribute* pMagicAtt =  pclClient->pclMagicManager->GetMagicAttributePtr() ;
	if ( pMagicAtt == NULL ) return ;
	SI32 group = pMagicAtt->GetMagicGroup(m_pMagicInfo->sHeader.si32MagicKind) ;

	switch(group)
	{
	case MAGIC_GROUP_UPPERDRGON:
		{
			SetMagicSpr( GIMG_MAGIC_UPPERDRAGON1 );				// Image index

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()*2/3 -80 ;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			TSpr* pspr1	= pclClient->GetGlobalSpr(GIMG_MAGIC_UPPERDRAGON2);
			if(pspr1)
			{
				SI32 drawx = m_si32DrawXPos + GetMagicSpr()->GetXSize()/2 - pspr1->GetXSize()/2 ;
				SI32 drawy = m_si32DrawYPos + GetMagicSpr()->GetYSize() - pspr1->GetYSize() + 40;
				GP.PutSprScreen(pspr1, drawx, drawy, GetCurrentFrame()% pspr1->GetImageNum());
			}
			TSpr* pspr2	= pclClient->GetGlobalSpr(GIMG_MAGIC_UPPERDRAGON3);
			if(pspr2)
			{
				SI32 drawx = m_si32DrawXPos + GetMagicSpr()->GetXSize()/2 - pspr2->GetXSize()/2 ;
				SI32 drawy = m_si32DrawYPos + GetMagicSpr()->GetYSize() - pspr2->GetYSize() + 20 ;
				GP.PutSprScreen(pspr2, drawx, drawy, GetCurrentFrame()% pspr2->GetImageNum());
			}
		}
		break;
	case MAGIC_GROUP_SUMMONDRAGON:
		{
			SetMagicSpr( GIMG_MAGIC_SUMMONDRAGON );				// Image index

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()*2/3 ;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());
		}
		break;
	case MAGIC_GROUP_FROZENGOLEM:
		{
			SetMagicSpr( GIMG_MAGIC_FROZENGOLEM );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+45;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;
	case MAGIC_GROUP_FROZENQUEEN:
		{
			SetMagicSpr( GIMG_MAGIC_FROZENQUEEN );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+50;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;
	case MAGIC_GROUP_FROZENSORCERER:
		{
			SetMagicSpr( GIMG_MAGIC_FROZENSORCERER );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+80;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;
	case MAGIC_GROUP_GOTPA:
		{
			SetMagicSpr( GIMG_MAGIC_GOTPA );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+20;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;

	case MAGIC_GROUP_DRACULA:
		{
			SetMagicSpr( GIMG_MAGIC_DRACULA );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+170;

			GP.PutSpr(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			// FX출력
			SetMagicSpr( GIMG_MAGIC_DRACULAFX );
			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			if ( GetCurrentFrame() == 1 )
			{
				if(pclClient->pclCM->IsAlive(1) == TRUE)			
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DRACULA_MAGIC")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				}
			}		
		}
		break;
	case MAGIC_GROUP_PRIEST:
		{
			SetMagicSpr( GIMG_MAGIC_PRIEST );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+50;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			if ( GetCurrentFrame() == 1 )
			{
				if(pclClient->pclCM->IsAlive(1) == TRUE)			
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_PRIEST_ATTACK")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				}
			}		

		}
		break;
	case MAGIC_GROUP_MANDRAGORA:
		{
			SetMagicSpr( GIMG_MAGIC_MANDRAGORA );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+45;

			GP.PutSpr(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % (GetMagicSpr()->GetImageNum()/2));

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % (GetMagicSpr()->GetImageNum()/2) + (GetMagicSpr()->GetImageNum()/2));

			if ( GetCurrentFrame() == 1 )
			{
				if(pclClient->pclCM->IsAlive(1) == TRUE)			
				{
				//	pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DRACULA_MAGIC")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				}
			}		
		}
		break;
	case MAGIC_GROUP_PAPAGOBLIN:
		{
			SetMagicSpr( GIMG_MAGIC_PAPAGOBIN );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+55;
			
			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			if ( GetCurrentFrame() == 1 )
			{
				if(pclClient->pclCM->IsAlive(1) == TRUE)			
				{
					//	pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DRACULA_MAGIC")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				}
			}		
		}
		break;
	case MAGIC_GROUP_EMPERORMASTER:
		{
			SetMagicSpr( GIMG_MAGIC_EMPERORMASTER );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+80;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());		
		}
		break;
	case MAGIC_GROUP_GREATWALLSOLDIER:
		{
			SetMagicSpr( GIMG_MAGIC_GREATWALLSOLDIER );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+170;

			GP.PutSpr(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			// FX출력
			SetMagicSpr( GIMG_MAGIC_GREATWALLSOLDIERFX );
			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());		
		}
		break;
	case MAGIC_GROUP_GREATWALLMAGICIAN:
		{
			SetMagicSpr( GIMG_MAGIC_GREATWALLMAGICIAN );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+80;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());		
		}
		break;


		/////////////
	//case MAGIC_GROUP_GREATWALLASURA:
	//	{
	//		SetMagicSpr( GIMG_MAGIC_GREATWALLASURA );

	//		//--------------------------------------
	//		// 화면 출력용 좌표로 전환한다. 
	//		//--------------------------------------
	//		SHORT screenx=0, screeny=0;
	//		cltMapClient* pclmap = (cltMapClient*)m_pclMap;
	//		pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

	//		m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
	//		m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+80;

	//		GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

	//		if ( GetCurrentFrame() == 1 )
	//		{
	//			if(pclClient->pclCM->IsAlive(1) == TRUE)			
	//			{
	//				pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GREATWALLTWINHEAD_MAGIC")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
	//			}
	//		}
	//	}
	//	break;
		///////////////


	case MAGIC_GROUP_GREATWALLSPIDER:
		{
			SetMagicSpr( GIMG_MAGIC_GREATWALLSPIDER );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+80;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

			if ( GetCurrentFrame() == 1 )
			{
				if(pclClient->pclCM->IsAlive(1) == TRUE)			
				{
					pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GREATWALLTWINHEAD_MAGIC")), pclClient->pclCM->CR[1]->GetX(), pclClient->pclCM->CR[1]->GetY() );
				}
			}		

		}
		break;
	case MAGIC_GROUP_SFROZENGOLEM:
		{
			SetMagicSpr( GIMG_MAGIC_SFROZENGOLEM );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+45;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;
	case MAGIC_GROUP_SFROZENQUEEN:
		{
			SetMagicSpr( GIMG_MAGIC_SFROZENQUEEN );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+50;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;

	case MAGIC_GROUP_CATPET:
		{
			SetMagicSpr( GIMG_CUT );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize();	// 냥이는 보정값을 더하지 않는다

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;

	case MAGIC_GROUP_AURABLADE:
		{
			SetMagicSpr( GIMG_MAGIC_AURABLADE );

			//--------------------------------------
			// 화면 출력용 좌표로 전환한다. 
			//--------------------------------------
			SHORT screenx=0, screeny=0;
			cltMapClient* pclmap = (cltMapClient*)m_pclMap;
			pclmap->TransFromRealDotToScreenDot(m_clDotHitPos.siX, m_clDotHitPos.siY, screenx, screeny);

			m_si32DrawXPos = screenx - GetMagicSpr()->GetXSize()/2;
			m_si32DrawYPos = screeny - GetMagicSpr()->GetYSize()+50;

			GP.PutSprScreen(GetMagicSpr(), m_si32DrawXPos, m_si32DrawYPos, GetCurrentFrame() % GetMagicSpr()->GetImageNum());

		}
		break;

	}
	return;
}


// Push
bool CMagicUpperDragonObj::Push( BYTE *pData, SI32 si32Size )
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

void CMagicUpperDragonObj::SetDamage( SI32 si32Damage )
{
	stMagicMsgDamage sMagicMsgDamage;

	sMagicMsgDamage.sMagicMsgHeader.si16MsgSize = sizeof( stMagicMsgDamage );
	sMagicMsgDamage.sMagicMsgHeader.si32MagicKind = GetKind();
	sMagicMsgDamage.sMagicMsgHeader.si32MsgKind = MAGIC_DAMAGE;

	sMagicMsgDamage.si32FromID = m_pMagicInfo->sHeader.si32FromID;
	sMagicMsgDamage.si32ToID = m_pMagicInfo->sHeader.si32ToID;

	sMagicMsgDamage.si32Damage = si32Damage;

	Push( (BYTE*) &sMagicMsgDamage, sMagicMsgDamage.sMagicMsgHeader.si16MsgSize );	

	return;
}
