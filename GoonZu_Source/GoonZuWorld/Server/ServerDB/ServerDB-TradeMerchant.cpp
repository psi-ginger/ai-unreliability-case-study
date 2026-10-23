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

#include "../../Server/AuthServer/AuthMsg.h"
#include "../../Server/Statistics/Statistics.h"

#include "../../Server/Minister/MinisterBase.h"
#include "../../Server/Minister/MinisterMgr.h"
#include "../../Server/Minister/Minister-Izo.h"
#include "../../Server/Minister/Minister-Goonzu.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-TradeMerchant.h"
#include "SystemNPC\SystemNPC.h"
#include "SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"
#include "../../Common/SystemNPC/TradeMerchantNPC/TradeMerchantNPC.h"


// 무역 상인 NPC 의 기본 정보를 DB에 저장한다.
void cltServer::DoMsg_DBMSG_RESPONSE_TRADEMERCHANTSET( sPacketHeader* pPacket )
{
	sDBResponse_TradeMerchantSet* pclMsg = (sDBResponse_TradeMerchantSet* )pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}
	
	CTradeMerchantNPC *pTradeMerchantNPC = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr( pclMsg->siTradeMerchantKind );

	if ( pTradeMerchantNPC == NULL )
	{
		pclLog->FilePrint( TEXT("Config/TradeMerchant.log"), TEXT("DoMsg_DBMSG_RESPONSE_TRADEMERCHANTSET pTradeMerchatnNPC == NULL, pclMsg->siTradeMerchantKind = %d"), pclMsg->siTradeMerchantKind );
		return;
	}

	SI32 siTradeTax = 5 ;

	if ( pclMinisterMgr )
	{
		CMinisterIzo *pclminister = (CMinisterIzo *)pclMinisterMgr->GetMinister(MINISTER_IZO);
		if ( pclminister )
			siTradeTax = pclminister->m_siTradeTax ;
	}

	pTradeMerchantNPC->Set( true, pclMsg->siTradeMerchantKind, pclMsg->siBuyItemUnique, pclMsg->siBuyItemPrice, pclMsg->siBuyTotalMoney ,siTradeTax);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 10] // 어떤 무역상인이 구입할려는 것을 설정.
		// param1 == 총 구입 가격, 
		// param2 == 구입하는 아이템, 
		// param3 == 구입 가격, 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPC, LOGCOMMAND_INDEX_NPC_TRADE_MERCHANT_SET, 
							 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, pclMsg->siTradeMerchantKind, 0, 0, 0, 
							 pclMsg->siBuyTotalMoney, pclMsg->siBuyItemUnique, pclMsg->siBuyItemPrice, 0, 0, 
							 NULL, NULL);
	}
	return;
}

// 무역 상인에게 아이템을 판매한다. ( 무역 상인 입장에서는 구입 )
void cltServer::DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON( sPacketHeader* pPacket )
{
	sDBResponse_TradeMerchantBuyItemFromPerson* pclMsg = (sDBResponse_TradeMerchantBuyItemFromPerson* )pPacket;

	// 데이터 검증 추가
	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )			return;

	if ( pclMsg->siResult != 1 )
	{
		if ( pclMsg->siResult == 2 )
		{
			SI32 id1 = pclMsg->siCharID;
			if ( id1 > 0 && pclCM->IsValidID( id1 ) == TRUE && pclCM->CR[id1]->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				SendServerResponseMsg(0, SRVAL_TRADEMERCHANT_MONEY_INSUFFICIENTY,  0, 0, pclCM->CR[id1]->GetCharUnique());
			}
		}
		else
		{
			pclLog->FilePrint( TEXT("Config/TradeMerchant.log"), TEXT("DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON siResult = %d"), pclMsg->siResult );
		}
		
		return;
	}
	
	SI32 id = pclMsg->siCharID;

	CTradeMerchantNPC *pTradeMerchantNPC = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr( pclMsg->siTradeMerchantKind );

	if ( pTradeMerchantNPC == NULL )
	{
		pclLog->FilePrint( TEXT("Config/TradeMerchant.log"), TEXT("DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON pTradeMerchatnNPC == NULL, pclMsg->siTradeMerchantKind = %d"), pclMsg->siTradeMerchantKind );
		return;
	}

	pTradeMerchantNPC->ChangeBuyTotalMoney( pclMsg->siLeftBuyTotalMoney );

	//-----------------------------------
	// 명, 일본, 러시아 무역상 거래량 통계
	//-----------------------------------
	++pclStatisticsManager->clDailyStatistics.siTradeMerchantNum; 
	pclStatisticsManager->clDailyStatistics.siTradeMerchantMoney += pclMsg->siIncreateMoney;
	pclStatisticsManager->clDailyStatistics.siTradeTax += pclMsg->siTradeTax;
	//-----------------------------------

	// 왕실자금 적립
	CMinisterGoonzu *pclminister = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
	pclminister->AddTempFinances( pclMsg->siTradeTax );
	
	// 통화량 조절 
	cltMoney clmoney;
	clmoney.Set(pclMsg->siTradeTax);
	clCurrencyInfo.clOutputMoney_clMarketTax.Increase(&clmoney);
	

	if ( id > 0 && pclCM->IsValidID( id ) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clItem );
		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );
		
		cltGameMsgResponse_TradeMerchantBuyItemFromPerson clTradeMerchantBuyItemFromPerson( pclMsg->siItemPos, &pclMsg->clItem, pclMsg->siIncreateMoney, pclMsg->siLeftMoney );
		cltMsg clMsg(GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON, sizeof(clTradeMerchantBuyItemFromPerson), (BYTE*)&clTradeMerchantBuyItemFromPerson);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);


		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 유저가 무역 상인에게 물건을 판매한다.(무역 상인 입장에서는 구입)
			// param1	 == 아이템 위치, 
			// param3    == Trade Tax
			// param5    == Left Buy Total Money
			// TEMP LEEKH -------------------------------------------------------------------
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPC, LOGCOMMAND_INDEX_NPC_TRADE_MERCHANT_BUY_ITEM, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siIncreateMoney, pclMsg->siLeftMoney, &pclMsg->clItem, 0, 0, pclMsg->siTradeMerchantKind, 0, 0, 0, 
								 pclMsg->siItemPos, 0, pclMsg->siTradeTax, 0, pclMsg->siLeftBuyTotalMoney,
								 NULL, NULL);
		}

	}
	
	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON( sPacketHeader* pPacket )
{
	sDBResponse_TradeMerchantBuyHorseFromPerson* pclMsg = (sDBResponse_TradeMerchantBuyHorseFromPerson* )pPacket;

	if ( pclMsg->siResult != 1 )
	{
		pclLog->FilePrint( TEXT("Config/TradeMerchant.log"), TEXT("DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON siResult = %d"), pclMsg->siResult );
		
		return;
	}

	SI32 id = pclMsg->siCharID;
	

	//---------------------------------------
	// 통계에 추가
	//---------------------------------------
	++pclStatisticsManager->clDailyStatistics.siSystemBuyHorseNum;
	pclStatisticsManager->clDailyStatistics.siSystemBuyHorseMoney += pclMsg->siIncreaseMoney;
	//---------------------------------------


	

	if ( id > 0 && pclCM->IsValidID( id ) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		cltHorse* pclHorse = pclCM->CR[ id ]->pclCI->clHorseInfo.GetCurrentShowHorse();
		if ( pclHorse && pclHorse->siHorseUnique > 0 )
		{
			++siHorseTradeCount;
		}

		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );
		
		pclCM->CR[id]->UnrideHorse();
		if(pclHorse)	pclHorse->Init();
		
		//---------------------------------------
		// 이웃들에게 말정보 변경을 알림 ( 말 교환, 또는 보냄 )
		//---------------------------------------
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyHorseStatus( pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse);

		cltGameMsgResponse_TradeMerchantBuyHorseFromPerson clTradeMerchantBuyHorseFromPerson( pclMsg->siIncreaseMoney, pclMsg->siLeftMoney );
		cltMsg clMsg(GAMEMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON, sizeof(clTradeMerchantBuyHorseFromPerson), (BYTE*)&clTradeMerchantBuyHorseFromPerson);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
		// 말 정보를 업데이트 시킨다. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);



		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 상인이 말을 구입
			// param1 == 소유한 말의 index, 
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_NPC, LOGCOMMAND_INDEX_NPC_TRADE_MERCHANT_BUY_HORSE, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, pclMsg->siIncreaseMoney, pclMsg->siLeftMoney, NULL, 0, 0, pclMsg->siTradeMerchantKind, 0, 0, 0, 
											pclMsg->siPersonHorseIndex, 0, 0, 0, 0, NULL, NULL);
		}

	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEM( sPacketHeader* pPacket )
{
	sDBResponse_TradeMerchantBuyItem* pclMsg = (sDBResponse_TradeMerchantBuyItem* )pPacket;

	// 데이터 검증 추가
	if( pclMsg->siItemPos < 0 || pclMsg->siItemPos >= MAX_ITEM_PER_PERSON )			return;

	if ( pclMsg->siResult != 1 )
	{
		pclLog->FilePrint( TEXT("Config/TradeMerchant.log"), TEXT("DoMsg_DBMSG_RESPONSE_TRADEMERCHANTBUYITEM siResult = %d"), pclMsg->siResult );
		
		return;
	}
	
	SI32 id = pclMsg->siCharID;

	if ( id > 0 && pclCM->IsValidID( id ) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if ( pclMsg->siTradeMerchantKind == GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")) )
		{
			pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );

			pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos ].Set( &pclMsg->clLeftItem );
			
			cltGameMsgResponse_TradeMerchantBuyItem clGameMsgResponse_TradeMerchantBuyItem( GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")), pclMsg->siItemPos, &pclMsg->clLeftItem, pclMsg->siBuyItemUnique, pclMsg->siIncreaseMoney, pclMsg->siLeftMoney );
			cltMsg clMsg(GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEM, sizeof(clGameMsgResponse_TradeMerchantBuyItem), (BYTE*)&clGameMsgResponse_TradeMerchantBuyItem);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}		
	}

	return;
}
