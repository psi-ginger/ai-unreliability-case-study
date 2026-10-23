
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../CommonLogic/Marriage/MarriageManager.h"
#include "../../CommonLogic/Marriage/WeddingManager.h"

#include "../../common/Order/order.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Wedding.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Wedding.h"


void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_RESERVE(cltMsg* pclMsg, SI32 id )
{
	if ( NULL ==  m_pclMarriageMgr )
	{
		return;
	}

	cltGameMsgRequest_WeddingReserve* pclinfo = (cltGameMsgRequest_WeddingReserve*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{	
		return;
	}

	// 결혼 안했다면
	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	SI32 siGetPersonID = pclChar->pclCI->GetPersonID();
	if ( 0 >= siGetPersonID )
	{
		return;
	}

	SI16 siYear				= pclinfo->m_siYear;
	SI16 siMonth			= pclinfo->m_siMonth;
	SI16 siDay				= pclinfo->m_siDay;
	SI16 siHour				= pclinfo->m_siHour;

	SI32 siReserveResult	= m_pclMarriageMgr->m_clWeddingList.CanReserve( siYear, siMonth, siDay, siHour, siGetPersonID );
	if ( WEDDING_RESERVE_RESULT_SUCCESS != siReserveResult )
	{
		return;
	}
 
	// 예약 비용
	// 기준가격에 더할 수를 뽑아온다
	SI32 siNpcRate = pclItemManager->clItemStatistic.siNPCPriceRate;
	siNpcRate = siNpcRate / 500;

	// 가격을 계산하고
	GMONEY siReserveCost = (WEDDING_RESERVE_BASIC_COST) * ( 2.5 * siNpcRate );
	if ( 0 >= siNpcRate )
	{
		siReserveCost = (WEDDING_RESERVE_BASIC_COST);
	}

	// 은행에 돈을 체크
	if ( siReserveCost > pclChar->pclCI->clBank.GetMoney() )
	{
		return;
	}

	SYSTEMTIME stReserveTime;
	ZeroMemory( &stReserveTime, sizeof(stReserveTime) );

	stReserveTime.wYear		= siYear;
	stReserveTime.wMonth	= siMonth;
	stReserveTime.wDay		= siDay;
	stReserveTime.wHour		= siHour;

	sDBRequest_WeddingReserve clMsg( siGetPersonID, siReserveCost, stReserveTime );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}
void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET(cltMsg* pclMsg, SI32 id )
{
	if ( NULL ==  m_pclMarriageMgr )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{	
		return;
	}

	cltGameMsgResponse_WeddingSimpleListGet	clSimpleList;
	cltWeddingReserveUnit*					pclSimpleUnit = NULL;

	SI16 siCurrentYear		= sTime.wYear;
	SI16 siCurrentMonth		= sTime.wMonth;
	SI16 siCurrentDay		= sTime.wDay;
	SI16 siCurrentHour		= sTime.wHour;

	SI16 siReserveStatus	= WEDDING_RESERVE_STATUS_NONE;

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		pclSimpleUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( siIndex );
		if ( NULL == pclSimpleUnit )
		{
			continue;
		}

		// 예약되어있을때 그것이 현재 시간이라면 진행중이라고 표시한다
		if ( (0 < pclSimpleUnit->m_siMate1_PersonID) || (0 < pclSimpleUnit->m_siMate2_PersonID) )
		{
			if ( TRUE == pclSimpleUnit->IsSameTime(siCurrentYear, siCurrentMonth, siCurrentDay, siCurrentHour) )
			{
				siReserveStatus = WEDDING_RESERVE_STATUS_PROGRESS;
			}
			else
			{
				siReserveStatus = WEDDING_RESERVE_STATUS_RESERVE;
			}
		}
		else
		{
			siReserveStatus = WEDDING_RESERVE_STATUS_NONE;
		}

		clSimpleList.m_clSimpleUnit[siIndex].Set( pclSimpleUnit->m_stReserveTime, siReserveStatus );
	}

	cltMsg clMsg( GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET, sizeof(clSimpleList), (BYTE*)&clSimpleList );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_WeddingQuestionAnswer* pclinfo = (cltGameMsgRequest_WeddingQuestionAnswer*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	SI32 siMyPersonID = pclChar->pclCI->GetPersonID();

	cltWeddingReserveUnit* pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtrFromNowTime();
	if ( NULL == pclUnit )
	{
		return;
	}

	if ( FALSE == pclUnit->IsReserved() )
	{
		return;
	}

	// 질문의 답을 한 사람이 예약자 중에 없다면
	if ( (siMyPersonID != pclUnit->m_siMate1_PersonID) && (siMyPersonID != pclUnit->m_siMate2_PersonID) )
	{
		return;
	}

	// 현재 상태랑 다르다면 잘 못 온것!!
	if ( pclinfo->m_siAnswerKind != m_pclMarriageMgr->GetWeddingStatus() )
	{
		return;
	}

	SI32 siChangeStatus = 0;
	switch ( pclinfo->m_siAnswerKind )
	{
	case MARRIAGE_WEDDING_STATUS_MATE1_QUESTION_WAIT:	{	siChangeStatus = MARRIAGE_WEDDING_STATUS_MATE2_QUESTION;	}	break;
	case MARRIAGE_WEDDING_STATUS_MATE2_QUESTION_WAIT:	{	siChangeStatus = MARRIAGE_WEDDING_STATUS_WEDDING_END;		}	break;
	case MARRIAGE_WEDDING_STATUS_GIVEITEM_WAIT:			{	siChangeStatus = MARRIAGE_WEDDING_STATUS_GIVEITEM;			}	break;
	default:
		{
			return;
		}
	}

	if ( 0 >= siChangeStatus )
	{
		return;
	}

	m_pclMarriageMgr->ChangeWeddingStatus( siChangeStatus );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_LIST_GET(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_WeddingListGet* pclinfo = (cltGameMsgRequest_WeddingListGet*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	if ( 0 > pclinfo->m_siRequestPage )
	{
		return;
	}

	SI32 siStartPos = (pclinfo->m_siRequestPage * WEDDING_RESERVE_LIST_PAGE_PER_NUM);
	if ( WEDDING_RESERVE_LIST_NUM <= siStartPos )
	{
		return;
	}

	cltGameMsgResponse_WeddingListGet clList;

	SI32 siCount = 0;

	for ( SI32 siIndex=siStartPos; siIndex<(siStartPos+WEDDING_RESERVE_LIST_PAGE_PER_NUM); siIndex++ )
	{
		if ( WEDDING_RESERVE_LIST_NUM <= siIndex )
		{
			break;
		}

		if ( WEDDING_RESERVE_LIST_PAGE_PER_NUM <= siCount )
		{
			break;
		}

		clList.m_clUnit[siCount].Set( m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr(siIndex) );

		siCount++;
	}

	cltMsg clMsg( GAMEMSG_RESPONSE_WEDDING_LIST_GET, sizeof(clList), (BYTE*)&clList );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_HALL_ENTER(cltMsg* pclMsg, SI32 id )
{
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	SI32					siMyPersonID	= pclChar->pclCI->GetPersonID();

	SI16					siCurrentYear	= sTime.wYear;
	SI16					siCurrentMonth	= sTime.wMonth;
	SI16					siCurrentDay	= sTime.wDay;
	SI16					siCurrentHour	= sTime.wHour;
	SI16					siCurrentMinute	= sTime.wMinute;

	cltWeddingReserveUnit*	pclUnit			= NULL;

	// 현재 진행중인 예식을 들어갈때
	if ( (0 <= siCurrentMinute) && (30 > siCurrentMinute) )
	{
		pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( siCurrentYear, siCurrentMonth, siCurrentDay, siCurrentHour );
	}
	// 다음 예약인것에 예식을 들어갈때
	else if ( (50 <= siCurrentMinute) && (60 > siCurrentMinute) )
	{
		SYSTEMTIME stAddTime;
		ZeroMemory( &stAddTime, sizeof(stAddTime) );

		stAddTime.wYear		= siCurrentYear;
		stAddTime.wMonth	= siCurrentMonth;
		stAddTime.wDay		= siCurrentDay;
		stAddTime.wHour		= siCurrentHour;

		TimeAddHour( &stAddTime, 1, &stAddTime );

		pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( stAddTime.wYear, stAddTime.wMonth, stAddTime.wDay, stAddTime.wHour );
	}

	if ( NULL == pclUnit )
	{
		return;
	}

	if ( FALSE == pclUnit->IsReserved() )
	{
		return;
	}

	// 예식자일때
	if ( (siMyPersonID == pclUnit->m_siMate1_PersonID) || (siMyPersonID == pclUnit->m_siMate2_PersonID) )
	{
		cltWarpInfo clWarpInfo( 1, MAPINDEX_WEDDINGHALL, true, false, MARRIAGE_WEDDING_COMEIN_MARRIAGE_PERSON_X, MARRIAGE_WEDDING_COMEIN_MARRIAGE_PERSON_Y, -1,-1 );

		pclChar->SetWarpInfo( &clWarpInfo, 3102 );
		pclChar->Warp();
	}
	// 하객일때
	else
	{
		cltWarpInfo clWarpInfo( 1, MAPINDEX_WEDDINGHALL, true, false, MARRIAGE_WEDDING_COMEIN_CONGRATULATOR_X, MARRIAGE_WEDDING_COMEIN_CONGRATULATOR_Y, -1,-1 );

		pclChar->SetWarpInfo( &clWarpInfo, 3103 );
		pclChar->Warp();
	}

}

void cltServer::DoMsg_GAMEMSG_REQUEST_WEDDING_RESERVE_CANCEL(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_WeddingReserveCancel* pclinfo = (cltGameMsgRequest_WeddingReserveCancel*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 너무 자주 오지 못하도록 한다
	if ( true == pclChar->CheckLastMsgClock( 1000 ) )
	{
		return;
	}

	// 결혼도 안했는데 예약을 취소할수없지
	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	SI16					siCancelYear	= pclinfo->m_siYear;
	SI16					siCancelMonth	= pclinfo->m_siMonth;
	SI16					siCancelDay		= pclinfo->m_siDay;
	SI16					siCancelHour	= pclinfo->m_siHour;

	cltWeddingReserveUnit*	pclUnit			= NULL;
	pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( siCancelYear, siCancelMonth, siCancelDay, siCancelHour );
	if ( NULL == pclUnit )
	{
		return;
	}

	if ( FALSE == pclUnit->IsReserved() )
	{
		return;
	}

	SI16 siCurrentYear	= sTime.wYear;
	SI16 siCurrentMonth	= sTime.wMonth;
	SI16 siCurrentDay	= sTime.wDay;
	SI16 siCurrentHour	= sTime.wHour;

	// 현재 시간 예약은 취소가 불가능하다
	if ( TRUE == pclUnit->IsSameTime(siCurrentYear, siCurrentMonth, siCurrentDay, siCurrentHour) )
	{
		return;
	}

	// 내가 한 예약이 아니면 안됨
	SI32 siMyPersonID = pclChar->pclCI->GetPersonID();
	if ( (siMyPersonID != pclUnit->m_siMate1_PersonID) && (siMyPersonID != pclUnit->m_siMate2_PersonID) )
	{
		return;
	}

	sDBRequest_WeddingReserveCancel clMsg( siMyPersonID, pclUnit->m_siReserveNum );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_MARRIAGEMATE_WARP(cltMsg* pclMsg, SI32 id )
{
	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 결혼도 안했는데 어딜!!
	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	// 배우자 캐릭터가 게임하고 있지도 않은데 그르면 안되
	SI32			siMateCharID	= pclCM->GetIDFromPersonID( pclChar->pclMarriage->GetMatePersonID() );
	cltCharServer*	pclMateChar		= pclCM->GetCharServer( siMateCharID );
	if ( NULL == pclMateChar )
	{
		return	;
	}

	SI32 siMateMapIndex = pclMateChar->GetMapIndex();
	SI32 siMateXPos		= pclMateChar->GetX();
	SI32 siMateYPos		= pclMateChar->GetY();

	// 유효하지 않은 맵이라면!!
	if ( (0 > siMateMapIndex) || (MAX_MAP_NUMBER <= siMateMapIndex) )
	{
		return;
	}

	// 해당 맵 지역이 없다면!!!
	if ( NULL == pclMapManager->pclMapArray[siMateMapIndex] )
	{
		return;
	}

	// 유효하지 않은 지역이라면!!
	if ( false == pclMapManager->IsValidArea(siMateMapIndex, siMateXPos, siMateYPos) )
	{
		return;
	}

	// 마을로 워프인지 체크
	bool bVillageWarp = false;
	if ( (0 < siMateMapIndex) && (CUR_VILLAGE_LIMIT >= siMateMapIndex) )
	{
		if ( true == pclMapManager->pclVillageManager->IsValidVillage(siMateMapIndex) )
		{
			bVillageWarp = true;
		}
	}

	// 이지역에서는 워프할수 있을까?
	if ( false == pclChar->CanWarpFrom() )
	{
		return;
	}

	// 해당 지역으로 워프할수있을까?
	if ( false == pclChar->CanWarpTo(siMateMapIndex) )
	{
		return;
	}

	// 그 케릭터로 실제로 워프할수있을까?
	if ( false == pclChar->CanWarpToChar(siMateCharID) )
	{
		return;
	}
	SI32 siWarpUnique	= 0;

	// 마을로 이동일 경우
	if ( bVillageWarp )
	{
		SI32 siGetMapTypeUnique = pclMapManager->pclMapArray[siMateMapIndex]->siMapUnique;
		if (0 > siGetMapTypeUnique)
		{
			return;
		}

		siWarpUnique = 2300;
	}
	else
	{
		siWarpUnique = 2200;
	}

	if ( id )
	{
		OrderStop( id, BY_COMPUTER );

		// 워프 이팩트 표시
		cltGameMsgResponse_WarpEffect clinfo( id );
		cltMsg clMsg(GAMEMSG_RESPONSE_WARPEFFECT, sizeof(clinfo), (BYTE*)&clinfo);
		pclChar->SendNeighbourMsg(&clMsg, true);

		// 실제로 워프하기
		cltWarpInfo clWarpInfo( 1, siMateMapIndex, false, true, siMateXPos, siMateYPos, -1, -1 );
		pclChar->SetWarpInfo( &clWarpInfo, siWarpUnique );
		pclChar->Warp();
	}



	

}