#include "../Client/NInterface/NPartyDlg/NPartyInviteDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "..\..\..\CommonLogic\Msg\MsgType-Party.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;


CNPartyInviteDlg::CNPartyInviteDlg() : CDialogController()
{
	m_siPartyIndex = 0;
	m_siPartyLeaderPeronID = 0;

	m_pEdit = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
}

CNPartyInviteDlg::~CNPartyInviteDlg()
{
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
}

void CNPartyInviteDlg::Create()
{
	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/DLG_NParty_Invite.ddf"));
	file.CreatePopupDialog( this, NPARTYINVITE_DLG, TEXT("dialog_Party_Invite"), StaticCallBackDialogPartyInviteDlg );

	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		MovePos( 720, 66 );			
	else
		MovePos( 554, 66 );

	
	m_pEdit = new CEdit( this );
	m_pButton[0] = new CButton( this );
	m_pButton[1] = new CButton( this );

	file.CreateControl( m_pEdit,	    NPARTY_INVITE_DIALOG_EDIT,    TEXT("editbox_Party_Invite"));  
	file.CreateControl( m_pButton[0],   NPARTY_INVITE_DIALOG_BUTTON1, TEXT("button_Accept"));  
	file.CreateControl( m_pButton[1],   NPARTY_INVITE_DIALOG_BUTTON2, TEXT("button_Cancel"));  

	Hide();
}

void CNPartyInviteDlg::Hide()
{
	CControllerMgr::Show( FALSE );
}

void CNPartyInviteDlg::Show()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	CControllerMgr::Show( TRUE );
}


void CALLBACK CNPartyInviteDlg::StaticCallBackDialogPartyInviteDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPartyInviteDlg *pThis = (CNPartyInviteDlg *)pControllerMgr;
	pThis->CallBackDialogPartyInviteDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNPartyInviteDlg::CallBackDialogPartyInviteDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;

	case NPARTY_INVITE_DIALOG_BUTTON1:	// 수락
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: 
				{
					// 파티가 생성되어 있는 곳에서의 초대
					if ( m_siPartyIndex > 0 && m_siPartyLeaderPeronID > 0 )
					{
						cltGameMsgRequest_Party_Join clPartyJoin( m_siPartyIndex, m_siPartyLeaderPeronID );
						cltMsg clMsg( GAMEMSG_REQUEST_PARTY_JOIN, sizeof(clPartyJoin), (BYTE*)&clPartyJoin );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else if ( m_siPartyLeaderPeronID > 0 )	// 파티가 생성되어 있지 않은 곳에서의 초대 ( 파티장이 될 사람의 PersonID 는 있음 )
					{
						cltGameMsgRequest_Party_CreateJoin clPartyCreateJoin( m_siPartyLeaderPeronID );
						cltMsg clMsg( GAMEMSG_REQUEST_PARTY_CREATEJOIN, sizeof(clPartyCreateJoin), (BYTE*)&clPartyCreateJoin );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					Hide();
				}
				break;
			}
		}
		break;
	case NPARTY_INVITE_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: // 거절
				{
					cltGameMsgRequest_Party_InviteReject clPartyInviteReject( m_siPartyLeaderPeronID );
					cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITEREJECT, sizeof(clPartyInviteReject), (BYTE*)&clPartyInviteReject );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					Hide();
				}
				break;
			}
		}
		break;
	}
}

void CNPartyInviteDlg::PartyCreateInvite( SI32 LeaderPersonID, TCHAR *strLeaderCharName )
{
	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2001);
	StringCchPrintf( Buffer, 256, pText, strLeaderCharName );
	m_pEdit->SetText(Buffer); // 파티 초대 메세지

	m_siPartyIndex = 0;
	m_siPartyLeaderPeronID = LeaderPersonID;

	Show();
}

void CNPartyInviteDlg::PartyInvite( TCHAR *CharName, SI32 PartyLeaderPersonID, SI32 PartyIndex )
{
	if ( PartyIndex < 1 )
	{
		return;
	}

	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2001);
	StringCchPrintf( Buffer, 256, pText, CharName );
	m_pEdit->SetText(Buffer); // 파티 초대 메세지

	m_siPartyIndex = PartyIndex;
	m_siPartyLeaderPeronID = PartyLeaderPersonID;

	Show();
}