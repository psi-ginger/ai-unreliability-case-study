// 파티매칭
#include "NPartyMatching_CreatePartyDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../Client/PartyQuestMgr/PartyQuestMgr_Client.h"
#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"


extern cltCommonLogic* pclClient;







CNCreatePartyDlg::CNCreatePartyDlg( void )
{
	bPacketSended = false;

	siTempCategory = -1;
	siTempMission = -1;
}

CNCreatePartyDlg::~CNCreatePartyDlg( void )
{

}

void CNCreatePartyDlg::Init( )
{
	bPacketSended = false;

	//////////////////////////////////////////////////////////////////////////
	// 대화상자의 리스트 컨트롤들 초기화
	//////////////////////////////////////////////////////////////////////////
	CEdit* pEditPartyName = NULL;
	CEdit* pEditGreeting = NULL;
	pEditPartyName = m_InterfaceFactory.GetEdit( NCREATE_PARTY_BASE_DLG_EDITBOX_PARTY_NAME_DETAIL );
	pEditGreeting = m_InterfaceFactory.GetEdit( NCREATE_PARTY_BASE_DLG_EDITBOX_GREETINGS_DETAIL );
	if ( NULL == pEditPartyName || NULL == pEditGreeting ) { DeleteDialog(); return; }

	pEditPartyName->SetMaxEditTextLength( MAX_PARTYMATCHING_PARTYNAME_LENGTH );
	pEditPartyName->SetAutoScroll(TRUE);
	pEditPartyName->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pEditPartyName->Refresh();

	pEditGreeting->SetMaxEditTextLength( MAX_PARTYMATCHING_GREETING_LENGTH );
	pEditGreeting->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_WORDBREAK );
	pEditGreeting->Refresh();


	CList* pQuestList = (CList*)m_InterfaceFactory.GetList( NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST );
	CList* pStepInfoList = (CList*)m_InterfaceFactory.GetList( NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_INFO_LIST );
	if ( NULL == pQuestList || NULL == pStepInfoList ) { DeleteDialog(); return; }

	SI16 siFirstColumn		= 0;
	SI16 siSecondColumn		= 0;
	SI16 siThirdColumn		= 0;
	SI16 siFourthColumn		= 0;
	SI16 siFifthColumn		= 0;

	TCHAR* pText = NULL;

	// 파티매칭 리스트 초기화
	pQuestList->SetBorder( true );
	pQuestList->SetBKColorUse( true );

	siFirstColumn		= pQuestList->GetWidth();

	pText = GetTxtFromMgr(40785);		// 몬스터(아이템)
	pQuestList->SetColumn(0, siFirstColumn, pText);

	// 퀘스트 스텝 정보 리스트 초기화
	pStepInfoList->SetBorder( true );
	pStepInfoList->SetBKColorUse( true );
	//pStepInfoList->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK );

	siFirstColumn		= 40;
	siSecondColumn		= 90;
	siThirdColumn		= 50;
	siFourthColumn		= 65;
	siFifthColumn		= pStepInfoList->GetWidth()	- (siFirstColumn + siSecondColumn + siThirdColumn + siFourthColumn);

	pText = GetTxtFromMgr(40341);		// 미션 번호
	pStepInfoList->SetColumn(0, siFirstColumn, pText);
	pText = GetTxtFromMgr(40785);		// 몬스터 (아이템)
	pStepInfoList->SetColumn(1, siSecondColumn, pText);
	pText = GetTxtFromMgr(1679);		// 수량
	pStepInfoList->SetColumn(2, siThirdColumn, pText);
	pText = GetTxtFromMgr(1871);		// 제한시간
	pStepInfoList->SetColumn(3, siFourthColumn, pText);
	pText = GetTxtFromMgr(4574);		// 보상
	pStepInfoList->SetColumn(4, siFifthColumn, pText);
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// 초기화된 리스트 컨트롤에 값을 채워 넣음
	//////////////////////////////////////////////////////////////////////////

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;

	SI32 siCategory = siTempCategory;
	if ( 0 > siCategory || MAX_PARTYQUEST_CATEGORY_NUM <= siCategory ) return;

	// 퀘스트 리스트 초기화 (max mission 갯수만큼 무조건 돌림)
	for ( SI16 i = 0; i < MAX_PARTYQUEST_MISSION_NUM; ++i )
	{
		TCHAR szQuestName[128];
		ZeroMemory( szQuestName, sizeof(TCHAR)*128 );

		pClient->m_pPartyQuestMgr->GetPartyQuestUnitData( siCategory, i, 0, szQuestName );
		NTCHARString128 strQuestName = szQuestName;

		if ( false == strQuestName.IsEmpty() )
		{
			stListBoxItemData listitem;
			listitem.Set( i, szQuestName );
			pQuestList->AddItem( &listitem );
		}
	}

	pQuestList->Refresh();
	pStepInfoList->Refresh();
}

void CNCreatePartyDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPartyMatchingDlg/DLG_NCreateParty_Dlg.ddf" ) );
	file.CreatePopupDialog( this, NCREATE_PARTY_BASE_DLG, TEXT( "dialog_create_party_base" ), StaticCallBackDialogNCreatePartyDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NCREATE_PARTY_BASE_DLG_STATICTEXT_GREETINGS, this), NCREATE_PARTY_BASE_DLG_STATICTEXT_GREETINGS, TEXT("statictext_greetings") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NCREATE_PARTY_BASE_DLG_STATICTEXT_QUEST, this), NCREATE_PARTY_BASE_DLG_STATICTEXT_QUEST, TEXT("statictext_quest") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NCREATE_PARTY_BASE_DLG_STATICTEXT_PARTY_NAME, this), NCREATE_PARTY_BASE_DLG_STATICTEXT_PARTY_NAME, TEXT("statictext_party_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NCREATE_PARTY_BASE_DLG_STATICTEXT_SELECTQUESET, this), NCREATE_PARTY_BASE_DLG_STATICTEXT_SELECTQUESET, TEXT("statictext_selectqueset") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NCREATE_PARTY_BASE_DLG_BUTTON_CREATE_PARTY, this), NCREATE_PARTY_BASE_DLG_BUTTON_CREATE_PARTY, TEXT("button_create_party") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NCREATE_PARTY_BASE_DLG_EDITBOX_PARTY_NAME_DETAIL, this), NCREATE_PARTY_BASE_DLG_EDITBOX_PARTY_NAME_DETAIL, TEXT("editbox_party_name_detail") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NCREATE_PARTY_BASE_DLG_EDITBOX_GREETINGS_DETAIL, this), NCREATE_PARTY_BASE_DLG_EDITBOX_GREETINGS_DETAIL, TEXT("editbox_greetings_detail") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST, this), NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST, TEXT("listbox_quest_title_list") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_INFO_LIST, this), NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_INFO_LIST, TEXT("listbox_quest_info_list") );
}

void CALLBACK CNCreatePartyDlg::StaticCallBackDialogNCreatePartyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNCreatePartyDlg* pThis = (CNCreatePartyDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNCreatePartyDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNCreatePartyDlg::CallBackDialogNCreatePartyDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
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

	case NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				EventProcess_Select_QuestList();
			}
		}
		break;
	case NCREATE_PARTY_BASE_DLG_BUTTON_CREATE_PARTY:
		{
			pClient->m_pClient_PartyMatchingMgr->Click_PartyMakeButton2();
		}
		break;
	}
}


//------------------------------------------------------------------------------------------------------------
// interface
//------------------------------------------------------------------------------------------------------------

//파티 생성에 필요한 정보들을 얻는다. 입력 안된 값이 있거나 하면 false 리턴
bool CNCreatePartyDlg::Get_Info_PartyMatchingCreate(CInfo_PartyMatching& rInfo)
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return false;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return false;

	CList* pQuestList = m_InterfaceFactory.GetList( NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST );
	CEdit* pPartyName = m_InterfaceFactory.GetEdit( NCREATE_PARTY_BASE_DLG_EDITBOX_PARTY_NAME_DETAIL );
	CEdit* pGreeting = m_InterfaceFactory.GetEdit( NCREATE_PARTY_BASE_DLG_EDITBOX_GREETINGS_DETAIL );
	if ( NULL == pQuestList || NULL == pPartyName || NULL == pGreeting ) return false;

	SI32 siCategory = -1;
	SI32 siMission = -1;
	TCHAR* pszPartyName = NULL;
	TCHAR* pszGreeting = NULL;

	siCategory = siTempCategory;
	siMission = pQuestList->GetSelectedIndex();
	if ( 0 > siCategory || 0 > siMission || MAX_PARTYQUEST_CATEGORY_NUM <= siCategory || MAX_PARTYQUEST_MISSION_NUM <= siMission )
	{
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40783 );		// 퀘스트를 선택해 주세요.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pBody );

		return false;
	}

	rInfo.m_siParentQuest = siCategory;
	rInfo.m_siChildQuest = siMission;

	NTCHARString64 strPartyName = pPartyName->GetText();
	NTCHARString128 strGreeting = pGreeting->GetText();

	if ( true == strPartyName.IsEmpty() || true == strGreeting.IsEmpty() )
	{
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40784 );		// 파티명과 인사말을 모두 작성해주세요.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pBody );

		return false;
	}
 
 
	//---------------- 
	TCHAR szPartyName[MAX_PARTYMATCHING_PARTYNAME_LENGTH + 1];
	TCHAR szGreeting[MAX_PARTYMATCHING_GREETING_LENGTH + 1];
	ZeroMemory( szPartyName, sizeof(szPartyName) );
	ZeroMemory( szGreeting, sizeof(szPartyName) );

	StringCchCopy( szPartyName, MAX_PARTYMATCHING_PARTYNAME_LENGTH, strPartyName );
	StringCchCopy( szGreeting, MAX_PARTYMATCHING_GREETING_LENGTH, strGreeting );

	szPartyName[MAX_PARTYMATCHING_PARTYNAME_LENGTH] = TEXT('\0');		// 복사한 이름의 맨 마지막에 강제로 널문자 넣어서 오류 방지
	szGreeting[MAX_PARTYMATCHING_GREETING_LENGTH] = TEXT('\0');

	// 욕설 필터링
	pClient->m_pCAbuseFilter->CheckAbuse( szPartyName );
	pClient->m_pCAbuseFilter->CheckAbuse( szGreeting );

	// 정상적으로 예외처리 끝났으면 파티매칭 인포에 정보를 담는다.
	rInfo.m_strPartyName = szPartyName;
	rInfo.m_strHelloMessage = szGreeting;
	//----------------------------------------------------------


	// 정상적으로 예외처리 끝났으면 파티매칭 인포에 정보를 담는다.
	//rInfo.m_strPartyName = strPartyName;
	//rInfo.m_strHelloMessage = strGreeting;
	//----------------------------------------------------------

	return true;
}


//------------------------------------------------------------------------------------------------------------
// event process
//------------------------------------------------------------------------------------------------------------

// 퀘스트 리스트에서 특정 퀘스트를 선택했다. (미션을 골랐다)
void CNCreatePartyDlg::EventProcess_Select_QuestList()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	CList* pList = NULL;
	pList = m_InterfaceFactory.GetList( NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_TITLE_LIST );
	if ( NULL == pList ) return;

	// 선택한 미션의 스텝 정보를 대화상자 하단 리스트 컨트롤에 출력해준다.
	if ( NULL == pClient->m_pPartyQuestMgr ) return;

	CList* pStepInfoList = m_InterfaceFactory.GetList( NCREATE_PARTY_BASE_DLG_LISTBOX_QUEST_INFO_LIST );
	if ( NULL == pStepInfoList ) return;

	pStepInfoList->Clear();

	// 스텝정보
	SI32 siCategory = -1;
	SI32 siMission = -1;
	siCategory = siTempCategory;
	siMission = pList->GetSelectedIndex();
	if ( 0 > siCategory || 0 > siMission ) return;




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
}