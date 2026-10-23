#include "../Client/ninterface/NArtifactDlg/NArtifactRestoreDlg.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../CommonLogic/MsgType-Item.h"
#include "Item/ItemCommon/Itemunit.h"


NArtifactRestoreDlg::NArtifactRestoreDlg()
{
	ZeroMemory( m_clArtifactItem, sizeof( m_clArtifactItem ) );

	m_InventoryPos = -1;
	

	m_Spr[IMAGE_GAGE_BACK]	 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE_BACK  );
	m_Spr[IMAGE_GAGE]		 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_GAUGE	   );
	m_Spr[IMAGE_EFFECT]		 = pclClient->GetGlobalSpr( GIMG_ARTIFACT_SLOT_EFFECT );
	

	if(NULL == m_Spr[IMAGE_GAGE_BACK] || NULL == m_Spr[IMAGE_GAGE] || NULL == m_Spr[IMAGE_EFFECT])
	{
		MsgBox("이미지 얻기 실패", "NArtifactRestoreDlg");
	}

	m_siGaugeCount  = 0;
	m_siEffectCount = 0;
	m_bGaugeAction  = false;
	m_bEffectAction = false;
}

NArtifactRestoreDlg::~NArtifactRestoreDlg()
{

}

void NArtifactRestoreDlg::Clear( void )
{
	DelAllListViewItem();
	m_siGaugeCount  =  0;
	//m_siEffectCount =  0;
	m_bGaugeAction  = false;
	//m_bEffectAction = false;
}


void NArtifactRestoreDlg::Create(  CControllerMgr *pParent  )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NArtifactDlg/NArtifactRestoreDlg.ddf" ) );
	file.CreateChildDialog( this, ARTIFACT_RESTORE_DLG, TEXT( "dialog_restore" ), StaticNArtifactRestoreDlgProc, pParent );
	
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_RESTORE_BUTTON_RESTORE,			this ), ARTIFACT_RESTORE_BUTTON_RESTORE,		 TEXT( "button_restore" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		ARTIFACT_RESTORE_BUTTON_RECEIVE,			this ), ARTIFACT_RESTORE_BUTTON_RECEIVE,		 TEXT( "button_receive" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM,	this ), ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM, TEXT( "listview_equip_register_Item" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_SLOT1,			this ), ARTIFACT_RESTORE_LISTVIEW_SLOT1,		 TEXT( "listview_slot1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_SLOT2,			this ), ARTIFACT_RESTORE_LISTVIEW_SLOT2,		 TEXT( "listview_slot2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_SLOT3,			this ), ARTIFACT_RESTORE_LISTVIEW_SLOT3,		 TEXT( "listview_slot3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_SLOT4,			this ), ARTIFACT_RESTORE_LISTVIEW_SLOT4,		 TEXT( "listview_slot4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	ARTIFACT_RESTORE_LISTVIEW_SLOT5,			this ), ARTIFACT_RESTORE_LISTVIEW_SLOT5,		 TEXT( "listview_slot5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_RESTORE_OUTLINE1,					this ), ARTIFACT_RESTORE_OUTLINE1,				 TEXT( "outline_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_RESTORE_OUTLINE2,					this ), ARTIFACT_RESTORE_OUTLINE2,				 TEXT( "outline_NONAME2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_RESTORE_OUTLINE3,					this ), ARTIFACT_RESTORE_OUTLINE3,				 TEXT( "outline_NONAME3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_RESTORE_OUTLINE4,					this ), ARTIFACT_RESTORE_OUTLINE4,				 TEXT( "outline_NONAME4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	ARTIFACT_RESTORE_OUTLINE5,					this ), ARTIFACT_RESTORE_OUTLINE5,				 TEXT( "outline_NONAME5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		ARTIFACT_RESTORE_STATIC_NOMAME1,			this ), ARTIFACT_RESTORE_STATIC_NOMAME1,		 TEXT( "statictext_NONAME1" ) );	

	CStatic* pStatic = NULL;
	pStatic = m_InterfaceFactory.GetStatic(ARTIFACT_RESTORE_STATIC_NOMAME1);
	pStatic->SetTextPrintOption(DT_CENTER);
	pStatic->Refresh();
	
	COutLine* pOutLine = m_InterfaceFactory.GetOutLine(ARTIFACT_RESTORE_OUTLINE5);
	pOutLine->Show(false);

	// 이미 환원 된 아이템이 있을 수 있으므로 리스트를 요청한다.
	SendServerMsg( GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST );
}


void CALLBACK NArtifactRestoreDlg::StaticNArtifactRestoreDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NArtifactRestoreDlg *pThis = (NArtifactRestoreDlg*) pControllerMgr;
	pThis->NArtifactRestoreDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NArtifactRestoreDlg::NArtifactRestoreDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
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
		case ARTIFACT_RESTORE_BUTTON_RESTORE:
			{
				if( 0 > m_InventoryPos )
				{
					NotifyMsg(cltItemManagerCommon::NO_RESTORE_ITEM); 
					return;
				}

				if( 0 >= m_clArtifactItem[ EQUIP_SLOT ].siUnique )
				{
					NotifyMsg(cltItemManagerCommon::NO_RESTORE_ITEM);
					return;
				}
				TCHAR* pTitle = GetTxtFromMgr(578);
				TCHAR* pText  = GetTxtFromMgr(8393);

				stMsgBox MsgBox;
				CControllerMgr *pParent = this->GetParentDialog();
				if(pParent == NULL ) 
					return;

				MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );
			}
			break;
		case ARTIFACT_RESTORE_BUTTON_RECEIVE:
			{
				// 환원받은 아이템 수령 메시지.
				SendServerMsg( GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE );
			}
			break;
		case ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM:
			{
				switch( nEvent )
				{
					case EVENT_LISTVIEW_LBUTTONDOWN:
						{
							
						}
						break;
					case EVENT_LISTVIEW_LBUTTONUP:
						{
							
						}
						break;
					case EVENT_LISTVIEW_RBUTTONUP:
						{
							DelAllListViewItem();							
						}
						break;
				}
			}
			break;
	}
}

void NArtifactRestoreDlg::SetArtifactStorage(cltItem* _cltItemlist)
{
	memcpy(m_clDisasembleItemItemList, _cltItemlist, sizeof(m_clDisasembleItemItemList));

	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;
	
	// 환원된 아이템 정보를 담을 메모리 0으로 초기화.
	ZeroMemory( m_clArtifactItem, sizeof( m_clArtifactItem ) );

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	CListView*			pListView						   = NULL;
	stListViewItemData	ListViewItemData;
	
	pListView = m_InterfaceFactory.GetListView( ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM );
	pListView->DeleteAllItems();

	for ( SI32 i = CRYSTAL_SLOT1; i < SLOT_COUNT; ++i )
	{
		m_clArtifactItem[ i ].Set( &_cltItemlist[ i-1 ] );			
		
		// 넣을 리스트뷰를 얻고 아이템을 모조리 삭제.
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_RESTORE_LISTVIEW_SLOT1 + i-1 );
		pListView->DeleteAllItems();
		
		if ( m_clArtifactItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ i ].siUnique );

				if (ItemName)
				{
					ZeroMemory( ToolTipMsg, sizeof( ToolTipMsg ) );
					pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ i ], ToolTipMsg );
					
					ListViewItemData.Init();
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, m_clArtifactItem[ i ].siItemNum, ItemName, ToolTipMsg );
					
					pListView->InsertItem( &ListViewItemData );   
				}
			}
		}
	}
}


void NArtifactRestoreDlg::InsertEquipItem( CListView* _pListView, cltItem* _cltItem )
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont  = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	m_clArtifactItem[ EQUIP_SLOT ].Set( _cltItem );			

	if ( m_clArtifactItem[ EQUIP_SLOT ].siUnique > 0 )
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clArtifactItem[ EQUIP_SLOT ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clArtifactItem[ EQUIP_SLOT ].siUnique );

			if(ItemName)
			{
				pclClient->pclItemManager->GetExplain( &m_clArtifactItem[ EQUIP_SLOT ], ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, m_clArtifactItem[ EQUIP_SLOT ].siItemNum, ItemName, ToolTipMsg );

				_pListView->DeleteAllItems();
				_pListView->InsertItem( &ListViewItemData );   
			}
		}
	}
}

void NArtifactRestoreDlg::DelListViewItem( CListView* _pListView )
{
	if( false == CanDel() )
		return;

	m_clArtifactItem[ EQUIP_SLOT ].Init();
	_pListView->DeleteAllItems();
}

void NArtifactRestoreDlg::DelAllListViewItem( void )
{
	ZeroMemory( &m_clArtifactItem[0], sizeof( m_clArtifactItem[0] ) ); 

	CListView*	pListView = NULL;

	pListView  = m_InterfaceFactory.GetListView( ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM );
	pListView->DeleteAllItems();

	/*for( SI32 SlotCount = 0; SlotCount < SLOT_COUNT; ++ SlotCount )
	{
		pListView  = m_InterfaceFactory.GetListView( ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM + SlotCount );
		pListView->DeleteAllItems();
	}*/
}


bool NArtifactRestoreDlg::CanInsert( void )
{
	return true;
}

bool NArtifactRestoreDlg::CanDel( void )
{
	return true;
}

bool NArtifactRestoreDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
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
		case ARTIFACT_RESTORE_LISTVIEW_REGISTER_ITEM:
			{
				if(false == IsRestoreItemType(pclClickAndDrop_Item->GetItem()->siUnique))
				{
					NotifyMsg(cltItemManagerCommon::NO_RESTORE_ITEM_REGISTER);
					return true; 
				}

				if(false == IsEmptyDisasembleItem())
					return true;

				m_InventoryPos = pclClickAndDrop->m_siPara1;

				InsertEquipItem( (CListView*)pvoidControl, pclClickAndDrop_Item->GetItem() );
			}
			break;
	}

	return true;
}

void NArtifactRestoreDlg::SendServerMsg( SI32 _MsgType )
{
	SI32	MsgSize	 = 0;
	void*	pcltInfo = NULL;
	
	switch( _MsgType )
	{
		case GAMEMSG_REQUEST_DISASSEMBLEITEM:
			{
				if( 0 > m_InventoryPos )
					return;
				
				if( 0 >= m_clArtifactItem[ EQUIP_SLOT ].siUnique )
					return;

				pcltInfo = new cltGameMsgRequest_ItemDisAssemble( m_InventoryPos, &m_clArtifactItem[ EQUIP_SLOT ], ItemDisAssemble_NPC_KIND::NPC_ATIFACT );
				MsgSize  = sizeof( cltGameMsgRequest_ItemDisAssemble );
			}
			break;
		case GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST:
			{
				pcltInfo = new cltGameMsgRequest_ItemDisAssemble_List( ItemDisAssemble_NPC_KIND::NPC_ATIFACT );
				MsgSize  = sizeof( cltGameMsgRequest_ItemDisAssemble_List );
			}
			break;
		case GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE:
			{
				MsgSize  = sizeof( cltGameMsgRequest_ItemDisAssemble_Recieve );
				
				for ( SI32 i = CRYSTAL_SLOT1; i < SLOT_COUNT; ++i )
				{
					if( 0 < m_clArtifactItem[ i ].siUnique && 0 < m_clArtifactItem[ i ].siItemNum )
					{
						cltGameMsgRequest_ItemDisAssemble_Recieve cltInfo( i-1, &m_clArtifactItem[ i ], ItemDisAssemble_NPC_KIND::NPC_ATIFACT );		

						// 연속해서 요청이 들어가야해서 추가했다. 
						cltMsg clMsg( _MsgType, MsgSize, (BYTE*)&cltInfo );
						pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
					}
				}
				return;	// 요거는 밑에서 메시지를 보내면 안돼서 리턴함.
			}
			break;
	}
	
	cltMsg clMsg( _MsgType, MsgSize, (BYTE*)pcltInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	DeleteBuffer( _MsgType, pcltInfo );
}

void NArtifactRestoreDlg::DeleteBuffer( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		case GAMEMSG_REQUEST_DISASSEMBLEITEM:			delete (cltGameMsgRequest_ItemDisAssemble*)_ClassData;			break;	
		case GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST:		delete (cltGameMsgRequest_ItemDisAssemble_List*)_ClassData;		break;	
		case GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE:	delete (cltGameMsgRequest_ItemDisAssemble_Recieve*)_ClassData;	break;	
	}

}

void NArtifactRestoreDlg::Action(void)
{
	// 게이지 효과 종료.
	if(GAGE_NUM <= m_siGaugeCount)
	{
		m_bGaugeAction = false;
		m_siGaugeCount = 0;

		m_bEffectAction = true;
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_ATIFACT")), 0, 0);

		SendServerMsg( GAMEMSG_REQUEST_DISASSEMBLEITEM );
	}
	// 이펙트 효과 종료.
	if(EFFECT_NUM <= m_siEffectCount)
	{ 
		m_bEffectAction = false;
		m_siEffectCount = 0;

		//SetArtifactStorage(m_clDisasembleItemItemList);
	}  
}

void NArtifactRestoreDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	//게이지 효과 그리기
	if(true == m_bGaugeAction)
	{
		COutLine* pLineOut = m_InterfaceFactory.GetOutLine( ARTIFACT_RESTORE_OUTLINE5 );

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



	if( 0 >= m_clArtifactItem[EQUIP_SLOT].siUnique )  
		int a =0;//return;

	CListView* pListView = NULL;
	SI32		X_Add = 0;
	SI32		Y_Add = 0;        
	SI32		Alpha = 0;
	for( SI32 CrystalCount = 0; CrystalCount < MAX_CRYSTAL_NUM; ++CrystalCount )
	{	
		if(0 >= m_clDisasembleItemItemList[CrystalCount].siUnique)
			continue;
		
		pListView = m_InterfaceFactory.GetListView( ARTIFACT_RESTORE_LISTVIEW_SLOT1 + CrystalCount );

		X_Add = ScreenRX + pListView->GetX();
		Y_Add = ScreenRY + pListView->GetY();

		// 이펙트 효과 그리기..
		if(true == m_bEffectAction)
		{
			GP.PutSprScreen(m_Spr[IMAGE_EFFECT], GetX() +  X_Add, GetY() + Y_Add, m_siEffectCount );
		}
	}  

	// 이펙트 효과 카운트.
	if(true == m_bEffectAction)
	{
		static DWORD StartTickCount = GetTickCount();
		DWORD EndTickCount	= GetTickCount();

		if(50 < EndTickCount - StartTickCount) 
		{ 
			StartTickCount = EndTickCount; 

			m_siEffectCount++;  
		}
	}
}

void NArtifactRestoreDlg::SetDisasembleItem(cltItem* _cltItemlist)
{
	m_bGaugeAction = true;

	memcpy(m_clDisasembleItemItemList, _cltItemlist, sizeof(m_clDisasembleItemItemList));	
}

void NArtifactRestoreDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 1:
		{
			if(RetValue)
			{
				m_bGaugeAction = true;
			}
		}
		break;
	}
}

bool NArtifactRestoreDlg::IsEmptyDisasembleItem(void)
{
	for ( SI32 i = CRYSTAL_SLOT1; i < SLOT_COUNT; ++i )
	{
		if(0 < m_clArtifactItem[ i ].siUnique)
		{
			NotifyMsg(cltItemManagerCommon::ALREADY_RESULT_RESTORE_ITEM);
			return false;
		}
	}

	return true;
} 

bool NArtifactRestoreDlg::IsRestoreItemType(SI32 _siItemUnique)
{
	// [기형] 휴면 장비 아이템인지 확인
	if( NULL == pclClient)
	{
		return false;
	}

	CDormancySystem *pclDormancySystem = pclClient->pclDormancySystem;
	if( NULL != pclDormancySystem)
	{
		CDormancyEquipItemRewardClient *pclDormancyClient = (CDormancyEquipItemRewardClient *)pclDormancySystem->GetDormancyEquipItemReward();
		if( NULL != pclDormancyClient)
		{
			if( TRUE == pclDormancyClient->IsDormancyEquipItem(_siItemUnique))
			{
				return false;
			}
		}
	}

	SI32 itemtype = pclClient->pclItemManager->GetItemType(_siItemUnique) ;
	switch(itemtype)
	{
		case ITEMTYPE_SWORD:	
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_AXE:	
		case ITEMTYPE_BOW:	
		case ITEMTYPE_GUN:	
		case ITEMTYPE_STAFF:	
		case ITEMTYPE_HELMET:	
		case ITEMTYPE_ARMOUR:	
		case ITEMTYPE_BELT:	
		case ITEMTYPE_SHOES:	return true;	
		//case ITEMTYPE_NECK:	
		//case ITEMTYPE_RING:		
		//case ITEMTYPE_CRYSTAL:	
		//case ITEMTYPE_HAT:	
		//case ITEMTYPE_DRESS:	
		//case ITEMTYPE_MANTLE:	
	}
	return false;
}


void NArtifactRestoreDlg::NotifyMsg( SI32 _MsgType )
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
	case cltItemManagerCommon::ALREADY_USED_INVEN_ITEM: 	pText = GetTxtFromMgr(8090); break;
	case cltItemManagerCommon::SAME_CRYSTAL:				pText = GetTxtFromMgr(8089); break;
	case cltItemManagerCommon::NO_ITEM_UNIQUE:				pText = GetTxtFromMgr(8084); break;
	case cltItemManagerCommon::NO_SLOT_ITEM:				pText = GetTxtFromMgr(8082); break;
	case cltItemManagerCommon::NO_USED_SLOT:				pText = GetTxtFromMgr(8086); break;
	case cltItemManagerCommon::NO_CRYSTAL:					pText = GetTxtFromMgr(8082); break;
	case cltItemManagerCommon::ALL_EMPTY_CRYSTAL:			pText = GetTxtFromMgr(8082); break;
	case cltItemManagerCommon::ALREADY_USE_SLOT:			pText = GetTxtFromMgr(8088); break;
	case cltItemManagerCommon::LIMIT_LEVEL:					pText = GetTxtFromMgr(8357); break;
	case cltItemManagerCommon::LIMIT_CRYSTAL_ABILITY:		pText = GetTxtFromMgr(8358); break;
	case cltItemManagerCommon::NO_RESTORE_ITEM:				pText = GetTxtFromMgr(8398); break;
	case cltItemManagerCommon::NO_RESTORE_ITEM_REGISTER:	pText = GetTxtFromMgr(8399); break;
	case cltItemManagerCommon::ALREADY_RESULT_RESTORE_ITEM:	pText = GetTxtFromMgr(8400); break;
	}

	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
}