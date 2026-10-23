//---------------------------------
// 2004/8/17 이제형
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"


#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-PersonalShop.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "Skill\Skill-Manager.h"

#include "MsgType-Person.h"
#include "MsgType-Item.h"
#include "Msg/MsgType-Personalshop.h"


void cltServer::DOMSG_DBMSG_RESPONSE_STARTPERSONALSHOP(sPacketHeader* pPacket)
{
	sDBResponse_StartPersonalShop* pclMsg = (sDBResponse_StartPersonalShop*)pPacket;

	// 해당 Item이 없다.
	if(pclMsg->siResult == 0)			return;

	SI32 id = pclMsg->siCharID;
	if(pclCM->IsValidID(id) == false)	return;

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	//================================================ 
	// Client의 정보를 인벤토리 Update한다.
	if(pclchar->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	//================================================ 
	
	//================================================ 
	// 마을에서 Ticket을 사용하면 마을에 5000냥을 추가한다.
	if(pclMsg->siVillageUnique>0 && pclMsg->siVillageUnique < MAX_VILLAGE_NUMBER)
	{
		sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, pclMsg->siVillageUnique, VILLAGE_PROFIT_FOR_PERSONALSHOP );
		pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
	}
	//================================================ 

	//============= 개인상점을 시작한다. ============= 
	// 캐릭터의 개인상점을 설정한다. 
	pclchar->SetHeadChat(false, NULL);				// 머릿말을 지운다. 
	pclchar->SetPersonalShopData(true, NULL);		// 기존의 정보로 설정한다.
	//================================================ 
	//================================================ 
	// 설정 정보를 설정한다.
	{
		cltGameMsgResponse_StartPersonalShop clinfo(pclchar->GetCharUnique(), 1, pclchar->bPremiumPersonalShop,pclMsg->m_siShopMode );
		cltMsg clMsg(GAMEMSG_RESPONSE_STARTPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			SI32 siCommandType = 0;
			SI32 siCommandIndex = 0;
			SI32 siReason = 0;
			if( pclMsg->m_siShopMode == PSHOP_MODE_MAKEITEM || 
				pclMsg->m_siShopMode == PSHOP_MODE_MAKEITEM_WANT)
			{
				siCommandType = LOGCOMMAND_TYPE_COMMISSION;
				siCommandIndex = LOGCOMMAND_INDEX_COMMISSION_START;
				siReason = USEITEM_REASON_COMMISSION;
			}
			else
			{
				siCommandType = LOGCOMMAND_TYPE_PRIVATETRADE;
				siCommandIndex = LOGCOMMAND_INDEX_PRIVATETRADE_START;
				siReason = USEITEM_REASON_PERSONSHOP;
			}

			// PCK : 아이템 사용로그 기록 남김.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
				siReason, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem_Log, 0, 
				0, 0, 0, 0, 0, pclMsg->siItemPos, 0, 0, 0, 0, NULL, NULL);

			// [로그추가 : 황진성 2007. 10. 16] // 개인상점 시작 쓰기. 
			// param1 == 프리미엄 여부. param2 == 아이템 유니크
			SendLog_WriteLogToDB(siCommandType, siCommandIndex, 
				0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				pclchar->bPremiumPersonalShop, pclMsg->clItem.siUnique, 0, 0, 0, NULL, NULL);
		}

	}
	//================================================ 
	//================================================ 
	// 이웃들에게 개인상점 베너를 보낸다.
	{
		cltGameMsgResponse_PersonalShopBanner clinfo(pclchar->GetCharUnique(), pclchar->bPersonalShopSwitch, &pclchar->clShopBanner);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONALSHOPBANNER, sizeof(clinfo), (BYTE*)&clinfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	//================================================ 

	//개인상점 구매 퀘스트가 진행중인지 확인한다. 
	((cltCharServer*)pclCM->CR[id])->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENPBSTORE);
}


void cltServer::DOMSG_DBMSG_RESPONSE_PERSONALSHOPBUYITEM(sPacketHeader* pPacket)
{
	sDBResponse_PersonalShopBuyItem * pclMsg = (sDBResponse_PersonalShopBuyItem *)pPacket;
	SI32 buyercharid	= pclMsg->siCharID;
	SI32 buyerpersonid	= pclMsg->siBuyerPersonID;
	SI32 buyerinvpos	= pclMsg->siBuyerInvPos;

	SI32 shoppercharid	= pclMsg->siShopperCharID;
	SI32 shopperpersonid= pclMsg->siShopperPersonID;
	SI32 shopperinvpos	= pclMsg->siShopperInvPos;
	GMONEY itemcoast = pclMsg->siItemCost;
	SI32 itemnum = pclMsg->siBuyItemNum;
	SI32 itemunique = pclMsg->clBuyerInvItem.siUnique;

	// 상점주인이 존재하지 않는다면 상점창을 닫도록 한다.
	cltShopTradeData	clShopData;
	clShopData.Init();
	clShopData.m_siShopMode = PSHOP_MODE_NONE;
	clShopData.m_siPersonID = shopperpersonid;


	if ( pclMsg->siResult < 1 )	
	{	
		// 갱신된 상점주인의 정보를 전송받는다.
		if(pclCM->IsValidID(shoppercharid) == TRUE && pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
		{
			cltCharServer* pclcharshopper = ((cltCharServer*)pclCM->CR[shoppercharid]);

			if( pclMsg->siShowCaseIndex >= 0 && pclMsg->siShowCaseIndex < MAX_ITEM_PER_PERSON )
			{
				pclcharshopper->clShopData.m_clItem[pclMsg->siShowCaseIndex].siItemNum += pclMsg->siBuyItemNum;
			}

			// 상점을 개설하지 않고 있다면 상점을 닫도록 한다.
			if(pclcharshopper->bPersonalShopSwitch == false)	
				pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
		
			clShopData.Set(&pclcharshopper->clShopData);
		}

		if(pclCM->IsValidID(buyercharid) == TRUE)
		{
			if(pclCM->CR[buyercharid]->pclCI->GetPersonID() == buyerpersonid)
			{
				// Client에게 siSuccess 오류정보를 전송한다.
				cltGameMsgResponse_BuyPersonalShop clInfo(buyercharid, BUYPERSONALSHOP_FAIL_UNKNOWN, pclMsg->clBuyerInvItem.siUnique,  pclMsg->siBuyItemNum);
				cltMsg clMsg(GAMEMSG_RESPONSE_BUYPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
				pclCM->CR[buyercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				
				//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				//{
				//	// [로그추가 : 황진성 2007. 10. 9] // 개인상점 구매 쓰기.
				//	// param1 == 결과 , param2 == 산 아이템 인벤위치. param3 == 아이템 유니크, param4 == 유저가 가진 해당 아이템 최종개수
				//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_BUY, 
				//						0, (cltCharServer*)(pclCM->CR[buyercharid]), NULL, pclMsg->siShopperPersonID, 0, 0, &pclMsg->clBuyItem, 0, 0, 0, 0, 0, 0, 
				//						BUYPERSONALSHOP_FAIL_UNKNOWN, pclMsg->siBuyerInvPos, pclMsg->clBuyerInvItem.siUnique, pclMsg->clBuyerInvItem.siItemNum, 0, NULL, NULL);	
				//}

				// 개인상점 갱신정보를 보낸다.
				{
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[buyercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
				
			}
		}
	}
	else // 성공시 
	{
		BOOL bShopSoldOut = TRUE;
		//===========================================================
		// 상점주인의 정보를 갱신하고 갱신된 정보를 설정하도록 한다.
		//===========================================================
		if(pclCM->IsValidID(shoppercharid) == TRUE && pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
		{
			// 판매완료에 따라서
			cltCharServer* pclcharshopper = ((cltCharServer*)pclCM->CR[shoppercharid]);
			
			// 상점을 개설하지 않고 있다면 상점을 닫도록 한다.
			if(pclcharshopper->bPersonalShopSwitch == false)	
				pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
			
			
			for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
			{
				if(pclcharshopper->clShopData.m_clItem[i].siItemNum > 0 )
				{
					bShopSoldOut = FALSE;
					break;
				}
			}
			// 상점아이템 전부 매진됬다면 상점모드 종료
			if(bShopSoldOut == TRUE)	pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
			clShopData.Set(&pclcharshopper->clShopData);

			// 상점 주인의 배너정보를 Update 한다. 
			pclcharshopper->SetPersonalShopBanner(&pclcharshopper->clShopData);


			//개인상점 판매 퀘스트가 진행중인지 확인한다. 
			pclcharshopper->DoQuestCheckAndAction(QUEST_CLEAR_COND_SELLFROMPBSTORE);
		}
		
		

		
		// 구매자 
		if(pclCM->IsValidID(buyercharid) == TRUE)
		{
			if(pclCM->CR[buyercharid]->pclCI->GetPersonID() == buyerpersonid)
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( buyerinvpos >= 0 || buyerinvpos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[buyercharid]->pclCI->clCharItem.clItem[buyerinvpos].Set(&pclMsg->clBuyerInvItem);

					cltGameMsgResponse_PersonItemInfo clinfo(buyerinvpos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clBuyerInvItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[buyercharid]->SendNetMsg((sPacketHeader*)&clMsg);
					
				}
				// 보유 자금을 업데이트 한다. 
				pclCM->CR[buyercharid]->pclCI->clIP.SetMoney(pclMsg->siBuyerMoney);
				pclCM->CR[buyercharid]->SetUpdateSwitch(UPDATE_IP, true, 0);
				
				// Client에게 성공정보를 보내준다.			
				{
					cltGameMsgResponse_BuyPersonalShop clInfo(buyercharid, BUYPERSONALSHOP_SUCCESS_BUY, pclMsg->clBuyerInvItem.siUnique);
					cltMsg clMsg(GAMEMSG_RESPONSE_BUYPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[buyercharid]->SendNetMsg((sPacketHeader*)&clMsg);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 10. 9] // 개인상점 판매 쓰기.
						// param1 == 판매 아이템 인벤위치. 
						// param2 == 아이템 유니크, 
						// param3 == 유저가 가진 해당 아이템 최종개수
						//SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_SELL, 
						//					0, (cltCharServer*)(pclCM->CR[pclMsg->siShopperCharID]), NULL, pclMsg->siBuyerPersonID, 0, 0, &pclMsg->clBuyItem, 0, 0, 0, 0, 0, 0, 
						//					pclMsg->siShopperInvPos, pclMsg->clShopperInvItem.siUnique, pclMsg->clShopperInvItem.siItemNum, 0, 0, NULL, NULL);	
					
						// [로그추가 : 황진성 2007. 10. 9] // 개인상점 구매 쓰기.
						// param1 == 산 아이템 인벤위치. 
						// param2 == 아이템 유니크, 
						// param3 == 유저가 가진 해당 아이템 최종개수
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_BUY, 
											0, (cltCharServer*)(pclCM->CR[buyercharid]), NULL, pclMsg->siShopperPersonID,  pclMsg->siItemCost, pclMsg->siBuyerMoney, &pclMsg->clBuyItem, 0, 0, 0, 0, 0, 0, 
											pclMsg->siBuyerInvPos, pclMsg->clBuyerInvItem.siUnique, pclMsg->clBuyerInvItem.siItemNum, 0, 0, NULL, NULL);	
					}

					

				}
				
				// 개인상점 갱신정보를 보낸다.
				{
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData,itemunique , itemnum, itemcoast, TRUE);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[buyercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
				
				//개인상점 구매 퀘스트가 진행중인지 확인한다. 
				((cltCharServer*)pclCM->CR[buyercharid])->DoQuestCheckAndAction(QUEST_CLEAR_COND_BUYFROMPBSTORE);
				
			}
		}


		//===========================================================
		// 상점주인 
		//===========================================================

		if(pclCM->IsValidID(shoppercharid) == TRUE)
		{
			if(pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( shopperinvpos >= 0 && shopperinvpos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[shoppercharid]->pclCI->clCharItem.clItem[shopperinvpos].Set(&pclMsg->clShopperInvItem);

					cltGameMsgResponse_PersonItemInfo clinfo(shopperinvpos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clShopperInvItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[shoppercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				// 보유 자금을 업데이트 한다. 
				pclCM->CR[shoppercharid]->pclCI->clIP.SetMoney(pclMsg->siShopperMoney);
				pclCM->CR[shoppercharid]->SetUpdateSwitch(UPDATE_IP, true, 0);

				//[진성] 귀속전용 이용권 소모. 요것은 아이템이 거래가 되면 하나를 없앤다. 왜? 돈으로 사니까.ㅋㅋ => 2008-4-21
				cltCharServer* pclchar = (cltCharServer* )pclCM->CR[shoppercharid];
				if(0 < pclchar->clShopData.m_clSpacialItem.siUnique)
				{
					sDBRequest_UseItem useitem(shoppercharid, 
												pclchar->pclCI->GetPersonID(), 
												pclchar->clShopData.m_siSelectedSpacialInventoryPos, 
												&pclchar->clShopData.m_clSpacialItem, 
												USEITEM_REASON_NONE);
					pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
				}
				
				
				// 물건이 몽딴 팔리면 Stop시키도록 한다.
				//else
				{

					// 개인상점 갱신정보를 보낸다.
					// TODO : 팔린 아이템 정보, 입수한 금액 표시
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData,itemunique , itemnum, itemcoast,FALSE );
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[shoppercharid]->SendNetMsg((sPacketHeader*)&clMsg);

					if(bShopSoldOut == TRUE)
					{
						// 캐릭터의 개인상점을 설정한다. 
						((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopBanner(NULL);
						((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopData(false, NULL);

						// 이웃들에게 종료가 되었음을 알려준다.
						cltGameMsgResponse_StopPersonalShop clinfo(pclCM->CR[shoppercharid]->GetCharUnique(), pclCM->CR[shoppercharid]->pclCI->GetPersonID(), 2);
						cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
						((cltCharServer*)pclCM->CR[shoppercharid])->SendNeighbourMsg(&clMsg, true);

						if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
						{
							// [로그추가 : 황진성 2007. 10. 9] // 개인상점 종료 쓰기. 
							// param1 == 결과.
							SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_END, 
												0, (cltCharServer*)(pclCM->CR[shoppercharid]), NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
												2, 0, 0, 0, 0, NULL, NULL);
						}
					}
				}
			}
		}
	}

	return;
}

void cltServer::DOMSG_DBMSG_RESPONSE_PERSONALSHOPSELLITEM(sPacketHeader* pPacket)
{
	sDBResponse_PersonalShopSellItem * pclMsg = (sDBResponse_PersonalShopSellItem *)pPacket;
	SI32 sellercharid	= pclMsg->siCharID;
	SI32 sellerpersonid	= pclMsg->siSellerPersonID;
	SI32 sellerinvpos	= pclMsg->siSellerInvPos;

	SI32 shoppercharid	= pclMsg->siShopperCharID;
	SI32 shopperpersonid= pclMsg->siShopperPersonID;
	SI32 shopperinvpos	= pclMsg->siShopperInvPos;
	SI32 itemnum   = pclMsg->siSellItemNum;
	GMONEY itemcoast = pclMsg->siItemCost;
	SI32 itemunique = pclMsg->clShopperInvItem.siUnique;

	// 상점주인이 존재하지 않는다면 상점창을 닫도록 한다.
	cltShopTradeData	clShopData;
	clShopData.Init();
	clShopData.m_siShopMode = PSHOP_MODE_NONE;
	clShopData.m_siPersonID = shopperpersonid;

	if ( pclMsg->siResult < 1 )	
	{	
		// 갱신된 상점주인의 정보를 전송받는다.
		if(pclCM->IsValidID(shoppercharid) == TRUE && pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
		{
			cltCharServer* pclcharshopper = ((cltCharServer*)pclCM->CR[shoppercharid]);

			if( pclMsg->siShowCaseIndex >= 0 && pclMsg->siShowCaseIndex < MAX_ITEM_PER_PERSON)
			{
				pclcharshopper->clShopData.m_clItem[pclMsg->siShowCaseIndex].siItemNum += pclMsg->siSellItemNum;
			}

			// 상점을 개설하지 않고 있다면 상점을 닫도록 한다.
			if(pclcharshopper->bPersonalShopSwitch == false)	
				pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
		
			clShopData.Set(&pclcharshopper->clShopData);
		}

		if(pclCM->IsValidID(sellercharid) == TRUE)
		{
			if(pclCM->CR[sellercharid]->pclCI->GetPersonID() == sellerpersonid)
			{
				// Client에게 siSuccess 오류정보를 전송한다.
				cltGameMsgResponse_SellPersonalShop clInfo(sellercharid, SELLPERSONALSHOP_FAIL_UNKNOWN, pclMsg->clSellerInvItem.siUnique,  pclMsg->siSellItemNum);
				cltMsg clMsg(GAMEMSG_RESPONSE_SELLPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
				pclCM->CR[sellercharid]->SendNetMsg((sPacketHeader*)&clMsg);

				//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				//{
				//	// [로그추가 : 황진성 2007. 10. 9] // 개인상점 판매 쓰기. 
				//	// param1 == 성공여부, param2 == 아이템 유니크.
				//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_SELL, 
				//						0, (cltCharServer*)(pclCM->CR[shoppercharid]), NULL, 0, 0, 0, NULL, pclMsg->siSellItemNum, 0, 0, 0, 0, 0,
				//						SELLPERSONALSHOP_FAIL_UNKNOWN, pclMsg->clShopperInvItem.siUnique,  0, 0, 0, NULL, NULL);
				//}

				
				// 개인상점 갱신정보를 보낸다.
				{
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[sellercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
				
			}
		}
	}
	else // 성공시 
	{
		
		BOOL bShopSoldOut = TRUE;
		//===========================================================
		// 상점주인의 정보를 갱신하고 갱신된 정보를 설정하도록 한다.
		//===========================================================
		if(pclCM->IsValidID(shoppercharid) == TRUE && pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
		{
			// 판매완료에 따라서
			cltCharServer* pclcharshopper = ((cltCharServer*)pclCM->CR[shoppercharid]);
			
			// 상점을 개설하지 않고 있다면 상점을 닫도록 한다.
			if(pclcharshopper->bPersonalShopSwitch == false)	
				pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
	
			
			for(SI32 i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i)
			{
				if(pclcharshopper->clShopData.m_clItem[i].siItemNum > 0 )
				{
					bShopSoldOut = FALSE;
					break;
				}
			}
			if(bShopSoldOut == TRUE)	pclcharshopper->clShopData.m_siShopMode = PSHOP_MODE_NONE;
			clShopData.Set(&pclcharshopper->clShopData);
			
			// 상점 주인의 배너정보를 Update 한다.
			pclcharshopper->SetPersonalShopBanner(&pclcharshopper->clShopData);

			//개인거래 퀘스트가 진행중인지 확인한다. 
			pclcharshopper->DoQuestCheckAndAction(QUEST_CLEAR_COND_BUYTOPBSTORE);
		}
		

		// 판매자  
		if(pclCM->IsValidID(sellercharid) == TRUE)
		{
			if(pclCM->CR[sellercharid]->pclCI->GetPersonID() == sellerpersonid)
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( sellerinvpos >= 0 && sellerinvpos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[sellercharid]->pclCI->clCharItem.clItem[sellerinvpos].Set(&pclMsg->clSellerInvItem);

					cltGameMsgResponse_PersonItemInfo clinfo(sellerinvpos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clSellerInvItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[sellercharid]->SendNetMsg((sPacketHeader*)&clMsg);

				}
				// 보유 자금을 업데이트 한다. 
				pclCM->CR[sellercharid]->pclCI->clIP.SetMoney(pclMsg->siSellerMoney);
				pclCM->CR[sellercharid]->SetUpdateSwitch(UPDATE_IP, true, 0);

				// Client에게 성공정보를 보내준다.			
				{
					cltGameMsgResponse_SellPersonalShop clInfo(sellercharid, SELLPERSONALSHOP_SUCCESS_SELL, pclMsg->clShopperInvItem.siUnique,  pclMsg->siSellItemNum);
					cltMsg clMsg(GAMEMSG_RESPONSE_SELLPERSONALSHOP, sizeof(clInfo), (BYTE*)&clInfo);
					pclCM->CR[sellercharid]->SendNetMsg((sPacketHeader*)&clMsg);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 10. 9] // 개인상점 판매 쓰기. 
						// param1 == seller의 아이템 위치, param2 == seller의 아이템 유니크.
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_SELL, 
											0, (cltCharServer*)(pclCM->CR[sellercharid]), NULL, shopperpersonid, itemcoast, pclMsg->siSellerMoney, &pclMsg->clSellerInvItem_Log, pclMsg->siSellItemNum, 0, 0, 0, 0, 0,
											sellerinvpos, pclMsg->clSellerInvItem_Log.siUnique,  0, 0, 0, NULL, NULL);
					}

				}

				// 개인상점 갱신정보를 보낸다.
				{
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData,itemunique, itemnum,itemcoast,FALSE);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[sellercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}

			}
		}
		//===========================================================
		// 상점주인 
		//===========================================================
		if(pclCM->IsValidID(shoppercharid) == TRUE)
		{
			if(pclCM->CR[shoppercharid]->pclCI->GetPersonID() == shopperpersonid)
			{
				// 클라이언트로 업데이트 된 정보를 보낸다. 
				if( shopperinvpos >= 0 && shopperinvpos < MAX_ITEM_PER_PERSON )
				{
					pclCM->CR[shoppercharid]->pclCI->clCharItem.clItem[shopperinvpos].Set(&pclMsg->clShopperInvItem);

					cltGameMsgResponse_PersonItemInfo clinfo(shopperinvpos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clShopperInvItem, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[shoppercharid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
				
				// 보유 자금을 업데이트 한다. 
				pclCM->CR[shoppercharid]->pclCI->clIP.SetMoney(pclMsg->siShopperMoney);
				pclCM->CR[shoppercharid]->SetUpdateSwitch(UPDATE_IP, true, 0);
				
				// 물건이 몽딴 팔리면 Stop시키도록 한다.
				/*
				if(bShopSoldOut == TRUE)
				{
					// 캐릭터의 개인상점을 설정한다. 
					((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopBanner(NULL);
					((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopData(false, NULL);
					
					// 이웃들에게 종료가 되었음을 알려준다.
					cltGameMsgResponse_StopPersonalShop clinfo(pclCM->CR[shoppercharid]->GetCharUnique(), pclCM->CR[shoppercharid]->pclCI->GetPersonID(), 2);
					cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
					((cltCharServer*)pclCM->CR[shoppercharid])->SendNeighbourMsg(&clMsg, true);
				}
				else*/
				{
					// 개인상점 갱신정보를 보낸다.
					cltGameMsgResponse_UpdatePersonalShopData clinfo(shoppercharid, &clShopData,itemunique ,itemnum, itemcoast,TRUE);
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[shoppercharid]->SendNetMsg((sPacketHeader*)&clMsg);

					if(bShopSoldOut == TRUE)
					{
						// 캐릭터의 개인상점을 설정한다. 
						((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopBanner(NULL);
						((cltCharServer* )pclCM->CR[shoppercharid])->SetPersonalShopData(false, NULL);

						// 이웃들에게 종료가 되었음을 알려준다.
						cltGameMsgResponse_StopPersonalShop clinfo(pclCM->CR[shoppercharid]->GetCharUnique(), pclCM->CR[shoppercharid]->pclCI->GetPersonID(), 2);
						cltMsg clMsg(GAMEMSG_RESPONSE_STOPPERSONALSHOP, sizeof(clinfo), (BYTE*)&clinfo);
						((cltCharServer*)pclCM->CR[shoppercharid])->SendNeighbourMsg(&clMsg, true);

						if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
						{
							// [로그추가 : 황진성 2007. 10. 9] // 개인상점 종료 쓰기.
							// param1 == 결과.
							SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRIVATETRADE, LOGCOMMAND_INDEX_PRIVATETRADE_END, 
												0, (cltCharServer*)(pclCM->CR[shoppercharid]), NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
												2, 0, 0, 0, 0, NULL, NULL);
						}
					}	
				}
			}
		}	
	}
}