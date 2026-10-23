

#include "VoteDlg.h"

#include "../../Client/client.h"
//#include "../../CommonLogic/MsgType-Person.h"

#include "../../CommonLogic/Msg/MsgType-Vote.h"

#include "../../Common/VoteMgr/VoteBaseInfo.h"
#include "../../Common/VoteMgr/VoteMgr.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

extern cltCommonLogic* pclClient;

CVoteDlg::CVoteDlg()
{
	m_hDlg = NULL;
	m_hVoteAdvertiseDlg = NULL;
	m_bShow = false;	
	m_pstCandidatureList = new stCandidature[ MAX_CANDIDATURE_NUM ];
	memset( m_pstCandidatureList, 0, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	
}

CVoteDlg::~CVoteDlg()
{
	if ( m_pstCandidatureList )
	{
		delete [] m_pstCandidatureList;
		m_pstCandidatureList = NULL;
	}
	
	if( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CVoteDlg::Initialize()
{
	memset( m_pstCandidatureList, 0, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	
	return;
}

void CVoteDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_VOTEKINGSELECTION ), hWnd,  StaticVoteDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CVoteDlg::StaticVoteAdvertiseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pVoteDlg->VoteAdvertiseDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CVoteDlg::VoteAdvertiseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_ADVERTISE );
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				{
					ShowWindow( hDlg, SW_HIDE );
				}
				break;
			}
		}
		break;
	case WM_CLOSE:
		{
			ShowWindow( hDlg, SW_HIDE );
		}
		break;
	case WM_DESTROY:
		{
			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_ADVERTISE );
			if( hwnd )
			{
				UnEmbedBrowserObject(hwnd);
			}

			m_hVoteAdvertiseDlg = NULL;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CVoteDlg::StaticVoteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pVoteDlg->VoteDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CVoteDlg::VoteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			m_hVoteListCtrl = GetDlgItem( hDlg, IDC_LIST_VOTEKINGSELECTION );
			
			ListView_SetExtendedListViewStyle( m_hVoteListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );

			m_hDlg = hDlg;

			m_hVoteAdvertiseDlg = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE( IDD_DIALOG_VOTEADVERTISE ), hDlg, StaticVoteAdvertiseDlgProc );
			
			LVCOLUMN Col;

			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;

			Col.cx = 200;
			char* pText = GetTxtFromMgr(2469);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( m_hVoteListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );
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
						

			if ( hdr->hwndFrom == GetDlgItem(hDlg, IDC_LIST_VOTEKINGSELECTION ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
						{
							char NameBuffer[ MAX_PLAYER_NAME ] = "";
							LVITEM lvi;
							lvi.mask = LVIF_TEXT;
							lvi.iItem = nlv->iItem;
							lvi.iSubItem = 0;
							lvi.pszText = NameBuffer;
							lvi.cchTextMax = MAX_PLAYER_NAME - 1;
							
							SendDlgItemMessage( hDlg, IDC_LIST_VOTEKINGSELECTION, LVM_GETITEM, 0, (LPARAM)&lvi );
							
							strcat( NameBuffer, "" );
							
							SetWindowText( GetDlgItem( hDlg, IDC_EDIT_VOTENAME ), NameBuffer );
						}
					}
					break;
				}
			}
		}
		break;
		
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
				{	
					char NameBuffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_VOTENAME ), NameBuffer, MAX_PLAYER_NAME );

					if ( NameBuffer == NULL || strlen ( NameBuffer ) < 1 )
					{
						return FALSE;
					}

					SI16 i = 0;
					for ( ; i < MAX_CANDIDATURE_NUM; ++i )
					{
						if ( m_pstCandidatureList[ i ].siListIndex > 0 )
						{
							if ( strcmp( m_pstCandidatureList[ i ].strChatName, NameBuffer ) == 0 )
							{
								break;
							}
						}
					}

					if ( i == MAX_CANDIDATURE_NUM )
					{
						return FALSE;
					}

					// 실제 투표에 참여
					cltGameMsgRequest_VoteKingSelection clVoteKingSelection( m_pstCandidatureList[ i ].siListIndex );
					cltMsg clMsg( GAMEMSG_REQUEST_VOTEKINGSELECTION, sizeof(clVoteKingSelection), (char*)&clVoteKingSelection );
					pclClient->SendMsgToServer((char*)&clMsg);
					
					Hide();
				}
				break;
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_ADVERTISE:
				{
					char NameBuffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_VOTENAME ), NameBuffer, MAX_PLAYER_NAME );

					if ( NameBuffer == NULL || strlen ( NameBuffer ) < 1 )
					{
						return FALSE;
					}

					SI16 i = 0;
					for ( ; i < MAX_CANDIDATURE_NUM; ++i )
					{
						if ( m_pstCandidatureList[ i ].siListIndex > 0 )
						{
							if ( strcmp( m_pstCandidatureList[ i ].strChatName, NameBuffer ) == 0 )
							{
								break;
							}
						}
					}

					if ( i == MAX_CANDIDATURE_NUM )
					{
						return FALSE;
					}
					
					DisplayVoteAdvertise( m_pstCandidatureList[ i ].strWebPageURL );


				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hVoteAdvertiseDlg )
			{
				SendMessage( m_hVoteAdvertiseDlg, WM_DESTROY, 0, 0 );
			}

			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

bool CVoteDlg::IsShow()
{
	return m_bShow;
}

void CVoteDlg::Show()
{
	m_bShow = true;
	LoadCandidatureList();
	ShowWindow( m_hDlg, SW_SHOW );
}

void CVoteDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	Initialize();

	return;
}

void CVoteDlg::LoadCandidatureList()
{
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_VOTENAME ), "" );
	ListView_DeleteAllItems( m_hVoteListCtrl );

	//-----------------------------------------------------------------------
	// 보여주는 리스트를 랜덤하게 바꿔줌.
	SI16 i = 0;
	SI16 CandiateUserNum = 0;
	SI16 CandidateSequenceList[ MAX_CANDIDATURE_NUM ] = { 0 };

	for ( i = 0; i < MAX_CANDIDATURE_NUM; ++i )
	{
		if ( m_pstCandidatureList[ i ].siListIndex > 0 )
		{
			CandidateSequenceList[ i ] = i;
			++CandiateUserNum;
		}
	}

	if ( CandiateUserNum < 1 )
	{
		return;
	}

	SI16 SwapNumer;
	for ( i = 0; i < CandiateUserNum; ++i )
	{
		SI16 RandomNum = rand() % CandiateUserNum;

		if ( i != RandomNum )
		{
			SwapNumer = CandidateSequenceList[ i ];
			CandidateSequenceList[ i ] = CandidateSequenceList[ RandomNum ];
			CandidateSequenceList[ RandomNum ] = SwapNumer;
		}
	}
	//-----------------------------------------------------------------------


	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	
	Li.iSubItem = 0;
	
	SI16 siCount = 0;

	char Buffer[32] = "";

	for ( i = 0; i < CandiateUserNum; ++i )
	{
		Li.iItem = siCount++;
		Li.iSubItem = 0;
		Li.pszText = m_pstCandidatureList[ CandidateSequenceList[ i ] ].strChatName;
		SendMessage( m_hVoteListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );
	}

	return;
}

void CVoteDlg::Set( char *VoteExplain, stCandidature *pCandidature )
{
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_VOTE_EXPLAIN ), VoteExplain );
	memcpy( m_pstCandidatureList, pCandidature, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	Show();
	return;
}

void CVoteDlg::DisplayVoteAdvertise( char *HtmlPage )
{
	if ( HtmlPage == NULL || strlen( HtmlPage ) < 1 )
	{
		return;
	}

	char buffer[ 1024 ] = "";
	sprintf( buffer, "http://goonzu.ndoors.com/center/vote/vote_view_client.asp?list_seq=%s", HtmlPage );

	DisplayHTMLPage( GetDlgItem( m_hVoteAdvertiseDlg, IDC_STATIC_ADVERTISE ), buffer );
	ShowWindow( m_hVoteAdvertiseDlg, SW_SHOW );
	
	return;
}

