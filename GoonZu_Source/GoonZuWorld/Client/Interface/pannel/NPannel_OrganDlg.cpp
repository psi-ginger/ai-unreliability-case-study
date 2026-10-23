#include "NPannel_OrganDlg.h"
#include "NPannel_CharStatusDlg.h"
#include "NPannel_SummonDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client.h"
#include "Char/CharManager/CharManager.h"
#include "Char/ParametaBox/parametabox.h"




CNPannel_OrganDlg::CNPannel_OrganDlg() : CDialogController()
{

}


CNPannel_OrganDlg::~CNPannel_OrganDlg()
{

}


void CNPannel_OrganDlg::Create()
{
	// 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	// 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPannelDlg/DLG_Pannel_Organ.ddf" ) );
	file.CreatePopupDialog( this, PANNEL_ORGAN_DLG, TEXT( "dialog_organ" ), StaticPannel_OrganDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_ORG1,	this ), PANNEL_ORGAN_ORG1,	TEXT( "statictext_org1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_ORG2,	this ), PANNEL_ORGAN_ORG2,	TEXT( "statictext_org2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_ORG3,	this ), PANNEL_ORGAN_ORG3,	TEXT( "statictext_org3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_ORG4,	this ), PANNEL_ORGAN_ORG4,	TEXT( "statictext_org4" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_MAXORG1,	this ), PANNEL_ORGAN_MAXORG1,	TEXT( "statictext_maxorg1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_MAXORG2,	this ), PANNEL_ORGAN_MAXORG2,	TEXT( "statictext_maxorg2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_MAXORG3,	this ), PANNEL_ORGAN_MAXORG3,	TEXT( "statictext_maxorg3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_MAXORG4,	this ), PANNEL_ORGAN_MAXORG4,	TEXT( "statictext_maxorg4" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_NAME_ORG1,	this ), PANNEL_ORGAN_NAME_ORG1,	TEXT( "statictext_name_org1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_NAME_ORG2,	this ), PANNEL_ORGAN_NAME_ORG2,	TEXT( "statictext_name_org2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_NAME_ORG3,	this ), PANNEL_ORGAN_NAME_ORG3,	TEXT( "statictext_name_org3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_ORGAN_NAME_ORG4,	this ), PANNEL_ORGAN_NAME_ORG4,	TEXT( "statictext_name_org4" ) );

	if ( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
	{
		// 기본 설정 (폰트)
		((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_NAME_ORG1))->SetFontKind( TEXT("TimeNTales") );
		((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_NAME_ORG2))->SetFontKind( TEXT("TimeNTales") );
		((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_NAME_ORG3))->SetFontKind( TEXT("TimeNTales") );
		((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_NAME_ORG4))->SetFontKind( TEXT("TimeNTales") );

	}

	// 텍스트 정렬 방식
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_ORG1))->SetTextPrintOption( DT_RIGHT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_ORG2))->SetTextPrintOption( DT_RIGHT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_ORG3))->SetTextPrintOption( DT_RIGHT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_ORG4))->SetTextPrintOption( DT_RIGHT | DT_SINGLELINE );

	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_MAXORG1))->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_MAXORG2))->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_MAXORG3))->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );
	((CStatic*)m_InterfaceFactory.GetStatic(PANNEL_ORGAN_MAXORG4))->SetTextPrintOption( DT_LEFT | DT_SINGLELINE );

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void CALLBACK CNPannel_OrganDlg::StaticPannel_OrganDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPannel_OrganDlg *pThis = (CNPannel_OrganDlg*) pControllerMgr;
	pThis->NPannel_OrganDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPannel_OrganDlg::NPannel_OrganDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

}

void CNPannel_OrganDlg::Action()
{
	//SI32 siNowOrgan[4] = { PANNEL_ORGAN_ORG1, PANNEL_ORGAN_ORG2, PANNEL_ORGAN_ORG3, PANNEL_ORGAN_ORG4  };
	//SI32 siMaxOrgan[4] = { PANNEL_ORGAN_MAXORG1, PANNEL_ORGAN_MAXORG2, PANNEL_ORGAN_MAXORG3, PANNEL_ORGAN_MAXORG4  };
	//SI32 siOrganType[4] = { HEALTH_ORGAN_TYPE_ORG1, HEALTH_ORGAN_TYPE_ORG2, HEALTH_ORGAN_TYPE_ORG3, HEALTH_ORGAN_TYPE_ORG4 };

	Magnet();

	for ( SI32 siCount=0; siCount<4; siCount++ )
	{
		//WriteOrgan( siNowOrgan[siCount], siMaxOrgan[siCount], siOrganType[siCount] );
		WriteOrgan( PANNEL_ORGAN_ORG1+siCount, PANNEL_ORGAN_MAXORG1+siCount, HEALTH_ORGAN_TYPE_ORG1+siCount );
	}
}

void CNPannel_OrganDlg::WriteOrgan( SI32 siNowOrganController, SI32 siMaxOrganController, SI32 siOrganType )
{
	cltCharClient* pclChar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclChar == NULL )
	{
		return;
	}

	SI32 siLevel	= pclChar->pclCI->clIP.GetLevel();
	SI32 siOrgMax	= min(100, MAXORG(siLevel));

	CStatic* pOrg		= (CStatic*)m_InterfaceFactory.GetStatic( siNowOrganController );
	CStatic* pMaxOrg	= (CStatic*)m_InterfaceFactory.GetStatic( siMaxOrganController );

	if ( (pOrg == NULL) || (pMaxOrg == NULL))
	{
		return;
	}

	NTCHARString64	kMaxOrg;
	SI32			siOrg = pclChar->pclCI->clHealthInfo.GetOrg( siOrganType );

	// 현재 값
	if ( siOrg < 0 )
	{
		pOrg->SetFontColor( RGB(255,0,0) );
	}
	else if ( siOrg == siOrgMax )
	{
		pOrg->SetFontColor( RGB(0,0,255) );
	}
	else
	{
		pOrg->SetFontColor( COLOR_DIALOG_BLACK );
	}
	pOrg->SetText( SI32ToString( siOrg ) );

	// 최대값
	kMaxOrg.FormatString( TEXT("/%d"), siOrgMax );
	pMaxOrg->SetText( kMaxOrg );


}

void CNPannel_OrganDlg::Show()
{
	CDialogController::Show( true );
}

void CNPannel_OrganDlg::Hide()
{
	CDialogController::Show( false );
}

void CNPannel_OrganDlg::Magnet()
{
	cltClient *pclclient = (cltClient*)pclClient;
	if ( pclclient == NULL )
	{
		return;
	}

	//-- 자석모드
	CNPannel_SummonDlg		*pclSummonDlg		= (CNPannel_SummonDlg*)pclclient->m_pDialog[PANNEL_SUMMON_DLG];
	CNPannel_CharStatusDlg	*pclCharStatusDlg	= (CNPannel_CharStatusDlg*)pclclient->m_pDialog[PANNEL_CHARSTATUS_DLG];

	if ( (pclSummonDlg == NULL) || (pclCharStatusDlg == NULL) )
	{
		return;
	}

	SI16 siSnapRange	= 10;
	
	// 보정값
	SI16 siCorrectionX	= 1;
	SI16 siCorrectionY	= 1;
	// 대상이 되는 위치
	SI16 siSourceX		= pclSummonDlg->GetX() + pclSummonDlg->GetWidth() + siCorrectionX;
	SI16 siSourceY		= pclCharStatusDlg->GetY() + pclCharStatusDlg->GetHeight() + siCorrectionY;
	// 대상과의 거리
	SI16 siDistanceX	= GetX() - siSourceX;
	SI16 siDistanceY	= GetY() - siSourceY;

	if(abs(siDistanceY) < siSnapRange )
	{
		MovePos(GetX(),siSourceY);
	}	

	if(abs(siDistanceX) < siSnapRange )
	{
		MovePos(siSourceX,GetY());
	}


}
