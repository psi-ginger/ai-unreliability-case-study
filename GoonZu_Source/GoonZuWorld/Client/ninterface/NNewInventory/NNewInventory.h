#ifndef _NNEWINVENTORY_H_
#define _NNEWINVENTORY_H_

#include "../../InterfaceMgr/DialogController.h"

#include "../../../Common/Item/ItemCommon/ItemUnit.h"


#define NEWMAKE_MATERIALFOR_MAKEITEM	1
#define NEWMAKE_TOOLSFOR_MAKEITEM		2
#define NEWMAKE_ORIGINALFOR_ENCHANT		3
#define NEWMAKE_MATFOR_ENCHANT			4
#define NEWMAKE_ELEFOR_ENCHANT			5


class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CChatTabButton;

class cltSummonInfo;

class CItemInventoryBuffer;
struct stMoveData;
#define DEFAULT_MAKEITEMNUM 1

#define TAB_SELECTED_PERSON 0
#define TAB_SELECTED_HORSE 1
#define TAB_SELECTED_BAG 2

class cltMakeItemContainer
{
public:
	cltMakeItemContainer()
	{
		Init();
	}
	void Init()
	{
		for(SI32 i=0; i<4; i++)
		{
			cltools[i].Init();          //제조 아이템 툴
			m_sitools[i] = 0;           //제조툴 인덱스
		}
		for(SI32 i=0; i<25; i++)
		{
			clMaterials[i].Init();   //제조 아이템 
			m_siMaterials[i] = 0;    //제조 아이템 인덱스
		}
		clOrgEnchantItem.Init();     //인챈트 시킬 아이템
		clMatEnchantItem.Init();     //인챈트로 인해 소진될 아이템
		for(SI32 i=0; i<NOW_ENCHANT_NUMBER; i++)
		{	
			clEleEnchantItem[i].Init(); //인챈트 속성돌아이템
		}
	}

	cltItem cltools[4];			//제조를 위한 툴 아이템 저장공간
	SI32	m_sitools[4];			//제조툴 인덱스
	cltItem clMaterials[25];		//제조를 위한 재료 아이템 저장공간
	SI32	m_siMaterials[25];		//제조 아이템 인덱스
	cltItem clOrgEnchantItem;		//인챈트 시킬 아이템
	cltItem clMatEnchantItem;		//인챈트로 인해 소진될 아이템
	cltItem clEleEnchantItem[NOW_ENCHANT_NUMBER];	//인챈트 속성돌아이템

};


class CNNewInventory : public CDialogController
{
public:
	CNNewInventory();
	~CNNewInventory();

	void Create();

	static void CALLBACK StaticCallBackDialogNNewInventory( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewInventory( UINT nEvent, int nControlID, CControllerObj* pControl );

	void MoveItem( SI32 Destination );

	void ShowTools( SI32 Destination ); //제조창중 툴
	void ShowMaterials( SI32 Destination ); //제조창중 재료창

	void ShowEnchantOrg(SI32 Destination); //인챈트창중 인챈트시킬아이템 보여주기
	void ShowEnchantMat(SI32 Destination); //인챈트창중 사라질아이템 보여주기
	void ShowEnchantElem(SI32 Destination); //인챈트창중 속성아이템 보여주기

	void ChangeItem( SI32 Destination );
	void RepairItem( SI32 SelectedInventoryItemIndex );
	void UseNolimitTicket(SI32 ItemUnique,SI32 Type);
	void UseItem( SI16 Pos );
	void DropItem( SI16 Pos );
	void DropItem_New( cltItem* pclItem, SI16 Pos );
	void EquipItem( SI16 Pos );
	void ExchangeWeapon();
	void ExchangeClothes();

	bool CheckBag( SI32 siSource, SI32 siDestination );

	bool MoveItem( SI32 siSource, SI32 siDestination );
	bool ChangeItem( SI32 siSource, SI32 siDestination );

	//SI32    CheckIsInventory();
	SI32 GetInventoryType(SI32 type);
	SI32 GetInventoryTypeForSummon( SI32 siItemType );

	void CancellationSealingUp( SI32 siIndex);

	void Show();
	void Hide();

	void Action();

	bool IsSummonDataChanged();
	bool IsItemDataChanged();

	void LoadItemData();
	void LoadSummonData();
	void LoadInventoryTabData();

	void UpdateMakeItemData();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	bool Pop( stMoveData * pMoveData );
	bool Pop( BYTE* pMoveData );


	void IsMinimi(bool bSet);
	void ChangeMaxMode(bool bSet);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void CheckWrongSelectedInven();	// 말이 없는데 말인벤이 선택된 경우를 체크해준다
	void SelectInven(SI16 SelectInventoryIndex);

	void ShowInvenWeight();
	void ShowSlotNum();			// 인벤의 슬롯/총슬롯 갯수를 보여준다.

	bool CheckCanEnchintItem(SI32 unique );
	bool CheckCanPutonItem(SI32 siUnique);
	bool CheckCanEquipItem(SI32 siUnique);

	void SetCharStatusChanged(bool Changed){ m_bStatusChanged = Changed; };
	void CheckCharStatusChanged();
	void MakeSetEnchant(bool bMakeButton);

	void SetInventoryMode(SI16 DialogId);

	virtual bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

	void GetPositionFromControllerID( IN SI16 siControllerID, IN SI16 siLastSelectedIndex, OUT SI16* psiSelectPos, OUT SI16* psiMakeItemPos );

	void UseWarpTargetWorld(SI32 siItemUnique )	;	//	 어떤 티켓을 가지고 해당 위치로 가야 할때 

	void UseLevelLimitBoxOpen(SI32 siItemUnique )	;	// [성웅] 레벨 보상 상자에서 아이템을 준다 

	

public:
	static SI16	m_siSelectedIndex;
	static bool	m_bMini;
	static bool	m_bMax;

	SI16		m_siActionFrame;
	SI16		m_siCurrentSummonIndex;
	SI16		m_siCurrentInventoryIndex;
	SI16		m_siComboBoxSelectedIndex;
	SI16		m_siLastSelectInventoryIndex;
	bool		m_bMakeButton;

	SI32		*m_pMakeableItemList;
	SI32		m_siMakeableItemNum;

	bool		m_bHorseHave;
	bool		m_bBagHave;
	bool		m_bClickChangeItem;	// 더블 클릭으로 아이템 바꾼다(무기류)
	bool		m_bStatusChanged;

	cltItem		*m_pclInventoryItem;
	CItemInventoryBuffer *m_pBuffer;
	cltPItemInfo *m_pPersonItemInfo;

	CButton		*m_pMakeItemBtn;
	CButton		*m_pEnchantItemBtn;
	CButton		*m_pItemmallListBtn;
	//CComboBox	*m_pMakeItemComboBox;
	//CStatic		*m_pMakeItemStatic;
	CStatic		*m_pWeightStatic;
	//CEdit		*m_pMakeItemInputEdit;

	CListView	*m_pInventoryListView;
	CListView	*m_pMoveItemToHorseListView;
	CListView	*m_pMoveItemToBag0ListView;
	CListView	*m_pMoveItemToBag1ListView;

	CButton*	m_pButtonMini;
	CButton*	m_pButtonMax;
	CButton*	m_pButtonSpaceBox;					// [추가 : 황진성 2008. 1. 29 => 공간 상자.  버튼 포인터 .]
	//CChatTabButton*	m_pButtonManInventory;
	//CChatTabButton*	m_pButtonHorseInventory;
	//CChatTabButton*	m_pButtonBagInventory;

	CStatic*    m_pStatic[3];
	
	//	CStatic*    m_pStatic[2];
	static	cltMakeItemContainer	clMakeItemContainer;

};
#endif