#ifndef _POSTOFFICE_H
#define _POSTOFFICE_H

#include <Directives.h>

#include "..\StructureClass\StructureClass.h"

//#include <Stdio.h>

//#include "..\..\Common\Item\ItemCommon\cltitem.h"

#define	MAX_POSTOFFICE_TAB_NUMBER	6

class cltTaxPaymentListDlg;

#define MAX_SAVEUSERITEM_LIST				60
#define MAX_SAVEUSERHORSE_LIST				20
#define MAX_POSTOFFICE_RECV_NUMBER			30
#define MAX_POSTOFFICE_MSG_SIZE				128
#define MAX_POSTOFFICE_RECORD_NUMBER		100

#define LPARAMHIWORD_ITEM	1
#define LPARAMHIWORD_MSG	2
#define LPARAMHIWORD_MONEY	3

#define POSTOFFICE_MSGTYPE_ITEM			1
#define POSTOFFICE_MSGTYPE_MSG			2
#define POSTOFFICE_MSGTYPE_MONEY		3

// 운영자한테 보내질 유저의 아이템 정보
class cltSaveUersItemInfo
{
public:
	
	SI32				siIndex;			// DB에 저장되어 잇는 인덱스	
	SI64				siServiceCharge;	// 아이템 갯수만큼의 수수료
	cltItem				clItem;				// item
	cltDate				clDate;				// 송신일

	cltSaveUersItemInfo()
	{
		ZeroMemory( this, sizeof(cltSaveUersItemInfo) );
	}
};			

//KHY - 1127 - 클라라 물품 보관 개선
// 운영자한테 보내질 유저의 기승동물 정보
class cltSaveUersHorseInfo
{
public:
	
	SI32				siIndex;			// DB에 저장되어 잇는 인덱스	
	SI64				siServiceCharge;	// 아이템 갯수만큼의 수수료
	cltDate				clDate;				// 송신일

	cltHorse			clHorse;  //저장된 말정보를 가져온다.

	cltSaveUersHorseInfo()
	{
		ZeroMemory( this, sizeof(cltSaveUersHorseInfo) );
	}
};			

class cltMarketConditionsInfo
{
public:

	SI16 		siUnique; 							// 아이템 유니크.
	GMONEY		siMarketConditionsMoney;			// 시세.
	GMONEY		siDifferenceMoney;					 // 차액.

	cltMarketConditionsInfo()
	{
		ZeroMemory( this, sizeof(cltMarketConditionsInfo) );
	}
};			

// 수신아이템정보
class cltPostOfficeRecvItemInfo
{
public:
	SI32				siMsgID;

	SI32				siPostType;												// 0:일반유저, 1:운영자
	TCHAR				szSenderPersonName[ MAX_PLAYER_NAME ];					// 송신자 name
	cltItem				clItem;													// item

	cltDate				clDate;													// 송신일

	cltPostOfficeRecvItemInfo()
	{
		ZeroMemory( this, sizeof(cltPostOfficeRecvItemInfo) );
	}
};

// 수신현금정보
class cltPostOfficeRecvMoneyInfo
{
public:
	SI32				siMsgID;

	SI32				siPostType;												// 0:일반유저, 1:운영자
	TCHAR				szSenderPersonName[ MAX_PLAYER_NAME ];					// 송신자 name
	GMONEY				siMoney;												// 금액

	cltDate				clDate;													// 송신일

	cltPostOfficeRecvMoneyInfo()
	{
		ZeroMemory( this, sizeof(cltPostOfficeRecvMoneyInfo) );
	}
};

// 수신메시지정보
class cltPostOfficeRecvMsgInfo
{
public:
	SI32				siMsgID;

	SI32				siPostType;												// 0:일반유저, 1:운영자
	TCHAR				szSenderPersonName[ MAX_PLAYER_NAME ];					// 송신자 name
	TCHAR				szMsg[ MAX_POSTOFFICE_MSG_SIZE ];						// 메시지 

	cltDate				clDate;													// 송신일

	cltPostOfficeRecvMsgInfo()
	{
		ZeroMemory( this, sizeof(cltPostOfficeRecvMsgInfo) );
	}
};

class cltPostOfficeRecvDataInfo
{
public:
	cltDate		clDate;
	TCHAR		szPersonName[ MAX_PLAYER_NAME ];
	TCHAR		szMsg[ 210 ];
	LONG		lParam;

	cltPostOfficeRecvDataInfo()
	{
		ZeroMemory( this, sizeof( cltPostOfficeRecvDataInfo ) );
	}
};



// 송수신내역
class cltPostOfficeRecordInfo
{
public:
	SI32				siMsgID;
	SI16				siMsgType;												// 송수신내역타입
	SI32				siPostType;												// 0:일반유저, 1:운영자
	SI16				siSendRecv;												// 1:송신 2:수신
	TCHAR				szPersonName[ MAX_PLAYER_NAME ];						// 송수신자 name
	SI16				siItemUnique;											// 아이템 Unique
	SI16				siItemNum;												// 아이템 Num
//	TCHAR				szMsg[ MAX_POSTOFFICE_MSG_SIZE ];						// 메시지
	GMONEY				siMoney;												// 금액

	cltDate				clDate;													// 일자

	cltPostOfficeRecordInfo()
	{
		ZeroMemory( this, sizeof( cltPostOfficeRecordInfo ) );
	}

};


//-------------------------------------------------
// 개별적인 우체국
//-------------------------------------------------
class cltPostOffice : public cltStructureBasic{
private:

public:
	cltPostOffice(SI32 sivillageunique);
	~cltPostOffice();

};

//-----------------------------------------------
// 우체국 관리자. 
//-----------------------------------------------
class cltPostOfficeManager : public cltStructureMngBasic{
public:

	cltPostOfficeManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltPostOfficeManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 id);

	// 우체국을 신설한다.
	BOOL CreatePostOffice(SI32 siunique);

	void LoadMyInventory( HWND hDlg );
	BOOL IsMyInventoryChanged();

	void AddItemToRecvList( cltPostOfficeRecvItemInfo *pItemInfo );
	void AddMsgToRecvList( cltPostOfficeRecvMsgInfo *pMsgInfo );
	void AddMoneyToRecvList( cltPostOfficeRecvMoneyInfo *pMoneyInfo );
	
	void DispMsgInfo( HWND hDlg );

	void ListRecvItem_DeleteAll();												// 수신함 리스트를 초기화한다
	void ListRecvItem_DelItem( LPARAM lParam );									// 수신함에서 하나의 아이템을 지운다

	cltItem* GetItemFromList( SI32 siMsgID );									// 수신함에서 아이템(물품)정보를 얻어온다

	void ShowPostOfficeInfo();

public:	
	cltItem						*m_pclMyItem;				// 내 인벤토리 체크

	HWND						m_hUserInventory;

	HIMAGELIST					m_hImageList;
	
	SI32						m_siInventorySelectedIndex;
	HWND						m_hwndRecvList;
	
	SI32						m_siCurMsgType;
	SI32						m_siCurMsgID;

	BOOL						m_bAddItemToRecvList;
	BOOL						m_bAddMsgToRecvList;
	BOOL						m_bAddMoneyToRecvList;
	BOOL						m_bRefreshList;
	
	BOOL						m_bMainExplain;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltPostOfficeRecvDataInfo, MAX_POSTOFFICE_RECV_NUMBER>		m_recvDataInfo;
	NSafeTArray<cltPostOfficeRecvItemInfo, MAX_POSTOFFICE_RECV_NUMBER>		m_recvItemInfo;
	NSafeTArray<cltPostOfficeRecordInfo, MAX_POSTOFFICE_RECORD_NUMBER>		m_recordinfo;
#else
	cltPostOfficeRecvDataInfo	m_recvDataInfo[ MAX_POSTOFFICE_RECV_NUMBER ]; 
	cltPostOfficeRecvItemInfo	m_recvItemInfo[ MAX_POSTOFFICE_RECV_NUMBER ];
	cltPostOfficeRecordInfo		m_recordinfo[ MAX_POSTOFFICE_RECORD_NUMBER ];
#endif
	SI32						m_recordcount;

	BOOL						bUpdateRecord;
	void						ShowRecordList( HWND hDlg );


};

#endif
