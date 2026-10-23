//----------------------------
// SJY 2005/12/06
//----------------------------
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"

#include "MsgRval-Define.h"

#include "../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"
#include "../CommonLogic/Msg/MsgType-WorldMoney.h"
#include "../CommonLogic/WorldTraderMgr/WorldTraderManager.h"
#include "../CommonLogic/WorldPassport/WorldPassport.h"

#include "..\Server\Minister\MinisterBase.h"
#include "..\Server\Minister\Minister-Izo.h"
#include "..\Server\Minister\MinisterMgr.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_DISTRIBUTION_WORLDMONEY( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_Distrubution_WorldMoney* pclinfo = (cltGameMsgRequest_Distrubution_WorldMoney*)pclMsg->cData;

	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	SI64 totalmoney = pclinfo->siWorldMoney * pclinfo->siWorldMoneyPrice ;
	if ( pclDistributionWorldMoney->IsCorrectDistribution(pclinfo->siWorldMoney,pclinfo->siWorldMoneyPrice) == FALSE ||
		pclchar->pclCI->clIP.GetMoney() < totalmoney || pclchar->pclCI->clIP.GetLevel() < FATHER_LEVEL )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_DIS_WORLDMONEY,  0, 0, id);
		return ;
	}

	sDBRequest_Distribution_WorldMoney clMsg((UI16)pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),totalmoney,pclinfo->siWorldMoney,INPUTREASON_WORLDMONEY_DISTRIBUTION);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return ;
}


void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDMONEY_TRADELIST(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if( id <= 0 )
		//&& pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	for ( SI32 i = 0 ; i < ( MAX_WORLDMONEY_TRADELIST_NUM / MAX_WORLDMONEY_TRADELIST_PER_SEND ) ; i ++ )
	{
#ifdef _SAFE_MEMORY
		NSafeTArray<cltWorldMoneyTradeList, MAX_WORLDMONEY_TRADELIST_PER_SEND>	clList;
#else
		cltWorldMoneyTradeList clList[MAX_WORLDMONEY_TRADELIST_PER_SEND];
#endif

		for ( SI32 j = 0 ; j < MAX_WORLDMONEY_TRADELIST_PER_SEND ; j ++ )
		{
			if( (i*MAX_WORLDMONEY_TRADELIST_PER_SEND+j) >= 0 && (i*MAX_WORLDMONEY_TRADELIST_PER_SEND+j) < MAX_WORLDMONEY_TRADELIST_NUM )
				clList[j].Set(&pclDistributionWorldMoney->clTradeList[i*MAX_WORLDMONEY_TRADELIST_PER_SEND+j]);
		}

		bool bEnd = false ;
		if ( i == ( MAX_WORLDMONEY_TRADELIST_NUM / MAX_WORLDMONEY_TRADELIST_PER_SEND ) - 1 )
			bEnd = true ;

		//cltGameMsgResponse_WorldMoney_TradeList clinfo(clList,i,bEnd);
#ifdef _SAFE_MEMORY
		cltGameMsgResponse_WorldMoney_TradeList clinfo(&clList[0],i,bEnd,pclDistributionWorldMoney->siAverageWorldMoneyPrice);
#else
		cltGameMsgResponse_WorldMoney_TradeList clinfo(clList,i,bEnd,pclDistributionWorldMoney->siAverageWorldMoneyPrice);
#endif

		cltMsg clMsg(GAMEMSG_RESPONSE_WORLDMONEY_TRADELIST, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDMONEY_SELLORDER(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	cltGameMsgRequest_WorldMoney_SellOrder* pclinfo = (cltGameMsgRequest_WorldMoney_SellOrder*)pclMsg->cData;

	// 이미 올린 아큐가 있다
	if ( pclchar->pclCI->clWorldMoney.siTradeWorldMoney != 0 &&
		pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice != 0 )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_SELLORDER,  0, 0, id);
		return ;
	}

	// 올리려고 한 아큐보다 적게 가지고 있다.
	if ( pclinfo->siWorldMoney > pclchar->pclCI->clWorldMoney.siWorldMoney )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_SELLORDER,  0, 0, id);
		return ;
	}

	// 수수료가 있냐?
	SI64 fee = pclinfo->siWorldMoney * pclinfo->siPrice / 100 ; // 세금은 1%
	if ( pclchar->pclCI->clIP.GetMoney() < fee )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_SELLORDER,  0, 0, id);
		return ;
	}

	if ( pclinfo->siPrice <= 0 || pclinfo->siWorldMoney <= 0 )
		return ;

	SI32 gamedate = pclTime->clDate.GetDateVary();

	sDBRequest_WorldMoney_SellOrder clMsg((UI16)pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),pclinfo->siPrice,pclinfo->siWorldMoney,fee,gamedate);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_TAX, fee);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDMONEY_BUYORDER(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	cltGameMsgRequest_WorldMoney_BuyOrder* pclinfo = (cltGameMsgRequest_WorldMoney_BuyOrder*)pclMsg->cData;

	if ( pclinfo->siIndex < 0 || pclinfo->siIndex >= MAX_WORLDMONEY_TRADELIST_NUM )
		return ;

	// 실제로 있는 주문이냐?
	if ( pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPersonID <= 0 ||
		pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siWorldMoney <= 0 ||
		pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPrice <= 0 )
	{
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_BUYORDER,  0, 0, id);
		return ;
	}

	if ( pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPersonID == pclchar->pclCI->GetPersonID())
	{
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_BUYORDER_MYSELF, 0, 0, id);
		return;
	}

	SI64 realBuyWorldMoney = pclinfo->siWorldMoney ;
	if ( pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siWorldMoney < pclinfo->siWorldMoney )
		realBuyWorldMoney = pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siWorldMoney ;

	// 돈이 있냐?
	SI64 totalMoney = realBuyWorldMoney * pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPrice ;
	if ( pclchar->pclCI->clIP.GetMoney() < totalMoney )
	{
		//SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_BUYORDER,  0, 0, id);
		SendServerResponseMsg(0, SRVAL_FAIL_WORLDMONEY_BUYORDER_NOMONEY,  0, 0, id);

		return ;
	}

	// 100억 아큐를 넘어선 안된다.
	if ( pclchar->pclCI->clWorldMoney.GetWorldMoney() + realBuyWorldMoney > 10000000000 )
		return ;

	sDBRequest_WorldMoney_BuyOrder clMsg((UI16)pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),pclinfo->siIndex,
		realBuyWorldMoney,pclDistributionWorldMoney->clTradeList[pclinfo->siIndex].siPrice);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDMONEY_CANCELORDER(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	// 실제 올린 주문이 있냐?
	if ( pclchar->pclCI->clWorldMoney.siTradeWorldMoney <= 0 ||
		pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice <= 0 )
		return ;

	sDBRequest_WorldMoney_CancelOrder clMsg((UI16)pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID());
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

// TradeServer
// 팔기 주문 등록
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_SET_SELLORDER(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_TradeServer_Set_SellOrder* pclinfo = (cltGameMsgRequest_TradeServer_Set_SellOrder*)pclMsg->cData;

	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	if ( bTradeServer == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_GM,0,0,pclchar->GetCharUnique());
		return ;
	}

	// 현재 무역 거래중이면 안된다!
	if ( pclchar->bTradeServerUsing == true )
	{
		return ;
	}

	// 개인상점 개설중이며 개인상점에 출품된 아이템이라면 팔수 없다.
	if( pclchar->bPersonalShopSwitch == true && pclchar->clShopData.m_siShopMode ==PSHOP_MODE_SELL)
	{
		for( SI16 i = 0; i < MAX_SHOP_ITEM_NUMBER ; i++)
		{
			if( pclchar->clShopData.m_siSelectedInventoryPos[i] == pclinfo->siPos	)
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
		if( pclchar->clShopData.m_siSelectedSpacialInventoryPos == pclinfo->siPos	)
		{
			// 상점 아이템이 실제로 존재하는지 체크 
			if( pclchar->clShopData.m_clSpacialItem.siUnique > 0 )
			{
				SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, id );
				return ;
			}
		}
	}


	// 다른나라의 특산품일경우 되팔수 없음
	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) )
		{
			if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
			{
				return;
			}
		}
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) )
		{
			if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) == false )
			{
				return;
			}
		}
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) )
		{
			if ( pclItemManager->IsItemInfoAtb( pclinfo->clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
			{
				return;
			}
		}
	}

	// 시간제한
	if ( sTime.wHour < siTradeServerStartTime || sTime.wHour > siTradeServerEndTime )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_LIMITTIME,siTradeServerStartTime,siTradeServerEndTime,pclchar->GetCharUnique());
		return ;
	}

	// 트레이드 서버랑 연결되어 있냐?
	if ( pclNTradeServerProcess == NULL || pclNTradeServerProcess->GetTradeServerServersSession()->GetState() != SESSION_STATE_ESTABLISHED )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_SESSION,0,0,pclchar->GetCharUnique());
		return ;
	}

	cltTradeServerOrder clOrder;
	clOrder.Init();
	clOrder.clitem.siUnique = pclinfo->clitem.siUnique;
	clOrder.clitem.siItemNum = pclinfo->clitem.siItemNum;
	clOrder.clitem.siPrice = pclinfo->siPrice ;
	clOrder.clitem.siType = pclinfo->clitem.Element.siType;
	clOrder.clitem.siPower = pclinfo->clitem.Element.siPower;
	clOrder.clitem.siRareType = pclinfo->clitem.clItemCommon.clCommonInfo.uiRareInfo ;
	memcpy(clOrder.clitem.szItemData,pclinfo->clitem.cData8,MAX_ITEM_UNION_BYTE);

	clOrder.siIndex = 0 ;
	clOrder.siPersonID = pclchar->pclCI->GetPersonID();
	MStrCpy(clOrder.szAccountID,pclchar->pclCI->clBI.szAccountID,MAX_PLAYER_NAME);
	//MStrCpy(clOrder.szServerName,szServerName,20);
	MStrCpy(clOrder.szCharName,pclchar->GetName(),20);

	clOrder.siCharNation = GetTradeServer_CharNation( pclClient->pclCM->GetCharServer(id) );//pclClient->pclCM->CR[id]->pclCI->clBI.siCountryID;
	if( clOrder.siCharNation == -1 )		return;

	NTCHARString<4> kServerNation;
	kServerNation = GetTradeServer_ServerNation( pclClient->siServiceArea );
	if( kServerNation.Length() <= 0 )		return;

	MStrCpy(clOrder.szServerNation, kServerNation, 4);

	/*
#ifdef _DEBUG
	MStrCpy(clOrder.szServerNation,"tst",4);
#else
	if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		clOrder.siCharNation = 1;
		MStrCpy(clOrder.szServerNation,"jpn",4);
	}
	else if (pclClient->siServiceArea == ConstServiceArea_English)
	{
		if( clOrder.siCharNation == -1  ) clOrder.siCharNation = 2;
		MStrCpy(clOrder.szServerNation,"eng",4);
	}
#endif
	*/

	clOrder.siServerIndex = siServerUnique ;


	// 거래가능한 아이템이냐? 숫자는 맞냐?
	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clitem.siUnique);
	if ( ref <= 0 || ref >= MAX_ITEMINFO_NUMBER ) return ;
	if ( pclItemManager->pclItemInfo[ref]->siTradeServerMinNum <= 0 ) return ;
	if ( pclItemManager->pclItemInfo[ref]->siTradeServerMinNum > pclinfo->clitem.siItemNum ) return ;

	// 수수료로 지불할 아큐를 가지고 있냐?
	SI64 fee = clOrder.clitem.siPrice / 100 ;
	if ( pclchar->pclCI->clWorldMoney.siWorldMoney < fee )
		return;

	if ( pclinfo->siPos < PERSONITEM_INV0 || pclinfo->siPos >= MAX_ITEM_PER_PERSON )
		return ;

	// 이미 이동중인 것은 안된다!
	if ( pclchar->GetNoMoveItemSwitch(pclinfo->siPos) != 0 )
		return ;

	if ( pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].siUnique != pclinfo->clitem.siUnique )
		return ;

	// 실제로 가지고 있나?
	if ( pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].siItemNum < pclinfo->clitem.siItemNum )
		return ;

	clOrder.clitem.siRank = 0 ;
	SI32 itemtype = pclItemManager->pclItemInfo[ref]->siType ;
	if( itemtype < 0 || itemtype >= MAX_ITEMTYPE_NUMBER )	return;

	// OBJECT 물건이면
	if ( pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
	{
		if ( clOrder.clitem.siItemNum != 1 )
			return ;
		// 실제로 같은 물건인가?
		if ( pclchar->pclCI->clCharItem.clItem[pclinfo->siPos].IsSame(&pclinfo->clitem,true) )
		{
			clOrder.clitem.siRank = pclItemManager->GetTradeServerObjectRank(&pclinfo->clitem);

			if ( clOrder.clitem.siRank < 0 || clOrder.clitem.siRank >= MAX_TRADESERVER_OBJECT_RANK )
				return ;
		}
		else return ;
	}

	// 트레이드중인 것이니 아이템 이동을 잠그자!
	pclchar->SetNoMoveItemSwitch(pclinfo->siPos,true);

	// 트레이드 서버로 팔기 주문 요청하자.

	if ( true == IsCountrySwitch( Switch_Trade_Renewal ) )
	{
		// 여권이 없으면 판매 신청을 못한다
		if ( false == pclchar->pclCI->clWorldPassport.IsHavePassport() )
		{
			return;
		}
		
		// 물건을 팔아도 아큐가 무역서버에 저장되도록 한다
		NTradeServerRequest_Set_SellOrder_HoldAcu clMsg( pclchar->GetCharUnique(), pclinfo->siPos, &clOrder, true );
		pclNTradeServerProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		NTradeServerRequest_Set_SellOrder clMsg(pclchar->GetCharUnique(),pclinfo->siPos,&clOrder);
		pclNTradeServerProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	
	// 현재 무역 서버로 패킷 보냈다고 알려준다.
	pclchar->bTradeServerUsing = true ;
}

// 주문 취소
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_CANCEL_SELLORDER(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	if ( bTradeServer == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_GM,0,0,pclchar->GetCharUnique());
		return ;
	}

	// 현재 무역 거래중이면 안된다!
	if ( pclchar->bTradeServerUsing == true )
	{
		return ;
	}

	// 시간제한
	if ( sTime.wHour < siTradeServerStartTime || sTime.wHour > siTradeServerEndTime )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_LIMITTIME,siTradeServerStartTime,siTradeServerEndTime,pclchar->GetCharUnique());
		return ;
	}

	// 트레이드 서버랑 연결되어 있냐?
	if ( pclNTradeServerProcess == NULL || pclNTradeServerProcess->GetTradeServerServersSession()->GetState() != SESSION_STATE_ESTABLISHED )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_SESSION,0,0,pclchar->GetCharUnique());
		return ;
	}

	cltGameMsgRequest_TradeServer_Cancel_SellOrder* pclinfo = (cltGameMsgRequest_TradeServer_Cancel_SellOrder*)pclMsg->cData ;

	if ( pclinfo->siIndex < 0 || pclinfo->siIndex >= MAX_TRADESERVER_ORDER_LIST )
	{
		pclLog->FilePrint("TRADESERVER_CANCEL_SELLORDER_ERROR.txt", "pclinfo->siIndex Error");
		return ;
	}

	if ( pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siUnique != pclinfo->siItemUnique )
	{
		pclLog->FilePrint("TRADESERVER_CANCEL_SELLORDER_ERROR.txt", "%d %d %d clitem.siUnique Error", pclinfo->siIndex,
			pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siUnique,pclinfo->siItemUnique );

		return ;
	}
	if ( pclWorldTraderManager->clOrder[pclinfo->siIndex].siPersonID != pclchar->pclCI->GetPersonID() )
	{
		pclLog->FilePrint("TRADESERVER_CANCEL_SELLORDER_ERROR.txt", "%d %d siPersonID Error", pclWorldTraderManager->clOrder[pclinfo->siIndex].siPersonID,
			pclchar->pclCI->GetPersonID() );
		return ;
	}

	cltItem clitem;
	clitem.Init();

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->siItemUnique);
	if ( ref )
	{
		SI32 itemtype = pclItemManager->pclItemInfo[ref]->siType ;
		// 오브젝트 물건이냐?
		if ( pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
		{
			if ( pclItemManager->MakeTradeServerObjectForRank(&clitem,pclinfo->siItemUnique,pclinfo->siItemNum,
				pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siRank,
				pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siRareType,
				pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siType,
				pclWorldTraderManager->clOrder[pclinfo->siIndex].clitem.siPower) == FALSE )
				return ;
		}
		else
		{
			clitem.siUnique = pclinfo->siItemUnique ;
			clitem.siItemNum = pclinfo->siItemNum ;
		}
	}
	else return ;

	cltItem cltoitem;
	cltoitem.Init();
	SI16 pos = 0 ;
	SI32 returnvalue = 0 ;

	if ( pclchar->CanAddInv(INVMODE_ALL,&clitem,&pos,&cltoitem,&returnvalue) == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_BUY_FULLINV,0,0,pclchar->GetCharUnique());
		return ;
	}

	//NTradeServerRequest_Cancel_SellOrder clMsg(pclchar->GetCharUnique(),&pclWorldTraderManager->clOrder[pclinfo->siIndex]);

	// 들어갈 자리를 예약한다.
	pclchar->SetInputItemReserve(pos,CurrentClock);

	NTradeServerRequest_Cancel_SellOrder clMsg(pclchar->GetCharUnique(),&pclWorldTraderManager->clOrder[pclinfo->siIndex],pos);
	pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);

	// 현재 무역 서버로 패킷 보냈다고 알려준다.
	pclchar->bTradeServerUsing = true ;
}

// 사기 주문
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_SET_BUYORDER(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	// 개편된 무역에서는 여권이 꼭 있어야 한다
	if ( true == IsCountrySwitch(Switch_Trade_Renewal) )
	{
		if ( false == pclchar->pclCI->clWorldPassport.IsHavePassport() )
		{
			return;
		}
	}

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	if ( bTradeServer == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_GM,0,0,pclchar->GetCharUnique());
		return ;
	}

	// 시간제한
	if ( sTime.wHour < siTradeServerStartTime || sTime.wHour > siTradeServerEndTime )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_LIMITTIME,siTradeServerStartTime,siTradeServerEndTime,pclchar->GetCharUnique());
		return ;
	}

	// 현재 무역 거래중이면 안된다!
	if ( pclchar->bTradeServerUsing == true )
	{
		return ;
	}

	// 트레이드 서버랑 연결되어 있냐?
	if ( pclNTradeServerProcess == NULL || pclNTradeServerProcess->GetTradeServerServersSession()->GetState() != SESSION_STATE_ESTABLISHED )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_SESSION,0,0,pclchar->GetCharUnique());
		return ;
	}

	cltGameMsgRequest_TradeServer_Set_BuyOrder* pclinfo = (cltGameMsgRequest_TradeServer_Set_BuyOrder*)pclMsg->cData ;
	if( pclinfo->clOrder.siIndex < 0 || pclinfo->clOrder.siIndex >= MAX_TRADESERVER_ORDER_LIST )	return;

	if ( pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex].IsSame(&pclinfo->clOrder) == false )
		return ;

	SI64 price = pclinfo->clOrder.clitem.siPrice ;
	if ( pclchar->pclCI->clWorldMoney.siWorldMoney < price )
		return ;

	cltItem clitem;
	clitem.Init();

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->clOrder.clitem.siUnique);
	if ( ref )
	{
		SI32 itemtype = pclItemManager->pclItemInfo[ref]->siType ;
		if( itemtype < 0 || itemtype >= MAX_ITEMTYPE_NUMBER )		return;

		// 오브젝트 물건이냐?
		if ( pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_OBJECT) )
		{
			if ( pclItemManager->MakeTradeServerObjectForRank(&clitem,pclinfo->clOrder.clitem.siUnique,
				pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siRank,
				pclinfo->clOrder.clitem.siRareType,pclinfo->clOrder.clitem.siType,
				pclinfo->clOrder.clitem.siPower) == FALSE )
				return ;
		}
		else
		{
			clitem.siUnique = pclinfo->clOrder.clitem.siUnique ;
			clitem.siItemNum = pclinfo->clOrder.clitem.siItemNum ;
		}
	}
	else
	{
		return ;
	}

	//--------------------------------------------------------------------------------------
	// 세금 검사
	//--------------------------------------------------------------------------------------
	bool bIsFeeCheck = false;

	// 세금을 부과한다.
	CMinisterIzo *pclminister = (CMinisterIzo *)pclMinisterMgr->GetMinister(MINISTER_IZO);
	if ( (pclminister) && (pclminister->GetIsWorldTradeTax()) )
	{
		bIsFeeCheck = true;
	}

	// 다른 지역 특산품일 경우 세금을 부과하지 않는다
	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if ( pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) )
		{
			if(pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) == false )
			{
				bIsFeeCheck = false;
			}
		}
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		if ( pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_KOREA) ||
			pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_GLOBAL) )
		{
			if(pclClient->pclItemManager->IsItemInfoAtb( clitem.siUnique, ITEMINFOATB_SPECIAL_JAPAN) == false )
			{		
				bIsFeeCheck = false;
			}
		}
	}

	if ( true == bIsFeeCheck )
	{
		SI64 siPrice				= pclinfo->clOrder.clitem.siPrice;
		SI16 siItemNum				= pclinfo->clOrder.clitem.siItemNum;
		SI64 siAverageWorldMoney	= pclDistributionWorldMoney->siAverageWorldMoneyPrice;

		// 주문에 올려진 아이템의 한개당 가격
		GMONEY siOrderPrice = 0;
		if ( 0 < siItemNum )
		{
			siOrderPrice = ( siPrice * siAverageWorldMoney ) / siItemNum ;
		}

		// 아이템의 시세
		cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(pclinfo->clOrder.clitem.siUnique);
		if ( pclprice == NULL )
		{
			return ;
		}

		SI64 itemprice	= pclprice->siPrice ;

		SI32 siRatio	= 0;

		if ( 0 != itemprice )
		{
			siRatio = (SI32)(siOrderPrice * 100 / itemprice);
		}
		else

		{
			return;
		}

		// 실제 적용될 세율을 구한다.
		SI32 siTotalSpecialTaxRate = 0;

		if ( (pclminister) && (siRatio < pclminister->GetSpecialTaxRange()) )
		{
			siTotalSpecialTaxRate = pclminister->GetSpecialTaxRate() + ( pclminister->GetSpecialTaxRange() - siRatio );
		}
		else
		{
			siTotalSpecialTaxRate = 1;
		}

		GMONEY siTax = ( siPrice * siAverageWorldMoney ) * siTotalSpecialTaxRate / 100;
		if ( (0 < siTax) && (pclchar->pclCI->clIP.GetMoney() < siTax ) )
		{
			return;
		}

	}


	cltItem cltoitem;
	cltoitem.Init();
	SI16 pos = 0 ;
	SI32 returnvalue = 0 ;

	if ( pclchar->CanAddInv(INVMODE_ALL,&clitem,&pos,&cltoitem,&returnvalue) == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_BUY_FULLINV,0,0,pclchar->GetCharUnique());
		return ;
	}

	SI16 serverindex = siServerUnique;

	// 들어갈 자리를 예약한다.
	pclchar->SetInputItemReserve(pos,CurrentClock);

	NTCHARString<4>	kServerNation;
	kServerNation = GetTradeServer_ServerNation( pclClient->siServiceArea );

	NTradeServerRequest_Set_BuyOrder clMsg( pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),
		(char*)pclchar->GetName(),pclchar->pclCI->clBI.szAccountID,
		kServerNation ,serverindex,0,
		&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex],pos);
	pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);

/*
#ifdef _DEBUG
	NTradeServerRequest_Set_BuyOrder clMsg( pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),
		(char*)pclchar->GetName(),pclchar->pclCI->clBI.szAccountID,
		"tst",serverindex,0,
		&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex],pos);
	pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);
#else
	if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		NTradeServerRequest_Set_BuyOrder clMsg( pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),
			(char*)pclchar->GetName(),pclchar->pclCI->clBI.szAccountID,
			"jpn",serverindex,0,
			//&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex]);
			&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex],pos);

		pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);
	}
	else if (pclClient->siServiceArea == ConstServiceArea_English)
	{
		NTradeServerRequest_Set_BuyOrder clMsg( pclchar->GetCharUnique(),pclchar->pclCI->GetPersonID(),
			(char*)pclchar->GetName(),pclchar->pclCI->clBI.szAccountID,
			"eng",serverindex,0,
			//&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex]);
			&pclWorldTraderManager->clOrder[pclinfo->clOrder.siIndex],pos);

		pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);
	}
#endif*/

	// 현재 무역 서버로 패킷 보냈다고 알려준다.
	pclchar->bTradeServerUsing = true ;
}

// 리스트 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_LISTINFO(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	if ( bTradeServer == false )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_GM,0,0,pclchar->GetCharUnique());
		return ;
	}

	// 시간제한
	if ( sTime.wHour < siTradeServerStartTime || sTime.wHour > siTradeServerEndTime )
	{
		SendServerResponseMsg(0,SRVAL_FAIL_TRADESERVER_LIMITTIME,siTradeServerStartTime,siTradeServerEndTime,pclchar->GetCharUnique());
		return ;
	}

	cltGameMsgRequest_TradeServer_Get_ListInfo* pclinfo = (cltGameMsgRequest_TradeServer_Get_ListInfo*)pclMsg->cData;

	SI32 ref = pclItemManager->FindItemRefFromUnique(pclinfo->siIndex);
	if ( ref <= 0 || ref >= MAX_ITEMINFO_NUMBER )
		return ;
	if ( pclItemManager->pclItemInfo[ref]->siTradeServerMinNum <= 0 )
		return ;

	SI32 itemunique = pclinfo->siIndex ;

	SI32 index = 0 , listnum = 0 ;
	cltTradeServerOrder clorder[MAX_TRADESERVER_GET_LIST];
	for ( SI32 j = 0 ; j < MAX_TRADESERVER_GET_LIST ; j ++ )
		clorder[j].Init();

	SI64 averageworldmoneyprice = pclDistributionWorldMoney->siAverageWorldMoneyPrice ;
	cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(itemunique);
	if ( pclprice == NULL )
		return ;
	SI64 itemprice = pclprice->siPrice ;

	bool bIsItem = false ;

	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
	{
/*		bool bDiffNation = false;

		if (siServiceArea == ConstServiceArea_English)
		{
			if (_tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("eng")) != 0 && 
				pclWorldTraderManager->clOrder[i].szServerNation[0] != NULL	)
			{
				bDiffNation  = true;
			}
		}
		else if (siServiceArea == ConstServiceArea_Japan)
		{
			if (_tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("jpn")) != 0 &&
				pclWorldTraderManager->clOrder[i].szServerNation[0] != NULL	)
			{
				bDiffNation  = true;
			}
		}
		/*
		else if (siServiceArea == ConstServiceArea_NHNChina)
		{
			if (_tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("NHNChi")) != 0 && 
				pclWorldTraderManager->clOrder[i].szServerNation[0] != NULL	)
			{
				bDiffNation  = true;
			}
		}
		*/

		// 같은 아이템?
		bool bCanAdd = true;

		if( pclWorldTraderManager->clOrder[i].clitem.siUnique != itemunique )				bCanAdd = false;

#ifndef _DEBUG
		// 디버그 버젼이 아니면, 내부 테스트 서버는 보이지 않는다.
		if( _tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("tst")) == 0 )	bCanAdd = false;
		if( _tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("tgl")) == 0 )	bCanAdd = false;
#endif

		// 군주 해외서비스는 한국 군주 넷마블 서비스에서 올린 아이템은 보이지 않는다.
		if( _tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("net")) == 0 )	bCanAdd = false;

		// 같은 국가 같은 서버의 내용은 보이지 않는다.
		NTCHARString<4> kNation;
		kNation = GetTradeServer_ServerNation( siServiceArea );

		if( _tcslen(pclWorldTraderManager->clOrder[i].szServerNation) <= 0 )				bCanAdd = false;
		if( _tcscmp(kNation, pclWorldTraderManager->clOrder[i].szServerNation) == 0 &&
			siServerUnique == pclWorldTraderManager->clOrder[i].siServerIndex)
		{
			bCanAdd = false;
		}

		if( bCanAdd )
		{
			if( index >= 0 && index < MAX_TRADESERVER_GET_LIST )
			{
				clorder[index].Set(&pclWorldTraderManager->clOrder[i]);
				index ++ ;
			}
			bIsItem = true ;
		}
/*		if ( (siServerUnique != pclWorldTraderManager->clOrder[i].siServerIndex && 
			pclWorldTraderManager->clOrder[i].clitem.siUnique == itemunique 
			&& bDiffNation == false) 
			|| ( bDiffNation &&  pclWorldTraderManager->clOrder[i].clitem.siUnique == itemunique    )
			)
		{

			//========================================================
			// 글로벌버전일때는 넷마블서버 물품은 안보이도록
			//========================================================
			if ((siServiceArea == ConstServiceArea_English &&_tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("net")) != 0	) ||
				// 일본버전일때는 넷마블서버 물품은 안보이도록
				(siServiceArea == ConstServiceArea_Japan &&_tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("net")) != 0	) ||
				// NHNCHINA
				(siServiceArea == ConstServiceArea_NHNChina && _tcscmp(pclWorldTraderManager->clOrder[i].szServerNation, TEXT("net")) != 0	))
			{
				if( index >= 0 && index < MAX_TRADESERVER_GET_LIST )
				{
					clorder[index].Set(&pclWorldTraderManager->clOrder[i]);
					index ++ ;
				}
				bIsItem = true ;
			}
			//========================================================
		}*/

		CMinisterIzo* pclminister = (CMinisterIzo*)pclMinisterMgr->GetMinister(MINISTER_IZO);

		bool isworldtradetax = pclminister->GetIsWorldTradeTax();
		SI32 spcialtaxrange = pclminister->GetSpecialTaxRange();
		SI32 spcialtaxrate = pclminister->GetSpecialTaxRate();

		if ( i == MAX_TRADESERVER_ORDER_LIST - 1 )
		{
			cltGameMsgResponse_TradeServer_Get_ListInfo clinfo(clorder, listnum, true, itemprice,
				averageworldmoneyprice, bIsItem, isworldtradetax, spcialtaxrange, spcialtaxrate);
			cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_GET_LISTINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else if ( index == MAX_TRADESERVER_GET_LIST )
		{
			cltGameMsgResponse_TradeServer_Get_ListInfo clinfo(clorder, listnum, false, 0, 0, bIsItem,
                isworldtradetax, spcialtaxrange, spcialtaxrate);
			cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_GET_LISTINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			index = 0 ;
			listnum ++ ;
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_MYINFO(cltMsg*pclMsg,SI32 id)
{
	if ( pclCM->IsValidID(id) == false ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
	{
		// 서버 유니크가 일치하는지 확인한다.
		if( pclWorldTraderManager->clOrder[i].siServerIndex != pclClient->siServerUnique ) 
			continue;

#ifdef _DEBUG
		// 디버그일때는 국가는 체크하지 않는다
		if( _tcscmp( pclWorldTraderManager->clOrder[i].szServerNation , TEXT("tst") ) != 0)
		{
			continue;
		}
#else
		if(	pclClient->siServiceArea == ConstServiceArea_Japan )
		{
			if( _tcscmp( pclWorldTraderManager->clOrder[i].szServerNation , TEXT("jpn") ) != 0)
			{
				continue;
			}
		}
		else if( pclClient->siServiceArea == ConstServiceArea_English )
		{
			if( _tcscmp( pclWorldTraderManager->clOrder[i].szServerNation , TEXT("eng") ) != 0)
			{
				continue;
			}
		}
		/*(이건 뭥미 ㅡㅡ;)
		else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
		{
			if( _tcscmp( pclWorldTraderManager->clOrder[i].szServerNation , TEXT("NHNChi") ) != 0)
				continue;
		}
		*/
		else
		{
			// 일본 , 글로벌만 무역을 지원한다.
			continue;
		}
#endif

		if ( pclWorldTraderManager->clOrder[i].siPersonID == pclchar->pclCI->GetPersonID() )
		{
			cltGameMsgResponse_TradeServer_Get_MyInfo clinfo(&pclWorldTraderManager->clOrder[i]);
			cltMsg clMsg(GAMEMSG_RESPONSE_TRADESERVER_GET_MYINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			break;
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU(cltMsg*pclMsg,SI32 id)
{
	cltGameMsgRequest_TradeServer_Get_Withdraw_Acu* pclInfo = (cltGameMsgRequest_TradeServer_Get_Withdraw_Acu*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	// 받으려는 아큐가 없다면
	if ( 0 >= pclInfo->m_siWithdrawAcu )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( true == pclchar->CheckLastMsgClock(1000) )
	{
		return;
	}

	// 이미 무역 이용중이라면 안된다
	if ( true == pclchar->bTradeServerUsing )
	{
		return;
	}
	
	// 트레이드 서버랑 연결되어 있냐?
	if ( (NULL == pclNTradeServerProcess) || (FALSE == pclNTradeServerProcess->IsEstablished()) )
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_TRADESERVER_SESSION, 0, 0, pclchar->GetCharUnique() );
		return;
	}

	char szNationCode[4] = { '\0', };
	if ( false == GetNationCode(szNationCode, sizeof(szNationCode)) )
	{
		return;
	}
	SI32 siPersonID = pclchar->pclCI->GetPersonID();

	NTradeServerRequest_Withdraw_Acu clMsg( id, siPersonID, szNationCode, siServerUnique, pclInfo->m_siWithdrawAcu );
	pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);

	// 무역서버 사용중이라고 해놓는다
	pclchar->bTradeServerUsing = true;
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_TRADESERVER_GET_SAVEDACU_LIST(cltMsg*pclMsg,SI32 id)
{
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( true == pclchar->CheckLastMsgClock(1000) )
	{
		return;
	}

	// 트레이드 서버랑 연결되어 있냐?
	if ( (NULL == pclNTradeServerProcess) || (FALSE == pclNTradeServerProcess->IsEstablished()) )
	{
		SendServerResponseMsg( 0, SRVAL_FAIL_TRADESERVER_SESSION, 0, 0, pclchar->GetCharUnique() );
		return;
	}

	char szNationCode[4] = { '\0', };
	if ( false == GetNationCode(szNationCode, sizeof(szNationCode)) )
	{
		return;
	}
		
	NTradeServerRequest_Get_SavedACU_List clMsg( pclchar->GetCharID(), pclchar->pclCI->GetPersonID(), szNationCode, siServerUnique );
	pclNTradeServerProcess->SendMsg((sPacketHeader*)&clMsg);

}

// WorldPassport( 무역 여권 )
void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDPASSPORT_SET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_WorldPassport_Set* pclInfo = (cltGameMsgRequest_WorldPassport_Set*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	// 이미 여권이 있는데 발급신청을 한다면
	if ( true == pclchar->pclCI->clWorldPassport.IsHavePassport() )
	{
		return;
	}

	// 발급 비용 아큐 체크
	if ( Const_Passport_Issue_Cost_Acu > pclchar->pclCI->clWorldMoney.GetWorldMoney() )
	{
		return;
	}

	// 유효한 Passport Name인지 체크
	if ( false == pclchar->pclCI->clWorldPassport.IsCorrectPassportName(pclInfo->m_szPassportName) )
	{
		return;
	}

	// 인수로 쓰일 변수들 
	SI32 siPersonID	= pclchar->pclCI->GetPersonID();
	SI32 siCharID	= pclchar->GetCharID();

	// 발급일은 현재 시간
	cltDate clIssueDate;
	clIssueDate.Set( &pclTime->clDate );

	// 만료일은 현재 시간에 5년을 더한다
	cltDate clExpireDate;
	clExpireDate.Set( &clIssueDate );
	clExpireDate.uiYear += 5;


	sDBRequest_WorldPassport_Set clPassport_Set( 
		siPersonID,		
		siCharID, 
		pclInfo->m_szPassportName, 
		clIssueDate.GetDateVary(),
		clExpireDate.GetDateVary(),
		Const_Passport_Issue_Cost_Acu
	);
	pclGameDBProcess->SendMsg( (sPacketHeader*)&clPassport_Set );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_WORLDPASSPORT_EXTEND(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	// 여권도 없는데 연장 신청을 했다면
	if ( false == pclchar->pclCI->clWorldPassport.IsHavePassport() )
	{
		return;
	}

	// 연장 비용 아큐 체크
	if ( Const_Passport_Extend_Cost_Acu > pclchar->pclCI->clWorldMoney.GetWorldMoney() )
	{
		return;
	}

	// 인수로 쓰일 변수들 
	SI32 siPersonID	= pclchar->pclCI->GetPersonID();
	SI32 siCharID	= pclchar->GetCharID();

	// 만료일 설정
	cltDate clExpireDate;
	clExpireDate.MakeFullDateFromDateVary( pclchar->pclCI->clWorldPassport.GetExpireDateVary() );

	// 원래 만료시간이 현재 시간보다 이전시간이면 기본설정일을 현재시간으로 한다
	if ( pclTime->clDate.GetDateVary() > clExpireDate.GetDateVary() )
	{
		clExpireDate.Set( &pclTime->clDate );
	}
	// 연장 기간은 5년이다
	clExpireDate.uiYear += 5;


	sDBRequest_WorldPassport_Extend clPassport_Extend( siPersonID, siCharID, clExpireDate.GetDateVary(), Const_Passport_Extend_Cost_Acu );
	pclGameDBProcess->SendMsg( (sPacketHeader*)&clPassport_Extend );

}

