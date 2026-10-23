#include "Char\CharManager\CharManager.h"

#include "..\Common\SystemNPC\TradeMerchantNPC\TradeMerchantNPC.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Map\Map.h"
#include "../Common/Char/KindInfo/kindinfo.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-TradeMerchant.h"

#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"
#include "../../CommonLogic/MsgRval-Define.h"

#include "../Minister/MinisterBase.h"
#include "../Minister/Minister-Izo.h"
#include "../Minister/MinisterMgr.h"


// 무역 상인이 현재 구입하는 정보를 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYINFO(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuyInfo* pclinfo = (cltGameMsgRequest_TradeMerchantBuyInfo*)pclMsg->cData;

	if ( pclinfo->siNPCCharUnique < 1 || pclinfo->siNPCKind < 1 )
	{
		return;
	}

	SI32 NPCID = pclCM->GetIDFromCharUnique( pclinfo->siNPCCharUnique );

	if ( NPCID < 1 || pclCM->IsValidID( NPCID ) == FALSE )
	{
		return;
	}

	if ( pclCM->CR[NPCID]->GetKind() != pclinfo->siNPCKind )
	{
		return;
	}

	CTradeMerchantNPC *pTradeMerchantNPC = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr( pclinfo->siNPCKind );
	
	if ( pTradeMerchantNPC == NULL )
	{
		return;
	}

	GetLocalTime(&sTime);
	if ( sTime.wHour < TRADEMERCHATNPC_BUY_START_TIME || sTime.wHour >= TRADEMERCHATNPC_BUY_END_TIME )
	{
		SendServerResponseMsg(0, SRVAL_TRADEMERCHANT_NOT_BUY_TIME,  0, 0, pclCM->CR[id]->GetCharUnique());
		// 가능 시간이 아님
		return;
	}

	// 아직 준비되지 않았음
	if ( ! pTradeMerchantNPC->m_bTradeMerchantSet )
	{
		SendServerResponseMsg(0, SRVAL_TRADEMERCHANT_PREPARE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	cltGameMsgResponse_TradeMerchantBuyInfo clGameMsgResponseTradeMerchantBuyInfo( pTradeMerchantNPC );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADEMERCHANTBUYINFO,  sizeof( clGameMsgResponseTradeMerchantBuyInfo ), (BYTE*)&clGameMsgResponseTradeMerchantBuyInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 무역 상인에게 유저가 물품을 판매
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}
	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuyItemFromPerson* pclinfo = (cltGameMsgRequest_TradeMerchantBuyItemFromPerson*)pclMsg->cData;

	if( pclinfo->siNPCKind < 1 )
	{
		return;
	}

	CTradeMerchantNPC *pTradeMerchantNPC = pclTradeMerchantNPCMgr->GetTradeMerchantNPCPtr( pclinfo->siNPCKind );

	// 피로방지 시스템 적용
	pTradeMerchantNPC->m_siBuyItemPrice = ((cltCharServer*)pclCM->CR[id])->CalcTiredPoint( pTradeMerchantNPC->m_siBuyItemPrice );
	
	if ( pTradeMerchantNPC == NULL )
	{
		return;
	}

	GetLocalTime(&sTime);
	if ( sTime.wHour < TRADEMERCHATNPC_BUY_START_TIME || sTime.wHour >= TRADEMERCHATNPC_BUY_END_TIME )
	{
		// 가능 시간이 아님
		return;
	}

	// 아직 준비되지 않았음
	if ( ! pTradeMerchantNPC->m_bTradeMerchantSet )
	{
		return;
	}

	// NPC 가 아이템을 구입할 정도의 돈이 되는지..
	if ( pTradeMerchantNPC->m_siBuyTotalMoney < pTradeMerchantNPC->m_siBuyItemPrice )
	{
		SendServerResponseMsg(0, SRVAL_TRADEMERCHANT_MONEY_INSUFFICIENTY,  0, 0, pclCM->CR[id]->GetCharUnique());
		
		return;
	}


	// 아이템 위치 확인
	if ( pclinfo->siItemPos < PERSONITEM_INV0 || pclinfo->siItemPos >= MAX_ITEM_PER_PERSON )
	{
		return;
	}

	// 아이템 위치 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique < 1 )
	{
		return;
	}

	// 아이템 일치 확인
	if ( ! pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].IsSame( &pclinfo->clItem, true ) )
	{
		return;
	}

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 판매할 수 없다.
	//------------------------------------------------------
	if( pclinfo->clItem.clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
		return;

	// NPC 가 구입중이 아이템이 맞는지 확인
	if ( pTradeMerchantNPC->m_siBuyItemUnique != pclinfo->clItem.siUnique )
	{
		return;
	}

	// 중첩 아이템 확인 - 중첩 아이템은 안됨
	if ( pclinfo->clItem.CanPile( pclItemManager ) )
	{
		return;
	}
	
	CMinisterIzo* pclMinister = (CMinisterIzo*)pclMinisterMgr->GetMinister( MINISTER_IZO );
	if ( pclMinister == NULL )
	{
		return;
	}

	GMONEY tradetax = ( pTradeMerchantNPC->m_siBuyItemPrice / 100 ) * pclMinister->m_siTradeTax ;
	GMONEY buyitemprice = pTradeMerchantNPC->m_siBuyItemPrice - tradetax ;

	sDBRequest_TradeMerchantBuyItemFromPerson clMsg( id, PersonID, pclinfo->siNPCKind, pclinfo->siItemPos, &pclinfo->clItem, buyitemprice,tradetax);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_NPCBUYITEM, buyitemprice);
}

// 상인이 말을 구입하는 정보 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTHORSEBUYINFO(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}
	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantHorseBuyInfo * pclinfo = (cltGameMsgRequest_TradeMerchantHorseBuyInfo *)pclMsg->cData;

	// 상인이 말 상인인지 확인
	if( pclinfo->siMerchantKind != GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) )
	{
		return;
	}

	// 말 소유 확인
	if ( ! pclCM->CR[id]->HaveHorse() )
	{
		return;
	}

	cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if( pclHorse == NULL ||	pclHorse->siHorseUnique <= 0)
		return;

	// 말 타고 있는지 확인
	if ( pclCM->CR[id]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
	{
		return;
	}

	// 거래 가능한 말인지 조건 체크
	SI32 CurTotalStat = pclHorse->siMoveSpeed +
						pclHorse->siStr +
						pclHorse->siDex +
						pclHorse->siMag +
						pclHorse->siStatusBonus;

	// Total Stat 이 9000 이상만 가능
	if ( CurTotalStat < 9000 )
	{
		return;
	}

	SI32 datevary = pclTime->clDate.GetDateVary();

	//SI32 FullLife	= pclHorseManager->GetFullLife(pclCM->CR[id]->pclCI->clHorseInfo.clHorse.siHorseUnique);
	//SI32 Life		= pclHorseManager->GetLife(&pclCM->CR[id]->pclCI->clHorseInfo.clHorse,  datevary );
	// 수명으로 남은 년. 
	//SI32 leftage	= max(0, (FullLife - Life) / 360);
	
	// 가격 설정이 아직 되지 않았음.
	if ( siHorseCurrentPrice < 1 )
	{
		return;
	}

	GMONEY HorsePrice = 0;
	switch ( pclHorse->siHorseUnique )
	{
	case 1:
		//HorsePrice = siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage;
		HorsePrice = siHorseCurrentPrice;
		break;
	case 2:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.25;
		HorsePrice = siHorseCurrentPrice * 13 / 10;
		break;
	case 3:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.50;
		HorsePrice = siHorseCurrentPrice * 16 / 10;
		break;
	case 4:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 5:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 6:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 7:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 8:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 9:
		//HorsePrice = ( siHorseCurrentPrice - siHorseCurrentPrice * 0.05 * leftage ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 22 / 10;
		break;
	default:
		return;
	}

	cltGameMsgResponse_TradeMerchantHorseBuyInfo clGameMsgResponse_TradeMerchantHorseBuyInfo( pclHorse->siHorseUnique, HorsePrice );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADEMERCHANTHORSEBUYINFO,  sizeof( clGameMsgResponse_TradeMerchantHorseBuyInfo ), (BYTE*)&clGameMsgResponse_TradeMerchantHorseBuyInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	return;
}

// 상인이 말을 구입
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}
	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuyHorseFromPerson * pclinfo = (cltGameMsgRequest_TradeMerchantBuyHorseFromPerson*)pclMsg->cData;

	// 말 소유 확인
	if ( ! pclCM->CR[id]->HaveHorse() )
	{
		return;
	}

	// 말 타고 있는지 확인
	if ( pclCM->CR[id]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
	{
		return;
	}

	// 말이 아이템 소유하고 있음.
	if ( pclCM->CR[id]->pclCI->clCharItem.IsHorseHaveItem() )
	{
		return;
	}

	cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
	
	if( pclHorse == NULL ||	pclHorse->siHorseUnique <= 0)
		return;

	// 거래 가능한 말인지 조건 체크
	SI32 CurTotalStat = pclHorse->siMoveSpeed +
						pclHorse->siStr +
						pclHorse->siDex +
						pclHorse->siMag +
						pclHorse->siStatusBonus;

	// Total Stat 이 9000 이상만 가능 ( 90.00 )
	if ( CurTotalStat < 9000 )
	{
		return;
	}

	SI32 datevary = pclTime->clDate.GetDateVary();

	//SI32 Life = pclHorseManager->GetLife( &pclCM->CR[id]->pclCI->clHorseInfo.clHorse, datevary );
	
	// 가격 설정이 아직 되지 않았음.
	if ( siHorseCurrentPrice < 1 )
	{
		return;
	}

	GMONEY HorsePrice = 0;
	switch ( pclHorse->siHorseUnique )
	{
	case 1:
		//HorsePrice = siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365;
		HorsePrice = siHorseCurrentPrice;
		break;
	case 2:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.25;
		HorsePrice = siHorseCurrentPrice * 13 / 10;
		break;
	case 3:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.50;
		HorsePrice = siHorseCurrentPrice * 16 / 10;
		break;
	case 4:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 5:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 6:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 7:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 8:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 19 / 10;
		break;
	case 9:
		//HorsePrice = ( siHorseCurrentPrice + siHorseCurrentPrice * 0.05 * ( Life - MAX_HORSE_LIFE ) / 365 ) * 1.75;
		HorsePrice = siHorseCurrentPrice * 22 / 10;
		break;
	default:
		return;
	}

	sDBRequest_TradeMerchantBuyHorseFromPerson clMsg( id, PersonID, GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")), pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse, HorsePrice );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return;
}

// 상인이 물건을 구입하는 정보를 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMINFO(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}
	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuyItemInfo * pclinfo = (cltGameMsgRequest_TradeMerchantBuyItemInfo*)pclMsg->cData;

	if ( pclinfo->siItemPos < 1 || pclinfo->siItemPos>= MAX_ITEM_PER_PERSON )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique < 1 )
	{
		return;
	}

	// 명품 또는 호품인지 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 ||
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
	}
	else
	{
		return;
	}

	// 아이템의 시세를 구한다. 
	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique );
	
	if ( pclprice == NULL )
	{
		return;
	}
	
	GMONEY Price	= 0;
	
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 )
	{
		Price = pclprice->siPrice * 10;			// 아이템 시세의 10배( 명품 )
	}
	else if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
		Price = pclprice->siPrice * 5;			// 아이템 시세의 5배( 호품 )
	}
	else if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
		Price = pclprice->siPrice * 2;			// 아이템 시세의 2배( 호품 )
	}

	cltGameMsgResponse_TradeMerchantBuyItemInfo clGameMsgResponse_TradeMerchantBuyItemInfo( GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")), pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique, Price );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMINFO,  sizeof( clGameMsgResponse_TradeMerchantBuyItemInfo ), (BYTE*)&clGameMsgResponse_TradeMerchantBuyItemInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	
	return;
}

// 상인이 물건을 구입
void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYITEM(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}
	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuyItem* pclinfo = (cltGameMsgRequest_TradeMerchantBuyItem*)pclMsg->cData;

	if ( pclinfo->siItemPos < 1 || pclinfo->siItemPos>= MAX_ITEM_PER_PERSON )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique < 1 )
	{
		return;
	}

	//------------------------------------------------------
	// 파티홀에서 받은 아이템은 판매할 수 없다.
	//------------------------------------------------------
	if( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiGetItemReason == cltItemCommonInfo::REASON_GIVE_PARTYHALL_ITEM )
		return;
	
	// 명품 또는 호품인지 확인
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 ||
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
	}
	else
	{
		return;
	}
	
	// 아이템의 시세를 구한다. 
	cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice( pclItemManager, pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].siUnique );
	
	if ( pclprice == NULL )
	{
		return;
	}
	
	GMONEY Price	= 0;
	
	if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 )
	{
		Price = pclprice->siPrice * 10;			// 아이템 시세의 10배( 명품 )
	}
	else if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1 )
	{
		Price = pclprice->siPrice * 5;			// 아이템 시세의 3배( 호품 )
	}
	//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
	else if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5 )
	{
		Price = pclprice->siPrice * 2;			// 아이템 시세의 2배( 신기 )
	}

	// 글로벌이 아니면 이전 아이템 판매 액수로 적용.
	if(!pclClient->IsCountrySwitch(Switch_SellToForeignMerchant))
	{	
		if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1  )
		{
			Price /= 5;			// 아이템 시세의 5배를 풀고 3배( 호품 )
			Price *= 3;
		}
		//[추가 : 황진성 2007. 12. 5 신기물품은 시세의 2배.]
		else if ( pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_5  )
		{
			Price /= 2;			// 아이템 시세의 2배를 푼다.( 신기 )
		}
	}
	
	sDBRequest_TradeMerchantBuyItem clMsg( id, PersonID, pclinfo->siMerchantKind, pclinfo->siItemPos, &pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ], Price );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
	pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_NPCBUYITEM, Price);
	return;
}

