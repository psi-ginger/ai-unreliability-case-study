#ifndef _MSGTYPE_PRIVATETRADE_H_
#define _MSGTYPE_PRIVATETRADE_H_

#include "..\CommonLogic.h"	
//GAMEMSG_REQUEST_PRIVATETRADE

class cltGameMsgRequest_PrivateTrade_Accept
{
public:	
	SI32 siToPersonCharUnique;
	SI16 siTradeMode;

	cltGameMsgRequest_PrivateTrade_Accept( SI32 ToPersonCharUnique, SI16 TradMode )
	{		
		siToPersonCharUnique = ToPersonCharUnique;
		siTradeMode = TradMode;
	}
};

class cltGameMsgResponse_PrivateTrade_Accept
{
public:
	//SI16 siResult;					// 1 : 성공, 2 : 실패
	SI32 siOtherCharUnique;				// 상대방의 캐릭터 유니크 값
	//SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	//SI16 siPrivateTradeOrderCountIndex;
	SI16 siTradeMode;
	SI16 siOtherCharLevel;
	TCHAR strCharName[ MAX_PLAYER_NAME ];
	TCHAR strAccountID[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_PrivateTrade_Accept( SI32 OtherCharUnique, 
		//SI16 PrivateTradeOrderMgrIndex, 
		//SI16 PrivateTradeOrderCountIndex,
		SI16 TradeMode, SI16 OtherCharLevel, 
		TCHAR *CharName,
		TCHAR *AccountID 
		)
	{
		siOtherCharUnique = OtherCharUnique;
		//siPrivateTradeOrderMgrIndex = PrivateTradeOrderMgrIndex;
		//siPrivateTradeOrderCountIndex = PrivateTradeOrderCountIndex;
		siTradeMode = TradeMode;
		siOtherCharLevel = OtherCharLevel;

		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
		MStrCpy( strAccountID, AccountID, MAX_PLAYER_NAME );
	}
};


class cltGameMsgRequest_PrivateTrade
{
public:	
	SI32 siToPersonCharUnique;
	SI16 siTradeMode;
	

	cltGameMsgRequest_PrivateTrade( SI32 ToPersonCharUnique, SI16 TradMode )
	{		
		siToPersonCharUnique = ToPersonCharUnique;
		siTradeMode = TradMode;
	}
};

class cltGameMsgResponse_PrivateTrade
{
public:
	//SI16 siResult;					// 1 : 성공, 2 : 실패
	SI32 siOtherCharUnique;				// 상대방의 캐릭터 유니크 값
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	SI16 siTradeMode;
	SI16 siOtherCharLevel;
	TCHAR strCharName[ MAX_PLAYER_NAME ];
	TCHAR strAccountID[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_PrivateTrade( SI32 OtherCharUnique, 
		SI16 PrivateTradeOrderMgrIndex, SI16 PrivateTradeOrderCountIndex,
		SI16 TradeMode, SI16 OtherCharLevel, 
		TCHAR *CharName,
		TCHAR *AccountID 
		)
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = PrivateTradeOrderMgrIndex;
		siPrivateTradeOrderCountIndex = PrivateTradeOrderCountIndex;
		siTradeMode = TradeMode;
		siOtherCharLevel = OtherCharLevel;

		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
		MStrCpy( strAccountID, AccountID, MAX_PLAYER_NAME );
	}
};

class cltGameMsgRequest_PrivateTrade_Cancel
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	cltGameMsgRequest_PrivateTrade_Cancel( SI32 CharUnique, SI16 PrivateTradeOrderMgrIndex, SI16 PrivateTradeOrderCountIndex )
	{
		siOtherCharUnique = CharUnique;
		siPrivateTradeOrderMgrIndex = PrivateTradeOrderMgrIndex;
		siPrivateTradeOrderCountIndex = PrivateTradeOrderCountIndex;
	}
};

class cltGameMsgResponse_PrivateTrade_Cancel
{
public:
	
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	cltGameMsgResponse_PrivateTrade_Cancel()
	{
	}
};

class cltGameMsgRequest_PrivateTrade_SelectMode
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16 siSelectedMode;

	cltGameMsgRequest_PrivateTrade_SelectMode( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 SelectedMode )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
		siSelectedMode = SelectedMode;
	}	
};

class cltGameMsgRequest_PrivateTrade_AddItem
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI16	siItemPos;
	cltItem	clItem;						// 안에 내구도나 기타 정보 같은게 중요시 되는 거라...
	SI32	siAddItemNum;
	
	cltGameMsgRequest_PrivateTrade_AddItem( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 ItemPos, cltItem *pclItem, SI32 AddItemNum )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
		siAddItemNum = AddItemNum;
	}
};

class cltGameMsgResponse_PrivateTrade_AddItem
{
public:
	SI32	siCharUnique;
	SI16	siAddedIndex;
	cltItem	clItem;
	

	cltGameMsgResponse_PrivateTrade_AddItem( SI32 CharUnique, SI16 AddedIndex, cltItem *pclItem )
	{
		siCharUnique = CharUnique;
		siAddedIndex = AddedIndex;
		clItem.Set( pclItem );
	}
};

class cltGameMsgRequest_PrivateTrade_DelList
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	
	SI16 siDelType;
	SI16 siParam;
	
	cltGameMsgRequest_PrivateTrade_DelList( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 DelType, SI16 Param )
	{
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siDelType = DelType;
		siParam = Param;
	}
};

class cltGameMsgResponse_PrivateTrade_DelList
{
public:
	SI32 siCharUnique;
	SI16 siDelType;
	SI16 siParam;

	cltGameMsgResponse_PrivateTrade_DelList( SI32 CharUnique, SI16 DelType, SI16 Param )
	{
		siCharUnique = CharUnique;
		siDelType = DelType;
		siParam = Param;
	}
};

class cltGameMsgRequest_PrivateTrade_DelItem
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	
	SI16 siDelIndex;
	cltItem clItem;

	cltGameMsgRequest_PrivateTrade_DelItem( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 DelIndex, cltItem* pclItem )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siDelIndex = DelIndex;
		clItem.Set( pclItem );

	}
};

class cltGameMsgRequest_PrivateTrade_AddPrice
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	GMONEY siPrice;

	cltGameMsgRequest_PrivateTrade_AddPrice( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, GMONEY Price )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex =  MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
		siPrice = Price;
	}
};

class cltGameMsgResponse_PrivateTrade_AddPrice
{
public:
	SI32 siOtherCharUnique;
	GMONEY siPrice;

	cltGameMsgResponse_PrivateTrade_AddPrice( SI32 OtherCharUnique, GMONEY Price )
	{
		siOtherCharUnique = OtherCharUnique;		
		siPrice = Price;
	}
};

class cltGameMsgRequest_PrivateTrade_AddStock
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;

	SI32 siVillageUnique;
	SI32 siStockNum;

	cltGameMsgRequest_PrivateTrade_AddStock( SI32 OtherCharUnique, SI32 MgrIndex, SI32 CountIndex, SI32 VillageUnique, SI32 StockNum )
	{
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siVillageUnique = VillageUnique;
		siStockNum = StockNum;
	}
};

class cltGameMsgResponse_PrivateTrade_AddStock
{
public:
	SI32 siOtherCharUnique;

	SI32 siVillageUnique;
	SI32 siStockNum;
	
	cltGameMsgResponse_PrivateTrade_AddStock( SI32 OtherCharUnique, SI32 StockVillageUnique, SI32 StockNum )
	{
		siOtherCharUnique = OtherCharUnique;

		siVillageUnique = StockVillageUnique;
		siStockNum = StockNum;
	}
};

class cltGameMsgRequest_PrivateTrade_AddHouseUnit
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;
	SI16 siPrivateTradeOrderCountIndex;

	SI32 siHouseVillageUnique;
	SI32 siHouseUnitSlot;

	cltGameMsgRequest_PrivateTrade_AddHouseUnit( SI32 OtherCharUnique, SI32 MgrIndex, SI32 CountIndex, SI32 HouseVillageUnique, SI32 HouseUnitSlot )
	{
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siHouseVillageUnique = HouseVillageUnique;
		siHouseUnitSlot = HouseUnitSlot;
	}
};

class cltGameMsgResponse_PrivateTrade_AddHouseUnit
{
public:
	SI32 siOtherCharUnique;
	
	SI32 siHouseVillageUnique;
	SI32 siHouseUnitSlot;
	cltRentContract clRentContract;

	cltGameMsgResponse_PrivateTrade_AddHouseUnit( SI32 OtherCharUnique, SI32 HouseVillageUnique, SI32 HouseUnitSlot, cltRentContract *pclRentContract )
	{
		siOtherCharUnique = OtherCharUnique;

		siHouseVillageUnique = HouseVillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		clRentContract.Set( pclRentContract );
	}
};


class cltGameMsgRequest_PrivateTrade_AddHorse
{
public:
	SI32 siOtherCharUnique;

	SI16 siPrivateTradeOrderMgrIndex;
	SI16 siPrivateTradeOrderCountIndex;

	SI16 siHorseIndex;

	cltGameMsgRequest_PrivateTrade_AddHorse( SI32 OtherCharUnique, SI32 MgrIndex, SI32 CountIndex, SI16 HorseIndex )
	{
		siOtherCharUnique = OtherCharUnique;

		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;

		siHorseIndex = HorseIndex;
	}
};

class cltGameMsgResponse_PrivateTrade_AddHorse
{
public:
	SI32 siOtherCharUnique;
	SI16 siHorseIndex;
	cltHorse clHorseInfo;

	cltGameMsgResponse_PrivateTrade_AddHorse( SI32 OtherCharUnique, SI16 HorseIndex, cltHorse *pclHorse )
	{
		siOtherCharUnique = OtherCharUnique;
		siHorseIndex = HorseIndex;
		if(pclHorse)	clHorseInfo.Set( pclHorse );
	}
};


class cltGameMsgResponse_PrivateTrade_DelItem
{
public:
	SI32 siCharUnique;
	SI16 siDelIndex;

	cltGameMsgResponse_PrivateTrade_DelItem( SI32 CharUnique, SI16 DelIndex )
	{
		siCharUnique = CharUnique;
		siDelIndex = DelIndex;
	}
};


class cltGameMsgRequest_PrivateTrade_MyAccept
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	SI16 siAcceptCounter;

	cltGameMsgRequest_PrivateTrade_MyAccept( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex, SI16 AcceptCounter )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
		siAcceptCounter = AcceptCounter;
	}
};


// 
class cltGameMsgResponse_PrivateTrade_MyAccept
{
public:
	SI16 siResult;
	cltGameMsgResponse_PrivateTrade_MyAccept( SI16 Result )
	{
		siResult = Result;
	}
};

class cltGameMsgRequest_PrivateTrade_MyCancel
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	
	cltGameMsgRequest_PrivateTrade_MyCancel( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
	
	}
};


// 바뀐 아이템 정보들만 보내겠지... cltItem 객체 하나씩 보낸다는 군...  
class cltGameMsgResponse_PrivateTrade_MyCancel
{
public:
	SI16 siResult;
	cltGameMsgResponse_PrivateTrade_MyCancel( SI16 Result )
	{
		siResult = Result;
	}
};

class cltGameMsgRequest_PrivateTrade_OtherAccept
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	

	cltGameMsgRequest_PrivateTrade_OtherAccept( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;
	
	}
	
};

class cltGameMsgResponse_PrivateTrade_OtherAccept
{
public:
	SI16 siResult;
	cltGameMsgResponse_PrivateTrade_OtherAccept( SI16 Result )
	{
		siResult = Result;
	}
};

class cltGameMsgRequest_PrivateTrade_OtherCancel
{
public:
	SI32 siOtherCharUnique;
	SI16 siPrivateTradeOrderMgrIndex;	// PrivatetTradeOrderMgr 로 부터 할당받은 거래 index
	SI16 siPrivateTradeOrderCountIndex;
	

	cltGameMsgRequest_PrivateTrade_OtherCancel( SI32 OtherCharUnique, SI16 MgrIndex, SI16 CountIndex )
	{
		siOtherCharUnique = OtherCharUnique;
		siPrivateTradeOrderMgrIndex = MgrIndex;
		siPrivateTradeOrderCountIndex = CountIndex;		
	}

};


class cltGameMsgResponse_PrivateTrade_OtherCancel
{
public:
	SI16 siResult;

	cltGameMsgResponse_PrivateTrade_OtherCancel( SI16 Result )
	{
		siResult = Result;
	}
};

class cltGameMsgResponse_PrivateTrade_End
{
public:
	SI16 siResult;
	
	SI32 siRecvStockVillageUnique;
	SI32 siRecvStockNum;
	
	SI32 siSendStockVillageUnique;
	SI32 siSendStockNum;
	
	SI32 siRecvHouseVillageUnique;
	SI32 siRecvHouseUnitSlot;

	cltRentContract clRecvRentContract;

	SI32 siSendHouseVillageUnique;
	SI32 siSendHouseUnitSlot;

	SI16 siHorseIndex;
	cltHorse clHorse;
	
	cltGameMsgResponse_PrivateTrade_End( SI16 Result,
		SI32 RecvStockVillageUnique, SI32 RecvStockNum,
		SI32 SendStockVillageUnique, SI32 SendStockNum,
		SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot,
		cltRentContract *pclRecvRentContract, 
		SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot,
		SI16 HorseIndex, cltHorse *pclHorse
		)
	{
		siResult = Result;
		
		siRecvStockVillageUnique = RecvStockVillageUnique;
		siRecvStockNum = RecvStockNum;
		
		siSendStockVillageUnique = SendStockVillageUnique;
		siSendStockNum = SendStockNum;

		siRecvHouseVillageUnique = RecvHouseVillageUnique;
		siRecvHouseUnitSlot = RecvHouseUnitSlot;
		
		if(pclRecvRentContract != NULL)		clRecvRentContract.Set( pclRecvRentContract );

		siSendHouseVillageUnique = SendHouseVillageUnique;
		siSendHouseUnitSlot = SendHouseUnitSlot;

		siHorseIndex = HorseIndex;
		
		if(pclHorse != NULL)		clHorse.Set( pclHorse );
	}
};


#endif