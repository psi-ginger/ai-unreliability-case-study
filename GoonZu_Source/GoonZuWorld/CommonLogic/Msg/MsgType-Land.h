#ifndef _MSGTYPELAND_H
#define _MSGTYPELAND_H

#include "../Land/Land.h"

//------------------------------------
// 농장 계약 정보 변경
//------------------------------------
class cltGameMsgRequest_LandChangeContract {
public:
	SI32 siVillageUnique;
	cltRentContract clRentContract;

	cltGameMsgRequest_LandChangeContract( SI32 villageunique, const cltRentContract* pclcontract )
	{
		siVillageUnique = villageunique;
		clRentContract.Set(pclcontract);
	}
};


//------------------------------------
// 농장 유니트 리스트 전송. 
//------------------------------------
class cltGameMsgResponse_LandUnitList{
public:
	SI32 siVillageUnique;
	cltSimplePerson clPerson[ MAX_LANDUNIT_NUMBER ];			// 소유주. 

	cltGameMsgResponse_LandUnitList( SI32 villageunique )
	{
		siVillageUnique	= villageunique;
	}
};


//------------------------------------
// 임차 신청 
//------------------------------------
class cltGameMsgRequest_LandRentApply
{
public:
	cltRentContract clRentContract;

	cltGameMsgRequest_LandRentApply( cltRentContract* pclcontract )
	{
		clRentContract.Set(pclcontract);
	}
};


//------------------------------------
// 임차 희망자 정보 
//------------------------------------
class cltGameMsgResponse_LandApplyerList
{
public:
	SI32				siVillageUnique;
	cltRentApplyer		clLandApplyer[ MAX_APPLY_PER_LAND ];

	cltGameMsgResponse_LandApplyerList( int villageunique ) 
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_LandApplyerList ) );

		siVillageUnique = villageunique;
	}



};


//------------------------------------
// 농장 유니트 상세 정보 리스트 전송. 
//------------------------------------
class cltGameMsgResponse_LandUnitListDetail{
public:
	SI32 siVillageUnique;
	cltRentContract	clContract[ MAX_LANDUNIT_NUMBER ];			// 계약 조건. 

	cltGameMsgResponse_LandUnitListDetail(SI32 villageunique)
	{
		siVillageUnique	= villageunique;
	}
};


//----------------------------------------
// 임대허가 
//----------------------------------------
class cltGameMsgRequest_LandRent{
public:
	SI32 siPersonID;		// 임대 희망자의 PersonID
	SI32 siVillageUnique;	// 농장이 위치한 마을의 VillageUnique
	SI32 siIndex;			// 임대할 빈 농장의 슬롯. 

	cltGameMsgRequest_LandRent( SI32 personid, SI32 villageunique, SI32 index )
	{
		siPersonID		= personid;
		siVillageUnique	= villageunique;
		siIndex			= index;
	}

};


#endif