#ifndef _NSUMMONMARKETDLG_H
#define _NSUMMONMARKETDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../../CommonLogic/SummonMarket/SummonMarket.h"

#define MAX_TRADE_MONEY 50000000000		// 거래 할수있는 최대 금액.

class NSummonMarketDlg : public NStrBaseDlg
{
public:
	NSummonMarketDlg();
	virtual ~NSummonMarketDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 소환수 구입
class NSummonMarketBuyDlg : public NStrTabDlg
{
public:
	enum 
	{
		IMAGESTATIC_PORTRAIT ,
		BUTTON_BUY	,
		BUTTON_FIRST	,
		BUTTON_NEXT	,
		BUTTON_SELECTORDER	,
		BUTTON_REFRESH,
		LISTBOX_SUMMON	,
		LISTBOX_EXPLAIN	,
		LISTBOX_ABILITY_INFO,
		COMBOBOX_KIND	,
		RADIOBUTTON_SUMMON,
		RADIOBUTTON_SUMMON_PET,
		RADIOBUTTON_SUMMON_BOSS,
		STATICTEXT_PAGE,
	};

	NSummonMarketBuyDlg();
	virtual ~NSummonMarketBuyDlg();

	void InitDlg();
	void Action();

	void AddSummonKind();	// combo setting
	void ShowResponseOrder();
	void ShowResponseExplain();
	void DrawSummonPortrait();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void Refresh(void);

};



// 사용자 메뉴 == 소환수 팔기
class NSummonMarketUserDlg : public NStrTabDlg
{
public:
	enum SUMMONDLG_VARIABLE
	{
		 SUMMONDLG_BAR_WIDTH		= 126
		,SUMMONDLG_PORTRAIT_START_X	= 32
		,SUMMONDLG_PORTRAIT_START_Y	= 57
		,SUMMONDLG_PORTRAIT_WIDTH	= 46
		,SUMMONDLG_MAX_PORTRAIT		= 2
	};
	
	enum 
	{
		BUTTON_SELL,
		BUTTON_CANCELORDER,
		//BUTTON_SELECTSELL,
		EDITBOX_REALFEE,
		EDITBOX_SELLPRICE,
		EDITBOX_SELLPRICECHAR,
		EDITBOX_PLAYERNAME,
		LISTBOX_MYSUMMON,
		LISTBOX_MYORDER,
		LISTBOX_DETAILINFO,
		STATICTEXT_FEERATE,
		CHECKBOX_SELECTSELL,

		LISTBOX_ABILITY_INFO,	//[진성] 소환수 거래소 리스트 박스 추가.   => 2008-7-11
		IMAGESTATIC_MYSUMMON_1,	//[진성] 소환수 초상화 이미지 컨트롤 추가. => 2008-7-11
		IMAGESTATIC_MYSUMMON_2,
		IMAGESTATIC_MYSUMMON_3,
		IMAGESTATIC_MYORDER_1,
		IMAGESTATIC_MYORDER_2,
		IMAGESTATIC_MYORDER_3,
		STATIC_SELECT_CHAR_NAME,//[진성] 지정거래 "캐릭터 명" 텍스트 컨트롤 추가. => 2008-7-11
		
	};

	NSummonMarketUserDlg();
	virtual ~NSummonMarketUserDlg();

	void InitDlg();
	void Action();

	void ShowMySummonList();
	void ShowMyOrderList();
	void ShowMySummonInfo();
	void ShowMyOrderInfo();

	BOOL ShowFee();
	BOOL ShowRealFee(GMONEY price);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void ClickSummonPotrait(SI32 _Index);

	void RequestMyOrder(void);

private:
	TSpr				m_sprSummonPortrait;
	GMONEY				m_Price;
};


// 장예원 행수
class NSummonMarketChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};
	NSummonMarketChiefDlg();
	virtual ~NSummonMarketChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif

