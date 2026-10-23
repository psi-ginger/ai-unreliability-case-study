#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Dormancy.h"

#include "../../CommonLogic/Msg/MsgType-DormancyAccount.h"
			
// [기형] 포인트로 아이템 구입
void cltServer::DoMsg_DBMSG_RESPONSE_DORMANCY_POINTITEM_BUY(sPacketHeader* pPacket)
{
	if( NULL == pclDormancySystem)
		return;

	const SI32	DBMSG_SUCCESS_FLAG = 1;

	sDBResonse_BuyDormancyPointItem* pclMsg = (sDBResonse_BuyDormancyPointItem*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	// DB처리 결과 여부 확인
	if ( DBMSG_SUCCESS_FLAG != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	CDormancyPointRewardServer *pclDormancyServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
	if(NULL == pclDormancyServer)
		return;

	pclDormancyServer->BuyPointItemResponse(pclChar, &pclMsg->m_clItem, pclMsg->m_siItemPos, pclMsg->m_siItemPoint);
}

// [기형] 포인트 관련 정보 저장
void cltServer::DoMsg_DBMSG_RESPONSE_DORMANCY_POINTINFO_SET(sPacketHeader* pPacket)
{
	if( NULL == pclDormancySystem)
		return;

	const SI32	DBMSG_SUCCESS_FLAG = 1;

	sDBResonse_SetDormancyPointInfo* pclMsg = (sDBResonse_SetDormancyPointInfo*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	// DB처리 결과 여부 확인
	if ( DBMSG_SUCCESS_FLAG != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	CDormancyPointRewardServer *pclDormancyServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
	if(NULL == pclDormancyServer)
		return;

	pclDormancyServer->RecvDBDormancyInfo(pclChar, pclMsg->m_siPoint);
	pclDormancyServer->SendClientDormancyInfo(pclChar);
}

// [기형] 휴면 계정 상태 변경
void cltServer::DoMsg_DBMSG_RESPONSE_DORMANCY_STATE_CHANGE(sPacketHeader* pPacket)
{
	if( NULL == pclDormancySystem)
		return;

	const SI32	DBMSG_SUCCESS_FLAG = 1;

	sDBResponse_ChangeDormancyState* pclMsg = (sDBResponse_ChangeDormancyState*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	// DB처리 결과 여부 확인
	if ( DBMSG_SUCCESS_FLAG != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	CDormancyPointRewardServer *pclDormancyServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
	if(NULL == pclDormancyServer)
		return;

	pclDormancyServer->RecvDBChangeDormancyState(pclChar ,pclMsg->m_bDormancyState);
}


