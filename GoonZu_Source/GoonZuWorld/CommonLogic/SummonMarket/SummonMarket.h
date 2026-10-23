#ifndef _SUMMONMARKET_H
#define _SUMMONMARKET_H

#include <Directives.h>

#include "..\StructureClass\StructureClass.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

#define MAX_SUMMONMARKET_ORDER_NUMBER			10000
#define MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER	60
#define MAX_SUMMONMARKET_ORDER_PER_PERSON		3

#define	MAX_SUMMONMARKET_TAB_NUMBER				4

class cltTaxPaymentListDlg;


//-------------------------------------------------
// 소환수 주문 정보
//-------------------------------------------------
class cltSummonOrder
{
public:
	SI32			siIndex;		// 주문 고유의 식별자(인덱스)
	SI32			siPersonID;		// 주문을 낸 PersonID
	GMONEY			siPrice;		// 주문 가격. 
	cltDate			clDate;			// 시간 정보. 

	cltSummon		clSummon;		// 소환수정보

	SI32			siSelectPersonID;	// 구입 지정된 PersonID (0일경우 지정거래아님)


	cltSummonOrder()
	{
		Init();
	}

	void Init()
	{
		siIndex = 0;
		siPersonID = 0;
		siPrice = 0;

		clSummon.Init();

		siSelectPersonID = 0;
	}

	void Set( cltSummonOrder *pclOrder )
	{
		siIndex = pclOrder->siIndex;
		siPersonID = pclOrder->siPersonID;
		siPrice = pclOrder->siPrice;
		clDate.Set( &pclOrder->clDate );

		clSummon.Set( &pclOrder->clSummon );

		siSelectPersonID = pclOrder->siSelectPersonID;
	}

};


//-------------------------------------------------
// 개별적인 소환수거래소
//-------------------------------------------------
class cltSummonMarket : public cltStructureBasic{
private:

public:
	cltSummonMarket(SI32 sivillageunique);
	~cltSummonMarket();

};


//-----------------------------------------------
// 소환수거래소 관리자. 
//-----------------------------------------------
class cltSummonMarketManager : public cltStructureMngBasic{
public:

	cltSummonMarketManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltSummonMarketManager();

	void GetTitleText(SI32 villageunique);

	// 최초 환영 인사소환수표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 소환수거래소를 신설한다.
	BOOL CreateSummonMarket(SI32 siunique);

	void DispMsgInfo( HWND hDlg );
	void ShowSummonMarketInfo();

	BOOL ShowFeeRate(HWND hDlg, SI32 rate);
	
	BOOL ShowFee( HWND hDlg );
	BOOL ShowRealFee(HWND hDlg, GMONEY price);

	void AddOrder( cltSummonOrder *pclOrder );
	void DelOrder( SI32 orderIndex );
	void UpdatePrice( int index, cltSummonOrder *pclOrder );	

	// 내 주문 정보 갱신
	void AddSummonKind( HWND hDlg );


	void AddMyOrder( cltSummonOrder *pclOrder );
	void DelMyOrder( SI32 orderIndex );

	void ShowResponseOrder( HWND hDlg );
	void ShowResponseExplain( HWND hDlg );

	void ShowMySummonInfo( HWND hDlg );
	void ShowMyOrderInfo( HWND hDlg );

	void ShowMySummonList(HWND hDlg);
	void ShowMyOrderList(HWND hDlg);

	void DrawSummonPortrait( HWND hDlg );

	void Action( bool bdaychangedswitch );

public:	
	BOOL						m_bMainExplain;

	cltSummonOrder				*pclSummonOrder;
	SI32						siAddStartIndex;
	SI32						siTotalOrderNumber;

	BOOL						bSelectedSwitch;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSummonOrder, MAX_SUMMONMARKET_ORDER_PER_PERSON>	clMyOrder;
#else
	cltSummonOrder				clMyOrder[ MAX_SUMMONMARKET_ORDER_PER_PERSON ];
#endif
	BOOL						bShowMyOrderSwitch;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSummonOrder, MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER>	clResponseOrder;
#else
	cltSummonOrder				clResponseOrder[ MAX_SUMMONMARKET_RESPONSE_ORDER_NUMBER ];
#endif

	SI32						siTotalResponseOrderNumber;
	BOOL						bShowResponseOrderSwitch;
	
	SI32						siSelectedIndex;
	SI32						siMySummonSelectedIndex;		// 내 소환수 리스트
	SI32						siMyOrderSelectedIndex;			// 내 주문 리스트 

	SI32						m_siKind;
	SI32						m_siSelectList;


	HBITMAP						m_hSummonPortraitBitmap;
	SI32						m_siPortraitSummonKind;

	SI32						m_siCurrentPage;				// [진성] 현재 페이지로 변경. => 2008-7-19//주문 리스트 요청시 시작 위치
	SI32						m_siTotalPageNum;				// [진성] 전체 페이지 수.	  => 2008-7-19

};

#endif
