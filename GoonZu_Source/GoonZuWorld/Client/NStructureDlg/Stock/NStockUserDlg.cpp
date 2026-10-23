#include "NStockDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Stock.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"

#include "../CommonLogic/Bank/Bank-Manager.h"

#include <tchar.h>

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;


NStockUserDlg::NStockUserDlg()
{

}

NStockUserDlg::~NStockUserDlg()
{

}

void NStockUserDlg::InitDlg()
{
	SetControlMap( BUTTON_BUY, TEXT("button_buy") );
	SetControlMap( BUTTON_SELL, TEXT("button_sell") );
	SetControlMap( BUTTON_CANCEL_ORDER, TEXT("button_cancel_order") );

	//SetControlMap( COMBO_BUY_KIND, TEXT("combobox_buy_kind") );
	SetControlMap( COMBO_SELL_KIND, TEXT("combobox_sell_kind") );

	//SetControlMap( EDIT_BUY_AMOUNT, TEXT("editbox_buy_amount") );
	//SetControlMap( EDIT_BUY_PRICE, TEXT("editbox_buy_price") );
	SetControlMap( EDIT_SELL_AMOUNT, TEXT("editbox_sell_amount") );
	SetControlMap( EDIT_SELL_PRICE, TEXT("editbox_sell_price") );

	SetControlMap( EDIT_FEERATE, TEXT("editbox_feerate") );

	SetControlMap( LIST_HAVESTOCK, TEXT("listbox_havestock") );
	SetControlMap( LIST_ORDERSTOCK, TEXT("listbox_orderstock") );
	SetControlMap( LIST_MOST_ORDER_SELL, TEXT("listbox_most_order_sell") );
	SetControlMap( LIST_MOST_ORDER_BUY, TEXT("listbox_most_order_buy") );
	
	SetControlMap( EDIT_AVERAGE_BUY, TEXT("editbox_average_Buy") );
	SetControlMap( EDIT_AVERAGE_SELL, TEXT("editbox_average_Sell") );

	//[김영훈 추가:2007.11.21 - 주식시장에서 갯수랑 가격에 숫자 이외에 값이 입력 못하도록 수정]
	((CEdit*)m_pControlMap[ EDIT_SELL_AMOUNT ])->SetNumberMode(true);
	//((CEdit*)m_pControlMap[ EDIT_SELL_PRICE ])->SetNumberMode(true);

	CList *pList;
	
	// 보유 주식 현황
	pList = (CList *)m_pControlMap[ LIST_HAVESTOCK ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(3452);
	pList->SetColumn( 0, pList->GetWidth(), pText);
	pList->Refresh();

	// 내 주문 내용 
	pList = (CList *)m_pControlMap[ LIST_ORDERSTOCK ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText = GetTxtFromMgr(3453);
	pList->SetColumn( 0, pList->GetWidth(), pText );
	pList->Refresh();

	// 최적 주문
	pList = (CList *)m_pControlMap[ LIST_MOST_ORDER_SELL ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText = GetTxtFromMgr(6723);
	pList->SetColumn( 0, pList->GetWidth(), pText );
	pList->Refresh();


	pList = (CList *)m_pControlMap[ LIST_MOST_ORDER_BUY ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText = GetTxtFromMgr(6724);
	pList->SetColumn( 0, pList->GetWidth(), pText );
	pList->Refresh();

	// By LEEKH - 2007.10.17 - 입력제한
	CEdit* pEdit_Sell_Amount = (CEdit*)m_pControlMap[ EDIT_SELL_AMOUNT ];
	pEdit_Sell_Amount->SetMaxEditTextLength( 5 );

	//// By LEEKH - 2007.10.17 - 입력제한
	//CEdit* pEdit_Sell_Price = (CEdit*)m_pControlMap[ EDIT_SELL_PRICE ];
	//pEdit_Sell_Price->SetMaxEditTextLength( 11 );

	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);
		
	//pStockManager->clMostOrder.Init();

	memset( &pStockManager->clSellMostOrder[0], 0, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
	memset( &pStockManager->clBuyMostOrder[0], 0, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );

	pStockManager->siAverageSellMoney = 0;
	pStockManager->siAverageBuyMoney = 0;

	pStockManager->clOldStockInfo.Init();
	
	// 보유 주식을 리스트 박스에 추가한다. 
	SetHaveStock();

	// 주문 내용을 리스트 박스에 추가한다. 
	SetStockOrderList();

	// 매수 종목을 콤보박스에 추가한다. 
//	SetComboBuyKind();

	// 매도 종목을 콤보박스에 추가한다.
	SetComboSellKind();

}

void NStockUserDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	TCHAR buffer[ 256 ];

	// 보유 주식을 리스트 박스에 추가한다. 
	SetHaveStock();

	// 주문 내용을 리스트 박스에 추가한다. 
	SetStockOrderList();

	// 수수료 정보를 보여준다
	ShowFeeInfo();

	// 최적 주문 내용을 보여준다. 
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);

	CList *pSellList, *pBuyList;
	pSellList = (CList *)m_pControlMap[ LIST_MOST_ORDER_SELL ];
	pBuyList = (CList *)m_pControlMap[ LIST_MOST_ORDER_BUY ];

	pSellList->Clear();
	pBuyList->Clear();

	stListBoxItemData itemdata;

	TCHAR temp[256];

	TCHAR* name  = NULL;
	TCHAR* pText = GetTxtFromMgr(1486);
	/*
	
	StringCchPrintf(temp, 256, pText);

	itemdata.Init();
	itemdata.Set( 0, temp );
	pSellList->AddItem( &itemdata );

	pText = GetTxtFromMgr(1487);
	StringCchPrintf(temp, 256, pText);

	itemdata.Init();
	itemdata.Set( 0, temp );
	pBuyList->AddItem( &itemdata );
*/
	TCHAR* pTextMoney = GetTxtFromMgr(611); //돈 단위를 국가별로 구분.

	StringCchPrintf(temp,256,TEXT("%I64d%s"),pStockManager->siAverageSellMoney, pTextMoney);
	((CEdit *)m_pControlMap[ EDIT_AVERAGE_SELL ])->SetText(temp);
	
	StringCchPrintf(temp,256,TEXT("%I64d%s"),pStockManager->siAverageBuyMoney, pTextMoney);
	((CEdit *)m_pControlMap[ EDIT_AVERAGE_BUY ])->SetText(temp);

	pText = GetTxtFromMgr(1488);

	for( SI32 i = 0; i < MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM ; i ++ )
	{
		name = pclClient->pclVillageManager->GetName( pStockManager->clSellMostOrder[i].siVillageUnique );
		if( name )
		{
			StringCchPrintf(buffer, 256, pText, 
							TEXT(""), 
							name, 
							pStockManager->clSellMostOrder[i].siPrice, 
							pStockManager->clSellMostOrder[i].siAmount);

			itemdata.Init();
			itemdata.Set( 0, buffer );
			pSellList->AddItem( &itemdata );
		}

		name = pclClient->pclVillageManager->GetName( pStockManager->clBuyMostOrder[i].siVillageUnique );
		if( name )
		{
			StringCchPrintf(buffer, 256, pText, 
							TEXT(""), 
							name, 
							pStockManager->clBuyMostOrder[i].siPrice, 
							pStockManager->clBuyMostOrder[i].siAmount);

			itemdata.Init();
			itemdata.Set( 0, buffer );
			pBuyList->AddItem( &itemdata );
		}
	}

	pBuyList->Refresh();
	pSellList->Refresh();	

}

void NStockUserDlg::ShowFeeInfo()
{
	TCHAR buffer[256];

	REAL64 feerate = (REAL64)m_pStrManager->clClientStrInfo.clStockStrInfo.clFee.GetFeeRate() / 10;

	StringCchPrintf( buffer, 256, TEXT("%3.1f%%"), feerate);

	SetEditText( EDIT_FEERATE, buffer );
}


// 보유 주식을 표시한다. 
void NStockUserDlg::SetHaveStock()
{
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);
	CList *pList;
	
	// 보유 주식 현황
	pList = (CList *)m_pControlMap[ LIST_HAVESTOCK ];

	SI32 i;
	TCHAR buffer[256];

	SI32 id = 1;
	if(id==0)return ;

	// 정보가 갱신된 상태에서만 업데이트 한다. 
	if( pclCM->CR[id]->pclCI->clStock.clInfo.IsSame(&pStockManager->clOldStockInfo) == true)return ;
	pStockManager->clOldStockInfo.Set(&pclCM->CR[id]->pclCI->clStock.clInfo);


	// 리스트 박스를 초기화한다. 
	pList->Clear();

	stListBoxItemData itemdata;

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		SI32 villageunique	= pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
		SI32 amount			= pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siAmount;
	
		if(villageunique)
		{
			if(villageunique >=0)
			{
				TCHAR* pText = GetTxtFromMgr(1500);
				StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName, amount);
				itemdata.Init();
				itemdata.Set( 0, buffer );
				pList->AddItem( &itemdata );
			}
			else
			{
				MsgBox(TEXT("fds09f82j"), TEXT("fdo9873:%d"), villageunique);
			}
		}
	}

	NListSort( pList );

	//<= pList->Refresh();
}

// 주문 내용을 리스트 박스에 추가한다. 
void NStockUserDlg::SetStockOrderList()
{
	cltStockManager *pStockManager = ((cltStockManager *)m_pStrManager);
	CList *pList;

	// 내 주문 내용 
	pList = (CList *)m_pControlMap[ LIST_ORDERSTOCK ];

	SI32 i;

	// 리스트 박스를 초기화한다. 
	pList->Clear();

	SI32 id = 1;
	if(id <= 0)return ;

	stListBoxItemData itemdata;

	for(i = 0;i < MAX_ORDER_PER_PERSON;i++)
	{
		cltTradeOrder* pclStockOrder = &pclCM->CR[id]->pclCI->clStock.clOrderInfo.clOrderUnit[i].clOrder;
		SI32 villageunique	= pclStockOrder->GetVillageUnique();
		SI32 amount			= pclStockOrder->siAmount;
		GMONEY price			= pclStockOrder->siPrice;
		SI32 ordermode		= pclCM->CR[id]->pclCI->clStock.clOrderInfo.clOrderUnit[i].siOrderMode;

	
		if(villageunique)
		{
			if(villageunique >=0)
			{
				NTCHARString128 strMessage( TEXT("") );
				
				if(ordermode == ORDERMODE_SELL)
				{
					strMessage = GetTxtFromMgr(1501);
				}
				else
				{
					strMessage = GetTxtFromMgr(1502);
				}

				TCHAR szMoney[128]= { '\0', };
				g_JYLibFunction.SetNumUnit( price, szMoney, sizeof(szMoney) );

				strMessage.Replace( TEXT("#village#"), pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName );
				strMessage.Replace( TEXT("#amount#"), SI32ToString(amount) );
				strMessage.Replace( TEXT("#money#"), szMoney );

				
				itemdata.Init();
				itemdata.Set( 0, strMessage );
				pList->AddItem( &itemdata );
			}
			else
			{
				MsgBox(TEXT("fds2309f82j"), TEXT("fdfdo9873:%d"), villageunique);
			}
		}
	}

	NListSort( pList );

	//<= pList->Refresh();

}


// 매도 종목을 콤보 박스에 추가한다. 
void NStockUserDlg::SetComboSellKind()
{

	CComboBox *pCombo;
	pCombo = (CComboBox *)m_pControlMap[ COMBO_SELL_KIND ];	
	
	stComboBoxItemData itemdata;
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		// [영훈] 초보자 마을 : 초보자 마을 주식은 주식 거래 리스트에 안나타나게 한다
		if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
		{
			if ( Const_Beginner_Village == i )
			{
				continue;
			}
		}

		if(pclClient->pclVillageManager->pclVillageInfo[i])
		{
			itemdata.Init();
			itemdata.Set( pclClient->pclVillageManager->pclVillageInfo[i]->szName );			
			pCombo->AddItem( &itemdata );
		}
	}

	NComboBoxSort( pCombo );
}


void NStockUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	TCHAR buffer[ 512 ];
	cltClient* pclclient = (cltClient*)pclClient;

	CComboBox *pComboBuyKind = (CComboBox *)m_pControlMap[ COMBO_SELL_KIND ];
	CComboBox *pComboSellKind = (CComboBox *)m_pControlMap[ COMBO_SELL_KIND ];

	
	CASE_CONTROL( EDIT_SELL_PRICE )
	{
		switch(nEvent) {
		case EVENT_EDITBOX_CHANGE :
			NTCHARString512	kRawPrice( GetEditText( EDIT_SELL_PRICE ));
			kRawPrice.ReplaceAll(",", "");	// , 삭제
			TCHAR		szPrice[64] = { '\0', };
			GMONEY		siPrice = _tstoi64(kRawPrice);
			if( siPrice > pclClient->GetGlobalValue("GV_StockTradeMoneyLimit"))
				siPrice = pclClient->GetGlobalValue("GV_StockTradeMoneyLimit");
			
			TCHAR buf[128] = "";
			g_JYLibFunction.SetNumUnit( siPrice, buf, sizeof(buf) );
			SetEditText( EDIT_SELL_PRICE, buf ); 
		}
	}

	CASE_CONTROL( COMBO_BUY_KIND )
	{
		
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				// 사용자 유효성 검사. 
				SI32 id = 1;
				if(id <= 0)break;

				// 선택한 마을의 이름을 확보한다. 
				TCHAR *szSelText = pComboBuyKind->GetText( pComboBuyKind->GetSelectedIndex() );

				// 마을의 유니크를 확보한다. 
				SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
				if(villageunique <= 0)break;
					
				// 게임서버에 최저팔자 주문을 의뢰한다. 
				cltGameMsgRequest_StockOrderInfo clInfo(villageunique);
				cltMsg clMsg(GAMEMSG_REQUEST_STOCKORDERINFO, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
		}
	}

	else CASE_CONTROL( COMBO_SELL_KIND )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED: 
			{
				// 사용자 유효성 검사. 
				SI32 id = 1;
				if(id <= 0)break;

				TCHAR *szSelText = pComboSellKind->GetText( pComboSellKind->GetSelectedIndex() );

				// 마을의 유니크를 확보한다. 
				SI32 villageunique = pclClient->pclVillageManager->FindVillageUniqueFromName(szSelText);
				if(villageunique <= 0)break;
					
				// 게임서버에 최고사자 주문을 의뢰한다. 
				cltGameMsgRequest_StockOrderInfo clInfo(villageunique);
				cltMsg clMsg(GAMEMSG_REQUEST_STOCKORDERINFO, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_CANCEL_ORDER )
	{
		SI32 id = 1;
		if(id)
		{
			cltMsg clMsg(GAMEMSG_REQUEST_CANCELSTOCKORDER, 0, NULL);
			
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	else CASE_CONTROL( BUTTON_BUY )
	{
		SI32 id = 1;

		//-------------------------------------
		// 제한된 주문 건수 이상은 할 수 없다. 
		//-------------------------------------
		SI32 stockordernum = pclCM->CR[id]->GetStockOrderNumber();
		if(stockordernum >= MAX_ORDER_PER_PERSON)
		{
			TCHAR* pText = GetTxtFromMgr(1489);
			SetMainExplain(pText);
			return;
		}

		//------------------------------
		//	전장에 계좌가 있는지 확인한다. 
		//------------------------------

		//전장에 계좌가 없다면 없다는 메세지를 출력하고 0원일 경우 0원이라는 메세지를 
		//출력한다.[성웅 2007.06.26]

		
		BOOL bAccount = pclCM->CR[id]->pclCI->clBank.clAccountNum.IsValid();

		if (bAccount==FALSE) {
			TCHAR* pText = GetTxtFromMgr(1490);
			SetMainExplain(pText);
			return;
		}
		
		else if(pclCM->CR[id]->GetBankMoney() == 0)
		{
			TCHAR* pText = GetTxtFromMgr(7113);
			SetMainExplain(pText);
			return;
		}	

		//-----------------------------
		// 종목을 확인한다. (유니크값) 
		//------------------------------
		TCHAR *pControlText;
		pControlText = pComboBuyKind->GetText( pComboBuyKind->GetSelectedIndex() );

		SI32 ref = pclClient->pclVillageManager->FindVillageUniqueFromName( pControlText );
		if(ref < 0)
		{
			TCHAR* pText = GetTxtFromMgr(1491);
			SetMainExplain(pText);
			return;
		}
		SI32 villageunique = pclClient->pclVillageManager->pclVillageInfo[ref]->siVillageUnique;

		//----------------------------
		// 수량을 확인한다. 
		//----------------------------
		pControlText = GetEditText( EDIT_SELL_AMOUNT );

		SI32 amount = _tstoi( pControlText );

		if(amount <= 0)
		{
			TCHAR* pText = GetTxtFromMgr(1492);
			SetMainExplain(pText);
			return;
		}
		if(amount > MAX_AMOUNT_PER_DEAL)
		{
			TCHAR* pText = GetTxtFromMgr(1493);
			StringCchPrintf(buffer, 512, pText, MAX_AMOUNT_PER_DEAL);
			SetMainExplain(buffer);
			return;
		}

		//---------------------------
		// 단가를 확인한다. 
		//---------------------------
		//pControlText = GetEditText( EDIT_SELL_PRICE );

		//// by LEEKH 2007.10.17 돈에 관련된 수치는 가능한 SI64로 처리한다.
		////SI32 price = _tstoi( pControlText );
		//SI64 price = _tstoi64( pControlText );
		NTCHARString512	kRawPrice( GetEditText( EDIT_SELL_PRICE ));
		kRawPrice.ReplaceAll(",", "");	// , 삭제
		GMONEY		price = _tstoi64(kRawPrice);
		if( price > pclClient->GetGlobalValue("GV_StockTradeMoneyLimit"))
			price = pclClient->GetGlobalValue("GV_StockTradeMoneyLimit");

		if(price <= 0)
		{
			TCHAR* pText = GetTxtFromMgr(1494);
			SetMainExplain(pText);
			return;
		}

		//------------------------------------------
		// 합계 금액이 잔고에 남아 있는지 확인한다. 
		//------------------------------------------
		SI64 totalmoney = (SI64)price * (SI64)amount;

		/*if( price > 2100000000)
		{
			TCHAR* pText = GetTxtFromMgr(5484);
			StringCchPrintf(buffer, 512, pText);

			SetMainExplain(buffer);
			return;
		}*/

		GMONEY diff		= totalmoney - pclCM->CR[id]->GetBankMoney();

		if(diff > 0)
		{
			TCHAR* pText = GetTxtFromMgr(1495);
			StringCchPrintf(buffer, 512, pText, diff );

			SetMainExplain(buffer);
			return;
		}

		TCHAR		szPrice[64] = { '\0', };
		g_JYLibFunction.SetNumUnit(price, szPrice, sizeof(szPrice));

		TCHAR*			pTitle = GetTxtFromMgr(850);
		NTCHARString512 strText( GetTxtFromMgr(5399) );

		strText.Replace( TEXT("#village#"), pclClient->pclVillageManager->GetName(villageunique) );
		strText.Replace( TEXT("#amount#"), SI32ToString(amount) );
		strText.Replace( TEXT("#price#"), szPrice );


		stMsgBox MsgBox;
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle, strText, MSGBOX_TYPE_YESNO, 0 );
		BYTE TempBuffer[ 512 ] = "";
		SI16 Size = sizeof( id ) + sizeof( price ) + sizeof(amount) + sizeof(villageunique) ;
		memcpy( TempBuffer, &id, sizeof( id ) );
		memcpy( &TempBuffer[ sizeof(id) ], &price, sizeof( price ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) ], &amount, sizeof( amount ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( price ) + sizeof(amount)], &villageunique, sizeof( villageunique ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		////---------------------------------------
		//// 서버로 주식을 매수하라는 요청을 통보한다. 
		////---------------------------------------
		//if(id)
		//{
		//	// PersonID를 0으로 보내면 서버에서 적절한 값으로 대입한다.
		//	cltTradeOrder clStockOrder(0, price, amount, 0, villageunique);
		//	
		//	cltMsg clMsg(GAMEMSG_REQUEST_BUYSTOCK,  
		//		sizeof(cltTradeOrder), (TCHAR*)&clStockOrder);
		//	
		//	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//}


	}

	else CASE_CONTROL( BUTTON_SELL )
	{
		SI32 id = 1;
        BOOL bStockAccount = pclCM->CR[id]->pclCI->clStock.clInfo.HaveStock();
		//-------------------------------------
		// 주식이 없으면 없다고 한다. [2007.08.10 손성웅]
		//-------------------------------------
		if (bStockAccount==FALSE) {
			TCHAR* pText = GetTxtFromMgr(1497);
			SetMainExplain(pText);
			return;
		}
		//-------------------------------------
		// 제한된 주문 건수 이상은 할 수 없다. 
		//-------------------------------------
		SI32 stockordernum = pclCM->CR[id]->GetStockOrderNumber();
		if(stockordernum >= MAX_ORDER_PER_PERSON)
		{
			TCHAR* pText = GetTxtFromMgr(1496);
			SetMainExplain(pText);
			return;
		}

		//------------------------------
		//	전장에 계좌가 있는지 확인한다. 
		//------------------------------
		BOOL bAccount = pclCM->CR[id]->pclCI->clBank.clAccountNum.IsValid();

		if (bAccount==FALSE) {
			TCHAR* pText = GetTxtFromMgr(1490);
			SetMainExplain(pText);
			return;
		}

		//if(pclCM->CR[id]->GetBankMoney() == 0)
		//{
		//	TCHAR* pText = GetTxtFromMgr(1490);
		//	SetMainExplain(pText);
		//	return;
		//}

		//-----------------------------
		// 종목을 확인한다. (유니크값) 
		//------------------------------

		TCHAR *pControlText;
		pControlText = pComboSellKind->GetText( pComboSellKind->GetSelectedIndex() );
		
		SI32 ref = pclClient->pclVillageManager->FindVillageUniqueFromName( pControlText );
		if(ref < 0)
		{
			TCHAR* pText = GetTxtFromMgr(1491);
			SetMainExplain(pText);
			return;
		}
		SI32 villageunique = pclClient->pclVillageManager->pclVillageInfo[ref]->siVillageUnique;

		// 해당 villageunique를 보유하고 있는지 확인한다. 
		SI32 maxsellamount = pclCM->CR[id]->pclCI->clStock.clInfo.GetStockAmount(villageunique);
		if(maxsellamount == 0)
		{
			TCHAR* pText = GetTxtFromMgr(1497);
			SetMainExplain(pText);
			return;
		}

		//----------------------------
		// 수량을 확인한다. 
		//----------------------------
		pControlText = GetEditText( EDIT_SELL_AMOUNT );		
		SI32 amount = _tstoi( pControlText );

		if(amount <= 0)
		{
			TCHAR* pText = GetTxtFromMgr(1492);
			SetMainExplain(pText);
			return;
		}
		else if(amount > maxsellamount)
		{
			TCHAR* pText = GetTxtFromMgr(1498);
			SetMainExplain(pText);
			return;
		}
		else if(amount > MAX_AMOUNT_PER_DEAL)
		{
			TCHAR* pText = GetTxtFromMgr(1493);
			StringCchPrintf(buffer, 512, pText, MAX_AMOUNT_PER_DEAL);
			SetMainExplain(buffer);
			return;
		}

		//---------------------------
		// 단가를 확인한다. 
		//---------------------------
		NTCHARString512	kRawPrice( GetEditText( EDIT_SELL_PRICE ));
		kRawPrice.ReplaceAll(",", "");	// , 삭제
		TCHAR		szPrice[64] = { '\0', };
		GMONEY		siPrice = _tstoi64(kRawPrice);
		if( siPrice > pclClient->GetGlobalValue("GV_StockTradeMoneyLimit"))
			siPrice = pclClient->GetGlobalValue("GV_StockTradeMoneyLimit");

		//pControlText = GetEditText( EDIT_SELL_PRICE );		
		//SI64 price = _tstoi64(pControlText);
		if(siPrice <= 0)
		{
			TCHAR* pText = GetTxtFromMgr(1494);
			SetMainExplain(pText);
			return;
		}

		//TCHAR		szPrice[64] = { '\0', };
		g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));

		TCHAR*			pTitle = GetTxtFromMgr(5401);
		NTCHARString512 strText( GetTxtFromMgr(5400) );

		strText.Replace( TEXT("#village#"), pclClient->pclVillageManager->GetName(villageunique) );
		strText.Replace( TEXT("#amount#"), SI32ToString(amount) );
		strText.Replace( TEXT("#price#"), szPrice );

		
		stMsgBox MsgBox;
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle, strText, MSGBOX_TYPE_YESNO, 1 );
		BYTE TempBuffer[ 512 ];
		SI16 Size = sizeof( id ) + sizeof( siPrice ) + sizeof(amount) + sizeof(villageunique) ;
		memcpy( TempBuffer, &id, sizeof( id ) );
		memcpy( &TempBuffer[ sizeof(id) ], &siPrice, sizeof( siPrice ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( siPrice ) ], &amount, sizeof( amount ) );
		memcpy( &TempBuffer[ sizeof( id ) + sizeof( siPrice ) + sizeof(amount)], &villageunique, sizeof( villageunique ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );


		////---------------------------------------
		//// 서버로 주식을 매도하라는 요청을 통보한다. 
		////---------------------------------------
		//if(id)
		//{
		//	// PersonID를 0으로 보내면 서버에서 적절한 값으로 대입한다.
		//	cltTradeOrder clStockOrder(0, price, amount, 0, villageunique);
		//	
		//	cltMsg clMsg(GAMEMSG_REQUEST_SELLSTOCK, 
		//		sizeof(cltTradeOrder), (TCHAR*)&clStockOrder);
		//	
		//	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//}

	}

}

void NStockUserDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0: // 주식 사기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 siPos			= 0;
				SI32 id				= *((SI32*)(pData + siPos));	siPos += sizeof(id);
				SI64 price			= *((SI64*)(pData + siPos));	siPos += sizeof(price);
				SI32 amount			= *((SI32*)(pData + siPos));	siPos += sizeof(amount);
				SI32 villageunique	= *((SI32*)(pData + siPos));	siPos += sizeof(villageunique);

				//---------------------------------------
				// 서버로 주식을 매수하라는 요청을 통보한다. 
				//---------------------------------------
				if(id)
				{
					// PersonID를 0으로 보내면 서버에서 적절한 값으로 대입한다.
					cltTradeOrder clStockOrder(0, price, amount, 0, villageunique);
					
					cltMsg clMsg(GAMEMSG_REQUEST_BUYSTOCK,  
						sizeof(cltTradeOrder), (BYTE*)&clStockOrder);
					
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				}

			}
		}
		break;
	case 1:	// 주식 팔기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 siPos			= 0;
				SI32 id				= *((SI32*)(pData + siPos));	siPos += sizeof(id);
				SI64 price			= *((SI64*)(pData + siPos));	siPos += sizeof(price);
				SI32 amount			= *((SI32*)(pData + siPos));	siPos += sizeof(amount);
				SI32 villageunique	= *((SI32*)(pData + siPos));	siPos += sizeof(villageunique);

				//---------------------------------------
				// 서버로 주식을 매도하라는 요청을 통보한다. 
				//---------------------------------------
				if(id)
				{
					// PersonID를 0으로 보내면 서버에서 적절한 값으로 대입한다.
					cltTradeOrder clStockOrder(0, price, amount, 0, villageunique);
					
					cltMsg clMsg(GAMEMSG_REQUEST_SELLSTOCK, sizeof(cltTradeOrder), (BYTE*)&clStockOrder);
					
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
		break;
	}
}
