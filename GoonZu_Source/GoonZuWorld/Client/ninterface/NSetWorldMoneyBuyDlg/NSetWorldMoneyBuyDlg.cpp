//-----------------------------------
// 2005/11/21 SJY
//-----------------------------------

#include "NSetWorldMoneyBuyDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char/CharManager/CharManager.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNSetWorldMoneyBuyDlg::CNSetWorldMoneyBuyDlg()
{
	m_bShow = false;
	m_pStatic_NONAME1	 = NULL; 
	m_pButton_buy	 = NULL; 
	m_pButton_reject	 = NULL; 
	m_pEdit_explain	 = NULL; 
	m_pEdit_num	 = NULL; 
}

CNSetWorldMoneyBuyDlg::~CNSetWorldMoneyBuyDlg()
{
	if( 	m_pStatic_NONAME1	 )  	delete 	m_pStatic_NONAME1	;
	if( 	m_pButton_buy	 )  	delete 	m_pButton_buy	;
	if( 	m_pButton_reject	 )  	delete 	m_pButton_reject	;
	if( 	m_pEdit_explain	 )  	delete 	m_pEdit_explain	;
	if( 	m_pEdit_num	 )  	delete 	m_pEdit_num	;
}

void CNSetWorldMoneyBuyDlg::Initialize()
{
	m_siAvailableBuyingWorldMoney = 0;
	m_siWorldMoneyPrice = 0;
}

void CNSetWorldMoneyBuyDlg::Create( )
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile( TEXT("NInterface/Data/NSetWorldMoneyBuy/DLG_NSetWorldMoneyBuy.ddf") );

		file.CreatePopupDialog( this, NSETWORLDMONEYBUT_DLG, TEXT("dialog_worldmoney_distribution"), NSetWorldMoneyBuyStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_worldmoney_distribution") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pStatic_NONAME1	 = new 	CStatic	( this )	;
		m_pButton_buy	 = new 	CButton	( this )	;
		m_pButton_reject	 = new 	CButton	( this )	;
		m_pEdit_explain	 = new 	CEdit	( this )	;
		m_pEdit_num	 = new 	CEdit	( this )	;

		file.CreateControl( 	m_pStatic_NONAME1	, 	NWORLDMONEY_DISTRIBUTION_STATICTEXT_NONAME1	, TEXT("statictext_NONAME1") )	;
		file.CreateControl( 	m_pButton_buy	, 	NWORLDMONEY_DISTRIBUTION_BUTTON_BUY	, TEXT("button_buy") )	;
		file.CreateControl( 	m_pButton_reject	, 	NWORLDMONEY_DISTRIBUTION_BUTTON_REJECT	, TEXT("button_reject") )	;
		file.CreateControl( 	m_pEdit_explain	, 	NWORLDMONEY_DISTRIBUTION_EDITBOX_EXPLAIN	, TEXT("editbox_explain") )	;
		file.CreateControl( 	m_pEdit_num	, 	NWORLDMONEY_DISTRIBUTION_EDITBOX_NUM	, TEXT("editbox_num") )	;

		m_pEdit_num->SetNumberMode( true );

	}
	else
	{
		DeleteDialog();
	}
}

void  CALLBACK CNSetWorldMoneyBuyDlg::NSetWorldMoneyBuyStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNSetWorldMoneyBuyDlg *pThis = (CNSetWorldMoneyBuyDlg*) pControllerMgr;
	pThis->NSetWorldMoneyBuyProc( nEvent, nControlID, pControl );
}

void  CALLBACK CNSetWorldMoneyBuyDlg::NSetWorldMoneyBuyProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case NWORLDMONEY_DISTRIBUTION_BUTTON_REJECT:
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NWORLDMONEY_DISTRIBUTION_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 수량 확인
					SI64 BuyingWorldMoney =_atoi64( m_pEdit_num->GetText() );
					if( BuyingWorldMoney <= 0 ) return;

					if ( BuyingWorldMoney < 1 || BuyingWorldMoney > m_siAvailableBuyingWorldMoney )
					{
						TCHAR* pTitle = GetTxtFromMgr(2462);
						TCHAR* pText = GetTxtFromMgr(2463);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}

					// 돈 확인
					if ( BuyingWorldMoney * m_siWorldMoneyPrice > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
					{
						TCHAR* pTitle = GetTxtFromMgr(1260);
						TCHAR* pText = GetTxtFromMgr(2464);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}

					cltGameMsgRequest_Distrubution_WorldMoney clinfo(BuyingWorldMoney,m_siWorldMoneyPrice);
					cltMsg clMsg( GAMEMSG_REQUEST_DISTRIBUTION_WORLDMONEY, sizeof(clinfo), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();

				}
				break;
			}
		}
		break;
	}

	return;

}

bool CNSetWorldMoneyBuyDlg::IsShow()
{
	return m_bShow;
}

void CNSetWorldMoneyBuyDlg::Show()
{
	m_bShow = true;

	CDialogController::Show( true );

	return;
}

void CNSetWorldMoneyBuyDlg::Hide()
{
	m_bShow = false;

	CDialogController::Show( false );
	Initialize();

	return;
}

void CNSetWorldMoneyBuyDlg::Set( SI64 WorldMoney, SI64 StockPrice,SI64 TotalWorldMoney)
{
	m_siAvailableBuyingWorldMoney = WorldMoney;
	m_siWorldMoneyPrice = StockPrice;

	TCHAR Buffer[ 1024 ] = TEXT("");

	char* pText = GetTxtFromMgr(6150);
	sprintf( Buffer, pText, m_siWorldMoneyPrice, TotalWorldMoney );
	m_pEdit_explain->SetText( Buffer );

	if( WorldMoney < 0 )
		m_pEdit_num->SetText( TEXT("") );
	else
	{
		TCHAR _buffer[15];
		memset( _buffer, 0, sizeof(_buffer));
		StringCchPrintf(_buffer, 15, TEXT("%I64d"), WorldMoney );
		m_pEdit_num->SetText(_buffer );
	}

	Show();

	return;
}

