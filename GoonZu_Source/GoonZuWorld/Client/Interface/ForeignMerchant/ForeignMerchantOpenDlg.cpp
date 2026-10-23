#include "ForeignMerchantOpenDlg.h"

#include "ForeignMerchantBuyCashItemDlg.h"
#include "ForeignMerchantDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\..\CommonLogic\CommonLogic.h"
#include "../../Client.h"
#include "../../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPCBuyCashItem.h"

extern cltCommonLogic* pclClient;

CForeignMerchantOpenDlg::CForeignMerchantOpenDlg()
{
	m_hDlg = NULL;
	m_siNPCCharUnique = 0;

	m_hPortraitBitmap = NULL;
}

CForeignMerchantOpenDlg::~CForeignMerchantOpenDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
		m_hDlg = NULL;
	}
}

void CForeignMerchantOpenDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_FOREIGNMERCHANT_OPEN ), hWnd, StaticForeignMerchantOpenDlgProc );	

	Hide();

	if ( pclClient->siServiceArea != ConstServiceArea_Korea && pclClient->siServiceArea != ConstServiceArea_Netmarble)
		EnableWindow(GetDlgItem(m_hDlg,IDC_BUTTON_CASH_SELL_ITEM),FALSE);
	else EnableWindow(GetDlgItem(m_hDlg,IDC_BUTTON_CASH_SELL_ITEM),TRUE);

	return;
}

BOOL CALLBACK CForeignMerchantOpenDlg::StaticForeignMerchantOpenDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	return pclclient->m_pForeignMerchantOpenDlg->ForeignMerchantOpenDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CForeignMerchantOpenDlg::ForeignMerchantOpenDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			if( m_hPortraitBitmap == NULL )
			{
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_CHINAMERCHANT.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_FOREIGN_MERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}

			SetWindowText( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ), "물품을 팔고 싶다면 물품 판매하기를 누르고,\r\n캐쉬 물품을 판매하고 싶다면 캐쉬아이템 판매하기를\r\n누르게나." );
		}
		return TRUE;

	case WM_COMMAND:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				break;
			case IDC_BUTTON_SELL_ITEM:
				{	
					if ( m_siNPCCharUnique > 0 )
					{
						pclclient->m_pForeignMerchantDlg->Show( m_siNPCCharUnique );
					}
					Hide();
				}
				break;
			case IDC_BUTTON_CASH_SELL_ITEM:
				{
					pclclient->m_pForeignMerchantBuyCashItemDlg->Show();
					Hide();
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

bool CForeignMerchantOpenDlg::IsShow()
{
	return m_bShow;
}

void CForeignMerchantOpenDlg::Show( SI32 npccharunique )
{
	m_siNPCCharUnique = npccharunique;

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	
	return;
}

void CForeignMerchantOpenDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	m_siNPCCharUnique = 0;

	return;
}