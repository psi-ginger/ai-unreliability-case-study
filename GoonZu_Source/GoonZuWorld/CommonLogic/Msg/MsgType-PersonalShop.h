#ifndef _MSGTYPEPERSONALSHOP_H
#define _MSGTYPEPERSONALSHOP_H

#include "Char/CharCommon/Char-Common.h"

//-------------------------------------------
// 개인상점 배너 통보. 
//-------------------------------------------
class cltGameMsgRequest_PersonalShopBanner{
public:
	SI16 siCharUnique;	
	cltGameMsgRequest_PersonalShopBanner(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

class cltGameMsgResponse_PersonalShopBanner{
public:
	SI16 siCharUnique;		// 머릿말을 사용하는 캐릭터의 유니크. 
	SI16 siResult;
	cltShopBannerData clShopBanner;

	cltGameMsgResponse_PersonalShopBanner(SI16 charunique, SI16 result, cltShopBannerData *pbannerdata)
	{
		siCharUnique = charunique;
		siResult     = result;

		clShopBanner.Set(pbannerdata);
	}
};


//-------------------------------------------
// 개인상점 데이터 통보 
//-------------------------------------------
class cltGameMsgRequest_PersonalShopData{
public:
	SI16 siCharUnique;
	BOOL bManufacture;
	BOOL bMaker	;	// 자신이 제작자인지 아닌지 표시 
	
	cltGameMsgRequest_PersonalShopData(SI16 charunique,BOOL Manufacture, BOOL Maker ) // Manufacture 가 참이면 아이템 제조 의뢰
	{
		siCharUnique = charunique;
		bManufacture = Manufacture;
		bMaker = Maker;
	}
};

class cltGameMsgResponse_PersonalShopData{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	cltShopTradeData clShopData;
	
	cltGameMsgResponse_PersonalShopData(SI16 charunique, cltShopTradeData * pshopdata)
	{
		siCharUnique = charunique;
		clShopData.Set(pshopdata);
	}
};

//-------------------------------------------
// 개인상점 배너 갱신 응답 
//-------------------------------------------

class cltGameMsgResponse_UpdatePersonalShopBanner{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
	cltShopBannerData clShopBanner;

	cltGameMsgResponse_UpdatePersonalShopBanner(SI16 charunique, SI16 result, cltShopBannerData * pbannerdata)
	{
		siCharUnique = charunique;
		siResult	 = result;
		clShopBanner.Set(pbannerdata);
	}
};

//-------------------------------------------
// 개인상점 데이터 갱신 응답 
//-------------------------------------------

class cltGameMsgResponse_UpdatePersonalShopData{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	cltShopTradeData clShopData;
	SI32 siItemUnique;
	SI32 siItemNum;
	GMONEY siItemCoast;
	
	BOOL bBuyer;


	cltGameMsgResponse_UpdatePersonalShopData(SI16 charunique, cltShopTradeData * pshopdata,SI32 itemunique = 0, SI32 itemnum = 0, GMONEY itemcoast = 0, BOOL buyer = FALSE)
	{
		siCharUnique = charunique;
		clShopData.Set(pshopdata);
		siItemUnique = itemunique;
		siItemNum = itemnum;
		
		siItemCoast = itemcoast;
		bBuyer = buyer;
	}
};


//-------------------------------------------
// 개인상점 데이터 시작 
//-------------------------------------------
class cltGameMsgRequest_StartPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 

	cltShopTradeData clShopData;

	cltGameMsgRequest_StartPersonalShop(SI16 charunique, cltShopTradeData * pshopdata)
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_StartPersonalShop));

		siCharUnique = charunique;
		clShopData.Set(pshopdata);
	}
};

class cltGameMsgResponse_StartPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
							// 0 : 실패 
							//-1 : [개인상점이용권]
	bool bPremium;			// 프리미엄 여부
	SI08 m_siShopMode;		// 샵이 열린게 무슨 모드인지 

	cltGameMsgResponse_StartPersonalShop(SI16 charunique, SI16 shopresult,bool premium, SI08 siShopMode)
	{
		siCharUnique = charunique;
		siResult = shopresult;
		bPremium = premium;
		m_siShopMode = siShopMode;
	}

};

//-------------------------------------------
// 개인상점 종료 
//-------------------------------------------
class cltGameMsgRequest_StopPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	cltGameMsgRequest_StopPersonalShop(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

class cltGameMsgResponse_StopPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI32 siPersonID;
	SI16 siResult;
	
	cltGameMsgResponse_StopPersonalShop(SI16 charunique, SI32 personid, SI16 result)
	{
		siCharUnique = charunique;
		siPersonID   = personid;
		siResult	 = result;
	}
};


//-------------------------------------------
// 본인이 물품구입 
//-------------------------------------------

#define BUYPERSONALSHOP_SUCCESS_BUY			 1	// 구매 성공. 
#define BUYPERSONALSHOP_FAIL_UNKNOWN		 0	// 구매 실패 
#define BUYPERSONALSHOP_FAIL_SHOPPERSTATUS  -1	// 구매 실패 - 상점 주인 설정이 바뀌었다.
#define BUYPERSONALSHOP_FAIL_BUYERSTATUS    -2	// 구매 실패 - 구매자의 정보에 오류가 있다.
#define BUYPERSONALSHOP_FAIL_SHOWCASESTATUS -3	// 구매 실패 - 상점의 물품 정보가 바뀌었다.
#define BUYPERSONALSHOP_FAIL_BUY_NOITEM		-4	// 구매 실패 - 상점주인이 물건이 없어서 
#define BUYPERSONALSHOP_FAIL_BUY_NOMONEY	-5	// 구매 실패 - 구매자가   돈이 없어서 
#define BUYPERSONALSHOP_FAIL_BUY_NOINVSPACE	-6	// 구매 실패 - 구매자의 인벤토리가 꽉차있다.


class cltGameMsgRequest_BuyPersonalShop{
public:

	// 상점주인의 정보 
	SI16			siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI08		    siShopMode;		// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	SI32			siPersonID;

	cltItem			clItem;
	GMONEY			siPrice;
	SI16			siShowcaseIndex;

	cltGameMsgRequest_BuyPersonalShop(SI16 charunique, SI08 shopmode, SI32 personid, cltItem *pitem, GMONEY price, SI16 showcaseindex )
	{
		siCharUnique = charunique;
		siShopMode   = shopmode;
		siPersonID   = personid;
		clItem.Set(pitem);
		siPrice		= price;
		siShowcaseIndex = showcaseindex;
	}
};

class cltGameMsgResponse_BuyPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
	SI32 siitemUnique;
	SI32 siItemNum;
	SI32 siItemCoast;
	cltGameMsgResponse_BuyPersonalShop(SI16 charunique, SI16 shopresult, SI32 itemunique, SI32 itemnum = 0)
	{
		siCharUnique = charunique;
		siResult     = shopresult;
		siitemUnique = itemunique;
		siItemNum    = itemnum;

	}
};

//-------------------------------------------
// 본인이 물품판매 
//-------------------------------------------
#define SELLPERSONALSHOP_SUCCESS_SELL			 1	// 판매 성공. 
#define SELLPERSONALSHOP_FAIL_UNKNOWN			 0	// 판매 실패 
#define SELLPERSONALSHOP_FAIL_SHOPPERSTATUS		-1	// 판매 실패 - 상점 주인 설정이 바뀌었다.
#define SELLPERSONALSHOP_FAIL_SELLERSTATUS		-2	// 판매 실패 - 판매자의 정보에 오류가 있다.
#define SELLPERSONALSHOP_FAIL_SHOWCASESTATUS	-3	// 판매 실패 - 상점의 물품 정보가 바뀌었다.
#define SELLPERSONALSHOP_FAIL_SELL_NOITEM		-4	// 판매 실패 - 판매자의 물건이 없어서 
#define SELLPERSONALSHOP_FAIL_SELL_NOMONEY		-5	// 판매 실패 - 상점주인의 돈이 없어서 
#define SELLPERSONALSHOP_FAIL_SELL_NOINVSPACE	-6	// 판매 실패 - 상점주인의 인벤토리가 꽉차있다.


class cltGameMsgRequest_SellPersonalShop{
public:
	// 상점주인의 정보 
	SI16			siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI08		    siShopMode;		// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	SI32			siPersonID;

	cltItem			clItem;
	GMONEY			siPrice;
	SI16			siShowcaseIndex;	
	SI16			siSellerInventoryRealIndex;

	cltGameMsgRequest_SellPersonalShop(SI16 charunique, SI08 shopmode, SI32 personid, cltItem *pitem, GMONEY price, SI16 showcaseindex, SI16 sellerinvindex )
	{
		siCharUnique = charunique;
		siShopMode   = shopmode;
		siPersonID   = personid;
		clItem.Set(pitem);
		siPrice		= price;
		siShowcaseIndex = showcaseindex;
		siSellerInventoryRealIndex = sellerinvindex;
	}
};

class cltGameMsgResponse_SellPersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
	SI32 siitemUnique;
	SI32 siItemNum;
	SI32 siItemCoast;
	cltGameMsgResponse_SellPersonalShop(SI16 charunique, SI16 shopresult, SI32 itemunique, SI32 itemnum = 0)
	{
		siCharUnique = charunique;
		siResult     = shopresult;
		siitemUnique = itemunique;
		siItemNum    = itemnum;

	}
};


//-------------------------------------------
// 본인이 물품제작 
//-------------------------------------------
#define MAKEPERSONALSHOP_SUCCESS_MAKE			 1	// 제작 성공. 
#define MAKEPERSONALSHOP_FAIL_UNKNOWN			 0	// 제작 실패 
#define MAKEPERSONALSHOP_FAIL_SHOPPERSTATUS		-1	// 제작 실패 - 상점 주인 설정이 바뀌었다.
#define MAKEPERSONALSHOP_FAIL_MAKERSTATUS		-2	// 제작 실패 - 제조자의  정보에 오류가 있다.
#define MAKEPERSONALSHOP_FAIL_SHOWCASESTATUS	-3	// 제작 실패 - 상점의 물품 정보가 바뀌었다.
#define MAKEPERSONALSHOP_FAIL_MAKE_NOITEM		-4	// 제작 실패 - 상점주인이 의뢰 물건이 없어서 
#define MAKEPERSONALSHOP_FAIL_MAKE_NOMONEY		-5	// 제작 실패 - 상점주인이 돈이 없어서 
#define MAKEPERSONALSHOP_FAIL_MAKE_NEEDABILITY	-6	// 제작 실패 - 제조자의 능력이 부족하다.
#define MAKEPERSONALSHOP_FAIL_NOWMAKINGSTATUS	-7	// 제작 실패 - 다른사람의 물건을 제작 중이다.


class cltGameMsgRequest_MakePersonalShop{
public:

	// 상점주인의 정보 
	SI16			siCharUnique;	// 개인상점을 여는  캐릭터의 유니크. 
	SI08		    siShopMode;		// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	SI32			siPersonID;

	cltItem			clItem;
	GMONEY			siPrice;
	SI16			siHandPoint;
	SI16			siSkillLevel;
	SI16			siShowcaseIndex;
	
	SI08			siType;

	cltGameMsgRequest_MakePersonalShop(SI16 charunique, SI08 shopmode, SI32 personid, cltItem *pitem, GMONEY price, SI16 handpoint, SI16 skilllevel,SI16 showcaseindex,SI08 type )
	{
		siCharUnique = charunique;
		siShopMode   = shopmode;
		siPersonID   = personid;
		clItem.Set(pitem);
		siPrice		= price;
		siHandPoint  = handpoint;
		siSkillLevel = skilllevel;
		siShowcaseIndex = showcaseindex;

		siType = type;
	}
};

class cltGameMsgResponse_MakePersonalShop{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
	SI32 siitemUnique;
	SI32 siItemNum;
	cltGameMsgResponse_MakePersonalShop(SI16 charunique, SI16 shopresult, SI32 itemunique, SI32 itemnum)
	{
		siCharUnique = charunique;
		siResult     = shopresult;
		siitemUnique = itemunique;
		siItemNum    = itemnum;
	}
};

class cltGameMsgRequest_MakePersonalShop_Manufactuer{
public:

	// 상점주인의 정보 
	SI16			siCharUnique;	// 개인상점을 여는  캐릭터의 유니크. 
	SI08		    siShopMode;		// PSHOP_MODE_BUY, PSHOP_MODE_SELL
	SI32			siPersonID;

	cltItem			clItem;
	GMONEY			siPrice;
	SI16			siHandPoint;
	SI16			siSkillLevel;
	SI16			siShowcaseIndex;

	SI08			siType;

	cltGameMsgRequest_MakePersonalShop_Manufactuer(SI16 charunique, SI08 shopmode, SI32 personid, cltItem *pitem, GMONEY price, SI16 handpoint, SI16 skilllevel,SI16 showcaseindex,SI08 type )
	{
		siCharUnique = charunique;
		siShopMode   = shopmode;
		siPersonID   = personid;
		clItem.Set(pitem);
		siPrice		= price;
		siHandPoint  = handpoint;
		siSkillLevel = skilllevel;
		siShowcaseIndex = showcaseindex;

		siType = type;
	}
};

class cltGameMsgResponse_MakePersonalShop_Manufacture{
public:
	SI16 siCharUnique;		// 개인상점을 여는  캐릭터의 유니크. 
	SI16 siResult;			// 개인상점의 실패여부 
	SI32 siitemUnique;
	SI32 siItemNum;
	cltGameMsgResponse_MakePersonalShop_Manufacture(SI16 charunique, SI16 shopresult, SI32 itemunique, SI32 itemnum)
	{
		siCharUnique = charunique;
		siResult     = shopresult;
		siitemUnique = itemunique;
		siItemNum    = itemnum;
	}
};

//-------------------------------------------
// 상점 검색 
//-------------------------------------------
class cltGameMsgRequest_SearchPersonalShopItem{
public:
	SI32 siShopMode;
	SI32 siItemUniqueType;		// 검색 물품 Unique
	GMONEY siMaxPrice;			// 가격이하의 물품 검색 
	cltGameMsgRequest_SearchPersonalShopItem(SI32 shopmode, SI32 itemuniquetype, GMONEY maxprice)
	{
		siShopMode   = shopmode;
		siItemUniqueType = itemuniquetype;
		siMaxPrice   = maxprice;
	}
};

class cltGameMsgResponse_SearchPersonalShopItem{
public:
	SI16 siResult;
	cltSearchShopData	clSearchShopData;
	
	cltGameMsgResponse_SearchPersonalShopItem(SI16 result, cltSearchShopData *psearchshopdata)
	{
		siResult	 = result;
		clSearchShopData.Set(psearchshopdata);
	}
};

class cltGameMsgRequest_SearchManufacture{
public:
	SI32 siShopMode;
	SI32 siItemUniqueType;		// 검색 물품 Unique
	cltGameMsgRequest_SearchManufacture(SI32 shopmode, SI32 itemuniquetype)
	{
		siShopMode   = shopmode;
		siItemUniqueType = itemuniquetype;
	}
};

class cltGameMsgResponse_SearchManufacture{
public:
	SI16 siResult;
	cltSearchShopData	clSearchShopData;

	cltGameMsgResponse_SearchManufacture(SI16 result, cltSearchShopData *psearchshopdata)
	{
		siResult	 = result;
		clSearchShopData.Set(psearchshopdata);
	}
};


#endif