#include "NInventory.h"

#include "../../Inventory/ItemInventoryBuffer.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/ControllerSurfaceObj.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../NotifyMsgDlg/NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

// 탭 변경시 리스트뷰 배경 이미지 파일 명은 변경 안되지만, 폰트(=index)는 변경되어야 함으로...
#define INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON	15
#define INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE	13
#define INVENTORY_TAB_IMAGE_FONT_INDEX_BAG		17

#define INVENTORY_WEIGHT_DRAW_WIDTH				70

#define MINPOS_SCROLL_WEIGHT					5
#define MAXPOS_SCROLL_WEIGHT					65

#define WEIGHT_SCROLL_OFFSET					10

#define WEIGHT_SURFACE_WIDTH					300
#define WEIGHT_SURFACE_HEIGHT					100

#define DEFAULT_MAKEITEMNUM						1

CNInventory::CNInventory() : CDialogController()
{
	m_siSelectedIndex = -1;
	//m_siPreItemDragIndex = -1;


	SI16 i = 0;
	
	m_pMakeItemBtn = NULL;
	m_pMakeItemComboBox = NULL;
	m_pMakeItemStatic = NULL;
	m_pMakeItemInputEdit = NULL;

	for ( i = 0; i < MAX_SHAPE_ITEM; ++i )
	{
		m_pEquipmemntItemListView [ i ] = NULL;
	}

	m_pMoveItemToHorseListView = NULL;
	m_pMoveItemToBagListView = NULL;

	m_pInventoryListView = NULL;

	for ( i = 0; i < 3; ++i )
	{
		m_pChangeEquipmentWindow[ i ] = NULL;
		m_pChangeInventoryWindow[ i ] = NULL;
	}

	m_pChangeWeapon = NULL;
	m_pChangeClothes = NULL;

	m_pclEquipItem = NULL;
	m_pclInventoryItem = NULL;

	m_siCurrentSummonIndex = -1;
	m_siCurrentInventoryIndex = 0;

	m_pMakeableItemList = NULL;
	m_siMakeableItemNum = 0;

	m_pSummonInfo = new cltSummonInfo();
	memset( m_pSummonInfo, 0, sizeof( cltSummonInfo ) );

	m_bHorseHave = false;
	m_bBagHave = false;

	m_pBuffer = new CItemInventoryBuffer();

	m_siComboBoxSelectedIndex = -1;

	m_siCurrentWeightDrawXSize = 0;

	m_siCurrentWeightDrawPos = INVENTORY_WEIGHT_DRAW_WIDTH;

	m_WeightImgEncBuffer = NULL;

	m_pWeightSurfaceObj = NULL;
	
	SetActionTimer( 500 );
}

CNInventory::~CNInventory()
{
	SI16 i = 0;

	if ( m_pWeightSurfaceObj )
	{
		delete m_pWeightSurfaceObj;
		m_pWeightSurfaceObj = NULL;
	}

	if ( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	if ( m_pSummonInfo )
	{
		delete m_pSummonInfo;
		m_pSummonInfo = NULL;
	}

	if ( m_pclEquipItem )
	{
		delete [] m_pclEquipItem;
		m_pclEquipItem = NULL;
	}

	if ( m_pclInventoryItem )
	{
		delete [] m_pclInventoryItem;
		m_pclInventoryItem = NULL;
	}
	
	if ( m_pMakeItemBtn )
	{
		delete m_pMakeItemBtn;
		m_pMakeItemBtn = NULL;
	}

	if ( m_pMakeItemComboBox )
	{
		delete m_pMakeItemComboBox;
		m_pMakeItemComboBox = NULL;
	}

	if ( m_pMakeItemStatic )
	{
		delete m_pMakeItemStatic;
		m_pMakeItemStatic = NULL;
	}

	if ( m_pMakeItemInputEdit )
	{
		delete m_pMakeItemInputEdit;
		m_pMakeItemInputEdit = NULL;
	}

	for ( i = 0; i < MAX_SHAPE_ITEM; ++i )
	{
		if ( m_pEquipmemntItemListView [ i ] )
		{
			delete m_pEquipmemntItemListView [ i ];
			m_pEquipmemntItemListView [ i ] = NULL;
		}
	}

	if ( m_pMoveItemToHorseListView )
	{
		delete m_pMoveItemToHorseListView;
		m_pMoveItemToHorseListView = NULL;
	}

	if ( m_pMoveItemToBagListView )
	{
		delete m_pMoveItemToBagListView;
		m_pMoveItemToBagListView = NULL;
	}

	if ( m_pInventoryListView )
	{
		delete m_pInventoryListView;
		m_pInventoryListView = NULL;
	}

	for ( i = 0; i < 3; ++i )
	{
		if ( m_pChangeEquipmentWindow[ i ] )
		{
			delete m_pChangeEquipmentWindow[ i ];
			m_pChangeEquipmentWindow[ i ] = NULL;
		}

		if ( m_pChangeInventoryWindow[ i ] )
		{
			delete m_pChangeInventoryWindow[ i ];
			m_pChangeInventoryWindow[ i ] = NULL;
		}
	}

	if ( m_pChangeWeapon )
	{
		delete m_pChangeWeapon;
		m_pChangeWeapon = NULL;
	}
	if(m_pChangeClothes)
	{
		delete m_pChangeClothes;
		m_pChangeClothes = NULL;
	}

	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}
}

void CNInventory::Create()
{
	CInterfaceFile file;
	
	// file.LoadFile(  TEXT("NInterface/Data/NInventory_surface수정본") );
    
	TCHAR* pTitle = GetTxtFromMgr(5067);
	CDialogController::Create( NINVENTORY_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNInventory, pTitle, TEXT("NInterface/Interface/Interface_242x411_00_000.SPR"), 0, false, 0, 100, 100, 242, 411, true, false, true );


	int i = 0;

	m_pclEquipItem = new cltItem[ MAX_SHAPE_ITEM ];
	memset( m_pclEquipItem, 0, sizeof( cltItem ) * MAX_SHAPE_ITEM );

	m_pclInventoryItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	memset( m_pclInventoryItem, 0, sizeof( cltItem ) * MAX_ITEM_PER_PERSON );

	m_pMakeItemBtn = new CButton( this );
	TCHAR* pText = GetTxtFromMgr(5068);
	m_pMakeItemBtn->Create( NINVENTORY_DIALOG_MAKEITEM_BUTTON, pText, TEXT("NInterface/Button/BUTTON_4X16_00_103.SPR"), 12, true, 3, 196, 388, 40, 16, 40, 16 );

	
	m_pMakeItemStatic = new CStatic( this );
	m_pMakeItemStatic->Create( NINVENTORY_DIALOG_MAKEITEM_STATIC, NULL, NULL, 0, false, 0, 114, 386, 30, 17, 30, 17 );
	pText = GetTxtFromMgr(3885);
	m_pMakeItemStatic->SetText( pText, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	m_pMakeItemInputEdit = new CEdit( this );
	m_pMakeItemInputEdit->Create( NINVENTORY_DIALOG_MAKEITEM_INPUTEDIT, NULL, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 142, 387, 52, 17, 52, 17, false, false );

	for ( i = 0; i < MAX_SHAPE_ITEM; ++i  )
	{	
		if ( i > 12 )			// 현재 13, 14, 15 없음.
		{
			break;
		}

		m_pEquipmemntItemListView[ i ] = new CListView( this );
		switch( i )
		{
		case 0:			// Helmet
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 0, false, 0, 13, 47, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 1:			// Armour
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 1, false, 0, 57, 47, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 2:			// Weapon1
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 2, false, 0, 13, 135, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 3:			// Neck
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 3, false, 0, 101, 135, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 4:			// Ring1
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 4, false, 0, 145, 91, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 5:			// Ring2
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 5, false, 0, 189, 91, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;		// Belt
		case 6:
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 6, false, 0, 101, 47, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 7:			// Shoes
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 7, false, 0, 145, 47, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 8:			// Weapon2
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 8, false, 0, 57, 135, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 9:			// Mantle
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 9, false, 0, 101, 91, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 10:		// Bag
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 10, false, 0, 189, 47, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 11:		// Hat
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 11, false, 0, 13, 91, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		case 12:		// Dress
			{
				m_pEquipmemntItemListView[ i ]->Create( NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW + i, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 12, false, 0, 57, 91, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100  );
			}
			break;
		}
	}

	m_pMoveItemToHorseListView = new CListView( this );
	m_pMoveItemToHorseListView->Create( NINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 13, false, 0, 145, 135, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100 );

	m_pMoveItemToBagListView = new CListView( this );
	m_pMoveItemToBagListView->Create( NINVENTORY_DIALOG_MOVEITEMTOBAG_LISTVIEW, TEXT("NInterface/Image/Image_40X40_00_000.Spr"), 17, false, 0, 189, 135, 40, 40, 40, 40, 0, 0, 0, 0, 40, 40, false, false, true, 100, 100 );

	m_pInventoryListView = new CListView( this );
	//m_pInventoryListView->Create( NINVENTORY_DIALOG_INVENTORY_LISTVIEW, TEXT("NInterface/Image/IMAGE_228x179_00_000.SPR"), 0, false, 0, 6, 208, 228, 179, 228, 179, 2, 2, 4, 4, 42, 41, true, true, true, 100, 100 );

	m_pInventoryListView->Create( NINVENTORY_DIALOG_INVENTORY_LISTVIEW, TEXT("NInterface/Image/Image_230x181_00_000.SPR"), 0, false, 0, 6, 206, 222, 179, 228, 179, 2, 2, 4, 4, 41, 41, true, true, true, 100, 100 );


	for ( i = 0; i < 3; ++i )
	{
		m_pChangeEquipmentWindow[ i ] = new CButton( this );
		m_pChangeInventoryWindow[ i ] = new CButton( this );

		m_pChangeEquipmentWindow[ i ]->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pChangeInventoryWindow[ i ]->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		switch ( i )
		{
		case 0:
			{
				m_pChangeEquipmentWindow[ i ]->SetFontColor( RGB( 0, 0, 0 ) );
				m_pChangeInventoryWindow[ i ]->SetFontColor( RGB( 0, 0, 0 ) );
				m_pChangeEquipmentWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 23, 22, 65, 16, 65, 16 );
				m_pChangeInventoryWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 97, 187, 43, 16, 43, 16 );
			}
			break;
		case 1:
			{
				m_pChangeEquipmentWindow[ i ]->SetFontColor( RGB( 255, 255, 255 ) );
				m_pChangeInventoryWindow[ i ]->SetFontColor( RGB( 255, 255, 255 ) );
				m_pChangeEquipmentWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 88, 22, 65, 16, 65, 16 );
				m_pChangeInventoryWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 143, 187, 43, 16, 43, 16 );
			}
			break;
		case 2:
			{
				m_pChangeEquipmentWindow[ i ]->SetFontColor( RGB( 255, 255, 255 ) );
				m_pChangeInventoryWindow[ i ]->SetFontColor( RGB( 255, 255, 255 ) );
				m_pChangeEquipmentWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 154, 22, 65, 16, 65, 16 );
				m_pChangeInventoryWindow[ i ]->Create( NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW0_BUTTON + i, NULL, TEXT("NInterface/Button/Button_4X16_00_103.Spr"), 0, true, 3, 192, 187, 43, 16, 43, 16 );
			}
			break;
		}
	}

	m_pChangeWeapon = new CButton( this );
	m_pChangeWeapon->Create( NINVENTORY_DIALOG_CHANGEWEAPON_BUTTON, NULL, TEXT("NInterface/Button/BUTTON_36x20_00_000.SPR"), 0, false, 0, 7, 185, 36, 20, 36, 20 );

	m_pChangeClothes = new CButton( this );
	m_pChangeClothes->Create( NINVENTORY_DIALOG_CHANGECLOTHES_BUTTON, NULL, TEXT("NInterface/Button/BUTTON_36x20_00_000.SPR"), 0, false, 0, 47, 185, 36, 20, 36, 20 );
	m_pChangeClothes->SetFontIndex(8);

	//cyj 무기교체, 의복교체 툴팁 추가
	pText = GetTxtFromMgr(5735);
	m_pChangeWeapon->SetUseToolTip( pText );
	pText = GetTxtFromMgr(5736);
	m_pChangeClothes->SetUseToolTip( pText );

	m_pMakeItemComboBox = new CComboBox( this );
	m_pMakeItemComboBox->Create( NINVENTORY_DIALOG_MAKEITEM_COMBOBOX, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 5, 387, 108, 17, 108, 187, 170 );

	SetTimerOnOff( true );
	Hide();



	return;
}

void CALLBACK CNInventory::StaticCallBackDialogNInventory( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNInventory *pThis = (CNInventory *)pControllerMgr;
	pThis->CallBackDialogNInventory( nEvent, nControlID, pControl );
}

void CALLBACK CNInventory::CallBackDialogNInventory( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;

	// 만들기 버튼
	case NINVENTORY_DIALOG_MAKEITEM_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR *InputText = m_pMakeItemInputEdit->GetText();

					if ( InputText && _tcslen( InputText ) > 0 )
					{
						SI16 MakeItemNum = _tstoi( InputText );

						if ( MakeItemNum < 1 )
						{
							break;
						}

						if ( m_siComboBoxSelectedIndex < 0 )
						{
							break;
						}
						
						// 수량 초과
						if ( m_pMakeableItemList && MakeItemNum > m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 + 1 ]  )
						{
							break;
						}

						m_pBuffer->Push( INV_MAKE_ITEM, m_pMakeableItemList[ m_siComboBoxSelectedIndex  * 2 ], MakeItemNum );
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	// 제조 가능 아이템 ComboBox
	case NINVENTORY_DIALOG_MAKEITEM_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siComboBoxSelectedIndex = m_pMakeItemComboBox->GetSelectedIndex();

					SetMakeItemNumEnable();
				}
				break;
			}
		}
		break;
	case NINVENTORY_DIALOG_INVENTORY_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();
					SI16 LastSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();

					if ( m_siSelectedIndex >= 0 )
					{
						if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
						{
							CControllerMgr *pDialog = NULL;
							CControllerObj *pControllerObj = NULL;

							if ( pclclient->m_pInterfaceMgr->GetControllerAtPoint( &pDialog, &pControllerObj ) == false )
							{
								POINT pt;
								GetCursorPos( &pt );

								HWND hFindWnd = WindowFromPoint( pt );
								if ( hFindWnd != NULL && hFindWnd == pclclient->GetHwnd() )
								{
									DropItem( m_siSelectedIndex );

									m_pInventoryListView->ClearSelectedItem();
									m_siSelectedIndex = -1;
								}
							}
							else
							{
								if ( pControllerObj )
								{
									SI16 SelectPos = -1;
									
									SI16 StartPos = 0;
									if ( m_siCurrentSummonIndex != -1 )
									{
										//StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
									}

									switch( pControllerObj->GetControllerID() )
									{
									case NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW:
										SelectPos = PERSONITEM_HELMET + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT1_LISTVIEW:
										SelectPos = PERSONITEM_ARMOUR + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT2_LISTVIEW:
										SelectPos = PERSONITEM_WEAPON1 + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT3_LISTVIEW:
										SelectPos = PERSONITEM_NECK + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT4_LISTVIEW:
										SelectPos = PERSONITEM_RING1 + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT5_LISTVIEW:
										SelectPos = PERSONITEM_RING2 + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT6_LISTVIEW:
										SelectPos = PERSONITEM_BELT + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT7_LISTVIEW:
										SelectPos = PERSONITEM_SHOES + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT8_LISTVIEW:
										SelectPos = PERSONITEM_WEAPON2 + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT9_LISTVIEW:
										SelectPos = PERSONITEM_MANTLE + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT10_LISTVIEW:
										SelectPos = PERSONITEM_BAG + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT11_LISTVIEW:
										SelectPos = PERSONITEM_HAT + StartPos;
										break;
									case NINVENTORY_DIALOG_EQUIPMENT12_LISTVIEW:
										SelectPos = PERSONITEM_DRESS + StartPos;
										break;
									case NINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW:
										{	
											// 사람 또는 가방 탭인 경우에만 이동 가능함으로.
											if ( m_siCurrentInventoryIndex == 0 || m_siCurrentInventoryIndex == 2 )
											{
												if ( pclClient->pclCM->CR[1]->HaveHorse() )
												{
													SI16 pos;
													cltItem cltoitem;
													SI32 rtnvalue;

													if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_HORSE, 
														&m_pclInventoryItem[ m_siSelectedIndex ],
														NULL, 
														&pos, 
														&cltoitem,
														&rtnvalue) == true)
													{
														SelectPos = pos;
													}
													else
													{
														SelectPos = -1;
													}
												}
											}
											else if ( m_siCurrentInventoryIndex == 1 )
											{
												if ( pclClient->pclCM->CR[1]->HaveHorse() )
												{
													SI16 pos;
													cltItem cltoitem;
													SI32 rtnvalue;

													if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
														&m_pclInventoryItem[ m_siSelectedIndex ],
														NULL, 
														&pos, 
														&cltoitem,
														&rtnvalue) == true)
													{
														SelectPos = pos;
													}
													else
													{
														SelectPos = -1;
													}
												}
											}
										}
										break;
									case NINVENTORY_DIALOG_MOVEITEMTOBAG_LISTVIEW:
										{	
											// 사람 또는 말 탭인 경우에만 가방으로 아이템 이동 가능
											if ( m_siCurrentInventoryIndex == 0 || m_siCurrentInventoryIndex == 1 )
											{
												if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
												{
													SI16 pos;
													cltItem cltoitem;
													SI32 rtnvalue;

													if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG, 
														&m_pclInventoryItem[ m_siSelectedIndex ],
														NULL, 
														&pos, 
														&cltoitem,
														&rtnvalue) == true)
													{
														SelectPos = pos;
													}
													else
													{
														SelectPos = -1;
													}
												}
											}
											else if ( m_siCurrentInventoryIndex == 2 )
											{
												if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
												{
													SI16 pos;
													cltItem cltoitem;
													SI32 rtnvalue;

													if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
														&m_pclInventoryItem[ m_siSelectedIndex ],
														NULL, 
														&pos, 
														&cltoitem,
														&rtnvalue) == true)
													{
														SelectPos = pos;
													}
													else
													{
														SelectPos = -1;
													}
												}
											}
										}
										break;
									case NINVENTORY_DIALOG_INVENTORY_LISTVIEW:
										SelectPos = LastSelectedIndex;
										break;
									default:
										break;
									}


									if ( SelectPos > -1 )
									{


										SI32 siItemUinque =m_pclInventoryItem[m_siSelectedIndex].siUnique; //m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique;
										SI32 ItemType= pclclient->pclItemManager->GetItemType(siItemUinque);
										SI32 siItemType = GetInventoryType(ItemType);
										
										if(SelectPos==PERSONITEM_WEAPON2 || SelectPos==PERSONITEM_WEAPON1)
										{
                                           if(ItemType >=ITEMTYPE_SWORD && ItemType <=ITEMTYPE_GUN)
										   {
											   siItemType = SelectPos;
										   }
										}

										if(SelectPos==PERSONITEM_RING1 || SelectPos==PERSONITEM_RING2)
										{
											if(ItemType == ITEMTYPE_RING)
											{
												siItemType = SelectPos;
											}
										}

										if(ItemType == ITEMTYPE_SCROLL || SelectPos>14 )
										{
											siItemType = SelectPos;
										}										

										if ( m_pclInventoryItem[ siItemType ].siUnique == 0 )
										{
											MoveItem( siItemType );
										}
										else if(m_pclInventoryItem[ siItemType ].siUnique != 0 && ItemType != ITEMTYPE_SCROLL)
										{
											ChangeItem( siItemType );										
										}
										else if ( SelectPos != m_siSelectedIndex )
										{
											m_pInventoryListView->SetFocus(false);
											m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, siItemType, m_pclInventoryItem[ m_siSelectedIndex ].siUnique, &m_pclInventoryItem[ siItemType ] );
										}
									}
								}
							}

							m_pInventoryListView->ClearSelectedItem();
							m_siSelectedIndex = -1;

							pclclient->m_pInterfaceMgr->ItemEndDrag();
						}
						else
						{
							//m_siPreItemDragIndex = -1;
						}
					}
					else
					{
						m_siSelectedIndex = SelectedIndex;
						//m_siPreItemDragIndex = -1;
					}
				}
				break;
			case EVENT_LISTVIEW_LBUTTONUP:
				{
					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();

					if ( SelectedIndex >= 0 && SelectedIndex == m_siSelectedIndex && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
					{	
						m_siSelectedIndex = SelectedIndex;

						//if ( m_siPreItemDragIndex != SelectedIndex )
						//{
						//	m_siPreItemDragIndex = SelectedIndex;
							SI32 GlobalImageIndex;
							SI32 GlobalImageFont;
							if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclInventoryItem[ SelectedIndex ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
							{
								pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ SelectedIndex ].siUnique, ITEMSOUND_SELECT );
								pclclient->m_pInterfaceMgr->ItemBeginDrag( GlobalImageIndex, GlobalImageFont );
							}
						//}
						//else
						//{
						//	m_siPreItemDragIndex = -1;
						//}
					}
					else
					{
						m_siSelectedIndex = -1;
						//m_siPreItemDragIndex = -1;
					}
				}
				break;
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					if ( m_siSelectedIndex >= 0 )
					{
						if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
						{
							m_pInventoryListView->ClearSelectedItem();
							m_siSelectedIndex = -1;

							pclclient->m_pInterfaceMgr->ItemEndDrag();
						}
					}
				}
				break;
			case EVENT_LISTVIEW_RBUTTONUP:
				{
					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();
					if ( SelectedIndex >= 0 && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
					{
						if ( m_pclInventoryItem[ SelectedIndex ].CanRepair( pclClient->pclItemManager ) )
						{
							RepairItem( SelectedIndex );
						}
						else if ( m_pclInventoryItem[SelectedIndex].IsSealingUp(pclClient->pclItemManager) )
						{
							CancellationSealingUp(SelectedIndex);
						}
						else
						{
							UseItem( SelectedIndex );
						}
					}
				}
				break;

			}
		}
		break;

	case NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 0 ], PERSONITEM_HELMET + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 0 ], PERSONITEM_HELMET, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT1_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 1 ], PERSONITEM_ARMOUR + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 1 ], PERSONITEM_ARMOUR, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT2_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 2 ], PERSONITEM_WEAPON1 + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 2 ], PERSONITEM_WEAPON1, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT3_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 3 ], PERSONITEM_NECK + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 3 ], PERSONITEM_NECK, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT4_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 4 ], PERSONITEM_RING1 + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 4 ], PERSONITEM_RING1, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT5_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 5 ], PERSONITEM_RING2 + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 5 ], PERSONITEM_RING2, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT6_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 6 ], PERSONITEM_BELT + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 6 ], PERSONITEM_BELT, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT7_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 7 ], PERSONITEM_SHOES + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 7 ], PERSONITEM_SHOES, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT8_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 8 ], PERSONITEM_WEAPON2 + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 8 ], PERSONITEM_WEAPON2, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT9_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 9 ], PERSONITEM_MANTLE + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 9 ], PERSONITEM_MANTLE, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT10_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 10 ], PERSONITEM_BAG + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 10 ], PERSONITEM_BAG, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT11_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 11 ], PERSONITEM_HAT + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 11 ], PERSONITEM_HAT, nEvent );
			}
		}
		break;
	case NINVENTORY_DIALOG_EQUIPMENT12_LISTVIEW:
		{
			if ( m_siCurrentSummonIndex != - 1 )
			{
				//EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 12 ], PERSONITEM_DRESS + PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex, nEvent );
			}
			else
			{
				EquipmentListViewMsgProc( m_pEquipmemntItemListView[ 12 ], PERSONITEM_DRESS, nEvent );
			}
		}
		break;


		// 말에게 아이템 이동
	case NINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW:
		break;
	case NINVENTORY_DIALOG_MOVEITEMTOBAG_LISTVIEW:
		break;

		// 장착창 변경( 사람, 소환영웅1, 소환영웅2 )
	case NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW0_BUTTON:
		{
			m_siCurrentSummonIndex = -1;
			LoadItemData();

			m_pChangeEquipmentWindow[ 0 ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChangeEquipmentWindow[ 1 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeEquipmentWindow[ 2 ]->SetFontColor( RGB( 255, 255, 255 ) );

			m_pChangeEquipmentWindow[ 0 ]->Refresh();
			m_pChangeEquipmentWindow[ 1 ]->Refresh();
			m_pChangeEquipmentWindow[ 2 ]->Refresh();
		}
		break;
	case NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW1_BUTTON:
		{
			m_siCurrentSummonIndex = 0;
			LoadItemData();

			m_pChangeEquipmentWindow[ 0 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeEquipmentWindow[ 1 ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChangeEquipmentWindow[ 2 ]->SetFontColor( RGB( 255, 255, 255 ) );

			m_pChangeEquipmentWindow[ 0 ]->Refresh();
			m_pChangeEquipmentWindow[ 1 ]->Refresh();
			m_pChangeEquipmentWindow[ 2 ]->Refresh();
		}
		break;
	case NINVENTORY_DIALOG_CHANGEEQUIPMENTWINDOW2_BUTTON:
		{
			m_siCurrentSummonIndex = 1;
			LoadItemData();

			m_pChangeEquipmentWindow[ 0 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeEquipmentWindow[ 1 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeEquipmentWindow[ 2 ]->SetFontColor( RGB( 0, 0, 0 ) );

			m_pChangeEquipmentWindow[ 0 ]->Refresh();
			m_pChangeEquipmentWindow[ 1 ]->Refresh();
			m_pChangeEquipmentWindow[ 2 ]->Refresh();
		}
		break;

		// 인벤토리 변경 ( 사람, 말, 가방 )
	case NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW0_BUTTON:
		{
			m_siCurrentInventoryIndex = 0;
			LoadItemData();

			m_pChangeInventoryWindow[ 0 ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChangeInventoryWindow[ 1 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeInventoryWindow[ 2 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeInventoryWindow[ 0 ]->Refresh();
			m_pChangeInventoryWindow[ 1 ]->Refresh();
			m_pChangeInventoryWindow[ 2 ]->Refresh();

			m_pMoveItemToHorseListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE );
			m_pMoveItemToBagListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );
		}
		break;
	case NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW1_BUTTON:
		{
			m_siCurrentInventoryIndex = 1;
			LoadItemData();

			m_pChangeInventoryWindow[ 0 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeInventoryWindow[ 1 ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChangeInventoryWindow[ 2 ]->SetFontColor( RGB( 255, 255, 255 ) );

			m_pChangeInventoryWindow[ 0 ]->Refresh();
			m_pChangeInventoryWindow[ 1 ]->Refresh();
			m_pChangeInventoryWindow[ 2 ]->Refresh();

			m_pMoveItemToHorseListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON );
			m_pMoveItemToBagListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );
		}
		break;
	case NINVENTORY_DIALOG_CHANGEINVENTORYWINDOW2_BUTTON:
		{
			m_siCurrentInventoryIndex = 2;
			LoadItemData();

			m_pChangeInventoryWindow[ 0 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeInventoryWindow[ 1 ]->SetFontColor( RGB( 255, 255, 255 ) );
			m_pChangeInventoryWindow[ 2 ]->SetFontColor( RGB( 0, 0, 0 ) );

			m_pChangeInventoryWindow[ 0 ]->Refresh();
			m_pChangeInventoryWindow[ 1 ]->Refresh();
			m_pChangeInventoryWindow[ 2 ]->Refresh();

			m_pMoveItemToHorseListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE );
			m_pMoveItemToBagListView->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON );
		}
		break;

		// 무기 변경
	case NINVENTORY_DIALOG_CHANGEWEAPON_BUTTON:
		{
			ExchangeWeapon();
		}
		break;
	case NINVENTORY_DIALOG_CHANGECLOTHES_BUTTON:
		{			
			ExchangeClothes();
		}
		break;
	}

	return;
}

void CNInventory::EquipmentListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	switch( nEvent )
	{
	case EVENT_LISTVIEW_LBUTTONDOWN:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();
			SI16 LastSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();

			if ( m_siSelectedIndex >= 0 )
			{
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{
					CControllerMgr *pDialog = NULL;
					CControllerObj *pControllerObj = NULL;

					if ( pclclient->m_pInterfaceMgr->GetControllerAtPoint( &pDialog, &pControllerObj ) == false )
					{
						if ( GetFocus() == pclclient->GetHwnd() )
						{
							DropItem( m_siSelectedIndex );

							pListView->ClearSelectedItem();
							m_siSelectedIndex = -1;
						}
					}
					else
					{
						if ( pControllerObj )
						{
							SI16 SelectPos = -1;

							SI16 StartPos = 0;
							if ( m_siCurrentSummonIndex != -1 )
							{
								//StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
							}

							switch( pControllerObj->GetControllerID() )
							{
							case NINVENTORY_DIALOG_EQUIPMENT0_LISTVIEW:
								SelectPos = PERSONITEM_HELMET + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT1_LISTVIEW:
								SelectPos = PERSONITEM_ARMOUR + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT2_LISTVIEW:
								SelectPos = PERSONITEM_WEAPON1 + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT3_LISTVIEW:
								SelectPos = PERSONITEM_NECK + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT4_LISTVIEW:
								SelectPos = PERSONITEM_RING1 + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT5_LISTVIEW:
								SelectPos = PERSONITEM_RING2 + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT6_LISTVIEW:
								SelectPos = PERSONITEM_BELT + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT7_LISTVIEW:
								SelectPos = PERSONITEM_SHOES + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT8_LISTVIEW:
								SelectPos = PERSONITEM_WEAPON2 + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT9_LISTVIEW:
								SelectPos = PERSONITEM_MANTLE + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT10_LISTVIEW:
								SelectPos = PERSONITEM_BAG + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT11_LISTVIEW:
								SelectPos = PERSONITEM_HAT + StartPos;
								break;
							case NINVENTORY_DIALOG_EQUIPMENT12_LISTVIEW:
								SelectPos = PERSONITEM_DRESS + StartPos;
								break;
							case NINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW:
								{	
									// 사람 또는 가방 탭인 경우에만 이동 가능함으로.
									if ( m_siCurrentInventoryIndex == 0 || m_siCurrentInventoryIndex == 2 )
									{
										if ( pclClient->pclCM->CR[1]->HaveHorse() )
										{
											SI16 pos;
											cltItem cltoitem;
											SI32 rtnvalue;

											if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_HORSE, 
												&m_pclInventoryItem[ m_siSelectedIndex ],
												NULL, 
												&pos, 
												&cltoitem,
												&rtnvalue) == true)
											{
												SelectPos = pos;
											}
											else
											{
												SelectPos = -1;
											}
										}
									}
									else if ( m_siCurrentInventoryIndex == 1 )
									{
										if ( pclClient->pclCM->CR[1]->HaveHorse() )
										{
											SI16 pos;
											cltItem cltoitem;
											SI32 rtnvalue;

											if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
												&m_pclInventoryItem[ m_siSelectedIndex ],
												NULL, 
												&pos, 
												&cltoitem,
												&rtnvalue) == true)
											{
												SelectPos = pos;
											}
											else
											{
												SelectPos = -1;
											}
										}
									}
								}
								break;
							case NINVENTORY_DIALOG_MOVEITEMTOBAG_LISTVIEW:
								{	
									// 사람 또는 말 탭인 경우에만 가방으로 아이템 이동 가능
									if ( m_siCurrentInventoryIndex == 0 || m_siCurrentInventoryIndex == 1 )
									{
										if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
										{
											SI16 pos;
											cltItem cltoitem;
											SI32 rtnvalue;

											if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG, 
												&m_pclInventoryItem[ m_siSelectedIndex ],
												NULL, 
												&pos, 
												&cltoitem,
												&rtnvalue) == true)
											{
												SelectPos = pos;
											}
											else
											{
												SelectPos = -1;
											}
										}
									}
									else if ( m_siCurrentInventoryIndex == 2 )
									{
										if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
										{
											SI16 pos;
											cltItem cltoitem;
											SI32 rtnvalue;

											if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
												&m_pclInventoryItem[ m_siSelectedIndex ],
												NULL, 
												&pos, 
												&cltoitem,
												&rtnvalue) == true)
											{
												SelectPos = pos;
											}
											else
											{
												SelectPos = -1;
											}
										}
									}
								}
								break;
							case NINVENTORY_DIALOG_INVENTORY_LISTVIEW:
								SelectPos = LastSelectedIndex;
								break;
							default:
								break;
							}

							if ( SelectPos > -1 )
							{
								if ( m_siSelectedIndex != SelectPos && m_pclInventoryItem[ SelectPos ].siUnique == 0 )
								{
									MoveItem( SelectPos );
								}
								else if ( SelectPos != m_siSelectedIndex )
								{
									m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, SelectPos, m_pclInventoryItem[ m_siSelectedIndex ].siUnique, &m_pclInventoryItem[ SelectPos ] );
								}
							}
						}
					}

					pListView->ClearSelectedItem();
					m_siSelectedIndex = -1;

					pclclient->m_pInterfaceMgr->ItemEndDrag();
				}
				else
				{
					//m_siPreItemDragIndex = -1;
				}
			}
			else
			{
				m_siSelectedIndex = SelectedIndex;
				//m_siPreItemDragIndex = -1;
			}
		}
		break;
	case EVENT_LISTVIEW_LBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if ( SelectedIndex >= 0 && SelectedIndex == m_siSelectedIndex && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
			{	
				m_siSelectedIndex = SelectedIndex;

				//if ( m_siPreItemDragIndex != SelectedIndex )
				//{
				//	m_siPreItemDragIndex = SelectedIndex;
					SI32 GlobalImageIndex;
					SI32 GlobalImageFont;
					if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclInventoryItem[ SelectedIndex ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
					{
						pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ SelectedIndex ].siUnique, ITEMSOUND_SELECT );
						pclclient->m_pInterfaceMgr->ItemBeginDrag( GlobalImageIndex, GlobalImageFont );
					}
				//}
				//else
				//{
				//	m_siPreItemDragIndex = -1;
				//}
			}
			else
			{
				m_siSelectedIndex = -1;
				//m_siPreItemDragIndex = -1;
			}
		}
		break;
	case EVENT_LISTVIEW_RBUTTONDOWN:
		{
			if ( m_siSelectedIndex >= 0 )
			{
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{
					pListView->ClearSelectedItem();
					m_siSelectedIndex = -1;

					pclclient->m_pInterfaceMgr->ItemEndDrag();
				}
			}
		}
		break;
	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();
			if ( SelectedIndex >= 0 && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
			{
				if ( m_pclInventoryItem[ SelectedIndex ].CanRepair( pclClient->pclItemManager ) )
				{
					RepairItem( SelectedIndex );
				}
				else if ( m_pclInventoryItem[SelectedIndex].IsSealingUp(pclClient->pclItemManager) )
				{
					CancellationSealingUp(SelectedIndex);
				}
				else
				{
					UseItem( SelectedIndex );
				}
			}
		}
		break;
	}

	/*
	switch( nEvent )
	{
	case EVENT_LISTVIEW_LBUTTONDOWN:
		{
			SI16 SelectedIndex = ItemPos;

			if ( m_siSelectedIndex >= 0 )
			{
				SI16 LastSelectedIndex = ItemPos;

				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{
					if ( m_siSelectedIndex != LastSelectedIndex )
					{
						// Move
						if ( m_pclInventoryItem[ LastSelectedIndex ].siUnique == 0 )
						{
							MoveItem( LastSelectedIndex );
						}
						else
						{
							m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, LastSelectedIndex, m_pclInventoryItem[ m_siSelectedIndex ].siUnique, &m_pclInventoryItem[ LastSelectedIndex ] );
						}

						if ( m_siCurrentSummonIndex != - 1 )
						{
							if ( m_pEquipmemntItemListView[ ItemPos - ( PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex ) ] )
							{
								m_pEquipmemntItemListView[ ItemPos - ( PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex ) ]->ClearSelectedItem();
							}
						}
						else
						{
							if ( m_pEquipmemntItemListView[ ItemPos - PERSONITEM_HELMET ] )
							{
								m_pEquipmemntItemListView[ ItemPos - PERSONITEM_HELMET ]->ClearSelectedItem();
							}
						}


						//m_pInventoryListView->ClearSelectedItem();
						m_siSelectedIndex = -1;
					}

					pclclient->m_pInterfaceMgr->ItemEndDrag();
				}
				else
				{
					m_siPreItemDragIndex = -1;
				}
			}
			else
			{
				m_siSelectedIndex = SelectedIndex;
				m_siPreItemDragIndex = -1;
			}
		}
		break;
	case EVENT_LISTVIEW_LBUTTONUP:
		{
			SI16 SelectedIndex = ItemPos;

			if ( SelectedIndex >= 0 && SelectedIndex == m_siSelectedIndex && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
			{	
				m_siSelectedIndex = SelectedIndex;

				if ( m_siPreItemDragIndex != SelectedIndex )
				{
					m_siPreItemDragIndex = SelectedIndex;
					SI32 GlobalImageIndex;
					SI32 GlobalImageFont;
					if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclInventoryItem[ SelectedIndex ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
					{
						pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ SelectedIndex ].siUnique, ITEMSOUND_SELECT );
						pclclient->m_pInterfaceMgr->ItemBeginDrag( GlobalImageIndex, GlobalImageFont );
					}
				}
				else
				{
					m_siPreItemDragIndex = -1;
				}
			}
			else
			{
				m_siSelectedIndex = -1;
				m_siPreItemDragIndex = -1;
			}
		}
		break;
	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = ItemPos;
			if ( SelectedIndex >= 0 && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
			{
				if ( m_pclInventoryItem[ SelectedIndex ].CanRepair( pclClient->pclItemManager ) )
				{
					RepairItem( SelectedIndex );
				}
				else if ( m_pclInventoryItem[SelectedIndex].IsSealingUp(pclClient->pclItemManager) )
				{
					CancellationSealingUp(SelectedIndex);
				}							
				else
				{
					UseItem( SelectedIndex );
				}
			}
		}
		break;
	}
	*/

	return;
}

void CNInventory::MoveItem( SI32 Destination )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_siSelectedIndex == PERSONITEM_BAG )
	{	
		// 가방 내로 가방 옮기기 불가
		if( Destination >= PERSONITEM_BAG0 && Destination < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY )
		{
			TCHAR* pTitle = GetTxtFromMgr(1167);
			TCHAR* pText = GetTxtFromMgr(1168);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		// 가방에 아이템 있다면 옮기기 불가
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY; ++t )
		{
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				TCHAR* pTitle = GetTxtFromMgr(1165);
				TCHAR* pText = GetTxtFromMgr(1166);

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
		}
	}


	SI32 MoveInfo = MAKELONG( m_siSelectedIndex, Destination );

	if ( m_pclInventoryItem[ Destination ].siUnique != 0 )
	{
		if ( m_pclInventoryItem[ Destination ].CanPile( pclClient->pclItemManager ) == false )
		{
			return;
		}
	}

	if((Destination >= 0  && Destination <= 14 ) )//|| (Destination >= PERSONITEM_SUMMON1EQUIP0  && Destination <= PERSONITEM_SUMMON2EQUIP0+14))
	{
		// 밀봉상태일때는 물품 이동 불가
		if(m_pclInventoryItem[ m_siSelectedIndex ].IsSealingUp(pclClient->pclItemManager))
		{
			TCHAR *pTitle = GetTxtFromMgr(5662);
			TCHAR *pText = GetTxtFromMgr(5663);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return;		
		}
	}


	pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );

	m_pBuffer->Push( INV_MOVE_ITEM, MoveInfo, &m_pclInventoryItem[ m_siSelectedIndex ] );

	return;
}

void CNInventory::RepairItem( SI32 SelectedInventoryItemIndex )
{
	if ( m_pclInventoryItem[ SelectedInventoryItemIndex ].CanRepair( pclClient->pclItemManager ) )
	{
		TCHAR* pTitle = GetTxtFromMgr(1141);
		TCHAR* pText = GetTxtFromMgr(1140);


		cltClient *pclclient = ( cltClient * )pclClient;
		
		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( SelectedInventoryItemIndex ) + sizeof( cltItem );
		memcpy( TempBuffer, &SelectedInventoryItemIndex, sizeof( SelectedInventoryItemIndex ) );
		memcpy( &TempBuffer[ sizeof( SelectedInventoryItemIndex ) ], &m_pclInventoryItem[ SelectedInventoryItemIndex ], sizeof( cltItem ) );

		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
        
		/*
		if (  MessageBox(NULL, pTitle, pText, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			m_pBuffer->Push( INV_REPAIR_ITEM, SelectedInventoryItemIndex, &m_pclInventoryItem[ SelectedInventoryItemIndex ] );
		}
		*/

		return;
	}
}

void CNInventory::CancellationSealingUp(SI32 siIndex)
{
	if ( m_pclInventoryItem[ siIndex ].IsSealingUp( pclClient->pclItemManager ) )
	{
		TCHAR* pTitle = GetTxtFromMgr(5661);
		TCHAR* pText = GetTxtFromMgr(5660);


		cltClient *pclclient = ( cltClient * )pclClient;

		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 3 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( siIndex ) + sizeof( cltItem );
		memcpy( TempBuffer, &siIndex, sizeof( siIndex ) );
		memcpy( &TempBuffer[ sizeof( siIndex ) ], &m_pclInventoryItem[ siIndex ], sizeof( cltItem ) );

		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		/*
		if (  MessageBox(NULL, pTitle, pText, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
		m_pBuffer->Push( INV_REPAIR_ITEM, SelectedInventoryItemIndex, &m_pclInventoryItem[ SelectedInventoryItemIndex ] );
		}
		*/

		return;
	}
}

void CNInventory::UseItem( SI16 Pos )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_pclInventoryItem[ Pos ].siUnique == 0 )
	{
		return;
	}

	if ( m_pclInventoryItem[ Pos ].GetType( pclClient->pclItemManager ) == ITEMTYPE_BOOK )
	{
		TCHAR* pTitle = GetTxtFromMgr(1143);
		TCHAR* pText = GetTxtFromMgr(1142);

		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( Pos ) + sizeof( cltItem );
		memcpy( TempBuffer, &Pos, sizeof( Pos ) );
		memcpy( &TempBuffer[ sizeof( Pos ) ], &m_pclInventoryItem[ Pos ], sizeof( cltItem ) );

		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		/*
		if (  MessageBox(NULL, pTitle, pText, MB_ICONQUESTION | MB_YESNO) == IDYES )
		{
			m_pBuffer->Push( INV_USE_ITEM, Pos, &m_pclInventoryItem[ Pos ] );
		}
		*/
	}
	else
	{
		m_pBuffer->Push( INV_USE_ITEM, Pos, &m_pclInventoryItem[ Pos ] );
	}

	m_siSelectedIndex = -1;

	return;
}

void CNInventory::DropItem( SI16 Pos )
{
	if ( m_pclInventoryItem[ Pos ].siUnique == 0 )
	{
		return;
	}

	TCHAR* pTitle = GetTxtFromMgr(5261);
	TCHAR* pText = GetTxtFromMgr(5262);

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 2 );
	BYTE TempBuffer[ 128 ];
	SI16 Size = sizeof( Pos ) + sizeof( cltItem );
	memcpy( TempBuffer, &Pos, sizeof( Pos ) );
	memcpy( &TempBuffer[ sizeof( Pos ) ], &m_pclInventoryItem[ Pos ], sizeof( cltItem ) );

	cltClient *pclclient = (cltClient *)pclClient;
	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

	m_siSelectedIndex = -1;

	/*
	m_pBuffer->Push( INV_DROP_ITEM, Pos, &m_pclInventoryItem[ Pos ] );

	m_siSelectedIndex = -1;
	*/
}

// WEAPON1 과 WEAPON2의 위치를 교환한다.
void CNInventory::ExchangeWeapon()
{
	if ( m_pclInventoryItem[ PERSONITEM_WEAPON1 ].siUnique == 0 && 
		m_pclInventoryItem[ PERSONITEM_WEAPON2 ].siUnique == 0 )
	{
		return;
	}

	cltMsg clMsg(GAMEMSG_REQUEST_EXCHANGEWEAPON, 0, NULL);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	m_siSelectedIndex = -1;

	return;
}

void CNInventory::ExchangeClothes()
{
	
	if ( m_pclInventoryItem[ PERSONITEM_DRESS ].siUnique == ITEMUNIQUE(0) ||
		m_pclInventoryItem[ PERSONITEM_ARMOUR ].siUnique == ITEMUNIQUE(0) || 
		m_pclInventoryItem[ PERSONITEM_HELMET ].siUnique == ITEMUNIQUE(0) || 
		m_pclInventoryItem[ PERSONITEM_HAT ].siUnique == ITEMUNIQUE(0) ||
		m_pclInventoryItem[ PERSONITEM_DRESS ].siUnique == ITEMUNIQUE(23500) ||
		m_pclInventoryItem[ PERSONITEM_DRESS ].siUnique == ITEMUNIQUE(23505) ||
		m_pclInventoryItem[ PERSONITEM_DRESS ].siUnique == ITEMUNIQUE(23510) ||
		m_pclInventoryItem[ PERSONITEM_HAT ].siUnique == ITEMUNIQUE(23010) ||
		m_pclInventoryItem[ PERSONITEM_HAT ].siUnique == ITEMUNIQUE(23000) ||
		m_pclInventoryItem[ PERSONITEM_HAT ].siUnique == ITEMUNIQUE(23005)
		)
	{
		return;
	}

	cltMsg clMsg(GAMEMSG_REQUEST_EXCHANGECLOTHES, 0, NULL);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	m_siSelectedIndex = -1;
	

	return;
}

void CNInventory::Show()
{	
	CDialogController::Show( true );

	m_pMakeItemInputEdit->SetText( NULL );


	// 상위탭 버튼에 이름 적기 위해서.
	if ( m_pChangeEquipmentWindow[ 0 ] )
	{
		m_pChangeEquipmentWindow[ 0 ]->SetText( (TCHAR*)pclClient->pclCM->CR[1]->GetName(), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}

	LoadInventoryTabData();
	LoadSummonData();
	LoadItemData();

	return;
}

void CNInventory::Hide()
{
	CDialogController::Show( false );

	return;
}

void CNInventory::Action()
{
	bool bBagHave = false;
	if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
	{
		bBagHave = true;
	}

	if(  m_bHorseHave != pclClient->pclCM->CR[1]->HaveHorse() ||
		m_bBagHave != bBagHave
		)
	{
		// 탭 다이얼로그 변화 체크 ( 말이 있느냐 없느냐에 따라 틀려져야지.. )
		LoadInventoryTabData();
	}

	if ( IsSummonDataChanged() )
	{
		LoadSummonData();
		LoadItemData();
	}
	else if ( IsItemDataChanged() )
	{
		LoadItemData();
	}

	m_siCurrentWeightDrawPos -= WEIGHT_SCROLL_OFFSET;

	if ( m_siCurrentWeightDrawXSize > 0 && m_siCurrentWeightDrawPos + m_siCurrentWeightDrawXSize <= MINPOS_SCROLL_WEIGHT )
	{
		m_siCurrentWeightDrawPos = INVENTORY_WEIGHT_DRAW_WIDTH;
	}

	return;
}

bool CNInventory::IsSummonDataChanged()
{
	SI32 id = 1;
	for ( SI16 i = 0; i < MAX_SUMMON_OWN; ++i )
	{
		if ( m_pSummonInfo->clSummon[ i ].siKind != pclClient->pclCM->CR[id]->pclCI->clSummonInfo.clSummon[ i ].siKind )
		{
			return true;
		}
	}

	return false;
}

bool CNInventory::IsItemDataChanged()
{
	SI32 id = 1;

	for ( int j = 0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclInventoryItem[ j ];
		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ j ];

		for( int i = 0;i < size; ++i )
		{
			if( src[ i ] != dst[ i ] )
			{
				return true;
			}
		}
	}

	return false;
}

void CNInventory::LoadItemData()
{
	SI16 StartPos = 0;

	// 소환수 Tab인 경우.
	if ( m_siCurrentSummonIndex != -1 )
	{
		//StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
	}


	// 장착창의 아이템 정보를 읽어와서 셑팅해 준다.
	SI16 i = 0;

	SI32 id = 1;

	for ( i = 0; i < MAX_SHAPE_ITEM; ++i )
	{
		if ( m_pEquipmemntItemListView[ i ] )
		{
			m_pEquipmemntItemListView[ i ]->DeleteAllItems();
		}
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	for ( i = StartPos; i < StartPos + MAX_SHAPE_ITEM; ++i )
	{
		m_pclEquipItem[ i - StartPos ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );
		
		if ( m_pclEquipItem[ i - StartPos ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclEquipItem[ i - StartPos ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pclEquipItem[ i - StartPos ].siUnique );

				if ( ItemName )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pclEquipItem[ i - StartPos ], ToolTipMsg, false );
					SI32 ddd = sizeof(ToolTipMsg);
					SI32 dsfgs = _tcslen(ToolTipMsg);

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, 1,ItemName, ToolTipMsg );

					if ( m_pEquipmemntItemListView[ i - StartPos ] )
					{
						m_pEquipmemntItemListView[ i - StartPos ]->SetStartItemPos( i );
						m_pEquipmemntItemListView[ i - StartPos ]->InsertItem( &ListViewItemData );
					}
				}
			}
		}
	}

	// 사람, 말, 가방 인벤
	StartPos = PERSONITEM_INV0 + ( m_siCurrentInventoryIndex * MAX_ITEM_PER_INVENTORY );
	m_pInventoryListView->DeleteAllItems();

	m_pInventoryListView->SetStartItemPos( StartPos );
	for ( i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_pclInventoryItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );

		if ( m_pclInventoryItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclInventoryItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pclInventoryItem[ i ].siUnique );

				if ( ItemName )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pclInventoryItem[ i ], ToolTipMsg );
					SI32 size = sizeof(ToolTipMsg);
					SI32 ddd = _tcslen(ToolTipMsg);

					if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_INVENTORY )
					{
						stListViewItemData ListViewItemData;
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_pclInventoryItem[ i ].siItemNum, ItemName, ToolTipMsg );
						m_pInventoryListView->InsertItem( &ListViewItemData );
					}
				}
			}
		}
	}


	// 주어진 재료를 근거로 생산 가능한 아이템의 리스트를 작성해서 리턴한다. 
	// 얻어온 리스트는 사용후 삭제해 주어야 한다. 
	SI32 siMakeableItemNum = 0;	

	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}	

	m_pMakeableItemList = NULL;

	m_pMakeItemComboBox->Clear();

	if( pclClient->pclItemManager->FindMakeableItem(
		&pclClient->pclCM->CR[id]->pclCI->clCharSkill,
		&pclClient->pclCM->CR[id]->pclCI->clCharItem,
		&pclClient->pclCM->CR[id]->clPB.clTotalBA, 
		MAX_INVITEM_PER_PERSON, 
		&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0], 
		&m_siMakeableItemNum, &m_pMakeableItemList,
		false) == true)
	{

		// 콤보 박스에 아이템의 생산가능한 명단을 표시한다. 
		for( i = 0; i < m_siMakeableItemNum; ++i )
		{
			SI32 unique		= m_pMakeableItemList[ i * 2 ];
			SI32 itemnum	= m_pMakeableItemList[ i * 2 +1];

			if ( unique !=  0 )
			{
				TCHAR buffer[256];
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( unique );

				if(ref > 0)
				{	
					TCHAR* pText = GetTxtFromMgr(676);

					StringCchPrintf( buffer, 256, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );

					stComboBoxItemData ComboBoxItemData;
					ComboBoxItemData.Init();
					ComboBoxItemData.Set( buffer );					

					m_pMakeItemComboBox->AddItem( &ComboBoxItemData );
				}
			}
		}
	}

	TCHAR strWeightBuffer[ 128 ] = TEXT("");
	SI32 siTotalWeight = pclClient->pclCM->CR[1]->GetTotalWeight();		// 현재 인벤토리의 총무게
	SI32 siMaxWeight = pclClient->pclCM->CR[1]->GetMaxWeight();			// 들수 있는 최대 무게

	TCHAR* pText = GetTxtFromMgr(1158);


	StringCchPrintf( strWeightBuffer, 128, pText, (float)siTotalWeight/10.0, (float)siMaxWeight/10.0);

	if ( m_pWeightSurfaceObj == NULL )
	{
		m_pWeightSurfaceObj = new CControllerSurfaceObj();
		m_pWeightSurfaceObj->Create( WEIGHT_SURFACE_WIDTH, WEIGHT_SURFACE_HEIGHT );
	}
	
	// 무게 설정 부분
	LPDIRECTDRAWSURFACE7 lpSurface = m_pWeightSurfaceObj->GetSurface();

	if ( lpSurface )
	{
		HDC hDC;
		HFONT hFont;
		

		g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
		
		if( lpSurface->GetDC( &hDC ) == DD_OK)
		{
			cltClient *pclclient = (cltClient *)pclClient;
			if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
				hFont = CreateFont( BASE_FONT_HEIGHT, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, DEFAULT_FONT_KIND );
			}
			else {
				hFont = CreateFont( BASE_FONT_HEIGHT, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, DEFAULT_FONT_KIND );
			}			
			HFONT hOldFont = NULL;
			hOldFont = (HFONT)SelectObject( hDC, hFont );

			SIZE sz;
			if ( GetTextExtentPoint32( hDC, strWeightBuffer, _tcslen( strWeightBuffer ), &sz ) > 0 )
			{
				m_siCurrentWeightDrawXSize = sz.cx;

				// +10 은 그냥 넉넉한 사이즈를 위한것
				m_siWeightSurfaceWidth = sz.cx + ( sz.cx % 2 );			// 2의 배수 맞추기 위해서...
				m_siWeightSurfaceHeight = sz.cy;

				if ( m_WeightImgEncBuffer )
				{
					delete [] m_WeightImgEncBuffer;
					m_WeightImgEncBuffer = NULL;
				}
				
				m_WeightImgEncBuffer = new RGB16[ m_siWeightSurfaceWidth * m_siWeightSurfaceHeight * 2 ];
				SetBkMode( hDC, TRANSPARENT );

				//RECT Rect = { 0, 0, 74, 17 };
				//DrawText( hDC, strWeightBuffer, _tcslen( strWeightBuffer ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
				
				//cyj 무게 표시 폰트 안짤리도록 수정
				//TextOut( hDC, 0, 3, strWeightBuffer, _tcslen( strWeightBuffer ) );
				TextOut( hDC, 0, 0, strWeightBuffer, _tcslen( strWeightBuffer ) );
			}
			else
			{
				m_siCurrentWeightDrawXSize = 0;
			}

			SelectObject( hDC, hOldFont );
			if ( hFont )
			{
				DeleteObject( hFont );
				hFont = NULL;
			}

			lpSurface->ReleaseDC( hDC );
		}

		if ( m_siCurrentWeightDrawXSize > 0 && m_WeightImgEncBuffer != NULL && GP.LockSurface( lpSurface ) )
		{
			int size;
			g_graphics.EncodeXpr( m_WeightImgEncBuffer, m_siWeightSurfaceWidth, m_siWeightSurfaceHeight, RGB16_TRANS, &size );

			GP.UnlockSurface( lpSurface );
		}
	}

	return;
}


void CNInventory::LoadSummonData()
{
	SI32 id = 1;
	// 소환수 부분
	memcpy( m_pSummonInfo, &pclClient->pclCM->CR[id]->pclCI->clSummonInfo, sizeof( cltSummonInfo ) );

	SI16 Count = 0;

	// 사람
	TCHAR* pText = (TCHAR*)pclClient->pclCM->CR[id]->GetName();

	if ( pText == NULL )
	{
		pText = GetTxtFromMgr(1159);
	}
	m_pChangeEquipmentWindow[ Count++ ]->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );


	for ( SI16 i = 0; i < MAX_SUMMON_OWN; ++i )
	{
		//if ( m_pSummonInfo->clSummon[ i ].siKind > 0 )
		//{
			if ( m_pSummonInfo->clSummon[ i ].szName != NULL )
			{
				m_pChangeEquipmentWindow[ Count++ ]->SetText( m_pSummonInfo->clSummon[ i ].szName, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			}
		//}
		//else
		//{
			// 소환영웅
		//	pText = GetTxtFromMgr(848);

		//	m_pChangeEquipmentWindow[ Count++ ]->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		////}
	}

	return;
}

void CNInventory::LoadInventoryTabData()
{
	SI32 id = 1;

	SI16 Counter = 0;

	TCHAR *pText = GetTxtFromMgr(1159);	
	m_pChangeInventoryWindow[ Counter++ ]->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	m_bHorseHave = pclClient->pclCM->CR[id]->HaveHorse();

	pText = GetTxtFromMgr(372);
	m_pChangeInventoryWindow[ Counter ]->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	if ( m_bHorseHave )
	{
		m_pChangeInventoryWindow[ Counter++ ]->Enable( true );
	}
	else
	{	
		m_pChangeInventoryWindow[ Counter++ ]->Enable( false );
	}

	pText = GetTxtFromMgr(1033);
	m_pChangeInventoryWindow[ Counter ]->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	if ( pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
	{
		m_bBagHave = true;
		m_pChangeInventoryWindow[ Counter++ ]->Enable( true );
	}
	else
	{
		m_bBagHave = false;
		m_pChangeInventoryWindow[ Counter++ ]->Enable( false );
	}

	return;
}


void CNInventory::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  )
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 id = 1;

	// 사람 탭인 경우에만 현재 무기 설정 이미지 보여줘야지..
	if ( m_siCurrentSummonIndex == SELECTED_HUMAN )
	{
		if ( pclClient->pclCM->CR[id]->ActiveWeapon > 0 && m_pclInventoryItem[ pclClient->pclCM->CR[id]->ActiveWeapon ].siUnique > 0 )
		{
			if ( pclClient->pclCM->CR[id]->ActiveWeapon == PERSONITEM_WEAPON1 )
			{
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON1 ]->SetFontIndex( 14 );
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON2 ]->SetFontIndex( 8 );
			}
			else if( pclClient->pclCM->CR[id]->ActiveWeapon == PERSONITEM_WEAPON2 )
			{
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON1 ]->SetFontIndex( 2 );
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON2 ]->SetFontIndex( 14 );
			}
			else
			{
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON1 ]->SetFontIndex( 2 );
				m_pEquipmemntItemListView[ PERSONITEM_WEAPON2 ]->SetFontIndex( 8 );
			}
		}
		else
		{
			m_pEquipmemntItemListView[ PERSONITEM_WEAPON1 ]->SetFontIndex( 2 );
			m_pEquipmemntItemListView[ PERSONITEM_WEAPON2 ]->SetFontIndex( 8 );		

		}



		
		if ( pclClient->pclCM->CR[id]->ActiveClothes > 0 && m_pclInventoryItem[ pclClient->pclCM->CR[id]->ActiveClothes ].siUnique > 0 &&
			pclClient->pclCM->CR[id]->ActiveHat >= 0 && m_pclInventoryItem[ pclClient->pclCM->CR[id]->ActiveHat ].siUnique > 0
			)
		{
			if ( pclClient->pclCM->CR[id]->ActiveClothes == PERSONITEM_DRESS )
			{
				m_pEquipmemntItemListView[ PERSONITEM_DRESS ]->SetFontIndex( 18 );
				m_pEquipmemntItemListView[ PERSONITEM_ARMOUR ]->SetFontIndex( 1 );

				m_pEquipmemntItemListView[ PERSONITEM_HAT ]->SetFontIndex( 18 );
				m_pEquipmemntItemListView[ PERSONITEM_HELMET ]->SetFontIndex( 0 );
			
	
				TCHAR* pText = GetTxtFromMgr(5345);
				m_pChangeClothes->SetUseToolTip(pText);

				pText = GetTxtFromMgr(5344);
				m_pChangeWeapon->SetUseToolTip(pText);


			}
			else if( pclClient->pclCM->CR[id]->ActiveClothes == PERSONITEM_ARMOUR )
			{
				m_pEquipmemntItemListView[ PERSONITEM_DRESS ]->SetFontIndex( 1 );
				m_pEquipmemntItemListView[ PERSONITEM_ARMOUR ]->SetFontIndex( 19 );

				m_pEquipmemntItemListView[ PERSONITEM_HAT ]->SetFontIndex( 0 );
				m_pEquipmemntItemListView[ PERSONITEM_HELMET ]->SetFontIndex( 18 );
				TCHAR* pText = GetTxtFromMgr(5343);
				m_pChangeClothes->SetUseToolTip(pText);

				pText = GetTxtFromMgr(5344);
				m_pChangeWeapon->SetUseToolTip(pText);

			}
			else
			{
				m_pEquipmemntItemListView[ PERSONITEM_DRESS ]->SetFontIndex( 12 );
				m_pEquipmemntItemListView[ PERSONITEM_ARMOUR ]->SetFontIndex( 1 );

				m_pEquipmemntItemListView[ PERSONITEM_HAT ]->SetFontIndex( 11 );
				m_pEquipmemntItemListView[ PERSONITEM_HELMET ]->SetFontIndex( 0 );
			}
		}

		else
		{		
			m_pEquipmemntItemListView[ PERSONITEM_DRESS ]->SetFontIndex( 12 );
			m_pEquipmemntItemListView[ PERSONITEM_ARMOUR ]->SetFontIndex( 1 );

			m_pEquipmemntItemListView[ PERSONITEM_HELMET ]->SetFontIndex( 0 );
			m_pEquipmemntItemListView[ PERSONITEM_HAT ]->SetFontIndex( 11 );

		}

	}
	
}


void CNInventory::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	cltClient *pclclient = (cltClient *)pclClient;

	// 사람 탭인 경우에만 단축키 출력 그리고 현재 무기 설정 이미지 보여주는 것도 되지( tab 버튼으로 바꾸는 거 )
	if ( m_siCurrentInventoryIndex == 0 )
	{
		SI16 i = 0;
		TSpr *pAcceleratorImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( LISTVIEW_ACCELERATOR_FILENAME );

		if ( pAcceleratorImg )
		{
			for ( i = 0; i < 5; ++i )
			{
				// 시작 : 9, 298    OffSet : 57
				GP.PutSpr(pAcceleratorImg, ScreenRX + GetX() + 9 + i * 46, ScreenRY + GetY() + 210, LISTVIEW_ACCELERATOR_FONTINDEX + i + 1 /*1은 F0 부터 시작함으로*/ );
			}
		}
	}

	if ( m_siCurrentWeightDrawXSize > 0 )
	{
		if ( m_siCurrentWeightDrawXSize > 0 )
		{
			if ( m_siCurrentWeightDrawPos + m_siCurrentWeightDrawXSize > MINPOS_SCROLL_WEIGHT && m_siCurrentWeightDrawPos <= MAXPOS_SCROLL_WEIGHT )
			{
				int x1, y1, x2, y2;
				int clip_x1, clip_y1, clip_x2, clip_y2;

				GP.GetClipArea( x1, y1, x2, y2 );

				clip_x1 = max( ScreenRX + GetX() + 119 + MINPOS_SCROLL_WEIGHT, x1 );
				clip_y1 = max( ScreenRY + GetY() + 5, y1 );
				clip_x2 = min( ScreenRX + GetX() + 119 + MAXPOS_SCROLL_WEIGHT, x2 );
				clip_y2 = min( clip_y1 + 17, y2 );

				GP.SetClipArea( clip_x1, clip_y1, clip_x2, clip_y2 );
				
				GP.PutXpr16( ScreenRX + GetX() + 119 + m_siCurrentWeightDrawPos, ScreenRY + GetY() + 5,
					m_siWeightSurfaceWidth, m_siWeightSurfaceHeight,
					m_WeightImgEncBuffer,
					RGB16_TRANS );

				GP.SetClipArea( x1, y1, x2, y2 );
			}
		}
	}

	return;
}

bool CNInventory::Pop( stMoveData * pMoveData )
{
	return m_pBuffer->Pop( pMoveData );	
}

bool CNInventory::Pop( BYTE* pMoveData )
{
	return m_pBuffer->Pop( pMoveData );
}

void CNInventory::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				SI32 *SelectedInventoryItemIndex = (SI32 *)pData;
				cltItem *pclItem = (cltItem *)&pData[4];

				if ( m_pclInventoryItem[ *SelectedInventoryItemIndex ].siUnique == 0 )
				{
					return;
				}

				m_pBuffer->Push( INV_REPAIR_ITEM, *SelectedInventoryItemIndex, pclItem );
			}
		}
		break;
	case 1:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				SI16 *Pos = (SI16 *)pData;
				cltItem *pclItem = (cltItem *)&pData[2];
				
				if ( m_pclInventoryItem[ *Pos ].siUnique == 0 )
				{
					return;
				}

				m_pBuffer->Push( INV_USE_ITEM, *Pos, pclItem );
			}
		}
		break;
	case 2:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI16 *Pos = (SI16 *)pData;
				cltItem *pclItem = (cltItem *)&pData[2];

				if ( m_pclInventoryItem[ *Pos ].siUnique == 0 )
				{
					return;
				}

				m_pBuffer->Push( INV_DROP_ITEM, *Pos, pclItem );

				m_siSelectedIndex = -1;
			}
		}
		break;
	case 3:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				SI32 *SelectedInventoryItemIndex = (SI32 *)pData;
				cltItem *pclItem = (cltItem *)&pData[4];

				if ( m_pclInventoryItem[ *SelectedInventoryItemIndex ].siUnique == 0 )
				{
					return;
				}

				m_pBuffer->Push( INV_CANCELLATIONSEALINGUP_ITEM, *SelectedInventoryItemIndex, pclItem );
			}
		}
		break;	

	}
	return;
}


SI32 CNInventory::GetInventoryType(SI32 siItemType)
{
	SI32 StartPos =0;
	SI32 siItemUinque =m_pclInventoryItem[m_siSelectedIndex].siUnique;

	if (siItemType == ITEMTYPE_HELMET )
		return PERSONITEM_HELMET;

	else if  (siItemType == ITEMTYPE_ARMOUR  )
		return PERSONITEM_ARMOUR;

	else if(( siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == ITEMUNIQUE(10100) ||siItemUinque == ITEMUNIQUE(10090)) && (m_pclInventoryItem[ PERSONITEM_WEAPON1 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_WEAPON1 + StartPos ))
		return PERSONITEM_WEAPON2;

	else if ( siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == ITEMUNIQUE(10100) ||siItemUinque == ITEMUNIQUE(10090))
		return PERSONITEM_WEAPON1;

	else if ( siItemType == ITEMTYPE_NECK || siItemType == ITEMTYPE_AVATAR_NECK) // 아바타 목걸이
		return PERSONITEM_NECK;

	else if (( siItemType == ITEMTYPE_RING  || siItemType == ITEMTYPE_AVATAR_RING ) // 아바타 반지
		&&( m_pclInventoryItem[ PERSONITEM_RING2 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_RING2 + StartPos ))
		return PERSONITEM_RING1;

	else if ( siItemType == ITEMTYPE_RING )
		return PERSONITEM_RING2;

	else if ( siItemType == ITEMTYPE_BELT )
		return PERSONITEM_BELT;

	else if ( siItemType == ITEMTYPE_SHOES )
		return PERSONITEM_SHOES;

	else if ( siItemType == ITEMTYPE_MANTLE || siItemType == ITEMTYPE_AVATAR_MANTLE ) //  아바타 망토
		return PERSONITEM_MANTLE;

	else if ( siItemType == ITEMTYPE_BAG )
		return PERSONITEM_BAG;

	else if ( siItemType == ITEMTYPE_HAT || siItemType == ITEMTYPE_AVATAR_HAT) // 아바타 모자
		return PERSONITEM_HAT;

	else if ( siItemType == ITEMTYPE_DRESS || siItemType == ITEMTYPE_AVATAR_DRESS ) // 아바타 의복
		return PERSONITEM_DRESS;
	else
	{
		return PERSONITEM_NONE;
	}
}


void CNInventory::ChangeItem( SI32 Destination )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_siSelectedIndex == PERSONITEM_BAG )
	{	
		// 가방 내로 가방 옮기기 불가
		if( Destination >= PERSONITEM_BAG0 && Destination < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY )
		{
			TCHAR* pTitle = GetTxtFromMgr(1167);
			TCHAR* pText = GetTxtFromMgr(1168);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		// 가방에 아이템 있다면 옮기기 불가
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY; ++t )
		{
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				TCHAR* pTitle = GetTxtFromMgr(1165);
				TCHAR* pText = GetTxtFromMgr(1166);

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
		}
	}


	SI32 MoveInfo = MAKELONG( m_siSelectedIndex, Destination );
/*
	if ( m_pclInventoryItem[ Destination ].siUnique != 0 )
	{
		if ( m_pclInventoryItem[ Destination ].CanPile( pclClient->pclItemManager ) == false )
		{
			return;
		}
	}
*/
	if((Destination >= 0  && Destination <= 14 ))// || (Destination >= PERSONITEM_SUMMON1EQUIP0  && Destination <= PERSONITEM_SUMMON2EQUIP0+14))
	{
		// 밀봉상태면 옮기기 불가
		if(m_pclInventoryItem[ m_siSelectedIndex ].IsSealingUp(pclClient->pclItemManager))
		{
			TCHAR *pTitle = GetTxtFromMgr(5662);
			TCHAR *pText = GetTxtFromMgr(5663);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return;		
		}
	}


	pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );
	m_pBuffer->Push( INV_CHANGE_ITEM, MoveInfo, &m_pclInventoryItem[ m_siSelectedIndex ] );


	return;


}

void CNInventory::SetMakeItemNumEnable()
{
	if (m_siComboBoxSelectedIndex >= 0)
	{
		
		SI32 unique		= m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 ];
		SI32 itemnum	= m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 + 1 ];

		if ( unique !=  0 )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( unique );

			if(ref > 0)
			{	
				TCHAR buffer[128] = TEXT("");

				if (pclClient->pclItemManager->pclItemInfo[ref]->clItem.CanPile(pclClient->pclItemManager) == FALSE)
				{
					StringCchPrintf(buffer, 128, TEXT("%d"), DEFAULT_MAKEITEMNUM);
					m_pMakeItemInputEdit->SetReadOnly(true);
				}
				else
				{
					m_pMakeItemInputEdit->SetReadOnly(false);
				}
				m_pMakeItemInputEdit->SetText(buffer);
			}
		}
	}
}