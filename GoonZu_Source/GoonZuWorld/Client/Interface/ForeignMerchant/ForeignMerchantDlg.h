#ifndef _FOREIGNMERCHANTDLG_H_
#define _FOREIGNMERCHANTDLG_H_

#include "ForeignMerchantBaseInfo.h"


class cltPItemInfo;
class CLibListMgr;
class CForeignMerchantNPC;

class CForeignMerchantDlg
{
public:
	HINSTANCE			m_hInst;
	HWND				m_hDlg;
	
	CForeignMerchantDlg();
	~CForeignMerchantDlg();

	void	Initialize();
	void	Destroy();

	void	Create( HWND hWnd, HINSTANCE hInst );
	
	static	BOOL CALLBACK StaticForeignMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK ForeignMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	void	CreateForeignMerchantDlg( SI16 siForeignMerchantKind, SI32 npcccharunique);

	bool	IsShow();
	void	Show( SI32 npcccharunique );
	void	Hide();

	void	LoadInventoryData();
	void	LoadForeignNPCData();

	bool	IsInventoryChanged();
	bool	IsNPCBuyItemInfoChanged();

	void	Push( UI16 uiSellItemNum );
	bool	Pop( stSellToForeignMerchant *pSellToForeignMerchant );

private:

	HWND				m_hSellItemListCtrl;

	SI16				m_siForeignMerchantKind;

	bool				m_bShow;

	SI32				m_siSelectedIndex;
	SI32				m_siListBoxSelectedIndex;

	cltItem				m_clItem[ MAX_ITEM_PER_PERSON ];		// 장착된 아이템은 체크할 필요 없으므로...

	cltPItemInfo		*m_pPersonItemInfo;	
	
	CLibListMgr			*m_pForeignMerchantMgr;

	HBITMAP				m_hItemImage;

	SI32				m_si32ImageListIndex;

	bool				m_bIsListRotate;
	
	HIMAGELIST			m_hInventoryImageList;

	CForeignMerchantNPC			*m_pForeignMerchantNPC;
	stForeignMerchantNPCInfo	m_ForeignMerchantNPCInfo;

	HBITMAP				m_hPortraitBitmap;
	HBITMAP				m_hItemBitmap;
	
	SI32				m_siNPCCharUnique;
	SI32				m_siItemType;

};

#endif