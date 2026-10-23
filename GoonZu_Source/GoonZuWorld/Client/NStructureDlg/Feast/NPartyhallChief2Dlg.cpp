#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/ListView.h"
#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPartyHallChief2Dlg::NPartyHallChief2Dlg()
{
	m_pMyInventory = new NStrInventory;
}

NPartyHallChief2Dlg::~NPartyHallChief2Dlg()
{
	NDelete( m_pMyInventory );
}

void NPartyHallChief2Dlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	SetControlMap( BUTTON_INPUT, TEXT("button_input") );
	SetControlMap( BUTTON_OUTPUT, TEXT("button_output") );

	SetControlMap( EDIT_INVEN_NAME, TEXT("editbox_inven_name") );
	SetControlMap( EDIT_INVEN_NUM, TEXT("editbox_inven_num") );

	SetControlMap( EDIT_STG_NAME, TEXT("editbox_stg_name") );
	SetControlMap( EDIT_STG_NUM, TEXT("editbox_stg_num") );

	SetControlMap( LISTVIEW_MYINVENTORY, TEXT("listview_myinventory") );
	SetControlMap( LISTVIEW_FEASTSTG, TEXT("listview_feaststg") );

	SetControlMap( EDIT_COST,		TEXT("editbox_money") );
	SetControlMap( BUTTON_SETCOST,	TEXT("button_money_set") );


	// MyInventory
	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, (CListView*)m_pControlMap[ LISTVIEW_MYINVENTORY ] );

	m_pMyInventory->m_pListView->SetBKColorUse( true );
	m_pMyInventory->m_pListView->SetBorder( true );

	m_pMyInventory->LoadMyInventory( true );

	// Stg Item
	CListView *pListView = (CListView *)m_pControlMap[ LISTVIEW_FEASTSTG ];
	
	pListView->SetBKColorUse( true );
	pListView->SetBorder( true );

	LoadFeastStg();


}

void NPartyHallChief2Dlg::Action()
{
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	if( m_pMyInventory->IsMyInventoryChanged() ) {
		m_pMyInventory->LoadMyInventory( true );
	}

	if( pFeastManager->IsStgChanged() ) {
		LoadFeastStg();
	}


}

void NPartyHallChief2Dlg::LoadFeastStg()
{
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	CListView *pListView = (CListView*)m_pControlMap[ LISTVIEW_FEASTSTG ];

	m_siSelectedStgIndex = -1;

	SI16 Counter = 0;

	TCHAR buffer[ 256 ] = TEXT("");
	TCHAR *pItemName;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	pListView->DeleteAllItems();
	pListView->SetStartItemPos( StartPos );

	for ( SI32 i = 0; i < MAX_FEAST_STG_NUMBER; ++i )
	{
		pFeastManager->m_pclStgItem[i].Set( 
			&pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastStg[ i ] );

		if ( pFeastManager->m_pclStgItem[ i ].siItemUnique > 0 && 
			pFeastManager->m_pclStgItem[ i ].siItemNum > 0 )
		{

			pItemName = (TCHAR *)pclClient->pclItemManager->GetName( pFeastManager->m_pclStgItem[ i ].siItemUnique );

			if( pItemName ) 
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( 
					pFeastManager->m_pclStgItem[i].siItemUnique, 
					&GlobalImageIndex, &GlobalImageFont ) )
				{

				//	TCHAR* pText = GetTxtFromMgr(676);
				//	sprintf(buffer, pText, ItemName, m_pclItem[i].GetItemNum() );
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

					cltItem TempItem;
					TempItem.Init();
					TempItem.siUnique = pFeastManager->m_pclStgItem[ i ].siItemUnique;
					TempItem.siItemNum = pFeastManager->m_pclStgItem[ i ].siItemNum;

					pclClient->pclItemManager->GetExplain( &TempItem, ToolTipMsg );
										
					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,pFeastManager->m_pclStgItem[i].siItemNum, pItemName, ToolTipMsg );
					pListView->InsertItem( &ListViewItemData );
				}
			}

		}
	}

}

void NPartyHallChief2Dlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	TCHAR buffer[ 256 ];

	CASE_CONTROL( LISTVIEW_MYINVENTORY )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				m_pMyInventory->SelectAction();

				TCHAR itemname[ 64 ];
				if( m_pMyInventory->m_clSelectedItem.GetFullName(					
					pclClient->pclItemManager, itemname, 64) == true) 
				{

					SetEditText( EDIT_INVEN_NAME, itemname );
		
					_itot( m_pMyInventory->m_clSelectedItem.siItemNum, buffer, 10 );
					SetEditText( EDIT_INVEN_NUM, buffer );
				}
			}
			break;
		}
	}

	else CASE_CONTROL( LISTVIEW_FEASTSTG )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				CListView *pListView = (CListView *)m_pControlMap[ LISTVIEW_FEASTSTG ];
				m_siSelectedStgIndex = pListView->GetSelectedItemIndex();

				if( m_siSelectedStgIndex < 0 ) return;

				SetEditText( EDIT_STG_NAME,
					(TCHAR*)pclClient->pclItemManager->GetName( 
					pFeastManager->m_pclStgItem[ m_siSelectedStgIndex ].siItemUnique ) );				

				_itot( pFeastManager->m_pclStgItem[ m_siSelectedStgIndex ].siItemNum, buffer, 10 );
				SetEditText( EDIT_STG_NUM, buffer );
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_INPUT )
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

		if( m_pMyInventory->m_siSelectedIndex < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1834);
			SetMainExplain(pText);
			return;
		}

		SI32 amount = _tstoi( GetEditText( EDIT_INVEN_NUM ) );

		if ( amount < 1 || 
			 amount > m_pMyInventory->m_pclItem[ m_pMyInventory->m_siSelectedIndex ].siItemNum )
		{
			TCHAR* pText = GetTxtFromMgr(1835);
			SetMainExplain(pText);
			return;
		}				

		cltGameMsgRequest_FeastStgItemIn sendMsg( 
			m_pMyInventory->m_siSelectedIndex, 
			m_pMyInventory->m_pclItem[ m_pMyInventory->m_siSelectedIndex ].siUnique, 
			amount );

		cltMsg clMsg( GAMEMSG_REQUEST_FEAST_STGITEMIN, sizeof(cltGameMsgRequest_FeastStgItemIn), (BYTE*)&sendMsg);				
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
	}

	else CASE_CONTROL( BUTTON_OUTPUT )
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


		if( m_siSelectedStgIndex < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1834);
			SetMainExplain(pText);
			return;
		}

		SI32 amount = _tstoi( GetEditText( EDIT_STG_NUM ) );

		if ( amount < 1 || 
			 amount > pFeastManager->m_pclStgItem[ m_siSelectedStgIndex ].siItemNum )
		{
			TCHAR* pText = GetTxtFromMgr(1835);
			SetMainExplain( pText );
			return;
		}				

		cltGameMsgRequest_FeastStgItemOut sendMsg( 
			m_siSelectedStgIndex, 
			pFeastManager->m_pclStgItem[ m_siSelectedStgIndex ].siItemUnique, 
			amount );

		cltMsg clMsg( GAMEMSG_REQUEST_FEAST_STGITEMOUT, sizeof(cltGameMsgRequest_FeastStgItemOut), (BYTE*)&sendMsg);				
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	}
	else CASE_CONTROL( EDIT_COST )
	{	
		CEdit* pEdit = (CEdit*)m_pControlMap[ EDIT_COST ];
		EditCountRange( pEdit, 1000000000000 );
	}
	else CASE_CONTROL( BUTTON_SETCOST )
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
 

		GMONEY cost;
		cost = _atoi64( GetEditText( EDIT_COST ) );

		//최소액수.
		SI32 MinCost = 500000;
		SI32 npcRate =  pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
		if( 500 < npcRate )
		{
			MinCost = npcRate * 1000;
		}
		SI32 MaxCost = MinCost * 10;

		// 최대액수
		if(cost > MaxCost)
		{ 
			TCHAR szBuffer[32] = "";
			g_JYLibFunction.SetNumUnit(MaxCost, szBuffer, sizeof(szBuffer));

			NTCHARString128 kbuf(GetTxtFromMgr(8276));
			kbuf.Replace("#money#", szBuffer);
			SetMainExplain(kbuf);
			return;
		}

		//최저액수 
		if(cost < MinCost)
		{
			TCHAR szBuffer[32] = "";
			g_JYLibFunction.SetNumUnit(MinCost, szBuffer, sizeof(szBuffer));

			NTCHARString128 kbuf(GetTxtFromMgr(1833));
			kbuf.Replace("#money#", szBuffer);
			SetMainExplain(kbuf);
			return;
		}
	
		cltGameMsgRequest_FeastSetCost sendmsg( cost );
		cltMsg clMsg( GAMEMSG_REQUEST_FEAST_SETCOST, sizeof( sendmsg ), (BYTE*)&sendmsg );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}


void NPartyHallChief2Dlg::EditCountRange( CEdit* _pEdit, GMONEY _MaxCount )
{
	GMONEY   ItemCount = _atoi64( _pEdit->GetText() );
	
	if( _MaxCount < ItemCount )
	{
		TCHAR szBuffer[64] = "";
		_i64toa( _MaxCount, szBuffer, 10 );
		_pEdit->SetText( szBuffer );
	}
	else if( 0 >= ItemCount )
	{
		_pEdit->SetText("");
	}
	else
	{
		TCHAR szCount[16] = "";
		_i64toa(ItemCount, szCount, 10);
		_pEdit->SetText(szCount);
	}
}
