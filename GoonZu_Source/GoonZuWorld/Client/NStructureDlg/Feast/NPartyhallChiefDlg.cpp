#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"
#include "../common/Item/ItemCommon/Item-EventStructure.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPartyHallChiefDlg::NPartyHallChiefDlg()
{
	ZeroMemory(m_DecorationKind, sizeof(m_DecorationKind));
	ZeroMemory(m_MaterialItem,	 sizeof(m_MaterialItem));
}

NPartyHallChiefDlg::~NPartyHallChiefDlg()
{
	
}

void NPartyHallChiefDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	pFeastManager->siChiefPresentSelectIndex = -1;

	SetControlMap( BUTTON_ADD_DECORATIN,	TEXT("button_addpresent")	);
	SetControlMap( LIST_MATERIAL,			TEXT("listbox_material")	);
	SetControlMap( LIST_DECORATION,			TEXT("listbox_present")		);
	SetControlMap( COMBO_ITEMTYPE,			TEXT("combobox_itemtype")	);
	
	// List 
	CList* pList = (CList*)m_pControlMap[ LIST_MATERIAL ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[3];
	pText[0] = GetTxtFromMgr(2311);
	pText[1] = GetTxtFromMgr(2312);
	pText[2] = GetTxtFromMgr(8341);	

	pList->SetColumn( 0, 173,  pText[ 0 ] );
	pList->SetColumn( 1,  75,  pText[ 1 ] );
	pList->SetColumn( 2,  87,  pText[ 2 ] );
      
 
	pList->Refresh();

	// List 
	pList = (CList*)m_pControlMap[ LIST_DECORATION ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText[0] = GetTxtFromMgr(8342);
	pText[1] = GetTxtFromMgr(1832);
	
	pList->SetColumn( 0, 268,  pText[ 0 ] );
	pList->SetColumn( 1, 68,   pText[ 1 ] );


	pList->Refresh();


	CComboBox *pComboDecorationItem = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	
	// 파티홀 장식.<장식물 추가시 여기에 kind 넣어주기.>
	SI32 StartDecorationKind = pclClient->GetUniqueFromHash( TEXT("KIND_PCAKE"));
	for(SI32 Count = 0; Count < MAX_PARTYHALL_DECORATION_ITEM; ++Count )
	{
		m_DecorationKind[Count] = StartDecorationKind + Count;
	}
	/*m_DecorationKind[0] = pclClient->GetUniqueFromHash( TEXT("KIND_PCAKE"));
	m_DecorationKind[1] = pclClient->GetUniqueFromHash( TEXT("KIND_PCHAMPAGNE"));
	m_DecorationKind[2] = pclClient->GetUniqueFromHash( TEXT("KIND_PBARBECUE"));
	m_DecorationKind[3] = pclClient->GetUniqueFromHash( TEXT("KIND_PGARLAND"));
	m_DecorationKind[4] = pclClient->GetUniqueFromHash( TEXT("KIND_PICESTATUE"));*/




	ShowItemListInCombo(pComboDecorationItem, 1, false);
	ShowDecorationList();

}

void NPartyHallChiefDlg::Action()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	if( pFeastManager->bUpdateChief2 ) 
	{
		pFeastManager->bUpdateChief2 = FALSE;

		
	}
}


void NPartyHallChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_DECORATION ];
	stListBoxItemData *pItemData;

	

	CASE_CONTROL( COMBO_ITEMTYPE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				// List 
				CComboBox			*pComboPresent = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
				stComboBoxItemData	*pItemData;

				int index = pComboPresent->GetSelectedIndex();

				if( index < 0 ) 
					return;

				pItemData = pComboPresent->GetItemData( index );


				SetItemMaterialInfo( pItemData->strText );
			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_ADD_DECORATIN )
	{		
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		if( false == MakeDecoration() )
		{
			return;
		}

		CComboBox*	pComboItemType = (CComboBox *)m_pControlMap[ COMBO_ITEMTYPE ];
		int index = pComboItemType->GetSelectedIndex();
		if( index < 0 ) 
			return;

		cltGameMsgRequest_PartyHall_Add_Decoration sendmsg( m_pStrManager->siVillageUnique, m_DecorationKind[index], 1, m_MaterialItem );
		cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_ADD_DECORATION, sizeof(sendmsg), (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	else CASE_CONTROL( LIST_DECORATION )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				pFeastManager->siChiefPresentSelectIndex = -1;

				int index = pList->GetSelectedIndex();
				if( index < 0 ) return;

				pItemData = pList->GetItemData( index );

				pFeastManager->siChiefPresentSelectIndex = pItemData->siParam[0];

			}
			break;
		}
	}

}

bool NPartyHallChiefDlg::MakeDecoration( void )
{
	CList* pList = (CList*)m_pControlMap[ LIST_MATERIAL ];
	
	stListBoxItemData* ComboBoxItemData;
	
	for( SI32 Count = 0; Count < pList->GetListNum(); ++Count )
	{
		ComboBoxItemData = pList->GetItemData( Count );

		if( atoi(ComboBoxItemData->strText[1]) > atoi(ComboBoxItemData->strText[2]) )
		{
			SetMainExplain(GetTxtFromMgr(8366));
			return false;
		}
	}
		
	return true;
}


// 파티 창고에 있는 아이템의 개수 얻기.
SI32 NPartyHallChiefDlg::PartyhallItemCount(SI32 _ItemUnique)
{	
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	for ( SI32 i = 0; i < MAX_FEAST_STG_NUMBER; ++i )
	{
		pFeastManager->m_pclStgItem[i].Set( &pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastStg[ i ] );

		if ( pFeastManager->m_pclStgItem[ i ].siItemUnique > 0 && pFeastManager->m_pclStgItem[ i ].siItemNum > 0 )
		{
			if( pFeastManager->m_pclStgItem[ i ].siItemUnique == _ItemUnique )
			{
				return pFeastManager->m_pclStgItem[ i ].siItemNum;
			}
		}
	}

	return 0;
}

void NPartyHallChiefDlg::SetChangeItemMaterialInfo(cltFeastStg* pStgItem)
{
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);
	
	memcpy(pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastStg, pStgItem, sizeof(pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastStg[0]) * MAX_FEAST_STG_NUMBER);

	CComboBox			*pComboPresent = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	stComboBoxItemData	*pItemData;

	int index = pComboPresent->GetSelectedIndex();

	if( index < 0 ) 
		return;

	pItemData = pComboPresent->GetItemData( index );


	SetItemMaterialInfo( pItemData->strText );
}

//  재료 아이템 리스트.
void NPartyHallChiefDlg::SetItemMaterialInfo(TCHAR* pItemName)
{
	// 어떤 아이템이 선택 되었는가
	CComboBox*	pComboPresent = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	int index = pComboPresent->GetSelectedIndex();
	if( index < 0 ) 
		return;
	
	// 장식을 만드는데 필요한 아이템 정보를 얻는다.
	cltEventStructureUnit* pDecoration = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE__PCAKE + index);
	if( NULL == pDecoration )
		return;
 

	CList*	pList = (CList*)m_pControlMap[ LIST_MATERIAL ];
	pList->Clear();

	ZeroMemory(m_MaterialItem,	 sizeof(m_MaterialItem));
	
	stListBoxItemData   itemdata;
	TCHAR*				npcName    = NULL;
	SI32				ItemUnique = 0;
	SI32				ItemNum	   = 0;
	TCHAR				buffer[6]  = "";

	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);
	
	for(SI32 Count = 0; Count < MAX_MATERIAL_NUMBER; Count++ )
	{
		ItemUnique = pDecoration->clMaterial[Count].siUnique;
		ItemNum	   = pDecoration->clMaterial[Count].siNeedNum;

		if( 0 >= ItemUnique)
			break;

		for( SI32 i = 0; i < MAX_FEAST_STG_NUMBER; ++i )
		{
			if( ItemUnique == pFeastManager->m_pclStgItem[ i ].siItemUnique )
			{
				m_MaterialItem[ i ].siItemUnique = ItemUnique;
				m_MaterialItem[ i ].siItemNum	 = ItemNum;
			}
		}

		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( ItemUnique );
		itemdata.Set( 0, ItemName );

		StringCchPrintf( buffer, sizeof(buffer), TEXT("%d"), ItemNum );
		itemdata.Set( 1, buffer );

		StringCchPrintf( buffer, sizeof(buffer), TEXT("%d"), PartyhallItemCount(ItemUnique) );
		itemdata.Set( 2, buffer );

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}


// 제조 가능한 장식물 넣기.
BOOL NPartyHallChiefDlg::ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	// 콤보 박스를 지운다.
	pCombo->Clear();

	stComboBoxItemData  itemdata;
	TCHAR*				npcName = NULL;

	for(SI32 Count = 0; Count < MAX_PARTYHALL_DECORATION_ITEM; Count++ )
	{
		if( 0 >= m_DecorationKind[Count])
			return TRUE;

		npcName = (TCHAR*)pclClient->pclKindInfoSet->GetName(m_DecorationKind[Count]);
		if( NULL == npcName )
			break;

		itemdata.Init();
		itemdata.Set( npcName );
		pCombo->AddItem( &itemdata );
	}


	return TRUE;
}

// 보유 장식물
void NPartyHallChiefDlg::ShowDecorationList( void )
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_DECORATION ];
	stListBoxItemData itemdata;

	TCHAR buffer[256];
	SI32 index = 0;

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE__PCAKE);

	PartyHall_Decoration* pDecorationList = &pFeastManager->clDecoration[0];

	TCHAR * npcName = NULL;

	for( int i = 0; i < MAX_PARTYHALL_DECORATION_ITEM; ++i ) 
	{
		if( 0 < pDecorationList[i].m_DacorationKind ) 
		{
			itemdata.Init();
			itemdata.siParam[0] = i;

			npcName = (TCHAR*)pclClient->pclKindInfoSet->GetName(pDecorationList[i].m_DacorationKind);
			if( NULL == npcName )
				break;

			itemdata.Set( 0,  npcName);

			_itot( pDecorationList[i].m_DacorationNum, buffer, 10 );
			itemdata.Set( 1, buffer );

			pList->AddItem( &itemdata );
			index++;
		}
	}

	pList->Refresh();
}


