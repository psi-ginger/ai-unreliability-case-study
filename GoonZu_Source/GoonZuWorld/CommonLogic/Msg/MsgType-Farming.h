#ifndef _MSGTYPEFARMING_H_
#define _MSGTYPEFARMING_H_

#include "..\..\Common\CommonHeader.h"

#include "../../Common/Item/ItemCommon/cltItem.h"
#include "../../Common/Farming/FarmingMgr.h"

#include "../../../DBManager/GameDBManager_World/dbMsg.h"

class cltGameMsgRequest_FarmingStart
{
public:
	SI16 siItemUnique;
	cltGameMsgRequest_FarmingStart( SI16 ItemUnique )
	{
		siItemUnique = ItemUnique;
	}
};

class cltGameMsgResponse_FarmingGetProducts
{
public:
	SI16 siKind;					// 누가 수확 했는지에 대한 정보를 가지고 있음.

	SI16 siItemUnique;

	SI16 siInvPos;
	cltItem clItem;

	cltGameMsgResponse_FarmingGetProducts( SI16 Kind, SI16 ItemUnique, SI16 InvPos, cltItem *pclItem )
	{
		siKind = Kind;
		siItemUnique = ItemUnique;

		siInvPos = InvPos;
		clItem.Set( pclItem );
	}
};

class cltGameMsgResponse_FarmingGetProductsNotify
{
public:
	SI32 siCharUnique;	
	SI16 siItemUnique;

	cltGameMsgResponse_FarmingGetProductsNotify( SI32 CharUnique, SI16 ItemUnique)
	{
		siCharUnique = CharUnique;
		siItemUnique = ItemUnique;
	}
};

class cltGameMsgRequest_FarmingPayRentFee
{
public:
	GMONEY siRentFee;
	
	SI16 siFarmingItemUnique;

	cltGameMsgRequest_FarmingPayRentFee( GMONEY RentFee, SI16 FarmingItemUnique )
	{
		siRentFee = RentFee;
		siFarmingItemUnique = FarmingItemUnique;
	}
};

#endif