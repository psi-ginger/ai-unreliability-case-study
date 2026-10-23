#include "PartyInviteDlg.h"
#include "..\..\..\CommonLogic\Msg\MsgType-Party.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

CPartyInviteDlg g_PartyInviteDlg;

CPartyInviteDlg::CPartyInviteDlg()
{
	m_hDlg = NULL;
	m_siPartyIndex = 0;
	m_siPartyLeaderPeronID = 0;
}

CPartyInviteDlg::~CPartyInviteDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CPartyInviteDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PARTY_INVITE), hWnd, StaticPartyInviteDlgProc );
	Hide();
	
	return;
}

BOOL CALLBACK CPartyInviteDlg::StaticPartyInviteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PartyInviteDlg.PartyInviteDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPartyInviteDlg::PartyInviteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;			
		}
		return TRUE;

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case IDOK:
				{
					// 파티가 생성되어 있는 곳에서의 초대
					if ( m_siPartyIndex > 0 && m_siPartyLeaderPeronID > 0 )
					{
						cltGameMsgRequest_Party_Join clPartyJoin( m_siPartyIndex, m_siPartyLeaderPeronID );
						cltMsg clMsg( GAMEMSG_REQUEST_PARTY_JOIN, sizeof(clPartyJoin), (char*)&clPartyJoin );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
					else if ( m_siPartyLeaderPeronID > 0 )	// 파티가 생성되어 있지 않은 곳에서의 초대 ( 파티장이 될 사람의 PersonID 는 있음 )
					{
						cltGameMsgRequest_Party_CreateJoin clPartyCreateJoin( m_siPartyLeaderPeronID );
						cltMsg clMsg( GAMEMSG_REQUEST_PARTY_CREATEJOIN, sizeof(clPartyCreateJoin), (char*)&clPartyCreateJoin );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
					
					Hide();
				}
				break;
			case IDCANCEL:			// 거절
				{
					cltGameMsgRequest_Party_InviteReject clPartyInviteReject( m_siPartyLeaderPeronID );
					cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITEREJECT, sizeof(clPartyInviteReject), (char*)&clPartyInviteReject );
					pclClient->SendMsgToServer((char*)&clMsg);

					Hide();
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			Hide();
		}
		break;

	case WM_DESTROY:
		{
			m_hDlg = NULL;
		}
		break;
	}
	
	return FALSE;
}

bool CPartyInviteDlg::IsShow()
{
	return m_bShow;
}

void CPartyInviteDlg::Show()
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
}

void CPartyInviteDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	m_siPartyIndex = 0;

	return;
}


void CPartyInviteDlg::PartyCreateInvite( SI32 LeaderPersonID, char *strLeaderCharName )
{
	char Buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2001);
	sprintf( Buffer, pText, strLeaderCharName );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_PARTY_INVITE_MSG ), Buffer );

	m_siPartyIndex = 0;
	m_siPartyLeaderPeronID = LeaderPersonID;

	Show();

	return;
}

void CPartyInviteDlg::PartyInvite( char *CharName, SI32 PartyLeaderPersonID, SI32 PartyIndex )
{
	if ( PartyIndex < 1 )
	{
		return;
	}

	char Buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2001);
	sprintf( Buffer, pText, CharName );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_PARTY_INVITE_MSG ), Buffer );

	m_siPartyIndex = PartyIndex;
	m_siPartyLeaderPeronID = PartyLeaderPersonID;

	Show();
	
	return;
}