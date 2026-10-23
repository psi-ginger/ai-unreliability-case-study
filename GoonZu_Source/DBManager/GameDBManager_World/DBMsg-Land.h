#ifndef _DBMSGLAND_H
#define _DBMSGLAND_H

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Land\Land.h"

#define LANDRECALL_REASON_FEEDELAY			1	// 연체로 농장 회수 .
#define LANDRECALL_REASON_CONTRACTEND		2	// 계약 종료로 농장 회수. 

//-----------------------------------------
// 단위 농장의 정보를 얻어온다. 
//-----------------------------------------
class sDBRequest_ManyLandUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;

	sDBRequest_ManyLandUnit()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyLandUnit));
	}

	sDBRequest_ManyLandUnit(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_ManyLandUnit );
		packetHeader.usCmd	= DBMSG_REQUEST_MANYLANDUNIT;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};

class sDBResponse_ManyLandUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;								// 결과값.

	SI32				siVillageUnique;						// 대상이 되는 농장이 위치한 마을의 Unique
	cltLandUnit			clLandUnit[ MAX_LANDUNIT_NUMBER ];		// 한 마을에 있는 단위 농장 전부의 정보를 담는다. 	

	sDBResponse_ManyLandUnit()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyLandUnit));
	}
};


//-----------------------------------------
// 농장의 계약 조건을 변경하도록 한다. 
//-----------------------------------------
class sDBRequest_LandChangeContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 대상이 되는 농장이 위치한 마을
	cltRentContract		clContract;				// 계약 조건
	
	cltDate				clDate;					// 계약 변경일

	sDBRequest_LandChangeContract()
	{
		ZeroMemory(this, sizeof(sDBRequest_LandChangeContract));
	}

	sDBRequest_LandChangeContract(SI32 id, SI32 villageunique, cltRentContract* pclcontract, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_LandChangeContract );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_CHANGECONTRACT;

		usCharID		= id;
		siVillageUnique	= villageunique;

		clContract.Set(pclcontract);
		clDate.Set( pclDate );
	}
};


class sDBResponse_LandChangeContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;				// 결과값.

	SI32				siVillageUnique;		// 대상이 되는 농장이 위치한 마을
	cltRentContract		clContract;				// 변경된 계약 조건

	cltDate				clDate;					// 계약 변경일

	sDBResponse_LandChangeContract()
	{
		ZeroMemory(this, sizeof(sDBResponse_LandChangeContract));
	}
};



//-----------------------------------------
// 농장 임차 신청한다. 
//-----------------------------------------
class sDBRequest_LandRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 대상이 되는 농장이 위치한 마을
	SI32				siPersonID;				// 신청자.
	
	cltRentContract		clContract;				// 계약정보

	sDBRequest_LandRentApply()
	{
		ZeroMemory(this, sizeof(sDBRequest_LandRentApply));
	}

	sDBRequest_LandRentApply( SI32 id, SI32 villageunique, SI32 personid, cltRentContract* pclcontract )
	{
		packetHeader.usSize = sizeof( sDBRequest_LandRentApply );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_RENTAPPLY;

		usCharID		= id;
		siVillageUnique	= villageunique;
		siPersonID		= personid;

		clContract.Set(pclcontract);
	}
};


class sDBResponse_LandRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 농장이 위치한 마을
	
	SI32				siIndex;			// 임차 희망자의 정보가 담긴 위치를 가리킨다.
	cltSimplePerson		clApplyer;			// 임차 희망자 정보를 담는다. 

	cltRentContract		clContract;			// 희망자의 계약 조건을 담는다. 

	sDBResponse_LandRentApply()
	{
		ZeroMemory(this, sizeof(sDBResponse_LandRentApply));
	}
};


//-----------------------------------------
// 농장 임차 신청을 취소한다. 
//-----------------------------------------
class sDBRequest_LandCancelRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 신청자.
	SI32				siVillageUnique;	// 대상이 되는 농장이 위치한 마을

	sDBRequest_LandCancelRentApply()
	{
		ZeroMemory( this, sizeof( sDBRequest_LandCancelRentApply ) );
	}

	sDBRequest_LandCancelRentApply( SI32 id, SI32 villageunique, SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_LandCancelRentApply );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_CANCELRENTAPPLY;

		usCharID		= id;

		siVillageUnique	= villageunique;
		siPersonID		= personid;
	}
};


class sDBResponse_LandCancelRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 농장이 위치한 마을
	SI32				siPersonID;

	sDBResponse_LandCancelRentApply()
	{
		ZeroMemory( this, sizeof(sDBResponse_LandCancelRentApply) );
	}
};


//-----------------------------------------
// 임차 희망자 명단을 얻어온다. 
//-----------------------------------------
class sDBRequest_LandApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique; // 대상이 되는 농장이 위치한 마을의 Unique

	sDBRequest_LandApplyerList()
	{
		ZeroMemory(this, sizeof(sDBRequest_LandApplyerList) );
	}

	sDBRequest_LandApplyerList(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_LandApplyerList );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_APPLYERLIST;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};


class sDBResponse_LandApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.

	SI32				siVillageUnique;					// 대상이 되는 농장이 위치한 마을
	cltRentApplyer		clApplyer[ MAX_APPLY_PER_LAND ];	// 임차 희망자 명단을 담는다. 

	sDBResponse_LandApplyerList()
	{
		ZeroMemory(this, sizeof(sDBResponse_LandApplyerList) );
	}
};


//-----------------------------------------------------
// 임차 희망자의 명단을 모두 삭제한다. 
//-----------------------------------------------------
class sDBRequest_LandDelApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;

	// PCK : 로그용 패킷 추가. 임차 희망자 명단을 모조리 삭제하는것을 수정함.
	SI32				siDelSlotNum;

	sDBRequest_LandDelApplyer()
	{
		ZeroMemory( this, sizeof(sDBRequest_LandDelApplyer) );
	}

	sDBRequest_LandDelApplyer( SI32 id, SI32 personid, SI32 villageunique, SI32 DelSlotNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_LandDelApplyer );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_DELAPPLYER;

		usCharID		= id;
		siPersonID		= personid;
		siVillageUnique	= villageunique;
		siDelSlotNum = DelSlotNum;
	}

};


class sDBResponse_LandDelApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;		

	SI32				siPersonID;			
	SI32				siVillageUnique;

	// PCK : 로그용 패킷 추가, 임차 희망자 명단을 모조리 삭제하는것을 수정함.
	SI32				siDelSlotNum;

	sDBResponse_LandDelApplyer()
	{
		ZeroMemory(this, sizeof(sDBResponse_LandDelApplyer));
	}
};

//-----------------------------------------
// 농장을 임대한다. 
//-----------------------------------------
class sDBRequest_LandRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 신청자.

	SI32				siVillageUnique;		// 대상이 되는 농장이 위치한 마을의 Unique
	SI32				siIndex;				// 농장의 번호. 
	
	cltRentContract		clRentContract;			// 계약 조건. 

	sDBRequest_LandRent()
	{
		ZeroMemory(this, sizeof(sDBRequest_LandRent));
	}

	sDBRequest_LandRent(SI32 id, SI32 villageunique, SI32 personid, SI32 index, cltRentContract* pclcontract)
	{
		packetHeader.usSize = sizeof( sDBRequest_LandRent );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_RENT;

		usCharID		= id;

		siPersonID		= personid;

		siVillageUnique	= villageunique;
		siIndex			= index;

		clRentContract.Set(pclcontract);
	}
};


class sDBResponse_LandRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltSimplePerson		clPerson;			// 신청자.

	SI32				siVillageUnique;	// 대상이 되는 농장이 위치한 마을
	SI32				siIndex;			// 단위 농장의 인덱스 

	cltRentContract		clRentContract;		// 계약 조건. 

	sDBResponse_LandRent()
	{
		ZeroMemory(this, sizeof(sDBResponse_LandRent));
	}
};



//-----------------------------------------------------
// 농장의 임대료를 지불한다.
//-----------------------------------------------------
class sDBRequest_LandPayRentFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 임대료를 지불하는 사람.

	GMONEY				siRentFee;				// 지불해야 할 임대료. 

	SI32				siVillageUnique;		// 임대료를 받게될 농장의 마을
	SI32				siLandIndex;			// 대상이 되는 단위 농장의 인덱스. 



	sDBRequest_LandPayRentFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_LandPayRentFee));
	}

	sDBRequest_LandPayRentFee( SI32 id, SI32 personid, GMONEY rentfee, SI32 villageunique, SI32 landindex)
	{
		packetHeader.usSize = sizeof( sDBRequest_LandPayRentFee );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_PAYRENTFEE;

		usCharID		= id;
		siPersonID		= personid;
		
		siRentFee		= rentfee;

		siVillageUnique	= villageunique;
		siLandIndex	= landindex;
	}

};


class sDBResponse_LandPayRentFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 임대료를 지불한 사람. 
	cltMoney			clBankMoney;		// 지불자의 은행잔고.

	GMONEY				siRentFee;			// 지불했어야 할 임대료. 
	SI32				siDelayNum;			// 연체 횟수
	GMONEY				siDelayMoney;		// 연체금 합계. 

	SI32				siVillageUnique;	// 대상이 되는 농장이 위치한 마을
	SI32				siLandIndex;		// 단위 농장의 인덱스
	cltMoney			clLandMoney;		// 임대료를 받은 결과 농장 관리소의 보유 자금. 

	sDBResponse_LandPayRentFee()
	{
		ZeroMemory( this, sizeof( sDBResponse_LandPayRentFee ) );
	}
};



//-----------------------------------------------------
// 농장을 회수한다. 
//-----------------------------------------------------
class sDBRequest_LandRecall
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유(연체, 계약종료등..)
	
	SI32				siVillageUnique;	// 농장의 마을 유니크
	SI32				siLandIndex;		// 대상이 되는 단위 농장의 인덱스. 

	sDBRequest_LandRecall()
	{
		ZeroMemory( this, sizeof(sDBRequest_LandRecall) );
	}

	sDBRequest_LandRecall( SI32 personid, SI32 reason, SI32 villageunique, SI32 landindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_LandRecall );
		packetHeader.usCmd	= DBMSG_REQUEST_LAND_RECALL;

		siPersonID		= personid;

		siRecallReason	= reason;

		siVillageUnique	= villageunique;
		siLandIndex	= landindex;
	}

};


class sDBResponse_LandRecall
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유( 연체, 계약 종료 등..) 
	
	SI32				siVillageUnique;	// 농장의 마을 유니크. 
	SI32				siLandIndex;		// 대상이 되는 단위 농장의 인덱스. 

	sDBResponse_LandRecall()
	{
		ZeroMemory( this, sizeof(sDBResponse_LandRecall) );
	}
};


#endif