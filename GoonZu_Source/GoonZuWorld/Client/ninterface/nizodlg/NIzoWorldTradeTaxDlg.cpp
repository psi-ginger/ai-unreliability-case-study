#include "NIzoWorldTradeTaxDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Izo.h"


extern cltCommonLogic* pclClient;

CNIzoWorldTradeTaxDlg::CNIzoWorldTradeTaxDlg() : CDialogController()
{
	for ( SI32 i=0; i<7; i++)
	{
		m_pStatic[i] = NULL;
	}
	for ( SI32 i=0; i<3; i++)
	{
		m_pOutLine[i] = NULL;
	}
	m_pCheckboxIsSetTax = NULL;
	m_pButtonTaxRangeUp = NULL;
	m_pButtonTaxRangeDown = NULL;
	m_pButtonTaxRateUp = NULL;
	m_pButtonTaxRateDown = NULL;
	m_pEditTaxRange = NULL;
	m_pEditTaxRate = NULL;

	m_bWorldTradeTax = false;
	m_siSpecialTaxRange = 0;
	m_siSpecialTaxRate = 0;
}

CNIzoWorldTradeTaxDlg::~CNIzoWorldTradeTaxDlg()
{
	for( SI32 i=0; i<7; i++){
		SAFE_DELETE( m_pStatic[i] )
	}
	for( SI32 i=0; i<3; i++){
		SAFE_DELETE( m_pOutLine[i] )
	}
	SAFE_DELETE(m_pCheckboxIsSetTax);
	SAFE_DELETE(m_pButtonTaxRangeUp);
	SAFE_DELETE(m_pButtonTaxRangeDown);
	SAFE_DELETE(m_pButtonTaxRateUp);
	SAFE_DELETE(m_pButtonTaxRateDown);
	SAFE_DELETE(m_pEditTaxRange);
	SAFE_DELETE(m_pEditTaxRate);
}

void CNIzoWorldTradeTaxDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NIzoDlg/DLG_Izo_WorldTradeTax.ddf"));
	file.CreateChildDialog( this, NIZOWORLDTRADETAX_DLG, TEXT("dialog_izo_worldtradetax"), StaticCallBackDialogNIzoWorldTradeTaxDlg, pParent);

	for( SI32 i=0; i<7; i++){
		NEWCSTATIC( m_pStatic[i] )
	}
	for( SI32 i=0; i<3; i++){
		NEWCOUTLINE( m_pOutLine[i] )
	}
	NEWCCHECKBOX(m_pCheckboxIsSetTax);
	NEWCBUTTON(m_pButtonTaxRangeUp);
	NEWCBUTTON(m_pButtonTaxRangeDown);
	NEWCBUTTON(m_pButtonTaxRateUp);
	NEWCBUTTON(m_pButtonTaxRateDown);
	NEWCEDIT(m_pEditTaxRange);
	NEWCEDIT(m_pEditTaxRate);

	file.CreateControl( m_pStatic[0], NIZOWORLDTRADETAX_DLG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NIZOWORLDTRADETAX_DLG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NIZOWORLDTRADETAX_DLG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NIZOWORLDTRADETAX_DLG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NIZOWORLDTRADETAX_DLG_STATIC5, TEXT("statictext_NONAME5") );
	file.CreateControl( m_pStatic[5], NIZOWORLDTRADETAX_DLG_STATIC6, TEXT("statictext_NONAME6") );
	file.CreateControl( m_pStatic[6], NIZOWORLDTRADETAX_DLG_STATIC7, TEXT("statictext_NONAME7") );

	file.CreateControl( m_pCheckboxIsSetTax, NIZOWORLDTRADETAX_DLG_CHECKBOX_SETTAX, TEXT("checkbox_settax") );
	file.CreateControl( m_pButtonTaxRangeUp, NIZOWORLDTRADETAX_DLG_BUTTON_RANGEUP, TEXT("button_taxrangeup") );
	file.CreateControl( m_pButtonTaxRangeDown, NIZOWORLDTRADETAX_DLG_BUTTON_RANGEDOWN, TEXT("button_taxrangedown") );
	file.CreateControl( m_pButtonTaxRateUp, NIZOWORLDTRADETAX_DLG_BUTTON_RATEUP, TEXT("button_taxrateup") );
	file.CreateControl( m_pButtonTaxRateDown, NIZOWORLDTRADETAX_DLG_BUTTON_RATEDOWN, TEXT("button_taxratedown") );
	file.CreateControl( m_pEditTaxRange, NIZOWORLDTRADETAX_DLG_EDIT_RANGE, TEXT("editbox_taxrange") );
	file.CreateControl( m_pEditTaxRate, NIZOWORLDTRADETAX_DLG_EDIT_RATE, TEXT("editbox_taxrate") );

	file.CreateControl( m_pOutLine[0], NIZOWORLDTRADETAX_DLG_OUTLINE1, TEXT("outline_NONAME1") );
	file.CreateControl( m_pOutLine[1], NIZOWORLDTRADETAX_DLG_OUTLINE2, TEXT("outline_NONAME2") );
	file.CreateControl( m_pOutLine[2], NIZOWORLDTRADETAX_DLG_OUTLINE3, TEXT("outline_NONAME3") );


}

void CNIzoWorldTradeTaxDlg::Action()
{
}

void CALLBACK CNIzoWorldTradeTaxDlg::StaticCallBackDialogNIzoWorldTradeTaxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNIzoWorldTradeTaxDlg *pThis = (CNIzoWorldTradeTaxDlg *)pControllerMgr;
	pThis->CallBackDialogNIzoWorldTradeTaxDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNIzoWorldTradeTaxDlg::CallBackDialogNIzoWorldTradeTaxDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
	case NIZOWORLDTRADETAX_DLG_CHECKBOX_SETTAX:
		if ( EVENT_CHECKBOX_CHANGED == nEvent )
		{
			m_bWorldTradeTax = m_pCheckboxIsSetTax->IsCheck();
		}
		break;
	case NIZOWORLDTRADETAX_DLG_BUTTON_RANGEUP:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSpecialTaxRange += Const_SpecialTaxStep;

			if ( m_siSpecialTaxRange > Const_SpecialTaxRange_Max )
			{
				m_siSpecialTaxRange = Const_SpecialTaxRange_Max;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT("%d"), m_siSpecialTaxRange );
			m_pEditTaxRange->SetText( buffer );
			//m_pButtonTaxRangeUp->Enable(false);
			//m_pButtonTaxRangeDown->Enable(false);
		}
		break;
	case NIZOWORLDTRADETAX_DLG_BUTTON_RANGEDOWN:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSpecialTaxRange -= Const_SpecialTaxStep;

			if ( m_siSpecialTaxRange < Const_SpecialTaxRange_Min )
			{
				m_siSpecialTaxRange = Const_SpecialTaxRange_Min;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT("%d"), m_siSpecialTaxRange );
			m_pEditTaxRange->SetText( buffer );
			//m_pButtonTaxRangeUp->Enable(false);
			//m_pButtonTaxRangeDown->Enable(false);
		}
		break;
	case NIZOWORLDTRADETAX_DLG_BUTTON_RATEUP:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSpecialTaxRate++;

 			if ( m_siSpecialTaxRate > Const_SpecialTaxRate_Max )
			{
				m_siSpecialTaxRate = Const_SpecialTaxRate_Max;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT("%d"), m_siSpecialTaxRate );
			m_pEditTaxRate->SetText( buffer );
			//m_pButtonTaxRateUp->Enable(false);
			//m_pButtonTaxRateDown->Enable(false);
		}
		break;
	case NIZOWORLDTRADETAX_DLG_BUTTON_RATEDOWN:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siSpecialTaxRate--;

 			if ( m_siSpecialTaxRate < Const_SpecialTaxRate_Min )
			{
				m_siSpecialTaxRate = Const_SpecialTaxRate_Min;
				break;
			}

			TCHAR buffer[8];
			StringCchPrintf( buffer, 8, TEXT("%d"), m_siSpecialTaxRate );
			m_pEditTaxRate->SetText( buffer );
			//m_pButtonTaxRateUp->Enable(false);
			//m_pButtonTaxRateDown->Enable(false);
		}
		break;
	}

}
void CNIzoWorldTradeTaxDlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNIzoWorldTradeTaxDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNIzoWorldTradeTaxDlg::LoadWorldTradeTax()
{
	m_pCheckboxIsSetTax->SetCheck( m_bWorldTradeTax );

	TCHAR buffer[16];

	StringCchPrintf( buffer, 16, TEXT("%d"), m_siSpecialTaxRange );
	m_pEditTaxRange->SetText(buffer);

	StringCchPrintf( buffer, 16, TEXT("%d"), m_siSpecialTaxRate );
	m_pEditTaxRate->SetText(buffer);
}
