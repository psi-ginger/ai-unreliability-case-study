//---------------------------------
// 이강형 2007/04/19
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"
#include "..\Server\Server.h"

#include "../common/Event/MofuMofu/MofuMofu.h"

//-------------------------------------
// Common
//-------------------------------------
#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-MofuMofuEvent.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-CityHall.h"
#include "MsgRval-Define.h"
#include "Msg\MsgType-MofuMofuEvent.h"

void cltServer::DoMsg_DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVE(sPacketHeader* pPacket)
{
	sDBResponse_MofuMofuEvent_Reserve* pclMsg = (sDBResponse_MofuMofuEvent_Reserve*)pPacket;
	if(pclMsg == NULL)					return;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)	return ;

	if(pclMsg->siResult == 1)
	{
		cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		if( pclStrInfo != NULL )
		{
			pclStrInfo->clCommonInfo.clMoney.itMoney  = pclMsg->siCityHallMoney;

			cltMoney clMoney;
			clMoney.Set( pclMsg->EventCost * -1 );

			cltMoney clCityHallMoney;
			clCityHallMoney.Set( pclMsg->siCityHallMoney );

			pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
				INOUTCODE_EVENTPAY, &pclTime->clDate, &clMoney, &clCityHallMoney);

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique , RANKTYPE_CITYHALL, 
				INOUTCODE_EVENTPAY, &pclTime->clDate, &clMoney, &clCityHallMoney );
			
			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
			//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
			//	TCHAR pszCharParam1[50] = {'\0', };
			//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
			//																pclTime->clDate.uiYear,
			//																pclTime->clDate.uiMonth,
			//																pclTime->clDate.uiDay,
			//																pclTime->clDate.uiHour,
			//																pclTime->clDate.uiMinute);
			//	
			//	cltServer* pclserver = (cltServer*)pclClient;
			//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
			//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			//									clMoney.itMoney, clCityHallMoney.itMoney, 0, 0, 0, 
			//									pszCharParam1, NULL);
			//}
		}
	}
	else
	{
		// 이벤트 예약에 실패하였습니다.
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_MOFUMOFUEVENT_FAIL_RESERVE,  0, 0, pclCM->CR[id]->GetCharUnique() );

		pclClient->pclMofuMofuEvent->SetRoom(pclMsg->stReserveHour.wYear, pclMsg->stReserveHour.wMonth, pclMsg->stReserveHour.wDay, pclMsg->stReserveHour.wHour, 0);
	}

	cltGameMsgResponse_MofuMofuEvent_Reserve		clInfo(pclMsg->siResult, pclMsg->stReserveHour.wYear, pclMsg->stReserveHour.wMonth, pclMsg->stReserveHour.wDay, pclMsg->stReserveHour.wHour);
	cltMsg clMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVE, sizeof(cltGameMsgResponse_MofuMofuEvent_Reserve), (BYTE*)&clInfo);
	pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 15] // 모후모후 이벤트 예약.
		// param1 == 번호, 
		// param2 == 이벤트 비용, 
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MOFUMOFU_EVENT, LOGCOMMAND_INDEX_MOFUMOFU_EVENT_ADD_RESERVE, 
										0, (cltCharServer*)pclCM->CR[pclMsg->usCharID], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->stReserveHour.wHour, pclMsg->EventCost, 0, 0, 0, 
										NULL, NULL);
	}

	cltGameMsgResponse_MofuMofuEvent_ReserveList	clListInfo;

	// 예약 후 리스트 재전송
	for(SI32 i=0; i<MOFUMOFUEVENT_ROOMNUM; i++)
	{
		cltMofuMofuEventReserveUnit* pclRoom = pclMofuMofuEvent->GetRoom(i);
		if(pclRoom == NULL)				continue;

		clListInfo.m_Reserved[i].Set(pclRoom);
	}

	clListInfo.m_siCount = MOFUMOFUEVENT_ROOMNUM;

	SI32 siSize = clListInfo.GetSize();
	cltMsg clListMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST, siSize, (BYTE*)&clListInfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clListMsg);

}

void cltServer::DoMsg_DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST( sPacketHeader* pPacket )
{
	sDBResponse_MofuMofuEvent_ReserveList* pclMsg = (sDBResponse_MofuMofuEvent_ReserveList*)pPacket;
	if(pclMsg == NULL)					return;

	for(SI32 i=0; i<MOFUMOFUEVENT_ROOMNUM; i++)
	{
		pclClient->pclMofuMofuEvent->SetRoom(pclMsg->ReserveRooms[i].m_ReserveHour.wYear, pclMsg->ReserveRooms[i].m_ReserveHour.wMonth, pclMsg->ReserveRooms[i].m_ReserveHour.wDay, pclMsg->ReserveRooms[i].m_ReserveHour.wHour, pclMsg->ReserveRooms[i].m_siVillageUnique);
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_MofuMofuEvent_ReserveList"));
}
