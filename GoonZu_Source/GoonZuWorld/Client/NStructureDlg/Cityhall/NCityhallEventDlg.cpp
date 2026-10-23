#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "Msg\MsgType-Village.h"

#include "..\..\Client\Music\Music.h"
#include "..\..\MarkManager\MarkManager.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../Lib/JYLibFunction.h"
#include "../Client/Client.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallEventDlg::NCityhallEventDlg()
{
}

NCityhallEventDlg::~NCityhallEventDlg()
{
}

void NCityhallEventDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	
	SetControlMap( BUTTON_STARTEVENT, TEXT("button_startevent") );
	SetControlMap( BUTTON_PREV, TEXT("button_prev") );
	SetControlMap( BUTTON_NEXT, TEXT("button_next") );
	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );

	SetControlMap( EDIT_VILLAGENAME, TEXT("editbox_villagename") );
	SetControlMap( EDIT_EXPLAIN, TEXT("editbox_explain") );

	SetControlMap( LIST_EVENT, TEXT("listbox_event") );

	SetControlMap( IMAGE_MYVILLMARK, TEXT("imagestatic_myvillmark") );
	SetControlMap( IMAGE_VILLAGEMARK, TEXT("imagestatic_villagemark") );
	
	CButton *pButton = (CButton *)m_pControlMap[ BUTTON_STARTEVENT ];		
    pButton->Enable(TRUE);
	


	CList *pList = (CList *)m_pControlMap[ LIST_EVENT ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	// 이벤트 리스트 보여주기 
	int i;

	int width[] = { 180, 140 };
//	TCHAR *text[] = { TEXT("이벤트명"), TEXT("비용") };

	TCHAR* pText[2];

	//KHY - 0710 - 텍스트 수정. 
	pText[0] = GetTxtFromMgr(1529);
	pText[1] = GetTxtFromMgr(1530);

	for( i = 0; i < 2; ++i ) { 
		pList->SetColumn( i, width[ i ], pText[ i ] );
	}

	// 이벤트 리스트 보여주기 
	TCHAR buffer[ 256 ];
	int item = 0;
	for( i = 0; i < pCityhallManager->clEventManager.siEventNum; ++i ) {

		if( pCityhallManager->clEventManager.clEventInfo[ i ].eventid == 0 ) continue;

		// 이벤트명
		itemdata.Init();
		itemdata.siParam[0] = i;
		itemdata.Set( 0, pCityhallManager->clEventManager.clEventInfo[ i ].szEventName );
		
		SI32 cost = 0;
		cost = (SI32)(pCityhallManager->clEventManager.clEventInfo[ i ].siEventMoney * (pclClient->pclItemManager->clItemStatistic.siItemPriceIndex / 10000000.));
		cost = max(cost, 2000000);
		
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit( cost, buffer, 256, pText);
		itemdata.Set( 1, buffer );

		pList->AddItem( &itemdata );

	}

	pList->Refresh();




	// -------
	cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->siVillageUnique ];

	if( pVillageInfo ) {

		pCityhallManager->m_siMyVillMarkIndex = pVillageInfo->clVillageVary.siVillageMark;

	} else {

		pCityhallManager->m_siMyVillMarkIndex = 0;
	}

	pCityhallManager->m_siVillageMarkIndex = 1;

	pCityhallManager->siSelectedIndex = -1;

	// -------

	ShowVillageMark();

	// [영훈] 초보자 마을 : 초보자 마을은 이벤트 목록 안보임
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pCityhallManager->siVillageUnique )
		{
			pList->Show( false );
		}
	}
}

void NCityhallEventDlg::Action()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->siVillageUnique ];

	if( pVillageInfo ) {

		pCityhallManager->m_siMyVillMarkIndex = pVillageInfo->clVillageVary.siVillageMark;

	} else {

		pCityhallManager->m_siMyVillMarkIndex = 0;
	}

}


void NCityhallEventDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CImageStatic *pImageMyVill = (CImageStatic *)m_pControlMap[ IMAGE_MYVILLMARK ];
	CImageStatic *pImageVillage = (CImageStatic *)m_pControlMap[ IMAGE_VILLAGEMARK ];
	
	TSpr* pspr = NULL;
	SI32 fontIndex = 0;

	pclclient->m_pMarkManager->GetMarkSprAndFontIndex( pCityhallManager->m_siMyVillMarkIndex, &pspr, &fontIndex );

	if( pCityhallManager->m_siMyVillMarkIndex > 0 ) {

		GP.PutSpr(pspr, GetX() + pImageMyVill->GetX() + ScreenRX,
					GetY() + pImageMyVill->GetY() + ScreenRY,
						fontIndex );
	}
	pclclient->m_pMarkManager->GetMarkSprAndFontIndex( pCityhallManager->m_siVillageMarkIndex, &pspr, &fontIndex );

	if( pCityhallManager->m_siVillageMarkIndex > 0 ) {

		//pspr->PutSprT( GetX() + pImageVillage->GetX() + ScreenRX,
		GP.PutSpr(pspr, GetX() + pImageVillage->GetX() + ScreenRX,
					GetY() + pImageVillage->GetY() + ScreenRY,
						fontIndex );
	}

}

void NCityhallEventDlg::ShowVillageMark()
{

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	if( pCityhallManager->m_siVillageMarkIndex == 0 ) return;

	cltVillageInfo *pVillageInfo;

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i ) {
		pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ i ];

		if( pVillageInfo == NULL ) continue;

		if( pVillageInfo->clVillageVary.siVillageMark == pCityhallManager->m_siVillageMarkIndex )
		{
			SetEditText( EDIT_VILLAGENAME, pclClient->pclVillageManager->GetName( i ) );
			return;
		}

	}

	TCHAR* pText = GetTxtFromMgr(1533);
	SetEditText( EDIT_VILLAGENAME, pText );
}


void NCityhallEventDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	cltClient *pclclient = (cltClient*)pclClient;
	
	CList *pList = (CList *)m_pControlMap[ LIST_EVENT ];
	stListBoxItemData *pitemdata;

	if(pclClient->IsCountrySwitch(Switch_BasicStructureSet))
	{
		CButton *pButton = (CButton *)m_pControlMap[ BUTTON_STARTEVENT ];
		SI32 dura = pclClient->pclCityHallManager->clClientStrInfo.clCommonInfo.siStrDura;//건물의 내구도를 가지고 온다.
		if (dura == 0) 
		{
			pButton->Enable(FALSE);//만약 내구도가 0이면 이벤트 버튼 선택불가능
		}
	}

	CASE_CONTROL( LIST_EVENT )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

				pCityhallManager->siSelectedIndex = -1;
				SI16 index = pList->GetSelectedIndex();
				if( index < 0 ) return;

				pitemdata = pList->GetItemData( index );

				pCityhallManager->siSelectedIndex = index;

				SetEditText( EDIT_EXPLAIN,
					pCityhallManager->clEventManager.clEventInfo[ pCityhallManager->siSelectedIndex ].szEventExplain );
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_STARTEVENT )
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
		if( pCityhallManager->siSelectedIndex == -1 ) return;

		SI32 eventid = pCityhallManager->clEventManager.clEventInfo[ pCityhallManager->siSelectedIndex ].eventid;
		SI32 sivillageunique=pclCM->CR[1]->GetCurrentVillageUnique();//마을유니크
		SI32 residentnum = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siResidentNum;//주민수
		cltClient* pclclient = (cltClient*)pclClient;
			
		if( eventid == 0 ) return;

		// [영훈] 초보자 마을 : 초보자 마을은 이벤트 시작 못하게
		if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
		{
			if ( Const_Beginner_Village == pCityhallManager->siVillageUnique )
			{
				return;
			}
		}
		//
		if(pclClient->pclCM->CR[1]->pclCI->clBI.GetMapIndex() != pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage)
		{
			TCHAR* pText;
			pText = GetTxtFromMgr(6481); 
			SetMainExplain(pText);
			return;
		}

		//등록된 주민이 0 일경우에 수렵제 및 라쿤이벤트 불가.[2007.08.01 손성웅]
		if (residentnum == 0)  
		{
			TCHAR* pText;
			pText = GetTxtFromMgr(7555);
			SetMainExplain(pText);
			return;
		}
		switch(eventid)
		{
		case 1:		// 라티우스 대수렵제 (불가사리 이벤트)
			{		
				if (pclclient->siMofuMofuEvent_VilliageUnique == sivillageunique) 
				{
					TCHAR* pText;
					pText = GetTxtFromMgr(7556);
					SetMainExplain(pText);
					break;//모후모후 이벤트발생 마을 유니크와 같을 경우수렵이벤트 예약을 받지 않음[2007.08.02 손성웅]
				}

				cltGameMsgRequest_StartEvent sendMsg( eventid );
				cltMsg clMsg( GAMEMSG_REQUEST_STARTEVENT, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
			break;

		case 2:		// 모후모후 이벤트
			{
				if (residentnum == 0) 
				{
					TCHAR* pText;
					pText = GetTxtFromMgr(7555);
					SetMainExplain(pText);
					break;//등록된 주민이 0 일경우에 수렵제 및 라쿤이벤트 불가.[2007.08.01 손성웅]
				}

				pclclient->CreateInterface( NMOFUMOFULIST_DLG );
			}
			break;
		case 3:		//[진성] 몬스터 습격. => 2008-3-26
			{	
				cltGameMsgRequest_StartEvent sendMsg( eventid );
				cltMsg clMsg( GAMEMSG_REQUEST_STARTEVENT, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_PREV )
	{
		pCityhallManager->m_siVillageMarkIndex--;
		if( pCityhallManager->m_siVillageMarkIndex == 0 ) 
			pCityhallManager->m_siVillageMarkIndex = 1;

		ShowVillageMark();
	}

	else CASE_CONTROL( BUTTON_NEXT )
	{
		if( pCityhallManager->m_siVillageMarkIndex == pclclient->m_pMarkManager->GetTotalCount()  ) 
			pCityhallManager->m_siVillageMarkIndex = pclclient->m_pMarkManager->GetTotalCount();
		else pCityhallManager->m_siVillageMarkIndex++;

		ShowVillageMark();
	}

	else CASE_CONTROL( BUTTON_SETUP )
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

		if( pCityhallManager->m_siVillageMarkIndex == pCityhallManager->m_siMyVillMarkIndex ) return;

		TCHAR buffer[256];

		TCHAR* pText = GetTxtFromMgr(1531);
		StringCchPrintf(buffer, 256, pText);

		pText = GetTxtFromMgr(1532);
		
		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 20 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if(  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
		//{
		//	return;
		//}

		//if( pCityhallManager->m_siVillageMarkIndex < 1 ||
		//	pCityhallManager->m_siVillageMarkIndex > MAX_VILLAGEMARK_NUMBER ) return;
	

		//cltGameMsgRequest_SetVillageMark sendMsg( pCityhallManager->m_siVillageMarkIndex );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETVILLAGEMARK, sizeof( sendMsg ), (BYTE*)&sendMsg );
		//pclClient->pclCM->CR[1]->SendNetMsg( (TCHAR*)&clMsg );

	}

}

void NCityhallEventDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	cltClient* pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 20:	// 마을 마크 설정
		{
			if ( RetValue )
			{
				if( pCityhallManager->m_siVillageMarkIndex < 1 ||
					pCityhallManager->m_siVillageMarkIndex > pclclient->m_pMarkManager->GetTotalCount() ) return;
			

				cltGameMsgRequest_SetVillageMark sendMsg( pCityhallManager->m_siVillageMarkIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_SETVILLAGEMARK, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );

			}
		}
		break;
	}
}

