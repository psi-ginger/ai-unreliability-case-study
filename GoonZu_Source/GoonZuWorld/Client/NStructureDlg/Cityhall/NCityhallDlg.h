#ifndef _NCITYHALLDLG_H
#define _NCITYHALLDLG_H

#define MAX_TAX_NUM	11


#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/Cityhall/Cityhall.h"
#include "../../../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "../../../Server/Empire/EmpireManager.h"


class NStrInventory;
class CEmpireUnitInfo;
class cltGameMsgResponse_EmpireInfo;

class NCityhallDlg : public NStrBaseDlg
{
public:
	NCityhallDlg();
	virtual ~NCityhallDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 상단정보1
class NCityhallInfoDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_VILLAGE_SCORE,
		EDIT_RESIDENTNUM,
		EDIT_STRDURA,
		EDIT_CHIEFNAME,

		LIST_RANKLIST,

		BUTTON_BOARD,
		BUTTON_OPENMINIHOME,
	};

	NCityhallInfoDlg();
	virtual ~NCityhallInfoDlg();

	void InitDlg();
	
	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


// 관청정보2
class NCityhallInfo2Dlg : public NStrTabDlg
{
public:
	enum {
		EDIT_STRMONEY,
		EDIT_PROFIT,
		EDIT_CURRENT_PRICE,
		EDIT_TOTAL_STOCK,

		LIST_DIVIDEND,
		LIST_INOUTMONEY,

		BUTTON_ACCOUNTBOOK	// 회계장부
	};

	NCityhallInfo2Dlg();
	virtual ~NCityhallInfo2Dlg();

	void InitDlg();

	void Action();

	virtual void Show();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	BOOL					m_bDispDividend;

	cltDividendInfo			m_dividendInfo;
};

// 상단정보2
//class NCityhallInfo2Dlg : public NStrTabDlg
//{
//public:
//	enum {
//		EDIT_STRMONEY,
//		EDIT_PROFIT,
//		EDIT_CURRENT_PRICE,
//		EDIT_TOTAL_STOCK,
//
//		LIST_INOUTMONEY
//	};
//
//	NCityhallInfo2Dlg();
//	virtual ~NCityhallInfo2Dlg();
//
//	void InitDlg();
//
//	void Action();
//
//	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
//};

// 주주명부
class NCityhallStockHolderDlg : public NStrTabDlg
{
public:
	enum {
		LIST_STOCKHOLDER,
		EDIT_STOCKHOLDERNAME,
		BUTTON_OPENMINIHOME,

	};

	NCityhallStockHolderDlg();
	virtual ~NCityhallStockHolderDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


// 주민등록
class NCityhallResidentDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_CHIEFNAME,
		EDIT_BEGINCHIEF,
		EDIT_ENDCHIEF,

		BUTTON_RESIDENTAPPLY,
		BUTTON_CANCEL_RESIDENTAPPLY,

		BUTTON_RANKAPPLY,
		BUTTON_CANCEL_RANKAPPLY,

		COMBO_RANK
	};

	NCityhallResidentDlg();
	virtual ~NCityhallResidentDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 주주총회
class NCityhallGeneralMeetingDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_SUGGEST,
		BUTTON_ACCEPT,
//		BUTTON_AGREEMENT,
//		BUTTON_OPPOSITION,
		
		EDIT_EXPLAIN,
		EDIT_START_DATE,
		EDIT_END_DATE,
//		EDIT_AGREEMENT,
//		EDIT_OPPOSITION,

//		LIST_AGREEMENT,
//		LIST_OPPOSITION

		BUTTON_VOTE
	};

	NCityhallGeneralMeetingDlg();
	virtual ~NCityhallGeneralMeetingDlg();

	void InitDlg();

	void Action();

//	void InitVoteList();
	void ShowMeetingInfo();
//	void ShowVoterList();
	void ShowLeftTime();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	virtual void Show();

	//void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:
	bool	m_bleftTimeBreak;		//[진성] 남은 시간이 없다면 시간이 가지 않게 한다. => 2008-9-30

};

// 기부하기
class NCityhallDonateDlg : public NStrTabDlg
{
public:
	enum
	{
		BUTTON_DONATE = 0,
		BUTTON_PREV,
		BUTTON_NEXT,
		LIST_DONATEMEMBER,
		EDIT_DONATEMONEY,
		EDIT_PAGE,
		
	};

	SI32 m_siRequestPage;
	SI32 m_siMaxPage;

	NCityhallDonateDlg();
	virtual ~NCityhallDonateDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetDonateMemberList( DonateMemberList* pDonateMemberList, SI32 MaxPage );

};

// 유저 메뉴에서 제국
class NCityhallUserMenuEmpireDlg : public NStrTabDlg
{
public:
	enum
	{
		COMBOBOX_VILLAGE = 0,
		LIST_VILLAGE,
		LIST_GUILD,
	};

	SI32 m_siVillageUnique[MAX_EMPIRENUMBER];
	NCityhallUserMenuEmpireDlg();
	virtual ~NCityhallUserMenuEmpireDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void SetEmpireMemberList( cltGameMsgResponse_EmpireInfo* EmpireUnitInfo ); 
};

// 대행수 메뉴에서 제국
class NCityhallChiefMenuEmpireDlg : public NStrTabDlg
{
public:
	enum
	{
		BUTTON_TAX = 0,
		BUTTON_CREATEEMPIRE,
		BUTTON_INVITEEMPIRE,
		BUTTON_SECEDEEMPIRE,
		LIST_TAX,
		LIST_VILLAGE,
		LIST_GUILD,
		EDIT_TAX
	};

	NCityhallChiefMenuEmpireDlg();
	virtual ~NCityhallChiefMenuEmpireDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void SetEmpireMemberList( cltGameMsgResponse_EmpireInfo* EmpireUnitInfo ); 
};

// 수익배당내역
class NCityhallDividendListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_DIVIDEND,
	};

	NCityhallDividendListDlg();
	virtual ~NCityhallDividendListDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	BOOL					m_bDispDividend;

	cltDividendInfo			m_dividendInfo;

};


// 행수메뉴1
class NCityhallChiefDlg : public NStrTabDlg
{
public:
	enum {
		COMBO_STRUCT,
		EDIT_TAX,

        BUTTON_APPLY,//일정이상의 텍스트 입력방지 [2007.07.05성웅]

        BUTTON_SETTAX,
		BUTTON_BERANK,
		BUTTON_CONSTRUCT,		
		BUTTON_REG_RESIDENT,
		BUTTON_ADMIN_RESIDENT,
		BUTTON_HIRENPC,
		BUTTON_SETNOTICE,
		BUTTON_DECLAREWAR,
		BUTTON_MAKETREE,
		STATIC_HIRE_EVENTNPC,
		COMBO_EVENTNPC
	};

	NCityhallChiefDlg();
	virtual ~NCityhallChiefDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	bool NewYearEventTime(void);
};

#define LIMIT_STOCKMONEY	5000
// 배당
class NCityhallCapitalDlg : public NStrTabDlg
{
public:
	//enum {
	//	BUTTON_DIVIDEND,
	//	BUTTON_RESIDENTSHARE,
	//	BUTTON_TOCAPITALFUND,

	//	EDIT_DIVIDEND,
	//	EDIT_RESIDENTSHARE,
	//	EDIT_MONEY,

	//	COMBO_RESIDENTSHARE
	//};

	enum {
		BUTTON_REQUESTSTOCK,
		BUTTON_TOCAPITALFUND,

		EDIT_STOCKMONEY,
		EDIT_MONEY,

		STATIC_ADDCAPITAL_TITLE,
		STATIC_DESC1,
		STATIC_ADDCAPITAL,

		OUTLINE_APPCAPITAL
	};

	NCityhallCapitalDlg();
	virtual ~NCityhallCapitalDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};

// 성황신
class NCityhallGodDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_GOD_KIND,
		BUTTON_GOD_LIFEUP,
		BUTTON_GOD_LEVELUP,

		BUTTON_CASTLE_LIFEUP,
		BUTTON_CASTLE_LEVELUP,

		EDIT_GOD_LASTUPDATE,
		EDIT_GOD_KIND,
		EDIT_GOD_KINDEXP,
		EDIT_GOD_LIFE,
		EDIT_GOD_LEVEL,

		EDIT_CASTLE_LASTUPDATE,
		EDIT_CASTLE_KIND,
		EDIT_CASTLE_KINDEXP,
		EDIT_CASTLE_LIFE,
		EDIT_CASTLE_LEVEL,

		COMBO_GOD_KIND
	};

	NCityhallGodDlg();
	virtual ~NCityhallGodDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 이벤트
class NCityhallEventDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_STARTEVENT,
		BUTTON_PREV,
		BUTTON_NEXT,
		BUTTON_SETUP,

		EDIT_VILLAGENAME,
		EDIT_EXPLAIN,

		LIST_EVENT,

		IMAGE_MYVILLMARK,
		IMAGE_VILLAGEMARK
	};

	NCityhallEventDlg();
	virtual ~NCityhallEventDlg();

	void InitDlg();

	void Action();

	void ShowVillageMark();

	virtual void DrawDialogFirst( SI32 ScreenRX, SI32 SreenRY );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 마을 지도 변경 
class NCityhallChangeMapDlg : public NStrTabDlg
{
public:
	enum {

		IMAGESTATIC_MAP	,
		BUTTON_PREV	,
		BUTTON_SETUP	,
		BUTTON_NEXT	,
		EDITBOX_COST	,
		EDITBOX_VILLAGETYPE	,
		EDITBOX_NEEDLEVEL	,
	};

	NCityhallChangeMapDlg();
	virtual ~NCityhallChangeMapDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void ShowVillageMap();
};

// 마을 건물에 자본금 넣어주기 
class NCityhallSetBaseMoneyDlg : public NStrTabDlg
{
public:
	enum
	{
		LIST_STRUCTURE	, // 해당 마을에 건물 목록
		BUTTON_SETMONEY, 
		EDIT_SETMONEY
	};

	NCityhallSetBaseMoneyDlg();
	virtual ~NCityhallSetBaseMoneyDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	bool m_bUpdateSwitch;
	SI16 m_siSelectRankType;
};


#endif
