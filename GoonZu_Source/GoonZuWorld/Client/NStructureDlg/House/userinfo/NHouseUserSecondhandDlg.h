#pragma once 

#include "../Client/InterfaceMgr/DialogController.h"
#include "../CommonLogic/House/House.h"


class CEdit;
class CButton;
class CList;

class CHouseUnitBuffer;

class CNHouseUserSecondhandDlg : public CDialogController
{
public:

	CNHouseUserSecondhandDlg();
	~CNHouseUserSecondhandDlg();

	void Create( CControllerMgr *pParent );
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticCallBackDialogNHouseUserSecondhandDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHouseUserSecondhandDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer);
	void SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex);
	void SetHouseUserItem(cltHouseUnit *pclHouseUnit);
	void LoadWareHouseInventory1();
	bool IsWareHouseInventoryChanged();
	void ShowMakeItemFromUsedGoodsItem(cltItem* pcltItem);

public:

	CHouseUnitBuffer			*m_pHouseUnitBuffer;
	cltHouseUnit				*m_pclHouseUnit;
	SI32			m_siCurrentVillageIndex;
	SI32			m_siCurrentHouseIndex;
	SI32			m_siUsedGoodsItemSelectedIndex;
	SI32			m_siUsedGoodsItemRealSelectedIndex;

	cltItem			m_clWareHouseItem[ MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER ];

	CEdit*		m_pEdit[2];
	CList*		m_pList;
	CButton*		m_pButton[2];	// 0 : 고물상 짓기 1: 물품 해체

};