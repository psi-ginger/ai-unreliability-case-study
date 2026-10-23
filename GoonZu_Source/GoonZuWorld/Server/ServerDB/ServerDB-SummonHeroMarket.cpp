#include "..\Common\CommonHeader.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Letter\Letter.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-SummonHeroMarket.h"
#include "Statistics\Statistics.h"

//------------------------------------
// CommonLogic
//-----------------------------------
#include "Msg\MsgType-SummonHeroMarket.h"
#include "MsgType-Person.h"
#include "MsgRval-Define.h"

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_GETMANYORDER(sPacketHeader* pPacket)
{
	SI32 i;

	sDBResponse_SummonHeroMarketGetManyOrder* pclMsg = (sDBResponse_SummonHeroMarketGetManyOrder*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if( pclMsg->usCount > GET_MANY_SUMMONHEROMARKET_ORDER_NUM )		pclMsg->usCount = GET_MANY_SUMMONHEROMARKET_ORDER_NUM;
	// 서버의 주문 버퍼에 DB에서 보내온 주문 리스트를 넣는다. 
	for( i = 0; i < pclMsg->usCount; ++i )
	{
		pclSummonHeroMarketManager->AddOrder( &pclMsg->clOrder[ i ] );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_SummonHeroMarketGetManyOrder"));
}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_ORDERSELL(sPacketHeader* pPacket)
{
	SI32 returnval = 0;

	sDBResponse_SummonHeroMarketOrderSell *pclMsg = (sDBResponse_SummonHeroMarketOrderSell *)pPacket;

	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	returnval = SRVAL_SUMMONHEROMARKET_FAIL_SELL;

	switch( pclMsg->siResult ) {
	case 1:
		{			
			pclSummonHeroMarketManager->AddOrder( &pclMsg->clOrder );
			
			// 소환수 삭제 

			//---------------------------------------------
			// 소환수를 생성되어있다면 제거한다
			//---------------------------------------------
			SI32 siSummonID = pclCM->CR[id]->GetSummonID();
			if(pclCM->IsValidID(siSummonID)  == TRUE )
			{
				if(pclCM->CR[siSummonID]->GetSummonIndex() == pclMsg->clOrder.clSummon.siSummonID)
				{
					//---------------------------------------------
					// 게임에서 소환수를 지운다. 
					//---------------------------------------------
					pclCM->CR[siSummonID]->DeleteInCell();
					pclCM->DeleteChar(siSummonID);
					//==========================================
					// 주인이 갖고 있는 소환수의 정보를 설정한다.
					//==========================================
					pclCM->CR[id]->siChildCharUnique  = 0;
					pclCM->CR[id]->siSummonIndex	  = 0;

					// 주인한테도 정보를 갱신시킨다.
					cltGameMsgResponse_ChangeSummonStatus clInfo(id, pclCM->CR[id]->siChildCharUnique, pclCM->CR[id]->siSummonIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}


			pclCM->CR[ id ]->pclCI->clSummonInfo.SummonInit( pclMsg->clOrder.clSummon.siSummonID );

			// 현금 정보 업데이트 
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siPersonMoney );
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			// 보유자금 업데이트 
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_SUMMONHEROMARKET ); 
			if( pStrInfo ) {
				pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clSummonMarketMoney );
			}

			if(pclMsg->clOrder.siSelectPersonID != 0 )
			{
				if( pclMsg->siPos >= 0 && pclMsg->siPos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siPos ].Set( &pclMsg->clitem );

					cltGameMsgResponse_PersonItemInfo clinfo1( pclMsg->siPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clitem, 0);
					cltMsg clMsg1(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo1), (BYTE*)&clinfo1);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);
				}
			}

			// 클라이언트로 통보 
			cltGameMsgResponse_SummonHeroMarketSell sendMsg;
			sendMsg.clSummonOrder.Set( &pclMsg->clOrder );

			cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONHEROMARKET_SELL, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

			returnval = SRVAL_SUMMONHEROMARKET_SUCCESS_SELL;
		

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// TEMP LEEKH -------------------------------------------------------------------
				// [로그추가 : 황진성 2007. 10. 18] // 소환 영웅 주문.(수정필요)
				// param1 == 위치. param2 == 거래소 보유자금.
				/*TCHAR str[256];
				ZeroMemory(str, sizeof(str));
				StringCchPrintf(str, sizeof(str), TEXT("%d Year %d Month %d Day %d Hour %d Minute"),	
														pclMsg->clOrder.clDate.uiYear, 
														pclMsg->clOrder.clDate.uiMonth, 
														pclMsg->clOrder.clDate.uiDay, 
														pclMsg->clOrder.clDate.uiHour, 
														pclMsg->clOrder.clDate.uiMinute);

			
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON_HERO_MARKET, LOGCOMMAND_INDEX_SUMMON_HERO_MARKET_SELL_ORDER, 
												0, (cltCharServer*)pclCM->CR[id], &pclMsg->clOrder.clSummon, pclMsg->clOrder.siSelectPersonID, 
												pclMsg->siTax, pclMsg->siPersonMoney, &pclMsg->clitem, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
												pclMsg->siPos, pclMsg->clSummonMarketMoney.itMoney, 0, 0, 0, str, NULL);*/
				// TEMP LEEKH -------------------------------------------------------------------
			}
		}
		break;

	case 2:
		{
			returnval = SRVAL_SUMMONHEROMARKET_FAIL_SELL_NOTENOUGHMONEY;
		}
		break;

	case 5:
		{
			returnval = SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_WRONGNAME;
		}

	}

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_MYORDER(sPacketHeader* pPacket)
{

	sDBResponse_SummonHeroMarketMyOrder *pclMsg = (sDBResponse_SummonHeroMarketMyOrder *)pPacket;
	
	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	if( pclMsg->siResult == 1 ) {

		cltGameMsgResponse_SummonHeroMarketMyOrder sendMsg;

		if( pclMsg->usCount > MAX_SUMMONHEROMARKET_ORDER_PER_PERSON )		pclMsg->usCount = MAX_SUMMONHEROMARKET_ORDER_PER_PERSON;
		for( int i = 0; i < pclMsg->usCount; ++i ) {

			sendMsg.clMyOrder[ i ].Set( &pclMsg->clMyOrder[ i ] );
		}

		sendMsg.usCount = pclMsg->usCount;

		cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONHEROMARKET_MYORDER, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

	} 

}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER(sPacketHeader* pPacket)
{
	sDBResponse_SummonHeroMarketCancelOrder *pclMsg = (sDBResponse_SummonHeroMarketCancelOrder *)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE ) return;

	SI32 returnval = SRVAL_SUMMONHEROMARKET_FAIL_CANCELORDER;

	switch( pclMsg->siResult ) 
	{
	case 1:
		{
			// 주문 삭제 
			pclSummonHeroMarketManager->DelOrder( pclMsg->siOrderIndex );

			// 소환수 복구
			pclCM->CR[ id ]->pclCI->clSummonInfo.SetSummon( pclMsg->clSummon.siSummonID, &pclMsg->clSummon );

			cltGameMsgResponse_SummonHeroMarketCancelOrder sendMsg( pclMsg->siOrderIndex, &pclMsg->clSummon );
			cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

			returnval = SRVAL_SUMMONHEROMARKET_SUCCESS_CANCELORDER;
			
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			{
				// [로그추가 : 황진성 2007. 10. 11] // 영웅 소환수 등록된 판매 내역 취소.
				// param1 == 등록된 판매 번호.
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON_HERO_MARKET, LOGCOMMAND_INDEX_SUMMON_HERO_MARKET_SELL_ORDER_CANCEL, 
												0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
												pclMsg->siOrderIndex, 0, 0, 0, 0, NULL, NULL);
			}
		}
		break;

	case 2:
		{
			// 소환수를 더이상 보유할 수 없음 
			returnval = SRVAL_SUMMONHEROMARKET_FAIL_CANCELORDER_NOMORE;
		}
		break;

	} 

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}


}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_BUY(sPacketHeader* pPacket)
{
	SI32 returnval = 0;

	sDBResponse_SummonHeroMarketBuy *pclMsg = (sDBResponse_SummonHeroMarketBuy *)pPacket;

	SI32 id = pclMsg->usCharID;
	
	if(pclCM->IsValidID(id) == FALSE) return;

	returnval = SRVAL_SUMMONHEROMARKET_FAIL_BUY;

	switch( pclMsg->siResult ) 
	{
	case 1:
		{
			// 주문 삭제 
			pclSummonHeroMarketManager->DelOrder( pclMsg->siOrderIndex );

			// 장예원 자금 
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_SUMMONHEROMARKET );


			if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)	
			{

				//--------------------------------
				// 소환수 정보 업데이트
				//--------------------------------
				pclCM->CR[ id ]->pclCI->clSummonInfo.SetSummon( pclMsg->clSummon.siSummonID, &pclMsg->clSummon );


				//--------------------------------
				// 현금 보유 정보를 업데이트 한다. 
				//--------------------------------
				pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siMoney );
				pclCM->CR[id]->SetUpdateSwitch( UPDATE_IP, true, 0 );

				//--------------------------------
				// 클라이언트로 응답 패킷 보냄
				//--------------------------------
				cltGameMsgResponse_SummonHeroMarketBuy sendMsg( pclMsg->siOrderIndex, &pclMsg->clSummon );
				cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONHEROMARKET_BUY, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 11] // 소환영웅 구입.
					// param1 == 구입할 소환영웅의 주문 번호.
					cltServer * pclServer = (cltServer*)pclClient;
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON_HERO_MARKET, LOGCOMMAND_INDEX_SUMMON_HERO_MARKET_BUY, 
													0, (cltCharServer*)pclCM->CR[id], &pclMsg->clSummon, pclMsg->siSellerPersonID, 
													pclMsg->siTradePrice, pclMsg->siMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
													pclMsg->siOrderIndex, 0, 0, 0, 0, NULL, NULL);
				}
			}


			//-----------------------------------------
			// 팔자의 정보(전장 잔액, 주문 정보 )를 업데이트 한다. 
			//-----------------------------------------
			SI32 sellerid = pclCM->GetIDFromPersonID( pclMsg->siSellerPersonID );
			GMONEY price	= pclMsg->siTradePrice;

			//---------------------------------------------
			// 물건 판 사람의 계좌에서 수수료를 지불한다. 
			//--------------------------------------------
			// 수수료를 확인한다. 
			cltStrInfo* pclstrinfo = pclSummonHeroMarketManager->GetStrInfo( pclMsg->siVillageUnique );
			if(pclstrinfo == NULL)return ;

			GMONEY fee = pclstrinfo->clSummonMarketStrInfo.clFee.CalcFee( price );

			if(fee)
			{
				cltMoney clmoney;
				clmoney.itMoney = fee;
				sDBRequest_PayFee clinfo(sellerid, pclMsg->siSellerPersonID, pclMsg->siVillageUnique, RANKTYPE_SUMMONHEROMARKET, FEETYPE_MARKET_SELL, &clmoney);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);

				pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
			}

			// 쪽지를 보낸다. 
			cltLetterMsg_SummonHeroMarketTrade clletter1( pclMsg->clSummon.siKind, pclMsg->clSummon.szName, price, fee );
			SendLetterMsg(pclMsg->siSellerPersonID, (cltLetterHeader*)&clletter1);


			// 소환수 거래 건수. 
			pclStatisticsManager->clDailyStatistics.siSummonMarketTradeNumber++;
 
			// 소환수 거래 금액 누계 
			pclStatisticsManager->clDailyStatistics.siSummonMarketTradeMoney += price;

			if(pclCM->IsValidID(sellerid))
			{
/*
				//---------------------------------
				// 전장 잔액 
				//---------------------------------
				pclCM->CR[sellerid]->pclCI->clBank.SetMoney( pclMsg->clSellerBankMoney.itMoney );
				
				pclCM->CR[sellerid]->SetUpdateSwitch( UPDATE_BANK, true );
*/
			}

			returnval = SRVAL_SUMMONHEROMARKET_SUCCESS_BUY;
		}
		break;

	case 2:
		{
			returnval = SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTENOUGHMONEY;
		}
		break;

	case 3:
	case 4:
		{
			// 주문 삭제 
			pclSummonHeroMarketManager->DelOrder( pclMsg->siOrderIndex );

			returnval = SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTVALID;
		}
		break;
	case 6:
		{
			returnval = SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTSELECTPERSON;
		}
		break;

	} 

	if( returnval ) {
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}


}

void cltServer::DoMsg_DBMSG_RESPONSE_SUMMONHEROMARKET_SETFEE(sPacketHeader* pPacket)
{
	SI32 returnval = 0;	

	sDBResponse_SummonHeroMarketSetFee *pclMsg = (sDBResponse_SummonHeroMarketSetFee *)pPacket;

	SI32 id = pclMsg->usCharID;
	
	if( pclCM->IsValidID(id) == FALSE || pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID ) return;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_SUMMONHEROMARKET );

	if( pStrInfo ) {
		pStrInfo->clSummonMarketStrInfo.clFee.Set( &pclMsg->clFee );
	
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_SUCCESS_SETFEE, 0, 0, pclCM->CR[id]->GetCharUnique() );

	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_REDUCESUMMONHEROORDERPRICE(sPacketHeader* pPacket)
{
	sDBResponse_ReduceSummonHeroOrderPrice* pclMsg = (sDBResponse_ReduceSummonHeroOrderPrice*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	pclSummonHeroMarketManager->UpdatePrice( pclMsg->arrayindex, &pclMsg->clOrder );

	// 만약 주문자가 있다면 갱신된 정보를 통보한다. 

	// 쪽지를 보낸다. 
	cltLetterMsg_SummonHeroMarketPriceDown clletter( pclMsg->clOrder.clSummon.siKind,
		pclMsg->clOrder.clSummon.szName, 
		pclMsg->clOrder.siPrice );
	SendLetterMsg( pclMsg->clOrder.siPersonID, (cltLetterHeader*)&clletter );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 소환영웅 주문 가격을 낮춤.
		// param1 == 소환영웅의 주문 번호. param2 == 하락한 금액. pszParam1 == 소환수 이름.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SUMMON_HERO_MARKET, LOGCOMMAND_INDEX_SUMMON_HERO_MARKET_PRICE_REDUCE, 
										0, NULL, NULL, pclMsg->clOrder.siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->arrayindex, pclMsg->clOrder.siPrice, 0, 0, 0, pclMsg->clOrder.clSummon.szName, NULL);
	}

}