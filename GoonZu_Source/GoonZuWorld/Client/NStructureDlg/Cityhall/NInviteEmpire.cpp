#include "../Client/NStructureDlg/Cityhall/NInviteEmpire.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-Empire.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNInviteEmpireDlg::CNInviteEmpireDlg() : CDialogController()
{
}

CNInviteEmpireDlg::~CNInviteEmpireDlg()
{
}

void CNInviteEmpireDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_InviteEmpire.ddf"));
	file.CreatePopupDialog( this, NINVITEEMPIRE_DLG, TEXT("dialog_InviteEmpire"), StaticCallBackDialogNInviteEmpireDlg );

	file.CreateControl( AddInterface( STATIC, NINVITEEMPIRE_STATIC ), NINVITEEMPIRE_STATIC, TEXT("statictext_NONAME1")); 
	file.CreateControl( AddInterface( EDIT,   NINVITEEMPIRE_EDIT ),   NINVITEEMPIRE_EDIT,   TEXT("editbox_NONAME1")); 
	file.CreateControl( AddInterface( BUTTON, NINVITEEMPIRE_BUTTON_INVITEVILLAGE ), NINVITEEMPIRE_BUTTON_INVITEVILLAGE, TEXT("button_NONAME1")); 
	file.CreateControl( AddInterface( BUTTON, NINVITEEMPIRE_BUTTON_INVITEGUILD ), NINVITEEMPIRE_BUTTON_INVITEGUILD, TEXT("button_NONAME2")); 
}

CControllerObj* CNInviteEmpireDlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}

void CALLBACK CNInviteEmpireDlg::StaticCallBackDialogNInviteEmpireDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNInviteEmpireDlg *pThis = (CNInviteEmpireDlg *)pControllerMgr;
	pThis->CallBackDialogNInviteEmpireDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNInviteEmpireDlg::CallBackDialogNInviteEmpireDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NINVITEEMPIRE_BUTTON_INVITEVILLAGE:
		{
			CEdit* edit = m_InterfaceFactory_.GetEdit(NINVITEEMPIRE_EDIT);
			SI32 len = _tcslen( edit->GetText() );
			if( len > 0 && len < MAX_PLAYER_NAME )
			{
				cltGameMsgRequest_InviteEmpire clInviteEmpire( cltGameMsgRequest_InviteEmpire::VILLAGE, edit->GetText() );
				cltMsg clMsg( GAMEMSG_REQUEST_INVITEEMPIRE, sizeof(clInviteEmpire), (BYTE*)&clInviteEmpire );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;

	case NINVITEEMPIRE_BUTTON_INVITEGUILD:
		{
			CEdit* edit = m_InterfaceFactory_.GetEdit(NINVITEEMPIRE_EDIT);
			SI32 len = _tcslen( edit->GetText() );
			if( len > 0 && len < MAX_PLAYER_NAME )
			{
				cltGameMsgRequest_InviteEmpire clInviteEmpire( cltGameMsgRequest_InviteEmpire::GUILD, edit->GetText() );
			}
		}
		break;
	}
}
