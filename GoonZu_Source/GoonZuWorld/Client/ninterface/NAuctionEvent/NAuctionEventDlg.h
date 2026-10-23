/*====================================

클래스 : CNAuctionEventDlg
작성일 : 07 .08 . 22
작성자: 손성웅
변경사항:
차후작업:
=====================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

enum
{
	AUCTION_EVENT_COUPON_1000WON,
	AUCTION_EVENT_COUPON_3000WON,
	AUCTION_EVENT_COUPON_5000WON,
	AUCTION_EVENT_COUPON_50POINT,
	AUCTION_EVENT_COUPON_100POINT,
	AUCTION_EVENT_COUPON_500POINT,
};

class CNAuctionEventDlg : public CDialogController
{
public:
	CNAuctionEventDlg();
	~CNAuctionEventDlg();

	void init();
	void Destroy();

	void Create();

	static void CALLBACK StaticSelectAuctionEventProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSelectAuctionEventProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;
	InterfaceFactory	m_InterfaceFactory1;


	void SelectAuctionEvent( );

	void SetCoupon( SI32 siCoupon );
	//void Setstatic( SI32 siCoupon );

};