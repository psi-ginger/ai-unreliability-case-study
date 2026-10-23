#ifndef _MSGTYPEWORLDMONEY_H
#define _MSGTYPEWORLDMONEY_H

#include "../../../DBManager/GameDBManager_World/DBMsg-WorldMoney.h"
#include "../../CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"

//-------------------------------------------
// 아큐 사는 창을 연다. - 운영자 명령어로
//-------------------------------------------
class cltGameMsgResponse_SetWorldMoney_BuyDlg{
public:
	SI64				siWorldMoney; // 살 수 있는 아큐
	SI64				siWorldMoneyPrice; // 아큐의 가격 ( 1아큐당 얼마 )
	SI64				siTotalWorldMoney; // 총 공모된 아큐

	cltGameMsgResponse_SetWorldMoney_BuyDlg(SI64 money , SI64 price,SI64 total)
	{
		siWorldMoney = money ;
		siWorldMoneyPrice = price ;
		siTotalWorldMoney = total ;
	}
};

class cltGameMsgRequest_Distrubution_WorldMoney{
public:
	SI64			siWorldMoney; // 사는 아큐의 수
	SI64			siWorldMoneyPrice; // 사는 아큐의 가격

	cltGameMsgRequest_Distrubution_WorldMoney(SI64 money,SI64 price)
	{
		siWorldMoney = money ;
		siWorldMoneyPrice = price ;
	}
};

class cltGameMsgResponse_Distrubution_WorldMoney{
public:
	SI64			siWorldMoney; // 사는 아큐의 수
	SI64			siLeftWorldMoney; // 산 뒤의 아큐의 수
	SI64			siWorldMoneyPrice; // 산 아큐의 가격
	SI64			siLeftMoney; // 사고 남은 돈

	cltGameMsgResponse_Distrubution_WorldMoney(SI64 worldmoney,SI64 leftworldmoney,SI64 price,SI64 leftmoney)
	{
		siWorldMoney = worldmoney ; // 사는 아큐의 수
		siLeftWorldMoney = leftworldmoney; // 산 뒤의 아큐의 수
		siWorldMoneyPrice = price; // 산 아큐의 가격
		siLeftMoney = leftmoney; // 사고 남은 돈
	}
};

// 아큐 거래소 리스트를 돌려준다
class cltGameMsgResponse_WorldMoney_TradeList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltWorldMoneyTradeList, 50>		clInfo;
#else
	cltWorldMoneyTradeList		clInfo[50];
#endif
	SI32						siPage;
	bool						bEnd;

	SI64						siAverageWorldMoneyPrice;

	cltGameMsgResponse_WorldMoney_TradeList(cltWorldMoneyTradeList*pclinfo,SI32 page ,bool end,SI64 price)
	{
		for ( SI32 i = 0 ; i < 50 ; i ++ )
			clInfo[i].Set(&pclinfo[i]);

		siPage = page ;
		bEnd = end ;

		siAverageWorldMoneyPrice = price;
	}
};

// 아큐를 거래소에 등록한다.
class cltGameMsgRequest_WorldMoney_SellOrder
{
public:
	SI64		siWorldMoney; // 등록할 아큐
	SI64		siPrice; // 등록할 가격

	cltGameMsgRequest_WorldMoney_SellOrder(SI64 worldmoney,SI64 price)
	{
		siWorldMoney = worldmoney ;
		siPrice = price ;
	}
};

class cltGameMsgResponse_WorldMoney_SellOrder
{
public:
	SI32		siResult;

	SI64		siWorldMoney; // 등록한 아큐
	SI64		siPrice; // 등록한 가격

	SI64		siLeftWorldMoney; // 등록한 뒤의 아큐

	SI64		siLeftMoney; // 세금 빼고 남은 돈

	cltGameMsgResponse_WorldMoney_SellOrder(SI32 result,SI64 worldmoney,SI64 leftworldmoney,SI64 price,SI64 leftmoney)
	{
		siResult = result;

		siWorldMoney = worldmoney ;
		siPrice = price ;

		siLeftWorldMoney = leftworldmoney ;

		siLeftMoney = leftmoney ;
	}
};

// 아큐를 거래소에서 구입한다.
class cltGameMsgRequest_WorldMoney_BuyOrder
{
public:
	SI32		siIndex; // 살 리스트의 인덱스
	SI64		siWorldMoney; // 살 아큐

	cltGameMsgRequest_WorldMoney_BuyOrder(SI32 index , SI64 worldmoney)
	{
		siIndex = index ;
		siWorldMoney = worldmoney ;
	}
};

class cltGameMsgResponse_WorldMoney_BuyOrder
{
public:
	SI32		siResult;

	SI64		siWorldMoney; // 산 아큐
	SI64		siLeftWorldMoney; // 산 뒤의 총 아큐

	SI64		siLeftMoney; // 사고 남은 돈

	cltGameMsgResponse_WorldMoney_BuyOrder(SI32 result,SI64 worldmoney,SI64 leftworldmoney,SI64 leftmoney)
	{
		siResult = result ;

		siWorldMoney = worldmoney ;
		siLeftWorldMoney = leftworldmoney ;

		siLeftMoney = leftmoney ;
	}
};

class cltGameMsgResponse_WorldMoney_CancelOrder
{
public:
	SI32		siResult;

	SI64		siLeftWorldMoney; // 회수한 뒤의 총 아큐

	cltGameMsgResponse_WorldMoney_CancelOrder(SI32 result,SI64 leftworldmoney)
	{
		siResult = result ;

		siLeftWorldMoney = leftworldmoney ;
	}
};

class cltGameMsgResponse_WorldMoney_Set_Char
{
public:
	TCHAR		szCharName[MAX_PLAYER_NAME];

	SI64		siWorldMoney;
	SI64		siLeftWorldMoney;

	cltGameMsgResponse_WorldMoney_Set_Char(TCHAR *name,SI64 worldmoney,SI64 leftworldmoney)
	{
		MStrCpy(szCharName,name,MAX_PLAYER_NAME);

		siWorldMoney = worldmoney ;
		siLeftWorldMoney = leftworldmoney ;
	}
};


//----------------------------------------------
// TradeServer
//----------------------------------------------
// 트레이드 서버에 주문을 올린다.
class cltGameMsgRequest_TradeServer_Set_SellOrder
{
public:
	cltItem			clitem; // 주문품
	SI16			siPos; // 주문품 위치

	SI64			siPrice; // 주문품 가격

	cltGameMsgRequest_TradeServer_Set_SellOrder(cltItem*pclitem,SI16 pos,SI64 price)
	{
		clitem.Set(pclitem) ;
		siPos = pos ;

		siPrice = price ;
	}
};

class cltGameMsgResponse_TradeServer_Set_SellOrder
{
public:
	SI32		siResult;

	//cltItem		clitem; // 주문품
	cltTradeServerOrder clOrder;
	SI16		siPos; // 주문품 위치
	cltItem		clLeftItem; // 주문뒤 남은 아이템

	SI64		siLeftWorldMoney; // 주문뒤 남은 아큐

	cltGameMsgResponse_TradeServer_Set_SellOrder(SI32 result,cltTradeServerOrder*pclorder,SI16 pos,cltItem*pclleftitem,SI64 leftworldmoney)
	{
		siResult = result ;

		clOrder.Set(pclorder);
		siPos = pos ;
		clLeftItem.Set(pclleftitem);

		siLeftWorldMoney = leftworldmoney ;
	}
};

// 트레이드 서버에 주문을 내린다.
class cltGameMsgRequest_TradeServer_Cancel_SellOrder
{
public:
	SI32			siItemUnique;
	SI32			siItemNum;
	SI32			siIndex;

	cltGameMsgRequest_TradeServer_Cancel_SellOrder(SI32 unique,SI32 itemnum,SI32 index)
	{
		siItemUnique = unique ;
		siItemNum = itemnum ;
		siIndex = index ;
	}
};

class cltGameMsgResponse_TradeServer_Cancel_SellOrder
{
public:
	SI32		siResult;

	cltTradeServerOrder		clOrder; // 주문품
	SI16		siPos; // 주문품 위치
	cltItem		clLeftItem; // 주문뒤 남은 아이템

	cltGameMsgResponse_TradeServer_Cancel_SellOrder(SI32 result,cltTradeServerOrder*pclOrder,SI16 pos,cltItem*pclleftitem)
	{
		siResult = result ;

		clOrder.Set(pclOrder);
		siPos = pos ;
		clLeftItem.Set(pclleftitem);
	}
};

// 트레이드 서버에 주문을 산다.
class cltGameMsgRequest_TradeServer_Set_BuyOrder
{
public:
	cltTradeServerOrder		clOrder; // 주문품

	cltGameMsgRequest_TradeServer_Set_BuyOrder(cltTradeServerOrder*pclorder)
	{
		clOrder.Set(pclorder);
	}
};

class cltGameMsgResponse_TradeServer_Set_BuyOrder
{
public:
	SI32		siResult;

	cltTradeServerOrder		clOrder; // 주문품
	SI16		siPos; // 주문품 위치
	cltItem		clLeftItem; // 주문뒤 남은 아이템
	SI64		siLeftWorldMoney;

	cltGameMsgResponse_TradeServer_Set_BuyOrder(SI32 result,cltTradeServerOrder*pclOrder,SI16 pos,cltItem*pclleftitem,SI64 worldmoney)
	{
		siResult = result ;

		clOrder.Set(pclOrder);
		siPos = pos ;
		clLeftItem.Set(pclleftitem);
		siLeftWorldMoney = worldmoney;
	}
};

// 트레이드 서버에 리스트를 요청한다.
class cltGameMsgRequest_TradeServer_Get_ListInfo
{
public:
	SI32			siIndex; // 아이템 유니크다!

	cltGameMsgRequest_TradeServer_Get_ListInfo(SI32 index)
	{
		siIndex = index ;
	}
};

//class cltGameMsgResponse_TradeServer_Get_ListInfo
//{
//public:
//	cltTradeServerOrder			clInfo[MAX_TRADESERVER_GET_LIST];
//
//	SI32						siIndex;
//	bool						bEnd;
//
//	cltGameMsgResponse_TradeServer_Get_ListInfo(cltTradeServerOrder*pclinfo,SI32 index,bool end)
//	{
//		for ( SI32 i = 0 ; i < 50 ; i ++ )
//		{
//			clInfo[i].Set(&pclinfo[i]);
//		}
//
//		siIndex = index ;
//		bEnd = end ;
//	}
//};

class cltGameMsgResponse_TradeServer_Get_ListInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTradeServerOrder, MAX_TRADESERVER_GET_LIST>		clInfo;
#else
	cltTradeServerOrder			clInfo[MAX_TRADESERVER_GET_LIST];
#endif

	SI32						siIndex;
	bool						bEnd;
	SI64						siAveragePrice; // 물품의 시세
	SI64						siAverageWorldMoney; // 아큐의 시세
	bool						bIsItem; // 올라와 있는 아이템이 있냐?

	bool						bIsWorldTradeTax;			// 무역 세금 적용여부
	SI32						siSpecialTaxRange;		// 특별세 적용 범위
	SI32						siSpecialTaxRate;		// 특별세율

	cltGameMsgResponse_TradeServer_Get_ListInfo(cltTradeServerOrder*pclinfo,SI32 index,bool end,
		SI64 price,SI64 worldmoney,bool isitem, bool isworldtradetax, SI32 specialtaxrange,
		SI32 specialtaxrate )
	{
		for ( SI32 i = 0 ; i < 50 ; i ++ )
		{
			clInfo[i].Set(&pclinfo[i]);
		}

		siIndex = index ;
		bEnd = end ;

		siAveragePrice = price ;
		siAverageWorldMoney = worldmoney ;

		bIsItem = isitem ;

		bIsWorldTradeTax = isworldtradetax;
		siSpecialTaxRange = specialtaxrange;
		siSpecialTaxRate = specialtaxrate;
	}
};


// 트레이드 서버에서 팔린 물건에 대한 정보를 받앗다.
class cltGameMsgResponse_TradeServer_Get_SellInfo
{
public:
	cltTradeServerOrder	clOrder;

	SI64				siLeftWorldMoney;

	cltGameMsgResponse_TradeServer_Get_SellInfo(cltTradeServerOrder*pclinfo,SI64 worldmoney)
	{
		clOrder.Set(pclinfo);

		siLeftWorldMoney = worldmoney ;
	}
};

// 트레이드 서버에 올린 자기 물건의 정보를 받는다 - 이건 무역 서버와의 시간차로 인하여 틀릴 수도 있다!
class cltGameMsgResponse_TradeServer_Get_MyInfo
{
public:
	cltTradeServerOrder clOrder;

	cltGameMsgResponse_TradeServer_Get_MyInfo(cltTradeServerOrder* pclorder)
	{
		clOrder.Set(pclorder);
	}
};


// 무역서버에 저장된 아큐와 거래내역을 받는다
class cltGameMsgResponse_TradeServer_Get_SavedACU_List
{
public:
	SI64				m_siSavedACU;

	cltTradeAcuList		m_clAcuList[MAX_TRADESERVER_GET_LIST];

	cltGameMsgResponse_TradeServer_Get_SavedACU_List( SI64 siSavedACU, cltTradeAcuList* pclAcuList )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_TradeServer_Get_SavedACU_List) );

		m_siSavedACU = siSavedACU;

		if ( pclAcuList )
		{
			memcpy( m_clAcuList, pclAcuList, sizeof(m_clAcuList) );
		}
	}
};	// GAMEMSG_RESPONSE_TRADESERVER_GET_SAVEDACU_LIST

// 저장된 아큐 받아오기
class cltGameMsgRequest_TradeServer_Get_Withdraw_Acu
{
public:
	SI64	m_siWithdrawAcu;

	cltGameMsgRequest_TradeServer_Get_Withdraw_Acu( SI64 siWithdrawAcu )
	{
		m_siWithdrawAcu = siWithdrawAcu;
	}
};	// GAMEMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU

class cltGameMsgResponse_TradeServer_Get_Withdraw_Acu
{
public:
	SI64	m_siHoldingAcu;		// 변경된 내가 갖고 있는 아큐

	SI64	m_siLeftSavedAcu;	// 지급되고 남은 저장된 아큐
	

	cltGameMsgResponse_TradeServer_Get_Withdraw_Acu( SI64 siHoldingAcu, SI64 siLeftSavedAcu )
	{
		m_siHoldingAcu		= siHoldingAcu;
		m_siLeftSavedAcu	= siLeftSavedAcu;
	}
};	// GAMEMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU
//-----------------------------------------------------------------------------------------------
//
// 무역관련 여권
//
//-----------------------------------------------------------------------------------------------

// 여권 발급
class cltGameMsgRequest_WorldPassport_Set
{
public:
	TCHAR	m_szPassportName[ Const_Max_WorldPassport_Name ];
	

	cltGameMsgRequest_WorldPassport_Set( TCHAR* pszPassportName )
	{
		// Init
		ZeroMemory( this, sizeof(cltGameMsgRequest_WorldPassport_Set) );

		// Set
		if ( pszPassportName )
		{
			MStrCpy( m_szPassportName, pszPassportName, Const_Max_WorldPassport_Name );
		}
	}
};	// GAMEMSG_REQUEST_WORLDPASSPORT_SET

class cltGameMsgResponse_WorldPassport_Set
{
public:
	TCHAR	m_szPassportName[ Const_Max_WorldPassport_Name ];

	SI32	m_siIssueDateVary;	// 발행일
	SI32	m_siExpireDateVary;	// 만료일

	SI64	m_siLeftAcu;		// 여권 발행비를 내고 남은 내 아큐


	cltGameMsgResponse_WorldPassport_Set( TCHAR* pszPassportName, SI32 siIssueDateVary, SI32 siExpireDateVary, SI64 siLeftAcu )
	{
		// Init
		ZeroMemory( this, sizeof(cltGameMsgResponse_WorldPassport_Set) );

		// Set
		if ( pszPassportName )
		{
			MStrCpy( m_szPassportName, pszPassportName, Const_Max_WorldPassport_Name );
		}

		m_siIssueDateVary	= siIssueDateVary;
		m_siExpireDateVary	= siExpireDateVary;

		m_siLeftAcu			= siLeftAcu;
		
	}
};	// GAMEMSG_RESPONSE_WORLDPASSPORT_SET

// 여권 기간 연장

//----------------------------------------------------------------
// Request는 메시지만 존재
// GAMEMSG_REQUEST_WORLDPASSPORT_EXTEND
//----------------------------------------------------------------

class cltGameMsgResponse_WorldPassport_Extend
{
public:
	SI32	m_siExpireDateVary;	// 만료일

	SI64	m_siLeftAcu;		// 여권 연장비를 내고 남은 내 아큐

	
	cltGameMsgResponse_WorldPassport_Extend( SI32 siExpireDateVary, SI64 siLeftAcu )
	{
		// Init
		ZeroMemory( this, sizeof(cltGameMsgResponse_WorldPassport_Extend) );

		// Set
		m_siExpireDateVary	= siExpireDateVary;

		m_siLeftAcu			= siLeftAcu;
	}

	
};	// GAMEMSG_RESPONSE_WORLDPASSPORT_EXTEND

#endif