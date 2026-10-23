#include "../../../Client/NInterface/NTradeServerDlg/NTradeServerDlg.h"

#include "../../../Client/InterfaceMgr/InterfaceFile.h"
#include "../../../Client/InterfaceMgr/InterfaceMgr.h"

#include "../../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"

#include "Char/CharManager/CharManager.h"

#include "../../../Client/Client.h"
#include "../../../CommonLogic/TextMgr/TextMgr.h"
#include "../../../CommonLogic/WorldTraderMgr/WorldTraderManager.h"
#include "../../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "../../../Server/NTradeServerProcess/TradeServerMsg.h"

extern cltCommonLogic* pclClient;

//SI32 Sort(cltTradeServerOrder sisortdata[], cltTradeServerOrder* pindex);
//SI32 m_siListIndex[MAX_TRADESERVER_ORDER_LIST];

CNTradeServerBuyDlg::CNTradeServerBuyDlg() : CDialogController()
{
	m_siImageType = 0;
	m_siImageIndex = 0;

//	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
//	{
//		m_siListIndex[i] = -1 ;
//	}

	siItemPrice = 0 ;
	siWorldMoneyPrice = 0 ;

	m_kUpdateTimer_ComboBoxDisable.Init( 1100 );		// 서버에서 1초로 제한하므로, 클라이언트는 100ms를 더 잡아본다.
}

CNTradeServerBuyDlg::~CNTradeServerBuyDlg()
{
}

void CNTradeServerBuyDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile( TEXT("NInterface/Data/NTradeServerDlg/DLG_TradeBuy.ddf") );
	file.CreateChildDialog( this, NTRADESERVERBUY_DLG, TEXT("dialog_tradebuy"), StaticCallBackDialogNTradeServerBuyDlg, pParent );

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERBUY_STATIC1, this ),			NTRADESERVERBUY_STATIC1,		 TEXT("statictext_NONAME1")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERBUY_STATIC2, this ),			NTRADESERVERBUY_STATIC2,		 TEXT("statictext_NONAME2")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERBUY_STATIC3, this ),			NTRADESERVERBUY_STATIC3,		 "statictext_NONAME3"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERBUY_STATIC4, this ),			NTRADESERVERBUY_STATIC4,		 "statictext_NONAME4"); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, NTRADESERVERBUY_STATIC_SHOWITEM, this ),	NTRADESERVERBUY_STATIC_SHOWITEM,		 "statictext_showitem"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( IMAGESTATIC, NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG, this ),	NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG,		 "imagestatic_NationFlag"); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBUY_EDIT_AMOUNT, this ),			NTRADESERVERBUY_EDIT_AMOUNT,		 "editbox_amount"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBUY_EDIT_EXPLAIN, this ),			NTRADESERVERBUY_EDIT_EXPLAIN,		 "editbox_explain"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBUY_EDIT_ITEMPRICE, this ),	    NTRADESERVERBUY_EDIT_ITEMPRICE,		 "editbox_itemprice"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBUY_EDIT_WORLDMONEYPRICE, this ),	NTRADESERVERBUY_EDIT_WORLDMONEYPRICE,"editbox_worldmoneyprice"); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( EDIT,   NTRADESERVERBUY_EDIT_NATIONNAME, this ),		NTRADESERVERBUY_EDIT_NATIONNAME,	 "editbox_nationname"); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERBUY_BUTTON_REFRESH, this ) ,	NTRADESERVERBUY_BUTTON_REFRESH,      TEXT("button_refresh")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, NTRADESERVERBUY_BUTTON_BUY, this ),		NTRADESERVERBUY_BUTTON_BUY, TEXT("button_buy")); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, NTRADESERVERBUY_COMBOBOX_ITEMTYPE, this ), NTRADESERVERBUY_COMBOBOX_ITEMTYPE,   TEXT("combobox_itemtype")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, NTRADESERVERBUY_COMBOBOX_ITEM, this ),     NTRADESERVERBUY_COMBOBOX_ITEM, TEXT("combobox_item")); 


	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( LIST, NTRADESERVERBUY_LIST_ORDERLIST, this ),   NTRADESERVERBUY_LIST_ORDERLIST,         TEXT("listbox_orderlist")); 

	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, NTRADESERVERBUY_OUTLINE1, this ),         NTRADESERVERBUY_OUTLINE1,         TEXT("outline_NONAME1")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, NTRADESERVERBUY_OUTLINE2, this ),         NTRADESERVERBUY_OUTLINE2,         TEXT("outline_NONAME2")); 
	file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, NTRADESERVERBUY_OUTLINE3, this ),         NTRADESERVERBUY_OUTLINE3,         "outline_NONAME3"); 

	TCHAR *pText = GetTxtFromMgr(6182);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(0, 110, pText);
	pText = GetTxtFromMgr(6183);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(1, 40, pText);
	pText = GetTxtFromMgr(6147);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(2, 40, pText);
	pText = GetTxtFromMgr(6231);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(3, 60, pText);
	pText = GetTxtFromMgr(6184);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(4, 100, pText);
	pText = GetTxtFromMgr(6232);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(5, 60, pText);

	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC1 )->SetTextPrintOption( DT_LEFT );
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC2 )->SetTextPrintOption( DT_LEFT );
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC3 )->SetTextPrintOption( DT_LEFT );
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC4 )->SetTextPrintOption( DT_LEFT );

	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC1 )->Refresh();
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC2 )->Refresh();
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC3 )->Refresh();
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC4 )->Refresh();

	//m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(6, 44, "나라");
	pText = GetTxtFromMgr(6184);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetColumn(6, 10, "");

	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetBorder(TRUE);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->SetBKColorUse(TRUE);
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->Refresh();

	MovePos( 0 , 130 );

	CComboBox *pComboItemType = m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE );
	stComboBoxItemData itemdata;

	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	int i;
	// 콤보 박스를 지운다.
	//	pComboItemType->Clear();

	// 콤보 박스에 아이템의 종류를 설정한다. 
/*	for( i = 0; i < MAX_ITEMTYPE_NUMBER; i++ )
	{	
		cltItemTypeInfo* pinfo = pclitemmanager->pclItemTypeInfo[i];
		if(pinfo == NULL)continue;
		if(pinfo->IsAtb(ITEMATB_TRADESERVER) == FALSE)continue;

		if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;

		itemdata.Init();
		itemdata.Set( (TCHAR*)pinfo->GetName() );                                   
		pComboItemType->AddItem( &itemdata );
	}*/

	for(i = 0; i<MAX_ITEMINFO_NUMBER; i++)
	{
		cltItemInfo* pclItemInfo = pclClient->pclItemManager->pclItemInfo[i];	if( pclItemInfo == NULL )	continue;
		if( pclItemInfo->siTradeServerMinNum <= 0 )					continue;

		SI32 siType = pclItemInfo->siType;		if( siType < 0 || siType >= MAX_ITEMTYPE_NUMBER )			continue;
		NTCHARString32 kTypeName = pclClient->pclItemManager->pclItemTypeInfo[siType]->GetName();

		SI32 find = pComboItemType->FindIndexFromText( kTypeName );

		if( find < 0 )
		{
			itemdata.Init();
			itemdata.Set( kTypeName );                                   
			pComboItemType->AddItem( &itemdata );
		}
	}

	// 빨리빨리 정보를 보지 못하도록 휠 기능을 끈다.
	m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE )->SetEnableWheel( false );
	m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM )->SetEnableWheel( false );

	SetActionTimer( 250 );

	SetAlphaMode( 13 );

	Hide();
}

void CNTradeServerBuyDlg::Hide()
{
	CControllerMgr::Show( false );

	SetTimerOnOff( false );
}

void CNTradeServerBuyDlg::Show()
{
	CControllerMgr::Show( true );
//	CComboBox *pComboItemType = m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE );
//	pComboItemType->SetCursel(0);
//	ShowItemImage(pComboItemType->GetText(0));

	SetTimerOnOff( true );
}

void CNTradeServerBuyDlg::Action()
{
	if( IsShow() )
	{
		if( m_kUpdateTimer_ComboBoxDisable.IsTimed( GetTickCount(), false ) )
		{
			m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE )->Enable( true );
			m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM )->Enable( true );

			m_InterfaceFactory_.GetButton( NTRADESERVERBUY_BUTTON_REFRESH )->Enable( true );
			m_InterfaceFactory_.GetButton( NTRADESERVERBUY_BUTTON_BUY )->Enable( true );
		}
	}
}

void CALLBACK CNTradeServerBuyDlg::StaticCallBackDialogNTradeServerBuyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeServerBuyDlg *pThis = (CNTradeServerBuyDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeServerBuyDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeServerBuyDlg::CallBackDialogNTradeServerBuyDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADESERVERBUY_COMBOBOX_ITEMTYPE:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					CComboBox *pComboItemType = m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE );
					CComboBox *pComboItem =     m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM );

					SI16 index = pComboItemType->GetSelectedIndex();
					if( index < 0 ) return;

					TCHAR *szSelText = pComboItemType->GetText( index );

					SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
					if(itemtypeunique >=0)
					{
						ShowItemListInCombo( pComboItem, itemtypeunique, false );
						pComboItem->SetCursel( 0 );
						RefreshOrder();
					}
				}
				break;
			}
		}
		break;

	case NTRADESERVERBUY_COMBOBOX_ITEM:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					CComboBox *pComboItem =     m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM );
					SI16 index = pComboItem->GetSelectedIndex();
					if( index < 0 ) return;
					TCHAR *szSelText = pComboItem->GetText( index );
					if ( szSelText == NULL ) break;
					SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);
					if ( itemunique <= 0 ) break;

					ShowItemImage(szSelText);

					for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
					{
						pclClient->pclWorldTraderManager->clOrder[i].Init();
					}

					cltGameMsgRequest_TradeServer_Get_ListInfo clinfo(itemunique);
					cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_GET_LISTINFO, sizeof(cltGameMsgRequest_TradeServer_Get_ListInfo), (BYTE*)&clinfo);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

					StartComboBoxDisable();
				}
				break;
			}
		}
		break;
	case NTRADESERVERBUY_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex() < 0 )
						break;

					SI32 siSelectedIndex = m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex();
					if( siSelectedIndex < 0 )			break;

//					SI32 index =  m_siListIndex[m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex()];
//					if ( index < 0 )
//						break;

					stListBoxItemData* pListBoxItemData = m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetItemData( siSelectedIndex, 0 );
					if( pListBoxItemData == NULL )		break;

					cltTradeServerOrder* pclTradeServerOrder = pclClient->pclWorldTraderManager->GetTradeServerOrder( pListBoxItemData->siParam[0] );
					if( pclTradeServerOrder == NULL )	break;

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

					SI64 worldmoney = pclTradeServerOrder->clitem.siPrice ;
					if ( pclClient->pclCM->CR[1]->pclCI->clWorldMoney.GetWorldMoney() < worldmoney )
					{
						TCHAR* pTitle = TEXT("");
						TCHAR* pText = TEXT("");

						pTitle = GetTxtFromMgr(6261);
						pText = GetTxtFromMgr(6262);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
						break;
					}

					SI32 itemunique = pclTradeServerOrder->clitem.siUnique;
					
					// 다른나라 특산품일 경우 세금 적용 유무와 상관없이 관세를 매기지 않는다.
					if ( pclClient->siServiceArea == ConstServiceArea_Japan )
					{
						if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_KOREA) ||
							pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_GLOBAL) )
						{
							if ( pclClient->pclItemManager->IsItemInfoAtb(itemunique, ITEMINFOATB_SPECIAL_JAPAN) == false )
							{
								cltGameMsgRequest_TradeServer_Set_BuyOrder clinfo( pclTradeServerOrder );
								cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER, sizeof(cltGameMsgRequest_TradeServer_Set_BuyOrder), (BYTE*)&clinfo);
								pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

								StartComboBoxDisable();
								break;
							}
						}
						
					}
					else if ( pclClient->siServiceArea == ConstServiceArea_English || pclClient->siServiceArea == ConstServiceArea_NHNChina )
					{
						if ( pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_KOREA) ||
							pclClient->pclItemManager->IsItemInfoAtb( itemunique, ITEMINFOATB_SPECIAL_JAPAN) )
						{
							if ( pclClient->pclItemManager->IsItemInfoAtb(itemunique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
							{
								cltGameMsgRequest_TradeServer_Set_BuyOrder clinfo( pclTradeServerOrder );
								cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER, sizeof(cltGameMsgRequest_TradeServer_Set_BuyOrder), (BYTE*)&clinfo);
								pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

								StartComboBoxDisable();
								break;
							}
						}
					}

					// 세금이 적용되고 있는가..
					if ( m_bWorldTradeTax )
					{
						SI64 siPrice = pclTradeServerOrder->clitem.siPrice;
						SI16 siItemNum = pclTradeServerOrder->clitem.siItemNum;
						// 주문에 올려진 아이템의 한개당 가격
						if ( 0 == siItemNum )
							break;

						GMONEY siOrderPrice = ( siPrice * siWorldMoneyPrice ) / siItemNum ;

						SI32 siRatio = 0;

						if ( 0 == siItemPrice )
							break;

						siRatio = (SI32)(siOrderPrice * 100 / siItemPrice);

						// 실제 적용될 세율을 구한다.
						SI32 siTotalSpecialTaxRate = 0;

						if ( siRatio <= m_siSpecialTaxRange )
						{
							siTotalSpecialTaxRate = m_siSpecialTaxRate + ( m_siSpecialTaxRange - siRatio );
						}
						else
						{
							siTotalSpecialTaxRate = 1;
						}

						GMONEY siTax = ( siPrice * siWorldMoneyPrice ) * siTotalSpecialTaxRate / 100;


						if ( siTax > pclclient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
						{
							TCHAR* pTitle = GetTxtFromMgr(1260);
							TCHAR* pText = GetTxtFromMgr(1677);

							pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pText );
							
							break;
						}

						TCHAR* pText;
						TCHAR buffer[128];
						if( siTotalSpecialTaxRate == 1 )
						{
							pText = GetTxtFromMgr( 6380 );
							StringCchPrintf( buffer, 128, pText, siTax );
						}
						else
						{
							pText = GetTxtFromMgr( 6381 );
							StringCchPrintf( buffer, 128, pText, siTotalSpecialTaxRate, siTax );
						}

						TCHAR* pTitle = GetTxtFromMgr( 6382 );
						stMsgBox MsgBox;

						CControllerMgr *pParent = NULL;
						pParent = this->GetParentDialog();

						MsgBox.Set( pParent, pTitle, buffer, MSGBOX_TYPE_YESNO, 2 );
						pclclient->SetMsgBox( &MsgBox, (BYTE*)&pclTradeServerOrder->siIndex, sizeof(SI32) );
					}
					else
					{
						cltGameMsgRequest_TradeServer_Set_BuyOrder clinfo( pclTradeServerOrder );
						cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER, sizeof(cltGameMsgRequest_TradeServer_Set_BuyOrder), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

						StartComboBoxDisable();
					}
				}
				break;
			}
		}
		break;
	case NTRADESERVERBUY_BUTTON_REFRESH:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					CComboBox *pComboItem =     m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM );
					SI16 index = pComboItem->GetSelectedIndex();
					if( index < 0 ) return;
					char *szSelText = pComboItem->GetText( index );
					if ( szSelText == NULL ) break;
					SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);
					if ( itemunique <= 0 ) break;

					for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
					{
						pclClient->pclWorldTraderManager->clOrder[i].Init();
					}

					cltGameMsgRequest_TradeServer_Get_ListInfo clinfo(itemunique);
					cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_GET_LISTINFO, sizeof(cltGameMsgRequest_TradeServer_Get_ListInfo), (BYTE*)&clinfo);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

					StartComboBoxDisable();
				}
				break;
			}
		}
		break;
	case NTRADESERVERBUY_LIST_ORDERLIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_EXPLAIN)->ClearText();

					TCHAR ItemNum[16] = TEXT("");
//					if ( m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex() < 0 )
//						break;
//					SI32 index =  m_siListIndex[m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex()];
//					if ( index < 0 )
//						break;

					SI32 siSelectedIndex = m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetSelectedIndex();
					if( siSelectedIndex < 0 )			break;

					stListBoxItemData* pListBoxItemData = m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->GetItemData( siSelectedIndex, 0 );
					if( pListBoxItemData == NULL )		break;

					cltTradeServerOrder* pclTradeServerOrder = pclClient->pclWorldTraderManager->GetTradeServerOrder( pListBoxItemData->siParam[0] );
					if( pclTradeServerOrder == NULL )	break;

					ShowNationFlag(pclTradeServerOrder->siCharNation);

					StringCchPrintf( ItemNum, 16, TEXT("%d"), pclTradeServerOrder->clitem.siItemNum);

					m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_AMOUNT)->SetText(ItemNum);

					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclTradeServerOrder->clitem.siUnique);
					if ( ref )
					{
						SI32 itemtype = pclClient->pclItemManager->pclItemInfo[ref]->siType ;
						// 오브젝트면
						if ( pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
						{
							cltItem clitem;
							clitem.Init();

							if ( pclClient->pclItemManager->MakeTradeServerObjectForRank(&clitem, pclTradeServerOrder->clitem.siUnique,
								pclTradeServerOrder->clitem.siItemNum,
								pclTradeServerOrder->clitem.siRank,
								pclTradeServerOrder->clitem.siRareType,
								pclTradeServerOrder->clitem.siType,
								pclTradeServerOrder->clitem.siPower) )
							{
								SI32 itemstatus = 0 ;
								char explain[1024] = "";
								//if ( pclClient->pclItemManager->GetExplain(&clitem ,explain,(bool)&itemstatus) == TRUE )
								if ( pclClient->pclItemManager->GetExplain(&clitem ,explain) == TRUE )
								{
									TCHAR *pText = GetTxtFromMgr(6233);
									char buffer[1024] = "" ;
									sprintf(buffer,pText,
										pclTradeServerOrder->clitem.siRank,
										explain);
									m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_EXPLAIN)->SetText(buffer);
								}
							}
						}
					}
				}
				break;
			}
		}
		break;

	}
}

BOOL CNTradeServerBuyDlg::ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	SI32 i;
	// 콤보 박스를 지운다.
	pCombo->Clear();

	stComboBoxItemData itemdata;

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( i = 0;i < MAX_ITEMINFO_NUMBER;i++ )
	{	
		cltItemInfo* pinfo = pclitemmanager->pclItemInfo[i];

		if(pinfo == NULL)continue;
		if(pinfo->siType != itemtypeunique)continue;

		// 주어진 타입과 같은 아이템만 열거한다. 
		// 사용자에게 보여지는 아이템인 경우에만. 
		bool bshow = false;
		if(pinfo->siTradeServerMinNum > 0 )
		{
			bshow = true;
		}
		else
		{
			bshow = false;
		}

		if(bshow)
		{
			itemdata.Init();
			itemdata.Set( (TCHAR *)pinfo->GetName() );
			pCombo->AddItem( &itemdata );
		}
	}

	return TRUE;
}

int TradeServerOrder_CompFunc( const void* pParam1, const void* pParam2 )
{
	cltTradeServerOrder* pOrder1 = (cltTradeServerOrder*)pParam1;
	cltTradeServerOrder* pOrder2 = (cltTradeServerOrder*)pParam2;

	SI64 siPrice1 = pOrder1->clitem.siPrice / max(1, pOrder1->clitem.siItemNum);
	SI64 siPrice2 = pOrder2->clitem.siPrice / max(1, pOrder2->clitem.siItemNum);

	return (int)(siPrice1 - siPrice2);
}

void CNTradeServerBuyDlg::RefreshOrder()
{
	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->Clear();

	stListBoxItemData tempListBoxItem;
	TCHAR tempNumber[32] = TEXT("");
	TCHAR tempPrice[32] = TEXT("");
	TCHAR server[20] = TEXT("");

	NSafeTArray<cltTradeServerOrder, MAX_TRADESERVER_ORDER_LIST>	clorder;
	//SI32 index = Sort(&pclClient->pclWorldTraderManager->clOrder[0], &clorder[0]);
	for( SI32 i=0; i<MAX_TRADESERVER_ORDER_LIST; i++ )
	{
		clorder[i].Set( &pclClient->pclWorldTraderManager->clOrder[i] );
	}
	qsort(&clorder[0], MAX_TRADESERVER_ORDER_LIST, sizeof(cltTradeServerOrder), TradeServerOrder_CompFunc );

	// 판매 목록 리스트를 연결한다.
	for( SI32 i = 0; i < MAX_TRADESERVER_ORDER_LIST; ++i )	
	{
		if( clorder[i].clitem.siUnique != 0 )
		{
			tempListBoxItem.Init();

			if( clorder[i].clitem.siUnique <= 0 )		continue;
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( clorder[i].clitem.siUnique );
			if( ref <= 0)								continue;

			SI32 type = pclClient->pclItemManager->pclItemInfo[ref]->siType;
			if( type < 0)								continue;

			TCHAR* ItemName = (TCHAR*)pclClient->pclItemManager->GetName(clorder[i].clitem.siUnique);
			tempListBoxItem.Set( 0, ItemName, clorder[i].siIndex );

			StringCchPrintf( tempNumber, 32, TEXT("%d"), clorder[i].clitem.siItemNum);
			tempListBoxItem.Set( 1, tempNumber );

			StringCchPrintf( tempPrice, 32, TEXT("%d"), clorder[i].clitem.siPrice );
			tempListBoxItem.Set( 2, tempPrice );

			ZeroMemory(tempPrice,sizeof(TCHAR)*32);
			REAL64 fprice = (REAL64)clorder[i].clitem.siPrice / (REAL64)clorder[i].clitem.siItemNum ;
			sprintf( tempPrice, "%3.3f", fprice );
			tempListBoxItem.Set( 3, tempPrice );

			pclClient->GetServerNameFromNation(clorder[i].szServerNation,clorder[i].siServerIndex,server);
			tempListBoxItem.Set( 4, server );

			//SI64 totalprice = clorder[i].clitem.siPrice * siWorldMoneyPrice ;
			SI64 totalprice =( clorder[i].clitem.siPrice * siWorldMoneyPrice ) / clorder[i].clitem.siItemNum ;

			ZeroMemory(tempPrice,sizeof(TCHAR)*32);
			g_JYLibFunction.SetNumUnit(totalprice,tempPrice, 32);
			tempListBoxItem.Set( 5, tempPrice );

			m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->AddItem( &tempListBoxItem );
		}
	}

	m_InterfaceFactory_.GetList( NTRADESERVERBUY_LIST_ORDERLIST )->Refresh();

	char buffer[128] = "" ;

	TCHAR *pText = GetTxtFromMgr(611);

	g_JYLibFunction.SetNumUnit(siItemPrice,buffer, 128,pText );
	m_InterfaceFactory_.GetEdit( NTRADESERVERBUY_EDIT_ITEMPRICE )->SetText(buffer);
	ZeroMemory(buffer,sizeof(TCHAR)*128);

	g_JYLibFunction.SetNumUnit(siWorldMoneyPrice,buffer, 128,pText );
	m_InterfaceFactory_.GetEdit( NTRADESERVERBUY_EDIT_WORLDMONEYPRICE )->SetText(buffer);

}


/*SI32 Sort(cltTradeServerOrder sortdata[], cltTradeServerOrder* pindex)
{
	SI32 i;
	SI32 index = 0;

	bool binfo[MAX_TRADESERVER_ORDER_LIST];

	for(i = 0;i < MAX_TRADESERVER_ORDER_LIST;i++)
	{
		binfo[i] = false;
		pindex[i].Init();
	}

	for(SI32 vill = 0;vill < MAX_TRADESERVER_ORDER_LIST;vill++)
	{
		SI64 minprice = 100000000;
		SI32 maxindex = -1;

		for(i = 0;i < MAX_TRADESERVER_ORDER_LIST;i++)
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclClient->pclWorldTraderManager->clOrder[i].clitem.siUnique );
			if( ref <= 0)
				continue;

			SI32 type = pclClient->pclItemManager->pclItemInfo[ref]->siType;
			if( type < 0)
				continue;

			SI64 price = sortdata[i].clitem.siPrice / sortdata[i].clitem.siItemNum ;

			if(binfo[i] == false && price <= minprice )
			{
				minprice	= price;
				maxindex	= i;
			}
			else if ( i == 0 && index == 0 )
			{
				minprice	= price;
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			binfo[maxindex] = true;
			pindex[index].Set(&pclClient->pclWorldTraderManager->clOrder[maxindex]);
			m_siListIndex[index] = maxindex ;
			index++;
		}
	}

	return index;
}
*/

void CNTradeServerBuyDlg::ShowItemImage(char* pName)
{
	SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(pName);

	if( itemunique <= 0 )
		return;

	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) == false )
		return ;

	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetItemSprType(m_siImageType);
	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFontIndex(m_siImageIndex);

	//if( GIMG_ITEMIMG0 == m_siImageType)
	//{
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFileName("Gimg\\ItemImg0.SPR");
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFontIndex(m_siImageIndex);
	//}
	//else if( GIMG_ITEMIMG1 == m_siImageType)
	//{
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFileName("Gimg\\ItemImg1.SPR");
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFontIndex(m_siImageIndex);
	//}
	//else if( GIMG_ITEMIMG2 == m_siImageType)
	//{
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFileName("Gimg\\ItemImg2.SPR");
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFontIndex(m_siImageIndex);
	//}
	//else if( GIMG_ITEMIMG3 == m_siImageType)
	//{
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFileName("Gimg\\ItemImg3.SPR");
	//	m_InterfaceFactory_.GetStatic( NTRADESERVERBUY_STATIC_SHOWITEM )->SetFontIndex(m_siImageIndex);
	//}

}

void CNTradeServerBuyDlg::ShowNationFlag(SI32 index)
{
	m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFileName("Gimg\\National_Flag_for_Trade.SPR");
	m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFontIndex( index );
	m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_NATIONNAME)->SetText(pclClient->pclCountryManager->GetCountryFromUnique(index) );


	//switch(index)
	//{
	//case 0:
	//	{
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFileName("Gimg\\NationFlag.spr");
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFontIndex(0);
	//		m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_NATIONNAME)->SetText("KOREA");
	//	}
	//	break;
	//case 1:
	//	{
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFileName("Gimg\\NationFlag.spr");
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFontIndex(1);
	//		m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_NATIONNAME)->SetText("JAPAN");
	//	}
	//	break;
	//case 2:
	//	{
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFileName("Gimg\\NationFlag.spr");
	//		m_InterfaceFactory_.GetImageStatic(NTRADESERVERBUY_IMAGESTATIC_NATIONFLAG)->SetFontIndex(2);
	//		m_InterfaceFactory_.GetEdit(NTRADESERVERBUY_EDIT_NATIONNAME)->SetText("GLOBAL");
	//	}
	//	break;
	//default: return ; break;
	//}
}

void CNTradeServerBuyDlg::StartComboBoxDisable()
{
	m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEMTYPE )->Enable( false );
	m_InterfaceFactory_.GetComboBox( NTRADESERVERBUY_COMBOBOX_ITEM )->Enable( false );

	m_InterfaceFactory_.GetButton( NTRADESERVERBUY_BUTTON_REFRESH )->Enable( false );
	m_InterfaceFactory_.GetButton( NTRADESERVERBUY_BUTTON_BUY )->Enable( false );

	m_kUpdateTimer_ComboBoxDisable.SetActive( true, GetTickCount() );
}


void CNTradeServerBuyDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 2:
		if( RetValue )
		{
			SI32 siOrderIndex = *(SI32*)pData;

			cltTradeServerOrder* pclTradeServerOrder = pclClient->pclWorldTraderManager->GetTradeServerOrder( siOrderIndex );
			if( pclTradeServerOrder )
			{
				cltGameMsgRequest_TradeServer_Set_BuyOrder clinfo( pclTradeServerOrder );
				cltMsg clMsg(GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER, sizeof(cltGameMsgRequest_TradeServer_Set_BuyOrder), (BYTE*)&clinfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	}

}