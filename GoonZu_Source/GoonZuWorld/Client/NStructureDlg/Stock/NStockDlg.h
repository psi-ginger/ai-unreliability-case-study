#ifndef _NSTOCKDLG_H
#define _NSTOCKDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../NLib/NUpdateTimer.h"//주식공모시 5초 딜레이위한 헤더

class CComboBox;

// 여각 기본
class NStockDlg : public NStrBaseDlg
{
public:
	NStockDlg();
	virtual ~NStockDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 주식 매매 
class NStockUserDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_BUY,
		BUTTON_SELL,
		BUTTON_CANCEL_ORDER,

		COMBO_BUY_KIND,
		COMBO_SELL_KIND,

		EDIT_BUY_AMOUNT,
		EDIT_BUY_PRICE,
		EDIT_SELL_AMOUNT,
		EDIT_SELL_PRICE,

		EDIT_FEERATE,

		LIST_HAVESTOCK,
		LIST_ORDERSTOCK,
		LIST_MOST_ORDER_SELL,
		LIST_MOST_ORDER_BUY,
		
		EDIT_AVERAGE_BUY,
		EDIT_AVERAGE_SELL

	};

	NStockUserDlg();
	virtual ~NStockUserDlg();

	void InitDlg();

	void Action();

	// 보유 주식을 표시한다. 
	void SetHaveStock();

	// 주문 내용을 리스트 박스에 추가한다. 
	void SetStockOrderList();

	// 매수 종목을 콤보 박스에 추가한다.
//	void SetComboBuyKind();

	// 매도 종목을 콤보 박스에 추가한다. 
	void SetComboSellKind();

	void ShowFeeInfo();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 증자 
class NStockUser2Dlg : public NStrTabDlg
{
public:
	enum {
		COMBO_BUYER_TYPE, // 주주가 사는냐 일반인이 사느냐
		COMBO_VILLAGE,	  // 주식을 살 마을

		EDIT_DATE,		  // 증자 남은 시간
		EDIT_TOTAL_INCREASESTOCK_AMOUNT,
		EDIT_LEFTSTOCK_AMOUNT,
		EDIT_CURRENT_STOCKPRICE,
		EDIT_BUYSTOCK_AMOUNT,
		EDIT_LIMITBUYSTOCK_AMOUNT,
		EDIT_STARTDAY,	// 증자 시작일!!

		BUTTON_BUYSTOCK,

		LIST_STOCKHOLDER	// 상위 주주 명부
	};

	SI32 m_siSelectVillageIndex_;
	/*
	#ifdef _DEBUG // kkm
	enum {
	COMBO_BUYER_TYPE, // 주주가 사는냐 일반인이 사느냐
	COMBO_VILLAGE,	  // 주식을 살 마을

	EDIT_DATE,		  // 증자 남은 시간
	EDIT_TOTAL_INCREASESTOCK_AMOUNT,
	EDIT_LEFTSTOCK_AMOUNT,
	EDIT_CURRENT_STOCKPRICE,
	EDIT_BUYSTOCK_AMOUNT,

	BUTTON_BUYSTOCK,

	LIST_STOCKHOLDER	// 상위 주주 명부
	};
	#else
	enum {
	COMBO_STOCKBID_ITEM,

	EDIT_TOTAL_AMOUNT,
	EDIT_BEGIN_PRICE,

	EDIT_AMOUNT,
	EDIT_PRICE,

	EDIT_DATE,

	BUTTON_BIDSTOCK,
	BUTTON_REFRESH_LIST,

	LIST_STOCKBIDDER
	};
	#endif
	*/	
	NStockUser2Dlg();
	virtual ~NStockUser2Dlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	// 콤보박스 텍스트에 따라 정렬
	void NComboBoxSort(CComboBox* in_pCombo );
};

// 행수 메뉴 
class NStockChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};

	NStockChiefDlg();
	virtual ~NStockChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};

// 공모 청약
class NStockPublicSubscriptionDlg : public NStrTabDlg
{
public:
	enum {
		COMBO_VILLAGELIST,
		BUTTON_ORDER,
	};

	NUpdateTimer	m_kUpdateTimer_ButtonDisable;

	NStockPublicSubscriptionDlg();
	virtual ~NStockPublicSubscriptionDlg();

	void InitDlg();
	
	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	virtual void Show();
};

#endif