#pragma once

#include "Resource.h"
#include "NWennyOldMan.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"



class CButton;
class CListView;

class CNWennyOldManUpgradeItem  : public CDialogController
{
public:
		CNWennyOldManUpgradeItem();
	 ~CNWennyOldManUpgradeItem();
	 
	static void CALLBACK	StaticNWennyOldManUpgradeItemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NWennyOldManUpgradeItemProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create(CControllerMgr *pParent);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void LoadUpgradeItemData();	
	void UpgradeItemSuccess( bool Success );
	void Show();
	void Hide();
	void Action();


	
	BOOL IsChangedInvItem();	

	
private:
	
	cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
	GMONEY					m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];
	cltPItemInfo			*m_pPersonItemInfo;
	


	//cltItem			*m_pInvChildItem;
    SI32			m_siSelectIndex;
	SI32			m_siVillage;
	CListView       *m_pNWennyUpgradeListview;	
	CButton         *m_pNWennyUpgradeButton1;
	CButton         *m_pNWennyUpgradeButton2;


};
