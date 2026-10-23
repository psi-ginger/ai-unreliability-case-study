#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-ShipMarket.h"
#include "..\CommonLogic\ShipMarket\ShipMarket.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMARKET_CANCELORDER(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipMarketCancelOrder *pclInfo = (cltGameMsgResponse_ShipMarketCancelOrder *)pclMsg->cData;

	pclShipMarketManager->DelMyOrder( pclInfo->siOrderIndex );

	pclShipMarketManager->bShowMyOrderSwitch = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMARKET_MYORDER(cltMsg* pclMsg)
{
	
	cltGameMsgResponse_ShipMarketMyOrder *pclInfo = (cltGameMsgResponse_ShipMarketMyOrder *)pclMsg->cData;

	ZeroMemory( pclShipMarketManager->clMyOrder, sizeof( pclShipMarketManager->clMyOrder ) );

	for( int i = 0; i < pclInfo->usCount; ++i ) {
		pclShipMarketManager->clMyOrder[ i ].Set( &pclInfo->clMyOrder[ i ] );
	}

	pclShipMarketManager->bShowMyOrderSwitch = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMARKET_ORDERLIST(cltMsg* pclMsg)
{
	
	cltGameMsgResponse_ShipMarketOrderList *pclinfo = (cltGameMsgResponse_ShipMarketOrderList *)pclMsg->cData;

	ZeroMemory( pclShipMarketManager->clResponseOrder, sizeof( pclShipMarketManager->clResponseOrder ) );

	for( int i = 0; i < pclinfo->usCount; ++i ) {
		pclShipMarketManager->clResponseOrder[ i ].Set( &pclinfo->clOrder[ i ] );
	}

	pclShipMarketManager->siTotalResponseOrderNumber = pclinfo->usCount;
	pclShipMarketManager->bShowResponseOrderSwitch = TRUE;
	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMARKET_SELL(cltMsg* pclMsg)
{
	
	cltGameMsgResponse_ShipMarketSell *pclInfo = (cltGameMsgResponse_ShipMarketSell *)pclMsg->cData;

	pclShipMarketManager->AddMyOrder( &pclInfo->clShipOrder );

	pclShipMarketManager->bShowMyOrderSwitch = TRUE;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SHIPMARKET_BUY(cltMsg* pclMsg)
{
	cltGameMsgResponse_ShipMarketBuy *pclInfo = (cltGameMsgResponse_ShipMarketBuy *)pclMsg->cData;


	pclCM->CR[ 1 ]->pclCI->clShipInfo.clShip[ pclInfo->siSlot ].Set( &pclInfo->clShip );
	pclCM->CR[ 1 ]->pclCI->clShipItemInfo.clShipStgItemInfo[ pclInfo->siSlot ].Set( &pclInfo->clShipItemInfo );
	

	pclShipMarketManager->bShowMyOrderSwitch = TRUE;
}
