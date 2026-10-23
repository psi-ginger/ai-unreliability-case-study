#ifndef _PRIVATETRADEORDERMGR_H_
#define _PRIVATETRADEORDERMGR_H_

#include "..\SMLib\LibList\LibListMgr.h"
#include "PrivateTradeOrderBaseInfo.h"
#include "NDataTypes.h"

class cltTime;
class CPrivateTradeOrderObj;
class cltItem;
class cltHorse;

class CPrivateTradeOrderMgr : public CLibListMgr
{
public:
	CPrivateTradeOrderMgr();
	~CPrivateTradeOrderMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyObj();

	bool Set( SI32 siPerson1ID, SI32 siPerson2ID, SI16 *psiMgrIndex, SI16 *psiCountIndex, SI16 TradeMode );
	void Action( cltTime *pTime );

	CPrivateTradeOrderObj *FindTradeOrderObj( SI16 siMgrIndex, SI16 siCountIndex );
	CPrivateTradeOrderObj *FindTradeOrderObj( SI32 siPersonID );

	SI16 AddItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, cltItem *pclItem, SI16 siItemPos );
	bool PrivateTradeSetPrice( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, GMONEY siPrice );
	bool SetStock( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 siVillageUnique, SI32 siStockNum );
	bool SetHouseUnit( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 HouseVillageUnique, SI32 HouseUnitSlot );
	bool SetHorse( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 siHorseIndex, cltHorse *pclTradeHorse );

	void Cancel( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );
	void Cancel( SI32 siPerson1ID, SI16 siMgrIndex, SI16 siCountIndex );
	void Cancel( SI32 siPerson1ID );

	bool GetOrderIndexItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siOrderListIndex, SI16 siMgrIndex, SI16 siCountIndex, cltItem *pclItem );
	bool GetOrderItemList( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, cltItem *pclPerson1Item, cltItem *pclPerson2Item, SI16 *psiPerson1ItemPosList, SI16 *psiPerson2ItemPosList );

	bool DelOrderListIndex( SI32 Person1ID, SI32 Person2ID, SI16 DelType, SI16 DelIndex, SI16 MgrIndex, SI16 CountIndex );
	bool DelOrderIndexItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siOrderListIndex, SI16 siMgrIndex, SI16 siCountIndex );

	bool IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );

	bool SetMyAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );
	bool SetMyCancel( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );

	bool IsAllAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );

	bool IsAcceptStatus( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );

	SI16 GetOrderIndexItemInventoryPos( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 siOrderIndex  );
	
	bool IsAvailableMoney( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, GMONEY Person1Money, GMONEY Person2Money );
	bool IsAvailableStock( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 Char1ID, SI32 Char2ID );
	bool IsAvailableHorse( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 Char1ID, SI32 Char2ID );

	bool IsSameAcceptCounter( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 siAcceptCounter );

	GMONEY GetPersonMoney( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );
	bool GetStockInfo ( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 *pStockVillageUnique1, SI32 *pStockNum1, SI32 *pStockVillageUnique2, SI32 *pStockNum2 );
	bool GetHouseUnitInfo ( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 *pHouseVillageUnique1, SI32 *pHouseUnit1, SI32 *pHouseVillageUnique2, SI32 *pHouseUnit2 );
	bool GetHorseInfo ( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 *Person1HorseTrade, SI16 *Person2HorseTrade );

	void IncreaseAcceptCounter( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );

	bool IsAvailableAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex );
	
	void PrivateTradeCancel( SI32 Person1ID, SI32 Person2ID, SI32 id1, SI32 id2, SI16 MgrIndex, SI16 CountIndex, SI16 Reason );
private:


};

#endif