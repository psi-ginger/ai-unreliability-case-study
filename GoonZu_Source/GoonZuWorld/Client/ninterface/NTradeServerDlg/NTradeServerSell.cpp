#include "../../NInterface/NTradeServerDlg/NTradeServerDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../../../CommonLogic/TextMgr/TextMgr.h"

#include "../../NStructureDlg/StrBaseDlg/StrInventory.h"

#include "Char/CharManager/CharManager.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/Msg/MsgType-WorldMoney.h"

extern cltCommonLogic* pclClient;

CNTradeServerSellDlg::CNTradeServerSellDlg() : CDialogController(), m_pMyInventory(NULL), m_pStgItem(NULL), m_bMyInven(false)
{
}

CNTradeServerSellDlg::~CNTradeServerSellDlg()
{
	SAFE_DELETE( m_pMyInventory  );
	SAFE_DELETE( m_pStgItem );
}

void CNTradeServerSellDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile( TEXT("NInterface/Data/NTradeServerDlg/DLG_TradeSell.ddf") );
	file.CreateChildDialog( this, NTRADESERVERSELL_DLG, TEXT("dialog_tradesell"), StaticCallBackDialogNTradeServerSellDlg, pParent );

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC1, this ),			NTRADESERVERSELL_STATIC1,				 TEXT("statictext_NONAME1")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC2, this ),			NTRADESERVERSELL_STATIC2,				 TEXT("statictext_NONAME2")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC3, this ),			NTRADESERVERSELL_STATIC3,				 TEXT("statictext_NONAME3")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC4, this ),			NTRADESERVERSELL_STATIC4,				 TEXT("statictext_NONAME4")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC5, this ),			NTRADESERVERSELL_STATIC5,				 TEXT("statictext_NONAME5")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC6, this ),			NTRADESERVERSELL_STATIC6,				 TEXT("statictext_NONAME6")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC7, this ),			NTRADESERVERSELL_STATIC7,				 TEXT("statictext_ranktype"));
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERSELL_STATIC10, this ),			NTRADESERVERSELL_STATIC10,				 TEXT("statictext_NONAME10")); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERSELL_EDIT_FEERATE, this ),	    NTRADESERVERSELL_EDIT_FEERATE,	 TEXT("editbox_feerate")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERSELL_EDIT_FEE, this ),			NTRADESERVERSELL_EDIT_FEE,	 TEXT("editbox_fee")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERSELL_EDIT_AMOUNT, this ),	    NTRADESERVERSELL_EDIT_AMOUNT,	 TEXT("editbox_amount")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERSELL_EDIT_PRICE, this ),	    NTRADESERVERSELL_EDIT_PRICE,	 TEXT("editbox_price")); 
	
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERSELL_BUTTON_SELL, this ),			NTRADESERVERSELL_BUTTON_SELL, TEXT("button_sell")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERSELL_BUTTON_CANCEL_ORDER, this ),  NTRADESERVERSELL_BUTTON_CANCEL_ORDER,    TEXT("button_cancel_order")); 
	
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( LISTVIEW,    NTRADESERVERSELL_LISTVIEW_MYINVENTORY, this ),      NTRADESERVERSELL_LISTVIEW_MYINVENTORY,   TEXT("listview_myinventory")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( LISTVIEW,    NTRADESERVERSELL_LISTVIEW_MARKETSTORAGE, this ),    NTRADESERVERSELL_LISTVIEW_MARKETSTORAGE, TEXT("listview_marketstorage")); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, NTRADESERVERSELL_OUTLINE, this ),         NTRADESERVERSELL_OUTLINE,         TEXT("outline_NONAME1")); 

	m_pMyInventory = new NStrInventory;
	m_pStgItem = new NStrInventory;

	CListView* MyListView = m_InterfaceFactory_.GetListView( NTRADESERVERSELL_LISTVIEW_MYINVENTORY );
	MyListView->SetBorder( true );
	MyListView->SetBKColorUse( true );

	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, MyListView );
	m_pMyInventory->LoadMyInventory_InternationalTrade();

	CListView* StgItemListView = m_InterfaceFactory_.GetListView( NTRADESERVERSELL_LISTVIEW_MARKETSTORAGE );
	StgItemListView->SetBorder( true );
	StgItemListView->SetBKColorUse( true );

	SI16 i = 0 ;
	cltItem tempItem;//[MAX_TRADESERVER_ORDER];
	//for( i = 0; i < MAX_TRADESERVER_ORDER; ++i )
	{
		if( pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siUnique > 0 )
		{
			tempItem.Init();
			tempItem.siUnique = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siUnique;
			tempItem.siItemNum = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siItemNum;
			tempItem.Element.siPower = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siPower;
			tempItem.Element.siType  = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siType;
			memcpy(tempItem.cData8,pclClient->pclCM->CR[1]->pclCI->clWorldOrder.szData,MAX_ITEM_UNION_BYTE);
		}
	}

	m_pStgItem->CreateInventory(  MAX_TRADESERVER_ORDER, StgItemListView, &tempItem );
 	m_pStgItem->LoadInventory( NULL, FALSE );

	m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_FEERATE)->SetText(TEXT("1%"));

	MovePos( 0 , 130 );
	Hide();
}

void CNTradeServerSellDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNTradeServerSellDlg::Show()
{
	CControllerMgr::Show( true );

}

void CNTradeServerSellDlg::Action()
{
	if( m_pMyInventory->IsMyInventoryChanged() ) 
	{
		m_pMyInventory->LoadMyInventory_InternationalTrade();
	}

	SI16 i = 0 ;
	cltItem tempItem;//[MAX_TRADESERVER_ORDER];
	//for( i = 0; i < MAX_TRADESERVER_ORDER; ++i )
	{
		if( pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siUnique > 0 )
		{
			tempItem.Init();
			tempItem.siUnique = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siUnique;
			tempItem.siItemNum = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siItemNum;
			tempItem.Element.siPower = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siPower;
			tempItem.Element.siType  = pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siType;
			memcpy(tempItem.cData8,pclClient->pclCM->CR[1]->pclCI->clWorldOrder.szData,MAX_ITEM_UNION_BYTE);
		}
	}

	if( m_pStgItem->IsInventoryChanged(&tempItem) )
	{
		m_pStgItem->LoadInventory( &tempItem, TRUE );
	}

	SI32 itemnum = _tstoi(m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_AMOUNT)->GetText());
	SI64 price = _tstoi64(m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_PRICE)->GetText());

	SI64 fee = price / 100 ;
	if ( fee <= 0 )
		fee = 1 ;
	TCHAR feebuf[128] = TEXT("") ;
	g_JYLibFunction.SetNumUnit(fee, feebuf, 128);
	m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_FEE)->SetText(feebuf);
}

void CALLBACK CNTradeServerSellDlg::StaticCallBackDialogNTradeServerSellDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeServerSellDlg *pThis = (CNTradeServerSellDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeServerSellDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeServerSellDlg::CallBackDialogNTradeServerSellDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADESERVERSELL_LISTVIEW_MYINVENTORY:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					m_pMyInventory->SelectAction();

					if( m_pMyInventory->m_siSelectedIndex < PERSONITEM_INV0)
						break;
					if( m_pMyInventory->m_siSelectedIndex >= PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
						break;

					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_pMyInventory->m_clSelectedItem.siUnique);
					if ( ref <= 0 || ref >= MAX_ITEMINFO_NUMBER )
						break;

					SI32 itemnum = m_pMyInventory->m_clSelectedItem.GetItemNum() ;
					char buffer[64] = "" ;
					sprintf(buffer,"%d",itemnum);
					m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_AMOUNT)->SetText(buffer);
				}
				break;
			}
		}
		break;

	case NTRADESERVERSELL_LISTVIEW_MARKETSTORAGE:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					m_pStgItem->SelectAction();
				}
				break;
			}
		}
		break;

	case NTRADESERVERSELL_BUTTON_SELL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 물건 판매 신청
					if( m_pMyInventory->m_siSelectedIndex < PERSONITEM_INV0)
						break;
					if( m_pMyInventory->m_siSelectedIndex >= PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
						break;
					SI32 itemunique = m_pMyInventory->m_clSelectedItem.siUnique;

					// 개편된 무역에서는 여권이 꼭 있어야 한다
					if ( true == pclClient->IsCountrySwitch(Switch_Trade_Renewal) )
					{
						if ( false == pclClient->pclCM->CR[1]->pclCI->clWorldPassport.IsHavePassport() )
						{
							TCHAR *pTitle	= GetTxtFromMgr(8836);
							TCHAR *pText	= GetTxtFromMgr(8837);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
							break;
						}
					}

					// 다른 국가의 특산품일 경우 팔수 없다.
					if ( pclClient->siServiceArea == ConstServiceArea_English || pclClient->siServiceArea == ConstServiceArea_NHNChina)
					{
						if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_KOREA) ||
							pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_JAPAN) )
						{
							if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
							{
								TCHAR *pTitle = GetTxtFromMgr(6224);
								TCHAR *pText = GetTxtFromMgr(6642);

								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
								break;
							}
						}
					}
					else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
					{
						if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_KOREA) ||
							pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_GLOBAL) )
						{
							if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_JAPAN) == false )
							{
								TCHAR *pTitle = GetTxtFromMgr(6224);
								TCHAR *pText = GetTxtFromMgr(6642);

								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
								break;
							}
						}
					}
					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_pMyInventory->m_clSelectedItem.siUnique);
					if ( ref <= 0 || ref >= MAX_ITEMINFO_NUMBER )
						break;
					SI32 itemminnum = pclClient->pclItemManager->pclItemInfo[ref]->siTradeServerMinNum ;
					// 무조건 판매가능한 아이템은 위의 itemnum이 0을 초과해야 한다.
					if ( itemminnum <= 0 )
					{
						TCHAR *pTitle = GetTxtFromMgr(6224);
						TCHAR *pText = GetTxtFromMgr(6225);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						break;
					}

					if( itemminnum > m_pMyInventory->m_clSelectedItem.GetItemNum() )
					{
						TCHAR buffer[256] = TEXT("");
						TCHAR *pText = GetTxtFromMgr(6165);
						StringCchPrintf( buffer, 256, pText, itemminnum );

						pText = GetTxtFromMgr(6166);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer);
						
						break;
					}

					SI32 itemnum = _ttoi(m_InterfaceFactory_.GetEdit(NTRADESERVERSELL_EDIT_AMOUNT)->GetText());
					if ( itemnum <= 0 || itemnum < itemminnum )
					{
						TCHAR buffer[256] = ("");
						TCHAR *pText = GetTxtFromMgr(6165);
						StringCchPrintf( buffer, 256, pText, itemminnum);
						
						pText = GetTxtFromMgr(6166);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer);
						
						break;
					}

					SI32 itemtype = pclclient->pclItemManager->pclItemInfo[ref]->siType ;
					if ( pclclient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
					{
						itemnum = 1 ;
					}

					if ( itemnum > pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_pMyInventory->m_siSelectedIndex].GetItemNum() )
					{
						TCHAR *pTitle = GetTxtFromMgr(6166);
						TCHAR *pText = GetTxtFromMgr(6167);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						break;
					}

					SI64 TradeAq = 0;
					TradeAq = _tstoi( m_InterfaceFactory_.GetEdit( NTRADESERVERSELL_EDIT_PRICE )->GetText() );
					if( TradeAq <=0 )
					{
						TCHAR *pTitle = GetTxtFromMgr(6166);
						TCHAR *pText = GetTxtFromMgr(6168);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					SI64 fee = TradeAq / 100 ;
					if ( fee <= 0 )
						fee = 1 ;
					if( fee > pclClient->pclCM->CR[1]->pclCI->clWorldMoney.siWorldMoney )
					{
						TCHAR buffer[256] = TEXT("");
						TCHAR *pText = GetTxtFromMgr(6169);
						StringCchPrintf( buffer, 256, pText, fee );
						pText = GetTxtFromMgr(6177);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pText, buffer);
						break;
					}

					TCHAR buf[256] = TEXT("");

					TCHAR itemname[20] = "" ;
					if ( m_pMyInventory->m_clSelectedItem.GetFullName(pclClient->pclItemManager,itemname, 20) )
					{
						TCHAR *pText = GetTxtFromMgr(6170);

						StringCchPrintf( buf, 256, pText, (char*)m_pMyInventory->m_clSelectedItem.GetShortName(pclClient->pclItemManager), itemnum, TradeAq, fee );


						stMsgBox MsgBox;
						CControllerMgr *pParent = NULL;
						pParent = this->GetParentDialog();
						if(pParent == NULL ) break;

						pText = GetTxtFromMgr(6178);
						MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 0 );


						TCHAR TempBuffer[ 256 ] = TEXT("");
						SI16 Size = sizeof( itemnum ) + sizeof( TradeAq );
						memcpy( TempBuffer, &itemnum, sizeof( itemnum ) );
						memcpy( &TempBuffer[ sizeof(itemnum) ], &TradeAq, sizeof( TradeAq ) );
						pclclient->SetMsgBox( &MsgBox, (BYTE*)TempBuffer, Size );
					}

				}
				break;
			}
		}
		break;
	case NTRADESERVERSELL_BUTTON_CANCEL_ORDER:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_pStgItem->CheckCurrentSelect() == FALSE ) 
						return;

					TCHAR buf[256] = TEXT("");
					TCHAR *pText = GetTxtFromMgr(6256);
					StringCchPrintf( buf, 256, pText, (TCHAR*)m_pStgItem->m_clSelectedItem.GetShortName(pclClient->pclItemManager));

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) break;

                    pText = GetTxtFromMgr(6171);
					MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;
	}
}

void CNTradeServerSellDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if( RetValue )
			{
				// 물건 판매 신청
				if( m_pMyInventory->m_siSelectedIndex < PERSONITEM_INV0)
					return;
				if( m_pMyInventory->m_siSelectedIndex >= PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON)
					return;

				SI32 itemnum = *((SI32*)pData);
				SI64 TradeAq = *((SI64*)(pData + 4));

				cltItem clitem;
				clitem.Init();
				clitem.Set(&m_pMyInventory->m_clSelectedItem);
				clitem.siItemNum = itemnum ;

				cltGameMsgRequest_TradeServer_Set_SellOrder clSellOrder( &clitem, m_pMyInventory->m_siSelectedIndex, TradeAq );
				cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_SET_SELLORDER, sizeof(cltGameMsgRequest_TradeServer_Set_SellOrder), (BYTE*)&clSellOrder);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if( RetValue )
			{

				// 물건 취소 신청
				cltGameMsgRequest_TradeServer_Cancel_SellOrder clinfo( pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siUnique,
					pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siItemNum,
					pclClient->pclCM->CR[1]->pclCI->clWorldOrder.siIndex);
				cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_CANCEL_SELLORDER, 
					sizeof(cltGameMsgRequest_TradeServer_Cancel_SellOrder), (BYTE*)&clinfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

			}
		}
		break;
	}
}