#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
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

NFeastUserDlg::NFeastUserDlg()
{

}

NFeastUserDlg::~NFeastUserDlg()
{

}

void NFeastUserDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	SetControlMap( BUTTON_GIVEITEM, TEXT("button_giveitem") );

	SetControlMap( EDIT_ITEMNAME, TEXT("editbox_itemname") );
	SetControlMap( EDIT_STATUS, TEXT("editbox_status") );
	SetControlMap( EDIT_NAME, TEXT("editbox_name") );
	SetControlMap( EDIT_DATE, TEXT("editbox_date") );
	SetControlMap( EDIT_REASON, TEXT("editbox_reason") );

	SetControlMap( LIST_PRESENT, TEXT("listbox_present") );

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[4];
	
	pText[0] = TEXT("");
	pText[1] = GetTxtFromMgr(1831);
	pText[2] = GetTxtFromMgr(1832);
	pText[3] = GetTxtFromMgr(1838);

	pList->SetColumn( 0, 40, pText[ 0 ] );
	pList->SetColumn( 1, 132, pText[ 1 ] );
	pList->SetColumn( 2, 72, pText[ 2 ] );
	pList->SetColumn( 3, 92, pText[ 3 ] );

	pList->Refresh();

	pFeastManager->siUserPresentSelectIndex = -1;
}

void NFeastUserDlg::Action()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	if( pFeastManager->bUpdateUser ) {
		pFeastManager->bUpdateUser = FALSE;

		ShowFeastInfo();
		ShowPresentInfo();
	}
}

void NFeastUserDlg::ShowFeastInfo()
{

	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	TCHAR buffer[256];
	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;

	if( pclFeastStrInfo->clFeastInfo.siStatus == 0 ) {

		TCHAR* pText = GetTxtFromMgr(1561);
		SetEditText( EDIT_NAME, pText );
		SetEditText( EDIT_DATE, pText );
		SetEditText( EDIT_REASON, pText );

	} else {

		SetEditText( EDIT_NAME, pclFeastStrInfo->clFeastResvInfo.szPersonName );

		TCHAR* pText = GetTxtFromMgr(1839);

		StringCchPrintf( buffer, 256, pText,
			pclFeastStrInfo->clFeastResvInfo.siYear,
			pclFeastStrInfo->clFeastResvInfo.siMonth,
			pclFeastStrInfo->clFeastResvInfo.siDay,
			pclFeastStrInfo->clFeastResvInfo.siHour,
			pclFeastStrInfo->clFeastResvInfo.siMinute );

		SetEditText( EDIT_DATE, buffer );
		SetEditText( EDIT_REASON, pclFeastStrInfo->clFeastResvInfo.szReason );
	}

	if( pclFeastStrInfo->clFeastInfo.siStatus != 1 ) {
		TCHAR* pText = GetTxtFromMgr(1840);
		SetEditText( EDIT_STATUS, pText );
	} else {
		TCHAR* pText = GetTxtFromMgr(1841);
		SetEditText( EDIT_STATUS, pText );
	}	
}

void NFeastUserDlg::ShowPresentInfo()
{

	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];
	stListBoxItemData itemdata;

	SI32 i, n;
	TCHAR buffer[256];
	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	n = 0;
	for( i = 0; i < MAX_FEAST_PRESENT_NUMBER; i++ )
	{
		if( pclFeastStrInfo->clFeastInfo.siItemUnique[ i ] ) {

			itemdata.Init();

			itemdata.siParam[0] = i;

			StringCchPrintf( buffer, 256, TEXT("%d"), n + 1 );
			itemdata.Set( 0, buffer );

			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclFeastStrInfo->clFeastInfo.siItemUnique[ i ] );
			itemdata.Set( 1, ItemName );

			StringCchPrintf( buffer, 256, TEXT("%d"), pclFeastStrInfo->clFeastInfo.siItemNum[ i ] );
			itemdata.Set( 2, buffer );

			if( pclFeastStrInfo->clFeastInfo.siItemGive[ i ] ) {
				TCHAR* pText = GetTxtFromMgr(1842);
				StringCchCopy( buffer, 256, pText );
			} else {
				TCHAR* pText = GetTxtFromMgr(1843);
				StringCchCopy( buffer, 256, pText );
			}
			itemdata.Set( 3, buffer );

			pList->AddItem( &itemdata );
			n++;
		}
	}

	pList->Refresh();

}

void NFeastUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	TCHAR buffer[256];

	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];
	stListBoxItemData *pItemData;

	CASE_CONTROL( LIST_PRESENT )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				pFeastManager->siUserPresentSelectIndex = -1;

				int index = pList->GetSelectedIndex();

				if( index < 0 ) return;

				pItemData = pList->GetItemData( index );

				pFeastManager->siUserPresentSelectIndex = pItemData->siParam[0];
				
				SI32 itemunique = pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[ pFeastManager->siUserPresentSelectIndex ];

				TCHAR* pText = GetTxtFromMgr(676);
				StringCchPrintf( buffer, 256, pText,
					pclClient->pclItemManager->GetName( itemunique ),
					pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemNum[ pFeastManager->siUserPresentSelectIndex ] );

				SetEditText( EDIT_ITEMNAME, buffer );
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_GIVEITEM )
	{
		if( pFeastManager->siUserPresentSelectIndex < 0 ) return;
			
		cltGameMsgRequest_FeastGiveItem sendmsg( 
			pFeastManager->siUserPresentSelectIndex,  
			pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[ pFeastManager->siUserPresentSelectIndex ],
			pFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siItemNum[ pFeastManager->siUserPresentSelectIndex ] );

		cltMsg clMsg(GAMEMSG_REQUEST_FEAST_GIVEITEM, sizeof(sendmsg), (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}