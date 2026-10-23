#include "../common/CommonHeader.h"
#include "../../Client.h"
#include "NGuildSendMsg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "MSG/MsgType-PostOffice.h"

CNGuildSendMsgDlg::CNGuildSendMsgDlg()
{
	Initialize();
}

CNGuildSendMsgDlg::~CNGuildSendMsgDlg()
{
	Destroy();
}

void CNGuildSendMsgDlg::Initialize()
{
	ZeroMemory( m_szMasterName, MAX_PLAYER_NAME );
}

void CNGuildSendMsgDlg::Destroy()
{

}

void CNGuildSendMsgDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NGuildInfo/DLG_NGuildSendMsg.ddf"));

	file.CreatePopupDialog( this, NGUILDSENDMSG_DLG, TEXT("dialog_guildsendmsg"),StatiNGuildSendMsgDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NGUILDSENDMSG_EDIT_MSG, this)  , NGUILDSENDMSG_EDIT_MSG, TEXT("editbox_msg"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGUILDSENDMSG_BUTTON_SEND, this ), NGUILDSENDMSG_BUTTON_SEND, TEXT("button_send"));
}

void CNGuildSendMsgDlg::Hide()
{
	CControllerMgr::Show(false);

}

void CNGuildSendMsgDlg::Show()
{
	CControllerMgr::Show(true);

}


void CALLBACK CNGuildSendMsgDlg::StatiNGuildSendMsgDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildSendMsgDlg *pThis = (CNGuildSendMsgDlg*) pControllerMgr;
	pThis->NGuildSendMsgDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNGuildSendMsgDlg::NGuildSendMsgDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				DeleteDialog();
			}
		}
		break;
	case NGUILDSENDMSG_BUTTON_SEND:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				Send();
			}
		}
		break;
	}
}

void CNGuildSendMsgDlg::Send()
{
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NGUILDSENDMSG_EDIT_MSG);

	TCHAR* szmsg = pEdit->GetText();

	if ( szmsg && _tcscmp( szmsg, TEXT("") ) != 0 )
	{
		cltGameMsgRequest_PostOfficeSendMsg sendMsg( m_szMasterName, szmsg );

		cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG, 
			sizeof(cltGameMsgRequest_PostOfficeSendMsg), (BYTE*)&sendMsg);

		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
	}

	DeleteDialog();
}

void CNGuildSendMsgDlg::SetMasterName(TCHAR* masterName )
{
	if ( masterName )
	{
        StringCchCopy( m_szMasterName, MAX_PLAYER_NAME, masterName );
	}
}