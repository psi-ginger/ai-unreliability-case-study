#include "NNewNPCRepair.h"

#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../ninterface/NNewInventory/NNewInventory.h"

#include "../../../CommonLogic/MsgType-Item.h"

NNewNPCRepairDlg::NNewNPCRepairDlg()
{
	Init();
}

NNewNPCRepairDlg::~NNewNPCRepairDlg()
{
}

void NNewNPCRepairDlg::Init()
{
	m_siRepairItemPos = 0;

	for( SI32 i=0; i<m_ExtraMaterial.GetSize(); i++ )
	{
		m_ExtraMaterial[i].Init();
	}
}

void NNewNPCRepairDlg::Create( )
{
	if( IsCreate() )					return;

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NPCRepair/DLG_NewNPCRepair.ddf");

	file.CreatePopupDialog( this, NNEWNPCREPAIR_DLG,	"dialog_NPCRepair", StaticNNewNPCRepairDlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_PORTRAIT_IMAGE,	this), NNEWNPCREPAIR_STATIC_PORTRAIT_IMAGE, 	"statictext_portrait" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NNEWNPCREPAIR_EDIT_NPCTEXT,				this), NNEWNPCREPAIR_EDIT_NPCTEXT,				"editbox_npctext" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_NPCREPAIR3,		this), NNEWNPCREPAIR_STATIC_NPCREPAIR3,			"statictext_NPCRepair3" );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_IMAGESTATIC_PLUS,			this), NNEWNPCREPAIR_IMAGESTATIC_PLUS,			"imagestatic_plus" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_REPAIRITEM,		this),	NNEWNPCREPAIR_LISTVIEW_REPAIRITEM,		"listview_repairitem" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE,		this),	NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE,		"listview_repairstone" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1,	this),	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1,	"listview_extramaterial1" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2,	this),	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2,	"listview_extramaterial2" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3,	this),	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3,	"listview_extramaterial3" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4,	this),	NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4,	"listview_extramaterial4" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_REPAIRITEM,		this), NNEWNPCREPAIR_STATIC_REPAIRITEM, 		"statictext_repairitem" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_REPAIRSTONE,		this), NNEWNPCREPAIR_STATIC_REPAIRSTONE, 		"statictext_repairstone" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_MATERIAL,			this), NNEWNPCREPAIR_STATIC_MATERIAL, 			"statictext_material" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NNEWNPCREPAIR_STATIC_REPAIRPRICE,		this),	NNEWNPCREPAIR_STATIC_REPAIRPRICE,		"statictext_repairprice" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NNEWNPCREPAIR_EDITBOX_REPAIRPRICE,		this),	NNEWNPCREPAIR_EDITBOX_REPAIRPRICE,		"editbox_repairfee" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NNEWNPCREPAIR_BUTTON_NPCREPAIR,			this), NNEWNPCREPAIR_BUTTON_NPCREPAIR,			"button_npcrepair" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NNEWNPCREPAIR_BUTTON_CLOSE,				this), NNEWNPCREPAIR_BUTTON_CLOSE,				"button_close" );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NNEWNPCREPAIR_OUTLINE_NONAME1,			this), NNEWNPCREPAIR_OUTLINE_NONAME1,			"outline_NONAME1" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NNEWNPCREPAIR_OUTLINE_NONAME2,			this), NNEWNPCREPAIR_OUTLINE_NONAME2,			"outline_NONAME2" );
	
	CEdit* pclEdit_NpcText = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDIT_NPCTEXT );
	if ( pclEdit_NpcText )
	{
		pclEdit_NpcText->SetText( GetTxtFromMgr(40120) );
	}
	
	CEdit* pkEdit_RepairPrice = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDITBOX_REPAIRPRICE );
	if( pkEdit_RepairPrice )
	{
		pkEdit_RepairPrice->SetReadOnly( true );
	}

	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NNEWNPCREPAIR_STATIC_PORTRAIT_IMAGE );
	if ( pImage )
	{
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_REPAIRMAN") ), szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImage->SetFileName(szPortrait);
		pImage->SetFontIndex(siPortraitFont);
	}

	CStatic* pclStatic_RepairItem = m_InterfaceFactory.GetStatic( NNEWNPCREPAIR_STATIC_REPAIRITEM );
	if( pclStatic_RepairItem )
	{
		pclStatic_RepairItem->SetTextPrintOption( DT_TOP | DT_CENTER | DT_VCENTER );
		pclStatic_RepairItem->Refresh();
	}

	CStatic* pclStatic_RepairStone = m_InterfaceFactory.GetStatic( NNEWNPCREPAIR_STATIC_REPAIRSTONE );
	if( pclStatic_RepairStone )
	{
		pclStatic_RepairStone->SetTextPrintOption( DT_TOP | DT_CENTER | DT_VCENTER );
		pclStatic_RepairStone->Refresh();
	}

	CStatic* pclStatic_Material = m_InterfaceFactory.GetStatic( NNEWNPCREPAIR_STATIC_MATERIAL );
	if( pclStatic_Material )
	{
		pclStatic_Material->SetTextPrintOption( DT_TOP | DT_CENTER | DT_VCENTER );
		pclStatic_Material->Refresh();
	}

	Show( true );
}

void CALLBACK NNewNPCRepairDlg::StaticNNewNPCRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NNewNPCRepairDlg *pThis = (NNewNPCRepairDlg*) pControllerMgr;
	pThis->NNewNPCRepairDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NNewNPCRepairDlg::NNewNPCRepairDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NNEWNPCREPAIR_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Init();

					Show( false );
					DeleteDialog();

					CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
					CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
					CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
					CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

					if( personinventory )		personinventory->LoadItemData();
					if( horseinventory )		horseinventory->LoadItemData();
					if( bag0inventory )			bag0inventory->LoadItemData();
					if( bag1inventory )			bag1inventory->LoadItemData();
				}
				break;
			}
		}
		break;

	case NNEWNPCREPAIR_BUTTON_NPCREPAIR:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
					if( pclchar == NULL )									return;

					cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[m_siRepairItemPos];
					if( pclitem == NULL || pclitem->siUnique <= 0 )					return;

					CEdit* pclNPCText = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDIT_NPCTEXT );
					if ( NULL == pclNPCText )
					{
						return;
					}

					SI16 siNeedRepairStone = pclClient->pclItemManager->NeedRepairStoneNum( pclitem );
					if( m_ExtraMaterial[0].siUseItemNum < siNeedRepairStone )
					{
						// 수리석이 모자랍니다.
						NTCHARString128	kText( GetTxtFromMgr(40142) );
						kText.Replace( "#itemname#", pclClient->pclItemManager->GetName( pclitem->siUnique ) );
						kText.Replace( "#stonenum#", SI16ToString(siNeedRepairStone) );

						pclNPCText->SetText( kText );

						return;
					}

					GMONEY RepairMoney = pclClient->pclItemManager->RepairPrice( pclitem );
					if( pclchar->pclCI->clIP.GetMoney() < RepairMoney )
					{
						// 수리비용이 부족합니다.
						NTCHARString128	kText( GetTxtFromMgr(40143) );
						NTCHARString64 kUnit( GetTxtFromMgr(611) );

						TCHAR			szMoneyString[32] = { '\0', };
						g_JYLibFunction.SetNumUnit( RepairMoney, szMoneyString, sizeof(szMoneyString) );

						kText.Replace( "#itemname#",	pclClient->pclItemManager->GetName( pclitem->siUnique ) );
						kText.Replace( "#money#",		szMoneyString );
						kText.Replace( "#unit#",		kUnit );
											
						pclNPCText->SetText( kText );

						return;
					}


					bool bHasBlessingStone = false;
					for( SI32 i=1; i<m_ExtraMaterial.GetSize(); i++ )
					{
						SI32 siBlessingStoneNum = pclClient->pclItemManager->NeedBlessingStoneNum( pclitem, ITEMUNIQUE(m_ExtraMaterial[i].siItemUnique) );
						if( siBlessingStoneNum > 0 && siBlessingStoneNum <= m_ExtraMaterial[i].siUseItemNum )
						{
							bHasBlessingStone = true;
						}
					}

					stMsgBox kMsgBox;

					NTCHARString128 kMsg( "" );
					NTCHARString128 kTitle( GetTxtFromMgr(40119) );
					SI16 siMsgIndex = 1;


					if( bHasBlessingStone )
					{
						kMsg = GetTxtFromMgr(40140);
						kMsg.Replace( "#itemname#" , pclClient->pclItemManager->GetName(pclitem->siUnique ) );
						siMsgIndex = 2;
					}
					else
					{
						kMsg = GetTxtFromMgr(40139);
						kMsg.Replace( "#itemname#" , pclClient->pclItemManager->GetName(pclitem->siUnique ) );
						kMsg.Replace( "#decmaxdur#", SI64ToString(pclClient->GetGlobalValue("GV_Decrease_MaxDurability")) );
						siMsgIndex = 1;
					}

					kMsgBox.Set( this, kTitle, kMsg, MSGBOX_TYPE_YESNO, siMsgIndex );
					pclclient->SetMsgBox( &kMsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;
	case NNEWNPCREPAIR_LISTVIEW_REPAIRITEM:
	case NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE:
	case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1:
	case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2:
	case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3:
	case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					CListView* pkListView_Item =  m_InterfaceFactory.GetListView( nControlID );
					if( pkListView_Item )
					{
						pkListView_Item->DeleteAllItems();
					}

					switch( nControlID )
					{
					case NNEWNPCREPAIR_LISTVIEW_REPAIRITEM:			m_siRepairItemPos = 0;		break;
					case NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE:		m_ExtraMaterial[0].Init();	break;
					case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1:		m_ExtraMaterial[1].Init();	break;
					case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2:		m_ExtraMaterial[2].Init();	break;
					case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3:		m_ExtraMaterial[3].Init();	break;
					case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4:		m_ExtraMaterial[4].Init();	break;
					}

					CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
					CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
					CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
					CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

					if( personinventory )		personinventory->LoadItemData();
					if( horseinventory )		horseinventory->LoadItemData();
					if( bag0inventory )			bag0inventory->LoadItemData();
					if( bag1inventory )			bag1inventory->LoadItemData();
				}
				break;
			}
		}
		break;
	}
}

void NNewNPCRepairDlg::Show( bool bShow )
{
	CDialogController::Show( bShow );

	if( CDialogController::IsShow() )
	{
	}
}

void NNewNPCRepairDlg::Action()
{
	if( CDialogController::IsShow() == false )		return;
}


void NNewNPCRepairDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 1:
	case 2:
		{
			if( RetValue == false )				return;

			SendRepairMsg();
		}
		break;
	}
}

bool NNewNPCRepairDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	stListViewItemData ListViewItemData;
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;
	TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

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

			switch( pclClickAndDrop_Item->m_siDstControlID )
			{
			case NNEWNPCREPAIR_LISTVIEW_REPAIRITEM:
				{
					// 수리할 수 없는 아이템은 등록할 수 없다.
					if( pclClient->pclItemManager->CanRepairItem( pclItem ) == false )				return true;

					m_siRepairItemPos = pclClickAndDrop_Item->m_siPara1;

					SetTheDropItemMessage();
				}
				break;
			case NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE:
				{
					// 수리석이 아닌 아이템은 등록할 수 없다.
					if( pclClient->pclItemManager->IsRepairStone( pclItem ) == false )				return true;

					m_ExtraMaterial[0].siStgPos		= pclClickAndDrop_Item->m_siPara1;
					m_ExtraMaterial[0].siItemUnique	= pclItem->siUnique;
					m_ExtraMaterial[0].siUseItemNum	= pclItem->siItemNum;
				}
				break;

			case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1:
			case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2:
			case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3:
			case NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4:
				{
					// 추가 재료인지 체크한다.
					if( pclClient->pclItemManager->IsRepairExtraMaterial( pclItem ) == false )		return true;

					SI32 siMaterialPos = pclClickAndDrop_Item->m_siDstControlID - NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1 + 1;

					m_ExtraMaterial[siMaterialPos].siStgPos		= pclClickAndDrop_Item->m_siPara1;
					m_ExtraMaterial[siMaterialPos].siItemUnique	= pclItem->siUnique;
					m_ExtraMaterial[siMaterialPos].siUseItemNum	= pclItem->siItemNum;
				}
				break;
			default:
				{
					return false;
				}
				break;
			}			

			if( pkListView_Item && pclItem )
			{
				NTCHARString64 kItemName( pclClient->pclItemManager->GetName(pclItem->siUnique) );
				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont );
				pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );

				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, pclItem->siItemNum, kItemName, ToolTipMsg, false );
				pkListView_Item->InsertItem( &ListViewItemData );
			}

			if( pclItem && 
				pclClickAndDrop_Item->m_siDstControlID == NNEWNPCREPAIR_LISTVIEW_REPAIRITEM )
			{
				CEdit* pkEdit_RepairPrice = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDITBOX_REPAIRPRICE );
				GMONEY RepairMoney = pclClient->pclItemManager->RepairPrice( pclItem );
				NTCHARString16 kUnit( GetTxtFromMgr(611));
				TCHAR MoneyString[32] = { '\0', };
				if( pkEdit_RepairPrice )
				{
					g_JYLibFunction.SetNumUnit( RepairMoney, MoneyString, sizeof(MoneyString), kUnit );
					pkEdit_RepairPrice->SetText( MoneyString );
				}
			}

			CNNewInventory* pInventory = (CNNewInventory*)pclclient->m_pDialog[pclClickAndDrop_Item->m_siSrcDialogID];
			if( pInventory )
			{
				pInventory->LoadItemData();
			}

			return true;
		}
		break;
	}

	return false;
}


bool NNewNPCRepairDlg::SendRepairMsg()
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if( pclchar )
	{
		cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[m_siRepairItemPos];

		CEdit* pclNPCText = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDIT_NPCTEXT );
		if ( NULL == pclNPCText )
		{
			return false;
		}

		SI16 siNeedRepairStone = pclClient->pclItemManager->NeedRepairStoneNum( pclitem );
		if( m_ExtraMaterial[0].siUseItemNum < siNeedRepairStone )
		{
			// 수리석이 모자랍니다.
			NTCHARString128	kText( GetTxtFromMgr(40142) );
			kText.Replace( "#itemname#", pclClient->pclItemManager->GetName( pclitem->siUnique ) );
			kText.Replace( "#stonenum#", SI16ToString(siNeedRepairStone) );

			pclNPCText->SetText( kText );

			return false;
		}

		GMONEY Price = 0;
		NTCHARString32 kUnit( GetTxtFromMgr(611));
		CEdit* pclEdit_Price = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDITBOX_REPAIRPRICE );
		if( pclEdit_Price )
		{
			NTCHARString128 kText( pclEdit_Price->GetText() );
			kText.ReplaceAll( kUnit, "");
			kText.ReplaceAll( ",", "");
			kText.ReplaceAll( " ", "");
			Price = _atoi64( kText );
		}

		GMONEY RepairMoney = pclClient->pclItemManager->RepairPrice( pclitem );
		if( pclchar->pclCI->clIP.GetMoney() < RepairMoney )
		{
			// 수리비용이 부족합니다.
			NTCHARString128	kText( GetTxtFromMgr(40143) );
			NTCHARString64 kUnit( GetTxtFromMgr(611) );

			TCHAR			szMoneyString[32] = { '\0', };
			g_JYLibFunction.SetNumUnit( RepairMoney, szMoneyString, sizeof(szMoneyString) );

			kText.Replace( "#itemname#",	pclClient->pclItemManager->GetName( pclitem->siUnique ) );
			kText.Replace( "#money#",		szMoneyString );
			kText.Replace( "#unit#",		kUnit );

			pclNPCText->SetText( kText );


			return false;
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
				return false;
			}
		}

		cltGameMsgRequest_NewNPCRepair clNewNPCRepairMsg( m_siRepairItemPos, pclitem, Price, m_ExtraMaterial );
		cltMsg clMsg( GAMEMSG_REQUEST_NEWNPCREPAIR, sizeof(clNewNPCRepairMsg), (BYTE*)&clNewNPCRepairMsg );
		pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );
	}

	return true;
}

void NNewNPCRepairDlg::SetTheDropItemMessage( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if ( NULL == pclchar )
	{
		return;
	}

	if ( (PERSONITEM_INV0 > m_siRepairItemPos) || (MAX_ITEM_PER_PERSON <= m_siRepairItemPos) )
	{
		return;
	}

	cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[m_siRepairItemPos];
	if ( NULL == pclitem )
	{
		return;
	}

	CEdit* pclNPCText = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDIT_NPCTEXT );
	if ( NULL == pclNPCText )
	{
		return;
	}

	GMONEY	siRepairMoney		= pclClient->pclItemManager->RepairPrice( pclitem );
	SI32	siNeedRepairStone	= pclClient->pclItemManager->NeedRepairStoneNum( pclitem );
	SI32	siDecMaxDur			= pclClient->GetGlobalValue( "GV_Decrease_MaxDurability" );

	NTCHARString256 strDropMessage = GetTxtFromMgr( 40138 );

	TCHAR			szMoneyString[32] = { '\0', };
	g_JYLibFunction.SetNumUnit( siRepairMoney, szMoneyString, sizeof(szMoneyString) );

	strDropMessage.Replace( TEXT("#money#"),		szMoneyString );
	strDropMessage.Replace( TEXT("#stonenum#"),		SI32ToString(siNeedRepairStone) );
	strDropMessage.Replace( TEXT("#decmaxdur#"),	SI32ToString(siDecMaxDur) );

	pclNPCText->SetText( strDropMessage );
}

void NNewNPCRepairDlg::SetNPCText( TCHAR* pszText )
{
	if ( NULL == pszText )
	{
		return;
	}

	CEdit* pclNPCText = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDIT_NPCTEXT );
	if ( NULL == pclNPCText )
	{
		return;
	}

	pclNPCText->SetText( pszText );
}

void NNewNPCRepairDlg::InitDialogExceptNPCText( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	CListView* pkListView_Item =  m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_REPAIRITEM );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	pkListView_Item = m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_REPAIRSTONE );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	pkListView_Item = m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL1 );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	pkListView_Item = m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL2 );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	pkListView_Item = m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL3 );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	pkListView_Item = m_InterfaceFactory.GetListView( NNEWNPCREPAIR_LISTVIEW_EXTRAMATERIAL4 );
	if( pkListView_Item )	pkListView_Item->DeleteAllItems();

	m_siRepairItemPos = 0;
	m_ExtraMaterial[0].Init();
	m_ExtraMaterial[1].Init();
	m_ExtraMaterial[2].Init();
	m_ExtraMaterial[3].Init();
	m_ExtraMaterial[4].Init();

	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	CNNewInventory* horseinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG];
	CNNewInventory* bag0inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG];
	CNNewInventory* bag1inventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG];

	if( personinventory )		personinventory->LoadItemData();
	if( horseinventory )		horseinventory->LoadItemData();
	if( bag0inventory )			bag0inventory->LoadItemData();
	if( bag1inventory )			bag1inventory->LoadItemData();

	CEdit* pkEdit_RepairPrice = m_InterfaceFactory.GetEdit( NNEWNPCREPAIR_EDITBOX_REPAIRPRICE );
	if( pkEdit_RepairPrice )
	{
		pkEdit_RepairPrice->SetText( TEXT("0") );
	}

}