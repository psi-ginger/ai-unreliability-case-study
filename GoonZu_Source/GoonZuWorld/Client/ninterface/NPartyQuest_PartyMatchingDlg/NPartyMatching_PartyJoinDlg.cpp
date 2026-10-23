
#include "NPartyMatching_PartyJoinDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../CommonLogic/Msg/MsgType-Party.h"



extern cltCommonLogic* pclClient;

CNPartyMatching_PartyJoinDlg::CNPartyMatching_PartyJoinDlg( void )
{

}

CNPartyMatching_PartyJoinDlg::~CNPartyMatching_PartyJoinDlg( void )
{

}

void CNPartyMatching_PartyJoinDlg::Init( )
{

}

void CNPartyMatching_PartyJoinDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPartyMatchingDlg/DLG_NPartyJoinDlg.ddf" ) );
	file.CreatePopupDialog( this, NPARTY_JOIN_DLG, TEXT( "dialog_Party_Join" ), StaticCallBackDialogNPartyMatching_PartyJoinDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_JOIN_DLG_BUTTON_ACCEPT, this), NPARTY_JOIN_DLG_BUTTON_ACCEPT, TEXT("button_Accept") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_JOIN_DLG_BUTTON_CANCEL, this), NPARTY_JOIN_DLG_BUTTON_CANCEL, TEXT("button_Cancel") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NPARTY_JOIN_DLG_EDITBOX_PARTY_ASKJOINMSG, this), NPARTY_JOIN_DLG_EDITBOX_PARTY_ASKJOINMSG, TEXT("editbox_Party_askJoinmsg") );

	m_siLeaderUnique = -1;
	m_siMemberUnique = -1;
	m_siPartyIndex = -1;
}

void CALLBACK CNPartyMatching_PartyJoinDlg::StaticCallBackDialogNPartyMatching_PartyJoinDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPartyMatching_PartyJoinDlg* pThis = (CNPartyMatching_PartyJoinDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNPartyMatching_PartyJoinDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNPartyMatching_PartyJoinDlg::CallBackDialogNPartyMatching_PartyJoinDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_siLeaderUnique = -1;
			m_siMemberUnique = -1;
			m_siPartyIndex = -1;

			DeleteDialog();
		}
		break;
	case NPARTY_JOIN_DLG_BUTTON_ACCEPT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				if ( 0 >= m_siLeaderUnique || 0 >= m_siMemberUnique || 0 >= m_siPartyIndex )
				{
					// 서버로 승낙 패킷 보냄
					cltGameMsgRequest_Party_WantJoinResult clMsgInfo( true, m_siLeaderUnique, m_siMemberUnique, m_siPartyIndex );
					cltMsg clMsg( GAMEMSG_REQUEST_PARTY_WANTJOINRESULT, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}

				m_siLeaderUnique = -1;
				m_siMemberUnique = -1;
				m_siPartyIndex = -1;

				DeleteDialog();
			}
		}
		break;
	case NPARTY_JOIN_DLG_BUTTON_CANCEL:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{ 
				// 서버로 거절 패킷 보냄
				cltGameMsgRequest_Party_WantJoinResult clMsgInfo( false, m_siLeaderUnique, m_siMemberUnique, m_siPartyIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_PARTY_WANTJOINRESULT, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

				m_siLeaderUnique = -1;
				m_siMemberUnique = -1;
				m_siPartyIndex = -1;

				DeleteDialog();
			}
		}
		break;
	}
}

void CNPartyMatching_PartyJoinDlg::SetJoinRequestInfo(SI32 siLeaderUnique, SI32 siMemberUnique, SI32 siPartyIndex)
{
	m_siLeaderUnique = siLeaderUnique;
	m_siMemberUnique = siMemberUnique;
	m_siPartyIndex = siPartyIndex;
}

void CNPartyMatching_PartyJoinDlg::SetText(TCHAR* pszMessage)
{
	if ( NULL == pszMessage ) return;

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NPARTY_JOIN_DLG_EDITBOX_PARTY_ASKJOINMSG );
	if ( NULL == pEdit ) return;

	pEdit->SetText( pszMessage );
}

void CNPartyMatching_PartyJoinDlg::Show()
{
	CDialogController::Show( true );
}

void CNPartyMatching_PartyJoinDlg::Hide()
{
	CDialogController::Show( false );
}