//---------------------------------
// 2003/6/3 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "Stock.h"


#include "MsgType-Stock.h"

#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;


cltStockManager::cltStockManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) 
: cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, ranktype, strFileName )
{

	bInitBuyOrder = FALSE;
	bInitSellOrder = FALSE;

	bBidding = FALSE;

	UpdateStockBidVillageSwitch = FALSE;
    UpdateStockBidderSwitch = FALSE;
	UpdateInvestSwitch = FALSE;

	UpdateStockOfferingInfoSwitch = FALSE;
	ZeroMemory( &clStockOfferingInfo, sizeof(clStockOfferingInfo) );


	// 증자 마을 리스트를 초기화 한다 
	InitStockBidVillage();

	// 증자 마을들의 입찰자 리스트를 초기화 한다 
	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {
		InitStockBidder( i );
	}

	tickTemp = GetTickCount();

	// 사자, 팔자 주문을 초기화한다. 
	pclOrderManager = NULL;

	if(gamemode == GAMEMODE_SERVER)
	{
		pclOrderManager	= new cltStockOrderManager;
		//pclAcountManager	= new cltStockAcountManager;
	}

	//siMostOrderMode	= ORDERMODE_SELL;
	//clMostOrder.Init();

	memset( &clSellMostOrder[0], 0, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
	memset( &clBuyMostOrder[0], 0, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );

}


cltStockManager::~cltStockManager()
{

/*	if(pclAcountManager)
	{
		delete pclAcountManager;
		pclAcountManager = NULL;
	}
*/

	if(pclOrderManager)
	{
		delete pclOrderManager;
		pclOrderManager = NULL;
	}


}

// 최초 환영 인사말표시..
void cltStockManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1499);
	SetMainExplain(hDlg, pText);
}

void cltStockManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
	
}


// 여각을 신설한다.
SI32 cltStockManager::CreateStock(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltStockMarket(siunique);
		return siunique;
	}
	
	return 0;
}

// 보유 주식을 표시한다. 
void cltStockManager::SetHaveStock(HWND hDlg)
{
	SI32 i;
	TCHAR buffer[256];

	SI32 id = 1;
	if(id==0)return ;

	// 정보가 갱신된 상태에서만 업데이트 한다. 
	if(pclCM->CR[id]->pclCI->clStock.clInfo.IsSame(&clOldStockInfo) == true)return ;
	clOldStockInfo.Set(&pclCM->CR[id]->pclCI->clStock.clInfo);

	
	// 리스트 박스를 초기화한다. 
	SendDlgItemMessage(hDlg, IDC_LIST_HAVESTOCK, LB_RESETCONTENT, 0, 0);

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		SI32 villageunique	= pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
		SI32 amount			= pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siAmount;
	
		if(villageunique)
		{
			if(villageunique >=0)
			{
				TCHAR* pText = GetTxtFromMgr(1500);
				StringCchPrintf(buffer, 256, pText,pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName, amount); 
				SendDlgItemMessage(hDlg, IDC_LIST_HAVESTOCK, LB_ADDSTRING, 0, (LPARAM)buffer);
			}
			else
			{
				MsgBox(TEXT("fds09f82j"), TEXT("fdo9873:%d"), villageunique);
			}
		}
	}

}

// 주문 내용을 리스트 박스에 추가한다. 
void cltStockManager::SetStockOrderList(HWND hDlg)
{
	SI32 i;
	TCHAR buffer[256];

	// 리스트 박스를 초기화한다. 
	SendDlgItemMessage(hDlg, IDC_LIST_STOCKORDERINFO, LB_RESETCONTENT, 0, 0);

	SI32 id = 1;
	if(id <= 0)return ;

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
				if(ordermode == ORDERMODE_SELL)
				{
					TCHAR* pText = GetTxtFromMgr(1501);
					StringCchPrintf(buffer, 256, pText,pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName, amount, price); 
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(1502);
					StringCchPrintf(buffer, 256, pText,pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName, amount, price); 
				}
				
				SendDlgItemMessage(hDlg, IDC_LIST_STOCKORDERINFO, LB_ADDSTRING, 0, (LPARAM)buffer);
			}
			else
			{
				MsgBox(TEXT("fds2309f82j"), TEXT("fdfdo9873:%d"), villageunique);
			}
		}
	}

}

void cltStockManager::SetComboBuyKind(HWND hDlg)
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclClient->pclVillageManager->pclVillageInfo[i])
		{
			SendDlgItemMessage(hDlg, IDC_COMBO_BUYSTOCK_KIND, CB_ADDSTRING, 0, (LPARAM)pclClient->pclVillageManager->pclVillageInfo[i]->szName);
		}
	}

}

// 매도 종목을 콤보 박스에 추가한다. 
void cltStockManager::SetComboSellKind(HWND hDlg)
{
	SI32 i;

	SI32 id = 1;
	if(id == 0)return ;

	for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		SI32 villageunique = pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
		if(villageunique)
		{
			if(villageunique >= 0)
			{
				SendDlgItemMessage(hDlg, IDC_COMBO_SELLSTOCK_KIND, CB_ADDSTRING, 0, (LPARAM)pclClient->pclVillageManager->pclVillageInfo[villageunique]->szName);
			}
		}
	}

}

// 선택한 주식 종목에 대한 최적 주문을 설정한다.
void cltStockManager::SetMostOrder(cltTradeOrder* pclSellOrder, cltTradeOrder* pclBuyOrder, GMONEY AverageSell , GMONEY AverageBuy )
{
#ifdef _SAFE_MEMORY
    clSellMostOrder.MemCpy(pclSellOrder);
	clBuyMostOrder.MemCpy(pclBuyOrder);
#else
	memcpy( clSellMostOrder, pclSellOrder, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
	memcpy( clBuyMostOrder, pclBuyOrder, sizeof(cltTradeOrder) * MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM );
#endif	
	siAverageSellMoney = AverageSell;
	siAverageBuyMoney = AverageBuy;
}

void cltStockManager::ShowFeeInfo( HWND hDlg )
{
	TCHAR buffer[256];

	float feerate = (float)clClientStrInfo.clStockStrInfo.clFee.GetFeeRate() / 10;

	StringCchPrintf( buffer, 256, TEXT("%3.1f%%"), feerate);

	SetDlgItemText( hDlg, IDC_EDIT_STOCKFEERATE, buffer );
}


// 주식 경매에 관한 처리를 주기적으로 한다
void cltStockManager::Action(cltTime* pcltime)
{
	if(pcltime == NULL) return ;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	if( bInitBuyOrder == FALSE || bInitSellOrder == FALSE ) return;

	//----------------------------------------
	// 
	//----------------------------------------
	cltServer *pclServer = (cltServer *)pclClient;

	if( pclServer->GetFrame() % ( 40 * 60 ) == 0 ) 
	{
		SetBidItem();
	}

	
	if( pclServer->GetFrame() % ( 40 * 3 ) == 0 )
	{
		int i, j = 0;

		// 경매 종료 처리
		for( i = 1; i < MAX_VILLAGE_NUMBER; ++i )
		{
			if( clStockBidVillageInfo[ i ].siTottalIncreaseStockAmount_ > 0 )
			{
				cltDate clEndDate;


				clEndDate.Set( &clStockBidVillageInfo[ i ].clBiddingDate_ );
				clEndDate.AddMonth( 6 );

				// 입찰 종료 시간이 경과하면,
				if( clEndDate.GetHourVary() - ((cltServer *)pclClient)->pclTime->clDate.GetHourVary() <= 0 ) 
				{
					CompletionStockBidVillage( i );
				}
			}
		}
	}
}

void cltStockManager::SetBidItem()
{

	int i, j = 0;
	int orderamount[ MAX_VILLAGE_NUMBER ];
	BOOL bSoldOut[ MAX_VILLAGE_NUMBER ];

	for( i = 0; i < MAX_VILLAGE_NUMBER; ++i ) {

		orderamount[ i ] = 0;
		bSoldOut[ i ] = TRUE;
	}

	for( i = 0; i < pclOrderManager->siMaxOrderNum; ++i )
	{
		// 사자 주문이 유효 하다면, 마을 총 주문수량값 증가 
		if( pclOrderManager->pclOrderBuy[ i ]->IsValid() ) {

			orderamount[ pclOrderManager->pclOrderBuy[ i ]->GetVillageUnique() ] += 
				pclOrderManager->pclOrderBuy[ i ]->siAmount;
		}

		// 시스템 주문이 남아 있는지 여부 
		if( pclOrderManager->pclOrderSell[ i ]->IsValid() ) {


			if( pclOrderManager->pclOrderSell[ i ]->siPersonID == 0 ) {
				
				bSoldOut[ pclOrderManager->pclOrderSell[ i ]->GetVillageUnique() ] = FALSE;
			}
			
		}

	}

	//cltDate clDate;

	for( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {

		// 유효한 마을이 아니면, skip
		if( pclClient->pclVillageManager->pclVillageInfo[i] == NULL ) continue;

		// 입찰 중인 마을이면, skip
		if( clStockBidVillageInfo[i].siVillageUnique_ != NULL ) continue;

		if( bSoldOut[ i ] == TRUE /*&& orderamount[ i ] >= 10*/ ) {

			// 증자 주식 수 결정 
			cltVillageInvestInfo* pInvestInfo = &pclClient->pclVillageManager->pclVillageInfo[i]->clVillageVary.clInvestInfo;		

			SI32 stockAmount = pInvestInfo->clInvestInfo[ 0 ].siStockNum;

			if( stockAmount == 0 ) continue;
			
			stockAmount = SI32 (stockAmount * 0.05);

			if( stockAmount < 20 ) stockAmount = 20;

			if( stockAmount > 500 ) stockAmount = 500;
			
/*			
			clDate.Set( &((cltServer *)pclClient)->pclTime->clDate );
			clDate.AddMonth( 3 );

			// 주식 경매 한지 3일이 지나지 았았으면, 다시 증자 할 수 없다
			if( clStockBidderManager[ i ].clBiddingDate.IsLaterMonthThan( &clDate ) == FALSE ) continue;
*/
			cltStrInfo *pStrInfo = pclClient->pclCityHallManager->GetStrInfo( i );
			if(pStrInfo == NULL)continue;

			if( pStrInfo->clCityhallStrInfo.siBeginningPrice < 5000 ) pStrInfo->clCityhallStrInfo.siBeginningPrice = 5000;

			sDBRequest_SetStockBidVillage clMsg( i, stockAmount, &((cltServer *)pclClient)->pclTime->clDate, pStrInfo->clCityhallStrInfo.siBeginningPrice );
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			bBidding = TRUE;
		}		

	}

}

void cltStockManager::InitStockBidVillage()
{
	for( int i = 0; i < MAX_VILLAGE_NUMBER; ++i ) {

		clStockBidVillageInfo[ i ].siVillageUnique_ = 0;
	}

	bBidding = FALSE;
}

void cltStockManager::InitStockBidder( SI32 villageUnique )
{
	if( villageUnique <= 0 || villageUnique >= MAX_VILLAGE_NUMBER ) return;

	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {

		clStockBidderManager[ villageUnique ].clStockBidder[ i ].clPerson.Init();
	}
}

void cltStockManager::InitClientStockBidder()
{
	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {

		clClientStockBidder[ i ].clPerson.Init();
	}
}

//void cltStockManager::SetStockBidVillage( SI32 villageUnique, cltDate *pclDate, SI32 stockAmount, SI32 beginningPrice )
//{
//	clStockBidVillageInfo[ villageUnique ].siVillageUnique_ = villageUnique;
//	clStockBidVillageInfo[ villageUnique ].clBiddingDate.Set( pclDate );
//	clStockBidVillageInfo[ villageUnique ].siStockAmount = stockAmount;
//	clStockBidVillageInfo[ villageUnique ].siBeginningPrice = beginningPrice;
//}
void cltStockManager::SetStockBidVillage( SI32 VillageUnique, SI32 TotalStockAmount, SI32 TottalIncreaseStockAmount, SI32 LeftIncreaseStockAmount, SI64 CurrentStockPrice, cltDate *pclDate )
{
	if( VillageUnique < 0 || VillageUnique >= MAX_VILLAGE_NUMBER )	return;

	clStockBidVillageInfo[ VillageUnique ].siVillageUnique_ = VillageUnique;
	clStockBidVillageInfo[ VillageUnique ].siTotalStockAmount_ = TotalStockAmount;
	clStockBidVillageInfo[ VillageUnique ].siTottalIncreaseStockAmount_ = TottalIncreaseStockAmount;
	clStockBidVillageInfo[ VillageUnique ].siLeftIncreaseStockAmount_ = LeftIncreaseStockAmount;
	clStockBidVillageInfo[ VillageUnique ].siCurrentStockPrice_ = CurrentStockPrice;
	clStockBidVillageInfo[ VillageUnique ].clBiddingDate_.Set( pclDate );
}


void cltStockManager::CompletionStockBidVillage( SI32 villageUnique )
{
	
	sDBRequest_CompleteStockBidVillage	clMsg( villageUnique );
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	
}

// 오래된 주문을 삭제하는등 액션을 취한다. 
void cltStockManager::Action(bool bdaychangedswitch)
{
	SI32 i = 0;
	static SI32 stockorderindex	= 0;

	//----------------------------------------
	// 매일 주문 정보를 정리한다.
	//----------------------------------------
/*	if(bdaychangedswitch)
	{
		// 전체 주문을 검색하여 날짜가 일정 시간 이상 경과한 주문은 삭제한다. 
		cltServer* pclserver = (cltServer* )pclClient;
		if(pclserver->pclTime)
		{
			
			SI32 curdate = pclserver->pclTime->clDate.GetDateVary();
			
			for(i = 0;i < pclOrderManager->siMaxOrderNum;i++)
			{
				cltTradeOrder clorder;
				clorder.Set(pclOrderManager->pclOrderBuy[i]);

				if(clorder.siPersonID <= 0)continue;

				if(clorder.siDate + 210 < curdate)	// 210일에 한번씩 
				{
				
					// 변경된 주문 정보를 DB로 보낸다. 
					// 가격이 더 이상 떨어지지 않으면, 삭제한다.
//					sDBRequest_ReduceOrderPrice clMsg(&clorder);
//					pclserver->SendDBMsg((sPacketHeader *)&clMsg);
				}
			}
		}
	}
	*/
}

