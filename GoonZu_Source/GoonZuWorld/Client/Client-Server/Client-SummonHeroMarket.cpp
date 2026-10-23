#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-SummonHeroMarket.h"
#include "..\CommonLogic\SummonHeroMarket\SummonHeroMarket.h"

#include "Char\CharManager\CharManager.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NCharStatus/NCharStatus.h"
#include "../NInterface/NCharStatus/NCharSummon.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonHeroMarketCancelOrder *pclInfo = (cltGameMsgResponse_SummonHeroMarketCancelOrder *)pclMsg->cData;

	pclSummonHeroMarketManager->DelMyOrder( pclInfo->siOrderIndex );

	pclSummonHeroMarketManager->bShowMyOrderSwitch = TRUE;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SetSummon( pclInfo->clSummon.siSummonID, &pclInfo->clSummon );

	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_MYORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonHeroMarketMyOrder *pclInfo = (cltGameMsgResponse_SummonHeroMarketMyOrder *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclSummonHeroMarketManager->clMyOrder.ZeroMem();
#else
	ZeroMemory( pclSummonHeroMarketManager->clMyOrder, sizeof( pclSummonHeroMarketManager->clMyOrder ) );
#endif

	for( int i = 0; i < pclInfo->usCount; ++i ) {
		pclSummonHeroMarketManager->clMyOrder[ i ].Set( &pclInfo->clMyOrder[ i ] );
	}

	pclSummonHeroMarketManager->bShowMyOrderSwitch = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonHeroMarketOrderList *pclinfo = (cltGameMsgResponse_SummonHeroMarketOrderList *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclSummonHeroMarketManager->clResponseOrder.ZeroMem();
#else
	ZeroMemory( pclSummonHeroMarketManager->clResponseOrder, sizeof( pclSummonHeroMarketManager->clResponseOrder ) );
#endif

	for( int i = 0; i < pclinfo->usCount; ++i ) {
		pclSummonHeroMarketManager->clResponseOrder[ i ].Set( &pclinfo->clOrder[ i ] );
	}

	pclSummonHeroMarketManager->m_siStartPos = pclinfo->siNextStartPos;
	pclSummonHeroMarketManager->siTotalResponseOrderNumber = pclinfo->usCount;
	pclSummonHeroMarketManager->bShowResponseOrderSwitch = TRUE;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_SELL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonHeroMarketSell *pclInfo = (cltGameMsgResponse_SummonHeroMarketSell *)pclMsg->cData;

	pclSummonHeroMarketManager->AddMyOrder( &pclInfo->clSummonOrder );

	pclSummonHeroMarketManager->bShowMyOrderSwitch = TRUE;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SummonInit( pclInfo->clSummonOrder.clSummon.siSummonID );

	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_BUY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonHeroMarketBuy *pclInfo = (cltGameMsgResponse_SummonHeroMarketBuy *)pclMsg->cData;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SetSummon( pclInfo->clSummon.siSummonID, &pclInfo->clSummon );

	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}

	pclSummonHeroMarketManager->bShowMyOrderSwitch = TRUE;
}
