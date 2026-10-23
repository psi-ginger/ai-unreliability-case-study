#include "NNewInventory.h"

#include "../../ninterface/nnewmakeitem/NNewMakeItemDlg.h"
#include "../../ninterface/NSelectItemCountDlg/NSelectItemCountDlg.h"
#include "../../ninterface/ngachadlg/NGachaDlg.h"

#include "../../Inventory/ItemInventoryBuffer.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/ChatTabButton.h"

#include "../../InterfaceMgr/ControllerSurfaceObj.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../Client/NInterface/NCharStatus/NCharStatus.h"
#include "../../Client/NInterface/NCharStatus/NEquipWindow.h"
#include "../../Client/ninterface/NTreasureBoxDlg/NTreasureBoxDlg.h"
#include "../../Client/ninterface/NOpenBigItemBoxDlg/NOpenBigItemBoxDlg.h"
#include "../../Client/ninterface/NMySummonDlg/NMySummonDlg.h"

#include "../../../../DBManager/GameDBManager_World/DBMsg-ItemMall.h"

//#include "../../Client/NInterface/NCharStatus/NCharEquipWindow.h"

//#include "../Client/NInterface/NTutorialDlg/NTutorialDlg.h"
#include "../Client/ninterface/NArtifactDlg/NArtifactBaseDlg.h"
#include "../Client/ninterface/NElixirDlg/NElixirUseDlg.h"
#include "../Client/ninterface/NNewSelectBoxDlg/NNewSelectBoxDlg.h"

#include "../Client/ninterface/nnpcrepairdlg/NNewNPCRepair.h"
#include "../Client/ninterface/nnewmakeitem/NItemRepair.h"
#include "../Client/ninterface/nnewmakeitem/NNewMakeItemDlg.h"

#include "../Client/Interface/NHalloweenSpiritBox/NHalloweenSpiritBox.h"

#include "../CommonLogic/Msg/MsgType-Quest.h"
#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../CommonLogic/BoxItemMgr/BoxItemMgr.h"
//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../Server/GachaManager/GachaManager.h"
#include "../Server/Gacha2Manager/Gacha2Manager.h"

#include "../../Client/Client.h"	
extern cltCommonLogic* pclClient;

// 탭 변경시 리스트뷰 배경 이미지 파일 명은 변경 안되지만, 폰트(=index)는 변경되어야 함으로...
#define INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON	15
#define INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE	13
#define INVENTORY_TAB_IMAGE_FONT_INDEX_BAG		17

// 다른 인벤토리로 아이템 이동시 마지막 선택값이 어디에서 왔는지 알기 위한..
#define INVEN_FROM_PERSON					0
#define INVEN_FROM_HORSE					1
#define INVEN_FROM_BAG0						2
#define INVEN_FROM_BAG1						3

SI16 CNNewInventory::m_siSelectedIndex = -1;
bool CNNewInventory::m_bMax = false;
bool CNNewInventory::m_bMini = false;
cltMakeItemContainer CNNewInventory::clMakeItemContainer;

CNNewInventory::CNNewInventory() : CDialogController()
{
	m_bMakeButton = false;
	//m_siSelectedIndex = -1;
	m_siCurrentSummonIndex = SELECTED_HUMAN;
	m_siCurrentInventoryIndex = TAB_SELECTED_PERSON;
	m_siComboBoxSelectedIndex = -1;
	m_siLastSelectInventoryIndex = -1;

	m_pMakeableItemList = NULL;
	m_siMakeableItemNum = 0;

	m_bHorseHave = false;
	m_bBagHave = false;
	m_bClickChangeItem = false;
	m_bStatusChanged = false;
	m_pclInventoryItem = NULL;
	m_pBuffer = new CItemInventoryBuffer();

	m_pMakeItemBtn = NULL;
	m_pEnchantItemBtn = NULL;
	m_pButtonSpaceBox = NULL;
	//m_pMakeItemComboBox = NULL;
	//m_pMakeItemStatic = NULL;
	m_pWeightStatic	= NULL;
	//m_pMakeItemInputEdit = NULL;

	m_pInventoryListView = NULL;
	m_pMoveItemToHorseListView = NULL;
	m_pMoveItemToBag0ListView = NULL;
	m_pMoveItemToBag1ListView = NULL;

	m_pButtonMini = NULL;
	m_pButtonMax = NULL;
	//m_pButtonManInventory = NULL;
	//m_pButtonHorseInventory = NULL;
	//m_pButtonBagInventory = NULL;



	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;

	SetActionTimer( 1000 );
}

CNNewInventory::~CNNewInventory()
{
	if ( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	if ( m_pclInventoryItem )
	{
		delete [] m_pclInventoryItem;
		m_pclInventoryItem = NULL;
	}

	if ( m_pWeightStatic )
	{
		delete m_pWeightStatic;
		m_pWeightStatic = NULL;

	}

	if ( m_pMakeItemBtn )
	{
		delete m_pMakeItemBtn;
		m_pMakeItemBtn = NULL;
	}

	if ( m_pEnchantItemBtn )
	{
		delete	m_pEnchantItemBtn;
		m_pEnchantItemBtn = NULL;
	}


	if ( m_pInventoryListView )
	{
		delete m_pInventoryListView;
		m_pInventoryListView = NULL;
	}

	SAFE_DELETE( m_pItemmallListBtn );
	SAFE_DELETE( m_pButtonSpaceBox );

	SAFE_DELETE( m_pMoveItemToHorseListView );
	SAFE_DELETE( m_pMoveItemToBag0ListView );
	SAFE_DELETE( m_pMoveItemToBag1ListView );

	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}


	SAFE_DELETE( m_pButtonMini );
	SAFE_DELETE( m_pButtonMax );
	//SAFE_DELETE(m_pButtonManInventory );
	//SAFE_DELETE(m_pButtonHorseInventory );
	//SAFE_DELETE(m_pButtonBagInventory );

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);

}

void CNNewInventory::Create()
{
	
	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NNewInventoryDlg/DLG_NewInventory.ddf"));
	file.CreatePopupDialog( this, NNEWINVENTORY_DLG, TEXT("dialog_NewInventory"), StaticCallBackDialogNNewInventory );

	cltClient *pclclient = (cltClient *)pclClient;

	MovePos( pclclient->GetScreenXSize() - 218, 250 );


	int i = 0;

	m_pclInventoryItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	memset( m_pclInventoryItem, 0, sizeof( cltItem ) * MAX_ITEM_PER_PERSON );



	NEWCBUTTON(m_pMakeItemBtn);
	NEWCBUTTON(m_pEnchantItemBtn);
	NEWCBUTTON(m_pItemmallListBtn);
	NEWCBUTTON(m_pButtonSpaceBox);			//[추가 : 황진성 2008. 1. 29 => 공간 상자. 버튼 메모리 확보]
	NEWCSTATIC(m_pWeightStatic);

	NEWCLISTVIEW(m_pInventoryListView);
	NEWCLISTVIEW(m_pMoveItemToHorseListView);
	NEWCLISTVIEW(m_pMoveItemToBag0ListView);
	NEWCLISTVIEW(m_pMoveItemToBag1ListView);

	NEWCBUTTON(m_pButtonMini);
	NEWCBUTTON(m_pButtonMax);

	//m_pButtonManInventory = new CChatTabButton(this);
	//m_pButtonHorseInventory = new CChatTabButton(this);
	//m_pButtonBagInventory = new CChatTabButton(this);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);


	file.CreateControl( m_pItemmallListBtn, NNEWINVENTORY_DIALOG_ITEMMALL_BUTTON, TEXT("button_NONAME3"));
	file.CreateControl( m_pEnchantItemBtn, NNEWINVENTORY_DIALOG_ENCHANTITEM_BUTTON, TEXT("button_NONAME4")); 
	file.CreateControl( m_pMakeItemBtn, NNEWINVENTORY_DIALOG_MAKEITEM_BUTTON, TEXT("button_NONAME5")); 
	
	file.CreateControl( m_pWeightStatic, NNEWINVENTORY_DIALOG_WEIGHT_STATIC, TEXT("statictext_weight"));

	file.CreateControl( m_pInventoryListView, NNEWINVENTORY_DIALOG_INVENTORY_LISTVIEW, TEXT("listview_NONAME1")); 
	file.CreateControl( m_pMoveItemToHorseListView, NNEWINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW, TEXT("listview_NONAME2")); 
	file.CreateControl( m_pMoveItemToBag1ListView, NNEWINVENTORY_DIALOG_MOVEITEMTOBAG1_LISTVIEW, TEXT("listview_NONAME3")); 
	file.CreateControl( m_pMoveItemToBag0ListView, NNEWINVENTORY_DIALOG_MOVEITEMTOBAG0_LISTVIEW, TEXT("listview_NONAME4")); 

	file.CreateControl( m_pButtonMini, NNEWINVENTORY_DIALOG_BUTTON1, TEXT("button_NONAME1"));
	file.CreateControl( m_pButtonMax, NNEWINVENTORY_DIALOG_BUTTON2, TEXT("button_NONAME2")); 

	// [추가 : 황진성 2008. 1. 29 => 공간 상자.  버튼생성 .]
	file.CreateControl( m_pButtonSpaceBox, NNEWINVENTORY_DIALOG_BUTTON_SPACE_BOX, TEXT("button_space_box")); 
	
	//file.CreateControl( m_pButtonManInventory, NNEWINVENTORY_DIALOG_BUTTON2, TEXT("button_NONAME2")); 
	//file.CreateControl( m_pButtonHorseInventory, NNEWINVENTORY_DIALOG_BUTTON3, TEXT("button_NONAME3")); 
	//file.CreateControl( m_pButtonBagInventory, NNEWINVENTORY_DIALOG_BUTTON4, TEXT("button_NONAME4")); 

	file.CreateControl( m_pStatic[0], NNEWINVENTORY_DIALOG_STATIC1, TEXT("statictext_NONAME1")); 
	file.CreateControl( m_pStatic[1], NNEWINVENTORY_DIALOG_STATIC2, TEXT("statictext_NONAME2")); 
	file.CreateControl( m_pStatic[2], NNEWINVENTORY_DIALOG_STATIC3, TEXT("statictext_NONAME3"));

	m_pStatic[0]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE );
	m_pStatic[1]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );
	m_pStatic[2]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );


	// [추가 : 황진성 2008. 1. 29 => 공간 상자. 인벤관련 버튼의 위치 수정과 제거.]
	if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
	{
		m_pButtonMini->SetFileName("NInterface/Button/BUTTON_Min_Max_24X38_00_000.SPR");
		m_pButtonMini->SetX( m_pButtonMini->GetX() - 3);
		m_pButtonMax->Show( false );	// 최대화 버튼은 사용하지 않으므로 안보이게.
		
		SI16 Space = 44; 
		m_pMakeItemBtn->SetY( m_pButtonMax->GetY() );
		m_pEnchantItemBtn->SetY( m_pMakeItemBtn->GetY()	    + Space );
		m_pItemmallListBtn->SetY( m_pEnchantItemBtn->GetY() + Space );
		m_pButtonSpaceBox->SetY(m_pItemmallListBtn->GetY()	+ Space );
	}
	else
	{
		m_pButtonSpaceBox->Show( false );
	}
	//[유상]영문판일경우 폰트크기 늘림 - 한국추가 By LEEKH
	if( pclClient->IsWhereServiceArea(ConstServiceArea_English 
									| ConstServiceArea_Korea 
									| ConstServiceArea_USA 
									| ConstServiceArea_NHNChina 
									| ConstServiceArea_EUROPE) 
	)
	{
		m_pWeightStatic->SetFontSize(11);
	}
	else
	{
		m_pWeightStatic->SetFontSize(10); 
	}

	TCHAR* pText = GetTxtFromMgr(5067); //  "소지품( alt+i)"
	SetTitle(pText,CONTROLLER_TITLE_COLOR,11);

	if ( GetDialogControllerID() != NNEWINVENTORY_DLG  )
	{
        m_pWeightStatic->Show( false );
	}

	if ( pclclient->siServiceArea == ConstServiceArea_China )
	{
		ShowSlotNum();
	}
	else
	{
		ShowInvenWeight();
	}


	m_pCloseButton->SetFileName(TEXT("NInterface/common/Common_12x12_00_000.SPR"));

	SetTimerOnOff( true );

	CDialogController::Show( false );

	return;
}

void CALLBACK CNNewInventory::StaticCallBackDialogNNewInventory( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
 	CNNewInventory *pThis = (CNNewInventory *)pControllerMgr;
	pThis->CallBackDialogNNewInventory( nEvent, nControlID, pControl );
}

void CALLBACK CNNewInventory::CallBackDialogNNewInventory( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	CControllerMgr *pDialog = NULL;
	CControllerObj *pControllerObj = NULL;

	if ( pclclient->m_pInterfaceMgr->GetControllerAtPoint( &pDialog, &pControllerObj ) == true )
	{
		if ( pDialog->GetDialogControllerID() != GetDialogControllerID()  && pControllerObj != NULL)
		{
			((CNNewInventory*)pDialog)->CallBackDialogNNewInventory( nEvent, pControllerObj->GetControllerID(), pControllerObj );
		}
	}


	switch ( nControlID )
	{
	// [추가 : 황진성 2008. 1. 29 => 공간 상자 창 클릭으로 띄우기.]
	case NNEWINVENTORY_DIALOG_BUTTON_SPACE_BOX:
		{
			pclclient->CreateInterface( NSPACE_BOX );
		}
		break;
	case NNEWEQUIPWINDOW_DLG_LISTVIEW1:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					int a = 0;
				}
				break;
			}
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;

		// 만들기 버튼
	case NNEWINVENTORY_DIALOG_MAKEITEM_BUTTON:
		{
			// 제조창을 여는 퀘스트 진행인 경우
			SI16 questtype, questunique, para1, para2;
			SI16 questclearcond = QUEST_CLEAR_COND_OPENMAKEITEM;
			if(	pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
				&& para1 == clMakeItemContainer.clMatEnchantItem.siUnique )
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				cltClient* pclclient = (cltClient* )pclClient;
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}

			if(pclclient->m_pDialog[ NNEWITEMMAKE_DLG ] == NULL)
			{
				pclclient->CreateInterface(NNEWITEMMAKE_DLG);
				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);

				if( ( pclclient->pclCM->CR[1]->HaveHorse() || 
					pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique > 0 )
					&& m_bMax )
				{
					(pclclient->m_pDialog[ NNEWITEMMAKE_DLG ])->MovePos( 450, 205 );
				}
			}
		}
		break;
		// 인첸트 버튼
	case NNEWINVENTORY_DIALOG_ENCHANTITEM_BUTTON:
		{
			if(pclclient->m_pDialog[ NNEWITEMMAKE_DLG ] == NULL)
			{
				pclclient->CreateInterface(NNEWITEMMAKE_DLG);
				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);

				((CNewMakeItemDlg*)pclclient->m_pDialog[ NNEWITEMMAKE_DLG ])->ShowEnchantTab();

				if( ( pclclient->pclCM->CR[1]->HaveHorse() || 
					pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique > 0 )
					&& m_bMax )
				{
					(pclclient->m_pDialog[ NNEWITEMMAKE_DLG ])->MovePos( 450, 205 );
				}
			}
		}
		break;
 
	case NNEWINVENTORY_DIALOG_ITEMMALL_BUTTON:
		{
			if(pclClient->IsCountrySwitch(Switch_PeriodItem))		//	090326 [종호]	기간제버튼 눌릴시 내게 걸린 버프의 기간을 표시하느냐 아니면 내가 가진 아이템중 ITEMMALL && 기간 인 아이템을 표시하느냐
			{
				if(pclclient->m_pDialog[ NPERIODITEMLIST_DLG ] == NULL )
				{
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
					pclclient->CreateInterface(NPERIODITEMLIST_DLG);
					pclclient->m_pDialog[ NPERIODITEMLIST_DLG ]->MovePos( 303, 205 );
				}
			}
			else
			{
				if(pclclient->m_pDialog[ NITEMMALLPRODUCT_DLG ] == NULL )
				{
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
					pclclient->CreateInterface(NITEMMALLPRODUCT_DLG);
					pclclient->m_pDialog[ NITEMMALLPRODUCT_DLG ]->MovePos( 303, 205 );
				}

			}
		}


	case NNEWINVENTORY_DIALOG_INVENTORY_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_DBLCLK:
				{

					pclclient->m_pInterfaceMgr->ItemEndDrag();

					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();
					// SI16 LastSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();
					EquipItem( SelectedIndex );


				}
				break;
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					if ( (true == pclClient->pclCM->CR[1]->bPersonalShopSwitch) || 
						 (true == pclClient->pclCM->CR[1]->bPremiumPersonalShop) )
					{
						cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;

						if(pChar != NULL )
						{
							if( pclClient->IsCountrySwitch( Switch_Change_manufacture_request_interface ) )
							{
								if(pChar->siShopMode == PSHOP_MODE_MAKEITEM || pChar->siShopMode == PSHOP_MODE_MAKEITEM_WANT)
								{
									TCHAR* pTitle = GetTxtFromMgr(1891);
									TCHAR* pText = GetTxtFromMgr(20099);	//제조/제작 의뢰 중에는 아이템의 이동이 불가능 합니다.
									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
									break ;
								}
							}							
						}
						else
						{
							TCHAR* pTitle = GetTxtFromMgr(1891);
							TCHAR* pText = GetTxtFromMgr(10038);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							break ;
						}
					}
					
					// 새로운 클릭앤 드랍에서는 아무 처리를 하지 않는다.
					if( pclClient->IsCountrySwitch(Switch_ClickAndDrop))
					{
						break;	// case 탈출
					}
					

					if( pclclient->m_pDialog[NSELECTITEMCOUNT_DLG] != NULL )
					{
						(pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->DeleteDialog();
					}

					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();
					SI16 LastSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();

					CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
					CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
					CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
					CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

					CControllerMgr *pDialog = NULL;
					CControllerObj *pControllerObj = NULL;

					if ( pclclient->m_pInterfaceMgr->GetControllerAtPoint( &pDialog, &pControllerObj ) == true )
					{
						SI16 DialogID = pDialog->GetDialogControllerID();
						switch ( DialogID )
						{
						case NNEWINVENTORY_DLG:
							LastSelectedIndex = personinventory->m_pInventoryListView->GetLastSelectedItemIndex();
							break;
						case NNEWINVENTORY_HORSE_DLG:
							LastSelectedIndex = horseinventory->m_pInventoryListView->GetLastSelectedItemIndex();
							break;
						case NNEWINVENTORY_BAG0_DLG:
							LastSelectedIndex = bag0inventory->m_pInventoryListView->GetLastSelectedItemIndex();
							break;						
						case NNEWINVENTORY_BAG1_DLG:
							LastSelectedIndex = bag1inventory->m_pInventoryListView->GetLastSelectedItemIndex();
							break;						}
					}

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
									if( ( pclclient->pclItemManager->GetItemAtb( m_pclInventoryItem[ m_siSelectedIndex ].siUnique ) & ITEMATB_OBJECT) == false )
									{
										if ( pclClient->IsCountrySwitch(Switch_DestroyDropItem) )
										{
											/*bool bTemp1 = pclclient->pclItemManager->IsCanDestroyDropItem( m_pclInventoryItem[ m_siSelectedIndex ].siUnique)	;
											bool bTemp2 = m_pclInventoryItem->IsBelonging( pclClient->pclItemManager ) ;*/
											if ( m_pclInventoryItem[ m_siSelectedIndex ].GetItemNum() > 1)
											{
												if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
												{
													pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
												}
												((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_DROPITEM );
												
											}
											else
											{
												DropItem( m_siSelectedIndex );
												
											}
										}
										else{
											if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
											{
												pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
											}
											((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_DROPITEM );
										}
										
									}
									else
									{
										DropItem( m_siSelectedIndex );
									}
									m_pInventoryListView->ClearSelectedItem();
									m_siSelectedIndex = -1;

								}
							}
							else
							{
								if ( pControllerObj )
								{
									SI16 SelectPos				= -1;
									SI16 MakeItemPos			= -1;
									m_siCurrentSummonIndex		= SELECTED_HUMAN;
									

									GetPositionFromControllerID( pControllerObj->GetControllerID(), LastSelectedIndex ,&SelectPos, &MakeItemPos );
									

									// 장비 장착창으로의 이동이고 장비 장착착이 열려있다면
									if ( ((0 <= SelectPos) && (MAX_SHAPE_ITEM > SelectPos)) && (pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ]) )
									{
										// 그때는 사람인지 소환수인지 판단해본다
										m_siCurrentSummonIndex = ((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->m_siCurrentSummonIndex;
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

										// 사람일때
										if ( SELECTED_HUMAN == m_siCurrentSummonIndex )
										{
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
										// 소환수 일때
										else
										{
											//if ( pclClient->pclCM->CR[1]->pclCI->clSummonInfo.clItemInfo[ m_siCurrentSummonIndex ].clEquipItem[ SelectPos ].siUnique == 0 )
											{
												MoveItem( SelectPos );
											}
										}

									}
									if( MakeItemPos > 0) //제료  in 제조
									{
										SI32 siItemUinque =m_pclInventoryItem[m_siSelectedIndex].siUnique; //m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique;
										SI32 ItemType= pclclient->pclItemManager->GetItemType(siItemUinque);									

										if ( m_pclInventoryItem[m_siSelectedIndex].siUnique != 0 ) //ShowItem
										{	
											TCHAR * pTitleText = GetTxtFromMgr(5927); // 조합불가 라는 알림창 타이틀을 미리 설정해둔다.
											TCHAR * pExplain  = NULL;

											if(MakeItemPos ==NEWMAKE_MATERIALFOR_MAKEITEM)
												ShowMaterials(m_siSelectedIndex);
											else if(MakeItemPos ==NEWMAKE_TOOLSFOR_MAKEITEM)
											{
												if ( m_pclInventoryItem[m_siSelectedIndex].IsSealingUp(pclclient->pclItemManager) == true )
												{
													pTitleText = GetTxtFromMgr(5662);
													pExplain = GetTxtFromMgr(5663);	// 밀봉을 해제한후 사용하세요
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText , pExplain);				
												}
												else
													ShowTools(m_siSelectedIndex);
											}

											else if(MakeItemPos ==NEWMAKE_ORIGINALFOR_ENCHANT)
											{
												if (CheckCanPutonItem(m_pclInventoryItem[m_siSelectedIndex].siUnique ) == false)
												{
													pExplain = GetTxtFromMgr(5928);	// 무기 방어구만 조합가능합니다
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText , pExplain);
												}												
												else
													ShowEnchantOrg(m_siSelectedIndex);
											}
											else if(MakeItemPos ==NEWMAKE_MATFOR_ENCHANT)
											{
												SI32 org = pclclient->pclCM->CR[1]->siOrgInvEnchantInx;

												if(org ==0)
												{ 
													pExplain = GetTxtFromMgr(5929);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}
												else if(m_siSelectedIndex == org)
												{
													pExplain = GetTxtFromMgr(5930);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}
												else if (CheckCanPutonItem(m_pclInventoryItem[m_siSelectedIndex].siUnique ) == false)
												{									
													pExplain = GetTxtFromMgr(5928);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}
												else if (m_pclInventoryItem[org].siUnique != m_pclInventoryItem[m_siSelectedIndex].siUnique )
												{
													pExplain = GetTxtFromMgr(5931);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}
												else
													ShowEnchantMat(m_siSelectedIndex);
											}
											else if(MakeItemPos ==NEWMAKE_ELEFOR_ENCHANT)
											{
												// 대상 아이템이 설정되어 있지 않다
												if( pclclient->pclCM->CR[1]->siOrgInvEnchantInx <= 0)
												{
													pExplain = GetTxtFromMgr(5929);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}
												// 인첸트 속성 변경 아이템이 놓인상태라면
												else if ( (m_pclInventoryItem[m_siSelectedIndex].siUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE)
													&& (pclClient->pclCM->CR[1]->siOrgInvEnchantInx > 0)
													&& (m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].siUnique > 0)
													&& (m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager) > 0)
												)
												{
													pTitleText	= GetTxtFromMgr(7940);
													pExplain	= GetTxtFromMgr(7949);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);


													if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
													{
														SI32				siEncnantType	= m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager);
														CNewMakeItemDlg*	tools			= (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];

														(tools->NewEnchantTab)->SetChangeEnchanteButton(siEncnantType);
													}

													ShowEnchantElem(m_siSelectedIndex);
													
												}

												// 속성을 가진 인첸트 대상 아이템이 설정되어있다.
												else if(  pclClient->pclCM->CR[1]->siOrgInvEnchantInx > 0 
													&& m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].siUnique > 0  
													//&& m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager) > 0  
													)
												{

													SI32 EnchantType = m_pclInventoryItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager);
													if(EnchantType == ENCHANT_TYPE_FIRE_A || EnchantType == ENCHANT_TYPE_FIRE_D) // 불속성 장비라면
													{
														if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5350) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
														{// 아니면 잘못된 아니템입니다 라고 알려준다
															pExplain = GetTxtFromMgr(5932);
															pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
														}
														else
														{
															ShowEnchantElem(m_siSelectedIndex);
														}
													}
													else if(EnchantType == ENCHANT_TYPE_WATER_A || EnchantType == ENCHANT_TYPE_WATER_D) // 불속성 장비라면
													{
														// 물의 찬여석인지 확인한다.
														if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5355) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370)  && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
														{// 아니면 잘못된 아니템입니다 라고 알려준다
															pExplain = GetTxtFromMgr(5932);
															pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
														}
														else
														{
															ShowEnchantElem(m_siSelectedIndex);
														}
													}
													else if(EnchantType == ENCHANT_TYPE_WIND_A || EnchantType == ENCHANT_TYPE_WIND_D) // 불속성 장비라면
													{
														// 바람 찬여석인지 확인한다.
														if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5360) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370)  && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
														{// 아니면 잘못된 아니템입니다 라고 알려준다
															pExplain = GetTxtFromMgr(5932);
															pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
														}
														else
														{
															ShowEnchantElem(m_siSelectedIndex);
														}
													}
													else if(EnchantType == ENCHANT_TYPE_GROUND_A || EnchantType == ENCHANT_TYPE_GROUND_D) // 불속성 장비라면
													{
														// 땅의 찬여석인지 확인한다.
														if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5365) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370)  
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607)
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
														{// 아니면 잘못된 아니템입니다 라고 알려준다
															pExplain = GetTxtFromMgr(5932);
															pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
														}
														else
														{
															ShowEnchantElem(m_siSelectedIndex);
														}
													}
													else	
													{
														// 속성이 없는 아이템이라면 찬여석 추가가 안된다.
														if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370)  && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606) 
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607) 
															&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
														{
															pExplain = GetTxtFromMgr(5932);
															pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
														}
														else
														{
															ShowEnchantElem(m_siSelectedIndex);
														}
													}
												}

												else if( m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5350) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5355) 
													&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5360) && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5365)
													&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(5370)  && m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13606)
													&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ITEMUNIQUE(13607) 
													&& m_pclInventoryItem[m_siSelectedIndex].siUnique != ENCHANT_USEDAYPLUS_ITEMUNIQUE )
												{
													pExplain = GetTxtFromMgr(5932);
													pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplain);
												}

												else
													ShowEnchantElem(m_siSelectedIndex);
											}	

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
					//	if( true == m_bClickChangeItem )
					//		break;

					// 만약 상점중이라면 아이템 이동 불가

					if ( (true == pclClient->pclCM->CR[1]->bPersonalShopSwitch) || 
						(true == pclClient->pclCM->CR[1]->bPremiumPersonalShop) )
					{
						break ;
					}

					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();

					// 새로운 클릭 앤 드랍
					if(pclClient->IsCountrySwitch(Switch_ClickAndDrop))
					{
						bool bIsSplitMode = false;
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							if( ( pclclient->pclItemManager->GetItemAtb(m_pclInventoryItem[ SelectedIndex ].siUnique) & ITEMATB_OBJECT) == false )
							{
								if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
								{
									pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
								}
								((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetSelectedItemIndex( SelectedIndex );
								((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_SPLITITEM );
								bIsSplitMode = true;
							}
						}
						else if ( SelectedIndex >= 0 && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
						{
							// 클릭 앤 드랍을 시행하기 전에 아이템정보를 체크하고 갱신한다.
							if ( IsItemDataChanged() )	{			LoadItemData();				}
							pclclient->m_pInterfaceMgr->BeginClickAndDrop_Item(&m_pclInventoryItem[ SelectedIndex ], GetDialogControllerID(), nControlID, SelectedIndex);
						}
					}
					else
					{
						if ( SelectedIndex >= 0 && SelectedIndex == m_siSelectedIndex && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
						{	
							m_siSelectedIndex = SelectedIndex;

							bool bIsSplitMode = false;
							if( GetKeyState(VK_CONTROL) < 0 )
							{
								if( ( pclclient->pclItemManager->GetItemAtb(m_pclInventoryItem[ SelectedIndex ].siUnique) & ITEMATB_OBJECT) == false )
								{
									if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
									{
										pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
									}									
									((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_SPLITITEM );
									bIsSplitMode = true;
								}
							}

							if( !bIsSplitMode )
							{
								SI32 GlobalImageIndex;
								SI32 GlobalImageFont;
								if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclInventoryItem[ SelectedIndex ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
								{
									pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ SelectedIndex ].siUnique, ITEMSOUND_SELECT );
									pclclient->m_pInterfaceMgr->ItemBeginDrag( GlobalImageIndex, GlobalImageFont );
								}
							}
						}
						else
						{
							m_siSelectedIndex = -1;
						}
					}
				}
				break;
			case EVENT_LISTVIEW_RBUTTONUP:
				{
					SI16 SelectedIndex = m_pInventoryListView->GetSelectedItemIndex();
					if ( SelectedIndex >= 0 && m_pclInventoryItem[ SelectedIndex ].siUnique > 0 )
					{
						/*
						if ( m_pclInventoryItem[ SelectedIndex ].CanRepair( pclClient->pclItemManager ) )
						{
						//		RepairItem( SelectedIndex );
						}
						else*/

						if ( m_pclInventoryItem[SelectedIndex].IsSealingUp(pclClient->pclItemManager) )
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
			}
		}
		break;
	case NNEWINVENTORY_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// [추가 : 황진성 2008. 1. 29 => 공간 상자. 버튼 하나로 인벤 최대, 최소화 되게]
					if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
					{
						// 기본 상태일때 최대화.
						if( false == m_bMax && false == m_bMini )
						{
							// 최대화를 하는대 가방이나 말이 없어서 최대화 할것이 없다면 최소화 시킨다.
							bool bSet = true;
							if( ( pclclient->pclCM->CR[1]->HaveHorse() == false )  &&
								( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique < 1) && 
								( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique < 1) )
							{
								bSet = false;
							}

							if( bSet ) //최대화
							{
								m_bMax = true;
								ChangeMaxMode( m_bMax );
							}
							else //최소화.
							{
								m_bMax  = false;
								m_bMini = true;
								IsMinimi( m_bMini );  
							}
						}
						// 최소화 상태일 때 기본상태
						else if( false == m_bMax && true == m_bMini )
						{
							m_bMini = false;
							IsMinimi( m_bMini );
						}
						// 최대화 상태일 때 최소 상태로
						else if( true == m_bMax && false == m_bMini )
						{
							m_bMax  = false;
							m_bMini = true;
							IsMinimi( m_bMini );  
						}
					}
					else
					{
						m_bMini = !m_bMini;
						IsMinimi( m_bMini );
					}
				}
				break;
			}
		}
		break;
	case NNEWINVENTORY_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_bMax = !m_bMax;
					ChangeMaxMode( m_bMax );
				}
				break;
			}
		}
		break;
	}

	return;
}


void CNNewInventory::ShowTools( SI32 Destination )
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i = 0;

	//밀봉상태인 도구는 올릴수 없다.
	//부식된 도구는 올릴수 없다.e

	SI32 siItemType =pclClient->pclItemManager->GetItemType(m_pclInventoryItem[ Destination ].siUnique);
	if(siItemType != ITEMTYPE_IRON  && siItemType != ITEMTYPE_DISH && siItemType != ITEMTYPE_PAPER) //paper 는 붓 먹 벼루
		return;
	cltItem* pclitem = &pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[ Destination ];
	if(pclitem->IsSealingUp(pclClient->pclItemManager))	return;//밀봉상태의 아이템 올리지 못하도록 한다.
    	
	for(i = 0; i < 4; i++) //도구는 종류별로만 올라간다.
	{
		if(clMakeItemContainer.cltools[i].siUnique!=0 && clMakeItemContainer.cltools[i].siUnique == m_pclInventoryItem[ Destination ].siUnique )
		{
			return;
		}
	}

	for(i = 0; i < 4; i++) //자리가 비어진 칸에 도구를 올린다.
	{
		if(clMakeItemContainer.cltools[i].siUnique == 0 )
		{
			memcpy(&(clMakeItemContainer.cltools[i]), &m_pclInventoryItem[ Destination ], sizeof(cltItem));
			clMakeItemContainer.m_sitools[i] = Destination;
			pclClient->pclCM->CR[1]->siInvMakeItemIndex[i+25] =Destination;

			cltGameMsgRequest_MakeItem_SelectedIndex clGameMsgRequest_MakeItem_SelectedIndex(i+25,Destination);
			cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMINVINDEX, sizeof(clGameMsgRequest_MakeItem_SelectedIndex), (BYTE*)&clGameMsgRequest_MakeItem_SelectedIndex);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			break;
		}
	}

	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		CNewMakeItemDlg* tools  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
		(tools->NewMakeTab)->ShowTools(clMakeItemContainer.cltools);
	}

	m_pInventoryListView->SetItemDrawAlphaOption(Destination);

	return;
}


void CNNewInventory::ShowMaterials( SI32 Destination )
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 i = 0;

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

	for(i = 0; i < 25; i++)
	{
		SI32 siItemType =pclClient->pclItemManager->GetItemType(m_pclInventoryItem[ Destination ].siUnique);
		if(siItemType == ITEMTYPE_IRON || siItemType == ITEMTYPE_DISH || siItemType == ITEMTYPE_PAPER)
			return;

		if(clMakeItemContainer.clMaterials[i].siUnique!=0 && 
			clMakeItemContainer.clMaterials[i].siUnique == m_pclInventoryItem[ Destination ].siUnique )//자동으로 넣을때는 인벤을 처음부터 검사 하기 때문에 장착창까지 본다
			//그래서 장착창 만큼 인덱스 뒤부터 검사 해야 한다.,
		{
			return;
		}
	}

	//재료를 아이템 제작 창에 올렸을때 위치를 이용해서 구분하다. 유니크는 사용 하지 않는다

	for(i = 0; i < 25; i++) //자리가 비어진 칸에 재료를 올린다.
	{
		if( clMakeItemContainer.clMaterials[i].siUnique == 0 )
		{
			memcpy( &(clMakeItemContainer.clMaterials[i]), &m_pclInventoryItem[ Destination ], sizeof(cltItem));
			clMakeItemContainer.m_siMaterials[i] = Destination;
			pclClient->pclCM->CR[1]->siInvMakeItemIndex[i] =Destination;

			//인덱스랑  제료 유니크를 쏘네 흠..
			cltGameMsgRequest_MakeItem_SelectedIndex clGameMsgRequest_MakeItem_SelectedIndex(i,Destination);//(i = 제작창 인덱스, Destination = 원 소유자 인벤인덱스)
			cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMINVINDEX, sizeof(clGameMsgRequest_MakeItem_SelectedIndex), (BYTE*)&clGameMsgRequest_MakeItem_SelectedIndex);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
			break;
		}
	}

	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
		(NewMakeItem->NewMakeTab)->ShowMaterials(clMakeItemContainer.clMaterials);
	}

	m_pInventoryListView->SetItemDrawAlphaOption(Destination);

	return;
}


void CNNewInventory::ShowEnchantOrg(SI32 Destination) //인챈트창중 인챈트시킬아이템 보여주기
{
	cltClient *pclclient = (cltClient *)pclClient;

	pclclient->pclCM->CR[1]->siOrgInvEnchantInx = Destination;
	SI32 OrgIndex = pclclient->pclCM->CR[1]->siOrgInvEnchantInx;
	SI32 MatIndex = pclclient->pclCM->CR[1]->siMatInvEnchantInx;


	if( MatIndex == Destination) return;

	//밀봉상태인 도구는 올릴수 없다.
	//부식된 도구는 올릴수 없다.

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

	//if(clOrgEnchantItem.siUnique == 0 )
	{
		clMakeItemContainer.clOrgEnchantItem.Set( &m_pclInventoryItem[ Destination] );			
		pclClient->pclCM->CR[1]->siOrgInvEnchantInx =Destination;		
	}
	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		CNewMakeItemDlg* tools  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
		(tools->NewEnchantTab)->ShowOrgEnchantItem(clMakeItemContainer.clOrgEnchantItem);
	}

	m_pInventoryListView->SetItemDrawAlphaOption(Destination);

	return;
}
void CNNewInventory::ShowEnchantMat(SI32 Destination) //인챈트창중 사라질아이템 보여주기
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 OrgIndex = pclclient->pclCM->CR[1]->siOrgInvEnchantInx;
	SI32 MatIndex = pclclient->pclCM->CR[1]->siMatInvEnchantInx;


	if( OrgIndex == Destination) return;



	//밀봉상태인 도구는 올릴수 없다.
	//부식된 도구는 올릴수 없다.

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];


	//if(clMatEnchantItem.siUnique == 0 )
	{
		memcpy(&(clMakeItemContainer.clMatEnchantItem), &m_pclInventoryItem[ Destination ], sizeof(cltItem));		
		pclclient->pclCM->CR[1]->siMatInvEnchantInx = Destination;	
	}


	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		CNewMakeItemDlg* tools  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
		(tools->NewEnchantTab)->ShowMatEnchantItem(clMakeItemContainer.clMatEnchantItem);
	}

	m_pInventoryListView->SetItemDrawAlphaOption(Destination);

	//인첸트 아이템을 세팅하는 퀘스트가 진행중이라면

	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_SETENCHANTITEM;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& para1 == clMakeItemContainer.clMatEnchantItem.siUnique )
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		cltClient* pclclient = (cltClient* )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}


	return;
}

void CNNewInventory::ShowEnchantElem(SI32 Destination) //인챈트창중 속성아이템 보여주기
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i = 0;

	//밀봉상태인 도구는 올릴수 없다.
	//부식된 도구는 올릴수 없다.


	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

	for(i = 0; i < NOW_ENCHANT_NUMBER; i++) //종류별로만 올라간다.
	{		
		if(clMakeItemContainer.clEleEnchantItem[i].siUnique!=0 && 
			clMakeItemContainer.clEleEnchantItem[i].siUnique == m_pclInventoryItem[ Destination ].siUnique )
		{
			return;
		}
	}


	for(i = 0; i < NOW_ENCHANT_NUMBER; i++) //자리가 비어진 칸에 도구를 올린다.
	{
		if(clMakeItemContainer.clEleEnchantItem[i].siUnique == 0 )
		{
			memcpy(&(clMakeItemContainer.clEleEnchantItem[i]), &m_pclInventoryItem[ Destination ], sizeof(cltItem));
			//clEleEnchantItem[i] = Destination;
			pclClient->pclCM->CR[1]->siElementEnchantInx[i] =Destination;					
			break;
		}
	}

	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		CNewMakeItemDlg* tools  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
		(tools->NewEnchantTab)->ShowEleEnchantItem(clMakeItemContainer.clEleEnchantItem);
	}

	m_pInventoryListView->SetItemDrawAlphaOption(Destination);

	return;
}

bool CNNewInventory::CheckBag( SI32 siSource, SI32 siDestination )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)			return false;

	SI32 siBagStart = -1;

	switch(siSource)
	{
	case PERSONITEM_BAG:		siBagStart = PERSONITEM_BAG0;		break;
	case PERSONITEM_BAG_SUB:	siBagStart = PERSONITEM_BAG1;		break;
	default:
		return true;
	}

	if(siBagStart < 0)			return false;

	// 장착하고 있는 가방을 빼서 그 가방안으로 넣을수 없다.
	if( siDestination >= siBagStart && siDestination < siBagStart + MAX_ITEM_PER_BAG )
	{
		TCHAR* pTitle = GetTxtFromMgr(1167);
		TCHAR* pText = GetTxtFromMgr(1168);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return false;
	}

	// 가방에 아이템 있다면 옮기기 불가
	for ( SI16 t = siBagStart; t < siBagStart + MAX_ITEM_PER_BAG; ++t )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
		{
			TCHAR* pTitle = GetTxtFromMgr(1165);
			TCHAR* pText = GetTxtFromMgr(1166);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return false;
		}
	}

	return true;
}


bool CNNewInventory::MoveItem( SI32 siSource, SI32 siDestination )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)					return false;

	// 가방 체크
	if(CheckBag(siSource, siDestination) == false)			return false;

	// 겹칠 수 없는 아이템이면 옮길 수 없다.
	if ( m_pclInventoryItem[ siDestination ].siUnique > 0 )
	{
		if ( m_pclInventoryItem[ siDestination ].CanPile( pclClient->pclItemManager ) == false )
		{
			return false;
		}
	}

	// 장착창에 들어가는 경우 장착 가능한지 확인 한다.
	if( (siDestination >= PERSONITEM_HELMET  && siDestination <= PERSONITEM_RESERVED1) //|| // 캐릭터 장착창
		//(siDestination >= PERSONITEM_SUMMON1EQUIP0 + PERSONITEM_HELMET  && siDestination <= PERSONITEM_SUMMON1EQUIP0 + PERSONITEM_RESERVED1) || // 소환수 장착창
		//(siDestination >= PERSONITEM_SUMMON2EQUIP0 + PERSONITEM_HELMET  && siDestination <= PERSONITEM_SUMMON2EQUIP0 + PERSONITEM_RESERVED1) // 소환수 장착창
		)
	{
		// 밀봉된 아이템은 장착할 수 없다.
		if(m_pclInventoryItem[ siSource ].IsSealingUp(pclClient->pclItemManager))
		{
			TCHAR*pTitle = GetTxtFromMgr(5662);
			TCHAR *pText = GetTxtFromMgr(5663);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return false;		
		}

		// 장착 조건을 검사한다.
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_pclInventoryItem[ siSource ].siUnique);
		if (ref > 0)
		{
			bool bCanEquip = true;
			SI32 siKind = pclClient->pclCM->CR[1]->GetKind();

			bCanEquip = m_pclInventoryItem[ siSource ].IsCorrectPrivateEquip(pclClient->pclItemManager, siKind);
			if (bCanEquip == false)
			{
				TCHAR*pTitle = GetTxtFromMgr(6023);
				TCHAR *pText = GetTxtFromMgr(6024);

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return false;
			}
		}
	}


	SI32 MoveInfo = MAKELONG( siSource, siDestination );
	pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ siSource ].siUnique, ITEMSOUND_SELECT );
	m_pBuffer->Push( INV_MOVE_ITEM, MoveInfo, &m_pclInventoryItem[ siSource ] );

	return true;
}

bool CNNewInventory::ChangeItem( SI32 siSource, SI32 siDestination )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if(pclclient == NULL)					return false;

	// 가방 체크
	if(CheckBag(siSource, siDestination) == false)			return false;

	SI32 MoveInfo = MAKELONG( siSource, siDestination );

	// 장착창에 들어가는 경우 장착 가능한지 확인 한다.
	if( (siDestination >= PERSONITEM_HELMET  && siSource <= PERSONITEM_RESERVED1) //|| // 캐릭터 장착창
		//(siDestination >= PERSONITEM_SUMMON1EQUIP0 + PERSONITEM_HELMET  && siSource <= PERSONITEM_SUMMON1EQUIP0 + PERSONITEM_RESERVED1) || // 소환수 장착창
		//(siDestination >= PERSONITEM_SUMMON2EQUIP0 + PERSONITEM_HELMET  && siSource <= PERSONITEM_SUMMON2EQUIP0 + PERSONITEM_RESERVED1)  // 소환수 장착창
	)
	{
		if(m_pclInventoryItem[ siDestination ].IsSealingUp(pclClient->pclItemManager))
		{
			TCHAR *pTitle = GetTxtFromMgr(5662);
			TCHAR *pText = GetTxtFromMgr(5663);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return false;		
		}
	}

	pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ siSource ].siUnique, ITEMSOUND_SELECT );
	m_pBuffer->Push( INV_CHANGE_ITEM, MoveInfo, &m_pclInventoryItem[ siSource ] );

	return false;
}

void CNNewInventory::MoveItem( SI32 Destination )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_siSelectedIndex == PERSONITEM_BAG )
	{	
		// 가방 내로 가방 옮기기 불가
		if( Destination >= PERSONITEM_BAG0 && Destination < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG )
		{
			TCHAR* pTitle = GetTxtFromMgr(1167);
			TCHAR* pText = GetTxtFromMgr(1168);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		// 가방에 아이템 있다면 옮기기 불가
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG; ++t )
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

	if ( m_siSelectedIndex == PERSONITEM_BAG_SUB )
	{	
		// 가방 내로 가방 옮기기 불가
		if( Destination >= PERSONITEM_BAG1 && Destination < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
		{
			TCHAR* pTitle = GetTxtFromMgr(1167);
			TCHAR* pText = GetTxtFromMgr(1168);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		// 가방에 아이템 있다면 옮기기 불가
		for ( SI16 t = PERSONITEM_BAG1; t < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG; ++t )
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

	cltItem clToItem;

	// 사람일때
	if ( SELECTED_HUMAN == m_siCurrentSummonIndex )
	{
		clToItem.Set( &m_pclInventoryItem[ Destination ] );
	}
	// 소환수 일때
	else
	{
		clToItem.Set( &pclClient->pclCM->CR[1]->pclCI->clSummonInfo.clItemInfo[m_siCurrentSummonIndex].clEquipItem[Destination] );
	}

	SI32 MoveInfo = MAKELONG( m_siSelectedIndex, Destination );

	if ( clToItem.siUnique != 0 )
	{
		if ( clToItem.CanPile( pclClient->pclItemManager ) == false )
		{
			return;
		}
	}

	if((Destination >= 0  && Destination <= 14 ) || Destination == 82 || Destination == 97 )
	{
		if(m_pclInventoryItem[ m_siSelectedIndex ].IsSealingUp(pclClient->pclItemManager))
		{
			TCHAR*pTitle = GetTxtFromMgr(5662);
			TCHAR *pText = GetTxtFromMgr(5663);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return;		
		}

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_pclInventoryItem[ m_siSelectedIndex ].siUnique);

		if (ref > 0)
		{
			bool bCanEquip = true;
			SI32 siKind = pclClient->pclCM->CR[1]->GetKind();

			bCanEquip = m_pclInventoryItem[ m_siSelectedIndex ].IsCorrectPrivateEquip(pclClient->pclItemManager, siKind);

			if (bCanEquip == false)
			{
				TCHAR*pTitle = GetTxtFromMgr(6023);
				TCHAR *pText = GetTxtFromMgr(6024);

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;		
			}

		}

	}

	pclClient->pclItemManager->PlaySound( m_pclInventoryItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );
	m_pBuffer->Push( INV_MOVE_ITEM, MoveInfo, &m_pclInventoryItem[ m_siSelectedIndex ] );

	return;
}

void CNNewInventory::RepairItem( SI32 SelectedInventoryItemIndex )
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

		return;
	}
}

void CNNewInventory::CancellationSealingUp(SI32 siIndex)
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

		return;
	}
}

void CNNewInventory::UseNolimitTicket(SI32 ItemUnique, SI32 Type)
{
	cltDate MaxWarpDate;//무한 티켓 날자 셋팅 ㅡㅡ;이렇게 안하는 방법은없을까.
	MaxWarpDate.uiYear = 255;
	MaxWarpDate.uiMonth = 12;
	MaxWarpDate.uiDay = 30;
	SI32 MAX_GAME_DATE = MaxWarpDate.GetDateVary();
	
	//필요 없는 조건으로 보여서 삭제	
	//현제 기간제 워프 티켓을 사용중인 상태에서 무한워프티켓을 사용하려 했을때
	if (pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp == true &&
		ItemUnique == ITEMUNIQUE(24102)) {
			//워프 티켓을 사용하는 기간 도중에는 무한 워프 티켓 사용불가능 메세지를 날리고 리턴 시킨다.
			cltClient *pclclient = ( cltClient * )pclClient;
			TCHAR * pTitle = GetTxtFromMgr(7863);
			TCHAR * pText = GetTxtFromMgr(7864);
			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, pText , MSGBOX_TYPE_OK, 5 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( ItemUnique );
			memcpy( TempBuffer, &ItemUnique, sizeof( ItemUnique ) );	

			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
	//무한워프 티켓을 사용하였고 또 다시 사용하고 있을경우
	else if (Type ==NOLIMIT_TICKET &&
		pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp == true &&
		pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary == MAX_GAME_DATE){
			cltClient *pclclient = ( cltClient * )pclClient;
			TCHAR * pTitle = GetTxtFromMgr(7863);
			TCHAR * pText = GetTxtFromMgr(7865);
			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, pText , MSGBOX_TYPE_OK, 5 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( ItemUnique );
			memcpy( TempBuffer, &ItemUnique, sizeof( ItemUnique ) );	

			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
	//이미 다른 기간제 워프 티켓을 사용하고 있고 무한워프 티켓을 사용하고 있을 경우
	/*
	else if (Type ==NOLIMIT_TICKET &&
		pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp == true&&
		pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary < MAX_GAME_DATE){
			cltClient *pclclient = ( cltClient * )pclClient;
			TCHAR * pTitle = GetTxtFromMgr(7863);
			TCHAR * pText = GetTxtFromMgr(7864);
			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, pText , MSGBOX_TYPE_OK, 5 );
			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( ItemUnique );
			memcpy( TempBuffer, &ItemUnique, sizeof( ItemUnique ) );	

			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
	*/
	// 그외의 경우에 워프 티켓을 사용한다.(날짜가 중복 되므로)
	else{
		TCHAR* pTitle = NULL;
		switch( Type ) {
		case NOLIMIT_TICKET :{
				pTitle = GetTxtFromMgr(4381);
			}break;
		case MARKET_TICKET :{
				pTitle = GetTxtFromMgr(10157);
			}break;
		case SPACEBOX_TICKET:{/// 공간 창고 박스
				pTitle = GetTxtFromMgr(7914);
			}break;
		case SUMMON_SPACE_TICKET:
			{
				// 소환수 소지 갯수 확장
				pTitle = GetTxtFromMgr(8785);
			}break;
		}		
		TCHAR* pText = GetTxtFromMgr(6919);
		TCHAR szBuffer[256];
		szBuffer[0] = 0;
		cltClient *pclclient = ( cltClient * )pclClient;

		const TCHAR* pItemName = pclclient->pclItemManager->GetName(ItemUnique);

		if( pItemName == NULL ) return;

		StringCchPrintf(szBuffer , 256 , pText , pItemName );

		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, szBuffer , MSGBOX_TYPE_YESNO, 4 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( ItemUnique );
		memcpy( TempBuffer, &ItemUnique, sizeof( ItemUnique ) );	

		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
}


void CNNewInventory::UseWarpTargetWorld(SI32 siItemUnique )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	const TCHAR* pItemName = pclclient->pclItemManager->GetName(siItemUnique);	if( pItemName == NULL ) return;

	TCHAR* pTitle =GetTxtFromMgr(40379);
	NTCHARString256 szExplain(GetTxtFromMgr(40380));	// 티켓을 사용 하시겠습니까? 

	switch( siItemUnique ) 
	{
	case ITEMUNIQUE(24128):
		{			
			szExplain.Replace( "#itemname#" , pItemName )	;
		}
		break;
	}
	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, szExplain , MSGBOX_TYPE_YESNO, 6 );
	
	BYTE TempBuffer[ 128 ];
	SI16 Size = sizeof( siItemUnique );
	memcpy( TempBuffer, &siItemUnique, sizeof( siItemUnique ) );	

	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
}
// [성웅] 레벨 보상 상자에서 아이템을 준다 
void CNNewInventory::UseLevelLimitBoxOpen(SI32 siItemUnique )
{
	if ( pclClient->IsCountrySwitch(Switch_LevelLimitBox) == false)		return	;
	cltClient *pclclient = ( cltClient * )pclClient;

	const TCHAR* pItemName = pclclient->pclItemManager->GetName(siItemUnique);	if( pItemName == NULL ) return;

	TCHAR* pTitle =GetTxtFromMgr(40379);
	NTCHARString256 szExplain(GetTxtFromMgr(40380));
	szExplain.Replace( "#itemname#" , pItemName )	;

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, szExplain , MSGBOX_TYPE_YESNO, 7 );

	BYTE TempBuffer[ 128 ];
	SI16 Size = sizeof( siItemUnique );
	memcpy( TempBuffer, &siItemUnique, sizeof( siItemUnique ) );	

	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
}
void CNNewInventory::UseItem( SI16 Pos )
{

	cltClient* pclclient = (cltClient*)pclClient;
	if ( m_pclInventoryItem[ Pos ].siUnique == 0 )
	{
		return;
	}

	TCHAR*		pTitle	= GetTxtFromMgr(1143);
	TCHAR*		pText	= NULL;
	
	if ( m_pclInventoryItem[ Pos ].GetType( pclClient->pclItemManager ) == ITEMTYPE_BOOK )
	{
		if( pclClient->pclItemManager->IsItemInfoAtb( m_pclInventoryItem[ Pos ].siUnique, ITEMINFOATB_SKILLBOOK ) )
		{
			if( false == pclClient->IsCountrySwitch( Switch_SkillBook ) )	return;

			// 스킬북 보조 마법책.
			if( pclclient->pclItemManager->pclSkillBookManager->GetSkillBookParentMagicFromItemUnique( m_pclInventoryItem[ Pos ].siUnique ) )
			{
				pText = GetTxtFromMgr(30530);
			}
			// 스킬북 마법책.
			else
			{
				pText = GetTxtFromMgr(30517);
			}
		}
		else
		{
			pText = GetTxtFromMgr(1142); 
		}

		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( Pos ) + sizeof( cltItem );
		memcpy( TempBuffer, &Pos, sizeof( Pos ) );
		memcpy( &TempBuffer[ sizeof( Pos ) ], &m_pclInventoryItem[ Pos ], sizeof( cltItem ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
	// 박스 아이템의 경우 여기에서 체크 후 사용가능한지 판단
	else if ( (pclClient->m_pclBoxItemMgr) && (TRUE == pclClient->m_pclBoxItemMgr->IsNormalBoxItem(m_pclInventoryItem[ Pos ].siUnique)) )
	{
		if ( !pclclient->m_pDialog[ NTREASUREBOX_DLG ] )
		{
			pclclient->CreateInterface(NTREASUREBOX_DLG);
		}

		if( ((CNTreasureBoxDlg*)pclclient->m_pDialog[NTREASUREBOX_DLG])->CanDraw() )
		{
			((CNTreasureBoxDlg*)pclclient->m_pDialog[NTREASUREBOX_DLG])->SetTreasureBox( Pos, &m_pclInventoryItem[ Pos ], m_pBuffer );
		}
	}
// [성웅] 레벨 보상 상자에서 아이템을 준다 
	else if( pclClient->IsCountrySwitch(Switch_LevelLimitBox) 
		&& pclClient->pclItemManager->pclRewardItemLimitLevelForChina != NULL 
		&& pclClient->pclItemManager->pclRewardItemLimitLevelForChina->IsRewardBox_Unique( m_pclInventoryItem[ Pos ].siUnique ) ) 
	{
		UseLevelLimitBoxOpen(m_pclInventoryItem[ Pos ].siUnique)	;
	}
	else
	{	
		// [영훈] 아이템 사용 if -> switch로 변경
		switch ( m_pclInventoryItem[ Pos ].siUnique )
		{
			//----------------------------------------------------------------------------------------------------
			// 트레져 박스
			case ITEMUNIQUE(7975):	// 중국책상자
			case ITEMUNIQUE(13002):	// 양말
			case ITEMUNIQUE(13005):	// 황금알
			case ITEMUNIQUE(13015):	// 빨간장미
			case ITEMUNIQUE(13021):	// 흰색장미
			case ITEMUNIQUE(13028):	// 라쿤알
			case ITEMUNIQUE(13300):	// 제갈공명
			case ITEMUNIQUE(13301):	// 틴틴상자
			case ITEMUNIQUE(13302):	// 키메라 상자
			case ITEMUNIQUE(13303):	// 행운의 상자
			case ITEMUNIQUE(13304):	// 책상자
			case ITEMUNIQUE(13305):	// 냐옹이 상자
			case ITEMUNIQUE(13306):	// 메이드상자
			case ITEMUNIQUE(13307):	// 근력반지
			case ITEMUNIQUE(13308):	// 순발력반지
			case ITEMUNIQUE(13309):	// 생명력반지
			case ITEMUNIQUE(13310):	// 지력반지
			case ITEMUNIQUE(13311):	// 손재주반지
			case ITEMUNIQUE(13312):	// 행운반지
			case ITEMUNIQUE(13313):	// 그리스복상자
			case ITEMUNIQUE(13314):	// 보물상자
			case ITEMUNIQUE(13315):	// 재료상자
			case ITEMUNIQUE(13316):	// 신년행운상자
			//case ITEMUNIQUE(13317):	// 닌자 상자
			case ITEMUNIQUE(13318):	// 초코상자
			case ITEMUNIQUE(13319):	// 캔디상자
			case ITEMUNIQUE(13320):	// 해적상자
			case ITEMUNIQUE(13321):	// 금빛상자
			case ITEMUNIQUE(13322):	// 강아지 상자
			case ITEMUNIQUE(13323):	// 루돌프상자
			case ITEMUNIQUE(13324):	// 대만상자
			case ITEMUNIQUE(13326):	// 농부상자
			case ITEMUNIQUE(13327):	// 광부상자
			case ITEMUNIQUE(13328):	// 상어
			case ITEMUNIQUE(13329):	// 신년상자(럭키상자)
			//case ITEMUNIQUE(13330):	// 고양이 변신 상자  (가챠)
			case ITEMUNIQUE(13331):	// // [성웅] 2차복주머니 2008.06.30 ( 가챠)
			case ITEMUNIQUE(13333):	// 라쿤 가죽 지갑.
			case ITEMUNIQUE(13332):	// 호박의복상자
			case ITEMUNIQUE(13335):	// [진성] 루미너리(USA) 1주년 기념 과금 상자  => 2008-9-25
			case ITEMUNIQUE(13336):	// 이불 상자.
			case ITEMUNIQUE(13337):	// 라쿤 보물 상자.
			//case ITEMUNIQUE(13338):	// Christmas Spirit BOX
			case ITEMUNIQUE(13342):	// 군주상자
			case ITEMUNIQUE(13341):	// 폭주족 의복 상자
			//case ITEMUNIQUE(13343): // 투사 스피릿 상자 (검사스피릿,거너스피릿)
			case ITEMUNIQUE(13344): // 아바타 상자
			case ITEMUNIQUE(13346):	// 결혼예복 상자
			case ITEMUNIQUE(13347):	// 낡은 쇠뭉치
			case ITEMUNIQUE(13348): // 결혼식 예복 상자
			case ITEMUNIQUE(13350): // 잠자는 냐옹이 상자
			case ITEMUNIQUE(13351): // 중국 pc방 상자
			//case ITEMUNIQUE(13352):	// 요정 스피릿 상자
			case ITEMUNIQUE(13349):	// 중국 추천인 상자	
			//case ITEMUNIQUE(13357):	// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치
			case ITEMUNIQUE(13358):	// 2010신년이벤트 - 대길시 얻을 수 있는 상자.
			case ITEMUNIQUE(13063):	// 2010신년이벤트 - 포춘쿠키
			case ITEMUNIQUE(13359):	// [춘기] 3차 복주머니 (2009.11.26)
			case ITEMUNIQUE(13360):	// EVENT ITEM 선물 상자
			case ITEMUNIQUE(13361):	// 제조책상자
			case ITEMUNIQUE(13362):	// 기술책상자
			case ITEMUNIQUE(13363):	// 생산책상자
			case ITEMUNIQUE(13364):	// 악마 의복 상자.
				{
					if ( !pclclient->m_pDialog[ NTREASUREBOX_DLG ] )
					{
						pclclient->CreateInterface(NTREASUREBOX_DLG);
					}

					if( ((CNTreasureBoxDlg*)pclclient->m_pDialog[NTREASUREBOX_DLG])->CanDraw() )
					{
						((CNTreasureBoxDlg*)pclclient->m_pDialog[NTREASUREBOX_DLG])->SetTreasureBox( Pos, &m_pclInventoryItem[ Pos ], m_pBuffer );
					}
				}
				break;

			//----------------------------------------------------------------------------------------------------
			// 대박상자
			case ITEMUNIQUE(13325):
				{
					if ( !pclclient->m_pDialog[ NOPENBIGITEMBOX_DLG ] )
					{
						pclclient->CreateInterface(NOPENBIGITEMBOX_DLG);
					}
					((CNOpenBigItemBoxDlg*)pclclient->m_pDialog[NOPENBIGITEMBOX_DLG])->SetBigItemBox( Pos, &m_pclInventoryItem[ Pos ] );
				}
				break;

			//----------------------------------------------------------------------------------------------------
			// 신년상자
			case ITEMUNIQUE(13340):
				{
					// 창이 열려 있으면 닫는다
					if ( pclclient->m_pDialog[NEWSELECTBOX_DLG] )
					{
						pclclient->DestroyInterface( pclclient->m_pDialog[NEWSELECTBOX_DLG] );
					}

					pclclient->CreateInterface(NEWSELECTBOX_DLG);

					((CNNewSelectBoxDlg*)pclclient->m_pDialog[NEWSELECTBOX_DLG])->OpenNewSelectBox( m_pclInventoryItem[ Pos ].siUnique );
					
				}
				break;

			//----------------------------------------------------------------------------------------------------
			case ITEMUNIQUE(24114):
				{
					if ( !pclclient->m_pDialog[ CLTNCHARNAMECHANGE_DLG ] )
					{
						pclclient->CreateInterface(CLTNCHARNAMECHANGE_DLG);
					}
				}
				break;

			//----------------------------------------------------------------------------------------------------
			// 워프티켓
			case ITEMUNIQUE(24094):
			case ITEMUNIQUE(24096):
			case ITEMUNIQUE(24097):
			case ITEMUNIQUE(24098):
			case ITEMUNIQUE(24102):	// 무한 워프 티켓
			case ITEMUNIQUE(24126):	// 순간이동정기권7일(EVENT)
			case ITEMUNIQUE(24129): // Goonzu Enent Item - Item Unique 24094와 같은 기능.
				{
					UseNolimitTicket(m_pclInventoryItem[ Pos ].siUnique, NOLIMIT_TICKET );
				}
				break;

			//----------------------------------------------------------------------------------------------------
			case ITEMUNIQUE(24099):
				{
					UseNolimitTicket(m_pclInventoryItem[ Pos ].siUnique, MARKET_TICKET );
				}
				break;

			//----------------------------------------------------------------------------------------------------
			// 공간상자
			case ITEMUNIQUE(24111):	// 1일 공간 상자사용티켓
			case ITEMUNIQUE(24112):	// 7일 공간 상자사용티켓
			case ITEMUNIQUE(24113):	// 30일 공간 상자사용티켓
			case ITEMUNIQUE(24127):	// 30일 공간 상자사용티켓(EVENT)
			case ITEMUNIQUE(24131):	// 1일 공간 상자 사용티켓 GE
				{
					UseNolimitTicket(m_pclInventoryItem[ Pos ].siUnique, SPACEBOX_TICKET );
				}
				break;
			case ITEMUNIQUE(24128):	// 라쿤월드로 이동
				{
					//감옥이면 사용불가능
					if ( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					{
						TCHAR* pTitle = GetTxtFromMgr(836);
						TCHAR* pText = GetTxtFromMgr(6033);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText);
						return ;
					}
					else if( pclclient->pclCM->CR[1]->bPersonalShopSwitch == true || 
						pclclient->pclCM->CR[1]->bPremiumPersonalShop == true )
					{
						TCHAR* pTitle = GetTxtFromMgr(836);
						TCHAR* pText = GetTxtFromMgr(8283);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText);
						return ;
					}
					UseWarpTargetWorld(m_pclInventoryItem[ Pos ].siUnique)	;

				}break	;

			//----------------------------------------------------------------------------------------------------
			// 갓차
			case GACHA_GOLDKEY_ITEMUNIQUE:
			case GACHA_SILVERKEY_ITEMUNIQUE:
			case GACHA_BRONZEKEY_ITEMUNIQUE:
			case GACHA_BRONZEKEY_EVENT_ITEMUNIQUE:
			case GACHA_KEYPIECE_ITEMUNIQUE:
				{
					if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
					{
						cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
						if ( (NULL == pclchar) || (false == pclchar->IsBlankInv(1, INVMODE_ALL)) )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
							return; 
						}

						pclclient->CreateInterface( NCARRYOVERGACHA_DLG );
					}
					else if( pclClient->IsCountrySwitch(Switch_Gacha_Reform) )
					{
						//[진성] 인벤토리가 다 차있으면 가차 창을 열지 않는다.
						cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
						if(false == pclchar->IsBlankInv(1, INVMODE_ALL) )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
							return; 
						}
						else
						{
							pclclient->CreateInterface(NGACHANEW_DLG);
						}
					}
					else
					{
						pclclient->CreateInterface(NGACHA_DLG);
					}
				}
				break;

			// [영훈] 이월 가챠 만능열쇠
			case CARRYOVER_GOLDKEY_ITEMUNIQUE:
			case CARRYOVER_SILVERKEY_ITEMUNIQUE:
			case CARRYOVER_BRONZEKEY_ITEMUNIQUE:
				{
					if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
					{
						cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
						if ( (NULL == pclchar) || (false == pclchar->IsBlankInv(1, INVMODE_ALL)) )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
							return; 
						}

						pclclient->CreateInterface( NCARRYOVERGACHA_DLG );
					}
				}
				break;

			//----------------------------------------------------------------------------------------------------
			//KHY - 1104 -  갓차2 -라쿤 보물 상자. 
			case GACHA2_RACCOONKEY_ITEMUNIQUE:
				{
						//[진성] 인벤토리가 다 차있으면 가차 창을 열지 않는다.
						cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
						if(false == pclchar->IsBlankInv(1, INVMODE_ALL) )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
							return; 
						}
						else
						{
							pclclient->CreateInterface(NGACHA2_DLG);
						}
				}
				break;

			//----------------------------------------------------------------------------------------------------
			// 선거시스템 : 유세창 열기			
			case ITEMUNIQUE(24080):
				{
					if ( false == pclclient->bVoteCampaign )
					{
						TCHAR	*pTitle	= GetTxtFromMgr(5614);
						TCHAR	*pText	= GetTxtFromMgr(8797);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
					else
					{
						pclclient->CreateInterface( ELECTIONSTUMP_DLG );
					}
				}
				break;
			
			//----------------------------------------------------------------------------------------------------
			case ITEMUNIQUE(24117):	// 소환수 소지 갯수 추가.
				{
					if( pclclient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate > 0 )
					{
						TCHAR	*pTitle	= GetTxtFromMgr(512);
						TCHAR	*pText	= GetTxtFromMgr(8769);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}
					else
					{
						UseNolimitTicket(m_pclInventoryItem[ Pos ].siUnique, SUMMON_SPACE_TICKET );
					}

					
				}
				break;
			//[진성] 엘릭서 사용 창.
			case ITEMUNIQUE(13634):
				{
					if( pclClient->IsCountrySwitch( Switch_Elixir ) )
					{
						cltCharClient* pclChar = pclClient->pclCM->GetCharClient(1);
						//if(	pclChar && false == pclChar->pcltElixir->IsElixir() )
						{
							pclclient->CreateInterface( NELIXIR_USE_DLG );

							((NElixirUseDlg*)pclclient->m_pDialog[NELIXIR_USE_DLG])->SetItemUnique( ITEMUNIQUE(13634) );
						}
						//else
						{
							//pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(9759) ); 
						}
					}
				}
				break;

			case ITEMUNIQUE(8123):
				{
					if( pclClient->IsCountrySwitch( Switch_NewDurability) )
					{
						pclclient->CreateInterface( NNEWITEMMAKE_DLG );
						CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
						NItemRepairDlg* pclItemRepairDlg = NULL;
						if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;

						if( pclItemRepairDlg )
						{
							pclItemRepairDlg->SetMode( 1 );
						}
						pclNewMakeItemDlg->ShowRepairTab();
					}
				}
				break;

			case ITEMUNIQUE(13356):
			case ITEMUNIQUE(13357):
			case ITEMUNIQUE(13338):
			case ITEMUNIQUE(13343):
			case ITEMUNIQUE(13352):
			case ITEMUNIQUE(13317):
			case ITEMUNIQUE(13330):
				{
					// 할로윈 스피릿 선택 dlg
					cltClient* pClient = (cltClient*)pclClient;
					if(pClient != NULL)
					{
						if(pClient->m_pDialog[NHALLOWEENSPIRITBOX_DLG] == NULL)
						{
							pClient->CreateInterface(NHALLOWEENSPIRITBOX_DLG);

							((NHalloweenSpiritBoxDlg*)pclclient->m_pDialog[NHALLOWEENSPIRITBOX_DLG])->SetSpritItmeUnique( m_pclInventoryItem[ Pos ].siUnique );
						}
						else
						{
							// 만들어져 있으면 그냥 셋
							((NHalloweenSpiritBoxDlg*)pclclient->m_pDialog[NHALLOWEENSPIRITBOX_DLG])->SetSpritItmeUnique( m_pclInventoryItem[ Pos ].siUnique );
						}
					}
				}
				break;
			//[진성] 발렌타인데이. 달콤한 초콜릿
			case ITEMUNIQUE(13661):
				{
					pclclient->CreateInterface( VALENTINEDAY_CONFESSION_QUESTION_DLG );
				}
				break;
			//----------------------------------------------------------------------------------------------------
			default:
				{
					m_pBuffer->Push( INV_USE_ITEM, Pos, &m_pclInventoryItem[ Pos ] );
				}
		}
	}
	m_siSelectedIndex = -1;

	return;
}

void CNNewInventory::DropItem( SI16 Pos )
{
	if ( m_pclInventoryItem[ Pos ].siUnique == 0 )
	{
		return;
	}
	if ( pclClient->IsCountrySwitch(Switch_DestroyDropItem) ){
		SI16 siItemnum = m_pclInventoryItem[ Pos ].GetItemNum();// 아이템 갯수
		if ( siItemnum <= 0 ) return;
		SI32 siItemUnique = m_pclInventoryItem[ Pos ].siUnique;// 아이템 유니크
		if ( siItemUnique <= 0 ) return;
		TCHAR* tcItemname = ( TCHAR* )pclClient->pclItemManager->GetName( siItemUnique);// 아이템 이름
		if ( tcItemname == NULL ) return;

		TCHAR* pTitle = GetTxtFromMgr(5261);
		NTCHARString256	kText(GetTxtFromMgr(8571));
		kText.Replace(TEXT("#itemname#"),tcItemname );
		kText.Replace(TEXT("#num#"),SI16ToString(siItemnum) );
		
		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle,kText, MSGBOX_TYPE_YESNO, 2 );
		BYTE TempBuffer[ 256 ];
		SI16 Size = sizeof( Pos ) + sizeof( cltItem );
		memcpy( TempBuffer, &Pos, sizeof( Pos ) );
		memcpy( &TempBuffer[ sizeof( Pos ) ], &m_pclInventoryItem[ Pos ], sizeof( cltItem ) );
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		m_siSelectedIndex = -1;		
	}
	else{
		TCHAR* pTitle = GetTxtFromMgr(5261);
		TCHAR* pText;
		//cyj 귀속 아이템일때는 파괴하도록 메세지 수정
		if (m_pclInventoryItem[ Pos ].IsBelonging(pclClient->pclItemManager))
		{
			pText = GetTxtFromMgr(5899);
		}
		else
		{
			pText = GetTxtFromMgr(5262);
		}
		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 2 );

		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( Pos ) + sizeof( cltItem );
		memcpy( TempBuffer, &Pos, sizeof( Pos ) );
		memcpy( &TempBuffer[ sizeof( Pos ) ], &m_pclInventoryItem[ Pos ], sizeof( cltItem ) );

		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		m_siSelectedIndex = -1;
	}	
}

void CNNewInventory::DropItem_New( cltItem* pclItem, SI16 Pos )
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 요청한 아이템과 현재 아이템을 비교해서 틀리면 아무것도 안한다.
	if( pclItem->IsSame(&m_pclInventoryItem[ Pos ], true) == false )			return;

	/*if ( pclClient->IsCountrySwitch(Switch_DestroyDropItem) ){
		DropItem( Pos );
	}
	else{*/
		if( ( pclclient->pclItemManager->GetItemAtb(m_pclInventoryItem[ Pos ].siUnique) & ITEMATB_OBJECT) == false )
		{
			if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
			{
				pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
			}
			((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetSelectedItemIndex( Pos );
			((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_DROPITEM );
		}
		else
		{
			DropItem( Pos );
		}
	//}
}

// WEAPON1 과 WEAPON2의 위치를 교환한다.
void CNNewInventory::ExchangeWeapon()
{
}

void CNNewInventory::ExchangeClothes()
{
}

void CNNewInventory::Show()
{	
	cltClient *pclclient = (cltClient *)pclClient;

	CDialogController::Show( true );
	m_bMax = false;
	ChangeMaxMode( m_bMax );

	CheckWrongSelectedInven();
	LoadItemData();

	return;
}

void CNNewInventory::Hide()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 모든 인벤을 숨겨라!!
	((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG])->CDialogController::Show( false );
	((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG])->CDialogController::Show( false );
	((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG])->CDialogController::Show( false );
	((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG])->CDialogController::Show( false );

	return;
}

void CNNewInventory::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI16 dialogID = GetDialogControllerID();

	if(IsShow())
	{
		// 맥코이 퀘스트 수행시 소지창이 열려있어도 클리어 단계로 넘어간다. by LEEKH 2008.08.10
		// 부하 방지를 위해 일정간격으로 수행한다.
		static SI32 siUpdateTime = 0;
		if(GetTickCount() - siUpdateTime > 500)
		{
			siUpdateTime = GetTickCount();
		}

		if ( pclclient->siServiceArea == ConstServiceArea_China )
		{
			ShowSlotNum();
		}
		else
		{
			ShowInvenWeight();
		}
	}

	if ( IsItemDataChanged()  )
	{
		LoadItemData();
		if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
		{
			UpdateMakeItemData();
		}
	}

	CheckCharStatusChanged();
	

	return;
}


void CNNewInventory::UpdateMakeItemData()
{
	cltClient *pclclient = (cltClient *)pclClient;

	for(SI32 i =0; i <25 ; i++)
	{	

		SI32 index = clMakeItemContainer.m_siMaterials[i];
		//아이템이 자리가 이동된 경우
		if( m_pclInventoryItem[index].siUnique  !=  clMakeItemContainer.clMaterials[i].siUnique )
		{
			clMakeItemContainer.clMaterials[i].Init();
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
				(NewMakeItem->NewMakeTab)->ShowMaterials(clMakeItemContainer.clMaterials);
			}
		}
		//제조되어 아이템 갯수가 줄어든경우 
		else if( m_pclInventoryItem[index].siItemNum !=  clMakeItemContainer.clMaterials[i].siItemNum  && m_pclInventoryItem[index].siUnique  ==  clMakeItemContainer.clMaterials[i].siUnique)
		{
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];				
				clMakeItemContainer.clMaterials[i].siItemNum= m_pclInventoryItem[index].siItemNum;
				(NewMakeItem->NewMakeTab)->ShowMaterials(clMakeItemContainer.clMaterials);
			}
		}			
	}


	for(SI32 i =0; i <4  ; i++)
	{
		SI32 index = clMakeItemContainer.m_sitools[i];
		if( m_pclInventoryItem[index].siUnique  != clMakeItemContainer.cltools[i].siUnique )
		{
			clMakeItemContainer.cltools[i].Init();
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
				(NewMakeItem->NewMakeTab)->ShowTools(clMakeItemContainer.cltools);
			}

		}
		else if( m_pclInventoryItem[index].siItemNum ==  clMakeItemContainer.cltools[i].siItemNum  && m_pclInventoryItem[index].siUnique  ==  clMakeItemContainer.cltools[i].siUnique)
		{
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];				
				clMakeItemContainer.cltools[i].siItemNum= m_pclInventoryItem[index].siItemNum;
				(NewMakeItem->NewMakeTab)->ShowTools(clMakeItemContainer.cltools);
			}
		}		
	}

	for(SI32 i =0; i <NOW_ENCHANT_NUMBER ; i++) //인챈트 관련
	{
		SI32 index = pclclient->pclCM->CR[1]->siElementEnchantInx[i];
		if( m_pclInventoryItem[index].siUnique  !=  clMakeItemContainer.clEleEnchantItem[i].siUnique )
		{
			clMakeItemContainer.clEleEnchantItem[i].Init();
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
				clMakeItemContainer.clEleEnchantItem[i].siItemNum = m_pclInventoryItem[index].siItemNum;
				(NewMakeItem->NewEnchantTab)->ShowEleEnchantItem(clMakeItemContainer.clEleEnchantItem);
			}

		}
		else if( m_pclInventoryItem[index].siItemNum !=  clMakeItemContainer.clEleEnchantItem[i].siItemNum  && m_pclInventoryItem[index].siUnique  ==  clMakeItemContainer.clEleEnchantItem[i].siUnique)
		{
			if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
			{
				CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];				
				clMakeItemContainer.clEleEnchantItem[i].siItemNum= m_pclInventoryItem[index].siItemNum;
				(NewMakeItem->NewEnchantTab)->ShowEleEnchantItem(clMakeItemContainer.clEleEnchantItem);
			}
		}		
	}



	SI32 index = pclclient->pclCM->CR[1]->siOrgInvEnchantInx;
	if( index!=0 && m_pclInventoryItem[index].siUnique  !=  clMakeItemContainer.clOrgEnchantItem.siUnique )
	{
		clMakeItemContainer.clOrgEnchantItem.Init();
		if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
		{
			CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
			(NewMakeItem->NewEnchantTab)->ShowOrgEnchantItem( m_pclInventoryItem[index]);
		}

	}
	else if(index!=0 && m_pclInventoryItem[index].siItemNum ==  clMakeItemContainer.clOrgEnchantItem.siItemNum  && m_pclInventoryItem[index].siUnique  ==  clMakeItemContainer.clOrgEnchantItem.siUnique)
	{
		if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
		{
			CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];				
			clMakeItemContainer.clOrgEnchantItem.siItemNum= m_pclInventoryItem[index].siItemNum;
			(NewMakeItem->NewEnchantTab)->ShowOrgEnchantItem( m_pclInventoryItem[index]);
		}
	}



	index = pclclient->pclCM->CR[1]->siMatInvEnchantInx;
	if( m_pclInventoryItem[index].siUnique  !=  clMakeItemContainer.clMatEnchantItem.siUnique )
	{
		clMakeItemContainer.clMatEnchantItem.Init();
		if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
		{
			CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
			(NewMakeItem->NewEnchantTab)->ShowMatEnchantItem(clMakeItemContainer.clMatEnchantItem);
		}

	}
	else if( m_pclInventoryItem[index].siItemNum ==  clMakeItemContainer.clMatEnchantItem.siItemNum  && m_pclInventoryItem[index].siUnique  ==  clMakeItemContainer.clMatEnchantItem.siUnique)
	{
		if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
		{
			CNewMakeItemDlg* NewMakeItem  = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];				
			clMakeItemContainer.clMatEnchantItem.siItemNum= m_pclInventoryItem[index].siItemNum;
			(NewMakeItem->NewEnchantTab)->ShowMatEnchantItem(clMakeItemContainer.clMatEnchantItem);
		}
	}	

	return;
}

bool CNNewInventory::IsItemDataChanged()
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

void CNNewInventory::LoadItemData()
{
	SI16 StartPos = 0;
	SI32 i = 0;
	SI32 id = 1;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	cltClient* pclclient = (cltClient*)pclClient;

	// 어떤 인벤인지..
	SI16 contollerid = GetDialogControllerID();
	switch( contollerid )
	{
	case NNEWINVENTORY_DLG : 
		StartPos = PERSONITEM_INV0;
		break;
	case NNEWINVENTORY_HORSE_DLG : 
		StartPos = PERSONITEM_HORSE0;
		break;
	case NNEWINVENTORY_BAG0_DLG : 
		StartPos = PERSONITEM_BAG0;
		break;
	case NNEWINVENTORY_BAG1_DLG : 
		StartPos = PERSONITEM_BAG1;
		break;
	}

	//	StartPos = PERSONITEM_INV0 + ( m_siCurrentInventoryIndex * MAX_ITEM_PER_INVENTORY );
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
					//SI32 siItemStatus = 0 ;					

					pclClient->pclItemManager->GetExplain( &m_pclInventoryItem[ i ], ToolTipMsg,true,false,true, pclClient->pclCM->CR[1]->GetKind() );//,&siItemStatus);
					SI32 size = sizeof(ToolTipMsg);
					SI32 ddd = _tcslen(ToolTipMsg);
					bool bIsSealed = m_pclInventoryItem[ i ].IsSealingUp(pclClient->pclItemManager);

					if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_INVENTORY )
					{
						// 가방의 경우에는 가방에 넣을수 있는 아이템 갯수 만큼만..
						if( i >= StartPos + MAX_ITEM_PER_BAG && (  GetDialogControllerID() == NNEWINVENTORY_BAG0_DLG  || GetDialogControllerID() == NNEWINVENTORY_BAG1_DLG ) )
							continue;

						// 재조창이 떠있고 그 안에 포함된 아이템이라면, 드로우 옵션을 반투명으로 설정해줌

						stListViewItemData ListViewItemData;

						//KHY - 0723 - 기간아이템 파괴 방지. 혹시 몰라서.
						if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
						{	
							SI32		useableDay	= 0;
							BOOL bRet = pclclient->GetItemUseAbleDay( &(m_pclInventoryItem[ i ]), &useableDay);

							if(bRet == TRUE && useableDay < 0)		ListViewItemData.SetExpired(true);
//							if(bRet == TRUE )				
//								ListViewItemData.UpViewSet(&(pclClient->sUpViewEffect));
						}
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_pclInventoryItem[ i ].siItemNum, ItemName, ToolTipMsg, false, bIsSealed ); // 아이템 툴팁은 색이 들어가니 ColorSwitch = true
						m_pInventoryListView->InsertItem( &ListViewItemData );
					}
				}
			}
		}
	}

	// 제조창이 떠있으면 제조아이템 재료 목록,  을 탐색하여 드로우 옵션을 반투명으로~ ㄱㄱ
	if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
	{
		for( i = 0; i < 25; i++)
		{
			if(clMakeItemContainer.m_siMaterials[ i ] > 0)
			{
				m_pInventoryListView->SetItemDrawAlphaOption(clMakeItemContainer.m_siMaterials[ i ],true);
			}
		}
		for(i = 0; i < 4; i++)
		{
			if(clMakeItemContainer.m_sitools[i] > 0)
			{
				m_pInventoryListView->SetItemDrawAlphaOption(clMakeItemContainer.m_sitools[ i ],true);
			}
		}

		if(pclclient->pclCM->CR[1]->siOrgInvEnchantInx > 0)
		{
			m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siOrgInvEnchantInx, true);
		}

		if(pclclient->pclCM->CR[1]->siMatInvEnchantInx > 0)
		{
			m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siMatInvEnchantInx, true);
		}

		for(i = 0; i < NOW_ENCHANT_NUMBER; i++)
		{
			if(pclclient->pclCM->CR[1]->siElementEnchantInx[i] > 0)
			{
				m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siElementEnchantInx[i], true);
			}
		}
	}

	if( pclclient->m_pDialog[NNEWNPCREPAIR_DLG] && pclclient->m_pDialog[NNEWNPCREPAIR_DLG]->IsShow() )
	{
		NNewNPCRepairDlg* pkNewNPCRepairDlg = (NNewNPCRepairDlg*)pclclient->m_pDialog[NNEWNPCREPAIR_DLG];

		SI32 siRepairItemPos = pkNewNPCRepairDlg->GetRepairItemPos();
		m_pInventoryListView->SetItemDrawAlphaOption( siRepairItemPos, true );

		for( SI32 i=0; i<pkNewNPCRepairDlg->GetMaterialItemCount(); i++ )
		{
			SI32 siMaterialItemPos = pkNewNPCRepairDlg->GetMaterialItemPos( i );
			m_pInventoryListView->SetItemDrawAlphaOption( siMaterialItemPos, true );
		}
	}

	CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
	NItemRepairDlg* pclItemRepairDlg = NULL;
	if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;

	if( pclItemRepairDlg && pclItemRepairDlg->IsShow() )
	{
		SI32 siRepairItemPos = pclItemRepairDlg->GetRepairItemPos();
		m_pInventoryListView->SetItemDrawAlphaOption( siRepairItemPos, true );

		for( SI32 i=0; i<pclItemRepairDlg->GetMaterialItemCount(); i++ )
		{
			SI32 siMaterialItemPos = pclItemRepairDlg->GetMaterialItemPos( i );
			m_pInventoryListView->SetItemDrawAlphaOption( siMaterialItemPos, true );
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

	//m_pMakeItemComboBox->Clear();

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

					//sprintf( buffer, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );
					StringCchPrintf(buffer,256, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );

				}
			}
		}
	}
	if ( m_bMax && IsShow() )
	{

		ChangeMaxMode( m_bMax );
	}
	return;
}


void CNNewInventory::LoadSummonData()
{
}

void CNNewInventory::LoadInventoryTabData()
{
	//SI32 id = 1;

	//SI16 Counter = 0;

	//m_bHorseHave = pclClient->pclCM->CR[id]->HaveHorse();

	//if ( m_bHorseHave )
	//{
	//	m_pButtonHorseInventory->Enable( true );
	//}
	//else
	//{	
	//	m_pButtonHorseInventory->Enable( false );
	//}


	//if ( pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
	//{
	//	m_bBagHave = true;
	//	m_pButtonBagInventory->Enable( true );
	//}
	//else
	//{
	//	m_bBagHave = false;
	//	m_pButtonBagInventory->Enable( false );
	//}

	return;
}


void CNNewInventory::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  )
{

}


void CNNewInventory::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	cltClient *pclclient = (cltClient *)pclClient;

	//// 사람 탭인 경우에만 단축키 출력 그리고 현재 무기 설정 이미지 보여주는 것도 되지( tab 버튼으로 바꾸는 거 )
	//if ( m_siCurrentInventoryIndex == TAB_SELECTED_PERSON )
	//{
	if( GetDialogControllerID() == NNEWINVENTORY_DLG )
	{
		SI16 i = 0;
		TSpr *pAcceleratorImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( LISTVIEW_ACCELERATOR_FILENAME );

		if ( pAcceleratorImg )
		{
			if( m_bMax && pclClient->pclCM->CR[1]->HaveHorse() )
			{
				for ( i = 0; i < 5; ++i )
				{
					// 시작 : 9, 298    OffSet : 57
					GP.PutSpr(pAcceleratorImg, ScreenRX + GetX() + 7, ScreenRY + GetY() + 20 + (i * 44), LISTVIEW_ACCELERATOR_FONTINDEX  + i  ); //*1은 F0 부터 시작함으로 
				}
			}
			else
			{
				for ( i = 0; i < 5; ++i )
				{
					// 시작 : 9, 298    OffSet : 57
					GP.PutSpr(pAcceleratorImg, ScreenRX + GetX() + 43, ScreenRY + GetY() + 20 + (i * 44), LISTVIEW_ACCELERATOR_FONTINDEX  + i  ); //*1은 F0 부터 시작함으로 
				}
			}
		}
	}
	return;
}

bool CNNewInventory::Pop( stMoveData * pMoveData )
{
	return m_pBuffer->Pop( pMoveData );	
}

bool CNNewInventory::Pop( BYTE* pMoveData )
{
	return m_pBuffer->Pop( pMoveData );
}

void CNNewInventory::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
	case 4:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				SI32 *ItemUnique = (SI32 *)pData;				

				if ( *ItemUnique <= 0 )
				{
					return;
				}
				cltGameMsgRequest_NolimitTicket_Warp clInfo(*ItemUnique);
				cltMsg clMsg( GAMEMSG_REQUEST_NOLIMITTICKET_WARP , sizeof(clInfo) , (BYTE*)&clInfo );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
			}
		}
		break;
	case 5:
		{
			//무한 워프 쓰면안되면 일로 보내버림
			//그냥 튕겨 내자.할일 없다.
			return;
		}break	;

	case 6:	// 해당 위치로 날라간다.
		{
			if ( RetValue )
			{
				SI32 *ItemUnique = (SI32 *)pData;				

				if ( *ItemUnique <= 0 )
				{
					return;
				}
				cltGameMsgRequest_WarpTargetWorld clInfo(*ItemUnique);
				cltMsg clMsg( GAMEMSG_REQUEST_WARP_TARGETWORLD , sizeof(clInfo) , (BYTE*)&clInfo );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
			}
		}break	;
// [성웅] 레벨 보상 상자에서 아이템을 준다 
	case 7:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				if ( pclClient->IsCountrySwitch(Switch_LevelLimitBox) == false)		return	;
				SI32 *ItemUnique = (SI32 *)pData;				

				if ( *ItemUnique <= 0 )
				{
					return;
				}
				cltGameMsgRequest_UseLimitLevleBox clInfo(*ItemUnique);
				cltMsg clMsg( GAMEMSG_REQUEST_OPEN_LEVEL_LIMIT_BOX , sizeof(clInfo) , (BYTE*)&clInfo );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
			}
		}
		break;
	}	
	return;
}


SI32 CNNewInventory::GetInventoryType(SI32 siItemType)
{
	SI32 StartPos =0;
	SI32 siItemUinque =m_pclInventoryItem[m_siSelectedIndex].siUnique;

	if (siItemType == ITEMTYPE_HELMET )
		return PERSONITEM_HELMET;

	else if  (siItemType == ITEMTYPE_ARMOUR  )
		return PERSONITEM_ARMOUR;

	//cyj 화살 및 탄환 장착 가능하도록 수정
	else if(( siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == ITEMUNIQUE(10100) ||siItemUinque == ITEMUNIQUE(10090) || m_pclInventoryItem[m_siSelectedIndex].IsBulletItem()) 
		&& (m_pclInventoryItem[ PERSONITEM_WEAPON1 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_WEAPON1 + StartPos ))
		return PERSONITEM_WEAPON2;

	//cyj 화살 및 탄환 장착 가능하도록 수정
	else if ( siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == ITEMUNIQUE(10100) ||siItemUinque == ITEMUNIQUE(10090) || m_pclInventoryItem[m_siSelectedIndex].IsBulletItem())
		return PERSONITEM_WEAPON1;

	else if ( siItemType == ITEMTYPE_NECK || siItemType == ITEMTYPE_AVATAR_NECK ) // 아바타 목걸이
		return PERSONITEM_NECK;

	else if (( siItemType == ITEMTYPE_RING || siItemType == ITEMTYPE_AVATAR_RING ) // 아바타 반지
		&&( m_pclInventoryItem[ PERSONITEM_RING2 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_RING2 + StartPos ))
		return PERSONITEM_RING1;

	else if ( siItemType == ITEMTYPE_RING || siItemType == ITEMTYPE_AVATAR_RING )
		return PERSONITEM_RING2;

	else if ( siItemType == ITEMTYPE_BELT )
		return PERSONITEM_BELT;

	else if ( siItemType == ITEMTYPE_SHOES )
		return PERSONITEM_SHOES;

	else if ( siItemType ==ITEMTYPE_MANTLE || siItemType == ITEMTYPE_AVATAR_MANTLE) // 아바타 망토
		return PERSONITEM_MANTLE;

	else if (( siItemType == ITEMTYPE_BAG )
		&&( m_pclInventoryItem[ PERSONITEM_BAG + StartPos ].siUnique == 0 ) )
		return PERSONITEM_BAG;

	else if ( siItemType == ITEMTYPE_BAG )
		return PERSONITEM_BAG_SUB;

	else if ( siItemType == ITEMTYPE_HAT || siItemType == ITEMTYPE_AVATAR_HAT ) // 아바타 모자
		return PERSONITEM_HAT;

	else if ( siItemType == ITEMTYPE_DRESS || siItemType == ITEMTYPE_AVATAR_DRESS ) // 아바타 의복
		return PERSONITEM_DRESS;
	else
	{
		return PERSONITEM_NONE;
	}
}

SI32 CNNewInventory::GetInventoryTypeForSummon( SI32 siItemType )
{
	switch ( siItemType )
	{
		case ITEMTYPE_HELMET:
			return PERSONITEM_HELMET;

		case ITEMTYPE_ARMOUR:
			return PERSONITEM_ARMOUR;
		
		case ITEMTYPE_GUN:
		case ITEMTYPE_BOW:
		case ITEMTYPE_AXE:
		case ITEMTYPE_STAFF:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_SWORD:
			return PERSONITEM_WEAPON1;

		case ITEMTYPE_BELT:
			return PERSONITEM_BELT;

		case ITEMTYPE_SHOES:
			return PERSONITEM_SHOES;
		
		case ITEMTYPE_MANTLE:
			return PERSONITEM_MANTLE;
	}

	if ( TRUE == m_pclInventoryItem[m_siSelectedIndex].IsBulletItem() )
	{
		return PERSONITEM_WEAPON2;
	}

	return PERSONITEM_NONE;

}


void CNNewInventory::ChangeItem( SI32 Destination )
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
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG; ++t )
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

	if ( m_siSelectedIndex == PERSONITEM_BAG_SUB )
	{	
		// 가방 내로 가방 옮기기 불가
		if( Destination >= PERSONITEM_BAG1 && Destination < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
		{
			TCHAR* pTitle = GetTxtFromMgr(1167);
			TCHAR* pText = GetTxtFromMgr(1168);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		// 가방에 아이템 있다면 옮기기 불가
		for ( SI16 t = PERSONITEM_BAG1; t < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG; ++t )
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
	if((Destination >= 0  && Destination <= 14 ) || Destination == 82 || Destination == 97 )
	{
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

void CNNewInventory::IsMinimi(bool bSet)
{
	cltClient *pclclient = (cltClient *)pclClient;

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	if( bSet ) // 최소화
	{
		m_bMax = false;
		horseinventory->CDialogController::Show( false );
		bag0inventory->CDialogController::Show( false );
		bag1inventory->CDialogController::Show( false );
		m_siCurrentInventoryIndex = TAB_SELECTED_PERSON;
		personinventory->LoadItemData();


		personinventory->SetFileName( TEXT("NInterface/interface/INTERFACE_84x240_00_000.SPR") );

		// SetDialogPos -> SetTitle 순서에 주의!
		// SetDialogPos 로 엔코더버퍼의 크기도 변하게 된다.

		personinventory->SetDialogPos( pclclient->GetScreenXSize() - 84, 250, 68, 240 );
		personinventory->SetTitle(NULL);
		personinventory->m_pInventoryListView->MoveController(40, 17);

		personinventory->SetMoveable( false );

		if ( pclclient->IsCountrySwitch( Switch_AutoMakeItem ) == true ){
			personinventory->m_pMakeItemBtn->Show( true );//최소화 일때도 제작 버튼은 활성화.
		}
		else{
			personinventory->m_pMakeItemBtn->Show( false );//최소화 일때도 제작 버튼은 활성화.
		}
		
		personinventory->m_pEnchantItemBtn->Show( false );
		personinventory->m_pItemmallListBtn->Show( false );
	
		//		m_pMakeItemComboBox->Show( false );
		//		m_pMakeItemStatic->Show( false );
		//	m_pMakeItemInputEdit->Show( false );

		personinventory->m_pInventoryListView->Show( true );
		personinventory->m_pMoveItemToHorseListView->Show( false );
		personinventory->m_pMoveItemToBag0ListView->Show( false );
		personinventory->m_pMoveItemToBag1ListView->Show( false );


		personinventory->m_pButtonMini->Show( true );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			personinventory->m_pButtonSpaceBox->Show( false );
		}
		else
		{
			personinventory->m_pButtonMax->Show( true );
		}

		//m_pButtonManInventory->Show( false );
		//m_pButtonHorseInventory->Show( false );
		//m_pButtonBagInventory->Show( false );

		personinventory->m_pStatic[0]->Show( false );
		personinventory->m_pStatic[1]->Show( false );
		personinventory->m_pStatic[2]->Show( false );
		if ( pclclient->siServiceArea == ConstServiceArea_China )
		{
			personinventory->m_pWeightStatic->MoveController( -25, 3 );
		}
		else
		{
			personinventory->m_pWeightStatic->Show( false );
		}
	}
	else
	{
		m_bMax = false;
		ChangeMaxMode( m_bMax );
		//TCHAR* pText = GetTxtFromMgr(5067);

		//SetFileName( TEXT("NInterface/interface/INTERFACE_202x325_00_000.SPR") );

		//SetDialogPos( pclclient->GetScreenXSize() - 202, 250, 202, 325 );
		//SetTitle(pText,CONTROLLER_TITLE_COLOR,11);		

		//SetMoveable( true );

		//m_pMakeItemBtn->Show( true );
		////m_pMakeItemComboBox->Show( true );
		////		m_pMakeItemStatic->Show( true );
		////	m_pMakeItemInputEdit->Show( true );

		//m_pInventoryListView->Show( true );
		//m_pMoveItemToHorseListView->Show( true );
		//m_pMoveItemToBagListView->Show( true );

		//m_pButtonMini->Show( true );
		////m_pButtonManInventory->Show( true );
		////m_pButtonHorseInventory->Show( true );
		////m_pButtonBagInventory->Show( true );

		//m_pStatic[0]->Show( true );
		//m_pStatic[1]->Show( true );
	}
}

void CNNewInventory::ChangeMaxMode(bool bSet)//손성웅
{
	m_bMini = false;

	cltClient*	pclclient = (cltClient*)pclClient;
	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	//if (HORSEACTION_DIE) {//말이 죽으면 아이템이 밀리는 등의 버그가 발생하여 죽었을경우 처리.[2008.08.02 손성웅]
	//	horseinventory->CDialogController::Show( false );
	//	bag0inventory->CDialogController::Show( false );
	//	bag1inventory->CDialogController::Show( false );
	//	personinventory->SetFileName( TEXT("NInterface/interface/INTERFACE_218x280_00_000.SPR") );
	//	personinventory->SetDialogPos( pclclient->GetScreenXSize() - 218 , 250 , 218, 280 );
	//	TCHAR* pText = GetTxtFromMgr(5067); //  "소지품( alt+i)"
	//	personinventory->SetTitle(pText,CONTROLLER_TITLE_COLOR,11);
	//	personinventory->m_pInventoryListView->MoveController( 40, 17 );
	//	personinventory->m_pWeightStatic->MoveController( 96, 3 );
	//	//KHY - 0618 - 삭제.항상 false. 		
	//	personinventory->m_pMoveItemToHorseListView->Show( false );
	//	personinventory->m_pMoveItemToBag0ListView->Show( false );
	//	personinventory->m_pMoveItemToBag1ListView->Show( false );
	//	//////////////////////
	//	personinventory->m_pMakeItemBtn->Show( true );
	//	personinventory->m_pEnchantItemBtn->Show( true );
	//	personinventory->m_pItemmallListBtn->Show( true );
	//	personinventory->m_pButtonMini->Show( true );
	//	personinventory->m_pButtonMax->Show( true );
	//	personinventory->SetMoveable( true );	
	//	//KHY - 0618 - 삭제.항상 false. 
	//	personinventory->m_pStatic[0]->Show( false );
	//	personinventory->m_pStatic[1]->Show( false );
	//	personinventory->m_pStatic[2]->Show( false);
	//	//////////////////////
	//	personinventory->m_pCloseButton->Show(true);
	//	personinventory->m_pCloseButton->MoveController(GetWidth() - 16, 3);
	//	//personinventory->m_pMakeItemBtn->MoveController( 23, 280 );
	//	//personinventory->m_pItemmallListBtn->MoveController( 23, 300 );
	//}

	if ( ( pclclient->pclCM->CR[1]->HaveHorse() == false )  &&
		( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique < 1) && 
		( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique < 1) )
	{
		m_bMax = false;
		bSet = false;
	}

	personinventory->m_pWeightStatic->Show( true );

	if(bSet)
	{
		personinventory->SetFileName( TEXT("NInterface/interface/INTERFACE_218x239_00_000.SPR") );
		personinventory->SetDialogPos( pclclient->GetScreenXSize() - 218 , 250 , 218, 239 );
		TCHAR* pText = GetTxtFromMgr(5067); //  "소지품( alt+i)"
		personinventory->SetTitle(pText,CONTROLLER_TITLE_COLOR,11);
		personinventory->m_pMoveItemToHorseListView->Show( false );
		personinventory->m_pMoveItemToBag0ListView->Show( false );
		personinventory->m_pMoveItemToBag1ListView->Show( false );
		personinventory->SetMoveable( false );
		personinventory->m_pStatic[0]->Show( false );
		personinventory->m_pStatic[1]->Show( false );
		personinventory->m_pStatic[2]->Show( false );
		personinventory->m_pButtonMini->Show( true );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			personinventory->m_pButtonSpaceBox->Show( true );
		}		
		else
		{
			personinventory->m_pButtonMax->Show( true );
		}
		
		personinventory->m_pMakeItemBtn->Show( true );
		personinventory->m_pEnchantItemBtn->Show( true );
		personinventory->m_pItemmallListBtn->Show( true );

		if ( pclclient->pclCM->CR[1]->HaveHorse() )//말인벤토리.
		{
			horseinventory->CDialogController::Show( true );
			horseinventory->m_pMakeItemBtn->Show( true );
			horseinventory->m_pEnchantItemBtn->Show( true );
			horseinventory->m_pItemmallListBtn->Show( true );
			personinventory->SetFileName( TEXT("NInterface/interface/INTERFACE_182x239_00_000.SPR") );
			personinventory->SetDialogPos( pclclient->GetScreenXSize() - 182 , 250 , 182, 239 );
			TCHAR* pText = GetTxtFromMgr(4661); //  "소지품( alt+i)"<<07.0608뒤에 (alt+i)삭제를 위해 텍스트교체

			personinventory->SetTitle(pText,CONTROLLER_TITLE_COLOR,11);
			personinventory->m_pInventoryListView->MoveController( 3, 17 );
			personinventory->m_pWeightStatic->MoveController( 60, 3 );
			personinventory->m_pButtonMini->Show( false );
			
			if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
			{
				personinventory->m_pButtonSpaceBox->Show( false );
			}
			else
			{
				personinventory->m_pButtonMax->Show( false );
			}

			personinventory->m_pMakeItemBtn->Show( false );
			personinventory->m_pEnchantItemBtn->Show( false );
			personinventory->m_pItemmallListBtn->Show( false );
			personinventory->m_pCloseButton->Show(true);
			personinventory->m_pCloseButton->MoveController(GetWidth() - 16, 3);

		}
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
		{
			bag0inventory->CDialogController::Show( true );
			//personinventory->m_pMakeItemBtn->Show( false );
			//personinventory->m_pItemmallListBtn->Show( false );
			//bag0inventory->m_pMakeItemBtn->MoveController( 23, 151 );
			//bag0inventory->m_pMakeItemBtn->Show( true );
			//bag0inventory->m_pItemmallListBtn->MoveController( 23, 171 );
			//bag0inventory->m_pItemmallListBtn->Show( true );
		}
		//else
		//{
		//	personinventory->m_pMakeItemBtn->MoveController( 23, 238 );
		//	personinventory->m_pItemmallListBtn->MoveController( 23, 258 );
		//}
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique > 0 )
		{
			bag1inventory->CDialogController::Show( true );
		}

		personinventory->m_pCloseButton->Show(true);
		personinventory->m_pCloseButton->MoveController(personinventory->GetWidth() - 16, 3);
	}
	else
	{
		horseinventory->CDialogController::Show( false );
		bag0inventory->CDialogController::Show( false );
		bag1inventory->CDialogController::Show( false );
		personinventory->SetFileName( TEXT("NInterface/interface/INTERFACE_218x280_00_000.SPR") );
		personinventory->SetDialogPos( pclclient->GetScreenXSize() - 218 , 250 , 218, 280 );
		TCHAR* pText = GetTxtFromMgr(5067); //  "소지품( alt+i)"
		personinventory->SetTitle(pText,CONTROLLER_TITLE_COLOR,11);
		personinventory->m_pInventoryListView->MoveController( 40, 17 );
		personinventory->m_pWeightStatic->MoveController( 96, 3 );
//KHY - 0618 - 삭제.항상 false. 		
		personinventory->m_pMoveItemToHorseListView->Show( true );
		personinventory->m_pMoveItemToBag0ListView->Show( true );
		personinventory->m_pMoveItemToBag1ListView->Show( true );
//////////////////////
		personinventory->m_pMakeItemBtn->Show( true );
		personinventory->m_pEnchantItemBtn->Show( true );
		personinventory->m_pItemmallListBtn->Show( true );
		personinventory->m_pButtonMini->Show( true );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			personinventory->m_pButtonSpaceBox->Show( true );
		}
		else
		{
			personinventory->m_pButtonMax->Show( true );
		}

		personinventory->SetMoveable( true );	
//KHY - 0618 - 삭제.항상 false. 
		personinventory->m_pStatic[0]->Show( true );
		personinventory->m_pStatic[1]->Show( true );
		personinventory->m_pStatic[2]->Show( true );
//////////////////////
		personinventory->m_pCloseButton->Show(true);
		personinventory->m_pCloseButton->MoveController(GetWidth() - 16, 3);
		//personinventory->m_pMakeItemBtn->MoveController( 23, 280 );
		//personinventory->m_pItemmallListBtn->MoveController( 23, 300 );
	}
}

void CNNewInventory::CheckWrongSelectedInven()//잘못된인벤
{

	if(m_bHorseHave == false)
	{
		if(m_siCurrentInventoryIndex == TAB_SELECTED_HORSE)
		{
			SelectInven(TAB_SELECTED_PERSON);
		}
	}

	if(m_bBagHave == false)
	{
		if(m_siCurrentInventoryIndex == TAB_SELECTED_BAG)
		{
			SelectInven(TAB_SELECTED_PERSON);
		}
	}
}

void CNNewInventory::SelectInven(SI16 SelectInventoryIndex)//주석처리 됐었던 부분이다,.
{
	//m_siCurrentInventoryIndex = SelectInventoryIndex;

	//if(m_siCurrentInventoryIndex == TAB_SELECTED_PERSON)
	//{
//		m_pStatic[0]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE );
//		m_pStatic[1]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );
//
//		m_pButtonManInventory->SetSelect(TRUE);
//		m_pButtonHorseInventory->SetSelect(FALSE);
//		m_pButtonBagInventory->SetSelect(FALSE);
//	}
//	else if( m_siCurrentInventoryIndex == TAB_SELECTED_HORSE)
//	{
//		m_pStatic[0]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON );
//		m_pStatic[1]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_BAG );	
//
//		m_pButtonManInventory->SetSelect(FALSE);
//		m_pButtonHorseInventory->SetSelect(TRUE);
//		m_pButtonBagInventory->SetSelect(FALSE);
//	}
//	else if( m_siCurrentInventoryIndex == TAB_SELECTED_BAG)
//	{
//		m_pStatic[0]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_PERSON );
//		m_pStatic[1]->SetFontIndex( INVENTORY_TAB_IMAGE_FONT_INDEX_HORSE );
//
//		m_pButtonManInventory->SetSelect(FALSE);
//		m_pButtonHorseInventory->SetSelect(FALSE);
//		m_pButtonBagInventory->SetSelect(TRUE);
//	}
}

void CNNewInventory::ShowInvenWeight()
{
	if ( GetDialogControllerID() != NNEWINVENTORY_DLG || m_bMini )
		return;

	SI32 siTotalWeight = pclClient->pclCM->CR[1]->GetTotalWeight();		// 현재 인벤토리의 총무게
	SI32 siMaxWeight = pclClient->pclCM->CR[1]->GetMaxWeight();	

	// 이전의 무게와 변동 없으면 리턴
	static SI32	oldsiTotalWeight = 0;
	static SI32	oldsiMaxWeight = 0;

	if( oldsiTotalWeight == siTotalWeight && oldsiMaxWeight == siMaxWeight) return;

	oldsiTotalWeight = siTotalWeight;
	oldsiMaxWeight = siMaxWeight;


	// 20.0/30.0   라는 무게표시 문자열
	TCHAR strWeightBuffer[ 128 ] = TEXT("");
	// 들수 있는 최대 무게
	TCHAR* pText = GetTxtFromMgr(1158);
	StringCchPrintf( strWeightBuffer, 128, pText, (float)siTotalWeight/10.0, (float)siMaxWeight/10.0);



	m_pWeightStatic->SetText(strWeightBuffer,DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	if( siTotalWeight > siMaxWeight  )     
	{ 
		m_pWeightStatic->SetFontColor( RGB(225,50,50) );
	}
	else 
	{
		m_pWeightStatic->SetFontColor( CONTROLLER_TITLE_COLOR );
	}


	m_pWeightStatic->Refresh();
}

void CNNewInventory::ShowSlotNum()
{
	if ( GetDialogControllerID() != NNEWINVENTORY_DLG )
	{
		m_pWeightStatic->SetText(TEXT(""),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	SI32 maxSlotNum = MAX_ITEM_PER_INVENTORY;
	SI32 curSlotNum = 0;

	for ( SI32 i = PERSONITEM_INV0 ; i < PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY ; i++ )
	{
		if ( pclchar->pclCI->clCharItem.clItem[i].siUnique > 0 )
		{
			++curSlotNum;
		}
	}

	if( pclchar->HaveHorse() )
	{
		maxSlotNum += MAX_ITEM_PER_INVENTORY;

		for ( SI32 i = PERSONITEM_HORSE0 ; i < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY ; i++ )
		{
			if ( pclchar->pclCI->clCharItem.clItem[i].siUnique > 0 )
			{
				++curSlotNum;
			}
		}
	}

	if( pclchar->pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique != 0 )
	{
		maxSlotNum += MAX_ITEM_PER_BAG;

		for ( SI32 i = PERSONITEM_BAG0 ; i < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG ; i++ )
		{
			if ( pclchar->pclCI->clCharItem.clItem[i].siUnique > 0 )
			{
				++curSlotNum;
			}
		}
	}

	if( pclchar->pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique != 0 )
	{
		maxSlotNum += MAX_ITEM_PER_BAG;

		for ( SI32 i = PERSONITEM_BAG1 ; i < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG ; i++ )
		{
			if ( pclchar->pclCI->clCharItem.clItem[i].siUnique > 0 )
			{
				++curSlotNum;
			}
		}
	}

	TCHAR buf[64];
	StringCchPrintf( buf, 64, TEXT("%d/%d"), curSlotNum, maxSlotNum );
	m_pWeightStatic->SetText(buf,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

bool CNNewInventory::CheckCanEnchintItem(SI32 siUnique)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 sitype = pclclient->pclItemManager->GetItemType(siUnique);
	switch(sitype)
	{
	case  ITEMTYPE_SWORD:
	case  ITEMTYPE_SPEAR:
	case  ITEMTYPE_STAFF:
	case  ITEMTYPE_AXE:
	case  ITEMTYPE_BOW:
	case  ITEMTYPE_GUN:
	case  ITEMTYPE_CANNON:
	case  ITEMTYPE_ARMOUR:
	case  ITEMTYPE_BELT:
	case  ITEMTYPE_SHOES:
	case  ITEMTYPE_HELMET:
		return true;						 	

	}				
	return false;
}				

bool CNNewInventory::CheckCanPutonItem(SI32 siUnique)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 sitype = pclclient->pclItemManager->GetItemType(siUnique);
	switch(sitype)
	{
	case  ITEMTYPE_SWORD:
	case  ITEMTYPE_SPEAR:
	case  ITEMTYPE_STAFF:
	case  ITEMTYPE_AXE:
	case  ITEMTYPE_BOW:
	case  ITEMTYPE_GUN:
	case  ITEMTYPE_CANNON:
	case  ITEMTYPE_ARMOUR:
	case  ITEMTYPE_BELT:
	case  ITEMTYPE_SHOES:
	case  ITEMTYPE_HELMET:
	case  ITEMTYPE_AVATAR_MANTLE:
	case  ITEMTYPE_AVATAR_DRESS:
	case  ITEMTYPE_AVATAR_HAT:
	case  ITEMTYPE_AVATAR_NECK:
	case  ITEMTYPE_AVATAR_RING:
		return true;						 	

	}				
	return false;
}				


bool CNNewInventory::CheckCanEquipItem(SI32 siUnique)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 sitype = pclclient->pclItemManager->GetItemType(siUnique);

	cltItem clItem(siUnique);


	// 불렛아이템일때 장착가능
	if (clItem.IsBulletItem())
		return true;

/*	//과금 총알 착용가능[성웅]
	if (siUnique == ITEMUNIQUE(7857)||ITEMUNIQUE(7856)) {
		return true;
	}*/

	// 낫, 곡괭이, 낚시대 일때는 장착가능
	if (siUnique == ITEMUNIQUE(10090) || siUnique == ITEMUNIQUE(10100) ||siUnique == ITEMUNIQUE(17100)
		|| siUnique == ITEMUNIQUE(17150))
		return true;

	switch(sitype)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:
	case ITEMTYPE_RING:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_NECK:
	case ITEMTYPE_AVATAR_MANTLE:
	case ITEMTYPE_AVATAR_DRESS:
	case ITEMTYPE_AVATAR_HAT:		
	case ITEMTYPE_AVATAR_NECK:	
	case ITEMTYPE_AVATAR_RING:
	case ITEMTYPE_MANTLE:
		{
			return true;
		}
		break;
	}

	return false;
}				

void CNNewInventory::MakeSetEnchant(bool bMakeButton)
{
	m_pMakeItemBtn->Enable(bMakeButton);    
	m_bMakeButton = bMakeButton;
}


void CNNewInventory::CheckCharStatusChanged()
{	
	cltCharCommon* pclchar = pclClient->pclCM->CR[1];		
	static SI32 oldLevel = pclchar->pclCI->clIP.GetLevel();
	static SI32 oldStr = pclchar->clPB.clTotalBA.GetStr();
	static SI32 oldDex = pclchar->clPB.clTotalBA.GetDex();
	static SI32 oldMag = pclchar->clPB.clTotalBA.GetMag();
	static SI32 oldVit = pclchar->clPB.clTotalBA.GetVit();
	static SI32	oldHnd = pclchar->clPB.clTotalBA.GetHnd();
	static SI32	oldFame = pclchar->pclCI->clIP.siFameLevel;

	//static cltPSkillInfo oldSkilinfo =	pclchar->pclCI->clCharSkill;


	if( pclchar->pclCI->clIP.GetLevel() !=  oldLevel) m_bStatusChanged = TRUE;
	if( pclchar->clPB.clTotalBA.GetStr() !=  oldStr) m_bStatusChanged = TRUE;
	if( pclchar->clPB.clTotalBA.GetDex() !=  oldDex) m_bStatusChanged = TRUE;
	if( pclchar->clPB.clTotalBA.GetMag() !=  oldMag) m_bStatusChanged = TRUE;
	if( pclchar->clPB.clTotalBA.GetVit() !=  oldVit) m_bStatusChanged = TRUE;
	if( pclchar->clPB.clTotalBA.GetHnd() !=  oldHnd) m_bStatusChanged = TRUE;
	if( pclchar->pclCI->clIP.siFameLevel !=  oldFame) m_bStatusChanged = TRUE;


	if(m_bStatusChanged == TRUE)
	{	
		LoadItemData();

		m_bStatusChanged = FALSE;
		oldLevel = pclchar->pclCI->clIP.GetLevel();
		oldStr = pclchar->clPB.clTotalBA.GetStr();
		oldDex = pclchar->clPB.clTotalBA.GetDex();
		oldMag = pclchar->clPB.clTotalBA.GetMag();
		oldVit = pclchar->clPB.clTotalBA.GetVit();
		oldHnd = pclchar->clPB.clTotalBA.GetHnd();
		oldFame = pclchar->pclCI->clIP.siFameLevel;

	}
}

void CNNewInventory::EquipItem( SI16 Pos )
{
	cltClient	*pclclient		= (cltClient *)pclClient;
	SI16		SelectedIndex	= Pos;
	m_siSelectedIndex			= Pos;
	m_siCurrentSummonIndex		= SELECTED_HUMAN; 

	// 창이 혹시 안열려 있을수도 있으므로
	if ( pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ] )
	{
		m_siCurrentSummonIndex = ((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->m_siCurrentSummonIndex;
	}
	

	if ( m_siSelectedIndex >= 0 )
	{
		m_bClickChangeItem = true;

		SI16 SelectPos	= -1;
		SI16 StartPos	= 0;

		SelectPos = m_siSelectedIndex;

		if ( SelectPos > -1 )
		{
			SI32 siItemUinque	= m_pclInventoryItem[m_siSelectedIndex].siUnique; //m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique;
			SI32 ItemType		= pclclient->pclItemManager->GetItemType(siItemUinque);
			SI32 siItemType		= GetInventoryType(ItemType);

			if( PERSONITEM_NONE == siItemType)
			{
				m_pInventoryListView->ClearSelectedItem();
				m_siSelectedIndex = -1;
				m_bClickChangeItem = false;
				return;
			}

			//KHY - 1023 - 총알 화살 무한사용 방지.
			if( !(pclclient->m_pDialog[ NPERSONALSHOP_DLG ]) )
			{
				if ( SELECTED_HUMAN ==  m_siCurrentSummonIndex )
				{
					if ( m_pclInventoryItem[ siItemType ].siUnique == 0 )
					{											
						MoveItem( siItemType );
					}
					else if(m_pclInventoryItem[ siItemType ].siUnique != 0 && ItemType != ITEMTYPE_SCROLL)
					{											
						ChangeItem( siItemType );										
					}

				}
				else
				{
					SI32 siSummonInventoryType = GetInventoryTypeForSummon(ItemType);
					if ( PERSONITEM_NONE == siSummonInventoryType )
					{
						TCHAR* pTitle	= GetTxtFromMgr(1074);
						TCHAR* pText	= GetTxtFromMgr(1075);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
					else
					{
						MoveItem( GetInventoryTypeForSummon(ItemType) );
					}
				}
			}
		}

		m_pInventoryListView->ClearSelectedItem();
		m_siSelectedIndex = -1;
		m_bClickChangeItem = false;
	}
	else
	{
		m_siSelectedIndex = SelectedIndex;
		//m_siPreItemDragIndex = -1;
	}
}
void CNNewInventory::SetInventoryMode(SI16 DialogId)
{
	cltClient* pclclient = (cltClient*)pclClient;

	SetDialogControllerID( DialogId );
	CNNewInventory* inventory = (CNNewInventory*)pclclient->m_pDialog[DialogId];

	TCHAR* pszText = NULL;

	switch( DialogId )
	{
	case NNEWINVENTORY_HORSE_DLG:
		inventory->SetFileName( TEXT("NInterface/interface/INTERFACE_218x239_00_000.SPR") );
		inventory->SetDialogPos( pclclient->GetScreenXSize() - 399 , 250 , 218, 239 );
		pszText = GetTxtFromMgr(6349);
		inventory->SetTitle(pszText ,CONTROLLER_TITLE_COLOR,11);
		inventory->m_pMoveItemToHorseListView->Show( false );
		inventory->m_pMoveItemToBag0ListView->Show( false );
		inventory->m_pMoveItemToBag1ListView->Show( false );
		inventory->m_pMakeItemBtn->Show( false );
		inventory->m_pEnchantItemBtn->Show( false );
		inventory->m_pItemmallListBtn->Show( false );
		inventory->m_pButtonMini->Show( true );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			inventory->m_pButtonSpaceBox->Show( true );
		}
		else
		{
			inventory->m_pButtonMax->Show( true );
		}

		inventory->SetMoveable( false );
		inventory->m_pCloseButton->Show( false );
		inventory->m_pStatic[0]->Show( false );
		inventory->m_pStatic[1]->Show( false );
		inventory->m_pStatic[2]->Show( false );
		break;
	case NNEWINVENTORY_BAG0_DLG:
		inventory->SetFileName(TEXT("NInterface/interface/INTERFACE_182x151_00_000.SPR") );
		inventory->SetDialogPos( pclclient->GetScreenXSize() - 182 , 488 , 182, 151 );
//		inventory->m_pInventoryListView->MoveController(3, 17);
		inventory->m_pInventoryListView->SetControllerPos( 3, 17, 180, 134, 180, 134 );
		pszText = GetTxtFromMgr(6350);
		inventory->SetTitle(pszText,CONTROLLER_TITLE_COLOR,11);
		inventory->m_pMoveItemToHorseListView->Show( false );
		inventory->m_pMoveItemToBag0ListView->Show( false );
		inventory->m_pMoveItemToBag1ListView->Show( false );
		inventory->m_pMakeItemBtn->Show( false );
		inventory->m_pEnchantItemBtn->Show( false );
		inventory->m_pItemmallListBtn->Show( false );
		inventory->m_pButtonMini->Show( false );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			inventory->m_pButtonSpaceBox->Show( false );				
		}
		else
		{
			inventory->m_pButtonMax->Show( false );
		}

		inventory->SetMoveable( false );
		inventory->m_pCloseButton->Show( false );
		inventory->m_pStatic[0]->Show( false );
		inventory->m_pStatic[1]->Show( false );
		inventory->m_pStatic[2]->Show( false );
		break;
	case NNEWINVENTORY_BAG1_DLG:
		inventory->SetFileName(TEXT("NInterface/interface/INTERFACE_182x151_00_000.SPR") );
		inventory->SetDialogPos( pclclient->GetScreenXSize() - 363 , 488 , 182, 151 );
//		inventory->m_pInventoryListView->MoveController(3, 17);
		inventory->m_pInventoryListView->SetControllerPos( 3, 17, 180, 134, 180, 134 );
		pszText = GetTxtFromMgr(6351);
		inventory->SetTitle(pszText,CONTROLLER_TITLE_COLOR,11);
		inventory->m_pMoveItemToHorseListView->Show( false );
		inventory->m_pMoveItemToBag0ListView->Show( false );
		inventory->m_pMoveItemToBag1ListView->Show( false );
		inventory->m_pMakeItemBtn->Show( false );
		inventory->m_pEnchantItemBtn->Show( false );
		inventory->m_pItemmallListBtn->Show( false );
		inventory->m_pButtonMini->Show( false );
		
		if( pclclient->IsCountrySwitch( Switch_SpaceBox ) )
		{
			inventory->m_pButtonSpaceBox->Show( false );
		}	
		else
		{
			inventory->m_pButtonMax->Show( false );
		}

		inventory->SetMoveable( false );
		inventory->m_pCloseButton->Show( false );
		inventory->m_pStatic[0]->Show( false );
		inventory->m_pStatic[1]->Show( false );
		inventory->m_pStatic[2]->Show( false );
		break;
	}
}


bool CNNewInventory::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	switch(pclClickAndDrop_Item->m_siSrcDialogID)
	{
	// 인벤토리에서 인벤토리로 이동하는 경우
	case NNEWINVENTORY_DLG:
	case NNEWINVENTORY_HORSE_DLG:
	case NNEWINVENTORY_BAG0_DLG:
	case NNEWINVENTORY_BAG1_DLG:
		{
			SI32 siSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();
			
			// 같은 위치의 아이템 이므로 리턴.ㅋ
			if( siSelectedIndex == pclClickAndDrop_Item->m_siPara1 )
			{
				return true;
			}

			if( m_pclInventoryItem[ siSelectedIndex ].siUnique <= 0 )
			{
				MoveItem(pclClickAndDrop_Item->m_siPara1, siSelectedIndex);
			}
			else
			{
				ChangeItem(pclClickAndDrop_Item->m_siPara1, siSelectedIndex);
			}
		
            return true;
		}
		break;

	// 장비장착창에서 인벤토리로 이동되었음.
	case NNEWEQUIPWINDOW_DLG:
		{
			SI32 siSelectedIndex = m_pInventoryListView->GetLastSelectedItemIndex();
			if( pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ] )
			{
				m_siCurrentSummonIndex = ((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->m_siCurrentSummonIndex;

				if ( SELECTED_HUMAN == m_siCurrentSummonIndex )
				{
					if( m_pclInventoryItem[ siSelectedIndex ].siUnique <= 0 )
					{
						((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->MoveItem(pclClickAndDrop_Item->m_siPara1, siSelectedIndex);
					}
					else
					{
						((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->ChangeItem(siSelectedIndex, pclClickAndDrop_Item->m_siPara1);
					}
				}
				else
				{
					((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->MoveItem(pclClickAndDrop_Item->m_siPara1, siSelectedIndex);
				}
			}

			return true;
		}
		break;
	}

	return false;
}

void CNNewInventory::GetPositionFromControllerID( IN SI16 siControllerID, IN SI16 siLastSelectedIndex, OUT SI16* psiSelectPos, OUT SI16* psiMakeItemPos )
{
	if ( (NULL == psiSelectPos) || (NULL == psiMakeItemPos) )
	{
		return;
	}

	SI16 siSelectPos	= -1;
	SI16 siMakeItemPos	= -1;


	switch( siControllerID )
	{
		case NNEWEQUIPWINDOW_DLG_LISTVIEW1:
			siSelectPos = PERSONITEM_HELMET;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW2:
			siSelectPos = PERSONITEM_ARMOUR;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW3:
			siSelectPos = PERSONITEM_WEAPON1;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW4:
			siSelectPos = PERSONITEM_NECK;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW5:
			siSelectPos = PERSONITEM_RING1;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW6:
			siSelectPos = PERSONITEM_RING2;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW7:
			siSelectPos = PERSONITEM_BELT;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW8:
			siSelectPos = PERSONITEM_SHOES;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW9:
			siSelectPos = PERSONITEM_WEAPON2;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW10:
			siSelectPos = PERSONITEM_MANTLE;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW11:
			siSelectPos = PERSONITEM_BAG;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW12:
			siSelectPos = PERSONITEM_HAT;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW13:
			siSelectPos = PERSONITEM_DRESS;
			break;
		case NNEWEQUIPWINDOW_DLG_LISTVIEW14:
			siSelectPos = PERSONITEM_BAG_SUB;
			break;

		case NNEWINVENTORY_DIALOG_MOVEITEMTOHORSE_LISTVIEW:
			{	
				if ( pclClient->pclCM->CR[1]->HaveHorse() )
				{
					SI16		pos;
					cltItem		cltoitem;
					SI32		rtnvalue;

					if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_HORSE, 
						&m_pclInventoryItem[ m_siSelectedIndex ],
						NULL, 
						&pos, 
						&cltoitem,
						&rtnvalue) == true)
					{
						siSelectPos = pos;
					}
					else
					{
						siSelectPos = -1;
					}
				}
			}
			break;
		case NNEWINVENTORY_DIALOG_MOVEITEMTOBAG0_LISTVIEW:
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
						siSelectPos = pos;
					}
					else
					{
						siSelectPos = -1;
					}
				}
			}
			break;
		case NNEWINVENTORY_DIALOG_MOVEITEMTOBAG1_LISTVIEW:
			{
				if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG_SUB ].siUnique > 0  )
				{
					SI16 pos;
					cltItem cltoitem;
					SI32 rtnvalue;

					if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG_SUB, 
						&m_pclInventoryItem[ m_siSelectedIndex ],
						NULL, 
						&pos, 
						&cltoitem,
						&rtnvalue) == true)
					{
						siSelectPos = pos;
					}
					else
					{
						siSelectPos = -1;
					}
				}
			}
			break;
		case NNEWINVENTORY_DIALOG_INVENTORY_LISTVIEW:
			siSelectPos = siLastSelectedIndex;
			break;
		case NEWMAKE_LISTVIEW2: //툴
			siMakeItemPos  = NEWMAKE_TOOLSFOR_MAKEITEM;
			break;
		case NEWMAKE_LISTVIEW1: //재료
			siMakeItemPos  = NEWMAKE_MATERIALFOR_MAKEITEM;
			break;		
		case NEWMAKE_LISTVIEW_ORIGINALITEM:
			siMakeItemPos  = NEWMAKE_ORIGINALFOR_ENCHANT;
			break;
		case NEWMAKE_LISTVIEW_MATERIALITEM:
			siMakeItemPos  = NEWMAKE_MATFOR_ENCHANT;
			break;
		case NEWMAKE_LISTVIEW_ELEMENTITEM:
			siMakeItemPos  = NEWMAKE_ELEFOR_ENCHANT;
			break;
	}

	*psiSelectPos	= siSelectPos;
	*psiMakeItemPos	= siMakeItemPos;
}
