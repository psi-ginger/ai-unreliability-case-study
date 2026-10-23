//---------------------------------
// 2003/8/19 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"
#include "../CommonLogic/Msg/MsgType-NPC.h"

#include "../../Server/AuthServer/AuthMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-ForeignMerchantNPC.h"
#include "../../DBManager/GameDBManager_World/DBMsg-IPGClient.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Market.h"


#include "SystemNPC\SystemNPC.h"
#include "SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"

#include "../../CommonLogic/MsgType-Person.h"

// NPC에게 판매할 것을 요청하는 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_SELLNPC(sPacketHeader* pPacket)
{
	sDBResponse_SellNPC* pclMsg = (sDBResponse_SellNPC*)pPacket;

	if(pclMsg->siResult != 1 )
	{
		return ;
	}

	SI32 id = pclMsg->usCharID;

	GMONEY BMoney = 0;
	if(pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		// 클라이언트로 업데이트 된 정보를 보낸다. 
		if( pclMsg->siItemPos >= 0 || pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			// 인벤토리를 업데이트 한다. 
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		
		BMoney = pclCM->CR[id]->pclCI->clIP.GetMoney();
		
		// 보유 자금을 업데이트 한다. 
		pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siMoney);

		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);
		
		// 거래 성공을 알려준다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_SELLNPC, SYSTEMNPC_SUCCESS_SELL);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
		// NPC 거래 금액의 일부를 마을에 상납한다.
		GMONEY incmoney = pclMsg->siChangedMoney;

		// 일정 부분만 마을의 수익금으로 
		incmoney = incmoney * VILLAGE_PROFIT_RATE_FOR_MERCHANT / 100;
		if(incmoney)
		{
			// villageunique로 자금 전달
			SI32 villageunique = pclMsg->siVillageUnique;

			if(villageunique > 0)
			{	
				//sDBRequest_ChangeStrMoney sendMsg(CHANGESTRMONEY_SELLNPC, 0, villageunique, RANKTYPE_CITYHALL, incmoney);
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, villageunique, incmoney );
				
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // NPC에게 판매.
			// param1 == 아이템의 위치, param2 == 아이템 유니크.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_SELL_ITEM, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siSellMoney * pclMsg->clSellItem.siItemNum, pclMsg->siMoney, 
								 &pclMsg->clSellItem, pclMsg->clItem.siItemNum, 0, pclMsg->siNPCKind, pclMsg->siVillageUnique, 0, 0, 
								 pclMsg->siItemPos, 0, 0, 0, 0, NULL, NULL);
		}
	}

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	// [로그추가 : 황진성 2007. 10. 9] // NPC 판매.
	//SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPCTRADE, LOGCOMMAND_INDEX_NPCTRADE_SELL, 
	//					 0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
	//					 0, 0, 0, 0, 0, NULL, NULL);
	//}

	// 아이템 판매 단가가 만원이상이 되면 로그에 남김.
	// 캐릭터명, 아이템유니크, 판매된 아이템갯수, 팔기전에 캐릭터의 소지금, 판후의 캐릭터의 소지금을 기록.
	if(pclMsg->siItemNum > 0 && (pclMsg->siChangedMoney / pclMsg->siItemNum) >= 10000)
	{
		pclLog->FilePrint(TEXT("Config\\SellItemLog.log"), TEXT("Name:%s	ItemUnique:%d	ItemNum:%d	Before Money:%I64d	After Money:%I64d"),
			    pclCM->CR[id]->GetName(), pclMsg->siIitemUnique, pclMsg->siItemNum, BMoney, pclMsg->siMoney);
	}	
}


// NPC에게서 구입할 것을 요청하는 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_BUYNPC(sPacketHeader* pPacket)
{
	sDBResponse_BuyNPC* pclMsg = (sDBResponse_BuyNPC*)pPacket;

	if( pclMsg->siResult != 1 )
	{
		return ;
	}


	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			// 인벤토리를 업데이트 한다. 
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		// 보유 자금을 업데이트 한다. 
		pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siMoney);

		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);


		SI32 itemUnique = pclMsg->clItem.siUnique;
		SI32 type = pclClient->pclItemManager->GetItemType(itemUnique);
		SI32 rtnMsg;				// 성공시 메세지

		//아이템 타입에 다른 효과음을 내기 위해서 

		if( itemUnique == ITEMUNIQUE(7850)  || itemUnique == ITEMUNIQUE(13600))		rtnMsg = SYSTEMNPC_SUCCESS_BUY_ARROW;	//화살
		else if( itemUnique == ITEMUNIQUE(7855) || itemUnique == ITEMUNIQUE(13601))	rtnMsg = SYSTEMNPC_SUCCESS_BUY_BULLET;	//총알
		
		else
		{
			switch( type)
			{
			case ITEMTYPE_FOOD:		//음식
				rtnMsg = SYSTEMNPC_SUCCESS_BUY_FOOD;	
				break;
			case ITEMTYPE_HEAL:		//약
				rtnMsg = SYSTEMNPC_SUCCESS_BUY_HEAL;	
				break;
			default:				//기본
				rtnMsg = SYSTEMNPC_SUCCESS_BUY;
				break;
			}
		}
		// 거래 성공을 알려준다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_BUYNPC, rtnMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // NPC에게 구매.
			// param1 == 아이템 위치.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_BUY_ITEM, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siBuyMoney*pclMsg->clBuyItem.siItemNum, pclMsg->siMoney, &pclMsg->clBuyItem, pclMsg->clItem.siItemNum, 0, pclMsg->siNPCkind, 0, 0, 0, 
											pclMsg->siItemPos, 0, 0, 0, 0, NULL, NULL);
		}

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		// [로그추가 : 황진성 2007. 10. 9] // NPC 구매.
		//// param1 == 거래성공여부.
		//SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPCTRADE, LOGCOMMAND_INDEX_NPCTRADE_BUY, 
		//	0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
		//	rtnMsg, 0, 0, 0, 0, NULL, NULL);
		//}
	}
}


// 교역 상인 NPC 의 정보를 DB에 저장한다.
void cltServer::DoMsg_DBMSG_RESPONSE_SETFOREIGNMERCHANTNPCINFO( sPacketHeader* pPacket )
{
	sDBResponse_SetForeignMerchantNPCInfo* pclMsg = (sDBResponse_SetForeignMerchantNPCInfo*)pPacket;

	if( pclMsg->siResult != 1 )
	{
		return ;
	}

	return;
}

// 교역 상인 NPC 의 정보를 DB로 부터 받아온다.
void cltServer::DoMsg_DBMSG_RESPONSE_GETFOREIGNMERCHANTNPCINFO( sPacketHeader* pPacket )
{
	/*
	sDBResponse_GetForeignMerchantNPCInfo* pclMsg = (sDBResponse_GetForeignMerchantNPCInfo*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if ( pclMsg->siResult == -1 )				// 상인 데이터가 없음.
	{
		pclForeignMerchantNPCMgr->SetReady( true );
		return ;
	}


	pclForeignMerchantNPCMgr->Set
		(
		pclMsg->siForeignMerchantKind,
		pclMsg->siItemUnique,
		pclMsg->siPrice,
		pclMsg->siRestNum,
		pclMsg->clDate 
		);


	pclForeignMerchantNPCMgr->SetReady( true );

	*/
	return;
}

// 교역 상인 NPC 가 캐쉬 아이템 구입
void cltServer::DoMsg_DBMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM( sPacketHeader* pPacket )
{
	sDBResponse_ForeignMerchantBuyCashItem* pclMsg = (sDBResponse_ForeignMerchantBuyCashItem*)pPacket;

	if( pclMsg->siResult != 1 )
	{
		return ;
	}

	SI32 id = pclMsg->siCharID;
	
	PushCash( id, pclMsg->siPersonID, pclMsg->szAccountID, pclMsg->uiEventMoney, PUSHCASH_FOREIGNMERCHANTBUY );	

	if( id > 0 && pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clLeftItem );

			cltGameMsgResponse_ForeignMerchantBuyCashItem clGameMsgResponse_ForeignMerchantBuyCashItem( pclMsg->siItemPos, &pclMsg->clLeftItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM, sizeof(clGameMsgResponse_ForeignMerchantBuyCashItem), (BYTE*)&clGameMsgResponse_ForeignMerchantBuyCashItem);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 무역상에 캐쉬아이템 판매시 기록.
			// param1 == siForeignMerchantCharID,
			// param2 == siForeignMerchantCharUnique,
			// param3 == 아이템위치
			// param4 == 이벤트 비용
			// pszparam1 == 계정
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPC, LOGCOMMAND_INDEX_NPC_TRADE_MERCHANT_BUY_CASHITEM, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clLeftItem, 0, 0, pclMsg->siForeignMerchantKind, 0, 0, 0, 
											pclMsg->siForeignMerchantCharID, 
											pclMsg->siForeignMerchantCharUnique, 
											pclMsg->siItemPos, 
											pclMsg->uiEventMoney, 
											0,
											pclMsg->szAccountID, 
											NULL);
		}

	}	

	return;
}

// NPC에게서 추천포인트로 상품을 구입할 것을 요청하는 것에 대한 응답.  
void cltServer::DoMsg_DBMSG_RESPONSE_BUYRECOMMANDPRODUCT(sPacketHeader* pPacket)
{
	sDBResonse_BuyRecommendProduct* pclMsg = (sDBResonse_BuyRecommendProduct*)pPacket;
	
	if( pclMsg->siResult != 1 )
	{
		return ;
	}

	SI32 id = pclMsg->usCharID;
	if( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			// 인벤토리를 업데이트 한다. 
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			// 클라이언트로 업데이트 된 정보를 보낸다. 
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

		// 성공적인 거래 결과를 알려준다. 
		cltGameMsgResponse_BuyRecommandProduct clinfo( pclMsg->siUsedPoint , pclMsg->siRecommendPoint, &pclMsg->clItem );
		cltMsg clMsg(GAMEMSG_RESPONSE_BUYRECOMMANDPRODUCT, sizeof(clinfo), (BYTE*)&clinfo  );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 추천 포인트로 아이템을 구입.
			// param1 == 아이템 위치,
			// param2 == 사용 포인트, 
			// param3 == 보유 포인트.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_BUY_RECOMMEND_PRODUCT, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem_Log, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
								 pclMsg->siItemPos, pclMsg->siUsedPoint, pclMsg->siRecommendPoint, 0, 0, 
								 NULL, NULL);
		}
	}
}


// NPC에게서 자신을 추천인으로 등록한 사람드의 목록을 얻어온다.
void cltServer::DoMsg_DBMSG_RESPONSE_RECOMMANDPOINTLIST(sPacketHeader* pPacket)
{
	sDBResponse_RecommendPointList* pclMsg = (sDBResponse_RecommendPointList*)pPacket;
	/*
	if( pclMsg->siResult != 1 )
	{

		//todo:



		return ;
	}*/

	SI32 id = pclMsg->uiCharID;
	if(pclCM->IsValidID(id) == false) return;

    if( pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID) return;



	cltGameMsgResponse_RecommandPointList clinfo(pclMsg->siCurrentPage,pclMsg->siMaxPage,pclMsg->sRPListData);
	cltMsg clMsg( GAMEMSG_RESPONSE_RECOMMANDPOINTLIST, sizeof(clinfo), (BYTE*)&clinfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

//KHY - 0530 명나라 상인 구매제한.
void cltServer::DoMsg_DBMSG_RESPONSE_GET_FOREIGNMERCHANTBUYLIMIT(sPacketHeader* pPacket)
{
	sDBResponse_GetForeignMerchantBuyLimit* pclMsg = (sDBResponse_GetForeignMerchantBuyLimit*)pPacket;

	if(pclMsg->siResult == 1) //성공
	{
		gForeignMerchantBuyLimit = pclMsg->gPrice;
		fForeignMerchantPercentUP = pclMsg->fUpPercent;
		fForeignMerchantPercentDOWN = pclMsg->fDownPercent;
	}
	else
	{
		gForeignMerchantBuyLimit = 0	;
		fForeignMerchantPercentUP = 0	;
		fForeignMerchantPercentDOWN = 0	;
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_GET_FOREIGNMERCHANTNOWMONEY(sPacketHeader* pPacket)
{
	sDBResponse_GetForeignMerchantNowMoney* pclMsg = (sDBResponse_GetForeignMerchantNowMoney*)pPacket;

	if(pclMsg->siResult == 1) //성공
		gForeignMerchantNowMoney = pclMsg->gPrice;
	else
		gForeignMerchantNowMoney = 0;
}

