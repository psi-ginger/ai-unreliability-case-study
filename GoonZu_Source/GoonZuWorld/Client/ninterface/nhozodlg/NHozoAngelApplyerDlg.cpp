#include "../Client/NInterface/NHozoDlg/NHozoAngelApplyerDlg.h"

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

CNHozoAngelApplyerDlg::CNHozoAngelApplyerDlg() : CDialogController()
{
	m_pStatic	= NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pOutLine = NULL;
}

CNHozoAngelApplyerDlg::~CNHozoAngelApplyerDlg()
{
	SAFE_DELETE(m_pStatic);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);

	SAFE_DELETE(m_pOutLine);
}

void CNHozoAngelApplyerDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_AngelApplyer.ddf"));
	file.CreateChildDialog( this, NHOZOANGELAPPYER_DLG, TEXT("dialog_Hozo_AngelApplyer"), StaticCallBackDialogNHozoAngelApplyerDlg, pParent );
	
	NEWCSTATIC(m_pStatic);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);

	NEWCOUTLINE(m_pOutLine);
	
	file.CreateControl( m_pStatic, NHOZOANGELAPPLYER_DIALOG_STATIC, TEXT("statictext_NONAME1") );

	file.CreateControl( m_pButton[0], NHOZOANGELAPPLYER_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHOZOANGELAPPLYER_DIALOG_BUTTON2, TEXT("button_NONAME2") );

	file.CreateControl( m_pOutLine, NHOZOANGELAPPLYER_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	Hide();
}

void CNHozoAngelApplyerDlg::Action()
{

}

void CNHozoAngelApplyerDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoAngelApplyerDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHozoAngelApplyerDlg::StaticCallBackDialogNHozoAngelApplyerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoAngelApplyerDlg *pThis = (CNHozoAngelApplyerDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoAngelApplyerDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoAngelApplyerDlg::CallBackDialogNHozoAngelApplyerDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHOZOANGELAPPLYER_DIALOG_BUTTON1:
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

	case NHOZOANGELAPPLYER_DIALOG_BUTTON2: // 취소하기
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
