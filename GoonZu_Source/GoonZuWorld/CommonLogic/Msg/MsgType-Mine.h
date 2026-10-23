#ifndef _MSGTYPEMINE_H
#define _MSGTYPEMINE_H

#include "../Mine/Mine.h"

//------------------------------------
// 농장 계약 정보 변경
//------------------------------------
class cltGameMsgRequest_MineChangeContract {
public:
	SI32 siVillageUnique;
	cltRentContract clRentContract;

	cltGameMsgRequest_MineChangeContract( SI32 villageunique, const cltRentContract* pclcontract )
	{
		siVillageUnique = villageunique;
		clRentContract.Set(pclcontract);
	}
};


//------------------------------------
// 농장 유니트 리스트 전송. 
//------------------------------------
class cltGameMsgResponse_MineUnitList{
public:
	SI32 siVillageUnique;
	cltSimplePerson clPerson[ MAX_MINEUNIT_NUMBER ];			// 소유주. 

	cltGameMsgResponse_MineUnitList( SI32 villageunique )
	{
		siVillageUnique	= villageunique;
	}
};


//------------------------------------
// 임차 신청 
//------------------------------------
class cltGameMsgRequest_MineRentApply
{
public:
	cltRentContract clRentContract;

	cltGameMsgRequest_MineRentApply( cltRentContract* pclcontract )
	{
		clRentContract.Set(pclcontract);
	}
};


//------------------------------------
// 임차 희망자 정보 
//------------------------------------
class cltGameMsgResponse_MineApplyerList
{
public:
	SI32				siVillageUnique;
	cltRentApplyer		clMineApplyer[ MAX_APPLY_PER_MINE ];

	cltGameMsgResponse_MineApplyerList( int villageunique ) 
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_MineApplyerList ) );

		siVillageUnique = villageunique;
	}



};


//------------------------------------
// 농장 유니트 상세 정보 리스트 전송. 
//------------------------------------
class cltGameMsgResponse_MineUnitListDetail{
public:
	SI32 siVillageUnique;
	cltRentContract	clContract[ MAX_MINEUNIT_NUMBER ];			// 계약 조건. 

	cltGameMsgResponse_MineUnitListDetail(SI32 villageunique)
	{
		siVillageUnique	= villageunique;
	}
};


//----------------------------------------
// 임대허가 
//----------------------------------------
class cltGameMsgRequest_MineRent{
public:
	SI32 siPersonID;		// 임대 희망자의 PersonID
	SI32 siVillageUnique;	// 농장이 위치한 마을의 VillageUnique
	SI32 siIndex;			// 임대할 빈 농장의 슬롯. 

	cltGameMsgRequest_MineRent( SI32 personid, SI32 villageunique, SI32 index )
	{
		siPersonID		= personid;
		siVillageUnique	= villageunique;
		siIndex			= index;
	}

};


#endif