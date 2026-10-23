#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CNIzoMarketPriceDlg : public CDialogController
{
private:
	SI32	m_siMarketDownRate;

public:
	CNIzoMarketPriceDlg();
	~CNIzoMarketPriceDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	static void CALLBACK StaticCallBackDialogNIzoMarketPriceDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNIzoMarketPriceDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	CStatic*	m_pStatic[3];
	CEdit*		m_pEditDownRate;
	CButton*	m_pButtonRateUp;
	CButton*	m_pButtonRateDown;

	void	SetMarketDownRate( SI32 rate ){ m_siMarketDownRate = rate; }
	SI32	GetMarketDownRate(){ return m_siMarketDownRate; }

	void	LoadMarketDownRate();
};