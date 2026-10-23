#include "NItemRepair.h"

#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../ninterface/NNewInventory/NNewInventory.h"
#include "../../ninterface/nnewmakeitem/NNewMakeItemDlg.h"

#include "../../../CommonLogic/MsgType-Item.h"

extern cltCommonLogic* pclClient;

NItemRepairDlg::NItemRepairDlg()
{
	Init();
}

NItemRepairDlg::~NItemRepairDlg()
{
}

void NItemRepairDlg::Init()
{
	m_siRepairItemPos = 0;
	m_clRepairItem.Init();

	for( SI32 i=0; i<m_ExtraMaterial.GetSize(); i++ )
	{
		m_ExtraMaterial[i].Init();
		m_ExtraMaterialItem[i].Init();
	}
	m_siMode = 1;

	for( SI32 i=0; i<m_clBeforeItem.GetSize(); i++ )
	{
		m_clBeforeItem[i].Init();
	}

	m_siItemPersonID = 0;
	m_siItemCharUnique = 0;

	m_Money = 0;
}

void NItemRepairDlg::Create( CControllerMgr *pParent )
{
	if( IsCreate() )		return;

	CInterfaceFile file;

	file.LoadFile("NInterface/Data/NNewMakeItem/DLG_ItemRepair.ddf");

	file.CreateChildDialog( this, NITEMREPAIR_DLG,	"dialog_ItemRepair", StaticNItemRepairDlgProc, pParent );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NITEMREPAIR_STATICT_TITLE,				this),	NITEMREPAIR_STATICT_TITLE,			"statictext_title" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX,	NITEMREPAIR_COMBOBOX_SELECTITEM,		this),	NITEMREPAIR_COMBOBOX_SELECTITEM,	"combobox_SelectItem" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NITEMREPAIR_STATIC_PRICETITLE,			this),	NITEMREPAIR_STATIC_PRICETITLE,		"statictext_pricetitle" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NITEMREPAIR_EDITBOX_PRICE,				this),	NITEMREPAIR_EDITBOX_PRICE,			"editbox_price" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NITEMREPAIR_LISTVIEW_REPAIRITEM,		this),	NITEMREPAIR_LISTVIEW_REPAIRITEM,	"listview_RepairItem" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NITEMREPAIR_LISTVIEW_REPAIRSTONE,		this),	NITEMREPAIR_LISTVIEW_REPAIRSTONE,	"listview_RepairStone" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NITEMREPAIR_LISTVIEW_MATERIALS,			this),	NITEMREPAIR_LISTVIEW_MATERIALS,		"listview_Materials" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NITEMREPAIR_BUTTON_REPAIR,				this), NITEMREPAIR_BUTTON_REPAIR,			"button_Repair" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NITEMREPAIR_BUTTON_EXTRABUTTON,			this), NITEMREPAIR_BUTTON_EXTRABUTTON,		"button_ExtraButton" );

//	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,NITEMREPAIR_IMAGESTATIC_BACKGROUND,		this), NITEMREPAIR_IMAGESTATIC_BACKGROUND,	"imagestatic_background" );

	CStatic* pclStatic_Title = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATICT_TITLE );
	if( pclStatic_Title )
	{
		pclStatic_Title->SetFontColor( RGB(255, 255, 255));
		pclStatic_Title->Refresh();
	}

	CStatic* pclStatic_pricetitle = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATIC_PRICETITLE );
	if( pclStatic_pricetitle )
	{
		pclStatic_pricetitle->SetFontColor( RGB(255, 255, 255));
		pclStatic_pricetitle->Refresh();
	}

	Show( false );

	SetActionTimer( 300 );    
	SetTimerOnOff( true );
}

void CALLBACK NItemRepairDlg::StaticNItemRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NItemRepairDlg *pThis = (NItemRepairDlg*) pControllerMgr;
	pThis->NItemRepairDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NItemRepairDlg::NItemRepairDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if( pclchar == NULL )							return;

	switch( nControlID )
	{
	case NITEMREPAIR_EDITBOX_PRICE:
		{
			switch ( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					GMONEY gPrice = 0;
					CEdit* pclEdit_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
					if( pclEdit_Price )
					{
						gPrice = _atoi64( pclEdit_Price->GetText() );

						GMONEY gLimitePrice = pclClient->GetGlobalValue( TEXT("GV_Repair_MaxRequestMoney") );
						if ( gLimitePrice < gPrice )
						{
							pclEdit_Price->SetText( SI64ToString(gLimitePrice) );
						}
					}
				}
				break;
			}
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					CloseDialog();
				}
				break;
			}
		}
		break;

	case NITEMREPAIR_BUTTON_REPAIR:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltItem* pclitem = &m_clRepairItem;

					if ( (ITEMREPAIR_MODE_SELF == m_siMode) && (pclchar->CanRepairItem( pclitem ) == false) )
					{
						NTCHARString32	strTitle;
						NTCHARString128	strText;

						if ( 0 >= pclitem->siUnique )	// 수리할 아이템을 넣어주세요
						{
							strTitle	= GetTxtFromMgr(1058);
							strText		= GetTxtFromMgr(40271);
						}
						else							// 기술이 부족해서 수리할 수 없습니다
						{
							strTitle	= GetTxtFromMgr(1058);
							strText		= GetTxtFromMgr(40162);
						}

						if ( pclclient->m_pDialog[NOTIFYMSG_DLG] )
						{
							pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
						}

						return;
					}

					if ( false == pclClient->pclItemManager->IsRepairStone(m_ExtraMaterial[0].siItemUnique) )
					{
						// 수리석이 아닙니다
						return;
					}

					SI16 siNeedRepairStone = pclClient->pclItemManager->NeedRepairStoneNum( pclitem );
					if( m_ExtraMaterial[0].siUseItemNum < siNeedRepairStone )
					{
						// 수리석이 모자랍니다.
						return;
					}

					// 수리시 차감되는 최대 내구도 수치보다 작다면 축복의 돌이 있어야만 수리됨
					SI32 siNowMaxDur = pclitem->GetMaxDurability();
					SI32 siDecMaxDur = (SI32)pclClient->GetGlobalValue( TEXT("GV_Decrease_MaxDurability") );
					if ( siNowMaxDur <= siDecMaxDur )
					{
						SI32 siBlessingStoneUnique	= ITEMUNIQUE(5370);
						SI32 siNeedBlessingStone	= pclClient->pclItemManager->NeedBlessingStoneNum( pclitem, siBlessingStoneUnique );
						BOOL bCheckBlessingStone	= FALSE;

						// 축복의 돌 & 갯수를 검사한다
						for ( SI32 i=0; i<m_ExtraMaterial.GetSize(); i++ )
						{
							if ( (siBlessingStoneUnique == m_ExtraMaterial[i].siItemUnique) && (siNeedBlessingStone <= m_ExtraMaterial[i].siUseItemNum) )
							{
								bCheckBlessingStone = TRUE;
								break;
							}
						}

						// 돌이 없거나 모자르다
						if ( FALSE == bCheckBlessingStone )
						{
							NTCHARString32	strTitle	= GetTxtFromMgr( 422 );
							NTCHARString128	strText		= GetTxtFromMgr( 40172 );

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
							return;
						}
					}

					GMONEY gPrice = 0;
					CEdit* pclEdit_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
					if( pclEdit_Price )
					{
						gPrice = _atoi64( pclEdit_Price->GetText() );
					}

					GMONEY gLimitePrice = pclClient->GetGlobalValue( TEXT("GV_Repair_MaxRequestMoney") );
					if ( gLimitePrice < gPrice )
					{
						return;
					}

					// 수리 요청
					cltGameMsgRequest_NewItemRepair clNewNPCRepairMsg( m_siRepairItemPos, pclitem, gPrice, m_ExtraMaterial, m_siItemPersonID, m_siMode );
					cltMsg clMsg( GAMEMSG_REQUEST_NEWITEMREPAIR, sizeof(clNewNPCRepairMsg), (BYTE*)&clNewNPCRepairMsg );
					pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );
				}
				break;
			}
		}
		break;

	case NITEMREPAIR_BUTTON_EXTRABUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					switch( m_siMode )
					{
					// 최대 내구도 증가 
					case ITEMREPAIR_MODE_SELF:
						{
							cltItem* pclitem = &m_clRepairItem;

							// 최대내구도를 회복할수 있는 아이템 인가
							if ( false == pclClient->pclItemManager->CanMaxRepairItem(pclitem) )
							{
								return;
							}

							if ( (false == pclClient->pclItemManager->IsMaxRepairStone(m_ExtraMaterial[0].siItemUnique)) || (0 >= m_ExtraMaterial[0].siUseItemNum) )
							{
								return;
							}

							// 모드를 최대 내구도 증가 모드로 변경해서 보내줌 
							cltGameMsgRequest_NewItemRepair clNewNPCRepairMsg( m_siRepairItemPos, pclitem, 0, m_ExtraMaterial, m_siItemPersonID, ITEMREPAIR_MODE_GROWUP_MAX );
							cltMsg clMsg( GAMEMSG_REQUEST_NEWITEMREPAIR, sizeof(clNewNPCRepairMsg), (BYTE*)&clNewNPCRepairMsg );
							pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );

						}
						break;
					case ITEMREPAIR_MODE_REQUEST_PRECESS:// 창띄우고 종료누르면
						{
							CloseDialog();
							return;
						}
					case ITEMREPAIR_MODE_CANCEL:
						{
							// 수리 의뢰 취소
							cltGameMsgRequest_NewItemRepair clNewNPCRepairMsg( m_siRepairItemPos, &m_clRepairItem, 0, m_ExtraMaterial, m_siItemPersonID, m_siMode );
							cltMsg clMsg( GAMEMSG_REQUEST_NEWITEMREPAIR, sizeof(clNewNPCRepairMsg), (BYTE*)&clNewNPCRepairMsg );
							pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );
						}
						break;
					}
				}
				break;
			}
		}
		break;

	case NITEMREPAIR_COMBOBOX_SELECTITEM:
		{
			CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
			if( pclComboBox_SelectItem )
			{
				SI16 siSelected = pclComboBox_SelectItem->GetSelectedIndex();
				if( siSelected >= 0 )
				{
					stComboBoxItemData* pclItemData = pclComboBox_SelectItem->GetItemData( siSelected );
					if( pclItemData )
					{
						InsertRepairItem( pclItemData->siParam, &pclchar->pclCI->clCharItem.clItem[pclItemData->siParam] );
					}

					for( SI32 i=PERSONITEM_INV0; i<MAX_ITEM_PER_PERSON; i++ )
					{
						cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ i ];
						if( pclClient->pclItemManager->IsRepairStone( pclitem ))
						{
							m_siItemPersonID	= pclchar->pclCI->GetPersonID();
							m_siItemCharUnique	= pclchar->GetCharUnique();
							InsertStoneItem( i, pclitem );
							break;
						}
					}

					UpdateInterface();
				}
			}
		}
		break;
	case NITEMREPAIR_LISTVIEW_REPAIRITEM:
	case NITEMREPAIR_LISTVIEW_REPAIRSTONE:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					switch( nControlID )
					{
					case NITEMREPAIR_LISTVIEW_REPAIRITEM:
						{
							m_siRepairItemPos = 0;
							m_clRepairItem.Init();
						}
						break;

					case NITEMREPAIR_LISTVIEW_REPAIRSTONE:
						{
							m_ExtraMaterial[0].Init();
						}
						break;
					}

					UpdateInterface();
				}
				break;
			}
		}
		break;

	case NITEMREPAIR_LISTVIEW_MATERIALS:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					CListView* pkListView_Item = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_MATERIALS );

					m_ExtraMaterial[ pkListView_Item->GetSelectedItemIndex() + 1 ].Init();
					UpdateInterface();
				}
				break;
			}
		}
		break;
	}
}

void NItemRepairDlg::Show( bool bShow )
{
	CDialogController::Show( bShow );

	if( CDialogController::IsShow() )
	{
	}
}

void NItemRepairDlg::CloseDialog()
{
	cltClient* pclclient = (cltClient*)pclClient;

	Init();
	///Show(false)	;
	DeleteDialog();
	// 인벤토리 업데이트
	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	if( personinventory )		personinventory->LoadItemData();
	if( horseinventory )		horseinventory->LoadItemData();
	if( bag0inventory )			bag0inventory->LoadItemData();
	if( bag1inventory )			bag1inventory->LoadItemData();

	// 매인 make창을 종료 한다 .
	((CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG])->CloseDialog()	;
}

// 1:직접수리, 2:수리의뢰, 3:의뢰수리, 4:수리의뢰 중 [취소하는 창]
void NItemRepairDlg::SetMode( SI16 siMode )
{
	m_siMode = siMode;

	switch( siMode )
	{
	case ITEMREPAIR_MODE_SELF:	// 1:직접수리
		{
			CStatic* pclStatic_Title = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATICT_TITLE );
			if( pclStatic_Title )				pclStatic_Title->Show( true );

			CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
			if( pclComboBox_SelectItem )		pclComboBox_SelectItem->Show( true );

			CEdit* pclEditBox_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
			if( pclEditBox_Price )				pclEditBox_Price->Enable( false );

			CButton* pclButton_Repair = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_REPAIR );
			if( pclButton_Repair )				pclButton_Repair->SetText( GetTxtFromMgr(40121) );
			pclButton_Repair->Enable( true );

			CButton* pclButton_Extra = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_EXTRABUTTON );
			if( pclButton_Extra )				pclButton_Extra->SetText( GetTxtFromMgr(40132) );
			pclButton_Extra->Enable( true );
		}
		break;
	case ITEMREPAIR_MODE_REQUEST:	// 2:수리의뢰
		{
			CStatic* pclStatic_Title = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATICT_TITLE );
			if( pclStatic_Title )				pclStatic_Title->Show( true );

			CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
			if( pclComboBox_SelectItem )		pclComboBox_SelectItem->Show( true );

			CEdit* pclEditBox_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
			if( pclEditBox_Price )				
			{
				pclEditBox_Price->Enable( true );
				pclEditBox_Price->SetReadOnly( false );
			}

			CButton* pclButton_Repair = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_REPAIR );
			if( pclButton_Repair )				pclButton_Repair->SetText( GetTxtFromMgr(40129) );
			pclButton_Repair->Enable( true );

			CButton* pclButton_Extra = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_EXTRABUTTON );
			if( pclButton_Extra )				pclButton_Extra->SetText( GetTxtFromMgr(40130) );
			pclButton_Extra->Enable( false );
		}
		break;
	case ITEMREPAIR_MODE_REQUEST_PRECESS:	// 3:의뢰수리
		{
			CStatic* pclStatic_Title = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATICT_TITLE );
			if( pclStatic_Title )				pclStatic_Title->Show( false );

			CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
			if( pclComboBox_SelectItem )		pclComboBox_SelectItem->Show( false );

			CListView* pclListView_RepairItem = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRITEM );
			if( pclListView_RepairItem )		pclListView_RepairItem->Enable( false );

			CListView* pclListView_RepairStone = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRSTONE );
			if( pclListView_RepairStone )		pclListView_RepairStone->Enable( false );

			CListView* pclListView_Materials = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_MATERIALS );
			if( pclListView_Materials )			pclListView_Materials->Enable( false );

			CEdit* pclEditBox_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
			if( pclEditBox_Price )				pclEditBox_Price->SetReadOnly( true );
		
			CButton* pclButton_Repair = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_REPAIR );
			if( pclButton_Repair )				pclButton_Repair->SetText( GetTxtFromMgr(40131) );
			pclButton_Repair->Enable( true );

			CButton* pclButton_Extra = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_EXTRABUTTON );
			if( pclButton_Extra )				pclButton_Extra->SetText( GetTxtFromMgr(3303) );
			pclButton_Extra->Enable( true );
		}
		break;
	case ITEMREPAIR_MODE_CANCEL:	// 4:수리의뢰 중 [취소하는 창]
		{
			CStatic* pclStatic_Title = m_InterfaceFactory.GetStatic( NITEMREPAIR_STATICT_TITLE );
			if( pclStatic_Title )				pclStatic_Title->Show( false );

			CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
			if( pclComboBox_SelectItem )		pclComboBox_SelectItem->Show( false );

			CListView* pclListView_RepairItem = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRITEM );
			if( pclListView_RepairItem )		pclListView_RepairItem->Enable( false );

			CListView* pclListView_RepairStone = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRSTONE );
			if( pclListView_RepairStone )		pclListView_RepairStone->Enable( false );

			CListView* pclListView_Materials = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_MATERIALS );
			if( pclListView_Materials )			pclListView_Materials->Enable( false );

			CEdit* pclEditBox_Price = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
			if( pclEditBox_Price )				pclEditBox_Price->SetReadOnly( true );

			CButton* pclButton_Repair = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_REPAIR );
			if( pclButton_Repair )				pclButton_Repair->SetText( GetTxtFromMgr(40129) );
			pclButton_Repair->Enable( false );

			CButton* pclButton_Extra = m_InterfaceFactory.GetButton( NITEMREPAIR_BUTTON_EXTRABUTTON );
			if( pclButton_Extra )				pclButton_Extra->SetText( GetTxtFromMgr(40130) );
			pclButton_Extra->Enable( true );
		}
		break;
	}

	m_siRepairItemPos = 0;
	m_ExtraMaterial[0].Init();

	UpdateSelectItemComboBox();
	UpdateInterface();
}

void NItemRepairDlg::Action()
{
	if( CDialogController::IsShow() == false )		return;

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if( pclchar != NULL )
	{
		bool bChangeInv = false;
		for( SI32 i=0; i<m_clBeforeItem.GetSize(); i++ )
		{
			cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ i + PERSONITEM_INV0 ];
			if( pclitem == NULL )				continue;

			if( pclitem->IsSame( &m_clBeforeItem[i], true ) == false )
			{
				bChangeInv = true;
			}
		}
		
		if( bChangeInv )
		{
			UpdateSelectItemComboBox();

			for( SI32 i=0; i<m_clBeforeItem.GetSize(); i++ )
			{
				cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ i + PERSONITEM_INV0 ];
				if( pclitem == NULL )				continue;

				m_clBeforeItem[i].Set( pclitem );
			}
		}
	}
}

void NItemRepairDlg::UpdateSelectItemComboBox()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if( pclchar == NULL )							return;

	CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
	if( pclComboBox_SelectItem == NULL || pclComboBox_SelectItem->IsShow() == false )	return;

	stComboBoxItemData itemdata;
	pclComboBox_SelectItem->Clear();

	bool bCanRepair = false;

	for(SI32 i=PERSONITEM_INV0; i<MAX_ITEM_PER_PERSON; i++ )
	{
		cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ i ];
		if( pclitem == NULL )				continue;
		if( pclitem->siUnique <= 0 )		continue;

		// 직접수리일경우 기술도 체크한다
		if ( ITEMREPAIR_MODE_SELF == m_siMode )
		{
			bCanRepair = ( pclClient->pclItemManager->CanRepairItem( pclitem ) && pclchar->CanRepairItem( pclitem ) );
		}
		else
		{
			bCanRepair = pclClient->pclItemManager->CanRepairItem( pclitem );
		}

		if( bCanRepair )
		{
			itemdata.Init();
			itemdata.siParam = i;
			StringCchPrintf( itemdata.strText, sizeof(itemdata.strText), "%s(%d/%d)",
				pclClient->pclItemManager->GetName( pclitem->siUnique ), 
				pclitem->GetDurability(),
				pclitem->GetMaxDurability() );

			pclComboBox_SelectItem->AddItem( &itemdata );
		}
	}
}

void NItemRepairDlg::InsertRepairItem( SI16 siItemPos, cltItem* pclitem )
{
	if( pclitem == NULL )														return;
	if( pclitem->siUnique <= 0 )												return;

	m_siRepairItemPos = siItemPos;
	m_clRepairItem.Set( pclitem ); 
}

void NItemRepairDlg::InsertStoneItem( SI16 siItemPos, cltItem* pclitem )
{
	m_ExtraMaterial[0].siStgPos			= siItemPos;
	m_ExtraMaterial[0].siItemUnique		= pclitem->siUnique;
	m_ExtraMaterial[0].siUseItemNum		= pclitem->siItemNum;

	m_ExtraMaterialItem[0].Set( pclitem );
}

bool NItemRepairDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	stListViewItemData ListViewItemData;
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;
	TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if( pclchar == NULL )											return false;

	// 인벤토리에서 인벤토리로 이동하는 경우
	switch( pclClickAndDrop_Item->m_siSrcDialogID )
	{
	case NNEWINVENTORY_DLG:
	case NNEWINVENTORY_HORSE_DLG:
	case NNEWINVENTORY_BAG0_DLG:
	case NNEWINVENTORY_BAG1_DLG:
		{
			CListView* pkListView_Item = m_InterfaceFactory.GetListView( pclClickAndDrop_Item->m_siDstControlID );
			cltItem* pclItem = pclClickAndDrop_Item->GetItem();

			SI16 siNewIndex = 0;

			cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
			if( pclchar )
			{
				m_siItemPersonID = pclchar->pclCI->GetPersonID();
				m_siItemCharUnique	= pclchar->GetCharUnique();
			}

			switch( pclClickAndDrop_Item->m_siDstControlID )
			{
			case NITEMREPAIR_LISTVIEW_REPAIRITEM:
				{
					// 수리혹은 최대내구도 회복이 가능한 아이템만 등록 가능
					if( (false == pclClient->pclItemManager->CanRepairItem(pclItem)) && (false == pclClient->pclItemManager->CanMaxRepairItem(pclItem)) )
					{
						//return false;
					}
					else
					{
						InsertRepairItem( pclClickAndDrop_Item->m_siPara1, &pclchar->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1] );
					}

					InsertRepairItem( pclClickAndDrop_Item->m_siPara1, &pclchar->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1] );
				}
				break;
			case NITEMREPAIR_LISTVIEW_REPAIRSTONE:
				{
					if( true == pclClient->pclItemManager->IsRepairStone(pclItem) ) // 수리석일때
					{
						InsertStoneItem( pclClickAndDrop_Item->m_siPara1, &pclchar->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1] );
					}
					else if ( true == pclClient->pclItemManager->IsMaxRepairStone(pclItem) ) // 최대내구도 회복석일때
					{
						InsertStoneItem( pclClickAndDrop_Item->m_siPara1, &pclchar->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1] );
					}
					else
					{
						return true;
					}
				}
				break;

			case NITEMREPAIR_LISTVIEW_MATERIALS:
				{
					// 추가 재료인지 체크한다.
					if( pclClient->pclItemManager->IsRepairExtraMaterial( pclItem ) == false )		return true;
					
					siNewIndex = -1;
					for( SI32 i=0; i<m_ExtraMaterial.GetSize() - 1; i++ )
					{
						stListViewItemData* pkListViewItemData =  pkListView_Item->GetListViewItemData( i );
						if( pkListViewItemData == NULL )
						{
							siNewIndex = i;
							break;
						}
					}
					if( siNewIndex >= 0 )
					{
						m_ExtraMaterial[siNewIndex+1].siStgPos		= pclClickAndDrop_Item->m_siPara1;
						m_ExtraMaterial[siNewIndex+1].siItemUnique	= pclItem->siUnique;
						m_ExtraMaterial[siNewIndex+1].siUseItemNum	= pclItem->siItemNum;

						m_ExtraMaterialItem[siNewIndex+1].Set( &pclchar->pclCI->clCharItem.clItem[pclClickAndDrop_Item->m_siPara1] );
					}
				}
				break;
			default:
				{
					return false;
				}
				break;
			}

			switch( m_siMode )
			{
			case ITEMREPAIR_MODE_SELF:
				{
					m_Money = pclClient->pclItemManager->GetNPCRepairPrice( &m_clRepairItem );
				}
				break;
			}

			UpdateInterface();

			return true;
		}
		break;
	}

	return false;
}

void NItemRepairDlg::UpdateInterface()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 수리하려는 아이템
	stListViewItemData ListViewItemData;
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;
	TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

	CListView* pkListView_Item = NULL;
	cltItem* pclItem = NULL;

	// 수리하려는 아이템
	pclItem = NULL;
	pkListView_Item = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRITEM );
	if( m_siRepairItemPos > 0 && m_clRepairItem.siUnique > 0 )		pclItem = &m_clRepairItem;

	if( pkListView_Item)	pkListView_Item->DeleteAllItems();
	if( pkListView_Item && pclItem && pclItem->siUnique > 0 )
	{
		NTCHARString64 kItemName( pclClient->pclItemManager->GetName(pclItem->siUnique) );
		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont );
		pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );

		ListViewItemData.Init();
		ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, pclItem->siItemNum, kItemName, ToolTipMsg, false );
		pkListView_Item->InsertItem( &ListViewItemData );
	}

	// 선택된 아이템으로 콤보박스도 선택해 준다.
	CComboBox* pclComboBox_SelectItem = m_InterfaceFactory.GetComboBox( NITEMREPAIR_COMBOBOX_SELECTITEM );
	if( pclComboBox_SelectItem && pclComboBox_SelectItem->IsShow() )
	{
		for( SI32 i=0; i<pclComboBox_SelectItem->GetItemNum(); i++ )
		{
			stComboBoxItemData* pclItemData = pclComboBox_SelectItem->GetItemData( i );
			if( pclItemData )
			{
				if( pclItemData->siParam == m_siRepairItemPos )
				{
					pclComboBox_SelectItem->SetCursel( i, false );
					break;
				}
			}
		}
	}	

	// 수리석 아이템
	pclItem = NULL;
	pkListView_Item = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_REPAIRSTONE );
	if( m_ExtraMaterial[0].siStgPos > 0 && m_ExtraMaterialItem[0].siUnique > 0 )		pclItem = &m_ExtraMaterialItem[0];

	if( pkListView_Item)	pkListView_Item->DeleteAllItems();
	if( pkListView_Item && pclItem )
	{
		NTCHARString64 kItemName( pclClient->pclItemManager->GetName(pclItem->siUnique) );
		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont );
		pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );

		ListViewItemData.Init();
		ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, pclItem->siItemNum, kItemName, ToolTipMsg, false );
		pkListView_Item->InsertItem( &ListViewItemData );
	}

	// 추가적인 재료 아이템들
	pkListView_Item = m_InterfaceFactory.GetListView( NITEMREPAIR_LISTVIEW_MATERIALS );
	pkListView_Item->DeleteAllItems();
	for( SI32 i=1; i<m_ExtraMaterial.GetSize(); i++ )
	{
		pclItem = NULL;
		if( m_ExtraMaterial[i].siStgPos > 0  && m_ExtraMaterialItem[ i ].siUnique > 0 )		pclItem = &m_ExtraMaterialItem[i];

		if( pkListView_Item && pclItem )
		{
			NTCHARString64 kItemName( pclClient->pclItemManager->GetName(pclItem->siUnique) );
			pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont );
			pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - 1, pclItem->siItemNum, kItemName, ToolTipMsg, false );
			pkListView_Item->InsertItem( &ListViewItemData );
		}
	}
 
	// 인벤토리 업데이트
	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	if( personinventory )		personinventory->LoadItemData();
	if( horseinventory )		horseinventory->LoadItemData();
	if( bag0inventory )			bag0inventory->LoadItemData();
	if( bag1inventory )			bag1inventory->LoadItemData();
}

void NItemRepairDlg::SetRepairInfo( SI32 siItemCharIUnique, SI32 siItemCharPersonID, SI16 siRepairItemPos, cltItem* pclRepairItem, RepairItemExtraMaterial& kMaterial, GMONEY Money )
{
	m_siItemCharUnique	= siItemCharIUnique;
	m_siItemPersonID	= siItemCharPersonID;

	m_siRepairItemPos	= siRepairItemPos;
	m_clRepairItem.Set( pclRepairItem );

	for( SI32 i=0; i<m_ExtraMaterialItem.GetSize(); i++ )
	{
		m_ExtraMaterial[i].Set( &kMaterial[i] );
		m_ExtraMaterialItem[i].Init();
		m_ExtraMaterialItem[i].siUnique = kMaterial[i].siItemUnique;
		m_ExtraMaterialItem[i].siItemNum = kMaterial[i].siUseItemNum;
	}

	m_Money = Money;
	CEdit* pclEdit_Money = m_InterfaceFactory.GetEdit( NITEMREPAIR_EDITBOX_PRICE );
	if( pclEdit_Money )
	{
		pclEdit_Money->SetText( SI64ToString(m_Money) );
		pclEdit_Money->Enable(true);
		pclEdit_Money->Refresh();
	}

	UpdateInterface();
}
SI16 NItemRepairDlg::GetMode()
{
	return m_siMode	;
}