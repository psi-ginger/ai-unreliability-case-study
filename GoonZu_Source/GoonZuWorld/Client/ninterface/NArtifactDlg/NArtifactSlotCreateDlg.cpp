#include "../Client/ninterface/NArtifactDlg/NArtifactSlotCreateDlg.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../CommonLogic/MsgType-Item.h"
#include "Item/ItemCommon/Itemunit.h"

NArtifactSlotCreateDlg::NArtifactSlotCreateDlg()
{
	m_Spr[IMAGE_GAGE_BACK]	 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE_BACK  );
	m_Spr[IMAGE_GAGE]		 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE	   );
	m_Spr[IMAGE_EFFECT]		 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_EFFECT );

	if(NULL == m_Spr[IMAGE_GAGE_BACK] || NULL == m_Spr[IMAGE_GAGE] || NULL == m_Spr[IMAGE_EFFECT])
	{
		MsgBox("이미지 얻기 실패", "NArtifactSlotCreateDlg");
	}

	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;
}

NArtifactSlotCreateDlg::~NArtifactSlotCreateDlg()
{

}

void NArtifactSlotCreateDlg::Clear( void )
{
	DelAllListViewItem();
	m_CurrentSlot   = -1;
	m_siGaugeCount  =  0;
	m_siEffectCount =  0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;


	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_SLOT_CREATE_BUTTON_CREATE);
	pButton->Enable(true);

}


void NArtifactSlotCreateDlg::Create(  CControllerMgr *pParent  )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NArtifactDlg/NArtifactSlotCreateDlg.ddf" ) );
	file.CreateChildDialog( this, ARTIFACT_SLOT_CREATE_DLG, TEXT( "dialog_slot_create" ), StaticNArtifactSlotCreateDlgProc, pParent );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_SLOT_CREATE_BUTTON_STATIC_REGISTER_ITEM,		this ), ARTIFACT_SLOT_CREATE_BUTTON_STATIC_REGISTER_ITEM,	 TEXT( "button_static_register_item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_SLOT_CREATE_BUTTON_STATIC_REGISTER_CRYSTAL,	this ), ARTIFACT_SLOT_CREATE_BUTTON_STATIC_REGISTER_CRYSTAL, TEXT( "button_static_register_crystal" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_SLOT_CREATE_BUTTON_STATIC_NEW_ITEM,			this ), ARTIFACT_SLOT_CREATE_BUTTON_STATIC_NEW_ITEM,		 TEXT( "button_static_New_Item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_SLOT_CREATE_BUTTON_CREATE,						this ), ARTIFACT_SLOT_CREATE_BUTTON_CREATE,					 TEXT( "button_slot_create" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM,			this ), ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM,		 TEXT( "listview_equip_register" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_CRYSTAL,			this ), ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_CRYSTAL,		 TEXT( "listview_crystal_register" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_SLOT_CREATE_LISTVIEW_RESULT_ITEM,				this ), ARTIFACT_SLOT_CREATE_LISTVIEW_RESULT_ITEM,			 TEXT( "listview_result_item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE1,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE1,					 TEXT( "outline_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE2,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE2,					 TEXT( "outline_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE3,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE3,					 TEXT( "outline_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE4,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE4,					 TEXT( "outline_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE5,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE5,					 TEXT( "outline_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE6,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE6,					 TEXT( "outline_NONAME6" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_SLOT_CREATE_OUTLINE_LINE7,						this ), ARTIFACT_SLOT_CREATE_OUTLINE_LINE7,					 TEXT( "outline_NONAME7" ) );	
	
	COutLine* pOutLine = m_InterfaceFactory.GetOutLine(ARTIFACT_SLOT_CREATE_OUTLINE_LINE7);
	pOutLine->Show(false);
}


void CALLBACK NArtifactSlotCreateDlg::StaticNArtifactSlotCreateDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NArtifactSlotCreateDlg *pThis = (NArtifactSlotCreateDlg*) pControllerMgr;
	pThis->NArtifactSlotCreateDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NArtifactSlotCreateDlg::NArtifactSlotCreateDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	if(m_bGaugeAction || m_bEffectAction)
		return;

	cltClient*  pclclient     = (cltClient *)pclClient;
	void*	    pvoidControl  = m_InterfaceFactory.GetControl( nControlID );


	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	
	case ARTIFACT_SLOT_CREATE_BUTTON_CREATE:
		{
			if(false == IsSlotCreateMsg())
			{
				NotifyMsg( cltItemManagerCommon::NO_ITEM_UNIQUE );
				return;
			}
			TCHAR* pTitle = GetTxtFromMgr(578);
			TCHAR* pText  = GetTxtFromMgr(8360);

			stMsgBox MsgBox;
			CControllerMgr *pParent = this->GetParentDialog();
			if(pParent == NULL ) 
				return;

			MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		}
		break;
	case ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM:
		{
			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						int a = 0;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						m_InterfaceFactory.GetListView(ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM)->DeleteAllItems();
						m_InterfaceFactory.GetListView(ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_CRYSTAL)->DeleteAllItems();
						
						ZeroMemory(m_InventoryPos,		sizeof(m_InventoryPos));		
						ZeroMemory(m_clArtifactItem,	sizeof(m_clArtifactItem));		
					}
					break;
			}
		}	
		break;
	case ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_CRYSTAL:
		{	
			switch( nEvent )
			{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						int a = 0;
					}
					break;
				case EVENT_LISTVIEW_RBUTTONUP:
					{
						int a = 0;
					}
					break;
			}
		}
		break;

	}
}


bool NArtifactSlotCreateDlg::InsertEquip( CListView* _pListView, cltItem* _cltItem )
{
	// 슬롯 범위 조사.
	if( false == CanInsertEquip() )
	{
		return false; 
	}

	// 아이템이 슬롯을 사용 할 수 있는 아이템인가. - 아바타는 사용할 수 없다.
	if (cltItemManagerCommon::SLOTITEMTYPE_WEAPON != pclClient->pclItemManager->UsedSlotItem( _cltItem->siUnique ) )
	{
		NotifyMsg( cltItemManagerCommon::NO_ITEM_UNIQUE );
		return false;		
	}

	DelAllListViewItem();

	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ m_CurrentSlot ].Set( _cltItem );			

	if( m_clArtifactItem[ m_CurrentSlot ].siUnique > 0 )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( _cltItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( _cltItem->siUnique );

			if (ItemName)
			{
				pclClient->pclItemManager->GetExplain( _cltItem, ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	_cltItem->siItemNum, ItemName, ToolTipMsg );

				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   
			}
		}
	}

	return true; 
}

bool NArtifactSlotCreateDlg::InsertCrystal( CListView* _pListView, cltItem* _cltItem )
{
	//ATB로 결정체 걸러내기
	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(_cltItem->siUnique);
	if(ref <= 0)
		return false;

	if(pclClient->pclItemManager->pclItemInfo[ref]->siSetItemIndex > 0) 
		return false ;

	// 슬롯생성용 결정체여야 한다.
	if(false == pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(ITEMINFOATB_CRYSTAL_SLOT_CREATE))
	{ 
		NotifyMsg(cltItemManagerCommon::DIFFERENT_CRYSTAL);
		return false;
	}
    
	// 슬롯 범위 조사.
	if( false == CanInsertCrystal( _cltItem ) )
	{
		return false;
	}

	if( false == IsCristal( _cltItem->siUnique ) )
	{
		return false;
	}

	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ m_CurrentSlot ].Set( _cltItem );			

	if( m_clArtifactItem[ m_CurrentSlot ].siUnique > 0 )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ m_CurrentSlot ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ m_CurrentSlot ].siUnique );

			if (ItemName)
			{
				pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ m_CurrentSlot ], ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	m_clArtifactItem[ m_CurrentSlot ].siItemNum, ItemName, ToolTipMsg );

				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   
			}
		}
	}

	return false;
}

void NArtifactSlotCreateDlg::DelListViewItem( CListView* _pListView )
{
	if( false == CanDel() )
		return;

	m_clArtifactItem[ m_CurrentSlot ].Init();
	_pListView->DeleteAllItems();
}

void NArtifactSlotCreateDlg::DelAllListViewItem( void )
{
	ZeroMemory( m_clArtifactItem, sizeof( m_clArtifactItem ) ); 

	CListView*	pListView = NULL;

	SI32 DelListViewCount = USED_SLOT_COUNT + 1; // +1 은 결과 아이템이 보이는 슬롯이다.

	for( SI32 SlotCount = 0; SlotCount < DelListViewCount; ++ SlotCount )
	{
		pListView  = m_InterfaceFactory.GetListView( ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM + SlotCount );
		pListView->DeleteAllItems();
	}
}


bool NArtifactSlotCreateDlg::CanInsertEquip( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot > EQUIP_SLOT )
		return false;

	//추가 되면 결정체 슬롯에 관련 슬롯이 보이게 세팅하는부분을 추가하자.
	

	return true;
}

bool NArtifactSlotCreateDlg::CanInsertCrystal( cltItem* _cltItem )
{
	if( CRYSTAL_SLOT > m_CurrentSlot || m_CurrentSlot >= USED_SLOT_COUNT )
		return false;

	// 장비 슬롯에 장비가 없으면.
	if( 0 >= m_clArtifactItem[EQUIP_SLOT].siUnique )
	{
		NotifyMsg( cltItemManagerCommon::NO_ITEM_UNIQUE );
		return false;
	}

	SI32	ref			  = pclClient->pclItemManager->FindItemRefFromUnique(m_clArtifactItem[EQUIP_SLOT].siUnique);	
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

	// 더이상 생성할 슬롯이 없다면.
	if( temCrystalNum <= m_clArtifactItem[EQUIP_SLOT].siUseCrystalNum )
	{
		NotifyMsg( cltItemManagerCommon::FULL_CRYSTAL_SLOT );
		return false;
	}
	return true;
}

bool NArtifactSlotCreateDlg::CanDel( void )
{
	if( 0 > m_CurrentSlot || m_CurrentSlot >= USED_SLOT_COUNT )
		return false;

	if( 0 >= m_clArtifactItem[ m_CurrentSlot ].siUnique 	|| 0 >= m_clArtifactItem[ m_CurrentSlot ].siItemNum )
		return false;

	return true;
}

bool NArtifactSlotCreateDlg::IsCristal( SI32 _siItemUnique )
{
	SI32 ref	= pclClient->pclItemManager->FindItemRefFromUnique( _siItemUnique );

	SI32 siType = pclClient->pclItemManager->pclItemInfo[ ref ]->siType; 

	if( ITEMTYPE_CRYSTAL == siType )
	{
		return true;
	}

	return false;
}

void NArtifactSlotCreateDlg::SlotCreateSuccess( cltItem* _cltItem )
{
	ZeroMemory( m_clArtifactItem, sizeof( m_clArtifactItem ) ); 

	DelAllListViewItem();  
		
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;
	
	CListView*			pListView = m_InterfaceFactory.GetListView( ARTIFACT_SLOT_CREATE_LISTVIEW_RESULT_ITEM );

	if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( _cltItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		if( 0 >= _cltItem->siUnique )
			return;

		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( _cltItem->siUnique );

		if (ItemName)
		{
			pclClient->pclItemManager->GetExplain( _cltItem, ToolTipMsg );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	_cltItem->siItemNum, ItemName, ToolTipMsg );

			pListView->DeleteAllItems();
			pListView->InsertItem( &ListViewItemData );   
		}
	}

	CButton* pButton = NULL;
	pButton = m_InterfaceFactory.GetButton(ARTIFACT_SLOT_CREATE_BUTTON_CREATE);
	pButton->Enable(true);
}

bool NArtifactSlotCreateDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	if(m_bGaugeAction || m_bEffectAction)
		return true;

	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	void*	pvoidControl  = m_InterfaceFactory.GetControl( pclClickAndDrop->m_siDstControlID );

	switch ( pclClickAndDrop->m_siDstControlID )
	{
		case ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_ITEM:
			{
				m_CurrentSlot = EQUIP_SLOT;

				m_InventoryPos[ EQUIP_SLOT ] = pclClickAndDrop->m_siPara1;

				InsertEquip( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() );
			}
			break;
		case ARTIFACT_SLOT_CREATE_LISTVIEW_REGISTER_CRYSTAL:
			{
				m_CurrentSlot = CRYSTAL_SLOT;

				m_InventoryPos[ CRYSTAL_SLOT ] = pclClickAndDrop->m_siPara1;

				InsertCrystal( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() );
			}
			break;
	}

	return true;
}

void NArtifactSlotCreateDlg::SendServerMsg( SI32 _MsgType )
{	
	SI32	MsgSize	 = 0;
	void*	pcltInfo = NULL;

	switch( _MsgType )
	{
		case GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE:
			{
				// 슬롯이 있는 아이템인가.
				BOOL bResult = pclClient->pclItemManager->CanCrystalSlotCreateData( &m_clArtifactItem[EQUIP_SLOT], &m_clArtifactItem[CRYSTAL_SLOT] );
				switch( bResult )					
				{
					case cltItemManagerCommon::NO_ITEM_UNIQUE:
					case cltItemManagerCommon::NO_SLOT_ITEM:
					case cltItemManagerCommon::USE_CRYSTAL_SLOT:
					case cltItemManagerCommon::NO_CRYSTAL:			return;
				}
				
				// 슬롯생성 아이템은 하나만 없앤다.
				m_clArtifactItem[CRYSTAL_SLOT].siItemNum = 1;
			
				pcltInfo = new cltGameMsgRequest_Artifact_Slot_Create(  m_InventoryPos[EQUIP_SLOT],		&m_clArtifactItem[EQUIP_SLOT], 
																		m_InventoryPos[CRYSTAL_SLOT],	&m_clArtifactItem[CRYSTAL_SLOT] );
				MsgSize  = sizeof( cltGameMsgRequest_Artifact_Slot_Create );

				CButton* pButton = NULL;
				pButton = m_InterfaceFactory.GetButton(ARTIFACT_SLOT_CREATE_BUTTON_CREATE);
				pButton->Enable(false);
			}
			break;
	}
	
	cltMsg clMsg( _MsgType, MsgSize, (BYTE*)pcltInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	DeleteBuffer( _MsgType, pcltInfo );
}


void NArtifactSlotCreateDlg::DeleteBuffer( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		case GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE:	delete (cltGameMsgRequest_Artifact_Slot_Create*)_ClassData;			break;	
	}

}


void NArtifactSlotCreateDlg::NotifyMsg( SI32 _MsgType )
{
	cltClient *pclclient = (cltClient*)pclClient;
	TCHAR* pTitle = NULL;
	TCHAR* pText  = NULL;

	if ( NULL == pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
	{
		return;
	}

	pTitle = GetTxtFromMgr(836);

	switch( _MsgType )		
	{
		case cltItemManagerCommon::NO_ITEM_UNIQUE:			pText = GetTxtFromMgr(8084); break;
		case cltItemManagerCommon::FULL_CRYSTAL_SLOT:		pText = GetTxtFromMgr(8087); break;
		case cltItemManagerCommon::DIFFERENT_CRYSTAL:		pText = GetTxtFromMgr(8402); break;
	}

	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
}

void NArtifactSlotCreateDlg::Action(void)
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
		
		//if( 0 < m_clCreateSlotSuccessItem.siUnique)
		{
			SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE );				
			//SlotCreateSuccess(&m_clCreateSlotSuccessItem);  
		}
	}  
}
void NArtifactSlotCreateDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	//게이지 효과 그리기
	if(true == m_bGaugeAction)
	{
		COutLine* pLineOut = m_InterfaceFactory.GetOutLine(ARTIFACT_SLOT_CREATE_OUTLINE_LINE7);

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

	// 이펙트 효과 그리기..
	if(true == m_bEffectAction)
	{
		CListView*	pListView = m_InterfaceFactory.GetListView( ARTIFACT_SLOT_CREATE_LISTVIEW_RESULT_ITEM );
		GP.PutSprScreen(m_Spr[IMAGE_EFFECT], GetX() + ScreenRX + pListView->GetX(), GetY() + ScreenRY + pListView->GetY(), m_siEffectCount );
		
		static DWORD StartTickCount = GetTickCount();
			   DWORD EndTickCount	= GetTickCount();
		if(50 < EndTickCount - StartTickCount) 
		{ 
			StartTickCount = EndTickCount; 

			m_siEffectCount++;  
		}
	}
}

bool NArtifactSlotCreateDlg::IsSlotCreateMsg(void)
{
	if(0 < m_clArtifactItem[EQUIP_SLOT].siUnique && 0 < m_clArtifactItem[CRYSTAL_SLOT].siUnique)
	{
		return true;
	}

	return false;
}


void NArtifactSlotCreateDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:
		{
			if(RetValue)
			{
				if(false == IsSlotCreateMsg())
					return;
				
				m_bGaugeAction = true;

				CButton* pButton = NULL;
				pButton = m_InterfaceFactory.GetButton(ARTIFACT_SLOT_CREATE_BUTTON_CREATE);
				pButton->Enable(false);
 
				//SendServerMsg( GAMEMSG_REQUEST_ARTIFACT_SLOT_CREATE );				
			}
		}
		break;
	default:	
		{

		}
		break;
	}
}


