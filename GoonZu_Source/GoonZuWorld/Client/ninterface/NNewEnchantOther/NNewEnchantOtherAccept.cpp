#include "../../../Common/PrivateTradeOrderMgr/PrivateTradeOrderBaseInfo.h"

#include "NNewEnchantOtherAccept.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "Client.h"
#include "../../CommonLogic/Msg/MsgType-PrivateTrade.h"

extern cltCommonLogic* pclClient;


CNNewEnchantOthereAccept::CNNewEnchantOthereAccept()
{	
	m_pEdit = NULL;
	m_pOKButton = NULL;
	m_pCancelButton = NULL;

}

CNNewEnchantOthereAccept::~CNNewEnchantOthereAccept()
{	


}


void CNNewEnchantOthereAccept::Create()
{
	if ( ! IsCreate() )
	{

		CDialogController::Create( NCOMMONMSG_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNNewEnchantOthereAcceptProc, NULL, TEXT("NInterface/Interface/Interface_20x20_00_109.Spr"), 9, true, 9, 430, 340, 280, 120, true, false, true );

		SetTopMostWindow( true );
		SetModalDialog( true );
		Show(FALSE);
	}
}

void CNNewEnchantOthereAccept::Action()
{


}

void CALLBACK CNNewEnchantOthereAccept::StatiCNNewEnchantOthereAcceptProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewEnchantOthereAccept *pThis = (CNNewEnchantOthereAccept*) pControllerMgr;
	pThis->NPrivateTradeAcceptDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNNewEnchantOthereAccept::NPrivateTradeAcceptDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

	return;
}

void CNNewEnchantOthereAccept::RetMsgBox(bool rVal,SI16 MsgIndex, BYTE* pData, SI16 DataLen) //[영진]
{

	cltClient *pclclient = (cltClient *)pclClient;
	cltGameMsgResponse_PrivateTrade_Accept* acData = (cltGameMsgResponse_PrivateTrade_Accept*)pData;

	if ( rVal )
	{// 거래 요청 수락
		cltGameMsgRequest_PrivateTrade_Accept clinfo(acData->siOtherCharUnique,PRIVATE_ENCHANT_MODE_AGREE_ACCEPT );

		cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);	// 실패 메시지 전달

		DeleteDialog();		
	}
	else
	{		
		cltGameMsgRequest_PrivateTrade_Accept clinfo(acData->siOtherCharUnique,PRIVATE_ENCHANT_MODE_REJECT_ACCEPT );

		cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);	// 실패 메시지 전달

		DeleteDialog();
	}

	return;
}
