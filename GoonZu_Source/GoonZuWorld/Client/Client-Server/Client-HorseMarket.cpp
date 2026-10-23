#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-HorseMarket.h"
#include "..\CommonLogic\HorseMarket\HorseMarket.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_OUTHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HorseMarketOutHorse *pclInfo = (cltGameMsgResponse_HorseMarketOutHorse *)pclMsg->cData;

	pclHorseMarketManager->clMyAccount.Init();

	pclCM->CR[ 1 ]->pclCI->clHorseInfo.SetHorse( pclInfo->siHorseIndex, &pclInfo->clHorse );
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_MYACCOUNT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HorseMarketMyAccount *pclInfo = (cltGameMsgResponse_HorseMarketMyAccount *)pclMsg->cData;

	pclHorseMarketManager->clMyAccount.Set( &pclInfo->clHorseOrder );

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HorseMarketOrderList *pclinfo = (cltGameMsgResponse_HorseMarketOrderList *)pclMsg->cData;

	for( int i = 0; i < pclinfo->usCount; ++i ) {
		pclHorseMarketManager->clResponseOrder[ i ].Set( &pclinfo->clHorseOrder[ i ] );
	}

	pclHorseMarketManager->siTotalResponseOrderNumber = pclinfo->usCount;
	pclHorseMarketManager->bShowResponseOrderSwitch = TRUE;

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_SELLHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HorseMarketSellHorse *pclInfo = (cltGameMsgResponse_HorseMarketSellHorse *)pclMsg->cData;

	pclCM->CR[ 1 ]->pclCI->clHorseInfo.Init( pclInfo->siHorseIndex );

	pclHorseMarketManager->clMyAccount.Set( &pclInfo->clHorseOrder );

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_BUYHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HorseMarketBuyHorse *pclInfo = (cltGameMsgResponse_HorseMarketBuyHorse *)pclMsg->cData;

	pclHorseMarketManager->clMyAccount.Init();

	pclCM->CR[ 1 ]->pclCI->clHorseInfo.SetHorse( pclInfo->siHorseIndex, &pclInfo->clHorse );

	pclHorseMarketManager->bSelectedSwitch = TRUE;	

}

