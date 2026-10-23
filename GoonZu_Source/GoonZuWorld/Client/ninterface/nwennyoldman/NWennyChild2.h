#pragma once

#include "Resource.h"
#include "NWennyOldMan.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"

class CButton;
class CListView;

class CNWennyOldManPerformanceUpgradeItem  : public CDialogController
{
public:
		CNWennyOldManPerformanceUpgradeItem();
	 ~CNWennyOldManPerformanceUpgradeItem();
	 
	static void CALLBACK	StaticNWennyOldManPerformanceUpgradeItemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NWennyOldManPerformanceUpgradeItemProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create(CControllerMgr *pParent);	
	void LoadPerformanceUpgradeItemData();
	void PerformanceUpgradeItemSuccess();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	BOOL IsChangedInvItem();
	void Action();

	void Show();
	void Hide();
	


private:
	cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
	GMONEY					m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];
	cltPItemInfo			*m_pPersonItemInfo;
	
   
    SI32			m_siSelectIndex;
	SI32			m_siVillage;
	CListView       *m_pNWennyPerformanceListview;	
	CButton         *m_pNWennyPerformanceButton1;
	CButton         *m_pNWennyPerformanceButton2;




};
