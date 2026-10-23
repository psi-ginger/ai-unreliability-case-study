#ifndef _NPOSTOFFICEDLG_H
#define _NPOSTOFFICEDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/PostOffice/PostOffice.h"

class NStrInventory;

class NPostOfficeDlg : public NStrBaseDlg
{
public:
	NPostOfficeDlg();
	virtual ~NPostOfficeDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 물품 보내기
class NPostOfficeSendItemDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_PLAYERNAME,
		EDIT_ITEMNAME,
		EDIT_AMOUNT,
		EDIT_RECEIVERNAME,
		EDIT_MSG,

		LISTVIEW_MYINVENTORY,

		BUTTON_SENDITEM,
		BUTTON_SENDMSG

	};

	NPostOfficeSendItemDlg();
	virtual ~NPostOfficeSendItemDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void WriteName(TCHAR* pName);

public:
	NStrInventory*	m_pMyInventory;

};



// 현금 보내기
class NPostOfficeSendMoneyDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_PLAYERNAME,
		
		EDIT_CURRENTMONEY,
		EDIT_MONEY,
		EDIT_MONEY_UNIT,
		EDIT_FEE,
		EDIT_FEERATE,
		EDIT_MSG,
		EDIT_RECEIVERNAME,

		BUTTON_SENDMONEY,
		BUTTON_SENDMSG		
	};

	NPostOfficeSendMoneyDlg();
	virtual ~NPostOfficeSendMoneyDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};


// 수신함	
class NPostOfficeRecvDlg : public NStrTabDlg
{
public:
	enum {
		LIST_RECVLIST,
		EDIT_MSG,

		BUTTON_RECV,
		BUTTON_RECVOK,
		BUTTON_SENDOK,
		BUTTON_REPLY//답장하기 [2007.08.17 손성웅]
	};

	NPostOfficeRecvDlg();
	virtual ~NPostOfficeRecvDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

// 송수신내역열람
class NPostOfficeRecordDlg : public NStrTabDlg
{
public:
	enum {
		LIST_RECORDLIST,
		EDIT_MSG,

		BUTTON_RECORD

	};
	NPostOfficeRecordDlg();
	virtual ~NPostOfficeRecordDlg();

	void InitDlg();

	void Action();

	void ShowRecordList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};


// 역참 행수 메뉴
class NPostOfficeChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE_MONEY,
		EDIT_FEERATE_ITEM,
		EDIT_FEE_MSG,

		BUTTON_SETUP
	};
	NPostOfficeChiefDlg();
	virtual ~NPostOfficeChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};



//[진성] 단체 메시지 보내기. 물품 보내기 => 2008-8-4
class NNewPostOfficeSendItemDlg : public NStrTabDlg
{
public:
	enum 
	{
		STATIC_PLAYERNAME,
		EDIT_PLAYERNAME,
		EDIT_ITEMNAME,
		EDIT_AMOUNT,
		EDIT_RECEIVERNAME,
		EDIT_MSG,

		LISTVIEW_MYINVENTORY,

		BUTTON_SENDITEM,
		BUTTON_SENDMSG
	};

	NNewPostOfficeSendItemDlg();
	virtual ~NNewPostOfficeSendItemDlg();

	void InitDlg();

	void Action();
	void Destroy();
	void Show();
	void Hide();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void WriteName(TCHAR* pName);

public:
	NStrInventory*	m_pMyInventory;

};

#endif