//---------------------------------
// 2003/8/12 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "..\Server\Server.h"
#include "../../CommonLogic/CommonLogic.h"

//---------------------------------
// Common
//---------------------------------
#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "..\Server\Candidate\Candidate.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Cityhall.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\..\Common\Letter\Letter.h"

#include "MsgType-Structure.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-NPC.h"
#include "Msg/MsgType-Empire.h"

#include "MsgRval-Define.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"	// 마을 파산시 증자 입찰 초기화
#include "../../CommonLogic/StructureClass/StructureClass.h"
#include "../../CommonLogic/Land/Land.h"
#include "../../CommonLogic/Mine/Mine.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"

#include "../../Server/ServerEvent/ServerEvent-Quest.h"
#include "../../Server/ServerEvent/ServerEvent_Manager.h"

#include "../common/Event/event.h"
#include "../common/Item/ItemCommon/Item-EventStructure.h"

extern cltCommonLogic* pclClient;

// 마을 투자자리스트를 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_INVESTLIST(sPacketHeader* pPacket)
{
	sDBResponse_InvestList* pclMsg = (sDBResponse_InvestList*)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if(pclMsg->clInvestList.siVillageUnique <= 0 || pclMsg->clInvestList.siVillageUnique >= MAX_VILLAGE_NUMBER )
		return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// 마을 투자자 정보를 업데이트 한다.
	SI32 villageunique = pclMsg->clInvestList.siVillageUnique;
	// 데이터 검증 - by LEEKH 2008.02.13
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )				return;

	pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clInvestInfo.Set(&pclMsg->clInvestList);
	//너무 많이 찍혀서 일단 이부분은 보류
	//pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_InvestList"));
}

// 입후보 할 것을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_CANDIDATE(sPacketHeader* pPacket)
{
	sDBResponse_Candidate* pclMsg = (sDBResponse_Candidate* )pPacket;

	SI32 returnval = 0;

	// [영훈] 초보자 마을 : 초보자 마을에 입후보가 되자마자 영주가 되도록 한다
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pclMsg->clRank.siVillageUnique )
		{	// 영주를 입후보 하고 리스트에 등록에 성공 했다면 바로 영주가 되도록 한다
			if ( (RANKTYPE_CITYHALL == pclMsg->clRank.siType) && (1 == pclMsg->siResult) )
			{
				SI32 siPersonID			= pclMsg->clPerson.siPersonID;
				SI32 siCandidateIndex	= pclMsg->siIndex;

				sDBRequest_BeRank clMsg( pclMsg->usCharID, siPersonID, siCandidateIndex, &pclMsg->clRank );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				return;
			}
		}
	}
		
	switch(pclMsg->siResult)
	{
	case 0: // 등록실패. 
		returnval	= SRVAL_CITYHALL_FAILCANDIDATE;
		break;
	case 1:		// 입후보 성공. 
		{
			pclCityHallManager->SetCandidate(pclMsg->siIndex, &pclMsg->clRank, &pclMsg->clPerson);
			returnval	= SRVAL_CITYHALL_SUCCESSCANDIDATE;
		}
		break;
	case -1:	// 이미 같은 직위의 후보에 등록되어 있어 실패. 
		returnval	= SRVAL_CITYHALL_FAILCANDIDATE_ALREADY;
		break;
	case -2:	// 이미 신분을 갖고 있다면 실패. 
		returnval	= SRVAL_CITYHALL_FAILCANDIDATE_ALREADYRANK;
		break;

	default:
		MsgBox( TEXT("fd9jsff"), TEXT("Fd099:%d"), pclMsg->siResult);
		break;
	}
		
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 입후보 취소할 것을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_CANCELCANDIDATE(sPacketHeader* pPacket)
{
	sDBResponse_CancelCandidate* pclMsg = (sDBResponse_CancelCandidate* )pPacket;

	SI32 returnval = 0;
		
	switch(pclMsg->siResult)
	{
	case 0: //  등록 취소실패. 
		returnval	= SRVAL_CITYHALL_FAILCANCELCANDIDATE;
		break;
	case 1:		// 등록취소  성공. 
		{
			
			pclCityHallManager->SetCandidate(pclMsg->siIndex, &pclMsg->clRank, &pclMsg->clPerson);

			returnval	= SRVAL_CITYHALL_SUCCESSCANCELCANDIDATE;
		}
		break;
	default:
		MsgBox( TEXT("fd9jsff"), TEXT("Fd099:%d"), pclMsg->siResult);
		break;
	}
		
	
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}


// 특정 Person의 신분 정보를 얻오라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_RANKINFO(sPacketHeader* pPacket)
{
	sDBResponse_RankInfo* pclMsg = (sDBResponse_RankInfo*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 id = pclMsg->usCharID;
	if(id == 0 || pclCM->IsValidID(id)== false )
	{		
		id = pclCM->GetIDFromPersonID(pclMsg->clRank.clPerson.GetPersonID());
		if(id == 0 || pclCM->IsValidID(id)== false)
		{
			return ;
		}
	}

	pclCM->CR[id]->pclCI->clCharRank.Set(&pclMsg->clRank.clRank);

	// 클라이언트에 통보한다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_RANK, true, 0);

}

// 입후보자가 특정 신분이 되도록 할 것을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_BERANK(sPacketHeader* pPacket)
{
	sDBResponse_BeRank* pclMsg = (sDBResponse_BeRank*)pPacket;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:		//  실패. 
				//	returnval	= SRVAL_CITYHALL_FAILCANCELCANDIDATE;
		return ;
	case 1:		//	취임 성공. 
		{
			// 신분을 업데이트 한다. 
			SI32 personid = pclMsg->clRank.GetPersonID();
			if(personid)
			{
				//---------------------------------------------
				// 기존 신분의 사용자를 확보한다. 
				//---------------------------------------------
				cltRank* pclrank = pclRankManager->GetRank(pclMsg->clRank.clRank.siType, pclMsg->clRank.clRank.siVillageUnique);
				cltRank cloldrank;
				cloldrank.Set(pclrank);

				//---------------------------------------------------------------------------------
				// 기존 신분자와 새로운 신분자가 다르다면 해고당한 것이므로 쪽지를 보내 통보한다. 
				//---------------------------------------------------------------------------------
				SI32 oldpersonid = cloldrank.clPerson.GetPersonID();
				if(oldpersonid != personid)
				{
					cltLetterMsg_Fire clletter(&cloldrank.clRank,&pclTime->clDate);

					// 쪽지를 DB나 클라이언트에게 보낸다. 
					SendLetterMsg(oldpersonid, (cltLetterHeader*)&clletter);
				}
				

				//-------------------------------------------
				// 신분을 업데이트 한다. 
				//-------------------------------------------
				pclRankManager->SetRank(&pclMsg->clRank.clRank, &pclMsg->clRank.clPerson);

				
				//------------------------------------------------------------------------------
				// 후보자 명단을 업데이트 한다. 신분에 등록되었으므로 후보자에서는 지워야 한다. 
				//------------------------------------------------------------------------------
				if(pclMsg->clRank.clRank.siType)
				{
					pclCityHallManager->SetCandidate(pclMsg->siCandidateIndex, &pclMsg->clRank.clRank, &pclMsg->clCandidatePerson);
				}

				//-------------------------------------------
				// 임명된 사람에게 쪽지를 보낸다. 
				//-------------------------------------------
				cltLetterMsg_BeRank clletter(&pclMsg->clRank.clRank, &pclTime->clDate);
				// 쪽지를 DB나 클라이언트에게 보낸다. 
				SI32 letterid = SendLetterMsg(personid, (cltLetterHeader*)&clletter);
				if(pclCM->IsValidID(letterid))
				{
					pclCM->CR[letterid]->pclCI->clCharRank.Set(&pclMsg->clRank.clRank);
					pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_RANK, true, 0);
				}
			}

			// LEEKH MODIFY 2007.10.25
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 11] // Person을 특정 Rank로 만들어.
				// param1 == 직급의 타입
				// charparam1 == 직급이 되는 캐릭터 이름
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RANK, LOGCOMMAND_INDEX_RANK_SET, 
					0, NULL, NULL, pclMsg->clCandidatePerson.siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->clRank.clRank.siVillageUnique, 0, 0, 
					pclMsg->clRank.clRank.GetRankType(), 0, 0, 0, 0, (TCHAR*)pclMsg->clCandidatePerson.GetName(), NULL);
			}

			returnval	= SRVAL_CITYHALL_SUCCESSBERANK;
		}
		break;

	case 2:
		returnval   = SRVAL_CITYHALL_FAIL_BERANK_NOTHOME;
		break;

	default:
		MsgBox( TEXT("fd9jsff"), TEXT("F11d099:%d"), pclMsg->siResult);
		break;
	}
		
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 입후보자 명단을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYCANDIDATEINFO(sPacketHeader* pPacket)
{

	sDBResponse_ManyCandidateInfo* pclMsg = (sDBResponse_ManyCandidateInfo*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 i;

	for(i = 0; i < MAX_CANDIDATE_NUMBER;i++)
	{
		pclCityHallManager->SetCandidate(i, &pclMsg->clRank, &pclMsg->clPerson[i]);
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyCandidateInfo"));

}


// 후보자 명단을 삭제할 것을 요청한 것에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_DELCANDIDATE(sPacketHeader* pPacket)
{
	sDBResponse_DelCandidate* pclMsg = (sDBResponse_DelCandidate* )pPacket;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0: //  실패. 
		break;
	case 1:		// 삭제 성공 
		{

			// 특정 신분의 후보들만 삭제한다. 
			pclCityHallManager->DeleteCandidate(&pclMsg->clRank);

			returnval	= SRVAL_CITYHALL_SUCCESSDELCANDIDATE;
		}
		break;
	default:
		MsgBox( TEXT("12fd9jsff"), TEXT("F11d099:%d"), pclMsg->siResult);
		break;
	}
		
			

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;
	
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 신분직에서 사임한다는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_RESIGN(sPacketHeader* pPacket)
{
	sDBResponse_Resign* pclMsg = (sDBResponse_Resign*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	pclRankManager->SetRank(&pclMsg->clRank, &pclMsg->clPerson);
	
	// 해임 일자를 갱신 한다 
//#if defined(_CITYHALL_BANKRUPTCY)
	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
	if( pclStrInfo != NULL && pclMsg->clRank.siType == 1 )	// 대행수일 경우에만 대행수 사임일 업뎃
		pclStrInfo->clCityhallStrInfo.clResignDate.Set( &pclMsg->clDate );
//#endif
	
	//Person정보를 업데이트 한다. 
	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id))
	{
		pclCM->CR[id]->pclCI->clCharRank.Init();
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_RANK, true, 0);

		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 11] // 등록된 신분직을 사임.
			// param1 == 직급 타입
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RANK, LOGCOMMAND_INDEX_RANK_ERSIGN, 
								0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								pclMsg->clRank.siType, 0, 0, 0, 0, NULL, NULL);
		}
	}
}

// 신분정보 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_MANYRANK(sPacketHeader* pPacket)
{
	sDBResponse_ManyRank* pclMsg = (sDBResponse_ManyRank*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 i;

	for(i = 0;i < GET_MANY_RANK_NUM;i++)
	{
		if(pclMsg->clRank[i].clRank.siType)
		{
			pclRankManager->SetRank(&pclMsg->clRank[i].clRank, &pclMsg->clRank[i].clPerson);
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_ManyRank"));
}


// 주민 등록 신청에 대한 응답 
void cltServer::DoMsg_DBMSG_RESPONSE_APPLYRESIDENT(sPacketHeader* pPacket)
{
	sDBResponse_ApplyResident* pclMsg = (sDBResponse_ApplyResident*)pPacket;

	SI32 returnval = 0;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 0 ) 
	{
		return;
	}

	SI32 id	= pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) 
	{
		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 9] // 주민등록 신청 쓰기. 
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_APPLYRESIDENT, 
								0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								0, 0, 0, 0, 0, NULL, NULL);
		}
	}

	if(pclCM->IsValidID(id) == FALSE)return ;

	switch( pclMsg->siResult )
	{
	case 1:
		returnval = SRVAL_CITYHALL_SUCCESSAPPLYRESIDENT;

		((cltCityHall *)(pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ]))->AddApplyer( &pclMsg->clApplyer );

		//  마을 가입 퀘스트가 진행중인지 확인한다. 
		((cltCharServer*)pclCM->CR[id])->DoQuestCheckAndAction(QUEST_CLEAR_COND_APPLYVILLAGE);
		break;

	case -1:
		returnval = SRVAL_CITYHALL_FAILALREADYAPPLY;
		break;

	case -2:
		returnval = SRVAL_CITYHALL_FAILALREADYRESIDENT;
		break;

	case -3:
		returnval = SRVAL_CITYHALL_FAILEXCEEDAPPLYER;
		break;

	case -4:
        returnval = SRVAL_CITYHALL_FAILTIMELIMIT;
		break;
	}

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		//[진성] 마을을 통한 신청이 아닌 다른 캐릭의 초대로 신청이 된거라 별도의 메시지 주기. 
		if( true == pclMsg->bSendMsg )
		{
			SendServerResponseMsg( 0, SRVAL_VILLAGE_RESIDENT_RESULT, returnval, 0, pclCM->CR[id]->GetCharUnique() );
		}
	}	
}

// 주민 등록 신청 취소 및 주민 등록 해지 응답 
void cltServer::DoMsg_DBMSG_RESPONSE_CANCELRESIDENT(sPacketHeader* pPacket)
{

	sDBResponse_CancelResident* pclMsg = (sDBResponse_CancelResident*)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 returnval = 0;

	if( pclMsg->siResult == 0 ) 
	{
		return;
	}
	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) 
	{
		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 9. 20] // 주민등록 신청 취소.
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_RESIDENTCANCEL,
								0, pclchar, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0,
								0, 0, 0, 0, 0, NULL, NULL);
		}
	}

	SI32 residentid	= pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	switch( pclMsg->siResult )
	{
	case 1:
		{
			returnval = SRVAL_CITYHALL_SUCCESSCANCELRESIDENT;
			
			if( pclCM->IsValidID( residentid ) ) {

				if( pclCM->CR[residentid]->pclCI->clBI.siHomeVillage == pclMsg->siVillageUnique ) {
					// 주민 등록 된 상태에서 등록 해지 
					
					pclCM->CR[residentid]->pclCI->clBI.siHomeVillage = 0;
					
					cltGameMsgResponse_HomeVillage clinfo(pclCM->CR[residentid]->GetCharUnique(), 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_HOMEVILLAGE, sizeof(clinfo), (BYTE*)&clinfo);
					((cltCharServer*)pclCM->CR[residentid])->SendNeighbourMsg(&clMsg, true);

					// 제국 정보 초기화
					pclCM->CR[residentid]->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_NONE, 0 );
					cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[residentid]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_NONE, 0 );
					cltMsg clMsg2(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
					((cltCharServer*)pclCM->CR[residentid])->SendNeighbourMsg(&clMsg2, true);
				}
			}

			// 주민 숫자 변경
			pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique )->clCityhallStrInfo.siResidentNum = pclMsg->siResidentNum;

			((cltCityHall *)(pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ]))->DeleteApplyer( pclMsg->siPersonID );

			if(pclCM->IsValidID(id)) {
			
				cltMsg clMsg( GAMEMSG_RESPONSE_UPDATERESIDENTAPPLYERLIST, pclMsg->siPersonID );

				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		break;

	case 0:
		returnval = SRVAL_CITYHALL_FAILCANCELRESIDENT;
		break;
	}

	if(returnval)
	{
		
		// 클라이언트로 서버의 응답값을 보낸다. 
		if(pclCM->IsValidID(id)) SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}	
	
}


void cltServer::DoMsg_DBMSG_RESPONSE_MANYRESIDENTOUT( sPacketHeader *pPacket )
{
	sDBResponse_ManyResidentOut* pclMsg = (sDBResponse_ManyResidentOut*)pPacket;

	if( pclMsg->siResult == 0 ) {
		return;
	}

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->usCount > 200)		pclMsg->usCount = 200;

	for( int i = 0; i < pclMsg->usCount; ++i ) 
	{
		if( pclMsg->siPersonID[ i ] < 0 )			continue;

		// 자동 해지된 캐릭터에게 쪽지를 보낸다.
		cltLetterMsg_ResidentOutDate clletter( pclMsg->siVillageUnique );
		sDBRequest_PushLetter clMsg( pclMsg->siPersonID[ i ], (cltLetterUnit *)&clletter );
		pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);
		
		// 자동탈퇴된 로그를 찍는다.
		// PersonID만 알고있으므로 - TargetPersonID에 기록한다.
		// CharParam1 : 군주력
		NTCHARString<50>	kCharPaaram1;
		kCharPaaram1 += UI32ToString(pclTime->clDate.uiYear);		kCharPaaram1 += "-";
		kCharPaaram1 += UI32ToString(pclTime->clDate.uiMonth);		kCharPaaram1 += "-";
		kCharPaaram1 += UI32ToString(pclTime->clDate.uiDay);
		
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_DISMISS_RESIDENT, 
			0, NULL, NULL, pclMsg->siPersonID[ i ], 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			0, 0, 0, 0, 0, kCharPaaram1, NULL);
	}

	// 마을의 인원수를 감소시킨다.
	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
	if( pStrInfo )
	{
		pStrInfo->clCityhallStrInfo.siResidentNum -= pclMsg->usCount;
		// 보안코드 - 주민수가 0보다 작을수는 없다.
		if( pStrInfo->clCityhallStrInfo.siResidentNum < 0 )		pStrInfo->clCityhallStrInfo.siResidentNum = 0;
	}
}


// 주민 등록 신청자 리스트 응답 
void cltServer::DoMsg_DBMSG_RESPONSE_GETRESIDENTAPPLYERLIST(sPacketHeader* pPacket)
{
	sDBResponse_GetResidentApplyerList* pclMsg = (sDBResponse_GetResidentApplyerList *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 0 )
	{
		return ;
	}

	for( int i = 0; i < MAX_RESIDENT_APPLYER; ++i ) {

		((cltCityHall *)(pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ]))->SetResidentApplyer( i, &pclMsg->clApplyer[ i ] );
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetResidentApplyerList"));

}

void cltServer::DoMsg_DBMSG_RESPONSE_GETRESIDENTINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetResidentInfo *pclMsg = (sDBResponse_GetResidentInfo *)pPacket;

	UI16 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id)) {

		cltGameMsgResponse_GetResidentInfo sendMsg;

		sendMsg.siResult = pclMsg->siResult;

		sendMsg.clResident.clPerson.Set( pclMsg->siPersonID, pclMsg->szPersonName );
		sendMsg.clResident.siGrade = pclMsg->siScore;
		sendMsg.clResident.clDate.Set( &pclMsg->clRegDate );
		
		cltMsg clMsg( GAMEMSG_RESPONSE_RESIDENTINFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		SI32 returnval = 0;

		if( pclMsg->siResult == 1 ) returnval = SRVAL_CITYHALL_SUCCESS_GETRESIDENTINFO;
		else returnval = SRVAL_CITYHALL_FAIL_GETRESIDENTINFO;

		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

}

// 주민 리스트 응답 
void cltServer::DoMsg_DBMSG_RESPONSE_GETRESIDENTLIST(sPacketHeader* pPacket)
{
	sDBResponse_GetResidentList* pclMsg = (sDBResponse_GetResidentList *)pPacket;

	cltGameMsgResponse_GetResidentList sendMsg;

	UI16 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE) return;

	int c = 0;
	for( int i = 0; i < MAX_RESIDENT_NUMBER; ++i ) {

		if( pclMsg->clResident[ i ].clPerson.GetPersonID() ) {

			sendMsg.clResident[ c ].Set( &pclMsg->clResident[ i ] );

			c++;
		}

	}

	sendMsg.count = c;

	sendMsg.siVillageUnique = pclMsg->siVillageUnique;

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_RESIDENTLIST, sendMsg.GetTotalSize(), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);

/*	
	sDBResponse_GetResidentList* pclMsg = (sDBResponse_GetResidentList *)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	for( int i = 0; i < MAX_RESIDENT_NUMBER; ++i ) {

		((cltCityHall *)(pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ]))->SetResident( i, &pclMsg->clResident[ i ] );
	}
*/
	
}

// 주민 등록 허가 응답 
void cltServer::DoMsg_DBMSG_RESPONSE_BERESIDENT(sPacketHeader* pPacket)
{
	sDBResponse_BeResident* pclMsg = (sDBResponse_BeResident *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	SI32 id	= pclMsg->usCharID;

	if(pclMsg->siResult == 1)
	{
		if ( IsCountrySwitch(Switch_Server_Event) )
		{
			CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclServerEventManager->GetServerEvent_Quest();
			if ( pclSE_Quest )
			{
				pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_RESIDENTVILLAGE, pclMsg->siVillageUnique, 1 );
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 9] // 주민등록 허락 쓰기. 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_BERESIDENT, 
								0, NULL, NULL, pclMsg->clResident.clPerson.GetPersonID(), 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								0, 0, 0, 0, 0, NULL, NULL);
		}
	}

	SI32 residentid = pclCM->GetIDFromPersonID(pclMsg->clResident.clPerson.GetPersonID());

	cltCityHall *pclCityhall = ((cltCityHall *)(pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ]));

	if( pclCityhall == NULL ) return;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if(pclMsg->siResult == 2)
	{
		if( pclCM->IsValidID(id) == FALSE ) return;

		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_BERESIDENT_WRONGRANK,  0, 0, pclCM->CR[id]->GetCharUnique());
		pclCityhall->DeleteApplyer( pclMsg->clResident.clPerson.GetPersonID() );

		return;
	}
		
	// 초보자 마을 : 초보자 마을 가입 완료
	bool bCondition = false;

	if ( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true )
	{
		if ( Const_Beginner_Village == pclMsg->siVillageUnique )
		{
			bCondition = true;
		}
	}

	// 주민 신청 리스트에서 삭제 
	if ( pclCityhall->DeleteApplyer(pclMsg->clResident.clPerson.GetPersonID( )) || (true == bCondition) )
	{
		// 주민 숫자 증가 
		pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique )->clCityhallStrInfo.siResidentNum++;

		//
		if( pclCM->IsValidID(residentid) ) 
		{
			pclCM->CR[residentid]->pclCI->clBI.siHomeVillage = pclMsg->siVillageUnique;

			{
				cltGameMsgResponse_HomeVillage clinfo(pclCM->CR[residentid]->GetCharUnique(), pclMsg->siVillageUnique);
				cltMsg clMsg(GAMEMSG_RESPONSE_HOMEVILLAGE, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclCM->CR[residentid])->SendNeighbourMsg(&clMsg, true);
			}

			// 제국정보 업데이트
			SI32 empireUnique = pclEmpireManager->FindEmpireUnique( pclMsg->siVillageUnique );

			if( 0 < empireUnique )
			{
				pclCM->CR[residentid]->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_PERSON, empireUnique );
				cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[residentid]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_PERSON, empireUnique );
				cltMsg clMsg2(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
				((cltCharServer*)pclCM->CR[residentid])->SendNeighbourMsg(&clMsg2, true);
			}

			{
				cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique  );

				if( pclinfo != NULL)
				{
					SI32 siEventNPCKind[MAX_HIRED_EVENTNPC_NUM];
					// 이벤트NPC정보 전송
					for (SI32 i = 0; i <MAX_HIRED_EVENTNPC_NUM; i++)
					{
						siEventNPCKind[i] = pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind;
					}
					cltGameMsgResponse_VillageHaveEventNPC clinfo( siEventNPCKind );
					cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC, sizeof(clinfo), (BYTE*)&clinfo);
					((cltCharServer*)pclCM->CR[residentid])->SendNeighbourMsg(&clMsg, true);
				}
			}
		}

	}
		
	if( pclCM->IsValidID(id) == FALSE ) return;

	cltMsg clMsg( GAMEMSG_RESPONSE_UPDATERESIDENTAPPLYERLIST, pclMsg->clResident.clPerson.GetPersonID() );

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


void cltServer::DoMsg_DBMSG_RESPONSE_HIRENPC( sPacketHeader* pPacket )
{
	sDBResponse_HireNPC *pclMsg = (sDBResponse_HireNPC *)pPacket;
	if( pclMsg->siVillageUnique < 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if(pclMsg->siResult == 1)
	{
		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 18]
			// param1 == NPC 고용이냐 트리 고용이냐.
			// param2 == 슬롯.
			// param3 == 월급. 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_NPCHIRE, 
								0, NULL, NULL, 0, pclMsg->clHireNPCInfo.siHirePay, pclMsg->clCityhallMoney.itMoney, NULL, 0, 0, pclMsg->clHireNPCInfo.siNPCKind, pclMsg->siVillageUnique, 0, 0, 
								pclMsg->uiMode, pclMsg->usSlot, pclMsg->clHireNPCInfo.siMonthlyPay, 0, 0, NULL, NULL);
		}
	}

	UI16 id	= pclMsg->usCharID;

	if( pclMsg->siResult == 1  &&  pclMsg->uiMode == HIRENPC_HIRENPC ) 
	{
		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );

		// 상단 보유자금 갱신 (고용일경우)
		if( pclMsg->clHireNPCInfo.siNPCKind ) 
		{
			pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
		}

		//---------------------------------------------
		// 서버에 NPC를 배치한다. 
		//---------------------------------------------
		// 고용한다. 
		if(pclMsg->clHireNPCInfo.siNPCKind)
		{
			Hire(pclMsg->clHireNPCInfo.siNPCKind, pclMsg->usSlot, pclMsg->siVillageUnique, true);

			if(pclCM->IsValidID(id)) 
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_HIRENPC, 0, 0, pclCM->CR[id]->GetCharUnique());
			}


					cltMoney clMoney;
					clMoney.Set( -(SI64)( pclMsg->clHireNPCInfo.siMonthlyPay * 12 ) );

					pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( pclMsg->clHireNPCInfo.siNPCKind,
																					INOUTCODE_NPCPAY, 
																					&pclTime->clDate, 
																					&clMoney, 
																					&pclMsg->clCityhallMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg(	pclMsg->siVillageUnique,
																pclMsg->clHireNPCInfo.siNPCKind, 
																INOUTCODE_NPCPAY, 
																&pclTime->clDate, 
																&clMoney, 
																&pclMsg->clCityhallMoney );

					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

					clMoney.Set( -clMoney.itMoney );
					clCurrencyInfo.clOutputMoney_NPCPayment.Increase( &clMoney );

		}
		// 해고한다. 
		else
		{
			Hire(pclMsg->clHireNPCInfo.siNPCKind, pclMsg->usSlot, pclMsg->siVillageUnique, false);
		}
		
		if( pclMsg->usSlot >= 0 && pclMsg->usSlot < MAX_HIRED_NPC_NUM )
			pclinfo->clCityhallStrInfo.clHiredNPC[ pclMsg->usSlot ] = pclMsg->clHireNPCInfo;

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)return ;

		cltGameMsgResponse_HireNPC clHireNPC;												

		clHireNPC.siResult = 1;
		clHireNPC.siVillageUnique = pclMsg->siVillageUnique;	

		// 클라이언트로 정보를 보낸다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_HIRENPC, sizeof(cltGameMsgResponse_HireNPC), (BYTE*)&clHireNPC );
	
		if(pclCM->IsValidID(id))	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else if( pclMsg->siResult == 1 && pclMsg->uiMode == HIRENPC_HIREEVENTNPC )
	{
		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclMsg->siVillageUnique );
		if(pclinfo == NULL)			return;

		// 상단 보유자금 갱신 (고용일경우)
		if( pclMsg->clHireNPCInfo.siNPCKind ) 
		{
			pclinfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
		}
		//---------------------------------------------
		// 서버에 NPC를 배치한다. 
		//---------------------------------------------
		// 고용한다. 
		if( pclMsg->clHireNPCInfo.siNPCKind )
		{
			//---------------------------------------------
			// 마을에 이벤트NPC 정보 설정
			//---------------------------------------------
			if( pclinfo && (pclMsg->usSlot >= 0 && pclMsg->usSlot < MAX_HIRED_EVENTNPC_NUM))
			{
				pclinfo->clCityhallStrInfo.clHiredEventNPC[pclMsg->usSlot].Set(&pclMsg->clHireNPCInfo);
			}
			
			// 이벤트 NPC
			HireEventNPC( pclMsg->clHireNPCInfo.siNPCKind, pclMsg->siVillageUnique, pclMsg->usSlot, true );

			if(pclCM->IsValidID(id)) 
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_HIRENPC, 0, 0, pclCM->CR[id]->GetCharUnique());
			}

			cltMoney clMoney;
			clMoney.Set( -(SI64)( pclMsg->clHireNPCInfo.siHirePay ) );

			pclinfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 	pclMsg->clHireNPCInfo.siNPCKind,
																			INOUTCODE_NPCPAY, 
																			&pclTime->clDate, 
																			&clMoney, 
																			&pclMsg->clCityhallMoney );

			sDBRequest_CityHallSetInOutMoney sendMsg(	pclMsg->siVillageUnique,
														pclMsg->clHireNPCInfo.siNPCKind, 
														INOUTCODE_NPCPAY, 
														&pclTime->clDate, 
														&clMoney, 
														&pclMsg->clCityhallMoney );

            pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

			clMoney.Set( -clMoney.itMoney );
			clCurrencyInfo.clOutputMoney_NPCPayment.Increase( &clMoney );

			// PCK : 히나단이 있을때 선공방지임을 알려준다.
			if(pclMsg->clHireNPCInfo.siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))
			{
				// 이웃에게 통보 
				cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, true);
				cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
				((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
			}


		}
		else
		{
			HireEventNPC( pclMsg->clHireNPCInfo.siNPCKind,  pclMsg->siVillageUnique, pclMsg->usSlot, false );
			//---------------------------------------------
			// 마을에 이벤트NPC 정보 설정
			//---------------------------------------------
			if( pclinfo && (pclMsg->usSlot >= 0 && pclMsg->usSlot < MAX_HIRED_EVENTNPC_NUM) )
			{
				pclinfo->clCityhallStrInfo.clHiredEventNPC[pclMsg->usSlot].Set(&pclMsg->clHireNPCInfo);
			}
		}
	
		//  모든 마을 사람의 이벤트NPC 정보를 갱신한다.
		SI32 structurekind[MAX_HIRED_EVENTNPC_NUM];
		for (SI32 i = 0; i <MAX_HIRED_EVENTNPC_NUM; i++) 
		{
			structurekind[i]= pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind;
		}

		cltGameMsgResponse_VillageHaveEventNPC clinfo( structurekind );
		cltMsg clMsg( GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC, sizeof(clinfo), (BYTE*)&clinfo );
		pclCM->SendAllPersonHomeMsg( (sPacketHeader*)&clMsg, pclMsg->siVillageUnique );
	}
	else if( pclMsg->siResult == -1  && pclMsg->uiMode == HIRENPC_HIREEVENTNPC ) 
	{
		// 상단 보유자금 부족
		if(pclCM->IsValidID(id)) 
		{
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_HIRENPC_NOTENOUGHMONEY, 0, 0, pclCM->CR[id]->GetCharUnique());
		}
	} 
	else if( pclMsg->siResult == -1  && pclMsg->uiMode == HIRENPC_HIREEVENTNPC )
	{
		// 상단 보유자금 부족
		if(pclCM->IsValidID(id)) 
		{
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_HIRECHRISTMAS_NOTENOUGHMONEY, 0, 0, pclCM->CR[id]->GetCharUnique());
		}
	} 	

}

void cltServer::DoMsg_DBMSG_RESPONSE_RESIDENTSHARE(	sPacketHeader *pPacket )
{
	
	sDBResponse_ResidentShare *pclMsg = (sDBResponse_ResidentShare *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siCount > 100 )			pclMsg->siCount = 100;

	SI32 charID, personID;


	if( pclMsg->siResult == 1 ) {


		cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		if( pclStrInfo ) {
			//pclStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );			
			pclStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clCityhallMoney ); 

			// 상단 입출금 내역에 추가한다
			cltMoney clMoney;

			clMoney.Set( -pclMsg->clTotalShareMoney.itMoney );

			pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
				INOUTCODE_RESIDENTSHARE, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, RANKTYPE_CITYHALL, 
				INOUTCODE_RESIDENTSHARE, &pclTime->clDate, &clMoney, &pclMsg->clCityhallMoney );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

			// LEEKH MODIFY 2007.10.25
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			//{
			//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
			//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
			//	// charparam1 == 게임시간
			//	TCHAR szCharParam1[50];
			//	StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("%03d-%02d-%02d %02d:%02d"), 
			//																pclTime->clDate.uiYear, 
			//																pclTime->clDate.uiMonth, 
			//																pclTime->clDate.uiDay,
			//																pclTime->clDate.uiHour, 
			//																pclTime->clDate.uiMinute);

			//	cltServer* pclserver = (cltServer*)pclClient;
			//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
			//									INOUTCODE_RESIDENTSHARE, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
			//									clMoney.itMoney, pclMsg->clCityhallMoney.itMoney, 0, 0, 0, szCharParam1, NULL);
			//}
		}

		for( int i = 0; i < pclMsg->siCount; ++i ) 
		{
			charID = pclMsg->siCharIndex[ i ];
			personID = pclMsg->siPersonID[ i ];

			if( pclCM->IsValidID(charID) && pclCM->CR[ charID ]->pclCI->GetPersonID() == personID ) {

				// 현금 정보를 갱신한다.
				pclCM->CR[ charID ]->pclCI->clIP.SetMoney( pclMsg->siMoney[ i ] );

				// 클라이언트로 정보를 보낸다. 
				pclCM->CR[ charID ]->SetUpdateSwitch( UPDATE_IP, true, 0 );
				

				cltLetterMsg_ResidentShare clletter( pclMsg->siVillageUnique, 1, pclMsg->siShareMoney[ i ] );
				cltLetterUnit clletterunit( (cltLetterHeader *)&clletter );
				pclCM->CR[ charID ]->SendLetterMsg( &clletterunit );

			}

		}

	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_BUILDSTRUCTURE( sPacketHeader *pPacket )
{
	sDBResponse_BuildStructure *pclMsg = (sDBResponse_BuildStructure *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 1 ) 
	{ 
		pclMsg->siCost;

		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
		
		if( pStrInfo ) 
		{
			pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCityhallMoney );
		}

		pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ]->clVillageVary.clStructureInfo.Set( pclMsg->siStructure, pclMsg->siSlot );

		//------------------------------------------
		// 사용자들을 모두 마을 밖으로 워프 시킨다. 
		//------------------------------------------
		pclCM->WarpToVillageGate(pclMsg->siVillageUnique);
		
		//-------------------------------------------
		// 해당 마을안의 구조를 다시 설정한다. 
		//-------------------------------------------
		pclMapManager->MakeLastMap(pclMsg->siVillageUnique);
		//pclMapManager->pclMapArray[pclMsg->siVillageUnique]->NewFieldObjectSetInMap();

		
		// [진성] 마을 점수를 사용하는거면 
		sDBRequest_Add_Village_Score clScoreMsg( pclMsg->siVillageUnique, -pclMsg->siNeedVillageScore );
		pclClient->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clScoreMsg);
		
		
		cltGameMsgResponse_BuildStructure sendMsg( pclMsg->siVillageUnique, pclMsg->siSlot, pclMsg->siStructure );
		cltMsg clMsg( GAMEMSG_RESPONSE_BUILDSTRUCTURE, sizeof( cltGameMsgResponse_BuildStructure ), (BYTE*)&sendMsg );

		pclCM->SendAllPersonInVillageMsg( (sPacketHeader*)&clMsg, pclMsg->siVillageUnique );

		// 건물 내구도를 최대로 설정한다
		{
			SI32 ranktype = pclVillageStructureSet->GetRankTypeFromStrUnique( pclMsg->siStructure );
			if( ranktype ) 
			{
				sDBRequest_SetStrDura sendMsg( pclMsg->siVillageUnique, ranktype, 10000 );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 11] // 건물을 건설.
			// param1 == 슬롯.  
			// param2 == 건설 건물 unique, 
			// param3 == 타입.
			// param4 == 건설 비용.
			// param5 == 관청자금.
			cltCharServer* pclchar = NULL; 
			if(pclCM->IsValidID(pclMsg->usCharID))	pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_BUILD_STRUCTURE, 
				0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siSlot, pclMsg->siStructure, pclMsg->siType, pclMsg->siCost, pclMsg->clCityhallMoney.itMoney, NULL, NULL);
		}

	} else if(pclMsg->siResult == 2 ) {
		
		SI32 id	= pclMsg->usCharID;

		if(pclCM->IsValidID(id)) {

			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTENOUGHMONEY, 0, 0, pclCM->CR[id]->GetCharUnique());
		}

	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_DESTROYSTRUCTURE( sPacketHeader *pPacket )
{
	sDBResponse_DestroyStructure *pclMsg = (sDBResponse_DestroyStructure *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( pclMsg->siResult == 1 )
	{

		//------------------------------------------
		// 행수를 해임한다 
		//------------------------------------------
		cltSimpleRank clrank( pclMsg->siRankType, pclMsg->siVillageUnique );
		pclRankManager->DeleteRank( pclMsg->siChiefPersonID, &clrank );

		//------------------------------------------
		// 마을 안의 구조물을 삭제한다. 
		//------------------------------------------
		pclMapManager->pclMapArray[pclMsg->siVillageUnique]->DeleteVillageStructure(pclMsg->siSlot);

		pclVillageManager->pclVillageInfo[ pclMsg->siVillageUnique ]->clVillageVary.clStructureInfo.Del( pclMsg->siSlot );
        
		// 건물이 사라진경우 후처리 - 임대정보를 삭제한다.
		switch( pclMsg->siStructure )
		{
		case 50:		// 광산
			{
				// 모든 농장 사용자를 삭제한다. - DB에서도 삭제되었음.
				if(pclMsg->siVillageUnique)
				{
					cltMine *pclMine = (cltMine*)pclMineManager->pclStruct[pclMsg->siVillageUnique];
					pclMine->InitRentApplyer();
					pclMine->InitMineUnits();
				}
			}
			break;
		case 130:		// 농장
			{
				// 모든 광산 사용자를 삭제한다. - DB에서도 삭제되었음.
				if(pclMsg->siVillageUnique)
				{
					cltLand *pclLand = (cltLand *)pclLandManager->pclStruct[pclMsg->siVillageUnique];
					pclLand->InitRentApplyer();
					pclLand->InitLandUnits();
				}
			}
			break;
		}

		//------------------------------------------
		// 사용자들을 모두 마을 밖으로 워프 시킨다. 
		//------------------------------------------
		pclCM->WarpToVillageGate(pclMsg->siVillageUnique);

		//-------------------------------------------
		// 해당 마을안의 구조를 다시 설정한다. 
		//-------------------------------------------
		pclMapManager->MakeLastMap(pclMsg->siVillageUnique);
		
		//-------------------------------------------
		// 마지막으로 건물을 삭제한 시간을 업데이트 한다. 
		//--------------------------------------------
		// 마을의 villageunique 근거로 건물의 정보를 구한다. 
		cltStrInfo* pclinfo = GetStrInfo( pclMsg->siVillageUnique , RANKTYPE_CITYHALL );
		if(pclinfo == NULL)return ;
		pclinfo->clCityhallStrInfo.siLastDestroyStrDate = pclMsg->siLastDestroyDateVary;

		cltGameMsgResponse_DestroyStructure sendMsg( pclMsg->siVillageUnique, pclMsg->siSlot, pclMsg->siStructure ); 
		cltMsg clMsg( GAMEMSG_RESPONSE_DESTROYSTRUCTURE, sizeof( cltGameMsgResponse_DestroyStructure ), (BYTE*)&sendMsg );

		pclCM->SendAllPersonInVillageMsg( (sPacketHeader*)&clMsg, pclMsg->siVillageUnique );

		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			cltCharServer* pclchar = NULL; 
			if(pclCM->IsValidID(pclMsg->usCharID))	pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];

			// [로그추가 : 황진성 2007. 10. 11] // 건물 제거.
			// param1 == 슬롯.  param2 == 건설 건물. unique, param3 == 행수 타입. param4 == 건물을 해체한 마지막 시간.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STRUCTURE, LOGCOMMAND_INDEX_STRUCTURE_DESTROY, 
				0, pclchar, NULL, pclMsg->siChiefPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siSlot, pclMsg->siStructure, pclMsg->siRankType, pclMsg->siLastDestroyDateVary, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_PROFITTOCAPITALFUND( sPacketHeader *pPacket )
{
	sDBResponse_ProfitToCapitalFund *pclMsg = (sDBResponse_ProfitToCapitalFund *)pPacket;

	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) {

		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

		pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clCapitalFund );
		pStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clProfitMoney );

		if(pclCM->IsValidID(id)) {

			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_SUCCESS_PROFITTOCAPITALFUND, 0, 0, pclCM->CR[id]->GetCharUnique());
		}
	} else {
		if(pclCM->IsValidID(id)) {

			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_PROFITTOCAPITALFUND, 0, 0, pclCM->CR[id]->GetCharUnique());
		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_CHANGECITYHALLPROFITMONEY( sPacketHeader *pPacket )
{
	sDBResponse_ChangeCityhallProfitMoney *pclMsg = (sDBResponse_ChangeCityhallProfitMoney *)pPacket;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

	// 성공이면 건물 보유자금 갱신 
	if( pclMsg->siResult == 1 ) {

		if( pStrInfo )
		{
			pStrInfo->clCityhallStrInfo.clProfitMoney.Set( &pclMsg->clMoney );

			// 대마도 이익금이면 기록해준다.
			switch(pclMsg->siReason)
			{
			case CHANGESTRMONEY_INSTANCEPROFIT:
				{
					cltMoney clMoney;
					clMoney.Set( (SI64)(pclMsg->siChangeMoney ) );

					pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( 0,
						INOUTCODE_INSTANCEPROFIT, &pclTime->clDate, &clMoney, &pclMsg->clMoney );

					sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique, 0, 
						INOUTCODE_INSTANCEPROFIT, &pclTime->clDate, &clMoney, &pclMsg->clMoney );
					
					pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
				break;
			default:
				break;
			}
		}

	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_INFO( sPacketHeader *pPacket )
{
	sDBResponse_GeneralMeetingInfo *pclMsg = (sDBResponse_GeneralMeetingInfo *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			return;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

	if( pclCityHall == NULL ) return;

	pclCityHall->clGeneralMeetingInfo.siVillageUnique = pclMsg->siVillageUnique;
	pclCityHall->clGeneralMeetingInfo.siMatterType = pclMsg->siMatterType;
	pclCityHall->clGeneralMeetingInfo.siTotalStock = pclMsg->siTotalStock;
	pclCityHall->clGeneralMeetingInfo.siPersonID = pclMsg->siPersonID;
	pclCityHall->clGeneralMeetingInfo.clStartDate.Set( &pclMsg->clStartDate );
	pclCityHall->clGeneralMeetingInfo.clCloseDate.Set( &pclMsg->clCloseDate );
	pclCityHall->clGeneralMeetingInfo.siCandidatePersonID = pclMsg->siCandidatePersonID;
	memcpy( pclCityHall->clGeneralMeetingInfo.szCandidateName, pclMsg->szCandidateName, MAX_PLAYER_NAME );
	pclCityHall->clGeneralMeetingInfo.szCandidateName[ MAX_PLAYER_NAME - 1 ] = NULL;
	pclCityHall->clGeneralMeetingInfo.siAcceptCandidate = pclMsg->siAcceptCandidate;
	
#ifdef _SAFE_MEMORY
	pclCityHall->clGeneralMeetingInfo.clVoter.ZeroMem();
#else
	ZeroMemory( pclCityHall->clGeneralMeetingInfo.clVoter, sizeof( cltGeneralMeetingVoter ) * 100 );
#endif
	pclCityHall->clGeneralMeetingInfo.SetVoter( pclMsg->voter );
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GeneralMeetingInfo"));

}

//[추가 : 황진성 2008. 2. 21 => 주주총회 발동 설정 마을 관련 처리함수 DB->S ]
void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF( sPacketHeader* pPacket )
{
	sDBResponse_GeneralMeeting_Suggest_On_Off *pclMsg = (sDBResponse_GeneralMeeting_Suggest_On_Off*)pPacket;
	
	if( 0 < pclMsg->m_siVillageUnique && MAX_VILLAGE_NUMBER > pclMsg->m_siVillageUnique )
	{
		pclClient->pclVillageManager->pclVillageInfo[pclMsg->m_siVillageUnique]->clVillageVary.m_bGeneralMeetingSuggest = pclMsg->m_bGeneralMeeting_Suggest_On_Off;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF_LIST( sPacketHeader* pPacket )
{
	sDBResponse_GeneralMeeting_Suggest_On_Off_List *pclMsg = (sDBResponse_GeneralMeeting_Suggest_On_Off_List*)pPacket;
	
	for(SI32 Count = 0; Count < MAX_VILLAGE_NUMBER; ++Count)
	{
		pclClient->pclVillageManager->pclVillageInfo[Count]->clVillageVary.m_bGeneralMeetingSuggest = pclMsg->m_bGeneralMeeting_Suggest_On_Off_List[Count];
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_SUGGEST_CHIEF( sPacketHeader *pPacket )
{
	sDBResponse_GeneralMeeting_SuggestChief *pclMsg = (sDBResponse_GeneralMeeting_SuggestChief *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			return;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

	if( pclCityHall == NULL ) return;

	SI32 returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST;

	SI32 id = pclMsg->usCharID;

	switch( pclMsg->siResult ) 
	{
		case 1:	// 발의 성공 
			{
				pclCityHall->clGeneralMeetingInfo.siVillageUnique	= pclMsg->siVillageUnique;
				pclCityHall->clGeneralMeetingInfo.siMatterType		= 1;
				pclCityHall->clGeneralMeetingInfo.siTotalStock		= pclMsg->siTotalStock;
				pclCityHall->clGeneralMeetingInfo.siPersonID		= pclMsg->siPersonID;
				pclCityHall->clGeneralMeetingInfo.clStartDate.Set( &pclMsg->clStartDate );
				pclCityHall->clGeneralMeetingInfo.clCloseDate.Set( &pclMsg->clCloseDate );
				pclCityHall->clGeneralMeetingInfo.siCandidatePersonID = pclMsg->siCandidatePersonID;
				memcpy( pclCityHall->clGeneralMeetingInfo.szCandidateName, pclMsg->szCandidateName, MAX_PLAYER_NAME );
				pclCityHall->clGeneralMeetingInfo.szCandidateName[ MAX_PLAYER_NAME - 1 ] = NULL;
				pclCityHall->clGeneralMeetingInfo.siAcceptCandidate = 0;
				
	#ifdef _SAFE_MEMORY
				pclCityHall->clGeneralMeetingInfo.clVoter.ZeroMem();
	#else
				ZeroMemory( pclCityHall->clGeneralMeetingInfo.clVoter, sizeof( cltGeneralMeetingVoter ) * 100 );
	#endif
				pclCityHall->clGeneralMeetingInfo.SetVoter( pclMsg->clVoter );

				returnval = SRVAL_GENERALMEETING_SUCCESS_SUGGEST;


				cltGameMsgResponse_GeneralMeetingInfo sendMsg;

				sendMsg.clGeneralMeetingInfo = pclCityHall->clGeneralMeetingInfo;

				// 해당 마을이 정상 상태이고 대행수가 없을 때
				// 즉, 파산으로 갈 가능성이 있는 마을일 때만 주총 발의 성공시 해임 일자를 갱신 한다.
				// 해임일을 기준으로 파산 선고를 하게 된다.
				cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

				if( pclStrInfo != NULL )
				{
					cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL, pclMsg->siVillageUnique);
					cltSimpleRank tmpSimpleRank;
					tmpSimpleRank.Init();

					// 대행수가 없고
					if(pclrank == NULL || pclrank->clPerson.siPersonID == 0/*pclrank->clRank.siVillageUnique == 0 || pclrank->IsSame( &tmpSimpleRank ) == TRUE*/ ) // 초기화 값과 같다는것은 대행수가 없는 것이므로
					{
						// 정상기 일때만 파산 선고 기준일을 위해 해임 일자 갱신
						if( pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_NORMAL )
							pclStrInfo->clCityhallStrInfo.clResignDate.Set( &(((cltServer *)pclClient)->pclTime->clDate) );
					}
				}

				// 클라이언트로 정보를 보낸다. 
				if( pclCM->IsValidID(id) ) 
				{
					// 주식을 업데이트 해준다.
					pclCM->CR[id]->pclCI->clStock.clInfo.Set( pclMsg->siVillageUnique, pclMsg->siStockAmount );
					pclCM->CR[id]->SetUpdateSwitch( UPDATE_STOCK, true, 0 );

					cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_INFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}

				//cyj 주주총회 발의되자마자 뉴스 자막으로 뜨도록 변경
				dwSpecialNews = CurrentClock + 60001;
				dwGeneralMeetingNewsMin = sTime.wMinute + GENERALMEETING_NEWS_INTERVAL;

				// LEEKH MODIFY 2007.10.25
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 18] // 주주총회 발의. (확인필요 : 정보가 부족한지.)
					// param1 == 마을의 총 주식수, param2 == 발의자의 해당 마을 보유주식수.
					// pszCharParam1 == 시작시간, 
					// pszCharParam2 == 종료시간. 
					TCHAR str[2][50];
					ZeroMemory(str, sizeof(str));
					
					StringCchPrintf(str[0], sizeof(str[0]), TEXT("START => %03d-%02d-%02d %02d:%02d"),	
																	sendMsg.clGeneralMeetingInfo.clStartDate.uiYear,
																	sendMsg.clGeneralMeetingInfo.clStartDate.uiMonth,
																	sendMsg.clGeneralMeetingInfo.clStartDate.uiDay,
																	sendMsg.clGeneralMeetingInfo.clStartDate.uiHour,
																	sendMsg.clGeneralMeetingInfo.clStartDate.uiMinute );

					StringCchPrintf(str[1], sizeof(str[1]), TEXT("CLOSE => %03d-%02d-%02d %02d:%02d"),	
																	sendMsg.clGeneralMeetingInfo.clCloseDate.uiYear,
																	sendMsg.clGeneralMeetingInfo.clCloseDate.uiMonth,
																	sendMsg.clGeneralMeetingInfo.clCloseDate.uiDay,
																	sendMsg.clGeneralMeetingInfo.clCloseDate.uiHour,
																	sendMsg.clGeneralMeetingInfo.clCloseDate.uiMinute);


					cltCharServer* pclchar = NULL;
					if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GENERALMEETING, LOGCOMMAND_INDEX_GENERALMEETING_SUGGESTCHIEF, 
													0, pclchar, NULL, sendMsg.clGeneralMeetingInfo.siCandidatePersonID, 0, 0, NULL, 0, 0, 0, sendMsg.clGeneralMeetingInfo.siVillageUnique, 0, 0, 
													pclMsg->siTotalStock, pclMsg->siStockAmount, 0, 0, 0, str[0], str[1]);
				}
			}
			break;

		case 2: // 이미 주주총회가 진행 중임 
			{
				returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST_ALEADY;
			}
			break;

		case 3: // 발의자의 마을 보유 주식소가 3%미만이다
			{
				returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST_NOTENOUGHSTOCK;
			}
			break;

		case 4:	// 대행수 후보자로 지정된 캐릭터명이 잘못되었다 
			{
				returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST_WRONGCANDIDATE;
			}
			break;
		case 5:
			{
				returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST_ALREDYCANDIDATE;
			}
			break;
		case 6:
			{
				returnval = SRVAL_GENERALMEETING_FAIL_SUGGEST_CHIEFLEVEL;
			}
			break;
	}
	
	if( pclCM->IsValidID(id) ) 
	{	
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_VOTE( sPacketHeader *pPacket )
{
	sDBResponse_GeneralMeeting_Vote *pclMsg = (sDBResponse_GeneralMeeting_Vote *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			return;

	SI32 returnval = SRVAL_GENERALMEETING_FAIL_VOTE;

	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) 
	{
		cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

		if( pclCityHall == NULL ) 
			return;

		pclCityHall->clGeneralMeetingInfo.Vote( pclMsg->siVoterPersonID, pclMsg->siVoteType );

		returnval = SRVAL_GENERALMEETING_SUCCESS_VOTE;

		cltGameMsgResponse_GeneralMeetingInfo sendMsg;

		sendMsg.clGeneralMeetingInfo = pclCityHall->clGeneralMeetingInfo;


		// 클라이언트로 정보를 보낸다. 
		if( pclCM->IsValidID(id) ) 
		{	
			cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_INFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

		// 투표수가 51%이상이면 주주총회를 종료한다 
		SI32 agree, opposit, total, notparticipate;
		
		agree			= pclCityHall->clGeneralMeetingInfo.GetTotalAgree();			//찬성
		opposit			= pclCityHall->clGeneralMeetingInfo.GetTotalOpposition();		//반대.
		notparticipate	= pclCityHall->clGeneralMeetingInfo.GetTotalNotParticipate();	//투표참 석반대인원.
		total			= pclCityHall->clGeneralMeetingInfo.siTotalStock;

		if (notparticipate >= total) 
		{
			sDBRequest_GeneralMeeting_CloseChief sendMsg( pclMsg->siVillageUnique, 2, &pclTime->clDate );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
		}

		if( total >= agree && agree > (total * 0.5) ) 
		{
			switch( pclCityHall->clGeneralMeetingInfo.siMatterType ) 
			{
				case 1:
					{
						sDBRequest_GeneralMeeting_CloseChief sendMsg( pclMsg->siVillageUnique, 1, &pclTime->clDate );
						pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
					}
					break;
			}
		} 
		else if( total >= opposit && opposit >= (total * 0.5) ) 
		{
			//투표인원의 전부가 반대를 하거나 정확하게 50%센트의 주주가 반대를 할경우
			//임용이 안되도록 수식을 조정하였습니다. [2007.07.10 성웅.]
			switch( pclCityHall->clGeneralMeetingInfo.siMatterType ) 
			{
				case 1:
					{
						sDBRequest_GeneralMeeting_CloseChief sendMsg( pclMsg->siVillageUnique, 2, &pclTime->clDate );
						pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

					}
					break;
			}
		}

		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 주주총회 투표
			// param1 == 투표타입.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GENERALMEETING, LOGCOMMAND_INDEX_GENERALMEETING_VOTE, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siVoteType, 0, 0, 0, 0, NULL, NULL);
		}
	}

	if( pclCM->IsValidID(id) ) 
	{	
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE( sPacketHeader *pPacket )
{
	sDBResponse_GeneralMeeting_AcceptCandidate *pclMsg = (sDBResponse_GeneralMeeting_AcceptCandidate *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			return;

	SI32 id = pclMsg->usCharID;

	if( pclMsg->siResult == 1 ) 
	{
		cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

		if( pclCityHall == NULL ) return;

		pclCityHall->clGeneralMeetingInfo.siAcceptCandidate = 1;

		//KHY - 1115 - 주주총회 발동 시점 변경. -
		//[진성] DB에 저장된 시간을 다시 세팅. 이미 되있지만 ㅋㅋ => 2008-7-21
		//pclCityHall->clGeneralMeetingInfo.clStartDate.MakeFullDateFromDateVary( pclMsg->siStartDate );	// DB에서 안줘~ T.T
		pclCityHall->clGeneralMeetingInfo.clCloseDate.Set( &pclMsg->clCloseDate ); // 투표종료 남은 시간.

		if( pclCM->IsValidID(id) ) 
		{
			cltGameMsgResponse_GeneralMeetingInfo sendMsg;

			sendMsg.clGeneralMeetingInfo = pclCityHall->clGeneralMeetingInfo;

			cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_INFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_GENERALMEETING_SUCCESS_ACCEPTCANDIDATE,  0, 0, pclCM->CR[id]->GetCharUnique());
		}	


		cltMsg clMsg( GAMEMSG_NOTICE_GENERALMEETING, pclMsg->siVillageUnique );
		pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

		SI32 index = 0;
		SI32 charid;

		for( int j = 0; j < 100; ++j )
		{
			charid = pclCM->GetIDFromPersonID(pclCityHall->clGeneralMeetingInfo.clVoter[ j ].siPersonID);

			if (charid > 0 && pclClient->pclCM->IsValidID(charid) && IsPC(charid))
			{
				if (pclCityHall)
				{
					// 주주총회 개최 중인 마을의 주주 이라면,
					cltGameMsgResponse_GeneralMeetingLetter sendMsg;

					if(    pclCityHall->clGeneralMeetingInfo.clVoter[ j ].siPersonID    == pclCM->CR[charid]->pclCI->GetPersonID()
						&& pclCityHall->clGeneralMeetingInfo.clVoter[ j ].siParticipate == 0 )
					{
						sendMsg.siVillageUnique[ 0 ] = pclMsg->siVillageUnique;
					}
					cltMsg clMsg( GAMEMSG_RESPONSE_GENERALMEETINGLETTER, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg );				
				}
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 주주총회 동의
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GENERALMEETING, LOGCOMMAND_INDEX_GENERALMEETING_ACCEPTCANDIDATE, 
				0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				0, 0, 0, 0, 0, NULL, NULL);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_CLOSE_CHIEF( sPacketHeader *pPacket )
{
	sDBResponse_GeneralMeeting_CloseChief *pclMsg = (sDBResponse_GeneralMeeting_CloseChief *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			
		return;

	if( pclMsg->siResult == 1 )
	{
		cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

		if( pclCityHall == NULL ) 
			return;

		pclCityHall->clGeneralMeetingInfo.siMatterType = 0;

		//[진성] 취임 찬성. => 2008-7-21
		if( pclMsg->siVoteResult == 1 ) 
		{
			// 취임 일자를 갱신 한다 
			cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
			pclStrInfo->clCityhallStrInfo.clChiefDate.Set( &pclMsg->clDate );


			// 신분을 업데이트 한다. 
			SI32 personid = pclMsg->siChiefPersonID;

			cltSimpleRank newrank;
			newrank.Set( RANKTYPE_CITYHALL, pclMsg->siVillageUnique );

			cltSimplePerson newperson;
			newperson.Set( pclMsg->siChiefPersonID, pclMsg->szChiefName );

			if(personid)
			{
				//---------------------------------------------
				// 임명될 사람이 기존에 다른 신분을 가지고 있었다면 해제한다 
				//---------------------------------------------
				cltRank *pclTempRank = pclRankManager->GetRankInfo( pclMsg->siChiefPersonID );

				if( pclTempRank ) 
				{
					pclTempRank->Init();
				}

				//---------------------------------------------
				// 기존 신분의 사용자를 확보한다. 
				//---------------------------------------------
				cltRank* pclrank = pclRankManager->GetRank( RANKTYPE_CITYHALL, pclMsg->siVillageUnique );
				cltRank cloldrank;
				cloldrank.Set(pclrank);

				//---------------------------------------------------------------------------------
				// 기존 신분자와 새로운 신분자가 다르다면 해고당한 것이므로 쪽지를 보내 통보한다. 
				//---------------------------------------------------------------------------------
				SI32 oldpersonid = cloldrank.clPerson.GetPersonID();
				if(oldpersonid != personid)
				{
					cltLetterMsg_Fire clletter( &cloldrank.clRank,&pclTime->clDate);

					// 쪽지를 DB나 클라이언트에게 보낸다. 
					SendLetterMsg(oldpersonid, (cltLetterHeader*)&clletter);
					
					SI32 oldmayorid = pclCM->GetIDFromPersonID(oldpersonid);
					if(pclCM->IsValidID(oldmayorid))
					{
						pclCM->CR[oldmayorid]->pclCI->clCharRank.Init();
						pclCM->CR[oldmayorid]->SetUpdateSwitch(UPDATE_RANK, true, 0);
					}
				}
				

				//-------------------------------------------
				// 신분을 업데이트 한다. 
				//-------------------------------------------
				pclRankManager->SetRank(&newrank, &newperson);
				
				//-------------------------------------------
				// 임명된 사람에게 쪽지를 보낸다. 
				//-------------------------------------------
				cltLetterMsg_BeRank clletter( &newrank,&pclTime->clDate);
				// 쪽지를 DB나 클라이언트에게 보낸다. 
				SI32 letterid = SendLetterMsg(personid, (cltLetterHeader*)&clletter);
				if(pclCM->IsValidID(letterid))
				{
					pclCM->CR[letterid]->pclCI->clCharRank.Set( &newrank );
					pclCM->CR[letterid]->SetUpdateSwitch(UPDATE_RANK, true, 0);
				}
				
			}
		}
		//[진성] 취임 반대. => 2008-7-21
		else
		{
			cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

			if( pclCityHall == NULL ) 
				return;

			pclCityHall->clGeneralMeetingInfo.siMatterType = 0;
		}

		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 18] // 주주총회 종료.
			// pszParam1 == 게임시간.
			TCHAR szCharParam1[50] = {'\0', };
			StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("%03d-%02d-%02d %02d:%02d"), 
				pclMsg->clDate.uiYear, 
				pclMsg->clDate.uiMonth, 
				pclMsg->clDate.uiDay,
				pclMsg->clDate.uiHour, 
				pclMsg->clDate.uiMinute);

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GENERALMEETING, LOGCOMMAND_INDEX_GENERALMEETING_CLOSECHIEF, 
				0, NULL, NULL, pclMsg->siChiefPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				0, 0, 0, 0, 0, szCharParam1, NULL);
		}
	}   
	else if( pclMsg->siResult == 2 ) 
	{
		cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

		if( pclCityHall == NULL ) 
			return;

		pclCityHall->clGeneralMeetingInfo.siMatterType = 0;
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GENERALMEETING_SETVOTERSTATE( sPacketHeader *pPacket )//기권상황추가필요.
{
	sDBResponse_GeneralMeeting_SetVoterState *pclMsg = (sDBResponse_GeneralMeeting_SetVoterState *)pPacket;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siVillageUnique <= 0 || pclMsg->siVillageUnique >= MAX_VILLAGE_NUMBER )			return;

	SI32 id = pclMsg->usCharID;
	SI32 totalstock = pclCityHallManager->clGeneralMeetingInfo.siTotalStock;

	if( pclCM->IsValidID(id) == false ) return;

	if( pclMsg->siResult == 1 )
	{
		SI32 villageunique = pclMsg->siVillageUnique;
		if( villageunique == 0 ) return;

		cltCityHall *pCityHall = ((cltCityHall *)(pclCityHallManager->pclStruct[ villageunique ]));
		cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ pclMsg->siVillageUnique ];

		SI32 total;

		total = pclCityHall->clGeneralMeetingInfo.siTotalStock;

		if( pCityHall == NULL ) return;
		if( pCityHall->clGeneralMeetingInfo.siMatterType == 1 &&
			pCityHall->clGeneralMeetingInfo.siAcceptCandidate == 1 ) 
		{
			// 주주총회 중이라면,
			for( int j = 0; j < 100; ++j )
			{
				if( pCityHall->clGeneralMeetingInfo.clVoter[ j ].siPersonID == pclMsg->siPersonID && 
					pCityHall->clGeneralMeetingInfo.clVoter[ j ].siParticipate == 0 )
				{

					pCityHall->clGeneralMeetingInfo.clVoter[ j ].siParticipate = pclMsg->siParticipate;
					break;
				}
			}
		}

		if ((pCityHall->clGeneralMeetingInfo.siTotalStock)*0.5
			<= pCityHall->clGeneralMeetingInfo.GetTotalNotParticipate())
		{
			{
				sDBRequest_GeneralMeeting_CloseChief sendMsg( pclMsg->siVillageUnique, 2, &pclTime->clDate );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
		}//주주총회가 진행중이고 50%이상이 투표기권하면 투표종료[2007.07.30 손성웅]

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 주주총회 참여 여부를 설정한다
			// param1 == 참여여부
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))	pclchar = (cltCharServer*)pclCM->CR[id];
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GENERALMEETING, LOGCOMMAND_INDEX_GENERALMEETING_VOTERSTATESET, 
				0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
				pclMsg->siParticipate, 0, 0, 0, 0, NULL, NULL);
		}

		cltGameMsgResponse_GeneralMeetingParticipate sendMsg( pclMsg->siVillageUnique, pclMsg->siParticipate );

		cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_PARTICIPATE, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// 마을 상단의 DB 상태 정보를 갱신한후 처리한다.
//#if defined(_CITYHALL_BANKRUPTCY)
void cltServer::DoMsg_DBMSG_RESPONSE_CITYHALL_STATE_CHANGED( sPacketHeader *pPacket )
{
	sDBReponse_CityHall_State_Changed *pclMsg = (sDBReponse_CityHall_State_Changed *)pPacket;

	cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
	if( pclStrInfo == NULL )				return;

	if(pclMsg->siResult == 1)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 마을 상태 변경.							
			// param1 == 이전상태. 
			// param2 == 현재상태.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_SATATECHANGE, 
								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
								pclMsg->usPrevStatus, pclMsg->usCurStatus, 0, 0, 0, NULL, NULL);
		}
	}

	if( pclMsg->siResult == 1 )
	{
		switch( pclMsg->usCurStatus )
		{
		case 	CITYHALL_BEING_BANKRUPT	:		// DB에 마을 파산 상태 저장 완료
			{
				if( pclMsg->usPrevStatus != CITYHALL_BEING_NORMAL )
				{
					pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_NORMAL;
					break;
				}
                pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_BANKRUPT;

				// 특정 주주 들에게 알림
				// 해당 주주들의 PersonID를 DB에 요청한다. => Letter Msg로 알림
				sDBRequest_Notify_Cityhall_Bankruptcy clMsg( pclMsg->siVillageUnique, cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTED );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			}
			break;
		case 	CITYHALL_BEING_INITIALIZED	:		// DB에 마을 초기화 상태 저장 완료
			{
				// 마을 주식이 전부 초기화 되야 비로소 상단 상태가 초기화 된다.
				if ( pclStrInfo->clCityhallStrInfo.siTotalStockAmount != 0 )
				{
					pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_BANKRUPT;
					return;
				}

				if( pclMsg->usPrevStatus != CITYHALL_BEING_BANKRUPT )
				{
					pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_BANKRUPT;
					break;
				}

				pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_INITIALIZED;

				return;
			}
			break;
		case 	CITYHALL_BEING_STABLE		:		// DB에 마을 안정기 상태 저장 완료 , 안정기에서부터는 주식 공모 및 주식 소유가 가능하다.
			{
				if( pclMsg->usPrevStatus != CITYHALL_BEING_INITIALIZED )
				{
					pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_INITIALIZED;
					break;
				}

				pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_STABLE;
			}
			break;
		case 	CITYHALL_BEING_NORMAL		:		// DB에 마을 정상 상태 저장 완료
			{
				// 정상 => 정상으로 바뀔 경우는 이리로 안온다..
				if( pclMsg->usPrevStatus != CITYHALL_BEING_STABLE )
				{
                    pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_STABLE;					
					break;
				}

				pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_NORMAL;
			}
			break;
		}
	}
	else	// error 발생시 상단 상태를 이전 상태로 되돌림
	{
		// DB에 저장된 상태와 이전 상태가 다르다면
		// pclMsg->usCurStatus : DB에 실제 저장된 이전 값이 리턴
		// pclMsg->usPrevStatus : 서버에서 보낸 usPrevStatus가 그대로 리턴됨
		pclStrInfo->clCityhallStrInfo.usCurStatus = pclMsg->usCurStatus;
	}
}
//#endif

//#if defined(_CITYHALL_BANKRUPTCY)
// 1번만 공지
void cltServer::DoMsg_DBMSG_RESPONSE_NOTIFY_CITYHALL_BANKRUPTCY( sPacketHeader *pPacket )
{
	sDBReponse_Notify_Cityhall_Bankruptcy *pclMsg = (sDBReponse_Notify_Cityhall_Bankruptcy *)pPacket;

	if( pclMsg->siResult == 1 )
	{
		for( int i = 0; i < MAX_BANKRUPT_STOCKHOLDER_NUMBER_TO_BE_NOTIFIED; ++i ) {

			if( pclMsg->siPersonID[ i ] == 0 ) continue;

			switch( pclMsg->usType )
			{
			case cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTCY_ALERT:
				{
					// 6개월 후 파산 함을 통보
					cltDate cldate;
					cldate.Set( &(pclTime->clDate) );
					cldate.AddMonth( CITYHALL_BANKRUPTCY_NOTIFY_AFTER_MONTHS );

					// 해당 시각에 파산 예정임을 공지함
					cltLetterMsg_VillageNameAndDate clletter( pclMsg->siVillageUnique, &cldate, cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTCY_ALERT );
					SendLetterMsg(pclMsg->siPersonID[ i ], (cltLetterHeader*)&clletter );
				}
				break;
			case cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTED:
				{
					// 현재 시각으로 파산하여 증자 입찰 주식 보상 했음을 통지
					cltLetterMsg_VillageNameAndDate clletter( pclMsg->siVillageUnique, &(pclTime->clDate), cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTED );
					SendLetterMsg(pclMsg->siPersonID[ i ], (cltLetterHeader*)&clletter );
				}
				break;
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_DONATEVILLAGEMONEY( sPacketHeader *pPacket )
{
	sDBResponse_DonateVillageMoney *pclMsg = (sDBResponse_DonateVillageMoney *)pPacket;

	SI32 id = pclMsg->siCharID;
	if( false == pclCM->IsValidID(id) )
		return;

	if( pclMsg->siResult != 1 )
		return;

	cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );

	// 성공이면 건물 보유자금 갱신 
	if( pStrInfo )
	{
		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siChangedMyMoney );
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

		pStrInfo->clCityhallStrInfo.clProfitMoney.Set( pclMsg->siChangedVillageProfitMoney );

		cltMoney clMoney;
		clMoney.Set( pclMsg->siDonateMoney );

		cltMoney clCityHallMoney; 
		clCityHallMoney.Set( pclMsg->siChangedVillageProfitMoney );

		pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_DONATEMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney);

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique , RANKTYPE_CITYHALL, 
			INOUTCODE_DONATEMONEY, &pclTime->clDate, &clMoney, &clCityHallMoney );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [로그추가 : 황진성 2007. 10. 18] // 관청 보유자금 입출금 내역.				
		//	// param1 == 입출금 금액, param2 == 상단보유자금, pszParam1 == 시간.
		//	// charparam1 == 게임시간
		//	TCHAR szCharParam1[50];
		//	StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("%03d-%02d-%02d %02d:%02d"), 
		//																pclTime->clDate.uiYear, 
		//																pclTime->clDate.uiMonth, 
		//																pclTime->clDate.uiDay,
		//																pclTime->clDate.uiHour, 
		//																pclTime->clDate.uiMinute);
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		//									INOUTCODE_DONATEMONEY, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		//									clMoney.itMoney, clCityHallMoney.itMoney, 0, 0, 0, szCharParam1, NULL);
		//}


		cltGameMsgResponse_DonateVillageMoney clDonateVillageMoney( pclMsg->siVillageUnique, pclMsg->siDonateMoney );
		cltMsg clMsg(GAMEMSG_RESPONSE_DONATEVILLAGEMONEY, sizeof( clDonateVillageMoney ), (BYTE*)&clDonateVillageMoney );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		sDBRequest_DonateMemberList clMsg2( id, pclMsg->siVillageUnique, 1 );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2 );
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_DONATEMEMBERLIST( sPacketHeader *pPacket )
{
	sDBResponse_DonateMemberList *pclMsg = (sDBResponse_DonateMemberList *)pPacket;

	SI32 id = pclMsg->siCharID;
	if( false == pclCM->IsValidID(id) )
		return;

	if( pclMsg->siResult != 1 )
		return;

	cltGameMsgResponse_DonateMemberList clDonateMemberList( pclMsg->sDonateMemberList, pclMsg->siMaxPage );
	cltMsg clMsg(GAMEMSG_RESPONSE_DONATEMEMBERLIST, sizeof( clDonateMemberList ), (BYTE*)&clDonateMemberList );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}
// 파산 후 마을 여러 정보 초기화는 서버 점검시 수작업으로 한다.
//void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGE_INITIALIZATION( sPacketHeader *pPacket )
//{
//	sDBRepose_Village_Initialization *pclMsg = (sDBRepose_Village_Initialization*)pPacket;
//
//	if( pclMsg->siResult == 1 )
//	{
//		cltStrInfo *pclStrInfo = GetStrInfo( pclMsg->siVillageUnique, RANKTYPE_CITYHALL );
//
//		if( pclStrInfo == NULL ) return;
//
//		switch( pclMsg->usCurStatus )
//		{
//		case 	CITYHALL_HAS_BEEN_INITIALIZED	:		// DB에 마을 파산 상태 저장 완료
//			{
//				pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_INITIALIZED;
//			}
//			break;
//		default:
//			{
//				pclStrInfo->clCityhallStrInfo.usCurStatus = pclMsg->usPrevStatus;
//			}
//			break;
//		}
//	}
//	else
//	{
//		pclStrInfo->clCityhallStrInfo.usCurStatus = pclMsg->usPrevStatus;
//	}
//}

//#endif


void cltServer::DoMsg_DBMSG_RESPONSE_EVENT_INSERTMATERIAL( sPacketHeader *pPacket )
{
	sDBResponse_Event_InsertMaterial *pclMsg = (sDBResponse_Event_InsertMaterial *)pPacket;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)							return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar == NULL)											return;
	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID)		return;

	cltGameMsgResponse_Event_InsertMaterial clRet( pclMsg->siResult );
	cltMsg clMsgRet(GAMEMSG_RESPONSE_EVENT_INSERTMATERIAL, sizeof( clRet ), (BYTE*)&clRet );
	pclchar->SendNetMsg((sPacketHeader*)&clMsgRet);

	if( pclMsg->siResult == 1)
	{
		TCHAR szCharParam1[50];
		TCHAR szCharParam2[50];

		StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("%d(%d)[%d], %d(%d)[%d], %d(%d)[%d], %d(%d)[%d], %d(%d)[%d]"), 
					pclMsg->clMaterial[0].siItemUnique, pclMsg->clMaterial[0].siUseItemNum, pclMsg->clMaterial[0].siUseItemNum,
					pclMsg->clMaterial[1].siItemUnique, pclMsg->clMaterial[1].siUseItemNum, pclMsg->clMaterial[1].siUseItemNum,
					pclMsg->clMaterial[2].siItemUnique, pclMsg->clMaterial[2].siUseItemNum, pclMsg->clMaterial[2].siUseItemNum,
					pclMsg->clMaterial[3].siItemUnique, pclMsg->clMaterial[3].siUseItemNum, pclMsg->clMaterial[3].siUseItemNum,
					pclMsg->clMaterial[4].siItemUnique, pclMsg->clMaterial[4].siUseItemNum, pclMsg->clMaterial[4].siUseItemNum );

		StringCchPrintf(szCharParam2, sizeof(szCharParam2), TEXT("%d(%d)[%d], %d(%d)[%d], %d(%d)[%d], %d(%d)[%d], %d(%d)[%d]"), 
					pclMsg->clMaterial[5].siItemUnique, pclMsg->clMaterial[5].siUseItemNum, pclMsg->clMaterial[5].siUseItemNum,
					pclMsg->clMaterial[6].siItemUnique, pclMsg->clMaterial[6].siUseItemNum, pclMsg->clMaterial[6].siUseItemNum,
					pclMsg->clMaterial[7].siItemUnique, pclMsg->clMaterial[7].siUseItemNum, pclMsg->clMaterial[7].siUseItemNum,
					pclMsg->clMaterial[8].siItemUnique, pclMsg->clMaterial[8].siUseItemNum, pclMsg->clMaterial[8].siUseItemNum,
					pclMsg->clMaterial[9].siItemUnique, pclMsg->clMaterial[9].siUseItemNum, pclMsg->clMaterial[9].siUseItemNum );

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_EVENT_INSERTMATERIAL, 0,
							pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->m_siVillageUnique, 0, 0, 
							pclMsg->m_siConstructType, 0, 0, 0, 0,
							szCharParam1, szCharParam2);
	}

	// [춘기] 엔펠리스앞 월드 트리의 경우에는 재료 갯수의 유무를 따져서 재료가 다 채워졌으면 완료 메시지를 요청한다.
	if(pclMsg->m_siConstructType == EVENT_STRUCTURE_WORLDCHRISTMASTREE)
	{
		cltEventStructureUnit* pEventStruct = pclEventStructure->GetEventStructure(pclMsg->m_siConstructType);
		if( pEventStruct == NULL )						return;

		if(pclMsg->clMaterial[0].siUseItemNum == pEventStruct->clMaterial[0].siNeedNum
			&& pclMsg->clMaterial[1].siUseItemNum == pEventStruct->clMaterial[1].siNeedNum && pclMsg->clMaterial[2].siUseItemNum == pEventStruct->clMaterial[2].siNeedNum
			&& pclMsg->clMaterial[3].siUseItemNum == pEventStruct->clMaterial[3].siNeedNum && pclMsg->clMaterial[4].siUseItemNum == pEventStruct->clMaterial[4].siNeedNum
			&& pclMsg->clMaterial[5].siUseItemNum == pEventStruct->clMaterial[5].siNeedNum)
		{
			SI16	siItemUnique[MAX_MATERIAL_NUMBER];	// 건설 재료 정보
			SI16	siItemNum[MAX_MATERIAL_NUMBER];		// 건설 재료 정보

			// 건설 재료에서 필요개수를 찾아 채운다.
			for(int i = 0; i < MAX_MATERIAL_NUMBER; i++)
			{
				cltMaterialInfo* pMaterial = &pEventStruct->clMaterial[i];
				if(pMaterial == NULL)					
					continue;

				siItemUnique[i] = pMaterial->siUnique;
				siItemNum[i]    = pMaterial->siNeedNum;
			}

			sDBRequest_Event_MaterialComplete clMsg( id, pclchar->pclCI->GetPersonID(), VILLAGE_NONE , pclMsg->m_siConstructType,
				siItemUnique, siItemNum, 0, NULL, false );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

			// 다시 넣을수 있게 재료를 초기화 시킨다.
			//DBStructMaterialInfo clinfo[MAX_MATERIAL_NUMBER];

			//for(i=0; i<MAX_MATERIAL_NUMBER; i++)
			//{
			//	clinfo[i].siItemUnique	= pEventStruct->clMaterial[i].siUnique;
			//	clinfo[i].siNeedNum		= pEventStruct->clMaterial[i].siNeedNum;
			//	clinfo[i].siUseItemNum	= 0;
			//	clinfo[i].siStgPos		= -1;
			//}

			//sDBRequest_Event_InsertMaterial clMsg2( id, pclchar->pclCI->GetPersonID(), pclMsg->m_siVillageUnique , pclMsg->m_siConstructType, clinfo );
			//pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);
		}
	}
	// 재료리스트를 다시 요청한다.
	sDBRequest_Event_MaterialList clMsg( id, pclchar->pclCI->GetPersonID(), pclMsg->m_siVillageUnique, pclMsg->m_siConstructType );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_EVENT_MATERIALLIST( sPacketHeader *pPacket )
{
	sDBResponse_Event_MaterialList *pclMsg = (sDBResponse_Event_MaterialList *)pPacket;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)							return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar == NULL)											return;
	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID)		return;

	cltGameMsgResponse_Event_MaterialList clMaterialList( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, pclMsg->m_siItemUnique, pclMsg->m_siItemNum );
	cltMsg clMsg(GAMEMSG_RESPONSE_EVENT_MATERIALLIST, sizeof( clMaterialList ), (BYTE*)&clMaterialList );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_EVENT_MATERIALCOMPLETE( sPacketHeader *pPacket )
{
	sDBResponse_Event_MaterialComplete *pclMsg = (sDBResponse_Event_MaterialComplete *)pPacket;

#ifdef _DEBUG
	if(-200 > pclMsg->siResult)
	{
		MessageBox(NULL, "DB에 이미 넣은 아이템이 내가 넣어서 만든 아이템이랑 틀려.", "알림",  NULL);
		return;
	}
#endif

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id) == false)							return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar == NULL)											return;
	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID)		return;

	// 데이터 검증 - by LEEKH 2008.02.13
	if( pclMsg->siOutputPos < 0 || pclMsg->siOutputPos >= MAX_ITEM_PER_PERSON )		return;

	if( pclMsg->siOutputPos > 0 &&
		pclMsg->clItem.siUnique > 0 )
	{
		//--------------------------
		// 인벤을 업데이트 한다. 
		//--------------------------
		pclchar->pclCI->clCharItem.clItem[pclMsg->siOutputPos].Set(&pclMsg->clItem);

		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siOutputPos, PERSONITEMINFO_OPTION_NONE, pclMsg->siAddedNum, 0, &pclchar->pclCI->clCharItem.clItem[pclMsg->siOutputPos], 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}

	switch(pclMsg->m_siConstructType)
	{
	case EVENT_STRUCTURE_TREE:		// 트리용 나무
		{
			SI32 PineTreeKind		= pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE"));//기존의 변신전 나무
			SI32 ChristmastreeKind	= pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"));//크리스마스 트리 카인드 찾기.

			//NPC 해고를
			SI32 siIndex = 0;

			// npc 슬롯이 1번이다.
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(pclMsg->m_siVillageUnique);
			if( pclinfo == NULL ) 
				return;


			// 1단계 동상의 슬롯을 찾는다.
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if (pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == PineTreeKind ) 
				{
					siIndex = i;
					break;
				}
			}
						
            if( pclMsg->siResult == 1 && siIndex >= 0 )
			{
				// NPC 해고를 서버에 요청한다 
				sDBRequest_HireNPC clMsg( id, pclMsg->m_siVillageUnique, siIndex, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

				//NPC 고용 요청
				sDBRequest_HireNPC clHireMsg( id, pclMsg->m_siVillageUnique, siIndex, ChristmastreeKind, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );
				
				cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, ChristmastreeKind );
				cltMsg clCompleteMsg(GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
				pclCM->SendAllPersonMsg((sPacketHeader*)&clCompleteMsg);
			}
		}
		break;
	case EVENT_STRUCTURE_CHRISTMASTREEBASE:		// 트리용 나무
		{
			SI32 PineTreeKind		= pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREEBASE"));//기존의 변신전 나무
			SI32 ChristmastreeKind	= pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"));//크리스마스 트리 카인드 찾기.

			//NPC 해고를
			SI32 siIndex = 0;

			// npc 슬롯이 1번이다.
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(pclMsg->m_siVillageUnique);
			if( pclinfo == NULL ) 
				return;


			// 1단계 동상의 슬롯을 찾는다.
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if (pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == PineTreeKind ) 
				{
					siIndex = i;
					break;
				}
			}

			if( pclMsg->siResult == 1 && siIndex >= 0 )
			{
				// NPC 해고를 서버에 요청한다 
				sDBRequest_HireNPC clMsg( id, pclMsg->m_siVillageUnique, siIndex, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

				//NPC 고용 요청
				sDBRequest_HireNPC clHireMsg( id, pclMsg->m_siVillageUnique, siIndex, ChristmastreeKind, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );

				cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, ChristmastreeKind );
				cltMsg clCompleteMsg(GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
				pclCM->SendAllPersonMsg((sPacketHeader*)&clCompleteMsg);
			}
		}
		break;

	case EVENT_STRUCTURE_HERO_BRONZE:		// 영웅동상.
		{
			// 1단계 동상.
			SI32 HeroBonzeBase		= pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUEBASE"));
			// 2단계 동상.
			SI32 HeroBonzeComplete	= pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"));
			SI32 siIndex = -1;

			// npc 슬롯이 1번이다.
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(pclMsg->m_siVillageUnique);
			if( pclinfo == NULL ) 
				return;

		
			// 1단계 동상의 슬롯을 찾는다.
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if (pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == HeroBonzeBase ) 
				{
					siIndex = i;
					break;
				}
			}
			

			if( pclMsg->siResult == 1 && siIndex >= 0 )
			{
				// NPC 해고를 서버에 요청한다 
				sDBRequest_HireNPC clMsg( id, pclMsg->m_siVillageUnique, siIndex, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

				//NPC 고용 요청
				sDBRequest_HireNPC clHireMsg( id, pclMsg->m_siVillageUnique, siIndex, HeroBonzeComplete, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );

				cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, HeroBonzeComplete );
				cltMsg clCompleteMsg(GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
				pclCM->SendAllPersonMsg((sPacketHeader*)&clCompleteMsg);
			}
		}
		break;
	case EVENT_STRUCTURE_HALLOWEEN:		// 할로윈 촛대 //Switch_HalloweenEvent
		{
			// 할로윈 촛대 베이스
			SI32 HalloweenBase		= pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENBASE"));
			// 2단계 동상.
			SI32 HalloweenComplete	= pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEEN"));
			SI32 siIndex = -1;

			// npc 슬롯이 1번이다.
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(pclMsg->m_siVillageUnique);
			if( pclinfo == NULL ) 
				return;


			// 1단계 동상의 슬롯을 찾는다.
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if (pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == HalloweenBase ) 
				{
					siIndex = i;
					break;
				}
			}


			if( pclMsg->siResult == 1 && siIndex >= 0 )
			{
				// NPC 해고를 서버에 요청한다 
				sDBRequest_HireNPC clMsg( id, pclMsg->m_siVillageUnique, siIndex, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

				//NPC 고용 요청
				sDBRequest_HireNPC clHireMsg( id, pclMsg->m_siVillageUnique, siIndex, HalloweenComplete, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );

				cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, HalloweenComplete );
				cltMsg clCompleteMsg(GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
				pclCM->SendAllPersonMsg((sPacketHeader*)&clCompleteMsg);
			}
		}
		break;
	case EVENT_STRUCTURE_NEWYEARTREEBASE:
		{
			// 할로윈 촛대 베이스
			SI32 NewYearTreeBase		= pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREEBASE"));
			// 2단계 동상.
			SI32 NewYearTreeComplete	= pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"));
			SI32 siIndex = -1;

			// npc 슬롯이 1번이다.
			cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(pclMsg->m_siVillageUnique);
			if( pclinfo == NULL ) 
				return;


			// 1단계 동상의 슬롯을 찾는다.
			for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
			{
				if (pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == NewYearTreeBase ) 
				{
					siIndex = i;
					break;
				}
			}

			if( pclMsg->siResult == 1 && siIndex >= 0 )
			{
				// NPC 해고를 서버에 요청한다 
				sDBRequest_HireNPC clMsg( id, pclMsg->m_siVillageUnique, siIndex, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

				//NPC 고용 요청
				sDBRequest_HireNPC clHireMsg( id, pclMsg->m_siVillageUnique, siIndex, NewYearTreeComplete, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
				((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clHireMsg );

				cltGameMsgResponse_Event_CompleteBuild clComplete( pclMsg->m_siVillageUnique, pclMsg->m_siConstructType, NewYearTreeComplete );
				cltMsg clCompleteMsg(GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD, sizeof( clComplete ), (BYTE*)&clComplete );
				pclCM->SendAllPersonMsg((sPacketHeader*)&clCompleteMsg);
			}
		}
		break;
	case EVENT_STRUCTURE_WORLDCHRISTMASTREE:
		{
			cltServer* pclserver = (cltServer*)pclClient;

			// 월드트리 1단계
			SI32 siWorldTreeKind01 = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"));
			// 월드트리 2단계
			SI32 siWorldTreeKind02 = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02"));

			// 1단계 트리를 없앤다.
			SI32 siWorldTreeID01 = 0;
			SI32 siIndex = 0;

			while(siWorldTreeID01 = pclClient->pclCM->GetCharOrder(siIndex))
			{
				siIndex++;

				if (siWorldTreeID01 > 0 && pclClient->pclCM->IsValidID(siWorldTreeID01))
				{
					cltCharServer* pclWorldTreeChar01 = (cltCharServer*)pclClient->pclCM->CR[siWorldTreeID01];

					if (pclWorldTreeChar01->GetKind() == siWorldTreeKind01 )
					{
						// 셀 내에서의 정보를 삭제한다. 
						pclClient->pclCM->CR[siWorldTreeID01]->DeleteInCell();
						pclCM->DeleteChar(siWorldTreeID01);
					}
				}
			}

			// 2단계 트리를 건설한다.
			pclserver->siWorldTreeID[0] = 0;
			pclserver->bWorldTreeConstruct = true;
			pclserver->siEventNPCId[3] = 0;

			// 해당정보를 서버에 저장한다.
			TCHAR szFileName[256] = "";
			StringCchPrintf(szFileName, sizeof(szFileName), TEXT("Config\\WorldTree.dat"));

			FILE *fp = fopen(szFileName, "w+");
			if( fp != NULL)
			{
				fprintf(fp, "%d\n", pclserver->bWorldTreeConstruct);
				fclose( fp );
			}		
		}
		break;
	case 1001:	// 크리스마스 트리
		{
			// 크리스마스 트리에서 선물을 지급했다.
			// DB작업으로 선물이 지급되었으므로 서버에서 할 일은 없다.
		}
		break;
	}

	cltGameMsgResponse_Event_MaterialComplete clResult( pclMsg->siResult, pclMsg->m_siVillageUnique, pclMsg->m_siConstructType );
	cltMsg clMsg(GAMEMSG_RESPONSE_EVENT_MATERIALCOMPLETE, sizeof( clResult ), (BYTE*)&clResult );
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_CITYHALL_SETINOUTMONEY(sPacketHeader* pPacket)
{
	// 로그를 남기기 위해 내용 추가함.
	// param1 = 입출금 건물타입, charparam1 == 게임시간
	sDBResponse_CityHallSetInOutMoney* pclMsg = (sDBResponse_CityHallSetInOutMoney*)pPacket;

	TCHAR szCharParam1[50];
	StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("%03d-%02d-%02d %02d:%02d"), 
		pclTime->clDate.uiYear, 
		pclTime->clDate.uiMonth, 
		pclTime->clDate.uiDay,
		pclTime->clDate.uiHour, 
		pclTime->clDate.uiMinute);

	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_INOUTMONEY, 
		pclMsg->siInOutCode, NULL, NULL, 0, pclMsg->clInOutMoney.itMoney, pclMsg->clCityHallMoney.itMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
		pclMsg->siStrType, 0, 0, 0, 0, szCharParam1, NULL);
}
//명예의 전달 리스트 패이지







