//---------------------------------
// 2003/11/13 김태곤
//---------------------------------

#include "Client.h"
#include "Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "Interface\StockDistributionDlg\StockDistributionDlg.h"
#include "Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "Prompt\PromptBaseDefinition.h"
#include "Prompt\PromptMgr.h"


#include "MsgType-Stock.h"

#include "NInterface/NStockDistributionDlg/NStockDistributionDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "ninterface/NSystemRewardList/NExchangeStock.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKORDERINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockOrderInfo* pclInfo = (cltGameMsgResponse_StockOrderInfo*)pclMsg->cData;

	// 받은 주문 정보를 주문 관리자에게 보낸다. 
#ifdef _SAFE_MEMORY
	pclStockManager->SetMostOrder( &pclInfo->clSellTradeOrder[0] , &pclInfo->clBuyTradeOrder[0] , pclInfo->siAverageSell, pclInfo->siAverageBuy );
#else
	pclStockManager->SetMostOrder( pclInfo->clSellTradeOrder , pclInfo->clBuyTradeOrder , pclInfo->siAverageSell, pclInfo->siAverageBuy );
#endif
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKBIDVILLAGELIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockBidVillageList* pclInfo = (cltGameMsgResponse_StockBidVillageList *)pclMsg->cData;

	int count = pclInfo->usCount;

	if( count > MAX_VILLAGE_NUMBER )		count = MAX_VILLAGE_NUMBER;
	for( int i = 0; i < count; ++i ) {

		//pclStockManager->clStockBidVillageInfo[ pclInfo->clStockBidVillageInfo[ i ].siVillageUnique ].Set(
		//	&pclInfo->clStockBidVillageInfo[ i ] );

		if( pclInfo->clStockBidVillageInfo[ i ].siVillageUnique_ >= 0 &&
			pclInfo->clStockBidVillageInfo[ i ].siVillageUnique_ < MAX_VILLAGE_NUMBER )
		{
			pclStockManager->clStockBidVillageInfo[ pclInfo->clStockBidVillageInfo[ i ].siVillageUnique_ ].Set(
				&pclInfo->clStockBidVillageInfo[ i ] );
		}
	}

	
	pclStockManager->UpdateStockBidVillageSwitch = TRUE;

	if( count > 0 ) 
	{
		pclStockManager->bBidding = TRUE;
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKBIDDERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockBidder* pclInfo = (cltGameMsgResponse_StockBidder *)pclMsg->cData;
	
	int count = pclInfo->usCount;

	pclStockManager->InitClientStockBidder();

	for( int i = 0; i < count; ++i ) {

		pclStockManager->clClientStockBidder[ i ].Set( &pclInfo->clStockBidder[ i ] );
	}

	pclStockManager->UpdateStockBidderSwitch = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKDISTRIBUTIONSET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockDistributionSet *pclinfo = (cltGameMsgResponse_StockDistributionSet *)pclMsg->cData;

	if ( pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL )
	{
		return;
	}

	if( m_pDialog[ NSTOCK_DISTRIBUTION_DLG ] == NULL )
		CreateInterface( NSTOCK_DISTRIBUTION_DLG );

	((CNStockDistributionDlg*)(m_pDialog[ NSTOCK_DISTRIBUTION_DLG ]))->Set( pclinfo->siVillageUnique, pclinfo->siEachPersonAmount, pclinfo->siStockPrice, pclinfo->bVillageWarStockDistribution );
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKDISTRIBUTIONORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockDistributionOrder *pclinfo = (cltGameMsgResponse_StockDistributionOrder*)pclMsg->cData;

	if ( pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL )
	{
		return;
	}

	TCHAR *VillageName = pclVillageManager->GetName(pclinfo->siVillageUnique);

	if ( VillageName == NULL )
	{
		return;
	}	
	
	TCHAR Buffer[ 512 ] = TEXT("");
	TCHAR* pTitle = GetTxtFromMgr(850);
	TCHAR* pText  = GetTxtFromMgr(849);

	

	StringCchPrintf( Buffer, 512, pText, VillageName, pclinfo->siAmount, pclinfo->siAmount * pclinfo->siPrice );
	
	m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, Buffer );

	return;
}


// 여각에 올려진 사자, 팔자 주문의 정보 통보. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKTRADEINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockTradeInfo *pclinfo = (cltGameMsgResponse_StockTradeInfo*)pclMsg->cData;

	
	// 자막에서 처리한다.
	TCHAR buffer[1024];
	TCHAR temp[256];
	TCHAR* pvillagename = pclVillageManager->GetName(pclinfo->siVillageUnique);
	if(pvillagename == NULL)return ;


	StringCchPrintf(buffer, 1024, TEXT("[%s]"), pvillagename);

	if(pclinfo->siSellPrice)
	{
		TCHAR* pText  = GetTxtFromMgr(851);

		

		StringCchPrintf(temp, 256, pText, pclinfo->siSellAmount,pclinfo->siSellPrice);

		StringCchCat(buffer, 1024, temp);
	}

	if(pclinfo->siBuyPrice)
	{
		TCHAR* pText  = GetTxtFromMgr(852);

		

        StringCchPrintf(temp, 256, pText, pclinfo->siBuyAmount, pclinfo->siBuyPrice);

		StringCchCat(buffer, 1024, temp);

	}

	g_PromptMgr.Set(buffer, TEXT(""), 1, RGB(250, 250, 250));
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_CANCEL_STOCKORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_CancelStockOrder *pclinfo = (cltGameMsgResponse_CancelStockOrder*)pclMsg->cData;

	if(m_pDialog[ NOTIFYMSG_DLG ]==NULL)
	{
		CreateInterface( NOTIFYMSG_DLG );
	}

	TCHAR *pTitle = GetTxtFromMgr(5614);
	TCHAR *pText = GetTxtFromMgr(6082);
	m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_EXCHANGESTOCK(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ExchangeStock *pclinfo = (cltGameMsgResponse_ExchangeStock*)pclMsg->cData;

	if( true == pclinfo->bResult )
	{
		TCHAR *pTitle = GetTxtFromMgr(10277);
		TCHAR *pText = GetTxtFromMgr(10290);

		TCHAR buffer[128] = {'\0'};
		TCHAR amount[32] = {'\0'};

		TCHAR* villageName = pclClient->pclVillageManager->GetName( pclinfo->siVillageUnique );
		StringCchPrintf( amount, 32, "%d", pclinfo->siSellAmount );

		Nsprintf( buffer, pText, TEXT("village"), villageName, TEXT("amount"), amount, NULL );

		if( m_pDialog[ NOTIFYMSG_DLG ] == NULL )
		{
			CreateInterface( NOTIFYMSG_DLG );
		}

		m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);

		if( m_pDialog[ NEXCHANGESTOCK_DLG ] )
		{
			((CNExchangStockDlg*)m_pDialog[ NEXCHANGESTOCK_DLG ])->SetStockAmount( pclinfo->siLeftAmount );
		}
	}
	else
	{
		TCHAR *pTitle = GetTxtFromMgr(10277);
		TCHAR *pText = GetTxtFromMgr(7549);

		if( m_pDialog[ NOTIFYMSG_DLG ] == NULL )
		{
			CreateInterface( NOTIFYMSG_DLG );
		}

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

	}

	
}
