#include "PrivateTradeOrderObj.h"
#include "PrivateTradeOrderBaseInfo.h"

#include "../Char/CharCommon/Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "..\CommonLogic\CommonLogic.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic *pclClient;

CEachPersonTradeOrderInfo::CEachPersonTradeOrderInfo()
{
	Initialize();
}

CEachPersonTradeOrderInfo::~CEachPersonTradeOrderInfo()
{
	Destroy();
}

void CEachPersonTradeOrderInfo::Initialize()
{
	memset( this, 0, sizeof( CEachPersonTradeOrderInfo ) );
	
	Person1TradeData.m_siHorseIndex  = -1;
	Person2TradeData.m_siHorseIndex  = -1;

	return;
}

void CEachPersonTradeOrderInfo::Destroy()
{
	return;
}

void CEachPersonTradeOrderInfo::Set( SI32 siPerson1ID, SI32 siPerson2ID )
{
	Person1TradeData.m_siPersonID = siPerson1ID;
	Person2TradeData.m_siPersonID = siPerson2ID;
	return;
}

void CEachPersonTradeOrderInfo::SetPerson1TradePrice( GMONEY siPrice )
{
	if ( siPrice < 1 || siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") )
	{
		return;
	}

	Person1TradeData.m_siPrice = siPrice;

	return;
}

void CEachPersonTradeOrderInfo::SetPerson2TradePrice( GMONEY siPrice )
{
	if ( siPrice < 1 || siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") )
	{
		return;
	}

	Person2TradeData.m_siPrice = siPrice;

	return;
}

void CEachPersonTradeOrderInfo::AddPerson1TradeItem( cltItem *pclItem )
{
	if ( pclItem->siUnique == 0 || pclItem->siItemNum < 1 ) 
	{
		return;	
	}

	for ( SI32 i = 0 ; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( Person1TradeData.m_clItem[ i ].siUnique == 0 )		// 아이템이 아무것도 없는 상태라면...
		{
			Person1TradeData.m_clItem[ i ].Set( pclItem );
			return;
		}
	}

#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("MAX_PRIVATETRADEORDER_ITEM_NUM 만큼 거래 아이템이 꽉찬 상태"), TEXT("더 추가 못함") );
#endif

	return;
}

void CEachPersonTradeOrderInfo::AddPerson2TradeItem( cltItem *pclItem )
{
	if ( pclItem->siUnique == 0 || pclItem->siItemNum < 1 ) 
	{
		return;	
	}

	for ( SI32 i = 0 ; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( Person2TradeData.m_clItem[ i ].siUnique == 0 )		// 아이템이 아무것도 없는 상태라면...
		{
			Person2TradeData.m_clItem[ i ].Set( pclItem );
			return;
		}
	}

#ifdef _DEBUG
	if (pclClient->GameMode == GAMEMODE_SERVER)
		pclClient->pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("MAX_PRIVATETRADEORDER_ITEM_NUM 만큼 거래 아이템이 꽉찬 상태"), TEXT("더 추가 못함") );
#endif

	return;
}

SI32 CEachPersonTradeOrderInfo::GetPerson1ID()
{
	return Person1TradeData.m_siPersonID;
}

SI32 CEachPersonTradeOrderInfo::GetPerson2ID()
{
	return Person2TradeData.m_siPersonID;
}

void CEachPersonTradeOrderInfo::GetPersonID( SI32 *pPerson1ID, SI32 *pPerson2ID )
{
	*pPerson1ID = Person1TradeData.m_siPersonID;
	*pPerson2ID = Person2TradeData.m_siPersonID;

	return;
}

bool CEachPersonTradeOrderInfo::IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID )
{
	if ( Person1TradeData.m_siPersonID == siPerson1ID ||
		Person1TradeData.m_siPersonID == siPerson2ID ||
		Person2TradeData.m_siPersonID == siPerson1ID ||
		Person2TradeData.m_siPersonID == siPerson2ID )
	{
		return true;
	}

	return false;
}

stTradeData *CEachPersonTradeOrderInfo::GetTradeData( SI32 siPersonID )
{
	if ( Person1TradeData.m_siPersonID == siPersonID )
	{
		return &Person1TradeData;
	}
	else
	{
		return &Person2TradeData;
	}
}

stTradeData *CEachPersonTradeOrderInfo::GetPerson1TradeData()
{
	return &Person1TradeData;
}

stTradeData *CEachPersonTradeOrderInfo::GetPerson2TradeData()
{
	return &Person2TradeData;
}





CPrivateTradeOrderObj::CPrivateTradeOrderObj( SI16 siManagerIndex )
{
	m_pEachPersonTradeOrderInfo = new CEachPersonTradeOrderInfo();

	m_siMgrIndex = siManagerIndex;
	m_siCountIndex = 0;

	m_siAcceptCounter = 0;

	m_siSelectMode = PRIVATETRADE_TRADE_MODE;

	Initialize();
}

CPrivateTradeOrderObj::~CPrivateTradeOrderObj()
{
	Destroy();

	if ( m_pEachPersonTradeOrderInfo )
	{
		delete m_pEachPersonTradeOrderInfo;
		m_pEachPersonTradeOrderInfo = NULL;
	}

	m_siAcceptCounter = 0;
}

void CPrivateTradeOrderObj::Initialize()
{
	if ( m_pEachPersonTradeOrderInfo )
	{
		m_pEachPersonTradeOrderInfo->Initialize();
		
		++m_siCountIndex;

		if ( m_siCountIndex > 30000 )		// SI16 표현 가능 값 
		{
			m_siCountIndex = 0;
		}
	}

	m_siAcceptCounter = 0;

	m_siSelectMode = PRIVATETRADE_TRADE_MODE;

	return;
}

void CPrivateTradeOrderObj::Destroy()
{

	return;
}

void CPrivateTradeOrderObj::Set( SI32 siPerson1ID, SI32 siPerson2ID, SI16 *psiMgrIndex, SI16 *psiCountIndex, SI16 TradeMode )
{
	m_pEachPersonTradeOrderInfo->Set( siPerson1ID, siPerson2ID );
	
	*psiMgrIndex = m_siMgrIndex;
	*psiCountIndex = m_siCountIndex;

	m_siSelectMode = TradeMode;

	return;
}

// 유저에게 할당된 다이얼로그가 열린 상황에서 다른 유저에게 거래 신청을 했을때, 거래가 다른 것임을 인지하기 위해서...
// 아주 드문 경우이지만, 이런 일이 발생할 경우 할당을 다른걸 해야지...
bool CPrivateTradeOrderObj::IsSame( SI16 siManagerIndex, SI16 siCountIndex )
{
	if ( m_siMgrIndex == siManagerIndex && m_siCountIndex == siCountIndex )
	{
		return true;
	}

	return false;
}

bool CPrivateTradeOrderObj::IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID )
{
	return m_pEachPersonTradeOrderInfo->IsTradeStatus( siPerson1ID, siPerson2ID );
}

bool CPrivateTradeOrderObj::IsExist( SI32 siPerson1ID )
{
	if ( m_pEachPersonTradeOrderInfo->GetPerson1ID() == siPerson1ID )
	{
		return true;
	}

	if ( m_pEachPersonTradeOrderInfo->GetPerson2ID() == siPerson1ID )
	{
		return true;
	}

	return false;
}

SI16 CPrivateTradeOrderObj::AddItem( SI32 siPersonID, cltItem *pclItem, SI16 siItemPos )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	// 인벤토리 같은 곳에꺼 또 올리려고 그러네...
	for ( SI16 i = 0 ; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_siSelectedInventoryPos[ i ] == siItemPos )
		{
			return -1;
		}
	}

	// for ( i = 0 ; i < 1; ++i )
	for ( i = 0 ; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique == 0 )		// 아이템이 아무것도 없는 상태라면...
		{
			pTradeData->m_clItem[ i ].Set( pclItem );		// 아이템 추가
			pTradeData->m_siSelectedInventoryPos[ i ]  = siItemPos;
			return i;
		}
	}

	return -1;			// 아이템 더 이상 추가할수 없음.
}

bool CPrivateTradeOrderObj::PrivateTradeSetPrice( SI32 siPersonID, GMONEY siPrice )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	
	// 돈 입력은 한번만 가능함으로.
	if ( pTradeData->m_siPrice > 0 )
	{
		return false;
	}

	pTradeData->m_siPrice = siPrice;

	return true;
}

void CPrivateTradeOrderObj::SetStock( SI32 siPersonID, SI32 siVillageUnique, SI32 siStockNum )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	pTradeData->SetStock( siVillageUnique, siStockNum );
	return;
}

void CPrivateTradeOrderObj::SetHouseUnit( SI32 siPersonID, SI32 HouseVillageUnique, SI32 HouseUnitSlot )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	pTradeData->SetHouseUnit( HouseVillageUnique, HouseUnitSlot );
	return;
}

void CPrivateTradeOrderObj::SetHorse( SI32 siPersonID, SI16 siHorseIndex, cltHorse *pclHorse )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	pTradeData->SetHorse( siHorseIndex, pclHorse );
	return;
}

bool CPrivateTradeOrderObj::GetOrderIndexItem( SI32 siPersonID, SI16 siOrderListIndex, cltItem *pclItem )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	if ( pTradeData->m_clItem[ siOrderListIndex ].siUnique < 1 )
	{
		return false;
	}

	pclItem->Set( &pTradeData->m_clItem[ siOrderListIndex ] );

	return true;
}

bool CPrivateTradeOrderObj::DelOrderListIndex( SI32 PersonID, SI16 DelType, SI16 Param )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( PersonID );

	switch( DelType )
	{
	case PRIVATETRADE_TYPE_MONEY:
		{
			return false;
			// 과거에는 돈은 지울 수 있었지만.. 이제는 지울 수 없음.
			//pTradeData->m_siPrice = 0;
		}
		break;
	case PRIVATETRADE_TYPE_ITEM:
		{
			memset( &pTradeData->m_clItem[ Param ], 0, sizeof( cltItem ) );
			pTradeData->m_siSelectedInventoryPos[ Param ] = 0;
		}
		break;
	case PRIVATETRADE_TYPE_STOCK:
		{
			pTradeData->m_siStockVillageUnique = 0;
			pTradeData->m_siStockNum = 0;
		}
		break;
	case PRIVATETRADE_TYPE_HOUSEUNIT:
		{
			pTradeData->m_siHouseVillageUnique = 0;
			pTradeData->m_siHouseUnitSlot = 0;
			pTradeData->m_clRentContract.Init();
		}
		break;
	case PRIVATETRADE_TYPE_HORSE:
		{
			pTradeData->m_siHorseIndex = -1;
			pTradeData->m_clTradeHorse.Init();
		}
		break;
	}

	return true;
}

void CPrivateTradeOrderObj::DelOrderIndexItem( SI32 siPersonID, SI16 siOrderListIndex )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	memset( &pTradeData->m_clItem[ siOrderListIndex ], 0, sizeof( cltItem ) );
	pTradeData->m_siSelectedInventoryPos[ siOrderListIndex ] = 0;
	
	return;
}

void CPrivateTradeOrderObj::SetMyAccept( SI32 siPersonID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	pTradeData->m_bMyAccept = true;
	
	m_uiAcceptFrame = pclClient->GetFrame();
	
	return;
}

void CPrivateTradeOrderObj::SetMyCancel( SI32 siPersonID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );
	pTradeData->m_bMyAccept = false;
	return;
}

bool CPrivateTradeOrderObj::IsAllAccept()
{
	stTradeData *pTradeData1 = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	stTradeData *pTradeData2 = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();

	if ( pTradeData1->m_bMyAccept && pTradeData2->m_bMyAccept )
	{
		return true;
	}

	return false;
}

bool CPrivateTradeOrderObj::IsAcceptStatus( SI32 siPersonID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	return pTradeData->m_bMyAccept;
}

SI16 CPrivateTradeOrderObj::GetOrderIndexItemInventoryPos( SI32 siPersonID, SI32 siOrderIndex )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	return pTradeData->m_siSelectedInventoryPos[ siOrderIndex ];
}

void CPrivateTradeOrderObj::GetOrderItemList( SI32 siPersonID, cltItem *pclItemList, SI16 *psiItemPosList )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

#ifdef _SAFE_MEMORY
	SI32 i= 0;
	for(i=0; i<MAX_PRIVATETRADEORDER_ITEM_NUM; i++)			pclItemList[i].Set(&pTradeData->m_clItem[i]);
	for(i=0; i<MAX_PRIVATETRADEORDER_ITEM_NUM; i++)			psiItemPosList[i] = pTradeData->m_siSelectedInventoryPos[i];
#else
	memcpy( pclItemList, pTradeData->m_clItem, sizeof(cltItem) * MAX_PRIVATETRADEORDER_ITEM_NUM );
	memcpy( psiItemPosList, pTradeData->m_siSelectedInventoryPos, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
#endif

	return;
}

bool CPrivateTradeOrderObj::IsAvailableMoney( SI32 siPersonID, GMONEY Price )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	if ( pTradeData->m_siPrice <= Price )
	{
		return true;
	}

	return false;
}

bool CPrivateTradeOrderObj::IsAvailableStock( SI32 siPersonID, SI32 siCharID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	// 현재 가진 주식보다 많이 거래 할려고 한다. ( 이 경우는 중간에 여각에서 주식이 팔렸거나 하면 그럴수 있지... )
	if ( pTradeData->m_siStockNum > pclClient->pclCM->CR[siCharID]->pclCI->clStock.clInfo.GetStockAmount( pTradeData->m_siStockVillageUnique ) )
	{
		return false;
	}

	return true;
}

bool CPrivateTradeOrderObj::IsAvailableHorse( SI32 siPersonID, SI32 siCharID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	// 말 거래 안하므로 신경 안써도 됨.
	if ( pTradeData->m_siHorseIndex == -1 )
	{
		return true;
	}

	BYTE *pTradeHorse	= (BYTE*)&pTradeData->m_clTradeHorse;
	BYTE *pCurrentHorse = (BYTE*)pclClient->pclCM->CR[siCharID]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if( pCurrentHorse == NULL )
	{
		return false;
	}

	SI32 HorseSize = sizeof( cltHorse );
	for ( SI32 i = 0; i < HorseSize; ++i )
	{
		if ( pTradeHorse[i] != pCurrentHorse[i] )
		{
			return false;
		}
	}	

	return true;
}

void CPrivateTradeOrderObj::IncreaseAcceptCounter()
{
	++m_siAcceptCounter;
	return;
}

SI16 CPrivateTradeOrderObj::GetAcceptCounter()
{
	return m_siAcceptCounter;
}

GMONEY CPrivateTradeOrderObj::GetPersonMoney( SI32 siPersonID )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	return pTradeData->m_siPrice;
}

void CPrivateTradeOrderObj::GetStockInfo( SI32 siPersonID, SI32 *pStockVillageUnique, SI32 *pStockNum )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	*pStockVillageUnique = pTradeData->m_siStockVillageUnique;
	*pStockNum = pTradeData->m_siStockNum;

	return;
}

void CPrivateTradeOrderObj::GetHouseUnitInfo( SI32 siPersonID, SI32 *pHouseVillageUnique, SI32 *pHouseUnitSlot )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	*pHouseVillageUnique = pTradeData->m_siHouseVillageUnique;
	*pHouseUnitSlot = pTradeData->m_siHouseUnitSlot;

	return;
}

void CPrivateTradeOrderObj::GetHorseInfo( SI32 siPersonID, SI16 *pHorseIndex )
{
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetTradeData( siPersonID );

	*pHorseIndex = pTradeData->m_siHorseIndex;

	return;
}

bool CPrivateTradeOrderObj::IsAvailableAccpet()
{
	if ( TABS( pclClient->GetFrame() - m_uiAcceptFrame ) > 60 )
	{
		return true;
	}

	return false;
}

SI16 CPrivateTradeOrderObj::GetSelcteMode()
{
	return m_siSelectMode;
}

CEachPersonTradeOrderInfo* CPrivateTradeOrderObj::GetEachPersonTradeOrderInfo()
{
	return m_pEachPersonTradeOrderInfo;
}