//---------------------------------
// 2003/9/2 김태곤
//---------------------------------

#include "../Common/CommonHeader.h"
#include "../Server/Server.h"

#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "../../Common/Map/Map.h"
#include "Char/CharManager/CharManager.h"

#include "../Server/Candidate/Candidate.h"

#include "../../../Server/AuthServer/AuthMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "MsgRval-Define.h"
#include "Msg/MsgType-Village.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"


#include "../../CommonLogic/Cityhall/Cityhall.h"
#include "../Common/StockDistributionMgr/StockDistributionMgr.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Stock.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"


// 마을 건물 정보 요청. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETSTRUCTUREINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetStructureInfo* pclMsg = (sDBResponse_GetStructureInfo*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 마을 건물 정보를 업데이트 한다.
	SI32 villageunique = pclMsg->siVillageUnique;
	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER )		return;

	SI32 i;
	for(i = 0;i < MAX_CONSTRUCTION_NUM;i++)
	{
		pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clStructureInfo.Set(pclMsg->siVillageStructureInfo[i], i);
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetStructureInfo"));
		pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clStructureInfo.Set(pclMsg->siVillageStructureInfo[i], i);"));

	}
	
	//---------------------------
	// 마을의 지도를 업데이트 한다. 
	//---------------------------
	SI32 mapindex = pclMapManager->GetMapIndexFromVillageUnique(villageunique);
	if(mapindex >= 0)
	{
		pclMapManager->MakeLastMap(mapindex);
	}
	

}

// 마을 정보 응답
void cltServer::DoMsg_DBMSG_RESPONSE_GETVILLAGEINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetVillageInfo* pclMsg = (sDBResponse_GetVillageInfo *)pPacket;
	
	//if( pclMsg->siVillageUnique == 0 ) return;
	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	cltVillageVary *pVillVary = &(pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ]->clVillageVary);

	pVillVary->siMasterVillage = pclMsg->siMasterVillage;
	pVillVary->siParentVillage = pclMsg->siParentVillage;
	pVillVary->siVillageMark = pclMsg->siVillageMark;

	pVillVary->siTotalVillageScore = pclMsg->siTotalVillageScore;
	pVillVary->siNewYearEventScore = pclMsg->siNewYearEventScore;
	
	//[추가 : 황진성 2008. 2. 21 => 주주총회 발동 플레그 초기화]
	pVillVary->m_bGeneralMeetingSuggest = pclMsg->m_bGeneralMeetingSuggest;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

	pStrInfo->clCityhallStrInfo.siMasterVillage = pclMsg->siMasterVillage;
	pStrInfo->clCityhallStrInfo.siParentVillage = pclMsg->siParentVillage;
	pStrInfo->clCityhallStrInfo.siTotalVillageScore = pclMsg->siTotalVillageScore;
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetVillageInfo"));

}

// 선전포고 
void cltServer::DoMsg_DBMSG_RESPONSE_DECLAREWAR( sPacketHeader* pPacket )
{
	SI32 returnval = 0;

	sDBResponse_DeclareWar* pclMsg = (sDBResponse_DeclareWar *)pPacket;
	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siTargetVillage < 0 || pclMsg->siTargetVillage >= MAX_VILLAGE_NUMBER )		return;
	
	if( pclMsg->siResult == 1 ) {

		cltStrInfo *pStrInfo;

		pStrInfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

		if( pStrInfo ) {
			pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ pclMsg->siTargetVillage ] = 1;
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateYear = pclMsg->clDate.m_uiYear ;
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateMonth = pclMsg->clDate.m_uiMonth ;
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateDay = pclMsg->clDate.m_uiDay ;
			
			pclVillageManager->clDeclareWarDate[pclMsg->siVillageUnique].Set(&pclMsg->clDate);
		}

		pStrInfo = pclCityHallManager->GetStrInfo( pclMsg->siTargetVillage );
		
		if( pStrInfo ) pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ pclMsg->siVillageUnique ] = 2;
		
		pclVillageManager->siDeclareWarVillage[ pclMsg->siVillageUnique ] = pclMsg->siTargetVillage;

		returnval = SRVAL_CITYHALL_SUCCESS_DECLAREWAR;
		
		// 접속한 모든 클라이언트에게 알린다
		cltGameMsgResponse_DeclareWar sendMsg;
		
		sendMsg.siVillageUnique = pclMsg->siVillageUnique;
		sendMsg.siTargetVillage = pclMsg->siTargetVillage;
		sendMsg.clDate.Set( &pclMsg->clDate);

		cltMsg clMsg( GAMEMSG_RESPONSE_DECLAREWAR, sizeof( cltGameMsgResponse_DeclareWar ), (BYTE*)&sendMsg );
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg );

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)
		{
			return ;
		}
		else
		{
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// TEMP LEEKH -------------------------------------------------------------------
				// [로그추가 : 황진성 2007. 10. 18] // 마을 전쟁 선포.
				// param1		 == 선포 당하는 마을, 
				// pszCharParam1 == 시간. 
				TCHAR str[50];
				ZeroMemory(str, sizeof(str));
				StringCchPrintf(str, sizeof(str), TEXT("%d-%d-%d"),	
														pclMsg->clDate.m_uiYear,
														pclMsg->clDate.m_uiMonth,
														pclMsg->clDate.m_uiDay);


				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_DECLARE_WAR, 
									 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									 pclMsg->siTargetVillage, 0, 0, 0, 0, str, NULL);
				// TEMP LEEKH -------------------------------------------------------------------
			}
		}
			
	} 
	else if(pclMsg->siResult == -1 ) 
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_TARGET;

	}
	else if(pclMsg->siResult == -2 ) 
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_MADEPROTECT_SELF;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}


}

// 선전포고 취소 
void cltServer::DoMsg_DBMSG_RESPONSE_CANCELDECLAREWAR( sPacketHeader* pPacket )
{
	SI32 returnval = 0;

	sDBResponse_CancelDeclareWar *pclMsg = (sDBResponse_CancelDeclareWar *)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;
	if( pclMsg->siTargetVillage < 0 || pclMsg->siTargetVillage >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 1 ) {
		
		cltStrInfo *pStrInfo;

		pStrInfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

		if( pStrInfo ) {
			pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ pclMsg->siTargetVillage ] = 0;
			//pStrInfo->clCityhallStrInfo.clDeclareWarDate.Set( &pclMsg->clDate );
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateYear = pclMsg->clDate.m_uiYear;
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateMonth = pclMsg->clDate.m_uiMonth;
			pStrInfo->clCityhallStrInfo.uiDeclareWarDateDay = pclMsg->clDate.m_uiDay;
		}

		pStrInfo = pclCityHallManager->GetStrInfo( pclMsg->siTargetVillage );
		
		if( pStrInfo ) pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ pclMsg->siVillageUnique ] = 0;

		pclVillageManager->siDeclareWarVillage[ pclMsg->siVillageUnique ] = 0;
		pclVillageManager->siNowWarVillage[ pclMsg->siVillageUnique ] = 0;

		returnval = SRVAL_CITYHALL_SUCCESS_CANCELDECLAREWAR;

		// 접속한 모든 클라이언트에게 알린다
		cltGameMsgResponse_CancelDeclareWar sendMsg;
		
		sendMsg.siVillageUnique = pclMsg->siVillageUnique;
		sendMsg.siTargetVillage = pclMsg->siTargetVillage;

		cltMsg clMsg( GAMEMSG_RESPONSE_CANCELDECLAREWAR, sizeof( cltGameMsgResponse_CancelDeclareWar ), (BYTE*)&sendMsg );
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg );

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)
		{
			return ;
		}
		else
		{
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 16] // 마을 전쟁 선포 취소.
				// param1 == 선포 취소 당하는 마을, 
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_DECLARE_WAR_CANCEL, 
												0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
												pclMsg->siTargetVillage, 0, 0, 0, 0, 
												NULL, NULL);
			}
		}

	} else {
		returnval = SRVAL_CITYHALL_FAIL_CANCELDECLAREWAR;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_SETPARENTVILLAGE( sPacketHeader* pPacket )
{
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETSYMBOLSTATUS( sPacketHeader* pPacket )
{
	SI32 returnval = 0;
		
	sDBResponse_SetSymbolStatus *pclMsg = (sDBResponse_SetSymbolStatus *)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 1 ) 
	{
		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		switch( pclMsg->siStatKind )
		{
		case SYMBOLSTAT_KIND:
			pStrInfo->clCityhallStrInfo.siSymbolKind = pclMsg->siStatValue;
			pclVillageManager->ChangeVillageGod( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;

		case SYMBOLSTAT_LIFE:
			pStrInfo->clCityhallStrInfo.siSymbolLife = pclMsg->siStatValue;
			pclVillageManager->SetLifeOfGodInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;

		case SYMBOLSTAT_MANA:
			pStrInfo->clCityhallStrInfo.siSymbolMana = pclMsg->siStatValue;
			pclVillageManager->SetManaOfGodInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;

		case SYMBOLSTAT_CAPA:
			pStrInfo->clCityhallStrInfo.siSymbolCapa = pclMsg->siStatValue;
			pclVillageManager->SetCapaOfGodInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;
		}
		
		SI32 siVillageGodCharUnique;
		if( pclVillageManager->IsThereVillageGod(pclMsg->siVillageUnique, &siVillageGodCharUnique ) == TRUE )
		{
			if( pclCM->IsValidID(siVillageGodCharUnique))
				pclCM->CR[ siVillageGodCharUnique ]->ParametaBoxAction();
		}

		// 상단 자금 갱신
		pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
		
		// 상단 최근 갱신일 수정 
		pStrInfo->clCityhallStrInfo.clLastSymbolUpdate.Set( &pclMsg->clDate );

		// 상단 입출금 내역에 추가한다			
		{
			cltMoney clMoney;
			clMoney.Set( -pclMsg->siCost );

			pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
				INOUTCODE_GODSTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
				INOUTCODE_GODSTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		
			// 게임에서 빠져 나간돈 갱신 
			clMoney.Set( -clMoney.itMoney );
			clCurrencyInfo.clOutputMoney_clVillageWar.Increase( &clMoney );
		}		

		// 대행수에게 통보
		
		cltGameMsgResponse_SetSymbolStatus sendMsg;

		sendMsg.siVillageUnique = pclMsg->siVillageUnique;
		sendMsg.siCost = pclMsg->siCost;
		sendMsg.siStatKind = pclMsg->siStatKind;
		sendMsg.siStatValue = pclMsg->siStatValue;

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)return;

		cltMsg clNormalMsg( GAMEMSG_RESPONSE_SETSYMBOLSTATUS, sizeof( cltGameMsgResponse_SetSymbolStatus ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 17] // 성황신 설정.
			// param1 == 스탯 종류, 
			// param2 == 스탯 수치,
			// param3 == 지불 비용.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_SYMBOL_STATUS_SET, 
				0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siStatKind, pclMsg->siStatValue, pclMsg->siCost, 0, 0, NULL, NULL);
		}

//		SI32 id	= pclMsg->usCharID;
//		if(pclCM->IsValidID(id) == FALSE)return;

//		UpdateStrInfoToClient( id, RANKTYPE_CITYHALL );

	} else if( pclMsg->siResult == 2) {
		// 보유자금 부족 
		returnval = SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_MONEY;
	}

	
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}	


void cltServer::DoMsg_DBMSG_RESPONSE_SETCASTLESTATUS( sPacketHeader* pPacket )
{
	SI32 returnval = 0;
		
	sDBResponse_SetCastleStatus *pclMsg = (sDBResponse_SetCastleStatus *)pPacket;

	if( pclMsg->siResult == 1 ) {

		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		switch( pclMsg->siStatKind )
		{
		case SYMBOLSTAT_KIND:
			/*
			pStrInfo->clCityhallStrInfo.siCastleKind = pclMsg->siStatValue;
			pclVillageManager->ChangeVillageGod( pclMsg->siVillageUnique, pclMsg->siStatValue );
			*/
			break;

		case SYMBOLSTAT_LIFE:
			pStrInfo->clCityhallStrInfo.siCastleLife = pclMsg->siStatValue;
			pclVillageManager->SetLifeOfCastleInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;

		case SYMBOLSTAT_MANA:
			pStrInfo->clCityhallStrInfo.siCastleMana = pclMsg->siStatValue;
			pclVillageManager->SetManaOfCastleInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;

		case SYMBOLSTAT_CAPA:
			pStrInfo->clCityhallStrInfo.siCastleCapa = pclMsg->siStatValue;
			pclVillageManager->SetCapaOfCastleInVillage( pclMsg->siVillageUnique, pclMsg->siStatValue );
			break;
		}
		
		
		SI32 siVillageCastleCharUnique;
		if( pclVillageManager->IsThereCastle(pclMsg->siVillageUnique, &siVillageCastleCharUnique ) == TRUE ) {
			
			if(pclCM->IsValidID(siVillageCastleCharUnique))
				pclCM->CR[ siVillageCastleCharUnique ]->ParametaBoxAction();
		}
		
		

		// 상단 자금 갱신
		pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
		
		// 상단 최근 갱신일 수정 
		pStrInfo->clCityhallStrInfo.clLastCastleUpdate.Set( &pclMsg->clDate );

		// 상단 입출금 내역에 추가한다			
		{
			cltMoney clMoney;
			clMoney.Set( -pclMsg->siCost );

			pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
				INOUTCODE_CASTLESTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
				INOUTCODE_CASTLESTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// TEMP LEEKH -------------------------------------------------------------------
			//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
			//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
			//	TCHAR pszCharParam1[50] = {'\0', };
			//	StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), TEXT("%d-%d-%d, %d:%d"),	
			//																pclTime->clDate.uiYear,
			//																pclTime->clDate.uiMonth,
			//																pclTime->clDate.uiDay,
			//																pclTime->clDate.uiHour,
			//																pclTime->clDate.uiMinute);

			//	cltServer* pclserver = (cltServer*)pclClient;
			//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
			//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, 
			//									pszCharParam1, NULL);
			//	// TEMP LEEKH -------------------------------------------------------------------
			//}
			

			// 게임에서 빠져 나간돈 갱신 
			clMoney.Set( -clMoney.itMoney );
			clCurrencyInfo.clOutputMoney_clVillageWar.Increase( &clMoney );
		}		



		

		// 대행수에게 통보

		cltGameMsgResponse_SetCastleStatus sendMsg;

		sendMsg.siVillageUnique = pclMsg->siVillageUnique;
		sendMsg.siCost = pclMsg->siCost;
		sendMsg.siStatKind = pclMsg->siStatKind;
		sendMsg.siStatValue = pclMsg->siStatValue;

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)return;

		cltMsg clNormalMsg( GAMEMSG_RESPONSE_SETCASTLESTATUS, sizeof( cltGameMsgResponse_SetCastleStatus ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);


		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 성벽 업그레이드시 관청 건물과 연결되어 변경되는 값이 저장.
			// param1 == 지불 비용, 
			// param2 == 스탯 종류, 
			// param3 == 스탯 수치. 
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_CASTLE_STATUS_SET, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siCost, pclMsg->siStatKind, pclMsg->siStatValue, 0, 0, NULL, NULL);
		}

//		SI32 id	= pclMsg->usCharID;
//		if(pclCM->IsValidID(id) == FALSE)return;

//		UpdateStrInfoToClient( id, RANKTYPE_CITYHALL );

	} else if( pclMsg->siResult == 2) {
		// 보유자금 부족 
		returnval = SRVAL_CITYHALL_FAIL_CASTLESTATUS_MONEY;
	}

	
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}	

void cltServer::DoMsg_DBMSG_RESPONSE_SETVILLAGEWARLOSS( sPacketHeader* pPacket )
{
	int i;

	sDBResponse_SetVillageWarLoss *pclMsg = (sDBResponse_SetVillageWarLoss *)pPacket;

	if( pclMsg->siResult == 1 ) {

		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		// 마을 총 주식 수량 갱신
		pStrInfo->clCityhallStrInfo.siTotalStockAmount = pclMsg->siTotalVillageStockAmount;

		// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
		if( pclMsg->usCount > 30 )		pclMsg->usCount = 30;
		for( i = 0; i < pclMsg->usCount; ++i ) {

			// 쪽지를 보낸다

			// pclMsg->siStockHolderPersonID[ i ]				// PersonID
			// pclMsg->siStockAmount[ i ]						// 소각 후 남은 주식 수량
			// pclMsg->siLossPersent							// 소각 비율
			// pclMsg->siWinVillage								// 승리 마을 
			
			cltLetterMsg_StockLoss clletter( pclMsg->siVillageUnique, pclMsg->siWinVillage, 
				pclMsg->siLossPersent, pclMsg->siStockAmount[ i ] );
			SendLetterMsg( pclMsg->siStockHolderPersonID[ i ], (cltLetterHeader*)&clletter );
		
			//주식이 소각될때 마을의 전쟁에 서 졌을때.소각된다.
			//주식 소각시 로그 남김 [2007.09.03 손성웅]
			SI32 tatallossstockamount = pclMsg->siTotalLossStockAmount;			//소각주식 총량
			SI32 villageuniaue		  = pclMsg->siVillageUnique;				//소각되는 마을 유니크
			// param1 == 소각주식 총량, param2 == 소각후 마을 주식 총량, param3 == 주식 소각 비율, param4 == 소각 주식 주주 숫자
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCKLOSS, 0, NULL,
								 NULL, villageuniaue, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
								 tatallossstockamount, pclMsg->siTotalVillageStockAmount, pclMsg->siLossPersent, pclMsg->usCount, 0, 
								 NULL, NULL);
		}

		if( pclMsg->usCount > 0 && pclMsg->siTotalLossStockAmount > 0 ) {
			// 공모 개시
			cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siWinVillage, RANKTYPE_CITYHALL );

			if ( pStrInfo == NULL )
			{
				return;
			}

			if ( pStrInfo->clCityhallStrInfo.siResidentNum < 1 )
			{
				return;
			}

			SI32 unitStock = (SI32)( pclMsg->siTotalLossStockAmount / pStrInfo->clCityhallStrInfo.siResidentNum );
			if( unitStock == 0)
				unitStock = 1;
			
			sDBRequest_MaxStockDistributionNum clMsg( pclMsg->siVillageUnique, pclMsg->siTotalLossStockAmount, unitStock, 5000, true, 0, pclMsg->siWinVillage );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}

	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETVILLAGELEVEL( sPacketHeader *pPacket )
{
	sDBResponse_SetVillageLevel *pclMsg = (sDBResponse_SetVillageLevel *)pPacket;

	if( pclMsg->siResult == 1 ) {

		cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		if( pclStrInfo ) {
	
			pclStrInfo->clCityhallStrInfo.siVillageLevel = pclMsg->siLevel;
		}

	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 9] // 마을 레벨.
		// param1 ==  레벨, param2 == siSlaveVillageNum, param3 == siResidentLevel, param4 == siVisitLevel.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_LEVEL_SET, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->siLevel, pclMsg->siSlaveVillageNum, pclMsg->siResidentLevel, pclMsg->siVisitLevel, 0, NULL, NULL);
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_SETVILLAGEMARK( sPacketHeader* pPacket )
{
	sDBResponse_SetVillageMark* pclMsg = (sDBResponse_SetVillageMark *)pPacket;

	// 배열인덱스 검증 추가 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if( pclMsg->siResult == 1 ) {


		if( pclMsg->siVillageUnique == 0 ) return;

		cltVillageInfo *pclVillageInfo = pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ];

		if( pclVillageInfo == NULL ) return;

		pclVillageInfo->clVillageVary.siVillageMark = pclMsg->siVillageMark;


		cltGameMsgResponse_SetVillageMark sendMsg( pclMsg->siVillageUnique, pclMsg->siVillageMark );


		cltMsg clMsg( GAMEMSG_RESPONSE_SETVILLAGEMARK, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg );

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 9] // 마을 마크 설정.
			// param1 == 마크, 
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_MARK_SET, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siVillageMark, 0, 0, 0, 0, NULL, NULL);
		}


		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_VILLAGEMARK,  0, 0, pclCM->CR[id]->GetCharUnique());

		// 보유자금 변경 
		{
			sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEMARK, 0, pclMsg->siVillageUnique, RANKTYPE_CITYHALL, -1000000 );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
		}


	} else if( pclMsg->siResult == 2 ) {
	
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_VILLAGEMARK_ALREADYUSED,  0, 0, pclCM->CR[id]->GetCharUnique());

	} else if( pclMsg->siResult == 3 ) {

		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_VILLAGEMARK_DATE,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETVILLAGEMAP( sPacketHeader* pPacket )
{
	sDBResponse_SetVillageMap* pclMsg = (sDBResponse_SetVillageMap *)pPacket;

	SI32 id	= pclMsg->usCharID;

	if( pclCM->IsValidID(id) == FALSE )return ;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	if( pclMsg->siResult == 1 ) {

		//_LEON_HUNTMAP
        switch(pclMsg->siMapType)
		{
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_CITYHALL:
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_VILLAGEMAP,  0, 0, pclCM->CR[id]->GetCharUnique());

				// 보유자금 변경 
				sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEMAP, 0, pclMsg->siVillageUnique, RANKTYPE_CITYHALL, -pclMsg->siNeedMoney );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
			break;
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_HUNT:
			{
				SendServerResponseMsg( RANKTYPE_HUNT, SRVAL_HUNT_SUCCESS_HUNTMAP,  0, 0, pclCM->CR[id]->GetCharUnique());

				// 보유자금 변경 => 수렵장 지도 변경 자금은 마을 관청 자금에서 출금된다.
				// pclMsg->siVillageUnique는 맵인덱스 번호이므로 예를들면, 인천(5번)이 205번이므로
				// 캐릭터가 현재 있는 마을 유니크를 구함
				sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEMAP, 0, villageunique, RANKTYPE_CITYHALL, -pclMsg->siNeedMoney );
				//sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_VILLAGEMAP, 0, pclMsg->siVillageUnique, RANKTYPE_HUNT, -pclMsg->siNeedMoney );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
		default:
			return;
		}
			
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 17] // 마을 지도 변경.
			// param1 == 마을 지도 번호, 
			// param2 == 돈, 
			// param3 == 지도 타입.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_MAP_CHANGE, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
											pclMsg->siVillageMapIndex, -pclMsg->siNeedMoney, pclMsg->siMapType, 0, 0, NULL, NULL);
		}

	} else {
		switch(pclMsg->siMapType)
		{
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_CITYHALL:
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_VILLAGEMAP,  0, 0, pclCM->CR[id]->GetCharUnique());
			break;
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_HUNT:
			SendServerResponseMsg( RANKTYPE_HUNT, SRVAL_HUNT_SUCCESS_HUNTMAP,  0, 0, pclCM->CR[id]->GetCharUnique());
			break;
		default:
			return;
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETINSTANCEPROFIT_VILLAGE(sPacketHeader* pPacket )
{
	sDBResponse_SetInstanceProfit_Village* pclMsg = (sDBResponse_SetInstanceProfit_Village*)pPacket ;

	if ( pclMsg->siResult == 1 )
	{
		pclVillageManager->siInstanceProfitVillageunique = pclMsg->siVillageUnique ;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_CHANGE_INSTANCEPROFIT(sPacketHeader* pPacket )
{
	sDBResponse_Change_InstanceProfit* pclMsg = (sDBResponse_Change_InstanceProfit*)pPacket ;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclMsg->bVillage )
		{
			SI32 villageunique = pclVillageManager->siInstanceProfitVillageunique ;
			GMONEY money = pclMsg->siLeftMoney ;

			if ( villageunique && pclVillageManager->IsValidVillage(villageunique) && money )
			{
				{
					sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_INSTANCEPROFIT, 0, villageunique,  money);
					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}

				{
					sDBRequest_Change_InstanceProfit sendMsg(-money,false);
					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
			}

			pclVillageManager->siInstanceProfitVillageunique = 0 ;
		}
		else
		{
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGERESIDENTSVISITORSINFO( sPacketHeader *pPacket )
{
	sDBResponse_VillageResidentsVisitorsInfo *pclMsg = (sDBResponse_VillageResidentsVisitorsInfo *)pPacket;

	if( pclMsg->siResult != 1 )
		return;

	for( SI16 i = 1;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclVillageManager->pclVillageInfo[i])
		{
			// 모든 마을의 부모마을, 선전포고 마을 정보 및 마을마크정보를 요청한다 
			{
				sDBRequest_GetVillageInfo clMsg2( i );
				pclGameDBProcess->SendMsg( i % DBCONNECTION_NUMBER, (sPacketHeader *)&clMsg2);

			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETVILLAGEWAR_RESULT(sPacketHeader *pPacket)
{
	sDBResponse_SetVillageWarResult * pclMsg = (sDBResponse_SetVillageWarResult *)pPacket;

	if( pclMsg->siResult != 1)
	{

		return;
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// TEMP LEEKH -------------------------------------------------------------------
		// [로그추가 : 황진성 2007. 10. 18] // 마을 전쟁이 끝난 결과.
		// param1	 == 승리한 마을, 
		// param2	 == 패한 마을, 
		// param3	 == 이긴 마을 자금, 
		// param4	 == 패한 마을 자금, 
		// param5	 == +,- 금액, 
		// pszParam1 == 시간.
		TCHAR str[50] = {'\0', };
		StringCchPrintf(str, sizeof(str), TEXT("%d-%d-%d, %d : %d"),	
												pclTime->clDate.uiYear,
												pclTime->clDate.uiMonth,
												pclTime->clDate.uiDay,
												pclTime->clDate.uiHour,
												pclTime->clDate.uiMinute	);

		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_RESULT, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->m_siwinVillageUnique, pclMsg->m_siloseVillageUnique, pclMsg->m_siwinnerVillageMoney, 
										pclMsg->m_siloserVillageMoney, pclMsg->m_siwinnerGetVillageMoney, 
										str, NULL);
	}

	// 승, 패자의 관청 자금 세팅

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->m_siwinVillageUnique, RANKTYPE_CITYHALL );

	if( pclStrInfo != NULL )
	{
		pclStrInfo->clCommonInfo.clMoney.itMoney  = pclMsg->m_siwinnerVillageMoney;


		cltMoney clMoney;
		clMoney.Set( pclMsg->m_siwinnerGetVillageMoney );
		
		cltMoney clCityHallMoney;
		clCityHallMoney.Set( pclMsg->m_siwinnerVillageMoney );

		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_VILLAGEWARMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney);

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->m_siwinVillageUnique , RANKTYPE_CITYHALL, 
			INOUTCODE_VILLAGEWARMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// TEMP LEEKH -------------------------------------------------------------------
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
		//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->m_siwinVillageUnique, 0, 0, 
		//									clMoney.itMoney, clCityHallMoney.itMoney, 0, 0, 0, 
		//									pszCharParam1, NULL);
		//}

		pclStrInfo->clCityhallStrInfo.siVillaeWarRecordWin++;

	}
		
	pclStrInfo = GetStrInfo( pclMsg->m_siloseVillageUnique, RANKTYPE_CITYHALL );
	
	if( pclStrInfo != NULL )
	{
		pclStrInfo->clCommonInfo.clMoney.itMoney  = pclMsg->m_siloserVillageMoney;

		cltMoney clMoney;
		clMoney.Set( -(pclMsg->m_siwinnerGetVillageMoney) );

		cltMoney clCityHallMoney;
		clCityHallMoney.Set( pclMsg->m_siloserVillageMoney );

		pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_VILLAGEWARMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney);

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->m_siloseVillageUnique , RANKTYPE_CITYHALL, 
			INOUTCODE_VILLAGEWARMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		pclStrInfo->clCityhallStrInfo.siVillaeWarRecordLose++;

		// 패배 국가의 패배 날짜 세팅
        pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateYear = pclMsg->m_clDateLoser.m_uiYear;
		pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateMonth = pclMsg->m_clDateLoser.m_uiMonth;
		pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateDay = pclMsg->m_clDateLoser.m_uiDay;
		//((cltCityhallStrInfo*)(pclStrInfo))->clLastWarLoseDate.Set( &pclMsg->m_clDateLoser );
		
	}
	//// 종료 됐으니 종료된 마을은 리스트 에서 뺀다.
	//// 2009-03-11 [ 성웅 ]
	//SI32 siWinner = pclMsg->m_siloseVillageUnique	;	// 승리 마을
	//SI32 siLosser = pclMsg->m_siwinVillageUnique	;	// 패배 마을 
	//
	//pclVillageManager->siDeclareWarVillage[siWinner]	=	0	;
	//pclVillageManager->siDeclareWarVillage[siLosser]	=	0	;
	/*
	// 상단 입출금 내역에 추가한다			
	{
		cltMoney clMoney;
		clMoney.Set( -pclMsg->siCost );

		pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_GODSTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
			INOUTCODE_GODSTAT, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );


		// 게임에서 빠져 나간돈 갱신 
		clMoney.Set( -clMoney.itMoney );
		clCurrencyInfo.clOutputMoney_clVillageWar.Increase( &clMoney );
	}
*/
	

}

void cltServer::DoMsg_DBMSG_RESPONSE_ADD_VILLAGE_SCORE(sPacketHeader* pPacket)
{
	sDBResponse_Add_Village_Score* pclMsg = (sDBResponse_Add_Village_Score*)pPacket;	

	pclVillageManager->pclVillageInfo[pclMsg->m_siVillageUnique]->clVillageVary.siVillageScore		  = pclMsg->m_siVillageScore;
	pclVillageManager->pclVillageInfo[pclMsg->m_siVillageUnique]->clVillageVary.siTotalVillageScore  += pclMsg->m_siAddVillageScore;
}

void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGEJOINPLAYTIME(sPacketHeader* pPacket)
{
	// PCK : 로그를 남기기 위한 패킷추가
	sDBReponse_VillageJoinPlayTime* pclMsg = (sDBReponse_VillageJoinPlayTime*)pPacket;

	// PCK : PersonID를 근거로 CharID를 찾음.(PersonID넣지말자!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// LEEKH MODIFY 2007.10.24
	// [로그추가 : 황진성 2007. 10. 9] // 유저의 마을 가입 후 플레이 시간.
	// param1 == 플레이 시간.
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_PLAY_TIME, 
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									pclMsg->siVillageJoinPlayTime, 0, 0, 0, 0, NULL, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGEJOINLEVELUP(sPacketHeader* pPacket)
{
	// PCK : 로그를 남기기 위한 패킷 추가
	sDBResponse_VillageJoinLevelUp*	pclMsg = (sDBResponse_VillageJoinLevelUp*)pPacket;

	// PCK : PersonID를 근거로 CharID를 찾음.(PersonID넣지말자!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// [로그추가 : 황진성 2007. 10. 9] // 마을이 있는경우 레벨업 정보.
	// Param1 == 레벨업 횟수
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_LEVEL_UP, 
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									pclMsg->siLevelCount, 0, 0, 0, 0, NULL, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGEJOINMAKEITEM(sPacketHeader* pPacket)
{
	// PCK : 로그를 남기기 위한 패킷 추가
	sDBResponse_VillageJoinMakeItem* pclMsg = (sDBResponse_VillageJoinMakeItem*)pPacket;

	// PCK : PersonID를 근거로 CharID를 찾음.(PersonID넣지말자!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// [로그추가 : 황진성 2007. 10. 16] // 마을이 있는경우 아이템 만든 스킬 정보.
	// param1 == 스킬 경험치.
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_MAKE_ITEM_SKILL_EXP, 
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
									pclMsg->siItemSkillExp, 0, 0, 0, 0, NULL, NULL);
}