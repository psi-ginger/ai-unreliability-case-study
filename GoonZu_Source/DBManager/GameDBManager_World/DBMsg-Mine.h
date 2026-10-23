#ifndef _DBMSGMINE_H
#define _DBMSGMINE_H

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Mine\Mine.h"

#define MINERECALL_REASON_FEEDELAY			1	// 연체로 광산 회수 .
#define MINERECALL_REASON_CONTRACTEND		2	// 계약 종료로 광산 회수. 

//-----------------------------------------
// 단위 광산의 정보를 얻어온다. 
//-----------------------------------------
class sDBRequest_ManyMineUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;

	sDBRequest_ManyMineUnit()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyMineUnit));
	}

	sDBRequest_ManyMineUnit(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_ManyMineUnit );
		packetHeader.usCmd	= DBMSG_REQUEST_MANYMINEUNIT;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};

class sDBResponse_ManyMineUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;								// 결과값.

	SI32				siVillageUnique;						// 대상이 되는 광산이 위치한 마을의 Unique
	cltMineUnit			clMineUnit[ MAX_MINEUNIT_NUMBER ];		// 한 마을에 있는 단위 광산 전부의 정보를 담는다. 	

	sDBResponse_ManyMineUnit()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyMineUnit));
	}
};


//-----------------------------------------
// 광산의 계약 조건을 변경하도록 한다. 
//-----------------------------------------
class sDBRequest_MineChangeContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 대상이 되는 광산이 위치한 마을
	cltRentContract		clContract;				// 계약 조건
	
	cltDate				clDate;					// 계약 변경일

	sDBRequest_MineChangeContract()
	{
		ZeroMemory(this, sizeof(sDBRequest_MineChangeContract));
	}

	sDBRequest_MineChangeContract(SI32 id, SI32 villageunique, cltRentContract* pclcontract, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_MineChangeContract );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_CHANGECONTRACT;

		usCharID		= id;
		siVillageUnique	= villageunique;

		clContract.Set(pclcontract);
		clDate.Set( pclDate );
	}
};


class sDBResponse_MineChangeContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;				// 결과값.

	SI32				siVillageUnique;		// 대상이 되는 광산이 위치한 마을
	cltRentContract		clContract;				// 변경된 계약 조건

	cltDate				clDate;					// 계약 변경일

	sDBResponse_MineChangeContract()
	{
		ZeroMemory(this, sizeof(sDBResponse_MineChangeContract));
	}
};



//-----------------------------------------
// 광산 임차 신청한다. 
//-----------------------------------------
class sDBRequest_MineRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// 대상이 되는 광산이 위치한 마을
	SI32				siPersonID;				// 신청자.
	
	cltRentContract		clContract;				// 계약정보

	sDBRequest_MineRentApply()
	{
		ZeroMemory(this, sizeof(sDBRequest_MineRentApply));
	}

	sDBRequest_MineRentApply( SI32 id, SI32 villageunique, SI32 personid, cltRentContract* pclcontract )
	{
		packetHeader.usSize = sizeof( sDBRequest_MineRentApply );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_RENTAPPLY;

		usCharID		= id;
		siVillageUnique	= villageunique;
		siPersonID		= personid;

		clContract.Set(pclcontract);
	}
};


class sDBResponse_MineRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 광산이 위치한 마을
	
	SI32				siIndex;			// 임차 희망자의 정보가 담긴 위치를 가리킨다.
	cltSimplePerson		clApplyer;			// 임차 희망자 정보를 담는다. 

	cltRentContract		clContract;			// 희망자의 계약 조건을 담는다. 

	sDBResponse_MineRentApply()
	{
		ZeroMemory(this, sizeof(sDBResponse_MineRentApply));
	}
};


//-----------------------------------------
// 광산 임차 신청을 취소한다. 
//-----------------------------------------
class sDBRequest_MineCancelRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 신청자.
	SI32				siVillageUnique;	// 대상이 되는 광산이 위치한 마을

	sDBRequest_MineCancelRentApply()
	{
		ZeroMemory( this, sizeof( sDBRequest_MineCancelRentApply ) );
	}

	sDBRequest_MineCancelRentApply( SI32 id, SI32 villageunique, SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_MineCancelRentApply );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_CANCELRENTAPPLY;

		usCharID		= id;

		siVillageUnique	= villageunique;
		siPersonID		= personid;
	}
};


class sDBResponse_MineCancelRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 광산이 위치한 마을
	SI32				siPersonID;

	sDBResponse_MineCancelRentApply()
	{
		ZeroMemory( this, sizeof(sDBResponse_MineCancelRentApply) );
	}
};


//-----------------------------------------
// 임차 희망자 명단을 얻어온다. 
//-----------------------------------------
class sDBRequest_MineApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique; // 대상이 되는 광산이 위치한 마을의 Unique

	sDBRequest_MineApplyerList()
	{
		ZeroMemory(this, sizeof(sDBRequest_MineApplyerList) );
	}

	sDBRequest_MineApplyerList(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_MineApplyerList );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_APPLYERLIST;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};


class sDBResponse_MineApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 결과값.

	SI32				siVillageUnique;					// 대상이 되는 광산이 위치한 마을
	cltRentApplyer		clApplyer[ MAX_APPLY_PER_MINE ];	// 임차 희망자 명단을 담는다. 

	sDBResponse_MineApplyerList()
	{
		ZeroMemory(this, sizeof(sDBResponse_MineApplyerList) );
	}
};


//-----------------------------------------------------
// 임차 희망자의 명단을 모두 삭제한다. 
//-----------------------------------------------------
class sDBRequest_MineDelApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;

	// [추가 : 황진성 2007. 11. 2 로그에 이용. 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
	SI32				siDelSlotNum;

	sDBRequest_MineDelApplyer()
	{
		ZeroMemory( this, sizeof(sDBRequest_MineDelApplyer) );
	}

	// [추가 : 황진성 2007. 11. 2 로그에 이용. 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
	sDBRequest_MineDelApplyer( SI32 id, SI32 personid, SI32 villageunique, SI32 DelSlotNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_MineDelApplyer );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_DELAPPLYER;

		usCharID		= id;
		siPersonID		= personid;
		siVillageUnique	= villageunique;
		
		// [추가 : 황진성 2007. 11. 2 로그에 이용. 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
		siDelSlotNum    = DelSlotNum;
	}

};


class sDBResponse_MineDelApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;		

	SI32				siPersonID;			
	SI32				siVillageUnique;

	// [추가 : 황진성 2007. 11. 2 로그에 이용. 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
	SI32				siDelSlotNum;

	sDBResponse_MineDelApplyer()
	{
		ZeroMemory(this, sizeof(sDBResponse_MineDelApplyer));
	}
};

//-----------------------------------------
// 광산을 임대한다. 
//-----------------------------------------
class sDBRequest_MineRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 신청자.

	SI32				siVillageUnique;		// 대상이 되는 광산이 위치한 마을의 Unique
	SI32				siIndex;				// 광산의 번호. 
	
	cltRentContract		clRentContract;			// 계약 조건. 

	sDBRequest_MineRent()
	{
		ZeroMemory(this, sizeof(sDBRequest_MineRent));
	}

	sDBRequest_MineRent(SI32 id, SI32 villageunique, SI32 personid, SI32 index, cltRentContract* pclcontract)
	{
		packetHeader.usSize = sizeof( sDBRequest_MineRent );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_RENT;

		usCharID		= id;

		siPersonID		= personid;

		siVillageUnique	= villageunique;
		siIndex			= index;

		clRentContract.Set(pclcontract);
	}
};


class sDBResponse_MineRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltSimplePerson		clPerson;			// 신청자.

	SI32				siVillageUnique;	// 대상이 되는 광산이 위치한 마을
	SI32				siIndex;			// 단위 광산의 인덱스 

	cltRentContract		clRentContract;		// 계약 조건. 

	sDBResponse_MineRent()
	{
		ZeroMemory(this, sizeof(sDBResponse_MineRent));
	}
};



//-----------------------------------------------------
// 광산의 임대료를 지불한다.
//-----------------------------------------------------
class sDBRequest_MinePayRentFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 임대료를 지불하는 사람.

	GMONEY				siRentFee;				// 지불해야 할 임대료. 

	SI32				siVillageUnique;		// 임대료를 받게될 광산의 마을
	SI32				siMineIndex;			// 대상이 되는 단위 광산의 인덱스. 



	sDBRequest_MinePayRentFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_MinePayRentFee));
	}

	sDBRequest_MinePayRentFee( SI32 id, SI32 personid, GMONEY rentfee, SI32 villageunique, SI32 Mineindex)
	{
		packetHeader.usSize = sizeof( sDBRequest_MinePayRentFee );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_PAYRENTFEE;

		usCharID		= id;
		siPersonID		= personid;
		
		siRentFee		= rentfee;

		siVillageUnique	= villageunique;
		siMineIndex	= Mineindex;
	}

};


class sDBResponse_MinePayRentFee
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

	SI32				siVillageUnique;	// 대상이 되는 광산이 위치한 마을
	SI32				siMineIndex;		// 단위 광산의 인덱스
	cltMoney			clMineMoney;		// 임대료를 받은 결과 광산 관리소의 보유 자금. 

	sDBResponse_MinePayRentFee()
	{
		ZeroMemory( this, sizeof( sDBResponse_MinePayRentFee ) );
	}
};



//-----------------------------------------------------
// 광산을 회수한다. 
//-----------------------------------------------------
class sDBRequest_MineRecall
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유(연체, 계약종료등..)
	
	SI32				siVillageUnique;	// 광산의 마을 유니크
	SI32				siMineIndex;		// 대상이 되는 단위 광산의 인덱스. 

	sDBRequest_MineRecall()
	{
		ZeroMemory( this, sizeof(sDBRequest_MineRecall) );
	}

	sDBRequest_MineRecall( SI32 personid, SI32 reason, SI32 villageunique, SI32 Mineindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_MineRecall );
		packetHeader.usCmd	= DBMSG_REQUEST_MINE_RECALL;

		siPersonID		= personid;

		siRecallReason	= reason;

		siVillageUnique	= villageunique;
		siMineIndex	= Mineindex;
	}

};


class sDBResponse_MineRecall
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유( 연체, 계약 종료 등..) 
	
	SI32				siVillageUnique;	// 광산의 마을 유니크. 
	SI32				siMineIndex;		// 대상이 되는 단위 광산의 인덱스. 

	sDBResponse_MineRecall()
	{
		ZeroMemory( this, sizeof(sDBResponse_MineRecall) );
	}
};

#endif