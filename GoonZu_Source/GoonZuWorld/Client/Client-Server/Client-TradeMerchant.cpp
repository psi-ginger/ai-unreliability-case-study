#include "../Client.h"
#include "../Music\Music.h"
#include "SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"
#include "..\..\CommonLogic\MsgType.h"
#include "..\..\CommonLogic\Msg\MsgType-TradeMerchant.h"
#include "..\..\CommonLogic\Msg\MsgType-Quest.h"
#include "..\..\CommonLogic\MsgType-Structure.h"
#include "SystemNPC\SystemNPC.h"

#include "..\Interface\TradeMerchantDlg\TradeMerchantDlg.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../Interface/TradeMerchantDish/TradeMerchantDish.h"
#include "../Interface/TradeMerchantHorse/TradeMerchantHorse.h"
#include "../Client/NInterface/NTradeMerchantHorseDlg/NTradeMerchantHorseDlg.h"
#include "../client/NInterface/NTradeMerchant/NTradeMerchant.h"
#include "../Client/NInterface/NTradeMerchantHighClassDlg/NTradeMerchantHighClassDlg.h"
#include "../Client/NInterface/NSummonUnitBuyNpcDlg/NSummonUnitBuyNpcDlg.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuyInfo *pclInfo = (cltGameMsgResponse_TradeMerchantBuyInfo*)pclMsg->cData;

	if(g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if( ((CNTradeMerchantDlg*)m_pDialog[ NTRADEMERCHANT_DLG ]) )
			((CNTradeMerchantDlg*)m_pDialog[ NTRADEMERCHANT_DLG ])->Set( &pclInfo->cTradeMerchantNPC );
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuyItemFromPerson *pclInfo = (cltGameMsgResponse_TradeMerchantBuyItemFromPerson*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siItemPos ].Set( &pclInfo->clItem );
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftMoney );

	if(g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if( ((CNTradeMerchantDlg*)m_pDialog[ NTRADEMERCHANT_DLG ]) )
			((CNTradeMerchantDlg*)m_pDialog[ NTRADEMERCHANT_DLG ])->SellToNPCSuccess();
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTHORSEBUYINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantHorseBuyInfo *pclInfo = (cltGameMsgResponse_TradeMerchantHorseBuyInfo*)pclMsg->cData;

	if(g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if(m_pDialog[ NTRADEMERCHANTHORSE_DLG ])
		{
			stNTradeMerchantHorseData Data;
			Data.siFirst = pclInfo->siHorseUnique;
			Data.siSecond = pclInfo->siHorsePrice;
			Data.siParam = 0;
			m_pDialog[ NTRADEMERCHANTHORSE_DLG ]->SetData(&Data); 
		}
	}
	

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuyHorseFromPerson *pclInfo = (cltGameMsgResponse_TradeMerchantBuyHorseFromPerson*)pclMsg->cData;

	if(g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if(m_pDialog[ NTRADEMERCHANTHORSE_DLG ])
		{
			stNTradeMerchantHorseData Data;
			Data.siParam = 1;
			m_pDialog[ NTRADEMERCHANTHORSE_DLG ]->SetData(&Data);
		}
	}
	
    
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftPersonMoney );
	pclCM->CR[1]->UnrideHorse();
	return;
}

// 상인이 아이템을 구입하는 정보
void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuyItemInfo *pclInfo = (cltGameMsgResponse_TradeMerchantBuyItemInfo*)pclMsg->cData;

	if ( pclInfo->siMerchantKind == GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")) )
	{
		if(g_SoundOnOffDlg.m_bFullScreenMode)
		{
			if( ((CNTradeMerchantHighClassDlg*)m_pDialog[ NTRADEMERCHANTHIGHCLASS_DLG ] ) )
				((CNTradeMerchantHighClassDlg*)m_pDialog[ NTRADEMERCHANTHIGHCLASS_DLG ] )->SetDishPrice( pclInfo->siItemUnique, pclInfo->siBuyPrice );
		}
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_TradeMerchantBuyItem *pclInfo = (cltGameMsgResponse_TradeMerchantBuyItem*)pclMsg->cData;

	if ( pclInfo->siMerchantKind == GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")) )
	{
		pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siItemPos ].Set( &pclInfo->clItem );
		pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftPersonMoney );

		if(g_SoundOnOffDlg.m_bFullScreenMode)
		{
			if( ((CNTradeMerchantHighClassDlg*)m_pDialog[ NTRADEMERCHANTHIGHCLASS_DLG ] ) )
				((CNTradeMerchantHighClassDlg*)m_pDialog[ NTRADEMERCHANTHIGHCLASS_DLG ] )->SellDish( pclInfo->siItemUnique );
		}
	}

	return;
}
