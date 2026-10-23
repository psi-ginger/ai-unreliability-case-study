//------------------------------------------------------------------------------
//
//	아티팩트 결정체 조합 창.
//
//	2008. 2. 25
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _NARTIFACT_MIXTURE_DLG_
#define _NARTIFACT_MIXTURE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

class NArtifactMixtureDlg : public CDialogController
{
	enum
	{
		GAGE_NUM	= 152,
		EFFECT_NUM	= 12
	};
	enum{ NO_SLOT = -1, EQUIP_SLOT, CRYSTAL_SLOT1, CRYSTAL_SLOT2, CRYSTAL_SLOT3, CRYSTAL_SLOT4, CRYSTAL_SLOT5, TOTAL_SLOT_COUNT }; 

public:
	NArtifactMixtureDlg();
	~NArtifactMixtureDlg();
	void Clear( void );

	void Create( CControllerMgr *pParent );

	void Action(void);
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY ); 

	static void CALLBACK StaticNArtifactMixtureDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NArtifactMixtureDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

		   
    bool InsertEquip( CListView* _pListView, cltItem* _cltItem );						// 장비 슬롯에 아이템 넣기.
	void InsertEquipSetCrystal( void );													// 장비 슬롯이 들어가면 해당 결정체 정보를 세팅.
    bool InsertCrystal( CListView* _pListView, cltItem* _cltItem );						// 결정체 슬롯에 아이템 넣기.
	void DelListViewItem( CListView* _pListView, SI08 _SlotNum );						// 해당 슬롯에 아이템 비우기.
	void DelAllListViewItem( void );													// 해당 슬롯에 아이템 비우기.
	bool CanInsertEquip( void );														// 아이템 배열 슬롯의 범위 확인.
	bool CanInsertCrystal( cltItem* _cltItem );											// 아이템 배열 슬롯의 범위 확인.
	bool CanDel( void );																// 아이템 삭제가 가능한지 확인.
	bool CanMixure( SI32 _ItemUnique );													// 조합이 가능 확인.
	bool IsCristal( SI32 _ItemUnique );													// 결정체 아이템인가.
	void MixtureSuccess( cltItem* _cltItem );											// 조합 성공. S->C
	void DeleteBuffer( SI32 _ClassType, void* _ClassData );								// 사용된 메시지 버퍼 메모리 제거.
	bool SameCrystalInvenItem( SI32 _InvenPos);											// 아이템이 인벤에서의 위치가 같은면 같은 아이템으로 확인하는 함수.
	bool SameEquipCrystalItem( cltItem* _cltItem );										// 장비 아이템에 슬롯에 추가하려는 결정체가 있는지 확인(같은능력으로 구분)
	void EquipExplain( cltItem* pEquip );												// 장비 아이템의 정보 설명.
	void CrystalExplain( CEdit* _pExplainStatic, cltItem* pEquipItem, SI16 _siSlot);	// 아이템에 박힌 슬롯의 정보를 보여준다.
	void EquipCrystalExplain( CEdit* _pExplainEdit, cltItem* pEquipCrystal, SI16 _siSlot ); // 현재 넣은 결정체의 정보를 보여준다.
	void NotifyMsg( SI32 _MsgType );	
	bool SameEquipCrystalAbility( cltItem* _cltItem );									// 같은 능력의 결정체가 있다면.
	bool Islevel(SI32 EquipItemUnique, SI32 CristalItemUnique);							// 크리스탈을 장착할때 장착가능한 레벨인지 검사.
	bool IsAbilityType(cltItem* pclEquipItem, SI32 CristalItemAbilytype);				// 결정체가 조합하려는 아이템에 조합이 되는 결정체 인지 검사.
	bool IsMixture(void);																// 조합이 가능한지 검사.


	void SendServerMsg( SI32 _MsgType );												// 서버에 메시지를 보냄.
	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	

public:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_InventoryPos[ TOTAL_SLOT_COUNT ];						// 슬롯에 들어간 아이템의 실재 인벤의 위치
	cltItem				m_clArtifactItem[ TOTAL_SLOT_COUNT ];					// 슬롯에 들어간 아이템.
	cltItem				m_clMixtureSuccessItem;

	SI08				m_CurrentSlot;

	BOOL				m_bCrystalSlotImageList[TOTAL_SLOT_COUNT];				// 결합중인 슬롯과 빈슬롯의 구분. 초기화시 장비에 박힌 슬롯만 true;

	TSpr*				m_Spr[4];

	
	
	SI16				m_siGaugeCount;
	SI16				m_siEffectCount;
	bool				m_bGaugeAction;
	bool				m_bEffectAction;
};

#endif
