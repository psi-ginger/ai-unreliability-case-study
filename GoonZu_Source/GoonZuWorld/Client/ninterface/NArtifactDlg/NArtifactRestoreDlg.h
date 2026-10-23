//------------------------------------------------------------------------------
//
//	아티팩트 창.
//
//	2008. 2. 26
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _NARTIFACT_RESTORE_DLG_
#define _NARTIFACT_RESTORE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "Item/ItemCommon/Itemunit.h"

class NArtifactRestoreDlg : public CDialogController
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

	enum{ SLOT_COUNT = 6, DISASSEMBLE_ITEM_NUM = 5, };
	enum{ NO_SLOT	 = -1, EQUIP_SLOT, CRYSTAL_SLOT1, CRYSTAL_SLOT2, CRYSTAL_SLOT3, CRYSTAL_SLOT4, CRYSTAL_SLOT5 }; 

public:
	NArtifactRestoreDlg();
	~NArtifactRestoreDlg();
	void Clear( void );

	void Create( CControllerMgr *pParent );
	void Action(void);
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY ); 

	static void CALLBACK StaticNArtifactRestoreDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NArtifactRestoreDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
   void SetArtifactStorage(cltItem* _itemlist);									// 환원 된 아이템 넣기.
   void InsertEquipItem( CListView* _pListView, cltItem* _cltItem );			// 슬롯에 아이템 넣기.
   void DelListViewItem( CListView* _pListView );								// 해당 슬롯에 아이템 비우기.
   void DelAllListViewItem( void );												// 모든 슬롯을 비우기.
   bool CanInsert( void );														// 아이템 배열 슬롯의 범위 확인.
   bool CanDel( void );															// 아이템 삭제가 가능한지 확인.
   void DeleteBuffer( SI32 _ClassType, void* _ClassData );						// 사용된 메시지 버퍼 메모리 제거.
   void SetDisasembleItem(cltItem* _cltItemlist);								// 분해되 아이템 리스트를 세팅한다. 그리고 게이지 준비.
   
   bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );					// 인벤토리에서 아이템을 끌어옴,
   void SendServerMsg( SI32 _MsgType );											// 서버에 메시지를 보냄.
   void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
   bool IsEmptyDisasembleItem(void);											// 분해가 되서 수령해야 할 비어 아이템이 있는가.
   bool IsRestoreItemType(SI32 _siItemUnique);									// 분해가 되는 아이템 타입인가
   void NotifyMsg( SI32 _MsgType );												// 관련 팝업 메시지.

public:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_InventoryPos;										// 장비 아이템의 인벤상의 위치.
	cltItem				m_clArtifactItem[ SLOT_COUNT ];				
	cltItem				m_clDisasembleItemItemList[DISASSEMBLE_ITEM_NUM];

	TSpr*				m_Spr[IMAGE_NUM];

	SI16				m_siGaugeCount;
	SI16				m_siEffectCount;
	bool				m_bGaugeAction;
	bool				m_bEffectAction;
};

#endif