#pragma once

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"
#include "Item/ItemCommon/ItemUnit.h"
#include "../NPrivateTradeDlg/PrivateTradeBaseInfo.h"

class CEachPersonTradeOrderInfo;
class CLibListMgr;
class CList;
class CListView;
class CStatic;
class CButton;
class CEdit;

class cltItem;
class cltRentContract;
class cltHorse;
class cltPItemInfo;


class CNPrivateRepairDlg :public CDialogController
{
public:
	CNPrivateRepairDlg();
	~CNPrivateRepairDlg();


	static void CALLBACK StaticNPrivateRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPrivateRepairDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Show( SI32 siOtherCharUnique, SI16 TradeMode, SI16 Level, TCHAR *Name, TCHAR *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel );
	void Create();
	void Action();



	bool IsShow();
	
	void ClearPrivateTradeList();		
	void LoadInventoryData();
	bool IsInventoryChanged();

	void PrivateTradeOtherListReload();
	void PrivateTradeMyListReload();
	void PrivateTradeOtherListInfo( SI32 Index );
	void PrivateTradeMyListInfo( SI32 Index );
	
	void PrivateTradeAddItem( SI32 siCharUnique, SI32 siAddedIndex, cltItem *pclItem );
	void PrivateTradeDelList( SI32 CharUnique, SI16 DelType, SI16 Param );
	void PrivateTradeDelItem( SI32 siCharUnique, SI16 siDelIndex );
	void PrivateTradeAddPrice( SI32 siCharUnique, GMONEY siPrice );	
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
	void PushMyAccept();
	void PushMyCancel();
	bool Pop( BYTE *pData );
	
	
	
	CEdit            *m_pNTRepairEdit1;

private:

	CStatic          *m_pNTRepairStatic1;
	CStatic          *m_pNTRepairStatic2;
	CStatic          *m_pNTRepairStatic3;
	CList            *m_pNTRepairList1;
	CList            *m_pNTRepairList2;
	
	CEdit            *m_pNTRepairEdit2;
	CEdit            *m_pNTRepairEdit3;
	CEdit            *m_pNTRepairEdit4;
	CListView        *m_pNTRepairListView;
	CButton          *m_pNTRepairButton1;  //귓속말 보내기 
	//CButton          *m_pNTRepairButton2;  //상대방 꼼지 가기 
	CButton          *m_pNTRepairButton3;  //선택 물품 올리기 
	CButton          *m_pNTRepairButton4;  //선택 물품 내리기 
	CButton          *m_pNTRepairButton5;   //입력 버튼 
	CButton          *m_pNTRepairButton6;   // 입력 help 버튼
	CButton          *m_pNTRepairButton7;   //수락하기
	CButton          *m_pNTRepairButton8;  // 수락하기 help 버튼 


    



	SI16		        m_siSelectedModeIndex;	// 거래냐 수리이냐 를 구분하기 위한것
	UI32	 	        m_uiLastAcceptFrame;	// 거래에 주어지는 간격
	SI32                m_siOtherCharLevel;
	GMONEY				m_siPrice;				// 거래에 사용될 최종 가격
	SI32                m_siInvSelectedIndex;
	
	
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
	cltItem				m_clItem[ MAX_ITEM_PER_PERSON ];
	cltItem             m_clUserItem[MAX_ITEM_PER_PERSON];
	

	TCHAR  				          m_strOtherCharName[ MAX_PLAYER_NAME ];	  
	CEachPersonTradeOrderInfo    *m_pEachPersonTradeOrderInfo;	// 거래 상대방 캐릭터명	
	CLibListMgr                  *m_pPrivateTradeBufferMgr;	

};

