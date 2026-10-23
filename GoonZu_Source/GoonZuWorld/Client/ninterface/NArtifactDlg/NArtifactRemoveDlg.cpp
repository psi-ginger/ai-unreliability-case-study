#include "../Client/ninterface/NArtifactDlg/NArtifactRemoveDlg.h"
#include "../CommonLogic/MsgType-Item.h"
#include "Item/ItemCommon/Itemunit.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


NArtifactRemoveDlg::NArtifactRemoveDlg()
{
	ZeroMemory( m_clArtifactItem,	sizeof( m_clArtifactItem ) );
	ZeroMemory( m_bRemoveSlotIndex, sizeof( m_bRemoveSlotIndex ) );
	ZeroMemory( m_clCrystalItem,	sizeof( m_clCrystalItem ) );
	ZeroMemory( m_InventoryPos,		sizeof( m_InventoryPos ) );
	
	m_Spr[IMAGE_SLOT_REMOVE]  = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_REMOVE );
	m_Spr[IMAGE_GAGE_BACK]	  = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE_BACK  );
	m_Spr[IMAGE_GAGE]		  = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE	   );
	m_Spr[IMAGE_EFFECT]		  = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_EFFECT );
	m_Spr[IMAGE_ARROW_IN_OUT] = pclClient->GetGlobalSpr( GIMG_ARROW_IN_OUT		   );

	if(NULL == m_Spr[IMAGE_SLOT_REMOVE] || NULL == m_Spr[IMAGE_GAGE_BACK] || 
	   NULL == m_Spr[IMAGE_GAGE]	    || NULL == m_Spr[IMAGE_EFFECT]	  ||
	   NULL == m_Spr[IMAGE_ARROW_IN_OUT]										) 
	{
		MsgBox("이미지 얻기 실패", "NArtifactRemoveDlg");
	}

	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;
}

NArtifactRemoveDlg::~NArtifactRemoveDlg()
{

}

void NArtifactRemoveDlg::Clear( void )
{
	DelAllListViewItem();
	m_CurrentSlot   = -1;
	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;
	
	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
	pButton->Enable(true); 
}


void NArtifactRemoveDlg::Create(  CControllerMgr *pParent  )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NArtifactDlg/NArtifactRemoveDlg.ddf" ) );
	file.CreateChildDialog( this, ARTIFACT_SLOT_CREATE_DLG, TEXT( "dialog_remove" ), StaticNArtifactRemoveDlgProc, pParent );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	  ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT1,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT1,	TEXT( "editbox_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT2,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT2,	TEXT( "editbox_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT3,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT3,	TEXT( "editbox_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT4,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT4,	TEXT( "editbox_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT5,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT5,	TEXT( "editbox_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,     ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT6,	this ), ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT6,	TEXT( "editbox_NONAME6" ) );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,   ARTIFACT_REMOVE_BUTTON_REMOVE,					this ), ARTIFACT_REMOVE_BUTTON_REMOVE,						TEXT( "button_remove" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM,		this ), ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM,		TEXT( "listview_equip_register_Item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1,	this ), ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1,	TEXT( "listview_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT2,	this ), ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT2,	TEXT( "listview_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT3,	this ), ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT3,	TEXT( "listview_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT4,	this ), ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT4,	TEXT( "listview_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT5,	this ), ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT5,	TEXT( "listview_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	  ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT1,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT1,		TEXT( "statictext_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT2,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT2,		TEXT( "statictext_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT3,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT3,		TEXT( "statictext_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT4,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT4,		TEXT( "statictext_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT5,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT5,		TEXT( "statictext_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT6,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT6,		TEXT( "statictext_NONAME6" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT7,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT7,		TEXT( "statictext_NONAME7" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,   ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT8,	this ), ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT8,		TEXT( "statictext_NONAME8" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE1,					this ), ARTIFACT_REMOVE_OUTLINE_LINE1,						TEXT( "outline_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE2,					this ), ARTIFACT_REMOVE_OUTLINE_LINE2,						TEXT( "outline_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE3,					this ), ARTIFACT_REMOVE_OUTLINE_LINE3,						TEXT( "outline_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE4,					this ), ARTIFACT_REMOVE_OUTLINE_LINE4,						TEXT( "outline_NONAME4" ) );	
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE5,					this ), ARTIFACT_REMOVE_OUTLINE_LINE5,						TEXT( "outline_NONAME5" ) );	
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE6,					this ), ARTIFACT_REMOVE_OUTLINE_LINE6,						TEXT( "outline_NONAME6" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE7,					this ), ARTIFACT_REMOVE_OUTLINE_LINE7,						TEXT( "outline_NONAME7" ) );	
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE8,					this ), ARTIFACT_REMOVE_OUTLINE_LINE8,						TEXT( "outline_NONAME8" ) );	
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE9,					this ), ARTIFACT_REMOVE_OUTLINE_LINE9,						TEXT( "outline_NONAME9" ) );
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  ARTIFACT_REMOVE_OUTLINE_LINE10,					this ), ARTIFACT_REMOVE_OUTLINE_LINE10,						TEXT( "outline_NONAME10" ) );										  								  
    
	CStatic* pStatic = NULL;
	pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT7);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->Refresh();
	
	pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT8);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->Refresh();

	COutLine* pOutLine = m_InterfaceFactory.GetOutLine(ARTIFACT_REMOVE_OUTLINE_LINE4);
	pOutLine->Show(false);

}

void CALLBACK NArtifactRemoveDlg::StaticNArtifactRemoveDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NArtifactRemoveDlg *pThis = (NArtifactRemoveDlg*) pControllerMgr;
	pThis->NArtifactRemoveDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NArtifactRemoveDlg::NArtifactRemoveDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	if(m_bGaugeAction || m_bEffectAction)
		return;

	cltClient*  pclclient     = (cltClient *)pclClient;
	void*	    pvoidControl  = m_InterfaceFactory.GetControl( nControlID );
	bool		bSlot		  = false;
	bool		bDelSlot	  = false;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	

	case ARTIFACT_REMOVE_BUTTON_REMOVE:
		{
			if(false == IsRemove())
				return;

			m_bGaugeAction = true;  
			//SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_REMOVE );
		}
		break;

	case ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM:
		{
			m_CurrentSlot = EQUIP_SLOT;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONDOWN:
					{
						bDelSlot = true;
					}
				break;
			}
		}
		break;
	case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1:
		{
			m_CurrentSlot = CRYSTAL_SLOT1;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;
						//ClickRemoveSlot( m_CurrentSlot );
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						bDelSlot							= true;
						m_bRemoveSlotIndex[CRYSTAL_SLOT1]	= false;
						m_InventoryPos[ CRYSTAL_SLOT1 ]		= 0;
						m_clCrystalItem[CRYSTAL_SLOT1].Init();
					}
					break;
			}
		}
		break;
	case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT2:
		{
			m_CurrentSlot = CRYSTAL_SLOT2;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;

						//ClickRemoveSlot( m_CurrentSlot );
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						bDelSlot							= true;
						m_bRemoveSlotIndex[CRYSTAL_SLOT2]	= false;
						m_InventoryPos[ CRYSTAL_SLOT2 ]		= 0;
						m_clCrystalItem[CRYSTAL_SLOT2].Init();
					}
					break;
			}
		}
		break;
	case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT3:
		{
			m_CurrentSlot = CRYSTAL_SLOT3;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;

						//ClickRemoveSlot( m_CurrentSlot );
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						bDelSlot							= true;
						m_bRemoveSlotIndex[CRYSTAL_SLOT3]	= false;
						m_InventoryPos[ CRYSTAL_SLOT3 ]		= 0;
						m_clCrystalItem[CRYSTAL_SLOT3].Init();
					}
					break;
			}
		}
		break;
	case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT4:
		{
			m_CurrentSlot = CRYSTAL_SLOT4;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;

						//ClickRemoveSlot( m_CurrentSlot );
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						bDelSlot							= true;
						m_bRemoveSlotIndex[CRYSTAL_SLOT4]	= false;
						m_InventoryPos[ CRYSTAL_SLOT4 ]		= 0;
						m_clCrystalItem[CRYSTAL_SLOT4].Init();
					}
					break;
			}
		}
		break;
	case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT5:
		{
			m_CurrentSlot = CRYSTAL_SLOT5;

			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						bSlot = true;    

						//ClickRemoveSlot( m_CurrentSlot );
					}
					break;
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						bSlot = true;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						bDelSlot						  = true;
						m_bRemoveSlotIndex[CRYSTAL_SLOT5] = false;
						m_InventoryPos[ CRYSTAL_SLOT5 ]	  = 0;
						m_clCrystalItem[CRYSTAL_SLOT5].Init();
					}
					break;
			}
		}
		break;
	}

	// 슬롯을 선택.
	if( true == bSlot )
	{
	/*	cltItem clitem;
		clitem.siUnique  = 1;
		clitem.siItemNum = 1;
		InsertListViewItem( (CListView*)pvoidControl, &clitem );*/
	}

	// 슬롯에 아이템을 삭제 하면....
	if( true == bDelSlot )
	{
		// 장비 슬롯은 보이는 것만 제거.
		if( ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM == nControlID )
		{
			DelAllListViewItem();
		}
		// 결정체 슬롯은 실제로 결정체를 제거하기 위한 작업을 한다.
		else
		{
			//DelListViewItem( (CListView*)pvoidControl ); 
		}

	}
}

bool NArtifactRemoveDlg::InsertListViewItem( CListView* _pListView, cltItem* _cltItem )
{
	// 슬롯 범위 조사.
	if( false == CanInsert() )
	{
		return false;
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont  = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ m_CurrentSlot ].Set( _cltItem );			

	if ( m_clArtifactItem[ m_CurrentSlot ].siUnique > 0 )
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ m_CurrentSlot ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ m_CurrentSlot ].siUnique );

			if (ItemName)
			{
				pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ m_CurrentSlot ], ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, m_clArtifactItem[ m_CurrentSlot ].siItemNum, ItemName, ToolTipMsg );

				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   

				EquipExplain( _cltItem );
			}
		}
	}
	
	InsertEquipSetCrystal();

	return true;
}

void NArtifactRemoveDlg::DelListViewItem( CListView* _pListView )
{
	if( false == CanDel() )
		return;

	m_clArtifactItem[ m_CurrentSlot ].Init();
	_pListView->DeleteAllItems();
}

void NArtifactRemoveDlg::DelAllListViewItem( void )
{
	ZeroMemory( m_clArtifactItem,	sizeof( m_clArtifactItem ) );
	ZeroMemory( m_bRemoveSlotIndex, sizeof( m_bRemoveSlotIndex ) );
	ZeroMemory( m_clCrystalItem,	sizeof( m_clCrystalItem ) );
		
	CStatic*	pStatic	  = NULL;
	CEdit*		pEdit	  = NULL;
	CListView*	pListView = NULL;

	for( SI32 SlotCount = 0; SlotCount < SLOT_COUNT; ++ SlotCount )
	{
		pListView  = m_InterfaceFactory.GetListView( ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM + SlotCount );
		pListView->DeleteAllItems();

		//pStatic	   = m_InterfaceFactory.GetStatic(ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT1 + SlotCount );	
		//pStatic->SetText("");

		pEdit	   = m_InterfaceFactory.GetEdit(ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT1 + SlotCount );	
		pEdit->SetText("");
	}
}


bool NArtifactRemoveDlg::CanInsert( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot >= SLOT_COUNT )
		return false;

	if( 0 < m_clArtifactItem[ m_CurrentSlot ].siUnique 	|| 0 < m_clArtifactItem[ m_CurrentSlot ].siItemNum )
		return false;

	return true;
}

bool NArtifactRemoveDlg::CanDel( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot >= SLOT_COUNT )
		return false;

	if( 0 >= m_clArtifactItem[ m_CurrentSlot ].siUnique 	|| 0 >= m_clArtifactItem[ m_CurrentSlot ].siItemNum )
		return false;

	return true;
}
SI32 NArtifactRemoveDlg::RemoveItemPos( void )
{
	SI32 pos = pclClient->pclCM->CR[1]->pclCI->clCharItem.FindItemInv( 1 );
	if( 0 > pos )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( "알림", "인벤토리에 제거에 사용할 아이템이 없다." );
	}

	return pos;
}

void NArtifactRemoveDlg::InsertEquipSetCrystal( void )
{
	CStatic*	pStatic				= NULL;
	CEdit*		pEdit				= NULL;
	CListView*	pListView			= NULL;
	SI32		GlobalImageIndex	= -1;
	SI32		GlobalImageFont		= -1;


	cltItem				CrystalcltItem;
	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;
	SI32				siCrystalSlot = -1;
	for( SI32 Count = 0; Count < m_clArtifactItem[ m_CurrentSlot ].siUseCrystalNum; ++Count )
	{
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1 + Count );
		if(NULL == pListView)
			continue;

		pListView->DeleteAllItems();
		pEdit = m_InterfaceFactory.GetEdit(ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT1 + Count);
		// 결정체 아이템 정보 쓰기.
		CrystalExplain( pEdit, &m_clArtifactItem[EQUIP_SLOT], Count );  

		// 리스트 뷰를 채워준다.
		CrystalcltItem.siUnique  = m_clArtifactItem[ m_CurrentSlot ].cltItemCrytal[ Count ].siCrystalType;
		if(0 < CrystalcltItem.siUnique )
		{
			CrystalcltItem.siItemNum = 1;
		}

		//CrystalcltItem.clItemCrystal.siDurability		 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].GetDurability();
		CrystalcltItem.clItemCrystal.siMaxDurability	 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].GetMaxDurability();
		CrystalcltItem.clItemCrystal.uiCrystalAbility	 = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].uiCrystalAbility;
		CrystalcltItem.clItemCrystal.uiCrystalAbilityNum = m_clArtifactItem[ EQUIP_SLOT ].cltItemCrytal[ Count ].uiCrystalAbilityNum;

		// 결정체가 들어갈 슬롯에 결정체 그리기.
		siCrystalSlot = CRYSTAL_SLOT1 + Count;

		m_clArtifactItem[ siCrystalSlot ].Set( &CrystalcltItem );			

		if( m_clArtifactItem[ siCrystalSlot ].siUnique > 0 )
		{
			if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ siCrystalSlot ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ siCrystalSlot ].siUnique );

				if (ItemName)
				{
					pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ siCrystalSlot ], ToolTipMsg );

					ListViewItemData.Init();
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	m_clArtifactItem[ siCrystalSlot ].siItemNum, ItemName, ToolTipMsg );

					pListView->InsertItem( &ListViewItemData );   

					//pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_REMOVE_STATIC_CRYSTAL_REGISTER_SLOT1 + Count);
				}
			}
		}
	}
}

void NArtifactRemoveDlg::ClickRemoveSlot( SI32 _SlotIndex )
{
	m_bRemoveSlotIndex[ _SlotIndex ] = !( m_bRemoveSlotIndex[ _SlotIndex ] );
}

void NArtifactRemoveDlg::SendServerMsg( SI32 _MsgType )
{	
	SI32	MsgSize	 = 0;
	void*	pcltInfo = NULL;

	switch( _MsgType )
	{
		case GAMEMSG_REQUEST_ARTIFACT_REMOVE:
			{
				// 슬롯이 있는 아이템인가.
				BOOL bResult = pclClient->pclItemManager->CanCrystalRemoveData( &m_clArtifactItem[EQUIP_SLOT], &m_bRemoveSlotIndex[CRYSTAL_SLOT1], &m_clCrystalItem[CRYSTAL_SLOT1] );
				switch( bResult )	
				{
					case cltItemManagerCommon::NO_ITEM_UNIQUE:		NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);	return;
					case cltItemManagerCommon::NO_SLOT_ITEM:		NotifyMsg(cltItemManagerCommon::NO_SLOT_ITEM);		return;
					case cltItemManagerCommon::NO_CRYSTAL_SLOT:		NotifyMsg(cltItemManagerCommon::NO_CRYSTAL_SLOT);	return;
					case cltItemManagerCommon::NO_REMOVE_CRYSTAL:	NotifyMsg(cltItemManagerCommon::NO_REMOVE_CRYSTAL);	return;
					case cltItemManagerCommon::NO_LOW_DURABILITY:	NotifyMsg(cltItemManagerCommon::NO_LOW_DURABILITY);	return;					
				}

				// 아이템은 하나만 없앤다.
				for(SI16 count = CRYSTAL_SLOT1; count < TOTAL_SLOT_COUNT ; count++)
				{
					if(m_clCrystalItem[count].siUnique > 0)
						m_clCrystalItem[count].siItemNum = 1;
				}

				pcltInfo = new cltGameMsgRequest_Artifact_Remove(  m_InventoryPos[EQUIP_SLOT],	   &m_clArtifactItem[EQUIP_SLOT],
																   &m_InventoryPos[CRYSTAL_SLOT1],   &m_clCrystalItem[CRYSTAL_SLOT1],
																   &m_bRemoveSlotIndex[CRYSTAL_SLOT1]												);
				MsgSize  = sizeof( cltGameMsgRequest_Artifact_Remove );

				CButton* pButton = NULL;
				pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
				pButton->Enable(false); 
			}
			break;
	}

	cltMsg clMsg( _MsgType, MsgSize, (BYTE*)pcltInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	DeleteBuffer( _MsgType, pcltInfo );
}

void NArtifactRemoveDlg::DeleteBuffer( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
	case GAMEMSG_REQUEST_ARTIFACT_REMOVE:	delete (cltGameMsgRequest_Artifact_Remove*)_ClassData;			break;	
	}

}

bool NArtifactRemoveDlg::CanMixure( SI32 _ItemUnique )
{
	SI32 siSlotType = pclClient->pclItemManager->UsedSlotItem( _ItemUnique );

	//[진성] 아바타 결정체. 아바타들은 리무브를 사용할수 없다.
	if((cltItemManagerCommon::SLOTITEMTYPE_AVATAR == siSlotType) || (cltItemManagerCommon::SLOTITEMTYPE_DORMANCY == siSlotType))
	{
		return false;		
	}
	else
	{
		return true;
	}
}

bool NArtifactRemoveDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	if(m_bGaugeAction || m_bEffectAction)
		return true;

	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	CListView* pListView = m_InterfaceFactory.GetListView( pclClickAndDrop->m_siDstControlID );

	// 이미 슬롯에 올린 인벤의 아이템이 있다.
	if( true == SameCrystalInvenItem(pclClickAndDrop->m_siPara1) )
	{
		NotifyMsg(cltItemManagerCommon::ALREADY_USED_INVEN_ITEM);
		return true;
	}
	// 사용 아이템에 따라서 글자를 변경 해준다.
	SI32 siDropitemUnique = pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClickAndDrop->m_siPara1 ].siUnique	;
	TCHAR* pText  = NULL;
	switch ( siDropitemUnique )
	{
		// 리차지 결정체
	case ITEMUNIQUE(1804) :
		{
			CButton* pButton = NULL;
			pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
			pText = GetTxtFromMgr(9875);//충전하기
			pButton->SetText(pText);
			pButton->Refresh();
		}
		break;
		// 리무브 결정체
	case ITEMUNIQUE(1803) :
		{
			CButton* pButton = NULL;
			pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
			pText = GetTxtFromMgr(8209);//분리하기.
			pButton->SetText(pText);
			pButton->Refresh();
		}
		break;
	}
	switch ( pclClickAndDrop->m_siDstControlID )
	{
		case ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM:
			{
				ZeroMemory( m_clArtifactItem,	sizeof( m_clArtifactItem ) );
				ZeroMemory( m_bRemoveSlotIndex, sizeof( m_bRemoveSlotIndex ) );
				ZeroMemory( m_clCrystalItem,	sizeof( m_clCrystalItem ) );
				ZeroMemory( m_InventoryPos,		sizeof( m_InventoryPos ) );

				DelAllListViewItem();

				// 조합이 가능한 아이템인지 검사.
				if( false == CanMixure( pclClickAndDrop_Item->GetItem()->siUnique ) )
				{
					NotifyMsg(cltItemManagerCommon::NO_CRYSTAL);
					return true;
				}

				m_CurrentSlot				 = EQUIP_SLOT;
				m_InventoryPos[ EQUIP_SLOT ] = 	pclClickAndDrop->m_siPara1;
				
				if( false == InsertListViewItem( pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				 = NO_SLOT;
					m_InventoryPos[ EQUIP_SLOT ] = 	0;
				}
			}
			break;
		case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1:
			{
				m_CurrentSlot						= CRYSTAL_SLOT1;
				m_InventoryPos[ CRYSTAL_SLOT1 ] 	= pclClickAndDrop->m_siPara1;
				m_bRemoveSlotIndex[ CRYSTAL_SLOT1 ] = true;

				if( false == InsertCrystal( (CListView*)pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    	= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT1 ] 	= 	0;
					m_bRemoveSlotIndex[ CRYSTAL_SLOT1 ] = false;
				}

			}
			break;
		case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT2:
			{
				m_CurrentSlot						= CRYSTAL_SLOT2;
				m_InventoryPos[ CRYSTAL_SLOT2 ] 	= pclClickAndDrop->m_siPara1;
				m_bRemoveSlotIndex[ CRYSTAL_SLOT2 ] = true;

				if( false == InsertCrystal( (CListView*)pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    	= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT2 ] 	= 	0;
					m_bRemoveSlotIndex[ CRYSTAL_SLOT2 ] = false;
				}
			}
			break;
		case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT3:
			{
				m_CurrentSlot						= CRYSTAL_SLOT3;
				m_InventoryPos[ CRYSTAL_SLOT3 ]		= pclClickAndDrop->m_siPara1;
				m_bRemoveSlotIndex[ CRYSTAL_SLOT3 ] = true;

				if( false == InsertCrystal( (CListView*)pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    	= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT3 ] 	= 	0;
					m_bRemoveSlotIndex[ CRYSTAL_SLOT3 ] = false;
				}
			}
			break;
		case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT4:
			{
				m_CurrentSlot						= CRYSTAL_SLOT4;
				m_InventoryPos[ CRYSTAL_SLOT4 ] 	= pclClickAndDrop->m_siPara1;
				m_bRemoveSlotIndex[ CRYSTAL_SLOT4 ] = true;

				if( false == InsertCrystal( (CListView*)pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    	= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT4 ] 	= 	0;
					m_bRemoveSlotIndex[ CRYSTAL_SLOT4 ] = false;
				}
			}
			break;
		case ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT5:
			{
				m_CurrentSlot						= CRYSTAL_SLOT5;
				m_InventoryPos[ CRYSTAL_SLOT5 ]		= pclClickAndDrop->m_siPara1;
				m_bRemoveSlotIndex[ CRYSTAL_SLOT5 ] = true;

				if( false == InsertCrystal( (CListView*)pListView, pclClickAndDrop_Item->GetItem() ) )
				{
					m_CurrentSlot				    	= NO_SLOT;
					m_InventoryPos[ CRYSTAL_SLOT5 ] 	= 0;
					m_bRemoveSlotIndex[ CRYSTAL_SLOT5 ] = false;
				}
			}
			break;

	}

	return true;
}

bool NArtifactRemoveDlg::CanInsertCrystal( cltItem* _cltItem )
{
	if( CRYSTAL_SLOT1 > m_CurrentSlot || m_CurrentSlot >= SLOT_COUNT )
		return false;

	if( m_CurrentSlot > _cltItem->siUseCrystalNum )
		return false;

	if( 0 >= _cltItem->cltItemCrytal[ m_CurrentSlot-1 ].GetCrystalType() )
		return false;

	return true;
}

bool NArtifactRemoveDlg::IsCristal( SI32 _siItemUnique )
{
	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique( _siItemUnique );

	SI32 siType = pclClient->pclItemManager->pclItemInfo[ ref ]->siType; 

	if( ITEMTYPE_CRYSTAL == siType )
	{
		return true;
	}

	return false;
}

bool NArtifactRemoveDlg::InsertCrystal( CListView* _pListView, cltItem* _cltItem )
{
	 //ATB로 결정체 걸러내기
	 SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(_cltItem->siUnique);
	 if(ref <= 0)
		 return false;

	 if(pclClient->pclItemManager->pclItemInfo[ref]->siSetItemIndex > 0) 
		 return false ;

	 // 분리용 결정체여야 한다.
	 if(false == pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CRYSTAL_REMOVE))
		 return false;

	// 장비가 없으면 슬롯에 결정체도 못넣는다.
	if( 0 >= m_clArtifactItem[ EQUIP_SLOT ].siUnique )
		return false;

	// 슬롯 범위 조사.
	if( false == CanInsertCrystal( &m_clArtifactItem[ EQUIP_SLOT ] ) )
	{
		return false;
	}

	if( false == IsCristal( _cltItem->siUnique ) )
	{
		return false;
	}

	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	m_clCrystalItem[ m_CurrentSlot ].Set( _cltItem );			

	return true;
}


void NArtifactRemoveDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	if( 0 >= m_clArtifactItem[EQUIP_SLOT].siUnique )
		return;

	CListView* pListView = NULL;
	SI32		X_Add = 0;
	SI32		Y_Add = 0;     
	for( SI32 CrystalCount = 0; CrystalCount < m_clArtifactItem[EQUIP_SLOT].siUseCrystalNum; ++CrystalCount )
	{	  
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1 + CrystalCount );

		X_Add = GetX() + ScreenRX + pListView->GetX();
		Y_Add = GetY() + ScreenRY + pListView->GetY();
			
		// 화살표로 리무브 아이템을 넣으라고 보여주기.
		// 결정체가 존재하면 보이되 이미 제거하겠다고 리무브 결정체를 등록한 슬롯에는 보이지 않는다.
		if( 0 < m_clArtifactItem[EQUIP_SLOT].cltItemCrytal[CrystalCount].GetCrystalType() &&
			false == m_bRemoveSlotIndex[CRYSTAL_SLOT1 + CrystalCount]						)
		{
			static DWORD PrevTickCount = GetTickCount();
			static SI32  ImageFont	   = 1;
			if(GetTickCount() - PrevTickCount > 400)  
			{
				PrevTickCount = GetTickCount();
				ImageFont = !ImageFont;
			}
			//화살표 이미지는 리스트뷰보다 가로,세로 20이 더 크다. 그래서 위치 보정을 -10으로...
			GP.PutSprAlpha(m_Spr[IMAGE_ARROW_IN_OUT], X_Add - 10, Y_Add - 10, 16, ImageFont );   
		} 
		 
		// 제거하려고 리무브 결정체를 등록한 아이템이면 아이템 설명부분에 체크표시를 그린다.
		if( true == m_bRemoveSlotIndex[CRYSTAL_SLOT1 + CrystalCount] )
		{
			GP.PutSprAlpha(m_Spr[IMAGE_SLOT_REMOVE], X_Add + 42, Y_Add, 8, 0 );   

			// 이펙트 효과 그리기..
			if(true == m_bEffectAction)
			{
				CListView*	pListView = m_InterfaceFactory.GetListView( ARTIFACT_REMOVE_LISTVIEW_CRYSTAL_REGISTER_SLOT1 + CrystalCount);
				GP.PutSprScreen(m_Spr[IMAGE_EFFECT], GetX() + ScreenRX + pListView->GetX(), GetY() + ScreenRY + pListView->GetY(), m_siEffectCount );
				
				static DWORD StartTickCount = GetTickCount();
				if(50 < GetTickCount() - StartTickCount)  
				{ 
					StartTickCount = GetTickCount(); 

					m_siEffectCount++;  
				}
			}
		}     
	}  
	
	//게이지 효과 그리기
	if(true == m_bGaugeAction)
	{
		COutLine* pLineOut = m_InterfaceFactory.GetOutLine(ARTIFACT_REMOVE_OUTLINE_LINE4);

		GP.PutSprAlpha(m_Spr[IMAGE_GAGE_BACK],  GetX() + pLineOut->GetX() +  ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 8, 0, 0 );

		// 게이지 앞,뒤 이미지.
		GP.PutSpr(m_Spr[IMAGE_GAGE], GetX() + pLineOut->GetX() +  ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 0 );
		GP.PutSpr(m_Spr[IMAGE_GAGE], GetX() + pLineOut->GetX() + 306 +  ScreenRX, GetY() + pLineOut->GetY() + ScreenRY, 2 );

		for(SI32 Count = 0; Count < m_siGaugeCount; ++Count)  
		{
			GP.PutSpr(m_Spr[IMAGE_GAGE], GetX() + pLineOut->GetX() +  ScreenRX + 2 + (Count * 2), GetY() + pLineOut->GetY() + ScreenRY, 1 );
		}       
		m_siGaugeCount++;
	}

	
}

bool NArtifactRemoveDlg::SameCrystalInvenItem( SI32 _InvenPos)
{
	for(SI32 Count = CRYSTAL_SLOT1; Count < SLOT_COUNT; ++Count)
	{
		if(_InvenPos == m_InventoryPos[Count] )
			return true;
	}

	return false;
}

void NArtifactRemoveDlg::NotifyMsg( SI32 _MsgType )
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR* pTitle = NULL;
	TCHAR* pText  = NULL;

	if ( NULL == pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
		return;

	pTitle = GetTxtFromMgr(836);

	switch( _MsgType )		
	{
		case cltItemManagerCommon::ALREADY_USED_INVEN_ITEM: pText = GetTxtFromMgr(8090); break;
		case cltItemManagerCommon::NO_ITEM_UNIQUE:			pText = GetTxtFromMgr(8084); break;
		case cltItemManagerCommon::NO_SLOT_ITEM:			pText = GetTxtFromMgr(8082); break;
		case cltItemManagerCommon::NO_CRYSTAL_SLOT:			pText = GetTxtFromMgr(8085); break;
		case cltItemManagerCommon::NO_REMOVE_CRYSTAL:		pText = GetTxtFromMgr(8082); break;
		case cltItemManagerCommon::NO_LOW_DURABILITY:		pText = GetTxtFromMgr(8686); break;		
		case cltItemManagerCommon::NO_CRYSTAL		:		pText = GetTxtFromMgr(8402); break;
		default:
			// assert에 걸리면 위의 switch case타입을 추가해야 합니다.
			assert(0);
			break;
	}

	//  [7/13/2009 종호] 예외처리 코드 추가
	if(pText != NULL)
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
}

void NArtifactRemoveDlg::Action(void)
{
	// 게이지 효과 종료.
	if(GAGE_NUM <= m_siGaugeCount)
	{
		m_bGaugeAction = false;
		m_siGaugeCount = 0;

		m_bEffectAction = true;
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_ATIFACT")), 0, 0);
	}
	// 이펙트 효과 종료.
	if(EFFECT_NUM <= m_siEffectCount)
	{
		m_bEffectAction = false;
		m_siEffectCount = 0;

		SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_REMOVE );

		if( 0 < m_clRemoveSuccessItem.siUnique)
		{
			RemoveSuccess(&m_clRemoveSuccessItem);
		}
	}  
}

void NArtifactRemoveDlg::RemoveSuccess(cltItem* pEquip)
{
	ZeroMemory( m_clArtifactItem,	sizeof( m_clArtifactItem ) );
	ZeroMemory( m_bRemoveSlotIndex, sizeof( m_bRemoveSlotIndex ) );
	ZeroMemory( m_clCrystalItem,	sizeof( m_clCrystalItem ) );
	ZeroMemory( &m_InventoryPos[CRYSTAL_SLOT1],		sizeof( m_InventoryPos[CRYSTAL_SLOT1] ) * 5 );
	
	//DelAllListViewItem();

	m_CurrentSlot = 0;
	CListView* pListView  = m_InterfaceFactory.GetListView(ARTIFACT_REMOVE_LISTVIEW_EQUIP_REGISTER_ITEM);
	InsertListViewItem( pListView, pEquip );

	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
	pButton->Enable(true);
}

void NArtifactRemoveDlg::EquipExplain( cltItem* pEquip )
{
	CEdit*	pEdit		= m_InterfaceFactory.GetEdit( ARTIFACT_REMOVE_EDIT_CRYSTAL_REGISTER_SLOT6 );	
	TCHAR szbuffer[512] = "";

	SI32 MixtureSlotNum = 0;
	for(SI32 Count = 0; Count < pEquip->GetUseCrystalNum(); ++Count)
	{
		if(0 < pEquip->cltItemCrytal[Count].GetCrystalType())
			++MixtureSlotNum;
	}

	SI32	ref			  = pclClient->pclItemManager->FindItemRefFromUnique(pEquip->siUnique);	
	SI32	siLevel		  = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel;
	SI32    temCrystalNum = 0;
	if((siLevel >= 1)&&(siLevel <= 29))
	{
		temCrystalNum = 2;
	}
	else if((siLevel >= 30)&&(siLevel <= 59))
	{
		temCrystalNum = 3;
	}
	else if((siLevel >= 60)&&(siLevel <= 89))
	{
		temCrystalNum = 4;
	}
	else if(siLevel >= 90)
	{
		temCrystalNum = 5;
	}

	TCHAR* ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pEquip->siUnique );

	//StringCchPrintfA( szbuffer, sizeof( szbuffer ), GetTxtFromMgr(8378),	ItemName, 
	//																		pEquip->GetUseCrystalNum(),
	//																		temCrystalNum, 
	//																		MixtureSlotNum  );  

	NTCHARString256 strText = GetTxtFromMgr(8378);

	NTCHARString256 strName = (TCHAR*)pclClient->pclItemManager->GetName( pEquip->siUnique );	// 결정체 이름
	strText.Replace(TEXT("#ItemName#"), strName );

	SI16 strCurrentSlot = pEquip->GetUseCrystalNum();		// 현재 슬롯 수 
	strText.Replace(TEXT("#CurrentSlotNum#"), SI16ToString(strCurrentSlot) );

	SI32 strTotalSlot = temCrystalNum;						// 토탈 슬롯 수
	strText.Replace(TEXT("#TotalSlotNum#"), SI32ToString(strTotalSlot) );

	SI32 strMixureCrystalNum = MixtureSlotNum;				// 조합된 결정체
	strText.Replace(TEXT("#MitureCrystalNum#"), SI32ToString(strMixureCrystalNum) );

	pEdit->SetText( strText );  
}

void NArtifactRemoveDlg::CrystalExplain( CEdit* _pExplainEdit, cltItem* pEquipItem, SI16 _siSlot )
{
	TCHAR szbuffer[512]				= "";
	TCHAR buf[64]					= "";
	TCHAR CrystalReturnString[50]	= "";

	if(0 < pEquipItem->cltItemCrytal[_siSlot].GetCrystalType())
	{
		((cltItemManagerClient*)pclClient->pclItemManager)->GetCrystalAbilityTypeName(	buf, 
			pEquipItem->cltItemCrytal[_siSlot].GetCrystalAblilty(), 
			pEquipItem->cltItemCrytal[_siSlot].GetCrystalAbilityNum(), 
			CrystalReturnString );


		StringCchPrintfA( szbuffer, sizeof( szbuffer ), "%s\r\n %d / %d", 	CrystalReturnString,
			pEquipItem->cltItemCrytal[_siSlot].GetDurability() / 300,
			pEquipItem->cltItemCrytal[_siSlot].GetMaxDurability() / 300);

	}
	
	_pExplainEdit->SetText( szbuffer );
}

bool NArtifactRemoveDlg::IsRemove(void)
{
	// 슬롯이 있는 아이템인가.
	BOOL bResult = pclClient->pclItemManager->CanCrystalRemoveData( &m_clArtifactItem[EQUIP_SLOT], &m_bRemoveSlotIndex[CRYSTAL_SLOT1], &m_clCrystalItem[CRYSTAL_SLOT1] );
	switch( bResult )	
	{
		case cltItemManagerCommon::NO_ITEM_UNIQUE:		NotifyMsg(cltItemManagerCommon::NO_ITEM_UNIQUE);	return false;
		case cltItemManagerCommon::NO_SLOT_ITEM:		NotifyMsg(cltItemManagerCommon::NO_SLOT_ITEM);		return false;
		case cltItemManagerCommon::NO_CRYSTAL_SLOT:		NotifyMsg(cltItemManagerCommon::NO_CRYSTAL_SLOT);	return false;
		case cltItemManagerCommon::NO_REMOVE_CRYSTAL:	NotifyMsg(cltItemManagerCommon::NO_REMOVE_CRYSTAL);	return false;
	}

	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_REMOVE_BUTTON_REMOVE);
	pButton->Enable(false); 

	return true;
}
