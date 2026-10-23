#ifndef _NFOREIGNMERCHANTBuyItem_H_
#define _NFOREIGNMERCHANTBuyItem_H_


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

class cltPItemInfo;
class CLibListMgr;
class CForeignMerchantNPC;

#define FOREIGNMERCHANTDLG_SETTIMER_TIME			1000

#define MAX_FOREIGN_MERCHANT_OBJ_NUM				30
#define MAX_FOREIGN_MERCHANT_OBJ_BUFFER_SIZE		128		// 128 byte 면 충분

struct stForeignMerchantHeader
{
	UI16 uiMsg;
	UI16 uiSize;
};

struct stSellToForeignMerchant
{
	stForeignMerchantHeader sHeader;
	SI16					siForeignMerchantKind;		// 상인 구분(일본 상인, 청상인)
	UI16					uiSelectedIndex;			// 아이템 위치 (당연히 10이상의 값이겠지... 장착창에서는 바로 팔수 없으므로... )
	UI16					uiSellItemNum;				// 아이템 판매 개수
	SI16					siSellItemUnique;			// 아이템 유니크 값, <--- 버그의 위험성 때문에... 기간이 끝나는 시점에서 다른 아이템으로 변경되는 동안에 보내진 패킷일 경우에 대한 대비책

	SI32					siNPCCharUnique;			// NPC CharUnique

	stSellToForeignMerchant()
	{
		siForeignMerchantKind = 0;
		uiSelectedIndex = 0;
		uiSellItemNum = 0;
		siSellItemUnique = 0;
		siNPCCharUnique = 0;

		sHeader.uiMsg = SELL_TO_FOREIGNMERCHANT;
		sHeader.uiSize = sizeof( stSellToForeignMerchant );
	}
};

struct stNForeignMerchantBuyItemData
{
	stSellToForeignMerchant *pSellToForeignMerchant;
	SI16 siParam;
};

class CNForeignMerchantBuyItemDlg : public CDialogController
{
public:

	CNForeignMerchantBuyItemDlg();
	~CNForeignMerchantBuyItemDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNForeignMerchantBuyItemDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNForeignMerchantBuyItemDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetNormalItemBuyMode(SI32 NPCCharUnique);

	void SetData(void *pBuffer );
//////////////////////////////////////////////////////////////////////////////////////////////
//	일반 아이템 관련
	void LoadInventoryData();
	void LoadForeignNPCData();

	bool IsInventoryChanged();
	bool IsNPCBuyItemInfoChanged();

	void Push( UI16 uiSellItemNum );
	bool Pop( stSellToForeignMerchant *pSellToForeignMerchant );
	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
//////////////////////////////////////////////////////////////////////////////////////////////

private:

	SI32				m_siImageType;	// 무기 이미지 종류
	SI32				m_siImageIndex;	// 무기 이미지 인덱스
	SI16				m_siForeignMerchantKind;
	SI32				m_siSelectedIndex;
	SI32				m_siListBoxSelectedIndex;
	SI32				m_si32ImageListIndex;
	SI32				m_siNPCCharUnique;
	SI32				m_siItemType;

	bool				m_bIsListRotate;

	cltItem						m_clItem[ MAX_ITEM_PER_PERSON ];		// 장착된 아이템은 체크할 필요 없으므로...
	cltPItemInfo				*m_pPersonItemInfo;	
	CLibListMgr					*m_pForeignMerchantMgr;
	CForeignMerchantNPC			*m_pForeignMerchantNPC;
	stForeignMerchantNPCInfo	m_ForeignMerchantNPCInfo;

	

	CStatic*	m_pStatic[6];
	CEdit*		m_pEdit[2];
	CComboBox*	m_pCombo;
	CList*		m_pList;
	CListView*	m_pListView;
	CButton*	m_pButtonSellItem;
	CButton*	m_pButtonClose;
};

#endif
