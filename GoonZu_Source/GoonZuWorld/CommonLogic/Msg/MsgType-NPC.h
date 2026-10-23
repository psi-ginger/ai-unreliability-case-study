#ifndef _MSGTYPE_NPC_H_
#define _MSGTYPE_NPC_H_

#include "CommonLogic.h"
#include "server.h"

#include "../../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPC.h"
#include "../../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPCBuyCashItem.h"
#include "../../Common/SystemNPC/RPMerchantNPC/RPMerchantNPC.h"

#define SELL_TO_FOREIGNMERCHANT						21530

class cltGameMsgRequest_NPCSellItemInfo
{
public:
	SI32 siReq;
	SI32 siNPCKind;
	
	cltGameMsgRequest_NPCSellItemInfo( SI32 Req, SI32 NPCKind )
	{
		siReq = Req;
		siNPCKind = NPCKind;
	}
};

class cltGameMsgResponse_NPCSellItemInfo
{
public:
	SI32				siNPCKind;
	cltNPCInteInfoUnit	clNPCInteInfoUnitList[MAX_NPC_ITEM_NUMBER];

	cltGameMsgResponse_NPCSellItemInfo( SI32 NPCKind, cltNPCInteInfoUnit *pclNPCInteInfoUnitList )
	{
		siNPCKind = NPCKind;
		memcpy( clNPCInteInfoUnitList, pclNPCInteInfoUnitList, sizeof( cltNPCInteInfoUnit ) * MAX_NPC_ITEM_NUMBER );
	}
};

class cltGameMsgRequest_SellToForeignMerchant
{	
public:
	SI16					siForeignMerchantKind;		// 상인 구분(일본 상인, 청상인)
	UI16					uiSelectedIndex;			// 아이템 위치 (당연히 10이상의 값이겠지... 장착창에서는 바로 팔수 없으므로... )
	UI16					uiSellItemNum;				// 아이템 판매 개수
	SI16					siSellItemUnique;			// 아이템 유니크 값, <--- 버그의 위험성 때문에... 기간이 끝나는 시점에서 다른 아이템으로 변경되는 동안에 보내진 패킷일 경우에 대한 대비책
	SI32					siNPCCharUnique;			// NPC CharUnique

	//[추가중 : 황진성 2007. 11. 19 아이템 등급(고급, 일반 등등)]
	UI16					uiRareInfo;

	cltGameMsgRequest_SellToForeignMerchant()
	{
		siForeignMerchantKind = 0;
		uiSelectedIndex = 0;
		uiSellItemNum = 0;
		siSellItemUnique = 0;
		siNPCCharUnique = 0;
	}

	cltGameMsgRequest_SellToForeignMerchant(SI16 ForeignMerchantKind, UI16 SelectedIndex, UI16 SellItemNum, 
				SI16 SellItemUnique, SI32 NPCCharUnique, UI16 RareInfo)
	{
		siForeignMerchantKind = ForeignMerchantKind;
		uiSelectedIndex = SelectedIndex;
		uiSellItemNum = SellItemNum;
		siSellItemUnique = SellItemUnique;
		siNPCCharUnique = NPCCharUnique;

		//[추가중 : 황진성 2007. 11. 19 아이템 등급(고급, 일반 등등)]
        uiRareInfo = RareInfo;
	}


};

class cltGameMsgResponse_PreSellToForeignMerchant
{	
public:
	SI16					siRemainDay;
	SI16					siPercent;
	GMONEY					siPrice;

	//[추가 : 황진성 2007. 12. 5 명나라 상인에게 파는 물건의 등급.]
	UI16					uiRareInfo;
	// 내구도에 의한 패널티(100이면 패널티가 없는것임)
	SI16					siDurabilityPenalty;

	cltGameMsgResponse_PreSellToForeignMerchant()
	{
		siRemainDay			= 0;
		siPercent			= 0;
		siPrice				= 0;
		uiRareInfo			= 0;
		siDurabilityPenalty	= 0;
	}

	cltGameMsgResponse_PreSellToForeignMerchant(SI16 RemainDay, SI16 Percent, GMONEY Price, UI16 RareInfo, SI16 durabilityPenalty)
	{
		siRemainDay			= RemainDay;
		siPercent			= Percent;
		siPrice				= Price;
		uiRareInfo			= RareInfo;
		siDurabilityPenalty	= durabilityPenalty;
	}

};


class cltGameMsgRequest_ForeignMerchantNPCBuyInfo
{	
public:
	SI16					siForeignMerchantKind;

	cltGameMsgRequest_ForeignMerchantNPCBuyInfo( SI16 ForeignMerchantKind )
	{
		siForeignMerchantKind = ForeignMerchantKind;
	}
};

class cltGameMsgResponse_ForeignMerchantNPCBuyInfo
{	
public:
	SI16					siForeignMerchantKind;		// 상인 구분(일본 상인, 청상인)
	SI16					siBuyItemType;				// 상인이 구입중인 아이템의 타입
	SI16					siBuyItemUniqueList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];			// 아이템 unique
	GMONEY					siBuyItemAveragePriceList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];		// 아이템 구입 가격
	SI16					siBuyItemListCount;
	

	void Set( SI16 Kind, SI16 BuyItemType, SI16* UniqueList, GMONEY* PriceList, SI16 BuyItemListCount )
	{
		siForeignMerchantKind = Kind;
		for ( SI32 i = 0; i < BuyItemListCount; ++i )
		{
			siBuyItemUniqueList[ i ] = UniqueList[ i ];
			siBuyItemAveragePriceList[ i ] = PriceList[ i ];
		}
		siBuyItemType = BuyItemType;
		siBuyItemListCount = BuyItemListCount;
	}
};

class cltGameMsgRequest_ForeignMerchantBuyCashItemInfo
{
public:
	SI32 siItemType;

	cltGameMsgRequest_ForeignMerchantBuyCashItemInfo( SI32 ItemType )
	{	
		siItemType = ItemType;
	}
};

struct stItemInfoList
{
	SI16 siBuyItemUniqueList;		// 아이템 unique
	UI16 uiBuyItemPriceList;		// 아이템 구입 가격
};

class cltGameMsgResponse_ForeignMerchantBuyCashItemInfo
{
public:
	SI32 siBuyItemType;
	SI16 siBuyItemListCount;
	stItemInfoList ItemInfoList[ MAX_AVAILABLE_CASHITEMNUM ];

	void Set( SI32 BuyItemType, SI16* UniqueList, UI16* PriceList, SI16 BuyItemListCount )
	{	
		siBuyItemType = BuyItemType;

		for ( SI32 i = 0; i < BuyItemListCount; ++i )
		{
			ItemInfoList[ i ].siBuyItemUniqueList = UniqueList[ i ];
			ItemInfoList[ i ].uiBuyItemPriceList = PriceList[ i ];
		}
		
		siBuyItemListCount = BuyItemListCount;
	}

	SI32 GetTotalSize()
	{
		return  sizeof( SI32 ) + sizeof( SI16 ) + sizeof( stItemInfoList ) * siBuyItemListCount;
	}
};

class cltGameMsgRequest_ForeignMerchantBuyCashItem
{
public:
	SI16 siItemPos;
	cltItem clSellItem;

	cltGameMsgRequest_ForeignMerchantBuyCashItem( SI16 ItemPos, cltItem *pclItem )
	{
		siItemPos = ItemPos;
		clSellItem.Set( pclItem );
	}
};

class cltGameMsgResponse_ForeignMerchantBuyCashItem
{
public:
	SI16 siItemPos;
	cltItem clLeftItem;

	cltGameMsgResponse_ForeignMerchantBuyCashItem( SI16 ItemPos, cltItem *pclItem )
	{
		siItemPos = ItemPos;
		clLeftItem.Set( pclItem );
	}
};


class cltGameMsgResponse_GetRecommandPoint
{
public:
	
	SI16 siResult;
	SI32 siRecommandPoint;

	cltGameMsgResponse_GetRecommandPoint( SI16 Result, SI32 RecommandPoint )
	{
		siResult = Result;
        siRecommandPoint  = RecommandPoint;              		
	}
};


class cltGameMsgRequest_BuyRecommandProduct
{
public:
	SI32 siRecommandProductIndex;
    cltItem	sRecommandProduct;
    
	cltGameMsgRequest_BuyRecommandProduct(SI32 RecommandProductIndex,cltItem* RecommandProduct )
	{
		siRecommandProductIndex = RecommandProductIndex;
		sRecommandProduct.Set(RecommandProduct);
	}
};

class cltGameMsgResponse_BuyRecommandProduct
{
public:
	
	SI32	siUsedPoint;
	SI32	siRecommandPoint;
	cltItem	clBuyItem;
	
	cltGameMsgResponse_BuyRecommandProduct(SI32 UsedPoint , SI32 RecommandPoint, cltItem * pclBuyItem )
	{		
		siUsedPoint = UsedPoint;
		siRecommandPoint = RecommandPoint;
		clBuyItem.Set(pclBuyItem);
	}
};

class cltGameMsgRequest_RecommandPointList
{
public:
	SI16 siCurrentPage;

	cltGameMsgRequest_RecommandPointList(SI16 CurrentPage )
	{
		siCurrentPage = CurrentPage;
	}
    
};

class cltGameMsgResponse_RecommandPointList
{
public:
	SI16 siCurrentPage;
	SI16 siMaxPage;
	
	stRPListData	sRPListData[MAX_RPLISTDATA_PER_PAGE];
	
	cltGameMsgResponse_RecommandPointList(SI16 CurrentPage,SI16 MaxPage,stRPListData* RPListData )
	{
		siCurrentPage = CurrentPage;
		siMaxPage	  = MaxPage;
		memcpy(sRPListData,RPListData, sizeof(stRPListData) * MAX_RPLISTDATA_PER_PAGE );
	}
};

class cltGameMsgResponse_VillageHaveEventNPC
{
public:
	SI32 siNPCKind[MAX_HIRED_EVENTNPC_NUM];

	cltGameMsgResponse_VillageHaveEventNPC(void)
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_VillageHaveEventNPC));
	}

	cltGameMsgResponse_VillageHaveEventNPC(SI32 *npckind)
	{
		memcpy(siNPCKind,npckind, sizeof(siNPCKind));
	}

};

class cltGameMsgRequest_ManufactureNPC
{
public:
	SI32 siItemUnique;

	cltGameMsgRequest_ManufactureNPC( SI32 unique )
	{
		siItemUnique = unique;
	}
};

class cltGameMsgRequest_RecieveBeginnerItem
{
public:
	SI32 siItemUnique;

	cltGameMsgRequest_RecieveBeginnerItem( SI32 unique )
	{
		siItemUnique = unique;
	}
};

//[진성] 프로모터 아이템 정보.
class cltGameRequest_PromoterNPC_Give_Item_Get
{
public:
	cltGameRequest_PromoterNPC_Give_Item_Get()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

class cltGameResponse_PromoterNPC_Give_Item_Get
{
public:
	SI32	ItemUnique[MAX_CHARACTER_LEVEL+1];
	SI32	ItemNum[MAX_CHARACTER_LEVEL+1];

	cltGameResponse_PromoterNPC_Give_Item_Get( void )
	{
		ZeroMemory(this, sizeof(*this));
	}
};

class cltGameRequest_Promoter_Info_Total_Get
{
public:
	cltGameRequest_Promoter_Info_Total_Get( void )
	{

	}
};

//class cltGameRequest_Promoter_Info_Get
//{
//public:
//	cltGameRequest_Promoter_Info_Get( void )
//	{
//
//	}
//};

class cltGameResponse_Promoter_Info_Get
{
public:
	SI32			m_siRecommendPersonID;
	SI16			m_siCharKind;
	SI16			m_siLevel;
	TCHAR			m_szPromoterCharName[MAX_PLAYER_NAME];
	bool			m_bAuth;


	cltGameResponse_Promoter_Info_Get( SI32 _siRecommendPersonID, SI16 _siCharKind, SI16 _siLevel, bool _bAuth, TCHAR* _pszPromoterCharName )
	{
		ZeroMemory(this, sizeof(*this));

		m_siRecommendPersonID	= _siRecommendPersonID;
		m_siCharKind			= _siCharKind;
		m_siLevel				= _siLevel;
		m_bAuth					= _bAuth;
		_tcscpy( m_szPromoterCharName, _pszPromoterCharName );
	}
};

class cltGameRequest_Promoter_URL_Get
{
public:
	cltGameRequest_Promoter_URL_Get( void )
	{

	}
};

class cltGameResponse_Promoter_URL_Get
{
public:
	TCHAR 	m_szURL[NHNCHINAPROMOTER_URL_SIZE];
	
	cltGameResponse_Promoter_URL_Get( TCHAR* _pszURL )
	{	
		if( NULL == _pszURL ) return;		
		_tcscpy( m_szURL, _pszURL );
	}
};

class cltGameResponse_Promoter_Set
{
public:
	TCHAR 	m_szPromoterName[MAX_PLAYER_NAME];

	cltGameResponse_Promoter_Set( TCHAR* _pszPromoterName )
	{	
		if( NULL == _pszPromoterName ) return;		

		StringCchCopy( m_szPromoterName, sizeof(m_szPromoterName), _pszPromoterName );
	}
};


//[진성] 추천인을 소개하는 메시지.
class cltGameResponse_Recommend_Introduce
{
public:
	sPacketHeader		packetHeader;

	TCHAR				m_szRecommendName[MAX_PLAYER_NAME];
	SI32				m_siKind;
	bool				m_siIsConnect;

	cltGameResponse_Recommend_Introduce( TCHAR* _pszRecommendName, SI32 _siKind, bool _siIsConnect )
	{
		ZeroMemory( this, sizeof(cltGameResponse_Recommend_Introduce) );

		if( _pszRecommendName )
			StringCchCopy( m_szRecommendName, sizeof(m_szRecommendName), _pszRecommendName );

		m_siKind	  = _siKind;
		m_siIsConnect = _siIsConnect;
	}
};

//[진성] 추천인을 소개하는 메시지.
class cltGameResponse_Recommendee_Introduce
{
public:
	sPacketHeader		packetHeader;

	TCHAR				m_szRecommendeeName[MAX_PLAYER_NAME];
	SI32				m_siKind;
	bool				m_siIsConnect;

	cltGameResponse_Recommendee_Introduce( TCHAR* _pszRecommendeeName, SI32 _siKind, bool _siIsConnect )
	{
		ZeroMemory( this, sizeof(cltGameResponse_Recommendee_Introduce) );

		if( _pszRecommendeeName )
			StringCchCopy( m_szRecommendeeName, sizeof(m_szRecommendeeName), _pszRecommendeeName );

		m_siKind	  = _siKind;
		m_siIsConnect = _siIsConnect;
	}
};

// [춘기] 몬스터 생성 요청 2009-10-06
class cltGameRequest_MonsterCreate
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siMonsterKind;
	SI32			m_siMonLevel;
	SI32			m_siMonCreateNum;
	SI32			m_siAggro;
	SI32			m_siMonAC;
	SI32			m_siMinDmg;
	SI32			m_siMaxDmg;
	SI32			m_siMonStr;
	SI32			m_siMonDex;
	SI32			m_siMonVit;
	SI32			m_siMonIntl;
	SI32			m_siMonHnd;
	SI32			m_siMonLuk;
	SI32			m_siMonWis;
	SI32			m_siCriRate;
	SI32			m_siAttInter;
	SI32			m_siFire;
	SI32			m_siWater;
	SI32			m_siLand;
	SI32			m_siWind;
	SI32			m_siNature;

	cltGameRequest_MonsterCreate(SI32 siMonsterKind, SI32 siMonLevel, SI32 siMonCreateNum, SI32 siAggro, SI32 siMonAC, SI32 siMinDmg, SI32 siMaxDmg, SI32 siMonStr, SI32 siMonDex, SI32 siMonVit, SI32 siMonIntl, SI32 siMonHnd,
		                         SI32 siMonLuk, SI32 siMonWis, SI32 siCriRate, SI32 siAttInter, SI32 siFire, SI32 siWater, SI32 siLand, SI32 siWind, SI32 siNature)
	{
		ZeroMemory( this, sizeof(cltGameRequest_MonsterCreate) );

		m_siMonsterKind = siMonsterKind;
		m_siMonLevel = siMonLevel;
		m_siMonCreateNum = siMonCreateNum;
		m_siAggro = siAggro;
		m_siMonAC = siMonAC;
		m_siMinDmg = siMinDmg;
		m_siMaxDmg= siMaxDmg;
		m_siMonStr = siMonStr;
		m_siMonDex = siMonDex;
		m_siMonVit = siMonVit;
		m_siMonIntl = siMonIntl;
		m_siMonHnd = siMonHnd;
		m_siMonLuk = siMonLuk;
		m_siMonWis = siMonWis;
		m_siCriRate = siCriRate;
		m_siAttInter = siAttInter;
		m_siFire = siFire;
		m_siWater = siWater;
		m_siLand = siLand;
		m_siWind = siWind;
		m_siNature = siNature;
	}
};

class cltGameReponse_MonsterCreate
{
public:
	sPacketHeader	packetHeader;

	SI32	m_siReault;

	cltGameReponse_MonsterCreate(SI32 siResult)
	{
		m_siReault = siResult;
	}
};


//[진성] 발렌타인데이 아이템 포장의뢰
class cltGameRequest_ValentineDay_Item_Packing
{
public:
	sPacketHeader	packetHeader;
	
	cltItem			m_clPackingItem;	// 포장의뢰 아이템.

	cltGameRequest_ValentineDay_Item_Packing( cltItem* _pclPackingItem )
	{
		m_clPackingItem.Set( _pclPackingItem );
	}
};

//[진성] 발렌타인데이 아이템 포장의뢰
class cltGameResponse_ValentineDay_Item_Packing
{
public:
	sPacketHeader	packetHeader;
	
	cltItem			m_clResultItem[2];	// 의뢰결과 획득한 아이템.
	
	cltGameResponse_ValentineDay_Item_Packing( cltItem* _pclResultItem )
	{
		memcpy( m_clResultItem, _pclResultItem, sizeof(m_clResultItem) );
	}
};

//[진성] 발렌타인데이 아이템 구입
class cltGameRequest_ValentineDay_Buy_Item
{
public:
	sPacketHeader	packetHeader;

	cltItem			m_clBuyItem;	// 포장의뢰 아이템.
	GMONEY			m_UseMoney;

	cltGameRequest_ValentineDay_Buy_Item( cltItem* _pclBuyItem, GMONEY _UseMoney )
	{
		m_clBuyItem.Set( _pclBuyItem );
		m_UseMoney = _UseMoney;
	}
};

//[진성] 발렌타인데이 아이템 구입
class cltGameResponse_ValentineDay_Buy_Item
{
public:
	sPacketHeader	packetHeader;
	cltItem			m_clBuyItem;	// 포장의뢰 아이템.

	cltGameResponse_ValentineDay_Buy_Item( cltItem* _pclBuyItem )
	{
		m_clBuyItem.Set( _pclBuyItem );
	}
};

#define CONFESSION_MSG_NUM	128
//[진성] 발렌타인데이 고백
class cltGameRequest_ValentineDay_Confession_Question
{
public:
	sPacketHeader	packetHeader;

	TCHAR			m_szTargetName[MAX_PLAYER_NAME];		// 고백 당할사람의 이름.
	TCHAR			m_szMsg[CONFESSION_MSG_NUM];

	cltGameRequest_ValentineDay_Confession_Question( TCHAR* _pszTargetName, TCHAR* _pszMsg )
	{
		StringCchCopy( m_szTargetName,	sizeof(m_szTargetName), _pszTargetName);
		StringCchCopy( m_szMsg,			sizeof(m_szMsg),		_pszMsg);
	}
};

//[진성] 발렌타인데이 고백
class cltGameResponse_ValentineDay_Confession_Question
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siPersonID;
	TCHAR			m_szName[MAX_PLAYER_NAME];				// 고백을 하는 사람의 이름.
	TCHAR			m_szMsg[CONFESSION_MSG_NUM];

	cltGameResponse_ValentineDay_Confession_Question( SI32 _siPersonID, TCHAR* _pszName, TCHAR* _pszMsg )
	{
		m_siPersonID = _siPersonID;
		StringCchCopy( m_szName,	sizeof(m_szName),	_pszName);
		StringCchCopy( m_szMsg,		sizeof(m_szMsg),	_pszMsg);
	}
};

//[진성] 발렌타인데이 고백
class cltGameRequest_ValentineDay_Confession_Answer
{
public:
	sPacketHeader	packetHeader;

	SI32			m_siTargetPersonID;						// 고백을 하는 사람의 personID
	bool			m_bResult;

	cltGameRequest_ValentineDay_Confession_Answer( SI32 _siTargetPersonID, bool _bResult )
	{
		m_siTargetPersonID	= _siTargetPersonID;
		m_bResult			= _bResult;
	}
};

//[진성] 발렌타인데이 고백
class cltGameResponse_ValentineDay_Confession_Answer
{
public:
	sPacketHeader	packetHeader;

	bool			m_bResult;
	TCHAR			m_szName[MAX_PLAYER_NAME];		// 고백을 받은 사람의 이름. (답변자)
	
	cltGameResponse_ValentineDay_Confession_Answer( bool _bResult, TCHAR* _pszName )
	{
		m_bResult = _bResult;
		StringCchCopy( m_szName, sizeof(m_szName), _pszName);
	}
};


#endif