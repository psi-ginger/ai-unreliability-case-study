#include "Char\CharManager\CharManager.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/MinisterMgr.h"
#include "../Server/Minister/Minister-Izo.h"
#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-SummonMarket.h"

#include "MsgRval-Define.h"
#include "..\CommonLogic\Msg\MsgType-SummonMarket.h"	

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_MYORDER(cltMsg* pclMsg, SI32 id )
{
 	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	sDBRequest_SummonMarketMyOrder sendMsg( id, personid );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonMarketCancelOrder *pclinfo = (cltGameMsgRequest_SummonMarketCancelOrder *)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)
		return ;

	if(FALSE == pclCM->CR[id]->pclCI->clSummonInfo.CanHaveSummon())
	{
		// 소환수를 더이상 보유할 수 없음 
		SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_CANCELORDER_NOMORE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	sDBRequest_SummonMarketCancelOrder sendMsg( id, personid, pclinfo->siOrderIndex );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonMarketOrderList *pclinfo = (cltGameMsgRequest_SummonMarketOrderList *)pclMsg->cData;

	cltGameMsgResponse_SummonMarketOrderList sendMsg;

	cltSummonOrder *pclOrder;
	int count = 0;
	int i;

	sendMsg.usCount		  = 0;
	SI32 TotalSummonCount = 0;

	if( pclinfo->siSelectList == 1 ) 
	{
		SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

		for( i = 0; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) 
		{
			pclOrder = &pclSummonMarketManager->pclSummonOrder[ i ];
		
			if( pclOrder->siPersonID &&	pclOrder->siSelectPersonID == personid ) 
			{
				++count; 

				sendMsg.clOrder[ sendMsg.usCount ].Set( pclOrder );
				++sendMsg.usCount;
			}

			if( sendMsg.usCount == MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER ) 
				break;

			if( count >= pclSummonMarketManager->siTotalOrderNumber ) 
				break;
		}
	}
	else 
	{
		SI32 siOrderListStart = pclinfo->siStartPos * MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER;	// 주문 정보를 세팅할 처음 배열.
		for(i = 0; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) 
		{
			pclOrder = &pclSummonMarketManager->pclSummonOrder[ i ];

			//test++++++++++++++++++++++++++++++++++++++++++++
			// 주문 만땅 채우기 테스트.
			/*if(i < MAX_SUMMONMARKET_ORDER_NUMBER)
			{
				memcpy(&pclSummonMarketManager->pclSummonOrder[i], &pclSummonMarketManager->pclSummonOrder[0], sizeof(pclSummonMarketManager->pclSummonOrder[0]));
				pclSummonMarketManager->pclSummonOrder[i].siPrice += i;
			} */
			//test++++++++++++++++++++++++++++++++++++++++++++

			if(	pclOrder->siPersonID &&	pclOrder->clSummon.siKind == pclinfo->siKind)
			{		
				TotalSummonCount++;
			}

			if(		pclOrder->siPersonID 	
				&&  pclOrder->clSummon.siKind == pclinfo->siKind 
				&&  i >= siOrderListStart
				&&  sendMsg.usCount < MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER)
			{
				++count; 

				sendMsg.clOrder[ sendMsg.usCount ].Set( pclOrder );
				++sendMsg.usCount;
			}

			//if( sendMsg.usCount == MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER ) ;
				//break;

			//if( count >= pclSummonMarketManager->siTotalOrderNumber ) ;
				//break;
		}
	}

	// 지정 거래시 페이지수는 무조건 하나.
	if( pclinfo->siSelectList == 1 ) 
	{
		sendMsg.siCurrentPage  = 0;
		sendMsg.usTotalPage	   = 0;	//[진성] 전체 페이지 수.
	}
	else
	{
		sendMsg.siCurrentPage  = pclinfo->siStartPos;
		sendMsg.usTotalPage	   = TotalSummonCount / MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER;	//[진성] 전체 페이지 수.
	}	

	cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONMARKET_ORDERLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_SELL(cltMsg* pclMsg, SI32 id )
{

	cltGameMsgRequest_SummonMarketSell *pclinfo = (cltGameMsgRequest_SummonMarketSell *)pclMsg->cData;

	GMONEY price = pclinfo->siPrice;
	GMONEY tax = (GMONEY)(price * 0.01);

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// Person이 위치한 마을의 VillageUnique를 확보한다
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// 밖으로 나와있는 소환수의 정보를 주인에게 update 시킨다.
	((cltCharServer *)pclCM->CR[ id ])->UpdateTakeOutSummon();

	// 실제로 서버에서 소환수를 보유하고 있는가?
	cltSummon *pclSummon = pclCM->CR[ id ]->pclCI->clSummonInfo.GetSummon( pclinfo->siSummonIndex );
	if( pclSummon == NULL ) return;

	//[진성] 소환수 장비 착용 검사.
	if( pclCM->CR[ id ]->pclCI->clSummonInfo.IsHaveEquipItem(pclinfo->siSummonIndex) )
	{
		SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELL_HAVEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}
	//[진성] 이전 소환수 장비 착용 검사.
	/*if( pclCM->CR[ id ]->IsSummonHaveItem( pclinfo->siSummonIndex ) ) 
	{
		SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELL_HAVEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}*/

	if( pclClient->pclKindInfoSet->IsValidKind(pclSummon->siKind) == false ) 
	{ 
		//SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELL_NOTVALID,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	cltKindInfo* pclKI	= NULL;
	//[진성] 소환수 테이블 분리. => 2008-6-18
	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclKI = pclClient->pclKindInfoSet->pclSummonKI[ pclSummon->siKind ];
	}
	else
	{
		pclKI = pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ];
	}
	if(NULL == pclKI)
		return;
	
	// 거래 불가 소환수 확인.
	if(false == pclKI->bSummonTrade)
		return;

	if( pclSummon->siStatus == SUMMON_STATUS_DEAD || pclSummon->clIP.GetLife() <= 0 ) 
	{
		return;
	}

	//[진성] 이전 소환수 장비 착용 검사.
	/*if( pclCM->CR[ id ]->IsSummonHaveItem( pclinfo->siSummonIndex ) ) 
	{
		return;
	}*/

	cltSummonOrder clOrder;

	clOrder.siPersonID = personid;
	clOrder.siPrice = price;
	clOrder.clDate.Set( &pclTime->clDate );
	
	clOrder.clSummon.Set( pclSummon );
	clOrder.clSummon.siStatus = SUMMON_STATUS_TAKEIN; // 소환수가 나와있지 않는 상태로 만든다.


	SI16 pos = 0;
	cltItem clitem;

	clitem.Init();

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	// 지정 거래 대상자가 없는데 거래 종류가 일반 거래가 아니거나
	// 지정 거래 대상자가 있는데 거래 종류가 지정 거래가 아니면
	// => 잘못된 거래임
	if(     (pclinfo->szSelectPersonName[ 0 ] == NULL && pclinfo->uiTradeType != cltGameMsgRequest_SummonMarketSell::TRADE_TYPE_NORMAL) 
		||  (pclinfo->szSelectPersonName[ 0 ] != NULL && pclinfo->uiTradeType != cltGameMsgRequest_SummonMarketSell::TRADE_TYPE_SELECTIVE ) )
	{
        // 소환수 팔자 주문에 실패하였습니다.			
		SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELL, 0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
//#endif

	// 지정 거래일 경우,
	if( _tcslen(pclinfo->szSelectPersonName) > 0 ) 
	{
		SI16 itemunique = ITEMUNIQUE(24055) ; // 지정거래이용권

		if ( pclCM->CR[id]->pclCI->clCharItem.GetItemNum( itemunique ) <= 0 ) 
		{
			// 지정 거래 이용권 없음 
			SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELECTSELL_NOTHAVETICKET,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}

		pos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( itemunique );
		if( pos < 0 ) 
		{
			// 지정 거래 이용권 없음 
			SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELECTSELL_NOTHAVETICKET,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}

		CMinisterIzo* pclminister = (CMinisterIzo*)pclMinisterMgr->GetMinister(MINISTER_IZO);
		
		if( pclminister->m_siTradeLimitMoney > pclinfo->siPrice ) 
		{
			SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SELECTSELL_WRONGPRICE,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}


		clitem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[pos] );
		clitem.SetItemNum( 1 );

	}

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	sDBRequest_SummonMarketOrderSell sendMsg( id, villageunique, &clOrder, tax, pclinfo->szSelectPersonName, pos, &clitem, pclinfo->uiTradeType );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, tax);
//#endif

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_BUY(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonMarketBuy *pclinfo = (cltGameMsgRequest_SummonMarketBuy *)pclMsg->cData;

	GMONEY	price		 = 0;
	SI32	orderindex	 = 0;
	SI32	housevillage = 0;
	SI32	houseindex	 = 0;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// Person이 위치한 마을의 VillageUnique를 확보한다
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
		return ;

	if( pclCM->CR[ id ]->pclCI->clIP.GetLevel() < LIMIT_LEVEL_HATCH_DOLL ) 
		return;

	// 소환수를 더 보유할 수 있는가
	if( pclCM->CR[ id ]->pclCI->clSummonInfo.CanHaveSummon() == FALSE ) 
	{
		SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_BUY_NOMORE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	int count = 0;
	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_NUMBER; ++i ) 
	{
		if( pclSummonMarketManager->pclSummonOrder[ i ].siPersonID != 0 ) 
		{
			++count;

			if( pclSummonMarketManager->pclSummonOrder[ i ].siIndex == pclinfo->siOrderIndex ) 
			{

				if( pclSummonMarketManager->pclSummonOrder[ i ].siPersonID == personid ) 
				{
					SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_BUY_NOTSELF,  0, 0, pclCM->CR[id]->GetCharUnique());
					return;
				}


				orderindex = pclSummonMarketManager->pclSummonOrder[ i ].siIndex;
				price	   = pclSummonMarketManager->pclSummonOrder[ i ].siPrice;

				sDBRequest_SummonMarketBuy sendMsg( id, personid, villageunique, orderindex, price );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				return;
			}
		}

		if( count >= pclSummonMarketManager->siTotalOrderNumber ) 
			break;
	}

	SendServerResponseMsg( RANKTYPE_SUMMONMARKET, SRVAL_REALESTATEMARKET_FAIL_BUY_NOTVALID,  0, 0, pclCM->CR[id]->GetCharUnique());
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONMARKET_SETFEE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonMarketSetFee *pclinfo = (cltGameMsgRequest_SummonMarketSetFee *)pclMsg->cData;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;
	
	// 수수료 정보를 확보한다. 
	cltFee* pclfee = &pclinfo->clFee;
	// 수수료가 적절한지 확인한다. 
	if(pclfee->IsValid(1000) == FALSE)return;
	
	//----------------------------------
	// 요청자가 장예원행수인지 확인한다. 
	//----------------------------------
	cltSimpleRank clrank(RANKTYPE_SUMMONMARKET, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 수수료 설정에 실패. 
		SendServerResponseMsg(RANKTYPE_SUMMONMARKET, SRVAL_SUMMONMARKET_FAIL_SETFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	sDBRequest_SummonMarketSetFee clMsg(id, PersonID, villageunique, pclfee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}
