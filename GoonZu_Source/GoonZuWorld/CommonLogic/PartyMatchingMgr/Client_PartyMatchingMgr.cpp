// 파티매칭
#include "Client_PartyMatchingMgr.h"
#include "../../commonlogic/msg/MsgType-PartyMatching.h"
#include "../../CommonLogic/PartyMatchingMgr/Defines_PartyMatchingMgr.h"
#include "../../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"

#include "../../Client/InterfaceMgr/DialogController.h"
#include "../../Client/ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_ApplytoPartyDlg.h"
#include "../../Client/ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_CreatePartyDlg.h"

#include "../../Client/Client.h"

#include "../../commonlogic/msgtype.h"
#include "../../CommonLogic/Msg/MsgType-Party.h"






CClient_PartyMatchingMgr::CClient_PartyMatchingMgr()
{
	m_PartyList.RemoveAll();

	m_siSelectedCategory = -1;
	m_siSelectedMission = -1;
}

CClient_PartyMatchingMgr::~CClient_PartyMatchingMgr()
{
	m_PartyList.RemoveAll();
}

void CClient_PartyMatchingMgr::Click_PartyMatchingNPC()
{
	cltClient* pClient = (cltClient*)pclClient;

	if(pClient)
	{
		// 파티 퀘스트 시작 대화상자 출력
		if(pClient->m_pDialog[NSELECT_QUEST_BASE_DLG] == NULL )
		{
			pClient->CreateInterface( NSELECT_QUEST_BASE_DLG );
		}
	}
}

void CClient_PartyMatchingMgr::Click_DestroyDlgButton()
{
	cltClient* pClient = (cltClient*)pclClient;

	if(pClient)
	{
		// 파티 퀘스트 시작 대화상자 출력
		if (pClient->m_pDialog[NSELECT_QUEST_BASE_DLG] != NULL)
		{
			pClient->m_pDialog[NSELECT_QUEST_BASE_DLG]->DeleteDialog();
		}
	}	
}

void CClient_PartyMatchingMgr::Click_CategoryButton(SI16 siIndex)
{
	// 예외처리
	if ( 0 > siIndex || MAX_PARTYQUEST_CATEGORY_NUM <= siIndex ) return;
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;
	cltCharClient* pclCharClient = pclClient->pclCM->GetCharClient(1);
	if ( NULL == pclCharClient ) return;
	if ( NULL == pClient->m_pPartyQuestMgr ) return;
	PartyQuest_CatecoryData* pCategoryData = NULL;
	pCategoryData = ((CPartyQuestMgr_Common*)pClient->m_pPartyQuestMgr)->GetPartyQuestCategoryData( siIndex );
	if ( NULL == pCategoryData ) return;
	//------------------------------------------------------------------------------------------------------

	// 실제 작업 처리
	const SI16 siCharLevel = pclCharClient->pclCI->clIP.GetLevel();

	if ( PARTYQUEST_KIND_PCROOM == pCategoryData->m_siKind )
	{
		if ( true != pclCharClient->pclCI->clBI.bPCRoom )
		{
			TCHAR*	pTitle	= GetTxtFromMgr(512);	// 실패
			TCHAR*	pBody	= GetTxtFromMgr(40395); // 해당 퀘스트는 PC방 유저만 수행 할 수 있는 퀘스트 입니다.
			pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

			return;
		}
	}

	if ( siCharLevel >= pCategoryData->m_siMinLevel && siCharLevel <= pCategoryData->m_siMaxLevel )
	{
		// 파티 퀘스트 시작 대화상자 출력
		if (pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG] == NULL)
		{
			pClient->CreateInterface( NPARTY_MATCHING_BASE_DLG );
		}

		// 셀렉트 퀘스트 대화상자를 닫는다.
		if (pClient->m_pDialog[NSELECT_QUEST_BASE_DLG] != NULL)
		{
			pClient->m_pDialog[NSELECT_QUEST_BASE_DLG]->DeleteDialog();
		}

		// 선택한 카테고리(퀘스트 그룹 인덱스)를 저장한다.
		((CNApplytoParty_PartyMatching*)(pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG]))->siTempCategory = siIndex;

		MsgProcess_Request_PartyMatchingList( siIndex );
	}
	else // 레벨이 맞지 않을때 
	{
		NTCHARString256 strTitle = GetTxtFromMgr(512);	// 실패
		NTCHARString256 strText = GetTxtFromMgr(40394); // 해당 퀘스트는 #level# 레벨의 유저만 수행 할 수 있는 퀘스트 입니다.
		NTCHARString32 strLevel;
		strLevel = SI16ToString( pCategoryData->m_siMinLevel );
		strLevel = strLevel + TEXT("~");
		strLevel = strLevel + ((TCHAR*)(SI16ToString( pCategoryData->m_siMaxLevel )));
		strText.Replace( TEXT("#level#"), strLevel );

		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
	}
}

void CClient_PartyMatchingMgr::Click_PartyEnterRequestButton(SI16 siSelectedIndex)
{
	// 1. 요청하려는 자신이 파티에 가입 상태인지 체크
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	cltCharClient* pCharClient = (cltCharClient*)pClient->pclCM->GetCharClient(1);
	if ( NULL == pCharClient ) return;

	SI32 siIndex = -1;
	siIndex = AlreadyExist( pCharClient->GetCharUnique() );

	if ( 0 <= siIndex )
	{
		// 파티매칭에 등록한 상태에서는 다른 파티매칭에 가입할수 없다.
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40798 );		// 파티매칭을 등록한 상태에서는 다른 파티매칭에 가입할수 없습니다.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
		return;
	}

	// 2. 서버에 해당 파티에 가입을 하고 싶다는 패킷을 보냄
	MsgProcess_Request_JoinPartyMatching( siSelectedIndex );
}

void CClient_PartyMatchingMgr::Click_WhisperButton(SI16 siSelectedIndex)
{
	if ( 0 > siSelectedIndex || m_PartyList.GetCount() <= (unsigned int)siSelectedIndex ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	if ( 0 > m_PartyList[siSelectedIndex].m_siLeaderID ) return;
	cltCharClient* pCharClient = (cltCharClient*)pClient->pclCM->GetCharClient( 1 );

	SI32 siMyCharUnique = pCharClient->GetCharUnique();
	if ( m_PartyList[siSelectedIndex].m_siLeaderID == siMyCharUnique ) return;

	if ( false == m_PartyList[siSelectedIndex].m_strLeaderName.IsEmpty() && _tcslen( m_PartyList[siSelectedIndex].m_strLeaderName ) > 0 )
	{
		pClient->RequestSecretChatByCharUnique( m_PartyList[siSelectedIndex].m_siLeaderID );
	}
}

void CClient_PartyMatchingMgr::Click_PartyMakeButton(SI16 siTmpCate)
{
	// NCREATE_PARTY_BASE_DLG
	cltClient* pClient = (cltClient*)pclClient;
	if ( 0 > siTmpCate ) return;

	if( NULL != pClient )
	{
		// 파티 퀘스트 시작 대화상자 출력
		if(pClient->m_pDialog[NCREATE_PARTY_BASE_DLG] == NULL )
		{
			pClient->CreateInterface( NCREATE_PARTY_BASE_DLG );

			((CNCreatePartyDlg*)(pClient->m_pDialog[NCREATE_PARTY_BASE_DLG]))->siTempCategory = siTmpCate;
			((CNCreatePartyDlg*)(pClient->m_pDialog[NCREATE_PARTY_BASE_DLG]))->Init();
		}
	}
}

void CClient_PartyMatchingMgr::Click_PartyMakeButton2() // 파티만들기창에서 파티 만들기.
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	CNCreatePartyDlg* pDlg = (CNCreatePartyDlg*)pClient->m_pDialog[NCREATE_PARTY_BASE_DLG];
	if ( NULL == pDlg ) return;

	CNApplytoParty_PartyMatching* pListDlg = (CNApplytoParty_PartyMatching*)pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG];
	if ( NULL == pListDlg ) return;

	CInfo_PartyMatching questInfo;

	// 파티원의 렙등 파티 멤버에 대한 조건이 맞는지 체크 한다.
	if ( false == Get_Info_PartyMemberInfo( questInfo ) )
	{
		// 조건에 맞지 않을 경우.
		cltClient* pClient = (cltClient*)pclClient;

		TCHAR* pTitle = GetTxtFromMgr(512);		// 실패
		TCHAR* pBody = GetTxtFromMgr(40426);	// 파티에 가입된 상태에서는 파티 만들기가 불가능 합니다.

		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pBody );

		return;
	}

	// 대화상자에 입력된 진행하려는 퀘스트 정보들을 얻어다 파티매칭 인포에 넣는다.
	if ( false == pDlg->Get_Info_PartyMatchingCreate( questInfo ) )
	{
		// 여기선 아무 처리도 안함
		return;
	}

	// 매칭 생성 요청 하라고 서버에 패킷 보냄
	MsgProcess_Request_CreatePartyMatching( questInfo );
}

bool CClient_PartyMatchingMgr::Get_Info_PartyMemberInfo(CInfo_PartyMatching& rInfo)
{
	// 파티 멤버에 대한 조건을 체크 하고 파티 멤버의 정보를 파티매칭 인포에 넣는다.
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return false;

	// 1. 파장이 다른 파티에 가입되어 있다면
	cltCharClient* pclCharClient = pClient->pclCM->GetCharClient(1);
	if( NULL == pclCharClient ) return false;

	// 최종적으로 파장 정보등 파티 멤버에 대한 정보를 세팅
	rInfo.m_siLeaderID = pclCharClient->GetCharUnique();
	rInfo.m_strLeaderName = pclCharClient->GetName();

	return true;
}

void CClient_PartyMatchingMgr::Set_Category(SI32 siCategory)
{
	if ( 0 > siCategory || MAX_PARTYQUEST_CATEGORY_NUM <= siCategory ) return;

	m_siSelectedCategory = siCategory;
}

void CClient_PartyMatchingMgr::Set_Mission(SI32 siMission)
{
	if ( 0 > siMission || MAX_PARTYQUEST_MISSION_NUM <= siMission ) return;

	m_siSelectedMission = siMission;
}

SI32 CClient_PartyMatchingMgr::Get_Category()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return -1;

	SI32 siCount = 0;
	siCount = m_PartyList.GetCount();

	for ( SI16 i = 0; i < siCount; ++i)
	{
		if ( 0 >= m_PartyList[i].m_siLeaderID ) continue;
		cltCharClient* pCharClient = (cltCharClient*)pClient->pclCM->GetCharClient( 1 );

		SI32 siMyCharUnique = pCharClient->GetCharUnique();
		if ( m_PartyList[i].m_siLeaderID == siMyCharUnique ) return m_PartyList[i].m_siParentQuest;
	}

	return -1;
}

SI32 CClient_PartyMatchingMgr::Get_Mission()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return -1;

	SI32 siCount = 0;
	siCount = m_PartyList.GetCount();

	for ( SI16 i = 0; i < siCount; ++i)
	{
		if ( 0 >= m_PartyList[i].m_siLeaderID ) continue;
		cltCharClient* pCharClient = (cltCharClient*)pClient->pclCM->GetCharClient( 1 );

		SI32 siMyCharUnique = pCharClient->GetCharUnique();
		if ( m_PartyList[i].m_siLeaderID == siMyCharUnique ) return m_PartyList[i].m_siChildQuest;
	}

	return -1;
}

void CClient_PartyMatchingMgr::Action()
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	// 파티매칭 리스트 대화상자의 패킷 보내는 버튼들에 대한 연타 방지
	CNApplytoParty_PartyMatching* pListDlg = (CNApplytoParty_PartyMatching*)pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG];
	if ( NULL != pListDlg )
	{
		if ( true == pListDlg->bPacketSended )
		{
			if ( pClient->CurrentClock > pListDlg->dwPacketBlockTime )
			{
				pListDlg->bPacketSended = false;
				pListDlg->pJoinRequest->Enable( true );
				pListDlg->pJoinRequest->Refresh();
			}
		}
	}
}

CInfo_PartyMatching* CClient_PartyMatchingMgr::Get_PartyMatchingInfo(SI16 siSelectedIndex)
{
	if ( 0 > siSelectedIndex || m_PartyList.GetCount() <= (unsigned int)siSelectedIndex ) return NULL;

	return &m_PartyList[siSelectedIndex];
}

SI32 CClient_PartyMatchingMgr::AlreadyExist(SI32 siMyCharUnique)
{
	unsigned int iCount = m_PartyList.GetCount();
	for ( unsigned int i = 0; i < iCount; ++i )
	{
		if ( siMyCharUnique == m_PartyList[i].m_siLeaderID )
		{
			return (SI32)i;
		}
	}

	return -1;
}


//---------------------------------------------------------------------------------------------------------
// 패킷 처리하는 함수들
//---------------------------------------------------------------------------------------------------------

//-- Request

// 새로운 파티매칭을 생성하고 싶다고 서버에 패킷 보냄
void CClient_PartyMatchingMgr::MsgProcess_Request_CreatePartyMatching(CInfo_PartyMatching& rInfo)
{
	if( false == rInfo.CheckCondition() ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	CNCreatePartyDlg* pDlg = (CNCreatePartyDlg*)pClient->m_pDialog[NCREATE_PARTY_BASE_DLG];
	if ( NULL == pDlg ) return;

	pDlg->DeleteDialog();

	if ( true == pDlg->bPacketSended ) return;

	cltGameRequest_PartyMatchingCreate clinfo(rInfo);
	cltMsg clMsg( GAMEMSG_REQUEST_CREATECOMPLETEPARTY, sizeof(clinfo), (BYTE*)&clinfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	if ( NULL != pDlg ) pDlg->bPacketSended = true;
}

// 파티에 가입하고 싶다는 요청
void CClient_PartyMatchingMgr::MsgProcess_Request_JoinPartyMatching(SI16 siSelectedIndex)
{
	// 서버에 요청
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	if ( 0 > siSelectedIndex || m_PartyList.GetCount() <= (unsigned int)siSelectedIndex ) return;

	if ( 0 > m_PartyList[siSelectedIndex].m_siLeaderID ) return;

	cltCharClient* pLeaderChar = (cltCharClient*)pClient->pclCM->CR[m_PartyList[siSelectedIndex].m_siLeaderID];
	if ( NULL == pLeaderChar ) return;

	SI32 siLeaderUnique = m_PartyList[siSelectedIndex].m_siLeaderID;
	if ( 0 >= siLeaderUnique ) return;

	cltCharClient* pMemberChar = (cltCharClient*)pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pMemberChar ) return;

	SI32 siMemberUnique = pMemberChar->GetCharUnique();
	if ( 0 >= siMemberUnique ) return;

	NTCHARString16 strMemberName( pMemberChar->GetName() );
	if ( true == strMemberName.IsEmpty() ) return;

	CNApplytoParty_PartyMatching* pDlg = (CNApplytoParty_PartyMatching*)pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG];
	if ( NULL == pDlg ) return;

	cltGameMsgRequest_Party_WantJoin clMsgInfo( siLeaderUnique, siMemberUnique, (TCHAR*)strMemberName );
	cltMsg clMsg( GAMEMSG_REQUEST_PARTY_WANTJOIN, sizeof(clMsgInfo), (BYTE*)&clMsgInfo );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	pDlg->bPacketSended = true;
	pDlg->dwPacketBlockTime = pClient->CurrentClock + 3000;
	pDlg->pJoinRequest->Enable( false );
	pDlg->pJoinRequest->Refresh();
}

//-- Response

// 파티매칭 리스트 요청에 대한 응답 패킷 처리
void CClient_PartyMatchingMgr::MsgProcess_Response_PartyMatchingLevel(SI32 siCharID, cltGameResponse_PartyMatchingLevel* pInfo)
{
	if ( NULL == pInfo ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	if ( NULL == pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG] )
	{
		pClient->CreateInterface( NPARTY_MATCHING_BASE_DLG );
	}

	CNApplytoParty_PartyMatching* pDlg = (CNApplytoParty_PartyMatching*)pClient->m_pDialog[NPARTY_MATCHING_BASE_DLG];
	if ( NULL == pDlg ) return;

	if ( true == pInfo->bNeedClearOldList )
	{
		m_PartyList.RemoveAll();
		pDlg->Clear_Controls();
	}

	for ( SI16 i = 0; i < MAX_PARTYMATCHING_LIST_SEND_SIZE; ++i )
	{
		if ( 0 >= pInfo->m_PartyMatchingList[i].m_siLeaderID ) continue;	// 파장 아이디가 0보다 작거나 같은 값이 나왔다는건 잘못된 정보라는 뜻.

		pDlg->AddListItem_PartyMatchingView( &(pInfo->m_PartyMatchingList[i]) );
		m_PartyList.PushBack( pInfo->m_PartyMatchingList[i] );
	}
}

// 파티매칭 새로 만들기 요청에 대한 응답
void CClient_PartyMatchingMgr::MsgProcess_Response_PartyMatchingCreate(SI32 siCharID, bool bResult)
{
	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	TCHAR* pTitle = NULL;
	TCHAR* pBody = NULL;

	if ( true == bResult )
	{
		pTitle = GetTxtFromMgr( 389 );		// 성공
		pBody = GetTxtFromMgr( 40780 );		// 파티 만들기가 성공적으로 이루어졌습니다.

		// 파티 생성 대화상자가 떠있을경우 닫아준다.
		if( pClient->m_pDialog[NCREATE_PARTY_BASE_DLG] != NULL )
		{
			pClient->m_pDialog[NCREATE_PARTY_BASE_DLG]->DeleteDialog();
		}
	}
	else
	{
		pTitle = GetTxtFromMgr( 512 );		// 실패
		pBody = GetTxtFromMgr( 40782 );		// 알수없는 이유로 만들기 실패했어요. 등등 //파티에 가입된 상태에서는 파티 가입 신청이 불가능합니다.
	}

	pClient->m_pDialog[NOTIFYMSG_DLG]->SetText( pTitle, pBody );
}

// 파티에 가입하고 싶다는 요청에 대한 응답 (사용 안할수도 있음)
void CClient_PartyMatchingMgr::MsgProcess_Response_PartyMatchingJoin()
{
}

// 서버로 해당 카테고리의 파티매칭 리스트를 달라고 요청한다.
void CClient_PartyMatchingMgr::MsgProcess_Request_PartyMatchingList(SI16 siCategory)
{
	if ( 0 > siCategory || MAX_PARTYQUEST_CATEGORY_NUM <= siCategory ) return;
	if ( NULL == pclClient ) return;
	if ( NULL == pclClient->pclCM ) return;
	cltCharClient* pMyChar = (cltCharClient*)pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pMyChar ) return;

	cltGameRequest_PartyMatchingLevel clinfo( pMyChar->pclCI->GetPersonID(), siCategory );
	cltMsg clMsg(GAMEMSG_REQUEST_PARTYMATCHING_LEVEL, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}