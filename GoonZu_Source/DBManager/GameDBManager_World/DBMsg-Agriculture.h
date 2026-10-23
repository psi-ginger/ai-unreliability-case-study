#ifndef _DBMSG_AGRICULTURE_H_
#define _DBMSG_AGRICULTURE_H_

#include "DBMsg.h"
#include "../../GoonZuWorld/Common/Agriculture/Agriculture.h"

class sDBRequest_AgricultureGetProducts
{
public:
	sPacketHeader packetHeader;	

	SI32 siPersonID;
	SI32 siCharID;
	
	// Output
	SI16 siGetProductsInvPos;					// 획득한 물품을 넣을 인벤토리 위치
	cltItem clGetProductsItem;					// 획득한 물품
	bool bCreateSID;							// 획득한 물품의 SID 생성 여부
	
	SI32 siCharUnique;							// 획득한 캐릭 Unique( 사람인지, 소환수인지 구분 )
	SI32 siKind;								// 구분 사람이냐, 소환수냐.
	
	SI16 siAgricultureType;						// 산업 분류( Type )
			
	sDBRequest_AgricultureGetProducts( SI32 PersonID,	SI32 CharID,
		SI16 GetProductsInvPos, cltItem *pGetProductsItem, bool bcreatesid,
		SI32 CharUnique, SI32 Kind,
		SI16 AgricultureType
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_AgricultureGetProducts );
		packetHeader.usCmd	= DBMSG_REQUEST_AGRICULTUREGETPRODUCTS;
		
		siPersonID = PersonID;
		siCharID = CharID;
		
		siGetProductsInvPos = GetProductsInvPos;
		clGetProductsItem.Set( pGetProductsItem );
		bCreateSID = bcreatesid;

		siCharUnique = CharUnique;
		siKind = Kind;

		siAgricultureType = AgricultureType;
	}
};

class sDBResponse_AgricultureGetProducts
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siAddedInvPos;					// 들어간 위치
	SI16 siAddedItemNum;				// 추가된 아이템의 개수
	cltItem clLeftItem;					// 최종 아이템 정보


	SI32 siCharUnique;					// 서버에서 보낸값 그대로 돌려주면 됨
	SI32 siKind;						// 서버에서 보낸값 그대로 돌려주면 됨

	SI16 siAgricultureType;				// 서버에서 보낸값 그대로 돌려주면 됨

	cltItem clGetProductsItem;			// 획득한 물품 for LOG
	
	sDBResponse_AgricultureGetProducts()
	{
		memset( this, 0, sizeof( sDBResponse_AgricultureGetProducts ) );
	}
};

class sDBRequest_AgriculturePayMoneyForRentLand
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	SI16 siVillageUnique;					// 마을 index
	SI16 siStructType;						// 건물의 번호

	GMONEY siPayMoney;						// 지불 금액

	SI16 siAgricultureItemUnique;			// 생산할려는 물품

	UI32 uiLeaseExpire;						// 임시로 빌리는 기간 종료시간(분)

	sDBRequest_AgriculturePayMoneyForRentLand
		(
		SI32 PersonID, SI32 CharID,
		SI16 VillageUnique, SI16 StructType,
		GMONEY PayMoney,
		SI16 AgricultureItemUnique,
		UI32 LeaseExpire
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_AgriculturePayMoneyForRentLand );
		packetHeader.usCmd	= DBMSG_REQUEST_AGRICULTUREPAYMONEYFORRENTLAND;

		siPersonID = PersonID;
		siCharID = CharID;

		siVillageUnique = VillageUnique;
		siStructType = StructType;

		siPayMoney = PayMoney;

		siAgricultureItemUnique = AgricultureItemUnique;

		uiLeaseExpire = LeaseExpire;
	}
};

class sDBResponse_AgriculturePayMoneyForRentLand
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

	SI16 siAgricultureItemUnique;		// 서버에서 보낸값 그대로 돌려주기

	UI32 uiLeaseExpire;						// 임시로 빌리는 기간 종료시간(분)

	sDBResponse_AgriculturePayMoneyForRentLand()
	{
		memset( this, 0, sizeof( sDBResponse_AgriculturePayMoneyForRentLand ) );
	}	
};

class sDBRequest_GetAgricultureLeaseInfo
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;

	sDBRequest_GetAgricultureLeaseInfo( SI32 personid )
	{
		packetHeader.usSize = sizeof(sDBRequest_GetAgricultureLeaseInfo );
		packetHeader.usCmd = DBMSG_REQUEST_GETAGRICULTURELEASEINFO;

		siPersonID = personid;
	}
};

class sDBResponse_GetAgricultureLeaseInfo
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;
	
	SI32 siPersonID;
	SI16 siVillageUnique[MAX_AGRICULTURE_COUNT];
	UI32 uiExpire[MAX_AGRICULTURE_COUNT];

	sDBResponse_GetAgricultureLeaseInfo()
	{
		memset( this, 0 , sizeof(sDBResponse_GetAgricultureLeaseInfo ) ) ;
	}
};

class sDBRequest_ExpiredAgricultureLease
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siVillageUnique;
	UI32 uiExpireTime;

	sDBRequest_ExpiredAgricultureLease( SI32 personid, SI32 villageunique, UI32 expiretime)
	{
		packetHeader.usSize = sizeof(sDBRequest_ExpiredAgricultureLease);
		packetHeader.usCmd = DBMSG_REQUEST_EXPIREDAGRICULTERLEASE;

		siPersonID = personid;
		siVillageUnique = villageunique;
		uiExpireTime = expiretime;
	}
};

class sDBResponse_ExpiredAgricultureLease
{
public:
	sPacketHeader packetHeader;

	SI32	siResult;
	SI32	siPersonID;
	SI32	siVillageUnique;
	UI32	uiExpireTime;

	sDBResponse_ExpiredAgricultureLease()
	{
		memset( this, 0, sizeof(sDBResponse_ExpiredAgricultureLease) );
	}
};
#endif