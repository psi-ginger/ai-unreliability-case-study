#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "../../common/Char/CharManager/CharManager.h"
#include "NServerEventIndunInfo.h"



CNServerEventIndunInfoDlg::CNServerEventIndunInfoDlg() : CDialogController(), m_dwRemainTime(0)
{
}

CNServerEventIndunInfoDlg::~CNServerEventIndunInfoDlg()
{
}

void CNServerEventIndunInfoDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NServerEvent/NServerEventIndunInfo.ddf" ) );

	file.CreatePopupDialog( this, SERVER_EVENT_INDUNINFO, TEXT( "dialog_servereventinduninfo" ), StaticServerEventIndunInfoDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_REMAINTIME_STATIC,		this ), SERVER_EVENT_REMAINTIME_STATIC,		TEXT( "statictext_remaintime" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_REMAINMONSTER_STATIC,	this ), SERVER_EVENT_REMAINMONSTER_STATIC,	TEXT( "statictext_remainmonster" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_SCORE_STATIC,			this ), SERVER_EVENT_SCORE_STATIC,			TEXT( "statictext_score" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_INDUNSCORE_STATIC,		this ), SERVER_EVENT_INDUNSCORE_STATIC,		TEXT( "statictext_indunscore" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_CLEARSTEP_STATIC,		this ), SERVER_EVENT_CLEARSTEP_STATIC,		TEXT( "statictext_clearstep" ) );

	// 정가운데로
	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclclient )
	{
		SI16 siPosX = (pclclient->GetScreenXSize() - GetWidth() + MAP_TILE_XSIZE) / 2;
		SI16 siPosY = 100;

		SetDialogPos( siPosX, siPosY, 0, 0 );
	}

	for ( SI32 siIndex=0; siIndex<=(SERVER_EVENT_CLEARSTEP_STATIC-SERVER_EVENT_REMAINTIME_STATIC); siIndex++ )
	{
		CStatic* pStatic = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_REMAINTIME_STATIC + siIndex );
		if ( pStatic )
		{
			pStatic->SetFontColor( RGB(0xff, 0xff, 0xff) );
		}
	}

	m_InterfaceFactory.GetStatic( SERVER_EVENT_CLEARSTEP_STATIC )->SetFontSize( 50 );
	m_InterfaceFactory.GetStatic( SERVER_EVENT_CLEARSTEP_STATIC )->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	SetAlphaMode( 10 );

	Hide();

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

}

void CNServerEventIndunInfoDlg::Action()
{
	if ( INSTANCEMAP_TYPE_SERVER != pclClient->pclMapManager->GetInstanceMapType(pclClient->pclCM->CR[1]->GetMapIndex()) )
	{
		DeleteDialog();
	}
}

void CALLBACK CNServerEventIndunInfoDlg::StaticServerEventIndunInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNServerEventIndunInfoDlg *pThis = (CNServerEventIndunInfoDlg*) pControllerMgr;
	pThis->NServerEventIndunInfoDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNServerEventIndunInfoDlg::NServerEventIndunInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	return;
}

void CNServerEventIndunInfoDlg::Show()
{
	CDialogController::Show( true );
}

void CNServerEventIndunInfoDlg::Hide()
{
	CDialogController::Show( false );
}

void CNServerEventIndunInfoDlg::SetServerEventIndunInfo( SI16 siServerUnique, SI32 siClearStep, DWORD dwRemainTime, SI32 siRemainMonster, SI64 siScore, SI32 siDungeonScore )
{
	// 다이얼로그 배경설정
	if ( (0 < siServerUnique) && (4 >= siServerUnique) )
	{
		SetFontIndex( siServerUnique-1 );
	}
	else
	{
		SetFontIndex( 0 );
	}

	// 클리어 단계 표시
	CStatic* pClearStep = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_CLEARSTEP_STATIC );
	if ( pClearStep )
	{
		pClearStep->SetText( SI32ToString(siClearStep) );
	}

	// 남은시간
	CStatic* pRemainTime = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_REMAINTIME_STATIC );
	if ( pRemainTime )
	{
		NTCHARString256 strRemainTime( GetTxtFromMgr(8543) );

		strRemainTime.Replace( TEXT("#remaintime#"), SI32ToString((SI32)(dwRemainTime/1000)) );
		pRemainTime->SetText( strRemainTime );
	}

	// 남은 몬스터
	CStatic* pRemainMonster = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_REMAINMONSTER_STATIC );
	if ( pRemainMonster )
	{
		NTCHARString256 strRemainMonster( GetTxtFromMgr(8544) );

		strRemainMonster.Replace( TEXT("#remainmonster#"), SI32ToString(siRemainMonster) );
		pRemainMonster->SetText( strRemainMonster );
	}

	// 내점수
	CStatic* pScore = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_SCORE_STATIC );
	if( pScore )
	{
		NTCHARString256 strScore( GetTxtFromMgr(8545) );

		strScore.Replace( TEXT("#score#"), SI64ToString(siScore) );
		pScore->SetText( strScore );
	}

	// 사냥터 점수
	CStatic* pIndunScore = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_INDUNSCORE_STATIC );
	if ( pIndunScore )
	{
		NTCHARString256 strIndunScore( GetTxtFromMgr(8546) );

		strIndunScore.Replace( TEXT("#indunscore#"), SI32ToString(siDungeonScore) );
		pIndunScore->SetText( strIndunScore );
	}

	Show();
}
