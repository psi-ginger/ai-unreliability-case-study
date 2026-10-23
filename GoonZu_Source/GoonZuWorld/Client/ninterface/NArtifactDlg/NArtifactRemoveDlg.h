//------------------------------------------------------------------------------
//
//	아티팩트 창.
//
//	2008. 2. 26
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _NARTIFACT_REMOVE_DLG_
#define _NARTIFACT_REMOVE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

class NArtifactRemoveDlg : public CDialogController
{
	enum
	{
		GAGE_NUM	= 152,
		EFFECT_NUM	= 12

	};
	enum
	{
		IMAGE_GAGE_BACK,			// 게이지 배경.
		IMAGE_GAGE,					// 게이지.
		IMAGE_EFFECT,				// 이펙트 효과.
		IMAGE_SLOT_REMOVE,			// 제거용 이미지.
		IMAGE_ARROW_IN_OUT,

		IMAGE_NUM,					// 이미지 개수.   
	};

	enum{ SLOT_COUNT = 6, };
	enum{ NO_SLOT = -1, EQUIP_SLOT, CRYSTAL_SLOT1, CRYSTAL_SLOT2, CRYSTAL_SLOT3, CRYSTAL_SLOT4, CRYSTAL_SLOT5,TOTAL_SLOT_COUNT }; 

public:
	NArtifactRemoveDlg();
	~NArtifactRemoveDlg();
	void Clear( void );

	void Create( CControllerMgr *pParent );
	void Action(void);

	static void CALLBACK StaticNArtifactRemoveDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NArtifactRemoveDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool InsertListViewItem( CListView* _pListView, cltItem* _cltItem );		// 슬롯에 아이템 넣기.
	void DelListViewItem( CListView* _pListView );								// 해당 슬롯에 아이템 비우기.
	void DelAllListViewItem( void );											// 모든 슬롯을 비우기.
	bool CanInsert( void );														// 아이템 배열 슬롯의 범위 확인.
	bool CanDel( void );														// 아이템 삭제가 가능한지 확인.
	SI32 RemoveItemPos( void );													// 결정 제거에 사용할 아이템이 인벤에 있는지 확인하고 위치 리턴.
	void ClickRemoveSlot( SI32 _SlotIndex );									// 슬롯의 제거 여부. 
	void InsertEquipSetCrystal( void );
	bool CanMixure( SI32 _ItemUnique );											// 조합이 가능 확인.
	bool InsertCrystal( CListView* _pListView, cltItem* _cltItem );
	bool CanInsertCrystal( cltItem* _cltItem );
	bool IsCristal( SI32 _siItemUnique );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	bool SameCrystalInvenItem( SI32 _InvenPos);
	void NotifyMsg( SI32 _MsgType );
	void RemoveSuccess(cltItem* pEquip);
	void EquipExplain( cltItem* pEquip );												// 장비 아이템의 정보 설명.
	void CrystalExplain( CEdit* _pExplainStatic, cltItem* pEquipItem, SI16 _siSlot);	// 아이템에 박힌 슬롯의 정보를 보여준다.

	void SendServerMsg( SI32 _MsgType );												// 서버에 메시지를 보냄.
	void DeleteBuffer( SI32 _ClassType, void* _ClassData );
	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	bool IsRemove(void);

public:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_InventoryPos[SLOT_COUNT];
	cltItem				m_clCrystalItem[ SLOT_COUNT ];									// 결정체를 제거할 때 사용할 아이템을 인벤에서 찾아 넣는다. 

	cltItem				m_clArtifactItem[ SLOT_COUNT ];	

	SI08				m_CurrentSlot;

	bool				m_bRemoveSlotIndex[ SLOT_COUNT ];						// 결정체를 제거할 때 사용할 아이템을 인벤에서의 위치. 	
	
	TSpr*				m_Spr[IMAGE_NUM];

	cltItem				m_clRemoveSuccessItem;

	SI16				m_siGaugeCount;
	SI16				m_siEffectCount;
	bool				m_bGaugeAction;
	bool				m_bEffectAction;
};

#endif