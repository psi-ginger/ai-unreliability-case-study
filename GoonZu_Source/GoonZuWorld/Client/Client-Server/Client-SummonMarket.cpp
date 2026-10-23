#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-SummonMarket.h"
#include "..\CommonLogic\SummonMarket\SummonMarket.h"

#include "Char\CharManager\CharManager.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NCharStatus/NCharStatus.h"
#include "../NInterface/NCharStatus/NCharSummon.h"

#include "../Client/NStructureDlg/summonmarket/NSummonMarketDlg.h"



void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonMarketCancelOrder *pclInfo = (cltGameMsgResponse_SummonMarketCancelOrder *)pclMsg->cData;

	pclSummonMarketManager->DelMyOrder( pclInfo->siOrderIndex );

	pclSummonMarketManager->bShowMyOrderSwitch = TRUE;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SetSummon( pclInfo->clSummon.siSummonID, &pclInfo->clSummon );

	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_MYORDER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonMarketMyOrder *pclInfo = (cltGameMsgResponse_SummonMarketMyOrder *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclSummonMarketManager->clMyOrder.ZeroMem();
#else
	ZeroMemory( pclSummonMarketManager->clMyOrder, sizeof( pclSummonMarketManager->clMyOrder ) );
#endif

	for( int i = 0; i < pclInfo->usCount; ++i ) {
		pclSummonMarketManager->clMyOrder[ i ].Set( &pclInfo->clMyOrder[ i ] );
	}

	pclSummonMarketManager->bShowMyOrderSwitch = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonMarketOrderList *pclinfo = (cltGameMsgResponse_SummonMarketOrderList *)pclMsg->cData;

#ifdef _SAFE_MEMORY
	pclSummonMarketManager->clResponseOrder.ZeroMem();
#else
	ZeroMemory( pclSummonMarketManager->clResponseOrder, sizeof( pclSummonMarketManager->clResponseOrder ) );
#endif

	for( int i = 0; i < pclinfo->usCount; ++i ) 
	{
		pclSummonMarketManager->clResponseOrder[ i ].Set( &pclinfo->clOrder[ i ] );
	}

	pclSummonMarketManager->m_siCurrentPage				= pclinfo->siCurrentPage;
	pclSummonMarketManager->m_siTotalPageNum			= pclinfo->usTotalPage;
	pclSummonMarketManager->siTotalResponseOrderNumber	= pclinfo->usCount;
	pclSummonMarketManager->bShowResponseOrderSwitch	= TRUE;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_SELL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonMarketSell *pclInfo = (cltGameMsgResponse_SummonMarketSell *)pclMsg->cData;

	pclSummonMarketManager->AddMyOrder( &pclInfo->clSummonOrder );

	pclSummonMarketManager->bShowMyOrderSwitch = TRUE;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SummonInit( pclInfo->clSummonOrder.clSummon.siSummonID );

	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_BUY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonMarketBuy *pclInfo = (cltGameMsgResponse_SummonMarketBuy *)pclMsg->cData;

	pclCM->CR[ 1 ]->pclCI->clSummonInfo.SetSummon( pclInfo->clSummon.siSummonID, &pclInfo->clSummon );
	
	if( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ CLTNCHARSTATUS_DLG ] && ((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
	{
		((cltNCharStatus*)(m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->m_bChangedInitSummonStatus = TRUE;        
	}
	if(m_pDialog[ NSUMMONMARKET_DLG ])
	{
		CControllerMgr* pChild = ((NSummonMarketDlg*)(m_pDialog[ NSUMMONMARKET_DLG ]))->GetChildDialog(1);
		((NSummonMarketBuyDlg*)pChild)->Refresh();
	}
	
	pclSummonMarketManager->bShowMyOrderSwitch = TRUE;
}
