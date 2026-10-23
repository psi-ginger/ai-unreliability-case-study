/* ==========================================================================
	클래스 :		CNStockDistributionDlg

	작성일 :		05/05/12
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/


#include "./NStockDistributionDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType-Stock.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNStockDistributionDlg::CNStockDistributionDlg()
{
	m_bShow = false;
	m_pStatic_NONAME1	 = NULL; 
	m_pButton_buy	 = NULL; 
	m_pButton_reject	 = NULL; 
	m_pEdit_explain	 = NULL; 
	m_pEdit_num	 = NULL; 
}

CNStockDistributionDlg::~CNStockDistributionDlg()
{
	if( 	m_pStatic_NONAME1	 )  	delete 	m_pStatic_NONAME1	;
	if( 	m_pButton_buy	 )  	delete 	m_pButton_buy	;
	if( 	m_pButton_reject	 )  	delete 	m_pButton_reject	;
	if( 	m_pEdit_explain	 )  	delete 	m_pEdit_explain	;
	if( 	m_pEdit_num	 )  	delete 	m_pEdit_num	;
}

void CNStockDistributionDlg::Initialize()
{
	m_siVillageUnique = 0;
	m_siAvailableStockBuyingNum = 0;
	m_siStockPrice = 0;
	m_bVillageWarStockDistribution = false;
}

void CNStockDistributionDlg::Create( )
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStockDistributionDlg/DLG_NStockDistribution.ddf"));

		file.CreatePopupDialog( this, NSTOCK_DISTRIBUTION_DLG, TEXT("dialog_stock_distribution"), NStockDistributionStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_stock_distribution") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pStatic_NONAME1	 = new 	CStatic	( this )	;
		m_pButton_buy	 = new 	CButton	( this )	;
		m_pButton_reject	 = new 	CButton	( this )	;
		m_pEdit_explain	 = new 	CEdit	( this )	;
		m_pEdit_num	 = new 	CEdit	( this )	;

		file.CreateControl( 	m_pStatic_NONAME1	, 	NSTOCK_DISTRIBUTION_STATICTEXT_NONAME1	, TEXT("statictext_NONAME1") )	;
		file.CreateControl( 	m_pButton_buy	, 	NSTOCK_DISTRIBUTION_BUTTON_BUY	, TEXT("button_buy") )	;
		file.CreateControl( 	m_pButton_reject	, 	NSTOCK_DISTRIBUTION_BUTTON_REJECT	, TEXT("button_reject") )	;
		file.CreateControl( 	m_pEdit_explain	, 	NSTOCK_DISTRIBUTION_EDITBOX_EXPLAIN	, TEXT("editbox_explain") )	;
		file.CreateControl( 	m_pEdit_num	, 	NSTOCK_DISTRIBUTION_EDITBOX_NUM	, TEXT("editbox_num") )	;

		m_pEdit_num->SetNumberMode( true );
        
	}
	else
	{
		DeleteDialog();
	}
}

void  CALLBACK CNStockDistributionDlg::NStockDistributionStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNStockDistributionDlg *pThis = (CNStockDistributionDlg*) pControllerMgr;
	pThis->NStockDistributionProc( nEvent, nControlID, pControl );
}

void  CALLBACK CNStockDistributionDlg::NStockDistributionProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case NSTOCK_DISTRIBUTION_BUTTON_REJECT:
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NSTOCK_DISTRIBUTION_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siVillageUnique > 0 )
					{
						// 주식 구매

						// 수량 확인
						//SI32 BuyingStockNum = GetDlgItemInt( hDlg, IDC_EDIT_STOCK_BUYING_NUM, NULL, FALSE );
						SI32 BuyingStockNum = _tstoi( m_pEdit_num->GetText() );
						if( BuyingStockNum <= 0 ) return;

						if ( BuyingStockNum < 1 || BuyingStockNum > m_siAvailableStockBuyingNum )
						{
							TCHAR* pTitle = GetTxtFromMgr(2462);
							TCHAR* pText = GetTxtFromMgr(2463);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return;
						}

						// 돈 확인
						if ( BuyingStockNum * m_siStockPrice > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
						{
							TCHAR* pTitle = GetTxtFromMgr(1260);
							TCHAR* pText = GetTxtFromMgr(2464);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return;
						}
						

						cltGameMsgRequest_StockDistributionOrder clStockDistributionOrder( m_siVillageUnique, BuyingStockNum, m_bVillageWarStockDistribution );
						cltMsg clMsg( GAMEMSG_REQUEST_STOCKDISTRIBUTIONORDER, sizeof(clStockDistributionOrder), (BYTE*)&clStockDistributionOrder );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						
						DeleteDialog();
					}

				}
				return;
			}
		}
		break;
	}

	return;
}

bool CNStockDistributionDlg::IsShow()
{
	return m_bShow;
}

void CNStockDistributionDlg::Show()
{
	m_bShow = true;

	CDialogController::Show( true );

	return;
}

void CNStockDistributionDlg::Hide()
{
	m_bShow = false;

	CDialogController::Show( false );
	Initialize();

	return;
}

void CNStockDistributionDlg::Set( SI32 VillageUnique, SI32 StockDistributionNum, GMONEY StockPrice, bool VillageWarStockDistribution )
{
	TCHAR *VillageName = pclClient->pclVillageManager->GetName(VillageUnique);

	if ( VillageName == NULL )
	{
		return;
	}

	m_siVillageUnique = VillageUnique;
	m_siAvailableStockBuyingNum = StockDistributionNum;
	m_siStockPrice = StockPrice;
	m_bVillageWarStockDistribution = VillageWarStockDistribution;

	TCHAR Buffer[ 1024 ] = TEXT("");

	TCHAR* pText = GetTxtFromMgr(2465);
	StringCchPrintf( Buffer, 1024, pText, VillageName, m_siStockPrice, m_siAvailableStockBuyingNum );
	m_pEdit_explain->SetText( Buffer );

	if( StockDistributionNum < 0 )
		m_pEdit_num->SetText( TEXT("") );
	else
	{
		TCHAR _buffer[15];
		memset( _buffer, 0, sizeof(_buffer));
        StringCchPrintf(_buffer, 15, TEXT("%d"), StockDistributionNum );
		m_pEdit_num->SetText(_buffer );
	}

	//SetDlgItemInt( m_hDlg, IDC_EDIT_STOCK_BUYING_NUM, StockDistributionNum, FALSE );
	
	Show();

	return;
}

