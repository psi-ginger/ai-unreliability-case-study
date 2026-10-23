#pragma once

// 돈 물품 
#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"


class CEachPersonTradeOrderInfo;
class CLibListMgr;
class CListView;
class CEdit;
class CStatic;
class CButton;
class CNPrivateTradeDlg;

class CNTradeChild1 : public CDialogController
{
	
public:
	CNTradeChild1();
	~CNTradeChild1();
	
	static void CALLBACK StaticNTradeChild1Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NTradeChild1Proc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Create(CControllerMgr *pParent);
	void Show();
	void Hide();
	void LoadInventoryData();
	bool IsInventoryChanged();

	void PushAddItem();

	SI16                m_siInvSelectedIndex;
	cltItem				m_clUserItem[ MAX_ITEM_PER_PERSON ];
	CNPrivateTradeDlg* m_pParent;
	CEdit               *m_pNPTrade1Edit1;
	CEdit               *m_pNPTrade1Edit2;
	CButton             *m_pNPTrade1Button1;
	CButton             *m_pNPTrade1Button2;
	CButton             *m_pNPTrade1Button3;
	GMONEY				m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];

private:
	CEachPersonTradeOrderInfo *m_pEachPersonTradeOrderInfo;
	cltPItemInfo		*m_pPersonItemInfo;	
	//cltItem			m_clItem[ MAX_ITEM_PER_PERSON ];		// 장착된 아이템은 체크할 필요 없으므로...
	
	

	SI32				m_siSelectIndex;	
	CListView           *m_pNPTradeListView1;
		
	CStatic             *m_pNPTrade1Static1;
	CStatic             *m_pNPTrade1Static2;
	
	
};