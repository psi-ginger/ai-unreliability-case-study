#include "PrivateChatDlg.h"
#include "../ChatDlg/ChatDataMgr.h"
#include "PrivateChatDlgMgr.h"

#include "..\..\FontManager\FontManager.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"

#include "../../Music/music.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"


CPrivateChatRejectViewDlg g_PrivateChatRejectViewDlg;
CPrivateChatDlg	g_PrivateChatDlg[ MAX_PRIVATE_CHAT_DLG_NUM ];

#include "../../Client/client.h"

// 욕설 필터
#include "../../Client/AbuseFilter/AbuseFilter.h"

extern cltCommonLogic* pclClient;

CPrivateChatRejectViewDlg::CPrivateChatRejectViewDlg()
{	
	m_bShow = false;
}

CPrivateChatRejectViewDlg::~CPrivateChatRejectViewDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CPrivateChatRejectViewDlg::Create( HWND hWnd, HINSTANCE hInst, CPrivateChatDlgMgr *pPrivateChatDlgMgr )
{
	m_pPrivateChatDlgMgr = pPrivateChatDlgMgr;
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_PRIVATECHATREJECTVIEW ), hWnd, StaticPrivateChatRejectViewDlg );
	Hide();
	
	return;
}

BOOL CALLBACK CPrivateChatRejectViewDlg::StaticPrivateChatRejectViewDlg(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_PrivateChatRejectViewDlg.PrivateChatRejectViewDlg( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatRejectViewDlg::PrivateChatRejectViewDlg(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			m_hDlg = hDlg;
			m_hRejectUserList = GetDlgItem( hDlg, IDC_LIST_PRIVATECHAT_REJECTUSER );
			m_hUserName = GetDlgItem( hDlg, IDC_EDIT_NAME );

			LVCOLUMN Col;			

			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			Col.cx = 150;

			char* pText = GetTxtFromMgr(1556);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( m_hRejectUserList, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

			
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
						

			if ( hdr->hwndFrom == m_hRejectUserList )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
							{
								char Buffer[256 ] = "";
								ListView_GetItemText( m_hRejectUserList, nlv->iItem, 0, Buffer, 128 );
								SetWindowText( m_hUserName, Buffer );
							}
						}
					}
					break;
				}
			}
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT_CANCEL:
				{
					char strUserName[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hUserName, strUserName, MAX_PLAYER_NAME - 1 );
					strcat( strUserName, "" );

					if ( strlen( strUserName ) > 0 )
					{
						m_pPrivateChatDlgMgr->DelRejectUser( strUserName );
						LoadRejectList();
					}
				}
				break;
			}
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

bool CPrivateChatRejectViewDlg::IsShow()
{
	return m_bShow;
}
	
void CPrivateChatRejectViewDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}
	
	LoadRejectList();

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CPrivateChatRejectViewDlg::Hide()
{
	ListView_DeleteAllItems( m_hRejectUserList );

	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CPrivateChatRejectViewDlg::LoadRejectList()
{
	SetWindowText( m_hUserName, "" );

	ListView_DeleteAllItems( m_hRejectUserList );

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;
	Li.iImage = 0;
	Li.iSubItem = 0;
	
	SI16 siCount = 0;
	for ( SI16 i = 0; i < MAX_REJECT_USER_NUM; ++i )
	{
		if ( strlen( m_pPrivateChatDlgMgr->m_strRejectUserList[i] ) > 0 )
		{
			Li.iItem = siCount;
			Li.pszText = (char*)m_pPrivateChatDlgMgr->m_strRejectUserList[i];
			SendMessage( m_hRejectUserList, LVM_INSERTITEM, 0, (LPARAM)&Li );
			++siCount;
		}
	}
}

CPrivateChatDlg::CPrivateChatDlg()
{
	//m_hChatDataInputEdit = NULL;

	m_pChatDataViewMgr = new CChatDataMgr( true );
	m_pChatDataViewMgr->CreateMaxEmptyObj( MAX_CHAT_DATA_RECORD_NUM );

	m_pChatDataSendMgr = new CChatDataMgr( false );
	m_pChatDataSendMgr->CreateMaxEmptyObj( MAX_CHAT_DATA_BUFFER_NUM );

	m_strChatViewData = new char[ ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM ];
	memset( m_strChatViewData, 0, ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM );

	for ( SI32 i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		m_hDlg[i] = NULL;
		m_bActive[ i ] = false;
		m_bTimerStatus[ i ] = false;
	}

	for ( i = 0 ; i < MAX_PRIVATE_CHAT_DLG_NUM ; i ++ )
		for ( int j = 0 ; j < 10 ; j ++ )
			m_bBitmap[i][j] = NULL ;

	m_hAnswerEditBrush = CreateSolidBrush(COLOR_DIALOG_YELLOW);
	m_hAnswerDlgBrush  = CreateSolidBrush(COLOR_DIALOG_GRAY);

	Initialize();
}

CPrivateChatDlg::~CPrivateChatDlg()
{
	Destroy();

	if ( m_pChatDataViewMgr )
	{
		delete m_pChatDataViewMgr;
		m_pChatDataViewMgr = NULL;
	}

	if ( m_pChatDataSendMgr )
	{
		delete m_pChatDataSendMgr;
		m_pChatDataSendMgr = NULL;
	}

	if ( m_strChatViewData )
	{
		delete [] m_strChatViewData;
		m_strChatViewData = NULL;
	}

	if ( m_hAnswerEditBrush)
	{
		DeleteObject( m_hAnswerEditBrush);
		m_hAnswerEditBrush = NULL;
	}
	
	if ( m_hAnswerDlgBrush)
	{
		DeleteObject( m_hAnswerDlgBrush);
		m_hAnswerDlgBrush = NULL ;
	}

/*	for ( SI32 i = 0; i < MAX_PRIVATE_CHAT_DLG_NUM; ++i )
	{
		if ( m_hDlg[i] )
		{
			DestroyWindow( m_hDlg[i] );
		}
	}*/
}

void CPrivateChatDlg::Initialize()
{
	memset( m_strUserName, 0, MAX_PLAYER_NAME );
	return;
}

void CPrivateChatDlg::Destroy()
{
	for ( int i = 0 ; i < MAX_PRIVATE_CHAT_DLG_NUM ; i ++ )
		for ( int j = 0 ; j < 10 ; j ++ )
		{
			if ( m_bBitmap[i][j] )
			{
				DeleteObject(m_bBitmap[i][j]);
				m_bBitmap[i][j] = NULL ;
			}
		}

	return;
}

BOOL CPrivateChatDlg::SetCtlColorStatic(WPARAM wParam)
{
	HDC hdc = (HDC)wParam;
	char* pText1 = GetTxtFromMgr(244);
	char* pText2 = GetTxtFromMgr(599);
	if ( !strcmp(m_strUserName,pText1) || !strcmp(m_strUserName,pText2) ||
		 !strcmp(pclClient->pclCM->CR[1]->GetName(),pText1) ||
		 !strcmp(pclClient->pclCM->CR[1]->GetName(),pText2) )
		SetTextColor( hdc, RGB(0, 0, 255));
	else
		SetTextColor( hdc, COLOR_DIALOG_BLACK);
	SetBkColor( hdc, COLOR_DIALOG_YELLOW);
				
	return (BOOL)m_hAnswerEditBrush;
}

BOOL CPrivateChatDlg::SetCtlColorDlg(WPARAM wParam)
{
	HDC hdc = (HDC)wParam;
	//SetTextColor( hdc, COLOR_DIALOG_BLACK );
	//SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );

	return (BOOL)m_hAnswerDlgBrush;
}

void CPrivateChatDlg::Create( HWND hWnd, HINSTANCE hInst, CPrivateChatDlgMgr *pPrivateChatDlgMgr, SI32 siPrivateChatDlgIndex )
{
	//m_hInst = hInst;
	m_siPrivateChatDlgIndex = siPrivateChatDlgIndex;
	m_pPrivateChatDlgMgr = pPrivateChatDlgMgr;

	switch( m_siPrivateChatDlgIndex )
	{
	case 0:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc0 );
		break;
	case 1:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc1 );
		break;
	case 2:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc2 );
		break;
	case 3:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc3 );
		break;
	case 4:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc4 );
		break;
	case 5:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc5 );
		break;
	case 6:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc6 );
		break;
	case 7:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc7 );
		break;
	case 8:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc8 );
		break;
	case 9:
		m_hDlg[m_siPrivateChatDlgIndex] = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATECHAT), hWnd, StaticPrivateChatDlgProc9 );
		break;

	default:
		{char* pText = GetTxtFromMgr(2468);
			MessageBox( NULL, pText, "error", MB_OK|MB_TOPMOST );
		}
		break;

	}	

	Hide();
	m_bShow = false;

	g_PrivateChatRejectViewDlg.Create( hWnd, hInst, pPrivateChatDlgMgr );
	
	return;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc0(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 0 ].EditSubProc0( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc0(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 0 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc0( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 0 ].PrivateChatDlgProc0( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc0( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			if(pclClient->siServiceArea == ConstServiceArea_Netmarble)
			{
				HWND hTemp = GetDlgItem(hDlg, IDC_BUTTON_MINIHOME);
				ShowWindow(hTemp, SW_HIDE);
			}
			
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 0 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc0);
			}

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,0);
		}
		return TRUE;
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 0 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 0 ] );
				m_OldEditProc[ 0 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 1 ].EditSubProc1( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1);

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 1 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc1( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 1 ].PrivateChatDlgProc1( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc1( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 1 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc1);
			}

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,1);
		}
		return TRUE;	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);			
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);		
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 1 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 1 ] );
				m_OldEditProc[ 1 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 2 ].EditSubProc2( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 2 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc2( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 2 ].PrivateChatDlgProc2( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc2( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 2 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc2);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,2);
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);			
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 2 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 2 ] );
				m_OldEditProc[ 2 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc3(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 3 ].EditSubProc3( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc3(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 3 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc3( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 3 ].PrivateChatDlgProc3( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc3( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 3 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc3);
			}

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,3);

		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);			
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 3 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 3 ] );
				m_OldEditProc[ 3 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;

		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc4(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 4 ].EditSubProc4( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc4(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 4 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc4( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 4 ].PrivateChatDlgProc4( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc4( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 4 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc4);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,4);
			
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);			
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 4 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 4 ] );
				m_OldEditProc[ 4 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc5(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 5 ].EditSubProc5( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc5(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 5 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc5( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 5 ].PrivateChatDlgProc5( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc5( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 5 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc5);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,5);
			
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}

		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 5 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 5 ] );
				m_OldEditProc[ 5 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc6(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 6 ].EditSubProc6( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc6(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 6 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc6( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 6 ].PrivateChatDlgProc6( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc6( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 6 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc6);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}
			BitMapLoad(hDlg,6);
			
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 6 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 6 ] );
				m_OldEditProc[ 6 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc7(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 7 ].EditSubProc7( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc7(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 7 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc7( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 7 ].PrivateChatDlgProc7( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc7( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 7 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc7);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,7);
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 7 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 7 ] );
				m_OldEditProc[ 7 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}


LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc8(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 8 ].EditSubProc8( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc8(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 8 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc8( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 8 ].PrivateChatDlgProc8( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc8( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 8 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc8);
			}
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,8);
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 8 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 8 ] );
				m_OldEditProc[ 8 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}


LRESULT CALLBACK CPrivateChatDlg::StaticEditSubProc9(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_PrivateChatDlg[ 9 ].EditSubProc9( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CPrivateChatDlg::EditSubProc9(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CHAR :
		if( wParam == VK_RETURN )
		{
			if ( m_hDlg )
			{
				memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
				GetWindowText( hWnd, m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );

				// 스트링의 길이가 0보다 큰 경우만 보냄. 
				// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
				if ( strlen( m_strChatData ) > 0 )
				{
					Send();
					SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
				}
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], "" );
		}

		break;
	default:
		break;
    }

    return CallWindowProc( m_OldEditProc[ 9 ], hWnd, msg, wParam, lParam );
}

BOOL CALLBACK CPrivateChatDlg::StaticPrivateChatDlgProc9( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateChatDlg[ 9 ].PrivateChatDlgProc9( hDlg, iMsg, wParam, lParam );	
}

BOOL CALLBACK CPrivateChatDlg::PrivateChatDlgProc9( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd = NULL;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{

			m_hChatDataViewEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			m_hChatDataInputEdit[m_siPrivateChatDlgIndex] = GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT );

			g_FontManager.SetFont( hDlg, IDC_EDIT_RECV_CHAT_DATA );
			g_FontManager.SetFont( hDlg, IDC_EDIT_CHAT_INPUT );

			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] )
			{
				m_OldEditProc[ 9 ] = (WNDPROC)SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG)StaticEditSubProc9);
			}

			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				EmbedBrowserObject(hwnd);
				MoveWindow( hwnd, 8, 2, 73, 73, TRUE );
			}

			BitMapLoad(hDlg,9);
		}
		return TRUE;
	
	case WM_ACTIVATE:
		{
			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = false;
			}
			else
			{
				m_bActive[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, FALSE );
				KillTimer( hDlg, 0 );
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = false;
			}
		}
		break;	
	case WM_TIMER:
		{
			if ( ! m_bActive[ m_siPrivateChatDlgIndex ] )
			{
				m_bTimerStatus[ m_siPrivateChatDlgIndex ] = true;
				FlashWindow( hDlg, TRUE );
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RECV_CHAT_DATA ) == (HWND)lParam )
			{
				return SetCtlColorStatic(wParam);
			}
		}
		break;
	case WM_CTLCOLORDLG:
		{
			return SetCtlColorDlg(wParam);
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_REJECT:
				{
					Reject();
				}
				break;
			case IDC_BUTTON_MINIHOME:
				{
					if ( m_siCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_SEND:
				{
					memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_CHAT_INPUT ), m_strChatData, MAX_CHAT_DATA_LENGTH - 1 );
					strcat( m_strChatData, "" );

					// 스트링의 길이가 0보다 큰 경우만 보냄. 
					// 중복된 채팅 데이터도 막아야 됨... ( 장난삼아 계속 보낼 수 있으므로.. )
					if ( strlen( m_strChatData ) > 0 )
					{
						Send();
						SetWindowText( m_hChatDataInputEdit[ m_siPrivateChatDlgIndex ], "" );
					}
				}
				break;
			case IDC_BUTTON_REJECT_MGR:
				{
					g_PrivateChatRejectViewDlg.Show();
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
			hwnd = GetDlgItem(hDlg,IDC_STATIC_PRIVATEPROFILE);
			if(hwnd)
			{
				UnEmbedBrowserObject(hwnd);
			}
			
			if ( m_hChatDataInputEdit[m_siPrivateChatDlgIndex] && m_OldEditProc[ 9 ] != 0 )
			{
				SetWindowLong( m_hChatDataInputEdit[m_siPrivateChatDlgIndex], GWL_WNDPROC, (LONG) m_OldEditProc[ 9 ] );
				m_OldEditProc[ 9 ] = 0;
			}

			m_hDlg[m_siPrivateChatDlgIndex] = NULL;
		}

		break;
	}

	return FALSE;
}

bool CPrivateChatDlg::IsShow()
{
	return m_bShow;
}

void CPrivateChatDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}

	char* pText = GetTxtFromMgr(5066);
	Recv("♡♡♡",pText);

	pText = GetTxtFromMgr(5381);
	Recv("♡♡♡",pText);

	m_bShow = true;	
	//SetWindowText( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], "" );
	pclClient->PushEffect( EFFECT_WHISPERCHAT, 0, 0 );
	ShowWindow ( m_hDlg[m_siPrivateChatDlgIndex], SW_SHOWNOACTIVATE );

	return;
}

void CPrivateChatDlg::Hide()
{
	if ( ! m_bShow )
	{
		return;
	}

	m_bShow = false;
	SetWindowText( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], "" );

	KillTimer( m_hDlg[m_siPrivateChatDlgIndex], 0 );
	ShowWindow( m_hDlg[m_siPrivateChatDlgIndex], SW_HIDE );
	
	
	if ( m_pChatDataViewMgr )
	{
		delete m_pChatDataViewMgr;

		m_pChatDataViewMgr = new CChatDataMgr( true );
		m_pChatDataViewMgr->CreateMaxEmptyObj( MAX_CHAT_DATA_RECORD_NUM );
	}
	

	return;
}

void CPrivateChatDlg::Send()
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( g_SoundOnOffDlg.m_bWhisperChatReject )
	{
		char* pTitle = GetTxtFromMgr(2395);
		char* pText = GetTxtFromMgr(2396);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	if ( pclclient->m_pCAbuseFilter->CheckAbuse(m_strChatData) == false )
	{
		char* pTitle = GetTxtFromMgr(1853);
		char* pText = GetTxtFromMgr(1854);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return ;
	}

	Recv( (char*)pclClient->pclCM->CR[1]->GetName(), m_strChatData );

	m_pChatDataSendMgr->PushWhisper( WHISPER_CHAT_DATA_SEND, m_strChatData );

	LoadData();
}

void CPrivateChatDlg::Recv( char *strUserID, char *strChatRecvData )
{
	
	m_pChatDataViewMgr->Push( strUserID, strChatRecvData );
	LoadData();
	return;
}

bool CPrivateChatDlg::Pop ( stWhisperChatDataSend *pWhisperChatDataSend )
{
	stWhisperChatDataSend WhisperSendData;
	WhisperSendData.siToID = m_siCharUnique;
	
	if ( m_pChatDataSendMgr->Pop( WhisperSendData.strChatData, &WhisperSendData.siToID ) )
	{
		memcpy( pWhisperChatDataSend, &WhisperSendData, sizeof( stWhisperChatDataSend ) );
		return true;
	}

	return false;
}

void CPrivateChatDlg::LoadData()
{
	m_pChatDataViewMgr->LoadData( m_strChatViewData );

	
	SetWindowText( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], "" );
	SendMessage( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], EM_SETSEL, (WPARAM)-1, (LPARAM)-1 );
	SendMessage( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], EM_REPLACESEL, 0, (LPARAM)m_strChatViewData );
	/*
	SetWindowText( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], m_strChatViewData );
	
	SendMessage( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], WM_VSCROLL, SB_BOTTOM, 0 );
	*/

	return;
}

void CPrivateChatDlg::Set( SI32 siCharUnique, char *strUserName ,SI32 level)
{	
	m_siCharUnique = siCharUnique;

	SetWindowText( m_hChatDataViewEdit[m_siPrivateChatDlgIndex], "" );

	if ( strUserName != NULL )
	{
		char buffer[256] = "" ;
		MStrCpy ( m_strUserName, strUserName, MAX_PLAYER_NAME );
		sprintf(buffer,"%s ( Lv.%d )",strUserName,level);
		SetWindowText( m_hDlg[m_siPrivateChatDlgIndex], buffer );
	}

	
	Show();
	return;
}

bool CPrivateChatDlg::IsExist( SI32 siCharUnique, char *strUserName )
{
	if( m_siCharUnique == siCharUnique )
	{
		return true;
	}

	if ( strUserName == NULL )
	{
		return false;
	}

	if ( strlen( strUserName ) < 1 )
	{
		return false;
	}

	if ( m_strUserName == NULL )
	{
		return false;
	}

	if ( strlen( m_strUserName ) < 1 )
	{
		return false;
	}

	if ( strcmp( strUserName, m_strUserName ) == 0 )
	{
		m_siCharUnique = siCharUnique;
		return true;
	}

	return false;
}

void CPrivateChatDlg::Reject()
{	
	if ( m_pPrivateChatDlgMgr )
	{
		m_pPrivateChatDlgMgr->AddRejectUser( m_strUserName );
	}
	
	return;
}

void CPrivateChatDlg::BitMapLoad(HWND hDlg,SI32 num)
{
	m_bBitmap[num][0] = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface\\PrivateChat\\ggomg.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_bBitmap[num][0] )
		SendDlgItemMessage( hDlg, IDC_BUTTON_MINIHOME, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bBitmap[num][0]);

	m_bBitmap[num][1] = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface\\PrivateChat\\rejection.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_bBitmap[num][1] )
		SendDlgItemMessage( hDlg, IDC_BUTTON_REJECT, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bBitmap[num][1]);

	m_bBitmap[num][2] = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface\\PrivateChat\\rejectionlist.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_bBitmap[num][2] )
		SendDlgItemMessage( hDlg, IDC_BUTTON_REJECT_MGR, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bBitmap[num][2]);

	m_bBitmap[num][3] = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface\\PrivateChat\\send.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_bBitmap[num][3] )
		SendDlgItemMessage( hDlg, IDC_BUTTON_SEND, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bBitmap[num][3]);

	m_bBitmap[num][4] = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface\\PrivateChat\\admin.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_bBitmap[num][4] )
		SendDlgItemMessage( hDlg, IDC_STATIC_ADMINBITMAP, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bBitmap[num][4]);

	HWND hwnd = GetDlgItem(hDlg,IDC_STATIC_ADMINBITMAP);
	if(hwnd)	MoveWindow(hwnd,176,0,177,76,TRUE);
	hwnd = GetDlgItem(hDlg,IDC_BUTTON_REJECT);
	if(hwnd)	MoveWindow(hwnd,90,28,79,22,TRUE);
	hwnd = GetDlgItem(hDlg,IDC_BUTTON_REJECT_MGR);
	if(hwnd)	MoveWindow(hwnd,90,54,79,22,TRUE);
	hwnd = GetDlgItem(hDlg,IDC_BUTTON_SEND);
	if(hwnd)	MoveWindow(hwnd,288,220,63,21,TRUE);
	hwnd = GetDlgItem(hDlg,IDC_BUTTON_MINIHOME);
	if(hwnd)	MoveWindow(hwnd,90,2,79,22,TRUE);
}