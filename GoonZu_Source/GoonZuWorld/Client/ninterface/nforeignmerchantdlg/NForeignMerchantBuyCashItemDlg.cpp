#include "../Client/NInterface/NForeignMerchantDlg/NForeignMerchantBuyCashItemDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPCBuyCashItem.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"
#include "..\..\..\CommonLogic\CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNForeignMerchantBuyCashItemDlg::CNForeignMerchantBuyCashItemDlg() : CDialogController()
{
	m_siImageType = 0;	// 무기 이미지 종류
	m_siImageIndex = 0;	// 무기 이미지 인덱스
	
	
	m_pclItem = new cltItem[ PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON ];
	m_siItemUniqueList = new SI16[ MAX_AVAILABLE_CASHITEMNUM ];
	m_uiItemPriceList = new UI16[ MAX_AVAILABLE_CASHITEMNUM ];

	memset( m_siItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_uiItemPriceList, 0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );

	m_pForeignMerchantNPC = NULL;

	m_siSelectedIndex = -1;
	m_siListBoxSelectedIndex = -1;
	m_siComboBoxSelectedIndex = -1;
	m_siListNum = 0;
	m_siForeignMerchantKind = -1;
	m_siItemType = -1;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pStatic[3] = NULL;
	m_pStatic[4] = NULL;
	m_pStatic[5] = NULL;
	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;
	m_pCombo = NULL;
	m_pList = NULL;
	m_pListView = NULL;
	m_pButtonSellItem = NULL;
	m_pButtonClose = NULL;
}

CNForeignMerchantBuyCashItemDlg::~CNForeignMerchantBuyCashItemDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE_A(m_siItemUniqueList);
	SAFE_DELETE_A(m_uiItemPriceList);
	SAFE_DELETE_A(m_pclItem);
	
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);
	SAFE_DELETE(m_pStatic[4]);
	SAFE_DELETE(m_pStatic[5]);
	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);
	SAFE_DELETE(m_pCombo);
	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pListView);
	SAFE_DELETE(m_pButtonSellItem);
	SAFE_DELETE(m_pButtonClose);
}

void CNForeignMerchantBuyCashItemDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NForeignMerchantDlg/DLG_ForeignMerchant_Normal.ddf"));
		file.CreatePopupDialog( this, NFOREIGNMERCHANTBUYCASHITEM_DLG, TEXT("dialog_ForeignMerchant_Normal"), StaticCallBackDialogNForeignMerchantBuyCashItemDlg );

		TCHAR* pText = GetTxtFromMgr(3388);
		SetTitle(pText);

		m_pStatic[0]		  = new CStatic( this );
		m_pStatic[1]		  = new CStatic( this );
		m_pStatic[2]		  = new CStatic( this );
		m_pStatic[3]		  = new CStatic( this );
		m_pStatic[4]		  = new CStatic( this );
		m_pStatic[5]		  = new CStatic( this );
		m_pEdit[0]			  = new CEdit( this );
		m_pEdit[1]			  = new CEdit( this );
		m_pCombo			  = new CComboBox( this );
		m_pList				  = new CList( this );
		m_pListView			  = new CListView( this );
		m_pButtonSellItem = new CButton( this );
		m_pButtonClose  = new CButton( this );

		file.CreateControl( m_pStatic[0], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[1], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[2], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
		m_pStatic[2]->SetBKColorUse( true );
		m_pStatic[2]->SetBorder( true );
		m_pStatic[2]->Refresh();
		file.CreateControl( m_pStatic[3], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
		file.CreateControl( m_pStatic[4], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC5, TEXT("statictext_NONAME5") );
		file.CreateControl( m_pStatic[5], NFOREIGNMERCHANTBUYITEM_DIALOG_STATIC6, TEXT("statictext_NONAME6") );

		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash(TEXT("KIND_CHINAMERCHANT")) - 1);

		file.CreateControl( m_pEdit[0], NFOREIGNMERCHANTBUYITEM_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pEdit[1], NFOREIGNMERCHANTBUYITEM_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

		file.CreateControl( m_pList, NFOREIGNMERCHANTBUYITEM_DIALOG_LIST, TEXT("listbox_NONAME1") );
		file.CreateControl( m_pListView, NFOREIGNMERCHANTBUYITEM_DIALOG_LISTVIEW, TEXT("listview_NONAME1") );

		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();

		m_pListView->SetBKColorUse( true );
		m_pListView->SetBorder( true );

		file.CreateControl( m_pButtonSellItem, NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonClose, NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		EnableButton(m_pButtonSellItem, false);

		file.CreateControl( m_pCombo, NFOREIGNMERCHANTBUYITEM_DIALOG_COMBOBOX , TEXT("combobox_NONAME1") );

		SetTimerOnOff(true);

		SetCashItemBuyMode();
	}
	else
	{
		DeleteDialog();
	}
}

void CNForeignMerchantBuyCashItemDlg::Action()
{
	if ( IsUserInventoryChanged() )
	{
		LoadUserInventory();
	}
}


void CALLBACK CNForeignMerchantBuyCashItemDlg::StaticCallBackDialogNForeignMerchantBuyCashItemDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNForeignMerchantBuyCashItemDlg *pThis = (CNForeignMerchantBuyCashItemDlg *)pControllerMgr;
	pThis->CallBackDialogNForeignMerchantBuyCashItemDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNForeignMerchantBuyCashItemDlg::CallBackDialogNForeignMerchantBuyCashItemDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NFOREIGNMERCHANTBUYITEM_DIALOG_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32 SelectIndex = m_pCombo->GetSelectedIndex();
				
					if(SelectIndex < 0)
						break;

					m_siComboBoxSelectedIndex = SelectIndex;
				//	m_siForeignMerchantKind = SelectedIndex;
				
					m_siItemType = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( m_pCombo->GetText(m_siComboBoxSelectedIndex) );

					if ( m_siItemType > 0 )
					{
						// 현재 교역 상인이 구입중인 물품에 대한 정보를 요청함.
						cltGameMsgRequest_ForeignMerchantBuyCashItemInfo cltameMsgRequest_ForeignMerchantBuyCashItemInfo( m_siItemType );
						cltMsg clMsg(GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEMINFO, sizeof(cltGameMsgRequest_ForeignMerchantBuyCashItemInfo), (BYTE*)&cltameMsgRequest_ForeignMerchantBuyCashItemInfo);
						pclClient->SendMsgToServer((sPacketHeader*) &clMsg );

						LoadUserInventory();
					}

				}
				break;
			}
		}
		break;

	case NFOREIGNMERCHANTBUYITEM_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					m_siListBoxSelectedIndex = m_pList->GetSelectedIndex();

					if ( m_siListBoxSelectedIndex > -1 )
					{
						SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(m_pList->GetText(m_siListBoxSelectedIndex, 0));

						if(itemunique)
						{
							cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

							if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) )
							{}
							else return;
						}
						if( 105 == m_siImageType)
						{
							m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg0.SPR"));
						}
						else if( 106 == m_siImageType)
						{
							m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg1.SPR"));
						}
						else if( 107 == m_siImageType)
						{
							m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg2.SPR"));
						}

						m_pStatic[3]->SetFontIndex(m_siImageIndex);
					}
				}
				break;
			}
		}
		break;

	case NFOREIGNMERCHANTBUYITEM_DIALOG_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					m_siSelectedIndex = m_pListView->GetSelectedItemIndex();
					if(m_siSelectedIndex >= 0)
					{
						TCHAR ItemName[ 128 ] = TEXT("");

						m_pclItem[m_siSelectedIndex].GetFullName( pclClient->pclItemManager, ItemName , 128);
						m_pEdit[1]->SetText(ItemName);

						EnableButton(m_pButtonSellItem, TRUE);
					}
					else
					{
						m_pEdit[1]->SetText(TEXT(""));
						EnableButton(m_pButtonSellItem, FALSE);
					}
				}
				break;
			}
		}
		break;

	case NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON1:	//  물건 팔기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 팔수 있는 물건은 한개
					UI16 uiSellItemNum = 1; // _tstoi(m_pEdit[1]->GetText());

				//	if ( uiSellItemNum < 1 )
				//	{
				//		TCHAR* pText = GetTxtFromMgr(1861);
				//		TCHAR* pTitle = GetTxtFromMgr(1862);
				//		MessageBox( NULL, pText, pTitle, MB_OK|MB_TOPMOST );
				//		break;
				//	}

				//	if ( uiSellItemNum > 10 )
				//	{
				//		TCHAR* pText = GetTxtFromMgr(1863);
				//		TCHAR* pTitle = GetTxtFromMgr(1864);
				//		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, pTitle );
				//		return;
				//	}

					if ( m_siSelectedIndex < 0 )
					{
				//		TCHAR* pText = GetTxtFromMgr(1865);
				//		TCHAR* pTitle = GetTxtFromMgr(1866);
				//		MessageBox( NULL, pText, pTitle, MB_OK|MB_TOPMOST );
						break;
					}

					bool Find = false;
					for ( SI16 i = 0; i < m_siListNum; ++i )
					{
						if ( m_pclItem[ m_siSelectedIndex ].siUnique == m_siItemUniqueList[ i ] )
						{
							Find = true;
							break;
						}
					}

					if ( ! Find )
					{
						TCHAR* pText = GetTxtFromMgr(1867);
						TCHAR* pTitle = GetTxtFromMgr(1868);
						MessageBox( NULL, pText, pTitle, MB_OK|MB_TOPMOST );
						return;
					}

					cltItem clItem;
					clItem.Set( &m_pclItem[ m_siSelectedIndex ] );
					clItem.siItemNum = uiSellItemNum;

					cltGameMsgRequest_ForeignMerchantBuyCashItem clGameMsgRequest_ForeignMerchantBuyCashItem( m_siSelectedIndex, &clItem );
					cltMsg clMsg( GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM, sizeof(clGameMsgRequest_ForeignMerchantBuyCashItem), (BYTE*)&clGameMsgRequest_ForeignMerchantBuyCashItem );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
				}
				break;
			}
		}
		break;

	case NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON2:	// 닫기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}

}

void CNForeignMerchantBuyCashItemDlg::SetCashItemBuyMode()
{
	cltClient *pclclient = (cltClient *)pclClient;
//	pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY ), LVSIL_NORMAL );

	TCHAR ItemTypeCodeList[ MAX_ITEMTYPE_NUMBER ][ 64 ];
	pclClient->pclForeignMerchantNPCBuyCashItem->GetItemTypeCodeList( (TCHAR*)&ItemTypeCodeList[0] );

	TCHAR* pText = GetTxtFromMgr(1860);
//	SetWindowText( GetDlgItem( hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN ), pText);
	m_pEdit[0]->SetText(pText);

	for ( SI16 i = 0; i < MAX_ITEMTYPE_NUMBER; ++i )
	{
		if(  ItemTypeCodeList[ i ] && _tcslen( ItemTypeCodeList[ i ] ) > 0 )
		{
			SI32 ItemTypeUnique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeCode( ItemTypeCodeList[ i ] );
			if ( ItemTypeUnique > 0 )
			{
				stComboBoxItemData Data;
				Data.Set(pclClient->pclItemManager->pclItemTypeInfo[ ItemTypeUnique ]->szName);
				m_pCombo->AddItem(&Data);
			//	SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT ), CB_ADDSTRING, 0, (LPARAM)pclClient->pclItemManager->pclItemTypeInfo[ ItemTypeUnique ]->szName );
			}
		}
	}

	pText = GetTxtFromMgr(1227);
//	SendMessage( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTCOLUMN, 0, (LPARAM)&Col );
	m_pList->SetColumn(0, 112, pText);

	pText = GetTxtFromMgr(1678);
//	SendMessage( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTCOLUMN, 1, (LPARAM)&Col );
	m_pList->SetColumn(1, 112, pText);

	m_pList->Refresh();

	m_siListBoxSelectedIndex = -1;
}

bool CNForeignMerchantBuyCashItemDlg::IsUserInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclItem[ j ];
		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

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

void CNForeignMerchantBuyCashItemDlg::LoadUserInventory()
{
	//	m_pListView->DeleteAllItems();
	m_siSelectedIndex = -1;

	for ( int i = PERSONITEM_INV0; i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++i )
	{
		m_pclItem[ i ] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}

	SI32 index = 0;
	TCHAR itemname[128];

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;
	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	if(m_siItemType > 0)
	{
		for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
		{
			if(m_pclItem[i].siUnique)
			{
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_pclItem[i].siUnique );

				if(ref > 0)
				{
					SI32 ItemType1 = pclClient->pclItemManager->pclItemInfo[ref]->siType;

					if ( ItemType1 == m_siItemType )
					{	
						if(m_pclItem[i].GetFullName(pclClient->pclItemManager, itemname, 128) == true)
						{
							if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
							{
								TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
								pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ToolTipMsg );

								stListViewItemData ListViewItemData;
								ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos , m_pclItem[i].GetItemNum(), itemname, ToolTipMsg );														
								m_pListView->InsertItem( &ListViewItemData );
							}
						}
					}
				}
			}
		}
	}
}

void CNForeignMerchantBuyCashItemDlg::Set( stItemInfoList *ItemInfoList, SI16 ListNum )
{
//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ) );
	m_pList->Clear();

	SI32 i = 0;
	for ( i = 0; i < ListNum; ++i )
	{
		m_siItemUniqueList[ i ] = ItemInfoList[ i ].siBuyItemUniqueList;
		m_uiItemPriceList[ i ] = ItemInfoList[ i ].uiBuyItemPriceList;
	}

	m_siListNum = ListNum;

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	

	Li.iItem = 0;	
	Li.iSubItem = 0;

	SI16 Count = 0;

	TCHAR Buffer[ 1024 ];

	for ( i = 0; i < ListNum; ++i )
	{
		TCHAR *ItemName = ( TCHAR * )pclClient->pclItemManager->GetName( m_siItemUniqueList[ i ] );

		if ( ItemName == NULL )
		{
			continue;
		}

		stListBoxItemData Data;

		Li.iItem = Count++;
		Li.iSubItem = 0;
		Li.pszText = ItemName;

		Data.Set(0, Li.pszText);
	//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTITEM, 0, (LPARAM)&Li );

		Li.iSubItem = 1;
		StringCchPrintf( Buffer, 1024, TEXT("%d"), m_uiItemPriceList[ i ] );
		Li.pszText = Buffer;			
		Data.Set(1, Li.pszText);
	//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_SETITEM, 0, (LPARAM)&Li );

		m_pList->AddItem(&Data);
		
		if ( i == 0 )
		{
			m_siListBoxSelectedIndex = 0;
			m_pList->SetSelectIndex(m_siListBoxSelectedIndex);

			SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(m_pList->GetText(m_siListBoxSelectedIndex, 0));

			if(itemunique)
			{
				cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

				if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) )
				{}
				else return;
			}
			if( 105 == m_siImageType)
			{
				m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg0.SPR"));
			}
			else if( 106 == m_siImageType)
			{
				m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg1.SPR"));
			}
			else if( 107 == m_siImageType)
			{
				m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg2.SPR"));
			}

			m_pStatic[3]->SetFontIndex(m_siImageIndex);
		}
	}
	
	m_pList->Refresh();
	m_pList->SetSelectIndex(0);
}

void CNForeignMerchantBuyCashItemDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	TCHAR* pText = GetTxtFromMgr(3389);
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(false);
	}
}
