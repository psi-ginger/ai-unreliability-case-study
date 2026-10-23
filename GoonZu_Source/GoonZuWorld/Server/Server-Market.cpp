//---------------------------------
// 2003/6/21 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "MsgRval-Define.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_SETMARKETFEE(cltMsg* pclMsg, SI32 id)
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
	// 요청자가 객주행수인지 확인한다. 
	//----------------------------------
	cltSimpleRank clrank(RANKTYPE_MARKET, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 수수료 설정에 실패. 
		SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAILSETFEE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	//-----------------------------------
	// DB로 전장 정보를 보낸다. 
	//-----------------------------------
	// DB 로 보낸다.

	sDBRequest_SetMarketFee clMsg(id, PersonID, villageunique, pclfee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}


void cltServer::DoMsg_GAMEMSG_REQUEST_SELLITEM(cltMsg* pclMsg, SI32 id)
{
	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;
	

	cltTradeOrder* pclOrder = (cltTradeOrder*)pclMsg->cData;
	
	// Person이 위치한 dy마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	// 새로운 내구도 시스템에는 밀봉을 체크하지 않는다.
	bool bIsBelonging = false;
	if( IsCountrySwitch( Switch_NewDurability) )
	{
		SI32 siForeverDurability	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
		SI32 siForeverDurability2	= (SI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );
		SI32 siGetMaxDurability		= pclOrder->clItemData.clItemCommon.clCommonInfo.uiMaxDurability;
		
		// 무한 내구도 물품이 아니면 무조건 귀속되지 않음
		if ( (siForeverDurability != siGetMaxDurability) && (siForeverDurability2 != siGetMaxDurability) )
		{
			bIsBelonging = false;
		}
		else
		{
			bIsBelonging = (pclOrder->clItemData.uiDateDay > 0 && pclOrder->clItemData.uiDateMonth > 0 && pclOrder->clItemData.uiDateDay > 0);
		}
	}
	else
	{
		bIsBelonging = (pclOrder->clItemData.uiDateDay > 0 && pclOrder->clItemData.uiDateMonth > 0 && pclOrder->clItemData.uiDateDay > 0);
	}

	//---------------------------------------------
	// 거래 가능 품목인지 확인한다. 
	//---------------------------------------------
	if( pclItemManager->IsItemInfoAtb(pclOrder->clItemData.siUnique, ITEMINFOATB_NOTTRADE) || 
		//===============================================================
		//cyj 귀속아이템이면 마켓에 못 올리도록
		//===============================================================
		bIsBelonging ||
		 //===============================================================
		pclOrder->clItemData.siUnique == 8150 || pclOrder->clItemData.siUnique == 14300 ||
		pclOrder->clItemData.siUnique == 14400 || pclOrder->clItemData.siUnique == 14500 )
	{
		SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------------------
	// 파티홀에서 받은 아이템은 거래 할 수 없다.
	//---------------------------------------------
	if( pclOrder->clItemData.clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
		return;
	
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
		if( ITEMUNIQUE(2755) == pclOrder->clItemData.siUnique || ITEMUNIQUE(2754) == pclOrder->clItemData.siUnique )	
		{	
			if( pclOrder->clItemData.siUnique != siFourLeafUnique )	
			{
				SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, pclchar->GetID() );
				return;	
			}
		}
	}

	// 가격이 적정 범위안인지 확인한다. 
	if(pclOrder->siPrice <= 0 || pclOrder->siPrice > MAX_PRICE_IN_MARKET)
		return ;

	// 세금을 낼 돈이 있는지 확인한다. 
	cltStrInfo* pclinfo = GetStrInfo( villageunique, RANKTYPE_MARKET );
	if(pclinfo == NULL)return ;

	SI64 totalmoney = (SI64)pclOrder->siPrice * (SI64)pclOrder->siAmount;
	// 시장 거래 제한금
	if( totalmoney > pclClient->GetGlobalValue("GV_MarketMoneyLimit")+1) return ;
	
	SI64 tax = pclinfo->clMarketStrInfo.clFee.CalcFee(totalmoney);

	//SI64 tax = ( (SI64)pclOrder->siPrice * (SI64)pclOrder->siAmount ) / 100;
	// 객주 프리미엄을 쓰면 tax는 0 안쓰면 소지금이랑 세금체크
	if( false == pclClient->pclCM->CR[id]->pclCI->clBI.bMarKetPremiumService )
	{
		if(tax > MIN_FEE)
		{
			if(pclCM->CR[id]->pclCI->clIP.GetMoney() < tax)return ;
		}
		else
		{
			tax = 0;
		}
	}
	else
		tax = 0;

	// PersonID를 업데이트 한다.
	pclOrder->siPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	
	// 주문 날짜를 확보한다. 
	pclOrder->siDate = pclTime->clDate.GetDateVary();

	// 팔자 주문용 아이템의 정보를 확보한다. 
	cltItem clItem;
	clItem.Set(&pclOrder->clItemData);

	// Person당 주문 개수가 적합한지 확인한다. 
	if( false == pclCM->CR[id]->pclCI->clBI.bMarKetPremiumService )
	{
		// 객주 프리미엄이 없다면 판매 가능한 물건은 4개
		if( pclCM->CR[id]->GetMarketOrderNumber() >= 4 )
		{
			SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAIL_ORDERSELL_OVERFLOW,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}
	else
	{
		// 객주 프리미엄이 있다면 판매 가능한 물건은 8개
		if( pclCM->CR[id]->GetMarketOrderNumber() >= MAX_MARKET_ORDER_PER_PERSON )
		{
			SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAIL_ORDERSELL_OVERFLOW,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}

	// 이미 올려진 물품인지 확인한다. 
	if( pclCM->CR[id]->pclCI->clMarket.IsInMarketOrder(clItem.siUnique) == true)
	{
		return ;
	}
	
	// 매수 주문을 넣는다. 
	SI32 orderref = -1;
	SI32 retval = 0;
	
	// DB의 팔자 주문 버퍼에 저장해 놓을 것을 요청한다. 
	SI32 itempos = pclCM->CR[id]->pclCI->clCharItem.FindItemInv(&clItem, true);
	
	SI32 itemunique = clItem.siUnique;

	if(itempos >= 0 && itemunique > 0 )
	{
		// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
		if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode == PSHOP_MODE_SELL)
		{
			for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
			{
				if( pclchar->clShopData.m_siSelectedInventoryPos[i] == itempos	)
				{
					// 상점 아이템이 실제로 존재하는지 체크 
					if( pclchar->clShopData.m_clItem[ i ].siUnique > 0 )
					{
						SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
						return ;
					}
				}
			}

			// 귀속전용 개인상점
			if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == itempos	)
			{
				// 상점 아이템이 실제로 존재하는지 체크 
				if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
				{
					SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
					return ;
				}
			}
		}
	}

	//기간제 아이템의 경우 남은 사용기간이 있는지 확인한다.
	if ( itemunique > 0 )
	{
		if ( clItem.IsHaveUseDay() )  //[영진] 남은 사용기간 없음 알림
		{
			SI32 useableDay = 0;
			BOOL rVal = GetItemUseAbleDay(&clItem,&useableDay);
			if(rVal == FALSE) return;

			if( useableDay < 0)		// 사용기간이 지난 아이템은 거래할 수 없다.
			{
				SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
				return;
			}
		}
	}

	if(itempos >= 0)
	{
		// 아이템 거래 통계를 낸다. 
		pclItemManager->AddTradeNum(pclOrder->GetItemUnique(), pclOrder->siAmount);

		// 새로운 통계 - 아이템 거래 (메모장타이틀, 해당아이템유니크, 메모장안의컬럼이름, 추가되는 수)
		NStatistics_Add("Item", pclOrder->GetItemUnique(),"TradeNum", pclOrder->siAmount);
		NStatistics_Set("Item", pclOrder->GetItemUnique(), "Name", pclItemManager->GetName(pclOrder->GetItemUnique()));
		
		// 퀘스트 정보를 업데이트 한다.
		((cltCharServer*)(pclCM->CR[id]))->IncQuestPara(QUEST_CLEAR_COND_UPITEMTOMARKET, clItem.siUnique  , pclOrder->siAmount);


		sDBRequest_OrderMarket	clMsg(id, ORDERMODE_SELL, villageunique, pclOrder, itempos, clItem.CanPile(pclItemManager), tax);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}

// 객주에서 물품 구입에 대한 처리를 한다. 
void cltServer::MarketBuyAction(SI16 id, SI16 villageunique, cltTradeOrder* pclOrder, SI16 siReason)
{

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// 팔자 주문을 확보한다. 
	SI32 orderref = -1;
	SI32 retval = 0;
	
	// 주문 리스트에서 인덱스의 주문 정보를 확보한다. 
	cltTradeOrder* pclorder = pclMarketManager->pclOrderManager->GetOrder(ORDERMODE_SELL, pclOrder->siIndex);
	if(pclorder == NULL)
	{
		if(pclchar)
		{
			SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  500, 0, pclchar->GetCharUnique());
			return ;
		}
	}
	
	// 가격이 틀리면 실패. 
	if(pclorder->siPrice != pclOrder->siPrice)
	{
		if(pclchar)
		{
		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  550, 0, pclchar->GetCharUnique());
		return ;
		}
	}
	
	// 실제 물품의 상세 정보가 틀리면 실패. 
	if(pclMarketManager->pclOrderManager->IsSameKind(pclorder, pclOrder) == false)
	{
		if(pclchar)
		{

		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  600, 0, pclchar->GetCharUnique());
		return ;
		}
	}
	
	// 거래 가격. (두 주문의  평균 가격) 
	GMONEY tradeprice		= pclorder->siPrice;
	
	cltItem clItem;
	clItem.Set(&pclOrder->clItemData);
	
	// 총 거래대금. 
	GMONEY totaltrademoney = tradeprice * pclOrder->siAmount;

	if(pclchar)
	{
		
		// 총 거래 대금이 보유 현금 한도를 초과하면 주문 실패. 
		if(totaltrademoney > pclCM->CR[id]->pclCI->clIP.GetMoney())
		{
			SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  700, 0, pclchar->GetCharUnique());
			return ;
		}
	}
	
	//---------------------------------------------
	// 구입후 개수가 최대 허용 개수를 초과하면 실패 
	//---------------------------------------------
/*	cltItem cltoitem;
	if(pclchar)
	{
		if(pclchar->pclCI->clCharItem.GetItemNum(clItem.siUnique) +  pclOrder->siAmount > MAX_ITEM_PILE_NUMBER)
		{
			SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  800, 0, pclchar->GetCharUnique());
			return ;
		}
	}
*/	
	//--------------------------------------------
	// 구입한 물품을 위치할 자리를 확인한다. 
	//--------------------------------------------
	SI16 EmptyPos	= 0;
	SI32 rtnvalue	= 0;
	cltItem clToItem;
	cltItem clitem;
	
	clitem.Set(&pclOrder->clItemData);
	
	// 아이템을 넣을 수 있는지 체크
	if(pclchar)
	{
		if ( ! pclchar->CanAddInv(INVMODE_ALL, &clitem, &EmptyPos, &clToItem , &rtnvalue) )
		{
			SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  900, 0, pclchar->GetCharUnique());
			return ;
		}
	}
	
	sDBRequest_TradeMarket clMsg(id, villageunique, clItem.CanPile(pclItemManager), pclOrder, pclorder->siIndex, tradeprice, EmptyPos, MARKETTRADE_REASON_NORMAL);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_BUYITEM(cltMsg* pclMsg, SI32 id)
{
	cltTradeOrder* pclOrder = (cltTradeOrder*)pclMsg->cData;

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)
	{
		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  100, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}


	if(pclOrder->siType != TRADEORDER_TYPE_OBJECT)
	{

		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  200, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//------------------------------------------
	// 원래 주문자와 자신이 같으면 물건을 살 수 없다. 
	//---------------------------------------------
	if(pclOrder->siPersonID == pclCM->CR[id]->pclCI->GetPersonID())
	{
		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  300, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// Person이 위치한 마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
	{
		SendServerResponseMsg(0, SRVAL_MARKET_SELL_FAIL,  400, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	if(pclClient->IsCountrySwitch(Switch_FourLeaf))
	{
		//[진성] 네잎크로버가 현재 사용할수 없는 거면 꺼내지 몬한다. => 2008-8-14
		if( ITEMUNIQUE(2755) == pclOrder->clItemData.siUnique || ITEMUNIQUE(2754) == pclOrder->clItemData.siUnique )	
		{	
			if( pclOrder->clItemData.siUnique != siFourLeafUnique )	
			{
				SendServerResponseMsg( 0, SRVAL_RETURN_NOT_USE_FOURLEAF, 0, 0, pclchar->GetID() );
				return;	
			}
		}
	}



	// PersonID를 업데이트 한다.
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	pclOrder->siPersonID = personid;
	
	MarketBuyAction( id, villageunique, pclOrder, MARKETTRADE_REASON_NORMAL );

}

// 객주의 주문 리스트를 요청한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_MARKETORDER(cltMsg* pclMsg, SI32 id)
{
	SI32 i, j;

	// 마지막 메시지 보낸 시간을 체크한다. 
	if(id && pclCM->CR[id]->CheckLastMsgClock(800) == true)
	{
		return ;
	}

	cltGameMsgRequest_MarketOrder* pclinfo = (cltGameMsgRequest_MarketOrder*)pclMsg->cData;	
	
	// 요구되는 아이템을 확보한다. 
	SI32 itemunique = pclinfo->siItemUnique;
	if(itemunique <= 0)return ;
	if(itemunique >= MAX_ITEM_UNIQUE)return ;

	GMONEY pricecond = pclinfo->siPriceCondition;
	if(pricecond < 0)pricecond = 0;

	// 아이템의 타입을 구한다. 
	SI32 itemtype = pclItemManager->GetItemType(itemunique);
	if(itemtype <= 0)return ;


	// 객주 주문에서 요구되는 아이템의 것들을 찾아서 클라이언트로 보낸다.
	SI32 searchnum			= pclMarketManager->siItemTypeOrderIndex[itemtype];
	cltTradeOrder** porder	= pclMarketManager->pclOrderManager->pclOrderSell;

	//-------------------------------------------
	// 현재 시세를 보낸다. 
	//-------------------------------------------
	{
		cltItemPriceUnit clunit;
		
		clunit.siUnique = itemunique;
		
		cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(itemunique);
		if(pclprice)
		{
			clunit.siPrice	= pclprice->siPrice;
			
			// 클라이언트로 정보를 보낸다.
			cltGameMsgResponse_ProductPrice clinfo(itemunique, clunit.siPrice);
			cltMsg clMsg(GAMEMSG_RESPONSE_PRODUCTPRICE, sizeof(clinfo), (BYTE*)&clinfo);
			
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}


	//-------------------------------------------------
	// 아이템 팔자 주문을 확보해서 보낸다. 
	//------------------------------------------------
	SI32 sendnum	=	0;
	cltTradeOrder	clorder[MARKET_ORDER_SEND_UNIT];
	SI32 index		= 0;
	SI32 ref		= 0;

	if (searchnum > MAX_MARKET_ORDER_NUMBER)		searchnum = MAX_MARKET_ORDER_NUMBER;
	for(i = 0;i < searchnum;i++)
	{
		ref = pclMarketManager->pclItemTypeOrder[itemtype][i];

		if( ref < 0 || ref >= MAX_MARKET_ORDER_NUMBER)	continue;
		if( porder[ref] == NULL)						continue;

		// 같은 유니크 값을 갖는다면, 
		if(porder[ref]->clItemData.siUnique != itemunique)	continue;
		if(sendnum >= MAX_RESPONSE_ORDER_NUMBER)			continue;
		if(pricecond && pricecond < porder[ref]->siPrice)	continue;	// 가격 제한 조건에 걸리는지 확인. 

		sendnum++;
		
		clorder[index].Set(porder[ref]);
		index++;
		
		// 꽉 찼으면 보낸다. 
		if(index == MARKET_ORDER_SEND_UNIT)
		{
			// 보낸다. 
			// 클라이언트로 정보를 보낸다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_MARKETORDER, sizeof(cltTradeOrder) * MARKET_ORDER_SEND_UNIT, (BYTE*)clorder);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			
			// 다시 초기화한다. 
			index	= 0;
			for(j  = 0;j < MARKET_ORDER_SEND_UNIT;j++)
			{
				clorder[j].Init();
			}
		}
		
	}

	// 남은 것을 보낸다. 
	if(index)
	{
		// 클라이언트로 정보를 보낸다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_MARKETORDER, sizeof(cltTradeOrder) * MARKET_ORDER_SEND_UNIT, (BYTE*)clorder);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	if(sendnum == 0)
	{
		for(j  = 0;j < MARKET_ORDER_SEND_UNIT;j++)
		{
			clorder[j].Init();
		}

		// 클라이언트로 정보를 보낸다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_MARKETORDER, sizeof(cltTradeOrder) * MARKET_ORDER_SEND_UNIT, (BYTE*)clorder);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// 물품 회수. 
void cltServer::DoMsg_GAMEMSG_REQUEST_GETFROMMARKET(cltMsg* pclMsg, SI32 id)
{	
	// 마지막 메시지 보낸 시간을 체크한다. 
	if(id && pclCM->CR[id]->CheckLastMsgClock(500) == true)
	{
		return ;
	}


	cltGetItemFromMarketInfo* pclInfo = (cltGetItemFromMarketInfo*)pclMsg->cData;
	
	// PersonID를 업데이트 한다.
	SI32 personid		= pclCM->CR[id]->pclCI->GetPersonID();

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();
	
	// 해당 person의 인벤에서 아이템을 옮겨놓을 적절한 위치를 찾는다. 
	SI16 sitopos;
	cltItem clto;
	SI32 rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pclInfo->clFromItem, &sitopos, &clto, &rtnvalue) == true)
	{
		// 클라이언트에서 기록되지 않은 정보는 서버에서 기록한다. (보안상의 이유. ) 
		pclInfo->siPersonID	= personid;
		pclInfo->siToPos	= sitopos;
		pclInfo->clToItem.Set(&clto);
		
		// DB메니저에게 아이템을 옮겨놓을 것을 요청한다.
		sDBRequest_GetItemFromMarket clMsg(id, pclInfo);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		
	}
	// 적절한 위치를 찾을 수 없다면 실패. 
	else
	{
		SendServerResponseMsg(RANKTYPE_MARKET, SRVAL_MARKET_FAIL_GETFROMMARKET,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}	
}
void cltServer::DoMsg_GAMEMSG_REQUEST_PRODUCTPRICE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ProductPrice* pclInfo = (cltGameMsgRequest_ProductPrice*)pclMsg->cData;

	cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(pclInfo->siItemUnique);
	if(pclprice)
	{
		// 클라이언트로 정보를 보낸다.
		cltGameMsgResponse_ProductPrice clinfo(pclInfo->siItemUnique, pclprice->siPrice, pclInfo->siDialogID );
		cltMsg clMsg(GAMEMSG_RESPONSE_PRODUCTPRICE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWMARKETACCOUNT(cltMsg* pclMsg, SI32 id)
{
	if( sTime.wHour == 5 && ( sTime.wMinute >= 50 && sTime.wMinute <= 59 ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	if( false == pclCM->IsValidID(id) )
		return;

	//--------------------------------------------
	// 서버에 렉이 있으면 중지 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;

	SI32 personID = pclCM->CR[id]->pclCI->GetPersonID();
	if( 0 < personID )
	{
		sDBRequest_NewMarketAccount	clMsg( id, siServerUnique, personID );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWMARKETITEMLIST(cltMsg* pclMsg, SI32 id)
{
	if( sTime.wHour == 5 && ( sTime.wMinute >= 50 && sTime.wMinute <= 59 ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	if( false == pclCM->IsValidID(id) )
		return;

	//--------------------------------------------
	// 서버에 렉이 있으면 중지 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;

	cltGameMsgRequest_NewMarketItemList* pclinfo = (cltGameMsgRequest_NewMarketItemList*)pclMsg->cData;

	SI32 itemunique = pclinfo->siItemUnique;
	if(itemunique <= 0)return ;
	if(itemunique >= MAX_ITEM_UNIQUE)return ;

	if( 0 < pclinfo->siRequestPage )
	{
		sDBRequest_NewMarketItemList	clMsg( id, pclinfo->uiType, itemunique, pclinfo->siRequestPage );
		pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_NEWSELLITEM(cltMsg* pclMsg, SI32 id)
{
	if( sTime.wHour == 5 && ( sTime.wMinute >= 50 && sTime.wMinute <= 59 ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	//--------------------------------------------
	// 서버에 렉이 있으면 중지 
	//--------------------------------------------
	if(dwDBLatency > 3000)return ;

	cltGameMsgRequest_NewSellItem * pclmsg = (cltGameMsgRequest_NewSellItem*)pclMsg->cData ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
	if( NULL == pclchar ) return;

	pclchar->ItemUseReserveAction();	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	pclchar->BulletUseReserveAction();	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 

	if(false == IsUseFourLeaf(0, pclchar)) return;


	cltNewTradeOrder pclOrder ;
	pclOrder.Init();
	pclOrder.siServerUnique = siServerUnique;
	pclOrder.siPersonID = pclCM->CR[id]->pclCI->GetPersonID();
	pclOrder.siAmount = pclmsg->clTradeOrder.siAmount;
	pclOrder.siDate = pclmsg->clTradeOrder.siDate;
	pclOrder.siPrice = pclmsg->clTradeOrder.siPrice;
	pclOrder.clItemData.Set(&pclmsg->clTradeOrder.clItemData);

	// Person이 위치한 마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)return ;

	//---------------------------------------------
	// 거래 가능 품목인지 확인한다. 
	//---------------------------------------------
	if( false == pclItemManager->IsNewMarketTrade( pclOrder.clItemData.siUnique ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	if(	true == pclItemManager->IsItemInfoAtb(pclOrder.clItemData.siUnique, ITEMINFOATB_NOTTRADE) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclOrder.clItemData.siUnique);
	if ( ref )
	{
		SI32 itemtype = pclItemManager->pclItemInfo[ref]->siType ;
		// 오브젝트 물건이냐?
		if ( pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
		{
			if(false == pclOrder.clItemData.IsSealingUp(pclClient->pclItemManager))	
			{
				SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
				return;
			}
		}
	}

	SI32 itemRef = pclItemManager->FindItemRefFromUnique(pclOrder.clItemData.siUnique);

	if( itemRef <= 0 || itemRef >= MAX_ITEMINFO_NUMBER ) return;
	if( pclItemManager->pclItemInfo[ itemRef ]->IsItemInfoAtb(ITEMINFOATB_NOTTRADE) == true )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	// 가격이 적정 범위안인지 확인한다. 
	if(pclOrder.siPrice <= 0 || pclOrder.siPrice > MAX_PRICE_IN_MARKET)return ;

	// 세금을 낼 돈이 있는지 확인한다. 

	cltStrInfo* pclinfo = GetStrInfo( villageunique, RANKTYPE_NEWMARKET );
	if(pclinfo == NULL)return ;

	GMONEY totalmoney = (SI64)pclOrder.siPrice * (SI64)pclOrder.siAmount;

	GMONEY tax = pclinfo->clNewMarketStrInfo.clFee.CalcFee(totalmoney);
	if(pclCM->CR[id]->pclCI->clIP.GetMoney() < tax)	
		return;
		
	// PersonID를 업데이트 한다.
	pclOrder.siPersonID = pclCM->CR[id]->pclCI->GetPersonID();

	// 주문 날짜를 확보한다. 
	pclOrder.siDate = pclTime->clDate.GetDateVary();

	// 팔자 주문용 아이템의 정보를 확보한다. 
	cltItem clItem;
	clItem.Set(&pclOrder.clItemData);

	SI32 itempos = pclmsg->siPos;
	if(itempos >= 0)
	{
		if ( pclCM->CR[id]->pclCI->clCharItem.clItem[itempos].IsSame(&clItem,true) )
		{
			sDBRequest_NewOrderMarket	clMsg(id,  villageunique, &pclOrder, itempos, clItem.CanPile(pclItemManager), tax, 0);
			pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GETFROMNEWMARKET(cltMsg* pclMsg, SI32 id)
{
	if( sTime.wHour == 5 && ( sTime.wMinute >= 50 && sTime.wMinute <= 59 ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)
	{
		return ;
	}

	// 물품 찾기는 객주꺼 그대로 쓴다
	cltGetItemFromNewMarketInfo* pclInfo = (cltGetItemFromNewMarketInfo*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
	if( NULL == pclchar ) return;

	pclchar->ItemUseReserveAction();	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	pclchar->BulletUseReserveAction();	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 

	if(false == IsUseFourLeaf(0, pclchar)) return;

	// PersonID를 업데이트 한다.
	SI32 personid = pclchar->pclCI->GetPersonID();

	if( 0 > personid )
		return;

	if( 0 > pclInfo->siOrderIndex )
		return;

	// 해당 person의 인벤에서 아이템을 옮겨놓을 적절한 위치를 찾는다. 
	SI16	sitopos;
	cltItem clto;
	SI32	rtnvalue;
	if(pclchar->CanAddInv(INVMODE_ALL, &pclInfo->clFromItem, &sitopos, &clto, &rtnvalue) == true)
	{
		// 클라이언트에서 기록되지 않은 정보는 서버에서 기록한다. (보안상의 이유. ) 
		sDBRequest_GetItemFromNewMarket clMsg( id, siServerUnique, personid,  pclInfo->siOrderIndex, pclInfo->bPileSwitch, &pclInfo->clFromItem, sitopos);
		pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
	}
	// 적절한 위치를 찾을 수 없다면 실패. 
	else
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_GETFROMMARKET,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_NEWBUYITEM(cltMsg* pclMsg, SI32 id)
{
	if( sTime.wHour == 5 && ( sTime.wMinute >= 50 && sTime.wMinute <= 59 ) )
	{
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_NOTRADEITEM,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;		
	}

	//--------------------------------------------
	// 서버에 렉이 있으면 아이템을 사용하지 않는다. 
	//--------------------------------------------
	if(dwDBLatency > 3000)
	{
		SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  100, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	cltNewTradeOrder* pclOrder = (cltNewTradeOrder*)pclMsg->cData;

	cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
	if( NULL == pclchar ) return;

	pclchar->ItemUseReserveAction();	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	pclchar->BulletUseReserveAction();	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 

	if(false == IsUseFourLeaf(0, pclchar)) return;


	//------------------------------------------
	// 원래 주문자와 자신이 같으면 물건을 살 수 없다. 
	//---------------------------------------------
	if( pclOrder->siPersonID == pclCM->CR[id]->pclCI->GetPersonID() &&
		pclOrder->siServerUnique == siServerUnique ) 
	{
		SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  300, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// Person이 위치한 마을의 VillageUnique를 확보한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
	{
		SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  400, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// PersonID를 업데이트 한다.
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 거래 가격. 
	SI64 tradeprice	= pclOrder->siPrice;

	cltItem clItem;
	clItem.Set(&pclOrder->clItemData);

	// 기간이 이상한 놈들 것도 못 구입한다.
	SI16 date = (SI16)clItem.uiDateUseDay ;
	if ( date < 0 )
	{
		if ( pclchar )
		{
			SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  600, 0, pclchar->GetCharUnique());
		}
		return ;
	}

	// 총 거래대금. 
	SI64 totaltrademoney = tradeprice * pclOrder->siAmount;

	if(pclchar)
	{
		// 총 거래 대금이 보유 현금 한도를 초과하면 주문 실패. 
		if(totaltrademoney > pclCM->CR[id]->pclCI->clIP.GetMoney())
		{
			SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  700, 0, pclchar->GetCharUnique());
			return ;
		}
	}

	//---------------------------------------------
	// 구입후 개수가 최대 허용 개수를 초과하면 실패 
	//---------------------------------------------
	cltItem cltoitem;
	if(pclchar)
	{
		if(pclchar->pclCI->clCharItem.GetItemNum(clItem.siUnique) +  pclOrder->siAmount > 100000 )
		{
			SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  800, 0, pclchar->GetCharUnique());
			return ;
		}
	}

	//--------------------------------------------
	// 구입한 물품을 위치할 자리를 확인한다. 
	//--------------------------------------------
	SI16 EmptyPos	= 0;
	SI32 rtnvalue	= 0;
	cltItem clToItem;
	cltItem clitem;

	clitem.Set(&pclOrder->clItemData);

	// 아이템을 넣을 수 있는지 체크
	if(pclchar)
	{
		if ( ! pclchar->CanAddInv(INVMODE_ALL, &clitem, &EmptyPos, &clToItem , &rtnvalue) )
		{
			SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  900, 0, pclchar->GetCharUnique());
			return ;
		}
	}

	sDBRequest_NewTradeMarket clMsg(id, siServerUnique, villageunique, personid, pclOrder, tradeprice,  EmptyPos, clItem.CanPile(pclItemManager), clItem.IsCreateSID(pclItemManager), &sTime );
	pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETNEWMARKETFEE(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
	if( NULL == pclchar ) return;

	// 하루에 한번만 수수료 설정이 가능하게.
	if( false == pclNewMarketManager->m_bSetFee ) 
	{ 
		SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAILSETFEE,  0, 0, pclchar->GetCharUnique());
		return;
	}

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageUnique = pclchar->GetCurrentVillageUnique();
	if( 0 < villageUnique && MAX_VILLAGE_NUMBER >= villageUnique )
	{
		// 수수료 정보를 확보한다. 
		cltFee* pclfee = (cltFee*)pclMsg->cData;
		
		// 통합마켓 수수료는 5% ~ 15%
		if( MIN_FEE_RATE_NEWMARKET*10 <= pclfee->GetFeeRate() && MAX_FEE_RATE_NEWMARKET*10 >= pclfee->GetFeeRate() )
		{
			//----------------------------------
			// 요청자가 객주행수인지 확인한다. 
			//----------------------------------
			cltSimpleRank clrank(RANKTYPE_NEWMARKET, villageUnique);
			if( FALSE == pclRankManager->IsRank(pclchar->pclCI->GetPersonID(), &clrank) )
			{
				// 수수료 설정에 실패. 
				SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_MARKET_FAILSETFEE,  0, 0, pclchar->GetCharUnique());
				return ;
			}

			// DB 로 보낸다.
			sDBRequest_SetNewMarketFee clMsg(id, pclchar->pclCI->GetPersonID(), villageUnique, pclfee);
			pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
		}
	}
}




