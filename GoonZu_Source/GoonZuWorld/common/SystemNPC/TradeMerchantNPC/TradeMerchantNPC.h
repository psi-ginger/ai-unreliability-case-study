#ifndef _TRADEMERCHANTNPC_H_
#define _TRADEMERCHANTNPC_H_

#include <Directives.h>
#include "CommonLogic.h"

#define MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY		100000000

#define TRADEMERCHATNPC_BUY_START_TIME			18			// 18시
#define TRADEMERCHATNPC_BUY_END_TIME			21			// 21시

#define MAX_TRADEMERCHANTNPC_NUM				10			// 무역 상인의 최대수

class CTradeMerchantNPC
{
public:
	bool m_bTradeMerchantSet;

	SI32 m_siTradeMerchantNPCKind;

	SI32 m_siBuyItemUnique;
	GMONEY m_siBuyItemPrice;

	GMONEY m_siBuyTotalMoney;
	GMONEY m_siTradeTax; // 관세

	CTradeMerchantNPC()
	{
		Init();
	}

	~CTradeMerchantNPC()
	{
	}

	void Init()
	{
		memset( this, 0, sizeof( CTradeMerchantNPC ) );
	}

	void Set( CTradeMerchantNPC *pTradeMerchantNPC )
	{
		memcpy( this, pTradeMerchantNPC, sizeof( CTradeMerchantNPC ) );
	}

	void Set( bool TradeMerchantSet, SI32 TradeMerchatNPCKind, SI32 BuyItemUnique,
			  GMONEY BuyItemPrice, GMONEY BuyTotalMoney , GMONEY TradeTax)
	{
		m_bTradeMerchantSet = TradeMerchantSet;
		m_siTradeMerchantNPCKind = TradeMerchatNPCKind;
		m_siBuyItemUnique = BuyItemUnique;
		m_siBuyItemPrice = BuyItemPrice;
		m_siBuyTotalMoney = BuyTotalMoney;
		m_siTradeTax = TradeTax ;
	}

	void ChangeBuyItemInfo( SI16 ItemUnqiue, GMONEY ItemPrice )
	{
		m_siBuyItemUnique = ItemUnqiue;
		m_siBuyItemPrice = ItemPrice;
	}

	void ChangeBuyTotalMoney( GMONEY BuyTotalMoney )
	{
		m_siBuyTotalMoney = BuyTotalMoney;
	}
};

class CTradeMerchantNPCMgr
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CTradeMerchantNPC *, MAX_TRADEMERCHANTNPC_NUM>		m_pTradeMerchantNPC;
#else
	CTradeMerchantNPC *m_pTradeMerchantNPC[ MAX_TRADEMERCHANTNPC_NUM ];
#endif

	CTradeMerchantNPCMgr();
	~CTradeMerchantNPCMgr();

	void Init();

	bool Set( bool TradeMerchantSet, SI32 TradeMerchantKind, SI16 BuyItemUnique, GMONEY BuyItemPrice, GMONEY BuyTotalMoney,GMONEY TradeTax);

	void Action( WORD CurrentDay );
	
	CTradeMerchantNPC *GetTradeMerchantNPCPtr( SI32 NPCKind );
	
	WORD m_siCurrentDay;

};

#endif
