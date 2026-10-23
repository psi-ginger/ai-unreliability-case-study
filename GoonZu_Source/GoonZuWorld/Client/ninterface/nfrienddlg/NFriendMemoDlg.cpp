#include "NFriendMemoDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"

extern cltCommonLogic* pclClient;

CNFriendMemoDlg::CNFriendMemoDlg()
{
	Initialize();
}

CNFriendMemoDlg::~CNFriendMemoDlg()
{
	Destroy();
}

void CNFriendMemoDlg::Initialize()
{
	m_siFriendPersonID = 0;
}

void CNFriendMemoDlg::Destroy()
{

}

void CNFriendMemoDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NFriendMemo.ddf"));
	file.CreatePopupDialog( this, NFRIENDMEMO_DLG, TEXT("dialog_friendmemo"), StaticFriendMemoDlgProc );
		
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,NFRIENDMEMO_EDIT_MEMO, this) ,NFRIENDMEMO_EDIT_MEMO, TEXT("editbox_memo"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NFRIENDMEMO_BUTTON_SAVE, this)  , NFRIENDMEMO_BUTTON_SAVE , TEXT("button_save"));

}

void CALLBACK CNFriendMemoDlg::StaticFriendMemoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNFriendMemoDlg *pThis = (CNFriendMemoDlg*) pControllerMgr;
	pThis->NFriendMemoDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNFriendMemoDlg::NFriendMemoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NFRIENDMEMO_BUTTON_SAVE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				cltClient* pclclient = (cltClient*)pclClient;

				CEdit* pEdit = m_InterfaceFactory.GetEdit(NFRIENDMEMO_EDIT_MEMO);

				TCHAR* pText = pEdit->GetText();
				
				size_t textSize = _tcslen( pText );
				if ( textSize > MAX_FRIEND_NOTESIZE || textSize < 1 )
				{
					TCHAR* pTitle = GetTxtFromMgr(6778);
					TCHAR* pErrText = GetTxtFromMgr(6782);

					TCHAR buffer[256];
					StringCchPrintf( buffer, 256, pErrText, MAX_FRIEND_NOTESIZE/2 , MAX_FRIEND_NOTESIZE-1 );
					pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, buffer );

					return;
				}

				if ( m_siFriendPersonID > 0 )
				{
					cltGameMsgRequest_FriendMemoSet clinfo( m_siFriendPersonID, pEdit->GetText() ); 
					cltMsg clMsg(GAMEMSG_REQUEST_FRIENDMEMOSET, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
		}
	default:
		break;
	}
}

void CNFriendMemoDlg::SetFriendMemo( TCHAR* memo )
{
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NFRIENDMEMO_EDIT_MEMO);

	if ( pEdit )
	{
		pEdit->SetText( memo );
	}
}

void CNFriendMemoDlg::SetTitle(TCHAR* charName )
{
	TCHAR buffer[64] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(6779);

	StringCchPrintf( buffer, 64, pText, charName );

	CDialogController::SetTitle( buffer );
}

void CNFriendMemoDlg::SetFriendPersonID(SI32 personID )
{
	m_siFriendPersonID = personID;
}


