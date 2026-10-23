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

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NFeastChiefDlg::NFeastChiefDlg()
{
}

NFeastChiefDlg::~NFeastChiefDlg()
{
	
}

void NFeastChiefDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	pFeastManager->siChiefPresentSelectIndex = -1;

	SetControlMap( BUTTON_ADDPRESENT, TEXT("button_addpresent") );
	SetControlMap( BUTTON_DELPRESENT, TEXT("button_delpresent") );
	SetControlMap( BUTTON_SETCOST, TEXT("button_setcost") );

	SetControlMap( EDIT_ITEMNUM, TEXT("editbox_itemnum") );
	SetControlMap( EDIT_COST, TEXT("editbox_cost") );

	SetControlMap( LIST_PRESENT, TEXT("listbox_present") );

	SetControlMap( COMBO_ITEMTYPE, TEXT("combobox_itemtype") );
	SetControlMap( COMBO_ITEM, TEXT("combobox_item") );

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];

	//[김영훈 추가:2007.11.21 - 선물할 아이템 겟수 입력시 5자리(9999)까지 입력되도록 수정]
	((CEdit*)m_pControlMap[ EDIT_ITEMNUM ])->SetMaxEditTextLength(6);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[3];
	
	pText[0] = TEXT("");
	pText[1] = GetTxtFromMgr(1831);
	pText[2] = GetTxtFromMgr(1832);

	pList->SetColumn( 0, 40, pText[ 0 ] );
	pList->SetColumn( 1, 168, pText[ 1 ] );
	pList->SetColumn( 2, 126, pText[ 2 ] );

	pList->Refresh();


	// 아이템 종류를 보여준다. 
	{
		CComboBox *pComboItemType = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
		stComboBoxItemData itemdata;

		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		int i;
		// 콤보 박스를 지운다.
		pComboItemType->Clear();

		// 콤보 박스에 아이템의 종류를 설정한다. 
		for( i = 0; i < MAX_ITEMTYPE_NUMBER; i++ )
		{	
			cltItemTypeInfo* pinfo = pclitemmanager->pclItemTypeInfo[i];
			if(pinfo == NULL)continue;

			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;
			if(pinfo->IsAtb(ITEMATB_OBJECT))continue;
			// 콤보리스트에서 숨기는 속성이 있다면
			if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;


			itemdata.Init();
			itemdata.Set( (TCHAR*)pinfo->GetName() );
			pComboItemType->AddItem( &itemdata );
		}
	}


	ShowPresentList();
}

void NFeastChiefDlg::Action()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	if( pFeastManager->bUpdateChief2 ) {
		pFeastManager->bUpdateChief2 = FALSE;

		ShowPresentList();
	}
}

void NFeastChiefDlg::ShowPresentList()
{

	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];
	stListBoxItemData itemdata;

	TCHAR buffer[256];
	SI32 index = 0;

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	for( int i = 0; i < MAX_FEAST_PRESENT_NUMBER; ++i ) {

		if( pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[ i ] ) {

			itemdata.Init();
			itemdata.siParam[0] = i;

			StringCchPrintf( buffer, 256, TEXT("%d"), index + 1 );
			itemdata.Set( 0, buffer );	

			itemdata.Set( 1, (TCHAR*)pclClient->pclItemManager->GetName( 
				pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[ i ] ) );

			_itot( pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemNum[ i ], buffer, 10 );
			itemdata.Set( 2, buffer );

			pList->AddItem( &itemdata );
			index++;

		}
	}

	pList->Refresh();
}

BOOL NFeastChiefDlg::ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	SI32 i;
	// 콤보 박스를 지운다.
	pCombo->Clear();

	stComboBoxItemData itemdata;

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( i = 0;i < MAX_ITEMINFO_NUMBER;i++ )
	{	
		cltItemInfo* pinfo = pclitemmanager->pclItemInfo[i];

		if(pinfo == NULL)continue;
		if(pinfo->siType != itemtypeunique)continue;

		// 주어진 타입과 같은 아이템만 열거한다. 
		// 사용자에게 보여지는 아이템인 경우에만. 
		bool bshow = false; 

		if(pinfo->IsItemInfoAtb(ITEMINFOATB_NOTTRADE))
		{
			continue;
		}

		if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
		{
			if(btradeinterface == true)
			{
			//	if(pinfo->IsItemInfoAtb(ITEMINFOATB_TRADELIST)  == true)
				{
					bshow = true;
				}
			}
			else
			{
				bshow = true;
			}

			if(bshow)
			{
				itemdata.Init();
				itemdata.Set( (TCHAR *)pinfo->GetName() );
				pCombo->AddItem( &itemdata );
			}
		}
	}

	return TRUE;
}


void NFeastChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	//TCHAR buffer[256];
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];
	stListBoxItemData *pItemData;

	CComboBox *pComboItemType = (CComboBox *)m_pControlMap[ COMBO_ITEMTYPE ];
	CComboBox *pComboItem = (CComboBox *)m_pControlMap[ COMBO_ITEM ];
	stComboBoxItemData *pComboItemData;

	CASE_CONTROL( COMBO_ITEMTYPE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				int index = pComboItemType->GetSelectedIndex();
				if( index < 0 ) return;

				TCHAR *szSelText = pComboItemType->GetText( index );
	
				SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
				if(itemtypeunique >=0)
				{
					ShowItemListInCombo( pComboItem, itemtypeunique, false );
					pComboItem->SetCursel( 0 );
				}
			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_ADDPRESENT )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_FEAST ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}


		int index = pComboItem->GetSelectedIndex();
		if( index < 0 ) return;

		pComboItemData = pComboItem->GetItemData( index );

		SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName( pComboItemData->strText );

		SI32 itemnum = _tstoi( GetEditText( EDIT_ITEMNUM ) );
		
		//[김영훈 추가:2007.11.26-오버플로어 발생 방지를 위해 최대값 이상 들어갔을때 무조건 최대값 + 1로 변환]
		if ( itemnum > MAX_ITEM_PILE_NUMBER ) 
			itemnum = MAX_ITEM_PILE_NUMBER + 1;

		cltGameMsgRequest_FeastAddPresentList sendmsg( itemunique, itemnum );
		cltMsg clMsg(GAMEMSG_REQUEST_FEAST_ADDPRESENTLIST, sizeof(sendmsg), (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	else CASE_CONTROL( BUTTON_DELPRESENT )
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


		if( pFeastManager->siChiefPresentSelectIndex < 0 ) return;

		SI32 itemunique = pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[ pFeastManager->siChiefPresentSelectIndex ];
		SI32 itemnum = pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemNum[ pFeastManager->siChiefPresentSelectIndex ];
		
		cltGameMsgRequest_FeastDelPresentList sendmsg( 
			pFeastManager->siChiefPresentSelectIndex,
			itemunique,
			itemnum
			);

		cltMsg clMsg( GAMEMSG_REQUEST_FEAST_DELPRESENTLIST, sizeof(sendmsg), (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);


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


		SI32 cost;

		cost = _tstoi( GetEditText( EDIT_COST ) );

		if( cost < 1000000 ) 
		{
			TCHAR szBuffer[32] = "";
			g_JYLibFunction.SetNumUnit( 1000000, szBuffer, 128 );
			
			NTCHARString128 kbuf(GetTxtFromMgr(1833));
			kbuf.Replace("#money#", szBuffer);
			SetMainExplain( kbuf );
			return;
		}

		cltGameMsgRequest_FeastSetCost sendmsg( cost );
		cltMsg clMsg( GAMEMSG_REQUEST_FEAST_SETCOST, sizeof( sendmsg ), (BYTE*)&sendmsg );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( LIST_PRESENT )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				pFeastManager->siChiefPresentSelectIndex = -1;

				int index = pList->GetSelectedIndex();
				if( index < 0 ) return;

				pItemData = pList->GetItemData( index );

				pFeastManager->siChiefPresentSelectIndex = 
					pItemData->siParam[0];

			}
			break;
		}
	}

}