#include "../Client/NStructureDlg/House/UserInfo/NHouseUserItemDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"


#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../Client/Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include <CommonLogic.h>

#include "../common/Char/CharCommon/Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../Client/Interface/HouseUnit/HouseUnitBuffer.h"

#include "../CommonLogic/MsgType-House.h"

extern cltCommonLogic* pclClient;

CNHouseUserItemDlg::CNHouseUserItemDlg() : CDialogController()
{
	m_siUserInventorySelectedRealItemIndex = -1;
	m_siUserInventorySelectedIndex = -1;
	m_siWareHouseInventorySelectedIndex = -1;
	m_siWareHouseInventorySelectedRealItemIndex = -1;
	m_siWareHouseInputOutputItemNum = 0;
	m_siUserInventorySelectedItemNumber = 0;
	m_siUserWareHouseSelectedItemNumber = 0;
	m_siCurrentVillageIndex = 0;
	m_siCurrentHouseIndex = 0;

	m_pclHouseUnit   = NULL;

	m_pEdit			 = NULL;
	m_pEditItemCount = NULL;

	m_pStatic[0]	 = NULL;
	m_pStatic[1]	 = NULL;
	m_pStatic[2]	 = NULL;

	m_pButton[0]	 = NULL;
	m_pButton[1]	 = NULL;
	m_pArrangementButton = NULL;

	m_pListView[0]	 = NULL;
	m_pListView[1]	 = NULL;
}

CNHouseUserItemDlg::~CNHouseUserItemDlg()
{
	SAFE_DELETE(m_pListView[1]);
	SAFE_DELETE(m_pListView[0]);

	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pArrangementButton);

	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[0]);

	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pEditItemCount);
}

void CNHouseUserItemDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserItem.ddf"));
	file.CreateChildDialog( this, NHOUSEUSERITEM_DLG, TEXT("dialog_HouseUserItem"), StaticCallBackDialogNHouseUserItemDlg, pParent );

	NEWCEDIT(m_pEdit);
	NEWCEDIT(m_pEditItemCount);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCBUTTON(m_pArrangementButton);

	NEWCLISTVIEW(m_pListView[0]);
	NEWCLISTVIEW(m_pListView[1]);


	file.CreateControl( m_pEdit, NHOUSEUSERITEM_DIALOG_EDIT, TEXT("editbox_NONAME1") );

	file.CreateControl( m_pStatic[0], NHOUSEUSERITEM_DIALOG_STATIC, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOUSEUSERITEM_DIALOG_STATIC, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOUSEUSERITEM_DIALOG_STATIC, TEXT("statictext_NONAME3") );

	file.CreateControl( m_pButton[0], NHOUSEUSERITEM_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHOUSEUSERITEM_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	file.CreateControl( m_pArrangementButton, NHOUSEUSERITEM_DIALOG_BUTTON_ARRANGEMENT, TEXT("button_Arrangement") );

	file.CreateControl( m_pListView[0], NHOUSEUSERITEM_DIALOG_LISTVIEW1, TEXT("listview_NONAME1") );
	file.CreateControl( m_pListView[1], NHOUSEUSERITEM_DIALOG_LISTVIEW2, TEXT("listview_NONAME2") );

	file.CreateControl( m_pEditItemCount, NHOUSEUSERITEM_DIALOG_ITEM_COUNT, TEXT("editbox_Item_Count") );
	
	m_pListView[0]->SetBKColorUse( true );
	m_pListView[0]->SetBorder( true );

	m_pListView[1]->SetBKColorUse( true );
	m_pListView[1]->SetBorder( true );

	Hide();

	if(!pclClient->IsCountrySwitch(Switch_ChangeHouseList))
	{
		m_pArrangementButton->Show(FALSE);
	}
}

void CNHouseUserItemDlg::Hide()
{
	//	SetTimerOnOff(FALSE);

	CControllerMgr::Show( false );

}

void CNHouseUserItemDlg::Show()
{
	//	SetTimerOnOff(TRUE);

	CControllerMgr::Show( true );

}

void CNHouseUserItemDlg::Action()
{
	if ( IsUserInventoryChanged() )
	{
		LoadUserInventory();
	}

	if ( IsWareHouseInventoryChanged() )
	{
		LoadWareHouseInventory();
		//LoadWareHouseMakeItemList();
	}

}

void CALLBACK CNHouseUserItemDlg::StaticCallBackDialogNHouseUserItemDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserItemDlg *pThis = (CNHouseUserItemDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserItemDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserItemDlg::CallBackDialogNHouseUserItemDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHOUSEUSERITEM_DIALOG_BUTTON1:	// <- 방향 화살표 버튼
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siWareHouseInventorySelectedRealItemIndex < 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2142);
						TCHAR* pText = GetTxtFromMgr(2143);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					BOOL bSuccess;
					//	m_siWareHouseInputOutputItemNum = GetDlgItemInt( hDlg, IDC_EDIT_WAREHOUSE_INOUT_ITEM_NUM, &bSuccess, FALSE );
					SI32 TempItemNumber = _tstoi(m_pEdit->GetText());
					m_siWareHouseInputOutputItemNum = TempItemNumber;// GetDlgItemInt( hDlg, IDC_EDIT_WAREHOUSE_INOUT_ITEM_NUM, &bSuccess, FALSE );

					if(m_siWareHouseInputOutputItemNum > m_siUserWareHouseSelectedItemNumber)
						bSuccess = FALSE;
					else
						bSuccess = TRUE;

					if ( bSuccess == FALSE || m_siWareHouseInputOutputItemNum < 1 )
					{
						TCHAR* pTitle = GetTxtFromMgr(1164);
						TCHAR* pText = GetTxtFromMgr(1163);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					OutputToWareHouse();
				}
				break;
			}
		}
		break;
	case NHOUSEUSERITEM_DIALOG_BUTTON_ARRANGEMENT:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgResponse_ArrangeHouseItem clGameMsgResponse_ArrangeHouseItem( m_siCurrentVillageIndex, m_siCurrentHouseIndex);
					cltMsg clMsg( GAMEMSG_REQUEST_HOUSEITEMARRANGEMENT, sizeof(clGameMsgResponse_ArrangeHouseItem), (BYTE*)&clGameMsgResponse_ArrangeHouseItem );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					break;
				}
			}
			break;
		}
	case NHOUSEUSERITEM_DIALOG_BUTTON2: // -> 방향 화살표 버튼 창고로 옮기기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//// 개인 상점중에는 아이템을 창고로 옮길 수 없다.
					//if( true == pclclient->pclCM->CR[1]->bPersonalShopSwitch ||
					//	true == pclclient->pclCM->CR[1]->bPremiumPersonalShop )
					//{
					//	TCHAR* pTitle = GetTxtFromMgr(1891);
					//	TCHAR* pText = GetTxtFromMgr(10038);

					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					//	break;
					//}

					if ( m_siUserInventorySelectedRealItemIndex < 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2142);
						TCHAR* pText = GetTxtFromMgr(2141);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					BOOL bSuccess;
					SI32 TempItemNumber = _tstoi(m_pEdit->GetText());
					m_siWareHouseInputOutputItemNum = TempItemNumber;// GetDlgItemInt( hDlg, IDC_EDIT_WAREHOUSE_INOUT_ITEM_NUM, &bSuccess, FALSE );

					if(m_siWareHouseInputOutputItemNum > m_siUserInventorySelectedItemNumber)
						bSuccess = FALSE;
					else
						bSuccess = TRUE;

					if ( bSuccess == FALSE || m_siWareHouseInputOutputItemNum < 1 )
					{
						TCHAR* pTitle = GetTxtFromMgr(1164);
						TCHAR* pText = GetTxtFromMgr(1163);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					InputToWareHouse();
				}
				break;
			}
		}
		break;

	case NHOUSEUSERITEM_DIALOG_LISTVIEW1:	// 유저 인벤
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					//	m_siUserInventorySelectedRealItemIndex = nlv->lParam;
					SI32 TempUserInventorySelectedIndex = m_pListView[0]->GetSelectedItemIndex();

					if(TempUserInventorySelectedIndex < 0)
						break;

					m_siUserInventorySelectedIndex = TempUserInventorySelectedIndex;
					m_siUserInventorySelectedRealItemIndex = TempUserInventorySelectedIndex;

					SI32 siListViewIndex = PERSONITEM_INV0;

					if ( m_clUserItem[ m_siUserInventorySelectedIndex ].siUnique != 0 )
					{
						TCHAR buffer[10] = TEXT("");
						m_siUserInventorySelectedItemNumber = m_clUserItem[ m_siUserInventorySelectedIndex ].GetItemNum();
						m_pEdit->SetText(_itot(m_clUserItem[ m_siUserInventorySelectedIndex ].GetItemNum(), buffer, 10));
						break;
					}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERITEM_DIALOG_LISTVIEW2:	// 창고 인벤
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					SI32 TempUserInventorySelectedIndex = m_pListView[1]->GetSelectedItemIndex();

					if(TempUserInventorySelectedIndex < 0)
						break;

					m_siWareHouseInventorySelectedRealItemIndex = TempUserInventorySelectedIndex;
					m_siWareHouseInventorySelectedIndex = TempUserInventorySelectedIndex;

					SI32 siListViewIndex = 0;

					if ( m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ].siUnique != 0 )
					{
						TCHAR buffer[10] = TEXT("");
						m_siUserWareHouseSelectedItemNumber = m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ].GetItemNum();
						m_pEdit->SetText(_itot(m_siUserWareHouseSelectedItemNumber, buffer, 10));
					}
				}
				break;
			}
		}
		break;
	}
}

void CNHouseUserItemDlg::SetPersonItemInfo(cltPItemInfo* pPersonItemInfo)
{
	m_pPersonItemInfo = pPersonItemInfo; 
}

void CNHouseUserItemDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

void CNHouseUserItemDlg::SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer)
{
	m_pHouseUnitBuffer = pHouseUnitBuffer;
}
void CNHouseUserItemDlg::SetHouseUserItem(cltHouseUnit *pclHouseUnit)
{
	m_pclHouseUnit	  = pclHouseUnit;

	//	LoadUserInventory();
	//	LoadWareHouseInventory();
}

void CNHouseUserItemDlg::LoadUserInventory()
{
	m_siUserInventorySelectedIndex = -1;

	SI32 i;
	for ( i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_clUserItem[ i ] = m_pPersonItemInfo->clItem[ i ];
		//m_siUserItemPrice[ i ] = pclClient->pclItemManager->pclItemPrice->GetMinPrice( m_clUserItem[ i ].siUnique );
	}
	//BOOL SetInventoryInListView(cltItemManagerCommon* pclitemmanager, HWND hDlg, SI32 ndlgitem, cltImageList* pclImageList, cltPItemInfo* pcliteminfo)
	//	SetInventoryInListView( pclClient->pclItemManager, m_hWareHouseInventoryDlg, IDC_LIST_WAREHOUSE_USER_INVENTORY, NULL, m_pPersonItemInfo );*/

	// 이미지 리스트를 리스트 뷰 컨트롤에 등록한다. 
	//	HWND hwnd = GetDlgItem(hDlg, ndlgitem);
	// 기존의 항목을 모두 삭제한다. 
	//	ListView_DeleteAllItems(hwnd);
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	m_pListView[0]->DeleteAllItems();
	m_pListView[0]->SetStartItemPos( StartPos );

	SI32 index = 0;
	TCHAR ItemName[128];
	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++i)
	{
		if(m_clUserItem[i].siUnique > 0)
		{
			if( m_pPersonItemInfo->clItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pPersonItemInfo->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_pPersonItemInfo->clItem[ i ], ToolTipMsg );

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_pPersonItemInfo->clItem[i].GetItemNum(), ItemName, ToolTipMsg );														
					m_pListView[0]->InsertItem( &ListViewItemData );
				}
			}
		}
	}
}

void CNHouseUserItemDlg::LoadWareHouseInventory()
{
	m_siWareHouseInventorySelectedIndex = -1;

	SI32 i;
	for ( i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
	{
		m_clWareHouseItem[ i ] = m_pclHouseUnit->clStorageInfo.clItem[ i ];
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	m_pListView[1]->DeleteAllItems();
	m_pListView[1]->SetStartItemPos( StartPos );


	SI32 index = 0;
	TCHAR ItemName[128];
	
	SI32 ItemCount = 0;
	
	for( i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
	{
		if( m_clWareHouseItem[ i ].siUnique)
		{
			if(m_clWareHouseItem[ i ].GetFullName(pclClient->pclItemManager, ItemName, 128) == true)
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clWareHouseItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_clWareHouseItem[ i ], ToolTipMsg );

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,m_clWareHouseItem[ i ].GetItemNum(), ItemName, ToolTipMsg );														
					m_pListView[1]->InsertItem( &ListViewItemData );
					//	StartPos++;
					++ItemCount;
				}

				//	sprintf(buffer, TEXT("%s"), itemname );
			}
		}
	}

	TCHAR szItemCount[16] = "";
	StringCchPrintf( szItemCount, sizeof( szItemCount ), "(%d / %d)", ItemCount, MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER );
	m_pEditItemCount->SetText( szItemCount );   
}

bool CNHouseUserItemDlg::IsUserInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clUserItem[ j ];
		BYTE* dst = (BYTE*)&m_pPersonItemInfo->clItem[ j ];

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

bool CNHouseUserItemDlg::IsWareHouseInventoryChanged()
{
	for ( int j = 0; j < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clWareHouseItem[ j ];
		BYTE* dst = (BYTE*)&m_pclHouseUnit->clStorageInfo.clItem[ j ];

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void CNHouseUserItemDlg::InputToWareHouse()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( m_clUserItem[ m_siUserInventorySelectedRealItemIndex ].GetItemNum() < m_siWareHouseInputOutputItemNum )
	{
		TCHAR* pTitle = GetTxtFromMgr(2144);
		TCHAR* pText = GetTxtFromMgr(1163);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	if ( m_clUserItem[ m_siUserInventorySelectedRealItemIndex ].GetItemNum() == m_siWareHouseInputOutputItemNum )
	{
		m_siUserInventorySelectedIndex = -1;
	}

	m_clUserItem[ m_siUserInventorySelectedRealItemIndex ].SetItemNum( m_siWareHouseInputOutputItemNum);
	m_pHouseUnitBuffer->Push( HOUSEUNIT_INPUT_USERINVENTORY_TO_WAREHOUSE, m_siUserInventorySelectedRealItemIndex, m_siCurrentVillageIndex, m_siCurrentHouseIndex, m_clUserItem[ m_siUserInventorySelectedRealItemIndex ] );		//여기서 i는 유저의 아이템 위치임

	/*
	if ( m_siUserInventorySelectedIndex == -1 )
	{
		m_siUserInventorySelectedRealItemIndex = -1;
	}
	*/

		return;
}

void CNHouseUserItemDlg::OutputToWareHouse()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ].GetItemNum() < m_siWareHouseInputOutputItemNum )
	{
		TCHAR* pTitle = GetTxtFromMgr(2144);
		TCHAR* pText = GetTxtFromMgr(1163);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}

	if ( m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ].GetItemNum() == m_siWareHouseInputOutputItemNum )
	{
		m_siWareHouseInventorySelectedIndex = -1;
	}

	m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ].SetItemNum(m_siWareHouseInputOutputItemNum);
	m_pHouseUnitBuffer->Push( HOUSEUNIT_OUTPUT_USERINVENTORY_FROM_WAREHOUSE, m_siWareHouseInventorySelectedRealItemIndex, m_siCurrentVillageIndex, m_siCurrentHouseIndex, m_clWareHouseItem[ m_siWareHouseInventorySelectedRealItemIndex ] );		//여기서 i는 유저의 아이템 위치임


	return;
}

