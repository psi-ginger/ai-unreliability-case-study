#ifndef _DBMSGHOUSE_H
#define _DBMSGHOUSE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\House\House.h"

//-----------------------------------------------------
// 주택 창고의 특정 위치에 대한 정보를 요청한다. 
//-----------------------------------------------------
class sDBRequest_GetHouseStgPos
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Person
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// House
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?
	SI16				siStgPos;			// 대상이 되는 창고내 위치. 

	sDBRequest_GetHouseStgPos()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetHouseStgPos));
	}

	sDBRequest_GetHouseStgPos(SI32 id, SI32 personid, 
						SI32 villageunique, SI32 houseunitslot, SI32 stgpos)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetHouseStgPos );
		packetHeader.usCmd	= DBMSG_REQUEST_GETHOUSESTGPOS;

		usCharID		= id;

		siPersonID		= personid;

		siVillageUnique	= villageunique;
		siHouseUnitSlot	= houseunitslot;
		siStgPos		= stgpos;

	}
};


class sDBResponse_GetHouseStgPos
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// Person
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// House
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?
	SI16				siStgPos;			// 대상이 되는 창고내 위치. 

	// Output
	cltItem				clItem;				// 생산품의 정보. (개수 포함. ) 

	sDBResponse_GetHouseStgPos()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetHouseStgPos));
	}
};


//-----------------------------------------------------
// 임차 희망자의 명단을 모두 삭제한다. 
//-----------------------------------------------------
class sDBRequest_DelHouseApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// where
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique

	// PCK : 임차 희망자 명단을 모조리 삭제하는것을 수정함.
	SI32				siDelSlotNum;

	sDBRequest_DelHouseApplyer()
	{
		ZeroMemory(this, sizeof(sDBRequest_DelHouseApplyer));
	}

	sDBRequest_DelHouseApplyer(SI32 id, SI32 personid, SI32 villageunique, SI32 delSlotNum)
	{
		packetHeader.usSize = sizeof( sDBRequest_DelHouseApplyer );
		packetHeader.usCmd	= DBMSG_REQUEST_DELHOUSEAPPLYER;

		usCharID		= id;

		siPersonID		= personid;

		siVillageUnique	= villageunique;
		siDelSlotNum	= delSlotNum;
	}

};


class sDBResponse_DelHouseApplyer
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// where
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique

	// PCK : 임차 희망자 명단을 모조리 삭제하는것을 수정함.
	SI32				siDelSlotNum;

	sDBResponse_DelHouseApplyer()
	{
		ZeroMemory(this, sizeof(sDBResponse_DelHouseApplyer));
	}
};


//-----------------------------------------------------
// 시전의 임대료를 지불한다. 
//-----------------------------------------------------
class sDBRequest_PayRentFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who
	SI32				siPersonID;			// 임대료를 지불하는 사람.
	GMONEY				siRentFee;			// 지불해야 할 임대료. 

	//whom				
	SI32				siVillageUnique;	// 임대료를 받게될 시전의 마을 유니크. 

	// for what
	SI32				siHouseIndex;		// 대상이 되는 단위 시전의 인덱스. 

	sDBRequest_PayRentFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_PayRentFee));
	}

	sDBRequest_PayRentFee(SI32 id, SI32 personid, GMONEY rentfee, SI32 villageunique, SI32 houseindex)
	{
		packetHeader.usSize = sizeof( sDBRequest_PayRentFee );
		packetHeader.usCmd	= DBMSG_REQUEST_PAYRENTFEE;

		usCharID		= id;

		siPersonID		= personid;

		siRentFee		= rentfee;

		siVillageUnique	= villageunique;

		siHouseIndex	= houseindex;
	}

};


class sDBResponse_PayRentFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;			// 임대료를 지불한 사람. 
	GMONEY				siBankMoney;		// 지불자의 은행잔고.
	GMONEY				siRentFee;			// 지불했어야 할 임대료. 
	SI32				siDelayNum;			// 연체 횟수
	GMONEY				siDelayMoney;		// 연체금 합계. 

	// for what
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseIndex;
	GMONEY				siHouseMoney;		// 임대료를 받아서 결과 시전의 보유 자금. 

	sDBResponse_PayRentFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_PayRentFee));
	}
};



//-----------------------------------------------------
// 시전을 회수한다. 
//-----------------------------------------------------
class sDBRequest_RecallHouse
{
public:
	sPacketHeader		packetHeader;

	// who
	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유(연체, 계약종료등..) 

	//whom				
	SI32				siVillageUnique;	// 시전의 마을 유니크. 

	// for what
	SI32				siHouseIndex;		// 대상이 되는 단위 시전의 인덱스. 
	
	GMONEY				siPenaltyMoney;
	
	//cltMoney			clPenaltyMoney;
	TCHAR				szOwnerName[MAX_PLAYER_NAME];


	sDBRequest_RecallHouse()
	{
		ZeroMemory(this, sizeof(sDBRequest_RecallHouse));
	}

	sDBRequest_RecallHouse(SI32 personid, SI32 reason, SI32 villageunique, SI32 houseindex, GMONEY penaltyMoney = 0, const TCHAR* ownerName = NULL)
	{
		packetHeader.usSize = sizeof( sDBRequest_RecallHouse );
		packetHeader.usCmd	= DBMSG_REQUEST_RECALLHOUSE;

		siPersonID		= personid;

		siRecallReason	= reason;

		siVillageUnique	= villageunique;

		siHouseIndex	= houseindex;

		siPenaltyMoney = penaltyMoney;
		
        MStrCpy(szOwnerName, ownerName, MAX_PLAYER_NAME);
	}

};


class sDBResponse_RecallHouse
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;			// 임차인. 

	SI32				siRecallReason;		// 회수 사유(연체, 계약종료등..) 

	//whom				
	SI32				siVillageUnique;	// 시전의 마을 유니크. 

	// for what
	SI32				siHouseIndex;		// 대상이 되는 단위 시전의 인덱스. 
	
	GMONEY				siPenaltyMoney;		// 유저가 지불한 위약금
	GMONEY				siRemainMoney;		// 유저의 남은돈
	GMONEY				siHouseMoney;		// 해약금을 받은 시전의 현재 자산
	
	TCHAR				szOwnerName[MAX_PLAYER_NAME];

//	cltMoney				clPenaltyMoney;
//	cltMoney				clRemainMoney;		
//  cltMoney				clHouseMoney

	sDBResponse_RecallHouse()
	{
		ZeroMemory(this, sizeof(sDBResponse_RecallHouse));
	}
};


//-----------------------------------------
// 주택을 임대한다. 
//-----------------------------------------
class sDBRequest_HouseRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siPersonID;			// 신청자.
	SI32				siIndex;			// 주택의 번호. 
	cltRentContract		clRentContract;		// 계약 조건. 
	TCHAR				m_szCharName[MAX_PLAYER_NAME];

	sDBRequest_HouseRent()
	{
		ZeroMemory(this, sizeof(sDBRequest_HouseRent));
	}

	sDBRequest_HouseRent(SI32 id, SI32 villageunique, SI32 personid, SI32 index, cltRentContract* pclcontract , TCHAR* pszCharName)
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseRent );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSERENT;

		usCharID		= id;
		siVillageUnique	= villageunique;
		siPersonID		= personid;
		siIndex			= index;

		clRentContract.Set(pclcontract);

		_tcscpy(m_szCharName, pszCharName);
	}
};


class sDBResponse_HouseRent
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	cltSimplePerson		clPerson;			// 신청자.
	SI32				siIndex;			// 주택의 번호. 
	cltRentContract		clRentContract;		// 계약 조건. 

	SI32				siApplyerIndex;		// 신청자 상태의 위치. 
	cltRentApplyer		clApplyer;			// 신청자 정보. 
	
	TCHAR				m_szName[MAX_PLAYER_NAME];

	sDBResponse_HouseRent()
	{
		ZeroMemory(this, sizeof(sDBResponse_HouseRent));
	}
};



//-------------------------
// 부동산 계약을 연장한다. 
//-------------------------
class sDBRequest_ExtendContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 신청자.

	SI32				siVillageUnique;	// 대상이 되는 부동산이 위치한 마을의 Unique
	SI32				siRealEstateType;	// 부동산의 종류.(REALESTATE_HOUSE.._)  
	SI32				siIndex;			// 부동산의 번호. 
	cltRentContract		clRentContract;		// 계약 조건. 

	sDBRequest_ExtendContract()
	{
		ZeroMemory(this, sizeof(sDBRequest_ExtendContract));
	}

	sDBRequest_ExtendContract(SI32 id, SI32 personid, SI32 villageunique, SI32 realestatetype, SI32 index, cltRentContract* pclcontract)
	{
		packetHeader.usSize = sizeof( sDBRequest_ExtendContract );
		packetHeader.usCmd	= DBMSG_REQUEST_EXTENDCONTRACT;

		usCharID			= id;
	
		siPersonID			= personid;
		siVillageUnique		= villageunique;
		siRealEstateType	= realestatetype;
		siIndex				= index;
		clRentContract.Set(pclcontract);
	}
};


class sDBResponse_ExtendContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltSimplePerson		clPerson;			// 신청자.

	SI32				siVillageUnique;	// 대상이 되는 부동산이 위치한 마을의 Unique
	SI32				siRealEstateType;	// 부동산의 종류.(REALESTATE_HOUSE.._)  
	SI32				siIndex;			// 부동산의 번호. 
	cltRentContract		clRentContract;		// 계약 조건. 
	GMONEY				siBankMoney;		// 계약연장을 위해 임대료 지불하고 남은 현재의 전장 잔액. 

	sDBResponse_ExtendContract()
	{
		ZeroMemory(this, sizeof(sDBResponse_ExtendContract));
	}
};

//-------------------------
// 창고 이름 변경 
//-------------------------
class sDBRequest_ChangeHouseUnitName
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;

	SI32				m_siPersonID;			// 신청자.

	SI32				m_siVillageUnique;		// 대상이 되는 부동산이 위치한 마을의 Unique
	SI32				m_siIndex;				// 부동산의 번호. 
	TCHAR				m_szName[MAX_PLAYER_NAME]; // 창고 이름				

	sDBRequest_ChangeHouseUnitName()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeHouseUnitName));
	}

	sDBRequest_ChangeHouseUnitName(SI32 id, SI32 personid, SI32 villageunique, SI32 index, TCHAR* pszName)
	{
		m_packetHeader.usSize = sizeof( sDBRequest_ChangeHouseUnitName );
		m_packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITNAMECHANGE;

		m_usCharID			= id;

		m_siPersonID			= personid;
		m_siVillageUnique		= villageunique;
		m_siIndex				= index;
		_tcscpy( m_szName , pszName);
	}
};


class sDBResponse_ChangeHouseUnitName
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;

	SI32				m_siResult;			// 결과값.

	SI32				m_siPersonID;			// 신청자.

	SI32				m_siVillageUnique;		// 대상이 되는 부동산이 위치한 마을의 Unique
	SI32				m_siIndex;				// 부동산의 번호. 
	TCHAR				m_szName[MAX_PLAYER_NAME]; // 창고 이름				

	sDBResponse_ChangeHouseUnitName()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeHouseUnitName));
	}
};

//-----------------------------------------
// 주택의 계약 조건을 변경하도록 한다. 
//-----------------------------------------
class sDBRequest_ChangeHouseContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;// 대상이 되는 주택이 위치한 마을의 Unique
	cltRentContract		clContract;		// 계약 조건.
	cltDate				clDate;			// 계약 변경일

	sDBRequest_ChangeHouseContract()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeHouseContract));
	}

	sDBRequest_ChangeHouseContract(SI32 id, SI32 villageunique, cltRentContract* pclcontract, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeHouseContract );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGEHOUSECONTRACT;

		usCharID		= id;
		siVillageUnique	= villageunique;

		clContract.Set(pclcontract);
		clDate.Set( pclDate );
	}
};


class sDBResponse_ChangeHouseContract
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;// 대상이 되는 주택이 위치한 마을의 Unique
	cltRentContract		clContract;		// 변경된 계약 조건.

	cltDate				clDate;			// 계약 변경일

	sDBResponse_ChangeHouseContract()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeHouseContract));
	}
};

//-----------------------------------------
// 단위 주택의 정보를 얻어온다. 
//-----------------------------------------
class sDBRequest_ManyHouseUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique; // 대상이 되는 주택이 위치한 마을의 Unique

	sDBRequest_ManyHouseUnit()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyHouseUnit));
	}

	sDBRequest_ManyHouseUnit(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_ManyHouseUnit );
		packetHeader.usCmd	= DBMSG_REQUEST_MANYHOUSEUNIT;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};

#define MANYHOUSEUNIT_NUMBER	20
class sDBResponse_ManyHouseUnit
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;								// 결과값.

	SI32				siVillageUnique;						// 대상이 되는 주택이 위치한 마을의 Unique
	
	SI32				siStartIndex;
	cltHouseUnit		clHouseUnit[MANYHOUSEUNIT_NUMBER];		// 한 마을에 있는 단위 주택 전부의 정보를 담는다. 	

	sDBResponse_ManyHouseUnit()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyHouseUnit));
	}
};


//-----------------------------------------
// 임차 희망자 명단을 얻어온다. 
//-----------------------------------------
class sDBRequest_GetHouseApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique; // 대상이 되는 주택이 위치한 마을의 Unique

	sDBRequest_GetHouseApplyerList()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetHouseApplyerList));
	}

	sDBRequest_GetHouseApplyerList(SI32 id, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetHouseApplyerList );
		packetHeader.usCmd	= DBMSG_REQUEST_GETHOUSEAPPLYERLIST;

		usCharID		= id;
		siVillageUnique	= villageunique;
	}
};


class sDBResponse_GetHouseApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	
	cltRentApplyer		clApply[MAX_APPLY_PER_HOUSE];	// 임차 희망자 명단을 담는다. 

	sDBResponse_GetHouseApplyerList()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetHouseApplyerList));
	}
};

//-----------------------------------------
// 주택 임차를 신청한다. 
//-----------------------------------------
class sDBRequest_HouseRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siPersonID;			// 신청자.
	
	cltRentContract		clContract;

	sDBRequest_HouseRentApply()
	{
		ZeroMemory(this, sizeof(sDBRequest_HouseRentApply));
	}

	sDBRequest_HouseRentApply(SI32 id, SI32 villageunique, SI32 personid, cltRentContract* pclcontract)
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseRentApply );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSERENTAPPLY;

		usCharID		= id;
		siVillageUnique	= villageunique;
		siPersonID		= personid;
		clContract.Set(pclcontract);
	}
};


class sDBResponse_HouseRentApply
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	
	SI32				siIndex;			// 임차 희망자의 정보가 담긴 위치를 가리킨다.
	cltSimplePerson		clApply;			// 임차 희망자 정보를 담는다. 

	cltRentContract		clContract;			// 희망자의 계약 조건을 담는다. 
	sDBResponse_HouseRentApply()
	{
		ZeroMemory(this, sizeof(sDBResponse_HouseRentApply));
	}
};


//-----------------------------------------
// 주택 임차 신청을 취소한다. 
//-----------------------------------------
class sDBRequest_HouseRentApplyCancel
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who 
	SI32				siPersonID;			// 신청자.

	// where
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique

	sDBRequest_HouseRentApplyCancel()
	{
		ZeroMemory(this, sizeof(sDBRequest_HouseRentApplyCancel));
	}

	sDBRequest_HouseRentApplyCancel(SI32 id, SI32 villageunique, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseRentApplyCancel );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSERENTAPPLYCANCEL;

		usCharID		= id;

		siVillageUnique	= villageunique;
		siPersonID		= personid;
	}
};


class sDBResponse_HouseRentApplyCancel
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	
	SI32				siIndex;			// 임차 희망자의 정보가 담긴 위치를 가리킨다.
	cltRentApplyer		clApply;			// 임차 희망자 정보를 담는다. 

	sDBResponse_HouseRentApplyCancel()
	{
		ZeroMemory(this, sizeof(sDBResponse_HouseRentApplyCancel));
	}
};

//-----------------------------------------
//  아이템을 인벤토리에서 창고로 옮긴다. 
//-----------------------------------------
class sDBRequest_MoveItemInvToStg
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from 
	SI32				siPersonID;			// 대상이 되는 Person
	SI16				siInvPos;			// 인벤토리내 위치. 

	// to 
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?
	SI16				siStgPos;			// 단위 주택내 위치. 

	// what
	cltItem				clItem;
	bool				bCanPileSwitch;


	sDBRequest_MoveItemInvToStg()
	{
		ZeroMemory(this, sizeof(sDBRequest_MoveItemInvToStg));
	}

	sDBRequest_MoveItemInvToStg(SI32 id, SI32 personid, SI32 invpos, SI32 villageunique, SI32 houseunitslot, SI32 stgpos, cltItem* pclitem, bool bcanpileswitch)
	{
		packetHeader.usSize = sizeof( sDBRequest_MoveItemInvToStg );
		packetHeader.usCmd	= DBMSG_REQUEST_MOVEITEMINVTOSTG;

		usCharID		= id;

		siPersonID		= personid;
		siInvPos		= invpos;

		siVillageUnique	= villageunique;
		siHouseUnitSlot	= houseunitslot;
		siStgPos		= stgpos;

		clItem.Set(pclitem);
		bCanPileSwitch	= bcanpileswitch;
	}
};


class sDBResponse_MoveItemInvToStg
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// from
	SI32				siPersonID;
	SI16				siInvPos;
	cltItem				clInvItem;

	// to 
	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;
	SI16				siStgPos;
	cltItem				clStgItem;
	
	//[추가 : 황진성 2007. 10. 24 로그에 사용. 인벤에서 창고로 이동하는 아이템 정보]
	cltItem				clMoveItem;

	sDBResponse_MoveItemInvToStg()
	{
		ZeroMemory(this, sizeof(sDBResponse_MoveItemInvToStg));
	}
};


//-----------------------------------------
//  아이템을 창고에서 인벤토리로 옮긴다. 
//-----------------------------------------
class sDBRequest_MoveItemStgToInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from 
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?
	SI16				siStgPos;			// 단위 주택내 위치. 

	// to 
	SI32				siPersonID;			// 대상이 되는 Person
	SI16				siInvPos;			// 인벤토리내 위치. 

	// what
	cltItem				clItem;
	bool				bCanPileSwitch;


	sDBRequest_MoveItemStgToInv()
	{
		ZeroMemory(this, sizeof(sDBRequest_MoveItemStgToInv));
	}

	sDBRequest_MoveItemStgToInv(SI32 id, SI32 villageunique, SI32 houseunitslot, SI32 stgpos, SI32 personid, SI32 invpos, cltItem* pclitem, bool bcanpileswitch)
	{
		packetHeader.usSize = sizeof( sDBRequest_MoveItemStgToInv );
		packetHeader.usCmd	= DBMSG_REQUEST_MOVEITEMSTGTOINV;

		usCharID		= id;

		siVillageUnique	= villageunique;
		siHouseUnitSlot	= houseunitslot;
		siStgPos		= stgpos;

		siPersonID		= personid;
		siInvPos		= invpos;

		clItem.Set(pclitem);
		bCanPileSwitch	= bcanpileswitch;
	}
};


class sDBResponse_MoveItemStgToInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// from
	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;
	SI16				siStgPos;
	cltItem				clStgItem;

	// to 
	SI32				siPersonID;
	SI16				siInvPos;
	cltItem				clInvItem;

	//[추가 : 황진성 2007. 10. 24 로그에 사용. 창고에서 인벤로 이동하는 아이템 정보]
	cltItem				clMoveItem;

	sDBResponse_MoveItemStgToInv()
	{
		ZeroMemory(this, sizeof(sDBResponse_MoveItemStgToInv));
	}
};



class sDBRequest_MakeItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Person
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// House
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?

	// Material 
	DBMaterialInfo		clMaterial[MAX_MATERIAL_NUMBER];

	// Output
	SI16				siOutputPos;		// 생산품이 위치할 공간. 
	cltItem				clItem;				// 생산품의 정보. (개수 포함. ) 
	bool				bCanPileSwitch;		// 생산품이 집적가능한가 ?
	bool				bAlmostFailSwitch;	// 거의 실패한 제품인가 ?

	sDBRequest_MakeItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_MakeItem));
	}

	sDBRequest_MakeItem(SI32 id, SI32 personid, 
						SI32 villageunique, SI32 houseunitslot, 
						DBMaterialInfo* pclinfo, 
						SI32 outputpos, cltItem* pclitem, bool bcanpileswitch, bool balmostfailswitch)
	{
		packetHeader.usSize = sizeof( sDBRequest_MakeItem );
		packetHeader.usCmd	= DBMSG_REQUEST_MAKEITEM;

		usCharID		= id;

		siPersonID		= personid;

		siVillageUnique	= villageunique;
		siHouseUnitSlot	= houseunitslot;

		for(SI32 i = 0;i < MAX_MATERIAL_NUMBER;i++)
		{
			clMaterial[i].Set(&pclinfo[i]);
		}

		siOutputPos	= outputpos;
		clItem.Set(pclitem);
		bCanPileSwitch		= bcanpileswitch;
		bAlmostFailSwitch	= balmostfailswitch;
	}
};


class sDBResponse_MakeItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// Person
	SI32				siPersonID;			// 물건을 만드는 사람. 

	// House
	SI32				siVillageUnique;	// 대상이 되는 주택이 위치한 마을의 Unique
	SI32				siHouseUnitSlot;	// 주택중 어떤 단위주택인가 ?

	// Output
	SI16				siOutputPos;		// 생산품이 위치할 공간. 
	SI16				siAddedNum;			// 추가된 아이템 개수. 
	cltItem				clItem;				// 생산품의 정보. (개수 포함. ) 
	bool				bAlmostFailSwitch;	// 거의 실패한 제품인가 ?

	sDBResponse_MakeItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_MakeItem));
	}
};

class sDBRequest_HouseUnitStableCreate
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	GMONEY				siHouseUnitStableCreateFee;
	//SI16				siItemPos;
	//cltItem			clItem;

	SI16				siStableIndex;

	sDBRequest_HouseUnitStableCreate( SI32 CharID, SI32 PersonID,
		SI32 VillageUnique, SI32 HouseUnitSlot,
		GMONEY HouseUnitStableCreateFee,  SI16 StableIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitStableCreate );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSTABLECREATE;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siHouseUnitStableCreateFee = HouseUnitStableCreateFee;
		//siItemPos = ItemPos;
		//clItem.Set( pclItem );


		siStableIndex = StableIndex;
	}
};

class sDBResponse_HouseUnitStableCreate
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	GMONEY				siHouseUnitStableCreateFee;
	//cltMoney			clLeftHouseUnitMoney;
	//SI32				siPersonLeftMoney;

	//SI16				siItemPos;
	//cltItem			clLeftItem;

	SI16				siStableIndex;

	sDBResponse_HouseUnitStableCreate()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitStableCreate ) );
	}
};

class sDBRequest_HouseUnitHorseIn
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siPersonHorseIndex;
	SI16				siHouseUnitStgHorseIndex;
	
	//SI32				siHouseUnitHorseInFee;
	
	sDBRequest_HouseUnitHorseIn( SI32 CharID, SI32 PersonID,
		SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 PersonHorseIndex, SI16 HouseUnitStgHorseIndex )//, SI32 HouseUnitHorseInFee )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitHorseIn );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITHORSEIN;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siPersonHorseIndex = PersonHorseIndex;
		siHouseUnitStgHorseIndex = HouseUnitStgHorseIndex;

		//siHouseUnitHorseInFee = HouseUnitHorseInFee;
	}
};

class sDBResponse_HouseUnitHorseIn
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siPersonHorseIndex;
	cltHorse			clPersonHorse;

	SI16				siHouseUnitStgHorseIndex;
	cltHorse			clHouseUnitStgHorse;

	sDBResponse_HouseUnitHorseIn()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitHorseIn ) );
	}	
};


class sDBRequest_HouseUnitHorseOut
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siHouseUnitStgHorseIndex;

	SI16				siPersonHorseIndex;	

	sDBRequest_HouseUnitHorseOut( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 HouseUnitStgHorseIndex, SI16 PersonHorseIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitHorseIn );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITHORSEOUT;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siHouseUnitStgHorseIndex = HouseUnitStgHorseIndex;

		siPersonHorseIndex = PersonHorseIndex;		
	}
	
};

class sDBResponse_HouseUnitHorseOut
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siHouseUnitStgHorseIndex;
	cltHorse			clHouseUnitHorse;
	
	SI16				siPersonHorseIndex;
	cltHorse			clPersonHorse;

	sDBResponse_HouseUnitHorseOut()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitHorseOut ) );
	}
};

class sDBRequest_HouseUnitHorseInfoSet
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siHouseUnitHorseIndex;
	cltHorse			clHouseUnitHorseInfo;

	sDBRequest_HouseUnitHorseInfoSet( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 HouseUnitHorseIndex, cltHorse *pclHouseUnitHorseInfo )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitHorseInfoSet );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITHORSEINFOSET;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siHouseUnitHorseIndex = HouseUnitHorseIndex;
		clHouseUnitHorseInfo.Set( pclHouseUnitHorseInfo );
	}	
};

class sDBResponse_HouseUnitHorseInfoSet
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siHouseUnitHorseIndex;
	cltHorse			clHouseUnitHorseInfo;

	sDBResponse_HouseUnitHorseInfoSet()
	{
		memset ( this, 0, sizeof( sDBResponse_HouseUnitHorseInfoSet ) );
	}
};


class sDBRequest_HouseUnitHorseEatFood
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siHouseUnitItemIndex;
	
	SI16				siHouseUnitHorseEatItemUnique;
	SI16				siHouseUnitHorseEatItemNum;

	sDBRequest_HouseUnitHorseEatFood( SI32 CharID, SI32 PersonID, SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 HouseUnitItemIndex, SI16 HouseUnitHorseEatItemUnique, SI16 HouseUnitHorseEatItemNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitHorseEatFood );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITHORSEEATFOOD;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siHouseUnitItemIndex = HouseUnitItemIndex;
		
		siHouseUnitHorseEatItemUnique = HouseUnitHorseEatItemUnique;
		siHouseUnitHorseEatItemNum = HouseUnitHorseEatItemNum;
	}
};

class sDBResponse_HouseUnitHorseEatFood
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;
	
	SI16				siHouseUnitHorseEatItemUnique;
	SI16				siHouseUnitHorseEatItemNum;

	SI16				siHouseUnitItemIndex;
	cltItem				clHouseUnitItem;

	sDBResponse_HouseUnitHorseEatFood()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitHorseEatFood ) );
	}

};

class sDBRequest_HouseUnitUsedGoodsBuildingCreate
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siItemPos;				// 소비할 이용권의 유저 인벤 위치
	cltItem				clItem;					// 소비할 이용권 정보와 개수

	sDBRequest_HouseUnitUsedGoodsBuildingCreate( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 ItemPos, cltItem *pclItem )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitUsedGoodsBuildingCreate );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class sDBResponse_HouseUnitUsedGoodsBuildingCreate
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;				// 1 : 성공, Others : 실패

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siItemPos;				// 소비된 이용권의 유저 인벤 위치
	cltItem				clLeftItem;				// 소비하고 남은 유저 인벤 정보

	sDBResponse_HouseUnitUsedGoodsBuildingCreate()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitUsedGoodsBuildingCreate ));
	}
};

class sDBRequest_HouseUnitMakeItemFromUsedGoods
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	// 없앨 아이템
	SI16				siFromHoustUnitStgItemPos;			// 고물 아이템 위치 ( 시전내 아이템 위치임 )
	SI16				siFromHouseUnitItemUnique;			// 고물 아이템 Unique
	
	// 만들 아이템
	SI16				siToHouseUnitStgItemPos[ MAX_MATERIAL_NUMBER ];
	cltItem				clMakeItem[ MAX_MATERIAL_NUMBER ];

	SI16				siMakePercent;

	sDBRequest_HouseUnitMakeItemFromUsedGoods( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 FromHoustUnitStgItemPos, SI16 FromHouseUnitItemUnique,
		SI16 *ToHoustUnitStgItemPos, cltItem *pclMakeItem,
		SI16 MakePercent
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitMakeItemFromUsedGoods );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siFromHoustUnitStgItemPos = FromHoustUnitStgItemPos;
		siFromHouseUnitItemUnique = FromHouseUnitItemUnique;

		memcpy( siToHouseUnitStgItemPos, ToHoustUnitStgItemPos, sizeof( SI16 ) * MAX_MATERIAL_NUMBER );
		memcpy( clMakeItem, pclMakeItem, sizeof( cltItem ) * MAX_MATERIAL_NUMBER );

		siMakePercent = MakePercent;
	}
};

class sDBResponse_HouseUnitMakeItemFromUsedGoods
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siFromItemPos;				// 사라진 고물 아이템의 위치
	SI16				siFromItemUnique;			// 사라진 고물 아이템 Unique

	// 만들고 시전에 남은 아이템 수
	SI16				siToHouseUnitStgItemPos[ MAX_MATERIAL_NUMBER ];		// 만들어진 시전 아이템 위치
	cltItem				clMakeLeftItem[ MAX_MATERIAL_NUMBER ];					// 아이템 위치에 남은 아이템

	SI16				siMakePercent;

	sDBResponse_HouseUnitMakeItemFromUsedGoods()
	{
		memset( this, 0, sizeof ( sDBResponse_HouseUnitMakeItemFromUsedGoods ) );

		for( SI16 i = 0; i < MAX_MATERIAL_NUMBER; ++i )
		{
			siToHouseUnitStgItemPos [ i ] = -1;
		}
	}
};


// 소환수 목장을 만든다.
class sDBRequest_HouseUnitSummonBuildingCreate
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;

	SI16				siItemPos;
	cltItem				clItem;

	sDBRequest_HouseUnitSummonBuildingCreate( SI32 CharID, SI32 PersonID, SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 ItemPos, cltItem *pclItem,
		SI16 SummonBuildingIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitSummonBuildingCreate );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSUMMONBUILDINGCREATE;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siSummonBuildingIndex = SummonBuildingIndex;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class sDBResponse_HouseUnitSummonBuildingCreate
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;

	SI16				siItemPos;
	cltItem				clLeftItem;

	sDBResponse_HouseUnitSummonBuildingCreate()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitSummonBuildingCreate ) );
	}
};


// 소환수 목장을 업그레이드 시킨다.
class sDBRequest_HouseUnitSummonBuildingUpgrade
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;		// 마을 번호
	SI32				siHouseUnitSlot;		// 시전 번호

	SI16				siSummonBuildingIndex;	// 목장 index

	SI16				siItemPos;				// 사용해야 할 아이템 위치
	cltItem				clItem;					// 사용해야 할 아이템 Unique, 개수

	SI16				siLevel;				// 목장 레벨

	sDBRequest_HouseUnitSummonBuildingUpgrade( SI32 CharID, SI32 PersonID, SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 SummonBuildingIndex,
		SI16 ItemPos, cltItem *pclItem,
		SI16 Level )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitSummonBuildingUpgrade );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSUMMONBUILDINGUPGRADE;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siSummonBuildingIndex = SummonBuildingIndex;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siLevel = Level;
	}
};

class sDBResponse_HouseUnitSummonBuildingUpgrade
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 1:성공, Other:실패

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;

	SI16				siItemPos;					// 사용하고 남은 아이템 정보
	cltItem				clLeftItem;
	
	SI16				siLevel;					// 목장 레벨

	sDBResponse_HouseUnitSummonBuildingUpgrade()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitSummonBuildingUpgrade ) );
	}
};

class sDBRequest_HouseUnitSummonIn
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siPersonSummonIndex;
	SI16				siSummonBuildingIndex;
			
	sDBRequest_HouseUnitSummonIn( SI32 CharID, SI32 PersonID,
		SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 PersonSummonIndex, SI16 SummonBuildingIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitSummonIn );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSUMMONIN;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siPersonSummonIndex = PersonSummonIndex;
		siSummonBuildingIndex = SummonBuildingIndex;
	}
};

class sDBResponse_HouseUnitSummonIn
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siPersonSummonIndex;

	SI16				siSummonBuildingIndex;
	cltSummon			clHouseUnitStgSummon;

	sDBResponse_HouseUnitSummonIn()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitSummonIn ) );
	}	
};

class sDBRequest_HouseUnitSummonOut
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;

	SI16				siPersonSummonIndex;	

	sDBRequest_HouseUnitSummonOut( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 SummonBuildingIndex, SI16 PersonSummonIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitSummonOut );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSUMMONOUT;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siSummonBuildingIndex = SummonBuildingIndex;

		siPersonSummonIndex = PersonSummonIndex;		
	}
	
};

class sDBResponse_HouseUnitSummonOut
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;
	
	SI16				siPersonSummonIndex;
	cltSummon			clPersonSummon;

	sDBResponse_HouseUnitSummonOut()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitSummonOut ) );
	}
};

class sDBRequest_HouseUnitSummonEat
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;

	SI16				siItemPos;
	cltItem				clItem;

	cltSummon			clSummon;

	sDBRequest_HouseUnitSummonEat( SI32 CharID, SI32 PersonID, 
		SI32 VillageUnique, SI32 HouseUnitSlot, 
		SI16 SummonBuildingIndex,
		SI16 ItemPos, cltItem *pclItem,
		cltSummon *pSummon
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_HouseUnitSummonEat );
		packetHeader.usCmd	= DBMSG_REQUEST_HOUSEUNITSUMMONEAT;

		siCharID = CharID;
		siPersonID = PersonID;

		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siSummonBuildingIndex = SummonBuildingIndex;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		clSummon.Set( pSummon );
	}
	
};

class sDBResponse_HouseUnitSummonEat
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI32				siHouseUnitSlot;

	SI16				siSummonBuildingIndex;
	
	SI16				siItemPos;
	cltItem				clLeftItem;

	cltSummon			clSummon;

	sDBResponse_HouseUnitSummonEat()
	{
		memset( this, 0, sizeof( sDBResponse_HouseUnitSummonEat ) );
	}
};

#endif