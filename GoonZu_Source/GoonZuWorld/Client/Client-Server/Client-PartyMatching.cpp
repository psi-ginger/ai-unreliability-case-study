// 파티매칭
#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"

#include "Char/CharClient/Char-Client.h"
#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../CommonLogic/Msg/MsgType-PartyMatching.h"

#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"
#include "../../Client/ninterface/NPartyQuest_PartyMatchingDlg/NPartyMatching_ApplytoPartyDlg.h"





// 예외상황에 대한 처리
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYMATCHINGEXCEPTION( cltMsg* pclMsg, SI32 id )
{
	cltGameResponse_PartyMatchingException* pRcvInfo = (cltGameResponse_PartyMatchingException*)pclMsg->cData;
	if ( NULL == pRcvInfo ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;

	// 예외에 대한 처리부
	if ( PARTYMATCHING_ALREADY_PARTYMEMBER & pRcvInfo->siExceptionFlags )
	{
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40782 );		// 파티에 가입되어 있는 상태에선 할수 없습니다.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

		return;		// 하나의 알림 대화창을 가지고 이용하지만 대화상자가 서로 다른걸 사용하는 예외상황일땐 return 안시켜도 됨.
	}

	if ( PARTYMATCHING_WRONG_LEVELCONDITION & pRcvInfo->siExceptionFlags )
	{
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40782 );		// 레벨 조건이 적합하지 않습니다.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

		return;
	}

	if ( PARTYMATCHING_WRONG_CHARID & pRcvInfo->siExceptionFlags )
	{
		TCHAR* pTitle = GetTxtFromMgr( 512 );		// 실패
		TCHAR* pBody = GetTxtFromMgr( 40782 );		// 해당 캐릭터를 찾을수가 없습니다.
		pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);

		return;
	}
}

// 서버에서 온 파티원 이름 정보 처리
void cltClient::DoMsg_GAMEMSG_RESPONSE_GETPARTYMEMBERNAMELIST( cltMsg* pclMsg, SI32 id )
{
	cltGameResponse_GetPartyMemberInfo* pRcvInfo = (cltGameResponse_GetPartyMemberInfo*)pclMsg->cData;
	if ( NULL == pRcvInfo ) return;

	CNApplytoParty_PartyMatching* pDlg = (CNApplytoParty_PartyMatching*)m_pDialog[NPARTY_MATCHING_BASE_DLG];
	if ( NULL == pDlg ) return;
	if ( NULL == pDlg->pMemberList ) return;

	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		stListBoxItemData item;
		item.Init();
		item.Set( 0, pRcvInfo->aPartyMemberNameList[i] );
		pDlg->pMemberList->AddItem( &item );
	}

	pDlg->pMemberList->Refresh();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYMATCHING_LEVEL( cltMsg* pclMsg, SI32 id )
{
	cltGameResponse_PartyMatchingLevel* pInfo = (cltGameResponse_PartyMatchingLevel*)pclMsg->cData;
	if ( NULL == pInfo ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;

	pClient->m_pClient_PartyMatchingMgr->MsgProcess_Response_PartyMatchingLevel( id, pInfo );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_APPLYTOPARTYJOIN( cltMsg* pclMsg, SI32 id )
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_CREATEMYPARTY( cltMsg* pclMsg, SI32 id )
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_CREATECOMPLETEPARTY( cltMsg* pclMsg, SI32 id )
{
	cltGameResponse_PartyMatchingCreate* pInfo = (cltGameResponse_PartyMatchingCreate*)pclMsg->cData;
	if ( NULL == pInfo ) return;

	cltClient* pClient = (cltClient*)pclClient;
	if ( NULL == pClient ) return;
	if ( NULL == pClient->m_pClient_PartyMatchingMgr ) return;

	pClient->m_pClient_PartyMatchingMgr->MsgProcess_Response_PartyMatchingCreate( id, pInfo->m_bResult );
}