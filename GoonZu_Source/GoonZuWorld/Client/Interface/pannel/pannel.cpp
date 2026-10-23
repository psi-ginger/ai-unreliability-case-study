//---------------------------------
// 2005/10/3 김광명
//---------------------------------
#include <CommonLogic.h>
#include "Pannel.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "../../Client.h"

#include "Char\CharManager\CharManager.h"

#include "Interface/pannel/NPannel_DateMoneyDlg.h"
#include "Interface/pannel/NPannel_CharStatusDlg.h"
#include "Interface/pannel/NPannel_SummonDlg.h"
#include "Interface/pannel/NPannel_OrganDlg.h"
#include "Interface/pannel/NPannel_SoulGuardDlg.h"

#include "../Client/ninterface/NGMInfo/NGMInfoDlg.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"


extern cltCommonLogic* pclClient;

cltPannel::cltPannel( SI32 siDrawStartX, SI32 siDrawStartY )
{
	// 그릴 시작 위치를 받아온다
	m_siDrawStartX = siDrawStartX;
	m_siDrawStartY = siDrawStartY;

}

cltPannel::~cltPannel()
{
}


void cltPannel::SetszDate(TCHAR* szDate)
{
	if ( szDate == NULL )
	{
		return;
	}

	StringCchCopy(m_szDate, PANNEL_MAX_STRING_SIZE, szDate);

	m_szDate[PANNEL_MAX_STRING_SIZE-1] = '\0';
}

void cltPannel::Draw( LPDIRECTDRAWSURFACE7 lpsurface,  SI32 siRealX,  SI32 siRealY, SI32 siRandomX,  SI32 siRandomY )
{
	cltClient* pclclient = (cltClient *)pclClient;
	if ( pclclient == NULL )
	{
		return;
	}

	// 관전 모드일 경우 보여주지 않는다
	if( pclclient->bWatchModeSwitch )
	{
		if ( pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ] != NULL )
		{
			((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->Hide();
		}

		if ( pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ] != NULL )
		{
			((CNPannel_CharStatusDlg*)pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ])->Hide();
		}

		if ( pclclient->m_pDialog[ PANNEL_SUMMON_DLG ] != NULL )
		{
			((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->Hide();
		}

		if ( pclclient->m_pDialog[ PANNEL_ORGAN_DLG ] != NULL )
		{
			((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->Hide();
		}

		if ( pclclient->m_pDialog[ PANNEL_SOULGUARD_DLG ] != NULL )
		{
			((CNPannel_SoulGuardDlg*)pclclient->m_pDialog[ PANNEL_SOULGUARD_DLG ])->Hide();
		}
			
		return;
	}

	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	// 판넬 시작 위치
	SI32 siPannelX	= m_siDrawStartX + /*siRealX*/ + siRandomX;
	SI32 siPannelY	= m_siDrawStartY + /*siRealY*/ + siRandomY;

	// 날짜,돈 시작위치
	SI32 siDateMoneyStartX	= siPannelX + 2;
	SI32 siDateMoneyStartY	= siPannelY + 1;
	// 캐릭터 정보 시작위치
	SI32 siCharStatusStartX = siPannelX + 2;
	SI32 siCharStatusStartY = siPannelY + 23;
	// 소환수 정보 시작위치
	SI32 siSummonStartX		= siPannelX + 2;
	SI32 siSummonStartY		= siPannelY + 104;
	// 내장기관 정보 시작위치
	SI32 siOrganStartX		= siPannelX + 136;
	SI32 siOrganStartY		= siPannelY + 104;
	// 수호정령 정보 시작위치
	SI32 siSoulGuardStartX	= siPannelX + 2;
	SI32 siSoulGuardStartY	= siPannelY + 184;


	//---------------------------------------------------------------
	//	날짜와 돈 출력
	//---------------------------------------------------------------
	GMONEY	gmMoney							= pclChar->pclCI->clIP.GetMoney();
	TCHAR	szMoney[PANNEL_MAX_STRING_SIZE]	= TEXT("");
	TCHAR*	pText							= GetTxtFromMgr(4837);

	g_JYLibFunction.SetNumUnit( gmMoney, szMoney, PANNEL_MAX_STRING_SIZE, pText );

	
	if ( pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ] == NULL )
	{
		pclclient->CreateInterface( PANNEL_DATEMONEY_DLG );
		((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->SetX( siDateMoneyStartX );
		((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->SetY( siDateMoneyStartY );
	}

	((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->SetDateMoney( m_szDate, szMoney );
	if ( ((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->IsShow() == false )
	{
		((CNPannel_DateMoneyDlg*)pclclient->m_pDialog[ PANNEL_DATEMONEY_DLG ])->Show();
	}


	//---------------------------------------------------------------
	//	캐릭터 정보
	//---------------------------------------------------------------
	if ( pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ] == NULL )
	{
		pclclient->CreateInterface( PANNEL_CHARSTATUS_DLG );
		((CNPannel_CharStatusDlg*)pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ])->SetX( siCharStatusStartX );
		((CNPannel_CharStatusDlg*)pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ])->SetY( siCharStatusStartY );
	}

	if ( ((CNPannel_CharStatusDlg*)pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ])->IsShow() == false )
	{
		((CNPannel_CharStatusDlg*)pclclient->m_pDialog[ PANNEL_CHARSTATUS_DLG ])->Show();
	}

	
	//---------------------------------------------------------------
	//	소환수
	//---------------------------------------------------------------
	CNPannel_SummonDlg *pclSummonDlg = (CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ];
	if ( pclSummonDlg == NULL )
	{
		pclclient->CreateInterface( PANNEL_SUMMON_DLG );
		pclSummonDlg = (CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ];

		if ( pclSummonDlg == NULL )
		{
			return;
		}

		pclSummonDlg->SetX( siSummonStartX );
		pclSummonDlg->SetY( siSummonStartY );
	}

	if ( pclSummonDlg->GetShowDialog() == false )
	{
		pclSummonDlg->Hide();
	}
	else if ( pclSummonDlg->IsShow() == false )
	{
		pclSummonDlg->Show();
	}
	/*
	if ( pclclient->m_pDialog[ PANNEL_SUMMON_DLG ] == NULL )
	{
		pclclient->CreateInterface( PANNEL_SUMMON_DLG );
		((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->SetX( siSummonStartX );
		((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->SetY( siSummonStartY );
	}

	if ( ((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->GetShowDialog() == false )
	{
		((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->Hide();
	}
	else
	{
		if ( ((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->IsShow() == false )
		{
			((CNPannel_SummonDlg*)pclclient->m_pDialog[ PANNEL_SUMMON_DLG ])->Show();
		}
	}
	*/

	//---------------------------------------------------------------
	//	내장기관
	//---------------------------------------------------------------
	CNPannel_OrganDlg *pclOrganDlg = (CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ];
	if ( pclOrganDlg == NULL )
	{
		pclclient->CreateInterface( PANNEL_ORGAN_DLG );

		pclOrganDlg = (CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ];
		if ( pclOrganDlg == NULL )
		{
			return;
		}
		pclOrganDlg->SetX( siOrganStartX );
		pclOrganDlg->SetY( siOrganStartY );
	}

	if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bOrganPannelShow == false )
	{
		pclOrganDlg->Hide();
	}
	else if ( pclOrganDlg->IsShow() == false ) 
	{
		pclOrganDlg->Show();
	}

	//---------------------------------------------------------------
	//	수호정령
	//---------------------------------------------------------------
	CNPannel_SoulGuardDlg* pclSoulGuardDlg = (CNPannel_SoulGuardDlg*)pclclient->m_pDialog[ PANNEL_SOULGUARD_DLG ];
	if ( pclSoulGuardDlg == NULL )
	{
		pclclient->CreateInterface( PANNEL_SOULGUARD_DLG );
		pclSoulGuardDlg = (CNPannel_SoulGuardDlg*)pclclient->m_pDialog[ PANNEL_SOULGUARD_DLG ];

		if ( pclSoulGuardDlg )
		{
			pclSoulGuardDlg->SetX( siSoulGuardStartX );
			pclSoulGuardDlg->SetY( siSoulGuardStartY );
		}
	}
	else
	{
		if ( pclSoulGuardDlg->GetShowDialog() == false )
		{
			pclSoulGuardDlg->Hide();
		}
		else if ( pclSoulGuardDlg->IsShow() == false )
		{
			pclSoulGuardDlg->Show();
		}
	}

	/*
	if ( pclclient->m_pDialog[ PANNEL_ORGAN_DLG ] == NULL )
	{
		pclclient->CreateInterface( PANNEL_ORGAN_DLG );
		((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->SetX( siOrganStartX );
		((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->SetY( siOrganStartY );
	}

	if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bOrganPannelShow == false )
	{
		((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->Hide();
	}
	else
	{
		if ( ((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->IsShow() == false )
		{
			((CNPannel_OrganDlg*)pclclient->m_pDialog[ PANNEL_ORGAN_DLG ])->Show();
		}
	}
	*/
	//---------------------------------------------------------------
	//	GM Info
	//---------------------------------------------------------------
	if ( pclclient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
	{
		if ( pclclient->m_pDialog[ NGMINFO_DLG ] == NULL )
		{
			pclclient->CreateInterface( NGMINFO_DLG );
		}
	}

	//---------------------------------------------------------------
	//	내구도 경고창
	//---------------------------------------------------------------
	if ( pclclient->IsCountrySwitch(Switch_NewDurability) )
	{
		if ( NULL == pclclient->m_pDialog[NWARNDURABILITY_DLG] )
		{
			pclclient->CreateInterface( NWARNDURABILITY_DLG );
		}
	}
}


/*
cltPannel::cltPannel(SI32 drawstartx, SI32 drawstarty)
{
	DrawStartX	= drawstartx;
	DrawStartY	= drawstarty;

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/CharPannelBack.SPR"), &m_PannelBackGroundSpr );	
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonBack.Spr"), &m_SummonPannelBackSpr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/Exp_new.Spr"), &m_ExpSpr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/ExpRound.Spr"), &m_ExpRoundSpr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonExp.Spr"), &m_SummonExpSpr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/HPMPgage.Spr"), &m_HpMpSqr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/PlayerPortrait.SPR"), &m_PlayerPortraitSqr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonLife.Spr"), &m_SummonLifeSpr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonMana.Spr"), &m_SummonManaSpr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonStamina.Spr"), &m_SummonStaminaSpr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonRecovery.Spr"), &m_SummonRecoverySpr );

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/GageBackGround.SPR"), &m_GageBackGroundSpr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonGray.SPR"), &m_SummonBackGroundSpr );
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonGary2.Spr"), &m_SummonBackGroundSpr2 );


	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonPortrait.SPR"),&m_SummonPortrait[0]);
	//KHY - 0128 - 소환수 초상 파일 추가.
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/SummonPortrait1.SPR"),&m_SummonPortrait[1]);

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/HealthStatus.SPR"),&m_HealthStatusSpr);
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/HungryBar.SPR"),&m_HungryGageSpr);

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/HealthStatus2.SPR"),&m_HealthStatusSpr2);

	GP.LoadSprFromFile(TEXT("./Interface/Pannel/LevelMark.SPR"),&m_LevelMark);
	GP.LoadSprFromFile(TEXT("./Interface/Pannel/LevelNumber.SPR"),&m_LevelNumber);

	m_siSickStatus = 0 ;
	m_siHungryStatus = 0 ;
	m_szDate[0] = 0;
	m_szMoney[0] = 0;

	m_siDrawHealthStatusCnt = 0;
}


cltPannel::~cltPannel()
{



	if ( m_PannelBackGroundSpr.pImage )
	{
		GP.FreeSpr( m_PannelBackGroundSpr );
	}
	if ( m_SummonPannelBackSpr.pImage )
	{
		GP.FreeSpr(m_SummonPannelBackSpr) ;
	}

	if ( m_ExpSpr.pImage )
	{
		GP.FreeSpr( m_ExpSpr );
	}
	if ( m_ExpRoundSpr.pImage )
	{
		GP.FreeSpr( m_ExpRoundSpr );
	}
	if ( m_SummonExpSpr.pImage )
	{
		GP.FreeSpr(m_SummonExpSpr) ;
	}
	if ( m_PlayerPortraitSqr.pImage )
	{
		GP.FreeSpr(m_PlayerPortraitSqr) ;
	}
	if ( m_HpMpSqr.pImage )
	{
		GP.FreeSpr(m_HpMpSqr) ;
	}

	if ( m_SummonLifeSpr.pImage )
	{
		GP.FreeSpr(m_SummonLifeSpr) ;
	}

	if ( m_SummonManaSpr.pImage )
	{
		GP.FreeSpr(m_SummonManaSpr) ;
	}

	if ( m_SummonStaminaSpr.pImage )
	{
		GP.FreeSpr(m_SummonStaminaSpr) ;
	}

	if ( m_SummonRecoverySpr.pImage )
	{
		GP.FreeSpr(m_SummonRecoverySpr) ;
	}

	if ( m_GageBackGroundSpr.pImage )
	{
		GP.FreeSpr( m_GageBackGroundSpr );
	}
	if ( m_SummonBackGroundSpr.pImage )
	{
		GP.FreeSpr(m_SummonBackGroundSpr) ;
	}

	if ( m_SummonBackGroundSpr2.pImage )
	{
		GP.FreeSpr(m_SummonBackGroundSpr2) ;
	}

	if ( m_SummonPortrait[0].pImage )
	{
		GP.FreeSpr(m_SummonPortrait[0]);
	}

	if ( m_SummonPortrait[1].pImage )
	{
		GP.FreeSpr(m_SummonPortrait[1]);
	}

	if ( m_HealthStatusSpr.pImage )
	{
		GP.FreeSpr(m_HealthStatusSpr);
	}
	if ( m_HungryGageSpr.pImage )
	{
		GP.FreeSpr(m_HungryGageSpr);
	}
	if ( m_HealthStatusSpr2.pImage )
	{
		GP.FreeSpr(m_HealthStatusSpr2);
	}

	m_siSickStatus = 0 ;
	m_siHungryStatus = 0 ;
}

void cltPannel::Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry, 
					 TCHAR* rank, SI64 money, SI32 level, SI32 curexp, SI32 nextexp, SI32 life, SI32 maxlife,
					 SI32 mana, SI32 maxmana,SI32 health, SI32 siMousePosX, SI32 siMousePosY,
					 SI32 preexp, SI32 heallife,SI32 healmana,SI32 randx,SI32 randy)
{  
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	//// 말풍선 관련 처리들/////

	if((pclClient->CurrentClock / 50) % 1000 ==0)
	{
		m_siDrawHealthStatusCnt = 200;
	}

	if( m_siDrawHealthStatusCnt > 0 )
	{
		m_siDrawHealthStatusCnt--;
	}


	if ( life < 0 )
	{		
		return;
	}

	if ( mana < 0 )
	{
		return;
	}

	float f32ExpRate	= 1.0f;

	if ( nextexp == preexp )
	{
		f32ExpRate = 1.0f;
	}
	else
	{
		f32ExpRate = ( curexp - preexp ) / ( ( nextexp - preexp ) * 1.0f );
	}

	if ( f32ExpRate > 1.0f )
	{
		f32ExpRate = 1.0f;
	}
	else if ( f32ExpRate < 0.0f )
	{
		f32ExpRate = 0.0f;
	}

	float f32LifeRate	= 1.0f;
	if ( life > maxlife && maxlife > 0 )
	{
		life = maxlife;
		f32LifeRate =	life / ( maxlife * 1.0f );
	}
	else if ( maxlife > 0 )
	{
		f32LifeRate	=	life / ( maxlife * 1.0f );
	}

	float f32HealLifeRate = 1.0f;
	if ( heallife > maxlife && maxlife > 0 )
	{
		heallife = maxlife ;
		f32HealLifeRate =	heallife / ( maxlife * 1.0f );
	}
	else if ( maxlife > 0 )
	{
		f32HealLifeRate	=	heallife / ( maxlife * 1.0f );
	}

	float f32ManaRate	=	1.0f; 
	if ( mana > maxmana && maxmana > 0 )
	{
		mana = maxmana;
		f32ManaRate = mana / ( maxmana * 1.0f );
	}
	else if ( maxmana > 0 )
	{
		f32ManaRate = mana / ( maxmana * 1.0f );
	}

	SI32 pannelstartx	= DrawStartX + rx + randx ;
	SI32 pannelstarty	= DrawStartY + ry + randy ;

	SI32 ExpStartX = pannelstartx + 23;
	SI32 ExpStartY = pannelstarty + 107;

	SI32 LifeStartX = pannelstartx + 23;
	SI32 LifeStartY = pannelstarty + 82;

	SI32 ManaStartX = pannelstartx + 23;
	SI32 ManaStartY = pannelstarty + 93;

	SI32 i = 0;
	SI32 siExpWidth = (SI32)(212 * f32ExpRate);
	SI32 siLifeWidth = (SI32)(212 * f32LifeRate);
	SI32 siManaWidth = (SI32)(212 * f32ManaRate);

	SI32 siHealLife = (SI32)(212 * f32HealLifeRate) ;
	//SI32 siHealMana;

	if( GP.LockSurface(lpsurface) == TRUE )
	{		
		GP.PutSpr( &m_PannelBackGroundSpr, pannelstartx, pannelstarty, 0 );

		if ( siExpWidth > 6 )
		{
			for ( i = 0; i < siExpWidth - 6 ; i+=2 )
			{
				GP.PutSpr( &m_ExpSpr, ExpStartX + i, ExpStartY, 0 );
			}
		}
		else if ( siExpWidth > 0 )
		{
			GP.PutSpr( &m_ExpSpr, ExpStartX + i, ExpStartY, 0 );
		}

		if ( siLifeWidth > 6 )
		{
			GP.PutSpr( &m_HpMpSqr, LifeStartX , LifeStartY, 0 );
			for ( i = 2 ; i < siHealLife - 8; i += 2 )
			{
				GP.PutSpr( &m_HpMpSqr, LifeStartX + i, LifeStartY, 1 );
			}

			for ( i = 2; i < siLifeWidth - 8; i+= 2 ) 
			{
				GP.PutSprScreen( &m_HpMpSqr, LifeStartX + i, LifeStartY, 1 );
			}
			GP.PutSpr( &m_HpMpSqr, LifeStartX + i , LifeStartY, 2 );
		}
		else if ( siLifeWidth > 0 )
		{
			GP.PutSprScreen( &m_HpMpSqr, LifeStartX, LifeStartY, 0 );
			GP.PutSpr( &m_HpMpSqr, LifeStartX, LifeStartY, 0 );
		}

		if ( siManaWidth > 6 )
		{
			GP.PutSpr( &m_HpMpSqr, ManaStartX , ManaStartY, 3 );
			for ( i = 2; i < siManaWidth - 8 ; i+= 2 )
			{
				GP.PutSpr( &m_HpMpSqr, ManaStartX + i, ManaStartY, 4 );
			}
			GP.PutSpr( &m_HpMpSqr, ManaStartX + i , ManaStartY, 5 );
		}
		else if (siManaWidth > 0 )
		{
			GP.PutSpr( &m_HpMpSqr, ManaStartX, ManaStartY, 3 );
		}

		DrawHungryGage(pannelstartx,pannelstarty,health);

		static bool bSwitch = true ;
		static SI32 siDelay = 0 ;

		if (bSwitch)
		{
			DrawBreath(pannelstartx,pannelstarty,health);
			DrawDigest(pannelstartx,pannelstarty,health);
			DrawCirculation(pannelstartx,pannelstarty,health);
			DrawNerve(pannelstartx,pannelstarty,health);

			siDelay ++ ;
			if ( siDelay > 10 )
			{
				siDelay = 0 ;
				bSwitch = false ;
			}
		}
		else
		{
			siDelay ++ ;
			if ( siDelay > 10 )
			{
				siDelay = 0 ;
				bSwitch = true ;
			}
		}


		// 얼굴 그려주기
		if(pclclient->pclCM->CR[1]->GetKind() == KIND_HERO3) // 동양 남자
		{
			GP.PutSpr(&m_PlayerPortraitSqr, pannelstartx + 9 ,pannelstarty + 18, 0  );
		}
		else if(pclclient->pclCM->CR[1]->GetKind() == KIND_HEROINE3) // 동양 여자
		{
			GP.PutSpr(&m_PlayerPortraitSqr, pannelstartx + 9 ,pannelstarty + 18, 1  );
		}
		else if(pclclient->pclCM->CR[1]->GetKind() == KIND_HERO4) // 서양 남자
		{
			GP.PutSpr(&m_PlayerPortraitSqr, pannelstartx + 9 ,pannelstarty + 18, 2  );
		}
		else if(pclclient->pclCM->CR[1]->GetKind() == KIND_HEROINE4) // 서양 여자
		{
			GP.PutSpr(&m_PlayerPortraitSqr, pannelstartx + 9 ,pannelstarty + 18, 3  );
		}

		if (pclClient->IsCountrySwitch(Switch_DisplayLevelInPannel) )
		{
			// 레벨 출력
			SI32 siLevelFont = 0;
			if		(level > 60)	{	siLevelFont = 2;		}
			else if (level > 30)	{	siLevelFont = 2;		}
			else					siLevelFont = 2;

			GP.PutSpr(&m_LevelMark, pannelstartx + 1 ,pannelstarty + 55, siLevelFont  );

			// 레벨 숫자 출력
			TCHAR strNum[ 10 ]= TEXT("");
			_itot( level, strNum, 10 );

			SI32 si32StrLength = _tcslen( strNum );
			SI32 addFont = 0;
			if		(level > 60)	{	addFont = 20;	}
			else if (level > 30)	{	addFont = 20;	}
			else					addFont = 20;

			SI32 basex = pannelstartx + 1 + 28 ;
			SI32 basey = pannelstarty + 55 + 6;
			switch( si32StrLength )
			{
			case 1:
				{
					GP.PutSpr(&m_LevelNumber, basex, basey,  level + addFont );
				}
				break;

			case 2:
				{
					GP.PutSpr( &m_LevelNumber, basex, basey,								level/10 + addFont );
					GP.PutSpr( &m_LevelNumber, basex + m_LevelNumber.GetXSize() - 1, basey, level%10 + addFont );
				}
				break;

			case 3:
				{
					GP.PutSpr( &m_LevelNumber, basex, basey,									level/100 + addFont );
					GP.PutSpr( &m_LevelNumber, basex + m_LevelNumber.GetXSize() - 1, basey,    (level/10)%10 + addFont );
					GP.PutSpr( &m_LevelNumber, basex + m_LevelNumber.GetXSize() * 2 - 2, basey, level%10 + addFont );
				}
				break;
			}
		}

		GP.UnlockSurface(lpsurface);
	}

	//////////////////////////////////////////////////////////////////////////
	// 텍스트 출력
	//////////////////////////////////////////////////////////////////////////

	HDC hdc;	
	if(lpsurface->GetDC(&hdc) == DD_OK)
	{
		HFONT hFont;
		if ( pclClient->siServiceArea == ConstServiceArea_English )
		{
			hFont = g_FontManager.SetFont( 12, TEXT("Tahoma"));	
		}
		else
		{
			hFont = g_FontManager.SetFont( 11 );
		}

		HFONT hOldFont = NULL;
		if ( hFont != NULL )
		{
			hOldFont = (HFONT)SelectObject( hdc, hFont );
		}
		SetBkMode(hdc, TRANSPARENT);

		TCHAR szDiseaseText[128] = TEXT("");
		TCHAR szDestText[256] = TEXT("");
		TCHAR szHealthText[256] = TEXT("");
		// 병에 걸렸다면 캐릭터 말풍선에 질병 출력 
		if( TRUE == pclClient->pclCM->CR[1]->clPB.clHealth.GetCurDiseaseName(szDiseaseText,128)//상태이상 메세지 출력.
			&& m_siDrawHealthStatusCnt > 0	) 
		{	
			GP.PutSpr(&m_HealthStatusSpr, pannelstartx + 100,pannelstarty + 50 );
			
			TCHAR* pszSrcText = GetTxtFromMgr(5889);
			StringCchPrintf(szDestText,256,pszSrcText, szDiseaseText);
		}
		else if( GetHealthText(szHealthText) ) // 건강상태가 좋지 않다면..
		{
			GP.PutSpr(&m_HealthStatusSpr, pannelstartx + 100,pannelstarty + 50 );
			StringCchCopy( szDestText, 256, szHealthText );
		}
		// 배고프면 말풍선에 배고프다고 출력
		else if(m_siHungryStatus & HEALTH_HUNGRY && m_siDrawHealthStatusCnt > 0)
		{
			GP.PutSpr(&m_HealthStatusSpr, pannelstartx + 100,pannelstarty + 50 );
			TCHAR* pText = GetTxtFromMgr(5828);
			StringCchCopy(szDestText, 256, pText);
		} // 배부르면 말풍선에 배부르다고
		else if( m_siHungryStatus & HEALTH_LEPLETE && m_siDrawHealthStatusCnt > 0)
		{
			GP.PutSpr(&m_HealthStatusSpr, pannelstartx + 100,pannelstarty + 50 );
			TCHAR* pText = GetTxtFromMgr(5829);
			StringCchCopy(szDestText, 256, pText);
		}
		HFONT hHealthFont;
		if ( pclClient->siServiceArea == ConstServiceArea_English )
		{
			hHealthFont  = g_FontManager.SetFont( 13, TEXT("Tahoma") );
		}
		else
		{
			hHealthFont  = g_FontManager.SetFont( 13 );
		}
		if ( hHealthFont != NULL )
		{
			hOldFont = (HFONT)SelectObject( hdc, hHealthFont );
		}
		SetTextColor(hdc,COLOR_DIALOG_RED);
		SetTextAlign( hdc, TA_CENTER ); 
		TextOut(hdc, pannelstartx + 180, pannelstarty + 62, szDestText, lstrlen(szDestText) );  
		if ( hOldFont )
		{
			SelectObject( hdc, hOldFont );
		}


//		HFONT hHealthFont;
		if( pclClient->siServiceArea == ConstServiceArea_English )
		{
			hHealthFont  = g_FontManager.SetFont( 12, TEXT("Tahoma") );
		}
		else
		{
			hHealthFont  = g_FontManager.SetFont( 11 );
		}

		if ( hHealthFont != NULL )
		{
			hOldFont = (HFONT)SelectObject( hdc, hHealthFont );
		}

		SetTextColor(hdc,COLOR_DIALOG_WHITE); 


		/////////////////////////// hp xx/xx, mp xx/xx, exp -> xx.x%   HP, MP, EXP 출력
		SetTextAlign( hdc, TA_CENTER ); 

		{
			TCHAR szDestText[128] = TEXT("");   
			StringCchPrintf( szDestText,128,"%d/%d",life,maxlife);  
			TextOut(hdc, pannelstartx + 129, pannelstarty + 81 , szDestText, lstrlen(szDestText));
			StringCchPrintf( szDestText,128,"%d/%d",mana,maxmana); 
			TextOut(hdc, pannelstartx + 129, pannelstarty + 93 , szDestText, lstrlen(szDestText));

			if( curexp <= nextexp )  
			{
				float expPercentage = (curexp - preexp) / (float)(nextexp - preexp) * 100;     
				StringCchPrintf( szDestText,128,"  %3.2f%s", expPercentage,"%" );   
				TextOut(hdc, pannelstartx + 129, pannelstarty + 106 , szDestText, lstrlen(szDestText));
			}
		}

		///////////////////////////////////////

		SetTextAlign( hdc, DT_RIGHT );			

		if(m_szDate[0] != NULL )	
		{
			// 게임 시간 출력
			TextOut(hdc, pannelstartx + 132, pannelstarty + 5, m_szDate, lstrlen(m_szDate));
		}

		if ( pclClient && pclClient->pclCM )
		{
			// 현재 가진 돈 출력
  			GMONEY Money = pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();
			TCHAR buffer[256 ] = TEXT("");
			TCHAR* pText = GetTxtFromMgr(4837);
			g_JYLibFunction.SetNumUnit( Money, buffer, 256, pText );

			TextOut(hdc, pannelstartx + 254, pannelstarty + 5, buffer, lstrlen(buffer));
		}
		// [영훈] 서버 시간(PC시간) 보여주기 - 2008.01.28
		if ( pclclient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
		{
			//////////////////////////////////////////////////////////
			// 서버시간 보여주기
			//////////////////////////////////////////////////////////
			TCHAR tszDisplayServerTime[256] = {'\0'};

			// 날짜
			UI16 uiYear		= pclclient->sTime.wYear;
			UI16 uiMonth	= pclclient->sTime.wMonth;
			UI16 uiDay		= pclclient->sTime.wDay;
			// 시간
			UI16 uiHour		= pclclient->sTime.wHour;
			UI16 uiMinute	= pclclient->sTime.wMinute;
			UI16 uiSecond	= pclclient->sTime.wSecond;

			StringCchPrintf(tszDisplayServerTime, 256, "[Server Time] %04d/%02d/%02d-%02d:%02d:%02d", 
				uiYear, uiMonth, uiDay, uiHour, uiMinute, uiSecond);

			SetTextAlign( hdc, DT_LEFT );	

			SetTextColor(hdc,COLOR_DIALOG_BLACK); // 그림자
			TextOut(hdc, pannelstartx + 270, pannelstarty + 5, tszDisplayServerTime, _tcslen(tszDisplayServerTime) );
			SetTextColor(hdc,COLOR_DIALOG_WHITE); // 실제 보여지는 숫자
			TextOut(hdc, pannelstartx + 269, pannelstarty + 4, tszDisplayServerTime, _tcslen(tszDisplayServerTime) );


			//////////////////////////////////////////////////////////
			// AutoUnblock Mode 설정 정보 보여주기
			//////////////////////////////////////////////////////////

			NTCHARString64 kAutoUnblockMode = TEXT("[Auto Unblock Mode : #mode#]");

			kAutoUnblockMode.Replace( TEXT("#mode#"), (pclclient->bAutoUnblock?TEXT("On"):TEXT("Off")) );

			SetTextColor(hdc,COLOR_DIALOG_BLACK); // 그림자
			TextOut(hdc, pannelstartx + 270, pannelstarty + 25, kAutoUnblockMode, kAutoUnblockMode.Length() );
			SetTextColor(hdc,COLOR_DIALOG_WHITE); // 실제 보여지는 문자
			TextOut(hdc, pannelstartx + 269, pannelstarty + 24, kAutoUnblockMode, kAutoUnblockMode.Length() );
		}


		if ( hOldFont )
		{
			SelectObject( hdc, hOldFont );
		}
		lpsurface->ReleaseDC(hdc);
	}
	///////// 텍스트 출력 끝
}

void cltPannel::DrawHungryGage(SI32 x , SI32 y , SI32 health)
{
	SI32 startX = x + 108 ;
	SI32 startY = y + 45;
	SI32 status = 0 ;

	// 9단계 배고픔 상태를 7단계 그래프로 표현

	m_siHungryStatus = 0 ;

	if ( health & HEALTH_STATUS_HUNGRY_1 )
	{
		status = 6 ;
		m_siHungryStatus |= HEALTH_LEPLETE ;
	}
	else if ( health & HEALTH_STATUS_HUNGRY_2 )
	{
		status = 5 ;
		m_siHungryStatus |= HEALTH_LEPLETE ;
	}
	else if ( health & HEALTH_STATUS_HUNGRY_3 )
	{
		status = 4 ;
		m_siHungryStatus |= HEALTH_LEPLETE ;
	}
	else if ( health & HEALTH_STATUS_HUNGRY_4 || health & HEALTH_STATUS_HUNGRY_5 || health & HEALTH_STATUS_HUNGRY_6 )
	{	
		status = 3 ;
	}
	else if ( health & HEALTH_STATUS_HUNGRY_7 )
	{
		status = 2 ;
		m_siHungryStatus |= HEALTH_HUNGRY ;
	}
	else if ( health & HEALTH_STATUS_HUNGRY_8 )
	{
		status = 1 ;
		m_siHungryStatus |= HEALTH_HUNGRY ;
	}
	else
	{
		status = 0 ;
		m_siHungryStatus |= HEALTH_HUNGRY ;
	}


	if( status >= 0 && status <= 6)
		GP.PutSpr(&m_HungryGageSpr, x + 126 ,y + 16 , status );
}

void cltPannel::DrawBreath(SI32 x,SI32 y,SI32 health)
{
	SI32 status = -1 ;

	m_siSickStatus = 0 ;

	if ( health & HEALTH_STATUS_ORG3_GOOD )
		return ;
	else if ( health & HEALTH_STATUS_ORG3_NORMAL )
		status = 9 ;
	else if ( health & HEALTH_STATUS_ORG3_BAD )
		status = 10 ;
	else if ( health & HEALTH_STATUS_ORG3_SERIOUS )
		status = 11 ;

	m_siSickStatus |= HEALTH_SICK ;

	//cyj 실제로 그려주지는 않음
	//GP.PutSpr(&m_HealthStatusSpr,x,y,status);
}

void cltPannel::DrawDigest(SI32 x,SI32 y,SI32 health)
{
	SI32 status = -1 ;

	if ( health & HEALTH_STATUS_ORG1_GOOD )
		return ;
	else if ( health & HEALTH_STATUS_ORG1_NORMAL )
		status = 6 ;
	else if ( health & HEALTH_STATUS_ORG1_BAD )
		status = 7 ;
	else if ( health & HEALTH_STATUS_ORG1_SERIOUS )
		status = 8 ;

	if ( !(m_siSickStatus & HEALTH_SICK) )
		m_siSickStatus |= HEALTH_SICK ;

	//cyj 실제로 그려주지는 않음
	//GP.PutSpr(&m_HealthStatusSpr,x,y,status);
}

void cltPannel::DrawCirculation(SI32 x,SI32 y,SI32 health)
{
	SI32 status = -1 ;

	if ( health & HEALTH_STATUS_ORG2_GOOD )
		return ;
	else if ( health & HEALTH_STATUS_ORG2_NORMAL )
		status = 3 ;
	else if ( health & HEALTH_STATUS_ORG2_BAD )
		status = 4 ;
	else if ( health & HEALTH_STATUS_ORG2_SERIOUS )
		status = 5 ;

	if ( !(m_siSickStatus & HEALTH_SICK) )
		m_siSickStatus |= HEALTH_SICK ;

	//cyj 실제로 그려주지는 않음
	//GP.PutSpr(&m_HealthStatusSpr,x,y,status);
}

void cltPannel::DrawNerve(SI32 x,SI32 y,SI32 health)
{
	SI32 status = -1 ;

	if ( health & HEALTH_STATUS_ORG4_GOOD )
		return ;
	else if ( health & HEALTH_STATUS_ORG4_NORMAL )
		status = 0 ;
	else if ( health & HEALTH_STATUS_ORG4_BAD )
		status = 1 ;
	else if ( health & HEALTH_STATUS_ORG4_SERIOUS )
		status = 2 ;

	if ( !(m_siSickStatus & HEALTH_SICK) )
		m_siSickStatus |= HEALTH_SICK ;

	//cyj 실제로 그려주지는 않음
	//GP.PutSpr(&m_HealthStatusSpr,x,y,status);
}

void cltPannel::DrawSummon(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry, SI16 portrait,
						   SI32 curexp, SI32 nextexp, SI32 life, SI32 maxlife,SI32 mana, SI32 maxmana,
						   SI32 preexp,SI32 heallife,SI32 status, SI32 stamina, SI32 maxstamina,
						   SI32 leftsummonstart, SI32 summonindex,SI32 randx,SI32 randy)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( summonindex < 0 || summonindex >= MAX_SUMMON_OWN ) return ;
	if ( m_SummonPannelBackSpr.pImage == NULL ) return ;

	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	if ( life < 0 )
	{		
		return;
	}

	if ( mana < 0 )
	{
		return;
	}

	float f32ExpRate	= 1.0f;

	if ( nextexp == preexp )
	{
		f32ExpRate = 1.0f;
	}
	else
	{
		f32ExpRate = ( curexp - preexp ) / ( ( nextexp - preexp ) * 1.0f );
	}

	if ( f32ExpRate > 1.0f )
	{
		f32ExpRate = 1.0f;
	}
	else if ( f32ExpRate < 0.0f )
	{
		f32ExpRate = 0.0f;
	}

	float f32LifeRate	= 1.0f;
	if ( life > maxlife && maxlife > 0 )
	{
		life = maxlife;
		f32LifeRate =	life / ( maxlife * 1.0f );
	}
	else if ( maxlife > 0 )
	{
		f32LifeRate	=	life / ( maxlife * 1.0f );
	}

	float f32HealLifeRate = 1.0f;
	if ( heallife > maxlife && maxlife > 0 )
	{
		heallife = maxlife ;
		f32HealLifeRate =	heallife / ( maxlife * 1.0f );
	}
	else if ( maxlife > 0 )
	{
		f32HealLifeRate	=	heallife / ( maxlife * 1.0f );
	}

	float f32ManaRate	=	1.0f; 
	if ( mana > maxmana && maxmana > 0 )
	{
		mana = maxmana;
		f32ManaRate = mana / ( maxmana * 1.0f );
	}
	else if ( maxmana > 0 )
	{
		f32ManaRate = mana / ( maxmana * 1.0f );
	}

	bool bEmptyStamina = false;

	if (stamina <= 0)
	{
		bEmptyStamina = true;
	}

	float f32StaminaRate = 1.0f;
	if (bEmptyStamina)
	{
		// 스테미너없을때는 회복시간을
		f32StaminaRate = leftsummonstart / (SUMMON_TAKEOUT_DELAY * 1.0f );
	}
	else
	{
		// 스태미너있을때는 스테미너 양을
		if ( stamina > maxstamina && stamina > 0)
		{
			stamina = maxstamina;
			f32StaminaRate = stamina / (maxstamina * 1.0f );
		}
		else if ( maxstamina > 0)
		{
			f32StaminaRate = stamina / (maxstamina * 1.0f );
		}

	}

	SI32 pannelstartx	= DrawStartX + rx + randx ;
	SI32 pannelstarty	= DrawStartY + ry + 120 + ( m_SummonPannelBackSpr.header.iYSize * summonindex ) + randy ;

	SI32 LifeStartX = pannelstartx + 48;
	SI32 LifeStartY = pannelstarty + 16;

	SI32 ManaStartX = pannelstartx + 48;
	SI32 ManaStartY = pannelstarty + 22;

	SI32 ExpStartX = pannelstartx + 48;
	SI32 ExpStartY = pannelstarty + 28;

	SI32 StaminaStartX = pannelstartx + 8;
	SI32 StaminaStartY = pannelstarty + 42;

	SI32 PortraitStartX = pannelstartx + 2 ;
	SI32 PortraitStartY = pannelstarty + 2 ;

	SI32 i = 0;

	SI32 siExpWidth = (SI32)(94 * f32ExpRate);
	SI32 siLifeWidth = (SI32)(94 * f32LifeRate);
	SI32 siManaWidth = (SI32)(94 * f32ManaRate);
	SI32 siStaminaWidth = (SI32) (134 * f32StaminaRate);

	SI32 siHealLife = (SI32)(94*f32HealLifeRate) ;

	if( GP.LockSurface(lpsurface) == TRUE )
	{
		GP.PutSpr( &m_SummonPannelBackSpr, pannelstartx, pannelstarty, 0 );

		GP.PutSpr( &m_SummonBackGroundSpr, ExpStartX, ExpStartY, 0 );		
		if ( siExpWidth > 2 )
		{
			//GP.PutSpr( &m_SummonExpSpr, ExpStartX , ExpStartY, 0 );
			for ( i = 2; i < siExpWidth - 2 ; i+=2 )
			{
				GP.PutSpr( &m_SummonExpSpr, ExpStartX + i, ExpStartY, 1 );
			}
			//GP.PutSpr( &m_SummonExpSpr, ExpStartX + i, ExpStartY, 2 );
		}
		else if ( siExpWidth > 0 )
		{
			//GP.PutSpr( &m_SummonExpSpr, ExpStartX , ExpStartY, 2 );
		}

		GP.PutSpr( &m_SummonBackGroundSpr, LifeStartX, LifeStartY, 0 );
		if ( siLifeWidth > 2 )
		{
			GP.PutSprScreen( &m_SummonLifeSpr, LifeStartX , LifeStartY, 0 );
			for ( i = 2; i < siHealLife - 2; i+=2 )
			{
				GP.PutSprScreen( &m_SummonLifeSpr, LifeStartX + i, LifeStartY, 1 );
			}
			GP.PutSprScreen( &m_SummonLifeSpr, LifeStartX + i, LifeStartY, 2 );

			GP.PutSpr( &m_SummonLifeSpr, LifeStartX , LifeStartY, 0 );
			for ( i = 2; i < siLifeWidth - 2; i+=2 )
			{
				GP.PutSpr( &m_SummonLifeSpr, LifeStartX + i, LifeStartY, 1 );
			}
			GP.PutSpr( &m_SummonLifeSpr, LifeStartX + i, LifeStartY, 2 );
		}
		else if ( siLifeWidth > 0 )
		{
			GP.PutSprScreen( &m_SummonLifeSpr, LifeStartX , LifeStartY, 2 );
			GP.PutSpr( &m_SummonLifeSpr, LifeStartX , LifeStartY, 2 );
		}

		GP.PutSpr( &m_SummonBackGroundSpr, ManaStartX, ManaStartY, 0 );
		if ( siManaWidth > 2 )
		{
			GP.PutSpr( &m_SummonManaSpr, ManaStartX , ManaStartY, 0 );
			for ( i = 2; i < siManaWidth - 2 ; i+=2 )
			{
				GP.PutSpr( &m_SummonManaSpr, ManaStartX + i, ManaStartY, 1 );
			}
			GP.PutSpr( &m_SummonManaSpr, ManaStartX + i, ManaStartY, 2 );
		}
		else if ( siManaWidth > 0 )
		{
			GP.PutSpr( &m_SummonManaSpr, ManaStartX , ManaStartY, 2 );
		}

		GP.PutSpr( &m_SummonBackGroundSpr2, StaminaStartX, StaminaStartY, 0 );
		if ( siStaminaWidth > 2 )
		{
			//스태미너 떨어졌을때는 회복게이지를 보여줌
			if (bEmptyStamina)
			{
				GP.PutSpr( &m_SummonRecoverySpr, StaminaStartX , StaminaStartY, 0 );
				for ( i = 2; i < siStaminaWidth - 2 ; i+=2 )
				{
					GP.PutSpr( &m_SummonRecoverySpr, StaminaStartX + i, StaminaStartY, 1 );
				}
				GP.PutSpr( &m_SummonRecoverySpr, StaminaStartX + i, StaminaStartY, 2 );
			}
			//스태미너 있을때는 스태미너 게이지를 보여줌
			else
			{
				GP.PutSpr( &m_SummonStaminaSpr, StaminaStartX , StaminaStartY, 0 );
				for ( i = 2; i < siStaminaWidth - 2 ; i+=2 )
				{
					GP.PutSpr( &m_SummonStaminaSpr, StaminaStartX + i, StaminaStartY, 1 );
				}
				GP.PutSpr( &m_SummonStaminaSpr, StaminaStartX + i, StaminaStartY, 2 );
			}
		}
		else if ( siStaminaWidth > 0 )
		{
			//스태미너 떨어졌을때는 회복게이지를 보여줌
			if (bEmptyStamina)
			{
				GP.PutSpr( &m_SummonRecoverySpr, StaminaStartX , StaminaStartY, 2 );
			}
			//스태미너 있을때는 스태미너 게이지를 보여줌
			else
			{
				GP.PutSpr( &m_SummonStaminaSpr, StaminaStartX , StaminaStartY, 2 );
			}
		}

		SI32 porindex = 0 ;
		//KHY - 0128 - 소환수 초상 파일 추가
		// 소환수 얼굴 넣기
		if(portrait >= 0 )
		{
			SI32 siImageIndex = 0;
			SI32 siPortraitIndex = 0;

			siImageIndex	= portrait / 51;
			siPortraitIndex = portrait % 51;

			switch(status)
			{
			case SUMMON_STATUS_TAKEOUT:
				{
					porindex = siPortraitIndex * 4 ;
				}
				break;
			case SUMMON_STATUS_TAKEIN:
				porindex = siPortraitIndex * 4 + 2 ;
				break;
			case SUMMON_STATUS_DEAD:
				porindex = siPortraitIndex * 4 + 3 ;
				break;
			default: break;
			}

			if( bEmptyStamina  )
				porindex = portrait * 4 + 1;

			if ( (siImageIndex >= 0 && siImageIndex < 2) && 
					porindex < m_SummonPortrait[siImageIndex].header.iImageNum && porindex >= 0 )
				GP.PutSpr( &m_SummonPortrait[siImageIndex], PortraitStartX,PortraitStartY, porindex);
		}


		GP.UnlockSurface(lpsurface);
	}
}

void cltPannel::SetszDate(TCHAR* szdate)
{
	StringCchCopy(m_szDate,256,szdate);
}

bool cltPannel::GetHealthText( TCHAR* pHealthText )
{
	TCHAR* pText = NULL;

	SI32 id = 1;

	if ( pclClient->pclCM->IsAlive(id) == false )
		return false;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

    SI32 status = pclchar->clPB.clHealth.AnalyzeHealth( pclchar->pclCI->clIP.GetLevel());

	if ( status & HEALTH_STATUS_ORG1_SERIOUS )
	{
		pText = GetTxtFromMgr(10003);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG2_SERIOUS )
	{
		pText = GetTxtFromMgr(10006);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG3_SERIOUS )
	{
		pText = GetTxtFromMgr(10009);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG4_SERIOUS )
	{
		pText = GetTxtFromMgr(10012);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG1_BAD )
	{
		pText = GetTxtFromMgr(10002);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG2_BAD )
	{
		pText = GetTxtFromMgr(10005);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG3_BAD )
	{
		pText = GetTxtFromMgr(10008);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG4_BAD )
	{
		pText = GetTxtFromMgr(10011);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG1_NORMAL )
	{
		pText = GetTxtFromMgr(10001);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG2_NORMAL )
	{
		pText = GetTxtFromMgr(10004);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG3_NORMAL )
	{
		pText = GetTxtFromMgr(10007);
		StringCchCopy( pHealthText, 256, pText );
	}
	else if ( status & HEALTH_STATUS_ORG4_NORMAL )
	{
		pText = GetTxtFromMgr(10010);
		StringCchCopy( pHealthText, 256, pText );
	}
	else
	{
		return false;
	}

	return true;
}
*/