/*====================================

클래스 : CNAuctionEventDlg
작성일 : 07 .08 . 22
작성자: 손성웅
변경사항:
차후작업:
=====================================*/

//KHY - 0822 - 해피빈 퀘스트 추가.	

#include "NAuctionEventDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "Char/CharManager/CharManager.h"

CNAuctionEventDlg :: CNAuctionEventDlg()
{
    Initialize();
}

CNAuctionEventDlg ::~CNAuctionEventDlg(){
	Destroy();
}

void CNAuctionEventDlg::init(){

}


void CNAuctionEventDlg :: Destroy(){

}
void CNAuctionEventDlg ::Create(){


	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NAuctionEventDlg/DLG_AuctionEventDlg.ddf"));

	file.CreatePopupDialog( this, NAUCTIONEVENT_DLG , TEXT("dialog_AuctionEvent"), StaticSelectAuctionEventProc );

	file.CreateControl( m_InterfaceFactory1.CreateUserInterface( IMAGESTATIC,NAUCTIONEVENT_IMAGESTATIC_CUPON_0 , this )
		,NAUCTIONEVENT_IMAGESTATIC_CUPON_0,"imagestatic_AUCTIONC");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,TEXTSTATIC_CONGRA , this ),TEXTSTATIC_CONGRA,"statictext_congra");
	
	CImageStatic* pImageStatic_Coupon = (CImageStatic*)GetControl(NAUCTIONEVENT_IMAGESTATIC_CUPON_0);

	if(pImageStatic_Coupon)		pImageStatic_Coupon->Show( false );

	Show(true);
}

void CALLBACK CNAuctionEventDlg ::StaticSelectAuctionEventProc(CControllerMgr *pControllerMgr,UINT nEvent, int nControlID,CControllerObj *pControl)
{
	CNAuctionEventDlg *pThis = (CNAuctionEventDlg*)pControllerMgr;
	pThis->NSelectAuctionEventProc(nEvent, nControlID, pControl);
}


void CALLBACK CNAuctionEventDlg ::NSelectAuctionEventProc(UINT nEvent, int nControlID,CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NSELECTAUCTIONEVENT_BUTTON_ACCEPT:
		{
			DeleteDialog();
		}		
	}
}

void CNAuctionEventDlg ::SelectAuctionEvent( )
{
	/*cltGameMsgRequest_AuctionEventQuestStart clInfo( QUEST_TYPE_AuctionEvent, QUEST_AuctionEvent_STEP_START );
	cltMsg clMsg( GAMEMSG_REQUEST_AuctionEventQUEST_START, sizeof( clInfo ), (BYTE*)&clInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);*/


}

void CNAuctionEventDlg::SetCoupon( SI32 siCoupon )
{
	CImageStatic* pImageStatic_Coupon = (CImageStatic*)GetControl(NAUCTIONEVENT_IMAGESTATIC_CUPON_0);
	if(pImageStatic_Coupon == NULL)					return;

	CStatic* pStatic_Congra = (CStatic*)GetControl(TEXTSTATIC_CONGRA);
	if(pStatic_Congra == NULL)						return;

	TCHAR* pTitle = NULL;
	TCHAR* pText = NULL;

	switch(siCoupon)
	{
	case AUCTION_EVENT_COUPON_1000WON:		pImageStatic_Coupon->SetFontIndex( 4 );		pText = GetTxtFromMgr(7566);	pStatic_Congra->SetText(pText);		break;
	case AUCTION_EVENT_COUPON_3000WON:		pImageStatic_Coupon->SetFontIndex( 9 );		pText = GetTxtFromMgr(7567);	pStatic_Congra->SetText(pText);     break;
	case AUCTION_EVENT_COUPON_5000WON:		pImageStatic_Coupon->SetFontIndex( 6 );		pText = GetTxtFromMgr(7568);	pStatic_Congra->SetText(pText);     break;
	case AUCTION_EVENT_COUPON_50POINT:		pImageStatic_Coupon->SetFontIndex( 0 );		pText = GetTxtFromMgr(7569);	pStatic_Congra->SetText(pText);     break;
	case AUCTION_EVENT_COUPON_100POINT:		pImageStatic_Coupon->SetFontIndex( 1 );		pText = GetTxtFromMgr(7570);	pStatic_Congra->SetText(pText);     break;
	case AUCTION_EVENT_COUPON_500POINT:		pImageStatic_Coupon->SetFontIndex( 2 );		pText = GetTxtFromMgr(7571);	pStatic_Congra->SetText(pText);     break;
	default:																			break;
	}
	pImageStatic_Coupon->Show( true );
}


//void CNAuctionEventDlg :: Setstatic (SI32 sicoupon)
//{
//	CImageStatic* textstaticcongra = (CImageStatic*)GetControl(TEXTSTATIC_CONGRA);
//	if(textstaticcongra == NULL)					return;
//
//	switch(siCoupon)
//	{
//	case AUCTION_EVENT_COUPON_1000WON:		textstaticcongra->SetSetFontIndex( 4 );		break;
//	case AUCTION_EVENT_COUPON_3000WON:		textstaticcongra->SetFontIndex( 9 );		break;
//	case AUCTION_EVENT_COUPON_5000WON:		textstaticcongra->SetFontIndex( 6 );		break;
//	case AUCTION_EVENT_COUPON_50POINT:		textstaticcongra->SetFontIndex( 0 );		break;
//	case AUCTION_EVENT_COUPON_100POINT:		textstaticcongra->SetFontIndex( 1 );		break;
//	case AUCTION_EVENT_COUPON_500POINT:		textstaticcongra->SetFontIndex( 2 );		break;
//	default:																			break;
//	}
//	pImageStatic_Coupon->Show( true );
//}
