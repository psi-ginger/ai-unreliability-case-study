//---------------------------------
// 2003/3/11 김태곤
//---------------------------------
#include "..\Client.h"

#include "NPCRepair.h"
#include "..\..\Resource.h"
#include "..\Interface\ItemListView\ItemListView.h"
#include "..\CommonLogic\MsgType-Item.h"
#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

cltNPCRepairManager::cltNPCRepairManager()
{
	m_hDlg				= NULL;
	m_pItemListView		= NULL;
	bDlgRectInitSwitch	= false;
	m_hPortraitBitmap	= NULL;

	m_siInvSelectedIndex = -1;
	m_siCharUnique = 0;

	m_pclUserInvItem = new cltItem[ MAX_ITEM_PER_PERSON ];

	m_siItemRepairPrice = 0;
}

cltNPCRepairManager::~cltNPCRepairManager()
{
	if( m_pclUserInvItem )
	{
		delete [] m_pclUserInvItem;
		m_pclUserInvItem = NULL;
	}

	if ( m_pItemListView )
	{
		delete m_pItemListView;
		m_pItemListView = NULL;
	}

	if ( m_hPortraitBitmap != NULL )
	{
		DeleteObject( m_hPortraitBitmap );
		m_hPortraitBitmap = NULL;
	}
	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}


BOOL cltNPCRepairManager::CreateDlg(HINSTANCE hinst, HWND hwnd, SI32 CharUnique )
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_siCharUnique = CharUnique;

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_NPCREPAIR), hwnd, StaticDlgProc);

		if(bDlgRectInitSwitch == true)
		{
			MoveWindow(m_hDlg, 
				DlgRect.left, DlgRect.top, 
				DlgRect.right - DlgRect.left, 
				DlgRect.bottom - DlgRect.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);		

		return TRUE;
	}
	else
	{
		m_siCharUnique = 0;
		DestroyWindow(m_hDlg);
	}


	return 0;
}

BOOL CALLBACK cltNPCRepairManager::StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->pclNPCRepairManager->DlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltNPCRepairManager::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	static HWND hEdit;
	static HWND hCombo;
	char szSelText[128] = "";
	

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{	
			if ( m_pItemListView != NULL )
			{
				delete m_pItemListView;
				m_pItemListView = NULL;
			}

			m_pItemListView = new CItemListView();
			
			m_hDlg = hDlg;
			m_pItemListView->Create( hDlg, IDC_LIST_NPCREPAIRINV );
			
			m_siInvSelectedIndex = -1;
			m_siPreInvSelectedIndex = -1;

			LoadItemData();
			m_pItemListView->SetItemToListView( pclClient->pclItemManager, hDlg, IDC_LIST_NPCREPAIRINV, &m_pclUserInvItem[0], MAX_ITEM_PER_PERSON );


			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "./Interface/Portrait/KIND_JANGYOUNGSIL.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_NPCREPAIR, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
			}

			char* pText = GetTxtFromMgr(1425);

			SetWindowText( GetDlgItem( hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), pText );

			SetTimer( hDlg, 1, NPCREPAIR_SETTIMER_VALUE, NULL );
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

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_NPCREPAIRINV ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							m_siInvSelectedIndex = nlv->lParam;
						}

						if ( m_siInvSelectedIndex < 0 )
						{
							return FALSE;
						}
												
						if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == false )
						{
							char* pText = GetTxtFromMgr(1426);

							SetWindowText( GetDlgItem( hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), pText);
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						// 마우스를 가까이 했을때 나오는 툴팁 부분
						if ( ngt->iItem >= 0 )
						{							
							SI32 siListViewIndex = 0;
							
							for ( int i = 0; i < MAX_ITEM_PER_PERSON; ++i )
							{
								if ( m_pclUserInvItem[ i ].siUnique != 0 )
								{
									if ( ngt->iItem == siListViewIndex )
									{										
										pclClient->pclItemManager->GetExplain( &m_pclUserInvItem[ i ], ngt->pszText );
										break;
									}
									else
									{
										++siListViewIndex;
									}
								}
							}							
						}
					}
					break;
				}
			}
		}

		return TRUE;

	case WM_TIMER:
		{
			if ( m_siInvSelectedIndex >= 0 && m_siPreInvSelectedIndex != m_siInvSelectedIndex )
			{
				m_siPreInvSelectedIndex = m_siInvSelectedIndex;
				
				if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == true )
				{
					if ( m_pclUserInvItem[ m_siInvSelectedIndex ].GetDurability(  ) == m_pclUserInvItem[ m_siInvSelectedIndex ].GetMaxDurability( pclClient->pclItemManager ) )
					{
						char* pText = GetTxtFromMgr(1427);
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), pText );
					}
					else
					{
						cltGameMsgRequest_NPCRepairPrice clNPCRepairPrice( &m_pclUserInvItem[ m_siInvSelectedIndex ]);
						cltMsg clMsg( GAMEMSG_REQUEST_NPCREPAIRPRICE, sizeof(clNPCRepairPrice), (char*)&clNPCRepairPrice );
						pclClient->SendMsgToServer((char*)&clMsg);						
					}
				}
			}

			if ( IsChangedInvItem() )
			{
				LoadItemData();
				m_pItemListView->SetItemToListView( pclClient->pclItemManager, hDlg, IDC_LIST_NPCREPAIRINV, &m_pclUserInvItem[0], MAX_ITEM_PER_PERSON );
			}
		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_NPCREPAIR:
			{
				if ( m_siInvSelectedIndex < 0 )
				{
					return FALSE;
				}

				if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == true )
				{
					if ( m_pclUserInvItem[ m_siInvSelectedIndex ].GetDurability( ) == m_pclUserInvItem[ m_siInvSelectedIndex ].GetMaxDurability( pclClient->pclItemManager ) )
					{
						char* pText = GetTxtFromMgr(1428);
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), pText );
						return FALSE;
					}

					if( m_siCharUnique > 0 )
					{
						if( m_siItemRepairPrice > 0 && pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < m_siItemRepairPrice )
						{
							char* pTitle = GetTxtFromMgr(1429);
							char* pText = GetTxtFromMgr(1430);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						}

						cltGameMsgRequest_NPCRepair clNPCRepair( m_siCharUnique, m_siInvSelectedIndex, &m_pclUserInvItem[ m_siInvSelectedIndex ] );
						cltMsg clMsg( GAMEMSG_REQUSET_NPCREPAIR, sizeof(clNPCRepair), (char*)&clNPCRepair );
						pclClient->SendMsgToServer((char*)&clMsg);
					}				
				}
			}
			break;
		case IDOK:
			DestroyWindow(m_hDlg);
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
		}
		return TRUE;
		
	case WM_DESTROY:
		if ( m_pItemListView != NULL )
		{
			delete m_pItemListView;
			m_pItemListView = NULL;
		}

		if ( m_hPortraitBitmap != NULL )
		{
			DeleteObject( m_hPortraitBitmap );
			m_hPortraitBitmap = NULL;
		}

		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(hDlg, &DlgRect);
		bDlgRectInitSwitch = true;

		m_hDlg = NULL;

		m_siItemRepairPrice = 0;

		return TRUE;

	}

	return FALSE;
}

// 인벤토리 아이템이 바뀌었는지 체크
bool cltNPCRepairManager::IsChangedInvItem()
{
	for ( int j = 0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, PERSONITEM_INV0 부터 함.
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_pclUserInvItem[ j ];
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

// 지속적으로 update 시키지 않기 위해서 처리한것.
void cltNPCRepairManager::LoadItemData()
{
	for ( int i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_pclUserInvItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}

	return;
}

void cltNPCRepairManager::SetItemRepairPriceExplain( SI32 ItemUnique, SI32 ItemPrice )
{
	char Buffer[ 512 ] = "";
	char *ItemName = (char *)pclClient->pclItemManager->GetName( ItemUnique );

	if ( ItemName == NULL )
	{
		return;
	}
	
	char* pText = GetTxtFromMgr(1431);
	char money[128]= " " ;
	sprintf(money,"%d",ItemPrice);
	Nsprintf(Buffer,pText,"item",  ItemName  ,"money", money   ,NULL);
	//sprintf( Buffer, pText, ItemName, ItemPrice );
	
	m_siItemRepairPrice = ItemPrice;

	if ( m_hDlg )
	{
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), Buffer );
	}

	return;
}

void cltNPCRepairManager::SetReairSuccessMent( char *Text )
{
	if ( Text != NULL && strlen ( Text ) > 0 )
	{
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_NPCREPAIR_EXPLAIN ), Text );
	}
	
	return;
}