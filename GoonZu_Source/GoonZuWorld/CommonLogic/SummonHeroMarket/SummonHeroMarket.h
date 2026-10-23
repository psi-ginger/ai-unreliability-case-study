#ifndef _SUMMONHEROMARKET_H
#define _SUMMONHEROMARKET_H

#include <Directives.h>

#include "..\StructureClass\StructureClass.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

#define MAX_SUMMONHEROMARKET_ORDER_NUMBER			10000
#define MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER	60
#define MAX_SUMMONHEROMARKET_ORDER_PER_PERSON		3

#define	MAX_SUMMONHEROMARKET_TAB_NUMBER				4

class cltTaxPaymentListDlg;

#include "../SummonMarket/SummonMarket.h"
#include "../SummonHeroMarket/SummonHeroMarket.h"

//-------------------------------------------------
// 개별적인 소환수거래소
//-------------------------------------------------
class cltSummonHeroMarket : public cltStructureBasic{
private:

public:
	cltSummonHeroMarket(SI32 sivillageunique);
	~cltSummonHeroMarket();

};


//-----------------------------------------------
// 소환수거래소 관리자. 
//-----------------------------------------------
class cltSummonHeroMarketManager : public cltStructureMngBasic{
public:

	cltSummonHeroMarketManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltSummonHeroMarketManager();

	void GetTitleText(SI32 villageunique);

	// 최초 환영 인사소환수표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 소환수거래소를 신설한다.
	BOOL CreateSummonHeroMarket(SI32 siunique);

	void DispMsgInfo( HWND hDlg );
	void ShowSummonHeroMarketInfo();

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
	NSafeTArray<cltSummonOrder, MAX_SUMMONHEROMARKET_ORDER_PER_PERSON>		clMyOrder;
#else
	cltSummonOrder				clMyOrder[ MAX_SUMMONHEROMARKET_ORDER_PER_PERSON ];
#endif
	BOOL						bShowMyOrderSwitch;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSummonOrder, MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER>		clResponseOrder;
#else
	cltSummonOrder				clResponseOrder[ MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER ];
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

	SI32						m_siStartPos;					// 주문 리스트 요청시 시작 위치

};

#endif
