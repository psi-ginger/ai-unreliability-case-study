#ifndef _DBMSG_MOFUMOFUEVENT_H_
#define _DBMSG_MOFUMOFUEVENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "../../GoonZuWorld/common/Event/MofuMofu/MofuMofu.h"

class sDBRequest_MofuMofuEvent_Reserve
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siPersonID;

	SYSTEMTIME			stReserveHour;
	SI32				siVillageUnique;

	GMONEY				EventCost;

public:
	sDBRequest_MofuMofuEvent_Reserve(UI16 id, SI32 personid, SYSTEMTIME& Hour, SI32 villageunique, GMONEY Cost)
		:	usCharID(id),				siPersonID(personid),	
			siVillageUnique(villageunique),
			EventCost(Cost)
	{
		packetHeader.usSize = sizeof( sDBRequest_MofuMofuEvent_Reserve );
		packetHeader.usCmd	= DBMSG_REQUEST_MOFUMOFUEVENT_RESERVE;

		memcpy(&stReserveHour, &Hour, sizeof(stReserveHour));
	}
};

class sDBResponse_MofuMofuEvent_Reserve
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SYSTEMTIME			stReserveHour;
	SI32				siVillageUnique;

	GMONEY				EventCost;

	GMONEY				siCityHallMoney;

public:
	sDBResponse_MofuMofuEvent_Reserve()
	{
		ZeroMemory( this, sizeof( sDBResponse_MofuMofuEvent_Reserve ) );
	}
};


class sDBRequest_MofuMofuEvent_ReserveList
{
public:
	sPacketHeader		packetHeader;

	SYSTEMTIME			stCurrent;

public:
	sDBRequest_MofuMofuEvent_ReserveList(SYSTEMTIME& Cur)
	{
		packetHeader.usSize = sizeof( sDBRequest_MofuMofuEvent_ReserveList );
		packetHeader.usCmd	= DBMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST;

		memcpy(&stCurrent, &Cur, sizeof(stCurrent));
	}
};

class sDBResponse_MofuMofuEvent_ReserveList
{
public:
	sPacketHeader		packetHeader;

	cltMofuMofuEventReserveUnit		ReserveRooms[MOFUMOFUEVENT_ROOMNUM];

public:
	sDBResponse_MofuMofuEvent_ReserveList()
	{
		ZeroMemory( this, sizeof( sDBResponse_MofuMofuEvent_ReserveList ) );
	}
};

class sDBRequest_MofuMofuEvent_End
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	SI32				siSuccess;
	SI32				siAddPoint;

public:
	sDBRequest_MofuMofuEvent_End()
	{
		packetHeader.usSize = sizeof( sDBRequest_MofuMofuEvent_ReserveList );
		packetHeader.usCmd	= DBMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST;
	}
};

#endif
