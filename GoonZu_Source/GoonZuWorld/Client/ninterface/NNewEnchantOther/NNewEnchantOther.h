#pragma once
// NNewEnchantOther.h: interface for the NNewEnchantOther class.
//
//////////////////////////////////////////////////////////////////////
/* ==========================================================================
클래스 :		CNNewEnchantOther

작성일 :		06/02/17
작성자 :		김태양

목  적 :		


용  법 :		
변경사항 :		없음

차후작업 :		

========================================================================*/
#include "Resource.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Inventory/ItemInventoryBaseDefinition.h"
#include "../NPrivateTradeDlg/PrivateTradeBaseInfo.h"

class CButton;
class CListView;
class CEachPersonTradeOrderInfo;

class CNNewEnchantOther  : public CDialogController
{
public:
	CNNewEnchantOther();
	~CNNewEnchantOther();

	static void CALLBACK	StatiCNNewEnchantOtherProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NNewEnchantOtherProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create();	
	void Show( SI32 siOtherCharUnique,SI16 simode, SI16 Level, char *Name, char *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel );
	void Hide();
	void EnableButton(bool bWater, bool bFire, bool bWind, bool bGround);

	void ShowOrgEnchantItem(cltItem cltOrgItem);
	void ShowMatEnchantItem(cltItem cltMatItem);
	void ShowEleEnchantItem(cltItem cltEleItem[]);	
	bool IsShow();	
	void PushCancel();
	void PrivateTradeEnd( SI32 siResult, 
		SI32 RecvStockVillageUnique, SI32 RecvStockNum, 
		SI32 SendStockVillageUnique, SI32 SendStockNum, 
		SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot, 
		cltRentContract *pclRecvRentContract,
		SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot, 
		SI16 HorseIndex, cltHorse *pclHorse );	
	bool Pop( BYTE *pData );
	void SetUpdateButton(bool updatebutton);
	void SetStartButton(bool Startbutton);
	bool GetStartButton();
	bool GetUpdateButton();
	bool IsCheckSkill(cltItem clOutputItem);
	SI32 GetMatOtherCharUnique();
	void SetPriceText(GMONEY sifee);



private:
	//cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
	//SI32					m_siUserItemPrice[ MAX_ITEM_PER_PERSON ];
	//cltPItemInfo			*m_pPersonItemInfo;

	InterfaceFactory m_InterfaceFactory;
	cltItem m_clOrgEnchantItem;		//인챈트 시킬 아이템
	cltItem m_clMatEnchantItem;		//인챈트로 인해 소진될 아이템
	cltItem m_clEleEnchantItem[3];	//인챈트 속성돌아이템
	SI32 m_siEnchantableItemList;
	SI32 m_siEnchantableItemCompare;


	SI32			m_siAccpetCounter;
	SI32			m_siOtherCharUnique;
	SI16			m_siPrivateTrdeOrderMgrIndex;
	SI16			m_siPrivateTrdeOrderCountIndex;
	char  			m_strOtherCharName[ MAX_PLAYER_NAME ];	  
	SI32            m_siOtherCharLevel;

	CLibListMgr                  *m_pPrivateTradeBufferMgr;	
	CEachPersonTradeOrderInfo    *m_pEachPersonTradeOrderInfo;	// 거래 상대방 캐릭터명	



	bool m_bShow;
	bool m_bMyAccept;

};
