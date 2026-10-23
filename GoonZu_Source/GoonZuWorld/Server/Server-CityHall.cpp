//---------------------------------
// 2003/6/9 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"
#include "..\Server\Candidate\Candidate.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Cityhall.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\CommonLogic\MsgType-Structure.h"
#include "..\CommonLogic\Msg\MsgType-Village.h"
#include "..\CommonLogic\Msg\MsgType-Rank.h"
#include "MsgRval-Define.h"

#include "..\Server\Minister\MinisterBase.h"
#include "..\Server\Minister\MinisterMgr.h"
#include "..\Server\Minister\Minister-Hozo.h"

#include "../common/Item/ItemCommon/Item-EventStructure.h"
 
//-------------------------------------
// Common
//-------------------------------------0
#include "PersonInfo.h"
#include "..\Common\Event\Hunting\Hunting.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_INVESTLIST(cltMsg* pclMsg, SI32 id)
{
	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	// 투자자 정보를 구한다. 
	cltVillageInvestInfo* pMayorInfo = &pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.clInvestInfo;

	// 어떤 마을의 정보인지를 업데이트 한다. 
	pMayorInfo->siVillageUnique = villageunique;

	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg(GAMEMSG_RESPONSE_INVESTLIST, sizeof(cltVillageInvestInfo), (BYTE*)pMayorInfo);
	
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 신분에 임명한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_BECHIEF(cltMsg* pclMsg, SI32 id)
{

	cltRank* pclrank = (cltRank*)pclMsg->cData;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclrank->GetVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;
	
	//-----------------------------------------
	// 자격이 있는지 확인한다. 
	//-----------------------------------------
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 대행수 임명인 경우 Person이 마을의 최대주주인지 확인한다. 
	if(pclrank->clRank.siType == RANKTYPE_CITYHALL)
	{
		// 대행수는 임명할 수 없다.!!!!!
		return ;
		
		/*if(pclVillageManager->IsMostBeholder(villageunique, personid) == FALSE)
		{
			// 최대주주가 아니다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTBESTBEHOLDER,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}*/
	}
	// 기타 임명인 경우 대행수 인지 확인한다. 
	else
	{
		cltSimpleRank clrank(RANKTYPE_CITYHALL, villageunique);
		if( pclRankManager->IsRank(personid, &clrank) == FALSE)
		{
			// 대행수가 아니다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTMAYOR,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}

	//-----------------------------------------
	// 피임명자의 정보를 구한다. 
	//-----------------------------------------
	if(pclrank->GetPersonID() == 0)return ;

	// 피임명자의 데이터를 구한다.
	SI32 candidateindex = -1;
	cltSimplePerson* pclcandidateinfo = pclCityHallManager->pclStruct[villageunique]->GetCandidateInfoFromPersonID(pclrank->GetRankType(), pclrank->GetPersonID(), &candidateindex);
	if(pclcandidateinfo == NULL)
	{
		// 후보자 명단에 해당 캐릭터가 없다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_INVALIDCANDIDATE,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	
	//DB매니저에게 요청한다.
	sDBRequest_BeRank clMsg(id, pclrank->GetPersonID(), candidateindex, &pclrank->clRank);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}


// 후보로 등록신청한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_CANDIDATE(cltMsg* pclMsg, SI32 id)
{

	//-----------------------------------------------------
	// 입후보한 내역을 확인한다. 
	//-----------------------------------------------------
	cltSimpleRank* pclrank = (cltSimpleRank*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if(personid <= 0)return ;

	SI32 villageunique = pclrank->GetVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;

	// [영훈] 초보자 마을 : 초보자 마을은 길드만 소영주 신청 가능하고 그 신분이 수호천사가 아니면 안된다
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == villageunique )
		{
			if ( (RANKTYPE_GUILD != pclrank->GetRankType()) || (pclCM->CR[id]->pclCI->clBI.uiAngelSwitch == false) )
			{
				return;
			}
		}
	}
		
	//---------------------------------------------------
	// 후보 등록이 가능한 레벨인지 체크 한다 
	//---------------------------------------------------

	SI32 needlevel = pclCityHallManager->GetNeedLevelFromRankType( pclrank->GetRankType() );

	if( pclCM->CR[id]->pclCI->clIP.GetLevel() < needlevel ) {

		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAILCANDIDATE_LEVELERROR,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}

	//---------------------------------------------------
	// 이미 같은 직위의 후보로 등록이 되어 있다면 실패. 
	//---------------------------------------------------
	if(pclCityHallManager->pclStruct[villageunique] == NULL)
	{
		return ;
	}

	SI32 index = 0;
	if(pclCityHallManager->pclStruct[villageunique]->GetCandidateInfoFromPersonID(pclrank->GetRankType(), personid, &index) != NULL)
	{
		// 이미 같은 직위에 등록되어 있다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAILCANDIDATE_ALREADY,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------------------------
	// 해당 신분의 입후보자가 최대 수를 넘어섰다면 실패. 
	//---------------------------------------------------
	if(pclCityHallManager->pclStruct[villageunique]->GetCandidateNum(pclrank->GetRankType()) >= (MAX_CANDIDATE_NUMBER -1))
	{
		// 최대 입후보 가능자 수를 넘어섰다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAILCANDIDATE_OVERFLOW,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}


	//---------------------------------------------------
	// 이미 신분을 보유하고 있다면 실패. 
	//---------------------------------------------------
	if(pclCM->CR[id]->pclCI->clCharRank.GetRankType())
	{
		// 이미 다른 직위를 보유하고 있으면 실패.  
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAILCANDIDATE_ALREADYRANK,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	//---------------------------------------------------
	// DBManager에게 후보 등록을 요청한다. 
	//---------------------------------------------------
	sDBRequest_Candidate clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclrank);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CANCELCANDIDATE(cltMsg* pclMsg, SI32 id)
{
	// 마을의 신원을 확인한다. 
	SI32 villageunique = pclMsg->siData32[0];
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;


	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	

	// DBManager에게 요청한다. 
	sDBRequest_CancelCandidate clMsg(id, personid);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


}

// 신분 입후보자 명단을 요청한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_CANDIDATELIST(cltMsg* pclMsg, SI32 id)
{
	// 마을의 신원을 확인한다. 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank* pclrank = (cltSimpleRank*)pclMsg->cData;

	SI32 villageunique = pclrank->GetVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	//---------------------------------------
	// 적절한 자격이 있는지 확인한다. 
	//---------------------------------------
	// 대행수 임명이면 최대 주주여야 한다. 
	if(pclrank->siType == RANKTYPE_CITYHALL)
	{
		if(pclVillageManager->IsMostBeholder(villageunique, personid) == FALSE)
		{
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTBESTBEHOLDER,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}
	// 행수 임명이면 대행수여야 한다. 
	else
	{
		cltSimpleRank clrank(RANKTYPE_CITYHALL,villageunique);
		if( pclRankManager->IsRank(personid, &clrank) == FALSE)
		{
			// 대행수가 아니다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTMAYOR,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}
	
	
	SI32 i;
	SI32 sendmsgnum = 0;
	cltSimplePerson* pclPerson;
	for(i = 0;i< MAX_CANDIDATE_NUMBER;i++)
	{
		pclPerson = pclCityHallManager->pclStruct[villageunique]->GetCandidateInfoFromRef(pclrank->GetRankType(), i);
		
		if(pclPerson->GetPersonID() <= 0)continue;

		// 후보자 명단 보낸 개수를 파악한다. 
		sendmsgnum++;
		
		// 클라이언트로 cltCandidate데이터를 보내기 위해 포장한다. 
		cltGameMsgResponse_CandidateList clinfo(i, pclrank, pclPerson);
		cltMsg clMsg(GAMEMSG_RESPONSE_CANDIDATELIST, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}
	
	// 실제로 보낸 명단이 하나도 없는 경우, 
	if(sendmsgnum == 0)
	{
		// 후보자가 없음을 알린다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_NOCANDIDATE,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
	
}


void cltServer::DoMsg_GAMEMSG_REQUEST_DELCANDIDATE(cltMsg* pclMsg, SI32 id)
{

	// 마을의 신원을 확인한다. 
	cltSimpleRank* pclrank = (cltSimpleRank*)pclMsg->cData;
	SI32 villageunique = pclrank->GetVillageUnique();
	
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 returnval = 0;
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 최대주주인지 확인한다. 
	if(pclrank->siType == RANKTYPE_CITYHALL)
	{
		if(pclVillageManager->IsMostBeholder(villageunique, personid) == FALSE)
		{
			// 최대주주가 아니다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTBESTBEHOLDER,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}
	// 기타 임명인 경우 대행수 인지 확인한다. 
	else
	{
		cltSimpleRank clrank(RANKTYPE_CITYHALL,villageunique);
		if( pclRankManager->IsRank(personid, &clrank) == FALSE)
		{
			// 대행수가 아니다. 
			SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_NOTMAYOR,  0, 0, pclCM->CR[id]->GetCharUnique());
			return ;
		}
	}
	
	//DBManager에게 후보를 삭제할 것을 요청한다. 
	sDBRequest_DelCandidate clMsg(id, pclrank);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}


void cltServer::DoMsg_GAMEMSG_REQUEST_RESIGN(cltMsg* pclMsg, SI32 id)
{
	SI32 returnval = 0;

	//-------------------------------
	// 마을의 신원을 확인한다. 
	//-------------------------------
	cltSimpleRank* pclrank = (cltSimpleRank*)pclMsg->cData;

	SI32 villageunique = pclrank->GetVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID == 0)return ;

//#if defined(_CITYHALL_BANKRUPTCY)
	cltStrInfo* pclStrInfo = pclCityHallManager->GetStrInfo(villageunique);

	// 대행수의 사임인가
	if( pclStrInfo != NULL && pclrank->siType == 1 )
	{
		// 마을이 안정기 일 때는 대행수는 사임할 수 없다.
		if( pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_STABLE )
		{
			// 마을이 안정기 입니다.. => 대행수 사임 실패 => 알림
			cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( villageunique,  pclStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_CITYHALLCHIEF_RESIGN );
			cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			return;
		}
	}
//#endif

	//-------------------------------------
	// 대상자가 Chief인지 확인한다. 
	//-------------------------------------
	cltSimpleRank clrank(pclrank->GetRankType(),villageunique);
	if( pclRankManager->IsRank(PersonID, &clrank) == TRUE)
	{

		// Rank정보에서 대상자를 삭제한다. 
		sDBRequest_Resign clMsg(id, PersonID, &clrank);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		
		returnval = SRVAL_SUCCESSRESIGNMAYOR;
	}
	else
	{
		returnval = SRVAL_NOMAYOR;	// 대행수이 아니다. 
	}


	// Person에게 결과값을 보내준다. 
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(pclrank->GetRankType(), returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_APPLYRESIDENT( cltMsg *pclMsg, SI32 id )
{
	SI32 returnval = 0;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID == 0)return ;

	bool bCheckMaxResidentCount = true;

	// 초보자 마을은 대신이 설정한 최대 주민수를 체크하지 않는다. - by LEEKH 2008-05-04
	if( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true )
	{
		if( villageunique == Const_Beginner_Village )
		{
			bCheckMaxResidentCount = false;
		}
	}

	if( bCheckMaxResidentCount )
	{
		cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( villageunique );

		if( pStrInfo ) 
		{
			CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister( MINISTER_HOZO );
			if( pStrInfo->clCityhallStrInfo.siResidentNum > pclminister->m_siResidentNum )
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_EXCEEDMAXRESIDENT, 0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}
		}
	}

	cltCharServer *pclChar = (cltCharServer*)pclCM->CR[id];
	if ( NULL == pclChar )
	{
		return;
	}

	// 초보자 마을 : 초보자 마을은 가입퀘스트시에 자동가입되도록한다
	if ( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true )
	{
		SI16 siQuestType, siQuestUnique, siPara1, siPara2;
		SI32 siQuestClearCond	= QUEST_CLEAR_COND_APPLYVILLAGE;

		bool bQuestGoing		= pclChar->pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2);
		bool bTutorialVillage	= ( Const_Beginner_Village == villageunique );

		SI32 siLevel			= pclCM->CR[id]->pclCI->clIP.GetLevel();


		if( (true == bQuestGoing) && (true == bTutorialVillage) )
		{
			// 이미 다른 마을에 가입된 상태이거나 40레벨 이상이면 퀘스트만 클리어
			if ( (0 < pclChar->pclCI->clBI.siHomeVillage) || (Const_BV_Max_Resident_Level <= siLevel) )
			{
				pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_APPLYVILLAGE);

				return;
			}
			// 초보자 마을에 가입할 수 있는 레벨이라면 퀘스트 클리어 동시에 자동 가입
			else if ( Const_BV_Max_Resident_Level > siLevel )
			{
				sDBRequest_BeResident clMsg( id, PersonID, villageunique, &pclTime->clDate, true );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_APPLYVILLAGE);

				return;
			}
		}	// if( (true == bQuestGoing) && (true == bTutorialVillage) ) 종료
	}	// if ( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true ) 종료

	sDBRequest_ApplyResident clMsg( id, PersonID, villageunique, &pclTime->clDate );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_APPLYRESIDENT2( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_ApplyResident2* pclmsg = (cltGameMsgRequest_ApplyResident2*)pclMsg->cData;

	SI32 returnval = 0;

	// 캐릭터가 초대받은 마을을 구한다. 
	SI32 villageunique = pclmsg->siVillageUnique;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if(PersonID == 0)return ;

	bool bCheckMaxResidentCount = true;

	// 초보자 마을은 대신이 설정한 최대 주민수를 체크하지 않는다. - by LEEKH 2008-05-04
	if( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true )
	{
		if( villageunique == Const_Beginner_Village )
		{
			bCheckMaxResidentCount = false;
		}
	}

	if( bCheckMaxResidentCount )
	{
		cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( villageunique );

		if( pStrInfo ) 
		{
			CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister( MINISTER_HOZO );
			if( pStrInfo->clCityhallStrInfo.siResidentNum > pclminister->m_siResidentNum )
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_EXCEEDMAXRESIDENT, 0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}
		}
	}

	cltCharServer *pclChar = (cltCharServer*)pclCM->CR[id];
	if ( NULL == pclChar )
	{
		return;
	}

	// 초보자 마을 : 초보자 마을은 가입퀘스트시에 자동가입되도록한다
	if ( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true )
	{
		SI16 siQuestType, siQuestUnique, siPara1, siPara2;
		SI32 siQuestClearCond	= QUEST_CLEAR_COND_APPLYVILLAGE;

		bool bQuestGoing		= pclChar->pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2);
		bool bTutorialVillage	= ( Const_Beginner_Village == villageunique );

		SI32 siLevel			= pclCM->CR[id]->pclCI->clIP.GetLevel();


		if( (true == bQuestGoing) && (true == bTutorialVillage) )
		{
			// 이미 다른 마을에 가입된 상태이거나 40레벨 이상이면 퀘스트만 클리어
			if ( (0 < pclChar->pclCI->clBI.siHomeVillage) || (Const_BV_Max_Resident_Level <= siLevel) )
			{
				pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_APPLYVILLAGE);

				return;
			}
			// 초보자 마을에 가입할 수 있는 레벨이라면 퀘스트 클리어 동시에 자동 가입
			else if ( Const_BV_Max_Resident_Level > siLevel )
			{
				sDBRequest_BeResident clMsg( id, PersonID, villageunique, &pclTime->clDate, true );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				pclChar->DoQuestCheckAndAction(QUEST_CLEAR_COND_APPLYVILLAGE);

				return;
			}
		}	// if( (true == bQuestGoing) && (true == bTutorialVillage) ) 종료
	}	// if ( pclClient->IsCountrySwitch( Switch_TutorialVillage ) == true ) 종료

	// 영주에게 편지를 보낸다.
	cltRank* pclRank = pclRankManager->GetRank( RANKTYPE_CITYHALL, villageunique );
	if ( pclRank != NULL )
	{
		cltRank clRank;
		SI32	siPersonID = 0;
		SI32	siCharID   = 0;

		clRank.Set( pclRank );
		siPersonID = clRank.clPerson.GetPersonID();

		if( 0 < siPersonID )
		{
			SI32 siID = pclCM->GetIDFromPersonID( siPersonID );
			if( 0 >= siID )
			{
				cltLetterMsg_Invite_Applyresident clletter( pclmsg->szMentorName, (TCHAR*)pclChar->GetName() );
				sDBRequest_PushLetter clMsg( siPersonID, (cltLetterUnit*)&clletter );
				pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg );
			}
			else
			{
				cltLetterMsg_Invite_Applyresident clletter( pclmsg->szMentorName, (TCHAR*)pclChar->GetName() );
				SendLetterMsg(  siPersonID , (cltLetterHeader*)&clletter );
			}
		}
	}
	

	sDBRequest_ApplyResident clMsg( id, PersonID, villageunique, &pclTime->clDate, true );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CANCELRESIDENT( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_CancelResident *pclmsg = (cltGameMsgRequest_CancelResident *)pclMsg->cData;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;


	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE && personid != pclmsg->siPersonID )
	{
		return;
	}

	sDBRequest_CancelResident clMsg( id, pclmsg->siPersonID, villageunique, false );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_BERESIDENT( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_BeResident *pclmsg = (cltGameMsgRequest_BeResident *)pclMsg->cData;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;
	
	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( villageunique );

	if( pStrInfo ) {
		
		CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);

		if( pStrInfo->clCityhallStrInfo.siResidentNum > pclminister->m_siResidentNum ) {

			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_EXCEEDMAXRESIDENT, 0, 0, pclCM->CR[id]->GetCharUnique() );

			return;
		}
	}

	if( pclClient->pclVillageManager->IsVillageAtWar(villageunique) == TRUE)
	{

		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_VILLAGEATWAR, 0, 0, pclCM->CR[id]->GetCharUnique() );

		return;
	}


	sDBRequest_BeResident clMsg( id, pclmsg->siPersonID, villageunique, &pclTime->clDate, false );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_RESIDENTAPPLYERLIST( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_GetResidentApplyerList *pclmsg = (cltGameMsgRequest_GetResidentApplyerList *)pclMsg->cData;

	cltGameMsgResponse_GetResidentApplyerList retMsg;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	cltCityHall *pclCityHall = (cltCityHall *)(pclCityHallManager->pclStruct[ villageunique ]);

	int c = 0;
	for( int i = 0; i < MAX_RESIDENT_APPLYER; ++i ) {

		if( pclCityHall->clResidentApplyer[ i ].clApplyer.GetPersonID() ) {

			if( c >= MAX_RESIDENT_APPLYER )		continue;
			retMsg.clResidentApplyer[ c ].Set( &pclCityHall->clResidentApplyer[ i ] );

			c++;
		}

	}

	retMsg.count = c;

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_RESIDENTAPPLYERLIST, retMsg.GetTotalSize(), (BYTE*)&retMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);


}

void cltServer::DoMsg_GAMEMSG_REQUEST_RESIDENTLIST( cltMsg *pclMsg, SI32 id )
{

	cltGameMsgRequest_GetResidentList *pclmsg = (cltGameMsgRequest_GetResidentList *)pclMsg->cData;


	sDBRequest_GetResidentList clMsg( id, pclmsg->siVillageUnique, pclmsg->siSort );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);	


	/*
	cltGameMsgResponse_GetResidentList retMsg;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;

	cltCityHall *pclCityHall = (cltCityHall *)(pclCityHallManager->pclStruct[ villageunique ]);

	int c = 0;
	for( int i = 0; i < MAX_RESIDENT_NUMBER; ++i ) {

		if( pclCityHall->clResident[ i ].clPerson.GetPersonID() ) {

			retMsg.clResident[ c ].Set( &pclCityHall->clResident[ i ] );

			c++;
		}

	}

	retMsg.count = c;

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_RESIDENTLIST, retMsg.GetTotalSize(), (BYTE*)&retMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
	*/

}

// 주민 상세 정보 
void cltServer::DoMsg_GAMEMSG_REQUEST_RESIDENTINFO( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_GetResidentInfo *pclInfo = (cltGameMsgRequest_GetResidentInfo *)pclMsg->cData;
	
	
	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageUnique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageUnique <= 0 || villageUnique >= MAX_VILLAGE_NUMBER)	return ;

	sDBRequest_GetResidentInfo clMsg( id, villageUnique, pclInfo->szPersonName );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}


// 주민 점수 부여
void cltServer::DoMsg_GAMEMSG_REQUEST_SETRESIDENTSCORE( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_SetResidentScore *pclInfo = (cltGameMsgRequest_SetResidentScore *)pclMsg->cData;

	// 캐릭터가 위치한 마을을 구한다. 
	SI32 villageUnique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageUnique <= 0 || villageUnique >= MAX_VILLAGE_NUMBER)	return ;


	sDBRequest_SetResidentScore clMsg( villageUnique, pclInfo->siPersonId, pclInfo->siScore );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}


// 마을 주민 고용 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HIRENPC( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_HireNPC *pclInfo = (cltGameMsgRequest_HireNPC *)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}
	
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclchar->GetCurrentVillageUnique();
	if (villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)
	{
		return ;
	}
	
	SI32 personid = pclchar->pclCI->GetPersonID();
	// 권한이 있는가 
	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	UI16 slot = pclInfo->usSlot;
	if( slot < 0 || slot >= MAX_HIRED_NPC_NUM )
	{
		return;
	}

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);

	if( pclinfo == NULL ) return;

	// 해고이면
	if( pclInfo->siNPCKind == 0 ) {

		if( pclinfo->clCityhallStrInfo.clHiredNPC[ slot ].siNPCKind > 0 ) {

			sDBRequest_HireNPC clMsg( id, villageunique, slot, 0, 0, 0, &pclTime->clDate , HIRENPC_HIRENPC );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

			
		}
		return;
	}


	// 고용이면,

	// NPC들이 위치한 건물 슬롯에 건물이 지어져 있다면 NPC를 고용할 수 없다 

	// slot이 비어있는지 점검 
	if( pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.clStructureInfo.IsExist( 6 ) ) {
		// slot이 비어있지 않다면,
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_HIRENPC_BUILDSTRUCTURE, 0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	int i;

	for( i = 0; i < MAX_HIRED_NPC_NUM; ++i ) {

		if( pclinfo->clCityhallStrInfo.clHiredNPC[ i ].siNPCKind == pclInfo->siNPCKind ) break;
	}

	// 이미 고용된 것이면 return;
	if( i < MAX_HIRED_NPC_NUM )
	{
		return;
	}
	

//#if defined(_CITYHALL_BANKRUPTCY)
	// 마을 안정기 이므로 npc 고용이 무료이다.
	cltStrInfo* pclStrInfo = pclinfo;
	if( pclStrInfo->clCityhallStrInfo.usCurStatus != CITYHALL_BEING_STABLE )
	{
		//// 고용 가능한 마을 레벨인가
		//if( pclinfo->clCityhallStrInfo.siVillageLevel <
		//	pclKindInfoSet->GetHireVillageLevel( pclInfo->siNPCKind ) )
		//{
		//	SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_HIRENPC_VILLAGELEVEL, 0, 0, pclCM->CR[id]->GetCharUnique() );
		//	return;
		//}
	}
//#endif
	
	GMONEY gmPayment = pclKindInfoSet->GetHireBasicMoney( pclInfo->siNPCKind );

	gmPayment = gmPayment * ( pclItemManager->clItemStatistic.siNPCPriceRate / 100. );

	//#if defined(_CITYHALL_BANKRUPTCY)
	// 마을 안정기 이므로 npc 고용이 무료이다.
	if( pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_STABLE )
	{
		gmPayment = 0;
	}
	//#endif


	// 슬롯 번호에 NPC가 없을때
	if( slot >= 0 && slot < MAX_HIRED_NPC_NUM ) 
	{
		if( pclinfo->clCityhallStrInfo.clHiredNPC[ slot ].siNPCKind == 0 ) 
		{
			sDBRequest_HireNPC clMsg( id, villageunique, slot, pclInfo->siNPCKind, gmPayment, gmPayment, &pclTime->clDate , HIRENPC_HIRENPC );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
				
			return;
		}
	}

	// 위에서 슬롯번호에 NPC가 이미 고용되어 있을때 이곳에서 빈 슬롯을 찾아서 NPC를 고용한다
	for( i = 0; i < MAX_HIRED_NPC_NUM; ++i )
	{
		if( pclinfo->clCityhallStrInfo.clHiredNPC[ i ].siNPCKind == 0 ) 
		{
			sDBRequest_HireNPC clMsg( id, villageunique, i, pclInfo->siNPCKind, gmPayment, gmPayment, &pclTime->clDate , HIRENPC_HIRENPC );

			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
			
			return;			
		}
	}
	
}

// 마을에 고용된 주민 리스트 요청 
void cltServer::DoMsg_GAMEMSG_REQUEST_GETNPCLIST( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_GetNPCList *pclInfo = (cltGameMsgRequest_GetNPCList *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
	if(pclinfo == NULL)return ;

	// 응답 보내기
	cltGameMsgResponse_GetNPCList rspMsg;

	rspMsg.siVillageUnique = villageunique;

#ifdef _SAFE_MEMORY
	rspMsg.clHiredNPC.MemCpy(pclinfo->clCityhallStrInfo.clHiredNPC);
#else
	memcpy( rspMsg.clHiredNPC, pclinfo->clCityhallStrInfo.clHiredNPC, sizeof( cltHiredNPC ) * MAX_HIRED_NPC_NUM );
#endif

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_GETNPCLIST, sizeof( cltGameMsgResponse_GetNPCList ), (BYTE*)&rspMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
}

// NPC 급여 정보 요청 
void cltServer::DoMsg_GAMEMSG_REQUEST_NPCPAYMENT( cltMsg *pclMsg, SI32 id )
{

	cltGameMsgResponse_NPCPayment rspMsg;
	rspMsg.siCount = 0;

	SI32 kind;

	for( int i = 0; i < MAX_HIREDNPC_KIND_NUM; ++i ) {

		kind = pclHiredNPCPayManager->clHiredNPCPayInfo[ i ].siNPCKind;

		if( kind ) 
		{
			GMONEY gmPayment = pclKindInfoSet->GetHireBasicMoney( kind );
			gmPayment = gmPayment * (pclItemManager->clItemStatistic.siNPCPriceRate / 100.);

			if( rspMsg.siCount >= MAX_HIREDNPC_KIND_NUM )	continue;
			rspMsg.clHiredNPCPayInfo[ rspMsg.siCount ]				= pclHiredNPCPayManager->clHiredNPCPayInfo[ i ];
			rspMsg.clHiredNPCPayInfo[ rspMsg.siCount ].siPayment	= gmPayment;
			++rspMsg.siCount;
		}
	}

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_NPCPAYMENT, sizeof( cltGameMsgResponse_NPCPayment ), (BYTE*)&rspMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);
}

// 선전 포고 
void cltServer::DoMsg_GAMEMSG_REQUEST_DECLAREWAR( cltMsg *pclMsg, SI32 id )
{

	// 공성전 강제로 봉인중
	

	SI32 returnval = 0;

	if(  true == bForceStopVillageWarProcess	)
	{	
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTNATION;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return ;
	}

	SI32 targetVillage = pclMsg->siData32[ 0 ];
	if( targetVillage < 0 || targetVillage >= MAX_VILLAGE_NUMBER )		return;

	SI32 villageunique = pclCM->CR[id]->pclCI->clBI.siHomeVillage;
	if( villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER )		return;

	// [영훈] 초보자 마을 : 초보자 마을은 공성전 발동 못하게
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( (Const_Beginner_Village == villageunique) 
			|| (Const_Beginner_Village == targetVillage) 
		)
		{
			return;
		}
	}

	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTNATION;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );

		return ;
	}
	
	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	if( villageunique == targetVillage ) return;

	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if(pStrInfo == NULL)return ;
	cltStrInfo *pTargetStrInfo = GetStrInfo( targetVillage, RANKTYPE_CITYHALL );
	if(pTargetStrInfo == NULL)return ;

	if( pStrInfo == NULL || pTargetStrInfo == NULL ) return;

	if( bVillageWarSwitch ) {
		// 현재 공성중이면,

		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_WARTIME;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL,targetVillage);
	
	SI32 Homevillageunique	= pclCM->CR[id]->pclCI->clBI.siHomeVillage;
	cltStrInfo* pclstrinfo = pclClient->pclCityHallManager->GetStrInfo(Homevillageunique);
	if(pclstrinfo == NULL)return ;
	 SI32 VillageRegNum = pclstrinfo->clCityhallStrInfo.siResidentNum;// 마을 주민수

     
	if(pclrank == NULL || pclrank->clPerson.siPersonID <= 0 ) //상대편 마을에 대행수가 없다면
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTMAYOR;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 마을 에 주민이 하나도 없음 
	if (VillageRegNum == NULL || VillageRegNum <= 0 ) {
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_NOTMAYOR;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 마을 순위가 5단계 넘게 차이가 날경우 실패

	SI32 myVillageRank = pclVillageManager->GetVillageRank(villageunique);
	SI32 TargetVillageRank = pclVillageManager->GetVillageRank(targetVillage);
	
	if( TargetVillageRank - myVillageRank > 5)
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_LOWERRANK;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 마지막 패배 시간 이후 7일이 지나지 않았다면 실패
	
	NDate CurrentDate;

	CurrentDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
	
	SI32 CurrentDateVary = CurrentDate.GetDateVary();

	cltStrInfo *pclStrInfo = GetStrInfo(villageunique, RANKTYPE_CITYHALL );
	if(pclStrInfo  == NULL) return;


	NDate LastWarLoseDate;

	LastWarLoseDate.SetDate( pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateYear,
		pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateMonth,
		pclStrInfo->clCityhallStrInfo.uiLastWarLoseDateDay)	;

	SI32 LastWarLoseDateVary = LastWarLoseDate.GetDateVary();
	
	// 날짜값이 적정치 않았다면 무효화 한다.
    if( LastWarLoseDate.m_uiYear == 0 ||  LastWarLoseDate.m_uiMonth == 0 || LastWarLoseDate.m_uiDay == 0 )
	{
		LastWarLoseDateVary = 0;
	}
	

	if( (LastWarLoseDateVary > 0 ) && ( LastWarLoseDateVary + 7 >  CurrentDateVary) )
	{
		// 패배한지 7일이 지나야 공성전을 선포할수 있다.
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_AFTERLOSE;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	cltStrInfo* pclTargetStrInfo = GetStrInfo(targetVillage, RANKTYPE_CITYHALL);

	if(pclTargetStrInfo == NULL) return;

	NDate TargetLastWarLoseDate;

	TargetLastWarLoseDate.SetDate( pclTargetStrInfo->clCityhallStrInfo.uiLastWarLoseDateYear,
		pclTargetStrInfo->clCityhallStrInfo.uiLastWarLoseDateMonth,
		pclTargetStrInfo->clCityhallStrInfo.uiLastWarLoseDateDay );

	LastWarLoseDateVary =  TargetLastWarLoseDate.GetDateVary();  //((cltCityhallStrInfo*)(pclTargetStrInfo))->clLastWarLoseDate.GetDateVary();

	// 날짜값이 적정치 않았다면 무효화 한다.
	if( TargetLastWarLoseDate.m_uiYear == 0 ||  TargetLastWarLoseDate.m_uiMonth == 0 || TargetLastWarLoseDate.m_uiDay == 0 )
	{
		LastWarLoseDateVary = 0;
	}


	if( (LastWarLoseDateVary > 0 ) && ( LastWarLoseDateVary + 7 >  CurrentDateVary) )
	{		
			// 패배한지 7일이 지나야 공성전 대상으로 지목할 수 있다.
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_TARGET_AFTERLOSE;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}


	if( pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ targetVillage ] ){
		// 이미 선전 포고가 되어 있는가

		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	} 

	//대상 마을이 이미 선전포고를 했다
	if( pclClient->pclVillageManager->siDeclareWarVillage[targetVillage] > 0 )
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	
	SI32 i = 0;
	for(  i = 1; i < MAX_VILLAGE_NUMBER; ++i )
	{

		// 이미 선전 포고를 한 마을이 있다.
		if( pStrInfo->clCityhallStrInfo.bDeclareWarVillage[ i ] == 1 )
		{
			returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADYACT;
			SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
		// 대상 마을이 이미 공성전 선포를 받았다
		if( pclClient->pclVillageManager->siDeclareWarVillage[i] == targetVillage  )
		{
			returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY;
			SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
		// 이미 나를 대상으로 공성전 선포를 한 마을이 있다.
		if( pclClient->pclVillageManager->siDeclareWarVillage[i] == villageunique )
		{
			returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_ALREADY;
			SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}


	
	// 영진 [공성전] 선전포고 제한 사양 변경
	/*
	// 선전포고를 하거나 해지한지 15일이 지났는가
	{

		SI32 wardate = pStrInfo->clCityhallStrInfo.clDeclareWarDate.GetDateVary();
		SI32 currdate = pclTime->clDate.GetDateVary();

		if( currdate - wardate <= 15 ) {
			returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_DATE;
			SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}*/

/*
	// 주민숫자가 50%이상 차이가 나면
	{
		SI32 targetResidentNum = pTargetStrInfo->clCityhallStrInfo.siResidentNum;
		if( targetResidentNum < 50 ) targetResidentNum = 50;

		if( pStrInfo->clCityhallStrInfo.siResidentNum / 2 > targetResidentNum ) {
				returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_GAP;
				SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
		}
	}
*/	
//#if defined(_CITYHALL_BANKRUPTCY)
	if( pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_STABLE 
		|| pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT 
		|| pTargetStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_INITIALIZED  )
	{
		// 공성 대상 마을이 안정기입니다.. => 선전포고 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( targetVillage, pTargetStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_DECLARE_WAR );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}

	// 안정기의 마을이 다른 마을을 공성할 수 없다.
	if( pStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_STABLE )
	{
		// 공성 대상 마을이 안정기입니다.. => 선전포고 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( villageunique, pStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_DECLARE_WAR );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}
//#endif

	// 아직 주식이 안 풀린 마을에겐 공성할 수가 없다
	if ( pTargetStrInfo->clCityhallStrInfo.siTotalStockAmount <= 0 )
	{
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_NOMASTER;
		SendServerResponseMsg( 0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	NDate clRealDate;
	clRealDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
	
	sDBRequest_DeclareWar clMsg( id, villageunique, targetVillage, &clRealDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CANCELDECLAREWAR( cltMsg *pclMsg, SI32 id )
{
	SI32 returnval = 0;

	SI32 targetVillage = pclMsg->siData32[ 0 ];
	if(targetVillage < 0 || targetVillage >= MAX_VILLAGE_NUMBER)	return;

	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if( pStrInfo == NULL ) return;


	if( bVillageWarSwitch ) {
		returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_WARTIME;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	if( pclCityHallManager->GetStrInfo( villageunique )->clCityhallStrInfo.bDeclareWarVillage[ targetVillage ] != 1 ) {

		returnval = SRVAL_CITYHALL_FAIL_CANCELDECLAREWAR;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
/*
	// 선전포고를 하거나 해지한지 15일이 지났는가
	{

		SI32 wardate = pStrInfo->clCityhallStrInfo.clDeclareWarDate.GetDateVary();
		SI32 currdate = pclTime->clDate.GetDateVary();

		if( currdate - wardate <= 15 ) {
			returnval = SRVAL_CITYHALL_FAIL_DECLAREWAR_DATE;
			SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}
*/
	NDate clCurDate;
	clCurDate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
	sDBRequest_CancelDeclareWar clMsg( id, villageunique, targetVillage, &clCurDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

// 마을정보 요청( 최고 부모 마을, 부모 마을, 선전포고 마을 )
void cltServer::DoMsg_GAMEMSG_REQUEST_GETVILLAGEINFO( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgResponse_GetVillageInfo sendMsg;

	cltVillageInfo *pVillageInfo;

	for( int i = 1; i < MAX_VILLAGE_NUMBER; ++i )
	{
		pVillageInfo = pclVillageManager->pclVillageInfo[ i ];

		if( pVillageInfo == NULL ) continue;

		sendMsg.siMasterVillage[ i ] = pVillageInfo->clVillageVary.siMasterVillage;

		sendMsg.siParentVillage[ i ] = pVillageInfo->clVillageVary.siParentVillage;

		sendMsg.siVillageMark[ i ] = pVillageInfo->clVillageVary.siVillageMark;

		sendMsg.siTotalVillageScore[ i ] = pVillageInfo->clVillageVary.siTotalVillageScore;
		
	}
	
#ifdef _SAFE_MEMORY
	memcpy( sendMsg.siDeclareWarVillage, &pclVillageManager->siDeclareWarVillage[0], sizeof( sendMsg.siDeclareWarVillage ) );
	memcpy( sendMsg.clDeclareWarDate, &pclVillageManager->clDeclareWarDate[0], sizeof( sendMsg.clDeclareWarDate ) );
	memcpy( sendMsg.siNowWarVillage, &pclVillageManager->siNowWarVillage[0],sizeof(sendMsg.siNowWarVillage) );
#else
	memcpy( sendMsg.siDeclareWarVillage, pclVillageManager->siDeclareWarVillage, sizeof( sendMsg.siDeclareWarVillage ) );
	memcpy( sendMsg.clDeclareWarDate, pclVillageManager->clDeclareWarDate, sizeof( sendMsg.clDeclareWarDate ) );
	memcpy( sendMsg.siNowWarVillage, pclVillageManager->siNowWarVillage,sizeof(sendMsg.siNowWarVillage) );
#endif

	cltMsg clNormalMsg( GAMEMSG_RESPONSE_GETVILLAGEINFO, sizeof( cltGameMsgResponse_GetVillageInfo ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clNormalMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_RESIDENTSLIST( cltMsg *pclMsg, SI32 id )
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clBI.siHomeVillage < 1 || pclCM->CR[id]->pclCI->clBI.siHomeVillage >= MAX_VILLAGE_NUMBER )
	{
		return;
	}

	cltGameMsgRequest_ResidentsList *pclInfo = (cltGameMsgRequest_ResidentsList*)pclMsg->cData;
	
	if ( pclInfo->siVillageUnique < 1 || pclInfo->siVillageUnique >= MAX_VILLAGE_NUMBER )
	{
		return;
	}

	if ( pclCM->CR[id]->pclCI->clBI.siHomeVillage != pclInfo->siVillageUnique )
	{
		return;
	}

	cltGameMsgResponse_ResidentsList clResidentsList;
	clResidentsList.Set( pclInfo->siVillageUnique );
	
	SI32 CharID = 0;
	SI32 index = 0;
	while( CharID = pclCM->GetCharOrder( index ))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[CharID];
		if(IsPC(CharID))
		{
			if(pclchar->pclCI->clBI.siHomeVillage == pclInfo->siVillageUnique )
			{
				// 캐릭터 정보, 레벨, 전공
				clResidentsList.Add( &pclchar->pclCI->clBI.clPerson, pclchar->pclCI->clIP.GetLevel(), pclchar->siVillageWarKillScore, &pclchar->pclCI->clCharRank, pclchar->pclCI->clBI.szAccountID );
			}
		}
	}


	if ( clResidentsList.uiCount > 0 )
	{
		cltMsg clMsg( GAMEMSG_RESPONSE_RESIDENTSLIST, clResidentsList.GetTotalSize(), (BYTE*)&clResidentsList );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	
	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETSYMBOLSTATUS( cltMsg *pclMsg, SI32 id )
{
	SI32 returnval;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	if( bVillageWarSwitch ) {
		// 현재 공성중이면,
		returnval = SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_WARTIME;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	SI32 siGodCharUnique;
	if( pclVillageManager->IsThereVillageGod( villageunique, &siGodCharUnique ) == FALSE ) {
		returnval = SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_NOTEXIST;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	cltGameMsgRequest_SetSymbolStatus *pclInfo = (cltGameMsgRequest_SetSymbolStatus *)pclMsg->cData;

	if( pclInfo->siStatValue < 0 || pclInfo->siStatValue >= MAX_KINDINFO_NUMBER )	return;

	SI32	siMaxLife = pclVillageManager->GetMaxLifeOfGodInVillage( villageunique );
	if( siMaxLife == 0 ) return;
	
	SI32	siStatValue = 0;
	GMONEY	siCost = 0;
	
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if(pStrInfo == NULL)return ;

	// 종류 및 상태에 따른 비용과 수치 결정
	switch( pclInfo->siStatKind )
	{
	case SYMBOLSTAT_KIND:
		{
			cltKindInfo *pclKindInfo = pclKindInfoSet->pclKI[ pclInfo->siStatValue ];
			if( pclKindInfo == NULL ) return;
			
			// 현재 성황신 상태보다 낮은 단계의 성황신으로는 변경할 수 없다

			if( pclKindInfoSet->GetSortLevel( pStrInfo->clCityhallStrInfo.siSymbolKind ) >
				pclKindInfoSet->GetSortLevel( pclInfo->siStatValue ) ) {

				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_LOWERLEVEL,  0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}



			// 마을 주민 수 체크
			if( pclKindInfo->GetResidentNumForVillageGod() > pStrInfo->clCityhallStrInfo.siResidentNum ) {

				returnval = SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_RESIDENT;
				SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
				return;

			}

			// 등록할 수 있는 가 
			if( pclKindInfo->CanGodForVillage( villageunique ) == FALSE ) {

				return;
			}


			siCost = pclKindInfo->GetMoneyForVillageGod();
			siStatValue = pclInfo->siStatValue;

			if( siCost == 0 ) return;
		}
		break;

	case SYMBOLSTAT_LIFE:
		{
			SI32 value = 0 ;
			cltKindInfo * pclkind = NULL;
			if( pStrInfo->clCityhallStrInfo.siSymbolKind >= 0 && pStrInfo->clCityhallStrInfo.siSymbolKind < MAX_KINDINFO_NUMBER )
				pclkind = pclKindInfoSet->pclKI[pStrInfo->clCityhallStrInfo.siSymbolKind];

			if ( pclkind )
			{
				if ( pclkind->IsAtb(ATB_GOD) )
					value = 300 ;
				else
					value = 600 ;
			}
			else return;

			siStatValue = SI32(siMaxLife * 0.2);
			if( siStatValue + pStrInfo->clCityhallStrInfo.siSymbolLife > siMaxLife ) {
				siStatValue = siMaxLife - pStrInfo->clCityhallStrInfo.siSymbolLife;
			}

			siCost = siStatValue * value;
			siStatValue += pStrInfo->clCityhallStrInfo.siSymbolLife;
		}
		
		break;

	case SYMBOLSTAT_MANA:
		{

		}
		break;

	case SYMBOLSTAT_CAPA:
		{
			siStatValue = pStrInfo->clCityhallStrInfo.siSymbolCapa + 1;
			if ( siStatValue > 250  )
			{				
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_MAXLEVEL,  0, 0, pclCM->CR[id]->GetCharUnique() );
				return ;
			}
			siCost = 100000;

			cltKindInfo *pclKindInfo = NULL;
			if( pStrInfo->clCityhallStrInfo.siSymbolKind >= 0 && pStrInfo->clCityhallStrInfo.siSymbolKind < MAX_KINDINFO_NUMBER )
				pclKindInfo = pclKindInfoSet->pclKI[ pStrInfo->clCityhallStrInfo.siSymbolKind ];

			if( pclKindInfo != NULL ) {

				siCost = pclKindInfo->GetUpMoneyForVillageGod();
			}

			if( siCost == 0 ) return;
		}
		break;
 
	}

	// 지난번 갱신 후 1달이 지났는가
	{
		SI32 lastdate = pStrInfo->clCityhallStrInfo.clLastSymbolUpdate.GetDateVary();
		SI32 currdate = pclTime->clDate.GetDateVary();

		if( currdate - lastdate < 30 ) {
			returnval = SRVAL_CITYHALL_FAIL_SYMBOLSTATUS_DATE;
			SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}

	sDBRequest_SetSymbolStatus clMsg( id, villageunique, pclInfo->siStatKind, siStatValue, siCost, &pclTime->clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETCASTLESTATUS( cltMsg *pclMsg, SI32 id )
{

	SI32 returnval;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	if( bVillageWarSwitch ) {
		// 현재 공성중이면,
		returnval = SRVAL_CITYHALL_FAIL_CASTLESTATUS_WARTIME;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	SI32 siCastleCharUnique;
	if( pclVillageManager->IsThereCastle( villageunique, &siCastleCharUnique ) == FALSE ) {
		returnval = SRVAL_CITYHALL_FAIL_CASTLESTATUS_NOTEXIST;
		SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	

	cltGameMsgRequest_SetCastleStatus *pclInfo = (cltGameMsgRequest_SetCastleStatus *)pclMsg->cData;

	SI32	siMaxLife = pclVillageManager->GetMaxLifeOfCastleInVillage( villageunique );
	if( siMaxLife == 0 ) return;
	
	SI32	siStatValue = 0;
	GMONEY	siCost = 0;
	
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if(pStrInfo == NULL)return;

	// 종류 및 상태에 따른 비용과 수치 결정
	switch( pclInfo->siStatKind )
	{
	case SYMBOLSTAT_KIND:
		{
		}
		break;

	case SYMBOLSTAT_LIFE:
		{
			siStatValue = SI32(siMaxLife * 0.2);
			if( siStatValue + pStrInfo->clCityhallStrInfo.siCastleLife > siMaxLife ) {
				siStatValue = siMaxLife - pStrInfo->clCityhallStrInfo.siCastleLife;
			}

			siCost = siStatValue * 200;
			siStatValue += pStrInfo->clCityhallStrInfo.siCastleLife;
		}
		
		break;

	case SYMBOLSTAT_MANA:
		{
		}
		break;

	case SYMBOLSTAT_CAPA:
		{
			siStatValue = pStrInfo->clCityhallStrInfo.siCastleCapa + 1;
			if ( siStatValue > 250 )
			{
				SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_CASTLESTATUS_MAXLEVEL,  0, 0, pclCM->CR[id]->GetCharUnique() );
				return ;
			}

			siCost = 100000;
/*
			cltKindInfo *pclKindInfo = pclKindInfoSet->pclKI[ pStrInfo->clCityhallStrInfo.siSymbolKind ];

			if( pclKindInfo != NULL ) {

				siCost = pclKindInfo->GetUpMoneyForVillageGod();
			}
*/
		}
		break;

	}


	// 지난번 갱신 후 1달이 지났는가
	{
		SI32 lastdate = pStrInfo->clCityhallStrInfo.clLastCastleUpdate.GetDateVary();
		SI32 currdate = pclTime->clDate.GetDateVary();

		if( currdate - lastdate < 30 ) {

			returnval = SRVAL_CITYHALL_FAIL_CASTLESTATUS_DATE;
			SendServerResponseMsg( RANKTYPE_CITYHALL, returnval,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}

	sDBRequest_SetCastleStatus clMsg( id, villageunique, pclInfo->siStatKind, siStatValue, siCost, &pclTime->clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_RESIDENTSHARE( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_ResidentShare *pclInfo = (cltGameMsgRequest_ResidentShare *)pclMsg->cData;
	// 마을 이익금 차감 루틴 -1
	if( pclInfo->clTotalShareMoney.itMoney < 1000000 ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	pclCM->FindResidentListInVillage( villageunique );
	
	// 마을에 있는 주민 수가 30명 이하이면 배당 할 수 없다 
	if( siParaCharListIndex < 30 ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGCOUNT,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	// 배당 가능한 시간인가( 18:00 ~ 23:00 )

	SYSTEMTIME systime;
	GetLocalTime( &systime );
	if( systime.wHour < 18 || systime.wHour > 23 ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_RESIDENTSHARE_WRONGTIME,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 건물을 해체한지 한달이 지났는가 
	cltStrInfo *pStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if( pStrInfo->clCityhallStrInfo.siLastDestroyStrDate + 30 > pclTime->clDate.GetDateVary() ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_RESIDENTSHARE_DATE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	if( pStrInfo->clCityhallStrInfo.clProfitMoney.itMoney < pclInfo->clTotalShareMoney.itMoney ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_RESIDENTSHARE_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	sDBRequest_ResidentShare sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_REQUEST_RESIDENTSHARE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siCount = siParaCharListIndex;

	if( sendMsg.siCount > 100 ) sendMsg.siCount = 100;

	GMONEY sharemoney = (GMONEY)((REAL64)pclInfo->clTotalShareMoney.itMoney / (REAL64)sendMsg.siCount );

	sendMsg.siVillageUnique = villageunique;
	sendMsg.clTotalShareMoney.Set( sharemoney * sendMsg.siCount );

	for( int i = 0; i < sendMsg.siCount; ++i ) {

		sendMsg.siCharIndex[ i ] = siParaCharList[ i ];
		sendMsg.siPersonID[ i ] = pclCM->CR[ siParaCharList[ i ] ]->pclCI->GetPersonID();
		sendMsg.siShareMoney[ i ] = sharemoney;
	}
	
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_GETSTRUCTURELIST(cltMsg* pclMsg, SI32 id)
{

}


void cltServer::DoMsg_GAMEMSG_REQUEST_BUILDSTRUCTURE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_BuildStructure *pclInfo = (cltGameMsgRequest_BuildStructure *)pclMsg->cData;	

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )		return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}


	// slot이 비어있는지 점검 
	if( pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.clStructureInfo.IsExist( pclInfo->siSlot ) ) {
		// slot이 비어있지 않다면,
		return;
	}

	// 해당 슬롯이 NPC가 위치할 수 있는 슬롯일때 NPC가 있는지 점검
	// NPC가 있다면 건물을 지을 수 없다
	if( pclInfo->siSlot == 6 ) {

		cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );

		for( int i = 0; i < MAX_HIRED_NPC_NUM; ++i )
		{
			if( pclStrInfo->clCityhallStrInfo.clHiredNPC[ i ].siNPCKind != 0 ) break;
		}

		if( i < MAX_HIRED_NPC_NUM ) {
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_CONSTRUCT_NPCZONE,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}

	}

	// 농장일 경우 
	if( pclInfo->siStructureUnique == LAND_UNIQUE ) {
		if( pclVillageManager->pclVillageInfo[ villageunique ]->bFarmSwitch == FALSE ) {
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTFARM,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}

	// 광산일 경우
	if( pclInfo->siStructureUnique == MINE_UNIQUE ) {
		if( pclVillageManager->pclVillageInfo[ villageunique ]->bIronSwitch == FALSE ) {
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_CONSTRUCT_NOTMINE,  0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}

	//[진성] 통합마켓일 경우 건설 조건은 마을 순위가 20위 이상.
	if( NEWMARKET_UNIQUE == pclInfo->siStructureUnique ) 
	{
		if( 20 < pclVillageManager->GetVillageRank(villageunique) ) return;
	}

	// 건설 가능한 건물인지 점검
	SI32 ref = pclVillageStructureSet->FindRefFromUnique( pclInfo->siStructureUnique );
	if( ref == -1 ) return;
	
	if( pclVillageStructureSet->pclVillageStructureInfo[ ref ]->bDestroyable == FALSE ) {
		// 건설 가능한 건물이 아니라면,
		return;
	}

	// 이미 건설 되어 있는 건물인지 점검 
	if( true == pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.clStructureInfo.IsDuplicate( pclInfo->siStructureUnique ) ) 
	{
		// 이미 건설 되어 있는 건물이라면,
		return;
	}

	// 마을 건물 가격 조정
	SI32 npcRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
	SI32 siPenalty = 0;
		 if ( npcRate >= 500 )  siPenalty = 100;
	else if ( npcRate >= 400 )  siPenalty =  90;
	else if ( npcRate >= 300 )  siPenalty =  80;
	else if ( npcRate >= 200 )  siPenalty =  70;
	else						siPenalty =  60;

	GMONEY cost = pclVillageStructureSet->pclVillageStructureInfo[ ref ]->siNeedMoney * siPenalty / 100;

	// 건물을 건설한다
	sDBRequest_BuildStructure sendMsg( id, villageunique, pclInfo->siSlot, pclInfo->siStructureUnique, cost, BUILD_BY_PERSON, pclVillageStructureSet->pclVillageStructureInfo[ ref ]->siNeedVillageScore );

	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

// 건물을 해체 한다. 
void cltServer::DoMsg_GAMEMSG_REQUEST_DESTROYSTRUCTURE(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_DestroyStructure *pclInfo = (cltGameMsgRequest_DestroyStructure *)pclMsg->cData;	

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )		return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	//이곳에서 날려 먹는데!!!
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	// 마을의 villageunique 근거로 건물의 정보를 구한다. 
	cltStrInfo* pclinfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
	if(pclinfo == NULL)return ;

	// 오늘 날짜의 DateVary를 구한다. 
	SI32 datevary = pclTime->clDate.GetDateVary();
	if(datevary < pclinfo->clCityhallStrInfo.siLastDestroyStrDate + 30)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAILDESTROYSTR_NOLAST,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;	// 한달 이내면 실패. 
	}


	// 주주총회 중에는 건물을 해체할 수 없다 
	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];
	if( pclCityHall->clGeneralMeetingInfo.siMatterType != 0 ) {
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_DESTROYSTR_MEETING,  0, 0, pclCM->CR[id]->GetCharUnique());
		return;
	}	


	// 해체 가능한 건물인지 점검
	SI32 siStructureUnique = pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.clStructureInfo.Get( pclInfo->siSlot );
	if( siStructureUnique == 0 ) {
		// 슬롯에 건물이 없다.
		return;
	}

	SI32 ref = pclVillageStructureSet->FindRefFromUnique( siStructureUnique );
	if( ref == -1 ) return;
    
	SI32 ranktype = 0;
	if ( ref >= 0 && ref < MAX_VILLAGE_STRUCTURE_NUMBER)
		ranktype = pclVillageStructureSet->GetRankTypeFromStructureName( pclVillageStructureSet->pclVillageStructureInfo[ ref ]->szName );
	if( ranktype == 0 ) return;

	sDBRequest_DestroyStructure sendMsg( id, villageunique, pclInfo->siSlot, siStructureUnique, ranktype, datevary);

	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_VILLAGENOTICE( cltMsg* pclMsg, SI32 id )
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltGameMsgRequest_VillageNotice *pclInfo = (cltGameMsgRequest_VillageNotice*)pclMsg->cData;	

	// 마을의 villageunique 근거로 건물의 정보를 구한다. 
	cltStrInfo* pclinfo = GetStrInfo( pclInfo->siVillageUnique, RANKTYPE_CITYHALL );
	
	if( pclinfo )
	{
		cltGameMsgResponse_VillageNotice ResPonse_VillageNotice( pclInfo->siVillageUnique, pclinfo->clCommonInfo.strNotice );
		cltMsg clMsg( GAMEMSG_RESPONSE_VILLAGENOTICE, sizeof(ResPonse_VillageNotice), (BYTE*)&ResPonse_VillageNotice );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_ALLSTRCHIEFINFO( cltMsg* pclMsg, SI32 id )
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 siVillageUnique = pclMsg->siData32[ 0 ];
	if( siVillageUnique < 0 || siVillageUnique >= MAX_VILLAGE_NUMBER)		return;

	cltGameMsgResponse_AllStrChiefInfo sendMsg;

	ZeroMemory( &sendMsg, sizeof( sendMsg ));

	cltRank* pclRank;

	for( int i = 1; i < MAX_RANKTYPE_NUMBER; ++i ) {

		cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ siVillageUnique ];
		if( pVillageInfo == NULL ) continue;

		if( pVillageInfo->clVillageVary.clStructureInfo.IsThereStructure( i ) == false ) continue;

		pclRank = pclRankManager->GetRank( i, siVillageUnique );

		sendMsg.siRankType[ i ] = i;

		if( pclRank ) {
			
			if( pclRank->GetPersonID() ) {
				memcpy( sendMsg.szChiefPersonName[ i ], pclRank->GetName(), MAX_PLAYER_NAME );
				sendMsg.szChiefPersonName[ i ][ MAX_PLAYER_NAME - 1 ] = NULL;				
			} else {
				TCHAR* pText = GetTxtFromMgr(1561);
				StringCchCopy( sendMsg.szChiefPersonName[ i ], MAX_PLAYER_NAME, pText );
			}
		} else {
			TCHAR* pText = GetTxtFromMgr(1561);
			StringCchCopy( sendMsg.szChiefPersonName[ i ], MAX_PLAYER_NAME, pText );
		}
	}

	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg(GAMEMSG_RESPONSE_ALLSTRCHIEFINFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_PROFITTOCAPITALFUND( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgRequest_ProfitToCapitalFund *pclInfo = (cltGameMsgRequest_ProfitToCapitalFund *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	if( pclInfo->siMoney < 0 ) return;

	sDBRequest_ProfitToCapitalFund sendMsg( id, villageunique, pclInfo->siMoney );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_STARTEVENT( cltMsg* pclMsg, SI32 id )
{

	cltGameMsgRequest_StartEvent *pclInfo = (cltGameMsgRequest_StartEvent *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )	return ;

	// [영훈] 초보자 마을 : 초보자 마을은 마을 이벤트 시작 안되게
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == villageunique )
		{
			return;
		}
	}

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	
	cltCityhallEventInfo *pclEvent = pclCityHallManager->clEventManager.GetEventInfoFromEventID( pclInfo->siEventID );

	if( pclEvent == NULL ) return;

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );

	if( pclStrInfo == NULL ) return;
	
	// 보유자금(자본금)이 충분한가?
	SI32 cost = (SI32)(pclEvent->siEventMoney * (pclItemManager->clItemStatistic.siItemPriceIndex / 10000000.));
	cost = max(cost, 2000000);

	if( pclStrInfo->clCommonInfo.clMoney.itMoney < cost ) {
		// 보유자금 부족
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_EVENT_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 이벤트 시작
	BOOL bStarted = FALSE;
	switch( pclEvent->eventid ) 
	{
		case 1:
			// 불가사리 사냥 이벤트 
			{
				if( pclEventHunting->Start( villageunique, cost ) == true ) {
					// 성공 
					bStarted = TRUE;

				}
				else 
				{
					// 실패 : 이미 진행중..
					SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_EVENT_ALREADYSTARTED,  0, 0, pclCM->CR[id]->GetCharUnique() );
				}
			} 
			break;
		//[진성] 마을 이벤트 - 몬스터 습격. 2008-3-26
		case 3:
			{
				// 이벤트 중이면 안되.
				if(pclMonsterAttackEvent->IsEvent())
				{
					SendServerResponseMsg( 0, SRVAL_MONSTER_ATTACK_EVENT,  MONSTER_ATTACK_EVENT_ALREADY_RUN, 0, pclCM->CR[id]->GetCharUnique() );
					return;
				}

				pclMonsterAttackEvent->StartEvent(villageunique);
				bStarted = TRUE;
			}
			break;
	}

	if( bStarted ) {

		sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_EVENTPAY, pclInfo->siEventID, villageunique, RANKTYPE_CITYHALL, -cost );

		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	}


}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETVILLAGEMARK(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SetVillageMark *pclInfo = (cltGameMsgRequest_SetVillageMark *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	cltStrInfo *pclStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );

	if( pclStrInfo == NULL ) return;
	
	// 보유자금(자본금)이 충분한가?
	if( pclStrInfo->clCommonInfo.clMoney.itMoney < 1000000 ) {
		// 보유자금 부족
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_VILLAGEMARK_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	sDBRequest_SetVillageMark sendMsg( id, villageunique, pclInfo->siVillageMark, &pclTime->clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}


void cltServer::DoMsg_GAMEMSG_REQUEST_SETVILLAGEMAP(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SetVillageMap *pclInfo = (cltGameMsgRequest_SetVillageMap *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank;
	clrank.Init();

	cltStrInfo *pclStrInfo = NULL;
	pclStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );	// 마을지도나 수렵장 지도 모두 관청 자금에서 출금됨

	//_LEON_HUNTMAP
	switch(pclInfo->siMapType)
	{
	case cltGameMsgRequest_SetVillageMap::MAPTYPE_CITYHALL:
		clrank.Set( RANKTYPE_CITYHALL, villageunique );
		//pclStrInfo = GetStrInfo( villageunique, RANKTYPE_CITYHALL );
		break;
	case cltGameMsgRequest_SetVillageMap::MAPTYPE_HUNT:
		{
			SI32 i = 0;
			clrank.Set( RANKTYPE_HUNT, villageunique );
			//pclStrInfo = GetStrInfo( villageunique, RANKTYPE_HUNT );
			for( i = 0; i<MAX_HUNT_MAP_NUMBER-1; i++)
			{
				if(villageunique == pclMapManager->GetParentMap(HUNT_MAP_START + i ) )
				{
					villageunique = HUNT_MAP_START + i;
					break;
				}
			}
			if( villageunique == HUNT_MAP_START + MAX_HUNT_MAP_NUMBER-1 )
				return;

			cltMapTypeInfo *pMapTypeInfo = NULL;

			for( i=0; i<MAX_HUNT_MAP_NUMBER-1; i++)
			{
				pMapTypeInfo = NULL;
				pMapTypeInfo = pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

				if( pMapTypeInfo == NULL ) continue;
				if( pMapTypeInfo->siAdaptHuntType != HUNTTYPE_ALL ) continue;
				SI32 mapunique = pMapTypeInfo->siMapUnique;
				if( pclInfo->siVillageMapUnique == mapunique ) break;
			}

			if( i >= MAX_HUNT_MAP_NUMBER-1 ) return;		// 잘못된 mapunique...
		}
		break;
	default:
		return;
	}

	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
		return;

	if( pclStrInfo == NULL ) return;
	
	cltMapTypeInfo *pMapTypeInfo = NULL;

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) {

		pMapTypeInfo = pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

		if( pMapTypeInfo == NULL ) continue;

		if( pMapTypeInfo->siMapUnique == pclInfo->siVillageMapUnique ) break;
	}

	if( i == MAX_MAPTYPE_NUMBER ) return;

	if( pMapTypeInfo == NULL ) return;

	// 보유자금(자본금)이 충분한가? 마을지도나 수렵장 지도 모두 관청 자금에서 나간다.
	if( pclStrInfo->clCommonInfo.clMoney.itMoney < pMapTypeInfo->siAdaptNeedVillageMoney ) {
		// 보유자금 부족

		switch(pclInfo->siMapType)
		{
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_CITYHALL:
			SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_CITYHALL_FAIL_VILLAGEMAP_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
			break;
		case cltGameMsgRequest_SetVillageMap::MAPTYPE_HUNT:
			SendServerResponseMsg( RANKTYPE_HUNT, SRVAL_HUNT_FAIL_HUNTMAP_NOTENOUGHMONEY,  0, 0, pclCM->CR[id]->GetCharUnique() );
			break;
		default:
			return;
		}

		return;
	}

	sDBRequest_SetVillageMap sendMsg( id, villageunique, pclInfo->siVillageMapUnique, pMapTypeInfo->siAdaptNeedVillageMoney, pclInfo->siMapType );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_VILLAGELEVELDETAILINFO( cltMsg *pclMsg, SI32 id )
{
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER )		return ;


	cltGameMsgResponse_VillageLevelDetailInfo sendMsg;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	pclCityHall->clLevelInfo.siVillageTotalScore = pclVillageManager->pclVillageInfo[ villageunique ]->clVillageVary.siTotalVillageScore;
	sendMsg.clLevelInfo.Set( &pclCityHall->clLevelInfo );
	
	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGELEVELDETAILINFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_INFO(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;



	cltGameMsgResponse_GeneralMeetingInfo sendMsg;

	sendMsg.clGeneralMeetingInfo = pclCityHall->clGeneralMeetingInfo;

	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_INFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_SUGGEST(cltMsg* pclMsg, SI32 id)//주주총회발의하기.
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_GeneralMeetingSuggest *pclInfo = (cltGameMsgRequest_GeneralMeetingSuggest *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	// [추가 : 황진성 2008. 2. 20 => 주주총회를 열기 위해 운영자가 마을에 주주총회가 발동가능하게 해야 한다. ]
	if( true == pclClient->IsCountrySwitch( Switch_GeneralMeetingSuggest ) )
	{
		if( false == pclClient->pclVillageManager->pclVillageInfo[villageunique]->clVillageVary.m_bGeneralMeetingSuggest )
		{
			SendServerResponseMsg(0, SRVAL_GENERAL_MEETING_SUGGEST_OFF_FAIL, 0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	}

	cltStrInfo* pclStrInfo = pclCityHallManager->GetStrInfo(villageunique);
	
	if(	   pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT 
		|| pclStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_INITIALIZED  )
	{
		// 마을이 파산 상태입니다.. => 주주총회 발의 실패 => 알림
		cltGameMsgResponse_NotifyClientAboutCityHallState clinfo( villageunique,  pclStrInfo->clCityhallStrInfo.usCurStatus, cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_GENERALMEETING_SUGGEST );
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 요건 주총 시작일
	cltDate clStartDate;
	clStartDate.Set( &pclTime->clDate );
	SI32 StartDate = clStartDate.GetDateVary();

	cltDate clDate;
	clDate.Set( &pclTime->clDate );
	//KHY - 1115 - 주주총회 발동 시점 변경. - 후보자 동의 까지 1달만 기다린다.
	//clDate.AddMonth( 1 );  
	//[진성] 주주총회 종료 시간을 게임시간 3달로 변경. => 2008-7-21
	clDate.AddMonth( 3 );

	sDBRequest_GeneralMeeting_SuggestChief sendMsg( id, villageunique, personid, pclInfo->szCandidateName, StartDate, &clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_VOTE(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_GeneralMeetingVote *pclInfo = (cltGameMsgRequest_GeneralMeetingVote *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 투표권자인가
	if( pclCityHall->clGeneralMeetingInfo.IsVoter( personid ) == FALSE ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_GENERALMEETING_FAIL_VOTE_NOTVOTER,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	// 이미 투표 하였나
	if( pclCityHall->clGeneralMeetingInfo.IsAleadyVote( personid ) ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_GENERALMEETING_FAIL_VOTE_ALREADY,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	sDBRequest_GeneralMeeting_Vote sendMsg( id, villageunique, personid, pclInfo->siVoteType );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_VOTE_REFRESH(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_GeneralMeetingVoteRefresh *pclInfo = (cltGameMsgRequest_GeneralMeetingVoteRefresh*)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;

	cltGameMsgResponse_GeneralMeetingInfo sendMsg;

	sendMsg.clGeneralMeetingInfo = pclCityHall->clGeneralMeetingInfo;

	// 클라이언트로 정보를 보낸다. 
	if( pclCM->IsValidID(id) ) {	
		cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_INFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}


}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;
	
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	if( pclCityHall->clGeneralMeetingInfo.siMatterType == 0 ||
		pclCityHall->clGeneralMeetingInfo.siAcceptCandidate == 1 ) {
		return;
	}

	if( personid != pclCityHall->clGeneralMeetingInfo.siCandidatePersonID ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_GENERALMEETING_FAIL_ACCEPTCANDIDATE,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	//KHY - 1115 - 주주총회 발동 시점 변경. -대행수 후보자가 본인 동의를 하는 순간 2달을 투표기간으로 설정한다.
	/*cltDate clStartDate;
	clStartDate.Set( &pclTime->clDate );
	SI32 StartDate = clStartDate.GetDateVary();
	
	cltDate clDate;
	clDate.Set( &pclTime->clDate );
	clDate.AddMonth( 2 );*/

	//[진성] 주주총회 참가자 동의, 투표 시간은 이미 있으므로 그대로 적용.( 발동후 부터 발동후 3달간 ) => 2008-7-21
	SI32	StartDate = pclCityHall->clGeneralMeetingInfo.clStartDate.GetDateVary();
	cltDate clDate	  = pclCityHall->clGeneralMeetingInfo.clCloseDate;

	sDBRequest_GeneralMeeting_AcceptCandidate sendMsg(	id, 
														villageunique, 
														personid, 
														StartDate, 
														&clDate );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_PARTICIPATE(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_GeneralMeetingParticipate *pclInfo = (cltGameMsgRequest_GeneralMeetingParticipate *)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	
	sDBRequest_GeneralMeeting_SetVoterState sendMsg( pclInfo->siVillageUnique, personid, pclInfo->siParticipate );
	sendMsg.usCharID = id;
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHAT(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id))
		return;

	cltGameMsgRequest_GeneralMeetingChat *pclInfo = (cltGameMsgRequest_GeneralMeetingChat*)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();


	// 클라이언트로 정보를 보낸다. 

	cltGameMsgResponse_GeneralMeetingChat sendMsg;

	memcpy( sendMsg.szName, pclCM->CR[id]->GetName(), MAX_PLAYER_NAME );
	sendMsg.szName[ MAX_PLAYER_NAME - 1 ] = NULL;

	memcpy( sendMsg.szText, pclInfo->szText, 128 );
	sendMsg.szText[ 127 ] = NULL;

	sendMsg.textlen = _tcslen( sendMsg.szText );


	cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_CHAT, MAX_PLAYER_NAME + 16 + sendMsg.textlen, (BYTE*)&sendMsg );
	pclCityHall->clChatMemberManager.SendMsg( (BYTE*)&clMsg );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHATJOIN(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	// 투표권자인가
	if( pclCityHall->clGeneralMeetingInfo.IsVoter( personid ) == FALSE &&
		pclCityHall->clGeneralMeetingInfo.siCandidatePersonID != personid &&
		pclCM->CR[id]->GetGameMasterLevel() < 100 ) {
		SendServerResponseMsg( RANKTYPE_CITYHALL, SRVAL_GENERALMEETING_FAIL_VOTE_NOTVOTER,  0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	pclCityHall->clChatMemberManager.AddChatMember( id, pclCM->CR[id]->pclCI->GetPersonID() );

	// 클라이언트로 정보를 보낸다. 
	cltMsg clMsg(GAMEMSG_RESPONSE_GENERALMEETING_CHATJOIN, 0 );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_GENERALMEETING_CHATOUT(cltMsg* pclMsg, SI32 id)
{
	if( false == pclCM->IsValidID(id) )
		return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER ) return ;

	cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[ villageunique ];

	if( pclCityHall == NULL ) return;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	pclCityHall->clChatMemberManager.DelChatMember( pclCM->CR[id]->pclCI->GetPersonID() );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHANGESTRBASEMONEY( cltMsg* pclMsg, SI32 id )
{
	if( pclCM->IsValidID( id ) == false )
	{
		return;
	}

	cltGameMsgRequest_ChangeStrBaseMoney *pclInfo = (cltGameMsgRequest_ChangeStrBaseMoney *)pclMsg->cData;

	if( pclInfo == NULL )
		return;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageunique <= 0 ) return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}


	cltStrInfo* pStrInfo = pclCityHallManager->GetStrInfo( villageunique );

	if( pclInfo->siMoney > pStrInfo->clCommonInfo.clMoney.itMoney )
		return;

	sDBRequest_ChangeStrBaseMoney sendMsg( id, villageunique, pclInfo->siType, pclInfo->siMoney );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
}


// 이벤트 NPC 고용 요청
void cltServer::DoMsg_GAMEMSG_REQUEST_HIREEVENTNPC( cltMsg *pclMsg, SI32 id )
{

	cltGameMsgRequest_HireEventNPC *pclInfo = (cltGameMsgRequest_HireEventNPC *)pclMsg->cData;

	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclInfo->siVillageUnique ;
	if(villageunique <= 0)return ;

	// 권한이 있는가 
	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	cltSimpleRank clrank( RANKTYPE_CITYHALL, villageunique );
	if( pclRankManager->IsRank(personid, &clrank) == FALSE)
	{
		return;
	}

	SI32 eventNpcKind = pclInfo->siEventNpcKind;

	cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo(villageunique);
	if( pclinfo == NULL ) return;

	// 똑같은 NPC는 다시 고용할 수 없다.
	SI32 siNPCKind = 0;
	for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++)
	{
		siNPCKind = pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind;		
		if(siNPCKind == eventNpcKind) 
		{
			return;
		}

		// 크리스마스 트리 2단계가있다면.
		if(siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1")))
		{
			// 설치하려는 구조물이 영웅의 동상 1단계 이면 설치 생략.
			if(eventNpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREEBASE")))
			{
				return;
			}
		}

		// 영웅의 동상 2단계가 있으면.
		if(siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE")))
		{
			// 설치하려는 구조물이 영웅의 동상 1단계 이면 설치 생략.
			if(eventNpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUEBASE")))
			{
				return;
			}
		}
		// 할로윈 촛대 2단계가 있으면 //Switch_HalloweenEvent
		if(siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEEN")))
		{
			// 설치 하려는 구조물이 할로윈 촛대 베이스면 생략
			if(eventNpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENBASE")))
			{
				return;
			}
		}
		// 신년이벤트 건물 2단계가 있으면
		if(siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE")))
		{
			// 설치 하려는 구조물이 할로윈 촛대 베이스면 생략
			if(eventNpcKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREEBASE")))
			{
				return;
			}
		}
	}

	SI64 siPayment = 0;
	// 신년 이벤트 기간에만 나무를 세울수 있도록.	

	//---------------------------------------------------------------------------------------------
	// 이벤트 NPC 가격 결정
	if( eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_KADOMACH")) ||
		eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_NEWYEARTREE"))	)
	{
		if(true == NewYearEventDay())
		{			
			siPayment = pclClient->GetGlobalValue(TEXT("GV_NewYearValue"));	//신년 트리 세우는 돈을 읽어 온다	
		}
	}
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_RETURNTOWER"))) 
	{
		SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		if (npcrate < 500) siPayment = 5000000;
		else if (npcrate < 1000) siPayment = 10000000;
		else if (npcrate < 1500) siPayment = 15000000;
		else if (npcrate < 2000) siPayment = 20000000;
		else if (npcrate < 2500) siPayment = 25000000;
		else if (npcrate < 3000) siPayment = 30000000;
		else{
			siPayment = 35000000;
		}
	}
	//[진성] 영웅의 동상 가격 => 2008-6-12
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_ROULETTESTATUEBASE"))) 
	{
		SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		if (npcrate < 100)		 siPayment =   5000000;
		else if (npcrate < 300)  siPayment =  10000000;
		else if (npcrate < 700)  siPayment =  20000000;
		else if (npcrate < 1000) siPayment =  40000000;
		else if (npcrate < 2000) siPayment =  60000000;
		else if (npcrate < 3000) siPayment =  80000000;
		else					 siPayment = 100000000;
	}
	//[성웅] 할로윈 촛대 //Switch_HalloweenEvent
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_HALLOWEENBASE"))) 
	{
		SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		if (npcrate < 100)		 siPayment =   1000000;
		else if (npcrate < 300)  siPayment =  2000000;
		else if (npcrate < 700)  siPayment =  8000000;
		else if (npcrate < 1000) siPayment =  10000000;
		else if (npcrate < 2000) siPayment =  15000000;
		else if (npcrate < 3000) siPayment =  20000000;
		else					 siPayment = 50000000;
	}
	//KHY - 1113 - 크리스마스 이벤트 - 같은 설정값을 읽어온다.
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_PINE_TREE")) ||
		eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_CHRISTMASTREEBASE")) ) 
	{
		//siPayment = pclClient->GetGlobalValue(TEXT("GV_Christmasvalue"));	//트리 세우는 돈을 읽어 온다
		SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		if (npcrate < 100)		  siPayment =  5000000;
		else if (npcrate < 300)   siPayment =  10000000;
		else if (npcrate < 700)   siPayment =  20000000;
		else if (npcrate < 1000)  siPayment =  40000000;
		else if (npcrate < 2000)  siPayment =  60000000;
		else if (npcrate < 3000)  siPayment =  80000000;
		else if (npcrate < 4000)  siPayment =  100000000;
		else if (npcrate < 5500)  siPayment =  120000000;
		else if (npcrate < 8500)  siPayment =  160000000;
		else if (npcrate < 14500) siPayment =  280000000;
		else if (npcrate < 25500) siPayment =  500000000;
		else if (npcrate < 33500) siPayment =  650000000;
		else if (npcrate < 35500) siPayment =  800000000;
		else					  siPayment =  800000000;
	}
	// 수정 : PCK - 히나마츠리 이벤트 (08.12.16)
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_HINADOLL"))) 
	{
		SI64 npcRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;//NPCRate를 읽어 온다.

		siPayment = npcRate * 300000;
	}
	// [춘기] 신년이벤트 구조물 (08.11.24)
	else if(eventNpcKind == pclClient->GetUniqueFromHash( TEXT("KIND_NEWYEARTREEBASE"))) 
	{
		SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		if (npcrate < 100)		 siPayment =   5000000;
		else if (npcrate < 300)  siPayment =  10000000;
		else if (npcrate < 700)  siPayment =  20000000;
		else if (npcrate < 1000) siPayment =  40000000;
		else if (npcrate < 2000) siPayment =  60000000;
		else if (npcrate < 3000) siPayment =  80000000;
		// KHY - 1021 - NPCRate 값 확대.
		else if (npcrate < 4000) siPayment =  100000000;
		else if (npcrate < 5500) siPayment =  120000000;
		else if (npcrate < 8500) siPayment =  160000000;
		else if (npcrate < 14500) siPayment = 280000000;
		else if (npcrate < 25500) siPayment = 500000000;
		else if (npcrate < 33500) siPayment = 650000000;
		else					  siPayment = 800000000;
	}

	//---------------------------------------------------------------------------------------------
	if( siPayment <= 0 )				return;

	for (SI32 i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {

		if( pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind == 0 )
		{
			if(pclKindInfoSet->IsValidKind(eventNpcKind) == false) 
				return;

			if(eventNpcKind > 0)
			{
				sDBRequest_HireNPC clMsg( id, villageunique, i , eventNpcKind, siPayment, 0, &pclTime->clDate, HIRENPC_HIREEVENTNPC );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
			}

			return;
		}
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_EVENT_INSERTMATERIAL( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_Event_InsertMaterial *pclInfo = (cltGameMsgRequest_Event_InsertMaterial *)pclMsg->cData;

	if( pclCM->IsValidID(id) == false)				return;
	if( pclInfo->m_siVillageUnique < 0 )			return;
	if( pclInfo->m_siConstructType <= 0 )			return;

	cltEventStructureUnit* pEventStruct = pclEventStructure->GetEventStructure(pclInfo->m_siConstructType);
	if( pEventStruct == NULL )						return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if( pclchar == NULL)							return;

	//---------------------------------------------
	// 제작자에게 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();


	//---------------------------------------------
	// 제작자에게 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	DBStructMaterialInfo clinfo[MAX_MATERIAL_NUMBER];

	int		i, j;
	bool	bFind = false;

	for(i=0; i<MAX_MATERIAL_NUMBER; i++)
	{
		clinfo[i].siItemUnique	= pEventStruct->clMaterial[i].siUnique;
		clinfo[i].siNeedNum		= pEventStruct->clMaterial[i].siNeedNum;
		clinfo[i].siUseItemNum	= 0;
		clinfo[i].siStgPos		= -1;

		bFind = false;
		for(j=0; j<MAX_MATERIAL_NUMBER; j++)
		{
			if( pclInfo->m_siItemUnique[j] == clinfo[i].siItemUnique )
			{
				clinfo[i].siUseItemNum = pclInfo->m_siItemNum[j];
				bFind = true;
			}
		}

		// 없는 재료는 Unique 를 0으로 설정한다.
		if(bFind == false)
		{
			clinfo[i].siItemUnique	= 0;
			clinfo[i].siNeedNum		= 0;
		}

		if(clinfo[i].siUseItemNum <= 0)
		{
			clinfo[i].siItemUnique	= 0;
			clinfo[i].siNeedNum		= 0;
		}
	}

	// 전달된 사용 재료를 인벤에서 찾는다.
	for( i = 0;i < MAX_MATERIAL_NUMBER; i++)
	{
		SI32 siMaterialUnique	= clinfo[i].siItemUnique;
		SI32 siMaterialNum		= clinfo[i].siUseItemNum;

		if( siMaterialUnique <= 0 )					continue;
		if( siMaterialNum < 0 )						continue;

		// 인벤에서 해당 재료를 찾는다. 
		for( j = PERSONITEM_INV0; j < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; j++ )
		{
			cltItem* pclItem = &pclchar->pclCI->clCharItem.clItem[j];
			if(pclItem == NULL)					continue;

			if( pclItem->siUnique == siMaterialUnique && pclItem->siItemNum >= siMaterialNum )
			{
				clinfo[i].siItemUnique	= siMaterialUnique;
				clinfo[i].siStgPos		= j;
				clinfo[i].siUseItemNum	= siMaterialNum;
			
				break;
			}
		}
	}

	sDBRequest_Event_InsertMaterial clMsg( id, pclchar->pclCI->GetPersonID(), pclInfo->m_siVillageUnique , pclInfo->m_siConstructType, clinfo );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALLIST( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_Event_MaterialList *pclInfo = (cltGameMsgRequest_Event_MaterialList *)pclMsg->cData;

	if( pclCM->IsValidID(id) == false)			return;

	if( pclInfo->m_siConstructType <= 0)		return;
	if( pclInfo->m_siVillageUnique < 0)			return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar == NULL)							return;

	sDBRequest_Event_MaterialList clMsg( id, pclchar->pclCI->GetPersonID(), pclInfo->m_siVillageUnique , pclInfo->m_siConstructType );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE( cltMsg *pclMsg, SI32 id )
{
	cltGameMsgRequest_Event_MaterialComplete *pclInfo = (cltGameMsgRequest_Event_MaterialComplete *)pclMsg->cData;

	if( pclCM->IsValidID(id) == false)			return;

	if( pclInfo->m_siConstructType <= 0)		return;
	if( pclInfo->m_siVillageUnique < 0)			return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar == NULL)							return;

	cltEventStructureUnit* pEventStruct = pclEventStructure->GetEventStructure(pclInfo->m_siConstructType);
	if( pEventStruct == NULL )						return;

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

	cltItem	clRewardItem;
	clRewardItem.Init();

	SI16 itempos = 0;
	cltItem cltoitem;
	SI32 rtnvalue = 0;

	cltoitem.Init();

	if( pEventStruct->m_siRewardItemUnique > 0 && pEventStruct->m_siRewardItemNum > 0 )
	{
		SI16 brareswitch = 0;
		SI32 usedate = 0;
		if(pclClient->pclItemManager->MakeRandItemUnique( pEventStruct->m_siRewardItemUnique, &clRewardItem, 0, 0, &brareswitch,0,0,0,0,0,usedate) == false)	return;

		// 개수를 조정한다. 
		// 중첩 가능한 아이템은 원하는 개수만큼 만들 수 있다. 
		if(clRewardItem.CanPile(pclClient->pclItemManager) == true)		
			clRewardItem.SetItemNum(pEventStruct->m_siRewardItemNum);
		else															
			clRewardItem.SetItemNum(1);

		// 해당아이템을 인벤에 넣을 수 있는지 확인한다. 
		if(pclchar->CanAddInv(INVMODE_ALL, &clRewardItem, &itempos, &cltoitem, &rtnvalue) == false)
		{
			return;
		}
	}

	sDBRequest_Event_MaterialComplete clMsg( id, pclchar->pclCI->GetPersonID(), pclInfo->m_siVillageUnique , pclInfo->m_siConstructType,
											 siItemUnique, siItemNum, itempos, &clRewardItem, clRewardItem.CanPile(pclItemManager) );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DONATEVILLAGEMONEY( cltMsg *pclMsg, SI32 id )
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_DonateVillageMoney *pclInfo = (cltGameMsgRequest_DonateVillageMoney *)pclMsg->cData;

	// 기부할 마을과 위치가 틀리다면 리턴
	SI32 villageUnique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageUnique != pclInfo->siVillageUnique )
		return;

	if( villageUnique < 0 || villageUnique > CUR_GLOBAL_VILLAGE_LIMIT )
		return;

	// 기부할 돈이 내 소지금보다 많다면 리턴
	SI64 donateMoney = pclInfo->siDonateMoney;
	SI64 myMoney = pclCM->CR[id]->pclCI->clIP.GetMoney();
	if( donateMoney > myMoney )
	{
		// 소지금 보다 기부금이 더 많다.
		SendServerResponseMsg(RANKTYPE_CITYHALL, SRVAL_CITYHALL_DONATE_OVERMONEY,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
	}
	

	if( donateMoney < MIN_DONATE_MONEY )
		return;

	SI32 personID = pclCM->CR[id]->pclCI->GetPersonID();

	cltDate clDonateDate;
	clDonateDate.Set( &pclTime->clDate );
	SI32 donateDate = clDonateDate.GetDateVary();

	sDBRequest_DonateVillageMoney clMsg( id, personID , villageUnique, donateDate, pclInfo->siDonateMoney );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

	pclDelMoneyStatistics->Add(DELMONEY_DONATE, pclInfo->siDonateMoney);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DONATEMEMBERLIST( cltMsg *pclMsg, SI32 id )
{
	if( false == pclCM->IsValidID(id) )
		return;

	cltGameMsgRequest_DonateMemberList *pclInfo = (cltGameMsgRequest_DonateMemberList *)pclMsg->cData;

	SI32 villageUnique = pclCM->CR[id]->GetCurrentVillageUnique();
	if( villageUnique < 0 || villageUnique > CUR_GLOBAL_VILLAGE_LIMIT )
		return;

	if( 0 > pclInfo->siRequestPage )
		return;

	sDBRequest_DonateMemberList clMsg( id, villageUnique, pclInfo->siRequestPage );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}




