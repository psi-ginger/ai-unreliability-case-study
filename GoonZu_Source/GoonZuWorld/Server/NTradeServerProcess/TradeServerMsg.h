#ifndef _NTRADESERVERMSG_H
#define _NTRADESERVERMSG_H

#define MAX_TRADESERVER_GET_LIST	50		// 한번에 긁어올 리스트의 최대수
#define MAX_TRADESERVER_ORDER_LIST	1500	// 최대 등록 갯수
#define MAX_TRADESERVER_ACU_LIST	10		// 아큐 최신내역 최대수

// 주문품 올린 시간 - 실제시간
class cltTradeDate
{
public:
	UI08		uiYear;
	UI08		uiMonth;
	UI08		uiDay;
	UI08		uiHour;

public:
	cltTradeDate()
	{
		uiYear = 0 ;
		uiMonth = 0 ;
		uiDay = 0 ;
		uiHour = 0 ;
	}

	void Set(UI08 year, UI08 month, UI08 day, UI08 hour, UI08 minute)
	{
		uiYear		= year;
		uiMonth		= month;
		uiDay		= day;
		uiHour		= hour;
	}
	void Set(cltTradeDate*clinfo)
	{
		uiYear		= clinfo->uiYear ;
		uiMonth		= clinfo->uiMonth ;
		uiDay		= clinfo->uiDay ;
		uiHour		= clinfo->uiHour ;
	}

	void Init()
	{
		uiYear = 0 ;
		uiMonth = 0 ;
		uiDay = 0 ;
		uiHour = 0 ;
	}
};

// 주문품 상세 정보
class cltTradeItem
{
public:
	SI16 siUnique;
	SI16 siItemNum;

	SI16 siType; // 속성 종류
	SI16 siPower; // 속성석 힘

	SI16 siRank; // 주문품 등급
	SI64 siPrice; // 주문품 가격

	SI08 siRareType; // 주문품의 레어도(명품,호품 등)

	char szItemData[24] ;

	cltTradeDate clDate;

	// 투구나 갑옷에 붙은 스킬수(국내무역때문에 추가했다)
	UI08 uiSkillNumber;
	// 장인의망치 횟수(국내무역때문에 추가했다)
	UI08 uiChangeAbilityCount;
	//성능향상 회수(국내무역,나중에 호품,명품에도 추가)
	UI08 uiAbilityUp;

	cltTradeItem()
	{
		siUnique = 0 ;
		siItemNum = 0 ;

		siType = 0;
		siPower = 0;

		siRank = 0 ;
		siPrice = 0 ;

		siRareType = 0 ;

		ZeroMemory(szItemData,sizeof(char) * 24 );

		clDate.Init();

		uiSkillNumber = 0;
		uiChangeAbilityCount = 0;
		uiAbilityUp = 0;
	}

	void Set(cltTradeItem*pcltradeitem)
	{
		siUnique = pcltradeitem->siUnique ;
		siItemNum = pcltradeitem->siItemNum ;

		siType = pcltradeitem->siType;
		siPower = pcltradeitem->siPower ;

		siRank = pcltradeitem->siRank ;
		siPrice = pcltradeitem->siPrice ;

		siRareType = pcltradeitem->siRareType ;

		// 국내에서 바꾼 것 때문에 일단 막고 보자 - 2006/09/18 SJY
//		if ( pcltradeitem->szItemData[0] != NULL && pcltradeitem->szItemData[0] != '\0')
			memcpy(szItemData,pcltradeitem->szItemData,24);
//		else ZeroMemory(szItemData,sizeof(char)*24);


		clDate.Set(&pcltradeitem->clDate);

		uiSkillNumber = pcltradeitem->uiSkillNumber;
		uiChangeAbilityCount = pcltradeitem->uiChangeAbilityCount;
		uiAbilityUp			= pcltradeitem->uiAbilityUp;

	}

	void Init()
	{
		siUnique = 0 ;
		siItemNum = 0 ;

		siType = 0;
		siPower = 0;

		siRank = 0 ;
		siPrice = 0 ;

		siRareType = 0 ;

		ZeroMemory(szItemData,sizeof(char) * 24 );

		clDate.Init();

		uiSkillNumber = 0;
		uiChangeAbilityCount = 0;
		uiAbilityUp	= 0;
	}
};

// 주문 상세 정보
class cltTradeServerOrder
{
public:
	//char szServerName[20]; // 주문 서버
	char szAccountID[20]; // 주문자 계정
	char szCharName[20]; // 주문자 이름

	SI32 siPersonID; // 주문자 personid
	SI16 siCharNation; // 주문자 국가 번호

	cltTradeItem clitem;
	SI32 siIndex; // 주문의 고유값

	char szServerNation[4]; // 서버 국가 코드
	SI16 siServerIndex; // 주문 서버

	cltTradeServerOrder()
	{
		//ZeroMemory(szServerName,sizeof(char) * 20);
		ZeroMemory(szAccountID,sizeof(char) * 20);
		ZeroMemory(szCharName,sizeof(char) * 20);

		siCharNation = 0 ;
		siPersonID = 0 ;

		clitem.Init();

		siIndex = 0 ;

		ZeroMemory(szServerNation,sizeof(char)*4);
		siServerIndex = 0 ;
	}

	void Init()
	{
		//ZeroMemory(szServerName,sizeof(char) * 20);
		ZeroMemory(szAccountID,sizeof(char) * 20);
		ZeroMemory(szCharName,sizeof(char) * 20);

		siCharNation = 0 ;
		siPersonID = 0 ;

		clitem.Init();

		siIndex = 0 ;

		ZeroMemory(szServerNation,sizeof(char)*4);
		siServerIndex = 0 ;
	}

	void Set(cltTradeServerOrder* pclOrder)
	{
		//memcpy(szServerName,pclOrder->szServerName,20);
		memcpy(szAccountID,pclOrder->szAccountID,20);
		memcpy(szCharName,pclOrder->szCharName,20);

		siCharNation = pclOrder->siCharNation ;
		siPersonID = pclOrder->siPersonID ;

		clitem.Set(&pclOrder->clitem);

		siIndex = pclOrder->siIndex ;

		memcpy(szServerNation,pclOrder->szServerNation,4);
		siServerIndex = pclOrder->siServerIndex ;
	}

	bool IsSame(cltTradeServerOrder* pclOrder)
	{
		//if ( strcmp(szServerName,pclOrder->szServerName) )
		//	return false ;
		if ( strcmp(szAccountID,pclOrder->szAccountID) )
			return false ;
		if ( strcmp(szCharName,pclOrder->szCharName) )
			return false ;

		if ( siCharNation != pclOrder->siCharNation )
			return false ;
		if ( siPersonID != pclOrder->siPersonID )
			return false ;
		if ( siIndex != pclOrder->siIndex )
			return false ;

		if ( clitem.siItemNum != pclOrder->clitem.siItemNum )
			return false ;
		if ( clitem.siPower != pclOrder->clitem.siPower )
			return false ;
		if ( clitem.siPrice != pclOrder->clitem.siPrice )
			return false ;
		if ( clitem.siRank != pclOrder->clitem.siRank )
			return false ;
		if ( clitem.siRareType != pclOrder->clitem.siRareType )
			return false ;
		if ( clitem.siType != pclOrder->clitem.siType )
			return false ;
		if ( clitem.siUnique != pclOrder->clitem.siUnique )
			return false ;

		if ( strcmp(clitem.szItemData,pclOrder->clitem.szItemData) != 0 )
			return false ;

		if ( strcmp(szServerNation,pclOrder->szServerNation) )
			return false ;
		if ( siServerIndex != pclOrder->siServerIndex )
			return false ;
		return true ;
	}
};

class cltTradeAcuList
{
public:
	SI16			m_siItemUnique;	// 판매한 아이템 유니크
	SI64			m_siPriceACU;	// 판매된 가격
	cltTradeDate	m_clDate;		// 판매된 날짜


	cltTradeAcuList()
	{
		Init();
	}

	cltTradeAcuList( SI16 siItemUnique, SI64 siPriceACU, cltTradeDate* pclDate )
	{
		Init();

		m_siItemUnique	= siItemUnique;
		m_siPriceACU	= siPriceACU;

		if ( pclDate )
		{
			m_clDate.Set( pclDate );
		}
	}

	cltTradeAcuList( cltTradeAcuList* pclAcuList )
	{
		Init();

		if ( pclAcuList )
		{
			memcpy( this, pclAcuList, sizeof(cltTradeAcuList) );
		}
	}

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltTradeAcuList) );
	}

	bool IsEmpty( void )
	{
		if ( 0 < m_siItemUnique )
		{
			return false;
		}

		if ( 0 < m_siPriceACU )
		{
			return false;
		}

		if ( (0 < m_clDate.uiYear) || (0 < m_clDate.uiMonth) || (0 < m_clDate.uiDay) || (0 < m_clDate.uiHour) )
		{
			return false;
		}

		return true;
	}
};


// Server -> TradeServer
enum {
	NTRADESERVERREQUEST_SET_SELLORDER = 1,		// 주문을 올린다.
	NTRADESERVERRESPONSE_SET_SELLORDER,

	NTRADESERVERREQUEST_CANCEL_SELLORDER,		// 주문을 내린다.
	NTRADESERVERRESPONSE_CANCEL_SELLORDER,

	NTRADESERVERREQUEST_SET_BUYORDER,			// 주문을 산다.
	NTRADESERVERRESPONSE_SET_BUYORDER,

	NTRADESERVERRESPONSE_SET_SELLINFO,			// 팔린 주문의 정보를 팔린 사람이 있는 서버에 보낸다.

	NTRADESERVERREQUEST_DEL_SELLINFO,			// 팔린 주문의 정보를 삭제하라 - 서버에서 처리했다.

	NTRADESERVERREQUEST_GET_LISTINFO,			// 리스트를 요청한다.
	NTRADESERVERRESPONSE_GET_LISTINFO,

	NTRADESERVERREQUEST_DELETE_SELLORDER,		// 주문 올리기는 성공했는데 서버에서 아이템 지우기 실패한 경우에 무역서버에 올라간 것을 지운다

	//-------------------------------------------------------------------
	// [군주해외] 김영훈 20080918 무역개편관련 추가 시작
	//-------------------------------------------------------------------
	NTRADESERVERREQUEST_SET_SELLORDER_HOLDACU,	// 주문을 올린다.(팔린후아큐는 무역서버에 저장된다)
												// 리스폰은 기존의 것을 사용한다
	// 무역서버에 있는 ACU받기
	NTRADESERVERREQUEST_WITHDRAW_ACU,			// 무역서버에 저장된 아큐 회수
	NTRADESERVERRESPONSE_WITHDRAW_ACU,	

	NTRADESERVERREQUEST_WITHDRAW_ACU_SUCCESS,	// 아큐 받았다
												// 리스폰은 필요없다

	NTRADESERVERREQUEST_GET_SAVEDACU_LIST,		// 무역서버에 저장되어있는 ACU/목록 받기
	NTRADESERVERRESPONSE_GET_SAVEDACU_LIST,
	//-------------------------------------------------------------------
	// [군주해외] 김영훈 20080918 무역개편관련 추가 끝
	//-------------------------------------------------------------------

	//-------------------------------------------------------------------
	// [군주해외] 이강형 20090121 무역서버 개편
	//-------------------------------------------------------------------
	NTRADESERVERREQUEST_HEARTBEAT,
	NTRADESERVERRESPONSE_HEARTBEAT,

	NTRADESERVERREQUEST_SET_SELLINFO, // 팔린 주문의 정보를 요청한다.

	NTRADESERVERREQUEST_SET_SERVERVERSION, // 서버의 버젼을 설정한다.
	NTRADESERVERRESPONSE_SET_SERVERVERSION, // 서버의 버젼을 설정한다.
	//-------------------------------------------------------------------

	NTRADESERVERMSG_END
};

// TradeServer(GameProtocol) -> TradeServerDB(DBProcess -> DBProtocol)
enum {
	NTRADESERVERDB_REQUEST_SET_SELLORDER = 1,	// 주문을 올린다.
	NTRADESERVERDB_RESPONSE_SET_SELLORDER,

	NTRADESERVERDB_REQUEST_CANCEL_SELLORDER,	// 주문을 내린다.
	NTRADESERVERDB_RESPONSE_CANCEL_SELLORDER,

	NTRADESERVERDB_REQUEST_SET_BUYORDER,		// 주문을 산다.
	NTRADESERVERDB_RESPONSE_SET_BUYORDER,

	NTRADESERVERDB_REQUEST_SET_SELLINFO,
	NTRADESERVERDB_RESPONSE_SET_SELLINFO,		// 팔린 주문의 정보를 팔린 사람이 있는 서버에 보낸다.

	NTRADESERVERDB_REQUEST_DEL_SELLINFO,		// 팔린 주문의 정보를 삭제하라 - 서버에서 처리했다.

	NTRADESERVERDB_REQUEST_GET_LISTINFO,		// 리스트를 요청한다.
	NTRADESERVERDB_RESPONSE_GET_LISTINFO,

	NTRADESERVERDB_REQUEST_REDUCEPRICE,			// 가격을 깎으라고 요청한다.
	NTRADESERVERDB_RESPONSE_REDUCEPRICE,

	NTRADESERVERDB_REQUEST_DELETE_SELLORDER,	// 주문 올리기는 성공했는데 서버에서 아이템 지우기 실패한 경우에 무역서버에 올라간 것을 지운다

	//-------------------------------------------------------------------
	// [군주해외] 김영훈 20080918 무역개편관련 추가 시작
	//-------------------------------------------------------------------
	NTRADESERVERDB_REQUEST_SET_SELLORDER_HOLDACU,// 주문을 올림과 동시에 이것은 물건이 팔려도 아큐는 무역서버에 있는다

	NTRADESERVERDB_REQUEST_WITHDRAW_ACU,		// 무역서버에 저장된 아큐 회수
	NTRADESERVERDB_RESPONSE_WITHDRAW_ACU,

	NTRADESERVERDB_REQUEST_WITHDRAW_ACU_SUCCESS,// 아큐 회수가 성공하였다
												// 리스폰은 없다

	NTRADESERVERDB_REQUEST_SAVEDACU_LIST,		// 무역서버에 저장된 아큐와 최근 거래내역 정보만 가져온다
	NTRADESERVERDB_RESPONSE_SAVEDACU_LIST,
	//-------------------------------------------------------------------
	// [군주해외] 김영훈 20080918 무역개편관련 추가 끝
	//-------------------------------------------------------------------

	NTRADESERVERDB_MSG_END
};

//-------------------------------------------------------------
// GameServer <-> TradeServer
//--------------------------------------------------------------

// 트레이드 서버에 팔기주문을 올린다.
class NTradeServerRequest_Set_SellOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID; // 캐릭터 ID

	SI16				siPos; // 올릴 주문의 위치

	cltTradeServerOrder	clOrder; // 올릴 주문의 내용

	NTradeServerRequest_Set_SellOrder(SI32 id ,SI16 pos ,cltTradeServerOrder*pclOrder)
	{
		packetHeader.usCmd = NTRADESERVERREQUEST_SET_SELLORDER;
		packetHeader.usSize = sizeof( NTradeServerRequest_Set_SellOrder );

		siCharID = id ;

		siPos = pos ;

		clOrder.Set(pclOrder);
	}
};

class NTradeServerResponse_Set_SellOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID

	SI16				siPos; // 올린 주문의 위치

	cltTradeServerOrder	clOrder; // 올린 주문의 내용

	NTradeServerResponse_Set_SellOrder(SI32 result,SI32 charid,SI16 pos,cltTradeServerOrder*pclOrder)
	{
		packetHeader.usCmd = NTRADESERVERRESPONSE_SET_SELLORDER;
		packetHeader.usSize = sizeof( NTradeServerResponse_Set_SellOrder );

		siResult = result ;

		siCharID = charid ;

		siPos = pos ;

		clOrder.Set(pclOrder);
	}
};
// 팔기주문과 함께 이것은 팔려도 아큐를 받지 않고 무역서버에 저장된다 
class NTradeServerRequest_Set_SellOrder_HoldAcu
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;		// 캐릭터 ID

	SI16				m_siPos;		// 올릴 주문의 위치

	cltTradeServerOrder	m_clOrder;		// 올릴 주문의 내용

	bool				m_bHoldAcu;	// 무역서버에 아큐가 저장될지를 결정한다( 1이상이면 저장 )

	NTradeServerRequest_Set_SellOrder_HoldAcu( SI32 siCharID, SI16 siPos ,cltTradeServerOrder*pclOrder, bool bHoldAcu )
	{
		m_packetHeader.usCmd	= NTRADESERVERREQUEST_SET_SELLORDER_HOLDACU;
		m_packetHeader.usSize	= sizeof( NTradeServerRequest_Set_SellOrder_HoldAcu );

		m_siCharID				= siCharID;

		m_siPos					= siPos;

		m_clOrder.Set(pclOrder);

		m_bHoldAcu				= bHoldAcu;
	}
};

// 트레이드 서버에 올린 주문을 취소한다.
class NTradeServerRequest_Cancel_SellOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID; // 캐릭터 ID

	cltTradeServerOrder	clOrder; // 취소할 주문 내용
	SI16				siPos ; // 취소한 주문이 들어갈 자리

	//NTradeServerRequest_Cancel_SellOrder(SI32 id,cltTradeServerOrder*pclOrder)
	NTradeServerRequest_Cancel_SellOrder(SI32 id,cltTradeServerOrder*pclOrder,SI16 pos)
	{
		packetHeader.usCmd = NTRADESERVERREQUEST_CANCEL_SELLORDER;
		packetHeader.usSize = sizeof( NTradeServerRequest_Cancel_SellOrder );

		siCharID = id ;

		clOrder.Set(pclOrder);
		siPos = pos ;

	}
};

class NTradeServerResponse_Cancel_SellOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID

	cltTradeServerOrder	clOrder; // 취소한 주문 내용
	SI16				siPos;

	NTradeServerResponse_Cancel_SellOrder()
	{
		ZeroMemory( this, sizeof( NTradeServerResponse_Cancel_SellOrder ) );
	}
};

// 트레이드 서버에 올라간 주문을 산다.
class NTradeServerRequest_Set_BuyOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;			// 산 캐릭터 ID
	SI32				siPersonID;			// 산 캐릭터 PersonID

	//char				szServerName[20] ;	// 산 캐릭터가 있는 서버
	char				szServerNation[4] ;	// 산 캐릭터가 있는 서버의 국가 코드
	SI16				siServerIndex;		// 산 캐릭터의 서버 번호
	SI16				siCharNation;		// 캐릭터의 국가 코드
	char				szCharName[20] ;	// 산 캐릭터의 이름
	char				szAccountID[20] ;	// 산 캐릭터의 계정

	cltTradeServerOrder	clOrder;			// 주문 내용
	SI16				siPos;

	//NTradeServerRequest_Set_BuyOrder(SI32 id,SI32 personid,/*char*server,*/char*name,char*account,
	//	char*servernation,SI16 serverindex,SI16 charnation,
	//	cltTradeServerOrder*pclOrder)
	NTradeServerRequest_Set_BuyOrder(SI32 id,SI32 personid,/*char*server,*/char*name,char*account,
		char*servernation,SI16 serverindex,SI16 charnation,
		cltTradeServerOrder*pclOrder,SI16 pos)
	{
		packetHeader.usCmd = NTRADESERVERREQUEST_SET_BUYORDER;
		packetHeader.usSize = sizeof( NTradeServerRequest_Set_BuyOrder );

		siCharID = id ;
		siPersonID = personid ;

		//memcpy(szServerName,server,20);
		memcpy(szServerNation,servernation,4);
		siServerIndex = serverindex;
		siCharNation = charnation;
		memcpy(szCharName,name,20);
		memcpy(szAccountID,account,20);

		clOrder.Set(pclOrder);
		siPos = pos ;

	}
};

class NTradeServerResponse_Set_BuyOrder
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID
	SI32				siPersonID; // 캐릭터 PersonID

	cltTradeServerOrder	clOrder; // 주문 내용
	SI16				siPos;


	NTradeServerResponse_Set_BuyOrder()
	{
		ZeroMemory( this, sizeof( NTradeServerResponse_Set_BuyOrder ) );
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20090210 이강형
//
// 팔린 주문의 정보를 요청한다.
//---------------------------------------------------------------------------
class NTradeServerRequest_Set_SellInfo
{
public:
	sPacketHeader		packetHeader;

	NTradeServerRequest_Set_SellInfo()
	{
		ZeroMemory(this, sizeof(NTradeServerRequest_Set_SellInfo));
		packetHeader.usCmd = NTRADESERVERREQUEST_SET_SELLINFO;
		packetHeader.usSize = sizeof(NTradeServerRequest_Set_SellInfo);
	}
};


// 팔린 주문의 정보를 팔린 사람이 있는 서버에 보낸다.
class NTradeServerResponse_Set_SellInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패

	//char				szBuyerServer[20] ; // 산 사람이 있는 서버
	char				szBuyerServerNation[4] ; // 산 사람의 서버 국가 코드
	SI16				siBuyerServerIndex; // 산 사람의 서버 인덱스
	char				szBuyerName[20] ; // 산 사람의 이름
	char				szBuyerAccountID[20] ; // 산 사람의 계정
	SI16				siBuyerNation; // 산 사람의 국가 인덱스

	cltTradeServerOrder	clLeftOrder; // 팔리고 남은 주문 내역

	NTradeServerResponse_Set_SellInfo()
	{
		ZeroMemory( this, sizeof( NTradeServerResponse_Set_SellInfo ) );
	}
};

// 팔린 주문을 제대로 받았으니 이제 지워라고 요청한다.
class NTradeServerRequest_Del_SellInfo
{
public:
	sPacketHeader		packetHeader;

	cltTradeServerOrder	clOrder; // 지워야할 주문 내역

	NTradeServerRequest_Del_SellInfo(cltTradeServerOrder*pclorder)
	{
		packetHeader.usCmd = NTRADESERVERREQUEST_DEL_SELLINFO;
		packetHeader.usSize = sizeof( NTradeServerRequest_Del_SellInfo );

		clOrder.Set(pclorder);
	}
};

// 중계 서버의 물품 리스트를 요청한다.
class NTradeServerRequest_Get_ListInfo
{
public:
	sPacketHeader		packetHeader;

	//char				szServerName[20] ; // Server 이름 '태조''정종'...
	char				szServerNation[4] ; // 국가 코드 'kor','jpn'...
	SI16				siServerIndex;

	NTradeServerRequest_Get_ListInfo(char* servernation,SI16 serverindex)
	{
		packetHeader.usCmd = NTRADESERVERREQUEST_GET_LISTINFO;
		packetHeader.usSize = sizeof( NTradeServerRequest_Get_ListInfo );

		//memcpy(szServerName,server,20);
		memcpy(szServerNation,servernation,4);
		siServerIndex = serverindex ;
	}
};

class NTradeServerResponse_Get_ListInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패
	SI32				siListStartIndex;

	cltTradeServerOrder	clOrder[MAX_TRADESERVER_GET_LIST];

	NTradeServerResponse_Get_ListInfo()
	{
		ZeroMemory( this, sizeof( NTradeServerResponse_Get_ListInfo ) );
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 저장된 아큐와 최근 아큐내역을 요청한다(정보만 가져옴)
//---------------------------------------------------------------------------
class NTradeServerRequest_Get_SavedACU_List
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;
	SI32				m_siPersonID;

	char				m_szServerNation[4]; // 국가 코드 'kor','jpn'...
	SI16				m_siServerIndex;


	NTradeServerRequest_Get_SavedACU_List( SI32 siCharID, SI32 siPersonID, char* szServerNation, SI16 siServerIndex )
	{
		ZeroMemory( this, sizeof(NTradeServerRequest_Get_SavedACU_List) );

		m_packetHeader.usCmd	= NTRADESERVERREQUEST_GET_SAVEDACU_LIST;
		m_packetHeader.usSize	= sizeof( NTradeServerRequest_Get_SavedACU_List );

		m_siCharID				= siCharID;
		m_siPersonID			= siPersonID;

		memcpy( m_szServerNation, szServerNation, 4 );
		m_siServerIndex			= siServerIndex;
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 저장된 아큐와 최근 아큐내역을 요청 -> 응답
//---------------------------------------------------------------------------
class NTradeServerResponse_Get_SavedACU_List
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;

	SI32				m_siCharID;
	SI32				m_siPersonID;

	SI64				m_siSavedACU;							// 저장된 Acu
	cltTradeAcuList		m_clAcuList[MAX_TRADESERVER_ACU_LIST];	// 최근 거래내역


	NTradeServerResponse_Get_SavedACU_List( )
	{
		ZeroMemory( this, sizeof(NTradeServerResponse_Get_SavedACU_List) );
	}
};	// NTRADESERVERRESPONSE_GET_SAVEDACU_LIST

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 무역서버에 저장된 아큐를 받기 요청
//---------------------------------------------------------------------------
class NTradeServerRequest_Withdraw_Acu
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;
	SI32				m_siPersonID;

	char				m_szServerNation[4]; // 국가 코드 'kor','jpn'...
	SI16				m_siServerIndex;

	SI64				m_siWithdrawAcu;	// 받을 아큐


	NTradeServerRequest_Withdraw_Acu( SI32 siCharID, SI32 siPersonID, char* szServerNation, SI16 siServerIndex, SI64 siWithdrawAcu )
	{
		ZeroMemory( this, sizeof(NTradeServerRequest_Withdraw_Acu) );

		m_packetHeader.usCmd	= NTRADESERVERREQUEST_WITHDRAW_ACU;
		m_packetHeader.usSize	= sizeof( NTradeServerRequest_Withdraw_Acu );

		m_siCharID				= siCharID;
		m_siPersonID			= siPersonID;

		memcpy( m_szServerNation, szServerNation, 4 );
		m_siServerIndex			= siServerIndex;

		m_siWithdrawAcu			= siWithdrawAcu;
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 무역서버에 저장된 아큐를 받기 요청 -> 응답
//---------------------------------------------------------------------------
class NTradeServerResponse_Withdraw_Acu
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;

	SI32				m_siCharID;
	SI32				m_siPersonID;

	SI64				m_siLeftSavedAcu;	// 무역서버에 저장된 아큐
	SI64				m_siWithdrawAcu;	// 받을 아큐

	SI32				m_siReservedNum;	// 성공 메시지를 보낼때 이 번호의 아큐를 차감해라


	NTradeServerResponse_Withdraw_Acu()
	{
		ZeroMemory( this, sizeof(NTradeServerResponse_Withdraw_Acu) );
	}
};	// NTRADESERVERRESPONSE_WITHDRAW_ACU

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 무역서버에 아큐를 성공적으로 받았음을 알려준다
//---------------------------------------------------------------------------
class NTradeServerRequest_Withdraw_Acu_Success
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siReservedNum;


	NTradeServerRequest_Withdraw_Acu_Success( SI32 siReservedNum )
	{
		ZeroMemory( this, sizeof(NTradeServerRequest_Withdraw_Acu_Success) );

		m_packetHeader.usCmd	= NTRADESERVERREQUEST_WITHDRAW_ACU_SUCCESS;
		m_packetHeader.usSize	= sizeof( NTradeServerRequest_Withdraw_Acu_Success );

		m_siReservedNum			= siReservedNum;
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20090210 이강형
//
// HeartBeat 패킷
//---------------------------------------------------------------------------
class NTradeServerRequest_HeartBeat		// 게임서버 -> 무역서버
{
public:
	sPacketHeader	m_packetHeader;

	NTradeServerRequest_HeartBeat()
	{
		ZeroMemory(this, sizeof(NTradeServerRequest_HeartBeat));
		m_packetHeader.usCmd = NTRADESERVERREQUEST_HEARTBEAT;
		m_packetHeader.usSize = sizeof( NTradeServerRequest_HeartBeat );
	}
};

class NTradeServerResponse_HeartBeat	// 무역서버 -> 게임서버
{
public:
	sPacketHeader		m_packetHeader;

	NTradeServerResponse_HeartBeat()
	{
		ZeroMemory(this, sizeof(NTradeServerResponse_HeartBeat));
		m_packetHeader.usCmd = NTRADESERVERRESPONSE_HEARTBEAT;
		m_packetHeader.usSize = sizeof( NTradeServerResponse_HeartBeat );
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20090210 이강형
//
// 서버 버젼 설정 패킷
//---------------------------------------------------------------------------
class NTradeServerRequest_Set_ServerVersion		// 게임서버 -> 무역서버
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siVersion;

	NTradeServerRequest_Set_ServerVersion(SI32 siVersion)
	{
		ZeroMemory(this, sizeof(NTradeServerRequest_Set_ServerVersion));
		m_packetHeader.usCmd = NTRADESERVERREQUEST_SET_SERVERVERSION;
		m_packetHeader.usSize = sizeof( NTradeServerRequest_Set_ServerVersion );

		m_siVersion = siVersion;
	}
};

class NTradeServerResponse_Set_ServerVersion	// 무역서버 -> 게임서버
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siVersion;

	NTradeServerResponse_Set_ServerVersion(SI32 siVersion)
	{
		ZeroMemory(this, sizeof(NTradeServerResponse_Set_ServerVersion));
		m_packetHeader.usCmd = NTRADESERVERRESPONSE_SET_SERVERVERSION;
		m_packetHeader.usSize = sizeof( NTradeServerResponse_Set_ServerVersion );

		m_siVersion = siVersion;
	}
};


//-------------------------------------------------------------
// TradeServer <-> TradeServerDB
//--------------------------------------------------------------

// 트레이드 서버에 팔기주문을 올린다.
class NTradeServerDB_Request_Set_SellOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siCharID; // 캐릭터 ID

	SI16				siPos; // 올릴 주문의 위치

	cltTradeServerOrder	clOrder; // 올릴 주문의 내용

	NTradeServerDB_Request_Set_SellOrder(UI16 serverid ,SI32 id ,SI16 pos ,cltTradeServerOrder*pclOrder)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_SET_SELLORDER;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_Set_SellOrder );

		uiServerIndex = serverid ;

		siCharID = id ;

		siPos = pos ;

		clOrder.Set(pclOrder);
	}
};

class NTradeServerDB_Response_Set_SellOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID

	SI16				siPos; // 올린 주문의 위치

	cltTradeServerOrder	clOrder; // 올린 주문의 내용

	NTradeServerDB_Response_Set_SellOrder()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_Set_SellOrder ) );
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 주문을 올림과 동시에 이것은 팔려도 아큐는 무역서버에 그냥 있음을 의미
//
// (리스폰은 NTradeServerDB_Response_Set_SellOrder을 쓴다)
//---------------------------------------------------------------------------
class NTradeServerDB_Request_Set_SellOrder_HoldAcu
{
public:
	sPacketHeader		m_packetHeader;

	UI16				m_uiServerIndex;

	SI32				m_siCharID;			// 캐릭터 ID

	SI16				m_siPos;			// 올릴 주문의 위치

	cltTradeServerOrder	m_clOrder;			// 올릴 주문의 내용

	bool				m_bHodlAcu;			// 무역서버에 아큐가 저장될지를 결정한다( 1이상이면 저장 )

	NTradeServerDB_Request_Set_SellOrder_HoldAcu(UI16 siServerID, SI32 siCharID, SI16 siPos, cltTradeServerOrder*pclOrder, bool bHoldAcu)
	{
		m_packetHeader.usCmd	= NTRADESERVERDB_REQUEST_SET_SELLORDER_HOLDACU;
		m_packetHeader.usSize	= sizeof( NTradeServerDB_Request_Set_SellOrder_HoldAcu );

		m_uiServerIndex			= siServerID;

		m_siCharID				= siCharID;

		m_siPos					= siPos;

		m_clOrder.Set(pclOrder);

		m_bHodlAcu				= bHoldAcu;
	}
};

// 트레이드 서버에 올린 주문을 취소한다.
class NTradeServerDB_Request_Cancel_SellOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siCharID; // 캐릭터 ID

	cltTradeServerOrder	clOrder; // 취소할 주문 내용
	SI16				siPos;

	//NTradeServerDB_Request_Cancel_SellOrder(UI16 serverid ,SI32 id ,cltTradeServerOrder*pclOrder)
	NTradeServerDB_Request_Cancel_SellOrder(UI16 serverid ,SI32 id ,cltTradeServerOrder*pclOrder,SI16 pos)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_CANCEL_SELLORDER;
		packetHeader.usSize = sizeof( NTradeServerRequest_Cancel_SellOrder );

		uiServerIndex = serverid ;

		siCharID = id ;

		clOrder.Set(pclOrder);
		siPos = pos ;

	}
};

class NTradeServerDB_Response_Cancel_SellOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID

	cltTradeServerOrder	clOrder; // 취소한 주문 내용
	SI16				siPos ;

	NTradeServerDB_Response_Cancel_SellOrder()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_Cancel_SellOrder ) );
	}
};

// 트레이드 서버에 올라간 주문을 산다.
class NTradeServerDB_Request_Set_BuyOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siCharID; // 산 캐릭터 ID
	SI32				siPersonID; // 산 캐릭터 PersonID

	//char				szServerName[20] ; // 산 캐릭터가 있는 서버
	char				szServerNation[4] ; // 산 캐릭터가 있는 서버의 국가 코드
	SI16				siServerIndex; // 산 캐릭터의 서버 번호
	SI16				siCharNation; // 캐릭터의 국가 코드
	char				szCharName[20] ; // 산 캐릭터의 이름
	char				szAccountID[20] ; // 산 캐릭터의 계정

	cltTradeServerOrder	clOrder; // 주문 내용
	SI16				siPos;

	//NTradeServerDB_Request_Set_BuyOrder(UI16 serverid ,SI32 id ,SI32 personid,/*char*server,*/char*name,char*account,
	//	char*servernation,SI16 serverindex,SI16 charnation,
	//	cltTradeServerOrder*pclOrder)
	NTradeServerDB_Request_Set_BuyOrder(UI16 serverid ,SI32 id ,SI32 personid,/*char*server,*/char*name,char*account,
		char*servernation,SI16 serverindex,SI16 charnation,
		cltTradeServerOrder*pclOrder,SI16 pos)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_SET_BUYORDER;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_Set_BuyOrder );

		uiServerIndex = serverid ;

		siCharID = id ;
		siPersonID = personid ;

		//memcpy(szServerName,server,20);
		memcpy(szServerNation,servernation,4);
		siServerIndex = serverindex;
		siCharNation = charnation;
		memcpy(szCharName,name,20);
		memcpy(szAccountID,account,20);

		clOrder.Set(pclOrder);
		siPos = pos ;

	}
};

class NTradeServerDB_Response_Set_BuyOrder
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siResult; // 1외에는 모두 실패

	SI32				siCharID; // 캐릭터 ID
	SI32				siPersonID; // 캐릭터 PersonID

	char				szServerNation[4] ; // 산 캐릭터가 있는 서버의 국가 코드
	SI16				siServerIndex; // 산 캐릭터의 서버 번호
	char				szCharName[20] ; // 산 캐릭터의 이름

	cltTradeServerOrder	clOrder; // 주문 내용
	SI16				siPos;

	NTradeServerDB_Response_Set_BuyOrder()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_Set_BuyOrder ) );
	}
};

class NTradeServerDB_Request_Set_SellInfo // 서버가 연결되어 있으니까 SetSellInfo를 실행하라
{
public:
	sPacketHeader		packetHeader;

	char				szServerIP[20]; // 연결되어 있는 서버 IP

	NTradeServerDB_Request_Set_SellInfo(char*ip)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_SET_SELLINFO;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_Set_SellInfo );

		memcpy(szServerIP,ip,20);
	}
};

// 팔린 주문의 정보를 팔린 사람이 있는 서버에 보낸다.
class NTradeServerDB_Response_Set_SellInfo
{
public:
	sPacketHeader		packetHeader;

	char				szServerIP[20]; // 판 사람이 있는 서버 IP

	SI32				siResult; // 1외에는 모두 실패

	//char				szBuyerServer[20] ; // 산 사람이 있는 서버
	char				szBuyerServerNation[4] ; // 산 사람의 서버 국가 코드
	SI16				siBuyerServerIndex; // 산 사람의 서버 인덱스
	char				szBuyerName[20] ; // 산 사람의 이름
	char				szBuyerAccountID[20] ; // 산 사람의 계정
	SI16				siBuyerNation; // 산 사람의 국가 인덱스

	cltTradeServerOrder	clLeftOrder; // 판 주문의 상세 정보 - 판매자의 정보(personid,servername등)

	NTradeServerDB_Response_Set_SellInfo()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_Set_SellInfo ) );
	}
};

// 팔린 주문의 정보를 삭제하라 - 서버에서 처리했다.
class NTradeServerDB_Request_Del_SellInfo
{
public:
	sPacketHeader		packetHeader;

	cltTradeServerOrder	clOrder; // 삭제할 주문 내역

	NTradeServerDB_Request_Del_SellInfo(cltTradeServerOrder*pclOrder)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_DEL_SELLINFO;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_Del_SellInfo );

		clOrder.Set(pclOrder);
	}
};

// 중계 서버의 물품 리스트를 요청한다.
class NTradeServerDB_Request_Get_ListInfo
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siListStartIndex; // 리스트 스타트 인덱스

	bool				bDBInit; // 무역 서버 프로그램이 처음에 요청하는거냐?

	NTradeServerDB_Request_Get_ListInfo(UI16 serverid ,SI32 index,bool dbinit)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_GET_LISTINFO;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_Get_ListInfo );

		uiServerIndex = serverid ;

		siListStartIndex = index ;

		bDBInit = dbinit;
	}
};

class NTradeServerDB_Response_Get_ListInfo
{
public:
	sPacketHeader		packetHeader;

	UI16				uiServerIndex;

	SI32				siResult; // 1외에는 모두 실패
	SI32				siListStartIndex;

	cltTradeServerOrder	clOrder[MAX_TRADESERVER_GET_LIST];

	bool				bDBInit; // 무역 서버 프로그램이 처음에 요청하는거냐?

	NTradeServerDB_Response_Get_ListInfo()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_Get_ListInfo ) );
	}
};

// 중계 서버의 물품의 가격을 내린다.
class NTradeServerDB_Request_ReducePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siIndex;
	SI64				siPrice;

	cltTradeDate		clDate;
	char				szSellerName[20];
	char				szSellerServerNation[20];
	SI32				siSellerServerIndex;

	SI32				siItemUnique;

	NTradeServerDB_Request_ReducePrice(SI32 index , SI64 price, cltTradeDate*date,char*name,char*server,SI32 serverindex,SI32 itemunique)
	{
		packetHeader.usCmd = NTRADESERVERDB_REQUEST_REDUCEPRICE;
		packetHeader.usSize = sizeof( NTradeServerDB_Request_ReducePrice );

		siIndex = index;
		siPrice = price;

		clDate.Set(date);
		memcpy(szSellerName,name,20);
		memcpy(szSellerServerNation,server,20);
		siSellerServerIndex = serverindex ;

		siItemUnique = itemunique ;
	}
};

class NTradeServerDB_Response_ReducePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult; // 1외에는 모두 실패

	cltTradeServerOrder	clOrder;

	NTradeServerDB_Response_ReducePrice()
	{
		ZeroMemory( this, sizeof( NTradeServerDB_Response_ReducePrice ) );
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 무역서버에 저장된 아큐 받기를 요청한다
//
// (지금 시점에서 아큐가 소모되는 것은 아니고 나중에 Success패킷이 전달되야 소모된다)
//---------------------------------------------------------------------------
class NTradeServerDB_Request_Withdraw_Acu
{
public:
	sPacketHeader	m_packetHeader;

	UI16			m_uiServerIndex;

	SI32			m_siCharID;
	SI32			m_siPersonID;

	char			m_szServerNation[4];	// 국가코드
	SI16			m_siServerIndex;		// 서버 인덱스

	SI64			m_siWithdrawAcu;		// 받을 아큐


	NTradeServerDB_Request_Withdraw_Acu( UI16 uiServerIndex, SI32 siCharID, SI32 siPersonID, char* pszServerNation, SI16 siServerIndex, SI64 siWithdrawAcu )
	{
		m_packetHeader.usCmd	= NTRADESERVERDB_REQUEST_WITHDRAW_ACU;
		m_packetHeader.usSize	= sizeof( NTradeServerDB_Request_Withdraw_Acu );

		m_uiServerIndex			= uiServerIndex;

		m_siCharID				= siCharID;
		m_siPersonID			= siPersonID;

		memcpy( m_szServerNation, pszServerNation, 4 );
		m_siServerIndex			= siServerIndex;

		m_siWithdrawAcu			= siWithdrawAcu;
	}
};

class NTradeServerDB_Response_Withdraw_Acu
{
public:
	sPacketHeader	m_packetHeader;

	UI16			m_uiServerIndex;
	SI32			m_siResult;

	SI32			m_siCharID;
	SI32			m_siPersonID;

	SI64			m_siWithdrawAcu;		// 받은 아큐
	SI64			m_siLeftSavedAcu;		// 받고 난 후 저장된 아큐

	SI32			m_siReservedNum;		// 성공한후 이 번호의 아큐를 차감하기 위한 변수


	NTradeServerDB_Response_Withdraw_Acu()
	{
		ZeroMemory( this, sizeof(NTradeServerDB_Response_Withdraw_Acu) );
	}
};	// NTRADESERVERDB_RESPONSE_WITHDRAW_ACU

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 아큐 받기를 성공 알림
//
// (위에서 받기 예약되 있던 아큐를 감소한다)
//---------------------------------------------------------------------------
class NTradeServerDB_Request_Withdraw_Acu_Success
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siReservedNum;

	NTradeServerDB_Request_Withdraw_Acu_Success( SI32 siReservedNum )
	{
		m_packetHeader.usCmd	= NTRADESERVERDB_REQUEST_WITHDRAW_ACU_SUCCESS;
		m_packetHeader.usSize	= sizeof( NTradeServerDB_Request_Withdraw_Acu_Success );

		m_siReservedNum			= siReservedNum;
	}
};

//---------------------------------------------------------------------------
// [군주해외] 20080918 김영훈
//
// 무역서버에 저장된 ACU와 최신 거래내역 정보만 요청한다
//---------------------------------------------------------------------------
class NTradeServerDB_Request_SavedAcu_List
{
public:
	sPacketHeader	m_packetHeader;

	UI16			m_uiServerIndex;

	SI32			m_siCharID;
	SI32			m_siPersonID;

	char			m_szServerNation[4];	// 국가코드
	SI16			m_siServerIndex;		// 서버 인덱스


	NTradeServerDB_Request_SavedAcu_List( UI16 uiServerIndex, SI32 siCharID, SI32 siPersonID, char* pszServerNation, SI16 siServerIndex )
	{
		m_packetHeader.usCmd	= NTRADESERVERDB_REQUEST_SAVEDACU_LIST;
		m_packetHeader.usSize	= sizeof( NTradeServerDB_Request_SavedAcu_List );

		m_uiServerIndex			= uiServerIndex;

		m_siCharID				= siCharID;
		m_siPersonID			= siPersonID;

		memcpy( m_szServerNation, pszServerNation, 4 );
		m_siServerIndex			= siServerIndex;
	}
};

class NTradeServerDB_Response_SavedAcu_List
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;
	UI16			m_uiServerIndex;

	SI32			m_siCharID;
	SI32			m_siPersonID;

	SI64			m_siSavedAcu;
	cltTradeAcuList	m_clAcuList[MAX_TRADESERVER_ACU_LIST];	// 최근 거래내역


	NTradeServerDB_Response_SavedAcu_List( )
	{
		ZeroMemory( this, sizeof(NTradeServerDB_Response_SavedAcu_List) );
	}
};	// NTRADESERVERDB_RESPONSE_SAVEDACU_LIST


#endif