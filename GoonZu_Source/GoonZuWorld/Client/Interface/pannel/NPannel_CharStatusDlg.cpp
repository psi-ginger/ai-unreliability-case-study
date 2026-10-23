#include "NPannel_CharStatusDlg.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "Char/ParametaBox/parametabox.h"



CNPannel_CharStatusDlg::CNPannel_CharStatusDlg() : CDialogController()
{
	m_siHungryStatus = 0;

	m_kUpdateTimer_DrawToolTip.Init(2 * 1000);	
	m_kUpdateTimer_DrawToolTip.SetActive(true, ::GetTickCount());

	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_2x10_00_000.SPR"),		&m_sprGage );
	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_8x14_00_000_1.SPR"),	&m_sprHungry );
}


CNPannel_CharStatusDlg::~CNPannel_CharStatusDlg()
{
	if ( m_sprGage.pImage )
	{
		GP.FreeSpr( m_sprGage );
	}

	if ( m_sprHungry.pImage )
	{
		GP.FreeSpr( m_sprHungry );
	}
}


void CNPannel_CharStatusDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPannelDlg/DLG_Pannel_CharStatus.ddf" ) );
	file.CreatePopupDialog( this, PANNEL_CHARSTATUS_DLG, TEXT( "dialog_charstatus" ), StaticPannel_CharStatusDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_TOOLTIP,	this ), PANNEL_CHARSTATUS_TOOLTIP,	TEXT( "statictext_tooltip" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_HUNGRY,	this ), PANNEL_CHARSTATUS_HUNGRY,	TEXT( "statictext_hungry" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_HEALTH,	this ), PANNEL_CHARSTATUS_HEALTH,	TEXT( "statictext_health" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_LEVEL,	this ), PANNEL_CHARSTATUS_LEVEL,	TEXT( "statictext_level" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_CHARNAME,	this ), PANNEL_CHARSTATUS_CHARNAME,	TEXT( "statictext_charname" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_HP,		this ), PANNEL_CHARSTATUS_HP,		TEXT( "statictext_hp" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_MP,		this ), PANNEL_CHARSTATUS_MP,		TEXT( "statictext_mp" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_EXP,		this ), PANNEL_CHARSTATUS_EXP,		TEXT( "statictext_exp" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_CHARSTATUS_FACE,		this ), PANNEL_CHARSTATUS_FACE,		TEXT( "imagestatic_face" ) );

	//-- 에디트 컨트롤 설정

	// 배고픔
	CStatic* pHungry = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HUNGRY );
	if ( pHungry != NULL )
	{
		pHungry->SetFontSize( 11 );
		pHungry->SetTextPrintOption( DT_CENTER | DT_SINGLELINE );
	}
	
	// 건강
	CStatic* pHealth = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HEALTH );
	if ( pHealth != NULL )
	{
		pHealth->SetFontSize( 11 );
		pHealth->SetTextPrintOption( DT_CENTER | DT_SINGLELINE );
		pHealth->SetFontColor( COLOR_DIALOG_YELLOW );
	}

	// 레벨
	CStatic* pLevel = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_LEVEL );
	if ( pLevel != NULL )
	{
		pLevel->SetFontWeight( 600 );
		pLevel->SetTextPrintOption( DT_CENTER | DT_SINGLELINE );
		pLevel->SetFontColor( COLOR_DIALOG_YELLOW );
	}

	// 이름
	CStatic* pCharName = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_CHARNAME );
	if ( pCharName != NULL )
	{
		pCharName->SetFontWeight( 600 );
		pCharName->SetFontColor( COLOR_DIALOG_BLUE );
	}

	// 체력
	CStatic* pHP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HP );
	if ( pHP != NULL )
	{
		pHP->SetFontSize( 11 );
		pHP->SetTextPrintOption( DT_VCENTER | DT_CENTER | DT_SINGLELINE );
		pHP->SetFontColor( COLOR_DIALOG_WHITE );
	}

	// 마력
	CStatic* pMP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_MP );
	if ( pMP != NULL )
	{
		pMP->SetFontSize( 11 );
		pMP->SetTextPrintOption( DT_VCENTER | DT_CENTER | DT_SINGLELINE );
		pMP->SetFontColor( COLOR_DIALOG_WHITE );
	}

	// 경험치
	CStatic* pEXP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_EXP );
	if ( pEXP != NULL )
	{
		pEXP->SetFontSize( 11 );
		pEXP->SetTextPrintOption( DT_VCENTER | DT_CENTER | DT_SINGLELINE );
		pEXP->SetFontColor( COLOR_DIALOG_WHITE );
	}

	// 말풍선
	CStatic* pToolTip = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_TOOLTIP );
	if ( pToolTip != NULL )
	{
		pToolTip->SetTextPrintOption( DT_VCENTER | DT_CENTER | DT_SINGLELINE );
		pToolTip->Show( false );
	}

}

void CALLBACK CNPannel_CharStatusDlg::StaticPannel_CharStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPannel_CharStatusDlg *pThis = (CNPannel_CharStatusDlg*) pControllerMgr;
	pThis->NPannel_CharStatusDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPannel_CharStatusDlg::NPannel_CharStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

}

// 이미지에 관련된 정보를 그린다
void CNPannel_CharStatusDlg::DrawCharStatus( SI32 ScreenRX, SI32 ScreenRY )
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	SI32 i;

	SI32 siNowX				= GetX();
	SI32 siNowY				= GetY();

	SI32 siRealHPStartX		= siNowX + ScreenRX + CHARSTATUS_HP_START_X;
	SI32 siRealHPStartY		= siNowY + ScreenRY + CHARSTATUS_HP_START_Y;

	SI32 siRealMPStartX		= siNowX + ScreenRX + CHARSTATUS_MP_START_X;
	SI32 siRealMPStartY		= siNowY + ScreenRY + CHARSTATUS_MP_START_Y;

	SI32 siRealEXPStartX	= siNowX + ScreenRX + CHARSTATUS_EXP_START_X;
	SI32 siRealEXPStartY	= siNowY + ScreenRY + CHARSTATUS_EXP_START_Y;


	//-----------------------------------------------------------------------------
	//	초상 표시 [시작] - 직접그리지는 않고 Index만 바꾼다
	//-----------------------------------------------------------------------------
	SI08 siFaceIndex = 0;

	switch ( pclChar->GetKind() ) 
	{
		//서양여자
		case KIND_HEROINE4:		siFaceIndex = 0;	break;
		// 동양남자
		case KIND_HERO3:		siFaceIndex = 1;	break;
		// 동양여자
		case KIND_HEROINE3:		siFaceIndex = 2;	break;
		// 서양남자
		case KIND_HERO4:		siFaceIndex = 3;	break;
	}

	((CImageStatic*)m_InterfaceFactory.GetImageStatic(PANNEL_CHARSTATUS_FACE))->SetFontIndex( siFaceIndex );

	//-----------------------------------------------------------------------------
	//	초상 표시 [종료]
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//	HP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI32	siNowHp			= pclChar->pclCI->clIP.GetLife();
	SI32	siMaxHp			= pclChar->clPB.GetMaxLife();
	SI32	siHealHp		= pclChar->pclCI->clIP.Life + pclChar->clPB.siReservedLife;

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

	SI32 siHpWidth		= (SI32)(CHARSTATUS_BAR_WIDTH * f32LifeRate);
	SI32 siHealWidth	= (SI32)(CHARSTATUS_BAR_WIDTH * f32HealLifeRate) ;

    siHpWidth	= min( CHARSTATUS_BAR_WIDTH, siHpWidth );
	siHealWidth = min( CHARSTATUS_BAR_WIDTH, siHealWidth );

	if ( siHpWidth > 2 )
	{
		GP.PutSprScreen( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM );
		for ( i = 2; i < siHealWidth - 2; i+=2 )
		{
			GP.PutSprScreen( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 1 );
		}
		GP.PutSprScreen( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 2 );

		GP.PutSpr( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM );
		for ( i = 2; i < siHpWidth - 2; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 1 );
		}
		GP.PutSpr( &m_sprGage, siRealHPStartX + i, siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
	}
	else if ( siHpWidth > 0 )
	{
		GP.PutSprScreen( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
		GP.PutSpr( &m_sprGage, siRealHPStartX , siRealHPStartY, CHARSTATUS_HP_NUM + 2 );
	}
	//-----------------------------------------------------------------------------
	//	HP 표시 [종료]
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//	MP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI32	siNowMp	= pclChar->pclCI->clIP.GetMana();
	SI32	siMaxMP	= pclChar->clPB.GetMaxMana();

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

	SI32 siMpWidth = (SI32)(CHARSTATUS_BAR_WIDTH * f32ManaRate);

	if ( siMpWidth > 2 )
	{
		GP.PutSpr( &m_sprGage, siRealMPStartX , siRealMPStartY, CHARSTATUS_MP_NUM );
		for ( i = 2; i < siMpWidth - 2 ; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealMPStartX + i, siRealMPStartY, CHARSTATUS_MP_NUM + 1 );
		}
		GP.PutSpr( &m_sprGage, siRealMPStartX + i, siRealMPStartY, CHARSTATUS_MP_NUM + 2 );
	}
	else if ( siMpWidth > 0 )
	{
		GP.PutSpr( &m_sprGage, siRealMPStartX , siRealMPStartY, CHARSTATUS_MP_NUM + 2 );
	}
	//-----------------------------------------------------------------------------
	//	MP 표시 [종료]
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	//	EXP 표시 [시작]
	//-----------------------------------------------------------------------------
	SI64	siCurExp	= pclChar->pclCI->clIP.GetCurExp();// SI32->SI64 경험치 오버플로우때문에 수정
	SI64	siPreExp	= pclChar->clPB.GetPrevExp();// SI32->SI64 경험치 오버플로우때문에 수정
	SI64	siNextExp	= pclChar->clPB.GetNextExp();// SI32->SI64 경험치 오버플로우때문에 수정

	REAL64	f32ExpRate	= 0.0f;// SI32->SI64 경험치 오버플로우때문에 수정


	if ( siNextExp == siPreExp )
	{
		f32ExpRate = 1.0f;
	}
	else
	{
		f32ExpRate = ( siCurExp - siPreExp ) / ( ( siNextExp - siPreExp ) * 1.0f );
	}

	if ( f32ExpRate > 1.0f )
	{
		f32ExpRate = 1.0f;
	}
	else if ( f32ExpRate < 0.0f )
	{
		f32ExpRate = 0.0f;
	}

	SI32 siExpWidth = (SI32)(CHARSTATUS_BAR_WIDTH * f32ExpRate);

	if ( siExpWidth > 2 )
	{
		GP.PutSpr( &m_sprGage, siRealEXPStartX , siRealEXPStartY, CHARSTATUS_EXP_NUM );
		for ( i = 2; i < siExpWidth - 2 ; i+=2 )
		{
			GP.PutSpr( &m_sprGage, siRealEXPStartX + i, siRealEXPStartY, CHARSTATUS_EXP_NUM + 1 );
		}
		GP.PutSpr( &m_sprGage, siRealEXPStartX + i, siRealEXPStartY, CHARSTATUS_EXP_NUM + 2 );
	}
	else if ( siExpWidth > 0 )
	{
		GP.PutSpr( &m_sprGage, siRealEXPStartX , siRealEXPStartY, CHARSTATUS_EXP_NUM + 2 );
	}
	//-----------------------------------------------------------------------------
	//	EXP 표시 [종료]
	//-----------------------------------------------------------------------------

}

// 글자에 관련된 정보를 그린다
void CNPannel_CharStatusDlg::WriteCharStatus()
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	// 배고픔
	CStatic* pHungry = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HUNGRY );
	if ( pHungry != NULL )
	{
		NTCHARString16 kHungry(TEXT(""));

		// 양의 값일때 앞에 '+'를 붙여준다
		if ( pclChar->pclCI->clHealthInfo.siHungry >= 0 )
		{
			if ( pclChar->pclCI->clHealthInfo.siHungry > 0 )
			{
				kHungry = TEXT("+");
			}
			pHungry->SetFontColor( COLOR_DIALOG_YELLOW );
		}
		else
		{
			pHungry->SetFontColor( RGB(255, 0, 0) );
		}
		
		kHungry += SI32ToString( pclChar->pclCI->clHealthInfo.siHungry );
		
		pHungry->SetText( kHungry );
	}

	// 건강 수치
	CStatic* pHealth = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HEALTH );
	if ( pHealth != NULL )
	{
		NTCHARString16 kHealth;
		
		kHealth = SI32ToString( (SI32)pclChar->pclCI->clHealthInfo.uiHealthPoint );
		
		pHealth->SetText( kHealth );
	}

	// 레벨
	CStatic* pLevel = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_LEVEL );
	if ( pLevel != NULL )
	{
		NTCHARString16 kLevel;
		
		kLevel.FormatString( TEXT("Lv.%d"), pclChar->pclCI->clIP.GetLevel() );
		
		pLevel->SetText( kLevel );
	}

	// 케릭터 이름
	CStatic* pCharName = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_CHARNAME );
	if ( pCharName != NULL )
	{
		NTCHARString64 kCharName;

		kCharName = pclChar->GetName();

		pCharName->SetText( kCharName );
	}

	// 체력
	CStatic* pHP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_HP );
	if ( pHP != NULL ) 
	{
		NTCHARString64 kHP;
		SI32			siNowHP = pclChar->GetLife();
		SI32			siMaxHP = pclChar->clPB.GetMaxLife();

		kHP.FormatString( TEXT("%d / %d"), siNowHP, siMaxHP );

		pHP->SetText( kHP );
	}

	// 마력
	CStatic* pMP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_MP );
	if ( pMP != NULL ) 
	{
		NTCHARString64	kMP;
		SI32			siNowMP = pclChar->GetMana();
		SI32			siMaxMP = pclChar->clPB.GetMaxMana();

		kMP.FormatString( TEXT("%d / %d"), siNowMP, siMaxMP );

		pMP->SetText( kMP );
	}

	// 경험치
	CStatic* pEXP = (CStatic*)m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_EXP );
	if ( pEXP != NULL ) 
	{
		NTCHARString64	kEXP;
		SI64			siCurEXP	= pclChar->pclCI->clIP.GetCurExp();// SI32->SI64 경험치 오버플로우때문에 수정
		SI64			siPrevExp	= pclChar->clPB.GetPrevExp();// SI32->SI64 경험치 오버플로우때문에 수정
		SI64			siNextEXP	= pclChar->clPB.GetNextExp();// SI32->SI64 경험치 오버플로우때문에 수정
		
		REAL64			fExpPercent = (siCurEXP - siPrevExp) / (REAL64)(siNextEXP - siPrevExp) * 100;// SI32->SI64 경험치 오버플로우때문에 수정


		if ( fExpPercent < 0.0f )
		{
			fExpPercent = 0.0f;
		}

		kEXP.FormatString( TEXT("%3.2f%%"), fExpPercent );

		pEXP->SetText( kEXP );
	}

}

void CNPannel_CharStatusDlg::DrawHungry( SI32 ScreenRX, SI32 ScreenRY )
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	SI32 siRealHungryStartX	= GetX() + ScreenRX + CHARSTATUS_HUNGRY_START_X;
	SI32 siRealHungryStartY	= GetY() + ScreenRY + CHARSTATUS_HUNGRY_START_Y;

	SI32 siHungryStatus		= pclChar->clPB.clHealth.GetHungryStatus(pclChar->pclCI->clIP.GetLevel());
	SI32 siRealStatus		= 0;

	SI32 siPosition			= 0;
	SI32 siFontIndex		= 0;

	SI32 siCorrectionNum	= 0;


	// 읔 노가다 ㅠ_ㅠ
	if  ( siHungryStatus & HEALTH_STATUS_HUNGRY_1 )
	{
		siPosition		= 8;
		siFontIndex		= 0;
		siRealStatus	|= HEALTH_LEPLETE;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_2 )
	{
		siPosition		= 7;
		siFontIndex		= 1;
		siRealStatus	|= HEALTH_LEPLETE;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_3 )
	{
		siPosition		= 6;
		siFontIndex		= 2;
		siRealStatus	|= HEALTH_LEPLETE;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_4 )
	{	
		siPosition	= 5;
		siFontIndex	= 3;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_5 )
	{	
		siPosition	= 4;
		siFontIndex	= 4;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_6 )
	{	
		siPosition	= 3;
		siFontIndex	= 3;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_7 )
	{
		siPosition		= 2;
		siFontIndex		= 2;
		siRealStatus	|= HEALTH_HUNGRY;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_8 )
	{
		siPosition		= 1;
		siFontIndex		= 1;
		siRealStatus	|= HEALTH_HUNGRY;
	}
	else if ( siHungryStatus & HEALTH_STATUS_HUNGRY_9 )
	{
		siPosition		= 0;
		siFontIndex		= 0;
		siRealStatus	|= HEALTH_HUNGRY;
	}

	siCorrectionNum		= 4 - siPosition;
	// 툴팁 표시될때 쓰일 상태
	m_siHungryStatus	= siRealStatus;


	GP.PutSpr(&m_sprHungry, ( siRealHungryStartX + (CHARSTATUS_HUNGRY_WIDTH * siPosition) ) + siCorrectionNum, siRealHungryStartY, siFontIndex );
}

void CNPannel_CharStatusDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	WriteCharStatus();
	WriteToolTip();
}

void CNPannel_CharStatusDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	DrawCharStatus( ScreenRX, ScreenRY );
	DrawHungry( ScreenRX, ScreenRY );
}

void CNPannel_CharStatusDlg::Show()
{
	CDialogController::Show( true );
}

void CNPannel_CharStatusDlg::Hide()
{
	CDialogController::Show( false );
}

bool CNPannel_CharStatusDlg::GetToolTipText( TCHAR* pszToolTip )
{
	if ( NULL == pszToolTip )
	{
		return false;
	}

	SI32 siID = 1;
	if ( false == pclClient->pclCM->IsAlive(siID) )
	{
		return false;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[siID];
	if ( NULL == pclchar )
	{
		return false;
	}

	bool	bReturnValue						= true;
	SI32	siHealthTextNum						= 0;
	TCHAR	szDiseaseText[CHARSTATUS_BUF_SIZE]	= TEXT("");

	// 병에 걸렸을 때
	/*if ( true == pclchar->clPB.clHealth.GetCurDiseaseName(szDiseaseText, CHARSTATUS_BUF_SIZE) )
	{
		TCHAR* pText = GetTxtFromMgr( 5889 );

		if ( pText )
		{
			StringCchPrintf( pszToolTip, CHARSTATUS_BUF_SIZE, pText, szDiseaseText );
		}
	}*/


	// 병에 걸렸을 때
	if ( true == pclchar->clPB.clHealth.GetCurDiseaseName(szDiseaseText, CHARSTATUS_BUF_SIZE) )
	{
		TCHAR* pText = GetTxtFromMgr( 5889 );

		if ( pText )
		{
			StringCchPrintf( pszToolTip, CHARSTATUS_BUF_SIZE, pText, szDiseaseText );
		}
	}

	// 건강상태가 좋지 않을 때
	else if ( (siHealthTextNum = GetHealthTextNum( )) )
	{
		TCHAR* pText = GetTxtFromMgr( siHealthTextNum );

		if ( pText )
		{
			StringCchCopy( pszToolTip, CHARSTATUS_BUF_SIZE, pText );
		}
	}
	// 배가 고플 때
	else if ( m_siHungryStatus & HEALTH_HUNGRY )
	{
		TCHAR* pText = GetTxtFromMgr( 5828 );

		if ( pText )
		{
			StringCchCopy( pszToolTip, CHARSTATUS_BUF_SIZE, pText );
		}
	}
	// 배가 부를 때
	else if ( m_siHungryStatus & HEALTH_LEPLETE )
	{
		TCHAR* pText = GetTxtFromMgr( 5829 );

		if ( pText )
		{
			StringCchCopy( pszToolTip, CHARSTATUS_BUF_SIZE, pText );
		}
	}
	else
	{
		bReturnValue = false;
	}


	return bReturnValue;
}

SI32 CNPannel_CharStatusDlg::GetHealthTextNum( void )
{
	SI32 siID = 1;
	if ( false == pclClient->pclCM->IsAlive(siID) )
	{
		return 0;
	}

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[siID];
	if ( NULL == pclchar )
	{
		return 0;
	}

	SI32 siStauts				= pclchar->clPB.clHealth.AnalyzeHealth( pclchar->pclCI->clIP.GetLevel() );
	SI32 siReturnHealthTextNum	= 0;

	if ( siStauts & HEALTH_STATUS_ORG1_SERIOUS )
	{
		siReturnHealthTextNum = 10003;
	}
	else if ( siStauts & HEALTH_STATUS_ORG2_SERIOUS )
	{
		siReturnHealthTextNum = 10006;
	}
	else if ( siStauts & HEALTH_STATUS_ORG3_SERIOUS )
	{
		siReturnHealthTextNum = 10009;
	}
	else if ( siStauts & HEALTH_STATUS_ORG4_SERIOUS )
	{
		siReturnHealthTextNum = 10012;
	}
	else if ( siStauts & HEALTH_STATUS_ORG1_BAD )
	{
		siReturnHealthTextNum = 10002;
	}
	else if ( siStauts & HEALTH_STATUS_ORG2_BAD )
	{
		siReturnHealthTextNum = 10005;
	}
	else if ( siStauts & HEALTH_STATUS_ORG3_BAD )
	{
		siReturnHealthTextNum = 10008;
	}
	else if ( siStauts & HEALTH_STATUS_ORG4_BAD )
	{
		siReturnHealthTextNum = 10011;
	}
	else if ( siStauts & HEALTH_STATUS_ORG1_NORMAL )
	{
		siReturnHealthTextNum = 10001;
	}
	else if ( siStauts & HEALTH_STATUS_ORG2_NORMAL )
	{
		siReturnHealthTextNum = 10004;
	}
	else if ( siStauts & HEALTH_STATUS_ORG3_NORMAL )
	{
		siReturnHealthTextNum = 10007;
	}
	else if ( siStauts & HEALTH_STATUS_ORG4_NORMAL )
	{
		siReturnHealthTextNum = 10010;
	}


	return siReturnHealthTextNum;
}

void CNPannel_CharStatusDlg::WriteToolTip( void )
{
	if ( false == m_kUpdateTimer_DrawToolTip.IsTimed(GetTickCount( )) )
	{
		return;
	}

	CStatic* pToolTipStatic = m_InterfaceFactory.GetStatic( PANNEL_CHARSTATUS_TOOLTIP );
	if ( NULL == pToolTipStatic )
	{
		return;
	}

	// 현재 보여지는 상태라면 보여지지 않음
	if ( pToolTipStatic->IsShow() )
	{
		pToolTipStatic->Show( false );
	}
	// 보여지지 않을때만 보여질 수 있는지 검사
	else
	{
		TCHAR	szToolTip[CHARSTATUS_BUF_SIZE]	= { '\0', };

		if ( true == GetToolTipText(szToolTip) )
		{
			pToolTipStatic->SetText( szToolTip );
			pToolTipStatic->Show( true );
		}
	}

}