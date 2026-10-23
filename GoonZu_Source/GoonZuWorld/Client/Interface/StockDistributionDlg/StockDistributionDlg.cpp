#include "StockDistributionDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType-Stock.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CStockDistributionDlg::CStockDistributionDlg()
{
	m_hDlg = NULL;
	m_bShow = false;
}

CStockDistributionDlg::~CStockDistributionDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CStockDistributionDlg::Initialize()
{
	m_siVillageUnique = 0;
	m_siAvailableStockBuyingNum = 0;
	m_siStockPrice = 0;
	m_bVillageWarStockDistribution = false;
}

void CStockDistributionDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_STOCK_DISTRIBUTION ), hWnd,  StaticStockDistributionDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CStockDistributionDlg::StaticStockDistributionDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pStockDistributionDlg->StockDistributionDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CStockDistributionDlg::StockDistributionDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;			
		}
		return TRUE;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
				{
					if ( m_siVillageUnique > 0 )
					{
						// 주식 구매

						// 수량 확인
						SI32 BuyingStockNum = GetDlgItemInt( hDlg, IDC_EDIT_STOCK_BUYING_NUM, NULL, FALSE );

						if ( BuyingStockNum < 1 || BuyingStockNum > m_siAvailableStockBuyingNum )
						{
							char* pTitle = GetTxtFromMgr(2462);
							char* pText = GetTxtFromMgr(2463);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return FALSE;
						}

						// 돈 확인
						if ( BuyingStockNum * m_siStockPrice > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
						{
							char* pTitle = GetTxtFromMgr(1260);
							char* pText = GetTxtFromMgr(2464);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return FALSE;
						}
						

						cltGameMsgRequest_StockDistributionOrder clStockDistributionOrder( m_siVillageUnique, BuyingStockNum, m_bVillageWarStockDistribution );
						cltMsg clMsg( GAMEMSG_REQUEST_STOCKDISTRIBUTIONORDER, sizeof(clStockDistributionOrder), (char*)&clStockDistributionOrder );
						pclClient->SendMsgToServer((char*)&clMsg);
						
						Hide();
					}

				}
				return TRUE;
				
			case IDCANCEL:
				Hide();
				return TRUE;
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

bool CStockDistributionDlg::IsShow()
{
	return m_bShow;
}

void CStockDistributionDlg::Show()
{
	/*
	if ( m_bShow )
	{
		Hide();
		return;
	}
	*/

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CStockDistributionDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	Initialize();

	return;
}

void CStockDistributionDlg::Set( SI32 VillageUnique, SI32 StockDistributionNum, SI32 StockPrice, bool VillageWarStockDistribution )
{
	if ( m_hDlg == NULL )
	{
		return;
	}

	char *VillageName = pclClient->pclVillageManager->GetName(VillageUnique);

	if ( VillageName == NULL )
	{
		return;
	}

	m_siVillageUnique = VillageUnique;
	m_siAvailableStockBuyingNum = StockDistributionNum;
	m_siStockPrice = StockPrice;
	m_bVillageWarStockDistribution = VillageWarStockDistribution;

	char Buffer[ 1024 ] = "";

	char* pText = GetTxtFromMgr(2465);
	sprintf( Buffer, pText, VillageName, m_siStockPrice, m_siAvailableStockBuyingNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_STOCK_DISTRIBUTION_EXPLAIN ), Buffer );

	SetDlgItemInt( m_hDlg, IDC_EDIT_STOCK_BUYING_NUM, StockDistributionNum, FALSE );
	
	Show();

	return;
}