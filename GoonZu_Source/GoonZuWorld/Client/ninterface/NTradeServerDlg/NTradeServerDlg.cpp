#include "../../NInterface/NTradeServerDlg/NTradeServerDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../InterfaceMgr/DialogController.h"

#include "../../Client.h"
#include "../../../CommonLogic/TextMgr/TextMgr.h"
#include "../../../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNTradeServerBaseDlg::CNTradeServerBaseDlg() : CDialogController(), m_TradeServerSell(NULL), m_TradeServerBuy(NULL)
{
}

CNTradeServerBaseDlg::~CNTradeServerBaseDlg()
{
	SetTimerOnOff( false );
	SAFE_DELETE( m_TradeServerSell );
	SAFE_DELETE( m_TradeServerBuy );
}

void CNTradeServerBaseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTradeServerDlg/DLG_TradeBase.ddf"));
		file.CreatePopupDialog( this, NTRADESERVERBASE_DLG, TEXT("dialog_tradebase"), StaticCallBackDialogNTradeServerBaseDlg );

		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( IMAGESTATIC, NTRADESERVERBASE_STATIC, this ),		 NTRADESERVERBASE_STATIC,		TEXT("imagestatic_NONAME1")); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBASE_EDIT, this ),			 NTRADESERVERBASE_EDIT,			TEXT("editbox_NONAME1")); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERBASE_BUTTON_SELL, this ) ,	 NTRADESERVERBASE_BUTTON_SELL,  TEXT("button_NONAME1")); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERBASE_BUTTON_BUY, this ),	 NTRADESERVERBASE_BUTTON_BUY,   TEXT("button_NONAME2")); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERBASE_BUTTON_CLOSE, this ),  NTRADESERVERBASE_BUTTON_CLOSE, TEXT("button_NONAME3")); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, NTRADESERVERBASE_OUTLINE, this ),      NTRADESERVERBASE_OUTLINE,      TEXT("outline_NONAME1")); 

		m_TradeServerSell = new CNTradeServerSellDlg();
		m_TradeServerSell->Create( this );
		m_TradeServerSell->Show();

		m_TradeServerBuy = new CNTradeServerBuyDlg();
		m_TradeServerBuy->Create( this );

		SetTimerOnOff( true );

		TCHAR* pText = GetTxtFromMgr(6186);
		m_InterfaceFactory_.GetEdit(NTRADESERVERBASE_EDIT)->SetText(pText);
		pText = GetTxtFromMgr(6187);
		SetTitle(pText);

		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBASE_STATIC)->SetFileName( TEXT("Interface/Portrait/PortraitIcon1.SPR"));
		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBASE_STATIC)->SetFontIndex(1);

		pclClient->pclCM->CR[1]->pclCI->clWorldOrder.Set(0,0,0,0,0,0,0,0,NULL);

		cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_GET_MYINFO,0);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

		SetAlphaMode( 15 );
		SetFadeIn( 150 );
	}
	else
	{
		DeleteDialog();
	}
}

void CNTradeServerBaseDlg::Action()
{
	if( m_TradeServerSell->IsShow() )
		m_TradeServerSell->Action();
	if( m_TradeServerBuy->IsShow() )
		m_TradeServerBuy->Action();
}

void CALLBACK CNTradeServerBaseDlg::StaticCallBackDialogNTradeServerBaseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeServerBaseDlg *pThis = (CNTradeServerBaseDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeServerBaseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeServerBaseDlg::CallBackDialogNTradeServerBaseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADESERVERBASE_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NTRADESERVERBASE_BUTTON_SELL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_TradeServerSell->Show();
					m_TradeServerBuy->Hide();
				}
				break;
			}
		}
		break;

	case NTRADESERVERBASE_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_TradeServerSell->Hide();
					m_TradeServerBuy->Show();
				}
				break;
			}
		}
		break;
	}
}

void CNTradeServerBaseDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
	case 1:
		m_TradeServerSell->RetMsgBox(RetValue,MsgIndex,pData,DataLen);
		break;
	case 2:
		m_TradeServerBuy->RetMsgBox(RetValue, MsgIndex, pData, DataLen );
		break;
	}
}

void CNTradeServerBaseDlg::ShowBuyDlg()
{	
	m_TradeServerBuy->Show();
}