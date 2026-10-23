#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CNIzoWorldTradeTaxDlg : public CDialogController
{
private:
	bool	m_bWorldTradeTax;
	SI32	m_siSpecialTaxRange;
	SI32	m_siSpecialTaxRate;

public:
	CNIzoWorldTradeTaxDlg();
	~CNIzoWorldTradeTaxDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	static void CALLBACK StaticCallBackDialogNIzoWorldTradeTaxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNIzoWorldTradeTaxDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void SetbWorldTradeTax( bool set ){ m_bWorldTradeTax = set; }
	void SetSpecialTaxRange( GMONEY range ){ m_siSpecialTaxRange = range; }
	void SetSpecialTaxRate( SI32 rate ){ m_siSpecialTaxRate = rate; }
	bool GetbWorldTradeTax(){ return m_bWorldTradeTax; }
	SI32 GetSpecialTaxRange(){ return m_siSpecialTaxRange; }
	SI32 GetSpecialTaxRate(){ return m_siSpecialTaxRate; }

	void LoadWorldTradeTax();

	CStatic*	m_pStatic[7];
	CCheckBox*	m_pCheckboxIsSetTax;
	CEdit*		m_pEditTaxRange;
	CEdit*		m_pEditTaxRate;
	CButton*	m_pButtonTaxRangeUp;
	CButton*	m_pButtonTaxRangeDown;
	CButton*	m_pButtonTaxRateUp;
	CButton*	m_pButtonTaxRateDown;
	COutLine*	m_pOutLine[3];
};