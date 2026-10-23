#include "../../common/BokBulBokEventMgr/BokBulBokEvent.h"

#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"



// 복불복 게임 참여 가능 여부 체크
void cltServer::DoMsg_GAMEMSG_REQUEST_BBB_CHECK_PlAY_CONDITION(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_BBBCheckCondition * pclInfo = (cltGameRequest_BBBCheckCondition*) pclMsg->cData;
	if (NULL == pclInfo)		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )		return;

	// 실제 처리 부
	m_pServer_BBBMgr->AddBBBList( id );		// bbb 매니저에 요청을 한 유저의 id를 넘겨 게임 참여/대기리스트 등록/실패에 대한 처리
}

// 복불복 대기자리스트 수락에 대한 메세지 처리
void cltServer::DoMsg_GAMEMSG_REQUEST_BBB_WAITLIST_ANSWER(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_BBB_WaitList_Answer* pclinfo = (cltGameMsgRequest_BBB_WaitList_Answer*)pclMsg->cData;	
	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	
	if ( NULL == pclChar )		return;

	// 유저를 대기리스트에 등록시키고 클라에 대기자 상황을 알려줌.
	bool bResult = m_pServer_BBBMgr->Insert_List( id );

	m_pServer_BBBMgr->SendWaitListToClient( id, bResult );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BBB_WAITLIST_CANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_BBB_WaitList_Cancel* pclinfo = (cltGameMsgRequest_BBB_WaitList_Cancel*)pclMsg->cData;	
	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	
	if ( NULL == pclChar )		return;

	// 유저를 대기리스트에 삭제시키고 클라에 대기자 상황을 알려줌.
	m_pServer_BBBMgr->Remove_List( id );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BBB_WARPMAPANSWER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_BBB_WarpMapAnswer* pclinfo = (cltGameMsgRequest_BBB_WarpMapAnswer*)pclMsg->cData;	
	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	
	if ( NULL == pclChar )		return;

	// 워프 할래요? 에 대한 대답에 따라 적절한 처리 수행
	if ( true == pclinfo->m_bWantWarpMap )
	{
		m_pServer_BBBMgr->ActiveTemporaryReserve( id );		// 사전 예약 활성화
	}
	else
	{
		m_pServer_BBBMgr->CancelTemporaryReserve( id );		// 사전 예약 비활성화
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BBB_ALREADYRESERVED(cltMsg* pclMsg, SI32 id)
{
	//cltGameMsgRequest_BBB_AlreadyReserved* 
	cltGameMsgRequest_BBB_WarpMapAnswer* pclinfo = (cltGameMsgRequest_BBB_WarpMapAnswer*)pclMsg->cData;	
	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	
	if ( NULL == pclChar )		return;

	m_pServer_BBBMgr->Check_AlreadyReserved( id );
}

// [지연] : 2010신년이벤트 - 퀘스트 거절 메세지
void cltServer::DoMsg_GAMEMSG_REQUEST_2010NEWYEAREVENT_QUESTCANCEL(cltMsg* pclMsg, SI32 id)
{
	/*cltGameMsgRequest_2010NewYearEvent_QuestCancel* pclinfo = (cltGameMsgRequest_2010NewYearEvent_QuestCancel*)pclMsg->cData;	
	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	
	if ( NULL == pclChar )		return;*/


}