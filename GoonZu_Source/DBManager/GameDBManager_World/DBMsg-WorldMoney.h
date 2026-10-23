#ifndef _DBMSGWORLDMONEY_H
#define _DBMSGWORLDMONEY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "../../GoonzuWorld/CommonLogic/WorldMoneyMgr/WorldMoneyMgr.h"
#include "../../GoonzuWorld/Server/NTradeServerProcess/TradeServerMsg.h"
#include "../../GoonZuWorld/CommonLogic/WorldPassport/WorldPassport.h"

// 아큐를 시스템에게서 산다.
#define INPUTREASON_WORLDMONEY_DISTRIBUTION		1	// 공모방식으로 샀다.

class sDBRequest_Distribution_WorldMoney
{
public:
	sPacketHeader			packetHeader;

	UI16					usCharId;
	SI32					siPersonID;

	GMONEY					siMoney;			// 아큐사는데 들어간 돈
	SI64					siWorldMoney;		// 산 아큐

	SI32					siReason;

	sDBRequest_Distribution_WorldMoney( UI16 charid,SI32 personid,GMONEY money , SI64 worldmoney,SI32 reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_Distribution_WorldMoney );
		packetHeader.usCmd = DBMSG_REQUEST_DISTRIBUTION_WORLDMONEY;

		usCharId = charid ;
		siPersonID = personid ;

		siMoney = money ;
		siWorldMoney = worldmoney ;

		siReason = reason ;
	}
};

class sDBResponse_Distribution_WorldMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	GMONEY				siMoney;			// 아큐사는데 들어간 돈
	GMONEY				siLeftMoney;		// 아큐 사고 남은 돈
	SI64				siWorldMoney;		// 산 아큐
	SI64				siLeftWorldMoney;	// 산 뒤의 아큐

	SI32				siReason;

	sDBResponse_Distribution_WorldMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_Distribution_WorldMoney));
	}
};

class sDBRequest_WorldMoney_TradeList
{
public:
	sPacketHeader			packetHeader;

	SI32					siPage;

	sDBRequest_WorldMoney_TradeList(SI32 page )
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_TradeList );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_TRADELIST;

		siPage = page ;
	}
};

class sDBResponse_WorldMoney_TradeList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPage;

	SI32				siPersonID[MAX_WORLDMONEY_TRADELIST_PER_SEND];
	SI64				siWorldMoney[MAX_WORLDMONEY_TRADELIST_PER_SEND];
	GMONEY				siPrice[MAX_WORLDMONEY_TRADELIST_PER_SEND];
	SI32				siDate[MAX_WORLDMONEY_TRADELIST_PER_SEND];

	sDBResponse_WorldMoney_TradeList()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_TradeList));
	}
};

// 아큐 팔기를 요청한다.
class sDBRequest_WorldMoney_SellOrder
{
public:
	sPacketHeader			packetHeader;

	UI16					usCharId;
	SI32					siPersonID;

	GMONEY					siPrice; // 아큐의 가격 ( 개당 )
	SI64					siWorldMoney; // 올린 아큐

	GMONEY					siFee; // 수수료

	SI32					siDate; // 올린 시간(게임시간)

	sDBRequest_WorldMoney_SellOrder(UI16 charid , SI32 personid , GMONEY price,SI64 worldmoney , GMONEY fee,SI32 date)
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_SellOrder );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_SELLORDER;

		usCharId = charid ;
		siPersonID = personid ;

		siPrice = price ;
		siWorldMoney = worldmoney ;

		siFee = fee ;

		siDate = date ;
	}
};

class sDBResponse_WorldMoney_SellOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharId;
	SI32				siPersonID;

	SI32				siListIndex; // 리스트에서의 인덱스
	GMONEY				siPrice; // 아큐의 가격 ( 개당 )
	SI64				siWorldMoney; // 올린 아큐

	GMONEY				siLeftMoney; //수수료를 제하고 남은 돈
	SI64				siLeftWorldMoney; // 올리고 남은 아큐

	SI32				siDate; // 올린 시간(게임시간)

	// PCK : 로그용 패킷 추가
	GMONEY				siFee;

	sDBResponse_WorldMoney_SellOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_SellOrder));
	}
};

// 아큐 사기를 요청한다.
class sDBRequest_WorldMoney_BuyOrder
{
public:
	sPacketHeader			packetHeader;

	UI16					usCharId;
	SI32					siPersonID;

	SI32					siListIndex;

	SI64					siWorldMoney; // 사기 희망하는 아큐
	GMONEY					siPrice; // 사는 가격

	sDBRequest_WorldMoney_BuyOrder(UI16 charid , SI32 personid , SI32 index,SI64 worldmoney , GMONEY price)
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_BuyOrder );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_BUYORDER;

		usCharId = charid ;
		siPersonID = personid ;

		siListIndex = index ;

		siWorldMoney = worldmoney ;
		siPrice = price ;
	}
};

class sDBResponse_WorldMoney_BuyOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharId;
	SI32				siPersonID;

	SI32				siListIndex; // 리스트에서의 인덱스

	SI64				siWorldMoney; // 산 아큐
	SI64				siLeftListWorldMoney; // 리스트에서의 사고 남은 아큐 - 다 팔렸으면 0

	GMONEY				siPrice; // 산 가격 - 미거래시 0
	GMONEY				siLeftMoney; // 사고 남은 돈
	SI64				siLeftWorldMoney; // 산 뒤의 아큐

	SI32				siSellerPersonID;
	GMONEY				siSellerBankMoney;

	sDBResponse_WorldMoney_BuyOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_BuyOrder));
	}
};

// 올렸던 아큐를 취소해서 회수한다.
class sDBRequest_WorldMoney_CancelOrder
{
public:
	sPacketHeader			packetHeader;

	UI16					usCharId;
	SI32					siPersonID;

	sDBRequest_WorldMoney_CancelOrder(UI16 charid , SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_CancelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_CANCELORDER;

		usCharId = charid ;
		siPersonID = personid ;
	}
};

class sDBResponse_WorldMoney_CancelOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharId;
	SI32				siPersonID;

	SI32				siListIndex; // 리스트에서의 인덱스

	SI64				siLeftWorldMoney; // 취소하고 받은 뒤의 아큐수

	sDBResponse_WorldMoney_CancelOrder()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_CancelOrder));
	}
};

// 아큐의 가격을 떨군다.
class sDBRequest_WorldMoney_ReducePrice
{
public:
	sPacketHeader			packetHeader;

	SI32					siIndex; // 변경할 주문의 인덱스
	GMONEY					siPrice; // 깎은 가격

	SI32					siDate; // 변경된 설정 시간

	SI32					siPersonID; // personid 확인용
	SI64					siWorldMoney; // worldmoney 확인용

	sDBRequest_WorldMoney_ReducePrice(SI32 index , GMONEY price , SI32 date , SI32 personid , SI64 worldmoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_ReducePrice );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_REDUCEPRICE;

		siIndex = index ;
		siPrice = price ;

		siDate = date ;

		siPersonID = personid ;
		siWorldMoney = worldmoney ;
	}
};

class sDBResponse_WorldMoney_ReducePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;

	SI32				siIndex; // 변경된 주문의 인덱스
	GMONEY				siPrice; // 변경된 가격

	SI32				siDate; // 변경된 설정 시간

	sDBResponse_WorldMoney_ReducePrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_ReducePrice));
	}
};

// 아큐를 넣어준다.
class sDBRequest_WorldMoney_Set_Char
{
public:
	sPacketHeader			packetHeader;

	SI32					siCharID;
	SI32					siPersonID;

	SI64					siWorldMoney;

	sDBRequest_WorldMoney_Set_Char(SI32 charid,SI32 personid,SI64 worldmoney)
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_Set_Char );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_SET_CHAR;

		siCharID = charid;
		siPersonID = personid ;

		siWorldMoney = worldmoney ;
	}
};

class sDBResponse_WorldMoney_Set_Char
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI32				siCharID;
	SI32				siPersonID;

	SI64				siWorldMoney;	// 넣어준 아큐
	SI64				siLeftWorldMoney; // 넣어준뒤 남은 아큐값

	sDBResponse_WorldMoney_Set_Char()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_Set_Char));
	}
};

//--------------------------------------------
// TradeServer
//--------------------------------------------

// 트레이드 서버에 주문을 등록했다.
class sDBRequest_TradeServer_SetOrder
{
public:
	sPacketHeader			packetHeader;

	SI32					siCharId;
	SI32					siPersonID;

	cltTradeServerOrder		clOrder; // 주문 내역
	SI16					siPos;

	GMONEY					siWorldMoneyFee; // 물건 수수료

	sDBRequest_TradeServer_SetOrder(SI32 charid,SI32 personid,cltTradeServerOrder*pclOrder,SI16 pos,SI64 fee)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeServer_SetOrder );
		packetHeader.usCmd = DBMSG_REQUEST_TRADESERVER_SETORDER;

		siCharId = charid;
		siPersonID = personid ;

		clOrder.Set(pclOrder);
		siPos = pos ;

		siWorldMoneyFee = fee ;
	}
};

class sDBResponse_TradeServer_SetOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 전부 실패

	SI32				siCharID;
	SI32				siPersonID;

	cltTradeServerOrder	clOrder;
	SI16				siPos;

	cltItem				clLeftItem; // 주문한 뒤 남은 아이템
	SI64				siLeftWorldMoney; // 주문한 뒤 남은 아큐

	// PCK : 로그용 패킷 추가
	GMONEY				siWorldMoneyFee;		// 등록할때 지불하는 수수료

	sDBResponse_TradeServer_SetOrder()
	{
		ZeroMemory(this,sizeof(sDBResponse_TradeServer_SetOrder));
	}
};

//교역 서버에 올린 주문을 취소한다.
class sDBRequest_TradeServer_CancelOrder
{
public:
	sPacketHeader			packetHeader;

	SI32					siCharId;
	SI32					siPersonID;

	cltTradeServerOrder		clOrder; // 주문 내역
	SI16					siPos;

	cltItem					clitem; // 취소한 아이템의 실질 데이타

	sDBRequest_TradeServer_CancelOrder(SI32 charid,SI32 personid,cltTradeServerOrder*pclOrder,SI16 pos,cltItem*pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeServer_CancelOrder );
		packetHeader.usCmd = DBMSG_REQUEST_TRADESERVER_CANCELORDER;

		siCharId = charid;
		siPersonID = personid ;

		clOrder.Set(pclOrder);
		siPos = pos ;

		clitem.Set(pclitem);
	}
};

// 아큐 시세를 저장한다.
class sDBRequest_WorldMoney_SetAveragePrice
{
public:
	sPacketHeader			packetHeader;

	SI64					siAverageWorldMoneyPrice; // 아큐 시세

	sDBRequest_WorldMoney_SetAveragePrice(SI64 price)
	{
		packetHeader.usSize = sizeof( sDBRequest_WorldMoney_SetAveragePrice );
		packetHeader.usCmd = DBMSG_REQUEST_WORLDMONEY_SETAVERAGEPRICE;

		siAverageWorldMoneyPrice = price ;
	}
};

class sDBResponse_WorldMoney_SetAveragePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	GMONEY				siAverageWorldMoneyPrice;

	sDBResponse_WorldMoney_SetAveragePrice()
	{
		ZeroMemory(this, sizeof(sDBResponse_WorldMoney_SetAveragePrice));
	}
};

class sDBResponse_TradeServer_CancelOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 전부 실패

	SI32				siCharID;
	SI32				siPersonID;

	cltTradeServerOrder	clOrder;
	SI16				siPos;

	cltItem				clLeftItem; // 주문한 뒤 남은 아이템

	sDBResponse_TradeServer_CancelOrder()
	{
		ZeroMemory(this,sizeof(sDBResponse_TradeServer_CancelOrder));
	}
};

// 교역 서버에서 물건을 샀다.
class sDBRequest_TradeServer_BuyItem
{
public:
	sPacketHeader			packetHeader;

	SI32					siCharId;
	SI32					siPersonID;

	cltTradeServerOrder		clOrder; // 주문 내역
	char					szSellerServer[20];
	SI16					siPos;

	cltItem					clitem;

	//sDBRequest_TradeServer_BuyItem(SI32 charid,SI32 personid,cltTradeServerOrder*pclOrder,SI16 pos,cltItem*pclitem)
	sDBRequest_TradeServer_BuyItem(SI32 charid,SI32 personid,cltTradeServerOrder*pclOrder,SI16 pos,cltItem*pclitem,char*sellerserver)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeServer_BuyItem );
		packetHeader.usCmd = DBMSG_REQUEST_TRADESERVER_BUYITEM;

		siCharId = charid;
		siPersonID = personid ;

		clOrder.Set(pclOrder);
		siPos = pos ;

		clitem.Set(pclitem);

		MStrCpy(szSellerServer,sellerserver,20);
	}
};

class sDBResponse_TradeServer_BuyItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 전부 실패

	SI32				siCharID;
	SI32				siPersonID;

	cltTradeServerOrder	clOrder;
	SI16				siPos;

	cltItem				clLeftItem; // 주문한 뒤 남은 아이템
	SI64				siLeftWorldMoney;

	sDBResponse_TradeServer_BuyItem()
	{
		ZeroMemory(this,sizeof(sDBResponse_TradeServer_BuyItem));
	}
};

// 교역 서버에서 팔린 물건에 대한 정보를 저장한다.
class sDBRequest_TradeServer_Set_SellInfo
{
public:
	sPacketHeader			packetHeader;

	//	SI32					siPersonID;
	char				szBuyerServer[20] ; // 산 사람이 있는 서버
	char				szBuyerName[20] ; // 산 사람의 이름
	char				szBuyerAccountID[20] ; // 산 사람의 계정

	cltTradeServerOrder		clOrder; // 팔린 주문 내역

	sDBRequest_TradeServer_Set_SellInfo(char*buyserver,char*buyname,char*buyaccount,cltTradeServerOrder*pclOrder)
	{
		packetHeader.usSize = sizeof( sDBRequest_TradeServer_Set_SellInfo );
		packetHeader.usCmd = DBMSG_REQUEST_TRADESERVER_SET_SELLINFO;

		MStrCpy(szBuyerServer,buyserver,20);
		MStrCpy(szBuyerName,buyname,20);
		MStrCpy(szBuyerAccountID,buyaccount,20);

		clOrder.Set(pclOrder);
	}
};

class sDBResponse_TradeServer_Set_SellInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 전부 실패

	//	SI32				siPersonID;
	char				szBuyerServer[20] ; // 산 사람이 있는 서버
	char				szBuyerName[20] ; // 산 사람의 이름
	char				szBuyerAccountID[20] ; // 산 사람의 계정

	cltTradeServerOrder	clOrder;
	SI64				siLeftWorldMoney;

	sDBResponse_TradeServer_Set_SellInfo()
	{
		ZeroMemory(this,sizeof(sDBResponse_TradeServer_Set_SellInfo));
	}
};

// 무역서버에서 저장된 아큐 받기
class sDBRequest_TradeServer_Get_Withdraw_Acu
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID;
	SI32				m_siCharID;

	SI64				m_siLeftSavedAcu;	// 무역서버에 저장된 아큐
	SI64				m_siWithdrawAcu;	// 받을 아큐

	SI32				m_siReservedNum;	// 아큐를 받고 난후 무역서버에 이 번호의 아큐는 지급되었다고 보낼 번호

	sDBRequest_TradeServer_Get_Withdraw_Acu( SI32 siPersonID, SI32 siCharID, SI64 siLeftSavedAcu, SI64 siWithdrawAcu, SI32 siReservedNum )
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_TradeServer_Get_Withdraw_Acu );
		m_packetHeader.usCmd	= DBMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU;

		m_siPersonID			= siPersonID;
		m_siCharID				= siCharID;

		m_siLeftSavedAcu		= siLeftSavedAcu;
		m_siWithdrawAcu			= siWithdrawAcu;

		m_siReservedNum			= siReservedNum;
	}
};

class sDBResponse_TradeServer_Get_Withdraw_Acu
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;

	SI32				m_siPersonID;
	SI32				m_siCharID;

	SI64				m_siLeftSavedAcu;	// 무역서버에 저장된 아큐
	SI64				m_siWithdrawAcu;	// 받을 아큐

	SI64				m_siLeftAcu;		// 변경된 내 아큐

	SI32				m_siReservedNum;	// 이번호는 무역서버로 이 번호의 아큐는 받았다고 보내기 위한용


	sDBResponse_TradeServer_Get_Withdraw_Acu( )
	{
		ZeroMemory( this, sizeof(sDBResponse_TradeServer_Get_Withdraw_Acu) );
	}
};	// DBMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU


//===================================================================================================================
//
// WorldPassport(무역관련 여권)
//
//===================================================================================================================

// 여권을 발행한다

class sDBRequest_WorldPassport_Set
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siPersonID;
	SI32			m_siCharID;
	
	TCHAR			m_szPassportName[Const_Max_WorldPassport_Name];	// 여권이름

	SI32			m_siIssueDateVary;								// 발행일
	SI32			m_siExpireDateVary;								// 유효기간

	SI64			m_siPaidAcu;									// 발급비용

	
	sDBRequest_WorldPassport_Set( SI32 siPersonID, SI32 siCharID, TCHAR* pszPassportName, SI32 siIssueDateVary, SI32 siExpireDateVary, SI64 siPaidAcu )
	{
		packetHeader.usSize	= sizeof( sDBRequest_WorldPassport_Set );
		packetHeader.usCmd	= DBMSG_REQUEST_WORLDPASSPORT_SET;

		m_siPersonID		= siPersonID;
		m_siCharID			= siCharID;

		MStrCpy( m_szPassportName, pszPassportName, Const_Max_WorldPassport_Name );

		m_siIssueDateVary	= siIssueDateVary;
		m_siExpireDateVary	= siExpireDateVary;

		m_siPaidAcu			= siPaidAcu;
	}
	
};

class sDBResponse_WorldPassport_Set
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siResult;										// 결과값

	SI32			m_siPersonID;
	SI32			m_siCharID;

	TCHAR			m_szPassportName[Const_Max_WorldPassport_Name];	// 여권이름

	SI32			m_siIssueDateVary;								// 발행일
	SI32			m_siExpireDateVary;								// 유효기간

	SI64			m_siPaidAcu;									// 발급비용
	SI64			m_siLeftAcu;									// 내가 가진 변경된 아큐


	sDBResponse_WorldPassport_Set()
	{
		ZeroMemory( this, sizeof(sDBResponse_WorldPassport_Set) );
	}

};	// DBMSG_RESPONSE_WORLDPASSPORT_SET

// 여권을 연장한다

class sDBRequest_WorldPassport_Extend
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siPersonID;
	SI32			m_siCharID;

	SI32			m_siExpireDateVary;								// 유효기간

	SI64			m_siPaidAcu;									// 연장비용


	sDBRequest_WorldPassport_Extend( SI32 siPersonID, SI32 siCharID, SI32 siExpireDateVary, SI64 siPaidAcu )
	{
		packetHeader.usSize	= sizeof( sDBRequest_WorldPassport_Extend );
		packetHeader.usCmd	= DBMSG_REQUEST_WORLDPASSPORT_EXTEND;

		m_siPersonID		= siPersonID;
		m_siCharID			= siCharID;

		m_siExpireDateVary	= siExpireDateVary;

		m_siPaidAcu			= siPaidAcu;
	}

};

class sDBResponse_WorldPassport_Extend
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siResult;										// 결과값

	SI32			m_siPersonID;
	SI32			m_siCharID;

	SI32			m_siExpireDateVary;								// 유효기간

	SI64			m_siPaidAcu;									// 연장비용
	SI64			m_siLeftAcu;									// 내가 가진 변경된 아큐


	sDBResponse_WorldPassport_Extend()
	{
		ZeroMemory( this, sizeof(sDBResponse_WorldPassport_Extend) );
	}

};	// DBMSG_RESPONSE_WORLDPASSPORT_EXTEND



#endif

