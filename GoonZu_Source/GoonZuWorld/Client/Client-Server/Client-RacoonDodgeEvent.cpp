#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"
#include "../Common/RacoonDodgeEvent/RacoonDodgeEvent.h"


#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../Client/NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../Client/NInterface/NRacconDodgeEventDlg/NRacoonDodgeEventReserveListDlg.h"
//Switch_10MonthEvent_RacoonDodge
// 라쿤 이벤트 예약에 대한 답변
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_RESERVATION(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;
	cltGameMsgResponse_ReserveList* pclinfo = (cltGameMsgResponse_ReserveList*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	if (pclclient->m_pDialog[NRACOONDODGE_EVENT_RESERVELIST_DLG] == NULL)
	{
		pclclient->CreateInterface( NRACOONDODGE_EVENT_RESERVELIST_DLG );
	}

	((NRacoonDodgeEventReserveListDlg*)pclclient->m_pDialog[NRACOONDODGE_EVENT_RESERVELIST_DLG])->SetListData( pclinfo );
}
// 라쿤 이벤트 5초카운트 해라
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_WAIT5SECOND(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Remain5Seceond* pclinfo = (cltGameMsgResponse_Remain5Seceond*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	// 5초 카운트 올라간다.
	pclChar->siRacoonDodgeWaitTime = pclClient->CurrentClock  ; // 현제 서버에서 패킷받은 시간 셋팅
	pclChar->bRacoonDodgeEventCountDown = true ;

	
}
// 라쿤이벤트에 선정 되었다 할꺼니?
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_REQUEST_PLAYEVENT(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	cltGameMsgResponse_Request_PlayEvent* pclinfo = (cltGameMsgResponse_Request_PlayEvent*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	// 길문 창을 띄우고 해당 답변을 서버로 보낸다
	if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
	{
		pclclient->CreateInterface( NCOMMONMSG_DLG );
	}
	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REQUEST_RACOONDODGEEVENT_ANSWER);
	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec(10)	;	// 10초이후에 화면을 보이지 않게 한다.
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_30SECOND(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	cltGameMsgResponse_30Seceond* pclinfo = (cltGameMsgResponse_30Seceond*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	pclClient->bTimeCount = true	;
	pclClient->siTimeSet	=	pclinfo->siSecond/1000		;	// 30초..초단위로 넣습니다~
	pclClient->dwWriteSetTime	=	pclClient->CurrentClock	;	// 시간을 저장한 시간
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_ROUNDCOUNT(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	cltGameMsgResponse_RoundCount* pclinfo = (cltGameMsgResponse_RoundCount*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	pclClient->siRoundCount	= pclinfo->siRound	;
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_30SECOND_CLEAR(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	cltGameMsgResponse_30Seceond_Clear* pclinfo = (cltGameMsgResponse_30Seceond_Clear*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	pclClient->bTimeCount = false	;
	pclClient->siTimeSet	=	0		;	// 30초..초단위로 넣습니다~
	pclClient->dwWriteSetTime	=	0	;	// 시간을 저장한 시간
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR(cltMsg* pclMsg, SI32 id)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	cltGameMsgResponse_Round_Clear* pclinfo = (cltGameMsgResponse_Round_Clear*)pclMsg->cData;
	// 기본 적인 체크 
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}
	pclClient->siRoundCount	= -1	;
}