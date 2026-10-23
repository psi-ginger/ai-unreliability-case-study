#include "PartyDlg.h"

#include "../../../CommonLogic/Msg/MsgType-Party.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"

#include "../../../Common/SMLib/LibList/LibList.h"
#include "../../../common/SMLib/LibList/LibListMgr.h"
#include "Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CPartyDlg g_PartyDlg;

CPartyDlg::CPartyDlg()
{
	m_hRichEditModuleLoad = LoadLibrary("Riched20.dll");
	m_hDlg = NULL;
	m_hRichEdit = NULL;
	m_hRichEditDlg = NULL;
	m_hChatDataInputEdit = NULL;
	m_siPartyUserNum = 0;

	m_strViewData = new char[ MAX_RICHEDIT_OBJ_SIZE ];
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );

	m_strChatData = new char[ MAX_CHAT_DATA_LENGTH ];
	memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );

	m_siSelectedUserListIndex = -1;

	m_pChatDataList = new CLibListMgr();
	m_pChatDataList->CreateMaxEmptyObj( 2, 200 );
	
	Initialize();
}

CPartyDlg::~CPartyDlg()
{
	if( m_strViewData )
	{
		delete [] m_strViewData;
		m_strViewData = NULL;
	}

	if ( m_strChatData )
	{
		delete [] m_strChatData;
		m_strChatData = NULL;
	}
	
	if ( m_pChatDataList )
	{
		delete m_pChatDataList;
		m_pChatDataList = NULL;
	}

	if ( m_hRichEdit )
	{
		DestroyWindow(m_hRichEdit);
	}

	if ( m_hRichEditDlg )
	{
		DestroyWindow( m_hRichEditDlg );
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}

	FreeLibrary( m_hRichEditModuleLoad );
}

void CPartyDlg::Initialize()
{
	m_bIsPartyLeader = false;
	m_bPartyStatus = false;
	
	m_bShow = false;
	
	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		m_stClientPartyUser[ i ].Init();
	}

	return;
}

void CPartyDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hInst = hInst ;
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PARTY), hWnd, StaticPartyDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CPartyDlg::StaticPartyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PartyDlg.PartyDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPartyDlg::PartyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{			

			InitCommonControls();			

			m_hDlg = hDlg;

			ListView_SetExtendedListViewStyle( GetDlgItem(hDlg, IDC_LIST_PARTY_USER ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
			
			LVCOLUMN Col;			

			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			
			Col.cx = 110;
			char* pText = GetTxtFromMgr(1984);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( GetDlgItem(hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

			Col.cx = 50;
			pText = GetTxtFromMgr(1506);
			Col.pszText = pText;
			Col.iSubItem = 1;
			SendMessage( GetDlgItem(hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTCOLUMN, 1, (LPARAM)&Col );

			Col.cx = 100;
			pText = GetTxtFromMgr(1985);
			Col.pszText = pText;
			Col.iSubItem = 2;
			SendMessage( GetDlgItem(hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTCOLUMN, 2, (LPARAM)&Col );

			HWND hWnd = GetDlgItem( hDlg, IDC_PARTY_CHAT );
			m_hRichEditDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_RICHEDIT_CHAT_VIEW ), hWnd,  StaticRichEditChatViewDlgProc );

			ShowWindow( m_hRichEditDlg, SW_SHOW );

			RECT rect;
			GetClientRect( hWnd, &rect);

			MoveWindow( m_hRichEditDlg,
				rect.left, rect.top, rect.right-rect.left,
				rect.bottom-rect.top+2, TRUE);

			m_hChatDataInputEdit = GetDlgItem( hDlg, IDC_PARTY_EDIT );
			SendMessage( m_hChatDataInputEdit, EM_LIMITTEXT, (WPARAM)MAX_CHAT_DATA_LENGTH - 1, 0 );
			m_OldEditProc = (WNDPROC)SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG)StaticEditSubProc);
		}
		return TRUE;

	case WM_NOTIFY:
		{
			LPNMHDR						hdr;
			
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;
						
			if ( hdr->hwndFrom == GetDlgItem(hDlg, IDC_LIST_PARTY_USER ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							char NameBuffer[ MAX_PLAYER_NAME ] = "";
							LVITEM lvi;
							lvi.mask = LVIF_TEXT;
							lvi.iItem = nlv->iItem;
							lvi.iSubItem = 0;
							lvi.pszText = NameBuffer;
							lvi.cchTextMax = MAX_PLAYER_NAME - 1;

							SendDlgItemMessage( hDlg, IDC_LIST_PARTY_USER, LVM_GETITEM, 0, (LPARAM)&lvi );
							m_siSelectedUserListIndex = FindUserSelectedIndex( NameBuffer );
						}
					}
					break;
				}

				/*
			case IDC_LIST_PARTY_USER:
				{
					switch( HIWORD( wParam ) )
					{
					case LVN_ITEMCHANGED:
						{
							char Buffer[256 ] = "";
							SI32 siSelectedIndex = SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_GETCURSEL, 0, 0 );
							SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_GETTEXT, siSelectedIndex, (LPARAM) Buffer );
							m_siSelectedUserListIndex = FindUserSelectedIndex( Buffer );
						}
						break;
					}
					*/

			}
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case IDCANCEL:
				{
				}
				break;
			case IDC_BUTTON_PARTY_WARP:
				{
 					if ( m_siSelectedUserListIndex >= 0 )
					{
						if( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
							return TRUE ;
						if(!strcmp(pclclient->pclCM->CR[1]->GetName(),m_stClientPartyUser[ m_siSelectedUserListIndex ].strCharName))
							return TRUE ;

						// 영자나라에 있으면 불가능
						if ( pclclient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
							pclclient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
						{
							return TRUE ;
						}

						char buffer[256] = "" ;

						if ( !pclclient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp &&
							pclclient->pclCM->CR[1]->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
						{
							SI32 itemunique = 14005 ;  // 순간이동이용권
							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
							{
								char* pText = GetTxtFromMgr(3756);
								sprintf(buffer,pText);
								pText = GetTxtFromMgr(3326);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pText, buffer);
								return TRUE;
							}
						}

						char * pText = NULL ;

						if ( !pclclient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp &&
							pclclient->pclCM->CR[1]->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
						{
							pText = GetTxtFromMgr(5184);
							sprintf(buffer,pText,m_stClientPartyUser[ m_siSelectedUserListIndex ].strCharName);
						}
						else
						{
							pText = GetTxtFromMgr(5185);
							sprintf(buffer,pText,m_stClientPartyUser[ m_siSelectedUserListIndex ].strCharName);
						}
						pText = GetTxtFromMgr(5183);
						if (  MessageBox(NULL, buffer,pText,
							MB_ICONQUESTION | MB_YESNO) == IDNO )
						{
							return TRUE;
						}

						cltGameMsgRequest_PartyWarp clinfo(m_stClientPartyUser[ m_siSelectedUserListIndex ].strCharName);
						cltMsg clMsg(GAMEMSG_REQUEST_PARTYWARP,sizeof(clinfo),(char*)&clinfo);
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			// 탈퇴
			case IDC_BUTTON_PARTY_LEAVE:
				{
					// 파티장 이면서 자기 자신은 아닌 것을 선택했을때
					if ( m_bIsPartyLeader && m_siSelectedUserListIndex > 0 )
					{
						char Buffer[256 ] ="";
						char* pText = GetTxtFromMgr(1986);
						sprintf( Buffer, pText, m_stClientPartyUser[ m_siSelectedUserListIndex ].strCharName );
						
						char* pTitle = GetTxtFromMgr(1987);
						if (  MessageBox(NULL, Buffer, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
						{
							cltGameMsgRequest_Party_ForceLeave clPartyForceLeave( m_siPartyIndex, m_stClientPartyUser[ m_siSelectedUserListIndex ].siCharUnique );
							cltMsg clMsg(GAMEMSG_REQUEST_PARTY_FORCELEAVE, sizeof(clPartyForceLeave), (char*)&clPartyForceLeave);
							pclClient->SendMsgToServer((char*)&clMsg);
						}
					}
					// 파티장이 아닐때 ( 자기 자신만 나갈때 )
					else
					{
						char* pText = GetTxtFromMgr(1988);
						char* pTitle = GetTxtFromMgr(1987);
						if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
						{
							cltGameMsgRequest_Party_Leave clPartyLeave( m_siPartyIndex );
							cltMsg clMsg(GAMEMSG_REQUEST_PARTY_LEAVE, sizeof(clPartyLeave), (char*)&clPartyLeave);
							pclClient->SendMsgToServer((char*)&clMsg);
							Hide();
						}
					}
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
		}
		break;


	case WM_DESTROY:
		{
			SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG) m_OldEditProc );			
			Hide();
			m_hRichEdit = NULL ;
			m_hRichEditDlg = NULL;
			m_hDlg = NULL;
		}
		break;

	}

	return FALSE;
}

BOOL CPartyDlg::StaticRichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PartyDlg.RichEditChatViewDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CPartyDlg::RichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hRichEditDlg = hDlg;
			m_hRichEdit = CreateWindow( RICHEDIT_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT, 0, 0, 0, 0, hDlg, (HMENU)NULL, m_hInst, NULL );
			SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
			SetWindowText( m_hRichEdit, "" );
			//AddChat( "운영자", "군주 온라인에 오신 것을 환영합니다.", RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
		}
		return TRUE;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect( hDlg, &rect );
			MoveWindow( m_hRichEdit,
				rect.left, rect.top , rect.right-rect.left,
				rect.bottom-rect.top, TRUE);
		}
		break;

	case WM_CLOSE:
		{
			Hide();
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPartyDlg::StaticEditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_PartyDlg.EditSubProc( hWnd, msg, wParam, lParam );
}


LRESULT CALLBACK CPartyDlg::EditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_KEYUP:
	
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					SendPartyChat();
					SetWindowText( m_hChatDataInputEdit, "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit, "" );
		}

		break;
	default :
		break;
    }

    return CallWindowProc( m_OldEditProc, hWnd, msg, wParam, lParam );
}

void CPartyDlg::SendPartyChat()
{
	CObj *pTemp = m_pChatDataList->PopFrontToEmptyListAndPushBackToExistList();
	if ( pTemp == NULL )
	{
		return;
	}

	stPartyChatSend PartyChatSend( m_strChatData ) ;
	pTemp->SetData( (char*)&PartyChatSend, sizeof( stPartyChatSend ) );
}

void CPartyDlg::AddChat( char *strUserID, char *strChatData, COLORREF NameColor, COLORREF TextColor )
{
	SI32 PreLineCount = 0, NextLineCount = 0;
	PreLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	strcpy( m_strViewData, "" );	
	strcat( m_strViewData, "[" );
	strcat( m_strViewData, strUserID );
	strcat( m_strViewData, "] : " );
	AddName( m_strViewData, NameColor );

	strcpy( m_strViewData, "" );
	strcat( m_strViewData, strChatData );
	strcat( m_strViewData, "\r\n" );
	AddText( m_strViewData, TextColor );
	
	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
}

void CPartyDlg::AddChat( char *strChatData, COLORREF NameColor, COLORREF TextColor )
{
	SI32 PreLineCount = 0, NextLineCount = 0;
	PreLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	strcpy( m_strViewData, "" );
	strcat( m_strViewData, strChatData );
	strcat( m_strViewData, "\r\n" );
	AddText( m_strViewData, TextColor );

	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
}

void CPartyDlg::AddName( char *PlayerName, COLORREF Color )
{	
	SetFont( Color );

	int iTotalTextLength = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iTotalTextLength, iTotalTextLength );
	int iStartPos = iTotalTextLength;
	SendMessage( m_hRichEdit, EM_REPLACESEL, 0, (LPARAM)PlayerName );
	
	int iEndPos = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iStartPos, iEndPos );

	SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
	return;
}

void CPartyDlg::AddText( char *ChatText, COLORREF Color )
{	
	SetFont( Color );

	int iTotalTextLength = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iTotalTextLength, iTotalTextLength );
	int iStartPos = iTotalTextLength;	
	SendMessage( m_hRichEdit, EM_REPLACESEL, 0, (LPARAM)ChatText );	
		
	int iEndPos = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iStartPos, iEndPos );
	
	SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );	
	return;
}

void CPartyDlg::SetFont( COLORREF Color, DWORD dwMask, DWORD dwEffects, char *FontKind, SI16 YHeight )
{
	/*
	DWORD dwMask = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
	COLORREF TextColor = RGB( 0, 0, 255 );
	char TextKind[ 16 ] = "굴림";
	*/

	dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	dwEffects = 0;
	
	CHARFORMAT2 cf;
	memset( &cf, 0, sizeof( CHARFORMAT2 ) );	
	cf.cbSize = sizeof( CHARFORMAT2 );
	cf.dwMask = dwMask;
	cf.crTextColor = Color;
	cf.dwEffects = dwEffects;
	cf.yHeight = YHeight;
	strcpy(cf.szFaceName, FontKind );
	
	SendMessage( m_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf );
	
	return;
}

bool CPartyDlg::IsShow()
{
	return m_bShow;
}

void CPartyDlg::Show()
{
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	SetFocus(m_hChatDataInputEdit);
}

void CPartyDlg::Hide()
{
	Initialize();				// 파티가 종료되었으므로 기존에 있던 정보들을 초기화 해줌

	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	m_siPartyUserNum = 0;

	m_bIsPartyLeader = false;
	m_bPartyStatus = false;

	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

	if ( m_strViewData )
	{	
		memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	}
	
	if ( m_strChatData )
	{
		memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
	}	

	if ( m_hRichEdit )
	{
		SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
		SetWindowText( m_hRichEdit, "" );
	}

	m_siSelectedUserListIndex = -1;

	char* pText = GetTxtFromMgr(1989);
	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PARTY_USER_NUM ), pText );

	return;
}

bool CPartyDlg::IsPartyStatus()
{
	return m_bPartyStatus;
}

bool CPartyDlg::IsPartyLeader()
{
	return m_bIsPartyLeader;
}

bool CPartyDlg::IsPartyUser( SI32 CharUnique )
{
	if ( m_bPartyStatus )
	{
		for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
		{
			if ( m_stClientPartyUser[ i ].bPartyStatus && m_stClientPartyUser[ i ].siCharUnique == CharUnique )
			{
				return true;
			}
		}
	}

	return false;
}

SI32 CPartyDlg::GetPartyIndex()
{
	return m_siPartyIndex;
}

/*
// 파티징으로서 파티에 참가
void CPartyDlg::PartyCreate( SI32 PartyIndex )
{
	m_siSelectedUserListIndex = -1;
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_RESETCONTENT, 0, 0 );

	m_siPartyIndex = PartyIndex;
	
	m_bPartyStatus = true;
	m_bIsPartyLeader = true;

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_PARTY_EXPLAIN ), "파티를 만드셨습니다.");
	
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LB_ADDSTRING, 0, (LPARAM)pclClient->pclCM->CR[1]->GetName() );

	m_siPartyUserNum = 1;
	m_stClientPartyUser[ 0 ].bPartyStatus = true;
	m_stClientPartyUser[ 0 ].bIsPartyLeader = true;
	m_stClientPartyUser[ 0 ].siCharUnique = pclClient->pclCM->CR[1]->GetCharUnique();
	strcpy ( m_stClientPartyUser[ 0 ].strCharName, pclClient->pclCM->CR[1]->GetName() );
	
	Show();

	PrintUserNum();

	return;
}
*/


void CPartyDlg::PartyCreateJoin( SI32 PartyIndex, SI32 LeaderPersonID, SI32 LeaderCharUnique, char *strLeaderChatName, SI32 LeaderLevel, SI32 MemberPersonID, SI32 MemberCharUnique, char *strMemberCharName, SI32 MemberLevel )
{
	m_siSelectedUserListIndex = -1;
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

	m_siPartyIndex = PartyIndex;

	m_bPartyStatus = true;
	
	if ( pclClient->pclCM->CR[1]->pclCI->GetPersonID() == LeaderPersonID )
	{
		m_bIsPartyLeader = true;
		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), "파티를 만드셨습니다. 파티원 추가 1명당 10%의 경험치를 더 얻을 수 있습니다.");
		char* pText = GetTxtFromMgr(1990);
		AddChat( pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
	}
	else
	{
		m_bIsPartyLeader = false;
		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), "파티에 참가하셨습니다. 파티원 추가 1명당 10%의 경험치를 더 얻을 수 있습니다.");
		char* pText = GetTxtFromMgr(1991);
		AddChat(pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
	}

	m_stClientPartyUser[ 0 ].bPartyStatus = true;
	m_stClientPartyUser[ 0 ].bIsPartyLeader = true;
	m_stClientPartyUser[ 0 ].siCharUnique = LeaderCharUnique;
	m_stClientPartyUser[ 0 ].siLevel = LeaderLevel;
	strcpy( m_stClientPartyUser[ 0 ].strCharName, strLeaderChatName );

	m_stClientPartyUser[ 1 ].bPartyStatus = true;
	m_stClientPartyUser[ 1 ].bIsPartyLeader = false;
	m_stClientPartyUser[ 1 ].siCharUnique = MemberCharUnique;
	m_stClientPartyUser[ 1 ].siLevel = MemberLevel;
	strcpy( m_stClientPartyUser[ 1 ].strCharName, strMemberCharName );
	
	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	
	
	Li.iItem = 0;
	Li.iSubItem = 0;
	Li.pszText = m_stClientPartyUser[ 0 ].strCharName;

	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );
			

	char strLevel1[32] = "";
	char strLevel2[32] = "";

	Li.iSubItem = 1;
	sprintf( strLevel1, "%d", m_stClientPartyUser[0].siLevel );
	Li.pszText = strLevel1;
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

	Li.iSubItem = 2;
	Li.pszText = "0";
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );


	Li.iItem = 1;
	Li.iSubItem = 0;
	Li.pszText = m_stClientPartyUser[ 1 ].strCharName;

	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );

	Li.iSubItem = 1;
	sprintf( strLevel2, "%d", m_stClientPartyUser[1].siLevel );
	Li.pszText = strLevel2;
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );


	Li.iSubItem = 2;
	Li.pszText = "0";
	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

	m_siPartyUserNum = 2;
	PrintUserNum();

	pclClient->pclCM->CR[1]->siPartyIndex = PartyIndex;
	
	Show();
	
	return;
}

void CPartyDlg::PartyJoin( SI32 PartyIndex, SI16 UserListIndex )
{
	m_siSelectedUserListIndex = -1;
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

	m_siPartyIndex = PartyIndex;

	m_bPartyStatus = true;
	m_bIsPartyLeader = false;

	m_siPartyUserNum = 0;
	//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), "파티에 참가 되셨습니다. 파티원 추가 1명당 5%의 경험치를 더 얻을 수 있습니다." );
	char* pText = GetTxtFromMgr(1992);
	AddChat( pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	cltGameMsgRequest_Party_AllUserList clPartyAllUserList( m_siPartyIndex );
	cltMsg clMsg(GAMEMSG_REQUEST_PARTY_ALLUSERLIST, sizeof(clPartyAllUserList), (char*)&clPartyAllUserList);
	pclClient->SendMsgToServer((char*)&clMsg);

	pclClient->pclCM->CR[1]->siPartyIndex = PartyIndex;

	Show();

	return;
}

void CPartyDlg::PartyDelete( SI32 PartyIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_siPartyIndex == PartyIndex )
	{
		char* pTitle = GetTxtFromMgr(1993);
		char* pText = GetTxtFromMgr(1994);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		pclClient->pclCM->CR[1]->siPartyIndex = 0;
		Hide();
	}

	return;
}

void CPartyDlg::PartyAddUser( bool PartyLeader, SI32 CharUnique, char *CharName, SI16 UserListIndex, SI32 Level )
{
	m_stClientPartyUser[ UserListIndex ].bPartyStatus = true;
	m_stClientPartyUser[ UserListIndex ].bIsPartyLeader = PartyLeader;
	m_stClientPartyUser[ UserListIndex ].siCharUnique = CharUnique;
	strcpy( m_stClientPartyUser[ UserListIndex ].strCharName, CharName );
	m_stClientPartyUser[ UserListIndex ].siLevel = Level;
	
	char Buffer[ 1024 ] = "";
	char* pText = GetTxtFromMgr(1995);
	sprintf( Buffer, pText, CharName );
	//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
	AddChat( Buffer, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	++m_siPartyUserNum;

	ReloadPartyUserList();

	PrintUserNum();

	return;
}

void CPartyDlg::PartyDelUser( SI32 CharUnique, SI16 UserListIndex )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		if ( CharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_siSelectedUserListIndex = -1;
			ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

			m_stClientPartyUser[ UserListIndex ].Init();
			
			--m_siPartyUserNum;

			Hide();
			return;
		}

		char Buffer[256 ] = "";
		char* pText = GetTxtFromMgr(1996);
		sprintf( Buffer, pText, m_stClientPartyUser[ UserListIndex ].strCharName );
		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
		AddChat( Buffer, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

		m_stClientPartyUser[ UserListIndex ].Init();

		--m_siPartyUserNum;

		ReloadPartyUserList();
	}

	if ( m_siPartyUserNum < 1 )
	{
		Hide();
	}

	PrintUserNum();

	return;
}

void CPartyDlg::PartyForceDelUser( SI32 CharUnique, SI16 UserListIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		if ( CharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_siSelectedUserListIndex = -1;
			ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );
			Hide();
			char* pTitle = GetTxtFromMgr(1997);
			char* pText = GetTxtFromMgr(1998);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		char Buffer[256 ] = "";
		char* pText = GetTxtFromMgr(1999);
		sprintf( Buffer, pText, m_stClientPartyUser[ UserListIndex ].strCharName );

		//SetWindowText( GetDlgItem( m_hDlg, IDC_PARTY_CHAT ), Buffer );
		AddChat( Buffer, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
		m_stClientPartyUser[ UserListIndex ].Init();
		--m_siPartyUserNum;
		
		ReloadPartyUserList();
	}

	PrintUserNum();

	return;
}

void CPartyDlg::PartyChangeLeader( SI32 CharUnique, SI16 UserListIndex )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique )
	{
		m_stClientPartyUser[ UserListIndex ].bIsPartyLeader = true;
		m_stClientPartyUser[ 0 ].Set( &m_stClientPartyUser[ UserListIndex ] );
		m_stClientPartyUser[ UserListIndex ].Init();

		// 이 클라이언트가 파티장이 된 경우
		if ( m_stClientPartyUser[0].siCharUnique == pclClient->pclCM->CR[1]->GetCharUnique() )
		{
			m_bIsPartyLeader = true; 
		}

		ReloadPartyUserList();
	}

	return;
}

void CPartyDlg::PartyNotifyExp( SI32 CharUnique, SI16 UserListIndex, SI32 Exp )
{
	if ( m_stClientPartyUser[ UserListIndex ].siCharUnique == CharUnique && Exp > 0 )
	{
		m_stClientPartyUser[ UserListIndex ].siExp = Exp;
		ReloadPartyUserList();
	}

	return;
}

SI32 CPartyDlg::FindUserSelectedIndex( char *CharName )
{
	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stClientPartyUser[ i ].bPartyStatus )
		{
			if ( strcmp( m_stClientPartyUser[ i ].strCharName, CharName ) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}

void CPartyDlg::PrintUserNum()
{
	char Buffer[256] = "";
	char* pText = GetTxtFromMgr(2000);
	sprintf( Buffer, pText, m_siPartyUserNum, MAX_PARTY_USER_NUM );

	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PARTY_USER_NUM ), Buffer );
	return;
}

void CPartyDlg::ReloadPartyUserList()
{
	m_siSelectedUserListIndex = -1;
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ) );

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	
	
	Li.iItem = 0;	
	Li.iSubItem = 0;

	SI32 Count = 0;
	char strExp[32];
	char strLevel[32];
	for ( int i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stClientPartyUser[ i ].bPartyStatus )
		{
			Li.iItem = Count++;
			Li.iSubItem = 0;			
			Li.pszText = m_stClientPartyUser[ i ].strCharName;
			SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_INSERTITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 1;
			sprintf( strLevel, "%d", m_stClientPartyUser[ i ].siLevel );
			Li.pszText = strLevel;
			SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 2;
			sprintf( strExp, "%d", m_stClientPartyUser[ i ].siExp );
			Li.pszText = strExp;
			SendMessage( GetDlgItem( m_hDlg, IDC_LIST_PARTY_USER ), LVM_SETITEM, 0, (LPARAM)&Li );
		}
	}

	return;
}

bool CPartyDlg::PopPartyChatSendData( stPartyChatSend *pPartyChatSend )
{
	CObj *pFirst = m_pChatDataList->GetExistList()->GetFirst();

	if ( pFirst == NULL )
	{
		return false;
	}

	UI32 uiSize;
	pFirst->GetData( (char*)pPartyChatSend, &uiSize );
	
	m_pChatDataList->GetExistList()->Pop( pFirst );
	m_pChatDataList->PushBackToEmptyList( pFirst );

	return true;
}