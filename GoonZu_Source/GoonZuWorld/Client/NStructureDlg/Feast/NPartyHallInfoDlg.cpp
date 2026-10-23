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

NPartyHallInfoDlg::NPartyHallInfoDlg()
{
	m_siPartyIndex = 0;	// 0번 맵의 파티장이 기본 
	
}

NPartyHallInfoDlg::~NPartyHallInfoDlg()
{

}

void NPartyHallInfoDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	SetControlMap( BUTTON_ENTRANCE_APPLICATION, TEXT("button_giveitem") );
	SetControlMap( COMBOBOX_PARTYHALL_LIST,		TEXT("combobox_partyhall_list") );
	SetControlMap( EDIT_NAME,					TEXT("editbox_name") );
	SetControlMap( EDIT_DATE,					TEXT("editbox_date") );
	SetControlMap( EDIT_REASON,					TEXT("editbox_reason") );
	SetControlMap( EDIT_ENTRANCE_TYPE,			TEXT("editbox_enter_type") );
	SetControlMap( LIST_PRESENT,				TEXT("listbox_present") );

	
	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[4];
	
	pText[0] = TEXT("");
	pText[1] = GetTxtFromMgr(1831);
	pText[2] = GetTxtFromMgr(1832);
	//pText[3] = GetTxtFromMgr(1838);

	pList->SetColumn( 0,  40,  pText[ 0 ] );
	pList->SetColumn( 1, 224,  pText[ 1 ] );
	pList->SetColumn( 2,  72,  pText[ 2 ] );
	//pList->SetColumn( 3,  92,  pText[ 3 ] );

	pList->Refresh();

	// 파티장 리스트 세팅.
	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBOBOX_PARTYHALL_LIST ];
	if( NULL == pCombo )
		return;

	
    
	pCombo->Clear();
	stComboBoxItemData itemdata;

	TCHAR* pMapName   = NULL;
	TCHAR  buffer[64] = "";
	
	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
	{	
		pMapName =  pclClient->pclMapManager->GetMapName( PARTYHALL_START_MAP_INDEX + i );	  
		if(0 == strcmp(pMapName, "empty_partyhall"))
			continue;

		itemdata.Init();
		itemdata.Set( pMapName );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel(0);
	m_siPartyIndex = 0;
	  	
	// 파티홀의 전체 정보를 서버에 요청.
	cltGameMsgRequest_PartyHall_Total_Info cltinfo(m_siPartyIndex);
	cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_TOTAL_INFO, sizeof(cltGameMsgRequest_PartyHall_Total_Info), (BYTE*)&cltinfo);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NPartyHallInfoDlg::Action()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	if( pFeastManager->bUpdateUser ) 
	{
		pFeastManager->bUpdateUser = FALSE;

	
	}
}

void NPartyHallInfoDlg::ShowFeastInfo()
{
	
}

void NPartyHallInfoDlg::ShowPartPresentInfo( SI16 _ListIndex )
{
	cltFeastManager*		pFeastManager			= (cltFeastManager *)m_pStrManager;
	
	// List 
	CList *pList = (CList*)m_pControlMap[ LIST_PRESENT ];
	stListBoxItemData itemdata;

	SI32							i = 0, n = 0;
	TCHAR							buffer[256];
	cltPartyHallPresentItemInfo*	pclPresentItemList = &pFeastManager->clPartyHallResvInfo[ m_siPartyIndex ][_ListIndex].m_PresentItem[0];

	// 리스트를 모두 지운다.    
	pList->Clear();
	
	for( i = 0; i < RESERVATIO_PRESENT_ITEM; i++ )
	{
		if( pclPresentItemList[i].m_siItemUnique )
		{
			itemdata.Init();

			itemdata.siParam[0] = i;

			StringCchPrintf( buffer, 256, TEXT("%d"), n + 1 );
			itemdata.Set( 0, buffer );

			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclPresentItemList[i].m_siItemUnique );
			itemdata.Set( 1, ItemName );

			StringCchPrintf( buffer, 256, TEXT("%d"), pclPresentItemList[i].m_siItemNum );
			itemdata.Set( 2, buffer );

			pList->AddItem( &itemdata );
			n++;
		}
	}

	pList->Refresh();

}

void NPartyHallInfoDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
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
			}
			break;
		}
	}
	// 파티참가 신청을 한다.
	else CASE_CONTROL( BUTTON_ENTRANCE_APPLICATION )
	{
		SI32 siPersonID = pclClient->pclCM->CR[1]->pclCI->GetPersonID();
		
		cltGameMsgRequest_PartyHall_Entrance_Application clinfo( siPersonID, m_siPartyIndex );
		
		// 연회에 참여.
		cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_ENTRANCE_APPLICATION, sizeof(clinfo), (BYTE*)&clinfo );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);				
	}
	else CASE_CONTROL( COMBOBOX_PARTYHALL_LIST )
	{
		switch( nEvent )
		{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					CComboBox* pComboBox = (CComboBox*)m_pControlMap[ COMBOBOX_PARTYHALL_LIST ];

					SI16 Index = pComboBox->GetSelectedIndex();
					
					if( 0 > Index )
						return;
					
					if( m_siPartyIndex == Index )
						return;

					m_siPartyIndex = Index;

					SetPartyInfo();
				}
				break;  
		}
	}
}

void NPartyHallInfoDlg::SetPartyInfo( void )
{
	ClearControll();

	cltClient* pclclient = (cltClient*)pclClient;

	cltFeastManager*	 pFeastManager			 = (cltFeastManager *)m_pStrManager;
	cltPatyHallResvInfo* pclPatyHallResvInfoList = &pFeastManager->clPartyHallResvInfo[ m_siPartyIndex ][0];

    cltPatyHallResvInfo* pclPatyHallResvInfo = NULL;

	SI32 ResvCount = 0;
	for( ResvCount = 0; ResvCount < MAX_PARTYHALL_RESERVATION_INFO_LIST; ++ResvCount )
	{
		if( pclclient->sTime.wMonth == pclPatyHallResvInfoList[ResvCount].m_SystemTime.wMonth &&
			pclclient->sTime.wHour  == pclPatyHallResvInfoList[ResvCount].m_SystemTime.wHour)
		{
			pclPatyHallResvInfo	= &pclPatyHallResvInfoList[ResvCount];
			break;
		}
	}

	if(NULL == pclPatyHallResvInfo)
	{
		SetEditText(EDIT_NAME, GetTxtFromMgr(1561));
		return;
	}

	if(0 == strlen(pclPatyHallResvInfo->m_szPersonName))
	{
		SetEditText(EDIT_NAME, GetTxtFromMgr(1561));
	}

	SetEditText( EDIT_NAME, pclPatyHallResvInfo->m_szPersonName );

	TCHAR	buffer[128] = "";
	TCHAR*	pText		= GetTxtFromMgr(1839);
	StringCchPrintf( buffer, sizeof(buffer), pText,	pclPatyHallResvInfo->m_SystemTime.wYear,
													pclPatyHallResvInfo->m_SystemTime.wMonth,
													pclPatyHallResvInfo->m_SystemTime.wDay,
													pclPatyHallResvInfo->m_SystemTime.wHour,
													pclPatyHallResvInfo->m_SystemTime.wMinute );
	SetEditText( EDIT_DATE,   buffer );

	SetEditText( EDIT_REASON, pclPatyHallResvInfo->m_szReason );

	switch(pclPatyHallResvInfo->m_siEntranceType)
	{
		case TYPE_ENTRANCE_PUBLIC:	pText = GetTxtFromMgr(6290); break;
		case TYPE_ENTRANCE_VILLAGE:	pText = GetTxtFromMgr(8299); break;	
		case TYPE_ENTRANCE_GUILD:	pText = GetTxtFromMgr(8300); break;
		case TYPE_ENTRANCE_PRIVATE:	pText = GetTxtFromMgr(6291); break;
	}
	
	SetEditText( EDIT_ENTRANCE_TYPE, pText );

	// 선물 세팅.
	ShowPartPresentInfo( ResvCount );	
}

void NPartyHallInfoDlg::ClearControll( void )
{   
	((CEdit*)m_pControlMap[ EDIT_NAME ])->SetText("");   
	((CEdit*)m_pControlMap[ EDIT_DATE ])->SetText("");
	((CEdit*)m_pControlMap[ EDIT_REASON ])->SetText("");
	((CEdit*)m_pControlMap[ EDIT_ENTRANCE_TYPE ])->SetText("");
	((CList*)m_pControlMap[ LIST_PRESENT ])->Clear();
	((CList*)m_pControlMap[ LIST_PRESENT ])->Refresh();  

	
}