
/* ==========================================================================
	클래스 :		CNNPCTradeDlg

	작성일 :		2005/03/31
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/

#include "../../interface/NPC1/NPCTradeBaseInfo.h"
#include "../../InterfaceMgr/DialogController.h"
#define MAX_AVAILABLE_CASHITEMNUM		256


class CNPCTradeBuffer;
class CButton;
class CStatic;
class CListView;
class CEdit;

class CNNPCTradeDlg : public CDialogController
{
public:
	CNNPCTradeDlg();
	~CNNPCTradeDlg();

	void	Initialize();
	void	Destroy();
	//void    Set(cltPItemInfo *pPersonItemInfo, cltNPCItemInfo *pNPCItemInfo);

	void	Create( );

	
	static void CALLBACK StaticNNPCTradeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NNPCTradeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool	IsUserInventoryChanged();						// 유저 인벤토리가 바뀐 것이 있는지 체크
	bool	IsNPCInventoryChanged();						// NPC 인벤토리가 바뀐 것이 있는지 체크

	void	LoadUserInventory();							// 유저 인벤토리 데이터 로딩
	void	LoadNPCInventory();								// NPC  인벤토리 데이터 로딩

	void	SellItemToNPC();								// 유저가 NPC 에게 아이템을 판매
	void	BuyItemFromNPC();								// NPC 로 부터 유저가 아이템 구입

	void    SettingListView();
	void	UpdateUserInventory();							// 유저가 아이템을 팔거나 구입할 시에 서버로 부터 데이터를 받아서 변경되었으면, 그 때 update 해 주기 위해서...
	void	UpdateNPCInventory();							// NPC의 인벤토리는 거의 변할 일이 없지만, 혹시나 가격이 변할 수도 있으므로...
	void    Action();
	
	void	Show(SI32 npckind);								// 다이얼로그 보여 주기	
	//void	Hide();											// 다이얼로그 숨기기

	//bool	IsShow();

	bool	Pop( stNPCTradeData * pNPCTradeData );			// 데이터 가져가는 함수

	void	SetText( TCHAR *strText );						// 팔거나 사거나 했을때 상황을 보여줘야 하므로...
	void	SetText( SI32 siTextIndex );					// 팔거나 사거나 했을때 상황을 보여줘야 하므로...
	bool LoadBuyCashItemInfo();
	bool IsSameItemTypeExist( TCHAR *ItemType );
	bool IsCheckCashItem(SI32 Uinque);

	void	SetDialogInfo( SI32 siTitleTextNum, SI32 siExplainTextNum );


private:
	cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
	GMONEY					m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];

	cltItem					m_clNPCItem[ MAX_NPC_ITEM_NUMBER ];
	GMONEY					m_siNPCItemPrice[ MAX_NPC_ITEM_NUMBER ];

	cltPItemInfo			*m_pPersonItemInfo;
	cltNPCItemInfo			*m_pNPCItemInfo;

	//HWND					m_hDlg;
	//HWND					m_hUserInventory;
	//HWND					m_hNPCInventory;
	//HWND					m_hNPCTradeInfoViewEdit;
	bool					m_bShowFlag;


	CNPCTradeBuffer			*m_pNPCTradeBuffer;

	SI32					m_siUserInventorySelectedIndex;
	SI32					m_siNPCInventorySelectedIndex;

	SI32					m_siSellItemNum;
	SI32					m_siBuyItemNum;
	
	SI32					m_siNPCCharUnique;
	SI32					m_siNPCKind;

	CStatic     	       *m_pNNPCTradeStatic1;
	CStatic     	       *m_pNNPCTradeStatic2;
	CStatic     	       *m_pNNPCTradeStatic3;
	CStatic     	       *m_pNNPCTradeStatic4;
	CStatic     	       *m_pNNPCTradeStatic5;
	CEdit                  *m_pNNPCTradeEdit;
	CButton                *m_pNNPCTradeButton1;
	CButton                *m_pNNPCTradeButton2;
	CListView              *m_pNNPCTradeListView1;
	CListView              *m_pNNPCTradeListView2;


	TCHAR m_strItemTypeList[ MAX_ITEMTYPE_NUMBER ][ 64 ];
	SI16 m_siItemUniqueList[ MAX_AVAILABLE_CASHITEMNUM ];
	UI16 m_uiItemPriceList[ MAX_AVAILABLE_CASHITEMNUM ];

	//HBITMAP					m_hPortraitBitmap;

};
