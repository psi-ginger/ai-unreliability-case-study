#include "../Client/NInterface/NYezoDlg/NYezoAngelApplyerDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../CommonLogic/MsgType-System.h"

extern cltCommonLogic* pclClient;

CNYezoAngelApplyerDlg::CNYezoAngelApplyerDlg() : CDialogController()
{
	m_pStatic	= NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pOutLine = NULL;
}

CNYezoAngelApplyerDlg::~CNYezoAngelApplyerDlg()
{
	SAFE_DELETE(m_pStatic);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);

	SAFE_DELETE(m_pOutLine);
}

void CNYezoAngelApplyerDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NYezoDlg/DLG_Yezo_AngelApplyer.ddf"));
	file.CreateChildDialog( this, NYEZOANGELAPPYER_DLG, TEXT("dialog_Yezo_AngelApplyer"), StaticCallBackDialogNYezoAngelApplyerDlg, pParent );
	
	NEWCSTATIC(m_pStatic);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);

	NEWCOUTLINE(m_pOutLine);
	
	file.CreateControl( m_pStatic, NYEZOANGELAPPLYER_DIALOG_STATIC, TEXT("statictext_NONAME1") );

	file.CreateControl( m_pButton[0], NYEZOANGELAPPLYER_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NYEZOANGELAPPLYER_DIALOG_BUTTON2, TEXT("button_NONAME2") );

	file.CreateControl( m_pOutLine, NYEZOANGELAPPLYER_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	Hide();
}

void CNYezoAngelApplyerDlg::Action()
{

}

void CNYezoAngelApplyerDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNYezoAngelApplyerDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNYezoAngelApplyerDlg::StaticCallBackDialogNYezoAngelApplyerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNYezoAngelApplyerDlg *pThis = (CNYezoAngelApplyerDlg *)pControllerMgr;
	pThis->CallBackDialogNYezoAngelApplyerDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNYezoAngelApplyerDlg::CallBackDialogNYezoAngelApplyerDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NYEZOANGELAPPLYER_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	// 신청하기
				{
					cltGameMsgRequest_SetAngelApplyer clinfo(true);
					cltMsg clMsg( GAMEMSG_REQUEST_SETANGELAPPLYER, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}

		}
		break;

	case NYEZOANGELAPPLYER_DIALOG_BUTTON2: // 취소하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_SetAngelApplyer clinfo(false);
					cltMsg clMsg( GAMEMSG_REQUEST_SETANGELAPPLYER, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}

		}
		break;
	}
}
