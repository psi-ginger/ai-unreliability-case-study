#ifndef _MSGTYPE_MINING_H_
#define _MSGTYPE_MINING_H_

class cltGameMsgRequest_MiningStart
{
public:
	SI16 siVillageUnique;
	SI32 siFieldObjectUnique;

	cltGameMsgRequest_MiningStart( SI16 VillageUnique, SI32 FieldObjectUnique )
	{
		siVillageUnique = VillageUnique;
		siFieldObjectUnique = FieldObjectUnique;
	}
};

class cltGameMsgResponse_MiningGetProductsNotify
{
public:
	SI32 siCharUnique;
	SI16 siItemUnique;
	cltGameMsgResponse_MiningGetProductsNotify( SI32 CharUnique, SI16 ItemUnique )
	{
		siCharUnique = CharUnique;
		siItemUnique = ItemUnique;
	}
};

class cltGameMsgResponse_MiningForNotRentMan
{
public:
	SI16 siVillageUnique;
	GMONEY siRentPrice;

	cltGameMsgResponse_MiningForNotRentMan( SI16 VillageUnique, GMONEY RentPrice )
	{
		siVillageUnique = VillageUnique;
		siRentPrice = RentPrice;
	}
};

class cltGameMsgRequest_MiningPayRentFee
{
public:
	GMONEY siRentFee;
	SI32 siFieldObjectUnique; 

	cltGameMsgRequest_MiningPayRentFee( GMONEY RentFee, SI32 FieldObjectUnique)
	{
		siRentFee = RentFee;
		siFieldObjectUnique = FieldObjectUnique;
	}
};

#endif