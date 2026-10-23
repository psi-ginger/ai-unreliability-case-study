#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"

#include "Char/CharClient/Char-Client.h"
#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../CommonLogic/Msg/MsgType-BokBulBok.h"

#include "../Client/Client_BokBulBokMgr/Client_BBBMgr.h"
	

// 게임 맵에 입장가능
void cltClient::DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTERMAP_ENABLE(cltMsg* pclMsg, SI32 id)	
{
	cltGameResponse_BBBCheckCondition* pclInfo = (cltGameResponse_BBBCheckCondition*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Warp_GameStage(id);
}

// 게임 리스트에 등록 가능
void cltClient::DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_WAITLIST_ENABLE(cltMsg* pclMsg, SI32 id)	
{
	cltGameResponse_BBBCheckCondition* pclInfo = (cltGameResponse_BBBCheckCondition*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Pop_WaitListDlg();
}

// 게임 참여 불가능
void cltClient::DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTRYREQUEST_FAIL(cltMsg* pclMsg, SI32 id)	
{
	cltGameResponse_BBBCheckCondition* pclInfo = (cltGameResponse_BBBCheckCondition*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Pop_EntryFailDlg();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_WAITLIST_ANSWER(cltMsg* pclMsg, SI32 id)
{
	cltGameResponse_BBB_WaitList_Answer* pclInfo = (cltGameResponse_BBB_WaitList_Answer*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	//TCHAR WaitList[MAX_RESERVATION_NUM][MAX_CHARACTER_NAME_LENGTH];
	//ZeroMemory( WaitList, sizeof(TCHAR) * MAX_RESERVATION_NUM * MAX_CHARACTER_NAME_LENGTH );

	//memcpy( WaitList, pclInfo->m_aCharName, sizeof(TCHAR) * MAX_RESERVATION_NUM * MAX_CHARACTER_NAME_LENGTH );

	m_pClient_BBBMgr->Pop_DetailWaitListDlg( (TCHAR**)pclInfo->m_aCharName, pclInfo->m_bResult, pclInfo->m_siTotalMoney );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_WAITLIST_CANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_WaitList_Cancel* pclInfo = (cltGameMsgResponse_BBB_WaitList_Cancel*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->RefreshWaitList( (TCHAR**)pclInfo->m_aCharName );
	m_pClient_BBBMgr->Pop_WaitListCancelResult();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_STAGESTART(cltMsg* pclMsg, SI32 id)
{
	// 매 스테이지 시작시 서버로부터 받은 패킷 처리
	cltGameMsgResponse_BBB_StageStart* pclInfo = (cltGameMsgResponse_BBB_StageStart*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->StageStart( pclInfo->m_siPersonID, pclInfo->m_siCurrentStage, pclInfo->m_dwStageStartTime );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_STAGEFAIL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_StageFail* pclInfo = (cltGameMsgResponse_BBB_StageFail*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->StageFail( pclInfo->m_siPersonID, pclInfo->m_dwStageEndTime );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_WARPMAPQUESTION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_WarpMapQuestion* pclInfo = (cltGameMsgResponse_BBB_WarpMapQuestion*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Pop_WarpMapQuestionDlg();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_NOTENOUGHMONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_NotEnoughMoney* pclInfo = (cltGameMsgResponse_BBB_NotEnoughMoney*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Pop_NotEnoughMoneyDlg();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_STAGECLEAR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_StageClear* pclInfo = (cltGameMsgResponse_BBB_StageClear*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->StageClear();
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_GM_SHOWMONSTERTYPE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_GM_ShowMonstertype* pclInfo = (cltGameMsgResponse_BBB_GM_ShowMonstertype*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->m_bGMShowMonsterType = pclInfo->m_bShow;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_10CLEARNOTICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_BBB_10ClearNotice* pclInfo = (cltGameMsgResponse_BBB_10ClearNotice*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	cltClient* pClient = (cltClient*)pclClient;
	cltCharClient* pCharClient = pClient->pclCM->GetCharClient( 1 );
	if ( NULL == pCharClient ) return;

	 if ( 0 == _tcscmp( pclInfo->szWinnerName, pCharClient->GetName() ) )
	{
		m_pClient_BBBMgr->DrawWinImage_ForMe();
	}

	m_pClient_BBBMgr->Notice_10ClearNotice( pclInfo->m_siMoney, pclInfo->szWinnerName );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_BBB_ALREADYRESERVED(cltMsg* pclMsg, SI32 id)
{
	//cltGameResponse_BBB_WaitList_Answer
	cltGameResponse_BBB_WaitList_Answer* pclInfo = (cltGameResponse_BBB_WaitList_Answer*)pclMsg->cData;
	if ( NULL == pclInfo ) return;

	m_pClient_BBBMgr->Receive_AlreadyReserved( (TCHAR**)pclInfo->m_aCharName, pclInfo->m_bResult, pclInfo->m_siTotalMoney );
}