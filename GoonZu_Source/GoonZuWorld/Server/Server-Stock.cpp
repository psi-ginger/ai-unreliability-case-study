//---------------------------------
// 2003/6/3 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "MsgType-Stock.h"
#include "MsgRval-Define.h"

#include "..\Common\StockDistributionMgr\StockDistributionMgr.h"
#include "..\Common\StockDistributionMgr\StockDistributionObj.h"

#include "..\CommonLogic\Cityhall\Cityhall.h"
#include "../Client/NStructureDlg/Cityhall/NCityhallDlg.h"

#include "MsgType-Structure.h"	// 마을 파산 관련 메세지 처리

// 여각에 적절한 상대 주문을 요청한다. (클라이언트 사용자가 참고할 수 있게. ) 
void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKORDERINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StockOrderInfo* pclInfo = (cltGameMsgRequest_StockOrderInfo*)pclMsg->cData;

	// 메시지의 유효성 검사. 
	if(pclInfo->siVillageUnique <= 0 || pclInfo->siVillageUnique >= MAX_VILLAGE_NUMBER)return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0) return ;

	// 적절한 주문을 찾는다. 
	GMONEY price =0; 
	
	cltTradeOrder clOrder(personid, price, 1, 0, pclInfo->siVillageUnique);
	
	cltTradeOrder clSellMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];
	cltTradeOrder clBuyMostOrderList[MAX_VILLAGESTOCK_PRICE_INFO_LIST_NUM];

	GMONEY	siAverageSell = 0;
	GMONEY	siAverageBuy = 0;

	pclStockManager->pclOrderManager->GetMostSellBuyOrder(&clOrder, clSellMostOrderList, clBuyMostOrderList, &siAverageSell, &siAverageBuy);
	

	// 찾은 주문을 클라이언트로 보낸다. 
	cltGameMsgResponse_StockOrderInfo clInfo( clSellMostOrderList, clBuyMostOrderList , siAverageSell , siAverageBuy );
	cltMsg clMsg(GAMEMSG_RESPONSE_STOCKORDERINFO, sizeof(clInfo), (BYTE*)&clInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_CANCELSTOCKORDER(cltMsg* pclMsg, SI32 id)
{
	// PersonID를 구한다. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 주문을 취소하라는 요청을 DB로 보낸다.
	sDBRequest_CancelStockOrder clMsg(id, personid);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
}

// 사자 주문. 
void cltServer::DoMsg_GAMEMSG_REQUEST_BUYSTOCK(cltMsg* pclMsg, SI32 id)
{
	cltTradeOrder* pclStockOrder = (cltTradeOrder*)pclMsg->cData;

	// Person이 위치한 마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// [영훈] 초보자 마을 : 초보자 마을의 주식은 살수없다
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pclStockOrder->siVillageUnique )
		{
			return;
		}
	}
	// 한번에 주문할 수 있는 수량을 초과하면 실패. 
	if(pclStockOrder->siAmount > MAX_AMOUNT_PER_DEAL)return ;

	// PersonID를 업데이트 한다.
	pclStockOrder->siPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	// 주문 날짜를 확보한다. 
	pclStockOrder->siDate = pclTime->clDate.GetDateVary();


	// Person당 주문 개수가 적합한지 확인한다. 
	if( pclCM->CR[id]->GetStockOrderNumber() >= MAX_ORDER_PER_PERSON)return ;

	//#if defined(_CITYHALL_BANKRUPTCY)
	// 해당 마을이 파산 상태일 때는 주식 사자 주문을 올릴 수 없다.
	SI32 targetVillage = pclStockOrder->siVillageUnique;
	cltStrInfo *pTargetStrInfo = GetStrInfo( targetVillage, RANKTYPE_CITYHALL );
	if(pTargetStrInfo == NULL)return ;

	if( pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT )
	{
		// 주식 구입 대상 마을이 파산상태입니다.. => 구입 주문 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( targetVillage,  pTargetStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_STOCK_TRADE );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}
	//#endif

	// 주식 매수 주문을 넣는다. 
	SI32 retval	= 0;
	cltTradeOrder* pclorder = pclStockManager->pclOrderManager->FindOpositeOrder(ORDERMODE_BUY, pclStockOrder);
	if(pclorder)
	{
		// 적절한 반대 주문을 찾았다. 
		// 거래 수량과 가격 등을 정한후에 DB로 통보한다. 

		// 거래 가격. (두 주문의  평균 가격) 
		//GMONEY tradeprice		= (pclStockOrder->siPrice + pclorder->siPrice) / 2;


		//KHY - 1114 - '팔자' 주문의 가격으로 결정. - 작거나 같은 주문임은 FindOpositeOrder 서 찾음.
		//'매수'주문일때, 해당 마을의 '매도주문가격'이 '매수주문가격'보다 낮은 주식이 있다면 그 주식의 '매도주문가격'으로 사야한다.
		
		GMONEY tradeprice		= pclorder->siPrice;
		
		sDBRequest_TradeStock clMsg(id, ORDERMODE_BUY, villageunique, pclStockOrder, pclorder->siIndex, tradeprice);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	}
	else
	{
		// DB의 주문 버퍼에 저장해 놓을 것을 요청한다. 
		sDBRequest_OrderStock	clMsg(id, ORDERMODE_BUY, pclStockOrder);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_SELLSTOCK(cltMsg* pclMsg, SI32 id)
{
	cltTradeOrder* pclStockOrder = (cltTradeOrder*)pclMsg->cData;

	// Person이 위치한 마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// [영훈] 초보자 마을 : 초보자 마을의 주식은 구입할 수 없다
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pclStockOrder->siVillageUnique )
		{
			return;
		}
	}

	// PersonID를 업데이트 한다.
	pclStockOrder->siPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	// 주문 날짜를 확보한다. 
	pclStockOrder->siDate = pclTime->clDate.GetDateVary();

	// Person당 주문 개수가 적합한지 확인한다. 
	if( pclCM->CR[id]->GetStockOrderNumber() >= MAX_ORDER_PER_PERSON || pclStockOrder->siAmount <= 0 ) 
		return ;

	//[진성] 주식매도시 이미 매도된 주식외의 매도가 가능한 주식보다 많은 주식을 매도등록가능한 버그 수정. 2008-5-8
	//매도 주식의 수를 구한다.
	SI32 OrderStockAmount = 0;
	for(SI32 Count = 0; Count < MAX_MARKET_ORDER_PER_PERSON; ++Count)
	{
		if(pclCM->CR[id]->pclCI->clStock.clOrderInfo.clOrderUnit[Count].clOrder.GetVillageUnique() == pclStockOrder->siVillageUnique)
		{
			OrderStockAmount += pclCM->CR[id]->pclCI->clStock.clOrderInfo.clOrderUnit[Count].clOrder.siAmount;
		}
	}

	//[진성] 매도한 주식수를 전체 주식수에서 빼서 매도가능한 주식수면 등록하게 한다.
	if(pclStockOrder->siAmount > pclCM->CR[id]->pclCI->clStock.clInfo.GetStockAmount(pclStockOrder->siVillageUnique) - OrderStockAmount)
	{
		SendServerResponseMsg( 0, SRVAL_OVER_STOCK_ORDER, 0, 0, id );
		return;
	}
	
	// 주문 금액이 터무니 없으면 실패. 21억 이하까지
	if(pclStockOrder->siPrice <= 0 || pclStockOrder->siPrice > pclClient->GetGlobalValue("GV_StockTradeMoneyLimit") )
	{
		return ;
	}

	//#if defined(_CITYHALL_BANKRUPTCY)
	// 해당 마을이 파산 상태일 때는 주식 사자 주문을 올릴 수 없다.
	SI32 targetVillage = pclStockOrder->siVillageUnique;
	cltStrInfo *pTargetStrInfo = GetStrInfo( targetVillage, RANKTYPE_CITYHALL );
	if(pTargetStrInfo == NULL)return ;

	if( pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT )
	{
		// 주식 구입 대상 마을이 파산상태입니다.. => 구입 주문 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( targetVillage,  pTargetStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_STOCK_TRADE );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}
	//#endif

	// 주식 매도 주문을 넣는다. 
	SI32 orderref = -1;
	SI32 retval = 0;
	cltTradeOrder* pclorder = pclStockManager->pclOrderManager->FindOpositeOrder(ORDERMODE_SELL, pclStockOrder);
	if(pclorder)
	{
		// 거래 수량과 가격 등을 정한후에 DB로 통보한다. 

		// 거래 가격. (두 주문의  평균 가격) 
		//GMONEY tradeprice		= (pclStockOrder->siPrice + pclorder->siPrice) / 2;

		//KHY - 1114 - '팔자' 주문의 가격으로 결정. - 높거나 같은 주문임은 FindOpositeOrder 서 찾음.
		//'매도'주문일때, 해당 마을의 '매수주문가격'이 '매도주문가격'보다 높은 주식이 있다면 그 주식의 '매수주문가격'으로 사야한다. 
		GMONEY tradeprice		= pclorder->siPrice;

		sDBRequest_TradeStock clMsg(id, ORDERMODE_SELL, villageunique, pclStockOrder, pclorder->siIndex, tradeprice);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		// DB의 주문 버퍼에 저장해 놓을 것을 요청한다. 
		sDBRequest_OrderStock	clMsg(id, ORDERMODE_SELL, pclStockOrder);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETSTOCKFEE(cltMsg* pclMsg, SI32 id)
{

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	// 수수료 정보를 확보한다. 
	cltFee* pclfee = (cltFee*)pclMsg->cData;
	// 수수료가 적절한지 확인한다. 
	if(pclfee->IsValid(1000) == FALSE)return;

	//----------------------------------
	// 요청자가 여각행수인지 확인한다. 
	//----------------------------------
	cltSimpleRank clrank(RANKTYPE_STOCK, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 수수료 설정에 실패. 
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAILSETFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	//-----------------------------------
	// DB로 전장 정보를 보낸다. 
	//-----------------------------------
	// DB 로 보낸다.

	sDBRequest_SetStockFee clMsg(id, PersonID, villageunique, pclfee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}


void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKBIDVILLAGELIST(cltMsg* pclMsg, SI32 id)
{

	int count = 0;

	cltGameMsgResponse_StockBidVillageList sendMsg;

	for( int i = 0; i < MAX_VILLAGE_NUMBER; ++i ) {

		//if( pclStockManager->clStockBidVillageInfo[ i ].siVillageUnique != 0 ) {
		if( pclStockManager->clStockBidVillageInfo[ i ].siVillageUnique_ != 0 ) {

			if( count >= 0 && count < MAX_VILLAGE_NUMBER)
			{
				sendMsg.clStockBidVillageInfo[ count ].Set( &pclStockManager->clStockBidVillageInfo[ i ] );
				count++;
			}
		}

	}

	if( count > 0 ) {

		sendMsg.usCount = count;

		// 증자 마을 리스트를 전송한다
		cltMsg clMsg( GAMEMSG_RESPONSE_STOCKBIDVILLAGELIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}


}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKBIDDERLIST(cltMsg* pclMsg, SI32 id)
{

	SI32 villageUnique = pclMsg->siData32[ 0 ];
	if( villageUnique < 0 || villageUnique >= MAX_VILLAGE_NUMBER )		return;

	cltGameMsgResponse_StockBidder sendMsg;

	UI16 count = 0;

	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {

		if( pclStockManager->clStockBidderManager[ villageUnique ].clStockBidder[ i ].clPerson.GetPersonID() ) {

			if( count >= 0 && count < MAX_STOCKBIDDER_INVILLAGE)
			{
				sendMsg.clStockBidder[ count ].Set( 
					&pclStockManager->clStockBidderManager[ villageUnique ].clStockBidder[ i ] );

				count++;
			}
		}
	}

	sendMsg.usCount = count;

	// 증자 마을 리스트를 전송한다
	cltMsg clMsg( GAMEMSG_RESPONSE_STOCKBIDDERLIST, sizeof( sendMsg ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BIDSTOCK(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgRequest_BidStock* pclInfo = (cltGameMsgRequest_BidStock*)pclMsg->cData;

	if( pclInfo->siVillageUnique == 0 ) return;

	if( pclInfo->siStockMoney < LIMIT_STOCKMONEY )
		return;

	cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( pclInfo->siVillageUnique );

	if( pStrInfo == NULL ) return;

	cltStrInfo *pclStrInfo = pStrInfo;

	if( pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT )	
	{
		// 마을이 파산기 입니다.. => 증자 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( pclInfo->siVillageUnique,  pclStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_BIDSTOCK);
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}

	// DB요청
	sDBRequest_BidStock clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(), pclInfo->siVillageUnique, pclInfo->siStockMoney, &pclTime->clDate );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
	// 증자주식수량 보다 많은지 비교 
	//	if( pclInfo->siAmount > pclStockManager->clStockBidVillageInfo[ pclInfo->siVillageUnique ].siStockAmount ) {
	//		return;
	//	}


	//	// 시작가 보다 작은지 비교 
	//	if( pclInfo->clPrice.itMoney < pStrInfo->clCityhallStrInfo.siBeginningPrice ) {
	//		return;
	//	}

	//	// 이전 입찰자의 입찰 금액보다 10%이상인지 비교
	//	if( pclStockManager->clStockBidderManager[ pclInfo->siVillageUnique ].clStockBidder[0].clPerson.siPersonID )
	//	{
	//		if( pclInfo->clPrice.itMoney < pclStockManager->clStockBidderManager[ pclInfo->siVillageUnique ].clStockBidder[0].siPrice * 1.1 )
	//		{
	//			SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BIDDING_WRONGVALUE10,  0, 0, pclCM->CR[id]->GetCharUnique());
	//			return;
	//		}
	//	}



	/*	//------------------------------------------
	// 수수료를 결정한다. 
	//------------------------------------------		
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	SI64 fee = 0;

	SI64 money = (SI64)(pclInfo->siAmount * pclInfo->clPrice.itMoney);

	cltStrInfo* pclinfo = pclStockManager->GetStrInfo( villageunique );

	if( pclinfo ) 
	{
	// DB요청
	sDBRequest_BidStock clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &pclTime->clDate );

	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	*/

}

void cltServer::DoMsg_GAMEMSG_REQUEST_CANCELBIDSTOCK(cltMsg* pclMsg, SI32 id)
{



}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKBIDDATE(cltMsg* pclMsg, SI32 id)
{
	SI32 villageUnique = pclMsg->siData32[ 0 ];

	if( villageUnique <= 0 || villageUnique >= MAX_VILLAGE_NUMBER) return;

	//cltGameMsgResponse_StockBidDate sendMsg( villageUnique, &pclStockManager->clStockBidVillageInfo[ villageUnique ].clBiddingDate );
	cltGameMsgResponse_StockBidDate sendMsg( villageUnique, &pclStockManager->clStockBidVillageInfo[ villageUnique ].clBiddingDate_ );
	cltMsg clMsg( GAMEMSG_RESPONSE_STOCKBIDDATE, sizeof( cltGameMsgResponse_StockBidDate ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKDISTRIBUTIONORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StockDistributionOrder *pclinfo = (cltGameMsgRequest_StockDistributionOrder*)pclMsg->cData;

	if(pclCM->IsAlive(id) == false)return ;

	// 레벨 20 미만인 경우 참여 불가
	if ( pclCM->CR[id]->pclCI->clIP.GetLevel() <  FATHER_LEVEL)
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1) return ;

	if ( pclinfo->siVillageUnique < 1 )
	{
		return;
	}

	CStockDistributionObj *pObj = pclStockDistributionMgr->GetStockDistributionObj( pclinfo->siVillageUnique, pclinfo->bVillageWarStockDistribution );

	if ( pObj == NULL )
	{
		// 공모가 현재 진행중이지 않음.
		SendServerResponseMsg(0, SRVAL_STOCK_FAIL_DISTRIBUTIONSTOP,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if ( pclinfo->siAmount < 1 || pclinfo->siAmount > pObj->GetDistributionStockUnit() )
	{
		return;
	}

	// 적게도 배당 할 수 없다. ( 20 주라면 유저들은 꼭 20주만 사야 된다. 19주도 살 수 없게 막아 버렸음. )
	if ( pclinfo->siAmount != pObj->GetDistributionStockUnit() )
	{
		return;
	}

	if ( pObj->GetLeftDistributionStockNum() < 1 )
	{
		return;
	}

	if ( pclinfo->siAmount > pObj->GetLeftDistributionStockNum() )
	{
		pclinfo->siAmount = pObj->GetLeftDistributionStockNum();
	}

	GMONEY siStockPrice = pObj->GetDistributionStockPrice();
	if ( siStockPrice < 1 )
	{
		return;
	}
	GMONEY siBuyingPrice = pclinfo->siAmount * siStockPrice;

	// 돈 부족
	if ( pclCM->CR[id]->pclCI->clIP.GetMoney()  < siBuyingPrice )
	{
		return;
	}

	sDBRequest_StockDistributionOrder clMsg( 
		pclinfo->siVillageUnique,
		PersonID, 
		id, 
		pclinfo->siAmount,
		pObj->GetDistributionStockPrice(),
		pObj->IsVillageWarStockDistribution()
		);

	pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	
	pclDelMoneyStatistics->Add(DELMONEY_STOCK, siBuyingPrice);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BUYBIDSTOCK(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsAlive(id) == false)
		return ;

	cltGameMsgRequest_BuyBidStock *pclinfo = (cltGameMsgRequest_BuyBidStock*)pclMsg->cData;

	if( pclinfo == NULL )
		return;

	// 마을 유니크 체크
	if( pclinfo->siVillageUnique_ <= 0 || pclinfo->siVillageUnique_ >= MAX_VILLAGE_NUMBER )
		return;

	if( pclinfo->siType_ < 0 || pclinfo->siType_ > 1 )
		return;

	if( pclinfo->siType_ == 0 )
	{
		cltDate clEndDate;

		clEndDate.Set( &pclStockManager->clStockBidVillageInfo[ pclinfo->siVillageUnique_ ].clBiddingDate_ );
		clEndDate.AddMonth( 3 );

		// 입찰 종료 시간이 경과하면,
		if( clEndDate.GetHourVary() - pclTime->clDate.GetHourVary() <= 0 ) 
			return;
	}
	else if( pclinfo->siType_ == 1 )
	{
		cltDate clEndDate;

		clEndDate.Set( &pclStockManager->clStockBidVillageInfo[ pclinfo->siVillageUnique_ ].clBiddingDate_ );
		clEndDate.AddMonth( 6 );

		// 입찰 종료 시간이 경과하면,
		if( clEndDate.GetHourVary() - pclTime->clDate.GetHourVary() <= 0 ) 
			return;
	}

	// 수량 체크
	if( pclinfo->siAmount_ <=0 )
	{
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BUY_BIDSTOCK_EXCEEDAMOUNT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	SI64 Price = 0; // 한주 가격
	SI64 TotalPrice = 0;
	bool bBuySwitch = false;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();

	// 주주일때
	if( pclinfo->siType_ == 0 )
	{
		SI32 FindIndex = 0;

		for(SI32 i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			// 원하는 종목을 찾으면, 
			if(pclinfo->siVillageUnique_ == pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique)
			{
				FindIndex = i;
				break;
			}
		}

		if( pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[FindIndex].bBuySwitch == true )
		{
			SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BUY_BIDSTOCK_ALREADY,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}

		if( pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[FindIndex].siLimitBuyBidStockAmount < pclinfo->siAmount_ )
			return;

		Price = pclinfo->clPrice_.itMoney;// * 8 / 10;
		TotalPrice = Price * pclinfo->siAmount_ * 8 / 10;
		bBuySwitch = true;
	}
	else if( pclinfo->siType_ == 1)
	{
		Price = pclinfo->clPrice_.itMoney;
		TotalPrice = Price * pclinfo->siAmount_;
	}

	// 살려는 총 주식값이 내 소지금보다 많은지 체크
	if( TotalPrice > pclCM->CR[id]->pclCI->clIP.GetMoney() )
	{
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BUY_BIDSTOCK_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	SI64 fee = TotalPrice / 10;

	sDBRequest_BuyBidStock clMsg(id, PersonID, pclinfo->siType_, pclinfo->siVillageUnique_, pclinfo->siAmount_, Price, TotalPrice, bBuySwitch, fee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_STOCK, TotalPrice);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MYSTOCKINFO(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsAlive(id) == false)
		return ;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();

	if( PersonID < 0 )
		return;

	sDBRequest_MyStockInfo clMsg(id, PersonID );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKOFFERINGINFO(cltMsg* pclMsg, SI32 id )
{
	
	cltGameMsgResponse_StockOfferingInfo sendMsg;

	for( int i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		sendMsg.clStockOfferingInfo[ i ].siVillageUnique = pclStockOfferingManager->clStockInfo[ i ].siVillageUnique;
		sendMsg.clStockOfferingInfo[ i ].siTotalOffering = pclStockOfferingManager->clStockInfo[ i ].siTotalOffering;
		sendMsg.clStockOfferingInfo[ i ].siStockAmount = pclStockOfferingManager->clStockInfo[ i ].siStockAmount;
		sendMsg.clStockOfferingInfo[ i ].siPrice = pclStockOfferingManager->clStockInfo[ i ].siPrice;
		sendMsg.clStockOfferingInfo[ i ].siStatus = pclStockOfferingManager->clStockInfo[ i ].siStatus;
	}

	cltMsg clMsg( GAMEMSG_RESPONSE_STOCKOFFERINGINFO, sizeof( cltGameMsgResponse_StockOfferingInfo ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_STOCKOFFERINGORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_StockOfferingOrder* pclinfo = (cltGameMsgRequest_StockOfferingOrder*)pclMsg->cData;

	if(pclCM->IsAlive(id) == false)
		return ;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();

	if( PersonID < 0 )
		return;

	cltCharServer* pclCharSever = (cltCharServer*)pclCM->CR[id];
	if(pclCharSever == NULL)			return;

	//SI32 villageunique = pclMsg->siData32[ 0 ];
	SI32 villageunique = pclinfo->siVillageUnique;

	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER ) {
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDER,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	if(pclStockOfferingManager->clStockInfo[ villageunique ].siStatus != 1 ) {
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDER,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if(pclCM->CR[id]->pclCI->clIP.GetLevel() < 20)
	{
		SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDERLIMIT,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	sDBRequest_StockOfferingOrder clMsg( id, villageunique, PersonID,
		pclStockOfferingManager->clStockInfo[ villageunique ].siStockAmount,
		pclStockOfferingManager->clStockInfo[ villageunique ].siPrice,
		pclCharSever->GetSession()->GetIP(),
		pclinfo->szPrivateIP, pclinfo->szMacAddress);

	
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_EXCHANGESTOCK(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsAlive(id) == false)
		return ;

	cltGameMsgRequest_ExchangeStock *pclinfo = (cltGameMsgRequest_ExchangeStock*)pclMsg->cData;

    SI32 villageUnique = pclinfo->siVillageUnique;
	SI32 personID = pclCM->CR[id]->pclCI->GetPersonID();
	SI32 sellAmount = pclinfo->siSellAmount;
	SI32 bankType = pclinfo->siBankType;
	SI32 myStockAmount = 0;

	if( 0 >= villageUnique )
	{
		/// 마을값이 잘 못 된 경우라서 리턴
		return;
	}

	bool haveStock = false;
	for( SI32 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
	{
		if( villageUnique == pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique )
		{
			myStockAmount = pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[i].siAmount;
			haveStock = true;
			break;
		}
	}
	
	/// 팔려고 하는 주식이 주식공모 후 2년이 지나야 판매가 가능하다.

	if( false == haveStock )
	{
		/// 주식이 없다
		return;
	}

	if( sellAmount < 100 || ( 0 != sellAmount % 100 ) )
	{
		/// 100이하로는 팔 수 없다.
		return;
	}

	if( sellAmount > myStockAmount )
	{
		/// 가지고 있는 주식보다 많이 팔 수 없다.
		return;
	}

	sDBRequest_ExchangeFromStock clMsg( id, personID, pclinfo->szSecondPassWord, villageUnique,
		                                              sellAmount, bankType, pclinfo->szBankAccount );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GETBEGINNERVILLAGESTOCK( cltMsg* pclMsg, SI32 id )
{
	if ( pclCM->IsAlive(id) == false )
	{
		return;
	}

	cltCharServer* pclChar = (cltCharServer*)pclCM->CR[id];
	if ( pclChar == NULL )
	{
		return;
	}

	SI32 siPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( siPersonID < 0 )
	{
		return;
	}

	// 초보자 마을에 주식을 받을 수 있는 레벨이어야 한다
	if ( pclChar->pclCI->clIP.GetLevel() >= Const_BV_StockDividend_Level )
	{
		return;
	}

	// 초보자 마을 주민이어야 한다.
	if ( pclChar->pclCI->clBI.siHomeVillage != Const_Beginner_Village )
	{
		return;
	}

	// 초보자 마을의 주식이 있으면 안된다
	for ( SI32 siCount=0; siCount<MAX_STOCK_KIND_IN_ACOUNT; siCount++)
	{
		SI32 siFindVillageUnique	= pclChar->pclCI->clStock.clInfo.clUnit[siCount].siVillageUnique;
		SI32 siFindStockAmount		= pclChar->pclCI->clStock.clInfo.clUnit[siCount].siAmount;

		if ( Const_Beginner_Village == siFindVillageUnique )
		{
			if ( siFindStockAmount > 0 )
			{
				return;
			}
			break;
		}
	}

	sDBRequest_GiveStock clMsg( id, siPersonID, Const_Beginner_Village, 10 );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}
