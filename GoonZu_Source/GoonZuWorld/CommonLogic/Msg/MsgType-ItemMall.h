#ifndef _MSGTYPEITEMMALL_H
#define _MSGTYPEITEMMALL_H

#include "../../common/item/ItemCommon/ItemMallData.h"
#include "../../../DBManager/GameDBManager_world/DBMsg-ItemMall.h"

#define MAX_BILL_PAGGE_URL_LENGTH 1024



//---------------------------------------------------
// 아이템 몰에서 아이템을 샀었는가?
//---------------------------------------------------
class cltGameMsgResponse_ItemMallExistence{
public:
	bool	bExistence;

	cltGameMsgResponse_ItemMallExistence(bool existence)
	{
		bExistence = existence ;
	}
};

//---------------------------------------------------
// 아이템 몰에서 산 아이템 정보들을 요청
//---------------------------------------------------
class cltGameMsgRequest_ItemMallItemList{
public:
	SI32		siListPage;

	cltGameMsgRequest_ItemMallItemList(SI32 page)
	{
		siListPage = page;
	}
};

class cltGameMsgResponse_ItemMallItemList{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEMNUM_PER_ITEMMALL>		clItem;
	NSafeTArray<SI32, MAX_ITEMNUM_PER_ITEMMALL>			siItemID;
#else
	cltItem		clItem[MAX_ITEMNUM_PER_ITEMMALL];
	SI32		siItemID[MAX_ITEMNUM_PER_ITEMMALL]; // 아이템 번호
#endif
	SI32		siTotalItemNum;
	SI32		siListPage;

	cltGameMsgResponse_ItemMallItemList(cltItem *pclitem,SI32* itemid,SI32 totalitemnum,SI32 page)
	{
		for ( int i = 0 ; i < MAX_ITEMNUM_PER_ITEMMALL ; i ++ )
		{
			clItem[i].Set(&pclitem[i]);
			siItemID[i] = itemid[i] ;
		}

		siTotalItemNum = totalitemnum;
		siListPage = page;
	}
};

//---------------------------------------------------
// 아이템 몰에서 산 인벤토리로 이동을 요청
//---------------------------------------------------
class cltGameMsgRequest_ItemMallGetItem{
public:
	SI32		siItemID; // 아이템 번호
	SI32		siItemPos; // 아이템의 위치
	cltItem		clItem; // 아이템의 정보

	UI16		uiStr;
	UI16		uiDex;
	UI16		uiMag;
	UI16		uiVit;
	UI16		uiHnd;
	UI16		uiLuk;

	cltGameMsgRequest_ItemMallGetItem(SI32 itemid,SI32 itempos,cltItem *pclitem, UI16 Str = 0, UI16 Dex = 0,
								UI16 Mag = 0, UI16 Vit = 0, UI16 Hnd = 0, UI16 Luk = 0)
	{
		siItemID = itemid;
		siItemPos = itempos ;
		clItem.Set(pclitem);

		uiStr = Str;
		uiDex = Dex;
		uiMag = Mag;
		uiVit = Vit;
		uiHnd = Hnd;
		uiLuk = Luk;

	}
};

class cltGameMsgResponse_ItemMallGetItem{
public:
	SI16		siInvPos;
	cltItem		clItem;

	cltGameMsgResponse_ItemMallGetItem(SI16 pos,cltItem * pclitem)
	{
		siInvPos = pos;
		clItem.Set(pclitem);
	}
};

// 순간이동이용권을 이용하여 사냥터 정보창에서 이동하기
class cltGameMsgRequest_HuntMapWarp
{
public:
	SI32 siMapIndex;
	//KHY - 0703 - 게임 머니로 귀환하기.
	SI64 siWMoney;
	// by LEEKH - 이동시에 맵 안으로 들어가는가?
	bool	m_bInMap;

	cltGameMsgRequest_HuntMapWarp(SI32 mapindex , SI64 wMoney, bool bInMap = false)
	{
		siMapIndex = mapindex ;
		siWMoney = wMoney;
		m_bInMap = bInMap;
	}
};

// 순간이동이용권을 이용하여 개인상점으로 
class cltGameMsgRequest_PersonalShopWarp
{
public:
	SI32 siMapIndex;
	SI32 siX;
	SI32 siY;

	cltGameMsgRequest_PersonalShopWarp(SI32 mapindex, SI32 x, SI32 y)
	{
		siMapIndex = mapindex ;
		siX = x;
		siY = y;
	}
};

enum FRIENDWARP_MODE
{
	FRIENDWARP_MODE_NONE,
	FRIENDWARP_MODE_GUILD,
	FRIENDWARP_MODE_FRIEND,
};

// 순간이동이용권을 이용하여 친구에게로 
class cltGameMsgRequest_FriendWarp
{
public:
	SI32 siPersonID;
	SI16 siMode;	

	cltGameMsgRequest_FriendWarp(SI32 personid, SI16 mode)
	{
		siPersonID = personid ;
		siMode = mode;
	}
};

class cltGameMsgRequest_PartyWarp
{
public:
	TCHAR szName[MAX_PLAYER_NAME];

	cltGameMsgRequest_PartyWarp(TCHAR * name)
	{
		MStrCpy(szName, name, MAX_PLAYER_NAME );
	}
};


// 이름변경이용권을 이용하여 이름 변경하기
class cltGameMsgRequest_ChangeCharName
{
public:
	TCHAR szName[MAX_PLAYER_NAME];
	bool  m_bUseItem;				// [종호_NHN->NDOORS] 아이템을 이용해서 캐릭터명 변경이용할 경우
	SI08  m_siSlotIndex;			// [종호_NHN->NDOORS] 현재 선택된 캐릭터의 슬롯 인덱스


	cltGameMsgRequest_ChangeCharName(TCHAR * name, bool bUseItem)
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_ChangeCharName));

		MStrCpy( szName, name, MAX_PLAYER_NAME );
		m_bUseItem = bUseItem;
	}
};

class cltGameMsgResponse_ChangeCharName
{
public:
	SI32		siResult;
	TCHAR		szName[MAX_PLAYER_NAME];

	bool  m_bUseItem;				// [종호_NHN->NDOORS] 아이템을 이용해서 캐릭터명 변경이용할 경우

	cltGameMsgResponse_ChangeCharName(SI32 result,TCHAR * name, bool m_bUseItem = true)
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_ChangeCharName));

		siResult = result ;

		MStrCpy(szName, name, MAX_PLAYER_NAME );

		m_bUseItem = m_bUseItem;
	}
};

class cltGameMsgResponse_Change_CharKind
{
public:
	SI32		siResult;

	SI16		siKind;

	cltGameMsgResponse_Change_CharKind(SI32 result,SI16 kind)
	{
		siResult = result ;

		siKind = kind ;
	}
};

class cltGameMsgResponse_NoLimitTicket_Warp
{
public:

	SI32		siType;
	bool		bUsing;
	SI32		siDateVary;


	cltGameMsgResponse_NoLimitTicket_Warp( SI32 Type, bool use , SI32 datevary )
	{
		siType = Type;
		bUsing = use ;
		siDateVary = datevary ;
	}
};
// 티켓으로 해당 지역으로 직접 날라간다
class cltGameMsgRequest_WarpTargetWorld
{
public:
	SI32 siItemUnique;

	cltGameMsgRequest_WarpTargetWorld( SI32 ItemUnique )
	{
		siItemUnique = ItemUnique;
	}

};class cltGameMsgResponse_WarpTargetWorld
{
public:

	SI32 siItemUnique	;


	cltGameMsgResponse_WarpTargetWorld( SI32 siItemUnique )
	{
		siItemUnique = siItemUnique;
	}
};

// 아이템몰에서 대분류와 소분류를 기준으로 상품 리스트를 요청한다.
class cltGameMsgRequest_Get_Sel_PGList
{
public:
	SI32				siMainCategory; // 대분류
	SI32				siSubCategory; // 소분류

	TCHAR				szType[4] ; // 상품 구분

	SI32				siPage ; // 불러올 페이지

	cltGameMsgRequest_Get_Sel_PGList(SI32 main,SI32 sub,TCHAR* type,SI32 page)
	{
		siMainCategory = main ;
		siSubCategory = sub ;

		MStrCpy(szType,type,4) ;

		siPage = page ;
	}
};

class cltGameMsgResponse_Get_Sel_PGList
{
public:
	SI32				siResult ;		// 1외에는 실패

	SI32				siMainCategory; // 대분류
	SI32				siSubCategory; // 소분류

	TCHAR				szType[4] ; // 상품 구분

	SI32				siPage ; // 불러온 페이지
	SI32				siMaxPage;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemMallData, MAX_ITEMMALL_LIST_NUM>		clItemData;
#else
	cltItemMallData		clItemData[MAX_ITEMMALL_LIST_NUM]; // 상품 정보
#endif

	cltGameMsgResponse_Get_Sel_PGList(SI32 result,SI32 main,SI32 sub,TCHAR* type,SI32 page, SI32 maxpage, cltItemMallData* pclinfo)
	{
		siResult = result ;

		siMainCategory = main ;
		siSubCategory = sub;

		MStrCpy(szType,type,4);

		siPage = page;
		siMaxPage = maxpage;

		for ( SI32 i = 0 ; i < MAX_ITEMMALL_LIST_NUM ; i ++ )
			clItemData[i].Set(&pclinfo[i]);
	}
};

// 선택한 아이템의 상세 정보 및 그룹 아이템 정보를 불러온다.
class cltGameMsgRequest_Get_ItemMall_ItemInfo
{
public:
	SI32				siGroupKey; // 그룹키값

	cltGameMsgRequest_Get_ItemMall_ItemInfo(SI32 key)
	{
		siGroupKey = key;
	}
};

class cltGameMsgResponse_Get_ItemMall_ItemInfo
{
public:
	SI32				siResult ;		// 1외에는 실패

#ifdef _SAFE_MEMORY
	NSafeTArray<cltItemMallData, 3>			clItemData;
#else
	cltItemMallData		clItemData[3]; // 정보( 기간제만 아이템들 정보이다. )
#endif

	cltGameMsgResponse_Get_ItemMall_ItemInfo(SI32 result,cltItemMallData* pclinfo)
	{
		siResult = result ;

		for ( SI32 i = 0 ; i < 3 ; i ++ )
			clItemData[i].Set(&pclinfo[i]);
	}
};

// 내 캐쉬 정보를 가져온다.
class cltGameMsgResponse_Get_MyCash_Info
{
public:
	SI32				siResult;
	SI32				siRealCash; // 캐쉬 정보 ( 유가 )
	SI32				siBonusCash; // 보너스 캐쉬 정보 + 이벤트 캐쉬 정보 + etc

	cltGameMsgResponse_Get_MyCash_Info(SI32 result, SI32 real,SI32 bonus)
	{
		siResult = result;
		siRealCash = real ;
		siBonusCash = bonus ;
	}
};

// 아이템 사기
class cltGameMsgRequest_ItemMall_Buyitem
{
public:
	SI32				siProductKey ;
	SI32				siProductNum;
	SI32				siProductPrice;
	SI32				siProductEventPrice;	
	TCHAR				szItemName[50];
	TCHAR				szSecondPassWord[9];
	bool				bAgeLimit;				// [진성] 거래제한. KOR: 나이, ENG: 레벨 => 2008-9-22

	cltGameMsgRequest_ItemMall_Buyitem( SI32 key,SI32 num,SI32 price, SI32 EventPrice, TCHAR* itemname, TCHAR* SecondPassWord = NULL )
	{
		siProductKey		= key;
		siProductNum		= num;
		siProductPrice		= price;
		siProductEventPrice = EventPrice;

		szItemName[0]		= '\0';
		szSecondPassWord[0]	= '\0';

		if(itemname)		MStrCpy(szItemName, itemname, sizeof(szItemName));

		if( SecondPassWord )
		{
			//MStrCpy( szSecondPassWord, SecondPassWord, 9 );
			MStrCpy( szSecondPassWord, SecondPassWord,  sizeof(szSecondPassWord) );
		}
		else
		{
			szSecondPassWord[0] = '\0';
		}

		bAgeLimit = 0;
	}

	//[진성] 거래제한. 생성자 추가. KOR: 나이, ENG: 레벨 => 2008-9-22
	cltGameMsgRequest_ItemMall_Buyitem( SI32 key,SI32 num,SI32 price, SI32 EventPrice, TCHAR* itemname, bool _bAgeLimit )
	{
		siProductKey		= key ;
		siProductNum		= num ;
		siProductPrice		= price ;
		siProductEventPrice	= EventPrice ;

		szItemName[0]		= '\0';
		szSecondPassWord[0]	= '\0';

		if(itemname)		MStrCpy(szItemName, itemname, sizeof(szItemName));

		szSecondPassWord[0] = '\0';

		bAgeLimit = _bAgeLimit;
	}
};

class cltGameMsgResponse_ItemMall_Buyitem
{
public:
	SI32				siResult ;		// 1외에는 실패

	cltItemMallData		clItemData; // 산 아이템 정보

	SI32				siProductNum;
	SI32				siRealCash; // 캐쉬 정보 ( 유가 )
	SI32				siBonusCash; // 보너스 캐쉬 정보 + 이벤트 캐쉬 정보 + etc

	TCHAR				szCode[50];

	cltGameMsgResponse_ItemMall_Buyitem(SI32 result,cltItemMallData*pclinfo,SI32 num, SI32 realcash, SI32 bonuscash, TCHAR*code)
	{
		siResult = result ;

		clItemData.Set(pclinfo);

		siProductNum = num ;
		siRealCash = realcash;
		siBonusCash = bonuscash;

		MStrCpy(szCode,code,50);
	}
};

// 아이템 선물하기
class cltGameMsgRequest_ItemMall_Giftitem
{
public:
	TCHAR				szGetAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductKey;	// 선물품
	SI32				siProductNum;
	SI32				siProductPrice;
	SI32				siProductEventPrice;

	TCHAR				szComment[200];
	TCHAR				szItemName[50];
	
	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME];
    SI32				siItemImageFont;
	TCHAR				szSecondPassWord[9];

	cltGameMsgRequest_ItemMall_Giftitem(TCHAR*getid,SI32 key,SI32 num,SI32 price, SI32 EventPrice,TCHAR*comment,TCHAR* itemname,SI16 ServerUnique,TCHAR* CharName,SI32 ItemImageFont, TCHAR* SecondPassWord = NULL )
	{
		MStrCpy(szGetAccountID,getid,MAX_PLAYER_NAME);
		siProductKey		= key ;
		siProductNum		= num ;
		siProductPrice		= price ;
		siProductEventPrice = EventPrice ;

		MStrCpy(szComment,comment,200);
		MStrCpy(szItemName,itemname,50);

		siGetServerUnique = ServerUnique;
		MStrCpy( szGetCharName,CharName,MAX_PLAYER_NAME );		

		siItemImageFont = ItemImageFont;

		if( SecondPassWord )
		{
			MStrCpy( szSecondPassWord, SecondPassWord, 9 );
		}
		else
		{
			szSecondPassWord[0] = '\0';
		}

	}
};

class cltGameMsgResponse_ItemMall_Giftitem
{
public:
	SI32				siResult ;		// 1외에는 실패

	TCHAR				szGetCharName[MAX_PLAYER_NAME] ; // 선물 받을 사람 AccountID
	SI32				siProductKey;	// 선물품
	SI32				siProductNum;
	SI32				siProductPrice;
	SI32				siRealCash; // 캐쉬 정보 ( 유가 )
	SI32				siBonusCash; // 보너스 캐쉬 정보 + 이벤트 캐쉬 정보 + etc
		
	TCHAR				szCode[50];
	TCHAR				szItemName[50]; 	
	
	SI32				siHanFailReason;	// [진성] hangame 선물주기에 실패한 이유. => 2008-7-24
	
	cltGameMsgResponse_ItemMall_Giftitem(SI32 result,TCHAR*getname,SI32 key,SI32 num,SI32 price, SI32 realcash, SI32 bonuscash, TCHAR*code, TCHAR* itemname, SI32 _siHanFailReason)
	{
		siResult = result ;

		MStrCpy(szGetCharName,getname,MAX_PLAYER_NAME);
		siProductKey = key ;
		siProductKey = key ;
		siProductNum = num ;
		siProductPrice = price ;

		siRealCash = realcash;
		siBonusCash = bonuscash;

		MStrCpy(szCode,code,50);
		MStrCpy(szItemName,itemname,50);
		
		siHanFailReason = _siHanFailReason;
	}
};


// 아이디 검사하기
class cltGameMsgRequest_Check_AccountID
{
public:
	SI16				siServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME] ; // 선물 받을 계정의 캐릭터 이름


	cltGameMsgRequest_Check_AccountID( SI16 ServerUnique,  TCHAR* GetCharName)
	{
		siServerUnique = ServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME);
	}
};


class cltGameMsgResponse_Check_AccountID
{
public:
	SI32				siResult ;		// 1이면 아이디 존재, 나머지는 아이디 존재하지 않음
	
	SI16				siGetServerUnique;
	TCHAR				szGetCharName[MAX_PLAYER_NAME] ; // 선물 받을 계정의 캐릭터 이름
    
	TCHAR				szAccountID[MAX_PLAYER_NAME] ; // 선물 받을 사람 ID
	TCHAR				szAccountName[MAX_PLAYER_NAME] ;	// 선물 받을 사람 이름


	cltGameMsgResponse_Check_AccountID(SI32 result,  SI16 GetServerUnique,  TCHAR* GetCharName  ,TCHAR* account, TCHAR* name)
	{
		siResult = result ;

		siGetServerUnique = GetServerUnique;
		MStrCpy(szGetCharName,GetCharName,MAX_PLAYER_NAME);
		MStrCpy(szAccountID,account,MAX_PLAYER_NAME);
		MStrCpy(szAccountName,name,MAX_PLAYER_NAME);
	}
};


class cltGameMsgRequest_BillPage_URL
{
public:

	TCHAR				szAccountID[MAX_PLAYER_NAME] ; // 자신의 계정 ID
	TCHAR				szPassWord[FILE_NAME_SIZE] ;	// 자신의 계정 PASSWORD

	cltGameMsgRequest_BillPage_URL(TCHAR* account, TCHAR* password)
	{
		MStrCpy(szAccountID,account,MAX_PLAYER_NAME);
		MStrCpy(szPassWord,password,FILE_NAME_SIZE);
	}
};


class cltGameMsgResponse_BillPage_URL
{
public:
	
	TCHAR				szBillPageURL[MAX_BILL_PAGGE_URL_LENGTH];	// 암호화된 빌 페이지 URL
		
	cltGameMsgResponse_BillPage_URL(TCHAR* billpageurl)
	{
		MStrCpy(szBillPageURL,billpageurl,MAX_BILL_PAGGE_URL_LENGTH);		
	}
};

class cltGameMsgRequest_LoginPage_URL
{
public:

	TCHAR				szAccountID[MAX_PLAYER_NAME] ; // 자신의 계정 ID
	TCHAR				szPassWord[FILE_NAME_SIZE] ;	// 자신의 계정 PASSWORD
	TCHAR				szUrl[MAX_PATH];

	cltGameMsgRequest_LoginPage_URL(TCHAR* account, TCHAR* password, TCHAR* url)
	{
		MStrCpy(szAccountID,account,MAX_PLAYER_NAME);
		MStrCpy(szPassWord,password,FILE_NAME_SIZE);
		MStrCpy(szUrl, url, MAX_PATH);
	}
};


class cltGameMsgResponse_LoginPage_URL
{
public:

	TCHAR				szLoginPageURL[MAX_BILL_PAGGE_URL_LENGTH];	// 암호화된 문장
	TCHAR				szUrl[MAX_PATH];

	cltGameMsgResponse_LoginPage_URL(TCHAR* loginpageurl, TCHAR* url)
	{
		MStrCpy(szLoginPageURL, loginpageurl,MAX_BILL_PAGGE_URL_LENGTH);		
		MStrCpy(szUrl, url, MAX_PATH);
	}
};


class cltGameMsgRequest_GachaPage_URL
{
public:

	TCHAR				szAccountID[MAX_PLAYER_NAME] ; // 자신의 계정 ID
	TCHAR				szPassWord[FILE_NAME_SIZE] ;	// 자신의 계정 PASSWORD

	cltGameMsgRequest_GachaPage_URL(TCHAR* account, TCHAR* password)
	{
		MStrCpy(szAccountID,account,MAX_PLAYER_NAME);
		MStrCpy(szPassWord,password,FILE_NAME_SIZE);
	}
};


class cltGameMsgResponse_GachaPage_URL
{
public:

	TCHAR				szGachaPageURL[MAX_BILL_PAGGE_URL_LENGTH];	// 암호화된  페이지 URL

	cltGameMsgResponse_GachaPage_URL(TCHAR* gachapageurl)
	{
		MStrCpy(szGachaPageURL,gachapageurl,MAX_BILL_PAGGE_URL_LENGTH);		
	}
};


// 변신할 수 있는 영웅 정보 설정
class cltGameMsgResponse_Set_ChangeHero
{
public:
	SI16			siPos;
	cltItem			clitem;

	SI32			siCode;

	cltGameMsgResponse_Set_ChangeHero(SI16 pos,cltItem*pclitem,SI32 code)
	{
		siPos = pos ;
		clitem.Set(pclitem);

		siCode = code ;
	}
};

// 영웅으로 변신
class cltGameMsgRequest_Change_Hero
{
public:
	SI32			siCharKind; // 변신할 영웅
	bool			bChange; // 변신 아니면 해제

	cltGameMsgRequest_Change_Hero(SI32 kind,bool change)
	{
		siCharKind = kind ;
		bChange = change ;
	}
};

class cltGameMsgRequest_Present_Asnwer
{
public:
	SI16			siToServerUnique;
	TCHAR			szToCharName[MAX_PLAYER_NAME];
	TCHAR			szToAccountName[MAX_PLAYER_NAME];
	TCHAR			szComment[200];
	cltGameMsgRequest_Present_Asnwer(SI16 ToServerUnique, TCHAR * ToCharName, TCHAR * ToAccountName , TCHAR * Comment )
	{
		siToServerUnique = ToServerUnique;
		MStrCpy(szToCharName,ToCharName,MAX_PLAYER_NAME);
		MStrCpy(szToAccountName,ToAccountName,MAX_PLAYER_NAME);
		MStrCpy(szComment,Comment,200);
	}

};

//[진성] 프로모터가 새내기 캐릭에게 이동.
class cltGameMsgRequest_PromoterWarp
{
public:
	TCHAR	m_szName[MAX_PLAYER_NAME];

	cltGameMsgRequest_PromoterWarp( TCHAR*	_pszName )
	{
		ZeroMemory( this, sizeof(*this) );

		if( _pszName )
			StringCchCopy( m_szName, sizeof(m_szName), _pszName );
	}
};

#endif