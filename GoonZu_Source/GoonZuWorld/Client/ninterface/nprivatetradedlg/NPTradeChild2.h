// 주식
#pragma once

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"


class CEachPersonTradeOrderInfo;
class CNPrivateTradeDlg;
class CLibListMgr;
class CListView;
class CComboBox;
class CButton;
class CStatic;
class CEdit;




class CNTradeChild2 : public CDialogController
{
	
public:
	CNTradeChild2();
	~CNTradeChild2();
	


	
	static void CALLBACK StaticNTradeChild2Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NTradeChild2Proc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Create(CControllerMgr *pParent);
	void LoadStockList();
	bool IsStockListChanged();
	void Show();
	void Hide();
	CEdit               *m_pNPTrade2Edit;
	CNPrivateTradeDlg*   m_pParent;
	SI32                 m_siSelectedVillageIndex;
	CComboBox           *m_pNPTrade2Combo;

private:
	//UI32	        	m_uiLastAcceptFrame;	// 거래에 주어지는 간격
	SI32	        	m_SILastAcceptTime;		// 거래에 주어지는 간격	-> 프레임에서 커런트 타임을 저장하도록 수정 
	CButton             *m_pNPTrade2Button1;
	CButton             *m_pNPTrade2Button2;
	
	
	CStatic             *m_pNPTrade2Static1;
	CStatic             *m_pNPTrade2Static2;
	//SI32				m_siStockKindList[MAX_STOCK_KIND_IN_ACOUNT];	// 주식 리스트 ( 종목 )
	//SI32				m_siStockNumList[MAX_STOCK_KIND_IN_ACOUNT];		// 주식 리스트 ( 개수 )




};