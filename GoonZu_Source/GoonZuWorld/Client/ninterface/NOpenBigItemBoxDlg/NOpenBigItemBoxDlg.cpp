#include "NOpenBigItemBoxDlg.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "Char/CharManager/CharManager.h"
#include "Char/CharCommon/Char-Common.h"
#include "Char/CharClient/Char-Client.h"

#include "../../../Client/Client.h"

#include "../../../CommonLogic/MsgType-Item.h"

#include "../NNewInventory/NNewInventory.h"

extern cltCommonLogic* pclClient;

CNOpenBigItemBoxDlg::CNOpenBigItemBoxDlg()
: m_siBoxItemPos(-1),
  m_siKeyItemPos(-1)
{
	m_UpdateTimer_SendMsg.Init(3000);
	m_UpdateTimer_Twinkle.Init(200);

	Init();
}

CNOpenBigItemBoxDlg::~CNOpenBigItemBoxDlg()
{
	Destroy();
}

void CNOpenBigItemBoxDlg::Init()
{
}

void CNOpenBigItemBoxDlg::Destroy()
{

}

void CNOpenBigItemBoxDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile	file;

		file.LoadFile(TEXT("NInterface/Data/NOpenBigItemBox/DLG_NOpenBigItemBox.ddf"));
		file.CreatePopupDialog( this, NOPENBIGITEMBOX_DLG, TEXT("dialog_openbigitembox"),StaticOpenBigItemBoxDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT ,		NOPENBIGITEMBOX_EDITBOX_GREETING, this) ,			NOPENBIGITEMBOX_EDITBOX_GREETING, TEXT("editbox_greeting"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW ,	NOPENBIGITEMBOX_LISTVIEW_BIGITEMBOX, this) ,		NOPENBIGITEMBOX_LISTVIEW_BIGITEMBOX, TEXT("listview_bigitembox"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW ,	NOPENBIGITEMBOX_LISTVIEW_KEYITEM, this) ,			NOPENBIGITEMBOX_LISTVIEW_KEYITEM, TEXT("listview_keyitem"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,	NOPENBIGITEMBOX_STATICTEXT_BIGITEMBOXCOUNT, this) ,	NOPENBIGITEMBOX_STATICTEXT_BIGITEMBOXCOUNT, TEXT("statictext_bigitemboxcount"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,	NOPENBIGITEMBOX_STATICTEXT_KEYITEMCOUNT, this) ,	NOPENBIGITEMBOX_STATICTEXT_KEYITEMCOUNT, TEXT("statictext_keyitemcount"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE ,	NOPENBIGITEMBOX_OUTLINE_NONAME1, this) ,			NOPENBIGITEMBOX_OUTLINE_NONAME1, TEXT("outline_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,	NOPENBIGITEMBOX_BUTTON_OPENBIGITEM, this) ,			NOPENBIGITEMBOX_BUTTON_OPENBIGITEM, TEXT("button_openbigitem"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM, this) ,		NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM, TEXT("imagestatic_ResultItem"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE ,	NOPENBIGITEMBOX_OUTLINE_NONAME2, this) ,			NOPENBIGITEMBOX_OUTLINE_NONAME2, TEXT("outline_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,	NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY, this) ,	NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY, TEXT("statictext_resultdisplay"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,NOPENBIGITEMBOX_IMAGESTATIC_MARK, this) ,			NOPENBIGITEMBOX_IMAGESTATIC_MARK, TEXT("imagestatic_mark"));

		m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFileName(TEXT("GImg\\TREASUREBOX2.SPR"));
		m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFontIndex(0);

		m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_BIGITEMBOXCOUNT)->SetTextPrintOption(DT_CENTER | DT_TOP | DT_SINGLELINE);
		m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_BIGITEMBOXCOUNT)->Refresh();

		NTCHARString256	kText(GetTxtFromMgr(8115));
		m_InterfaceFactory.GetEdit( NOPENBIGITEMBOX_EDITBOX_GREETING )->SetText(kText);

		CDialogController::SetTimerOnOff( true );
		CDialogController::SetActionTimer(100);

		TCHAR* pText = GetTxtFromMgr(6632);
		m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY)->SetText(pText);

		CListView* pclListView_BigItemBox = m_InterfaceFactory.GetListView(NOPENBIGITEMBOX_LISTVIEW_BIGITEMBOX);
		if( pclListView_BigItemBox )		pclListView_BigItemBox->SetUseToolTip(false);
 
		CListView* pclListView_KeyItem = m_InterfaceFactory.GetListView(NOPENBIGITEMBOX_LISTVIEW_KEYITEM);
		if( pclListView_KeyItem )			pclListView_KeyItem->SetUseToolTip(false);

		Show( true );
	}
}

void CALLBACK CNOpenBigItemBoxDlg::StaticOpenBigItemBoxDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNOpenBigItemBoxDlg *pThis = (CNOpenBigItemBoxDlg*) pControllerMgr;
	pThis->NOpenBigItemBoxDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNOpenBigItemBoxDlg::NOpenBigItemBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NOPENBIGITEMBOX_BUTTON_OPENBIGITEM:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_siBoxItemPos <= 0 || m_siKeyItemPos <= 0)			return;

					TCHAR* pText = GetTxtFromMgr(6632);
					m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY)->SetText(pText);

					m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFileName(TEXT("GImg\\TREASUREBOX2.SPR"));
					m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFontIndex(0);

					m_UpdateTimer_SendMsg.SetActive(true, ::GetTickCount());
					m_UpdateTimer_Twinkle.SetActive(true, ::GetTickCount());
				}
				break;
			}
		}
		break;
	case NOPENBIGITEMBOX_LISTVIEW_KEYITEM:
		{
			switch(nEvent)
			{
			case EVENT_LISTVIEW_RBUTTONUP:
				{
					CListView* pclListView_KeyItem = m_InterfaceFactory.GetListView(NOPENBIGITEMBOX_LISTVIEW_KEYITEM);
					if( pclListView_KeyItem)			pclListView_KeyItem->DeleteAllItems();
				}
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CNOpenBigItemBoxDlg::Action()
{
	if( m_UpdateTimer_Twinkle.IsActivated() &&
		m_UpdateTimer_Twinkle.IsTimed(::GetTickCount(), true) )
	{
		SI16 siFontIndex = (m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM))->GetFontIndex();

		siFontIndex++;
		if ( siFontIndex >= 2 )	siFontIndex = 0;

		(m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM))->SetFontIndex(siFontIndex);
	}

	if( m_UpdateTimer_SendMsg.IsActivated() &&
		m_UpdateTimer_SendMsg.IsTimed(::GetTickCount(), false) )
	{
		cltCharClient* pclcharclient = (cltCharClient*)pclClient->pclCM->CR[1];

		cltItem* pclBoxItem = &pclcharclient->pclCI->clCharItem.clItem[m_siBoxItemPos];
		cltItem* pclKeyItem = &pclcharclient->pclCI->clCharItem.clItem[m_siKeyItemPos];

		if( pclBoxItem == NULL )		return;
		if( pclKeyItem == NULL )		return;

		if( pclBoxItem->siUnique != ITEMUNIQUE(13325) )
			return;

		bool bIsKey = false;

		if( pclKeyItem->siUnique == ITEMUNIQUE(8113)  ||
			pclKeyItem->siUnique == ITEMUNIQUE(13621) )
		{
			bIsKey = true;
		}

		if( bIsKey == false )
			return;

		cltItem clUseBoxItem;
		cltItem clUseKeyItem;

		clUseBoxItem.Set(pclBoxItem);
		clUseKeyItem.Set(pclKeyItem);

		clUseBoxItem.SetItemNum(1);
		clUseKeyItem.SetItemNum(1);

		cltGameMsgRequest_OpenBigItemBox	clOpenBigBoxMsg(m_siBoxItemPos, &clUseBoxItem, m_siKeyItemPos, &clUseKeyItem);
		cltMsg clMsg(GAMEMSG_REQUEST_OPENBIGITEMBOX, sizeof(cltGameMsgRequest_OpenBigItemBox), (BYTE*)&clOpenBigBoxMsg);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

bool CNOpenBigItemBoxDlg::SetBigItemBox( SI32 siPos, cltItem* pclItem )
{
	CListView* pclListView_BigItemBox = m_InterfaceFactory.GetListView(NOPENBIGITEMBOX_LISTVIEW_BIGITEMBOX);
	if( pclListView_BigItemBox == NULL)			return false;

	pclListView_BigItemBox->DeleteAllItems();

	cltClient* pclclient = (cltClient*)pclClient;

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	CListView*	pInventoryListView = NULL;

	if( siPos >= PERSONITEM_INV0 && siPos < PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY )
	{
		pInventoryListView = personinventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_HORSE0 && siPos < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY )
	{
		pInventoryListView = horseinventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_BAG0 && siPos < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG )
	{
		pInventoryListView = bag0inventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_BAG1 && siPos < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
	{
		pInventoryListView = bag1inventory->m_pInventoryListView;
	}

	if( pclItem->siUnique == ITEMUNIQUE(13325) )
	{
		SI32 GlobalImageIndex = -1;
		SI32 GlobalImageFont = -1;

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont ))
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclItem->siUnique );

			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, pclItem->siItemNum, ItemName, TEXT(""), false );
			pclListView_BigItemBox->InsertItem( &ListViewItemData );

			m_siBoxItemPos = siPos;

			//if(pInventoryListView)		pInventoryListView->SetItemDrawAlphaOption(siPos);
		}
		return true;
	}

	return false;
}

bool CNOpenBigItemBoxDlg::SetKeyItem( SI32 siPos, cltItem* pclItem )
{
	cltClient* pclclient = (cltClient*)pclClient;

	CListView* pclListView_KeyItem = m_InterfaceFactory.GetListView(NOPENBIGITEMBOX_LISTVIEW_KEYITEM);
	if( pclListView_KeyItem == NULL )			return false;

	pclListView_KeyItem->DeleteAllItems();

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	CListView*	pInventoryListView = NULL;

	if( siPos >= PERSONITEM_INV0 && siPos < PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY )
	{
		pInventoryListView = personinventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_HORSE0 && siPos < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY )
	{
		pInventoryListView = horseinventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_BAG0 && siPos < PERSONITEM_BAG0 + MAX_ITEM_PER_BAG )
	{
		pInventoryListView = bag0inventory->m_pInventoryListView;
	}
	else if( siPos >= PERSONITEM_BAG1 && siPos < PERSONITEM_BAG1 + MAX_ITEM_PER_BAG )
	{
		pInventoryListView = bag1inventory->m_pInventoryListView;
	}

	if( pclItem->siUnique == ITEMUNIQUE(8113) ||
		pclItem->siUnique == ITEMUNIQUE(13621) )
	{
		SI32 GlobalImageIndex = -1;
		SI32 GlobalImageFont = -1;

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont ))
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclItem->siUnique );

			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, pclItem->siItemNum, ItemName, TEXT(""), false );
			pclListView_KeyItem->InsertItem( &ListViewItemData );

			m_siKeyItemPos = siPos;

			//if(pInventoryListView)		pInventoryListView->SetItemDrawAlphaOption(siPos);
		}

		return true;
	}

	return false;
}

bool CNOpenBigItemBoxDlg::SetResult( SI32 Unique, SI32 Number )
{
	m_UpdateTimer_Twinkle.SetActive( false, ::GetTickCount() );

	if( Unique <= 0 && Number <= 0 )
	{
		m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFileName(TEXT("GImg\\TREASUREBOX2.SPR"));
		m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM)->SetFontIndex(1);

		NTCHARString128	kText(GetTxtFromMgr(6634));
		(m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY))->SetText(kText);
		return true;
	}

	TCHAR buffer[256];

	TCHAR* pText = GetTxtFromMgr(6633);

	StringCchPrintf( buffer, 256, pText, pclClient->pclItemManager->GetName(Unique), Number);
	(m_InterfaceFactory.GetStatic(NOPENBIGITEMBOX_STATICTEXT_RESULTDISPLAY))->SetText(buffer);
 
	SI32 itemTypeIndex = 0;
	SI32 itemFontIndex = 0;

	pclClient->pclItemManager->GetItemGImageFileIndexAndFont( Unique, &itemTypeIndex, &itemFontIndex );

	(m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM))->SetItemSprType(itemTypeIndex);
	(m_InterfaceFactory.GetImageStatic(NOPENBIGITEMBOX_IMAGESTATIC_RESULTITEM))->SetFontIndex(itemFontIndex);

	// 박스와 키 아이템 재 설정
	SetBigItemBox( m_siBoxItemPos, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_siBoxItemPos]);
	SetKeyItem( m_siKeyItemPos, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_siKeyItemPos]);

	return true;
}

bool CNOpenBigItemBoxDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	CListView*	pInventoryListView = NULL;

	switch ( pclClickAndDrop_Item->m_siSrcDialogID )
	{
	case NNEWINVENTORY_DLG:			pInventoryListView = personinventory->m_pInventoryListView;		break;
	case NNEWINVENTORY_HORSE_DLG:	pInventoryListView = horseinventory->m_pInventoryListView;		break;
	case NNEWINVENTORY_BAG0_DLG:	pInventoryListView = bag0inventory->m_pInventoryListView;		break;						
	case NNEWINVENTORY_BAG1_DLG:	pInventoryListView = bag1inventory->m_pInventoryListView;		break;
	}

	switch(pclClickAndDrop_Item->m_siSrcDialogID)
	{
	// 인벤토리에서 온경우
	case NNEWINVENTORY_DLG:
	case NNEWINVENTORY_HORSE_DLG:
	case NNEWINVENTORY_BAG0_DLG:
	case NNEWINVENTORY_BAG1_DLG:
		{ 
			cltItem* pclItem = NULL;
			if( pclClickAndDrop_Item->m_siPara1 >= PERSONITEM_INV0 && pclClickAndDrop_Item->m_siPara1 < MAX_ITEM_PER_PERSON )
				pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1];

			if(pclItem == NULL)			return false;

			switch(pclClickAndDrop_Item->m_siDstControlID)
			{
			case NOPENBIGITEMBOX_LISTVIEW_BIGITEMBOX:
				{
					return true;
				}
				break;
			case NOPENBIGITEMBOX_LISTVIEW_KEYITEM:
				{
					SetKeyItem( pclClickAndDrop_Item->m_siPara1, pclItem);
					return true;
				}
				break;
			}
		}
		break;
	}

	return false;
}