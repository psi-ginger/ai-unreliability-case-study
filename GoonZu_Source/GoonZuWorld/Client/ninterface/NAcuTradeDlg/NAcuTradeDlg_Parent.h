// [영훈] 아큐 거래창 리뉴얼
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Server/NTradeServerProcess/TradeServerMsg.h"

struct stACUTRADE_RETMSG_ORDERTOSELL
{
	GMONEY gmSellAcu;
	GMONEY gmSellUnit;
};

enum ACUTRADE_RETMSG
{
	ACUTRADE_RETMSG_ORDERTOSELL = 0,	// Acu 팔기
	ACUTRADE_RETMSG_ORDERTOEXTEND		// 여권 연장
};

enum ACUTRADE_TAB
{
	ACUTRADE_TAB_SELL		= 0,	// 팔기창
	ACUTRADE_TAB_BUY,				// 사기창
	ACUTRADE_TAB_WITHDRAW,			// 회수창
	ACUTRADE_TAB_PASSPORT,			// 여권창

	//----------------------------------------
	// 이 위로 추가해 주세요
	//----------------------------------------
	ACUTRADE_TAB_MAX
};

class CNAcuTradeDlg_Parent : public CDialogController
{
	// 변수영역
private:
	InterfaceFactory	m_InterfaceFactory;
	CDialogController*	m_pTabDialog[ACUTRADE_TAB_MAX];


	// 함수영역
public:
	CNAcuTradeDlg_Parent();
	~CNAcuTradeDlg_Parent();

	void Create();

	static void CALLBACK StaticAcuTradeDlg_ParentProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAcuTradeDlg_ParentProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void Show();
	void Hide();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	// -1값(음수)이 들어온다면 아큐시세 정보는 업데이트 안한다
	void Refresh( GMONEY gmExchangeRate = (-1) );

	void SetWithdrawDialogMemberData( GMONEY gmSavedAcu, cltTradeAcuList* pclAcuList=NULL );
	

private:
	void SelectTabDialog( SI32 siControllID );
	void SetTabDialogButton( CButton* pclButton, COLORREF FontColor, SI16 siWeight );
	
	void Request_WorldMoney_TradeList( void );

	// -1값(음수)이 들어온다면 아큐시세 정보는 업데이트 안한다
	void RefreshAcuInfo( GMONEY gmExchangeRate = (-1) );

	void RefreshSell( void );
	void RefreshBuy( void );
	void RefreshWithdraw( void );
	void RefreshPassport( void );

	void SendForRefresh( void );


};
