#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Lib/Directives.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class COutLine;

class CNIzoPriceValueDlg;
class CNIzoWorldTradeTaxDlg;
class CNIzoMarketPriceDlg;

class CNIzoBaseDlg : public CDialogController
{
public:

	CNIzoBaseDlg();
	~CNIzoBaseDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNIzoBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNIzoBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetIzoBase(CMinisterBase *pMini,SI32 pricetime,SI32 priceinx,SI32 pricevalue,
					GMONEY money,SI32 calctime, GMONEY tradetax,GMONEY tradelimitmoney,
					bool worldtradetax, SI32 specialtaxrange, SI32 specialtaxrate, SI32 marketdownrate,
					SI16 tintin);

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(SI32 Index);


private:

	TCHAR m_szIzoName[MAX_PLAYER_NAME];
	TCHAR m_szIzoProfile[256];
	TCHAR m_szIzoID[MAX_PLAYER_NAME];

	BOOL m_bSet;
	BOOL m_bIzoCheck;
	BOOL m_bSound;

	CStatic*	m_pStatic[3];
	CButton*	m_pButton[5];
	CEdit*		m_pEdit;
	COutLine*	m_pOutLine;
	CNIzoPriceValueDlg*		m_pNIzoPriceValueDlg;
	CNIzoWorldTradeTaxDlg*	m_pNIzoWorldTradeTaxDlg;
	CNIzoMarketPriceDlg*	m_pNIzoMarketPriceDlg;

	CButton*	m_pButtonTab[3];
};