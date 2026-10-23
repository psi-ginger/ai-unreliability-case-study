//---------------------------------
// 2004/3/17 이제형
//---------------------------------

#include "../Client.h"
#include "../Music/Music.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType-Structure.h"
#include "../CommonLogic/Msg/MsgType-Village.h"
#include "../CommonLogic/MsgType-Stock.h"

// 필살기 효과. 
#include "Char/CharClient/CharCriticalEffect/CharCriticalEffect.h"
#include "Char/CharClient/CharJim/CharJim.h"

#include "Father/Father.h"
#include "School/School.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../NInterface/NGeneralMeetingLetterDlg.h"
#include "../NStructureDlg/Cityhall/NCityhallGeneralMeeting2Dlg.h"
#include "../NStructureDlg/Cityhall/NCityhallDlg.h"
#include "../InterfaceMgr/Interface/ChatEdit.h"

#include "../Interface/RecommendPersonDlg/RecommendPersonDlg.h"
#include "../ninterface/ninfodlg/NInfoDlg.h"
#include "../NInterface/NVillageDetailInfo/NVillageDetailInfo.h"
#include "../ninterface/NContributionEvent_CommonDlg/NContributionEvent_CommonDlg.h"
#include "../ninterface/NNewNotify/NNewNotifyDlg.h"

#include "../../CommonLogic/StructureClass/StructureClass.h"

extern cltCommonLogic* pclClient;

// 마을 상세 정보용 건물 정보 통보. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO(cltMsg* pclMsg, SI32 id)
{
	cltClient* pclclient = (cltClient*)pclClient;

	//cltStrInfo* pclinfo = (cltStrInfo* )pclMsg->cData;
	cltGameMsgResponse_VillageInfoStrInfo* pclinfo = (cltGameMsgResponse_VillageInfoStrInfo*)pclMsg->cData;
	
	TCHAR buffer[1800]	= TEXT("");
	TCHAR temp[256];

	TCHAR* pText = NULL;
	TCHAR VillName[MAX_PATH];	// 256
	TCHAR State[100];	// 정상, 파산, 안정, 초기화기
	TCHAR Chief[100];	// 관청 대행수 존재 여부 : 있음, 없음
	TCHAR Meeting[100];	// 주주총회 진행 여부 : 있음, 없음
	TCHAR Stock[100];	// 주식 수 : ~주
	TCHAR Next[100];		// 다음 상태
	TCHAR Detail1[400];	// 상세 정보1
	TCHAR Detail2[400];	// 상세 정보2

	VillName[ 0 ] = NULL; 
	State[ 0 ] = NULL; 
	Chief[ 0 ] = NULL;
	Meeting[ 0 ] = NULL;
	Stock[ 0 ] = NULL;
	Next[ 0 ] = NULL;
	Detail1[ 0 ] = NULL; 
	Detail2[ 0 ] = NULL; 

	cltDate remaineddate;
	memset( &remaineddate, 0, sizeof(cltDate) );

	// 마을 이름 구함
	StringCchCopy( VillName, MAX_PATH, pclclient->pclVillageManager->GetName( pclinfo->siVillageUnique ) );

	switch(pclinfo->siStrType)
	{
	case RANKTYPE_HUNT:
		{
			switch(pclinfo->clTargetStr.clHuntStrInfo.siHuntStatus)
			{
			case HUNT_ENTER_ITEMINSUFFICIENY:	// 재고 부족상황. 
				pText = GetTxtFromMgr(1392);
				StringCchCat(buffer, 1800, pText);
				break;

			case HUNT_ENTER_ENABLESTATUS:	// 정상 이용가능 상황. 
			default:
				pText = GetTxtFromMgr(1393);
				StringCchCat(buffer, 1800, pText);
				break;
			}

			pText = GetTxtFromMgr(1394);
			StringCchPrintf(temp, 256, pText, pclinfo->clTargetStr.clHuntStrInfo.siUserNum);
			StringCchCat(buffer, 1800, temp);

			// 몬스터 배치 현황
			for(SI32 i = 0;i < MAX_HUNT_LINEUP_NUMBER;i++)
			{
				SI32 kind	= pclinfo->clTargetStr.clHuntStrInfo.clHuntLineUp[i].siLineUpKind;
				SI32 num	= pclinfo->clTargetStr.clHuntStrInfo.clHuntLineUp[i].siLineUpKindNum;
				if(kind <= 0)continue;

				pText = GetTxtFromMgr(1395);
				StringCchPrintf(temp, 256, pText, pclKindInfoSet->GetName(kind), num);

				StringCchCat(buffer, 1800, temp);
			}
		}
		break;
	case RANKTYPE_CITYHALL:
		{
			// 상세정보1 : 디폴트 "없음"
			StringCchPrintf( Detail1, 400, TEXT("%s"), GetTxtFromMgr(1287) );

			switch( pclinfo->clTargetStr.clCityhallStrInfo.usCurStatus )
			{
			case 	CITYHALL_BEING_BANKRUPT	:		// 파산기
				StringCchPrintf( State, 100, TEXT("%s"), GetTxtFromMgr(5240) );
				break;
			case 	CITYHALL_BEING_INITIALIZED	:	// 초기화기
				StringCchPrintf( State, 100, TEXT("%s"), GetTxtFromMgr(5241) );
				break;
			case 	CITYHALL_BEING_STABLE		:	// 안정기
				StringCchPrintf( State, 100, TEXT("%s"), GetTxtFromMgr(5242) );
				StringCchPrintf( Next, 100, TEXT("%s"), GetTxtFromMgr(5239) );
				break;
			case CITYHALL_BEING_NORMAL			:	// 정상기
				StringCchPrintf( State, 100, TEXT("%s"), GetTxtFromMgr(5239) );
				StringCchPrintf( Next, 100, TEXT("%s"), GetTxtFromMgr(5240) );	// 파산기
				break;
			default:
				break;
			}

			pText = GetTxtFromMgr(1644);	// %d년 %d월 %d일

			if( pclinfo->siParam1 <= 0 )	// 대행수 없음	: 1287
			{
                StringCchPrintf( Chief, 100, TEXT("%s"), GetTxtFromMgr(1287) );
			}
			else	// 있음 : 1952
			{
				StringCchPrintf( Chief, 100, TEXT("%s"), GetTxtFromMgr(1952) );
			}

			if( pclinfo->siParam2 <= 0 ) // 주총 없음 
			{
                StringCchPrintf( Meeting, 100, TEXT("%s"), GetTxtFromMgr(1287) );
			}
			else	// 있음 : 1952
			{
				StringCchPrintf( Meeting, 100, TEXT("%s"), GetTxtFromMgr(1952) );
			}

			if( pclinfo->clTargetStr.clCityhallStrInfo.siTotalStockAmount <= 0 )
			{
				StringCchPrintf( Stock, 100, TEXT("%d"), 0 );	// 주식 없음
			}
			else
			{	// 주식 수 
				StringCchPrintf( Stock, 100, TEXT("%d"), pclinfo->clTargetStr.clCityhallStrInfo.siTotalStockAmount );
			}
			
			switch( pclinfo->clTargetStr.clCityhallStrInfo.usCurStatus )
			{
			case CITYHALL_BEING_NORMAL:			// 정상기
				{
					// siParam1 - 0 : 대행수 없음 , 0 보다 큼 : 대행수 personid
					if( pclinfo->siParam1 <= 0 )
					{
						remaineddate = pclclient->pclCityHallManager->GetCityHallBankruptcyDetailInfo( (cltCityhallStrInfo*)(&pclinfo->clTargetStr) );
                        StringCchPrintf( Detail1, 400, pText, remaineddate.uiYear, remaineddate.uiMonth, remaineddate.uiDay );
					}
				}
				break;
			case CITYHALL_BEING_BANKRUPT:		// 파산기
				{
					StringCchPrintf( Detail2, 400, TEXT("%s"), GetTxtFromMgr( 5198 ) );	// 파산기에는 ...
				}
				break;
			case CITYHALL_BEING_INITIALIZED:	// 초기화기
				{
					StringCchPrintf( Detail2, 400, GetTxtFromMgr(5241) );
				}
				break;
			case CITYHALL_BEING_STABLE:			// 안정기
				{
					StringCchPrintf( Detail1, 400, pText, remaineddate.uiYear, remaineddate.uiMonth, remaineddate.uiDay );
					StringCchPrintf( Detail2, 400, GetTxtFromMgr(5163), CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION  ); // 안정기에는...
				}
				break;
			}
		}
		break;
	default :
		return;
	}

	if( pclinfo->siStrType == RANKTYPE_CITYHALL )
	{
		//[#villagename#]마을이 [#state#]상태입니다.\r관청 대행수 존재 여부 : [#chief#]\r주주총회 진행여부 : [#meeting#]\r주식 수 : [#stock#] 주\r[상세정보]\r다음 상태까지 남은 시간 : [#next#] [#detail1#]\r
		//[#detail2#]
		Nsprintf( buffer, GetTxtFromMgr(5421), TEXT("villagename"), VillName, 
			TEXT("state"), State, TEXT("chief"), Chief,
			TEXT("meeting"), Meeting, TEXT("stock"), Stock, 
			TEXT("next"), Next, TEXT("detail1"), Detail1, 
			TEXT("detail2"), Detail2, NULL );
	}

	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NINFO_DLG ])
	{
		((cltNInfoDlg*)m_pDialog[ NINFO_DLG ])->m_pTabDlg3->ShowStrInfo(buffer);
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_INFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GeneralMeetingInfo *pclInfo = (cltGameMsgResponse_GeneralMeetingInfo *)pclMsg->cData;

	pclCityHallManager->clGeneralMeetingInfo = pclInfo->clGeneralMeetingInfo;

	pclCityHallManager->bUpdateGeneralMeetingInfo = TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_SUGGEST(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_VOTE(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_NOTICE_GENERALMEETING(cltMsg* pclMsg, SI32 id)
{
	TCHAR buf[ 512 ];

	int villageunique = (int)pclMsg->siData32[ 0 ];

	TCHAR* pTitle = GetTxtFromMgr(3465);
	TCHAR* pText = GetTxtFromMgr(2951);

	StringCchPrintf( buf, 256, pText, pclVillageManager->GetName( villageunique ) );

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buf );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETINGLETTER(cltMsg* pclMsg, SI32 id)
{

	char buf[ 512 ];

	cltGameMsgResponse_GeneralMeetingLetter *pclInfo = (cltGameMsgResponse_GeneralMeetingLetter *)pclMsg->cData;

	
	NGeneralMeetingLetterDlg *pDlg = (NGeneralMeetingLetterDlg *)m_pDialog[NGENERALMEETINGLETTER_DLG ];
	
	if( pDlg == NULL) {
		CreateInterface( NGENERALMEETINGLETTER_DLG );
		pDlg = (NGeneralMeetingLetterDlg *)m_pDialog[NGENERALMEETINGLETTER_DLG ];
	}

	pDlg->InitInfo();
	for( int i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
	{
		int villageunique = pclInfo->siVillageUnique[ i ];
		if( villageunique )
		{
			pDlg->AddInfo( villageunique );
			{

				TCHAR* pTitle = GetTxtFromMgr(3465);
				TCHAR* pText = GetTxtFromMgr(2951);

				StringCchPrintf( buf, 256, pText, pclVillageManager->GetName( villageunique ) );

				m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buf );
			}
		}
	}

	pDlg->m_curIndex = 0;
	pDlg->SetText();

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_PARTICIPATE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GeneralMeetingParticipate *pclInfo = (cltGameMsgResponse_GeneralMeetingParticipate *)pclMsg->cData;

	NGeneralMeetingLetterDlg *pDlg = (NGeneralMeetingLetterDlg *)m_pDialog[NGENERALMEETINGLETTER_DLG ];

	if( pDlg == NULL ) return;

	pDlg->DeleteDialog();

	TCHAR buf[ 256 ];

	if( pclInfo->siParticipate == 1 ) {

		TCHAR* pTitle = GetTxtFromMgr(3465);
		TCHAR* pText = GetTxtFromMgr(5840);


		StringCchPrintf( buf, 256, pText, pclVillageManager->GetName( pclInfo->siVillageUnique ) );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buf);

	}
	else {

		TCHAR* pTitle = GetTxtFromMgr(3465);//기권시 기권하였다는 메세지 출력.
		TCHAR* pText = GetTxtFromMgr(5851);
		
		StringCchPrintf( buf, 256, pText, pclVillageManager->GetName( pclInfo->siVillageUnique ) );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buf);
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHAT(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_GeneralMeetingChat *pclInfo = (cltGameMsgResponse_GeneralMeetingChat *)pclMsg->cData;


	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLUE;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_BLUE;
	MStrCpy( ChatEditData.strNameData, pclInfo->szName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, pclInfo->szText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = 0;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	NGeneralMeeting2Dlg *pDlg = (NGeneralMeeting2Dlg*)m_pDialog[ NGENERALMEETING2_DLG ];
	if( pDlg ) {
		pDlg->m_pEditChat->Insert(&ChatEditData);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHATJOIN(cltMsg* pclMsg, SI32 id)
{

	if( m_pDialog[ NGENERALMEETING2_DLG ]==NULL )
	{
		CreateInterface( NGENERALMEETING2_DLG );
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHATOUT(cltMsg* pclMsg, SI32 id)
{

}


void cltClient::DoMsg_GAMEMSG_RESPONSE_SETVILLAGEUPGRADE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SetVillageUpgrade* pclinfo = (cltGameMsgResponse_SetVillageUpgrade*)pclMsg->cData ;

	for ( SI32 i = 0 ; i < MAX_VILLAGE_NUMBER ; i ++ )
	{
		if( pclVillageManager->pclVillageInfo[i] == NULL ) continue;

		pclVillageManager->pclVillageInfo[i]->bUpgradeSwitch = false ;
	}

	for ( i = 0 ; i < 10 ; i ++ )
	{
		SI32 villageunique = pclinfo->siVillageUnique[i] ;
		if( pclVillageManager->pclVillageInfo[villageunique] == NULL ) continue;

		pclVillageManager->pclVillageInfo[villageunique]->bUpgradeSwitch = true ;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_STOCKOFFERINGINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_StockOfferingInfo* pclinfo = (cltGameMsgResponse_StockOfferingInfo*)pclMsg->cData;

	for( int i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		pclStockManager->clStockOfferingInfo[ i ].siVillageUnique = pclinfo->clStockOfferingInfo[i].siVillageUnique;
		pclStockManager->clStockOfferingInfo[ i ].siTotalOffering = pclinfo->clStockOfferingInfo[i].siTotalOffering;
		pclStockManager->clStockOfferingInfo[ i ].siStockAmount = pclinfo->clStockOfferingInfo[i].siStockAmount;
		pclStockManager->clStockOfferingInfo[ i ].siPrice = pclinfo->clStockOfferingInfo[i].siPrice;
		pclStockManager->clStockOfferingInfo[ i ].siStatus = pclinfo->clStockOfferingInfo[i].siStatus;
	}

	pclStockManager->UpdateStockOfferingInfoSwitch = TRUE;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GETSTRNOTICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_GetStrNotice* pclinfo = (cltGameMsgResponse_GetStrNotice*)pclMsg->cData;

	TCHAR buffer[256] = TEXT("");
	StringCchCopy(buffer, 256, pclinfo->szStrNotice);

	if ( m_pDialog[ NINFO_DLG ])
	{
		((cltNInfoDlg*)m_pDialog[ NINFO_DLG ])->m_pTabDlg3->ShowStrInfo(buffer);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DONATEVILLAGEMONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DonateVillageMoney* pclinfo = (cltGameMsgResponse_DonateVillageMoney*)pclMsg->cData;

	TCHAR buffer[256] = {'\0'};
	
	SI32 villageUnique = pclinfo->siVillageUnique;
	TCHAR* villageName = pclClient->pclMapManager->GetMapName(villageUnique);
		
	TCHAR* title = GetTxtFromMgr(10162);
	TCHAR* text = GetTxtFromMgr(10160);
	StringCchPrintf( buffer, 256, text, pclinfo->siDonateMoney );

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( title, buffer );

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DONATEMEMBERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DonateMemberList* pclinfo = (cltGameMsgResponse_DonateMemberList*)pclMsg->cData;

	if ( m_pDialog[ NCITYHALL_DLG ] )
	{
#ifdef _SAFE_MEMORY
		((NCityhallDonateDlg*)((NCityhallDlg*)m_pDialog[ NCITYHALL_DLG ])->m_pUserChildDlg[5])->SetDonateMemberList( &pclinfo->sDonateMemberList[0], pclinfo->siMaxPage );
#else
		((NCityhallDonateDlg*)((NCityhallDlg*)m_pDialog[ NCITYHALL_DLG ])->m_pUserChildDlg[5])->SetDonateMemberList( pclinfo->sDonateMemberList, pclinfo->siMaxPage );
#endif
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Contribution_Item_List* pclInfo = (cltGameMsgResponse_Contribution_Item_List*)pclMsg->cData;

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if( NULL == pclChar )	return;

	NContributionEvent_CommonDlg::StaticSetContributionItemList( pclInfo->m_siItemUnique, pclInfo->m_siItemNum );
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_CONTRIBUTION_INPUT_ITEM_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Contribution_Item_List_Input* pclInfo = (cltGameMsgResponse_Contribution_Item_List_Input*)pclMsg->cData;

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if( NULL == pclChar )	return;


	NContributionEvent_CommonDlg::StaticSetContributionInputItemList( pclInfo->m_siInputItemUnique, pclInfo->m_siInputItemNum );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GIANTRACCOON_RESURRECT_EVENT_START(cltMsg* pclMsg, SI32 id)
{
	m_pDialog[ NNEWNOTIFY_DLG ]->SetText( GetTxtFromMgr(40203), GetTxtFromMgr(40396) );	
	((CNNewNotifyDlg*)m_pDialog[ NNEWNOTIFY_DLG ])->SetImage( pclClient->GetUniqueFromHash( TEXT("KIND_POTRACCOON") ) );
}

