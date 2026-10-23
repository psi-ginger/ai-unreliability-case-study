#include "RichEditchatMgrDlg.h"
#include "RichEditChatDataMgr.h"
#include "RichEditChatDataObj.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "../PartyDlg/PartyDlgBaseInfo.h"
#include "../PartyDlg/PartyDlg.h"

#include "../../Client/client.h"
#include "../../Client/Music/Music.h"

#include "../../Common/Message/message.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../../Common/Char/CharManager/CharManager.h"

#include "../ResidentsViewDlg/ResidentsViewDlg.h"
extern cltCommonLogic* pclClient;
//extern g_PartyDlg;

CRichEditChatMgrDlg::CRichEditChatMgrDlg()
{
	m_hRichEditModuleLoad = LoadLibrary("Riched20.dll");

	m_hDlg = NULL;
	m_hRichEdit = NULL;
	m_hRichEditDlg = NULL;
	m_hCurrentTabDlg = NULL;
	m_hChatDataInputEdit = NULL;

	m_pRichEditChatDataRecvMgr = new CRichEditChatDataMgr( true );
	m_pRichEditChatDataSendMgr = new CRichEditChatDataMgr( false );
	m_pChatStatusBarBuffer = new CChatStatusBarBuffer();

	m_strViewData = new char[ MAX_RICHEDIT_OBJ_SIZE ];
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );

	m_strChatData = new char[ MAX_CHAT_DATA_LENGTH ];
	memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );

	m_siCurrentTabMode = TAB_MODE_CHAT;

	Initialize();
}

CRichEditChatMgrDlg::~CRichEditChatMgrDlg()
{
	if ( m_pRichEditChatDataSendMgr )
	{
		delete m_pRichEditChatDataSendMgr;
		m_pRichEditChatDataSendMgr = NULL;
	}

	if ( m_pRichEditChatDataRecvMgr )
	{
		delete m_pRichEditChatDataRecvMgr;
		m_pRichEditChatDataRecvMgr = NULL;
	}

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
	
	if ( m_hRichEditDlg )
	{
		DestroyWindow( m_hRichEditDlg );
	}

	if (m_hDlg)
	{
		DestroyWindow( m_hDlg );
	}

	if ( m_pChatStatusBarBuffer )
	{
		delete m_pChatStatusBarBuffer;
		m_pChatStatusBarBuffer;
	}


	// RichEdit 2.0 dll 로드 한거 해제
	FreeLibrary( m_hRichEditModuleLoad );
}

void CRichEditChatMgrDlg::Initialize()
{
	if ( m_hRichEdit )
	{
		CHARFORMAT2 cf;

		memset( &cf, 0, sizeof( CHARFORMAT2 ));
		cf.cbSize = sizeof( CHARFORMAT2 );
		cf.dwMask = CFM_COLOR;
		
		cf.crTextColor = RGB( 0, 0, 0 );
		cf.dwEffects = 0;
		cf.yHeight = RICHEDIT_FONT_HEIGHT;
		char* pText = GetTxtFromMgr(1758);
		strcpy(cf.szFaceName, pText );
		SendMessage( m_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf );
	}

	return;
}

void CRichEditChatMgrDlg::Create( HINSTANCE hInst, HWND hWnd )
{	
	m_hInst = hInst;	
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_RICHEDIT_CHAT_MGR ), hWnd,  StaticRichEditChatMgrDlgProc );
	Hide();
	
	return;
}

LRESULT CALLBACK CRichEditChatMgrDlg::StaticEditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pRichEditChatMgrDlg->EditSubProc( hWnd, msg, wParam, lParam );    
}


LRESULT CALLBACK CRichEditChatMgrDlg::EditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
					Push();
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

BOOL CALLBACK CRichEditChatMgrDlg::StaticRichEditChatMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{	
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pRichEditChatMgrDlg->RichEditChatMgrDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CRichEditChatMgrDlg::RichEditChatMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			m_hTabCtrl = GetDlgItem( hDlg, IDC_TAB_RICHEDIT_CHAT_MGR );
			m_hRichEditDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_RICHEDIT_CHAT_VIEW ), m_hTabCtrl,  StaticRichEditChatViewDlgProc );
			
			/*
			MoveWindow( m_hRichEdit,
				rect.left, rect.top, rect.right-rect.left,
				rect.bottom-rect.top, TRUE);
			*/
			m_hCurrentTabDlg = m_hRichEditDlg;
			ShowWindow( m_hCurrentTabDlg, SW_SHOW );

			m_hChatDataInputEdit = GetDlgItem( hDlg, IDC_EDIT_CHATINPUT );
			SendMessage( m_hChatDataInputEdit, EM_LIMITTEXT, (WPARAM)MAX_CHAT_DATA_LENGTH - 1, 0 );
			m_OldEditProc = (WNDPROC)SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG)StaticEditSubProc);
		}
		return TRUE;
	
	case WM_NOTIFY:
		{
			LPNMHDR lpNmhdr =(LPNMHDR)lParam;
			
			switch ( lpNmhdr->idFrom )
			{
				case IDC_TAB_RICHEDIT_CHAT_MGR:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( m_hTabCtrl );

						switch ( index )
						{
						case TAB_MODE_CHAT:
							{
								m_siCurrentTabMode = TAB_MODE_CHAT;
								SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
								LoadChat();
							}
							break;
						
						case TAB_MODE_RESIDENTSCHAT:
							{
								m_siCurrentTabMode = TAB_MODE_RESIDENTSCHAT;
								SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 1, 0 );
								LoadResidentsChat();
							}
							break;

						case TAB_MODE_GUILDCHAT:
							{
								m_siCurrentTabMode = TAB_MODE_GUILDCHAT;
								SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 2, 0 );
								LoadGuildChat();
							}
							break;
						

						case TAB_MODE_PARTYCHAT:
							{
								m_siCurrentTabMode = TAB_MODE_PARTYCHAT;
								SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 3, 0 );
								LoadPartyChat();
							}
							break;
					
						default:
							return FALSE;
						}
					}
				}
				break;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			/*	case IDC_COMBO_SELECT_CHAT_MODE:
				{
					switch( HIWORD( wParam ) )
					{
						case CBN_SELCHANGE:
						{
							SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_CHAT_MODE  ), CB_GETCURSEL, 0, 0 );
							if ( SelectedIndex == CB_ERR )
							{
								return FALSE;
							}

							if( SelectedIndex > 3)
								SelectedIndex = 0;

							switch ( SelectedIndex )
							{
							case TAB_MODE_CHAT:
								m_siCurrentTabMode = TAB_MODE_CHAT;
							//	SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
								LoadChat();
								break;

							case TAB_MODE_RESIDENTSCHAT:
								m_siCurrentTabMode = TAB_MODE_RESIDENTSCHAT;
							//	SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
								LoadResidentsChat();
								break;

							case TAB_MODE_GUILDCHAT:
								m_siCurrentTabMode = TAB_MODE_GUILDCHAT;
							//	SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
								LoadGuildChat();
								break;


							case TAB_MODE_PARTYCHAT:
								m_siCurrentTabMode = TAB_MODE_PARTYCHAT;
							//	SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
								LoadPartyChat();
								break;

							default:
								return FALSE;
							}
						}
						break;
					}
				}
				break;
				*/
		
		
				case IDOK:					
					return TRUE;
					
				case IDCANCEL:					
					Hide();
					return TRUE;
			
					/*
					switch( HIWORD ( wParam ) )
					{
					}
					*/
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
			SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG) m_OldEditProc );

			m_hRichEditDlg = NULL;
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CRichEditChatMgrDlg::StaticRichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pRichEditChatMgrDlg->RichEditChatViewDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CRichEditChatMgrDlg::RichEditChatViewDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hRichEditDlg = hDlg;
			m_hRichEdit = CreateWindow( RICHEDIT_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT, 0, 0, 0, 0, hDlg, (HMENU)NULL, m_hInst, NULL );
			SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
			SetWindowText( m_hRichEdit, "" );
			char* pTitle = GetTxtFromMgr(244);
			char* pText = GetTxtFromMgr(1859);
			AddChat( pTitle, pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
		}
		return TRUE;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect( hDlg, &rect );
			MoveWindow( m_hRichEdit,
				rect.left, rect.top, rect.right-rect.left,
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

bool CRichEditChatMgrDlg::IsShow()
{
	return m_bShow;
}

void CRichEditChatMgrDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	TabCtrl_DeleteAllItems(m_hTabCtrl);
	//---------------------------------------------------
	// 텝 컨트롤의 탭 부분 기본 설정
	TC_ITEM item;
	// to use an image set this to TCIF_IMAGE  
	item.mask= TCIF_TEXT; 
	// index into tabs image list, set to -1 when not in use
	item.iImage = -1;

	item.lParam = 0;
	char* pText = GetTxtFromMgr(1214);
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 0, &item );

	pText = GetTxtFromMgr(1770);
	item.lParam = 1;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 1, &item );

	pText = GetTxtFromMgr(5360);
	item.lParam = 2;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 2, &item );		

	pText = GetTxtFromMgr(1221);
	item.lParam = 3;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 3, &item );	

	RECT rect;

	GetClientRect( m_hTabCtrl, &rect);
	TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);

	MoveWindow( m_hRichEditDlg,
		rect.left, rect.top, rect.right-rect.left,
		rect.bottom-rect.top, TRUE);


	//---------------------------------------------------

	ComboDelete(m_hDlg, IDC_COMBO_SELECT_CHAT_MODE);
	// 일반
	pText = GetTxtFromMgr(1214);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 마을
	pText = GetTxtFromMgr(1770);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText);
	// 상단
	pText = GetTxtFromMgr(5360);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 파티
	pText = GetTxtFromMgr(1221);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 광고
	pText = GetTxtFromMgr(1215);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 자막광고
	pText = GetTxtFromMgr(1856);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );

	if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.siType == RANKTYPE_CITYHALL )
	{
		pText = GetTxtFromMgr(1217);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
		pText = GetTxtFromMgr(1218);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
	{
		pText = GetTxtFromMgr(1219);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CRichEditChatMgrDlg::Hide()
{
	TabCtrl_DeleteAllItems(m_hTabCtrl);
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CRichEditChatMgrDlg::CharacterLogin()
{
	TabCtrl_DeleteAllItems(m_hTabCtrl);
	//---------------------------------------------------
	// 텝 컨트롤의 탭 부분 기본 설정
	TC_ITEM item;
	// to use an image set this to TCIF_IMAGE  
	item.mask= TCIF_TEXT; 
	// index into tabs image list, set to -1 when not in use
	item.iImage = -1;

	item.lParam = 0;
	char* pText = GetTxtFromMgr(1214);
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 0, &item );

	pText = GetTxtFromMgr(1770);
	item.lParam = 1;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 1, &item );

	pText = GetTxtFromMgr(5360);
	item.lParam = 2;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 2, &item );		

	pText = GetTxtFromMgr(1221);
	item.lParam = 3;
	item.pszText = pText;
	TabCtrl_InsertItem( m_hTabCtrl, 3, &item );	

	RECT rect;

	GetClientRect( m_hTabCtrl, &rect);
	TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);

	MoveWindow( m_hRichEditDlg,
		rect.left, rect.top, rect.right-rect.left,
		rect.bottom-rect.top, TRUE);


	//---------------------------------------------------

	ComboDelete(m_hDlg, IDC_COMBO_SELECT_CHAT_MODE);
	// 일반
	pText = GetTxtFromMgr(1214);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 마을
	pText = GetTxtFromMgr(1770);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText);
	// 상단
	pText = GetTxtFromMgr(5360);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 파티
	pText = GetTxtFromMgr(1221);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 광고
	pText = GetTxtFromMgr(1215);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	// 자막광고
	pText = GetTxtFromMgr(1856);
	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );

	if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.siType == RANKTYPE_CITYHALL )
	{
		pText = GetTxtFromMgr(1217);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
		pText = GetTxtFromMgr(1218);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
	{
		pText = GetTxtFromMgr(1219);

		SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE, CB_SETCURSEL, 0, 0 );
}

void CRichEditChatMgrDlg::SetFont( COLORREF Color, DWORD dwMask, DWORD dwEffects, char *FontKind, SI16 YHeight )
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

bool CRichEditChatMgrDlg::CheckData( char *PlayerName, char *ChatText )
{
	if ( strlen( PlayerName ) > MAX_PLAYER_NAME - 1 + 10 || strlen( ChatText ) > MAX_CHAT_DATA_LENGTH - 1 )
	{
		return false;
	}

	return true;
}

void CRichEditChatMgrDlg::ChatStore( char *PlayerName, char *ChatText, SI16 kind )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stRichEditChatRecv ChatRecv(kind);
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10 );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

}

SI32 CRichEditChatMgrDlg::CompareKey(char *data, SI32 type)
{
	switch(type)
	{
	case 0:
		{
			if(strncmp(data,"@",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(strncmp(data,"#",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(strncmp(data,"$",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_DEAULT;
		}
		break;
	case 1:
		{
			if(strncmp(data,"!",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(strncmp(data,"#",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(strncmp(data,"$",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_RESIDENTSCHAT;
		}
		break;
	case 2:
		{
			if(strncmp(data,"!",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(strncmp(data,"@",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(strncmp(data,"$",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_GUILDCHAT;
		}
		break;
	case 3:
		{
			if(strncmp(data,"!",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(strncmp(data,"@",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(strncmp(data,"#",1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else
				return TAB_MODE_PARTYCHAT;
		}
		break;
	}
}
void CRichEditChatMgrDlg::ChatRecv( char *PlayerName, char *ChatText, SI16 kind )
{	
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	
	stRichEditChatRecv ChatRecv(kind);
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10 );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );
	
	if ( m_siCurrentTabMode == TAB_MODE_CHAT )
	{
		if( kind == TAB_MODE_RESIDENTSCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_GREEN_COLOR  );
		else if( kind == TAB_MODE_GUILDCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_RED_COLOR  );
		else if( kind == TAB_MODE_PARTYCHAT )
			AddChat( PlayerName, ChatText,RICHEDIT_BLUE_COLOR ,RICHEDIT_YELLOW_COLOR   );
		else
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLACK_COLOR );
	}
	else if ( m_siCurrentTabMode == TAB_MODE_RESIDENTSCHAT )
	{
		if( kind == TAB_MODE_RESIDENTSCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_GREEN_COLOR  );
		else if( kind == TAB_MODE_GUILDCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_RED_COLOR  );
		else if( kind == TAB_MODE_PARTYCHAT )
			AddChat( PlayerName, ChatText,RICHEDIT_BLUE_COLOR ,RICHEDIT_YELLOW_COLOR   );
		else
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLACK_COLOR );
	}
	else if( m_siCurrentTabMode == TAB_MODE_GUILDCHAT)
	{
		if( kind == TAB_MODE_RESIDENTSCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_GREEN_COLOR  );
		else if( kind == TAB_MODE_GUILDCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_RED_COLOR  );
		else if( kind == TAB_MODE_PARTYCHAT )
			AddChat( PlayerName, ChatText,RICHEDIT_BLUE_COLOR ,RICHEDIT_YELLOW_COLOR   );
		else
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLACK_COLOR );
	}
	else if ( m_siCurrentTabMode == TAB_MODE_PARTYCHAT )
	{
		if( kind == TAB_MODE_RESIDENTSCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_GREEN_COLOR  );
		else if( kind == TAB_MODE_GUILDCHAT )
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_RED_COLOR  );
		else if( kind == TAB_MODE_PARTYCHAT )
			AddChat( PlayerName, ChatText,RICHEDIT_BLUE_COLOR ,RICHEDIT_YELLOW_COLOR   );
		else
			AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLACK_COLOR );
	}
	else
	{
		AddChat( PlayerName, ChatText,RICHEDIT_YELLOW_COLOR ,RICHEDIT_BLUE_COLOR   );
	}

	return;
}

void CRichEditChatMgrDlg::ResidentsChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	char buf[256]=" ";
	sprintf(buf, "마을:%s", PlayerName);
	strcpy(PlayerName,buf);
	
	stRichEditResidentsChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10);
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pResidentsViewDlg )
		pclclient->m_pResidentsViewDlg->AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_GREEN_COLOR );

	if ( pclclient->m_pResidentsViewDlg && !pclclient->m_pResidentsViewDlg->IsShow() )
	{	
		char szVillageChatMsg[ MAX_CHAT_DATA_LENGTH + MAX_PLAYER_NAME + 10 ] = "";	// 20 은 기타 표시용 [] :  뭐 요런거랑 공백
		sprintf( szVillageChatMsg, "[%s]:%s", PlayerName, ChatText  );
		pclclient->pclMessage->SetVillageChatMsg( szVillageChatMsg );
	}

	return;
}

void CRichEditChatMgrDlg::GuildChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	char buf[256]=" ";
	sprintf(buf, "상단:%s", PlayerName);
	strcpy(PlayerName,buf);

	stRichEditGuildChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10 );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );


//	if ( m_siCurrentTabMode == TAB_MODE_GUILDCHAT || m_siCurrentTabMode == TAB_MODE_CHAT )
//	{
//		AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_RED_COLOR );
//	}

	return;
}

void CRichEditChatMgrDlg::PartyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	char buf[256]=" ";
	sprintf(buf, "파티:%s", PlayerName);
	strcpy(PlayerName,buf);

	stRichEditPartyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10 );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	
//	if ( m_siCurrentTabMode == TAB_MODE_PARTYCHAT || m_siCurrentTabMode == TAB_MODE_CHAT )
//	{
//		AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_YELLOW_COLOR );
//	}

	g_PartyDlg.AddChat(PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_YELLOW_COLOR );

	
	return;

}

void CRichEditChatMgrDlg::MasterNotifyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stRichEditMasterNotifyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10);
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );

	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pResidentsViewDlg )
		pclclient->m_pResidentsViewDlg->AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	return;
}

void CRichEditChatMgrDlg::VillageNotifyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	
	stRichEditVillageNotifyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );

	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
	
	return;
}

void CRichEditChatMgrDlg::ResidentsNotifyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	
	stRichEditResidentsNotifyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10);
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
		
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	AddChat( PlayerName, ChatText, RICHEDIT_YELLOW_COLOR, RICHEDIT_BLUE_COLOR );

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pResidentsViewDlg )
		pclclient->m_pResidentsViewDlg->AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
	if ( pclclient->m_pResidentsViewDlg && !pclclient->m_pResidentsViewDlg->IsShow() )
	{	
		char szVillageChatMsg[ MAX_CHAT_DATA_LENGTH + MAX_PLAYER_NAME + 20 ] = "";	// 20 은 기타 표시용 [] :  뭐 요런거랑 공백
		sprintf( szVillageChatMsg, "[%s]:%s", PlayerName, ChatText  );
		pclclient->pclMessage->SetVillageChatMsg( szVillageChatMsg );
	}

	return;
}

void CRichEditChatMgrDlg::KingNotifyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stRichEditKingNotifyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );

	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}

	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME );
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );
	
	return;
}

void CRichEditChatMgrDlg::GoodsProductionNotifyChatRecv( char *PlayerName, char *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}
	
	stRichEditGoodsProductionNotifyChatRecv ChatRecv;
	SI32 DataSize = sizeof( ChatRecv );
	if ( DataSize > MAX_RICHEDIT_OBJ_SIZE )
	{
		return;
	}
	MStrCpy( ChatRecv.strUserID, PlayerName, MAX_PLAYER_NAME + 10);
	MStrCpy( ChatRecv.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );	
	m_pRichEditChatDataRecvMgr->AddData( (char*)&ChatRecv, DataSize );

	AddChat( PlayerName, ChatText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	return;
}

void CRichEditChatMgrDlg::AddChat( char *strUserID, char *strChatData, COLORREF NameColor, COLORREF TextColor )
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
	
	/*
	strcpy( m_strViewData, "" );	
	strcat( m_strViewData, "[" );
	strcat( m_strViewData, strUserID );
	strcat( m_strViewData, "] : " );	
	strcat( m_strViewData, strChatData );
	strcat( m_strViewData, "\r\n" );
	SetFont( Color );
	int iTotalTextLength = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL,iTotalTextLength, iTotalTextLength );
	SendMessage( m_hRichEdit, EM_REPLACESEL, 0, (LPARAM)m_strViewData );
	int iStartPos = iTotalTextLength;
	int iEndPos = GetWindowTextLength(m_hRichEdit);
	SendMessage( m_hRichEdit, EM_SETSEL, iStartPos, iEndPos );
	SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
	*/

	NextLineCount = SendMessage( m_hRichEdit, EM_GETLINECOUNT, 0, 0 );

	SendMessage( m_hRichEdit, EM_LINESCROLL, 0, NextLineCount - PreLineCount );
	return;
}

void CRichEditChatMgrDlg::AddName( char *PlayerName, COLORREF Color )
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

void CRichEditChatMgrDlg::AddText( char *ChatText, COLORREF Color )
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

// 일반 채팅
void CRichEditChatMgrDlg::LoadChat()
{
	SI32 count = 0;
	CRichEditChatDataObj *pObj = ( CRichEditChatDataObj * )m_pRichEditChatDataRecvMgr->GetExistList()->GetFirst();
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	SetWindowText( m_hRichEdit, "" );
	char* pTitle = GetTxtFromMgr(244);
	char* pText = GetTxtFromMgr(1859);
	AddChat( pTitle, pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	while ( pObj )
	{
		stRichEditChatMgrHeader *pHeader = (stRichEditChatMgrHeader *)pObj->GetDataPtr();

		switch( pHeader->siMsgKind )
		{

	

		case MSG_RICHEDIT_CHAT_RECV:
			{
				stRichEditChatRecv *pChatRecv = (stRichEditChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;

		case MSG_RICHEDIT_MASTERNOTIFYCHAT_RECV:
			{
				stRichEditMasterNotifyChatRecv *pChatRecv = (stRichEditMasterNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;

		case MSG_RICHEDIT_VILLAGENOTIFYCHAT_RECV:
			{
				stRichEditVillageNotifyChatRecv *pChatRecv = (stRichEditVillageNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;

		case MSG_RICHEDIT_RESIDENTSNOTIFYCHAT_RECV:
			{
				stRichEditResidentsNotifyChatRecv *pChatRecv = (stRichEditResidentsNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;

		case MSG_RICHEDIT_GOODSPRODUCTIONNOTIFYCHAT_RECV:
			{
				stRichEditGoodsProductionNotifyChatRecv *pChatRecv = (stRichEditGoodsProductionNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		}
		
		pObj = ( CRichEditChatDataObj * )pObj->GetNext();
	}

	return;
}


// 주민 채팅
void CRichEditChatMgrDlg::LoadResidentsChat()
{	
	CRichEditChatDataObj *pObj = ( CRichEditChatDataObj * )m_pRichEditChatDataRecvMgr->GetExistList()->GetFirst();
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	strcpy( m_strViewData, "" );
	

	
	SetWindowText( m_hRichEdit, "" );
	AddChat( "운영자", "군주 온라인에 오신 것을 환영합니다.", RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

	while ( pObj )
	{
		stRichEditChatMgrHeader *pHeader = (stRichEditChatMgrHeader *)pObj->GetDataPtr();
		
		switch( pHeader->siMsgKind )
		{
		case MSG_RICHEDIT_RESIDENTSCHAT_RECV:
			{
				stRichEditResidentsChatRecv *pChatRecv = (stRichEditResidentsChatRecv *)pObj->GetDataPtr();											
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;

		case MSG_RICHEDIT_MASTERNOTIFYCHAT_RECV:
			{
				stRichEditMasterNotifyChatRecv *pChatRecv = (stRichEditMasterNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_VILLAGENOTIFYCHAT_RECV:
			{
				stRichEditVillageNotifyChatRecv *pChatRecv = (stRichEditVillageNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_RESIDENTSNOTIFYCHAT_RECV:
			{
				stRichEditResidentsNotifyChatRecv *pChatRecv = (stRichEditResidentsNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_GOODSPRODUCTIONNOTIFYCHAT_RECV:
			{
				stRichEditGoodsProductionNotifyChatRecv *pChatRecv = (stRichEditGoodsProductionNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;		
		}
		
		pObj = ( CRichEditChatDataObj * )pObj->GetNext();
	}

	return;
}

//  길드 채팅
void CRichEditChatMgrDlg::LoadGuildChat()
{
	CRichEditChatDataObj *pObj = ( CRichEditChatDataObj * )m_pRichEditChatDataRecvMgr->GetExistList()->GetFirst();
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	strcpy( m_strViewData, "" );

	SetWindowText( m_hRichEdit, "" );
	AddChat( "운영자", "군주 온라인에 오신 것을 환영합니다.", RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );


	while ( pObj )
	{
		stRichEditChatMgrHeader *pHeader = (stRichEditChatMgrHeader *)pObj->GetDataPtr();

		switch( pHeader->siMsgKind )
		{
		case MSG_RICHEDIT_GUILDCHAT_RECV:
			{
				stRichEditGuildChatRecv *pChatRecv = (stRichEditGuildChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_MASTERNOTIFYCHAT_RECV:
			{
				stRichEditMasterNotifyChatRecv *pChatRecv = (stRichEditMasterNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_VILLAGENOTIFYCHAT_RECV:
			{
				stRichEditVillageNotifyChatRecv *pChatRecv = (stRichEditVillageNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_RESIDENTSNOTIFYCHAT_RECV:
			{
				stRichEditResidentsNotifyChatRecv *pChatRecv = (stRichEditResidentsNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_GOODSPRODUCTIONNOTIFYCHAT_RECV:
			{
				stRichEditGoodsProductionNotifyChatRecv *pChatRecv = (stRichEditGoodsProductionNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;		
		}

		pObj = ( CRichEditChatDataObj * )pObj->GetNext();
	}

	return;
}


// 파티 채팅
void CRichEditChatMgrDlg::LoadPartyChat()
{
	CRichEditChatDataObj *pObj = ( CRichEditChatDataObj * )m_pRichEditChatDataRecvMgr->GetExistList()->GetFirst();
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	strcpy( m_strViewData, "" );

	SetWindowText( m_hRichEdit, "" );
	AddChat( "운영자", "군주 온라인에 오신 것을 환영합니다.", RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );


	while ( pObj )
	{
		stRichEditChatMgrHeader *pHeader = (stRichEditChatMgrHeader *)pObj->GetDataPtr();

		switch( pHeader->siMsgKind )
		{
		case MSG_RICHEDIT_PARTYCHAT_RECV:
			{
				stRichEditPartyChatRecv *pChatRecv = (stRichEditPartyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_MASTERNOTIFYCHAT_RECV:
			{
				stRichEditMasterNotifyChatRecv *pChatRecv = (stRichEditMasterNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_VILLAGENOTIFYCHAT_RECV:
			{
				stRichEditVillageNotifyChatRecv *pChatRecv = (stRichEditVillageNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_RESIDENTSNOTIFYCHAT_RECV:
			{
				stRichEditResidentsNotifyChatRecv *pChatRecv = (stRichEditResidentsNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;
		case MSG_RICHEDIT_GOODSPRODUCTIONNOTIFYCHAT_RECV:
			{
				stRichEditGoodsProductionNotifyChatRecv *pChatRecv = (stRichEditGoodsProductionNotifyChatRecv *)pObj->GetDataPtr();
				AddChat( pChatRecv->strUserID, pChatRecv->strChatData, pChatRecv->sHeader.NameColor, pChatRecv->sHeader.TextColor );
			}
			break;		
		}
		
		pObj = ( CRichEditChatDataObj * )pObj->GetNext();
	}

	return;
}


void CRichEditChatMgrDlg::Push()
{
	SI32 siSelectedIndex = 0;
	SI32 siSelectedMode = m_siCurrentTabMode;
	SI32 siSelectedCombo = SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE  ), CB_GETCURSEL, 0, 0 );
	
	char pSelectText[50] = "";

	SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_SELECT_CHAT_MODE  ), CB_GETLBTEXT, siSelectedCombo, (LPARAM)pSelectText );
	//	char* pSelectText;

	char* pText[9];

	// 일반
	pText[0]= GetTxtFromMgr(1214);
	// 마을
	pText[1]= GetTxtFromMgr(1770);
	// 상단
	pText[2]= GetTxtFromMgr(5560);
	// 파티
	pText[3]= GetTxtFromMgr(1221);
	// 광고
	pText[4]= GetTxtFromMgr(1215);
	// 자막광고
	pText[5]= GetTxtFromMgr(1856);
	// 마을공지
	pText[6]= GetTxtFromMgr(1217);
	// 주민공지
	pText[7] = GetTxtFromMgr(1218);
	// 군주공지
	pText[8] = GetTxtFromMgr(1219);

	stRichEditChatDataSend SendData;
	// 일반
	if ( strcmp( pSelectText, pText[0] ) == 0 )				
	{
		SI32 siCompareKey = CompareKey(m_strChatData, siSelectedCombo);

		if( siCompareKey == TAB_MODE_DEAULT )
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_RESIDENTSCHAT )
		{
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_GUILDCHAT )
		{
			SendData.si16Msg = GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_PARTYCHAT )
		{
			SendData.si16Msg = PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
	}
	// 주민채팅
	else if ( strcmp( pSelectText, pText[1] ) == 0 )			
	{
		SI32 siCompareKey = CompareKey(m_strChatData, siSelectedCombo);

		if( siCompareKey == TAB_MODE_DEAULT )
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_RESIDENTSCHAT )
		{
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_GUILDCHAT )
		{
			SendData.si16Msg = GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_PARTYCHAT )
		{
			SendData.si16Msg = PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
	}
	// 상단채팅
	else if ( strcmp( pSelectText, pText[2] ) == 0 )			
	{
		SI32 siCompareKey = CompareKey(m_strChatData, siSelectedCombo);

		if( siCompareKey == TAB_MODE_DEAULT )
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_RESIDENTSCHAT )
		{
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_GUILDCHAT )
		{
			SendData.si16Msg = GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_PARTYCHAT )
		{
			SendData.si16Msg = PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
	}
	// 파티채팅
	else if ( strcmp( pSelectText, pText[3] ) == 0 )
	{
		SI32 siCompareKey = CompareKey(m_strChatData, siSelectedCombo);

		if( siCompareKey == TAB_MODE_DEAULT )
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_RESIDENTSCHAT )
		{
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_GUILDCHAT )
		{
			SendData.si16Msg = GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		else if( siCompareKey == TAB_MODE_PARTYCHAT )
		{
			SendData.si16Msg = PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
	}
	// 광고
	else if ( strcmp( pSelectText, pText[4] ) == 0 )
	{
		SendData.si16Msg = HEAD_CHAT_DATA_SEND;
		strcpy( SendData.strChatData, m_strChatData );
		m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
	}
	// 자막광고
	else if ( strcmp( pSelectText, pText[5] ) == 0 )
	{
		SendData.si16Msg = NEWS_CHAT_DATA_SEND;
		strcpy( SendData.strChatData, m_strChatData );
		m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
	}
	// 마을공지
	else if ( strcmp( pSelectText, pText[6] ) == 0 )
	{
		SendData.si16Msg = TOWN_CHAT_DATA_SEND;
		strcpy( SendData.strChatData, m_strChatData );
		m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
	}
	// 주민공지
	else if ( strcmp( pSelectText, pText[7] ) == 0 )
	{
		SendData.si16Msg = RESIDENTSNOTIFY_CHAT_DATA_SEND;
		strcpy( SendData.strChatData, m_strChatData );
		m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
	}
	// 군주공지
	else if ( strcmp( pSelectText, pText[8] ) == 0 )
	{
		SendData.si16Msg = KINGNOTIFY_CHAT_DATA_SEND;
		strcpy( SendData.strChatData, m_strChatData );
		m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
	}

/*	if(siSelectedCombo ==0 && siSelectedMode ==TAB_MODE_CHAT) //일반채팅
	{
		siSelectedIndex =0;
	}
	else if(siSelectedCombo ==0 && siSelectedMode ==TAB_MODE_RESIDENTSCHAT)//주민채팅
	{
		siSelectedIndex =4	;
	}
	else if(siSelectedCombo ==0 && siSelectedMode ==TAB_MODE_GUILDCHAT ) //상단 채팅 
	{
		siSelectedIndex= 7;
	}
	else if( siSelectedCombo ==0 && siSelectedMode ==TAB_MODE_PARTYCHAT )  //파티 채팅
	{
		siSelectedIndex= 6;
	}

    else if(siSelectedCombo ==2)
	{
		siSelectedIndex =5 ;
	}

	else if(siSelectedCombo ==3 && siSelectedMode ==TOWN_CHAT_DATA_SEND ) //마을 공지
	{
		siSelectedIndex =2 ;
	}

	else if(siSelectedCombo ==3 && siSelectedMode ==RESIDENTSNOTIFY_CHAT_DATA_SEND) //주민공지
	{
		siSelectedIndex =3 ;
	}
	else
	{
		siSelectedIndex = siSelectedCombo;
	}



	stRichEditChatDataSend SendData;

	switch( siSelectedIndex )
	{
	case 0:
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 1:
		{
			SendData.si16Msg = HEAD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 2:
		{
			SendData.si16Msg = TOWN_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 3:
		{
			SendData.si16Msg = RESIDENTSNOTIFY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 4:
		{		
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 5:
		{		
			SendData.si16Msg = NEWS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 6:
		{
			SendData.si16Msg =PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 7:
		{
            SendData.si16Msg =GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 8:
		{
			SendData.si16Msg =GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, m_strChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;

	}
*/
	return;
}

void CRichEditChatMgrDlg::Push( SI16 SelectedIndex, char *ChatData )
{
	stRichEditChatDataSend SendData;

	switch( SelectedIndex )
	{
	case 0:
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 1:
		{
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 2:
		{
			SendData.si16Msg = GUILD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 3:
		{
			SendData.si16Msg = PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 4:
		{		
			SendData.si16Msg = HEAD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 5:
		{		
			SendData.si16Msg = NEWS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 6:
		{
			SendData.si16Msg = TOWN_CHAT_DATA_SEND;	
				strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 7:
		{
			SendData.si16Msg = RESIDENTSNOTIFY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 8:
		{
			SendData.si16Msg = KINGNOTIFY_CHAT_DATA_SEND;
				strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;

/*	switch( SelectedIndex )
	{
	case 0:
		{
			SendData.si16Msg = CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 1:
		{
			SendData.si16Msg = HEAD_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 2:
		{
			SendData.si16Msg = TOWN_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 3:
		{
			SendData.si16Msg = RESIDENTSNOTIFY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 4:
		{		
			SendData.si16Msg = RESIDENTS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 5:
		{		
			SendData.si16Msg = NEWS_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;
	case 6:
		{
			SendData.si16Msg =PARTY_CHAT_DATA_SEND;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
		break;

	case 7:
		{
			SendData.si16Msg =GUILD_CHAT_DATA_SEND;;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
	case 8:
		{
			SendData.si16Msg =GUILD_CHAT_DATA_SEND;;
			strcpy( SendData.strChatData, ChatData );
			m_pRichEditChatDataSendMgr->AddData( (char*)&SendData, sizeof( stRichEditChatDataSend ) );
		}
*/
	}
	
	return;
}

bool CRichEditChatMgrDlg::Pop( stRichEditChatDataSend *pSendData )
{
	return m_pRichEditChatDataSendMgr->Pop( pSendData );
}

CRichEditChatDataMgr *CRichEditChatMgrDlg::GetRichEditChatDataRecvMgrPtr()
{
	return m_pRichEditChatDataRecvMgr;
}