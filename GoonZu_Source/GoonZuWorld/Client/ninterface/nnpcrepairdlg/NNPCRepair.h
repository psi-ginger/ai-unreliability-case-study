//---------------------------------
// 2003/3/11 김태곤
//---------------------------------

/* ==========================================================================
	클래스 :		NcltNPCRepairManager

	작성일 :		05/03/29
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/
#include <Directives.h>

#include "../../InterfaceMgr/DialogController.h"
#include "..\../../Client\Inventory\ItemInventoryBaseDefinition.h"

#define NPCREPAIR_SETTIMER_VALUE	1000

class CButton;
class CStatic;
class CListView;


class NcltNPCRepairManager : public CDialogController
{
public:

	bool bDlgRectInitSwitch;
	RECT DlgRect;

	NcltNPCRepairManager();
	~NcltNPCRepairManager();

	void Create(SI32 CharUnique);

	static void CALLBACK StaticNDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);


	void Action();
	bool IsChangedInvItem();
	void LoadItemData();
	void SettingListView();


	void SetItemRepairPriceExplain( SI32 ItemUnique, GMONEY ItemPrice );
	void SetReairSuccessMent( TCHAR *Text );

private:
	//CItemListView	*m_pItemListView;
	cltItem			*m_pclUserInvItem;
	SI16			m_siInvSelectedIndex;
	SI16			m_siPreInvSelectedIndex;
	//HBITMAP			m_hPortraitBitmap;	
	SI32			m_siCharUnique;	
	GMONEY			m_siItemRepairPrice;
	
	CStatic     	*m_pNNPCRepairStatic1;
	CStatic     	*m_pNNPCRepairStatic2;
	CStatic     	*m_pNNPCRepairStatic3;
	CListView      	*m_pNNPCRepairListView;	
	CButton     	*m_pNNPCRepairButton1;
	CButton     	*m_pNNPCRepairButton2;





};


