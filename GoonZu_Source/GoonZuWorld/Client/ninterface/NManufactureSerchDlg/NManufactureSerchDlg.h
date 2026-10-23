#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CEdit;
class CList;
class CRadioButton;
class CStatic;
class CComboBox;
class CCheckBox;

#define NMANUFACTURESEARCHDLG_SETTIMER_TIME 300

class CNManufactureSearchDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 80,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 80,
	};

	CNManufactureSearchDlg();
	~CNManufactureSearchDlg();

	void Initialize();
	void Destroy();

	void Create();

	static	void CALLBACK StaticNManufactureSearchDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NManufactureSearchDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	
	bool IsShow();
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	bool SetItemTypeInCombo( bool bshowall);

	bool IsShopShowCaseChanged();
	void SortSearchedData();
	void LoadShopShwocase();
	void DisplaySowCaseItemInfo(SI32 index);
	void SearchShopItem();

	void SetItemImageData(SI32 ItemUnique);
	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	
public:
	CStatic			*m_pStatictext_itemsearch	;
	CComboBox		*m_pCombobox_iteminfo_type	;	
	CRadioButton	*m_pRadiobutton_makeitem	;	
	CRadioButton	*m_pRadiobutton_makeitem_want	;		// 제작의뢰 
	CButton			*m_pButton_search	;	
	CList			*m_pListbox_shop_showcase	;
	CEdit			*m_pEditbox_item_explain	;
	CEdit			*m_pEditbox_shopper_location	;
	CButton			*m_pButton_personalshop_warp	;	
	CButton			*m_pButton_close	;	
	CImageStatic	*m_pImageStatic_Item;
	bool			m_bShow;
	bool			m_bTimer;	
	BOOL			m_bShopDataChanged;

	cltSearchShopData	m_clSearhShopData;

	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	SI32			m_siShopShowCaseSelectedIndex;

	clock_t			m_LastSearchTime; 

	SI32	m_siImageIndex;
	SI32	m_siImageType;
};

