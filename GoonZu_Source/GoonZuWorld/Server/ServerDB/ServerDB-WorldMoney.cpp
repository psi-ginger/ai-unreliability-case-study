//---------------------------------
// SJY 2005/12/06
//---------------------------------

#include "Char/CharManager/CharManager.h"
#include "Letter/Letter.h"

#include "../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"

#include "../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "../../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"
#include "../../CommonLogic/WorldTraderMgr/WorldTraderManager.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"


void cltServer::DoMsg_DBMSG_RESPONSE_DISTRIBUTION_WORLDMONEY( sPacketHeader* pPacket )
{
	sDBResponse_Distribution_WorldMoney* pclinfo = (sDBResponse_Distribution_WorldMoney*)pPacket;

	SI32 id = pclinfo->usCharID ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult != 1 )
		return ;

	pclchar->pclCI->clIP.SetMoney(pclinfo->siLeftMoney);
	pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

	cltGameMsgResponse_Distrubution_WorldMoney clinfo(pclinfo->siWorldMoney,pclinfo->siLeftWorldMoney,pclinfo->siMoney,pclinfo->siLeftMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_DISTRIBUTION_WORLDMONEY, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아큐 공모.
		// param1 == 사는 아큐의 수, param2 == 산 뒤의 아큐의 수
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_DISTRIBUTION, 
							0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, pclinfo->siMoney, pclinfo->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0, 
							pclinfo->siWorldMoney, pclinfo->siLeftWorldMoney, 0, 0, 0, NULL, NULL);
	}
	return ;
}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_TRADELIST( sPacketHeader* pPacket )
{
	sDBResponse_WorldMoney_TradeList* pclinfo = (sDBResponse_WorldMoney_TradeList*)pPacket;

	if ( pclinfo->siResult != 1 )
		return ;

	if ( pclinfo->siPage < 0 || pclinfo->siPage >= MAX_WORLDMONEY_TRADELIST_NUM )
		return ;

	for ( SI32 i = pclinfo->siPage ; i < pclinfo->siPage + MAX_WORLDMONEY_TRADELIST_PER_SEND ; i ++ )
	{
		// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
		if (i < 0 || i >= MAX_WORLDMONEY_TRADELIST_NUM)		continue;
		if ( (i-pclinfo->siPage) < 0 || (i-pclinfo->siPage) >= MAX_WORLDMONEY_TRADELIST_PER_SEND )	continue;

		pclDistributionWorldMoney->clTradeList[i].Set(pclinfo->siWorldMoney[i-pclinfo->siPage],
			pclinfo->siPrice[i-pclinfo->siPage],
			pclinfo->siPersonID[i-pclinfo->siPage],
			pclinfo->siDate[i-pclinfo->siPage]);
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_WorldMoney_TradeList"));

}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_SELLORDER( sPacketHeader* pPacket )
{
	sDBResponse_WorldMoney_SellOrder* pclinfo = (sDBResponse_WorldMoney_SellOrder*)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if (pclinfo->siListIndex < 0 || pclinfo->siListIndex >= MAX_WORLDMONEY_TRADELIST_NUM )
		return;

	SI32 id = pclinfo->usCharId ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult != 1 )
		return ;

	// 리스트에 등록한다.
	pclDistributionWorldMoney->clTradeList[pclinfo->siListIndex].Set(pclinfo->siWorldMoney,pclinfo->siPrice,
		pclinfo->siPersonID,pclinfo->siDate) ;

	// 판매자의 정보를 갱신한다.
	pclchar->pclCI->clIP.SetMoney(pclinfo->siLeftMoney); // 남은 아큐
	pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney); // 남은 돈
	pclchar->pclCI->clWorldMoney.siTradeWorldMoney = pclinfo->siWorldMoney ; // 올린 아큐
	pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice = pclinfo->siPrice ; // 올린 가격

	cltGameMsgResponse_WorldMoney_SellOrder clinfo(pclinfo->siResult,pclinfo->siWorldMoney,pclinfo->siLeftWorldMoney,pclinfo->siPrice,pclinfo->siLeftMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_WORLDMONEY_SELLORDER, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아큐 판매 등록.
		// param1 == 사는 아큐의 수, param2 == 산 뒤의 아큐의 수, param3 == 산 아큐의 가격 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_SELL_ORDER, 
							0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, pclinfo->siFee, pclinfo->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0, 
							pclinfo->siWorldMoney, pclinfo->siLeftWorldMoney, pclinfo->siPrice, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_BUYORDER( sPacketHeader* pPacket )
{
	sDBResponse_WorldMoney_BuyOrder* pclinfo = (sDBResponse_WorldMoney_BuyOrder*)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if ( pclinfo->siListIndex < 0 || pclinfo->siListIndex >= MAX_WORLDMONEY_TRADELIST_NUM )
		return ;

	SI32 id = pclinfo->usCharId ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult != 1 )
		return ;

	// 리스트를 갱신한다.
	pclDistributionWorldMoney->clTradeList[pclinfo->siListIndex].Set(pclinfo->siLeftListWorldMoney,pclinfo->siPrice,
		pclinfo->siSellerPersonID,
		pclDistributionWorldMoney->clTradeList[pclinfo->siListIndex].siDate);

	if ( pclinfo->siLeftListWorldMoney == 0 ) // 다 팔린 건가?
	{
		pclDistributionWorldMoney->clTradeList[pclinfo->siListIndex].Set(0,0,0,0);
	}

	// 구입자의 정보를 갱신한다.
	pclchar->pclCI->clIP.SetMoney(pclinfo->siLeftMoney);
	pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

	// PCK : 아큐의 총 구매 가격
	SI32 siWorldMoneySum = pclinfo->siWorldMoney * pclinfo->siPrice;

	// 판매자의 정보를 갱신한다.
	if ( pclinfo->siSellerPersonID > 0 )
	{
		SI32 sellerID = pclCM->GetIDFromPersonID(pclinfo->siSellerPersonID);
		if ( IsPC(sellerID) )
		{
			pclCM->CR[sellerID]->pclCI->clBank.SetMoney(pclinfo->siSellerBankMoney);

			pclCM->CR[sellerID]->pclCI->clWorldMoney.siTradeWorldMoney = pclinfo->siLeftListWorldMoney ;
			if ( pclinfo->siLeftListWorldMoney == 0 )
			{
				pclCM->CR[sellerID]->pclCI->clWorldMoney.siTradeWorldMoneyPrice = 0 ;
			}

			pclCM->CR[sellerID]->SetUpdateSwitch(UPDATE_BANK,true,0);
			pclCM->CR[sellerID]->SetUpdateSwitch(UPDATE_WORLDMONEY,true,0);
		}

		cltLetterMsg_WorldMoneyTradeResult clletter(pclinfo->siWorldMoney,pclinfo->siWorldMoney*pclinfo->siPrice);
		// 쪽지를 DB나 클라이언트에게 보낸다. 
		SendLetterMsg(pclinfo->siSellerPersonID, (cltLetterHeader*)&clletter);
	}

	cltGameMsgResponse_WorldMoney_BuyOrder clinfo(pclinfo->siResult,pclinfo->siWorldMoney,pclinfo->siLeftWorldMoney,pclinfo->siLeftMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_WORLDMONEY_BUYORDER, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아큐 구입.
		// param1 == 산 아큐. param2 == 산 뒤의 총 아큐
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_BUY, 
							0, (cltCharServer*)(pclCM->CR[id]), NULL, pclinfo->siSellerPersonID, siWorldMoneySum, pclinfo->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0, 
							pclinfo->siWorldMoney, pclinfo->siLeftWorldMoney, 0, 0, 0, NULL, NULL);
	}
	
	SI64 averageprice = pclDistributionWorldMoney->SetAverageWorldMoney(pclinfo->siWorldMoney,pclinfo->siPrice);

	sDBRequest_WorldMoney_SetAveragePrice clMsg2(averageprice);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg2);

	// 퀘스트를 수행한다.
	//pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_TRYTOBUYACU);

}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_CANCELORDER( sPacketHeader* pPacket )
{
	sDBResponse_WorldMoney_CancelOrder* pclinfo = (sDBResponse_WorldMoney_CancelOrder*)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if ( pclinfo->siListIndex < 0 || pclinfo->siListIndex >= MAX_WORLDMONEY_TRADELIST_NUM )
		return ;

	SI32 id = pclinfo->usCharId ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult != 1 )
		return ;

	// 리스트를 갱신한다.
	pclDistributionWorldMoney->clTradeList[pclinfo->siListIndex].Set(0,0,0,0);

	// 취소자의 정보를 갱신한다.
	pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);
	pclchar->pclCI->clWorldMoney.siTradeWorldMoney = 0 ;
	pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice = 0 ;

	cltGameMsgResponse_WorldMoney_CancelOrder clinfo(pclinfo->siResult,pclinfo->siLeftWorldMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_WORLDMONEY_CANCELORDER, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 아큐 판매 등록 취소.
		// param1 == 리스트에서의 인덱스, param2 == 취소하고 받은 뒤의 아큐수.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_CANCEL_ORDER, 
							0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
							pclinfo->siListIndex, pclinfo->siLeftWorldMoney, 0, 0, 0, NULL, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_SETAVERAGEPRICE(sPacketHeader*pPacket)
{
	sDBResponse_WorldMoney_SetAveragePrice* pclinfo = (sDBResponse_WorldMoney_SetAveragePrice*)pPacket;

	if ( pclinfo->siResult == 1 )
	{
		pclDistributionWorldMoney->siAverageWorldMoneyPrice = pclinfo->siAverageWorldMoneyPrice ;

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 아큐 평균 가격.
			// param1 == 평균 아큐 가격.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_AVERAGE_PRICE, 
								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								pclinfo->siAverageWorldMoneyPrice, 0, 0, 0, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_REDUCEPRICE(sPacketHeader*pPacket)
{
	sDBResponse_WorldMoney_ReducePrice * pclinfo = (sDBResponse_WorldMoney_ReducePrice*)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if ( pclinfo->siIndex < 0 || pclinfo->siIndex >= MAX_WORLDMONEY_TRADELIST_NUM )
		return ;

	if ( pclinfo->siResult == 1 )
	{
		pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siDate = pclinfo->siDate ;
		pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPrice = pclinfo->siPrice ;

		cltLetterMsg_WorldMoneyReducePrice clletter(pclinfo->siPrice,pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siWorldMoney,
			pclinfo->siDate);
		// 쪽지를 DB나 클라이언트에게 보낸다. 
		SendLetterMsg(pclinfo->siPersonID, (cltLetterHeader*)&clletter);

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 10. 18] // 아큐의 가격을 떨군다.
		//	// param1 == 결과값, param2 == 변경된 주문의 인덱스, param3 == 변경된 가격, param4 == 아큐. param5 == 시간.
		//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_REDUCE_PRICE, 
		//						0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		//						pclinfo->siResult, 
		//						pclinfo->siIndex,
		//						pclinfo->siPrice, 
		//						pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siWorldMoney,
		//						pclinfo->siDate, NULL, NULL);
		//}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDMONEY_SET_CHAR(sPacketHeader*pPacket)
{
	sDBResponse_WorldMoney_Set_Char * pclinfo = (sDBResponse_WorldMoney_Set_Char*)pPacket;

	SI32 id = pclinfo->siCharID ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult == 1 )
	{
		pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);
		pclchar->SetUpdateSwitch(UPDATE_WORLDMONEY,true,0);

		TCHAR* name = (TCHAR*)pclchar->GetName();

		cltGameMsgResponse_WorldMoney_Set_Char clinfo(name, pclinfo->siWorldMoney,pclinfo->siLeftWorldMoney);
		cltMsg clMsg(GAMEMSG_RESPONSE_WORLDMONEY_SET_CHAR, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 아큐 추가. 
			// param1 == 아큐, param2 == 최종 아큐. pszParam1 == 이름. 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_MONEY, LOGCOMMAND_INDEX_WORLD_MONEY_ADD, 
								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								pclinfo->siWorldMoney, pclinfo->siLeftWorldMoney, 0, 0, 0, name, NULL);
		}
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_TRADESERVER_SETORDER( sPacketHeader* pPacket )
{
	sDBResponse_TradeServer_SetOrder* pclinfo = (sDBResponse_TradeServer_SetOrder*)pPacket;

	if ( pclinfo->siResult == 1 )
	{
		SI32 siID = pclinfo->siCharID ;
		if ( pclCM->IsValidID(siID) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
			if( pclchar == NULL )													return;
			if( pclinfo->siPos < 0 || pclinfo->siPos >= MAX_ITEM_PER_PERSON )		return;

			// 남은 아이템이랑 아큐를 설정한다.
			pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clLeftItem);
			pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

			cltGameMsgResponse_TradeServer_Set_SellOrder clinfo(pclinfo->siResult,&pclinfo->clOrder,pclinfo->siPos,&pclinfo->clLeftItem,
				pclinfo->siLeftWorldMoney);
			cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_SET_SELLORDER, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			// 트레이드중이라고 잠근 것을 풀어주자!
			pclchar->SetNoMoveItemSwitch(pclinfo->siPos,false);

			// 현재 무역 서버와의 거래가 끝났다
			pclchar->bTradeServerUsing = false ;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 11] // 현재 무역 서버와의 판매 등록.
				// param1 == 아이템 유니크, 
				// param2 == 아이템 위치,
				// param3 == 아이템 갯수
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TRADE_SERVER, LOGCOMMAND_INDEX_TRADE_SERVER_SELL_ORDER, 
												0, (cltCharServer*)pclchar, NULL, 0, pclinfo->siWorldMoneyFee, pclinfo->siLeftWorldMoney, NULL, 0, 0, 0, 0, 0, 0, 
												pclinfo->clOrder.clitem.siUnique, pclinfo->siPos, pclinfo->clOrder.clitem.siItemNum, 0, 0, 
												NULL, NULL);
			}
		}
	}
	else
	{
		SI32 siID = pclinfo->siCharID ;
		if ( pclCM->IsValidID(siID) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
			if ( pclchar == NULL )													return;
			if( pclinfo->siPos < 0 || pclinfo->siPos >= MAX_ITEM_PER_PERSON )		return;

			if ( pclchar->pclCI->GetPersonID() == pclinfo->siPersonID )
			{
				// 트레이드중이라고 잠근 것을 풀어주자!
				pclchar->SetNoMoveItemSwitch(pclinfo->siPos,false);

				// 현재 무역 서버와의 거래가 끝났다
				pclchar->bTradeServerUsing = false ;
			}
		}

	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_TRADESERVER_CANCELORDER( sPacketHeader* pPacket )
{
	sDBResponse_TradeServer_CancelOrder* pclinfo = (sDBResponse_TradeServer_CancelOrder*)pPacket;

	SI32 siID = pclinfo->siCharID ;
	if ( pclCM->IsValidID(siID) == false )
		return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
	if( pclchar == NULL )													return;
	if( pclinfo->siPos < 0 || pclinfo->siPos >= MAX_ITEM_PER_PERSON )		return;

	// 예약되어 있던 자리를 해제한다.
	pclchar->SetInputItemReserve(pclinfo->siPos,0);

	// 현재 무역 서버와의 거래가 끝났다
	pclchar->bTradeServerUsing = false ;

	if ( pclinfo->siResult == 1 )
	{
		//SI32 siID = pclinfo->siCharID ;
		//if ( pclCM->IsValidID(siID) )
		//{
		//	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
		//	if ( pclchar == NULL )
		//		return ;

		//	// 남은 아이템을 설정한다.
		//	pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clLeftItem);

		//	// 리스트를 수정한다.
		//	//pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Set(&pclinfo->clOrder);
		//	pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init();

		//	cltGameMsgResponse_TradeServer_Cancel_SellOrder clinfo(pclinfo->siResult,&pclinfo->clOrder,pclinfo->siPos,&pclinfo->clLeftItem);
		//	cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_CANCEL_SELLORDER,sizeof(clinfo),(BYTE*)&clinfo);
		//	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		//}

		// 남은 아이템을 설정한다.
		pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clLeftItem);

		cltGameMsgResponse_TradeServer_Cancel_SellOrder clinfo(pclinfo->siResult,&pclinfo->clOrder,pclinfo->siPos,&pclinfo->clLeftItem);
		cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_CANCEL_SELLORDER,sizeof(clinfo),(BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 현재 무역 서버와의 판매 취소.				
			// param1 == 아이템 유니크
			// param2 == 위치
			// param3 == 주문번호
			// param4 == 캐릭터 국가번호
			// param5 == 서버 번호
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TRADE_SERVER, LOGCOMMAND_INDEX_TRADE_SERVER_CANCEL_SELL, 
											0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclinfo->clOrder.clitem.siUnique, pclinfo->siPos, pclinfo->clOrder.siIndex, pclinfo->clOrder.siCharNation, pclinfo->clOrder.siServerIndex, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_TRADESERVER_BUYITEM( sPacketHeader* pPacket )
{
	sDBResponse_TradeServer_BuyItem* pclinfo = (sDBResponse_TradeServer_BuyItem*)pPacket;

	SI32 siID = pclinfo->siCharID ;
	if ( pclCM->IsValidID(siID) == false )
		return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
	if ( pclchar == NULL )													return;
	if( pclinfo->siPos < 0 || pclinfo->siPos >= MAX_ITEM_PER_PERSON )		return;

	// 예약되어 있던 자리를 해제한다.
	pclchar->SetInputItemReserve(pclinfo->siPos,0);

	// 현재 무역 서버와의 거래가 끝났다
	pclchar->bTradeServerUsing = false ;

	if ( pclinfo->siResult == 1 )
	{
		//SI32 siID = pclinfo->siCharID ;
		//if ( pclCM->IsValidID(siID) )
		//{
		//	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
		//	if ( pclchar == NULL )
		//		return ;

		//	pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clLeftItem);
		//	pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

		//	cltGameMsgResponse_TradeServer_Set_BuyOrder clinfo( pclinfo->siResult,&pclinfo->clOrder,pclinfo->siPos,
		//		&pclinfo->clLeftItem,pclinfo->siLeftWorldMoney);
		//	cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_SET_BUYORDER,sizeof(clinfo),(BYTE*)&clinfo);
		//	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		//}

		pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].Set(&pclinfo->clLeftItem);
		pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

		cltGameMsgResponse_TradeServer_Set_BuyOrder clinfo( pclinfo->siResult,&pclinfo->clOrder,pclinfo->siPos,
			&pclinfo->clLeftItem,pclinfo->siLeftWorldMoney);
		cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_SET_BUYORDER,sizeof(clinfo),(BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);

		//서버무역 수입물품 통계 추가
		pclItemManager->AddWorldTradeIncomeItemNum(pclinfo->clLeftItem.siUnique, pclinfo->clLeftItem.siItemNum);

		// 새로운 통계 - 서버무역에 의한 수입물품의 통계를 낸다.
		NStatistics_Add("Item", pclinfo->clLeftItem.siUnique, "TradeIncome", pclinfo->clLeftItem.siItemNum);
		NStatistics_Set("Item", pclinfo->clLeftItem.siUnique, "Name", (const char*)pclItemManager->GetName(pclinfo->clLeftItem.siUnique));

		// 퀘스트를 수행한다.
		//pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_TRYTOBUYIMPORTS);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11]  // 현재 무역 서버와의 구매.				
			// param1	 == 아이템 유니크,  param2 == 아이템 위치, param3 == 아이템 가격
			// pszParam1 ==  주문품 위치
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TRADE_SERVER, LOGCOMMAND_INDEX_TRADE_SERVER_BUY, 
											0, (cltCharServer*)pclchar, NULL, 0, 0, pclinfo->siLeftWorldMoney, NULL, 0, 0, 0, 0, 0, 0, 
											pclinfo->clOrder.clitem.siUnique, pclinfo->siPos, pclinfo->clOrder.clitem.siPrice, 0, 0, 0, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_TRADESERVER_SET_SELLINFO( sPacketHeader* pPacket )
{
	sDBResponse_TradeServer_Set_SellInfo* pclinfo = ( sDBResponse_TradeServer_Set_SellInfo*)pPacket;

	if ( pclinfo->siResult == 1 )
	{
		if ( pclinfo->clOrder.siPersonID > 0 )
		{
			SI32 siID = 0;
			siID = pclCM->GetIDFromPersonID(pclinfo->clOrder.siPersonID);

			// 현재 서버에 존재한다면
			if ( IsPC(siID) && pclCM->IsValidID(siID) && pclCM->CR[siID]->pclCI->GetPersonID() == pclinfo->clOrder.siPersonID )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];
				if ( pclchar == NULL )
					return ;

				pclchar->pclCI->clWorldMoney.SetWorldMoney(pclinfo->siLeftWorldMoney);

				cltGameMsgResponse_TradeServer_Get_SellInfo clinfo(&pclinfo->clOrder,pclinfo->siLeftWorldMoney);
				cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_GET_SELLINFO,sizeof(clinfo),(BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}

			if ( pclinfo->clOrder.siIndex >= 0 )//&& pclinfo->clOrder.siIndex < MAX_TRADESERVER_ORDER_LIST )
			{
				//	pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].Init() ;

				//if ( pclNTradeServerProcess )
				if ( pclNTradeServerProcess && pclNTradeServerProcess->GetTradeServerServersSession()->GetState() == SESSION_STATE_ESTABLISHED )
				{
					NTradeServerRequest_Del_SellInfo clMsg(&pclinfo->clOrder);
					pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);
				}
				else
				{
					TCHAR* itemname = (TCHAR*)pclItemManager->GetName(pclinfo->clOrder.clitem.siUnique);
					if ( itemname )
					{
						pclLog->FilePrint("Config\\TradeServerSellError.log","Session NULL : ID[%d] Item[%s] Num[%d] Price[%I64d]",
							pclinfo->clOrder.siPersonID,itemname,pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siPrice);
					}
				}
			}
			else
			{
				TCHAR* itemname = (TCHAR*)pclItemManager->GetName(pclinfo->clOrder.clitem.siUnique);
				if ( itemname )
				{
					pclLog->FilePrint("Config\\TradeServerSellIndexError.log","List Error : Index[%d] ID[%d] Item[%s] Num[%d] Price[%I64d]",
						pclinfo->clOrder.siIndex,pclinfo->clOrder.siPersonID,itemname,pclinfo->clOrder.clitem.siItemNum,
						pclinfo->clOrder.clitem.siPrice);
				}
			}

			cltLetterMsg_TradeServerSell clletter(pclinfo->clOrder.clitem.siUnique,pclinfo->clOrder.clitem.siItemNum,
				pclinfo->clOrder.clitem.siPrice,
				pclinfo->szBuyerServer,pclinfo->szBuyerName);
			// 쪽지를 DB나 클라이언트에게 보낸다. 
			SendLetterMsg(pclinfo->clOrder.siPersonID, (cltLetterHeader*)&clletter);


			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 11] // 무역거래.
				// param1	 == 아이템 유니크, 
				// param2	 == 아이템 개수, 
				// pszParam1 == 산 사람이 있는 서버, 
				// pszParam1 == 산 사람이 이름.  
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TRADE_SERVER, LOGCOMMAND_INDEX_TRADE_SERVER_SELLED, 
												0, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
												pclinfo->clOrder.clitem.siUnique, pclinfo->clOrder.clitem.siItemNum, 
												pclinfo->clOrder.clitem.siPrice, 0, 0, 
												pclinfo->szBuyerServer, pclinfo->szBuyerName);
			}

			//서버무역 수출물품 통계 추가
			pclItemManager->AddWorldTradeExportItemNum(pclinfo->clOrder.clitem.siUnique,pclinfo->clOrder.clitem.siItemNum);

			// 새로운 통계 - 서버무역에 의한 수출물품의 통계를 낸다.
			NStatistics_Add("Item", pclinfo->clOrder.clitem.siUnique, "TradeExport", pclinfo->clOrder.clitem.siItemNum);
			NStatistics_Set("Item", pclinfo->clOrder.clitem.siUnique, "Name", pclItemManager->GetName(pclinfo->clOrder.clitem.siUnique));
		}
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU( sPacketHeader* pPacket )
{
	sDBResponse_TradeServer_Get_Withdraw_Acu* pclInfo = (sDBResponse_TradeServer_Get_Withdraw_Acu*)pPacket;
	if ( NULL == pclInfo )
	{
		return;
	}

	if ( 1 != pclInfo->m_siResult )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( pclInfo->m_siCharID );
	if ( NULL == pclchar )
	{
		return;
	}
	
	// 요청한 사람과 다른 사람이라면
	if ( pclchar->pclCI->GetPersonID() != pclInfo->m_siPersonID )
	{
		return;
	}

	//------------------------------------------
	// 서버정보 셋팅
	//------------------------------------------

	// 내 변경된 아큐를 설정
	pclchar->pclCI->clWorldMoney.SetWorldMoney( pclInfo->m_siLeftAcu );

	//------------------------------------------
	// 무역서버에 정상적으로 받았다는 메시지 보냄
	//------------------------------------------
	SI64 siTradeServerConnect = 0;	// 이 변수는 로그를 남길때 무역서버에 결과를 보냈는지 안보냈는지를 확인하는 변수

	if ( (pclNTradeServerProcess) && (TRUE == pclNTradeServerProcess->IsEstablished()) )
	{
		siTradeServerConnect = 1;

		NTradeServerRequest_Withdraw_Acu_Success clTSMsg( pclInfo->m_siReservedNum );
		pclNTradeServerProcess->SendMsg((sPacketHeader*)&clTSMsg);
	}

	//------------------------------------------
	// 클라이언트로 정보 보냄
	//------------------------------------------
	cltGameMsgResponse_TradeServer_Get_Withdraw_Acu clInfo( pclInfo->m_siLeftAcu, pclInfo->m_siLeftSavedAcu );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	//------------------------------------------
	// 로그 남김
	//------------------------------------------
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// Param1	: 내가 현재 갖고 있는 아큐
		// Param2	: 무역서버에 저장되어있는 아큐
		// Param3	: 무역서버에서 받은 아큐
		// Param4	: 무역서버에 받았다는 결과를 보냈는지 여부(0=DisConnect/1=Connect)
		
		cltServer* pclserver = (cltServer*)pclClient;
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_TRADE_SERVER, LOGCOMMAND_INDEX_TRADE_WITHDRAW_ACU, 
			0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			pclInfo->m_siLeftAcu, pclInfo->m_siLeftSavedAcu, pclInfo->m_siWithdrawAcu, siTradeServerConnect, 0,
			NULL, NULL
			);
	}

	// 무역 사용중임을 해제한다
	pclchar->bTradeServerUsing = false;

}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDPASSPORT_SET(sPacketHeader*pPacket)
{
	sDBResponse_WorldPassport_Set* pclinfo = (sDBResponse_WorldPassport_Set*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	SI32 siCharID = pclCM->GetIDFromPersonID( pclinfo->m_siPersonID );
	cltCharServer* pclchar = pclCM->GetCharServer( siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( 1 > pclinfo->m_siResult )
	{
		// 중복된 이름이 이미 존재함
		if ( -1 == pclinfo->m_siResult )
		{
			SendServerResponseMsg( 0, SRVAL_FAIL_WORLDPASSPORT_OVERLAPPING, 0, 0, siCharID );
		}
		return;
	}

	//------------------------------------------
	// 서버정보 셋팅
	//------------------------------------------

	// 케릭터에 여권관련 정보 셋팅
	pclchar->pclCI->clWorldPassport.Set( pclinfo->m_szPassportName, pclinfo->m_siIssueDateVary, pclinfo->m_siExpireDateVary );

	// 발급비용을 지불하고 난 아큐
	pclchar->pclCI->clWorldMoney.SetWorldMoney( pclinfo->m_siLeftAcu );


	//------------------------------------------
	// 클라이언트로 정보 보냄
	//------------------------------------------
	cltGameMsgResponse_WorldPassport_Set clinfo( pclinfo->m_szPassportName, pclinfo->m_siIssueDateVary, pclinfo->m_siExpireDateVary, pclinfo->m_siLeftAcu );
	cltMsg clMsg(GAMEMSG_RESPONSE_WORLDPASSPORT_SET,sizeof(clinfo),(BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);


	//------------------------------------------
	// 로그 남김
	//------------------------------------------
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// Param1	: 여권 발행일
		// Param2	: 여권 만료일
		// Param3	: 여권 발행 비용(아큐)
		// Param4	: 발급비용을 제외한 현재 아큐
		// szParam1	: 여권이름
		cltServer* pclserver = (cltServer*)pclClient;
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_PASSPORT, LOGCOMMAND_INDEX_WORLD_PASSPORT_SET, 
			0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			pclinfo->m_siIssueDateVary, pclinfo->m_siExpireDateVary, pclinfo->m_siPaidAcu, pclinfo->m_siLeftAcu, 0,
			pclinfo->m_szPassportName, NULL
		);
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_WORLDPASSPORT_EXTEND(sPacketHeader*pPacket)
{
	sDBResponse_WorldPassport_Extend* pclinfo = ( sDBResponse_WorldPassport_Extend*)pPacket;
	if ( NULL == pclinfo )
	{
		return;
	}

	SI32 siCharID = pclCM->GetIDFromPersonID( pclinfo->m_siPersonID );
	cltCharServer* pclchar = pclCM->GetCharServer( siCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( 1 > pclinfo->m_siResult )
	{
		return;
	}

	//------------------------------------------
	// 서버정보 셋팅
	//------------------------------------------
	// 케릭터에 여권 만료일 셋팅
	pclchar->pclCI->clWorldPassport.SetExpireDateVary( pclinfo->m_siExpireDateVary );

	// 연장비용을 지불하고 난 아큐
	pclchar->pclCI->clWorldMoney.SetWorldMoney( pclinfo->m_siLeftAcu );


	//------------------------------------------
	// 클라이언트로 정보 보냄
	//------------------------------------------
	cltGameMsgResponse_WorldPassport_Extend clinfo( pclinfo->m_siExpireDateVary, pclinfo->m_siLeftAcu );
	cltMsg clMsg(GAMEMSG_RESPONSE_WORLDPASSPORT_EXTEND,sizeof(clinfo),(BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);


	//------------------------------------------
	// 로그 남김
	//------------------------------------------
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// Param1	: 여권 만료일
		// Param2	: 여권 연장 비용(아큐)
		// Param3	: 연장비용을 제외한 현재 아큐
		cltServer* pclserver = (cltServer*)pclClient;
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_WORLD_PASSPORT, LOGCOMMAND_INDEX_WORLD_PASSPORT_EXTEND, 
			0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			pclinfo->m_siExpireDateVary, pclinfo->m_siPaidAcu, pclinfo->m_siLeftAcu, 0, 0,
			NULL, NULL
		);
	}


}