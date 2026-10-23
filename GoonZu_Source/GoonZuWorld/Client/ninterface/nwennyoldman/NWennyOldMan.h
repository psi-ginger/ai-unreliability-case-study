
// NWennyOldMan.h: interface for the CNWennyOldMan class.
//
//////////////////////////////////////////////////////////////////////

/* ==========================================================================
	클래스 :		NNWennyOldman

	작성일 :		05/04/04
	작성자 :		김태양
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#pragma once

#include <Directives.h>
#include <stdio.h>

#include "../resource.h"
#include "NWennyChild1.h"
#include "NWennyChild2.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"


class CItemListView;
class CButton;
class CListView;
class CEdit;
class CStatic;
class CNWennyOldManUpgradeItem;
class CNWennyOldManPerformanceUpgradeItem;

enum WENNYOLDMAN_AUTO_STATE
{
	WENNYOLDMAN_AUTO_STATE_NONE,
	WENNYOLDMAN_AUTO_STATE_BUTTON_CLICK,
	WENNYOLDMAN_AUTO_STATE_SEND,
	WENNYOLDMAN_AUTO_STATE_WAITRECV,
	WENNYOLDMAN_AUTO_STATE_RECV,
	WENNYOLDMAN_AUTO_STATE_WAIT,
	WENNYOLDMAN_AUTO_STATE_END,	
};

class CNWennyOldMan  : public CDialogController
{
public:

	CNWennyOldMan();
	 ~CNWennyOldMan();

	static void CALLBACK	StaticNWennyOldManProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NWennyOldManProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	
	void Create();	
	void Action();
	void SetWennyText(TCHAR* ptext);
	void ClearAutoState()		{	m_siAutoState = WENNYOLDMAN_AUTO_STATE_END;	}

	BOOL IsChangedInvItem();
	void LoadDisAssembleItemData();
	void LoadWeenyOldManInven();
	void SetWeennyStorage(cltItem* itemlist);

private:

	BOOL	m_bShow;
	SI32	m_siSelectIndexMyInven;
	SI32	m_siSelectIndexStorage;		

	SI32	m_siStorageCount;

	SI32	m_sendFrame;

	SI32	m_siAutoState;

	cltItem			m_clUserItem[ MAX_ITEM_PER_PERSON ];
	cltItem			m_clWennyItem[ MAX_ITEM_PER_WENNYOLD ];
	GMONEY			m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];

	SI32			m_siInventoryPos[ MAX_ITEM_PER_PERSON ];
	cltPItemInfo	*m_pPersonItemInfo;

   
	CStatic         *m_pNWennyStatic;		
	CStatic         *m_pNWennyStaticGreeting;  
	CButton         *m_pNWennyButtonDisAssemble;
	CButton         *m_pNWennyButtonRecieve;
	CStatic         *m_pNWennyStatic1;		
	CStatic         *m_pNWennyStatic2;		
	CListView		*m_pMyInventory;
	CListView		*m_pWennyStorage;

	clock_t			m_SendDisassembleClock;

};

