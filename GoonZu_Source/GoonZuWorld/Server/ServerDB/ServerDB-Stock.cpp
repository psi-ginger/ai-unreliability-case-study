//---------------------------------
// 2003/8/5 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"
//-----------------------------------------
// Server
//-----------------------------------------
#include "../Server/Statistics/Statistics.h"


#include "..\..\..\DBManager\GameDBManager_World\DBMsg-CityHall.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Bank.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"

#include "..\Common\StockDistributionMgr\StockDistributionMgr.h"

#include "..\CommonLogic\MsgType-Person.h"


#include "MsgRval-Define.h"

#include "..\..\CommonLogic\CityHall\Cityhall.h"

#include "..\CommonLogic\MsgType-Stock.h"


// 주식 주문을 취소하라는 요청에 대한 응답
void cltServer::DoMsg_DBMSG_RESPONSE_CANCELSTOCKORDER(sPacketHeader* pPacket)
{
	sDBResponse_CancelStockOrder* pclMsg = (sDBResponse_CancelStockOrder*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 주식 주문 관리자에게 PersonID명의의 주문을 모두 취소하게 한다.
	pclStockManager->pclOrderManager->CancelOrder(pclMsg->siPersonID);

	// 클라이언트에게 취소된 내용을 보낸다. 
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) && pclCM->CR[id]->IsValidConnection())
	{
		// Person이 알고 있는 자신의 주문 내역을 초기화한다. 
		pclCM->CR[id]->pclCI->clStock.clOrderInfo.Init();
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCKORDER, true, 0);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 주식주문을 취소할 것을 요청.
			// 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_ORDER_CANCEL, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								 0, 0, 0, 0, 0, NULL, NULL);
		}
	}
}

// 주식 주문을 DB로 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETMANYSTOCKORDER(sPacketHeader* pPacket)
{
	SI32 i;

	sDBResponse_GetManyStockOrder* pclMsg = (sDBResponse_GetManyStockOrder*)pPacket;

	// id가 0인경우도 가능하다. 

	if( pclMsg->siOrderMode == ORDERMODE_SELL ) {
		pclStockManager->bInitSellOrder = TRUE;
		//너무 많이 찍혀서 보류
		//pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetManyStockOrder : ORDERMODE_SELL"));
	} else {
		pclStockManager->bInitBuyOrder = TRUE;
		//너무 많이 찍혀서 보류
		//pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetManyStockOrder : ORDERMODE_BUY"));
	}

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 서버의 주문 버퍼에 DB에서 보내온 주문 리스트를 넣는다. 
	for(i = 0;i < GET_MANY_STOCK_ORDER_NUM;i++)
	{
		pclStockManager->pclOrderManager->SetOrder(pclMsg->siOrderMode, &pclMsg->clTradeOrder[i]);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("pclStockManager->pclOrderManager->SetOrder(pclMsg->siOrderMode, &pclMsg->clTradeOrder[i])"));
	}
	

}

// 주식 거래의 결과를 얻어온 것을 처리한다. 
void cltServer::DoMsg_DBMSG_RESPONSE_TRADESTOCK(sPacketHeader* pPacket)
{
	sDBResponse_TradeStock* pclMsg = (sDBResponse_TradeStock*)pPacket;

	SI32 returnval = 0;

	if (pclMsg->siOrderMode == ORDERMODE_BUY ||	pclMsg->siResult == 1) 
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{			
			// 주식 매입 성공 로그[2007.9.4 손성웅]
			// 수정 : PCK
			SI32 charID			= pclMsg->usCharID;
			SI32 villageunique	= pclMsg->siVillageUnique;		
			GMONEY price		= pclMsg->siTradePrice;
			SI16 stockamount	= pclMsg->siTradeAmount;
						
			// 실재 거래된 총 액수
			GMONEY TotalPrice	= price * stockamount;
			
			// [로그추가 : 황진성 2007. 11. 1 주식 매입.]
			// param1 == 거래된 주식수
			// param2 == 하나 거래된 가격
			// param3 == 실재 거래된 총 액수
			// param4 == 남은 주식 수
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(charID))		pclchar = (cltCharServer*)pclCM->CR[charID];
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK,LOGCOMMAND_INDEX_STOCKBUY,
								0,pclchar, NULL, pclMsg->clDstOrder.siPersonID, 0, pclMsg->siBankMoney, NULL, 0, 0, 0, villageunique, 0, 0,
								stockamount, price, TotalPrice, pclMsg->siLeftAmount, 0, NULL, NULL);
		
							
			// [로그추가 : 황진성 2007. 11. 1 주식 판매.]
			// param1 == 거래된 주식수
			// param2 == 하나 거래된 가격
			// param3 == 실재 거래된 총 액수
			// param4 == 남은 주식 수
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK,LOGCOMMAND_INDEX_STOCKSELL,0,
								NULL, NULL, pclMsg->clOrgOrder.siPersonID, 0, pclMsg->siDstBankMoney, NULL, 0, 0, 0, villageunique, 0, 0,
								stockamount, price, TotalPrice, pclMsg->siDstLeftAmount, 0, NULL, NULL);
		}
	}

	switch(pclMsg->siResult)
	{
	case 0:
		return;
	case 1:	// 거래 성공. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_SUCCESSTRADE;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_SUCCESSTRADE;
		}
		break;
	case -1:// 사자의 전장 계좌가 없다. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_FAILNOACCOUNT;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_FAILNOACCOUNT;
		}
		break;
	case -2:	// 사자의 전장 계좌에 잔액이 충분하지 않다. 주문은 취소된다. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_FAILNOTENOUGHMONEY;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_FAILNOTENOUGHMONEY;
		}
		break;

	case -3:	// 제한된 종목 이상의 주식은 살 수 없다. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_FAILLIMITSTOCKKIND;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_FAILLIMITSTOCKKIND;
		}
		break;
	case -4:	// 주문중 일부는 처리되고 일부는 처리되지 않았다. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_HALFSUCCESS;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_HALFSUCCESS;
		}
		break;
	case -5:	// 상대 주문이 허구여서 한주도 거래가 체결되지 않았다. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_FAILLIAR;
		}
		else
		{
			returnval	= SRVAL_STOCK_BUY_FAILLIAR;
		}
		break;
	case -6:	// 팔자의 계좌가 유효하지 않아서 거래가 체결되지 않음. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_DSTSELLERNOACCOUNT;  // 나는 사려고 하는데 상대방의 계좌가 없음. 
		}
		else
		{
			returnval	= SRVAL_STOCK_DSTBUYERNOACCOUNT;	// 나는 팔려고 하는데 상대방의 계좌가 없음. 
		}
		break;


	default:
		MsgBox( TEXT("fd9jfd"), TEXT("11fld9j:%d"), pclMsg->siResult);
		return ;
	}

	//----------------------------------------------------------
	// DB로 부터 받은 주문 반대 정보를 서버에 업데이트 한다. 
	//----------------------------------------------------------
	SI32 mode = 0;
	if(pclMsg->siOrderMode == ORDERMODE_SELL)
	{
		mode = ORDERMODE_BUY;
	}
	else
	{
		mode = ORDERMODE_SELL;
	}

	// 반대주문 수량이 0이면 이 주문은 삭제되어야 할 주문이 된다. 따라서 VillageUnique를 0으로 설정. 
	// 거래되는 종목의 종류를 확보한다.
	SI32 tradevillageunique	= pclMsg->clDstOrder.siVillageUnique;

	if(pclMsg->clDstOrder.siAmount == 0)
	{
		pclMsg->clDstOrder.siPrice			= 0;
		pclMsg->clDstOrder.siVillageUnique	= 0;
	}

	pclStockManager->pclOrderManager->SetOrder(mode, &pclMsg->clDstOrder);

	// 주문자가 접속해 있다면, 변경된 주문 내용을 통보해준다. 
	SI32 tempid = pclCM->GetIDFromPersonID(pclMsg->clDstOrder.siPersonID);
	if(pclCM->IsValidID(tempid))
	{
		pclCM->CR[tempid]->pclCI->clStock.clOrderInfo.Set(mode, &pclMsg->clDstOrder);
		pclCM->CR[tempid]->SetUpdateSwitch(UPDATE_STOCKORDER, true, 0);
	}


	//------------------------------------------------------
	// 클라이언트로부터 받은 주문의 남은 내용을 확인한다. 
	//------------------------------------------------------

	// 모든 처리가 종료되었다.
	if(pclMsg->clOrgOrder.siAmount == 0)
	{
		// 실제로 거래된 주식이 있다면, 
		if(pclMsg->siTradeAmount)
		{
			// 거래된 가격을 업데이트 한다. 
			if(pclMsg->siTradePrice)
			{
				pclVillageManager->SetCurrentStockPrice(pclMsg->clOrgOrder.siVillageUnique, pclMsg->siTradePrice);
			}
		}
	}
	// 처리가 완료되지 않았다면 다시 서버의 주문 정보를 확인한다. 
	else
	{
		cltTradeOrder* pclorder = pclStockManager->pclOrderManager->FindOpositeOrder(pclMsg->siOrderMode, &pclMsg->clOrgOrder);
		// 서버의 주문 버퍼에서 적절한 반대 주문을 찾은 것이라면, 
		if(pclorder)
		{
			// 거래 수량과 가격 등을 정한후에 DB로 통보한다. 

			// 거래 가격. (두 주문의  평균 가격) 
			GMONEY tradeprice		= (pclMsg->clOrgOrder.siPrice + pclorder->siPrice) / 2;

			sDBRequest_TradeStock clMsg(pclMsg->usCharID, pclMsg->siOrderMode, pclMsg->siVillageUnique, &pclMsg->clOrgOrder, pclorder->siIndex, tradeprice);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		}
		// 적절한 반대 주문을 찾지 못했다면, 
		else
		{
			// DB의 주문 버퍼에 저장해 놓을 것을 요청한다. 
			sDBRequest_OrderStock	clMsg(pclMsg->usCharID, pclMsg->siOrderMode, &pclMsg->clOrgOrder);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
	}


	// 수수료를 확인한다. 
	cltStrInfo* pclstrinfo = pclStockManager->GetStrInfo(pclMsg->siVillageUnique);
	if(pclstrinfo == NULL)return ;

	GMONEY fee = pclstrinfo->clMarketStrInfo.clFee.CalcFee(pclMsg->siTradeAmount * pclMsg->siTradePrice);

	//---------------------------------------
	// 거래 내역에 대한 쪽지를 남긴다. 
	//---------------------------------------
	SI32 dstordermode;
	if(pclMsg->siOrderMode == ORDERMODE_SELL)
	{
		dstordermode = ORDERMODE_BUY;
	}
	else
	{
		dstordermode = ORDERMODE_SELL;
	}
	// 거래된 주식이 있다면 쪽지를 보낸다. (Org)
	if(pclMsg->siTradeAmount)
	{
		//----------------------------------------
		// 주식 거래 통계를 집계한다. 
		//----------------------------------------
		pclStatisticsManager->clDailyStatistics.siStockTradeNumber += pclMsg->siTradeAmount;
 
		// Org에 쪽지를 보낸다. 
		cltLetterMsg_StockTrade clletter1(tradevillageunique, pclMsg->siOrderMode, pclMsg->siTradeAmount, pclMsg->siLeftAmount, pclMsg->siTradePrice*pclMsg->siTradeAmount, fee);
		SI32 orgid = SendLetterMsg(pclMsg->clOrgOrder.siPersonID, (cltLetterHeader*)&clletter1);
		if(pclCM->IsValidID(orgid))
		{
			//-------------------------------------
			// 거래 요청자의 정보를 업데이트 한다. 
			//-------------------------------------

			// 잔여 주식수. 
			pclCM->CR[orgid]->pclCI->clStock.clInfo.Set(tradevillageunique, pclMsg->siLeftAmount);
			pclCM->CR[orgid]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

			// 전장 잔금. 
			pclCM->CR[orgid]->pclCI->clBank.SetMoney(pclMsg->siBankMoney);
			pclCM->CR[orgid]->SetUpdateSwitch(UPDATE_BANK, true, 0);

		}

		// Dst에 쪽지를 보낸다. 
		cltLetterMsg_StockTrade clletter2(tradevillageunique, dstordermode, pclMsg->siTradeAmount, pclMsg->siDstLeftAmount, pclMsg->siTradePrice*pclMsg->siTradeAmount, fee);
		SI32 dstid = SendLetterMsg(pclMsg->clDstOrder.siPersonID, (cltLetterHeader*)&clletter2);
		if(pclCM->IsValidID(dstid))
		{
			//------------------------------------
			// 거래 상대자의 정보를 업데이트 한다. 
			//-------------------------------------

			// 잔여 주식수. 
			pclCM->CR[dstid]->pclCI->clStock.clInfo.Set(tradevillageunique, pclMsg->siDstLeftAmount);
			pclCM->CR[dstid]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

			// 전장 잔금. 
			pclCM->CR[dstid]->pclCI->clBank.SetMoney(pclMsg->siDstBankMoney);
			pclCM->CR[dstid]->SetUpdateSwitch(UPDATE_BANK, true, 0);
		}

		//-----------------------------------
		// 판매자는 수수료를 지불한다. 
		//-----------------------------------
		// 지불해야 할 수수료가 있으면, 
		if(fee)
		{
			cltMoney clmoney;
			clmoney.itMoney = fee;

			if(pclMsg->siOrderMode == ORDERMODE_SELL)
			{
				sDBRequest_PayFee clinfo(orgid, pclMsg->clOrgOrder.siPersonID, pclMsg->siVillageUnique, RANKTYPE_STOCK, FEETYPE_STOCK_SELL, &clmoney);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);

				pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
			}
			else
			{
				sDBRequest_PayFee clinfo(dstid, pclMsg->clDstOrder.siPersonID, pclMsg->siVillageUnique, RANKTYPE_STOCK, FEETYPE_STOCK_SELL, &clmoney);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);

				pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
			}

		}

	}


	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;


	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_STOCK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}


}

// 주문 정보를 DB에 추가하라는 요청에 대한 응답을 처리한다.
void cltServer::DoMsg_DBMSG_RESPONSE_ORDERSTOCK(sPacketHeader* pPacket)
{
	sDBResponse_OrderStock* pclMsg = (sDBResponse_OrderStock*)pPacket;
	
	SI32 id = pclMsg->usCharID;

	SI32 returnval = 0;
	switch(pclMsg->siResult)
	{
	case 0:
		return;
	case 1:	// 거래 성공. 
		if(pclMsg->siOrderMode == ORDERMODE_BUY)
		{
			returnval	= SRVAL_STOCK_BUY_SUCCESSORDER;
		}
		else
		{
			returnval	= SRVAL_STOCK_SELL_SUCCESSORDER;
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 11. 1] // 주식매도, 매수 예약.
			// param1 == 어떤종류의 주문인가? 주식, 아이템, 혹은 길드주식.
			// param2 == 주문 고유의 식별자(인덱스) (0~약 16000).
			// param3 == 주문 수량. 
			// param4 == 주문 개당 가격. 
			// param5 == 시간 정보.

			cltCharServer* pclchar = NULL;
			if( pclCM->IsValidID(id) )		pclchar = (cltCharServer*)pclCM->CR[id];
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_ORDER, 
				returnval, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clOrder.siVillageUnique, 0, 0, 
				pclMsg->clOrder.siType,
				pclMsg->clOrder.siIndex,
				pclMsg->clOrder.siAmount,
				pclMsg->clOrder.siPrice,
				pclMsg->clOrder.siDate,
				NULL, NULL);
		}

		break;
	}

	// 서버의 주문 버퍼에 DB로부터 얻은 주문 정보를 추가한다. 
	pclStockManager->pclOrderManager->SetOrder(pclMsg->siOrderMode, &pclMsg->clOrder);


	// 클라이언트가 유효한 경우라면, 
	// 클라이언트로 이 주문 정보를 보낸다. 
	if(pclCM->IsValidID(id) && pclCM->CR[id]->IsValidConnection())
	{
		pclCM->CR[id]->pclCI->clStock.clOrderInfo.Set(pclMsg->siOrderMode, &pclMsg->clOrder);
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCKORDER, true, 0);

		if(returnval)
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_STOCK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
	}
}

// 주식 가격 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYSTOCKPRICE(sPacketHeader* pPacket)
{
	sDBResponse_ManyStockPrice* pclMsg = (sDBResponse_ManyStockPrice*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 i;

	// 주가를 얻어온다
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		pclVillageManager->SetCurrentStockPrice(i, pclMsg->siCurrentStockPrice[i]);
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyStockPrice"));
}

// 주식 거래 수수료 정보를 설정하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETSTOCKFEE(sPacketHeader* pPacket)
{
	sDBResponse_SetStockFee* pclMsg = (sDBResponse_SetStockFee*)pPacket;

	// 결과를 확인한다. 
	SI32 returnval = 0;
	switch(pclMsg->siResult)
	{
	case 0:
		return ;
	case 1:	// 수수료 변경 성공. 
		returnval	= SRVAL_STOCK_SUCCESSSETFEE;
		break;
	default:
		MsgBox( TEXT("vc984j"), TEXT("fdd:%d"), pclMsg->siResult);
	}

	// 서버의 해당 전장의 정보를 업데이트 한다. 
	cltStrInfo* pclinfo = pclStockManager->GetStrInfo(pclMsg->siVillageUnique);
	pclinfo->clStockStrInfo.clFee.Set(&pclMsg->clFee);

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_STOCK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	//KHY - 0727 - 주식 수수료 관련 버그 수정. 현재 마을에 있는 모든 유저 client의 건물 정보를 업데이트한다.
	cltMsg clNormalMsg(GAMEMSG_RESPONSE_STRINFO, 	pclinfo->GetTotalSize(), 	(BYTE*)pclinfo);
//	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);

	SI32 index = 0;
	while(id = pclCM->GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->GetCurrentVillageUnique() == pclMsg->siVillageUnique)
			{
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
			}
			
		}
	}

	////////////////////////////////////////////////////////////////	
}

// 증자 중인 마을 목록을 얻어온다 
void cltServer::DoMsg_DBMSG_RESPONSE_GETSTOCKBIDVILLAGELIST(sPacketHeader* pPacket)
{
	sDBResponse_GetStockBidVillageList *pclMsg = (sDBResponse_GetStockBidVillageList *)pPacket;

	cltStockBidVillageInfo *pStockBidVillageInfo;

	//SI32 beginningPrice;

	if( pclMsg->usCount > MAX_VILLAGE_NUMBER )		pclMsg->usCount = MAX_VILLAGE_NUMBER;

	for( int i = 0; i < pclMsg->usCount; ++i ) {

		//pStockBidVillageInfo = &pclMsg->clStockBidVillageInfo[ i ];

		//beginningPrice = pStockBidVillageInfo->siBeginningPrice;
		//if( beginningPrice < 5000 ) beginningPrice = 5000;
		//
		//pclStockManager->SetStockBidVillage(
		//	pStockBidVillageInfo->siVillageUnique,
		//	&pStockBidVillageInfo->clBiddingDate,
		//	pStockBidVillageInfo->siStockAmount,
		//	beginningPrice );
		pStockBidVillageInfo = &pclMsg->clStockBidVillageInfo[ i ];

		pclStockManager->SetStockBidVillage( pStockBidVillageInfo->siVillageUnique_,
			pStockBidVillageInfo->siTotalStockAmount_,
			pStockBidVillageInfo->siTottalIncreaseStockAmount_,
			pStockBidVillageInfo->siLeftIncreaseStockAmount_,
			pStockBidVillageInfo->siCurrentStockPrice_,
			&pStockBidVillageInfo->clBiddingDate_
			);

	}


	if( pclMsg->usCount > 0 ) pclStockManager->bBidding = TRUE;
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStockBidVillageList"));
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETSTOCKBIDDERLIST(sPacketHeader* pPacket)
{
	sDBResponse_GetStockBidderList *pclMsg = (sDBResponse_GetStockBidderList *)pPacket;
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	int count = pclMsg->usCount;
	if( count > MAX_STOCKBIDDER_INVILLAGE )		count = MAX_STOCKBIDDER_INVILLAGE;

	pclStockManager->InitStockBidder( pclMsg->siVillageUnique );

	if( count == 0 ) return;

	for( int i = 0; i < count; ++i ) {

		pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ i ].Set(
			&pclMsg->clStockBidder[ i ] );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStockBidderList"));

}


void cltServer::DoMsg_DBMSG_RESPONSE_SETSTOCKBIDVILLAGE(sPacketHeader* pPacket)
{
	//sDBResponse_SetStockBidVillage *pclMsg = (sDBResponse_SetStockBidVillage *)pPacket;

	//if( pclMsg->siResult == 1 ) {
	//	pclStockManager->SetStockBidVillage( pclMsg->siVillageUnique, &pclMsg->clDate, pclMsg->siStockAmount, pclMsg->siBeginningPrice );
	//}
}

//void cltServer::DoMsg_DBMSG_RESPONSE_BIDSTOCK(sPacketHeader* pPacket)
//{
//	sDBResponse_BidStock *pclMsg = (sDBResponse_BidStock *)pPacket;
//
//	SI32 id	= pclMsg->usCharID;
//	if(pclCM->IsValidID(id) == FALSE)return ;
//
//	// 결과를 확인한다. 
//	SI32 returnval = 0;
//
//	if( pclMsg->siResult != 1 )
//	{
//		returnval	= SRVAL_STOCK_FAIL_BIDDING;
//		if(returnval)
//		{
//			// 클라이언트로 서버의 응답값을 보낸다. 
//			SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
//		}
//		return;
//	}
//	else
//	{
//		returnval = SRVAL_STOCK_SUCCESS_BIDDING;
//
//		pclStockManager->SetStockBidVillage( pclMsg->clStockBidVillageInfo.siVillageUnique_,
//			pclMsg->clStockBidVillageInfo.siTotalStockAmount_,
//			pclMsg->clStockBidVillageInfo.siTottalIncreaseStockAmount_,
//			pclMsg->clStockBidVillageInfo.siLeftIncreaseStockAmount_,
//			pclMsg->clStockBidVillageInfo.siCurrentStockPrice_,
//			&pclMsg->clStockBidVillageInfo.clBiddingDate_ );
//		pclStockManager->bBidding = TRUE;
//
//		if(returnval)
//		{
//			// 클라이언트로 서버의 응답값을 보낸다. 
//			SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
//		}
//	}
//	
//}

void cltServer::DoMsg_DBMSG_RESPONSE_BIDSTOCK(sPacketHeader* pPacket)
{
	sDBResponse_BidStock *pclMsg = (sDBResponse_BidStock *)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	// 결과를 확인한다. 
	SI32 returnval = 0;

	if( pclMsg->siResult != 1 )
	{
		returnval	= SRVAL_STOCK_FAIL_BIDDING;
		if(returnval)
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
			
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 8] // 주식 증자 참여기록.
			//	// param2 == 직급 타입, 
			//	// param2 == 결과.
			//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_BID, 
			//						 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			//						 RANKTYPE_CITYHALL, returnval, 0, 0, 0, NULL, NULL);
			//}
		}
		return;
	}
	else
	{
		returnval = SRVAL_STOCK_SUCCESS_BIDDING;

		pclStockManager->SetStockBidVillage( pclMsg->clStockBidVillageInfo.siVillageUnique_,
			pclMsg->clStockBidVillageInfo.siTotalStockAmount_,
			pclMsg->clStockBidVillageInfo.siTottalIncreaseStockAmount_,
			pclMsg->clStockBidVillageInfo.siLeftIncreaseStockAmount_,
			pclMsg->clStockBidVillageInfo.siCurrentStockPrice_,
			&pclMsg->clStockBidVillageInfo.clBiddingDate_ );
		pclStockManager->bBidding = TRUE;

		if(returnval)
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 8] // 주식 증자 참여기록.
				// param2 == 직급 타입, 
				// param2 == 결과.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_BID, 
									 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									 RANKTYPE_CITYHALL, returnval, 0, 0, 0, NULL, NULL);
			}
		}
	}
}

//void cltServer::DoMsg_DBMSG_RESPONSE_COMPLETESTOCKBIDVILLAGE(sPacketHeader* pPacket)
//{
//	sDBResponse_CompleteStockBidVillage *pclMsg = (sDBResponse_CompleteStockBidVillage *)pPacket;
//
//	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
//
//	if( pclMsg->siResult == 1 ) {
//		// 증자 성공이면, 	
//
//		// 상단 보유자금 갱신
//		pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
//
//		// 상단 총 발행주식수 갱신
//		pclinfo->clCityhallStrInfo.siTotalStockAmount +=
//			pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siStockAmount;
//
//
//		// 상단 입출금 내역에 추가한다			
//		{
//			cltMoney clMoney;
//
//			clMoney.itMoney = pclMsg->clTotalMoney.itMoney - pclMsg->clTaxMoney.itMoney;
//
//			pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
//				INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );
//
//			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
//				INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );
//
//			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
//			
//
//			// 게임에서 빠져 나간돈 갱신 
//			clCurrencyInfo.clOutputMoney_clStockBidTax.Increase( &pclMsg->clTaxMoney );
//		}
//
//		// 시작가 갱신 
//		SI32 amount = 0;
//		SI32 beginningPrice;
//
//
//		// 입찰자들에게 결과 쪽지 보내기
//
//		for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {
//
//			if( pclMsg->clResultStockBidder[ i ].siPersonID == 0 ) continue;
//
//			cltLetterMsg_ResultBidStock clLetter( 
//				pclMsg->clResultStockBidder[ i ].bSuccessfulBid,
//				pclMsg->siVillageUnique,
//				pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siStockAmount,
//				&pclMsg->clResultStockBidder[ i ].clBidMoney,
//				pclMsg->clResultStockBidder[ i ].bExceedLimit );
//
//			SendLetterMsg( pclMsg->clResultStockBidder[ i ].siPersonID, (cltLetterHeader*)&clLetter);
//
//		}
//
////		beginningPrice = (int)( pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ i ].siPrice * 0.0097 ) * 100;
//
//		beginningPrice = (int)( pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ 0 ].siPrice * 0.007 ) * 100;
//		pclinfo->clCityhallStrInfo.siBeginningPrice = beginningPrice;
//
//		sDBRequest_SetBeginningPrice sendMsg( pclMsg->siVillageUnique, beginningPrice );
//		pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
//
//	} else {
//		// 증자 실패이면
//
//		// 시작가 하향 조정 
//		SI32 beginningPrice = (int)(pclinfo->clCityhallStrInfo.siBeginningPrice * 0.009) * 100;
//
//		if( beginningPrice < 5000 ) beginningPrice = 5000;
//
//		pclinfo->clCityhallStrInfo.siBeginningPrice = beginningPrice;
//
//		sDBRequest_SetBeginningPrice sendMsg( pclMsg->siVillageUnique, beginningPrice );
//		pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
//
//	}
//
//	pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siVillageUnique = 0;
//
//	pclStockManager->InitStockBidder( pclMsg->siVillageUnique );
//}

void cltServer::DoMsg_DBMSG_RESPONSE_COMPLETESTOCKBIDVILLAGE(sPacketHeader* pPacket)
{
	sDBResponse_CompleteStockBidVillage *pclMsg = (sDBResponse_CompleteStockBidVillage *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult != 1 ) 
		return;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

	if ( pclinfo == NULL )
		return ;
	if ( pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique] == NULL )
		return ;
	if ( pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique]->siVillageUnique <= 0 ||
		pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique]->siVillageUnique >= MAX_VILLAGE_NUMBER )
		return ;

	cltDate tempDate;
	tempDate.Set( 0, 0, 0, 0, 0 );
	pclStockManager->SetStockBidVillage( pclMsg->siVillageUnique, 0, 0, 0, 0, &tempDate);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// TEMP LEEKH --------------------------------------------------------------
		// [로그추가 : 황진성 2007. 10. 8] // 주식 증자 완료.
		// pszParam1 == 시간.
		TCHAR str[50] = {'\0', };
		StringCchPrintf(str, sizeof(str), TEXT("%d-%d-%d, %d:%d"),	
												tempDate.uiYear,
												tempDate.uiMonth,
												tempDate.uiDay,
												tempDate.uiHour,
												tempDate.uiMinute );

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_BID_COMPLETE, 
							 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							 pclMsg->clTaxMoney.itMoney, pclMsg->clTotalMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, str, NULL);
		// TEMP LEEKH --------------------------------------------------------------
	}

	/*	sDBResponse_CompleteStockBidVillage *pclMsg = (sDBResponse_CompleteStockBidVillage *)pPacket;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

	if ( pclinfo == NULL )
	return ;
	if ( pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique] == NULL )
	return ;
	if ( pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique]->siVillageUnique <= 0 ||
	pclVillageManager->pclVillageInfo[pclMsg->siVillageUnique]->siVillageUnique >= MAX_VILLAGE_NUMBER )
	return ;

	if( pclMsg->siResult == 1 ) {
	// 증자 성공이면, 	

	// 상단 보유자금 갱신
	pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );

	// 상단 총 발행주식수 갱신
	pclinfo->clCityhallStrInfo.siTotalStockAmount +=
	pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siStockAmount;


	// 상단 입출금 내역에 추가한다			
	{
	cltMoney clMoney;

	clMoney.itMoney = pclMsg->clTotalMoney.itMoney - pclMsg->clTaxMoney.itMoney;

	pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
	INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
	INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );


	// 게임에서 빠져 나간돈 갱신 
	clCurrencyInfo.clOutputMoney_clStockBidTax.Increase( &pclMsg->clTaxMoney );
	}

	// 시작가 갱신 
	SI32 amount = 0;
	SI64 beginningPrice;


	// 입찰자들에게 결과 쪽지 보내기

	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {

	if( pclMsg->clResultStockBidder[ i ].siPersonID == 0 ) continue;

	cltLetterMsg_ResultBidStock clLetter( 
	pclMsg->clResultStockBidder[ i ].bSuccessfulBid,
	pclMsg->siVillageUnique,
	pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siStockAmount,
	&pclMsg->clResultStockBidder[ i ].clBidMoney,
	pclMsg->clResultStockBidder[ i ].bExceedLimit );

	SendLetterMsg( pclMsg->clResultStockBidder[ i ].siPersonID, (cltLetterHeader*)&clLetter);

	}

	//		beginningPrice = (int)( pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ i ].siPrice * 0.0097 ) * 100;

	beginningPrice = (int)( pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ 0 ].siPrice * 0.007 ) * 100;
	pclinfo->clCityhallStrInfo.siBeginningPrice = beginningPrice;

	sDBRequest_SetBeginningPrice sendMsg( pclMsg->siVillageUnique, beginningPrice );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	} else {
	// 증자 실패이면

	// 시작가 하향 조정 
	SI64 beginningPrice = (SI64)(pclinfo->clCityhallStrInfo.siBeginningPrice * 0.009) * 100;

	if( beginningPrice < 5000 ) beginningPrice = 5000;

	pclinfo->clCityhallStrInfo.siBeginningPrice = beginningPrice;

	sDBRequest_SetBeginningPrice sendMsg( pclMsg->siVillageUnique, beginningPrice );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	}

	pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siVillageUnique = 0;

	pclStockManager->InitStockBidder( pclMsg->siVillageUnique );*/
}


void cltServer::DoMsg_DBMSG_RESPONSE_STOCKDISTRIBUTIONORDER(sPacketHeader* pPacket)
{
	sDBResponse_StockDistributionOrder *pclMsg = (sDBResponse_StockDistributionOrder*)pPacket;

	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	switch( pclMsg->siResult)
	{	
	case 1:
		break;
	case -2:		// 10종목 이상 보유.
		SendServerResponseMsg(0, SRVAL_STOCKGIVEFAIL_FULL,  MAX_STOCK_KIND_IN_ACOUNT, 0, pclCM->CR[id]->GetCharUnique());
		return;
	case -3:		
		SendServerResponseMsg(0, SRVAL_STOCK_FAIL_DISTRIBUTIONSTOP,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	default:
		return;
	}

	if ( pclMsg->siVillageUnique < 1 )
	{
		return;
	}

	// 배열 인덱스 체크
	if ( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if ( ! pclMsg->bVillageWarStockDistribution )
	{
		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

		if(pclinfo != NULL)
		{
			pclinfo->clCityhallStrInfo.siTotalStockAmount += pclMsg->siStockDistributionNum;
		}
	}

	// 주식 공모 관리자 업데이트
	// DB 로 부터 응답이 늦게 와서 공모가 끝났다면 IncreaseDistributionStockNum 이 함수에서는 아무것도 하지 않고 그냥 return 하게 되어 있다.
	pclStockDistributionMgr->IncreaseDistributionStockNum( pclMsg->siVillageUnique, pclMsg->siStockDistributionNum );

	// 캐릭터 ID 체크
	if(pclCM->IsValidID(id) == false)		return;

	// 주식을 업데이트 한다. 
	pclCM->CR[id]->pclCI->clStock.clInfo.Add(pclMsg->siVillageUnique, pclMsg->siStockDistributionNum);

	//업데이트 된 정보를 클라이언트로 통보한다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

	// 돈 업데이트
	pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->clPersonMoney.itMoney );

	// 주식 구입에 돈 지급했음을 알림
	cltGameMsgResponse_ChangeMoney clinfo(pclCM->CR[id]->pclCI->clIP.GetMoney(), 0, CHANGE_MONEY_REASON_NOTDEFINED);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// 주식 산돈 알림
	cltGameMsgResponse_StockDistributionOrder clinfo1( pclMsg->siVillageUnique, pclMsg->siStockDistributionNum, pclMsg->siStockPrice );
	cltMsg clMsg1(GAMEMSG_RESPONSE_STOCKDISTRIBUTIONORDER, sizeof(clinfo1), (BYTE*)&clinfo1);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	SI32 siTotalMoney = pclMsg->siStockDistributionNum * pclMsg->siStockPrice;	// PCK : 주식구입에 투자한 돈

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 9] // 주식 공모 구입.
		// param1 == 주식 수. 
		// param2 == 주문 개당 가격. 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_DISTRIBUTION, 
							 0, (cltCharServer*)pclCM->CR[id], NULL, 0, siTotalMoney, pclMsg->clPersonMoney.itMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							 pclMsg->siStockDistributionNum, pclMsg->siStockPrice, 0, 0, 0, NULL, NULL);
	}

	cltMoney clMoney;
	clMoney.Set( pclMsg->siStockDistributionNum * pclMsg->siStockPrice );
	clCurrencyInfo.clOutputMoney_Stock.Increase( &clMoney );

	//주식 공모 결과 로그 남김 [2007.08.04 손성웅]
	//주식 공모에 돈을 지불 하고 난뒤 로그에 남긴다.
	GMONEY price		= pclMsg->siStockPrice;
	SI32 villageunique	= pclMsg->siVillageUnique;
	SI16 stockamount	= pclMsg->siStockDistributionNum;
	
	// [로그추가 : 황진성 2007. 11. 1] // 주식 공모.
	// param1 == 주문 개당 가격. 
	// param2 == 주식 수. 
	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCKRESPONSE, 
						 0 ,(cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, pclMsg->clPersonMoney.itMoney, NULL, 0, 0, 0, villageunique, 0, 0,
						 price, stockamount, 0, 0, 0, NULL, NULL);
	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_MAXSTOCKDISTRIBUTIONNUM(sPacketHeader* pPacket)
{
	sDBResponse_MaxStockDistributionNum *pclMsg = (sDBResponse_MaxStockDistributionNum*)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if ( ! pclMsg->bVillageWarStockDistribution )
	{
		if ( pclMsg->uiRememberVillageUnique )
		{
			clServerStartInfo.siStockDistributionVillageUnique = pclMsg->siVillageUnique;
		}
		else
		{
			clServerStartInfo.siStockDistributionVillageUnique = 0;
		}
	}

	pclStockDistributionMgr->Set( pclMsg->siVillageUnique, pclMsg->siMaxStockDistributionNum, pclMsg->siStockDistributionNum, pclMsg->siStockPrice, pclMsg->bVillageWarStockDistribution, pclMsg->siVillageWarWinVillageUnique );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 9] // 최대 주식 공모.
		// param1 == 주식 가격.
		// param2 == 유저들에게 주는 최대 주식수, 
		// param3 == 각 유저들에게 줄수 있는 주식수, 
		// param4 == 공성에 의한 주식 공모인지 구분 짓기 위한 것,  
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_MAX_DISTRIBUTION_SET, 
							 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							 pclMsg->siStockPrice, pclMsg->siMaxStockDistributionNum, pclMsg->siStockDistributionNum, pclMsg->bVillageWarStockDistribution, 0, 
							 NULL, NULL);
	}
	return;
}

//#if defined(_CITYHALL_BANKRUPTCY)
void cltServer::DoMsg_DBMSG_RESPONSE_RETURNSTOCKBIDVILLAGE(sPacketHeader* pPacket)
{
	//sDBResponse_ReturnStockBidVillage *pclMsg = (sDBResponse_ReturnStockBidVillage *)pPacket;

	//cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

	//if( pclMsg->siResult == 1 ) {
	//	// 증자 입찰 보상 성공이면, 	

	//	// 상단 보유자금 갱신

	//	// 상단 총 발행주식수 갱신

	//	// 상단 입출금 내역에 추가한다			


	//	// 입찰자들에게 결과 쪽지 보내기

	//	for( int i = 0; i < MAX_STOCKBIDDER_INVILLAGE; ++i ) {

	//		if( pclMsg->clResultStockBidder[ i ].siPersonID == 0 ) continue;

	//		// 현재 시각으로 파산하여 증자 입찰 주식 보상 했음을 통지
	//		cltLetterMsg_VillageNameAndDate clletter( pclMsg->siVillageUnique, &(pclTime->clDate), cltLetterMsg_VillageNameAndDate::LETTERTYPE_RETURNSTOCKBIDVILLAGE );
	//		SendLetterMsg(pclMsg->clResultStockBidder[ i ].siPersonID, (cltLetterHeader*)&clletter );

	//		//---------------------------------
	//		// 클라이언트에게 개인별 전장 잔액 업뎃
	//		//---------------------------------
	//		SI32 id = pclCM->GetIDFromPersonID(pclMsg->clResultStockBidder[ i ].siPersonID);

	//		// 게임에 접속중이면 클라이언트로 통보한다. 
	//		if(id)
	//		{
	//			pclCM->CR[id]->pclCI->clBank.SetMoney((SI32)(pclMsg->clResultStockBidder[i].clPersonBankMoney.itMoney));

	//			pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
	//		}
	//	}

	//	// 시작가 갱신 필요 없음
	//	//SI32 amount = 0;
	//	//SI32 beginningPrice;
	//	//beginningPrice = (int)( pclStockManager->clStockBidderManager[ pclMsg->siVillageUnique ].clStockBidder[ 0 ].siPrice * 0.007 ) * 100;
	//	//pclinfo->clCityhallStrInfo.siBeginningPrice = beginningPrice;

	//	//sDBRequest_SetBeginningPrice sendMsg( pclMsg->siVillageUnique, beginningPrice );
	//	//pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);


	//} else {
	//	// 증자 입찰 보상 실패면, 	

	//	// 시작가 하향 조정 

	//}

	//pclStockManager->clStockBidVillageInfo[ pclMsg->siVillageUnique ].siVillageUnique = 0;

	//pclStockManager->InitStockBidder( pclMsg->siVillageUnique );
}

void cltServer::DoMsg_DBMSG_RESPONSE_BUYBIDSTOCK(sPacketHeader* pPacket)
{
	sDBResponse_BuyBidStock *pclMsg = (sDBResponse_BuyBidStock *)pPacket;

	if( pclMsg == NULL )
		return;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == false )
		return;

	if( pclMsg->siResult != 1 )
	{
		if( pclMsg->siResult == -100 )
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BUY_BIDSTOCK_NOTBUYER,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		else
		{
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_FAIL_BUY_BIDSTOCK,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		return;
	}

	SI32 tempVillageUnique = pclMsg->clStockBidVillageInfo.siVillageUnique_;

	if( tempVillageUnique <=0 )
		return;

	// 배열 인덱스 체크
	if( tempVillageUnique < 0 || tempVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siAmount <= 0 )
		return;

	if( pclMsg->clStockBidVillageInfo.siCurrentStockPrice_ <=0 )
		return;

	SI64 tempTotalStockPrice = pclMsg->siTotalPrice;

	if( tempTotalStockPrice <= 0)
		return;

	// 소지금액에서 체크하자~
	if( tempTotalStockPrice > pclCM->CR[id]->pclCI->clIP.GetMoney() )
		return;

	// 소지금액 업뎃
	pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siBuyStockLeftMoney );

	// 내 주식을 업데이트 한다. 
	pclCM->CR[id]->pclCI->clStock.clInfo.Add(pclMsg->clStockBidVillageInfo.siVillageUnique_, pclMsg->siAmount );
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

	// 주식 구입에 돈 지급했음을 알림
	cltGameMsgResponse_ChangeMoney clinfo(pclCM->CR[id]->pclCI->clIP.GetMoney(), 0, CHANGE_MONEY_REASON_BUYBIDSTOCK);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// 주식 산돈 알림
	GMONEY tempStockMoney = 0;
	if( pclMsg->siBuyMode == 0 )
		tempStockMoney = pclMsg->clStockBidVillageInfo.siCurrentStockPrice_ * 8 / 10;
	
	else if( pclMsg->siBuyMode == 1 )
		tempStockMoney = pclMsg->clStockBidVillageInfo.siCurrentStockPrice_;
	
	cltGameMsgResponse_StockDistributionOrder clinfo1( tempVillageUnique, pclMsg->siAmount, tempStockMoney );
	cltMsg clMsg1(GAMEMSG_RESPONSE_STOCKDISTRIBUTIONORDER, sizeof(clinfo1), (BYTE*)&clinfo1);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 9] // 주식 공모 구입.
	//	// param1 == 주식 수. 
	//	// param2 == 주문 개당 가격. 
	//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_DISTRIBUTION, 
	//						 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, tempVillageUnique, 0, 0, 
	//						 pclMsg->siAmount, tempStockMoney, 0, 0, 0, NULL, NULL);
	//}

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( tempVillageUnique );

	if ( pclinfo == NULL )
		return ;
	if ( pclVillageManager->pclVillageInfo[tempVillageUnique] == NULL )
		return ;
	if ( pclVillageManager->pclVillageInfo[tempVillageUnique]->siVillageUnique <= 0 ||
		pclVillageManager->pclVillageInfo[tempVillageUnique]->siVillageUnique >= MAX_VILLAGE_NUMBER )
		return ;

	// 증자 성공이면, 	
	// 상단 이익금을 갱신한다.
	pclinfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clCityhallMoney );

	// 상단 총 발행주식수 갱신
	pclinfo->clCityhallStrInfo.siTotalStockAmount += pclMsg->siAmount;


	// 상단 입출금 내역에 추가한다			
	{
		cltMoney clMoney;

		clMoney.itMoney = pclMsg->siTotalPrice - pclMsg->siFee;

		pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg( tempVillageUnique, RANKTYPE_CITYHALL, 
			INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH --------------------------------------------------------------
		//	// [로그추가 : 황진성 2007. 9. 20] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
		//	TCHAR pszCharParam1[50] = {'\0', };
		//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
		//																pclTime->clDate.uiYear,
		//																pclTime->clDate.uiMonth,
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour,
		//																pclTime->clDate.uiMinute);
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, tempVillageUnique, 0, 0, 
		//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, pszCharParam1, NULL);
		//	// TEMP LEEKH --------------------------------------------------------------
		//}

		// 게임에서 빠져 나간돈 갱신
		clMoney.itMoney = pclMsg->siFee;
		clCurrencyInfo.clOutputMoney_clStockBidTax.Increase( &clMoney );
	}

	pclStockManager->SetStockBidVillage( pclMsg->clStockBidVillageInfo.siVillageUnique_,
		pclMsg->clStockBidVillageInfo.siTotalStockAmount_,
		pclMsg->clStockBidVillageInfo.siTottalIncreaseStockAmount_,
		pclMsg->clStockBidVillageInfo.siLeftIncreaseStockAmount_,
		pclMsg->clStockBidVillageInfo.siCurrentStockPrice_,
		&pclMsg->clStockBidVillageInfo.clBiddingDate_ );

	int count = 0;

	cltGameMsgResponse_StockBidVillageList sendMsg;

	for( int i = 0; i < MAX_VILLAGE_NUMBER; ++i ) {

		if( pclStockManager->clStockBidVillageInfo[ i ].siVillageUnique_ != 0 ) {

			if( count >= MAX_VILLAGE_NUMBER )		continue;
			sendMsg.clStockBidVillageInfo[ count ].Set( &pclStockManager->clStockBidVillageInfo[ i ] );
			count++;
		}

	}

	if( count > 0 )
	{

		sendMsg.usCount = count;

		// 증자 마을 리스트를 전송한다
		cltMsg clMsg( GAMEMSG_RESPONSE_STOCKBIDVILLAGELIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	sDBRequest_InvestList clMsgInvest(tempVillageUnique);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsgInvest);

	// 투자자 정보를 구한다. 
	cltVillageInvestInfo* pMayorInfo = &pclVillageManager->pclVillageInfo[tempVillageUnique]->clVillageVary.clInvestInfo;

	// 어떤 마을의 정보인지를 업데이트 한다. 
	pMayorInfo->siVillageUnique = tempVillageUnique;

	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg2(GAMEMSG_RESPONSE_INVESTLIST, sizeof(cltVillageInvestInfo), (BYTE*)pMayorInfo);

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

	/*	if( pclMsg->clStockBidVillageInfo.siLeftIncreaseStockAmount_ == 0 )
	{
	sDBRequest_CompleteStockBidVillage	clMsg( tempVillageUnique );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
	*/
	// 클라이언트로 서버의 응답값을 보낸다. 
	SendServerResponseMsg(RANKTYPE_STOCK, SRVAL_STOCK_SUCCESS_BUY_BIDSTOCK,  0, 0, pclCM->CR[id]->GetCharUnique());

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 8] // 증자주식 사기
		// param1 == 주문수량.
		// param2 == 수수료.
		// param3 == 총 거래 금액.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_BID_BUY, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->siAmount, pclMsg->siFee, pclMsg->siTotalPrice, 0, 0, 
										NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_MYSTOCKINFO(sPacketHeader* pPacket)
{
	sDBResponse_MyStockInfo *pclMsg = (sDBResponse_MyStockInfo *)pPacket;

	if( pclMsg == NULL )
		return;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == false )
		return;

	if( pclMsg->siResult != 1 )
	{
		return;
	}

	// 내 주식을 업데이트 한다. 
	pclCM->CR[id]->pclCI->clStock.clInfo.Set( &pclMsg->clInfo ); 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCK, true, 0);
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETSTOCKOFFERINGINFO( sPacketHeader* pPacket )
{
	sDBResponse_GetStockOfferingInfo *pclMsg = (sDBResponse_GetStockOfferingInfo *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siVillageUnique = pclMsg->siVillageUnique;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siTotalOffering = pclMsg->siTotalOffering;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStockAmount = pclMsg->siStockAmount;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siPrice = pclMsg->siPrice;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStatus = pclMsg->siStatus;
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStockOfferingInfo"));

}

void cltServer::DoMsg_DBMSG_RESPONSE_SETSTOCKOFFERING( sPacketHeader* pPacket )
{
	sDBResponse_SetStockOffering *pclMsg = (sDBResponse_SetStockOffering *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siVillageUnique = pclMsg->siVillageUnique;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siTotalOffering = pclMsg->siTotalOffering;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStockAmount = pclMsg->siStockAmount;
	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siPrice = pclMsg->siPrice;

	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStatus = 1;
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 8] // 주식 공모 마을 설정 (공모 청약 시작)
		// param1 == 총 공모 주식 수량, param2 == 1인당 배당 주식 수, param3 == 1인당 배당 주식 가격
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_SET, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->siTotalOffering, pclMsg->siStockAmount, pclMsg->siPrice, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_ENDSTOCKOFFERING( sPacketHeader* pPacket )
{
	sDBResponse_EndStockOffering *pclMsg = (sDBResponse_EndStockOffering *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStatus = 2;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 8] // 주식 공모 청약 종료. 
		// 
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_END, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										0, 0, 0, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_STOCKOFFERINGORDER( sPacketHeader* pPacket )
{
	sDBResponse_StockOfferingOrder *pclMsg = (sDBResponse_StockOfferingOrder *)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == false ) return;

	switch( pclMsg->siResult )
	{
	case 1:
		{
			// 돈 업데이트
			pclCM->CR[id]->pclCI->clIP.DecreaseMoney( pclMsg->siMoney );
			//pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonMoney );

			// 주식 구입에 돈 지급했음을 알림
			cltGameMsgResponse_ChangeMoney clinfo(pclCM->CR[id]->pclCI->clIP.GetMoney(), 0, CHANGE_MONEY_REASON_NOTDEFINED);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			pclDelMoneyStatistics->Add(DELMONEY_STOCK, pclMsg->siMoney);
			
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 8] // 주식 공모 청약 주문.
				// param1 == 주식수.
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_ORDER, 
												0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siMoney, pclMsg->siPersonMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
												pclMsg->siStockAmount, 0, 0, 0, 0, NULL, NULL);
			}

			SendServerResponseMsg( RANKTYPE_STOCK, SRVAL_STOCKOFFERING_SUCCESS_ORDER,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		break;

	case 2:	// 소지금 부족
		SendServerResponseMsg( RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDER,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;

	case 3: // 10개 이상 주식 보유
		SendServerResponseMsg( RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDER,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;

	case 4: // 해당 마을 주식 공모에 이미 신청 했음
		SendServerResponseMsg( RANKTYPE_STOCK, SRVAL_STOCKOFFERING_FAIL_ORDERALREADY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETSTOCKOFFERINGORDERLIST( sPacketHeader* pPacket )
{
	sDBResponse_GetStockOfferingOrderList *pclMsg = (sDBResponse_GetStockOfferingOrderList *)pPacket;


	// 여러개의 오더 리스트를 받다가, 마지막 데이터이면( villageunique == -1 )
	if( pclMsg->siVillageUnique == -1 )
	{
		pclClient->pclLog->FilePrint(TEXT("Config\\StockLog.log"), TEXT("ORDERLIST..(last) %d"), pclStockOfferingManager->siCount );

		int i;
		SI32 villageunique = pclStockOfferingManager->siVillageUnique;
		if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER )		return;

		// 섞는다.
		if( IsCountrySwitch( Switch_LotteryStockRandom ) )
		{
			if( pclStockOfferingManager->siCount >= 2)
			{
				for ( i=0; i<pclStockOfferingManager->siCount; i++)
				{
					SI32 siChoose = rand() % pclStockOfferingManager->siCount;
					SI32 siDistance = (rand() % pclStockOfferingManager->siCount) + 1;

					SI32 siPos1 = siChoose;
					SI32 siPos2 = (siChoose + siDistance) % pclStockOfferingManager->siCount;

					if( siPos1 < 0 || siPos1 >= 2000 )		continue;
					if( siPos2 < 0 || siPos2 >= 2000 )		continue;

					cltStockOfferingOrderInfo temp;

					if(siPos1 != siPos2)
					{
						temp.Set(&pclStockOfferingManager->clStockOrder[siPos1]);
						pclStockOfferingManager->clStockOrder[siPos1].Set(&pclStockOfferingManager->clStockOrder[siPos2]);
						pclStockOfferingManager->clStockOrder[siPos2].Set(&temp);
					}
				}
			}
		}

		// 앞에서 부터 당첨 시킨다. (위에서 섞었으므로 결국은 랜덤이다.)
		SI32 totalAmount = pclStockOfferingManager->clStockInfo[ villageunique ].siTotalOffering;
		SI32 offerAmount = 0;

		for( i = 0; i < pclStockOfferingManager->siCount; ++i )
		{
			if( i >= 2000 )		continue;

			cltStockOfferingOrderInfo *pOrder = &pclStockOfferingManager->clStockOrder[ i ];

			SI32 id = pclCM->GetIDFromPersonID( pOrder->siPersonID );
			
			bool bWinning = false;
			
			// 게임에 접속중이고, 배당 가능한 주식이 남아있다면,
			if( id && offerAmount < totalAmount )
				bWinning = true;

			// 이전에 당첨된 IP, MACADDRESS에 있으면 당첨되지 않는다. - 중복 허용개수를 설정할 수 있다.
			if( IsCountrySwitch( Switch_LotteryStockRandom ) )
			{
				// 당첨되는 경우에만 체크한다. - 불필요한 CPU점유 삭제
				if( bWinning == true)
				{
					if( pclStockOfferingManager->CheckStockOfferingOrderDuplicate(pOrder->szIP, pOrder->szPrivateIP, pOrder->szMacAddress) )
						bWinning = false;
				}
			}
			
			//if( id && offerAmount < totalAmount )
			if( bWinning )
			{				
				offerAmount += pOrder->siStockAmount;

				sDBRequest_ExecuteStockOfferingOrder clMsg( 
					id,
					1, 
					villageunique, 
					pOrder->siPersonID,
					pOrder->siStockAmount,
					pOrder->siMoney,
					pOrder->szIP,
					pOrder->szPrivateIP,
					pOrder->szMacAddress );

				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );		
			}
			else
			{
				sDBRequest_ExecuteStockOfferingOrder clMsg( 
					id,
					2, 
					villageunique, 
					pOrder->siPersonID,
					pOrder->siStockAmount,
					pOrder->siMoney,
					pOrder->szIP,
					pOrder->szPrivateIP,
					pOrder->szMacAddress);

				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
			}
		}

		// 추첨 완료..
		pclStockOfferingManager->bLottery = FALSE;
		pclStockOfferingManager->siCount = 0;

		pclStockOfferingManager->siVillageUnique = 0;
		pclStockOfferingManager->siDuplicateCount = 0;

		sDBRequest_ResetStockOfferingOrderList clMsg( villageunique );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

		sDBRequest_GetStrInfo clMsg2(0, villageunique, RANKTYPE_CITYHALL );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);			

		cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
		cltMoney clMoney;

		clMoney.itMoney = offerAmount * pclStockOfferingManager->clStockInfo[ villageunique ].siPrice;

		pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg( villageunique, RANKTYPE_CITYHALL, 
			INOUTCODE_STOCK, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH --------------------------------------------------------------
		//	// [로그추가 : 황진성 2007. 9. 20] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
		//	TCHAR pszCharParam1[50] = {'\0', };
		//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
		//																pclTime->clDate.uiYear,
		//																pclTime->clDate.uiMonth,
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour,
		//																pclTime->clDate.uiMinute);
		//
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, villageunique, 0, 0, 
		//									clMoney.itMoney, pStrInfo->clCityhallStrInfo.clProfitMoney.itMoney, 0, 0, 0, 
		//									pszCharParam1, NULL);
		//	// TEMP LEEKH --------------------------------------------------------------
		//}
	}
	else
	{
		int i = pclStockOfferingManager->siCount;

		if( i < 2000 ) {
		
			pclStockOfferingManager->clStockOrder[ i ].siVillageUnique = pclMsg->siVillageUnique;
			pclStockOfferingManager->clStockOrder[ i ].siPersonID = pclMsg->siPersonID;
			pclStockOfferingManager->clStockOrder[ i ].siStockAmount = pclMsg->siStockAmount;
			pclStockOfferingManager->clStockOrder[ i ].siMoney = pclMsg->siMoney;
			StringCchCopy(pclStockOfferingManager->clStockOrder[ i ].szIP,			sizeof(pclStockOfferingManager->clStockOrder[ i ].szIP),		 pclMsg->szIP);
			StringCchCopy(pclStockOfferingManager->clStockOrder[ i ].szPrivateIP,	sizeof(pclStockOfferingManager->clStockOrder[ i ].szPrivateIP),  pclMsg->szPrivateIP);
			StringCchCopy(pclStockOfferingManager->clStockOrder[ i ].szMacAddress,	sizeof(pclStockOfferingManager->clStockOrder[ i ].szMacAddress), pclMsg->szMacAddress);
			
			pclStockOfferingManager->siCount = i + 1;

			pclClient->pclLog->FilePrint(TEXT("Config\\StockLog.log"), TEXT("ORDERLIST..(make) %d, IP:%s(%s), MAC:%s"), pclMsg->siPersonID, pclMsg->szIP, pclMsg->szPrivateIP, pclMsg->szMacAddress );
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_EXECUTESTOCKOFFERINGORDER( sPacketHeader* pPacket )
{
	sDBResponse_ExecuteStockOfferingOrder *pclMsg = (sDBResponse_ExecuteStockOfferingOrder *)pPacket;

	pclClient->pclLog->FilePrint(TEXT("Config\\StockLog.log"), TEXT("excute result %d, type %d, personid %d, IP:%s(%s), MAC:%s"), pclMsg->siResult, pclMsg->siType, pclMsg->siPersonID, pclMsg->szIP, pclMsg->szPrivateIP, pclMsg->szMacAddress );

	if( pclMsg->siResult == 1 )
	{
		if( pclMsg->siType == 1)  // 당첨
		{
			SI32 id = pclMsg->usCharID;

			if(pclCM->IsValidID(id) )
			{		
				//주식을 업데이트 한다
				pclCM->CR[id]->pclCI->clStock.clInfo.Add(pclMsg->siVillageUnique, pclMsg->siStockAmount);

				//업데이트 된 정보를 클라이언트로 통보한다. 
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

				//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				//{
				//	// [로그추가 : 황진성 2007. 10. 8] // 주식 청약 주문 실행.
				//	// param1 == 타입. 
				//	// param2 == 개수.
				//	cltServer * pclServer = (cltServer*)pclClient;
				//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_ORDER_EXECUTE, 
				//									0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siMoney, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				//									pclMsg->siType, pclMsg->siStockAmount, 0, 0, 0, NULL, NULL);
				//}
			}

			cltLetterMsg_StockOfferingResult clletter( pclMsg->siType, pclMsg->siVillageUnique, pclMsg->siStockAmount, pclMsg->siMoney );
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclMsg->siPersonID, (cltLetterHeader*)&clletter);

		} 
		else
		{				  // 미당첨

			SI32 id = pclMsg->usCharID;

			if(pclCM->IsValidID(id) )
			{		
				//------------------------------
				// 계좌 정보 업데이트. 
				//------------------------------
				// 계좌 정보를 확보한다. 
				pclCM->CR[id]->pclCI->clBank.IncreaseMoney(pclMsg->siMoney);

				// 클라이언트로 정보를 보낸다. 
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
			}

			cltLetterMsg_StockOfferingResult clletter( pclMsg->siType, pclMsg->siVillageUnique, pclMsg->siStockAmount, pclMsg->siMoney );
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclMsg->siPersonID, (cltLetterHeader*)&clletter);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 8] // 주식 청약 주문 실행.
			//	// param1 == 타입. param2 == 개수.
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	cltCharServer* pclchar = NULL;
			//	if( pclCM->IsValidID(id) )		pclchar = (cltCharServer*)pclCM->CR[id];
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_ORDER_EXECUTE, 
			//									0, pclchar, NULL, 0, pclMsg->siMoney, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			//									pclMsg->siType, pclMsg->siStockAmount, 0, 0, 0, NULL, NULL);
			//}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 주식 청약 주문 실행.
			// param2 == 개수.
			cltServer * pclServer = (cltServer*)pclClient;
			cltCharServer* pclchar = NULL;
			SI32 id = pclMsg->usCharID;
			if( pclCM->IsValidID(id) )		pclchar = (cltCharServer*)pclCM->CR[id];
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_ORDER_EXECUTE, 
				pclMsg->siType, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siMoney, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				0, pclMsg->siStockAmount, 0, 0, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_RESETSTOCKOFFERINGORDERLIST( sPacketHeader* pPacket )
{
	sDBResponse_ResetStockOfferingOrderList *pclMsg = (sDBResponse_ResetStockOfferingOrderList *)pPacket;

	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	pclStockOfferingManager->clStockInfo[ pclMsg->siVillageUnique ].siStatus = 0;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 8] // 주식 청약 주문 리셋.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_OFFERING_ORDER_RESET, 
							 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							 0, 0, 0, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_EXCHANGESTOCK( sPacketHeader* pPacket )
{
	sDBResponse_ExchangeFromStock *pclMsg = (sDBResponse_ExchangeFromStock *)pPacket;

	SI32 id = pclMsg->siCharID;
	if( 1 != pclMsg->siResult )
	{
		/// 성공이 아니므로 리턴
		if( -1 == pclMsg->siResult )
		{
			if( pclCM->IsValidID(id) ) 
			{
				cltGameMsgResponse_ExchangeStock clExchangeStock( false, 0, 0, 0 );
				cltMsg clMsg( GAMEMSG_RESPONSE_EXCHANGESTOCK, sizeof(clExchangeStock), (BYTE*)&clExchangeStock);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
			}
		}
		return;
	}

	SI32 villageUnique = pclMsg->siStockUnique;
	SI32 sellAmount = pclMsg->siSellAmount;
	SI32 leftAmount = pclMsg->siLeftStockAmount;
	SI32 totalStockAmount = pclMsg->siTotalStockAmount;
	
	if( pclCM->IsValidID(id) ) 
	{
		// 주식을 업데이트 해준다.
		pclCM->CR[id]->pclCI->clStock.clInfo.Set( villageUnique, leftAmount );
		pclCM->CR[id]->SetUpdateSwitch( UPDATE_STOCK, true, 0 );

		cltGameMsgResponse_ExchangeStock clExchangeStock( true, villageUnique, sellAmount, leftAmount );
		cltMsg clMsg( GAMEMSG_RESPONSE_EXCHANGESTOCK, sizeof(clExchangeStock), (BYTE*)&clExchangeStock);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	/// 해당 국가의 총 주식수 업데이트
	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( villageUnique );

	if(pclinfo != NULL)
	{
		pclinfo->clCityhallStrInfo.siTotalStockAmount = totalStockAmount;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GIVESTOCK( sPacketHeader* pPacket )
{
	sDBResponse_GiveStock *pclMsg = (sDBResponse_GiveStock*)pPacket;
	if ( pclMsg == NULL )
	{
		return;
	}

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	SI32 siCharID = pclMsg->siCharID;
	if ( pclCM->IsValidID(siCharID) == NULL )
	{
		return;
	}

	SI32 siVillageUnique	= pclMsg->siVillageUnique;
	SI32 siAmount			= pclMsg->siStockAmount;

	// 주식을 업데이트 해준다.
	pclCM->CR[siCharID]->pclCI->clStock.clInfo.Set( siVillageUnique, siAmount );
	pclCM->CR[siCharID]->SetUpdateSwitch( UPDATE_STOCK, true, 0 );

}
