#pragma once

#include <Stdio.h>
#include "CommonLogic.h"
#include "../StructureClass/StructureClass.h"
#include "../Account/Account.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"
#include "../../Common/Item/ItemCommon/cltitem.h"

extern cltCommonLogic* pclClient;


class cltNewMarket : public cltStructureBasic
{
public:

	cltNewMarket(SI32 sivillageunique) : cltStructureBasic(sivillageunique){}
	~cltNewMarket(){}
};


class cltNewMarketTradeOrder
{
public:
	cltNewMarketTradeOrder() { ZeroMemory( this, sizeof(*this) ); }
	~cltNewMarketTradeOrder(){}

public:
	UI08		GetServerUnique( void )	{ return m_siServerUnique;													}
	GMONEY		GetMoney( void )		{ return m_siPrice;													}
	GMONEY		GetTotalMoney( void )	{ return m_siPrice * m_clItem.siItemNum;							}
	SI16		GetItemUnique( void )	{ return m_clItem.siUnique;											}
	SI16		GetItemNum( void )		{ return m_clItem.siItemNum;										}
	SI32		GetItemType( void )		{ return pclClient->pclItemManager->GetItemType(m_clItem.siUnique);	}
	SI32		GetDateVary( void )		{ return m_siDateVary;												}
	
	UI08		m_siServerUnique;		// 서버 유니크.
	SI32		m_siOrderIndex;			// 주문 고유의 식별자
	SI32		m_siPersonID;			// 주문을 낸 PersonID
	SI32		m_siDateVary;			// 시간 정보.(주문시간으로 처리하는 것덜이 있어.)		
	GMONEY		m_siPrice;				// 주문 개당 가격. 
	cltItem		m_clItem;				// 아이템 정보.
};

class NewMarketTradeOrderManager
{
public:



};