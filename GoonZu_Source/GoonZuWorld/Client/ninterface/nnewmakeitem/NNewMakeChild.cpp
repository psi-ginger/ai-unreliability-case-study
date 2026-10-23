#include "client.h"

#include "NNewMakeItemDlg.h"
#include "NNewMakeChild.h"

#include "../../Inventory/ItemInventoryBuffer.h"


#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../CommonLogic/Msg/MsgType-Quest.h"

#include "NInterface/NNewInventory/NNewInventory.h"

extern cltCommonLogic* pclClient;

CNNewMakeChild::CNNewMakeChild()
{	
	m_pMakeableItemList = NULL;
}
CNNewMakeChild::~CNNewMakeChild()
{
	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}	
}
void CNNewMakeChild::Create(CControllerMgr *pParent)
{
	cltClient *pclclient = (cltClient*)pclClient;    
	
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		if (pclClient->siServiceArea == ConstServiceArea_Korea) {

			file.LoadFile(TEXT("NInterface/Data/NNewMakeItem/DLG_NewItem_k.ddf"));
		}
		else
		{
			file.LoadFile(TEXT("NInterface/Data/NNewMakeItem/DLG_NewItem.ddf"));
		}
		file.CreateChildDialog( this, NNEWMAKECHILD_DLG, TEXT("dialog_make"),StatiCNNewMakeChildProc, pParent);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON3, this ), NEWMAKE_BUTTON3,   TEXT("button_Process")); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NEWMAKE_LISTVIEW1, this ), NEWMAKE_LISTVIEW1,   TEXT("listview_material")); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NEWMAKE_LISTVIEW2, this ), NEWMAKE_LISTVIEW2,   TEXT("listview_tool")); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEWMAKE_COMBOBOX, this ), NEWMAKE_COMBOBOX,   TEXT("combobox_itemKind")); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEWMAKE_EDIT, this ), NEWMAKE_EDIT,  TEXT("editbox_ItemNum")); 

		SetActionTimer(1000);

		if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
		{
			CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
			for(SI32 i =0 ; i <4; i++)
			{
				CNNewInventory::clMakeItemContainer.cltools[i].Init();
			}	
			for(SI32 i =0 ; i <25; i++)
			{
				CNNewInventory::clMakeItemContainer.clMaterials[i].Init();
			}	
		}			

		for(SI32 i =0; i< 28 ; i++)
		{
			pclClient->pclCM->CR[1]->siInvMakeItemIndex[i] = 0;
			SI32 Destination = pclClient->pclCM->CR[1]->siInvMakeItemIndex[i] ;
			cltGameMsgRequest_MakeItem_SelectedIndex clGameMsgRequest_MakeItem_SelectedIndex(i,Destination);
			cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMINVINDEX, sizeof(clGameMsgRequest_MakeItem_SelectedIndex), (BYTE*)&clGameMsgRequest_MakeItem_SelectedIndex);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		ComboMakeItem();
	}
}

void CNNewMakeChild::Hide()
{
	CControllerMgr::Show( false );
}

void CNNewMakeChild::Show()
{
	CControllerMgr::Show( true );
}


void CALLBACK CNNewMakeChild::StatiCNNewMakeChildProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewMakeChild *pThis = (CNNewMakeChild*)pControllerMgr;
	pThis->NNewMakeChildProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewMakeChild::NNewMakeChildProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;    
	
	switch( nControlID)
	{
	case NEWMAKE_COMBOBOX:		
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED://�޺��ڽ��� �����۵��� �������!
				{
					m_siComboBoxSelectedIndex	=	m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->GetSelectedIndex();
					SetMakeItemNumEnable();
					break;
				}
			case EVENT_COMBOBOX_OPEN:
				{
					ComboMakeItem();
					SI16 questtype, questunique, para1, para2;
					SI16 questclearcond =  QUEST_CLEAR_COND_CLICKMAKEBUTTON;
					if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
					{
						SI32 MaterialUnique[4] = { ITEMUNIQUE(5030) , ITEMUNIQUE(5020) , ITEMUNIQUE(7000) , ITEMUNIQUE(6110) };
						
						if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
						{
							CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

							for(SI32 i =0 ; i <25; i++)
							{
								if(CNNewInventory::clMakeItemContainer.clMaterials[i].siUnique == MaterialUnique[0])
								{
									MaterialUnique[0] = 0;
								}
								else if(CNNewInventory::clMakeItemContainer.clMaterials[i].siUnique == MaterialUnique[1])
								{
									MaterialUnique[1] = 0;
								}
								else if(CNNewInventory::clMakeItemContainer.clMaterials[i].siUnique == MaterialUnique[2])
								{
									MaterialUnique[2] = 0;
								}
								else if(CNNewInventory::clMakeItemContainer.clMaterials[i].siUnique == MaterialUnique[3])
								{
									MaterialUnique[3] = 0;
								}
							}	
						}		

						if( MaterialUnique[0] > 0 || MaterialUnique[1] > 0  || MaterialUnique[2] > 0  || MaterialUnique[3] > 0  )
						{
							cltClient *pclclient = (cltClient*)pclClient;
							TCHAR* pTitle = GetTxtFromMgr(6139);
							TCHAR* pText = GetTxtFromMgr(6145);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);	
							break;
						}

						cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
						cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
						cltClient* pclclient = (cltClient* )pclClient;
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
			}

		}
		break;		
	case NEWMAKE_BUTTON3: //�����ϱ� ��ư
		{
			TCHAR *InputText =m_InterfaceFactory.GetEdit(NEWMAKE_EDIT)->GetText();//����� ����.
			CNNewInventory* newinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
			CItemInventoryBuffer* m_pBuffer = newinventory->m_pBuffer;
			if ( InputText && strlen( InputText ) > 0 )
			{
				SI16 MakeItemNum = atoi( InputText );
				if ( MakeItemNum < 1 )
				{
					break;
				}

				if ( m_siComboBoxSelectedIndex < 0 )
				{
					break;
				}
				if( pclClient->IsCountrySwitch( Switch_Change_manufacture_request_interface ) )
				{
					cltCharClient* pchar = pclClient->pclCM->GetCharClient(1)	;
					if(pchar != NULL )
					{
						if(pchar->siShopMode == PSHOP_MODE_MAKEITEM || 
							pchar->siShopMode == PSHOP_MODE_MAKEITEM_WANT)
						{
							break	;
						}
					}

				}

				// ���� �ʰ�
				if ( m_pMakeableItemList && MakeItemNum > m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 + 1 ]  )
				{
					break;
				}

				m_pBuffer->Push( INV_MAKE_ITEM, m_pMakeableItemList[ m_siComboBoxSelectedIndex  * 2 ], MakeItemNum );
				ComboMakeItem();
				

			}						
		}
		break;
	case NEWMAKE_LISTVIEW1: //���â
		{
            MaterialListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW1), 0 , nEvent );				
			
		}
		break;
	case NEWMAKE_LISTVIEW2: //��â
		{			
			ToolListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2), 0 , nEvent );			
			
		}
		break;

	}
	return ;
}

void CNNewMakeChild::ComboMakeItem()
{
	cltClient *pclclient = (cltClient*)pclClient;
	// �־��� ��Ḧ �ٰŷ� ���� ������ �������� ����Ʈ�� �ۼ��ؼ� �����Ѵ�. 
	// ���� ����Ʈ�� ����� ������ �־�� �Ѵ�. 
	SI32 siMakeableItemNum = 0;	
	SI32 i =0;

	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}	
	m_pMakeableItemList = NULL;

	cltPItemInfo     clAbleMakeItem;
	clAbleMakeItem.Init();
	for(i=0; i<29; i++)
	{		
		if(i<25)
		{
			memcpy(&clAbleMakeItem.clItem[i], &m_clMaterials[i], sizeof(cltItem));
		}
		else
		{
            memcpy(&clAbleMakeItem.clItem[i], &m_cltools[i-25], sizeof(cltItem));
		}
	}
	m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->Clear();
	SI32 id =1;
	if (pclclient->IsCountrySwitch( Switch_AutoMakeItem ) == true) {
		if( pclClient->pclItemManager->FindMakeableItem(
			&pclClient->pclCM->CR[id]->pclCI->clCharSkill,  //��ų
			//&clAbleMakeItem,
			&pclClient->pclCM->CR[1]->pclCI->clCharItem,
			&pclClient->pclCM->CR[id]->clPB.clTotalBA,		//������ 
			MAX_INVITEM_PER_PERSON,
			//&clAbleMakeItem.clItem[0],//����â�� �÷��� ������.
			&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0],//�κ��� �ִ� �����۟� ���� ����� �ִ� �������� ����
			&m_siMakeableItemNum, &m_pMakeableItemList,
			false// true�� ������ ������� �ʴ´� false�� �������� ���
			) == true)
		{
			// �޺� �ڽ��� �������� ���갡���� ����� ǥ���Ѵ�. 
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

						sprintf( buffer, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );

						stComboBoxItemData ComboBoxItemData;
						ComboBoxItemData.Init();
						ComboBoxItemData.Set( buffer );					

						m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->AddItem( &ComboBoxItemData );
						m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->Refresh();
					}
				}
			}
		}
	}
	else{
		if( pclClient->pclItemManager->FindMakeableItem(
			&pclClient->pclCM->CR[id]->pclCI->clCharSkill,  //��ų
			//&clAbleMakeItem,
			&pclClient->pclCM->CR[1]->pclCI->clCharItem,
			&pclClient->pclCM->CR[id]->clPB.clTotalBA,		//������ 
			MAX_INVITEM_PER_PERSON,
			&clAbleMakeItem.clItem[0],//����â�� �÷��� ������.
			&m_siMakeableItemNum, &m_pMakeableItemList,
			true// true�� ������ ������� �ʴ´� false�� �������� ���
			) == true)
		{
			// �޺� �ڽ��� �������� ���갡���� ����� ǥ���Ѵ�. 
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

						sprintf( buffer, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );

						stComboBoxItemData ComboBoxItemData;
						ComboBoxItemData.Init();
						ComboBoxItemData.Set( buffer );					

						m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->AddItem( &ComboBoxItemData );
						m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->Refresh();
					}
				}
			}
		}
	}
}
void  CNNewMakeChild::SetMakeItemComboBoxEnable(TCHAR * m_siComboBoxSelectedName){

}

void CNNewMakeChild::Action()
{
    //ComboMakeItem();
}

void CNNewMakeChild::ShowMaterials(cltItem clMaterials[])
{
	cltClient *pclclient = (cltClient*)pclClient;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	memcpy(m_clMaterials,clMaterials, sizeof(cltItem)*25);
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW1)->DeleteAllItems();

	for(SI32 i =0; i < 25 ; i++)
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clMaterials[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( clMaterials[ i ].siUnique );

			if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				pclClient->pclItemManager->GetExplain( &clMaterials[ i ], ToolTipMsg );
				SI32 size = sizeof(ToolTipMsg);
				SI32 ddd = strlen(ToolTipMsg);


				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i , clMaterials[ i ].siItemNum, ItemName, ToolTipMsg );
				m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW1)->InsertItem(&ListViewItemData);				
			}
		}
	}
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW1)->Refresh();
	if (pclclient->IsCountrySwitch( Switch_AutoMakeItem ) == true)
	{
		//ComboMakeItem();
	}
	else
	{
		ComboMakeItem();
	}

}


void CNNewMakeChild::ShowTools(cltItem cltools[])
{
	cltClient *pclclient = (cltClient*)pclClient;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	
	memcpy(m_cltools,cltools, sizeof(cltItem)*4);
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2)->DeleteAllItems();

	for(SI32 i =0; i< 4 ; i++)
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_cltools[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_cltools[ i ].siUnique );

			if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
//				SI32 siItemStatus = 0 ;
//				pclClient->pclItemManager->GetExplain( &m_cltools[ i ], ToolTipMsg ,&siItemStatus);
				pclClient->pclItemManager->GetExplain( &m_cltools[ i ], ToolTipMsg);
				SI32 size = sizeof(ToolTipMsg);
				SI32 ddd = strlen(ToolTipMsg);

			
				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i , m_cltools[ i ].siItemNum, ItemName, ToolTipMsg );
				m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2)->InsertItem(&ListViewItemData);				
			}
		}
	}
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2)->Refresh();
	if (pclclient->IsCountrySwitch( Switch_AutoMakeItem ) == true) {
		//ComboMakeItem();
	}
	else
	{
		ComboMakeItem();
	}
    
}


void CNNewMakeChild::MaterialListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// �����̵� ����̶� ������ ������� �ִ�!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;

	static int TempCounter =0 ;

	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP://���Ḧ ����.
		{
    		SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;
			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])                                            
			{ 
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				CNNewInventory::clMakeItemContainer.clMaterials[SelectedIndex].Init();						
				equip->m_pInventoryListView->SetItemDrawAlphaOption(CNNewInventory::clMakeItemContainer.m_siMaterials[SelectedIndex],false);

				// ������ ��ȣ�� �����Ѵ�.
				cltGameMsgRequest_MakeItem_SelectedIndex clGameMsgRequest_MakeItem_SelectedIndex(SelectedIndex, 0);
				cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMINVINDEX, sizeof(clGameMsgRequest_MakeItem_SelectedIndex), (BYTE*)&clGameMsgRequest_MakeItem_SelectedIndex);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);					
			}	
			m_clMaterials[SelectedIndex].Init();			

			pListView->DeleteItem(SelectedIndex);
		}
		break;
	}


}

void CNNewMakeChild::ToolListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// �����̵� ����̶� ������ ������� �ִ�!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;

	//	if( m_pInventoryListView == NULL )
	//		return;

	static int TempCounter =0 ;

	switch( nEvent )
	{

	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;

			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				CNNewInventory::clMakeItemContainer.cltools[SelectedIndex].Init();
				equip->m_pInventoryListView->SetItemDrawAlphaOption(CNNewInventory::clMakeItemContainer.m_sitools[SelectedIndex],false);
			}	
			m_cltools[SelectedIndex].Init();				
			
		//	CListView* tempListView = m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2);
			
			pListView->DeleteItem(SelectedIndex);			
		}
		break;
	}		
}
	
void CNNewMakeChild::SetMakeItemNumEnable()
{
	cltClient *pclclient = (cltClient*)pclClient;
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
					
					m_InterfaceFactory.GetEdit(NEWMAKE_EDIT)->SetReadOnly(true);
				}
				else
				{
					m_InterfaceFactory.GetEdit(NEWMAKE_EDIT)->SetReadOnly(false);
				}
				m_InterfaceFactory.GetEdit(NEWMAKE_EDIT)->SetText(buffer);
				if ( pclclient->IsCountrySwitch( Switch_AutoMakeItem ) == true ){
					AutoSetItemMat();//���⿡�� �������� �־��ش�.��������!
				}
				else
				{
				}
				
			}
		}
	}
}

SI32 CNNewMakeChild::GetComboBoxPosX()
{
	return GetX() +	m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->GetX() + m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->GetWidth()/2;
}
SI32 CNNewMakeChild::GetComboBoxPosY()
{
	return GetY() + m_InterfaceFactory.GetComboBox(NEWMAKE_COMBOBOX)->GetY();
}

void CNNewMakeChild::AutoSetItemMat()//�ڵ����� ��� �ֱ�.
{
	cltClient *pclclient = (cltClient *)pclClient;
	CNNewInventory* newinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

	if (m_siComboBoxSelectedIndex < 0)  return;// m_siComboBoxSelectedIndex �� 0���� ������ �ƹ��ϵ� ���ع����� ����.
	ClearMakeItemMat();//���� �������� �� Ŭ����

	SI32 unique		= m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 ];//���� ������ ����ũ
	SI32 itemnum	= m_pMakeableItemList[ m_siComboBoxSelectedIndex * 2 + 1 ];//���鰹���� �⺻ 1.

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(unique);
	if (ref <= 0)	return;
	for(SI32 i = 0;i < MAX_MATERIAL_NUMBER;i++)
	{
 		SI32 materialunique = 0;
		SI32 materialneednum = 0;
		materialunique = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[i].siUnique;//�ʿ� ���
		materialneednum = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[i].siNeedNum;//�ʿ���� ����
		if (materialunique <= 0) continue;
		SI32 haveitemposion = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos(materialunique);//�κ��丮���� �ش� �������� ��ġ�� ������ �´�.		
		if (haveitemposion == -1 )	return;
		//�� �༮�� ���δ� �޺� �ڽ��� Ŭ���� �ϸ� �ȵȴ�.
		newinventory->ShowMaterials(haveitemposion);//��Ḧ �ִ´�.
	}
	for(SI32 i = 0;i < MAX_TOOL_NUMBER; i++)
	{
		SI32 needtoolsunique = 0;
		needtoolsunique = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clTool[i].siUnique;//�ʿ� ��
		SI32 havetoolposion = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos(needtoolsunique);//�κ��丮���� �ش� �������� ��ġ�� ������ �´�.		
		if (havetoolposion == -1 )	return;
		//�� �༮�� ���δ� �޺� �ڽ��� Ŭ���� �ϸ� �ȵȴ�.
		newinventory->ShowTools(havetoolposion);//���� �Ŵ´�.
	}
}
void CNNewMakeChild:: ClearMakeItemMat()//�ڵ����� ���������� ����.
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI08 Index = 0;
	CListView* pListView1 = 	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW1);
	CListView* pListView2 = 	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW2);

	if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
	{
		//��Ḧ ����
		for (Index = 0; Index<MAX_MATERIAL_NUMBER; Index ++) {
			CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
			CNNewInventory::clMakeItemContainer.clMaterials[Index].Init();						
			equip->m_pInventoryListView->SetItemDrawAlphaOption(CNNewInventory::clMakeItemContainer.m_siMaterials[Index],false);
			cltGameMsgRequest_MakeItem_SelectedIndex clGameMsgRequest_MakeItem_SelectedIndex(Index, 0);
			cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMINVINDEX, sizeof(clGameMsgRequest_MakeItem_SelectedIndex), (BYTE*)&clGameMsgRequest_MakeItem_SelectedIndex);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			m_clMaterials[Index].Init();
			pListView1->DeleteItem(Index);
			
		}
		//���� ����
		for (Index = 0; Index <MAX_TOOL_NUMBER; Index++) {
			CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
			CNNewInventory::clMakeItemContainer.cltools[Index].Init();
			equip->m_pInventoryListView->SetItemDrawAlphaOption(CNNewInventory::clMakeItemContainer.m_sitools[Index],false);
			m_cltools[Index].Init();
			pListView2->DeleteItem(Index);
		}
	}
}

bool CNNewMakeChild::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;
	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	if(personinventory == NULL)										return false;
	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	switch(pclClickAndDrop_Item->m_siSrcDialogID)
	{
	// �κ��丮���� �κ��丮�� �̵��ϴ� ���
	case NNEWINVENTORY_DLG:
	case NNEWINVENTORY_HORSE_DLG:
	case NNEWINVENTORY_BAG0_DLG:
	case NNEWINVENTORY_BAG1_DLG:
		{
			switch(pclClickAndDrop_Item->m_siDstControlID)
			{
			case NEWMAKE_LISTVIEW1:
				{
					personinventory->ShowMaterials(pclClickAndDrop_Item->m_siPara1);
					return true;
				}				
				break;
			case NEWMAKE_LISTVIEW2:
				{
					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1].IsSealingUp(pclclient->pclItemManager) == true )
					{
						NTCHARString64 kTitle(GetTxtFromMgr(5662));
						NTCHARString64 kText(GetTxtFromMgr(5663));
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitle , kText);				
					}
					else
						personinventory->ShowTools(pclClickAndDrop_Item->m_siPara1);

					return true;
				}				
				break;
			}			
		}
		break;
	}

	return false;
}
