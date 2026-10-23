#ifndef _DBMSGFOREIGNMERCHANTNPC_H
#define _DBMSGFOREIGNMERCHANTNPC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//-----------------------------------------
// 교역 상인 NPC의 정보를 DB에 저장한다.
//-----------------------------------------
class sDBRequest_SetForeignMerchantNPCInfo
{
public:
	sPacketHeader		packetHeader;

	SI16				siForeignMerchantKind;
	SI16				siItemUnique;
	GMONEY				siPrice;
	SI32				siRestNum;
	cltDate				clDate;

	sDBRequest_SetForeignMerchantNPCInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetForeignMerchantNPCInfo));
	}

	sDBRequest_SetForeignMerchantNPCInfo( SI16 ForeignMerchantKind, SI16 ItemUnique, GMONEY Price, SI32 RestNum, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetForeignMerchantNPCInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETFOREIGNMERCHANTNPCINFO;

		siForeignMerchantKind = ForeignMerchantKind;
		siItemUnique =  ItemUnique;
		siPrice = Price;
		siRestNum = RestNum;
		clDate.Set( pclDate );
	}
};

//-----------------------------------------
// 교역 상인 NPC의 정보를 DB에 저장에 대한 응답
//-----------------------------------------
class sDBResponse_SetForeignMerchantNPCInfo
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.
	
	sDBResponse_SetForeignMerchantNPCInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetForeignMerchantNPCInfo));
	}
};



//-----------------------------------------
// 교역 상인 NPC의 정보를 DB에 요청한다.
//-----------------------------------------
class sDBRequest_GetForeignMerchantNPCInfo
{
public:
	sPacketHeader		packetHeader;

	SI16				siForeignMerchantKind;
	
	sDBRequest_GetForeignMerchantNPCInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetForeignMerchantNPCInfo));
	}

	sDBRequest_GetForeignMerchantNPCInfo( SI16 ForeignMerchantKind )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetForeignMerchantNPCInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_GETFOREIGNMERCHANTNPCINFO;

		siForeignMerchantKind = ForeignMerchantKind;		
	}
};

//-----------------------------------------
// 교역 상인 NPC의 정보를 DB로 부터 받아왔다. 
//-----------------------------------------
class sDBResponse_GetForeignMerchantNPCInfo
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI16				siForeignMerchantKind;
	SI16				siItemUnique;
	GMONEY				siPrice;
	SI32				siRestNum;
	cltDate				clDate;
	
	sDBResponse_GetForeignMerchantNPCInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetForeignMerchantNPCInfo ) );
	}
};

class sDBRequest_ForeignMerchantBuyCashItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
	TCHAR				szAccountID[ MAX_PLAYER_NAME ];

	SI16				siForeignMerchantKind;
	SI32				siForeignMerchantCharID;
	SI32				siForeignMerchantCharUnique;

	SI16				siItemPos;
	cltItem				clSellItem;

	UI16				uiEventMoney;

	sDBRequest_ForeignMerchantBuyCashItem( SI32 CharID, SI32 PersonID, TCHAR *AccountID,
		SI16 ForeignMerchantKind, SI32 ForeignMerchantCharID, SI32 ForeignMerchantCharUnique,
		SI16 ItemPos, cltItem *pclSellItem, UI16 EventMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_ForeignMerchantBuyCashItem );
		packetHeader.usCmd	= DBMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM;

		siCharID = CharID;
		siPersonID = PersonID;		
		memcpy( szAccountID, AccountID, MAX_PLAYER_NAME - 1 );
		szAccountID[ MAX_PLAYER_NAME - 1 ] = NULL;

		siForeignMerchantKind = ForeignMerchantKind;
		siForeignMerchantCharID = ForeignMerchantCharID;
		siForeignMerchantCharUnique = ForeignMerchantCharUnique;

		siItemPos = ItemPos;
		clSellItem.Set( pclSellItem );

		uiEventMoney = EventMoney;
	}
	
};

class sDBResponse_ForeignMerchantBuyCashItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
	TCHAR				szAccountID[ MAX_PLAYER_NAME ];
	
	SI16				siForeignMerchantKind;
	SI32				siForeignMerchantCharID;
	SI32				siForeignMerchantCharUnique;

	SI16				siItemPos;
	cltItem				clLeftItem;

	UI16				uiEventMoney;

	sDBResponse_ForeignMerchantBuyCashItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_ForeignMerchantBuyCashItem ) );
	}
};

/*
//-----------------------------------------
// 사용자의 로그인을 제한/허용한다.
//-----------------------------------------
class sDBRequest_Block
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siBlockMode;		// 0 : 제한, 1: 허용. 

	sDBRequest_Block()
	{
		ZeroMemory(this, sizeof(sDBRequest_Block));
	}

	sDBRequest_Block(UI16 charid, SI32 blockmode)
	{
		packetHeader.usSize = sizeof( sDBRequest_Block );
		packetHeader.usCmd	= DBMSG_REQUEST_BLOCK;

		usCharID	= charid;
		siBlockMode	= blockmode;
	}
};

class sDBResponse_Block
{public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siBlockMode;		// 0 : 제한, 1: 허용. 

	sDBResponse_Block()
	{
		ZeroMemory(this, sizeof(sDBResponse_Block));
	}
};


//-----------------------------------------
// 사용자 숫자를 저장한다. 
//-----------------------------------------
class sDBRequest_SetUserNum
{
public:
	sPacketHeader		packetHeader;

	SI32				siUserNum;

	sDBRequest_SetUserNum()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetUserNum));
	}

	sDBRequest_SetUserNum(SI32 usernum)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetUserNum );
		packetHeader.usCmd	= DBMSG_REQUEST_SETUSERNUM;

		siUserNum		= usernum;
	}
};


//-----------------------------------------
// 시간을 저장하도록 한다. 
//-----------------------------------------
class sDBRequest_SetTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siYesr;
	SI32				siMonth;
	SI32				siDay;
	SI32				siHour;
	SI32				siMinute;

	sDBRequest_SetTime()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetTime));
	}

	sDBRequest_SetTime(SI32 year, SI32 month, SI32 day, SI32 hour, SI32 minute)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetTime );
		packetHeader.usCmd	= DBMSG_REQUEST_SETTIME;

		siYesr		= year;
		siMonth		= month;
		siDay		= day;
		siHour		= hour;
		siMinute	= minute;
	}
};

class sDBResponse_SetTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	sDBResponse_SetTime()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetTime));
	}
};


//-----------------------------------------
// 시간을 얻어오도록 한다. 
//-----------------------------------------
class sDBRequest_GetTime
{
public:
	sPacketHeader		packetHeader;

	sDBRequest_GetTime()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetTime));
		packetHeader.usSize = sizeof( sDBRequest_GetTime );
		packetHeader.usCmd	= DBMSG_REQUEST_GETTIME;

	}
};

class sDBResponse_GetTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siYear;
	SI32				siMonth;
	SI32				siDay;
	SI32				siHour;
	SI32				siMinute;

	sDBResponse_GetTime()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetTime));
	}
};
*/


#endif
