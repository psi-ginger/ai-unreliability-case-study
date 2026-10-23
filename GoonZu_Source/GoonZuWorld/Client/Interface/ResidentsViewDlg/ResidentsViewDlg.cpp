#include "../../Client/client.h"

//------------------------------------
// CommonLogic
//------------------------------------
#include "MsgType.h"
#include "CommonLogic.h"
#include "MsgType-System.h"
#include "Msg/MsgType-Village.h"
#include "MsgType-Person.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"



#include "SMLib\LibList\LibListMgr.h"
#include "SMLib\LibList\LibList.h"

#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../Music/Music.h"

#include "../../../lib/WebHTML/WebHTML.h"


#include "../RichEditChatMgrDlg/RichEditChatDataObj.h"
#include "../RichEditChatMgrDlg/RichEditChatDataMgr.h"



#include "ResidentsViewDlg.h"

extern cltCommonLogic* pclClient;

CResidentsViewDlg::CResidentsViewDlg()
{
	m_hRichEditModuleLoad = LoadLibrary("Riched20.dll");

	m_hDlg = NULL;
	m_hRichEdit = NULL;
	m_hResidentsViewListCtrl = NULL;
	m_bShow = false;

	m_strChatData = new char[ MAX_CHAT_DATA_LENGTH ];
	memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );

	m_strViewData = new char[ MAX_RICHEDIT_OBJ_SIZE ];
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );

	m_pVillageResidentsList = new cltVillageResidentsList[ MAX_RESIDENTS_PER_VILLAGE ];

	Init();
}

CResidentsViewDlg::~CResidentsViewDlg()
{
	if ( m_strChatData )
	{
		delete [] m_strChatData;
		m_strChatData = NULL;
	}

	if ( m_strViewData )
	{
		delete [] m_strViewData;
		m_strViewData = NULL;
	}

	
	if( m_pVillageResidentsList )
	{
		delete [] m_pVillageResidentsList;
		m_pVillageResidentsList = NULL;
	}

	/*
	if ( m_hRichEdit )
	{
		DestroyWindow( m_hRichEdit );		
	}
	*/

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}

	// RichEdit 2.0 dll 로드 한거 해제
	FreeLibrary( m_hRichEditModuleLoad );
}

void CResidentsViewDlg::Init()
{
	m_siSelectedIndex = -1;

	memset( m_pVillageResidentsList, 0, sizeof( cltVillageResidentsList ) * MAX_RESIDENTS_PER_VILLAGE );

	return;
}

void CResidentsViewDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_RESIDENTSVIEW ), hParent, StaticResidentsViewDlgProc );
	
	Hide();

	return;
}

LRESULT CALLBACK CResidentsViewDlg::StaticEditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pResidentsViewDlg->EditSubProc( hWnd, msg, wParam, lParam );
}


LRESULT CALLBACK CResidentsViewDlg::EditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
					SI32 siSelectedIndex = SendDlgItemMessage( m_hDlg, IDC_COMBO_SELECT_CHATMODE, CB_GETCURSEL, 0, 0 );
					
					if ( siSelectedIndex == CB_ERR )
					{
						break;
					}

					cltClient *pclclient = (cltClient*)pclClient;

					switch( siSelectedIndex )
					{
					// 주민 채팅
					case 0:
						// 4 가 주민 채팅임
						pclclient->m_pRichEditChatMgrDlg->Push( 1, m_strChatData );
						break;
					// 주민 공지
					case 1:
						// 4 가 주민 공지임
						pclclient->m_pRichEditChatMgrDlg->Push( 7, m_strChatData );
						break;
					}
						
					
					
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

BOOL CALLBACK CResidentsViewDlg::StaticResidentsViewDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pResidentsViewDlg->ResidentsViewDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CResidentsViewDlg::ResidentsViewDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch( iMsg )
	{
		case WM_INITDIALOG:
			{
				if(pclClient->siServiceArea == ConstServiceArea_Netmarble)
				{
					HWND hTemp=GetDlgItem(hDlg,IDC_BUTTON_GGOMZI);
					ShowWindow(hTemp, SW_HIDE);
				}

				
				InitCommonControls();
				m_hDlg = hDlg;

				m_hResidentsViewListCtrl = GetDlgItem( hDlg, IDC_LIST_RESIDENTS );

				ListView_SetExtendedListViewStyle( m_hResidentsViewListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );

				LVCOLUMN Col;			
				
				Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				Col.fmt = LVCFMT_CENTER;
				
				Col.cx = 100;
				char* pText = GetTxtFromMgr(1984);
				Col.pszText = pText;
				Col.iSubItem = 0;
				SendMessage( m_hResidentsViewListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

				Col.cx = 38;
				pText = GetTxtFromMgr(1506);
				Col.pszText = pText;
				Col.iSubItem = 1;
				SendMessage( m_hResidentsViewListCtrl, LVM_INSERTCOLUMN, 1, (LPARAM)&Col );

				Col.cx = 110;
				pText = GetTxtFromMgr(2447);
				Col.pszText = pText;
				Col.iSubItem = 2;
				SendMessage( m_hResidentsViewListCtrl, LVM_INSERTCOLUMN, 2, (LPARAM)&Col );
				
				Col.cx = 60;
				pText = GetTxtFromMgr(2448);
				Col.pszText = pText;
				Col.iSubItem = 3;
				SendMessage( m_hResidentsViewListCtrl, LVM_INSERTCOLUMN, 3, (LPARAM)&Col );
				

				pText = GetTxtFromMgr(1216);
				SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHATMODE, CB_ADDSTRING, 0, (LPARAM)pText );
				pText = GetTxtFromMgr(1218);
				SendDlgItemMessage( hDlg, IDC_COMBO_SELECT_CHATMODE, CB_ADDSTRING, 0, (LPARAM)pText );
				
				SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_CHATMODE ), CB_SETCURSEL, 0, 0 );

				//---------------------------------
				// HTML 을 초기화한다.
				//---------------------------------
				//==================================			
				HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
				if ( hwnd )
				{
					EmbedBrowserObject(hwnd);
				}
				
				//MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 3, 192, 73, 73, TRUE );
				MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 5, 350, 73, 73, TRUE );
				//==================================
				
				m_hRichEdit = CreateWindow( RICHEDIT_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT, 0, 1, 327, 135, hDlg, (HMENU)NULL, pclClient->GetInstance(), NULL );
				SendMessage( m_hRichEdit, EM_HIDESELECTION, 1, 0 );
				SetWindowText( m_hRichEdit, "" );
				ShowWindow( m_hRichEdit, SW_SHOW );
				
				m_hChatDataInputEdit = GetDlgItem( hDlg, IDC_EDIT_CHATINPUT );
				SendMessage( m_hChatDataInputEdit, EM_LIMITTEXT, (WPARAM)MAX_CHAT_DATA_LENGTH - 1, 0 );
				m_OldEditProc = (WNDPROC)SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG)StaticEditSubProc);
			}
			return TRUE;
		case WM_SIZE:
			{
				//============================
				// HTML Resize 를 한다.
				//============================
				//==================================
				HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE );
				if( hwnd )
				{
					ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
				}
			}
			break;
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
				
				
				if ( hdr->hwndFrom == m_hResidentsViewListCtrl )
				{
					switch( hdr->code )
					{
					case LVN_ITEMCHANGED:
						{
							if ( nia->iItem >= 0 )
							{
								if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
								{
									if ( m_pVillageResidentsList[ nlv->iItem ].clPerson.GetName() != NULL )
									{
										SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_CHARNAME ), m_pVillageResidentsList[ nlv->iItem ].clPerson.GetName() );

/*										if ( m_pVillageResidentsList[ nlv->iItem ].strAccountID != NULL )
										{
											char ProfileURL[MAX_PATH];

											pclClient->GetCZProfileUrl( ProfileURL, m_pVillageResidentsList[ nlv->iItem ].strAccountID );
											
											HWND hWnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
											DisplayHTMLPage(hWnd, ProfileURL);
										}*/

										m_siSelectedIndex = nlv->iItem;
									}
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
				switch( LOWORD( wParam ) )
				{
				case IDC_BUTTON_GGOMZI:
					{
						if ( m_siSelectedIndex > -1 )
						{	
							if ( m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID > 0 )
							{
								if ( m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.GetName() != NULL )
								{								
									cltGameMsgRequest_Userid clUserid( (char*)m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.GetName() );
									cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
									pclClient->SendMsgToServer((char*)&clMsg);
								}
							}
						}
					}
					break;
				case IDC_BUTTON_WHISPER:
					{
						if ( m_siSelectedIndex > -1 )
						{	
							if ( m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID > 0 )
							{
								if ( pclClient->pclCM->CR[1]->pclCI->GetPersonID() == m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID )
								{
									char* pTitle = GetTxtFromMgr(2449);
									char* pText = GetTxtFromMgr(2450);
									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
									return FALSE;
								}
								
								cltClient *pclclient = (cltClient *)pclClient;
								pclclient->RequestSecretChatByPersonID( m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID );
							}
						}
					}
					break;
				case IDC_BUTTON_DETAILINFO:
					{
						if ( m_siSelectedIndex > -1 )
						{	
							if ( m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID > 0 )
							{
								cltGameMsgRequest_PersonPrivateInfoByPersonID clinfo(m_pVillageResidentsList[ m_siSelectedIndex ].clPerson.siPersonID);
								cltMsg clMsg(GAMEMSG_REQUEST_PERSONPRIVATEINFOBYPERSONID, sizeof(clinfo), (char*)&clinfo);
								pclClient->SendMsgToServer((char*)&clMsg);
							}
						}
					}
					break;
				case IDC_BUTTON_REFRESH:
					{
						cltGameMsgRequest_ResidentsList clResidentsList( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage );	
						cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTSLIST, sizeof(clResidentsList), (char*)&clResidentsList );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
					break;
				case IDC_BUTTON_GOTOHOME:
					{
						if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage < 1 )
						{
							char* pTitle = GetTxtFromMgr(2451);
							char* pText = GetTxtFromMgr(2452);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}
						
						if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage == pclClient->pclCM->CR[1]->GetCurrentVillageUnique() )
						{
							char* pTitle = GetTxtFromMgr(2453);
							char* pText = GetTxtFromMgr(2454);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}

						if ( pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON) == true )
						{
							char* pTitle = GetTxtFromMgr(2455);
							char* pText = GetTxtFromMgr(2456);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}
						// 영자나라에 있으면 불가능
						if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
							pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
						{
							return FALSE ;
						}

						char *HomeVillageName = pclClient->pclVillageManager->GetName(pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage);
						if ( HomeVillageName == NULL )
						{
							return FALSE;
						}
						
						SI32 HomeVillageIndex = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;
						if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(14000) > 0 )
						{
							char buffer[256];
							char* pText = GetTxtFromMgr(1893);
							wsprintf(buffer,pText, HomeVillageName );
							pText = GetTxtFromMgr(1894);
							if (  MessageBox(NULL, pText, buffer, MB_ICONQUESTION | MB_YESNO) == IDYES )
							{
								pclClient->PushEffect( EFFECT_FANPRIEST_WARP, 0, 0 );
								cltMsg clMsg(GAMEMSG_REQUEST_RESIDENTWARPVILLAGE, sizeof(HomeVillageIndex), (char*)&HomeVillageIndex );
								pclClient->SendMsgToServer((char*)&clMsg);
							}
						}
						else
						{
							char* pTitle = GetTxtFromMgr(1891);
							char* pText = GetTxtFromMgr(1895);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						}
					}
					break;
				case IDCANCEL:
					Hide();
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
				SetWindowLong( m_hChatDataInputEdit, GWL_WNDPROC, (LONG) m_OldEditProc );
				
				//---------------------------------
				// HTML 을 해제한다.
				//---------------------------------
				//==================================
				HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
				if( hwnd )
				{
					DoPageAction(hwnd, WEBPAGE_STOP);
					UnEmbedBrowserObject(hwnd);
				}
				
				m_hDlg = NULL;
			}
			break;
	}
	return FALSE;
}

bool CResidentsViewDlg::IsShow()
{
	return m_bShow;
}

void CResidentsViewDlg::Show()
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage < 1 )
	{
		char* pTitle = GetTxtFromMgr(2457);
		char* pText = GetTxtFromMgr(2452);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	if ( IsShow() )
	{
		Hide();
		return;
	}
	
	cltGameMsgRequest_ResidentsList clResidentsList( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage );	
	cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTSLIST, sizeof(clResidentsList), (char*)&clResidentsList );
	pclClient->SendMsgToServer((char*)&clMsg);

	LoadResidentsChat();

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CResidentsViewDlg::Hide()
{
	ShowWindow( m_hDlg, SW_HIDE );
	m_bShow = false;

	return;
}

void CResidentsViewDlg::RecvResidentsList( cltVillageResidentsList *pclPersonList, UI32 ListCount )
{
	Init();

	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_CHARNAME ), "" );

	ListView_DeleteAllItems( m_hResidentsViewListCtrl );

	memcpy( m_pVillageResidentsList, pclPersonList, ListCount * sizeof( cltVillageResidentsList ) );

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	
	
	Li.iItem = 0;	
	Li.iSubItem = 0;

	SI32 Count = 0;
	
	char strLevel[ 8 ];
	char strMilitaryMerit[ 8 ];
	char strRank[ 128 ];
	char VillageRank[ 128 ];

	for ( int i = 0; i < MAX_RESIDENTS_PER_VILLAGE; ++i )
	{
		if ( m_pVillageResidentsList[ i ].clPerson.siPersonID > 0 )
		{			
			Li.iItem = Count++;
			Li.iSubItem = 0;			
			Li.pszText = m_pVillageResidentsList[ i ].clPerson.szName;
			SendMessage( m_hResidentsViewListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 1;
			sprintf( strLevel, "%d", m_pVillageResidentsList[ i ].siLevel );
			Li.pszText = strLevel;
			SendMessage( m_hResidentsViewListCtrl, LVM_SETITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 2;
			
			if ( m_pVillageResidentsList[ i ].clSimpleRank.GetVillageUnique() > 0 )
			{
				if ( GetRankName( &m_pVillageResidentsList[ i ].clSimpleRank, VillageRank ) == TRUE )
				{
					strcpy( strRank, VillageRank );
				}
				else
				{
					char* pText = GetTxtFromMgr(1216);
					strcpy( strRank, pText );
				}
			}
			else
			{
				char* pText = GetTxtFromMgr(1216);
				strcpy( strRank, pText );
			}
			Li.pszText = strRank;
			SendMessage( m_hResidentsViewListCtrl, LVM_SETITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 3;
			sprintf( strMilitaryMerit, "%d", m_pVillageResidentsList[ i ].siMilitaryMerit );
			Li.pszText = strMilitaryMerit;
			SendMessage( m_hResidentsViewListCtrl, LVM_SETITEM, 0, (LPARAM)&Li );
		}
	}

	return;
}

void CResidentsViewDlg::LoadResidentsChat()
{
	cltClient *pclclient = (cltClient *)pclClient;
	CRichEditChatDataObj *pObj = ( CRichEditChatDataObj * )pclclient->m_pRichEditChatMgrDlg->GetRichEditChatDataRecvMgrPtr()->GetExistList()->GetFirst();
	memset( m_strViewData, 0, MAX_RICHEDIT_OBJ_SIZE );
	strcpy( m_strViewData, "" );

	SetWindowText( m_hRichEdit, "" );
	char* pTitle = GetTxtFromMgr(244);
	char* pText = GetTxtFromMgr(1859);
	AddChat( pTitle, pText, RICHEDIT_BLUE_COLOR, RICHEDIT_BLUE_COLOR );

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

void CResidentsViewDlg::SetFont( COLORREF Color, DWORD dwMask, DWORD dwEffects, char *FontKind, SI16 YHeight )
{
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

void CResidentsViewDlg::AddChat( char *strUserID, char *strChatData, COLORREF NameColor, COLORREF TextColor )
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

void CResidentsViewDlg::AddName( char *PlayerName, COLORREF Color )
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

void CResidentsViewDlg::AddText( char *ChatText, COLORREF Color )
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
