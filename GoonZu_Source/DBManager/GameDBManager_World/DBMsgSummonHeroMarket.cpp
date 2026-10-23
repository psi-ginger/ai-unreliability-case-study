#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-SummonHeroMarket.h"

void DBGameProtocol::GetSummonHeroMarketStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_SUMMONHEROMARKET;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clSummonHeroMarketStrInfo.clFee );
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney, 0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

	/* 건물 기본정보 */
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siStrDura, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemNum, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siUseItemNum, 0 );
			m_odbc.GetData( &(sendMsg.clStrInfo.clCommonInfo.clBaseMoney.itMoney), 0);
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siRepairStrDura, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// 세금정보를 얻는다.
	GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );
	
	// send
	SendPacket( (BYTE *)&sendMsg );	
}

void DBGameProtocol::SummonHeroMarketSetFee()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketSetFee *pRecvMsg = (sDBRequest_SummonHeroMarketSetFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketSetFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_SETFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltDBFee clFee( &pRecvMsg->clFee );

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketFeeSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetFeeInfo( &clFee );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		GetFeeInfo( &sendMsg.clFee );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SummonHeroMarketGetManyOrder()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketGetManyOrder *pRecvMsg = (sDBRequest_SummonHeroMarketGetManyOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketGetManyOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_GETMANYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketOrderManyGet" )
	END_SP()
	
	EXEC_SQL()
	
	
	sendMsg.usCount 	= 0;
	sendMsg.siResult	= 1;

	BOOL bSend = FALSE;
	SI32 siDateVary = 0;
	BEGIN_FETCHDATA()
		
		bSend = FALSE;

		GetSummonOrder( &sendMsg.clOrder[sendMsg.usCount] );
		sendMsg.usCount++;

		// 50개 까지 받고 전송하기를 반복한다.
		if( sendMsg.usCount >= GET_MANY_SUMMONHEROMARKET_ORDER_NUM )
		{
			// send
			SendPacket( (BYTE *)&sendMsg );
			sendMsg.usCount = 0;
			
			bSend = TRUE;
		}
	
	END_FETCHDATA()

	if( bSend == FALSE ) {
		SendPacket( (BYTE *)&sendMsg );
	}
}

void DBGameProtocol::SummonHeroMarketOrderSell()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketOrderSell *pRecvMsg = (sDBRequest_SummonHeroMarketOrderSell *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketOrderSell sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_ORDERSELL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siOrderDay = pRecvMsg->clOrder.clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketSellOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetSummonOrder( &pRecvMsg->clOrder, &siOrderDay );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siTax, sizeof(pRecvMsg->siTax) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szSelectPersonName, sizeof(pRecvMsg->szSelectPersonName));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		SetItemParam( &pRecvMsg->clitem );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		GetSummonOrder( &sendMsg.clOrder );
		m_odbc.GetData( &sendMsg.siTax, 0 );
		m_odbc.GetData( &sendMsg.clSummonMarketMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonMoney, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPos, 0 );
		GetItemData( &sendMsg.clitem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, OrderIndex = %d, Price = %I64d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, ItemPos = %d, LeftItemNum = %d, TradeType = %d"
				, sendMsg.siVillageUnique, sendMsg.clOrder.siIndex, sendMsg.clOrder.siPrice, sendMsg.clOrder.clSummon.siSummonID, sendMsg.clOrder.clSummon.siKind, sendMsg.clOrder.clSummon.szName, sendMsg.clOrder.clSummon.siStatus
				, sendMsg.clOrder.clSummon.siAttackType, sendMsg.clOrder.clSummon.siIdentity
				, sendMsg.clOrder.clSummon.clIP.siStr, sendMsg.clOrder.clSummon.clIP.siDex, sendMsg.clOrder.clSummon.clIP.siVit, sendMsg.clOrder.clSummon.clIP.siMag
				, sendMsg.clOrder.clSummon.clIP.Life, sendMsg.clOrder.clSummon.clIP.Mana
				, sendMsg.clOrder.clSummon.clIP.siExp, sendMsg.clOrder.clSummon.clIP.siLevel, sendMsg.clOrder.clSummon.clIP.siBonus
				, sendMsg.clOrder.clSummon.clScroll.siUnique[0], sendMsg.clOrder.clSummon.clScroll.siPercent[0]
				, sendMsg.clOrder.clSummon.clScroll.siUnique[1], sendMsg.clOrder.clSummon.clScroll.siPercent[1]
				, sendMsg.siPos, sendMsg.clitem.siItemNum, pRecvMsg->uiTradeType
				);
		sDBLogMsg_QueryString LogMsg("SummonHeroMarket Sell Order", sendMsg.clOrder.siPersonID, sendMsg.clOrder.siSelectPersonID, pRecvMsg->clitem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonHeroMarketMyOrder()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketMyOrder *pRecvMsg = (sDBRequest_SummonHeroMarketMyOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketMyOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_MYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketMyOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	sendMsg.usCount = 0;
	BEGIN_FETCHDATA()
		if(sendMsg.usCount >= 3 )
			continue;
		GetSummonOrder( &sendMsg.clMyOrder[sendMsg.usCount++] );
	END_FETCHDATA()

	// 주문정보가 하나이상일때 1
	if(sendMsg.usCount > 0)
		sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SummonHeroMarketCancelOrder()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketCancelOrder *pRecvMsg = (sDBRequest_SummonHeroMarketCancelOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketCancelOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siMAX_SUMMON_OWN = MAX_SUMMON_OWN;

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketCancelOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOrderIndex, sizeof(pRecvMsg->siOrderIndex) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_SUMMON_OWN, sizeof(siMAX_SUMMON_OWN) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, SummonID = %d, Kind = %d, Name = %s, Str/Dex/Vit/Mag = %d/%d/%d/%d"
					, sendMsg.siOrderIndex, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				);
		sDBLogMsg_QueryString LogMsg("SummonHeroMarket Sell Order Cancel", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonHeroMarketBuy()
{
	// 받은 패킷
	sDBRequest_SummonHeroMarketBuy *pRecvMsg = (sDBRequest_SummonHeroMarketBuy *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonHeroMarketBuy sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONHEROMARKET_BUY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siMAX_SUMMON_OWN = MAX_SUMMON_OWN;

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketBuy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOrderIndex, sizeof(pRecvMsg->siOrderIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_SUMMON_OWN, sizeof(siMAX_SUMMON_OWN) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		m_odbc.GetData( &sendMsg.siTradePrice, 0 );
		GetSummon( &sendMsg.clSummon );
		m_odbc.GetData( &sendMsg.siSellerPersonID, 0 );
		m_odbc.GetData( &sendMsg.clSellerBankMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, Price = %I64d, AfterBuyerMoney = %I64d, AfterSellerBankMoney = %I64d, SummonID = %d, Kind = %d, Name = %s, Str/Dex/Vit/Mag = %d/%d/%d/%d"
				, pRecvMsg->siOrderIndex, sendMsg.siTradePrice, sendMsg.siMoney, sendMsg.clSellerBankMoney.itMoney
				, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				);
		sDBLogMsg_QueryString LogMsg("SummonHeroMarket Buy", sendMsg.siPersonID, sendMsg.siSellerPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonHeroMarketReduceOrderPrice()
{
	// 받은 패킷
	sDBRequest_ReduceSummonHeroOrderPrice *pRecvMsg = (sDBRequest_ReduceSummonHeroOrderPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ReduceSummonHeroOrderPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REDUCESUMMONHEROORDERPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siOrderDay = pRecvMsg->clOrder.clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_SummonHeroMarketReduceOrderPrice" )
		SetSummonOrder( &pRecvMsg->clOrder, &siOrderDay );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetSummonOrder( &sendMsg.clOrder );
	END_FETCHDATA()

	sendMsg.arrayindex = pRecvMsg->arrayindex;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, Price = %I64d, SummonKind = %d, SummonName = %s"
				, pRecvMsg->clOrder.siIndex, pRecvMsg->clOrder.siPrice, pRecvMsg->clOrder.clSummon.siKind, pRecvMsg->clOrder.clSummon.szName
				);
		sDBLogMsg_QueryString LogMsg("SummonHeroMarket Price Reduce", pRecvMsg->clOrder.siPersonID, pRecvMsg->clOrder.siSelectPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
