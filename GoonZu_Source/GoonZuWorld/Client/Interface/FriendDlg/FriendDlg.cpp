#include "FriendDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../ImageInterface/FriendLoginDraw/FriendLoginDraw.h"

#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;

#define FRIENDDLG_TIMER_VALUE			10000

CFriendDlg::CFriendDlg()
{
	m_hDlg = NULL;
	m_hLogInOutImg = NULL;
	m_bShow = false;

	m_hFriendWarpBtnBitmap = NULL;
	
	m_pFriendInfo = new cltFriendInfo();
	m_pFriendInfo->Init();

	Initialize();
}

CFriendDlg::~CFriendDlg()
{
	Destroy();
	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CFriendDlg::Initialize()
{
	for ( int i = 0 ; i < MAX_FRIEND_NUMBER ; i ++ )
		strcpy(m_szAccountID[i],"");
}

void CFriendDlg::Destroy()
{
	if ( m_pFriendInfo )
	{
		delete m_pFriendInfo;
		m_pFriendInfo = NULL;
	}
}

void CFriendDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_FRIEND ), hWnd,  StaticFriendDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CFriendDlg::StaticFriendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	pclclient->m_pFriendDlg->FriendDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CFriendDlg::FriendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();


			if(pclClient->siServiceArea == ConstServiceArea_Netmarble)
			{
				HWND hTemp=GetDlgItem(hDlg,IDC_BUTTON_FRIEND_MINIHOME);
				ShowWindow(hTemp, SW_HIDE);
			}

			m_hDlg = hDlg;
			m_hFriendListCtrl = GetDlgItem( hDlg, IDC_LIST_FRIEND );

			m_hNameInputEdit = GetDlgItem( hDlg, IDC_EDIT_FRIEND_NAME_INPUT );

			SendMessage( m_hNameInputEdit, EM_LIMITTEXT, (WPARAM) MAX_PLAYER_NAME - 1, (LPARAM) 0 );

			m_hLogInOutImg = ImageList_LoadBitmap( pclClient->GetInstance(), MAKEINTRESOURCE( IDB_BITMAP_FRIEND_LOGINOUT ), 16, 1, 0 );
			SendMessage( m_hFriendListCtrl, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)m_hLogInOutImg );
			
			m_hFriendWarpBtnBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/FriendDlg/FriendWarpBtn.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hFriendWarpBtnBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_FRIEND_WARP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hFriendWarpBtnBitmap );
			}

			LVCOLUMN Col;			

			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			Col.cx = 200;

			char* pText = GetTxtFromMgr(1556);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( m_hFriendListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}

			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 2, 213, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		return TRUE;
	case WM_TIMER:
		{
			if ( IsChanged() )
			{
				LoadFriendList();
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
			
			if ( hdr->hwndFrom == m_hFriendListCtrl )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
							{
								char buffer[256 ] = "";
								ListView_GetItemText( m_hFriendListCtrl, nlv->iItem, 0, buffer, 128 );
								SetWindowText( m_hNameInputEdit, buffer );

								HWND hWnd = NULL ;

								char ProfileURL[MAX_PATH];

								// GGOMG
/*								pclClient->GetCZProfileUrl( ProfileURL, m_szAccountID[nlv->iItem] );

								hWnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
								if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

								// INTRO
								pclClient->GetCZIntroUrl( ProfileURL, m_szAccountID[nlv->iItem] );

								hWnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
								if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);
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
			case IDOK:
				Hide();
				return TRUE;
				
			case IDCANCEL:
				Hide();
				return TRUE;
				/*
				switch( HIWORD ( wParam ) )
				{
				}
				*/
			case IDC_BUTTON_FRIEND_ADD:
				{
					char buffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hNameInputEdit, buffer, MAX_PLAYER_NAME - 1 );
					buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					if ( strlen( buffer ) < 1 )
					{
						return FALSE;
					}					
					
					if ( strcmp( buffer, pclClient->pclCM->CR[1]->GetName() ) == 0 )
					{
						char* pTitle = GetTxtFromMgr(2458);
						char* pText = GetTxtFromMgr(2459);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return false;
					}

					cltGameMsgRequest_FriendAdd clFriendAdd( buffer );
					cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (char*)&clFriendAdd );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_FRIEND_DEL:
				{
					char buffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hNameInputEdit, buffer, MAX_PLAYER_NAME - 1 );
					buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

					if ( strlen( buffer ) < 1 )
					{
						return FALSE;
					}
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && strcmp( pSimplePersonInfo->GetName(), buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltGameMsgRequest_FriendDel clFriendDel( PersonID );
						cltMsg clMsg( GAMEMSG_REQUEST_FRIENDDEL, sizeof(clFriendDel), (char*)&clFriendDel );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_FRIEND_WHISPER_CHAT:
				{
					char buffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hNameInputEdit, buffer, MAX_PLAYER_NAME - 1 );
					buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && strcmp( pSimplePersonInfo->GetName(), buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(PersonID);

				}
				break;
			case IDC_BUTTON_FRIEND_MINIHOME:
				{
					char buffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hNameInputEdit, buffer, MAX_PLAYER_NAME - 1 );
					buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && strcmp( pSimplePersonInfo->GetName(), buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltGameMsgRequest_Userid clUserid( buffer );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
				
			case IDC_BUTTON_FRIEND_WARP:
				{
					char buffer[ MAX_PLAYER_NAME ] = "";
					GetWindowText( m_hNameInputEdit, buffer, MAX_PLAYER_NAME - 1 );
					buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && strcmp( pSimplePersonInfo->GetName(), buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
		
						// 감옥에서는 워프 불가 
						if( pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON))	
						{
							char* pTitle = GetTxtFromMgr(1413);
							char* pText = GetTxtFromMgr(2259);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return TRUE ;
						}
						
						// 탑승중이면 워프 불가. 
						if(pclchar->clInnerStatus.siRideShipID)
						{
							char* pTitle = GetTxtFromMgr(1413);
							char* pText = GetTxtFromMgr(1416);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return TRUE;
						}
						
						// 영자나라에 있으면 불가능
						if ( pclchar->GetMapIndex() == MAPINDEX_MASTERWORLD &&
							pclchar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
						{
							return TRUE ;
						}

						char* pTitle = NULL ;
						char* pText = NULL ;

						char temp[256];
						if ( !pclchar->pclCI->clBI.bUseNoLimitTicketWarp &&
							 pclchar->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
						{
							// 순간이동이용권을 보유하고 있는지 확인한다. 
							SI32 itemunique = 14005 ;  // 순간이동이용권
							SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
							if(pos < 0)
							{
								pTitle = GetTxtFromMgr(1413);
								pText = GetTxtFromMgr(2460);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

								return TRUE;
							}

							pText = GetTxtFromMgr(2461);
							sprintf(temp, pText, buffer);
							pText = GetTxtFromMgr(1418);
						}
						else
						{
							pText = GetTxtFromMgr(3191);
							sprintf(temp, pText, buffer);
							pText = GetTxtFromMgr(3189);
						}

						if (  MessageBox(NULL, temp , pText, MB_ICONQUESTION | MB_YESNO) == IDNO )
						{
							return TRUE;
						}
						cltGameMsgRequest_FriendWarp clFriendWarp( PersonID );
						cltMsg clMsg( GAMEMSG_REQUEST_FRIENDWARP, sizeof( clFriendWarp ), (char*)&clFriendWarp );
						pclClient->SendMsgToServer((char*)&clMsg);						
					}
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
			if ( m_hFriendWarpBtnBitmap )
			{
				DeleteObject( m_hFriendWarpBtnBitmap );
				m_hFriendWarpBtnBitmap = NULL;
			}

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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_INTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hLogInOutImg )
			{
				ImageList_Destroy( m_hLogInOutImg );
				m_hLogInOutImg = NULL;
			}
			
			KillTimer( m_hDlg, 0 );
			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CFriendDlg::IsShow()
{
	return m_bShow;
}

void CFriendDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	/*
	아래 코드 필요 없음. ( pclClient->친구 리스트 ) 거기꺼 가죠 오면 됨.
	cltGameMsgRequest_Friend_List clFriendList();
	cltMsg clMsg( GAMEMSG_REQUEST_FRIEND_LIST, sizeof(clFriendList), (char*)&clFriendList );
	pclClient->SendMsgToServer((char*)&clMsg);
	*/

	// 친구 정보는 PersonID 랑 캐릭터명 만 있으면 됨.

	LoadFriendList();
	SetTimer( m_hDlg, FRIENDDLG_TIMER_VALUE, 0, NULL );

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CFriendDlg::Hide()
{
	KillTimer( m_hDlg, 0 );
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

bool CFriendDlg::IsChanged()
{
	char *src = (char*)m_pFriendInfo;
	char *dest = (char*)&pclClient->pclCM->CR[1]->pclCI->clFriendInfo;

	for ( SI32 i = 0; i < sizeof( cltFriendInfo ); ++i )
	{
		if(src[i] != dest[i])
		{
			return true;
		}
	}
	
	return false;
}

void CFriendDlg::LoadFriendList()
{
	ListView_DeleteAllItems( m_hFriendListCtrl );

	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_IMAGE;
	Li.state = 0;
	Li.stateMask = 0;
	Li.iImage = 0;
	Li.iSubItem = 0;
	
	SI16 siCount = 0;
	
	memcpy( m_pFriendInfo, &pclClient->pclCM->CR[1]->pclCI->clFriendInfo, sizeof( cltFriendInfo ) );
	
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[i];
		
		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			strcpy(m_szAccountID[i],pclClient->pclCM->CR[1]->pclCI->clFriendInfo.szAccountID[i]);
			if ( pclClient->pclCM->CR[1]->pclCI->clFriendInfo.bConnectStatus[ i ] )
			{
				Li.iImage = 1;
			}
			else
			{
				Li.iImage = 0;
			}

			char *Name = (char*)pSimplePersonInfo->GetName();
			if ( Name == NULL || strlen( Name ) < 1 )
			{
				continue;
			}
			
			Li.iItem = siCount++;
			Li.pszText = (char*)pSimplePersonInfo->GetName();
			SendMessage( m_hFriendListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );
		}
	}
	return;
}

// 친구가 추가 된 경우
void CFriendDlg::FriendAddList( SI16 ListIndex, SI32 PersonID, SI32 CharID, char *FriendName ,char *AccountID)
{
	if ( PersonID < 1 || FriendName == NULL || strlen( FriendName ) < 1 )
	{
		return;
	}

	if ( CharID > 0 )
	{
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.Set( ListIndex, PersonID, CharID, FriendName, true ,AccountID);
	}
	else
	{
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.Set( ListIndex, PersonID, CharID, FriendName, false ,AccountID);
	}
	
	LoadFriendList();
	
	/*		
	cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ListIndex];
	if ( pSimplePersonInfo )
	{
		pSimplePersonInfo->Set( PersonID, FriendName );
		
		LoadFriendList();
	}
	*/
	

	return;
}

void CFriendDlg::FriendDelList( SI16 ListIndex, SI32 PersonID )
{
	if ( pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ListIndex].GetPersonID() == PersonID )
	{
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.Init( ListIndex );
		LoadFriendList();
	}
	
	/*
	cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ListIndex];
	if ( pSimplePersonInfo )
	{
		pSimplePersonInfo->Init();
		
		LoadFriendList();
	}
	*/

	return;
}

void CFriendDlg::FriendLogin( SI32 PersonID, SI32 CharID ,char * AccountID)
{
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ i ].GetPersonID() == PersonID )
		{
			pclClient->pclCM->CR[1]->pclCI->clFriendInfo.Set( i, CharID, true ,AccountID);
			
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->m_pFriendLoginDraw->Set( (char*)pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ i ].GetName() );
			
			LoadFriendList();
			break;
		}
	}

	/*
	cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ ListIndex ];
	
	if ( pSimplePersonInfo && pSimplePersonInfo->GetPersonID() == PersonID )
	{
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.Set( ListIndex, CharID, true );
		LoadFriendList();
	}
	*/

	return;
}

void CFriendDlg::FriendLogOut( SI32 PersonID )
{
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ i ].GetPersonID() == PersonID )
		{
			pclClient->pclCM->CR[1]->pclCI->clFriendInfo.bConnectStatus[i] = false;
			pclClient->pclCM->CR[1]->pclCI->clFriendInfo.siCharID[i] = 0;
			LoadFriendList();
			break;
		}
	}

	/*
	if ( pclClient->pclCM->CR[1]->pclCI->clFriendInfo.clFriend[ListIndex].GetPersonID() == PersonID )
	{
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.bConnectStatus[ListIndex] = false;
		pclClient->pclCM->CR[1]->pclCI->clFriendInfo.siCharID[ListIndex] = 0;
		LoadFriendList();
	}
	*/


	return;
}