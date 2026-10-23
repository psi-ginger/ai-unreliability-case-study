#ifndef _NFOREIGNMERCHANT_H_
#define _NFOREIGNMERCHANT_H_


#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

// 명나라 상인

class CNForeignMerchantDlg : public CDialogController
{
public:

	CNForeignMerchantDlg();
	~CNForeignMerchantDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNForeignMerchantDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNForeignMerchantDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetNPCCharUnique(SI32 NPCCharUnique);

private:

	SI32		m_siNPCCharUnique;

	CStatic*	m_pStatic;
	CEdit*		m_pEdit;
	CButton*	m_pButtonNomalItemBuy;
	CButton*	m_pButtonCashItemBuy;
};

#endif