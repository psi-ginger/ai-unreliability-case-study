#include "../Client/NInterface/NForeignMerchantDlg/NForeignMerchantBuyItemDlg.h"

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
#include "../CommonLogic/Msg/MsgType-Quest.h"

#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"
#include "..\..\..\CommonLogic\CommonLogic.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNForeignMerchantBuyItemDlg::CNForeignMerchantBuyItemDlg() : CDialogController()
{
	m_siImageType = 0;	// 무기 이미지 종류
	m_siImageIndex = 0;	// 무기 이미지 인덱스
	m_pPersonItemInfo = NULL;
	m_pForeignMerchantMgr = NULL;
	m_siSelectedIndex = -1;
	m_bIsListRotate = false;	
	m_si32ImageListIndex = 0;
	m_siForeignMerchantKind = -1;

	m_pForeignMerchantNPC = NULL;
	memset( &m_ForeignMerchantNPCInfo, 0, sizeof( m_ForeignMerchantNPCInfo ) );

	m_siNPCCharUnique = 0;	// 그냥 값을 넣어줬음

	m_siItemType = 0;

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

CNForeignMerchantBuyItemDlg::~CNForeignMerchantBuyItemDlg()
{
	SetTimerOnOff(false);

	SAFE_DELETE(m_pForeignMerchantMgr);
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

void CNForeignMerchantBuyItemDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;
					   
		file.LoadFile(TEXT("NInterface/Data/NForeignMerchantDlg/DLG_ForeignMerchant_Normal.ddf"));
		file.CreatePopupDialog( this, NFOREIGNMERCHANTBUYITEM_DLG, TEXT("dialog_ForeignMerchant_Normal"), StaticCallBackDialogNForeignMerchantBuyItemDlg );

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
		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();

		file.CreateControl( m_pListView, NFOREIGNMERCHANTBUYITEM_DIALOG_LISTVIEW, TEXT("listview_NONAME1") );
		m_pListView->SetBKColorUse( true );
		m_pListView->SetBorder( true );

		file.CreateControl( m_pButtonSellItem, NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonClose, NFOREIGNMERCHANTBUYITEM_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		
		EnableButton(m_pButtonSellItem, FALSE);

		file.CreateControl( m_pCombo, NFOREIGNMERCHANTBUYITEM_DIALOG_COMBOBOX , TEXT("combobox_NONAME1") );
	}
	else
	{
		DeleteDialog();
	}
}

void CNForeignMerchantBuyItemDlg::Action()
{
	if ( IsInventoryChanged() )
	{
		LoadInventoryData();
	}

	if ( IsNPCBuyItemInfoChanged() )
	{
		LoadInventoryData();
		LoadForeignNPCData();
	}
	
}


void CALLBACK CNForeignMerchantBuyItemDlg::StaticCallBackDialogNForeignMerchantBuyItemDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNForeignMerchantBuyItemDlg *pThis = (CNForeignMerchantBuyItemDlg *)pControllerMgr;
	pThis->CallBackDialogNForeignMerchantBuyItemDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNForeignMerchantBuyItemDlg::CallBackDialogNForeignMerchantBuyItemDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
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
					SI32 SelectedIndex = m_pCombo->GetSelectedIndex();

					if ( SelectedIndex < 0 )
					{
						return ;
					}


					m_pList->Clear();
					m_pList->Refresh();

					//빈칸이미지
					m_pStatic[3]->Show( false );

					
					m_siForeignMerchantKind = SelectedIndex;

					// 현재 교역 상인이 구입중인 물품에 대한 정보를 요청함.
					cltGameMsgRequest_ForeignMerchantNPCBuyInfo clForeignMerchantNPCBuyInfo( m_siForeignMerchantKind );
					cltMsg clMsg(GAMEMSG_REQUEST_FOREIGNMERCHANTNPCBUYINFO, sizeof(cltGameMsgRequest_ForeignMerchantNPCBuyInfo), (BYTE*)&clForeignMerchantNPCBuyInfo);
					pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
				
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
						//if( GIMG_ITEMIMG0 == m_siImageType)
						//{
						//	m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg0.SPR"));
						//}
						//else if( GIMG_ITEMIMG1 == m_siImageType)
						//{
						//	m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg1.SPR"));
						//}
						//else if( GIMG_ITEMIMG2 == m_siImageType)
						//{
						//	m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg2.SPR"));
						//}
						//else if( GIMG_ITEMIMG3 == m_siImageType)
						//{
						//	m_pStatic[3]->SetFileName(TEXT("GImg/ItemImg3.SPR"));
						//}

                        m_pStatic[3]->SetItemSprType( m_siImageType );
						m_pStatic[3]->SetFontIndex(m_siImageIndex);
						m_pStatic[3]->Show( true );
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

						m_clItem[m_siSelectedIndex].GetFullName( pclClient->pclItemManager, ItemName, 128 );
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
					if ( m_siSelectedIndex < 0 )
					{
						break;
					}

					// 명나라는 1개밖에 못 산다
					cltGameMsgRequest_SellToForeignMerchant clPreSellToForeignMerchant(m_siForeignMerchantKind, 
									(UI16) m_siSelectedIndex, 1, m_clItem[ m_siSelectedIndex ].siUnique, m_siNPCCharUnique,
									(UI16) m_clItem[m_siSelectedIndex].clItemCommon.clCommonInfo.uiRareInfo);

					cltMsg clMsg(GAMEMSG_REQUEST_PRESELLTOFOREIGNMERCHANT,
						sizeof(cltGameMsgRequest_SellToForeignMerchant), (BYTE*)&clPreSellToForeignMerchant);

					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					m_pEdit[1]->SetText(TEXT(""));
					EnableButton(m_pButtonSellItem, FALSE);
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

void CNForeignMerchantBuyItemDlg::SetNormalItemBuyMode(SI32 NPCCharUnique)
{
	m_siNPCCharUnique = NPCCharUnique;

	m_pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

	if ( m_pForeignMerchantMgr == NULL )
	{
		m_pForeignMerchantMgr = new CLibListMgr();
		m_pForeignMerchantMgr->CreateMaxEmptyObj( MAX_FOREIGN_MERCHANT_OBJ_NUM, MAX_FOREIGN_MERCHANT_OBJ_BUFFER_SIZE );
	}

    stComboBoxItemData Data;
	TCHAR* pText = NULL;

	pText = GetTxtFromMgr(1024);
  	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1026);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2370);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2366);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2376);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2368);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2374);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2372);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1030);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1029);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2398);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2399);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1028);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1027);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1025);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(1031);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2470);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	pText = GetTxtFromMgr(2471);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);

	/*cyj 명상인 대포 팔지않도록 삭제
	pText = GetTxtFromMgr(2378);
	Data.Set(pText);
	m_pCombo->AddItem(&Data);
	m_pCombo->Refresh();
	*/

//	ListView_SetExtendedListViewStyle( GetDlgItem(hDlg, IDC_LIST_USER_INVENTORY ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
//	ListView_SetExtendedListViewStyle( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );

	pText = GetTxtFromMgr(1227);
	m_pList->SetColumn(0, 112, pText);

	pText = GetTxtFromMgr(1678);
	m_pList->SetColumn(1, 112, pText);

	pText = GetTxtFromMgr(2472);
	m_pEdit[0]->SetText(pText);

//	m_pList->Clear();
	m_pList->Refresh();

//	m_siForeignMerchantKind = -1;
	LoadInventoryData();

	SetTimerOnOff(true);
}

void CNForeignMerchantBuyItemDlg::LoadInventoryData()
{
//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_USER_INVENTORY ) );
//	m_pListView->DeleteAllItems();
//	m_pListView->SetStartItemPos(0);
	m_pListView->DeleteAllItems();

	m_siSelectedIndex = -1;

	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_clItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	}

	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );	
	if (m_pForeignMerchantNPC == NULL) 
	{
		return;
	}
	
	m_pForeignMerchantNPC->GetForeignMerchantNPCInfo( &m_ForeignMerchantNPCInfo );
	if ( m_ForeignMerchantNPCInfo.siBuyItemListCount > 0 )
	{
		cltItem clItem;
		clItem.Init();
		clItem.siUnique = m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ 0 ];
		clItem.siItemNum = 1;

		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( clItem.siUnique );

		if(ref > 0)
		{
			SI32 ItemType = pclClient->pclItemManager->pclItemInfo[ref]->siType;
			if ( ItemType > 0 )
			{
			//	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_USER_INVENTORY, NULL, m_clItem, ItemType );
			//	(cltItemManagerCommon* pclitemmanager, HWND hDlg, SI32 ndlgitem, cltImageList* pclImageList, cltItem* pclItem, SI32 ItemType )

				SI32 i;
			//	TCHAR buffer[256];

				SI32 index = 0;
				TCHAR itemname[128];

				SI32 GlobalImageIndex = -1;
				SI32 GlobalImageFont = -1;
				SI32 StartPos=0;
				SI32 id = 1;
				m_pListView->DeleteAllItems();
				m_pListView->SetStartItemPos( StartPos );


				for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; i++ )
				{			
					if( m_clItem[i].siUnique )
					{
						SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_clItem[i].siUnique );

						if(ref > 0)
						{
							SI32 ItemType1 = pclClient->pclItemManager->pclItemInfo[ref]->siType;

							if ( ItemType1 == ItemType )
							{	
								if(m_clItem[i].GetFullName(pclClient->pclItemManager, itemname, 128) == true)
								{
									if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
									{
										//	sprintf(buffer, TEXT("%s %d개"), itemname, m_clItem[i].GetItemNum());
										TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
										pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ToolTipMsg );

										stListViewItemData ListViewItemData;
										ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos , m_clItem[i].GetItemNum(), itemname, ToolTipMsg );														
										m_pListView->InsertItem( &ListViewItemData );
									}
								}
							}
						}
					}
				}
			}
			m_siItemType = ItemType;
		}
	}

}

void CNForeignMerchantBuyItemDlg::LoadForeignNPCData()
{
	//	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ) );	
	m_pList->Clear();

	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );	

	if ( m_pForeignMerchantNPC == NULL )			// 없는 NPC 찾고 있으므로...
	{
		TCHAR* pText = GetTxtFromMgr(2473);
		m_pEdit[0]->SetText(pText);
		return;
	}
	else
	{
		m_pForeignMerchantNPC->GetForeignMerchantNPCInfo( &m_ForeignMerchantNPCInfo );

		TCHAR* pText = GetTxtFromMgr(2474);
		//	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN1 ), pText );
		m_pEdit[0]->SetText(pText);

		LVITEM Li;
		Li.mask = LVIF_TEXT;
		Li.state = 0;
		Li.stateMask = 0;	

		Li.iItem = 0;	
		Li.iSubItem = 0;

		SI16 Count = 0;

		TCHAR Buffer[ 1024 ];

		SI32 i = 0;
		for (; i < m_ForeignMerchantNPCInfo.siBuyItemListCount; ++i )
		{
			TCHAR *ItemName = ( TCHAR * )pclClient->pclItemManager->GetName( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] );

			if ( ItemName == NULL )
			{
				continue;
			}

			stListBoxItemData Data;

			Li.iItem = Count++;
			Li.iSubItem = 0;
			Li.pszText = ItemName;
			//	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTITEM, 0, (LPARAM)&Li );
			Data.Set(0, Li.pszText);

			Li.iSubItem = 1;
			g_JYLibFunction.SetNumUnit( m_ForeignMerchantNPCInfo.siBuyItemPriceList[ i ], Buffer, 1024 );
			//StringCchPrintf( Buffer, 1024, TEXT("%d"), m_ForeignMerchantNPCInfo.siBuyItemPriceList[ i ] );
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
	}

	m_pList->Refresh();
	m_pList->SetSelectIndex(0);
}

bool CNForeignMerchantBuyItemDlg::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clItem[ j ];
		BYTE* dst = (BYTE*)&m_pPersonItemInfo->clItem[ j];

		for(int i = 0;i < size; i++)
		{
			if(src[i] != dst[i])
			{
				return true;
			}
		}
	}
	return false;
}


bool CNForeignMerchantBuyItemDlg::IsNPCBuyItemInfoChanged()
{
	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );

	if ( m_pForeignMerchantNPC == NULL )
	{
		return false;
	}

	stForeignMerchantNPCInfo *pForeignMerchantNPCInfo = m_pForeignMerchantNPC->GetForeignMerchantNPCInfo();

	if ( pForeignMerchantNPCInfo == NULL )
	{
		return false;
	}

	SI32 size = sizeof(stForeignMerchantNPCInfo);
	BYTE* src = (BYTE*)&m_ForeignMerchantNPCInfo;
	BYTE* dst = (BYTE*)pForeignMerchantNPCInfo;

	for(int i = 0;i < size; i++)
	{
		if(src[i] != dst[i])
		{
			return true;
		}
	}

	return false;
}

void CNForeignMerchantBuyItemDlg::Push( UI16 uiSellItemNum )
{
	if ( m_siNPCCharUnique < 1 )
	{
		return;
	}

	if ( uiSellItemNum > 1000 )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		TCHAR* pTitle = GetTxtFromMgr(1863);
		TCHAR* pText = GetTxtFromMgr(2475);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	bool bFindExactItem = false;
	for ( SI32 i = 0; i < m_ForeignMerchantNPCInfo.siBuyItemListCount; ++i )
	{
		if ( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] == m_clItem[ m_siSelectedIndex ].siUnique )
		{
			bFindExactItem = true;
			break;
		}
	}

	if ( ! bFindExactItem )
	{
	//	TCHAR* pTitle = GetTxtFromMgr(1868);
	//	TCHAR* pText = GetTxtFromMgr(1867);
	//	MessageBox( NULL, pText, pTitle, MB_OK|MB_TOPMOST );
		return;
	}

	stSellToForeignMerchant sSellToForeignMerchant;
	sSellToForeignMerchant.siForeignMerchantKind = m_siForeignMerchantKind;
	sSellToForeignMerchant.uiSelectedIndex = (UI16) m_siSelectedIndex;
	sSellToForeignMerchant.uiSellItemNum = uiSellItemNum;
	sSellToForeignMerchant.siSellItemUnique = m_clItem[ m_siSelectedIndex ].siUnique;
	sSellToForeignMerchant.siNPCCharUnique = m_siNPCCharUnique;

	CObj *pTemp = m_pForeignMerchantMgr->PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sSellToForeignMerchant, sSellToForeignMerchant.sHeader.uiSize );
	}

}

bool CNForeignMerchantBuyItemDlg::Pop( stSellToForeignMerchant *pSellToForeignMerchant )
{
	CObj *pTemp = m_pForeignMerchantMgr->PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp )
	{
		UI32 uiDataSize;
		pTemp->GetData( (BYTE*)pSellToForeignMerchant, &uiDataSize );
		return true;
	}

	return false;
}

void CNForeignMerchantBuyItemDlg::SetData(void *pBuffer )
{
	stNForeignMerchantBuyItemData* Data = (stNForeignMerchantBuyItemData*)pBuffer;

	switch(Data->siParam)
	{
	case 0:
		Pop(Data->pSellToForeignMerchant);
		break;
	}
}

void CNForeignMerchantBuyItemDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNForeignMerchantBuyItemDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				//------------------------------------------
				// NPC에게 물건을 판매하는 퀘스트 중이었다면, 
				//------------------------------------------
				SI16 questtype, questunique, para1, para2;
				SI16 questclearcond =  QUEST_CLEAR_COND_SELLSWORDNPC;
				if(pclclient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
				{
					if(para1 == m_clItem[ m_siSelectedIndex ].siUnique)
					{
						cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
						cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}

				questclearcond =  QUEST_CLEAR_COND_SELLNPC;
				if(pclclient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
				{
					cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
					cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				questclearcond =  QUEST_CLEAR_COND_SELLTRADER;
				if(pclclient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
				{
					cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
					cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				Push( 1 );
			}
		}
		break;
	}
}