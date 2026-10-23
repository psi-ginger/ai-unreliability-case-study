#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-SummonHeroMarket.h"

#include "MsgRval-Define.h"
#include "..\CommonLogic\Msg\MsgType-SummonHeroMarket.h"	

#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/MinisterMgr.h"
#include "../Server/Minister/Minister-Izo.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_MYORDER(cltMsg* pclMsg, SI32 id )
{
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	sDBRequest_SummonHeroMarketMyOrder sendMsg( id, personid );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonHeroMarketCancelOrder *pclinfo = (cltGameMsgRequest_SummonHeroMarketCancelOrder *)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	sDBRequest_SummonHeroMarketCancelOrder sendMsg( id, personid, pclinfo->siOrderIndex );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_SummonHeroMarketOrderList *pclinfo = (cltGameMsgRequest_SummonHeroMarketOrderList *)pclMsg->cData;

	cltGameMsgResponse_SummonHeroMarketOrderList sendMsg;

	cltSummonOrder *pclOrder;
	int count = 0;

	sendMsg.usCount = 0;

	int i = 0;

	if( pclinfo->siSelectList == 1 ) {

		SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

		for( i = 0; i < MAX_SUMMONHEROMARKET_ORDER_NUMBER; ++i ) {

			pclOrder = &pclSummonHeroMarketManager->pclSummonOrder[ i ];
			
			if( pclOrder->siPersonID  &&
				pclOrder->siSelectPersonID == personid) {
				
				++count; 

				sendMsg.clOrder[ sendMsg.usCount ].Set( pclOrder );
				++sendMsg.usCount;
			}

			if( sendMsg.usCount == MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER ) break;

			if( count >= pclSummonHeroMarketManager->siTotalOrderNumber ) break;
		}
	}
	else
	{
		for( i = 0; i < MAX_SUMMONHEROMARKET_ORDER_NUMBER; ++i ) {

			pclOrder = &pclSummonHeroMarketManager->pclSummonOrder[ i ];
			
			if( pclOrder->siPersonID 
				&& pclOrder->clSummon.siKind == pclinfo->siKind 
				&& i > pclinfo->siStartPos ) {
				++count; 

				sendMsg.clOrder[ sendMsg.usCount ].Set( pclOrder );
				++sendMsg.usCount;
			}

			if( sendMsg.usCount == MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER ) break;

			if( count >= pclSummonHeroMarketManager->siTotalOrderNumber ) break;
		}
	}

	if( sendMsg.usCount > 0 ) {

		sendMsg.siNextStartPos = i;

		cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONHEROMARKET_ORDERLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_SELL(cltMsg* pclMsg, SI32 id )
{

	cltGameMsgRequest_SummonHeroMarketSell *pclinfo = (cltGameMsgRequest_SummonHeroMarketSell *)pclMsg->cData;

	GMONEY price = pclinfo->siPrice;
	GMONEY tax = price * 1 / 100 ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// Person이 위치한 마을의 VillageUnique를 확보한다
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	// 밖으로 나와있는 소환수의 정보를 주인에게 update 시킨다.
	((cltCharServer *)pclCM->CR[ id ])->UpdateTakeOutSummon();

	// 실제로 서버에서 소환수를 보유하고 있는가?
	cltSummon *pclSummon = pclCM->CR[ id ]->pclCI->clSummonInfo.GetSummon( pclinfo->siSummonIndex );
	if( pclSummon == NULL ) return;

	if( pclCM->CR[ id ]->pclCI->clSummonInfo.IsHaveEquipItem(pclinfo->siSummonIndex) ) {
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELL_HAVEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if( pclSummon->siKind == 0 ) { 
		
		//SendServerResponseMsg( RANKTYPR_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELL_NOTVALID,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	if( pclSummon->siStatus == SUMMON_STATUS_DEAD || pclSummon->clIP.GetLife() <= 0 ) 
	{
		return;
	}

	if( pclCM->CR[ id ]->pclCI->clSummonInfo.IsHaveEquipItem(pclinfo->siSummonIndex) ) {
		return;
	}

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
	if( (pclinfo->szSelectPersonName[ 0 ] == NULL && pclinfo->uiTradeType != cltGameMsgRequest_SummonHeroMarketSell::TRADE_TYPE_NORMAL) 
		||  (pclinfo->szSelectPersonName[ 0 ] != NULL && pclinfo->uiTradeType != cltGameMsgRequest_SummonHeroMarketSell::TRADE_TYPE_SELECTIVE ) )
	{
        // 소환영웅 팔자 주문에 실패하였습니다.			
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELL, 0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
//#endif

	// 지정 거래일 경우,
	if( pclinfo->szSelectPersonName[ 0 ] ) {
			
		SI16 itemunique = ITEMUNIQUE(24055) ; // 지정거래이용권  14160 ->  24055

		if ( pclCM->CR[id]->pclCI->clCharItem.GetItemNum( itemunique ) <= 0 ) {
			// 지정 거래 이용권 없음 
			SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_NOTHAVETICKET,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}

		pos = pclCM->CR[id]->pclCI->clCharItem.GetItemPos( itemunique );
		if( pos < 0 ) {
			// 지정 거래 이용권 없음 
			SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_NOTHAVETICKET,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}

		CMinisterIzo* pclminister = (CMinisterIzo*)pclMinisterMgr->GetMinister(MINISTER_IZO);
		
		if( pclminister->m_siTradeLimitMoney > pclinfo->siPrice ) {
			SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SELECTSELL_WRONGPRICE,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}


		clitem.Set( &pclCM->CR[id]->pclCI->clCharItem.clItem[pos] );
		clitem.SetItemNum( 1 );

	}

//#if defined(_LEON_ADD_SELECTSELL_CODE)
	sDBRequest_SummonHeroMarketOrderSell sendMsg( id, villageunique, &clOrder, tax, pclinfo->szSelectPersonName, pos, &clitem, pclinfo->uiTradeType );
	pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, tax);
	
//#endif

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_BUY(cltMsg* pclMsg, SI32 id )
{
	
	cltGameMsgRequest_SummonHeroMarketBuy *pclinfo = (cltGameMsgRequest_SummonHeroMarketBuy *)pclMsg->cData;

	GMONEY price = 0;
	SI32 orderindex = 0;
	SI32 housevillage = 0;
	SI32 houseindex = 0;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// Person이 위치한 마을의 VillageUnique를 확보한다
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	if( pclCM->CR[ id ]->pclCI->clIP.GetLevel() < LIMIT_LEVEL_HATCH_DOLL ) return;

	// 소환수를 더 보유할 수 있는가
	if( pclCM->CR[ id ]->pclCI->clSummonInfo.CanHaveSummon() == FALSE ) {
		SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOMORE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	int count = 0;
	for( int i = 0; i < MAX_SUMMONHEROMARKET_ORDER_NUMBER; ++i ) {
		if( pclSummonHeroMarketManager->pclSummonOrder[ i ].siPersonID != 0 ) {
			++count;

			if( pclSummonHeroMarketManager->pclSummonOrder[ i ].siIndex == 
				pclinfo->siOrderIndex ) {

				if( pclSummonHeroMarketManager->pclSummonOrder[ i ].siPersonID == personid ) {
					
					SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_BUY_NOTSELF,  0, 0, pclCM->CR[id]->GetCharUnique());
					return;
				}

				SI32 kind = pclSummonHeroMarketManager->pclSummonOrder[ i ].clSummon.siKind ;
				// 같은 국가 코드가 아니면 살 수가 없다.
				if ( pclCM->CR[id]->pclKI->siDefaultCountry != pclKindInfoSet->pclKI[kind]->siDefaultCountry )
				{
					return ;
				}

				orderindex = pclSummonHeroMarketManager->pclSummonOrder[ i ].siIndex;
				price = pclSummonHeroMarketManager->pclSummonOrder[ i ].siPrice;

				sDBRequest_SummonHeroMarketBuy sendMsg( id, personid, villageunique, orderindex, price );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

				return;
			}
		}

		if( count >= pclSummonHeroMarketManager->siTotalOrderNumber ) break;
	}

	SendServerResponseMsg( RANKTYPE_SUMMONHEROMARKET, SRVAL_REALESTATEMARKET_FAIL_BUY_NOTVALID,  0, 0, pclCM->CR[id]->GetCharUnique());

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONHEROMARKET_SETFEE(cltMsg* pclMsg, SI32 id )
{

	cltGameMsgRequest_SummonHeroMarketSetFee *pclinfo = (cltGameMsgRequest_SummonHeroMarketSetFee *)pclMsg->cData;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;
	
	// 수수료 정보를 확보한다. 
	cltFee* pclfee = &pclinfo->clFee;
	// 수수료가 적절한지 확인한다. 
	if(pclfee->IsValid(1000) == FALSE)return;
	
	//----------------------------------
	// 요청자가 비변사행수인지 확인한다. 
	//----------------------------------
	cltSimpleRank clrank(RANKTYPE_SUMMONHEROMARKET, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 수수료 설정에 실패. 
		SendServerResponseMsg(RANKTYPE_SUMMONHEROMARKET, SRVAL_SUMMONHEROMARKET_FAIL_SETFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}


	sDBRequest_SummonHeroMarketSetFee clMsg(id, PersonID, villageunique, pclfee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}
