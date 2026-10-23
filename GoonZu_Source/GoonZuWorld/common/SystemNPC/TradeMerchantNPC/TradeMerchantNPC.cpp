#include "TradeMerchantNPC.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-TradeMerchant.h"

#include "..\..\..\CommonLogic\CommonLogic.h"
#include "..\..\..\Server\Server.h"

extern cltCommonLogic *pclClient;

CTradeMerchantNPCMgr::CTradeMerchantNPCMgr()
{
	for( SI16 i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		m_pTradeMerchantNPC[ i ] = NULL;
	}
	
	m_siCurrentDay = 0;

	
	//Set( false, pclClient->GetUniqueFromHash(TEXT("KIND_CHINATRADER")), 0, 0, 0 ,0);
	Set( false, pclClient->GetUniqueFromHash( TEXT("KIND_JAPANTRADER")), 0, 0, 0 ,0);
	//Set( false, pclClient->GetUniqueFromHash( TEXT("KIND_RUSSIATRADER")), 0, 0, 0 ,0);
}

CTradeMerchantNPCMgr::~CTradeMerchantNPCMgr()
{
	for( SI16 i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		if ( m_pTradeMerchantNPC[ i ] != NULL )
		{
			delete m_pTradeMerchantNPC[ i ];
			m_pTradeMerchantNPC[ i ] = NULL;
		}
	}
}

void CTradeMerchantNPCMgr::Init()
{
	return;
}

bool CTradeMerchantNPCMgr::Set( bool TradeMerchantSet, SI32 TradeMerchantKind, SI16 BuyItemUnique, GMONEY BuyItemPrice, GMONEY BuyTotalMoney ,GMONEY TradeTax)
{
	SI16 i = 0;
	for( i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		if ( m_pTradeMerchantNPC[ i ] != NULL )
		{
			if ( m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind == TradeMerchantKind )
			{
				m_pTradeMerchantNPC[ i ]->m_siBuyItemUnique = BuyItemUnique;
				m_pTradeMerchantNPC[ i ]->m_siBuyItemPrice = BuyItemPrice;
				m_pTradeMerchantNPC[ i ]->m_siBuyTotalMoney = BuyTotalMoney;
				m_pTradeMerchantNPC[ i ]->m_siTradeTax = TradeTax;
				m_pTradeMerchantNPC[ i ]->m_bTradeMerchantSet = true;
			
				return true;
			}
		}
	}

	for ( i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		if ( m_pTradeMerchantNPC[ i ] == NULL )
		{
			m_pTradeMerchantNPC[ i ] = new CTradeMerchantNPC();
			m_pTradeMerchantNPC[ i ]->Set( TradeMerchantSet, TradeMerchantKind, BuyItemUnique, BuyItemPrice, BuyTotalMoney, TradeTax);
			
			return true;
		}
	}
	
	return false;
}

void CTradeMerchantNPCMgr::Action( WORD CurrentDay )
{
	SI16 ItemUnique;
	SI16 ItemNum;
	GMONEY ItemPrice;

	cltServer *pclServer = (cltServer *)pclClient;
	SI16 i = 0;
	
	// 하루에 한번만 호출 가능
	if ( m_siCurrentDay == CurrentDay )
	{
		// 시간내에 다 팔린 경우에는, 재설정
		for( i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
		{
			if ( m_pTradeMerchantNPC[ i ] != NULL && 
				m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind > 0 &&
				m_pTradeMerchantNPC[ i ]->m_bTradeMerchantSet == true &&
				m_pTradeMerchantNPC[ i ]->m_siBuyItemPrice > 0 &&
				m_pTradeMerchantNPC[ i ]->m_siBuyTotalMoney/m_pTradeMerchantNPC[ i ]->m_siBuyItemPrice == 0
				)
			{
				if ( pclClient->pclItemManager->GetTraderItem( &ItemUnique, &ItemNum, &ItemPrice ) )
				{
					sDBRequest_TradeMerchantSet clMsg( m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind, MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY, ItemUnique, ItemPrice );
					pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				}
			}
		}

		return;
	}

	m_siCurrentDay = CurrentDay;
	
	for( i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		if ( m_pTradeMerchantNPC[ i ] != NULL && m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind > 0 )
		{
			if ( pclClient->pclItemManager->GetTraderItem( &ItemUnique, &ItemNum, &ItemPrice ) )
			{
				sDBRequest_TradeMerchantSet clMsg( m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind, MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY, ItemUnique, ItemPrice );
				pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
	}
	
	return;
}

CTradeMerchantNPC *CTradeMerchantNPCMgr::GetTradeMerchantNPCPtr( SI32 NPCKind )
{
	SI16 i = 0;
	for( i = 0; i < MAX_TRADEMERCHANTNPC_NUM; ++i )
	{
		if ( m_pTradeMerchantNPC[ i ] != NULL && m_pTradeMerchantNPC[ i ]->m_siTradeMerchantNPCKind == NPCKind )
		{
			return m_pTradeMerchantNPC[ i ];
		}
	}

	return NULL;
}