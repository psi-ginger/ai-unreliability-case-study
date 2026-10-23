#pragma once


#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"
#include "Stock/Stock.h"
#include "Char/CharCommon/Char-EconomyInfo.h"


class CEachPersonTradeOrderInfo;
class CLibListMgr;
class CList;
class CStatic;
class CButton;
class COutLine;
class CEdit;

class cltItem;
class cltRentContract;
class cltHorse;

class CNTradeChild1;
class CNTradeChild2;
class CNTradeChild3;
class CNTradeChild4;

class CNPrivateTradeDlg :public CDialogController
{
public:
	CNPrivateTradeDlg();
	~CNPrivateTradeDlg();


	static void CALLBACK StaticNPrivateTradeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPrivateTradeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Show( SI32 siOtherCharUnique, SI16 TradeMode, SI16 Level, TCHAR *Name, TCHAR *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel );
	void Create();
	void Action();

	CNTradeChild1 *NTradetabDlg1;
	CNTradeChild2 *NTradetabDlg2;
	CNTradeChild3 *NTradetabDlg3;
	CNTradeChild4 *NTradetabDlg4;




	bool IsShow();	
	//void Hide();
	void ClearPrivateTradeList();
	
	//void LoadStockList();
	bool IsStockListChanged();

	void PrivateTradeOtherListReload();
	void PrivateTradeMyListReload();

	void PrivateTradeOtherListInfo( SI32 Index );
	void PrivateTradeMyListInfo( SI32 Index );
	
	void PrivateTradeAddItem( SI32 siCharUnique, SI32 siAddedIndex, cltItem *pclItem );
	void PrivateTradeDelList( SI32 CharUnique, SI16 DelType, SI16 Param );
	void PrivateTradeDelItem( SI32 siCharUnique, SI16 siDelIndex );
	void PrivateTradeAddPrice( SI32 siCharUnique, GMONEY siPrice );
	void PrivateTradeAddStock( SI32 siCharUnique, SI32 siStockVillageUnique, SI32 siStockNum );
	void PrivateTradeAddHouseUnit( SI32 siCharUnique, SI32 siHouseVillageUnique, SI32 HouseUnitSlot, cltRentContract *pRentContract );
	void PrivateTradeAddHorse( SI32 siCharUnique, SI16 HorseIndex, cltHorse *pclHorse );
	void PrivateTradeMyAccept();
	void PrivateTradeMyCancel();
	void PrivateTradeOtherAccept( SI16 siResult );
	void PrivateTradeOtherCancel();
	void PrivateTradeEnd( SI32 siResult, 
		SI32 RecvStockVillageUnique, SI32 RecvStockNum, 
		SI32 SendStockVillageUnique, SI32 SendStockNum,
		SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot,
		cltRentContract *pclRecvRentContract,
		SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot, 
		SI16 HorseIndex, cltHorse *pclHorse );
	
	bool GetDelType( SI32 SelectedListIndex, SI16 *pType, SI16 *pParam );

	void PushCancel();
	void PushAddItem();
	void PushDelList();
	void PushDelItem();
	void PushPrice();
	void PushStock();
	void PushHouseUnit();
	void PushHorse();	
	void PushMyAccept();
	void PushMyCancel();	

	bool Pop( BYTE *pData );
	
		

	CEdit               *m_pNPTradeEdit;	
	CButton             *m_pNPTradeButton9;   //수락하기
	SI16		        m_siSelectedModeIndex;	// 거래냐 수리이냐 를 구분하기 위한것
	//UI32	 	        m_uiLastAcceptFrame;	// 거래에 주어지는 간격->기존에는 프레임을 저장하였는데 커런트 타임을 저장 하는 것으로 변경 하였습니다. 2009-03-09 m_uiLastAcceptFrame->m_uiLastAcceptTime
	SI32	 	        m_siLastAcceptTime;	// 거래에 주어지는 간격->기존에는 프레임을 저장하였는데 커런트 타임을 저장 하는 것으로 변경 하였습니다. 2009-03-09

	SI32				m_siStockKindList[MAX_STOCK_KIND_IN_ACOUNT];	// 주식 리스트 ( 종목 )
	SI32				m_siStockNumList[MAX_STOCK_KIND_IN_ACOUNT];		// 주식 리스트 ( 개수 )
	SI32				m_siStockVillageUnique;	// 거래에 사용될 주식 종목
	SI32				m_siStockNum;			// 거래에 사용될 주식수
	SI32				m_siHouseVillageUniqueList[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];	// 시전 소속 마을
	SI32				m_siHouseUnitSlotList[ MAX_REAL_ESTATE_NUMBER_PER_PERSON ];			// 시전 번호

	

	SI32                m_siOtherCharLevel;
	GMONEY				m_siPrice;				// 거래에 사용될 최종 가격
	
	

private:


	COutLine           *m_pNPTradeOutline1; 
	COutLine           *m_pNPTradeOutline2; 
	COutLine           *m_pNPTradeOutline3; 

	CList              *m_pNPTradeList4; 
	CList              *m_pNPTradeList6; 


    CStatic            *m_pNPTradeStatic1;
	
	
	CStatic      *m_pNPTradeStatic3;	
	CEdit      *m_pNPTradeEdit5;	
	CEdit      *m_pNPTradeEdit7;
	CStatic      *m_pNPTradeStatic8;
	CStatic      *m_pNPTradeStatic9;

	CButton      *m_pNPTradeButton1;  // 귓속말 보내기 옆의 ?
	CButton      *m_pNPTradeButton2;  //귓속말 보내기
	CButton      *m_pNPTradeButton3;  //꼼지가기
	CButton      *m_pNPTradeButton4;  //돈, 믈픔
	CButton      *m_pNPTradeButton5;  //주식
	CButton      *m_pNPTradeButton6;   //말
	CButton      *m_pNPTradeButton7;   //부동산
	CButton      *m_pNPTradeButton8;   //선택 지우기
	
	CButton      *m_pNPTradeButton10;  //수락하기 옆의 물음표
	CButton      *m_pNPTradeButton11;  //보낼물품옆의 ?

	
	SI16 m_siBitmapAnimationIndex;

	
	bool m_bShow;
	bool m_bMyAccept;

	SI32 m_siAccpetCounter;

	SI32 m_siOtherCharUnique;


	SI16 m_siPrivateTrdeOrderMgrIndex;
	SI16 m_siPrivateTrdeOrderCountIndex;

   
	SI32				m_siMyPrivateTradeListSelectedIndex;	// 내가 올려놓은 리스트 중에서 아이템 선택
	SI32				m_siOtherPrivateTradeListSelectedIndex;	// 상대방이 올려놓은 리스트 중에서 아이템 선택

	cltPItemInfo		*m_pPersonItemInfo;	
	//HBITMAP				m_hItemImage;
	//SI32				m_si32ImageListIndex;
	//bool				m_bIsListRotate;
		


	cltItem				m_clItem[ MAX_ITEM_PER_PERSON ];
	
	

	

	TCHAR				m_strOtherCharName[ MAX_PLAYER_NAME ];	
   CEachPersonTradeOrderInfo *m_pEachPersonTradeOrderInfo;	// 거래 상대방 캐릭터명
   //SI32				m_siStockKindList[MAX_STOCK_KIND_IN_ACOUNT];	// 주식 리스트 ( 종목 )
	//SI32				m_siStockNumList[MAX_STOCK_KIND_IN_ACOUNT];	

	
	CLibListMgr         *m_pPrivateTradeBufferMgr;
	

};

