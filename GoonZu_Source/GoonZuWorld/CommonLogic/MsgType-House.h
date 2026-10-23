#ifndef _MSGTYPEHOUSE_H
#define _MSGTYPEHOUSE_H

#include "House/House.h"

//-------------------------------------------
// 창고에서 아이템을 만든다. 
//-------------------------------------------
class cltGameMsgRequest_MakeItemInStg{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	SI16 siVillageUnique;
	SI16 siHouseIndex;

	cltGameMsgRequest_MakeItemInStg(SI16 itemunique, SI16 itemnum, SI16 villageunique, SI16 houseindex)
	{
		siItemUnique	= itemunique;
		siItemNum		= itemnum;

		siVillageUnique = villageunique;
		siHouseIndex	= houseindex;
	}

};

//------------------------------------
// 나의 창고 이름 수
//------------------------------------
class cltGameMsgRequest_ChangeHouseUnitName{
public:
	SI16 m_siVillageUnique;
	SI16 m_siHouseIndex;

	TCHAR m_szHouseName[MAX_PLAYER_NAME];

	cltGameMsgRequest_ChangeHouseUnitName(SI16 villageunique, SI16 houseindex,TCHAR* pszHouseName)
	{
		m_siVillageUnique = villageunique;
		m_siHouseIndex	= houseindex;

		_tcscpy(m_szHouseName,pszHouseName);
	}
};

//------------------------------------
// 창고 아이템 정리
//------------------------------------
class cltGameMsgResponse_ArrangeHouseItem{
public:
	SI16 m_siVillageUnique;
	SI16 m_siHouseIndex;

	cltGameMsgResponse_ArrangeHouseItem(SI16 villageunique, SI16 houseindex)
	{
		m_siVillageUnique = villageunique;
		m_siHouseIndex	= houseindex;
	}
};

//------------------------------------
// 나의 창고 이름 수정
//------------------------------------
class cltGameMsgResponse_ChangeHouseUnitName{
public:
	SI16 m_siVillageUnique;
	SI16 m_siHouseIndex;

	TCHAR m_szHouseName[MAX_PLAYER_NAME];

	cltGameMsgResponse_ChangeHouseUnitName(SI16 villageunique, SI16 houseindex,TCHAR* pszHouseName)
	{
		m_siVillageUnique = villageunique;
		m_siHouseIndex	= houseindex;

		_tcscpy(m_szHouseName,pszHouseName);
	}
};

//------------------------------------
// 나의 시전 유니트 리스트 전송. 
//------------------------------------
class cltGameMsgResponse_MyHouseUnitList{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMyHouseInfo, MAX_RENT_COUNT>		clMyHouseInfo;		
#else
	cltMyHouseInfo	clMyHouseInfo[MAX_RENT_COUNT];						
#endif

};

//------------------------------------
// 시전 유니트 리스트 전송. 
//------------------------------------
class cltGameMsgRequest_HouseUnitList{
public:
	SI32 siVillageUnique;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSimplePerson, MAX_HOUSEUNIT_NUMBER>		clPerson;
	NSafeTArray<bool, MAX_HOUSEUNIT_NUMBER>					bSell;		//[지연] - 부동산 거래 방식 수정 -  판매 정보 저장을 위한 공간 추가.
#else
	cltSimplePerson clPerson[MAX_HOUSEUNIT_NUMBER];						// 소유주. 
	bool			bSell[MAX_HOUSEUNIT_NUMBER];						// 판매 정보 저장을 위한 공간 추가.
#endif

	cltGameMsgRequest_HouseUnitList(SI32 villageunique)
	{
		siVillageUnique	= villageunique;
	}
};

//------------------------------------
// 시전 유니트 상세 정보 리스트 전송. 
//------------------------------------
class cltGameMsgRequest_HouseUnitListDetail{
public:
	SI32 siVillageUnique;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltRentContract, MAX_HOUSEUNIT_NUMBER>		clContract;
#else
	cltRentContract	clContract[MAX_HOUSEUNIT_NUMBER];			// 계약 조건. 
#endif

	cltGameMsgRequest_HouseUnitListDetail(SI32 villageunique)
	{
		siVillageUnique	= villageunique;
	}


};


//------------------------------------
// 시전 창고 정보 요청
//------------------------------------
class GameMsgRequest_HouseUnitStgInfo{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	GameMsgRequest_HouseUnitStgInfo(SI32 villageunique, SI32 houseunitslot)
	{
		siVillageUnique = villageunique;
		siHouseUnitSlot	= houseunitslot;
	}
};

//------------------------------------
// 시전 창고 정보 요청 응답
//------------------------------------
class GameMsgResponse_HouseUnitStgInfo{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	cltHouseUnitStorageInfo clStgInfo;

	GameMsgResponse_HouseUnitStgInfo(SI32 villageunique, SI32 houseunitslot, cltHouseUnitStorageInfo* pclinfo)
	{
		siVillageUnique = villageunique;
		siHouseUnitSlot	= houseunitslot;
		clStgInfo.Set(pclinfo);

	}

};

class cltGameMsgRequest_HouseCancellationPenaltyMoney
{
public:			
	SI32				siVillageUnique;	// 시전의 마을 유니크. 
	SI32				siHouseIndex;	

	cltGameMsgRequest_HouseCancellationPenaltyMoney(SI32 villageunique, SI32 houseindex) 
	{
		siVillageUnique	= villageunique;
		siHouseIndex	= houseindex;
	}
};


class cltGameMsgResponse_HouseCancellationPenaltyMoney
{
public:
	//SI32				siVillageUnique;	// 시전의 마을 유니크. 
	//SI32				siHouseIndex;	
	GMONEY				siPenaltyMoney;		// 지불할 위약금
	

	cltGameMsgResponse_HouseCancellationPenaltyMoney( SI32 PenaltyMoney) 
	{
	//	siVillageUnique	= villageunique;
	//	siHouseIndex	= houseindex;
		siPenaltyMoney  = PenaltyMoney;
	}
};


class cltGameMsgRequest_HouseCancellation
{
public:

	//whom				
	SI32				siVillageUnique;	// 시전의 마을 유니크. 
	// for what
	SI32				siHouseIndex;	
	//SI32				siPenaltyMoney;

	cltGameMsgRequest_HouseCancellation(SI32 villageunique, SI32 houseindex) //, SI32 PenaltyMoney)
	{
		siVillageUnique	= villageunique;
		siHouseIndex	= houseindex;
		//siPenaltyMoney = PenaltyMoney;
	}
};



class cltGameMsgRequest_HouseUnitStableCreate
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;
	
	SI16 siCreateStableIndex;				// 마구간 짓기를 원하는 곳은 번호

	cltGameMsgRequest_HouseUnitStableCreate( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 CreateStableIndex )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;
		siCreateStableIndex = CreateStableIndex;
	}
};

class cltGameMsgResponse_HouseUnitStableCreate
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;
	
	SI16 siCreatedStableIndex;		// 마구간 지어진곳
	
	
	//SI32 siLeftPersonMoney;			// 남은 돈
	GMONEY siCreatedStableFee;		// 마구간 짓는데 든 비용
	//cltMoney clHouseMoney;			// 시전에 남은돈
	
	//SI16 siItemPos;
	//cltItem clLeftItem;

	cltGameMsgResponse_HouseUnitStableCreate( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 CreatedStableIndex, 
		GMONEY CreatedStableFee )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siCreatedStableIndex = CreatedStableIndex;

		siCreatedStableFee = CreatedStableFee;

		//siItemPos = ItemPos;
		//clLeftItem.Set( pclLeftItem );
	}
};

//------------------------------------
// 시전 창고에 말을 넣어 달라고 요청
//------------------------------------
class cltGameMsgRequest_HouseUnitHorseIn
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;
	
	SI16 siHorseIndex;
	SI16 siHorseUnique;
	
	SI16 siStgHorseIndex;	

	cltGameMsgRequest_HouseUnitHorseIn( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 HorseIndex, SI16 HorseUnique, SI16 StgHorseIndex )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siHorseIndex = HorseIndex;
		siHorseUnique = HorseUnique;

		siStgHorseIndex = StgHorseIndex;
	}
};

// 이 메시지가 간다면, 데리고 다니던, 또는 타고 다니던 말이 창고에 들어가져야 함.
// 물론, 타고 다니던 또는 데리고 다니던 말은 없어져야지...
class cltGameMsgResponse_HouseUnitHorseIn
{
public:
	SI32		siVillageUnique;
	SI32		siHouseUnitSlot;

	SI16		siPersonHorseIndex;		// 사람이 데리고 다닌던 말들 중에서 빼줘야 하는 Index
	cltHorse	clPersonHorse;			// 시전에도 말이 있고, 사람에게도 말이 있다면 바꿈(change)

	SI16		siStgHorseIndex;
	cltHorse	clStgHorse;		

	cltGameMsgResponse_HouseUnitHorseIn( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 PersonHorseIndex, cltHorse *pclPersonHorse, SI16 StgHorseIndex, cltHorse *pclStgHorse )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siPersonHorseIndex = PersonHorseIndex;
		if(pclPersonHorse)	clPersonHorse.Set( pclPersonHorse );

		siStgHorseIndex = StgHorseIndex;
		if(pclStgHorse)		clStgHorse.Set( pclStgHorse );
	}
};

//------------------------------------
// 시전 창고에 말을 꺼내 달라고 요청
//------------------------------------
class cltGameMsgRequest_HouseUnitHorseOut
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siStgHorseIndex;
	SI16 siStgHorseUnique;

	cltGameMsgRequest_HouseUnitHorseOut( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 StgHorseIndex, SI16 StgHorseUnique )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siStgHorseIndex = StgHorseIndex;
		siStgHorseUnique = StgHorseUnique;
	}
};

class cltGameMsgResponse_HouseUnitHorseOut
{
public:
	SI32		siVillageUnique;
	SI32		siHouseUnitSlot;

	SI16		siStgHorseIndex;	// 창고의 말들 중에서 빼줘야 하는 Index
	cltHorse	clStgHorse;

	SI16		siHorseIndex;		// 내가 데리고 다니는 말의 배열 Index( 배열로 잡혀 있지만, 현재는 0번 배열 밖에 안 씀 )
	cltHorse	clHorse;			// 배열에 들어갈 말 정보
	
	cltGameMsgResponse_HouseUnitHorseOut( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 StgHorseIndex, cltHorse *pclStgHorse, SI16 HorseIndex, cltHorse *pclHorse )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;
		
		siStgHorseIndex = StgHorseIndex;
		clStgHorse.Set( pclStgHorse );

		siHorseIndex = HorseIndex;
		if(pclHorse)	clHorse.Set( pclHorse );
	}
};

//------------------------------------
// 시전 창고에 말 정보 변경 알림
//------------------------------------
class cltGameMsgResponse_HouseUnitHorseInfoSet
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siStgHorseIndex;	
	cltHorse clStgHorse;

	cltGameMsgResponse_HouseUnitHorseInfoSet( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 StgHorseIndex, cltHorse *pclHorse )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siStgHorseIndex = StgHorseIndex;
		if(pclHorse)	clStgHorse.Set( pclHorse );
	}
};

class cltGameMsgResponse_HouseUnitHorseEatFood
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siStgItemIndex;
	cltItem clStgItem;

	cltGameMsgResponse_HouseUnitHorseEatFood( SI32 VillageUnique, SI32 HouseUnitSlot, SI16 StgItemIndex, cltItem *pclItem )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siStgItemIndex = StgItemIndex;
		clStgItem.Set( pclItem );
	}
};

class cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate( SI32 VillageUnique, SI32 HouseUnitSlot )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;
	}
	
};

class cltGameMsgResponse_HouseUnitUsedGoodsBuildingCreate
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siItemPos;
	cltItem clLeftItem;

	cltGameMsgResponse_HouseUnitUsedGoodsBuildingCreate( SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 ItemPos, cltItem *pclLeftItem )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siItemPos = ItemPos;
		clLeftItem.Set( pclLeftItem );
	}
};

class cltGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo
{
public:
	cltItem		clItem;

	cltGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo( cltItem *pclItem )
	{
		clItem.Set( pclItem );
	}
};

class cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsInfo
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_MATERIAL_NUMBER>		clMakeableItem;
#else
	cltItem		clMakeableItem[ MAX_MATERIAL_NUMBER ];
#endif

	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsInfo( cltItem *pclMakeableItem )
	{	
#ifdef _SAFE_MEMORY
		clMakeableItem.MemCpy(pclMakeableItem);
#else
		memcpy( clMakeableItem, pclMakeableItem, sizeof( cltItem ) * MAX_MATERIAL_NUMBER );
#endif
	}
};

class cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siItemPos;
	cltItem clItem;

	cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods( SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 ItemPos, cltItem *pclItem )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class cltGameMsgResponse_HouseUnitMakeItemFromUsedGoods
{
public:
	SI32 siVillageUnique;
	SI32 siHouseUnitSlot;

	SI16 siSalvageItemPos;

	// 만들고 남은 최종 아이템
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_MATERIAL_NUMBER>			siToHouseUnitStgItemPos;
	NSafeTArray<cltItem, MAX_MATERIAL_NUMBER>		clMakeItem;
#else
	SI16				siToHouseUnitStgItemPos[ MAX_MATERIAL_NUMBER ];
	cltItem				clMakeItem[ MAX_MATERIAL_NUMBER ];
#endif

	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoods( SI32 VillageUnique, SI32 HouseUnitSlot,
		SI16 SalvageItemPos,
		SI16 *ToHouseUnitStgItemPos, cltItem *pclMakeItem )
	{
		siVillageUnique = VillageUnique;
		siHouseUnitSlot = HouseUnitSlot;

		siSalvageItemPos = SalvageItemPos;

#ifdef _SAFE_MEMORY
		siToHouseUnitStgItemPos.MemCpy(ToHouseUnitStgItemPos);
		clMakeItem.MemCpy(pclMakeItem);
#else
		memcpy( siToHouseUnitStgItemPos, ToHouseUnitStgItemPos, sizeof( SI16 ) * MAX_MATERIAL_NUMBER );
		memcpy( clMakeItem, pclMakeItem, sizeof( cltItem ) * MAX_MATERIAL_NUMBER );
#endif
	}
};

class cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsNotify
{
public:
	SI32 siCharUnique;
	SI16 siItemUnique;
	SI16 siMakePercent;
	
	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsNotify( SI32 CharUnique, SI16 ItemUnique, SI16 MakePercent )
	{
		siCharUnique = CharUnique;
		siItemUnique = ItemUnique;
		siMakePercent = MakePercent;
	}
};

#endif