#ifndef _PRIVATETRADEORDEROBJ_H_
#define _PRIVATETRADEORDEROBJ_H_

#include "..\SMLib\LibList\Obj.h"

#include "PrivateTradeOrderBaseInfo.h"

#include "..\Item\ItemCommon\ItemUnit.h"

#include "../CommonHeader.h"

// 각 유저가 거래창에 올리는 정보를 가지고 있음.
struct stTradeData
{
	SI32			m_siPersonID;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_PRIVATETRADEORDER_ITEM_NUM>	m_clItem;
	NSafeTArray<SI16, MAX_PRIVATETRADEORDER_ITEM_NUM>		m_siSelectedInventoryPos;
#else
	cltItem			m_clItem[ MAX_PRIVATETRADEORDER_ITEM_NUM ];
	SI16			m_siSelectedInventoryPos[ MAX_PRIVATETRADEORDER_ITEM_NUM ];
#endif
	GMONEY			m_siPrice;

	SI32			m_siStockVillageUnique;
	SI32			m_siStockNum;

	SI32			m_siHouseVillageUnique;
	SI32			m_siHouseUnitSlot;
	cltRentContract	m_clRentContract;

	SI16			m_siHorseIndex;
	cltHorse		m_clTradeHorse;

	bool			m_bMyAccept;
	bool			m_bOtherAccept;

	

	stTradeData()
	{
		memset( this, 0, sizeof( stTradeData ) );
		m_siHorseIndex = -1;
	}

	void SetStock( SI32 StockVillageUnique, SI32 StockNum )
	{
		m_siStockVillageUnique = StockVillageUnique;
		m_siStockNum = StockNum;
		return;
	}

	void SetHouseUnit( SI32 HouseVillageUnique, SI32 HouseUnitSlot )
	{
		m_siHouseVillageUnique = HouseVillageUnique;
		m_siHouseUnitSlot = HouseUnitSlot;
	}

	void SetHorse( SI16 HorseIndex, cltHorse *pclHorse )
	{
		m_siHorseIndex = HorseIndex;
		m_clTradeHorse.Set( pclHorse );
	}

	/*
	void ChangeMode()
	{
		memset( m_clItem, 0, sizeof( cltItem ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
		memset( m_siSelectedInventoryPos, 0, sizeof( SI16 ) * MAX_PRIVATETRADEORDER_ITEM_NUM );
		m_siPrice = 0;
		m_bMyAccept = false;
		m_bOtherAccept = false;
	}
	*/

};

class CEachPersonTradeOrderInfo
{
public:
	CEachPersonTradeOrderInfo();
	~CEachPersonTradeOrderInfo();

	void Initialize();
	void Destroy();

	void Set( SI32 siPerson1ID, SI32 siPerson2ID );
	
	void SetPerson1TradePrice( GMONEY siPrice );
	void SetPerson2TradePrice( GMONEY siPrice );

	void AddPerson1TradeItem( cltItem *pclItem );
	void AddPerson2TradeItem( cltItem *pclItem );

	SI32 GetPerson1ID();
	SI32 GetPerson2ID();

	void GetPersonID( SI32 *pPerson1ID, SI32 *pPerson2ID );

	bool IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID );
	
	stTradeData *GetTradeData( SI32 siPersonID );

	stTradeData *GetPerson1TradeData();
	stTradeData *GetPerson2TradeData();

private:
	stTradeData	Person1TradeData;
	stTradeData	Person2TradeData;

	bool		m_bPerson1MyAccept;
	bool		m_bPerson2MyAccept;	

};



class CPrivateTradeOrderObj : public CObj
{
public:
	CPrivateTradeOrderObj( SI16 siMgrIndex );
	~CPrivateTradeOrderObj();

	void Initialize();
	void Destroy();

	void Set( SI32 siPerson1ID, SI32 siPerson2ID, SI16 *psiMgrIndex, SI16 *psiCountIndex, SI16 TradeMode );

	bool IsSame( SI16 siMgrIndex, SI16 siCountIndex );

	bool IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID );

	bool IsExist( SI32 siPerson1ID );

	SI16 AddItem( SI32 siPersonID, cltItem *pclItem, SI16 siItemPos );

	bool PrivateTradeSetPrice( SI32 siPersonID, GMONEY siPrice );
	void SetStock( SI32 siPersonID, SI32 siVillageUnique, SI32 siStockNum );
	void SetHouseUnit( SI32 siPersonID, SI32 HouseVillageUnique, SI32 HouseUnitSlot );
	void SetHorse( SI32 siPersonID, SI16 siHorseIndex, cltHorse *pclHorse );

	bool GetOrderIndexItem( SI32 siPersonID, SI16 siOrderListIndex, cltItem *pclItem );

	bool DelOrderListIndex( SI32 PersonID, SI16 DelType, SI16 Param );
	void DelOrderIndexItem( SI32 siPersonID, SI16 siOrderListIndex );

	void SetMyAccept( SI32 siPersonID );
	void SetMyCancel( SI32 siPersonID );

	bool IsAllAccept();
	bool IsAcceptStatus( SI32 siPersonID );

	SI16 GetOrderIndexItemInventoryPos( SI32 siPersonID, SI32 siOrderIndex );

	void GetOrderItemList( SI32 siPersonID, cltItem *pclItemList, SI16 *psiItemPosList );

	bool IsAvailableMoney( SI32 siPersonID, GMONEY Price );
	bool IsAvailableStock( SI32 siPersonID, SI32 siCharID );
	bool IsAvailableHorse( SI32 siPersonID, SI32 siCharID );

	void IncreaseAcceptCounter();
	SI16 GetAcceptCounter();

	GMONEY GetPersonMoney( SI32 siPersonID );
	void GetStockInfo( SI32 siPersonID, SI32 *pStockVillageUnique, SI32 *pStockNum );
	void GetHouseUnitInfo( SI32 siPersonID, SI32 *pHouseVillageUnique, SI32 *pHouseUnitSlot );
	void GetHorseInfo( SI32 siPersonID, SI16 *pHorseIndex );

	bool IsAvailableAccpet();

	SI16 GetSelcteMode();

	CEachPersonTradeOrderInfo* GetEachPersonTradeOrderInfo();

private:

	CEachPersonTradeOrderInfo *m_pEachPersonTradeOrderInfo;

	SI16 m_siMgrIndex;			// Manager 의 index 값임
	SI32 m_siCountIndex;		// Manager 의 index 값을 좀더 명확히 하기 위해서...

	SI16 m_siSelectMode;		// 일반 거래냐, 수리냐를 구분하기 위해서.

	SI16 m_siAcceptCounter;		// AcceptCounter 가 같아야, 거래가 가능하다.

	UI32 m_uiAcceptFrame;	
};

#endif