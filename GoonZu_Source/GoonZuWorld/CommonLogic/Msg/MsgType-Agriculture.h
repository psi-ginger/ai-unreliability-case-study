#ifndef _MSGTYPEAGRICULTURE_H_
#define _MSGTYPEAGRICULTURE_H_

class cltGameMsgRequest_AgricultureAutoGain
{
public:
	SI08 siAgricultureType;
	bool bAuto;

	cltGameMsgRequest_AgricultureAutoGain( SI08 AgricultureType, bool Auto )
	{
		siAgricultureType = AgricultureType;
		bAuto = Auto;
	}
};

class cltGameMsgResponse_AgricultureAutoGain
{
public:
	SI08 siAgricultureType;
	bool bAuto;

	cltGameMsgResponse_AgricultureAutoGain( SI08 AgricultureType, bool Auto )
	{
		siAgricultureType = AgricultureType;
		bAuto = Auto;
	}
};

class cltGameMsgResponse_AgricultureGetProducts
{
public:
	
	SI16 siAddedInvPos;					// 들어간 위치
	SI16 siAddedItemNum;				// 추가된 아이템의 개수
	cltItem clLeftItem;					// 최종 아이템 정보

	SI32 siCharUnique;
	SI32 siKind;						

	SI16 siAgricultureType;

	cltGameMsgResponse_AgricultureGetProducts( SI16 AddedInvPos, SI16 AddedItemNum, cltItem *pclLeftItem, 
		SI32 CharUnique, SI32 Kind, SI16 AgricultureType )
	{
		siAddedInvPos = AddedInvPos;
		siAddedItemNum = AddedItemNum;
		clLeftItem.Set( pclLeftItem );

		siCharUnique = CharUnique;
		siKind = Kind;

		siAgricultureType = AgricultureType;
	}
};

class cltGameMsgResponse_AgricultureGetProductsNotify
{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	SI32 siCharUnique;
	SI32 siKind;

	SI16 siAgricultureType;

	cltGameMsgResponse_AgricultureGetProductsNotify( SI16 ItemUnique, SI16 ItemNum,
		SI32 CharUnique, SI32 Kind, SI16 AgricultureType )
	{
		siItemUnique = ItemUnique;
		siItemNum = ItemNum;

		siCharUnique = CharUnique;
		siKind = Kind;

		siAgricultureType = AgricultureType;
	}
};


class cltGameMsgResponse_AgricultureStart
{
public:
	SI16 siItemUnique;
	SI16 siSuccessPercent;
	SI16 siStructureType;
	SI32 siDelayTime;
	UI32 uiLeaseLandExpire;

	cltGameMsgResponse_AgricultureStart( SI16 ItemUnique, SI16 SuccessPercent, SI16 StructureType, SI32 DelayTime, SI32 LeaseLandExpire )
	{
		siItemUnique = ItemUnique;
		siSuccessPercent = SuccessPercent;
		siStructureType = StructureType;
		siDelayTime = DelayTime;
		uiLeaseLandExpire = LeaseLandExpire;
	}
};


class cltGameMsgResponse_AgriculturePayRentFee
{
public:
	SI16 siVillageUnique;
	SI16 siRankType;

	GMONEY siRentFee;
	GMONEY siLeftMoney;

	cltGameMsgResponse_AgriculturePayRentFee( GMONEY RentFee, GMONEY LeftMoney, SI16 VillageUnique, SI16 RankType )
	{
		siRentFee = RentFee;
		siLeftMoney = LeftMoney;

		siVillageUnique = VillageUnique;
		siRankType = RankType;
	}
};

class cltGameMsgResponse_AgricultureForNotRentMan
{
public:
	SI16 siVillageUnique;
	SI16 siItemUnique;
	SI16 siRankType;
	GMONEY siRentPrice;
	
	cltGameMsgResponse_AgricultureForNotRentMan( SI16 VillageUnique, GMONEY RentPrice, SI16 ItemUnique, SI16 RankType )
	{
		siVillageUnique = VillageUnique;
		siRentPrice = RentPrice;
		siItemUnique = ItemUnique;
		siRankType = RankType;
	}
};

class cltGameMsgResponse_AgricultureLeaseLeftTime
{
public:
	SI16 siType;
	SI32 siLeftTime;

	cltGameMsgResponse_AgricultureLeaseLeftTime( SI16 type, SI32 leftTime )
	{
		siType = type;
		siLeftTime = leftTime;
	}
};

#endif