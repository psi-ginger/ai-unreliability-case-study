
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../CommonLogic/Marriage/MarriageManager.h"
#include "../../CommonLogic/Marriage/WeddingManager.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Wedding.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "MsgRval-Define.h"
#include "Msg/MsgType-Wedding.h"

void cltServer::DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE_LIST_GET(sPacketHeader* pPacket)
{
	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	sDBResponse_WeddingReserveListGet* pclMsg = (sDBResponse_WeddingReserveListGet*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	cltWeddingReserveUnit* pclUnit = NULL;

	SI16 siYear		= 0;
	SI16 siMonth	= 0;
	SI16 siDay		= 0;
	SI16 siHour		= 0;

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		siYear	= pclMsg->m_clReserveList[siIndex].m_stReserveTime.wYear;
		siMonth	= pclMsg->m_clReserveList[siIndex].m_stReserveTime.wMonth;
		siDay	= pclMsg->m_clReserveList[siIndex].m_stReserveTime.wDay;
		siHour	= pclMsg->m_clReserveList[siIndex].m_stReserveTime.wHour;
				
		pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( siYear, siMonth, siDay, siHour );
		if ( NULL == pclUnit )
		{
			continue;
		}

		pclUnit->Set( &pclMsg->m_clReserveList[siIndex] );
	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE(sPacketHeader* pPacket)
{
	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	sDBResponse_WeddingReserve* pclMsg = (sDBResponse_WeddingReserve*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	// 성공이 아니면다 리턴
	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI16 siYear		= pclMsg->m_stReserveTime.wYear;
	SI16 siMonth	= pclMsg->m_stReserveTime.wMonth;
	SI16 siDay		= pclMsg->m_stReserveTime.wDay;
	SI16 siHour		= pclMsg->m_stReserveTime.wHour;

	// 예약 성공하면 리스트를 설정해주고
	if ( 1 == pclMsg->m_siResult )
	{
		cltWeddingReserveUnit* pclUnit = m_pclMarriageMgr->m_clWeddingList.GetReserveUnitPtr( siYear, siMonth, siDay, siHour );
		if ( pclUnit )
		{
			pclUnit->Set( pclMsg->m_siReserveNum,	 
						pclMsg->m_siPersonID,		pclMsg->m_szPersonName, 
						pclMsg->m_siMatePersonID,	pclMsg->m_szMateName,
						pclMsg->m_stReserveTime
						);
		}
	}

	// 아이템을 지급한다
	cltItem			clGiveItem;
	SI16			siRare		= 0;
	clGiveItem.Init();

	BOOL			bMakeItem	= pclItemManager->MakeRandItemUnique( Const_Wedding_FullDress_Unique, &clGiveItem, 0, 0, &siRare);
	clGiveItem.SetItemNum( Const_Wedding_FullDress_Num );

	BOOL			bGiveItem	= FALSE;

	// 캐릭터 1에게 지급한다
	SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( pclChar )
	{
		if ( GIVEITEM_RESULT_SUCCESS == pclChar->GiveItem(Const_Wedding_FullDress_Unique, Const_Wedding_FullDress_Num, INPUTITEMPERSON_OPTION_WEDDING_FULLDRESS, 0) )
		{
			bGiveItem = TRUE;
		}

		// 돈도 변경
		pclChar->pclCI->clBank.SetMoney( pclMsg->m_gmLeftBankMoney );
	}

	if ( (FALSE == bGiveItem) && (TRUE == bMakeItem) )
	{
		SendPostOfficeItemToPerson( pclMsg->m_siPersonID, &clGiveItem, NULL);
	}

	// 캐릭터 2에게 지급한다
	bGiveItem = FALSE;

	SI32			siMateCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siMatePersonID );
	cltCharServer*	pclMateChar		= pclCM->GetCharServer( siMateCharID );
	if ( pclMateChar )
	{
		if ( GIVEITEM_RESULT_SUCCESS == pclMateChar->GiveItem(Const_Wedding_FullDress_Unique, Const_Wedding_FullDress_Num, INPUTITEMPERSON_OPTION_WEDDING_FULLDRESS, 0) )
		{
			bGiveItem = TRUE;
		}

		// 배우자에게도 성공 메시지 보내준다
		cltGameMsgResponse_WeddingReserve clinfo( pclMsg->m_siResult, siYear, siMonth, siDay, siHour, 0 );
		cltMsg clMsg( GAMEMSG_RESPONSE_WEDDING_RESERVE, sizeof(clinfo), (BYTE*)&clinfo );
		pclMateChar->SendNetMsg( (sPacketHeader*)&clMsg );
	}

	if ( (FALSE == bGiveItem) && (TRUE == bMakeItem) )
	{
		SendPostOfficeItemToPerson( pclMsg->m_siMatePersonID, &clGiveItem, NULL);
	}

	// 아래에서 부터는 캐릭터에 해당하므로 캐릭터가 없으면 리턴
	if ( NULL == pclChar )
	{
		return;
	}

	// 은행 계좌의 돈을 변경
	cltGameMsgResponse_WeddingReserve clinfo( pclMsg->m_siResult, siYear, siMonth, siDay, siHour, pclMsg->m_gmLeftBankMoney );
	cltMsg clMsg( GAMEMSG_RESPONSE_WEDDING_RESERVE, sizeof(clinfo), (BYTE*)&clinfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsg );

	if ( IsWhereServiceArea(ConstSwitch_NewLog) )
	{
		// param1 = 예약 년
		// param2 = 예약 월
		// param3 = 예약 일
		// param4 = 예약 시

		SendLog_WriteLogToDB( LOGCOMMAND_TYPE_FAMILY, LOGCOMMAND_INDEX_FAMILY_RESERVE_WEDDING, 
			0, NULL, NULL, pclMsg->m_siPersonID, pclMsg->m_gmReserveMoney, pclMsg->m_gmLeftBankMoney, NULL, 0, 0, 0, 0, 0, 0, 
			siYear, siMonth, siDay, siHour, 0, NULL, NULL);
	}
	//
	/*
	//===============================================================================================
	// 예약리스트 자체도 다시 보내준다
	//===============================================================================================
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

	cltMsg clMsgList( GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET, sizeof(clSimpleList), (BYTE*)&clSimpleList );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsgList );
	*/
}

void cltServer::DoMsg_DBMSG_RESPONSE_WEDDING_RESERVE_CANCEL(sPacketHeader* pPacket)
{
	sDBResponse_WeddingReserveCancel* pclMsg = (sDBResponse_WeddingReserveCancel*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	if ( NULL == m_pclMarriageMgr )
	{
		return;
	}

	// 해당 예약 번호의 유닛을 초기화 하고
	m_pclMarriageMgr->m_clWeddingList.InitOneUnitExecptTime( pclMsg->m_siReserveNum );

	// 캐릭터 설정 변경
    SI32			siCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclChar		= pclCM->GetCharServer( siCharID );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	// 클라이언트로 메시지 보낸다
	cltGameMsgResponse_WeddingReserveCancel clinfo( pclMsg->m_siResult );
	cltMsg clMsgList( GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL, sizeof(clinfo), (BYTE*)&clinfo );
	pclChar->SendNetMsg( (sPacketHeader*)&clMsgList );


	SI32			siMatePID	= pclChar->pclMarriage->GetMatePersonID();
	SI32			siMateID	= pclCM->GetIDFromPersonID( siMatePID );
	cltCharServer*	pclMateChar	= pclCM->GetCharServer( siMateID );
	if ( NULL == pclMateChar )
	{
		cltLetterMsg_WeddingCeremony_Cancel clletter;
		sDBRequest_PushLetter clMsg( siMatePID, (cltLetterUnit*)&clletter );
		SendDBMsg( DBSELECT_LETTER, (sPacketHeader*)&clMsg );
	}
	else
	{
		// 클라이언트로 메시지 보낸다
		cltGameMsgResponse_WeddingReserveCancel clinfo( pclMsg->m_siResult );
		cltMsg clMsgList( GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL, sizeof(clinfo), (BYTE*)&clinfo );
		pclChar->SendNetMsg( (sPacketHeader*)&clMsgList );
	}

}