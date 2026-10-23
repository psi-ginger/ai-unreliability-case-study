#pragma once 


#include "../../InterfaceMgr/DialogController.h"
#include "Msg/MsgType-NPC.h"
//#include "../Client/Interface/ForeignMerchant/ForeignMerchantBaseInfo.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class cltItem;
struct stItemInfoList;
/*
struct stNForeignMerchantBuyItemData
{
	stSellToForeignMerchant *pSellToForeignMerchant;
	SI16 siParam;
};
*/
class CNForeignMerchantBuyCashItemDlg : public CDialogController
{
public:

	CNForeignMerchantBuyCashItemDlg();
	~CNForeignMerchantBuyCashItemDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNForeignMerchantBuyCashItemDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNForeignMerchantBuyCashItemDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetCashItemBuyMode();

	bool IsUserInventoryChanged();
	void LoadUserInventory();
	void Set( stItemInfoList *ItemInfoList, SI16 ListNum );
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화

private:

	SI32				m_siImageType;	// 무기 이미지 종류
	SI32				m_siImageIndex;	// 무기 이미지 인덱스
	SI32				m_siItemType;

	SI32 m_siSelectedIndex;
	SI32 m_siListBoxSelectedIndex;
	SI32 m_siComboBoxSelectedIndex;
	SI32 m_siForeignMerchantKind;

	cltItem *m_pclItem;

	GMONEY m_siItemPrice;

	SI16 *m_siItemUniqueList;
	UI16 *m_uiItemPriceList;
	SI16 m_siListNum;

	CForeignMerchantNPC			*m_pForeignMerchantNPC;

	CStatic*	m_pStatic[6];
	CEdit*		m_pEdit[2];
	CComboBox*	m_pCombo;
	CList*		m_pList;
	CListView*	m_pListView;
	CButton*	m_pButtonSellItem;
	CButton*	m_pButtonClose;
};

