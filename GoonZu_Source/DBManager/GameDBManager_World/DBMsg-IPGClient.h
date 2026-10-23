#ifndef _DBMSGIPGCLIENT_H
#define _DBMSGIPGCLIENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

#define GIVECASH_MASTER			0			// 운영자 명령 
#define GIVECASH_QUEST			1			// 퀘스트

class sDBRequest_GiveCash
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;

	TCHAR				szUserID[ 50 ];
	SI32				siAmount;

	SI32				siReason;


	sDBRequest_GiveCash( UI16 id, SI32 personid, TCHAR *userid, SI32 amount, SI32 reason ) 
	{
		packetHeader.usCmd = DBMSG_REQUEST_GIVECASH;
		packetHeader.usSize = sizeof( sDBRequest_GiveCash );

		usCharID = id;

		siReason = reason;
		siPersonID = personid;
		
		MStrCpy( szUserID, userid, MAX_PLAYER_NAME );
		siAmount = amount;
	}
};

class sDBResponse_GiveCash
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siPersonID;

	TCHAR				szUserID[ 50 ];
	SI32				siAmount;
	SI32				siReason;

	sDBResponse_GiveCash()
	{
		ZeroMemory( this, sizeof( sDBResponse_GiveCash ) );
	}

};

#endif
