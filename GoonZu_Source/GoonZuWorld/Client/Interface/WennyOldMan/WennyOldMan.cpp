// WennyOldMan.cpp: implementation of the CWennyOldMan class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../ItemListView/ItemListView.h"
#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../CommonLogic/MsgType-Item.h"
#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../../Common/HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"
#include "NInterface/NHintDlg/NHintDlg.h"

#include "../../Music/music.h"

#include "WennyOldMan.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWennyOldMan::CWennyOldMan()
{
	m_hDlg				= NULL;
	m_hUpgradeItemDlg	= NULL;
	m_hPerformanceUpgradeItemDlg = NULL;
	m_hCurrentDlg		= NULL;
	
	m_hBitmap			= NULL;
	m_hHelpBtnBitmap	= NULL;
	m_hTabCtrl			= NULL;

	m_bRectSwitch	= FALSE;
	m_bShow = FALSE ;

	m_siSelectIndex = -1;
	m_siVillage = -1 ;

	m_pInvItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	memset( m_pInvItem, 0, sizeof( cltItem ) * MAX_ITEM_PER_PERSON );
}

CWennyOldMan::~CWennyOldMan()
{
	if( m_pInvItem )
	{
		delete [] m_pInvItem;
		m_pInvItem = NULL;
	}

	if ( m_hBitmap != NULL )
	{
		DeleteObject( m_hBitmap );
		m_hBitmap = NULL;
	}

	if ( m_hDlg )
	{
		DestroyWindow(m_hDlg);
	}
}

void CWennyOldMan::Create(HINSTANCE hInst, HWND hParent )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_WENNYOLDMAN ), hParent, StaticWennyOldManProc );
	
	ShowWindow( m_hDlg, SW_HIDE );
}

BOOL CALLBACK CWennyOldMan::StaticWennyOldManUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pWennyOldMan->WennyOldManUpgradeItemProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CWennyOldMan::WennyOldManUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();			
			
			m_hUpgradeItemDlg = hDlg;

			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_ITEM ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_INFOTIP );
		}
		return TRUE;

	case WM_TIMER:
		{
			if ( IsChangedInvItem() )
			{
				LoadUpgradeItemData();
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

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_ITEM ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							m_siSelectIndex = nlv->lParam;
						}
					}					
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 Inx = 0;
							
							for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
							{
								if ( m_pInvItem[ i ].siUnique > 0 &&
									pclClient->pclItemManager->CanTune( &m_pInvItem[ i ] )
									)
								{
									if ( ngt->iItem == Inx )
									{
										pclClient->pclItemManager->GetExplain( &m_pInvItem[ i ], ngt->pszText );
										break;
									}
									else
									{
										++Inx;
									}
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
			switch(LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_UPGRADEITEM:
				{
					if ( m_siSelectIndex < 0 )
					{
						char* pTitle = GetTxtFromMgr(1898);
						char* pText = GetTxtFromMgr(1899);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}
					
					if ( ! pclClient->pclItemManager->CanTune(&m_pInvItem[ m_siSelectIndex ] ) )
					{
						char* pTitle = GetTxtFromMgr(1900);
						char* pText = GetTxtFromMgr(1901);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}
					
					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(14010) <= 0 )
					{
						char* pTitle = GetTxtFromMgr(1902);
						char* pText = GetTxtFromMgr(1903);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}

					char* pTitle = GetTxtFromMgr(1904);
					char* pText = GetTxtFromMgr(1905);
					
				//	char buffer[256] = {"등급향상"};
					if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
					{
						cltGameMsgRequest_UpgradeItem clinfo( m_siSelectIndex, m_siVillage );
						cltMsg clMsg(GAMEMSG_REQUEST_UPGRADEITEM, sizeof(clinfo), (char*)&clinfo);
						pclClient->SendMsgToServer((char*)&clMsg);
					}

				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );
			m_hUpgradeItemDlg = NULL;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CWennyOldMan::StaticWennyOldManPerformanceUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pWennyOldMan->WennyOldManPerformanceUpgradeItemProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CWennyOldMan::WennyOldManPerformanceUpgradeItemProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	char Nbuffer[512]=" ";
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			cltClient *pclclient = (cltClient *)pclClient;
			m_hPerformanceUpgradeItemDlg = hDlg;
			
			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );
			
		}
		return TRUE;

	case WM_TIMER:
		{
			if ( IsChangedInvItem() )
			{
				LoadPerformanceUpgradeItemData();
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

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							cltItem clDestItem;
							clDestItem.Init();
							
							SI32 ScrollUnique;
							SI32 ScrollNum;
							char *ScrollName = NULL;
							
							m_siSelectIndex = nlv->lParam;


							// 개조 가능한 아이템 인 경우.
							SI32 returnval = 0;
							if ( pclClient->pclItemManager->GetScrollForTune2(
								m_pInvItem[ m_siSelectIndex ],
								&ScrollUnique,
								&ScrollNum,
								&clDestItem,
								&returnval) )
							{								
								char Temp[ 512 ] = "";
								if ( pclClient->pclItemManager->GetExplain( &clDestItem, Temp ) )
								{
									char Buffer[ 1024 ] = "";
									char* pText = GetTxtFromMgr(1906);
									sprintf( Buffer, pText, Temp );
									SetWindowText( m_hEdit, Buffer );
								}
							}
						}
					}					
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 Inx = 0;
							
							cltItem clDestItem;
							clDestItem.Init();
					
							SI32 ScrollUnique;
							SI32 ScrollNum;
																
							for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
							{
								clDestItem.Init();

								SI32 returnval = 0;
								if ( m_pInvItem[ i ].siUnique > 0 &&
									( pclClient->pclItemManager->GetScrollForTune2( m_pInvItem[ i ], &ScrollUnique, &ScrollNum, &clDestItem, &returnval )  )
									)
								{
									if ( ngt->iItem == Inx )
									{
										pclClient->pclItemManager->GetExplain( &m_pInvItem[ i ], ngt->pszText );
										break;
									}
									else
									{
										++Inx;
									}
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
			switch(LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_PERFORMANCEUPGRADEITEM:
				{
					if ( m_siSelectIndex < 0 )
					{
						char* pTitle = GetTxtFromMgr(1898);
						char* pText  = GetTxtFromMgr(1899);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}
					
					if ( m_pInvItem[ m_siSelectIndex ].siUnique < 1 )
					{
						return FALSE;
					}
					
					if ( m_pInvItem[ m_siSelectIndex ].CanPile( pclClient->pclItemManager ) )
					{
						char* pTitle = GetTxtFromMgr(1907);
						char* pText  = GetTxtFromMgr(1908);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}

					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(14010) < 1 )
					{
						char* pTitle = GetTxtFromMgr(1907);
						char* pText  = GetTxtFromMgr(1909);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						break;
					}
					
					cltItem clDestItem;
					clDestItem.Init();
					
					SI32 ScrollUnique;
					SI32 ScrollNum;
					char *ScrollName = NULL;

					SI32 returnval = 0;
					if ( ! pclClient->pclItemManager->GetScrollForTune2(
						m_pInvItem[ m_siSelectIndex ],
						&ScrollUnique,
						&ScrollNum,
						&clDestItem,
						&returnval) )
					{
						switch(returnval)
						{
						case SCROLLTUNE_ERR_MINDAMAGE:
							{
								char* pTitle = GetTxtFromMgr(1907);
								char* pText  = GetTxtFromMgr(1910);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
							}
							break;
						case SCROLLTUNE_ERR_MAXDAMAGE:
							{
								char* pTitle = GetTxtFromMgr(1907);
								char* pText  = GetTxtFromMgr(1911);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
							}
							break;
						case SCROLLTUNE_ERR_AC:
							{
								char* pTitle = GetTxtFromMgr(1907);
								char* pText  = GetTxtFromMgr(1912);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
							}
							break;
						default:
							{
								char* pTitle = GetTxtFromMgr(1907);
								char* pText  = GetTxtFromMgr(1908);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							}
						}
						
						return FALSE;
					}

					ScrollName = (char*)pclClient->pclItemManager->GetName( ScrollUnique );
					if ( ScrollName == NULL || strlen ( ScrollName ) < 1 )
					{
						return FALSE;
					}
					
					SI16 ScrollItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( ScrollUnique );
					
					
					if ( ScrollItemPos < 0 )
					{
						ScrollName = (char*)pclClient->pclItemManager->GetName( ScrollUnique );
						
						if ( ScrollName != NULL )
						{
							char* pTitle = GetTxtFromMgr(1907);
							char* pText  = GetTxtFromMgr(1913);
							char num[128]= " " ;
							sprintf(num,"%d",ScrollNum);
							Nsprintf(Nbuffer,pText,"item",  ScrollName  ,"num", num   ,NULL);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);

							//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, ScrollName, ScrollNum );
						}
						
						return FALSE;
					}
					
					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ScrollItemPos ].GetItemNum() < ScrollNum )
					{
						ScrollName = (char*)pclClient->pclItemManager->GetName( ScrollUnique );
						
						if ( ScrollName != NULL )
						{
							char* pTitle = GetTxtFromMgr(1907);
							char* pText  = GetTxtFromMgr(1913);
							char num[128]= " " ;
							sprintf(num,"%d",ScrollNum);
							Nsprintf(Nbuffer,pText,"item",  ScrollName  ,"num", num   ,NULL);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);
							//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, ScrollName, ScrollNum );
						}
						
						return FALSE;
					}
					
					char buffer[256] = "";
					char* pText = GetTxtFromMgr(1914);
					char* pTitle = GetTxtFromMgr(1915);
					sprintf( buffer, pText, ScrollName, ScrollNum );

					if (  MessageBox(NULL, buffer, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
					{
						cltGameMsgRequest_PerformanceUpgradeItem clinfo( m_siVillage, m_siSelectIndex, &m_pInvItem[ m_siSelectIndex ] );
						cltMsg clMsg(GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM, sizeof(clinfo), (char*)&clinfo);
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );
			m_hPerformanceUpgradeItemDlg = NULL;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CWennyOldMan::StaticWennyOldManProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pWennyOldMan->WennyOldManProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CWennyOldMan::WennyOldManProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			m_hTabCtrl = GetDlgItem( hDlg, IDC_TAB_WENNYOLDMAN );

			m_hEdit = GetDlgItem( hDlg, IDC_EDIT_WENNYOLDMAN );

			m_hHelpBtnBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "statushint/hokhint.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hHelpBtnBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_HELP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHelpBtnBitmap );
			}

			m_hBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_WENNYOLDMAN.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_WENNYOLDMAN, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hBitmap);
			}

			TC_ITEM item;
			item.mask= TCIF_TEXT;
			item.iImage = -1;
			
			item.lParam = 0;
			char* pText = GetTxtFromMgr(1904);
			item.pszText = pText;
			
			TabCtrl_InsertItem( m_hTabCtrl, 0, &item);

			item.lParam = 1;
			pText = GetTxtFromMgr(1916);
			item.pszText = pText;
			
			TabCtrl_InsertItem( m_hTabCtrl, 1, &item);

			RECT rect;
			GetClientRect( m_hTabCtrl, &rect);
			TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);

			m_hPerformanceUpgradeItemDlg = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE( IDD_DIALOG_WENNYOLDMAN_PERFORMANCEUPGRADEITEM ), m_hTabCtrl, StaticWennyOldManPerformanceUpgradeItemProc );
			m_hUpgradeItemDlg = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE( IDD_DIALOG_WENNYOLDMAN_UPGRADEITEM ), m_hTabCtrl, StaticWennyOldManUpgradeItemProc );

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( m_hPerformanceUpgradeItemDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ), LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( GetDlgItem( m_hUpgradeItemDlg, IDC_LIST_ITEM ), LVSIL_NORMAL );
			
			
			MoveWindow( m_hUpgradeItemDlg, 
				rect.left, rect.top, rect.right-rect. left,
				rect.bottom- rect.top, TRUE);
			
			MoveWindow( m_hPerformanceUpgradeItemDlg, 
				rect.left, rect.top, rect.right-rect. left,
				rect.bottom- rect.top, TRUE);
						
			m_hCurrentDlg = m_hUpgradeItemDlg;
			
			ShowWindow( m_hCurrentDlg, SW_SHOW );
		}
		break;

	case WM_TIMER:
		{
			switch(wParam)
			{
			case 0:
				{
					SI32 village = pclclient->pclCM->CR[1]->GetMapIndex() ;
					if ( m_siVillage != village )
					{
						Hide();
						break;
					}					
				}
				break;
			}
		}
		break;
	
	case WM_NOTIFY:
		{
			LPNMHDR						lpNmhdr;
			lpNmhdr = ( LPNMHDR )		lParam;

			if ( lpNmhdr->code == TCN_SELCHANGE )
			{
				int index = TabCtrl_GetCurSel( m_hTabCtrl );
				
				char TabText[ 128 ] = "";
				
				TCITEM tc;
				tc.mask = TCIF_TEXT;
				tc.pszText = TabText;
				tc.cchTextMax = sizeof ( TabText ) - 1;
				
				KillTimer( m_hCurrentDlg, 0 );
				ShowWindow( m_hCurrentDlg, SW_HIDE );
				
				char buffer[1024];
				char* pText = GetTxtFromMgr(1917);
				sprintf(buffer, pText, MAX_DUR_FOR_TUNE);
				
				SetWindowText( m_hEdit, buffer );
				
				if ( TabCtrl_GetItem( m_hTabCtrl, index, &tc ) == TRUE )
				{
					char* pText1 = GetTxtFromMgr(1904);
					char* pText2 = GetTxtFromMgr(1916);
					if ( strcmp( tc.pszText, pText1 ) == 0 )
					{
						m_hCurrentDlg = m_hUpgradeItemDlg;
						LoadUpgradeItemData();
						SetTimer( m_hCurrentDlg, 0, 1000, NULL );
						ShowWindow( m_hCurrentDlg, SW_SHOW );
					}
					else if ( strcmp( tc.pszText, pText2 ) == 0 )
					{
						m_hCurrentDlg = m_hPerformanceUpgradeItemDlg;
						LoadPerformanceUpgradeItemData();
						SetTimer( m_hCurrentDlg, 0, 1000, NULL );
						ShowWindow( m_hCurrentDlg, SW_SHOW );
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			{
				Hide();
			}
			break;
		case IDC_BUTTON_HELP:
			{
				cltClient *pclclient = (cltClient *)pclClient;
				if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
				{
					pclclient->CreateInterface( NHINT_DLG );
				}
				((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show("hokburi");
				//pclclient->m_pHintDlg->Show("hokburi");
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hHelpBtnBitmap )
			{
				DeleteObject( m_hHelpBtnBitmap );
				m_hHelpBtnBitmap = NULL;
			}
			if ( m_hBitmap )
			{
				DeleteObject(m_hBitmap);
				m_hBitmap = NULL ;
			}

			KillTimer( hDlg, 0 );

			m_hEdit = NULL ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CWennyOldMan::Set()
{
	if ( !m_bShow )
		Show();
	else
		Hide();
}

void CWennyOldMan::Show()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( m_bRectSwitch )
	{
		MoveWindow(m_hDlg, 
			m_rtDlg.left, m_rtDlg.top, 
			m_rtDlg.right - m_rtDlg.left, 
			m_rtDlg.bottom - m_rtDlg.top, FALSE);
	}

	m_siSelectIndex = -1;
	m_bShow = TRUE ;

	m_siVillage = pclclient->pclCM->CR[1]->GetMapIndex() ;	
	
	char buffer[1024];
	char* pText = GetTxtFromMgr(1917);

	sprintf(buffer, pText, MAX_DUR_FOR_TUNE);

	SetWindowText( m_hEdit, buffer );

	SetTimer( m_hDlg, 0, 1000, NULL );
	SetTimer( m_hCurrentDlg, 0, 1000, NULL );

	LoadUpgradeItemData();
	LoadPerformanceUpgradeItemData();
	
	ShowWindow( m_hDlg, SW_SHOW );
}

void CWennyOldMan::Hide()
{
	GetWindowRect(m_hDlg, &m_rtDlg);

	m_bRectSwitch = TRUE ;
	m_bShow = FALSE ;

	KillTimer( m_hDlg, 0 );
	ShowWindow( m_hDlg, SW_HIDE );
}

BOOL CWennyOldMan::IsChangedInvItem()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )
	{
		SI32 size = sizeof(cltItem);
		
		char* src = (char*)&m_pInvItem[ j ];
		char* dst = (char*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];
		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void CWennyOldMan::LoadUpgradeItemData()
{
	m_siSelectIndex = -1;
	char Nbuffer[512]=" ";

	ListView_DeleteAllItems( GetDlgItem( m_hUpgradeItemDlg, IDC_LIST_ITEM ) );

	
	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	Li.state = 0;
	Li.stateMask = 0;	
	
	SI16 Counter = 0;
	
	char ItemName[128];
	char buffer[ 256 ] = "";

	for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_pInvItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];

		if ( m_pInvItem[i].siUnique > 0 && pclClient->pclItemManager->CanTune(&m_pInvItem[i] ) )
		{
			if( m_pInvItem[ i ].GetFullName( pclClient->pclItemManager, ItemName ) != NULL )
			{
				Li.iImage	= m_pInvItem[i].GetImage( pclClient->pclItemManager );
				Li.iSubItem	= 0;
				Li.iItem	= Counter;
				
				char* pText = GetTxtFromMgr(1918);
				char num[128]= " " ;
				sprintf(num,"%d",m_pInvItem[i].siItemNum);
				Nsprintf(Nbuffer,pText,"item", ItemName   ,"num", num   ,NULL);

				//sprintf(buffer, pText, ItemName, m_pInvItem[i].siItemNum );
				
				Li.pszText	= buffer;
				Li.lParam	= i;
				++Counter;
				
				ListView_InsertItem( GetDlgItem( m_hUpgradeItemDlg, IDC_LIST_ITEM ), &Li );
			}
		}
	}

	ListView_SetIconSpacing( GetDlgItem( m_hUpgradeItemDlg, IDC_LIST_ITEM ), LISTVIEW_ITEM_WIDTH, LISTVIEW_ITEM_HEIGHT );

	return;
}

void CWennyOldMan::LoadPerformanceUpgradeItemData()
{
	m_siSelectIndex = -1;
	char Nbuffer[512]=" ";


	ListView_DeleteAllItems( GetDlgItem( m_hPerformanceUpgradeItemDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ) );

	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	Li.state = 0;
	Li.stateMask = 0;	
	
	SI16 Counter = 0;
	

	char ItemName[128];
	char buffer[ 256 ] = "";

	cltItem clDestItem;
	clDestItem.Init();
	
	SI32 ScrollUnique;
	SI32 ScrollNum;

	for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_pInvItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
		clDestItem.Init();

		SI32 returnval = 0;
		if ( m_pInvItem[i].siUnique > 0 && 
			( pclClient->pclItemManager->GetScrollForTune2( m_pInvItem[i], &ScrollUnique, &ScrollNum, &clDestItem, &returnval ) )
			)
		{
			if( m_pInvItem[ i ].GetFullName( pclClient->pclItemManager, ItemName ) != NULL )
			{
				Li.iImage	= m_pInvItem[i].GetImage( pclClient->pclItemManager );
				Li.iSubItem	= 0;
				Li.iItem	= Counter;

				char* pText = GetTxtFromMgr(1918);
				char num[128]= " " ;
				sprintf(num,"%d",m_pInvItem[i].siItemNum);
				Nsprintf(Nbuffer,pText,"item", ItemName   ,"num", num   ,NULL);

				//sprintf(buffer, pText, ItemName, m_pInvItem[i].siItemNum );

				Li.pszText	= buffer;
				Li.lParam	= i;
				++Counter;
				
				m_pInvItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];

				ListView_InsertItem( GetDlgItem( m_hPerformanceUpgradeItemDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ), &Li );
			}
		}
	}

	ListView_SetIconSpacing( GetDlgItem( m_hPerformanceUpgradeItemDlg, IDC_LIST_PERFORMANCEUPGRADEITEM ), LISTVIEW_ITEM_WIDTH, LISTVIEW_ITEM_HEIGHT );

	return;
}

void CWennyOldMan::UpgradeItemSuccess( bool Success )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( Success )
	{
		pclclient->pclMusic->PushEffect( EFFECT_WENNYOLDMAN_SUCCESS, 0, 0 );
		char* pText = GetTxtFromMgr(1919);
		SetWindowText( m_hEdit, pText );
	}
	else
	{
		char* pText = GetTxtFromMgr(1920);
		SetWindowText( m_hEdit,pText );
	}
}

void CWennyOldMan::PerformanceUpgradeItemSuccess()
{
	char* pText = GetTxtFromMgr(1921);
	SetWindowText( m_hEdit, pText );
}