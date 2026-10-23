#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-RealEstateMarket.h"
#include "..\CommonLogic\RealEstate\RealEstate.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id)
{
	
	cltGameMsgResponse_RealEstateMarketCancelOrder *pclInfo = (cltGameMsgResponse_RealEstateMarketCancelOrder *)pclMsg->cData;

	pclRealEstateMarketManager->DelMyOrder( pclInfo->siOrderIndex );

	pclRealEstateMarketManager->bShowMyOrderSwitch = TRUE;
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_MYORDER(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_RealEstateMarketMyOrder *pclInfo = (cltGameMsgResponse_RealEstateMarketMyOrder *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclRealEstateMarketManager->clMyOrder.ZeroMem();
#else
	ZeroMemory( pclRealEstateMarketManager->clMyOrder, sizeof( pclRealEstateMarketManager->clMyOrder ) );
#endif

	for( int i = 0; i < pclInfo->usCount; ++i ) {
		pclRealEstateMarketManager->clMyOrder[ i ].Set( &pclInfo->clMyOrder[ i ] );
	}

	pclRealEstateMarketManager->bShowMyOrderSwitch = TRUE;

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_RealEstateMarketOrderList *pclinfo = (cltGameMsgResponse_RealEstateMarketOrderList *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclRealEstateMarketManager->clResponseOrder.ZeroMem();
#else
	ZeroMemory( pclRealEstateMarketManager->clResponseOrder, sizeof( pclRealEstateMarketManager->clResponseOrder ) );
#endif

	for( int i = 0; i < pclinfo->usCount; ++i ) {
		pclRealEstateMarketManager->clResponseOrder[ i ].Set( &pclinfo->clOrder[ i ] );
	}

	pclRealEstateMarketManager->siTotalResponseOrderNumber = pclinfo->usCount;
	pclRealEstateMarketManager->bShowResponseOrderSwitch = TRUE;

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_SELL(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_RealEstateMarketSell *pclInfo = (cltGameMsgResponse_RealEstateMarketSell *)pclMsg->cData;

	pclRealEstateMarketManager->AddMyOrder( &pclInfo->clRealEstateOrder );

	pclRealEstateMarketManager->bShowMyOrderSwitch = TRUE;
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_BUY(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_RealEstateMarketBuy *pclInfo = (cltGameMsgResponse_RealEstateMarketBuy *)pclMsg->cData;

	pclCM->CR[1]->pclCI->clRealEstate.Add( REALESTATE_TYPE_HOUSE,
		pclInfo->siVillageUnique, pclInfo->siHouseIndex, &pclInfo->clContract );

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_DETAILINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_RealEstateMarketDetailInfo *pclInfo = (cltGameMsgResponse_RealEstateMarketDetailInfo *)pclMsg->cData;

	pclRealEstateMarketManager->clDetailInfo.siVillageUnique = pclInfo->siVillageUnique;
	pclRealEstateMarketManager->clDetailInfo.siHouseIndex = pclInfo->siHouseIndex;
	pclRealEstateMarketManager->clDetailInfo.siCurrentRentFee = pclInfo->siCurrentRentFee;
	pclRealEstateMarketManager->clDetailInfo.siRentFee = pclInfo->siRentFee;
	pclRealEstateMarketManager->clDetailInfo.siRentPeriod = pclInfo->siRentPeriod;
	pclRealEstateMarketManager->clDetailInfo.clContractDate.Set( &pclInfo->clContractDate );
	pclRealEstateMarketManager->clDetailInfo.siItemNum = pclInfo->siItemNum;
	pclRealEstateMarketManager->clDetailInfo.siStableNum = pclInfo->siStableNum;
	pclRealEstateMarketManager->clDetailInfo.siHorseNum = pclInfo->siHorseNum;

	pclRealEstateMarketManager->bUpdateDetailInfo = TRUE;
}