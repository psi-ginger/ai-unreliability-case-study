#include "NWeddingAnniversaryDlg.h"

#include "../../Client.h"

#include "../../common/Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"


extern cltCommonLogic* pclClient;

CNWeddingAnniversaryDlg::CNWeddingAnniversaryDlg() : CDialogController()
{
	m_pclGetItem = NULL;

	m_updateTimer.Init( 1000 * 30 ); // 30초후에 종료된다
}

CNWeddingAnniversaryDlg::~CNWeddingAnniversaryDlg()
{
	SAFE_DELETE( m_pclGetItem );
}

void CNWeddingAnniversaryDlg::Init()
{

}

void CNWeddingAnniversaryDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		DeleteDialog();
		return;
	}

	SI16 siDialogWidth	= 200;
	SI16 siDialogHeight	= 200;

	SI16 siDialogXpos	= (pclclient->GetScreenXSize() - siDialogWidth + MAP_TILE_XSIZE) / 2;
	SI16 siDialogYpos	= 100;

	// 다이얼로그 생성
	CDialogController::Create( NWEDDINGANNIVERSARY_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNWeddingAnniversary, NULL, NULL, 0, false, 0, siDialogXpos, siDialogYpos, siDialogWidth, siDialogHeight, false, false, false );

	// 아이템 받기 버튼
	NEWCBUTTON( m_pclGetItem );
	m_pclGetItem->Create( 0, TEXT(""), TEXT("NInterface/Button/BUTTON_200x200_00_000.SPR"), 0, false, 0, 0, 0, siDialogWidth, siDialogHeight, siDialogWidth, siDialogHeight );

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	m_updateTimer.SetActive( true, GetTickCount() );

}

void CNWeddingAnniversaryDlg::Action()
{
	if ( true == m_updateTimer.IsTimed(GetTickCount(), false) )
	{
		DeleteDialog();
	}
}

void CALLBACK CNWeddingAnniversaryDlg::StaticCallBackDialogNWeddingAnniversary( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWeddingAnniversaryDlg* pThis = (CNWeddingAnniversaryDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNWeddingAnniversary( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNWeddingAnniversaryDlg::CallBackDialogNWeddingAnniversary( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case 0:// 받기버튼 클릭
		{
			OnClickButtonGetItem();
		}
		break;
	}
}

void CNWeddingAnniversaryDlg::OnClickButtonGetItem( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 결혼도 안했는데 이버튼을 클릭했다면
	if ( FALSE == pclchar->IsMarried() )
	{
		return;
	}

	// 아이템 받기 요청
	cltMsg clMsg( GAMEMSG_REQUEST_WEDDINGANNIVERSARY_ITEMGET, 0 );
	pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
	
}