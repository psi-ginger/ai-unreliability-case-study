//Switch_10MonthEvent_RacoonDodge
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/RacoonDodgeEvent/RacoonDodgeEvent.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-RacoonDodgeEvent.h"

// 라쿤  피하기 이벤트 예약 
void cltServer::DoMsg_GAMEMSG_REQUEST_RACOONDODGEEVENTRESERVATION(cltMsg* pclMsg, SI32 id )//Switch_10MonthEvent_RacoonDodge
{
	cltServer* pclserver = (cltServer*)pclClient;

	if( false == pclClient->IsCountrySwitch( Switch_10MonthEvent_RacoonDodge ) )	return ;	// 국가 코드가 안열렸으면 리턴 한다
	// 이벤트 기간이 아니면 리턴 한다. -> 미추가
	cltGameMsgRequest_ReservationEvent* pclinfo = (cltGameMsgRequest_ReservationEvent*)pclMsg->cData;	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	if ( NULL == pclChar )		return;
	
	SI32 siCharPersonID = pclinfo->siPersonID	;
	bool bSuccess = false ;

	// 내 소지금을 확인 한다 .
	GMONEY gMyMoney = pclChar->pclCI->clIP.GetMoney()	;
	GMONEY gCost	=	(pclClient->pclItemManager->clItemStatistic.siNPCPriceRate*1000)/100	;

	if ( gMyMoney >= gCost )
	{
		bSuccess = pclRacoonDodgeEventMgr->AddReserveList(siCharPersonID)	;
		if ( bSuccess == true ) 
		{
			//	돈을사용하고 디비로 알린다.
			sDBRequest_ChangeMoney clMsg(pclChar->GetID(), pclChar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_RACOONDODGE_USEMONEY, -gCost);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 손서웅 2009. 8. 24] // 라쿤이벤트 참가비 지불 
				// 타켓 personID 
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RACCOONDODGE_EVENT, LOGCOMMAND_INDEX_PVP_RACCOONDODGE_EVENT_RESERVE, 
					0, pclChar,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					gCost, 0, 0, 0, 0, NULL, NULL);	// para1 : 참가비
			}
			pclRacoonDodgeEventMgr->SendReserveList( siCharPersonID )	;	// 리스트를 클라이언트로 알려준다.
		}
	}
	else
	{
		pclserver->SendServerResponseMsg( 0, SRVAL_RACOONDODGEEVENT_NOTENOUGHMONEY , 0, 0, pclChar->GetCharUnique() );	// 빈자리가 없다
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_REQUEST_PLAYEVENT(cltMsg* pclMsg, SI32 id )//Switch_10MonthEvent_RacoonDodge	// 라쿤 피하기 참여 질문에대한 수락 
{
	if( false == pclClient->IsCountrySwitch( Switch_10MonthEvent_RacoonDodge ) )	return ;	// 국가 코드가 안열렸으면 리턴 한다
	// 이벤트 기간이 아니면 리턴 한다. -> 미추가
	cltGameMsgReQuest_Request_PlayEvent* pclinfo = (cltGameMsgReQuest_Request_PlayEvent*)pclMsg->cData;	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	if ( NULL == pclChar )		return;

	SI32 siCharPersonID = pclinfo->siPersonID	;
	bool bSuccess = false ;

	pclRacoonDodgeEventMgr->WarpGameSpace(siCharPersonID)	;	//수락했으니 지정된 맵으로 워프 시킨다.
}


void cltServer::DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_OVER5SECOND_START(cltMsg* pclMsg, SI32 id )//Switch_10MonthEvent_RacoonDodge
{
	if( false == pclClient->IsCountrySwitch( Switch_10MonthEvent_RacoonDodge ) )	return ;	// 국가 코드가 안열렸으면 리턴 한다

	cltGameMsgRequest_RacoonDodgeEveent_StartGame* pclinfo = (cltGameMsgRequest_RacoonDodgeEveent_StartGame*)pclMsg->cData;	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	if ( NULL == pclChar )		return;

	SI32 siEventStep = -1	;

	// 현제 게임 스탭을 받아온다
	siEventStep = pclRacoonDodgeEventMgr->GetStep()	;
	if ( siEventStep == EVENTSTEP_WAITTIME )	// 5초 대기하는 상태이면
	{
		pclRacoonDodgeEventMgr->ChangeStep( EVENTSTEP_PLAY ) ; // 이벤트를 시작한다.
	}
	else
	{
		// 이미 상태가 변경 됐던가 정상적인 상황에서 날라온 메세지가 아니다 처리 하지말고 리턴.
		return ; 
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_RACOONDODGE_EVENT_GET_RESERVE_LIST(cltMsg* pclMsg, SI32 id )//Switch_10MonthEvent_RacoonDodge
{
	if( false == pclClient->IsCountrySwitch( Switch_10MonthEvent_RacoonDodge ) )	return ;	// 국가 코드가 안열렸으면 리턴 한다
	// 이벤트 기간이 아니면 리턴 한다. -> 미추가
	cltGameMsgRequest_ReserveList* pclinfo = (cltGameMsgRequest_ReserveList*)pclMsg->cData;	if ( NULL == pclinfo )		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );	if ( NULL == pclChar )		return;

	SI32 siCharPersonID = pclinfo->siPersonID	;

	pclRacoonDodgeEventMgr->SendReserveList( siCharPersonID )	;	// 리스트를 클라이언트로 알려준다.

}