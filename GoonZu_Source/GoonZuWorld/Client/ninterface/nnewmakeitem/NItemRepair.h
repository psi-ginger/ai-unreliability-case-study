#ifndef _NITEMREPAIR_H_
#define _NITEMREPAIR_H_

#include <Directives.h>

#include "../../../Common/Item/ItemCommon/cltItem.h"

#include "../../InterfaceMgr/DialogController.h"

class NItemRepairDlg : public CDialogController
{
public:
	NItemRepairDlg();
	~NItemRepairDlg();

	void Init();

	void Create( CControllerMgr *pParent );

	static	void CALLBACK StaticNItemRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NItemRepairDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetMode( SI16 siMode );		// 1:직접수리, 2:수리의뢰, 3:의뢰수리, 4:수리의뢰 중 [취소하는 창]
	SI16 GetMode();
	void Show( bool bShow );
	void Action();

	virtual bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

	SI32 GetRepairItemPos()					{		return m_siRepairItemPos;						}
	SI32 GetMaterialItemCount()				{		return m_ExtraMaterial.GetSize();				}
	SI32 GetMaterialItemPos(SI32 siIndex)	{		return m_ExtraMaterial[siIndex].siStgPos;		}

	void UpdateSelectItemComboBox();

	void InsertRepairItem( SI16 siItemPos, cltItem* pclitem );
	void InsertStoneItem( SI16 siItemPos, cltItem* pclitem  );

	void SetRepairInfo( SI32 siItemCharIUnique, SI32 siItemCharPersonID, SI16 siRepairItemPos, cltItem* pclRepairItem, RepairItemExtraMaterial& kMaterial, GMONEY Money );

	void CloseDialog();

	SI32 GetItemCharUnique()				{		return m_siItemCharUnique;						}

protected:
	void UpdateInterface();

private:
	InterfaceFactory		m_InterfaceFactory;

	SI16					m_siRepairItemPos;
	cltItem					m_clRepairItem;

	RepairItemExtraMaterial	m_ExtraMaterial;
	NSafeTArray<cltItem, REPAIRE_ITEM_EXTRAMATERIAL_COUNT>		m_ExtraMaterialItem;

	NSafeTArray<cltItem, MAX_ITEM_PER_PERSON - PERSONITEM_INV0>		m_clBeforeItem;

	SI16					m_siMode;

	SI32					m_siItemCharUnique;
	SI32					m_siItemPersonID;

	GMONEY					m_Money;

};

#endif