#ifndef _DBMSG_FARMING_H_
#define _DBMSG_FARMING_H_

#include "DBMsg.h"
#include "../../BeTheRich/Common/Farming/FarmingMgr.h"

class sDBRequest_FarmingPayMoneyForRentLand
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siVillageUnique;				// 마을 index
	SI16 siStructType;					// 건물의 번호 ( 이 경우는 무조건 농장임 ) - RANKTYPE_LAND

	GMONEY siPayMoney;					// 지불 금액
	
	SI16 siFarmingItemUnique;			// 수확할려는 물품

	sDBRequest_FarmingPayMoneyForRentLand
		(
		SI32 PersonID, SI32 CharID,
		SI16 VillageUnique, SI16 StructType,
		GMONEY PayMoney,
		SI16 FarmingItemUnique
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_FarmingPayMoneyForRentLand );
		packetHeader.usCmd	= DBMSG_REQUEST_FARMINGPAYMONEYFORRENTLAND;

		siPersonID = PersonID;
		siCharID = CharID;
		
		siVillageUnique = VillageUnique;
		siStructType = StructType;

		siPayMoney = PayMoney;

		siFarmingItemUnique = FarmingItemUnique;
	}
};

class sDBResponse_FarmingPayMoneyForRentLand
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;						// 결과 : 1 성공, Other : 실패

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siVillageUnique;				// 서버에서 보낸값 그대로 돌려주면 됨
	SI16 siStructType;					// 서버에서 보낸값 그대로 돌려주면 됨
	
	GMONEY siPayMoney;					// 지불 금액
	GMONEY siLeftMoney;					// 지불하고 남은 금액

	SI16 siFarmingItemUnique;			// 서버에서 보낸값 그대로 돌려주기

	sDBResponse_FarmingPayMoneyForRentLand()
	{
		memset( this, 0, sizeof( sDBResponse_FarmingPayMoneyForRentLand ) );
	}	
};

#endif