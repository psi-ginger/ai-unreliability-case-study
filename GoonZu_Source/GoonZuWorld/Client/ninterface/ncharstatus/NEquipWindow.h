#pragma once

#include "../../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../InterfaceMgr/DialogController.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

class cltSummonInfo;

class CStatic;
class CEdit;
class CButton;
class CListView;

class CItemInventoryBuffer;
struct stMoveData;

//#define EQUIP_BTN_CNT	7	

class CNEquipWindow  : public CDialogController
{
public:
	enum EQUIP_WINDOW_VARIABLE
	{
		EWV_MAX_ALLBUTTON_NUM		= 7		// 장비 장착창에 쓰이는 버튼의 갯수
		,EWV_MAX_SELECTBUTTON_NUM	= 4		// 선택버튼의 갯수(사람 1, 소환수 3)
		,EWV_REAL_EQUIPITEM_NUM		= 14	// 리스트 뷰 갯수(실재 장착될 아이템 갯수)

		,EWV_HUMAN_SELECTBUTTON		= 0		// 선택버튼 사람
		,EWV_SUMMON_1_SELECTBUTTON	= 1		// 선택버튼 소환수 1
		,EWV_SUMMON_2_SELECTBUTTON	= 2		// 선택버튼 소환수 2
		,EWV_SUMMON_3_SELECTBUTTON	= 3		// 선택버튼 소환수 3
	};

public:
	CNEquipWindow();
	~CNEquipWindow();

	void Create();

	static void CALLBACK StaticCallBackDialogNEquipWindow( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNEquipWindow( UINT nEvent, int nControlID, CControllerObj* pControl );

	void EquipmentListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent );

	void Show();
	void Hide();

	void Action();

	bool IsSummonDataChanged();
	bool IsSummonItemDataChanged();
	bool IsItemDataChanged();
	
	void LoadItemData();
	void LoadSummonData();

	void MoveItem( SI32 Destination );
	void ChangeItem( SI32 Destination );

	void MoveItem( SI32 siSource, SI32 Destination );
	void ChangeItem( SI32 siSource, SI32 Destination );


	void ExchangeWeapon();
	void ExchangeClothes();
	//SI32    CheckIsInventory();
	SI32    GetInventoryType(SI32 type);

	bool Pop( stMoveData * pMoveData );
	bool Pop( BYTE* pMoveData );

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	virtual bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

	void SelectButtonChangeState( SI32 siSelectIndex, bool bOtherButtonFalse=false );
	void OnButtonSummonSelect( SI32 siSelectIndex );
	void OnButtonUpListView( CListView* pListView );


public:

	CButton*				m_pButton[EWV_MAX_ALLBUTTON_NUM];
	CListView*				m_pListView[EWV_REAL_EQUIPITEM_NUM];

	TSpr					m_SprBack;

	cltItem					*m_pclEquipItem;
	cltItem					*m_pclInventoryItem;
	cltSummonInfo			*m_pSummonInfo;
	CItemInventoryBuffer	*m_pBuffer;
	bool					m_bRideHorse;

	SI16					m_siSelectedIndex;
	SI16					m_siCurrentSummonIndex;
	SI16					m_siEquipSelectedIndex;
	SI16					LastSelectedIndex;

	TSpr					m_SprEquipItem;
};