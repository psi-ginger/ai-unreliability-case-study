// [영훈] 아큐 거래창 리뉴얼
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Server/NTradeServerProcess/TradeServerMsg.h"

class CNAcuTradeDlg_Withdraw : public CDialogController
{
	// 변수영역
private:
	InterfaceFactory	m_InterfaceFactory;

	SI64				m_siSavedACU;
	cltTradeAcuList		m_clAcuList[MAX_TRADESERVER_ACU_LIST];


	// 함수영역
public:
	CNAcuTradeDlg_Withdraw();
	~CNAcuTradeDlg_Withdraw();

	void Create(CControllerMgr* pParent);

	static void CALLBACK StaticAcuTradeDlg_WithdrawProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAcuTradeDlg_WithdrawProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void Show();
	void Hide();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetMemberData( SI64 siSavedACU, cltTradeAcuList* pclAcuList=NULL );
	
	void Refresh( void );

	void Request_SavedACU_List( void );
private:
	void OrderToWithdraw( void );
	
	
	

};
