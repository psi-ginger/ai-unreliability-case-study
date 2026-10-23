#include "StrSubRepairDlg.h"
#include "StrSubInventory.h"


#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../InterfaceMgr/Interface/ListView.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStrSubRepairDlg::NStrSubRepairDlg()
{
	m_pMyInventory = new NStrSubInventory;
}

NStrSubRepairDlg::~NStrSubRepairDlg()
{
	NDelete( m_pMyInventory );
}

void NStrSubRepairDlg::InitDlg()
{
	SetControlMap( EDIT_STRDURA, TEXT("editbox_strdura") );
	SetControlMap( EDIT_NEEDITEM, TEXT("editbox_needitem") );
	SetControlMap( EDIT_NEEDITEM_NUM, TEXT("editbox_needitem_num") );
	SetControlMap( EDIT_USEITEM_NUM, TEXT("editbox_useitem_num") );
	SetControlMap( EDIT_ITEMNAME, TEXT("editbox_itemname") );
	
	SetControlMap( BUTTON_USEITEM, TEXT("button_useitem") );
	
	SetControlMap( LISTVIEW_INVENTORY, TEXT("listview_inventory") );

	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, (CListView*)m_pControlMap[ LISTVIEW_INVENTORY ] );

	m_pMyInventory->m_pListView->SetBKColorUse( true );
	m_pMyInventory->m_pListView->SetBorder( true );

	m_pMyInventory->LoadMyInventory();
}

void NStrSubRepairDlg::Action()
{

	TCHAR buf[ 256 ];
	TCHAR *pszNeedItem;

	pszNeedItem = (TCHAR *)pclClient->pclItemManager->GetName( m_pStrManager->clClientStrInfo.clCommonInfo.siNeedItemUnique );

	if( pszNeedItem ) {
		SetEditText( EDIT_NEEDITEM, pszNeedItem );
	}

	_itot( m_pStrManager->clClientStrInfo.clCommonInfo.siStrDura, buf, 10 );
	SetEditText( EDIT_STRDURA, buf );

	_itot( m_pStrManager->clClientStrInfo.clCommonInfo.siNeedItemNum, buf, 10 );
	SetEditText( EDIT_NEEDITEM_NUM, buf );

	_itot( m_pStrManager->clClientStrInfo.clCommonInfo.siUseItemNum, buf, 10 );
	SetEditText( EDIT_USEITEM_NUM, buf );


	if( m_pMyInventory->IsMyInventoryChanged() ) {
		m_pMyInventory->LoadMyInventory();
	}
}

void NStrSubRepairDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CASE_CONTROL( LISTVIEW_INVENTORY )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				TCHAR buffer[ 128 ];

				m_pMyInventory->m_siSelectedIndex = 
					m_pMyInventory->m_pListView->GetSelectedItemIndex();

				if( m_pMyInventory->m_siSelectedIndex >= 0 ) {

					stListViewItemData *pItemData = 
						m_pMyInventory->m_pListView->GetListViewItemData(
						m_pMyInventory->m_siSelectedIndex - PERSONITEM_INV0 );

					if( pItemData ) {	

						StringCchPrintf( buffer, 128, TEXT("%s"),pItemData->ItemName );
						SetEditText( EDIT_ITEMNAME, buffer );
					}
				}
			}
			break;
		}
	}


	else CASE_CONTROL( BUTTON_USEITEM ) 
	{

		//// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		//{
		//	SI32 id = 1;
		//	cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
		//	if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
		//	{
		//		TCHAR* pText;
		//		if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
		//		else pText = GetTxtFromMgr(1440);
		//		SetMainExplain(pText);
		//		return;
		//	}
		//}

		//int index = m_pMyInventory->FindIndexFromUnique( 
		//	m_pStrManager->clClientStrInfo.clCommonInfo.siNeedItemUnique );

		//if( index >= 0 ) {

		//	cltGameMsgRequest_RepairStructure sendMsg( 
		//		m_pStrManager->siRankType,
		//		index, 
		//		m_pMyInventory->m_pclItem[ index ].siUnique, 1 );

		//	cltMsg clMsg( GAMEMSG_REQUEST_REPAIRSTRUCTURE, sizeof( sendMsg ), (BYTE*)&sendMsg );
		//	pclClient->SendMsgToServer( (TCHAR*)&clMsg );
		//}

	}

}
