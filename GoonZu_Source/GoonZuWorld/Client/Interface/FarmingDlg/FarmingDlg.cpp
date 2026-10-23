/*
#include "FarmingDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "../../CommonLogic/Msg/MsgType-Farming.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Common/Farming/FarmingMgr.h"

#include "../../Music/Music.h"
#include "../SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


#include "../../../lib/WebHTML/WebHTML.h"

#include <wininet.h>

#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;


CFarmingDlg::CFarmingDlg()
{
	m_hDlg = NULL;	
	m_bShow = false;
	m_bAutoFarming = false;
	m_pFarmingItemUniqueListPtr = NULL;
	m_hEditBrush = NULL;
	m_siFarmingItemUnique = 0;
	m_siDelayTime = 0;
}

CFarmingDlg::~CFarmingDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}

	if ( m_pFarmingItemUniqueListPtr )
	{
		delete [] m_pFarmingItemUniqueListPtr;
		m_pFarmingItemUniqueListPtr = NULL;
	}
}

void CFarmingDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_FARMING ), hWnd,  StaticFarmingDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CFarmingDlg::StaticFarmingDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pFarmingDlg->FarmingDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CFarmingDlg::FarmingDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			pclClient->pclItemManager->GetFarmItemList( &m_siFarmingItemNum, &m_pFarmingItemUniqueListPtr );
			
			for ( SI16 i = 0; i < m_siFarmingItemNum; ++i )
			{
				char *ItemName = (char*)pclClient->pclItemManager->GetName( m_pFarmingItemUniqueListPtr[i] );
				SendMessage( GetDlgItem( hDlg, IDC_COMBO_FARMING_ITEM  ), CB_ADDSTRING, 0, (LPARAM)ItemName );
			}

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_FARMING_ITEM_IMAGE ), LVSIL_NORMAL );

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);


			

					SetTimer( hDlg, 0, 2000, NULL );
				
			
			
		}
		return TRUE;

	case WM_TIMER:
		{
			if ( IsFarmingStatus() )			
			{	
				SetFarmingExplain();
				Show();
				//pclClient->PushEffect( EFFECT_FISHING_NOW, 0, 0 );
			}
			else
			{
				Hide();
				Reset();
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_FARMING_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				break;
			case IDC_BUTTON_FARMING:
				{
					SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_FARMING_ITEM  ), CB_GETCURSEL, 0, 0 );

					if (SelectedIndex == CB_ERR)
					{
						return FALSE;
					}
					
					cltGameMsgRequest_FarmingStart clinfo( m_pFarmingItemUniqueListPtr[ SelectedIndex] );
					cltMsg clMsg(GAMEMSG_REQUEST_FARMINGSTART, sizeof(clinfo), (char*)&clinfo);
					pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);
				}
				break;
			
			case IDC_LIST_FARMING_ITEM_IMAGE:
				{
					ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_FARMING_ITEM_IMAGE ) );

					SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_FARMING_ITEM  ), CB_GETCURSEL, 0, 0 );

					if (SelectedIndex == CB_ERR)
					{
						return FALSE;
					}

					cltItem clItem;
					clItem.Init();
					clItem.siItemNum = 1;
					clItem.siUnique = m_pFarmingItemUniqueListPtr[ SelectedIndex ];
					
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_FARMING_ITEM_IMAGE, NULL, &clItem, 5, 5 );
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL;
			
		}
		break;
	}

	return FALSE;
}

bool CFarmingDlg::IsShow()
{
	return m_bShow;
}

void CFarmingDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}
	
			
	if ( !( g_SoundOnOffDlg.m_bFullScreenMode )	)
				
	{

		char Buffer[256 ] = "";
		char* pText = GetTxtFromMgr(2445);
		sprintf( Buffer, pText, 
			pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0] ) );
		SetWindowText( m_hDlg, Buffer );
		
		m_bShow = true;
		ShowWindow( m_hDlg, SW_SHOW );	
	}

	return;
}

void CFarmingDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );	

	return;
}

void CFarmingDlg::SetFarmingItem( SI16 ItemUnique, SI32 DelayTime )
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_FARMING_ITEM_IMAGE ) );

	m_siFarmingItemUnique = ItemUnique;
	m_siDelayTime = DelayTime;

	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = ItemUnique;
	
	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_FARMING_ITEM_IMAGE, NULL, &clItem, 5, 5 );
	
	SetFarmingExplain();
	
	return;
}

void CFarmingDlg::SetFarmingExplain()
{
	char Temp[ 256 ] = "";
	char Buffer[256 ] = "";
	
	if ( m_siFarmingItemUnique > 0 )
	{
		char *ProductItemName = (char*)pclClient->pclItemManager->GetName( m_siFarmingItemUnique );

		if ( ProductItemName )
		{
			char* pText = GetTxtFromMgr(2446);
			sprintf( Buffer, pText, ProductItemName, ( m_siDelayTime / 1000 ) / 60, ( m_siDelayTime / 1000 ) % 60 );
		}
	}

	if (  pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique > 0 )
	{
		char *MachineName = (char*)pclClient->pclItemManager->GetName( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique );

		if ( MachineName )
		{
			sprintf( Temp, "[%s](%d/%d)\r\n", MachineName, pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].GetDurability(), pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].GetMaxDurability( pclClient->pclItemManager ) );
			strcat( Buffer, Temp );
		}
	}
	
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FARMING_EXPLAIN), Buffer );

	return;
}

bool CFarmingDlg::IsFarmingStatus()
{
	if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_FARM )
	{		
		return true;
	}

	return false;
}

void CFarmingDlg::Reset()
{
	m_siFarmingItemUnique = 0;
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_FARMING_ITEM_IMAGE ) );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FARMING_EXPLAIN), "" );
	SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_FARMING_ITEM  ), CB_SETCURSEL, (WPARAM)-1, 0 );
}
*/