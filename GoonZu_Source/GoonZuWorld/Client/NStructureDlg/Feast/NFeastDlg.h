#ifndef _NFEASTDLG_H
#define _NFEASTDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/Feast/Feast.h"

class CComboBox;
class NStrInventory;

class NFeastDlg : public NStrBaseDlg
{
public:
	NFeastDlg();
	virtual ~NFeastDlg();

	void Create();

	void GreetingMsg();
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 사용자 메뉴 
class NFeastUserDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_GIVEITEM,

		EDIT_ITEMNAME,
		EDIT_STATUS,
		EDIT_NAME,
		EDIT_DATE,
		EDIT_REASON,

		LIST_PRESENT
	};

	NFeastUserDlg();
	virtual ~NFeastUserDlg();

	void InitDlg();

	void Action();

	void ShowFeastInfo();
	void ShowPresentInfo();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

//[진성] 파티홀 파티정보.
class NPartyHallInfoDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_ENTRANCE_APPLICATION,

		EDIT_ITEMNAME,
		COMBOBOX_PARTYHALL_LIST,
		EDIT_NAME,
		EDIT_DATE,
		EDIT_REASON,
		EDIT_ENTRANCE_TYPE,

		LIST_PRESENT
	};

	NPartyHallInfoDlg();
	virtual ~NPartyHallInfoDlg();

	void InitDlg();

	void Action();

	void ShowFeastInfo();
	void ShowPartPresentInfo( SI16 _ListIndex );
   	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void SetPartyInfo( void );
	void ClearControll( void );

public:
	SI16	m_siPartyIndex;

};

// 연회 공지 메뉴 
class NFeastChatDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_INPUT,
		EDIT_CHAT,
		EDIT_EFFECT,
		LIST_EFFECT,
		BUTTON_USEEFFECT,
	};

	NFeastChatDlg();
	virtual ~NFeastChatDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void ShowEffectInfo();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// [진성] 파티홀 선물설정.
class NPartyHallPresentDlg : public NStrTabDlg
{
public:
	enum 
	{
		LISTVIEW_INVENTORY,
		LISTVIEW_PRESENT_LIST,
		EDIT_ITEM_NAME,
		EDIT_ITEM_NUM,
		BUTTON_INPUT_PRESENT,
		EDIT_EXPLAIN,
	};

	NPartyHallPresentDlg();
	virtual ~NPartyHallPresentDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	BOOL ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface );
	
	// 에디트 박스에 입력가능한 수로 변환.
	void EditCountRange( CEdit* _pEdit, SI32 _MaxCount );

	// 아이템의 재료.
	void SetItemMaterialInfo(TCHAR* pStrname);						

	// 재조가능한 개수.
	void SetMakeItemNumEnable( void );

	void SetPresentItem( void );			// 선물 리스트뷰에 아이템 보이기. 주최자만 보인다. 

	void SetMyInventory( void );
public:
	NStrInventory*	m_pMyInventory;

	SI16			m_siPresentItemInvenPos;
	SI32			m_siCurrentSelectedItemUnique;	// 선택한 아이템 유니크.
};


// 연회 예약 메뉴 
class NFeastResvDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_RESV,

		EDIT_COST,
		EDIT_REASON,
		EDIT_EXPLAIN,

		COMBO_YEAR,
		COMBO_MONTH,
		COMBO_DAY,
		COMBO_HOUR,
		COMBO_MINUTE
	};

	NFeastResvDlg();
	virtual ~NFeastResvDlg();

	void InitDlg();

	void Action();

	void SetDateCombo();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

//[진성] 파티홀. 예약창.
class NPartyHallReservationDlg : public NStrTabDlg
{
	enum{ RADIOBUTTON_NUM = 4 };
public:
	enum 
	{
		BUTTON_RESV,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		
		EDIT_COST,
		EDIT_REASON,
		EDIT_DECORATION_COST,

		COMBO_YEAR,
		COMBO_MONTH,
		COMBO_DAY,
		COMBO_HOUR,
		COMBO_MINUTE,
		COMBO_DECORATION_LIST,

		LIST_RESERVATION,
		RADIOBUTTON_PUBLIC,
		RADIOBUTTON_PRIVATE,
		RADIOBUTTON_VILLAGE,
		RADIOBUTTON_GUILD,

		EDIT_PUBLIC,
		EDIT_PRIVATE,
		EDIT_VILLAGE,
		EDIT_GUILD,

		COMMBO_PARTYHALL,
	};

	NPartyHallReservationDlg();
	virtual ~NPartyHallReservationDlg();

	void InitDlg();

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY ); 

	void SetDateCombo();
	bool SetReservationTime(SI32& year, SI32& month, SI32& day, SI32& hour, SI32& minute);
	void SetDecorationList( void );
	void SetReservationList( void );	// 예약 리스트 세튕!
	void SetMapNum( SI32 _IndexChange );
	void SetRadioButton( void );
	void SetTimeData( SI32 _DayIndex );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
	
	BOOL IsReservation(void);
	void SendReservation(void);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void Action();
	void SetReservationSuccess(void);

public:
	SI32			m_siMapIndex;
	SI32			m_siEntranceType;

	TSpr			m_Spr;
};


// 행수 메뉴 
class NFeastChiefDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_ADDPRESENT,
		BUTTON_DELPRESENT,
		BUTTON_SETCOST,

		EDIT_ITEMNUM,
		EDIT_COST,

		LIST_PRESENT,

		COMBO_ITEMTYPE,
		COMBO_ITEM
	};
	NFeastChiefDlg();
	virtual ~NFeastChiefDlg();

	void InitDlg();

	void Action();

	BOOL ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface );
	void ShowPresentList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


// 행수 메뉴 2
class NFeastChief2Dlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_INPUT,
		BUTTON_OUTPUT,

		EDIT_INVEN_NAME,
		EDIT_INVEN_NUM,

		EDIT_STG_NAME,
		EDIT_STG_NUM,

		LISTVIEW_MYINVENTORY,
		LISTVIEW_FEASTSTG
	};
	NFeastChief2Dlg();
	virtual ~NFeastChief2Dlg();

	void InitDlg();

	void Action();

	void LoadFeastStg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	NStrInventory*	m_pMyInventory;

	SI32			m_siSelectedStgIndex;



};

//[추가: 황진성 2008. 3. 10 => 파티홀. 관리자 메뉴, 장식 제조.]
// 행수 메뉴 
class NPartyHallChiefDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_ADD_DECORATIN,
		LIST_MATERIAL,			// 재료 리스트.
		LIST_DECORATION,		// 장식품 리스트.
		COMBO_ITEMTYPE,
	};
	NPartyHallChiefDlg();
	virtual ~NPartyHallChiefDlg();

	void InitDlg();

	void Action();

	bool MakeDecoration( void );
	SI32 PartyhallItemCount(SI32 _ItemUnique);
	BOOL ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface );
	void ShowDecorationList( void );
	void SetItemMaterialInfo(TCHAR* pItemName);
	void SetChangeItemMaterialInfo(cltFeastStg* pStgItem);
	
	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	SI32		m_siCurrentSelectedItemUnique;
	SI32		m_DecorationKind[MAX_PARTYHALL_DECORATION_ITEM];
	cltFeastStg m_MaterialItem[MAX_FEAST_STG_NUMBER];					// 재료가 되는 아이템을 넣는다.

};
//[추가: 황진성 2008. 3. 10 => 파티홀. 관리자 메뉴, 파티홀 관리.]
// 행수 메뉴 2
class NPartyHallChief2Dlg : public NStrTabDlg
{
public:
	enum 
	{
		BUTTON_INPUT,
		BUTTON_OUTPUT,

		EDIT_INVEN_NAME,
		EDIT_INVEN_NUM,

		EDIT_STG_NAME,
		EDIT_STG_NUM,

		LISTVIEW_MYINVENTORY,
		LISTVIEW_FEASTSTG,
	
		EDIT_COST,
		BUTTON_SETCOST,
		
	};
	NPartyHallChief2Dlg();
	virtual ~NPartyHallChief2Dlg();

	void InitDlg();

	void Action();

	void LoadFeastStg();

	void EditCountRange( CEdit* _pEdit, GMONEY _MaxCount );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	NStrInventory*	m_pMyInventory;

	SI32			m_siSelectedStgIndex;



};


#endif