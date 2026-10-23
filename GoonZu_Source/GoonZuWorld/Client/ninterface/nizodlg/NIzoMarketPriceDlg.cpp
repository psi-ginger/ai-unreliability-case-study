#include "NIzoMarketPriceDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Izo.h"


extern cltCommonLogic* pclClient;

CNIzoMarketPriceDlg::CNIzoMarketPriceDlg() : CDialogController()
{
	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pEditDownRate = NULL;
	m_pButtonRateUp = NULL;
	m_pButtonRateDown = NULL;
}

CNIzoMarketPriceDlg::~CNIzoMarketPriceDlg()
{
	
	SAFE_DELETE( m_pStatic[0] );
	SAFE_DELETE( m_pStatic[1] );
	SAFE_DELETE( m_pStatic[2] );
	SAFE_DELETE(m_pEditDownRate);
	SAFE_DELETE(m_pButtonRateUp);
	SAFE_DELETE(m_pButtonRateDown);
}

void CNIzoMarketPriceDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NIzoDlg/DLG_Izo_MarketPrice.ddf"));
	file.CreateChildDialog( this, NIZOMARKETPRICE_DLG, TEXT("dialog_izo_marketprice"), StaticCallBackDialogNIzoMarketPriceDlg, pParent);

	NEWCSTATIC( m_pStatic[0] );
	NEWCSTATIC( m_pStatic[1] );
	NEWCSTATIC( m_pStatic[2] );
	NEWCEDIT(m_pEditDownRate);
	NEWCBUTTON(m_pButtonRateUp);
	NEWCBUTTON(m_pButtonRateDown);

	file.CreateControl( m_pStatic[0], NIZOMARKETPRICE_DLG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NIZOMARKETPRICE_DLG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NIZOMARKETPRICE_DLG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pEditDownRate, NIZOMARKETPRICE_DLG_EDIT_DOWNRATE, TEXT("editbox_downrate") );
	file.CreateControl( m_pButtonRateUp, NIZOMARKETPRICE_DLG_BUTTON_RATEUP, TEXT("button_downrateup") );
	file.CreateControl( m_pButtonRateDown, NIZOMARKETPRICE_DLG_BUTTON_RATEDOWN, TEXT("button_downratedown") );

	m_pEditDownRate->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
	m_pEditDownRate->Refresh();

}

void CNIzoMarketPriceDlg::Action()
{
}

void CALLBACK CNIzoMarketPriceDlg::StaticCallBackDialogNIzoMarketPriceDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNIzoMarketPriceDlg *pThis = (CNIzoMarketPriceDlg *)pControllerMgr;
	pThis->CallBackDialogNIzoMarketPriceDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNIzoMarketPriceDlg::CallBackDialogNIzoMarketPriceDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
	case NIZOMARKETPRICE_DLG_BUTTON_RATEUP:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
 			m_siMarketDownRate++;

			if ( m_siMarketDownRate > Const_MarketDownRate_Max )
			{
				m_siMarketDownRate = Const_MarketDownRate_Max;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT(" %d"), m_siMarketDownRate );
			m_pEditDownRate->SetText( buffer );
			//m_pButtonRateUp->Enable(false);
			//m_pButtonRateDown->Enable(false);
		}
		break;
	case NIZOMARKETPRICE_DLG_BUTTON_RATEDOWN:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siMarketDownRate--;

			if ( m_siMarketDownRate < Const_MarketDownRate_Min )
			{
				m_siMarketDownRate = Const_MarketDownRate_Min;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT(" %d"), m_siMarketDownRate );
			m_pEditDownRate->SetText( buffer );
			//m_pButtonRateUp->Enable(false);
			//m_pButtonRateDown->Enable(false);
		}
		break;
	}
}
void CNIzoMarketPriceDlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNIzoMarketPriceDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNIzoMarketPriceDlg::LoadMarketDownRate()
{
	TCHAR buffer[16];

	StringCchPrintf( buffer, 16, TEXT(" %d"), m_siMarketDownRate );
	m_pEditDownRate->SetText(buffer);
}