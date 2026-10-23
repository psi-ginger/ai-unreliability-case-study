#include "../Client/Interface/MinigameDlg/MiniGameDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../Client/MiniGame/MiniGameManager.h"
#include "..\CommonLogic\CommonLogic.h"
#include "..\\Lib\\Graphic\\Graphic_Capsule.h"

#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;


CMiniGameDlg::CMiniGameDlg() :CDialogController()
{	
	m_pImageStatic = NULL;

	m_pMiniGame		   = NULL;
	m_pMiniGameManager = NULL;
}

CMiniGameDlg::~CMiniGameDlg()
{	
	SetTimerOnOff( false );

	if ( m_pImageStatic )
	{
		delete m_pImageStatic;
		m_pImageStatic = NULL;
	}
	if( m_pMiniGame )
	{
		delete m_pMiniGame;
		m_pMiniGame = NULL;
	}
	if( m_pMiniGameManager)
	{
		delete m_pMiniGameManager;
		m_pMiniGameManager = NULL;
	}
}

void CMiniGameDlg::Create()
{

	if ( ! IsCreate() )
	{
		// 1024 * 768
		if(gsiResolution == 1)
			CDialogController::Create( MONSTERCORPORATION_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogMiniGame, NULL, NULL, 0, false, 0, 120, 40, 722, 600, true, false );	
		else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
			CDialogController::Create( MONSTERCORPORATION_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogMiniGame, NULL, NULL, 0, false, 0, 200, 83, 722, 600, true, false );

		m_pImageStatic = new CImageStatic( this );
		m_pImageStatic->Create( MINIGAME_DIALOG_IMAGESTATIC, NULL, 0, false, 0, 0, 0, 722, 582, 722, 582 );

		m_pMiniGame = new CMiniGameFactory;

		m_pMiniGameManager = m_pMiniGame->CreateMiniGame(MINI_ZOOKEEPER);
		m_pMiniGameManager->Init();

		srand( (unsigned)time( NULL ) );

		SetTimerOnOff( true );
		SetActionTimer( 1 );

//		cltMsg clMsg( GAMEMSG_REQUEST_MONSTERCORP_START, 0, NULL );
//		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);


	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CMiniGameDlg::StaticCallBackDialogMiniGame( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CMiniGameDlg *pThis = (CMiniGameDlg*)pControllerMgr;
	pThis->CallBackDialogMiniGame( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CMiniGameDlg::CallBackDialogMiniGame( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 score = m_pMiniGameManager->GetResult();
					cltGameMsgRequest_MonsterCorp_Score clinfo(score);
					cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_SCORE,sizeof(clinfo),(BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}
				break;
			default:
				break;
			}
		}
		break;
	case MINIGAME_DIALOG_IMAGESTATIC:
		{
			switch( nEvent )
			{
			
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					POINT pt;
					m_pImageStatic->GetImageStaticPos( pt );
					m_pMiniGameManager->KeyAction( pt.x, pt.y );

				}
				break;
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					POINT pt;
					m_pImageStatic->GetImageStaticPos( pt );
				}
				break;
			}
		}
		break;
	}

	return;
}

void CMiniGameDlg::Action()
{
	if( m_pMiniGameManager )
	{
		m_pMiniGameManager->Action();
	}

}

void CMiniGameDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( m_pMiniGameManager )
		m_pMiniGameManager->Draw( ScreenRX + GetX(), ScreenRY + GetY());
}