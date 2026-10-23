#include "NExchangeStock.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"
#include "../../CommonLogic/Stock/Stock.h"

#include "../../../CommonLogic/MsgType-Stock.h"

extern cltCommonLogic* pclClient;

CNExchangStockDlg::CNExchangStockDlg()
{	
	m_siVillageUnique = 0;
}

CNExchangStockDlg::~CNExchangStockDlg()
{
}

void CNExchangStockDlg::Create()
{
	if ( IsCreate() )
		return;


	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NSystemRewardList/DLG_ExchangeStock.ddf"));
	file.CreatePopupDialog( this, NEXCHANGESTOCK_DLG , TEXT("dialog_exchangestock"), StaticExchangStockDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_SECONDPASSWORD , this ), NEXCHANGSTOCK_STATIC_SECONDPASSWORD,   "statictext_secondpassword");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_BANK , this ), NEXCHANGSTOCK_STATIC_BANK,   "statictext_bank");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_BANKACCOUNT , this ), NEXCHANGSTOCK_STATIC_BANKACCOUNT,   "statictext_bankaccount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_VILLAGE , this ), NEXCHANGSTOCK_STATIC_VILLAGE,   "statictext_village");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_STOCKAMOUNT , this ), NEXCHANGSTOCK_STATIC_STOCKAMOUNT,   "statictext_stockamount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEXCHANGSTOCK_STATIC_SELLAMOUNT , this ), NEXCHANGSTOCK_STATIC_SELLAMOUNT,   "statictext_sellamount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEXCHANGSTOCK_EDIT_SECONDPASSWORD , this ), NEXCHANGSTOCK_EDIT_SECONDPASSWORD,   "editbox_secondpassword");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEXCHANGSTOCK_EDIT_BANKACCOUNT , this ), NEXCHANGSTOCK_EDIT_BANKACCOUNT,   "editbox_bankaccount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEXCHANGSTOCK_EDIT_STOCKAMOUNT , this ), NEXCHANGSTOCK_EDIT_STOCKAMOUNT,   "editbox_stockamount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT , this ), NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT,   "combobox_sellamount");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEXCHANGSTOCK_COMBOBOX_BANK , this ), NEXCHANGSTOCK_COMBOBOX_BANK,   "combobox_bank");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEXCHANGSTOCK_COMBOBOX_VILLAGE , this ), NEXCHANGSTOCK_COMBOBOX_VILLAGE,   "combobox_village");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEXCHANGSTOCK_BUTTON_EXCHANGE , this ), NEXCHANGSTOCK_BUTTON_EXCHANGE,   "button_exchange");

	m_InterfaceFactory.GetEdit( NEXCHANGSTOCK_EDIT_SECONDPASSWORD )->SetPasswordMode( true );
	m_InterfaceFactory.GetEdit( NEXCHANGSTOCK_EDIT_BANKACCOUNT )->SetNumberMode( true );

	stComboBoxItemData comboBoxData;
	comboBoxData.Init();
	comboBoxData.Set( GetTxtFromMgr( 10288 ) );
	m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_BANK )->AddItem( &comboBoxData );
	m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_BANK )->SetCursel( 0 );

	if( true == pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.HaveStock() )
	{
		TCHAR	stockAmount[64] = {'\0'};
		for( SI16 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			SI32 villageunique  = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
			
			if(villageunique)
			{
				comboBoxData.Init();
				comboBoxData.Set( pclClient->pclVillageManager->GetName(villageunique) );
				m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_VILLAGE )->AddItem( &comboBoxData );

				if( 0 == i )
				{
					SI32 amount = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount;
					StringCchPrintf( stockAmount, 64, "%d", amount );

					m_InterfaceFactory.GetEdit( NEXCHANGSTOCK_EDIT_STOCKAMOUNT )->SetText( stockAmount );
				}
			}
		}

		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_VILLAGE )->SetCursel( 0 );

		comboBoxData.Init();
		comboBoxData.Set( "100" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "200" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "300" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "400" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "500" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "600" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "700" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "800" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "900" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );
		comboBoxData.Set( "1000" );
		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->AddItem( &comboBoxData );

		m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->SetCursel( 0 );
	}
}

void CALLBACK CNExchangStockDlg::StaticExchangStockDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNExchangStockDlg *pThis = (CNExchangStockDlg*) pControllerMgr;
	pThis->NSystemExchangStockDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNExchangStockDlg::NSystemExchangStockDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NEXCHANGSTOCK_COMBOBOX_VILLAGE:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI16 selectedIndex = m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_VILLAGE )->GetSelectedIndex();

					if( 0 > selectedIndex || MAX_STOCK_KIND_IN_ACOUNT <= selectedIndex )
						break;

                    m_siVillageUnique  = pclClient->pclVillageManager->GetMapIndexFromName( m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_VILLAGE )->GetText( selectedIndex ) );

					if( m_siVillageUnique )
					{
						for( SI16 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
						{
							SI32 villageunique  = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;

							if( m_siVillageUnique == villageunique )
							{

								TCHAR	stockAmount[64] = {'\0'};
								SI32 amount = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount;
								StringCchPrintf( stockAmount, 64, "%d", amount );

								m_InterfaceFactory.GetEdit( NEXCHANGSTOCK_EDIT_STOCKAMOUNT )->SetText( stockAmount );
								break;
							}
						}
					}
				}
				break;
			}
		}
		break;

	case NEXCHANGSTOCK_BUTTON_EXCHANGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI16 selectedIndex = m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->GetSelectedIndex();

					if( 0 > selectedIndex || MAX_STOCK_KIND_IN_ACOUNT <= selectedIndex )
						break;

					SI32 sellAmount =  atoi( m_InterfaceFactory.GetComboBox( NEXCHANGSTOCK_COMBOBOX_SELLAMOUNT )->GetText( selectedIndex ) );

					if( 0 > sellAmount )
					{
						/// 0주는 팔 수 없다
						return;
					}

					SI32 myStockAmount = 0;
					for( SI16 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
					{
						SI32 villageunique  = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;

						if( m_siVillageUnique == villageunique )
						{
							myStockAmount = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount;
							break;
						}
					}

					if( sellAmount > myStockAmount )
					{
						/// 내가 소지한 주식보다 많이 팔 수 없다
						return;
					}
					

					cltGameMsgRequest_ExchangeStock clExchangeStock( m_siVillageUnique,
																	 sellAmount,	
						                                             0,
																	 m_InterfaceFactory.GetEdit(NEXCHANGSTOCK_EDIT_BANKACCOUNT)->GetText(),
																	 m_InterfaceFactory.GetEdit(NEXCHANGSTOCK_EDIT_SECONDPASSWORD)->GetText());
					cltMsg clMsg( GAMEMSG_REQUEST_EXCHANGESTOCK, sizeof(clExchangeStock), (BYTE*)&clExchangeStock );
					pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
				}
				break;
			}
		}
		break;
	}
}

void CNExchangStockDlg::SetStockAmount( SI32 LeftAmount )
{
	TCHAR	stockAmount[64] = {'\0'};
	StringCchPrintf( stockAmount, 64, "%d", LeftAmount );
	m_InterfaceFactory.GetEdit( NEXCHANGSTOCK_EDIT_STOCKAMOUNT )->SetText( stockAmount );
}

