// 파티매칭
#include "NPartyMatching_ApplytoPartyDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../CommonLogic/CommonLogic.h"

#include "../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"
#include "../../Client/PartyQuestMgr/PartyQuestMgr_Client.h"

#include "../../CommonLogic/Msg/MsgType-PartyMatching.h"
#include "../../CommonLogic/MsgType.h"




extern cltCommonLogic* pclClient;

CNApplytoParty_PartyMatching::CNApplytoParty_PartyMatching( void )
{
	pPartyMatchingList = NULL;
	pMemberList = NULL;
	pStepInfoList = NULL;
	pGreeting = NULL;
	pJoinRequest = NULL;

	siTempCategory = -1;

	bPacketSended = false;
	dwPacketBlockTime = 0;
}

CNApplytoParty_PartyMatching::~CNApplytoParty_PartyMatching( void )
{

}

void CNApplytoParty_PartyMatching::Init( )
{

}

void CNApplytoParty_PartyMatching::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPartyMatchingDlg/DLG_NPartyList_Dlg.ddf" ) );
	file.CreatePopupDialog( this, NPARTY_MATCHING_BASE_DLG, TEXT( "dialog_party_matching_base" ), StaticCallBackDialogNApplytoParty_PartyMatching );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NPARTY_MATCHING_BASE_DLG_STATICTEXT_GREETINGS, this), NPARTY_MATCHING_BASE_DLG_STATICTEXT_GREETINGS, TEXT("statictext_greetings") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NPARTY_MATCHING_BASE_DLG_STATICTEXT_QUEST, this), NPARTY_MATCHING_BASE_DLG_STATICTEXT_QUEST, TEXT("statictext_quest") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NPARTY_MATCHING_BASE_DLG_STATICTEXT_PARTY_MEMBER, this), NPARTY_MATCHING_BASE_DLG_STATICTEXT_PARTY_MEMBER, TEXT("statictext_party_member") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_MATCHING_BASE_DLG_BUTTON_SENDMSG_TO_PARTYLEADER, this), NPARTY_MATCHING_BASE_DLG_BUTTON_SENDMSG_TO_PARTYLEADER, TEXT("button_sendmsg_to_partyleader") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_MATCHING_BASE_DLG_BUTTON_APPLY_TO_PARTY, this), NPARTY_MATCHING_BASE_DLG_BUTTON_APPLY_TO_PARTY, TEXT("button_apply_to_party") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_MATCHING_BASE_DLG_BUTTON_CREATE_PARTY, this), NPARTY_MATCHING_BASE_DLG_BUTTON_CREATE_PARTY, TEXT("button_create_party") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_MATCHING_BASE_DLG_BUTTON_REMOVE, this), NPARTY_MATCHING_BASE_DLG_BUTTON_REMOVE, TEXT("button_remove") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL, this), NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL, TEXT("editbox_greetings_detail") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST, this), NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST, TEXT("listbox_party_list") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME, this), NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME, TEXT("listbox_member_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST, this), NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST, TEXT("listbox_quest_list") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NPARTY_MATCHING_BASE_DLG_BUTTON_REFRESH, this), NPARTY_MATCHING_BASE_DLG_BUTTON_REFRESH, TEXT("button_refresh") );

	bPacketSended = false;
	dwPacketBlockTime = 0;

	pJoinRequest = m_InterfaceFactory.GetButton( NPARTY_MATCHING_BASE_DLG_BUTTON_APPLY_TO_PARTY );
	if ( NULL == pJoinRequest ) { DeleteDialog(); return; }

	pJoinRequest->Enable( true );

	pPartyMatchingList = (CList*)m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	pMemberList = (CList*)m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME );
	pStepInfoList = (CList*)m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST );
	pGreeting = m_InterfaceFactory.GetEdit( NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL );
	if ( NULL == pPartyMatchingList || NULL == pMemberList || NULL == pStepInfoList ) { DeleteDialog(); return; }

	SI16 siFirstColumn		= 0;
	SI16 siSecondColumn		= 0;
	SI16 siThirdColumn		= 0;
	SI16 siFourthColumn		= 0;
	SI16 siFifthColumn		= 0;

	TCHAR* pText = NULL;

	// 파티매칭 리스트 초기화
	pPartyMatchingList->SetBorder(true);
	pPartyMatchingList->SetBKColorUse(true);

	siFirstColumn		= 40;
	siThirdColumn		= 80;
	siSecondColumn		= pPartyMatchingList->GetWidth()	- (siFirstColumn + siThirdColumn);

	pText = GetTxtFromMgr(1641);		// 번호
	pPartyMatchingList->SetColumn(0, siFirstColumn, pText);
	pText = GetTxtFromMgr(40337);		// 파티명
	pPartyMatchingList->SetColumn(1, siSecondColumn, pText);
	pText = GetTxtFromMgr(40338);		// 파티장
	pPartyMatchingList->SetColumn(2, siThirdColumn, pText);

	// 파티멤버 리스트 초기화 (컨트롤의 성격이 맞지 않아 잠시 보류)
	pMemberList->SetBorder( true );
	pMemberList->SetBKColorUse( true );

	siFirstColumn		= pMemberList->GetWidth();

	pText = GetTxtFromMgr(40340);		// 파티원
	pMemberList->SetColumn(0, siFirstColumn, pText);

	// 퀘스트 스텝 정보 리스트 초기화
	pStepInfoList->SetBorder( true );
	pStepInfoList->SetBKColorUse( true );
	//pStepInfoList->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK );

	siFirstColumn		= 40;
	siSecondColumn		= 90;
	siThirdColumn		= 50;
	siFourthColumn		= 60;
	siFifthColumn		= pStepInfoList->GetWidth()	- (siFirstColumn + siSecondColumn + siThirdColumn + siFourthColumn);

	pText = GetTxtFromMgr(40341);		// 미션 번호
	pStepInfoList->SetColumn(0, siFirstColumn, pText);
	pText = GetTxtFromMgr(40785);		// 몬스터 (아이템)
	pStepInfoList->SetColumn(1, siSecondColumn, pText);
	pText = GetTxtFromMgr(1679);		// 수량
	pStepInfoList->SetColumn(2, siThirdColumn, pText);
	pText = GetTxtFromMgr(1871);		// 시간
	pStepInfoList->SetColumn(3, siFourthColumn, pText);
	pText = GetTxtFromMgr(4574);		// 보상
	pStepInfoList->SetColumn(4, siFifthColumn, pText);

	pPartyMatchingList->Refresh();
	pMemberList->Refresh();
	pStepInfoList->Refresh();
}

void CALLBACK CNApplytoParty_PartyMatching::StaticCallBackDialogNApplytoParty_PartyMatching( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNApplytoParty_PartyMatching* pThis = (CNApplytoParty_PartyMatching*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNApplytoParty_PartyMatching( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNApplytoParty_PartyMatching::CallBackDialogNApplytoParty_PartyMatching( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pClient = (cltClient*) pclClient;
	if ( NULL == pClient ) return;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;



	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

		// button
	case NPARTY_MATCHING_BASE_DLG_BUTTON_APPLY_TO_PARTY:	// 파티 가입 신청
		{
			EventProcess_ClickButton_PartyJoin();
		}
		break;
	case NPARTY_MATCHING_BASE_DLG_BUTTON_SENDMSG_TO_PARTYLEADER:
		{
			EventProcess_ClickButton_Whisper();
		}
		break;
	case NPARTY_MATCHING_BASE_DLG_BUTTON_CREATE_PARTY:
		{
			EventProcess_ClickButton_CreateNewPartyMatching();
		}
		break;

		// Listbox
	case NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				EventProcess_SelectItem_PartyMatchingList();
			}
		}
		break;
	case NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME :
		{
		}
		break;
	case NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST:
		{
		}
		break;

		// Edit
	case NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL:
		{

		}
		break;
	case NPARTY_MATCHING_BASE_DLG_BUTTON_REMOVE:
		{
			// 리스트에 내 매칭정보 삭제 요청
			cltGameRequest_RemovePartyMatching clinfo;
			cltMsg clMsg( GAMEMSG_REQUEST_REMOVEPARTYMATCHING, sizeof(clinfo), (BYTE*)&clinfo );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		}
		break;
	case NPARTY_MATCHING_BASE_DLG_BUTTON_REFRESH:
		{
			pClient->m_pClient_PartyMatchingMgr->MsgProcess_Request_PartyMatchingList( siTempCategory );
		}
		break;
	default:
		break;

	}
}



//---------------------------------------------------------------------------------------------------------
// 대화상자 컨트롤의 이벤트 처리 함수들
//---------------------------------------------------------------------------------------------------------

void CNApplytoParty_PartyMatching::EventProcess_SelectItem_PartyMatchingList()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;
	if ( NULL == pClient->m_pPartyQuestMgr ) return;

	CList* pPartyList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	CList* pStepInfoList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST );
	CList* pMemberList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME );
	CEdit* pGreeting = m_InterfaceFactory.GetEdit( NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL );
	if ( NULL == pPartyList || NULL == pStepInfoList || NULL == pMemberList || NULL == pGreeting ) return;

	pStepInfoList->Clear();
	pMemberList->Clear();
	pGreeting->Clear();

	SI16 siSelected = -1;
	siSelected = pPartyList->GetSelectedIndex();

	CInfo_PartyMatching* rInfo = pClient->m_pClient_PartyMatchingMgr->Get_PartyMatchingInfo( siSelected );
	if ( NULL == rInfo ) return;

	// 인사말
	pGreeting->SetText( rInfo->m_strHelloMessage );

	// 파티원 정보 - 서버로 패킷 보내고 처리된 결과를 받아서 처리
	cltGameRequest_GetPartyMemberInfo clSndInfo( rInfo->m_siLeaderID );
	cltMsg clSndMsg( GAMEMSG_REQUEST_GETPARTYMEMBERNAMELIST, sizeof(clSndInfo), (BYTE*)&clSndInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clSndMsg );

	// 스텝정보
	SI32 siCategory = -1;
	SI32 siMission = -1;
	siCategory = rInfo->m_siParentQuest;
	siMission = rInfo->m_siChildQuest;




	//////////////////////////////////////////////////////////////////////////
	// 이 for문은 따로 함수로 빠져야 할부분. 원래대로 였다면 퀘스트 매니저쪽에 있어서 출력을 하기위한 완성된 string배열이나 구조체 형태로 넘어왔어야 할 작업

	for ( SI16 i = 0; i < MAX_PARTYQUEST_STEP_NUM; ++i )
	{
		PartyQuest_UnitData* pUnitData = NULL;
		pUnitData = pClient->m_pPartyQuestMgr->GetPartyQuestUnitData( siCategory, siMission, i );
		if ( NULL == pUnitData ) continue;
		//if ( PARTYQUEST_TYPE_NONE >= pUnitData->m_siQuestType || PARTYQUEST_TYPE_END <= pUnitData->m_siQuestType ) continue;
		if ( 0 >= pUnitData->m_siTimeLimit ) continue;

		NTCHARString16 strStepNum( GetTxtFromMgr(40791) );		// "미션 #num#"
		NTCHARString16 strMonster;								// 몬스터
		NTCHARString16 strCount;								// 수량
		NTCHARString16 strTime( GetTxtFromMgr(40792) );			// "#minute#분"
		NTCHARString128 strReward;								// 보상

		// 스텝
		SI16 siStepNum = pUnitData->m_siQuestStep + 1;
		strStepNum.Replace( TEXT("#num#"),	SI32ToString(siStepNum) );

		// 몬스터
		if( PARTYQUEST_TYPE_HUNTING == pUnitData->m_siQuestType )
		{
			strMonster = pclClient->pclKindInfoSet->pclKI[pUnitData->m_siCondition1]->GetName();

		}
		else if( PARTYQUEST_TYPE_MAKEITEM == pUnitData->m_siQuestType )
		{
			strMonster = pclClient->pclItemManager->GetName( pUnitData->m_siCondition1 );
		}

		// 수량
		SI16 siCount = pUnitData->m_siAmount;
		strCount = SI32ToString(siCount);

		// 시간
		SI32 siTime = pUnitData->m_siTimeLimit;
		siTime /= 60;
		strTime.Replace( TEXT("#minute#"), SI32ToString(siTime) );

		// 보상
		SI32 siRewardExp = 0;
		SI32 siRewardUnique = 0;
		SI16 siRewardAmount = 0;

		// 아이템
		for ( SI08 k = 0; k < MAX_PARTYQUEST_REWARD_NUM; ++k )
		{
			siRewardUnique = pUnitData->m_siRewardItemUnique[k];
			siRewardAmount = pUnitData->m_siRewardItemAmount[k];

			if ( 0 < siRewardUnique )
			{
				if ( 0 != k )
				{
					/*SI16 siFontHeight = pStepInfoList->GetFontHeight();
					SI16 siLineHeight = pStepInfoList->GetLineHeight();
					pStepInfoList->SetLineHeight( siLineHeight + siFontHeight + 3 );

					strReward = strReward + TEXT("\r");*/

						strReward = strReward + TEXT(", ");
				}

				strReward = strReward + pclClient->pclItemManager->GetName( siRewardUnique );

				if ( 0 < siRewardAmount )
				{
					NTCHARString16 strRewardAmount( GetTxtFromMgr(40793) );		// " #num# 개"
					strRewardAmount.Replace( TEXT("#num#"), SI32ToString(siRewardAmount) );
					strReward = strReward + ((TCHAR*)strRewardAmount);
				}
			}
		}

		// 경험치
		siRewardExp = pUnitData->m_siRewardEXP;
		if ( 0 < siRewardExp )
		{
			strReward = strReward + TEXT("\n");

			strReward = GetTxtFromMgr(40794);		// "#exp# EXP" -> "2000 EXP" 와 같은 형태
			strReward.Replace( TEXT("#exp#"), SI32ToString(siRewardExp) );
		}

		// 이제 리스트 컨트롤에 데이터 추가
		stListBoxItemData item;
		item.Init();

		item.Set( 0, strStepNum );
		item.Set( 1, strMonster );
		item.Set( 2, strCount );
		item.Set( 3, strTime );
		item.Set( 4, strReward );

		pStepInfoList->AddItem( &item );

	}
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// 입력된 값들이 보이게 컨트롤 갱신
	pStepInfoList->Refresh();
	pMemberList->Refresh();
	pGreeting->Refresh();
}

void CNApplytoParty_PartyMatching::EventProcess_ClickButton_PartyJoin()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	CList* pPartyList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	if ( NULL == pPartyList ) return;

	SI16 siSelected = -1;
	siSelected = pPartyList->GetSelectedIndex();

	pClient->m_pClient_PartyMatchingMgr->Click_PartyEnterRequestButton( siSelected );
}

void CNApplytoParty_PartyMatching::EventProcess_ClickButton_Whisper()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	CList* pPartyList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	if ( NULL == pPartyList ) return;

	SI16 siSelected = -1;
	siSelected = pPartyList->GetSelectedIndex();

	pClient->m_pClient_PartyMatchingMgr->Click_WhisperButton( siSelected );
}

void CNApplytoParty_PartyMatching::EventProcess_ClickButton_CreateNewPartyMatching()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;

	cltCharClient* pMyChar = (cltCharClient*)pClient->pclCM->GetCharClient(1);
	if ( NULL == pMyChar ) return;

	SI32 siMyCharUnique = pMyChar->GetCharUnique();
	if ( 0 <= pClient->m_pClient_PartyMatchingMgr->AlreadyExist( siMyCharUnique ) )
	{
		// 이미 파티매칭에 등록이 되어 있다는 메세지 박스 띄우는것등의 처리는 여기에
		if ( NULL == pClient->m_pDialog[NOTIFYMSG_DLG] )
		{
			pClient->CreateInterface( NOTIFYMSG_DLG );
		}

		TCHAR* pTitle = GetTxtFromMgr( 512 );	// 실패
		TCHAR* pBody = GetTxtFromMgr( 40426 );	// 이미 파티매칭에 등록을 하셨습니다. (파티에 가입된 상태에서는 파티 만들기가 불가능 합니다.)

		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pBody );

		return;
	}

	pClient->m_pClient_PartyMatchingMgr->Click_PartyMakeButton( siTempCategory );
}





//---------------------------------------------------------------------------------------------------------
// 대화상자의 일반적인 인터페이스들
//---------------------------------------------------------------------------------------------------------

void CNApplytoParty_PartyMatching::AddListItem_PartyMatchingView(CInfo_PartyMatching* pInfo)
{
	// 파티매칭 리스트 컨트롤에 아이템 추가.

	CList* pList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	if ( NULL == pList ) return;
	if ( NULL == pInfo ) return;

	SI16 siCurrentPos = 0;
	siCurrentPos = pList->GetListNum();
	if ( 0 > siCurrentPos ) return;

	stListBoxItemData item;
	item.Init();

	item.Set( 0, SI16ToString(siCurrentPos+1) );
	item.Set( 1, pInfo->m_strPartyName );
	item.Set( 2, pInfo->m_strLeaderName );

	pList->AddItem( &item );
	pList->Refresh();
}

void CNApplytoParty_PartyMatching::SetData_PartyMemberView(SI32& rPartyMemberList)
{
	CList* pMemberList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_MEMBER_NAME );
	if ( NULL == pMemberList ) return;
}

void CNApplytoParty_PartyMatching::SetData_MissionView(SI32 siParentQuest, SI32 siChildQuest)
{
	CList* pStepList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_QUEST_LIST );
	if ( NULL == pStepList ) return;
}

void CNApplytoParty_PartyMatching::SetData_GreetingMessage(TCHAR* pszGreetingMessage)
{
	CEdit* pGreeting = m_InterfaceFactory.GetEdit( NPARTY_MATCHING_BASE_DLG_EDITBOX_GREETINGS_DETAIL );
	if ( NULL == pGreeting ) return;
}

void CNApplytoParty_PartyMatching::Clear_PartyMatchingList()
{
	CList* pList = m_InterfaceFactory.GetList( NPARTY_MATCHING_BASE_DLG_LISTBOX_PARTY_LIST );
	if ( NULL == pList ) return;

	pList->Clear();

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL != pClient->m_pClient_PartyMatchingMgr )
	{
		pClient->m_pClient_PartyMatchingMgr->m_PartyList.RemoveAll();
	}
}

void CNApplytoParty_PartyMatching::Clear_Controls()
{
	pPartyMatchingList->Clear();
	pMemberList->Clear();
	pStepInfoList->Clear();
	pGreeting->Clear();
	pJoinRequest->Enable( true );

	pPartyMatchingList->Refresh();
	pMemberList->Refresh();
	pStepInfoList->Refresh();
	pGreeting->Refresh();
	pJoinRequest->Refresh();
}