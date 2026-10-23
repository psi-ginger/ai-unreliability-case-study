#pragma once

#include "Resource.h"
#include "NNewMakeItemDlg.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"



class CButton;
class CListView;

class CNNewMakeChild  : public CDialogController
{
public:
		CNNewMakeChild();
	 ~CNNewMakeChild();
	 
	static void CALLBACK	StatiCNNewMakeChildProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NNewMakeChildProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create(CControllerMgr *pParent);
	void ToolListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent );
	void MaterialListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent );
	void Show();
	void Hide();
	void ShowTools(cltItem cltools[]);
	void ShowMaterials(cltItem cltools[]);
	void ComboMakeItem();
	void Action();
	void SetMakeItemNumEnable();
	SI32 GetComboBoxPosX();
	SI32 GetComboBoxPosY();
	void AutoSetItemMat();//자동으로 아이템을 넣는 부분.
	void SetMakeItemComboBoxEnable(TCHAR		* m_siComboBoxSelectedName);
	void ClearMakeItemMat();//자동으로 아이템을다 뺀다.

	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

private:
	
	//cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
	//SI32					m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];
	//cltPItemInfo			*m_pPersonItemInfo;
	cltItem		*m_pclInventoryItem;

	InterfaceFactory m_InterfaceFactory;
	SI16		m_siSelectedIndex;
	cltItem     m_cltools[4];
	cltItem     m_clMaterials[25];
	SI32		*m_pMakeableItemList;
	SI32		m_siMakeableItemNum;
	SI16		m_siComboBoxSelectedIndex;
	TCHAR		* m_siComboBoxSelectedName;

};
