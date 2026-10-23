#include <CommonLogic.h>
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "NPannel_SoulGuardDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "Char/KindInfo/kindinfo.h"

CNPannel_SoulGuardDlg::CNPannel_SoulGuardDlg() : CDialogController()
{
	GP.LoadSprFromFile( TEXT("Interface/Pannel/IMAGE_10X16_00_000.SPR"), &m_sprGage );

	m_bShowDialog			= false;

	m_siShowFellwShip		= 0;

	m_pclEditName			= NULL;
	m_pclImagePortrait		= NULL;

}


CNPannel_SoulGuardDlg::~CNPannel_SoulGuardDlg()
{
	GP.FreeSpr( m_sprGage );

	SAFE_DELETE( m_pclEditName );
	SAFE_DELETE( m_pclImagePortrait );

	SetTimerOnOff( false );

}

void CNPannel_SoulGuardDlg::Create()
{
	// 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	// 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPannelDlg/DLG_Pannel_SoulGuard.ddf" ) );
	file.CreatePopupDialog( this, PANNEL_SOULGUARD_DLG, TEXT( "dialog_pannel_soulguard" ), StaticPannel_SoulGuardDlgProc );

	NEWCEDIT( m_pclEditName );				file.CreateControl( m_pclEditName,		PANNEL_SOULGUARD_EDITBOX_NAME,			TEXT("editbox_name") );
	NEWCIMAGESTATIC( m_pclImagePortrait );	file.CreateControl( m_pclImagePortrait,	PANNEL_SOULGUARD_IMAGESTATIC_PORTRAIT,	TEXT("imagestatic_portrait") );
	
	m_pclEditName->SetTextPrintOption( DT_VCENTER | DT_SINGLELINE );

	SetWidth( 0 );

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void CALLBACK CNPannel_SoulGuardDlg::StaticPannel_SoulGuardDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPannel_SoulGuardDlg* pThis = (CNPannel_SoulGuardDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->NPannel_SoulGuardDlgProc( nEvent, nControlID, pControl);
	}
}

void CALLBACK CNPannel_SoulGuardDlg::NPannel_SoulGuardDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

}

void CNPannel_SoulGuardDlg::Action()
{
	DisplaySoulGuardInfo();
}

void CNPannel_SoulGuardDlg::Show()
{
	CDialogController::Show( true );
}

void CNPannel_SoulGuardDlg::Hide()
{
	CDialogController::Show( false );
}

void CNPannel_SoulGuardDlg::DisplaySoulGuardInfo()
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	if ( FALSE == pclChar->IsSoulGuardStatusOut() )
	{
		m_bShowDialog = false;

		return;
	}

	SI32 siSoulGuardKind = pclChar->pclSoulGuard->GetKind();
	if ( false == pclClient->pclKindInfoSet->IsValidKind(siSoulGuardKind) )
	{
		return;
	}

	cltKindInfo* pclGetInfo = pclClient->pclKindInfoSet->pclKI[siSoulGuardKind];
	if ( NULL == pclGetInfo )
	{
		return;
	}

	// 초상화 설정
	SI16 siPortraitIndex = (pclGetInfo->siSummonPortrait * 2);
	
	m_pclImagePortrait->SetFontIndex( siPortraitIndex );

	// 이름
	m_pclEditName->SetText( pclChar->pclSoulGuard->GetName() );

	// 드로우에 쓰일 친화력설정
	m_siShowFellwShip = pclChar->pclSoulGuard->GetFellowShip();

	m_bShowDialog = true;

}

void CNPannel_SoulGuardDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( false == m_bShowDialog )
	{
		return;
	}

	SI16 siAddX			= (m_siShowFellwShip / 10);
	if ( 10 <= siAddX )
	{
		siAddX = 9;
	}
	SI16 siFondIndex	= siAddX / 2;


	SI32 siDrawStartX	= GetX() + ScreenRX + 46 + ((m_sprGage.GetXSize()-2)*siAddX);
	SI32 siDrawStartY	= GetY() + ScreenRY + 24;

	GP.PutSpr( &m_sprGage, siDrawStartX, siDrawStartY, siFondIndex );
}