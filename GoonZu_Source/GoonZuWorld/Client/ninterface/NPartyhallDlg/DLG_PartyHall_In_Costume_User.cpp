#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Costume_User.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/NStructureDlg/Feast/NFeastDlg.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"
#include "../../Server/GachaManager/GachaManager.h"

NPartyHall_In_Costume_User_Dlg::NPartyHall_In_Costume_User_Dlg()
{
	m_SelectItemUnique = 0;
}

NPartyHall_In_Costume_User_Dlg::~NPartyHall_In_Costume_User_Dlg()
{

}

void NPartyHall_In_Costume_User_Dlg::Create(  CControllerMgr *pParent  )
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NStructureDlg/NPartyHall/DLG_PartyHall_In_Costume_User.ddf" ) );
	file.CreateChildDialog( this, PARTYHALL_IN_COSTUME_USER_DLG, TEXT( "dialog_in_costume_user" ), StaticNPartyHall_In_Costume_User_DlgProc, pParent  );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	PARTYHALL_COSTUME_LISTVIEW_INVENTORY,		this ), PARTYHALL_COSTUME_LISTVIEW_INVENTORY,		TEXT( "listview_inventory_costume" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		PARTYHALL_COSTUME_BUTTON_GIVE_ME,			this ), PARTYHALL_COSTUME_BUTTON_GIVE_ME,			TEXT( "button_give_me_costume" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		PARTYHALL_COSTUME_BUTTON_EXIT,				this ), PARTYHALL_COSTUME_BUTTON_EXIT,				TEXT( "button_exit" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		PARTYHALL_COSTUME_FIRECRACKER_STATIC_USE,	this ), PARTYHALL_COSTUME_FIRECRACKER_STATIC_USE,	TEXT( "statictext_firecracker" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		PARTYHALL_COSTUME_FIRECRACKER_LSIT,			this ), PARTYHALL_COSTUME_FIRECRACKER_LSIT,			TEXT( "listbox_firecrack_list" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		PARTYHALL_COSTUME_FIRECRACKER_BUTTON_USE,	this ), PARTYHALL_COSTUME_FIRECRACKER_BUTTON_USE,	TEXT( "button_use" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	PARTYHALL_COSTUME_OUTLINE1,					this ), PARTYHALL_COSTUME_OUTLINE1,					TEXT( "outline_NONAME1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	PARTYHALL_COSTUME_OUTLINE2,					this ), PARTYHALL_COSTUME_OUTLINE2,					TEXT( "outline_NONAME2" ) );	
	

	SetCostumeItem();

	
	ShowEffectInfo();
}

void CALLBACK NPartyHall_In_Costume_User_Dlg::StaticNPartyHall_In_Costume_User_DlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	NPartyHall_In_Costume_User_Dlg *pThis = (NPartyHall_In_Costume_User_Dlg*) pControllerMgr;
	pThis->NPartyHall_In_Costume_User_DlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NPartyHall_In_Costume_User_Dlg::NPartyHall_In_Costume_User_DlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient*	pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case PARTYHALL_COSTUME_FIRECRACKER_BUTTON_USE:
		{
			// 폭죽이 있는지 확인.
			SI32 Pos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos(ITEMUNIQUE(24060));
			if(0 > Pos)
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8293));
				return;
			}
			
			CList* pList = m_InterfaceFactory.GetList(PARTYHALL_COSTUME_FIRECRACKER_LSIT);
			m_Index		 = pList->GetSelectedIndex();
			if(0 > m_Index)
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8344));		
				return;
			}
			// 연회자 인지 체크
			SI32 personid   = pclClient->pclCM->CR[1]->pclCI->GetPersonID();

			TCHAR* pTitle = GetTxtFromMgr(5227);
			TCHAR* pText  = GetTxtFromMgr(5230);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
			pclclient->SetMsgBox( &MsgBox, (BYTE*)&m_Index, sizeof(m_Index) );
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case PARTYHALL_COSTUME_BUTTON_EXIT:
		{
			cltMsg clMsg( GAMEMSG_REQUEST_PARTYHALL_EXIT, NULL, NULL );
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);	
		}
		break;
	case PARTYHALL_COSTUME_BUTTON_GIVE_ME:
		{
			if( 0 >= m_SelectItemUnique )
				return;

			// 파티홀 의복은 소유개수에 제한이 있다.
			SI32 siHaveCount = 0;
			siHaveCount += pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(m_SelectItemUnique);
			siHaveCount += pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNumInEquip(m_SelectItemUnique);

			// 현재 1개까지만 소유할 수 있다.
			if( siHaveCount >= 1 )
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6860));
				return;
			}
			
			cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_GET_COSTUME, sizeof(SI32), (BYTE*)&m_SelectItemUnique);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);			
		}
		break;
	case PARTYHALL_COSTUME_LISTVIEW_INVENTORY:
		{
			CListView*  m_pListView = m_InterfaceFactory.GetListView( PARTYHALL_COSTUME_LISTVIEW_INVENTORY ); 

			SI32 SelectedIndex = m_pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				break;
			
			stListViewItemData* pListViewItemData = m_pListView->GetListViewItemData( SelectedIndex );

			m_SelectItemUnique = pclClient->pclItemManager->FindUniqueFromName( pListViewItemData->ItemName );
		}
		break;
	}
}

bool NPartyHall_In_Costume_User_Dlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	void*	pvoidControl  = m_InterfaceFactory.GetControl( pclClickAndDrop->m_siDstControlID );

	switch ( pclClickAndDrop->m_siDstControlID )
	{
		default:	break;
	}

	return true;
}

void NPartyHall_In_Costume_User_Dlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
		case 0:
			{
				if(RetValue)
				{
					cltGameMsgRequest_FeastUseEffect sendmsg( &pclClient->pclFeastManager->siChiefEffectSelectIndex );

					cltMsg clMsg(GAMEMSG_REQUEST_FEAST_USEEFFECT, sizeof(sendmsg), (BYTE*)&sendmsg );
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
			break;
		default:	
			{

			}
			break;
	}
}

void NPartyHall_In_Costume_User_Dlg::SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData )
{
	switch( _ClassType )
	{
		//case XXX:	delete XXX_ClassData; break;	
		default: break;
	}
}


void NPartyHall_In_Costume_User_Dlg::SetCostumeItem( void )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	CListView* pListView = m_InterfaceFactory.GetListView( PARTYHALL_COSTUME_LISTVIEW_INVENTORY );

	

	SI32 itemtype[2]		= { ITEMTYPE_DRESS, ITEMTYPE_HAT };
	SI32 GlobalImageIndex	= -1;
	SI32 GlobalImageFont	= -1;
	SI32 StartPos			=  0;
	SI32 id					=  1;
	
	stListViewItemData itemdata;

	pListView->DeleteAllItems();
	pListView->SetStartItemPos( StartPos );

	SI32 siItemCount = 0;
	// 콤보 박스에 아이템의 종류를 설정한다. 
	cltItemInfo* pinfo = NULL;
	SI32 ref;
	for( SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++ )
	{	
		pinfo = pclitemmanager->pclItemInfo[i];
		
		if(pinfo == NULL)
		{		
			continue;
		}
		
		
		ref = pclClient->pclItemManager->FindItemRefFromUnique(pinfo->clItem.siUnique);

		// 관복과 의복은 제외시킨다.
		if( 0 < pinfo->siNeedRank || true == pclClient->pclGachaManager->IsGachaRareItem( pinfo->clItem.siUnique ))
			continue;

		if(	pinfo->siType != itemtype[0] &&
			pinfo->siType != itemtype[1] 		)
		{		
			continue;
		}
		
		if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pinfo->clItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

				cltItem TempItem;
				TempItem.Init();
				TempItem.siUnique  = pinfo->clItem.siUnique;
				TempItem.siItemNum = 0;

				pclClient->pclItemManager->GetExplain( &TempItem, ToolTipMsg );

				TCHAR* Name = (TCHAR*)pinfo->GetName();

				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, siItemCount - StartPos , 0, Name, ToolTipMsg );
				pListView->InsertItem( &ListViewItemData );

				++siItemCount;
			}
		}
	}
}

void NPartyHall_In_Costume_User_Dlg::ShowEffectInfo()
{
	CList* pList = m_InterfaceFactory.GetList(PARTYHALL_COSTUME_FIRECRACKER_LSIT);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[3];
	pText[0] = GetTxtFromMgr(1641);
	pText[1] = GetTxtFromMgr(1782);
	pText[2] = GetTxtFromMgr(5223);

	pList->SetColumn( 0,  50,   pText[ 0 ] );
	pList->SetColumn( 1,  80,   pText[ 1 ] );      
	pList->SetColumn( 2,  144,  pText[ 2 ] );       

	pList->Refresh();

	SI32 FeastEffectName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = 
	{	
			// 하늘에서 떨어지는것
			5711,
			5712,
			5713,

			// 하늘에서 떨어지는것
			5714,
			5715,
			5716,
			5717,
			5718,

			// 2개가 하나 되는것
			5719,
			5720,
			5721,
	};

	SI32 FeastEffectKindName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = {

			// 하늘에서 떨어지는것
			5708,
			5708,
			5708,

			// 하늘에서 떨어지는것
			5709,
			5710,
			5709,
			5709,
			5709,

			// 2개가 하나 되는것
			5710,
			5710,
			5710,
	};

	TCHAR buffer[10];

	// List의 Item 모두 지우기
	pList->Clear();

	stListBoxItemData tmpListBoxItemData;
	for( SI16 i = 0; i < cltFeastManager::MAX_FEAST_EFFECT_NUM; i++)
	{
		tmpListBoxItemData.Init();

		StringCchPrintf(buffer, 10, TEXT("%d"), i + 1);

		tmpListBoxItemData.Set( 0, buffer );
		tmpListBoxItemData.siParam[0] = i;

		tmpListBoxItemData.Set( 1, GetTxtFromMgr(FeastEffectKindName[ i ]) );
		tmpListBoxItemData.Set( 2, GetTxtFromMgr(FeastEffectName[ i ]) );

		pList->AddItem( &tmpListBoxItemData );
	}

	pList->Refresh();
}