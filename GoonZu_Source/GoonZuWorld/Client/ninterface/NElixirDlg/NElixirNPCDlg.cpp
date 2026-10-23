#include "NElixirNPCDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

NElixirNPCDlg::NElixirNPCDlg()
{
	m_bClickSelectItem			 = true;
	m_siComboBoxItemtype		 = 0;
	m_siSelectChangeItemInvenPos = 0;

	ZeroMemory( m_siItemUnique, sizeof(m_siItemUnique) );
	ZeroMemory( m_siWaterNum,	sizeof(m_siWaterNum)   );
}

NElixirNPCDlg::~NElixirNPCDlg()
{
	Destroy();
}

void NElixirNPCDlg::Init()
{
	//초상화
	TCHAR	szPortrait[256];
	SI32	siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_ELIXIRNPC")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_InterfaceFactory.GetStatic( NELIXIR_NPC_STATIC_PORTRAIT_IMAGE )->SetFileName(szPortrait);
	m_InterfaceFactory.GetStatic( NELIXIR_NPC_STATIC_PORTRAIT_IMAGE )->SetFontIndex( siPortraitFont );
	m_InterfaceFactory.GetEdit( NELIXIR_NPC_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(9760) ); 

	// 리스트 창 초기화.
	CList* pList = m_InterfaceFactory.GetList( NELIXIR_NPC_LIST_ITEM_KIND );
	if( NULL == pList ) return;

	TCHAR* pText = NULL;
	pText = GetTxtFromMgr(8120);
	pList->SetColumn( 0, 120, pText );
	pText = GetTxtFromMgr(9761);
	pList->SetColumn( 1, 100, pText );
	pList->Refresh();

	// 내 인벤토리.
	CListView* pListView = m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_MY_INVENTOTY );
	if( NULL == pListView ) return;
	pListView->SetBKColorUse( true );
	pListView->SetBorder( true );

	// 리스트 박스를 선택하면 아이템 이미지가 리스트뷰에 보이는데 이때 툴팁과 개수는 보이지 말자.
	pListView = m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_SELECT_ITEM );
	pListView->Enable( false );
	pListView->SetUseToolTip( false );
	pListView->SetItemNumShow( false );

	// 교환 가능한 아이템만 콤보 리스트에 담자.
	CComboBox* pCombo = m_InterfaceFactory.GetComboBox( NELIXIR_NPC_COMBO_ITEM_TYPE );
	if( NULL == pCombo ) return;
	
	// 교환 가능한 아이템은 여기서 텍스트로 콤보박스에 추가 시켜주세용~
	stComboBoxItemData pComboBoxItemData;
	pComboBoxItemData.Set(GetTxtFromMgr(2366));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(2370));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(2368));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(2372));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(2374));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(2376));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(1024));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(1026));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(1029));
	pCombo->AddItem(&pComboBoxItemData);
	pComboBoxItemData.Set(GetTxtFromMgr(1030));
	pCombo->AddItem(&pComboBoxItemData);
	pCombo->SetCursel(0); 
}

void NElixirNPCDlg::Destroy()
{ 


}

void NElixirNPCDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NElixirDlg/NElixirNPCDlg.ddf");

	file.CreatePopupDialog( this, NELIXIR_NPC_DLG, "dialog_elixir_npc" , StatiNElixirNPCDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_NPC_STATIC_PORTRAIT_IMAGE,	this),  NELIXIR_NPC_STATIC_PORTRAIT_IMAGE,			TEXT("statictext_portrait_image")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_NPC_STATIC_ITEM_TYPE,		this),  NELIXIR_NPC_STATIC_ITEM_TYPE,				TEXT("statictext_item_type")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_NPC_STATIC_MY_INVENTORY,	this),  NELIXIR_NPC_STATIC_MY_INVENTORY,			TEXT("statictext_my_inventory")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_NPC_STATIC_CHANGE_ITEM,		this),  NELIXIR_NPC_STATIC_CHANGE_ITEM,				TEXT("statictext_change_item")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(COMBOBOX,  	NELIXIR_NPC_COMBO_ITEM_TYPE,		this),  NELIXIR_NPC_COMBO_ITEM_TYPE,				TEXT("combobox_item_type")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,		NELIXIR_NPC_LIST_ITEM_KIND,			this),  NELIXIR_NPC_LIST_ITEM_KIND,					TEXT("listbox_item_kind")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,  	NELIXIR_NPC_LISTVIEW_MY_INVENTOTY,	this),  NELIXIR_NPC_LISTVIEW_MY_INVENTOTY,			TEXT("listview_my_inventory")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	NELIXIR_NPC_BUTTON_ITEM_CHANGE,		this),  NELIXIR_NPC_BUTTON_ITEM_CHANGE,				TEXT("button_item_change")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	NELIXIR_NPC_BUTTON_CLOSE,			this),  NELIXIR_NPC_BUTTON_CLOSE,					TEXT("button_close")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		NELIXIR_NPC_EDIT_CHANGE_ITEM_NAME,	this),  NELIXIR_NPC_EDIT_CHANGE_ITEM_NAME,			TEXT("editbox_change_item_name")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		NELIXIR_NPC_EDIT_EXPLAIN,			this),  NELIXIR_NPC_EDIT_EXPLAIN,					TEXT("editbox_explian")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,  	NELIXIR_NPC_LISTVIEW_SELECT_ITEM,	this),  NELIXIR_NPC_LISTVIEW_SELECT_ITEM,			TEXT("listview_listbox_select_item"));

	Init();

	// 퀘스트 관련
	SI32 charkind = pclClient->GetUniqueFromHash(TEXT("KIND_ELIXIRNPC"));

	SI16 questtype, para1, para2;
	SI16 questunique;
	SI16 questclearcond =  QUEST_CLEAR_COND_GONPC;
	if ( pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true )
	{
		if(para1 == charkind )
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
	//

	SetX( 450 ); 
	SetY( 300 );
}

void NElixirNPCDlg::Action()
{

}

void CALLBACK NElixirNPCDlg::StatiNElixirNPCDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NElixirNPCDlg *pThis = (NElixirNPCDlg *)pControllerMgr;
	pThis->NElixirNPCDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NElixirNPCDlg::NElixirNPCDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	m_InterfaceFactory.GetEdit( NELIXIR_NPC_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(9760) ); 
 
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

		case NELIXIR_NPC_COMBO_ITEM_TYPE:
			{
				switch( nEvent )
				{
					case EVENT_COMBOBOX_SELECTION_CHANGED:
						{
							CComboBox* pCombo = m_InterfaceFactory.GetComboBox( NELIXIR_NPC_COMBO_ITEM_TYPE );
							if( NULL == pCombo )	return;
							
							int index = pCombo->GetSelectedIndex();
							if( index < 0 ) return;

							TCHAR* szSelText = pCombo->GetText( index );
							if( NULL == szSelText ) return;

							m_siComboBoxItemtype = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( szSelText );
							if( m_siComboBoxItemtype >=0 ) 
							{
								// 서버에 타입별 아이템 리스트 정보 요청. 
								cltGameMsgRequest_Elixir_Npc_Item_Type_List_Get clInfo( m_siComboBoxItemtype );
								cltMsg clMsg( GAMEMSG_REQUEST_ELIXIR_NPC_ITEM_TYPE_LIST_GET, sizeof(clInfo), (BYTE*)&clInfo );
								pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg); 
								
								SetMyInventory();
								 
								m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_SELECT_ITEM )->DeleteAllItems();
							}
						} 
						break;
				}
			}
			break;

		case NELIXIR_NPC_LIST_ITEM_KIND:
			{
				switch( nEvent )
				{
					case EVENT_LISTBOX_SELECTION:
						{
							SetSelectItem();
						}
						break;
				}
			}
			break;
		
		case NELIXIR_NPC_LISTVIEW_MY_INVENTOTY:
			{
				if( false == m_bClickSelectItem ) return;

				CListView* pListView = m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_MY_INVENTOTY );
				if( NULL == pListView ) return;
				
				// 내 인벤토리에 등록된 아이템 중 선택 아이템의 인벤에서의 위치 저장.
				m_siSelectChangeItemInvenPos = pListView->GetSelectedItemIndex();
				if( 0 < m_siSelectChangeItemInvenPos )
				{
					stListViewItemData* pListViewItemData = pListView->GetListViewItemData( m_siSelectChangeItemInvenPos );
					if( NULL == pListViewItemData ) return;
					
					m_InterfaceFactory.GetEdit( NELIXIR_NPC_EDIT_CHANGE_ITEM_NAME )->SetText( pListViewItemData->ItemName );
				}
			}
			break;
	
		// 아이템 교환.
		case NELIXIR_NPC_BUTTON_ITEM_CHANGE:
			{
				if( m_siSelectChangeItemInvenPos > 0 )
				{
					cltPItemInfo* pInvenItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

					if( 0 >= pInvenItemInfo->clItem[ m_siSelectChangeItemInvenPos ].siUnique ) return;
					
					for( SI32  i = 0; i < ELIXIR_LIST_NUM; i++ )
					{
						if( m_siItemUnique[i] == pInvenItemInfo->clItem[ m_siSelectChangeItemInvenPos ].siUnique )
						{
							if( 0>= m_siWaterNum[i] )
							{
								m_InterfaceFactory.GetEdit( NELIXIR_NPC_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(9762) ); 
							}
							else
							{
								NTCHARString128 kbuf( GetTxtFromMgr(9763) ); 
								kbuf.Replace( "#itemnum#", SI32ToString( m_siWaterNum[i] ) );

								stMsgBox MsgBox;  
								MsgBox.Set( this, GetTxtFromMgr(5614), kbuf, MSGBOX_TYPE_YESNO, 0 );			

								pclclient->SetMsgBox( &MsgBox, NULL, 0 );

								m_bClickSelectItem = false;
							}
						}
					}
				}
			}
			break;
	
		case NELIXIR_NPC_BUTTON_CLOSE:
			{
				DeleteDialog();
			}
			break;
	
		default:
			{

			}
			break;
	}
}

void NElixirNPCDlg::SetItemListInfo( SI32* siItemUnique, SI32* siWaterNum )
{
	CList* pList = m_InterfaceFactory.GetList( NELIXIR_NPC_LIST_ITEM_KIND );
	if( NULL == pList ) return;
	pList->Clear();

	//리스트 관련 정보 저장.
	memcpy( m_siItemUnique, siItemUnique,	sizeof(m_siItemUnique)	);
	memcpy( m_siWaterNum,	siWaterNum,		sizeof(m_siWaterNum)	);

	stListBoxItemData itemdata;

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( SI32  i = 0; i < ELIXIR_LIST_NUM; i++ )
	{	
		if( 0 >= siItemUnique[i] )	break;

		itemdata.Init();
		itemdata.Set( 0, (TCHAR*)pclClient->pclItemManager->GetName( siItemUnique[i] ) );
		itemdata.Set( 1, SI32ToString( siWaterNum[i] ) );
		pList->AddItem( &itemdata );
	}

	pList->Refresh(); 
	pList->SetSelectIndex(0);
	
	SetSelectItem();
}

void NElixirNPCDlg::SetMyInventory()
{	
	m_InterfaceFactory.GetEdit( NELIXIR_NPC_EDIT_CHANGE_ITEM_NAME )->SetText("");	// 이전에 선택 된 아이템의 이름이 사라지게. 
	m_bClickSelectItem			 = true;											// 리스트뷰에 등록된 아이템 선택 가능하게 변경.
	m_siSelectChangeItemInvenPos = 0;												// 선택된 아이템이 없게 위치를 0으로 .

	// 내 인벤에 있는 아이템을 리스트뷰에 등록하기.
	cltPItemInfo* pInvenItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;
	if( NULL == pInvenItemInfo ) return;
	
	SI32	GlobalImageIndex	= -1;
	SI32	GlobalImageFont		= -1;
	SI32	StartPos			=  0;
	
	TCHAR	Itemname[128]						= TEXT("");
	TCHAR	ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ]	= TEXT("");

	stListViewItemData ListViewItemData;
	
	CListView* pListView = m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_MY_INVENTOTY );
	if( NULL == pListView ) return;
	pListView->DeleteAllItems();
	pListView->SetStartItemPos( StartPos );

	for( SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; i++ )
	{			
		if( pInvenItemInfo->clItem[i].siUnique )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pInvenItemInfo->clItem[i].siUnique  );

			if(ref > 0)
			{
				// 콤보박스에서 선택된 아이템 타입에 관련된 아이템만을 등록.
				if ( pclClient->pclItemManager->pclItemInfo[ref]->siType == m_siComboBoxItemtype )
				{
					if( pInvenItemInfo->clItem[i].GetFullName( pclClient->pclItemManager, Itemname, 128 ) == true )
					{
						if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pInvenItemInfo->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
						{
							ZeroMemory( ToolTipMsg, sizeof(ToolTipMsg) );
							pclClient->pclItemManager->GetExplain( &pInvenItemInfo->clItem[ i ], ToolTipMsg );

							ListViewItemData.Init();
							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos , pInvenItemInfo->clItem[i].GetItemNum(), Itemname, ToolTipMsg );														
							pListView->InsertItem( &ListViewItemData );
						}
					}
				}
			}
		}
	}
}

void NElixirNPCDlg::SetSelectItem( void )
{	
	CList* pList = m_InterfaceFactory.GetList( NELIXIR_NPC_LIST_ITEM_KIND );
	if( NULL == pList ) return;

	int index = pList->GetSelectedIndex();
	if( index < 0 )		return;

	stListBoxItemData* pItemData = pList->GetItemData( index );
	if( NULL == pItemData ) return;

	CListView* pListView = m_InterfaceFactory.GetListView( NELIXIR_NPC_LISTVIEW_SELECT_ITEM );
	if( NULL == pListView ) return;
	pListView->DeleteAllItems();

	SI32	GlobalImageIndex = -1;
	SI32	GlobalImageFont	 = -1;
	SI32	siItemUnique	 = pclClient->pclItemManager->FindUniqueFromName( pItemData->strText[0] );
	SI32	ref				 = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique  );

	if(ref > 0)
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			stListViewItemData ListViewItemData;
			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , 1, pItemData->strText[0], NULL );														
			pListView->InsertItem( &ListViewItemData );
		}
	}
}

void NElixirNPCDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	
	switch( MsgIndex )
	{
		case 0:
			{
				if( RetValue ) 
				{
					if( m_siSelectChangeItemInvenPos > 0 )
					{
						cltPItemInfo* pInvenItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;
						if( NULL == pInvenItemInfo ) return;

						if( 0 >= pInvenItemInfo->clItem[ m_siSelectChangeItemInvenPos ].siUnique ) return;

						cltGameMsgRequest_Elixir_Npc_Change_Item clInfo( pInvenItemInfo->clItem[ m_siSelectChangeItemInvenPos ].siUnique, m_siSelectChangeItemInvenPos );
						cltMsg clMsg( GAMEMSG_REQUEST_ELIXIR_NPC_CHANGE_ITEM, sizeof(clInfo), (BYTE*)&clInfo );
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg); 
						
						m_siSelectChangeItemInvenPos = 0;
					}
				}
								
				m_bClickSelectItem = true;	// 리스트뷰에 등록된 아이템 선택 가능하게 변경.
			}
			break;
	}
}



