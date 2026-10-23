#pragma once 

#include "../../InterfaceMgr/DialogController.h"


class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class cltItem;

class CNTradeMerchantHighClassDlg : public CDialogController
{
public:

	CNTradeMerchantHighClassDlg();
	~CNTradeMerchantHighClassDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNTradeMerchantHighClassDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNTradeMerchantHighClassDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();

	void SetHighClassMerchant();

	bool IsInventoryChanged();
	void LoadDish();

	void SetDishPrice( SI16 DishItemUnique, GMONEY DishPrice );
	void SellDish( SI16 DishItemUnique );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:
	SI32				m_siImageType;	// 아이템 이미지 종류
	SI32				m_siImageIndex;	// 아이템  이미지 인덱스

	
	GMONEY m_siItemPrice;
	SI32 m_siNPCKind;	
	SI32 m_siSelectedIndex;

	cltItem	*m_pclItem;

	CStatic*	m_pStatic[3];		// 0 : 교역 상인 얼굴, 1: 선택 이미지 2: 그냥 배경
	CEdit*		m_pEdit;		

	CListView*	m_pListView;

	CButton*	m_pButtonSellItem;
	CButton*	m_pButtonClose;
};

