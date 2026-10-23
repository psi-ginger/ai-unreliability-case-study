#include "NHuntDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Hunt.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHuntChiefDlg::NHuntChiefDlg()
{
	m_pMyInventory = new NStrInventory;
}

NHuntChiefDlg::~NHuntChiefDlg()
{
	NDelete( m_pMyInventory );
}

void NHuntChiefDlg::InitDlg()
{
	m_Count = 0;

	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	SetControlMap( BUTTON_DELETE,		TEXT("button_delete") );
	SetControlMap( BUTTON_ADD,			TEXT("button_add") );
	SetControlMap( BUTTON_INPUT,		TEXT("button_input") );
	SetControlMap( BUTTON_OUTPUT,		TEXT("button_output") );

	SetControlMap( EDIT_MONSTERLEVEL,	TEXT("editbox_monsterlevel") );
	SetControlMap( COMBO_MONSTERNUMBER, TEXT("combobox_monsternumber"));
	SetControlMap( EDIT_INPUTNUMBER,	TEXT("editbox_inputnumber") );

	SetControlMap( LIST_MONSTERINFO,	TEXT("listbox_monsterinfo") );

	SetControlMap( COMBO_MONSTERKIND,	TEXT("combobox_monsterkind") );

	SetControlMap( LISTVIEW_MYINVENTORY, TEXT("listview_myinventory") );
	SetControlMap( LISTVIEW_HUNTSTG,	 TEXT("listview_huntstg") );


	// MyInventory
	m_pMyInventory->CreateInventory( MAX_ITEM_PER_PERSON, (CListView*)m_pControlMap[ LISTVIEW_MYINVENTORY ] );

	m_pMyInventory->m_pListView->SetBKColorUse( true );
	m_pMyInventory->m_pListView->SetBorder( true );

	m_pMyInventory->LoadMyInventory();

	// Stg Item
	CListView *pListView = (CListView *)m_pControlMap[ LISTVIEW_HUNTSTG ];
	
	pListView->SetBKColorUse( true );
	pListView->SetBorder( true );

	LoadHuntStg();

	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_MONSTERINFO ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1705);
	pList->SetColumn( 0, 96, pText );

	pText = GetTxtFromMgr(1506);
	pList->SetColumn( 1, 40, pText );

	pText = GetTxtFromMgr(1706);
	pList->SetColumn( 2, 40, pText );

	pList->Refresh();
	//----------------

	// 콤보박스 - 몬스터 숫자
	CComboBox *pCombo = (CComboBox*)m_pControlMap[ COMBO_MONSTERNUMBER ];
	pCombo->Clear();


	stComboBoxItemData comboitemdata;
	comboitemdata.Init();
	comboitemdata.Set( TEXT("10") );
	pCombo->AddItem( &comboitemdata );
	comboitemdata.Init();
	comboitemdata.Set( TEXT("20") );
	pCombo->AddItem( &comboitemdata );
	comboitemdata.Init();
	comboitemdata.Set( TEXT("30") );
	pCombo->AddItem( &comboitemdata );


	
	pHuntManager->bUpdateLineUpInfo = FALSE;

	LoadLineUpInfoList();

	SetEditText( EDIT_MONSTERLEVEL, TEXT("1") );

}

void NHuntChiefDlg::Action()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	if( m_pMyInventory->IsMyInventoryChanged() ) {
		m_pMyInventory->LoadMyInventory();
	}

	if( pHuntManager->IsHuntStorageInventoryChanged() ) {
		LoadHuntStg();
	}

	if( pHuntManager->bUpdateLineUpInfo ) {
		pHuntManager->bUpdateLineUpInfo = FALSE;
	
		LoadLineUpInfoList();
	} else {

		if( m_Count == 0 && m_pBaseDlg->m_dwActionCount % 5 ) {
			LoadLineUpInfoList();
		}
	}
}

void NHuntChiefDlg::LoadLineUpInfoList()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	TCHAR buffer[ 256 ];
	
	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_MONSTERINFO ];
	stListBoxItemData itemdata;

	pList->Clear();

	memset( pHuntManager->m_strListSelectedMonsterName, 0, 64 );
	
	m_Count = 0;

	for( int i = 0; i < 
		MAX_HUNT_LINEUP_NUMBER;	++i )
		
	{
		if ( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0)
		{
			
			TCHAR *MonsterName =(TCHAR*)pclClient->pclKindInfoSet->GetName( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind);
			if( MonsterName == NULL ) continue;
			
			m_Count++;

			itemdata.Init();
			itemdata.Set( 0, MonsterName );
			
			
			StringCchPrintf( buffer,  256, TEXT("%d"), pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindLevel );
			itemdata.Set( 1, buffer );
	
			StringCchPrintf( buffer,  256, TEXT("%d"), pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum );
			itemdata.Set( 2, buffer );
			
			pList->AddItem( &itemdata );
		}
	}

	pList->Refresh();

}

//몬스터종류 콤보박스 
void NHuntChiefDlg::LoadHuntStg()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	CComboBox *pCombo = (CComboBox*)m_pControlMap[ COMBO_MONSTERKIND ];
	stComboBoxItemData comboitemdata;

	CListView *pListView = (CListView*)m_pControlMap[ LISTVIEW_HUNTSTG ];

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

	pCombo->Clear();

	for ( SI32 i = 0; i < MAX_HUNT_STG_NUMBER; ++i )
	{
		pHuntManager->m_pclHuntStg[i].Set( 
			&pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntStg[ i ] );

		if ( pHuntManager->m_pclHuntStg[ i ].siItemUnique > 0 && pHuntManager->m_pclHuntStg[ i ].siItemNum > 0 )
		{

			pItemName = (TCHAR *)pclClient->pclItemManager->GetName( pHuntManager->m_pclHuntStg[ i ].siItemUnique );

			if( pItemName ) 
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( 
					pHuntManager->m_pclHuntStg[i].siItemUnique, 
					&GlobalImageIndex, &GlobalImageFont ) )
				{

				//	TCHAR* pText = GetTxtFromMgr(676);
				//	sprintf(buffer, pText, ItemName, m_pclItem[i].GetItemNum() );
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");

					cltItem TempItem;
					TempItem.Init();
					TempItem.siUnique = pHuntManager->m_pclHuntStg[ i ].siItemUnique;
					TempItem.siItemNum = pHuntManager->m_pclHuntStg[ i ].siItemNum;

					pclClient->pclItemManager->GetExplain( &TempItem, ToolTipMsg );
										
					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,pHuntManager->m_pclHuntStg[i].siItemNum, pItemName, ToolTipMsg );
					pListView->InsertItem( &ListViewItemData );
				}
			}


			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pHuntManager->m_pclHuntStg[i].siItemUnique );
			if(ref > 0)
			{
				SI32 monkind = pclClient->pclItemManager->pclItemInfo[ref]->siMakeKind;
				TCHAR *strMonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( monkind );//이름을 잘못 가저 온다!!!!!

				if ( strMonsterName != NULL )
				{
					comboitemdata.Init();
					comboitemdata.Set( strMonsterName );
					pCombo->AddItem( &comboitemdata );
				}
			}

		}
	}



}



void NHuntChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	CASE_CONTROL( LIST_MONSTERINFO )
	{
		CList *pList = (CList *)m_pControlMap[ LIST_MONSTERINFO ];
		stListBoxItemData *pItemData;

		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				int index = pList->GetSelectedIndex();
				if( index < 0 ) return;

				pItemData = pList->GetItemData( index );

				MStrCpy( pHuntManager->m_strListSelectedMonsterName,
					(TCHAR *)pItemData->strText, 32 );
			}
			break;
		}

	}

	else CASE_CONTROL( LISTVIEW_MYINVENTORY )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				m_pMyInventory->SelectAction();
			}
			break;
		}
	}

	else CASE_CONTROL( LISTVIEW_HUNTSTG )
	{
		switch( nEvent )
		{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			{
				CListView *pListView = (CListView *)m_pControlMap[ LISTVIEW_HUNTSTG ];
				m_siSelectedStgIndex = pListView->GetSelectedItemIndex();
			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_DELETE )
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

		if( pHuntManager->m_strListSelectedMonsterName != NULL && 
			_tcslen( pHuntManager->m_strListSelectedMonsterName ) > 0 )
		{
			SI32 MonKind = pclClient->pclKindInfoSet->FindKindFromName( pHuntManager->m_strListSelectedMonsterName );

			if ( MonKind > 0 )
			{
				for ( int i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
				{
					if ( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind == MonKind )
					{
						cltGameMsgRequest_HuntMonsterFieldListDel clHuntMonsterFieldListDel( pHuntManager->siVillageUnique, i );
						cltMsg clMsg( GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTDEL, 
							sizeof(clHuntMonsterFieldListDel), 
							(BYTE*)&clHuntMonsterFieldListDel );

						pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
					}
				}						
			}
		}
	}

	else CASE_CONTROL( BUTTON_ADD )
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

		cltClient *pclclient = (cltClient *)pclClient;

		CComboBox *pCombo = (CComboBox*)m_pControlMap[ COMBO_MONSTERKIND ];
		stComboBoxItemData *pItemData;

		SI16 siComboBoxIndex = pCombo->GetSelectedIndex();

		if( siComboBoxIndex < 0 ) return;

		TCHAR *pSelText;
		pItemData = pCombo->GetItemData( siComboBoxIndex );
		pSelText = pItemData->strText;
		
		SI32 monkind = 0;

		for( int i = 0; i < MAX_HUNT_STG_NUMBER; ++i )
		{
			if ( pHuntManager->m_pclHuntStg[i].siItemUnique != 0 )
			{
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pHuntManager->m_pclHuntStg[i].siItemUnique );
				if(ref > 0)
				{
					monkind = pclClient->pclItemManager->pclItemInfo[ref]->siMakeKind;
					TCHAR *strMonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( monkind );

					if ( strMonsterName != NULL )
					{
						if ( pItemData != NULL && _tcscmp( pSelText, strMonsterName ) == 0 )
						{
							break;
						}
					}
				}							
			}
		}

		if ( i == MAX_HUNT_STG_NUMBER )
		{
			return;
		}
		

		SI16 siMonsterLevel = _tstoi( GetEditText( EDIT_MONSTERLEVEL ) );

		if ( siMonsterLevel < 1 || siMonsterLevel > MAX_CHARACTER_LEVEL )
		{
			return;
		}

		CComboBox *pComboMonsterNumber = (CComboBox*)m_pControlMap[ COMBO_MONSTERNUMBER ];
		SI16 selectedindex = pComboMonsterNumber->GetSelectedIndex();

		SI16 siMonsterNum = 0;

		if ( selectedindex > -1 )
		{
			stComboBoxItemData *pItemDataNumber;
			pItemDataNumber = pComboMonsterNumber->GetItemData( selectedindex );
			siMonsterNum = _tstoi( pItemDataNumber->strText );
		}

		if ( siMonsterNum < 1 )
		{	
			TCHAR* pTitle = GetTxtFromMgr(2150);
			TCHAR* pText = GetTxtFromMgr(2151);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}

		if ( siMonsterNum > MAX_HUNT_LINEUP_EACH_MONSTER_NUM )
		{
			TCHAR* pTitle = GetTxtFromMgr(2150);
			TCHAR* pText = GetTxtFromMgr(2152);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_HUNT_LINEUP_EACH_MONSTER_NUM );
			return;
		}
		
		SI32 CurrentMonsterTotalNum = 0;
		for ( i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
		{
			if ( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 && 
				pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind != monkind )
			{
				CurrentMonsterTotalNum += pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum;
			}
		}

		if ( siMonsterNum < 1 || siMonsterNum + CurrentMonsterTotalNum > MAX_HUNT_MONSTER_NUM )
		{
			TCHAR* pTitle = GetTxtFromMgr(2150);
			TCHAR* pText = GetTxtFromMgr(2152);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_HUNT_MONSTER_NUM );
			return;
		}
		
		cltGameMsgRequest_HuntMonsterFieldListAdd clHuntMonsterFieldListAdd( pHuntManager->siVillageUnique, monkind, siMonsterLevel, siMonsterNum );
		cltMsg clMsg( GAMEMSG_REQUEST_HUNTMONSTERFIELDLISTADD, sizeof(clHuntMonsterFieldListAdd), (BYTE*)&clHuntMonsterFieldListAdd );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg );

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

		if ( m_pMyInventory->m_siSelectedIndex < 0 )
		{
			return;
		}

		SI32 StorageMonsterNum = _tstoi( GetEditText( EDIT_INPUTNUMBER ) );
		if( StorageMonsterNum < 1 || 
			StorageMonsterNum > m_pMyInventory->m_pclItem[m_pMyInventory->m_siSelectedIndex].siItemNum )
		{
			return;
		}
	
		//[진성] 레어 소환인형은 수련장에 등록이 불가능하게 한다. 2008-7-15
		if(pclClient->pclItemManager->IsItemInfoAtb( m_pMyInventory->m_pclItem[m_pMyInventory->m_siSelectedIndex].siUnique, ITEMINFOATB_RARE_SUMMONDOLL))
		{
			TCHAR* pText = GetTxtFromMgr(8581);
			SetMainExplain(pText);
			return;
		}

		cltGameMsgRequest_HuntMonsterIn clHuntMonsterIn( 
			pHuntManager->siVillageUnique, 
			m_pMyInventory->m_siSelectedIndex, 
			m_pMyInventory->m_pclItem[m_pMyInventory->m_siSelectedIndex].siUnique, 
			StorageMonsterNum );

		cltMsg clMsg( GAMEMSG_REQUEST_HUNTMONSTERIN, sizeof(clHuntMonsterIn), (BYTE*)&clHuntMonsterIn );
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

		if ( m_siSelectedStgIndex < 0 )
		{
			return;
		}

		SI32 StorageMonsterNum = _tstoi( GetEditText( EDIT_INPUTNUMBER ) );
		if( StorageMonsterNum < 1 ) 
		{
			return;
		}

		cltGameMsgRequest_HuntMonsterOut clHuntMonsterOut( 
			pHuntManager->siVillageUnique, 
			m_siSelectedStgIndex, 
			pHuntManager->m_pclHuntStg[m_siSelectedStgIndex].siItemUnique, 
			StorageMonsterNum );

		cltMsg clMsg( GAMEMSG_REQUEST_HUNTMONSTEROUT, sizeof(clHuntMonsterOut), (BYTE*)&clHuntMonsterOut );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
	}

}