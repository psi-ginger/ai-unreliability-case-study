//------------------------------------------------------------------------------
//
//	아티팩트 창.
//
//	2008. 2. 26
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _NARTIFACT_SLOT_CREATE_DLG_
#define _NARTIFACT_SLOT_CREATE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

class NArtifactSlotCreateDlg : public CDialogController
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
		

		IMAGE_NUM,					// 이미지 개수.   
	};

	enum{ 
			EQUIP_SLOT		 = 0,	// 장비   슬롯 번호.
			CRYSTAL_SLOT	 = 1,	// 결정체 슬롯 번호.
			USED_SLOT_COUNT  = 2,	// 사용가능한 슬롯 수.
			RESULT_ITEM_SLOT = 2,	// 결과가 나오는 슬롯번호.
		};

public:
	NArtifactSlotCreateDlg();
	~NArtifactSlotCreateDlg();
	void Clear( void );

	void Create( CControllerMgr *pParent );
	void Action(void);
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY ); 

	static void CALLBACK StaticNArtifactSlotCreateDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NArtifactSlotCreateDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool InsertEquip( CListView* _pListView, cltItem* _cltItem );				// 장비 슬롯에 아이템 넣기.
	bool InsertCrystal( CListView* _pListView, cltItem* _cltItem );				// 결정체 슬롯에 아이템 넣기.
	void DelListViewItem( CListView* _pListView );								// 해당 슬롯에 아이템 비우기.
	void DelAllListViewItem( void );											// 모든 슬롯을 비우기.
	bool CanInsertEquip( void );												// 아이템 배열 슬롯의 범위 확인.
	bool CanInsertCrystal( cltItem* _cltItem );									// 아이템 배열 슬롯의 범위 확인.
	bool CanInsert( void );														// 아이템 배열 슬롯의 범위 확인.
	bool CanDel( void );														// 아이템 삭제가 가능한지 확인.
	bool IsCristal( SI32 _ItemUnique );											// 결정체 아이템인가.
	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void SlotCreateSuccess( cltItem* _cltItem );
	void DeleteBuffer( SI32 _ClassType, void* _ClassData );
	void SendServerMsg( SI32 _MsgType );
	void NotifyMsg( SI32 _MsgType );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	bool IsSlotCreateMsg(void);

public:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_InventoryPos[USED_SLOT_COUNT];		
	cltItem				m_clArtifactItem[ USED_SLOT_COUNT ];	
	cltItem				m_clCreateSlotSuccessItem;

	SI08				m_CurrentSlot;

	TSpr*				m_Spr[IMAGE_NUM];

	SI16				m_siGaugeCount;
	SI16				m_siEffectCount;
	bool				m_bGaugeAction;
	bool				m_bEffectAction;
};

#endif