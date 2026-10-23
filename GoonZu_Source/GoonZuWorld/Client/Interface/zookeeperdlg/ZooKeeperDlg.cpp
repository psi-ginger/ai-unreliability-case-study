#include "MiniGameDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

#define ZOOKEEPER_DIALOG					500
#define ZOOKEEPER_DIALOG_CLOSE_BUTTON		501
#define ZOOKEEPER_DIALOG_IMAGESTATIC		502

CMiniGameDlg::CMiniGameDlg()
{
	m_pCloseButton = NULL;
	m_pImageStatic = NULL;
}

CMiniGameDlg::~CMiniGameDlg()
{
	if ( m_pCloseButton )
	{
		delete m_pCloseButton;
		m_pCloseButton = NULL;
	}
	if ( m_pImageStatic )
	{
		delete m_pImageStatic;
		m_pImageStatic = NULL;
	}
}

void CMiniGameDlg::Create()
{
	CDialogController::Create( ZOOKEEPER_DIALOG, StaticCallBackDialogZooKeeper, "ÁêÅ°ÆÛ", "Interface/NoticeEventMsgDlg/NoticeEventMsgDlg.Spr", 425, 100, 276, 368 );

	m_pCloseButton = new CButton( this );
	m_pCloseButton->Create( ZOOKEEPER_DIALOG_CLOSE_BUTTON, NULL, "Interface/Common/CloseButton.Spr", 258, 5, 14, 13, 14, 13 );

	m_pImageStatic = new CImageStatic( this );
	m_pImageStatic->Create( ZOOKEEPER_DIALOG_IMAGESTATIC, NULL, 0, 20, 100, 100, 100, 100 );

	Hide();
	return;
}

void CALLBACK CMiniGameDlg::StaticCallBackDialogZooKeeper( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CMiniGameDlg *pThis = (CMiniGameDlg*)pControllerMgr;
	pThis->CallBackDialogZooKeeper( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CMiniGameDlg::CallBackDialogZooKeeper( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case NOTIFYMSG_DIALOG_CLOSE_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Hide();
				}
				break;
			default:
				break;
			}
		}
		break;
	case ZOOKEEPER_DIALOG_IMAGESTATIC:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					POINT pt;
					m_pImageStatic->GetImageStaticPos( pt );

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

void CMiniGameDlg::Hide()
{
	CControllerMgr::Show( false );

	return;
}

void CMiniGameDlg::Show()
{
	CControllerMgr::Show( true );

	return;
}

void CMiniGameDlg::DrawDialog( SI32 ScreenRX, SI32 ScreenRY )
{
	return;
}