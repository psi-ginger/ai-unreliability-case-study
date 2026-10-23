
#pragma once

#include <Directives.h>
#include "../../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;
class CEdit;
class Cbutton;

class CNStockDistributionDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};

	CNStockDistributionDlg();
	~CNStockDistributionDlg();
	
	void Initialize();
	void Set( SI32 villageUnique, SI32 StockDistributionNum, GMONEY StockPrice, bool VillageWarStockDistribution );

	static	void CALLBACK NStockDistributionStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NStockDistributionProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();

	bool m_bShow;

private:

	SI32 m_siVillageUnique;
	SI32 m_siAvailableStockBuyingNum;
	GMONEY m_siStockPrice;
	bool m_bVillageWarStockDistribution;

	CStatic			*m_pStatic_NONAME1	;
	CButton			*m_pButton_buy	;
	CButton			*m_pButton_reject	;
	CEdit			*m_pEdit_explain	;
	CEdit			*m_pEdit_num	;

};


