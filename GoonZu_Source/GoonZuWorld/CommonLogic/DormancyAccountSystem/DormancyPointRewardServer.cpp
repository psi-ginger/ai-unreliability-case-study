#include ".\dormancypointreward.h"

#include "../CommonLogic/msgtype.h"
#include "../CommonLogic/msg/Msgtype-DormancyAccount.h"
#include "../common/char/CharServer/Char-Server.h"
#include "../../server/server.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Dormancy.h"
#include "../MsgRval-Define.h"
#include "../NLib/NUtil.h"
#include "../common/Letter/Letter.h"

CDormancyPointRewardServer::CDormancyPointRewardServer(void) : CDormancyPointReward()
{
	m_pclServer = (cltServer *)pclClient;
}

CDormancyPointRewardServer::~CDormancyPointRewardServer(void)
{
}

// 휴면 계정 상태 변경하기 요청
BOOL	CDormancyPointRewardServer::SendDBChangeDormancyState(cltCharServer *pclServer, bool bDormancyState)
{
	if( NULL == pclServer)
		return FALSE;
	if( NULL == pclServer->pclCI)
		return FALSE;
	if( NULL == m_pclServer->pclGameDBProcess)
		return FALSE;

	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;

	// 휴면 계정 상태 설정
	sDBRequest_ChangeDormancyState clMsg(pclServer->GetID(),pclServer->pclCI->GetPersonID(), bDormancyState);
	m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return TRUE;
}

// 휴면 계정 상태 변경하기 응답
BOOL	CDormancyPointRewardServer::RecvDBChangeDormancyState(cltCharServer *pclServer, bool bDormancyState)
{
	if( NULL == pclServer)
		return FALSE;

	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;
	rclDormancyInfo.SetDormancyState(bDormancyState);

	SendClientDormancyInfo(pclServer);
	return TRUE;
}


// 포인트 아이템 구입
BOOL	CDormancyPointRewardServer::BuyPointItemRequest(SI32 siCharId , cltCharServer *pclChar, SI32 siItemUnique)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == m_pclDormancyPointItemMgr)
		return FALSE;
	// 휴면 포인트 아이템 인지 검사한다.
	CDormancyPointItem *pclPointItem = m_pclDormancyPointItemMgr->GetDormancyPointItem(siItemUnique);
	if( NULL == pclPointItem)
		return FALSE;
	if( NULL == pclChar->pclCI)
		return FALSE;
	if( NULL == m_pclServer->pclItemManager)
		return FALSE;

	const SI32	ITEMNUM = 1;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclChar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclChar->BulletUseReserveAction();


	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclChar->pclCI->clDormancyInfo;

	// 차감된 휴면 포인트
	SI32 siPoint = rclDormancyInfo.GetDormancyPoint() - pclPointItem->GetPoint();

	if( siPoint < 0)
	{
		m_pclServer->SendServerResponseMsg(0, SRVAL_FAIL_NOT_ENOUGH_POINT, 0, 0, pclChar->GetID());
		return FALSE;
	}

	// 아이템
	cltItemInfo* pclItemInfo = m_pclServer->pclItemManager->FindItemInfo(siItemUnique);
	if( NULL == pclItemInfo )
	{
		return FALSE;
	}
	cltItem *pclItem = &pclItemInfo->clItem;
	if( NULL == pclItem)
	{
		return FALSE;
	}

	SI16 brareswitch = 0;
	// 보상 아이템
	cltItem clRewardItem;
	clRewardItem.Init();

	// 아이템 사용기간
	UI32 uiUsedate = pclItem->uiDateUseDay;

	if( FALSE == m_pclServer->pclItemManager->MakeRandItemUnique(siItemUnique, &clRewardItem, 0, 0, &brareswitch,0,0,0,0,0,uiUsedate))
		return FALSE;

	clRewardItem.SetItemNum( ITEMNUM );

	// 인벤에 추가 될 수 있는 지
	SI16 siItempos;
	cltItem cltoitem;
	SI32 rtnvalue;
	if(pclChar->CanAddInv(INVMODE_ALL, &clRewardItem, &siItempos, &cltoitem, &rtnvalue) == TRUE)
	{
		sDBRequest_BuyDormancyPointItem clMsg(siCharId, pclChar->pclCI->GetPersonID(), &clRewardItem, clRewardItem.CanPile(m_pclServer->pclItemManager), siPoint, pclPointItem->GetPoint() * -1, siItempos);
		m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		m_pclServer->SendServerResponseMsg(0, SRVAL_FAIL_IS_FULLIVEN, 0, 0, pclChar->GetID());
	}

	return TRUE;
}

// 포인트 아이템 구입
BOOL	CDormancyPointRewardServer::BuyPointItemResponse(cltCharServer*	pclChar, cltItem *pclItem, SI32 siItemPos, SI32 siItemPoint)
{
	if( NULL == pclChar)
		return FALSE;
	if( NULL == pclChar->pclCI)
		return FALSE;
	if( NULL == pclItem)
		return FALSE;
	if( NULL == m_pclDormancyPointItemMgr)
		return FALSE;

	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclChar->pclCI->clDormancyInfo;

	// 1.  포인트 차감 저장
	m_PointProcess.ChangePoint(pclChar,siItemPoint);

	// 2. 인벤토리에 아이템 저장
	if( siItemPos >= 0 && siItemPos < MAX_ITEM_PER_PERSON )
	{
		// 인벤토리를 업데이트 한다. 
		pclChar->pclCI->clCharItem.clItem[siItemPos].Set(pclItem);

		// 클라이언트로 업데이트 된 정보를 보낸다. 
		cltGameMsgResponse_PersonItemInfo clinfo(siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclChar->SendNetMsg((sPacketHeader*)&clMsg);
	}

	// 로그 - 휴면 포인트로 아이템 구입
	if(m_pclServer->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// para1 => 차감 포인트
		m_pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_DORMANCY, LOGCOMMAND_INDEX_DORMANCY_BUY_ITEM, 
			0, pclChar,NULL ,0, 0, 0, pclItem, 0, 0, 0, 0, 0, 0, 
			siItemPoint, 0, 0, 0, 0, NULL, NULL);
	}

	// 3.포인트 정보 전송
	SendClientDormancyInfo(pclChar);

	return TRUE;
}

// 휴면 포인트 정보 전송
BOOL	CDormancyPointRewardServer::SendClientDormancyInfo(cltCharServer*	pclChar)
{
	if( NULL == pclChar)
		return FALSE;

	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclChar->pclCI->clDormancyInfo;

	// 포인트 정보 전송
	cltGameMsgResponse_DormancyRefreshPointInfo clInfo( rclDormancyInfo.GetDormancyPoint(), rclDormancyInfo.GetRemainedDormancyPlayTime(), rclDormancyInfo.GetDormancyState() ,rclDormancyInfo.GetStartedDate()->wYear,rclDormancyInfo.GetStartedDate()->wMonth, rclDormancyInfo.GetStartedDate()->wDay);
	cltMsg clMsg(GAMEMSG_RESPONSE_DORMANCY_REFRESHPOINTINFO, sizeof(clInfo), (BYTE*)&clInfo);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
	
	return TRUE;
}

// DB로 휴면 포인트 정보 수신
BOOL	CDormancyPointRewardServer::RecvDBDormancyInfo(cltCharServer*	pclChar, SI32 siPoint)
{
	if( NULL == pclChar)
		return FALSE;

	// 휴면 포인트 변경
	m_PointProcess.ChangePoint(pclChar, siPoint);

	return TRUE;
}

BOOL	CDormancyPointRewardServer::SendDBDormancyInfo(cltCharServer *pclServer, SI32 siDPoint)
{
	if( NULL == pclServer)
		return FALSE;
	if( NULL == pclServer->pclCI)
		return FALSE;
	if( NULL == m_pclServer->pclGameDBProcess)
		return FALSE;
	if( pclServer->GetID() <= 0)
		return FALSE;

	// 휴면 포인트 정보
	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;

	if( FALSE == rclDormancyInfo.GetDormancyState())
		return FALSE;

	// 포인트 정보 전송
	sDBRequest_SetDormancyPointInfo clMsg(pclServer->GetID(),pclServer->pclCI->GetPersonID(), siDPoint ,rclDormancyInfo.GetRemainedDormancyPlayTime());
	m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return TRUE;
}

// 포인트 계산
BOOL	CDormancyPointRewardServer::CalcPoint(cltCharServer *pclServer)
{
	if( NULL == pclServer )
		return FALSE;

	SI32 siReturnValue = CDormancyPointProcess::CALCPOINT_RESULT_CALCERROR;
	const SI32	INCREASEPOINT = 1;

	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;

	if(rclDormancyInfo.GetDormancyState() == FALSE)
	{
		return FALSE;
	}

	siReturnValue = m_PointProcess.CalcPoint(pclServer);

	if( CDormancyPointProcess::CALCPOINT_RESULT_INCREASEPLAYTIME == siReturnValue )
	{
		// 플레이 시간 증가 전송
		SendDBDormancyInfo(pclServer, 0 );
	}
	else if( CDormancyPointProcess::CALCPOINT_RESULT_INCREASEPOINT == siReturnValue)
	{
		// 포인트 증가 메시지 전송
		SendDBDormancyInfo(pclServer, INCREASEPOINT );

		if(m_pclServer->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// 로그 남기기 - 시간에 따른 포인트 1 증가
			m_pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_DORMANCY, LOGCOMMAND_INDEX_DORMANCY_INCREASE_POINT, 
				0, pclServer,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}
	}

	return TRUE;
}

// 기간이 만료인지
BOOL	CDormancyPointRewardServer::IsExpiration(cltCharServer *pclServer, _SYSTEMTIME &rclCurrentDate)
{
	if( NULL == pclServer )
		return FALSE;

	SI32 siDate = GetDateFromExpiration(pclServer, rclCurrentDate);

	if( siDate == -1)
		return FALSE;

	if( DURATIONDATE < siDate)
	{
		// 로그 - 휴면 포인트로 아이템 구입
		if(m_pclServer->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			m_pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_DORMANCY, LOGCOMMAND_INDEX_DORMANCY_BUY_ITEM, 
				0, pclServer,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}

		return TRUE;
	}

	return FALSE;
}


// 휴면 계정 종료를 경고 한다.
BOOL	CDormancyPointRewardServer::WaringExpiration(cltCharServer *pclServer)
{
	if( NULL == pclServer )
		return FALSE;

	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;

	if(rclDormancyInfo.GetDormancyState() == FALSE)
	{
		return FALSE;
	}

	_SYSTEMTIME st;
	GetLocalTime(&st);

	SI32 siDate = GetDateFromExpiration(pclServer, st);

	// 남은 날짜
	SI32 siRemainedDate = DURATIONDATE - siDate ;

	if( siDate == -1)
		return FALSE;

	if( (0 <= siRemainedDate) && (siRemainedDate <= WARING_EXPIRATION_FLAG))
	{
		// 경고 쪽지 보내기
		cltLetterMsg_DormancyWaringExpiration clletter(siRemainedDate);
		m_pclServer->SendLetterMsg(pclServer->pclCI->GetPersonID(), (cltLetterHeader*)&clletter );

		return TRUE;
	}

	return FALSE;
}

// 휴면 계정 상태를 유지한 기간
SI32	CDormancyPointRewardServer::GetDateFromExpiration(cltCharServer *pclServer, _SYSTEMTIME &rclCurrentDate)
{
	if( NULL == pclServer )
		return -1;

	CDormancyInfo &rclDormancyInfo = pclServer->pclCI->clDormancyInfo;

	if(rclDormancyInfo.GetDormancyState() == FALSE)
	{
		return -1;
	}

	// 시작일
	_SYSTEMTIME *pStartedTime = rclDormancyInfo.GetStartedDate();

	// 필요 없는 값 초기화
	rclCurrentDate.wHour = 0;
	rclCurrentDate.wMinute = 0;
	rclCurrentDate.wSecond = 0;

	// 값을 제대로 받지 못했다면
	if( 0 == pStartedTime->wYear)
		return -1;

	if( NULL == pStartedTime)
		return -1;
	
	SI32 siDate = DiffDate(&rclCurrentDate, pStartedTime);

	return siDate;
}

// 처리 할지 여부 결정
BOOL	CDormancyPointRewardServer::DeterminProcessing(SI32	siCurrentime)
{
	return m_PointProcess.DeterminProcessing(siCurrentime);
}