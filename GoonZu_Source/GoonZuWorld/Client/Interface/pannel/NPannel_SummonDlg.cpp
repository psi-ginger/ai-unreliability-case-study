#include <CommonLogic.h>
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "NPannel_SummonDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "Char/ParametaBox/parametabox.h"
#include "Char/KindInfo/kindinfo.h"


CNPannel_SummonDlg::CNPannel_SummonDlg() : CDialogController()
{
	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_2x4_00_000.SPR"), &m_sprGage );

	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_33x30_00_000.SPR"), &m_sprSummonPortrait);
	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_32x30_00_000.SPR"), &m_sprSummonStatus);

	GP.LoadSprFromFile(TEXT("Interface/MagicMgrDlg/MagicCoolTimeSummon.SPR"), &m_sprMagicCoolTime );

	if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{	
		GP.LoadSprFromFile( TEXT("NInterface/Image/TransformBar.SPR"),	&m_sprTransformBar	);
		GP.LoadSprFromFile( TEXT("NInterface/image/TGage2.SPR"),		&m_sprTGage2		);
	}

	m_bShowDialog			= false;

	m_siMagicCoolTime		= 0;
	m_siMagicCoolTime_Start	= 0;
	m_siSummonIndex			= 0;
}


CNPannel_SummonDlg::~CNPannel_SummonDlg()
{
	GP.FreeSpr( m_sprGage );
	GP.FreeSpr( m_sprSummonPortrait );
	GP.FreeSpr( m_sprSummonStatus );
	GP.FreeSpr( m_sprMagicCoolTime );

	GP.FreeSpr( m_sprTransformBar);
	GP.FreeSpr( m_sprTGage2		 );
}


void CNPannel_SummonDlg::Create()
{
	// 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	// 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPannelDlg/DLG_Pannel_Summon.ddf" ) );
	file.CreatePopupDialog( this, PANNEL_SUMMON_DLG, TEXT( "dialog_summon" ), StaticPannel_SummonDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	PANNEL_SUMMON_SUMMON,	this ), PANNEL_SUMMON_SUMMON,	TEXT( "imagestatic_summon" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	PANNEL_SUMMON_STATUS,	this ), PANNEL_SUMMON_STATUS,	TEXT( "imagestatic_status" ) );

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void CALLBACK CNPannel_SummonDlg::StaticPannel_SummonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPannel_SummonDlg *pThis = (CNPannel_SummonDlg*) pControllerMgr;
	pThis->NPannel_SummonDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPannel_SummonDlg::NPannel_SummonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

}

void CNPannel_SummonDlg::Action()
{
	cltCharClient	*pclChar	= (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	//-- 소환수가 없을때는 본 다이얼 로그를 보여주지 않는다
	bool bShowDialog = false;

	for ( SI32 siCount=0; siCount<MAX_SUMMON_OWN; siCount++ )
	{
		cltSummon*		pclSummon	= pclChar->pclCI->clSummonInfo.GetSummon(siCount);
		if( (pclSummon == NULL) || (pclSummon->siKind <= 0) || (pclSummon->siKind >= MAX_KIND_NUMBER) )
		{
			continue;
		}

		SI32			siKind		= pclSummon->siKind;
		cltKindInfo*	pclKI		= pclClient->pclKindInfoSet->pclKI[siKind];
		if ( pclKI == NULL )
		{
			continue;
		}

		bShowDialog = true;
	}

	m_bShowDialog = bShowDialog;

}

void CNPannel_SummonDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	// 다이얼로그내에서 실제 시작위치
	SI32 siRealHPStartX			= GetX() + ScreenRX + SUMMONDLG_HP_START_X;
	SI32 siRealHPStartY			= GetY() + ScreenRY + SUMMONDLG_HP_START_Y;
	
	SI32 siRealMPStartX			= GetX() + ScreenRX + SUMMONDLG_MP_START_X;
	SI32 siRealMPStartY			= GetY() + ScreenRY + SUMMONDLG_MP_START_Y;

	SI32 siRealExpStartX		= GetX() + ScreenRX + SUMMONDLG_EXP_START_X;
	SI32 siRealExpStartY		= GetY() + ScreenRY + SUMMONDLG_EXP_START_Y;

	SI32 siRealStaminaStartX	= GetX() + ScreenRX + SUMMONDLG_STAMINA_START_X;
	SI32 siRealStaminaStartY	= GetY() + ScreenRY + SUMMONDLG_STAMINA_START_Y;

	SI32 siRealPortraitStartX	= GetX() + ScreenRX + SUMMONDLG_PORTRAIT_START_X;
	SI32 siRealPortraitStartY	= GetY() + ScreenRY + SUMMONDLG_PORTRAIT_START_Y;

	SI32 i = 0;


	for ( SI32 siCount=0; siCount<MAX_SUMMON_OWN; siCount++ )
	{
		cltSummon*		pclSummon	= pclChar->pclCI->clSummonInfo.GetSummon(siCount);

		if( (pclSummon == NULL) || (pclSummon->siKind <= 0) || (pclSummon->siKind >= MAX_KIND_NUMBER) )
		{
			continue;
		}

		SI32			siKind		= pclSummon->siKind;
		if ( false == pclClient->pclKindInfoSet->IsValidKind(siKind) )
		{
			continue;
		}

		cltKindInfo*	pclKI		= NULL;
		cltGMCommandKindInfo* pclGMki = NULL;

		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			pclKI = pclClient->pclKindInfoSet->pclSummonKI[pclSummon->siKind];
			pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
		}
		else
		{
			pclKI = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind];
			pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[pclSummon->siKind];
		}

		if ( pclKI == NULL )
		{
			continue;
		}
		
		if( pclGMki == NULL)
		{
			continue;
		}

		SI16			siAttackWearStoneNum	= pclSummon->siAttackWearStoneNum;
		SI16			siDefenseWearStoneNum	= pclSummon->siDefenseWearStoneNum;
		
		cltSummonScroll *pclScroll				= &pclSummon->clScroll;

		cltPIPInfo			clipinfo2;
		clipinfo2.Set(&pclSummon->clIP);

		cltParametaBox clPB;
		clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclKI, pclGMki);

		bool bmosterhero = false;
		// 아이템몰에서 판매하는 소환영웅이고 몬스터에서 진화한 것이라면, 
		if(pclKI->bSummonMall && pclSummon->siIdentity == 0)	
		{
			bmosterhero = true;
		}
		clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, siAttackWearStoneNum, siDefenseWearStoneNum, GAMEMODE_CLIENT, true );

		SI32 summonskill = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

		SI32 minutevary = pclClient->GetMinuteFromsTime(((cltClient*)pclClient)->sTime);
		SI32 leftsummonstart = min(TABS(pclSummon->siTakeInMinute - minutevary), SUMMON_TAKEOUT_DELAY);

		//-----------------------------------------------------------------------------
		//	소환수 초상 [시작]
		//-----------------------------------------------------------------------------
		SI32 siSummonPortrait	= pclKI->siSummonPortrait;
		SI32 siPortraitIndex	= 0;

		SI08 siSummonStatus		= pclSummon->siStatus;


		if( siSummonPortrait >= 0 )
		{
			// 먼저 기본(소환/회수)상태를 그린다
			
			// 소환상태/스테미너가 없는상태는 소환수 초상이 같음
			if ( (siSummonStatus == SUMMON_STATUS_TAKEOUT) || (pclSummon->clIP.GetLuk() <= 0) )
			{
				siPortraitIndex = (siSummonPortrait * 2);
			}
			// 회수상태/죽은상태라면 소환수 초상이 같음
			else if ( (siSummonStatus == SUMMON_STATUS_TAKEIN) || (siSummonStatus == SUMMON_STATUS_DEAD) )
			{
				siPortraitIndex = (siSummonPortrait * 2) + 1;
			}

			// 곱하고 더했기 때문에 그리기 전에 올바른 번호인지 체크
			if ( (siPortraitIndex < m_sprSummonPortrait.header.iImageNum) && (siPortraitIndex >= 0) )
			{
				GP.PutSpr( &m_sprSummonPortrait, (SUMMONDLG_PORTRAIT_WIDTH * siCount) + siRealPortraitStartX , siRealPortraitStartY, siPortraitIndex);

				// 죽은 상태라면
				if ( siSummonStatus == SUMMON_STATUS_DEAD )
				{
					GP.PutSpr( &m_sprSummonStatus, (SUMMONDLG_PORTRAIT_WIDTH * siCount) + siRealPortraitStartX , siRealPortraitStartY, 0);
				}
				// 스테미너가 없는 상태라면
				else if ( pclSummon->clIP.GetLuk() <= 0 )
				{
					GP.PutSpr( &m_sprSummonStatus, (SUMMONDLG_PORTRAIT_WIDTH * siCount) + siRealPortraitStartX , siRealPortraitStartY, 1);
				}
			}
		}	// if( siSummonPortrait >= 0 ) 종료
		//-----------------------------------------------------------------------------
		//	소환수 초상 [종료]
		//-----------------------------------------------------------------------------


		if ( pclSummon->siStatus != SUMMON_STATUS_TAKEOUT )
		{
			continue;
		}
		//-----------------------------------------------------------------------------
		//	HP 표시 [시작]
		//-----------------------------------------------------------------------------
		SI32	siNowHp			= pclSummon->clIP.GetLife();
		SI32	siMaxHp			= clPB.GetMaxLife();
		SI32	siHealHp		= pclSummon->clIP.GetLife();
		
		REAL32	f32LifeRate		= 0.0f;
		REAL32	f32HealLifeRate = 0.0f;


		if ( (siNowHp > siMaxHp) && (siMaxHp > 0) )
		{
			siNowHp = siMaxHp;
			f32LifeRate =	siNowHp / ( siMaxHp * 1.0f );
		}
		else if ( siMaxHp > 0 )
		{
			f32LifeRate	=	siNowHp / ( siMaxHp * 1.0f );
		}

		
		if ( (siHealHp > siMaxHp) && (siMaxHp > 0) )
		{
			siHealHp = siMaxHp ;
			f32HealLifeRate =	siHealHp / ( siMaxHp * 1.0f );
		}
		else if ( siHealHp > 0 )
		{
			f32HealLifeRate	=	siHealHp / ( siMaxHp * 1.0f );
		}

		SI32 siHpWidth		= (SI32)(SUMMONDLG_BAR_WIDTH * f32LifeRate);
		SI32 siHealWidth	= (SI32)(SUMMONDLG_BAR_WIDTH * f32HealLifeRate) ;

		for ( i = 0; i < siHealWidth - 2; i+=2 )
		{
			GP.PutSprScreen( &m_sprGage, siRealHPStartX + i, siRealHPStartY, SUMMONDLG_HP_NUM );
		}

		for ( i = 0; i < siHpWidth - 2; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealHPStartX + i, siRealHPStartY, SUMMONDLG_HP_NUM );
		}
		//-----------------------------------------------------------------------------
		//	HP 표시 [종료]
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		//	MP 표시 [시작]
		//-----------------------------------------------------------------------------
		SI32	siNowMp	= pclSummon->clIP.GetMana();
		SI32	siMaxMP	= clPB.GetMaxMana();

		REAL32	f32ManaRate	= 0.0f; 


		if ( (siNowMp > siMaxMP) && (siMaxMP > 0) )
		{
			siNowMp = siMaxMP;
			f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
		}
		else if ( siMaxMP > 0 )
		{
			f32ManaRate = siNowMp / ( siMaxMP * 1.0f );
		}

		SI32 siMpWidth = (SI32)(SUMMONDLG_BAR_WIDTH * f32ManaRate);

		for ( i = 0; i < siMpWidth - 2 ; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealMPStartX + i, siRealMPStartY, 1 );
		}
		//-----------------------------------------------------------------------------
		//	MP 표시 [종료]
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		//	EXP 표시 [시작]
		//-----------------------------------------------------------------------------
		SI64	siCurExp	= pclSummon->clIP.GetCurExp();// SI32->SI64 경험치 오버플로우때문에 수정
		SI64	siPreExp	= clPB.GetPrevExp();// SI32->SI64 경험치 오버플로우때문에 수정
		SI64	siNextExp	= clPB.GetNextExp();// SI32->SI64 경험치 오버플로우때문에 수정

		REAL64	f64ExpRate	= 0.0f;// SI32->SI64 경험치 오버플로우때문에 수정


		if ( siNextExp == siPreExp )
		{
			f64ExpRate = 1.0f;
		}
		else
		{
			f64ExpRate = ( siCurExp - siPreExp ) / ( ( siNextExp - siPreExp ) * 1.0f );
		}

		if ( f64ExpRate > 1.0f )
		{
			f64ExpRate = 1.0f;
		}
		else if ( f64ExpRate < 0.0f )
		{
			f64ExpRate = 0.0f;
		}
		
		// 공용 바 길이 기준
		SI32 siExpWidth = (SI32)(BASE_BAR_WIDTH * f64ExpRate);
		// 바 이미지 길이(실제 길이)
		// 와 계산 식의 바 길이(SUMMONDLG_BAR_WIDTH)가 정확히 맞아 떨어지지 않기 때문에 Flag값을 준다.
		SI32 siFlag = 2;
		// 공용 바에 대한 외부 인터페이스의 비율 
		REAL32 fBarRate = (REAL32) (SUMMONDLG_BAR_WIDTH-siFlag) / (REAL32)BASE_BAR_WIDTH;
		SI32 siTmpIncreaseValue = 0;
		for ( i = 0; i < siExpWidth  ; i+=2 )
		{
			siTmpIncreaseValue = i * fBarRate ; 	
			//홀수라면
			if((siTmpIncreaseValue % 2) == 1)
				--siTmpIncreaseValue;
			GP.PutSpr( &m_sprGage, siRealExpStartX + siTmpIncreaseValue, siRealExpStartY, SUMMONDLG_EXP_NUM );
		}
		//-----------------------------------------------------------------------------
		//	EXP 표시 [종료]
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		//	STAMINA 표시 [시작]
		//-----------------------------------------------------------------------------
		SI32	siNowStamina	= pclSummon->clIP.GetLuk();
		SI32	siMaxStamina	= pclClient->GetMaxStaminaValue(summonskill, pclSummon->siKind);
		
		REAL32	f32StaminaRate	= 0.0f;

		bool	bEmptyStamina	= false;


		if (siNowStamina <= 0)
		{
			bEmptyStamina = true;
		}

		
		if (bEmptyStamina)
		{
			// 스테미너없을때는 회복시간을
			f32StaminaRate = leftsummonstart / (SUMMON_TAKEOUT_DELAY * 1.0f );
		}
		else
		{
			// 스태미너있을때는 스테미너 양을
			if ( (siNowStamina > siMaxStamina) && (siNowStamina > 0) )
			{
				siNowStamina = siMaxStamina;
				f32StaminaRate = siNowStamina / (siMaxStamina * 1.0f );
			}
			else if ( siMaxStamina > 0)
			{
				f32StaminaRate = siNowStamina / (siMaxStamina * 1.0f );
			}
		}

		SI32 siStaminaWidth = (SI32) (SUMMONDLG_BAR_WIDTH * f32StaminaRate);

		if (bEmptyStamina)
		{
			//스태미너 떨어졌을때는 회복게이지를 보여줌
			for ( i = 0; i < siStaminaWidth - 2 ; i+=2 )
			{
				GP.PutSprScreen( &m_sprGage, siRealStaminaStartX + i, siRealStaminaStartY, SUMMONDLG_STAMINA_NUM );
			}
		}
		else
		{ 
			//스태미너 있을때는 스태미너 게이지를 보여줌
			for ( i = 0; i < siStaminaWidth - 2 ; i+=2 )
			{
				GP.PutSpr( &m_sprGage, siRealStaminaStartX + i, siRealStaminaStartY, SUMMONDLG_STAMINA_NUM );
			}
		}
		//-----------------------------------------------------------------------------
		//	STAMINA 표시 [종료]
		//-----------------------------------------------------------------------------

		//-----------------------------------------------------------------------------
		//	변신 STAMINA 표시 [시작]
		//-----------------------------------------------------------------------------
		SI32 siTransformStamina		= pclSummon->m_siTranformStamina;
		SI32 siTransformMaxStamina	= MAX_SUMMON_TRANSFORM_TIME;

		REAL32	f32TransformStaminaRate	= 0.0f;

		bool	bEmptyTransformStamina	= false;


		if (siTransformStamina <= 0)
		{
			bEmptyTransformStamina = true;
		}


		if (bEmptyTransformStamina)
		{
			// 스테미너없을때는 회복시간을
			//f32TransformStaminaRate = leftsummonstart / (SUMMON_TAKEOUT_DELAY * 1.0f );
			f32TransformStaminaRate = 0;
		}
		else
		{
			// 스태미너있을때는 스테미너 양을
			if ( (siTransformStamina > siTransformMaxStamina) && (siTransformStamina > 0) )
			{
				siTransformStamina = siTransformMaxStamina;
				f32TransformStaminaRate = siTransformStamina / (siTransformMaxStamina * 1.0f );
			}
			else if ( siTransformMaxStamina > 0)
			{
				f32TransformStaminaRate = siTransformStamina / (siTransformMaxStamina * 1.0f );
			}
		}

		SI32 siTransformStaminaWidth = (SI32) (SUMMONDLG_BAR_WIDTH * f32TransformStaminaRate);

		if (bEmptyTransformStamina)
		{
			//GP.PutSpr( &m_sprTransformBar, siRealStaminaStartX-5, siRealStaminaStartY+7, SUMMONDLG_STAMINA_NUM ); 
			//스태미너 떨어졌을때는 회복게이지를 보여줌
			for ( i = 0; i < siTransformStaminaWidth - 2 ; i+=2 )
			{
				//GP.PutSprScreen( &m_sprTGage2, siRealStaminaStartX + i, siRealStaminaStartY+10, SUMMONDLG_STAMINA_NUM ); 
			}
		}
		else
		{
			GP.PutSpr( &m_sprTransformBar, siRealStaminaStartX-5, siRealStaminaStartY+7, SUMMONDLG_STAMINA_NUM ); 
			
			//스태미너 있을때는 스태미너 게이지를 보여줌
			for ( i = 0; i < siTransformStaminaWidth - 2 ; i+=2 )
			{
				GP.PutSpr( &m_sprTGage2, siRealStaminaStartX + i, siRealStaminaStartY+10, SUMMONDLG_STAMINA_NUM );
			}
		}

		// 현재 보여지는 소환수의 배경이 위로 올라오도록 한다
		((CImageStatic*)m_InterfaceFactory.GetImageStatic(PANNEL_SUMMON_SUMMON))->SetFontIndex( siCount );

	} // for ( i = 0 ; i < MAX_SUMMON_OWN ; i ++ ) 종료


	// 마법 쿨타임 표시
	DrawMagicCoolTime( pclChar, ScreenRX, ScreenRY );
}


void CNPannel_SummonDlg::Show()
{
	CDialogController::Show( true );
}

void CNPannel_SummonDlg::Hide()
{
	CDialogController::Show( false );
}

void CNPannel_SummonDlg::DrawMagicCoolTime( cltCharClient* pclParent, SI32 ScreenRX, SI32 ScreenRY )
{
	if ( NULL == pclParent )
	{
		return;
	}

	SI16 siCoolTimeStartX	= GetX() + ScreenRX + SUMMONDLG_PORTRAIT_START_X;
	SI16 siCoolTimeStartY	= GetY() + ScreenRY + SUMMONDLG_PORTRAIT_START_Y;


	SI32 siSummonID = pclParent->GetSummonID();
	if ( 0 >= siSummonID )
	{
		return;
	}

	cltCharClient* pclSummon = pclClient->pclCM->GetCharClient( siSummonID );
	if ( NULL == pclSummon )
	{
		return;
	}

	SI32 siSummonIndex = pclSummon->GetSummonIndex();
	if ( (0 > siSummonIndex) || (MAX_SUMMON_OWN <= siSummonIndex) )
	{
		return;
	}

	/*
	if ( m_siSummonIndex != pclSummon->GetSummonIndex() )
	{
		m_siSummonIndex			= pclSummon->GetSummonIndex();
		m_siMagicCoolTime		= 0;
		m_siMagicCoolTime_Start	= 0;
	}
	*/

	SI32 siDummyValue = 0;
	

	if ( pclSummon->CanMagicManaDelay(pclSummon->GetCurrentMagic(), &siDummyValue) == true )
	{
		if ( (m_siMagicCoolTime > 0) && (m_sprMagicCoolTime.pImage != NULL) )
		{
			SI32 siDiff = TABS(pclClient->CurrentClock - m_siMagicCoolTime_Start);
			if(siDiff <= m_siMagicCoolTime)
			{
				// 위치 수정
				siCoolTimeStartX += SUMMONDLG_PORTRAIT_WIDTH*siSummonIndex;

				SI32 font = m_sprMagicCoolTime.GetImageNum() * siDiff / m_siMagicCoolTime;
				GP.PutSprScreen( &m_sprMagicCoolTime, siCoolTimeStartX, siCoolTimeStartY, font );
			}
			else
			{
				m_siMagicCoolTime = 0;
			}
		
		}
	}

}

void CNPannel_SummonDlg::SetMagicCoolTime( SI32 siMagicCoolTime )
{
	m_siMagicCoolTime		= siMagicCoolTime;
	m_siMagicCoolTime_Start	= pclClient->CurrentClock;
}