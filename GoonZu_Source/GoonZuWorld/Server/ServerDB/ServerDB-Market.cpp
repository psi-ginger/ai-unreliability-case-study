//---------------------------------
// 2003/8/9 김태곤
//---------------------------------

#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\..\Common\Map\Map.h"

#include "..\../../Server/AuthServer/AuthMsg.h"

#include "..\Minister\Minister-Goonzu.h"
#include "..\Minister\MinisterMgr.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

#include "MsgRval-Define.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../CommonLogic/Bank/Bank-Manager.h"

signed short UserMoneyCapsule(__int64 Money)
{
	signed short tMoney = (signed short)((Money % 7796) ^ 7796);

	return tMoney;
}

// 주문 가격 인하 요청 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_REDUCEORDERPRICE(sPacketHeader* pPacket)
{
	sDBResponse_ReduceOrderPrice* pclMsg = (sDBResponse_ReduceOrderPrice*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if(pclMarketManager->pclOrderManager->SetOrder(ORDERMODE_SELL, &pclMsg->clOrder) == true)
	{
		pclMarketManager->SetItemTypeOrder(&pclMsg->clOrder, 200);
	}

	// 만약 주문자가 있다면 갱신된 정보를 통보한다. 
	
	// 쪽지를 보낸다. 
	cltItem clitem;
	clitem.Set(&pclMsg->clOrder.clItemData);

	cltLetterMsg_MarketPriceDown clletter(clitem.siUnique, clitem.siItemNum, pclMsg->clOrder.siPrice);
	SI32 id = SendLetterMsg(pclMsg->clOrder.siPersonID, (cltLetterHeader*)&clletter);
	if(pclCM->IsValidID(id))
	{
		// 캐릭터의 객주 정보를 업데이트 한다. 
		pclCM->CR[id]->pclCI->clMarket.clInfo.UpdateMarketOrder(pclMsg->siMarketIndex, &pclMsg->clOrder);

		pclCM->CR[id]->SetUpdateSwitch(UPDATE_MARKET, true, 0);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 4] // 변경된 주문 정보(가격인하). 
			// param1 == 마켓 번호, 
			// param2 == 물품 유니크, 
			// param3 == 인하되기 전 금액, 
			// param4 == 인하된 금액, 
			// param5 == 주문번호
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_REDUCEORDERPRICE, 
											0, NULL, NULL, pclMsg->clOrder.siPersonID, 0, 0, &clitem, clitem.siItemNum, 0, 0, 0, 0, 0, 
											pclMsg->siMarketIndex, clitem.siUnique, pclMsg->siPrice, pclMsg->clOrder.siPrice, pclMsg->clOrder.siIndex, 
											NULL, NULL);
		}
	}
}

// 객주 계좌 정보 요청에  대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MARKETACCOUNT(sPacketHeader* pPacket)
{
	sDBResponse_MarketAccount* pclMsg = (sDBResponse_MarketAccount*)pPacket;

	if(pclMsg->siResult == 0)
	{

		return ;
	}

	if(pclMsg->siPersonID <= 0)
	{

		return ;
	}
	
	// 클라이언트가 무효한 경우라면, 
	// 적절한 캐릭터를 찾아야 한다. 
	SI32 id = pclMsg->usCharID;
	if(id == 0 || pclCM->IsValidID(id)== false )
	{
		//PersonID를 근거로 ID를 찾는다. 
		// 팔자 주문을 올려 놓은 사람의 물건이 팔렸을 때 팔자가 게임중에 있다면 그 팔린 정보를 보내기 위해서다. 
		id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
		if(id == 0 || pclCM->IsValidID(id)== false)
		{
			return ;
		}
	}


	if(pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)	
	{
		pclCM->CR[id]->pclCI->clMarket.clInfo.Set(&pclMsg->clMarketAccount);
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_MARKET, true, 0);
	}
}

// 객주 주문을 DB로 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETMANYMARKETORDER(sPacketHeader* pPacket)
{
	SI32 i;

	sDBResponse_GetManyMarketOrder* pclMsg = (sDBResponse_GetManyMarketOrder*)pPacket;

	// id가 0인경우도 가능하다. 

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 서버의 주문 버퍼에 DB에서 보내온 주문 리스트를 넣는다. 
	for(i = 0;i < GET_MANY_MARKET_ORDER_NUM;i++)
	{
		// 주문의 가격이 0 이면 무효한 주문이므로(주문이 없는 것임) 설정하지 않는다. 
		if(pclMsg->clTradeOrder[i].siPrice)
		{
			if(pclMarketManager->pclOrderManager->SetOrder(pclMsg->siOrderMode, &pclMsg->clTradeOrder[i]) == true)
			{
				pclMarketManager->SetItemTypeOrder(&pclMsg->clTradeOrder[i], 300);
			}
		}
		
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetManyMarketOrder"));

}

// 주문 정보를 DB에 추가하라는 요청에 대한 응답을 처리한다.
void cltServer::DoMsg_DBMSG_RESPONSE_ORDERMARKET(sPacketHeader* pPacket)
{
	sDBResponse_OrderMarket* pclMsg = (sDBResponse_OrderMarket*)pPacket;

	if(pclMsg->clOrder.siType != TRADEORDER_TYPE_OBJECT)return ;

	SI32 id = pclMsg->usCharID;

	SI32 returnval = 0;

	switch(pclMsg->siResult )
	{
	case 0:		
		break;
	case 1:		
		{
			// 서버의 주문 버퍼에 DB로부터 얻은 주문 정보를 추가한다. 
			if(pclMarketManager->pclOrderManager->SetOrder(pclMsg->siOrderMode, &pclMsg->clOrder) == true)
			{
				pclMarketManager->SetItemTypeOrder(&pclMsg->clOrder, 400);
			}

			// 클라이언트가 유효한 경우라면, 
			// 클라이언트로 이 주문 정보를 보낸다. 
			if(pclCM->IsValidID(id)== false) return ;
		
			if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->clOrder.siPersonID)	
			{
				
				//---------------------------------
				// 인벤토리 정보를 업데이트 한다.
				//---------------------------------
				if( pclMsg->siItemFromPos >= 0 && pclMsg->siItemFromPos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemFromPos].Set(&pclMsg->clFromItem);

					// 클라이언트로 업데이트 된 정보를 보낸다. 
					cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemFromPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 10] // 물품 등록 쓰기. 
					// param1	 == 주문번호. 
					// param2	 == 주문 개당 가격. 
					// param3    == 세금.
					// pszParam1 == 판매 주문한 아이템의 인벤 위치, 파는 아이템 유니크, 주문하고 남은 아이템의 개수.
					TCHAR str[50];
					StringCchPrintf(str, sizeof(str), "Pos: %d, Unique: %d, Num: %d",
														pclMsg->siItemFromPos, 
														pclMsg->clOrderItem.siUnique, 
														pclMsg->clOrderItem.siItemNum);
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_ORDERMARKET, 
										 0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clOrderItem, pclMsg->clFromItem.GetItemNum(), 0, 0, pclMsg->clOrder.siVillageUnique, 0, 0, 
										 pclMsg->clOrder.siIndex, pclMsg->clOrder.siPrice, pclMsg->siTax, 0, 0, str, NULL);
				}

				//-------------------------------
				// 세금을 납부한다. 
				//-------------------------------
				if( false == pclCM->CR[id]->pclCI->clBI.bMarKetPremiumService )
				{
					if(pclMsg->siTax > 0)
					{
						if(pclCM->CR[id]->pclCI->clIP.GetMoney() >= pclMsg->siTax)
						{
							sDBRequest_ChangeMoney clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_USEMARKET, -pclMsg->siTax);
							pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);


							// 재정을 적립한다. 
							//clCurrencyInfo.clReservedBudget.itMoney += pclMsg->siTax;

							CMinisterGoonzu *pclminister = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
							pclminister->AddTempFinances( pclMsg->siTax );

							// 통화량 조절 
							cltMoney clmoney;
							clmoney.Set(pclMsg->siTax);
							clCurrencyInfo.clOutputMoney_clMarketTax.Increase(&clmoney);

                            pclDelMoneyStatistics->Add(DELMONEY_MARKETSERVICECHARGE, pclMsg->siTax);
						}
					}
				}
			}

			returnval	= SRVAL_MARKET_SUCCESS_ORDERSELL; 

			pclCM->CR[id]->DoQuestCheckAndAction( QUEST_CLEAR_COND_ORDERTOMARKET );
		}
		break;

	default:
		MsgBox(TEXT("DoMsg_DBMSG_RESPONSE_ORDERMARKET()"), TEXT("result:%d"), pclMsg->siResult);
		break;
	}
		
		
	if(pclCM->IsValidID(id) == false)return ;

	// 클라이언트로 서버의 응답값을 보낸다. 
	if(returnval)
	{
		SendServerResponseMsg(RANKTYPE_MARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 객주 거래 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_TRADEMARKET(sPacketHeader* pPacket)
{
	sDBResponse_TradeMarket* pclMsg = (sDBResponse_TradeMarket* )pPacket;

	SI32 villageunique = pclMsg->siVillageUnique;

	// 클라이언트가 유효한 경우라면, 
	// 클라이언트로 이 주문 정보를 보낸다. 
	SI32 id = pclMsg->usCharID;
	
	SI32 returnval	= 0;
	SI32 rtnpara1	= 0;
		
	switch(pclMsg->siResult )
	{
	case -1:	returnval	= SRVAL_MARKET_SELL_FAIL;	rtnpara1 = 900; break;  // 인벤토리에 빈자리가 없어서 아이템을 옮길 수 없다. 
	case -2:	returnval	= SRVAL_MARKET_SELL_FAIL;	rtnpara1 = 1000; break;	// 물건 구매 실패 - 상대 주문이 허수 주문임(실제로는 물건이 없음.) 
	case -3:	returnval	= SRVAL_MARKET_SELL_FAIL;	rtnpara1 = 700; break;	// 물건 구매 실패 - 현금 부족. 
	case -4:	returnval	= SRVAL_MARKET_SELL_FAIL;	rtnpara1 = 1200; break;	// 물건 구매 실패 - 판매자가 통장이 없다. 
	case -5:	returnval	= SRVAL_MARKET_SELL_FAIL;	rtnpara1 = 800; break;	// 물건 구매 실패 - 물건 최대 개수를 초과. 
	case -6:																	// 물건 구매 실패 - 팔자의 전장이 꽉차서 돈을 입금시킬 수 없다. 
		{
			returnval	= SRVAL_MARKET_SELL_FAIL;
			rtnpara1	= 1400;

			SI32 index	= pclMsg->clDstOrder.siIndex;
			if(index < 0 || index >= MAX_MARKET_ORDER_NUMBER)
			{
				pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("fd3fdee index:%d"), index);
				return ;
			}
			pclMarketManager->pclOrderManager->pclOrderSell[index]->bBankError = true ;
		}
		break;
	case 0:		break;
	case 1:		
		{
			// 팔자주문을 업데이트 한다. 
			//기존 주문에서 찾는다. 
			SI32 index				= pclMsg->clDstOrder.siIndex;
			if(index < 0 || index >= MAX_MARKET_ORDER_NUMBER)
			{
				pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("fd3fdff index:%d"), index);
				return ;
			}
			SI32 orderitemunique	= pclMarketManager->pclOrderManager->pclOrderSell[index]->clItemData.siUnique;

			if(pclMarketManager->pclOrderManager->SetOrder(ORDERMODE_SELL, &pclMsg->clDstOrder) == true)
			{
				cltTradeOrder clorder;
				clorder.Set(&pclMsg->clDstOrder);
				clorder.clItemData.siUnique	= orderitemunique;
				
				pclMarketManager->SetItemTypeOrder(&clorder, 500);
			}
			
			
			//---------------------------------
			// 거래된 가격을 설정한다.
			//---------------------------------
			if(pclMsg->clItem.siUnique && pclMsg->siTradePrice)
			{
				pclItemManager->SetCurrentPrice(pclMsg->clItem.siUnique, pclMsg->siTradePrice, pclMsg->siTradeItemNum);

				cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(pclMsg->clItem.siUnique);
				if(pclprice)
				{
					const TCHAR* pszItemName = pclClient->pclItemManager->GetName(pclprice->siUnique);
					sDBRequest_SetItemPrice clMsg(pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, pszItemName);
					pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
			}
			
			if(pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)	
			{
				//--------------------------------
				// 현금 보유 정보를 업데이트 한다. 
				//--------------------------------
				pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siMoney);
				pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);


				//---------------------------------
				// 인벤토리 정보를 업데이트 한다.
				//---------------------------------
				if(pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON)
				{
					pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
					
					// 클라이언트로 업데이트 된 정보를 보낸다. 
					cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					
					((cltCharServer*)pclCM->CR[id])->IncQuestPara( QUEST_CLEAR_COND_BUYMARKET, pclMsg->clItem.siUnique, pclMsg->siTradeItemNum );

				}
			}

			//-----------------------------------------
			// 팔자의 정보(전장 잔액, 주문 정보 )를 업데이트 한다. 
			//-----------------------------------------
			SI32 sellerid = pclCM->GetIDFromPersonID(pclMsg->siSellerPersonID);
			// 실제로 거래가 이루어진 내역을 확보한다. 
			SI32	itemunique = pclMsg->clItem.siUnique;
			SI32	itemnum	   = pclMsg->siTradeItemNum;
			GMONEY	price	   = pclMsg->siTradePrice;

			if(itemunique)
			{
				//---------------------------------------------
				// 물건 판 사람의 계좌에서 수수료를 지불한다. 
				//--------------------------------------------
				// 수수료를 확인한다. 
				cltStrInfo* pclstrinfo = pclMarketManager->GetStrInfo(villageunique);
				if ( NULL != pclstrinfo )
				{
					GMONEY fee = pclstrinfo->clMarketStrInfo.clFee.CalcFee(itemnum * price);

					if(fee)
					{
						cltMoney clmoney;
						clmoney.itMoney = fee;
						sDBRequest_PayFee clinfo(sellerid, pclMsg->siSellerPersonID, villageunique, RANKTYPE_MARKET, FEETYPE_MARKET_SELL, &clmoney);
						pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);

						pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
					}


					// 쪽지를 보낸다.
					cltLetterMsg_MarketTrade clletter1(itemunique, itemnum, price*itemnum, fee);
					SendLetterMsg(pclMsg->siSellerPersonID, (cltLetterHeader*)&clletter1);
				}
			}

			if(pclCM->IsValidID(sellerid))
			{
				//---------------------------------
				// 전장 잔액 
				//---------------------------------
				pclCM->CR[sellerid]->pclCI->clBank.SetMoney(pclMsg->siSellerBankMoney);
				
				pclCM->CR[sellerid]->SetUpdateSwitch(UPDATE_BANK, true, 0);
			}


			returnval = SRVAL_MARKET_SUCCESS_TRADE;	
		}	
		break;

	default:
		MsgBox(TEXT("DoMsg_DBMSG_RESPONSE_TRADEMARKET()"), TEXT("result:%d"), pclMsg->siResult);
		break;
	}


	// 클라이언트로 서버의 응답값을 보낸다. 
	if(returnval)
	{
		if(pclCM->IsValidID(id))
		{
			if(returnval == SRVAL_MARKET_SUCCESS_TRADE)
			{
				SendServerResponseMsg(RANKTYPE_MARKET, returnval,  rtnpara1, 0, pclCM->CR[id]->GetCharUnique());
			}
			else
			{
				SendServerResponseMsg(0, returnval,  rtnpara1, 0, pclCM->CR[id]->GetCharUnique());
			}
			
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				SI32 sellerid = pclCM->GetIDFromPersonID(pclMsg->siSellerPersonID);

				TCHAR pszCharParam1[50] = {'\0', };
				
				cltItemData*  ItemData = &pclMsg->clDstOrder.clItemData;

				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "Unique: %d, Num: %d",
																		ItemData->siUnique, 
																		ItemData->siItemNum);

				// 전체 거래금액.
				SI32 siTotalTradeMoney = pclMsg->siTradePrice * pclMsg->clTradeItem.siItemNum;

				// [로그추가 : 황진성 2007. 10. 16] // 물품 판매 쓰기. 
				// param1 == 주문번호,
				// param2 == 아이템 하나의 가격.
				// pszCharParam1 == 주문 정보의 아이템 정보.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_SELL, 
									 pclMsg->siReason, (cltCharServer*)(pclCM->CR[sellerid]), NULL, pclMsg->siPersonID, 
									 siTotalTradeMoney, pclMsg->siSellerBankMoney, &pclMsg->clTradeItem, pclMsg->siTradeItemNum, 
									 0, 0, pclMsg->siVillageUnique, 0, 0, 
									 pclMsg->clDstOrder.siIndex, pclMsg->siTradePrice, 0, 0, 0, 
									 pszCharParam1, NULL);

				// [로그추가 : 황진성 2007. 10. 10] // 물품 구매 쓰기. 
				// param1 == 주문번호.
				// param2 == 아이템 하나의 가격.
				// pszCharParam1 == 유저 인벤의 아이템 정보.
				ZeroMemory(pszCharParam1, sizeof(pszCharParam1));
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "Pos: %d, Unique: %d, Num: %d",
																		pclMsg->siItemPos, 
																		pclMsg->clItem.siUnique, 
																		pclMsg->clItem.siItemNum);


				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_BUY, 
									pclMsg->siReason, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siSellerPersonID, 
									siTotalTradeMoney, pclMsg->siMoney, &pclMsg->clTradeItem,  pclMsg->siTradeItemNum, 
									0, 0, pclMsg->siVillageUnique, 0, 0, 
									pclMsg->clDstOrder.siIndex, pclMsg->siTradePrice, siTotalTradeMoney, 0, 0,
									pszCharParam1, NULL);
		
			}
		}
		
		// 시스템이 구매하면.
		if(0 == id)
		{
			TCHAR pszCharParam1[50] = {'\0', };

			cltItemData*  ItemData = &pclMsg->clDstOrder.clItemData;

			StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "Unique: %d, Num: %d",
																	ItemData->siUnique, 
																	ItemData->siItemNum);

			// 전체 거래금액.
			SI32 siTotalTradeMoney = pclMsg->siTradePrice * pclMsg->clTradeItem.siItemNum;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 시스템에서 물품 구매 쓰기. 
				// param1		 == 주문번호.
				// param2		 == 아이템 하나의 가격.
				// param3		 == 실제로 거래된 가격. (거래가 이루어지지 않았다면 0) 
				// pszCharParam1 == 유저 인벤의 아이템 정보.
				ZeroMemory(pszCharParam1, sizeof(pszCharParam1));
				StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "Pos: %d, Unique: %d, Num: %d",
																		pclMsg->siItemPos, 
																		pclMsg->clItem.siUnique, 
																		pclMsg->clItem.siItemNum);


				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SYSTEM, LOGCOMMAND_INDEX_SYSTEM_USER_ITEM_BUY, 
									pclMsg->siReason, NULL, NULL, pclMsg->siSellerPersonID, 0, 0, &pclMsg->clTradeItem,  pclMsg->siTradeItemNum, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									pclMsg->clDstOrder.siIndex, pclMsg->siTradePrice, siTotalTradeMoney, 0, 0,
									pszCharParam1, NULL);
			}

			// 시스템이 유저가 시장에 올린 아이템을 구입한 정보를 파일로 만들기위한 정보.
			clSystemBuyItem.m_sTime    = sTime;				// 현재 시간.
			clSystemBuyItem.m_siCnt++;						// 구매 횟수.
			clSystemBuyItem.m_siMoney += siTotalTradeMoney;	// 액수.
		}
	}
}


// 객주 창고에서 아이템을 꺼내오라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETITEMFROMMARKET(sPacketHeader* pPacket)
{
	sDBResponse_GetItemFromMarket* pclMsg = (sDBResponse_GetItemFromMarket*)pPacket;

	SI32 id = pclMsg->usCharID;

	SI32 returnval = 0;

	switch(pclMsg->siResult )
	{
	case 0:
		break;
	case 1:		
		{
			if(pclCM->IsValidID(id)== false) return;
			
			// 변경된 창고의 내용에 따라 주문도 변경한다.
			SI32 index				= pclMsg->clOrder.siIndex;
			if( index < 0 || index >= MAX_MARKET_ORDER_NUMBER)		return;
			SI32 orderitemunique	= pclMarketManager->pclOrderManager->pclOrderSell[index]->clItemData.siUnique;

			if(pclMarketManager->pclOrderManager->SetOrder(ORDERMODE_SELL, &pclMsg->clOrder) == true)
			{
				cltTradeOrder clorder;
				clorder.Set(&pclMsg->clOrder);
				clorder.clItemData.siUnique = orderitemunique;
				pclMarketManager->SetItemTypeOrder(&clorder, 600);
			}

			if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)	
			{
				if( pclMsg->siFromPos >= 0	&& pclMsg->siFromPos < MAX_MARKET_ORDER_PER_PERSON &&
					pclMsg->siToPos >= 0	&& pclMsg->siToPos < MAX_ITEM_PER_PERSON )
				{
					// 클라이언트에게 객주 창고의 최신 정보를 보내준다.
					pclCM->CR[id]->pclCI->clMarket.clInfo.clItem[pclMsg->siFromPos].Set(&pclMsg->clFromItem);
					pclCM->CR[id]->SetUpdateSwitch(UPDATE_MARKET, true, 0);

					// 클라이언트에게 업데이트된 인벤의 정보를 보내준다. 
					pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToPos].Set(&pclMsg->clToItem);

					cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siToPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}

			returnval	= SRVAL_MARKET_SUCCESS_GETFROMMARKET; 
		}
		break;

	default:
		MsgBox(TEXT("DoMsg_DBMSG_RESPONSE_GETITEMFROMMARKET()"), TEXT("result:%d"), pclMsg->siResult);
		break;
	}
		
	if(pclCM->IsValidID(id)== false) return;

	// 클라이언트로 서버의 응답값을 보낸다. 
	if(returnval)
	{
		SendServerResponseMsg(RANKTYPE_MARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 물품 회수 쓰기. 
			// param1 == 주문번호. 
			// param2 == 출발위치.
			// param3 == 회수 아이템 유니크.
			// param4 == 회수 아이템 개수.
			// pszParam1 == 회수한 아이템의 인벤 위치, 회수 아이템 유니크, 회수하고 인벤의 최종 아이템의 개수.
			TCHAR str[50] = {'\0', };
			StringCchPrintf(str, sizeof(str), "Pos: %d, Unique: %d, Num: %d",
											  pclMsg->siToPos, 
											  pclMsg->clToItem.siUnique, 
										  	  pclMsg->clToItem.siItemNum);

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_GETITEMFROMMARKET, 
								 0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clGetItem, pclMsg->clToItem.GetItemNum(), 0, 0, pclMsg->clOrder.siVillageUnique, 0, 0, 
								 pclMsg->clOrder.siIndex, pclMsg->siFromPos, pclMsg->clGetItem.siUnique, pclMsg->clGetItem.siItemNum, 0, str, NULL);
		}
	}
}

// 아이템 가격 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYITEMPRICE(sPacketHeader* pPacket)
{
	sDBResponse_ManyItemPrice* pclMsg = (sDBResponse_ManyItemPrice*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 i;
	// 아이템 가격을 얻어온다
	for(i = 0;i < MANYITEM_PRICE_NUMBER;i++)
	{
		if(pclMsg->siItemUnique[i])
		{
			SI32 index = pclItemManager->FindItemRefFromUnique(pclMsg->siItemUnique[i]);

			if(index > 0 && index < MAX_ITEMINFO_NUMBER) 
			{
				pclItemManager->pclItemPrice->clUnit[index].siPrice		= (SI64) pclMsg->fPrice[i];
				pclItemManager->pclItemPrice->clUnit[index].fPrice		= pclMsg->fPrice[i];
				pclItemManager->pclItemPrice->clUnit[index].siItemNum	= pclMsg->siItemNum[i];
			}
		}
	}

	// 마지막 메시지
	if( -1 == pclMsg->siResult )
	{
		// 아이템 시세가 없는경우 기본값으로 설정한다.	- By KKM 2007 / 08 / 22
		SI32 siItemCount = pclItemManager->siItemNum;
		if( siItemCount > MAX_ITEMINFO_NUMBER )		siItemCount = MAX_ITEMINFO_NUMBER;
		for( SI32 i=1; i<= siItemCount; ++i)
		{
			// 둘중에 하나라도 비정상적이면 기본값을 넣어준다.
			if(	pclItemManager->pclItemPrice->clUnit[i].siPrice <= 0 || 
				pclItemManager->pclItemPrice->clUnit[i].fPrice <= 0.001f )
			{
				pclItemManager->pclItemPrice->clUnit[i].siPrice		= BASIC_ITEMPRICE_DEFAULT;
				pclItemManager->pclItemPrice->clUnit[i].fPrice		= (float)BASIC_ITEMPRICE_DEFAULT;
			}
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyItemPrice"));
}

// 아이템 가격 저장 
void cltServer::DoMsg_DBMSG_RESPONSE_SETITEMPRICE(sPacketHeader* pPacket)
{

}

// 객주 거래 수수료 정보를 설정하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETMARKETFEE(sPacketHeader* pPacket)
{
	
	sDBResponse_SetMarketFee* pclMsg = (sDBResponse_SetMarketFee*)pPacket;

	// 결과를 확인한다. 
	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case 1:	// 수수료 변경 성공. 
		{

			// 서버의 해당 전장의 정보를 업데이트 한다. 
			cltStrInfo* pclinfo = pclMarketManager->GetStrInfo(pclMsg->siVillageUnique);
			pclinfo->clMarketStrInfo.clFee.Set(&pclMsg->clFee);

			returnval	= SRVAL_MARKET_SUCCESSSETFEE;
		}
		break;
	default:
		MsgBox(TEXT("vc984j"), TEXT("fdd:%d"), pclMsg->siResult);
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_MARKET, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_SYSTEMBUYITEMNOWMONEY(sPacketHeader* pPacket)
{
	sDBResponse_GetSystemBuyItemNowMoney* pclMsg = (sDBResponse_GetSystemBuyItemNowMoney*)pPacket;

	if(pclMsg->siResult == 1) //성공
		siMarketMoneyItemMaxpriceNow = pclMsg->gPrice;
	else
		siMarketMoneyItemMaxpriceNow = 0;
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKETACCOUNT(sPacketHeader* pPacket)
{
	sDBResponse_NewMarketAccount* pclMsg = (sDBResponse_NewMarketAccount*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return ;
	}

	SI32 id  = pclMsg->usCharID;
	if( false == pclCM->IsValidID(id) )
		return;

	if( pclMsg->siPersonID != pclCM->CR[id]->pclCI->GetPersonID() )
		return;

	if( pclMsg->siServerUnique != siServerUnique )
		return;

	cltGameMsgResponse_NewMarketAccount clNewMarketAccount( &pclMsg->clMarketAccount );
	cltMsg clMsg(GAMEMSG_RESPONSE_NEWMARKETACCOUNT, sizeof(cltGameMsgResponse_NewMarketAccount), (BYTE*)&clNewMarketAccount);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKETITEMLIST(sPacketHeader* pPacket)
{
	sDBResponse_NewMarketItemList* pclMsg = (sDBResponse_NewMarketItemList*)pPacket;

	SI32 id = pclMsg->usCharID;
	if( false == pclCM->IsValidID(id) )
		return;

	SI64 currentPrice = 0;

	if( 0 < pclMsg->siItemUnique )
	{
		SI32 index = pclItemManager->FindItemRefFromUnique(pclMsg->siItemUnique);
		if(index >= 0)
		{
			currentPrice = pclItemManager->pclItemNewPrice->clUnit[index].siPrice;
		}
	}

	if(pclMsg->siResult != 1)
	{
		if( -1 == pclMsg->siResult )
		{
			cltGameMsgResponse_NewMarketItemList clNewMarketItemList( pclMsg->uiType, 1, currentPrice, pclMsg->clNewTradeOrder );
			cltMsg clMsg(GAMEMSG_RESPONSE_NEWMARKETITEMLIST, sizeof(cltGameMsgResponse_NewMarketItemList), (BYTE*)&clNewMarketItemList);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		return ;
	}

	cltGameMsgResponse_NewMarketItemList clNewMarketItemList( pclMsg->uiType, pclMsg->siMaxPage, currentPrice, pclMsg->clNewTradeOrder );
	cltMsg clMsg(GAMEMSG_RESPONSE_NEWMARKETITEMLIST, sizeof(cltGameMsgResponse_NewMarketItemList), (BYTE*)&clNewMarketItemList);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETFROMNEWMARKET(sPacketHeader* pPacket)
{
	sDBResponse_GetItemFromNewMarket* pclMsg = (sDBResponse_GetItemFromNewMarket*)pPacket;

	SI32 id = pclMsg->usCharID;
	if( false == pclCM->IsValidID(id) )
		return;

	if(pclMsg->siResult != 1)
	{
		SendServerResponseMsg( 0, SRVAL_GET_SAVEUSERITEM_FAIL,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	if( pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID )
		return;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 16] // 물품 회수 쓰기. 
		// pszParam1 == 최종 인벤의 아이템 정보.
		TCHAR str[50] = {'\0', };
		StringCchPrintf(str, sizeof(str), "Pos: %d, Unique: %d, Num: %d",
										pclMsg->siToPos, 
										pclMsg->clToItem.siUnique, 
										pclMsg->clToItem.siItemNum);

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MARKET, LOGCOMMAND_INDEX_MARKET_GETITEMFROMMARKET, 
			0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clGetItem, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, str, NULL);
	}	

	//---------------------------------
	// 인벤토리 정보를 업데이트 한다.
	//---------------------------------
	pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siToPos].Set(&pclMsg->clToItem);

	// 클라이언트로 업데이트 된 정보를 보낸다. 
	cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siToPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clToItem, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	sDBRequest_NewMarketAccount	clMsg2( id, siServerUnique, pclCM->CR[id]->pclCI->GetPersonID() );
	pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg2);

	SendServerResponseMsg( RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_SUCCESS_GETFROMMARKET,  0, 0, pclCM->CR[id]->GetCharUnique());
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETNEWMARKETFEE(sPacketHeader* pPacket)
{
	sDBResponse_SetNewMarketFee* pclMsg = (sDBResponse_SetNewMarketFee*)pPacket;

	if(pclMsg->siResult != 1)	return;

	// 서버의 해당 전장의 정보를 업데이트 한다. 
	cltStrInfo* pclStrInfo = pclNewMarketManager->GetStrInfo(pclMsg->siVillageUnique);
	if( NULL != pclStrInfo	)
	{
		pclStrInfo->clNewMarketStrInfo.clFee.Set(&pclMsg->clFee);
		pclNewMarketManager->m_bSetFee = false;
	}

	SI32 returnval	= SRVAL_NEWMARKET_SUCCESSSETFEE;

	cltCharServer* pclChar = pclCM->GetCharServer( pclMsg->siCharID );
	if( NULL == pclChar ) return;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_NEWMARKET, returnval,  pclStrInfo->clNewMarketStrInfo.clFee.GetFeeRate(), 0, pclChar->GetCharUnique());
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWTRADEMARKET(sPacketHeader* pPacket)
{
	sDBResponse_NewTradeMarket* pclMsg = (sDBResponse_NewTradeMarket*)pPacket;

	SI32 id = pclMsg->usCharID;
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if( NULL == pclChar )	return;

	if(pclMsg->siResult != 1)
	{
		SendServerResponseMsg(0, SRVAL_NEWMARKET_BUY_FAIL,  500, 0, pclChar->GetCharUnique());
		return ;
	}

	// [로그추가 : 황진성 2007. 10. 16] // 물품 구매 쓰기. 
	// param1 == 서버 유니크.
	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NEWMARKET, LOGCOMMAND_INDEX_NEWMARKET_BUY, 
							0,	pclChar, NULL, pclMsg->siSellerPersonID, pclMsg->siTradePrice, pclMsg->siMoney, &pclMsg->clTradeItem, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							pclMsg->siSellerServerUnique, 0, 0, 0, 0, NULL, NULL	);
	
	if( pclChar->pclCI->GetPersonID() != pclMsg->siBuyerPersonID )
		return;

	//---------------------------------
	// 거래된 가격을 설정한다.
	//---------------------------------
	if( pclMsg->clItem.siUnique && 0 < pclMsg->siTradePrice )
	{
		SI32 index = pclItemManager->FindItemRefFromUnique(pclMsg->clItem.siUnique);
		if( 0 < index )
		{
			// 시세
			if( 0 >= pclItemManager->pclItemNewPrice->clUnit[index].siPrice )
			{
				cltItemPriceUnit* pclprice = pclItemManager->GetCurrentNewPrice( pclMsg->clItem.siUnique );
				if( pclprice )
				{
					sDBRequest_SetItemNewPrice clMsg( pclprice->siUnique, MIN_ITEM_PRICE, (float)MIN_ITEM_PRICE, pclprice->siItemNum );
					pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg );
				}
			}

			pclItemManager->pclItemNewPrice->clUnit[index].siPrice		= pclMsg->fPrice;

			// 절대 0과 같거나 작아서는 안된다
			if(pclItemManager->pclItemNewPrice->clUnit[index].siPrice <= 0)
			{
				pclItemManager->pclItemNewPrice->clUnit[index].siPrice	= MIN_ITEM_PRICE;
			}
			if(pclItemManager->pclItemNewPrice->clUnit[index].fPrice <= 0.001f)
			{
				pclItemManager->pclItemNewPrice->clUnit[index].fPrice	= (float)MIN_ITEM_PRICE;
			}

			pclItemManager->SetCurrentNewPrice( pclMsg->clItem.siUnique, pclMsg->siTradePrice, pclMsg->siTradeItemNum );
		}
		
		//--------------------------------
		// 현금 보유 정보를 업데이트 한다. 
		//--------------------------------
		pclChar->pclCI->clIP.SetMoney(pclMsg->siMoney);
		pclChar->SetUpdateSwitch(UPDATE_IP, true, 0);

		//---------------------------------
		// 인벤토리 정보를 업데이트 한다.
		//---------------------------------
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_BUYFROMNEWMARKET, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclChar->SendNetMsg((sPacketHeader*)&clMsg);

			// 혹시 OBJECT가 아니면서 기간이 설정된 아이템이면 수정해주자.
			SI32 atb = pclItemManager->GetItemAtb(pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].siUnique);
			if ( ( atb & ITEMATB_OBJECT ) == 0 )
			{
				if ( pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateYear   > 0 ||
					 pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateMonth  > 0 ||
					 pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateDay    > 0 ||
					 pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateUseDay > 0	)
				{
					pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateYear   = 0;
					pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateMonth  = 0;
					pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateDay    = 0;
					pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos].uiDateUseDay = 0;

					// 오브젝트가 아닌 것은 버그이므로 기간을 없애버리자
					sDBRequest_WasteDurability clMsg(	id, 
														pclChar->pclCI->GetPersonID(), 
														pclMsg->siItemPos,
														&pclChar->pclCI->clCharItem.clItem[pclMsg->siItemPos]	);
					
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
				}
			}

			SendServerResponseMsg( RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_SUCCESS_TRADE,  0, 0, pclChar->GetCharUnique() );
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWORDERMARKET(sPacketHeader* pPacket)
{
	sDBResponse_NewOrderMarket* pclMsg = (sDBResponse_NewOrderMarket*)pPacket;

	SI32 id = pclMsg->usCharID;

	SI32 returnval = 0;
	// 클라이언트가 유효한 경우라면, 
	if(pclCM->IsValidID(id)== FALSE) return ;

	switch(pclMsg->siResult )
	{
	case 0:	
		break;
	case -1:
		{
			SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_ORDERSELL_OVERFLOW,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		break;
	case -5:
		{
			SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_FAIL_ORDERSELL_ALREADY_ORDERED,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		break;
	case 1:		
		{
			if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->clOrder.siPersonID)	
			{
				//---------------------------------
				// 인벤토리 정보를 업데이트 한다.
				//---------------------------------
				pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemFromPos].Set(&pclMsg->clFromItem);

				// 클라이언트로 업데이트 된 정보를 보낸다. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemFromPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clFromItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				//-------------------------------
				// 세금을 납부한다. 
				//-------------------------------
				if( 0 < pclMsg->siTax )
				{
					if(pclCM->CR[id]->pclCI->clIP.GetMoney() >= pclMsg->siTax)
					{
						pclCM->CR[id]->pclCI->clIP.SetMoney( pclCM->CR[id]->pclCI->clIP.GetMoney() - pclMsg->siTax );
						SI64 tempUserMoney = pclCM->CR[id]->pclCI->clIP.GetMoney();
						SI16 modUserMoney  = UserMoneyCapsule(tempUserMoney);

						sDBRequest_ChangeMoney clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_USENEWMARKET, -pclMsg->siTax );
						pclGameDBProcess->SendMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);

						CMinisterGoonzu *pclminister = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
						pclminister->AddTempFinances( pclMsg->siTax / 2); // 수수료에 50% 는 서버에서 사라지고 50%만 세계금고에 넣는다.

						// 통화량 조절 
						cltMoney clmoney;
						clmoney.Set(pclMsg->siTax / 2);
						clCurrencyInfo.clOutputMoney_clNewMarketTax.Increase(&clmoney); 
					}
				}

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					cltItem clOrderItem;
					clOrderItem.Set(&pclMsg->clOrder.clItemData);
					// [로그추가 : 황진성 2007. 10. 10] // 물품 등록 쓰기. 
					// param1	 == 주문번호. 
					// param2	 == 주문 개당 가격. 
					// param3    == 세금.
					// pszParam1 == 판매 주문한 아이템의 인벤 위치, 파는 아이템 유니크, 주문하고 남은 아이템의 개수.
					TCHAR str[50];
					StringCchPrintf(str, sizeof(str), "Pos: %d, Unique: %d, Num: %d",
									pclMsg->siItemFromPos, 
									clOrderItem.siUnique, 
									clOrderItem.siItemNum);
					SendLog_WriteLogToDB( LOGCOMMAND_TYPE_NEWMARKET, LOGCOMMAND_INDEX_NEWMARKET_ORDER, 
											0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &clOrderItem, pclMsg->clFromItem.GetItemNum(), 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->clOrder.siOrderIndex, pclMsg->clOrder.siPrice, pclMsg->siTax, 0, 0, str, NULL);
				}
			}

			SendServerResponseMsg(RANKTYPE_NEWMARKET, SRVAL_NEWMARKET_SUCCESS_ORDERSELL,  0, 0, pclCM->CR[id]->GetCharUnique());			
		}
		break;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKETRESULT(sPacketHeader* pPacket)
{
	sDBResponse_NewMarketResult* pclMsg = (sDBResponse_NewMarketResult*)pPacket;

	if( 1 != pclMsg->siResult )
	{
		return;
	}

	for( SI16 i = 0; i < MAX_NEWMARKET_RESULTLIST; ++i )
	{
		SI32 personID = pclMsg->stSellerInfo[i].siSellerPersonID;
		if( 0 < personID )
		{
			SI32 id = pclCM->GetIDFromPersonID( personID );
			SI16 itemUnique		= pclMsg->stSellerInfo[i].siSellItemUnique;
			SI32 itemNum		= pclMsg->stSellerInfo[i].siSellItemNum;
			SI64 price			= pclMsg->stSellerInfo[i].siSellPrice;
			SI32 villageUnique	= pclMsg->stSellerInfo[i].siVillageUnique;

			// [로그추가 : 황진성 2007. 10. 16] // 물품 판매 쓰기. 
			// pszCharParam1 == 서버번호, 아이템 유니크, 개수.
			TCHAR pszStrParam1[50] = {'\0', };
			StringCchPrintf(pszStrParam1, sizeof(pszStrParam1), "Server : %d, Unique: %d, Num: %d",
																pclMsg->siServerUnique,
																itemUnique, 
																itemNum		);
			
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NEWMARKET, LOGCOMMAND_INDEX_NEWMARKET_SELL, 
									0,	0, 	NULL, personID, price, 0, NULL, 0, 0, 0, villageUnique, 0, 0, 
									0, 0, 0, 0, 0, pszStrParam1, NULL	);

			if( false == pclCM->IsValidID(id) )
			{
				if( 0 < itemUnique )
				{
					if( 0 < price)
					{
						sDBRequest_NewMarket_IncreaseBankMoney clMsg( 0, personID, price );
						pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
					}

					//---------------------------------------------
					// 물건 판 사람의 계좌에서 수수료를 지불한다. 
					//--------------------------------------------
					cltStrInfo* pclstrinfo = pclNewMarketManager->GetStrInfo(villageUnique);
					if(pclstrinfo == NULL)
						return;

					SI64 fee = pclstrinfo->clNewMarketStrInfo.clFee.CalcFee( price ); 
 
					if( 0 < fee )
					{
						cltMoney clmoney;
						clmoney.itMoney = fee; // 통합마켓은 수수료 지불한다.
						sDBRequest_PayFee clinfo( 0, personID, villageUnique, RANKTYPE_NEWMARKET, FEETYPE_NEWMARKET_SELL, &clmoney);
						pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);
					}

					// 쪽지를 보낸다.
					cltLetterMsg_MarketTrade clletter( itemUnique, itemNum, price, fee, cltLetterMsg_MarketTrade::TYPE_NEWMARKET );
					SendLetterMsg( personID, (cltLetterHeader*)&clletter );
				}
			}
			else
			{
				if( 0 < itemUnique )
				{
					if( 0 < price)
					{
						sDBRequest_NewMarket_IncreaseBankMoney clMsg( id, personID, price );
						pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
					}

					//---------------------------------------------
					// 물건 판 사람의 계좌에서 수수료를 지불한다. 
					//--------------------------------------------
					cltStrInfo* pclstrinfo = pclNewMarketManager->GetStrInfo(villageUnique);
					if(pclstrinfo == NULL)
						return;

					SI64 fee = pclstrinfo->clNewMarketStrInfo.clFee.CalcFee( price);

					if( 0 < fee )
					{
						cltMoney clmoney;
						clmoney.itMoney = fee;
						sDBRequest_PayFee clinfo( id, personID, villageUnique, RANKTYPE_NEWMARKET, FEETYPE_NEWMARKET_SELL, &clmoney);
						pclGameDBProcess->SendMsg((sPacketHeader *)&clinfo);
					}

					// 쪽지를 보낸다.
					cltLetterMsg_MarketTrade clletter( itemUnique, itemNum, price, fee, cltLetterMsg_MarketTrade::TYPE_NEWMARKET );
					SendLetterMsg( personID, (cltLetterHeader*)&clletter );
				}
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKET_SETITEMPRICE(sPacketHeader* pPacket)
{
	//sDBResponse_NewMarketResult* pclMsg = (sDBResponse_NewMarketResult*)pPacket;

	//if( 1 != pclMsg->siResult )
	//{
	//	return;
	//}
}

void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKET_INCREASEBANKMONEY(sPacketHeader* pPacket)
{
	sDBResponse_NewMarket_IncreaseBankMoney* pclMsg = (sDBResponse_NewMarket_IncreaseBankMoney*)pPacket;

	if( 1 != pclMsg->siResult )
	{
		return;
	}

	//-------------------------------
	// ToBank 업데이트. 
	//-------------------------------
	SI32 villageUnique = pclMsg->siVillageUnique;
	if( 0 < villageUnique )
	{
		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageUnique);
		if(pclinfo)
		{
			pclinfo->clBankStrInfo.clTotalInputMoney.Set(&pclMsg->clTotalInputMoney);
			pclinfo->clCommonInfo.clMoney.Set(&pclMsg->clCurrentLeftMoney);
		}
	}

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if( false == pclCM->IsValidID(id) )
	{
		return;
	}

	pclCM->CR[id]->pclCI->clBank.SetMoney( pclMsg->clTotalMoney.itMoney );
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
}

// 아이템 가격 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYITEMNEWPRICE(sPacketHeader* pPacket)
{
	sDBResponse_ManyItemNewPrice* pclMsg = (sDBResponse_ManyItemNewPrice*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return ;
	}

	SI32 i;

	// 아이템 가격을 얻어온다
	for(i = 0;i < MANYITEM_PRICE_NUMBER;i++)
	{
		if( 0 < pclMsg->siItemUnique[i] )
		{
			SI32 index = pclItemManager->FindItemRefFromUnique(pclMsg->siItemUnique[i]);
			if( 0 < index )
			{
				pclItemManager->pclItemNewPrice->clUnit[index].siPrice		= pclMsg->fPrice[i];
				pclItemManager->pclItemNewPrice->clUnit[index].fPrice		= pclMsg->fPrice[i];
				pclItemManager->pclItemNewPrice->clUnit[index].siItemNum	= pclMsg->siItemNum[i];

				// 절대 0과 같거나 작아서는 안된다
				if(pclItemManager->pclItemNewPrice->clUnit[index].siPrice <= 0)
				{
					pclItemManager->pclItemNewPrice->clUnit[index].siPrice	= MIN_ITEM_PRICE;
				}
				if(pclItemManager->pclItemNewPrice->clUnit[index].fPrice <= 0.001f)
				{
					pclItemManager->pclItemNewPrice->clUnit[index].fPrice	= (float)MIN_ITEM_PRICE;
				}
			}
		}
	}

}

// 아이템 가격 요청에 대한 응답 완료
void cltServer::DoMsg_DBMSG_RESPONSE_MANYITEMNEWPRICE_COMPLETED(sPacketHeader* pPacket)
{
	sDBResponse_ManyItemNewPrice_Completed* pclMsg = (sDBResponse_ManyItemNewPrice_Completed*)pPacket;

	if(pclMsg->siResult != 1)
	{
		return ;
	}

	for(SI32 i = 0; i < MAX_ITEMINFO_NUMBER; ++i)
	{
#ifdef _DEBUG
		pclLog->FilePrint( "newitemprice.txt", "%d	%I64d %d", pclItemManager->pclItemNewPrice->clUnit[i].siUnique,
			pclItemManager->pclItemNewPrice->clUnit[i].siPrice, pclItemManager->pclItemNewPrice->clUnit[i].siItemNum );
#endif
		if(pclItemManager->pclItemNewPrice->clUnit[i].siUnique <= 0)	continue;
		if(pclItemManager->pclItemNewPrice->clUnit[i].siPrice > 0)		continue;

		pclItemManager->pclItemNewPrice->clUnit[i].siPrice	= MIN_ITEM_PRICE;
		pclItemManager->pclItemNewPrice->clUnit[i].fPrice	= (float)MIN_ITEM_PRICE;
	}

	pclNewMarketManager->bUpdateItemPrice = true;
}

// 아이템 가격 저장 
void cltServer::DoMsg_DBMSG_RESPONSE_SETITEMNEWPRICE(sPacketHeader* pPacket)
{

}

// 아이템 가격 저장 
void cltServer::DoMsg_DBMSG_RESPONSE_NEWMARKETREDUCEPRICENOTICE(sPacketHeader* pPacket)
{
	sDBResponse_NewMarketReducePriceNotice* pclMsg = (sDBResponse_NewMarketReducePriceNotice*)pPacket;

	if( 1 !=  pclMsg->siResult )
		return;

	for( SI16 i = 0 ; i < MAX_REDUCEPRICE_USERLIST; ++i )
	{		
		if( 0 < pclMsg->stData[ i ].siPersonID )
		{
			cltLetterMsg_MarketPriceDown clletter( pclMsg->stData[ i ].siItemUnique, pclMsg->stData[ i ].siItemQty, pclMsg->stData[ i ].clPrice.itMoney, cltLetterMsg_MarketPriceDown::TYPE_NEWMARKET );
			SendLetterMsg( pclMsg->stData[ i ].siPersonID, (cltLetterHeader*)&clletter);
		}
	}
}



