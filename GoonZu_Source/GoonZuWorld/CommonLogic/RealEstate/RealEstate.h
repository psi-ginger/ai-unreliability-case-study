#ifndef _REALESTATEMARKET_H
#define _REALESTATEMARKET_H

#include <Directives.h>

#include "..\common\Char\CharCommon\Char-EconomyInfo.h"

#include "..\StructureClass\StructureClass.h"

#define MAX_REALESTATEMARKET_ORDER_NUMBER			4000
#define MAX_REALESTATEMARKET_RESPONSE_ORDER_NUMBER	100

#define	MAX_REALESTATEMARKET_TAB_NUMBER				4

class cltTaxPaymentListDlg;

class cltRealEstateDetailInfo
{
public:
	cltRealEstateDetailInfo() { ZeroMemory( this, sizeof( cltRealEstateDetailInfo ) ); };
	~cltRealEstateDetailInfo() {};
	
	SI32				siVillageUnique;
	SI32				siHouseIndex;

	GMONEY				siCurrentRentFee;
	GMONEY				siRentFee;
	SI32				siRentPeriod;
	cltDate				clContractDate;

	SI32				siItemNum;
	SI32				siStableNum;
	SI32				siHorseNum;

};

//-------------------------------------------------
// 부동산 주문 정보
//-------------------------------------------------
class cltRealEstateOrder
{
public:
	SI32			siIndex;			// 주문 고유의 식별자(인덱스)
	SI32			siPersonID;			// 주문을 낸 PersonID
	GMONEY			siPrice;			// 주문 가격. 
	cltDate			clDate;				// 시간 정보. 

	SI32			siVillageUnique;	// 마을 부동산 소속마을 
	SI32			siHouseIndex;		// 시전 번호

	cltRealEstateOrder()
	{
		Init();
	}

	void Init()
	{
		siIndex = 0;
		siPersonID = 0;
		siPrice = 0;
		siVillageUnique = 0;
		siHouseIndex = 0;
	}

	void Set( SI32 orderindex, SI32 personid, GMONEY price, cltDate *pclDate, SI32 villageunique, SI32 houseindex )
	{
		siIndex = orderindex;
		siPersonID = personid;
		siPrice = price;
		clDate.Set( pclDate );
		siVillageUnique = villageunique;
		siHouseIndex = houseindex;
	}

	void Set( cltRealEstateOrder *pclOrder )
	{
		siIndex = pclOrder->siIndex;
		siPersonID = pclOrder->siPersonID;
		siPrice = pclOrder->siPrice;
		clDate.Set( &pclOrder->clDate );
		siVillageUnique = pclOrder->siVillageUnique;
		siHouseIndex = pclOrder->siHouseIndex;
	}

};

//-------------------------------------------------
// 개별적인 거간
//-------------------------------------------------
class cltRealEstateMarket : public cltStructureBasic{
private:

public:
	cltRealEstateMarket(SI32 sivillageunique);
	~cltRealEstateMarket();

};

//-----------------------------------------------
// 거간 관리자. 
//-----------------------------------------------
class cltRealEstateMarketManager : public cltStructureMngBasic{
public:

	cltRealEstateMarketManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltRealEstateMarketManager();

	void GetTitleText(SI32 villageunique);

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 거간을 신설한다.
	BOOL CreateRealEstateMarket(SI32 siunique);

	//void DispMsgInfo( HWND hDlg );
	//void ShowRealEstateMarketInfo();

	//void ShowMyHorseInfo( HWND hDlg );
	//void ShowMyAccountInfo( HWND hDlg );

	void ShowResponseOrder( HWND hDlg );

	BOOL ShowFeeRate(HWND hDlg, SI32 rate);
	
	BOOL ShowFee( HWND hDlg );
	BOOL ShowRealFee(HWND hDlg, GMONEY price);

	void AddOrder( cltRealEstateOrder *pclOrder );
	void DelOrder( SI32 orderIndex );
	void UpdatePrice( int index, cltRealEstateOrder *pclOrder );

	bool IsExistOrder( SI32 personid, SI32 villageunique, SI32 houseindex);

	// 내 주문 정보 갱신
	void AddMyOrder( cltRealEstateOrder *pclOrder );
	void DelMyOrder( SI32 orderIndex );

	void SetMyRealEstateList();

	void ShowMyRealEstateList(HWND hDlg);
	void ShowMyOrderList(HWND hDlg);

	void ShowDetailInfo( HWND hDlg );
	
	void Action( bool bdaychangedswitch );

public:	
	BOOL						m_bMainExplain;
	
	cltRealEstateOrder			*pclRealEstateOrder;
	SI32						siAddStartIndex;
	SI32						siTotalOrderNumber;

	BOOL						bSelectedSwitch;


#ifdef _SAFE_MEMORY
	NSafeTArray<cltRealEstateUnit, MAX_REAL_ESTATE_NUMBER_PER_PERSON>		clMyRealEstate;
	NSafeTArray<cltRealEstateOrder, MAX_REAL_ESTATE_NUMBER_PER_PERSON>		clMyOrder;
#else
	cltRealEstateUnit			clMyRealEstate[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];
	cltRealEstateOrder			clMyOrder[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];
#endif
	BOOL						bShowMyOrderSwitch;

	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltRealEstateOrder, MAX_REALESTATEMARKET_RESPONSE_ORDER_NUMBER>	clResponseOrder;
#else
	cltRealEstateOrder			clResponseOrder[ MAX_REALESTATEMARKET_RESPONSE_ORDER_NUMBER ];
#endif
	SI32						siTotalResponseOrderNumber;

	BOOL						bShowResponseOrderSwitch;

	cltRealEstateDetailInfo		clDetailInfo;
	BOOL						bRequestDetailInfo;
	BOOL						bUpdateDetailInfo;

	
	SI32						siSelectedIndex;					// 사자 주문 리스트
	SI32						siMyRealEstateSelectedIndex;		// 내 부동산 리스트
	SI32						siMyOrderSelectedIndex;				// 내 주문 리스트 

};

#endif
