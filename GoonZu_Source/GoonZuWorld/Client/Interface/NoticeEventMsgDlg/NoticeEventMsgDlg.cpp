#include "NoticeEventMsgDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CNoticeEvnetMsgDlg::CNoticeEvnetMsgDlg() : CDialogController()
{	
	m_siGlobalImageIndex = -1;
}

CNoticeEvnetMsgDlg::~CNoticeEvnetMsgDlg()
{
}

void CNoticeEvnetMsgDlg::Create()
{
	CDialogController::Create( NOTICEEVENTMSG_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNoticeEventMsg, TEXT("공지 사항"), TEXT("Interface/NoticeEventMsgDlg/NoticeEventMsgDlg.Spr"), 0, false, 0, 425, 100, 276, 368 );
	
	Hide();

	return;
}

void CALLBACK CNoticeEvnetMsgDlg::StaticCallBackDialogNoticeEventMsg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNoticeEvnetMsgDlg *pThis = (CNoticeEvnetMsgDlg*)pControllerMgr;
	pThis->CallBackDialogNoticeEventMsg( nEvent, nControlID, pControl );
}

void CALLBACK CNoticeEvnetMsgDlg::CallBackDialogNoticeEventMsg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
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
	}

	return;
}

void CNoticeEvnetMsgDlg::Hide()
{
	CControllerMgr::Show( false );
	m_siGlobalImageIndex = -1;
	
	return;
}

void CNoticeEvnetMsgDlg::Show()
{
	CControllerMgr::Show( true );

	return;
}

void CNoticeEvnetMsgDlg::SetGlobalImage( TCHAR *Title, SI32 GlobalImageIndex )
{
	Show();

	CNoticeEvnetMsgDlg::SetTitle( Title );

	m_siGlobalImageIndex = GlobalImageIndex;
	
	return;
}

void CNoticeEvnetMsgDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( m_siGlobalImageIndex != -1 )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siGlobalImageIndex );
		
		if ( pSpr )
		{
			GP.PutSpr(pSpr, ScreenRX + GetX() + 5, ScreenRY + GetY() + 23, 0 );
		}
	}	
}

