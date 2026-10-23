#ifndef _DBMSGITEMALL_H
#define _DBMSGITEMALL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

#define MAX_ITEMMALL_LIST_NUM		6		// 리스트 하나당 최대 상품 수
#define MAX_ITEMMALL_SET_NUM		10		// 세트 상품에 들어갈 수 있는 최대 아이템 수

// 아이템 몰에서 리스트 불러올 때의 데이타
class cltItemMallListData{
public:
	SI32	siProductKey;		// 상품 코드
	SI32	siGroupKey;			// 그룹 코드

	bool	bGiftFlag;			// 선물가능 코드

	TCHAR	szName[128];		// 상품 이름

	SI32	siPrice;			// 상품 가격
	SI32	siEventPrice;		// 상품 이벤트 가격

	SI32	siSprFont;			// 상품의 이미지 폰트
	TCHAR	szExplain[256];		// 프로시져에서는 title
	TCHAR	szExplain2[256];	//	[종호] 상품에 대한 설명2

	bool	bOnlyBuySilverCoin;	// 은전으로만 구매가 가능하다
	
	bool	bAgeLimit;			//[진성] 거래제한. KOR: 나이, ENG: 레벨 
};

// 아이템 몰에서 세트 상품의 아이템 데이타 - 유니크와 갯수만 있으면 됨
class cltItemMallSetData{
public:
	SI32 siUnique[MAX_ITEMMALL_SET_NUM];
	SI32 siItemNum[MAX_ITEMMALL_SET_NUM];
	SI16 siLimitLevel[MAX_ITEMMALL_SET_NUM];
};

// 아이템 몰에서 그룹 아이템(기간제 아이템들)의 그룹 상세 정보값
class cltItemMallDateItemInfo{
public:
	SI32		siProductKey; // 상품 키값
	TCHAR		szName[128]; // 상품 이름
	SI32		siPrice; // 상품의 가격
};

class sDBRequest_ItemMallExistence
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	sDBRequest_ItemMallExistence(UI16 charid, SI32 personid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLEXISTENCE;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallExistence );

		usCharID		= charid;
		siPersonID		= personid;
	}
};

class sDBResponse_ItemMallExistence
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	bool				bExistence;

	sDBResponse_ItemMallExistence()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMallExistence));
	}
};

class sDBRequest_ItemMallItemList
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siMaxListItemNum;
	SI32				siListPage;

	sDBRequest_ItemMallItemList(UI16 charid, SI32 personid,SI32 listitemnum,SI32 page)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLITEMLIST;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallItemList );

		usCharID			= charid;
		siPersonID			= personid;

		siMaxListItemNum	= listitemnum;
		siListPage			= page;
	}
};

class sDBResponse_ItemMallItemList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siListPage;
	SI32				siTotalItemNum;

	SI32				siItemID[MAX_ITEMNUM_PER_ITEMMALL]; // 아이템 번호
	cltItem				clItem[MAX_ITEMNUM_PER_ITEMMALL];

	sDBResponse_ItemMallItemList()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMallItemList));
	}
};

class sDBRequest_ItemMallGetItem
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem( UI16 charid, SI32 personid,
		SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBRequest_ItemMallGetItem_Global
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_Global( UI16 charid, SI32 personid,
		SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_GLOBAL;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_Global );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBRequest_ItemMallGetItem_China
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_China( UI16 charid, SI32 personid,
		SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_CHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_China );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};


class sDBRequest_ItemMallGetItem_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_Korea( UI16 charid, SI32 personid,
									SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_KOREA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_Korea );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBRequest_ItemMallGetItem_NHNChina
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_NHNChina( UI16 charid, SI32 personid,
									SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_NHNCHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_NHNChina );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBRequest_ItemMallGetItem_USA
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_USA( UI16 charid, SI32 personid,
									SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_USA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_USA );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBRequest_ItemMallGetItem_Taiwan
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_Taiwan(	UI16 charid, SI32 personid,
		SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_TAIWAN;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_Taiwan );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};

class sDBResponse_ItemMallGetItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 인벤토리 위치

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// 아이템 정보

	bool				bPossession;

	sDBResponse_ItemMallGetItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMallGetItem));
	}
};

class sDBRequest_NewsChatAddmission // 자막광고 승인 요청
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템이 있는 인벤토리
	cltItem				clItem;			// 없어져야할 아이템 정보
	
	bool				bUseColorNews;

	TCHAR				szChatText[256];// 채팅 데이타

	sDBRequest_NewsChatAddmission( UI16 charid, SI32 personid,
		SI16 pos,cltItem *pclitem, bool busecolornews ,TCHAR* chattext)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_NEWSCHATADDMISSION;
		packetHeader.usSize = sizeof( sDBRequest_NewsChatAddmission );

		usCharID		= charid;
		siPersonID		= personid;

		siInvPos		= pos;
		clItem.Set(pclitem);

		bUseColorNews = busecolornews;

		memcpy( szChatText, chattext, 256 );
		szChatText[ 255 ] = NULL;
	}
};

class sDBResponse_NewsChatAddmission
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 인벤토리 위치
	cltItem				clItem;			// 남은 아이템 정보
	
	bool				bUseColorNews;

	TCHAR				szChatText[256];// 채팅 데이타

	sDBResponse_NewsChatAddmission()
	{
		ZeroMemory(this, sizeof(sDBResponse_NewsChatAddmission));
	}
};

#define NOLIMIT_SET_TICKET_30			0
#define NOLIMIT_SET_TICKET_90			1
#define NOLIMIT_SET_TICKET_210			2
#define NOLIMIT_SET_TICKET_420			3
#define NOLIMIT_SET_TICKET_900			4
#define NOLIMIT_SET_TICKET_DELETE		5
#define NOLIMIT_SET_TICKET_PREMIUM1		6	// 프리미엄 30일
#define NOLIMIT_SET_TICKET_PREMIUM2		7	// 프리미엄 900일
#define MARKET_SET_TICKET_900			8	// 프리미엄 900일
#define SPACEBOX_1						9	// 공간상자 1일
#define SPACEBOX_7						10	// 공간상자 7일
#define SPACEBOX_30						11	// 공간상자 30일


#define NOLIMIT_TICKET			1
#define MARKET_TICKET			2
#define SPACEBOX_TICKET			3		// 티켓타입 공간상자타입
#define SUMMON_SPACE_TICKET		4		// 티켓타입 소환수 저장확장
#define NewMarket_TICKET		5		// [진성] 통합마켓.

//***************************************************************************************************
//
// Class Name 		: sDBRequest_NoLimitTicket_Warp
//
// Last Modified 	: 2007 / 08 / 09
// Created		 	: 김광명
//
// Function			: 티겟을 이용한 프리미엄 서비스 설정( 순간이동, 객주 이용권)
//
//***************************************************************************************************
class sDBRequest_NoLimitTicket_Warp
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siPersonID;

	bool				bUsing;			// 사용했는가 아니면 끝내는가

	SI16				siInvPos;		// 아이템이 있는 인벤토리
	cltItem				clItem;			// 없어져야할 아이템 정보

	SI32				siDateVary;		// 끝나는 시간

	SI32				siType;			// 설정 종류

	sDBRequest_NoLimitTicket_Warp( bool use , UI16 charid, SI32 personid, SI16 pos,cltItem *pclitem , SI32 datevary, SI32 Type )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_NOLIMITTICKET_WARP;
		packetHeader.usSize = sizeof( sDBRequest_NoLimitTicket_Warp );

		usCharID		= charid ;
		siPersonID		= personid ;

		bUsing			= use ;

		siInvPos		= pos ;
		clItem.Set(pclitem);

		siDateVary		= datevary ;

		siType = Type;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_NoLimitTicket_Warp
//
// Last Modified 	: 2007 / 08 / 09
// Created		 	: 김광명
//
// Function			: 티겟을 이용한 프리미엄 서비스 설정( 순간이동, 객주 이용권)
//
//***************************************************************************************************
class sDBResponse_NoLimitTicket_Warp
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	bool				bUsing;			// 사용했는가 아니면 끝내는가

	SI16				siInvPos;		// 아이템이 있는 인벤토리
	cltItem				clItem;			// 없어져야할 아이템 정보

	SI32				siDateVary;		// 끝나는 시간

	SI32				siType;

	sDBResponse_NoLimitTicket_Warp()
	{
		ZeroMemory(this, sizeof(sDBResponse_NoLimitTicket_Warp));
	}	
};


// 아이템몰에서 대분류와 소분류를 기준으로 상품 리스트를 요청한다.
class sDBRequest_Get_Sel_PGList
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siMainCategory; // 대분류
	SI32				siSubCategory; // 소분류

	TCHAR				szType[4] ; // 상품 구분

	SI32				siPage ; // 불러올 페이지
	SI08				siAllowGrownUpItem;

	sDBRequest_Get_Sel_PGList(UI16 id ,SI32 personid,SI32 main,SI32 sub,TCHAR* type,SI32 page, SI08 AllowGrownUpItem = 0 )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_SEL_PGLIST;
		packetHeader.usSize = sizeof( sDBRequest_Get_Sel_PGList );

		usCharID = id ;
		siPersonID = personid;

		siMainCategory = main ;
		siSubCategory = sub ;

		MStrCpy(szType,type,4) ;

		siPage = page ;

		siAllowGrownUpItem = AllowGrownUpItem;
	}
};

class sDBResponse_Get_Sel_PGList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siMainCategory; // 대분류
	SI32				siSubCategory; // 소분류

	TCHAR				szType[4] ; // 상품 구분

	SI32				siPage ; // 불러온 페이지
	SI32				siMaxPage;

	cltItemMallListData clListData[MAX_ITEMMALL_LIST_NUM];
	cltItemMallSetData clItemData[MAX_ITEMMALL_LIST_NUM];
	
	sDBResponse_Get_Sel_PGList()
	{
		ZeroMemory(this, sizeof(sDBResponse_Get_Sel_PGList));
	}
};

// 선택한 아이템의 상세 정보 및 그룹 아이템 정보를 불러온다.
class sDBRequest_Get_ItemMall_ItemInfo
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siGroupKey; // 그룹키값

	sDBRequest_Get_ItemMall_ItemInfo(UI16 id ,SI32 personid,SI32 key)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_ITEMMALL_ITEMINFO;
		packetHeader.usSize = sizeof( sDBRequest_Get_ItemMall_ItemInfo );

		usCharID = id ;
		siPersonID = personid;

		siGroupKey = key;
	}
};

class sDBResponse_Get_ItemMall_ItemInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	cltItemMallDateItemInfo clItemData[3]; // 정보( 기간제만 아이템들 정보이다. )

	sDBResponse_Get_ItemMall_ItemInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_Get_ItemMall_ItemInfo));
	}
};

// 내 캐쉬 정보를 가져온다.
class SDBRequest_Get_MyCash_Info
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 나의 캐쉬

	SDBRequest_Get_MyCash_Info(UI16 charid , SI32 personid, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}
};

class SDBResponse_Get_MyCash_Info
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;

	SI32				siCash;			// 본 캐쉬 정보
	SI32				siEventCash;	// 이벤트 퀘시

	SDBResponse_Get_MyCash_Info()
	{
		ZeroMemory(this, sizeof(SDBResponse_Get_MyCash_Info));
	}
};

// 아이템 사기
class sDBRequest_ItemMall_Buyitem
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	sDBRequest_ItemMall_Buyitem(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
	}
};

class sDBResponse_ItemMall_Buyitem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;						// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ;					// 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum;					// 구매 수량
	SI32				siProductPrice;					// 주문 금액
	SI32				siProductEventPrice;			// 주문 금액

	SI32				siCash;							// 본 캐쉬 정보
	SI32				siEventCash;					// 이벤트 퀘시

	TCHAR				szTxNoCode[50] ;				// 결제 승인 번호
	TCHAR				szItemName[50];

	sDBResponse_ItemMall_Buyitem()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMall_Buyitem));
	}
};

class sDBResponse_ItemMall_Buyitem_ForXNoti
{
public:
	sPacketHeader		packetHeader;

	SI32				siArrayIndex;
	SI32				siResult ;		// 1외에는 실패

	SI32				siIdx;
	TCHAR				szTxCode[50];

	sDBResponse_ItemMall_Buyitem_ForXNoti()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMall_Buyitem_ForXNoti));
	}
};

// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	
	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	sDBRequest_ItemMall_Giftitem(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;
		
		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;
	}
};

class sDBResponse_ItemMall_Giftitem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	SI32				siCash;			// 본 캐쉬 정보
	SI32				siEventCash;	// 이벤트 퀘시

	TCHAR				szTxNoCode[50] ; // 결제 승인 번호
	TCHAR				szItemName[50];
	

	TCHAR				szComment[200] ; // 선물 메시지
	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	SI32				siGetItemPersonID;	// 선물 받은 사람의 PersonID

	SI32				siHanFailReason;	// [진성] hangame 선물주기에 실패한 이유. => 2008-7-24


	sDBResponse_ItemMall_Giftitem()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMall_Giftitem));
	}
};

class sDBResponse_ItemMall_Giftitem_ForXNoti
{
public:
	sPacketHeader		packetHeader;

	SI32				siArrayIndex;
	SI32				siResult ;		// 1외에는 실패

	SI32				siIdx;
	TCHAR				szTxCode[50];

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지

	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	TCHAR				szSendCharName[MAX_PLAYER_NAME];	//[진성] 선물을 주는 넘의 캐릭터 이름. => 2008-7-24
	
	sDBResponse_ItemMall_Giftitem_ForXNoti()
	{
		ZeroMemory(this, sizeof(sDBResponse_ItemMall_Giftitem_ForXNoti));
	}
};


class sDBRequest_ItemMall_CancelOrder_XNoti
{
public:
	sPacketHeader		packetHeader;

	TCHAR				szTxCode[50];

	sDBRequest_ItemMall_CancelOrder_XNoti(TCHAR* pszTxCode)
	{
		packetHeader.usCmd = DBMSG_REQUEST_ITEMMALL_CANCELORDER_XNOTI;
		packetHeader.usSize = sizeof(sDBRequest_ItemMall_CancelOrder_XNoti);

		ZeroMemory(szTxCode, sizeof(szTxCode));
		if(pszTxCode)	StringCchCopy(szTxCode, sizeof(szTxCode), pszTxCode);
	}
};

//-----------------------------------------------------------
// 글로벌용 구입, 선물 메세지
//-----------------------------------------------------------
// 아이템 사기
class sDBRequest_ItemMall_Buyitem_Global
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	sDBRequest_ItemMall_Buyitem_Global(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_GLOBAL;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_Global );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
	}
};

// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem_Global
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	sDBRequest_ItemMall_Giftitem_Global(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_GLOBAL;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_Global );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;
	}
};


class sDBRequest_ItemMall_Buyitem_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	sDBRequest_ItemMall_Buyitem_Korea(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_KOREA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_Korea );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
	}
};

// [종호_NHN->NDOORS] 아이템몰 잔액조회
class SDBRequest_Get_MyCash_Info_nDoors_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 나의 캐쉬

	SDBRequest_Get_MyCash_Info_nDoors_Korea(UI16 charid , SI32 personid, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_NDOORS_KOREA;
		packetHeader.usSize = sizeof(SDBRequest_Get_MyCash_Info_nDoors_Korea);

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}
};

// [종호_NHN->NDOORS] 아이템몰 잔액조회
class sDBRequest_ItemMall_Buyitem_nDoors_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	sDBRequest_ItemMall_Buyitem_nDoors_Korea(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_NDOORS_KOREA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_nDoors_Korea );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
	}
};

// [종호_NHN->NDOORS] 아이템몰 선물하기
class sDBRequest_ItemMall_Giftitem_nDoors_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	sDBRequest_ItemMall_Giftitem_nDoors_Korea(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_NDOORS_KOREA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_nDoors_Korea );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;
	}
};

class sDBRequest_ItemMall_Buyitem_USA
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szProductName[50];
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	UI32				uiIDNo;		// 한게임용 IDNO - by LEEKH 2007.11.11

	sDBRequest_ItemMall_Buyitem_USA(UI16 charid , SI32 personid ,SI32 key, TCHAR* name, TCHAR* account, SI32 num, SI32 price, TCHAR* ip, TCHAR* itemname, UI32 idno)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_USA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_USA );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID, account, MAX_PLAYER_NAME) ;
		MStrCpy(szProductName, name, sizeof(szProductName)) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);

		uiIDNo			= idno;
	}
};

// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	sDBRequest_ItemMall_Giftitem_Korea(UI16 charid , SI32 personid ,SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_KOREA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_Korea );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;
	}
};

//KHY - 0304 - 중국 NHNChinaBilling - 
// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem_NHNChina
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ;						// 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME];		// 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ;	// 선물 받을 사람 AccountID
	SI32				siProductNum;						// 구매 수량
	SI32				siProductPrice;						// 주문 금액
	SI32				siProductEventPrice;				// 주문 금액

	TCHAR				szComment[200] ;					// 선물 메시지
	TCHAR				szIP[64];							// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	TCHAR				szName[MAX_PLAYER_NAME];
	SI32				siServerID;

	sDBRequest_ItemMall_Giftitem_NHNChina(	UI16	charid, 
											SI32	personid, 
											SI32	key,
											TCHAR*	account,
											TCHAR*	getaccount,
											SI32	num,	 
											SI32	price,	
											SI32	Eventprice,	
											TCHAR*	comment,
											TCHAR*	ip,
											TCHAR*	itemname,
											SI16	GetServerUnique, 
											TCHAR*	GetCharName, 
											SI32	ItemImageFont,
											TCHAR*	Name, 
											SI32	ServerID)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_NHNCHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_NHNChina );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum		= num ;
		siProductPrice		= price ;
		siProductEventPrice = Eventprice;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;

		MStrCpy(szName,Name,MAX_PLAYER_NAME) ;
		siServerID	= ServerID ;
	}
};

// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem_USA
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szProductName[50] ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID 
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];		// 선물을 받을 캐릭터의 이름.
	SI32				siItemImageFont;

	UI32				uiIDNo;		// 한게임용 IDNO - by LEEKH 2007.11.11
	
	sDBRequest_ItemMall_Giftitem_USA(UI16 charid , SI32 personid ,SI32 key, TCHAR* productname, TCHAR* account, TCHAR* getaccount,
									SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont, UI32 idno )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_USA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_USA );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;		
		MStrCpy(szProductName,productname,sizeof(szProductName)) ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName, GetCharName, MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;	
		uiIDNo			= idno;	
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ItemMall_Buyitem_ForXNoti
//
// Last Modified 	: 2007 / 08 / 14
// Created		 	: 이강형
//
// Function			: 웹 상점에서 구매한 아이템을 DB에 추가한다.
//
//***************************************************************************************************
class sDBRequest_ItemMall_Buyitem_ForXNoti
{
public:
	sPacketHeader		packetHeader;

	SI32				siArrayIndex;
	SI32				siIdx;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	TCHAR				szCharName[MAX_PLAYER_NAME];	// 구매 캐릭터 이름
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szTxCode[50];

	SI32				siServiceArea;				// 서비스 지역 - by LEEKH 2007.11.11

	sDBRequest_ItemMall_Buyitem_ForXNoti(SI32 arrayindex, SI32 idx, TCHAR* paccountid, TCHAR* pcharname, SI32 key, SI32 num, SI32 price, TCHAR* ptxcode, SI32 servicearea)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_FORXNOTI;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_ForXNoti );

		siArrayIndex	= arrayindex;
		siIdx			= idx;

		siProductKey	= key ;
		MStrCpy(szMyAccountID, paccountid, MAX_PLAYER_NAME) ;
		MStrCpy(szCharName, pcharname, MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szTxCode, ptxcode, 50);
		siServiceArea	= servicearea;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ItemMall_Giftitem_ForXNoti
//
// Last Modified 	: 2007 / 08 / 14
// Created		 	: 이강형
//
// Function			: 웹 상점에서 구매한 아이템을 DB에 추가한다. 선물하기
//
//***************************************************************************************************
class sDBRequest_ItemMall_Giftitem_ForXNoti
{
public:
	sPacketHeader		packetHeader;

	SI32				siArrayIndex;
	SI32				siIdx;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szTxCode[50];

	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	TCHAR				szSendCharName[MAX_PLAYER_NAME];	//[진성] 선물을 주는 넘의 캐릭터 이름. => 2008-7-24

	sDBRequest_ItemMall_Giftitem_ForXNoti(SI32 arrayindex, SI32 idx, SI32 key,TCHAR* account, TCHAR* getaccount,
		SI32 num, SI32 price, TCHAR* comment, TCHAR* txCode, TCHAR* GetCharName, TCHAR* SendCharName )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_FORXNOTI;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_ForXNoti );

		siArrayIndex	= arrayindex;
		siIdx			= idx ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID, account, MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID, getaccount, MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment, comment, 200) ;
		MStrCpy(szTxCode, txCode, 50) ;

		MStrCpy(szGetCharName,  GetCharName,  MAX_PLAYER_NAME) ;
		MStrCpy(szSendCharName, SendCharName, MAX_PLAYER_NAME) ;
	}
};


//***************************************************************************************************
//
// Class Name 		: sDBRequest_ItemMall_Buyitem_China
//
// Last Modified 	: 2007 / 06 / 08
// Created		 	: 김광명
//
// Function			: 아이템몰에서 물건을 구입할때 사용한다(중국에서만)
//
//***************************************************************************************************
class sDBRequest_ItemMall_Buyitem_China
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액
	SI32				siEventPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	TCHAR				szSecondPassWord[9];

	sDBRequest_ItemMall_Buyitem_China( SI32 charid , SI32 personid ,SI32 key, TCHAR*account,
		                               SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname, TCHAR* SecondPassWord )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_CHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_China );

		siCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
		MStrCpy(szSecondPassWord, SecondPassWord, 9);
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ItemMall_Giftitem_China
//
// Last Modified 	: 2007 / 06 / 08
// Created		 	: 김광명
//
// Function			: 아이템몰에서 물건을 선물할때 사용한다(중국에서만)
//
//***************************************************************************************************
class sDBRequest_ItemMall_Giftitem_China
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	TCHAR				szSecondPassWord[9];


	sDBRequest_ItemMall_Giftitem_China( UI16 charid , SI32 personid ,SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, 
		SI32 ItemImageFont, TCHAR* SecondPassWord )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_CHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_China );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;

		MStrCpy(szSecondPassWord, SecondPassWord, 9);

	}
};

// 내 캐쉬 정보를 가져온다.
class SDBRequest_Get_MyCash_Info_Global
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 나의 캐쉬

	SDBRequest_Get_MyCash_Info_Global(UI16 charid , SI32 personid, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_GLOBAL;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info_Global );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}
};

//***************************************************************************************************
//
// Class Name 		: SDBRequest_Get_MyCash_Info_China
//
// Last Modified 	: 2007 / 06 / 08
// Created		 	: 김광명
//
// Function			: 내 캐시정보를 알아온다(중국용)
//
//***************************************************************************************************
class SDBRequest_Get_MyCash_Info_China
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; 

	SDBRequest_Get_MyCash_Info_China(UI16 charid , SI32 personid, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_CHINA;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info_China );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}
};

//-----------------------------------------------------------

class SDBRequest_Get_MyCash_Info_Korea
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 나의 캐쉬

	SDBRequest_Get_MyCash_Info_Korea(UI16 charid , SI32 personid, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_KOREA;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info_Korea );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}

};

//KHY - 0304 - 중국 NHNChinaBilling - 내 잔액을 알아온다.
class SDBRequest_Get_MyCash_Info_NHNChina
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; 
	SI32 				siServerID;

	SDBRequest_Get_MyCash_Info_NHNChina(UI16 charid , SI32 personid, TCHAR* accountid ,SI32 	ServerID)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_NHNCHINA;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info_NHNChina );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
		siServerID = ServerID;
	}
};

//KHY - 0304 - 중국 NHNChinaBilling - 
class sDBRequest_ItemMall_Buyitem_NHNChina
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siProductKey ;					// 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum;					// 구매 수량
	SI32				siProductPrice;					// 주문 금액
	SI32				siProductEventPrice;			//[진성] 중국 아이템몰 이벤트 주문 금액

	TCHAR				szIP[64];						// IP
	TCHAR				szItemName[50];
	TCHAR				szName[MAX_PLAYER_NAME];
	SI32				siServerID;

	sDBRequest_ItemMall_Buyitem_NHNChina( UI16 charid , 
										  SI32 personid ,
										  SI32 key,
										  TCHAR*account,
										  SI32 num,
										  SI32 price,	
										  SI32 Eventprice,	
										  TCHAR*ip, 
										  TCHAR* itemname , 
										  TCHAR* Name ,
										  SI32 ServerID  )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_NHNCHINA;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_NHNChina );

		usCharID		= charid ;
		siPersonID		= personid ;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum		= num ;
		siProductPrice		= price ;
		siProductEventPrice	= Eventprice ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
		MStrCpy(szName, Name, MAX_PLAYER_NAME);
		siServerID = ServerID;
	}
};


class sDBRequest_Get_MyCash_Info_USA
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 

	UI32				uiIDNo;		// 한게임용 IDNO - by LEEKH 2007.11.11

	sDBRequest_Get_MyCash_Info_USA(UI16 charid , SI32 personid, TCHAR* accountid, UI32 idno)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_USA;
		packetHeader.usSize = sizeof( sDBRequest_Get_MyCash_Info_USA );

		usCharID		= charid ;
		siPersonID		= personid ;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);

		uiIDNo			= idno;
	}

};

// 아이디 검사하기
class sDBRequest_Check_AccountID
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siServerUnique;
	TCHAR				szCharName[MAX_PLAYER_NAME] ; // 선물 받을 계정의 캐릭터 이름

	sDBRequest_Check_AccountID(UI16 charid , SI32 personid , SI16 ServerUnique, TCHAR* CharName)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_CHECK_ACCOUNTID;
		packetHeader.usSize = sizeof( sDBRequest_Check_AccountID );
        
		usCharID		= charid ;
		siPersonID = personid;
		siServerUnique = ServerUnique;
		MStrCpy(szCharName,CharName,MAX_PLAYER_NAME);
	}
};

class sDBResponse_Check_AccountID
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1이면 아이디 존재, 나머지는 아이디 존재하지 않음

	UI16				usCharID;
	SI32				siPersonID;

	SI16				siServerUnique;
	TCHAR				szCharName[MAX_PLAYER_NAME] ;		// 선물 받을 계정의 캐릭터 이름

	TCHAR				szAccountID[MAX_PLAYER_NAME] ;		// 선물 받을 사람 ID
	TCHAR				szAccountName[MAX_PLAYER_NAME] ;	// 선물 받을 사람 이름


	sDBResponse_Check_AccountID()
	{
		ZeroMemory(this, sizeof(sDBResponse_Check_AccountID));
	}
};

#define MAX_NUM_PER_BAGITEM			10

// 복주머니 아이템 설정하기
class sDBRequest_Set_Bagitem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siFromPos; // 사용한 아이템 위치
	cltItem				clFromItem; // 사용한 아이템

	SI16				siPos[MAX_NUM_PER_BAGITEM]; // 들어가야할 위치
	cltItem				clItem[MAX_NUM_PER_BAGITEM]; // 들어가야할 아이템

	sDBRequest_Set_Bagitem(SI32 charid,SI32 personid,SI16 frompos,cltItem*fromitem,SI16*pos,cltItem*pclitem)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_SET_BAGITEM;
		packetHeader.usSize = sizeof( sDBRequest_Set_Bagitem );

		siCharID = charid ;
		siPersonID = personid ;

		siFromPos = frompos ;
		clFromItem.Set(fromitem);

		for ( SI32 i = 0 ; i < MAX_NUM_PER_BAGITEM ; i ++ )
		{
			siPos[i] = pos[i] ;
			clItem[i].Set(&pclitem[i]);
		}
	}
};

class sDBResponse_Set_Bagitem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 모두 실패

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siLeftPos; // 사용한 아이템 위치
	cltItem				clLeftItem; // 사용한 아이템

	SI16				siPos[MAX_NUM_PER_BAGITEM]; // 들어간 위치
	cltItem				clItem[MAX_NUM_PER_BAGITEM]; // 들어간 아이템

	sDBResponse_Set_Bagitem()
	{
		ZeroMemory(this, sizeof(sDBResponse_Set_Bagitem));
	}
};

//-------------------------------------
// 영웅으로 변신하게 만든다.
//--------------------------------------
class sDBRequest_Set_ChangeHero
{
public:
	sPacketHeader	packetHeader;

	SI32			siCharID;
	SI32			siPersonID;

	SI16			siPos;
	cltItem			clitem;

	SI32			siCode;

	sDBRequest_Set_ChangeHero(SI32 charid,SI32 personid,SI16 pos,cltItem*pclitem,SI32 code)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_SET_CHANGEHERO;
		packetHeader.usSize = sizeof( sDBRequest_Set_ChangeHero );

		siCharID = charid;
		siPersonID = personid;

		siPos = pos;
		clitem.Set(pclitem);

		siCode = code;
	}
};

class sDBResponse_Set_ChangeHero
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 모두 실패

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siPos; // 사용한 아이템 위치
	cltItem				clItem; // 사용한 아이템

	SI32				siCode;

	sDBResponse_Set_ChangeHero()
	{
		ZeroMemory(this, sizeof(sDBResponse_Set_ChangeHero));
	}

};

/*
// 선물받은 아이템의 아이디를 근거로 상품 정보를 요청한다.
class sDBRequest_Get_Present_ItemData
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	
	SI32				siItemID;

	sDBRequest_Get_Present_ItemData(UI16 id ,SI32 personid,SI32 siItemID)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_PRESENT_ITEMDATA;
		packetHeader.usSize = sizeof( sDBRequest_Get_Present_ItemData );

		usCharID = id ;
		siPersonID = personid;
		
		siItemID = siItemID;
	}
};


class sDBResponse_Get_Present_ItemData
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult ;		// 1외에는 실패

	UI16				usCharID;
	SI32				siPersonID;

	cltItem				clItemData;


	sDBResponse_Get_Present_ItemData()
	{
		ZeroMemory(this, sizeof(sDBResponse_Get_Present_ItemData));
	}
};
*/


// [종호] 내 캐쉬 정보를 가져온다.
class SDBRequest_Get_MyCash_Info_Germany
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;								//	Character UniqueID
	SI32				siPersonID;								//	PersonID
	SI32				siIDNumber;								//	독일Billing에 필요한 AccountID Number
	TCHAR				szMyAccountID[MAX_PLAYER_NAME];			//	독일Billing에 필요한 AccountID String

	SDBRequest_Get_MyCash_Info_Germany(UI16 charid , SI32 personid, SI32 IDNumber, TCHAR* accountid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_MYCASH_INFO_GERMANY;
		packetHeader.usSize = sizeof( SDBRequest_Get_MyCash_Info_Germany );

		usCharID		=	charid ;
		siPersonID		=	personid ;
		siIDNumber		=	IDNumber;

		MStrCpy(szMyAccountID, accountid, MAX_PLAYER_NAME);
	}
};


//	[종호] 아이템 구매
class sDBRequest_ItemMall_Buyitem_Germany
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	SI32				siIDNumber;	

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szIP[64];	// IP
	TCHAR				szItemName[50];

	sDBRequest_ItemMall_Buyitem_Germany(UI16 charid , SI32 personid , SI32 IDNumber, SI32 key,TCHAR*account,SI32 num,SI32 price,	TCHAR*ip, TCHAR* itemname)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_BUYITEM_GERMANY;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Buyitem_Germany );

		usCharID		= charid ;
		siPersonID		= personid ;
		siIDNumber		= IDNumber;

		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName, itemname, 50);
	}
};


// 아이템 선물하기
class sDBRequest_ItemMall_Giftitem_Germany
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	SI32				siIDNumber;	

	SI32				siProductKey ; // 상품 인덱스
	TCHAR				szMyAccountID[MAX_PLAYER_NAME]; // 구매 아이디

	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductNum; // 구매 수량
	SI32				siProductPrice; // 주문 금액

	TCHAR				szComment[200] ; // 선물 메시지
	TCHAR				szIP[64];		// IP
	TCHAR				szItemName[50];

	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
	SI32				siItemImageFont;

	sDBRequest_ItemMall_Giftitem_Germany(UI16 charid , SI32 personid , SI32 IDNumber, SI32 key,TCHAR*account,TCHAR*getaccount,
		SI32 num,SI32 price,TCHAR* comment,TCHAR*ip,TCHAR* itemname,SI16 GetServerUnique, TCHAR* GetCharName, SI32 ItemImageFont )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALL_GIFTITEM_GERMANY;
		packetHeader.usSize = sizeof( sDBRequest_ItemMall_Giftitem_Germany );

		usCharID		= charid ;
		siPersonID		= personid ;
		siIDNumber		= IDNumber;	
	
		siProductKey	= key ;
		MStrCpy(szMyAccountID,account,MAX_PLAYER_NAME) ;
		MStrCpy(szGetAccountID,getaccount,MAX_PLAYER_NAME) ;
		siProductNum	= num ;
		siProductPrice	= price ;

		MStrCpy(szComment,comment,200) ;
		MStrCpy(szIP,ip,64) ;
		MStrCpy(szItemName,itemname,50) ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME) ;

		siItemImageFont = ItemImageFont;
	}
};

class sDBRequest_ItemMallGetItem_Germany
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;
	SI32				siIDNumber;	

	SI16				siInvPos;		// 아이템을 넣어야 할 위치.

	SI32				siItemID;		// 아이템 번호
	cltItem				clItem;			// Person에 넣을 아이템.

	bool				bPossession;	// 소지할 수있는 거냐 아니냐
	bool				bCreateSID;		// SID 생성 여부

	sDBRequest_ItemMallGetItem_Germany( UI16 charid, SI32 personid, SI16 pos,SI32 itemid,cltItem *pclitem,bool possession,bool createsid )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_ITEMMALLGETITEM_GERMANY;
		packetHeader.usSize = sizeof( sDBRequest_ItemMallGetItem_Germany );

		usCharID		= charid;
		siPersonID		= personid;
		siInvPos		= pos;

		siItemID		= itemid;		
		clItem.Set(pclitem);

		bPossession		= possession ;
		bCreateSID		= createsid;
	}
};
//***************************************************************************************************
//
// Class Name 		: sDBRequest_Warp_TargetWorld
//
// Last Modified 	: 2009 / 11 / 18
// Created		 	: 손성
//
// Function			: 타겟 티켓을 이용해서 해당 지역으로 이동 한다 
//
//***************************************************************************************************
class sDBRequest_Warp_TargetWorld
{
public:
	sPacketHeader		packetHeader;
	
	UI16				usCharID;
	SI32				siPersonID;

	SI16				siInvPos;		// 아이템이 있는 인벤토리
	cltItem				clItem;			// 없어져야할 아이템 정보


	sDBRequest_Warp_TargetWorld(UI16 charid, SI32 personid, SI16 pos,cltItem *pclitem )
	{
		packetHeader.usCmd  = DBMSG_REQUEST_WARP_TARGETWORLD;
		packetHeader.usSize = sizeof( sDBRequest_NoLimitTicket_Warp );

		usCharID		= charid ;
		siPersonID		= personid ;
		siInvPos		= pos ;
		clItem.Set(pclitem);
	}
};
class sDBResponse_TargetWorld
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	SI32				siResult ;		// 1외에는 실패

	SI16				siInvPos;		// 아이템이 있는 인벤토리
	cltItem				clItem;			// 없어져야할 아이템 정보


	sDBResponse_TargetWorld()
	{
		ZeroMemory(this, sizeof(sDBResponse_TargetWorld));
	}	
};

#endif
